#include<iostream>
#include<string.h>
#include<sccp.h>

// XUDTS_ExtendedUnitdataService
sccp::XUDTS_ExtendedUnitdataService::XUDTS_ExtendedUnitdataService(){
	type = XUDTS_EXTENDED_UNIDATA_SERVICE;
	called_party_address = NULL;
	calling_party_address = NULL;
	data = NULL;
	hop_counter = NULL;
	return_cause = NULL;
	pointer = new SCCPPointer[4];

}
sccp::XUDTS_ExtendedUnitdataService::~XUDTS_ExtendedUnitdataService(){
    /*
	if(called_party_address != NULL) delete called_party_address;
	if(calling_party_address != NULL) delete calling_party_address;
	if(data != NULL) delete data;
	if(hop_counter != NULL) delete hop_counter;
	if(return_cause != NULL) delete return_cause;
*/
}
void sccp::XUDTS_ExtendedUnitdataService::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//return_cause = new ReturnCause();
	return_cause = (ReturnCause*)_sccp_pool->request_param(RETURN_CAUSE);
	return_cause->byte_pos = 0;
	return_cause->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Fixed mandatory
	//hop_counter = new HopCounter();
	hop_counter = (HopCounter*)_sccp_pool->request_param(HOP_COUNTER);
	hop_counter->byte_pos = 0;
	hop_counter->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// pointers to mandatory variable parts
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;

	pointer[1].value = _data[byte_pos] & 0xFF;
	pointer[1].position = pointer[1].value + byte_pos;
	byte_pos++;

	pointer[2].value = _data[byte_pos] & 0xFF;
	pointer[2].position = pointer[2].value + byte_pos;
	byte_pos++;

	pointer[3].value = _data[byte_pos] & 0xFF;
	pointer[3].position = pointer[3].value + byte_pos;
	byte_pos++;

	// First mandatory variable part
	//called_party_address = new CalledPA();
	called_party_address = (CalledPA*)_sccp_pool->request_param(CALLED_PARTY_ADDRESS);
	called_party_address->byte_pos = 0;
	called_party_address->init(&_data[pointer[0].position + 1], _data[pointer[0].position] & 0xFF, _sccp_pool);

	// Second mandatory variable part
	//calling_party_address = new CallingPA();
	calling_party_address = (CallingPA*)_sccp_pool->request_param(CALLING_PARTY_ADDRESS);
	calling_party_address->byte_pos = 0;
	calling_party_address->init(&_data[pointer[1].position + 1], _data[pointer[1].position] & 0xFF, _sccp_pool);

	// Third mandatory variable part
	//data = new Data();
	data = (Data*)_sccp_pool->request_param(DATA);
	data->byte_pos = 0;
	data->init(&_data[pointer[2].position + 1], _data[pointer[2].position] & 0xFF, _sccp_pool);

	// optional
	byte_pos += ((_data[pointer[0].position] & 0xFF) + 2) +
				((_data[pointer[1].position] & 0xFF) + 2) +
				((_data[pointer[2].position] & 0xFF) + 2);

	process_optional_params(&_data[pointer[3].position], _data_length - byte_pos, _sccp_pool);

}


void sccp::XUDTS_ExtendedUnitdataService::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// return_cause
		return_cause->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// hop_counter
		hop_counter->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 4;
		tmp_l += 4;

		// called_party_address
		called_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[1].value = *result_length;

		// calling_party_address
		calling_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[2].value = *result_length + pointer[1].value;

		//data
		data->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[3].value = *result_length + pointer[2].value + pointer[1].value;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;
		*(tmp_pointers + 1) = pointer[1].value + 3;
		*(tmp_pointers + 2) = pointer[2].value + 2;
		*(tmp_pointers + 3) = (*result_length == 0 ? 0x00 : pointer[3].value + 1);


		// result
		*result_length = tmp_l;



	}
}

void sccp::XUDTS_ExtendedUnitdataService::init_new(SCCPPool* _sccp_pool){
	return_cause = new ReturnCause();
	hop_counter = new HopCounter();
	called_party_address = new CalledPA();
	calling_party_address = new CallingPA();
	data = new Data();
}

// XUDT_ExtendedUnitdata
sccp::XUDT_ExtendedUnitdata::XUDT_ExtendedUnitdata(){
	type = XUDT_EXTENDED_UNITDATA;
	called_party_address = NULL;
	calling_party_address = NULL;
	data = NULL;
	hop_counter = NULL;
	protocol_class = NULL;
	pointer = new SCCPPointer[4];

}
sccp::XUDT_ExtendedUnitdata::~XUDT_ExtendedUnitdata(){
    /*
	if(called_party_address != NULL) delete called_party_address;
	if(calling_party_address != NULL) delete calling_party_address;
	if(data != NULL) delete data;
	if(hop_counter != NULL) delete hop_counter;
	if(protocol_class != NULL) delete protocol_class;
*/
}
void sccp::XUDT_ExtendedUnitdata::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//protocol_class = new ProtocolClass();
	protocol_class = (ProtocolClass*)_sccp_pool->request_param(PROTOCOL_CLASS);
	protocol_class->byte_pos = 0;
	protocol_class->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Fixed mandatory
	//hop_counter = new HopCounter();
	hop_counter = (HopCounter*)_sccp_pool->request_param(HOP_COUNTER);
	hop_counter->byte_pos = 0;
	hop_counter->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;

	// pointers to mandatory variable parts
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;

	pointer[1].value = _data[byte_pos] & 0xFF;
	pointer[1].position = pointer[1].value + byte_pos;
	byte_pos++;

	pointer[2].value = _data[byte_pos] & 0xFF;
	pointer[2].position = pointer[2].value + byte_pos;
	byte_pos++;

	pointer[3].value = _data[byte_pos] & 0xFF;
	pointer[3].position = pointer[3].value + byte_pos;
	byte_pos++;

	// First mandatory variable part
	//called_party_address = new CalledPA();
	called_party_address = (CalledPA*)_sccp_pool->request_param(CALLED_PARTY_ADDRESS);
	called_party_address->byte_pos = 0;
	called_party_address->init(&_data[pointer[0].position + 1], _data[pointer[0].position] & 0xFF, _sccp_pool);

	// Second mandatory variable part
	//calling_party_address = new CallingPA();
	calling_party_address = (CallingPA*)_sccp_pool->request_param(CALLING_PARTY_ADDRESS);
	calling_party_address->byte_pos = 0;
	calling_party_address->init(&_data[pointer[1].position + 1], _data[pointer[1].position] & 0xFF, _sccp_pool);

	// Third mandatory variable part
	//data = new Data();
	data = (Data*)_sccp_pool->request_param(DATA);
	data->byte_pos = 0;
	data->init(&_data[pointer[2].position + 1], _data[pointer[2].position] & 0xFF, _sccp_pool);

	// optional
	byte_pos += ((_data[pointer[0].position] & 0xFF) + 2) +
				((_data[pointer[1].position] & 0xFF) + 2) +
				((_data[pointer[2].position] & 0xFF) + 2);

	process_optional_params(&_data[pointer[3].position], _data_length - byte_pos, _sccp_pool);

}


void sccp::XUDT_ExtendedUnitdata::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// protocol_class
		protocol_class->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// hop_counter
		hop_counter->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;


		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 4;
		tmp_l += 4;

		// called_party_address
		called_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[1].value = *result_length;

		// calling_party_address
		calling_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[2].value = *result_length + pointer[1].value;

		//data
		data->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[3].value = *result_length + pointer[2].value + pointer[1].value;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;
		*(tmp_pointers + 1) = pointer[1].value + 3;
		*(tmp_pointers + 2) = pointer[2].value + 2;
		*(tmp_pointers + 3) = (*result_length == 0 ? 0x00 : pointer[3].value + 1);


		// result
		*result_length = tmp_l;



	}
}

void sccp::XUDT_ExtendedUnitdata::init_new(SCCPPool* _sccp_pool){
	protocol_class = new ProtocolClass();
	hop_counter = new HopCounter();
	called_party_address = new CalledPA();
	calling_party_address = new CallingPA();
	data = new Data();
}


// UDTS_UnitDataService
sccp::UDTS_UnitDataService::UDTS_UnitDataService(){
	type = UDTS_UNITDATA_SERVICE;
	return_cause = NULL;
	called_party_address = NULL;
	calling_party_address = NULL;
	data = NULL;
	pointer = new SCCPPointer[3];
}
sccp::UDTS_UnitDataService::~UDTS_UnitDataService(){
    /*
	if(return_cause != NULL) delete return_cause;
	if(called_party_address != NULL) delete called_party_address;
	if(calling_party_address != NULL) delete calling_party_address;
	if(data != NULL) delete data;
*/
}
void sccp::UDTS_UnitDataService::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//return_cause = new ReturnCause();
	return_cause = (ReturnCause*)_sccp_pool->request_param(RETURN_CAUSE);
	return_cause->byte_pos = 0;
	return_cause->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// pointers to mandatory variable parts
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;

	pointer[1].value = _data[byte_pos] & 0xFF;
	pointer[1].position = pointer[1].value + byte_pos;
	byte_pos++;

	pointer[2].value = _data[byte_pos] & 0xFF;
	pointer[2].position = pointer[2].value + byte_pos;
	byte_pos++;

	// First mandatory variable part
	//called_party_address = new CalledPA();
	called_party_address = (CalledPA*)_sccp_pool->request_param(CALLED_PARTY_ADDRESS);
	called_party_address->byte_pos = 0;
	called_party_address->init(&_data[pointer[0].position + 1], _data[pointer[0].position] & 0xFF, _sccp_pool);

	// Second mandatory variable part
	//calling_party_address = new CallingPA();
	calling_party_address = (CallingPA*)_sccp_pool->request_param(CALLING_PARTY_ADDRESS);
	calling_party_address->byte_pos = 0;
	calling_party_address->init(&_data[pointer[1].position + 1], _data[pointer[1].position] & 0xFF, _sccp_pool);

	// Third mandatory variable part
	//data = new Data();
	data = (Data*)_sccp_pool->request_param(DATA);
	data->byte_pos = 0;
	data->init(&_data[pointer[2].position + 1], _data[pointer[2].position] & 0xFF, _sccp_pool);


}


