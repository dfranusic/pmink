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

#include <sgn.h>
#include <sgn_stats.h>
#include <getopt.h>


SgndDescriptor::SgndDescriptor(const char* _type, const char* _desc) : pmink::DaemonDescriptor(_type, NULL, _desc) {
	r14ps = NULL;
	hbeat = NULL;
	rt_r14pc = NULL;
	config = new config::Config();
	bzero(cfgd_id, sizeof(cfgd_id));

	// set daemon params
	set_param(0, config);

	// default extra param values
	// --r14p-streams
	extra_params.set_int(0, 10000);
	// --r14p-stimeout
	extra_params.set_int(1, 5);
	// --r14p-smsg-pool
	extra_params.set_int(2, 1000);
	// --r14p-sparam-pool
	extra_params.set_int(3, 100000);
        // --dev-m3ua-sm
	extra_params.set_int(4, 1);
        // --dev-smpp-sm
	extra_params.set_int(5, 1);

	// stats
	stats.register_item(sgn::SST_AS_NOT_FOUND);
	stats.register_item(sgn::SST_NO_VALID_ROUTES);
	stats.register_item(sgn::SST_GET_NEXT_ROUTE_ERROR);
	stats.register_item(sgn::SST_UNSUPPORTED_R14P_SERVICE);
	stats.register_item(sgn::SST_SMSG_SEND_ERROR);
	stats.register_item(sgn::SST_SMSG_MSG_POOL_EMPTY);
	stats.register_item(sgn::SST_SMSG_PARAM_POOL_EMPTY);
	stats.register_item(sgn::SST_SMSG_INCOMPLETE);

	// local ports default (32768 - 61000)
	local_ports.set_range(32768, 61000);
}


SgndDescriptor::~SgndDescriptor(){
	// m3ua ASPs
	for(std::map<std::string, sgn::M3UAConnection*>::iterator it = asp_map.begin(); it != asp_map.end(); it++){
	    // conn pointer
	    sgn::M3UAConnection* m3ua_conn = it->second;
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping M3UA ASP [%s]...", m3ua_conn->get_label());
	    // stop
	    m3ua_conn->stop();
	    // remove stats (to avoid name clashes due do delayed deallocation)
	    m3ua_conn->terminate();
	    // dec ref counter
	    m3ua_conn->refc_dec();
	}
	// smpp ASPs
	for(std::map<std::string, sgn::SMPPConnection*>::iterator it = smpp_asp_map.begin(); it != smpp_asp_map.end(); it++){
	    // conn pointer
	    sgn::SMPPConnection* smpp_conn = it->second;
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping SMPP ASP [%s]...", smpp_conn->get_label());
	    // stop children
	    smpp_conn->stop_children();
	    // stop
	    smpp_conn->stop();
	    // remove stats (to avoid name clashes due do delayed deallocation)
	    smpp_conn->terminate();
	    // dec ref counter
	    smpp_conn->refc_dec();
	}
	// pcap
	for(std::map<std::string, sgn::PCAPConnection*>::iterator it = pcap_map.begin(); it != pcap_map.end(); it++){
	    // conn pointer
	    sgn::PCAPConnection* pcap_conn = it->second;
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping PCAP instance [%s]...", pcap_conn->get_label());
	    // stop
	    pcap_conn->stop();
	    // remove stats (to avoid name clashes due do delayed deallocation)
	    pcap_conn->terminate();
	    // dec ref counter
	    pcap_conn->refc_dec();
	}

	// sleep a bit, give data processors time to exec logic handlers
	sleep(2);
	// data processors
	for(pmink_utils::WRR<sgn::DataProcessor*>::items_map_it_t it = dprocessors.begin(); it != dprocessors.end(); it++){
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Stopping data processor [%d]...", (*it).item->id);
	    delete (*it).item;
	}
	// m3ua ASs
	for(std::map<std::string, sgn::ApplicationServer*>::iterator it = as_map.begin(); it != as_map.end(); it++){
	    delete it->second;
	}
	// smpp ASs
	for(std::map<std::string, sgn::SMPP_ApplicationServer*>::iterator it = smpp_as_map.begin(); it != smpp_as_map.end(); it++){
	    delete it->second;
	}
	// free routing deamons address strings
	for(unsigned int i = 0; i<routing_daemons.size(); i++) delete routing_daemons[i];
	// non service msg handler
	delete non_srvc_hdnlr;
	// wait for ref thread to finish
	while(pmink_utils::RefCounted::get_item_count() > 0) sleep(1);
	// r14p stats
	delete r14p_stats;

}


void SgndDescriptor::process_arguements(int argc, char** argv){
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");
	int opt;
	int option_index = 0;
	struct option long_options[] = {
	    {"r14p-streams", required_argument, 0, 0},
	    {"r14p-stimeout", required_argument, 0, 0},
	    {"r14p-smsg-pool", required_argument, 0, 0},
	    {"r14p-sparam-pool", required_argument, 0, 0},
	    {"dev-m3ua-sm", required_argument, 0, 0},
	    {"dev-smpp-sm", required_argument, 0, 0},
	    {0, 0, 0, 0}
	};

	if(argc < 5){
		print_help();
		exit(EXIT_FAILURE);
		return;
	}else{
		while ((opt = getopt_long(argc, argv, "?r:i:t:D", long_options, &option_index)) != -1) {
			switch(opt){
				// long options
				case 0:
				   if(long_options[option_index].flag != 0) break;
				   switch(option_index){
				       // r14p-streams
				       case 0:
					   extra_params.set_int(0, atoi(optarg));
					   break;

				       // r14p-stimeout
				       case 1:
					   extra_params.set_int(1, atoi(optarg));
					   break;

				       // r14p-smsg-pool
				       case 2:
					   extra_params.set_int(2, atoi(optarg));
					   break;

				       // r14p-sparam-pool
				       case 3:
					   extra_params.set_int(3, atoi(optarg));
					   break;

				       // dev-m3ua-sm
				       case 4:
					   extra_params.set_int(4, atoi(optarg));
					   break;

				       // dev-smpp-sm
				       case 5:
					   extra_params.set_int(5, atoi(optarg));
					   break;

				   }
				   break;

				// help
				case '?':
					print_help();
					exit(EXIT_FAILURE);


				// daemon id
				case 'i':
				    if(set_daemon_id(optarg) > 0){
					std::cout << "ERROR: Maximum size of daemon id string is 15 characters!" << std::endl;
					exit(EXIT_FAILURE);

				    }
				    break;

				// daemon type
				case 't':
				    if(set_daemon_type(optarg) > 0){
					std::cout << "ERROR: Maximum size of daemon type string is 15 characters!" << std::endl;
					exit(EXIT_FAILURE);

				    }
				    break;


				// routing and config daemon address
				// 2 connections with each routing daemon (one for config and one for everything else)
				case 'r':
				    // check pattern (ipv4:port)
				    // check if valid
				    if(!boost::regex_match(optarg, addr_regex)){
					    std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
					    exit(EXIT_FAILURE);

				    }else{
					    routing_daemons.push_back(new std::string(optarg));
					    //cfg_daemons.push_back(new std::string(optarg));

				    }
				    break;

				// debug mode
				case 'D':
				    set_log_level(pmink::LLT_DEBUG);
				    break;

			}
		}

		// check mandatory id
		if(strlen(get_daemon_id()) == 0){
			std::cout << "ERROR: Daemon id not defined!" << std::endl;
			exit(EXIT_FAILURE);

		}


	}

}

void SgndDescriptor::print_help(){
	std::cout << daemon_type << " - " << daemon_description << std::endl;
	std::cout << "Copyright (c) 2012 Release14.org" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << " -?\thelp" << std::endl;
	std::cout << " -i\tunique daemon id" << std::endl;
	std::cout << " -t\tdaemon type override" << std::endl;
	std::cout << " -r\trouting daemon address (ipv4:port)" << std::endl;
	std::cout << " -D\tstart in debug mode" << std::endl;
	std::cout << std::endl;
	std::cout << "R14P Options:" << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << " --r14p-streams\t\tR14P Session stream pool\t\t(default = 10000)" << std::endl;
	std::cout << " --r14p-stimeout\tR14P Stream timeout in seconds\t\t(default = 5)" << std::endl;
	std::cout << " --r14p-smsg-pool\tR14P Service message pool\t\t(default = 1000)" << std::endl;
	std::cout << " --r14p-sparam-pool\tR14P Service message parameter pool\t(default = 100000)" << std::endl;
	std::cout << std::endl;
	std::cout << "Dev Options:" << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << " --dev-m3ua-sm\t\tEnable/Disable m3ua state machine\t(0 - Disable, 1 - Enable)" << std::endl;
	std::cout << " --dev-smpp-sm\t\tEnable/Disable smpp state machine\t(0 - Disable, 1 - Enable)" << std::endl;

}

