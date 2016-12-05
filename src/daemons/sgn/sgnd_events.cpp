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

#include <sgnd_events.h>
#include <sgn_stats.h>
#include <sgn.h>



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

	// get sgnd pointer
	SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
	// init config until connected
	while(!pmink::DaemonDescriptor::DAEMON_TERMINATED && sgnd->init_config(false) != 0){
	    sleep(5);
	}

}

int SrvcMsgDone::process_next(r14p::ServiceMessage* smsg, r14p::R14PStream* r14p_stream, char* next_route, uint32_t buff_size){
    // get daemon pointer
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // get r14ps
    r14p::R14PSession* r14ps = sgnd->r14ps;
    r14p::ServiceMsgManager* r14psmm = smsg->get_smsg_manager();

    if(get_next_route(smsg, next_route, buff_size, NULL, 0, NULL, false) != 0){
	// error
	sgnd->stats.inc(sgn::SST_GET_NEXT_ROUTE_ERROR, 1);
	return 1;

    }

    // check if route found
    if(strlen(next_route) == 0){
	sgnd->stats.inc(sgn::SST_NO_VALID_ROUTES, 1);
	return 2;
    }

    // set client if needed
    if(sgnd->rt_r14pc == NULL || !sgnd->rt_r14pc->is_registered()) sgnd->rt_r14pc = r14ps->get_registered_client("routingd");
    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(smsg) == 0){
	// send
	if(r14psmm->send(smsg, sgnd->rt_r14pc, next_route, NULL, true, &async_done) != 0){
	    // error
	    sgnd->stats.inc(sgn::SST_SMSG_SEND_ERROR, 1);
	    return 2;
	}
	// - do not free current smsg (will be freed when sent in async_done handler)
	// - pass flag skips checking for auto_free flag in ServiceStreamHandlerDone,
	//   mandatory in this case
	r14p_stream->set_param(r14p::SMSG_PT_PASS, smsg);

    }else{
	// error
	sgnd->stats.inc(sgn::SST_SMSG_PARAM_POOL_EMPTY, 1);
    }

    // ok
    return 0;

}

int SrvcMsgDone::get_next_route(r14p::ServiceMessage* smsg,
				char* next_route,
				uint32_t buff_size,
			        char* next_sub_route,
			        uint32_t sub_buff_size,
				int* next_sub_type,
				bool inc_as_index){
    // clear next route str
    *next_route = 0;
    // dest index
    uint32_t dest_index = 0;
    // check last index
    pmink_utils::VariantParam* vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_routing_index);
    if(vp != NULL) {
	if(inc_as_index) dest_index = (int)*vp + 1;
	else dest_index = (int)*vp;
    }

    // set next route
    vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_routing_destination, dest_index);
    if(vp != NULL) strncpy(next_route, (char*)*vp, buff_size);

    // next not needed in case of inc_as_index == false
    if(!inc_as_index) return 0;

    // sub type index
    uint32_t sub_index = (dest_index > 0 ? dest_index - 1 : 0);

    // reset next_type
    *next_sub_type = 0;
    // reset next sub route
    *next_sub_route = 0;

    // get next sub destination type
    vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_routing_sub_destination_type, sub_index);
    if(vp != NULL){
	// set next sub type
	*next_sub_type = (int)*vp;
	// check for AS type
	if((int)*vp == 1){
	    // get route name
	    vp = smsg->vpmap.get_param(asn1::ParameterType::_pt_pmink_routing_sub_destination, sub_index);
	    if(vp != NULL) strncpy(next_sub_route, (char*)*vp, sub_buff_size);
	}
    }

    // set current index
    if(inc_as_index) smsg->vpmap.set_int(asn1::ParameterType::_pt_pmink_routing_index, dest_index);


    // ok
    return 0;
}

