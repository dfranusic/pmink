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

#include <config_r14p.h>

void config::CfgUpdateClientTerm::run(r14p::R14PCallbackArgs* args){
	r14p::R14PClient* client = (r14p::R14PClient*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_CLIENT);
	r14p::R14PCallbackMethod* snew = client->get_callback(r14p::R14P_ET_STREAM_NEW, true);
	// delete stream new event
	if(snew != NULL) delete snew;
	// deallocate current event
	delete this;
}

config::CfgUpdateStreamNew::CfgUpdateStreamNew(){
    update_done = NULL;
    config = NULL;
}
void config::CfgUpdateStreamNew::run(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	char* tmp_val = NULL;
	uint32_t* tmp_ivp;

	// new stream next event
	CfgUpdateStreamNext* snext = new CfgUpdateStreamNext();
	snext->sdone.snew = this;
	snext->sdone.snext = snext;
	// set events
	stream->set_callback(r14p::R14P_ET_STREAM_NEXT, snext);
	stream->set_callback(r14p::R14P_ET_STREAM_END, &snext->sdone);


	// check for body
	if(in_msg->_body != NULL){
	    // check for config message
	    if(in_msg->_body->_conf->has_linked_data(*in_sess)){
		// check for GET action
		if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_set){
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

								// match param
								switch(be32toh(*param_id)){
								    // config item count
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_count:
									tmp_ivp = (uint32_t*)tmp_val;
									snext->update_count = be32toh(*tmp_ivp);
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
		    // continue
		    stream->continue_sequence();

		// not GET action, end stream
		}else{
		    stream->end_sequence();

		}
	    // continue with other handler
	    }else{
		run_continue(args);
	    }
	}


}

config::CfgUpdateStreamDone::CfgUpdateStreamDone(){
    snew = NULL;
    snext = NULL;

}

void config::CfgUpdateStreamDone::process_cfg_events(){
	// get node
	config::ConfigItem* root = snew->config->get_definition_root();
	// sanity check
	if(root == NULL) return;

	// group by ON CHANGE handler
	std::map<CfgNtfCallback*, config::ConfigItem> hndlr_map;
	std::map<CfgNtfCallback*, config::ConfigItem>::iterator it;
	config::ConfigItem* tmp_node = NULL;
	config::ConfigItem tmp_group;
	// check flat cfg nodes
	for(unsigned int i = 0; i<snext->cfg_res.children.size(); i++){
	    // find node in tree structure
	    tmp_node = (*root)(snext->cfg_res.children[i]->name.c_str());
	    // skip if not found
	    if(tmp_node == NULL) continue;
	    // skip if no ON CHAGNE andler
	    if(tmp_node->on_change == NULL) continue;
	    // update node value (important for MOD/DEL actions since values are modified after this event)
	    // no need to reset it after, it will be done if CfgUpdateStreamDone method
	    tmp_node->value = snext->cfg_res.children[i]->value;
	    // update node state
	    tmp_node->node_state = snext->cfg_res.children[i]->node_state;
	    // if ITEM node and DELETED, set value to empty string (important since values are modified after this event)
	    if(tmp_node->node_state == config::CONFIG_NS_DELETED && tmp_node->node_type == config::CONFIG_NT_ITEM) tmp_node->value = "";
	    // check if handler already in map
	    it = hndlr_map.find(tmp_node->on_change);
	    // new handler found
	    if(it == hndlr_map.end()){
		hndlr_map[tmp_node->on_change] = tmp_group;
		// add if not executed in previous pass
		if(!tmp_node->onc_hndlr_exec) hndlr_map[tmp_node->on_change].children.push_back(tmp_node);

	    // add node to handler
	    }else{
		// add if not executed in previous pass
		if(!tmp_node->onc_hndlr_exec) it->second.children.push_back(tmp_node);
	    }


	}

	// run handlers
	for(it = hndlr_map.begin(); it != hndlr_map.end(); it++){
	    // check if modification list is empty
	    if(it->second.children.size() > 0){
		// run event handler
		it->first->run(&it->second);
		// mark as executed
		for(unsigned int i = 0; i<it->second.children.size(); i++) it->second.children[i]->onc_hndlr_exec = !it->second.children[i]->onc_hndlr_exec;
	    }

	}




	// remove grouped children to avoid deallocation
	for(it = hndlr_map.begin(); it != hndlr_map.end(); it++) it->second.children.clear();
	// clear map
	hndlr_map.clear();


}


