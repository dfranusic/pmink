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

#include <stpd_events.h>
#include <r14p_utils.h>
#include <stp.h>
#include <stp_stats.h>

HbeatMissed::HbeatMissed(pmink::Atomic<bool>* _activity_flag){
    activity_flag = _activity_flag;

}

void HbeatMissed::run(r14p::R14PCallbackArgs* args){
	r14p::HeartbeatInfo* hi = (r14p::HeartbeatInfo*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_HBEAT_INFO);
	// set activity flag to false
	activity_flag->comp_swap(true, false);
	// stop heartbeat
	r14p::stop_heartbeat(hi);
	// display warning
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "R14P HBEAT not received, closing connection to [%s]...",
				   hi->target_daemon_id);
}

void HbeatRecv::run(r14p::R14PCallbackArgs* args){
    // do nothing
}

HbeatCleanup::HbeatCleanup(HbeatRecv* _recv, HbeatMissed* _missed){
	recv = _recv;
	missed = _missed;

}

void HbeatCleanup::run(r14p::R14PCallbackArgs* args){
	delete recv;
	delete missed;
	delete this;

	// get daemon pointer
	StpdDescriptor* dd = (StpdDescriptor*)pmink::CURRENT_DAEMON;
	// init config until connected
	while(!pmink::DaemonDescriptor::DAEMON_TERMINATED && dd->init_config(false) != 0){
	    sleep(5);
	}

}


int SrvcMsgDone::get_next_route(r14p::ServiceMessage* smsg, char* next_route, int* service_id, uint32_t buff_size){
    // dest index
    uint32_t dest_index = 0;
    // check last index
    pmink_utils::VariantParam* vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_routing_index);
    if(vp != NULL) dest_index = (int)*vp + 1;

    // get next destination
    vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_routing_destination, dest_index);
    if(vp != NULL){
	// get route name
	strncpy(next_route, (char*)*vp, buff_size);
	// set current index
	smsg->vpmap.set_int(asn1::ParameterType::_pt_pmink_routing_index, dest_index);
	// get r14p destination service id
	vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_routing_service_id, dest_index);
	if(vp != NULL) *service_id = (int)*vp;

    }else return 1;

    return 0;
}

void ServiceMessageAsyncDone::run(r14p::R14PCallbackArgs* args){
    // get service message
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    // r14psmm pointer
    r14p::ServiceMsgManager* r14psmm = smsg->get_smsg_manager();

    // check for routing error
    if(smsg->get_sdone_hndlr()->status != asn1::ErrorCode::_err_ok){
	// route str
	char dest_str[16] = {0};
	int dst_service_id = -1;
	// get daemon pointer
	StpdDescriptor* dd = (StpdDescriptor*)pmink::CURRENT_DAEMON;

	// try another route
	if(msg_done->get_next_route(smsg, dest_str, &dst_service_id, sizeof(dest_str)) != 0){
	    dd->stats.inc(stp::SST_NO_VALID_ROUTES, 1);
	    // free
	    r14psmm->free_smsg(smsg);
	    return;
	}

	// sync vpmap
	if(r14psmm->vpmap_sparam_sync(smsg) == 0){
	    // set r14p service id if defined per r14p destination
	    if(dst_service_id != -1) smsg->set_service_id(dst_service_id);
	    // send
	    if(r14psmm->send(smsg, dd->dest_r14pc, dest_str, NULL, true, this) != 0){
		// error
		dd->stats.inc(stp::SST_SMSG_SEND_ERROR, 1);
		// free
		r14psmm->free_smsg(smsg);
		return;
	    }

	    return;

	}else{
	    // error
	    dd->stats.inc(stp::SST_SMSG_PARAM_POOL_EMPTY, 1);

	}
    }


    // free
    r14psmm->free_smsg(smsg);

}