lockfree::QItem<packet_descriptor, 0>* SgndDescriptor::pd_alloc(void* pool_owner){
    sgn::_PCAPConnection<sgn::PcapPayload>* pconn = (sgn::_PCAPConnection<sgn::PcapPayload>*)pool_owner;
    sgn::PcapPayload* pld_item = NULL;
    // * allocate and return
    // * pmpcap expects packet_descriptor pointer (pd)
    if(pconn->get_pld_pool()->pop(&pld_item) == 0) return &pld_item->pd;
    // pool empty, inc counter
    pconn->stats.inc(sgn::CST_PLD_POOL_EMPTY, 1);
    // default return
    return NULL;
}

int SgndDescriptor::pd_dealloc(lockfree::QItem<packet_descriptor, 0>* pd, void* pool_owner){
    // * this method will never get called, pmpcap calls pd_dealloc when sd->lfq is full
    // * in this particular case lfq size = pool size, lfq can never get full
    // * pconn->pld_pool is a SPSC pool, push/pop should be done by two threads (no more),
    //   in this case pop is called by pmpcap capture thread and push is called
    //   by pconn data processor's dproc thread
    // * calling pd_dealloc from both pmpcap capture thread and pconn data processor's
    //   dproc thread would cause undefined behavior (push called by two separate threads)
    return 0;
    // ********************************************************************************************
    // **** the following code can be used with other pools (using locks), not with SPSC pools ****
    // ********************************************************************************************
    //sgn::_PCAPConnection<sgn::PcapPayload>* pconn = (sgn::_PCAPConnection<sgn::PcapPayload>*)pool_owner;
    // pd->data.args points to PcapPayload (set in _PcapConnection constructor)
    //return pconn->get_pld_pool()->push(1, (sgn::PcapPayload*)pd->data.args);
    // ********************************************************************************************
}

int SgndDescriptor::pd_start(void* pool_owner){
    sgn::_PCAPConnection<sgn::PcapPayload>* pconn = (sgn::_PCAPConnection<sgn::PcapPayload>*)pool_owner;
    pconn->refc_inc();
    return 0;
}

int SgndDescriptor::pd_finish(void* pool_owner){
    sgn::_PCAPConnection<sgn::PcapPayload>* pconn = (sgn::_PCAPConnection<sgn::PcapPayload>*)pool_owner;
    pconn->refc_dec();
    return 0;
}


void* SgndDescriptor::loop_p_expire(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    SgndDescriptor* dd = (SgndDescriptor*)ti->user_args;
    int total_sleep = 0;
    int ts = dd->loop_cmap.get_timeout();
    // loop
    while(ti->is_active()){
	// sleep 1 sec
	sleep(1);
	// inc total sleep
	++total_sleep;
	// check if hbeat interval reached
	if(total_sleep < ts) continue;
	// reset current timeout
	total_sleep = 0;
	// expire
	dd->loop_cmap.lock();
	dd->loop_cmap.expire();
	dd->loop_cmap.unlock();
    }

    return NULL;
}

