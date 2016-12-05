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

#include <configdc.h>
#include <getopt.h>

// Cfgd client descriptor
CfgdcDescriptor::CfgdcDescriptor(const char* _type, const char* _desc): pmink::DaemonDescriptor(_type, NULL, _desc), cfg_script(NULL){
    r14ps = NULL;
    config = new config::Config();
    bzero(cfgd_id, sizeof(cfgd_id));
    sem_init(&sem_cfgd, 0, 0);

    // default extra param values
    // --r14p-streams
    extra_params.set_int(0, 1000);
    // --r14p-stimeout
    extra_params.set_int(1, 5);

}

CfgdcDescriptor::~CfgdcDescriptor(){
	// free routing deamons address strings
	for(unsigned int i = 0; i<routing_daemons.size(); i++) delete routing_daemons[i];
	sem_destroy(&sem_cfgd);


}

void CfgdcDescriptor::process_arguements(int argc, char** argv){
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
		while ((opt = getopt_long(argc, argv, "?r:i:f:D", long_options, &option_index)) != -1) {
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

				// config script file
				case 'f':
				    cfg_script = optarg;
				    break;

				// daemon id
				case 'i':
				    if(set_daemon_id(optarg) > 0){
					std::cout << "ERROR: Maximum size of daemon id string is 15 characters!" << std::endl;
					exit(EXIT_FAILURE);

				    }
				    break;

				// routing and config daemon address
				case 'r':
				    // check pattern (ipv4:port)
				    // check if valid
				    if(!boost::regex_match(optarg, addr_regex)){
					std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
					exit(EXIT_FAILURE);

				    }else{
					routing_daemons.push_back(new std::string(optarg));

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
		// check connections
		if(routing_daemons.size() == 0){
			std::cout << "ERROR: Routing daemon(s) not defined!" << std::endl;
			exit(EXIT_FAILURE);

		}
		// check file
		if(pmink_utils::get_file_size(cfg_script) == 0){
			std::cout << "ERROR: Config daemon command script file not found!" << std::endl;
			exit(EXIT_FAILURE);

		}


	}

}

void CfgdcDescriptor::print_help(){
	std::cout << daemon_type << " - " << daemon_description << std::endl;
	std::cout << "Copyright (c) 2012 Release14.org" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "========" << std::endl;
	std::cout << " -?\thelp" << std::endl;
	std::cout << " -i\tunique daemon id" << std::endl;
	std::cout << " -f\tconfig daemon command script file" << std::endl;
	std::cout << " -r\trouting daemon address (ipv4:port)" << std::endl;
	std::cout << " -D\tstart in debug mode" << std::endl;
	std::cout << std::endl;
	std::cout << "R14P Options:" << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << " --r14p-streams\t\tR14P Session stream pool\t\t(default = 1000)" << std::endl;
	std::cout << " --r14p-stimeout\tR14P Stream timeout in seconds\t\t(default = 5)" << std::endl;

}


void CfgdcDescriptor::init_r14p(){
	// get pid and generate daemon id
	pid_t pd = getpid();
	std::ostringstream tmp_id;
	tmp_id << get_daemon_id() << pd;

	// start R14P session
	r14ps = r14p::init_session(get_daemon_type(),
				   tmp_id.str().c_str(),
				   (int)*extra_params.get_param(0),
				   (int)*extra_params.get_param(1),
				   false,
				   (int)*extra_params.get_param(1));

	// connect to routing daemons
	boost::smatch regex_groups;
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");

	// loop routing daemons
	for(unsigned int i = 0; i<routing_daemons.size(); i++){
	    // separate IP and PORT
	    boost::regex_search(*routing_daemons[i], regex_groups, addr_regex);
	    // connect to routing daemon
	    r14ps->connect(regex_groups[1].str().c_str(),
			   atoi(regex_groups[2].str().c_str()),
			   16,
			   NULL,
			   0);
	}
}

void CfgdcDescriptor::init(){
    // init r14p
    init_r14p();
}

void CfgdcDescriptor::terminate(){
    // destroy session, free memory
    r14p::destroy_session(r14ps);
    // deallocate config memory
    if(config->get_definition_root() != NULL) delete config->get_definition_root();
    // free config
    delete config;
}

int CfgdcDescriptor::configd_user_auth(){
    // get client
    r14p::R14PClient* r14p_client = r14ps->get_client((unsigned int)0);
    // null check
    if(r14p_client != NULL && r14p_client->is_registered()){
	// login
	if(config::user_login(config, r14p_client, NULL, (char*)cfgd_id, &cfgd_uid) == 0){
	    // err check
	    if(strlen((char*)cfgd_id) > 0){
		// ok
		return 0;
	    }
	}

    }
    // ok
    return 1;
}

int CfgdcDescriptor::configd_user_logout(){
    // get client
    r14p::R14PClient* r14p_client = r14ps->get_client((unsigned int)0);
    // null check
    if(r14p_client != NULL && r14p_client->is_registered()){
	config::user_logout(config, r14p_client, NULL, &cfgd_uid);
    }
    // ok
    return 0;
}


int CfgdcDescriptor::configd_process_line(char* line, bool do_discard, bool show_errors){
    // res
    config::ConfigItem configd_res;
    snext.cfg_res = &configd_res;
    snext.line_stream_lc = 0;
    snext.error_count = 0;
    snext.err_index = 0;
    snext.line_stream.str("");
    send.timeout = false;
    configd_res.children.clear();

    // get client
    r14p::R14PClient* r14p_client = r14ps->get_client((unsigned int)0);
    // start new R14P stream
    r14p::R14PStream* r14p_stream = r14p_client->new_stream("config_daemon", NULL, NULL, &snext);
    // check if stream can be created
    if(r14p_stream == NULL){
	// show errors if enabled
	if(show_errors) std::cout << "ERROR: Stream pool empty" << std::endl;
        return 1;
    }
    // set end event handler
    r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, &send);
    r14p_stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, &send);
    // create body
    asn1::R14PMessage* r14pm = r14p_stream->get_r14p_message();
    // prepare body
    if(r14pm->_body != NULL) {
	    r14pm->_body->unlink(1);
	    r14pm->_body->_conf->set_linked_data(1);

    }else{
	    r14pm->set_body();
	    r14pm->prepare();
    }

    // set bodu
    uint32_t ac_id = htobe32(asn1::ParameterType::_pt_pmink_config_ac_line);
    uint32_t auth_id = htobe32(asn1::ParameterType::_pt_pmink_auth_id);
    uint32_t cfg_action = asn1::ConfigAction::_ca_cfg_set;
    // remove payload
    if(r14pm->_body->_conf->_payload != NULL) r14pm->_body->_conf->_payload->unlink(1);
    // set params
    if(r14pm->_body->_conf->_params == NULL){
	    r14pm->_body->_conf->set_params();
	    // set children, allocate more
	    for(int i = 0; i<2; i++){
		    r14pm->_body->_conf->_params->set_child(i);
		    r14pm->_body->_conf->_params->get_child(i)->set_value();
		    r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

	    }
	    // prepare
	    r14pm->prepare();

    // unlink params before setting new ones
    }else{
	int cc = r14pm->_body->_conf->_params->children.size();
	if(cc < 2){
	    // set children, allocate more
	    for(int i = cc; i<2; i++){
		    r14pm->_body->_conf->_params->set_child(i);
		    r14pm->_body->_conf->_params->get_child(i)->set_value();
		    r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

	    }
	    // prepare
	    r14pm->prepare();

	}else if(cc > 2){
	    // remove extra children if used in some other session, only 2 needed
	    for(int i = 2; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
	}
    }
    // set values
    r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&cfg_action, 1);
    // ac id
    r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&ac_id, sizeof(uint32_t));
    r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)line, strlen(line));
    // auth id
    r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&auth_id, sizeof(uint32_t));
    r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)cfgd_uid.user_id, strlen((char*)cfgd_uid.user_id));

    // start stream
    r14p_stream->send(true);
    // wait for signal
    sem_wait(&sem_cfgd);
    // check for timeout
    if(send.timeout){
       ++snext.error_count; 
       snext.err_lst[0].assign("Stream timeout");
    }
    // show line
    if(show_errors) std::cout << ": " << line << std::endl;
    // stop on error
    if(snext.error_count > 0){
	// show errors if enabled
	if(show_errors) std::cout << "ERROR: " << snext.err_lst[0] << std::endl;
	// discard changes on config daemon
	if(do_discard) configd_process_line((char*)"discard", false, false);
	// err
	return 1;
    }
    // results (items)
    for(unsigned int i = 0; i<configd_res.children.size(); i++){
	std::cout << configd_res.children[i]->name;
	if(configd_res.children[i]->node_type != config::CONFIG_NT_BLOCK) std::cout << " = " << configd_res.children[i]->value;
	std::cout << std::endl;
    }

    // results (lines)
    if(snext.line_stream_lc > 0) std::cout << snext.line_stream.str() << std::endl;

    // ok
    return 0;
}


