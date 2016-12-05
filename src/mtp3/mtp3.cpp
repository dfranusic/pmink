#include<iostream>
#include<mtp3.h>
#include<endian.h>
#include<string.h>


// mtp3 signalling message base
mtp3::MTP3Message::MTP3Message(){
	message_group = __MTP3_UNKNOWN_MESSAGE_GROUP__;
	message_type = __MTP3_UNKNOWN_MESSAGE_TYPE__;
}

mtp3::MTP3Message::~MTP3Message(){}




// =============== Signalling network management messages ==================

// Changeover_order
mtp3::Changeover_order::Changeover_order(){
	message_group = CHM;
	message_type = COO;
}
void mtp3::Changeover_order::init(char* data, int data_length){
	// H0 & H1
	data++;
	fsn = *data;
}
void mtp3::Changeover_order::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// fsn
	*buffer = fsn;


}

// Changeover_order
mtp3::Changeover_ack::Changeover_ack(){
	message_group = CHM;
	message_type = COA;
}
void mtp3::Changeover_ack::init(char* data, int data_length){
	// H0 & H1
	data++;
	fsn = *data;
}
void mtp3::Changeover_ack::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// fsn
	*buffer = fsn;
}

// Changeback_declaration
mtp3::Changeback_declaration::Changeback_declaration(){
	message_group = CHM;
	message_type = CBD;
}
void mtp3::Changeback_declaration::init(char* data, int data_length){
	// H0 & H1
	data++;
	changeback_code = *data;
}
void mtp3::Changeback_declaration::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// fsn
	*buffer = changeback_code;
}

// Changeback_ack
mtp3::Changeback_ack::Changeback_ack(){
	message_group = CHM;
	message_type = CBA;
}
void mtp3::Changeback_ack::init(char* data, int data_length){
	// H0 & H1
	data++;
	changeback_code = *data;
}
void mtp3::Changeback_ack::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// fsn
	*buffer = changeback_code;
}

// EmergencyChageover_order
mtp3::EmergencyChageover_order::EmergencyChageover_order(){
	message_group = ECM;
	message_type = ECO;
}
void mtp3::EmergencyChageover_order::init(char* data, int data_length){}
void mtp3::EmergencyChageover_order::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}

// EmergencyChageover_ack
mtp3::EmergencyChageover_ack::EmergencyChageover_ack(){
	message_group = ECM;
	message_type = ECA;
}
void mtp3::EmergencyChageover_ack::init(char* data, int data_length){}
void mtp3::EmergencyChageover_ack::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}

// TransferProhibited
mtp3::TransferProhibited::TransferProhibited(){
	message_group = TFM;
	message_type = TFP;
}
void mtp3::TransferProhibited::init(char* data, int data_length){
	// H0 & H1
	data++;
	destination = htobe16((data[0] & 0xff) << 8 + (data[1] & 0xff));
}
void mtp3::TransferProhibited::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// destination
	int tmp = be16toh(destination);
	buffer[0] = tmp >> 8;
	buffer[1] = tmp;
}

// TransferAllowed
mtp3::TransferAllowed::TransferAllowed(){
	message_group = TFM;
	message_type = TFA;
}
void mtp3::TransferAllowed::init(char* data, int data_length){
	// H0 & H1
	data++;
	destination = htobe16((data[0] & 0xff) << 8 + (data[1] & 0xff));
}
void mtp3::TransferAllowed::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// destination
	int tmp = be16toh(destination);
	buffer[0] = tmp >> 8;
	buffer[1] = tmp;
}

// TransferRestricted
mtp3::TransferRestricted::TransferRestricted(){
	message_group = TFM;
	message_type = TFR;
}
void mtp3::TransferRestricted::init(char* data, int data_length){
	// H0 & H1
	data++;
	destination = htobe16((data[0] & 0xff) << 8 + (data[1] & 0xff));
}
void mtp3::TransferRestricted::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// destination
	int tmp = be16toh(destination);
	buffer[0] = tmp >> 8;
	buffer[1] = tmp;
}

