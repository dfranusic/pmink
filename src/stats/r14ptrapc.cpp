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

#include <r14p.h>
#include <r14p_stats.h>
#include <config.h>
#include <pmink_utils.h>
#include <antlr_utils.h>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <boost/regex.hpp>
#include <getopt.h>

void print_help(){
	cout << "r14ptrapc - pMINK R14P trap client" << endl;
	cout << "Copyright (c) 2012 Release14.org" << endl;
	cout << endl;
	cout << "Options:" << endl;
	cout << " -c\ttarget daemon address (ipv4:port)" << endl;
	cout << " -t\ttarget daemon type" << endl;
	cout << " -i\ttarget daemon id" << endl;
	cout << " -s\ttarget trap id (0 for ALL)" << endl;
	cout << " -a\tunique client id" << endl;
	std::cout << std::endl;
	std::cout << "R14P Options:" << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << " --r14p-streams\t\tR14P Session stream pool\t\t(default = 10)" << std::endl;
	std::cout << " --r14p-stimeout\tR14P Stream timeout in seconds\t\t(default = 5)" << std::endl;
	std::cout << " --r14p-smsg-pool\tR14P Service message pool\t\t(default = 10)" << std::endl;
	std::cout << " --r14p-sparam-pool\tR14P Service message parameter pool\t(default = 1000)" << std::endl;

}


class TrapVal {
public:
    TrapVal(): value(0){
	//bzero(desc, sizeof(desc));
    }
    //char desc[81];
    uint64_t value;
};


class AllDone: public r14p::R14PCallbackMethod {
public:
    AllDone(){
	sem_init(&signal, 0, 0);
    }
    ~AllDone(){
	sem_destroy(&signal);
    }

    // event handler method
    void run(r14p::R14PCallbackArgs* args){
	sem_post(&signal);

    }

    // signal
    sem_t signal;
};

class TrapDone: public r14p::R14PCallbackMethod {
public:

    // event handler method
    void run(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);

	// check for timeout error
	if(in_msg == NULL){
	    adone.run(args);
	    return;

	}

	// check for error
	if(in_msg->_header->_status != NULL){
	    if(in_msg->_header->_status->has_linked_data(*in_sess)){
		if(in_msg->_header->_status->linked_node->tlv->value[0] != 0){
		    adone.run(args);
		    return;
		}
	    }
	}
	// wait until stream complete was properly sent
	stream->set_callback(r14p::R14P_ET_PAYLOAD_SENT, &adone);

    }

    AllDone adone;
    std::map<std::string, TrapVal*>* traps;

};

