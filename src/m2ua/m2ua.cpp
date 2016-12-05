#include<m2ua.h>
#include<string.h>



// ======= PARAMETERS =============
// base M2UA parameter
m2ua::M2UAParam::M2UAParam(){
	value_length = 0;
	value = NULL;
	byte_pos = 0;
	//new_value_mem = false;
}
m2ua::M2UAParam::~M2UAParam(){
	//if(value != NULL && new_value_mem) delete value;
}
void m2ua::M2UAParam::init(char* data, int data_length){
	// set data pointer and length
	value = data;
	value_length = data_length;

}

void m2ua::M2UAParam::encode(char* buffer, int buffer_length, int offset, int* result_length){
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
		// get actual data
		memcpy(&buffer[4], value, value_length);

	}
	// result length
	*result_length = 4 + value_length;
}

// Protocol Data
m2ua::ProtocolData::ProtocolData(){
	type = PROTOCOL_DATA_1;
}

// Protocol Data TTC
m2ua::ProtocolData_TTC::ProtocolData_TTC(){
	type = PROTOCOL_DATA_2_TTC;
}

// Correlation Id
m2ua::Correlation_id::Correlation_id(){
	type = CORRELATION_ID;
}

// Sequence number
m2ua::SequenceNumber::SequenceNumber(){
	type = SEQUENCE_NUMBER;
}

// StateRequest
m2ua::StateRequest::StateRequest(){
	type = STATE_REQUEST_PARAM;
}

