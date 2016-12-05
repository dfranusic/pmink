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

#include <fgnd_events.h>
#include <r14p_utils.h>
#include <fgn.h>
#include <fgn_stats.h>
#include <connection.h>


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
	FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
	// init config until connected
	while(!pmink::DaemonDescriptor::DAEMON_TERMINATED && dd->init_config(false) != 0){
	    sleep(5);
	}

}

// Service message finished
void SrvcMsgDone::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    fgn::FilterManager* fm = &dd->filter_m;
    fgn::FWorker* fw = NULL;

    // check for missing params
    if(smsg->missing_params){
	// error
	dd->stats.inc(fgn::ST_SMSG_PARAM_POOL_EMPTY, 1);
	return;
    }

    // check for incomplete msg
    if(!smsg->is_complete()){
	// error
	dd->stats.inc(fgn::ST_SMSG_INCOMPLETE, 1);
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
	case asn1::ServiceId::_sid_fgn_filtering:
	    // do nothing, ok
	    dd->stats.inc(fgn::ST_SMSG_RX_COUNT, 1);
	    break;

	default:
	    // unsupported
	    dd->stats.inc(fgn::ST_UNSUPPORTED_R14P_SERVICE, 1);
	    return;
    }

    // check for source service id
    pmink_utils::VariantParam* vp_src_srvc_id = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_service_id);
    if(vp_src_srvc_id != NULL){
	switch((int)*vp_src_srvc_id){
	    // pdn data source
	    case asn1::ServiceId::_sid_pdn_filtering:
	    {
		// get fw
		fw = fm->get_fw();
		if(fw == NULL){
		    dd->stats.inc(fgn::ST_FWORKER_NA, 1);
		    break;
		}

		// get new object from pool
		fgn::FgnPayload* pld = dd->cpool.allocate_constructed();
		if(pld == NULL) {
		    dd->stats.inc(fgn::ST_FGN_PAYLOAD_POOL_EMPTY, 1);
		    break;
		}


		// reset
		pld->reset();
		// copy params
		pld->params = smsg->vpmap;
		// set phase
		pld->phase = fgn::CPT_PD_RPL;
		// send to worker queue
		fw->queue.push(pld);
		return;
	    }

	    // default data source
	    case asn1::ServiceId::_sid_sgn_forward:
		break;

	    // unknown data source
	    default:
		return;
	}

    }

    // check for special commands
    pmink_utils::VariantParam* vp_cmd = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_command_id);
    if(vp_cmd != NULL){
	switch((int)*vp_cmd){
	    // notification
	    case asn1::SmsHubCommandId::_shci_corr_ntf:
	    {

		// get fw
		fw = fm->get_fw();
		if(fw == NULL){
		    dd->stats.inc(fgn::ST_FWORKER_NA, 1);
		    break;
		}

		// get new object from pool
		fgn::FgnPayload* pld = dd->cpool.allocate_constructed();
		if(pld == NULL) {
		    dd->stats.inc(fgn::ST_FGN_PAYLOAD_POOL_EMPTY, 1);
		    break;
		}


		// reset
		pld->reset();
		// copy params
		pld->params = smsg->vpmap;
		// set phase
		pld->phase = fgn::CPT_NOTIFICATION_RCVD;
		// send to worker queue
		fw->queue.push(pld);
		break;
	    }
	}
	return;
    }


    // check conn type
    pmink_utils::VariantParam* vp_conn_type = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_connection_type);
    if(vp_conn_type == NULL) return;


    // *** smpp ***
    if((int)*vp_conn_type == sgn::CT_SMPP){
	// get pdu type
	pmink_utils::VariantParam* vp_smpp_pdu = smsg->vpmap.get_param(asn1::ParameterType::_pt_smpp_command_id);
	if(vp_smpp_pdu == NULL) return;

	// check pdu type
	switch((uint32_t)*vp_smpp_pdu){
	    // deliver_sm and submit_sm
	    case smpp::SUBMIT_SM:
	    case smpp::DELIVER_SM:
	    {
		// get fw
		fw = fm->get_fw();
		if(fw == NULL){
		    dd->stats.inc(fgn::ST_FWORKER_NA, 1);
		    break;
		}

		// get new object from pool
		fgn::FgnPayload* pld = dd->cpool.allocate_constructed();
		if(pld == NULL) {
		    dd->stats.inc(fgn::ST_FGN_PAYLOAD_POOL_EMPTY, 1);
		    break;
		}

		// get rule processor
		fgn::RuleProcessor* rproc = dd->rproc_pool.allocate_constructed();
		if(rproc == NULL){
		    // free pld
		    dd->cpool.deallocate_constructed(pld);
		    dd->stats.inc(fgn::ST_RPROC_POOL_EMPTY, 1);
		    break;
		}

		// reset
		pld->reset();
		// copy params
		pld->params = smsg->vpmap;
		// set rule proc
		pld->rproc = rproc;
		// set phase
		pld->phase = fgn::CPT_SMPP_SMS_FWD;
		// generate guid
		dd->generate_uuid((unsigned char*)&pld->guid);
		// send to worker queue
		fw->queue.push(pld);
		break;
	    }
	}

	return;
    }

    // *** ss7 m3ua ***
    if((int)*vp_conn_type != sgn::CT_M3UA) return;


    // check context
    pmink_utils::VariantParam* vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
    if(vp != NULL){
	// supported oids:
	// ===============
	// 0.4.0.0.1.0.25.3 - shortMsgMT-RelayContext-v3
	// 0.4.0.0.1.0.25.2 - shortMsgMT-RelayContext-v2

	// 0.4.0.0.1.0.20.3 - shortMsgGatewayContext-v3
	// 0.4.0.0.1.0.20.2 - shortMsgGatewayContext-v2
	// 0.4.0.0.1.0.20.1 - shortMsgGatewayContext-v1

	// 0.4.0.0.1.0.21.3 - shortMsgMO-RelayContext-v3
	// 0.4.0.0.1.0.21.2 - shortMsgMO-RelayContext-v2
	// 0.4.0.0.1.0.21.1 - shortMsgRelayContext-v1

	// str pointer
	char* ctx_oid = (char*)*vp;

	// check
	if(!(strcmp(ctx_oid, "0.4.0.0.1.0.25.3") == 0 ||
	     strcmp(ctx_oid, "0.4.0.0.1.0.25.2") == 0 ||
	     strcmp(ctx_oid, "0.4.0.0.1.0.20.3") == 0 ||
	     strcmp(ctx_oid, "0.4.0.0.1.0.20.2") == 0 ||
	     strcmp(ctx_oid, "0.4.0.0.1.0.20.1") == 0 ||
	     strcmp(ctx_oid, "0.4.0.0.1.0.21.3") == 0 ||
	     strcmp(ctx_oid, "0.4.0.0.1.0.21.2") == 0 ||
	     strcmp(ctx_oid, "0.4.0.0.1.0.21.1") == 0)) {
	    // not supported (back to stp)
	    dd->send_stp(&smsg->vpmap);
	    return;
	}

    }

    // get tcap component count
    vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_tcap_component_count);
    if(vp == NULL || (int)*vp == 0){
	// send to stp
	dd->send_stp(&smsg->vpmap);
	// check for tcap message type
	vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_tcap_message_type);
	if(vp == NULL) return;
	// only intersted in end/abort
	switch((int)*vp){
	    // End/Abort
	    case 4:
	    case 7:
	    {
		// get fw
		fw = fm->get_fw();
		if(fw == NULL){
		    dd->stats.inc(fgn::ST_FWORKER_NA, 1);
		    break;
		}

		// get new object from pool
		fgn::FgnPayload* pld = dd->cpool.allocate_constructed();
		if(pld == NULL) {
		    dd->stats.inc(fgn::ST_FGN_PAYLOAD_POOL_EMPTY, 1);
		    break;
		}

		// get rule processor
		fgn::RuleProcessor* rproc = dd->rproc_pool.allocate_constructed();
		if(rproc == NULL){
		    // free pld
		    dd->cpool.deallocate_constructed(pld);
		    dd->stats.inc(fgn::ST_RPROC_POOL_EMPTY, 1);
		    break;
		}

		// reset
		pld->reset();
		// copy params
		pld->params = smsg->vpmap;
		// set rule proc
		pld->rproc = rproc;
		// set operation index
		pld->tcap_op_index = 0;
		// set phase
		pld->phase = fgn::CPT_UNKNOWN_RESULT;
		// send to worker queue
		fw->queue.push(pld);
		break;
	    }

	    // default, no action
	    default: break;

	}


	return;
    }
    unsigned int cc = (int)*vp;

    // if cc > 1 send back to stp
    if(cc > 1) {
	dd->stats.inc(fgn::ST_TCAP_MULTIPLE_COMPONENTS, 1);
	dd->send_stp(&smsg->vpmap);
	return;
    }

    // loop components (just one)
    for(unsigned int i = 0; i<cc; i++){

	// check opcode
	vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_tcap_opcode, i);
	// no opcode found
	if(vp == NULL) {
	    // send to stop
	    dd->send_stp(&smsg->vpmap);
	    // check component type
	    vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_tcap_component_type, i);
	    // missing components
	    if(vp == NULL) continue;
	    // interested in results/errors, all except Invoke (1)
	    if((int)*vp == 1) continue;

	    // get fw
	    fw = fm->get_fw();
	    if(fw == NULL){
		dd->stats.inc(fgn::ST_FWORKER_NA, 1);
		break;
	    }

	    // get new object from pool
	    fgn::FgnPayload* pld = dd->cpool.allocate_constructed();
	    if(pld == NULL) {
		dd->stats.inc(fgn::ST_FGN_PAYLOAD_POOL_EMPTY, 1);
		break;
	    }

	    // get rule processor
	    fgn::RuleProcessor* rproc = dd->rproc_pool.allocate_constructed();
	    if(rproc == NULL){
		// free pld
		dd->cpool.deallocate_constructed(pld);
		dd->stats.inc(fgn::ST_RPROC_POOL_EMPTY, 1);
		break;
	    }

	    // reset
	    pld->reset();
	    // copy params
	    pld->params = smsg->vpmap;
	    // set rule proc
	    pld->rproc = rproc;
	    // set operation index
	    pld->tcap_op_index = i;
	    // set phase
	    pld->phase = fgn::CPT_UNKNOWN_RESULT;
	    // send to worker queue
	    fw->queue.push(pld);
	    continue;
	}

	switch((int)*vp){
	    // sri-sm
	    case asn1::gsmmap::GSMMAPOperationLocalvalue::_sendRoutingInfoForSM:
	    {
		// check component type
		vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_tcap_component_type, i);
		if(vp == NULL) {
		    dd->send_stp(&smsg->vpmap);
		    break;
		}

		// check if component is Invoke (1)
		if((int)*vp == 1) {
		    // route to stp
		    dd->send_stp(&smsg->vpmap);
		    // get fw
		    fw = fm->get_fw();
		    if(fw == NULL){
			dd->stats.inc(fgn::ST_FWORKER_NA, 1);
			break;
		    }

		    // get new object from pool
		    fgn::FgnPayload* pld = dd->cpool.allocate_constructed();
		    if(pld == NULL) {
			dd->stats.inc(fgn::ST_FGN_PAYLOAD_POOL_EMPTY, 1);
			break;
		    }

		    // get rule processor
		    fgn::RuleProcessor* rproc = dd->rproc_pool.allocate_constructed();
		    if(rproc == NULL){
			// free pld
			dd->cpool.deallocate_constructed(pld);
			dd->stats.inc(fgn::ST_RPROC_POOL_EMPTY, 1);
			break;
		    }

		    // reset
		    pld->reset();
		    // copy params
		    pld->params = smsg->vpmap;
		    // set rule proc
		    pld->rproc = rproc;
		    // set operation index
		    pld->tcap_op_index = i;
		    // set phase
		    pld->phase = fgn::CPT_SRI_SM_REQ;
		    // send to worker queue
		    fw->queue.push(pld);



		// return result or error
		}else {
		    // get fw
		    fw = fm->get_fw();
		    if(fw == NULL){
			dd->stats.inc(fgn::ST_FWORKER_NA, 1);
			break;
		    }

		    // get new object from pool
		    fgn::FgnPayload* pld = dd->cpool.allocate_constructed();
		    if(pld == NULL) {
			dd->stats.inc(fgn::ST_FGN_PAYLOAD_POOL_EMPTY, 1);
			break;
		    }

		    // get rule processor
		    fgn::RuleProcessor* rproc = dd->rproc_pool.allocate_constructed();
		    if(rproc == NULL){
			// free pld
			dd->cpool.deallocate_constructed(pld);
			dd->stats.inc(fgn::ST_RPROC_POOL_EMPTY, 1);
			break;
		    }

		    // reset
		    pld->reset();
		    // copy params
		    pld->params = smsg->vpmap;
		    // set rule proc
		    pld->rproc = rproc;
		    // set operation index
		    pld->tcap_op_index = i;
		    // set phase
		    pld->phase = fgn::CPT_SRI_SM_RPL;
		    // send to worker queue
		    fw->queue.push(pld);

		}
		break;
	    }

	    // ss7 mt/mo
	    case asn1::gsmmap::GSMMAPOperationLocalvalue::_mt_forwardSM:
	    case asn1::gsmmap::GSMMAPOperationLocalvalue::_mo_forwardSM:
	    {
		// check component type
		vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_tcap_component_type, i);
		if(vp == NULL){
		    dd->send_stp(&smsg->vpmap);
		    break;
		}

		// check if component is Invoke
		if((int)*vp == 1){
		    // get fw
		    fw = fm->get_fw();
		    if(fw == NULL){
			dd->stats.inc(fgn::ST_FWORKER_NA, 1);
			break;
		    }

		    // get new object from pool
		    fgn::FgnPayload* pld = dd->cpool.allocate_constructed();
		    if(pld == NULL) {
			dd->stats.inc(fgn::ST_FGN_PAYLOAD_POOL_EMPTY, 1);
			break;
		    }

		    // get rule processor
		    fgn::RuleProcessor* rproc = dd->rproc_pool.allocate_constructed();
		    if(rproc == NULL){
			// free pld
			dd->cpool.deallocate_constructed(pld);
			dd->stats.inc(fgn::ST_RPROC_POOL_EMPTY, 1);
			break;
		    }

		    // reset
		    pld->reset();
		    // copy params
		    pld->params = smsg->vpmap;
		    // set rule proc
		    pld->rproc = rproc;
		    // set operation index
		    pld->tcap_op_index = i;
		    // set phase
		    pld->phase = fgn::CPT_SS7_SMS_FWD;
		    // generate guid
		    dd->generate_uuid((unsigned char*)&pld->guid);
		    // send to worker queue
		    fw->queue.push(pld);

		// return result or error
		}else{
		    // route to stp
		    dd->send_stp(&smsg->vpmap);
		    // get fw
		    fw = fm->get_fw();
		    if(fw == NULL){
			dd->stats.inc(fgn::ST_FWORKER_NA, 1);
			break;
		    }

		    // get new object from pool
		    fgn::FgnPayload* pld = dd->cpool.allocate_constructed();
		    if(pld == NULL) {
			dd->stats.inc(fgn::ST_FGN_PAYLOAD_POOL_EMPTY, 1);
			break;
		    }

		    // get rule processor
		    fgn::RuleProcessor* rproc = dd->rproc_pool.allocate_constructed();
		    if(rproc == NULL){
			// free pld
			dd->cpool.deallocate_constructed(pld);
			dd->stats.inc(fgn::ST_RPROC_POOL_EMPTY, 1);
			break;
		    }

		    // reset
		    pld->reset();
		    // copy params
		    pld->params = smsg->vpmap;
		    // set rule proc
		    pld->rproc = rproc;
		    // set operation index
		    pld->tcap_op_index = i;
		    // set phase
		    pld->phase = fgn::CPT_SS7_SMS_RPL;
		    // generate guid
		    dd->generate_uuid((unsigned char*)&pld->guid);
		    // send to worker queue
		    fw->queue.push(pld);

		}
		break;
	    }

	    // unknown/unsupported
	    default:
		dd->send_stp(&smsg->vpmap);
		break;
	}
    }



}

// SrvcMsgErr
void SrvcMsgErr::run(r14p::R14PCallbackArgs* args){
    // get daemon pointer
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    // stats
    dd->stats.inc(fgn::ST_SMSG_MSG_POOL_EMPTY, 1);


}


void NewSrvcMsg::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    // set handlers
    smsg->set_callback(r14p::R14P_ET_SRVC_MSG_COMPLETE, &msg_done);
}

