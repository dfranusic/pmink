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

#include <rrp.h>
#include <arpa/inet.h>
#include <poll.h>
#include <rrp_events.h>

// RRRouteParam
void rrp::RRRouteParam::reset(){
    params.clear_params();
    count = 0;
}

int rrp::RRRouteParam::decode(const char* str){
/*
RouteNo;
SrcTrunk;
DstTrunk;
Type;
StoreAndForward[1=NotInUse,2=StoreAndForward,3=StoraAndForwardNoInlineSRI];
CgPA;
CdPA;
SSN;
SCOA;
SCDA;
NNN;
IMSI;
MSISDN;
MAPVersion[1,2,3];
MapAutoFallback[0=no,1=yes];
ErrorRemapList;
FilterFirst[0=no,1=yes];
TempErrorList;
PermanentErrorList;
DoHLR[0=no,1=yes];
Timeout[sec];
RouteId[Id iz baze cisto za debug log]
 */
    if(str == NULL) return 1;
    // tmp copy
    char tmp_data[strlen(str) + 1];
    char* tmp_data_p = tmp_data;
    strcpy(tmp_data, str);
    // clear params
    //params.clear_params();
    // tokenize
    char* token;
    int tc = 0;
    // split by ;
    while((token = strsep(&tmp_data_p, ";")) != NULL){
	switch(tc){
	    // RouteNo
	    case 0:
		params.set_int(RRRPT_ROUTE_NO, atoi(token), count);
		params.set_label(RRRPT_ROUTE_NO, "RouteNo");
		break;

	    // SrcTrunk
	    case 1:
		params.set_cstr(RRRPT_SRC_TRUNK, token, count);
		params.set_label(RRRPT_SRC_TRUNK, "SrcTrunk");
		break;

	    // DstTrunk
	    case 2:
		params.set_cstr(RRRPT_DST_TRUNK, token, count);
		params.set_label(RRRPT_DST_TRUNK, "DstTrunk");
		break;

	    // Type
	    case 3:
		params.set_int(RRRPT_TYPE, atoi(token), count);
		params.set_label(RRRPT_TYPE, "Type");
		break;

	    // StoreAndForward
	    case 4:
		params.set_int(RRRPT_STORE_AND_FWD, atoi(token), count);
		params.set_label(RRRPT_STORE_AND_FWD, "StoreAndForward");
		break;

	    // CgPA
	    case 5:
		params.set_cstr(RRRPT_CGPA, token, count);
		params.set_label(RRRPT_CGPA, "CgPA");
		break;

	    // CdPA
	    case 6:
		params.set_cstr(RRRPT_CDPA, token, count);
		params.set_label(RRRPT_CDPA, "CdPA");
		break;

	    // SSN
	    case 7:
		params.set_int(RRRPT_SSN, atoi(token), count);
		params.set_label(RRRPT_SSN, "SSN");
		break;

	    // SCOA
	    case 8:
		params.set_cstr(RRRPT_SCOA, token, count);
		params.set_label(RRRPT_SCOA, "SCOA");
		break;

	    // SCDA
	    case 9:
		params.set_cstr(RRRPT_SCDA, token, count);
		params.set_label(RRRPT_SCDA, "SCDA");
		break;

	    // NNN
	    case 10:
		params.set_cstr(RRRPT_NNN, token, count);
		params.set_label(RRRPT_NNN, "NNN");
		break;

	    // IMSI
	    case 11:
		params.set_cstr(RRRPT_IMSI, token, count);
		params.set_label(RRRPT_IMSI, "IMSI");
		break;

	    // MSISDN
	    case 12:
		params.set_cstr(RRRPT_MSISDN, token, count);
		params.set_label(RRRPT_MSISDN, "MSISDN");
		break;

	    // MAPVersion
	    case 13:
		params.set_int(RRRPT_MAP_VER, atoi(token), count);
		params.set_label(RRRPT_MAP_VER, "MAPVersion");
		break;

	    // MapAutoFallback
	    case 14:
		params.set_int(RRRPT_MAP_AUTO_FB, atoi(token), count);
		params.set_label(RRRPT_MAP_AUTO_FB, "MapAutoFallback");
		break;

	    // ErrorRemapList
	    case 15:
		params.set_cstr(RRRPT_ERR_REMAP_LST, token, count);
		params.set_label(RRRPT_ERR_REMAP_LST, "ErrorRemapList");
		break;

	    // FilterFirst
	    case 16:
		params.set_int(RRRPT_FILTER_FIRST, atoi(token), count);
		params.set_label(RRRPT_FILTER_FIRST, "FilterFirst");
		break;

	    // TempErrorList
	    case 17:
		params.set_cstr(RRRPT_TEMP_ERR_LST, token, count);
		params.set_label(RRRPT_TEMP_ERR_LST, "TempErrorList");
		break;

	    // PermanentErrorList
	    case 18:
		params.set_cstr(RRRPT_PERM_ERR_LST, token, count);
		params.set_label(RRRPT_PERM_ERR_LST, "PermanentErrorList");
		break;

	    // DoHLR
	    case 19:
		params.set_int(RRRPT_DO_HLR, atoi(token), count);
		params.set_label(RRRPT_DO_HLR, "DoHLR");
		break;

	    // Timeout
	    case 20:
		params.set_int(RRRPT_TIMEOUT, atoi(token), count);
		params.set_label(RRRPT_TIMEOUT, "Timeout");
		break;

	    // RouteId
	    case 21:
		params.set_int(RRRPT_ROUTE_ID, atoi(token), count);
		params.set_label(RRRPT_ROUTE_ID, "RouteId");
		break;

	    // HLRGtAddress
	    case 22:
		params.set_int(RRRPT_HLR_GT, atoi(token), count);
		params.set_label(RRRPT_HLR_GT, "HLRGtAddress");
		break;

	    // SMPPOptionalParamSupported
	    case 23:
		params.set_bool(RRRPT_SMPP_OPT_SUPPORTED, atoi(token) > 0, count);
		params.set_label(RRRPT_SMPP_OPT_SUPPORTED, "SMPPOptionalParamSupported");
		break;

	    // Smpp customer default encoding
	    case 24:
		params.set_int(RRRPT_SMPP_CUSTOMER_ENC_DEF, atoi(token), count);
		params.set_label(RRRPT_SMPP_CUSTOMER_ENC_DEF, "SMPPCustomerDefaultEncoding");
		break;

	    // Smpp customer default encoding
	    case 25:
		params.set_int(RRRPT_SMPP_VENDOR_ENC_DEF, atoi(token), count);
		params.set_label(RRRPT_SMPP_VENDOR_ENC_DEF, "SMPPVendorDefaultEncoding");
		break;

	    // sms oa address
	    case 26:
		params.set_cstr(RRRPT_SMS_OA_ADDR, token, count);
		params.set_label(RRRPT_SMS_OA_ADDR, "SMSOriginatingAddress");
		break;

	    // sms oa ton
	    case 27:
		params.set_int(RRRPT_SMS_OA_TON, atoi(token), count);
		params.set_label(RRRPT_SMS_OA_TON, "SMSOriginatingAddressTON");
		break;

	    // sms oa npi
	    case 28:
		params.set_int(RRRPT_SMS_OA_NPI, atoi(token), count);
		params.set_label(RRRPT_SMS_OA_NPI, "SMSOriginatingAddressNPI");
		break;

	    // sms da address
	    case 29:
		params.set_cstr(RRRPT_SMS_DA_ADDR, token, count);
		params.set_label(RRRPT_SMS_DA_ADDR, "SMSDestinationAddress");
		break;

	    // sms da ton
	    case 30:
		params.set_int(RRRPT_SMS_DA_TON, atoi(token), count);
		params.set_label(RRRPT_SMS_DA_TON, "SMSDestinationAddressTON");
		break;

	    // sms da npi
	    case 31:
		params.set_int(RRRPT_SMS_DA_NPI, atoi(token), count);
		params.set_label(RRRPT_SMS_DA_NPI, "SMSDestinationAddressNPI");
		break;

	    // err
	    default: return 1;
	}
	++tc;
    }
    // check tc
    if(tc != 32) return 1;
    // int count
    ++count;
    // ok
    return 0;
}

int rrp::RRRouteParam::encode(unsigned char* out){
    return 0;
}


// RRSize
int rrp::RRSize::get_size(const char* size_str, RRSize* size_p){
    // Size:300;1;1;7a57b6da591245c2999033fd21dad623
    if(size_str == NULL || size_p == NULL) return 1;
    // tmp copy
    char tmp_data[strlen(size_str) + 1];
    char* tmp_data_p = tmp_data;
    strcpy(tmp_data, size_str);
    char* token;
    int tc = 0;
    // split by ;
    while((token = strsep(&tmp_data_p, ";")) != NULL){
	//std::cout << "TOKEN: " << token << std::endl;
	switch(tc){
	    // length of current fragment/pdu
	    case 0:
		size_p->size = atoi(token);
		break;

	    // index of current fragment
	    case 1:
		size_p->fragment = atoi(token);
		break;

	    // more fragments flag
	    case 2:
		size_p->more_fragments = (atoi(token) == 1);
		break;

		// fragment stream guid
	    case 3:
	    {
		if(strlen(token) != 32) return 2;
		unsigned int i = 0, count = 0;
		for(; i<strlen(token) && count < sizeof(size_p->uuid); i += 2, count++) sscanf(&token[i], "%02x", &size_p->uuid[count]);
		break;
	    }
	}
	++tc;

    }
    // check tc
    if(!(tc == 1 || tc == 4)) return 1;
    // ok
    return 0;
}


int rrp::RRSize::encode_size(RRSize* size_p, unsigned char* buffer, int buffer_length){
    if(size_p == NULL || buffer == NULL) return 1;
    // Size:300;1;1;7a57b6da591245c2999033fd21dad623

    int res = 0;
    // no fragmentation
    if(size_p->fragment == 0){
	res += snprintf((char*)buffer, buffer_length, "Size:%d\n", size_p->size);
	return res;

    }

    // convert guid to str
    char str_guid[33];
    char* str_guid_p = str_guid;
    for(unsigned int i = 0; i<sizeof(size_p->uuid); i++){
	sprintf(str_guid_p, "%02x", size_p->uuid[i]);
	str_guid_p += 2;
    }
    str_guid[32] = 0;

    // fragmentation
    res += snprintf((char*)buffer, buffer_length, "Size:%d;%d;%d;%s\n", size_p->size, size_p->fragment, size_p->more_fragments, str_guid);

    // ok
    return res;
}



// RRStatus
uint32_t rrp::RRStatus::get_status(const char* str){
    if(str == NULL) return 0;
    char* saveptr;
    char str_cpy[strlen(str) + 1];
    strcpy(str_cpy, str);
    char* token = strtok_r(str_cpy, " ", &saveptr);
    if(token == NULL) return 0;
    return atoi(token);
}

// RRStateMachine
rrp::RRStateMachine::RRStateMachine(){

}

rrp::RRStateMachine::~RRStateMachine(){

}

void rrp::RRStateMachine::init(RRConnection* _rrc){

}

