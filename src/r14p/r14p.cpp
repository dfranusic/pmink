#include <iostream>
#include <r14p.h>
#include <r14p_reg_events.h>
#include <sctp.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include <endian.h>
#include <pmink_utils.h>
#include <errno.h>
#include <time.h>
#include <iomanip>

// HeartbeatInfo
r14p::HeartbeatInfo::HeartbeatInfo(){
    r14pc = NULL;
    interval = 0;
    bzero(target_daemon_id, sizeof(target_daemon_id));
    bzero(target_daemon_type, sizeof(target_daemon_type));
    on_missed = NULL;
    on_received = NULL;
    on_cleanup = NULL;
}

r14p::HeartbeatInfo::~HeartbeatInfo(){
}

void r14p::HeartbeatInfo::set_activity(bool _is_active){
	active.comp_swap(!_is_active, _is_active);

}

void r14p::HeartbeatInfo::set_next(bool _next){
	next.comp_swap(!_next, _next);

}


void r14p::HeartbeatInfo::inc_total_received(){
    total_received_count.fetch_add(1);
}

void r14p::HeartbeatInfo::inc_received(){
    received_count.fetch_add(1);
}

void r14p::HeartbeatInfo::inc_missed(){
    missed_count.fetch_add(1);
}

void r14p::HeartbeatInfo::reset_missed(){
    missed_count.fetch_and(0);
}




void r14p::HeartbeatInfo::inc_total_sent(){
    total_sent_count.fetch_add(1);
}


uint64_t r14p::HeartbeatInfo::get_total_received(){
    return total_received_count.get();

}

uint64_t r14p::HeartbeatInfo::get_received(){
    return received_count.get();
}

uint64_t r14p::HeartbeatInfo::get_missed(){
    return missed_count.get();

}



uint64_t r14p::HeartbeatInfo::get_total_sent(){
    return total_sent_count.get();

}


bool r14p::HeartbeatInfo::is_active(){
    return active.get();

}

bool r14p::HeartbeatInfo::next_ready(){
    return next.get();

}


void* r14p::HeartbeatInfo::heartbeat_loop(void* args){
    if(args != NULL){
	HeartbeatInfo* hi = (HeartbeatInfo*)args;
	unsigned int total_sleep = 0;
	r14p::R14PStream* r14p_stream = NULL;
	R14PCallbackArgs cb_args;

	// stream level heartbeat missed event
	class _tmp_missed: public R14PCallbackMethod {
	public:
	    _tmp_missed(HeartbeatInfo* _hi){
		hi = _hi;
	    }
	    void run(r14p::R14PCallbackArgs* args){
		hi->inc_missed();
		args->add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_HBEAT_INFO, hi);
		if(hi->on_missed != NULL) hi->on_missed->run(args);
		hi->inc_total_received();
		hi->set_next(true);
	    }

	    HeartbeatInfo* hi;
	};

	// stream level heartbeat received event
	class _tmp_recv: public R14PCallbackMethod {
	public:
	    _tmp_recv(HeartbeatInfo* _hi){
		hi = _hi;
	    }
	    void run(r14p::R14PCallbackArgs* args){
		hi->inc_received();
		hi->reset_missed();
		args->add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_HBEAT_INFO, hi);
		if(hi->on_received != NULL) hi->on_received->run(args);
		hi->inc_total_received();
		hi->set_next(true);
	    }

	    HeartbeatInfo* hi;

	};

	// stream level heartbeat sent event
	class _tmp_sent: public R14PCallbackMethod {
	public:
	    _tmp_sent(HeartbeatInfo* _hi){
		hi = _hi;
	    }
	    void run(r14p::R14PCallbackArgs* args){
		hi->inc_total_sent();
	    }

	    HeartbeatInfo* hi;

	};



	// stream level events
	_tmp_missed* tmp_missed = new _tmp_missed(hi);
	_tmp_recv* tmp_recv = new _tmp_recv(hi);
	_tmp_sent* tmp_sent = new _tmp_sent(hi);

	// loop
	while(hi->r14pc->is_active() && hi->is_active()){
	    // sleep 1 sec
	    sleep(1);
	    ++total_sleep;
	    // check if user timeout has been reached
	    if(total_sleep < hi->interval) continue;
	    // reset current timeout
	    total_sleep = 0;
	    // check if new hbeat should be sent
	    if(hi->next_ready() && hi->is_active()){
		// start new R14P stream
		r14p_stream = hi->r14pc->new_stream(hi->target_daemon_type, hi->target_daemon_id, NULL, NULL);
		// check for valid stream
		if(r14p_stream != NULL){
		    // set next flag
		    hi->set_next(false);
		    // set heartbeat reply events on stream level
		    r14p_stream->set_callback(R14P_ET_HEARTBEAT_MISSED, tmp_missed);
		    r14p_stream->set_callback(R14P_ET_HEARTBEAT_RECEIVED, tmp_recv);
		    r14p_stream->set_callback(R14P_ET_STREAM_TIMEOUT, tmp_missed);
		    r14p_stream->set_callback(R14P_ET_PAYLOAD_SENT, tmp_sent);
		    // set sequence flag
		    r14p_stream->set_sequence_flag(R14P_SF_HEARTBEAT);
		    // start stream
		    r14p_stream->send(false);

		}
	    }
	}
	// wait for streams to finish or timeout
	while(!hi->next_ready()) sleep(1);

	// cleanup
	if(hi->on_cleanup != NULL){
	    cb_args.clear_all_args();
	    hi->on_cleanup->run(&cb_args);
	}
	// get r14pc
	R14PClient* tmp_client = hi->r14pc;
	// free
	delete tmp_missed;
	delete tmp_recv;
	delete tmp_sent;
	delete hi;
	// detach
	pthread_detach(pthread_self());
	tmp_client->dec_thread_count();

    }


    return NULL;
}


// R14PCallbackArgs
r14p::R14PCallbackArgs::R14PCallbackArgs(){
}



void r14p::R14PCallbackArgs::clear_all_args(){
    in_args.clear();
    out_args.clear();
}
void r14p::R14PCallbackArgs::clear_args(R14PCBArgsType _args_type){
    switch(_args_type){
	case R14P_CB_INPUT_ARGS: in_args.clear(); break;
	case R14P_CB_OUTPUT_ARGS: out_args.clear(); break;
    }
}
int r14p::R14PCallbackArgs::get_arg_count(R14PCBArgsType _arg_type){
    switch(_arg_type){
	case R14P_CB_INPUT_ARGS: return in_args.size();
	case R14P_CB_OUTPUT_ARGS: return out_args.size();
    }

    return 0;
}

void r14p::R14PCallbackArgs::add_arg(R14PCBArgsType _args_type,  R14PCBArgType _arg_type, void* _arg){
    switch(_args_type){
	case R14P_CB_INPUT_ARGS: in_args[_arg_type] =_arg; break;
	case R14P_CB_OUTPUT_ARGS: out_args[_arg_type] = _arg; break;
    }


}

void* r14p::R14PCallbackArgs::get_arg(R14PCBArgsType _args_type, R14PCBArgType _arg_type){
    switch(_args_type){
	case R14P_CB_INPUT_ARGS:
	    if(in_args.find(_arg_type) != in_args.end()) return in_args[_arg_type]; else return NULL;

	case R14P_CB_OUTPUT_ARGS:
	    if(out_args.find(_arg_type) != out_args.end()) return out_args[_arg_type]; else return NULL;
    }

    return NULL;

}

// R14PCallbackMethod
r14p::R14PCallbackMethod::R14PCallbackMethod(){
    cb_cont = NULL;
}

r14p::R14PCallbackMethod::~R14PCallbackMethod(){}

void r14p::R14PCallbackMethod::run(R14PCallbackArgs* args){}

void r14p::R14PCallbackMethod::cleanup(R14PCallbackArgs* args){}

void r14p::R14PCallbackMethod::set_continue_callback(R14PCallbackMethod* cb){
    cb_cont = cb;
}

void r14p::R14PCallbackMethod::remove_continue_callback(){
    cb_cont = NULL;
}

void r14p::R14PCallbackMethod::run_continue(R14PCallbackArgs* args){
    if(cb_cont != NULL) cb_cont->run(args);
}

// R14PCallbackHandler
r14p::R14PCallbackHandler::R14PCallbackHandler(){

}

r14p::R14PCallbackHandler::~R14PCallbackHandler(){
	callback_map.clear();

}

void r14p::R14PCallbackHandler::set_callback(R14PEventType type, R14PCallbackMethod* method){
    if(method != NULL) callback_map[type] = method;
}

void r14p::R14PCallbackHandler::remove_callback(R14PEventType type){
	callback_map.erase(type);

}

void r14p::R14PCallbackHandler::clear(){
	callback_map.clear();
}

r14p::R14PCallbackMethod* r14p::R14PCallbackHandler::get_callback(R14PEventType type){
	std::map<R14PEventType, R14PCallbackMethod*>::iterator it = callback_map.find(type);
	// check if found
	if(it != callback_map.end()) return it->second;

	return NULL;

}



bool r14p::R14PCallbackHandler::process_callback(R14PEventType type, R14PCallbackArgs* args){
	// find callback
	std::map<R14PEventType, R14PCallbackMethod*>::iterator it = callback_map.find(type);
	// check if found
	if(it != callback_map.end()){
		// run callback
		it->second->run(args);
		return true;
	}
	return false;


}

bool r14p::R14PCallbackHandler::process_cleanup(R14PEventType type, R14PCallbackArgs* args){
	// find callback
	std::map<R14PEventType, R14PCallbackMethod*>::iterator it = callback_map.find(type);
	// check if found
	if(it != callback_map.end()){
		// run callback
		it->second->cleanup(args);
		return true;
	}
	return false;


}



// R14PPayload
r14p::R14PPayload::R14PPayload(){
	raw_data = NULL;
	raw_data_length = 0;
	sctp_sid = 0;
	r14p_stream_type = R14P_ST_UNKNOWN;
	client = NULL;
	stream = NULL;
	free_on_send = true;
}

r14p::R14PPayload::~R14PPayload(){

}


void r14p::R14PPayload::process_callback(R14PEventType type, R14PCallbackArgs* args){
	callback_handler.process_callback(type, args);
}

void r14p::R14PPayload::remove_callback(R14PEventType callback_type){
	callback_handler.remove_callback(callback_type);

}

void r14p::R14PPayload::clear_callbacks(){
	callback_handler.clear();
}

void r14p::R14PPayload::set_callback(R14PEventType callback_type, R14PCallbackMethod* callback_method){
	callback_handler.set_callback(callback_type, callback_method);
}

// R14PStats
r14p::R14PStats::R14PStats(){

}

r14p::R14PStats& r14p::R14PStats::operator=(R14PStats& rhs){
    bytes.set(rhs.bytes.get());
    datagram_bytes.set(rhs.datagram_bytes.get());
    datagram_errors.set(rhs.datagram_errors.get());
    datagrams.set(rhs.datagrams.get());
    discarded.set(rhs.discarded.get());
    malformed.set(rhs.malformed.get());
    packets.set(rhs.packets.get());
    stream_bytes.set(rhs.stream_bytes.get());
    stream_errors.set(rhs.stream_errors.get());
    streams.set(rhs.streams.get());
    socket_errors.set(rhs.socket_errors.get());
    strm_alloc_errors.set(rhs.strm_alloc_errors.get());
    strm_timeout.set(rhs.strm_timeout.get());
    strm_loopback.set(rhs.strm_loopback.get());
    return *this;
}

// R14PStateMachine
r14p::R14PStateMachine::R14PStateMachine(){
    r14pc = NULL;
    poll_timeout = 1000;
    sctp_flags = 0;
    include_body = false;
    mem_switch = false;
    route_c = NULL;
    route_this = false;
    seq_flag_tlv = NULL;
    seq_num_tlv = NULL;
    header = NULL;
    uuid_tlv = NULL;
    memset(d_id, 0, sizeof(d_id));
    memset(d_type, 0, sizeof(d_type));
    sctp_ntf = NULL;
    sctp_assoc = NULL;
    custom_seq_flag = 0;
    tmp_in_session_id = 0;
    sctp_len = 0;
    res = 0;

}

r14p::R14PStateMachine::~R14PStateMachine(){
    root_asn1_node.children.clear();
}

void r14p::R14PStateMachine::init(R14PClient* _r14pc){
    r14pc = _r14pc;
    // monitor POLLIN event
    fds_lst[0].events = POLLIN;
    // update socket in poll structure
    fds_lst[0].fd = r14pc->client_socket;
    // set poll timeout to 5 sec
    poll_timeout = r14pc->poll_interval * 1000;
    sctp_flags = 0;
    include_body = false;
    mem_switch = false;
    route_c = NULL;
    asn1_pool.set_pool_size(1000, 1000);
    asn1_pool.init_pool();
    routes.reserve(100);
    route_this = false;
    seq_flag_tlv = NULL;
    seq_num_tlv = NULL;
    header = NULL;
    uuid_tlv = NULL;
    memset(d_id, 0, sizeof(d_id));
    memset(d_type, 0, sizeof(d_type));

}

void r14p::R14PStateMachine::process_sf_stream_complete(R14PStream* tmp_stream){
    // update timestamp
    tmp_stream->set_timestamp(time(NULL));

    // validate sequence number
    if(r14pc->validate_seq_num(seq_num_tlv->value, seq_num_tlv->value_length, tmp_stream->get_sequence_num())){

	// stats
	r14pc->in_stats.stream_bytes.add_fetch(sctp_len);

    }else{
	// stats
	r14pc->in_stats.stream_errors.add_fetch(1);

    }

    // set stream callback args
    cb_stream_args.clear_all_args();
    cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
    cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_STREAM, tmp_stream);
    cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG, &r14p_in_message);
    cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG_ID, &tmp_in_session_id);
    // R14P_ET_STREAM_NEXT event
    tmp_stream->process_callback(R14P_ET_STREAM_END, &cb_stream_args);

}

void r14p::R14PStateMachine::process_sf_end(R14PStream* tmp_stream, bool remove_stream){
    // validate sequence number
    if(r14pc->validate_seq_num(seq_num_tlv->value, seq_num_tlv->value_length, tmp_stream->get_sequence_num())){

	// stats
	r14pc->in_stats.stream_bytes.add_fetch(sctp_len);
	// update timestamp
	tmp_stream->set_timestamp(time(NULL));
	// set sequence flag
	tmp_stream->set_sequence_flag(R14P_SF_END);
	// toggle sequence received flag
	tmp_stream->toggle_seq_reply_received();

	// set stream callback args
	cb_stream_args.clear_all_args();
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_STREAM, tmp_stream);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG, &r14p_in_message);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG_ID, &tmp_in_session_id);
	// R14P_ET_STREAM_END event
	tmp_stream->process_callback(R14P_ET_STREAM_END, &cb_stream_args);

	// inc sequenuce number
	if(tmp_stream->get_seq_reply_received()) tmp_stream->inc_sequence_num();


	// create payload
	R14PPayload* r14pp = tmp_stream->get_r14p_payload();
	// set free_on_send flag
	r14pp->free_on_send = remove_stream;
	if(!remove_stream) r14pp->out.set(true);

	// set sctp id
	r14pp->sctp_sid = rcvinfo.sinfo_stream;
	// generate STREAM_COMPLETE
	r14pc->generate_stream_complete(	&r14p_in_message,
						tmp_stream->get_r14p_message(),
						tmp_in_session_id,
						1,
						r14pp);


	// set sequence flag
	tmp_stream->set_sequence_flag(R14P_SF_END);

	// remove from list of active streams
	if(remove_stream) r14pc->remove_stream(tmp_stream);

	// send payload
	r14pc->internal_out_queue.push(1, r14pp);


    // sequence error
    }else{
	// stats
	r14pc->in_stats.stream_errors.add_fetch(1);

	// create payload
	R14PPayload* r14pp = tmp_stream->get_r14p_payload();
	// set free_on_send flag
	r14pp->free_on_send = remove_stream;
	if(!remove_stream) r14pp->out.set(true);

	// set sctp sid
	r14pp->sctp_sid = rcvinfo.sinfo_stream;
	// generate ERR
	r14pc->generate_err(	&r14p_in_message,
				tmp_stream->get_r14p_message(),
				tmp_in_session_id,
				1,
				r14pp,
				mem_switch,
				-1,
				NULL,
				NULL,
				asn1::ErrorCode::_err_out_of_sequence);


	// set stream callback args
	cb_stream_args.clear_all_args();
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_STREAM, tmp_stream);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG, &r14p_in_message);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG_ID, &tmp_in_session_id);
	// R14P_ET_STREAM_NEXT event
	tmp_stream->process_callback(R14P_ET_STREAM_END, &cb_stream_args);

	// remove from list of active streams
	if(remove_stream) r14pc->remove_stream(tmp_stream);

	// send payload
	r14pc->internal_out_queue.push(1, r14pp);


    }

}

void r14p::R14PStateMachine::process_sf_continue(R14PStream* tmp_stream, bool remove_stream){
    // validate sequence number
    if(r14pc->validate_seq_num(seq_num_tlv->value, seq_num_tlv->value_length, tmp_stream->get_sequence_num())){

	// stats
	r14pc->in_stats.stream_bytes.add_fetch(sctp_len);

	// update timestamp
	tmp_stream->set_timestamp(time(NULL));
	// toggle sequence received flag
	tmp_stream->toggle_seq_reply_received();

	// reset sequence flag (should be set in event handler, defaults to END)
	tmp_stream->set_sequence_flag(R14P_SF_UNKNOWN);

	// set stream callback args
	cb_stream_args.clear_all_args();
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_STREAM, tmp_stream);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG, &r14p_in_message);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG_ID, &tmp_in_session_id);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_BODY, &include_body);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_MEM_SWITCH, &mem_switch);
	// R14P_ET_STREAM_NEXT event
	tmp_stream->process_callback(R14P_ET_STREAM_NEXT, &cb_stream_args);

	// inc sequenuce number
	if(tmp_stream->get_seq_reply_received()) tmp_stream->inc_sequence_num();

	// create payload
	R14PPayload* r14pp = tmp_stream->get_r14p_payload();
	// set sctp sid
	r14pp->sctp_sid = rcvinfo.sinfo_stream;
	// generate ACK
	r14pc->generate_ack(	&r14p_in_message,
				tmp_stream->get_r14p_message(),
				tmp_in_session_id,
				1,
				r14pp,
				include_body,
				mem_switch);

	// send payload
	r14pp->free_on_send = false;
	r14pp->out.set(true);

	// toggle sequence received flag
	tmp_stream->toggle_seq_reply_received();
	// inc sequenuce number
	if(tmp_stream->get_seq_reply_received()) tmp_stream->inc_sequence_num();

	r14pc->internal_out_queue.push(1, r14pp);



    // sequence error
    }else{
	// stats
	r14pc->in_stats.stream_errors.add_fetch(1);

	// create payload
	R14PPayload* r14pp = tmp_stream->get_r14p_payload();
	// set free_on_send flag
	r14pp->free_on_send = remove_stream;
	if(!remove_stream) r14pp->out.set(true);

	// set sctp sid
	r14pp->sctp_sid = rcvinfo.sinfo_stream;
	// generate ERR
	r14pc->generate_err(	&r14p_in_message,
				tmp_stream->get_r14p_message(),
				tmp_in_session_id,
				1,
				r14pp,
				mem_switch,
				-1,
				NULL,
				NULL,
				asn1::ErrorCode::_err_out_of_sequence);

	// set stream callback args
	cb_stream_args.clear_all_args();
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_STREAM, tmp_stream);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG, &r14p_in_message);
	cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG_ID, &tmp_in_session_id);
	// R14P_ET_STREAM_NEXT event
	tmp_stream->process_callback(R14P_ET_STREAM_END, &cb_stream_args);

	// remove from list of active streams
	if(remove_stream) r14pc->remove_stream(tmp_stream);

	// send payload
	r14pc->internal_out_queue.push(1, r14pp);

    }

}