void sccp::UDTS_UnitDataService::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// return_cause
		return_cause->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 3;
		tmp_l += 3;

		// called_party_address
		called_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[1].value = *result_length;

		// calling_party_address
		calling_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[2].value = *result_length + pointer[1].value;

		//data
		data->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;
		*(tmp_pointers + 1) = pointer[1].value + 2;
		*(tmp_pointers + 2) = pointer[2].value + 1;


		// result
		*result_length = tmp_l;



	}
}

void sccp::UDTS_UnitDataService::init_new(SCCPPool* _sccp_pool){
	return_cause = new ReturnCause();
	called_party_address = new CalledPA();
	calling_party_address = new CallingPA();
	data = new Data();
}


// RSR_ResetRequest
sccp::RSR_ResetRequest::RSR_ResetRequest(){
	type = RSR_RESET_REQUEST;
	destination_local_reference = NULL;
	source_local_reference = NULL;
	reset_cause = NULL;
	pointer = new SCCPPointer[1];
}
sccp::RSR_ResetRequest::~RSR_ResetRequest(){
	//if(destination_local_reference != NULL) delete destination_local_reference;
	//if(source_local_reference != NULL) delete source_local_reference;
	//if(reset_cause != NULL) delete reset_cause;
}

void sccp::RSR_ResetRequest::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//source_local_reference = new Slr();
	source_local_reference = (Slr*)_sccp_pool->request_param(SOURCE_LOCAL_REFERENCE);
	source_local_reference->byte_pos = 0;
	source_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//reset_cause = new ResetCause();
	reset_cause = (ResetCause*)_sccp_pool->request_param(RESET_CAUSE);
	reset_cause->byte_pos = 0;
	reset_cause->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Pointers reserverd for future use
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;


}

void sccp::RSR_ResetRequest::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// source_local_reference
		source_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// reset_cause
		reset_cause->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 1;
		tmp_l++;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		// if no optional params present, pointer value is 0
		*tmp_pointers = (*result_length == 0 ? 0x00 : pointer[0].value);

		// result
		*result_length = tmp_l;



	}
}
void sccp::RSR_ResetRequest::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	source_local_reference = new Slr();
	reset_cause = new ResetCause();
}


// RSC_ResetConfirmation
sccp::RSC_ResetConfirmation::RSC_ResetConfirmation(){
	type = RSC_RESET_CONFIRM;
	destination_local_reference = NULL;
	source_local_reference = NULL;

}
sccp::RSC_ResetConfirmation::~RSC_ResetConfirmation(){
	//if(destination_local_reference != NULL) delete destination_local_reference;
	//if(source_local_reference != NULL) delete source_local_reference;
}

void sccp::RSC_ResetConfirmation::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//source_local_reference = new Slr();
	source_local_reference = (Slr*)_sccp_pool->request_param(SOURCE_LOCAL_REFERENCE);
	source_local_reference->byte_pos = 0;
	source_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;


}

void sccp::RSC_ResetConfirmation::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// source_local_reference
		source_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// result
		*result_length = tmp_l;

	}
}
void sccp::RSC_ResetConfirmation::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	source_local_reference = new Slr();
}


// RLSD_Released
sccp::RLSD_Released::RLSD_Released(){
	type = RLSD_RELEASED;
	destination_local_reference = NULL;
	source_local_reference = NULL;
	release_cause = NULL;
	pointer = new SCCPPointer[1];

}
sccp::RLSD_Released::~RLSD_Released(){
	//if(destination_local_reference != NULL) delete destination_local_reference;
	//if(source_local_reference != NULL) delete source_local_reference;
	//if(release_cause != NULL) delete release_cause;

}
void sccp::RLSD_Released::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//source_local_reference = new Slr();
	source_local_reference = (Slr*)_sccp_pool->request_param(SOURCE_LOCAL_REFERENCE);
	source_local_reference->byte_pos = 0;
	source_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//release_cause = new ReleaseCause();
	release_cause = (ReleaseCause*)_sccp_pool->request_param(RELEASE_CAUSE);
	release_cause->byte_pos = 0;
	release_cause->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;

	// Pointers
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;
	// optional
	process_optional_params(&_data[pointer[0].position], _data_length - byte_pos, _sccp_pool);

}

void sccp::RLSD_Released::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// source_local_reference
		source_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// release_cause
		release_cause->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 1;
		tmp_l++;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		// if no optional params present, pointer value is 0
		*tmp_pointers = (*result_length == 0 ? 0x00 : pointer[0].value);

		// result
		*result_length = tmp_l;



	}
}
void sccp::RLSD_Released::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	source_local_reference = new Slr();
	release_cause = new ReleaseCause();
}



// RLC_ReleaseComplete
sccp::RLC_ReleaseComplete::RLC_ReleaseComplete(){
	type = RLC_RELEASE_COMPLETE;
	destination_local_reference = NULL;
	source_local_reference = NULL;
}
sccp::RLC_ReleaseComplete::~RLC_ReleaseComplete(){
	//if(destination_local_reference != NULL) delete destination_local_reference;
	//if(source_local_reference != NULL) delete source_local_reference;
}

void sccp::RLC_ReleaseComplete::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//source_local_reference = new Slr();
	source_local_reference = (Slr*)_sccp_pool->request_param(SOURCE_LOCAL_REFERENCE);
	source_local_reference->byte_pos = 0;
	source_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;

}

void sccp::RLC_ReleaseComplete::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// source_local_reference
		source_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;


		// result
		*result_length = tmp_l;



	}
}
void sccp::RLC_ReleaseComplete::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	source_local_reference = new Slr();
}



// LUDTS_LongUnitdataService
sccp::LUDTS_LongUnitdataService::LUDTS_LongUnitdataService(){
	type = LUDTS_LONG_UNIDATA_SERVICE;
	called_party_address = NULL;
	calling_party_address = NULL;
	hop_counter = NULL;
	importance = NULL;;
	long_data = NULL;
	return_cause = NULL;
	segmentation = NULL;
	pointer = new SCCPPointer[4];
}

sccp::LUDTS_LongUnitdataService::~LUDTS_LongUnitdataService(){
    /*
	if(called_party_address != NULL) delete called_party_address;
	if(calling_party_address != NULL) delete calling_party_address;
	if(hop_counter != NULL) delete hop_counter;
	if(importance != NULL) delete importance;
	if(long_data != NULL) delete long_data;
	if(return_cause != NULL) delete return_cause;
	if(segmentation != NULL) delete segmentation;
	*/
}

void sccp::LUDTS_LongUnitdataService::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//return_cause = new ReturnCause();
	return_cause = (ReturnCause*)_sccp_pool->request_param(RETURN_CAUSE);
	return_cause->byte_pos = 0;
	return_cause->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Fixed mandatory
	//hop_counter = new HopCounter();
	hop_counter = (HopCounter*)_sccp_pool->request_param(HOP_COUNTER);
	hop_counter->byte_pos = 0;
	hop_counter->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// two byte pointers to mandatory variable parts
	pointer[0].value =  (_data[byte_pos] << 8) + (_data[byte_pos + 1] & 0xff);
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos += 2;

	pointer[1].value =  (_data[byte_pos] << 8) + (_data[byte_pos + 1] & 0xff);
	pointer[1].position = pointer[1].value + byte_pos;
	byte_pos += 2;

	pointer[2].value =  (_data[byte_pos] << 8) + (_data[byte_pos + 1] & 0xff);
	pointer[2].position = pointer[2].value + byte_pos;
	byte_pos += 2;

	pointer[3].value =  (_data[byte_pos] << 8) + (_data[byte_pos + 1] & 0xff);
	pointer[3].position = pointer[3].value + byte_pos;
	byte_pos += 2;

	// First mandatory variable part
	//called_party_address = new CalledPA();
	called_party_address = (CalledPA*)_sccp_pool->request_param(CALLED_PARTY_ADDRESS);
	called_party_address->byte_pos = 0;
	called_party_address->init(&_data[pointer[0].position + 2], _data[pointer[0].position] & 0xFF, _sccp_pool);

	// Second mandatory variable part
	//calling_party_address = new CallingPA();
	calling_party_address = (CallingPA*)_sccp_pool->request_param(CALLING_PARTY_ADDRESS);
	calling_party_address->byte_pos = 0;
	calling_party_address->init(&_data[pointer[1].position + 2], _data[pointer[1].position] & 0xFF, _sccp_pool);

	// Third mandatory variable part
	//long_data = new LongData();
	long_data = (LongData*)_sccp_pool->request_param(LONG_DATA);
	long_data->byte_pos = 0;
	long_data->init(&_data[pointer[2].position + 2], _data[pointer[2].position] & 0xFF, _sccp_pool);

	// optional
	byte_pos += ((_data[pointer[0].position] & 0xFF) + 2) +
				((_data[pointer[1].position] & 0xFF) + 2) +
				((_data[pointer[2].position] & 0xFF) + 2);

	process_optional_params(&_data[pointer[3].position], _data_length - byte_pos, _sccp_pool);


}


void sccp::LUDTS_LongUnitdataService::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// return_cause
		return_cause->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// hop_counter
		hop_counter->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// two byte pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 8;
		tmp_l += 8;

		// called_party_address
		called_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[1].value = *result_length;

		// calling_party_address
		calling_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[2].value = *result_length + pointer[1].value;

		//longdata
		long_data->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[3].value = *result_length + pointer[2].value + pointer[1].value;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;
		*(tmp_pointers + 1) = pointer[1].value + 3*2;
		*(tmp_pointers + 2) = pointer[2].value + 2*2;
		*(tmp_pointers + 3) = (*result_length == 0 ? 0x00 : pointer[3].value + 1*2);


		// result
		*result_length = tmp_l;


	}

}
void sccp::LUDTS_LongUnitdataService::init_new(SCCPPool* _sccp_pool){
	return_cause = new ReturnCause();
	hop_counter = new HopCounter();
	called_party_address = new CalledPA();
	calling_party_address = new CallingPA();
	long_data = new LongData();
}


// LUDT_LongUnitdata
sccp::LUDT_LongUnitdata::LUDT_LongUnitdata(){
	type = LUDT_LONG_UNIDATA;
	called_party_address = NULL;
	calling_party_address = NULL;
	hop_counter = NULL;
	long_data = NULL;
	pointer = new SCCPPointer[4];
}