int CfgdcDescriptor::process_cfgd_script(){
    // configd auth
    if(configd_user_auth() > 0) {
        std::cout << "ERROR: Auth error" << std::endl;
        return 1;
    }
    // check if configuration contents file exists
    int fsize = pmink_utils::get_file_size(cfg_script);
    // fsize already checked
    // init file buffer
    char* fbuff = new char[fsize + 1];
    bzero(fbuff, fsize + 1);
    // load contents
    if(pmink_utils::load_file(cfg_script, fbuff, &fsize) > 0){
        std::cout << "ERROR: Cannot load script file" << std::endl;
        return 1;
    }
    // tokenize
    char* tmp;
    char* token;
    token = strtok_r(fbuff, "\n", &tmp);
    // error check
    if(token == NULL) {
        std::cout << "ERROR: Script file processing error" << std::endl;
        return 2;
    }
    // process first token
    if(configd_process_line(token) > 0) {
	// configd logout
	configd_user_logout();
	// free
	delete[] fbuff;
	// err
	return 1;
    }

    // loop tokens
    while((token = strtok_r(NULL, "\n", &tmp)) != NULL){
	// check for special
	if(token[0] == ':'){
	    if(strlen(token) > 1) system(&token[1]);
	    continue;
	}
	// process token
	if(configd_process_line(token) > 0) break;

    }

    // configd logout
    configd_user_logout();

    // free
    delete[] fbuff;

    // ok
    return 0;
}

