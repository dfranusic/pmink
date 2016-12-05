#include <r14p_stats.h>



void r14p::TrapClientDone::run(r14p::R14PCallbackArgs* args){

}

r14p::TrapClientNew::TrapClientNew(){
    ss = NULL;
}

void r14p::TrapClientNew::run(r14p::R14PCallbackArgs* args){
	r14p::R14PClient* client = (r14p::R14PClient*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_CLIENT);
	client->set_callback(r14p::R14P_ET_STREAM_NEW, &snew);

}

r14p::TrapStreamNew::TrapStreamNew(){
	//trap_index = 0;
	trap_count = 0;
	ss = NULL;
	snew = NULL;
	stats_action = asn1::StatsAction::_sa_result;
	pt_stats_id = htobe32(asn1::ParameterType::_pt_pmink_stats_id);
	pt_stats_count = htobe32(asn1::ParameterType::_pt_pmink_stats_count);
	pt_stats_value = htobe32(asn1::ParameterType::_pt_pmink_stats_value);
	pt_stats_desc = htobe32(asn1::ParameterType::_pt_pmink_stats_description);

}

void r14p::TrapStreamNext::run(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);

	// check for trap ids
	if(sdone.snew->trap_iter != sdone.snew->traps.end()){

		// prepare body
		if(r14pm->_body != NULL) {
			r14pm->_body->unlink(1);
			r14pm->_body->_stats->set_linked_data(1);

		}else{
			r14pm->set_body();
			r14pm->prepare();
		}
		// set params
		if(r14pm->_body->_stats->_params == NULL){
			r14pm->_body->_stats->set_params();
			// set children, allocate more
			for(int i = 0; i<2; i++){
				r14pm->_body->_stats->_params->set_child(i);
				r14pm->_body->_stats->_params->get_child(i)->set_value();
				r14pm->_body->_stats->_params->get_child(i)->_value->set_child(0);

			}
			// prepare
			r14pm->prepare();

		// unlink params before setting new ones
		}else{
		    int cc = r14pm->_body->_stats->_params->children.size();
		    if(cc < 2){
			// set children, allocate more
			for(int i = cc; i<3; i++){
				r14pm->_body->_stats->_params->set_child(i);
				r14pm->_body->_stats->_params->get_child(i)->set_value();
				r14pm->_body->_stats->_params->get_child(i)->_value->set_child(0);

			}
			// prepare
			r14pm->prepare();

		    }else if(cc > 2){
			// remove extra children if used in some other session, only 4 needed
			for(int i = 2; i<cc; i++) r14pm->_body->_stats->_params->get_child(i)->unlink(1);
		    }
		}

		// set stats action
		r14pm->_body->_stats->_stats_action->set_linked_data(1, (unsigned char*)&sdone.snew->stats_action, 1);

		// stats id
		//r14pm->_body->_stats->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&sdone.snew->pt_stats_id, sizeof(uint32_t));
		//r14pm->_body->_stats->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&sdone.snew->trap_iter->first.id, sizeof(uint32_t));
		r14pm->_body->_stats->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&sdone.snew->pt_stats_id, sizeof(uint32_t));
		r14pm->_body->_stats->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)sdone.snew->trap_iter->first.label.c_str(), sdone.snew->trap_iter->first.label.length());

		// stats value
		r14pm->_body->_stats->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&sdone.snew->pt_stats_value, sizeof(uint32_t));
		r14pm->_body->_stats->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&sdone.snew->trap_iter->second, sizeof(uint64_t));

		// stats desc
		//r14pm->_body->_stats->_params->get_child(2)->_id->set_linked_data(1, (unsigned char*)&sdone.snew->pt_stats_desc, sizeof(uint32_t));
		//r14pm->_body->_stats->_params->get_child(2)->_value->get_child(0)->set_linked_data(1, (unsigned char*)sdone.snew->trap_iter->first.description, strlen(sdone.snew->trap_iter->first.description));

		// include body
		*include_body = true;

		// continue stream
		stream->continue_sequence();

		// next index
		++sdone.snew->trap_iter;
		//++sdone.snew->trap_index;

	// end stream
	}else stream->end_sequence();
}

