#include <iostream>
#include <sctp.h>
#include <r14p.h>
#include <r14p_asn1.h>
#include <fstream>
#include <deque>
#include <pool.h>

class PayloadSent: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args) {
	cout << "payload SENT" << endl;
    }
};

class StreamNext: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args) {
	r14p::R14PStream* stream = (r14p::R14PStream*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_STREAM);
	asn1::R14PMessage* r14pm = stream->get_r14p_message();
	bool* include_body = (bool*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_BODY);

	std::cout << "Seq num: " << stream->get_sequence_num() << " - " << stream->get_seq_reply_received() << " - " << stream->get_sequence_flag() <<  std::endl;
	if(r14pm->_body != NULL) {
		r14pm->_body->unlink(1);
		r14pm->_body->_encrypted_data->set_linked_data(1);

	}else{
		r14pm->set_body();
		r14pm->prepare();

	}
	r14pm->_body->_encrypted_data->set_linked_data(1, (unsigned char*)"12345", 5);
	//r14pm->_body->unlink(1);

	*include_body = true;

	if(stream->get_sequence_num() >= 2) stream->end_sequence();
	return;

    }
};

class DatagramReply: public r14p::R14PCallbackMethod {
    void run(r14p::R14PCallbackArgs* args) {
	cout << "datagram RESPONSE" << endl;

    }
};


int main(){

	PayloadSent pld_sent;
	StreamNext stream_next;
	DatagramReply dtg_response;
	timespec tm;
	timespec tm2;

	tm.tv_sec = 0;
	tm.tv_nsec = 1;

	// start R14P session
	r14p::R14PSession* r14ps = r14p::init_session("client_type", "client_id", 1000, 5, false, 5);

	// connect to end point
	r14p::R14PClient* r14p_client = r14ps->connect("127.0.0.1", 10000, 16, NULL, 0);
	if(r14p_client == NULL) return 1;

	r14p::R14PStream* new_stream = r14p_client->new_stream("test_dest_type", NULL, &pld_sent, &stream_next);
	if(new_stream != NULL) new_stream->send(false); else std::cout << ">>>>>>>>>>>>>>>>>>>>>> ERROR <<<<<<<<<<<<<<<<<<" << std::endl;

	unsigned char test_data[] = "ABCD";
	asn1::Body dm;
	int pldt = 100;
	dm._data->set_payload();
	dm.prepare();

	dm._data->_payload_type->set_linked_data(1, (unsigned char*)&pldt, 1);
	dm._data->_payload->set_linked_data(1, test_data, 4);
	dm.choice_selection = dm._data;


	//for(int i = 0; i<1; i++) r14p_client->send_datagram(&dm, NULL, &dtg_response,  "KK_DMN_T", "KK_DMN_ID");


	//for(int i = 0; i<1; i++) r14p_client->send_datagram(asn1::PayloadType::_dmt_unknown, test_data, 4, NULL, &dtg_response,  "KK_DMN_T", "KK_DMN_ID");





	//cout << "AAAAAAAAAA" << endl;
	//while(1){
	    sleep(5);

	    //}
	// disconnect client
	r14p_client->disconnect();
	// remove client, free memory
	r14ps->remove_client(r14p_client);
	sleep(1);

	// destroy session, free memory
	r14p::destroy_session(r14ps);

}