int rrp::RRStateMachine::run(RRPDU* pdu, RRConnection* rrc){
    // params pointer
    pmink_utils::PooledVPMap<uint32_t>* params = &pdu->params;
    // session
    RRSession* rrs = rrc->session;
    // check version
    pmink_utils::VariantParam* vp = params->get_param(RRPT_VERSION);
    if(vp == NULL) return 100;
    if((int)*vp != _RRP_VERSION_) return 101;
    // check session
    vp = params->get_param(RRPT_SESSION_ID);
    if(vp == NULL) return 10;
    if(vp->get_size() != 16) return 11;
    if(memcmp(rrc->session_id, (unsigned char*)*vp, 16) != 0) return 12;
    // check method
    vp = params->get_param(RRPT_METHOD);
    // sanity check
    if(vp == NULL) return 50;
    if(rrs->find_param_id((char*)*vp) == NULL) return 51;
    // check sequence state
    vp = params->get_param(RRPT_SEQUENCE_STATE);
    // sanity check
    if(vp == NULL) return 1;
    // convert string to param id
    RRParam* param_id = rrs->find_param_id((char*)*vp);
    // sanity check
    if(param_id == NULL) return 2;
    // check sequence state
    switch(param_id->id){
	// sequence begin
	case RRSST_BEGIN:
	{
	    // validate seq num
	    if(!rrc->validate_seq_num(pdu, 1)){
		// todo stats
		return 40;
	    }
	    // sequence pointer
	    RRSequence* rrseq = rrc->seq_pool.allocate_constructed();
	    // sanity check
	    if(rrseq == NULL) return 3;
	    // copy params
	    rrseq->pdu.params = pdu->params;
	    // process
	    process_begin(rrseq);
	    break;
	}
	// sequence continue
	case RRSST_CONTINUE:
	{
	    // get uuid
	    vp = params->get_param(RRPT_CALL_ID);
	    if(vp == NULL) {
		// todo
		return 20;
	    }
	    if(vp->get_size() != 16){
		// todo
		return 21;
	    }
	    // find seq
	    RRSequence* rrseq = rrc->get_sequence((unsigned char*)*vp);
	    if(rrseq == NULL){
		// todo err stats
		return 23;
	    }
	    // copy params
	    rrseq->pdu.params = pdu->params;
	    // process
	    process_continue(rrseq);
	    break;
	}
	// sequence end
	case RRSST_END:
	{
	    // get uuid
	    vp = params->get_param(RRPT_CALL_ID);
	    if(vp == NULL) {
		// todo
		return 20;
	    }
	    if(vp->get_size() != 16){
		// todo
		return 21;
	    }
	    // find seq
	    RRSequence* rrseq = rrc->get_sequence((unsigned char*)*vp);
	    if(rrseq == NULL){
		// todo err stats
		return 23;
	    }
	    // copy params
	    rrseq->pdu.params = pdu->params;
	    // process
	    process_end(rrseq, true);
	    break;
	}

	// sequence end ack/err
	case RRSST_SEQ_END:
	{
	    // get uuid
	    vp = params->get_param(RRPT_CALL_ID);
	    if(vp == NULL) {
		// todo
		return 20;
	    }
	    if(vp->get_size() != 16){
		// todo
		return 21;
	    }
	    // find seq
	    RRSequence* rrseq = rrc->get_sequence((unsigned char*)*vp);
	    if(rrseq == NULL){
		// todo err stats
		return 23;
	    }
	    // copy params
	    rrseq->pdu.params = pdu->params;
	    // process
	    process_seq_end(rrseq);
	    break;
	}
    }


    return 0;
}

void rrp::RRStateMachine::process_begin(RRSequence* tmp_seq){
    // uuid
    pmink_utils::VariantParam* vp = tmp_seq->pdu.params.get_param(RRPT_CALL_ID);
    // sanity check
    if(vp == NULL){
	//todo error
	// return to pool
	tmp_seq->connection->seq_pool.deallocate_constructed(tmp_seq);
	return;
    }

    // chck uuid size
    if(vp->get_size() != 16){
	// todo error
	// rturn to pool
	tmp_seq->connection->seq_pool.deallocate_constructed(tmp_seq);
	return;
    }
    // session pointer
    RRSession* rrs = tmp_seq->connection->session;
    // set uuid
    memcpy(tmp_seq->uuid, (unsigned char*)*vp, vp->get_size());
    // get source ctx
    vp = tmp_seq->pdu.params.get_param(RRPT_SRC_CTX);
    if(vp != NULL) strcpy(tmp_seq->destination_id, (char*)*vp);
    // reset
    tmp_seq->reset(false, false);
    // set method
    tmp_seq->method = (RRMethodType)rrs->find_param_id((char*)*tmp_seq->pdu.params.get_param(RRPT_METHOD))->id;
    // default state END
    tmp_seq->seq_state = RRSST_END;
    // add sequence
    tmp_seq->connection->add_sequence(tmp_seq);
    // run SEQUENCE_NEW event handler
    pmink_utils::EventArgs<RREventArgIdType, void*> e_args;
    e_args.add_arg(RREAIT_SEQUENCE, tmp_seq);
    tmp_seq->connection->e_handler.run_handler(RREIT_SEQUENCE_NEW, e_args);
    // params pointer
    pmink_utils::PooledVPMap<uint32_t>* params = &tmp_seq->pdu.params;


    // *** gen ack ***
    tmp_seq->gen_ack(true);
    // status
    params->set_cstr(RRPT_STATUS, rrs->find_status_id(RREC_200)->msg.c_str());
    /*
    // reverse From/To
    params->swap(pmink_utils::ParamTuple<RRParamType>(RRPT_FROM, 0),
		 pmink_utils::ParamTuple<RRParamType>(RRPT_TO, 0));

    // reverse FromPort/ToPort
    params->swap(pmink_utils::ParamTuple<RRParamType>(RRPT_FROM_PORT, 0),
		 pmink_utils::ParamTuple<RRParamType>(RRPT_TO_PORT, 0));

    // reverse DestinationContext/SourceContext
    params->swap(pmink_utils::ParamTuple<RRParamType>(RRPT_SRC_CTX, 0),
		 pmink_utils::ParamTuple<RRParamType>(RRPT_DST_CTX, 0));

*/

    // toggle seq ack
    tmp_seq->toggle_ack_rcvd();
    // inc seq num
    ++tmp_seq->seq_num;


    //push to out
    tmp_seq->out.set(true);
    tmp_seq->connection->out_queue.push(tmp_seq);

}

void rrp::RRStateMachine::process_continue(RRSequence* tmp_seq){
    //std::cout << "void rrp::RRStateMachine::process_continue" << std::endl;
    // ts
    tmp_seq->timestamp = time(NULL);
    // validate seq num
    if(!tmp_seq->connection->validate_seq_num(&tmp_seq->pdu, tmp_seq->seq_num)){
	// todo stats
	// free on send
	tmp_seq->free_on_send = true;
	// state RRSST_SEQ_END
	tmp_seq->seq_state = RRSST_SEQ_END;
	tmp_seq->pdu.params.set_cstr(RRPT_SEQUENCE_STATE, tmp_seq->connection->session->find_rev_str(RRSST_SEQ_END));
	// status err
	tmp_seq->pdu.params.set_cstr(RRPT_STATUS, tmp_seq->connection->session->find_status_id(RREC_483)->msg.c_str());
	// run SEQUENCE_END event handler
	pmink_utils::EventArgs<RREventArgIdType, void*> e_args;
	e_args.add_arg(RREAIT_SEQUENCE, tmp_seq);
	tmp_seq->e_handler.run_handler(RREIT_SEQUENCE_END, e_args);
	// remove from list
	tmp_seq->connection->remove_sequence(tmp_seq);
	// push to out
	tmp_seq->connection->out_queue.push(tmp_seq);
	return;
    }

    // default state END
    tmp_seq->seq_state = RRSST_END;
    // toggle seq ack
    tmp_seq->toggle_ack_rcvd();
    // run RREIT_SEQUENCE_NEXT event handler
    pmink_utils::EventArgs<RREventArgIdType, void*> e_args;
    e_args.add_arg(RREAIT_SEQUENCE, tmp_seq);
    tmp_seq->e_handler.run_handler(RREIT_SEQUENCE_NEXT, e_args);
    // inc sequenuce number
    if(tmp_seq->ack_rcvd) ++tmp_seq->seq_num;

    /*
    // set seq num
    tmp_seq->pdu.params.set_int(RRPT_SEQUENCE, tmp_seq->seq_num);
    // set seq state
    tmp_seq->pdu.params.set_cstr(RRPT_SEQUENCE_STATE, tmp_seq->connection->session->find_rev_str(tmp_seq->seq_state));
*/

    // *** gen ack ***
    tmp_seq->gen_ack(true);

    // toggle seq ack
    tmp_seq->toggle_ack_rcvd();
    // inc sequenuce number
    if(tmp_seq->ack_rcvd) ++tmp_seq->seq_num;
    // push to out
    tmp_seq->out.set(true);
    tmp_seq->connection->out_queue.push(tmp_seq);


}

void rrp::RRStateMachine::process_end(RRSequence* tmp_seq, bool remove_seq){
    // ts
    tmp_seq->timestamp = time(NULL);
    // validate seq num
    if(!tmp_seq->connection->validate_seq_num(&tmp_seq->pdu, tmp_seq->seq_num)){
	// todo stats
	// free on send
	tmp_seq->free_on_send = true;
	// state RRSST_SEQ_END
	tmp_seq->seq_state = RRSST_SEQ_END;
	tmp_seq->pdu.params.set_cstr(RRPT_SEQUENCE_STATE, tmp_seq->connection->session->find_rev_str(RRSST_SEQ_END));
	// status err
	tmp_seq->pdu.params.set_cstr(RRPT_STATUS, tmp_seq->connection->session->find_status_id(RREC_483)->msg.c_str());
	// run RREIT_SEQUENCE_END event handler
	pmink_utils::EventArgs<RREventArgIdType, void*> e_args;
	e_args.add_arg(RREAIT_SEQUENCE, tmp_seq);
	tmp_seq->e_handler.run_handler(RREIT_SEQUENCE_END, e_args);
	// remove from list
	tmp_seq->connection->remove_sequence(tmp_seq);
	// push to out
	tmp_seq->connection->out_queue.push(tmp_seq);
	return;
    }


    // default state END
    tmp_seq->seq_state = RRSST_SEQ_END;
    // toggle seq ack
    tmp_seq->toggle_ack_rcvd();
    // run RREIT_SEQUENCE_END event handler
    pmink_utils::EventArgs<RREventArgIdType, void*> e_args;
    e_args.add_arg(RREAIT_SEQUENCE, tmp_seq);
    tmp_seq->e_handler.run_handler(RREIT_SEQUENCE_END, e_args);
    // inc sequenuce number
    if(tmp_seq->ack_rcvd) ++tmp_seq->seq_num;


    // *** gen ack ***
    tmp_seq->gen_ack(true);
    // free on send
    tmp_seq->free_on_send = true;
    // remove from list
    if(remove_seq) tmp_seq->connection->remove_sequence(tmp_seq);
    // push to out
    tmp_seq->connection->out_queue.push(tmp_seq);


    /*
    // set seq num
    tmp_seq->pdu.params.set_int(RRPT_SEQUENCE, tmp_seq->seq_num);
    // set seq state
    // default state END (in case of event handler override)
    tmp_seq->seq_state = RRSST_SEQ_END;
    tmp_seq->pdu.params.set_cstr(RRPT_SEQUENCE_STATE, tmp_seq->connection->session->find_rev_str(RRSST_SEQ_END));
    // free on send
    tmp_seq->free_on_send = true;
    // remove from list
    if(remove_seq) tmp_seq->connection->remove_sequence(tmp_seq);
    // push to out
    tmp_seq->connection->out_queue.push(tmp_seq);
*/

}

