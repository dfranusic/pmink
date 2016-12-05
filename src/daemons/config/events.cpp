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

#include <events.h>
#include <fstream>
#include <antlr_utils.h>


ClientIdle::ClientIdle(){
    config = NULL;
}

void ClientIdle::run(r14p::R14PCallbackArgs* args){
    // * unlock to avoid deadlock
    // * could happen if client stream was interrupted and not properly closed
    // * if this client did not previously call lock(), unlock() method will fail silently
    config->unlock();

}

ClientDown::ClientDown(config::Config* _config){
    config = _config;
}


void ClientDown::run(r14p::R14PCallbackArgs* args){
	// * unlock to avoid deadlock
	// * could happen if client stream was interrupted and not properly closed
	// * if this client did not previously call lock(), unlock() method will fail silently
	config->unlock();
}



ClientDone::ClientDone(config::Config* _config){
    config = _config;
}

void ClientDone::run(r14p::R14PCallbackArgs* args){
	// nothing to do for now
}

void NewClient::run(r14p::R14PCallbackArgs* args){
	r14p::R14PClient* client = (r14p::R14PClient*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_CLIENT);
	// handle new stream event for current client
	client->set_callback(r14p::R14P_ET_STREAM_NEW, &new_stream);
	// handle client idle
	client->set_callback(r14p::R14P_ET_CLIENT_IDLE, &client_idle);

}

NewClient::NewClient(config::Config* _config){
	config = _config;
	new_stream.config = config;
	client_idle.config = _config;
}


void StreamDone::run(r14p::R14PCallbackArgs* args){
    r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
    StreamNext* snext = (StreamNext*)stream->get_callback(r14p::R14P_ET_STREAM_NEXT);
    r14p::R14PClient* client = stream->get_client();


    // notifications
    if(ntfy_lst.size() > 0){
	for(unsigned int i = 0; i<ntfy_lst.size(); i++){
	    // r14p notification
	    config::R14PCfgNotification* r14pn = (config::R14PCfgNotification*)ntfy_lst[i];
	    // notify all users
	    unsigned int j = 0;
	    while(j < r14pn->get_user_count()){
		    // check if client is still active
		    if(client->get_session()->get_client(r14pn->get_user(j)->r14pc) != NULL){
			    // check if ready to send
			    if(r14pn->ready){
				config::notify_user(snext->new_stream->config, &r14pn->ntf_cfg_lst, r14pn->get_user(j), r14pn);
			    }
			    // next
			    ++j;

		    // client terminated, remove notification
		    }else{
			r14pn->unreg_user(r14pn->get_user(j));
		    }
	    }
	    // clear and deallocate ntf cfg node list
	    for(unsigned int i = 0; i<r14pn->ntf_cfg_lst.children.size(); i++) delete r14pn->ntf_cfg_lst.children[i];
	    r14pn->ntf_cfg_lst.children.clear();
	    r14pn->ready = false;

	}
	// clear notification list
	ntfy_lst.clear();
    }


    // unlock config mutex
    snext->new_stream->config->unlock();
    // deallocate NewStream, allocated in NewStream::run
    if(snext != NULL){
	// clear ac res list or it will be deallocated (big no no)
	snext->new_stream->ac_res.children.clear();
	// clear tmp list
	for(unsigned int i = 0; i<snext->new_stream->tmp_node_lst.children.size(); i++) delete snext->new_stream->tmp_node_lst.children[i];
	snext->new_stream->tmp_node_lst.children.clear();
	// free new stream
	delete snext->new_stream;
    }

}

NewStream::NewStream(){
	stream_next.cfg_res = NULL;
	stream_next.new_stream = NULL;
	config_action = -1;
	ac_res_count = 0;
	ca_cfg_result = asn1::ConfigAction::_ca_cfg_result;
	pt_pmink_config_ac_line = htobe32(asn1::ParameterType::_pt_pmink_config_ac_line);
	pt_pmink_config_ac_err_count = htobe32(asn1::ParameterType::_pt_pmink_config_ac_err_count);
	pt_pmink_config_cli_path = htobe32(asn1::ParameterType::_pt_pmink_config_cli_path);
	pt_pmink_config_cfg_line_count = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_line_count);
	pt_cfg_item_cm_mode = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_cm_mode);
	config = NULL;
	cm_mode = config::CONFIG_MT_UNKNOWN;
	res_index = 0;
	error_count = 0;
	ac_mode = config::CONFIG_ACM_TAB;
	line_stream_lc = 0;
	tmp_size = 0;
	last_found = NULL;
	res_size = 0;
	err_index = 0;


}

int NewStream::get_cfg_uid(config::UserId* usr_id, asn1::R14PMessage* in_msg, int sess_id){
	// null check
	if(usr_id == NULL || in_msg == NULL) return 1;
	// check for body
	if(in_msg->_body != NULL){
	    // check for config message
	    if(in_msg->_body->_conf->has_linked_data(sess_id)){
		    // check for params part
		    if(in_msg->_body->_conf->_params != NULL){
			    if(in_msg->_body->_conf->_params->has_linked_data(sess_id)){
				// process params
				for(unsigned int i = 0; i<in_msg->_body->_conf->_params->children.size(); i++){
				    // check for current session
				    if(in_msg->_body->_conf->_params->get_child(i)->has_linked_data(sess_id)){
					// check param id, convert from big endian to host
					uint32_t* param_id = (uint32_t*)in_msg->_body->_conf->_params->get_child(i)->_id->linked_node->tlv->value;
					// check for value
					if(in_msg->_body->_conf->_params->get_child(i)->_value != NULL){
						// check if value exists in current session
						if(in_msg->_body->_conf->_params->get_child(i)->_value->has_linked_data(sess_id)){
							// check if child exists
							if(in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0) != NULL){
							    // check if child exists in current sesion
							    if(in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0)->has_linked_data(sess_id)){
								// set tmp values
								char* tmp_val = (char*)in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value;
								unsigned int tmp_val_l = in_msg->_body->_conf->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value_length;

								// match param
								switch(be32toh(*param_id)){
								    // config user auth id
								    case asn1::ParameterType::_pt_pmink_auth_id:
									// user id - user auth id
									if(tmp_val_l <= sizeof(usr_id->user_id)) memcpy(usr_id->user_id, tmp_val, tmp_val_l);
									// ok
									return 0;

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
	// err
	return 1;
}



void NewStream::run(r14p::R14PCallbackArgs* args){
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);

	// create new instance of NewStream (think of it as forking)
	NewStream* new_stream = new NewStream();
	new_stream->config = config;
	new_stream->stream_next.cfg_res = &new_stream->ac_res;
	new_stream->stream_next.new_stream = new_stream;

	// set cfg user id pointer
	config::UserId* new_cfg_usr_id = &new_stream->cfg_user_id;

	// set events
	stream->set_callback(r14p::R14P_ET_STREAM_NEXT, &new_stream->stream_next);
	stream->set_callback(r14p::R14P_ET_STREAM_END, &new_stream->stream_done);
	stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, &new_stream->stream_done);

	// create cfg user id
	if(get_cfg_uid(new_cfg_usr_id, in_msg, *in_sess) > 0){
	    stream->end_sequence();
	    return;
	}


	// check for body
	if(in_msg->_body != NULL){
	    // check for ConfigMessage
	    if(in_msg->_body->_conf->has_linked_data(*in_sess)){
		// User login
		if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_user_login){
		    new_stream->config_action = asn1::ConfigAction::_ca_cfg_user_login;
		    // lock config mutex
		    config->lock();
		    // set new user
		    config::UserInfo* usr_info = new config::UserInfo(config->get_definition_root());
		    config->set_definition_wn(new_cfg_usr_id, usr_info);
		    // process
		    new_stream->process_user_login(args);
		    // config mutex unlocked in R14P_ET_STREAM_END event

		// User logout
		}else if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_user_logout){
		    new_stream->config_action = asn1::ConfigAction::_ca_cfg_user_logout;
		    // lock config mutex
		    config->lock();
		    // process
		    new_stream->process_user_logout(args);
		    // config mutex unlocked in R14P_ET_STREAM_END event


		// AC mode (TAB mode in CLI)
		}else if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_ac){
		    new_stream->ac_mode = config::CONFIG_ACM_TAB;
		    new_stream->config_action = asn1::ConfigAction::_ca_cfg_ac;
		    // lock config mutex
		    config->lock();
		    // check is user id exists
		    config->update_definition_wn(new_cfg_usr_id);
		    // process
		    new_stream->process_tab(args);
		    // config mutex unlocked in R14P_ET_STREAM_END event


		// SET mode (ENTER mode in CLI)
		}else if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_set){
		    new_stream->ac_mode = config::CONFIG_ACM_ENTER;
		    new_stream->config_action = asn1::ConfigAction::_ca_cfg_set;
		    // lock config mutex
		    config->lock();
		    // check is user id exists
		    config->update_definition_wn(new_cfg_usr_id);
		    // process
		    new_stream->process_enter(args);
		    // config mutex unlocked in R14P_ET_STREAM_END event

		// GET mode
		}else if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_get){
		    new_stream->config_action = asn1::ConfigAction::_ca_cfg_get;
		    // lock config mutex
		    config->lock();
		    // check is user id exists
		    config->update_definition_wn(new_cfg_usr_id);
		    // process
		    new_stream->process_get(args);
		    // config mutex unlocked in R14P_ET_STREAM_END event


		// REPLICATE mode (action from other config daemon)
		}else if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_replicate){
		    new_stream->config_action = asn1::ConfigAction::_ca_cfg_replicate;
		    // lock config mutex
		    config->lock();
		    // check is user id exists
		    config->update_definition_wn(new_cfg_usr_id);
		    // process
		    new_stream->process_replicate(args);
		    // config mutex unlocked in R14P_ET_STREAM_END event

		}else stream->end_sequence();
	    }else stream->end_sequence();
	}else stream->end_sequence();


}
void NewStream::process_user_logout(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);

	// check if current user started transaction
	bool pretend = (config->get_transaction_owner() != cfg_user_id && config->transaction_started() ? true : false);
	if(!pretend){
		// discard changes
		config->discard(config->get_definition_root());
		// end transaction
		config->end_transaction();

	}
	// remove user
	config->remove_wn_user(&cfg_user_id);

	// nothing more to do
	stream->end_sequence();

}