// RouteSetTestProhibitedDest
mtp3::RouteSetTestProhibitedDest::RouteSetTestProhibitedDest(){
	message_group = RSM;
	message_type = RST;
}
void mtp3::RouteSetTestProhibitedDest::init(char* data, int data_length){
	// H0 & H1
	data++;
	destination = htobe16((data[0] & 0xff) << 8 + (data[1] & 0xff));
}
void mtp3::RouteSetTestProhibitedDest::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// destination
	int tmp = be16toh(destination);
	buffer[0] = tmp >> 8;
	buffer[1] = tmp;
}

// RouteSetTestRestrictedDest
mtp3::RouteSetTestRestrictedDest::RouteSetTestRestrictedDest(){
	message_group = RSM;
	message_type = RSR;
}
void mtp3::RouteSetTestRestrictedDest::init(char* data, int data_length){
	// H0 & H1
	data++;
	destination = htobe16((data[0] & 0xff) << 8 + (data[1] & 0xff));
}
void mtp3::RouteSetTestRestrictedDest::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// destination
	int tmp = be16toh(destination);
	buffer[0] = tmp >> 8;
	buffer[1] = tmp;
}

// LinkInhibit
mtp3::LinkInhibit::LinkInhibit(){
	message_group = MIM;
	message_type = LIN;
}
void mtp3::LinkInhibit::init(char* data, int data_length){}
void mtp3::LinkInhibit::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}


// LinkUninhibit
mtp3::LinkUninhibit::LinkUninhibit(){
	message_group = MIM;
	message_type = LUN;
}
void mtp3::LinkUninhibit::init(char* data, int data_length){}
void mtp3::LinkUninhibit::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}

// LinkInhibited_ack
mtp3::LinkInhibited_ack::LinkInhibited_ack(){
	message_group = MIM;
	message_type = LIA;
}
void mtp3::LinkInhibited_ack::init(char* data, int data_length){}
void mtp3::LinkInhibited_ack::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}


// LinkUninhibited_ack
mtp3::LinkUninhibited_ack::LinkUninhibited_ack(){
	message_group = MIM;
	message_type = LUA;
}
void mtp3::LinkUninhibited_ack::init(char* data, int data_length){}
void mtp3::LinkUninhibited_ack::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}


// LinkInhibit_denied
mtp3::LinkInhibit_denied::LinkInhibit_denied(){
	message_group = MIM;
	message_type = LID;
}
void mtp3::LinkInhibit_denied::init(char* data, int data_length){}
void mtp3::LinkInhibit_denied::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}

// LinkForceUninhibit
mtp3::LinkForceUninhibit::LinkForceUninhibit(){
	message_group = MIM;
	message_type = LFU;
}
void mtp3::LinkForceUninhibit::init(char* data, int data_length){}
void mtp3::LinkForceUninhibit::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}


// LinkLocalInhibitTest
mtp3::LinkLocalInhibitTest::LinkLocalInhibitTest(){
	message_group = MIM;
	message_type = LLT;
}
void mtp3::LinkLocalInhibitTest::init(char* data, int data_length){}
void mtp3::LinkLocalInhibitTest::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}

// LinkRemoteInhibitTest
mtp3::LinkRemoteInhibitTest::LinkRemoteInhibitTest(){
	message_group = MIM;
	message_type = LRT;
}
void mtp3::LinkRemoteInhibitTest::init(char* data, int data_length){}
void mtp3::LinkRemoteInhibitTest::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}

// TrafficRestartAllowed
mtp3::TrafficRestartAllowed::TrafficRestartAllowed(){
	message_group = TRM;
	message_type = TRA;
}
void mtp3::TrafficRestartAllowed::init(char* data, int data_length){}
void mtp3::TrafficRestartAllowed::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;

}


// DataLinkConn_order
mtp3::DataLinkConn_order::DataLinkConn_order(){
	message_group = DLM;
	message_type = DLC;
}
void mtp3::DataLinkConn_order::init(char* data, int data_length){
	// H0 & H1
	data++;
	data_link_identity = htobe16((data[0] & 0xff) << 8 + (data[1] & 0xff));

}
void mtp3::DataLinkConn_order::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// data_link_identity
	int tmp = be16toh(data_link_identity);
	buffer[0] = tmp >> 8;
	buffer[1] = tmp;

}

// ConnSuccessful
mtp3::ConnSuccessful::ConnSuccessful(){
	message_group = DLM;
	message_type = CSS;
}
void mtp3::ConnSuccessful::init(char* data, int data_length){}
void mtp3::ConnSuccessful::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;

}