void r14p::TrapStreamNew::run(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	char* tmp_val = NULL;
	int tmp_val_l = 0;

	// stream new fork
	snew = new TrapStreamNew();
	snew->snext.sdone.snew = snew;
	snew->ss = ss;


	// set events
	stream->set_callback(r14p::R14P_ET_STREAM_NEXT, &snew->snext);
	stream->set_callback(r14p::R14P_ET_STREAM_END, &snew->snext.sdone);
	stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, &snew->snext.sdone);


	// check for body
	if(in_msg->_body != NULL){
	    // check for config message
	    if(in_msg->_body->_stats->has_linked_data(*in_sess)){
		// check for GET action
		if(in_msg->_body->_stats->_stats_action->linked_node->tlv->value[0] == asn1::StatsAction::_sa_request){
		    // check for params part
		    if(in_msg->_body->_stats->_params != NULL){
			    if(in_msg->_body->_stats->_params->has_linked_data(*in_sess)){
				// process params
				for(unsigned int i = 0; i<in_msg->_body->_stats->_params->children.size(); i++){
				    // check for current session
				    if(in_msg->_body->_stats->_params->get_child(i)->has_linked_data(*in_sess)){
					// check param id, convert from big endian to host
					uint32_t* param_id = (uint32_t*)in_msg->_body->_stats->_params->get_child(i)->_id->linked_node->tlv->value;
					// check for value
					if(in_msg->_body->_stats->_params->get_child(i)->_value != NULL){
						// check if value exists in current session
						if(in_msg->_body->_stats->_params->get_child(i)->_value->has_linked_data(*in_sess)){
							// check if child exists
							if(in_msg->_body->_stats->_params->get_child(i)->_value->get_child(0) != NULL){
							    // check if child exists in current sesion
							    if(in_msg->_body->_stats->_params->get_child(i)->_value->get_child(0)->has_linked_data(*in_sess)){
								// set tmp values
								tmp_val = (char*)in_msg->_body->_stats->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value;
								tmp_val_l = in_msg->_body->_stats->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value_length;

								// match param
								switch(be32toh(*param_id)){
								    // config item count
								    case asn1::ParameterType::_pt_pmink_stats_id:
									TrapId tmp_trap_id;
									//tmp_trap_id.id = be32toh(*tmp_ivp);
									tmp_trap_id.label.assign(tmp_val, tmp_val_l);

									// lock
									ss->lock();

									// check for special 0 (ALL) id
									if(tmp_trap_id.label == "0"){
									    std::map<r14p::TrapId, r14p::R14PTrapHandler*, r14p::TrapIdCompare>* tmp_map = ss->get_trap_map();
									    typedef std::map<r14p::TrapId, r14p::R14PTrapHandler*, r14p::TrapIdCompare>::iterator it_type;
									    for(it_type it = tmp_map->begin(); it != tmp_map->end(); it++) {
										tmp_trap_id = it->first;
										//tmp_trap_id.id = htobe32(tmp_trap_id.id);
										snew->traps[tmp_trap_id] = htobe64(it->second->value);

									    }

									// normal id
									}else{
										// check if trap exists
										R14PTrapHandler* tmp_trph = ss->get_trap(&tmp_trap_id, true);
										if(tmp_trph != NULL){
											//tmp_trap_id.id = htobe32(tmp_trap_id.id);
											snew->traps[tmp_trap_id] = htobe64(tmp_trph->value);

										}

									}

									// unlock
									ss->unlock();
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
		// not stats request
		}else{
		    stream->end_sequence();

		}
	    }
	}

	// check for trap ids
	if(snew->traps.size() > 0){
		// set iterator
		snew->trap_iter = snew->traps.begin();

		// prepare body
		if(r14pm->_body != NULL) {
			r14pm->_body->unlink(1);
			r14pm->_body->_stats->set_linked_data(1);

		}else{
			r14pm->set_body();
			r14pm->prepare();
		}
		// set params
		if(r14pm->_body->_stats->_params == NULL){
			r14pm->_body->_stats->set_params();
			// set children, allocate more
			for(int i = 0; i<1; i++){
				r14pm->_body->_stats->_params->set_child(i);
				r14pm->_body->_stats->_params->get_child(i)->set_value();
				r14pm->_body->_stats->_params->get_child(i)->_value->set_child(0);

			}
			// prepare
			r14pm->prepare();

		// unlink params before setting new ones
		}else{
		    int cc = r14pm->_body->_stats->_params->children.size();
		    if(cc < 1){
			// set children, allocate more
			for(int i = cc; i<1; i++){
				r14pm->_body->_stats->_params->set_child(i);
				r14pm->_body->_stats->_params->get_child(i)->set_value();
				r14pm->_body->_stats->_params->get_child(i)->_value->set_child(0);

			}
			// prepare
			r14pm->prepare();

		    }else if(cc > 1){
			// remove extra children if used in some other session, only 4 needed
			for(int i = 1; i<cc; i++) r14pm->_body->_stats->_params->get_child(i)->unlink(1);
		    }
		}

		// set stats action
		r14pm->_body->_stats->_stats_action->set_linked_data(1, (unsigned char*)&stats_action, 1);

		// stats count
		snew->trap_count = htobe32(snew->traps.size());
		r14pm->_body->_stats->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pt_stats_count, sizeof(uint32_t));
		r14pm->_body->_stats->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&snew->trap_count, sizeof(uint32_t));

		// include body
		*include_body = true;


		// continue stream
		stream->continue_sequence();



	// end stream
	}else stream->end_sequence();





}

void r14p::TrapStreamDone::run(r14p::R14PCallbackArgs* args){
    delete snew;
}


r14p::R14PTrapHandler::R14PTrapHandler(){
    value = 0;
}

r14p::R14PTrapHandler::~R14PTrapHandler(){

}

void r14p::R14PTrapHandler::run(){

}


bool r14p::TrapIdCompare::operator () (const TrapId& x, const TrapId& y){
    //return x.id < y.id;
    return x.label < y.label;

}



r14p::TrapId::TrapId(const char* _label){
    if(_label != NULL) label.assign(_label);
}

r14p::TrapId::TrapId(const std::string& _label){
    label.assign(_label);
}


// R14PStatsHandler
r14p::R14PStatsHandler::R14PStatsHandler(pmink::Atomic<uint64_t>* _sval_p): sval_p(_sval_p){}

void r14p::R14PStatsHandler::run(){
    value = sval_p->get();

}

// R14PStatsClientCreated
void r14p::R14PStatsClientCreated::run(R14PCallbackArgs* args){
    R14PClient* r14pc = (R14PClient*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT);
    std::string tmp;

    // in stats
    tmp.assign("R14P_IN_");
    tmp.append(r14pc->get_end_point_daemon_type());
    tmp.append("_");
    tmp.append(r14pc->get_end_point_daemon_id());
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_PACKETS")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->packets));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_BYTES")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->bytes));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAMS")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->streams));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAM_BYTES")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->stream_bytes));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAM_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->stream_errors));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_DISCARDED")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->discarded));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_MALFORMED")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->malformed));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_SOCKET_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->socket_errors));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_POOL_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->strm_alloc_errors));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAM_TIMEOUT")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->strm_timeout));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAM_LOOPBACK")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->strm_loopback));

    // out stats
    tmp.assign("R14P_OUT_");
    tmp.append(r14pc->get_end_point_daemon_type());
    tmp.append("_");
    tmp.append(r14pc->get_end_point_daemon_id());
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_PACKETS")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->packets));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_BYTES")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->bytes));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAMS")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->streams));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAM_BYTES")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->stream_bytes));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAM_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->stream_errors));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_DISCARDED")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->discarded));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_MALFORMED")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->malformed));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_SOCKET_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->socket_errors));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_POOL_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->strm_alloc_errors));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAM_TIMEOUT")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->strm_timeout));
    r14p_stats->add_trap(r14p::TrapId(std::string(tmp + "_STREAM_LOOPBACK")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->strm_loopback));

}