sccp::LUDT_LongUnitdata::~LUDT_LongUnitdata(){
    /*
	if(called_party_address != NULL) delete called_party_address;
	if(calling_party_address != NULL) delete calling_party_address;
	if(hop_counter != NULL) delete hop_counter;
	if(long_data != NULL) delete long_data;
*/
}
void sccp::LUDT_LongUnitdata::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//protocol_class = new ProtocolClass();
	protocol_class = (ProtocolClass*)_sccp_pool->request_param(PROTOCOL_CLASS);
	protocol_class->byte_pos = 0;
	protocol_class->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Fixed mandatory
	//hop_counter = new HopCounter();
	hop_counter = (HopCounter*)_sccp_pool->request_param(HOP_COUNTER);
	hop_counter->byte_pos = 0;
	hop_counter->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;

	// two byte pointers to mandatory variable parts
	pointer[0].value =  (_data[byte_pos] << 8) + (_data[byte_pos + 1] & 0xff);
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos += 2;

	pointer[1].value =  (_data[byte_pos] << 8) + (_data[byte_pos + 1] & 0xff);
	pointer[1].position = pointer[1].value + byte_pos;
	byte_pos += 2;

	pointer[2].value =  (_data[byte_pos] << 8) + (_data[byte_pos + 1] & 0xff);
	pointer[2].position = pointer[2].value + byte_pos;
	byte_pos += 2;

	pointer[3].value =  (_data[byte_pos] << 8) + (_data[byte_pos + 1] & 0xff);
	pointer[3].position = pointer[3].value + byte_pos;
	byte_pos += 2;

	// First mandatory variable part
	//called_party_address = new CalledPA();
	called_party_address = (CalledPA*)_sccp_pool->request_param(CALLED_PARTY_ADDRESS);
	called_party_address->byte_pos = 0;
	called_party_address->init(&_data[pointer[0].position + 2], _data[pointer[0].position] & 0xFF, _sccp_pool);

	// Second mandatory variable part
	//calling_party_address = new CallingPA();
	calling_party_address = (CallingPA*)_sccp_pool->request_param(CALLING_PARTY_ADDRESS);
	calling_party_address->byte_pos = 0;
	calling_party_address->init(&_data[pointer[1].position + 2], _data[pointer[1].position] & 0xFF, _sccp_pool);

	// Third mandatory variable part
	//long_data = new LongData();
	long_data = (LongData*)_sccp_pool->request_param(LONG_DATA);
	long_data->byte_pos = 0;
	long_data->init(&_data[pointer[2].position + 2], _data[pointer[2].position] & 0xFF, _sccp_pool);

	// optional
	byte_pos += ((_data[pointer[0].position] & 0xFF) + 2) +
				((_data[pointer[1].position] & 0xFF) + 2) +
				((_data[pointer[2].position] & 0xFF) + 2);

	process_optional_params(&_data[pointer[3].position], _data_length - byte_pos, _sccp_pool);


}


void sccp::LUDT_LongUnitdata::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// protocol_class
		protocol_class->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// hop_counter
		hop_counter->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// two byte pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 8;
		tmp_l += 8;

		// called_party_address
		called_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[1].value = *result_length;

		// calling_party_address
		calling_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[2].value = *result_length + pointer[1].value;

		//longdata
		long_data->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[3].value = *result_length + pointer[2].value + pointer[1].value;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;
		*(tmp_pointers + 1) = pointer[1].value + 3*2;
		*(tmp_pointers + 2) = pointer[2].value + 2*2;
		*(tmp_pointers + 3) = (*result_length == 0 ? 0x00 : pointer[3].value + 1*2);


		// result
		*result_length = tmp_l;

	}
}

void sccp::LUDT_LongUnitdata::init_new(SCCPPool* _sccp_pool){
	protocol_class = new ProtocolClass();
	hop_counter = new HopCounter();
	called_party_address = new CalledPA();
	calling_party_address = new CallingPA();
	long_data = new LongData();
}


// IT_InactivityTest
sccp::IT_InactivityTest::IT_InactivityTest(){
	type = IT_INACTIVITY_TEST;
	destination_local_reference = NULL;
	source_local_reference = NULL;
	protocol_class = NULL;
	sequencing_segmenting = NULL;
	credit = NULL;
}
sccp::IT_InactivityTest::~IT_InactivityTest(){
    /*
	if(destination_local_reference != NULL) delete destination_local_reference;
	if(source_local_reference != NULL) delete source_local_reference;
	if(protocol_class != NULL) delete protocol_class;
	if(sequencing_segmenting != NULL) delete sequencing_segmenting;
	if(credit != NULL) delete credit;
	*/
}
void sccp::IT_InactivityTest::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//source_local_reference = new Slr();
	source_local_reference = (Slr*)_sccp_pool->request_param(SOURCE_LOCAL_REFERENCE);
	source_local_reference->byte_pos = 0;
	source_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//protocol_class = new ProtocolClass();
	protocol_class = (ProtocolClass*)_sccp_pool->request_param(PROTOCOL_CLASS);
	protocol_class->byte_pos = 0;
	protocol_class->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Fixed mandatory
	//sequencing_segmenting = new SeqSegm();
	sequencing_segmenting = (SeqSegm*)_sccp_pool->request_param(SEQUENCING_SEGMENTING);
	sequencing_segmenting->byte_pos = 0;
	sequencing_segmenting->init(&_data[byte_pos], 2, _sccp_pool);
	byte_pos += 2;
	// Fixed mandatory
	//credit = new Credit();
	credit = (Credit*)_sccp_pool->request_param(CREDIT);
	credit->byte_pos = 0;
	credit->init(&_data[byte_pos], 1, _sccp_pool);

}

void sccp::IT_InactivityTest::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// source_local_reference
		source_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// protocol_class
		protocol_class->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// sequencing_segmenting
		sequencing_segmenting->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// credit
		credit->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// result
		*result_length = tmp_l;

	}
}
void sccp::IT_InactivityTest::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	source_local_reference = new Slr();
	protocol_class = new ProtocolClass();
	sequencing_segmenting = new SeqSegm();
	credit = new Credit();
}


// ERR_PduError
sccp::ERR_PduError::ERR_PduError(){
	type = ERR_PROTOCOL_DATA_UNIT_ERROR;
	destination_local_reference = NULL;
	error_cause = NULL;
	pointer = new SCCPPointer[1];
}
sccp::ERR_PduError::~ERR_PduError(){
    /*
	if(destination_local_reference != NULL) delete destination_local_reference;
	if(error_cause != NULL) delete error_cause;
	*/
}
void sccp::ERR_PduError::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//error_cause = new ErrorCause();
	error_cause = (ErrorCause*)_sccp_pool->request_param(ERROR_CAUSE);
	error_cause->byte_pos = 0;
	error_cause->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Pointers
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;

}

void sccp::ERR_PduError::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 1;
		tmp_l++;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		// if no optional params present, pointer value is 0
		*tmp_pointers = (*result_length == 0 ? 0x00 : pointer[0].value);

		// result
		*result_length = tmp_l;



	}
}
void sccp::ERR_PduError::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	error_cause = new ErrorCause();
}


// ED_ExpeditedData
sccp::ED_ExpeditedData::ED_ExpeditedData(){
	type = ED_EXPEDITED_DATA;
	destination_local_reference = NULL;
	data = NULL;
	pointer = new SCCPPointer[1];
}
sccp::ED_ExpeditedData::~ED_ExpeditedData(){
    /*
	if(destination_local_reference != NULL) delete destination_local_reference;
	if(data != NULL) delete data;
	*/
}

void sccp::ED_ExpeditedData::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Pointers
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;
	// Second mandatory variable part
	//data = new Data();
	data = (Data*)_sccp_pool->request_param(DATA);
	data->byte_pos = 0;
	data->init(&_data[pointer[0].position + 1], _data[pointer[0].position] & 0xFF, _sccp_pool);


}

void sccp::ED_ExpeditedData::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 1;
		tmp_l++;

		//data
		data->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;


		// result
		*result_length = tmp_l;



	}
}
void sccp::ED_ExpeditedData::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	data = new Data();
}


// EA_ExpeditedDataAck
sccp::EA_ExpeditedDataAck::EA_ExpeditedDataAck(){
	type = EA_EXPEDITED_DATA_ACKNOWLEDGMENT;
	destination_local_reference = NULL;
}

sccp::EA_ExpeditedDataAck::~EA_ExpeditedDataAck(){
	//if(destination_local_reference != NULL) delete destination_local_reference;
}
void sccp::EA_ExpeditedDataAck::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;

}

void sccp::EA_ExpeditedDataAck::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// result
		*result_length = tmp_l;



	}
}
void sccp::EA_ExpeditedDataAck::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
}


// DT2_DataForm2
sccp::DT2_DataForm2::DT2_DataForm2(){
	type = DT2_DATA_FORM_2;
	destination_local_reference = NULL;
	segmenting_reassembling = NULL;
	data = NULL;
	pointer = new SCCPPointer[1];
}
sccp::DT2_DataForm2::~DT2_DataForm2(){
    /*
	if(destination_local_reference != NULL) delete destination_local_reference;
	if(segmenting_reassembling != NULL) delete segmenting_reassembling;
	if(data != NULL) delete data;
*/
}
void sccp::DT2_DataForm2::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//segmenting_reassembling = new SegmReassm();
	segmenting_reassembling = (SegmReassm*)_sccp_pool->request_param(SEGMENTING_REASSEMBLING);
	segmenting_reassembling->byte_pos = 0;
	segmenting_reassembling->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Pointers
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;
	// First mandatory variable part
	//data = new Data();
	data = (Data*)_sccp_pool->request_param(DATA);
	data->byte_pos = 0;
	data->init(&_data[pointer[0].position + 1], _data[pointer[0].position] & 0xFF, _sccp_pool);


}

void sccp::DT2_DataForm2::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// segmenting_reassembling
		segmenting_reassembling->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 1;
		tmp_l++;

		//data
		data->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;


		// result
		*result_length = tmp_l;



	}
}

void sccp::DT2_DataForm2::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	segmenting_reassembling = new SegmReassm();
	data = new Data();
}


