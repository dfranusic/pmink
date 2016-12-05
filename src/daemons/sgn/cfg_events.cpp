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

#include "cfg_events.h"
#include <sgn.h>
#include <sgn_stats.h>

// SMPP_ASPConfigMod
void SMPP_ASPConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* asp_root = cfg->children[0];
    // get root
    asp_root = asp_root->find_parent("asp");
    // sanity check
    if(asp_root == NULL) return;
    // tmp vars
    config::ConfigItem* asp_node = NULL;
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // lock
    sgnd->smpp_as_lock.lock();

    // loop asps
    for(unsigned int i = 0; i<asp_root->children.size(); i++){
	asp_node = asp_root->children[i];
	// rule deleted, continue
	if(asp_node->node_state == config::CONFIG_NS_DELETED){
	    del_conn(asp_node, NULL);

	// new asp connection
	}else if(asp_node->is_new){
	    add_conn(asp_node);

	// check for modifications
	}else if(asp_node->is_modified()){
	    // recreate connection
	    bool active = false;
	    sgn::SMPP_ApplicationServer* as = del_conn(asp_node, &active);
	    add_conn(asp_node, as, active);

	}

    }

    // unlock
    sgnd->smpp_as_lock.unlock();

}

void SMPP_ASPConfigMod::add_conn(config::ConfigItem* asp_node, sgn::SMPP_ApplicationServer* as, bool conn_active){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // log
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up SMPP ASP [%s]...", asp_node->name.c_str());

    // sanity check
    std::map<std::string, sgn::SMPPConnection*>::iterator it = sgnd->smpp_asp_map.find(asp_node->name);
    // err, already exists
    if(it != sgnd->smpp_asp_map.end()){
	// ASP exists
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "SMPP ASP [%s] already exists!",
				   asp_node->name.c_str());
	// next
	return;
    }

    int errc = 0;
    int mtu = 0;
    int pool = 0;

    config::ConfigItem* smpp_users = NULL;
    config::ConfigItem* smpp_user = NULL;

    // check for mandatory smpp params
    if(!asp_node->val_exists("smpp bind_method")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									     "Missing mandatory configuration item [smpp asp %s smpp bind_method]!",
									     asp_node->name.c_str(), errc++);

    //  min 1 user has to be defined
    if((*asp_node)("smpp users") == NULL) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
								      "Missing SMPP users configuration node set for SMPP ASP [%s]!", asp_node->name.c_str(), errc++);
    else{
	smpp_users = (*asp_node)("smpp users");
	if(smpp_users->children.size() == 0) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
								       "Missing SMPP users information for SMPP ASP [%s]!", asp_node->name.c_str(), errc++);

    }



    if(asp_node->to_int("mode") == sgn::CD_CLIENT){
	// check for mandatory tcp params
	if(!asp_node->val_exists("tcp remote ip")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									        "Missing mandatory configuration item [smpp asp %s tcp remote ip]!",
									        asp_node->name.c_str(), errc++);

	if(!asp_node->val_exists("tcp remote port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									         "Missing mandatory configuration item [smpp asp %s tcp remote port]!",
									         asp_node->name.c_str(), errc++);

    }else if(asp_node->to_int("mode") == sgn::CD_SERVER){
	// check for mandatory tcp params
	if(!asp_node->val_exists("tcp local port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									        "Missing mandatory configuration item [smpp asp %s tcp local port]!",
									        asp_node->name.c_str(), errc++);

    }

    // mtu
    mtu = asp_node->to_int("mtu", asp_node->parent->parent->to_int("mtu", 1024));
    if(mtu < 100 || mtu > 65535){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid MTU [%d] for ASP [%s], valid range = [100 - 65535]!",
						       mtu, asp_node->name.c_str(), errc++);
    }
    // pool
    pool = asp_node->to_int("pool", asp_node->parent->parent->to_int("pool", 4096));
    if(pool < 10){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid pool size [%d] for ASP [%s], valid range = [10 - 2147483647]!",
						       pool, asp_node->name.c_str(), errc++);

    }

    if(errc > 0){
	// SMPP connections contains errors
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot add SMPP ASP [%s], missing mandatory configuration items!",
				   asp_node->name.c_str());


    // init connection
    }else if(errc == 0){

	sgn::SMPPConnection* smpp_conn = new sgn::SMPPConnection((sgn::ConnectionDirection)asp_node->to_int("mode", 0),
								 sgn::SCT_ONE_MANY,
								 asp_node->name.c_str(),
								 mtu,
								 pool);



	// if SERVER mode set max conn count to 2 for main master conn (2 or more)
	if(smpp_conn->get_direction() == sgn::CD_SERVER) smpp_conn->params.set_int(sgn::CPT_MAX_CONN_COUNT, 2);

	// stats
	smpp_conn->params.set_pointer(sgn::CPT_R14P_STATS, sgnd->r14p_stats);

	// local port range
	smpp_conn->params.set_pointer(sgn::CPT_LOCAL_PORT_RANGE, &sgnd->local_ports);

	// event handlers
	smpp_conn->get_e_handler()->set_handler(sgn::CET_CONNECTION_UP, &sgnd->smpp_wrr_add_handler);
	smpp_conn->get_e_handler()->set_handler(sgn::CET_CONNECTION_DOWN, &sgnd->smpp_wrr_remove_handler);

	// add stats handlers
	std::string stats_prefix("SMPP::");
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_MSU")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_RX_MSU_COUNT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_MSU")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_TX_MSU_COUNT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_MTU_LIMIT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_MTU_LIMIT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_DISCARDED_MSU")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_DISCARDED_MSU));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_PLD_PPOOL_EMPTY")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_PLD_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_MALFORMED_MSU")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_MALFORMED_MSU));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_RX_ERROR));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_TX_ERROR));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_PMPLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_PMPLD_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_SEND_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMSG_SEND_ERROR));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_MSG_POOL_EMPTY")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMSG_MSG_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_PARAM_POOL_EMPTY")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMSG_PARAM_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMPP_ACTIVE")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMPP_ACTIVE));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TCP_ACTIVE")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_TCP_ACTIVE));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_SENT_COUNT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMSG_SENT_COUNT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_NO_VALID_ROUTES")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_NO_VALID_ROUTES));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_RX_HBEAT_COUNT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_TX_HBEAT_COUNT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_CMD_PROCESS_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_CMD_PROCESS_ERROR));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_ENCODE_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_ENCODE_ERROR));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RECONNECT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_RECONNECT));

	// tcp ip and port
	smpp_conn->params.set_cstr(sgn::CPT_TCP_REMOTE_IP, asp_node->to_cstr("tcp remote ip"));
	smpp_conn->params.set_int(sgn::CPT_TCP_REMOTE_PORT, asp_node->to_int("tcp remote port"));
	if(asp_node->val_exists("tcp local port")) smpp_conn->params.set_int(sgn::CPT_TCP_LOCAL_PORT, asp_node->to_int("tcp local port"));
	if(asp_node->val_exists("tcp local ip")) smpp_conn->params.set_cstr(sgn::CPT_TCP_LOCAL_IP, asp_node->to_cstr("tcp local ip"));

	// smpp timers
	smpp_conn->params.set_int(sgn::CPT_SMPP_SESSION_INIT_TIMER, asp_node->to_int("smpp timers session_init_timer", 10));
	smpp_conn->params.set_int(sgn::CPT_SMPP_ENQUIRE_LINK_TIMER, asp_node->to_int("smpp timers enquire_link_timer", 10));
	smpp_conn->params.set_int(sgn::CPT_SMPP_INACTIVITY_TIMER, asp_node->to_int("smpp timers inactivity_timer", 60));
	smpp_conn->params.set_int(sgn::CPT_SMPP_RESPONSE_TIMER, asp_node->to_int("smpp timers response_timer", 5));
	smpp_conn->params.set_int(sgn::CPT_RECONNECT_TIMER, asp_node->to_int("smpp timers response_timer", 5));

	// smpp
	smpp_conn->params.set_int(sgn::CPT_SMPP_BIND_METHOD, asp_node->to_int("smpp bind_method", smpp::BIND_TRANSCEIVER));

	// generate user map
	for(unsigned int j = 0; j<smpp_users->children.size(); j++){
	    smpp_user = smpp_users->children[j];
	    smpp_conn->usr_map[smpp_user->name] = sgn::SMPPUser(smpp_user->name.c_str(),
								smpp_user->to_cstr("password"),
								smpp_user->to_cstr("system_type"),
								smpp_user->to_int("max_conns", 1),
								(smpp::SmppTON)smpp_user->to_int("addr_ton", smpp::TON_UNKNOWN),
								(smpp::SmppNPI)smpp_user->to_int("addr_npi", smpp::UNKNOWN),
								smpp_user->to_int("interface_version", 0x34));

	}
	// user first user for CLIENT type connection
	sgn::SMPPUser& tmp_usr = smpp_conn->usr_map.begin()->second;
	smpp_conn->params.set_cstr(sgn::CPT_SMPP_BIND_SYSTEM_ID, tmp_usr.system_id);
	smpp_conn->params.set_cstr(sgn::CPT_SMPP_BIND_PASSWORD, tmp_usr.password);
	smpp_conn->params.set_cstr(sgn::CPT_SMPP_BIND_SYSTEM_TYPE, tmp_usr.system_type);
	smpp_conn->params.set_int(sgn::CPT_SMPP_BIND_IF_VERSION, tmp_usr.interface_version);
	smpp_conn->params.set_int(sgn::CPT_SMPP_BIND_TON, tmp_usr.ton);
	smpp_conn->params.set_int(sgn::CPT_SMPP_BIND_NPI, tmp_usr.npi);

	// global
	smpp_conn->params.set_bool(sgn::CPT_DATA_RETENTION, asp_node->to_int("dr", 0));
	smpp_conn->params.set_bool(sgn::CPT_DEV_SMPP_SM, ((int)*sgnd->extra_params.get_param(5) > 0 ? true : false));

	// link to data processor
	sgnd->dprocessors_lock.lock();
	sgn::DataProcessor* dproc = sgnd->dprocessors.run()->item;
	sgnd->dprocessors_lock.unlock();
	smpp_conn->params.set_pointer(sgn::CPT_DATA_PROCESSOR, dproc);
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Linking SMPP ASP [%s] with data processor [%d]", smpp_conn->get_label(), dproc->id);
	// create logic
	smpp_conn->set_logic(new sgn::DefaultConnectionLogic<sgn::SmppPayload>(smpp_conn));

	// add to list
	sgnd->smpp_asp_map[asp_node->name] = smpp_conn;

	// link with AS
	if(as != NULL) update_link(smpp_conn, as, conn_active);

    }


}