void rrp::RRStateMachine::process_seq_end(RRSequence* tmp_seq){
    // ts
    tmp_seq->timestamp = time(NULL);
    // run STREAM_END event handler
    pmink_utils::EventArgs<RREventArgIdType, void*> e_args;
    e_args.add_arg(RREAIT_SEQUENCE, tmp_seq);
    tmp_seq->e_handler.run_handler(RREIT_SEQUENCE_END, e_args);
    // check status for unregistered
    pmink_utils::VariantParam* vp = tmp_seq->pdu.params.get_param(RRPT_STATUS);
    if(vp != NULL && RRStatus::get_status((char*)*vp) == RREC_401){
	//PMDLOG(
	  //  std::cout << "======================== UNREGEISTERED =================" << std::endl;
	//)

	tmp_seq->connection->reconnect_flag.comp_swap(false, true);
    }

    // remove from list
    tmp_seq->connection->remove_sequence(tmp_seq);
    // return to pool
    tmp_seq->connection->seq_pool.deallocate_constructed(tmp_seq);



}


// RRStream
rrp::RRSequence::RRSequence():
	connection(NULL),
	free_on_send(false),
	timestamp(time(NULL)),
	ack_rcvd(false),
	timed_out(false),
	seq_state(RRSST_BEGIN),
	seq_num(1),
	method(RRRT_UNKNOWN){

    bzero(destination_id, sizeof(destination_id));
    bzero(uuid, sizeof(uuid));
    pdu.sequence = this;
}

rrp::RRSequence::~RRSequence(){

}

void rrp::RRSequence::gen_ack(bool clear_params){
    if(clear_params) pdu.params.clear_params();
    // call id
    pdu.params.set_octets(RRPT_CALL_ID, uuid, 16);
    // set seq state
    pdu.params.set_cstr(RRPT_SEQUENCE_STATE, connection->session->find_rev_str(seq_state));
    // seq seqeunce no
    pdu.params.set_int(RRPT_SEQUENCE, seq_num);
    // session id
    pdu.params.set_octets(RRPT_SESSION_ID, connection->session_id, 16);
    // version
    pdu.params.set_int(RRPT_VERSION, _RRP_VERSION_);
    // to
    pdu.params.set_cstr(RRPT_TO, connection->end_point_address);
    // from
    pdu.params.set_cstr(RRPT_FROM, connection->local_point_address);
    // from port
    pdu.params.set_int(RRPT_FROM_PORT, connection->local_point_port);
    // to port
    pdu.params.set_int(RRPT_TO_PORT, connection->end_point_port);
    // method
    pdu.params.set_cstr(RRPT_METHOD, connection->session->find_rev_str(method));
    // destination ctx
    pdu.params.set_cstr(RRPT_DST_CTX, destination_id);
    // source ctx
    pdu.params.set_cstr(RRPT_SRC_CTX, connection->session->daemon_id);

}


void rrp::RRSequence::reset(bool reset_uuid, bool mod_params){
    // ts
    timestamp = time(NULL);
    // seq num
    seq_num = 1;
    // method
    method = RRRT_UNKNOWN;
    // ack rcvd
    ack_rcvd = false;
    // reset free_on_send
    free_on_send = false;
    // timeout
    timed_out = false;
    // state
    seq_state = RRSST_BEGIN;
    // clear events
    e_handler.clear();
    // clear params
    params.clear_params();
    // get new uuid if flag set
    if(reset_uuid) connection->generate_uuid(uuid);
    // modify params
    if(mod_params){
	// clear params
	pdu.params.clear_params();
	// call id
	pdu.params.set_octets(RRPT_CALL_ID, uuid, 16);
	// set BEGIN
	pdu.params.set_cstr(RRPT_SEQUENCE_STATE, connection->session->find_rev_str(RRSST_BEGIN));
	// seq seqeunce no
	pdu.params.set_int(RRPT_SEQUENCE, 1);
	// session id
	pdu.params.set_octets(RRPT_SESSION_ID, connection->session_id, 16);
	// FROM
	pdu.params.set_cstr(RRPT_SRC_CTX, connection->session->daemon_id);
	// version
	pdu.params.set_int(RRPT_VERSION, _RRP_VERSION_);

    }



}




void rrp::RRSequence::toggle_ack_rcvd(){
    ack_rcvd = !ack_rcvd;

}


void rrp::RRSequence::send(){
    connection->out_queue.push(this);

}

rrp::RRConnection::RRConnection(int _socket,
				const char* _end_point_address,
				unsigned int _end_point_port,
				const char* _local_point_address,
				unsigned int _local_point_port,
				RRConnectionType _direction,
				int stream_pool_size,
				RRSession* _session):
	session(_session),
	direction(_direction),
	socket(_socket){


    // state
    state.set(RRCST_UNKNOWN);
    // state machine
    rrp_sm.init(this);

    // ports and addresses
    bzero(local_point_address, sizeof(local_point_address));
    bzero(end_point_address, sizeof(end_point_address));
    strcpy(end_point_address, _end_point_address);
    strcpy(local_point_address, _local_point_address);
    end_point_port = _end_point_port;
    local_point_port = _local_point_port;

    // random generator
    timespec tmp_time;
    clock_gettime(0, &tmp_time);
    ran_mt19937.seed(tmp_time.tv_nsec + tmp_time.tv_sec);
    random_generator = new boost::uuids::random_generator(ran_mt19937);

    // session id
    generate_uuid(session_id);


    // init stream pool
    seq_pool.init(stream_pool_size);
    seq_pool.construct_objects();
    // set stream connection pointer
    for(int i = 0; i<stream_pool_size; i++) seq_pool.get_constructed(i)->connection = this;

    // queues
    out_queue.set_capacity(stream_pool_size);

    // pthread attr
    pthread_attr_t pi_thread_attr;
    // scheduling parameters
    pthread_attr_init(&pi_thread_attr);
    // explicit FIFO scheduling
    pthread_attr_setinheritsched(&pi_thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&pi_thread_attr, SCHED_FIFO);
    // priority
    sched_param pi_sp;
    // max priority
    pi_sp.__sched_priority = 99;
    // set priorities
    pthread_attr_setschedparam(&pi_thread_attr, &pi_sp);
    // start
    thm.start();
    thm.new_thread(&out_loop, "rr_out", this, &pi_thread_attr);
    thm.new_thread(&in_loop, "rr_in", this, &pi_thread_attr);
    thm.new_thread(&timeout_loop, "rr_timeout", this);
    // destroy attributes
    pthread_attr_destroy(&pi_thread_attr);

}

rrp::RRConnection::~RRConnection(){
    // stop threads
    thm.stop();
    // process out_queue
    RRSequence* rrseq = NULL;
    while(out_queue.pop(&rrseq)) rrseq->connection->seq_pool.deallocate_constructed(rrseq);


    delete random_generator;
}

int rrp::RRConnection::generate_uuid(unsigned char* out){
    if(out == NULL) return 1;
    slock_uuid.lock();
    boost::uuids::uuid _uuid = (*random_generator)(); // operator ()
    memcpy(out, _uuid.data, 16);
    slock_uuid.unlock();
    return 0;

}

rrp::RRSequence* rrp::RRConnection::get_sequence(unsigned char* _uuid){
    if(_uuid == NULL) return NULL;
    RRSequence* seq = NULL;
    mtx_seqs.lock();
    for(unsigned int i = 0; i<seqs.size(); i++){
	seq = seqs[i];
	if(memcmp(seq->uuid, _uuid, 16) == 0){
	    mtx_seqs.unlock();
	    return seq;
	}
    }
    mtx_seqs.unlock();
    return NULL;
}

int rrp::RRConnection::reconnect_socket(){
    int _socket = -1;
    int ret;
    struct sockaddr_in local_bind;
    in_addr addr;
    bzero(&addr, sizeof(addr));
    addr.s_addr = htonl(INADDR_ANY);

    /* Create UDP Socket */
    _socket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if(_socket < 0){
	return -1;
    }
    int yes = 1;
    // set socket option
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    // convert local address if specified
    if(strlen(local_point_address) > 0) {
	// use ANY in case of error
	if(inet_aton(local_point_address, &addr) <= 0) addr.s_addr = htonl(INADDR_ANY);
    }

    // local bind
    bzero(&local_bind, sizeof(local_bind) );
    local_bind.sin_family = AF_INET;
    local_bind.sin_addr.s_addr = addr.s_addr;
    local_bind.sin_port = htons(local_point_port);
    ret = bind(_socket, (sockaddr *)&local_bind, sizeof(sockaddr_in));
    if(ret < 0){
	shutdown(_socket, SHUT_RDWR);
	close(_socket);
	return -1;
    }



    return _socket;
}

int rrp::RRConnection::init_reconnect(){
    // reset reg flag
    registered.set(false);
    // close socket
    close(socket);
    // new socket
    socket = reconnect_socket();
    //std::cout << "NEW SOCKET : " << socket << std::endl;
    // cehck socket
    if(socket <= 0) return -1;

    // register client (new thread needed)
    class _tmp_thread {
    public:
	static void* run(void* args){
	    // client pointer
	    RRConnection* tmp_rrc = (RRConnection*)args;
	    // register
	    if(tmp_rrc->register_client() != 0) tmp_rrc->reconnect_flag.comp_swap(false, true);
	    // detach
	    pthread_detach(pthread_self());
	    // return
	    return NULL;
	}
    };
    // tmp thread
    pthread_t tmp_thread_h;
    pthread_create(&tmp_thread_h, NULL, &_tmp_thread::run, this);

    // register
    //if(register_client() != 0) return 1;
    // ok
    return socket;
}