// R14PStatsClientDestroyed
void r14p::R14PStatsClientDestroyed::run(R14PCallbackArgs* args){
    R14PClient* r14pc = (R14PClient*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT);
    std::string tmp;

    // in stats
    tmp.assign("R14P_IN_");
    tmp.append(r14pc->get_end_point_daemon_type());
    tmp.append("_");
    tmp.append(r14pc->get_end_point_daemon_id());
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_PACKETS")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_BYTES")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAMS")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAM_BYTES")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAM_ERR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_DISCARDED")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_MALFORMED")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_SOCKET_ERR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_POOL_ERR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAM_TIMEOUT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAM_LOOPBACK")));

    // out stats
    tmp.assign("R14P_OUT_");
    tmp.append(r14pc->get_end_point_daemon_type());
    tmp.append("_");
    tmp.append(r14pc->get_end_point_daemon_id());
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_PACKETS")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_BYTES")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAMS")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAM_BYTES")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAM_ERR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_DISCARDED")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_MALFORMED")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_SOCKET_ERR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_POOL_ERR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAM_TIMEOUT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(tmp + "_STREAM_LOOPBACK")));

}


// R14PStatsSession
r14p::R14PStatsSession::R14PStatsSession(int _poll_interval, r14p::R14PSession* _host_r14ps, int _stats_port){
    active = false;
    pthread_mutex_init(&mtx_stats, NULL);
    thread_count = 0;
    poll_interval = _poll_interval;
    host_r14ps = _host_r14ps;
    r14ps = NULL;
    stats_port = _stats_port;
    client_created.r14p_stats = this;
    client_destroyed.r14p_stats = this;
}