void r14p::R14PStateMachine::run(){
	// check stream timeout
	r14pc->process_timeout();
	// check if reconnect procedure was quued in TX thread
	// due to socket write error
	if(r14pc->reconnect_queued.get()){
	    // force timeout of all active streams
	    r14pc->process_timeout(true);
	    // init re-connect procedure
	    r14pc->init_reconnect();
	    fds_lst[0].fd = r14pc->client_socket;
	    // reset reconnection flag
	    r14pc->reconnect_queued.comp_swap(true, false);
	}

	// poll socket
	res = poll(fds_lst, 1, poll_timeout);
	// check for timeout
	if(res > 0){
		// check for POLLIN event
		if((fds_lst[0].revents & POLLIN) == POLLIN){
			// receive sctp data chunk
			sctp_len = sctp::rcv_sctp(r14pc->client_socket, tmp_buff, sizeof(tmp_buff), &sctp_flags, &rcvinfo);
			// check for bytes received
			// sctp connection error
			if(sctp_len <= 0){
			    // stats
			    r14pc->in_stats.socket_errors.add_fetch(1);
			    // force timeout of all active streams
			    r14pc->process_timeout(true);
			    // init re-connect procedure
			    r14pc->init_reconnect();
			    fds_lst[0].fd = r14pc->client_socket;

			// process message
			}else{
			    // check if sctp notification
			    if(sctp_flags & MSG_NOTIFICATION){
				// notification pointer
				sctp_ntf = (sctp_notification*)tmp_buff;
				// check type
				switch(sctp_ntf->sn_header.sn_type){
				    // shutdown
				    case SCTP_SHUTDOWN_EVENT:
					// reconnect
					r14pc->reconnect_queued.comp_swap(false, true);
					break;

				    // abort
				    case SCTP_ASSOC_CHANGE:
					sctp_assoc = (sctp_assoc_change*)tmp_buff;
					if(sctp_assoc->sac_state == SCTP_COMM_LOST){
					    // reconnect
					    r14pc->reconnect_queued.comp_swap(false, true);

					}
					break;

				    default:
					break;
				}


			    // check for R14P PPID
			    }else if(be32toh(rcvinfo.sinfo_ppid) == sctp::R14P){
				// stats
				r14pc->in_stats.bytes.add_fetch(sctp_len);
				r14pc->in_stats.packets.add_fetch(1);

				// reset BER nodes
				root_asn1_node.children.clear();
				root_asn1_node.tlv = NULL;

				// next in session id
				tmp_in_session_id = _in_session_id.get_next_id(&r14p_in_message);

				// decode R14P packet
				res = asn1::decode((unsigned char*)tmp_buff, sctp_len, &root_asn1_node, &r14p_in_message, &asn1_pool, &tmp_in_session_id);
				// check for error
				if(res == 0){
					// version check
					if(r14p_in_message._header->_version->linked_node->tlv->value[0] != _R14P_VERSION_){
						// create payload
						R14PStream* r14ps = r14pc->allocate_stream_pool();
						// null check
						if(r14ps == NULL){
						    // stats
						    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
						    // loop end
						    return;
						}
						r14ps->set_timestamp(time(NULL));
						r14ps->clear_callbacks();
						r14ps->linked_stream = NULL;
						R14PPayload* r14pp = r14ps->get_r14p_payload();
						// reset
						r14pp->free_on_send = true;
						r14pp->r14p_stream_type = R14P_ST_STATELESS;
						r14pp->client = r14pc;
						r14pp->sctp_sid = rcvinfo.sinfo_stream;
						r14pp->clear_callbacks();

						// generate response
						r14pc->generate_err(	&r14p_in_message,
									&r14p_out_message,
									tmp_in_session_id,
									1,
									r14pp,
									mem_switch,
									-1,
									NULL,
									NULL,
									asn1::ErrorCode::_err_unsupported_version);

						// send payload
						r14pc->internal_out_queue.push(1, r14pp);

						// next iteration
						return;
					}

					// find route
					r14pc->route(&r14p_in_message, tmp_in_session_id, &routes, d_id, d_type);
					// no routes found
					if(routes.size() == 0){

						// create payload
						R14PStream* r14ps = r14pc->allocate_stream_pool();
						// null check
						if(r14ps == NULL){
						    // stats
						    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
						    // loop end
						    return;
						}
						r14ps->set_timestamp(time(NULL));
						r14ps->clear_callbacks();
						r14ps->linked_stream = NULL;
						R14PPayload* r14pp = r14ps->get_r14p_payload();
						// reset
						r14pp->free_on_send = true;
						r14pp->r14p_stream_type = R14P_ST_STATELESS;
						r14pp->client = r14pc;
						r14pp->sctp_sid = rcvinfo.sinfo_stream;
						r14pp->clear_callbacks();

						// detect custom sequence flag (heartbeat or stream-complete)
						custom_seq_flag = (r14p_in_message._header->_sequence_flag->linked_node->tlv->value[0] == asn1::SequenceFlag::_sf_heartbeat ? asn1::SequenceFlag::_sf_heartbeat : -1);

						// generate response
						r14pc->generate_err(	&r14p_in_message,
									&r14p_out_message,
									tmp_in_session_id,
									1,
									r14pp,
									mem_switch,
									custom_seq_flag,
									r14pc->get_session()->get_daemon_type(),
									r14pc->get_session()->get_daemon_id(),
									asn1::ErrorCode::_err_unknown_route);

						// send payload
						r14pc->internal_out_queue.push(1, r14pp);

						// next iteration
						return;

					}

					// assume no routing to this client
					route_this = false;

					// process routes
					for(unsigned int i = 0; i<routes.size(); i++){
						route_c = routes[i];
						// check if packet needs to be routed to some other client or
						// sent back to sender (sender sending to himself)
						if(route_c != r14pc || strcmp(route_c->get_end_point_daemon_type(), d_type) == 0){
						    // create payload
						    R14PStream* r14ps = route_c->allocate_stream_pool();
						    // null check
						    if(r14ps == NULL){
							// stats
							route_c->out_stats.strm_alloc_errors.add_fetch(1);

						    // ok
						    }else{
							r14ps->set_timestamp(time(NULL));
							r14ps->clear_callbacks();
							r14ps->linked_stream = NULL;
							R14PPayload* r14pp = r14ps->get_r14p_payload();
							// reset
							r14pp->free_on_send = true;
							r14pp->r14p_stream_type = R14P_ST_STATELESS;
							r14pp->client = route_c;
							r14pp->sctp_sid = rcvinfo.sinfo_stream;
							r14pp->clear_callbacks();

							// set new R14P header destination id if destination is final
							if(!route_c->is_router()){
								r14pc->set_destination_id(	&r14p_in_message,
												&r14p_out_message,
												tmp_in_session_id,
												1,
												(unsigned char*)route_c->get_end_point_daemon_id(),
												strlen(route_c->get_end_point_daemon_id()),
												r14pp);

							// in case of router destination, just forward packet
							// and update hop info
							// or return error
							}else{
							    // hop info
							    if(r14pc->update_hop_info(	&r14p_in_message,
											&r14p_out_message,
											tmp_in_session_id,
											1,
											(unsigned char*)route_c->get_end_point_daemon_id(),
											strlen(route_c->get_end_point_daemon_id()),
											r14pp) == 1){

								// detect custom sequence flag (heartbeat or stream-complete)
								custom_seq_flag = (r14p_in_message._header->_sequence_flag->linked_node->tlv->value[0] == asn1::SequenceFlag::_sf_heartbeat ? asn1::SequenceFlag::_sf_heartbeat : -1);

								// generate response
								r14pc->generate_err(	&r14p_in_message,
											&r14p_out_message,
											tmp_in_session_id,
											1,
											r14pp,
											mem_switch,
											custom_seq_flag,
											r14pc->get_session()->get_daemon_type(),
											r14pc->get_session()->get_daemon_id(),
											asn1::ErrorCode::_err_max_hops_exceeded);


							    }

							}

							// send payload
							route_c->push_out_queue(r14pp);

						    }
						    // dec refc counter
						    route_c->dec_refc();


						// route to this client
						}else route_this = true;

					}
					// check if current R14P message has to be routed to this client
					if(!route_this) return;


					// set defaults
					include_body = false;
					mem_switch = false;

					// set tlv pointers
					header = r14p_in_message._header;
					seq_flag_tlv =  header->_sequence_flag->linked_node->tlv;
					seq_num_tlv = header->_sequence_num->linked_node->tlv;
					uuid_tlv = header->_uuid->linked_node->tlv;

                                        // update timestamp
                                        r14pc->timestamp.set(time(NULL));

					// R14P_SF_HEARTBEAT - heartbeat
					if(seq_flag_tlv->value[0] == asn1::SequenceFlag::_sf_heartbeat){

					    // find stream
					    R14PStream* tmp_stream = r14pc->get_stream(uuid_tlv->value);

					    // NULL check, received heartbeat reply
					    if(tmp_stream != NULL){
							// validate seq num
							if(r14pc->validate_seq_num(	seq_num_tlv->value,
											seq_num_tlv->value_length,
											tmp_stream->get_sequence_num())){

							    // stats
							    r14pc->in_stats.streams.add_fetch(1);
							    r14pc->in_stats.stream_bytes.add_fetch(sctp_len);


							    // check heartbeat status
							    if(asn1::node_exists(header->_status, tmp_in_session_id)){

								    // heartbeat error
								    if(header->_status->linked_node->tlv->value[0] != 0){
									// add more callback args
									cb_args.clear_all_args();
									cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
									// R14P_ET_HEARTBEAT_MISSED event
									r14pc->process_callback(R14P_ET_HEARTBEAT_MISSED, &cb_args);
									tmp_stream->process_callback(R14P_ET_HEARTBEAT_MISSED, &cb_args);

								    // heartbeat ok
								    }else{
									// add more callback args
									cb_args.clear_all_args();
									cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
									// R14P_ET_HEARTBEAT_RECEIVED event
									r14pc->process_callback(R14P_ET_HEARTBEAT_RECEIVED, &cb_args);
									tmp_stream->process_callback(R14P_ET_HEARTBEAT_RECEIVED, &cb_args);

								    }

							    }


							    // remove from list of active streams
							    r14pc->remove_stream(tmp_stream);
							    // deallocate stream
							    r14pc->deallocate_stream_pool(tmp_stream);



							}else{
							    // stats
							    r14pc->in_stats.stream_errors.add_fetch(1);

							    // add more callback args
							    cb_args.clear_all_args();
							    cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
							    // R14P_ET_HEARTBEAT_MISSED event
							    r14pc->process_callback(R14P_ET_HEARTBEAT_MISSED, &cb_args);
							    tmp_stream->process_callback(R14P_ET_HEARTBEAT_MISSED, &cb_args);

							    // remove from list of active streams
							    r14pc->remove_stream(tmp_stream);
							    // deallocate stream
							    r14pc->deallocate_stream_pool(tmp_stream);

							}

					    // generate heartbeat reply
					    // hbeat request should not contain status node
					    }else if(!asn1::node_exists(header->_status, tmp_in_session_id)){
						// new stream
						R14PStream* r14ps = r14pc->allocate_stream_pool();
						// null check
						if(r14ps == NULL){
						    // stats
						    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
						    // loop end
						    return;
						}
						// update timestamp
						r14ps->set_timestamp(time(NULL));
						// reset
						r14ps->clear_callbacks();
						r14ps->reset(false);
						r14ps->set_client(r14pc);

						// create payload
						R14PPayload* r14pp = r14ps->get_r14p_payload();
						// reset
						r14pp->free_on_send = true;
						r14pp->r14p_stream_type = R14P_ST_STATEFUL;
						r14pp->client = r14pc;
						r14pp->sctp_sid = rcvinfo.sinfo_stream;
						r14pp->clear_callbacks();

						// set sequence flag
						r14ps->set_sequence_flag(R14P_SF_HEARTBEAT);

						// generate ACK
						r14pc->generate_ack(	&r14p_in_message,
									r14ps->get_r14p_message(),
									tmp_in_session_id,
									1,
									r14pp,
									include_body,
									mem_switch);

						// send payload
						r14pc->internal_out_queue.push(1, r14pp);

					    }

					// R14P_SF_STATELESS - single DATAGRAM
					}else if(seq_flag_tlv->value[0] == asn1::SequenceFlag::_sf_stateless){
						// stats
						r14pc->in_stats.datagrams.add_fetch(1);
						r14pc->in_stats.datagram_bytes.add_fetch(sctp_len);

						// validate sequence number
						if(r14pc->validate_seq_num(seq_num_tlv->value, seq_num_tlv->value_length, 1)){
						    // no error
						    res = asn1::ErrorCode::_err_ok;
						}else{
						    // sequence error
						    res = asn1::ErrorCode::_err_out_of_sequence;

						    // stats
						    r14pc->in_stats.datagram_errors.add_fetch(1);

						}

						// create payload
						R14PStream* r14ps = r14pc->allocate_stream_pool();
						// null check
						if(r14ps == NULL){
						    // stats
						    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
						    // loop end
						    return;
						}
						r14ps->set_timestamp(time(NULL));
						r14ps->clear_callbacks();
						r14ps->linked_stream = NULL;
						R14PPayload* r14pp = r14ps->get_r14p_payload();
						// reset
						r14pp->free_on_send = true;
						r14pp->r14p_stream_type = R14P_ST_STATELESS;
						r14pp->client = r14pc;
						r14pp->sctp_sid = rcvinfo.sinfo_stream;
						r14pp->clear_callbacks();

						// generate response
						r14pc->generate_err(	&r14p_in_message,
									&r14p_out_message,
									tmp_in_session_id,
									1,
									r14pp,
									mem_switch,
									-1,
									NULL,
									NULL,
									res);


						// callback args
						cb_args.clear_all_args();
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG, &r14p_in_message);
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG_ID, &tmp_in_session_id);

						// R14P_ET_DATAGRAM event
						r14pc->process_callback(R14P_ET_DATAGRAM, &cb_args);

						// send payload
						r14pc->internal_out_queue.push(1, r14pp);




					// R14P_SF_START -  multi packet stream start
					}else if(seq_flag_tlv->value[0] == asn1::SequenceFlag::_sf_start){
					    // loopback flag
					    bool loopback = false;
					    // new sequence num must start from 1
					    if(r14pc->validate_seq_num(seq_num_tlv->value, seq_num_tlv->value_length, 1)){
						// stats
						r14pc->in_stats.streams.add_fetch(1);
						r14pc->in_stats.stream_bytes.add_fetch(sctp_len);
						// check if stream exists
						R14PStream* tmp_stream = r14pc->get_stream(uuid_tlv->value);
						// check if stream exists
						if(tmp_stream != NULL){
						    // check if not yet linked and locally initiated
						    if(tmp_stream->linked_stream == NULL && tmp_stream->initiator == R14P_SIT_LOCAL){
							// new stream
							R14PStream* new_stream = r14pc->allocate_stream_pool();
							// null check
							if(new_stream == NULL){
							    // stats
							    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
							    // return, err
							    return;
							}
							// link streams
							tmp_stream->linked_stream = new_stream;
							// set last used
							tmp_stream->last_linked_side = new_stream;
							// switch pointer to new stream
							tmp_stream = new_stream;
							// set flag
							loopback = true;
							// counter
							r14pc->in_stats.strm_loopback.add_fetch(1);

						    // duplicate stream initialized from remote end, error
						    }else return;

						// new stream
						}else tmp_stream = r14pc->allocate_stream_pool();

						// null check
						if(tmp_stream == NULL){
						    // stats
						    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
						    // loop end
						    return;
						}
						// update timestamp
						tmp_stream->set_timestamp(time(NULL));
						// set uuid
						tmp_stream->set_uuid(uuid_tlv->value);
						// reset
						tmp_stream->clear_callbacks();
						tmp_stream->reset(false);
						tmp_stream->set_client(r14pc);
						// add to list of active streams
						if(!loopback) r14pc->add_stream(tmp_stream);

						// create payload
						R14PPayload* r14pp = tmp_stream->get_r14p_payload();
						// reset
						r14pp->free_on_send = false;
						r14pp->out.set(true);
						r14pp->r14p_stream_type = R14P_ST_STATEFUL;
						r14pp->client = r14pc;
						r14pp->sctp_sid = rcvinfo.sinfo_stream;
						r14pp->clear_callbacks();

						// reset sequence flag (should be set in event handler, defaults to END)
						tmp_stream->set_sequence_flag(R14P_SF_UNKNOWN);

						// add more callback args
						cb_args.clear_all_args();
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG, &r14p_in_message);
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG_ID, &tmp_in_session_id);
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_BODY, &include_body);
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_MEM_SWITCH, &mem_switch);
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_STREAM, tmp_stream);

						// R14P_ET_STREAM_NEW event
						r14pc->process_callback(R14P_ET_STREAM_NEW, &cb_args);

						// generate ACK
						r14pc->generate_ack(	&r14p_in_message,
									tmp_stream->get_r14p_message(),
									tmp_in_session_id,
									1,
									r14pp,
									include_body,
									mem_switch);

						// toggle sequence received flag
						tmp_stream->toggle_seq_reply_received();
						// inc sequence number
						tmp_stream->inc_sequence_num();

						// send payload
						r14pc->internal_out_queue.push(1, r14pp);




					    // sequence error
					    }else{
						// stats
						r14pc->in_stats.stream_errors.add_fetch(1);

						// create payload
						R14PStream* r14ps = r14pc->allocate_stream_pool();
						// null check
						if(r14ps == NULL){
						    // stats
						    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
						    // loop end
						    return;
						}
						R14PPayload* r14pp = r14ps->get_r14p_payload();
						r14ps->clear_callbacks();
						r14ps->linked_stream = NULL;
						// reset
						r14pp->free_on_send = true;
						r14pp->client = r14pc;
						r14pp->sctp_sid = rcvinfo.sinfo_stream;
						r14pp->clear_callbacks();

						// generate ERR
						r14pc->generate_err(	&r14p_in_message,
									&r14p_out_message,
									tmp_in_session_id,
									1,
									r14pp,
									mem_switch,
									-1,
									NULL,
									NULL,
									asn1::ErrorCode::_err_out_of_sequence);

						// send payload
						r14pc->internal_out_queue.push(1, r14pp);

					    }


					// R14P_SF_CONTINUE - multi packet stream part
					}else if(seq_flag_tlv->value[0] == asn1::SequenceFlag::_sf_continue){
					    // find stream
					    R14PStream* tmp_stream = r14pc->get_stream(uuid_tlv->value);
					    if(tmp_stream != NULL){
						if(tmp_stream->linked_stream != NULL){
						    // check which side needs to process sf_continue

						    // sender part
						    if(tmp_stream->last_linked_side == tmp_stream){
							// process
							process_sf_continue(tmp_stream->linked_stream, false);
							// set last used
							tmp_stream->last_linked_side = tmp_stream->linked_stream;

						    // linked part
						    }else{
							// process
							process_sf_continue(tmp_stream, false);
							// set last used
							tmp_stream->last_linked_side = tmp_stream;

						    }


						// no linked stream, process
						}else process_sf_continue(tmp_stream, true);


					    // unknown stream
					    }else{
						// stats
						r14pc->in_stats.stream_errors.add_fetch(1);

						// create payload
						R14PStream* r14ps = r14pc->allocate_stream_pool();
						// null check
						if(r14ps == NULL){
						    // stats
						    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
						    // loop end
						    return;
						}
						R14PPayload* r14pp = r14ps->get_r14p_payload();
						r14ps->clear_callbacks();
						r14ps->linked_stream = NULL;
						// reset
						r14pp->free_on_send = true;
						r14pp->r14p_stream_type = R14P_ST_UNKNOWN;
						r14pp->client = r14pc;
						r14pp->sctp_sid = rcvinfo.sinfo_stream;
						r14pp->clear_callbacks();
						// generate ERR
						r14pc->generate_err(	&r14p_in_message,
									&r14p_out_message,
									tmp_in_session_id,
									1,
									r14pp,
									mem_switch,
									-1,
									NULL,
									NULL,
									asn1::ErrorCode::_err_unknown_sequence);

						// send payload
						r14pc->internal_out_queue.push(1, r14pp);

					    }


					// R14P_SF_CONTINUE_WAIT - multi packet stream part
					// do nothing, expect another SF-CONTINUE from peer
					}else if(seq_flag_tlv->value[0] == asn1::SequenceFlag::_sf_continue_wait){
					    // find stream
					    R14PStream* tmp_stream = r14pc->get_stream(uuid_tlv->value);
					    // NULL check
					    if(tmp_stream != NULL){
						    // validate sequence number
						    if(r14pc->validate_seq_num(seq_num_tlv->value, seq_num_tlv->value_length, tmp_stream->get_sequence_num())){

							// stats
							r14pc->in_stats.stream_bytes.add_fetch(sctp_len);

							// update timestamp
							tmp_stream->set_timestamp(time(NULL));

							// set sequence flag
							tmp_stream->set_sequence_flag(R14P_SF_CONTINUE_WAIT);

							// do nothing

						    // sequence error
						    }else{
							// stats
							r14pc->in_stats.stream_errors.add_fetch(1);

							// create payload
							R14PPayload* r14pp = tmp_stream->get_r14p_payload();
							tmp_stream->linked_stream = NULL;
							// set free_on_send flag
							r14pp->free_on_send = true;
							// set sctp sid
							r14pp->sctp_sid = rcvinfo.sinfo_stream;
							// generate ERR
							r14pc->generate_err(	&r14p_in_message,
										tmp_stream->get_r14p_message(),
										tmp_in_session_id,
										1,
										r14pp,
										mem_switch,
										-1,
										NULL,
										NULL,
										asn1::ErrorCode::_err_out_of_sequence);


							// set stream callback args
							cb_stream_args.clear_all_args();
							cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
							cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_STREAM, tmp_stream);
							cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG, &r14p_in_message);
							cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG_ID, &tmp_in_session_id);
							// R14P_ET_STREAM_NEXT event
							tmp_stream->process_callback(R14P_ET_STREAM_END, &cb_stream_args);

							// remove from list of active streams
							r14pc->remove_stream(tmp_stream);

							// send payload
							r14pc->internal_out_queue.push(1, r14pp);

						    }
					    // unknown stream
					    }else{
						// stats
						r14pc->in_stats.stream_errors.add_fetch(1);

						// create payload
						R14PStream* r14ps = r14pc->allocate_stream_pool();
						// null check
						if(r14ps == NULL){
						    // stats
						    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
						    // loop end
						    return;
						}
						R14PPayload* r14pp = r14ps->get_r14p_payload();
						r14ps->clear_callbacks();
						r14ps->linked_stream = NULL;
						// reset
						r14pp->free_on_send = true;
						r14pp->r14p_stream_type = R14P_ST_UNKNOWN;
						r14pp->client = r14pc;
						r14pp->sctp_sid = rcvinfo.sinfo_stream;
						r14pp->clear_callbacks();
						// generate ERR
						r14pc->generate_err(	&r14p_in_message,
									&r14p_out_message,
									tmp_in_session_id,
									1,
									r14pp,
									mem_switch,
									-1,
									NULL,
									NULL,
									asn1::ErrorCode::_err_unknown_sequence);

						// send payload
						r14pc->internal_out_queue.push(1, r14pp);

					    }

					// R14P_SF_END - stream ending
					}else if(seq_flag_tlv->value[0] == asn1::SequenceFlag::_sf_end){
					    // find stream
					    R14PStream* tmp_stream = r14pc->get_stream(uuid_tlv->value);
					    // NULL check
					    if(tmp_stream != NULL){
						if(tmp_stream->linked_stream != NULL){
						    // check which side needs to process sf_end

						    // sender part
						    if(tmp_stream->last_linked_side == tmp_stream){
							// process
							process_sf_end(tmp_stream->linked_stream, false);
							// set last used
							tmp_stream->last_linked_side = tmp_stream->linked_stream;

						    // linked part
						    }else{
							// process
							process_sf_end(tmp_stream, false);
							// set last used
							tmp_stream->last_linked_side = tmp_stream;

						    }
						// no linked stream, process
						}else process_sf_end(tmp_stream, true);

					    // unknown stream
					    }else{
						// stats
						r14pc->in_stats.stream_errors.add_fetch(1);

						// create payload
						R14PStream* r14ps = r14pc->allocate_stream_pool();
						// null check
						if(r14ps == NULL){
						    // stats
						    r14pc->in_stats.strm_alloc_errors.add_fetch(1);
						    // loop end
						    return;
						}
						R14PPayload* r14pp = r14ps->get_r14p_payload();
						r14ps->clear_callbacks();
						r14ps->linked_stream = NULL;
						// reset
						r14pp->free_on_send = true;
						r14pp->r14p_stream_type = R14P_ST_UNKNOWN;
						//r14pp->stream = NULL;
						r14pp->client = r14pc;
						r14pp->sctp_sid = rcvinfo.sinfo_stream;
						r14pp->clear_callbacks();
						// generate ERR
						r14pc->generate_err(	&r14p_in_message,
									&r14p_out_message,
									tmp_in_session_id,
									1,
									r14pp,
									mem_switch,
									-1,
									NULL,
									NULL,
									asn1::ErrorCode::_err_unknown_sequence);

						// send payload
						r14pc->internal_out_queue.push(1, r14pp);

					    }
					// stream finished
					}else if(seq_flag_tlv->value[0] == asn1::SequenceFlag::_sf_stream_complete){
					    // find stream
					    R14PStream* tmp_stream = r14pc->get_stream(uuid_tlv->value);
					    // NULL check
					    if(tmp_stream != NULL){

						if(tmp_stream->linked_stream != NULL){
						    // check which side needs to process sf_end

						    // sender part
						    if(tmp_stream->last_linked_side == tmp_stream){
							// process
							process_sf_stream_complete(tmp_stream->linked_stream);
							// set last used
							tmp_stream->last_linked_side = tmp_stream->linked_stream;

						    // linked part
						    }else{
							// process
							process_sf_stream_complete(tmp_stream);
							// set last used
							tmp_stream->last_linked_side = tmp_stream;

						    }
						// no linked stream, process
						}else process_sf_stream_complete(tmp_stream);

						// remove from list of active streams
						r14pc->remove_stream(tmp_stream);
						// check linked
						if(tmp_stream->linked_stream != NULL){
						    r14pc->remove_stream(tmp_stream->linked_stream);
						    r14pc->deallocate_stream_pool(tmp_stream->linked_stream);
						}
						// deallocate stream
						r14pc->deallocate_stream_pool(tmp_stream);


					    }

					// R14P_ST_STATELESS_NO_REPLY - single DATAGRAM without reply
					}else if(seq_flag_tlv->value[0] == asn1::SequenceFlag::_sf_stateless_no_reply){
						// stats
						r14pc->in_stats.datagram_bytes.add_fetch(sctp_len);
						r14pc->in_stats.datagrams.add_fetch(1);

						// do nothing for now, reliability is SCTP dependent

						// callback args
						cb_args.clear_all_args();
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG, &r14p_in_message);
						cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_IN_MSG_ID, &tmp_in_session_id);

						// R14P_ET_DATAGRAM event
						r14pc->process_callback(R14P_ET_DATAGRAM, &cb_args);

					}


				}else{
				    // stats
				    r14pc->in_stats.malformed.add_fetch(1);
				}
			    }else{
				// stats
				r14pc->in_stats.discarded.add_fetch(1);
			    }
			}

		}else if(fds_lst[0].revents != 0){
		    // force timeout of all active streams
		    r14pc->process_timeout(true);
		    // init re-connect procedure
		    r14pc->init_reconnect();
		    fds_lst[0].fd = r14pc->client_socket;


		}
	// socket idle
	}else if(res == 0){
	    // add more callback args
	    cb_args.clear_all_args();
	    cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
	    // R14P_ET_STREAM_NEW event
	    r14pc->process_callback(R14P_ET_CLIENT_IDLE, &cb_args);

	// socket error
	}else{
	    // stats
	    r14pc->in_stats.socket_errors.add_fetch(1);
	    // force timeout of all active streams
	    r14pc->process_timeout(true);
	    // init re-connect procedure
	    r14pc->init_reconnect();
	    fds_lst[0].fd = r14pc->client_socket;
	}

}

