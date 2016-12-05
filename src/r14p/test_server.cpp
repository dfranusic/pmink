#include <iostream>
#include <sctp.h>
#include <r14p.h>
#include <r14p_asn1.h>
#include <fstream>
#include <deque>
#include <pool.h>

pthread_t in_thread;
asn1::ASN1Node tmp_node;
asn1::TLVNode tmp_tlv;
unsigned char tmp_body_buff[1024];

void* timer_thread(void* args){
    unsigned char tmp_buff[1024];
    int tmp_buff_len;

    cout << "Starting TIMER thread" << endl;
    sleep(2);
    r14p::R14PStream* stream = (r14p::R14PStream*)args;
    asn1::R14PMessage* msg = stream->get_r14p_message();
    r14p::R14PPayload* r14pld = stream->get_r14p_payload();


    // add body
    if(msg->_body != NULL) msg->_body->unlink(1); else{
	msg->set_body();
	msg->prepare();

    }
    msg->_body->choice_selection = &tmp_node;
    msg->_body->choice_selection->parent_node = msg->_body;
    msg->_body->choice_selection->set_linked_data(1);


    // continue
    stream->set_continue_flag();
    // encode
    r14pld->raw_data_length = asn1::encode(tmp_buff, 1024, msg, 1);
    memcpy(r14pld->raw_data, tmp_buff, r14pld->raw_data_length);
    stream->get_client()->push_out_queue(r14pld);


    cout << "Ending TIMER thread" << endl;
    pthread_detach(in_thread);
    return NULL;


}



class PayloadSent: public r14p::R14PCallbackMethod {
public:
    bool done;
    PayloadSent(){
	done = false;
    }
    void run(r14p::R14PCallbackArgs* args) {
	r14p::R14PPayload* pld = (r14p::R14PPayload*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_PAYLOAD);
	cout << "Payload SENT" << endl;
	if(!done){
		pthread_create(&in_thread, NULL, &timer_thread, pld->stream);
		done = true;

	}

    }

};

class StreamEnd: public r14p::R14PCallbackMethod {
public:

    void run(r14p::R14PCallbackArgs* args) {
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	cout << "Stream END!, error: " <<  endl;
    }
};


class StreamNext: public r14p::R14PCallbackMethod {
public:
    PayloadSent payload_sent;

    void run(r14p::R14PCallbackArgs* args) {
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* msg = stream->get_r14p_message();
	bool* mem_switch = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_MEM_SWITCH);
	asn1::R14PMessage* in_msg = (asn1::R14PMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG);
	int* in_sess = (int*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_IN_MSG_ID);

	std::cout << "Seq num: " << stream->get_sequence_num() << " - " << stream->get_seq_reply_received() << " - " << stream->get_sequence_flag() <<  std::endl;

	if(in_msg != NULL){
		if(in_msg->_body != NULL){
			if(in_msg->_body->choice_selection != NULL){
			    if(in_msg->_body->choice_selection->has_linked_data(*in_sess)){
				    asn1::TLVNode* orig_tlv = in_msg->_body->choice_selection->linked_node->tlv;
				    memcpy(tmp_body_buff, orig_tlv->value, orig_tlv->value_length);

				    cout << "BODY EXISTS!!!: " <<  orig_tlv->value_length << endl;


				    tmp_node.tlv = &tmp_tlv;
				    tmp_tlv.value = tmp_body_buff;
				    tmp_tlv.value_length = orig_tlv->value_length;
				    tmp_tlv.value_length_size = orig_tlv->value_length_size;

				    tmp_tlv.tag_value_size = orig_tlv->tag_value_size;
				    tmp_tlv.tag_value = orig_tlv->tag_value;
				    tmp_tlv.tag_class = orig_tlv->tag_class;
				    tmp_tlv.complexity = orig_tlv->complexity;
				    tmp_tlv.override_auto_complexity = true;


				    tmp_node.linked_node = &tmp_node;


			    }
			}

		}

	}



	cout << "Stream NEXT" << endl;
	*mem_switch = true;


	stream->get_r14p_payload()->set_callback(r14p::R14P_ET_PAYLOAD_SENT, &payload_sent);
	stream->wait_sequence();

    }
};

class NewDatagram: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args) {
	cout << "new DATAGRAM" << endl;

    }
};

class NewStream: public r14p::R14PCallbackMethod {
public:
    StreamNext stream_next;
    StreamEnd stream_end;

    void run(r14p::R14PCallbackArgs* args) {
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	stream->set_callback(r14p::R14P_ET_STREAM_NEXT, &stream_next);
	stream->set_callback(r14p::R14P_ET_STREAM_END, &stream_end);
	cout << "new Stream" << endl;
    }
};

class NewClient: public r14p::R14PCallbackMethod {
public:
    NewStream new_stream;
    NewDatagram new_datagram;

    void run(r14p::R14PCallbackArgs* args) {
	r14p::R14PClient* client = (r14p::R14PClient*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_CLIENT);
	client->set_callback(r14p::R14P_ET_STREAM_NEW, &new_stream);
	//client->set_callback(r14p::R14P_ET_DATAGRAM, &new_datagram);

	cout << "new CLIENT connection" << endl;
    }
};

int main(){
	NewClient new_client;

	// start R14P session
	r14p::R14PSession* r14ps = r14p::init_session("server_type", "server_id", 100, 5, false, 5);

	// accept connections (server mode)
	r14ps->start_server(NULL, 10000);

	// set callback for NEW_CLIENT event
	r14ps->set_callback(r14p::R14P_ET_CLIENT_NEW, &new_client);


	while(1){
	    sleep(1);
	}

	// stop server
	r14ps->stop_server();
	// destroy session, free memory
	r14p::destroy_session(r14ps);


}