// stream end
StreamEnd::StreamEnd(){
    timeout = false;
}

void StreamEnd::run(r14p::R14PCallbackArgs* args){
    asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
    CfgdcDescriptor* dd = (CfgdcDescriptor*)pmink::CURRENT_DAEMON;
    // check for timeout (in_msg == NULL)
    if(in_msg == NULL) timeout = true;
    // signal
    sem_post(&dd->sem_cfgd);

}

// stream next
StreamNext::StreamNext(): cfg_res(NULL),
			  cm_mode(config::CONFIG_MT_UNKNOWN),
			  ac_mode(config::CONFIG_ACM_ENTER),
			  line_stream_lc(0),
			  error_count(0),
			  err_index(0){


}

void StreamNext::run(r14p::R14PCallbackArgs* args){
    r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
    asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
    uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
    config::ConfigItem* cfg_item = NULL;
    char* tmp_val = NULL;
    int tmp_val_l = 0;
    std::string tmp_str;
    uint32_t* tmp_ivp;

    // check for body
    if(in_msg->_body != NULL){
	// check for config message
	if(in_msg->_body->_conf->has_linked_data(*in_sess)){
	    // check for config result
	    if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_result){
		// check for params part
		if(in_msg->_body->_conf->_params != NULL){
			if(in_msg->_body->_conf->_params->has_linked_data(*in_sess)){
			    // process params
			    for(unsigned int i = 0; i<in_msg->_body->_conf->_params->children.size(); i++){
				// check for current session
				if(in_msg->_body->_conf->_params->get_child(i)->has_linked_data(*in_sess)){
				    // check param id, convert from big endian to host
				    uint32_t* param_id = (uint32_t*)in_msg->_body->_conf->_params->get_child(i)->_id->linked_node->tlv->value;
				    // check for value
				    if(in_msg->_body->_conf->_params->get_child(i)->_value != NULL){
					    // check if value exists in current session
					    if(in_msg->_body->_conf->_params->get_child(i)->_value->has_linked_data(*in_sess)){
						    // check if child exists
						    if(in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0) != NULL){
							// check if child exists in current sesion
							if(in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0)->has_linked_data(*in_sess)){
							    // set tmp values
							    tmp_val = (char*)in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value;
							    tmp_val_l = in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value_length;

							    // match param
							    switch(be32toh(*param_id)){
								// error count
								case asn1::ParameterType::_pt_pmink_config_ac_err_count:
								    tmp_ivp = (uint32_t*)tmp_val;
								    error_count = be32toh(*tmp_ivp);
								    break;

								// result line count
								case asn1::ParameterType::_pt_pmink_config_cfg_line_count:
								    tmp_ivp = (uint32_t*)tmp_val;
								    line_stream_lc = be32toh(*tmp_ivp);
								    break;

								// result line
								case asn1::ParameterType::_pt_pmink_config_cfg_line:
								    tmp_str.clear();
								    tmp_str.append(tmp_val, tmp_val_l);
								    line_stream << tmp_str << std::endl;
								    break;

								// error line
								case asn1::ParameterType::_pt_pmink_config_cfg_ac_err:
								    tmp_str.clear();
								    tmp_str.append(tmp_val, tmp_val_l);
								    err_lst[err_index++] = tmp_str;
								    break;

								// cli path
								case asn1::ParameterType::_pt_pmink_config_cli_path:
								    tmp_str.clear();
								    tmp_str.append(tmp_val, tmp_val_l);
								    // regenerate cli path
								    //*plugin_info->cli->get_current_path_line() = "";
								    //plugin_info->cli->generate_path(plugin_info->cli->get_current_path(), plugin_info->cli->get_current_path_line());
								    // add cfg path to cli path
								    //plugin_info->cli->get_current_path_line()->append(tmp_str);
								    //plugin_info->cli->generate_prompt();
								    break;

								// ac line
								case asn1::ParameterType::_pt_pmink_config_ac_line:
								    // update line
								    //plugin_info->cli->clear_curent_line();
								    //plugin_info->cli->get_current_line()->append(tmp_val, tmp_val_l);
								    // add to history
								    //plugin_info->cli->add_to_history(plugin_info->cli->get_current_line());
								    //plugin_info->cli->history_index = plugin_info->cli->get_historu_size();
								    break;

								// config item name
								case asn1::ParameterType::_pt_pmink_config_cfg_item_name:
								    // start new config item
								    cfg_item = new config::ConfigItem();
								    cfg_res->children.push_back(cfg_item);
								    cfg_item->name.append(tmp_val, tmp_val_l);
								    break;

								// config item desc
								case asn1::ParameterType::_pt_pmink_config_cfg_item_desc:
								    cfg_item->desc.append(tmp_val, tmp_val_l);
								    break;

								// config item node state
								case asn1::ParameterType::_pt_pmink_config_cfg_item_ns:
								    cfg_item->node_state = (config::ConfigNodeState)*tmp_val;
								    break;

								// config item node value
								case asn1::ParameterType::_pt_pmink_config_cfg_item_value:
								    cfg_item->value.append(tmp_val, tmp_val_l);
								    break;

								// config item node new value
								case asn1::ParameterType::_pt_pmink_config_cfg_item_nvalue:
								    cfg_item->new_value.append(tmp_val, tmp_val_l);
								    break;

								// config item node type
								case asn1::ParameterType::_pt_pmink_config_cfg_item_nt:
								    cfg_item->node_type = (config::ConfigNodeType)*tmp_val;
								    break;

								// config item mode
								case asn1::ParameterType::_pt_pmink_config_cfg_cm_mode:
								    cm_mode = (config::ConfigModeType)*tmp_val;
								    break;


							    }

							}
						    }
					    }
				    }

				}
			    }

			}

		}
	    }
	}

    }
    // continue
    stream->continue_sequence();
}