void config::CfgUpdateStreamDone::run(r14p::R14PCallbackArgs* args){
    // update configuration, changes in snew->snext->cfg_res
    if(snew != NULL){
	    ConfigItem* tmp_item = NULL;
	    ConfigItem* ch_item = NULL;

	    // lock config
	    snew->config->lock();

	    // run cfg events (first pass handles MODIFICATION and DELETION)
	    process_cfg_events();

	    // loop config changes (create NEW nodes als)
	    for(unsigned int i = 0; i<snext->cfg_res.children.size(); i++){
		// set pointer
		tmp_item = snext->cfg_res.children[i];
		// check if config path exists (ConfigItem operator "()")
		ch_item = (*snew->config->get_definition_root())(tmp_item->name.c_str(), true, tmp_item->node_type, true);
		// config path exists
		if(ch_item != NULL){
		    // BLOCK node
		    if(ch_item->node_type == CONFIG_NT_BLOCK){
			// deletion
			if(tmp_item->node_state == CONFIG_NS_DELETED){
				// validate parent
				if(ch_item->parent != NULL){
					// get item index
					int index = ch_item->parent->find(ch_item);
					// check if found
					if(index > -1){
					    // delete from list
					    ch_item->parent->children.erase(ch_item->parent->children.begin() + index);
					    // free
					    delete ch_item;
					}

				}

			}
		    // ITEM node
		    }else if(ch_item->node_type == CONFIG_NT_ITEM){
			if(tmp_item->node_state == CONFIG_NS_DELETED){
			    ch_item->value = "";
			    tmp_item->value = "";
			}else ch_item->value = tmp_item->value;

		    }


		}

	    }

	    // run cfg events (second pass handles ADDITION)
	    process_cfg_events();

	    // clear ON CHANGE executed flags, set node states to READY
	    for(unsigned int i = 0; i<snext->cfg_res.children.size(); i++) {
		// set pointer
		tmp_item = snext->cfg_res.children[i];
		// check if config path exists (ConfigItem operator "()")
		ch_item = (*snew->config->get_definition_root())(tmp_item->name.c_str());
		// skip if not found
		if(ch_item == NULL) continue;
		// reset flags
		ch_item->onc_hndlr_exec = false;
		ch_item->is_new = false;
		// set node state to ready
		ch_item->node_state = CONFIG_NS_READY;
	    }

	    // run user defined event handler
	    if(snew->update_done != NULL) snew->update_done->run(&snext->cfg_res);

	    // unlock config
	    snew->config->unlock();

	    // free stream next memory
	    delete snext;

    }

}

config::CfgUpdateStreamNext::CfgUpdateStreamNext(){
    update_count = 0;
    res_index = 0;
}