// R14PClient
void r14p::R14PClient::init(){
	memset(end_point_address, 0, sizeof(end_point_address));
	memset(local_point_address, 0, sizeof(local_point_address));
	bzero(end_point_daemon_id, sizeof(end_point_daemon_id));
	bzero(end_point_daemon_type, sizeof(end_point_daemon_type));

	client_id = -1;
	client_socket = -1;
	router = false;
	in_thread = 0;
	out_thread = 0;
	timeout_thread = 0;
	exit_thread = 0;
	end_point_port = 0;
	local_point_port = 0;
	direction = R14P_CD_UNKNOWN;
	session = NULL;
	streams.reserve(max_concurrent_streams);

	// mutexes
	pthread_mutex_init(&mtx_streams, NULL);
	pthread_spin_init(&slock_callback, 0);
	pthread_spin_init(&slock_uuid, 0);

	// random generator
	timespec tmp_time;
	clock_gettime(0, &tmp_time);
	ran_mt19937.seed(tmp_time.tv_nsec + tmp_time.tv_sec);
	random_generator = new boost::uuids::random_generator(ran_mt19937);


	// queues
	out_queue.set_capacity(max_concurrent_streams);
	internal_out_queue.init(max_concurrent_streams);

	// memory pools
	mc_pool.init(max_concurrent_streams);
	mc_pool.construct_objects();

	pld_pool.init(max_concurrent_streams);
	pld_pool.construct_objects();

	r14pm_pool.init(max_concurrent_streams);
	r14pm_pool.construct_objects();

	stream_pool.init(max_concurrent_streams);
	stream_pool.construct_objects();

	// allocate raw payload buffers for R14PPayload objects
	R14PPayload* tmp_pld[pld_pool.get_chunk_count()];
	for(int i = 0; i<pld_pool.get_chunk_count(); i++){
		tmp_pld[i] = pld_pool.allocate_constructed();
		tmp_pld[i]->raw_data = mc_pool.allocate_constructed()->buffer;
	}
	for(int i = 0; i<pld_pool.get_chunk_count(); i++) pld_pool.deallocate_constructed(tmp_pld[i]);


	// set random generator, pld and msg for R14PStream objects
	R14PStream* tmp_stream[stream_pool.get_chunk_count()];
	for(int i = 0; i<stream_pool.get_chunk_count(); i++){
		tmp_stream[i] = stream_pool.allocate_constructed();
		tmp_stream[i]->set_random_generator(random_generator);
		tmp_stream[i]->set_client(this);
		tmp_stream[i]->set_r14p_message(r14pm_pool.allocate_constructed());
		tmp_stream[i]->set_r14p_payload(pld_pool.allocate_constructed());
		tmp_stream[i]->get_r14p_payload()->stream = tmp_stream[i];
	}
	for(int i = 0; i<stream_pool.get_chunk_count(); i++) stream_pool.deallocate_constructed(tmp_stream[i]);


}

r14p::R14PClient::R14PClient(){
	max_concurrent_streams = 100;

	// main init
	init();

}

r14p::R14PClient::R14PClient(	int _client_socket,
				const char* _end_point_address,
				unsigned int _end_point_port,
				const char* _local_point_address,
				unsigned int _local_point_port,
				R14PConnectionDirection _direction,
				int _max_concurrent_streams,
				int _stream_timeout,
				int _poll_interval){

	max_concurrent_streams = _max_concurrent_streams;
	stream_timeout = _stream_timeout;

	ref_counter.set(1);

	// main init
	init();

	// connection params
	direction = _direction;
	client_socket = _client_socket;
	poll_interval = _poll_interval;
	client_id = _local_point_port;
	strcpy(end_point_address, _end_point_address);
	strcpy(local_point_address, _local_point_address);
	end_point_port = _end_point_port;
	local_point_port = _local_point_port;

	// set as active
	set_activity(true);

}

r14p::R14PClient::~R14PClient(){
	// set as inactive
	set_activity(false);
	// wait for threads to fnish
	timespec st = {0, 100000000};
	while(get_thread_count() > 0){
	    nanosleep(&st, NULL);
	}

	// disconnect just in case
	if(client_socket > 0) disconnect();


	// deallocate active streams
	for(unsigned int i = 0; i<streams.size(); i++) if(streams[i] != NULL){
	    deallocate_stream_pool(streams[i]);
	}
	// clear active stream list
	streams.clear();

	// deallocate extra stream memory
	R14PStream* tmp_stream[stream_pool.get_chunk_count()];
	for(int i = 0; i<stream_pool.get_chunk_count(); i++){
		tmp_stream[i] = stream_pool.allocate_constructed();
		// sanity check
		if(tmp_stream[i] == NULL) continue;
		// * deallocate_mc_pool expects a pointer to MemChunk
		// * buffer is the first field in MemChunk class so both MemChunk class and MemChunk.buffer field share the same address
		// * this makes type casting the MemChunk.raw_data to MemChunk valid
		deallocate_mc_pool((memory::MemChunk<1024>*)tmp_stream[i]->get_r14p_payload()->raw_data);
		deallocate_pld_pool(tmp_stream[i]->get_r14p_payload());
		deallocate_r14pm_pool(tmp_stream[i]->get_r14p_message());
	}
	for(int i = 0; i<stream_pool.get_chunk_count(); i++) stream_pool.deallocate_constructed(tmp_stream[i]);


	// random generator
	delete random_generator;

	// destory mutexes
	pthread_mutex_destroy(&mtx_streams);
	pthread_spin_destroy(&slock_callback);
	pthread_spin_destroy(&slock_uuid);
}


uint32_t r14p::R14PClient::inc_refc(){
    return ref_counter.add_fetch(1);
}

uint32_t r14p::R14PClient::dec_refc(){
    return ref_counter.sub_fetch(1);
}

uint32_t r14p::R14PClient::get_refc(){
    return ref_counter.get();
}


int r14p::R14PClient::deallocate_mc_pool(memory::MemChunk<1024>* mem_chunk){
	if(mem_chunk != NULL){
		int res = mc_pool.deallocate_constructed(mem_chunk);
		return res;

	}
	return -1;

}

memory::MemChunk<1024>* r14p::R14PClient::allocate_mc_pool(){
	memory::MemChunk<1024>* tmp = mc_pool.allocate_constructed();
	return tmp;
}



int r14p::R14PClient::deallocate_pld_pool(R14PPayload* r14ppld){
	if(r14ppld != NULL){
		int res = pld_pool.deallocate_constructed(r14ppld);
		return res;

	}
	return -1;

}

r14p::R14PPayload* r14p::R14PClient::allocate_pld_pool(){
	R14PPayload* tmp = pld_pool.allocate_constructed();
	return tmp;
}



int r14p::R14PClient::deallocate_r14pm_pool(asn1::R14PMessage* r14pm){
	if(r14pm != NULL){
		int res = r14pm_pool.deallocate_constructed(r14pm);
		return res;

	}
	return -1;

}

asn1::R14PMessage* r14p::R14PClient::allocate_r14pm_pool(){
	asn1::R14PMessage* tmp = r14pm_pool.allocate_constructed();
	return tmp;
}

int r14p::R14PClient::deallocate_stream_pool(R14PStream* stream){
	if(stream != NULL){
		int res = stream_pool.deallocate_constructed(stream);
		return res;

	}
	return -1;

}

r14p::R14PStream* r14p::R14PClient::allocate_stream_pool(){
    return stream_pool.allocate_constructed();
}

int r14p::R14PClient::push_out_queue(R14PPayload* payload){
	if(payload != NULL){
		bool res = out_queue.push(payload);
		return !res;

	}

	return -1;
}
r14p::R14PPayload* r14p::R14PClient::pop_out_queue(){
    R14PPayload* tmp = NULL;
    out_queue.pop(&tmp);
    return tmp;

}

int r14p::R14PClient::generate_uuid(unsigned char* out){
    if(out == NULL) return 1;
    pthread_spin_lock(&slock_uuid);
    boost::uuids::uuid _uuid = (*random_generator)(); // operator ()
    memcpy(out, _uuid.data, 16);
    pthread_spin_unlock(&slock_uuid);
    return 0;
}


uint8_t r14p::R14PClient::is_active(){
    return active.get();

}

void r14p::R14PClient::set_activity(bool _is_active){
    active.comp_swap(!_is_active, _is_active);

}

int r14p::R14PClient::get_client_id(){
    return client_id;
}

int r14p::R14PClient::get_client_socket(){
	return client_socket;

}
char* r14p::R14PClient::get_end_point_address(){
	return end_point_address;
}

unsigned int r14p::R14PClient::get_end_point_port(){
	return end_point_port;
}

char* r14p::R14PClient::get_local_point_address(){
	return local_point_address;
}

unsigned int r14p::R14PClient::get_local_point_port(){
	return local_point_port;
}

void r14p::R14PClient::set_router_flag(bool _is_router){
    router = _is_router;
}

bool r14p::R14PClient::is_router(){
    return router;
}



int r14p::R14PClient::send(unsigned int sctp_stream_id, const unsigned char* data, unsigned int data_length){
	if(data != NULL){
		return sctp::send_sctp(client_socket, data, data_length, sctp::R14P, sctp_stream_id);
	}
	// err
	return -1;
}

int r14p::R14PClient::send_datagram(	asn1::Body* body,
					R14PCallbackMethod* on_sent_callback_method,
					R14PCallbackMethod* on_reply_callback_method,
					const char* dest_daemon_type,
					const char* dest_daemon_id){


	R14PStream* r14ps = allocate_stream_pool();
	// null check
	if(r14ps == NULL) return 1;
	R14PPayload* r14pp = r14ps->get_r14p_payload();
	asn1::R14PMessage* r14p_out_message = r14ps->get_r14p_message();
	r14ps->clear_callbacks();
	r14ps->reset(true);


	// next session id
	uint64_t tmp_session_id = 1;
	bool prepare_needed = false;

	// set optional
	// source id
	if(r14p_out_message->_header->_source->_id == NULL) {
	    r14p_out_message->_header->_source->set_id();
	    prepare_needed = true;
	}

	// destination id
	if(r14p_out_message->_header->_destination->_id == NULL) {
	    r14p_out_message->_header->_destination->set_id();
	    prepare_needed = true;
	}

	// body
	if(r14p_out_message->_body == NULL) {
	    r14p_out_message->set_body();
	    prepare_needed = true;
	}else{
	    r14p_out_message->_body->unlink(tmp_session_id);

	}



	// prepare only if one of optional fields was not set
	if(prepare_needed) r14p_out_message->prepare();

	// insert body
	r14p_out_message->_body->choice_selection = body->choice_selection;
	r14p_out_message->_body->choice_selection->parent_node = r14p_out_message->_body;
	// reset old_value_flag to recalculate parent nodes
	r14p_out_message->_body->choice_selection->linked_node->tlv->old_value_length = 0;
	// recalculate choice selection and update parent nodes
	r14p_out_message->_body->choice_selection->set_linked_data(tmp_session_id);

	// unlink status if exists
	if(r14p_out_message->_header->_status != NULL) r14p_out_message->_header->_status->unlink(tmp_session_id);

	// header
	int ver = _R14P_VERSION_;
	r14p_out_message->_header->_version->set_linked_data(tmp_session_id, (unsigned char*)&ver, 1);
	r14p_out_message->_header->_source->_id->set_linked_data(tmp_session_id, (unsigned char*)session->get_daemon_id(), strlen(session->get_daemon_id()));
	r14p_out_message->_header->_source->_type->set_linked_data(tmp_session_id, (unsigned char*)session->get_daemon_type(), strlen(session->get_daemon_type()));
	r14p_out_message->_header->_destination->_id->set_linked_data(tmp_session_id, (unsigned char*)dest_daemon_id, strlen(dest_daemon_id));
	r14p_out_message->_header->_destination->_type->set_linked_data(tmp_session_id, (unsigned char*)dest_daemon_type, strlen(dest_daemon_type));
	r14p_out_message->_header->_uuid->set_linked_data(tmp_session_id, r14ps->get_uuid(), 16);
	uint32_t seq_num = htobe32(1);
	r14p_out_message->_header->_sequence_num->set_linked_data(tmp_session_id, (unsigned char*)&seq_num, sizeof(uint32_t));

	// check if waiting for reply
	// do not wait for reply
	if(on_reply_callback_method == NULL){
	    //deallocate_stream_pool(r14ps);
	    r14pp->free_on_send = true;
	    int sf = asn1::SequenceFlag::_sf_stateless_no_reply;
	    r14pp->r14p_stream_type = R14P_ST_STATELESS_NO_REPLY;
	    r14p_out_message->_header->_sequence_flag->set_linked_data(tmp_session_id, (unsigned char*)&sf, 1);
	// wait for reply
	}else{
	    int sf = asn1::SequenceFlag::_sf_stateless;
	    r14p_out_message->_header->_sequence_flag->set_linked_data(tmp_session_id, (unsigned char*)&sf, 1);
	    r14pp->free_on_send = false;
	    r14pp->sctp_sid = 0;
	    r14pp->r14p_stream_type = R14P_ST_STATELESS;
	    r14ps->set_callback(R14P_ET_STREAM_END, on_reply_callback_method);
	    r14ps->set_sequence_flag(R14P_SF_STATELESS);
	    add_stream(r14ps);
	}

	r14pp->raw_data_length = asn1::encode(r14pp->raw_data, 1024, r14p_out_message, tmp_session_id);
	r14pp->client = this;
	r14pp->clear_callbacks();
	r14pp->stream->set_callback(R14P_ET_PAYLOAD_SENT, on_sent_callback_method);
	// send to queue
	push_out_queue(r14pp);


	return 0;

}