// DT1_DataForm1
sccp::DT1_DataForm1::DT1_DataForm1(){
	type = DT1_DATA_FORM_1;
	destination_local_reference = NULL;
	segmenting_reassembling = NULL;
	data = NULL;
	pointer = new SCCPPointer[1];
}
sccp::DT1_DataForm1::~DT1_DataForm1(){
    /*
	if(destination_local_reference != NULL) delete destination_local_reference;
	if(segmenting_reassembling != NULL) delete segmenting_reassembling;
	if(data != NULL) delete data;
*/
}
void sccp::DT1_DataForm1::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//segmenting_reassembling = new SegmReassm();
	segmenting_reassembling = (SegmReassm*)_sccp_pool->request_param(SEGMENTING_REASSEMBLING);
	segmenting_reassembling->byte_pos = 0;
	segmenting_reassembling->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Pointers
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;
	// First mandatory variable part
	//data = new Data();
	data = (Data*)_sccp_pool->request_param(DATA);
	data->byte_pos = 0;
	data->init(&_data[pointer[0].position + 1], _data[pointer[0].position] & 0xFF, _sccp_pool);


}

void sccp::DT1_DataForm1::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// segmenting_reassembling
		segmenting_reassembling->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 1;
		tmp_l++;

		//data
		data->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;


		// result
		*result_length = tmp_l;



	}
}

void sccp::DT1_DataForm1::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	segmenting_reassembling = new SegmReassm();
	data = new Data();
}

// CREF_ConnectionRefused
sccp::CREF_ConnectionRefused::CREF_ConnectionRefused(){
	type = CREF_CONNECTION_REFUSED;
	destination_local_reference = NULL;
	refusal_cause = NULL;
	pointer = new SCCPPointer[1];
}
sccp::CREF_ConnectionRefused::~CREF_ConnectionRefused(){
    /*
	if(destination_local_reference != NULL) delete destination_local_reference;
	if(refusal_cause != NULL) delete refusal_cause;
	*/
}
void sccp::CREF_ConnectionRefused::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//refusal_cause = new RefusalCause();
	refusal_cause = (RefusalCause*)_sccp_pool->request_param(REFUSAL_CAUSE);
	refusal_cause->byte_pos = 0;
	refusal_cause->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Pointers
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;
	// optional
	process_optional_params(&_data[pointer[0].position], _data_length - byte_pos, _sccp_pool);



}

void sccp::CREF_ConnectionRefused::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// refusal_cause
		refusal_cause->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 1;
		tmp_l++;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		// if no optional params present, pointer value is 0
		*tmp_pointers = (*result_length == 0 ? 0x00 : pointer[0].value);

		// result
		*result_length = tmp_l;



	}
}

void sccp::CREF_ConnectionRefused::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	refusal_cause = new RefusalCause();
}
// CR_ConnectionRequest

sccp::CR_ConnectionRequest::CR_ConnectionRequest(){
	type = CR_CONNECTION_REQUEST;
	source_local_reference = NULL;
	protocol_class = NULL;
	called_party_address = NULL;
	pointer = new SCCPPointer[2];

}
sccp::CR_ConnectionRequest::~CR_ConnectionRequest(){
    /*
	if(source_local_reference != NULL) delete source_local_reference;
	if(protocol_class != NULL) delete protocol_class;
	if(called_party_address != NULL) delete called_party_address;
*/
}
void sccp::CR_ConnectionRequest::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//source_local_reference = new Slr();
	source_local_reference = (Slr*)_sccp_pool->request_param(SOURCE_LOCAL_REFERENCE);
	source_local_reference->byte_pos = 0;
	source_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//protocol_class = new ProtocolClass();
	protocol_class = (ProtocolClass*)_sccp_pool->request_param(PROTOCOL_CLASS);
	protocol_class->byte_pos = 0;
	protocol_class->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// Pointers
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;
	pointer[1].value = _data[byte_pos] & 0xFF;
	pointer[1].position = pointer[1].value + byte_pos;
	byte_pos++;

	// Variable mandatory
	//called_party_address = new CalledPA();
	called_party_address = (CalledPA*)_sccp_pool->request_param(CALLED_PARTY_ADDRESS);
	called_party_address->byte_pos = 0;
	called_party_address->init(&_data[pointer[0].position + 1], _data[pointer[0].position] & 0xFF, _sccp_pool);
	byte_pos += (_data[pointer[0].position] & 0xFF) + 1;

	// optional
	process_optional_params(&_data[pointer[1].position], _data_length - byte_pos, _sccp_pool);

}

void sccp::CR_ConnectionRequest::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// source_local_reference
		source_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// protocol_class
		protocol_class->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 2;
		tmp_l += 2;

		// called_party_address
		called_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[1].value = *result_length;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;
		*(tmp_pointers + 1) = (*result_length == 0 ? 0x00 :pointer[1].value + 1);

		// result
		*result_length = tmp_l;



	}
}

void sccp::CR_ConnectionRequest::init_new(SCCPPool* _sccp_pool){
	source_local_reference = new Slr();
	protocol_class = new ProtocolClass();
	called_party_address = new CalledPA();
}

// CC_ConnectionConfirm
sccp::CC_ConnectionConfirm::CC_ConnectionConfirm(){
	type = CC_CONNECTION_CONFIRM;
	destination_local_reference = NULL;
	source_local_reference = NULL;
	protocol_class = NULL;
	called_party_address = NULL;
	pointer = new SCCPPointer[1];

}
sccp::CC_ConnectionConfirm::~CC_ConnectionConfirm(){
    /*
	if(destination_local_reference != NULL) delete destination_local_reference;
	if(source_local_reference != NULL) delete source_local_reference;
	if(protocol_class != NULL) delete protocol_class;
	if(called_party_address != NULL) delete called_party_address;
*/
}
void sccp::CC_ConnectionConfirm::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//source_local_reference = new Slr();
	source_local_reference = (Slr*)_sccp_pool->request_param(SOURCE_LOCAL_REFERENCE);
	source_local_reference->byte_pos = 0;
	source_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;
	// Fixed mandatory
	//protocol_class = new ProtocolClass();
	protocol_class = (ProtocolClass*)_sccp_pool->request_param(PROTOCOL_CLASS);
	protocol_class->byte_pos = 0;
	protocol_class->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
	// One pointer for optional part
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;
	// optional
	process_optional_params(&_data[pointer[0].position], _data_length - byte_pos, _sccp_pool);

}


void sccp::CC_ConnectionConfirm::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// source_local_reference
		source_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// protocol_class
		protocol_class->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 1;
		tmp_l++;

		// optional
		encode_optional_params(buffer, buffer_length, tmp_l, result_length);
		tmp_l += *result_length;

		// set pointers
		// if no optional params present, pointer value is 0
		*tmp_pointers = (*result_length == 0 ? 0x00 : pointer[0].value);

		// result
		*result_length = tmp_l;



	}
}

void sccp::CC_ConnectionConfirm::init_new(SCCPPool* _sccp_pool){
	destination_local_reference = new Dlr();
	source_local_reference = new Slr();
	protocol_class = new ProtocolClass();
}



// AK_DataAcknowledgement
sccp::AK_DataAcknowledgement::AK_DataAcknowledgement(){
	type = AK_DATA_ACKNOWLEDGEMENT;
	credit = NULL;
	receive_sequence_number = NULL;
	destination_local_reference = NULL;
}
sccp::AK_DataAcknowledgement::~AK_DataAcknowledgement(){
    /*
	if(credit != NULL) delete credit;
	if(receive_sequence_number != NULL) delete receive_sequence_number;
	if(destination_local_reference != NULL) delete destination_local_reference;
*/
}
void sccp::AK_DataAcknowledgement::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	//destination_local_reference = new Dlr();
	destination_local_reference = (Dlr*)_sccp_pool->request_param(DESTINATION_LOCAL_REFERENCE);
	destination_local_reference->byte_pos = 0;
	destination_local_reference->init(&_data[byte_pos], 3, _sccp_pool);
	byte_pos += 3;

	// Fixed mandatory
	//receive_sequence_number = new ReceiveSeqNum();
	receive_sequence_number = (ReceiveSeqNum*)_sccp_pool->request_param(RECEIVE_SEQUENCE_NUMBER);
	receive_sequence_number->byte_pos = 0;
	receive_sequence_number->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;

	// Fixed mandatory
	//credit = new Credit();
	credit = (Credit*)_sccp_pool->request_param(RECEIVE_SEQUENCE_NUMBER);
	credit->byte_pos = 0;
	credit->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;
}

void sccp::AK_DataAcknowledgement::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// destination_local_reference
		destination_local_reference->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// receive_sequence_number
		receive_sequence_number->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// credit
		credit->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// result
		*result_length = tmp_l;



	}
}

void sccp::AK_DataAcknowledgement::init_new(SCCPPool* _sccp_pool){
	credit = new Credit();
	receive_sequence_number = new ReceiveSeqNum();
	destination_local_reference = new Dlr();
}

// UDT UnitData
sccp::UDT_UnitData::UDT_UnitData(){
	type = UDT_UNITDATA;
	protocol_class = NULL;
	called_party_address = NULL;
	calling_party_address = NULL;
	data = NULL;
	pointer = new SCCPPointer[3];
}
sccp::UDT_UnitData::~UDT_UnitData(){
	//if(protocol_class != NULL) delete protocol_class;
	//if(called_party_address != NULL) delete called_party_address;
	//if(calling_party_address != NULL) delete calling_party_address;
	//if(data != NULL) delete data;
}

void sccp::UDT_UnitData::init_new(SCCPPool* _sccp_pool){
	protocol_class = new ProtocolClass();
	called_party_address = new CalledPA();
	calling_party_address = new CallingPA();
	data = new Data();

}
void sccp::UDT_UnitData::init(unsigned char* _data, int _data_length, SCCPPool* _sccp_pool){
	byte_pos++;
	// Fixed mandatory
	protocol_class = (ProtocolClass*)_sccp_pool->request_param(PROTOCOL_CLASS);
	protocol_class->byte_pos = 0;
	protocol_class->init(&_data[byte_pos], 1, _sccp_pool);
	byte_pos++;

	// pointers to mandatory variable parts
	pointer[0].value = _data[byte_pos] & 0xFF;
	pointer[0].position = pointer[0].value + byte_pos;
	byte_pos++;

	pointer[1].value = _data[byte_pos] & 0xFF;
	pointer[1].position = pointer[1].value + byte_pos;
	byte_pos++;

	pointer[2].value = _data[byte_pos] & 0xFF;
	pointer[2].position = pointer[2].value + byte_pos;
	byte_pos++;

	// First mandatory variable part
	called_party_address = (CalledPA*)_sccp_pool->request_param(CALLED_PARTY_ADDRESS);
	called_party_address->byte_pos = 0;
	called_party_address->init(&_data[pointer[0].position + 1], _data[pointer[0].position] & 0xFF, _sccp_pool);

	// Second mandatory variable part
	calling_party_address = (CallingPA*)_sccp_pool->request_param(CALLING_PARTY_ADDRESS);
	calling_party_address->byte_pos = 0;
	calling_party_address->init(&_data[pointer[1].position + 1], _data[pointer[1].position] & 0xFF, _sccp_pool);

	// Third mandatory variable part
	data = (Data*)_sccp_pool->request_param(DATA);
	data->byte_pos = 0;
	data->init(&_data[pointer[2].position + 1], _data[pointer[2].position] & 0xFF, _sccp_pool);

}