void rrp::RRConnection::process_timeout(bool override){
    // check seq timeout
    if(seq_timeout_check.comp_swap(true, false) || override){
	// update now
	time_t tm_now = (override ? std::numeric_limits<time_t>::max() : time(NULL));
	// lock
	mtx_seqs.lock();
	// tmp copy of active sequences
	std::vector<RRSequence*> tmp_seqs = seqs;
	// unlock
	mtx_seqs.unlock();
	// seq pointer
	RRSequence* seq = NULL;
	// loop active sequences
	for(std::vector<RRSequence*>::iterator it = tmp_seqs.begin(); it != tmp_seqs.end(); it++){
	    // seq pointer
	    seq = *it;
	    // skip if still in out_queue
	    if(seq->out.get()) continue;
	    // timeout found
	    if(tm_now - seq->timestamp >= session->seq_timeout){
		// set timeout flag
		seq->timed_out = true;
		// run SEQUENCE_TIMEOUT event handler
		pmink_utils::EventArgs<RREventArgIdType, void*> e_args;
		e_args.add_arg(RREAIT_SEQUENCE, seq);
		seq->e_handler.run_handler(RREIT_SEQUENCE_TIMEOUT, e_args);
	    }
	}
	// lock
	mtx_seqs.lock();
	// loop active sequences
	for(std::vector<RRSequence*>::iterator it = seqs.begin(); it != seqs.end(); ){
	    // seq pointer
	    seq = *it;
	    // free if timeout flag was set
	    if(seq->timed_out){
		// remove from active sequence list
		it = seqs.erase(it);
		// return to pool
		seq_pool.deallocate_constructed(seq);

	    // next seq
	    }else ++it;
	}
	// unlock
	mtx_seqs.unlock();
    }
}

int rrp::RRConnection::register_client(){
    // check if alreday registered
    if(registered.get()) return 1;
    // alloc seq
    RRSequence* rrs = new_sequence(".", RRRT_REGISTER, true);
    // sanity
    if(rrs == NULL) return 2;
    // set event handler
    RegisterEvent* rreh = new RegisterEvent(this);
    rrs->e_handler.set_handler(RREIT_SEQUENCE_END, rreh);
    rrs->e_handler.set_handler(RREIT_SEQUENCE_TIMEOUT, rreh);
    // push to out
    rrs->out.set(true);
    out_queue.push(rrs);
    // wait for status or timeout
    timespec ts = {0, 100000000};
    while(!rreh->finished.get()) nanosleep(&ts, NULL);
    // remove handlers
    rrs->e_handler.remove_handler(RREIT_SEQUENCE_END);
    rrs->e_handler.remove_handler(RREIT_SEQUENCE_TIMEOUT);
    delete rreh;

    // return reg status
    return (registered.get() ? 0 : 3);
}


void rrp::RRConnection::add_sequence(RRSequence* _seq){
    mtx_seqs.lock();
    seqs.push_back(_seq);
    mtx_seqs.unlock();

}

void rrp::RRConnection::remove_sequence(RRSequence* _seq){
    mtx_seqs.lock();
    for(unsigned int i = 0; i<seqs.size(); i++){
	if(seqs[i] == _seq){
	    seqs.erase(seqs.begin() + i);
	    break;
	}
    }
    mtx_seqs.unlock();

}

bool rrp::RRConnection::validate_seq_num(RRPDU* pdu,  unsigned int expected_seq_num){
    pmink_utils::VariantParam* vp = pdu->params.get_param(RRPT_SEQUENCE);
    if(vp == NULL) return false;
    if((int)*vp != expected_seq_num) return false;
    return true;
}

rrp::RRSequence* rrp::RRConnection::new_sequence(const char* _dest_id, RRMethodType _method, bool reset_uuid, RRSequence* rrs){
    if(rrs == NULL) rrs = seq_pool.allocate_constructed();
    if(rrs == NULL) return NULL;
    rrs->reset(reset_uuid, true);
    // check for valid method
    const char* method_str = session->find_rev_str(_method); 
    if(method_str == NULL) return NULL;
    // set method
    rrs->method = _method;
    rrs->pdu.params.set_cstr(rrp::RRPT_METHOD, method_str);
    // destination ctx
    strcpy(rrs->destination_id, _dest_id);
    rrs->pdu.params.set_cstr(RRPT_DST_CTX, _dest_id);
    // to
    rrs->pdu.params.set_cstr(RRPT_TO, end_point_address);
    // from
    rrs->pdu.params.set_cstr(RRPT_FROM, local_point_address);
    // from port
    rrs->pdu.params.set_int(RRPT_FROM_PORT, local_point_port);
    // to port
    rrs->pdu.params.set_int(RRPT_TO_PORT, end_point_port);
    // add
    add_sequence(rrs);
    // return
    return rrs;
}




void* rrp::RRConnection::in_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    RRConnection* conn = (RRConnection*)ti->user_args;

    int poll_timeout = 1000;
    pollfd fds_lst[1];
    // monitor POLLIN event
    fds_lst[0].events = POLLIN;
    // update socket in poll structure
    fds_lst[0].fd = conn->socket;
    // source address
    sockaddr_in src_addr = {0};
    socklen_t src_addr_l = sizeof(src_addr);
    int br;
    int res;
    timespec ts = {0, 1000000};
    unsigned char buff[conn->session->max_pdu_size];
    unsigned char frag_buff[conn->session->max_pdu_size];
    unsigned int frag_l = 0;
    unsigned int frag_c = 0;
    uint32_t frag_hash = 0;
    RRPDU pdu;
    pmink_utils::VariantParam* vp = NULL;
    RRSize rrsize;
    // loop
    while(ti->is_active()){
	// check timeout of all active seqs
	conn->process_timeout();
	// reconnect
	if(conn->reconnect_flag.get()){
	    //PMDLOG(
		//std::cout << "======================== RRP RECONNECTING =================" << std::endl;
	    //)

	    // force timeout of all active seqs
	    conn->process_timeout(true);
	    // update socket
	    fds_lst[0].fd = conn->init_reconnect();
	    // reset reconnect flag
	    conn->reconnect_flag.comp_swap(true, false);
	}

	// poll socket
	res = poll(fds_lst, 1, poll_timeout);
	// check for timeout
	if(res > 0){
	    // check for POLLIN event
	    if((fds_lst[0].revents & POLLIN) == POLLIN){
		// receive data
		br = recvfrom(conn->socket, buff, sizeof(buff), 0, (sockaddr*)&src_addr, &src_addr_l);
		// decode
		res = decode(buff, br, &pdu, conn->session);
		if(res == 0){
		    PMDLOG(
			std::cout << "======================== RRP Received =================" << std::endl;
			std::cout << std::string((char*)buff, br) << std::endl;
			std::cout << "======================================================="  << std::endl;
		    )
		    //std::cout << "DECODED" << std::endl;
		    //std::cout << pdu.params << std::endl;
		    // validate size
		    vp = pdu.params.get_param(RRPT_SIZE);
		    // sanity check
		    if(vp == NULL) {
			// todo err stats
			//std::cout << "Size missing..." << std::endl;
			continue;
		    }
		    // decode size
		    if(RRSize::get_size((char*)*vp, &rrsize) != 0){
			// todo err stats
			//std::cout << "Size malformed..." << std::endl;
			continue;
		    }

		    // check if size is correct
		    if(br != rrsize.size){
			// todo err stats
			//std::cout << "Size error..." << std::endl;
			continue;
		    }

		    // run state machine
		    res = conn->rrp_sm.run(&pdu, conn);
		    // check errors
		    if(res != 0){
			// todo errors
			//std::cout << "STATE MACHINE ERR: " << res << std::endl;
		    }

		// fragmented
		}else if(res == 100){
		    // get fragment size
		    vp = pdu.params.get_param(RRPT_SIZE);
		    // decode size
		    if(vp == NULL || RRSize::get_size((char*)*vp, &rrsize) != 0){
			// todo err stats
			continue;
		    }
		    //std::cout << "!!! FRAGMENTED: " << rrsize.size << ":" << rrsize.fragment << ":" << rrsize.more_fragments << std::endl;
		    // next expected fragment
		    ++frag_c;
		    // check if right fragmetn index received
		    if(frag_c != rrsize.fragment){
			//std::cout << "!!!UNEXPECTED FRAGMENT: " << rrsize.fragment << ", expected: " << frag_c << std::endl;
			// reset
			frag_c = 0;
			frag_l = 0;
			frag_hash = 0;
			continue;
		    }

		    // set hash if first fragment
		    if(frag_c == 1) frag_hash = pmink_utils::hash_fnv1a(rrsize.uuid, sizeof(rrsize.uuid));
		    // check if uuids match
		    else if(frag_hash != pmink_utils::hash_fnv1a(rrsize.uuid, sizeof(rrsize.uuid))){
			//std::cout << "!!!UNEXPECTED FRAGMENT UUID!! " << std::endl;
			// reset
			frag_c = 0;
			frag_l = 0;
			frag_hash = 0;
			continue;

		    }


		    // size without Size line
		    int tmp_s = rrsize.size - vp->get_size() - 5;
		    // overflow check (extra 10 just in case, when adding 'Size:' before reassembly)
		    if(frag_l + 10 >= sizeof(frag_buff)){
			// total fragment buffer overrflow
			// reset
			frag_c = 0;
			frag_l = 0;
			frag_hash = 0;
			continue;
		    }
		    // fragment data
		    memcpy(&frag_buff[frag_l], &buff[vp->get_size()  + 5], tmp_s);
		    // total frag length
		    frag_l += tmp_s;

		    // check if last fragment
		    if(!rrsize.more_fragments){
			// length of news Size line
			int nsl = 6 + pmink_utils::num_of_digits(frag_l + 6);

			// insert size
			char tmp_str[64];
			snprintf(tmp_str, sizeof(tmp_str), "Size:%d\n", frag_l + 6 + pmink_utils::num_of_digits(frag_l + 6));
			memmove(&frag_buff[nsl], frag_buff, frag_l);
			memcpy(frag_buff, tmp_str, nsl);
			frag_l += nsl;

			/*
			std::cout << "Total l: " << frag_l << std::endl;
			std::cout << "=====================" << std::endl;
			std::cout << std::string((char*)frag_buff, frag_l);
			std::cout << "=====================" << std::endl;
			*/
			//pmink_utils::print_hex(frag_buff, frag_l);




			// decode
			res = decode(frag_buff, frag_l, &pdu, conn->session);
			if(res == 0){
			    // run state machine
			    res = conn->rrp_sm.run(&pdu, conn);
			    // check errors
			    if(res != 0){
				// todo errors
				//std::cout << "STATE MACHINE ERR: " << res << std::endl;
			    }

			}else{
			    // todo stats
			    //std::cout << "DEcode ERROR!!! " << std::endl;
			    //std::cout << std::string((char*)frag_buff, frag_l) << std::endl;
			}

			// reset
			frag_c = 0;
			frag_l = 0;
			frag_hash = 0;

		    }

		}else{
		    //std::cout << "DEcode ERROR!!! " << std::endl;
		    // todo stats
		}


	    }
	// socket idle or error
	}else{
	    // idle
	    if(res == 0){
		nanosleep(&ts, NULL);
		//std::cout << "Socket idle..." << std::endl;

	    // error
	    }else{
		//std::cout << "Socket error..." << std::endl;
		// force timeout of all active seqs
		conn->process_timeout(true);


	    }
	}
    }

    // force timeout of all active seqs
    conn->process_timeout(true);

    return NULL;
}

void* rrp::RRConnection::timeout_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    RRConnection* conn = (RRConnection*)ti->user_args;
    int total_sleep = 0;

    // loop
    while(ti->is_active()){
	// sleep 1 sec
	sleep(1);
	++total_sleep;
	// check if user timeout has been reached
	if(total_sleep < conn->session->seq_timeout) continue;
	// set flag, timeout will be processed in IN thread
	conn->seq_timeout_check.comp_swap(false, true);
	// reset current timeout
	total_sleep = 0;

    }

    return NULL;
}




