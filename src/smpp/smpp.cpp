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

#include <smpp.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

// const
const char* smpp::SMPP_NULL_STR = "";

// SmppParam
smpp::SmppParam::SmppParam(): type(SMPP_PARAM_UNKNOWN), param_data(NULL), length(0), byte_pos(0){}
smpp::SmppParam::SmppParam(SmppParameterType _type): type(_type), param_data(NULL), length(0), byte_pos(0){}
smpp::SmppParam::~SmppParam(){}

int smpp::SmppParam::init(unsigned char* data, int data_length){
    param_data = data;
    length = data_length;
    return 0;
}

int smpp::SmppParam::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // parameter type = two bytes
    // parameter length = two bytes
    // data
    buffer[0] = type >> 8;
    buffer[1] = type;
    buffer[2] = length >> 8;
    buffer[3] = length;
    // if value already in buffer (buffer and value addresses are equal), skip
    if(&param_data[0] != &buffer[4]) memcpy(&buffer[4], param_data, length);
    // result length
    *result_length = 4 + length;
    // ok
    return 0;

}

// Additional_status_info_text
smpp::Additional_status_info_text::Additional_status_info_text(): SmppParam(ADDITIONAL_STATUS_INFO_TEXT){}
smpp::Additional_status_info_text::~Additional_status_info_text(){}

// Alert_on_message_delivery
smpp::Alert_on_message_delivery::Alert_on_message_delivery(): SmppParam(ALERT_ON_MESSAGE_DELIVERY){}
smpp::Alert_on_message_delivery::~Alert_on_message_delivery(){}

// Callback_num_atag
smpp::Callback_num_atag::Callback_num_atag():
	SmppParam(CALLBACK_NUM_ATAG),
	data_coding(DC_DEFAULT),
	character_octets(NULL),
	character_octet_count(0){}
smpp::Callback_num_atag::~Callback_num_atag(){}

int smpp::Callback_num_atag::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    // data coding
    data_coding = (Data_codingType)*data;
    ++data;
    --data_length;
    // character octets
    character_octets = data;
    character_octet_count = data_length;
    return 0;
}

int smpp::Callback_num_atag::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = data_coding;
    memcpy(&param_data[1], character_octets, character_octet_count);
    length = character_octet_count + 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Callback_num_pres_ind
smpp::Callback_num_pres_ind::Callback_num_pres_ind(): SmppParam(CALLBACK_NUM_PRES_IND),
						      presentation_indicator(PI_ALLOWED),
						      screening_indicator(SI_NOT_SCREENED){}
smpp::Callback_num_pres_ind::~Callback_num_pres_ind(){}

int smpp::Callback_num_pres_ind::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    // presentation_indicator
    presentation_indicator = (PresentationIndicator)(*data & 0x0c);
    // screening_indicator
    screening_indicator = (ScreeningIndicator)(*data & 0x03);
    // ok
    return 0;
}

int smpp::Callback_num_pres_ind::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = presentation_indicator | screening_indicator;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Callback_num
smpp::Callback_num::Callback_num(): SmppParam(CALLBACK_NUM),
				    dtmf(false),
				    ton(TON_UNKNOWN),
				    npi(UNKNOWN),
				    digits(NULL),
				    digit_count(0){}
smpp::Callback_num::~Callback_num(){}

int smpp::Callback_num::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    // dtmf
    dtmf = (*data & 0x01) == 0x01;
    ++data;
    // ton
    ton = (SmppTON)*data;
    ++data;
    // npi
    npi = (SmppNPI)*data;
    ++data;
    // digits
    digits = data;
    digit_count = data_length - 3;
    // ok
    return 0;
}

int smpp::Callback_num::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = dtmf;
    param_data[1] = ton;
    param_data[2] = npi;
    memcpy(&param_data[3], digits, digit_count);
    length = digit_count + 3;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Delivery_failure_reason
smpp::Delivery_failure_reason::Delivery_failure_reason(): SmppParam(DELIVERY_FAILURE_REASON),
							  dfr_type(DFR_DESTINATION_UNAVAILABLE){}
smpp::Delivery_failure_reason::~Delivery_failure_reason(){}

int smpp::Delivery_failure_reason::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    dfr_type = (DeliveryFailureReasonType)*data;
    // ok
    return 0;
}

int smpp::Delivery_failure_reason::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = dfr_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Dest_addr_subunit
smpp::Dest_addr_subunit::Dest_addr_subunit(): SmppParam(DEST_ADDR_SUBUNIT),
					      das_type(DAS_UNKNOWN){}
smpp::Dest_addr_subunit::~Dest_addr_subunit(){}

int smpp::Dest_addr_subunit::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    das_type = (DestAddrSubunitType)*data;
    // ok
    return 0;
}

int smpp::Dest_addr_subunit::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = das_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Dest_bearer_type
smpp::Dest_bearer_type::Dest_bearer_type(): SmppParam(DEST_BEARER_TYPE),
					    db_type(DB_UNKNOWN){}
smpp::Dest_bearer_type::~Dest_bearer_type(){}

int smpp::Dest_bearer_type::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    db_type = (DestBearerCodeType)*data;
    // ok
    return 0;
}

int smpp::Dest_bearer_type::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = db_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Dest_network_type
smpp::Dest_network_type::Dest_network_type(): SmppParam(DEST_NETWORK_TYPE),
					      dn_type(DN_UNKNOWN){}
smpp::Dest_network_type::~Dest_network_type(){}

int smpp::Dest_network_type::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    dn_type = (DestNetworkCodeType)*data;
    // ok
    return 0;
}

int smpp::Dest_network_type::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = dn_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Dest_subaddress
smpp::Dest_subaddress::Dest_subaddress(): SmppParam(DEST_SUBADDRESS),
					  ds_type(DS_NSAP_EVEN),
					  subaddress(NULL),
					  subaddress_length(0){}
smpp::Dest_subaddress::~Dest_subaddress(){}

int smpp::Dest_subaddress::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    ds_type = (DestSubaddressType)*data;
    ++data;
    subaddress = data;
    subaddress_length = data_length - 1;
    // ok
    return 0;
}

int smpp::Dest_subaddress::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = ds_type;
    memcpy(&param_data[1], subaddress, subaddress_length);
    length = subaddress_length + 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Dest_telematics_id
smpp::Dest_telematics_id::Dest_telematics_id(): SmppParam(DEST_TELEMATICS_ID),
						value(0){}
smpp::Dest_telematics_id::~Dest_telematics_id(){}

int smpp::Dest_telematics_id::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = (data[0] << 8) | data[1];
    // ok
    return 0;
}

int smpp::Dest_telematics_id::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = value >> 8;
    param_data[1] = value;
    length = 2;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}


// Destination_port
smpp::Destination_port::Destination_port(): SmppParam(DESTINATION_PORT),
					    value(0){}
smpp::Destination_port::~Destination_port(){}

int smpp::Destination_port::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = (data[0] << 8) | data[1];
    // ok
    return 0;
}

int smpp::Destination_port::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = value >> 8;
    param_data[1] = value;
    length = 2;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}


// Display_time
smpp::Display_time::Display_time(): SmppParam(DISPLAY_TIME),
				    dt_type(DT_TEMPORARY){}
smpp::Display_time::~Display_time(){}

int smpp::Display_time::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    dt_type = (DisplayTimeType)*data;
    // ok
    return 0;
}

int smpp::Display_time::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = dt_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Dpf_result
smpp::Dpf_result::Dpf_result(): SmppParam(DPF_RESULT),
			        dpf_set(false){}
smpp::Dpf_result::~Dpf_result(){}

int smpp::Dpf_result::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    dpf_set = *data == 1;
    // ok
    return 0;
}

int smpp::Dpf_result::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = dpf_set;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Its_reply_type
smpp::Its_reply_type::Its_reply_type(): SmppParam(ITS_REPLY_TYPE),
					its_rc_type(IRC_DIGIT){}
smpp::Its_reply_type::~Its_reply_type(){}

int smpp::Its_reply_type::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    its_rc_type = (ItsReplyCodeType)*data;
    // ok
    return 0;
}

int smpp::Its_reply_type::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = its_rc_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Its_session_info
smpp::Its_session_info::Its_session_info(): SmppParam(ITS_SESSION_INFO),
					    session_number(0),
					    sequence_number(0),
					    end_of_session(false){}
smpp::Its_session_info::~Its_session_info(){}

int smpp::Its_session_info::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    session_number = data[0];
    sequence_number = (data[1] & 0xfe) >> 1;
    end_of_session = (data[1] & 0x01) == 1;
    // ok
    return 0;
}

int smpp::Its_session_info::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = session_number;
    param_data[1] = (sequence_number << 1) | end_of_session;
    length = 2;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Language_indicator
smpp::Language_indicator::Language_indicator(): SmppParam(LANGUAGE_INDICATOR),
						li_type(LI_UNSPECIFIED){}
smpp::Language_indicator::~Language_indicator(){}

int smpp::Language_indicator::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    li_type = (LanguageIndicatorType)*data;
    // ok
    return 0;
}

int smpp::Language_indicator::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = li_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Message_payload
smpp::Message_payload::Message_payload(): SmppParam(MESSAGE_PAYLOAD){}
smpp::Message_payload::~Message_payload(){}

// Message_state
smpp::Message_state::Message_state(): SmppParam(MESSAGE_STATE),
				      message_state(MST_ENROUTE){}
smpp::Message_state::~Message_state(){}

int smpp::Message_state::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    message_state = (MessageStateType)*data;
    // ok
    return 0;
}

int smpp::Message_state::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = message_state;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}


// More_messages_to_send
smpp::More_messages_to_send::More_messages_to_send(): SmppParam(MORE_MESSAGES_TO_SEND){}
smpp::More_messages_to_send::~More_messages_to_send(){}

// Ms_availability_status
smpp::Ms_availability_status::Ms_availability_status(): SmppParam(MS_AVAILABILITY_STATUS),
							mas_type(MAS_AVAILABLE){}
smpp::Ms_availability_status::~Ms_availability_status(){}

int smpp::Ms_availability_status::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    mas_type = (MsAvailabilityStatusType)*data;
    // ok
    return 0;
}

int smpp::Ms_availability_status::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = mas_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Ms_msg_wait_facilities
smpp::Ms_msg_wait_facilities::Ms_msg_wait_facilities(): SmppParam(MS_MSG_WAIT_FACILITIES),
							mmwf_type(MMWF_VOICEMAIL_MESSAGE_WAITING),
							indication(false){}
smpp::Ms_msg_wait_facilities::~Ms_msg_wait_facilities(){}

int smpp::Ms_msg_wait_facilities::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    indication = (*data & 0x80) == 0x80;
    mmwf_type = (MsMsgWaitFacilitiesType)(*data & 0x03);
    // ok
    return 0;
}

int smpp::Ms_msg_wait_facilities::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = mmwf_type | (indication << 7);
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Ms_validity
smpp::Ms_validity::Ms_validity(): SmppParam(MS_VALIDITY),
				  mv_type(MV_STORE_INDEFINITELY){}
smpp::Ms_validity::~Ms_validity(){}

int smpp::Ms_validity::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    mv_type = (MsValidityType)*data;
    // ok
    return 0;
}

int smpp::Ms_validity::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = mv_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Network_error_code
smpp::Network_error_code::Network_error_code(): SmppParam(NETWORK_ERROR_CODE),
						nec_type(NEC_ANSI_136),
						error_code(ESME_ROK){}
smpp::Network_error_code::~Network_error_code(){}

int smpp::Network_error_code::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    nec_type = (NetworkErrorCodeType)*data;
    error_code = (SmppCommandStatus)((data[1] << 8) | data[2]);
    // ok
    return 0;
}

int smpp::Network_error_code::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = nec_type;
    param_data[1] = error_code >> 8;
    param_data[2] = error_code;
    length = 3;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}


// Number_of_messages
smpp::Number_of_messages::Number_of_messages(): SmppParam(NUMBER_OF_MESSAGES){}
smpp::Number_of_messages::~Number_of_messages(){}

// Payload_type
smpp::Payload_type::Payload_type(): SmppParam(PAYLOAD_TYPE),
				    pc_type(PC_DEFAULT){}
smpp::Payload_type::~Payload_type(){}

int smpp::Payload_type::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    pc_type = (PayloadCodeType)*data;
    // ok
    return 0;
}

int smpp::Payload_type::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = pc_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Privacy_indicator
smpp::Privacy_indicator::Privacy_indicator(): SmppParam(PRIVACY_INDICATOR),
					      pi_type(PI_PRIVACY_LEVEL_0){}
smpp::Privacy_indicator::~Privacy_indicator(){}

int smpp::Privacy_indicator::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    pi_type = (PrivacyIndicatorType)*data;
    // ok
    return 0;
}

int smpp::Privacy_indicator::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = pi_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Qos_time_to_live
smpp::Qos_time_to_live::Qos_time_to_live(): SmppParam(QOS_TIME_TO_LIVE),
					    value(0){}
smpp::Qos_time_to_live::~Qos_time_to_live(){}

int smpp::Qos_time_to_live::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    // ok
    return 0;
}

int smpp::Qos_time_to_live::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = value >> 24;
    param_data[1] = value >> 16;
    param_data[2] = value >> 8;
    param_data[3] = value;
    length = 4;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}


// Receipted_message_id
smpp::Receipted_message_id::Receipted_message_id(): SmppParam(RECEIPTED_MESSAGE_ID){}
smpp::Receipted_message_id::~Receipted_message_id(){}

// Sar_msg_ref_num
smpp::Sar_msg_ref_num::Sar_msg_ref_num(): SmppParam(SAR_MSG_REF_NUM),
					  value(0){}