void sccp::UDT_UnitData::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	int tmp_l = 0;
	unsigned char* tmp_pointers;
	if(buffer != NULL && buffer_length > 0){
		buffer += offset;
		// message type
		*buffer = type;
		tmp_l++;
		// protocol class
		protocol_class->encode(buffer, buffer_length, tmp_l, result_length, false, false);
		tmp_l += *result_length;

		// pointers
		//remember address and set first pointer
		tmp_pointers = buffer + tmp_l;
		pointer[0].value = 3;
		tmp_l += 3;

		// called_party_address
		called_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[1].value = *result_length;

		// calling_party_address
		calling_party_address->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;
		pointer[2].value = *result_length + pointer[1].value;

		//data
		data->encode(buffer, buffer_length, tmp_l, result_length, false, true);
		tmp_l += *result_length;

		// set pointers
		*(tmp_pointers) = pointer[0].value;
		*(tmp_pointers + 1) = pointer[1].value + 2;
		*(tmp_pointers + 2) = pointer[2].value + 1;


		// result
		*result_length = tmp_l;



	}
}

// SCCPPointer
sccp::SCCPPointer::SCCPPointer(){}

sccp::SCCPPointer::SCCPPointer(int _val, int _pos){
	value = _val;
	position = _pos;
}
sccp::SCCPPointer::~SCCPPointer(){}


// SCCPParam
sccp::SCCPParam::SCCPParam(){
	value_length = 0;
	byte_pos = 0;
	value = NULL;
	//new_value_mem = false;

}
sccp::SCCPParam::~SCCPParam(){
	//if(value != NULL && new_value_mem) delete value;
}

void sccp::SCCPParam::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	// set data pointer and length
	value = data;
	value_length = data_length;
}

void sccp::SCCPParam::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	buffer += offset;

	// data
	// param type
	if(enc_type){
		*(buffer++) = type;

	}
	// param length
	if(enc_length){
		*(buffer++) = value_length;

	}

	if(&value[0] != buffer){
		//std::cout << "m3ua::M3UAParam::encode> " << (int*)&value[0] << " != " << (int*)&buffer[4] << std::endl;
		// get actual data
		memcpy(buffer, value, value_length);
		//for(int i = 0; i<value_length; i++) buffer[i + 4] = value[i];

	}

	// get actual data
	//for(int i = 0; i<value_length; i++) *(buffer++) = value[i];

	// result length
	*result_length = value_length + (enc_length ? 1 : 0) + (enc_type ? 1 : 0);
}

// SCCPMessage
sccp::SCCPMessage::SCCPMessage(){
	pointer = NULL;
	pointer_size = 0;
	byte_pos = 0;
	optional_params.reserve(10);
}
sccp::SCCPMessage::~SCCPMessage(){
	if(pointer != NULL) delete[] pointer;
	SCCPParam* param = NULL;
	for(unsigned int i = 0; i<optional_params.size(); i++){
		param = optional_params[i];
		if(param != NULL) delete param;
	}
	optional_params.clear();
}
sccp::SCCPParam* sccp::SCCPMessage::get_optional_param(SCCPParameterType _param_type){
	SCCPParam* res_param = NULL;
	for(unsigned int i = 0; i<optional_params.size(); i++){
		res_param = optional_params[i];
		if(res_param->type == _param_type) return res_param;
	}
	return NULL;
}

int sccp::SCCPMessage::get_optional_params_count(){
	return optional_params.size();
}

void sccp::SCCPMessage::init_new(SCCPPool* _sccp_pool){

}
void sccp::SCCPMessage::process_optional_params(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	int tmp_byte_pos = 0;
	bool opt_done = false;
	int l = 0;
	SCCPParam* _param = NULL;
	if(data[0] != 0x00){
		// position = first optional parameter
		while(!opt_done){
			//_param = get_param((SCCPParameterType)(data[tmp_byte_pos++]));
			_param = _sccp_pool->request_param((SCCPParameterType)(data[tmp_byte_pos++]));
			if(_param != NULL){
				// End of optional parameters has no length, only one byte which is 0x00
				if(_param->type != END_OF_OPTIONAL_PARAMETERS){
					l = data[tmp_byte_pos++];
					_param->byte_pos = 0;
					_param->init(&data[tmp_byte_pos], l, _sccp_pool);
					optional_params.push_back(_param);
					tmp_byte_pos += l;
				}else{
					opt_done = true;
					l = 0;
				}
			}else opt_done = true;
		}
	}


}
void sccp::SCCPMessage::encode_optional_params(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	SCCPParam* param = NULL;
	int tmp_length = 0;
	*result_length = 0;
	if(optional_params.size() > 0){
		for(unsigned int i = 0; i<optional_params.size(); i++){
			param = optional_params[i];
			if(param != NULL){
				param->encode(buffer, buffer_length, offset, &tmp_length, true, true);
				offset += tmp_length;
				*result_length += tmp_length;
			}
		}
		// END OF OPTIONAL
		buffer[offset] = END_OF_OPTIONAL_PARAMETERS;
		*result_length += 1;


	}
}

sccp::SCCPParam* sccp::get_param(SCCPParameterType _param_type){
	switch(_param_type){
		case END_OF_OPTIONAL_PARAMETERS: return new Eoop();
		case DESTINATION_LOCAL_REFERENCE: return new Dlr();
		case SOURCE_LOCAL_REFERENCE: return new Slr();
		case CALLED_PARTY_ADDRESS: return new CalledPA();
		case CALLING_PARTY_ADDRESS: return new CallingPA();
		case PROTOCOL_CLASS: return new ProtocolClass();
		case SEGMENTING_REASSEMBLING: return new SegmReassm();
		case RECEIVE_SEQUENCE_NUMBER: return new ReceiveSeqNum();
		case SEQUENCING_SEGMENTING: return new SeqSegm();
		case CREDIT: return new Credit();
		case RELEASE_CAUSE: return new ReleaseCause();
		case RETURN_CAUSE: return new ReturnCause();
		case RESET_CAUSE: return new ResetCause();
		case ERROR_CAUSE: return new ErrorCause();
		case REFUSAL_CAUSE: return new RefusalCause();
		case DATA: return new Data();
		case SEGMENTATION: return new Segmentation();
		case HOP_COUNTER: return new HopCounter();
		case IMPORTANCE: return new Importance();
		case LONG_DATA: return new LongData();
	}
	return NULL;
}

// main decode method
sccp::SCCPMessage* sccp::decode(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	if(data != NULL && data_length > 0){
		MessageType mt = MessageType(data[0] & 0xFF);
		SCCPMessage* sccpm = _sccp_pool->request_message(mt);
		if(sccpm != NULL){
			sccpm->byte_pos = 0;
			sccpm->optional_params.clear();
			sccpm->init(data, data_length, _sccp_pool);
			return sccpm;
		}

	}
	return NULL;
}

// ProtocolClass
sccp::ProtocolClass::ProtocolClass(){
	type = PROTOCOL_CLASS;
}
sccp::ProtocolClass::~ProtocolClass(){}

void sccp::ProtocolClass::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	protocol_class = (ProtocolClassType)(data[0] & 0x0F);
	message_handling = MessageHandling(data[0] & 0xF0);

}
void sccp::ProtocolClass::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){

	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = protocol_class + message_handling;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// Eoop
sccp::Eoop::Eoop(){
	type = END_OF_OPTIONAL_PARAMETERS;
}


// Dlr
sccp::Dlr::Dlr(){
	type = DESTINATION_LOCAL_REFERENCE;
	dlr_value = 0;
}

void sccp::Dlr::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length >=3){
		dlr_value = (data[0] << 16) + (data[1] << 8) + (data[2] & 0xff);
	}
}
void sccp::Dlr::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 3;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=3){
		value[0] = dlr_value >> 16;
		value[1] = dlr_value >> 8;
		value[2] = dlr_value;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 3;

	value[0] = dlr_value >> 16;
	value[1] = dlr_value >> 8;
	value[2] = dlr_value;


	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// HopCounter
sccp::HopCounter::HopCounter(){
	type = HOP_COUNTER;
	hop_value = 0;
}
void sccp::HopCounter::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		hop_value = data[0] & 0xFF;

	}
}
void sccp::HopCounter::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = hop_value;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = hop_value;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// Importance
sccp::Importance::Importance(){
	type = IMPORTANCE;
	importance_value = 0;
}
void sccp::Importance::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		importance_value = data[0] & 0xFF;

	}
}
void sccp::Importance::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = importance_value;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = importance_value;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// ReceiveSeqNum
sccp::ReceiveSeqNum::ReceiveSeqNum(){
	type = RECEIVE_SEQUENCE_NUMBER;
	receive_seq_num = 0;
}
void sccp::ReceiveSeqNum::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		receive_seq_num = (data[0] & 0xFF) >> 1;

	}
}
void sccp::ReceiveSeqNum::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = receive_seq_num << 1;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = receive_seq_num << 1;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// RefusalCause
sccp::RefusalCause::RefusalCause(){
	type = REFUSAL_CAUSE;
}
void sccp::RefusalCause::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		refusal_cause = (RefusalCauseType)(data[0] & 0xFF);

	}
}
void sccp::RefusalCause::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = refusal_cause;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = refusal_cause;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// ReleaseCause
sccp::ReleaseCause::ReleaseCause(){
	type = RELEASE_CAUSE;
}
void sccp::ReleaseCause::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		release_cause = (ReleaseCauseType)(data[0] & 0xFF);

	}
}
void sccp::ReleaseCause::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = release_cause;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = release_cause;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}