void SgndDescriptor::process_config(){
    // create root node string
    std::string root_node_str(DAEMON_CFG_NODE);
    root_node_str.append(" ");
    root_node_str.append(daemon_id);

    // get node
    config::ConfigItem* root = (*config->get_definition_root())(root_node_str.c_str());
    config::ConfigItem* tmp_node = NULL;

    // check if configuration exists
    if(root == NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Configuration for node [%s] does not exist!", daemon_id);
	exit(EXIT_FAILURE);
    }

    // process configuration
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Configuration for node [%s] successfully received, processing...", daemon_id);

    int errc = 0;
    int mtu = 0;
    int pool = 0;
    bool tcp_syn_only = true;
    int tcp_max_conns = 0;
    int tcp_max_segms = 0;
    int tcp_timeout = 0;

    // local ports
    if(root->val_exists("local_ports")) {
	unsigned int tmp_range[2];
	if(sscanf(root->to_cstr("local_ports"), "%5u-%5u", &tmp_range[0], &tmp_range[1]) == 2){
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Setting local port range for CLIENT connections to [%d - %d]", tmp_range[0], tmp_range[1]);
	    local_ports.set_range(tmp_range[0], tmp_range[1]);
	}
    }


    // loop protection
    if(root->to_bool("loop_protection")) {
	loop_cmap.set_timeout((root->to_int("lp_list_ttl", 5) <= 0 ? 1 : root->to_int("lp_list_ttl", 5)));
	set_param(4, &loop_cmap);
	thm.new_thread(&loop_p_expire, "loop_p_exire", this);
    }

    // data processors setup
    int dprocs = root->to_int("dprocessors", thm.get_core_count());
    if(dprocs <= 0){
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot create data processors...");
	exit(EXIT_FAILURE);
    }
    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Setting up [%d] data processors, CPU cores detected = [%d]", dprocs, thm.get_core_count());
    char tmp_str[16];
    for(int i = 0; i<dprocs; i++){
	sgn::DataProcessor* dproc = new sgn::DataProcessor(i);
	sprintf(tmp_str, "dproc_%d", i);
	dprocessors.add_item(dproc, tmp_str, 1);
    }
    // data processors start
    pmink_utils::WRR<sgn::DataProcessor*>::items_map_it_t it = dprocessors.begin();
    for(; it != dprocessors.end(); it++){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting data processor [%d]...", (*it).item->id);
	(*it).item->sync_client_fds(*(*it).item->client_conns);
	(*it).item->sync_server_fds(*(*it).item->server_conns);
	(*it).item->start();
    }

    // pcap instances
    if((*root)("pcap instances") == NULL) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing PCAP instance configuration node set for node [%s]!", daemon_id);
    else{
	tmp_node = (*root)("pcap instances");
	// setup config on change events
	tmp_node->set_on_change_handler(&pcap_inst_mod_handler, true);
	// check if empty
	if(tmp_node->children.size() == 0) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Instances configuration node set is empty for node [%s]!", daemon_id);
	// check all instances
	config::ConfigItem* pcap_inst = NULL;
	for(unsigned int i = 0; i<tmp_node->children.size(); i++){
	    errc = 0;
	    pcap_inst = tmp_node->children[i];
	    // check for mandatory params
	    if(!pcap_inst->val_exists("if"))
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
					   "Missing mandatory configuration item [pcap intances %s if] for node [%s]!",
					   pcap_inst->name.c_str(), daemon_id, errc++);


	    // mtu
	    mtu = pcap_inst->to_int("mtu", ((*root)("pcap"))->to_int("mtu", 1024));
	    if(mtu < 100 || mtu > 65535){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid MTU [%d] for [pcap instance %s], valid range = [100 - 65535]!",
							       mtu, pcap_inst->name.c_str(), errc++);
	    }
	    // pool
	    pool = pcap_inst->to_int("pool", ((*root)("pcap"))->to_int("pool", 4096));
	    if(pool < 10){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid pool size [%d] for [pcap instance %s], valid range = [10 - 2147483647]!",
							       pool, pcap_inst->name.c_str(), errc++);

	    }
            // tcp syn_only
            tcp_syn_only = pcap_inst->to_int("tcp syn_only", ((*root)("pcap"))->to_int("tcp syn_only", 1));

            // tcp max_conns
    	    tcp_max_conns = pcap_inst->to_int("tcp max_conns", ((*root)("pcap"))->to_int("tcp max_conns", 1000));
	    if(tcp_max_conns < 1){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid TCP max_conns [%d] for [pcap instance %s], valid range = [1 - 2147483647]!",
							       tcp_max_conns, pcap_inst->name.c_str(), errc++);
	    }
            // tcp max_segms
    	    tcp_max_segms = pcap_inst->to_int("tcp max_segms", ((*root)("pcap"))->to_int("tcp max_segms", 100));
	    if(tcp_max_segms < 1){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid TCP max_segms [%d] for [pcap instance %s], valid range = [1 - 2147483647]!",
							       tcp_max_segms, pcap_inst->name.c_str(), errc++);
	    }
            // tcp timeout
    	    tcp_timeout = pcap_inst->to_int("tcp timeout", ((*root)("pcap"))->to_int("tcp timeout", 300));
	    if(tcp_timeout < 1 || tcp_timeout > 86400){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid TCP timeout [%d] for [pcap instance %s], valid range = [1 - 86400]!",
							       tcp_timeout, pcap_inst->name.c_str(), errc++);
	    }

	    // check for error count
	    if(errc > 0){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Cannot initialize pcap instance [%s] due to incomplete configuration, [%d] errors found!",
					   pcap_inst->name.c_str(), errc);
		continue;

	    // init instance
	    }else{
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up PCAP instance [%s], if = [%s]...",
					   pcap_inst->name.c_str(), pcap_inst->to_cstr("if"));

		sgn::PCAPConnection* pcap_conn = new sgn::PCAPConnection(sgn::CD_CLIENT,
									 sgn::SCT_ONE_ONE,
									 pcap_inst->name.c_str(),
									 mtu,
									 pool);

		// stats
		pcap_conn->params.set_pointer(sgn::CPT_R14P_STATS, r14p_stats);

		// add stats handlers
		std::string stats_prefix("PCAP::");
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_RX_MSU")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_RX_MSU_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_MTU_LIMIT")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_MTU_LIMIT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_DISCARDED_MSU")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_DISCARDED_MSU));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_PLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_PLD_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_MALFORMED_MSU")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_MALFORMED_MSU));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_TCP_CONNECTIONS")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_TCP_CONNS));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_TCP_MAX_CONNECTIONS")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_TCP_MAX_CONNS));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_TCP_INVALID_SEQ")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_TCP_INVALID_SEQ));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_TCP_MAX_SEGMS")), new sgn::SgnStatsHandler(&pcap_conn->stats, sgn::CST_TCP_MAX_SEGMS));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_M3UA_MALFORMED_MSU")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_MALFORMED_MSU));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_M3UA_PLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_PLD_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_M3UA_PMPLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_PMPLD_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_M3UA_SMSG_SENT_COUNT")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_SMSG_SENT_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_M3UA_SMSG_SEND_ERROR")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_SMSG_SEND_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_M3UA_SMSG_MSG_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_SMSG_MSG_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_M3UA_SMSG_PARAM_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_m3ua_conn()->stats, sgn::CST_SMSG_PARAM_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_SMPP_MALFORMED_MSU")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_MALFORMED_MSU));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_SMPP_PLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_PLD_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_SMPP_PMPLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_PMPLD_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_SMPP_SMSG_SENT_COUNT")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_SMSG_SENT_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_SMPP_SMSG_SEND_ERROR")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_SMSG_SEND_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_SMPP_SMSG_MSG_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_SMSG_MSG_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + pcap_inst->name + "_SMPP_SMSG_PARAM_POOL_EMPTY")), new sgn::SgnStatsHandler(&pcap_conn->get_smpp_conn()->stats, sgn::CST_SMSG_PARAM_POOL_EMPTY));



		// pcap params
		pcap_conn->params.set_cstr(sgn::CPT_PCAP_INTERFACE, pcap_inst->to_cstr("if"));
		pcap_conn->params.set_int(sgn::CPT_PCAP_PCAP_BUFFER, pcap_inst->to_int("pcap_buffer", 16777216));
		pcap_conn->params.set_int(sgn::CPT_PCAP_TYPE, pcap_inst->to_int("pcap_type", sgn::PCAPConnection::PT_M3UA));

		// link to data processor
		sgn::DataProcessor* dproc = dprocessors.run()->item;
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
		    pcap_map[pcap_inst->name] = pcap_conn;
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
    }



    // asp list
    if((*root)("m3ua asp") == NULL) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing M3UA ASP configuration node set for node [%s]!", daemon_id);
    else{
	tmp_node = (*root)("m3ua asp");
	// setup config on change events
	tmp_node->set_on_change_handler(&asp_mod_handler, true);
	// check if empty
	if(tmp_node->children.size() == 0) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "M3UA ASP configuration node set is empty for node [%s]!", daemon_id);
	// check all nodes
	config::ConfigItem* asp_node = NULL;
	for(unsigned int i = 0; i<tmp_node->children.size(); i++){
	    errc = 0;
	    asp_node = tmp_node->children[i];

	    // check for mandatory m3ua params
	    if(!asp_node->val_exists("m3ua opc")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									     "Missing mandatory configuration item [m3ua asp %s m3ua opc] for node [%s]!",
									     asp_node->name.c_str(), daemon_id, errc++);


	    if(asp_node->to_int("mode") == sgn::CD_CLIENT){
		// check for mandatory sctp params
		if(!asp_node->val_exists("sctp remote ip1")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
										        "Missing mandatory configuration item [m3ua asp %s sctp remote ip1] for node [%s]!",
										        asp_node->name.c_str(), daemon_id, errc++);

		if(!asp_node->val_exists("sctp remote port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
										         "Missing mandatory configuration item [m3ua asp %s sctp remote port] for node [%s]!",
										         asp_node->name.c_str(), daemon_id, errc++);

	    }else if(asp_node->to_int("mode") == sgn::CD_SERVER){
		// check for mandatory sctp params
		if(!asp_node->val_exists("sctp local port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
										        "Missing mandatory configuration item [m3ua asp %s sctp local port] for node [%s]!",
										        asp_node->name.c_str(), daemon_id, errc++);

	    }
	    // mtu
	    mtu = asp_node->to_int("mtu", ((*root)("m3ua"))->to_int("mtu", 1024));
	    if(mtu < 100 || mtu > 65535){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid MTU [%d] for [m3ua asp %s], valid range = [100 - 65535]!",
							       mtu, asp_node->name.c_str(), errc++);
	    }
	    // pool
	    pool = asp_node->to_int("pool", ((*root)("m3ua"))->to_int("pool", 4096));
	    if(pool < 10){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid pool size [%d] for [m3ua asp %s], valid range = [10 - 2147483647]!",
							       pool, asp_node->name.c_str(), errc++);

	    }

	    // err check
	    if(errc > 0){
		// ASP exists
		pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
					   "Cannot add M3UA ASP [%s], missing mandatory configuration items!",
					   asp_node->name.c_str());


	    // init connection
	    }else if(errc == 0){

		// new connection
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up M3UA ASP [%s], MTU = [%d], pool size = [%d]...",
							     asp_node->name.c_str(), mtu, pool);

		sgn::M3UAConnection* m3ua_conn = new sgn::M3UAConnection((sgn::ConnectionDirection)asp_node->to_int("mode", 0),
									 sgn::SCT_ONE_ONE,
									 asp_node->name.c_str(),
									 mtu,
									 pool);
		// stats
		m3ua_conn->params.set_pointer(sgn::CPT_R14P_STATS, r14p_stats);

		// local port range
		m3ua_conn->params.set_pointer(sgn::CPT_LOCAL_PORT_RANGE, &local_ports);

		// event handlers
		m3ua_conn->get_e_handler()->set_handler(sgn::CET_CONNECTION_UP, &as_wrr_add_handler);
		m3ua_conn->get_e_handler()->set_handler(sgn::CET_CONNECTION_DOWN, &as_wrr_remove_handler);


		// add stats handlers
		std::string stats_prefix("M3UA::");
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_MSU")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_RX_MSU_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_MSU")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_TX_MSU_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_MTU_LIMIT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_MTU_LIMIT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_DISCARDED_MSU")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_DISCARDED_MSU));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_PLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_PLD_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_MALFORMED_MSU")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_MALFORMED_MSU));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_RX_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_TX_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_PMPLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_PMPLD_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_SEND_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SMSG_SEND_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_MSG_POOL_EMPTY")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SMSG_MSG_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_PARAM_POOL_EMPTY")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SMSG_PARAM_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_M3UA_ACTIVE")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_M3UA_ACTIVE));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SCTP_ACTIVE")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SCTP_ACTIVE));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_SENT_COUNT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_SMSG_SENT_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_NO_VALID_ROUTES")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_NO_VALID_ROUTES));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_RX_HBEAT_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_TX_HBEAT_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_CMD_PROCESS_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_CMD_PROCESS_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_ENCODE_ERROR")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_ENCODE_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RECONNECT")), new sgn::SgnStatsHandler(&m3ua_conn->stats, sgn::CST_RECONNECT));


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
		m3ua_conn->params.set_bool(sgn::CPT_DEV_M3UA_SM, ((int)*extra_params.get_param(4) > 0 ? true : false));

		// link to data processor
		sgn::DataProcessor* dproc = dprocessors.run()->item;
		m3ua_conn->params.set_pointer(sgn::CPT_DATA_PROCESSOR, dproc);
		pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Linking M3UA ASP [%s] with data processor [%d]", m3ua_conn->get_label(), dproc->id);
		// create logic
		m3ua_conn->set_logic(new sgn::DefaultConnectionLogic<sgn::M3uaPayload>(m3ua_conn));

		// add to list
		asp_map[asp_node->name] = m3ua_conn;

	    }
	}
    }

    // as list
    if((*root)("m3ua as") == NULL) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing M3UA AS configuration node set for node [%s]!", daemon_id);
    else{
	tmp_node = (*root)("m3ua as");
	// setup config on change events
	tmp_node->set_on_change_handler(&as_mod_handler, true);
	// check if empty
	if(tmp_node->children.size() == 0) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "M3UA AS configuration node set is empty for node [%s]!", daemon_id);
	// check all AS nodes
	config::ConfigItem* as_node = NULL;
	config::ConfigItem* asp_nodes = NULL;
	config::ConfigItem* asp_node = NULL;
	sgn::ApplicationServer* as = NULL;
	std::map<std::string, sgn::M3UAConnection*>::iterator it;

	// loop ASs
	for(unsigned int i = 0; i<tmp_node->children.size(); i++){
	    errc = 0;
	    as_node = tmp_node->children[i];

	    // new as
	    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up M3UA AS [%s]...", as_node->name.c_str());

	    // add AS to list
	    as = new sgn::ApplicationServer(as_node->name.c_str());
	    as_map[as_node->name] = as;

	    // stats
	    as->params.set_pointer(sgn::CPT_R14P_STATS, r14p_stats);
	    // add stats handlers
	    std::string stats_prefix("M3UA::");
	    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + as_node->name + "_ASP_NOT_FOUND")), new sgn::SgnStatsHandler(&as->stats, sgn::ASST_ASP_NOT_FOUND));

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
	    asp_nodes = (*as_node)("asp");
	    if(asp_nodes == NULL){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing M3UA ASP links for AS [%s]!", as_node->name.c_str());
		continue;
	    }
	    // setup config on change events
	    asp_nodes->set_on_change_handler(&as_asp_mod_handler, true);

	    // loop ASP
	    for(unsigned int j = 0; j<asp_nodes->children.size(); j++){
		asp_node = asp_nodes->children[j];
		// new as
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
					   "Setting up M3UA ASP [%s] link connection for AS [%s]...",
					   asp_node->name.c_str(), as_node->name.c_str());

		// find ASP connection
		it = asp_map.find(asp_node->name);
		if(it == asp_map.end()){
		    // ASP connection not found
		    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
					       "M3UA ASP [%s] does not exist, cannot link with AS [%s]!",
					       asp_node->name.c_str(), as_node->name.c_str());

		    continue;
		}

		// ASP linked with AS
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
					   "M3UA ASP [%s] successfully linked with AS [%s]",
					   asp_node->name.c_str(), as_node->name.c_str());

		// link ASP with AS
		as->asp_map[it->first] = it->second;
		// set AS param in ASP
		it->second->as.set(as);
		// start ASP if active
		if(asp_node->to_int("active", 0) == 1){
		    // start
		    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting M3UA ASP [%s]...", it->first.c_str());
		    it->second->start();
		    // add conn to data processor conn list
		    sgn::DataProcessor* dproc = (sgn::DataProcessor*)(void*)*it->second->params.get_param(sgn::CPT_DATA_PROCESSOR);
		    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Adding M3UA ASP [%s] to data processor [%d]", it->first.c_str(), dproc->id);
		    if(it->second->get_direction() == sgn::CD_CLIENT){
			dproc->client_conns.update_start();
			it->second->refc_inc();
			(*dproc->client_conns).push_back(it->second);
			dproc->client_conns.update_end();

		    }else{
			dproc->server_conns.update_start();
			it->second->refc_inc();
			(*dproc->server_conns).push_back(it->second);
			dproc->server_conns.update_end();
		    }

		}

	    }

	}

    }

    // smpp asp list
    if((*root)("smpp asp") == NULL) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing SMPP ASP configuration node set for node [%s]!", daemon_id);
    else{
	tmp_node = (*root)("smpp asp");
	// setup config on change events
	tmp_node->set_on_change_handler(&smpp_asp_mod_handler, true);
	// check if empty
	if(tmp_node->children.size() == 0) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "SMPP ASP configuration node set is empty for node [%s]!", daemon_id);
	// check all nodes
	config::ConfigItem* asp_node = NULL;
	config::ConfigItem* smpp_users = NULL;
	config::ConfigItem* smpp_user = NULL;
	for(unsigned int i = 0; i<tmp_node->children.size(); i++){
	    errc = 0;
	    asp_node = tmp_node->children[i];

	    // check for mandatory smpp params
	    if(!asp_node->val_exists("smpp bind_method")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
										     "Missing mandatory configuration item [smpp asp %s smpp bind_method] for node [%s]!",
										     asp_node->name.c_str(), daemon_id, errc++);

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
										        "Missing mandatory configuration item [smpp asp %s tcp remote ip] for node [%s]!",
										        asp_node->name.c_str(), daemon_id, errc++);

		if(!asp_node->val_exists("tcp remote port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
										         "Missing mandatory configuration item [smpp asp %s tcp remote port] for node [%s]!",
										         asp_node->name.c_str(), daemon_id, errc++);

	    }else if(asp_node->to_int("mode") == sgn::CD_SERVER){
		// check for mandatory tcp params
		if(!asp_node->val_exists("tcp local port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
										        "Missing mandatory configuration item [smpp asp %s tcp local port] for node [%s]!",
										        asp_node->name.c_str(), daemon_id, errc++);

	    }
	    // mtu
	    mtu = asp_node->to_int("mtu", ((*root)("smpp"))->to_int("mtu", 1024));
	    if(mtu < 100 || mtu > 65535){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid MTU [%d] for [smpp asp %s], valid range = [100 - 65535]!",
							       mtu, asp_node->name.c_str(), errc++);
	    }
	    // pool
	    pool = asp_node->to_int("pool", ((*root)("smpp"))->to_int("pool", 4096));
	    if(pool < 10){
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Invalid pool size [%d] for [smpp asp %s], valid range = [10 - 2147483647]!",
							       pool, asp_node->name.c_str(), errc++);

	    }

	    if(errc > 0){
		// SMPP connections contains errors
		pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
					   "Cannot add SMPP ASP [%s], missing mandatory configuration items!",
					   asp_node->name.c_str());


	    // init connection
	    }else if(errc == 0){
		// new connection
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up SMPP ASP [%s], MTU = [%d], pool size = [%d]...",
							     asp_node->name.c_str(), mtu, pool);

		sgn::SMPPConnection* smpp_conn = new sgn::SMPPConnection((sgn::ConnectionDirection)asp_node->to_int("mode", 0),
									 sgn::SCT_ONE_MANY,
									 asp_node->name.c_str(),
									 mtu,
									 pool);



		// if SERVER mode set max conn count to 2 for main master conn (2 or more)
		if(smpp_conn->get_direction() == sgn::CD_SERVER) smpp_conn->params.set_int(sgn::CPT_MAX_CONN_COUNT, 2);

		// stats
		smpp_conn->params.set_pointer(sgn::CPT_R14P_STATS, r14p_stats);

		// local port range
		smpp_conn->params.set_pointer(sgn::CPT_LOCAL_PORT_RANGE, &local_ports);

		// event handlers
		smpp_conn->get_e_handler()->set_handler(sgn::CET_CONNECTION_UP, &smpp_wrr_add_handler);
		smpp_conn->get_e_handler()->set_handler(sgn::CET_CONNECTION_DOWN, &smpp_wrr_remove_handler);

		// add stats handlers
		std::string stats_prefix("SMPP::");
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_MSU")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_RX_MSU_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_MSU")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_TX_MSU_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_MTU_LIMIT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_MTU_LIMIT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_DISCARDED_MSU")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_DISCARDED_MSU));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_PLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_PLD_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_MALFORMED_MSU")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_MALFORMED_MSU));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_RX_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_TX_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_PMPLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_PMPLD_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_SEND_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMSG_SEND_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_MSG_POOL_EMPTY")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMSG_MSG_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_PARAM_POOL_EMPTY")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMSG_PARAM_POOL_EMPTY));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMPP_ACTIVE")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMPP_ACTIVE));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TCP_ACTIVE")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_TCP_ACTIVE));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_SMSG_SENT_COUNT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_SMSG_SENT_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_NO_VALID_ROUTES")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_NO_VALID_ROUTES));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_RX_HBEAT_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_TX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_TX_HBEAT_COUNT));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_CMD_PROCESS_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_CMD_PROCESS_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_ENCODE_ERROR")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_ENCODE_ERROR));
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + asp_node->name + "_RECONNECT")), new sgn::SgnStatsHandler(&smpp_conn->stats, sgn::CST_RECONNECT));

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

		/*
		// use first user from the list
		smpp_user = smpp_users->children[0];
		smpp_conn->params.set_cstr(sgn::CPT_SMPP_BIND_SYSTEM_ID, smpp_user->name.c_str());
		smpp_conn->params.set_cstr(sgn::CPT_SMPP_BIND_SYSTEM_TYPE, smpp_user->to_cstr("system_type"));
		smpp_conn->params.set_cstr(sgn::CPT_SMPP_BIND_PASSWORD, smpp_user->to_cstr("password"));
		*/
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
		smpp_conn->params.set_bool(sgn::CPT_DEV_SMPP_SM, ((int)*extra_params.get_param(5) > 0 ? true : false));

		// link to data processor
		sgn::DataProcessor* dproc = dprocessors.run()->item;
		smpp_conn->params.set_pointer(sgn::CPT_DATA_PROCESSOR, dproc);
		pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Linking SMPP ASP [%s] with data processor [%d]", smpp_conn->get_label(), dproc->id);
		// create logic
		smpp_conn->set_logic(new sgn::DefaultConnectionLogic<sgn::SmppPayload>(smpp_conn));

		// add to list
		smpp_asp_map[asp_node->name] = smpp_conn;

	    }

	}
	// smpp as list
	if((*root)("smpp as") == NULL) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing SMPP AS configuration node set for node [%s]!", daemon_id);
	else{
	    tmp_node = (*root)("smpp as");
	    // setup config on change events
	    tmp_node->set_on_change_handler(&smpp_as_mod_handler, true);
	    // check if empty
	    if(tmp_node->children.size() == 0) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "SMPP AS configuration node set is empty for node [%s]!", daemon_id);
	    // check all AS nodes
	    config::ConfigItem* as_node = NULL;
	    config::ConfigItem* asp_nodes = NULL;
	    config::ConfigItem* asp_node = NULL;
	    sgn::SMPP_ApplicationServer* as = NULL;
	    std::map<std::string, sgn::SMPPConnection*>::iterator it;

	    // loop ASs
	    for(unsigned int i = 0; i<tmp_node->children.size(); i++){
		errc = 0;
		as_node = tmp_node->children[i];

		// new as
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up SMPP AS [%s]...", as_node->name.c_str());

		// add AS to list
		as = new sgn::SMPP_ApplicationServer(as_node->name.c_str());
		smpp_as_map[as_node->name] = as;

		// stats
		as->params.set_pointer(sgn::CPT_R14P_STATS, r14p_stats);
		// add stats handlers
		std::string stats_prefix("SMPP::");
		r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + as_node->name + "_ASP_NOT_FOUND")), new sgn::SgnStatsHandler(&as->stats, sgn::ASST_ASP_NOT_FOUND));

		// check ASP connections for current AS
		asp_nodes = (*as_node)("asp");
		if(asp_nodes == NULL){
		    pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing SMPP ASP links for SMPP AS [%s]!", as_node->name.c_str());
		    continue;
		}
		// setup config on change events
		asp_nodes->set_on_change_handler(&smpp_as_asp_mod_handler, true);

		// loop ASP
		for(unsigned int j = 0; j<asp_nodes->children.size(); j++){
		    asp_node = asp_nodes->children[j];
		    // new as
		    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
					       "Setting up SMPP ASP [%s] link connection for SMPP AS [%s]...",
					       asp_node->name.c_str(), as_node->name.c_str());

		    // find ASP connection
		    it = smpp_asp_map.find(asp_node->name);
		    if(it == smpp_asp_map.end()){
			// ASP connection not found
			pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
						   "SMPP ASP [%s] does not exist, cannot link with SMPP AS [%s]!",
						   asp_node->name.c_str(), as_node->name.c_str());

			continue;
		    }

		    // ASP linked with AS
		    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
					       "SMPP ASP [%s] successfully linked with SMPP AS [%s]",
					       asp_node->name.c_str(), as_node->name.c_str());

		    // link ASP with AS
		    as->asp_map[it->first] = it->second;
		    // set AS param in ASP
		    it->second->as.set(as);

		    // add to WRR if server connection (one to many connection)
		    if(it->second->get_direction() == sgn::CD_SERVER) as->get_wrr()->add_item(it->second, it->first.c_str(), 1);

		    // start ASP if active
		    if(asp_node->to_int("active", 0) == 1){
			// start
			pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Starting SMPP ASP [%s]...", it->first.c_str());
			it->second->start();
			// add conn to data processor conn list
			sgn::DataProcessor* dproc = (sgn::DataProcessor*)(void*)*it->second->params.get_param(sgn::CPT_DATA_PROCESSOR);
			pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Adding SMPP ASP [%s] to data processor [%d]", it->first.c_str(), dproc->id);
			if(it->second->get_direction() == sgn::CD_CLIENT){
			    dproc->client_conns.update_start();
			    it->second->refc_inc();
			    (*dproc->client_conns).push_back(it->second);
			    dproc->client_conns.update_end();

			}else{
			    dproc->server_conns.update_start();
			    it->second->refc_inc();
			    (*dproc->server_conns).push_back(it->second);
			    dproc->server_conns.update_end();
			}
		    }

		}

	    }


	}
    }
}