r14p::R14PStatsSession::~R14PStatsSession(){
    set_activity(false);
    timespec st = {0, 100000000};
    while(get_thread_count() > 0){
	    nanosleep(&st, NULL);

    }
    r14ps->stop_server();
    r14p::destroy_session(r14ps);

}

void r14p::R14PStatsSession::init_r14p_session_stats(R14PSession* _r14ps){
    if(_r14ps == NULL) return;

    // set events
    _r14ps->set_callback(R14P_ET_CLIENT_CREATED, &client_created);
    _r14ps->set_callback(R14P_ET_CLIENT_DESTROYED, &client_destroyed);

    /*
    std::string tmp;
    // add clients
    _r14ps->lock_clients();
    for(unsigned int i = 0; i<_r14ps->get_client_count(true); i++){

	// clint
	R14PClient* r14pc = _r14ps->get_client(i, true);
	// in stats
	tmp.assign("R14P_IN_");
	tmp.append(r14pc->get_end_point_daemon_type());
	tmp.append("_");
	tmp.append(r14pc->get_end_point_daemon_id());
	add_trap(r14p::TrapId(std::string(tmp + "_PACKETS")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->packets));
	add_trap(r14p::TrapId(std::string(tmp + "_BYTES")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->bytes));
	add_trap(r14p::TrapId(std::string(tmp + "_STREAMS")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->streams));
	add_trap(r14p::TrapId(std::string(tmp + "_STREAM_BYTES")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->stream_bytes));
	add_trap(r14p::TrapId(std::string(tmp + "_STREAM_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->stream_errors));
	add_trap(r14p::TrapId(std::string(tmp + "_DISCARDED")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->discarded));
	add_trap(r14p::TrapId(std::string(tmp + "_MALFORMED")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->malformed));
	add_trap(r14p::TrapId(std::string(tmp + "_SOCKET_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->socket_errors));
	add_trap(r14p::TrapId(std::string(tmp + "_POOL_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_INBOUND_STATS)->strm_alloc_errors));

	// out stats
	tmp.assign("R14P_OUT_");
	tmp.append(r14pc->get_end_point_daemon_type());
	tmp.append("_");
	tmp.append(r14pc->get_end_point_daemon_id());
	add_trap(r14p::TrapId(std::string(tmp + "_PACKETS")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->packets));
	add_trap(r14p::TrapId(std::string(tmp + "_BYTES")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->bytes));
	add_trap(r14p::TrapId(std::string(tmp + "_STREAMS")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->streams));
	add_trap(r14p::TrapId(std::string(tmp + "_STREAM_BYTES")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->stream_bytes));
	add_trap(r14p::TrapId(std::string(tmp + "_STREAM_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->stream_errors));
	add_trap(r14p::TrapId(std::string(tmp + "_DISCARDED")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->discarded));
	add_trap(r14p::TrapId(std::string(tmp + "_MALFORMED")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->malformed));
	add_trap(r14p::TrapId(std::string(tmp + "_SOCKET_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->socket_errors));
	add_trap(r14p::TrapId(std::string(tmp + "_POOL_ERR")), new R14PStatsHandler(&r14pc->get_stats(R14P_OUTBOUND_STATS)->strm_alloc_errors));


    }
    _r14ps->unlock_clients();
    */
}