void NewStream::process_user_login(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);

	// nothing more to do
	stream->end_sequence();
}

// new stream, start sending config tree
void NewStream::process_replicate(r14p::R14PCallbackArgs* args){
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	tmp_size = 0;
	res_size = 0;
	error_count = 0;
	err_index = -1;
	last_found = NULL;
	res_index = 0;
	ac_res.children.clear();
	char* tmp_val = NULL;
	int tmp_val_l = 0;
	line.clear();
	cm_mode = config::CONFIG_MT_UNKNOWN;

	// check for body
	if(in_msg->_body != NULL){
	    // check for config message
	    if(in_msg->_body->_conf->has_linked_data(*in_sess)){
		// check for GET action
		if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_replicate){
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
								    // config item path
								    case asn1::ParameterType::_pt_pmink_config_replication_line:
									line.append(tmp_val, tmp_val_l);
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

	// check if replication line was received
	if(line.size() > 0){
		// tokenize
		pmink_utils::tokenize(&line, tmp_lst, 50, &tmp_size, true);
		// check for transaction owner
		bool pretend = (config->get_transaction_owner() != cfg_user_id && config->transaction_started() ? true : false);
		// auto complete
		config->auto_complete(	&cm_mode,
					config::CONFIG_ACM_ENTER,
					config->get_cmd_tree(),
					config->get_definition_wn(&cfg_user_id)->wnode,
					tmp_lst,
					tmp_size,
					&ac_res,
					&res_size,
					&last_found,
					&error_count,
					tmp_err,
					pretend,
					&tmp_node_lst);


		// process results
		if(ac_res.children.size() > 0){
			// delete mode
			if(cm_mode == config::CONFIG_MT_DEL){
			    // single result
			    if(ac_res.children.size() == 1){
				// item
				if(ac_res.children[0]->node_type == config::CONFIG_NT_ITEM){
				    if(!pretend){
					    // start transaction
					    config->start_transaction(&cfg_user_id);
					    // mark as deleted
					    ac_res.children[0]->node_state = config::CONFIG_NS_DELETED;

				    }

				}

			    // multiple results
			    }else if(ac_res.children.size() > 0){
				    // check if parent is block item
				    if(ac_res.children[0]->parent->node_type == config::CONFIG_NT_BLOCK){
					    config:: ConfigItem* tmp_item = ac_res.children[0]->parent;
					    // only allow deletion of template based node
					    if(tmp_item->parent->children.size() > 1){
						    // check if template
						    if(tmp_item->parent->children[0]->is_template){
							    // loop all template based nodes, try to match
							    for(unsigned int i = 1; i< tmp_item->parent->children.size(); i++) if(!tmp_item->parent->children[i]->is_template){
								if(tmp_item->parent->children[i] == tmp_item){
								    if(!pretend){
									    // start transaction
									    config->start_transaction(&cfg_user_id);
									    // mark as deleted
									    tmp_item->parent->children[i]->node_state = config::CONFIG_NS_DELETED;

								    }
								    break;
								}
							    }

						    }

					    }
				    }
			    }
			// cmd without params
			}else if(cm_mode == config::CONFIG_MT_CMD){
			    if(ac_res.children.size() >= 1){
				// cmd without params
				if(ac_res.children[0]->node_type == config::CONFIG_NT_CMD){
				    if(ac_res.children[0]->name == "discard"){
					if(!pretend){
						// set current definition path to top level
						config->reset_all_wns();
						// regenerate cli path
						cli_path = "";
						// discard
						config->discard(config->get_definition_root());
						// end transaction
						config->end_transaction();
					}


				    }

				// cmd with params
				}else if(ac_res.children[0]->node_type == config::CONFIG_NT_PARAM){
				    if(ac_res.children[0]->parent != NULL){
					    if(ac_res.children[0]->parent->node_type == config::CONFIG_NT_CMD){
						if(ac_res.children[0]->parent->name == "commit"){
						    if(!pretend){
							    if(config->commit(config->get_definition_root(), true) > 0){
								    // set current definition path to top level
								    config->reset_all_wns();
								    // regenerate cli path
								    cli_path = "";
								    // get rollback count
								    DIR* dir;
								    dirent* ent;
								    int c = 0;
								    stringstream tmp_str;

								    dir = opendir("./commit-log");
								    // if dir
								    if(dir != NULL) {
									// get dir contents
									while ((ent = readdir(dir)) != NULL) {
									    if(strncmp(ent->d_name, ".rollback", 9) == 0) ++c;

									}
									// close dir
									closedir(dir);
								    }


								    tmp_str << "./commit-log/.rollback." << c << ".pmcfg";
								    // save rollback
								    std::ofstream ofs(tmp_str.str().c_str(), std::ios::out | std::ios::binary);
								    if(ofs.is_open()){
									    // save current config excluding uncommitted changes
									    config->show_config(config->get_definition_root(), 0, &tmp_size, false, &ofs, true, &ac_res.children[0]->new_value);
									    ofs.close();

									    // prepare notifications
									    prepare_notifications();

									    // commit new configuration
									    config->commit(config->get_definition_root(), false);

									    // sort
									    config->sort(config->get_definition_root());

									    // end transaction
									    config->end_transaction();

									    // update current configuration contents file
									    std::ofstream orig_fs(((std::string*)pmink::CURRENT_DAEMON->get_param(1))->c_str(), std::ios::out | std::ios::binary);
									    if(orig_fs.is_open()){
										    config->show_config(config->get_definition_root(), 0, &tmp_size, false, &orig_fs, false, NULL);
										    orig_fs.close();

									    }


								    }


							    }
							    // clear value
							    ac_res.children[0]->new_value = "";

						    }

						    // rollback
						    }else if(ac_res.children[0]->parent->name == "rollback"){
							if(!pretend){
							    if(ac_res.children[0]->new_value != ""){
								    std::string tmp_path;
								    std::stringstream istr(ac_res.children[0]->new_value);
								    int rev_num = -1;
								    istr >> rev_num;
								    bool rev_found = false;
								    dirent** fnames;


								    int n = scandir("./commit-log/", &fnames, pmink_utils::_ac_rollback_revision_filter, pmink_utils::_ac_rollback_revision_sort);
								    if(n > 0){
									for(int i = 0; i<n; i++){
										if(rev_num == i){
										    rev_found = true;
										    tmp_path = "./commit-log/";
										    tmp_path.append(fnames[i]->d_name);
										}
										free(fnames[i]);

									}
									free(fnames);

									// if revision found
									if(rev_found){
									    tmp_size = 0;
									    // err check
									    tmp_size = pmink_utils::get_file_size(tmp_path.c_str());
									    if(tmp_size == 0){

										// nothing

									    }else{
										char* tmp_file_buff = new char[tmp_size + 1];
										bzero(tmp_file_buff, tmp_size + 1);
										pmink_utils::load_file(tmp_path.c_str(), tmp_file_buff, &tmp_size);

										antlr::PMinkParser* pmp = antlr::create_parser();
										pANTLR3_INPUT_STREAM input = pmp->input;
										ppminkLexer lxr = pmp->lexer;
										pANTLR3_COMMON_TOKEN_STREAM tstream = pmp->tstream;
										ppminkParser psr = pmp->parser;
										pminkParser_inputConfig_return_struct ast_cfg;
										config::ConfigItem* cfg_cnt = new config::ConfigItem();

										// reset error state
										lxr->pLexer->rec->state->errorCount = 0;
										psr->pParser->rec->state->errorCount = 0;
										input->reuse(input, (unsigned char*)tmp_file_buff, tmp_size, (unsigned char*)"file_stream");

										// token stream
										tstream->reset(tstream);
										// ast
										ast_cfg = psr->inputConfig(psr);
										// err check
										int err_c = lxr->pLexer->rec->getNumberOfSyntaxErrors(lxr->pLexer->rec);
										err_c += psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec);
										if(err_c > 0){
											// nothing
										}else{

											// set current definition path to top level
											config->reset_all_wns();
											// regenerate cli path
											cli_path = "";

											// get structure
											antlr::process_config(ast_cfg.tree, cfg_cnt);
											// validate
											if(config->validate(config->get_definition_root(), cfg_cnt)){

												// prepare for config data replacement
												config->replace_prepare(config->get_definition_root());
												// merge new data
												int res = config->merge(config->get_definition_root(), cfg_cnt, true);
												// err check
												if(res != 0){
													// nothing
												}else{
													// prepare notifications
													prepare_notifications();

													// commit new config
													config->commit(config->get_definition_root(), false);

													// update current configuration contents file
													std::ofstream orig_fs(((std::string*)pmink::CURRENT_DAEMON->get_param(1))->c_str(), std::ios::out | std::ios::binary);
													if(orig_fs.is_open()){
														config->show_config(config->get_definition_root(), 0, &tmp_size, false, &orig_fs, false, NULL);
														orig_fs.close();

													}
													// sort
													config->sort(config->get_definition_root());

													// end transaction
													config->end_transaction();

												}
											}

										}
										// free mem
										delete cfg_cnt;
										delete[] tmp_file_buff;
										antlr::free_mem(pmp);

									    }

									}
								    }
								}

								// clear value
								ac_res.children[0]->new_value = "";
							}

						}
					    }
				    }
				}
			    }
			}else if(cm_mode == config::CONFIG_MT_SET){
			    // do nothing, errors already present in tmp_err
			    if(!pretend){
				config->start_transaction(&cfg_user_id);
			    }
			}


		}
		// free temp nodes and clear res buffer
		ac_res.children.clear();
		for(unsigned int i = 0; i<tmp_node_lst.children.size(); i++) delete tmp_node_lst.children[i];
		tmp_node_lst.children.clear();

	}
}

// new stream, start sending config tree
void NewStream::process_get(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	r14p::R14PClient* client = stream->get_client();
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	tmp_size = 0;
	res_size = 0;
	error_count = 0;
	err_index = -1;
	last_found = NULL;
	res_index = 0;
	ac_res.children.clear();
	char* tmp_val = NULL;
	int tmp_val_l = 0;
	bool cfg_notify = false;
	line.clear();

	// check for body
	if(in_msg->_body != NULL){
	    // check for config message
	    if(in_msg->_body->_conf->has_linked_data(*in_sess)){
		// check for GET action
		if(in_msg->_body->_conf->_action->linked_node->tlv->value[0] == asn1::ConfigAction::_ca_cfg_get){
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
								    // config item path
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_path:
									line.append(tmp_val, tmp_val_l);
									break;

								    // config item notification flag
								    case asn1::ParameterType::_pt_pmink_config_cfg_item_notify:
									//tmp_ivp = (uint32_t*)tmp_val;
									cfg_notify = (tmp_val[0] == 1);
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

	// check if config item path item was received
	if(line.size() == 0){
	    stream->end_sequence();
	    return;
	}

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


	// check if user requested node exists
	config::ConfigItem* tmp_cfg_root = (*config->get_definition_root())(line.c_str());
	// node exists
	if(tmp_cfg_root != NULL){
		// set node notification
		if(cfg_notify){
		    // check if notification exists
		    config::CfgNotification* cfg_ntf = config->get_notification(&line);
		    if(cfg_ntf == NULL){
			    // create new notification handler
			    cfg_ntf = new config::R14PCfgNotification(&line);
			    config->add_notification(cfg_ntf);

		    }

		    // create ntf user id
		    config::R14PCfgNtfUser ntf_usr(client);
		    if(in_msg->_header->_source->_id != NULL){
			if(in_msg->_header->_source->_id->has_linked_data(*in_sess)){
			    tmp_val = (char*)in_msg->_header->_source->_id->linked_node->tlv->value;
			    tmp_val_l = in_msg->_header->_source->_id->linked_node->tlv->value_length;
			    std::string tmp_str(tmp_val, tmp_val_l);
			    // set registered user id
			    if(tmp_str.size() <= sizeof(ntf_usr.user_id)) memcpy(ntf_usr.user_id, tmp_str.c_str(), tmp_str.size());
			}
		    }
		    // set registered user type
		    tmp_val = (char*)in_msg->_header->_source->_type->linked_node->tlv->value;
		    tmp_val_l = in_msg->_header->_source->_type->linked_node->tlv->value_length;
		    std::string tmp_str(tmp_val, tmp_val_l);
		    if(tmp_str.size() <= sizeof(ntf_usr.user_type)) memcpy(ntf_usr.user_type, tmp_str.c_str(), tmp_str.size());


		    // unregister user if previously registered
		    cfg_ntf->unreg_user(&ntf_usr);
		    // register new node user
		    cfg_ntf->reg_user(&ntf_usr);

		}

		// flatten node structure to list
		config::Config::flatten(tmp_cfg_root, &ac_res);
		// get result size, convert to big endian
		ac_res_count = htobe32(ac_res.children.size());

		// set result action
		r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&ca_cfg_result, 1);
		// cfg item count
		r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&stream_next.pt_cfg_item_count, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&ac_res_count, sizeof(uint32_t));

		// include bodu
		*include_body = true;
		// continue
		stream->continue_sequence();

		if(ac_res.children.size() == 0) stream->end_sequence();

	// node does not exist
	}else{
		// node does not exist
		stream->end_sequence();
	}
}


// prepare notifictions
void NewStream::prepare_notifications(){
    // ****** notify *************
    // flatten node structure to list
    config::ConfigItem tmp_res;
    config::Config::flatten(config->get_definition_root(), &tmp_res);
    // remove unmodified nodes, only interested in modified nodes
    unsigned int i = 0;
    while(i < tmp_res.children.size()){
	if(tmp_res.children[i]->node_state == config::CONFIG_NS_READY) tmp_res.children.erase(tmp_res.children.begin() + i);
	else ++i;
    }

    config::ConfigItem* tmp_item = NULL;
    bool exists = false;
    std::string tmp_full_path;
    std::string tmp_str;
    // loop list
    for(unsigned int i = 0; i<tmp_res.children.size(); i++){
	tmp_item = tmp_res.children[i];
	// check if node has been modified
	if(tmp_item->node_state != config::CONFIG_NS_READY){
	    // get full node path
	    config::Config::get_parent_line(tmp_item, &tmp_full_path);
	    tmp_full_path.append(tmp_item->name);

	    // get users (check parent nodes)
	    while(tmp_item != NULL){
		// get notification
		config::Config::get_parent_line(tmp_item, &tmp_str);
		tmp_str.append(tmp_item->name);
		config::CfgNotification* cfg_ntf = config->get_notification(&tmp_str);
		// user found
		if(cfg_ntf != NULL){
		    // r14p notification
		    config::R14PCfgNotification* r14pn = (config::R14PCfgNotification*)cfg_ntf;
		    // add to notification list
		    config::ConfigItem* new_cfg_item = new config::ConfigItem();
		    new_cfg_item->name = tmp_full_path;
		    new_cfg_item->value = tmp_res.children[i]->new_value;
		    new_cfg_item->node_state = tmp_res.children[i]->node_state;
		    new_cfg_item->node_type = tmp_res.children[i]->node_type;
		    r14pn->ntf_cfg_lst.children.push_back(new_cfg_item);
		    // ready to send
		    r14pn->ready = true;

		    // check if client was already added to notification client list
		    exists = false;
		    for(unsigned int j = 0; j<stream_done.ntfy_lst.size(); j++) if(stream_done.ntfy_lst[j] == r14pn){
			exists = true;
			break;
		    }
		    // add to list if needed, notifications processed after commit (Stream done event)
		    if(!exists) stream_done.ntfy_lst.push_back(r14pn);

		}
		// level up (parent)
		tmp_item = tmp_item->parent;

	    }

	}
    }

    tmp_res.children.clear();
    // ****** notify *************

}

// new stream, send auto completed line in first packet
void NewStream::process_enter(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	r14p::R14PClient* client = stream->get_client();
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	tmp_size = 0;
	res_size = 0;
	error_count = 0;
	err_index = -1;
	last_found = NULL;
	res_index = 0;
	line_stream_lc = 0;
	cm_mode = config::CONFIG_MT_UNKNOWN;
	tmp_node_lst.children.clear();
	ac_res.children.clear();
	line_stream.str("");
	line_stream.clear();
	line_stream.seekg(0, std::ios::beg);
	char* tmp_val = NULL;
	int tmp_val_l = 0;

	// current path
	cli_path = "";
	config->generate_path(config->get_definition_wn(&cfg_user_id)->wnode, &cli_path);

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
		for(int i = 0; i<5; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	// unlink params before setting new ones
	}else{
	    int cc = r14pm->_body->_conf->_params->children.size();
	    if(cc < 5){
		// set children, allocate more
		for(int i = cc; i<5; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	    }else if(cc > 5){
		// remove extra children if used in some other session, only 2 needed
		for(int i = 5; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
	    }
	}

	// get line
	if(in_msg->_body != NULL){
	    // check for config message
	    if(in_msg->_body->_conf->has_linked_data(*in_sess)){
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
								    // config item count
								    case asn1::ParameterType::_pt_pmink_config_ac_line:
									line.clear();
									line.append(tmp_val, tmp_val_l);
									pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,  "cmd line received: [%s]", line.c_str());
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

	// tokenize
	pmink_utils::tokenize(&line, tmp_lst, 50, &tmp_size, true);
	// pretend
	bool pretend = (config->get_transaction_owner() != cfg_user_id && config->transaction_started() ? true : false);

	// auto complete
	config->auto_complete(	&cm_mode,
				config::CONFIG_ACM_ENTER,
				config->get_cmd_tree(),
				config->get_definition_wn(&cfg_user_id)->wnode,
				tmp_lst,
				tmp_size,
				&ac_res,
				&res_size,
				&last_found,
				&error_count,
				tmp_err,
				pretend,
				&tmp_node_lst);

	if(pretend){
	    switch(cm_mode){
		case config::CONFIG_MT_SET:
		case config::CONFIG_MT_EDIT:
		case config::CONFIG_MT_DEL:
		    tmp_err[error_count].clear();
		    tmp_err[error_count].assign("Transaction started by other user, cannot execute intrusive operation!");
		    ++error_count;
		    break;

		default: break;
	    }
	}

	// replace current line with auto completed values
	line.clear();
	for(int i = 0; i<tmp_size; i++){
		line.append(tmp_lst[i]);
		if(i < res_size) line.append(" "); else break;
	}



	// process results
	if(ac_res.children.size() > 0){
		// delete mode
		if(cm_mode == config::CONFIG_MT_DEL){
		    // single result
		    if(ac_res.children.size() == 1){
			// check if single item is really only one ITEM node (last one from input and one from ac_res should match)
			if(tmp_lst[tmp_size - 1] == ac_res.children[ac_res.children.size() - 1]->name){
			    // item
			    if(ac_res.children[0]->node_type == config::CONFIG_NT_ITEM){
				if(!pretend){
					// start transaction
					config->start_transaction(&cfg_user_id);
					// mark as deleted
					ac_res.children[0]->node_state = config::CONFIG_NS_DELETED;
					// replicate
					std::vector<std::string*>* cfg_daemons = (std::vector<std::string*>*)pmink::CURRENT_DAEMON->get_param(2);
					for(unsigned int i = 0; i<cfg_daemons->size(); i++){
						config::replicate(line.c_str(), client->get_session()->get_registered_client("routingd"), (*cfg_daemons)[i]->c_str(), &cfg_user_id);

					}


				}

			    }else if(ac_res.children[0]->node_type == config::CONFIG_NT_BLOCK){
				    tmp_err[error_count].clear();
				    tmp_err[error_count].append("Cannot delete non template block node \"");
				    tmp_err[error_count].append(ac_res.children[0]->parent->name);
				    tmp_err[error_count].append("\"!");
				    ++error_count;

			    }
			// check if single item is in fact only single child of template node that was set for deletion
			}else{
			    // check if parent is block item
			    if(ac_res.children[0]->parent->node_type == config::CONFIG_NT_BLOCK){
				    config::ConfigItem* tmp_item = ac_res.children[0]->parent;
				    // only allow deletion of template based node
				    if(tmp_item->parent != NULL){
					    if(tmp_item->parent->children.size() > 1){
						    // check if template
						    if(tmp_item->parent->children[0]->is_template){
							    // loop all template based nodes, try to match
							    for(unsigned int i = 1; i< tmp_item->parent->children.size(); i++) if(!tmp_item->parent->children[i]->is_template){
								if(tmp_item->parent->children[i] == tmp_item){
								    if(!pretend){
									    // start transaction
									    config->start_transaction(&cfg_user_id);
									    // mark as deleted
									    tmp_item->parent->children[i]->node_state = config::CONFIG_NS_DELETED;
									    // replicate
									    std::vector<std::string*>* cfg_daemons = (std::vector<std::string*>*)pmink::CURRENT_DAEMON->get_param(2);
									    for(unsigned int i = 0; i<cfg_daemons->size(); i++){
										    config::replicate(line.c_str(), client->get_session()->get_registered_client("routingd"), (*cfg_daemons)[i]->c_str(), &cfg_user_id);

									    }

								    }
								    break;
								}
							    }

						    }else{
							tmp_err[error_count].clear();
							tmp_err[error_count].append("Cannot delete non template block node \"");
							tmp_err[error_count].append(ac_res.children[0]->parent->name);
							tmp_err[error_count].append("\"!");
							++error_count;
						    }

					    }else{
						tmp_err[error_count].clear();
						tmp_err[error_count].append("Cannot delete non template block node \"");
						tmp_err[error_count].append(ac_res.children[0]->parent->name);
						tmp_err[error_count].append("\"!");
						++error_count;
					    }
				    }
			    }



			}




		    // multiple results
		    }else if(ac_res.children.size() > 0){

			    // check if parent is block item
			    if(ac_res.children[0]->parent->node_type == config::CONFIG_NT_BLOCK){
				    config::ConfigItem* tmp_item = ac_res.children[0]->parent;
				    // only allow deletion of template based node
				    if(tmp_item->parent != NULL){
					    if(tmp_item->parent->children.size() > 1){
						    // check if template
						    if(tmp_item->parent->children[0]->is_template){
							    // loop all template based nodes, try to match
							    for(unsigned int i = 1; i< tmp_item->parent->children.size(); i++) if(!tmp_item->parent->children[i]->is_template){
								if(tmp_item->parent->children[i] == tmp_item){
								    if(!pretend){
									    // start transaction
									    config->start_transaction(&cfg_user_id);
									    // mark as deleted
									    tmp_item->parent->children[i]->node_state = config::CONFIG_NS_DELETED;
									    // replicate
									    std::vector<std::string*>* cfg_daemons = (std::vector<std::string*>*)pmink::CURRENT_DAEMON->get_param(2);
									    for(unsigned int i = 0; i<cfg_daemons->size(); i++){
										    config::replicate(line.c_str(), client->get_session()->get_registered_client("routingd"), (*cfg_daemons)[i]->c_str(), &cfg_user_id);

									    }

								    }
								    break;
								}
							    }

						    }else{
							tmp_err[error_count].clear();
							tmp_err[error_count].append("Cannot delete non template block node \"");
							tmp_err[error_count].append(ac_res.children[0]->parent->name);
							tmp_err[error_count].append("\"!");
							++error_count;
						    }

					    }else{
						tmp_err[error_count].clear();
						tmp_err[error_count].append("Cannot delete non template block node \"");
						tmp_err[error_count].append(ac_res.children[0]->parent->name);
						tmp_err[error_count].append("\"!");
						++error_count;
					    }
				    }
			    }
		    }
		// special commands
		}else if(cm_mode == config::CONFIG_MT_CMD){
		    if(ac_res.children.size() >= 1){
			// cmd without params
			if(ac_res.children[0]->node_type == config::CONFIG_NT_CMD){
				if(ac_res.children[0]->name == "configuration"){
				    int tmp_size = 0;
				    // get date size
				    line_stream_lc = config->get_config_lc(config->get_definition_wn(&cfg_user_id)->wnode);
				    // get data
				    config->show_config(config->get_definition_wn(&cfg_user_id)->wnode, 0, &tmp_size, false, &line_stream);

				}else if(ac_res.children[0]->name == "commands"){
				    // get date size
				    line_stream_lc = config->get_commands_lc(config->get_definition_wn(&cfg_user_id)->wnode);
				    // get data
				    config->show_commands(config->get_definition_wn(&cfg_user_id)->wnode, 0, &line_stream);

				}else if(ac_res.children[0]->name == "top"){
				    if(!pretend){
					    // set current cfg path
					    config->get_definition_wn(&cfg_user_id)->wnode = config->get_definition_root();

					    // regenerate cli path
					    cli_path = "";

				    }else{
					    tmp_err[error_count].clear();
					    tmp_err[error_count].assign("Transaction started by other user, cannot execute intrusive operation!");
					    ++error_count;

				    }


				}else if(ac_res.children[0]->name == "up"){
				    if(!pretend){
					    if(config->get_definition_wn(&cfg_user_id)->wnode->parent != NULL){
						// set current cfg path
						config->get_definition_wn(&cfg_user_id)->wnode = config->get_definition_wn(&cfg_user_id)->wnode->parent;

						// regenerate cli path
						cli_path = "";

						// generate cfg path
						config->generate_path(config->get_definition_wn(&cfg_user_id)->wnode, &cli_path);


					    }

				    }else{
					    tmp_err[error_count].clear();
					    tmp_err[error_count].assign("Transaction started by other user, cannot execute intrusive operation!");
					    ++error_count;

				    }
				}else if(ac_res.children[0]->name == "discard"){
				    if(!pretend){
					    // set current definition path to top level
					    config->reset_all_wns();

					    // regenerate cli path
					    cli_path = "";

					    // generate prompt
					    config->discard(config->get_definition_root());

					    // end transaction
					    config->end_transaction();

					    // replicate
					    std::vector<std::string*>* cfg_daemons = (std::vector<std::string*>*)pmink::CURRENT_DAEMON->get_param(2);
					    for(unsigned int i = 0; i<cfg_daemons->size(); i++){
						    config::replicate(line.c_str(), client->get_session()->get_registered_client("routingd"), (*cfg_daemons)[i]->c_str(), &cfg_user_id);

					    }


				    }else{
					    tmp_err[error_count].clear();
					    tmp_err[error_count].assign("Transaction started by other user, cannot execute intrusive operation!");
					    ++error_count;
				    }


				}
			// cmd with params
			}else if(ac_res.children[0]->node_type == config::CONFIG_NT_PARAM){

			    if(ac_res.children[0]->parent != NULL){
				    if(ac_res.children[0]->parent->node_type == config::CONFIG_NT_CMD){
					if(ac_res.children[0]->parent->name == "commit"){
					    if(!pretend){
						    if(config->commit(config->get_definition_root(), true) > 0){
							    // set current definition path to top level
							    config->reset_all_wns();

							    // regenerate cli path
							    cli_path = "";

							    // generate prompt

							    // get rollback count
							    DIR* dir;
							    dirent* ent;
							    int c = 0;
							    stringstream tmp_str;

							    dir = opendir("./commit-log");
							    // if dir
							    if(dir != NULL) {
								// get dir contents
								while ((ent = readdir(dir)) != NULL) {
								    if(strncmp(ent->d_name, ".rollback", 9) == 0) ++c;

								}
								// close dir
								closedir(dir);
							    }


							    tmp_str << "./commit-log/.rollback." << c << ".pmcfg";
							    // save rollback
							    std::ofstream ofs(tmp_str.str().c_str(), std::ios::out | std::ios::binary);
							    if(ofs.is_open()){
								    // save current config excluding uncommitted changes
								    config->show_config(config->get_definition_root(), 0, &tmp_size, false, &ofs, true, &ac_res.children[0]->new_value);
								    ofs.close();

								    // prepare notifications
								    prepare_notifications();

								    // commit new configuration
								    config->commit(config->get_definition_root(), false);

								    // sort
								    config->sort(config->get_definition_root());

								    // update current configuration contents file
								    std::ofstream orig_fs(((std::string*)pmink::CURRENT_DAEMON->get_param(1))->c_str(), std::ios::out | std::ios::binary);
								    if(orig_fs.is_open()){
									    config->show_config(config->get_definition_root(), 0, &tmp_size, false, &orig_fs, false, NULL);
									    orig_fs.close();

								    }

								    // end transaction
								    config->end_transaction();

								    // replicate
								    std::vector<std::string*>* cfg_daemons = (std::vector<std::string*>*)pmink::CURRENT_DAEMON->get_param(2);
								    for(unsigned int i = 0; i<cfg_daemons->size(); i++){
									    config::replicate(line.c_str(), client->get_session()->get_registered_client("routingd"), (*cfg_daemons)[i]->c_str(), &cfg_user_id);

								    }

							    }else{
								    tmp_err[error_count].clear();
								    tmp_err[error_count].assign("Cannot create rollback configuration!");
								    ++error_count;
							    }

						    }
						    // clear value
						    ac_res.children[0]->new_value = "";
					    }else{
						    tmp_err[error_count].clear();
						    tmp_err[error_count].assign("Transaction started by other user, cannot execute intrusive operation!");
						    ++error_count;
					    }




				    // rollback
				    }else if(ac_res.children[0]->parent->name == "rollback"){
					if(!pretend){
					    if(ac_res.children[0]->new_value != ""){
						    std::string tmp_path;
						    std::stringstream istr(ac_res.children[0]->new_value);
						    int rev_num = -1;
						    istr >> rev_num;
						    bool rev_found = false;
						    dirent** fnames;


						    int n = scandir("./commit-log/", &fnames, pmink_utils::_ac_rollback_revision_filter, pmink_utils::_ac_rollback_revision_sort);
						    if(n > 0){
							for(int i = 0; i<n; i++){
								if(rev_num == i){
								    rev_found = true;
								    tmp_path = "./commit-log/";
								    tmp_path.append(fnames[i]->d_name);
								}
								free(fnames[i]);

							}
							free(fnames);

							// if revision found
							if(rev_found){
							    tmp_size = 0;
							    // err check
							    tmp_size = pmink_utils::get_file_size(tmp_path.c_str());
							    if(tmp_size == 0){
								tmp_err[error_count].clear();
								tmp_err[error_count].append("Cannot find rollback revision '");
								tmp_err[error_count].append(tmp_path);
								tmp_err[error_count].append("'!");
								++error_count;

							    }else{
								char* tmp_file_buff = new char[tmp_size + 1];
								bzero(tmp_file_buff, tmp_size + 1);
								pmink_utils::load_file(tmp_path.c_str(), tmp_file_buff, &tmp_size);
								line_stream << "Loading rollback configuration..." << std::endl;
								++line_stream_lc;

								antlr::PMinkParser* pmp = antlr::create_parser();
								pANTLR3_INPUT_STREAM input = pmp->input;
								ppminkLexer lxr = pmp->lexer;
								pANTLR3_COMMON_TOKEN_STREAM tstream = pmp->tstream;
								ppminkParser psr = pmp->parser;
								pminkParser_inputConfig_return_struct ast_cfg;
								config::ConfigItem* cfg_cnt = new config::ConfigItem();

								// reset error state
								lxr->pLexer->rec->state->errorCount = 0;
								psr->pParser->rec->state->errorCount = 0;
								input->reuse(input, (unsigned char*)tmp_file_buff, tmp_size, (unsigned char*)"file_stream");

								// token stream
								tstream->reset(tstream);
								// ast
								ast_cfg = psr->inputConfig(psr);
								// err check
								int err_c = lxr->pLexer->rec->getNumberOfSyntaxErrors(lxr->pLexer->rec);
								err_c += psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec);
								if(err_c > 0){
									tmp_err[error_count].clear();
									tmp_err[error_count].assign("Invalid rollback configuration file syntax!");
									++error_count;
								}else{
									line_stream << "Done" << std::endl;
									++line_stream_lc;

									// set current definition path to top level
									config->reset_all_wns();

									// regenerate cli path
									cli_path = "";

									// get structure
									antlr::process_config(ast_cfg.tree, cfg_cnt);
									// validate
									if(config->validate(config->get_definition_root(), cfg_cnt)){

										// prepare for config data replacement
										config->replace_prepare(config->get_definition_root());
										// merge new data
										line_stream << "Merging rollback configuration file..." << std::endl;
										++line_stream_lc;
										int res = config->merge(config->get_definition_root(), cfg_cnt, true);
										// err check
										if(res != 0){
											tmp_err[error_count].clear();
											tmp_err[error_count].assign("Cannot merge configuration file contents!");
											++error_count;
										}else{
											line_stream << "Done" << std::endl;
											line_stream << "Committing rollback configuration..." << std::endl;
											line_stream_lc += 2;

											// prepare notifications
											prepare_notifications();

											// commit new config
											config->commit(config->get_definition_root(), false);

											// update current configuration contents file
											std::ofstream orig_fs(((std::string*)pmink::CURRENT_DAEMON->get_param(1))->c_str(), std::ios::out | std::ios::binary);
											if(orig_fs.is_open()){
												config->show_config(config->get_definition_root(), 0, &tmp_size, false, &orig_fs, false, NULL);
												orig_fs.close();

											}

											// sort
											config->sort(config->get_definition_root());

											// end transaction
											config->end_transaction();

											// replicate
											std::vector<std::string*>* cfg_daemons = (std::vector<std::string*>*)pmink::CURRENT_DAEMON->get_param(2);
											for(unsigned int i = 0; i<cfg_daemons->size(); i++){
												config::replicate(line.c_str(), client->get_session()->get_registered_client("routingd"), (*cfg_daemons)[i]->c_str(), &cfg_user_id);

											}
											line_stream << "Done" << std::endl;
											++line_stream_lc;

										}


									// err
									}else{
									    tmp_err[error_count].clear();
									    tmp_err[error_count].assign("Invalid/undefined rollback configuration file contents!");
									    ++error_count;
									}

								}
								// free mem
								delete cfg_cnt;
								delete[] tmp_file_buff;
								antlr::free_mem(pmp);

							    }


						    }else{
							    tmp_err[error_count].clear();
							    tmp_err[error_count].append("Cannot find rollback revision '");
							    tmp_err[error_count].append(ac_res.children[0]->new_value);
							    tmp_err[error_count].append("'!");
							    ++error_count;
						    }


						    }else{
							tmp_err[error_count].clear();
							tmp_err[error_count].assign("Cannot find rollback information!");
							++error_count;
						    }





					    }else{
						tmp_err[error_count].clear();
						tmp_err[error_count].assign("Rollback revision not defined!");
						++error_count;
					    }

					    // clear value
					    ac_res.children[0]->new_value = "";
					}else{
					    tmp_err[error_count].clear();
					    tmp_err[error_count].assign("Transaction started by other user, cannot execute intrusive operation!");
					    ++error_count;
					}


					// save conf
					}else if(ac_res.children[0]->parent->name == "save"){
					    if(ac_res.children[0]->new_value != ""){
						    std::ofstream ofs(ac_res.children[0]->new_value.c_str(), std::ios::out | std::ios::binary);
						    if(ofs.is_open()){
							    line_stream << "Saving configuration to \"" << ac_res.children[0]->new_value << "\"..." << std::endl;
							    ++line_stream_lc;
							    config->show_config(config->get_definition_wn(&cfg_user_id)->wnode, 0, &tmp_size, false, &ofs, false, NULL);
							    ofs.close();
							    line_stream << "Done" << std::endl;
							    ++line_stream_lc;

						    }else{
							tmp_err[error_count].clear();
							tmp_err[error_count].append("Cannot create file \"");
							tmp_err[error_count].append(ac_res.children[0]->new_value);
							tmp_err[error_count].append("\"");
							++error_count;
						    }

						    // clear value
						    ac_res.children[0]->new_value = "";

					    }else{
						tmp_err[error_count].clear();
						tmp_err[error_count].assign("Filename not defined!");
						++error_count;
					    }

					// load conf
					}else if(ac_res.children[0]->parent->name == "load"){
					    if(!pretend){
						    if(ac_res.children[0]->new_value != ""){

							    tmp_size = 0;
							    // err
							    tmp_size = pmink_utils::get_file_size(ac_res.children[0]->new_value.c_str());
							    if(tmp_size == 0){
								tmp_err[error_count].clear();
								tmp_err[error_count].append("Cannot find file \"");
								tmp_err[error_count].append(ac_res.children[0]->new_value);
								tmp_err[error_count].append("\"");
								++error_count;

							    }else{
								char* tmp_file_buff = new char[tmp_size + 1];
								bzero(tmp_file_buff, tmp_size + 1);
								pmink_utils::load_file(ac_res.children[0]->new_value.c_str(), tmp_file_buff, &tmp_size);
								line_stream << "Loading new configuration file \"" << ac_res.children[0]->new_value << "\"..." << std::endl;
								++line_stream_lc;

								antlr::PMinkParser* pmp = antlr::create_parser();
								pANTLR3_INPUT_STREAM input = pmp->input;
								ppminkLexer lxr = pmp->lexer;
								pANTLR3_COMMON_TOKEN_STREAM tstream = pmp->tstream;
								ppminkParser psr = pmp->parser;
								pminkParser_inputConfig_return_struct ast_cfg;
								config::ConfigItem* cfg_cnt = new config::ConfigItem();

								// reset error state
								lxr->pLexer->rec->state->errorCount = 0;
								psr->pParser->rec->state->errorCount = 0;
								input->reuse(input, (unsigned char*)tmp_file_buff, tmp_size, (unsigned char*)"file_stream");

								// token stream
								tstream->reset(tstream);
								// ast
								ast_cfg = psr->inputConfig(psr);
								// err check
								int err_c = lxr->pLexer->rec->getNumberOfSyntaxErrors(lxr->pLexer->rec);
								err_c += psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec);
								if(err_c > 0){
									tmp_err[error_count].clear();
									tmp_err[error_count].assign("Invalid configuration file syntax!");
									++error_count;
								}else{
									line_stream << "Done" << std::endl;
									++line_stream_lc;

									// get structure
									antlr::process_config(ast_cfg.tree, cfg_cnt);
									// validate
									if(config->validate(config->get_definition_root(), cfg_cnt)){
											// prepare for config data replacement
											config->replace_prepare(config->get_definition_root());
											// merge new data
											line_stream << "Merging new configuration file..." << std::endl;
											++line_stream_lc;
											int res = config->merge(config->get_definition_root(), cfg_cnt, true);
											// err check
											if(res != 0){
												tmp_err[error_count].clear();
												tmp_err[error_count].assign("Cannot merge configuration file contents!");
												++error_count;
											}else{
												line_stream << "Done" << std::endl;
												++line_stream_lc;

												// start transaction
												config->start_transaction(&cfg_user_id);

											}

									// err
									}else{
									    tmp_err[error_count].clear();
									    tmp_err[error_count].assign("Invalid/undefined configuration file contents!");
									    ++error_count;
									}



								}
								// free mem
								delete cfg_cnt;
								delete[] tmp_file_buff;
								antlr::free_mem(pmp);
							    }
							    // clear value
							    ac_res.children[0]->new_value = "";


						    }else{
							tmp_err[error_count].clear();
							tmp_err[error_count].assign("Filename not defined!");
							++error_count;
						    }
					    }else{
						    tmp_err[error_count].clear();
						    tmp_err[error_count].assign("Transaction started by other user, cannot execute intrusive operation!");
						    ++error_count;
					    }

					}
				    }

			    }
			}

		    }
		// edit mode
		}else if(cm_mode == config::CONFIG_MT_EDIT){
		    if(last_found != NULL){
			if(last_found->node_type == config::CONFIG_NT_BLOCK){
			    if(!pretend){
				// set current cfg path
				config->get_definition_wn(&cfg_user_id)->wnode = last_found;

				// regenerate cli path
				cli_path = "";

				// generate cfg path
				config->generate_path(config->get_definition_wn(&cfg_user_id)->wnode, &cli_path);
			    }

			}else{
				tmp_err[error_count].clear();
				tmp_err[error_count].append("Cannot navigate to non block mode \"");
				tmp_err[error_count].append(last_found->name);
				tmp_err[error_count].append("\"!");
				++error_count;
			}
		    }

		// show mode
		}else if(cm_mode == config::CONFIG_MT_SHOW){
			// sitch to TAB mode, send config items
			ac_mode = config::CONFIG_ACM_TAB;

		// set mode
		}else if(cm_mode == config::CONFIG_MT_SET){
		    // do nothing, errors already present in tmp_err
		    if(!pretend){
			config->start_transaction(&cfg_user_id);
			// replicate
			std::vector<std::string*>* cfg_daemons = (std::vector<std::string*>*)pmink::CURRENT_DAEMON->get_param(2);
			for(unsigned int i = 0; i<cfg_daemons->size(); i++){
				config::replicate(line.c_str(), client->get_session()->get_registered_client("routingd"), (*cfg_daemons)[i]->c_str(), &cfg_user_id);

			}
		    }
		}
	}


	// set values
	r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&ca_cfg_result, 1);
	// ac lline
	r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pt_pmink_config_ac_line, sizeof(int));
	r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)line.c_str(), line.size());

	// cli path
	r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&pt_pmink_config_cli_path, sizeof(int));
	r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)cli_path.c_str(), cli_path.size());

	// error count
	err_index = error_count - 1;
	error_count = htobe32(error_count);
	r14pm->_body->_conf->_params->get_child(2)->_id->set_linked_data(1, (unsigned char*)&pt_pmink_config_ac_err_count, sizeof(int));
	r14pm->_body->_conf->_params->get_child(2)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&error_count, sizeof(int));

	// line count
	line_stream_lc = htobe32(line_stream_lc);
	r14pm->_body->_conf->_params->get_child(3)->_id->set_linked_data(1, (unsigned char*)&pt_pmink_config_cfg_line_count, sizeof(int));
	r14pm->_body->_conf->_params->get_child(3)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&line_stream_lc, sizeof(int));

	// cm mode
	r14pm->_body->_conf->_params->get_child(4)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_cm_mode, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(4)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&cm_mode, 1);

	// include body
	*include_body = true;
	// continue
	stream->continue_sequence();
}