int r14p::R14PClient::send_datagram(	int payload_type,
					unsigned char* payload,
					int payload_length,
					R14PCallbackMethod* on_sent_callback_method,
					R14PCallbackMethod* on_reply_callback_method,
					const char* dest_daemon_type,
					const char* dest_daemon_id){

	R14PStream* r14ps = allocate_stream_pool();
	// null check
	if(r14ps == NULL) return 1;

	R14PPayload* r14pp = r14ps->get_r14p_payload();
	asn1::R14PMessage* r14p_out_message = r14ps->get_r14p_message();
	r14ps->clear_callbacks();
	r14ps->reset(true);


	// next session id
	uint64_t tmp_session_id = 1;//get_next_out_session_id(r14p_out_message);
	bool prepare_needed = false;

	// set optional
	// source id
	if(r14p_out_message->_header->_source->_id == NULL) {
	    r14p_out_message->_header->_source->set_id();
	    prepare_needed = true;
	}

	// destination id
	if(r14p_out_message->_header->_destination->_id == NULL) {
	    r14p_out_message->_header->_destination->set_id();
	    prepare_needed = true;
	}

	// body
	if(r14p_out_message->_body == NULL) {
	    r14p_out_message->set_body();
	    prepare_needed = true;
	}else{
	    r14p_out_message->_body->unlink(tmp_session_id);
	    r14p_out_message->_body->_data->set_linked_data(tmp_session_id);
	}

	// data payload
	if(r14p_out_message->_body->_data->_payload == NULL) {
	    r14p_out_message->_body->_data->set_payload();
	    prepare_needed = true;
	}
	// prepare only if one of optional fields was not set
	if(prepare_needed) r14p_out_message->prepare();



	// unlink status if exists
	if(r14p_out_message->_header->_status != NULL) r14p_out_message->_header->_status->unlink(tmp_session_id);

	// header
	int ver = _R14P_VERSION_;
	r14p_out_message->_header->_version->set_linked_data(tmp_session_id, (unsigned char*)&ver, 1);
	r14p_out_message->_header->_source->_id->set_linked_data(tmp_session_id, (unsigned char*)session->get_daemon_id(), strlen(session->get_daemon_id()));
	r14p_out_message->_header->_source->_type->set_linked_data(tmp_session_id, (unsigned char*)session->get_daemon_type(), strlen(session->get_daemon_type()));
	r14p_out_message->_header->_destination->_id->set_linked_data(tmp_session_id, (unsigned char*)dest_daemon_id, strlen(dest_daemon_id));
	r14p_out_message->_header->_destination->_type->set_linked_data(tmp_session_id, (unsigned char*)dest_daemon_type, strlen(dest_daemon_type));

	r14p_out_message->_header->_uuid->set_linked_data(tmp_session_id, r14ps->get_uuid(), 16);

	uint32_t seq_num = htobe32(1);
	r14p_out_message->_header->_sequence_num->set_linked_data(tmp_session_id, (unsigned char*)&seq_num, sizeof(uint32_t));

	// body
	uint16_t tmp_i = htobe16(payload_type);
	r14p_out_message->_body->_data->_payload_type->set_linked_data(tmp_session_id, (unsigned char*)&tmp_i, 1);
	r14p_out_message->_body->_data->_payload->set_linked_data(tmp_session_id, payload, payload_length);







	// r14p payload
	// check if waiting for reply
	// do not wait for reply
	if(on_reply_callback_method == NULL){
	    r14pp->free_on_send = true;
	    int sf = asn1::SequenceFlag::_sf_stateless_no_reply;
	    r14pp->r14p_stream_type = R14P_ST_STATELESS_NO_REPLY;
	    r14p_out_message->_header->_sequence_flag->set_linked_data(tmp_session_id, (unsigned char*)&sf, 1);
	// wait for reply
	}else{
	    int sf = asn1::SequenceFlag::_sf_stateless;
	    r14p_out_message->_header->_sequence_flag->set_linked_data(tmp_session_id, (unsigned char*)&sf, 1);

	    r14pp->free_on_send = false;
	    r14pp->sctp_sid = 0;
	    r14pp->r14p_stream_type = R14P_ST_STATELESS;
	    r14ps->set_callback(R14P_ET_STREAM_END, on_reply_callback_method);
	    r14ps->set_sequence_flag(R14P_SF_STATELESS);
	    add_stream(r14ps);
	}

	r14pp->raw_data_length = asn1::encode(r14pp->raw_data, 1024, r14p_out_message, tmp_session_id);
	r14pp->client = this;
	r14pp->clear_callbacks();
	r14pp->stream->set_callback(R14P_ET_PAYLOAD_SENT, on_sent_callback_method);

	// send to queue
	push_out_queue(r14pp);


	return 0;
}



void r14p::R14PStream::set_r14p_payload(R14PPayload* _r14p_payload){
    r14p_payload = _r14p_payload;
}


r14p::R14PClient* r14p::R14PStream::get_client(){
    return client;
}


r14p::R14PPayload* r14p::R14PStream::get_r14p_payload(){
    return r14p_payload;
}

asn1::R14PMessage* r14p::R14PStream::get_r14p_message(){
    return r14p_message;
}

void r14p::R14PStream::set_r14p_message(asn1::R14PMessage* _r14p_message){
    r14p_message = _r14p_message;
}

void r14p::R14PStream::set_destination(const char* _dest_type, const char* _dest_id){
    if(strlen(_dest_type) >= sizeof(destination_type)) {
	memcpy(destination_type, _dest_type, sizeof(destination_type) - 1);
	destination_type[sizeof(destination_type) - 1] = 0;
    }else memcpy(destination_type, _dest_type, strlen(_dest_type) + 1);

    if(_dest_id != NULL) {
	if(strlen(_dest_id) >= sizeof(destination_id)) {
	    memcpy(destination_id, _dest_id, sizeof(destination_id) - 1);
	    destination_id[sizeof(destination_id) - 1] = 0;
	}else memcpy(destination_id, _dest_id, strlen(_dest_id) + 1);
    }

}


r14p::R14PStream* r14p::R14PClient::new_stream(const char* _dest_type,
					       const char* _dest_id,
					       R14PCallbackMethod* _on_sent_callback,
					       R14PCallbackMethod* _on_reply_callback){
    if(_dest_type == NULL) return NULL;
    R14PStream* stream = allocate_stream_pool();
    if(stream != NULL/* && r14pm != NULL*/){
	    stream->set_client(this);
	    stream->reset(true);
	    stream->clear_callbacks();
	    stream->set_destination(_dest_type, _dest_id);
	    stream->set_callback(R14P_ET_PAYLOAD_SENT, _on_sent_callback);
	    stream->set_callback(R14P_ET_STREAM_NEXT, _on_reply_callback);
	    add_stream(stream);
	    return stream;


    }
    return NULL;
}

r14p::R14PStream* r14p::R14PClient::create_stream(){
    R14PStream* stream = allocate_stream_pool();
    if(stream == NULL) return NULL;
    stream->set_client(this);
    return stream;
}

void r14p::R14PClient::add_stream(r14p::R14PStream* _stream){
	pthread_mutex_lock(&mtx_streams);
	streams.push_back(_stream);
        streams_active.set(true);
	pthread_mutex_unlock(&mtx_streams);

}

bool r14p::R14PClient::stream_exists(R14PStream* _stream){
	pthread_mutex_lock(&mtx_streams);
	for(unsigned int i = 0; i<streams.size(); i++) if(streams[i] == _stream) {
	    pthread_mutex_unlock(&mtx_streams);
	    return true;
	}
	pthread_mutex_unlock(&mtx_streams);
	return false;
}



void r14p::R14PClient::remove_stream(r14p::R14PStream* _stream){
	pthread_mutex_lock(&mtx_streams);
	for(unsigned int i = 0; i<streams.size(); i++){
	    if(streams[i] == _stream){
		streams.erase(streams.begin() + i);
                if(streams.size() == 0) streams_active.set(false);
		break;
	    }
	}
	pthread_mutex_unlock(&mtx_streams);

}

void r14p::R14PClient::remove_stream_unsafe(r14p::R14PStream* _stream){
	for(unsigned int i = 0; i<streams.size(); i++){
	    if(streams[i] == _stream){
		streams.erase(streams.begin() + i);
                if(streams.size() == 0) streams_active.set(false);
		break;
	    }
	}

}

int  r14p::R14PClient::get_stream_count(){
	pthread_mutex_lock(&mtx_streams);
	int c = streams.size();
	pthread_mutex_unlock(&mtx_streams);
	return c;

}
r14p::R14PStream* r14p::R14PClient::get_stream(unsigned int index){
	pthread_mutex_lock(&mtx_streams);
	if(streams.size() > index){
		R14PStream* stream = streams[index];
		pthread_mutex_unlock(&mtx_streams);
		return stream;
	}
	pthread_mutex_unlock(&mtx_streams);

	return NULL;
}

r14p::R14PStream* r14p::R14PClient::get_stream(const unsigned char* _uuid){
	if(_uuid == NULL) return NULL;
	R14PStream* stream = NULL;
	pthread_mutex_lock(&mtx_streams);
	for(unsigned int i = 0; i<streams.size(); i++){
		stream = streams[i];
		if(memcmp(stream->get_uuid(), _uuid, 16) == 0){
			pthread_mutex_unlock(&mtx_streams);
			return stream;
		}
	}
	pthread_mutex_unlock(&mtx_streams);
	return NULL;
}

void r14p::R14PClient::remove_callback(R14PEventType callback_type, bool unsafe){
	if(!unsafe) pthread_spin_lock(&slock_callback);// pthread_mutex_lock(&mtx_callback);
	callback_handler.remove_callback(callback_type);
	if(!unsafe) pthread_spin_unlock(&slock_callback);// pthread_mutex_unlock(&mtx_callback);

}

void r14p::R14PClient::set_callback(R14PEventType callback_type, R14PCallbackMethod* callback_method, bool unsafe){
	if(!unsafe) pthread_spin_lock(&slock_callback);//pthread_mutex_lock(&mtx_callback);
	callback_handler.set_callback(callback_type, callback_method);
	if(!unsafe) pthread_spin_unlock(&slock_callback);//pthread_mutex_unlock(&mtx_callback);
}

r14p::R14PCallbackMethod* r14p::R14PClient::get_callback(R14PEventType callback_type, bool unsafe){
	if(!unsafe) pthread_spin_lock(&slock_callback);//pthread_mutex_lock(&mtx_callback);
	R14PCallbackMethod* res = callback_handler.get_callback(callback_type);
	if(!unsafe) pthread_spin_unlock(&slock_callback);//pthread_mutex_unlock(&mtx_callback);
	return res;
}



bool r14p::R14PClient::process_cleanup(R14PEventType type, R14PCallbackArgs* args){
	pthread_spin_lock(&slock_callback);
	bool res = callback_handler.process_cleanup(type, args);
	pthread_spin_unlock(&slock_callback);
	return res;

}

bool r14p::R14PClient::process_callback(R14PEventType type, R14PCallbackArgs* args){
	pthread_spin_lock(&slock_callback);
	bool res = callback_handler.process_callback(type, args);
	pthread_spin_unlock(&slock_callback);
	return res;
}


int r14p::R14PClient::reconnect_socket(){
    int _socket = -1;
    while(_socket <= 0 && is_active()){
	// connect and bind to specific ip:port
	_socket = sctp::init_sctp_client_bind(inet_addr(end_point_address),
					      0,
					      inet_addr(local_point_address),
					      0,
					      local_point_port, end_point_port,
					      16);
	if(_socket > 0){
	    // client active
	    R14PCallbackArgs cb_args;
	    cb_args.clear_all_args();
	    cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, this);
	    // R14P_ET_CLIENT_RECONNECTED event
	    process_callback(R14P_ET_CLIENT_RECONNECTED, &cb_args);
	    // return
	    return _socket;
	}
	// pause
	sleep(poll_interval);
	// force timeout of all active streams
	process_timeout(true);


    }
    return -1;

}

int r14p::R14PClient::disconnect(){
	// set as inactive
	set_activity(false);
	// shutdown client
	int res = sctp::shutdown_sctp_client(client_socket);
	// socket closed
	if(res == 0) {
		client_socket = -1;
		return 0;
	}
	// error while closing socket
	return -1;
}

void r14p::R14PClient::init_threads(){
    // scheduling parameters
    pthread_attr_init(&in_thread_attr);
    pthread_attr_init(&out_thread_attr);
    pthread_attr_init(&timeout_thread_attr);

    // explicit FIFO scheduling for IN thread
    pthread_attr_setinheritsched(&in_thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&in_thread_attr, SCHED_FIFO);

    // explicit FIFO scheduling for OUT thread
    pthread_attr_setinheritsched(&out_thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&out_thread_attr, SCHED_FIFO);

    // explicit FIFO scheduling for TIMEOUT thread
    pthread_attr_setinheritsched(&timeout_thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&timeout_thread_attr, SCHED_FIFO);

    // priority
    sched_param in_sp;
    sched_param out_sp;
    sched_param timeout_sp;

    // max priority for IN/OUT
    in_sp.__sched_priority = 99;
    out_sp.__sched_priority = 99;
    // half priority for TIMEOUT
    timeout_sp.__sched_priority = 50;

    // set priorities
    pthread_attr_setschedparam(&in_thread_attr, &in_sp);
    pthread_attr_setschedparam(&out_thread_attr, &out_sp);
    pthread_attr_setschedparam(&timeout_thread_attr, &timeout_sp);

    if(pthread_create(&out_thread, &out_thread_attr, &out_loop, this) == 0) {
	pthread_setname_np(out_thread, "r14p_out");
	inc_thread_count();
    }
    if(pthread_create(&timeout_thread, &timeout_thread_attr, &timeout_loop, this) == 0) {
	pthread_setname_np(timeout_thread, "r14p_timeout");
	inc_thread_count();
    }

    // start threads
    if(pthread_create(&in_thread, &in_thread_attr, &in_loop, this) == 0) {
	pthread_setname_np(in_thread, "r14p_in");
	inc_thread_count();
    }


    // destroy atrributes
    pthread_attr_destroy(&in_thread_attr);
    pthread_attr_destroy(&out_thread_attr);
    pthread_attr_destroy(&timeout_thread_attr);


}

r14p::R14PStats* r14p::R14PClient::get_stats(R14PStatsType stats_type){
    switch(stats_type){
	case R14P_INBOUND_STATS:
	    return &in_stats;
	    break;

	case R14P_OUTBOUND_STATS:
	    return &out_stats;
	    break;

	default: return NULL;
    }

}


void r14p::R14PClient::get_stats(R14PStatsType stats_type, R14PStats* result){
    if(result != NULL){
	switch(stats_type){
	    case R14P_INBOUND_STATS:
		*result = in_stats;
		break;

	    case R14P_OUTBOUND_STATS:
		*result = out_stats;
		break;
	}
    }
}



void r14p::R14PClient::generate_stream_header(	asn1::R14PMessage* r14p_out_message,
						R14PStream* stream,
						uint64_t _session_id,
						R14PPayload* r14pld,
						bool _include_body,
						const char* _dest_type,
						const char* _dest_id){

    if(r14p_out_message != NULL && stream != NULL && r14pld != NULL){
	bool prepare_needed = false;

	// set optional
	// source id
	if(r14p_out_message->_header->_source->_id == NULL) {
	    r14p_out_message->_header->_source->set_id();
	    prepare_needed = true;
	}


	// destination id
	if(_dest_id != NULL){
		if(r14p_out_message->_header->_destination->_id == NULL) {
		    r14p_out_message->_header->_destination->set_id();
		    prepare_needed = true;
		}

	}else{
	    if(r14p_out_message->_header->_destination->_id != NULL) r14p_out_message->_header->_destination->_id->unlink(_session_id);
	}


	if(r14p_out_message->_header->_status != NULL) r14p_out_message->_header->_status->unlink(_session_id);

	// body
	if(!_include_body){
		if(r14p_out_message->_body != NULL) r14p_out_message->_body->unlink(_session_id);

	}

	// prepare only if one of optional fields was not set
	if(prepare_needed) r14p_out_message->prepare();



	// unlink status if exists
	if(r14p_out_message->_header->_status != NULL) r14p_out_message->_header->_status->unlink(_session_id);

	// header
	int ver = _R14P_VERSION_;
	r14p_out_message->_header->_version->set_linked_data(_session_id, (unsigned char*)&ver, 1);
	r14p_out_message->_header->_source->_id->set_linked_data(_session_id, (unsigned char*)session->get_daemon_id(), strlen(session->get_daemon_id()));
	r14p_out_message->_header->_source->_type->set_linked_data(_session_id, (unsigned char*)session->get_daemon_type(), strlen(session->get_daemon_type()));
	if(_dest_id != NULL) r14p_out_message->_header->_destination->_id->set_linked_data(_session_id, (unsigned char*)_dest_id, strlen(_dest_id));
	r14p_out_message->_header->_destination->_type->set_linked_data(_session_id, (unsigned char*)_dest_type, strlen(_dest_type));


	r14p_out_message->_header->_uuid->set_linked_data(_session_id, stream->get_uuid(), 16);

	uint32_t seqn = htobe32(stream->get_sequence_num());
	r14p_out_message->_header->_sequence_num->set_linked_data(_session_id, (unsigned char*)&seqn, 4);

	int seqf = stream->get_sequence_flag();
	r14p_out_message->_header->_sequence_flag->set_linked_data(_session_id, (unsigned char*)&seqf, 1);

	// body



	// encode
	r14pld->raw_data_length = asn1::encode(r14pld->raw_data, 1024, r14p_out_message, _session_id);

    }


}


void r14p::R14PClient::generate_stream_complete(	asn1::R14PMessage* r14p_orig_message,
							asn1::R14PMessage* r14p_out_message,
							uint64_t _orig_session_id,
							uint64_t _out_session_id,
							R14PPayload* r14pld){
    if(r14p_orig_message != NULL){

	// next session id
	uint64_t _session_id = _out_session_id;

	// check optional
	bool prepare_needed = false;
	bool source_id = false;
	bool destination_id = false;

	// check is status is set
	if(r14p_out_message->_header->_status == NULL){
	    r14p_out_message->_header->set_status();
	    prepare_needed = true;
	}

	// check if destination id is present
	if(r14p_orig_message->_header->_destination->_id != NULL){
		if(r14p_orig_message->_header->_destination->_id->has_linked_data(_orig_session_id)){
		    if(r14p_out_message->_header->_source->_id == NULL){
			r14p_out_message->_header->_source->set_id();
			prepare_needed = true;
		    }
		    destination_id = true;
		}

	}


	// check if source id is present
	if(r14p_orig_message->_header->_source->_id != NULL){
		if(r14p_orig_message->_header->_source->_id->has_linked_data(_orig_session_id)){

		    if(r14p_out_message->_header->_destination->_id == NULL){

			r14p_out_message->_header->_destination->set_id();
			prepare_needed = true;
		    }
		    source_id = true;
		}

	}


	// prepare only if one of optional fields was not set
	if(prepare_needed) r14p_out_message->prepare();

	// unlink body if exists
	if(r14p_out_message->_body != NULL) r14p_out_message->_body->unlink(_session_id);
	// check is status is set
	if(r14p_out_message->_header->_status != NULL) r14p_out_message->_header->_status->unlink(_out_session_id);


	// version
	int ver = _R14P_VERSION_;
	r14p_out_message->_header->_version->set_linked_data(_session_id, (unsigned char*)&ver, 1);

	// source
	r14p_out_message->_header->_source->_type->set_linked_data(	_session_id,
									r14p_orig_message->_header->_destination->_type->linked_node->tlv->value,
									r14p_orig_message->_header->_destination->_type->linked_node->tlv->value_length);


	if(destination_id){
	    r14p_out_message->_header->_source->_id->set_linked_data(	_session_id,
									r14p_orig_message->_header->_destination->_id->linked_node->tlv->value,
									r14p_orig_message->_header->_destination->_id->linked_node->tlv->value_length);

	}else if(r14p_out_message->_header->_source->_id != NULL) r14p_out_message->_header->_source->_id->unlink(_session_id);


	// destination
	r14p_out_message->_header->_destination->_type->set_linked_data(	_session_id,
										r14p_orig_message->_header->_source->_type->linked_node->tlv->value,
										r14p_orig_message->_header->_source->_type->linked_node->tlv->value_length);

	if(source_id){
	    r14p_out_message->_header->_destination->_id->set_linked_data(	_session_id,
										r14p_orig_message->_header->_source->_id->linked_node->tlv->value,
										r14p_orig_message->_header->_source->_id->linked_node->tlv->value_length);

	}else if(r14p_out_message->_header->_destination->_id != NULL) r14p_out_message->_header->_destination->_id->unlink(_session_id);


	// uuid
	r14p_out_message->_header->_uuid->set_linked_data(	_session_id,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value_length);

	// sequence num
	uint32_t seqn = htobe32(r14pld->stream->get_sequence_num());
	r14p_out_message->_header->_sequence_num->set_linked_data(_session_id, (unsigned char*)&seqn, 4);



	int sf = asn1::SequenceFlag::_sf_stream_complete;
	r14p_out_message->_header->_sequence_flag->set_linked_data(_session_id, (unsigned char*)&sf, 1);

	int status = asn1::ErrorCode::_err_ok;
	r14p_out_message->_header->_status->set_linked_data(_session_id, (unsigned char*)&status, 1);



	r14pld->raw_data_length = asn1::encode(r14pld->raw_data, 1024, r14p_out_message, _session_id);


    }
}