sgn::ApplicationServer* SgndDescriptor::get_as(const char* label){
    std::map<std::string, sgn::ApplicationServer*>::iterator it = as_map.find(std::string(label));
    if(it != as_map.end()) return it->second;
    return NULL;
}

sgn::SMPP_ApplicationServer* SgndDescriptor::get_smpp_as(const char* label){
    std::map<std::string, sgn::SMPP_ApplicationServer*>::iterator it = smpp_as_map.find(std::string(label));
    if(it != smpp_as_map.end()) return it->second;
    return NULL;
}


void SgndDescriptor::init(){
    // pmdebug
    #ifdef PMDEBUG
    std::map<uint32_t, std::string>* debug_labels = pmink_utils::create_debug_label_map();
    set_param(9999, debug_labels);
    #endif
    // pcap alloc/dealloc methods
    set_param(5, (void*)&pd_alloc);
    set_param(6, (void*)&pd_dealloc);
    // pcap start/finish methods
    set_param(7, (void*)&pd_start);
    set_param(8, (void*)&pd_finish);

    // init r14p
    init_r14p();
    // init config
    if(init_config() != 0){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot find any valid config daemon connection, terminating...");
	exit(EXIT_FAILURE);
    }
    /*
    while(init_config() != 0 && !DaemonDescriptor::DAEMON_TERMINATED){
	sleep(5);
    }
*/
}

