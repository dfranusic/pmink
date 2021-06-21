/*
 * This file is part of Project MINK <http://www.release14.org>.
 *
 * Copyright (C) 2012 Release14 Ltd.
 * http://www.release14.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <drd_events.h>
#include <r14p_utils.h>
#include <dr.h>


dr::HbeatMissed::HbeatMissed(pmink::Atomic<uint8_t>* _activity_flag){
    activity_flag = _activity_flag;

}

void dr::HbeatMissed::run(r14p::R14PCallbackArgs* args){
	r14p::HeartbeatInfo* hi = (r14p::HeartbeatInfo*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_HBEAT_INFO);
	// set activity flag to false
	activity_flag->comp_swap(true, false);
	// stop heartbeat
	r14p::stop_heartbeat(hi);
	// display warning
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "R14P HBEAT not received, closing connection to [%s]...",
				   hi->target_daemon_id);

}

void dr::HbeatRecv::run(r14p::R14PCallbackArgs* args){
    // do nothing
}

dr::HbeatCleanup::HbeatCleanup(HbeatRecv* _recv, HbeatMissed* _missed){
	recv = _recv;
	missed = _missed;

}

void dr::HbeatCleanup::run(r14p::R14PCallbackArgs* args){
	delete recv;
	delete missed;
	delete this;

	// get sgnd pointer
	DrdDescriptor* drd = (DrdDescriptor*)pmink::CURRENT_DAEMON;
	// init config until connected
	while(!pmink::DaemonDescriptor::DAEMON_TERMINATED && drd->init_config(false) != 0){
	    sleep(5);
	}

}

// Next fragmented param fragment
void dr::SrvcMsgParamStreamNext::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    r14p::ServiceParam* sparam = (r14p::ServiceParam*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_PARAM);

    smsg->vpmap.set_octets(sparam->get_id(), sparam->get_data(), sparam->get_data_size(), sparam->get_index(), smsg->get_frag_param()->get_fragment_index());

}

// Fragmented param stream finished
void dr::SrvcMsgParamStreamDone::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    r14p::ServiceParam* sparam = (r14p::ServiceParam*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_PARAM);

    smsg->vpmap.set_octets(sparam->get_id(), sparam->get_data(), sparam->get_data_size(), sparam->get_index(), smsg->get_frag_param()->get_fragment_index());
    pmink_utils::VariantParam* vparam = smsg->vpmap.defragment(sparam->get_id(), sparam->get_index());
    vparam->set_type((pmink_utils::VariantParamType)sparam->get_extra_type());

}

// New Fragmented param stream
dr::SrvcMsgNewParamStream::SrvcMsgNewParamStream(): new_msg(NULL){

}
void dr::SrvcMsgNewParamStream::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    r14p::ServiceParam* sparam = (r14p::ServiceParam*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_PARAM);

    // set handlers
    sparam->set_callback(r14p::R14P_ET_SRVC_PARAM_STREAM_NEXT, &param_next);
    sparam->set_callback(r14p::R14P_ET_SRVC_PARAM_STREAM_END, &param_done);

    smsg->vpmap.set_octets(sparam->get_id(), sparam->get_data(), sparam->get_data_size(), sparam->get_index(), smsg->get_frag_param()->get_fragment_index());

}

// New param
void dr::SrvcMsgNewParam::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    r14p::ServiceParam* sparam = (r14p::ServiceParam*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_PARAM);

    // check param type
    switch(sparam->get_extra_type()){
	// c string
	case pmink_utils::DPT_STRING:
	{
	    char tmp_str[256];
	    sparam->extract(tmp_str);
	    smsg->vpmap.set_cstr(sparam->get_id(), tmp_str, sparam->get_index());
	    break;
	}

	// int
	case pmink_utils::DPT_INT:
	{
	    uint64_t tmp = 0;
	    sparam->extract(&tmp);
	    smsg->vpmap.set_int(sparam->get_id(), tmp, sparam->get_index());
	    break;
	}
	// bool
	case pmink_utils::DPT_BOOL:
	{
	    bool tmp = false;
	    sparam->extract(&tmp);
	    smsg->vpmap.set_bool(sparam->get_id(), tmp, sparam->get_index());
	    break;
	}

	// other
	default:
	{
	    unsigned char tmp_buff[256];
	    sparam->extract(&tmp_buff);
	    smsg->vpmap.set_octets(sparam->get_id(), tmp_buff, sparam->get_data_size(), sparam->get_index());
	    break;
	}
    }

}

// Service Message Error
void dr::SrvcMsgErr::run(r14p::R14PCallbackArgs* args){
    pmink_utils::TCAPTransaction* tcap_trans;
    std::vector<pmink_utils::tcap_tmap_it_t> expire_lst;
    db::DBRecord* dbr;
    pmink_utils::tcap_tmap_it_t tmp_it;


    // check timeout
    time_t now = time(NULL);


    // lock
    new_msg->tcap_corr->lock();

    // expire
    int exp_c = new_msg->tcap_corr->expire(&expire_lst, false);
    // stats
    new_msg->drd->stats.inc(DST_TCAP_TIMEOUT, exp_c);

    // timeout
    if(now - new_msg->timeout_ts.get() > new_msg->tcap_timeout){
	// get uncorrelated tcap messages
	new_msg->tcap_corr->process_timeout(new_msg->tcap_timeout, &expire_lst, true);
	// stats
	new_msg->drd->stats.inc(DST_TCAP_TIMEOUT, expire_lst.size() - exp_c);
	// update timestamp
	new_msg->timeout_ts.set(time(NULL));

    }


    // loop uncorrelated tcap messages
    for(unsigned int i = 0; i<expire_lst.size(); i++){
	// tcap transaction pointer
	tmp_it = expire_lst[i];
	tcap_trans = &tmp_it->second;
	// loop operations
	for(unsigned int j = 0; j<tcap_trans->operations.size(); j++){
	    // operation ref
	    pmink_utils::TCAPOperation& tcap_op = tcap_trans->operations[j];
	    // check if operation already processed
	    if(tcap_op.processed) continue;
	    // get dbr record
	    dbr = new_msg->dbm->pool_dbr.allocate_constructed();
	    // null check
	    if(dbr == NULL){
		new_msg->drd->stats.inc(DST_DBR_POOL_EMPTY, 1);
		continue;
	    }
	    // prepare db record
	    new_msg->msg_done.fill_dbr_sms(dbr, &tcap_op);
	    // push to db
	    new_msg->dbm->push(dbr);
	}
	// cleanup tcap transaction
	new_msg->tcap_corr->remove_transaction(tmp_it);
    }


    // unlock
    new_msg->tcap_corr->unlock();
}


void dr::SrvcMsgDone::fill_dbr_sms(db::DBRecord* dbr, pmink_utils::TCAPOperation* tcap_op){

    // reset params
    dbr->params.clear_params();
    // set dbr type
    dbr->type = db::DB_RT_COMBINED;

    // copy req params to dbr param map
    dbr->params = tcap_op->request_params;

    // no reply, set error type
    if(tcap_op->reply_params.size() == 0){
	// no reply error type
	dbr->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 11, tcap_op->req_component_index);
	return;
    }

    // tmp vparam
    pmink_utils::VariantParam* vparam;

    // check for SCCP UDTS error (original message returned)
    if(tcap_op->tcap_trans->status == pmink_utils::TCAP_ST_SCCP_ERROR && tcap_op->reply_params.size() > 0){
	// sccp err flag
	bool sccp_err = false;
	// check for UDTS/XUDTS message type
	if((vparam = tcap_op->reply_params.get_param(asn1::ParameterType::_pt_sccp_message_type)) != NULL){
	    switch((int)*vparam){
		// UDTS
		case 0x0a:
		// XUDTS
		case 0x12:
		    sccp_err = true;
		    break;
	    }

	    // check if sccp err found
	    if(sccp_err){
		// look for UDTS/XUDTS return cause
		if((vparam = tcap_op->reply_params.get_param(asn1::ParameterType::_pt_sccp_return_cause)) != NULL){
		    // set err
		    dbr->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 13, tcap_op->req_component_index);
		    dbr->params.set_int(asn1::ParameterType::_pt_tcap_error_code, (int)*vparam, tcap_op->req_component_index);
		    return;
		}

	    }


	}
    }

    // *** process reply ***

    // error type
    vparam = tcap_op->reply_params.get_param(asn1::ParameterType::_pt_tcap_error_type, tcap_op->rpl_component_index);
    if(vparam != NULL) dbr->params.set_int(asn1::ParameterType::_pt_tcap_error_type, (int)*vparam, tcap_op->req_component_index);

    // error code
    vparam = tcap_op->reply_params.get_param(asn1::ParameterType::_pt_tcap_error_code, tcap_op->rpl_component_index);
    if(vparam != NULL) dbr->params.set_int(asn1::ParameterType::_pt_tcap_error_code, (int)*vparam, tcap_op->req_component_index);

    // nnn
    vparam = tcap_op->reply_params.get_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, tcap_op->rpl_component_index);
    if(vparam != NULL) dbr->params.set_cstr(asn1::ParameterType::_pt_gsmmap_nnn_digits, (char*)*vparam, tcap_op->req_component_index);

    // an
    vparam = tcap_op->reply_params.get_param(asn1::ParameterType::_pt_gsmmap_an_digits, tcap_op->rpl_component_index);
    if(vparam != NULL) dbr->params.set_cstr(asn1::ParameterType::_pt_gsmmap_an_digits, (char*)*vparam, tcap_op->req_component_index);

    // imsi
    vparam = tcap_op->reply_params.get_param(asn1::ParameterType::_pt_gsmmap_imsi, tcap_op->rpl_component_index);
    if(vparam != NULL) dbr->params.set_cstr(asn1::ParameterType::_pt_gsmmap_imsi, (char*)*vparam, tcap_op->req_component_index);




}

void dr::SrvcMsgDone::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);

    // check if smsg exists
    if(smsg == NULL){
	// stats already updated in r14p service message manager
	return;
    }

    // discard incomplete service message
    if(smsg->missing_params || !smsg->is_complete()){
	new_msg->drd->stats.inc(DST_SMSG_INCOMPLETE, 1);

	/*
	if(!smsg->is_complete()){
	    std::cout << "===== MSG INCOMPLETE === " << std::endl;
	    std::cout << std::dec;
	    std::cout << smsg->vpmap;
	    std::cout << std::endl;
	}
	*/
	return;
    }

    // validate service id
    if(smsg->get_service_id() != asn1::ServiceId::_sid_sms_data_retention) {
	new_msg->drd->stats.inc(DST_UNSUPPORTED_SERVICE, 1);
	return;
    }

    // stats
    new_msg->drd->stats.inc(DST_RX_COUNT, 1);