smpp::Sar_msg_ref_num::~Sar_msg_ref_num(){}

int smpp::Sar_msg_ref_num::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = (data[0] << 8) | data[1];
    // ok
    return 0;
}

int smpp::Sar_msg_ref_num::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = value >> 8;
    param_data[1] = value;
    length = 2;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Sar_segment_seqnum
smpp::Sar_segment_seqnum::Sar_segment_seqnum(): SmppParam(SAR_SEGMENT_SEQNUM),
						value(0){}
smpp::Sar_segment_seqnum::~Sar_segment_seqnum(){}

int smpp::Sar_segment_seqnum::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = *data;
    // ok
    return 0;
}

int smpp::Sar_segment_seqnum::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = value;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}


// Sar_total_segments
smpp::Sar_total_segments::Sar_total_segments(): SmppParam(SAR_TOTAL_SEGMENTS),
						value(0){}
smpp::Sar_total_segments::~Sar_total_segments(){}

int smpp::Sar_total_segments::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = *data;
    // ok
    return 0;
}

int smpp::Sar_total_segments::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = value;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Sc_interface_version
smpp::Sc_interface_version::Sc_interface_version(): SmppParam(SC_INTERFACE_VERSION),
						    value(0){}
smpp::Sc_interface_version::~Sc_interface_version(){}

int smpp::Sc_interface_version::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = *data;
    // ok
    return 0;
}

int smpp::Sc_interface_version::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = value;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Set_dpf
smpp::Set_dpf::Set_dpf(): SmppParam(SET_DPF),
			  value(0){}
smpp::Set_dpf::~Set_dpf(){}

int smpp::Set_dpf::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = *data;
    // ok
    return 0;
}

int smpp::Set_dpf::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = value;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Sms_signal
smpp::Sms_signal::Sms_signal(): SmppParam(SMS_SIGNAL),
				value(0){}
smpp::Sms_signal::~Sms_signal(){}

int smpp::Sms_signal::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = (data[0] << 8) | data[1];
    // ok
    return 0;
}

int smpp::Sms_signal::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = value >> 8;
    param_data[1] = value;
    length = 2;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Source_addr_subunit
smpp::Source_addr_subunit::Source_addr_subunit(): SmppParam(SOURCE_ADDR_SUBUNIT),
						  sas_type(SAS_UNKNOWN){}
smpp::Source_addr_subunit::~Source_addr_subunit(){}

int smpp::Source_addr_subunit::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    sas_type = (SourceAddrSubunitType)*data;
    // ok
    return 0;
}

int smpp::Source_addr_subunit::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = sas_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Source_bearer_type
smpp::Source_bearer_type::Source_bearer_type(): SmppParam(SOURCE_BEARER_TYPE),
						sbc_type(SBC_UNKNOWN){}
smpp::Source_bearer_type::~Source_bearer_type(){}

int smpp::Source_bearer_type::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    sbc_type = (SourceBearerCodeType)*data;
    // ok
    return 0;
}

int smpp::Source_bearer_type::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = sbc_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Source_network_type
smpp::Source_network_type::Source_network_type(): SmppParam(SOURCE_NETWORK_TYPE),
						  snc_type(SN_UNKNOWN){}
smpp::Source_network_type::~Source_network_type(){}

int smpp::Source_network_type::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    snc_type = (SourceNetworkCodeType)*data;
    // ok
    return 0;
}

int smpp::Source_network_type::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = snc_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Source_port
smpp::Source_port::Source_port(): SmppParam(SOURCE_PORT),
				  value(0){}
smpp::Source_port::~Source_port(){}

int smpp::Source_port::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = (data[0] << 8) | data[1];
    // ok
    return 0;
}

int smpp::Source_port::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = value >> 8;
    param_data[1] = value;
    length = 2;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Source_subaddress
smpp::Source_subaddress::Source_subaddress(): SmppParam(SOURCE_SUBADDRESS),
					      ss_type(SS_NSAP_EVEN),
					      subaddress(NULL),
					      subaddress_length(0){}
smpp::Source_subaddress::~Source_subaddress(){}

int smpp::Source_subaddress::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    ss_type = (SourceSubaddressType)*data;
    ++data;
    subaddress = data;
    subaddress_length = data_length - 1;
    // ok
    return 0;
}

int smpp::Source_subaddress::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = ss_type;
    memcpy(&param_data[1], subaddress, subaddress_length);
    length = subaddress_length + 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// Source_telematics_id
smpp::Source_telematics_id::Source_telematics_id(): SmppParam(SOURCE_TELEMATICS_ID),
						    value(0){}
smpp::Source_telematics_id::~Source_telematics_id(){}

int smpp::Source_telematics_id::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = *data;
    // ok
    return 0;
}

int smpp::Source_telematics_id::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = value;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// User_message_reference
smpp::User_message_reference::User_message_reference(): SmppParam(USER_MESSAGE_REFERENCE),
							value(0){}
smpp::User_message_reference::~User_message_reference(){}

int smpp::User_message_reference::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = (data[0] << 8) | data[1];
    // ok
    return 0;
}

int smpp::User_message_reference::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    param_data[0] = value >> 8;
    param_data[1] = value;
    length = 2;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// User_response_code
smpp::User_response_code::User_response_code(): SmppParam(USER_RESPONSE_CODE),
						value(0){}
smpp::User_response_code::~User_response_code(){}

int smpp::User_response_code::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    value = *data;
    // ok
    return 0;
}

int smpp::User_response_code::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = value;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}


// Ussd_service_op
smpp::Ussd_service_op::Ussd_service_op(): SmppParam(USSD_SERVICE_OP),
					  uso_type(USO_PSSR_INDICATION){}
smpp::Ussd_service_op::~Ussd_service_op(){}

int smpp::Ussd_service_op::init(unsigned char* data, int data_length){
    SmppParam::init(data, data_length);
    uso_type = (UssdServiceOpType)*data;
    // ok
    return 0;
}

int smpp::Ussd_service_op::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // skip 4 bytes - 2 byte param tag, 2 byte param length
    param_data = &buffer[offset + 4];
    *param_data = uso_type;
    length = 1;
    // base encode method
    SmppParam::encode(buffer, buffer_length, offset, result_length);
    // ok
    return 0;
}

// SmppCommand
smpp::SmppCommand::SmppCommand():
	type(GENERIC_NACK),
	byte_pos(0),
	length(0){}
smpp::SmppCommand::SmppCommand(SmppCommandId _type):
	type(_type),
	byte_pos(0),
	length(0){}

smpp::SmppCommand::~SmppCommand(){}

int smpp::SmppCommand::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // command specific
    return 0;
}

int smpp::SmppCommand::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    *result_length = 0;
    // command specific
    return 0;

}

int smpp::SmppCommand::encode_optional(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    int param_length = 0;
    SmppParam* tmp_param = NULL;
    int tmp_pl = 0;
    for(unsigned int i = 0; i<params.size(); i++){
	tmp_param = params[i];
	if(tmp_param != NULL){
	    tmp_pl = encode_parameter(tmp_param, buffer, buffer_length, offset);
	    param_length += tmp_pl;
	    offset += tmp_pl;

	}
    }
    *result_length += param_length;
    return 0;
}


int smpp::SmppCommand::get_param_count(){
    return params.size();
}

void smpp::SmppCommand::clear_params(){
    params.clear();
}

smpp::SmppParam* smpp::SmppCommand::get_parameter(SmppParameterType _param_type){
    SmppParam* tmp_param = NULL;
    for(unsigned int i = 0; i<params.size(); i++){
	    tmp_param = params[i];
	    if(tmp_param != NULL){
		    if(tmp_param->type == _param_type) return tmp_param;
	    }
    }
    return NULL;

}

smpp::SmppParam* smpp::SmppCommand::add_parameter(SmppParam* param){
     if(param != NULL) params.push_back(param);
     return param;

}

int smpp::SmppCommand::remove_parameter(SmppParameterType _param_type){
    SmppParam* tmp_param = NULL;
    for(unsigned int i = 0; i<params.size(); i++){
	tmp_param = params[i];
	if(tmp_param != NULL){
	    if(tmp_param->type == _param_type){
		params.erase(params.begin() + i);
		return 0;
	    }
	}
    }

    return 1;
}


void smpp::SmppCommand::process_params(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    if(data != NULL && data_length > 0){
	SmppParameterType pt;
	SmppParam* smpp_param = NULL;
	unsigned int tag;
	unsigned int l;
	while(byte_pos < data_length){
	    tag = (data[byte_pos] << 8) + (data[byte_pos + 1] & 0xFF);
	    byte_pos += 2;
	    // length(l) = two bytes for tag + two bytes for length + length of actual data
	    l = (data[byte_pos] << 8) + (data[byte_pos + 1] & 0xFF);
	    byte_pos += 2;
	    pt = (SmppParameterType)tag;
	    // create param
	    smpp_param = _smpp_pool->request_param(pt);
	    if(smpp_param != NULL){
		smpp_param->byte_pos = 0;
		smpp_param->init(&data[byte_pos], l);
		params.push_back(smpp_param);
	    }
	    // next
	    byte_pos += l;

	}
    }

}

int smpp::SmppCommand::encode_parameter(SmppParam* param, unsigned char* buffer, int buffer_length, int offset){
    if(buffer != NULL && param != NULL && buffer_length > 0){
	int param_res_length = 0;
	param->encode(buffer, buffer_length, offset, &param_res_length);
	return param_res_length;

    }
    return 0;

}

// SmppPDU
smpp::SmppPDU::SmppPDU(): command_length(0),
			  command_id(GENERIC_NACK),
			  command_status(ESME_ROK),
			  sequence_number(0),
			  command(NULL){}

smpp::SmppPDU::~SmppPDU(){}

int smpp::SmppPDU::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // sanity check
    if(buffer_length < 16) return 1;
    // offset
    buffer += offset;
    // common header
    uint32_t total_length = 16;
    // length calculated lated
    // save position for length values which are calculated later
    unsigned char* tmp_pos = buffer;
    buffer += 4;
    // command id
    buffer[0] = command_id >> 24;
    buffer[1] = command_id >> 16;
    buffer[2] = command_id >> 8;
    buffer[3] = command_id;
    buffer += 4;
    // status
    buffer[0] = command_status >> 24;
    buffer[1] = command_status >> 16;
    buffer[2] = command_status >> 8;
    buffer[3] = command_status;
    buffer += 4;
    // sequence
    buffer[0] = sequence_number >> 24;
    buffer[1] = sequence_number >> 16;
    buffer[2] = sequence_number >> 8;
    buffer[3] = sequence_number;
    buffer += 4;

    // command
    if(command != NULL){
	if(command->encode(buffer, buffer_length - 16, 0, result_length) != 0) return 2;
	total_length += *result_length;

    }
    // restore position
    buffer = tmp_pos;
    buffer[0] = total_length >> 24;
    buffer[1] = total_length >> 16;
    buffer[2] = total_length >> 8;
    buffer[3] = total_length;
    // result length
    *result_length = total_length;
    // ok
    return 0;


}
// SMPPCommandPoolItem
smpp::SMPPCommandPoolItem::SMPPCommandPoolItem(){
    pool = NULL;
    next_free_item = NULL;
    total_count = 0;
    free_count = 0;
    type = GENERIC_NACK;

}

smpp::SMPPCommandPoolItem::~SMPPCommandPoolItem(){
    for(int i = 0; i<total_count; i++) delete pool[i];
    delete[] pool;

}

smpp::SmppCommand* smpp::SMPPCommandPoolItem::create_command(SmppCommandId _command_type){
    SmppCommand* smppc = NULL;
    switch(_command_type){
	case GENERIC_NACK: smppc = new Generic_nack(); break;
	case BIND_RECEIVER: smppc = new Bind_receiver(); break;
	case BIND_RECEIVER_RESP: smppc = new Bind_receiver_resp(); break;
	case BIND_TRANSMITTER: smppc = new Bind_transmitter(); break;
	case BIND_TRANSMITTER_RESP: smppc = new Bind_transmitter_resp(); break;
	case QUERY_SM: smppc = new Query_sm(); break;
	case QUERY_SM_RESP: smppc = new Query_sm_resp(); break;
	case SUBMIT_SM: smppc = new Submit_sm(); break;
	case SUBMIT_SM_RESP: smppc = new Submit_sm_resp(); break;
	case DELIVER_SM: smppc = new Deliver_sm(); break;
	case DELIVER_SM_RESP: smppc = new Deliver_sm_resp(); break;
	case UNBIND: smppc = new Unbind(); break;
	case UNBIND_RESP: smppc = new Unbind_resp(); break;
	case REPLACE_SM: smppc = new Replace_sm(); break;
	case REPLACE_SM_RESP: smppc = new Replace_sm_resp(); break;
	case CANCEL_SM: smppc = new Cancel_sm(); break;
	case CANCEL_SM_RESP: smppc = new Cancel_sm_resp(); break;
	case BIND_TRANSCEIVER: smppc = new Bind_transceiver(); break;
	case BIND_TRANSCEIVER_RESP: smppc = new Bind_transceiver_resp(); break;
	case OUTBIND: smppc = new Outbind(); break;
	case ENQUIRE_LINK: smppc = new Enquire_link(); break;
	case ENQUIRE_LINK_RESP: smppc = new Enquire_link_resp(); break;
	case SUBMIT_MULTI: smppc = new Submit_multi(); break;
	case SUBMIT_MULTI_RESP: smppc = new Submit_multi_resp(); break;
	case ALERT_NOTIFICATION: smppc = new Alert_notification(); break;
	case DATA_SM: smppc = new Data_sm(); break;
	case DATA_SM_RESP: smppc = new Data_sm_resp(); break;
	default: break;

    }

    return smppc;
}