class TrapNext: public r14p::R14PCallbackMethod {
public:
    // event handler method
    void run(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	char* tmp_val = NULL;
	int tmp_val_l = 0;
	std::string tmp_sid;
	uint64_t* tmp_svalp;
	uint64_t tmp_sval;
	TrapVal* trap_val;

	// check for body
	if(in_msg->_body != NULL){
	    // check for config message
	    if(in_msg->_body->_stats->has_linked_data(*in_sess)){
		// check for GET action
		if(in_msg->_body->_stats->_stats_action->linked_node->tlv->value[0] == asn1::StatsAction::_sa_result){
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
									//tmp_ivp = (uint32_t*)tmp_val;
									//tmp_sid = *tmp_ivp;
									tmp_sid.assign(tmp_val, tmp_val_l);
									break;

								    case asn1::ParameterType::_pt_pmink_stats_value:
									tmp_svalp = (uint64_t*)tmp_val;
									tmp_sval = be64toh(*tmp_svalp);
									trap_val = new TrapVal();
									trap_val->value = tmp_sval;
									//memcpy(trap_val->desc, tmp_val, tmp_val_l);
 									(*tdone->traps)[tmp_sid] = trap_val;

									break;
									/*
								    case asn1::ParameterType::_pt_pmink_stats_description:
									trap_val = new TrapVal();
									trap_val->value = tmp_sval;
									//memcpy(trap_val->desc, tmp_val, tmp_val_l);
 									(*tdone->traps)[tmp_sid] = trap_val;
									break;
								    */

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



	// continue
	stream->continue_sequence();
    }

    TrapDone* tdone;

};




int main(int argc, char** argv){
	int opt;
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");
	std::string dm_type;
	std::string dm_id;
	std::string dm_addr;
	std::string client_id;
	boost::smatch regex_groups;
	std::map<std::string, TrapVal*> traps;
	int option_index = 0;
	struct option long_options[] = {
	    {"r14p-streams", required_argument, 0, 0},
	    {"r14p-stimeout", required_argument, 0, 0},
	    {"r14p-smsg-pool", required_argument, 0, 0},
	    {"r14p-sparam-pool", required_argument, 0, 0},
	    {0, 0, 0, 0}
	};
	// extra options
	pmink_utils::VariantParamMap<uint32_t> extra_params;

	// default extra param values
	// --r14p-streams
	extra_params.set_int(0, 10);
	// --r14p-stimeout
	extra_params.set_int(1, 5);
	// --r14p-smsg-pool
	extra_params.set_int(2, 10);
	// --r14p-sparam-pool
	extra_params.set_int(3, 1000);


	// argc check
	if(argc < 11){
		print_help();
		return 1;
	}else{
		while ((opt = getopt_long(argc, argv, "c:t:i:s:a:", long_options, &option_index)) != -1) {
		//while ((opt = getopt(argc, argv, "c:t:i:s:a:")) != -1) {
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



				// client id
				case 'a':
				    client_id = optarg;
				    if(client_id.size() > 10){
					std::cout << "ERROR: Maximum size of client id string is 10 characters!" << std::endl;
					exit(EXIT_FAILURE);

				    }
				    break;

				// target daemon id
				case 'i':
				    dm_id = optarg;
				    if(dm_id.size() > 15){
					std::cout << "ERROR: Maximum size of daemon id string is 15 characters!" << std::endl;
					exit(EXIT_FAILURE);

				    }
				    break;

				// target daemon type
				case 't':
				    dm_type = optarg;
				    if(dm_type.size() > 15){
					std::cout << "ERROR: Maximum size of daemon type string is 15 characters!" << std::endl;
					exit(EXIT_FAILURE);

				    }
				    break;

				// target trap id
				case 's':
				    //traps[htobe32(atoi(optarg))] = NULL;
				    traps[optarg] = NULL;
				    break;

				// config daemon address
				case 'c':
					// check pattern (ipv4:port)
					// check if valid
					if(!boost::regex_match(optarg, addr_regex)){
						std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
						exit(EXIT_FAILURE);

					}
					dm_addr = optarg;
					// separate IP and PORT
					boost::regex_search(dm_addr, regex_groups, addr_regex);
					break;
			}
		}
		// client id check
		if(client_id == ""){
			std::cout << "ERROR: Client id missing!" << std::endl;
			exit(EXIT_FAILURE);

		}

		// id check
		if(dm_id == ""){
			std::cout << "ERROR: Target daemon id missing!" << std::endl;
			exit(EXIT_FAILURE);

		}

		// type check
		if(dm_type == ""){
			std::cout << "ERROR: Target daemon type missing!" << std::endl;
			exit(EXIT_FAILURE);

		}

		// trap check
		if(traps.size() == 0){
			std::cout << "ERROR: Target trap id missing!" << std::endl;
			exit(EXIT_FAILURE);

		}
		// addr check
		if(dm_addr == ""){
			std::cout << "ERROR: Target daemon address missing!" << std::endl;
			exit(EXIT_FAILURE);

		}
		// update daemon type and id
		dm_type.insert(0, "%");
		dm_id.insert(0, "%");


		// get pid and generate daemon id
		pid_t pd = getpid();
		std::ostringstream tmp_id;
		tmp_id << client_id << pd;

		// start R14P session
		r14p::R14PSession* r14ps = r14p::init_session("r14ptrapc",
							      tmp_id.str().c_str(),
							      (int)*extra_params.get_param(0),
							      (int)*extra_params.get_param(1),
							      false,
							      (int)*extra_params.get_param(1));

		//r14p::R14PSession* r14ps = r14p::init_session("r14ptrapc", tmp_id.str().c_str() , 10, 1, false, 1);
		// connect
		r14p::R14PClient* r14p_client = r14ps->connect(regex_groups[1].str().c_str(), atoi(regex_groups[2].str().c_str()), 16, NULL, 0);

		// check client
		if(r14p_client != NULL){
			TrapNext tnext;
			TrapDone tdone;
			tnext.tdone = &tdone;
			tdone.traps = &traps;

			// start new R14P stream
			r14p::R14PStream* r14p_stream = r14p_client->new_stream(dm_type.c_str(), dm_id.c_str(), NULL, &tnext);
			// if stream cannot be created, return err
			if(r14p_stream == NULL){
				std::cout << "ERROR: Cannot allocate R14P stream!" << std::endl;
				exit(EXIT_FAILURE);

			}
			// set end event handler
			r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, &tdone);
			r14p_stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, &tdone);

			// create body
			asn1::R14PMessage* r14pm = r14p_stream->get_r14p_message();
			// prepare body
			if(r14pm->_body != NULL) {
				r14pm->_body->unlink(1);
				r14pm->_body->_stats->set_linked_data(1);

			}else{
				r14pm->set_body();
				r14pm->prepare();
			}

			// ids
			uint32_t pt_stats_id = htobe32(asn1::ParameterType::_pt_pmink_stats_id);
			uint32_t stats_action = asn1::StatsAction::_sa_request;

			// set params
			if(r14pm->_body->_stats->_params == NULL){
				r14pm->_body->_stats->set_params();
				// set children, allocate more
				for(unsigned int i = 0; i<traps.size(); i++){
					r14pm->_body->_stats->_params->set_child(i);
					r14pm->_body->_stats->_params->get_child(i)->set_value();
					r14pm->_body->_stats->_params->get_child(i)->_value->set_child(0);

				}
				// prepare
				r14pm->prepare();

			// unlink params before setting new ones
			}else{
			    unsigned int cc = r14pm->_body->_stats->_params->children.size();
			    if(cc < traps.size()){
				// set children, allocate more
				for(unsigned int i = cc; i<traps.size(); i++){
					r14pm->_body->_stats->_params->set_child(i);
					r14pm->_body->_stats->_params->get_child(i)->set_value();
					r14pm->_body->_stats->_params->get_child(i)->_value->set_child(0);

				}
				// prepare
				r14pm->prepare();

			    }else if(cc > traps.size()){
				// remove extra children if used in some other session, only 2 needed
				for(unsigned int i = traps.size(); i<cc; i++) r14pm->_body->_stats->_params->get_child(i)->unlink(1);
			    }
			}

			// set stats action
			r14pm->_body->_stats->_stats_action->set_linked_data(1, (unsigned char*)&stats_action, 1);

			// trap ids
			typedef std::map<std::string, TrapVal*>::iterator it_type;
			it_type it = traps.begin();
			for(unsigned int i = 0; i<traps.size(); i++){
				// stats id
				r14pm->_body->_stats->_params->get_child(i)->_id->set_linked_data(1, (unsigned char*)&pt_stats_id, sizeof(uint32_t));
				r14pm->_body->_stats->_params->get_child(i)->_value->get_child(0)->set_linked_data(1, (unsigned char*)it->first.c_str(), it->first.length());
				++it;

			}


			// start stream
			r14p_stream->send();

			// wait for signal
			timespec ts;
			clock_gettime(0, &ts);
			ts.tv_sec += 10;
			//int sres = sem_timedwait(&tdone.adone.signal, &ts);
			int sres = sem_wait(&tdone.adone.signal);
			// error check
			if(sres == -1){
				std::cout << "ERROR: Timeout while waiting for data!" << std::endl;
				exit(EXIT_FAILURE);
			}

			unsigned int max_id_size = 0;
			// get max id size
			for(it_type it = traps.begin(); it != traps.end(); it++) if(it->first != "0"){
			    if(it->first.length() > max_id_size) max_id_size = it->first.length();
			}


			// print result
			std::cout << setw(max_id_size) << "Trap Id" << setw(30) << "Trap Value"/* <<  setw(51) << "Trap Description" */<< std::endl;
			std::cout << setfill('-') << setw(max_id_size + 30) << '-' << std::endl;
			std::cout << setfill(' ');


			// loop traps
			for(it_type it = traps.begin(); it != traps.end(); it++) if(it->first != "0"){
			    if(it->second != NULL) {
				//std::string tmp_str(it->second->desc, 0, 50);
				std::cout << setw(max_id_size) << it->first << setw(30) << it->second->value /*<< setw(51) << "n/a" */<< std::endl;
				delete it->second;

			    // no data available
			    }else{
				std::cout << setw(max_id_size) << it->first << setw(30) << "-" /*<< setw(51) << "-" */<< std::endl;

			    }


			}

		}
		// free R14P session
		r14p::destroy_session(r14ps);




	}
	return 0;
}
