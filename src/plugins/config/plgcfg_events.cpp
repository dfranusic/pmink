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

#include <plgcfg_events.h>

PluginInfo::PluginInfo(){
	config = NULL;
	cli = NULL;
	r14ps = NULL;
	sem_init(&sem_cfgd, 0, 0);
	bzero(last_cfgd_id, sizeof(last_cfgd_id));
	last_r14pc = NULL;
	//cfgd_active = false;
	hbeat = NULL;

}

PluginInfo::~PluginInfo(){
	for(unsigned i = 0; i<cfgd_lst.size(); i++) delete cfgd_lst[i];
	sem_destroy(&sem_cfgd);
}


StreamEnd::StreamEnd(PluginInfo* _pi){
	plugin_info = _pi;

}

void StreamEnd::run(r14p::R14PCallbackArgs* args){
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);

	// no need to check for session id, header is mandatory and present in every r14p message
	if(	in_msg != NULL &&
		(in_msg->_header->_sequence_flag->linked_node->tlv->value[0] == asn1::SequenceFlag::_sf_end ||
		in_msg->_header->_sequence_flag->linked_node->tlv->value[0] == asn1::SequenceFlag::_sf_stream_complete)){

	}

	// signal
	sem_post(&plugin_info->sem_cfgd);
}



StreamNext::StreamNext(PluginInfo* _pi, config::ConfigItem* _cfg_res){
	plugin_info = _pi;
	cfg_res = _cfg_res;
	cm_mode = config::CONFIG_MT_UNKNOWN;
	ac_mode = config::CONFIG_ACM_TAB;
	line_stream_lc = 0;
	error_count = 0;
	err_index = 0;

}


void StreamNext::process_enter(r14p::R14PCallbackArgs* args){
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
									*plugin_info->cli->get_current_path_line() = "";
									plugin_info->cli->generate_path(plugin_info->cli->get_current_path(), plugin_info->cli->get_current_path_line());
									// add cfg path to cli path
									plugin_info->cli->get_current_path_line()->append(tmp_str);
									plugin_info->cli->generate_prompt();
									break;

								    // ac line
								    case asn1::ParameterType::_pt_pmink_config_ac_line:
									// update line
									plugin_info->cli->clear_curent_line();
									plugin_info->cli->get_current_line()->append(tmp_val, tmp_val_l);
									// add to history
									plugin_info->cli->add_to_history(plugin_info->cli->get_current_line());
									plugin_info->cli->history_index = plugin_info->cli->get_historu_size();
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

void StreamNext::process_tab(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	config::ConfigItem* cfg_item = NULL;
	char* tmp_val = NULL;
	int tmp_val_l = 0;

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
								    // ac line
								    case asn1::ParameterType::_pt_pmink_config_ac_line:
									// update line
									plugin_info->cli->clear_curent_line();
									plugin_info->cli->get_current_line()->append(tmp_val, tmp_val_l);
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

								    // config mode
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

void StreamNext::run(r14p::R14PCallbackArgs* args){
	switch(ac_mode){
	    case config::CONFIG_ACM_ENTER:
		process_enter(args);
		break;

	    case config::CONFIG_ACM_TAB:
		process_tab(args);
		break;
	}

}