void* rrp::RRConnection::out_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    RRConnection* conn = (RRConnection*)ti->user_args;

    sockaddr_in end_point;
    memset(&end_point, 0, sizeof(end_point));
    end_point.sin_family = AF_INET;
    end_point.sin_port = ntohs(conn->end_point_port);
    in_addr end_addr;
    inet_aton(conn->end_point_address, &end_addr);
    end_point.sin_addr.s_addr = end_addr.s_addr;
    RRSequence* rrseq = NULL;
    timespec ts = {0, 1000000};
    int enc_l = 0;
    int res;
    unsigned char tmp_buff[conn->session->max_pdu_size];
    unsigned char* tmp_buff_p = tmp_buff;
    unsigned char* tmp_buff_end_p = &tmp_buff[conn->session->max_pdu_size - 1];
    unsigned char rrsize_buff[256];
    RRSize rrsize;
    pmink_utils::VariantParam* vp = NULL;

    // loop
    while(ti->is_active()){
	// check out queue
	if(conn->out_queue.pop(&rrseq)){
	    // encode
	    if(rrseq->pdu.encode(tmp_buff, sizeof(tmp_buff), 0, &enc_l) == 0){
		// *** fragmentation ***
		if(enc_l > conn->session->mtu){
		    // check for call id
		    vp = rrseq->pdu.params.get_param(RRPT_CALL_ID);
		    if(vp == NULL) {
			// return to pool
			if(rrseq->free_on_send) rrseq->connection->seq_pool.deallocate_constructed(rrseq);
			else rrseq->out.set(false);
			continue;
		    }

		    // buffer end pointer
		    tmp_buff_end_p = &tmp_buff[enc_l];
		    // length of Size line
		    int size_str_l = pmink_utils::num_of_digits(enc_l) + 6;
		    // fwd pointer after Size line
		    tmp_buff_p = &tmp_buff[size_str_l];

		    //std::cout << "!!!!FRAGMENTED!!!" << std::endl;
		    //std::cout << "=======================" << std::endl;
		    //pmink_utils::print_hex(tmp_buff, enc_l);
		    //std::cout << std::string((char*)tmp_buff_p, enc_l) << std::endl;
		    //std::cout << "=======================" << std::endl;

		    // Size:300;1;1;7a57b6da591245c2999033fd21dad623\n
		    //int enc_l_wo_size = enc_l - size_str_l;
		    int total_l = enc_l - size_str_l;
		    //int enc_l_wo_size = conn->session->mtu - 50;
		    int frag_mtu = conn->session->mtu - 50;
		    int frag_l = frag_mtu;
		    //std::cout << "====TOTAL L : ===================" << total_l << std::endl;
		    // frag counter
		    int fragc = 0;

		    //int total_fragments = total_l / frag_l;
		    memcpy(rrsize.uuid, (unsigned char*)*vp, vp->get_size());

		    // loop fragments
		    while(total_l > 0){
			// new size
			rrsize.fragment = fragc + 1;
			rrsize.more_fragments = (frag_l < frag_mtu ? false : true);
			// 9 for 'Size:', ';' and '\n'
			// 32 for guid
			// 1 for more fragments
			rrsize.size = frag_l + 9 + 32 + 1 + pmink_utils::num_of_digits(rrsize.fragment) + pmink_utils::num_of_digits(frag_l + 9 + 32 + 1 + pmink_utils::num_of_digits(rrsize.fragment));
			// encode size
			int rrsize_enc_l = RRSize::encode_size(&rrsize, rrsize_buff, sizeof(rrsize_buff));
			// overflow check
			if(tmp_buff_end_p + rrsize_enc_l > &tmp_buff[conn->session->max_pdu_size]){
			    // todo err stats
			    //std::cout << "==========ERRR!!!!" << std::endl;
			    break;
			}
			// insert size
			memmove(&tmp_buff_p[rrsize_enc_l], tmp_buff_p, tmp_buff_end_p - tmp_buff_p);
			memcpy(tmp_buff_p, rrsize_buff, rrsize_enc_l);
			// update end pointer
			tmp_buff_end_p += rrsize_enc_l;

			/*
			std::cout << "=======================" << std::endl;
			std::cout << "Fragment: " << fragc << std::endl;
			std::cout << "Size: " << rrsize.size << std::endl;
			pmink_utils::print_hex(tmp_buff_p, rrsize.size);
			std::cout << std::string((char*)tmp_buff_p, rrsize.size) << std::endl;
			std::cout << "=======================" << std::endl;
			*/

			// send fragment
			res = sendto(conn->socket, tmp_buff_p, rrsize.size, MSG_NOSIGNAL, (sockaddr*)&end_point, sizeof(end_point));
			if(res == -1){
			    // queue reconnect
			    conn->reconnect_flag.comp_swap(false, true);

			}

			// frag counter
			++fragc;
			// fwd pointer
			tmp_buff_p += rrsize.size;
			// total length
			total_l -= frag_l;
			// frag length
			frag_l = (frag_mtu > total_l ? total_l : frag_mtu);
		    }

		    // next
		    continue;
		}

		// *** no fragmentation ***
		// send
		PMDLOG(
			std::cout << "======================== RRP Sending: " << enc_l << " =================" << std::endl;
			std::cout << std::string((char*)tmp_buff, enc_l) << std::endl;
			std::cout << "======================================================="  << std::endl;

		)

		//std::cout << "Sending...: " << enc_l << std::endl;
		//tmp_buff[enc_l] = 0;
		//std::cout << std::string((char*)tmp_buff, enc_l) << std::endl;

		res = sendto(conn->socket, tmp_buff, enc_l, MSG_NOSIGNAL, (sockaddr*)&end_point, sizeof(end_point));
		if(res == -1){
		    // queue reconnect
		    conn->reconnect_flag.comp_swap(false, true);
		}

	    }
	    // return to pool
	    if(rrseq->free_on_send) rrseq->connection->seq_pool.deallocate_constructed(rrseq);
	    else rrseq->out.set(false);


	    // check if end point port changed
	    if(conn->end_port_changed_flag.comp_swap(true, false)){
		//std::cout << "========== CHANGING END PORT ============: " <<conn->end_point_port << std::endl;
		end_point.sin_port = ntohs(conn->end_point_port);

	    }

	// no packets, sleep
	}else nanosleep(&ts, NULL);


    }

    return NULL;
}



// RRPDU
rrp::RRPDU::RRPDU():
    sequence(NULL){

    bzero(pcount, sizeof(pcount));

}

rrp::RRPDU::~RRPDU(){

}

void rrp::RRPDU::reset(){
    bzero(pcount, sizeof(pcount));

}

int rrp::RRPDU::encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, RRSession* sess){
    // sanity checks
    if(buffer == NULL || buffer_length <= 0 || result_length == NULL) return 1;
    // offset
    buffer += offset;
    // reset length
    *result_length = 0;
    // save buffer start pointer
    unsigned char* buff_start = buffer;
    // tmp buff
    char tmp_str[64];
    // rev it
    std::map<uint32_t, std::string>::iterator rev_it;
    // get session
    RRSession* rrs = (sess == NULL ? sequence->connection->session : sess);
    // loop params
    for(pmink_utils::VariantParamMap<uint32_t>::it_t it = params.get_begin(); it != params.get_end(); it++){
	// only 0(zero) context used
	if(it->first.context != 0) continue;
	// param ref
	pmink_utils::VariantParam& vp = it->second;
	// get string in reverse map
	rev_it = rrs->rev_param_def_map.find(it->first.key);
	// skip if not found
	if(rev_it == rrs->rev_param_def_map.end()) continue;
	// encode
	// param label
	memcpy(buffer, rev_it->second.c_str(), rev_it->second.size());
	buffer += rev_it->second.size();
	memcpy(buffer++, ":", 1);
	// param value
	switch(vp.get_type()){
	    // int param
	    case pmink_utils::DPT_INT:
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)vp);
		strcpy((char*)buffer, tmp_str);
		buffer += strlen(tmp_str);
		*result_length += strlen(tmp_str);
		break;

	    // string param
	    case pmink_utils::DPT_STRING:
		strcpy((char*)buffer, (char*)vp);
		buffer += vp.get_size() - 1;
		*result_length += vp.get_size() - 1;
		break;

	    // bool
	    case pmink_utils::DPT_BOOL:
	    {
		bool b = (bool)vp;
		memcpy(buffer++, &b, sizeof(b));
		*result_length += 1;
		break;
	    }

	    // octets
	    case pmink_utils::DPT_OCTETS:
	    {
		unsigned char* raw_data = (unsigned char*)vp;
		for(unsigned int i = 0; i<vp.get_size(); i++){
		    sprintf((char*)buffer, "%02x", raw_data[i]);
		    buffer += 2;

		}
		*result_length += (vp.get_size() * 2);
		break;
	    }

	    default: break;
	}
	// new line
	memcpy(buffer++, "\n", 1);
	// result size (:, newline, label)
	*result_length += 2 + rev_it->second.size();
    }

    // encode size
    snprintf(tmp_str, sizeof(tmp_str), "Size:%d\n", *result_length + 6 + pmink_utils::num_of_digits(*result_length + 6));
    // get size str length
    int szl = strlen(tmp_str);
    // move mem
    memmove(&buff_start[szl], buff_start, *result_length);
    // insert size string
    memcpy(buff_start, tmp_str, szl);
    // result size
    *result_length += szl;



    // ok
    return 0;
}


// RRSession
rrp::RRSession::RRSession(const char* _daemon_id,
			  int _seq_timeout,
			  int _seq_pool,
			  uint32_t _mtu,
			  uint32_t _max_pdu_size):
    seq_timeout(_seq_timeout),
    seq_pool(_seq_pool),
    server_socket(-1),
    server_port(0),
    mtu(_mtu),
    max_pdu_size(_max_pdu_size){

    bzero(server_local_address, sizeof(server_local_address));
    strcpy(daemon_id, _daemon_id);
    init_pdef_map();
}

rrp::RRSession::~RRSession(){
    mtx.lock();
    for(unsigned int i = 0; i<conns.size(); i++){
	delete conns[i];
    }
    conns.clear();
    mtx.unlock();

}


void rrp::RRSession::set_daemon_id(const char* _daemon_id){
    strcpy(daemon_id, _daemon_id);
}

void rrp::RRSession::set_seq_timeout(int _seq_timeout){
    seq_timeout = _seq_timeout;
}

void rrp::RRSession::set_seq_pool(int _seq_pool){
    seq_pool = _seq_pool;
}



void rrp::RRSession::add_conn(RRConnection* conn){
    mtx.lock();
    conns.push_back(conn);
    mtx.unlock();
}


int rrp::RRSession::remove_conn(RRConnection* conn){
    mtx.lock();
    for(unsigned int i = 0; i<conns.size(); i++) if(conns[i] == conn){
	// erase
	conns.erase(conns.begin() + i);
	// free
	delete conn;

	mtx.unlock();
	return 0;


    }

    mtx.unlock();
    return 1;
}