// ResetCause
sccp::ResetCause::ResetCause(){
	type = RESET_CAUSE;;
}
void sccp::ResetCause::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		reset_cause = (ResetCauseType)(data[0] & 0xFF);

	}
}
void sccp::ResetCause::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){

	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = reset_cause;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = reset_cause;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// ReturnCause
sccp::ReturnCause::ReturnCause(){
	type = RETURN_CAUSE;;
}
void sccp::ReturnCause::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		return_cause = (ReturnCauseType)(data[0] & 0xFF);

	}
}
void sccp::ReturnCause::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = return_cause;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = return_cause;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// Segmentation
sccp::Segmentation::Segmentation(){
	type = SEGMENTATION;
	segmentation_local_ref = NULL;
	segmentation_local_ref_length = 0;
}
void sccp::Segmentation::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		is_first_segment = (data[0] & 0x80) == 0x80;
		class_type = (data[0] & 0x40) >>6;
		num_of_remaining_segments = data[0] & 0x0F;
		segmentation_local_ref = &data[1];
		segmentation_local_ref_length = data_length - 1;


	}
}
void sccp::Segmentation::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1 + segmentation_local_ref_length;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = (is_first_segment ? 0x80 : 0x00) + (class_type << 6) + num_of_remaining_segments;
		if(segmentation_local_ref_length > 0 && segmentation_local_ref != NULL){
			memcpy(&value[1], segmentation_local_ref, segmentation_local_ref_length);
		}

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1 + segmentation_local_ref_length;
	value[0] = (is_first_segment ? 0x80 : 0x00) + (class_type << 6) + num_of_remaining_segments;
	if(segmentation_local_ref_length > 0 && segmentation_local_ref != NULL){
		memcpy(&value[1], segmentation_local_ref, segmentation_local_ref_length);
	}

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}


// SegmReassm
sccp::SegmReassm::SegmReassm(){
	type = SEGMENTING_REASSEMBLING;
}
void sccp::SegmReassm::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		more_data_indication =  (data[0] & 0x01) == 0x01;
	}
}
void sccp::SegmReassm::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = (more_data_indication ? 0x01 : 0x00);

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = (more_data_indication ? 0x01 : 0x00);

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// SeqSegm
sccp::SeqSegm::SeqSegm(){
	type = SEQUENCING_SEGMENTING;
}
void sccp::SeqSegm::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 1){
		send_seq_num = (data[0] & 0xFE) >> 1;
		recv_seq_num = (data[1] & 0xFE) >> 1;
		more_data_indication = (data[1] & 0x01) == 0x01;

	}
}
void sccp::SeqSegm::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 2;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=2){
		value[0] = send_seq_num << 1;
		value[1] = recv_seq_num << 1;
		value[1] = value[1] | (more_data_indication ? 0x01 : 0x00);

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 2;
	value[0] = send_seq_num << 1;
	value[1] = recv_seq_num << 1;
	value[1] = value[1] | (more_data_indication ? 0x01 : 0x00);

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// Data
sccp::Data::Data(){
	type = DATA;
}

// Slr
sccp::Slr::Slr(){
	type = SOURCE_LOCAL_REFERENCE;
}
void sccp::Slr::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 2){
		slr_value =  (data[0] << 16) + (data[1] << 8) + data[2];

	}
}
void sccp::Slr::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 3;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=3){
		value[0] = slr_value >> 16;
		value[1] = slr_value >> 8;
		value[2] = slr_value;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 3;
	value[0] = slr_value >> 16;
	value[1] = slr_value >> 8;
	value[2] = slr_value;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}


// LongData
sccp::LongData::LongData(){
	type = LONG_DATA;
}
void sccp::LongData::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	buffer += offset;

	// data
	// param type
	if(enc_type){
		*(buffer++) = type;

	}
	// param length
	if(enc_length){
		buffer[0] = value_length >> 8;
		buffer[1] = value_length;
		buffer += 2;

	}

	if(&value[0] != buffer){
		//std::cout << "m3ua::M3UAParam::encode> " << (int*)&value[0] << " != " << (int*)&buffer[4] << std::endl;
		// get actual data
		memcpy(buffer, value, value_length);
		//for(int i = 0; i<value_length; i++) buffer[i + 4] = value[i];

	}


	// get actual data
	//for(int i = 0; i<value_length; i++) *(buffer++) = value[i];

	// result length
	*result_length = value_length + (enc_length ? 2 : 0) + (enc_type ? 1 : 0);
}


// Credit
sccp::Credit::Credit(){
	type = CREDIT;
	window_size = 0;
}

void sccp::Credit::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		window_size = data[0] & 0xFF;

	}


}
void sccp::Credit::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = window_size;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = window_size;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}

// ErrorCause
sccp::ErrorCause::ErrorCause(){
	type = ERROR_CAUSE;
}
void sccp::ErrorCause::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	if(data != NULL && data_length > 0){
		error_cause = ErrorCauseType(data[0] & 0xFF);

	}

}
void sccp::ErrorCause::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	/*
	if(value == NULL){
		value_length = 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	if(value_length >=1){
		value[0] = error_cause;

	}
	*/
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = 1;
	value[0] = error_cause;

	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
}


// CallingPA
sccp::CallingPA::CallingPA(){
	type = CALLING_PARTY_ADDRESS;
	global_title = NULL;
}
sccp::CallingPA::~CallingPA(){
	//if(global_title != NULL) delete global_title;

}

void sccp::CallingPA::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	routing_indicator = (RoutingIndicator)(data[byte_pos] & 0x40);
	global_title_indicator = (GlobalTitleIndicator)(data[byte_pos] & 0x3C);
	ssn_indicator = (data[byte_pos] & 0x02) == 0x02;
	point_code_indicator = (data[byte_pos] & 0x01) == 0x01;
	byte_pos++;
	if(point_code_indicator){
		point_code = ((data[byte_pos + 1] & 0x3f) << 8) + (data[byte_pos]);
		byte_pos += 2;
	}
	if(ssn_indicator){
		subsystem_number = (SubsystemNumber)(data[byte_pos]);
		byte_pos++;
	}

	global_title = GlobalTitle::decode(&data[byte_pos], data_length - byte_pos, global_title_indicator, global_title, _sccp_pool);

}

void sccp::CallingPA::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	int res_length = 0;

	// encode GT is present
	if(global_title != NULL && global_title_indicator != GT_NO_TITLE){
		global_title->encode(buffer,
				     buffer_length,
				     offset + (point_code_indicator ? 2 : 0) + (ssn_indicator ? 1 : 0) + (enc_type ? 1 : 0) + (enc_length ? 1 : 0) + 1,
				     &res_length);
	}

	// set value pointer
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = res_length + (point_code_indicator ? 2 : 0) + (ssn_indicator ? 1 : 0) + 1;


	// common header
	byte_pos = 0;
	value[byte_pos++] = routing_indicator + global_title_indicator + (ssn_indicator ? 0x02 : 0x00) + (point_code_indicator ? 0x01 : 0x00);
	// point code if used
	if(point_code_indicator){
		value[byte_pos++] = point_code;
		value[byte_pos++] = point_code & 0x3f >> 8;

	}
	// sybsystem number if used
	if(ssn_indicator){
		value[byte_pos++] = subsystem_number;

	}

	// encode
	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
	//std::cout << std::dec << "void sccp::CalledPA::encode: " << *result_length << std::endl;

}


// CalledPA
sccp::CalledPA::CalledPA(){
	type = CALLED_PARTY_ADDRESS;
	global_title = NULL;
}
sccp::CalledPA::~CalledPA(){
	//if(global_title != NULL) delete global_title;

}

void sccp::CalledPA::init(unsigned char* data, int data_length, SCCPPool* _sccp_pool){
	SCCPParam::init(data, data_length, _sccp_pool);
	routing_indicator = (RoutingIndicator)(data[byte_pos] & 0x40);
	global_title_indicator = (GlobalTitleIndicator)(data[byte_pos] & 0x3C);
	ssn_indicator = (data[byte_pos] & 0x02) == 0x02;
	point_code_indicator = (data[byte_pos] & 0x01) == 0x01;
	byte_pos++;
	if(point_code_indicator){
		point_code = ((data[byte_pos + 1] & 0x3f) << 8) + (data[byte_pos]);
		byte_pos += 2;
	}
	if(ssn_indicator){
		subsystem_number = (SubsystemNumber)(data[byte_pos]);
		byte_pos++;
	}
	global_title = GlobalTitle::decode(&data[byte_pos], data_length - byte_pos, global_title_indicator, global_title, _sccp_pool);
}

void sccp::CalledPA::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length){
	int res_length = 0;

	// encode GT is present
	if(global_title != NULL && global_title_indicator != GT_NO_TITLE){
		global_title->encode(buffer,
				     buffer_length,
				     offset + (point_code_indicator ? 2 : 0) + (ssn_indicator ? 1 : 0) + (enc_type ? 1 : 0) + (enc_length ? 1 : 0) + 1,
				     &res_length);
	}

	// set value pointer
	value = &buffer[offset + (enc_type ? 1 : 0) + (enc_length ? 1 : 0)];
	value_length = res_length + (point_code_indicator ? 2 : 0) + (ssn_indicator ? 1 : 0) + 1;

	// common header
	byte_pos = 0;
	value[byte_pos++] = routing_indicator + global_title_indicator + (ssn_indicator ? 0x02 : 0x00) + (point_code_indicator ? 0x01 : 0x00);
	// point code if used
	if(point_code_indicator){
		value[byte_pos++] = point_code;
		value[byte_pos++] = point_code & 0x3f >> 8;

	}
	// sybsystem number if used
	if(ssn_indicator){
		value[byte_pos++] = subsystem_number;

	}

	// encode
	SCCPParam::encode(buffer, buffer_length, offset, result_length, enc_type, enc_length);
	//std::cout << std::dec << "void sccp::CalledPA::encode: " << *result_length << std::endl;


}

