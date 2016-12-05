#include<m3ua.h>
#include<string.h>
#include<iostream>

// SCON
m3ua::Scon::Scon(){
	type = SCON;
}

bool m3ua::Scon::check_mandatory(){
	return (get_parameter(M3UA_PT_AFFECTED_POINT_CODE) != NULL ? true : false);
}

// REG RSP
m3ua::Reg_rsp::Reg_rsp(){
	type = REG_RSP;
}
bool m3ua::Reg_rsp::check_mandatory(){
	return (get_parameter(M3UA_PT_REGISTRATION_RESULT) != NULL ? true : false);
}


// REG REQ
m3ua::Reg_req::Reg_req(){
	type = REG_REQ;
}
bool m3ua::Reg_req::check_mandatory(){
	return (get_parameter(M3UA_PT_ROUTING_KEY) != NULL ? true : false);
}
// NTFY
m3ua::Ntfy::Ntfy(){
	type = NTFY;
}
bool m3ua::Ntfy::check_mandatory(){
	return (get_parameter(M3UA_PT_STATUS) != NULL ? true : false);
}

// ERR
m3ua::Err::Err(){
	type = M3UA_ERR;
}
bool m3ua::Err::check_mandatory(){
	return (get_parameter(M3UA_PT_ROUTING_CONTEXT) != NULL &&
			get_parameter(M3UA_PT_NETWORK_APPEARANCE) != NULL &&
			get_parameter(M3UA_PT_AFFECTED_POINT_CODE) != NULL ? true : false);
}

// DUPU
m3ua::Dupu::Dupu(){
	type = DUPU;
}
bool m3ua::Dupu::check_mandatory(){
	return (get_parameter(M3UA_PT_USER_CAUSE) != NULL &&
			get_parameter(M3UA_PT_AFFECTED_POINT_CODE) != NULL ? true : false);
}

// DUNA
m3ua::Duna::Duna(){
	type = DUNA;
}
bool m3ua::Duna::check_mandatory(){
	return (get_parameter(M3UA_PT_AFFECTED_POINT_CODE) != NULL ? true : false);
}

// DRST
m3ua::Drst::Drst(){
	type = DRST;
}
bool m3ua::Drst::check_mandatory(){
	return (get_parameter(M3UA_PT_AFFECTED_POINT_CODE) != NULL ? true : false);
}

// DEREG RSP
m3ua::Dereg_rsp::Dereg_rsp(){
	type = DEREG_RSP;
}
bool m3ua::Dereg_rsp::check_mandatory(){
	return (get_parameter(M3UA_PT_DEREGISTRATION_RESULT) != NULL ? true : false);
}

// DEREG REQ
m3ua::Dereg_req::Dereg_req(){
	type = DEREG_REQ;
}
bool m3ua::Dereg_req::check_mandatory(){
	return (get_parameter(M3UA_PT_ROUTING_CONTEXT) != NULL ? true : false);
}

// DAVA
m3ua::Dava::Dava(){
	type = DAVA;
}
bool m3ua::Dava::check_mandatory(){
	return (get_parameter(M3UA_PT_AFFECTED_POINT_CODE) != NULL ? true : false);
}

// DAUD
m3ua::Daud::Daud(){
	type = DAUD;
}
bool m3ua::Daud::check_mandatory(){
	return (get_parameter(M3UA_PT_AFFECTED_POINT_CODE) != NULL ? true : false);
}

// BEAT ACK
m3ua::Beat_ack::Beat_ack(){
	type = BEAT_ACK;
}
bool m3ua::Beat_ack::check_mandatory(){
	return true;
}


// BEAT
m3ua::Beat::Beat(){
	type = BEAT;
}
bool m3ua::Beat::check_mandatory(){
	return true;
}

// ASPUP ACK
m3ua::Aspup_ack::Aspup_ack(){
	type = ASPUP_ACK;

}
bool m3ua::Aspup_ack::check_mandatory(){
	return true;
}

// ASPUP
m3ua::Aspup::Aspup(){
	type = ASPUP;

}
bool m3ua::Aspup::check_mandatory(){
	return true;
}

// ASPIA ACK
m3ua::Aspia_ack::Aspia_ack(){
	type = ASPIA_ACK;;

}
bool m3ua::Aspia_ack::check_mandatory(){
	return true;
}

// ASPIA
m3ua::Aspia::Aspia(){
	type = ASPIA;;

}
bool m3ua::Aspia::check_mandatory(){
	return true;
}

// ASPDN ACK
m3ua::Aspdn_ack::Aspdn_ack(){
	type = ASPDN_ACK;

}
bool m3ua::Aspdn_ack::check_mandatory(){
	return true;
}

// ASPDN
m3ua::Aspdn::Aspdn(){
	type = ASPDN;

}
bool m3ua::Aspdn::check_mandatory(){
	return true;
}

// ASPAC ACK
m3ua::Aspac_ack::Aspac_ack(){
	type = ASPAC_ACK;

}
bool m3ua::Aspac_ack::check_mandatory(){
	return true;
}

// ASPAC
m3ua::Aspac::Aspac(){
	type = ASPAC;

}
bool m3ua::Aspac::check_mandatory(){
	return true;
}

// m3ua DATA
m3ua::Data::Data(){
	type = DATA;
	byte_pos = 0;

}


bool m3ua::Data::check_mandatory(){
	return (get_parameter(M3UA_PT_PROTOCOL_DATA) != NULL ? true : false);
}


