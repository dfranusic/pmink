#include <iostream>
#include <smstpdu.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

// TPDUParam
smstpdu::TPDUParam::TPDUParam(TPDUParamType _type): length(0),
						    type(_type),
						    value(NULL){

}

smstpdu::TPDUParam::~TPDUParam(){

}

int smstpdu::TPDUParam::init(unsigned char* data, int _data_length){
    value = data;
    length = _data_length;
    return 0;
}

void smstpdu::TPDUParam::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    buffer += offset;
    memcpy(buffer, value, length);
    *result_length = length;

}

// AddressField
smstpdu::AddressField::AddressField(TPDUParamType _type): TPDUParam(_type),
							  length_digits(0),
							  type_of_number(TON_UNKNOWN),
							  numbering_plan(NP_UNKNOWN),
							  address_value(NULL){

}

int smstpdu::AddressField::init(unsigned char* data, int data_length){
    if(data == NULL || data_length < 2) return 1;
    length_digits = data[0];
    type_of_number = (TypeOfNumber)(data[1] & 0x70);
    numbering_plan = (NumberingPlan)(data[1] & 0x0f);
    address_value = &data[2];

    TPDUParam::init(data, 2 + (int)ceil((double)length_digits / 2));

    return 0;
}

void smstpdu::AddressField::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *(buffer++) = length_digits;
    *(buffer++) = 0x80 | type_of_number | numbering_plan;
    unsigned int tmp = ceil((double)length_digits / 2);
    memcpy(buffer, address_value, tmp);
    *result_length = 2 + tmp;
}



// TP_MessageTypeIndicator
smstpdu::TP_MessageTypeIndicator::TP_MessageTypeIndicator(): 	TPDUParam(TPDU_PT_TP_MTI),
								mti_type(TPMTIT_UNKNOWN){

}

int smstpdu::TP_MessageTypeIndicator::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    switch(*data & 0x03){
	case 0:
	    mti_type = TPMTIT_SMS_DELIVER;
	    break;

	case 1:
	    mti_type = TPMTIT_SMS_SUBMIT;
	    break;

	default:
	    mti_type = TPMTIT_UNKNOWN;
	    break;
    }

    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_MessageTypeIndicator::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer |= mti_type;
    *result_length = 1;

}


// TP_MoreMessagestoSend
smstpdu::TP_MoreMessagestoSend::TP_MoreMessagestoSend(): 	TPDUParam(TPDU_PT_TP_MMS),
								msg_waiting(false){

}

int smstpdu::TP_MoreMessagestoSend::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    msg_waiting = (*data & 0x04) == 0x04;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_MoreMessagestoSend::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer |= (msg_waiting << 2);
    *result_length = 1;

}


// TP_ValidityPeriodFormat
smstpdu::TP_ValidityPeriodFormat::TP_ValidityPeriodFormat():	TPDUParam(TPDU_PT_TP_VPF),
								vpf_type(TPVPFT_NOT_PRESENT){

}

int smstpdu::TP_ValidityPeriodFormat::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    vpf_type = (TPVPFType)(*data & 0x18);
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_ValidityPeriodFormat::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer |= vpf_type;
    *result_length = 1;

}

// TP_StatusReportIndication
smstpdu::TP_StatusReportIndication::TP_StatusReportIndication(): TPDUParam(TPDU_PT_TP_SRI),
								 status_report(false){

}

int smstpdu::TP_StatusReportIndication::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    status_report = (*data & 0x20) == 0x20;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_StatusReportIndication::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer |= (status_report << 5);
    *result_length = 1;

}

// TP_StatusReportRequest
smstpdu::TP_StatusReportRequest::TP_StatusReportRequest():	TPDUParam(TPDU_PT_TP_SRR),
								status_requested(false){

}

int smstpdu::TP_StatusReportRequest::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    status_requested = (*data & 0x20) == 0x20;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_StatusReportRequest::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer |= (status_requested << 5);
    *result_length = 1;

}


// TP_MessageReference
smstpdu::TP_MessageReference::TP_MessageReference():	TPDUParam(TPDU_PT_TP_MR),
							reference_number(0){

}

int smstpdu::TP_MessageReference::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    reference_number = *data;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_MessageReference::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = reference_number;
    *result_length = 1;

}

// TP_OriginatingAddress
smstpdu::TP_OriginatingAddress::TP_OriginatingAddress(): AddressField(TPDU_PT_TP_OA){

}

// TP_DestinationAddress
smstpdu::TP_DestinationAddress::TP_DestinationAddress(): AddressField(TPDU_PT_TP_DA){

}

// TP_ProtocolIdentifier
smstpdu::TP_ProtocolIdentifier::TP_ProtocolIdentifier():	TPDUParam(TPDU_PT_TP_PID),
								format(TPPIDFT_DEFAULT1),
								interworking(TPPIDIT_NO_INTERWORKING_BUT_SME_TO_SME),
								telematic_device(TPPIDTDT_IMPLICIT_SPECIFIC_TO_THIS_SC),
								message_related(TPPIDMRT_SHORT_MESSAGE_TYPE0){

}

int smstpdu::TP_ProtocolIdentifier::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    // format of subsequent bits
    format = (TPPIDFormatType)(*data & 0xc0);
    switch(format){
	case TPPIDFT_DEFAULT1:
	    // telematic interworking
	    interworking = (TPPIDInterworkingType)(*data & 0x20);
	    if(interworking == TPPIDIT_TELEMATIC_INTERWORKING) telematic_device = (TPPIDTelematicDeviceType)(*data & 0x1f);
	    break;

	case TPPIDFT_DEFAULT2:
	    message_related = (TPPIDMessageRelatedType)(*data & 0x3f);
	    break;

	default: break;

    }
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_ProtocolIdentifier::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = format;
    switch(format){
	case TPPIDFT_DEFAULT1:
	    // telematic interworking
	    *buffer |= interworking;
	    if(interworking == TPPIDIT_TELEMATIC_INTERWORKING) *buffer |= telematic_device;
	    break;

	case TPPIDFT_DEFAULT2:
	    *buffer |= message_related;
	    break;

	default: break;


    }
    *result_length = 1;


}

// TP_DataCodingScheme
smstpdu::TP_DataCodingScheme::TP_DataCodingScheme(TPDCSGroupType _dcs_grup): 	TPDUParam(TPDU_PT_TP_DCS),
										dcs_group(_dcs_grup),
										alphabet(TPDCSAT_GSM7){

}