void config::CfgUpdateStreamNext::run(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	char* tmp_val = NULL;
	int tmp_val_l = 0;
	std::string tmp_str;
	config::ConfigItem* cfg_item = NULL;

	// check for body
	if(in_msg->_body != NULL){
	    // check for config message
	    if(in_msg->_body->_conf->has_linked_data(*in_sess)){
		// check for config result
		if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_set){
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
							    // check if child exists in current session
							    if(in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0)->has_linked_data(*in_sess)){
								// set tmp values
								tmp_val = (char*)in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value;
								tmp_val_l = in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value_length;


								// match param
								switch(be32toh(*param_id)){
								    // config item path
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_path:
									// start new config item
									cfg_item = new config::ConfigItem();
									cfg_item->name.append(tmp_val, tmp_val_l);
									cfg_res.children.push_back(cfg_item);
									break;

								    // config item node value
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_value:
									cfg_item->value.append(tmp_val, tmp_val_l);
									break;

								    // config item node type
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_nt:
									cfg_item->node_type = (config::ConfigNodeType)*tmp_val;
									break;

								    // config item node state
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_ns:
									cfg_item->node_state = (config::ConfigNodeState)*tmp_val;
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
		    // continue
		    stream->continue_sequence();
		}
	    }

	}
}

config::R14PCfgNtfUser::R14PCfgNtfUser(r14p::R14PClient* _r14pc){
    r14pc = _r14pc;
}




config::R14PCfgNotification::R14PCfgNotification(std::string* _cfg_path) : CfgNotification(_cfg_path) {
    ready = false;
}

config::R14PCfgNotification::~R14PCfgNotification(){

}

int config::R14PCfgNotification::notify(void* args){
    return 0;
}

void* config::R14PCfgNotification::reg_user(void* usr){
    R14PCfgNtfUser* uid = (R14PCfgNtfUser*)usr;
    if(!user_exists(uid)){
	users.push_back(*uid);
	return usr;
    }
    return NULL;
}

int config::R14PCfgNotification::unreg_user(void* usr){
    config::UserId* uid = (config::UserId*)usr;
    for(unsigned int i = 0; i<users.size(); i++) if(users[i] == *uid){
	users.erase(users.begin() + i);
	return 0;
    }
    return 1;
}

bool config::R14PCfgNotification::user_exists(config::R14PCfgNtfUser* usr){
    for(unsigned int i = 0; i<users.size(); i++) if(users[i] == *usr) return true;
    return false;
}

config::R14PCfgNtfUser* config::R14PCfgNotification::get_user(unsigned int usr_index){
    if(usr_index < users.size()) return &users[usr_index];
    return NULL;
}

unsigned int config::R14PCfgNotification::get_user_count(){
    return users.size();
}


config::RegUseStreamDone::RegUseStreamDone(){
    snext = NULL;
    status = 0;
    sem_init(&signal, 0, 0);
}

config::RegUseStreamDone::~RegUseStreamDone(){
    sem_destroy(&signal);
}

void config::RegUseStreamDone::run(r14p::R14PCallbackArgs* args){
    r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
    status = stream->get_timeout_status();
    sem_post(&signal);
}

config::RegUsrStreamNext::RegUsrStreamNext(){
    cfg_count = 0;
}

void config::RegUsrStreamNext::run(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	config::ConfigItem* cfg_item = NULL;
	char* tmp_val = NULL;
	int tmp_val_l = 0;
	std::string tmp_str;

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
								    // item count
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_count:
									cfg_count = be32toh(*(uint32_t*)tmp_val);
									break;

								    // config item path
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_path:
									// start new config item
									cfg_item = new config::ConfigItem();
									cfg_res.children.push_back(cfg_item);
									cfg_item->name.append(tmp_val, tmp_val_l);
									break;

								    // config item node value
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_value:
									cfg_item->value.append(tmp_val, tmp_val_l);
									break;

								    // config item node type
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_nt:
									cfg_item->node_type = (config::ConfigNodeType)*tmp_val;
									break;

								    // config item node state
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_ns:
									cfg_item->node_state = (config::ConfigNodeState)*tmp_val;
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
		    // continue
		    stream->continue_sequence();
		}
	    }

	}
}

config::DistributeCfgStreamNext::DistributeCfgStreamNext(): res_count(0), res_index(0){
	ca_cfg_replicate = asn1::ConfigAction::_ca_cfg_replicate;
	pt_cfg_repl_line = htobe32(asn1::ParameterType::_pt_pmink_config_replication_line);
	pt_cfg_auth_id = htobe32(asn1::ParameterType::_pt_pmink_auth_id);

}