sgn::SMPP_ApplicationServer* SMPP_ASPConfigMod::del_conn(config::ConfigItem* asp_node, bool* active){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    sgn::SMPP_ApplicationServer* res = NULL;
    // get asp
    std::map<std::string, sgn::SMPPConnection*>::iterator it = sgnd->smpp_asp_map.find(asp_node->name);
    if(it == sgnd->smpp_asp_map.end()){
	// ASP not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find SMPP ASP [%s]!",
				   asp_node->name.c_str());

	// next
	return NULL;
    }
    // conn pointer
    sgn::SMPPConnection* smpp_conn = it->second;
    // check if active
    if(active != NULL){
	*active = smpp_conn->active.get();
    }
    // get as pointer
    sgn::SMPP_ApplicationServer* as = smpp_conn->as.get();
    // no AS link in ASP
    if(as == NULL){
	// AS not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find SMPP AS link for SMPP ASP [%s]!",
				   asp_node->name.c_str());


    // AS link in ASP found
    }else{
	// unlink
	it = as->asp_map.find(asp_node->name);
	// ASP link in AS missing
	if(it == as->asp_map.end()){
	    // ASP not found
	    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				       "Cannot find SMPP ASP [%s] link for SMPP AS [%s]!",
				       asp_node->name.c_str(),
				       as->get_label());


	// ASP link in AS found
	}else{
	    // use it for result
	    res = as;

	    // unlink from AS
	    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Unlinking SMPP ASP [%s] from SMPP AS [%s]...",
							  asp_node->name.c_str(),
							  as->get_label());
	    // remove wrr item for LOADSHARE
	    as->wrr_mtx.lock();
	    as->get_wrr()->remove(smpp_conn->get_label());
	    as->wrr_mtx.unlock();
	    // remove
	    as->asp_map.erase(it);

	}


    }

    // remove from list
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Removing SMPP ASP [%s]...",
						  asp_node->name.c_str());
    sgnd->smpp_asp_map.erase(asp_node->name);
    // stop children
    smpp_conn->stop_children();
    // stop
    smpp_conn->stop();
    // remove stats (to avoid name clashes due do delayed deallocation)
    smpp_conn->terminate();
    // dec ref counter
    // m3ua_conn pointer might not valid after call to dec()
    if(smpp_conn->refc_dec() == 0){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping SMPP ASP [%s]...",
						     asp_node->name.c_str());

    }

    return res;

}

void SMPP_ASPConfigMod::update_link(sgn::SMPPConnection* smpp_conn, sgn::SMPP_ApplicationServer* as, bool conn_active){
    // sanity check
    if(as == NULL || smpp_conn == NULL) return;
    // set ASP link in AS
    as->asp_map[smpp_conn->get_label()] = smpp_conn;
    // set AS link in ASP
    smpp_conn->as.set(as);
    // add to WRR if server connection (one to many connection)
    if(smpp_conn->get_direction() == sgn::CD_SERVER) as->get_wrr()->add_item(smpp_conn, smpp_conn->get_label(), 1);
    // log
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
			       "Setting up SMPP ASP [%s] link connection for SMPP AS [%s]...",
			       smpp_conn->get_label(), as->get_label());

    // start if active
    if(conn_active){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting SMPP ASP [%s]...", smpp_conn->get_label());
	// start
	smpp_conn->start();
	// get data processor
	sgn::DataProcessor* dproc = (sgn::DataProcessor*)(void*)*smpp_conn->params.get_param(sgn::CPT_DATA_PROCESSOR);
	// add to dproc conn list
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Adding SMPP ASP [%s] to data processor [%d]", smpp_conn->get_label(), dproc->id);
	if(smpp_conn->get_direction() == sgn::CD_CLIENT){
	    dproc->client_conns.update_start();
	    smpp_conn->refc_inc();
	    (*dproc->client_conns).push_back(smpp_conn);
	    dproc->client_conns.update_end();

	}else{
	    dproc->server_conns.update_start();
	    smpp_conn->refc_inc();
	    (*dproc->server_conns).push_back(smpp_conn);
	    dproc->server_conns.update_end();

	}

    }
}