smstpdu::TP_DataCodingScheme::~TP_DataCodingScheme(){

}


// TP_DCSGroups
smstpdu::TP_DataCodingScheme* smstpdu::TP_DCSGroups::get(unsigned char group_byte){
    // check special case
    if(group_byte == 0x00) return &general;

    // check groups with bits 7 and 6
    switch(group_byte & 0xc0){
	case TPDCSGT_GENERAL:
	    return &general;

	case TPDCSGT_AUTOMATIC_DELETION_GROUP:
	    return &general;

    }


    // if not found check groups with bits 7 - 4
    switch(group_byte & 0xf0){
	case TPDCSGT_MESSAGE_WAITING_DISCARD:
	    return &discard;

	case TPDCSGT_MESSAGE_WAITING_STORE:
	    return &store;

	case TPDCSGT_MESSAGE_WAITING_STORE_UCS2:
	    return &store_ucs2;

	case TPDCSGT_DATA_CODING_MESSAGE:
	    return &data_coding;

    }

    return NULL;
}

// TP_DCSGeneral
smstpdu::TP_DCSGeneral::TP_DCSGeneral(): 	TP_DataCodingScheme(TPDCSGT_GENERAL),
						compressed(false),
						has_message_class(false),
						message_class(TPDCSMCT_CLASS0){

}

int smstpdu::TP_DCSGeneral::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;

    //NOTE: The special case of bits 7-0 being 0000 0000 indicates
    //the Default GSM Alphabet.
    if(*data == 0x00){
	    compressed = false;
	    has_message_class = false;
	    alphabet = TPDCSAT_GSM7;
    }else{
	// bit 5
	compressed = (*data & 0x20) == 0x20;
	// bit 4
	has_message_class = (*data & 0x10) == 0x10;
	// bits 3, 2
	alphabet = (TPDCSAlphabetType)(*data & 0x0C);
	// bits 1, 0
	if(has_message_class) message_class = (TPDCSMessageClassType)(*data & 0x03);

    }

    return TP_DataCodingScheme::init(data, data_length);
}

void smstpdu::TP_DCSGeneral::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    // special case
    if(!compressed && !has_message_class && alphabet == TPDCSAT_GSM7) {
	*buffer = 0x00;
	*result_length = 1;
	return;

    }else{
	*buffer = dcs_group;
	*buffer |= (compressed ? 0x20 : 0);
	*buffer |= (has_message_class ? 0x10 : 0);
	*buffer |= alphabet;
	*result_length = 1;

    }


}

// TP_DCSDataCoding
smstpdu::TP_DCSDataCoding::TP_DCSDataCoding(): 	TP_DataCodingScheme(TPDCSGT_DATA_CODING_MESSAGE),
						message_class(TPDCSMCT_CLASS0){

}

int smstpdu::TP_DCSDataCoding::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    // bit 3 reserved
    // bit 2
    alphabet = (TPDCSAlphabetType)(*data & 0x04);
    // bits 1, 0
    message_class = (TPDCSMessageClassType)(*data & 0x03);
    return TP_DataCodingScheme::init(data, data_length);

}

void smstpdu::TP_DCSDataCoding::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = dcs_group;
    *buffer |= alphabet;
    *buffer |= message_class;
    *result_length = 1;


}

// TP_DCSDiscard
smstpdu::TP_DCSDiscard::TP_DCSDiscard():	TP_DataCodingScheme(TPDCSGT_MESSAGE_WAITING_DISCARD),
						indication(false),
						indication_type(TPDCSITT_VOICE_MAIL_MESSAGE_WAITING){

}

int smstpdu::TP_DCSDiscard::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    // bit 3
    indication = (*data & 0x08) == 0x08;
    // bit 2 reserved
    // bits 1, 0
    indication_type = (TPDCSIndicationTypeType)(*data & 0x03);
    alphabet = TPDCSAT_GSM7;
    return TP_DataCodingScheme::init(data, data_length);

}

void smstpdu::TP_DCSDiscard::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = dcs_group;
    *buffer |= (indication << 3);
    *buffer |= indication_type;
    *result_length = 1;


}

// TP_DCSStore
smstpdu::TP_DCSStore::TP_DCSStore():	TP_DataCodingScheme(TPDCSGT_MESSAGE_WAITING_STORE),
						indication(false),
						indication_type(TPDCSITT_VOICE_MAIL_MESSAGE_WAITING){

}

int smstpdu::TP_DCSStore::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    // bit 3
    indication = (*data & 0x08) == 0x08;
    // bit 2 reserved
    // bits 1, 0
    indication_type = (TPDCSIndicationTypeType)(*data & 0x03);
    alphabet = TPDCSAT_GSM7;
    return TP_DataCodingScheme::init(data, data_length);

}

void smstpdu::TP_DCSStore::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = dcs_group;
    *buffer |= (indication << 3);
    *buffer |= indication_type;
    *result_length = 1;


}


// TP_DCSStoreUCS2
smstpdu::TP_DCSStoreUCS2::TP_DCSStoreUCS2():	TP_DataCodingScheme(TPDCSGT_MESSAGE_WAITING_STORE_UCS2),
						indication(false),
						indication_type(TPDCSITT_VOICE_MAIL_MESSAGE_WAITING){

}

int smstpdu::TP_DCSStoreUCS2::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    // bit 3
    indication = (*data & 0x08) == 0x08;
    // bit 2 reserved
    // bits 1, 0
    indication_type = (TPDCSIndicationTypeType)(*data & 0x03);
    alphabet = TPDCSAT_UCS2;
    return TP_DataCodingScheme::init(data, data_length);

}

void smstpdu::TP_DCSStoreUCS2::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = dcs_group;
    *buffer |= (indication << 3);
    *buffer |= indication_type;
    *result_length = 1;


}

// TP_ServiceCentreTimeStamp
smstpdu::TP_ServiceCentreTimeStamp::TP_ServiceCentreTimeStamp(): 	TPDUParam(TPDU_PT_TP_SCTS),
									timestamp(0),
									gmt_offset(0){

}