void config::DistributeCfgStreamNext::run(r14p::R14PCallbackArgs* args){
    // nothing for now
}

void config::DistributeCfgStreamDone::run(r14p::R14PCallbackArgs* args){
    delete snext;
    delete this;
}




config::NtfyUsrStreamNext::NtfyUsrStreamNext(){
	cfg_ntf = NULL;
	config = NULL;
	res_count = 0;
	res_index = 0;
	ntf_user = NULL;
	// big endian parameter ids
	pt_cfg_item_path = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_path);
	pt_cfg_item_value = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_value);
	pt_cfg_item_count = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_count);
	ca_cfg_set = asn1::ConfigAction::_ca_cfg_set;
	pt_cfg_item_ns = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_ns);
	pt_cfg_item_nt = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_nt);

}

void config::NtfyUsrStreamNext::run(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);

	// more results
	if(res_index < cfg_flat.children.size()){
		// prepare body
		if(r14pm->_body != NULL) {
			r14pm->_body->unlink(1);
			r14pm->_body->_conf->set_linked_data(1);

		}else{
			r14pm->set_body();
			r14pm->prepare();
		}
		// remove payload
		if(r14pm->_body->_conf->_payload != NULL) r14pm->_body->_conf->_payload->unlink(1);
		// set params
		if(r14pm->_body->_conf->_params == NULL){
			r14pm->_body->_conf->set_params();
			// set children, allocate more
			for(int i = 0; i<4; i++){
				r14pm->_body->_conf->_params->set_child(i);
				r14pm->_body->_conf->_params->get_child(i)->set_value();
				r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

			}
			// prepare
			r14pm->prepare();

		// unlink params before setting new ones
		}else{
		    int cc = r14pm->_body->_conf->_params->children.size();
		    if(cc < 4){
			// set children, allocate more
			for(int i = cc; i<4; i++){
				r14pm->_body->_conf->_params->set_child(i);
				r14pm->_body->_conf->_params->get_child(i)->set_value();
				r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

			}
			// prepare
			r14pm->prepare();

		    }else if(cc > 4){
			// remove extra children if used in some other session, only 4 needed
			for(int i = 4; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
		    }
		}

		// set cfg action
		r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&ca_cfg_set, 1);

		// cfg path
		r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_path, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)cfg_flat.children[res_index]->name.c_str(), cfg_flat.children[res_index]->name.size());

		// cfg item value
		r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_value, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)cfg_flat.children[res_index]->value.c_str(), cfg_flat.children[res_index]->value.size());

		// cfg item node type
		r14pm->_body->_conf->_params->get_child(2)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_nt, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(2)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&cfg_flat.children[res_index]->node_type, 1);

		// cfg item node state
		r14pm->_body->_conf->_params->get_child(3)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_ns, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(3)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&cfg_flat.children[res_index]->node_state, 1);


		// include body
		*include_body = true;


		// next result item
		++res_index;

		// continue
		stream->continue_sequence();


	}else{
		stream->end_sequence();

	}


}

void config::NtfyUsrStreamDone::run(r14p::R14PCallbackArgs* args){
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);

	if(in_msg->_header->_status != NULL){
		if(in_msg->_header->_status->has_linked_data(*in_sess)){
		    // error
		    if(in_msg->_header->_status->linked_node->tlv->value[0] != 0){
			    // lock config
			    snext->config->lock();
			    // remove user
			    if(snext->cfg_ntf != NULL) snext->cfg_ntf->unreg_user(snext->ntf_user);
			    // unlock config
			    snext->config->unlock();
		    }
		}
	}

	// free stream next (allocated in notify_user)
	delete snext;
	// free stream done (allocated in notify_user)
	delete this;
}

config::NtfyUsrStreamDone::NtfyUsrStreamDone(){
    snext = NULL;

}