// SMPP_ASConfigMod
void SMPP_ASConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* as_root = cfg->children[0];
    // get root
    as_root = as_root->find_parent("as");
    // sanity check
    if(as_root == NULL) return;
    // tmp vars
    config::ConfigItem* as_node = NULL;
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // lock
    sgnd->smpp_as_lock.lock();

    // loop asps
    for(unsigned int i = 0; i<as_root->children.size(); i++){
	as_node = as_root->children[i];
	// rule deleted, continue
	if(as_node->node_state == config::CONFIG_NS_DELETED){
	    del_as(as_node);

	// new asp connection
	}else if(as_node->is_new){
	    add_as(as_node);

	// check for modifications
	}else if(as_node->is_modified()){
	    del_as(as_node);
	    add_as(as_node);

	}

    }

    // unlock
    sgnd->smpp_as_lock.unlock();

}

void SMPP_ASConfigMod::del_as(config::ConfigItem* as_node){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // find as
    std::map<std::string, sgn::SMPP_ApplicationServer*>::iterator it = sgnd->smpp_as_map.find(as_node->name);
    // check iffound
    if(it == sgnd->smpp_as_map.end()) return;
    // as pointer
    sgn::SMPP_ApplicationServer* as = it->second;
    // stop and unlink all linked ASPs
    for(std::map<std::string, sgn::SMPPConnection*>::iterator asp_it = as->asp_map.begin(); asp_it != as->asp_map.end(); asp_it++){
	// conn pointer
	sgn::SMPPConnection* smpp_conn = asp_it->second;
	// unlink from AS
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Unlinking SMPP ASP [%s] from SMPP AS [%s]...",
						      smpp_conn->get_label(),
						      as->get_label());

	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping SMPP ASP [%s]...", smpp_conn->get_label());
	// stop children
	smpp_conn->stop_children();
	// stop
	smpp_conn->stop();
	// remove link
	smpp_conn->as.set(NULL);

    }
    // remove from list
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Removing SMPP AS [%s]...",
						 as_node->name.c_str());

    // clear map
    as->asp_map.clear();
    // remove
    sgnd->smpp_as_map.erase(it);
    // free mem
    delete as;
}

void SMPP_ASConfigMod::add_as(config::ConfigItem* as_node){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // find as
    std::map<std::string, sgn::SMPP_ApplicationServer*>::iterator it = sgnd->smpp_as_map.find(as_node->name);
    // check if found
    if(it != sgnd->smpp_as_map.end()) {
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot add SMPP AS [%s], already present!",
						     as_node->name.c_str());
	return;
    }
    // add
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up SMPP AS [%s]...", as_node->name.c_str());
    sgn::SMPP_ApplicationServer* as = new sgn::SMPP_ApplicationServer(as_node->name.c_str());
    sgnd->smpp_as_map[as_node->name] = as;

    // stats
    as->params.set_pointer(sgn::CPT_R14P_STATS, sgnd->r14p_stats);
    // add stats handlers
    std::string stats_prefix("SMPP::");
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + as_node->name + "_ASP_NOT_FOUND")), new sgn::SgnStatsHandler(&as->stats, sgn::ASST_ASP_NOT_FOUND));

    // check ASP connections for current AS
    config::ConfigItem* asp_nodes = (*as_node)("asp");
    config::ConfigItem* asp_node = NULL;
    if(asp_nodes == NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing SMPP ASP links for AS [%s]!", as_node->name.c_str());
	return;
    }

    // setup config on change events
    asp_nodes->set_on_change_handler(&sgnd->smpp_as_asp_mod_handler, true);

    // loop ASP
    for(unsigned int j = 0; j<asp_nodes->children.size(); j++){
	// asp node
	asp_node = asp_nodes->children[j];
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
				   "Setting up SMPP ASP [%s] link connection for SMPP AS [%s]...",
				   asp_node->name.c_str(), as_node->name.c_str());

	// find ASP connection
	std::map<std::string, sgn::SMPPConnection*>::iterator it = sgnd->smpp_asp_map.find(asp_node->name);
	if(it == sgnd->smpp_asp_map.end()){
	    // ASP connection not found
	    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				       "SMPP ASP [%s] does not exist, cannot link with SMPP AS [%s]!",
				       asp_node->name.c_str(), as_node->name.c_str());
	    // next asp
	    continue;
	}

	// set conn pointer
	sgn::SMPPConnection* smpp_conn = it->second;

	// get as pointer
	sgn::SMPP_ApplicationServer* _tmp_as = smpp_conn->as.get();

	if(_tmp_as != NULL){
	    // ASP connection not found
	    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				       "Cannot link SMPP ASP [%s] with SMPP AS [%s], already linked with SMPP AS [%s]!",
				       asp_node->name.c_str(),
				       as_node->name.c_str(),
				       as->get_label());

	    // next asp
	    continue;
	}


	// link
	as->asp_map[asp_node->name] = smpp_conn;
	smpp_conn->as.set(as);
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
				   "SMPP ASP [%s] successfully linked with SMPP AS [%s]",
				   asp_node->name.c_str(), as_node->name.c_str());

	// start ASP if active
	if(asp_node->to_int("active", 0) == 1){
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting SMPP ASP [%s]...", smpp_conn->get_label());
	    // start
	    smpp_conn->start();
	    // get data processor
	    sgn::DataProcessor* dproc = (sgn::DataProcessor*)(void*)*smpp_conn->params.get_param(sgn::CPT_DATA_PROCESSOR);
	    // add to dproc conn list
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Adding SMPP ASP [%s] to data processor [%d]", smpp_conn->get_label(), dproc->id);
	    if(smpp_conn->get_direction() == sgn::CD_CLIENT){
		dproc->client_conns.update_start();
		smpp_conn->refc_inc();
		(*dproc->client_conns).push_back(smpp_conn);
		dproc->client_conns.update_end();

	    }else{
		dproc->server_conns.update_start();
		smpp_conn->refc_inc();
		(*dproc->server_conns).push_back(smpp_conn);
		dproc->server_conns.update_end();

	    }

	}

    }
}

// SMPP_AS_ASPConfigMod
void SMPP_AS_ASPConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* asp_root = cfg->children[0];
    // get root
    asp_root = asp_root->find_parent("asp");
    // sanity check
    if(asp_root == NULL) return;
    // tmp vars
    config::ConfigItem* asp_node = NULL;
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // lock
    sgnd->smpp_as_lock.lock();

    // loop asps
    for(unsigned int i = 0; i<asp_root->children.size(); i++){
	asp_node = asp_root->children[i];
	// rule deleted, continue
	if(asp_node->node_state == config::CONFIG_NS_DELETED){
	    del_as_asp(asp_node);

	// new asp connection
	}else if(asp_node->is_new){
	    add_as_asp(asp_node);

	// check for modifications
	}else if(asp_node->is_modified()){
	    del_as_asp(asp_node);
	    add_as_asp(asp_node);
	}

    }

    // unlock
    sgnd->smpp_as_lock.unlock();


}