int smstpdu::TP_ServiceCentreTimeStamp::init(unsigned char* data, int data_length){
    if(data == NULL || data_length < 7) return 1;
    // get local time
    time_t now = time(NULL);
    tm tm_new, tmnow;
    localtime_r(&now, &tmnow);
    // decode semi octets
    tm_new.tm_year = (2000 + decode_semi_octet_digit(data[0])) - 1900;
    tm_new.tm_mon = decode_semi_octet_digit(data[1]) - 1;
    tm_new.tm_mday = decode_semi_octet_digit(data[2]);
    tm_new.tm_hour = decode_semi_octet_digit(data[3]);
    tm_new.tm_min = decode_semi_octet_digit(data[4]);
    tm_new.tm_sec = decode_semi_octet_digit(data[5]);
    tm_new.tm_isdst = tmnow.tm_isdst;

    // get unix timestamp
    timestamp = mktime(&tm_new);

    // decode time zone
    uint8_t tzone = ((data[6] & 0x07) * 10) + ((data[6] & 0xf0) >> 4);
    int zone_dir = ((data[6] & 0x08) == 0x08 ? -1 : 1);

    // set gmt offset
    gmt_offset = (tzone * 15 / 60) * zone_dir;

    return TPDUParam::init(data, data_length);


}

void smstpdu::TP_ServiceCentreTimeStamp::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    tm new_tm;
    localtime_r(&timestamp, &new_tm);
    *(buffer++) = encode_semi_octet_digit((new_tm.tm_year + 1900) % 100);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_mon + 1);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_mday);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_hour);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_min);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_sec);
    // time zone
    *buffer = encode_semi_octet_digit((gmt_offset * 60) / 15);
    if(gmt_offset < 0) *buffer |= 0x08;

    // result size
    *result_length = 7;

}

// TP_ValidityPeriod
smstpdu::TP_ValidityPeriod::TP_ValidityPeriod(): 	TPDUParam(TPDU_PT_TP_VP),
							timestamp(0),
							gmt_offset(0),
							vpf_type(TPVPFT_NOT_PRESENT){

}

int smstpdu::TP_ValidityPeriod::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;

    unsigned int minutes;
    time_t now;
    tm tm_new = {0};
    tm tmnow = {0};
    switch(vpf_type){
	case TPVPFT_NOT_PRESENT:
	    TPDUParam::init(NULL, 0);
	    break;

	case TPVPFT_RELATIVE:
	    //(TP-VP+1) x 5min (i.e. 5min intervals up to 12hrs)
	    if(*data >= 0x00 && *data <= 0x8F){
		minutes = (*data + 1) * 5;
	    //12hr+((TP-VP-143) x 30min)
	    }else if(*data >= 0x90 && *data <= 0xA7){
		minutes = 720 + ((*data - 143) * 30);
	    //(TPVP-166) x 1day
	    }else if(*data >= 0xA8 && *data <= 0xC4){
		minutes = (*data - 166) * 1440;
	    //(TP-VP-192) x 1week
	    }else if(*data >= 0xC5 && *data <= 0xFF){
		minutes = (*data - 192) * 10080;
	    }
	    // get local time
	    now = time(NULL);
	    localtime_r(&now, &tmnow);
	    // add minutes
	    tmnow.tm_min += minutes;
	    // get unix timestamp
	    timestamp = mktime(&tmnow);
	    // update gmt offset
	    gmt_offset = tmnow.tm_gmtoff / 3600;

	    TPDUParam::init(data, 1);
	    break;

	case TPVPFT_ABSOLUTE:
	    if(data_length >= 7){
		// get local time
		now = time(NULL);
		localtime_r(&now, &tmnow);
		// decode semi octets
		tm_new.tm_year = (2000 + decode_semi_octet_digit(data[0])) - 1900;
		tm_new.tm_mon = decode_semi_octet_digit(data[1]) - 1;
		tm_new.tm_mday = decode_semi_octet_digit(data[2]);
		tm_new.tm_hour = decode_semi_octet_digit(data[3]);
		tm_new.tm_min = decode_semi_octet_digit(data[4]);
		tm_new.tm_sec = decode_semi_octet_digit(data[5]);
		tm_new.tm_isdst = tmnow.tm_isdst;
		// get unix timestamp
		timestamp = mktime(&tm_new);
		// decode time zone
		uint8_t tzone = ((data[6] & 0x07) * 10) + ((data[6] & 0xf0) >> 4);
		int zone_dir = ((data[6] & 0x08) == 0x08 ? -1 : 1);
		// set gmt offset
		gmt_offset = (tzone * 15 / 60) * zone_dir;

		TPDUParam::init(data, 7);
	    }
	    break;

	case TPVPFT_ENHANCED:
	    // TODO not used for now
	    break;

    }

    return 0;
}

void smstpdu::TP_ValidityPeriod::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;

    tm new_tm;
    time_t diff;

    // encode
    switch(vpf_type){
	case TPVPFT_RELATIVE:
	    // validity in minutes
	    diff = (timestamp - time(NULL)) / 60;
	    //(TP-VP+1) x 5min (i.e. 5min intervals up to 12hrs)
	    if(diff <= 720 ){
		*buffer = (diff / 5) - 1;
	    //12hr+((TP-VP-143) x 30min)
	    }else if(diff <= 1440){
		*buffer = (diff + 3570) / 30;
	    //(TPVP-166) x 1day
	    }else if(diff <= 43200){
		*buffer = (diff + 239040) / 1440;
	    //(TP-VP-192) x 1week
	    }else if(diff <= 635040){
		*buffer = (diff + 1935360) / 10080;
	    }

	    // result length
	    *result_length = 1;
	    break;

	case TPVPFT_ABSOLUTE:
	    // offset
	    buffer += offset;
	    // encode
	    localtime_r(&timestamp, &new_tm);
	    *(buffer++) = encode_semi_octet_digit((new_tm.tm_year + 1900) % 100);
	    *(buffer++) = encode_semi_octet_digit(new_tm.tm_mon + 1);
	    *(buffer++) = encode_semi_octet_digit(new_tm.tm_mday);
	    *(buffer++) = encode_semi_octet_digit(new_tm.tm_hour);
	    *(buffer++) = encode_semi_octet_digit(new_tm.tm_min);
	    *(buffer++) = encode_semi_octet_digit(new_tm.tm_sec);
	    // time zone
	    *buffer = encode_semi_octet_digit((gmt_offset * 60) / 15);
	    if(gmt_offset < 0) *buffer |= 0x08;
	    // result size
	    *result_length = 7;
	    break;

	case TPVPFT_ENHANCED:
	    // TODO not used for now
	    break;

	default: break;


    }

}