void smpp::SMPPCommandPoolItem::init_pool(){
	pool = new SmppCommand *[total_count];
	for(int i = 0; i<total_count; i++) pool[i] = create_command(type);
	next_free_item = pool[total_count - 1];
}

void smpp::SMPPCommandPoolItem::set_pool_size(int _total_count){
	total_count = _total_count;
	free_count = total_count;
}

smpp::SmppCommand* smpp::SMPPCommandPoolItem::request_item(){
	SmppCommand* tmp = next_free_item;
	--free_count;
	if(free_count <= 0) free_count = total_count;
	next_free_item = pool[free_count - 1];
	return tmp;
}


// SMPPParamPoolItem
smpp::SMPPParamPoolItem::SMPPParamPoolItem(){
    pool = NULL;
    next_free_item = NULL;
    total_count = 0;
    free_count = 0;
    type = SMPP_PARAM_UNKNOWN;

}

smpp::SMPPParamPoolItem::~SMPPParamPoolItem(){
    for(int i = 0; i<total_count; i++) delete pool[i];
    delete[] pool;

}


smpp::SmppParam* smpp::SMPPParamPoolItem::create_param(SmppParameterType _param_type){
    SmppParam* smppp = NULL;
    switch(_param_type){
	case SMPP_PARAM_UNKNOWN: break;
	case DEST_ADDR_SUBUNIT: smppp = new Dest_addr_subunit(); break;
	case DEST_NETWORK_TYPE: smppp = new Dest_network_type(); break;
	case DEST_BEARER_TYPE: smppp = new Dest_bearer_type(); break;
	case DEST_TELEMATICS_ID: smppp = new Dest_telematics_id(); break;
	case SOURCE_ADDR_SUBUNIT: smppp = new Source_addr_subunit(); break;
	case SOURCE_NETWORK_TYPE: smppp = new Source_network_type(); break;
	case SOURCE_BEARER_TYPE: smppp = new Source_bearer_type(); break;
	case SOURCE_TELEMATICS_ID: smppp = new Source_telematics_id(); break;
	case QOS_TIME_TO_LIVE: smppp = new Qos_time_to_live(); break;
	case PAYLOAD_TYPE: smppp = new Payload_type(); break;
	case ADDITIONAL_STATUS_INFO_TEXT: smppp = new Additional_status_info_text(); break;
	case RECEIPTED_MESSAGE_ID: smppp = new Receipted_message_id(); break;
	case MS_MSG_WAIT_FACILITIES: smppp = new Ms_msg_wait_facilities(); break;
	case PRIVACY_INDICATOR: smppp = new Privacy_indicator(); break;
	case SOURCE_SUBADDRESS: smppp = new Source_subaddress(); break;
	case DEST_SUBADDRESS: smppp = new Dest_subaddress(); break;
	case USER_MESSAGE_REFERENCE: smppp = new User_message_reference(); break;
	case USER_RESPONSE_CODE: smppp = new User_response_code(); break;
	case SOURCE_PORT: smppp = new Source_port(); break;
	case DESTINATION_PORT: smppp = new Destination_port(); break;
	case SAR_MSG_REF_NUM: smppp = new Sar_msg_ref_num(); break;
	case LANGUAGE_INDICATOR: smppp = new Language_indicator(); break;
	case SAR_TOTAL_SEGMENTS: smppp = new Sar_total_segments(); break;
	case SAR_SEGMENT_SEQNUM: smppp = new Sar_segment_seqnum(); break;
	case SC_INTERFACE_VERSION: smppp = new Sc_interface_version(); break;
	case CALLBACK_NUM_PRES_IND: smppp = new Callback_num_pres_ind(); break;
	case CALLBACK_NUM_ATAG: smppp = new Callback_num_atag(); break;
	case NUMBER_OF_MESSAGES: smppp = new Number_of_messages(); break;
	case CALLBACK_NUM: smppp = new Callback_num(); break;
	case DPF_RESULT: smppp = new Dpf_result(); break;
	case SET_DPF: smppp = new Set_dpf(); break;
	case MS_AVAILABILITY_STATUS: smppp = new Ms_availability_status(); break;
	case NETWORK_ERROR_CODE: smppp = new Network_error_code(); break;
	case MESSAGE_PAYLOAD: smppp = new Message_payload(); break;
	case DELIVERY_FAILURE_REASON: smppp = new Delivery_failure_reason(); break;
	case MORE_MESSAGES_TO_SEND: smppp = new More_messages_to_send(); break;
	case MESSAGE_STATE: smppp = new Message_state(); break;
	case USSD_SERVICE_OP: smppp = new Ussd_service_op(); break;
	case DISPLAY_TIME: smppp = new Display_time(); break;
	case SMS_SIGNAL: smppp = new Sms_signal(); break;
	case MS_VALIDITY: smppp = new Ms_validity(); break;
	case ALERT_ON_MESSAGE_DELIVERY: smppp = new Alert_on_message_delivery(); break;
	case ITS_REPLY_TYPE: smppp = new Its_reply_type(); break;
	case ITS_SESSION_INFO: smppp = new Its_session_info(); break;
	default: break;

    }

    return smppp;
}


void smpp::SMPPParamPoolItem::init_pool(){
	pool = new SmppParam *[total_count];
	for(int i = 0; i<total_count; i++) pool[i] = create_param(type);
	next_free_item = pool[total_count - 1];
}

void smpp::SMPPParamPoolItem::set_pool_size(int _total_count){
	total_count = _total_count;
	free_count = total_count;
}

smpp::SmppParam* smpp::SMPPParamPoolItem::request_item(){
	SmppParam* tmp = next_free_item;
	--free_count;
	if(free_count <= 0) free_count = total_count;
	next_free_item = pool[free_count - 1];
	return tmp;
}


// SMPPPool
smpp::SMPPPool::SMPPPool(): param_count(5),
			    command_count(1) {}

smpp::SMPPPool::~SMPPPool(){
    SMPPCommandPoolItem* tmp_cmd = NULL;
    std::map<SmppCommandId, SMPPCommandPoolItem*>::iterator it = COMMAND_POOL.begin();
    for(; it != COMMAND_POOL.end(); it++){
	tmp_cmd = it->second;
	delete tmp_cmd;
    }

    SMPPParamPoolItem* tmp_param = NULL;
    std::map<SmppParameterType, SMPPParamPoolItem*>::iterator it2 = PARAM_POOL.begin();
    for(; it2 != PARAM_POOL.end(); it2++){
	tmp_param = it2->second;
	delete tmp_param;

    }
}

void smpp::SMPPPool::set_pool_size(int _param_count, int _command_count){
	param_count = _param_count;
	command_count = _command_count;
}
void smpp::SMPPPool::init_command(SmppCommandId _command_type){
    COMMAND_POOL[_command_type] = new SMPPCommandPoolItem();
    COMMAND_POOL[_command_type]->type = _command_type;
    COMMAND_POOL[_command_type]->set_pool_size(command_count);
    COMMAND_POOL[_command_type]->init_pool();

}

void smpp::SMPPPool::init_param(SmppParameterType _param_type){
    PARAM_POOL[_param_type] = new SMPPParamPoolItem();
    PARAM_POOL[_param_type]->type = _param_type;
    PARAM_POOL[_param_type]->set_pool_size(param_count);
    PARAM_POOL[_param_type]->init_pool();
}

smpp::SmppCommand* smpp::SMPPPool::request_command(SmppCommandId command_type){
    std::map<SmppCommandId, SMPPCommandPoolItem*>::iterator it = COMMAND_POOL.find(command_type);
    if(it != COMMAND_POOL.end()) return it->second->request_item();
    return NULL;
}


smpp::SmppParam* smpp::SMPPPool::request_param(SmppParameterType param_type){
    std::map<SmppParameterType, SMPPParamPoolItem*>::iterator it = PARAM_POOL.find(param_type);
    if(it != PARAM_POOL.end()) return it->second->request_item();
    return NULL;
}

void smpp::SMPPPool::init_pool(){
    // command pool
    init_command(GENERIC_NACK);
    init_command(BIND_RECEIVER);
    init_command(BIND_RECEIVER_RESP);
    init_command(BIND_TRANSMITTER);
    init_command(BIND_TRANSMITTER_RESP);
    init_command(QUERY_SM);
    init_command(QUERY_SM_RESP);
    init_command(SUBMIT_SM);
    init_command(SUBMIT_SM_RESP);
    init_command(DELIVER_SM);
    init_command(DELIVER_SM_RESP);
    init_command(UNBIND);
    init_command(UNBIND_RESP);
    init_command(REPLACE_SM);
    init_command(REPLACE_SM_RESP);
    init_command(CANCEL_SM);
    init_command(CANCEL_SM_RESP);
    init_command(BIND_TRANSCEIVER);
    init_command(BIND_TRANSCEIVER_RESP);
    init_command(OUTBIND);
    init_command(ENQUIRE_LINK);
    init_command(ENQUIRE_LINK_RESP);
    init_command(SUBMIT_MULTI);
    init_command(SUBMIT_MULTI_RESP);
    init_command(ALERT_NOTIFICATION);
    init_command(DATA_SM);
    init_command(DATA_SM_RESP);

    // param pool
    init_param(DEST_ADDR_SUBUNIT);
    init_param(DEST_NETWORK_TYPE);
    init_param(DEST_BEARER_TYPE);
    init_param(DEST_TELEMATICS_ID);
    init_param(SOURCE_ADDR_SUBUNIT);
    init_param(SOURCE_NETWORK_TYPE);
    init_param(SOURCE_BEARER_TYPE);
    init_param(SOURCE_TELEMATICS_ID);
    init_param(QOS_TIME_TO_LIVE);
    init_param(PAYLOAD_TYPE);
    init_param(ADDITIONAL_STATUS_INFO_TEXT);
    init_param(RECEIPTED_MESSAGE_ID);
    init_param(MS_MSG_WAIT_FACILITIES);
    init_param(PRIVACY_INDICATOR);
    init_param(SOURCE_SUBADDRESS);
    init_param(DEST_SUBADDRESS);
    init_param(USER_MESSAGE_REFERENCE);
    init_param(USER_RESPONSE_CODE);
    init_param(SOURCE_PORT);
    init_param(DESTINATION_PORT);
    init_param(SAR_MSG_REF_NUM);
    init_param(LANGUAGE_INDICATOR);
    init_param(SAR_TOTAL_SEGMENTS);
    init_param(SAR_SEGMENT_SEQNUM);
    init_param(SC_INTERFACE_VERSION);
    init_param(CALLBACK_NUM_PRES_IND);
    init_param(CALLBACK_NUM_ATAG);
    init_param(NUMBER_OF_MESSAGES);
    init_param(CALLBACK_NUM);
    init_param(DPF_RESULT);
    init_param(SET_DPF);
    init_param(MS_AVAILABILITY_STATUS);
    init_param(NETWORK_ERROR_CODE);
    init_param(MESSAGE_PAYLOAD);
    init_param(DELIVERY_FAILURE_REASON);
    init_param(MORE_MESSAGES_TO_SEND);
    init_param(MESSAGE_STATE);
    init_param(USSD_SERVICE_OP);
    init_param(DISPLAY_TIME);
    init_param(SMS_SIGNAL);
    init_param(MS_VALIDITY);
    init_param(ALERT_ON_MESSAGE_DELIVERY);
    init_param(ITS_REPLY_TYPE);
    init_param(ITS_SESSION_INFO);
}



// Bind_transmitter
smpp::Bind_transmitter::Bind_transmitter():
	SmppCommand(BIND_TRANSMITTER),
	system_id(NULL),
	password(NULL),
	system_type(NULL),
	interface_version(0),
	addr_ton(TON_UNKNOWN),
	addr_npi(UNKNOWN),
	address_range(NULL){}
smpp::Bind_transmitter::~Bind_transmitter(){}

int smpp::Bind_transmitter::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 7 || _smpp_pool == NULL) return 1;
    // system_id
    system_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // password
    password = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // system_type
    system_type = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // interface_version
    interface_version = *data;
    ++data;
    --data_length;
    // addr_ton
    addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // addr_npi
    addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // address_range
    address_range = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // ok
    return 0;
}

int smpp::Bind_transmitter::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 7 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // system_id
    strcpy((char*)buffer, system_id);
    int tmpl = strlen(system_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // password
    strcpy((char*)buffer, password);
    tmpl = strlen(password) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // system_type
    strcpy((char*)buffer, system_type);
    tmpl = strlen(system_type) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // interface_version
    *buffer = interface_version;
    ++buffer;
    ++*result_length;
    // addr_ton
    *buffer = addr_ton;
    ++buffer;
    ++*result_length;
    // addr_npi
    *buffer = addr_npi;
    ++buffer;
    ++*result_length;
    // address_range
    strcpy((char*)buffer, address_range);
    tmpl = strlen(address_range) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // ok
    return 0;
}

// Bind_transmitter_resp
smpp::Bind_transmitter_resp::Bind_transmitter_resp():
	SmppCommand(BIND_TRANSMITTER_RESP),
	system_id(NULL){}
smpp::Bind_transmitter_resp::~Bind_transmitter_resp(){}

int smpp::Bind_transmitter_resp::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // special check (body can be absent in case of command_status != 0)
    if(data_length <= 0) {
	system_id = (char*)SMPP_NULL_STR;
	return 0;
    }
    // sanity check
    if(data == NULL || data_length < 1 || _smpp_pool == NULL) return 1;
    // system_id
    system_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // optional params
    process_params(data, data_length, _smpp_pool);
    // ok
    return 0;
}

