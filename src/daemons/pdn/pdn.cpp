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

#include <pdn.h>
#include <getopt.h>
#include <pdn_stats.h>
#include <boost/algorithm/string.hpp>

// PdndDescriptor
PdndDescriptor::PdndDescriptor(const char* _type, const char* _desc): pmink::DaemonDescriptor(_type, NULL, _desc),
								      r14ps(NULL),
								      r14p_stats(NULL),
								      r14psmm(NULL),
								      cfgd_r14pc(NULL),
								      hbeat(NULL),
								      non_srvc_hdnlr(NULL),
								      rt_r14pc(NULL){

	config = new config::Config();
	bzero(cfgd_id, sizeof(cfgd_id));

	// default extra param values
	// --r14p-streams
	extra_params.set_int(0, 1000);
	// --r14p-stimeout
	extra_params.set_int(1, 5);
	// --r14p-smsg-pool
	extra_params.set_int(2, 1000);
	// --r14p-sparam-pool
	extra_params.set_int(3, 5000);


	// stats
	/*
	stats.register_item(fgn::ST_UNSUPPORTED_R14P_SERVICE);
	stats.register_item(fgn::ST_SMSG_SEND_ERROR);
	stats.register_item(fgn::ST_SMSG_MSG_POOL_EMPTY);
	stats.register_item(fgn::ST_SMSG_PARAM_POOL_EMPTY);
	stats.register_item(fgn::ST_SMSG_INCOMPLETE);
	stats.register_item(fgn::ST_SMSG_RX_COUNT);
	stats.register_item(fgn::ST_FWORKER_NA);
	stats.register_item(fgn::ST_FGN_PAYLOAD_POOL_EMPTY);
	stats.register_item(fgn::ST_RPROC_POOL_EMPTY);
	stats.register_item(fgn::ST_TCAP_MULTIPLE_COMPONENTS);
    */



}

PdndDescriptor::~PdndDescriptor(){
	// service message manager
	delete r14psmm;
	// r14p stats
	delete r14p_stats;

}