int SgndDescriptor::init_config(bool _process_config){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting config daemon registration procedure...");

	// create root node string
	std::string root_node_str(DAEMON_CFG_NODE);
	root_node_str.append(" ");
	root_node_str.append(daemon_id);

	// loop routing daemons
	for(unsigned int i = 0; i<r14ps->get_client_count(); i++){
		// get client
		r14p::R14PClient* r14p_client = r14ps->get_client(i);
		// null check
		if(r14p_client != NULL && r14p_client->is_registered()){
			// log
			pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, 	"Connection to remote daemon established, L3 address = [%s:%d], R14P address = [%s:%s]",
									r14p_client->get_end_point_address(),
									r14p_client->get_end_point_port() ,
									r14p_client->get_end_point_daemon_type(),
									r14p_client->get_end_point_daemon_id());
			// check for active configd
			if(!cfgd_active.get()){
				// user login
				if(config::user_login(config, r14p_client, NULL, (char*)cfgd_id, &cfgd_uid) == 0){
				    if(strlen((char*)cfgd_id) > 0){
					// log
					pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "User [%s] successfully authenticated with config daemon [%s]", cfgd_uid.user_id, cfgd_id);
					// notification request
					if(config::notification_request(config, r14p_client, root_node_str.c_str(), NULL, (char*)cfgd_id, &cfgd_uid, NULL) == 0){

						// create hbeat events
						HbeatRecv* hb_recv = new HbeatRecv();
						HbeatMissed* hb_missed = new HbeatMissed(&cfgd_active);
						HbeatCleanup* hb_cleanup = new HbeatCleanup(hb_recv, hb_missed);

						// init hbeat
						hbeat = r14p::init_heartbeat("config_daemon", (char*)cfgd_id, r14p_client, 5, hb_recv, hb_missed, hb_cleanup);
						if(hbeat != NULL){
							cfgd_active.comp_swap(false, true);
							// log
							pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting R14P HBEAT for config daemon [%s], L3 address = [%s:%d]",
										   cfgd_id,
										   r14p_client->get_end_point_address(),
										   r14p_client->get_end_point_port());

						// free event memory on error
						}else{
						    delete hb_recv;
						    delete hb_missed;
						    delete hb_cleanup;
						}

						// log
						pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Registering notification request for node path [%s] with config daemon [%s]", root_node_str.c_str(), cfgd_id);
						// process config
						if(_process_config) process_config();

						// stop if config daemon connected
						// ok
						return 0;

					}else{
					    // log
					    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,	"Error while requesting notifications from config daemon [%s]!",
												cfgd_id);

					}

				    }else{
					    // log
					    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,	"Error while trying to find config daemon id via R14P connection, L3 address = [%s:%d], R14P address = [%s:%s]",
												r14p_client->get_end_point_address(),
												r14p_client->get_end_point_port() ,
												r14p_client->get_end_point_daemon_type(),
												r14p_client->get_end_point_daemon_id());

				    }

				}else{
					// log
					pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, 	"Error while trying to authenticate user [%s] with config daemon [%s:%d]!",
											cfgd_uid.user_id,
											r14p_client->get_end_point_address(),
											r14p_client->get_end_point_port());


				}

			}

		}else{
			// log
			pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Error while connecting to config daemon [%s:%d]!", r14p_client->get_end_point_address(), r14p_client->get_end_point_port());
		}


	}

	// err
	return 5;
}