// asunc done
void ServiceMessageAsyncDoneExt::run(r14p::R14PCallbackArgs* args){
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    r14p::ServiceMsgManager* r14psmm = smsg->get_smsg_manager();

    // check for routing error
    if(smsg->get_sdone_hndlr()->status != asn1::ErrorCode::_err_ok){
	// route str
	char dest_str[17];
	char dest_sub_str[17];
	// init strings
	dest_str[0] = 0;
	dest_sub_str[0] = 0;
	// next dest type
	int next_sub_type = -1;
	// get daemon pointer
	SgndDescriptor* dd = (SgndDescriptor*)pmink::CURRENT_DAEMON;


	// get next route
	if(msg_done->get_next_route(smsg, dest_str, sizeof(dest_str), dest_sub_str, sizeof(dest_sub_str), &next_sub_type, true) != 0){
	    // error
	    dd->stats.inc(sgn::SST_GET_NEXT_ROUTE_ERROR, 1);
	    // free
	    r14psmm->free_smsg(smsg);
	    return;

	}

	// check if route found
	if(strlen(dest_str) == 0){
	    dd->stats.inc(sgn::SST_NO_VALID_ROUTES, 1);
	    // free
	    r14psmm->free_smsg(smsg);
	    return;
	}
	// sync vpmap
	if(r14psmm->vpmap_sparam_sync(smsg) == 0){
	    // send
	    if(r14psmm->send(smsg, dd->rt_r14pc, dest_str, NULL, true, this) != 0){
		// error
		dd->stats.inc(sgn::SST_SMSG_SEND_ERROR, 1);
		// free
		r14psmm->free_smsg(smsg);
		return;
	    }

	    return;

	}else{
	    // error
	    dd->stats.inc(sgn::SST_SMSG_PARAM_POOL_EMPTY, 1);

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
    // default destination
    char dest_str[17];
    char dest_sub_str[17];
    // init strings
    dest_str[0] = 0;
    dest_sub_str[0] = 0;
    // next dest type
    int next_sub_type = -1;
    // get daemon pointer
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // get r14ps
    r14p::R14PSession* r14ps = sgnd->r14ps;

    // check for missing params
    if(smsg->missing_params){
	// error
	sgnd->stats.inc(sgn::SST_SMSG_PARAM_POOL_EMPTY, 1);
	return;
    }

    // check for incomplete msg
    if(!smsg->is_complete()){
	// error
	sgnd->stats.inc(sgn::SST_SMSG_INCOMPLETE, 1);
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
	// route message (AS or R14P)
	case asn1::ServiceId::_sid_sgn_forward:
	    // get next route
	    get_next_route(smsg, dest_str, sizeof(dest_str), dest_sub_str, sizeof(dest_sub_str), &next_sub_type, true);

	    // if next sub route is not AS, send to router
	    // sgnd only accepts AS sub destinations
	    if(next_sub_type != 1){
		// check if next route found
		if(strlen(dest_str) == 0){
		    sgnd->stats.inc(sgn::SST_NO_VALID_ROUTES, 1);
		    return;
		}

		// set client if needed
		if(sgnd->rt_r14pc == NULL || !sgnd->rt_r14pc->is_registered()) sgnd->rt_r14pc = r14ps->get_registered_client("routingd");
		// sync vpmap
		if(r14psmm->vpmap_sparam_sync(smsg) == 0){
		    // send
		    if(r14psmm->send(smsg, sgnd->rt_r14pc, dest_str, NULL, true, &async_done) != 0){
			// error
			sgnd->stats.inc(sgn::SST_SMSG_SEND_ERROR, 1);
			return;

		    }
		    // - do not free current smsg (will be freed when sent in async_done handler)
		    // - pass flag skips checking for auto_free flag in ServiceStreamHandlerDone,
		    //   mandatory in this case
		    r14p_stream->set_param(r14p::SMSG_PT_PASS, smsg);


		}else{
		    // error
		    sgnd->stats.inc(sgn::SST_SMSG_PARAM_POOL_EMPTY, 1);
		}
		return;

	    }

	    // do nothing, next route is AS, process
	    break;


	default:
	    // unsupported service id
	    sgnd->stats.inc(sgn::SST_UNSUPPORTED_R14P_SERVICE, 1);
	    return;
    }

    // **** route to AS ******

    // lock
    sgnd->as_lock.lock();
    // find m3ua as
    sgn::ApplicationServer* as = sgnd->get_as(dest_sub_str);
    // as not found, try next destination
    if(as == NULL){
	sgnd->as_lock.unlock();
	// ** smpp ***
	// lock
	sgnd->smpp_as_lock.lock();
	// find as
	sgn::SMPP_ApplicationServer* smpp_as = sgnd->get_smpp_as(dest_sub_str);
	// as not found, try next destination
	if(smpp_as == NULL){
	    sgnd->stats.inc(sgn::SST_AS_NOT_FOUND, 1);
	    // process next route
	    process_next(smsg, r14p_stream, dest_str, sizeof(dest_str));
	    // unlock
	    sgnd->smpp_as_lock.unlock();
	    return;
	}
	// ** smpp **
	smpp_as->wrr_mtx.lock();
	sgn::SMPPConnection* smpp_conn = smpp_as->get_next_asp();
	smpp_as->wrr_mtx.unlock();

	// no ASP found, try next destination
	if(smpp_conn == NULL){
	    smpp_as->stats.inc(sgn::ASST_ASP_NOT_FOUND, 1);
	    // process next route
	    process_next(smsg, r14p_stream, dest_str, sizeof(dest_str));
	    // unlock
	    sgnd->smpp_as_lock.unlock();
	    return;
	}

	// get client conn in case of server conn
	pmink_utils::VariantParam* vp_smpp_conn_id = smsg->vpmap.get_param(asn1::ParameterType::_pt_smpp_connection_id);
	smpp_conn = (sgn::SMPPConnection*)smpp_conn->get_conn((vp_smpp_conn_id != NULL ? (char*)*vp_smpp_conn_id : NULL), true);
	// no active connection in ASP (in case of server connection)
	if(smpp_conn == NULL) {
	    smpp_as->stats.inc(sgn::ASST_ASP_NOT_FOUND, 1);
	    // process next route
	    process_next(smsg, r14p_stream, dest_str, sizeof(dest_str));
	    // unlock
	    sgnd->smpp_as_lock.unlock();
	    return;
	}
	// unlock
	sgnd->smpp_as_lock.unlock();

	// allocate pmink payload
	sgn::PminkPayload* pmpld = smpp_conn->in_pool.allocate_constructed();
	if(pmpld != NULL){
	    // clear params
	    pmpld->params = smsg->vpmap;
	    pmpld->action = sgn::PAT_PROCESS;
	    smpp_conn->in_q.push(pmpld);

	// pool empty, err
	}else{
	    // stats
	    smpp_conn->stats.inc(sgn::CST_PMPLD_POOL_EMPTY, 1);
	}

	// dec refc (inc in smpp_conn->get_conn)
	smpp_conn->refc_dec();

	return;

    }


    // ** m3ua ***

    // AS found, get next ASP
    bool broadcast = false;
    as->wrr_mtx.lock();
    sgn::M3UAConnection* m3ua_conn = as->get_next_asp(&broadcast);
    as->wrr_mtx.unlock();

    // no ASP found, try next destination
    if(m3ua_conn == NULL){
	as->stats.inc(sgn::ASST_ASP_NOT_FOUND, 1);
	// process next route
	process_next(smsg, r14p_stream, dest_str, sizeof(dest_str));
	// unlock
	sgnd->as_lock.unlock();
	return;
    }

    // AS found, inc ref counter, set pointer
    m3ua_conn->refc_inc();

    // unlock
    sgnd->as_lock.unlock();


    // allocate pmink payload
    sgn::PminkPayload* pmpld = m3ua_conn->in_pool.allocate_constructed();
    if(pmpld != NULL){
	// clear params
	pmpld->params = smsg->vpmap;
	pmpld->action = sgn::PAT_PROCESS;
	m3ua_conn->in_q.push(pmpld);

    // pool empty, err
    }else{
	// stats
	m3ua_conn->stats.inc(sgn::CST_PMPLD_POOL_EMPTY, 1);
    }

    // dec refc
    m3ua_conn->refc_dec();

}
// SrvcMsgErr
void SrvcMsgErr::run(r14p::R14PCallbackArgs* args){
    // get daemon pointer
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // stats
    sgnd->stats.inc(sgn::SST_SMSG_MSG_POOL_EMPTY, 1);

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


// AS_WRR_Remove
void AS_WRR_Remove::run(pmink_utils::EventArgs<sgn::ConnectiontEventArgType, void*>& args){
    // get ASP
    void** p_arg = args.get_arg(sgn::CEAT_CONNECTION);
    sgn::M3UAConnection* m3ua_conn = (sgn::M3UAConnection*)*p_arg;
    // get AS
    sgn::ApplicationServer* as = m3ua_conn->as.get();
    // lock
    as->wrr_mtx.lock();
    // remove from WRR
    as->get_wrr()->remove(m3ua_conn->get_label());
    // unlock
    as->wrr_mtx.unlock();

}

// AS_WRR_Add
void AS_WRR_Add::run(pmink_utils::EventArgs<sgn::ConnectiontEventArgType, void*>& args){
    // get ASP
    void** p_arg = args.get_arg(sgn::CEAT_CONNECTION);
    sgn::M3UAConnection* m3ua_conn = (sgn::M3UAConnection*)*p_arg;
    // get AS
    sgn::ApplicationServer* as = m3ua_conn->as.get();
    // lock
    as->wrr_mtx.lock();
    // add to WRR
    as->get_wrr()->add_item(m3ua_conn, m3ua_conn->get_label(), 1);
    // unlock
    as->wrr_mtx.unlock();


}

// SMPP_WRR_Add
void SMPP_WRR_Add::run(pmink_utils::EventArgs<sgn::ConnectiontEventArgType, void*>& args){
    // get SMPP connection
    void** p_arg = args.get_arg(sgn::CEAT_CONNECTION);
    sgn::SMPPConnection* smpp_conn = (sgn::SMPPConnection*)*p_arg;
    sgn::SMPPConnection* conn_p = smpp_conn;
    // get smpp master conn
    sgn::SMPPConnection* smpp_master_conn = (sgn::SMPPConnection*)smpp_conn->get_master_conn();
    // check if one to many connection
    if(smpp_master_conn != NULL){
	// conn pointer update
	conn_p = smpp_master_conn;
	// add to list
	smpp_master_conn->usr_map_mtx.lock();
	smpp_master_conn->clients.add_item(smpp_conn, smpp_conn->get_label(), 1);
	smpp_master_conn->usr_map_mtx.unlock();
	return;
    }

    // get AS
    sgn::SMPP_ApplicationServer* as = conn_p->as.get();
    // lock
    as->wrr_mtx.lock();
    // add to WRR
    as->get_wrr()->add_item(conn_p, conn_p->get_label(), 1);
    // unlock
    as->wrr_mtx.unlock();
}

// SMPP_WRR_Remove
void SMPP_WRR_Remove::run(pmink_utils::EventArgs<sgn::ConnectiontEventArgType, void*>& args){
    // get SMPP connection
    void** p_arg = args.get_arg(sgn::CEAT_CONNECTION);
    sgn::SMPPConnection* smpp_conn = (sgn::SMPPConnection*)*p_arg;
    sgn::SMPPConnection* conn_p = smpp_conn;
    // get smpp master conn
    sgn::SMPPConnection* smpp_master_conn = (sgn::SMPPConnection*)smpp_conn->get_master_conn();
    // check if one to many connection
    if(smpp_master_conn != NULL){
	// conn pointer update
	conn_p = smpp_master_conn;
	// remove from list
	smpp_master_conn->usr_map_mtx.lock();
	// remove from children list
	int rc = smpp_master_conn->clients.remove(smpp_conn->get_label());
	smpp_master_conn->usr_map_mtx.unlock();
	// dec master conn user bind counter if client removal was successful
	if(rc == 0) {
	    smpp_master_conn->bind_dec(smpp_conn->smpp_user.system_id);
	}
	return;

    }


    // get AS
    sgn::SMPP_ApplicationServer* as = conn_p->as.get();
    // lock
    as->wrr_mtx.lock();
    // remove from WRR
    as->get_wrr()->remove(conn_p->get_label());
    // unlock
    as->wrr_mtx.unlock();
}