void SMPP_AS_ASPConfigMod::del_as_asp(config::ConfigItem* asp_node){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // get asp
    std::map<std::string, sgn::SMPPConnection*>::iterator it = sgnd->smpp_asp_map.find(asp_node->name);
    if(it == sgnd->smpp_asp_map.end()){
	// ASP not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find SMPP ASP [%s]!",
				   asp_node->name.c_str());

	// next
	return;
    }
    // conn pointer
    sgn::SMPPConnection* smpp_conn = it->second;
    // get as pointer
    sgn::SMPP_ApplicationServer* as = smpp_conn->as.get();
    // no AS link in ASP
    if(as == NULL){
	// AS not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find SMPP AS link for SMPP ASP [%s]!",
				   asp_node->name.c_str());

	return;
    }

    // get ASP in AS
    it = as->asp_map.find(asp_node->name);
    // ASP link in AS missing
    if(it == as->asp_map.end()){
	// ASP not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find SMPP ASP [%s] link for SMPP AS [%s]!",
				   asp_node->name.c_str(),
				   as->get_label());


    // ASP link in AS found
    }else{
	// unlink from AS
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Unlinking SMPP ASP [%s] from SMPP AS [%s]...",
						      asp_node->name.c_str(),
						      as->get_label());
	// remove wrr item for LOADSHARE
	as->wrr_mtx.lock();
	as->get_wrr()->remove(smpp_conn->get_label());
	as->wrr_mtx.unlock();
	// remove
	as->asp_map.erase(it);
	// stop
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping SMPP ASP [%s]...", smpp_conn->get_label());
	// stop children
	smpp_conn->stop_children();
	// stop
	smpp_conn->stop();
	// remove link
	smpp_conn->as.set(NULL);


    }

}


void SMPP_AS_ASPConfigMod::add_as_asp(config::ConfigItem* asp_node){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // get asp
    std::map<std::string, sgn::SMPPConnection*>::iterator it = sgnd->smpp_asp_map.find(asp_node->name);
    if(it == sgnd->smpp_asp_map.end()){
	// ASP not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find SMPP ASP [%s]!",
				   asp_node->name.c_str());

	// next
	return;
    }

    // conn pointer
    sgn::SMPPConnection* smpp_conn = it->second;
    // get as pointer
    sgn::SMPP_ApplicationServer* as = smpp_conn->as.get();
    // ASP linked with AS
    if(as != NULL){
	// ASP connection not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot link SMPP ASP [%s] with SMPP AS [%s], already linked with SMPP AS [%s]!",
				   asp_node->name.c_str(),
				   asp_node->parent->parent->name.c_str(),
				   as->get_label());

	return;
    }

    // find as
    std::map<std::string, sgn::SMPP_ApplicationServer*>::iterator it_as = sgnd->smpp_as_map.find(asp_node->parent->parent->name);
    // check if found
    if(it_as == sgnd->smpp_as_map.end()) {
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot link SMPP ASP [%s], SMPP AS [%s] does not exist!",
						     asp_node->name.c_str(), asp_node->parent->parent->name.c_str());
	return;
    }
    // get as pointer
    as = it_as->second;

    // link ASP with AS
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
			       "Setting up SMPP ASP [%s] link connection for SMPP AS [%s]...",
			       asp_node->name.c_str(), asp_node->parent->parent->name.c_str());

    as->asp_map[asp_node->name] = smpp_conn;
    smpp_conn->as.set(as);
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
			       "SMPP ASP [%s] successfully linked with SMPP AS [%s]",
			       asp_node->name.c_str(), as->get_label());

    // start ASP if active
    if(asp_node->to_int("active", 0) == 1){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting SMPP ASP [%s]...", smpp_conn->get_label());
	// start
	smpp_conn->start();
	// get data processor
	sgn::DataProcessor* dproc = (sgn::DataProcessor*)(void*)*smpp_conn->params.get_param(sgn::CPT_DATA_PROCESSOR);
	// add to dproc conn list
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Adding SMPP ASP [%s] to data processor [%d]", smpp_conn->get_label(), dproc->id);
	if(smpp_conn->get_direction() == sgn::CD_CLIENT){
	    dproc->client_conns.update_start();
	    smpp_conn->refc_inc();
	    (*dproc->client_conns).push_back(smpp_conn);
	    dproc->client_conns.update_end();

	}else{
	    dproc->server_conns.update_start();
	    smpp_conn->refc_inc();
	    (*dproc->server_conns).push_back(smpp_conn);
	    dproc->server_conns.update_end();

	}


    }

}



// AS_ASPConfigMod
void AS_ASPConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* asp_root = cfg->children[0];
    // get root
    asp_root = asp_root->find_parent("asp");
    // sanity check
    if(asp_root == NULL) return;
    // tmp vars
    config::ConfigItem* asp_node = NULL;
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // lock
    sgnd->as_lock.lock();

    // loop asps
    for(unsigned int i = 0; i<asp_root->children.size(); i++){
	asp_node = asp_root->children[i];
	// rule deleted, continue
	if(asp_node->node_state == config::CONFIG_NS_DELETED){
	    del_as_asp(asp_node);

	// new asp connection
	}else if(asp_node->is_new){
	    add_as_asp(asp_node);

	// check for modifications
	}else if(asp_node->is_modified()){
	    del_as_asp(asp_node);
	    add_as_asp(asp_node);
	}

    }

    // unlock
    sgnd->as_lock.unlock();


}

void AS_ASPConfigMod::del_as_asp(config::ConfigItem* asp_node){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // get asp
    std::map<std::string, sgn::M3UAConnection*>::iterator it = sgnd->asp_map.find(asp_node->name);
    if(it == sgnd->asp_map.end()){
	// ASP not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find ASP [%s]!",
				   asp_node->name.c_str());

	// next
	return;
    }
    // conn pointer
    sgn::M3UAConnection* m3ua_conn = it->second;
    // get as pointer
    sgn::ApplicationServer* as = m3ua_conn->as.get();
    // no AS link in ASP
    if(as == NULL){
	// AS not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find AS link for ASP [%s]!",
				   asp_node->name.c_str());

	return;
    }

    // get ASP in AS
    it = as->asp_map.find(asp_node->name);
    // ASP link in AS missing
    if(it == as->asp_map.end()){
	// ASP not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find ASP [%s] link for AS [%s]!",
				   asp_node->name.c_str(),
				   as->get_label());


    // ASP link in AS found
    }else{
	// unlink from AS
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Unlinking ASP [%s] from AS [%s]...",
						      asp_node->name.c_str(),
						      as->get_label());
	// remove wrr item for LOADSHARE
	as->wrr_mtx.lock();
	as->get_wrr()->remove(m3ua_conn->get_label());
	as->wrr_mtx.unlock();
	// remove
	as->asp_map.erase(it);
	// stop
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping ASP [%s]...", m3ua_conn->get_label());
	m3ua_conn->stop();
	// remove link
	m3ua_conn->as.set(NULL);


    }

}