void SgndDescriptor::init_r14p(){
	// handler for non service message messages
	non_srvc_hdnlr = config::create_cfg_event_handler(config);

	// service message manager
	r14psmm = new r14p::ServiceMsgManager(&idt_map,
					      NULL,
					      non_srvc_hdnlr,
					      (int)*extra_params.get_param(2),
					      (int)*extra_params.get_param(3));


	// start R14P session
	r14ps = r14p::init_session(get_daemon_type(),
				   get_daemon_id(),
				   (int)*extra_params.get_param(0),
				   (int)*extra_params.get_param(1),
				   false,
				   (int)*extra_params.get_param(1));




	// r14p stats
	r14p_stats = new r14p::R14PStatsSession(5, r14ps);
	// start stats
	r14p_stats->start();

	// init r14p stats
	r14p_stats->init_r14p_session_stats(r14ps);

	// init sgn stats
	r14p_stats->add_trap(r14p::TrapId("AS_NOT_FOUND"), new sgn::SgnStatsHandler(&stats, sgn::SST_AS_NOT_FOUND));
	r14p_stats->add_trap(r14p::TrapId("NO_VALID_ROUTES"), new sgn::SgnStatsHandler(&stats, sgn::SST_NO_VALID_ROUTES));
	r14p_stats->add_trap(r14p::TrapId("GET_NEXT_ROUTE_ERROR"), new sgn::SgnStatsHandler(&stats, sgn::SST_GET_NEXT_ROUTE_ERROR));
	r14p_stats->add_trap(r14p::TrapId("UNSUPPORTED_R14P_SERVICE"), new sgn::SgnStatsHandler(&stats, sgn::SST_UNSUPPORTED_R14P_SERVICE));
	r14p_stats->add_trap(r14p::TrapId("SMSG_SEND_ERROR"), new sgn::SgnStatsHandler(&stats, sgn::SST_SMSG_SEND_ERROR));
	r14p_stats->add_trap(r14p::TrapId("SMSG_MSG_POOL_EMPTY"), new sgn::SgnStatsHandler(&stats, sgn::SST_SMSG_MSG_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("SMSG_PARAM_POOL_EMPTY"), new sgn::SgnStatsHandler(&stats, sgn::SST_SMSG_PARAM_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("SST_SMSG_INCOMPLETE"), new sgn::SgnStatsHandler(&stats, sgn::SST_SMSG_INCOMPLETE));


	// set params
	set_param(1, r14psmm);
	set_param(2, r14ps);
	set_param(3, &new_srvc_msg.msg_done.async_done);

	// set service message handlers
	r14psmm->set_new_msg_handler(&new_srvc_msg);
	r14psmm->set_msg_err_handler(&new_srvc_msg.msg_err);


	// connect to routing daemons
	boost::smatch regex_groups;
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");

	// loop routing daemons
	for(unsigned int i = 0; i<routing_daemons.size(); i++){
	    // separate IP and PORT
	    boost::regex_search(*routing_daemons[i], regex_groups, addr_regex);
	    // connect to routing daemon
	    r14p::R14PClient* r14pc = r14ps->connect(regex_groups[1].str().c_str(), atoi(regex_groups[2].str().c_str()), 16, NULL, 0);

	    // setup client for service messages
	    if(r14pc != NULL) {
		r14psmm->setup_client(r14pc);
	    }

	    // connect stats with routing
	    r14pc = r14p_stats->get_r14p_session()->connect(regex_groups[1].str().c_str(), atoi(regex_groups[2].str().c_str()), 16, NULL, 0);
	    if(r14pc != NULL) r14p_stats->setup_client(r14pc);



	}

	// set rt client
	rt_r14pc = r14ps->get_registered_client("routingd");

}

void SgndDescriptor::terminate(){
	// destroy session, free memory
	r14p::destroy_session(r14ps);
	// free service message manager
	delete r14psmm;
	// deallocate config memory
	if(config->get_definition_root() != NULL) delete config->get_definition_root();
	// free config
	delete config;
	// remove stats
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("AS_NOT_FOUND")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("NO_VALID_ROUTES")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("GET_NEXT_ROUTE_ERROR")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("UNSUPPORTED_R14P_SERVICE")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_SEND_ERROR")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_MSG_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_PARAM_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SST_SMSG_INCOMPLETE")));

	#ifdef PMDEBUG
	delete (std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999);
	#endif

}

// DataProcessor
sgn::DataProcessor::DataProcessor(uint32_t _id): id(_id),
					         fds_max(4096),
					         fds_size_client(0),
					         fds_size_server(0){
    fds_lst_client = new pollfd[fds_max];
    fds_lst_server = new pollfd[fds_max];
}

sgn::DataProcessor::~DataProcessor(){
    // stop threads
    stop();
    // master lists refs
    dproc_conns_lst_t& server_conns_lst_m = *server_conns;
    dproc_conns_lst_t& client_conns_lst_m = *client_conns;

    // dec refc for all servers
    for(unsigned int i = 0; i<server_conns_lst_m.size(); i++) server_conns_lst_m[i]->refc_dec();
    // dec refc for all clients
    for(unsigned int i = 0; i<client_conns_lst_m.size(); i++) client_conns_lst_m[i]->refc_dec();

    // free poll lists
    delete[] fds_lst_client;
    delete[] fds_lst_server;
}