int smpp::Bind_transmitter_resp::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 1 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // system_id
    strcpy((char*)buffer, system_id);
    int tmpl = strlen(system_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // optional params
    encode_optional(buffer, buffer_length - *result_length, 0, result_length);
    // ok
    return 0;
}

// Bind_receiver
smpp::Bind_receiver::Bind_receiver():
	SmppCommand(BIND_RECEIVER),
	system_id(NULL),
	password(NULL),
	system_type(NULL),
	interface_version(0),
	addr_ton(TON_UNKNOWN),
	addr_npi(UNKNOWN),
	address_range(NULL){}
smpp::Bind_receiver::~Bind_receiver(){}

int smpp::Bind_receiver::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 7 || _smpp_pool == NULL) return 1;
    // system_id
    system_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // password
    password = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // system_type
    system_type = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // interface_version
    interface_version = *data;
    ++data;
    --data_length;
    // addr_ton
    addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // addr_npi
    addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // address_range
    address_range = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // ok
    return 0;
}

int smpp::Bind_receiver::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 7 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // system_id
    strcpy((char*)buffer, system_id);
    int tmpl = strlen(system_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // password
    strcpy((char*)buffer, password);
    tmpl = strlen(password) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // system_type
    strcpy((char*)buffer, system_type);
    tmpl = strlen(system_type) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // interface_version
    *buffer = interface_version;
    ++buffer;
    ++*result_length;
    // addr_ton
    *buffer = addr_ton;
    ++buffer;
    ++*result_length;
    // addr_npi
    *buffer = addr_npi;
    ++buffer;
    ++*result_length;
    // address_range
    strcpy((char*)buffer, address_range);
    tmpl = strlen(address_range) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // ok
    return 0;
}

// Bind_receiver_resp
smpp::Bind_receiver_resp::Bind_receiver_resp():
	SmppCommand(BIND_RECEIVER_RESP),
	system_id(NULL){}
smpp::Bind_receiver_resp::~Bind_receiver_resp(){}

int smpp::Bind_receiver_resp::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // special check (body can be absent in case of command_status != 0)
    if(data_length <= 0) {
	system_id = (char*)SMPP_NULL_STR;
	return 0;
    }
    // sanity check
    if(data == NULL || data_length < 1 || _smpp_pool == NULL) return 1;
    // system_id
    system_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // optional params
    process_params(data, data_length, _smpp_pool);
    // ok
    return 0;
}

int smpp::Bind_receiver_resp::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 1 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // system_id
    strcpy((char*)buffer, system_id);
    int tmpl = strlen(system_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // optional params
    encode_optional(buffer, buffer_length - *result_length, 0, result_length);
    // ok
    return 0;
}

// Bind_transceiver
smpp::Bind_transceiver::Bind_transceiver():
	SmppCommand(BIND_TRANSCEIVER),
	system_id(NULL),
	password(NULL),
	system_type(NULL),
	interface_version(0),
	addr_ton(TON_UNKNOWN),
	addr_npi(UNKNOWN),
	address_range(NULL){}
smpp::Bind_transceiver::~Bind_transceiver(){}

int smpp::Bind_transceiver::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 7 || _smpp_pool == NULL) return 1;
    // system_id
    system_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // password
    password = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // system_type
    system_type = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // interface_version
    interface_version = *data;
    ++data;
    --data_length;
    // addr_ton
    addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // addr_npi
    addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // address_range
    address_range = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // ok
    return 0;
}

int smpp::Bind_transceiver::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 7 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // system_id
    strcpy((char*)buffer, system_id);
    int tmpl = strlen(system_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // password
    strcpy((char*)buffer, password);
    tmpl = strlen(password) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // system_type
    strcpy((char*)buffer, system_type);
    tmpl = strlen(system_type) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // interface_version
    *buffer = interface_version;
    ++buffer;
    ++*result_length;
    // addr_ton
    *buffer = addr_ton;
    ++buffer;
    ++*result_length;
    // addr_npi
    *buffer = addr_npi;
    ++buffer;
    ++*result_length;
    // address_range
    strcpy((char*)buffer, address_range);
    tmpl = strlen(address_range) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // ok
    return 0;
}

// Bind_transceiver_resp
smpp::Bind_transceiver_resp::Bind_transceiver_resp():
	SmppCommand(BIND_TRANSCEIVER_RESP),
	system_id(NULL){}
smpp::Bind_transceiver_resp::~Bind_transceiver_resp(){}

int smpp::Bind_transceiver_resp::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // special check (body can be absent in case of command_status != 0)
    if(data_length <= 0) {
	system_id = (char*)SMPP_NULL_STR;
	return 0;
    }
    // sanity check
    if(data == NULL || data_length < 1 || _smpp_pool == NULL) return 1;
    // system_id
    system_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // optional params
    process_params(data, data_length, _smpp_pool);
    // ok
    return 0;
}

int smpp::Bind_transceiver_resp::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 1 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // system_id
    strcpy((char*)buffer, system_id);
    int tmpl = strlen(system_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // optional params
    encode_optional(buffer, buffer_length - *result_length, 0, result_length);
    // ok
    return 0;
}

// Outbind
smpp::Outbind::Outbind():
	SmppCommand(OUTBIND),
	system_id(NULL),
	password(NULL){}
smpp::Outbind::~Outbind(){}

int smpp::Outbind::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 2 || _smpp_pool == NULL) return 1;
    // system_id
    system_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // password
    password = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // ok
    return 0;
}

int smpp::Outbind::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 2 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // system_id
    strcpy((char*)buffer, system_id);
    int tmpl = strlen(system_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // password
    strcpy((char*)buffer, password);
    tmpl = strlen(password) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // ok
    return 0;
}

// Unbind
smpp::Unbind::Unbind(): SmppCommand(UNBIND){}
smpp::Unbind::~Unbind(){}


// Unbind_resp
smpp::Unbind_resp::Unbind_resp(): SmppCommand(UNBIND_RESP){}
smpp::Unbind_resp::~Unbind_resp(){}


// Generic_nack
smpp::Generic_nack::Generic_nack(): SmppCommand(GENERIC_NACK){}
smpp::Generic_nack::~Generic_nack(){}


// Submit_sm
smpp::Submit_sm::Submit_sm():
	SmppCommand(SUBMIT_SM),
	service_type(NULL),
	source_addr_ton(TON_UNKNOWN),
	source_addr_npi(UNKNOWN),
	source_addr(NULL),
	dest_addr_ton(TON_UNKNOWN),
	dest_addr_npi(UNKNOWN),
	destination_addr(NULL),
	esm_class_mm(MM_DEFAULT_SMSC_MODE),
	esm_class_mt(MT_DEFAULT),
	esm_gsm_specific(GNS_NO_SPECIFIC_FEATURES),
	protocol_id(0),
	priority_flag(0),
	schedule_delivery_time(NULL),
	validity_period(NULL),
	rd_smsc_dlvr_rcpt(SDR_NO_SMSC_DELIVERY),
	rd_sme_orig_ack(SOA_NO_SME_ACK),
	rd_intrmd_ntf(INT_NO),
	replace_if_present_flag(0),
	data_coding(DC_DEFAULT),
	sm_default_msg_id(0),
	sm_length(0),
	short_message(NULL){}


smpp::Submit_sm::~Submit_sm(){}

int smpp::Submit_sm::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 17 || _smpp_pool == NULL) return 1;
    // service_type
    service_type = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // source_addr_ton
    source_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // source_addr_npi
    source_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // source_addr
    source_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // dest_addr_ton
    dest_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // dest_addr_npi
    dest_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // destination_addr
    destination_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // esm_class
    esm_class_mm = (MessageMode)(*data & 0x03);
    esm_class_mt = (MessageType)(*data & 0x3c);
    esm_gsm_specific = (GSMNetworkSpecific)(*data & 0xc0);
    ++data;
    --data_length;
    // protocol_id
    protocol_id = *data;
    ++data;
    --data_length;
    // priority_flag
    priority_flag = *data;
    ++data;
    --data_length;
    // schedule_delivery_time
    schedule_delivery_time = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // validity_period
    validity_period = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // registered_delivery
    rd_smsc_dlvr_rcpt = (SMSCDeliveryReceiptType)(*data & 0x03);
    rd_sme_orig_ack = (SMEOrigAckType)(*data & 0x0c);
    rd_intrmd_ntf = (IntermediateNotificationType)(*data & 0x10);
    ++data;
    --data_length;
    // replace_if_present_flag
    replace_if_present_flag = *data;
    ++data;
    --data_length;
    // data_coding
    data_coding = (Data_codingType)(*data);
    ++data;
    --data_length;
    // sm_default_msg_id
    sm_default_msg_id = *data;
    ++data;
    --data_length;
    // sm_length
    sm_length = *data;
    ++data;
    --data_length;
    // short_message
    short_message = data;
    data += sm_length;
    data_length -= sm_length;
    // optional params
    process_params(data, data_length, _smpp_pool);
    // ok
    return 0;
}

int smpp::Submit_sm::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 17 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // service_type
    strcpy((char*)buffer, service_type);
    int tmpl = strlen(service_type) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // source_addr_ton
    *buffer = source_addr_ton;
    ++buffer;
    ++*result_length;
    // source_addr_npi
    *buffer = source_addr_npi;
    ++buffer;
    ++*result_length;
    // source_addr
    strcpy((char*)buffer, source_addr);
    tmpl = strlen(source_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // dest_addr_ton
    *buffer = dest_addr_ton;
    ++buffer;
    ++*result_length;
    // dest_addr_npi
    *buffer = dest_addr_npi;
    ++buffer;
    ++*result_length;
    // destination_addr
    strcpy((char*)buffer, destination_addr);
    tmpl = strlen(destination_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // esm_class
    *buffer = esm_class_mm | esm_class_mt | esm_gsm_specific;
    ++buffer;
    ++*result_length;
    // protocol_id
    *buffer = protocol_id;
    ++buffer;
    ++*result_length;
    // priority_flag
    *buffer = priority_flag;
    ++buffer;
    ++*result_length;
    // schedule_delivery_time
    strcpy((char*)buffer, schedule_delivery_time);
    tmpl = strlen(schedule_delivery_time) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // validity_period
    strcpy((char*)buffer, validity_period);
    tmpl = strlen(validity_period) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // registered_delivery
    *buffer = rd_smsc_dlvr_rcpt | rd_sme_orig_ack | rd_intrmd_ntf;
    ++buffer;
    ++*result_length;
    // replace_if_present_flag
    *buffer = replace_if_present_flag;
    ++buffer;
    ++*result_length;
    // data_coding
    *buffer = data_coding;
    ++buffer;
    ++*result_length;
    // sm_default_msg_id
    *buffer = sm_default_msg_id;
    ++buffer;
    ++*result_length;
    // sm_length
    *buffer = sm_length;
    ++buffer;
    ++*result_length;
    // short_message
    memcpy(buffer, short_message, sm_length);
    buffer += sm_length;
    *result_length += sm_length;
    // optional params
    encode_optional(buffer, buffer_length - *result_length, 0, result_length);
    // ok
    return 0;
}

// Submit_sm_resp
smpp::Submit_sm_resp::Submit_sm_resp():
	SmppCommand(SUBMIT_SM_RESP),
	message_id(NULL){}
smpp::Submit_sm_resp::~Submit_sm_resp(){}

int smpp::Submit_sm_resp::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // special check (body can be absent in case of command_status != 0)
    if(data_length <= 0) {
	message_id = (char*)SMPP_NULL_STR;
	return 0;
    }
    // sanity check
    if(data == NULL || data_length < 1 || _smpp_pool == NULL) return 1;
    // message_id
    message_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // ok
    return 0;
}

int smpp::Submit_sm_resp::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 1 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // message_id
    strcpy((char*)buffer, message_id);
    int tmpl = strlen(message_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // ok
    return 0;
}



// C str length
int smpp::Cstr_len(unsigned char* data, int data_length){
    if(data == NULL || data_length <=0) return -1;
    for(int i = 0; i<data_length; i++) if(data[i] == 0) return i + 1;
    return -1;
}

size_t ascii_2_num(const char *buf, size_t len){
    int n = 0;
    while(len--) n = n * 10 + *buf++ - '0';
    return n;
}

time_t smpp::decode_time_format(char* data){
    // err check
    if(data == NULL) return 0;
    if(strlen(data) < 16) return 0;
    // get local time
    time_t now = time(NULL);
    tm tm_new, tmnow;
    localtime_r(&now, &tmnow);
    tm_new.tm_year = (2000 + ascii_2_num(&data[0], 2)) - 1900;
    tm_new.tm_mon = ascii_2_num(&data[2], 2) - 1;
    tm_new.tm_mday = ascii_2_num(&data[4], 2);
    tm_new.tm_hour = ascii_2_num(&data[6], 2);
    tm_new.tm_min = ascii_2_num(&data[8], 2);
    tm_new.tm_sec = ascii_2_num(&data[10], 2);
    tm_new.tm_isdst = tmnow.tm_isdst;
    // ignore t (tenth of second)
    // time difference in quarter hours converted to min
    int tdiff = (ascii_2_num(&data[13], 2) * 15);
    // time diff direction
    switch(data[15]){
	// time advanced
	case '+':
	    tm_new.tm_min += tdiff;
	    break;

	// time retarded
	case '-':
	    tm_new.tm_min -= tdiff;
	    break;

	// time relative
	case 'R':
	    tm_new.tm_year += ((1900 + tmnow.tm_year) - 2000);
	    tm_new.tm_mon += tmnow.tm_mon;
	    tm_new.tm_mday += tmnow.tm_mday;
	    tm_new.tm_hour += tmnow.tm_hour;
	    tm_new.tm_min += tmnow.tm_min;
	    tm_new.tm_sec += tmnow.tm_sec;
	    break;
    }

    // get unix timestamp
    return mktime(&tm_new);

}