// GT_TTNPENOA
sccp::GT_TTNPENOA::GT_TTNPENOA(){
	type = GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
}
sccp::GT_TTNPENOA::~GT_TTNPENOA(){
	//if(address_information != NULL && new_value_mem) delete address_information;

}
void sccp::GT_TTNPENOA::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	if(value == NULL){
		value_length = address_information_length + 3;
		value = new char[value_length];
		new_value_mem = true;
	}
	*/
	value = &buffer[offset];
	value_length = address_information_length + 3;

	value[0] = translation_type;
	value[1] = numbering_plan + encoding_scheme;
	value[2] = nature_of_address;

	if(address_information_length > 0 && address_information != NULL){
		memcpy(&value[3], address_information, address_information_length);
	}

	GlobalTitle::encode(buffer, buffer_length, offset, result_length);
}
void sccp::GT_TTNPENOA::init(unsigned char* data, int data_length){
	GlobalTitle::init(data, data_length);
	translation_type = data[0] & 0xFF;
	numbering_plan = (NumberingPlan)(data[1] & 0xF0);
	encoding_scheme = (EncodingScheme)(data[1] & 0x0F);
	nature_of_address = (NatureOfAddress)(data[2] & 0xFF);
	address_information = &data[3];
	address_information_length = data_length - 3;

}


// GT TTNPE
sccp::GT_TTNPE::GT_TTNPE(){
	type = GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING;
}
sccp::GT_TTNPE::~GT_TTNPE(){
	//if(address_information != NULL && new_value_mem) delete address_information;

}
void sccp::GT_TTNPE::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	if(value == NULL){
		value_length = address_information_length + 2;
		value = new char[value_length];
		new_value_mem = true;
	}
	*/
	value = &buffer[offset];
	value_length = address_information_length + 2;

	value[0] = translation_type;
	value[1] = numbering_plan + encoding_scheme;

	if(address_information_length > 0 && address_information != NULL){
		memcpy(&value[2], address_information, address_information_length);
	}

	GlobalTitle::encode(buffer, buffer_length, offset, result_length);
}

void sccp::GT_TTNPE::init(unsigned char* data, int data_length){
	GlobalTitle::init(data, data_length);
	translation_type = data[0] & 0xFF;
	numbering_plan = (NumberingPlan)(data[1] & 0xF0);
	encoding_scheme = (EncodingScheme)(data[1] & 0x0F);
	address_information = &data[2];
	address_information_length = data_length - 2;

}

// GT TT
sccp::GT_TT::GT_TT(){
	type = GT_TRANSLATION_TYPE_ONLY;
}
sccp::GT_TT::~GT_TT(){
	//if(address_information != NULL && new_value_mem) delete address_information;

}
void sccp::GT_TT::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	if(value == NULL){
		value_length = address_information_length + 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	*/
	value = &buffer[offset];
	value_length = address_information_length + 1;

	value[0] = translation_type;

	if(address_information_length > 0 && address_information != NULL){
		memcpy(&value[1], address_information, address_information_length);
	}

	GlobalTitle::encode(buffer, buffer_length, offset, result_length);

}

void sccp::GT_TT::init(unsigned char* data, int data_length){
	GlobalTitle::init(data, data_length);
	translation_type = data[0] & 0xFF;
	address_information = &data[1];
	address_information_length = data_length - 1;

}


// GT NOA
sccp::GT_NOA::GT_NOA(){
	type = GT_NATURE_OF_ADDRESS_INDICATOR_ONLY;
}
sccp::GT_NOA::~GT_NOA(){
	//if(address_information != NULL && new_value_mem) delete address_information;

}
void sccp::GT_NOA::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	/*
	if(value == NULL){
		value_length = address_information_length + 1;
		value = new char[value_length];
		new_value_mem = true;
	}
	*/
	value = &buffer[offset];
	value_length = address_information_length + 1;

	value[0] = nature_of_address;
	//if(address_information_length % 2 != 0) value[0] = value[0] | 0x80;
	if(odd_num_of_addr_signals) value[0] |= 0x80;

	if(address_information_length > 0 && address_information != NULL){
		memcpy(&value[1], address_information, address_information_length);
	}

	GlobalTitle::encode(buffer, buffer_length, offset, result_length);
}


void sccp::GT_NOA::init(unsigned char* data, int data_length){
	GlobalTitle::init(data, data_length);
	nature_of_address = (NatureOfAddress)(data[0] & 0x7F);
	odd_num_of_addr_signals = (data[0] & 0x80) == 0x80;
	address_information = &data[1];
	address_information_length = data_length - 1;

}
// GlobalTitle
sccp::GlobalTitle::GlobalTitle(){
	value_length = 0;
	value = NULL;
	//new_value_mem = false;
}
sccp::GlobalTitle::~GlobalTitle(){
	//if(value != NULL && new_value_mem) delete value;

}

void sccp::GlobalTitle::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
	buffer += offset;

	if(value != buffer){
		//std::cout << "m3ua::M3UAParam::encode> " << (int*)&value[0] << " != " << (int*)&buffer[4] << std::endl;
		// get actual data
		memcpy(buffer, value, value_length);
		//for(int i = 0; i<value_length; i++) buffer[i + 4] = value[i];

	}

	//for(int i = 0; i<value_length; i++) *(buffer++) = value[i];
	// result length
	*result_length = value_length;

}
void sccp::GlobalTitle::init(unsigned char* data, int data_length){
	// set data pointer and length
	value = data;
	value_length = data_length;
}



// SCCPMessagePoolItem
sccp::SCCPMessagePoolItem::SCCPMessagePoolItem(){
	pool = NULL;
	next_free_item = NULL;
	total_count = 0;
	free_count = 0;

}

sccp::SCCPMessagePoolItem::~SCCPMessagePoolItem(){
    for(int i = 0; i<total_count; i++) delete pool[i];
    delete[] pool;

}

sccp::SCCPMessage* sccp::SCCPMessagePoolItem::create_message(MessageType _msg_type){
	SCCPMessage* sccpm = NULL;
	switch(_msg_type){
		case UDT_UNITDATA: sccpm = new UDT_UnitData(); break;
		case AK_DATA_ACKNOWLEDGEMENT: sccpm = new AK_DataAcknowledgement(); break;
		case CC_CONNECTION_CONFIRM: sccpm = new CC_ConnectionConfirm(); break;
		case CR_CONNECTION_REQUEST:	sccpm = new CR_ConnectionRequest(); break;
		case CREF_CONNECTION_REFUSED: sccpm = new CREF_ConnectionRefused(); break;
		case DT1_DATA_FORM_1: sccpm = new DT1_DataForm1(); break;
		case DT2_DATA_FORM_2: sccpm = new DT2_DataForm2(); break;
		case EA_EXPEDITED_DATA_ACKNOWLEDGMENT: sccpm = new EA_ExpeditedDataAck(); break;
		case ED_EXPEDITED_DATA: sccpm = new ED_ExpeditedData(); break;
		case ERR_PROTOCOL_DATA_UNIT_ERROR: sccpm = new ERR_PduError(); break;
		case IT_INACTIVITY_TEST: sccpm = new IT_InactivityTest(); break;
		case LUDT_LONG_UNIDATA: sccpm = new LUDT_LongUnitdata(); break;
		case LUDTS_LONG_UNIDATA_SERVICE: sccpm = new LUDTS_LongUnitdataService(); break;
		case RLC_RELEASE_COMPLETE: sccpm = new RLC_ReleaseComplete(); break;
		case RLSD_RELEASED: sccpm = new RLSD_Released(); break;
		case RSC_RESET_CONFIRM: sccpm = new RSC_ResetConfirmation(); break;
		case RSR_RESET_REQUEST: sccpm = new RSR_ResetRequest(); break;
		case UDTS_UNITDATA_SERVICE: sccpm = new UDTS_UnitDataService(); break;
		case XUDT_EXTENDED_UNITDATA: sccpm = new XUDT_ExtendedUnitdata(); break;
		case XUDTS_EXTENDED_UNIDATA_SERVICE: sccpm = new XUDTS_ExtendedUnitdataService(); break;

	}

	return sccpm;

}

void sccp::SCCPMessagePoolItem::init_pool(){
	pool = new SCCPMessage *[total_count];
	for(int i = 0; i<total_count; i++) pool[i] = create_message(type);
	next_free_item = pool[total_count - 1];
	//free_count--;
}

void sccp::SCCPMessagePoolItem::set_pool_size(int _total_count){
	total_count = _total_count;
	free_count = total_count;
}