void sgn::DataProcessor::start(){
    // pthread attr
    pthread_attr_t pi_thread_attr;
    // scheduling parameters
    pthread_attr_init(&pi_thread_attr);
    // explicit FIFO scheduling
    pthread_attr_setinheritsched(&pi_thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&pi_thread_attr, SCHED_FIFO);
    // priority
    sched_param pi_sp;
    // max priority
    pi_sp.__sched_priority = 99;
    // set priorities
    pthread_attr_setschedparam(&pi_thread_attr, &pi_sp);
    // start
    thm.start();
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "dproc_%d_tx", id);
    thm.new_thread(&dproc_tx_loop, tmp, this, &pi_thread_attr);
    snprintf(tmp, sizeof(tmp), "dproc_%d_proc", id);
    thm.new_thread(&dproc_proc_loop, tmp, this, &pi_thread_attr);
    snprintf(tmp, sizeof(tmp), "dproc_%d_rx", id);
    thm.new_thread(&dproc_rx_loop, tmp, this, &pi_thread_attr);
    snprintf(tmp, sizeof(tmp), "dproc_%d_srv", id);
    thm.new_thread(&dproc_server_loop, tmp, this, &pi_thread_attr);
    snprintf(tmp, sizeof(tmp), "dproc_%d_rec", id);
    thm.new_thread(&dproc_reconnect_loop, tmp, this, &pi_thread_attr);

    // destroy attributes
    pthread_attr_destroy(&pi_thread_attr);
}

void sgn::DataProcessor::stop(){
    thm.stop();
}

int sgn::DataProcessor::add_conn(sgn::ConnectionBase* conn){
    return 0;
}
int sgn::DataProcessor::del_conn(const char* id){
    return 0;
}

int sgn::DataProcessor::sync_server_fds(dproc_conns_lst_t& lst){
    // check if resize needed
    if(lst.size() > fds_max){
	delete[] fds_lst_server;
	fds_lst_server = new pollfd[lst.size() * 2];
	fds_max = lst.size() * 2;

    }

    // get sockets from conns
    for(unsigned int i = 0; i<lst.size(); i++){
	fds_lst_server[i].fd = lst[i]->get_server_socket();
	fds_lst_server[i].events = POLLIN;
    }

    // fds size
    fds_size_server = lst.size();
    // ok
    return 0;
}

int sgn::DataProcessor::sync_client_fds(dproc_conns_lst_t& lst){
    // check if resize needed
    if(lst.size() > fds_max){
	delete[] fds_lst_client;
	fds_lst_client = new pollfd[lst.size() * 2];
	fds_max = lst.size() * 2;

    }

    // get sockets from conns
    for(unsigned int i = 0; i<lst.size(); i++){
	fds_lst_client[i].fd = lst[i]->get_client_socket();
	fds_lst_client[i].events = POLLIN;
    }

    // fds size
    fds_size_client = lst.size();

    // ok
    return 0;
}

int sgn::DataProcessor::sync(dproc_conns_t& conns){
    if(conns.can_sync()){
	// lst ref
	dproc_conns_lst_t& lst = *conns;
	// loop lst
	for(unsigned int i = 0; i<lst.size(); i++){
	    // dec refc
	    lst[i]->refc_dec();
	}
    }else return 1;

    if(conns.sync_locked()){
	// lst ref
	dproc_conns_lst_t& lst = *conns;
	// loop lst
	for(unsigned int i = 0; i<lst.size(); i++){
	    // inc refc
	    lst[i]->refc_inc();
	}

	conns.unlock();
    }

    return 0;
}


// data processor threads
void* sgn::DataProcessor::dproc_rx_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    DataProcessor* dproc = (DataProcessor*)ti->user_args;
    std::vector<sgn::ConnectionBase*>::iterator it;
    sgn::ConnectionBase* conn = NULL;
    int poll_timeout = 1000;
    int res;
    timespec ts = {0, 1000000};
    timespec ts2 = {1, 0};
    pmink_utils::MTMasterSlave<dproc_conns_lst_t, false> client_conns_l(&dproc->client_conns);
    dproc_conns_lst_t& client_conns_lst = *client_conns_l;
    bool idle = true;
    // rx params type
    struct rx_params_t{
	int poll_res;
	short int poll_revents;
	int* poll_socket;
    } rx_params;

    // loop
    while(ti->is_active()){
	// sync
	if(dproc->sync(client_conns_l) == 0) dproc->sync_client_fds(client_conns_lst);

	// poll sockets
	res = poll(dproc->fds_lst_client, dproc->fds_size_client, poll_timeout);
	// check for timeout
	if(res > 0){
	    idle = true;
	    // loop
	    for(unsigned int i = 0; i<dproc->fds_size_client; i++){
		// get conn
		conn = client_conns_lst[i];
		// set params
		rx_params.poll_res = 1;
		rx_params.poll_revents = dproc->fds_lst_client[i].revents;
		rx_params.poll_socket = &dproc->fds_lst_client[i].fd;
		// run rx logic
		if(conn->get_logic()->run(CLPT_RX, &rx_params) != 10) idle = false;
	    }
	    if(idle) nanosleep(&ts, NULL);

	}else{
	    // timeout
	    if(res == 0){
		// loop connections
		for(unsigned int i = 0; i<client_conns_lst.size(); i++){
		    // conn pointer
		    conn = client_conns_lst[i];
		    // set params
		    rx_params.poll_res = 0;
		    rx_params.poll_revents = dproc->fds_lst_client[i].revents;
		    rx_params.poll_socket = &dproc->fds_lst_client[i].fd;
		    // run rx logic with poll_res 0
		    conn->get_logic()->run(CLPT_RX, &rx_params);

		}
		nanosleep(&ts, NULL);

	    // error
	    }else{
		char tmp_buff[256];
		char* str_err = strerror_r(errno, tmp_buff, sizeof(tmp_buff));
		pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "DataProcessor [%d] rx poll error [%d]: %s", dproc->id, errno, str_err);
		// 1s sleep to avoid killing the syslog
		nanosleep(&ts2, NULL);
	    }
	}



    }
    // dec refc for all
    for(unsigned int i = 0; i<client_conns_lst.size(); i++) client_conns_lst[i]->refc_dec();

    return NULL;
}

void* sgn::DataProcessor::dproc_tx_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    DataProcessor* dproc = (DataProcessor*)ti->user_args;
    std::vector<sgn::ConnectionBase*>::iterator it;
    sgn::ConnectionBase* conn = NULL;
    timespec ts = {0, 1000000};
    bool idle = true;
    pmink_utils::MTMasterSlave<dproc_conns_lst_t, false> client_conns_l(&dproc->client_conns);
    dproc_conns_lst_t& client_conns_lst = *client_conns_l;

    // loop
    while(ti->is_active()){
	// sync
	dproc->sync(client_conns_l);

	idle = true;
	for(unsigned int i = 0; i<client_conns_lst.size(); i++){
	    // conn pointer
	    conn = client_conns_lst[i];
	    // run tx logic
	    if(conn->get_logic()->run(CLPT_TX) != 10) idle = false;
	}

	if(idle) {
	    nanosleep(&ts, NULL);
	}

    }
    // dec refc for all
    for(unsigned int i = 0; i<client_conns_lst.size(); i++) client_conns_lst[i]->refc_dec();

    return NULL;
}

void* sgn::DataProcessor::dproc_reconnect_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    DataProcessor* dproc = (DataProcessor*)ti->user_args;
    sgn::ConnectionBase* conn = NULL;
    timespec ts = {1, 0};
    pmink_utils::MTMasterSlave<dproc_conns_lst_t, false> client_conns_l(&dproc->client_conns);
    dproc_conns_lst_t& client_conns_lst = *client_conns_l;

    // loop
    while(ti->is_active()){
	// sync
	dproc->sync(client_conns_l);
	// loop conns
	for(unsigned int i = 0; i<client_conns_lst.size(); i++){
	    // conn pointer
	    conn = client_conns_lst[i];
	    // run reconnect logic
	    conn->get_logic()->run(CLPT_RECONNECT);
	}
	// always sleep for 1sec
	nanosleep(&ts, NULL);

    }

    // dec refc for all
    for(unsigned int i = 0; i<client_conns_lst.size(); i++) client_conns_lst[i]->refc_dec();

    return NULL;

}