// Submit_multi
smpp::Submit_multi::Submit_multi():
	SmppCommand(SUBMIT_MULTI),
	service_type(NULL),
	source_addr_ton(TON_UNKNOWN),
	source_addr_npi(UNKNOWN),
	source_addr(NULL),
	number_of_dests(0),
	esm_class_mm(MM_DEFAULT_SMSC_MODE),
	esm_class_mt(MT_DEFAULT),
	esm_gsm_specific(GNS_NO_SPECIFIC_FEATURES),
	protocol_id(0),
	priority_flag(0),
	schedule_delivery_time(NULL),
	validity_period(NULL),
	rd_smsc_dlvr_rcpt(SDR_NO_SMSC_DELIVERY),
	rd_sme_orig_ack(SOA_NO_SME_ACK),
	rd_intrmd_ntf(INT_NO),
	replace_if_present_flag(0),
	data_coding(DC_DEFAULT),
	sm_default_msg_id(0),
	sm_length(0),
	short_message(NULL){}

smpp::Submit_multi::~Submit_multi(){}

int smpp::Submit_multi::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 17 || _smpp_pool == NULL) return 1;
    // service_type
    service_type = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // source_addr_ton
    source_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // source_addr_npi
    source_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // source_addr
    source_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // number_of_dests
    number_of_dests = *data;
    ++data;
    --data_length;
    // dest_address(es)
    int dest_flag;
    for(unsigned int i = 0; i<number_of_dests; i++){
	// get dest_flag
	dest_flag = *data;
	++data;
	--data_length;
	switch(dest_flag){
	    // SME address
	    case DA_SME_ADDRESS:
	    {
		AddressDefinition ad;
		// ton, npi, addr
		ad.dest_address.dest_addr_ton = (SmppTON)data[0];
		ad.dest_address.dest_addr_npi = (SmppNPI)data[1];
		ad.dest_address.destination_addr = (char*)&data[2];
		// skip ton and npi
		data += 2;
		data_length -=2;
		// skip dest_addr
		tmp_l = Cstr_len(data, data_length);
		if(tmp_l < 0) return 1;
		data += tmp_l;
		data_length -= tmp_l;
		// add to list
		dest_address.push_back(ad);
		break;

	    }
	    // Distribution list name
	    case DA_DISTRIBUTION_LIST_ADDRESS:
	    {
		AddressDefinition ad;
		ad.dl_address.dl_name = (char*)data;
		tmp_l = Cstr_len(data, data_length);
		if(tmp_l < 0) return 1;
		data += tmp_l;
		data_length -= tmp_l;
		// add to list
		dest_address.push_back(ad);
		break;

	    }

	    default: return 1;

	}
    }
    // esm_class
    esm_class_mm = (MessageMode)(*data & 0x03);
    esm_class_mt = (MessageType)(*data & 0x3c);
    esm_gsm_specific = (GSMNetworkSpecific)(*data & 0xc0);
    ++data;
    --data_length;
    // protocol_id
    protocol_id = *data;
    ++data;
    --data_length;
    // priority_flag
    priority_flag = *data;
    ++data;
    --data_length;
    // schedule_delivery_time
    schedule_delivery_time = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // validity_period
    validity_period = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // registered_delivery
    rd_smsc_dlvr_rcpt = (SMSCDeliveryReceiptType)(*data & 0x03);
    rd_sme_orig_ack = (SMEOrigAckType)(*data & 0x0c);
    rd_intrmd_ntf = (IntermediateNotificationType)(*data & 0x10);
    ++data;
    --data_length;
    // replace_if_present_flag
    replace_if_present_flag = *data;
    ++data;
    --data_length;
    // data_coding
    data_coding = (Data_codingType)(*data);
    ++data;
    --data_length;
    // sm_default_msg_id
    sm_default_msg_id = *data;
    ++data;
    --data_length;
    // sm_length
    sm_length = *data;
    ++data;
    --data_length;
    // short_message
    short_message = data;
    data += sm_length;
    data_length -= sm_length;
    // optional params
    process_params(data, data_length, _smpp_pool);
    // ok
    return 0;
}

int smpp::Submit_multi::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 17 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // service_type
    strcpy((char*)buffer, service_type);
    int tmpl = strlen(service_type) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // source_addr_ton
    *buffer = source_addr_ton;
    ++buffer;
    ++*result_length;
    // source_addr_npi
    *buffer = source_addr_npi;
    ++buffer;
    ++*result_length;
    // source_addr
    strcpy((char*)buffer, source_addr);
    tmpl = strlen(source_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // number_of_dests
    *buffer = number_of_dests;
    ++buffer;
    ++*result_length;
    // dest_address(es)
    for(unsigned int i = 0; i<dest_address.size(); i++){
	AddressDefinition ad = dest_address[i];
	// dest_flag
	*buffer = ad.dest_flag;
	++buffer;
	++*result_length;

	switch(ad.dest_flag){
	    case DA_SME_ADDRESS:
	    {
		// ton, npi, addr
		buffer[0] = ad.dest_address.dest_addr_ton;
		buffer[1] = ad.dest_address.dest_addr_npi;
		buffer += 2;
		*result_length +=2;
		// addr
		strcpy((char*)buffer, ad.dest_address.destination_addr);
		tmpl = strlen(ad.dest_address.destination_addr) + 1;
		*result_length += tmpl;
		buffer += tmpl;
		break;
	    }

	    case DA_DISTRIBUTION_LIST_ADDRESS:
	    {
		strcpy((char*)buffer, ad.dl_address.dl_name);
		tmpl = strlen(ad.dl_address.dl_name) + 1;
		*result_length += tmpl;
		buffer += tmpl;
		break;
	    }
	    default: return 1;
	}
    }
    // esm_class
    *buffer = esm_class_mm | esm_class_mt | esm_gsm_specific;
    ++buffer;
    ++*result_length;
    // protocol_id
    *buffer = protocol_id;
    ++buffer;
    ++*result_length;
    // priority_flag
    *buffer = priority_flag;
    ++buffer;
    ++*result_length;
    // schedule_delivery_time
    strcpy((char*)buffer, schedule_delivery_time);
    tmpl = strlen(schedule_delivery_time) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // validity_period
    strcpy((char*)buffer, validity_period);
    tmpl = strlen(validity_period) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // registered_delivery
    *buffer = rd_smsc_dlvr_rcpt | rd_sme_orig_ack | rd_intrmd_ntf;
    ++buffer;
    ++*result_length;
    // replace_if_present_flag
    *buffer = replace_if_present_flag;
    ++buffer;
    ++*result_length;
    // data_coding
    *buffer = data_coding;
    ++buffer;
    ++*result_length;
    // sm_default_msg_id
    *buffer = sm_default_msg_id;
    ++buffer;
    ++*result_length;
    // sm_length
    *buffer = sm_length;
    ++buffer;
    ++*result_length;
    // short_message
    memcpy(buffer, short_message, sm_length);
    buffer += sm_length;
    *result_length += sm_length;
    // optional params
    encode_optional(buffer, buffer_length - *result_length, 0, result_length);
    // ok
    return 0;
}

// Submit_multi_resp
smpp::Submit_multi_resp::Submit_multi_resp():
	SmppCommand(SUBMIT_MULTI_RESP),
	message_id(NULL),
	no_unsuccess(0){}
smpp::Submit_multi_resp::~Submit_multi_resp(){}

int smpp::Submit_multi_resp::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 2 || _smpp_pool == NULL) return 1;
    // message_id
    message_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // no_unsuccess
    no_unsuccess = *data;
    ++data;
    --data_length;
    // unsuccess_sme(s)
    for(unsigned int i = 0; i<no_unsuccess; i++){
	UnsDeliveryType ud;
	// ton, npi
	ud.dest_addr_ton = (SmppTON)data[0];
	ud.dest_addr_npi = (SmppNPI)data[1];
	ud.destination_addr = (char*)&data[2];
	data += 2;
	data_length =- 2;
	// addr
	tmp_l = Cstr_len(data, data_length);
	if(tmp_l < 0) return 1;
	data += tmp_l;
	data_length -= tmp_l;
	// error code
	ud.error_status_code = (SmppCommandStatus)((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
	data += 4;
	data_length -= 4;
	// add to list
	unsuccess_sme.push_back(ud);
    }
    // ok
    return 0;
}

int smpp::Submit_multi_resp::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 2 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // message_id
    strcpy((char*)buffer, message_id);
    int tmpl = strlen(message_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // no_unsuccess
    *buffer = no_unsuccess;
    ++buffer;
    ++*result_length;
    // unsuccess_sme(s)
    for(unsigned int i = 0; i<unsuccess_sme.size(); i++){
	UnsDeliveryType ud = unsuccess_sme[i];
	// ton, npi
	buffer[0] = ud.dest_addr_ton;
	buffer[1] = ud.dest_addr_npi;
	buffer += 2;
	++*result_length;
	// addr
	strcpy((char*)buffer, ud.destination_addr);
	tmpl = strlen(ud.destination_addr) + 1;
	*result_length += tmpl;
	buffer += tmpl;
	// error code
	buffer[0] = ud.error_status_code >> 24;
	buffer[1] = ud.error_status_code >> 16;
	buffer[2] = ud.error_status_code >> 8;
	buffer[3] = ud.error_status_code;
	buffer += 4;
	*result_length += 4;
    }

    return 0;
}

// Deliver_sm
smpp::Deliver_sm::Deliver_sm():
	SmppCommand(DELIVER_SM),
	service_type(NULL),
	source_addr_ton(TON_UNKNOWN),
	source_addr_npi(UNKNOWN),
	source_addr(NULL),
	dest_addr_ton(TON_UNKNOWN),
	dest_addr_npi(UNKNOWN),
	destination_addr(NULL),
	esm_class_mm(MM_DEFAULT_SMSC_MODE),
	esm_class_mt(MT_DEFAULT),
	esm_gsm_specific(GNS_NO_SPECIFIC_FEATURES),
	protocol_id(0),
	priority_flag(0),
	schedule_delivery_time(NULL),
	validity_period(NULL),
	rd_smsc_dlvr_rcpt(SDR_NO_SMSC_DELIVERY),
	rd_sme_orig_ack(SOA_NO_SME_ACK),
	rd_intrmd_ntf(INT_NO),
	replace_if_present_flag(0),
	data_coding(DC_DEFAULT),
	sm_default_msg_id(0),
	sm_length(0),
	short_message(NULL){}


smpp::Deliver_sm::~Deliver_sm(){}

int smpp::Deliver_sm::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 17 || _smpp_pool == NULL) return 1;
    // service_type
    service_type = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // source_addr_ton
    source_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // source_addr_npi
    source_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // source_addr
    source_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // dest_addr_ton
    dest_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // dest_addr_npi
    dest_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // destination_addr
    destination_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // esm_class
    esm_class_mm = (MessageMode)(*data & 0x03);
    esm_class_mt = (MessageType)(*data & 0x3c);
    esm_gsm_specific = (GSMNetworkSpecific)(*data & 0xc0);
    ++data;
    --data_length;
    // protocol_id
    protocol_id = *data;
    ++data;
    --data_length;
    // priority_flag
    priority_flag = *data;
    ++data;
    --data_length;
    // schedule_delivery_time
    schedule_delivery_time = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // validity_period
    validity_period = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // registered_delivery
    rd_smsc_dlvr_rcpt = (SMSCDeliveryReceiptType)(*data & 0x03);
    rd_sme_orig_ack = (SMEOrigAckType)(*data & 0x0c);
    rd_intrmd_ntf = (IntermediateNotificationType)(*data & 0x10);
    ++data;
    --data_length;
    // replace_if_present_flag
    replace_if_present_flag = *data;
    ++data;
    --data_length;
    // data_coding
    data_coding = (Data_codingType)(*data);
    ++data;
    --data_length;
    // sm_default_msg_id
    sm_default_msg_id = *data;
    ++data;
    --data_length;
    // sm_length
    sm_length = *data;
    ++data;
    --data_length;
    // short_message
    short_message = data;
    data += sm_length;
    data_length -= sm_length;
    // optional params
    process_params(data, data_length, _smpp_pool);
    // ok
    return 0;
}