// new stream, send auto completed line in first packet
void NewStream::process_tab(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
	tmp_size = 0;
	res_size = 0;
	error_count = 0;
	err_index = -1;
	last_found = NULL;
	res_index = 0;
	cm_mode = config::CONFIG_MT_UNKNOWN;
	tmp_node_lst.children.clear();
	ac_res.children.clear();
	line_stream.str("");
	line_stream.clear();
	line_stream.seekg(0, std::ios::beg);
	char* tmp_val = NULL;
	int tmp_val_l = 0;


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


	// get line
	if(in_msg->_body != NULL){
	    // check for config message
	    if(in_msg->_body->_conf->has_linked_data(*in_sess)){
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
								    // config item count
								    case asn1::ParameterType::_pt_pmink_config_ac_line:
									line.clear();
									line.append(tmp_val, tmp_val_l);
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

	// tokenize
	pmink_utils::tokenize(&line, tmp_lst, 50, &tmp_size, true);
	// auto complete
	config->auto_complete(	&cm_mode,
				config::CONFIG_ACM_TAB,
				config->get_cmd_tree(),
				config->get_definition_wn(&cfg_user_id)->wnode,
				tmp_lst,
				tmp_size,
				&ac_res,
				&res_size,
				&last_found,
				&error_count,
				tmp_err,
				false,
				&tmp_node_lst);

	// replace current line with auto completed values
	line.clear();
	for(int i = 0; i<tmp_size; i++){
		line.append(tmp_lst[i]);
		if(i < res_size) line.append(" "); else break;
	}

	// set values
	r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&ca_cfg_result, 1);
	// ac line
	r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pt_pmink_config_ac_line, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)line.c_str(), line.size());

	// cm mode
	r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_cm_mode, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&cm_mode, 1);

	// include bodu
	*include_body = true;
	// continue
	stream->continue_sequence();


}