// AffectedPointCode
m3ua::AffectedPointCode::AffectedPointCode(){
	type = M3UA_PT_AFFECTED_POINT_CODE;
}



// LocalRoutingKeyId
m3ua::LocalRoutingKeyId::LocalRoutingKeyId(){
	type = M3UA_PT_LOCAL_ROUTING_KEY_IDENTIFIER;
}

// DestinationPointCode
m3ua::DestinationPointCode::DestinationPointCode(){
	type = M3UA_PT_DESTINATION_POINT_CODE;
}

// ServiceIndicators
m3ua::ServiceIndicators::ServiceIndicators(){
	type = M3UA_PT_SERVICE_INDICATORS;
}

// OriginatingPointCodeList
m3ua::OriginatingPointCodeList::OriginatingPointCodeList(){
	type = M3UA_PT_ORIGINATING_POINT_CODE_LIST;
}

// CircuitRange
m3ua::CircuitRange::CircuitRange(){
	type = M3UA_PT_CIRCUIT_RANGE;
}

// RegistrationStatus
m3ua::RegistrationStatus::RegistrationStatus(){
	type = M3UA_PT_REGISTRATION_STATUS;
}

// DeregistrationStatus
m3ua::DeregistrationStatus::DeregistrationStatus(){
	type = M3UA_PT_DEREGISTRATION_STATUS;
}


// ASPIdentifier
m3ua::ASPIdentifier::ASPIdentifier(){
	type = M3UA_PT_ASP_IDENTIFIER;
}


//ConcernedDestination
m3ua::ConcernedDestination::ConcernedDestination(){
	type = M3UA_PT_CONCERNED_DESTINATION;
}


//CongestionIndications
m3ua::CongestionIndications::CongestionIndications(){
	type = M3UA_PT_CONGESTION_INDICATIONS;
	level = 0;
}
void m3ua::CongestionIndications::init(unsigned char* data, int data_length){
	M3UAParam::init(data, data_length);
	// level
	if(value_length >=4) level = value[3] & 0xff;


}
void m3ua::CongestionIndications::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = 0x00;
		value[1] = 0x00;
		value[2] = 0x00;
		value[3] = level;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = 0x00;
	value[1] = 0x00;
	value[2] = 0x00;
	value[3] = level;


	// base encode method
	M3UAParam::encode(buffer, buffer_length, offset, result_length);

}

//CorrelationId
m3ua::CorrelationId::CorrelationId(){
	type = M3UA_PT_CORRELATION_ID;
	correlation_id = 0;
}
void m3ua::CorrelationId::init(unsigned char* data, int data_length){
	M3UAParam::init(data, data_length);
	if(value_length > 3) correlation_id = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF);

}
void m3ua::CorrelationId::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = correlation_id >> 24;
		value[1] = correlation_id >> 16;
		value[2] = correlation_id >> 8;
		value[3] = correlation_id;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = correlation_id >> 24;
	value[1] = correlation_id >> 16;
	value[2] = correlation_id >> 8;
	value[3] = correlation_id;

	// base encode method
	M3UAParam::encode(buffer, buffer_length, offset, result_length);

}

//DeregistrationResult
m3ua::DeregistrationResult::DeregistrationResult(){
	type = M3UA_PT_DEREGISTRATION_RESULT;
}

//DiagnosticInformation
m3ua::DiagnosticInformation::DiagnosticInformation(){
	type = M3UA_PT_DIAGNOSTIC_INFO;
}

//ErrorCode
m3ua::ErrorCode::ErrorCode(){
	type = M3UA_PT_ERROR_CODE;
	error_code_type = (ErrorCodeType)0;
}
void m3ua::ErrorCode::init(unsigned char* data, int data_length){
	M3UAParam::init(data, data_length);
	if(value_length >=4) error_code_type = (ErrorCodeType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
}
void m3ua::ErrorCode::set_error_code(int val){
	error_code_type = (ErrorCodeType)val;
}

void m3ua::ErrorCode::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = error_code_type >> 24;
		value[1] = error_code_type >> 16;
		value[2] = error_code_type >> 8;
		value[3] = error_code_type;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = error_code_type >> 24;
	value[1] = error_code_type >> 16;
	value[2] = error_code_type >> 8;
	value[3] = error_code_type;
	// base encode method
	M3UAParam::encode(buffer, buffer_length, offset, result_length);

}

//HeartbeatData
m3ua::HeartbeatData::HeartbeatData(){
	type = M3UA_PT_HEARTBEAT;
}

//InfoString
m3ua::InfoString::InfoString(){
	type = M3UA_PT_INFO_STRING;
}

//NetworkAppearance
m3ua::NetworkAppearance::NetworkAppearance(){
	type = M3UA_PT_NETWORK_APPEARANCE;
	network_appearance = 0;
}
void m3ua::NetworkAppearance::init(unsigned char* data, int data_length){
	M3UAParam::init(data, data_length);
	if(value_length >=4) network_appearance = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF);
}

void m3ua::NetworkAppearance::set_network_appearance(int val){
	network_appearance = val;
}

void m3ua::NetworkAppearance::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = network_appearance >> 24;
		value[1] = network_appearance >> 16;
		value[2] = network_appearance >> 8;
		value[3] = network_appearance;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = network_appearance >> 24;
	value[1] = network_appearance >> 16;
	value[2] = network_appearance >> 8;
	value[3] = network_appearance;
	// base encode method
	M3UAParam::encode(buffer, buffer_length, offset, result_length);

}