void PdndDescriptor::process_arguements(int argc, char** argv){
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");
	int opt;
	int option_index = 0;
	struct option long_options[] = {
	    {"r14p-streams", required_argument, 0, 0},
	    {"r14p-stimeout", required_argument, 0, 0},
	    {"r14p-smsg-pool", required_argument, 0, 0},
	    {"r14p-sparam-pool", required_argument, 0, 0},
	    {0, 0, 0, 0}
	};


	if(argc < 5){
		print_help();
		exit(EXIT_FAILURE);
		return;
	}else{
		while ((opt = getopt_long(argc, argv, "?r:i:t:c:D", long_options, &option_index)) != -1) {
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


				// routing daemon address
				case 'r':
				    // check pattern (ipv4:port)
				    // check if valid
				    if(!boost::regex_match(optarg, addr_regex)){
					    std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
					    exit(EXIT_FAILURE);

				    }else{
					    routing_daemons.push_back(std::string(optarg));

				    }
				    break;

				// config daemon address
				case 'c':
				    // check pattern (ipv4:port)
				    // check if valid
				    if(!boost::regex_match(optarg, addr_regex)){
					    std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
					    exit(EXIT_FAILURE);

				    }else{
					    config_daemons.push_back(std::string(optarg));

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

		// use routiers is not using separate config daemon connection
		if(config_daemons.size() == 0) config_daemons = routing_daemons;

		// check for config daemons
		if(config_daemons.size() == 0){
			std::cout << "ERROR: Routing daemon(s) not defined!" << std::endl;
			exit(EXIT_FAILURE);

		}


	}

}

void PdndDescriptor::print_help(){
	std::cout << daemon_type << " - " << daemon_description << std::endl;
	std::cout << "Copyright (c) 2012 Release14.org" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << " -?\thelp" << std::endl;
	std::cout << " -i\tunique daemon id" << std::endl;
	std::cout << " -t\tdaemon type override" << std::endl;
	std::cout << " -r\trouting daemon address (ipv4:port)" << std::endl;
	std::cout << " -c\tconfig daemon address (ipv4:port)" << std::endl;
	std::cout << " -D\tstart in debug mode" << std::endl;
	std::cout << std::endl;
	std::cout << "R14P Options:" << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << " --r14p-streams\t\tR14P Session stream pool\t\t(default = 1000)" << std::endl;
	std::cout << " --r14p-stimeout\tR14P Stream timeout in seconds\t\t(default = 5)" << std::endl;
	std::cout << " --r14p-smsg-pool\tR14P Service message pool\t\t(default = 1000)" << std::endl;
	std::cout << " --r14p-sparam-pool\tR14P Service message parameter pool\t(default = 5000)" << std::endl;


}

void PdndDescriptor::init_r14p(){
	// handler for non service message messages
	non_srvc_hdnlr = config::create_cfg_event_handler(config);
	// service message manager
	r14psmm = new r14p::ServiceMsgManager(&idt_map,
					      NULL,
					      non_srvc_hdnlr,
					      (int)*extra_params.get_param(2),
					      (int)*extra_params.get_param(3));

	// set daemon params (searched for in dr::NewSrvcMsg constructor)
	set_param(0, config);
	set_param(1, r14psmm);

	// set service message handlers
	r14psmm->set_new_msg_handler(&new_srvc_msg);
	r14psmm->set_msg_err_handler(&new_srvc_msg.msg_err);

	// start R14P session
	r14ps = r14p::init_session(get_daemon_type(),
				   get_daemon_id(),
				   (int)*extra_params.get_param(0),
				   (int)*extra_params.get_param(1),
				   false,
				   (int)*extra_params.get_param(1));
	set_param(2, r14ps);

	// r14p stats
	r14p_stats = new r14p::R14PStatsSession(5, r14ps);
	set_param(3, r14p_stats);

	// start stats
	r14p_stats->start();

	// init r14p stats
	r14p_stats->init_r14p_session_stats(r14ps);

	// init stats
	/*
	r14p_stats->add_trap(r14p::TrapId("UNSUPPORTED_R14P_SERVICE"), new fgn::StatsHandler(&stats, fgn::ST_UNSUPPORTED_R14P_SERVICE));
	r14p_stats->add_trap(r14p::TrapId("SMSG_SEND_ERROR"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_SEND_ERROR));
	r14p_stats->add_trap(r14p::TrapId("SMSG_MSG_POOL_EMPTY"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_MSG_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("SMSG_PARAM_POOL_EMPTY"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_PARAM_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("SST_SMSG_INCOMPLETE"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_INCOMPLETE));
	r14p_stats->add_trap(r14p::TrapId("SMSG_RX_COUNT"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_RX_COUNT));
	r14p_stats->add_trap(r14p::TrapId("FWORKER_NA"), new fgn::StatsHandler(&stats, fgn::ST_FWORKER_NA));
	r14p_stats->add_trap(r14p::TrapId("FGN_PAYLOAD_POOL_EMPTY"), new fgn::StatsHandler(&stats, fgn::ST_FGN_PAYLOAD_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("RPROC_POOL_EMPTY"), new fgn::StatsHandler(&stats, fgn::ST_RPROC_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("TCAP_MULTIPLE_COMPONENTS"), new fgn::StatsHandler(&stats, fgn::ST_TCAP_MULTIPLE_COMPONENTS));
	*/

	// connect to routing daemons
	boost::smatch regex_groups;
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");

	// loop config daemons
	for(unsigned int i = 0; i<config_daemons.size(); i++){
	    // separate IP and PORT
	    boost::regex_search(config_daemons[i], regex_groups, addr_regex);
	    // connect to routing daemon
	    r14p::R14PClient* r14pc = r14ps->connect(regex_groups[1].str().c_str(),
						     atoi(regex_groups[2].str().c_str()),
						     16,
						     NULL,
						     0);

	    // setup client for service messages
	    if(r14pc != NULL) {
		// setup service message event handlers
		r14psmm->setup_client(r14pc);

	    }
	}


	// loop routing daemons
	for(unsigned int i = 0; i<routing_daemons.size(); i++){
	    // separate IP and PORT
	    boost::regex_search(routing_daemons[i], regex_groups, addr_regex);
	    // connect to routing daemon
	    r14p::R14PClient* r14pc = r14ps->connect(regex_groups[1].str().c_str(),
						     atoi(regex_groups[2].str().c_str()),
						     16,
						     NULL,
						     0);

	    // setup client for service messages
	    if(r14pc != NULL) {
		// setup service message event handlers
		r14psmm->setup_client(r14pc);

	    }

	    // connect stats with routing
	    r14pc = r14p_stats->get_r14p_session()->connect(regex_groups[1].str().c_str(), atoi(regex_groups[2].str().c_str()), 16, NULL, 0);
	    if(r14pc != NULL) r14p_stats->setup_client(r14pc);


	}


	// set rt client
	rt_r14pc = r14ps->get_registered_client("routingd");

}

int PdndDescriptor::init_config(bool _process_config){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting config daemon registration procedure...");
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
					if(config::notification_request(config, r14p_client, DAEMON_CFG_NODE, NULL, (char*)cfgd_id, &cfgd_uid, NULL) == 0){
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
						pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Registering notification request for node path [%s] with config daemon [%s]", DAEMON_CFG_NODE, cfgd_id);
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

void PdndDescriptor::init(){
    // pmdebug
    #ifdef PMDEBUG
    std::map<uint32_t, std::string>* debug_labels = pmink_utils::create_debug_label_map();
    set_param(9999, debug_labels);
    #endif

    // init r14p
    init_r14p();
    // init config
    if(init_config() != 0){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot find any valid config daemon connection, terminating...");
	exit(EXIT_FAILURE);
    }
}


void PdndDescriptor::process_config(){
    // create root node string
    std::string root_node_str(DAEMON_CFG_NODE);
    root_node_str.append(" ");
    root_node_str.append(daemon_id);

    // get node
    config::ConfigItem* root = (*config->get_definition_root())(root_node_str.c_str());

    // check if configuration exists
    if(root == NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Configuration root node [%s] does not exist!", daemon_id);
	// todo for testing, do not exit
	//exit(EXIT_FAILURE);
    }

    // process configuration
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Configuration for node [%s] successfully received, verifying...", daemon_id);


    // configuration ok, process
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Configuration for node [%s] successfully verified, starting...", daemon_id);

}


void PdndDescriptor::terminate(){
	// destroy session, free memory
	r14p::destroy_session(r14ps);
	// deallocate config memory
	if(config->get_definition_root() != NULL) delete config->get_definition_root();
	// free config
	delete config;
	// remove stats
	/*
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("UNSUPPORTED_R14P_SERVICE")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_SEND_ERROR")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_MSG_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_PARAM_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_INCOMPLETE")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_RX_COUNT")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("FWORKER_NA")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("FGN_PAYLOAD_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("RPROC_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("TCAP_MULTIPLE_COMPONENTS")));
	*/

	#ifdef PMDEBUG
	delete (std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999);
	#endif

}