void* sgn::DataProcessor::dproc_proc_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    DataProcessor* dproc = (DataProcessor*)ti->user_args;
    sgn::ConnectionBase* conn = NULL;
    timespec ts = {0, 1000000};
    bool idle = true;
    // client conns
    pmink_utils::MTMasterSlave<dproc_conns_lst_t, false> client_conns_l(&dproc->client_conns);
    dproc_conns_lst_t& client_conns_lst = *client_conns_l;
    dproc_conns_lst_t& client_conns_lst_m = *dproc->client_conns;
    // server conns
    dproc_conns_lst_t& server_conns_lst_m = *dproc->server_conns;
    // pcap conns
    pmink_utils::MTMasterSlave<dproc_conns_lst_t, false> pcap_conns_l(&dproc->pcap_conns);
    dproc_conns_lst_t& pcap_conns_lst = *pcap_conns_l;
    dproc_conns_lst_t& pcap_conns_lst_m = *dproc->pcap_conns;

    // loop
    while(ti->is_active()){
	// sync clients
	dproc->sync(client_conns_l);
	// loop conns
	idle = true;
	for(unsigned int i = 0; i<client_conns_lst.size(); i++){
	    // conn pointer
	    conn = client_conns_lst[i];
	    // check if connection is being freed (client threads stopped)
	    if(conn->is_garbage()) {
		//std::cout << "Conn: " << conn->get_label() << " shutting down..: " << conn->refc_get() << std::endl;
		dproc->client_conns.update_start();
		// erase-remove idiom (find conn pointer in master list and erase)
		client_conns_lst_m.erase(std::remove(client_conns_lst_m.begin(), client_conns_lst_m.end(), conn), client_conns_lst_m.end());
		conn->refc_dec();
		dproc->client_conns.update_end();
		// if 1-1 server connection, enable server
		if(conn->get_direction() == CD_SERVER && conn->get_server_type() == SCT_ONE_ONE){
		    dproc->server_conns.update_start();
		    conn->refc_inc(1); // master list ref inc
		    server_conns_lst_m.push_back(conn);
 		    dproc->server_conns.update_end();
		}
		// next conn
		continue;
	    }
	    // run proc logic
	    if(conn->get_logic()->run(CLPT_PROC) != 10) idle = false;
	    // run extra logic (usually keep alive msg handling like smpp enquire_link)
	    if(conn->get_logic()->run(CLPT_EXTRA) != 10) idle = false;

	}

	// sync pcap
	dproc->sync(pcap_conns_l);
	// loop conns
	for(unsigned int i = 0; i<pcap_conns_lst.size(); i++){
	    // conn pointer
	    conn = pcap_conns_lst[i];
	    // check if connection is being freed (client threads stopped)
	    if(conn->is_garbage()) {
		//std::cout << "Conn: " << conn->get_label() << " shutting down..: " << conn->refc_get() << std::endl;
		dproc->pcap_conns.update_start();
		// erase-remove idiom (find conn pointer in master list and erase)
		pcap_conns_lst_m.erase(std::remove(pcap_conns_lst_m.begin(), pcap_conns_lst_m.end(), conn), pcap_conns_lst_m.end());
		conn->refc_dec();
		dproc->pcap_conns.update_end();
		// next conn
		continue;
	    }
	    // run pcap logic
	    if(conn->get_logic()->run(CLPT_PCAP) != 10) idle = false;
	}

	if(idle) {
	    nanosleep(&ts, NULL);
	}

    }
    // dec refc for all
    for(unsigned int i = 0; i<client_conns_lst.size(); i++) client_conns_lst[i]->refc_dec();
    for(unsigned int i = 0; i<pcap_conns_lst.size(); i++) pcap_conns_lst[i]->refc_dec();

    return NULL;
}

void* sgn::DataProcessor::dproc_server_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    DataProcessor* dproc = (DataProcessor*)ti->user_args;
    sgn::ConnectionBase* conn = NULL;
    int poll_timeout = 1000;
    int res;
    timespec ts = {0, 1000000};
    timespec ts2 = {1, 0};
    pmink_utils::MTMasterSlave<dproc_conns_lst_t, false> server_conns_l(&dproc->server_conns);
    dproc_conns_lst_t& server_conns_lst = *server_conns_l;
    dproc_conns_lst_t& server_conns_lst_m = *dproc->server_conns;
    dproc_conns_lst_t& client_conns_lst_m = *dproc->client_conns;
    bool idle = true;

    // server params type
    struct srv_params_t{
	int poll_res;
	short int poll_revents;
	int* poll_socket;
	ConnectionBase* newconn;
    } srv_params;

    // loop
    while(ti->is_active()){
	// sync
	if(dproc->sync(server_conns_l) == 0) dproc->sync_server_fds(server_conns_lst);
	// poll sockets
	res = poll(dproc->fds_lst_server, dproc->fds_size_server, poll_timeout);
	// check for timeout
	if(res > 0){
	    idle = true;
	    // loop
	    for(unsigned int i = 0; i<dproc->fds_size_server; i++){
		// get conn
		conn = server_conns_lst[i];
		if(!conn->active.get()){
		    //std::cout << "Conn not active: " << conn->get_label() << std::endl;
		    dproc->server_conns.update_start();
		    // erase-remove idiom (find conn pointer in master list and erase)
		    server_conns_lst_m.erase(std::remove(server_conns_lst_m.begin(), server_conns_lst_m.end(), conn), server_conns_lst_m.end());
		    conn->refc_dec();
		    dproc->server_conns.update_end();
		    continue;
		}
		// set params
		srv_params.poll_res = 1;
		srv_params.poll_revents = dproc->fds_lst_server[i].revents;
		srv_params.poll_socket = &dproc->fds_lst_server[i].fd;
		srv_params.newconn = NULL;
		// run server logic
		int lres = conn->get_logic()->run(CLPT_SERVER, &srv_params);
		// check for socket error
		if(lres == -1){
		    char tmp_buff[256];
		    char* str_err = strerror_r(errno, tmp_buff, sizeof(tmp_buff));
		    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "DataProcessor [%d] server poll error [%d]: %s", dproc->id, errno, str_err);
		    // 1s sleep to avoid killing the syslog
		    nanosleep(&ts2, NULL);
		    continue;

		}
		// check if adding conn to data processor client list
		// 30 = 1-many server connection
		// 40 = 1-1 server connection
		if(lres == 30 || lres == 40){
		    idle = false;
		    //std::cout << "New conn: " << srv_params.newconn->get_label() << std::endl;
		    // add to master list
		    srv_params.newconn->refc_inc(1); // master list ref inc
		    dproc->client_conns.update_start();
		    client_conns_lst_m.push_back(srv_params.newconn);
		    dproc->client_conns.update_end();
		    // if 1-1 server connection, disable server
		    if(lres == 40){
			dproc->server_conns.update_start();
			// erase-remove idiom (find conn pointer in master list and erase)
			//std::cout << "Removing from server list: " <<  conn->get_label() << std::endl;
			server_conns_lst_m.erase(std::remove(server_conns_lst_m.begin(), server_conns_lst_m.end(), conn), server_conns_lst_m.end());
			conn->refc_dec();
			dproc->server_conns.update_end();
		    }
		}
	    }
	    if(idle) nanosleep(&ts, NULL);


	}else{
	    // timeout
	    if(res == 0){
		// loop connections
		for(unsigned int i = 0; i<server_conns_lst.size(); i++){
		    // conn pointer
		    conn = server_conns_lst[i];
		    if(!conn->active.get()){
			//std::cout << "Conn not active: " << conn->get_label() << std::endl;
			dproc->server_conns.update_start();
			// erase-remove idiom (find conn pointer in master list and erase)
			server_conns_lst_m.erase(std::remove(server_conns_lst_m.begin(), server_conns_lst_m.end(), conn), server_conns_lst_m.end());
			conn->refc_dec();
			dproc->server_conns.update_end();
			continue;
		    }
		    // set params
		    srv_params.poll_res = 0;
		    srv_params.poll_revents = dproc->fds_lst_server[i].revents;
		    srv_params.poll_socket = &dproc->fds_lst_server[i].fd;
		    srv_params.newconn = NULL;
		    // run rx logic with poll_res 0
		    conn->get_logic()->run(CLPT_SERVER, &srv_params);

		}
		nanosleep(&ts, NULL);

	    }else{
		char tmp_buff[256];
		char* str_err = strerror_r(errno, tmp_buff, sizeof(tmp_buff));
		pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "DataProcessor [%d] server poll error [%d]: %s", dproc->id, errno, str_err);
		// 1s sleep to avoid killing the syslog
		nanosleep(&ts2, NULL);

	    }
	}


    }

    // dec refc for all
    for(unsigned int i = 0; i<server_conns_lst.size(); i++) server_conns_lst[i]->refc_dec();

    return NULL;
}