//ProtocolData
m3ua::ProtocolData::ProtocolData(){
	type = M3UA_PT_PROTOCOL_DATA;
	user_protocol_data = NULL;
	user_protocol_data_length = 0;
}
void m3ua::ProtocolData::init(unsigned char* data, int data_length){
	M3UAParam::init(data, data_length);
	originating_point_code = (data[byte_pos] << 24) + (data[byte_pos + 1] << 16) + (data[byte_pos + 2] << 8) + (data[byte_pos + 3] & 0xFF);
	byte_pos += 4;
	destination_point_code = (data[byte_pos] << 24) + (data[byte_pos + 1] << 16) + (data[byte_pos + 2] << 8) + (data[byte_pos + 3] & 0xFF);
	byte_pos += 4;
	service_indicator = (ServiceIndicatorType)(data[byte_pos++] & 0xFF);
	network_indicator = data[byte_pos++] & 0xFF;
	message_priority = data[byte_pos++] & 0xFF;
	signalling_link_selection_code = data[byte_pos++] & 0xFF;
	user_protocol_data_length = data_length - byte_pos;
	user_protocol_data = &data[byte_pos];
}
void m3ua::ProtocolData::set_dpc(uint32_t val){
	destination_point_code = val;
}
void m3ua::ProtocolData::set_opc(uint32_t val){
	originating_point_code = val;
}
void m3ua::ProtocolData::set_si(int val){
	service_indicator = (ServiceIndicatorType)val;
}
void m3ua::ProtocolData::set_ni(uint8_t val){
	network_indicator = val;
}
void m3ua::ProtocolData::set_mp(uint8_t val){
	message_priority = val;
}
void m3ua::ProtocolData::set_sls(uint8_t val){
	signalling_link_selection_code = val;
}
void m3ua::ProtocolData::set_user_protocol_data(unsigned char* protocol_data, int protocol_data_length){
	user_protocol_data = protocol_data;
	user_protocol_data_length = protocol_data_length;
}
void m3ua::ProtocolData::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	if(value == NULL){
		value_length = user_protocol_data_length + 12;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >= 12){
		value[0] = originating_point_code >> 24;
		value[1] = originating_point_code >> 16;
		value[2] = originating_point_code >> 8;
		value[3] = originating_point_code;

		value[4] = destination_point_code >> 24;
		value[5] = destination_point_code >> 16;
		value[6] = destination_point_code >> 8;
		value[7] = destination_point_code;

		value[8] = service_indicator;
		value[9] = network_indicator;
		value[10] = message_priority;
		value[11] = signalling_link_selection_code;

		if(user_protocol_data != NULL && user_protocol_data_length > 0){
			memcpy(&value[12], user_protocol_data, user_protocol_data_length);

		}

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 12 + user_protocol_data_length;
	value[0] = originating_point_code >> 24;
	value[1] = originating_point_code >> 16;
	value[2] = originating_point_code >> 8;
	value[3] = originating_point_code;

	value[4] = destination_point_code >> 24;
	value[5] = destination_point_code >> 16;
	value[6] = destination_point_code >> 8;
	value[7] = destination_point_code;

	value[8] = service_indicator;
	value[9] = network_indicator;
	value[10] = message_priority;
	value[11] = signalling_link_selection_code;

	if(user_protocol_data != NULL && user_protocol_data_length > 0){
		memcpy(&value[12], user_protocol_data, user_protocol_data_length);

	}


	// base encode method
	M3UAParam::encode(buffer, buffer_length, offset, result_length);

}

//RegistrationResult
m3ua::RegistrationResult::RegistrationResult(){
	type = M3UA_PT_REGISTRATION_RESULT;
}


//RoutingContext
m3ua::RoutingContext::RoutingContext(){
	type = M3UA_PT_ROUTING_CONTEXT;
	routing_context = 0;
}

void m3ua::RoutingContext::init(unsigned char* data, int data_length){
	M3UAParam::init(data, data_length);
	if(value_length >=4) routing_context = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF);

}

void m3ua::RoutingContext::set_routing_context(int val){
	routing_context = val;
}

void m3ua::RoutingContext::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = routing_context >> 24;
		value[1] = routing_context >> 16;
		value[2] = routing_context >> 8;
		value[3] = routing_context;

	}
	*/

	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = routing_context >> 24;
	value[1] = routing_context >> 16;
	value[2] = routing_context >> 8;
	value[3] = routing_context;
	// base encode method
	M3UAParam::encode(buffer, buffer_length, offset, result_length);

}

//RoutingKey
m3ua::RoutingKey::RoutingKey(){
	type = M3UA_PT_ROUTING_KEY;
}


//Status
m3ua::Status::Status(){
	type = M3UA_PT_STATUS;
	status_info = (StatusInfoType)0;
	status_type = (StatusType)0;
}
void m3ua::Status::init(unsigned char* data, int data_length){
	M3UAParam::init(data, data_length);
	if(value_length >=4){
		status_type = (StatusType)((data[0] << 8) + (data[1] & 0xFF));
		status_info = (StatusInfoType)((status_type << 16) + ((data[2] << 8) + (data[3] & 0xFF)));

	}

}

void m3ua::Status::set_status(StatusInfoType info){
	status_type = (StatusType)(info >> 8);
	status_info = (StatusInfoType)info;

}