unsigned int r14p::R14PStatsSession::inc_thread_count(){
	pthread_mutex_lock(&mtx_stats);
	unsigned int tmp = ++thread_count;
	pthread_mutex_unlock(&mtx_stats);
	return tmp;

}

unsigned int r14p::R14PStatsSession::dec_thread_count(){
	pthread_mutex_lock(&mtx_stats);
	unsigned int tmp = --thread_count;
	pthread_mutex_unlock(&mtx_stats);
	return tmp;

}


unsigned int r14p::R14PStatsSession::get_thread_count(){
	pthread_mutex_lock(&mtx_stats);
	unsigned int tmp = thread_count;
	pthread_mutex_unlock(&mtx_stats);
	return tmp;

}

void r14p::R14PStatsSession::set_activity(bool _is_active){
	pthread_mutex_lock(&mtx_stats);
	active = _is_active;
	pthread_mutex_unlock(&mtx_stats);

}

bool r14p::R14PStatsSession::is_active(){
	pthread_mutex_lock(&mtx_stats);
	bool tmp = active;
	pthread_mutex_unlock(&mtx_stats);
	return tmp;

}


int r14p::R14PStatsSession::add_trap(const TrapId* trap_id, R14PTrapHandler* handler){
    typedef std::map<TrapId, R14PTrapHandler*, TrapIdCompare>::iterator it_type;
    pthread_mutex_lock(&mtx_stats);
    int res = 0;
    it_type it = trap_map.find(*trap_id);
    if(it != trap_map.end()) res = 1; else{
	if(trap_id->label != "0") trap_map[*trap_id] = handler; else res = 1;
    }
    pthread_mutex_unlock(&mtx_stats);
    return res;
}

int r14p::R14PStatsSession::add_trap(const TrapId& trap_id, R14PTrapHandler* handler){
    typedef std::map<TrapId, R14PTrapHandler*, TrapIdCompare>::iterator it_type;
    pthread_mutex_lock(&mtx_stats);
    int res = 0;
    it_type it = trap_map.find(trap_id);
    if(it != trap_map.end()) res = 1; else{
	if(trap_id.label != "0") trap_map[trap_id] = handler; else res = 1;
    }
    pthread_mutex_unlock(&mtx_stats);
    return res;
}


r14p::R14PTrapHandler* r14p::R14PStatsSession::remove_trap(const TrapId& trap_id){
    pthread_mutex_lock(&mtx_stats);
    std::map<TrapId, R14PTrapHandler*, TrapIdCompare>::iterator it = trap_map.find(trap_id);
    if(it == trap_map.end()){
	pthread_mutex_unlock(&mtx_stats);
	return NULL;
    }
    R14PTrapHandler* res = it->second;
    trap_map.erase(it);
    pthread_mutex_unlock(&mtx_stats);
    return res;

}

uint64_t r14p::R14PStatsSession::get_trap_value(TrapId* trap_id){
    typedef std::map<TrapId, R14PTrapHandler*, TrapIdCompare>::iterator it_type;
    uint64_t res = 0;
    pthread_mutex_lock(&mtx_stats);
    it_type it = trap_map.find(*trap_id);
    if(it != trap_map.end()){
	res = it->second->value;
	//memcpy(trap_id->description, it->first.description, sizeof(trap_id->description));
    }
    pthread_mutex_unlock(&mtx_stats);
    return res;

}