// TP_DischargeTime
smstpdu::TP_DischargeTime::TP_DischargeTime():	TPDUParam(TPDU_PT_TP_DT),
						timestamp(0),
						gmt_offset(0){
}

int smstpdu::TP_DischargeTime::init(unsigned char* data, int data_length){
    if(data == NULL || data_length < 7) return 1;
    // get local time
    time_t now = time(NULL);
    tm tm_new, tmnow;
    localtime_r(&now, &tmnow);
    // decode semi octets
    tm_new.tm_year = (2000 + decode_semi_octet_digit(data[0])) - 1900;
    tm_new.tm_mon = decode_semi_octet_digit(data[1]) - 1;
    tm_new.tm_mday = decode_semi_octet_digit(data[2]);
    tm_new.tm_hour = decode_semi_octet_digit(data[3]);
    tm_new.tm_min = decode_semi_octet_digit(data[4]);
    tm_new.tm_sec = decode_semi_octet_digit(data[5]);
    tm_new.tm_isdst = tmnow.tm_isdst;

    // get unix timestamp
    timestamp = mktime(&tm_new);

    // decode time zone
    uint8_t tzone = ((data[6] & 0x07) * 10) + ((data[6] & 0xf0) >> 4);
    int zone_dir = ((data[6] & 0x08) == 0x08 ? -1 : 1);

    // set gmt offset
    gmt_offset = (tzone * 15 / 60) * zone_dir;

    return TPDUParam::init(data, data_length);



}

void smstpdu::TP_DischargeTime::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    tm new_tm;
    localtime_r(&timestamp, &new_tm);
    *(buffer++) = encode_semi_octet_digit((new_tm.tm_year + 1900) % 100);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_mon + 1);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_mday);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_hour);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_min);
    *(buffer++) = encode_semi_octet_digit(new_tm.tm_sec);
    // time zone
    *buffer = encode_semi_octet_digit((gmt_offset * 60) / 15);
    if(gmt_offset < 0) *buffer |= 0x08;

    // result size
    *result_length = 7;

}


// TP_RecipientAddress
smstpdu::TP_RecipientAddress::TP_RecipientAddress(): AddressField(TPDU_PT_TP_RA){

}

// TP_Status
smstpdu::TP_Status::TP_Status(): 	TPDUParam(TPDU_PT_TP_ST),
					status_used(false),
					status(TPSTST_SM_RECEIVED_BY_SME){

}

int smstpdu::TP_Status::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    status_used = (*data & 0x80) == 0x80;
    if(status_used) status = (TPSTStatusType)(*data & 0x7f);
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_Status::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = (status_used << 7);
    if(status_used) *buffer |= status;


}

// TP_UserDataLength
smstpdu::TP_UserDataLength::TP_UserDataLength(): 	TPDUParam(TPDU_PT_TP_UDL),
							udl(0){

}

int smstpdu::TP_UserDataLength::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    udl = *data;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_UserDataLength::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = udl;
    *result_length = 1;


}

// TP_ReplyPath
smstpdu::TP_ReplyPath::TP_ReplyPath(): 	TPDUParam(TPDU_PT_TP_RP),
					rp_set(false){

}

int smstpdu::TP_ReplyPath::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    rp_set = (*data & 0x80) == 0x80;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_ReplyPath::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer |= (rp_set << 7);
    *result_length = 1;

}

// TP_MessageNumber
smstpdu::TP_MessageNumber::TP_MessageNumber(): 	TPDUParam(TPDU_PT_TP_MN),
						msg_number(0){

}

int smstpdu::TP_MessageNumber::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    msg_number = *data;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_MessageNumber::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = msg_number;
    *result_length = 1;

}

// TP_CommandType
smstpdu::TP_CommandType::TP_CommandType(): 	TPDUParam(TPDU_PT_TP_CT),
						operation_type(TPCTOT_PREV_SUBM_MSG_RELATED){

}

int smstpdu::TP_CommandType::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    operation_type = (TPCTOpTypeType)*data;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_CommandType::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = operation_type;
    *result_length = 1;

}

// TP_CommandDataLength
smstpdu::TP_CommandDataLength::TP_CommandDataLength(): 	TPDUParam(TPDU_PT_TP_CDL),
							cmd_length(0){

}

int smstpdu::TP_CommandDataLength::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    cmd_length = *data;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_CommandDataLength::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = cmd_length;
    *result_length = 1;

}

// TP_CommandData
smstpdu::TP_CommandData::TP_CommandData(): TPDUParam(TPDU_PT_TP_CD){

}


// TP_FailureCause
smstpdu::TP_FailureCause::TP_FailureCause(): 	TPDUParam(TPDU_PT_TP_FCS),
						err_type(TPFCSET_UNSPECIFIED_ERR){

}

int smstpdu::TP_FailureCause::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    err_type = (TPFCSErrorType)*data;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_FailureCause::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = err_type;
    *result_length = 1;

}

// TP_UserDataHeaderIndicator
smstpdu::TP_UserDataHeaderIndicator::TP_UserDataHeaderIndicator(): 	TPDUParam(TPDU_PT_TP_UDHI),
									udh_present(false){

}

int smstpdu::TP_UserDataHeaderIndicator::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    udh_present = (*data & 0x40) == 0x40;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_UserDataHeaderIndicator::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer |= (udh_present << 6);
    *result_length = 1;

}

// TP_UDHInformationElement
smstpdu::TP_UDHInformationElement::TP_UDHInformationElement(TPUDHIEType _type): TPDUParam(TPDU_PT_TP_UDH_IE),
										ie_type(_type){

}

int smstpdu::TP_UDHInformationElement::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    ie_type = (TPUDHIEType)*(data++);
    int l = *(data++);
    return TPDUParam::init(data, l);

}

void smstpdu::TP_UDHInformationElement::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *(buffer++) = ie_type;
    *(buffer++) = length - 2;
    memcpy(buffer, value, length - 2);
    *result_length = length;

}