int smpp::Deliver_sm::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 17 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // service_type
    strcpy((char*)buffer, service_type);
    int tmpl = strlen(service_type) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // source_addr_ton
    *buffer = source_addr_ton;
    ++buffer;
    ++*result_length;
    // source_addr_npi
    *buffer = source_addr_npi;
    ++buffer;
    ++*result_length;
    // source_addr
    strcpy((char*)buffer, source_addr);
    tmpl = strlen(source_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // dest_addr_ton
    *buffer = dest_addr_ton;
    ++buffer;
    ++*result_length;
    // dest_addr_npi
    *buffer = dest_addr_npi;
    ++buffer;
    ++*result_length;
    // destination_addr
    strcpy((char*)buffer, destination_addr);
    tmpl = strlen(destination_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // esm_class
    *buffer = esm_class_mm | esm_class_mt | esm_gsm_specific;
    ++buffer;
    ++*result_length;
    // protocol_id
    *buffer = protocol_id;
    ++buffer;
    ++*result_length;
    // priority_flag
    *buffer = priority_flag;
    ++buffer;
    ++*result_length;
    // schedule_delivery_time
    strcpy((char*)buffer, schedule_delivery_time);
    tmpl = strlen(schedule_delivery_time) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // validity_period
    strcpy((char*)buffer, validity_period);
    tmpl = strlen(validity_period) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // registered_delivery
    *buffer = rd_smsc_dlvr_rcpt | rd_sme_orig_ack | rd_intrmd_ntf;
    ++buffer;
    ++*result_length;
    // replace_if_present_flag
    *buffer = replace_if_present_flag;
    ++buffer;
    ++*result_length;
    // data_coding
    *buffer = data_coding;
    ++buffer;
    ++*result_length;
    // sm_default_msg_id
    *buffer = sm_default_msg_id;
    ++buffer;
    ++*result_length;
    // sm_length
    *buffer = sm_length;
    ++buffer;
    ++*result_length;
    // short_message
    memcpy(buffer, short_message, sm_length);
    buffer += sm_length;
    *result_length += sm_length;
    // optional params
    encode_optional(buffer, buffer_length - *result_length, 0, result_length);
    // ok
    return 0;
}


// Deliver_sm_resp
smpp::Deliver_sm_resp::Deliver_sm_resp():
	SmppCommand(DELIVER_SM_RESP),
	message_id(NULL){}
smpp::Deliver_sm_resp::~Deliver_sm_resp(){}

int smpp::Deliver_sm_resp::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 1 || _smpp_pool == NULL) return 1;
    // message_id
    message_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // ok
    return 0;
}

int smpp::Deliver_sm_resp::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 1 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // message_id
    strcpy((char*)buffer, message_id);
    int tmpl = strlen(message_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // ok
    return 0;
}

// Data_sm
smpp::Data_sm::Data_sm():
	SmppCommand(DATA_SM),
	service_type(NULL),
	source_addr_ton(TON_UNKNOWN),
	source_addr_npi(UNKNOWN),
	source_addr(NULL),
	dest_addr_ton(TON_UNKNOWN),
	dest_addr_npi(UNKNOWN),
	destination_addr(NULL),
	esm_class_mm(MM_DEFAULT_SMSC_MODE),
	esm_class_mt(MT_DEFAULT),
	esm_gsm_specific(GNS_NO_SPECIFIC_FEATURES),
	rd_smsc_dlvr_rcpt(SDR_NO_SMSC_DELIVERY),
	rd_sme_orig_ack(SOA_NO_SME_ACK),
	rd_intrmd_ntf(INT_NO),
	data_coding(DC_DEFAULT){}


smpp::Data_sm::~Data_sm(){}

int smpp::Data_sm::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 10 || _smpp_pool == NULL) return 1;
    // service_type
    service_type = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // source_addr_ton
    source_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // source_addr_npi
    source_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // source_addr
    source_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // dest_addr_ton
    dest_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // dest_addr_npi
    dest_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // destination_addr
    destination_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // esm_class
    esm_class_mm = (MessageMode)(*data & 0x03);
    esm_class_mt = (MessageType)(*data & 0x3c);
    esm_gsm_specific = (GSMNetworkSpecific)(*data & 0xc0);
    ++data;
    --data_length;
    // registered_delivery
    rd_smsc_dlvr_rcpt = (SMSCDeliveryReceiptType)(*data & 0x03);
    rd_sme_orig_ack = (SMEOrigAckType)(*data & 0x0c);
    rd_intrmd_ntf = (IntermediateNotificationType)(*data & 0x10);
    ++data;
    --data_length;
    // data_coding
    data_coding = (Data_codingType)(*data);
    ++data;
    --data_length;
    // optional params
    process_params(data, data_length, _smpp_pool);
    // ok
    return 0;
}

int smpp::Data_sm::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 10 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // service_type
    strcpy((char*)buffer, service_type);
    int tmpl = strlen(service_type) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // source_addr_ton
    *buffer = source_addr_ton;
    ++buffer;
    ++*result_length;
    // source_addr_npi
    *buffer = source_addr_npi;
    ++buffer;
    ++*result_length;
    // source_addr
    strcpy((char*)buffer, source_addr);
    tmpl = strlen(source_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // dest_addr_ton
    *buffer = dest_addr_ton;
    ++buffer;
    ++*result_length;
    // dest_addr_npi
    *buffer = dest_addr_npi;
    ++buffer;
    ++*result_length;
    // destination_addr
    strcpy((char*)buffer, destination_addr);
    tmpl = strlen(destination_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // esm_class
    *buffer = esm_class_mm | esm_class_mt | esm_gsm_specific;
    ++buffer;
    ++*result_length;
    // registered_delivery
    *buffer = rd_smsc_dlvr_rcpt | rd_sme_orig_ack | rd_intrmd_ntf;
    ++buffer;
    ++*result_length;
    // data_coding
    *buffer = data_coding;
    ++buffer;
    ++*result_length;
    // optional params
    encode_optional(buffer, buffer_length - *result_length, 0, result_length);
    // ok
    return 0;
}

// Data_sm_resp
smpp::Data_sm_resp::Data_sm_resp():
	SmppCommand(DATA_SM_RESP),
	message_id(NULL){}
smpp::Data_sm_resp::~Data_sm_resp(){}

int smpp::Data_sm_resp::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 1 || _smpp_pool == NULL) return 1;
    // message_id
    message_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // ok
    return 0;
}

int smpp::Data_sm_resp::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 1 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // message_id
    strcpy((char*)buffer, message_id);
    int tmpl = strlen(message_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // ok
    return 0;
}


// Query_sm
smpp::Query_sm::Query_sm():
	SmppCommand(QUERY_SM),
	message_id(NULL),
	source_addr_ton(TON_UNKNOWN),
	source_addr_npi(UNKNOWN),
	source_addr(NULL){}
smpp::Query_sm::~Query_sm(){}

int smpp::Query_sm::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 4 || _smpp_pool == NULL) return 1;
    // message_id
    message_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // source_addr_ton
    source_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // source_addr_npi
    source_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // source_addr
    source_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // ok
    return 0;
}

int smpp::Query_sm::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 4 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // message_id
    strcpy((char*)buffer, message_id);
    int tmpl = strlen(message_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // source_addr_ton
    *buffer = source_addr_ton;
    ++buffer;
    ++*result_length;
    // source_addr_npi
    *buffer = source_addr_npi;
    ++buffer;
    ++*result_length;
    // source_addr
    strcpy((char*)buffer, source_addr);
    tmpl = strlen(source_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // ok
    return 0;
}

// Query_sm_resp
smpp::Query_sm_resp::Query_sm_resp():
	SmppCommand(QUERY_SM_RESP),
	message_id(NULL),
	final_date(NULL),
	message_state(MST_ENROUTE),
	error_code(0){}
smpp::Query_sm_resp::~Query_sm_resp(){}

int smpp::Query_sm_resp::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 4 || _smpp_pool == NULL) return 1;
    // message_id
    message_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // final_date
    final_date = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // message_state
    message_state = (MessageStateType)*data;
    ++data;
    --data_length;
    // error_code
    error_code = *data;
    ++data;
    --data_length;
    // ok
    return 0;
}

int smpp::Query_sm_resp::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 4 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // message_id
    strcpy((char*)buffer, message_id);
    int tmpl = strlen(message_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // final_date
    strcpy((char*)buffer, final_date);
    tmpl = strlen(final_date) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // message_state
    *buffer = message_state;
    ++buffer;
    ++*result_length;
    // error_code
    *buffer = error_code;
    ++buffer;
    ++*result_length;
    // ok
    return 0;
}

// Cancel_sm
smpp::Cancel_sm::Cancel_sm():
	SmppCommand(CANCEL_SM),
	service_type(NULL),
	message_id(NULL),
	source_addr_ton(TON_UNKNOWN),
	source_addr_npi(UNKNOWN),
	source_addr(NULL),
	dest_addr_ton(TON_UNKNOWN),
	dest_addr_npi(UNKNOWN),
	destination_addr(NULL){}
smpp::Cancel_sm::~Cancel_sm(){}

int smpp::Cancel_sm::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 8 || _smpp_pool == NULL) return 1;
    // service_type
    service_type = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // message_id
    message_id = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // source_addr_ton
    source_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // source_addr_npi
    source_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // source_addr
    source_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // dest_addr_ton
    dest_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // dest_addr_npi
    dest_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // destination_addr
    destination_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // ok
    return 0;
}

int smpp::Cancel_sm::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 8 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // service_type
    strcpy((char*)buffer, service_type);
    int tmpl = strlen(service_type) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // message_id
    strcpy((char*)buffer, message_id);
    tmpl = strlen(message_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // source_addr_ton
    *buffer = source_addr_ton;
    ++buffer;
    ++*result_length;
    // source_addr_npi
    *buffer = source_addr_npi;
    ++buffer;
    ++*result_length;
    // source_addr
    strcpy((char*)buffer, source_addr);
    tmpl = strlen(source_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // dest_addr_ton
    *buffer = dest_addr_ton;
    ++buffer;
    ++*result_length;
    // dest_addr_npi
    *buffer = dest_addr_npi;
    ++buffer;
    ++*result_length;
    // destination_addr
    strcpy((char*)buffer, destination_addr);
    tmpl = strlen(destination_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // ok
    return 0;
}

// Cancel_sm_resp
smpp::Cancel_sm_resp::Cancel_sm_resp(): SmppCommand(CANCEL_SM_RESP){}
smpp::Cancel_sm_resp::~Cancel_sm_resp(){}



// Replace_sm
smpp::Replace_sm::Replace_sm():
	SmppCommand(REPLACE_SM),
	message_id(NULL),
	source_addr_ton(TON_UNKNOWN),
	source_addr_npi(UNKNOWN),
	source_addr(NULL),
	schedule_delivery_time(NULL),
	validity_period(NULL),
	rd_smsc_dlvr_rcpt(SDR_NO_SMSC_DELIVERY),
	rd_sme_orig_ack(SOA_NO_SME_ACK),
	rd_intrmd_ntf(INT_NO),
	sm_default_msg_id(0),
	sm_length(0),
	short_message(NULL){}
smpp::Replace_sm::~Replace_sm(){}

int smpp::Replace_sm::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 9 || _smpp_pool == NULL) return 1;
    // message_id
    message_id = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // source_addr_ton
    source_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // source_addr_npi
    source_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // source_addr
    source_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // schedule_delivery_time
    schedule_delivery_time = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // validity_period
    validity_period = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // registered_delivery
    rd_smsc_dlvr_rcpt = (SMSCDeliveryReceiptType)(*data & 0x03);
    rd_sme_orig_ack = (SMEOrigAckType)(*data & 0x0c);
    rd_intrmd_ntf = (IntermediateNotificationType)(*data & 0x10);
    ++data;
    --data_length;
    // sm_default_msg_id
    sm_default_msg_id = *data;
    ++data;
    --data_length;
    // sm_length
    sm_length = *data;
    ++data;
    --data_length;
    // short_message
    short_message = data;
    // ok
    return 0;
}

int smpp::Replace_sm::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 9 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // message_id
    strcpy((char*)buffer, message_id);
    int tmpl = strlen(message_id) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // source_addr_ton
    *buffer = source_addr_ton;
    ++buffer;
    ++*result_length;
    // source_addr_npi
    *buffer = source_addr_npi;
    ++buffer;
    ++*result_length;
    // source_addr
    strcpy((char*)buffer, source_addr);
    tmpl = strlen(source_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // schedule_delivery_time
    strcpy((char*)buffer, schedule_delivery_time);
    tmpl = strlen(schedule_delivery_time) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // validity_period
    strcpy((char*)buffer, validity_period);
    tmpl = strlen(validity_period) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // registered_delivery
    *buffer = rd_smsc_dlvr_rcpt | rd_sme_orig_ack | rd_intrmd_ntf;
    ++buffer;
    ++*result_length;
    // sm_default_msg_id
    *buffer = sm_default_msg_id;
    ++buffer;
    ++*result_length;
    // sm_length
    *buffer = sm_length;
    ++buffer;
    ++*result_length;
    // short_message
    memcpy(buffer, short_message, sm_length);
    *result_length += sm_length;
    // ok
    return 0;
}

// Replace_sm_resp
smpp::Replace_sm_resp::Replace_sm_resp(): SmppCommand(REPLACE_SM_RESP){}
smpp::Replace_sm_resp::~Replace_sm_resp(){}


// Enquire_link
smpp::Enquire_link::Enquire_link(): SmppCommand(ENQUIRE_LINK){}
smpp::Enquire_link::~Enquire_link(){}


// Enquire_link_resp
smpp::Enquire_link_resp::Enquire_link_resp(): SmppCommand(ENQUIRE_LINK_RESP){}
smpp::Enquire_link_resp::~Enquire_link_resp(){}


// Alert_notification
smpp::Alert_notification::Alert_notification():
	SmppCommand(ALERT_NOTIFICATION),
	source_addr_ton(TON_UNKNOWN),
	source_addr_npi(UNKNOWN),
	source_addr(NULL),
	esme_addr_ton(TON_UNKNOWN),
	esme_addr_npi(UNKNOWN),
	esme_addr(NULL){}
smpp::Alert_notification::~Alert_notification(){}

int smpp::Alert_notification::init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool){
    // sanity check
    if(data == NULL || data_length < 6 || _smpp_pool == NULL) return 1;
    // source_addr_ton
    source_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // source_addr_npi
    source_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // source_addr
    source_addr = (char*)data;
    int tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // esme_addr_ton
    esme_addr_ton = (SmppTON)*data;
    ++data;
    --data_length;
    // esme_addr_npi
    esme_addr_npi = (SmppNPI)*data;
    ++data;
    --data_length;
    // esme_addr
    esme_addr = (char*)data;
    tmp_l = Cstr_len(data, data_length);
    if(tmp_l < 0) return 1;
    data += tmp_l;
    data_length -= tmp_l;
    // optional params
    process_params(data, data_length, _smpp_pool);
    // ok
    return 0;
}