void AS_ASPConfigMod::add_as_asp(config::ConfigItem* asp_node){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // get asp
    std::map<std::string, sgn::M3UAConnection*>::iterator it = sgnd->asp_map.find(asp_node->name);
    if(it == sgnd->asp_map.end()){
	// ASP not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find ASP [%s]!",
				   asp_node->name.c_str());

	// next
	return;
    }

    // conn pointer
    sgn::M3UAConnection* m3ua_conn = it->second;
    // get as pointer
    sgn::ApplicationServer* as = m3ua_conn->as.get();
    // ASP linked with AS
    if(as != NULL){
	// ASP connection not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot link ASP [%s] with AS [%s], already linked with AS [%s]!",
				   asp_node->name.c_str(),
				   asp_node->parent->parent->name.c_str(),
				   as->get_label());

	return;
    }

    // find as
    std::map<std::string, sgn::ApplicationServer*>::iterator it_as = sgnd->as_map.find(asp_node->parent->parent->name);
    // check if found
    if(it_as == sgnd->as_map.end()) {
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot link ASP [%s], AS [%s] does not exist!",
						     asp_node->name.c_str(), asp_node->parent->parent->name.c_str());
	return;
    }
    // get as pointer
    as = it_as->second;

    // link ASP with AS
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
			       "Setting up ASP [%s] link connection for AS [%s]...",
			       asp_node->name.c_str(), asp_node->parent->parent->name.c_str());

    as->asp_map[asp_node->name] = m3ua_conn;
    m3ua_conn->as.set(as);
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
			       "ASP [%s] successfully linked with AS [%s]",
			       asp_node->name.c_str(), as->get_label());

    // start ASP if active
    if(asp_node->to_int("active", 0) == 1){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting ASP [%s]...", m3ua_conn->get_label());
	// start
	m3ua_conn->start();
	// get data processor
	sgn::DataProcessor* dproc = (sgn::DataProcessor*)(void*)*m3ua_conn->params.get_param(sgn::CPT_DATA_PROCESSOR);
	// add to dproc conn list
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Adding ASP [%s] to data processor [%d]", m3ua_conn->get_label(), dproc->id);
	if(m3ua_conn->get_direction() == sgn::CD_CLIENT){
	    dproc->client_conns.update_start();
	    m3ua_conn->refc_inc();
	    (*dproc->client_conns).push_back(m3ua_conn);
	    dproc->client_conns.update_end();

	}else{
	    dproc->server_conns.update_start();
	    m3ua_conn->refc_inc();
	    (*dproc->server_conns).push_back(m3ua_conn);
	    dproc->server_conns.update_end();

	}

    }


}



// ASConfigMod
void ASConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* as_root = cfg->children[0];
    // get root
    as_root = as_root->find_parent("as");
    // sanity check
    if(as_root == NULL) return;
    // tmp vars
    config::ConfigItem* as_node = NULL;
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // lock
    sgnd->as_lock.lock();

    // loop asps
    for(unsigned int i = 0; i<as_root->children.size(); i++){
	as_node = as_root->children[i];
	// rule deleted, continue
	if(as_node->node_state == config::CONFIG_NS_DELETED){
	    del_as(as_node);

	// new asp connection
	}else if(as_node->is_new){
	    add_as(as_node);

	// check for modifications
	}else if(as_node->is_modified()){
	    del_as(as_node);
	    add_as(as_node);

	}

    }

    // unlock
    sgnd->as_lock.unlock();

}

void ASConfigMod::add_as(config::ConfigItem* as_node){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // find as
    std::map<std::string, sgn::ApplicationServer*>::iterator it = sgnd->as_map.find(as_node->name);
    // check if found
    if(it != sgnd->as_map.end()) {
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot add AS [%s], already present!",
						     as_node->name.c_str());
	return;
    }
    // add
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up AS [%s]...", as_node->name.c_str());
    sgn::ApplicationServer* as = new sgn::ApplicationServer(as_node->name.c_str());
    sgnd->as_map[as_node->name] = as;

    // stats
    as->params.set_pointer(sgn::CPT_R14P_STATS, sgnd->r14p_stats);
    // add stats handlers
    std::string stats_prefix("M3UA::");
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + as_node->name + "_ASP_NOT_FOUND")), new sgn::SgnStatsHandler(&as->stats, sgn::ASST_ASP_NOT_FOUND));

    // routing-key params
    as->params.set_int(sgn::CPT_M3UA_RT_CTX, as_node->to_int("routing-key routing-context", 0xffffffff));
    as->params.set_int(sgn::CPT_M3UA_TRAFFIC_MODE, as_node->to_int("traffic-mode type", 2));

    // decoder params
    as->params.set_bool(sgn::CPT_DECODE_SCCP, as_node->to_int("decoder sccp", 1));
    as->params.set_bool(sgn::CPT_DECODE_ISUP, as_node->to_int("decoder isup", 1));
    as->params.set_bool(sgn::CPT_DECODE_TCAP, as_node->to_int("decoder tcap", 1));
    as->params.set_bool(sgn::CPT_DECODE_GSM_MAP, as_node->to_int("decoder gsm-map", 1));
    as->params.set_bool(sgn::CPT_DECODE_SMS_TPDU, as_node->to_int("decoder sms-tpdu", 1));

    // check ASP connections for current AS
    config::ConfigItem* asp_nodes = (*as_node)("asp");
    config::ConfigItem* asp_node = NULL;
    if(asp_nodes == NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing ASP links for AS [%s]!", as_node->name.c_str());
	return;
    }

    // setup config on change events
    asp_nodes->set_on_change_handler(&sgnd->as_asp_mod_handler, true);

    // loop ASP
    for(unsigned int j = 0; j<asp_nodes->children.size(); j++){
	// asp node
	asp_node = asp_nodes->children[j];
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
				   "Setting up ASP [%s] link connection for AS [%s]...",
				   asp_node->name.c_str(), as_node->name.c_str());

	// find ASP connection
	std::map<std::string, sgn::M3UAConnection*>::iterator it = sgnd->asp_map.find(asp_node->name);
	if(it == sgnd->asp_map.end()){
	    // ASP connection not found
	    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				       "ASP [%s] does not exist, cannot link with AS [%s]!",
				       asp_node->name.c_str(), as_node->name.c_str());
	    // next asp
	    continue;
	}

	// set conn pointer
	sgn::M3UAConnection* m3ua_conn = it->second;

	// get as pointer
	sgn::ApplicationServer* _tmp_as = m3ua_conn->as.get();

	if(_tmp_as != NULL){
	    // ASP connection not found
	    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				       "Cannot link ASP [%s] with AS [%s], already linked with AS [%s]!",
				       asp_node->name.c_str(),
				       as_node->name.c_str(),
				       as->get_label());

	    // next asp
	    continue;
	}


	// link
	as->asp_map[asp_node->name] = m3ua_conn;
	m3ua_conn->as.set(as);
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
				   "ASP [%s] successfully linked with AS [%s]",
				   asp_node->name.c_str(), as_node->name.c_str());

	// start ASP if active
	if(asp_node->to_int("active", 0) == 1){
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting ASP [%s]...", m3ua_conn->get_label());
	    // setup round robin for LOADSHARE
	    //as->get_wrr()->add_item(m3ua_conn, m3ua_conn->get_label(), 1);
	    // start
	    m3ua_conn->start();
	    // get data processor
	    sgn::DataProcessor* dproc = (sgn::DataProcessor*)(void*)*m3ua_conn->params.get_param(sgn::CPT_DATA_PROCESSOR);
	    // add to dproc conn list
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Adding SMPP ASP [%s] to data processor [%d]", m3ua_conn->get_label(), dproc->id);
	    if(m3ua_conn->get_direction() == sgn::CD_CLIENT){
		dproc->client_conns.update_start();
		m3ua_conn->refc_inc();
		(*dproc->client_conns).push_back(m3ua_conn);
		dproc->client_conns.update_end();

	    }else{
		dproc->server_conns.update_start();
		m3ua_conn->refc_inc();
		(*dproc->server_conns).push_back(m3ua_conn);
		dproc->server_conns.update_end();

	    }

	}

    }
}