int r14p::R14PClient::update_hop_info(		asn1::R14PMessage* r14p_orig_message,
						asn1::R14PMessage* r14p_out_message,
						uint64_t _orig_session_id,
						uint64_t _out_session_id,
						unsigned char* _destination,
						int _destination_length,
						R14PPayload* r14pld){

    // null check
    if(r14p_orig_message != NULL){
	// next session id
	uint64_t _session_id = _out_session_id;


	// check optional
	bool prepare_needed = false;
	bool source_id = false;
	bool dest_id = false;
	bool has_status = false;
	bool has_body = false;
	int current_hop = 0;
	int max_hops = 10;

	// body
	if(r14p_orig_message->_body != NULL){
		if(r14p_orig_message->_body->choice_selection != NULL){
			if(r14p_orig_message->_body->choice_selection->has_linked_data(_orig_session_id)){
			    has_body = true;
			}

		}

	}
	if(!has_body){
	    if(r14p_out_message->_body != NULL) r14p_out_message->_body->unlink(_session_id);
	}else{
	    if(r14p_out_message->_body == NULL){
		r14p_out_message->set_body();
		prepare_needed = true;
	    }
	}


	// status
	if(r14p_orig_message->_header->_status != NULL){
		if(r14p_orig_message->_header->_status->has_linked_data(_orig_session_id)){
		    has_status = true;
		}

	}
	if(has_status){
	    if(r14p_out_message->_header->_status == NULL){
		r14p_out_message->_header->set_status();
		prepare_needed = true;
	    }
	}else{
	    if(r14p_out_message->_header->_status != NULL){
		r14p_out_message->_header->_status->unlink(_session_id);
	    }
	}



	// source id
	if(r14p_orig_message->_header->_source->_id != NULL){
		if(r14p_orig_message->_header->_source->_id->has_linked_data(_orig_session_id)){
		    if(r14p_out_message->_header->_source->_id == NULL){
			r14p_out_message->_header->_source->set_id();
			prepare_needed = true;
		    }
		    source_id = true;
		}


	}
	if(source_id){
	    if(r14p_out_message->_header->_source->_id == NULL) r14p_out_message->_header->_source->set_id();

	}else{
	    if(r14p_out_message->_header->_source->_id != NULL) r14p_out_message->_header->_source->_id->unlink(_session_id);
	}


	// destination
	if(r14p_orig_message->_header->_destination->_id != NULL){
		if(r14p_orig_message->_header->_destination->_id->has_linked_data(_orig_session_id)){
		    if(r14p_out_message->_header->_destination->_id == NULL){
			r14p_out_message->_header->_destination->set_id();
			prepare_needed = true;
		    }
		    dest_id = true;
		}


	}
	if(source_id){
	    if(r14p_out_message->_header->_source->_id == NULL) r14p_out_message->_header->_source->set_id();

	}else{
	    if(r14p_out_message->_header->_source->_id != NULL) r14p_out_message->_header->_source->_id->unlink(_session_id);
	}

	if(dest_id){
	    if(r14p_out_message->_header->_destination->_id == NULL) r14p_out_message->_header->_destination->set_id();

	}else{
	    if(r14p_out_message->_header->_destination->_id != NULL) r14p_out_message->_header->_destination->_id->unlink(_session_id);
	}


	// source hop
	if(asn1::node_exists(r14p_orig_message->_header->_hop_info, _orig_session_id)){
	    memcpy(&current_hop, r14p_orig_message->_header->_hop_info->_current_hop->linked_node->tlv->value,
				 r14p_orig_message->_header->_hop_info->_current_hop->linked_node->tlv->value_length);

	    current_hop = be32toh(current_hop);


	    if(current_hop > max_hops) return 1;



	}


	// hop info
	if(r14p_out_message->_header->_hop_info == NULL){
	    r14p_out_message->_header->set_hop_info();
	    prepare_needed = true;
	}

	// prepare only if one of optional fields was not set
	if(prepare_needed) r14p_out_message->prepare();



	// version
	int ver = _R14P_VERSION_;
	r14p_out_message->_header->_version->set_linked_data(_session_id, (unsigned char*)&ver, 1);

	// source
	r14p_out_message->_header->_source->_type->set_linked_data(	_session_id,
									r14p_orig_message->_header->_source->_type->linked_node->tlv->value,
									r14p_orig_message->_header->_source->_type->linked_node->tlv->value_length);


	if(source_id){
	    r14p_out_message->_header->_source->_id->set_linked_data(	_session_id,
									r14p_orig_message->_header->_source->_id->linked_node->tlv->value,
									r14p_orig_message->_header->_source->_id->linked_node->tlv->value_length);

	}else if(r14p_out_message->_header->_source->_id != NULL) r14p_out_message->_header->_source->_id->unlink(_session_id);


	// destination
	r14p_out_message->_header->_destination->_type->set_linked_data(_session_id,
									r14p_orig_message->_header->_destination->_type->linked_node->tlv->value,
									r14p_orig_message->_header->_destination->_type->linked_node->tlv->value_length);


	if(dest_id){
	    r14p_out_message->_header->_destination->_id->set_linked_data(_session_id,
									  r14p_orig_message->_header->_destination->_id->linked_node->tlv->value,
									  r14p_orig_message->_header->_destination->_id->linked_node->tlv->value_length);

	}else if(r14p_out_message->_header->_destination->_id != NULL) r14p_out_message->_header->_destination->_id->unlink(_session_id);




	// uuid
	r14p_out_message->_header->_uuid->set_linked_data(	_session_id,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value_length);

	// sequence num
	r14p_out_message->_header->_sequence_num->set_linked_data(	_session_id,
									r14p_orig_message->_header->_sequence_num->linked_node->tlv->value,
									r14p_orig_message->_header->_sequence_num->linked_node->tlv->value_length);

	// sequence flag
	r14p_out_message->_header->_sequence_flag->set_linked_data(	_session_id,
									r14p_orig_message->_header->_sequence_flag->linked_node->tlv->value,
									r14p_orig_message->_header->_sequence_flag->linked_node->tlv->value_length);

	// status
	if(has_status){
		r14p_out_message->_header->_status->set_linked_data(	_session_id,
									r14p_orig_message->_header->_status->linked_node->tlv->value,
									r14p_orig_message->_header->_status->linked_node->tlv->value_length);

	}

	// hop info
	current_hop = htobe32(current_hop + 1);
	max_hops = htobe32(max_hops);
	r14p_out_message->_header->_hop_info->_current_hop->set_linked_data(_session_id, (unsigned char*)&current_hop, sizeof(current_hop));
	r14p_out_message->_header->_hop_info->_max_hops->set_linked_data(_session_id, (unsigned char*)&max_hops, sizeof(max_hops));

	// body
	if(has_body){
	    // get original choice selection
	    int ci = 0;
	    for(unsigned int i = 0; i<r14p_orig_message->_body->children.size(); i++){
		if(r14p_orig_message->_body->children[i] == r14p_orig_message->_body->choice_selection){
		    ci = i;
		    break;
		}
	    }
	    r14p_out_message->_body->choice_selection = r14p_out_message->_body->children[ci];
	    r14p_out_message->_body->choice_selection->set_linked_data(	_session_id,
									r14p_orig_message->_body->choice_selection->linked_node->tlv->value,
									r14p_orig_message->_body->choice_selection->linked_node->tlv->value_length);

	    // override auto complexity
	    r14p_out_message->_body->choice_selection->tlv->override_auto_complexity = true;

	}


	// encode
	r14pld->raw_data_length = asn1::encode(r14pld->raw_data, 1024, r14p_out_message, _session_id, false);


	// reset auto complexity flag
	if(has_body) r14p_out_message->_body->choice_selection->tlv->override_auto_complexity = false;


	return 0;

    }

    return 2;


}

void r14p::R14PClient::set_destination_id(	asn1::R14PMessage* r14p_orig_message,
						asn1::R14PMessage* r14p_out_message,
						uint64_t _orig_session_id,
						uint64_t _out_session_id,
						unsigned char* _destination,
						int _destination_length,
						R14PPayload* r14pld){

    // null check
    if(r14p_orig_message != NULL){
	// next session id
	uint64_t _session_id = _out_session_id;


	// check optional
	bool prepare_needed = false;
	bool source_id = false;
	bool has_status = false;
	bool has_body = false;

	// body
	if(r14p_orig_message->_body != NULL){
		if(r14p_orig_message->_body->choice_selection != NULL){
			if(r14p_orig_message->_body->choice_selection->has_linked_data(_orig_session_id)){
			    has_body = true;
			}

		}

	}
	if(!has_body){
	    if(r14p_out_message->_body != NULL) r14p_out_message->_body->unlink(_session_id);
	}else{
	    if(r14p_out_message->_body == NULL){
		r14p_out_message->set_body();
		prepare_needed = true;
	    }
	}

	// unlink hop info if exists
	if(r14p_out_message->_header->_hop_info != NULL) r14p_out_message->_header->_hop_info->unlink(_session_id);


	// status
	if(r14p_orig_message->_header->_status != NULL){
		if(r14p_orig_message->_header->_status->has_linked_data(_orig_session_id)){
		    has_status = true;
		}

	}
	if(has_status){
	    if(r14p_out_message->_header->_status == NULL){
		r14p_out_message->_header->set_status();
		prepare_needed = true;
	    }
	}else{
	    if(r14p_out_message->_header->_status != NULL){
		r14p_out_message->_header->_status->unlink(_session_id);
	    }
	}



	// source id
	if(r14p_orig_message->_header->_source->_id != NULL){
		if(r14p_orig_message->_header->_source->_id->has_linked_data(_orig_session_id)){
		    if(r14p_out_message->_header->_source->_id == NULL){
			r14p_out_message->_header->_source->set_id();
			prepare_needed = true;
		    }
		    source_id = true;
		}


	}
	if(source_id){
	    if(r14p_out_message->_header->_source->_id == NULL) r14p_out_message->_header->_source->set_id();

	}else{
	    if(r14p_out_message->_header->_source->_id != NULL) r14p_out_message->_header->_source->_id->unlink(_session_id);
	}


	// destination
	if(r14p_out_message->_header->_destination->_id == NULL){
	    r14p_out_message->_header->_destination->set_id();
	    prepare_needed = true;
	}

	// prepare only if one of optional fields was not set
	if(prepare_needed) r14p_out_message->prepare();



	// version
	int ver = _R14P_VERSION_;
	r14p_out_message->_header->_version->set_linked_data(_session_id, (unsigned char*)&ver, 1);

	// source
	r14p_out_message->_header->_source->_type->set_linked_data(	_session_id,
									r14p_orig_message->_header->_source->_type->linked_node->tlv->value,
									r14p_orig_message->_header->_source->_type->linked_node->tlv->value_length);


	if(source_id){
	    r14p_out_message->_header->_source->_id->set_linked_data(	_session_id,
									r14p_orig_message->_header->_source->_id->linked_node->tlv->value,
									r14p_orig_message->_header->_source->_id->linked_node->tlv->value_length);

	}else if(r14p_out_message->_header->_source->_id != NULL) r14p_out_message->_header->_source->_id->unlink(_session_id);


	// destination
	r14p_out_message->_header->_destination->_type->set_linked_data(	_session_id,
										r14p_orig_message->_header->_destination->_type->linked_node->tlv->value,
										r14p_orig_message->_header->_destination->_type->linked_node->tlv->value_length);

	r14p_out_message->_header->_destination->_id->set_linked_data(		_session_id,
										_destination,
										_destination_length);





	// uuid
	r14p_out_message->_header->_uuid->set_linked_data(	_session_id,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value_length);

	// sequence num
	r14p_out_message->_header->_sequence_num->set_linked_data(	_session_id,
									r14p_orig_message->_header->_sequence_num->linked_node->tlv->value,
									r14p_orig_message->_header->_sequence_num->linked_node->tlv->value_length);

	// sequence flag
	r14p_out_message->_header->_sequence_flag->set_linked_data(	_session_id,
									r14p_orig_message->_header->_sequence_flag->linked_node->tlv->value,
									r14p_orig_message->_header->_sequence_flag->linked_node->tlv->value_length);

	// status
	if(has_status){
		r14p_out_message->_header->_status->set_linked_data(	_session_id,
									r14p_orig_message->_header->_status->linked_node->tlv->value,
									r14p_orig_message->_header->_status->linked_node->tlv->value_length);

	}

	// body
	if(has_body){
	    // get original choice selection
	    int ci = 0;
	    for(unsigned int i = 0; i<r14p_orig_message->_body->children.size(); i++){
		if(r14p_orig_message->_body->children[i] == r14p_orig_message->_body->choice_selection){
		    ci = i;
		    break;
		}
	    }
	    r14p_out_message->_body->choice_selection = r14p_out_message->_body->children[ci];
	    r14p_out_message->_body->choice_selection->set_linked_data(	_session_id,
									r14p_orig_message->_body->choice_selection->linked_node->tlv->value,
									r14p_orig_message->_body->choice_selection->linked_node->tlv->value_length);

	    // override auto complexity
	    r14p_out_message->_body->choice_selection->tlv->override_auto_complexity = true;

	}


	// encode
	r14pld->raw_data_length = asn1::encode(r14pld->raw_data, 1024, r14p_out_message, _session_id, false);


	// reset auto complexity flag
	if(has_body) r14p_out_message->_body->choice_selection->tlv->override_auto_complexity = false;

    }

}



void  r14p::R14PClient::generate_err(	asn1::R14PMessage* r14p_orig_message,
					asn1::R14PMessage* r14p_out_message,
					uint64_t _orig_session_id,
					uint64_t _out_session_id,
					R14PPayload* r14pld,
					bool mem_switch,
					int _custom_seq_flag,
					char* _custom_dtype,
					char* _custom_did,
					int _error_code){
    if(r14p_orig_message != NULL){

	// next session id
	uint64_t _session_id = _out_session_id;


	// check optional
	bool prepare_needed = false;
	bool source_id = false;
	bool destination_id = false;

	// check is status is set
	if(r14p_out_message->_header->_status == NULL){
	    r14p_out_message->_header->set_status();
	    prepare_needed = true;
	}


	if(r14p_out_message->_header->_source->_id == NULL){
	    r14p_out_message->_header->_source->set_id();
	    prepare_needed = true;
	}
	destination_id = true;

	// check if source id is present
	if(r14p_orig_message->_header->_source->_id != NULL){
		if(r14p_orig_message->_header->_source->_id->has_linked_data(_orig_session_id)){

		    if(r14p_out_message->_header->_destination->_id == NULL){

			r14p_out_message->_header->_destination->set_id();
			prepare_needed = true;
		    }
		    source_id = true;
		}


	}

	// prepare only if one of optional fields was not set
	if(prepare_needed) r14p_out_message->prepare();

	// unlink body if exists
	if(r14p_out_message->_body != NULL) r14p_out_message->_body->unlink(_session_id);


	// version
	int ver = _R14P_VERSION_;
	r14p_out_message->_header->_version->set_linked_data(_session_id, (unsigned char*)&ver, 1);

	// unlink hop info if exists
	if(r14p_out_message->_header->_hop_info != NULL) r14p_out_message->_header->_hop_info->unlink(_session_id);


	// source
	if(_custom_did != NULL && _custom_dtype != NULL){
		r14p_out_message->_header->_source->_type->set_linked_data(	_session_id,
										(unsigned char*)_custom_dtype,
										strlen(_custom_dtype));

		r14p_out_message->_header->_source->_id->set_linked_data(	_session_id,
										(unsigned char*)_custom_did,
										strlen(_custom_did));

	}else{
		r14p_out_message->_header->_source->_type->set_linked_data(	_session_id,
										r14p_orig_message->_header->_destination->_type->linked_node->tlv->value,
										r14p_orig_message->_header->_destination->_type->linked_node->tlv->value_length);


		if(destination_id){
		    r14p_out_message->_header->_source->_id->set_linked_data(	_session_id,
										(unsigned char*)get_session()->get_daemon_id(),
										strlen(get_session()->get_daemon_id()));




		}else if(r14p_out_message->_header->_source->_id != NULL) r14p_out_message->_header->_source->_id->unlink(_session_id);

	}


	// destination
	r14p_out_message->_header->_destination->_type->set_linked_data(	_session_id,
										r14p_orig_message->_header->_source->_type->linked_node->tlv->value,
										r14p_orig_message->_header->_source->_type->linked_node->tlv->value_length);

	if(source_id){
	    r14p_out_message->_header->_destination->_id->set_linked_data(	_session_id,
										r14p_orig_message->_header->_source->_id->linked_node->tlv->value,
										r14p_orig_message->_header->_source->_id->linked_node->tlv->value_length);

	}else if(r14p_out_message->_header->_destination->_id != NULL) r14p_out_message->_header->_destination->_id->unlink(_session_id);


	// uuid
	r14p_out_message->_header->_uuid->set_linked_data(	_session_id,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value_length);

	// sequence num

	r14p_out_message->_header->_sequence_num->set_linked_data(	_session_id,
									r14p_orig_message->_header->_sequence_num->linked_node->tlv->value,
									r14p_orig_message->_header->_sequence_num->linked_node->tlv->value_length);


	int sf = (_custom_seq_flag == -1 ? asn1::SequenceFlag::_sf_stream_complete : _custom_seq_flag);
	r14p_out_message->_header->_sequence_flag->set_linked_data(_session_id, (unsigned char*)&sf, 1);

	//int status = asn1::ErrorCode::_err_out_of_sequence;
	r14p_out_message->_header->_status->set_linked_data(_session_id, (unsigned char*)&_error_code, 1);


	// encode
	r14pld->raw_data_length = asn1::encode(r14pld->raw_data, 1024, r14p_out_message, _session_id, mem_switch);

    }
}

