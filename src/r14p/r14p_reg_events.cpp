#include <r14p_reg_events.h>


void r14p::RegClientStreamDone::run(r14p::R14PCallbackArgs* args){
    //std::cout << "RegClientStreamDone::run" << std::endl;
    asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
    r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
    r14p::R14PClient* client = stream->get_client();
    R14PSession* r14ps = client->get_session();
    // timeout is in_msg is NULL
    if(in_msg == NULL) snew->status = -1;

    // set registration flag
    if(snew->status == 0) client->set_reg_flag(true);
    else client->disconnect();

    // run event if client registered
    if(client->is_registered()){

	// add client to routing method
	if(r14ps->get_routing_handler() != NULL){
	    r14ps->lock_clients();
	    r14ps->get_routing_handler()->update_client(client, client->get_end_point_daemon_type(), client->get_end_point_daemon_id());
	    r14ps->unlock_clients();

	}


	// remove new stream event for current client
	client->remove_callback(r14p::R14P_ET_STREAM_NEW);

	// process callback
	R14PCallbackArgs cb_args;
	cb_args.clear_all_args();
	cb_args.add_arg(R14P_CB_INPUT_ARGS, R14P_CB_ARG_CLIENT, client);
	r14ps->process_callback(R14P_ET_CLIENT_CREATED, &cb_args);
	r14ps->process_callback(R14P_ET_CLIENT_NEW, &cb_args);


    }


    // post
    //sem_post(&snew->signal);
    snew->done_signal.set(true);

}



r14p::RegClientStreamNew::RegClientStreamNew(R14PClient* _client){
    client = _client;
    sdone = NULL;
    //sem_init(&signal, 0, 0);
    status = 1;

}

r14p::RegClientStreamNew::~RegClientStreamNew(){
    //sem_destroy(&signal);

}