// TP_UDHIEConc8BitRef
smstpdu::TP_UDHIEConc8BitRef::TP_UDHIEConc8BitRef(): 	TP_UDHInformationElement(TPUDHIET_CONCATENATED_8BIT_REF),
							msg_id(0),
							msg_parts(0),
							msg_part(0){

}

int smstpdu::TP_UDHIEConc8BitRef::init(unsigned char* data, int data_length){
    if(data == NULL || data_length < 5) return 1;
    ie_type = (TPUDHIEType)*(data++);
    length = *(data++);
    value = data;
    msg_id = *(data++);
    msg_parts = *(data++);
    msg_part = *data;
    return 0;

}

void smstpdu::TP_UDHIEConc8BitRef::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *(buffer++) = ie_type;
    *(buffer++) = 3;
    *(buffer++) = msg_id;
    *(buffer++) = msg_parts;
    *buffer = msg_part;
    *result_length = 5;

}


// TP_UDHIEConc16BitRef
smstpdu::TP_UDHIEConc16BitRef::TP_UDHIEConc16BitRef(): 	TP_UDHInformationElement(TPUDHIET_CONCATENATED_16BIT_REF),
							msg_id(0),
							msg_parts(0),
							msg_part(0){

}

int smstpdu::TP_UDHIEConc16BitRef::init(unsigned char* data, int data_length){
    if(data == NULL || data_length < 6) return 1;
    ie_type = (TPUDHIEType)*(data++);
    length = *(data++);
    value = data;
    msg_id = (data[0] << 8) | data[1];
    data += 2;
    msg_parts = *(data++);
    msg_part = *data;
    return 0;

}

void smstpdu::TP_UDHIEConc16BitRef::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *(buffer++) = ie_type;
    *(buffer++) = 4;
    *(buffer++) = msg_id >> 8;
    *(buffer++) = msg_id;
    *(buffer++) = msg_parts;
    *buffer = msg_part;
    *result_length = 6;

}




// TP_UserDataHeader
smstpdu::TP_UserDataHeader::TP_UserDataHeader(int ie_pool_size): TPDUParam(TPDU_PT_TP_UDH){
    //ie_lst.reserve(10);
    ie_pool.set_pool_size(ie_pool_size);
    ie_pool.init_pool();
}


int smstpdu::TP_UserDataHeader::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;

    // get UDH data (first byte contains length)
    TPDUParam::init(data, *data + 1);
    ++data;

    // tmp vars
    int l = 0;
    TP_UDHInformationElement* ie = NULL;

    // clear
    ie_lst.clear();

    // data end
    unsigned char* data_end = data + length - 1;

    // get IE elements
    while(data < data_end){
	switch(data[0]){
	    // 8bit ref
	    case TPUDHIET_CONCATENATED_8BIT_REF:
		l = data[1];
		ie = ie_pool.request_ie(TPUDHIET_CONCATENATED_8BIT_REF);
		ie->init(&data[0], l + 2);
		ie_lst.push_back(ie);
		break;

	    // 16bit ref
	    case TPUDHIET_CONCATENATED_16BIT_REF:
		l = data[1];
		ie = ie_pool.request_ie(TPUDHIET_CONCATENATED_16BIT_REF);
		ie->init(&data[0], l + 2);
		ie_lst.push_back(ie);
		break;

	    // other
	    default:
		// at the moment other IEs are not relevant
		// skip them
		l = data[1];
		break;

	}
	// next IE
	data += l + 2;
    }

    return 0;

}

void smstpdu::TP_UserDataHeader::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    int tmp_l = 0;
    // local offset
    offset = 1;
    *result_length = 1; // length byte (will be filled later)
    // encode IEs
    for(unsigned int i = 0; i<ie_lst.size(); i++){
	ie_lst[i]->encode(buffer, buffer_length, offset, &tmp_l);
	//offset += tmp_l;
	offset += tmp_l;
	*result_length += tmp_l;
    }
    // total length minus length byte
    *buffer = *result_length - 1;
}



// TP_UserData
smstpdu::TP_UserData::TP_UserData(): TPDUParam(TPDU_PT_TP_UD){

}

// TP_RejectDuplicates
smstpdu::TP_RejectDuplicates::TP_RejectDuplicates(): 	TPDUParam(TPDU_PT_TP_RD),
							reject(false){

}

int smstpdu::TP_RejectDuplicates::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    reject = (*data & 0x04) == 0x04;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_RejectDuplicates::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer |= (reject << 2);
    *result_length = 1;

}


// TP_StatusReportQualifier
smstpdu::TP_StatusReportQualifier::TP_StatusReportQualifier(): 	TPDUParam(TPDU_PT_TP_SRQ),
								result_of(TPSRQT_RES_OF_SUBMIT){

}

int smstpdu::TP_StatusReportQualifier::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    result_of = (TPSRQType)*data;
    return TPDUParam::init(data, data_length);

}

void smstpdu::TP_StatusReportQualifier::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer |= (result_of << 5);
    *result_length = 1;

}

// TP_ParameterIndicator
smstpdu::TP_ParameterIndicator::TP_ParameterIndicator(): 	TPDUParam(TPDU_PT_TP_PI),
								tp_udl(false),
								tp_dcs(false),
								tp_pid(false){

}

int smstpdu::TP_ParameterIndicator::init(unsigned char* data, int data_length){
    if(data == NULL || data_length <= 0) return 1;
    // set initial data length to 0
    TPDUParam::init(data, 0);

    // get TP-PI octets (bit 7 = extension)
    // currently only first octet is supported
    do{
	tp_pid = (*data & 0x01) == 0x01;
	tp_dcs = (*data & 0x02) == 0x02;
	tp_udl = (*data & 0x04) == 0x04;
	// inc data length
	++length;
    }while((*(data++) & 0x80) == 0x80);

    return 0;

}

void smstpdu::TP_ParameterIndicator::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    *buffer = 0x80;
    *buffer |= tp_pid;
    *buffer |= (tp_dcs << 1);
    *buffer |= (tp_udl << 2);
    *result_length = 1;

}

// IEPoolItem
smstpdu::IEPoolItem::IEPoolItem(): pool(NULL),
				   next_free_item(NULL),
				   total_count(0),
				   free_count(0){

}

smstpdu::IEPoolItem::~IEPoolItem(){
    for(int i = 0; i<total_count; i++) delete pool[i];
    delete[] pool;

}