void r14p::R14PClient::generate_ack(	asn1::R14PMessage* r14p_orig_message,
					asn1::R14PMessage* r14p_out_message,
					uint64_t _orig_session_id,
					uint64_t _out_session_id,
					R14PPayload* r14pld,
					bool include_body,
					bool mem_switch){
    if(r14p_orig_message != NULL){

	// next session id
	uint64_t _session_id = _out_session_id;


	// check optional
	bool prepare_needed = false;
	bool source_id = false;
	bool destination_id = false;

	// check is status is set
	if(r14p_out_message->_header->_status == NULL){
	    r14p_out_message->_header->set_status();
	    prepare_needed = true;
	}


	if(r14p_out_message->_header->_source->_id == NULL){
	    r14p_out_message->_header->_source->set_id();
	    prepare_needed = true;
	}
	destination_id = true;

	// check if source id is present
	if(r14p_orig_message->_header->_source->_id != NULL){
		if(r14p_orig_message->_header->_source->_id->has_linked_data(_orig_session_id)){

		    if(r14p_out_message->_header->_destination->_id == NULL){

			r14p_out_message->_header->_destination->set_id();
			prepare_needed = true;
		    }
		    source_id = true;
		}


	}

	// prepare only if one of optional fields was not set
	if(prepare_needed) r14p_out_message->prepare();

	// unlink body if exists
	if(!include_body){
	    if(r14p_out_message->_body != NULL) r14p_out_message->_body->unlink(_session_id);
	}


	// version
	int ver = _R14P_VERSION_;
	r14p_out_message->_header->_version->set_linked_data(_session_id, (unsigned char*)&ver, 1);

	// source
	if(	r14p_orig_message->_header->_destination->_type->linked_node->tlv->value_length == 1 &&
		r14p_orig_message->_header->_destination->_type->linked_node->tlv->value[0] == '.'){

		r14p_out_message->_header->_source->_type->set_linked_data(	_session_id,
										(unsigned char*)get_session()->get_daemon_type(),
										strlen(get_session()->get_daemon_type()));

	}else{
		r14p_out_message->_header->_source->_type->set_linked_data(	_session_id,
										r14p_orig_message->_header->_destination->_type->linked_node->tlv->value,
										r14p_orig_message->_header->_destination->_type->linked_node->tlv->value_length);

	}


	if(destination_id){
	    r14p_out_message->_header->_source->_id->set_linked_data(	_session_id,
									(unsigned char*)get_session()->get_daemon_id(),
									strlen(get_session()->get_daemon_id()));


	}else if(r14p_out_message->_header->_source->_id != NULL) r14p_out_message->_header->_source->_id->unlink(_session_id);


	// destination
	r14p_out_message->_header->_destination->_type->set_linked_data(	_session_id,
										r14p_orig_message->_header->_source->_type->linked_node->tlv->value,
										r14p_orig_message->_header->_source->_type->linked_node->tlv->value_length);

	if(source_id){
	    r14p_out_message->_header->_destination->_id->set_linked_data(	_session_id,
										r14p_orig_message->_header->_source->_id->linked_node->tlv->value,
										r14p_orig_message->_header->_source->_id->linked_node->tlv->value_length);

	}else if(r14p_out_message->_header->_destination->_id != NULL) r14p_out_message->_header->_destination->_id->unlink(_session_id);


	// uuid
	r14p_out_message->_header->_uuid->set_linked_data(	_session_id,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value,
								r14p_orig_message->_header->_uuid->linked_node->tlv->value_length);

	// sequence num
	uint32_t seqn = htobe32(r14pld->stream->get_sequence_num());
	r14p_out_message->_header->_sequence_num->set_linked_data(_session_id, (unsigned char*)&seqn, 4);

	int sf;
	switch(r14pld->stream->get_sequence_flag()){
	    case R14P_SF_START: sf = asn1::SequenceFlag::_sf_continue; break;
	    case R14P_SF_CONTINUE: sf = asn1::SequenceFlag::_sf_continue; break;
	    case R14P_SF_CONTINUE_WAIT: sf = asn1::SequenceFlag::_sf_continue_wait; break;
	    case R14P_SF_STATELESS: sf = asn1::SequenceFlag::_sf_end; break;
	    case R14P_SF_HEARTBEAT: sf = asn1::SequenceFlag::_sf_heartbeat; break;
	    default: sf = asn1::SequenceFlag::_sf_end; break;
	}
	r14p_out_message->_header->_sequence_flag->set_linked_data(_session_id, (unsigned char*)&sf, 1);

	if(!r14pld->stream->get_seq_reply_received()){
		int status = asn1::ErrorCode::_err_ok;
		r14p_out_message->_header->_status->set_linked_data(_session_id, (unsigned char*)&status, 1);

	}else r14p_out_message->_header->_status->unlink(_session_id);


	// encode
	r14pld->raw_data_length = asn1::encode(r14pld->raw_data, 1024, r14p_out_message, _session_id, mem_switch);

	// inc sequence number if in R14P_SF_CONTINUE_WAIT state
	if(r14pld->stream->get_sequence_flag() == R14P_SF_CONTINUE_WAIT) {
	    r14pld->stream->inc_sequence_num();
	}

    }

}


void* r14p::R14PClient::timeout_loop(void* args){
    if(args != NULL){
	R14PClient* r14pc = (R14PClient*)args;
	R14PCallbackArgs cb_stream_args;
	int total_sleep = 0;

	// loop
	while(r14pc->is_active()){
	    // sleep 1 sec
	    sleep(1);
	    ++total_sleep;
	    // check if user timeout has been reached
	    if(total_sleep < r14pc->stream_timeout) continue;
	    // set flag, timeout will be processed in IN thread
	    r14pc->stream_timeout_check.comp_swap(false, true);
	    // reset current timeout
	    total_sleep = 0;
	}

	// detach thread
	r14pc->timeout_thread = 0;
	pthread_detach(pthread_self());
	r14pc->dec_thread_count();

    }


    return NULL;
}

void* r14p::R14PClient::reg_timeout_loop(void* args){
    if(args != NULL){
	RegClientStreamNew* snew = (RegClientStreamNew*)args;
	R14PClient* r14pc = snew->client;

	timespec ts = {0, 1000000}; // 1msec
	// wait for signal
	while(r14pc->is_active() && !snew->done_signal.get()){
	    nanosleep(&ts, NULL);
	}

	// free event mem
	delete snew->sdone;
	delete snew;

	// detach thread
	pthread_detach(r14pc->reg_timeout_thread);
	r14pc->reg_timeout_thread = 0;

	// dec thread count (this thread_)
	r14pc->dec_thread_count();
    }
    return NULL;
}

int r14p::R14PClient::register_client(){
	// check if alreday registered
	if(registered.get()) return -2;
	// events
	RegClientStreamNew* snew = new RegClientStreamNew(this);
	RegClientStreamDone* sdone = new RegClientStreamDone();
	snew->sdone = sdone;
	sdone->snew = snew;

	// handle new stream event for current client
	set_callback(r14p::R14P_ET_STREAM_NEW, snew);

	// start registration timeout thread
	if(pthread_create(&reg_timeout_thread, NULL, &reg_timeout_loop, snew) == 0) {
	    inc_thread_count();
	    // ok
	    return 0;
	}
	// err
	return -1;
}

void r14p::R14PClient::set_reg_flag(bool _reg_flag){
    registered.comp_swap(!_reg_flag, _reg_flag);

}

bool r14p::R14PClient::is_registered(){
    return registered.get();
}



void r14p::R14PClient::set_end_point_daemon_id(const char* _did){
    if(strlen(_did) >= sizeof(end_point_daemon_id)){
	memcpy(end_point_daemon_id, _did, sizeof(end_point_daemon_id) - 1);
	end_point_daemon_id[sizeof(end_point_daemon_id) - 1] = 0;
    }else memcpy(end_point_daemon_id, _did, strlen(_did) + 1);
}

void r14p::R14PClient::set_end_point_daemon_type(const char* _dtype){
    if(strlen(_dtype) >= sizeof(end_point_daemon_type)){
	memcpy(end_point_daemon_type, _dtype, sizeof(end_point_daemon_type) - 1);
	end_point_daemon_type[sizeof(end_point_daemon_type) - 1] = 0;
    }else memcpy(end_point_daemon_type, _dtype, strlen(_dtype) + 1);
}


char* r14p::R14PClient::get_end_point_daemon_id(){
    return end_point_daemon_id;
}

char* r14p::R14PClient::get_end_point_daemon_type(){
    return end_point_daemon_type;
}


int r14p::R14PClient::route(asn1::R14PMessage* in_msg, uint64_t sess_id, std::vector<R14PClient*>* routes, char* d_id, char* d_type){
	// null check
	if(in_msg == NULL || routes == NULL) return 1;
	// clear output
	routes->clear();


	memcpy(d_type,
	       in_msg->_header->_destination->_type->linked_node->tlv->value,
	       in_msg->_header->_destination->_type->linked_node->tlv->value_length);
	d_type[in_msg->_header->_destination->_type->linked_node->tlv->value_length] = 0;

	// destination id check
	if(in_msg->_header->_destination->_id != NULL){
	    if(in_msg->_header->_destination->_id->has_linked_data(sess_id)){

		memcpy(d_id,
		       in_msg->_header->_destination->_id->linked_node->tlv->value,
		       in_msg->_header->_destination->_id->linked_node->tlv->value_length);
		d_id[in_msg->_header->_destination->_id->linked_node->tlv->value_length] = 0;


		// route through this client
		if(strcmp(d_id, get_session()->get_daemon_id()) == 0 && strcmp(d_type, get_session()->get_daemon_type()) == 0){
		    routes->push_back(this);
		    return 0;

		// find route
		}else{
		    // routing check
		    if(get_session()->is_router()){
			// find route
			return get_session()->find_route(this, d_type, d_id, routes);

		    // routing not enabled
		    }else return 1;
		}
	    }


	}

	// *** no destination id ***

	// "." - this daemon
	if(strcmp(d_type, ".") == 0){
		routes->push_back(this);
		return 0;
	}else{
		// find route
		if(get_session()->is_router()){
		    return get_session()->find_route(this, d_type, NULL, routes);

		// routing not enabled
		}else return 1;

	}

	// ok
	return 0;
}


bool r14p::R14PClient::validate_seq_num(unsigned char* data, unsigned int data_len, unsigned int expected_seq_num){
    // uint32_t
    if(data_len == 4){
	    // convert to little endian
	    uint32_t tmp = 0;
	    memcpy(&tmp, data, data_len);
	    tmp = be32toh(tmp);
	    // return result
	    return (tmp == expected_seq_num);




    }
    return false;

}
void* r14p::R14PClient::exit_loop(void* args){
	if(args != NULL){
		// session pointer
		R14PClient* r14pc = (R14PClient*)args;
		R14PCallbackArgs cb_args;

		// wait for other threads to finish
		timespec st = {0, 100000000};
		while(r14pc->get_thread_count() > 1){
			nanosleep(&st, NULL);
			// lock
			pthread_mutex_lock(&r14pc->mtx_streams);
			// loop active streams
			for(std::vector<R14PStream*>::iterator it = r14pc->streams.begin(); it != r14pc->streams.end(); it++){
				// set out flag to false (important for process_timeout methos called from exit_loop)
				(*it)->r14p_payload->out.set(false);
			}
			// unlock
			pthread_mutex_unlock(&r14pc->mtx_streams);

		    // force timeout of all active streams
		    r14pc->process_timeout(true);
		}

		// R14P_ET_CLIENT_TERMINATED event on session level
		// callback args
		if(r14pc->get_session() != NULL){
			cb_args.clear_all_args();
			cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
			r14pc->get_session()->process_callback(R14P_ET_CLIENT_TERMINATED, &cb_args);

		}
		// R14P_ET_CLIENT_TERMINATED event on client level
		cb_args.clear_all_args();
		cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
		r14pc->process_callback(R14P_ET_CLIENT_TERMINATED, &cb_args);

		// detach thread
		pthread_detach(pthread_self());

		// dec thread count
		r14pc->dec_thread_count();

		// deallocate connection and remove from the list
		r14pc->get_session()->remove_client(r14pc);


	}

	return NULL;
}


void r14p::R14PClient::process_timeout(bool override){
    // check stream timeout
    if(stream_timeout_check.comp_swap(true, false) || override){
	// update now
	time_t tm_now = (override ? std::numeric_limits<time_t>::max() : time(NULL));
	R14PCallbackArgs cb_stream_args;
	// lock
	pthread_mutex_lock(&mtx_streams);
	// tmp copy of active streams
	std::vector<R14PStream*> tmp_streams = streams;
	// unlock
	pthread_mutex_unlock(&mtx_streams);
	// stream pointer
	R14PStream* tmp_stream = NULL;
	// loop active streams
	for(std::vector<R14PStream*>::iterator it = tmp_streams.begin(); it != tmp_streams.end(); it++){
	    // stream pointer
	    tmp_stream = *it;
	    // skip if still in out_queue
	    if(tmp_stream->r14p_payload->out.get()) continue;
	    // timeout found
	    if(tm_now - tmp_stream->get_timestamp() >= stream_timeout){
		// set timeout flag
		tmp_stream->set_timeout_status(true);
		if(tmp_stream->linked_stream != NULL) tmp_stream->linked_stream->set_timeout_status(true);


		// run R14P_ET_STREAM_TIMEOUT event
		cb_stream_args.clear_all_args();
		cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, this);
		cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_STREAM, tmp_stream);
		tmp_stream->process_callback(R14P_ET_STREAM_TIMEOUT, &cb_stream_args);

		if(tmp_stream->linked_stream != NULL){
		    cb_stream_args.clear_all_args();
		    cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, this);
		    cb_stream_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_STREAM, tmp_stream->linked_stream);
		    tmp_stream->linked_stream->process_callback(R14P_ET_STREAM_TIMEOUT, &cb_stream_args);

		}
		// stats
		if(tmp_stream->initiator == R14P_SIT_LOCAL) out_stats.strm_timeout.add_fetch(1);
		else in_stats.strm_timeout.add_fetch(1);

	    }
	}

	// lock
	pthread_mutex_lock(&mtx_streams);
	// loop active streams
	for(std::vector<R14PStream*>::iterator it = streams.begin(); it != streams.end(); ){
	    // stream pointer
	    tmp_stream = *it;
	    // free if timeout flag set
	    if(tmp_stream->get_timeout_status()){
		// remove from active stream list
		it = streams.erase(it);
		// return to pool
		if(tmp_stream->linked_stream != NULL) deallocate_stream_pool(tmp_stream->linked_stream);
		deallocate_stream_pool(tmp_stream);

	    // next stream
	    }else ++it;
	}
        if(streams.size() == 0) streams_active.set(false);
	// unlock
	pthread_mutex_unlock(&mtx_streams);
    }

}

void r14p::R14PClient::init_reconnect(){
    // set registered to false
    set_reg_flag(false);
    // reconnect mode for OUTBOUND connections
    if(direction == R14P_CD_OUTBOUND){
	    // close old socket
	    sctp::shutdown_sctp_client(client_socket);
	    // client re-connecting
	    R14PCallbackArgs cb_args;
	    cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, this);
	    // R14P_ET_CLIENT_RECONNECTING event
	    process_callback(R14P_ET_CLIENT_RECONNECTING, &cb_args);
	    // wait for socket connection
	    client_socket = reconnect_socket();
	    // register client
	    if(client_socket > 0){
		    // register client (new thread needed due to blocking nature of semaphore used in register_client method)
		    class _tmp_thread {
		    public:
			static void* run(void* args){
			    // client pointer
			    R14PClient* tmp_r14pc = (R14PClient*)args;
			    // register
			    tmp_r14pc->get_session()->register_client(tmp_r14pc, ".");
			    // detach
			    pthread_detach(pthread_self());
			    // dec thread count
			    tmp_r14pc->dec_thread_count();
			    // return
			    return NULL;
			}
		    };
		    // tmp thread
		    pthread_t tmp_thread_h;
		    if(pthread_create(&tmp_thread_h, NULL, &_tmp_thread::run, this) == 0) inc_thread_count();

	    }

    // set as inactive if INBOUND connection
    }else {
	set_activity(false);
	//client_socket = -1;
    }
}



void* r14p::R14PClient::in_loop(void* args){
    if(args != NULL){
	// session pointer
	R14PClient* r14pc = (R14PClient*)args;
	R14PStateMachine* r14p_sm = &r14pc->r14p_sm;
	R14PCallbackArgs cb_args;
	r14p_sm->init(r14pc);

	// loop
	while(r14pc->is_active()){
		// run state machine
	    r14p_sm->run();

	}

	// client terminating
	cb_args.clear_all_args();
	cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14pc);
	// R14P_ET_CLIENT_TERMINATING event
	r14pc->process_callback(R14P_ET_CLIENT_TERMINATING, &cb_args);

	// call stream timeout event handler on all active streams
	r14pc->process_timeout(true);

	// start exit cleanup thread
	r14pc->inc_thread_count();
	if(pthread_create(&r14pc->exit_thread, NULL, &exit_loop, r14pc) == 0){
	    pthread_setname_np(r14pc->exit_thread, "r14p_exit");
	}

	// detach thread
	pthread_detach(r14pc->in_thread);
	r14pc->in_thread = 0;
	r14pc->dec_thread_count();
    }

    return NULL;
}

int r14p::R14PClient::out_process(R14PPayload* r14ppld, R14PCallbackArgs* cb_args){
    // send through socket
    int res = send(r14ppld->sctp_sid, r14ppld->raw_data, r14ppld->raw_data_length);
    if(res == 0){
	// stats
	out_stats.packets.fetch_add(1);
	out_stats.bytes.fetch_add(r14ppld->raw_data_length);

	// process payload callbacks
	cb_args->clear_all_args();
	cb_args->add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_PAYLOAD, r14ppld);
	r14ppld->stream->process_callback(R14P_ET_PAYLOAD_SENT, cb_args);

	// free/return to pool if requested
	if(r14ppld->free_on_send){
	    // free memory, return back to pool
	    deallocate_stream_pool(r14ppld->stream);

	// set active stream payload out flag to false (leaving out queue)
	}else r14ppld->out.set(false);


    // sctp connection error, close socket
    }else{
	// stats
	out_stats.socket_errors.fetch_add(1);
	// queue reconnect processs
	reconnect_queued.comp_swap(false, true);
	// reconnection will be done in IN thread
	// do nothing here

	// free memory, return back to pool
	if(r14ppld->free_on_send) deallocate_stream_pool(r14ppld->stream);
	// set active stream payload out flag to false (leaving out queue)
	else r14ppld->out.set(false);

    }


    return res;

}


void* r14p::R14PClient::out_loop(void* args){
    // check for args
    if(args == NULL) return NULL;
    R14PCallbackArgs cb_args;
    R14PClient* r14pc = (R14PClient*)args;
    R14PPayload* r14ppld = NULL;
    bool internal_data, external_data;
    timespec pause_ts = {0, 1}; // 1nsec
    timespec pause_ts_long = {0, 1000000}; // 1msec

    // loop
    while(r14pc->is_active()){
        // reset
        internal_data = false;
        external_data = false;

        // pop internal
        if(r14pc->internal_out_queue.pop(&r14ppld) == 0){
            internal_data = true;
            r14pc->out_process(r14ppld, &cb_args);
        }


        // pop external
        r14ppld = r14pc->pop_out_queue();
        if(r14ppld != NULL) {
            external_data = true;
            r14pc->out_process(r14ppld, &cb_args);
        }

        // sleep if both queues are empty
        if(!(external_data || internal_data)) {
            // - use smaller sleep value (1 nsec) if the
            //   following conditions are met:
            //     1. at least one stream is active
            //     2. a packet was received by this client
            //        no longer than 1 sec ago (this is important
            //        in order to avoid seeing timed out streams
            //        as active streams if they still haven't
            //        been removed from the active stream list)
            if(r14pc->streams_active.get() && (time(NULL) - r14pc->timestamp.get() < 1))
                nanosleep(&pause_ts, NULL);

            // - sleep longer (1 msec) if there are no
            //   active streams
            else nanosleep(&pause_ts_long, NULL);
        }
    }

    // detach thread
    pthread_detach(r14pc->out_thread);
    r14pc->out_thread = 0;
    r14pc->dec_thread_count();
    return NULL;
}

unsigned int r14p::R14PClient::inc_thread_count(){
    return thread_count.add_fetch(1);
}

unsigned int r14p::R14PClient::dec_thread_count(){
    return thread_count.sub_fetch(1);
}


unsigned int r14p::R14PClient::get_thread_count(){
    return thread_count.get();

}

r14p::R14PSession* r14p::R14PClient::get_session(){
    return session;
}


void r14p::R14PClient::set_session(r14p::R14PSession* _session){
    session = _session;
}

// R14PStream
r14p::R14PStream::R14PStream(){
	memset(uuid, 0, 16);
	bzero(destination_type, 50);
	bzero(destination_id, 50);
	sequence_num = 0;
	sequence_flag = R14P_SF_UNKNOWN;
	client = NULL;
	random_generator = NULL;
	sequence_reply_received = false;
	r14p_message = NULL;
	r14p_payload = NULL;
	timestamp = 0;
	timeout = false;
	linked_stream = NULL;
	last_linked_side = NULL;
	initiator = R14P_SIT_LOCAL;
}