void ASConfigMod::del_as(config::ConfigItem* as_node){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // find as
    std::map<std::string, sgn::ApplicationServer*>::iterator it = sgnd->as_map.find(as_node->name);
    // check iffound
    if(it == sgnd->as_map.end()) return;
    // as pointer
    sgn::ApplicationServer* as = it->second;
    // stop and unlink all linked ASPs
    for(std::map<std::string, sgn::M3UAConnection*>::iterator asp_it = as->asp_map.begin(); asp_it != as->asp_map.end(); asp_it++){
	// conn pointer
	sgn::M3UAConnection* m3ua_conn = asp_it->second;
	// unlink from AS
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Unlinking ASP [%s] from AS [%s]...",
						      m3ua_conn->get_label(),
						      as->get_label());

	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping ASP [%s]...", m3ua_conn->get_label());
	// stop
	m3ua_conn->stop();
	// remove link
	m3ua_conn->as.set(NULL);

    }
    // remove from list
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Removing AS [%s]...",
						 as_node->name.c_str());

    // clear map
    as->asp_map.clear();
    // remove
    sgnd->as_map.erase(it);
    // free mem
    delete as;
}


// ASPConfigMod
sgn::ApplicationServer* ASPConfigMod::del_conn(config::ConfigItem* asp_node, bool* active){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    sgn::ApplicationServer* res = NULL;
    // get asp
    std::map<std::string, sgn::M3UAConnection*>::iterator it = sgnd->asp_map.find(asp_node->name);
    if(it == sgnd->asp_map.end()){
	// ASP not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find ASP [%s]!",
				   asp_node->name.c_str());

	// next
	return NULL;
    }
    // conn pointer
    sgn::M3UAConnection* m3ua_conn = it->second;
    // check if active
    if(active != NULL){
	*active = m3ua_conn->active.get();
    }
    // get as pointer
    sgn::ApplicationServer* as = m3ua_conn->as.get();
    // no AS link in ASP
    if(as == NULL){
	// AS not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find AS link for ASP [%s]!",
				   asp_node->name.c_str());


    // AS link in ASP found
    }else{
	// unlink
	it = as->asp_map.find(asp_node->name);
	// ASP link in AS missing
	if(it == as->asp_map.end()){
	    // ASP not found
	    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				       "Cannot find ASP [%s] link for AS [%s]!",
				       asp_node->name.c_str(),
				       as->get_label());


	// ASP link in AS found
	}else{
	    // use it for result
	    res = as;

	    // unlink from AS
	    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Unlinking ASP [%s] from AS [%s]...",
							  asp_node->name.c_str(),
							  as->get_label());
	    // remove wrr item for LOADSHARE
	    as->wrr_mtx.lock();
	    as->get_wrr()->remove(m3ua_conn->get_label());
	    as->wrr_mtx.unlock();
	    // remove
	    as->asp_map.erase(it);

	}


    }

    // remove from list
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Removing ASP [%s]...",
						  asp_node->name.c_str());
    sgnd->asp_map.erase(asp_node->name);

    // stop
    m3ua_conn->stop();
    // remove stats (to avoid name clashes due do delayed deallocation)
    m3ua_conn->terminate();
    // dec ref counter
    // m3ua_conn pointer might not valid after call to dec()
    if(m3ua_conn->refc_dec() == 0){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping ASP [%s]...",
						     asp_node->name.c_str());

    }
    return res;
}

void ASPConfigMod::add_conn(config::ConfigItem* asp_node, sgn::ApplicationServer* as, bool conn_active){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // log
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up ASP [%s]...", asp_node->name.c_str());

    // sanity check
    std::map<std::string, sgn::M3UAConnection*>::iterator it = sgnd->asp_map.find(asp_node->name);
    // err, already exists
    if(it != sgnd->asp_map.end()){
	// ASP exists
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "ASP [%s] already exists!",
				   asp_node->name.c_str());
	// next
	return;
    }


    int errc = 0;
    int mtu = 0;
    int pool = 0;

    if(asp_node->to_int("mode") == sgn::CD_CLIENT){
	// check for mandatory m3ua params
	if(!asp_node->val_exists("m3ua opc")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									 "Missing mandatory configuration item [m3ua opc] for ASP [%s]!",
									 asp_node->name.c_str(), errc++);

	// check for mandatory sctp params
	if(!asp_node->val_exists("sctp remote ip1")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
								   "Missing mandatory configuration item [sctp remote ip1] for ASP [%s]!",
								   asp_node->name.c_str(), errc++);

	if(!asp_node->val_exists("sctp remote port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
								   "Missing mandatory configuration item [sctp remote port] for ASP [%s]!",
								   asp_node->name.c_str(), errc++);

    }else if(asp_node->to_int("mode") == sgn::CD_SERVER){
	// check for mandatory sctp params
	if(!asp_node->val_exists("sctp local port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
								   "Missing mandatory configuration item [sctp local port] for ASP [%s]!",
								   asp_node->name.c_str(), errc++);

    }

    // mtu
    mtu = asp_node->to_int("mtu", asp_node->parent->parent->to_int("mtu", 1024));
    if(mtu < 100 || mtu > 65535){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid MTU [%d] for ASP [%s], valid range = [100 - 65535]!",
						       mtu, asp_node->name.c_str(), errc++);
    }
    // pool
    pool = asp_node->to_int("pool", asp_node->parent->parent->to_int("pool", 4096));
    if(pool < 10){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid pool size [%d] for ASP [%s], valid range = [10 - 2147483647]!",
						       pool, asp_node->name.c_str(), errc++);

    }

    // err check
    if(errc > 0){
	// ASP exists
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot add ASP [%s], missing mandatory configuration items!",
				   asp_node->name.c_str());
	// next
	return;


    }


    // create connection
    sgn::M3UAConnection* m3ua_conn = new sgn::M3UAConnection((sgn::ConnectionDirection)asp_node->to_int("mode", 0),
							     sgn::SCT_ONE_ONE,
							     asp_node->name.c_str(),
							     mtu,
							     pool);
    // stats
    m3ua_conn->params.set_pointer(sgn::CPT_R14P_STATS, sgnd->r14p_stats);

    // local port range
    m3ua_conn->params.set_pointer(sgn::CPT_LOCAL_PORT_RANGE, &sgnd->local_ports);

    // event handlers
    m3ua_conn->get_e_handler()->set_handler(sgn::CET_CONNECTION_UP, &sgnd->as_wrr_add_handler);
    m3ua_conn->get_e_handler()->set_handler(sgn::CET_CONNECTION_DOWN, &sgnd->as_wrr_remove_handler);


    // add stats handlers
    std::string stats_prefix("M3UA::");
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_MSU")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_RX_MSU_COUNT));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_MSU")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_TX_MSU_COUNT));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_MTU_LIMIT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_MTU_LIMIT));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_DISCARDED_MSU")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_DISCARDED_MSU));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_PLD_PPOOL_EMPTY")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_PLD_POOL_EMPTY));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_MALFORMED_MSU")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_MALFORMED_MSU));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_RX_ERROR));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_TX_ERROR));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_PMPLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_PMPLD_POOL_EMPTY));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_SEND_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SMSG_SEND_ERROR));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_MSG_POOL_EMPTY")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SMSG_MSG_POOL_EMPTY));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_PARAM_POOL_EMPTY")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SMSG_PARAM_POOL_EMPTY));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_M3UA_ACTIVE")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_M3UA_ACTIVE));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SCTP_ACTIVE")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SCTP_ACTIVE));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_SENT_COUNT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SMSG_SENT_COUNT));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_NO_VALID_ROUTES")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_NO_VALID_ROUTES));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_RX_HBEAT_COUNT));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_TX_HBEAT_COUNT));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_CMD_PROCESS_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_CMD_PROCESS_ERROR));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_ENCODE_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_ENCODE_ERROR));
    sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RECONNECT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_RECONNECT));

    // sctp ip and port
    m3ua_conn->params.set_cstr(sgn::CPT_SCTP_REMOTE_IP_1, asp_node->to_cstr("sctp remote ip1"));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_REMOTE_PORT, asp_node->to_int("sctp remote port"));
    if(asp_node->val_exists("sctp remote ip2")) m3ua_conn->params.set_cstr(sgn::CPT_SCTP_REMOTE_IP_2, asp_node->to_cstr("sctp remote ip2"));
    if(asp_node->val_exists("sctp local port")) m3ua_conn->params.set_int(sgn::CPT_SCTP_LOCAL_PORT, asp_node->to_int("sctp local port"));
    if(asp_node->val_exists("sctp local ip1")) m3ua_conn->params.set_cstr(sgn::CPT_SCTP_LOCAL_IP_1, asp_node->to_cstr("sctp local ip1"));
    if(asp_node->val_exists("sctp local ip2")) m3ua_conn->params.set_cstr(sgn::CPT_SCTP_LOCAL_IP_2, asp_node->to_cstr("sctp local ip2"));

    // sctp timers
    m3ua_conn->params.set_int(sgn::CPT_SCTP_HB_INTERVAL, asp_node->to_int("sctp timers hb-interval", 30000));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_MAX_BURST, asp_node->to_int("sctp timers max-burst", 4));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_MAX_INIT_RETRANS, asp_node->to_int("sctp timers max-init-retransmit", 8));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_PATH_MAX_RETRANS, asp_node->to_int("sctp timers path-max-retrans", 5));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_RTO_INITIAL, asp_node->to_int("sctp timers rto-initial", 3000));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_RTO_MAX, asp_node->to_int("sctp timers rto-max", 60000));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_RTO_MIN, asp_node->to_int("sctp timers rto-min", 1000));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_SACK_TIMEOUT, asp_node->to_int("sctp timers sack-timeout", 200));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_SACK_FREQ, asp_node->to_int("sctp timers sack-freq", 2));
    m3ua_conn->params.set_int(sgn::CPT_SCTP_VALID_COOKIE_LIFE, asp_node->to_int("sctp timers valid-cookie-life", 60000));

    // m3ua
    m3ua_conn->params.set_int(sgn::CPT_M3UA_OPC, asp_node->to_int("m3ua opc", 0));
    m3ua_conn->params.set_int(sgn::CPT_M3UA_DPC, asp_node->to_int("m3ua dpc", 0));
    m3ua_conn->params.set_int(sgn::CPT_M3UA_HBEAT_TIMER, asp_node->to_int("m3ua hbeat", 10));

    // global
    m3ua_conn->params.set_bool(sgn::CPT_DATA_RETENTION, asp_node->to_int("dr", 0));

    // link to data processor
    sgnd->dprocessors_lock.lock();
    sgn::DataProcessor* dproc = sgnd->dprocessors.run()->item;
    sgnd->dprocessors_lock.unlock();
    m3ua_conn->params.set_pointer(sgn::CPT_DATA_PROCESSOR, dproc);
    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Linking M3UA ASP [%s] with data processor [%d]", m3ua_conn->get_label(), dproc->id);
    // create logic
    m3ua_conn->set_logic(new sgn::DefaultConnectionLogic<sgn::M3uaPayload>(m3ua_conn));

    // add to list
    sgnd->asp_map[asp_node->name] = m3ua_conn;

    // link with AS
    if(as != NULL) update_link(m3ua_conn, as, conn_active);

}