StreamNext::StreamNext(){
	cfg_res = NULL;
	new_stream = NULL;
	// big endian parameter ids
	pt_cfg_item_name = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_name);
	pt_cfg_item_path = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_path);
	pt_cfg_item_desc = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_desc);
	pt_cfg_item_ns = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_ns);
	pt_cfg_item_value = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_value);
	pt_cfg_item_nvalue = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_nvalue);
	pt_cfg_item_nt = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_nt);
	pt_cfg_cfg_line = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_line);
	pt_cfg_cfg_error = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_ac_err);
	pt_cfg_item_count = htobe32(asn1::ParameterType::_pt_pmink_config_cfg_item_count);


}

// TAB stream next
void StreamNext::process_tab(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);

	// more results
	if(new_stream->res_index < new_stream->ac_res.children.size()){
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
			for(int i = 0; i<6; i++){
				r14pm->_body->_conf->_params->set_child(i);
				r14pm->_body->_conf->_params->get_child(i)->set_value();
				r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

			}
			// prepare
			r14pm->prepare();

		// unlink params before setting new ones
		}else{
		    int cc = r14pm->_body->_conf->_params->children.size();
		    if(cc < 6){
			// set children, allocate more
			for(int i = cc; i<6; i++){
				r14pm->_body->_conf->_params->set_child(i);
				r14pm->_body->_conf->_params->get_child(i)->set_value();
				r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

			}
			// prepare
			r14pm->prepare();

		    }else if(cc > 6){
			// remove extra children if used in some other session, only 2 needed
			for(int i = 6; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
		    }
		}

		// set result action
		r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&new_stream->ca_cfg_result, 1);

		// item name
		r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_name, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)new_stream->ac_res.children[new_stream->res_index]->name.c_str(),
													new_stream->ac_res.children[new_stream->res_index]->name.size());
		// item desc
		r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_desc, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)new_stream->ac_res.children[new_stream->res_index]->desc.c_str(),
													new_stream->ac_res.children[new_stream->res_index]->desc.size());
		// node state
		r14pm->_body->_conf->_params->get_child(2)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_ns, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(2)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)&new_stream->ac_res.children[new_stream->res_index]->node_state,
													1);

		// node value
		r14pm->_body->_conf->_params->get_child(3)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_value, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(3)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)new_stream->ac_res.children[new_stream->res_index]->value.c_str(),
													new_stream->ac_res.children[new_stream->res_index]->value.size());
		// node new value
		r14pm->_body->_conf->_params->get_child(4)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_nvalue, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(4)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)new_stream->ac_res.children[new_stream->res_index]->new_value.c_str(),
													new_stream->ac_res.children[new_stream->res_index]->new_value.size());
		// node type
		r14pm->_body->_conf->_params->get_child(5)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_nt, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(5)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)&new_stream->ac_res.children[new_stream->res_index]->node_type,
													1);
		// include body
		*include_body = true;
		// continue
		stream->continue_sequence();


		// next result item
		++new_stream->res_index;

	// finished
	}else{
	    // end sequence
	    stream->end_sequence();
	    // free temp nodes and clear res buffer
	    new_stream->ac_res.children.clear();
	    for(unsigned int i = 0; i<new_stream->tmp_node_lst.children.size(); i++) delete new_stream->tmp_node_lst.children[i];
	    new_stream->tmp_node_lst.children.clear();
	}
}