int config::replicate(const char* repl_line, r14p::R14PClient* _client, const char* _daemon_id, config::UserId* _cfg_user_id){
    // used inside R14P in_loop, cannot use semaphore, this method is async
    if(repl_line != NULL && _client != NULL && _daemon_id != NULL && _cfg_user_id != NULL){
	DistributeCfgStreamNext* snext = new DistributeCfgStreamNext();
	DistributeCfgStreamDone* sdone = new DistributeCfgStreamDone();
	sdone->snext = snext;

	snext->repl_line.append(repl_line);
	snext->cfg_user_id = *_cfg_user_id;


	// start new R14P stream
	r14p::R14PStream* r14p_stream = _client->new_stream(	"config_daemon",
								_daemon_id,
								NULL,
								snext);
	// if stream cannot be created, return err
	if(r14p_stream == NULL){
	    delete snext;
	    delete sdone;
	    return 1;
	}
	// set end event handler
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, sdone);
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

	// set cfg action
	r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&snext->ca_cfg_replicate, 1);

	// cfg replication line
	r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&snext->pt_cfg_repl_line, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)snext->repl_line.c_str(), snext->repl_line.size());

	// auth id
	r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&snext->pt_cfg_auth_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)snext->cfg_user_id.user_id, strlen((char*)snext->cfg_user_id.user_id));

	// start stream
	r14p_stream->send(true);


	// ok
	return 0;
    }
    // err
    return 1;
}


int config::notify_user(	config::Config* config,
				config::ConfigItem* cfg_flat,
				config::R14PCfgNtfUser* ntf_user,
				config::R14PCfgNotification* cfg_ntf){
    // used inside R14P in_loop, cannot use semaphore, this method is async
    if(cfg_flat != NULL && ntf_user != NULL){
	NtfyUsrStreamNext* snext = new NtfyUsrStreamNext();
	NtfyUsrStreamDone* sdone = new NtfyUsrStreamDone();
	sdone->snext = snext;
	// copy ntf cfg node list
	config->copy_nodes(cfg_flat, &snext->cfg_flat);
	//snext->cfg_flat = cfg_flat;
	snext->cfg_ntf = cfg_ntf;
	snext->ntf_user = ntf_user;
	snext->res_index = 0;
	snext->config = config;
	// start new R14P stream
	r14p::R14PStream* r14p_stream = ntf_user->r14pc->new_stream(	(char*)ntf_user->user_type,
									(char*)ntf_user->user_id,
									NULL,
									snext);

	// if stream cannot be created, return err
	if(r14p_stream == NULL){
	    delete snext;
	    delete sdone;
	    return 1;
	}
	// set end event handler
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, sdone);
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

	// remove payload
	if(r14pm->_body->_conf->_payload != NULL) r14pm->_body->_conf->_payload->unlink(1);
	// set params
	if(r14pm->_body->_conf->_params == NULL){
		r14pm->_body->_conf->set_params();
		// set children, allocate more
		for(int i = 0; i<1; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	// unlink params before setting new ones
	}else{
	    int cc = r14pm->_body->_conf->_params->children.size();
	    if(cc < 1){
		// set children, allocate more
		for(int i = cc; i<1; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	    }else if(cc > 1){
		// remove extra children if used in some other session, only 2 needed
		for(int i = 1; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
	    }
	}

	// set cfg action
	r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&snext->ca_cfg_set, 1);

	// cfg item count
	snext->res_count = htobe32(cfg_flat->children.size());
	r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&snext->pt_cfg_item_count, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&snext->res_count, sizeof(uint32_t));


	// start stream
	r14p_stream->send(true);


	// return ok
	return 0;
    }
    // return err
    return 1;
}