r14p::R14PStream::R14PStream(boost::uuids::random_generator* _random_generator){
	memset(uuid, 0, 16);
	sequence_num = 0;
	sequence_flag = R14P_SF_UNKNOWN;
	client = NULL;
	random_generator = _random_generator;
	timeout = false;
	linked_stream = NULL;
	last_linked_side = NULL;
	initiator = R14P_SIT_LOCAL;

	generate_uuid();

}

void r14p::R14PStream::set_random_generator(boost::uuids::random_generator* _random_generator){
	random_generator = _random_generator;
}

unsigned char* r14p::R14PStream::get_uuid(){
	return uuid;
}

void r14p::R14PStream::inc_sequence_num(){
    sequence_num++;
}

void r14p::R14PStream::wait_sequence(){
    sequence_flag = R14P_SF_CONTINUE_WAIT;
}
void r14p::R14PStream::set_continue_flag(){
    if(r14p_message != NULL){
	r14p_message->_header->_sequence_flag->linked_node->tlv->value[0] = asn1::SequenceFlag::_sf_continue;
    }
}

void r14p::R14PStream::end_sequence(){
    sequence_flag = R14P_SF_END;
}

void r14p::R14PStream::continue_sequence(){
    sequence_flag = R14P_SF_CONTINUE;
}

void r14p::R14PStream::generate_uuid(){
    client->generate_uuid(uuid);

}

void r14p::R14PStream::set_sequence_flag(R14PSequenceFlag _sequence_flag){
    sequence_flag = _sequence_flag;
}

void r14p::R14PStream::send(bool include_body){
	r14p_payload->free_on_send = false;
	r14p_payload->out.set(true);
	r14p_payload->r14p_stream_type = R14P_ST_STATEFUL;
	r14p_payload->client = client;
	r14p_payload->sctp_sid = 0;
	r14p_payload->clear_callbacks();
	r14p_payload->set_callback(R14P_ET_PAYLOAD_SENT, get_callback(R14P_ET_PAYLOAD_SENT));
	client->generate_stream_header(r14p_message, this, 1, r14p_payload, include_body, destination_type, (strlen(destination_id) == 0 ? NULL: destination_id));
	client->push_out_queue(r14p_payload);
}

void r14p::R14PStream::set_client(R14PClient* _client){
    client = _client;
}

void r14p::R14PStream::reset(bool reset_uuid){
    if(reset_uuid) generate_uuid();
    sequence_num = 1;
    sequence_flag = R14P_SF_START;
    sequence_reply_received = false;
    bzero(destination_id, 50);
    bzero(destination_type, 50);
    timestamp = time(NULL);
    timeout = false;
    linked_stream = NULL;
    last_linked_side = NULL;
    if(reset_uuid) initiator = R14P_SIT_LOCAL;
    else initiator = R14P_SIT_REMOTE;

}

void r14p::R14PStream::set_timestamp(time_t _timestamp){
    timestamp = _timestamp;
}

time_t r14p::R14PStream::get_timestamp(){
    return timestamp;
}



bool r14p::R14PStream::get_seq_reply_received(){
    return sequence_reply_received;
}

void r14p::R14PStream::toggle_seq_reply_received(){
    sequence_reply_received = !sequence_reply_received;
}


r14p::R14PSequenceFlag r14p::R14PStream::get_sequence_flag(){
    return sequence_flag;
}
unsigned int r14p::R14PStream::get_sequence_num(){
    return sequence_num;
}



void r14p::R14PStream::set_uuid(unsigned char* _uuid){
    memcpy(uuid, _uuid, 16);

}

r14p::R14PStream::~R14PStream(){
}

bool r14p::R14PStream::process_callback(R14PEventType type, R14PCallbackArgs* args){
	return callback_handler.process_callback(type, args);
}

void r14p::R14PStream::remove_callback(R14PEventType callback_type){
	callback_handler.remove_callback(callback_type);

}

void r14p::R14PStream::clear_callbacks(){
	callback_handler.clear();
}

void r14p::R14PStream::set_param(uint32_t param_id, void* param){
    params[param_id] = param;
}

int r14p::R14PStream::remove_param(uint32_t param_id){
    return params.erase(param_id);

}

void* r14p::R14PStream::get_param(uint32_t param_id){
    std::map<uint32_t, void*>::iterator it = params.find(param_id);
    return (it != params.end() ? it->second : NULL);
}

void r14p::R14PStream::clear_params(){
    params.clear();
}


bool r14p::R14PStream::get_timeout_status(){
    return timeout;
}

void r14p::R14PStream::set_timeout_status(bool _status){
    timeout = _status;
}


r14p::R14PCallbackMethod* r14p::R14PStream::get_callback(R14PEventType callback_type){
    return callback_handler.get_callback(callback_type);
}

void r14p::R14PStream::set_callback(R14PEventType callback_type, R14PCallbackMethod* callback_method){
	callback_handler.set_callback(callback_type, callback_method);
}


// RouteHandlerMethod
r14p::RouteHandlerMethod::RouteHandlerMethod(R14PSession* _r14ps): r14ps(_r14ps){

}

r14p::RouteHandlerMethod::~RouteHandlerMethod(){

}

void r14p::RouteHandlerMethod::run(std::vector<R14PClient*>* all_routes, std::vector<R14PClient*>* chosen_routes){
    if(all_routes != NULL && chosen_routes != NULL){
	if(all_routes->size() > 0) chosen_routes->push_back((*all_routes)[0]);
    }
}

void* r14p::RouteHandlerMethod::add_node(R14PClient* r14pc, const char* node_type, const char* node_id, pmink_utils::PooledVPMap<uint32_t>* params){ return NULL; }
void r14p::RouteHandlerMethod::clear(){}
void* r14p::RouteHandlerMethod::update_client(R14PClient* r14pc, const char* node_type, const char* node_id){ return NULL; }
int r14p::RouteHandlerMethod::remove_type(const char* node_type){ return 0; }
int r14p::RouteHandlerMethod::remove_node(const char* node_type, const char* node_id){ return 0; }
void* r14p::RouteHandlerMethod::get_node(const char* node_type, const char* node_id){ return NULL; }


// WRRRouteHandler
r14p::WRRRouteHandler::WRRRouteHandler(R14PSession* _r14ps): RouteHandlerMethod(_r14ps){

}

r14p::WRRRouteHandler::~WRRRouteHandler(){

}

void r14p::WRRRouteHandler::run(std::vector<R14PClient*>* all_routes, std::vector<R14PClient*>* chosen_routes){
    if(all_routes == NULL || chosen_routes == NULL || all_routes->size() == 0) return;
    // get session from first in the list
    R14PSession* r14ps = ((*all_routes)[0])->get_session();
    // sanity check
    if(r14ps == NULL) return;
    // get route type from first in the list
    char* dest_type = ((*all_routes)[0])->get_end_point_daemon_type();
    // create hash from dest type string
    uint32_t hash = pmink_utils::hash_fnv1a(dest_type, strlen(dest_type));
    // get wrr for specific dest type
    wrr_map_it_t it = wrr_map.find(hash);
    // if no wrr data found, return first in list (fallback to automatic routing)
    if(it == wrr_map.end()) {
	(*all_routes)[0]->inc_refc();
	chosen_routes->push_back((*all_routes)[0]);
	return;
    }
    // wrr data found, run wrr logic
    pmink_utils::WRR<R14PClient*>::items_map_val_t* wrr_res = it->second.run();
    // if no wrr data found, return
    if(wrr_res == NULL || wrr_res->item == NULL) return;
    // add route to result
    wrr_res->item->inc_refc();
    chosen_routes->push_back(wrr_res->item);


}

void* r14p::WRRRouteHandler::add_node(R14PClient* r14pc,
				      const char* node_type,
				      const char* node_id,
				      pmink_utils::PooledVPMap<uint32_t>* params){

    // sanity check
    if(node_type == NULL || params == NULL) return NULL;
    // create hash from dest type string
    uint32_t hash = pmink_utils::hash_fnv1a((char*)node_type, strlen(node_type));
    // insert or return ref
    wrr_map_insert_t in_it = wrr_map.insert(wrr_map_value_t(hash, pmink_utils::WRR<r14p::R14PClient*>()));
    // get weight param
    pmink_utils::VariantParam* vp = params->get_param(0);
    if(vp == NULL) return NULL;
    uint32_t weight = (int)*vp;
    // add node
    in_it.first->second.add_item(r14pc, node_id, weight);
    // disable or enable
    if(r14pc == NULL) in_it.first->second.disable(node_id);
    else in_it.first->second.enable(node_id);
    // get
    return in_it.first->second.get(node_id);

}

void* r14p::WRRRouteHandler::get_node(const char* node_type, const char* node_id){
    if(node_type == NULL || node_id == NULL) return NULL;
    // create hash from dest type string
    uint32_t hash = pmink_utils::hash_fnv1a((char*)node_type, strlen(node_type));
    // find
    wrr_map_it_t it = wrr_map.find(hash);
    // sanity check
    if(it == wrr_map.end()) return NULL;
    // get wrr item
    pmink_utils::WRRItem<r14p::R14PClient*>* wrr_item = it->second.get(node_id);
    // sanity check
    if(wrr_item == NULL) return NULL;
    // ok
    return wrr_item;
}


int r14p::WRRRouteHandler::remove_type(const char* node_type){
    // sanity check
    if(node_type == NULL) return 1;
    // create hash from dest type string
    uint32_t hash = pmink_utils::hash_fnv1a((char*)node_type, strlen(node_type));
    // find
    wrr_map_it_t it = wrr_map.find(hash);
    // sanity check
    if(it == wrr_map.end()) return 1;
    // remove
    wrr_map.erase(it);
    // ok
    return 0;
}

int r14p::WRRRouteHandler::remove_node(const char* node_type, const char* node_id){
    // sanity check
    if(node_type == NULL) return 1;
    // create hash from dest type string
    uint32_t hash = pmink_utils::hash_fnv1a((char*)node_type, strlen(node_type));
    // find
    wrr_map_it_t it = wrr_map.find(hash);
    // sanity check
    if(it == wrr_map.end()) return 1;
    // remove node
    it->second.remove(node_id);
    // ok
    return 0;

}


void* r14p::WRRRouteHandler::update_client(R14PClient* r14pc,
					   const char* node_type,
					   const char* node_id){

    // sanity check
    if(node_type == NULL || node_id == NULL) return NULL;
    // create hash from dest type string
    uint32_t hash = pmink_utils::hash_fnv1a((char*)node_type, strlen(node_type));
    // find
    wrr_map_it_t it = wrr_map.find(hash);
    // sanity check
    if(it == wrr_map.end()) return NULL;
    // get wrr item
    pmink_utils::WRRItem<r14p::R14PClient*>* wrr_item = it->second.get(node_id);
    // sanity check
    if(wrr_item == NULL) return NULL;
    // update client data
    wrr_item->item = r14pc;
    // disable if client was seet to null
    if(r14pc == NULL) it->second.disable(wrr_item);
    // or enable
    else it->second.enable(wrr_item);
    // return wrr item pointer
    return wrr_item;
}


void r14p::WRRRouteHandler::clear(){
    wrr_map.clear();

}



// R14PSession
r14p::R14PSession::R14PSession(const char* _daemon_type, const char* _daemon_id, int _max_concurrent_streams, int _stream_timeout, bool _router, int _poll_interval){
	server_socket.set(-1);
	server_thread = 0;
	router = _router;
	rh_method = NULL;
	max_concurrent_streams = _max_concurrent_streams;
	stream_timeout = (_stream_timeout < 1 ? 1 : _stream_timeout);
	poll_interval = _poll_interval;

	bzero(daemon_type, 50);
	bzero(daemon_id, 50);

	memcpy(daemon_type, _daemon_type, strlen(_daemon_type));
	memcpy(daemon_id, _daemon_id, strlen(_daemon_id));

	pthread_mutex_init(&mtx_callback, NULL);
	pthread_mutex_init(&mtx_clients, NULL);
}

r14p::R14PSession::~R14PSession(){
	// children
	clients.clear();

	// mutexes
	pthread_mutex_destroy(&mtx_callback);
	pthread_mutex_destroy(&mtx_clients);


}


int r14p::R14PSession::stop_server(){
	if(get_server_mode() && (get_server_socket() > 0)){
		// shutdown connection
		sctp::shutdown_sctp_client(get_server_socket());
		// socket closed
		set_server_mode(false);
		set_server_socket(-1);
		// error while closing socket
		return 0;
	}
	// error
	return -1;
}


int r14p::R14PSession::find_route(R14PClient* _client, const char* _daemon_type, const char* _daemon_id, std::vector<R14PClient*>* routes){
    // error check
    if(_daemon_type == NULL) return 1;
    //R14PClient* res_client = NULL;
    // daemon id not present
    if(_daemon_id == NULL){
	// check self
	// check if current daemon type is acceptable
	if(strcmp(get_daemon_type(), _daemon_type) == 0){
	    routes->push_back(_client);
	    return 0;
	}else{
	    R14PClient* tmp_client = NULL;
	    std::vector<R14PClient*> tmp_client_lst;
	    // lock client list
	    lock_clients();
	    // search
	    unsigned int count = get_client_count(true);
	    for(unsigned int i = 0; i<count; i++){
		// set pointer
		tmp_client = get_client(i, true);
		// skip if not registered or not active
		if(!tmp_client->is_registered() || !tmp_client->is_active()) continue;
		// check if client end point daemon type is acceptable
		if(strcmp(tmp_client->get_end_point_daemon_type(), _daemon_type) == 0){
		    // inc ref counter
		    tmp_client->inc_refc();
		    // add to list
		    tmp_client_lst.push_back(tmp_client);
		}

	    }
	    // run routing handler if defined or return first matched route
	    if(rh_method != NULL) rh_method->run(&tmp_client_lst, routes);
	    else{
		if(tmp_client_lst.size() > 0){
		    tmp_client_lst[0]->inc_refc();
		    routes->push_back(tmp_client_lst[0]);
		}

	    }
	    // dec ref counters
	    for(unsigned int i = 0; i<tmp_client_lst.size(); i++) tmp_client_lst[i]->dec_refc();

	    // unlock client list
	    unlock_clients();
	}
    // daemon id present
    }else{
	// check for special '*' id (should only be used with R14P_ST_STATELESS_NO_REPLY to avoid stream uuid conflicts)
	if(strcmp("*", _daemon_id) == 0){
	    R14PClient* tmp_client = NULL;
	    // lock client list
	    lock_clients();
	    // search
	    unsigned int count = get_client_count(true);
	    for(unsigned int i = 0; i<count; i++){
		// set pointer
		tmp_client = get_client(i, true);
		// skip if not registered or not active
		if(!tmp_client->is_registered() || !tmp_client->is_active()) continue;
		if(strcmp(tmp_client->get_end_point_daemon_type(), _daemon_type) == 0 && tmp_client != _client) routes->push_back(tmp_client);
	    }
	    // unlock client list
	    unlock_clients();

	// check if current daemon type and id are acceptable
	}else if(strcmp(get_daemon_type(), _daemon_type) == 0 && strcmp(get_daemon_id(), _daemon_id) == 0){
	    routes->push_back(_client);
	    return 0;
	}else{
	    R14PClient* tmp_client = NULL;
	    std::vector<R14PClient*> tmp_client_lst;
	    // lock client list
	    lock_clients();
	    // search
	    unsigned int count = get_client_count(true);
	    for(unsigned int i = 0; i<count; i++){
		// set pointer
		tmp_client = get_client(i, true);
		// skip if not registered or not active
		if(!tmp_client->is_registered() || !tmp_client->is_active()) continue;
		// check if client end point daemon type and id are acceptable
		if(	strcmp(tmp_client->get_end_point_daemon_type(), _daemon_type) == 0 &&
			strcmp(tmp_client->get_end_point_daemon_id(), _daemon_id) == 0) {

		    // inc ref counter
		    tmp_client->inc_refc();
		    // add to list
		    routes->push_back(tmp_client);
		    // unlock client list
		    unlock_clients();
		    // return (perfect match)
		    return 0;
		// if client has routing capabilities, add to list
		}
	    }

	    // run routing handler if defined or return first matched route
	    // routing list will be filled with routing capable clients
	    if(rh_method != NULL) rh_method->run(&tmp_client_lst, routes);
	    else{
		if(tmp_client_lst.size() > 0){
		    tmp_client_lst[0]->inc_refc();
		    routes->push_back(tmp_client_lst[0]);
		}
	    }
	    // dec ref counters
	    for(unsigned int i = 0; i<tmp_client_lst.size(); i++) tmp_client_lst[i]->dec_refc();

	    // unlock client list
	    unlock_clients();

	}
    }
    // ok
    return 0;
}




bool r14p::R14PSession::is_router(){
    return router;
}

void r14p::R14PSession::remove_callback(R14PEventType callback_type){
	pthread_mutex_lock(&mtx_callback);
	callback_handler.remove_callback(callback_type);
	pthread_mutex_unlock(&mtx_callback);

}

void r14p::R14PSession::set_callback(R14PEventType callback_type, R14PCallbackMethod* callback_method){
	pthread_mutex_lock(&mtx_callback);
	callback_handler.set_callback(callback_type, callback_method);
	pthread_mutex_unlock(&mtx_callback);
}


void r14p::R14PSession::process_callback(R14PEventType type, R14PCallbackArgs* args){
	pthread_mutex_lock(&mtx_callback);
	callback_handler.process_callback(type, args);
	pthread_mutex_unlock(&mtx_callback);
}


unsigned int r14p::R14PSession::inc_thread_count(){
    return thread_count.add_fetch(1);

}

unsigned int r14p::R14PSession::dec_thread_count(){
    return thread_count.sub_fetch(1);
}


unsigned int r14p::R14PSession::get_thread_count(){
    return thread_count.get();
}

void r14p::R14PSession::set_server_socket(int _socket){
    server_socket.set(_socket);

}

int r14p::R14PSession::get_server_socket(){
    return server_socket.get();
}

void r14p::R14PSession::set_server_mode(bool _server_mode){
    server_mode.comp_swap(!_server_mode, _server_mode);

}

bool r14p::R14PSession::get_server_mode(){
    return server_mode.get();
}

char* r14p::R14PSession::get_daemon_id(){
    return daemon_id;
}

char* r14p::R14PSession::get_daemon_type(){
    return daemon_type;
}


void* r14p::R14PSession::server_loop(void* args){
	if(args != NULL){
		// session pointer
		R14PSession* r14ps = (R14PSession*)args;
		int tmp_c = -1;
		sockaddr_in si, pi;
		int size_si = sizeof(sockaddr_in);
		int tmp_s;
		int res;
		pollfd fds_lst[1];
		// set poll timeout to 5 sec
		int poll_timeout = r14ps->poll_interval * 1000;

		// monitor POLLIN event
		fds_lst[0].events = POLLIN;

		// loop
		while(r14ps->get_server_mode()){
			// get server socket
			tmp_s = r14ps->get_server_socket();
			// update socket in poll structure
			fds_lst[0].fd = tmp_s;
			// poll
			res = poll(fds_lst, 1, poll_timeout);
			// check for timeout
			if(res > 0){
				// check for POLLIN event
				if((fds_lst[0].revents & POLLIN) == POLLIN){
					// get client socket and remote peer info
					tmp_c = sctp::get_client(tmp_s, &pi);
					// check if socket is valid
					if(tmp_c > 0) {
						// get local socket info
						getsockname(tmp_c, (sockaddr*)&si, (socklen_t*)&size_si);
						// add client
						R14PClient* client = new R14PClient(	tmp_c,
											inet_ntoa(pi.sin_addr),
											ntohs(pi.sin_port),
											inet_ntoa(si.sin_addr),
											ntohs(si.sin_port),
											R14P_CD_INBOUND,
											r14ps->max_concurrent_streams,
											r14ps->stream_timeout,
											r14ps->poll_interval);

						// set session
						client->set_session(r14ps);
						// add to list
						r14ps->add_client(client);
						// inc thread count
						r14ps->inc_thread_count();
						// start registration
						client->register_client();
						// start client threads
						client->init_threads();


					}
				}


			}

		}
		// detach thread
		pthread_detach(r14ps->server_thread);
		r14ps->server_thread = 0;
		r14ps->dec_thread_count();
	}

	return NULL;

}
int r14p::R14PSession::start_server(const char* bind_address, unsigned int bind_port){
	// server not started
	if(!get_server_mode() && (get_server_socket() < 0)){
		// bind to specific address
		if(bind_address != NULL){
			set_server_socket(sctp::init_sctp_server(inet_addr(bind_address), 0, bind_port));
			set_server_mode(true);
		// bind to INADDR_ANY
		}else{
			set_server_socket(sctp::init_sctp_server(0, 0, bind_port));
			set_server_mode(true);
		}
		// start server thread
		if(get_server_mode()){
			// server thread attribures
			pthread_attr_init(&server_thread_attr);
			// explicit FIFO scheduling for SERVER thread
			pthread_attr_setinheritsched(&server_thread_attr, PTHREAD_EXPLICIT_SCHED);
			pthread_attr_setschedpolicy(&server_thread_attr, SCHED_FIFO);
			// priority
			sched_param server_sp;
			// half priority for SERVER
			server_sp.__sched_priority = 50;
			// set priorities
			pthread_attr_setschedparam(&server_thread_attr, &server_sp);
			// start
		        if(pthread_create(&server_thread, &server_thread_attr, &server_loop, this) == 0){
		            // inc thread count
		            inc_thread_count();
			    // set name
			    pthread_setname_np(server_thread, "r14p_server");
			    // destroy atrributes
			    pthread_attr_destroy(&server_thread_attr);
		        }





		}

	}

	// return server socket
	return get_server_socket();
}