int rrp::RRSession::listen(unsigned int local_port, const char* local_address){
    /* Create UDP Socket */
    int connSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(connSock < 0){
	return 1;
    }
    in_addr addr;
    bzero(&addr, sizeof(addr));
    addr.s_addr = htonl(INADDR_ANY);
    int yes = 1;
    // set socket option
    setsockopt(connSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    // convert local address if specified
    if(local_address != NULL && strlen(local_address) > 0) {
	// use ANY in case of error
	if(inet_aton(local_address, &addr) <= 0) addr.s_addr = htonl(INADDR_ANY);
    }

    // local bind
    sockaddr_in local_bind;
    bzero(&local_bind, sizeof(local_bind) );
    local_bind.sin_family = AF_INET;
    local_bind.sin_addr.s_addr = addr.s_addr;
    local_bind.sin_port = htons(local_port);
    int ret = bind(connSock, (sockaddr *)&local_bind, sizeof(sockaddr_in));
    if(ret < 0){
	shutdown(connSock, SHUT_RDWR);
	close(connSock);
	return 2;
    }

    // set server socket
    server_socket = connSock;
    // set port
    server_port = local_port;
    // address
    strcpy(server_local_address, inet_ntoa(addr));


    // pthread attr
    pthread_attr_t pi_thread_attr;
    // scheduling parameters
    pthread_attr_init(&pi_thread_attr);
    // explicit FIFO scheduling
    pthread_attr_setinheritsched(&pi_thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&pi_thread_attr, SCHED_FIFO);
    // priority
    sched_param pi_sp;
    // max priority
    pi_sp.__sched_priority = 99;
    // set priorities
    pthread_attr_setschedparam(&pi_thread_attr, &pi_sp);
    // start
    thm.start();
    thm.new_thread(&server_loop, "rr_server", this);
    // destroy attributes
    pthread_attr_destroy(&pi_thread_attr);
    // ok
    return 0;
}


void* rrp::RRSession::server_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    RRSession* sess = (RRSession*)ti->user_args;

    int poll_timeout = 1000;
    pollfd fds_lst[1];
    // monitor POLLIN event
    fds_lst[0].events = POLLIN;
    // update socket in poll structure
    fds_lst[0].fd = sess->server_socket;
    // source address
    sockaddr_in src_addr = {0};
    socklen_t src_addr_l = sizeof(src_addr);
    int br;
    int res;
    timespec ts = {0, 1000000};
    RRPDU pdu;
    pmink_utils::VariantParam* vp = NULL;
    pmink_utils::PooledVPMap<uint32_t> tmp_params;
    unsigned char buff[sess->max_pdu_size];
    RRParam* rrp_p = NULL;
    RRConnection* rrc = NULL;
    int enc_l;
    RRSize rrsize;


    // loop
    while(ti->is_active()){
	// poll socket
	res = poll(fds_lst, 1, poll_timeout);
	// check for timeout
	if(res > 0){
	    // check for POLLIN event
	    if((fds_lst[0].revents & POLLIN) == POLLIN){
		// receive data
		br = recvfrom(fds_lst[0].fd, buff, sizeof(buff), 0, (sockaddr*)&src_addr, &src_addr_l);
		// decode
		if(decode(buff, br, &pdu, sess) == 0){
		   // std::cout << "RRP Received... "  << std::endl;
		    //std::cout << "==============="  << std::endl;
		    //buff[br] = 0;
		    //std::cout << buff << std::endl << std::endl;
		    //std::cout << "==============="  << std::endl;
		    // validate size
		    vp = pdu.params.get_param(RRPT_SIZE);
		    // sanity check
		    if(vp == NULL) {
			// todo err stats
			//std::cout << "Size missing..." << std::endl;
			continue;
		    }
		    // decode size
		    if(RRSize::get_size((char*)*vp, &rrsize) != 0){
			// todo err stats
			//std::cout << "Size malformed..." << std::endl;
			continue;
		    }

		    // check if size is correct
		    if(br != rrsize.size){
			// todo err stats
			//std::cout << "Size error..." << std::endl;
			continue;
		    }

		    // check for SEQUENCE STATE
		    vp = pdu.params.get_param(rrp::RRPT_SEQUENCE_STATE);
		    if(vp == NULL) continue;
		    // check for BEGIN
		    rrp_p = sess->find_param_id((char*)*vp);
		    if(rrp_p == NULL || rrp_p->id != RRSST_BEGIN) continue;

		    // check for METHOD
		    vp = pdu.params.get_param(rrp::RRPT_METHOD);
		    if(vp == NULL) continue;
		    // check for REGISTER
		    rrp_p = sess->find_param_id((char*)*vp);
		    if(rrp_p == NULL || rrp_p->id != RRRT_REGISTER) continue;

		    // REGISTER found
		   // std::cout << "!!!!REGISTER!!" << std::endl;





		    // create client
		    //std::cout << "Peer address: " << inet_ntoa(src_addr.sin_addr) << std::endl;
		    //std::cout << "Peer port: " << be16toh(src_addr.sin_port) << std::endl;
		    //std::cout << "Server port: " << sess->server_port << std::endl;
		    //std::cout << "Server address: " << sess->server_local_address << std::endl;


		    rrc = sess->connect(inet_ntoa(src_addr.sin_addr),
				        be16toh(src_addr.sin_port),
				        sess->server_local_address,
				        0,
				        RRCT_SERVER);

		    // save params
		    tmp_params = pdu.params;

		    // send REGISTER END
		    pdu.params.clear_params();
		    // set method
		    pdu.params.set_cstr(RRPT_METHOD, sess->find_rev_str(RRRT_REGISTER));
		    // set sequence state
		    pdu.params.set_cstr(RRPT_SEQUENCE_STATE, sess->find_rev_str(RRSST_END));
		    // session id
		    vp = tmp_params.get_param(RRPT_SESSION_ID);
		    if(vp != NULL && vp->get_size() == 16){
			memcpy(rrc->session_id, (unsigned char*)*vp, vp->get_size());
			pdu.params.set_octets(RRPT_SESSION_ID, (unsigned char*)*vp, vp->get_size());
		    }
		    // call id
		    vp = tmp_params.get_param(RRPT_CALL_ID);
		    if(vp != NULL) pdu.params.set_octets(RRPT_CALL_ID, (unsigned char*)*vp, vp->get_size());
		    // sequence no
		    pdu.params.set_int(RRPT_SEQUENCE, 1);
		    // version
		    pdu.params.set_int(RRPT_VERSION, 1);
		    // status
		    pdu.params.set_cstr(RRPT_STATUS, "200 OK");
		    // destination ctx
		    vp = tmp_params.get_param(RRPT_SRC_CTX);
		    if(vp != NULL) pdu.params.set_cstr(RRPT_DST_CTX, (char*)*vp);
		    // src ctx
		    pdu.params.set_cstr(RRPT_SRC_CTX, sess->daemon_id);
		    // to
		    vp = tmp_params.get_param(RRPT_FROM);
		    if(vp != NULL) pdu.params.set_cstr(RRPT_TO, (char*)*vp);
		    // from
		    pdu.params.set_cstr(RRPT_FROM, sess->server_local_address);
		    // from port
		    pdu.params.set_int(RRPT_FROM_PORT, rrc->local_point_port);
		    // to port
		    vp = tmp_params.get_param(RRPT_FROM_PORT);
		    if(vp != NULL) pdu.params.set_int(RRPT_TO_PORT, (int)*vp);

		    // encode
		    if(pdu.encode(buff, sizeof(buff), 0, &enc_l, sess) == 0){
			// send
			///std::cout << "Server sending..." << std::endl;
			//buff[enc_l] = 0;
			//std::cout << (char*)buff << std::endl;

			sendto(fds_lst[0].fd, buff, enc_l, MSG_NOSIGNAL, (sockaddr*)&src_addr, sizeof(src_addr));
		    }




		}else{
		    // todo stats
		    //std::cout << "DEcode ERROR!!! " << std::endl;
		    //buff[br] = 0;
		    //std::cout << buff << std::endl;
		}


	    }
	// socket idle or error
	}else{
	    // idle
	    if(res == 0){
		nanosleep(&ts, NULL);
		//std::cout << "Socket idle..." << std::endl;

	    // error
	    }else{
		//std::cout << "Socket error..." << std::endl;


	    }
	}
    }
    return NULL;
}