int config::user_logout(config::Config* config, r14p::R14PClient* cfgd_r14pc, const char* _daemon_id, config::UserId* cfg_user_id){
    if(config != NULL && cfgd_r14pc != NULL){
	// Client registration stream next
	class _InitUserStremDone: public r14p::R14PCallbackMethod {
	public:
		_InitUserStremDone(){
		    sem_init(&signal, 0, 0);

		}
		~_InitUserStremDone(){
		    sem_destroy(&signal);
		}

	        // event handler method
	        void run(r14p::R14PCallbackArgs* args){
	            sem_post(&signal);
	        }

	        // signal
	        sem_t signal;

	};
	// stream done event
	_InitUserStremDone sdone;

	// start new R14P stream
	r14p::R14PStream* r14p_stream = cfgd_r14pc->new_stream("config_daemon", _daemon_id, NULL, NULL);
	// if stream cannot be created, return err
	if(r14p_stream == NULL) return 1;
	// set end event handler
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, &sdone);
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, &sdone);
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
	uint32_t auth_id = htobe32(asn1::ParameterType::_pt_pmink_auth_id);
	uint32_t cfg_action = asn1::ConfigAction::_ca_cfg_user_logout;

	// remove payload
	if(r14pm->_body->_conf->_payload != NULL) r14pm->_body->_conf->_payload->unlink(1);
	// set params
	if(r14pm->_body->_conf->_params == NULL){
		r14pm->_body->_conf->set_params();
		// set children, allocate more
		for(int i = 0; i<1; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	// unlink params before setting new ones
	}else{
	    int cc = r14pm->_body->_conf->_params->children.size();
	    if(cc < 1){
		// set children, allocate more
		for(int i = cc; i<1; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	    }else if(cc > 1){
		// remove extra children if used in some other session, only 2 needed
		for(int i = 1; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
	    }
	}

	// set cfg action
	r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&cfg_action, 1);


	// user id (daemon_type : daemon_id : socket_id)
	char tmp_user_id[46];
	bzero(tmp_user_id, sizeof(tmp_user_id));
	memcpy(tmp_user_id, cfgd_r14pc->get_session()->get_daemon_type(), strlen(cfgd_r14pc->get_session()->get_daemon_type()));
	tmp_user_id[strlen(tmp_user_id)] = ':';
	memcpy(&tmp_user_id[strlen(tmp_user_id)], cfgd_r14pc->get_session()->get_daemon_id(), strlen(cfgd_r14pc->get_session()->get_daemon_id()));
	// set UserId values
	memcpy(cfg_user_id->user_id, tmp_user_id, strlen(tmp_user_id));
	memcpy(cfg_user_id->user_type, cfgd_r14pc->get_session()->get_daemon_type(), strlen(cfgd_r14pc->get_session()->get_daemon_type()));

	// cfg uth id
	r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&auth_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)tmp_user_id, strlen(tmp_user_id));


	// start stream
	r14p_stream->send(true);

	// wait for signal
	timespec ts;
	clock_gettime(0, &ts);
	ts.tv_sec += 10;
	int sres = sem_wait(&sdone.signal);
	// error check
	if(sres == -1) return 1;

	// return ok
	return 0;
    }

    // return err
    return 1;

}