void m3ua::Status::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = status_type >> 8;
		value[1] = status_type;
		value[2] = (status_info & 0x00ff) >> 8;
		value[3] = status_info;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = status_type >> 8;
	value[1] = status_type;
	value[2] = status_info >> 8;
	value[3] = status_info;
	// base encode method
	M3UAParam::encode(buffer, buffer_length, offset, result_length);

}

//TrafficModeType
m3ua::TrafficModeType::TrafficModeType(){
	type = M3UA_PT_TRAFFIC_MODE_TYPE;
	traffic_mode_type = (TMTType)0;
}

void m3ua::TrafficModeType::init(unsigned char* data, int data_length){
	M3UAParam::init(data, data_length);
	if(value_length >=4){
		traffic_mode_type = (TMTType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));

	}
}
void m3ua::TrafficModeType::set_tmt(TMTType tmt){
	traffic_mode_type = tmt;

}

void m3ua::TrafficModeType::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = traffic_mode_type >> 24;
		value[1] = traffic_mode_type >> 16;
		value[2] = traffic_mode_type >> 8;
		value[3] = traffic_mode_type;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = traffic_mode_type >> 24;
	value[1] = traffic_mode_type >> 16;
	value[2] = traffic_mode_type >> 8;
	value[3] = traffic_mode_type;
	// base encode method
	M3UAParam::encode(buffer, buffer_length, offset, result_length);

}

//UserCause
m3ua::UserCause::UserCause(){
	type= M3UA_PT_USER_CAUSE;
	user = 0;
	cause = (CauseType)0;
}
void m3ua::UserCause::init(unsigned char* data, int data_length){
	M3UAParam::init(data, data_length);
	if(value_length >=4){
		user = (data[0] << 8) + (data[1] & 0xFF);
		cause = (CauseType)((data[2] << 8) + (data[3] & 0xFF));
	}
}

void m3ua::UserCause::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = user >> 8;
		value[1] = user;
		value[2] = cause >> 8;
		value[3] = cause;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = user >> 8;
	value[1] = user;
	value[2] = cause >> 8;
	value[3] = cause;
	// base encode method
	M3UAParam::encode(buffer, buffer_length, offset, result_length);

}


// m3ua M3UA Packet
m3ua::M3UAPacket::M3UAPacket(): version(1),
				message_class(__M3UA_MESSAGE_CLASS_UNKNOWN__),
				message_type(__M3UA_MESSAGE_TYPE_UNKNOWN__),
				message_length(0),
				message(NULL){}

m3ua::M3UAPacket::~M3UAPacket(){}


void m3ua::M3UAPacket::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	unsigned char* tmp_pos;
	int total_length = 8;
	// offset
	buffer += offset;
	// common header
	buffer[0] = version;
	buffer[1] = 0x00;
	buffer[2] = message_class;
	buffer[3] = message_type;
	buffer += 4;
	// save position for length values which are calculated later
	tmp_pos = buffer;
	if(message != NULL){
		// m3ua parameters, encode and return total length(data + padding)
		// offset of extra 4 bytes for length which is calculated later
		message->encode(buffer, buffer_length, 4, result_length);
		total_length += *result_length;

	}
	// restore position
	buffer = tmp_pos;
	buffer[0] = total_length >> 24;
	buffer[1] = total_length >> 16;
	buffer[2] = total_length >> 8;
	buffer[3] = total_length & 0xFF;
	// result length
	*result_length = total_length;


}

// m3ua M3UAMessage

void m3ua::M3UAMessage::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int param_length = 0;
	M3UAParam* tmp_param = NULL;
	int tmp_pl = 0;
	for(unsigned int i = 0; i<params.size(); i++){
		tmp_param = params[i];
		if(tmp_param != NULL){
		    tmp_pl = encode_parameter(tmp_param, buffer, buffer_length, offset);
		    param_length += tmp_pl;
		    offset += tmp_pl;

		}
	}
	*result_length = param_length;
}


m3ua::M3UAMessage::M3UAMessage(){
	byte_pos = 0;
}
m3ua::M3UAMessage::~M3UAMessage(){
    /*
	M3UAParam* tmp_param = NULL;
	while(params.size() > 0){
		tmp_param = params[0];
		if(tmp_param != NULL) {
			params.erase(params.begin());
			delete tmp_param;
		}
	}
	*/
}

void m3ua::M3UAMessage::init(unsigned char* data, unsigned int data_length, M3UAPool* _m3ua_pool){
	if(data != NULL && data_length > 0) process_params(data, data_length, _m3ua_pool);
}