// ConnNotSuccessful
mtp3::ConnNotSuccessful::ConnNotSuccessful(){
	message_group = DLM;
	message_type = CNS;
}
void mtp3::ConnNotSuccessful::init(char* data, int data_length){}
void mtp3::ConnNotSuccessful::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}

// ConnNotPossible
mtp3::ConnNotPossible::ConnNotPossible(){
	message_group = DLM;
	message_type = CNP;
}
void mtp3::ConnNotPossible::init(char* data, int data_length){}
void mtp3::ConnNotPossible::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
}


// TransferControlled
mtp3::TransferControlled::TransferControlled(){
	message_group = FCM;
	message_type = TFC;
}
void mtp3::TransferControlled::init(char* data, int data_length){
	destination = htobe16((data[0] & 0xff) << 8 + (data[1] & 0xff));
}
void mtp3::TransferControlled::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// destination
	int tmp = be16toh(destination);
	buffer[0] = tmp >> 8;
	buffer[1] = tmp;
}

// RouteSetCongestionTest
mtp3::RouteSetCongestionTest::RouteSetCongestionTest(){
	message_group = FCM;
	message_type = RCT;
}
void mtp3::RouteSetCongestionTest::init(char* data, int data_length){}
void mtp3::RouteSetCongestionTest::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;

}

// UserPartUnavailable
mtp3::UserPartUnavailable::UserPartUnavailable(){
	message_group = UFC;
	message_type = UPU;
}
void mtp3::UserPartUnavailable::init(char* data, int data_length){
	// H0 & H1
	data++;
	int tmp = htobe32((data[0] & 0xff) << 24 + ((data[1] & 0xff) << 16) + ((data[2] & 0xff) << 8) + 0x00);
	unavailability_cause = (UnavailCauseType)((tmp >> 20) & 0x0f);
	user_part_id = (UserPartIdType)((tmp >> 16) & 0x0f);
	destination = tmp & 0x3fff;

}
void mtp3::UserPartUnavailable::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// unavail cause, user part id, destination
	int tmp = be32toh(0x00000000 + (destination & 0x3fff) + (user_part_id << 16) + (unavailability_cause << 20));
	buffer[0] = tmp >> 8;
	buffer[1] = tmp >> 16;
	buffer[2] = tmp >> 24;


}


// =============== Signalling network testing and maintenance messages ==================

// Sltm
mtp3::Sltm::Sltm(){
	message_group = TESTM;
	message_type = SLTM;
	test_pattern = NULL;
	test_pattern_length = 0;
}
void mtp3::Sltm::init(char* data, int data_length){
	// H0 & H1
	data++;
	test_pattern_length = (*(data++) & 0xff) >> 4;
	test_pattern = data;

}
void mtp3::Sltm::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// test pattern
	*(buffer++) = test_pattern_length << 4;
	if(test_pattern_length > 0) memcpy(buffer, test_pattern, test_pattern_length);


}

// Slta
mtp3::Slta::Slta(){
	message_group = TESTM;
	message_type = SLTA;
}
void mtp3::Slta::init(char* data, int data_length){
	// H0 & H1
	data++;
	test_pattern_length = (*(data++) & 0xff) >> 4;
	test_pattern = data;
}
void mtp3::Slta::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// H0 & H1
	*(buffer++) = message_type;
	// test pattern
	*(buffer++) = test_pattern_length << 4;
	if(test_pattern_length > 0) memcpy(buffer, test_pattern, test_pattern_length);

}


// mtp3 packet
mtp3::MTP3Packet::MTP3Packet(){
	sif = NULL;
	sif_length = 0;
	signalling_message = NULL;
	label.dpc = 0;
	label.opc = 0;
	label.slc = 0;
}

mtp3::MTP3Packet::~MTP3Packet(){}