void r14p::R14PStatsSession::lock(){
    pthread_mutex_lock(&mtx_stats);

}

void r14p::R14PStatsSession::unlock(){
    pthread_mutex_unlock(&mtx_stats);

}

r14p::R14PTrapHandler* r14p::R14PStatsSession::get_trap(TrapId* trap_id, bool unsafe){
    typedef std::map<TrapId, R14PTrapHandler*, TrapIdCompare>::iterator it_type;
    R14PTrapHandler* tmp_handler = NULL;
    if(!unsafe) pthread_mutex_lock(&mtx_stats);
    it_type it = trap_map.find(*trap_id);
    if(it != trap_map.end()){
	tmp_handler = it->second;
	//memcpy(trap_id->description, it->first.description, sizeof(trap_id->description));
    }
    if(!unsafe) pthread_mutex_unlock(&mtx_stats);
    return tmp_handler;
}

std::map<r14p::TrapId, r14p::R14PTrapHandler*, r14p::TrapIdCompare>* r14p::R14PStatsSession::get_trap_map(){
    return &trap_map;

}

void r14p::R14PStatsSession::setup_client(r14p::R14PClient* _client){
    _client->set_callback(r14p::R14P_ET_STREAM_NEW, &new_client.snew);

}


void r14p::R14PStatsSession::start(){
	std::string tmp_dtype;
	std::string tmp_did;

	// set activity flag
	active = true;

	// set daemon type and id
	tmp_dtype.append("%");
	tmp_dtype.append(host_r14ps->get_daemon_type());
	tmp_did.append("%");
	tmp_did.append(host_r14ps->get_daemon_id());

	// start R14P session
	r14ps = r14p::init_session(tmp_dtype.c_str(), tmp_did.c_str(), 100, 5, false, 5);
	// accept connections (server mode)
	if(stats_port > 0) r14ps->start_server(NULL, stats_port);
	// events
	new_client.ss = this;
	new_client.snew.ss = this;
	r14ps->set_callback(r14p::R14P_ET_CLIENT_NEW, &new_client);
	r14ps->set_callback(r14p::R14P_ET_CLIENT_TERMINATED, &client_done);

	// init trap thread
	pthread_t tmp_thread;
	if(pthread_create(&tmp_thread, NULL, &trap_loop, this) == 0){
	    inc_thread_count();
	    pthread_setname_np(tmp_thread, "r14p_stats");
	}

}

void r14p::R14PStatsSession::stop(){
    set_activity(false);
}

r14p::R14PSession* r14p::R14PStatsSession::get_r14p_session(){
    return r14ps;
}



void* r14p::R14PStatsSession::trap_loop(void* args){
    if(args != NULL){
	R14PStatsSession* ss = (R14PStatsSession*)args;
	typedef std::map<TrapId, R14PTrapHandler*, TrapIdCompare>::iterator it_type;
	std::map<TrapId, R14PTrapHandler*, TrapIdCompare>* tmp_map = ss->get_trap_map();
	R14PTrapHandler* tmp_handler = NULL;
	int total_sleep = 0;

	// loop
	while(ss->is_active()){
	    // sleep 1 sec
	    sleep(1);
	    ++total_sleep;
	    // check if user timeout has been reached
	    if(total_sleep < ss->poll_interval) continue;
	    // reset current timeout
	    total_sleep = 0;

	    // lock
	    pthread_mutex_lock(&ss->mtx_stats);

	    // loop
	    for(it_type it = tmp_map->begin(); it != tmp_map->end(); it++) {
		tmp_handler = it->second;
		// run handler
		tmp_handler->run();

	    }

	    // unlock
	    pthread_mutex_unlock(&ss->mtx_stats);

	}
	// detach thread
	pthread_detach(pthread_self());
	ss->dec_thread_count();

    }

    return NULL;
}