void m3ua::M3UAMessage::process_params(unsigned char* data, unsigned int data_length, M3UAPool* _m3ua_pool){
	if(data != NULL && data_length > 0){
		ParameterType pt;
		M3UAParam* m3ua_param = NULL;
		unsigned int tag;
		unsigned int l;
		unsigned int m;
		while(byte_pos < data_length){
			tag = (data[byte_pos] << 8) + (data[byte_pos + 1] & 0xFF);
			byte_pos += 2;
			// length(l) = two bytes for tag + two bytes for length + length of actual data
			l = (data[byte_pos] << 8) + (data[byte_pos + 1] & 0xFF);
			byte_pos += 2;
			pt = (ParameterType)(tag);
			// create param
			//m3ua_param = get_param(pt);
			m3ua_param = _m3ua_pool->request_param(pt);
			if(m3ua_param != NULL){
				m3ua_param->byte_pos = 0;
				m3ua_param->init(&data[byte_pos], l - 4);
				params.push_back(m3ua_param);
			}
			// parameter has to be a multiple of 4, if not, zero padding is added
			m = l % 4;
			byte_pos += l + (m > 0 ? 4 - m : 0) - 4;

		}
	}

}
int m3ua::M3UAMessage::encode_parameter(M3UAParam* param, unsigned char* buffer, int buffer_length, int offset){
	if(buffer != NULL && param != NULL && buffer_length > 0){
		int l = 0;
		int padding_count = 0;
		int m;
		//unsigned char* res = NULL;
		int param_res_length = 0;
		param->encode(buffer, buffer_length, offset, &param_res_length);
		l = param_res_length;
		// parameter has to be a multiple of 4, if not, zero padding is added
		// padding has to be included in total message length
		m = l % 4;
		padding_count = (m > 0 ? 4 - m : 0);
		l += padding_count;
		// parameter value(tag + length + value)
		if(param_res_length + offset + padding_count<= buffer_length){
			// padding
			for(int i = 0; i<padding_count; i++) buffer[i + offset + param_res_length] = 0x00;
			// result = parameter length + padding
			return l;
		}


	}
	return 0;

}

int m3ua::M3UAMessage::get_params_count(){
	return params.size();
}

m3ua::M3UAParam* m3ua::M3UAMessage::get_parameter(ParameterType _param_type){
	M3UAParam* tmp_param = NULL;
	for(unsigned int i = 0; i<params.size(); i++){
		tmp_param = params[i];
		if(tmp_param != NULL){
			if(tmp_param->type == _param_type) return tmp_param;
		}
	}
	return NULL;
}

m3ua::M3UAParam* m3ua::M3UAMessage::add_parameter(M3UAParam* param){
	 //M3UAParam* tmp_param = m3ua::get_param(_param_type);
	 if(param != NULL) params.push_back(param);
	 return param;


}
void m3ua::M3UAMessage::clear_params(){
    params.clear();
    /*
	M3UAParam* tmp_param = NULL;
	while(params.size() > 0){
		tmp_param = params[0];
		if(tmp_param != NULL) {
			params.erase(params.begin());
			delete tmp_param;
		}
	}
*/
}


// m3ua Param
m3ua::M3UAParam::M3UAParam(){
	value_length = 0;
	value = NULL;
	byte_pos = 0;
	//new_value_mem = false;
}
m3ua::M3UAParam::~M3UAParam(){
	//if(value != NULL && new_value_mem) delete value;
}
void m3ua::M3UAParam::init(unsigned char* data, int data_length){
	// set data pointer and length
	value = data;
	value_length = data_length;

}

void m3ua::M3UAParam::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;
	// parameter type = two bytes
	// parameter length = two bytes
	// data
	buffer[0] = type >> 8;
	buffer[1] = type;
	buffer[2] = (value_length + 4) >> 8;
	buffer[3] = value_length + 4;

	// if value already in buffer (buffer and value addresses are equal), skip
	if(&value[0] != &buffer[4]){
		//std::cout << "m3ua::M3UAParam::encode> " << (int*)&value[0] << " != " << (int*)&buffer[4] << std::endl;
		// get actual data
		memcpy(&buffer[4], value, value_length);
		//for(int i = 0; i<value_length; i++) buffer[i + 4] = value[i];

	}
	// result length
	*result_length = 4 + value_length;
}



// main m3ua param detect method
m3ua::M3UAParam* m3ua::get_param(ParameterType _param_type){
	M3UAParam* param = NULL;
	switch(_param_type){
		// Common parameters
		case M3UA_PT_INFO_STRING: param = new InfoString(); break;
		case M3UA_PT_ROUTING_CONTEXT: param = new RoutingContext(); break;
		case M3UA_PT_DIAGNOSTIC_INFO: param = new DiagnosticInformation(); break;
		case M3UA_PT_HEARTBEAT: param = new HeartbeatData(); break;
		case M3UA_PT_TRAFFIC_MODE_TYPE: param = new TrafficModeType(); break;
		case M3UA_PT_ERROR_CODE: param = new ErrorCode(); break;
		case M3UA_PT_STATUS: param = new Status(); break;
		case M3UA_PT_ASP_IDENTIFIER: param = new ASPIdentifier(); break;
		case M3UA_PT_AFFECTED_POINT_CODE: param = new AffectedPointCode(); break;
		case M3UA_PT_CORRELATION_ID: param = new CorrelationId(); break;
		// m3ua specific
		case M3UA_PT_NETWORK_APPEARANCE: param = new NetworkAppearance(); break;
		case M3UA_PT_USER_CAUSE: param = new UserCause(); break;
		case M3UA_PT_CONGESTION_INDICATIONS: param = new CongestionIndications(); break;
		case M3UA_PT_CONCERNED_DESTINATION: param = new ConcernedDestination(); break;
		case M3UA_PT_ROUTING_KEY: param = new RoutingKey(); break;
		case M3UA_PT_REGISTRATION_RESULT: param = new RegistrationResult(); break;
		case M3UA_PT_DEREGISTRATION_RESULT: param = new DeregistrationResult(); break;
		case M3UA_PT_LOCAL_ROUTING_KEY_IDENTIFIER: param = new LocalRoutingKeyId(); break;
		case M3UA_PT_DESTINATION_POINT_CODE: param = new DestinationPointCode(); break;
		case M3UA_PT_SERVICE_INDICATORS: param = new ServiceIndicators(); break;
		case M3UA_PT_ORIGINATING_POINT_CODE_LIST: param = new OriginatingPointCodeList(); break;
		case M3UA_PT_CIRCUIT_RANGE: param = new CircuitRange(); break;
		case M3UA_PT_PROTOCOL_DATA: param = new ProtocolData(); break;
		case M3UA_PT_REGISTRATION_STATUS: param = new RegistrationStatus(); break;
		case M3UA_PT_DEREGISTRATION_STATUS: param = new DeregistrationStatus(); break;
	}
	return param;
}