void mtp3::MTP3Packet::encode(char* buffer, int buffer_length, int offset, int* result_length){
	int tmp;
	// offset
	buffer += offset;
	// service information octet
	*buffer = network_indicator + service_indicator;
	buffer++;
	// label
	tmp = be32toh(((label.slc & 0xff) << 28) + ((label.opc & 0x3fff) << 14) + (label.dpc & 0x3fff));
	*(buffer++) = tmp >> 24;
	*(buffer++) = tmp >> 16;
	*(buffer++) = tmp >> 8;
	*(buffer++) = tmp;
	//std::cout << std::hex << tmp << std::endl;
	if(sif_length > 0 && sif != NULL) memcpy(buffer, sif, sif_length);
	*result_length = 5 + sif_length;


}

// main mtp3 decode method
mtp3::MTP3Packet* mtp3::decode(char* data, int data_length){
	if(data != NULL && data_length > 0){
		MTP3Packet* mtp3p = new MTP3Packet();
		int tmp;
		// service information octet
		mtp3p->network_indicator = (NetworkIndicatorType)(*data & 0xf0);
		mtp3p->service_indicator = (ServiceIndicatorType)(*data & 0x0f);
		data++;
		// label
		tmp = htobe32(((data[0] & 0xff) << 24) + ((data[1] & 0xff) << 16) + ((data[2] & 0xff) << 8) + (data[3] & 0xff));
		mtp3p->label.dpc = tmp & 0x3fff;
		mtp3p->label.opc = (tmp >> 14) & 0x3fff;
		mtp3p->label.slc = (tmp >> 28) & 0x0f;
		data += 4;
		// sif
		mtp3p->sif = data;
		mtp3p->sif_length = data_length - 5;
		// signalling mtp3 message
		if(mtp3p->service_indicator == SNMM || mtp3p->service_indicator == SNTMM){
			switch((mtp3p->service_indicator << 8) + (data[0] & 0xff)){
				// Signalling network management messages
				case CBA: mtp3p->signalling_message = new Changeback_ack(); break;
				case CBD: mtp3p->signalling_message = new Changeback_declaration(); break;
				case CNP: mtp3p->signalling_message = new ConnNotPossible(); break;
				case CNS: mtp3p->signalling_message = new ConnNotSuccessful(); break;
				case COA: mtp3p->signalling_message = new Changeover_ack(); break;
				case COO: mtp3p->signalling_message = new Changeover_order(); break;
				case CSS: mtp3p->signalling_message = new ConnSuccessful(); break;
				case DLC: mtp3p->signalling_message = new DataLinkConn_order(); break;
				case ECA: mtp3p->signalling_message = new EmergencyChageover_ack(); break;
				case ECO: mtp3p->signalling_message = new EmergencyChageover_order(); break;
				case LFU: mtp3p->signalling_message = new LinkForceUninhibit(); break;
				case LIA: mtp3p->signalling_message = new LinkInhibited_ack(); break;
				case LID: mtp3p->signalling_message = new LinkInhibit_denied(); break;
				case LIN: mtp3p->signalling_message = new LinkInhibit(); break;
				case LLT: mtp3p->signalling_message = new LinkLocalInhibitTest(); break;
				case LUA: mtp3p->signalling_message = new LinkUninhibited_ack(); break;
				case LUN: mtp3p->signalling_message = new LinkUninhibit(); break;
				case LRT: mtp3p->signalling_message = new LinkRemoteInhibitTest(); break;
				case RCT: mtp3p->signalling_message = new RouteSetCongestionTest(); break;
				case RSR: mtp3p->signalling_message = new RouteSetTestRestrictedDest(); break;
				case RST: mtp3p->signalling_message = new RouteSetTestProhibitedDest(); break;
				case TFA: mtp3p->signalling_message = new TransferAllowed(); break;
				case TFC: mtp3p->signalling_message = new TransferControlled(); break;
				case TFP: mtp3p->signalling_message = new TransferProhibited(); break;
				case TFR: mtp3p->signalling_message = new TransferRestricted(); break;
				case TRA: mtp3p->signalling_message = new TrafficRestartAllowed(); break;
				case UPU: mtp3p->signalling_message = new UserPartUnavailable(); break;
				// Signalling network testing and maintenance messages
				case SLTM: mtp3p->signalling_message = new Sltm(); break;
				case SLTA: mtp3p->signalling_message = new Slta(); break;
			}
			// init
			if(mtp3p->signalling_message != NULL) mtp3p->signalling_message->init(mtp3p->sif, mtp3p->sif_length);

		}




		return mtp3p;

	}
	return NULL;
}


