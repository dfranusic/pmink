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

#include <pdnd_events.h>
#include <r14p_utils.h>
#include <pdn.h>
#include <pdn_stats.h>


HbeatMissed::HbeatMissed(pmink::Atomic<uint8_t>* _activity_flag){
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
	PdndDescriptor* dd = (PdndDescriptor*)pmink::CURRENT_DAEMON;
	// init config until connected
	while(!pmink::DaemonDescriptor::DAEMON_TERMINATED && dd->init_config(false) != 0){
	    sleep(5);
	}

}


// Service message finished
void SrvcMsgDone::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    r14p::ServiceMsgManager* r14psmm = smsg->get_smsg_manager();
    PdndDescriptor* dd = (PdndDescriptor*)pmink::CURRENT_DAEMON;
    r14p::R14PStream* r14p_stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
    // todo

    // get r14ps
    r14p::R14PSession* r14ps = dd->r14ps;

    // check for missing params
    if(smsg->missing_params){
	// error
	//dd->stats.inc(fgn::ST_SMSG_PARAM_POOL_EMPTY, 1);
	return;
    }

    // check for incomplete msg
    if(!smsg->is_complete()){
	// error
	//dd->stats.inc(fgn::ST_SMSG_INCOMPLETE, 1);
	return;
    }

    // debug
    #ifdef PMDEBUG
    smsg->vpmap.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== RECEIVED via R14P =====" << std::endl;
	std::cout << smsg->vpmap;
	std::cout << "=============================" << std::endl;
    )

    // check service id
    switch(smsg->get_service_id()){
	case asn1::ServiceId::_sid_pdn_filtering:
	    // do nothing, ok
	    //dd->stats.inc(fgn::ST_SMSG_RX_COUNT, 1);
	    break;

	default:
	    // unsupported
	    //dd->stats.inc(fgn::ST_UNSUPPORTED_R14P_SERVICE, 1);
	    return;
    }


    // check for source type
    pmink_utils::VariantParam* vp_src_type = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_daemon_type);
    if(vp_src_type == NULL) return;

    // check for source type
    pmink_utils::VariantParam* vp_src_id = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_daemon_id);
    if(vp_src_type == NULL) return;

    // check service id
    pmink_utils::VariantParam* vp_srvc_id = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_service_id);

    // set service id
    smsg->set_service_id(asn1::ServiceId::_sid_fgn_filtering);
    // set client if needed
    if(dd->rt_r14pc == NULL || !dd->rt_r14pc->is_registered()) dd->rt_r14pc = r14ps->get_registered_client("routingd");

    // check for service id override
    if(vp_srvc_id != NULL) smsg->set_service_id((int)*vp_srvc_id);

    // **** TEMP, FOR TESTING ***
    smsg->vpmap.set_int(asn1::ParameterType::_pt_pmink_filter_result, 0);
    smsg->vpmap.set_int(asn1::ParameterType::_pt_pmink_service_id, asn1::ServiceId::_sid_pdn_filtering);
    smsg->vpmap.erase_param(asn1::ParameterType::_pt_pmink_daemon_id);
    smsg->vpmap.erase_param(asn1::ParameterType::_pt_pmink_daemon_type);
    smsg->vpmap.erase_param(asn1::ParameterType::_pt_pmink_correlation_notification);
    smsg->vpmap.erase_param(asn1::ParameterType::_pt_pmink_filter_list_id, 2);
    smsg->vpmap.erase_param(asn1::ParameterType::_pt_pmink_filter_data, 2);

    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(smsg) == 0){
	// send
	if(r14psmm->send(smsg, dd->rt_r14pc, (char*)*vp_src_type, (char*)*vp_src_id, true, &async_done) != 0){
	    // error
	    //dd->stats.inc(stp::SST_SMSG_SEND_ERROR, 1);
	    return;
	}
	// - do not free current smsg (will be freed when sent in async_done handler)
	// - pass flag skips checking for auto_free flag in ServiceStreamHandlerDone,
	//   mandatory in this case
	r14p_stream->set_param(r14p::SMSG_PT_PASS, smsg);

    }else{
	// error
	//dd->stats.inc(stp::SST_SMSG_PARAM_POOL_EMPTY, 1);

    }

}

// SrvcMsgErr
void SrvcMsgErr::run(r14p::R14PCallbackArgs* args){


}


void NewSrvcMsg::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    // set handlers
    smsg->set_callback(r14p::R14P_ET_SRVC_MSG_COMPLETE, &msg_done);
}

void ServiceMessageAsyncDone::run(r14p::R14PCallbackArgs* args){
    // get service message
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    // return service message to pool
    smsg->get_smsg_manager()->free_smsg(smsg);


}