// M3UAMessagePoolItem
m3ua::M3UAMessagePoolItem::M3UAMessagePoolItem(){
	pool = NULL;
	next_free_item = NULL;
	total_count = 0;
	free_count = 0;

}

m3ua::M3UAMessagePoolItem::~M3UAMessagePoolItem(){
    for(int i = 0; i<total_count; i++) delete pool[i];
    delete[] pool;

}

m3ua::M3UAMessage* m3ua::M3UAMessagePoolItem::create_message(MessageType _msg_type){
	M3UAMessage* m3uam = NULL;
	switch(_msg_type){
		case __M3UA_MESSAGE_TYPE_UNKNOWN__: break;
		case ASPAC: m3uam = new Aspac(); break;
		case ASPAC_ACK: m3uam = new Aspac_ack(); break;
		case ASPDN: m3uam = new Aspdn(); break;
		case ASPDN_ACK: m3uam = new Aspdn_ack(); break;
		case ASPIA: m3uam = new Aspia(); break;
		case ASPIA_ACK: m3uam = new Aspia_ack(); break;
		case ASPUP: m3uam = new Aspup(); break;
		case ASPUP_ACK: m3uam = new Aspup_ack(); break;
		case BEAT: m3uam = new Beat(); break;
		case BEAT_ACK: m3uam = new Beat_ack(); break;
		case DATA: m3uam = new Data(); break;
		case DAUD: m3uam = new Daud(); break;
		case DAVA: m3uam = new Dava(); break;
		case DEREG_REQ: m3uam = new Dereg_req(); break;
		case DEREG_RSP: m3uam = new Dereg_rsp(); break;
		case DRST: m3uam = new Drst(); break;
		case DUNA: m3uam = new Duna(); break;
		case DUPU: m3uam = new Dupu(); break;
		case M3UA_ERR: m3uam = new Err(); break;
		case NTFY: m3uam = new Ntfy(); break;
		case REG_REQ: m3uam = new Reg_req(); break;
		case REG_RSP: m3uam = new Reg_rsp(); break;
		case SCON: m3uam = new Scon(); break;

	}

	return m3uam;
}

void m3ua::M3UAMessagePoolItem::init_pool(){
	pool = new M3UAMessage *[total_count];
	for(int i = 0; i<total_count; i++) pool[i] = create_message(type);
	next_free_item = pool[total_count - 1];
	//free_count--;
}

void m3ua::M3UAMessagePoolItem::set_pool_size(int _total_count){
	total_count = _total_count;
	free_count = total_count;
}

m3ua::M3UAMessage* m3ua::M3UAMessagePoolItem::request_item(){
	M3UAMessage* tmp = next_free_item;
	--free_count;
	if(free_count <= 0) free_count = total_count;
	next_free_item = pool[free_count - 1];

	return tmp;

	/*
	M3UAMessage* msg = next_free_item;
	if(msg != NULL){
		free_count--;
		if(free_count > 0) next_free_item = pool[free_count - 1];
		else next_free_item = NULL;

	}
	return msg;
	*/
}


// M3UAParamPoolItem
m3ua::M3UAParamPoolItem::M3UAParamPoolItem(){
	pool = NULL;
	next_free_item = NULL;
	total_count = 0;
	free_count = 0;
}

m3ua::M3UAParamPoolItem::~M3UAParamPoolItem(){
    for(int i = 0; i<total_count; i++) delete pool[i];
    delete[] pool;

}