// GET stream next
void StreamNext::process_get(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);

	// more results
	if(new_stream->res_index < new_stream->ac_res.children.size()){
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

		// set result action
		r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&new_stream->ca_cfg_result, 1);

		// item path
		// get full path and save to tmp_node_lst
		config::Config::get_parent_line(new_stream->ac_res.children[new_stream->res_index], &new_stream->tmp_node_lst.name);
		new_stream->tmp_node_lst.name.append(new_stream->ac_res.children[new_stream->res_index]->name);
		r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_path, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)new_stream->tmp_node_lst.name.c_str(),
													new_stream->tmp_node_lst.name.size());
		// node value
		r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_value, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)new_stream->ac_res.children[new_stream->res_index]->value.c_str(),
													new_stream->ac_res.children[new_stream->res_index]->value.size());
		// node type
		r14pm->_body->_conf->_params->get_child(2)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_item_nt, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(2)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)&new_stream->ac_res.children[new_stream->res_index]->node_type,
													1);


		// include body
		*include_body = true;
		// continue
		stream->continue_sequence();

		// next result item
		++new_stream->res_index;

	// finished
	}else{
	    // end sequence
	    stream->end_sequence();
	    // free temp nodes and clear res buffer
	    new_stream->ac_res.children.clear();

	}
}