r14p::R14PClient* r14p::R14PSession::get_client(R14PClient* client){
	pthread_mutex_lock(&mtx_clients);
	R14PClient* tmp = NULL;
	for(unsigned int i = 0; i<clients.size(); i++) if(clients[i] == client) {
	    tmp = client;
	    break;
	}
	pthread_mutex_unlock(&mtx_clients);
	return tmp;
}

r14p::R14PClient* r14p::R14PSession::get_client(unsigned int client_index, bool unsafe){
	if(!unsafe) pthread_mutex_lock(&mtx_clients);
	R14PClient* tmp = NULL;
	if(clients.size() > client_index) tmp = clients[client_index];
	if(!unsafe) pthread_mutex_unlock(&mtx_clients);
	return tmp;
}

r14p::R14PClient* r14p::R14PSession::get_registered_client(unsigned int client_index, bool unsafe){
	if(!unsafe) pthread_mutex_lock(&mtx_clients);
	R14PClient* tmp = NULL;
	std::vector<R14PClient*> tmp_lst;
	// get registered clients
	for(unsigned int i = 0; i<clients.size(); i++) if(clients[i]->is_registered()) tmp_lst.push_back(clients[i]);
	// get registered client with index of client_index
	if(tmp_lst.size() > client_index) tmp = tmp_lst[client_index];
	if(!unsafe) pthread_mutex_unlock(&mtx_clients);
	return tmp;
}

r14p::R14PClient* r14p::R14PSession::get_registered_client(const char* daemon_type, bool unsafe){
	if(!unsafe) pthread_mutex_lock(&mtx_clients);
	R14PClient* tmp = NULL;
	std::vector<R14PClient*> tmp_lst;
	// get registered clients
	for(unsigned int i = 0; i<clients.size(); i++) if(clients[i]->is_registered() && strcmp(daemon_type, clients[i]->get_end_point_daemon_type()) == 0) tmp_lst.push_back(clients[i]);
	// get first in the list
	if(tmp_lst.size() > 0) tmp = tmp_lst[0];
	if(!unsafe) pthread_mutex_unlock(&mtx_clients);
	return tmp;
}

r14p::R14PClient* r14p::R14PSession::get_registered_client(const char* daemon_type, const char* daemon_id, bool unsafe){
	if(!unsafe) pthread_mutex_lock(&mtx_clients);
	R14PClient* tmp = NULL;
	// get registered clients
	for(unsigned int i = 0; i<clients.size(); i++) if(clients[i]->is_registered() &&
							  strcmp(daemon_type, clients[i]->get_end_point_daemon_type()) == 0 &&
							  strcmp(daemon_id, clients[i]->get_end_point_daemon_id()) == 0) {

	    tmp = clients[i];
	    if(!unsafe) pthread_mutex_unlock(&mtx_clients);
	    // found
	    return tmp;
	}
	if(!unsafe) pthread_mutex_unlock(&mtx_clients);
	// not found
	return NULL;

}



void r14p::R14PSession::set_routing_algo(R14PRoutingAlgorithm algo){
    switch(algo){
	case R14P_RA_AUTO:
	    set_routing_handler(NULL);
	    break;

	case R14P_RA_WRR:
	    WRRRouteHandler* wrr_rh = new WRRRouteHandler(this);
	    set_routing_handler(wrr_rh);
	    break;
    }
}


void r14p::R14PSession::set_routing_handler(RouteHandlerMethod* rhandler){
    // using mtx_clients mutex, used only by find_route method
    rh_method = rhandler;
}

r14p::RouteHandlerMethod* r14p::R14PSession::get_routing_handler(){
    // using mtx_clients mutex, used only by find_route method
    return rh_method;
}


void r14p::R14PSession::add_client(R14PClient* client){
	pthread_mutex_lock(&mtx_clients);
	clients.push_back(client);
	pthread_mutex_unlock(&mtx_clients);

}

void r14p::R14PSession::lock_clients(){
	pthread_mutex_lock(&mtx_clients);

}

void r14p::R14PSession::unlock_clients(){
	pthread_mutex_unlock(&mtx_clients);

}


int r14p::R14PSession::register_client(R14PClient* client, const char* dest_daemon_type){
    // using semaphore, should not be used in R14P client loops (in/out) or events
    if(client != NULL){

	class _RegClientStreamAllDone: public R14PCallbackMethod {
	public:
		_RegClientStreamAllDone(){
		    sem_init(&signal, 0, 0);
		    status = 0;
		}

		~_RegClientStreamAllDone(){
		    sem_destroy(&signal);
		}

		// event handler method
	        void run(r14p::R14PCallbackArgs* args){
		    r14p::R14PPayload* pld = (r14p::R14PPayload*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_PAYLOAD);
		    // check if all mandatory params were received
		    if(status >= 3) pld->client->set_reg_flag(true);
		    // signal
	            sem_post(&signal);

	        }

	        // signal
	        sem_t signal;
	        int status;

	};

	// Client registration stream next
	class _RegClientStreamDone: public R14PCallbackMethod {
	public:
	        // event handler method
	        void run(r14p::R14PCallbackArgs* args){
	            //std::cout << "_RegClientStreamDone::run" << std::endl;
		    r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
		    r14p::R14PClient* client = stream->get_client();
		    //asn1::R14PMessage* r14pm = stream->get_r14p_message();
		    //bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);
		    asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
		    uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
		    char* tmp_val = NULL;
		    int tmp_val_l = 0;
		    //uint32_t* tmp_ivp;
		    std::string tmp_str;
		    //int c = 0;

		    // check for body
		    if(in_msg != NULL && in_msg->_body != NULL){
			// check for config message
			if(in_msg->_body->_reg->has_linked_data(*in_sess)){
			    // check for GET action
			    if(in_msg->_body->_reg->_reg_action->linked_node->tlv->value[0] == asn1::RegistrationAction::_ra_reg_result){
				// check for params part
				if(in_msg->_body->_reg->_params != NULL){
					if(in_msg->_body->_reg->_params->has_linked_data(*in_sess)){
					    // process params
					    for(unsigned int i = 0; i<in_msg->_body->_reg->_params->children.size(); i++){
						// check for current session
						if(in_msg->_body->_reg->_params->get_child(i)->has_linked_data(*in_sess)){
						    // check param id, convert from big endian to host
						    uint32_t* param_id = (uint32_t*)in_msg->_body->_reg->_params->get_child(i)->_id->linked_node->tlv->value;
						    // check for value
						    if(in_msg->_body->_reg->_params->get_child(i)->_value != NULL){
							    // check if value exists in current session
							    if(in_msg->_body->_reg->_params->get_child(i)->_value->has_linked_data(*in_sess)){
								    // check if child exists
								    if(in_msg->_body->_reg->_params->get_child(i)->_value->get_child(0) != NULL){
									// check if child exists in current sesion
									if(in_msg->_body->_reg->_params->get_child(i)->_value->get_child(0)->has_linked_data(*in_sess)){
									    // set tmp values
									    tmp_val = (char*)in_msg->_body->_reg->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value;
									    tmp_val_l = in_msg->_body->_reg->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value_length;

									    // match param
									    switch(be32toh(*param_id)){
										// daemon type
										case asn1::ParameterType::_pt_pmink_daemon_type:
										    tmp_str.clear();
										    tmp_str.append(tmp_val, tmp_val_l);
										    client->set_end_point_daemon_type(tmp_str.c_str());
										    ++adone.status;
										    break;

										// daemon id
										case asn1::ParameterType::_pt_pmink_daemon_id:
										    tmp_str.clear();
										    tmp_str.append(tmp_val, tmp_val_l);
										    client->set_end_point_daemon_id(tmp_str.c_str());
										    ++adone.status;
										    break;

										// router status
										case asn1::ParameterType::_pt_pmink_router_status:
										    client->set_router_flag(tmp_val[0] == 0 ? false : true);
										    ++adone.status;
										    break;



									    }

									}
								    }
							    }
						    }
						}
					    }
					}
				}
			    }

			}
			// wait until stream complete was properly sent
			stream->set_callback(r14p::R14P_ET_PAYLOAD_SENT, &adone);
			return;
		    }

		    // *** stream timeout ***<

		    // signal
	            sem_post(&adone.signal);
	        }

	        _RegClientStreamAllDone adone;

	};

	// Client registration stream done
	class _RegClientStreamNext: public R14PCallbackMethod {
	public:
	        // event handler method
	        void run(r14p::R14PCallbackArgs* args){
		    r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
		    // end stream
		    stream->end_sequence();

	        }

	};

	// events
	_RegClientStreamDone sdone;
	_RegClientStreamNext snext;
	// start new R14P stream
	r14p::R14PStream* r14p_stream = client->new_stream(dest_daemon_type, NULL, NULL, &snext);
	// if stream cannot be created, return err
	if(r14p_stream == NULL) return 1;
	// set end event handler
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, &sdone);
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, &sdone);
	// create body
	asn1::R14PMessage* r14pm = r14p_stream->get_r14p_message();
	// prepare body
	if(r14pm->_body != NULL) {
		r14pm->_body->unlink(1);
		r14pm->_body->_conf->set_linked_data(1);

	}else{
		r14pm->set_body();
		r14pm->prepare();
	}
	// set bodu
	uint32_t pm_dtype = htobe32(asn1::ParameterType::_pt_pmink_daemon_type);
	uint32_t pm_did = htobe32(asn1::ParameterType::_pt_pmink_daemon_id);
	uint32_t pm_router = htobe32(asn1::ParameterType::_pt_pmink_router_status);
	uint32_t reg_action = asn1::RegistrationAction::_ra_reg_request;
	int router_flag = (client->get_session()->is_router() ? 1 : 0);
	// set params
	if(r14pm->_body->_reg->_params == NULL){
		r14pm->_body->_reg->set_params();
		// set children, allocate more
		for(int i = 0; i<3; i++){
			r14pm->_body->_reg->_params->set_child(i);
			r14pm->_body->_reg->_params->get_child(i)->set_value();
			r14pm->_body->_reg->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	// unlink params before setting new ones
	}else{
	    int cc = r14pm->_body->_reg->_params->children.size();
	    if(cc < 3){
		// set children, allocate more
		for(int i = cc; i<3; i++){
			r14pm->_body->_reg->_params->set_child(i);
			r14pm->_body->_reg->_params->get_child(i)->set_value();
			r14pm->_body->_reg->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	    }else if(cc > 3){
		// remove extra children if used in some other session, only 2 needed
		for(int i = 3; i<cc; i++) r14pm->_body->_reg->_params->get_child(i)->unlink(1);
	    }
	}
	// set reg action
	r14pm->_body->_reg->_reg_action->set_linked_data(1, (unsigned char*)&reg_action, 1);

	// set daemon type
	r14pm->_body->_reg->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pm_dtype, sizeof(uint32_t));
	r14pm->_body->_reg->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)client->get_session()->get_daemon_type(), strlen(client->get_session()->get_daemon_type()));

	// set daemon id
	r14pm->_body->_reg->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&pm_did, sizeof(uint32_t));
	r14pm->_body->_reg->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)client->get_session()->get_daemon_id(), strlen(client->get_session()->get_daemon_id()));

	// set router flag
	r14pm->_body->_reg->_params->get_child(2)->_id->set_linked_data(1, (unsigned char*)&pm_router, sizeof(uint32_t));
	r14pm->_body->_reg->_params->get_child(2)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&router_flag, 1);

	// start stream
	r14p_stream->send(true);

	// wait for signal
	timespec ts;
	clock_gettime(0, &ts);
	ts.tv_sec += 10;
	int sres = sem_wait(&sdone.adone.signal);
	// error check
	if(sres == -1) return 1;
	// check if registered
	if(client->is_registered()) return 0; else return 1;
    }

    // err
    return 1;
}


unsigned int r14p::R14PSession::get_client_count(bool unsafe){
	if(!unsafe) pthread_mutex_lock(&mtx_clients);
	unsigned int tmp = clients.size();
	if(!unsafe) pthread_mutex_unlock(&mtx_clients);
	return tmp;
}

int r14p::R14PSession::remove_client(unsigned int client_index){
	// lock mutex
	pthread_mutex_lock(&mtx_clients);
	// validate index
	if(clients.size() > client_index){
		// save pointer to erased client
		R14PClient* tmp = clients[client_index];
		// erase
		clients.erase(clients.begin() + client_index);
		// unlock mutex
		pthread_mutex_unlock(&mtx_clients);
		// free mem
		delete tmp;
		// ok
		return 0;
	}
	//unlock mutex
	pthread_mutex_unlock(&mtx_clients);
	// return error, not found
	return -1;

}

int r14p::R14PSession::remove_client(R14PClient* r14p_client){
	//lock mutex
	pthread_mutex_lock(&mtx_clients);
	if(r14p_client != NULL){
		for(unsigned int i = 0; i<clients.size(); i++) if(clients[i] == r14p_client) {
			// erase
			clients.erase(clients.begin() + i);
			// remove from routing
			if(rh_method != NULL){
			    rh_method->update_client(NULL,
						     r14p_client->get_end_point_daemon_type(),
						     r14p_client->get_end_point_daemon_id());
			}

			// unlock mutex
			pthread_mutex_unlock(&mtx_clients);
			// wait for ref counter to become zero
			if(r14p_client->dec_refc() > 0){
			    // pause 100msec
			    timespec st = {0, 100000000};
			    // loop wait
			    while(r14p_client->get_refc() > 0) nanosleep(&st, NULL);
			}


			// **** free out queues ****
			R14PPayload* r14ppld = NULL;
			// internal out
			while(r14p_client->internal_out_queue.pop(&r14ppld) == 0){
			    // free memory, return back to pool
			    if(r14ppld->free_on_send){
				if(r14ppld->stream->linked_stream != NULL) r14p_client->deallocate_stream_pool(r14ppld->stream->linked_stream);
				r14p_client->deallocate_stream_pool(r14ppld->stream);

			    }
			}
			// external out
			while((r14ppld = r14p_client->pop_out_queue()) != NULL){
			    // free memory, return back to pool
			    if(r14ppld->free_on_send){
				if(r14ppld->stream->linked_stream != NULL) r14p_client->deallocate_stream_pool(r14ppld->stream->linked_stream);
				r14p_client->deallocate_stream_pool(r14ppld->stream);

			    }
			}


			// process callback
			R14PCallbackArgs cb_args;
			cb_args.clear_all_args();
			cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, r14p_client);
			process_callback(R14P_ET_CLIENT_DESTROYED, &cb_args);

			// free mem, no refs present
			delete r14p_client;

			// dec thread count
			dec_thread_count();
			// found, return ok
			return 0;
		}
	}
	// unlock mutex
	pthread_mutex_unlock(&mtx_clients);
	// return error, not found
	return -1;
}



r14p::R14PClient* r14p::R14PSession::connect(	const char* end_point_address,
						unsigned int end_point_port,
						int stream_count,
						const char* local_address,
						unsigned int local_port,
						bool skip_r14p_reg){

	if(end_point_address == NULL || end_point_port == 0 || stream_count == 0) return NULL;
	// client
	int client_id = -1;
	if(local_address == NULL || local_port == 0){
		// connect
		// connect and bind to specific ip:port
		client_id = sctp::init_sctp_client_bind(inet_addr(end_point_address),
							0,
							0,
							0,
							0,
							end_point_port,
							stream_count);

	}else{
		// connect and bind to specific ip:port
		client_id = sctp::init_sctp_client_bind(inet_addr(end_point_address),
							0,
							inet_addr(local_address),
							0,
							local_port,
							end_point_port,
							stream_count);

	}

	// create client if socket is valid
	if(client_id > 0){
		// socket info
		sockaddr_in si;
		int size_si = sizeof(sockaddr_in);
		// get local socket info
		getsockname(client_id, (sockaddr*)&si, (socklen_t*)&size_si);
		// add client
		R14PClient* client = new R14PClient(	client_id,
							end_point_address,
							end_point_port,
							inet_ntoa(si.sin_addr),
							ntohs(si.sin_port),
							R14P_CD_OUTBOUND,
							max_concurrent_streams,
							stream_timeout,
							poll_interval);
		client->set_session(this);
		inc_thread_count();
		// start client threads
		client->init_threads();
		// R14P registration
		if(!skip_r14p_reg){
		    // add client if registered
		    if(register_client(client, ".") == 0){
			// process callback
			R14PCallbackArgs cb_args;
			cb_args.clear_all_args();
			cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, client);
			process_callback(R14P_ET_CLIENT_CREATED, &cb_args);

			add_client(client);

		    // free client on registration error
		    }else{
			client->get_session()->remove_client(client);
			client = NULL;
		    }

		}else{
		    add_client(client);

		}

		// return client
		return client;
	}
	// connection error
	return NULL;
}


// namespace methods
r14p::R14PSession* r14p::init_session(const char* _daemon_type, const char* _daemon_id, int _max_concurrent_streams, int _stream_timeout, bool _router, int _poll_interval){
	return new R14PSession(_daemon_type, _daemon_id, _max_concurrent_streams, _stream_timeout, _router, _poll_interval);
}

int r14p::destroy_session(R14PSession* r14p_session){
	if(r14p_session != NULL){
		r14p_session->lock_clients();
		for(unsigned int i = 0; i<r14p_session->get_client_count(true); i++){
		    r14p_session->get_client(i, true)->disconnect();
		}
		r14p_session->unlock_clients();

		// wait for session threads to finish

		timespec st = {0, 100000000};
		while(r14p_session->get_thread_count() > 0){
		    nanosleep(&st, NULL);
		}
		// free session
		delete r14p_session;
		// ok
		return 0;
	}
	// error
	return 1;

}

void r14p::stop_heartbeat(HeartbeatInfo* hi){
    if(hi != NULL) hi->set_activity(false);
}

r14p::HeartbeatInfo* r14p::init_heartbeat(	const char* _daemon_type,
						const char* _daemon_id,
						R14PClient* _client,
						unsigned int interval,
						R14PCallbackMethod* _on_received,
						R14PCallbackMethod* _on_missed,
						R14PCallbackMethod* _on_cleanup){

    if(_daemon_type != NULL && _daemon_id != NULL && _client != NULL){
	// check size
	if(strlen(_daemon_type) > 16 || strlen(_daemon_id) > 16) return NULL;
	// crete heartbeat info object
	HeartbeatInfo* hi = new HeartbeatInfo();
	hi->set_activity(true);
	hi->set_next(true);
	hi->r14pc = _client;
	hi->interval = (interval < 1 ? 1 : interval);
	hi->on_missed = _on_missed;
	hi->on_received = _on_received;
	hi->on_cleanup = _on_cleanup;
	memcpy(hi->target_daemon_type, _daemon_type, strlen(_daemon_type) + 1);
	memcpy(hi->target_daemon_id, _daemon_id, strlen(_daemon_id) + 1);
	// start thread
	pthread_t tmp_thr;
	if(pthread_create(&tmp_thr, NULL, &hi->heartbeat_loop, hi) == 0){
	    _client->inc_thread_count();
	    pthread_setname_np(tmp_thr, "r14p_hbeat");
	    return hi;
	}else{
	    delete hi;
	    return NULL;
	}
    }

    // err
    return NULL;
}