rrp::RRConnection* rrp::RRSession::connect( const char* end_point_address,
					    unsigned int end_point_port,
					    const char* local_address,
					    unsigned int local_port,
					    RRConnectionType direction){

    int ret, connSock;
    struct sockaddr_in local_bind;
    in_addr addr;
    bzero(&addr, sizeof(addr));
    addr.s_addr = htonl(INADDR_ANY);

    /* Create UDP Socket */
    connSock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if(connSock < 0){
	//std::cout << "SOCKET ERROR" << std::endl;
	return NULL;
    }
    int yes = 1;
    // set socket option
    setsockopt(connSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    // convert local address if specified
    if(local_address != NULL && strlen(local_address) > 0) {
	// use ANY in case of error
	if(inet_aton(local_address, &addr) <= 0) addr.s_addr = htonl(INADDR_ANY);
    }

    // local bind
    bzero(&local_bind, sizeof(local_bind) );
    local_bind.sin_family = AF_INET;
    local_bind.sin_addr.s_addr = addr.s_addr;
    local_bind.sin_port = htons(local_port);
    ret = bind(connSock, (sockaddr *)&local_bind, sizeof(sockaddr_in));
    if(ret < 0){
	shutdown(connSock, SHUT_RDWR);
	close(connSock);
	    //std::cout << "BIND ERROR" << std::endl;
	return NULL;
    }

    // socket info
    sockaddr_in si;
    int size_si = sizeof(sockaddr_in);
    // get local socket info
    getsockname(connSock, (sockaddr*)&si, (socklen_t*)&size_si);

    // create client
    RRConnection* rrc = new RRConnection(connSock,
					 end_point_address,
					 end_point_port,
					 inet_ntoa(si.sin_addr),
					 ntohs(si.sin_port),
					 direction,
					 seq_pool,
					 this);

    // add
    add_conn(rrc);

    // client/server
    switch(direction){
	case RRCT_CLIENT:
	    // register, remove if not registered
	    if(rrc->register_client() != 0){
		remove_conn(rrc);
		rrc = NULL;
	    }
	    break;

	case RRCT_SERVER:
	    // do nothing, register done by server
	    rrc->registered.set(true);
	    break;
    }

    // return
    return rrc;
}


const char* rrp::RRSession::find_rev_str(uint32_t id){
    std::map<uint32_t, std::string>::iterator it = rev_param_def_map.find(id);
    if(it == rev_param_def_map.end()) return NULL;
    return it->second.c_str();
}

rrp::RRParam* rrp::RRSession::find_param_id(const char* pstr){
    std::map<uint32_t, RRParam>::iterator it = param_def_map.find(pmink_utils::hash_fnv1a_str(pstr));
    if(it == param_def_map.end()) return NULL;
    return &it->second;
}


rrp::RRStatus* rrp::RRSession::find_status_id(uint32_t id){
    std::map<uint32_t, RRStatus>::iterator it = status_def_map.find(id);
    if(it == status_def_map.end()) return NULL;
    return &it->second;

}


void rrp::RRSession::init_pdef_map(){
    // ***** init status map *****
    status_def_map[RREC_200] 							= RRStatus(RREC_200, "200 OK");
    status_def_map[RREC_401] 							= RRStatus(RREC_401, "401 Unregistered");
    status_def_map[RREC_404] 							= RRStatus(RREC_404, "404 Route not Found");
    status_def_map[RREC_481] 							= RRStatus(RREC_481, "481 Call-Id Does Not Exist");
    status_def_map[RREC_482] 							= RRStatus(RREC_482, "482 Session-Id Does Not Exist");
    status_def_map[RREC_483] 							= RRStatus(RREC_483, "483 Wrong SequenceNo");
    status_def_map[RREC_485] 							= RRStatus(RREC_485, "485 Ambiguous");
    status_def_map[RREC_505] 							= RRStatus(RREC_505, "505 Version Not Supported");

    // ***** init param map *****
    // params
    param_def_map[pmink_utils::hash_fnv1a_str("Size")] 				= RRParam(RRPT_SIZE, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SequenceState")]			= RRParam(RRPT_SEQUENCE_STATE,		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("From")] 				= RRParam(RRPT_FROM,			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("To")] 				= RRParam(RRPT_TO, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("Call-ID")] 			= RRParam(RRPT_CALL_ID,			pmink_utils::DPT_OCTETS);
    param_def_map[pmink_utils::hash_fnv1a_str("Status")] 			= RRParam(RRPT_STATUS, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("Version")] 			= RRParam(RRPT_VERSION, 		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("SequenceNo")] 			= RRParam(RRPT_SEQUENCE, 		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("Found")] 			= RRParam(RRPT_FOUND, 			pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("Config")] 			= RRParam(RRPT_CONFIG, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("Method")] 			= RRParam(RRPT_METHOD, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("Route")] 			= RRParam(RRPT_ROUTE, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SrcTrunk")] 			= RRParam(RRPT_SRC_TRUNK, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("Type")] 				= RRParam(RRPT_TYPE, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("MSISDN")] 			= RRParam(RRPT_MSISDN, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("Session-ID")] 			= RRParam(RRPT_SESSION_ID, 		pmink_utils::DPT_OCTETS);
    param_def_map[pmink_utils::hash_fnv1a_str("FromPort")] 			= RRParam(RRPT_FROM_PORT, 		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("ToPort")] 			= RRParam(RRPT_TO_PORT, 		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("DestinationContext")] 		= RRParam(RRPT_DST_CTX, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SourceContext")] 		= RRParam(RRPT_SRC_CTX, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("MAPV")] 				= RRParam(RRPT_MAP_VERSION, 		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("CdPA")] 				= RRParam(RRPT_GT_CALLED, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("CgPA")] 				= RRParam(RRPT_GT_CALLING, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SCA")] 				= RRParam(RRPT_SCA, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("IMSI")] 				= RRParam(RRPT_IMSI, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("CdSSN")] 			= RRParam(RRPT_CALLED_SSN, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("CgSSN")] 			= RRParam(RRPT_CALLING_SSN, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("NNN")] 				= RRParam(RRPT_NNN, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("GUID")] 				= RRParam(RRPT_GUID, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SMSOriginatingAddress")]		= RRParam(RRPT_SMS_OA_ADDR, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SMSOriginatingAddressTON")]	= RRParam(RRPT_SMS_OA_TON, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("MessageId")]			= RRParam(RRPT_MSG_ID, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("DstTrunk")]			= RRParam(RRPT_DST_TRUNK, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("ConnectionType")]		= RRParam(RRPT_CONN_TYPE, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("ErrorInformation")]		= RRParam(RRPT_ERR_INFO, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("RateDirection")]			= RRParam(RRPT_RATE_DIRECTION, 		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("Direction")]			= RRParam(RRPT_DIRECTION, 		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("InternalStatus")]		= RRParam(RRPT_INTERNAL_STATUS, 	pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("Latency")]			= RRParam(RRPT_LATENCY, 		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("LatencyNsec")]			= RRParam(RRPT_LATENCY_NSEC, 		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("State")]				= RRParam(RRPT_STATE, 			pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("PacketData")]			= RRParam(RRPT_PACKET_DATA,		pmink_utils::DPT_OCTETS);
    param_def_map[pmink_utils::hash_fnv1a_str("Timestamp")]			= RRParam(RRPT_TIMESTAMP,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("TimestampNsec")]			= RRParam(RRPT_TIMESTAMP_NSEC,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("OPC")]				= RRParam(RRPT_OPC,			pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("DPC")]				= RRParam(RRPT_DPC,			pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("RRPMethod")]			= RRParam(RRPT_RRP_METHOD,		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("ErrorCode")]			= RRParam(RRPT_ERROR_CODE,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("ErrorType")]			= RRParam(RRPT_ERROR_TYPE,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("Username")]			= RRParam(RRPT_USERNAME,		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("IPAddress")]			= RRParam(RRPT_IP_ADDRESS,		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("CgTON")]				= RRParam(RRPT_CALLING_TON,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("CgNPI")]				= RRParam(RRPT_CALLING_NPI,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("CdTON")]				= RRParam(RRPT_CALLED_TON,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("CdNPI")]				= RRParam(RRPT_CALLED_NPI,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("TCAPSId")]			= RRParam(RRPT_TCAP_SID,		pmink_utils::DPT_OCTETS);
    param_def_map[pmink_utils::hash_fnv1a_str("TCAPDId")]			= RRParam(RRPT_TCAP_DID,		pmink_utils::DPT_OCTETS);
    param_def_map[pmink_utils::hash_fnv1a_str("SCOA")]				= RRParam(RRPT_SCOA,			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SCOATON")]			= RRParam(RRPT_SCOA_TON,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("SCOANPI")]			= RRParam(RRPT_SCOA_NPI,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("SCDA")]				= RRParam(RRPT_SCDA,			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SCDATON")]			= RRParam(RRPT_SCDA_TON,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("SCDANPI")]			= RRParam(RRPT_SCDA_NPI,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("TCAPOId")]			= RRParam(RRPT_TCAP_OID,		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SMSOriginatingAddressNPI")]	= RRParam(RRPT_SMS_OA_NPI,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("SMSDestinationAddress")]		= RRParam(RRPT_SMS_DA_ADDR,		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SMSDestinationAddressTON")]	= RRParam(RRPT_SMS_DA_TON,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("SMSDestinationAddressNPI")]	= RRParam(RRPT_SMS_DA_NPI,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("SMSTextDCS")]			= RRParam(RRPT_SMS_DCS,			pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("ANNN")]				= RRParam(RRPT_AN,			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("ConcatMsgId")]			= RRParam(RRPT_SMS_CONCAT_MSG_ID,	pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("PartNumber")]			= RRParam(RRPT_SMS_CONCAT_PARTNUM,	pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("Parts")]				= RRParam(RRPT_SMS_CONCAT_PARTS,	pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("SMSText")]			= RRParam(RRPT_SMS_TEXT,		pmink_utils::DPT_OCTETS);
    param_def_map[pmink_utils::hash_fnv1a_str("FilterResult")]			= RRParam(RRPT_FILTER_RESULT,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("FilterExitLabel")]		= RRParam(RRPT_FILTER_EXIT_RULE,	pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("DRMErrorCodeExtra")]		= RRParam(RRPT_DRM_ERR,			pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("Port")]				= RRParam(RRPT_PORT,			pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("mnpMCC")]			= RRParam(RRPT_MNP_MCC,			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("mnpMNC")]			= RRParam(RRPT_MNP_MNC,			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("mnpCached")]			= RRParam(RRPT_MNP_CACHED,		pmink_utils::DPT_INT);
    param_def_map[pmink_utils::hash_fnv1a_str("SMSCIdentifier")]		= RRParam(RRPT_SMSC_ID,			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("MessageState")]			= RRParam(RRPT_MSG_STATE,		pmink_utils::DPT_INT);

    // request types
    param_def_map[pmink_utils::hash_fnv1a_str("REGISTER")] 			= RRParam(RRRT_REGISTER, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("ConfigUpdate-ErrorMaps")] 	= RRParam(RRRT_CFG_UPDATE_ERR_MAPS, 	pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("TemporaryErrorsList")] 		= RRParam(RRRT_TMP_ERR_LIST, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("ROUTE")] 			= RRParam(RRRT_ROUTE, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("HBEAT")] 			= RRParam(RRRT_HBEAT, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("STORE")] 			= RRParam(RRRT_STORE, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("DELETE")] 			= RRParam(RRRT_DELETE, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("WINDOW")] 			= RRParam(RRRT_WINDOW, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("RATE")] 				= RRParam(RRRT_RATE, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("FILTER_RATE")] 			= RRParam(RRRT_FILTER_RATE,		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("MNP")] 				= RRParam(RRRT_MNP,			pmink_utils::DPT_STRING);
    // sequence states
    param_def_map[pmink_utils::hash_fnv1a_str("BEGIN")] 			= RRParam(RRSST_BEGIN, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("CONTINUE")] 			= RRParam(RRSST_CONTINUE, 		pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("END")] 				= RRParam(RRSST_END, 			pmink_utils::DPT_STRING);
    param_def_map[pmink_utils::hash_fnv1a_str("SEQ_END")] 			= RRParam(RRSST_SEQ_END, 		pmink_utils::DPT_STRING);


    // ***** init reverse param map *****
    // params
    rev_param_def_map[RRPT_SIZE] 						= "Size";
    rev_param_def_map[RRPT_SEQUENCE_STATE] 					= "SequenceState";
    rev_param_def_map[RRPT_FROM] 						= "From";
    rev_param_def_map[RRPT_TO] 							= "To";
    rev_param_def_map[RRPT_CALL_ID] 						= "Call-ID";
    rev_param_def_map[RRPT_STATUS] 						= "Status";
    rev_param_def_map[RRPT_VERSION] 						= "Version";
    rev_param_def_map[RRPT_SEQUENCE] 						= "SequenceNo";
    rev_param_def_map[RRPT_FOUND] 						= "Found";
    rev_param_def_map[RRPT_CONFIG] 						= "Config";
    rev_param_def_map[RRPT_METHOD] 						= "Method";
    rev_param_def_map[RRPT_ROUTE] 						= "Route";
    rev_param_def_map[RRPT_SRC_TRUNK] 						= "SrcTrunk";
    rev_param_def_map[RRPT_TYPE] 						= "Type";
    rev_param_def_map[RRPT_MSISDN] 						= "MSISDN";
    rev_param_def_map[RRPT_SESSION_ID] 						= "Session-ID";
    rev_param_def_map[RRPT_FROM_PORT] 						= "FromPort";
    rev_param_def_map[RRPT_TO_PORT] 						= "ToPort";
    rev_param_def_map[RRPT_DST_CTX] 						= "DestinationContext";
    rev_param_def_map[RRPT_SRC_CTX] 						= "SourceContext";
    rev_param_def_map[RRPT_MAP_VERSION] 					= "MAPV";
    rev_param_def_map[RRPT_GT_CALLED] 						= "CdPA";
    rev_param_def_map[RRPT_GT_CALLING] 						= "CgPA";
    rev_param_def_map[RRPT_SCA] 						= "SCA";
    rev_param_def_map[RRPT_IMSI] 						= "IMSI";
    rev_param_def_map[RRPT_CALLED_SSN] 						= "CdSSN";
    rev_param_def_map[RRPT_CALLING_SSN] 					= "CgSSN";
    rev_param_def_map[RRPT_NNN] 						= "NNN";
    rev_param_def_map[RRPT_GUID] 						= "GUID";
    rev_param_def_map[RRPT_SMS_OA_ADDR] 					= "SMSOriginatingAddress";
    rev_param_def_map[RRPT_SMS_OA_TON] 						= "SMSOriginatingAddressTON";
    rev_param_def_map[RRPT_MSG_ID] 						= "MessageId";
    rev_param_def_map[RRPT_DST_TRUNK] 						= "DstTrunk";
    rev_param_def_map[RRPT_CONN_TYPE] 						= "ConnectionType";
    rev_param_def_map[RRPT_ERR_INFO] 						= "ErrorInformation";
    rev_param_def_map[RRPT_RATE_DIRECTION] 					= "RateDirection";
    rev_param_def_map[RRPT_DIRECTION] 						= "Direction";
    rev_param_def_map[RRPT_INTERNAL_STATUS] 					= "InternalStatus";
    rev_param_def_map[RRPT_LATENCY] 						= "Latency";
    rev_param_def_map[RRPT_LATENCY_NSEC]					= "LatencyNsec";
    rev_param_def_map[RRPT_STATE] 						= "State";
    rev_param_def_map[RRPT_PACKET_DATA] 					= "PacketData";
    rev_param_def_map[RRPT_TIMESTAMP] 						= "Timestamp";
    rev_param_def_map[RRPT_TIMESTAMP_NSEC] 					= "TimestampNsec";
    rev_param_def_map[RRPT_OPC] 						= "OPC";
    rev_param_def_map[RRPT_DPC] 						= "DPC";
    rev_param_def_map[RRPT_RRP_METHOD] 						= "RRPMethod";
    rev_param_def_map[RRPT_ERROR_CODE] 						= "ErrorCode";
    rev_param_def_map[RRPT_ERROR_TYPE] 						= "ErrorType";
    rev_param_def_map[RRPT_USERNAME] 						= "Username";
    rev_param_def_map[RRPT_IP_ADDRESS] 						= "IPAddress";
    rev_param_def_map[RRPT_CALLING_TON] 					= "CgTON";
    rev_param_def_map[RRPT_CALLING_NPI] 					= "CgNPI";
    rev_param_def_map[RRPT_CALLED_TON] 						= "CdTON";
    rev_param_def_map[RRPT_CALLED_NPI] 						= "CdNPI";
    rev_param_def_map[RRPT_TCAP_SID] 						= "TCAPSId";
    rev_param_def_map[RRPT_TCAP_DID] 						= "TCAPDId";
    rev_param_def_map[RRPT_SCOA] 						= "SCOA";
    rev_param_def_map[RRPT_SCOA_TON] 						= "SCOATON";
    rev_param_def_map[RRPT_SCOA_NPI] 						= "SCOANPI";
    rev_param_def_map[RRPT_SCDA] 						= "SCDA";
    rev_param_def_map[RRPT_SCDA_TON] 						= "SCDATON";
    rev_param_def_map[RRPT_SCDA_NPI] 						= "SCDANPI";
    rev_param_def_map[RRPT_TCAP_OID] 						= "TCAPOId";
    rev_param_def_map[RRPT_SMS_OA_NPI] 						= "SMSOriginatingAddressNPI";
    rev_param_def_map[RRPT_SMS_DA_ADDR] 					= "SMSDestinationAddress";
    rev_param_def_map[RRPT_SMS_DA_TON] 						= "SMSDestinationAddressTON";
    rev_param_def_map[RRPT_SMS_DA_NPI] 						= "SMSDestinationAddressNPI";
    rev_param_def_map[RRPT_SMS_DCS] 						= "SMSTextDCS";
    rev_param_def_map[RRPT_AN] 							= "ANNN";
    rev_param_def_map[RRPT_SMS_CONCAT_MSG_ID] 					= "ConcatMsgId";
    rev_param_def_map[RRPT_SMS_CONCAT_PARTNUM] 					= "PartNumber";
    rev_param_def_map[RRPT_SMS_CONCAT_PARTS] 					= "Parts";
    rev_param_def_map[RRPT_SMS_TEXT] 						= "SMSText";
    rev_param_def_map[RRPT_FILTER_RESULT] 					= "FilterResult";
    rev_param_def_map[RRPT_FILTER_EXIT_RULE] 					= "FilterExitLabel";
    rev_param_def_map[RRPT_DRM_ERR] 						= "DRMErrorCodeExtra";
    rev_param_def_map[RRPT_PORT] 						= "Port";
    rev_param_def_map[RRPT_MNP_MCC] 						= "mnpMCC";
    rev_param_def_map[RRPT_MNP_MNC] 						= "mnpMNC";
    rev_param_def_map[RRPT_MNP_CACHED] 						= "mnpCached";
    rev_param_def_map[RRPT_SMSC_ID] 						= "SMSCIdentifier";
    rev_param_def_map[RRPT_MSG_STATE] 						= "MessageState";


    // method types
    rev_param_def_map[RRRT_REGISTER] 						= "REGISTER";
    rev_param_def_map[RRRT_CFG_UPDATE_ERR_MAPS] 				= "ConfigUpdate-ErrorMaps";
    rev_param_def_map[RRRT_TMP_ERR_LIST] 					= "TemporaryErrorsList";
    rev_param_def_map[RRRT_ROUTE] 						= "ROUTE";
    rev_param_def_map[RRRT_HBEAT] 						= "HBEAT";
    rev_param_def_map[RRRT_STORE] 						= "STORE";
    rev_param_def_map[RRRT_DELETE] 						= "DELETE";
    rev_param_def_map[RRRT_WINDOW] 						= "WINDOW";
    rev_param_def_map[RRRT_RATE] 						= "RATE";
    rev_param_def_map[RRRT_FILTER_RATE] 					= "FILTER_RATE";
    rev_param_def_map[RRRT_MNP] 						= "MNP";
    // sequence states
    rev_param_def_map[RRSST_BEGIN] 						= "BEGIN";
    rev_param_def_map[RRSST_CONTINUE] 						= "CONTINUE";
    rev_param_def_map[RRSST_END] 						= "END";
    rev_param_def_map[RRSST_SEQ_END] 						= "SEQ_END";


}