int smpp::Alert_notification::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    if(buffer == NULL || buffer_length < 6 || result_length == NULL) return 1;
    *result_length = 0;
    // offset
    buffer += offset;
    // source_addr_ton
    *buffer = source_addr_ton;
    ++buffer;
    ++*result_length;
    // source_addr_npi
    *buffer = source_addr_npi;
    ++buffer;
    ++*result_length;
    // source_addr
    strcpy((char*)buffer, source_addr);
    int tmpl = strlen(source_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // esme_addr_ton
    *buffer = esme_addr_ton;
    ++buffer;
    ++*result_length;
    // esme_addr_npi
    *buffer = esme_addr_npi;
    ++buffer;
    ++*result_length;
    // esme_addr
    strcpy((char*)buffer, esme_addr);
    tmpl = strlen(esme_addr) + 1;
    *result_length += tmpl;
    buffer += tmpl;
    // optional params
    encode_optional(buffer, buffer_length - *result_length, 0, result_length);
    // ok
    return 0;
}


// DlvrRcpt
smpp::DlvrRcpt::DlvrRcpt():
	//id_size(0),
	sub(0),
	dlvrd(0),
	submit_date(0),
	done_date(0),
	stat(MST_UNKNOWN),
	err(0),
	text_size(0){}


void smpp::DlvrRcpt::reset(){
    sub = 0;
    dlvrd = 0;
    submit_date = 0;
    done_date = 0;
    stat = MST_UNKNOWN;
    err = 0;
    text_size = 0;
    bzero(id, sizeof(id));

}


int smpp::DlvrRcpt::encode(unsigned char* buffer, int buffer_length, int* result_length){
    if(buffer == NULL || buffer_length < 93 || result_length == NULL) return 1;
    *result_length = 0;
    // id:1 sub:001 dlvrd:000 submit date:1501212135 done date:1501212136 stat:UNDELIV err:000 text:

    // id
    int id_l = strlen(id);
    memcpy(buffer, "id:", 3);
    memcpy(&buffer[3], id, id_l);
    memcpy(&buffer[3 + id_l], " ", 1);
    buffer += 3 + id_l + 1;
    *result_length +=  3 + id_l + 1;

    // sub
    memcpy(buffer, "sub:", 4);
    sprintf((char*)&buffer[4], "%03d", sub);
    memcpy(&buffer[7], " ", 1);
    buffer += 8;
    *result_length += 8;

    // dlvrd
    memcpy(buffer, "dlvrd:", 6);
    sprintf((char*)&buffer[6], "%03d", dlvrd);
    memcpy(&buffer[9], " ", 1);
    buffer += 10;
    *result_length += 10;

    // submit date
    memcpy(buffer, "submit date:", 12);
    buffer += 12;
    *result_length += 12;
    tm new_tm;
    localtime_r(&submit_date, &new_tm);
    sprintf((char*)buffer,
	    "%02d%02d%02d%02d%02d",
	    (new_tm.tm_year + 1900) % 100,
	    new_tm.tm_mon + 1,
	    new_tm.tm_mday,
	    new_tm.tm_hour,
	    new_tm.tm_min);

    buffer += 10;
    memcpy(buffer++, " ", 1);
    *result_length += 11;

    // done date
    memcpy(buffer, "done date:", 10);
    buffer += 10;
    *result_length += 10;
    localtime_r(&done_date, &new_tm);
    sprintf((char*)buffer,
	    "%02d%02d%02d%02d%02d",
	    (new_tm.tm_year + 1900) % 100,
	    new_tm.tm_mon + 1,
	    new_tm.tm_mday,
	    new_tm.tm_hour,
	    new_tm.tm_min);

    buffer += 10;
    memcpy(buffer++, " ", 1);
    *result_length += 11;

    // stat
    memcpy(buffer, "stat:", 5);
    buffer += 5;
    *result_length += 5;
    switch(stat){
	case MST_DELIVERED:
	    memcpy(buffer, "DELIVRD ", 8);
	    buffer += 8;
	    *result_length += 8;
	    break;

	case MST_EXPIRED:
	    memcpy(buffer, "EXPIRED ", 8);
	    buffer += 8;
	    *result_length += 8;
	    break;

	case MST_DELETED:
	    memcpy(buffer, "DELETED ", 8);
	    buffer += 8;
	    *result_length += 8;
	    break;

	case MST_UNDELIVERABLE:
	    memcpy(buffer, "UNDELIV ", 8);
	    buffer += 8;
	    *result_length += 8;
	    break;

	case MST_ACCEPTED:
	    memcpy(buffer, "ACCEPTD ", 8);
	    buffer += 8;
	    *result_length += 8;
	    break;

	case MST_UNKNOWN:
	    memcpy(buffer, "UNKNOWN ", 8);
	    buffer += 8;
	    *result_length += 8;
	    break;

	case MST_REJECTED:
	    memcpy(buffer, "REJECTD ", 8);
	    buffer += 8;
	    *result_length += 8;
	    break;

	default: break;

    }

    // err
    memcpy(buffer, "err:", 4);
    sprintf((char*)&buffer[4], "%03d", err);
    memcpy(&buffer[7], " ", 1);
    buffer += 8;
    *result_length += 8;

    // text
    memcpy(buffer, "text:", 5);
    buffer += 5;
    *result_length += 5;
    if(text_size > 0){
	memcpy(buffer, text, text_size);
	buffer += text_size;
	*result_length += text_size;
    }
    return 0;
}


int smpp::DlvrRcpt::decode_var_1(unsigned char* data, int data_length){
    if(data == NULL || data_length < 45) return 1;
    // Failed Id:<SMSCREF> OA:<OA> DA:<DA> date:<REPDATE> time:<REPTIME> dfr:<DFR> accepted <ACCDATE> <ACCTIME>
    // Failed Id:14000742856881948759 OA:99002 DA:421903333999 date:08.05.15 time:13:43:47 dfr:prijemca je neznamy accepted 08.05.15 13:43:47

    // SM sent by <OA> & accepted at <ACCTIME> on <ACCDATE> DELIVERED to <DA> at <REPTIME> on <REPDATE>.SMSC Ref:<SMSCREF>

    // reset
    reset();
    // split by space
    // tmp copy
    char tmp_data[data_length + 1];
    char* tmp_data_p = tmp_data;
    memcpy(tmp_data, data, data_length);
    tmp_data[data_length] = 0;
    // tokenize
    char* token;
    int tc = 0;
    unsigned int tmp_l = 0;
    unsigned int max_l = 0;
    int sub_type = -1;

    // check if Failed
    if(memcmp(data, "Failed Id:", 10) == 0){
	// set sub type
	sub_type = 0;
	// set status
	stat = MST_REJECTED;
	// split by space
	while((token = strsep(&tmp_data_p, " ")) != NULL){
	    //std::cout << "TOKEN: " << token << std::endl;
	    switch(tc){
		// id
		case 1:
		    tmp_l = strlen(token);
		    if(tmp_l < 4) return 10;
		    tmp_l -= 3;
		    //id_size = tmp_l;
		    max_l = (tmp_l > sizeof(id) -1 ? sizeof(id) - 1 : tmp_l);
		    memcpy(id, &token[3], max_l);
		    id[max_l] = 0;
		    break;

		// date
		case 4:
		{
		    tmp_l = strlen(token);
		    if(tmp_l == 5) {
			submit_date = 0;
			break;
		    }
		    if(tmp_l != 13) return 13;
		    // get local time
		    time_t now = time(NULL);
		    tm tm_new, tmnow;
		    localtime_r(&now, &tmnow);
		    tm_new.tm_year = (2000 + ascii_2_num(&token[11], 2)) - 1900;
		    tm_new.tm_mon = ascii_2_num(&token[8], 2) - 1;
		    tm_new.tm_mday = ascii_2_num(&token[5], 2);
		    tm_new.tm_hour = 0;
		    tm_new.tm_min = 0;
		    tm_new.tm_sec = 0;
		    tm_new.tm_isdst = tmnow.tm_isdst;
		    // get unix timestamp
		    submit_date = mktime(&tm_new);
		    break;
		}

		// time
		case 5:
		{
		    tmp_l = strlen(token);
		    if(tmp_l == 5) {
			break;
		    }
		    if(tmp_l != 13) return 13;
		    tm tm_new, tmnow;
		    localtime_r(&submit_date, &tmnow);
		    tm_new.tm_hour = ascii_2_num(&token[5], 2);
		    tm_new.tm_min = ascii_2_num(&token[8], 2);
		    tm_new.tm_sec = ascii_2_num(&token[11], 2);
		    tm_new.tm_isdst = tmnow.tm_isdst;
		    // get unix timestamp
		    submit_date = mktime(&tm_new);
		    break;
		}


	    }

	    ++tc;

	}
	// check tc
	if(tc < 10) return 1;
	// ok
	return 0;

    // check if OK
    }else if(memcmp(data, "SM sent by", 10) == 0){
	// set sub type
	sub_type = 1;
	// set status
	stat = MST_DELIVERED;
	// split by space
	while((token = strsep(&tmp_data_p, " ")) != NULL){
	    //std::cout << "TOKEN: " << token << std::endl;
	    switch(tc){
		// submit time
		case 7:
		{
		    tmp_l = strlen(token);
		    if(tmp_l == 0) {
			break;
		    }
		    if(tmp_l != 8) return 8;
		    // get local time
		    time_t now = time(NULL);
		    tm tm_new, tmnow;
		    localtime_r(&now, &tmnow);
		    tm_new.tm_hour = ascii_2_num(&token[0], 2);
		    tm_new.tm_min = ascii_2_num(&token[3], 2);
		    tm_new.tm_sec = ascii_2_num(&token[6], 2);
		    tm_new.tm_isdst = tmnow.tm_isdst;
		    // get unix timestamp
		    submit_date = mktime(&tm_new);
		    break;
		}

		// submit date
		case 9:
		{
		    tmp_l = strlen(token);
		    if(tmp_l == 0) {
			break;
		    }
		    if(tmp_l != 8) return 8;
		    tm tm_new, tmnow;
		    localtime_r(&submit_date, &tmnow);
		    tm_new.tm_year = (2000 + ascii_2_num(&token[6], 2)) - 1900;
		    tm_new.tm_mon = ascii_2_num(&token[3], 2) - 1;
		    tm_new.tm_mday = ascii_2_num(&token[0], 2);
		    // get unix timestamp
		    submit_date = mktime(&tm_new);
		    break;
		}

		// delivery time
		case 14:
		{
		    tmp_l = strlen(token);
		    if(tmp_l == 0) {
			break;
		    }
		    if(tmp_l != 8) return 8;
		    // get local time
		    time_t now = time(NULL);
		    tm tm_new, tmnow;
		    localtime_r(&now, &tmnow);
		    tm_new.tm_hour = ascii_2_num(&token[0], 2);
		    tm_new.tm_min = ascii_2_num(&token[3], 2);
		    tm_new.tm_sec = ascii_2_num(&token[6], 2);
		    tm_new.tm_isdst = tmnow.tm_isdst;
		    // get unix timestamp
		    done_date = mktime(&tm_new);
		    break;
		}

		// delivery date
		case 16:
		{
		    tmp_l = strlen(token);
		    if(tmp_l == 0) {
			break;
		    }
		    if(tmp_l < 8) return 8;
		    tm tm_new, tmnow;
		    localtime_r(&done_date, &tmnow);
		    tm_new.tm_year = (2000 + ascii_2_num(&token[6], 2)) - 1900;
		    tm_new.tm_mon = ascii_2_num(&token[3], 2) - 1;
		    tm_new.tm_mday = ascii_2_num(&token[0], 2);
		    // get unix timestamp
		    done_date = mktime(&tm_new);
		    break;
		}

		// id
		case 17:
		{
		    tmp_l = strlen(token);
		    if(tmp_l < 5) return 10;
		    tmp_l -= 4;
		    //id_size = tmp_l;
		    max_l = (tmp_l > sizeof(id) -1 ? sizeof(id) - 1 : tmp_l);
		    memcpy(id, &token[4], max_l);
		    id[max_l] = 0;
		    break;
		}


	    }

	    ++tc;
	}
	// check tc
	if(tc < 18) return 1;
	// ok
	return 0;

    }

    // sanity check
    if(sub_type == -1) return 1;

    // err
    return 1;
}