sccp::SCCPMessage* sccp::SCCPMessagePoolItem::request_item(){
	SCCPMessage* tmp = next_free_item;
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

void sccp::SCCPMessagePoolItem::return_item(SCCPMessage* item){
	next_free_item = item;
	pool[free_count++] = item;
}

int sccp::SCCPMessagePoolItem::get_free_count(){
	return free_count;
}


// SCCPGTPoolItem
sccp::SCCPGTPoolItem::SCCPGTPoolItem(){
	pool = NULL;
	next_free_item = NULL;
	total_count = 0;
	free_count = 0;
}

sccp::SCCPGTPoolItem::~SCCPGTPoolItem(){
    for(int i = 0; i<total_count; i++) delete pool[i];
    delete[] pool;

}

sccp::GlobalTitle* sccp::SCCPGTPoolItem::create_gt(GlobalTitleIndicator _param_type){
	switch(_param_type){
		case GT_NO_TITLE: return NULL;
		case GT_NATURE_OF_ADDRESS_INDICATOR_ONLY: return new GT_NOA();
		case GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING: return new GT_TTNPE();
		case GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS: return new GT_TTNPENOA();
		case GT_TRANSLATION_TYPE_ONLY: return new GT_TT();
	}
	return NULL;

}

void sccp::SCCPGTPoolItem::init_pool(){
	pool = new GlobalTitle *[total_count];
	for(int i = 0; i<total_count; i++) pool[i] = create_gt(type);
	next_free_item = pool[total_count - 1];
	//free_count--;
}

void sccp::SCCPGTPoolItem::set_pool_size(int _total_count){
	total_count = _total_count;
	free_count = total_count;
}


sccp::GlobalTitle* sccp::SCCPGTPoolItem::request_item(){
	GlobalTitle* tmp = next_free_item;
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

int sccp::SCCPGTPoolItem::get_free_count(){
	return free_count;
}
void sccp::SCCPGTPoolItem::return_item(GlobalTitle* item){
	next_free_item = item;
	free_count++;
	pool[free_count++] = item;
}


// SCCPParamPoolItem
sccp::SCCPParamPoolItem::SCCPParamPoolItem(){
	pool = NULL;
	next_free_item = NULL;
	total_count = 0;
	free_count = 0;
}

sccp::SCCPParamPoolItem::~SCCPParamPoolItem(){
    for(int i = 0; i<total_count; i++) delete pool[i];
    delete[] pool;

}

sccp::SCCPParam* sccp::SCCPParamPoolItem::create_param(SCCPParameterType _param_type){
	switch(_param_type){
		case END_OF_OPTIONAL_PARAMETERS: return new Eoop();
		case DESTINATION_LOCAL_REFERENCE: return new Dlr();
		case SOURCE_LOCAL_REFERENCE: return new Slr();
		case CALLED_PARTY_ADDRESS: return new CalledPA();
		case CALLING_PARTY_ADDRESS: return new CallingPA();
		case PROTOCOL_CLASS: return new ProtocolClass();
		case SEGMENTING_REASSEMBLING: return new SegmReassm();
		case RECEIVE_SEQUENCE_NUMBER: return new ReceiveSeqNum();
		case SEQUENCING_SEGMENTING: return new SeqSegm();
		case CREDIT: return new Credit();
		case RELEASE_CAUSE: return new ReleaseCause();
		case RETURN_CAUSE: return new ReturnCause();
		case RESET_CAUSE: return new ResetCause();
		case ERROR_CAUSE: return new ErrorCause();
		case REFUSAL_CAUSE: return new RefusalCause();
		case DATA: return new Data();
		case SEGMENTATION: return new Segmentation();
		case HOP_COUNTER: return new HopCounter();
		case IMPORTANCE: return new Importance();
		case LONG_DATA: return new LongData();

	}
	return NULL;
}

void sccp::SCCPParamPoolItem::init_pool(){
	pool = new SCCPParam *[total_count];
	for(int i = 0; i<total_count; i++) pool[i] = create_param(type);
	next_free_item = pool[total_count - 1];
	//free_count--;
}

void sccp::SCCPParamPoolItem::set_pool_size(int _total_count){
	total_count = _total_count;
	free_count = total_count;
}

sccp::SCCPParam* sccp::SCCPParamPoolItem::request_item(){
	SCCPParam* tmp = next_free_item;
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

int sccp::SCCPParamPoolItem::get_free_count(){
	return free_count;
}
void sccp::SCCPParamPoolItem::return_item(SCCPParam* item){
	next_free_item = item;
	free_count++;
	pool[free_count++] = item;
}


// SCCPPool
sccp::SCCPPool::SCCPPool() : param_count(10), message_count(10) {

}

sccp::SCCPPool::~SCCPPool(){
    SCCPMessagePoolItem* tmp_msg = NULL;
    std::map<MessageType, SCCPMessagePoolItem*>::iterator it = MESSAGE_POOL.begin();
    for(; it != MESSAGE_POOL.end(); it++){
	tmp_msg = it->second;
	delete tmp_msg;
    }

    SCCPParamPoolItem* tmp_param = NULL;
    std::map<SCCPParameterType, SCCPParamPoolItem*>::iterator it2 = PARAM_POOL.begin();
    for(; it2 != PARAM_POOL.end(); it2++){
	tmp_param = it2->second;
	delete tmp_param;

    }

    SCCPGTPoolItem* tmp_gt = NULL;
    std::map<GlobalTitleIndicator, SCCPGTPoolItem*>::iterator it3 = GT_POOL.begin();
    for(; it3 != GT_POOL.end(); it3++){
	tmp_gt = it3->second;
	delete tmp_gt;

    }

}

void sccp::SCCPPool::set_pool_size(int _param_count, int _message_count){
	param_count = _param_count;
	message_count = _message_count;
}
void sccp::SCCPPool::init_message(MessageType _msg_type){
	MESSAGE_POOL[_msg_type] = new SCCPMessagePoolItem();//[message_count];
	MESSAGE_POOL[_msg_type]->type = _msg_type;
	MESSAGE_POOL[_msg_type]->set_pool_size(message_count);
	MESSAGE_POOL[_msg_type]->init_pool();

}

void sccp::SCCPPool::init_param(SCCPParameterType _param_type){
	PARAM_POOL[_param_type] = new SCCPParamPoolItem();//[param_count];
	PARAM_POOL[_param_type]->type = _param_type;
	PARAM_POOL[_param_type]->set_pool_size(param_count);
	PARAM_POOL[_param_type]->init_pool();
}

void sccp::SCCPPool::init_gt(GlobalTitleIndicator _gt_type){
	GT_POOL[_gt_type] = new SCCPGTPoolItem();//[param_count];
	GT_POOL[_gt_type]->type = _gt_type;
	GT_POOL[_gt_type]->set_pool_size(param_count);
	GT_POOL[_gt_type]->init_pool();
}



sccp::SCCPMessage* sccp::SCCPPool::request_message(MessageType msg_type){
    std::map<MessageType, SCCPMessagePoolItem*>::iterator it = MESSAGE_POOL.find(msg_type);
    if(it == MESSAGE_POOL.end()) return NULL;
    return it->second->request_item();
    //return MESSAGE_POOL[msg_type]->request_item();
}

void sccp::SCCPPool::return_message(SCCPMessage *message){
	if(message != NULL) MESSAGE_POOL[message->type]->return_item(message);
}

sccp::SCCPParam* sccp::SCCPPool::request_param(SCCPParameterType param_type){
    std::map<SCCPParameterType, SCCPParamPoolItem*>::iterator it = PARAM_POOL.find(param_type);
    if(it == PARAM_POOL.end()) return NULL;
    return it->second->request_item();
    //return PARAM_POOL[param_type]->request_item();
}

void sccp::SCCPPool::return_param(SCCPParam *param){
	if(param != NULL) PARAM_POOL[param->type]->return_item(param);
}

sccp::GlobalTitle* sccp::SCCPPool::request_gt(GlobalTitleIndicator gt_type){
    std::map<GlobalTitleIndicator, SCCPGTPoolItem*>::iterator it = GT_POOL.find(gt_type);
    if(it == GT_POOL.end()) return NULL;
    return it->second->request_item();
    //return GT_POOL[gt_type]->request_item();
}

void sccp::SCCPPool::return_gt(GlobalTitle* gt){
	if(gt != NULL) GT_POOL[gt->type]->return_item(gt);
}



int sccp::SCCPPool::get_free_message_count(MessageType msg_type){
	return MESSAGE_POOL[msg_type]->get_free_count();
}

int sccp::SCCPPool::get_free_param_count(SCCPParameterType param_type){
	return PARAM_POOL[param_type]->get_free_count();
}

void sccp::SCCPPool::init_pool(){

	// message pool
	init_message(UDT_UNITDATA);
	init_message(AK_DATA_ACKNOWLEDGEMENT);
	init_message(CC_CONNECTION_CONFIRM);
	init_message(CR_CONNECTION_REQUEST);
	init_message(CREF_CONNECTION_REFUSED);
	init_message(DT1_DATA_FORM_1);
	init_message(DT2_DATA_FORM_2);
	init_message(EA_EXPEDITED_DATA_ACKNOWLEDGMENT);
	init_message(ED_EXPEDITED_DATA);
	init_message(ERR_PROTOCOL_DATA_UNIT_ERROR);
	init_message(IT_INACTIVITY_TEST);
	init_message(LUDT_LONG_UNIDATA);
	init_message(LUDTS_LONG_UNIDATA_SERVICE);
	init_message(RLC_RELEASE_COMPLETE);
	init_message(RLSD_RELEASED);
	init_message(RSC_RESET_CONFIRM);
	init_message(RSR_RESET_REQUEST);
	init_message(UDTS_UNITDATA_SERVICE);
	init_message(XUDT_EXTENDED_UNITDATA);
	init_message(XUDTS_EXTENDED_UNIDATA_SERVICE);

	// param pool
	init_param(END_OF_OPTIONAL_PARAMETERS);
	init_param(DESTINATION_LOCAL_REFERENCE);
	init_param(SOURCE_LOCAL_REFERENCE);
	init_param(CALLED_PARTY_ADDRESS);
	init_param(CALLING_PARTY_ADDRESS);
	init_param(PROTOCOL_CLASS);
	init_param(SEGMENTING_REASSEMBLING);
	init_param(RECEIVE_SEQUENCE_NUMBER);
	init_param(SEQUENCING_SEGMENTING);
	init_param(CREDIT);
	init_param(RELEASE_CAUSE);
	init_param(RETURN_CAUSE);
	init_param(RESET_CAUSE);
	init_param(ERROR_CAUSE);
	init_param(REFUSAL_CAUSE);
	init_param(DATA);
	init_param(SEGMENTATION);
	init_param(HOP_COUNTER);
	init_param(IMPORTANCE);
	init_param(LONG_DATA);

	// gt pool
	init_gt(GT_NATURE_OF_ADDRESS_INDICATOR_ONLY);
	init_gt(GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING);
	init_gt(GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	init_gt(GT_TRANSLATION_TYPE_ONLY);


}

// static methods
sccp::GlobalTitle* sccp::GlobalTitle::decode(unsigned char* data, int data_length, GlobalTitleIndicator gi, GlobalTitle* _gt, SCCPPool* _sccp_pool){
	if(data != NULL && data_length > 0){
		GlobalTitle* gt = _sccp_pool->request_gt(gi);
		//GlobalTitle* gt  = NULL;
		// resue GT object if possible
		//if(_gt == NULL) gt = _sccp_pool->request_gt(gi);
		//else if(_gt->type != gi) gt = _sccp_pool->request_gt(gi);
		//else gt = _gt;

		if(gt != NULL) gt->init(data, data_length);
		return gt;
	}
	return NULL;

}

sccp::GlobalTitle* sccp::GlobalTitle::prepareNew(GlobalTitleIndicator type){
	switch(type){
		case GT_NO_TITLE: return NULL;
		case GT_NATURE_OF_ADDRESS_INDICATOR_ONLY: return new GT_NOA();
		case GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING: return new GT_TTNPE();
		case GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS: return new GT_TTNPENOA();
		case GT_TRANSLATION_TYPE_ONLY: return new GT_TT();
	}
	return NULL;
}