/*
    std::cout << "===== MSG RECEIVED === " << std::endl;
    std::cout << std::dec;
    std::cout << smsg->vpmap;
    std::cout << std::endl;
*/
    pmink_utils::TCAPTransaction* tcap_trans = NULL;
    std::vector<pmink_utils::tcap_tmap_it_t> expire_lst;
    db::DBRecord* dbr = NULL;
    pmink_utils::tcap_tmap_it_t tmp_it;
    // *** check timeout ***
    time_t now = time(NULL);

    // lock
    new_msg->tcap_corr->lock();

    // expire
    int exp_c = new_msg->tcap_corr->expire(&expire_lst, false);
    // stats
    new_msg->drd->stats.inc(DST_TCAP_TIMEOUT, exp_c);

    // timeout
    if(now - new_msg->timeout_ts.get() > new_msg->tcap_timeout){
	// get uncorrelated tcap messages
	new_msg->tcap_corr->process_timeout(new_msg->tcap_timeout, &expire_lst, true);
	// stats
	new_msg->drd->stats.inc(DST_TCAP_TIMEOUT, expire_lst.size() - exp_c);
	// update timestamp
	new_msg->timeout_ts.set(time(NULL));

    }


    // loop uncorrelated tcap messages
    for(unsigned int i = 0; i<expire_lst.size(); i++){
	// tcap transaction pointer
	tmp_it = expire_lst[i];
	tcap_trans = &tmp_it->second;
	// loop operations
	for(unsigned int j = 0; j<tcap_trans->operations.size(); j++){
	    // operation ref
	    pmink_utils::TCAPOperation& tcap_op = tcap_trans->operations[j];
	    // check if operation already processed
	    if(tcap_op.processed) continue;
	    // get dbr record
	    dbr = new_msg->dbm->pool_dbr.allocate_constructed();
	    // null check
	    if(dbr == NULL){
		new_msg->drd->stats.inc(DST_DBR_POOL_EMPTY, 1);
		continue;
	    }
	    // prepare db record
	    fill_dbr_sms(dbr, &tcap_op);
	    // push to db
	    new_msg->dbm->push(dbr);
	}
	// cleanup tcap transaction
	new_msg->tcap_corr->remove_transaction(tmp_it);
    }
    // unlock
    new_msg->tcap_corr->unlock();


    // skip if context not supported (MAP part missing)
    pmink_utils::VariantParam* vparam = smsg->vpmap.get_param(asn1::ParameterType::_pt_tcap_dialogue_context_supported);
    if(vparam != NULL && !(bool)*vparam) return;

    // *** process tcap transaction ***
    // lock
    new_msg->tcap_corr->lock();


    // get tcap transaction
    tcap_trans = new_msg->tcap_corr->process(smsg, &smsg->vpmap);
    // result correlated (tmp_smsg contains invoke params)
    if(tcap_trans != NULL){
	// loop operations
	for(unsigned int i = 0; i<tcap_trans->operations.size(); i++){
	    // operation ref
	    pmink_utils::TCAPOperation& tcap_op = tcap_trans->operations[i];
	    // check if operation finished
	    if(!tcap_op.finished) continue;
	    // check if operation already processed
	    if(tcap_op.processed) continue;
	    // get dbr record
	    dbr = new_msg->dbm->pool_dbr.allocate_constructed();
	    // null check
	    if(dbr == NULL){
		new_msg->drd->stats.inc(DST_DBR_POOL_EMPTY, 1);
		continue;
	    }
	    // prepare db record
	    fill_dbr_sms(dbr, &tcap_op);
	    // push to db
	    new_msg->dbm->push(dbr);
	    // set as processed
	    tcap_op.processed = true;
	}


	// cleanup tcap transaction
	new_msg->tcap_corr->cleanup(tcap_trans);

    }
    // unlock
    new_msg->tcap_corr->unlock();

}

// New service message
dr::NewSrvcMsg::NewSrvcMsg(){
    msg_done.new_msg = this;
    msg_err.new_msg = this;
    // get db manager pointer
    dbm = (db::DBManager*)pmink::CURRENT_DAEMON->get_param(4);
    // get TCAP correlator pointer
    tcap_corr = (pmink_utils::TCAPCorrelator*)pmink::CURRENT_DAEMON->get_param(3);
    // last TCAP correlator timeout check timestamp
    timeout_ts.set(time(NULL));
    // tcap timeout default
    tcap_timeout = 5;
    // daemon pointer
    drd = (DrdDescriptor*)pmink::CURRENT_DAEMON;
}

dr::NewSrvcMsg::~NewSrvcMsg(){

}


void dr::NewSrvcMsg::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    // set handlers
    smsg->set_callback(r14p::R14P_ET_SRVC_SHORT_PARAM_NEW, &new_param);
    smsg->set_callback(r14p::R14P_ET_SRVC_PARAM_STREAM_NEW, &new_pstream);
    smsg->set_callback(r14p::R14P_ET_SRVC_MSG_COMPLETE, &msg_done);
    // clear variant params
    smsg->vpmap.clear_params();

}