int smpp::DlvrRcpt::decode_var_2(unsigned char* data, int data_length){
    if(data == NULL || data_length < 46) return 1;
    // id:1 submit date:20151203105731 done date:20151203105737 stat:DELIVRD err:0

    /*
    TOKEN: id:1
    TOKEN: submit
    TOKEN: date:20151203105731
    TOKEN: done
    TOKEN: date:20151203105737
    TOKEN: stat:DELIVRD
    TOKEN: err:0
*/

    // reset
    reset();
    // split by space
    // tmp copy
    char tmp_data[data_length + 1];
    char* tmp_data_p = tmp_data;
    memcpy(tmp_data, data, data_length);
    tmp_data[data_length] = 0;
    // tokenize
    char* token;
    int tc = 0;
    unsigned int tmp_l = 0;
    unsigned int max_l = 0;
    // split by space
    while((token = strsep(&tmp_data_p, " ")) != NULL){
	//std::cout << "TOKEN: " << token << std::endl;
	switch(tc){
	    // id
	    case 0:
		tmp_l = strlen(token);
		if(tmp_l < 4) return 10;
		tmp_l -= 3;
		//id_size = tmp_l;
		max_l = (tmp_l > sizeof(id) -1 ? sizeof(id) - 1 : tmp_l);
		memcpy(id, &token[3], max_l);
		id[max_l] = 0;
		break;

	    // submit date
	    case 2:
	    {
		tmp_l = strlen(token);
		if(tmp_l == 5) {
		    submit_date = 0;
		    break;
		}
		if(!(tmp_l == 17 || tmp_l == 19)) return 13;
		// get local time
		time_t now = time(NULL);
		tm tm_new, tmnow;
		localtime_r(&now, &tmnow);
		tm_new.tm_year = ascii_2_num(&token[5], 4) - 1900;
		tm_new.tm_mon = ascii_2_num(&token[9], 2) - 1;
		tm_new.tm_mday = ascii_2_num(&token[11], 2);
		tm_new.tm_hour = ascii_2_num(&token[13], 2);
		tm_new.tm_min = ascii_2_num(&token[15], 2);
		// seconds included
		if(tmp_l == 19) tm_new.tm_sec = ascii_2_num(&token[17], 2);
		else tm_new.tm_sec = 0;
		tm_new.tm_isdst = tmnow.tm_isdst;
		// get unix timestamp
		submit_date = mktime(&tm_new);
		break;
	    }

	    // done date
	    case 4:
	    {
		tmp_l = strlen(token);
		if(tmp_l == 5) {
		    done_date = 0;
		    break;
		}
		if(!(tmp_l == 17 || tmp_l == 19)) return 13;
		// get local time
		time_t now = time(NULL);
		tm tm_new, tmnow;
		localtime_r(&now, &tmnow);
		tm_new.tm_year = ascii_2_num(&token[5], 4) - 1900;
		tm_new.tm_mon = ascii_2_num(&token[9], 2) - 1;
		tm_new.tm_mday = ascii_2_num(&token[11], 2);
		tm_new.tm_hour = ascii_2_num(&token[13], 2);
		tm_new.tm_min = ascii_2_num(&token[15], 2);
		// seconds included
		if(tmp_l == 19) tm_new.tm_sec = ascii_2_num(&token[17], 2);
		else tm_new.tm_sec = 0;
		tm_new.tm_isdst = tmnow.tm_isdst;
		// get unix timestamp
		done_date = mktime(&tm_new);
		break;
	    }

	    // stat
	    case 5:
		tmp_l = strlen(token);
		if(tmp_l == 5) {
		    stat = MST_UNKNOWN;
		    break;
		}
		if(tmp_l != 12) return 14;
		tmp_l -= 12;
		// check status
		if(strcmp(&token[5], "DELIVRD") == 0) stat = MST_DELIVERED;
		else if(strcmp(&token[5], "EXPIRED") == 0) stat = MST_EXPIRED;
		else if(strcmp(&token[5], "DELETED") == 0) stat = MST_DELETED;
		else if(strcmp(&token[5], "UNDELIV") == 0) stat = MST_UNDELIVERABLE;
		else if(strcmp(&token[5], "ACCEPTD") == 0) stat = MST_ACCEPTED;
		else if(strcmp(&token[5], "UNKNOWN") == 0) stat = MST_UNKNOWN;
		else if(strcmp(&token[5], "REJECTD") == 0) stat = MST_REJECTED;
		else stat = MST_UNKNOWN;
		break;

	    // err
	    case 6:
		tmp_l = strlen(token);
		if(tmp_l == 4) {
		    err = 0;
		    break;
		}
		tmp_l -= 4;
		err = ascii_2_num(&token[4], tmp_l);
		break;

	}

	++tc;


    }

    // check tc
    if(tc < 7) return 1;
    // ok
    return 0;
}


int smpp::DlvrRcpt::decode(unsigned char* data, int data_length){
    if(data == NULL || data_length < 63) return 1;
    // id:1 sub:001 dlvrd:000 submit date:1501212135 done date:1501212136 stat:UNDELIV err:000 text:
    /*
    TOKEN: id:1
    TOKEN: sub:001
    TOKEN: dlvrd:000
    TOKEN: submit
    TOKEN: date:1501212135
    TOKEN: done
    TOKEN: date:1501212136
    TOKEN: stat:UNDELIV
    TOKEN: err:000
    TOKEN: text:
*/

    // reset
    reset();
    // split by space
    // tmp copy
    char tmp_data[data_length + 1];
    char* tmp_data_p = tmp_data;
    memcpy(tmp_data, data, data_length);
    tmp_data[data_length] = 0;
    // tokenize
    char* token;
    int tc = 0;
    unsigned int tmp_l = 0;
    unsigned int max_l = 0;
    // split by space
    while((token = strsep(&tmp_data_p, " ")) != NULL){
	//std::cout << "TOKEN: " << token << std::endl;
	switch(tc){
	    // id
	    case 0:
		tmp_l = strlen(token);
		if(tmp_l < 4) return 10;
		tmp_l -= 3;
		//id_size = tmp_l;
		max_l = (tmp_l > sizeof(id) -1 ? sizeof(id) - 1 : tmp_l);
		memcpy(id, &token[3], max_l);
		id[max_l] = 0;
		break;

	    // sub
	    case 1:
		tmp_l = strlen(token);
		if(tmp_l == 4) {
		    sub = 0;
		    break;
		}
		//if(tmp_l != 7) return 11;
		tmp_l -= 4;
		sub = ascii_2_num(&token[4], tmp_l);
		break;

	    // dlvrd
	    case 2:
		tmp_l = strlen(token);
		if(tmp_l == 6) {
		    dlvrd = 0;
		    break;
		}
		//if(tmp_l != 9) return 12;
		tmp_l -= 6;
		dlvrd = ascii_2_num(&token[6], tmp_l);
		break;

	    // submit date
	    case 4:
	    {
		tmp_l = strlen(token);
		if(tmp_l == 5) {
		    submit_date = 0;
		    break;
		}
		if(!(tmp_l == 15 || tmp_l == 17)) return 13;
		// get local time
		time_t now = time(NULL);
		tm tm_new, tmnow;
		localtime_r(&now, &tmnow);
		tm_new.tm_year = (2000 + ascii_2_num(&token[5], 2)) - 1900;
		tm_new.tm_mon = ascii_2_num(&token[7], 2) - 1;
		tm_new.tm_mday = ascii_2_num(&token[9], 2);
		tm_new.tm_hour = ascii_2_num(&token[11], 2);
		tm_new.tm_min = ascii_2_num(&token[13], 2);
		// seconds included
		if(tmp_l == 17) tm_new.tm_sec = ascii_2_num(&token[15], 2);
		else tm_new.tm_sec = 0;
		tm_new.tm_isdst = tmnow.tm_isdst;
		// get unix timestamp
		submit_date = mktime(&tm_new);
		break;
	    }

	    // done date
	    case 6:
	    {
		tmp_l = strlen(token);
		if(tmp_l == 5) {
		    done_date = 0;
		    break;
		}
		if(!(tmp_l == 15 || tmp_l == 17)) return 13;
		// get local time
		time_t now = time(NULL);
		tm tm_new, tmnow;
		localtime_r(&now, &tmnow);
		tm_new.tm_year = (2000 + ascii_2_num(&token[5], 2)) - 1900;
		tm_new.tm_mon = ascii_2_num(&token[7], 2) - 1;
		tm_new.tm_mday = ascii_2_num(&token[9], 2);
		tm_new.tm_hour = ascii_2_num(&token[11], 2);
		tm_new.tm_min = ascii_2_num(&token[13], 2);
		// seconds included
		if(tmp_l == 17) tm_new.tm_sec = ascii_2_num(&token[15], 2);
		else tm_new.tm_sec = 0;
		tm_new.tm_isdst = tmnow.tm_isdst;
		// get unix timestamp
		done_date = mktime(&tm_new);
		break;
	    }

	    // stat
	    case 7:
		tmp_l = strlen(token);
		if(tmp_l == 5) {
		    stat = MST_UNKNOWN;
		    break;
		}
		if(tmp_l != 12) return 14;
		tmp_l -= 12;
		// check status
		if(strcmp(&token[5], "DELIVRD") == 0) stat = MST_DELIVERED;
		else if(strcmp(&token[5], "EXPIRED") == 0) stat = MST_EXPIRED;
		else if(strcmp(&token[5], "DELETED") == 0) stat = MST_DELETED;
		else if(strcmp(&token[5], "UNDELIV") == 0) stat = MST_UNDELIVERABLE;
		else if(strcmp(&token[5], "ACCEPTD") == 0) stat = MST_ACCEPTED;
		else if(strcmp(&token[5], "UNKNOWN") == 0) stat = MST_UNKNOWN;
		else if(strcmp(&token[5], "REJECTD") == 0) stat = MST_REJECTED;
		else stat = MST_UNKNOWN;
		break;

	    // err
	    case 8:
		tmp_l = strlen(token);
		if(tmp_l == 4) {
		    err = 0;
		    break;
		}
		//if(tmp_l != 7) return 15;lse
		tmp_l -= 4;
		err = ascii_2_num(&token[4], tmp_l);
		break;

	    // text
	    case 9:
		tmp_l = strlen(token);
		if(tmp_l < 5) return 16;
		text_size = 0;
		if(tmp_l > 6 && tmp_l < 26){
		    tmp_l -= 5;
		    memcpy(text, &token[5], tmp_l);
		    text_size = tmp_l;

		}
		break;


	}

	++tc;


    }

    // check tc
    if(tc < 10) return 1;
    // ok
    return 0;
}

// TcpFraming
smpp::TcpFraming::TcpFraming(): sep_buff_p(sep_buff),
                                sep_buff_l(0),
                                next_pdu_l(0),
                                next_pdu_rb(0){}

smpp::TcpFraming::~TcpFraming(){}

int smpp::TcpFraming::process(unsigned char* buff, unsigned int buff_size, void* args){
    // ***************************************************************************
    // SMPP TCP reassembly
    // ***************************************************************************
    //
    // sep_buff	        -	reassembly buffer
    // sep_buff_l	-	number of bytes used in reassembly buffer
    // sep_buff_p 	- 	pointer to next free byte in reassembly buffer
    // next_pdu_l	-	length of current PDU
    // next_pdu_rb	-	number of bytes received for the current PDU
    // pdu_end_p	-	pointer to one byte past last PDU's byte in 
    //                          reassembly buffer
    //
    // ***************************************************************************
    // sanity check
    if(sep_buff_p + buff_size > &sep_buff[sizeof(sep_buff) - 1]) return 1;
    // copy data to sep buffer
    memcpy(sep_buff_p, buff, buff_size);
    // fwd buff pointer
    sep_buff_p += buff_size;
    // update sep buffer length
    sep_buff_l += buff_size;
    // set vars
    unsigned int sep_i = next_pdu_rb;
    unsigned char* pdu_end_p = sep_buff;
    // process bytes
    while(sep_i < sep_buff_l){
        // check if next pdu length is known
        if(next_pdu_l > 0){
            // inc pdu bytes received
            ++next_pdu_rb;
            // check if pdu complete
            if(next_pdu_l == next_pdu_rb){
                // update pdu end pointer (one byte after pdu's last byte)
                pdu_end_p = &sep_buff[sep_i + 1];
                // call pdu handler
                on_pdu_ready(pdu_end_p - next_pdu_l, next_pdu_l, args);
                // reset pdu length and bytes received
                next_pdu_l = 0;
                next_pdu_rb = 0;
            }
            // pdu length still unknown
        }else{
            // min 4 bytes in sep buffer
            if(sep_i + 3 < sep_buff_l){
                // get pdu l
                next_pdu_l = (sep_buff[sep_i] 	<< 24) 	|
                    (sep_buff[sep_i + 1] 	<< 16)	|
                    (sep_buff[sep_i + 2] 	<< 8) 	|
                     sep_buff[sep_i + 3];
                // set pdu bytes received
                next_pdu_rb = 4;
                // fwd index
                sep_i += 4;
                // pdu gets processed in next iteration
                continue;
            }
        }
        // fwd index
        ++sep_i;
    }
    // calculate new length of reassembly buffer
    sep_buff_l = sep_buff_p - pdu_end_p;
    // update reassembly buff pointer
    sep_buff_p = sep_buff + sep_buff_l;
    // move remaining data in reassembly buffer to begining
    memmove(sep_buff, pdu_end_p, sep_buff_l);
    // ok
    return 0;

}

void smpp::TcpFraming::reset(){
    // reset reassembly data
    sep_buff_p = sep_buff;
    sep_buff_l = 0;
    next_pdu_l = 0;
    next_pdu_rb = 0;
}

// smpp decode
int smpp::decode(unsigned char* data, unsigned int data_length, SmppPDU* pdu, SMPPPool* smpp_pool){
    // sanity check
    if(data == NULL || pdu == NULL || smpp_pool == NULL || data_length < 16) return 1;
    // length
    pdu->command_length = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    if(pdu->command_length != data_length) return 1;
    data += 4;
    // type
    pdu->command_id = (SmppCommandId)((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    data += 4;
    // status
    pdu->command_status = (SmppCommandStatus)((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    data += 4;
    // sequence
    pdu->sequence_number = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    data += 4;
    // command
    if(pdu->command_length >= 16){
	pdu->command = smpp_pool->request_command(pdu->command_id);
	if(pdu->command != NULL){
	    pdu->command->byte_pos = 0;
	    pdu->command->params.clear();
	    if(pdu->command->init(data, data_length - 16, smpp_pool) != 0) return 2;
	}else return 3;
    }else return 4;

    // ok
    return 0;
}