int config::user_login(config::Config* config, r14p::R14PClient* cfgd_r14pc, const char* _target_daemon_id, char* _connected_daemon_id, config::UserId* cfg_user_id){
    if(config != NULL && cfgd_r14pc != NULL){
	// Client registration stream next
	class _InitUserStremDone: public r14p::R14PCallbackMethod {
	public:
		_InitUserStremDone(char* _out_daemon_id){
		    sem_init(&signal, 0, 0);
		    out_daemon_id = _out_daemon_id;

		}
		~_InitUserStremDone(){
		    sem_destroy(&signal);
		}

	        // event handler method
	        void run(r14p::R14PCallbackArgs* args){
	            asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	            uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);

	            // timeout if in_msg is NULL
	            if(in_msg == NULL) {
			sem_post(&signal);
	        	return;
	            }
	            // check status
	            if(in_msg->_header->_status != NULL){
		            if(in_msg->_header->_status->has_linked_data(*in_sess)){
		        	if(in_msg->_header->_status->linked_node->tlv->value[0] == 0){

			            // get connected daemon id
			            if(in_msg->_header->_source->_id != NULL){
				            if(in_msg->_header->_source->_id->has_linked_data(*in_sess)){
				        	    // C string
					            memcpy(	out_daemon_id,
					        		in_msg->_header->_source->_id->linked_node->tlv->value,
					        		in_msg->_header->_source->_id->linked_node->tlv->value_length);

					            // null character
					            out_daemon_id[in_msg->_header->_source->_id->linked_node->tlv->value_length] = 0;

				            }
			            }

		        	}
		            }
	            }



	            sem_post(&signal);
	        }

	        // signal
	        sem_t signal;
	        char* out_daemon_id;

	};
	// reset connectd daeon
	_connected_daemon_id[0] = 0;
	// stream done event
	_InitUserStremDone* sdone = new _InitUserStremDone(_connected_daemon_id);

	// start new R14P stream
	r14p::R14PStream* r14p_stream = cfgd_r14pc->new_stream("config_daemon", _target_daemon_id, NULL, NULL);
	// if stream cannot be created, return err
	if(r14p_stream == NULL) return 1;
	// set end event handler
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, sdone);
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, sdone);
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
	uint32_t auth_id = htobe32(asn1::ParameterType::_pt_pmink_auth_id);
	uint32_t cfg_action = asn1::ConfigAction::_ca_cfg_user_login;

	// remove payload
	if(r14pm->_body->_conf->_payload != NULL) r14pm->_body->_conf->_payload->unlink(1);
	// set params
	if(r14pm->_body->_conf->_params == NULL){
		r14pm->_body->_conf->set_params();
		// set children, allocate more
		for(int i = 0; i<1; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	// unlink params before setting new ones
	}else{
	    int cc = r14pm->_body->_conf->_params->children.size();
	    if(cc < 1){
		// set children, allocate more
		for(int i = cc; i<1; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	    }else if(cc > 1){
		// remove extra children if used in some other session, only 2 needed
		for(int i = 1; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
	    }
	}

	// set cfg action
	r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&cfg_action, 1);

	// user id (daemon_type : daemon_id : socket_id)
	char tmp_user_id[46];
	bzero(tmp_user_id, sizeof(tmp_user_id));
	memcpy(tmp_user_id, cfgd_r14pc->get_session()->get_daemon_type(), strlen(cfgd_r14pc->get_session()->get_daemon_type()));
	tmp_user_id[strlen(tmp_user_id)] = ':';
	memcpy(&tmp_user_id[strlen(tmp_user_id)], cfgd_r14pc->get_session()->get_daemon_id(), strlen(cfgd_r14pc->get_session()->get_daemon_id()));
	// set UserId values
	memcpy(cfg_user_id->user_id, tmp_user_id, strlen(tmp_user_id));
	// cfg auth id
	r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&auth_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)tmp_user_id, strlen(tmp_user_id));


	// start stream
	r14p_stream->send(true);

	// wait for signal
	sem_wait(&sdone->signal);
	// error check
	//if(sres == -1) return 1;

	// free
	delete sdone;

	// return ok
	return 0;
    }

    // return err
    return 1;
}