void m2ua::StateRequest::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		state = (StateType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::StateRequest:: encode(char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = state >> 24;
		value[1] = state >> 16;
		value[2] = state >> 8;
		value[3] = state;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = state >> 24;
	value[1] = state >> 16;
	value[2] = state >> 8;
	value[3] = state;

	// base encode method
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}


// StateEvent
m2ua::StateEvent::StateEvent(){
	type = STATE_EVENT;
}

void m2ua::StateEvent::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		event = (EventType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::StateEvent:: encode(char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = event >> 24;
		value[1] = event >> 16;
		value[2] = event >> 8;
		value[3] = event;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = event >> 24;
	value[1] = event >> 16;
	value[2] = event >> 8;
	value[3] = event;

	// base encode method
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}


// CongestionStatus
m2ua::CongestionStatus::CongestionStatus(){
	type = CONGESTION_STATUS;
}

void m2ua::CongestionStatus::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		congestion_status = (CongestionDiscardStatusType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::CongestionStatus:: encode(char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = congestion_status >> 24;
		value[1] = congestion_status >> 16;
		value[2] = congestion_status >> 8;
		value[3] = congestion_status;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = congestion_status >> 24;
	value[1] = congestion_status >> 16;
	value[2] = congestion_status >> 8;
	value[3] = congestion_status;

	// base encode method
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}


// DiscardStatus
m2ua::DiscardStatus::DiscardStatus(){
	type = DISCARD_STATUS;
}

void m2ua::DiscardStatus::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		discard_status = (CongestionDiscardStatusType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::DiscardStatus:: encode(char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = discard_status >> 24;
		value[1] = discard_status >> 16;
		value[2] = discard_status >> 8;
		value[3] = discard_status;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = discard_status >> 24;
	value[1] = discard_status >> 16;
	value[2] = discard_status >> 8;
	value[3] = discard_status;

	// base encode method
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}

// Action
m2ua::Action::Action(){
	type = ACTION;
}

void m2ua::Action::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		action = (ActionType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::Action:: encode(char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = action >> 24;
		value[1] = action >> 16;
		value[2] = action >> 8;
		value[3] = action;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = action >> 24;
	value[1] = action >> 16;
	value[2] = action >> 8;
	value[3] = action;

	// base encode method
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}

// RetrievalResult
m2ua::RetrievalResult::RetrievalResult(){
	type = RETRIEVAL_RESULT;
}

void m2ua::RetrievalResult::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		result = (ResultType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::RetrievalResult:: encode(char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = result >> 24;
		value[1] = result >> 16;
		value[2] = result >> 8;
		value[3] = result;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = result >> 24;
	value[1] = result >> 16;
	value[2] = result >> 8;
	value[3] = result;

	// base encode method
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}

// Asp_identifier
m2ua::Asp_identifier::Asp_identifier(){
	type = ASP_IDENTIFIER;
}

// Info_string
m2ua::Info_string::Info_string(){
	type = INFO_STRING;
}

// Heartbeat_data
m2ua::Heartbeat_data::Heartbeat_data(){
	type = HEARTBEAT_DATA;
}

// TrafficModeType
m2ua::TrafficModeType::TrafficModeType(){
	type = TRAFFIC_MODE_TYPE;
}

void m2ua::TrafficModeType::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		tmt_type = (TMTType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::TrafficModeType:: encode(char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = tmt_type >> 24;
		value[1] = tmt_type >> 16;
		value[2] = tmt_type >> 8;
		value[3] = tmt_type;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = tmt_type >> 24;
	value[1] = tmt_type >> 16;
	value[2] = tmt_type >> 8;
	value[3] = tmt_type;

	// base encode method
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}

// InterfaceIdentifier_int_range
m2ua::InterfaceIdentifier_int_range::InterfaceIdentifier_int_range(){
	type = INTERFACE_INDICATOR_INTEGER_RANGE;
}

// InterfaceIdentifier_int
m2ua::InterfaceIdentifier_int::InterfaceIdentifier_int(){
	type = INTERFACE_INDICATOR_INTEGER;
}

// InterfaceIdentifier_txt
m2ua::InterfaceIdentifier_txt::InterfaceIdentifier_txt(){
	type = INTERFACE_INDICATOR_TEXT;
}

// ErrorCode
m2ua::ErrorCode::ErrorCode(){
	type = ERROR_CODE;
}

void m2ua::ErrorCode::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		error_code = (ErrorCodeType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::ErrorCode:: encode(char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = error_code >> 24;
		value[1] = error_code >> 16;
		value[2] = error_code >> 8;
		value[3] = error_code;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = error_code >> 24;
	value[1] = error_code >> 16;
	value[2] = error_code >> 8;
	value[3] = error_code;

	// base encode method
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}

// DiagnosticInformation
m2ua::DiagnosticInformation::DiagnosticInformation(){
	type = DIAGNOSTIC_INFORMATION;
}

// StatusTypeInfo
m2ua::StatusTypeInfo::StatusTypeInfo(){
	type = STATUS_TYPE_INFO;
}

void m2ua::StatusTypeInfo::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		status_type = (StatusType)((data[0] << 8) + (data[1] & 0xFF));
		status_info = (StatusInformationType)((data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::StatusTypeInfo:: encode(char* buffer, int buffer_length, int offset, int* result_length){
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
		value[2] = status_info >> 8;
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
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}

// LocalLKId
m2ua::LocalLKId::LocalLKId(){
	type = LOCAL_LK_IDENTIFIER;
}

// SignallingDTId
m2ua::SignallingDTId::SignallingDTId(){
	type = SDT_IDENTIFIER;
}

// SignallingDLId
m2ua::SignallingDLId::SignallingDLId(){
	type = SDL_IDENTIFIER;
}

// LinkKey
m2ua::LinkKey::LinkKey(){
	type = LINK_KEY;
}

// RegistrationStatus
m2ua::RegistrationStatus::RegistrationStatus(){
	type = REGISTRATION_STATUS;
}

void m2ua::RegistrationStatus::init(char* data, int data_length){
	M2UAParam::init(data, data_length);
	if(value_length >=4){
		registration_status = (RegistrationStatusType)((data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF));
	}

}

void m2ua::RegistrationStatus:: encode(char* buffer, int buffer_length, int offset, int* result_length){
	/*
	// if null allocate
	if(value == NULL){
		value = new char[4];
		value_length = 4;
		new_value_mem = true;
	}
	if(value_length >= 4){
		value[0] = registration_status >> 24;
		value[1] = registration_status >> 16;
		value[2] = registration_status >> 8;
		value[3] = registration_status;

	}
	*/
	// skip 4 bytes - 2 byte param tag, 2 byte param length, encoded later in M3UAParam::encode
	value = &buffer[offset + 4];
	value_length = 4;
	value[0] = registration_status >> 24;
	value[1] = registration_status >> 16;
	value[2] = registration_status >> 8;
	value[3] = registration_status;

	// base encode method
	M2UAParam::encode(buffer, buffer_length, offset, result_length);


}

// RegistrationResult
m2ua::RegistrationResult::RegistrationResult(){
	type = REGISTRATION_RESULT;
}

// DeregistrationStatus
m2ua::DeregistrationStatus::DeregistrationStatus(){
	type = DEREGISTRATION_STATUS;
}

// DeregistrationResult
m2ua::DeregistrationResult::DeregistrationResult(){
	type = DEREGISTRATION_RESULT;
}

// ======= MESSAGES =============

// base M2UA message
m2ua::M2UAMessage::M2UAMessage(){
	byte_pos = 0;
}
m2ua::M2UAMessage::~M2UAMessage(){
	M2UAParam* tmp_param = NULL;
	while(params.size() > 0){
		tmp_param = params[0];
		if(tmp_param != NULL) {
			params.erase(params.begin());
			delete tmp_param;
		}
	}
}
void m2ua::M2UAMessage::init(char* data, int data_length){
	if(data != NULL && data_length > 0) process_params(data, data_length);

}

void m2ua::M2UAMessage::init_new(){}

void m2ua::M2UAMessage::encode(char* buffer, int buffer_length, int offset, int* result_length){
	int param_length = 0;
	M2UAParam* tmp_param = NULL;
	for(int i = 0; i<params.size(); i++){
		tmp_param = params[i];
		if(tmp_param != NULL){
			param_length += encode_parameter(tmp_param, buffer, buffer_length, offset);
			offset += param_length;

		}
	}
	*result_length = param_length;
}

int m2ua::M2UAMessage::get_params_count(){
	return params.size();
}

m2ua::M2UAParam* m2ua::M2UAMessage::get_parameter(ParameterType _param_type){
	M2UAParam* tmp_param = NULL;
	for(int i = 0; i<params.size(); i++){
		tmp_param = params[i];
		if(tmp_param != NULL){
			if(tmp_param->type == _param_type) return tmp_param;
		}
	}
	return NULL;
}

m2ua::M2UAParam* m2ua::M2UAMessage::add_parameter(ParameterType _param_type){
	M2UAParam* tmp_param = m2ua::get_param(_param_type);
	 if(tmp_param != NULL) params.push_back(tmp_param);
	 return tmp_param;


}
void m2ua::M2UAMessage::clear_params(){
	M2UAParam* tmp_param = NULL;
	while(params.size() > 0){
		tmp_param = params[0];
		if(tmp_param != NULL) {
			params.erase(params.begin());
			delete tmp_param;
		}
	}

}



// Data
m2ua::Data::Data(){
	type = DATA;
}

bool m2ua::Data::check_mandatory(){
	return true;
}

// Data ack
m2ua::Data_ack::Data_ack(){
	type = DATA_ACK;
}

bool m2ua::Data_ack::check_mandatory(){
	return true;
}

// Establish_request
m2ua::Establish_request::Establish_request(){
	type = ESTABLISH_REQUEST;
}

bool m2ua::Establish_request::check_mandatory(){
	return true;
}

// Establish_confirm
m2ua::Establish_confirm::Establish_confirm(){
	type = ESTABLISH_CONFIRM;
}

bool m2ua::Establish_confirm::check_mandatory(){
	return true;
}

// Release_request
m2ua::Release_request::Release_request(){
	type = RELEASE_REQUEST;
}

bool m2ua::Release_request::check_mandatory(){
	return true;
}

// Release_indication
m2ua::Release_indication::Release_indication(){
	type = RELEASE_INDICATION;
}

bool m2ua::Release_indication::check_mandatory(){
	return true;
}

// Release_confirm
m2ua::Release_confirm::Release_confirm(){
	type = RELEASE_CONFIRM;
}

bool m2ua::Release_confirm::check_mandatory(){
	return true;
}


// State_request
m2ua::State_request::State_request(){
	type = STATE_REQUEST;
}

bool m2ua::State_request::check_mandatory(){
	return true;
}


// State_confirm
m2ua::State_confirm::State_confirm(){
	type = STATE_CONFIRM;
}

bool m2ua::State_confirm::check_mandatory(){
	return true;
}

// State_indication
m2ua::State_indication::State_indication(){
	type = STATE_INDICATION;
}

bool m2ua::State_indication::check_mandatory(){
	return true;
}

// Congestion_indication
m2ua::Congestion_indication::Congestion_indication(){
	type = CONGESTION_INDICATION;
}

bool m2ua::Congestion_indication::check_mandatory(){
	return true;
}

// Retrieval_request
m2ua::Retrieval_request::Retrieval_request(){
	type = DATA_RETRIEVAL_REQUEST;
}

bool m2ua::Retrieval_request::check_mandatory(){
	return true;
}

// Retrieval_confirm
m2ua::Retrieval_confirm::Retrieval_confirm(){
	type = DATA_RETRIEVAL_CONFIRM;
}

bool m2ua::Retrieval_confirm::check_mandatory(){
	return true;
}

// Retrieval_indication
m2ua::Retrieval_indication::Retrieval_indication(){
	type = DATA_RETRIEVAL_INDICATION;
}

bool m2ua::Retrieval_indication::check_mandatory(){
	return true;
}

// Retrieval_complete_indication
m2ua::Retrieval_complete_indication::Retrieval_complete_indication(){
	type = DATA_RETRIEVAL_COMPLETE_INDICATION;
}

bool m2ua::Retrieval_complete_indication::check_mandatory(){
	return true;
}

// Asp_up
m2ua::Asp_up::Asp_up(){
	type = ASP_UP;
}

bool m2ua::Asp_up::check_mandatory(){
	return true;
}

// Asp_up_ack
m2ua::Asp_up_ack::Asp_up_ack(){
	type = ASP_UP_ACK;
}

bool m2ua::Asp_up_ack::check_mandatory(){
	return true;
}

// Asp_down
m2ua::Asp_down::Asp_down(){
	type = ASP_DOWN;
}

bool m2ua::Asp_down::check_mandatory(){
	return true;
}

// Asp_down_ack
m2ua::Asp_down_ack::Asp_down_ack(){
	type = ASP_DOWN_ACK;
}

bool m2ua::Asp_down_ack::check_mandatory(){
	return true;
}

// Heartbeat
m2ua::Heartbeat::Heartbeat(){
	type = HEARTBEAT;
}

bool m2ua::Heartbeat::check_mandatory(){
	return true;
}

// Heartbeat_ack
m2ua::Heartbeat_ack::Heartbeat_ack(){
	type = HEARTBEAT_ACK;
}

bool m2ua::Heartbeat_ack::check_mandatory(){
	return true;
}

// Asp_active
m2ua::Asp_active::Asp_active(){
	type = ASP_ACTIVE;
}

bool m2ua::Asp_active::check_mandatory(){
	return true;
}

// Asp_active_ack
m2ua::Asp_active_ack::Asp_active_ack(){
	type = ASP_ACTIVE_ACK;
}

bool m2ua::Asp_active_ack::check_mandatory(){
	return true;
}

// Asp_inactive
m2ua::Asp_inactive::Asp_inactive(){
	type = ASP_INACTIVE;
}

bool m2ua::Asp_inactive::check_mandatory(){
	return true;
}

// Asp_inactive_ack
m2ua::Asp_inactive_ack::Asp_inactive_ack(){
	type = ASP_INACTIVE_ACK;
}

bool m2ua::Asp_inactive_ack::check_mandatory(){
	return true;
}

// Error
m2ua::Error::Error(){
	type = ERR;
}

bool m2ua::Error::check_mandatory(){
	return true;
}

// Notify
m2ua::Notify::Notify(){
	type = NTFY;
}

bool m2ua::Notify::check_mandatory(){
	return true;
}

// Registration_request
m2ua::Registration_request::Registration_request(){
	type = REG_REQ;
}

bool m2ua::Registration_request::check_mandatory(){
	return true;
}

// Registration_response
m2ua::Registration_response::Registration_response(){
	type = REG_RSP;
}

bool m2ua::Registration_response::check_mandatory(){
	return true;
}

// Deregistration_request
m2ua::Deregistration_request::Deregistration_request(){
	type = DEREG_REQ;
}

bool m2ua::Deregistration_request::check_mandatory(){
	return true;
}

// Deregistration_response
m2ua::Deregistration_response::Deregistration_response(){
	type = DEREG_RSP;
}

bool m2ua::Deregistration_response::check_mandatory(){
	return true;
}

// m2ua Packet
m2ua::M2UAPacket::M2UAPacket(){
	message = NULL;
	version = 1;
	message_class = __M2UA_MESSAGE_CLASS_UNKNOWN__;
	message_type = __M2UA_MESSAGE_TYPE_UNKNOWN__;
}

m2ua::M2UAPacket::~M2UAPacket(){
	if(message != NULL) delete message;
}

void m2ua::M2UAPacket::encode(char* buffer, int buffer_length, int offset, int* result_length){
	char* tmp_pos;
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
		// m2ua parameters, encode and return total length(data + padding)
		// offset of extra 4 bytes for length which is calculated later
		message->encode(buffer, buffer_length, 4, result_length);
		total_length += *result_length;

	}
	// restore position
	buffer = tmp_pos;
	buffer[0] = total_length >> 24;
	buffer[1] = total_length >> 16;
	buffer[2] = total_length >> 16;
	buffer[3] = total_length & 0xFF;
	// result length
	*result_length = total_length;


}


void m2ua::M2UAMessage::process_params(char* data, int data_length){
	if(data != NULL && data_length > 0){
		ParameterType pt;
		M2UAParam* m2ua_param = NULL;
		int tag;
		int l;
		int m;
		while(byte_pos < data_length){
			tag = (data[byte_pos] << 8) + (data[byte_pos + 1] & 0xFF);
			byte_pos += 2;
			// length(l) = two bytes for tag + two bytes for length + length of actual data
			l = (data[byte_pos] << 8) + (data[byte_pos + 1] & 0xFF);
			byte_pos += 2;
			pt = (ParameterType)(tag);
			// create param
			m2ua_param = get_param(pt);
			if(m2ua_param != NULL){
				m2ua_param->init(&data[byte_pos], l - 4);
				params.push_back(m2ua_param);
			}
			// parameter has to be a multiple of 4, if not, zero padding is added
			m = l % 4;
			byte_pos += l + (m > 0 ? 4 - m : 0) - 4;

		}
	}

}

int m2ua::M2UAMessage::encode_parameter(M2UAParam* param, char* buffer, int buffer_length, int offset){
	if(buffer != NULL && param != NULL && buffer_length > 0){
		int l = 0;
		int padding_count = 0;
		int m;
		char* res = NULL;
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
// ======= STATIC METHODS =============

// main m2ua param detect method
m2ua::M2UAParam* m2ua::get_param(ParameterType _param_type){
	M2UAParam* param = NULL;
	switch(_param_type){
		case INTERFACE_INDICATOR_INTEGER: param = new InterfaceIdentifier_int(); break;
		case INTERFACE_INDICATOR_TEXT: param = new InterfaceIdentifier_txt(); break;
		case INFO_STRING: param = new Info_string(); break;
		case DIAGNOSTIC_INFORMATION: param = new DiagnosticInformation(); break;
		case INTERFACE_INDICATOR_INTEGER_RANGE: param = new InterfaceIdentifier_int_range(); break;
		case HEARTBEAT_DATA: param = new Heartbeat_data(); break;
		case TRAFFIC_MODE_TYPE: param = new TrafficModeType(); break;
		case ERROR_CODE: param = new ErrorCode(); break;
		case STATUS_TYPE_INFO: param = new StatusTypeInfo(); break;
		case ASP_IDENTIFIER: param = new Asp_identifier(); break;
		case CORRELATION_ID: param = new Correlation_id(); break;
		case PROTOCOL_DATA_1: param = new ProtocolData(); break;
		case PROTOCOL_DATA_2_TTC: param = new ProtocolData_TTC(); break;
		case STATE_REQUEST_PARAM: param = new StateRequest(); break;
		case STATE_EVENT: param = new StateEvent(); break;
		case CONGESTION_STATUS: param = new CongestionStatus(); break;
		case DISCARD_STATUS: param = new DiscardStatus(); break;
		case ACTION: param = new Action(); break;
		case SEQUENCE_NUMBER: param = new SequenceNumber(); break;
		case RETRIEVAL_RESULT: param = new RetrievalResult(); break;
		case LINK_KEY: param = new LinkKey(); break;
		case LOCAL_LK_IDENTIFIER: param = new LocalLKId(); break;
		case SDT_IDENTIFIER: param = new SignallingDTId(); break;
		case SDL_IDENTIFIER: param = new SignallingDLId(); break;
		case REGISTRATION_RESULT: param = new RegistrationResult(); break;
		case REGISTRATION_STATUS: param = new RegistrationStatus(); break;
		case DEREGISTRATION_RESULT: param = new DeregistrationResult(); break;
		case DEREGISTRATION_STATUS: param = new DeregistrationStatus(); break;
	}

	return param;
}



// main m2ua decode method
m2ua::M2UAPacket* m2ua::decode(char* data, int data_length){
	if(data != NULL && data_length > 0){
		M2UAPacket* m2uap = new M2UAPacket();
		// min headaer length
		if(data_length >=8){
			m2uap->version = *data & 0xFF;
			// one byte version, one byte reserved
			data += 2;
			// only version 1.0 supported
			if(m2uap->version == 1){
				m2uap->message_class = (MessageClass)(*data & 0xFF);
				data++;
				m2uap->message_type = (MessageType)((m2uap->message_class << 8) + (*data & 0xFF));
				data++;
				m2uap->message_length = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + (data[3] & 0xFF);
				data += 4;

				if(m2uap->message_length > 0){
					//std::cout << std::dec << m2uap->message_type << std::endl;
					switch(m2uap->message_type){
						case DATA:  m2uap->message = new Data(); break;
						case ESTABLISH_REQUEST: m2uap->message = new Establish_request(); break;
						case ESTABLISH_CONFIRM: m2uap->message = new Establish_confirm(); break;
						case RELEASE_REQUEST: m2uap->message = new Release_request(); break;
						case RELEASE_CONFIRM: m2uap->message = new Release_confirm(); break;
						case RELEASE_INDICATION: m2uap->message = new Release_indication(); break;
						case STATE_REQUEST: m2uap->message = new State_request(); break;
						case STATE_CONFIRM: m2uap->message =  new State_confirm(); break;
						case STATE_INDICATION: m2uap->message = new State_indication(); break;
						case DATA_RETRIEVAL_REQUEST: m2uap->message = new Retrieval_request(); break;
						case DATA_RETRIEVAL_CONFIRM: m2uap->message = new Retrieval_confirm(); break;
						case DATA_RETRIEVAL_INDICATION: m2uap->message = new Retrieval_indication(); break;
						case DATA_RETRIEVAL_COMPLETE_INDICATION: m2uap->message = new Retrieval_complete_indication(); break;
						case CONGESTION_INDICATION: m2uap->message = new Congestion_indication(); break;
						case DATA_ACK: m2uap->message = new Data_ack(); break;
						case ASP_UP: m2uap->message = new Asp_up(); break;
						case ASP_DOWN: m2uap->message = new Asp_down(); break;
						case HEARTBEAT: m2uap->message = new Heartbeat(); break;
						case ASP_UP_ACK: m2uap->message = new Asp_up_ack(); break;
						case ASP_DOWN_ACK: m2uap->message = new Asp_down_ack(); break;
						case HEARTBEAT_ACK: m2uap->message = new Heartbeat_ack(); break;
						case ASP_ACTIVE: m2uap->message = new Asp_active(); break;
						case ASP_INACTIVE: m2uap->message = new Asp_inactive(); break;
						case ASP_ACTIVE_ACK: m2uap->message = new Asp_active_ack(); break;
						case ASP_INACTIVE_ACK: m2uap->message = new Asp_inactive_ack(); break;
						case ERR: m2uap->message = new Error(); break;
						case NTFY: m2uap->message = new Notify(); break;
						case REG_REQ: m2uap->message = new Registration_request(); break;
						case REG_RSP: m2uap->message = new Registration_response(); break;
						case DEREG_REQ: m2uap->message = new Deregistration_request(); break;
						case DEREG_RSP: m2uap->message = new Deregistration_response(); break;

					}
					if(m2uap->message != NULL) m2uap->message->init(data, data_length - 8);

				}
			}
		}
		return m2uap;

	}
	return NULL;
}