m3ua::M3UAParam* m3ua::M3UAParamPoolItem::create_param(ParameterType _param_type){
	M3UAParam* param = NULL;
	switch(_param_type){
		// Common parameters
		case M3UA_PT_INFO_STRING: param = new InfoString(); break;
		case M3UA_PT_ROUTING_CONTEXT: param = new RoutingContext(); break;
		case M3UA_PT_DIAGNOSTIC_INFO: param = new DiagnosticInformation(); break;
		case M3UA_PT_HEARTBEAT: param = new HeartbeatData(); break;
		case M3UA_PT_TRAFFIC_MODE_TYPE: param = new TrafficModeType(); break;
		case M3UA_PT_ERROR_CODE: param = new ErrorCode(); break;
		case M3UA_PT_STATUS: param = new Status(); break;
		case M3UA_PT_ASP_IDENTIFIER: param = new ASPIdentifier(); break;
		case M3UA_PT_AFFECTED_POINT_CODE: param = new AffectedPointCode(); break;
		case M3UA_PT_CORRELATION_ID: param = new CorrelationId(); break;
		// m3ua specific
		case M3UA_PT_NETWORK_APPEARANCE: param = new NetworkAppearance(); break;
		case M3UA_PT_USER_CAUSE: param = new UserCause(); break;
		case M3UA_PT_CONGESTION_INDICATIONS: param = new CongestionIndications(); break;
		case M3UA_PT_CONCERNED_DESTINATION: param = new ConcernedDestination(); break;
		case M3UA_PT_ROUTING_KEY: param = new RoutingKey(); break;
		case M3UA_PT_REGISTRATION_RESULT: param = new RegistrationResult(); break;
		case M3UA_PT_DEREGISTRATION_RESULT: param = new DeregistrationResult(); break;
		case M3UA_PT_LOCAL_ROUTING_KEY_IDENTIFIER: param = new LocalRoutingKeyId(); break;
		case M3UA_PT_DESTINATION_POINT_CODE: param = new DestinationPointCode(); break;
		case M3UA_PT_SERVICE_INDICATORS: param = new ServiceIndicators(); break;
		case M3UA_PT_ORIGINATING_POINT_CODE_LIST: param = new OriginatingPointCodeList(); break;
		case M3UA_PT_CIRCUIT_RANGE: param = new CircuitRange(); break;
		case M3UA_PT_PROTOCOL_DATA: param = new ProtocolData(); break;
		case M3UA_PT_REGISTRATION_STATUS: param = new RegistrationStatus(); break;
		case M3UA_PT_DEREGISTRATION_STATUS: param = new DeregistrationStatus(); break;
	}
	return param;
}

void m3ua::M3UAParamPoolItem::init_pool(){
	pool = new M3UAParam *[total_count];
	for(int i = 0; i<total_count; i++) pool[i] = create_param(type);
	next_free_item = pool[total_count - 1];
	//free_count--;
}

void m3ua::M3UAParamPoolItem::set_pool_size(int _total_count){
	total_count = _total_count;
	free_count = total_count;
}

m3ua::M3UAParam* m3ua::M3UAParamPoolItem::request_item(){
	M3UAParam* tmp = next_free_item;
	--free_count;
	if(free_count <= 0) free_count = total_count;
	next_free_item = pool[free_count - 1];

	return tmp;
	/*
	M3UAParam* param = next_free_item;
	if(param != NULL){
		free_count--;
		if(free_count > 0) next_free_item = pool[free_count - 1];
		else next_free_item = NULL;

	}
	return param;
	*/
}


// M3UAPool
m3ua::M3UAPool::M3UAPool(): param_count(5),
			    message_count(1) {}

m3ua::M3UAPool::~M3UAPool(){
    M3UAMessagePoolItem* tmp_msg = NULL;
    std::map<MessageType, M3UAMessagePoolItem*>::iterator it = MESSAGE_POOL.begin();
    for(; it != MESSAGE_POOL.end(); it++){
	tmp_msg = it->second;
	delete tmp_msg;
    }

    M3UAParamPoolItem* tmp_param = NULL;
    std::map<ParameterType, M3UAParamPoolItem*>::iterator it2 = PARAM_POOL.begin();
    for(; it2 != PARAM_POOL.end(); it2++){
	tmp_param = it2->second;
	delete tmp_param;

    }
}

void m3ua::M3UAPool::set_pool_size(int _param_count, int _message_count){
	param_count = _param_count;
	message_count = _message_count;
}
void m3ua::M3UAPool::init_message(MessageType _msg_type){
	MESSAGE_POOL[_msg_type] = new M3UAMessagePoolItem();//[message_count];
	MESSAGE_POOL[_msg_type]->type = _msg_type;
	MESSAGE_POOL[_msg_type]->set_pool_size(message_count);
	MESSAGE_POOL[_msg_type]->init_pool();

}

void m3ua::M3UAPool::init_param(ParameterType _param_type){
	PARAM_POOL[_param_type] = new M3UAParamPoolItem();//[param_count];
	PARAM_POOL[_param_type]->type = _param_type;
	PARAM_POOL[_param_type]->set_pool_size(param_count);
	PARAM_POOL[_param_type]->init_pool();
}



m3ua::M3UAMessage* m3ua::M3UAPool::request_message(MessageType msg_type){
    std::map<MessageType, M3UAMessagePoolItem*>::iterator it = MESSAGE_POOL.find(msg_type);
    if(it != MESSAGE_POOL.end()) return it->second->request_item();
    return NULL;
    //return MESSAGE_POOL[msg_type]->request_item();
}


m3ua::M3UAParam* m3ua::M3UAPool::request_param(ParameterType param_type){
    std::map<ParameterType, M3UAParamPoolItem*>::iterator it = PARAM_POOL.find(param_type);
    if(it != PARAM_POOL.end()) return it->second->request_item();
    return NULL;
    //return PARAM_POOL[param_type]->request_item();
}