smstpdu::TP_UDHInformationElement* smstpdu::IEPoolItem::create_param(TPUDHIEType _ie_type){
    TP_UDHInformationElement* ie = NULL;
    switch(_ie_type){
	case TPUDHIET_CONCATENATED_8BIT_REF:
	    ie = new TP_UDHIEConc8BitRef();
	    break;

	case TPUDHIET_CONCATENATED_16BIT_REF:
	    ie = new TP_UDHIEConc16BitRef();
	    break;

	default: break;
    }
    return ie;
}

smstpdu::TP_UDHInformationElement* smstpdu::IEPoolItem::request_item(){
	TP_UDHInformationElement* tmp = next_free_item;
	--free_count;
	if(free_count <= 0) free_count = total_count;
	next_free_item = pool[free_count - 1];

	return tmp;
}

void smstpdu::IEPoolItem::init_pool(){
	pool = new TP_UDHInformationElement *[total_count];
	for(int i = 0; i<total_count; i++) pool[i] = create_param(type);
	next_free_item = pool[total_count - 1];
	//free_count--;
}

void smstpdu::IEPoolItem::set_pool_size(int _total_count){
	total_count = _total_count;
	free_count = total_count;
}

// IEPool
smstpdu::IEPool::IEPool(): ie_count(5){

}

smstpdu::IEPool::~IEPool(){
    IEPoolItem* tmp_ie = NULL;
    std::map<TPUDHIEType, IEPoolItem*>::iterator it = IE_POOL.begin();
    for(; it != IE_POOL.end(); it++){
	tmp_ie = it->second;
	delete tmp_ie;
    }

}


void smstpdu::IEPool::init_ie(TPUDHIEType _ie_type){
	IE_POOL[_ie_type] = new IEPoolItem();
	IE_POOL[_ie_type]->type = _ie_type;
	IE_POOL[_ie_type]->set_pool_size(ie_count);
	IE_POOL[_ie_type]->init_pool();
}

void smstpdu::IEPool::set_pool_size(int _ie_count){
	ie_count = _ie_count;
}

void smstpdu::IEPool::init_pool(){
	// ie pool
	init_ie(TPUDHIET_CONCATENATED_8BIT_REF);
	init_ie(TPUDHIET_CONCATENATED_16BIT_REF);

}

smstpdu::TP_UDHInformationElement* smstpdu::IEPool::request_ie(TPUDHIEType ie_type){
    std::map<TPUDHIEType, IEPoolItem*>::iterator it = IE_POOL.find(ie_type);
    if(it != IE_POOL.end()) return it->second->request_item();
    return NULL;
}



// TPDU
smstpdu::TPDU::TPDU(TPDUType _type): type(_type){

}

smstpdu::TPDU::~TPDU(){

}

smstpdu::TPDUType smstpdu::TPDU::detect(unsigned char first_byte, TPDUDirectionType direction){
   switch(first_byte & 0x03){
       case TPMTIT_SMS_DELIVER:
	   if(direction == TPDUD_SC_MS) return TPDUT_SMS_DELIVER;
	   else return TPDUT_SMS_DELIVER_REPORT;

       case TPMTIT_SMS_SUBMIT:
	   if(direction == TPDUD_MS_SC) return TPDUT_SMS_SUBMIT;
	   else return TPDUT_SMS_SUBMIT_REPORT;

   }
   return TPDUT_UNKNOWN;
}



// SMS_Deliver
smstpdu::SMS_Deliver::SMS_Deliver(): 	TPDU(TPDUT_SMS_DELIVER),
					tp_dcs(NULL){

}

int smstpdu::SMS_Deliver::init(unsigned char* data, int data_length){
    if(data == NULL || data_length < 14) return 1;
    // TP-MTI
    tp_mti.init(data, 1);
    if(tp_mti.mti_type != TPMTIT_SMS_DELIVER) return 2;

    // TP-MMS
    tp_mms.init(data, 1);
    // TP-RP
    tp_rp.init(data, 1);
    // TP-UDHI
    tp_udhi.init(data, 1);
    // TP-SRI
    tp_sri.init(data++, 1);
    --data_length;

    // TP-OA
    if(tp_oa.init(data, data_length) != 0) return 3;
    data += tp_oa.length;
    data_length -= tp_oa.length;
    // err check
    if(data_length <= 0) return 1;

    // TP-PID
    tp_pid.init(data++, 1);
    --data_length;
    // err check
    if(data_length <= 0) return 1;

    // TP-DCS
    tp_dcs = dcs_groups.get(*data);
    if(tp_dcs != NULL) tp_dcs->init(data, 1);
    ++data;
    --data_length;
    // err check
    if(data_length <= 0) return 1;

    // TP-SCTS
    tp_scts.init(data, 7);
    data += 7;
    data_length -= 7;
    // err check
    if(data_length <= 0) return 1;

    // TP-UDL
    tp_udl.init(data, 1);
    ++data;
    --data_length;

    // TP-UDL length check
    if(tp_udl.udl > 0){
	// err check
	if(data_length <= 0) return 1;
	// TP-UD
	tp_ud.init(data, data_length);
	// check if UDH needs to be decoded
	if(tp_udhi.udh_present) tp_ud.udh.init(tp_ud.value, tp_ud.length);
    }

    // ok
    return 0;



}

void smstpdu::SMS_Deliver::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    int tmp_l = 0;
    // TP-MTI
    tp_mti.encode(buffer, buffer_length, offset, &tmp_l);
    // TP-MMS
    tp_mms.encode(buffer, buffer_length, offset, &tmp_l);
    // TP-RP
    tp_rp.encode(buffer, buffer_length, offset, &tmp_l);
    // TP-UDHI
    tp_udhi.encode(buffer, buffer_length, offset, &tmp_l);
    // TP-SRI
    tp_sri.encode(buffer, buffer_length, offset, &tmp_l);
    *result_length = tmp_l;
    ++offset;

    // TP-OA
    tp_oa.encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-PID
    tp_pid.encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-DCS
    tp_dcs->encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-SCTS
    tp_scts.encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-UDL
    tp_udl.encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-UD
    // encode UDH first if included
    if(tp_udhi.udh_present){
	tp_ud.udh.encode(buffer, buffer_length, offset, &tmp_l);
	offset += tmp_l;
	*result_length += tmp_l;
    }
    // encode sms text
    tp_ud.encode(buffer, buffer_length, offset, &tmp_l);
    *result_length += tmp_l;

}