void ASPConfigMod::update_link(sgn::M3UAConnection* m3ua_conn, sgn::ApplicationServer* as, bool conn_active){
    // sanity check
    if(as == NULL || m3ua_conn == NULL) return;
    // set ASP link in AS
    as->asp_map[m3ua_conn->get_label()] = m3ua_conn;
    // set AS link in ASP
    m3ua_conn->as.set(as);
    // log
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
			       "Setting up ASP [%s] link connection for AS [%s]...",
			       m3ua_conn->get_label(), as->get_label());

    // start if active
    if(conn_active){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting ASP [%s]...", m3ua_conn->get_label());
	// setup round robin for LOADSHARE
	//as->get_wrr()->add_item(m3ua_conn, m3ua_conn->get_label(), 1);
	// start
	m3ua_conn->start();
	// get data processor
	sgn::DataProcessor* dproc = (sgn::DataProcessor*)(void*)*m3ua_conn->params.get_param(sgn::CPT_DATA_PROCESSOR);
	// add to dproc conn list
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Adding ASP [%s] to data processor [%d]", m3ua_conn->get_label(), dproc->id);
	if(m3ua_conn->get_direction() == sgn::CD_CLIENT){
	    dproc->client_conns.update_start();
	    m3ua_conn->refc_inc();
	    (*dproc->client_conns).push_back(m3ua_conn);
	    dproc->client_conns.update_end();

	}else{
	    dproc->server_conns.update_start();
	    m3ua_conn->refc_inc();
	    (*dproc->server_conns).push_back(m3ua_conn);
	    dproc->server_conns.update_end();

	}

    }
}


void ASPConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* asp_root = cfg->children[0];
    // get root
    asp_root = asp_root->find_parent("asp");
    // sanity check
    if(asp_root == NULL) return;
    // tmp vars
    config::ConfigItem* asp_node = NULL;
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // lock
    sgnd->as_lock.lock();

    // loop asps
    for(unsigned int i = 0; i<asp_root->children.size(); i++){
	asp_node = asp_root->children[i];
	// rule deleted, continue
	if(asp_node->node_state == config::CONFIG_NS_DELETED){
	    del_conn(asp_node, NULL);

	// new asp connection
	}else if(asp_node->is_new){
	    add_conn(asp_node);

	// check for modifications
	}else if(asp_node->is_modified()){
	    // recreate connection
	    bool active = false;
	    sgn::ApplicationServer* as = del_conn(asp_node, &active);
	    add_conn(asp_node, as, active);

	}

    }

    // unlock
    sgnd->as_lock.unlock();

}

void PCAPInstanceConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* inst_root = cfg->children[0];
    // get root
    inst_root = inst_root->find_parent("instances");
    // sanity check
    if(inst_root == NULL) return;
    // tmp vars
    config::ConfigItem* instance = NULL;
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // lock
    sgnd->pcap_lock.lock();

    // loop asps
    for(unsigned int i = 0; i<inst_root->children.size(); i++){
	instance = inst_root->children[i];
	// instance deleted
	if(instance->node_state == config::CONFIG_NS_DELETED){
	    del_conn(instance);

	// new instance
	}else if(instance->is_new){
	    add_conn(instance);

	// check for modifications
	}else if(instance->is_modified()){
	    // not supported
	}

    }

    // unlock
    sgnd->pcap_lock.unlock();

}

int PCAPInstanceConfigMod::del_conn(config::ConfigItem* inst){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;
    // get conn
    std::map<std::string, sgn::PCAPConnection*>::iterator it = sgnd->pcap_map.find(inst->name);
    if(it == sgnd->pcap_map.end()){
	// ASP not found
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "Cannot find PCAP instance [%s]!",
				   inst->name.c_str());

	// err
	return 1;
    }
    // conn pointer
    sgn::PCAPConnection* pcap_conn = it->second;

    // remove from list
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Removing PCAP instance [%s]...",
			       inst->name.c_str());
    sgnd->pcap_map.erase(inst->name);

    // stop
    pcap_conn->stop();
    // remove stats (to avoid name clashes due do delayed deallocation)
    pcap_conn->terminate();
    // dec ref counter
    // pcap_conn pointer might not valid after call to dec()
    if(pcap_conn->refc_dec() == 0){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping PCAP instance [%s]...",
				   inst->name.c_str());

    }

    return 0;
}