void m3ua::M3UAPool::init_pool(){
	// message pool
	init_message(ASPAC);
	init_message(ASPAC_ACK);
	init_message(ASPDN);
	init_message(ASPDN_ACK);
	init_message(ASPIA);
	init_message(ASPIA_ACK);
	init_message(ASPUP);
	init_message(ASPUP_ACK);
	init_message(BEAT);
	init_message(BEAT_ACK);
	init_message(DATA);
	init_message(DAUD);
	init_message(DAVA);
	init_message(DEREG_REQ);
	init_message(DEREG_RSP);
	init_message(DRST);
	init_message(DUNA);
	init_message(DUPU);
	init_message(M3UA_ERR);
	init_message(NTFY);
	init_message(REG_REQ);
	init_message(REG_RSP);
	init_message(SCON);

	// param pool
	init_param(M3UA_PT_INFO_STRING);
	init_param(M3UA_PT_ROUTING_CONTEXT);
	init_param(M3UA_PT_DIAGNOSTIC_INFO);
	init_param(M3UA_PT_HEARTBEAT);
	init_param(M3UA_PT_TRAFFIC_MODE_TYPE);
	init_param(M3UA_PT_ERROR_CODE);
	init_param(M3UA_PT_STATUS);
	init_param(M3UA_PT_ASP_IDENTIFIER);
	init_param(M3UA_PT_AFFECTED_POINT_CODE);
	init_param(M3UA_PT_CORRELATION_ID);
	init_param(M3UA_PT_NETWORK_APPEARANCE);
	init_param(M3UA_PT_USER_CAUSE);
	init_param(M3UA_PT_CONGESTION_INDICATIONS);
	init_param(M3UA_PT_CONCERNED_DESTINATION);
	init_param(M3UA_PT_ROUTING_KEY);
	init_param(M3UA_PT_REGISTRATION_RESULT);
	init_param(M3UA_PT_DEREGISTRATION_RESULT);
	init_param(M3UA_PT_LOCAL_ROUTING_KEY_IDENTIFIER);
	init_param(M3UA_PT_DESTINATION_POINT_CODE);
	init_param(M3UA_PT_SERVICE_INDICATORS);
	init_param(M3UA_PT_ORIGINATING_POINT_CODE_LIST);
	init_param(M3UA_PT_CIRCUIT_RANGE);
	init_param(M3UA_PT_PROTOCOL_DATA);
	init_param(M3UA_PT_REGISTRATION_STATUS);
	init_param(M3UA_PT_DEREGISTRATION_STATUS);
}

// main m3ua decode method using memory pool
int m3ua::decode(unsigned char* data, int data_length, M3UAPacket* _m3uap, M3UAPool* _m3ua_pool){
	if(data != NULL && data_length > 0){
		// min header length
		if(data_length >= 8){
			_m3uap->version = *data & 0xFF;
			// one byte version, one byte reserved
			data += 2;
			// only version 1.0 supported
			if(_m3uap->version == 1){
				_m3uap->message_class = (MessageClass)(*data & 0xFF);
				++data;
				_m3uap->message_type = (MessageType)((_m3uap->message_class << 8) + (*data & 0xFF));
				++data;
				_m3uap->message_length = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF);
				data += 4;
				if(_m3uap->message_length > 0){
					_m3uap->message = _m3ua_pool->request_message(_m3uap->message_type);
					if(_m3uap->message != NULL){
						_m3uap->message->byte_pos = 0;
						_m3uap->message->params.clear();
						_m3uap->message->init(data, data_length - 8, _m3ua_pool);
					}


				}
				// ok
				return 0;

			// unsupported version
			}else return 2;

		}
	}
	// err
	return 1;

}

// main m3ua decode method
m3ua::M3UAPacket* m3ua::decode(unsigned char* data, int data_length){
	if(data != NULL && data_length > 0){
		M3UAPacket* m3uap = new M3UAPacket();
		// min header length
		if(data_length >=8){
			m3uap->version = *data & 0xFF;
			// one byte version, one byte reserved
			data += 2;
			// only version 1.0 supported
			if(m3uap->version == 1){
				m3uap->message_class = (MessageClass)(*data & 0xFF);
				data++;
				m3uap->message_type = (MessageType)((m3uap->message_class << 8) + (*data & 0xFF));
				data++;
				m3uap->message_length = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF);
				data += 4;
				if(m3uap->message_length > 0){
					switch(m3uap->message_type){
						case __M3UA_MESSAGE_TYPE_UNKNOWN__: break;
						case ASPAC: m3uap->message = new Aspac(); break;
						case ASPAC_ACK: m3uap->message = new Aspac_ack(); break;
						case ASPDN: m3uap->message = new Aspdn(); break;
						case ASPDN_ACK: m3uap->message = new Aspdn_ack(); break;
						case ASPIA: m3uap->message = new Aspia(); break;
						case ASPIA_ACK: m3uap->message = new Aspia_ack(); break;
						case ASPUP: m3uap->message = new Aspup(); break;
						case ASPUP_ACK: m3uap->message = new Aspup_ack(); break;
						case BEAT: m3uap->message = new Beat(); break;
						case BEAT_ACK: m3uap->message = new Beat_ack(); break;
						case DATA: m3uap->message = new Data(); break;
						case DAUD: m3uap->message = new Daud(); break;
						case DAVA: m3uap->message = new Dava(); break;
						case DEREG_REQ: m3uap->message = new Dereg_req(); break;
						case DEREG_RSP: m3uap->message = new Dereg_rsp(); break;
						case DRST: m3uap->message = new Drst(); break;
						case DUNA: m3uap->message = new Duna(); break;
						case DUPU: m3uap->message = new Dupu(); break;
						case M3UA_ERR: m3uap->message = new Err(); break;
						case NTFY: m3uap->message = new Ntfy(); break;
						case REG_REQ: m3uap->message = new Reg_req(); break;
						case REG_RSP: m3uap->message = new Reg_rsp(); break;
						case SCON: m3uap->message = new Scon(); break;

					}

					if(m3uap->message != NULL) m3uap->message->init(data, data_length - 8, NULL);


				}
			}

		}

		return m3uap;
	}
	return NULL;
}