smstpdu::TP_DataCodingScheme* smstpdu::SMS_Deliver::get_dcs_group(TPDCSGroupType dcs_group){
    return dcs_groups.get(dcs_group);
}


// SMS_Submit
smstpdu::SMS_Submit::SMS_Submit(): 	TPDU(TPDUT_SMS_SUBMIT),
					tp_dcs(NULL){

}

int smstpdu::SMS_Submit::init(unsigned char* data, int data_length){
    if(data == NULL || data_length < 9) return 1;
    // TP-MTI
    tp_mti.init(data, 1);
    if(tp_mti.mti_type != TPMTIT_SMS_SUBMIT) return 2;

    // TP-RD
    tp_rd.init(data, 1);
    // TP-VPF
    tp_vpf.init(data, 1);
    // TP-RP
    tp_rp.init(data, 1);
    // TP-UDHI
    tp_udhi.init(data, 1);
    // TP-SRR
    tp_srr.init(data++, 1);
    --data_length;

    // TP-MR
    tp_mr.init(data, 1);
    data++;
    --data_length;

    // TP-DA
    if(tp_da.init(data, data_length) != 0) return 3;
    data += tp_da.length;
    data_length -= tp_da.length;
    // err check
    if(data_length <= 0) return 1;

    // TP-PID
    tp_pid.init(data++, 1);
    --data_length;
    // err check
    if(data_length <= 0) return 1;

    // TP-DCS
    tp_dcs = dcs_groups.get(*data);
    if(tp_dcs != NULL) tp_dcs->init(data, 1);
    ++data;
    --data_length;
    // err check
    if(data_length <= 0) return 1;

    // TP-VP
    tp_vp.vpf_type = tp_vpf.vpf_type;
    tp_vp.init(data, data_length);
    data += tp_vp.length;
    data_length -= tp_vp.length;
    // err check
    if(data_length <= 0) return 1;

    // TP-UDL
    tp_udl.init(data, 1);
    ++data;
    --data_length;

    // TP-UDL length check
    if(tp_udl.udl > 0){
	// err check
	if(data_length <= 0) return 1;
	// TP-UD
	tp_ud.init(data, data_length);
	// check if UDH needs to be decoded
	if(tp_udhi.udh_present) tp_ud.udh.init(tp_ud.value, tp_ud.length);
    }

    // ok
    return 0;

}

void smstpdu::SMS_Submit::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length){
    // offset
    buffer += offset;
    // encode
    int tmp_l = 0;
    // TP-MTI
    tp_mti.encode(buffer, buffer_length, offset, &tmp_l);
    // TP-RD
    tp_rd.encode(buffer, buffer_length, offset, &tmp_l);
    // TP-VPF
    tp_vpf.encode(buffer, buffer_length, offset, &tmp_l);
    // TP-RP
    tp_rp.encode(buffer, buffer_length, offset, &tmp_l);
    // TP-UDHI
    tp_udhi.encode(buffer, buffer_length, offset, &tmp_l);
    // TP-SRR
    tp_srr.encode(buffer, buffer_length, offset, &tmp_l);
    *result_length = tmp_l;
    ++offset;

    // TP-MR
    tp_mr.encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-DA
    tp_da.encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-PID
    tp_pid.encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-DCS
    tp_dcs->encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-VP
    tp_vp.vpf_type = tp_vpf.vpf_type;
    tp_vp.encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-UDL
    tp_udl.encode(buffer, buffer_length, offset, &tmp_l);
    offset += tmp_l;
    *result_length += tmp_l;

    // TP-UD
    // encode UDH first if included
    if(tp_udhi.udh_present){
	tp_ud.udh.encode(buffer, buffer_length, offset, &tmp_l);
	offset += tmp_l;
	*result_length += tmp_l;
    }
    // encode sms text
    tp_ud.encode(buffer, buffer_length, offset, &tmp_l);
    *result_length += tmp_l;


}

smstpdu::TP_DataCodingScheme* smstpdu::SMS_Submit::get_dcs_group(TPDCSGroupType dcs_group){
    return dcs_groups.get(dcs_group);
}


// GSMAlphabet
uint16_t smstpdu::GsmAlphabet::GSM_DEFAULT_ALPHABET[GSM_ALPHABET_SIZE] = {
    /* ETSI GSM 03.38, version 6.0.1, section 6.2.1; Default alphabet */
    /* Using unicode for non ascii characters */
    '@',  0xa3, '$' , 0xa5, 0xe8, 0xe9, 0xf9, 0xec,
    0xf2, 0xc7, '\n', 0xd8, 0xf8, '\r', 0xc5, 0xe5,
   0x394, '_', 0x3a6,0x393,0x39b,0x3a9,0x3a0,0x3a8,
   0x3a3,0x398,0x39e, 0xa0, 0xc6, 0xe6, 0xdf, 0xc9,
    ' ',  '!',  '\"', '#',  0xa4,  '%',  '&',  '\'',
    '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
    '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
    '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
    0xa1, 'A',  'B',  'C',  'D',  'E',  'F',  'G',
    'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
    'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
    'X',  'Y',  'Z',  0xc4, 0xd6, 0xd1, 0xdc, 0xa7,
    0xbf, 'a',  'b',  'c',  'd',  'e',  'f',  'g',
    'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
    'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
    'x',  'y',  'z',  0xe4, 0xf6, 0xf1, 0xfc, 0xe0
};

uint16_t smstpdu::GsmAlphabet::gsm_char_decode(unsigned char val){
    if(val < GSM_ALPHABET_SIZE) return GSM_DEFAULT_ALPHABET[val];
    else return '?';
}



int smstpdu::GsmAlphabet::pack(const char* data,
			       int data_length,
			       int padding_bits,
			       int out_size,
			       unsigned char* output,
			       int *out_used){

    int bits = 0;
    int i;
    unsigned char octet;
    *out_used = 0;

    if(padding_bits){
       bits = 7 - padding_bits;
       *output++ = data[0] << (7 - bits);
       (*out_used) ++;
       bits++;

    }

    for(i = 0; i < data_length; i++){
       if(bits == 7){
          bits = 0;
          continue;

       }

       // buffer overflow
       if(*out_used == out_size) return 1;

       octet = (data[i] & 0x7f) >> bits;

       if(i < data_length - 1) octet |= data[i + 1] << (7 - bits);

       *output++ = octet;

       (*out_used)++;

       bits++;

    }

    // ok
    return 0;

}