// Service message finished
void SrvcMsgDone::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    r14p::ServiceMsgManager* r14psmm = smsg->get_smsg_manager();
    r14p::R14PStream* r14p_stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
    r14p::R14PClient* r14pc = r14p_stream->get_client();

    // default destination
    char dest_str[17];
    int dst_service_id = -1;
    // init string
    dest_str[0] = 0;
    // get daemon pointer
    StpdDescriptor* dd = (StpdDescriptor*)pmink::CURRENT_DAEMON;
    // get r14ps
    r14p::R14PSession* r14ps = dd->r14ps;

    // check for missing params
    if(smsg->missing_params){
	// error
	dd->stats.inc(stp::SST_SMSG_PARAM_POOL_EMPTY, 1);
	return;
    }

    // check for incomplete msg
    if(!smsg->is_complete()){
	// error
	dd->stats.inc(stp::SST_SMSG_INCOMPLETE, 1);
	return;
    }


    // check service id
    switch(smsg->get_service_id()){
	case asn1::ServiceId::_sid_stp_routing:
	    // do nothing, ok
	    dd->stats.inc(stp::SST_SMSG_RX_COUNT, 1);
	    break;

	default:
	    // unsupported
	    dd->stats.inc(stp::SST_UNSUPPORTED_R14P_SERVICE, 1);
	    return;
    }


    // get rule processor
    stp::rulepc_it_t it = dd->rule_pc_map.find(r14pc);
    if(it == dd->rule_pc_map.end()){
	// should not happen
	return;
    }

    // sync rules and lists with master
    it->second.sync();

    // get rule manager ref
    stp::rule_m_t& rule_m_ref = *it->second.rule_m;

    // run rules
    dd->stp_routing.run_rules(&smsg->vpmap, &rule_m_ref);

    // debug
    #ifdef PMDEBUG
    smsg->vpmap.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== PROCESSED =====" << std::endl;
	std::cout << smsg->vpmap;
	std::cout << "=====================" << std::endl;
    )
    // set service id
    smsg->set_service_id(asn1::ServiceId::_sid_sgn_forward);
    // set client if needed
    if(dd->dest_r14pc == NULL || !dd->dest_r14pc->is_registered()) dd->dest_r14pc = r14ps->get_registered_client("routingd");

    // *** check current destination type ***
    if(get_next_route(smsg, dest_str, &dst_service_id, sizeof(dest_str)) != 0){
	dd->stats.inc(stp::SST_NO_VALID_ROUTES, 1);
	return;
    }

    // set r14p service id if changed
    pmink_utils::VariantParam* vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_service_id, 2);
    if(vp != NULL){
	smsg->set_service_id((int)*vp);
	smsg->vpmap.erase_param(asn1::ParameterType::_pt_pmink_service_id, 2);
    }

    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(smsg) == 0){
	// set r14p service id if defined per r14p destination
	if(dst_service_id != -1) smsg->set_service_id(dst_service_id);
	// send
	if(r14psmm->send(smsg, dd->dest_r14pc, dest_str, NULL, true, &async_done) != 0){
	    // error
	    dd->stats.inc(stp::SST_SMSG_SEND_ERROR, 1);
	    return;
	}
	// - do not free current smsg (will be freed when sent in async_done handler)
	// - pass flag skips checking for auto_free flag in ServiceStreamHandlerDone,
	//   mandatory in this case
	r14p_stream->set_param(r14p::SMSG_PT_PASS, smsg);

    }else{
	// error
	dd->stats.inc(stp::SST_SMSG_PARAM_POOL_EMPTY, 1);

    }

}
// SrvcMsgErr
void SrvcMsgErr::run(r14p::R14PCallbackArgs* args){
    // get daemon pointer
    StpdDescriptor* stpd = (StpdDescriptor*)pmink::CURRENT_DAEMON;
    // stats
    stpd->stats.inc(stp::SST_SMSG_MSG_POOL_EMPTY, 1);

}


// New service message
NewSrvcMsg::NewSrvcMsg(){
    msg_done.new_msg = this;
    msg_done.async_done.msg_done = &msg_done;

}

NewSrvcMsg::~NewSrvcMsg(){

}


void NewSrvcMsg::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    // set handlers
    smsg->set_callback(r14p::R14P_ET_SRVC_MSG_COMPLETE, &msg_done);
}