// ENTER stream next
void StreamNext::process_enter(r14p::R14PCallbackArgs* args){
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);

	// results
	if(getline(new_stream->line_stream, new_stream->line_buffer)){
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

		// set result action
		r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&new_stream->ca_cfg_result, 1);

		// line
		r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_cfg_line, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)new_stream->line_buffer.c_str(),
													new_stream->line_buffer.size());
		// include body
		*include_body = true;
		// continue
		stream->continue_sequence();


	// errors
	}else if(new_stream->err_index >= 0){
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

		// set result action
		r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&new_stream->ca_cfg_result, 1);

		// error line
		r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pt_cfg_cfg_error, sizeof(uint32_t));
		r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(	1,
													(unsigned char*)new_stream->tmp_err[new_stream->err_index].c_str(),
													new_stream->tmp_err[new_stream->err_index].size());
		// include body
		*include_body = true;
		// continue
		stream->continue_sequence();


		// dec error index
		--new_stream->err_index;

	// finished
	}else{
	    // end sequence
	    stream->end_sequence();
	    // free temp nodes and clear res buffer
	    new_stream->ac_res.children.clear();
	    for(unsigned int i = 0; i<new_stream->tmp_node_lst.children.size(); i++) delete new_stream->tmp_node_lst.children[i];
	    new_stream->tmp_node_lst.children.clear();

	}
}


// stream next, send config item nodes after auto completed line
void StreamNext::run(r14p::R14PCallbackArgs* args) {
    // pmink daemons
    if(new_stream->config_action == asn1::ConfigAction::_ca_cfg_get){
	process_get(args);

    // CLI
    }else{
	switch(new_stream->ac_mode){
	    case config::CONFIG_ACM_TAB:
		process_tab(args);
		break;

	    case config::CONFIG_ACM_ENTER:
		process_enter(args);
		break;
	}

    }



}