void PCAPInstanceConfigMod::add_conn(config::ConfigItem* inst){
    SgndDescriptor* sgnd = (SgndDescriptor*)pmink::CURRENT_DAEMON;

    // log
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up PCAP instance [%s]...", inst->name.c_str());

    // sanity check
    std::map<std::string, sgn::PCAPConnection*>::iterator it = sgnd->pcap_map.find(inst->name);
    // err, already exists
    if(it != sgnd->pcap_map.end()){
	// ASP exists
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
				   "PCAP instance [%s] already exists!",
				   inst->name.c_str());
	// err
	return;
    }


    int errc = 0;
    int mtu = 0;
    int pool = 0;
    bool tcp_syn_only = true;
    int tcp_max_conns = 0;
    int tcp_max_segms = 0;
    int tcp_timeout = 0;

    // check for mandatory params
    if(!inst->val_exists("if"))
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
				   "Missing mandatory configuration item [pcap intances %s if]!",
				   inst->name.c_str(), errc++);


    // mtu
    mtu = inst->to_int("mtu", inst->parent->parent->to_int("mtu", 1024));
    if(mtu < 100 || mtu > 65535){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid MTU [%d] for PCAP instance [%s], valid range = [100 - 65535]!",
						       mtu, inst->name.c_str(), errc++);
    }
    // pool
    pool = inst->to_int("pool", inst->parent->parent->to_int("pool", 4096));
    if(pool < 10){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid pool size [%d] for PCAP instance [%s], valid range = [10 - 2147483647]!",
						       pool, inst->name.c_str(), errc++);

    }

    // tcp syn_only
    tcp_syn_only = inst->to_int("tcp syn_only", inst->parent->parent->to_int("tcp syn_only", 1));

    // tcp max_conns
    tcp_max_conns = inst->to_int("tcp max_conns", inst->parent->parent->to_int("tcp max_conns", 1000));
    if(tcp_max_conns < 1){
        pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid TCP max_conns [%d] for [pcap instance %s], valid range = [1 - 2147483647]!",
                                                       tcp_max_conns, inst->name.c_str(), errc++);
    }
    // tcp max_segms
    tcp_max_segms = inst->to_int("tcp max_segms", inst->parent->parent->to_int("tcp max_segms", 100));
    if(tcp_max_segms < 1){
        pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid TCP max_segms [%d] for [pcap instance %s], valid range = [1 - 2147483647]!",
                                                       tcp_max_segms, inst->name.c_str(), errc++);
    }
    // tcp timeout
    tcp_timeout = inst->to_int("tcp timeout", inst->parent->parent->to_int("tcp timeout", 300));
    if(tcp_timeout < 1 || tcp_timeout > 86400){
        pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid TCP timeout [%d] for [pcap instance %s], valid range = [1 - 86400]!",
                                                       tcp_timeout, inst->name.c_str(), errc++);
    }

    // check for error count
    if(errc > 0){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Cannot initialize PCAP instance [%s] due to incomplete configuration, [%d] errors found!",
				   inst->name.c_str(), errc);
    // init instance
    }else{
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up PCAP instance [%s], if = [%s]...",
				   inst->name.c_str(), inst->to_cstr("if"));

	sgn::PCAPConnection* pcap_conn = new sgn::PCAPConnection(sgn::CD_CLIENT,
								 sgn::SCT_ONE_ONE,
								 inst->name.c_str(),
								 mtu,
								 pool);

	// stats
	pcap_conn->params.set_pointer(sgn::CPT_R14P_STATS, sgnd->r14p_stats);

	// add stats handlers
	std::string stats_prefix("PCAP::");
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_RX_MSU")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_RX_MSU_COUNT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_MTU_LIMIT")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_MTU_LIMIT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_DISCARDED_MSU")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_DISCARDED_MSU));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_PLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_PLD_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_MALFORMED_MSU")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_MALFORMED_MSU));
        sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_TCP_CONNECTIONS")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_TCP_CONNS));
        sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_TCP_MAX_CONNECTIONS")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_TCP_MAX_CONNS));
        sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_TCP_INVALID_SEQ")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_TCP_INVALID_SEQ));
        sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_TCP_MAX_SEGMS")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_TCP_MAX_SEGMS));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_M3UA_MALFORMED_MSU")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_MALFORMED_MSU));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_M3UA_PLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_PLD_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_M3UA_PMPLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_PMPLD_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_M3UA_SMSG_SENT_COUNT")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_SMSG_SENT_COUNT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_M3UA_SMSG_SEND_ERROR")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_SMSG_SEND_ERROR));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_M3UA_SMSG_MSG_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_SMSG_MSG_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_M3UA_SMSG_PARAM_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_SMSG_PARAM_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_SMPP_MALFORMED_MSU")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_MALFORMED_MSU));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_SMPP_PLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_PLD_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_SMPP_PMPLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_PMPLD_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_SMPP_SMSG_SENT_COUNT")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_SMSG_SENT_COUNT));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_SMPP_SMSG_SEND_ERROR")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_SMSG_SEND_ERROR));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_SMPP_SMSG_MSG_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_SMSG_MSG_POOL_EMPTY));
	sgnd->r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + inst->name + "_SMPP_SMSG_PARAM_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_SMSG_PARAM_POOL_EMPTY));

	// pcap params
	pcap_conn->params.set_cstr(sgn::CPT_PCAP_INTERFACE, inst->to_cstr("if"));
	pcap_conn->params.set_int(sgn::CPT_PCAP_PCAP_BUFFER, inst->to_int("pcap_buffer", 16777216));
        pcap_conn->params.set_int(sgn::CPT_PCAP_TYPE, inst->to_int("pcap_type", sgn::PCAPConnection::PT_M3UA));

	// link to data processor
	sgnd->dprocessors_lock.lock();
	sgn::DataProcessor* dproc = sgnd->dprocessors.run()->item;
	sgnd->dprocessors_lock.unlock();
	pcap_conn->params.set_pointer(sgn::CPT_DATA_PROCESSOR, dproc);
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Linking PCAP instance [%s] with data processor [%d]", pcap_conn->get_label(), dproc->id);
	// create logic
	pcap_conn->set_logic(new sgn::DefaultConnectionLogic<sgn::PcapPayload>(pcap_conn));

        // tcp tracker options
        pmink_net::TcpTracker* tcp_tracker = pcap_conn->get_tcp_tracker();
        tcp_tracker->set_syn_only(tcp_syn_only);                
        tcp_tracker->set_max_conns(tcp_max_conns);
        tcp_tracker->set_max_segments(tcp_max_segms);
        tcp_tracker->set_timeout(tcp_timeout);

	// start
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting PCAP instance [%s]...", pcap_conn->get_label());
	if(pcap_conn->start() == 0){
	    // add to list
	    sgnd->pcap_map[inst->name] = pcap_conn;
	    // add conn to data processor conn list
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Adding PCAP instance [%s] to data processor [%d]", pcap_conn->get_label(), dproc->id);
	    dproc->pcap_conns.update_start();
	    pcap_conn->refc_inc();
	    (*dproc->pcap_conns).push_back(pcap_conn);
	    dproc->pcap_conns.update_end();

	}else{
	    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot start PCAP instance [%s]...", pcap_conn->get_label());
	    pcap_conn->refc_dec();

	}

    }
}