int config::notification_request(	config::Config* config,
					r14p::R14PClient* cfgd_r14pc,
					const char* usr_root,
					config::CfgNtfCallback* update_rcvd,
					const char* _daemon_id,
					config::UserId* cfg_user_id,
					r14p::R14PCallbackMethod* non_cfg_hndlr){

    if(config != NULL && cfgd_r14pc != NULL && usr_root != NULL){

	RegUsrStreamNext snext;
	RegUseStreamDone sdone;
	sdone.snext = &snext;
	// start new R14P stream
	r14p::R14PStream* r14p_stream = cfgd_r14pc->new_stream("config_daemon", _daemon_id, NULL, &snext);
	// if stream cannot be created, return err
	if(r14p_stream == NULL) return 1;
	// set end event handler
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, &sdone);
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, &sdone);
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
	uint32_t cfg_ntfy_id = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_notify);
	uint32_t cfg_path_id = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_path);
	uint32_t auth_id = htobe32(asn1::ParameterType::_pt_pmink_auth_id);
	uint32_t cfg_action = asn1::ConfigAction::_ca_cfg_get;
	int cfg_ntfy_flag = 1;
	// remove payload
	if(r14pm->_body->_conf->_payload != NULL) r14pm->_body->_conf->_payload->unlink(1);
	// set params
	if(r14pm->_body->_conf->_params == NULL){
		r14pm->_body->_conf->set_params();
		// set children, allocate more
		for(int i = 0; i<3; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	// unlink params before setting new ones
	}else{
	    int cc = r14pm->_body->_conf->_params->children.size();
	    if(cc < 3){
		// set children, allocate more
		for(int i = cc; i<3; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	    }else if(cc > 3){
		// remove extra children if used in some other session, only 2 needed
		for(int i = 3; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
	    }
	}

	// set cfg action
	r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&cfg_action, 1);

	// cfg path
	r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&cfg_path_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)usr_root, strlen(usr_root));

	// cfg notify flag
	r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&cfg_ntfy_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&cfg_ntfy_flag, 1);

	// auth id
	r14pm->_body->_conf->_params->get_child(2)->_id->set_linked_data(1, (unsigned char*)&auth_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(2)->_value->get_child(0)->set_linked_data(1, cfg_user_id->user_id, strlen((char*)cfg_user_id->user_id));


	// start stream
	r14p_stream->send(true);

	// wait for signal
	timespec ts;
	clock_gettime(0, &ts);
	ts.tv_sec += 10;
	int sres = sem_wait(&sdone.signal);
	// error check
	if(sres == -1 || sdone.status > 0 || snext.cfg_count != snext.cfg_res.children.size()) return 1;

	// crete new definition if needed
	if(config->get_definition_root() == NULL) config->new_definition();
	// loop list, only CONFIG_NT_ITEM nodes included
	for(unsigned int i = 0; i<snext.cfg_res.children.size(); i++){
	    ConfigItem* tmp_cfg = (*config->get_definition_root())(snext.cfg_res.children[i]->name.c_str(), true, snext.cfg_res.children[i]->node_type);
	    tmp_cfg->value = snext.cfg_res.children[i]->value;
	}
	// set event handlers
	if(	cfgd_r14pc->get_callback(r14p::R14P_ET_STREAM_NEW) == NULL &&
		cfgd_r14pc->get_callback(r14p::R14P_ET_CLIENT_TERMINATED) == NULL){

		CfgUpdateStreamNew* cfg_snew = new CfgUpdateStreamNew();
		CfgUpdateClientTerm* cfg_term = new CfgUpdateClientTerm();
		cfg_snew->update_done = update_rcvd;
		cfg_snew->config = config;
		cfg_snew->set_continue_callback(non_cfg_hndlr);
		cfgd_r14pc->set_callback(r14p::R14P_ET_STREAM_NEW, cfg_snew);
		cfgd_r14pc->set_callback(r14p::R14P_ET_CLIENT_TERMINATED, cfg_term);

	}



	// return ok
	return 0;
    }

    // return err
    return 1;
}

r14p::R14PCallbackMethod* config::create_cfg_event_handler(config::Config* config, r14p::R14PCallbackMethod* non_cfg_hndlr){
    CfgUpdateStreamNew* cfg_snew = new CfgUpdateStreamNew();
    cfg_snew->update_done = NULL;
    cfg_snew->config = config;
    cfg_snew->set_continue_callback(non_cfg_hndlr);
    return cfg_snew;


}