int rrp::decode(const unsigned char* data, int data_length, RRPDU* pdu, RRSession* rrs){
    if(pdu == NULL || rrs == NULL || data == NULL || data_length < 7) return 1;
    // tmp copy
    char tmp_data[data_length + 1];
    memcpy(tmp_data, data, data_length);
    // null terminate
    tmp_data[data_length] = 0;
    // tokenize
    char* token;
    char* saveptr;
    char* param_token;
    // clear params
    pdu->params.clear_params();
    // reset param counters
    pdu->reset();
    // get first line (Size)
    token = strtok_r(tmp_data, "\n", &saveptr);
    if(token == NULL) return 1;
    //std::cout << "FIRST TOKEN: " << token << std::endl;
    // copy
    char tmp_param_data[strlen(token) + 1];
    strcpy(tmp_param_data, token);
    char* tmp_param_data_p = tmp_param_data;
    param_token = strsep(&tmp_param_data_p, ":");
    // sanity check
    if(param_token == NULL) return 1;
    // get param id from param string
    RRParam* param_id = rrs->find_param_id(param_token);
    // sanity check
    if(param_id == NULL) return 2;
    // check if size
    if(param_id->id != RRPT_SIZE) return 2;
    // get size value
    param_token = strsep(&tmp_param_data_p, ":");
    // decode size
    RRSize rrsize;
    RRSize::get_size(param_token, &rrsize);

    // add size to param list
    pdu->params.set_cstr((RRParamType)param_id->id, param_token, pdu->pcount[param_id->id]++);
    pdu->params.set_label((RRParamType)param_id->id, rrs->find_rev_str(param_id->id));

    // check if fragmented
    if(rrsize.fragment > 0) return 100;

    // split by newline
    for(/*token = strtok_r(tmp_data, "\n", &saveptr)*/; token != NULL; token = strtok_r(NULL, "\n", &saveptr)){
	// copy
	char tmp_param_data[strlen(token) + 1];
	strcpy(tmp_param_data, token);
	char* tmp_param_data_p = tmp_param_data;
	// tokenize param line, split by :
	//param_token = strtok_r(tmp_param_data, ":", &param_saveptr);
	param_token = strsep(&tmp_param_data_p, ":");
	// sanity check
	if(param_token == NULL) return 1;
	// get param id from param string
	RRParam* param_id = rrs->find_param_id(param_token);
	// sanity check
	if(param_id == NULL) return 2;
	// get param value from param string
	//param_token = strtok_r(NULL, ":", &param_saveptr);
	param_token = strsep(&tmp_param_data_p, ":");
	// sanity check
	if(param_token == NULL) return 1;
	// add to param list
	switch(param_id->type){
	    // int
	    case pmink_utils::DPT_INT:
		pdu->params.set_int((RRParamType)param_id->id, atoi(param_token), pdu->pcount[param_id->id]++);
		pdu->params.set_label((RRParamType)param_id->id, rrs->find_rev_str(param_id->id));
		break;

	    // string
	    case pmink_utils::DPT_STRING:
		pdu->params.set_cstr((RRParamType)param_id->id, param_token, pdu->pcount[param_id->id]++);
		pdu->params.set_label((RRParamType)param_id->id, rrs->find_rev_str(param_id->id));
		break;

	    // bool
	    case pmink_utils::DPT_BOOL:
		pdu->params.set_bool((RRParamType)param_id->id, (strcmp(param_token, "1") == 0 ? true : false), pdu->pcount[param_id->id]++);
		pdu->params.set_label((RRParamType)param_id->id, rrs->find_rev_str(param_id->id));
		break;

	    // octets
	    case pmink_utils::DPT_OCTETS:
	    {
		unsigned char tmp_hex[256];
		unsigned int i = 0, count = 0;
		for(; i<strlen(param_token) && count < sizeof(tmp_hex); i += 2, count++) sscanf(&param_token[i], "%02x", &tmp_hex[count]);
		pdu->params.set_octets((RRParamType)param_id->id, tmp_hex, count, pdu->pcount[param_id->id]++);
		pdu->params.set_label((RRParamType)param_id->id, rrs->find_rev_str(param_id->id));
		break;
	    }

	    // unknown/unsupported
	    default:
		break;
	}

    }

    // set packet data
    pdu->params.set_octets(rrp::RRPT_PACKET_DATA, data, data_length);
    pdu->params.set_label(rrp::RRPT_PACKET_DATA, rrs->find_rev_str(rrp::RRPT_PACKET_DATA));


    //std::cout << "PARAMS" << std::endl;
    //std::cout << pdu->params << std::endl;


    // ok
    return 0;

}