void r14p::RegClientStreamNew::run(r14p::R14PCallbackArgs* args){
    r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
    r14p::R14PClient* client = stream->get_client();
    asn1::R14PMessage* r14pm = stream->get_r14p_message();
    bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);
    asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
    uint64_t* in_sess = (uint64_t*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);
    char* tmp_val = NULL;
    int tmp_val_l = 0;
    std::string tmp_str;
    int c = 0;

    // set end and timeout event handlers
    stream->set_callback(r14p::R14P_ET_STREAM_END, sdone);
    stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, sdone);
    // remove new stream event, discard any new streams (unsafe flag is TRUE, mutex already locked)
    client->remove_callback(r14p::R14P_ET_STREAM_NEW, true);

    //std::cout << "RegClientStreamNew::run" << std::endl;

    // check for body
    if(in_msg->_body != NULL){
	// check for config message
	if(in_msg->_body->_reg->has_linked_data(*in_sess)){
	    // check for GET action
	    if(in_msg->_body->_reg->_reg_action->linked_node->tlv->value[0] == asn1::RegistrationAction::_ra_reg_request){
		// check for params part
		if(in_msg->_body->_reg->_params != NULL){
			if(in_msg->_body->_reg->_params->has_linked_data(*in_sess)){
			    // process params
			    for(unsigned int i = 0; i<in_msg->_body->_reg->_params->children.size(); i++){
				// check for current session
				if(in_msg->_body->_reg->_params->get_child(i)->has_linked_data(*in_sess)){
				    // check param id, convert from big endian to host
				    uint32_t* param_id = (uint32_t*)in_msg->_body->_reg->_params->get_child(i)->_id->linked_node->tlv->value;
				    // check for value
				    if(in_msg->_body->_reg->_params->get_child(i)->_value != NULL){
					    // check if value exists in current session
					    if(in_msg->_body->_reg->_params->get_child(i)->_value->has_linked_data(*in_sess)){
						    // check if child exists
						    if(in_msg->_body->_reg->_params->get_child(i)->_value->get_child(0) != NULL){
							// check if child exists in current sesion
							if(in_msg->_body->_reg->_params->get_child(i)->_value->get_child(0)->has_linked_data(*in_sess)){
							    // set tmp values
							    tmp_val = (char*)in_msg->_body->_reg->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value;
							    tmp_val_l = in_msg->_body->_reg->_params->get_child(i)->_value->get_child(0)->linked_node->tlv->value_length;

							    // match param
							    switch(be32toh(*param_id)){
								// daemon type
								case asn1::ParameterType::_pt_pmink_daemon_type:
								    tmp_str.clear();
								    tmp_str.append(tmp_val, tmp_val_l);
								    client->set_end_point_daemon_type(tmp_str.c_str());
								    ++c;
								    //std::cout << "End poind dtype: " << client->get_end_point_daemon_type() << std::endl;
								    break;

								// daemon id
								case asn1::ParameterType::_pt_pmink_daemon_id:
								    tmp_str.clear();
								    tmp_str.append(tmp_val, tmp_val_l);
								    client->set_end_point_daemon_id(tmp_str.c_str());
								    ++c;
								    //std::cout << "End poind did: " << client->get_end_point_daemon_id() << std::endl;
								    break;

								// router status
								case asn1::ParameterType::_pt_pmink_router_status:
								    client->set_router_flag(tmp_val[0] == 0 ? false : true);
								    ++c;
								    //std::cout << "End poind router: " << client->is_router() << std::endl;
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
    // check if all mandatory params were received
    if(c >= 3) status = 0;//client->set_reg_flag(true);



    // prepare body
    if(r14pm->_body != NULL) {
	    r14pm->_body->unlink(1);
	    r14pm->_body->_conf->set_linked_data(1);

    }else{
	    r14pm->set_body();
	    r14pm->prepare();
    }
    // set bodu
    pm_dtype = htobe32(asn1::ParameterType::_pt_pmink_daemon_type);
    pm_did = htobe32(asn1::ParameterType::_pt_pmink_daemon_id);
    pm_router = htobe32(asn1::ParameterType::_pt_pmink_router_status);
    reg_action = asn1::RegistrationAction::_ra_reg_result;
    router_flag = (client->get_session()->is_router() ? 1 : 0);
    // set params
    if(r14pm->_body->_reg->_params == NULL){
	    r14pm->_body->_reg->set_params();
	    // set children, allocate more
	    for(int i = 0; i<3; i++){
		    r14pm->_body->_reg->_params->set_child(i);
		    r14pm->_body->_reg->_params->get_child(i)->set_value();
		    r14pm->_body->_reg->_params->get_child(i)->_value->set_child(0);

	    }
	    // prepare
	    r14pm->prepare();

    // unlink params before setting new ones
    }else{
	int cc = r14pm->_body->_reg->_params->children.size();
	if(cc < 3){
	    // set children, allocate more
	    for(int i = cc; i<3; i++){
		    r14pm->_body->_reg->_params->set_child(i);
		    r14pm->_body->_reg->_params->get_child(i)->set_value();
		    r14pm->_body->_reg->_params->get_child(i)->_value->set_child(0);

	    }
	    // prepare
	    r14pm->prepare();

	}else if(cc > 3){
	    // remove extra children if used in some other session, only 2 needed
	    for(int i = 3; i<cc; i++) r14pm->_body->_reg->_params->get_child(i)->unlink(1);
	}
    }

    // set reg action
    r14pm->_body->_reg->_reg_action->set_linked_data(1, (unsigned char*)&reg_action, 1);

    // set daemon type
    r14pm->_body->_reg->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&pm_dtype, sizeof(uint32_t));
    r14pm->_body->_reg->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)client->get_session()->get_daemon_type(), strlen(client->get_session()->get_daemon_type()));

    // set daemon id
    r14pm->_body->_reg->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&pm_did, sizeof(uint32_t));
    r14pm->_body->_reg->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)client->get_session()->get_daemon_id(), strlen(client->get_session()->get_daemon_id()));

    // set router flag
    r14pm->_body->_reg->_params->get_child(2)->_id->set_linked_data(1, (unsigned char*)&pm_router, sizeof(uint32_t));
    r14pm->_body->_reg->_params->get_child(2)->_value->get_child(0)->set_linked_data(1, (unsigned char*)&router_flag, 1);

    // include
    *include_body = true;

    // end stream
    stream->end_sequence();


}