int smstpdu::GsmAlphabet::unpack(unsigned int offset,
			         unsigned int in_length,
			         unsigned int out_length,
			         const uint8_t *input,
			         unsigned char *output){
    unsigned char *out_num = output; /* Current pointer to the output buffer */
    const uint8_t *in_num = input;    /* Current pointer to the input buffer */
    unsigned char rest = 0x00;
    int bits;

    bits = offset ? offset : 7;

    while ((unsigned int)(in_num - input) < in_length){
        *out_num = ((*in_num & GN_BYTE_MASK) << (7 - bits)) | rest;
        rest = *in_num >> bits;

        /* If we don't start from 0th bit, we shouldn't go to the
           next char. Under *out_num we have now 0 and under Rest -
           _first_ part of the char. */
        if ((in_num != input) || (bits == 7)) out_num++;
        in_num++;

        if ((unsigned int)(out_num - output) >= out_length) break;

        /* After reading 7 octets we have read 7 full characters but
           we have 7 bits as well. This is the next character */
        if (bits == 1){
            *out_num = rest;
            out_num++;
            bits = 7;
            rest = 0x00;

        }else{
            bits--;
        }
    }

    return (int)(out_num - output);
}



int smstpdu::GsmAlphabet::encode(   const char* data,
				    int data_length,
				    int padding_bits,
				    int out_size,
				    unsigned char* output,
				    int *out_used){


    int bits = 0;
    int i;
    unsigned char octet;
    *out_used = 0;

    if(padding_bits){
       bits = 7 - padding_bits;
       *output++ = data[0] << (7 - bits);
       (*out_used) ++;
       bits++;

    }

    for(i = 0; i < data_length; i++){
       if(bits == 7){
          bits = 0;
          continue;

       }

       // buffer overflow
       if(*out_used == out_size) return 1;

       octet = (data[i] & 0x7f) >> bits;

       if(i < data_length - 1) octet |= data[i + 1] << (7 - bits);

       *output++ = octet;

       (*out_used)++;

       bits++;

    }

    // ok
    return 0;

}





int smstpdu::GsmAlphabet::decode(	const unsigned char* data,
					uint32_t data_length,
					uint32_t septet_count,
					uint16_t* output,
					uint32_t out_size,
					int septet_offset){
    // err check
    if(data == NULL || output == NULL) return 1;

    uint32_t l = 0;
    unsigned char b;
    uint8_t n = 0;
    uint8_t p = 0;
    uint32_t c = 0;
    int buffer_l = out_size / sizeof(uint16_t);
    // loop
    for(unsigned int i = 0; i<data_length; i++){
	// process septets
	p |= (2 << (6 - l)); // (6-l)th power of two
	b = data[i] & ~p;
	if(c++ < septet_count && septet_offset-- <= 0 && buffer_l-- > 0) *(output++) = gsm_char_decode(((b << l) + (n >> (8 - l))));
	n = data[i] & p;
	if(++l == 7){
		l = 0;
		p = 0;
		if(c++ < septet_count && septet_offset-- <= 0 && buffer_l-- > 0) *(output++) = gsm_char_decode(n >> 1);
	}

    }
    // ok
    return 0;
}

uint16_t smstpdu::GsmAlphabet::decode_ext(unsigned char val){
    switch(val){
	case 0x0a: return 0x0c; /* form feed */
	case 0x14: return '^';
	case 0x28: return '{';
	case 0x29: return '}';
	case 0x2f: return '\\';
	case 0x3c: return '[';
	case 0x3d: return '~';
	case 0x3e: return ']';
	case 0x40: return '|';
	case 0x65: return 0x20ac; /* euro */
	default: return '?'; /* invalid character */
    }
}


uint8_t smstpdu::decode_semi_octet_digit(unsigned char data){
    return ((data & 0x0f) * 10) + ((data & 0xf0) >> 4);
}

void smstpdu::decode_tbcd_octets(unsigned char* data, unsigned int data_length, unsigned char* output, unsigned int out_size, bool skip_last){
    // buffer overflow check
    if((data_length * 2) + 1 > out_size) {
	// null character
	*output = 0;
	return;
    }

    char c1, c2;
    for(unsigned int i = 0; i<data_length; i++) {
	// get digits
	c1 = tbcd_decode(data[i] & 0x0f);
	c2 = tbcd_decode((data[i] & 0xf0) >> 4);
	// check for stop
	if(c1 != -1) *(output++) = c1; else { *output = 0; return; }
	if(c2 != -1) *(output++) = c2; else { *output = 0; return; }
    }
    // rewind if skipping last
    if(skip_last) --output;
    // null character
    *output = 0;
}

int smstpdu::encode_tbcd_octets(unsigned char* data, unsigned int data_length, unsigned char* output){
    if(data_length == 0) return 0;
    char c1, c2;
    unsigned int i = 0;
    while(i < data_length - 1){
	c1 = tbcd_encode(data[i]);
	c2 = tbcd_encode(data[i + 1]);
	*(output++) = (c2 << 4) | c1;
	i += 2;
    }

    // last digit
    if(i < data_length){
	c1 = tbcd_encode(data[i]);
	*output = 0xf0 | c1;
    }
    // number of bytes used
    return ceil((double)data_length / 2);
}


unsigned char smstpdu::tbcd_encode(unsigned char val){
    switch(val){
	case '*': return 10;
	case '#': return 11;
	case 'a': return 12;
	case 'b': return 13;
	case 'c': return 14;
	case 0xff: return 15;
	default: return (char)(val - ((int)'0'));
    }
}



char smstpdu::tbcd_decode(unsigned char val){
    switch(val){
	case 10: return '*';
	case 11: return '#';
	case 12: return 'a';
	case 13: return 'b';
	case 14: return 'c';
	case 15: return 0xff;
	default: return (char)(((int)'0') + val);
    }

}




uint8_t smstpdu::encode_semi_octet_digit(unsigned int val){
    if(val > 99) return 0;
    // extract digits
    uint8_t first = val / 10;
    uint8_t second = val % 10;
    // return packed
    return ((second << 4) | first);
}


