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

#include <routing.h>
#include <getopt.h>


RoutingdDescriptor::RoutingdDescriptor(const char* _type, const char* _desc) : pmink::DaemonDescriptor(_type, NULL, _desc) {
	r14ps = NULL;
	hbeat = NULL;
	r14p_stats = NULL;
	cfgd_r14pc = NULL;
	config = new config::Config();
	bzero(cfgd_id, sizeof(cfgd_id));
	r14p_port = 0;

	// set daemon params
	set_param(0, config);

	// default extra param values
	// --r14p-streams
	extra_params.set_int(0, 1000);
	// --r14p-stimeout
	extra_params.set_int(1, 5);

}

RoutingdDescriptor::~RoutingdDescriptor(){
	// free routing deamons address strings
	for(unsigned int i = 0; i<config_daemons.size(); i++) delete config_daemons[i];

}

void RoutingdDescriptor::process_arguements(int argc, char** argv){
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");
	int opt;
	int option_index = 0;
	struct option long_options[] = {
	    {"r14p-streams", required_argument, 0, 0},
	    {"r14p-stimeout", required_argument, 0, 0},
	    {0, 0, 0, 0}
	};

	if(argc < 5){
		print_help();
		exit(EXIT_FAILURE);
		return;
	}else{
		while ((opt = getopt_long(argc, argv, "?p:c:i:D", long_options, &option_index)) != -1) {
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

				// config daemon address
				case 'c':
				    // check pattern (ipv4:port)
				    // check if valid
				    if(!boost::regex_match(optarg, addr_regex)){
					    std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
					    exit(EXIT_FAILURE);

				    }else{
					    config_daemons.push_back(new std::string(optarg));

				    }
				    break;

				// r14p port
				case 'p':
				    r14p_port = atoi(optarg);
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

		// port
		if(r14p_port == 0){
		    std::cout << "ERROR: R14P IN port not defined!" << std::endl;
		    exit(EXIT_FAILURE);
		}


	}

}

void RoutingdDescriptor::print_help(){
	std::cout << daemon_type << " - " << daemon_description << std::endl;
	std::cout << "Copyright (c) 2012 Release14.org" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << " -?\thelp" << std::endl;
	std::cout << " -i\tunique daemon id" << std::endl;
	std::cout << " -c\tconfig daemon address (ipv4:port)" << std::endl;
	std::cout << " -p\tR14P inbound port" << std::endl;
	std::cout << " -D\tstart in debug mode" << std::endl;
	std::cout << std::endl;
	std::cout << "R14P Options:" << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << " --r14p-streams\t\tR14P Session stream pool\t\t(default = 1000)" << std::endl;
	std::cout << " --r14p-stimeout\tR14P Stream timeout in seconds\t\t(default = 5)" << std::endl;

}

void RoutingdDescriptor::init(){
    // init r14p
    init_r14p();
    // init config
    if(init_config() != 0){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Cannot find any valid config daemon connection, using automatic configuration...");
	// not exiting since routingd is allowed to run without configd connection
    }

    // accept connections (server mode)
    r14ps->start_server(NULL, r14p_port);

    // connect stats with routing
    r14p::R14PClient* r14pc = r14p_stats->get_r14p_session()->connect("127.0.0.1", r14p_port, 16, NULL, 0);
    if(r14pc != NULL) r14p_stats->setup_client(r14pc);

}

void RoutingdDescriptor::process_config(){
    // create root node string
    std::string root_node_str(DAEMON_CFG_NODE);
    root_node_str.append(" ");
    root_node_str.append(daemon_id);

    // get node
    config::ConfigItem* root = (*config->get_definition_root())(root_node_str.c_str());
    config::ConfigItem* tmp_node = NULL;

    // check if configuration exists
    if(root == NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Configuration for node [%s] does not exist, using automatic routing...", daemon_id);
	return;
    }

    // process configuration
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Configuration for node [%s] successfully received, processing...", daemon_id);

    // asp list
    if((*root)("destinations") == NULL) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing destination configuration node set for node [%s]!", daemon_id);
    else{
	tmp_node = (*root)("destinations");
	// setup config on chage events
	tmp_node->set_on_change_handler(&wrr_mod_handler, true);
	// check all nodes
	config::ConfigItem* dest_node_type = NULL;
	for(unsigned int i = 0; i<tmp_node->children.size(); i++){
	    dest_node_type = tmp_node->children[i];
	    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Processing configuration for destination type [%s] for node [%s]...",
							  dest_node_type->name.c_str(), daemon_id);

	    if((*dest_node_type)("nodes") == NULL) {
		pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing destination [%s] nodes configuration node set for node [%s]!",
								dest_node_type->name.c_str(), daemon_id);
		continue;
	    }

	    // nodes
	    config::ConfigItem* nodes = (*dest_node_type)("nodes");

	    // process nodes
	    config::ConfigItem* dest_node = NULL;
	    pmink_utils::PooledVPMap<uint32_t> tmp_params;
	    for(unsigned int j = 0; j<nodes->children.size(); j++){
		dest_node = nodes->children[j];
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Adding node [%s] to [%s] routing table with weight [%d]...",
							      dest_node->name.c_str(),
							      dest_node_type->name.c_str(),
							      dest_node->to_int("weight"),
							      daemon_id);

		// set weight data
		tmp_params.set_int(0, dest_node->to_int("weight", 1));
		// add to routing handler
		r14ps->get_routing_handler()->add_node(NULL, dest_node_type->name.c_str(), dest_node->name.c_str(), &tmp_params);

	    }

	}
    }
}

int RoutingdDescriptor::init_config(bool _process_config){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting config daemon registration procedure...");
	// loop routing daemons

	for(unsigned int i = 0; i<r14ps->get_client_count(); i++){
		// get client
		r14p::R14PClient* r14p_client = r14ps->get_client(i);
		// null check
		if(r14p_client != NULL && r14p_client->is_registered()){
		    // check only OUTBOUND
		    if(r14p_client->direction != r14p::R14P_CD_OUTBOUND) continue;
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


void RoutingdDescriptor::init_r14p(){
	// start R14P session
	r14ps = r14p::init_session(get_daemon_type(),
				   get_daemon_id(),
				   (int)*extra_params.get_param(0),
				   (int)*extra_params.get_param(1),
				   true,
				   (int)*extra_params.get_param(1));

	// set routing algorighm
	r14ps->set_routing_algo(r14p::R14P_RA_WRR);
	// set r14ps pointer
	wrr_mod_handler.r14ps = r14ps;

	// r14p stats
	r14p_stats = new r14p::R14PStatsSession(5, r14ps);
	// start stats
	r14p_stats->start();

	// init r14p stats
	r14p_stats->init_r14p_session_stats(r14ps);

	// set params
	set_param(2, r14ps);


	// connect to config daemons
	boost::smatch regex_groups;
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");


	// loop routing daemons
	for(unsigned int i = 0; i<config_daemons.size(); i++){
	    // separate IP and PORT
	    boost::regex_search(*config_daemons[i], regex_groups, addr_regex);
	    // connect to config daemon
	    r14ps->connect(regex_groups[1].str().c_str(), atoi(regex_groups[2].str().c_str()), 16, NULL, 0);
	}


}

void RoutingdDescriptor::terminate(){
	// stop server
	r14ps->stop_server();
	// stop stats
	r14p_stats->stop();
	// destroy session, free memory
	r14p::destroy_session(r14ps);
	// deallocate config memory
	if(config->get_definition_root() != NULL) delete config->get_definition_root();
	// free config
	delete config;
	// r14p stats
	delete r14p_stats;

}


