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
 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <istream>
#include <connection.h>
#include <sctp.h>
#include <arpa/inet.h>
#include <poll.h>
#include <linux/sysctl.h>
#include <sys/syscall.h>
#include <string.h>
#include <r14p_asn1.h>
#include <r14p_utils.h>
#include <daemon.h>
#include <r14p_stats.h>
#include <sgn_stats.h>
#include <netinet/tcp.h>
#include <rrp.h>
#include <error.h>
#include <fcntl.h>

// Payload
sgn::Payload::Payload(): type(CT_UNKNOWN),
			 raw(NULL),
			 size(0),
			 action(PAT_SEND),
			 extra(NULL){

}

// PcapPayload
sgn::PcapPayload::PcapPayload(){
    type = CT_PCAP;

}


// SctpPayload
sgn::SctpPayload::SctpPayload(): ppid(0),
				 sid(0){
    type = CT_SCTP;
    bzero(&sctp_info, sizeof(sctp_info));
    extra = &sctp_info;

}

// TcpPayload
sgn::TcpPayload::TcpPayload(){
    type = CT_TCP;
}

// SmppPayload
sgn::SmppPayload::SmppPayload(){
    type = CT_SMPP;
    // init pool
    //smpp_pdu._pool.init_pool();
}

// M3uaPayload
sgn::M3uaPayload::M3uaPayload(){
    type = CT_M3UA;
    // init pool
   // m3uap._pool.init_pool();

}

// PminkPayload
sgn::PminkPayload::PminkPayload(){
    type = CT_PMINK;
}


// M3UASendErrorHandler
sgn::M3UASendErrorHandler::M3UASendErrorHandler(_M3UAConnection<M3uaPayload>* _m3ua_conn): m3ua_conn(_m3ua_conn){}

void sgn::M3UASendErrorHandler::run(pmink_utils::EventArgs<ConnectiontEventArgType, void*>& args){
    void** pm_arg = args.get_arg(CEAT_PMPLD);
    PminkPayload* pmpld = (PminkPayload*)*pm_arg;
    // dest index
    uint32_t dest_index = 0;
    // check last index (already icremented when msg was received by sgnd, SrvcMsgDone::run)
    pmink_utils::VariantParam* vp = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_routing_index);
    if(vp == NULL){
	// error
	m3ua_conn->stats.inc(sgn::CST_NO_VALID_ROUTES, 1);
	// return to pool
	m3ua_conn->in_pool.deallocate_constructed(pmpld);
	return;
    }

    // set dest_index
    dest_index = (int)*vp;

    // get next destination
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_routing_destination, dest_index);
    if(vp == NULL){
	// error
	m3ua_conn->stats.inc(sgn::CST_NO_VALID_ROUTES, 1);
	// return to pool
	m3ua_conn->in_pool.deallocate_constructed(pmpld);
	return;
    }

    // set client if needed
    if(m3ua_conn->rt_r14pc == NULL || !m3ua_conn->rt_r14pc->is_registered()) m3ua_conn->rt_r14pc = m3ua_conn->r14ps->get_registered_client("routingd");
    // allocate new service message
    r14p::ServiceMessage* smsg = m3ua_conn->r14psmm->new_smsg();
    if(smsg != NULL){
	// copy data
	smsg->vpmap = pmpld->params;
	// set service id
	smsg->set_service_id(asn1::ServiceId::_sid_sgn_forward);
	// sync vpmap
	if(m3ua_conn->r14psmm->vpmap_sparam_sync(smsg) == 0){

	    // send
	    if(m3ua_conn->r14psmm->send(smsg,
					m3ua_conn->rt_r14pc,
					(char*)*vp,
					NULL,
					true,
					(r14p::R14PCallbackMethod*)pmink::CURRENT_DAEMON->get_param(3)) != 0){
		// error
		m3ua_conn->stats.inc(sgn::CST_SMSG_SEND_ERROR, 1);
		// free
		m3ua_conn->r14psmm->free_smsg(smsg);

	    }

	}else{
	    // error
	    m3ua_conn->stats.inc(sgn::CST_SMSG_PARAM_POOL_EMPTY, 1);
	    // free
	    m3ua_conn->r14psmm->free_smsg(smsg);
	}

    }else{
	// error
	m3ua_conn->stats.inc(sgn::CST_SMSG_MSG_POOL_EMPTY, 1);

    }



    // return to pool
    m3ua_conn->in_pool.deallocate_constructed(pmpld);

}

// SMPPConnection
sgn::SMPPConnection::SMPPConnection(ConnectionDirection _direction,
				    ServerConnectionType _server_type,
				    const char* _label,
				    int _max_mtu,
				    int _pool_size): _SMPPConnection<SmppPayload>(_direction,
										  _server_type,
										  _label,
										  _max_mtu,
										  _pool_size){

}


// _SMPPConnection
template<typename PLD_TYPE>
sgn::_SMPPConnection<PLD_TYPE>::_SMPPConnection(ConnectionDirection _direction,
						ServerConnectionType _server_type,
			   		        const char* _label,
						int _max_mtu,
						int _pool_size): _TCPConnection<PLD_TYPE>( _direction,
											    _server_type,
											    _label,
											    _max_mtu,
											    _pool_size),
											    conn_phase(0),
											    retain_data(false),
											    bind_ts(0),
											    terminated(false),
											    sep_buff_p(sep_buff),
											    sep_buff_l(0),
											    next_pdu_l(0),
											    next_pdu_rb(0){
    // connection type
    Connection<PLD_TYPE>::type = CT_SMPP;

    // default params
    Connection<PLD_TYPE>::params.set_int(CPT_SMPP_BIND_METHOD, smpp::BIND_TRANSCEIVER);
    Connection<PLD_TYPE>::params.set_int(CPT_SMPP_ENQUIRE_LINK_TIMER, 10);
    Connection<PLD_TYPE>::params.set_int(CPT_SMPP_INACTIVITY_TIMER, 60);
    Connection<PLD_TYPE>::params.set_int(CPT_SMPP_RESPONSE_TIMER, 5);
    Connection<PLD_TYPE>::params.set_int(CPT_SMPP_SESSION_INIT_TIMER, 10);
    Connection<PLD_TYPE>::params.set_cstr(CPT_SMPP_BIND_SYSTEM_ID, "");
    Connection<PLD_TYPE>::params.set_cstr(CPT_SMPP_BIND_PASSWORD, "");
    Connection<PLD_TYPE>::params.set_cstr(CPT_SMPP_BIND_SYSTEM_TYPE, "SMPP");
    Connection<PLD_TYPE>::params.set_int(CPT_SMPP_BIND_IF_VERSION, 0x34);
    Connection<PLD_TYPE>::params.set_int(CPT_MAX_CONN_COUNT, 1);
    Connection<PLD_TYPE>::params.set_bool(CPT_DATA_RETENTION, false);
    Connection<PLD_TYPE>::params.set_int(CPT_RECONNECT_TIMER, 5);
    Connection<PLD_TYPE>::params.set_bool(CPT_DEV_SMPP_SM, true);

    // enquire link
    enql_ts = 10;
    enql_last_ts = time(NULL);
    enql_enabled = true;

    // default bind timeout
    bind_timeout = 5;

    // reconnect timer
    Connection<PLD_TYPE>::reconnect_timer = 5;

    // pools
    smpp_rx_pool.init_pool();
    smpp_proc_pool.init_pool();
    enql_smppp._pool.set_pool_size(1, 1);
    enql_smppp._pool.init_pool();


    // converters
    UErrorCode icu_err = U_ZERO_ERROR;
    conv_utf16be = ucnv_open("utf-16be", &icu_err);
    iconv_gsm7_to_utf8 = libiconv_open("UTF-8", "GSM-7");
    iconv_utf8_to_gsm7 = libiconv_open("GSM-7", "UTF-8");


    // register stats
    ConnectionBase::stats.register_item(CST_MTU_LIMIT);
    ConnectionBase::stats.register_item(CST_DISCARDED_MSU);
    ConnectionBase::stats.register_item(CST_PLD_POOL_EMPTY);
    ConnectionBase::stats.register_item(CST_MALFORMED_MSU);
    ConnectionBase::stats.register_item(CST_PMPLD_POOL_EMPTY);
    ConnectionBase::stats.register_item(CST_SMSG_SEND_ERROR);
    ConnectionBase::stats.register_item(CST_SMSG_MSG_POOL_EMPTY);
    ConnectionBase::stats.register_item(CST_SMSG_PARAM_POOL_EMPTY);
    ConnectionBase::stats.register_item(CST_SMPP_ACTIVE);
    ConnectionBase::stats.register_item(CST_TCP_ACTIVE);
    ConnectionBase::stats.register_item(CST_SMSG_SENT_COUNT);
    ConnectionBase::stats.register_item(CST_RX_HBEAT_COUNT);
    ConnectionBase::stats.register_item(CST_TX_HBEAT_COUNT);
    ConnectionBase::stats.register_item(CST_CMD_PROCESS_ERROR);
    ConnectionBase::stats.register_item(CST_ENCODE_ERROR);
    ConnectionBase::stats.register_item(CST_RECONNECT);

    // get r14p session manager
    r14ps = (r14p::R14PSession*)pmink::CURRENT_DAEMON->get_param(2);
    // get r14p service msg manager
    r14psmm = (r14p::ServiceMsgManager*)pmink::CURRENT_DAEMON->get_param(1);
    // data retention r14p client
    dr_r14pc = r14ps->get_registered_client("routingd");
    // stp r14p client
    stp_r14pc = r14ps->get_registered_client("routingd");
    // router r14p client
    rt_r14pc = r14ps->get_registered_client("routingd");

    // seq nums
    out_seq_num.set(0);
}

template<typename PLD_TYPE>
sgn::_SMPPConnection<PLD_TYPE>::~_SMPPConnection(){
    // master
    terminate();
    stop();
    // one of child connections
    if(ConnectionBase::master_conn != NULL){
	// get smpp master conn
	SMPPConnection* smpp_master_conn = (SMPPConnection*)ConnectionBase::master_conn;
	// remove from list
	smpp_master_conn->usr_map_mtx.lock();
	// remove from children list
	int rc = smpp_master_conn->clients.remove(ConnectionBase::label.c_str());
	smpp_master_conn->usr_map_mtx.unlock();
	// dec master conn user bind counter if client removal was successful
	if(rc == 0) smpp_master_conn->bind_dec(smpp_user.system_id);
	// dec master ref
	smpp_master_conn->master_ref.fetch_sub(1);

    }
    // close converters
    ucnv_close(conv_utf16be);
    libiconv_close(iconv_gsm7_to_utf8);
    libiconv_close(iconv_utf8_to_gsm7);

    // free logic
    delete ConnectionBase::logic;
}

template<typename PLD_TYPE>
bool sgn::_SMPPConnection<PLD_TYPE>::refc_condition(){
    return (ConnectionBase::master_ref.get() <= 0);
}


template<typename PLD_TYPE>
void sgn::_SMPPConnection<PLD_TYPE>::stop_children(){
    usr_map_mtx.lock();
    for(typename pmink_utils::WRR<SMPPConnection*>::items_map_it_t it = clients.begin(); it != clients.end(); it++){
	(*it).item->reconnect_queued.comp_swap(false, true);
    }

    usr_map_mtx.unlock();
}


template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::bind_dec(const char* _sustem_id){
    // lock
    usr_map_mtx.lock();

    // find user
    std::map<std::string, SMPPUser>::iterator it = usr_map.find(std::string(_sustem_id));
    // check if found
    if(it == usr_map.end()){
	// unlcok
	usr_map_mtx.unlock();
	// auth err
	return -1;
    }

    // found, dec conn count or reset to zero
    int res = 0;
    if(it->second.conns > 0) res = --it->second.conns;
    else it->second.conns = 0;
    // unlock
    usr_map_mtx.unlock();
    // auth ok
    return res;

}


template<typename PLD_TYPE>
bool sgn::_SMPPConnection<PLD_TYPE>::bind_auth(const char* system_id, const char* password){
    // lock
    usr_map_mtx.lock();

    // find user
    std::map<std::string, SMPPUser>::iterator it = usr_map.find(std::string(system_id));
    // check if found
    if(it == usr_map.end()){
	// unlcok
	usr_map_mtx.unlock();
	// auth err
	return false;
    }

    // found, check pwd and conn count
    SMPPUser& usr = it->second;
    if(strcmp(usr.password, password) == 0 && usr.conns < usr.max_conns){
	// inc conns
	++usr.conns;
	// unlock
	usr_map_mtx.unlock();
	// auth ok
	return true;
    }

    // unlock
    usr_map_mtx.unlock();
    // auth err
    return false;
}


template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::extra_logic(){
    // skip if state machine is off
    if(!enql_enabled) return 10;
    // connection shutting down
    if(!ConnectionBase::active.get()) return 10;
    if(!Connection<PLD_TYPE>::cthreads_active.get()) return 10;
    if(Connection<PLD_TYPE>::reconnecting.get()) return 10;
    // check time diff
    if(time(NULL) - enql_last_ts < enql_ts) return 10;
    // update ts
    enql_last_ts = time(NULL);
     // check if smpp is active
    if(!smpp_active.get()) return 10;
    // if no enquire_link_resp was received, set smpp as inactive
    if(enql_diff.get() > 0){
	// flag for reconnection
	Connection<PLD_TYPE>::reconnect_queued.comp_swap(false, true);
	// reset counters
	enql_diff.set(0);
	return 10;
    }

    // inc counter
    enql_diff.fetch_add(1);

    unsigned char buff[64];
    int size = 0;

    // ENQUIRE LINK
    smpp::SmppCommand* tmpcmd = enql_smppp._pool.request_command(smpp::ENQUIRE_LINK);
    if(tmpcmd != NULL){
	// set params
	enql_smppp.command_id = smpp::ENQUIRE_LINK;
	enql_smppp.command_status = smpp::ESME_ROK;
	enql_smppp.command = tmpcmd;
	enql_smppp.command->clear_params();
	enql_smppp.sequence_number = out_seq_num.add_fetch(1);
	// encode
	if(enql_smppp.encode(buff, sizeof(buff), 0, &size) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
	    return 10;
	}

	// send
	if(send(ConnectionBase::client_socket, buff, size, MSG_NOSIGNAL) <= 0){
	    // socket down, flag for reconnection
	    Connection<PLD_TYPE>::reconnect_queued.comp_swap(false, true);

	}else{
	    // stats
	    ConnectionBase::stats.inc(CST_TX_MSU_COUNT, 1);

	}
    }

    return 10;
}



template<typename PLD_TYPE>
sgn::ConnectionBase* sgn::_SMPPConnection<PLD_TYPE>::get_conn(const char* id, bool inc_refc){
    // one to one connection
    if(Connection<PLD_TYPE>::max_in_connc == 1){
	// if smpp layer not fully established, return NULL
	if(smpp_active.get()) {
	    if(inc_refc) this->refc_inc();
	    return this;
	}else return NULL;
    }

    // one to many, use WRR
    usr_map_mtx.lock();
    pmink_utils::WRRItem<sgn::SMPPConnection*>* wrr_item = NULL;
    if(id != NULL){
	wrr_item = clients.get(id);
	if(wrr_item == NULL) wrr_item = clients.run();
    }else wrr_item = clients.run();
    sgn::ConnectionBase* res = (wrr_item != NULL ? (sgn::ConnectionBase*)wrr_item->item : NULL);
    if(res != NULL && inc_refc) res->refc_inc();
    usr_map_mtx.unlock();

    return res;

}

template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::convert(PminkPayload* pmpld){
    // get connection type
    pmink_utils::VariantParam* vp_conn_type = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_connection_type);
    if(vp_conn_type == NULL) return 10;

    // get current tcap component index
    pmink_utils::VariantParam* vp_tcap_comp_index = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_component_index);
    if(vp_tcap_comp_index == NULL) return 11;

    // get sms text data
    pmink_utils::VariantParam* vp_tpdu_ud = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_ud, (int)*vp_tcap_comp_index);
    if(vp_tpdu_ud == NULL) return 12;

    // get destination addr
    pmink_utils::VariantParam* vp_msisdn = pmpld->params.get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, 2);
    if(vp_msisdn == NULL) return 13;

    // get source addr
    pmink_utils::VariantParam* vp_src_addr = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, (int)*vp_tcap_comp_index);
    if(vp_src_addr == NULL) return 14;
    // get source ton
    smpp::SmppTON src_ton = smpp::TON_INTERNATIONAL;
    pmink_utils::VariantParam* vp_src_addr_ton = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, (int)*vp_tcap_comp_index);
    // get DCS
    pmink_utils::VariantParam* vp_dcs = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_dcs, (int)*vp_tcap_comp_index);
    if(vp_dcs == NULL) return 15;
    // get pid
    pmink_utils::VariantParam* vp_pid = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_pid, (int)*vp_tcap_comp_index);
    if(vp_pid == NULL) return 16;
    // get message class
    pmink_utils::VariantParam* vp_msg_class = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_message_class, (int)*vp_tcap_comp_index);
    // default customer encoding
    pmink_utils::VariantParam* vp_smpp_customer_enc = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_default_customer_encoding);
    // default vendoer encoding
    pmink_utils::VariantParam* vp_smpp_vendor_enc = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_default_vendor_encoding);

    // sanity check and convert SMSTPDU TON -> SMPP TON
    if(vp_src_addr_ton != NULL){
	switch((int)*vp_src_addr_ton){
	    case smstpdu::TON_INTERNATIONAL:
		src_ton = smpp::TON_INTERNATIONAL;
		break;

	    case smstpdu::TON_NATIONAL:
		src_ton = smpp::TON_NATIONAL;
		break;

	    case smstpdu::TON_NETWORK_SPECIFIC:
		src_ton = smpp::TON_NETWORK_SPECIFIC;
		break;

	    case smstpdu::TON_SUBSCRIBER_NUMBER:
		src_ton = smpp::TON_SUBSCRIBER_NUMBER;
		break;

	    case smstpdu::TON_ALPHANUMERIC:
		src_ton = smpp::TON_ALPHANUMERIC;
		break;

	    case smstpdu::TON_ABBREVIATED:
		src_ton = smpp::TON_ABBREVIATED;
		break;

	}

    }
    // get source numbering plan
    smpp::SmppNPI src_npi = smpp::ISDN_TELEPHONE;
    pmink_utils::VariantParam* vp_src_npi = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan);
    // sanity check and convert SMSTPDU NP -> SMPP NP
    if(vp_src_npi != NULL){
	switch((int)*vp_src_npi){
	    case smstpdu::NP_ISDN_TELEPHONE:
		src_npi = smpp::ISDN_TELEPHONE;
		break;

	    case smstpdu::NP_DATA_X121:
		src_npi = smpp::DATA_X121;
		break;

	    case smstpdu::NP_TELEX:
		src_npi = smpp::TELEX;
		break;

	    case smstpdu::NP_LAND_MOBILE:
		src_npi = smpp::LAND_MOBILE;
		break;

	    case smstpdu::NP_NATIONAL:
		src_npi = smpp::NATIONAL;
		break;

	    case smstpdu::NP_PRIVATE:
		src_npi = smpp::PRIVATE;
		break;

	    case smstpdu::NP_ERMES:
		src_npi = smpp::ERMES;
		break;

	}
    }

    // get connection type
    ConnectionType ctype = (ConnectionType)(int)*vp_conn_type;
    // check if converting M3UA -> SMPP
    if(ctype == CT_M3UA){
	// check if concatenated
	bool concat = false;
	pmink_utils::VariantParam* vp_msg_part = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_part);
	pmink_utils::VariantParam* vp_msg_parts = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_parts);
	pmink_utils::VariantParam* vp_msg_id = pmpld->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_id);
	if(vp_msg_part != NULL && vp_msg_parts != NULL && vp_msg_id != NULL) concat = true;

	smpp::Submit_sm* dsm = (smpp::Submit_sm*)smpp_proc_pool.request_command(smpp::SUBMIT_SM);
	// sanity check
	if(dsm == NULL) return 1;
	// reset optional
	dsm->params.clear();
	// set smppp pointer
	smpp::SmppPDU* smppp = &pmpld->smpp_pld.smpp_pdu;
	// set pdu command
	smppp->command = dsm;
	smppp->command_id = smpp::SUBMIT_SM;
	// set status and seq num
	smppp->command_status = smpp::ESME_ROK;
	smppp->sequence_number = 0;//out_seq_num.add_fetch(1);
	// set command params
	char* srvc_type = (char*)"";
	char* src_addr = (char*)*vp_src_addr;
	char* dst_addr = (char*)*vp_msisdn;
	char* null_str = (char*)"";
	char* short_msg = NULL;
	unsigned char tmp_data_coding_buff[1];
	int short_msg_l = 0;
	unsigned char text_buff[256];
	unsigned char pack_buff[256];
	unsigned char tp_ud_buff[160];
	unsigned char* text_buff_p = text_buff;
	int text_buff_l = 0;
	int text_enc_l = 0;
	int calc_size = 0;

	// if UCS2, re-encode to big endian two bytes per character
	if((int)*vp_dcs == smstpdu::TPDCSAT_UCS2){
	    // read decoded utf-8
	    icu::UnicodeString ucs2((char*)(unsigned char*)*vp_tpdu_ud, vp_tpdu_ud->get_size(), "utf-8");
	    // error res
	    UErrorCode icu_err = U_ZERO_ERROR;
	    // convert to utf-16 big endian
	    int tmp_size = ucs2.extract((char*)text_buff, sizeof(text_buff), conv_utf16be, icu_err);
	    // error check
	    if(icu_err != U_ZERO_ERROR) return 1;
	    // set sms text pointer
	    short_msg = (char*)text_buff;
	    // set length in octets
	    short_msg_l = tmp_size;

	// if GSM-7
	}else if((int)*vp_dcs == smstpdu::TPDCSAT_GSM7){
	    //short_msg = (char*)(unsigned char*)*vp_tpdu_ud;
	    //short_msg_l = vp_tpdu_ud->get_size();

	    // check if using unpacked
	    if(vp_smpp_vendor_enc != NULL && (int)*vp_smpp_vendor_enc == rrp::RRDET_ASCII){
		// set sms text pointer
		short_msg = (char*)(unsigned char*)*vp_tpdu_ud;
		// set length in octets
		short_msg_l = vp_tpdu_ud->get_size();


	    // using packed, re-encode and re-pack to septets
	    }else{
		// set pointers
		unsigned char* sms_text_p = (unsigned char*)*vp_tpdu_ud;
		size_t sms_text_l = vp_tpdu_ud->get_size();
		text_enc_l = sizeof(text_buff);
		// encode
		size_t iconv_ret = libiconv(iconv_utf8_to_gsm7,
					    (char**)&sms_text_p,
					    &sms_text_l,
					    (char**)&text_buff_p,
					    (size_t*)&text_enc_l);

		if(iconv_ret == (size_t)-1) {
		    // flush
		    libiconv(iconv_utf8_to_gsm7, NULL, NULL, (char**)&text_buff_p, (size_t*)&text_enc_l);
		    return 1;
		}
		// calc size
		calc_size = sizeof(text_buff) - text_enc_l;
		// flush
		libiconv(iconv_utf8_to_gsm7, NULL, NULL, (char**)&text_buff_p, (size_t*)&text_enc_l);
		// sizes
		text_enc_l = calc_size;
		text_buff_l = text_enc_l;
		// pack
		if(smstpdu::GsmAlphabet::pack((char*)text_buff, text_buff_l, 0, sizeof(pack_buff), pack_buff, &text_enc_l) != 0) return 1;
		// set sms text pointer
		short_msg = (char*)pack_buff;
		// set length in octets
		short_msg_l = text_enc_l;


	    }




	    /*
	    // repack
	    smstpdu::GsmAlphabet::encode((char*)(unsigned char*)*vp_tpdu_ud, vp_tpdu_ud->get_size(), 0, sizeof(text_buff), text_buff, &text_enc_l);
	    // set sms text pointer
	    short_msg = (char*)text_buff;
	    // set length in octets
	    short_msg_l = text_enc_l;
	    */

	// use original message
	}else{
	    // set sms text pointer
	    short_msg = (char*)(unsigned char*)*vp_tpdu_ud;
	    // set length in octets
	    short_msg_l = vp_tpdu_ud->get_size();

	}

	// sanity check
	if(short_msg == NULL || short_msg_l == 0) return 1;

	// service type
	dsm->service_type = srvc_type;
	// source
	dsm->source_addr_ton = src_ton;
	dsm->source_addr_npi = src_npi;
	dsm->source_addr = src_addr;
	// dest
	dsm->dest_addr_ton = smpp::TON_INTERNATIONAL;
	dsm->dest_addr_npi = smpp::ISDN_TELEPHONE;
	dsm->destination_addr = dst_addr;
	// esm class
	dsm->esm_class_mm = smpp::MM_DEFAULT_SMSC_MODE;
	dsm->esm_class_mt = smpp::MT_DEFAULT;
	dsm->esm_gsm_specific = (concat ? smpp::GNS_UDHI_INDICATOR : smpp::GNS_NO_SPECIFIC_FEATURES);
	// protocol id
	dsm->protocol_id = (int)*vp_pid;
	// priority level
	dsm->priority_flag = 0;
	// scheduled delivery
	dsm->schedule_delivery_time = null_str;
	// validity period
	dsm->validity_period = null_str;
	// registered delivery
	dsm->rd_smsc_dlvr_rcpt = smpp::SDR_NO_SMSC_DELIVERY;
	dsm->rd_sme_orig_ack = smpp::SOA_NO_SME_ACK;
	dsm->rd_intrmd_ntf = smpp::INT_NO;
	// replace if present
	dsm->replace_if_present_flag = 0;
	// if message class, use special data coding
	if(vp_msg_class != NULL){
	    tp_dcs_dcoding.message_class = (smstpdu::TPDCSMessageClassType)(int)*vp_msg_class;
	    tp_dcs_dcoding.alphabet = (smstpdu::TPDCSAlphabetType)(int)*vp_dcs;
	    int tmp_enc_l = 0;
	    tp_dcs_dcoding.encode(tmp_data_coding_buff, sizeof(tmp_data_coding_buff), 0, &tmp_enc_l);
	    dsm->data_coding = (smpp::Data_codingType)tmp_data_coding_buff[0];


	}else{
	    // data coding
	    switch((smstpdu::TPDCSAlphabetType)(int)*vp_dcs){
		// gsm 7bit
		case smstpdu::TPDCSAT_GSM7:
		    dsm->data_coding = smpp::DC_DEFAULT;
		    break;

		// binary
		case smstpdu::TPDCSAT_8BIT:
		    dsm->data_coding = smpp::DC_8BIT_BINARY_1;
		    break;


		// ucs2
		case smstpdu::TPDCSAT_UCS2:
		    dsm->data_coding = smpp::DC_UCS2;
		    break;

		// unknown/unsupported
		default:
		    dsm->data_coding = smpp::DC_8BIT_BINARY_1;
		    break;

	    }

	}



	// concatenated msg
	if(concat){
	    // if udh and 7bit, repack with padding
	    if((int)*vp_dcs == smstpdu::TPDCSAT_GSM7){

		// check if using unpacked
		if(vp_smpp_vendor_enc != NULL && (int)*vp_smpp_vendor_enc == rrp::RRDET_ASCII){
		    // set tp-ud
		    tp_ud.value = (unsigned char*)short_msg;
		    tp_ud.length = short_msg_l;

		// using packed, re-encode and re-pack to septets
		}else{
		    // repack
		    int text_enc_l = 0;
		    // padding = 7 - ((UDHL + 1 ) * 8 ) % 7
		    //smstpdu::GsmAlphabet::encode((char*)(unsigned char*)*vp_tpdu_ud, vp_tpdu_ud->get_size(), 7 - ((6 * 8) % 7), sizeof(text_buff), text_buff, &text_enc_l);
		    smstpdu::GsmAlphabet::pack((char*)text_buff, text_buff_l, 7 - ((6 * 8) % 7), sizeof(pack_buff), pack_buff, &text_enc_l);
		    // set tp-ud
		    tp_ud.value = pack_buff;
		    tp_ud.length = text_enc_l;

		}

	    // use original message
	    }else{
		// set tp-ud
		tp_ud.value = (unsigned char*)short_msg;
		tp_ud.length = short_msg_l;

	    }

	    // set tp-ud
	    //tp_ud.value = (unsigned char*)short_msg;
	    //tp_ud.length = short_msg_l;
	    // udh
	    tp_ud.udh.ie_lst.clear();
	    smstpdu::TP_UDHIEConc8BitRef* ie = (smstpdu::TP_UDHIEConc8BitRef*)tp_ud.udh.ie_pool.request_ie(smstpdu::TPUDHIET_CONCATENATED_8BIT_REF);
	    tp_ud.udh.ie_lst.push_back(ie);
	    ie->msg_id = (int)*vp_msg_id;
	    ie->msg_part = (int)*vp_msg_part;
	    ie->msg_parts = (int)*vp_msg_parts;

	    // encode tp-ud
	    int udh_enc_l = 0;
	    tp_ud.udh.encode(tp_ud_buff, sizeof(tp_ud_buff), 0, &udh_enc_l);
	    // add encoded text
	    memcpy(&tp_ud_buff[udh_enc_l], tp_ud.value, tp_ud.length);
	    // add to short_message param
	    dsm->short_message = tp_ud_buff;
	    // set sm length
	    dsm->sm_length = udh_enc_l + tp_ud.length;

	// single msg
	}else{
	    // short message
	    dsm->short_message = (unsigned char*)short_msg;
	    // sm length
	    dsm->sm_length = short_msg_l;

	}


	// predefined message
	dsm->sm_default_msg_id = 0;
	// encode
	if(smppp->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
	    return 1;
	}
	// set header data
	pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_header_data, pmpld->raw, pmpld->size);
	// ok
	return 0;
    }
    // err
    return 1;
}

template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::cmd_process(PminkPayload* pmpld, int cmd_id){
    switch(cmd_id){
	// generate sms ack
	case asn1::SmsHubCommandId::_shci_sms_ack:
	{

	    // get sequence
	    pmink_utils::VariantParam* vp_smpp_seq = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_sequence);
	    if(vp_smpp_seq == NULL) return 10;

	    // get pdu type
	    pmink_utils::VariantParam* vp_smpp_pdu = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_command_id);
	    if(vp_smpp_pdu == NULL) return 11;

	    // get guid
	    pmink_utils::VariantParam* vp_guid = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_guid);
	    if(vp_guid == NULL) return 12;

	    // set smppp pointer
	    smpp::SmppPDU* smppp = &pmpld->smpp_pld.smpp_pdu;

	    // buff for guid conversion
	    char guid_str[33];
	    bzero(guid_str, sizeof(guid_str));
	    char* guid_strp = guid_str;

	    switch((int)*vp_smpp_pdu){
		// submit_sm
		case smpp::SUBMIT_SM:
		{
		    smpp::Submit_sm_resp* dsm = (smpp::Submit_sm_resp*)smpp_proc_pool.request_command(smpp::SUBMIT_SM_RESP);
		    // sanity check
		    if(dsm == NULL) return 20;
		    // reset optional
		    dsm->params.clear();
		    // set smppp pointer
		    smpp::SmppPDU* smppp = &pmpld->smpp_pld.smpp_pdu;
		    // set pdu command
		    smppp->command = dsm;
		    smppp->command_id = smpp::SUBMIT_SM_RESP;
		    // set status and seq num
		    smppp->command_status = smpp::ESME_ROK;
		    smppp->sequence_number = (int)*vp_smpp_seq;
		    // status
		    pmink_utils::VariantParam* vp_status = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_command_status);
		    if(vp_status != NULL) smppp->command_status = (smpp::SmppCommandStatus)(int)*vp_status;
		    else smppp->command_status = smpp::ESME_ROK;

		    // guid
		    if(vp_guid->get_type() == pmink_utils::DPT_OCTETS){
			unsigned char* raw_data = (unsigned char*)*vp_guid;
			int gc = (vp_guid->get_size() > 16 ? 16 : vp_guid->get_size());
			for(unsigned int i = 0; i<gc; i++){
			    sprintf(guid_strp, "%02x", raw_data[i]);
			    guid_strp += 2;
			}
			dsm->message_id = guid_str;

		    }else dsm->message_id = (char*)*vp_guid;
		    break;
		}

		// deliver_sm
		case smpp::DELIVER_SM:
		{
		    smpp::Deliver_sm_resp* dsm = (smpp::Deliver_sm_resp*)smpp_proc_pool.request_command(smpp::DELIVER_SM_RESP);
		    // sanity check
		    if(dsm == NULL) return 20;
		    // reset optional
		    dsm->params.clear();
		    // set smppp pointer
		    smpp::SmppPDU* smppp = &pmpld->smpp_pld.smpp_pdu;
		    // set pdu command
		    smppp->command = dsm;
		    smppp->command_id = smpp::DELIVER_SM_RESP;
		    // set status and seq num
		    smppp->command_status = smpp::ESME_ROK;
		    smppp->sequence_number = (int)*vp_smpp_seq;
		    dsm->message_id = (char*)smpp::SMPP_NULL_STR;
		    // status
		    pmink_utils::VariantParam* vp_status = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_command_status);
		    if(vp_status != NULL) smppp->command_status = (smpp::SmppCommandStatus)(int)*vp_status;
		    else smppp->command_status = smpp::ESME_ROK;
		    break;
		}

		// unknown
		default:
		    return 100;
	    }



	    // encode
	    if(smppp->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size) != 0){
		// stats
		ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
		return 101;
	    }
	    // set header data
	    pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_header_data, pmpld->raw, pmpld->size);
	    // ok
	    return 0;
	}

	// generate sms delivery receipt
	case asn1::SmsHubCommandId::_shci_sms_dlvr_rcpt:
	{
	    // get src addr
	    pmink_utils::VariantParam* vp_src_addr = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_originator_address);
	    if(vp_src_addr == NULL) return 10;
	    // get src ton
	    pmink_utils::VariantParam* vp_src_addr_ton = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_originator_ton);
	    if(vp_src_addr_ton == NULL) return 11;
	    // get src np
	    pmink_utils::VariantParam* vp_src_addr_np = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_originator_np);
	    if(vp_src_addr_np == NULL) return 12;
	    // get dst addr
	    pmink_utils::VariantParam* vp_dst_addr = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_recipient_address);
	    if(vp_dst_addr == NULL) return 11;
	    // get dst ton
	    pmink_utils::VariantParam* vp_dst_addr_ton = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_recipient_ton);
	    if(vp_dst_addr_ton == NULL) return 12;
	    // get dst np
	    pmink_utils::VariantParam* vp_dst_addr_np = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_recipient_np);
	    if(vp_dst_addr_np == NULL) return 12;
	    // get sm length
	    pmink_utils::VariantParam* vp_sm_l = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_sm_length);
	    if(vp_sm_l == NULL) return 12;
	    // get smpp delivery receipt id
	    pmink_utils::VariantParam* vp_dlvr_rcpt_id = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_dlvr_rcpt_id);
	    if(vp_dlvr_rcpt_id == NULL) return 12;
	    // get smpp delivery receipt sub
	    pmink_utils::VariantParam* vp_dlvr_rcpt_sub = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_dlvr_rcpt_sub);
	    if(vp_dlvr_rcpt_sub == NULL) return 12;
	    // get smpp delivery receipt dlvrd
	    pmink_utils::VariantParam* vp_dlvr_rcpt_dlvrd = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_dlvr_rcpt_dlvrd);
	    if(vp_dlvr_rcpt_dlvrd == NULL) return 12;
	    // get smpp delivery receipt done date
	    pmink_utils::VariantParam* vp_dlvr_rcpt_done_date = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_dlvr_rcpt_done_date);
	    if(vp_dlvr_rcpt_done_date == NULL) return 12;
	    // get smpp delivery receipt stat
	    pmink_utils::VariantParam* vp_dlvr_rcpt_stat = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_dlvr_rcpt_stat);
	    if(vp_dlvr_rcpt_stat == NULL) return 12;
	    // get smpp delivery receipt err
	    pmink_utils::VariantParam* vp_dlvr_rcpt_err = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_dlvr_rcpt_err);
	    if(vp_dlvr_rcpt_err == NULL) return 12;
	    // get smpp delivery receipt text
	    pmink_utils::VariantParam* vp_dlvr_rcpt_text = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_dlvr_rcpt_text);
	    //if(vp_dlvr_rcpt_text == NULL) return 12;
	    // get timestamp
	    pmink_utils::VariantParam* vp_ts = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_timestamp);
	    if(vp_ts == NULL) return 12;
	    // get guid
	    pmink_utils::VariantParam* vp_guid = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_guid);
	    if(vp_guid == NULL) return 12;

	    // deliver sm
	    smpp::Deliver_sm* dsm = (smpp::Deliver_sm*)smpp_proc_pool.request_command(smpp::DELIVER_SM);
	    // sanity check
	    if(dsm == NULL) return 20;
	    // reset optional
	    dsm->params.clear();
	    // set smppp pointer
	    smpp::SmppPDU* smppp = &pmpld->smpp_pld.smpp_pdu;
	    // set pdu command
	    smppp->command = dsm;
	    smppp->command_id = smpp::DELIVER_SM;
	    // set status and seq num
	    smppp->command_status = smpp::ESME_ROK;

	    smppp->sequence_number = out_seq_num.add_fetch(1);
	    // set command params
	    char* srvc_type = (char*)"";
	    char* src_addr = (char*)*vp_src_addr;
	    char* dst_addr = (char*)*vp_dst_addr;
	    char* null_str = (char*)"";

	    // service type
	    dsm->service_type = srvc_type;
	    // source
	    dsm->source_addr_ton = (smpp::SmppTON)(int)*vp_src_addr_ton;
	    dsm->source_addr_npi = (smpp::SmppNPI)(int)*vp_src_addr_np;
	    dsm->source_addr = src_addr;
	    // dest
	    dsm->dest_addr_ton = (smpp::SmppTON)(int)*vp_dst_addr_ton;
	    dsm->dest_addr_npi = (smpp::SmppNPI)(int)*vp_dst_addr_np;
	    dsm->destination_addr = dst_addr;
	    // esm class
	    dsm->esm_class_mm = smpp::MM_DEFAULT_SMSC_MODE;
	    dsm->esm_class_mt = smpp::MT_SMSC_DELIVERY_RECEIPT;
	    dsm->esm_gsm_specific = smpp::GNS_NO_SPECIFIC_FEATURES;
	    // protocol id
	    dsm->protocol_id = 0;
	    // priority level
	    dsm->priority_flag = 0;
	    // scheduled delivery
	    dsm->schedule_delivery_time = null_str;
	    // validity period
	    dsm->validity_period = null_str;
	    // registered delivery
	    dsm->rd_smsc_dlvr_rcpt = smpp::SDR_NO_SMSC_DELIVERY;
	    dsm->rd_sme_orig_ack = smpp::SOA_NO_SME_ACK;
	    dsm->rd_intrmd_ntf = smpp::INT_NO;
	    // replace if present
	    dsm->replace_if_present_flag = 0;
	    // data coding
	    dsm->data_coding = smpp::DC_DEFAULT;
	    // predefined message
	    dsm->sm_default_msg_id = 0;
	    // delivery report id
	    size_t id_size = strlen((char*)*vp_dlvr_rcpt_id);
	    if(id_size + 1 > sizeof(smpp_dlvr_rcpt_proc.id)) id_size = sizeof(smpp_dlvr_rcpt_proc.id) - 1;
	    memcpy(smpp_dlvr_rcpt_proc.id, (char*)*vp_dlvr_rcpt_id, id_size);
	    smpp_dlvr_rcpt_proc.id[id_size] = 0;

	    // delivery report sub
	    smpp_dlvr_rcpt_proc.sub = (int)*vp_dlvr_rcpt_sub;
	    // delivery report dlvrd
	    smpp_dlvr_rcpt_proc.dlvrd = (int)*vp_dlvr_rcpt_dlvrd;
	    // delivery report submit date
	    smpp_dlvr_rcpt_proc.submit_date = (int)*vp_ts;
	    // delivery report done date
	    smpp_dlvr_rcpt_proc.done_date = (int)*vp_dlvr_rcpt_done_date;
	    // delivery report stat
	    smpp_dlvr_rcpt_proc.stat = (smpp::MessageStateType)(int)*vp_dlvr_rcpt_stat;
	    // delivery report err
	    smpp_dlvr_rcpt_proc.err = (int)*vp_dlvr_rcpt_err;
	    // delivery report text
	    if(vp_dlvr_rcpt_text != NULL){
		size_t rcpt_text_size = vp_dlvr_rcpt_text->get_size();
		if(rcpt_text_size > sizeof(smpp_dlvr_rcpt_proc.text)) rcpt_text_size = sizeof(smpp_dlvr_rcpt_proc.text);
		memcpy(smpp_dlvr_rcpt_proc.text, (unsigned char*)*vp_dlvr_rcpt_text, rcpt_text_size);
		smpp_dlvr_rcpt_proc.text_size = rcpt_text_size;
	    // no text, set size to 0
	    }else smpp_dlvr_rcpt_proc.text_size = 0;
	    unsigned char dlvr_rcpt_buff[1024];
	    int dlvr_enc_l = 0;
	    if(smpp_dlvr_rcpt_proc.encode(dlvr_rcpt_buff, sizeof(dlvr_rcpt_buff), &dlvr_enc_l) != 0) return 100;
	    // set pointer and length of delivery receipt
	    dsm->short_message = dlvr_rcpt_buff;
	    dsm->sm_length = dlvr_enc_l;

	    // encode
	    if(smppp->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size) != 0){
		// stats
		ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
		return 101;
	    }
	    // set header data
	    pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_header_data, pmpld->raw, pmpld->size);
	    // ok
	    return 0;
	}

	// generate UDH from sar params
	case asn1::SmsHubCommandId::_shci_smpp_generate_udh:
	{
	    // smpp header
	    pmink_utils::VariantParam* vp_smpp_hdr = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_header_data);
	    if(vp_smpp_hdr == NULL) return 9;

	    // get sequence
	    pmink_utils::VariantParam* vp_smpp_seq = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_sequence);
	    if(vp_smpp_seq == NULL) return 10;

	    // get pdu type
	    pmink_utils::VariantParam* vp_smpp_pdu = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_command_id);
	    if(vp_smpp_pdu == NULL) return 11;

	    // get sar msg ref num
	    pmink_utils::VariantParam* vp_smpp_sar_msgid = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
	    if(vp_smpp_sar_msgid == NULL) return 12;

	    // get sar seqnum
	    pmink_utils::VariantParam* vp_smpp_sar_seqnum = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_sar_segment_seqnum);
	    if(vp_smpp_sar_seqnum == NULL) return 13;

	    // get sar total segments
	    pmink_utils::VariantParam* vp_smpp_sar_segments = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_sar_total_segments);
	    if(vp_smpp_sar_segments == NULL) return 14;

	    // get sm length
	    pmink_utils::VariantParam* vp_smpp_sm_length = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_sm_length);
	    if(vp_smpp_sm_length == NULL) return 14;

	    // get sm
	    pmink_utils::VariantParam* vp_smpp_sm = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_sm);
	    if(vp_smpp_sm == NULL) return 14;

	    // set smppp pointer
	    smpp::SmppPDU* smppp = &pmpld->smpp_pld.smpp_pdu;

	    switch((int)*vp_smpp_pdu){
		// submit_sm
		case smpp::SUBMIT_SM:
		{
		    if(smpp::decode((unsigned char*)*vp_smpp_hdr, vp_smpp_hdr->get_size(), smppp, &smpp_proc_pool) != 0) return 1;
		    // set smppp pointer
		    smpp::SmppPDU* smppp = &pmpld->smpp_pld.smpp_pdu;
		    // generate sequence
		    smppp->sequence_number = out_seq_num.add_fetch(1);
		    // command pointer
		    smpp::Submit_sm* dsm = (smpp::Submit_sm*)smppp->command;
		    // get sar params
		    smpp::Sar_msg_ref_num* sar_msg_ref = (smpp::Sar_msg_ref_num*)dsm->get_parameter(smpp::SAR_MSG_REF_NUM);
		    smpp::Sar_segment_seqnum* sar_seqnum = (smpp::Sar_segment_seqnum*)dsm->get_parameter(smpp::SAR_SEGMENT_SEQNUM);
		    smpp::Sar_total_segments* sar_segments = (smpp::Sar_total_segments*)dsm->get_parameter(smpp::SAR_TOTAL_SEGMENTS);

		    // check
		    if(sar_msg_ref == NULL || sar_seqnum == NULL || sar_segments == NULL) return 1;
		    // check length
		    if((int)*vp_smpp_sm_length == 0) return 1;
		    // remove from list
		    dsm->remove_parameter(smpp::SAR_MSG_REF_NUM);
		    dsm->remove_parameter(smpp::SAR_SEGMENT_SEQNUM);
		    dsm->remove_parameter(smpp::SAR_TOTAL_SEGMENTS);
		    dsm->remove_parameter(smpp::MESSAGE_PAYLOAD);
		    // set udhi
		    dsm->esm_gsm_specific = smpp::GNS_UDHI_INDICATOR;
		    // generate udh
		    // tp-ud
		    unsigned char text_buff[256];
		    unsigned char pack_buff[256];
		    unsigned char* text_buff_p = text_buff;
		    int text_enc_l = sizeof(text_buff);
		    unsigned char* sms_text_p = (unsigned char*)*vp_smpp_sm;
		    size_t sms_text_l = vp_smpp_sm->get_size();
		    int calc_size = 0;


		    // padding = ((UDHL + 1 ) * 8 ) % 7
		    //smstpdu::GsmAlphabet::encode((char*)(unsigned char*)*vp_smpp_sm, (int)*vp_smpp_sm_length, 7 - ((6 * 8) % 7), sizeof(text_buff), text_buff, &text_enc_l);




		    // encode
		    size_t iconv_ret = libiconv(iconv_utf8_to_gsm7,
						(char**)&sms_text_p,
						&sms_text_l,
						(char**)&text_buff_p,
						(size_t*)&text_enc_l);

		    if(iconv_ret == (size_t)-1){
			// flush
			libiconv(iconv_utf8_to_gsm7, NULL, NULL, (char**)&text_buff_p, (size_t*)&text_enc_l);
			return 1;
		    }
		    // calc size
		    calc_size = sizeof(text_buff) - text_enc_l;
		    // flush
		    libiconv(iconv_utf8_to_gsm7, NULL, NULL, (char**)&text_buff_p, (size_t*)&text_enc_l);
		    // size
		    text_enc_l = calc_size;
		    // pack
		    // padding = ((UDHL + 1 ) * 8 ) % 7
		    if(smstpdu::GsmAlphabet::pack((char*)text_buff, text_enc_l, 7 - ((6 * 8) % 7), sizeof(pack_buff), pack_buff, &text_enc_l) != 0) return 1;



		    // set tp-ud
		    //tp_ud.value = (unsigned char*)*vp_smpp_sm;
		    //tp_ud.length = (int)*vp_smpp_sm_length;
		    tp_ud.value = pack_buff;
		    tp_ud.length = text_enc_l;
		    // udh
		    tp_ud.udh.ie_lst.clear();
		    smstpdu::TP_UDHIEConc8BitRef* ie = (smstpdu::TP_UDHIEConc8BitRef*)tp_ud.udh.ie_pool.request_ie(smstpdu::TPUDHIET_CONCATENATED_8BIT_REF);
		    tp_ud.udh.ie_lst.push_back(ie);
		    ie->msg_id = sar_msg_ref->value;
		    ie->msg_part = sar_seqnum->value;
		    ie->msg_parts = sar_segments->value;

		    // encode tp-ud
		    int udh_enc_l = 0;
		    unsigned char tp_ud_buff[1024];
		    tp_ud.udh.encode(tp_ud_buff, sizeof(tp_ud_buff), 0, &udh_enc_l);
		    // overflow check
		    if(udh_enc_l + tp_ud.length > sizeof(tp_ud_buff)) return 1;
		    // add encoded text
		    memcpy(&tp_ud_buff[udh_enc_l], tp_ud.value, tp_ud.length);
		    // add to short_message param
		    dsm->short_message = tp_ud_buff;
		    // set sm length
		    dsm->sm_length = udh_enc_l + tp_ud.length;
		    // encode
		    if(smppp->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size) != 0){
			// stats
			ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
			return 101;
		    }
		    // set header data
		    pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_header_data, pmpld->raw, pmpld->size);
		    // ok
		    return 0;
		}

		// deliver_sm
		case smpp::DELIVER_SM:
		{
		    break;
		}

		// unknown
		default:
		    return 100;
	    }



	    // encode
	    if(smppp->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size) != 0){
		// stats
		ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
		return 101;
	    }
	    // set header data
	    pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_header_data, pmpld->raw, pmpld->size);
	    // ok
	    return 0;
	}

	// unsupported
	default: return 0;

    }
    return 1;
}


template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::create_client(int _client_socket, ConnectionBase** new_conn){
    // - use SERVER side state machine
    // - use CLIENT side threading model

    // connection label = master label + ts sec + ts nsec
    std::ostringstream tmp_str;
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    tmp_str << ConnectionBase::get_label() << "_" << this << "_" << ts.tv_sec << "_" << ts.tv_nsec;
    // create new connection
    SMPPConnection* conn = new SMPPConnection(CD_SERVER, SCT_ONE_MANY, tmp_str.str().c_str(), Connection<PLD_TYPE>::max_mtu, Connection<PLD_TYPE>::pool_size);
    // use this params
    conn->params = ConnectionBase::params;
    // set max conn count to 1
    conn->params.set_int(CPT_MAX_CONN_COUNT, 1);
    // set socket
    conn->client_socket = _client_socket;
    // set master connection
    conn->master_conn = this;
    ConnectionBase::master_ref.fetch_add(1);
    // handlers
    conn->get_e_handler()->set_handler(CET_CONNECTION_UP, ConnectionBase::e_handler.get_handler(CET_CONNECTION_UP));
    conn->get_e_handler()->set_handler(CET_CONNECTION_DOWN, ConnectionBase::e_handler.get_handler(CET_CONNECTION_DOWN));
    conn->set_logic(new DefaultConnectionLogic<PLD_TYPE>(conn));

    // add stats handlers
    pmink_utils::VariantParam* vp = ConnectionBase::params.get_param(CPT_R14P_STATS);
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)(void*)*vp;
    std::string stats_prefix("SMPP::");
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_RX_MSU")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_RX_MSU_COUNT));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_TX_MSU")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_TX_MSU_COUNT));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_MTU_LIMIT")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_MTU_LIMIT));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_DISCARDED_MSU")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_DISCARDED_MSU));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_PLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_PLD_POOL_EMPTY));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_MALFORMED_MSU")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_MALFORMED_MSU));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_RX_ERROR")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_RX_ERROR));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_TX_ERROR")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_TX_ERROR));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_PMPLD_POOL_EMPTY")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_PMPLD_POOL_EMPTY));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_SMSG_SEND_ERROR")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_SMSG_SEND_ERROR));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_SMSG_MSG_POOL_EMPTY")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_SMSG_MSG_POOL_EMPTY));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_SMSG_PARAM_POOL_EMPTY")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_SMSG_PARAM_POOL_EMPTY));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_SMPP_ACTIVE")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_SMPP_ACTIVE));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_TCP_ACTIVE")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_TCP_ACTIVE));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_SMSG_SENT_COUNT")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_SMSG_SENT_COUNT));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_NO_VALID_ROUTES")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_NO_VALID_ROUTES));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_RX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_RX_HBEAT_COUNT));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_TX_HBEAT_COUNT")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_TX_HBEAT_COUNT));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_CMD_PROCESS_ERROR")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_CMD_PROCESS_ERROR));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_ENCODE_ERROR")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_ENCODE_ERROR));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + conn->label + "_RECONNECT")), new sgn::SgnStatsHandler(&conn->stats, sgn::CST_RECONNECT));

    // start
    conn->start();
    // set result
    if(new_conn != NULL) *new_conn = conn;
    // ok
    return 0;
}


template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::timer_event(){
    // check for tcp connection
    if(Connection<PLD_TYPE>::client_socket == -1) return 1;
    // check if BIND needs to be sent
    if(!smpp_active.get() && Connection<PLD_TYPE>::direction == CD_CLIENT){
	// init reconnect
	if(time(NULL) - bind_ts >= bind_timeout) smpp_connect();
    }

    return 0;

}


template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::data_rx_process(unsigned char* buff, int size, int data_type, void* extra){
    // check size (smpp recv can return 0 size and no error)
    if(size <= 0) return 10;

    // mtu limit check
    if(size > Connection<PLD_TYPE>::max_mtu){
	// stats
	ConnectionBase::stats.inc(CST_MTU_LIMIT, 1);
	return 2;
    }


    // connect procedure
    if(!smpp_active.get()){
	// get payload
	smpp::SmppPDU* smppp = NULL;
	PLD_TYPE* pld = NULL;
	if(Connection<PLD_TYPE>::pld_pool.pop(&pld) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_PLD_POOL_EMPTY, 1);
	    return 2;
	}
	// set smppp pointer
	smppp = &pld->smpp_pdu;

	// decode
	if(smpp::decode(buff, size, smppp, &smpp_rx_pool) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

	    // send GENERIC NACK
	    smpp::SmppCommand* tmpcmd = smpp_rx_pool.request_command(smpp::GENERIC_NACK);
	    if(tmpcmd != NULL){
		// set params
		smppp->command_id = smpp::GENERIC_NACK;
		smppp->command_status = smpp::ESME_RINVCMDLEN;
		smppp->command = tmpcmd;
		smppp->sequence_number = 0;
		smppp->command->clear_params();
		// encode
		if(smppp->encode(pld->raw,
			      Connection<PLD_TYPE>::max_mtu,
			      0,
			      &pld->size) != 0) {

		    // stats
		    ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
		    // free pld
		    pld->action = PAT_FREE;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		    return 101;
		}


		// push to out
		pld->action = PAT_SEND;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		return 0;


	    }else{
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		return 4;

	    }

	}

	// check connection phase (CLIENT phases start from 0, SERVER from 10)
	switch(conn_phase){
	    // **************** CLIENT MODE *******************

	    // BIND sent, waiting for BIND_RESP
	    case 0:
	    {
		// get bind method type
		smpp::SmppCommandId bind_method = (smpp::SmppCommandId)(int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_METHOD);
		// get bind method response type
		bind_method = (smpp::SmppCommandId)(bind_method | 0x80000000);
		// check if valid BIND response received
		// and status is OK
		if(smppp->command_id == bind_method && (smppp->command_status == smpp::ESME_ROK || smppp->command_status == smpp::ESME_RALYBND)){
		    // inc conn phase
		    ++conn_phase;
		    bind_ts = time(NULL);
		    // set smpp part as active
		    smpp_active.comp_swap(false, true);
		    // run event handler
		    pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;
		    e_args.add_arg(CEAT_CONNECTION, this);
		    ConnectionBase::e_handler.run_handler(CET_CONNECTION_UP, e_args);
		    // stats
		    ConnectionBase::stats.set(CST_SMPP_ACTIVE, 1);

		}else bind_ts = time(NULL);

		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		break;
	    }

	    // **************** SERVER MODE *******************
	    case 10:
	    {
		// get bind meethod
		smpp::SmppCommandId bind_method = (smpp::SmppCommandId)(int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_METHOD);
		// check if propper BIND received
		if(smppp->command_id == bind_method){

		    // allocate response command
		    smpp::SmppCommand* tmpcmd = smpp_rx_pool.request_command((smpp::SmppCommandId)(smppp->command_id | 0x80000000));
		    // error check
		    if(tmpcmd == NULL){
			// free pld
			pld->action = PAT_FREE;
			Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
			return 10;

		    }
		    // get master smpp connection
		    SMPPConnection* master_smpp = (SMPPConnection*)ConnectionBase::master_conn;
		    smpp::SmppCommandStatus tmp_status = smpp::ESME_RBINDFAIL;

		    // check bind type
		    switch(smppp->command_id){
			case smpp::BIND_TRANSMITTER:
			{
			    smpp::Bind_transmitter* bind = (smpp::Bind_transmitter*)smppp->command;
			    smpp::Bind_transmitter_resp* bresp = (smpp::Bind_transmitter_resp*)tmpcmd;
			    // set params
			    bresp->system_id = bind->system_id;
			    smppp->command_id = smpp::BIND_TRANSMITTER_RESP;
			    smppp->command = bresp;
			    smppp->command->clear_params();
			    // auth user
			    if(master_smpp->bind_auth(bind->system_id, bind->password)) smppp->command_status = smpp::ESME_ROK;
			    else smppp->command_status = smpp::ESME_RBINDFAIL;
			    // set user
			    strncpy(smpp_user.system_id, bind->system_id, sizeof(smpp_user.system_id));
			    break;
			}
			case smpp::BIND_RECEIVER:
			{
			    smpp::Bind_receiver* bind = (smpp::Bind_receiver*)smppp->command;
			    smpp::Bind_receiver_resp* bresp = (smpp::Bind_receiver_resp*)tmpcmd;
			    // set params
			    bresp->system_id = bind->system_id;
			    smppp->command_id = smpp::BIND_RECEIVER_RESP;
			    smppp->command_status = smpp::ESME_ROK;
			    smppp->command = bresp;
			    smppp->command->clear_params();
			    // auth user
			    if(master_smpp->bind_auth(bind->system_id, bind->password)) smppp->command_status = smpp::ESME_ROK;
			    else smppp->command_status = smpp::ESME_RBINDFAIL;
			    // set user
			    strncpy(smpp_user.system_id, bind->system_id, sizeof(smpp_user.system_id));
			    break;
			}

			case smpp::BIND_TRANSCEIVER:
			{
			    smpp::Bind_transceiver* bind = (smpp::Bind_transceiver*)smppp->command;
			    smpp::Bind_transceiver_resp* bresp = (smpp::Bind_transceiver_resp*)tmpcmd;
			    // set params
			    bresp->system_id = bind->system_id;
			    smppp->command_id = smpp::BIND_TRANSCEIVER_RESP;
			    smppp->command_status = smpp::ESME_ROK;
			    smppp->command = bresp;
			    smppp->command->clear_params();
			    // auth user
			    if(master_smpp->bind_auth(bind->system_id, bind->password)) smppp->command_status = smpp::ESME_ROK;
			    else smppp->command_status = smpp::ESME_RBINDFAIL;
			    // set user
			    strncpy(smpp_user.system_id, bind->system_id, sizeof(smpp_user.system_id));
			    break;
			}
			default: break;


		    }

		    // encode
		    if(smppp->encode(pld->raw,
				  Connection<PLD_TYPE>::max_mtu,
				  0,
				  &pld->size) != 0){

			// stats
			ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
			// free pld
			pld->action = PAT_FREE;
			Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
			return 101;
		    }
		    // get last encoded status
		    tmp_status = smppp->command_status;

		    // push to out
		    pld->action = PAT_SEND;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		    // check if auth was successful
		    if(tmp_status == smpp::ESME_ROK){
			// inc conn phase
			++conn_phase;
			// set smpp part as active
			smpp_active.comp_swap(false, true);
			// run event handler
			pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;
			e_args.add_arg(CEAT_CONNECTION, this);
			ConnectionBase::e_handler.run_handler(CET_CONNECTION_UP, e_args);
			// stats
			ConnectionBase::stats.set(CST_SMPP_ACTIVE, 1);
			if(ConnectionBase::master_conn != NULL) ConnectionBase::master_conn->stats.inc(CST_SMPP_ACTIVE, 1);

		    }


		}else{
		    // check if bind received
		    if( smppp->command_id == smpp::BIND_TRANSMITTER ||
		        smppp->command_id == smpp::BIND_RECEIVER ||
		        smppp->command_id == smpp::BIND_TRANSCEIVER){
			// check for command pointer
			if(smppp->command != NULL){

			    // allocate response command
			    smpp::SmppCommand* tmpcmd = smpp_rx_pool.request_command((smpp::SmppCommandId)(smppp->command_id | 0x80000000));
			    // error check
			    if(tmpcmd == NULL){
				// free pld
				pld->action = PAT_FREE;
				Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
				return 10;

			    }
			    // check bind type
			    switch(smppp->command_id){
				case smpp::BIND_TRANSMITTER:
				{
				    smpp::Bind_transmitter* bind = (smpp::Bind_transmitter*)smppp->command;
				    smpp::Bind_transmitter_resp* bresp = (smpp::Bind_transmitter_resp*)tmpcmd;
				    // set params
				    bresp->system_id = bind->system_id;
				    smppp->command_id = smpp::BIND_TRANSMITTER_RESP;
				    smppp->command_status = smpp::ESME_RINVCMDID;
				    smppp->command = bresp;
				    smppp->command->clear_params();

				    break;
				}
				case smpp::BIND_RECEIVER:
				{
				    smpp::Bind_receiver* bind = (smpp::Bind_receiver*)smppp->command;
				    smpp::Bind_receiver_resp* bresp = (smpp::Bind_receiver_resp*)tmpcmd;
				    // set params
				    bresp->system_id = bind->system_id;
				    smppp->command_id = smpp::BIND_RECEIVER_RESP;
				    smppp->command_status = smpp::ESME_RINVCMDID;
				    smppp->command = bresp;
				    smppp->command->clear_params();
				    break;
				}

				case smpp::BIND_TRANSCEIVER:
				{
				    smpp::Bind_transceiver* bind = (smpp::Bind_transceiver*)smppp->command;
				    smpp::Bind_transceiver_resp* bresp = (smpp::Bind_transceiver_resp*)tmpcmd;
				    // set params
				    bresp->system_id = bind->system_id;
				    smppp->command_id = smpp::BIND_TRANSCEIVER_RESP;
				    smppp->command_status = smpp::ESME_RINVCMDID;
				    smppp->command = bresp;
				    smppp->command->clear_params();
				    break;
				}
				default: break;


			    }

			    // encode
			    if(smppp->encode(pld->raw,
					  Connection<PLD_TYPE>::max_mtu,
					  0,
					  &pld->size) != 0){

				// stats
				ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
				// free pld
				pld->action = PAT_FREE;
				Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
				return 101;

			    }


			    // push to out
			    pld->action = PAT_SEND;
			    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);


			// command error
			}else{
			    // free pld
			    pld->action = PAT_FREE;
			    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

			}

		    // unsupported command id at this stage
		    }else{
			// free pld
			pld->action = PAT_FREE;
			Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		    }


		}

		break;
	    }

	    // unknown state, return to pool
	    default:
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		break;

	}

    // data
    }else{
	// get payload
	smpp::SmppPDU* smppp = NULL;
	PLD_TYPE* pld = NULL;
	if(Connection<PLD_TYPE>::pld_pool.pop(&pld) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_PLD_POOL_EMPTY, 1);
	    return 2;
	}
	// set smppp pointer
	smppp = &pld->smpp_pdu;

	// copy raw data to payload
	memcpy(pld->raw, buff, size);
	pld->size = size;

	// decode
	if(smpp::decode(pld->raw, pld->size, smppp, &smpp_rx_pool) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

	    // send GENERIC NACK
	    smpp::SmppCommand* tmpcmd = smpp_rx_pool.request_command(smpp::GENERIC_NACK);
	    if(tmpcmd != NULL){
		// set params
		smppp->command_id = smpp::GENERIC_NACK;
		smppp->command_status = smpp::ESME_RINVCMDLEN;
		smppp->command = tmpcmd;
		smppp->sequence_number = 0;
		smppp->command->clear_params();
		// encode
		if(smppp->encode(pld->raw,
			      Connection<PLD_TYPE>::max_mtu,
			      0,
			      &pld->size) != 0){

		    // stats
		    ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
		    // free pld
		    pld->action = PAT_FREE;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		    return 101;

		}


		// push to out
		pld->action = PAT_SEND;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		return 0;


	    }else{
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		return 4;

	    }

	}

	// check type
	switch(smppp->command_id){
	    // generic_nack
	    case smpp::GENERIC_NACK:
	    {
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		break;
	    }

	    // submit_sm
	    case smpp::SUBMIT_SM:
	    {
		// set action
		pld->action = PAT_PROCESS;
		// push to proc
		Connection<PLD_TYPE>::rx_proc_q.push(1, pld);
		break;
	    }

	    // submit_sm_resp
	    case smpp::SUBMIT_SM_RESP:
	    {
		// set action
		pld->action = PAT_PROCESS;
		// push to proc
		Connection<PLD_TYPE>::rx_proc_q.push(1, pld);
		break;
	    }


	    // deliver_sm
	    case smpp::DELIVER_SM:
	    {
		// set action
		pld->action = PAT_PROCESS;
		// push to proc
		Connection<PLD_TYPE>::rx_proc_q.push(1, pld);
		break;
	    }

	    // deliver_sm_resp
	    case smpp::DELIVER_SM_RESP:
	    {
		// set action
		pld->action = PAT_PROCESS;
		// push to proc
		Connection<PLD_TYPE>::rx_proc_q.push(1, pld);

		break;
	    }

	    // data_sm
	    case smpp::DATA_SM:
	    {
		// set action
		pld->action = PAT_PROCESS;
		// push to proc
		Connection<PLD_TYPE>::rx_proc_q.push(1, pld);
		break;
	    }

	    // data_sm_resp
	    case smpp::DATA_SM_RESP:
	    {
		// set action
		pld->action = PAT_PROCESS;
		// push to proc
		Connection<PLD_TYPE>::rx_proc_q.push(1, pld);
		break;
	    }

	    // unbind
	    case smpp::UNBIND:
		// close connection
		Connection<PLD_TYPE>::reconnect_queued.set(true);
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		break;

	    // enquire_link
	    case smpp::ENQUIRE_LINK:
	    {
		// send ENQUIRE_LINK_RESP
		smpp::SmppCommand* tmpcmd = smpp_rx_pool.request_command(smpp::ENQUIRE_LINK_RESP);
		if(tmpcmd != NULL){
		    // set params
		    smppp->command_id = smpp::ENQUIRE_LINK_RESP;
		    smppp->command_status = smpp::ESME_ROK;
		    smppp->command = tmpcmd;
		    smppp->command->clear_params();
		    // encode
		    if(smppp->encode(pld->raw,
				  Connection<PLD_TYPE>::max_mtu,
				  0,
				  &pld->size) != 0){

			// stats
			ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
			// free pld
			pld->action = PAT_FREE;
			Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
			return 101;

		    }


		    // push to out
		    pld->action = PAT_SEND;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		    return 0;


		}else{
		    // free pld
		    pld->action = PAT_FREE;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		    return 4;

		}

		break;
	    }

	    // enquire_link_resp
	    case smpp::ENQUIRE_LINK_RESP:
	    {
		// dec counter
		enql_diff.fetch_sub(1);

		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		break;
	    }


	    // alert_notification
	    case smpp::ALERT_NOTIFICATION:
		// do nothing
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		break;

	    // unsupported/unknown command
	    default:
	    {
		// send GENERIC NACK
		smpp::SmppCommand* tmpcmd = smpp_rx_pool.request_command(smpp::GENERIC_NACK);
		if(tmpcmd != NULL){
		    // set params
		    smppp->command_id = smpp::GENERIC_NACK;
		    smppp->command_status = smpp::ESME_RINVCMDID;
		    smppp->command = tmpcmd;
		    smppp->command->clear_params();
		    // encode
		    if(smppp->encode(pld->raw,
				  Connection<PLD_TYPE>::max_mtu,
				  0,
				  &pld->size) != 0){

			// stats
			ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
			// free pld
			pld->action = PAT_FREE;
			Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
			return 101;
		    }


		    // push to out
		    pld->action = PAT_SEND;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		    return 0;


		}else{
		    // free pld
		    pld->action = PAT_FREE;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		    return 4;

		}

		break;
	    }
	}


    }

    return 0;
}

template<typename PLD_TYPE>
void sgn::_SMPPConnection<PLD_TYPE>::pmpld_dr(PminkPayload* pmpld){
    // debug
    #ifdef PMDEBUG
    pmpld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== SENDING TO DR =====" << std::endl;
	std::cout << pmpld->params;
	std::cout << "=====================" << std::endl;
    )

    // *********************************************
    // ************ push to dr daemon *************
    // *********************************************

    // get new router if connection broken
    if(dr_r14pc == NULL || !dr_r14pc->is_registered()) dr_r14pc = r14ps->get_registered_client("routingd");
    // get first routing daemon
    r14p::R14PClient* r14pc = dr_r14pc;
    // null check
    if(r14pc == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // allocate new service message
    r14p::ServiceMessage* msg = r14psmm->new_smsg();
    // msg sanity check
    if(msg == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_MSG_POOL_EMPTY, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // service id
    msg->set_service_id(asn1::ServiceId::_sid_sms_data_retention);
    // copy params
    msg->vpmap = pmpld->params;
    // free pmpld
    Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(msg) != 0){
	// free
	r14psmm->free_smsg(msg);
	ConnectionBase::stats.inc(CST_SMSG_PARAM_POOL_EMPTY, 1);
	return;

    }
    // inc refc counter for conn
    this->refc_inc();
    // set service message params
    msg->params.set_param(2, this);
    // send service message
    int res = r14psmm->send(msg, r14pc, "drd", NULL, true, &smsg_async_done);
    if(res != 0){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	r14psmm->free_smsg(msg);
	// dec refc counter for conn
	this->refc_dec();
    }


    // *********************************************
    // *********************************************

}

template<typename PLD_TYPE>
void sgn::_SMPPConnection<PLD_TYPE>::pmpld_stp(PminkPayload* pmpld){
    // debug
    #ifdef PMDEBUG
    pmpld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== SENDING TO STP =====" << std::endl;
	std::cout << pmpld->params;
	std::cout << "=====================" << std::endl;
    )

    // *********************************************
    // ************ push to stp daemon *************
    // *********************************************
    // get new router if connection broken
    if(stp_r14pc == NULL || !stp_r14pc->is_registered()) stp_r14pc = r14ps->get_registered_client("routingd");
    // get first routing daemon
    r14p::R14PClient* r14pc = stp_r14pc;
    // null check
    if(r14pc == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // allocate new service message
    r14p::ServiceMessage* msg = r14psmm->new_smsg();
    // msg sanity check
    if(msg == NULL){
	// statsConnectionBase::stats
	ConnectionBase::stats.inc(CST_SMSG_MSG_POOL_EMPTY, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // service id
    msg->set_service_id(asn1::ServiceId::_sid_stp_routing);
    // copy params
    msg->vpmap = pmpld->params;
    // free pmpld
    Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(msg) != 0){
	// free
	r14psmm->free_smsg(msg);
	ConnectionBase::stats.inc(CST_SMSG_PARAM_POOL_EMPTY, 1);
	return;

    }
    // inc refc counter for conn
    this->refc_inc();
    // set service message params
    msg->params.set_param(2, this);
    // send service message
    int res = r14psmm->send(msg, r14pc, "stpd", NULL, true, &smsg_async_done);
    if(res != 0){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	r14psmm->free_smsg(msg);
	// dec refc counter for conn
	this->refc_dec();
    }


    // *********************************************
    // *********************************************

}


template<typename PLD_TYPE>
void sgn::_SMPPConnection<PLD_TYPE>::process_smpp(smpp::SmppPDU* pdu, PminkPayload* pmpld){

    // get peer info
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getpeername(Connection<PLD_TYPE>::client_socket, (sockaddr*)&addr, &len);
    // peer ip
    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_peer_ip, inet_ntoa(addr.sin_addr));
    // peer port
    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_peer_port, be16toh(addr.sin_port));
    // username
    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_system_id, smpp_user.system_id);


    // get as pointer
    SMPP_ApplicationServer* tmp_as = NULL;
    if(ConnectionBase::master_conn == NULL) tmp_as = as.get();
    else tmp_as = ((SMPPConnection*)ConnectionBase::master_conn)->as.get();

    // as label
    if(tmp_as != NULL) pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_as_label, tmp_as->get_label());
    // tmp label (defaults to connection label/ASP label)
    const char* tmp_label = ConnectionBase::label.c_str();
    // check if connection is ONE_TO_MANY server child connection
    if(ConnectionBase::master_conn != NULL){
        // in case of child connection, user master connection's label/ASP label
	tmp_label = ConnectionBase::master_conn->get_label();
	// connection id is child connection's label
	pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_connection_id, ConnectionBase::label.c_str());
    }
    // asp label
    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_asp_label, tmp_label);
    // operation
    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_command_id, pdu->command_id);
    // sequence
    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sequence, pdu->sequence_number);

    // check operation id
    switch(pdu->command_id){
	// submit_sm
	case smpp::SUBMIT_SM:
	{
	    // submit_sm pointer
	    smpp::Submit_sm* tmppdu = (smpp::Submit_sm*)pdu->command;
	    // service type
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_service_type, tmppdu->service_type);
	    // originator ton
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_originator_ton, tmppdu->source_addr_ton);
	    // originator np
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_originator_np, tmppdu->source_addr_npi);
	    // originator address
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_originator_address, tmppdu->source_addr);
	    // recipient ton
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_recipient_ton, tmppdu->dest_addr_ton);
	    // recipient np
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_recipient_np, tmppdu->dest_addr_npi);
	    // recipient address
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_recipient_address, tmppdu->destination_addr);
	    // esm mm
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_esm_message_mode, tmppdu->esm_class_mm);
	    // esm mt
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_esm_message_type, tmppdu->esm_class_mt);
	    // esm gsm features
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_esm_gsm_features, tmppdu->esm_gsm_specific);
	    // protocol id
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_protocol_id, tmppdu->protocol_id);
	    // priority flag
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_priority_flag, tmppdu->priority_flag);
	    // scheduled delivery time
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_delivery_time, smpp::decode_time_format(tmppdu->schedule_delivery_time));
	    // validity period
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_validity_period, smpp::decode_time_format(tmppdu->validity_period));
	    // registered delivery receipt
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_rd_smsc_receipt, tmppdu->rd_smsc_dlvr_rcpt);
	    // registered delivery sme ack
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_rd_sme_ack, tmppdu->rd_sme_orig_ack);
	    // registered delivery intermediate notification
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_rd_intermediate_notification, tmppdu->rd_intrmd_ntf);
	    // replace if present
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_replace_if_present_flag, tmppdu->replace_if_present_flag);
	    // data coding
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_data_coding, tmppdu->data_coding);
	    // predefined message
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sm_defaut_msg_id, tmppdu->sm_default_msg_id);
	    // message length
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sm_length, tmppdu->sm_length);
	    if(tmppdu->sm_length > 0){
		// message
		pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_sm, tmppdu->short_message, tmppdu->sm_length);

	    }else{
		// check message_payload
		smpp::SmppParam* tmpparam = tmppdu->get_parameter(smpp::MESSAGE_PAYLOAD);
		if(tmpparam != NULL){
		    pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_sm, tmpparam->param_data, tmpparam->length);
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sm_length, tmpparam->length);
		}


	    }
	    // check concatenated sar params
	    // sar msg ref num
	    smpp::SmppParam* tmpparam = tmppdu->get_parameter(smpp::SAR_MSG_REF_NUM);
	    if(tmpparam != NULL) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sar_msg_ref_num, ((smpp::Sar_msg_ref_num*)tmpparam)->value);
	    // sar total segments
	    tmpparam = tmppdu->get_parameter(smpp::SAR_TOTAL_SEGMENTS);
	    if(tmpparam != NULL) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sar_total_segments, ((smpp::Sar_total_segments*)tmpparam)->value);
	    // sar segment seqnum
	    tmpparam = tmppdu->get_parameter(smpp::SAR_SEGMENT_SEQNUM);
	    if(tmpparam != NULL) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sar_segment_seqnum, ((smpp::Sar_segment_seqnum*)tmpparam)->value);
	    break;
	}

	// submit_sm_resp
	case smpp::SUBMIT_SM_RESP:
	{
	    // submit_sm_resp pointer
	    smpp::Submit_sm_resp* tmppdu = (smpp::Submit_sm_resp*)pdu->command;
	    // messsage id
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_message_id, tmppdu->message_id);
	    // command status
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_command_status, pdu->command_status);
	    break;
	}


	// deliver_sm
	case smpp::DELIVER_SM:
	{
	    // deliver_sm pointer
	    smpp::Deliver_sm* tmppdu = (smpp::Deliver_sm*)pdu->command;
	    // service type
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_service_type, tmppdu->service_type);
	    // originator ton
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_originator_ton, tmppdu->source_addr_ton);
	    // originator np
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_originator_np, tmppdu->source_addr_npi);
	    // originator address
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_originator_address, tmppdu->source_addr);
	    // recipient ton
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_recipient_ton, tmppdu->dest_addr_ton);
	    // recipient np
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_recipient_np, tmppdu->dest_addr_npi);
	    // recipient address
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_recipient_address, tmppdu->destination_addr);
	    // esm mm
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_esm_message_mode, tmppdu->esm_class_mm);
	    // esm mt
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_esm_message_type, tmppdu->esm_class_mt);
	    // esm gsm features
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_esm_gsm_features, tmppdu->esm_gsm_specific);
	    // protocol id
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_protocol_id, tmppdu->protocol_id);
	    // priority flag
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_priority_flag, tmppdu->priority_flag);
	    // scheduled delivery time
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_delivery_time, smpp::decode_time_format(tmppdu->schedule_delivery_time));
	    // validity period
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_validity_period, smpp::decode_time_format(tmppdu->validity_period));
	    // registered delivery receipt
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_rd_smsc_receipt, tmppdu->rd_smsc_dlvr_rcpt);
	    // registered delivery sme ack
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_rd_sme_ack, tmppdu->rd_sme_orig_ack);
	    // registered delivery intermediate notification
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_rd_intermediate_notification, tmppdu->rd_intrmd_ntf);
	    // replace if present
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_replace_if_present_flag, tmppdu->replace_if_present_flag);
	    // data coding
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_data_coding, tmppdu->data_coding);
	    // predefined message
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sm_defaut_msg_id, tmppdu->sm_default_msg_id);
	    // message length
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sm_length, tmppdu->sm_length);
	    if(tmppdu->sm_length > 0){
		// message
		pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_sm, tmppdu->short_message, tmppdu->sm_length);

	    }else{
		// check message_payload
		smpp::SmppParam* tmpparam = tmppdu->get_parameter(smpp::MESSAGE_PAYLOAD);
		if(tmpparam != NULL){
		    pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_sm, tmpparam->param_data, tmpparam->length);
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sm_length, tmpparam->length);
		}

	    }

	    // check concatenated sar params
	    // sar msg ref num
	    smpp::SmppParam* tmpparam = tmppdu->get_parameter(smpp::SAR_MSG_REF_NUM);
	    if(tmpparam != NULL) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sar_msg_ref_num, ((smpp::Sar_msg_ref_num*)tmpparam)->value);
	    // sar total segments
	    tmpparam = tmppdu->get_parameter(smpp::SAR_TOTAL_SEGMENTS);
	    if(tmpparam != NULL) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sar_total_segments, ((smpp::Sar_total_segments*)tmpparam)->value);
	    // sar segment seqnum
	    tmpparam = tmppdu->get_parameter(smpp::SAR_SEGMENT_SEQNUM);
	    if(tmpparam != NULL) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sar_segment_seqnum, ((smpp::Sar_segment_seqnum*)tmpparam)->value);

	    // check for delivery receipt
	    if(tmppdu->esm_class_mt == smpp::MT_SMSC_DELIVERY_RECEIPT){
		// decode standard (smpp 3.4 appendix B)
		if(smpp_dlvr_rcpt_proc.decode(tmppdu->short_message, tmppdu->sm_length) == 0){
		    // format
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_format, 0);
		    // delivery receipt id
		    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_dlvr_rcpt_id, smpp_dlvr_rcpt_proc.id);
		    // delivery receipt sub
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_sub, smpp_dlvr_rcpt_proc.sub);
		    // delivery receipt dlvrd
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_dlvrd, smpp_dlvr_rcpt_proc.dlvrd);
		    // delivery receipt submit date
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_submit_date, smpp_dlvr_rcpt_proc.submit_date);
		    // delivery receipt done date
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_done_date, smpp_dlvr_rcpt_proc.done_date);
		    // delivery receipt stat
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_stat, smpp_dlvr_rcpt_proc.stat);
		    // delivery receipt err
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_err, smpp_dlvr_rcpt_proc.err);
		    // delivery receipt text
		    if(smpp_dlvr_rcpt_proc.text_size > 0)
			pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_dlvr_rcpt_text, smpp_dlvr_rcpt_proc.text, smpp_dlvr_rcpt_proc.text_size);

		// try variant 1 (TMOB slovakia)
		}else if(smpp_dlvr_rcpt_proc.decode_var_1(tmppdu->short_message, tmppdu->sm_length) == 0){
		    // format
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_format, 1);
		    // delivery receipt id
		    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_dlvr_rcpt_id, smpp_dlvr_rcpt_proc.id);
		    // delivery receipt sub
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_sub, smpp_dlvr_rcpt_proc.sub);
		    // delivery receipt dlvrd
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_dlvrd, smpp_dlvr_rcpt_proc.dlvrd);
		    // delivery receipt submit date
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_submit_date, smpp_dlvr_rcpt_proc.submit_date);
		    // delivery receipt done date
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_done_date, smpp_dlvr_rcpt_proc.done_date);
		    // delivery receipt stat
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_stat, smpp_dlvr_rcpt_proc.stat);
		    // delivery receipt err
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_err, smpp_dlvr_rcpt_proc.err);
		    // delivery receipt text
		    if(smpp_dlvr_rcpt_proc.text_size > 0)
			pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_dlvr_rcpt_text, smpp_dlvr_rcpt_proc.text, smpp_dlvr_rcpt_proc.text_size);

		// try variant 2
		}else if(smpp_dlvr_rcpt_proc.decode_var_2(tmppdu->short_message, tmppdu->sm_length) == 0){
		    // format
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_format, 2);
		    // delivery receipt id
		    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_dlvr_rcpt_id, smpp_dlvr_rcpt_proc.id);
		    // delivery receipt sub
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_sub, smpp_dlvr_rcpt_proc.sub);
		    // delivery receipt dlvrd
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_dlvrd, smpp_dlvr_rcpt_proc.dlvrd);
		    // delivery receipt submit date
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_submit_date, smpp_dlvr_rcpt_proc.submit_date);
		    // delivery receipt done date
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_done_date, smpp_dlvr_rcpt_proc.done_date);
		    // delivery receipt stat
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_stat, smpp_dlvr_rcpt_proc.stat);
		    // delivery receipt err
		    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_err, smpp_dlvr_rcpt_proc.err);
		    // delivery receipt text
		    if(smpp_dlvr_rcpt_proc.text_size > 0)
			pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_dlvr_rcpt_text, smpp_dlvr_rcpt_proc.text, smpp_dlvr_rcpt_proc.text_size);

		}
		// check for optional RECEIPTED_MESSAGE_ID parameter
		tmpparam = tmppdu->get_parameter(smpp::RECEIPTED_MESSAGE_ID);
		if(tmpparam != NULL) pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_dlvr_rcpt_id, (char*)tmpparam->param_data, 1);
		// check for optional MESSAGE_STATE parameter
		tmpparam = tmppdu->get_parameter(smpp::MESSAGE_STATE);
		if(tmpparam != NULL && tmpparam->length > 0) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_dlvr_rcpt_stat, tmpparam->param_data[0], 1);

	    }

	    break;
	}

	// deliver_sm_resp
	case smpp::DELIVER_SM_RESP:
	{
	    // deliver_sm_resp pointer
	    smpp::Deliver_sm_resp* tmppdu = (smpp::Deliver_sm_resp*)pdu->command;
	    // messsage id
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_message_id, tmppdu->message_id);
	    // command status
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_command_status, pdu->command_status);
	    break;
	}


	// data_sm
	case smpp::DATA_SM:
	{
	    // data_sm pointer
	    smpp::Data_sm* tmppdu = (smpp::Data_sm*)pdu->command;
	    // service type
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_service_type, tmppdu->service_type);
	    // originator ton
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_originator_ton, tmppdu->source_addr_ton);
	    // originator np
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_originator_np, tmppdu->source_addr_npi);
	    // originator address
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_originator_address, tmppdu->source_addr);
	    // recipient ton
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_recipient_ton, tmppdu->dest_addr_ton);
	    // recipient np
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_recipient_np, tmppdu->dest_addr_npi);
	    // recipient address
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_recipient_address, tmppdu->destination_addr);
	    // esm mm
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_esm_message_mode, tmppdu->esm_class_mm);
	    // esm mt
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_esm_message_type, tmppdu->esm_class_mt);
	    // esm gsm features
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_esm_gsm_features, tmppdu->esm_gsm_specific);
	    // registered delivery receipt
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_rd_smsc_receipt, tmppdu->rd_smsc_dlvr_rcpt);
	    // registered delivery sme ack
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_rd_sme_ack, tmppdu->rd_sme_orig_ack);
	    // registered delivery intermediate notification
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_rd_intermediate_notification, tmppdu->rd_intrmd_ntf);
	    // data coding
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_data_coding, tmppdu->data_coding);
	    // get optional message payload
	    smpp::SmppParam* tmpparam = tmppdu->get_parameter(smpp::MESSAGE_PAYLOAD);
	    if(tmpparam != NULL){
		pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_sm, tmpparam->param_data, tmpparam->length);
		pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sm_length, tmpparam->length);
	    }
	    // check concatenated sar params
	    // sar msg ref num
	    tmpparam = tmppdu->get_parameter(smpp::SAR_MSG_REF_NUM);
	    if(tmpparam != NULL) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sar_msg_ref_num, ((smpp::Sar_msg_ref_num*)tmpparam)->value);
	    // sar total segments
	    tmpparam = tmppdu->get_parameter(smpp::SAR_TOTAL_SEGMENTS);
	    if(tmpparam != NULL) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sar_total_segments, ((smpp::Sar_total_segments*)tmpparam)->value);
	    // sar segment seqnum
	    tmpparam = tmppdu->get_parameter(smpp::SAR_SEGMENT_SEQNUM);
	    if(tmpparam != NULL) pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sar_segment_seqnum, ((smpp::Sar_segment_seqnum*)tmpparam)->value);
	    break;
	}
	// data_sm_resp
	case smpp::DATA_SM_RESP:
	{
	    // data_sm_resp pointer
	    smpp::Data_sm_resp* tmppdu = (smpp::Data_sm_resp*)pdu->command;
	    // messsage id
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_message_id, tmppdu->message_id);
	    // command status
	    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_command_status, pdu->command_status);
	    break;
	}

	default: break;
    }
}


template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::process_dr(PLD_TYPE* pld){
    // skip if dr is off
    if(!retain_data) return 0;

    // smppp pointer
    smpp::SmppPDU* smppp = &pld->smpp_pdu;

    // get pmink payload
    PminkPayload* pmpld = Connection<PLD_TYPE>::in_pool.allocate_constructed();
    if(pmpld == NULL){
	// stats
	ConnectionBase::stats.inc(CST_PMPLD_POOL_EMPTY, 1);
	return 1;
    }
    // clear params
    pmpld->params.clear_params();
    // *** generate params ***
    // connection type
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_connection_type, Connection<PLD_TYPE>::type);
    // raw smpp packet
    pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_header_data, pld->raw, pld->size);
    // timestamp
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_timestamp, ts.tv_sec);
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_timestamp_nsec, ts.tv_nsec);

    // proces smpp
    process_smpp(smppp, pmpld);

    // forward pmpld params to stp
    pmpld_dr(pmpld);
    // ok
    return 0;
}


template<typename PLD_TYPE>
void sgn::_SMPPConnection<PLD_TYPE>::pmpld_notify(PminkPayload* _pmpld){
    /*
	// notify daemon
	// get peer info
	sockaddr_in addr;
	socklen_t len = sizeof(addr);
	getpeername(Connection<PLD_TYPE>::client_socket, (sockaddr*)&addr, &len);
	// peer ip
	//pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_peer_ip, inet_ntoa(addr.sin_addr));
	// peer port
	//pmpld->params.set_int(asn1::ParameterType::_pt_smpp_peer_port, be16toh(addr.sin_port));
     */

    // check for guid
    pmink_utils::VariantParam* vp_guid = _pmpld->params.get_param(asn1::ParameterType::_pt_pmink_guid);
    if(vp_guid == NULL) return;

    // check for source type
    pmink_utils::VariantParam* vp_src_type = _pmpld->params.get_param(asn1::ParameterType::_pt_pmink_daemon_type);
    if(vp_src_type == NULL) return;

    // check for source type
    pmink_utils::VariantParam* vp_src_id = _pmpld->params.get_param(asn1::ParameterType::_pt_pmink_daemon_id);
    if(vp_src_id == NULL) return;

    // check service id
    pmink_utils::VariantParam* vp_srvc_id = _pmpld->params.get_param(asn1::ParameterType::_pt_pmink_service_id);


    // get new pmink payload
    PminkPayload* pmpld = Connection<PLD_TYPE>::in_pool.allocate_constructed();
    if(pmpld == NULL){
	// stats
	ConnectionBase::stats.inc(CST_PMPLD_POOL_EMPTY, 1);
	return;
    }
    // clear params
    pmpld->params.clear_params();

    // set smppp pointer (decoded previously)
    smpp::SmppPDU* smppp = &_pmpld->smpp_pld.smpp_pdu;


    // get peer info
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getpeername(Connection<PLD_TYPE>::client_socket, (sockaddr*)&addr, &len);
    // peer ip
    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_peer_ip, inet_ntoa(addr.sin_addr));
    // peer port
    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_peer_port, be16toh(addr.sin_port));
    // username
    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_system_id, smpp_user.system_id);

    // connection type
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_connection_type, Connection<PLD_TYPE>::type);
    // sequence num
    pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sequence, smppp->sequence_number);
    // guid
    pmpld->params.set_octets(asn1::ParameterType::_pt_pmink_guid, (unsigned char*)*vp_guid, vp_guid->get_size());
    // dest type
    pmpld->params.set_cstr(asn1::ParameterType::_pt_pmink_daemon_type, (char*)*vp_src_type);
    // dest id
    pmpld->params.set_cstr(asn1::ParameterType::_pt_pmink_daemon_id, (char*)*vp_src_id);
    // get as pointer
    SMPP_ApplicationServer* tmp_as = NULL;
    if(ConnectionBase::master_conn == NULL) tmp_as = as.get();
    else tmp_as = ((SMPPConnection*)ConnectionBase::master_conn)->as.get();
    if(tmp_as != NULL) pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_as_label, tmp_as->get_label());
    // asp label
    const char* tmp_label = ConnectionBase::label.c_str();
    if(ConnectionBase::master_conn != NULL) tmp_label = ConnectionBase::master_conn->get_label();
    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_asp_label, tmp_label);
    // command id
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_command_id, asn1::SmsHubCommandId::_shci_corr_ntf);
    // trunk label
    pmpld->params.set(_pmpld->params,
		      pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_trunk_label, 2),
		      pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_trunk_label, 2));

    // packet data
    pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_header_data, _pmpld->raw, _pmpld->size);



    // *********************************************
    // ************ push to daemon *************
    // *********************************************
    // get new router if connection broken
    if(stp_r14pc == NULL || !stp_r14pc->is_registered()) stp_r14pc = r14ps->get_registered_client("routingd");
    // get first routing daemon
    r14p::R14PClient* r14pc = stp_r14pc;
    // null check
    if(r14pc == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // allocate new service message
    r14p::ServiceMessage* msg = r14psmm->new_smsg();
    // msg sanity check
    if(msg == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_MSG_POOL_EMPTY, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // service id
    msg->set_service_id(asn1::ServiceId::_sid_smshub_forward);
    // check for service id override
    if(vp_srvc_id != NULL) msg->set_service_id((int)*vp_srvc_id);
    // copy params
    msg->vpmap = pmpld->params;
    // free pmpld
    Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(msg) != 0){
	// free
	r14psmm->free_smsg(msg);
	ConnectionBase::stats.inc(CST_SMSG_PARAM_POOL_EMPTY, 1);
	return;

    }
    // inc refc counter for conn
    this->refc_inc();
    // set service message params
    msg->params.set_param(2, this);
    // send service message
    int res = r14psmm->send(msg, r14pc, (char*)*vp_src_type, (char*)*vp_src_id, true, &smsg_async_done);
    if(res != 0){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	r14psmm->free_smsg(msg);
	// dec refc counter for conn
	this->refc_dec();
    }


    // *********************************************
    // *********************************************

}


template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::process_stp(PLD_TYPE* pld){
    // smppp pointer
    smpp::SmppPDU* smppp = &pld->smpp_pdu;

    // get pmink payload
    PminkPayload* pmpld = Connection<PLD_TYPE>::in_pool.allocate_constructed();
    if(pmpld == NULL){
	// stats
	ConnectionBase::stats.inc(CST_PMPLD_POOL_EMPTY, 1);
	return 1;
    }
    // clear params
    pmpld->params.clear_params();
    // *** generate params ***
    // connection type
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_connection_type, Connection<PLD_TYPE>::type);
    // source daemon type
    pmpld->params.set_cstr(asn1::ParameterType::_pt_pmink_daemon_type, pmink::CURRENT_DAEMON->get_daemon_type());
    // source daemon id
    pmpld->params.set_cstr(asn1::ParameterType::_pt_pmink_daemon_id, pmink::CURRENT_DAEMON->get_daemon_id());
    // raw smpp packet
    pmpld->params.set_octets(asn1::ParameterType::_pt_smpp_header_data, pld->raw, pld->size);
    // timestamp
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_timestamp, ts.tv_sec);
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_timestamp_nsec, ts.tv_nsec);

    // proces smpp
    process_smpp(smppp, pmpld);

    // forward pmpld params to stp
    pmpld_stp(pmpld);
    // ok
    return 0;
}



template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::proc_process(PLD_TYPE* pld){
    // set smppp pointer
    smpp::SmppPDU* smppp = &pld->smpp_pdu;
    // decode
    if(smpp::decode(pld->raw, pld->size, smppp, &smpp_proc_pool) != 0) return 1;
    // dr
    process_dr(pld);
    // routing
    process_stp(pld);
    // ok
    return 0;
}


template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::proc_process_ext(PminkPayload* pmpld){
    // debug
    #ifdef PMDEBUG
    pmpld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== PROCESS/ROUTE R14P --> SMPP =====" << std::endl;
	std::cout << pmpld->params;
	std::cout << "=======================================" << std::endl;
    )

    // generate new sequence flag
    bool gen_seq = true;
    // check for special pmink cmd
    pmink_utils::VariantParam* vp = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_command_id);
    // if special cmd found
    if(vp != NULL){
	// do not generate new sequence
	gen_seq = false;
	// process special cmd
	if(cmd_process(pmpld, (int)*vp) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_CMD_PROCESS_ERROR, 1);
	    // free
	    Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	    return 1;

	// get raw packet data after conversion
	}else vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_header_data);

    // normal traffic
    }else{
	// get raw packet data
	vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_header_data);
	// missing smpp header data
	if(vp == NULL){
	    // try conversion
	    if(convert(pmpld) != 0){
		// stats
		ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);
		// free
		Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
		return 1;

	    // get raw packet data after conversion
	    }else vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_header_data);

	}

    }


    // set smppp pointer
    smpp::SmppPDU* smppp = &pmpld->smpp_pld.smpp_pdu;

    // decode
    if(smpp::decode((unsigned char*)*vp, vp->get_size(), smppp, &smpp_proc_pool) != 0){
	// stats
	ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return 2;

    }

    // generate sequence
    if(gen_seq){
	// update sequence number
	smppp->sequence_number = out_seq_num.add_fetch(1);
	// update
	pmpld->params.set_int(asn1::ParameterType::_pt_smpp_sequence, smppp->sequence_number);
    }


    // *** check smpp translations ***
    switch(smppp->command_id){
	// deliver_sm
	case smpp::DELIVER_SM:
	{
	    smpp::Deliver_sm* dsm = (smpp::Deliver_sm*)smppp->command;
	    // originator address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_originator_address, 2);
	    if(vp != NULL) dsm->source_addr = (char*)*vp;
	    // originator ton
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_originator_ton, 2);
	    if(vp != NULL) dsm->source_addr_ton = (smpp::SmppTON)(int)*vp;
	    // originator npi
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_originator_np, 2);
	    if(vp != NULL) dsm->source_addr_npi = (smpp::SmppNPI)(int)*vp;

	    // destination address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_recipient_address, 2);
	    if(vp != NULL) dsm->destination_addr = (char*)*vp;
	    // destination ton
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_recipient_ton, 2);
	    if(vp != NULL) dsm->dest_addr_ton = (smpp::SmppTON)(int)*vp;
	    // destination npi
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_recipient_np, 2);
	    if(vp != NULL) dsm->dest_addr_npi = (smpp::SmppNPI)(int)*vp;
	    break;
	}
	// submit_sm
	case smpp::SUBMIT_SM:
	{
	    smpp::Submit_sm* ssm = (smpp::Submit_sm*)smppp->command;
	    // originator address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_originator_address, 2);
	    if(vp != NULL) ssm->source_addr = (char*)*vp;
	    // originator ton
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_originator_ton, 2);
	    if(vp != NULL) ssm->source_addr_ton = (smpp::SmppTON)(int)*vp;
	    // originator npi
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_originator_np, 2);
	    if(vp != NULL) ssm->source_addr_npi = (smpp::SmppNPI)(int)*vp;

	    // destination address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_recipient_address, 2);
	    if(vp != NULL) ssm->destination_addr = (char*)*vp;
	    // destination ton
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_recipient_ton, 2);
	    if(vp != NULL) ssm->dest_addr_ton = (smpp::SmppTON)(int)*vp;
	    // destination npi
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_recipient_np, 2);
	    if(vp != NULL) ssm->dest_addr_npi = (smpp::SmppNPI)(int)*vp;
	    break;
	}

	default: break;
    }

    // encode
    if(smppp->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size) != 0){
	// stats
	ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return 101;
    }

    // check for correlation notification
    pmink_utils::VariantParam* vp_corr_ntf = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_correlation_notification);
    if(vp_corr_ntf != NULL && (bool)*vp_corr_ntf){
	pmpld_notify(pmpld);
    }

    // send
    pmpld->action = PAT_SEND;
    Connection<PLD_TYPE>::proc_tx_q.push(1, pmpld);

    // ok
    return 0;
}

template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::rx_on_start(){
    // state machine on
    if((bool)*ConnectionBase::params.get_param(CPT_DEV_SMPP_SM)){
	return smpp_connect();

    // state machine off
    }else{
	// set smpp part as active
	smpp_active.comp_swap(false, true);
	// run event handler
	pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;
	e_args.add_arg(CEAT_CONNECTION, this);
	ConnectionBase::e_handler.run_handler(CET_CONNECTION_UP, e_args);
	// stats
	ConnectionBase::stats.set(CST_SMPP_ACTIVE, 1);
	if(ConnectionBase::master_conn != NULL) ConnectionBase::master_conn->stats.inc(CST_SMPP_ACTIVE, 1);
	return 0;
    }
}



template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::reconnect(){
    // stats
    Connection<PLD_TYPE>::stats.set(CST_SMPP_ACTIVE, 0);
    if(ConnectionBase::master_conn != NULL && ConnectionBase::master_conn->stats.get(CST_SMPP_ACTIVE) > 0) ConnectionBase::master_conn->stats.dec(CST_SMPP_ACTIVE, 1);
    // tcp reconnect
    sgn::_TCPConnection<PLD_TYPE>::reconnect();
    // reset reassembly data
    sep_buff_p = sep_buff;
    sep_buff_l = 0;
    next_pdu_l = 0;
    next_pdu_rb = 0;
    // set smpp as inactive
    smpp_active.set(false);
    // server mode, stop client threads
    if(ConnectionBase::direction == CD_SERVER){
	// stop client threads
	Connection<PLD_TYPE>::cthreads_active.set(false);
	// dec ref count if one to many child connection (reconnect will be done by master conn)
	if(ConnectionBase::master_conn != NULL) {
	    ConnectionBase::refc_dec();
	    return 0;
	}
    }

    // state machine on
    if((bool)*ConnectionBase::params.get_param(CPT_DEV_SMPP_SM)){
	// connect smpp layer
	return smpp_connect();

    // state machine off
    }else{
	// do only for master client connection
	if(ConnectionBase::master_conn == NULL && ConnectionBase::direction == CD_CLIENT){
	    // set smpp part as active
	    smpp_active.comp_swap(false, true);
	    // run event handler
	    pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;
	    e_args.add_arg(CEAT_CONNECTION, this);
	    ConnectionBase::e_handler.run_handler(CET_CONNECTION_UP, e_args);
	    // stats
	    ConnectionBase::stats.set(CST_SMPP_ACTIVE, 1);

	}
	return 0;

    }
}

template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::data_rx(unsigned char* buff, int size, int* bytes_rvcd, int* pld_type, void* extra){
    // receive data
    int res = _TCPConnection<PLD_TYPE>::data_rx(buff, size, bytes_rvcd, pld_type, extra);
    // eerr check
    if(res == 0){
	// ****************************************************************************************
	// SMPP TCP reassembly
	// ****************************************************************************************
	//
	// sep_buff	-	reassembly buffer
	// sep_buff_l	-	number of bytes used in reassembly buffer
	// sep_buff_p 	- 	pointer to next free byte in reassembly buffer
	// next_pdu_l	-	length of current PDU
	// next_pdu_rb	-	number of bytes received for the current PDU
	// pdu_end_p	-	pointer to one byte past last PDU's byte in reassembly buffer
	//
	// ****************************************************************************************
	// sanity check
	if(sep_buff_p + *bytes_rvcd > &sep_buff[sizeof(sep_buff) - 1]) return 1;
	// copy data to sep buffer
	memcpy(sep_buff_p, buff, *bytes_rvcd);
	// fwd buff pointer
	sep_buff_p += *bytes_rvcd;
	// update sep buffer length
	sep_buff_l += *bytes_rvcd;
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
		    // process data
		    data_rx_process(pdu_end_p - next_pdu_l, next_pdu_l, *pld_type, extra);
		    // inc counter manually (skipped in rx_logic when *bytes_rvcd == 0)
		    ConnectionBase::stats.inc(CST_RX_MSU_COUNT, 1);
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
    }

    // set to zero to avoid running data_rx_process from connection base rx_logic
    // data_rx_process has already been called in this method
    *bytes_rvcd = 0;

    // return res from tcp recv
    return res;

}


template<typename PLD_TYPE>
void sgn::_SMPPConnection<PLD_TYPE>::terminate(){
    if(terminated) return;

    pmink_utils::VariantParam* vp = ConnectionBase::params.get_param(CPT_R14P_STATS);
    if(vp == NULL) return;
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)(void*)*vp;

    // remove stats handlers
    std::string stats_prefix("SMPP::");
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_RX_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TX_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_MTU_LIMIT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_DISCARDED_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_PLD_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_MALFORMED_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_RX_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TX_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_PMPLD_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMSG_SEND_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMSG_MSG_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMSG_PARAM_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMPP_ACTIVE")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TCP_ACTIVE")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMSG_SENT_COUNT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_NO_VALID_ROUTES")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_RX_HBEAT_COUNT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TX_HBEAT_COUNT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_CMD_PROCESS_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_ENCODE_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_RECONNECT")));

    terminated = true;




}

template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::start(){
    // get dr flag
    retain_data = (bool)*ConnectionBase::params.get_param(CPT_DATA_RETENTION);

    // get bind  timeout
    bind_timeout = (int)*ConnectionBase::params.get_param(CPT_SMPP_RESPONSE_TIMER);

    // get max connection count
    Connection<PLD_TYPE>::max_in_connc = (int)*ConnectionBase::params.get_param(CPT_MAX_CONN_COUNT);

    // get reconnect timer
    Connection<PLD_TYPE>::reconnect_timer = (int)*ConnectionBase::params.get_param(CPT_RECONNECT_TIMER);

    // enquire link timer
    enql_ts = (int)*ConnectionBase::params.get_param(CPT_SMPP_ENQUIRE_LINK_TIMER);

    // enquire link enabled/disabled
    enql_enabled = (bool)*ConnectionBase::params.get_param(CPT_DEV_SMPP_SM);

    // tcp start
    int tcp_res = _TCPConnection<PLD_TYPE>::start();
    if(tcp_res < 0) pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
						"Error while initializing TCP socket for SMPP connection [%s]",
						 Connection<PLD_TYPE>::label.c_str());
    // ok
    return 0;
}

template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::stop(){
    // smpp inactive
    smpp_active.comp_swap(true, false);
    // stats
    Connection<PLD_TYPE>::stats.set(CST_SMPP_ACTIVE, 0);
    // stop tcp
    _TCPConnection<PLD_TYPE>::stop();
    // ok
    return 0;
}

template<typename PLD_TYPE>
int sgn::_SMPPConnection<PLD_TYPE>::smpp_connect(){
    // reset seq counter
    out_seq_num.set(0);
    // client mode
    if(Connection<PLD_TYPE>::direction == CD_CLIENT){
	// if socket connected
	if(Connection<PLD_TYPE>::client_socket != -1){
	    // get payload packet
	    PLD_TYPE* pld = NULL;
	    if(Connection<PLD_TYPE>::pld_pool.pop(&pld) != 0){
		// stats
		ConnectionBase::stats.inc(CST_PLD_POOL_EMPTY, 1);
		return 1;
	    }

	    // get smpp packet
	    smpp::SmppPDU* smppp = &pld->smpp_pdu;

	    // get bind meethod
	    smpp::SmppCommandId bind_method = (smpp::SmppCommandId)(int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_METHOD);

	    // username
	    strncpy(smpp_user.system_id, (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_SYSTEM_ID), sizeof(smpp_user.system_id));

	    // get bind command
	    smpp::SmppCommand* tmpcmd = smpp_rx_pool.request_command(bind_method);
	    if(tmpcmd != NULL){
		// set params
		smppp->command_id = bind_method;
		smppp->command_status = smpp::ESME_ROK;
		smppp->command = tmpcmd;
		smppp->sequence_number = out_seq_num.add_fetch(1);
		smppp->command->clear_params();
		// check bind type
		switch(smppp->command_id){
		    case smpp::BIND_TRANSMITTER:
		    {
			smpp::Bind_transmitter* bind = (smpp::Bind_transmitter*)smppp->command;
			bind->system_id = (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_SYSTEM_ID);
			bind->password = (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_PASSWORD);
			bind->system_type = (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_SYSTEM_TYPE);
			bind->interface_version = (int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_IF_VERSION);
			bind->addr_ton = (smpp::SmppTON)(int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_TON);
			bind->addr_npi = (smpp::SmppNPI)(int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_NPI);
			bind->address_range = (char*)"";
			break;
		    }
		    case smpp::BIND_RECEIVER:
		    {
			smpp::Bind_receiver* bind = (smpp::Bind_receiver*)smppp->command;
			bind->system_id = (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_SYSTEM_ID);
			bind->password = (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_PASSWORD);
			bind->system_type = (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_SYSTEM_TYPE);
			bind->interface_version = (int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_IF_VERSION);
			bind->addr_ton = (smpp::SmppTON)(int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_TON);
			bind->addr_npi = (smpp::SmppNPI)(int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_NPI);
			bind->address_range = (char*)"";
			break;
		    }

		    case smpp::BIND_TRANSCEIVER:
		    {
			smpp::Bind_transceiver* bind = (smpp::Bind_transceiver*)smppp->command;
			bind->system_id = (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_SYSTEM_ID);
			bind->password = (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_PASSWORD);
			bind->system_type = (char*)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_SYSTEM_TYPE);
			bind->interface_version = (int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_IF_VERSION);
			bind->addr_ton = (smpp::SmppTON)(int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_TON);
			bind->addr_npi = (smpp::SmppNPI)(int)*Connection<PLD_TYPE>::params.get_param(CPT_SMPP_BIND_NPI);
			bind->address_range = (char*)"";
			break;
		    }
		    default: break;


		}

		// encode
		if(smppp->encode(pld->raw,
			      Connection<PLD_TYPE>::max_mtu,
			      0,
			      &pld->size) != 0){

		    // stats
		    ConnectionBase::stats.inc(CST_ENCODE_ERROR, 1);
		    // free pld
		    pld->action = PAT_FREE;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		    return 101;
		}

		// connect phase
		conn_phase = 0;

		// push to out
		pld->action = PAT_SEND;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		// set bind timestamp
		bind_ts = time(NULL);

		// ok
		return 0;

	    }else{
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

	    }
	}

    // server mode
    }else{
	conn_phase = 10;
	// ok
	return 0;

    }


    return 1;
}


// M3UAConnection
sgn::M3UAConnection::M3UAConnection(ConnectionDirection _direction,
				    ServerConnectionType _server_type,
				    const char* _label,
				    int _max_mtu,
				    int _pool_size): _M3UAConnection<M3uaPayload>(_direction,
										  _server_type,
										  _label,
										  _max_mtu,
										  _pool_size){

}
// _M3UAConnection
template<typename PLD_TYPE>
sgn::_M3UAConnection<PLD_TYPE>::_M3UAConnection(ConnectionDirection _direction,
						ServerConnectionType _server_type,
			   		        const char* _label,
						int _max_mtu,
						int _pool_size): _SCTPConnection<PLD_TYPE>( _direction,
											   _server_type,
											   _label,
											   _max_mtu,
											   _pool_size),
											   conn_phase(0),
                                                                                           as_status_info(m3ua::AS_INACTIVE),
											   opc(0),
											   dpc(0),
											   beat_data(0),
											   rng_dstr(1, std::numeric_limits<uint32_t>::max()),
											   retain_data(false),
											   aspup_ts(0),
											   send_e_handler(this),
											   terminated(false){

    // connection type
    Connection<PLD_TYPE>::type = CT_M3UA;

    // default params
    Connection<PLD_TYPE>::params.set_int(CPT_M3UA_TRAFFIC_MODE, m3ua::LOADSHARE);
    Connection<PLD_TYPE>::params.set_int(CPT_M3UA_ASPUP_TIMER, 2);
    Connection<PLD_TYPE>::params.set_int(CPT_RECONNECT_TIMER, 2);
    Connection<PLD_TYPE>::params.set_int(CPT_M3UA_HBEAT_TIMER, 10);
    Connection<PLD_TYPE>::params.set_int(CPT_M3UA_RT_CTX, 0xffffffff);
    Connection<PLD_TYPE>::params.set_int(CPT_M3UA_OPC, 0);
    Connection<PLD_TYPE>::params.set_int(CPT_M3UA_DPC, 0);
    Connection<PLD_TYPE>::params.set_bool(CPT_DATA_RETENTION, false);
    Connection<PLD_TYPE>::params.set_bool(CPT_DEV_M3UA_SM, true);
    // payload decoder params
    Connection<PLD_TYPE>::params.set_bool(CPT_DECODE_M3UA, true);
    Connection<PLD_TYPE>::params.set_bool(CPT_DECODE_SCCP, true);
    Connection<PLD_TYPE>::params.set_bool(CPT_DECODE_ISUP, true);
    Connection<PLD_TYPE>::params.set_bool(CPT_DECODE_TCAP, true);
    Connection<PLD_TYPE>::params.set_bool(CPT_DECODE_GSM_MAP, true);
    Connection<PLD_TYPE>::params.set_bool(CPT_DECODE_SMS_TPDU, true);

    decoders[0] = true; // CPT_DECODE_M3UA
    decoders[1] = true; // CPT_DECODE_SCCP
    decoders[2] = true; // CPT_DECODE_ISUP
    decoders[3] = true; // CPT_DECODE_TCAP
    decoders[4] = true; // CPT_DECODE_GSM_MAP
    decoders[5] = true; // CPT_DECODE_SMS_TPDU

    // hbeat
    hbeat_ts = 10;
    hbeat_last_ts = time(NULL);
    hbeat_enabled = true;


    // default asp up timeout
    aspup_timeout = 2;

    // opc
    opc = 0;

    // last DATA sid
    last_sid.set(1);

    // m3ua pool
    m3ua_rx_pool.init_pool();
    m3ua_proc_pool.init_pool();
    hbeat_m3uap._pool.set_pool_size(1, 1);
    hbeat_m3uap._pool.init_pool();

    // sccp pool
    sccp_pool.set_pool_size(5, 1);
    sccp_pool.init_pool();
    // tcap
    tcm.prepare();
    tcm_enc.prepare();
    // gsmmap
    mo_fwd_sm.prepare();
    mt_fwd_sm.prepare();
    mt_fwd_sm_enc.prepare();
    sri_sm_arg.prepare();
    sri_sm_arg_enc.prepare();
    sri_sm_res.prepare();
    sri_sm_res_enc.prepare();
    gsmmap_error_code.prepare();
    gsmmap_error_code_enc.prepare();
    dialogue_pdu.prepare();
    dialogue_pdu_enc.prepare();

    // init asn1 object pool
    asn1_pool.set_pool_size(500, 500);
    asn1_pool.init_pool();

    // converters
    UErrorCode icu_err = U_ZERO_ERROR;
    conv_utf8 = ucnv_open("utf-8", &icu_err);
    iconv_gsm7_to_utf8 = libiconv_open("UTF-8", "GSM-7");
    iconv_utf8_to_gsm7 = libiconv_open("GSM-7", "UTF-8");


    // random number
    rng.seed(time(NULL));


    // register stats
    ConnectionBase::stats.register_item(CST_MTU_LIMIT);
    ConnectionBase::stats.register_item(CST_DISCARDED_MSU);
    ConnectionBase::stats.register_item(CST_PLD_POOL_EMPTY);
    ConnectionBase::stats.register_item(CST_MALFORMED_MSU);
    ConnectionBase::stats.register_item(CST_PMPLD_POOL_EMPTY);
    ConnectionBase::stats.register_item(CST_SMSG_SEND_ERROR);
    ConnectionBase::stats.register_item(CST_SMSG_MSG_POOL_EMPTY);
    ConnectionBase::stats.register_item(CST_SMSG_PARAM_POOL_EMPTY);
    ConnectionBase::stats.register_item(CST_M3UA_ACTIVE);
    ConnectionBase::stats.register_item(CST_SCTP_ACTIVE);
    ConnectionBase::stats.register_item(CST_SMSG_SENT_COUNT);
    ConnectionBase::stats.register_item(CST_RX_HBEAT_COUNT);
    ConnectionBase::stats.register_item(CST_TX_HBEAT_COUNT);
    ConnectionBase::stats.register_item(CST_CMD_PROCESS_ERROR);
    ConnectionBase::stats.register_item(CST_ENCODE_ERROR);
    ConnectionBase::stats.register_item(CST_RECONNECT);

    // loop protection
    loop_cmap = (pmink_utils::CorrelationMap<uint32_t, uint32_t>*)pmink::CURRENT_DAEMON->get_param(4);

    // get r14p session manager
    r14ps = (r14p::R14PSession*)pmink::CURRENT_DAEMON->get_param(2);
    // get r14p service msg manager
    r14psmm = (r14p::ServiceMsgManager*)pmink::CURRENT_DAEMON->get_param(1);
    // data retention r14p client
    dr_r14pc = r14ps->get_registered_client("routingd");
    // stp r14p client
    stp_r14pc = r14ps->get_registered_client("routingd");
    // router r14p client
    rt_r14pc = r14ps->get_registered_client("routingd");
    // set handler
    ConnectionBase::e_handler.set_handler(CET_SEND_ERROR, &send_e_handler);



}

template<typename PLD_TYPE>
sgn::_M3UAConnection<PLD_TYPE>::~_M3UAConnection(){
    terminate();
    stop();

    // tcap
    root_tcap_node.children.clear();
    root_tcap_node.tlv = NULL;

    root_tcap_dialogue_node.children.clear();
    root_tcap_dialogue_node.tlv = NULL;

    // gsm map
    root_gsmmap_node.children.clear();
    root_gsmmap_node.tlv = NULL;

    root_gsmmap_err_node.children.clear();
    root_gsmmap_err_node.tlv = NULL;

    // close converters
    ucnv_close(conv_utf8);
    libiconv_close(iconv_gsm7_to_utf8);
    libiconv_close(iconv_utf8_to_gsm7);

    // free logic
    delete ConnectionBase::logic;


}

template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::extra_logic(){
    // skip if state machine is off
    if(!hbeat_enabled) return 10;
    // connection shutting down
    if(!ConnectionBase::active.get()) return 10;
    if(!Connection<PLD_TYPE>::cthreads_active.get()) return 10;
    if(Connection<PLD_TYPE>::reconnecting.get()) return 10;
    // check time diff
    if(time(NULL) - hbeat_last_ts < hbeat_ts) return 10;
    // update ts
    hbeat_last_ts = time(NULL);
    // check if m3ua is active
    if(!m3ua_active.get()) return 10;
    // if 2 beats missed, set m3ua as inactive
    if(beat_miss.get() >= 2){
	// set as inactive
	m3ua_active.comp_swap(true, false);
	// flag for reconnection
	Connection<PLD_TYPE>::reconnect_queued.comp_swap(false, true);
	// reset counters
	beat_miss.set(0);
	beat_diff.set(0);
	return 10;
    }

    // check if waiting for BEAT ACK
    if(beat_diff.get() > 0){
	// one beat missed
	beat_miss.fetch_add(1);
	return 10;
    }

    // reset missed
    beat_miss.set(0);
    // set beat data
    ++beat_data;
    // diff (+1)
    beat_diff.set(1);

    // m3ua BEAT
    hbeat_m3uap.message_class = m3ua::ASPSM;
    hbeat_m3uap.message_type = m3ua::BEAT;
    m3ua::Beat* beat = (m3ua::Beat*)hbeat_m3uap._pool.request_message(m3ua::BEAT);
    hbeat_m3uap.message = beat;
    beat->clear_params();
    m3ua::HeartbeatData* bdata = (m3ua::HeartbeatData*)hbeat_m3uap._pool.request_param(m3ua::M3UA_PT_HEARTBEAT);
    bdata->value = (unsigned char*)&beat_data;
    bdata->value_length = sizeof(uint32_t);
    beat->add_parameter(bdata);

    unsigned char buff[64];
    int size = 0;

    // encode
    hbeat_m3uap.encode(buff, sizeof(buff), 0, &size);
    // send
    if(sctp::send_sctp(ConnectionBase::client_socket, buff, size, sctp::M3UA, 0) != 0){
	// set as inactive
	m3ua_active.comp_swap(true, false);
	// socket down, flag for reconnection
	Connection<PLD_TYPE>::reconnect_queued.comp_swap(false, true);

    }else{
	// stats
	ConnectionBase::stats.inc(CST_TX_MSU_COUNT, 1);

    }

    return 10;
}



template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::rx_on_start(){
    // state machine on
    if((bool)*ConnectionBase::params.get_param(CPT_DEV_M3UA_SM)){
	return m3ua_connect();

    // state machine off
    }else{
	m3ua_active.set(true);
	// run event handler
	pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;
	e_args.add_arg(CEAT_CONNECTION, this);
	ConnectionBase::e_handler.run_handler(CET_CONNECTION_UP, e_args);
	// stats
	ConnectionBase::stats.set(CST_M3UA_ACTIVE, 1);
	return 0;

    }
}

template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::translate_calling_pa(sccp::CallingPA* cpa, PminkPayload* pmpld, sccp::SCCPPool* _sccp_pool){
    // check for null called party
    if(cpa == NULL) return;
    // vars
    pmink_utils::VariantParam* vp = NULL;

    // routing indicator
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, 2);
    if(vp != NULL) cpa->routing_indicator = (sccp::RoutingIndicator)(int)*vp;

    // global title indicator
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, 2);
    if(vp != NULL) {
	cpa->global_title_indicator = (sccp::GlobalTitleIndicator)(int)*vp;
	// check if new global title type should be set
	if(cpa->global_title == NULL || cpa->global_title->type != cpa->global_title_indicator){
	    cpa->global_title = _sccp_pool->request_gt(cpa->global_title_indicator);
	}
    }

    // ssn
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, 2);
    if(vp != NULL) {
	cpa->ssn_indicator = true;
	cpa->subsystem_number = (sccp::SubsystemNumber)(int)*vp;
    }

    // point-code
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, 2);
    if(vp != NULL) {
	cpa->point_code_indicator = true;
	cpa->point_code = (int)*vp;
    }

    // gt translations
    // check GT type
    switch(cpa->global_title->type){
	case sccp::GT_NATURE_OF_ADDRESS_INDICATOR_ONLY:
	{
	    sccp::GT_NOA* tmp_gt = (sccp::GT_NOA*)cpa->global_title;
	    // nai
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, 2);
	    if(vp != NULL) tmp_gt->nature_of_address = (sccp::NatureOfAddress)(int)*vp;
	    // address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, 2);
	    if(vp != NULL && strlen((char*)*vp) > 0) {
		unsigned char tmp_bcd[64];
		int nb = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp, vp->get_size() - 1, tmp_bcd);
		// save encoded
		pmink_utils::VariantParam* enc_vp = pmpld->params.set_octets(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, tmp_bcd, nb, 3);
		tmp_gt->address_information = (unsigned char*)*enc_vp;
		tmp_gt->address_information_length = enc_vp->get_size();
		// set odd/even flag
		tmp_gt->odd_num_of_addr_signals = ((vp->get_size() - 1) % 2 == 0 ? false : true);
	    }

	    break;
	}

	case sccp::GT_TRANSLATION_TYPE_ONLY:
	{
	    sccp::GT_TT* tmp_gt = (sccp::GT_TT*)cpa->global_title;
	    // tt
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, 2);
	    if(vp != NULL) tmp_gt->translation_type = (int)*vp;
	    // address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, 2);
	    if(vp != NULL && strlen((char*)*vp) > 0) {
		unsigned char tmp_bcd[64];
		int nb = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp, vp->get_size() - 1, tmp_bcd);
		// save encoded
		pmink_utils::VariantParam* enc_vp = pmpld->params.set_octets(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, tmp_bcd, nb, 3);
		tmp_gt->address_information = (unsigned char*)*enc_vp;
		tmp_gt->address_information_length = enc_vp->get_size();
	    }
	    break;
	}
	case sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING:
	{
	    sccp::GT_TTNPE* tmp_gt = (sccp::GT_TTNPE*)cpa->global_title;
	    // tt
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, 2);
	    if(vp != NULL) tmp_gt->translation_type = (int)*vp;
	    // np
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, 2);
	    if(vp != NULL) tmp_gt->numbering_plan = (sccp::NumberingPlan)(int)*vp;
	    // address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, 2);
	    if(vp != NULL && strlen((char*)*vp) > 0) {
		unsigned char tmp_bcd[64];
		int nb = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp, vp->get_size() - 1, tmp_bcd);
		// save encoded
		pmink_utils::VariantParam* enc_vp = pmpld->params.set_octets(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, tmp_bcd, nb, 3);
		tmp_gt->address_information = (unsigned char*)*enc_vp;
		tmp_gt->address_information_length = enc_vp->get_size();
		// set odd/even flag
		tmp_gt->encoding_scheme = ((vp->get_size() - 1) % 2 == 0 ? sccp::BCD_EVEN : sccp::BCD_ODD);
	    }

	    break;
	}
	case sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS:
	{
	    sccp::GT_TTNPENOA* tmp_gt = (sccp::GT_TTNPENOA*)cpa->global_title;
	    // tt
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, 2);
	    if(vp != NULL) tmp_gt->translation_type = (int)*vp;
	    // np
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, 2);
	    if(vp != NULL) tmp_gt->numbering_plan = (sccp::NumberingPlan)(int)*vp;
	    // nai
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, 2);
	    if(vp != NULL) tmp_gt->nature_of_address = (sccp::NatureOfAddress)(int)*vp;
	    // address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, 2);
	    if(vp != NULL && strlen((char*)*vp) > 0) {
		unsigned char tmp_bcd[64];
		int nb = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp, vp->get_size() - 1, tmp_bcd);
		// save encoded
		pmink_utils::VariantParam* enc_vp = pmpld->params.set_octets(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, tmp_bcd, nb, 3);
		tmp_gt->address_information = (unsigned char*)*enc_vp;
		tmp_gt->address_information_length = enc_vp->get_size();
		// set odd/even flag
		tmp_gt->encoding_scheme = ((vp->get_size() - 1) % 2 == 0 ? sccp::BCD_EVEN : sccp::BCD_ODD);
	    }

	    break;
	}
	default: break;
    }


}


template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::translate_called_pa(sccp::CalledPA* cpa, PminkPayload* pmpld, sccp::SCCPPool* _sccp_pool){
    // check for null called party
    if(cpa == NULL) return;
    // vars
    pmink_utils::VariantParam* vp = NULL;

    // routing indicator
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, 2);
    if(vp != NULL) cpa->routing_indicator = (sccp::RoutingIndicator)(int)*vp;

    // global title indicator
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, 2);
    if(vp != NULL) {
	cpa->global_title_indicator = (sccp::GlobalTitleIndicator)(int)*vp;
	// check if new global title type should be set
	if(cpa->global_title == NULL || cpa->global_title->type != cpa->global_title_indicator){
	    cpa->global_title = _sccp_pool->request_gt(cpa->global_title_indicator);
	}
    }

    // ssn
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, 2);
    if(vp != NULL) {
	cpa->ssn_indicator = true;
	cpa->subsystem_number = (sccp::SubsystemNumber)(int)*vp;
    }

    // point-code
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, 2);
    if(vp != NULL) {
	cpa->point_code_indicator = true;
	cpa->point_code = (int)*vp;
    }

    // gt translations
    // check GT type
    switch(cpa->global_title->type){
	case sccp::GT_NATURE_OF_ADDRESS_INDICATOR_ONLY:
	{
	    sccp::GT_NOA* tmp_gt = (sccp::GT_NOA*)cpa->global_title;
	    // nai
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, 2);
	    if(vp != NULL) tmp_gt->nature_of_address = (sccp::NatureOfAddress)(int)*vp;
	    // address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, 2);
	    if(vp != NULL && strlen((char*)*vp) > 0) {
		unsigned char tmp_bcd[64];
		int nb = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp, vp->get_size() - 1, tmp_bcd);
		// save encoded
		pmink_utils::VariantParam* enc_vp = pmpld->params.set_octets(asn1::ParameterType::_pt_sccp_called_pa_gt_address, tmp_bcd, nb, 3);
		tmp_gt->address_information = (unsigned char*)*enc_vp;
		tmp_gt->address_information_length = enc_vp->get_size();
		// set odd/even flag
		tmp_gt->odd_num_of_addr_signals = ((vp->get_size() - 1) % 2 == 0 ? false : true);
	    }

	    break;
	}

	case sccp::GT_TRANSLATION_TYPE_ONLY:
	{
	    sccp::GT_TT* tmp_gt = (sccp::GT_TT*)cpa->global_title;
	    // tt
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, 2);
	    if(vp != NULL) tmp_gt->translation_type = (int)*vp;
	    // address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, 2);
	    if(vp != NULL && strlen((char*)*vp) > 0) {
		unsigned char tmp_bcd[64];
		int nb = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp, vp->get_size() - 1, tmp_bcd);
		// save encoded
		pmink_utils::VariantParam* enc_vp = pmpld->params.set_octets(asn1::ParameterType::_pt_sccp_called_pa_gt_address, tmp_bcd, nb, 3);
		tmp_gt->address_information = (unsigned char*)*enc_vp;
		tmp_gt->address_information_length = enc_vp->get_size();
	    }
	    break;
	}
	case sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING:
	{
	    sccp::GT_TTNPE* tmp_gt = (sccp::GT_TTNPE*)cpa->global_title;
	    // tt
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, 2);
	    if(vp != NULL) tmp_gt->translation_type = (int)*vp;
	    // np
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, 2);
	    if(vp != NULL) tmp_gt->numbering_plan = (sccp::NumberingPlan)(int)*vp;
	    // address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, 2);
	    if(vp != NULL && strlen((char*)*vp) > 0) {
		unsigned char tmp_bcd[64];
		int nb = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp, vp->get_size() - 1, tmp_bcd);
		// save encoded
		pmink_utils::VariantParam* enc_vp = pmpld->params.set_octets(asn1::ParameterType::_pt_sccp_called_pa_gt_address, tmp_bcd, nb, 3);
		tmp_gt->address_information = (unsigned char*)*enc_vp;
		tmp_gt->address_information_length = enc_vp->get_size();
		// set odd/even flag
		tmp_gt->encoding_scheme = ((vp->get_size() - 1) % 2 == 0 ? sccp::BCD_EVEN : sccp::BCD_ODD);
	    }

	    break;
	}
	case sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS:
	{
	    sccp::GT_TTNPENOA* tmp_gt = (sccp::GT_TTNPENOA*)cpa->global_title;
	    // tt
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, 2);
	    if(vp != NULL) tmp_gt->translation_type = (int)*vp;
	    // np
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, 2);
	    if(vp != NULL) tmp_gt->numbering_plan = (sccp::NumberingPlan)(int)*vp;
	    // nai
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, 2);
	    if(vp != NULL) tmp_gt->nature_of_address = (sccp::NatureOfAddress)(int)*vp;
	    // address
	    vp = pmpld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, 2);
	    if(vp != NULL && strlen((char*)*vp) > 0) {
		unsigned char tmp_bcd[64];
		int nb = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp, vp->get_size() - 1, tmp_bcd);
		// save encoded
		pmink_utils::VariantParam* enc_vp = pmpld->params.set_octets(asn1::ParameterType::_pt_sccp_called_pa_gt_address, tmp_bcd, nb, 3);
		tmp_gt->address_information = (unsigned char*)*enc_vp;
		tmp_gt->address_information_length = enc_vp->get_size();
		// set odd/even flag
		tmp_gt->encoding_scheme = ((vp->get_size() - 1) % 2 == 0 ? sccp::BCD_EVEN : sccp::BCD_ODD);
	    }

	    break;
	}
	default: break;
    }


}

template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_called_pa(sccp::CalledPA* cpa, PminkPayload* pmpld){
    // check for null called party
    if(cpa == NULL) return;
    // routing indicator
    pmpld->params.set_int(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, cpa->routing_indicator);
    // global title indicator
    pmpld->params.set_int(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, cpa->global_title_indicator);
    // ssn indicator
    pmpld->params.set_bool(asn1::ParameterType::_pt_sccp_called_pa_ssn_indicator, cpa->ssn_indicator);
    // ssn (if present)
    if(cpa->ssn_indicator) pmpld->params.set_int(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, cpa->subsystem_number);
    // point code indicator
    pmpld->params.set_bool(asn1::ParameterType::_pt_sccp_called_pa_point_code_indicator, cpa->point_code_indicator);
    // point code (if present)
    if(cpa->point_code_indicator) pmpld->params.set_int(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, cpa->point_code);
}

template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_calling_pa(sccp::CallingPA* cpa, PminkPayload* pmpld){
    // check for null called party
    if(cpa == NULL) return;
    // routing indicator
    pmpld->params.set_int(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, cpa->routing_indicator);
    // global title indicator
    pmpld->params.set_int(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, cpa->global_title_indicator);
    // ssn indicator
    pmpld->params.set_bool(asn1::ParameterType::_pt_sccp_calling_pa_ssn_indicator, cpa->ssn_indicator);
    // ssn (if present)
    if(cpa->ssn_indicator) pmpld->params.set_int(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, cpa->subsystem_number);
    // point code indicator
    pmpld->params.set_bool(asn1::ParameterType::_pt_sccp_calling_pa_point_code_indicator, cpa->point_code_indicator);
    // point code (if present)
    if(cpa->point_code_indicator) pmpld->params.set_int(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, cpa->point_code);
}



template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_gt(sccp::GlobalTitle* _gt, PminkPayload* pmpld, uint32_t pcode){
    // check for null gt (in case of GT_NO_TITLE)
    if(_gt == NULL) return;
    // tmp buffer
    unsigned char tmp_str[256];
    // check gt type
    switch(_gt->type){
	case sccp::GT_NATURE_OF_ADDRESS_INDICATOR_ONLY:
	{
	    sccp::GT_NOA* gt = (sccp::GT_NOA*)_gt;
	    // nature of address
	    pmpld->params.set_int(pcode - 1, gt->nature_of_address);
	    // address digits
	    smstpdu::decode_tbcd_octets(gt->address_information, gt->address_information_length, tmp_str, sizeof(tmp_str), gt->odd_num_of_addr_signals);
	    pmpld->params.set_cstr(pcode, (char*)tmp_str);
	    break;
	}

	// national use, unknown encoding, specific
	case sccp::GT_TRANSLATION_TYPE_ONLY:
	    break;

	case sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING:
	{
	    sccp::GT_TTNPE* gt = (sccp::GT_TTNPE*)_gt;
	    // translation type
	    pmpld->params.set_int(pcode + 1, gt->translation_type);
	    // numbering plan
	    pmpld->params.set_int(pcode - 3, gt->numbering_plan);
	    // address digits
	    smstpdu::decode_tbcd_octets(gt->address_information, gt->address_information_length, tmp_str, sizeof(tmp_str), (gt->encoding_scheme == sccp::BCD_ODD ? true : false));
	    pmpld->params.set_cstr(pcode, (char*)tmp_str);
	    break;
	}

	case sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS:
	{
	    sccp::GT_TTNPENOA* gt = (sccp::GT_TTNPENOA*)_gt;
	    // translation type
	    pmpld->params.set_int(pcode + 1, gt->translation_type);
	    // numbering plan
	    pmpld->params.set_int(pcode - 3, gt->numbering_plan);
	    // nature of address
	    pmpld->params.set_int(pcode - 1, gt->nature_of_address);
	    // address digits
	    smstpdu::decode_tbcd_octets(gt->address_information, gt->address_information_length, tmp_str, sizeof(tmp_str), (gt->encoding_scheme == sccp::BCD_ODD ? true : false));
	    pmpld->params.set_cstr(pcode, (char*)tmp_str);
	    break;
	}

	// no GT
	case sccp::GT_NO_TITLE:
	    break;

    }

}

template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_sccp(unsigned char* buff, int size, PminkPayload* pmpld){
    // decode sccp message
    sccp::SCCPMessage* sccpm = sccp::decode(buff, size, &sccp_pool);
    // NULL check
    if(sccpm != NULL){
	// *** loop protection start ***
	if(loop_cmap != NULL){
	    uint32_t hash = pmink_utils::hash_fnv1a(buff, size);
	    loop_cmap->lock();
	    loop_cmap_it_t lit = loop_cmap->set(hash, 0);
	    uint32_t loop_count = ++lit->second.data;
	    loop_cmap->unlock();
	    // loop count
	    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_loop_count, loop_count);
	}
	// *** loop protection end ***

	// message type
	pmpld->params.set_int(asn1::ParameterType::_pt_sccp_message_type, sccpm->type);
	// sccp message
	switch(sccpm->type){
	    // UDT
	    case sccp::UDT_UNITDATA:
	    {
		// UDT pointer
		sccp::UDT_UnitData* udt = (sccp::UDT_UnitData*)sccpm;
		// process Called Party
		process_called_pa(udt->called_party_address, pmpld);
		// process Calling Party
		process_calling_pa(udt->calling_party_address, pmpld);
		// process Called GT
		process_gt(udt->called_party_address->global_title, pmpld, asn1::ParameterType::_pt_sccp_called_pa_gt_address);
		// process Calling GT
		process_gt(udt->calling_party_address->global_title, pmpld, asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
		// decode if active
		if(decoders[3]) process_tcap(udt->data->value, udt->data->value_length, pmpld);
		break;
	    }
	    // XUDT
	    case sccp::XUDT_EXTENDED_UNITDATA:
	    {
		// XUDT pointer
		sccp::XUDT_ExtendedUnitdata* xudt = (sccp::XUDT_ExtendedUnitdata*)sccpm;
		// process Called Party
		process_called_pa(xudt->called_party_address, pmpld);
		// process Calling Party
		process_calling_pa(xudt->calling_party_address, pmpld);
		// process Called GT
		process_gt(xudt->called_party_address->global_title, pmpld, asn1::ParameterType::_pt_sccp_called_pa_gt_address);
		// process Calling GT
		process_gt(xudt->calling_party_address->global_title, pmpld, asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
		// decode if active
		if(decoders[3]) process_tcap(xudt->data->value, xudt->data->value_length, pmpld);
		break;
	    }

	    // UDTS
	    case sccp::UDTS_UNITDATA_SERVICE:
	    {
		// UDTS pointer
		sccp::UDTS_UnitDataService* udts = (sccp::UDTS_UnitDataService*)sccpm;
		// process Called Party
		process_called_pa(udts->called_party_address, pmpld);
		// process Calling Party
		process_calling_pa(udts->calling_party_address, pmpld);
		// process Called GT
		process_gt(udts->called_party_address->global_title, pmpld, asn1::ParameterType::_pt_sccp_called_pa_gt_address);
		// process Calling GT
		process_gt(udts->calling_party_address->global_title, pmpld, asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
		// sccp return cause
		pmpld->params.set_int(asn1::ParameterType::_pt_sccp_return_cause, udts->return_cause->return_cause);
		// decode if active
		if(decoders[3]) process_tcap(udts->data->value, udts->data->value_length, pmpld);
		break;

	    }
	    // XUDTS
	    case sccp::XUDTS_EXTENDED_UNIDATA_SERVICE:
	    {
		// XUDTS pointer
		sccp::XUDTS_ExtendedUnitdataService* xudts = (sccp::XUDTS_ExtendedUnitdataService*)sccpm;
		// process Called Party
		process_called_pa(xudts->called_party_address, pmpld);
		// process Calling Party
		process_calling_pa(xudts->calling_party_address, pmpld);
		// process Called GT
		process_gt(xudts->called_party_address->global_title, pmpld, asn1::ParameterType::_pt_sccp_called_pa_gt_address);
		// process Calling GT
		process_gt(xudts->calling_party_address->global_title, pmpld, asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
		// sccp return cause
		pmpld->params.set_int(asn1::ParameterType::_pt_sccp_return_cause, xudts->return_cause->return_cause);
		// decode if active
		if(decoders[3]) process_tcap(xudts->data->value, xudts->data->value_length, pmpld);
		break;
	    }

	    // skip other types
	    default:
		break;
	}
    }else{
	// stats
	ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

    }
}

template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_m3ua(m3ua::ProtocolData* pdata, PminkPayload* pmpld){
    // null check
    if(pdata == NULL) return;
    // dpc
    pmpld->params.set_int(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, pdata->destination_point_code);
    // opc
    pmpld->params.set_int(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, pdata->originating_point_code);
    // si
    pmpld->params.set_int(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, pdata->service_indicator);
    // ni
    pmpld->params.set_int(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, pdata->network_indicator);
    // mp
    pmpld->params.set_int(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, pdata->message_priority);
    // sls
    pmpld->params.set_int(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, pdata->signalling_link_selection_code);
    // get as pointer
    ApplicationServer* tmp_as = as.get();
    // as label
    if(tmp_as != NULL) pmpld->params.set_cstr(asn1::ParameterType::_pt_m3ua_as_label, tmp_as->get_label());
    // asp label
    pmpld->params.set_cstr(asn1::ParameterType::_pt_m3ua_asp_label, ConnectionBase::label.c_str());

}


template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_smstpdu(unsigned char* buff,
						     int size,
						     PminkPayload* pmpld,
						     int component_index,
						     smstpdu::TPDUDirectionType tpdu_direction){

    // sanity check
    if(size <= 0) return;

    // try to detect tpdu direction
    smstpdu::TPDUType tpdu_type = smstpdu::TPDU::detect(buff[0], tpdu_direction);

    // tmp vars
    unsigned char tmp_str[256];
    uint16_t utf16_output[160];

    unsigned char unpack_buff[160];
    unsigned char* unpack_buff_p = unpack_buff;
    size_t unpack_buff_l = sizeof(unpack_buff);

    unsigned char gsm7_buff[256];
    unsigned char* gsm7_buff_p = gsm7_buff;
    size_t gsm7_buff_l = sizeof(gsm7_buff);



    icu::UnicodeString ucs;
    UErrorCode icu_err = U_ZERO_ERROR;
    int ud_offset = 0;
    unsigned char* ud_text_p = NULL;
    int ud_text_l = 0;
    int ud_udl = 0;

    // sms-submit
    if(tpdu_type == smstpdu::TPDUT_SMS_SUBMIT){
	// decode sms tpdu, stop on error
	if(sms_submit.init(buff, size) != 0) return;
	// sms text pointer
	ud_text_p = sms_submit.tp_ud.value;
	// sms udl
	ud_udl = sms_submit.tp_udl.udl;
	// sms data length in bytes
	ud_text_l = sms_submit.tp_ud.length;
	// tp-mti
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_mti, tpdu_type, component_index);
	// tp-rp
	pmpld->params.set_bool(asn1::ParameterType::_pt_smstpdu_tp_rp, sms_submit.tp_rp.rp_set, component_index);
	// tp-rd
	pmpld->params.set_bool(asn1::ParameterType::_pt_smstpdu_tp_rd, sms_submit.tp_rd.reject, component_index);
	// tp-vpf
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_vpf, sms_submit.tp_vpf.vpf_type, component_index);
	// tp-udhi
	pmpld->params.set_bool(asn1::ParameterType::_pt_smstpdu_tp_udhi, sms_submit.tp_udhi.udh_present, component_index);
	// tp-srr
	pmpld->params.set_bool(asn1::ParameterType::_pt_smstpdu_tp_srr, sms_submit.tp_srr.status_requested, component_index);
	// tp-mr
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_mr, sms_submit.tp_mr.reference_number, component_index);
	// tp-pid
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_pid, sms_submit.tp_pid.value[0], component_index);
	// tp-vp
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_vp, sms_submit.tp_vp.timestamp, component_index);


	// sms destination type of number and numbering plan
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number,
			      sms_submit.tp_da.type_of_number,
			      component_index);

	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan,
			      sms_submit.tp_da.numbering_plan,
			      component_index);



	// alphanumeric
	if(sms_submit.tp_da.type_of_number == smstpdu::TON_ALPHANUMERIC){
	    smstpdu::GsmAlphabet::decode(sms_submit.tp_da.address_value,
					 sms_submit.tp_da.length - 2,
					 sms_submit.tp_da.length_digits * 4 / 7,
					 //sms_submit.tp_da.length_digits / 2 * 8 / 7 ,
					 utf16_output,
					 sizeof(utf16_output));

	    // set unicode string
	    //ucs.setTo(utf8_output, sms_submit.tp_da.length_digits / 2 * 8 / 7);
	    ucs.setTo(utf16_output, sms_submit.tp_da.length_digits * 4 / 7);
	    // extract to utf-8 encoded c string
	    ucs.extract((char*)tmp_str, sizeof(tmp_str), conv_utf8, icu_err);
	    // error check
	    if(icu_err == U_ZERO_ERROR){
		//add param
		pmpld->params.set_cstr(asn1::ParameterType::_pt_smstpdu_tp_da_digits, (char*)tmp_str, component_index);

	    }

	// bcd encoded
	}else{
	    smstpdu::decode_tbcd_octets(sms_submit.tp_da.address_value, sms_submit.tp_da.length - 2, tmp_str, sizeof(tmp_str));
	    // add param
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smstpdu_tp_da_digits, (char*)tmp_str, component_index);

	}

	// data coding (skip the rest if not detected)
	if(sms_submit.tp_dcs == NULL) return;

	// data coding
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_dcs, sms_submit.tp_dcs->alphabet, component_index);

	// message class
	switch(sms_submit.tp_dcs->dcs_group){
	    case smstpdu::TPDCSGT_GENERAL:
	    case smstpdu::TPDCSGT_AUTOMATIC_DELETION_GROUP:
	    {
		// cast
		smstpdu::TP_DCSGeneral* dcs_general = (smstpdu::TP_DCSGeneral*)sms_submit.tp_dcs;
		// if message class defined
		if(dcs_general->has_message_class)
		    pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_message_class, dcs_general->message_class, component_index);

		break;
	    }

	    case smstpdu::TPDCSGT_DATA_CODING_MESSAGE:
	    {
		// cast
		smstpdu::TP_DCSDataCoding * dcs_data_coding = (smstpdu::TP_DCSDataCoding*)sms_submit.tp_dcs;
		// message class
		pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_message_class, dcs_data_coding->message_class, component_index);
		break;
	    }

	    default: break;

	}

	// udl
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_udl, sms_submit.tp_udl.udl, component_index);

	// check for udhi
	if(sms_submit.tp_udhi.udh_present){
	    // sms text pointer
	    ud_text_p += sms_submit.tp_ud.udh.length;
	    ud_text_l -= sms_submit.tp_ud.udh.length;

	    // tmp pointers
	    smstpdu::TP_UDHIEConc8BitRef* ref8bit;
	    smstpdu::TP_UDHIEConc16BitRef* ref16bit;

	    // calculate UD offset
	    // if 7bit, calculate septet offset
	    if(sms_submit.tp_dcs->alphabet == smstpdu::TPDCSAT_GSM7){
		//ud_offset = ceil((double)(sms_submit.tp_ud.udh.length * 8) / 7);
		ud_offset = 6 - (((sms_submit.tp_ud.udh.length - 1) * 8) % 7);
		// recalc udl
		ud_udl -= ((sms_submit.tp_ud.udh.length * 8) + ud_offset) / 7;

	    // else, offset is UDH length
	    }else ud_offset = sms_submit.tp_ud.udh.length;


	    // get ie list
	    std::vector<smstpdu::TP_UDHInformationElement*>* ie_lst = &sms_submit.tp_ud.udh.ie_lst;
	    smstpdu::TP_UDHInformationElement* tmp_ie;
	    // get id, parts and part
	    for(unsigned int i = 0; i<ie_lst->size(); i++){
		tmp_ie = (*ie_lst)[i];
		// check ie type
		switch(tmp_ie->ie_type){
		    // 8bit ref num
		    case smstpdu::TPUDHIET_CONCATENATED_8BIT_REF:
		    {
			ref8bit = ((smstpdu::TP_UDHIEConc8BitRef*)tmp_ie);
			// msg id
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_id,
					      ref8bit->msg_id,
					      component_index);
			// msg parts
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_parts,
					      ref8bit->msg_parts,
					      component_index);
			// msg part
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_part,
					      ref8bit->msg_part,
					      component_index);
			break;
		    }

		    // 16bit ref
		    case smstpdu::TPUDHIET_CONCATENATED_16BIT_REF:
		    {
			ref16bit = ((smstpdu::TP_UDHIEConc16BitRef*)tmp_ie);
			// msg id
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_id,
					      ref16bit->msg_id,
					      component_index);
			// msg parts
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_parts,
					      ref16bit->msg_parts,
					      component_index);
			// msg part
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_part,
					      ref16bit->msg_part,
					      component_index);
			break;
		    }

		    // skip other types
		    default:
			break;
		}
	    }
	}

	// sms text
	switch(sms_submit.tp_dcs->alphabet){
	    // 7bit gsm alphabet
	    case smstpdu::TPDCSAT_GSM7:
	    {
		/*
		smstpdu::GsmAlphabet::decode(sms_submit.tp_ud.value,
					     sms_submit.tp_ud.length,
					     sms_submit.tp_udl.udl,
					     utf8_output,
					     sizeof(utf8_output),
					     ud_offset);*/

		// unpack
		int unpack_l = smstpdu::GsmAlphabet::unpack(ud_offset, ud_text_l, sizeof(unpack_buff), ud_text_p, unpack_buff);
		// check if size valid
		if(unpack_l >= ud_udl){
		    // update in buffer size
		    unpack_buff_l = ud_udl;
		    // convert from gsm-7 to utf-8
		    size_t iconv_ret = libiconv(iconv_gsm7_to_utf8, (char**)&unpack_buff_p, &unpack_buff_l, (char**)&gsm7_buff_p, &gsm7_buff_l);
		    // error check
		    if(iconv_ret != (size_t)-1){
			// calc decoded size
			int tmp_size = sizeof(gsm7_buff) - gsm7_buff_l;
			//add param
			pmpld->params.set_octets(asn1::ParameterType::_pt_smstpdu_tp_ud, gsm7_buff, tmp_size, component_index);

		    }

		}
		// flush
		libiconv(iconv_gsm7_to_utf8, NULL, NULL, (char**)&gsm7_buff_p, &gsm7_buff_l);


		/*
		// set unicode string
		ucs.setTo(utf8_output, sms_submit.tp_udl.udl - ud_offset);
		// reset icu error
		icu_err = U_ZERO_ERROR;
		// extract to c string
		int tmp_size = ucs.extract((char*)tmp_str, sizeof(tmp_str), conv_utf8, icu_err);
		// error check
		if(icu_err == U_ZERO_ERROR){
		    //add param
		    pmpld->params.set_octets(asn1::ParameterType::_pt_smstpdu_tp_ud, (unsigned char*)tmp_str, tmp_size, component_index);
		}
		*/

		break;
	    }

	    // binary
	    case smstpdu::TPDCSAT_8BIT:
		pmpld->params.set_octets(asn1::ParameterType::_pt_smstpdu_tp_ud,
					 &sms_submit.tp_ud.value[ud_offset],
					 sms_submit.tp_ud.length - ud_offset,
					 component_index);
		break;

	    // ucs2
	    case smstpdu::TPDCSAT_UCS2:
	    {
		// ucs-2 str
		icu::UnicodeString ucs2((char*)&sms_submit.tp_ud.value[ud_offset],
					sms_submit.tp_udl.udl - ud_offset,
					"utf-16");
		// reset icu error
		icu_err = U_ZERO_ERROR;
		// extract to utf-8 encoded c string
		int tmp_size = ucs2.extract((char*)tmp_str, sizeof(tmp_str), conv_utf8, icu_err);
		// error check
		if(icu_err == U_ZERO_ERROR){
		    //add param
		    pmpld->params.set_octets(asn1::ParameterType::_pt_smstpdu_tp_ud, (unsigned char*)tmp_str, tmp_size, component_index);

		}

		break;
	    }

	    default: break;
	}


    // sms-deliver
    }else if(tpdu_type == smstpdu::TPDUT_SMS_DELIVER){
	// decode sms tpdu, stop on error
	if(sms_deliver.init(buff, size) != 0) return;
	// sms text pointer
	ud_text_p = sms_deliver.tp_ud.value;
	// sms udl
	ud_udl = sms_deliver.tp_udl.udl;
	// sms data length in bytes
	ud_text_l = sms_deliver.tp_ud.length;
	// tp-mti
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_mti, tpdu_type, component_index);
	// tp-rp
	pmpld->params.set_bool(asn1::ParameterType::_pt_smstpdu_tp_rp, sms_deliver.tp_rp.rp_set, component_index);
	// tp-udhi
	pmpld->params.set_bool(asn1::ParameterType::_pt_smstpdu_tp_udhi, sms_deliver.tp_udhi.udh_present, component_index);
	// tp-sri
	pmpld->params.set_bool(asn1::ParameterType::_pt_smstpdu_tp_sri, sms_deliver.tp_sri.status_report, component_index);
	// tp-mms
	pmpld->params.set_bool(asn1::ParameterType::_pt_smstpdu_tp_mms, sms_deliver.tp_mms.msg_waiting, component_index);
	// tp-pid
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_pid, sms_deliver.tp_pid.value[0], component_index);
	// tp-scts
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_scts, sms_deliver.tp_scts.timestamp, component_index);

	// sms originating type of number and numbering plan
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number,
			      sms_deliver.tp_oa.type_of_number,
			      component_index);

	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan,
			      sms_deliver.tp_oa.numbering_plan,
			      component_index);

	// sms originating
	// alphanumeric
	if(sms_deliver.tp_oa.type_of_number == smstpdu::TON_ALPHANUMERIC){
	    smstpdu::GsmAlphabet::decode(sms_deliver.tp_oa.address_value,
					 sms_deliver.tp_oa.length - 2,
					 sms_deliver.tp_oa.length_digits * 4 / 7,
					 //sms_deliver.tp_oa.length_digits / 2 * 8 / 7 ,
					 utf16_output,
					 sizeof(utf16_output));

	    // set unicode string
	    ucs.setTo(utf16_output, sms_deliver.tp_oa.length_digits * 4 / 7);
	    //ucs.setTo(utf8_output, sms_deliver.tp_oa.length_digits / 2 * 8 / 7);
	    // extract to utf-8 encoded c string
	    ucs.extract((char*)tmp_str, sizeof(tmp_str), conv_utf8, icu_err);
	    // error check
	    if(icu_err == U_ZERO_ERROR){
		//add param
		pmpld->params.set_cstr(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, (char*)tmp_str, component_index);

	    }

	// bcd encoded
	}else{
	    smstpdu::decode_tbcd_octets(sms_deliver.tp_oa.address_value, sms_deliver.tp_oa.length - 2, tmp_str, sizeof(tmp_str));
	    // add param
	    pmpld->params.set_cstr(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, (char*)tmp_str, component_index);

	}

	// data coding (skip the rest if not detected)
	if(sms_deliver.tp_dcs == NULL) return;

	// data coding
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_dcs, sms_deliver.tp_dcs->alphabet, component_index);

	// message class
	switch(sms_deliver.tp_dcs->dcs_group){
	    case smstpdu::TPDCSGT_GENERAL:
	    case smstpdu::TPDCSGT_AUTOMATIC_DELETION_GROUP:
	    {
		// cast
		smstpdu::TP_DCSGeneral* dcs_general = (smstpdu::TP_DCSGeneral*)sms_deliver.tp_dcs;
		// if message class defined
		if(dcs_general->has_message_class)
		    pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_message_class, dcs_general->message_class, component_index);

		break;
	    }

	    case smstpdu::TPDCSGT_DATA_CODING_MESSAGE:
	    {
		// cast
		smstpdu::TP_DCSDataCoding * dcs_data_coding = (smstpdu::TP_DCSDataCoding*)sms_deliver.tp_dcs;
		// message class
		pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_message_class, dcs_data_coding->message_class, component_index);
		break;
	    }

	    default: break;

	}

	// udl
	pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_tp_udl, sms_deliver.tp_udl.udl, component_index);

	// check for udhi
	if(sms_deliver.tp_udhi.udh_present){
	    // sms text pointer
	    ud_text_p += sms_deliver.tp_ud.udh.length;
	    ud_text_l -= sms_deliver.tp_ud.udh.length;

	    // tmp pointers
	    smstpdu::TP_UDHIEConc8BitRef* ref8bit;
	    smstpdu::TP_UDHIEConc16BitRef* ref16bit;
	    // calculate UD offset
	    // if 7bit, calculate septet offset
	    if(sms_deliver.tp_dcs->alphabet == smstpdu::TPDCSAT_GSM7){
		//ud_offset = ceil((double)(sms_deliver.tp_ud.udh.length * 8) / 7);
		ud_offset = 6 - (((sms_deliver.tp_ud.udh.length - 1) * 8) % 7);
		// recalc udl
		ud_udl -= ((sms_deliver.tp_ud.udh.length * 8) + ud_offset) / 7;

	    // else, offset is UDH length
	    }else ud_offset = sms_deliver.tp_ud.udh.length;


	    // get ie list
	    std::vector<smstpdu::TP_UDHInformationElement*>* ie_lst = &sms_deliver.tp_ud.udh.ie_lst;
	    smstpdu::TP_UDHInformationElement* tmp_ie;
	    // get id, parts and part
	    for(unsigned int i = 0; i<ie_lst->size(); i++){
		tmp_ie = (*ie_lst)[i];
		// check ie type
		switch(tmp_ie->ie_type){
		    // 8bit ref num
		    case smstpdu::TPUDHIET_CONCATENATED_8BIT_REF:
		    {
			ref8bit = ((smstpdu::TP_UDHIEConc8BitRef*)tmp_ie);
			// msg id
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_id,
					      ref8bit->msg_id,
					      component_index);
			// msg parts
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_parts,
					      ref8bit->msg_parts,
					      component_index);
			// msg part
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_part,
					      ref8bit->msg_part,
					      component_index);
			break;
		    }

		    // 16bit ref
		    case smstpdu::TPUDHIET_CONCATENATED_16BIT_REF:
		    {
			ref16bit = ((smstpdu::TP_UDHIEConc16BitRef*)tmp_ie);
			// msg id
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_id,
					      ref16bit->msg_id,
					      component_index);
			// msg parts
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_parts,
					      ref16bit->msg_parts,
					      component_index);
			// msg part
			pmpld->params.set_int(asn1::ParameterType::_pt_smstpdu_msg_part,
					      ref16bit->msg_part,
					      component_index);
			break;
		    }

		    // skip other types
		    default:
			break;
		}
	    }
	}



	// sms text
	switch(sms_deliver.tp_dcs->alphabet){
	    // 7bit gsm alphabet
	    case smstpdu::TPDCSAT_GSM7:
	    {
		/*
		smstpdu::GsmAlphabet::decode(sms_deliver.tp_ud.value,
					     sms_deliver.tp_ud.length,
					     sms_deliver.tp_udl.udl,
					     utf8_output,
					     sizeof(utf8_output),
					     ud_offset);*/

		// unpack
		int unpack_l = smstpdu::GsmAlphabet::unpack(ud_offset, ud_text_l, sizeof(unpack_buff), ud_text_p, unpack_buff);
		// check if size valid
		if(unpack_l >= ud_udl){
		    // update in buffer size
		    unpack_buff_l = ud_udl;
		    // convert from gsm-7 to utf-8
		    size_t iconv_ret = libiconv(iconv_gsm7_to_utf8, (char**)&unpack_buff_p, &unpack_buff_l, (char**)&gsm7_buff_p, &gsm7_buff_l);
		    // error check
		    if(iconv_ret != (size_t)-1){
			// calc decoded size
			int tmp_size = sizeof(gsm7_buff) - gsm7_buff_l;
			//add param
			pmpld->params.set_octets(asn1::ParameterType::_pt_smstpdu_tp_ud, gsm7_buff, tmp_size, component_index);

		    }

		}
		// flush
		libiconv(iconv_gsm7_to_utf8, NULL, NULL, (char**)&gsm7_buff_p, &gsm7_buff_l);



		/*
		// set unicode string
		ucs.setTo(utf8_output, sms_deliver.tp_udl.udl - ud_offset);
		// reset icu error
		icu_err = U_ZERO_ERROR;
		// extract to c string
		int tmp_size = ucs.extract((char*)tmp_str, sizeof(tmp_str), conv_utf8, icu_err);
		// error check
		if(icu_err == U_ZERO_ERROR){
		    //add param
		    pmpld->params.set_octets(asn1::ParameterType::_pt_smstpdu_tp_ud, (unsigned char*)tmp_str, tmp_size, component_index);

		}*/
		break;
	    }

	    // binary
	    case smstpdu::TPDCSAT_8BIT:
		pmpld->params.set_octets(asn1::ParameterType::_pt_smstpdu_tp_ud,
					 &sms_deliver.tp_ud.value[ud_offset],
					 sms_deliver.tp_ud.length - ud_offset,
					 component_index);
		break;

	    // ucs2
	    case smstpdu::TPDCSAT_UCS2:
	    {
		// ucs-2 str
		icu::UnicodeString ucs2((char*)&sms_deliver.tp_ud.value[ud_offset],
					sms_deliver.tp_udl.udl - ud_offset,
					"utf-16");

		// reset icu error
		icu_err = U_ZERO_ERROR;
		// extract to utf-8 encoded c string
		int tmp_size = ucs2.extract((char*)tmp_str, sizeof(tmp_str), conv_utf8, icu_err);
		// error check
		if(icu_err == U_ZERO_ERROR){
		    //add param
		    pmpld->params.set_octets(asn1::ParameterType::_pt_smstpdu_tp_ud, (unsigned char*)tmp_str, tmp_size, component_index);

		}
		break;
	    }

	    // skip other types
	    default:
		break;
	}



    }



}

template<typename PLD_TYPE>
bool sgn::_M3UAConnection<PLD_TYPE>::context_supported(const char* ctx_oid){
    // ok if not present
    if(ctx_oid == NULL) return true;
    // supported oids:
    // ===============
    // 0.4.0.0.1.0.25.3 - shortMsgMT-RelayContext-v3
    // 0.4.0.0.1.0.25.2 - shortMsgMT-RelayContext-v2

    // 0.4.0.0.1.0.20.3 - shortMsgGatewayContext-v3
    // 0.4.0.0.1.0.20.2 - shortMsgGatewayContext-v2
    // 0.4.0.0.1.0.20.1 - shortMsgGatewayContext-v1

    // 0.4.0.0.1.0.21.3 - shortMsgMO-RelayContext-v3
    // 0.4.0.0.1.0.21.2 - shortMsgMO-RelayContext-v2
    // 0.4.0.0.1.0.21.1 - shortMsgRelayContext-v1

    // check
    if(!(strcmp(ctx_oid, "0.4.0.0.1.0.25.3") == 0 ||
	 strcmp(ctx_oid, "0.4.0.0.1.0.25.2") == 0 ||
	 strcmp(ctx_oid, "0.4.0.0.1.0.20.3") == 0 ||
	 strcmp(ctx_oid, "0.4.0.0.1.0.20.2") == 0 ||
	 strcmp(ctx_oid, "0.4.0.0.1.0.20.1") == 0 ||
	 strcmp(ctx_oid, "0.4.0.0.1.0.21.3") == 0 ||
	 strcmp(ctx_oid, "0.4.0.0.1.0.21.2") == 0 ||
	 strcmp(ctx_oid, "0.4.0.0.1.0.21.1") == 0)) {
	// not supported
	return false;
    }
    // assume ok
    return true;
}


template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_gsmmap(unsigned char* buff,
						    int size,
						    PminkPayload* pmpld,
						    int component_type,
						    int opcode,
						    int component_index){
    // tmp buffer
    unsigned char tmp_str[256];
    smstpdu::TPDUDirectionType tpdu_direction;
    icu::UnicodeString ucs;

    // check opcode
    switch(opcode){
	// mt-forward sm
	case asn1::gsmmap::GSMMAPOperationLocalvalue::_mt_forwardSM:
	{

	    // set default direction
	    tpdu_direction = smstpdu::TPDUD_SC_MS;

	    // Invoke
	    if(component_type == 1){
		// next session id
		uint64_t tmp_session_id = asn1_session_id[2].get_next_id(&mt_fwd_sm);
		// reset BER nodes
		root_gsmmap_node.children.clear();
		root_gsmmap_node.tlv = NULL;
		// try to decode
		int res = asn1::decode(buff, size, &root_gsmmap_node, &mt_fwd_sm, &asn1_pool, &tmp_session_id);
		if(res == 0){
		    //asn1::print_structure(&root_gsmmap_node, 0);

		    // ***** check sm-RP-DA (IMSI or SCDA) *****
		    // check first for IMSI
		    if(mt_fwd_sm._sm_RP_DA->_imsi->has_linked_data(tmp_session_id)){
			smstpdu::decode_tbcd_octets(mt_fwd_sm._sm_RP_DA->_imsi->linked_node->tlv->value,
						    mt_fwd_sm._sm_RP_DA->_imsi->linked_node->tlv->value_length,
						    tmp_str,
						    sizeof(tmp_str));

			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_imsi, (char*)tmp_str, component_index);

		    // check for serviceCentreAddressDA
		    }else if(mt_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->has_linked_data(tmp_session_id)){
			//nature of address
			unsigned int tmp_i = mt_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->linked_node->tlv->value[0] & 0x70;
			switch(tmp_i){
			    case asn1::gsmmap::ASNAI_UNKNOWN:
			    case asn1::gsmmap::ASNAI_INTERNATIONAL:
			    case asn1::gsmmap::ASNAI_NATIONAL:
			    case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			    case asn1::gsmmap::ASNAI_SUBSCRIBER:
			    case asn1::gsmmap::ASNAI_ABBREVIATED:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_scda_type_of_number, tmp_i, component_index);
				break;

			}

			// numbering plan
			tmp_i = mt_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->linked_node->tlv->value[0] & 0x0f;
			switch(tmp_i){
			    case asn1::gsmmap::ASNP_UNKNOWN:
			    case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			    case asn1::gsmmap::ASNP_DATA_X121:
			    case asn1::gsmmap::ASNP_TELEX:
			    case asn1::gsmmap::ASNP_LAND_MOBILE:
			    case asn1::gsmmap::ASNP_NATIONAL:
			    case asn1::gsmmap::ASNP_PRIVATE:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, tmp_i, component_index);
				break;
			}

			// set direction
			tpdu_direction = smstpdu::TPDUD_MS_SC;
			// +1 (skip first byte (nature of address and numbering plan)
			smstpdu::decode_tbcd_octets(mt_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->linked_node->tlv->value + 1,
						    mt_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->linked_node->tlv->value_length - 1,
						    tmp_str,
						    sizeof(tmp_str));

			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_scda_digits, (char*)tmp_str, component_index);
		    }


		    // ***** check sm-RP-OA (MSISDN or SCOA) *****
		    if(mt_fwd_sm._sm_RP_OA->_msisdn->has_linked_data(tmp_session_id)){
			//nature of address
			unsigned int tmp_i = mt_fwd_sm._sm_RP_OA->_msisdn->linked_node->tlv->value[0] & 0x70;
			switch(tmp_i){
			    case asn1::gsmmap::ASNAI_UNKNOWN:
			    case asn1::gsmmap::ASNAI_INTERNATIONAL:
			    case asn1::gsmmap::ASNAI_NATIONAL:
			    case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			    case asn1::gsmmap::ASNAI_SUBSCRIBER:
			    case asn1::gsmmap::ASNAI_ABBREVIATED:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, tmp_i, component_index);
				break;

			}

			// numbering plan
			tmp_i = mt_fwd_sm._sm_RP_OA->_msisdn->linked_node->tlv->value[0] & 0x0f;
			switch(tmp_i){
			    case asn1::gsmmap::ASNP_UNKNOWN:
			    case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			    case asn1::gsmmap::ASNP_DATA_X121:
			    case asn1::gsmmap::ASNP_TELEX:
			    case asn1::gsmmap::ASNP_LAND_MOBILE:
			    case asn1::gsmmap::ASNP_NATIONAL:
			    case asn1::gsmmap::ASNP_PRIVATE:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, tmp_i, component_index);
				break;
			}

			// +1 (skip first byte (nature of address and numbering plan)
			smstpdu::decode_tbcd_octets(mt_fwd_sm._sm_RP_OA->_msisdn->linked_node->tlv->value + 1,
						    mt_fwd_sm._sm_RP_OA->_msisdn->linked_node->tlv->value_length - 1,
						    tmp_str,
						    sizeof(tmp_str));

			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_msisdn_digits, (char*)tmp_str, component_index);

		    }else if(mt_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->has_linked_data(tmp_session_id)){
			//nature of address
			unsigned int tmp_i = mt_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->linked_node->tlv->value[0] & 0x70;
			switch(tmp_i){
			    case asn1::gsmmap::ASNAI_UNKNOWN:
			    case asn1::gsmmap::ASNAI_INTERNATIONAL:
			    case asn1::gsmmap::ASNAI_NATIONAL:
			    case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			    case asn1::gsmmap::ASNAI_SUBSCRIBER:
			    case asn1::gsmmap::ASNAI_ABBREVIATED:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, tmp_i, component_index);
				break;

			}

			// numbering plan
			tmp_i = mt_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->linked_node->tlv->value[0] & 0x0f;
			switch(tmp_i){
			    case asn1::gsmmap::ASNP_UNKNOWN:
			    case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			    case asn1::gsmmap::ASNP_DATA_X121:
			    case asn1::gsmmap::ASNP_TELEX:
			    case asn1::gsmmap::ASNP_LAND_MOBILE:
			    case asn1::gsmmap::ASNP_NATIONAL:
			    case asn1::gsmmap::ASNP_PRIVATE:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, tmp_i, component_index);
				break;
			}

			// +1 (skip first byte (nature of address and numbering plan)
			smstpdu::decode_tbcd_octets(mt_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->linked_node->tlv->value + 1,
						    mt_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->linked_node->tlv->value_length - 1,
						    tmp_str,
						    sizeof(tmp_str));

			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_scoa_digits, (char*)tmp_str, component_index);


		    }

		    // ***** check sm-RP-UI *****
		    if(decoders[5]){
			process_smstpdu(mt_fwd_sm._sm_RP_UI->linked_node->tlv->value,
					mt_fwd_sm._sm_RP_UI->linked_node->tlv->value_length,
					pmpld,
					component_index,
					tpdu_direction);
		    }

		}else{
		    // stats
		    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

		}

	    }

	    break;
	}

	// mo-forward sm
	case asn1::gsmmap::GSMMAPOperationLocalvalue::_mo_forwardSM:
	{
	    // set default direction
	    tpdu_direction = smstpdu::TPDUD_MS_SC;

	    // Invoke
	    if(component_type == 1){

		// next session id
		uint64_t tmp_session_id = asn1_session_id[1].get_next_id(&mo_fwd_sm);
		// reset BER nodes
		root_gsmmap_node.children.clear();
		root_gsmmap_node.tlv = NULL;
		// try to decode
		int res = asn1::decode(buff, size, &root_gsmmap_node, &mo_fwd_sm, &asn1_pool, &tmp_session_id);
		if(res == 0){
		    //asn1::print_structure(&root_gsmmap_node, 0);

		    // ***** check sm-RP-DA (IMSI or SCDA) *****
		    // check first for IMSI
		    if(mo_fwd_sm._sm_RP_DA->_imsi->has_linked_data(tmp_session_id)){
			// set direction
			tpdu_direction = smstpdu::TPDUD_SC_MS;

			smstpdu::decode_tbcd_octets(mo_fwd_sm._sm_RP_DA->_imsi->linked_node->tlv->value,
						    mo_fwd_sm._sm_RP_DA->_imsi->linked_node->tlv->value_length,
						    tmp_str,
						    sizeof(tmp_str));

			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_imsi, (char*)tmp_str, component_index);

		    // check for serviceCentreAddressDA
		    }else if(mo_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->has_linked_data(tmp_session_id)){
			//nature of address
			unsigned int tmp_i = mo_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->linked_node->tlv->value[0] & 0x70;
			switch(tmp_i){
			    case asn1::gsmmap::ASNAI_UNKNOWN:
			    case asn1::gsmmap::ASNAI_INTERNATIONAL:
			    case asn1::gsmmap::ASNAI_NATIONAL:
			    case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			    case asn1::gsmmap::ASNAI_SUBSCRIBER:
			    case asn1::gsmmap::ASNAI_ABBREVIATED:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_scda_type_of_number, tmp_i, component_index);
				break;

			}

			// numbering plan
			tmp_i = mo_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->linked_node->tlv->value[0] & 0x0f;
			switch(tmp_i){
			    case asn1::gsmmap::ASNP_UNKNOWN:
			    case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			    case asn1::gsmmap::ASNP_DATA_X121:
			    case asn1::gsmmap::ASNP_TELEX:
			    case asn1::gsmmap::ASNP_LAND_MOBILE:
			    case asn1::gsmmap::ASNP_NATIONAL:
			    case asn1::gsmmap::ASNP_PRIVATE:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, tmp_i, component_index);
				break;
			}

			// +1 (skip first byte (nature of address and numbering plan)
			smstpdu::decode_tbcd_octets(mo_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->linked_node->tlv->value + 1,
						    mo_fwd_sm._sm_RP_DA->_serviceCentreAddressDA->linked_node->tlv->value_length - 1,
						    tmp_str,
						    sizeof(tmp_str));

			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_scda_digits, (char*)tmp_str, component_index);
		    }


		    // ***** check sm-RP-OA (MSISDN or SCOA) *****
		    if(mo_fwd_sm._sm_RP_OA->_msisdn->has_linked_data(tmp_session_id)){
			//nature of address
			unsigned int tmp_i = mo_fwd_sm._sm_RP_OA->_msisdn->linked_node->tlv->value[0] & 0x70;
			switch(tmp_i){
			    case asn1::gsmmap::ASNAI_UNKNOWN:
			    case asn1::gsmmap::ASNAI_INTERNATIONAL:
			    case asn1::gsmmap::ASNAI_NATIONAL:
			    case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			    case asn1::gsmmap::ASNAI_SUBSCRIBER:
			    case asn1::gsmmap::ASNAI_ABBREVIATED:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, tmp_i, component_index);
				break;

			}

			// numbering plan
			tmp_i = mo_fwd_sm._sm_RP_OA->_msisdn->linked_node->tlv->value[0] & 0x0f;
			switch(tmp_i){
			    case asn1::gsmmap::ASNP_UNKNOWN:
			    case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			    case asn1::gsmmap::ASNP_DATA_X121:
			    case asn1::gsmmap::ASNP_TELEX:
			    case asn1::gsmmap::ASNP_LAND_MOBILE:
			    case asn1::gsmmap::ASNP_NATIONAL:
			    case asn1::gsmmap::ASNP_PRIVATE:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, tmp_i, component_index);
				break;
			}


			// +1 (skip first byte (nature of address and numbering plan)
			smstpdu::decode_tbcd_octets(mo_fwd_sm._sm_RP_OA->_msisdn->linked_node->tlv->value + 1,
						    mo_fwd_sm._sm_RP_OA->_msisdn->linked_node->tlv->value_length - 1,
						    tmp_str,
						    sizeof(tmp_str));

			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_msisdn_digits, (char*)tmp_str, component_index);

		    }else if(mo_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->has_linked_data(tmp_session_id)){
			//nature of address
			unsigned int tmp_i = mo_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->linked_node->tlv->value[0] & 0x70;
			switch(tmp_i){
			    case asn1::gsmmap::ASNAI_UNKNOWN:
			    case asn1::gsmmap::ASNAI_INTERNATIONAL:
			    case asn1::gsmmap::ASNAI_NATIONAL:
			    case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			    case asn1::gsmmap::ASNAI_SUBSCRIBER:
			    case asn1::gsmmap::ASNAI_ABBREVIATED:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, tmp_i, component_index);
				break;

			}

			// numbering plan
			tmp_i = mo_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->linked_node->tlv->value[0] & 0x0f;
			switch(tmp_i){
			    case asn1::gsmmap::ASNP_UNKNOWN:
			    case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			    case asn1::gsmmap::ASNP_DATA_X121:
			    case asn1::gsmmap::ASNP_TELEX:
			    case asn1::gsmmap::ASNP_LAND_MOBILE:
			    case asn1::gsmmap::ASNP_NATIONAL:
			    case asn1::gsmmap::ASNP_PRIVATE:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, tmp_i, component_index);
				break;
			}

			// +1 (skip first byte (nature of address and numbering plan)
			smstpdu::decode_tbcd_octets(mo_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->linked_node->tlv->value + 1,
						    mo_fwd_sm._sm_RP_OA->_serviceCentreAddressOA->linked_node->tlv->value_length - 1,
						    tmp_str,
						    sizeof(tmp_str));

			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_scoa_digits, (char*)tmp_str, component_index);


		    }
		    // ***** check sm-RP-UI *****
		    if(decoders[5]){
			process_smstpdu(mo_fwd_sm._sm_RP_UI->linked_node->tlv->value,
					mo_fwd_sm._sm_RP_UI->linked_node->tlv->value_length,
					pmpld,
					component_index,
					tpdu_direction);
		    }



		    // ***** check IMSI *****
		    if(asn1::node_exists(mo_fwd_sm._imsi, tmp_session_id)){
			smstpdu::decode_tbcd_octets(mo_fwd_sm._imsi->linked_node->tlv->value,
						    mo_fwd_sm._imsi->linked_node->tlv->value_length,
						    tmp_str,
						    sizeof(tmp_str));

			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_imsi, (char*)tmp_str, component_index);

		    }

		}else{
		    // stats
		    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

		}
	    }
	    break;

	}

	// send routing info for SM
	case asn1::gsmmap::GSMMAPOperationLocalvalue::_sendRoutingInfoForSM:
	{
	    // Invoke (RoutingInfoForSM-Arg)
	    if(component_type == 1){
		// next session id
		uint64_t tmp_session_id = asn1_session_id[5].get_next_id(&sri_sm_arg);
		// reset BER nodes
		root_gsmmap_node.children.clear();
		root_gsmmap_node.tlv = NULL;
		// try to decode
		int res = asn1::decode(buff, size, &root_gsmmap_node, &sri_sm_arg, &asn1_pool, &tmp_session_id);
		if(res == 0){
		    //asn1::print_structure(&root_gsmmap_node, 0);
		    // *** MSISDN ***
		    //nature of address
		    unsigned int tmp_i = sri_sm_arg._msisdn->linked_node->tlv->value[0] & 0x70;
		    switch(tmp_i){
			case asn1::gsmmap::ASNAI_UNKNOWN:
			case asn1::gsmmap::ASNAI_INTERNATIONAL:
			case asn1::gsmmap::ASNAI_NATIONAL:
			case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			case asn1::gsmmap::ASNAI_SUBSCRIBER:
			case asn1::gsmmap::ASNAI_ABBREVIATED:
			    // add param
			    pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, tmp_i, component_index);
			    break;

		    }

		    // numbering plan
		    tmp_i = sri_sm_arg._msisdn->linked_node->tlv->value[0] & 0x0f;
		    switch(tmp_i){
			case asn1::gsmmap::ASNP_UNKNOWN:
			case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			case asn1::gsmmap::ASNP_DATA_X121:
			case asn1::gsmmap::ASNP_TELEX:
			case asn1::gsmmap::ASNP_LAND_MOBILE:
			case asn1::gsmmap::ASNP_NATIONAL:
			case asn1::gsmmap::ASNP_PRIVATE:
			    // add param
			    pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, tmp_i, component_index);
			    break;
		    }

		    smstpdu::decode_tbcd_octets(sri_sm_arg._msisdn->linked_node->tlv->value + 1,
						sri_sm_arg._msisdn->linked_node->tlv->value_length - 1,
						tmp_str,
						sizeof(tmp_str));

		    // add param
		    pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_msisdn_digits, (char*)tmp_str, component_index);

		    // *** Service Centre Address ***
		    //nature of address
		    tmp_i = sri_sm_arg._serviceCentreAddress->linked_node->tlv->value[0] & 0x70;
		    switch(tmp_i){
			case asn1::gsmmap::ASNAI_UNKNOWN:
			case asn1::gsmmap::ASNAI_INTERNATIONAL:
			case asn1::gsmmap::ASNAI_NATIONAL:
			case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			case asn1::gsmmap::ASNAI_SUBSCRIBER:
			case asn1::gsmmap::ASNAI_ABBREVIATED:
			    // add param
			    pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, tmp_i, component_index);
			    break;

		    }

		    // numbering plan
		    tmp_i = sri_sm_arg._serviceCentreAddress->linked_node->tlv->value[0] & 0x0f;
		    switch(tmp_i){
			case asn1::gsmmap::ASNP_UNKNOWN:
			case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			case asn1::gsmmap::ASNP_DATA_X121:
			case asn1::gsmmap::ASNP_TELEX:
			case asn1::gsmmap::ASNP_LAND_MOBILE:
			case asn1::gsmmap::ASNP_NATIONAL:
			case asn1::gsmmap::ASNP_PRIVATE:
			    // add param
			    pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, tmp_i, component_index);
			    break;
		    }

		    smstpdu::decode_tbcd_octets(sri_sm_arg._serviceCentreAddress->linked_node->tlv->value + 1,
						sri_sm_arg._serviceCentreAddress->linked_node->tlv->value_length - 1,
						tmp_str,
						sizeof(tmp_str));

		    // add param
		    pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_sca_digits, (char*)tmp_str, component_index);

		}else{
		    // stats
		    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

		}

	    // returnResultLast (RoutingInfoForSM-Res) or returnResultNostLast (RoutingInfoForSM-Res)
	    }else if(component_type == 2 || component_type == 7){
		// next session id
		uint64_t tmp_session_id = asn1_session_id[6].get_next_id(&sri_sm_res);
		// reset BER nodes
		root_gsmmap_node.children.clear();
		root_gsmmap_node.tlv = NULL;
		// try to decode
		int res = asn1::decode(buff, size, &root_gsmmap_node, &sri_sm_res, &asn1_pool, &tmp_session_id);
		if(res == 0){
		    //asn1::print_structure(&root_gsmmap_node, 0);
		    // *** IMSI ***
		    smstpdu::decode_tbcd_octets(sri_sm_res._imsi->linked_node->tlv->value,
						sri_sm_res._imsi->linked_node->tlv->value_length,
						tmp_str,
						sizeof(tmp_str));

		    // add param
		    pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_imsi, (char*)tmp_str, component_index);

		    // lwl pointer
		    asn1::LocationInfoWithLMSI* lwl = sri_sm_res._locationInfoWithLMSI;

		    // *** network node number ***
		    //nature of address
		    unsigned int tmp_i = lwl->_networkNode_Number->linked_node->tlv->value[0] & 0x70;
		    switch(tmp_i){
			case asn1::gsmmap::ASNAI_UNKNOWN:
			case asn1::gsmmap::ASNAI_INTERNATIONAL:
			case asn1::gsmmap::ASNAI_NATIONAL:
			case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			case asn1::gsmmap::ASNAI_SUBSCRIBER:
			case asn1::gsmmap::ASNAI_ABBREVIATED:
			    // add param
			    pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, tmp_i, component_index);
			    break;

		    }

		    // numbering plan
		    tmp_i = lwl->_networkNode_Number->linked_node->tlv->value[0] & 0x0f;
		    switch(tmp_i){
			case asn1::gsmmap::ASNP_UNKNOWN:
			case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			case asn1::gsmmap::ASNP_DATA_X121:
			case asn1::gsmmap::ASNP_TELEX:
			case asn1::gsmmap::ASNP_LAND_MOBILE:
			case asn1::gsmmap::ASNP_NATIONAL:
			case asn1::gsmmap::ASNP_PRIVATE:
			    // add param
			    pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, tmp_i, component_index);
			    break;
		    }

		    smstpdu::decode_tbcd_octets(lwl->_networkNode_Number->linked_node->tlv->value + 1,
						lwl->_networkNode_Number->linked_node->tlv->value_length - 1,
						tmp_str,
						sizeof(tmp_str));
		    // add param
		    pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_nnn_digits, (char*)tmp_str, component_index);


		    // *** additional number ***
		    if(asn1::node_exists(lwl->_additional_Number, tmp_session_id)){
			//nature of address
			tmp_i = lwl->_additional_Number->choice_selection->linked_node->tlv->value[0] & 0x70;
			switch(tmp_i){
			    case asn1::gsmmap::ASNAI_UNKNOWN:
			    case asn1::gsmmap::ASNAI_INTERNATIONAL:
			    case asn1::gsmmap::ASNAI_NATIONAL:
			    case asn1::gsmmap::ASNAI_NETWORK_SPECIFIC:
			    case asn1::gsmmap::ASNAI_SUBSCRIBER:
			    case asn1::gsmmap::ASNAI_ABBREVIATED:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_an_type_of_number, tmp_i, component_index);
				break;

			}

			// numbering plan
			tmp_i = lwl->_additional_Number->choice_selection->linked_node->tlv->value[0] & 0x0f;
			switch(tmp_i){
			    case asn1::gsmmap::ASNP_UNKNOWN:
			    case asn1::gsmmap::ASNP_ISDN_TELEPHONE:
			    case asn1::gsmmap::ASNP_DATA_X121:
			    case asn1::gsmmap::ASNP_TELEX:
			    case asn1::gsmmap::ASNP_LAND_MOBILE:
			    case asn1::gsmmap::ASNP_NATIONAL:
			    case asn1::gsmmap::ASNP_PRIVATE:
				// add param
				pmpld->params.set_int(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, tmp_i, component_index);
				break;
			}

			smstpdu::decode_tbcd_octets(lwl->_additional_Number->choice_selection->linked_node->tlv->value + 1,
						    lwl->_additional_Number->choice_selection->linked_node->tlv->value_length - 1,
						    tmp_str,
						    sizeof(tmp_str));
			// add param
			pmpld->params.set_cstr(asn1::ParameterType::_pt_gsmmap_an_digits, (char*)tmp_str, component_index);


		    }

		}else{
		    // stats
		    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

		}
	    }

	    break;
	}

	// skip other types
	default:
	    break;
    }

}



template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_tcap_components(asn1::itu_tcap::ComponentPortion* components, PminkPayload* pmpld, uint64_t sess_id){
    // component count
    int cc = 0;
    // check if componets exist
    if(asn1::node_exists(components, sess_id)){
	// component pointers
	asn1::itu_tcap::Component* component;
	asn1::itu_tcap::Invoke* invoke;
	asn1::itu_tcap::ReturnResult* return_result;
	asn1::itu_tcap::ReturnError* return_err;
	asn1::itu_tcap::Reject* reject;
	// opcode
	int opcode;
	int iid;
	int errc;
	int res;
	bool ctx_supported = true;;

	// validate dialogue context
	pmink_utils::VariantParam* vparam = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
	if(vparam != NULL && !context_supported((const char*)*vparam)) ctx_supported = false;

	// set param
	pmpld->params.set_bool(asn1::ParameterType::_pt_tcap_dialogue_context_supported, ctx_supported);


	// loop components
	for(unsigned int i = 0; i<components->children.size(); i++){
	    // component pointer
	    component = components->get_child(i);

	    // check session
	    if(!component->has_linked_data(sess_id)) continue;

	    // component count
	    ++cc;

	    // add component type param
	    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_component_type,
				  component->choice_selection->tlv->tag_value, i);

	    // check component type
	    switch(component->choice_selection->tlv->tag_value){
		// Invoke
		case 1:
		{
		    // invoke pointer
		    invoke = component->_invoke;

		    // get invoke id
		    iid = asn1::decode_int(invoke->_invokeID->linked_node->tlv->value,
					   invoke->_invokeID->linked_node->tlv->value_length);

		    // add invoke id param
		    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_component_invoke_id, iid, i);


		    // check for localValue
		    if(invoke->_opCode->_localValue->has_linked_data(sess_id)){
			// get opcode
			opcode = asn1::decode_int(invoke->_opCode->_localValue->linked_node->tlv->value,
						  invoke->_opCode->_localValue->linked_node->tlv->value_length);

			// add opcode param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_opcode, opcode, i);

			// process gsm map component
			if(decoders[4] && asn1::node_exists(invoke->_parameter, sess_id) && ctx_supported){
			    process_gsmmap(invoke->_parameter->linked_node->tlv->full_tlv,
					   invoke->_parameter->linked_node->tlv->full_tlv_length,
					   pmpld,
					   1,
					   opcode,
					   i);
			}

		    }


		    break;
		}

		// returnResultLast
		case 2:
		{
		    // return result pointer
		    return_result = component->_returnResultLast;

		    // get invoke id
		    iid = asn1::decode_int(return_result->_invokeID->linked_node->tlv->value,
					   return_result->_invokeID->linked_node->tlv->value_length);

		    // add invoke id param
		    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_component_invoke_id, iid, i);

		    // check if return res exists
		    if(asn1::node_exists(return_result->_resultretres, sess_id)){
			// check for localValue
			if(return_result->_resultretres->_opCode->_localValue->has_linked_data(sess_id)){
			    // get opcode
			    opcode = asn1::decode_int(return_result->_resultretres->_opCode->_localValue->linked_node->tlv->value,
						      return_result->_resultretres->_opCode->_localValue->linked_node->tlv->value_length);


			    // add opcode param
			    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_opcode, opcode, i);

			    // process gsm map component
			    if(decoders[4] && asn1::node_exists(return_result->_resultretres->_parameter, sess_id) && ctx_supported){
				process_gsmmap(return_result->_resultretres->_parameter->linked_node->tlv->full_tlv,
					       return_result->_resultretres->_parameter->linked_node->tlv->full_tlv_length,
					       pmpld,
					       2,
					       opcode,
					       i);
			    }


			}
		    }

		    break;
		}

		// returnError
		case 3:

		    // return error pointer
		    return_err = component->_returnError;

		    // get invoke id
		    iid = asn1::decode_int(return_err->_invokeID->linked_node->tlv->value,
					   return_err->_invokeID->linked_node->tlv->value_length);

		    // add invoke id param
		    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_component_invoke_id, iid, i);

		    // add error code (tcap national)
		    if(return_err->_errorCode->_nationaler->has_linked_data(sess_id)){
			// add error type param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 1, i);

			// get error code
			errc = asn1::decode_int(return_err->_errorCode->_nationaler->linked_node->tlv->value,
					        return_err->_errorCode->_nationaler->linked_node->tlv->value_length);

			// add error code param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_code, errc, i);

		    // add error code (tcap private)
		    }else if(return_err->_errorCode->_privateer->has_linked_data(sess_id)){
			// add error type param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 2, i);

			// get error code
			errc = asn1::decode_int(return_err->_errorCode->_privateer->linked_node->tlv->value,
					        return_err->_errorCode->_privateer->linked_node->tlv->value_length);

			// add error code param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_code, errc, i);

		    // decode as GSM MAP error code
		    }else{
			// add error type param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 10, i);
			// gsm mamp error code session id
			uint64_t tmp_session_id = asn1_session_id[3].get_next_id(&gsmmap_error_code);
			// reset BER nodes
			root_gsmmap_err_node.children.clear();
			root_gsmmap_err_node.tlv = NULL;

			// check for node
			if(asn1::node_exists(return_err->_parameter, sess_id)){
			    // try to decode
			    res = asn1::decode(return_err->_parameter->linked_node->tlv->full_tlv,
					       return_err->_parameter->linked_node->tlv->full_tlv_length,
					       &root_gsmmap_err_node,
					       &gsmmap_error_code,
					       &asn1_pool,
					       &tmp_session_id);
			    if(res == 0){
				// check for localValue
				if(gsmmap_error_code._localValue->has_linked_data(tmp_session_id)){
				    // get error code
				    errc = asn1::decode_int(gsmmap_error_code._localValue->linked_node->tlv->value,
							    gsmmap_error_code._localValue->linked_node->tlv->value_length);

				    // add error code param
				    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_code, errc, i);


				}
			    }else{
				// stats
				ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

			    }
			}
		    }

		    break;

		// reject
		case 4:
		    // reject pointer
		    reject = component->_reject;

		    // check for invoke id
		    if(reject->_invokeIDRej->_derivable->has_linked_data(sess_id)){
			// get invoke id
			iid = asn1::decode_int(reject->_invokeIDRej->_derivable->linked_node->tlv->value,
					       reject->_invokeIDRej->_derivable->linked_node->tlv->value_length);

			// add invoke id param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_component_invoke_id, iid, i);

		    }
		    // check error type
		    switch(reject->_problem->choice_selection->tlv->tag_value){
			// GeneralProblem
			case 0:
			    // add error type param
			    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 6, i);
			    break;

			// InvokeProblem
			case 1:
			    // add error type param
			    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 7, i);
			    break;

			// ReturnResultProblem
			case 2:
			    // add error type param
			    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 8, i);
			    break;

			// ReturnErrorProblem
			case 3:
			    // add error type param
			    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 9, i);
			    break;

		    }
		    // get error code
		    errc = asn1::decode_int(reject->_problem->choice_selection->linked_node->tlv->value,
					    reject->_problem->choice_selection->linked_node->tlv->value_length);

		    // add error code param
		    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_code, errc, i);
		    break;

		// returnResultNotLast
		case 7:
		    // return result pointer
		    return_result = component->_returnResultNotLast;

		    // get invoke id
		    iid = asn1::decode_int(return_result->_invokeID->linked_node->tlv->value,
					   return_result->_invokeID->linked_node->tlv->value_length);

		    // add invoke id param
		    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_component_invoke_id, iid, i);

		    // check if return res exists
		    if(asn1::node_exists(return_result->_resultretres, sess_id)){
			// check for localValue
			if(return_result->_resultretres->_opCode->_localValue->has_linked_data(sess_id)){
			    // get opcode
			    opcode = asn1::decode_int(return_result->_resultretres->_opCode->_localValue->linked_node->tlv->value,
						      return_result->_resultretres->_opCode->_localValue->linked_node->tlv->value_length);

			    // add opcode param
			    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_opcode, iid, i);

			    // process gsm map component
			    if(decoders[4] && asn1::node_exists(return_result->_resultretres->_parameter, sess_id) && ctx_supported){
				process_gsmmap(return_result->_resultretres->_parameter->linked_node->tlv->full_tlv,
					       return_result->_resultretres->_parameter->linked_node->tlv->full_tlv_length,
					       pmpld,
					       2,
					       opcode,
					       i);
			    }

			}
		    }

		    break;
	    }

	}


    }
    // add component count param
    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_component_count, cc);

}


template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_dialogue(asn1::DialoguePDU* dialogue, PminkPayload* pmpld, uint64_t sess_id){
    // check for dialogueRequest
    if(dialogue_pdu._dialogueRequest->has_linked_data(sess_id)){
	char tmp[50];
	pmink_utils::oid_decode(dialogue_pdu._dialogueRequest->_application_context_name->linked_node->tlv->value,
				dialogue_pdu._dialogueRequest->_application_context_name->linked_node->tlv->value_length,
				tmp);

	// add param
	pmpld->params.set_cstr(asn1::ParameterType::_pt_tcap_dialogue_context_oid, tmp);


    // check for dialogueResponse
    }else if(dialogue_pdu._dialogueResponse->has_linked_data(sess_id)){
	char tmp[50];
	pmink_utils::oid_decode(dialogue_pdu._dialogueResponse->_application_context_name->linked_node->tlv->value,
				dialogue_pdu._dialogueResponse->_application_context_name->linked_node->tlv->value_length,
				tmp);

	// add param
	pmpld->params.set_cstr(asn1::ParameterType::_pt_tcap_dialogue_context_oid, tmp);

	// check if error (0 = ok)
	if(dialogue_pdu._dialogueResponse->_result->linked_node->tlv->value[0] != 0){
	    // user error source
	    if(dialogue_pdu._dialogueResponse->_result_source_diagnostic->_dialogue_service_user->has_linked_data(sess_id)){
		// add error type param
		pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 4);
		// tmp node pointer
		asn1::Integer* tmp_node = dialogue_pdu._dialogueResponse->_result_source_diagnostic->_dialogue_service_user;
		// get error code
		int errc = asn1::decode_int(tmp_node->linked_node->tlv->value,
					    tmp_node->linked_node->tlv->value_length);


		// add error code param
		pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_code, errc);

	    // provider error source
	    }else{
		// add error type param
		pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 5);
		// tmp node pointer
		asn1::Integer* tmp_node = dialogue_pdu._dialogueResponse->_result_source_diagnostic->_dialogue_service_provider;
		// get error code
		int errc = asn1::decode_int(tmp_node->linked_node->tlv->value,
					    tmp_node->linked_node->tlv->value_length);


		// add error code param
		pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_code, errc);

	    }

	}



    // check for dialogueAbort
    }else if(dialogue_pdu._dialogueAbort->has_linked_data(sess_id)){
	// user error source (errc is error type in this case)
	int errc = asn1::decode_int(dialogue_pdu._dialogueAbort->_abort_source->linked_node->tlv->value,
				    dialogue_pdu._dialogueAbort->_abort_source->linked_node->tlv->value_length);

	// set error type
	if(errc == 0) pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 4);
	else if(errc == 1) pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 5);


    }


}


template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::process_tcap(unsigned char* buff, int size, PminkPayload* pmpld){
    // next session id
    uint64_t tmp_session_id = asn1_session_id[0].get_next_id(&tcm);
    // reset BER nodes
    root_tcap_node.children.clear();
    root_tcap_node.tlv = NULL;
    // try to detect tcap
    if(!(buff[0] == 0x62 || buff[0] == 0x64 || buff[0] == 0x65 || buff[0] == 0x67)) return;
    // try to decode
    int res = asn1::decode(buff, size, &root_tcap_node, &tcm, &asn1_pool, &tmp_session_id);
    if(res == 0){
	    //asn1::print_structure(&root_tcap_node, 0);
	    // choice selection sanity check
	    if(tcm.choice_selection != NULL){
		uint32_t tmp_id;
		int tcm_type = tcm.choice_selection->tlv->tag_value;
		// add tcm type
		pmpld->params.set_int(asn1::ParameterType::_pt_tcap_message_type, tcm_type);

		// check tcm type
		switch(tcm_type){
		    // TCAP Begin
		    case 2:
		    {
			// otid decode
			tmp_id = asn1::decode_int(tcm._begin->_otid->linked_node->tlv->value,
						  tcm._begin->_otid->linked_node->tlv->value_length);

			// add param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_source_transaction_id, tmp_id);
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_source_transaction_id_length,
					      tcm._begin->_otid->linked_node->tlv->value_length);

			// check for dialogue
			if(asn1::node_exists(tcm._begin->_dialoguePortion, tmp_session_id)){
			    // tcap dialogue session id
			    uint64_t tmp_dlg_session_id = asn1_session_id[4].get_next_id(&dialogue_pdu);
			    // reset BER nodes
			    root_tcap_dialogue_node.children.clear();
			    root_tcap_dialogue_node.tlv = NULL;
			    // get external pointer
			    asn1::External* external = tcm._begin->_dialoguePortion->_External_wrapper;
			    // check for data in current session
			    if(asn1::node_exists(external->_encoding->_single_ASN1_type, tmp_session_id)){
				// try to decode
				int res2 = asn1::decode(external->_encoding->_single_ASN1_type->linked_node->tlv->full_tlv,
							external->_encoding->_single_ASN1_type->linked_node->tlv->full_tlv_length,
							&root_tcap_dialogue_node,
							&dialogue_pdu,
							&asn1_pool,
							&tmp_dlg_session_id);
				// process dialogue
				if(res2 == 0) process_dialogue(&dialogue_pdu, pmpld, tmp_dlg_session_id);
				else{
				    // stats
				    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);


				}

			    }

			}

			// process component portion
			process_tcap_components(tcm._begin->_components, pmpld, tmp_session_id);
			break;
		    }

		    // TCAP End
		    case 4:
		    {

			// dtid decode
			tmp_id = asn1::decode_int(tcm._end->_dtid->linked_node->tlv->value,
						  tcm._end->_dtid->linked_node->tlv->value_length);


			// add param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_destination_transaction_id, tmp_id);
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_destination_transaction_id_length,
					      tcm._end->_dtid->linked_node->tlv->value_length);

			// check for dialogue
			if(asn1::node_exists(tcm._end->_dialoguePortion, tmp_session_id)){
			    // tcap dialogue session id
			    uint64_t tmp_dlg_session_id = asn1_session_id[4].get_next_id(&dialogue_pdu);
			    // reset BER nodes
			    root_tcap_dialogue_node.children.clear();
			    root_tcap_dialogue_node.tlv = NULL;
			    // get external pointer
			    asn1::External* external = tcm._end->_dialoguePortion->_External_wrapper;
			    // check for data in current session
			    if(asn1::node_exists(external->_encoding->_single_ASN1_type, tmp_session_id)){
				// try to decode
				int res2 = asn1::decode(external->_encoding->_single_ASN1_type->linked_node->tlv->full_tlv,
							external->_encoding->_single_ASN1_type->linked_node->tlv->full_tlv_length,
							&root_tcap_dialogue_node,
							&dialogue_pdu,
							&asn1_pool,
							&tmp_dlg_session_id);
				// process dialogue
				if(res2 == 0) process_dialogue(&dialogue_pdu, pmpld, tmp_dlg_session_id);
				else{
				    // stats
				    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

				}

			    }

			}

			// process component portion
			process_tcap_components(tcm._end->_components, pmpld, tmp_session_id);
			break;
		    }

		    // TCAP Continue
		    case 5:
		    {
			// dtid decode
			tmp_id = asn1::decode_int(tcm._continue->_dtid->linked_node->tlv->value,
						  tcm._continue->_dtid->linked_node->tlv->value_length);


			// add param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_destination_transaction_id, tmp_id);
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_destination_transaction_id_length,
					      tcm._continue->_dtid->linked_node->tlv->value_length);

			// otid decode
			tmp_id = asn1::decode_int(tcm._continue->_otid->linked_node->tlv->value,
						  tcm._continue->_otid->linked_node->tlv->value_length);
			// add param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_source_transaction_id, tmp_id);
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_destination_transaction_id_length,
					      tcm._continue->_otid->linked_node->tlv->value_length);

			// check for dialogue
			if(asn1::node_exists(tcm._continue->_dialoguePortion, tmp_session_id)){
			    // tcap dialogue session id
			    uint64_t tmp_dlg_session_id = asn1_session_id[4].get_next_id(&dialogue_pdu);
			    // reset BER nodes
			    root_tcap_dialogue_node.children.clear();
			    root_tcap_dialogue_node.tlv = NULL;
			    // get external pointer
			    asn1::External* external = tcm._continue->_dialoguePortion->_External_wrapper;
			    // check for data in current session
			    if(asn1::node_exists(external->_encoding->_single_ASN1_type, tmp_session_id)){
				// try to decode
				int res2 = asn1::decode(external->_encoding->_single_ASN1_type->linked_node->tlv->full_tlv,
							external->_encoding->_single_ASN1_type->linked_node->tlv->full_tlv_length,
							&root_tcap_dialogue_node,
							&dialogue_pdu,
							&asn1_pool,
							&tmp_dlg_session_id);
				// process dialogue
				if(res2 == 0) process_dialogue(&dialogue_pdu, pmpld, tmp_dlg_session_id);
				else{
				    // stats
				    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

				}

			    }

			}
			// process component portion
			process_tcap_components(tcm._continue->_components, pmpld, tmp_session_id);
			break;
		    }

		    // TCAP Abort
		    case 7:
		    {
			// dtid decode
			tmp_id = asn1::decode_int(tcm._abort->_dtid->linked_node->tlv->value,
						  tcm._abort->_dtid->linked_node->tlv->value_length);
			// add param
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_destination_transaction_id, tmp_id);
			pmpld->params.set_int(asn1::ParameterType::_pt_tcap_destination_transaction_id_length,
					      tcm._abort->_dtid->linked_node->tlv->value_length);

			// check for reason
			if(asn1::node_exists(tcm._abort->_reason, tmp_session_id)){
			    // p-abortCause
			    if(tcm._abort->_reason->_p_abortCause->has_linked_data(tmp_session_id)){
				// add error type param
				pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 3);

				// get error code
				int errc = asn1::decode_int(tcm._abort->_reason->_p_abortCause->linked_node->tlv->value,
							    tcm._abort->_reason->_p_abortCause->linked_node->tlv->value_length);

				// add error code param
				pmpld->params.set_int(asn1::ParameterType::_pt_tcap_error_code, errc);

			    // u-abortCause
			    }else{
				// check for dialogue part
				if(tcm._abort->_reason->_u_abortCause->_External_wrapper->has_linked_data(tmp_session_id)){
				    // tcap dialogue session id
				    uint64_t tmp_dlg_session_id = asn1_session_id[4].get_next_id(&dialogue_pdu);
				    // reset BER nodes
				    root_tcap_dialogue_node.children.clear();
				    root_tcap_dialogue_node.tlv = NULL;
				    // get external pointer
				    asn1::External* external = tcm._abort->_reason->_u_abortCause->_External_wrapper;
				    // check for data in current session
				    if(asn1::node_exists(external->_encoding->_single_ASN1_type, tmp_session_id)){
					// try to decode
					int res2 = asn1::decode(external->_encoding->_single_ASN1_type->linked_node->tlv->full_tlv,
								external->_encoding->_single_ASN1_type->linked_node->tlv->full_tlv_length,
								&root_tcap_dialogue_node,
								&dialogue_pdu,
								&asn1_pool,
								&tmp_dlg_session_id);
					if(res2 == 0){
					    //asn1::print_structure(&root_tcap_dialogue_node, 0);
					    // process dialogue
					    process_dialogue(&dialogue_pdu, pmpld, tmp_dlg_session_id);

					}else{
					    // stats
					    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

					}

				    }

				}

			    }
			}

			break;
		    }
		}

	    }


    }else{
	// stats
	ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

    }


}

template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::pmpld_notify(PminkPayload* _pmpld){

    // check for guid
    pmink_utils::VariantParam* vp_guid = _pmpld->params.get_param(asn1::ParameterType::_pt_pmink_guid);
    if(vp_guid == NULL) return;

    // check for source type
    pmink_utils::VariantParam* vp_src_type = _pmpld->params.get_param(asn1::ParameterType::_pt_pmink_daemon_type);
    if(vp_src_type == NULL) return;

    // check for source type
    pmink_utils::VariantParam* vp_src_id = _pmpld->params.get_param(asn1::ParameterType::_pt_pmink_daemon_id);
    if(vp_src_type == NULL) return;

    // check service id
    pmink_utils::VariantParam* vp_srvc_id = _pmpld->params.get_param(asn1::ParameterType::_pt_pmink_service_id);

    // get new pmink payload
    PminkPayload* pmpld = Connection<PLD_TYPE>::in_pool.allocate_constructed();
    if(pmpld == NULL){
	// stats
	ConnectionBase::stats.inc(CST_PMPLD_POOL_EMPTY, 1);
	return;
    }
    // clear params
    pmpld->params.clear_params();

    // set m3uap pointer
    m3ua::M3UAPacket* m3uap = &pmpld->m3ua_pld.m3uap;

    // connection type
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_connection_type, Connection<PLD_TYPE>::type);
    // guid
    pmpld->params.set_octets(asn1::ParameterType::_pt_pmink_guid, (unsigned char*)*vp_guid, vp_guid->get_size());
    // dest type
    pmpld->params.set_cstr(asn1::ParameterType::_pt_pmink_daemon_type, (char*)*vp_src_type);
    // dest id
    pmpld->params.set_cstr(asn1::ParameterType::_pt_pmink_daemon_id, (char*)*vp_src_id);
    // tcap sid
    pmpld->params.set(_pmpld->params,
		      pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_tcap_source_transaction_id),
		      pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_tcap_source_transaction_id));

    // tcap did
    pmpld->params.set(_pmpld->params,
		      pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_tcap_destination_transaction_id),
		      pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_tcap_destination_transaction_id));

    // get as pointer
    ApplicationServer* tmp_as = as.get();
    if(tmp_as != NULL) pmpld->params.set_cstr(asn1::ParameterType::_pt_m3ua_as_label, tmp_as->get_label());
    // asp label
    const char* tmp_label = ConnectionBase::label.c_str();
    pmpld->params.set_cstr(asn1::ParameterType::_pt_smpp_asp_label, tmp_label);
    // command id
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_command_id, asn1::SmsHubCommandId::_shci_corr_ntf);
    // trunk label
    pmpld->params.set(_pmpld->params,
		      pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_trunk_label, 2),
		      pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_trunk_label, 2));

    // packet data
    pmpld->params.set_octets(asn1::ParameterType::_pt_m3ua_header_data, _pmpld->raw, _pmpld->size);


    // *********************************************
    // ************ push to daemon *************
    // *********************************************
    // get new router if connection broken
    if(stp_r14pc == NULL || !stp_r14pc->is_registered()) stp_r14pc = r14ps->get_registered_client("routingd");
    // get first routing daemon
    r14p::R14PClient* r14pc = stp_r14pc;
    // null check
    if(r14pc == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // allocate new service message
    r14p::ServiceMessage* msg = r14psmm->new_smsg();
    // msg sanity check
    if(msg == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_MSG_POOL_EMPTY, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // service id
    msg->set_service_id(asn1::ServiceId::_sid_smshub_forward);
    // check for service id override
    if(vp_srvc_id != NULL) msg->set_service_id((int)*vp_srvc_id);
    // copy params
    msg->vpmap = pmpld->params;
    // free pmpld
    Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(msg) != 0){
	// free
	r14psmm->free_smsg(msg);
	ConnectionBase::stats.inc(CST_SMSG_PARAM_POOL_EMPTY, 1);
	return;

    }
    // inc refc counter for conn
    this->refc_inc();
    // set service message params
    msg->params.set_param(2, this);
    // send service message
    int res = r14psmm->send(msg, r14pc, (char*)*vp_src_type, (char*)*vp_src_id, true, &smsg_async_done);
    if(res != 0){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	r14psmm->free_smsg(msg);
	// dec refc counter for conn
	this->refc_dec();
    }


    // *********************************************
    // *********************************************

}


template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::pmpld_stp(PminkPayload* pmpld){
    // debug
    #ifdef PMDEBUG
    pmpld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== SENDING TO STP =====" << std::endl;
	std::cout << pmpld->params;
	std::cout << "=====================" << std::endl;
    )
    // *********************************************
    // ************ push to stp daemon *************
    // *********************************************
    // get new router if connection broken
    if(stp_r14pc == NULL || !stp_r14pc->is_registered()) stp_r14pc = r14ps->get_registered_client("routingd");
    // get first routing daemon
    r14p::R14PClient* r14pc = stp_r14pc;
    // null check
    if(r14pc == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // allocate new service message
    r14p::ServiceMessage* msg = r14psmm->new_smsg();
    // msg sanity check
    if(msg == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_MSG_POOL_EMPTY, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // service id
    msg->set_service_id(asn1::ServiceId::_sid_stp_routing);
    // copy params
    msg->vpmap = pmpld->params;
    // free pmpld
    Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(msg) != 0){
	// free
	r14psmm->free_smsg(msg);
	ConnectionBase::stats.inc(CST_SMSG_PARAM_POOL_EMPTY, 1);
	return;

    }
    // inc refc counter for conn
    this->refc_inc();
    // set service message params
    msg->params.set_param(2, this);
    // send service message
    int res = r14psmm->send(msg, r14pc, "stpd", NULL, true, &smsg_async_done);
    if(res != 0){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	r14psmm->free_smsg(msg);
	// dec refc counter for conn
	this->refc_dec();
    }


    // *********************************************
    // *********************************************

}

template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::pmpld_dr(PminkPayload* pmpld){
    // debug
    #ifdef PMDEBUG
    pmpld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== SENDING TO DR =====" << std::endl;
	std::cout << pmpld->params;
	std::cout << "=====================" << std::endl;
    )

    // *********************************************
    // ************ push to dr daemon *************
    // *********************************************

    // get new router if connection broken
    if(dr_r14pc == NULL || !dr_r14pc->is_registered()) dr_r14pc = r14ps->get_registered_client("routingd");
    // get first routing daemon
    r14p::R14PClient* r14pc = dr_r14pc;
    // null check
    if(r14pc == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // allocate new service message
    r14p::ServiceMessage* msg = r14psmm->new_smsg();
    // msg sanity check
    if(msg == NULL){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_MSG_POOL_EMPTY, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return;
    }
    // service id
    msg->set_service_id(asn1::ServiceId::_sid_sms_data_retention);
    // copy params
    msg->vpmap = pmpld->params;
    // free pmpld
    Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(msg) != 0){
	// free
	r14psmm->free_smsg(msg);
	ConnectionBase::stats.inc(CST_SMSG_PARAM_POOL_EMPTY, 1);
	return;

    }
    // inc refc counter for conn
    this->refc_inc();
    // set service message params
    msg->params.set_param(2, this);
    // send service message
    int res = r14psmm->send(msg, r14pc, "drd", NULL, true, &smsg_async_done);
    if(res != 0){
	// stats
	ConnectionBase::stats.inc(CST_SMSG_SEND_ERROR, 1);
	// free
	r14psmm->free_smsg(msg);
	// dec refc counter for conn
	this->refc_dec();
    }


    // *********************************************
    // *********************************************

}

template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::process_dr(PLD_TYPE* pld){
    // skip if dr is off
    if(!retain_data) return 0;

    // m3uap pointer
    m3ua::M3UAPacket* m3uap = &pld->m3uap;
    // m3ua DATA only
    if(m3uap->message->type != m3ua::DATA) return 1;
    // m3ua DATA
    m3ua::Data* data = (m3ua::Data*)m3uap->message;
    // m3ua ProtocolData parameter
    m3ua::ProtocolData* pdata = (m3ua::ProtocolData*)data->get_parameter(m3ua::M3UA_PT_PROTOCOL_DATA);
    // sanity check
    if(pdata == NULL) return 1;

    // get pmink payload
    PminkPayload* pmpld = Connection<PLD_TYPE>::in_pool.allocate_constructed();
    if(pmpld == NULL){
	// stats
	ConnectionBase::stats.inc(CST_PMPLD_POOL_EMPTY, 1);
	return 1;
    }
    // clear params
    pmpld->params.clear_params();
    // *** generate params ***
    // connection type
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_connection_type, Connection<PLD_TYPE>::type);
    // m3ua
    process_m3ua(pdata, pmpld);
    // timestamp
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_timestamp, ts.tv_sec);
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_timestamp_nsec, ts.tv_nsec);

    // decode if active and sccp si was set in m3ua data
    if(decoders[1] && pdata->service_indicator == m3ua::SCCP) process_sccp(pdata->user_protocol_data,
									   pdata->user_protocol_data_length,
									   pmpld);


    // forward pmpld params to r14p router daemon for data retention if enabled
    pmpld_dr(pmpld);
    // ok
    return 0;
}

template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::process_stp(PLD_TYPE* pld){
    // m3uap pointer
    m3ua::M3UAPacket* m3uap = &pld->m3uap;
    // m3ua DATA only
    if(m3uap->message->type != m3ua::DATA) return 1;
    // m3ua DATA
    m3ua::Data* data = (m3ua::Data*)m3uap->message;
    // m3ua ProtocolData parameter
    m3ua::ProtocolData* pdata = (m3ua::ProtocolData*)data->get_parameter(m3ua::M3UA_PT_PROTOCOL_DATA);
    // sanity check
    if(pdata == NULL) return 1;

    // get pmink payload
    PminkPayload* pmpld = Connection<PLD_TYPE>::in_pool.allocate_constructed();
    if(pmpld == NULL){
	// stats
	ConnectionBase::stats.inc(CST_PMPLD_POOL_EMPTY, 1);
	return 1;
    }
    // clear params
    pmpld->params.clear_params();
    // *** generate params ***
    // source daemon type
    pmpld->params.set_cstr(asn1::ParameterType::_pt_pmink_daemon_type, pmink::CURRENT_DAEMON->get_daemon_type());
    // source daemon id
    pmpld->params.set_cstr(asn1::ParameterType::_pt_pmink_daemon_id, pmink::CURRENT_DAEMON->get_daemon_id());
    // connection type
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_connection_type, Connection<PLD_TYPE>::type);
    // raw m3ua packet
    pmpld->params.set_octets(asn1::ParameterType::_pt_m3ua_header_data, pld->raw, pld->size);
    // m3ua
    process_m3ua(pdata, pmpld);
    // timestamp
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_timestamp, ts.tv_sec);
    pmpld->params.set_int(asn1::ParameterType::_pt_pmink_timestamp_nsec, ts.tv_nsec);

    // decode if active and sccp si was set in m3ua data
    if(decoders[1] && pdata->service_indicator == m3ua::SCCP) process_sccp(pdata->user_protocol_data,
									   pdata->user_protocol_data_length,
									   pmpld);


    // forward pmpld params to r14p router daemon for data retention if enabled
    pmpld_stp(pmpld);
    // ok
    return 0;
}

template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::proc_process_ext(PminkPayload* pmpld){
    // debug
    #ifdef PMDEBUG
    pmpld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== PROCESS/ROUTE R14P --> M3UA =====" << std::endl;
	std::cout << pmpld->params;
	std::cout << "=======================================" << std::endl;
    )

    // check for special pmink cmd
    pmink_utils::VariantParam* vp = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_command_id);
    // if special cmd found
    if(vp != NULL){
	// process special cmd
	if(cmd_process(pmpld, (int)*vp) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_CMD_PROCESS_ERROR, 1);
	    // free
	    Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	    return 1;

	// get raw packet data after conversion
	}else vp = pmpld->params.get_param(asn1::ParameterType::_pt_m3ua_header_data);

    // normal traffic
    }else{
	// get raw packet data
	vp = pmpld->params.get_param(asn1::ParameterType::_pt_m3ua_header_data);
	// missing m3ua header data
	if(vp == NULL){
	    // try conversion
	    if(convert(pmpld) != 0){
		// stats
		ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);
		// free
		Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
		return 1;

	    // get raw packet data after conversion
	    }else vp = pmpld->params.get_param(asn1::ParameterType::_pt_m3ua_header_data);

	}

    }


    // tmp buffer
    unsigned char tmp_buffer[1024];
    int sccp_enc_l = 0;


    // set m3uap pointer
    m3ua::M3UAPacket* m3uap = &pmpld->m3ua_pld.m3uap;
    // decode
    if(m3ua::decode((unsigned char*)*vp, vp->get_size(), m3uap, &m3ua_proc_pool) != 0){
	// stats
	ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return 2;
    }


    // check for m3ua DATA
    if(m3uap->message->type != m3ua::DATA){
	// stats
	ConnectionBase::stats.inc(CST_DISCARDED_MSU, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return 3;
    }

    // m3ua data pointer
    m3ua::Data* m3ua_data = (m3ua::Data*)m3uap->message;
    m3ua::ProtocolData* pdata = (m3ua::ProtocolData*)m3ua_data->get_parameter(m3ua::M3UA_PT_PROTOCOL_DATA);
    if(pdata == NULL){
	// stats
	ConnectionBase::stats.inc(CST_DISCARDED_MSU, 1);
	// free
	Connection<PLD_TYPE>::in_pool.deallocate_constructed(pmpld);
	return 4;
    }

    // *** check m3ua translations ***
    // opc
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, 2);
    if(vp != NULL) pdata->originating_point_code = (int)*vp; else pdata->originating_point_code = dpc;

    // dpc
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, 2);
    if(vp != NULL) pdata->destination_point_code = (int)*vp; else pdata->destination_point_code = opc;

    // si
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, 2);
    if(vp != NULL) pdata->service_indicator = (m3ua::ServiceIndicatorType)(int)*vp;

    // ni
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, 2);
    if(vp != NULL) pdata->network_indicator = (int)*vp;

    // mp
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, 2);
    if(vp != NULL) pdata->message_priority = (int)*vp;

    // sls
    vp = pmpld->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, 2);
    if(vp != NULL) pdata->signalling_link_selection_code = (int)*vp;

    // *** check sccp translations ***

    // decode sccp
    if(pdata->service_indicator == m3ua::SCCP){
	// decode sccp message
	sccp::SCCPMessage* sccpm = sccp::decode(pdata->user_protocol_data, pdata->user_protocol_data_length, &sccp_pool);
	// sanity check
	if(sccpm != NULL){
	    // sccp message
	    switch(sccpm->type){
		// UDT
		case sccp::UDT_UNITDATA:
		{
		    // UDT pointer
		    sccp::UDT_UnitData* udt = (sccp::UDT_UnitData*)sccpm;
		    // translate cgpa/cdpa
		    translate_calling_pa(udt->calling_party_address, pmpld, &sccp_pool);
		    translate_called_pa(udt->called_party_address, pmpld, &sccp_pool);
		    // encode
		    udt->encode(tmp_buffer, sizeof(tmp_buffer), 0, &sccp_enc_l);
		    break;
		}
		// XUDT
		case sccp::XUDT_EXTENDED_UNITDATA:
		{
		    // XUDT pointer
		    sccp::XUDT_ExtendedUnitdata* xudt = (sccp::XUDT_ExtendedUnitdata*)sccpm;
		    // translate cgpa/cdpa
		    translate_calling_pa(xudt->calling_party_address, pmpld, &sccp_pool);
		    translate_called_pa(xudt->called_party_address, pmpld, &sccp_pool);
		    // encode
		    xudt->encode(tmp_buffer, sizeof(tmp_buffer), 0, &sccp_enc_l);
		    break;
		}

		// UDTS
		case sccp::UDTS_UNITDATA_SERVICE:
		{
		    // UDTS pointer
		    sccp::UDTS_UnitDataService* udts = (sccp::UDTS_UnitDataService*)sccpm;
		    // translate cgpa/cdpa
		    translate_calling_pa(udts->calling_party_address, pmpld, &sccp_pool);
		    translate_called_pa(udts->called_party_address, pmpld, &sccp_pool);
		    // encode
		    udts->encode(tmp_buffer, sizeof(tmp_buffer), 0, &sccp_enc_l);
		    break;

		}
		// XUDTS
		case sccp::XUDTS_EXTENDED_UNIDATA_SERVICE:
		{
		    // XUDTS pointer
		    sccp::XUDTS_ExtendedUnitdataService* xudts = (sccp::XUDTS_ExtendedUnitdataService*)sccpm;
		    // translate cgpa/cdpa
		    translate_calling_pa(xudts->calling_party_address, pmpld, &sccp_pool);
		    translate_called_pa(xudts->called_party_address, pmpld, &sccp_pool);
		    // encode
		    xudts->encode(tmp_buffer, sizeof(tmp_buffer), 0, &sccp_enc_l);
		    break;
		}

		// skip other types
		default:
		    break;

	    }

	}

    }

    // check for re-encoded sccp
    if(sccp_enc_l > 0){
	pdata->user_protocol_data = tmp_buffer;
	pdata->user_protocol_data_length = sccp_enc_l;
    }



    // encode m3ua
    m3uap->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size);

    // check for correlation notification
    pmink_utils::VariantParam* vp_corr_ntf = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_correlation_notification);
    if(vp_corr_ntf != NULL && (bool)*vp_corr_ntf){
	pmpld_notify(pmpld);


    }



    // send
    pmpld->action = PAT_SEND;
    pmpld->m3ua_pld.ppid = sctp::M3UA;
    pmpld->m3ua_pld.sid = _M3UAConnection<PLD_TYPE>::last_sid.get();
    Connection<PLD_TYPE>::proc_tx_q.push(1, pmpld);
    return 0;
}


template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::proc_process(PLD_TYPE* pld){
    // set m3uap pointer
    m3ua::M3UAPacket* m3uap = &pld->m3uap;
    // decode
    if(m3ua::decode(pld->raw, pld->size, m3uap, &m3ua_proc_pool) != 0) return 1;
    // dr
    process_dr(pld);
    // routing
    process_stp(pld);
    // ok
    return 0;

}

template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::cmd_process(PminkPayload* pmpld, int cmd_id){
    switch(cmd_id){

	// generate sri-for-sm request
	// 0.4.0.0.1.0.20.3 - shortMsgGatewayContext-v3
	// 0.4.0.0.1.0.20.2 - shortMsgGatewayContext-v2
	// 0.4.0.0.1.0.20.1 - shortMsgGatewayContext-v1
	case asn1::SmsHubCommandId::_shci_sri_sm_req:
	{
	    // check mandatory
	    // msisdn
	    pmink_utils::VariantParam* vp_msisdn = pmpld->params.get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, 2);
	    if(vp_msisdn == NULL) return 20;
	    // sca
	    pmink_utils::VariantParam* vp_sca = pmpld->params.get_param(asn1::ParameterType::_pt_gsmmap_sca_digits, 2);
	    if(vp_sca == NULL) return 21;
	    // default version
	    char map_ver[17];
	    map_ver[0] = 0;

	    // init asn1 structures
	    asn1::reset(&tcm_enc);
	    asn1::reset(&dialogue_pdu_enc);
	    asn1::reset(&sri_sm_arg_enc);
	    // set optional parts
	    tcm_enc._begin->set_components();
	    tcm_enc._begin->_components->set_child(0);
	    tcm_enc._begin->_components->get_child(0)->_invoke->set_parameter();
	    // prepare optional parts
	    asn1::prepare(tcm_enc._begin->_components, tcm_enc._begin);
	    asn1::prepare(tcm_enc._begin->_components->get_child(0), tcm_enc._begin->_components);
	    asn1::prepare(tcm_enc._begin->_components->get_child(0)->_invoke->_parameter, tcm_enc._begin->_components->get_child(0)->_invoke);

	    // check for dialogue
	    bool tcap_dlg = false;
	    pmink_utils::VariantParam* vp_dlg_oid = pmpld->params.get_param(asn1::ParameterType::_pt_gsmmap_version, 2);
	    if(vp_dlg_oid != NULL && ((int)*vp_dlg_oid >= 2 && (int)*vp_dlg_oid <= 3)){
		tcm_enc._begin->set_dialoguePortion();
		asn1::prepare(tcm_enc._begin->_dialoguePortion, tcm_enc._begin);
		tcap_dlg = true;
		// map v2
		if((int)*vp_dlg_oid == 2){
		    strcpy(map_ver, "0.4.0.0.1.0.20.2");

		// map v3
		}else if((int)*vp_dlg_oid == 3){
		    strcpy(map_ver, "0.4.0.0.1.0.20.3");

		}

	    }

	    // GSM MAP part
	    unsigned char sri_sm_buff[1024];

	    unsigned char msisdn[64];
	    unsigned char sca[128];
	    unsigned char bool_true[1] = {1};
	    // msisdn
	    int msisdn_l = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp_msisdn, vp_msisdn->get_size() - 1, &msisdn[1]);
	    msisdn[0] = 0x80 | asn1::gsmmap::ASNAI_INTERNATIONAL | asn1::gsmmap::ASNP_ISDN_TELEPHONE;
	    sri_sm_arg_enc._msisdn->set_linked_data(1, msisdn, msisdn_l + 1);

	    // sm-rp-pri
	    sri_sm_arg_enc._sm_RP_PRI->set_linked_data(1, bool_true, 1);

	    // sca
	    int sca_l = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp_sca, vp_sca->get_size() - 1, &sca[1]);
	    sca[0] = 0x80 | asn1::gsmmap::ASNAI_INTERNATIONAL | asn1::gsmmap::ASNP_ISDN_TELEPHONE;
	    sri_sm_arg_enc._serviceCentreAddress->set_linked_data(1, sca, sca_l + 1);


	    int sri_sm_enc_l = asn1::encode(sri_sm_buff, sizeof(sri_sm_buff), &sri_sm_arg_enc, 1);



	    // TCAP part
	    unsigned char tcap_buff[1024];
	    unsigned char dlg_buff[1024];
	    unsigned char oid_data[64];
	    unsigned char oid_data2[64];
	    char oid_tmp_str[64];
	    char oid_tmp_str2[64];


	    // sid
	    uint32_t tmp_sid = rng_dstr(rng);;
	    unsigned int br;
	    // set tcap sid
	    pmpld->params.set_int(asn1::ParameterType::_pt_tcap_source_transaction_id, tmp_sid);

	    tmp_sid = asn1::prepare_int(tmp_sid, &br, -1);
	    tcm_enc._begin->_otid->set_linked_data(1, (unsigned char*)&tmp_sid, br);


	    // TCAP dialogue
	    if(tcap_dlg){
		// local OID copy
		strcpy(oid_tmp_str, map_ver);
		strcpy(oid_tmp_str2, "0.0.17.773.1.1.1");
		// encode oid
		int oid_enc_size = pmink_utils::oid_encode(oid_tmp_str, oid_data);
		int oid_enc_size2 = pmink_utils::oid_encode(oid_tmp_str2, oid_data2);
		dialogue_pdu_enc._dialogueRequest->_application_context_name->set_linked_data(1, oid_data, oid_enc_size);
		int dlg_enc_l = asn1::encode(dlg_buff, sizeof(dlg_buff), &dialogue_pdu_enc, 1);



		tcm_enc._begin->_dialoguePortion->_External_wrapper->_direct_reference->set_linked_data(1, oid_data2, oid_enc_size2);
		tcm_enc._begin->_dialoguePortion->_External_wrapper->_encoding->children[0]->tlv->override_auto_complexity = true;
		tcm_enc._begin->_dialoguePortion->_External_wrapper->_encoding->children[0]->set_linked_data(1, dlg_buff, dlg_enc_l);


	    }
	    // TCAP components
	    uint32_t tmp_iid = 1;
	    uint32_t opcode = asn1::gsmmap::GSMMAPOperationLocalvalue::_sendRoutingInfoForSM;
	    asn1::itu_tcap::Invoke* invoke = tcm_enc._begin->_components->get_child(0)->_invoke;
	    invoke->_invokeID->set_linked_data(1, (unsigned char*)&tmp_iid, 1);
	    invoke->_opCode->_localValue->set_linked_data(1, (unsigned char*)&opcode, 1);
	    invoke->_parameter->tlv->override_auto_complexity = true;
	    invoke->_parameter->tlv->complexity = sri_sm_arg_enc.tlv->complexity;
	    invoke->_parameter->tlv->tag_class = sri_sm_arg_enc.tlv->tag_class;
	    invoke->_parameter->tlv->tag_value = sri_sm_arg_enc.tlv->tag_value;
	    // skip T and L part of TLV, present in _paremeter already
	    invoke->_parameter->set_linked_data(1,
						&sri_sm_buff[1 + sri_sm_arg_enc.tlv->value_length_size],
						sri_sm_enc_l - 1 - sri_sm_arg_enc.tlv->value_length_size);

	    // encode and return number of bytes in buffer
	    int tcap_enc_l = asn1::encode(tcap_buff, sizeof(tcap_buff), &tcm_enc, 1);



	    // set m3uap pointer
	    m3ua::M3UAPacket* m3uap = &pmpld->m3ua_pld.m3uap;
	    m3uap->message_class = m3ua::TRANSFER;
	    m3uap->message_type = m3ua::DATA;
	    m3uap->message = m3ua_proc_pool.request_message(m3ua::DATA);
	    // DATA pointer
	    m3ua::Data* m3ua_data = (m3ua::Data*)m3uap->message;
	    m3ua_data->clear_params();
	    // sanity check
	    if(m3ua_data == NULL) return 3;
	    // protocol data param
	    m3ua::ProtocolData* pdata = (m3ua::ProtocolData*)m3ua_proc_pool.request_param(m3ua::M3UA_PT_PROTOCOL_DATA);
	    // sanity check
	    if(pdata == NULL) return 4;
	    // set pdata
	    pdata->service_indicator = m3ua::SCCP;
	    pdata->network_indicator = 0;
	    pdata->message_priority = 0;
	    pdata->signalling_link_selection_code = 0;
	    // add pdata to DATA
	    m3ua_data->add_parameter(pdata);

	    // SCCP part
	    sccp::UDT_UnitData* udt = (sccp::UDT_UnitData*)sccp_pool.request_message(sccp::UDT_UNITDATA);
	    // sanity check
	    if(udt == NULL) return 5;
	    // set UDT
	    udt->protocol_class = (sccp::ProtocolClass*)sccp_pool.request_param(sccp::PROTOCOL_CLASS);
	    // sanity
	    if(udt->protocol_class == NULL) return 6;
	    udt->called_party_address = (sccp::CalledPA*)sccp_pool.request_param(sccp::CALLED_PARTY_ADDRESS);
	    // sanity
	    if(udt->called_party_address == NULL) return 7;
	    udt->called_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	    // sanity
	    if(udt->called_party_address->global_title == NULL) return 8;
	    udt->calling_party_address = (sccp::CallingPA*)sccp_pool.request_param(sccp::CALLING_PARTY_ADDRESS);
	    // sanity
	    if(udt->calling_party_address == NULL) return 9;
	    udt->calling_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	    // sanity
	    if(udt->calling_party_address->global_title == NULL) return 10;
	    udt->data = (sccp::Data*)sccp_pool.request_param(sccp::DATA);
	    // sanity
	    if(udt->data == NULL) return 11;

	    unsigned char tmp_test[1] = {0};
	    unsigned char sccp_buff[1024];

	    // set params
	    udt->protocol_class->protocol_class = sccp::CLASS0;
	    udt->protocol_class->message_handling = sccp::RETURN_MESSAGE_ON_ERROR;
	    // called party
	    udt->called_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	    udt->called_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	    udt->called_party_address->ssn_indicator = true;
	    udt->called_party_address->point_code_indicator = false;
	    udt->called_party_address->subsystem_number = sccp::MSC;
	    // calling party
	    udt->calling_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	    udt->calling_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	    udt->calling_party_address->ssn_indicator = true;
	    udt->calling_party_address->point_code_indicator = false;
	    udt->calling_party_address->subsystem_number = sccp::MSC;

	    // called GT
	    sccp::GT_TTNPENOA* called_gt = (sccp::GT_TTNPENOA*)udt->called_party_address->global_title;
	    called_gt->translation_type = 0;
	    called_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	    called_gt->encoding_scheme = sccp::BCD_ODD;
	    called_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	    called_gt->address_information_length = 1;
	    called_gt->address_information = tmp_test;
	    // calling GT
	    sccp::GT_TTNPENOA* calling_gt = (sccp::GT_TTNPENOA*)udt->calling_party_address->global_title;
	    calling_gt->translation_type = 0;
	    calling_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	    calling_gt->encoding_scheme = sccp::BCD_ODD;
	    calling_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	    calling_gt->address_information_length = 1;
	    calling_gt->address_information = tmp_test;
	    // data
	    udt->data->value_length = tcap_enc_l;
	    udt->data->value = tcap_buff;

	    // encode sccp
	    int sccp_enc_l;
	    udt->encode(sccp_buff, sizeof(sccp_buff), 0, &sccp_enc_l);

	    // set pdata user data
	    pdata->user_protocol_data = sccp_buff;
	    pdata->user_protocol_data_length = sccp_enc_l;
	    // encode m3ua
	    m3uap->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size);
	    // set header data
	    pmpld->params.set_octets(asn1::ParameterType::_pt_m3ua_header_data, pmpld->raw, pmpld->size);

	    return 0;
	}

	// generate sri-for-sm ack
	case asn1::SmsHubCommandId::_shci_sri_sm_ack:
	{
	    // check mandatory
	    // imsi
	    pmink_utils::VariantParam* vp_imsi = pmpld->params.get_param(asn1::ParameterType::_pt_gsmmap_imsi, 2);
	    if(vp_imsi == NULL) return 20;
	    // nnn
	    pmink_utils::VariantParam* vp_nnn = pmpld->params.get_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, 2);
	    if(vp_nnn == NULL) return 21;
	    // used component
	    pmink_utils::VariantParam* vp_com_index = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_component_index);
	    if(vp_com_index == NULL) return 22;
	    // tcap sid
	    pmink_utils::VariantParam* vp_tcap_sid = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
	    if(vp_tcap_sid == NULL) return 23;
	    // tcap sid length
	    pmink_utils::VariantParam* vp_tcap_sid_l = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id_length);

	    // component invoke id
	    pmink_utils::VariantParam* vp_tcap_iid = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_component_invoke_id, (int)*vp_com_index);
	    if(vp_tcap_iid == NULL) return 24;

	    // init asn1 structures
	    asn1::reset(&tcm_enc);
	    asn1::reset(&dialogue_pdu_enc);
	    asn1::reset(&sri_sm_res_enc);
	    // set optional parts
	    tcm_enc._end->set_components();
	    tcm_enc._end->_components->set_child(0);
	    tcm_enc._end->_components->get_child(0)->_returnResultLast->set_resultretres();
	    tcm_enc._end->_components->get_child(0)->_returnResultLast->_resultretres->set_parameter();

	    // check for dialogue
	    bool tcap_dlg = false;
	    pmink_utils::VariantParam* vp_dlg_oid = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
	    if(vp_dlg_oid != NULL){
		tcm_enc._end->set_dialoguePortion();
		asn1::prepare(tcm_enc._end->_dialoguePortion, tcm_enc._end);
		tcap_dlg = true;

	    }

	    // prepare optional parts
	    asn1::prepare(tcm_enc._end->_components, tcm_enc._end);
	    asn1::prepare(tcm_enc._end->_components->get_child(0), tcm_enc._end->_components);
	    asn1::prepare(tcm_enc._end->_components->get_child(0)->_returnResultLast->_resultretres,
			  tcm_enc._end->_components->get_child(0)->_returnResultLast);
	    asn1::prepare(tcm_enc._end->_components->get_child(0)->_returnResultLast->_resultretres->_parameter,
			  tcm_enc._end->_components->get_child(0)->_returnResultLast->_resultretres);


	    // GSM MAP part
	    unsigned char sri_sm_buff[1024];
	    unsigned char imsi[64];
	    unsigned char nnn[128];

	    // imsi
	    int imsi_l = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp_imsi, vp_imsi->get_size() - 1, imsi);
	    sri_sm_res_enc._imsi->set_linked_data(1, imsi, imsi_l);

	    // nnn
	    int nnn_l = smstpdu::encode_tbcd_octets((unsigned char*)(char*)*vp_nnn, vp_nnn->get_size() - 1, &nnn[1]);
	    nnn[0] = 0x80 | asn1::gsmmap::ASNAI_INTERNATIONAL | asn1::gsmmap::ASNP_ISDN_TELEPHONE;
	    sri_sm_res_enc._locationInfoWithLMSI->_networkNode_Number->set_linked_data(1, nnn, nnn_l + 1);

	    // encode
	    int sri_sm_enc_l = asn1::encode(sri_sm_buff, sizeof(sri_sm_buff), &sri_sm_res_enc, 1);

	    // TCAP part
	    unsigned char tcap_buff[1024];
	    unsigned char dlg_buff[1024];
	    unsigned char oid_data[64];
	    unsigned char oid_data2[64];
	    char oid_tmp_str[64];
	    char oid_tmp_str2[64];

	    // get tcap sid
	    uint32_t tmp_sid = (int)*vp_tcap_sid;
	    unsigned int br;

	    tmp_sid = asn1::prepare_int(tmp_sid, &br, (vp_tcap_sid_l != NULL ? (int)*vp_tcap_sid_l : -1));
	    tcm_enc._end->_dtid->set_linked_data(1, (unsigned char*)&tmp_sid, br);



	    // TCAP dialogue
	    if(tcap_dlg){
		// local OID copy
		strncpy(oid_tmp_str, (char*)*vp_dlg_oid, sizeof(oid_tmp_str));
		strcpy(oid_tmp_str2, "0.0.17.773.1.1.1");
		// encode oid
		int oid_enc_size = pmink_utils::oid_encode(oid_tmp_str, oid_data);
		int oid_enc_size2 = pmink_utils::oid_encode(oid_tmp_str2, oid_data2);

		// app context
		dialogue_pdu_enc._dialogueResponse->_application_context_name->set_linked_data(1, oid_data, oid_enc_size);

		// version
		unsigned char pver[2] = {0x07, 0x80};
		dialogue_pdu_enc._dialogueResponse->_protocol_version->set_linked_data(1, pver, 2);

		// result
		int dlg_assoc_res = asn1::Associate_result::_accepted;
		dialogue_pdu_enc._dialogueResponse->_result->set_linked_data(1, (unsigned char*)&dlg_assoc_res, 1);

		// user diag
		int diag_res = 0;
		dialogue_pdu_enc._dialogueResponse->_result_source_diagnostic->_dialogue_service_user->set_linked_data(1, (unsigned char*)&diag_res, 1);

		// encode
		int dlg_enc_l = asn1::encode(dlg_buff, sizeof(dlg_buff), &dialogue_pdu_enc, 1);

		// diag ext wrapper
		tcm_enc._end->_dialoguePortion->_External_wrapper->_direct_reference->set_linked_data(1, oid_data2, oid_enc_size2);
		tcm_enc._end->_dialoguePortion->_External_wrapper->_encoding->children[0]->tlv->override_auto_complexity = true;
		tcm_enc._end->_dialoguePortion->_External_wrapper->_encoding->children[0]->set_linked_data(1, dlg_buff, dlg_enc_l);

	    }

	    // TCAP components
	    uint32_t tmp_iid = (int)*vp_tcap_iid;
	    uint32_t opcode = asn1::gsmmap::GSMMAPOperationLocalvalue::_sendRoutingInfoForSM;
	    asn1::itu_tcap::ReturnResult* result = tcm_enc._end->_components->get_child(0)->_returnResultLast;
	    result->_invokeID->set_linked_data(1, (unsigned char*)&tmp_iid, 1);
	    result->_resultretres->_opCode->_localValue->set_linked_data(1, (unsigned char*)&opcode, 1);
	    result->_resultretres->_parameter->tlv->override_auto_complexity = true;
	    result->_resultretres->_parameter->tlv->complexity = sri_sm_res_enc.tlv->complexity;
	    result->_resultretres->_parameter->tlv->tag_class = sri_sm_res_enc.tlv->tag_class;
	    result->_resultretres->_parameter->tlv->tag_value = sri_sm_res_enc.tlv->tag_value;
	    // skip T and L part of TLV, present in _paremeter already
	    result->_resultretres->_parameter->set_linked_data(1,
							       &sri_sm_buff[1 + sri_sm_res_enc.tlv->value_length_size],
							       sri_sm_enc_l - 1 - sri_sm_res_enc.tlv->value_length_size);

	    // encode and return number of bytes in buffer
	    int tcap_enc_l = asn1::encode(tcap_buff, sizeof(tcap_buff), &tcm_enc, 1);

	    // set m3uap pointer
	    m3ua::M3UAPacket* m3uap = &pmpld->m3ua_pld.m3uap;
	    m3uap->message_class = m3ua::TRANSFER;
	    m3uap->message_type = m3ua::DATA;
	    m3uap->message = m3ua_proc_pool.request_message(m3ua::DATA);
	    // DATA pointer
	    m3ua::Data* m3ua_data = (m3ua::Data*)m3uap->message;
	    m3ua_data->clear_params();
	    // sanity check
	    if(m3ua_data == NULL) return 3;
	    // protocol data param
	    m3ua::ProtocolData* pdata = (m3ua::ProtocolData*)m3ua_proc_pool.request_param(m3ua::M3UA_PT_PROTOCOL_DATA);
	    // sanity check
	    if(pdata == NULL) return 4;
	    // set pdata
	    pdata->service_indicator = m3ua::SCCP;
	    pdata->network_indicator = 0;
	    pdata->message_priority = 0;
	    pdata->signalling_link_selection_code = 0;
	    // add pdata to DATA
	    m3ua_data->add_parameter(pdata);

	    // SCCP part
	    sccp::UDT_UnitData* udt = (sccp::UDT_UnitData*)sccp_pool.request_message(sccp::UDT_UNITDATA);
	    // sanity check
	    if(udt == NULL) return 5;
	    // set UDT
	    udt->protocol_class = (sccp::ProtocolClass*)sccp_pool.request_param(sccp::PROTOCOL_CLASS);
	    // sanity
	    if(udt->protocol_class == NULL) return 6;
	    udt->called_party_address = (sccp::CalledPA*)sccp_pool.request_param(sccp::CALLED_PARTY_ADDRESS);
	    // sanity
	    if(udt->called_party_address == NULL) return 7;
	    udt->called_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	    // sanity
	    if(udt->called_party_address->global_title == NULL) return 8;
	    udt->calling_party_address = (sccp::CallingPA*)sccp_pool.request_param(sccp::CALLING_PARTY_ADDRESS);
	    // sanity
	    if(udt->calling_party_address == NULL) return 9;
	    udt->calling_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	    // sanity
	    if(udt->calling_party_address->global_title == NULL) return 10;
	    udt->data = (sccp::Data*)sccp_pool.request_param(sccp::DATA);
	    // sanity
	    if(udt->data == NULL) return 11;

	    unsigned char tmp_test[1] = {0};
	    unsigned char sccp_buff[1024];

	    // set params
	    udt->protocol_class->protocol_class = sccp::CLASS0;
	    udt->protocol_class->message_handling = sccp::RETURN_MESSAGE_ON_ERROR;
	    // called party
	    udt->called_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	    udt->called_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	    udt->called_party_address->ssn_indicator = true;
	    udt->called_party_address->point_code_indicator = false;
	    udt->called_party_address->subsystem_number = sccp::MSC;
	    // calling party
	    udt->calling_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	    udt->calling_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	    udt->calling_party_address->ssn_indicator = true;
	    udt->calling_party_address->point_code_indicator = false;
	    udt->calling_party_address->subsystem_number = sccp::MSC;

	    // called GT
	    sccp::GT_TTNPENOA* called_gt = (sccp::GT_TTNPENOA*)udt->called_party_address->global_title;
	    called_gt->translation_type = 0;
	    called_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	    called_gt->encoding_scheme = sccp::BCD_ODD;
	    called_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	    called_gt->address_information_length = 1;
	    called_gt->address_information = tmp_test;
	    // calling GT
	    sccp::GT_TTNPENOA* calling_gt = (sccp::GT_TTNPENOA*)udt->calling_party_address->global_title;
	    calling_gt->translation_type = 0;
	    calling_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	    calling_gt->encoding_scheme = sccp::BCD_ODD;
	    calling_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	    calling_gt->address_information_length = 1;
	    calling_gt->address_information = tmp_test;
	    // data
	    udt->data->value_length = tcap_enc_l;
	    udt->data->value = tcap_buff;

	    // encode sccp
	    int sccp_enc_l;
	    udt->encode(sccp_buff, sizeof(sccp_buff), 0, &sccp_enc_l);

	    // set pdata user data
	    pdata->user_protocol_data = sccp_buff;
	    pdata->user_protocol_data_length = sccp_enc_l;
	    // encode m3ua
	    m3uap->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size);
	    // set header data
	    pmpld->params.set_octets(asn1::ParameterType::_pt_m3ua_header_data, pmpld->raw, pmpld->size);
	    return 0;
	}
	// generate map-ack
	case asn1::SmsHubCommandId::_shci_sms_ack:
	{
	    // check mandatory
	    // used component
	    pmink_utils::VariantParam* vp_com_index = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_component_index);
	    if(vp_com_index == NULL) return 22;
	    // tcap sid
	    pmink_utils::VariantParam* vp_tcap_sid = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
	    if(vp_tcap_sid == NULL) return 23;
	    // tcap sid length
	    pmink_utils::VariantParam* vp_tcap_sid_l = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id_length);

	    // component invoke id
	    pmink_utils::VariantParam* vp_tcap_iid = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_component_invoke_id, (int)*vp_com_index);
	    if(vp_tcap_iid == NULL) return 24;

	    // error type
	    bool gen_err = false;
	    pmink_utils::VariantParam* vp_tcap_errt = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_error_type);
	    pmink_utils::VariantParam* vp_tcap_errc = NULL;
	    if(vp_tcap_errt != NULL){
		vp_tcap_errc = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_error_code);
		if(vp_tcap_errc == NULL) return 25;
		gen_err = true;

	    }



	    // init asn1 structures
	    asn1::reset(&tcm_enc);
	    asn1::reset(&dialogue_pdu_enc);
	    asn1::reset(&gsmmap_error_code_enc);
	    // set optional parts
	    tcm_enc._end->set_components();
	    tcm_enc._end->_components->set_child(0);
	    if(gen_err){
		tcm_enc._end->_components->get_child(0)->_returnError->set_parameter();
		asn1::reset(&gsmmap_error_code_enc);

	    }



	    // check for dialogue
	    bool tcap_dlg = false;
	    pmink_utils::VariantParam* vp_dlg_oid = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
	    if(vp_dlg_oid != NULL){
		tcm_enc._end->set_dialoguePortion();
		asn1::prepare(tcm_enc._end->_dialoguePortion, tcm_enc._end);
		tcap_dlg = true;

	    }

	    // ack
	    if(!gen_err){
		// prepare optional parts
		asn1::prepare(tcm_enc._end->_components, tcm_enc._end);
		asn1::prepare(tcm_enc._end->_components->get_child(0), tcm_enc._end->_components);
		asn1::prepare(tcm_enc._end->_components->get_child(0)->_returnResultLast->_resultretres,
			      tcm_enc._end->_components->get_child(0)->_returnResultLast);


	    //  err
	    }else{
		// prepare optional parts
		asn1::prepare(tcm_enc._end->_components, tcm_enc._end);
		asn1::prepare(tcm_enc._end->_components->get_child(0), tcm_enc._end->_components);
		asn1::prepare(tcm_enc._end->_components->get_child(0)->_returnError->_parameter,
			      tcm_enc._end->_components->get_child(0)->_returnError);

	    }



	    // TCAP part
	    unsigned char tcap_buff[1024];
	    unsigned char dlg_buff[1024];
	    unsigned char map_buff[1024];
	    unsigned char oid_data[64];
	    unsigned char oid_data2[64];
	    char oid_tmp_str[64];
	    char oid_tmp_str2[64];

	    // get tcap sid
	    uint32_t tmp_sid = (int)*vp_tcap_sid;
	    unsigned int br;
	    tmp_sid = asn1::prepare_int(tmp_sid, &br, (vp_tcap_sid_l != NULL ? (int)*vp_tcap_sid_l : -1));
	    tcm_enc._end->_dtid->set_linked_data(1, (unsigned char*)&tmp_sid, br);



	    // TCAP dialogue
	    if(tcap_dlg){
		// local OID copy
		strncpy(oid_tmp_str, (char*)*vp_dlg_oid, sizeof(oid_tmp_str));
		strcpy(oid_tmp_str2, "0.0.17.773.1.1.1");
		// encode oid
		int oid_enc_size = pmink_utils::oid_encode(oid_tmp_str, oid_data);
		int oid_enc_size2 = pmink_utils::oid_encode(oid_tmp_str2, oid_data2);

		// app context
		dialogue_pdu_enc._dialogueResponse->_application_context_name->set_linked_data(1, oid_data, oid_enc_size);

		// version
		unsigned char pver[2] = {0x07, 0x80};
		dialogue_pdu_enc._dialogueResponse->_protocol_version->set_linked_data(1, pver, 2);

		// result
		int dlg_assoc_res = asn1::Associate_result::_accepted;
		dialogue_pdu_enc._dialogueResponse->_result->set_linked_data(1, (unsigned char*)&dlg_assoc_res, 1);

		// user diag
		int diag_res = 0;
		dialogue_pdu_enc._dialogueResponse->_result_source_diagnostic->_dialogue_service_user->set_linked_data(1, (unsigned char*)&diag_res, 1);

		// encode
		int dlg_enc_l = asn1::encode(dlg_buff, sizeof(dlg_buff), &dialogue_pdu_enc, 1);

		// diag ext wrapper
		tcm_enc._end->_dialoguePortion->_External_wrapper->_direct_reference->set_linked_data(1, oid_data2, oid_enc_size2);
		tcm_enc._end->_dialoguePortion->_External_wrapper->_encoding->children[0]->tlv->override_auto_complexity = true;
		tcm_enc._end->_dialoguePortion->_External_wrapper->_encoding->children[0]->set_linked_data(1, dlg_buff, dlg_enc_l);

	    }
	    // invoke id
	    uint32_t tmp_iid = (int)*vp_tcap_iid;
	    int tcap_enc_l = 0;

	    // TCAP components
	    if(!gen_err){
		//uint32_t opcode = asn1::gsmmap::GSMMAPOperationLocalvalue::_sendRoutingInfoForSM;
		asn1::itu_tcap::ReturnResult* result = tcm_enc._end->_components->get_child(0)->_returnResultLast;
		result->_invokeID->set_linked_data(1, (unsigned char*)&tmp_iid, 1);

		// encode and return number of bytes in buffer
		tcap_enc_l = asn1::encode(tcap_buff, sizeof(tcap_buff), &tcm_enc, 1);

	    }else{

		// map error
		int err_code = (int)*vp_tcap_errc;
		gsmmap_error_code_enc._localValue->set_linked_data(1, (unsigned char*)&err_code, 1);
		// encode
		int map_enc_l = asn1::encode(map_buff, sizeof(map_buff), &gsmmap_error_code_enc, 1);

		// tcap part
		asn1::itu_tcap::ReturnError* rerr = tcm_enc._end->_components->get_child(0)->_returnError;
		rerr->_invokeID->set_linked_data(1, (unsigned char*)&tmp_iid, 1);


		rerr->_parameter->tlv->override_auto_complexity = true;
		rerr->_parameter->tlv->complexity = gsmmap_error_code_enc.choice_selection->tlv->complexity;
		rerr->_parameter->tlv->tag_class = gsmmap_error_code_enc.choice_selection->tlv->tag_class;
		rerr->_parameter->tlv->tag_value = gsmmap_error_code_enc.choice_selection->tlv->tag_value;
		rerr->_parameter->set_linked_data(1,
						  &map_buff[1 + gsmmap_error_code_enc.tlv->value_length_size],
						  map_enc_l - 1 - gsmmap_error_code_enc.tlv->value_length_size);

		// encode and return number of bytes in buffer
		tcap_enc_l = asn1::encode(tcap_buff, sizeof(tcap_buff), &tcm_enc, 1);


	    }



	    // set m3uap pointer
	    m3ua::M3UAPacket* m3uap = &pmpld->m3ua_pld.m3uap;
	    m3uap->message_class = m3ua::TRANSFER;
	    m3uap->message_type = m3ua::DATA;
	    m3uap->message = m3ua_proc_pool.request_message(m3ua::DATA);
	    // DATA pointer
	    m3ua::Data* m3ua_data = (m3ua::Data*)m3uap->message;
	    m3ua_data->clear_params();
	    // sanity check
	    if(m3ua_data == NULL) return 3;
	    // protocol data param
	    m3ua::ProtocolData* pdata = (m3ua::ProtocolData*)m3ua_proc_pool.request_param(m3ua::M3UA_PT_PROTOCOL_DATA);
	    // sanity check
	    if(pdata == NULL) return 4;
	    // set pdata
	    pdata->service_indicator = m3ua::SCCP;
	    pdata->network_indicator = 0;
	    pdata->message_priority = 0;
	    pdata->signalling_link_selection_code = 0;
	    // add pdata to DATA
	    m3ua_data->add_parameter(pdata);

	    // SCCP part
	    sccp::UDT_UnitData* udt = (sccp::UDT_UnitData*)sccp_pool.request_message(sccp::UDT_UNITDATA);
	    // sanity check
	    if(udt == NULL) return 5;
	    // set UDT
	    udt->protocol_class = (sccp::ProtocolClass*)sccp_pool.request_param(sccp::PROTOCOL_CLASS);
	    // sanity
	    if(udt->protocol_class == NULL) return 6;
	    udt->called_party_address = (sccp::CalledPA*)sccp_pool.request_param(sccp::CALLED_PARTY_ADDRESS);
	    // sanity
	    if(udt->called_party_address == NULL) return 7;
	    udt->called_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	    // sanity
	    if(udt->called_party_address->global_title == NULL) return 8;
	    udt->calling_party_address = (sccp::CallingPA*)sccp_pool.request_param(sccp::CALLING_PARTY_ADDRESS);
	    // sanity
	    if(udt->calling_party_address == NULL) return 9;
	    udt->calling_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	    // sanity
	    if(udt->calling_party_address->global_title == NULL) return 10;
	    udt->data = (sccp::Data*)sccp_pool.request_param(sccp::DATA);
	    // sanity
	    if(udt->data == NULL) return 11;

	    unsigned char tmp_test[1] = {0};
	    unsigned char sccp_buff[1024];

	    // set params
	    udt->protocol_class->protocol_class = sccp::CLASS0;
	    udt->protocol_class->message_handling = sccp::RETURN_MESSAGE_ON_ERROR;
	    // called party
	    udt->called_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	    udt->called_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	    udt->called_party_address->ssn_indicator = true;
	    udt->called_party_address->point_code_indicator = false;
	    udt->called_party_address->subsystem_number = sccp::MSC;
	    // calling party
	    udt->calling_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	    udt->calling_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	    udt->calling_party_address->ssn_indicator = true;
	    udt->calling_party_address->point_code_indicator = false;
	    udt->calling_party_address->subsystem_number = sccp::MSC;

	    // called GT
	    sccp::GT_TTNPENOA* called_gt = (sccp::GT_TTNPENOA*)udt->called_party_address->global_title;
	    called_gt->translation_type = 0;
	    called_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	    called_gt->encoding_scheme = sccp::BCD_ODD;
	    called_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	    called_gt->address_information_length = 1;
	    called_gt->address_information = tmp_test;
	    // calling GT
	    sccp::GT_TTNPENOA* calling_gt = (sccp::GT_TTNPENOA*)udt->calling_party_address->global_title;
	    calling_gt->translation_type = 0;
	    calling_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	    calling_gt->encoding_scheme = sccp::BCD_ODD;
	    calling_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	    calling_gt->address_information_length = 1;
	    calling_gt->address_information = tmp_test;
	    // data
	    udt->data->value_length = tcap_enc_l;
	    udt->data->value = tcap_buff;

	    // encode sccp
	    int sccp_enc_l;
	    udt->encode(sccp_buff, sizeof(sccp_buff), 0, &sccp_enc_l);

	    // set pdata user data
	    pdata->user_protocol_data = sccp_buff;
	    pdata->user_protocol_data_length = sccp_enc_l;
	    // encode m3ua
	    m3uap->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size);
	    // set header data
	    pmpld->params.set_octets(asn1::ParameterType::_pt_m3ua_header_data, pmpld->raw, pmpld->size);

	    return 0;
	}

	// generate tcap continue
	case asn1::SmsHubCommandId::_shci_tcap_continue:
	{
	    // check mandatory
	    // used component
	    //pmink_utils::VariantParam* vp_com_index = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_component_index);
	    //if(vp_com_index == NULL) return 22;
	    // tcap sid
	    pmink_utils::VariantParam* vp_tcap_sid = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
	    if(vp_tcap_sid == NULL) return 23;
	    // tcap sid length
	    pmink_utils::VariantParam* vp_tcap_sid_l = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id_length);

	    // component invoke id
	    //pmink_utils::VariantParam* vp_tcap_iid = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_component_invoke_id, (int)*vp_com_index);
	    //if(vp_tcap_iid == NULL) return 24;


	    // init asn1 structures
	    asn1::reset(&tcm_enc);
	    asn1::reset(&dialogue_pdu_enc);

	    // check for dialogue
	    bool tcap_dlg = false;
	    pmink_utils::VariantParam* vp_dlg_oid = pmpld->params.get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
	    if(vp_dlg_oid != NULL){
		tcm_enc._continue->set_dialoguePortion();
		asn1::prepare(tcm_enc._continue->_dialoguePortion, tcm_enc._continue);
		tcap_dlg = true;

	    }



	    // TCAP part
	    unsigned char tcap_buff[1024];
	    unsigned char dlg_buff[1024];
	    unsigned char oid_data[64];
	    unsigned char oid_data2[64];
	    char oid_tmp_str[64];
	    char oid_tmp_str2[64];


	    // get tcap did
	    uint32_t tmp_did = (int)*vp_tcap_sid;
	    unsigned int br;
	    tmp_did = asn1::prepare_int(tmp_did, &br, (vp_tcap_sid_l != NULL ? (int)*vp_tcap_sid_l : -1));
	    tcm_enc._continue->_dtid->set_linked_data(1, (unsigned char*)&tmp_did, br);
	    // generate new tcap sid
	    uint32_t tmp_sid = rng_dstr(rng);
	    tcm_enc._continue->_otid->set_linked_data(1, (unsigned char*)&tmp_sid, sizeof(tmp_sid));


	    // TCAP dialogue
	    if(tcap_dlg){
		// local OID copy
		strncpy(oid_tmp_str, (char*)*vp_dlg_oid, sizeof(oid_tmp_str));
		strcpy(oid_tmp_str2, "0.0.17.773.1.1.1");
		// encode oid
		int oid_enc_size = pmink_utils::oid_encode(oid_tmp_str, oid_data);
		int oid_enc_size2 = pmink_utils::oid_encode(oid_tmp_str2, oid_data2);

		// app context
		dialogue_pdu_enc._dialogueResponse->_application_context_name->set_linked_data(1, oid_data, oid_enc_size);

		// version
		unsigned char pver[2] = {0x07, 0x80};
		dialogue_pdu_enc._dialogueResponse->_protocol_version->set_linked_data(1, pver, 2);

		// result
		int dlg_assoc_res = asn1::Associate_result::_accepted;
		dialogue_pdu_enc._dialogueResponse->_result->set_linked_data(1, (unsigned char*)&dlg_assoc_res, 1);

		// user diag
		int diag_res = 0;
		dialogue_pdu_enc._dialogueResponse->_result_source_diagnostic->_dialogue_service_user->set_linked_data(1, (unsigned char*)&diag_res, 1);

		// encode
		int dlg_enc_l = asn1::encode(dlg_buff, sizeof(dlg_buff), &dialogue_pdu_enc, 1);

		// diag ext wrapper
		tcm_enc._continue->_dialoguePortion->_External_wrapper->_direct_reference->set_linked_data(1, oid_data2, oid_enc_size2);
		tcm_enc._continue->_dialoguePortion->_External_wrapper->_encoding->children[0]->tlv->override_auto_complexity = true;
		tcm_enc._continue->_dialoguePortion->_External_wrapper->_encoding->children[0]->set_linked_data(1, dlg_buff, dlg_enc_l);

	    }



	    // encode and return number of bytes in buffer
	    int tcap_enc_l = asn1::encode(tcap_buff, sizeof(tcap_buff), &tcm_enc, 1);



	    // set m3uap pointer
	    m3ua::M3UAPacket* m3uap = &pmpld->m3ua_pld.m3uap;
	    m3uap->message_class = m3ua::TRANSFER;
	    m3uap->message_type = m3ua::DATA;
	    m3uap->message = m3ua_proc_pool.request_message(m3ua::DATA);
	    // DATA pointer
	    m3ua::Data* m3ua_data = (m3ua::Data*)m3uap->message;
	    m3ua_data->clear_params();
	    // sanity check
	    if(m3ua_data == NULL) return 3;
	    // protocol data param
	    m3ua::ProtocolData* pdata = (m3ua::ProtocolData*)m3ua_proc_pool.request_param(m3ua::M3UA_PT_PROTOCOL_DATA);
	    // sanity check
	    if(pdata == NULL) return 4;
	    // set pdata
	    pdata->service_indicator = m3ua::SCCP;
	    pdata->network_indicator = 0;
	    pdata->message_priority = 0;
	    pdata->signalling_link_selection_code = 0;
	    // add pdata to DATA
	    m3ua_data->add_parameter(pdata);

	    // SCCP part
	    sccp::UDT_UnitData* udt = (sccp::UDT_UnitData*)sccp_pool.request_message(sccp::UDT_UNITDATA);
	    // sanity check
	    if(udt == NULL) return 5;
	    // set UDT
	    udt->protocol_class = (sccp::ProtocolClass*)sccp_pool.request_param(sccp::PROTOCOL_CLASS);
	    // sanity
	    if(udt->protocol_class == NULL) return 6;
	    udt->called_party_address = (sccp::CalledPA*)sccp_pool.request_param(sccp::CALLED_PARTY_ADDRESS);
	    // sanity
	    if(udt->called_party_address == NULL) return 7;
	    udt->called_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	    // sanity
	    if(udt->called_party_address->global_title == NULL) return 8;
	    udt->calling_party_address = (sccp::CallingPA*)sccp_pool.request_param(sccp::CALLING_PARTY_ADDRESS);
	    // sanity
	    if(udt->calling_party_address == NULL) return 9;
	    udt->calling_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	    // sanity
	    if(udt->calling_party_address->global_title == NULL) return 10;
	    udt->data = (sccp::Data*)sccp_pool.request_param(sccp::DATA);
	    // sanity
	    if(udt->data == NULL) return 11;

	    unsigned char tmp_test[1] = {0};
	    unsigned char sccp_buff[1024];

	    // set params
	    udt->protocol_class->protocol_class = sccp::CLASS0;
	    udt->protocol_class->message_handling = sccp::RETURN_MESSAGE_ON_ERROR;
	    // called party
	    udt->called_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	    udt->called_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	    udt->called_party_address->ssn_indicator = true;
	    udt->called_party_address->point_code_indicator = false;
	    udt->called_party_address->subsystem_number = sccp::MSC;
	    // calling party
	    udt->calling_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	    udt->calling_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	    udt->calling_party_address->ssn_indicator = true;
	    udt->calling_party_address->point_code_indicator = false;
	    udt->calling_party_address->subsystem_number = sccp::MSC;

	    // called GT
	    sccp::GT_TTNPENOA* called_gt = (sccp::GT_TTNPENOA*)udt->called_party_address->global_title;
	    called_gt->translation_type = 0;
	    called_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	    called_gt->encoding_scheme = sccp::BCD_ODD;
	    called_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	    called_gt->address_information_length = 1;
	    called_gt->address_information = tmp_test;
	    // calling GT
	    sccp::GT_TTNPENOA* calling_gt = (sccp::GT_TTNPENOA*)udt->calling_party_address->global_title;
	    calling_gt->translation_type = 0;
	    calling_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	    calling_gt->encoding_scheme = sccp::BCD_ODD;
	    calling_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	    calling_gt->address_information_length = 1;
	    calling_gt->address_information = tmp_test;
	    // data
	    udt->data->value_length = tcap_enc_l;
	    udt->data->value = tcap_buff;

	    // encode sccp
	    int sccp_enc_l;
	    udt->encode(sccp_buff, sizeof(sccp_buff), 0, &sccp_enc_l);

	    // set pdata user data
	    pdata->user_protocol_data = sccp_buff;
	    pdata->user_protocol_data_length = sccp_enc_l;
	    // encode m3ua
	    m3uap->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size);
	    // set header data
	    pmpld->params.set_octets(asn1::ParameterType::_pt_m3ua_header_data, pmpld->raw, pmpld->size);

	    return 0;
	}

	// unsupported
	default: return 0;
    }
    return 1;
}


template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::convert(PminkPayload* pmpld){
    /*
    // get connection type
    pmink_utils::VariantParam* vp = pmpld->params.get_param(asn1::ParameterType::_pt_pmink_connection_type);
    // sanity check
    if(vp == NULL) return 1;
    // get connection type
    ConnectionType ctype = (ConnectionType)(int)*vp;
    // check if converting SMPP -> M3UA
    if(ctype == CT_SMPP){
	// check for smpp header data
	vp = pmpld->params.get_param(asn1::ParameterType::_pt_smpp_header_data);
	// sanity check
	if(vp == NULL) return 2;

	// init asn1 structures
	asn1::reset(&tcm_enc);
	asn1::reset(&dialogue_pdu_enc);
	asn1::reset(&mt_fwd_sm_enc);
	// set optional parts
	tcm_enc._begin->set_dialoguePortion();
	tcm_enc._begin->set_components();
	tcm_enc._begin->_components->set_child(0);
	tcm_enc._begin->_components->get_child(0)->_invoke->set_parameter();
	// prepare optional parts
	asn1::prepare(tcm_enc._begin->_dialoguePortion, tcm_enc._begin);
	asn1::prepare(tcm_enc._begin->_components, tcm_enc._begin);
	asn1::prepare(tcm_enc._begin->_components->get_child(0), tcm_enc._begin->_components);
	asn1::prepare(tcm_enc._begin->_components->get_child(0)->_invoke->_parameter, tcm_enc._begin->_components->get_child(0)->_invoke);


	// SMS TPDU part
	unsigned char tp_oa[128];
	unsigned char tp_ud[128];
	unsigned char smstpdu_buff[1024];

	// first byte
	sms_deliver.tp_mti.mti_type = smstpdu::TPMTIT_SMS_DELIVER;
	sms_deliver.tp_mms.msg_waiting = false;
	sms_deliver.tp_sri.status_report = false;
	sms_deliver.tp_udhi.udh_present = true;
	sms_deliver.tp_rp.rp_set = false;

	// tp-oa
	int oa_l = smstpdu::encode_tbcd_octets((unsigned char*)"123456", 6, tp_oa);
	sms_deliver.tp_oa.length_digits = 6;
	sms_deliver.tp_oa.type_of_number = smstpdu::TON_INTERNATIONAL;
	sms_deliver.tp_oa.numbering_plan = smstpdu::NP_ISDN_TELEPHONE;
	sms_deliver.tp_oa.address_value = tp_oa;

	// tp-pid
	sms_deliver.tp_pid.format = smstpdu::TPPIDFT_DEFAULT1;
	sms_deliver.tp_pid.interworking = smstpdu::TPPIDIT_NO_INTERWORKING_BUT_SME_TO_SME;

	// tp-dcs
	sms_deliver.tp_dcs = sms_deliver.get_dcs_group(smstpdu::TPDCSGT_GENERAL);
	smstpdu::TP_DCSGeneral* dcs_group = (smstpdu::TP_DCSGeneral*)sms_deliver.tp_dcs;
	dcs_group->compressed = false;
	dcs_group->has_message_class = false;
	dcs_group->alphabet = smstpdu::TPDCSAT_GSM7;

	// tp-scts
	sms_deliver.tp_scts.timestamp = time(NULL);
	// tp-udl
	sms_deliver.tp_udl.udl = strlen("testing testing") + 7;
	// tp-ud
	int out_used = 0;
	const char test_gsm7[] = "testing testing";
	smstpdu::GsmAlphabet::encode(test_gsm7, sizeof(test_gsm7), (7 - (6 * 8 ) % 7), sizeof(tp_ud), tp_ud, &out_used);

	sms_deliver.tp_ud.value = tp_ud;
	sms_deliver.tp_ud.length = out_used;

	// udh
	sms_deliver.tp_ud.udh.ie_lst.clear();
	smstpdu::TP_UDHIEConc8BitRef* ie = (smstpdu::TP_UDHIEConc8BitRef*)sms_deliver.tp_ud.udh.ie_pool.request_ie(smstpdu::TPUDHIET_CONCATENATED_8BIT_REF);
	sms_deliver.tp_ud.udh.ie_lst.push_back(ie);
	ie->msg_id = 2;
	ie->msg_part = 2;
	ie->msg_parts = 3;


	int smstpdu_enc_l = 0;
	sms_deliver.encode(smstpdu_buff, sizeof(smstpdu_buff), 0, &smstpdu_enc_l);




	// GSM MAP part
	unsigned char mt_fwd_buff[1024];

	unsigned char imsi[64];
	unsigned char sca_oa[128];
	// sm-rp-DA
	int imsi_l = smstpdu::encode_tbcd_octets((unsigned char*)"293700200632750", 15, imsi);
	mt_fwd_sm_enc._sm_RP_DA->_imsi->set_linked_data(1, imsi, imsi_l);

	// sm-rp-OA
	int sca_oa_l = smstpdu::encode_tbcd_octets((unsigned char*)"38670007007", 11, &sca_oa[1]);
	sca_oa[0] = 0x80 | asn1::gsmmap::ASNAI_INTERNATIONAL | asn1::gsmmap::ASNP_ISDN_TELEPHONE;
	mt_fwd_sm_enc._sm_RP_OA->_serviceCentreAddressOA->set_linked_data(1, sca_oa, sca_oa_l + 1);
	// sm-rp-UI
	mt_fwd_sm_enc._sm_RP_UI->set_linked_data(1, smstpdu_buff, smstpdu_enc_l);

	int mt_fwd_enc_l = asn1::encode(mt_fwd_buff, sizeof(mt_fwd_buff), &mt_fwd_sm_enc, 1);





	// TCAP part
	unsigned char tcap_buff[1024];
	unsigned char dlg_buff[1024];
	unsigned char oid_data[64];
	unsigned char oid_data2[64];
	char oid_tmp_str[64];
	char oid_tmp_str2[64];


	// TCAP dialogue
	uint32_t tmp_sid = 1;
	// local OID copy
	strcpy(oid_tmp_str, "0.4.0.0.1.0.25.2");
	strcpy(oid_tmp_str2, "0.0.17.773.1.1.1");
	// encode oid
	int oid_enc_size = pmink_utils::oid_encode(oid_tmp_str, oid_data);
	int oid_enc_size2 = pmink_utils::oid_encode(oid_tmp_str2, oid_data2);

	tcm_enc._begin->_otid->set_linked_data(1, (unsigned char*)&tmp_sid, sizeof(uint32_t));
	dialogue_pdu_enc._dialogueRequest->_application_context_name->set_linked_data(1, oid_data, oid_enc_size);
	int dlg_enc_l = asn1::encode(dlg_buff, sizeof(dlg_buff), &dialogue_pdu_enc, 1);



	tcm_enc._begin->_dialoguePortion->_External_wrapper->_direct_reference->set_linked_data(1, oid_data2, oid_enc_size2);
	tcm_enc._begin->_dialoguePortion->_External_wrapper->_encoding->children[0]->tlv->override_auto_complexity = true;
	tcm_enc._begin->_dialoguePortion->_External_wrapper->_encoding->children[0]->set_linked_data(1, dlg_buff, dlg_enc_l);

	// TCAP components
	uint32_t tmp_iid = 1;
	uint32_t opcode = asn1::gsmmap::GSMMAPOperationLocalvalue::_mt_forwardSM;
	asn1::itu_tcap::Invoke* invoke = tcm_enc._begin->_components->get_child(0)->_invoke;
	invoke->_invokeID->set_linked_data(1, (unsigned char*)&tmp_iid, 1);
	invoke->_opCode->_localValue->set_linked_data(1, (unsigned char*)&opcode, 1);
	invoke->_parameter->tlv->override_auto_complexity = true;
	invoke->_parameter->tlv->complexity = mt_fwd_sm_enc.tlv->complexity;
	invoke->_parameter->tlv->tag_class = mt_fwd_sm_enc.tlv->tag_class;
	invoke->_parameter->tlv->tag_value = mt_fwd_sm_enc.tlv->tag_value;
	// skip T and L part of TLV, present in mt_fwd_buff value encoding
	invoke->_parameter->set_linked_data(1,
					    &mt_fwd_buff[1 + mt_fwd_sm_enc.tlv->value_length_size],
					    mt_fwd_enc_l - 1 - mt_fwd_sm_enc.tlv->value_length_size);

	// encode and return number of bytes in buffer
	int tcap_enc_l = asn1::encode(tcap_buff, sizeof(tcap_buff), &tcm_enc, 1);



	// set m3uap pointer
	m3ua::M3UAPacket* m3uap = &pmpld->m3ua_pld.m3uap;
	m3uap->message_class = m3ua::TRANSFER;
	m3uap->message_type = m3ua::DATA;
	m3uap->message = m3ua_proc_pool.request_message(m3ua::DATA);
	// DATA pointer
	m3ua::Data* m3ua_data = (m3ua::Data*)m3uap->message;
	// sanity check
	if(m3ua_data == NULL) return 3;
	// protocol data param
	m3ua::ProtocolData* pdata = (m3ua::ProtocolData*)m3ua_proc_pool.request_param(m3ua::M3UA_PT_PROTOCOL_DATA);
	// sanity check
	if(pdata == NULL) return 4;
	// set pdata
	pdata->service_indicator = m3ua::SCCP;
	pdata->network_indicator = 0;
	pdata->message_priority = 0;
	pdata->signalling_link_selection_code = 0;
	// add pdata to DATA
	m3ua_data->add_parameter(pdata);

	// SCCP part
	sccp::UDT_UnitData* udt = (sccp::UDT_UnitData*)sccp_pool.request_message(sccp::UDT_UNITDATA);
	// sanity check
	if(udt == NULL) return 5;
	// set UDT
	udt->protocol_class = (sccp::ProtocolClass*)sccp_pool.request_param(sccp::PROTOCOL_CLASS);
	// sanity
	if(udt->protocol_class == NULL) return 6;
	udt->called_party_address = (sccp::CalledPA*)sccp_pool.request_param(sccp::CALLED_PARTY_ADDRESS);
	// sanity
	if(udt->called_party_address == NULL) return 7;
	udt->called_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	// sanity
	if(udt->called_party_address->global_title == NULL) return 8;
	udt->calling_party_address = (sccp::CallingPA*)sccp_pool.request_param(sccp::CALLING_PARTY_ADDRESS);
	// sanity
	if(udt->calling_party_address == NULL) return 9;
	udt->calling_party_address->global_title = (sccp::GT_TTNPENOA*)sccp_pool.request_gt(sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
	// sanity
	if(udt->calling_party_address->global_title == NULL) return 10;
	udt->data = (sccp::Data*)sccp_pool.request_param(sccp::DATA);
	// sanity
	if(udt->data == NULL) return 11;

	unsigned char tmp_test[1] = {0};
	unsigned char sccp_buff[1024];

	// set params
	udt->protocol_class->protocol_class = sccp::CLASS0;
	udt->protocol_class->message_handling = sccp::RETURN_MESSAGE_ON_ERROR;
	// called party
	udt->called_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	udt->called_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	udt->called_party_address->ssn_indicator = true;
	udt->called_party_address->point_code_indicator = false;
	udt->called_party_address->subsystem_number = sccp::MSC;
	// calling party
	udt->calling_party_address->routing_indicator = sccp::ROUTE_ON_GT;
	udt->calling_party_address->global_title_indicator = sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS;
	udt->calling_party_address->ssn_indicator = true;
	udt->calling_party_address->point_code_indicator = false;
	udt->calling_party_address->subsystem_number = sccp::MSC;

	// called GT
	sccp::GT_TTNPENOA* called_gt = (sccp::GT_TTNPENOA*)udt->called_party_address->global_title;
	called_gt->translation_type = 0;
	called_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	called_gt->encoding_scheme = sccp::BCD_ODD;
	called_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	called_gt->address_information_length = 1;
	called_gt->address_information = tmp_test;
	// calling GT
	sccp::GT_TTNPENOA* calling_gt = (sccp::GT_TTNPENOA*)udt->calling_party_address->global_title;
	calling_gt->translation_type = 0;
	calling_gt->numbering_plan = sccp::ISDN_TELEPHONE;
	calling_gt->encoding_scheme = sccp::BCD_ODD;
	calling_gt->nature_of_address = sccp::NOA_INTERNATIONAL;
	calling_gt->address_information_length = 1;
	calling_gt->address_information = tmp_test;
	// data
	udt->data->value_length = tcap_enc_l;
	udt->data->value = tcap_buff;

	// encode sccp
	int sccp_enc_l;
	udt->encode(sccp_buff, sizeof(sccp_buff), 0, &sccp_enc_l);

	// set pdata user data
	pdata->user_protocol_data = sccp_buff;
	pdata->user_protocol_data_length = sccp_enc_l;
	// encode m3ua
	m3uap->encode(pmpld->raw, Connection<PLD_TYPE>::max_mtu, 0, &pmpld->size);
	// set header data
	pmpld->params.set_octets(asn1::ParameterType::_pt_m3ua_header_data, pmpld->raw, pmpld->size);


	// ok
	return 0;

    }
*/
    // err
    return 1;
}



template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::timer_event(){
    // check for sctp connection
    if(Connection<PLD_TYPE>::client_socket == -1) return 1;
    // check if ASPUP needs to be sent
    if(!m3ua_active.get() && Connection<PLD_TYPE>::direction == CD_CLIENT){
	// if timeout reached, init reconnect
	if(time(NULL) - aspup_ts >= aspup_timeout) m3ua_connect();
    }

    return 0;
}

template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::reconnect(){
    // stats
    Connection<PLD_TYPE>::stats.set(CST_M3UA_ACTIVE, 0);
    // sctp reconnect
    sgn::_SCTPConnection<PLD_TYPE>::reconnect();
    // set m3ua as inactive
    m3ua_active.set(false);
    // server mode, stop client threads
    if(ConnectionBase::direction == CD_SERVER){
	// stop client threads
	Connection<PLD_TYPE>::cthreads_active.set(false);


    }
    // state machine on
    if((bool)*ConnectionBase::params.get_param(CPT_DEV_M3UA_SM)){
	// connect m3ua layer
	return m3ua_connect();

    // state machine off
    }else{
	// do only for client connection
	if(ConnectionBase::direction == CD_CLIENT){
	    m3ua_active.set(true);
	    // run event handler
	    pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;
	    e_args.add_arg(CEAT_CONNECTION, this);
	    ConnectionBase::e_handler.run_handler(CET_CONNECTION_UP, e_args);
	    // stats
	    ConnectionBase::stats.set(CST_M3UA_ACTIVE, 1);
	    return 0;

	}
    }

    return 0;
}

template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::data_rx_process(unsigned char* buff, int size, int data_type, void* extra){
    // check size (sctp recv can return 0 size and no error in case of sctp notification messages)
    if(size <= 0) return 10;
    // m3ua ppid == 3
    if(data_type != sctp::M3UA) {
	// stats
	ConnectionBase::stats.inc(CST_DISCARDED_MSU, 1);
	return 1;
    }
    // mtu limit check
    if(size > Connection<PLD_TYPE>::max_mtu){
	// stats
	ConnectionBase::stats.inc(CST_MTU_LIMIT, 1);
	return 2;
    }


    // connect procedure
    if(!m3ua_active.get()){
	// get payload
	m3ua::M3UAPacket* m3uap = NULL;
	PLD_TYPE* pld = NULL;
	if(Connection<PLD_TYPE>::pld_pool.pop(&pld) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_PLD_POOL_EMPTY, 1);
	    return 2;
	}
	// set m3uap pointer
	m3uap = &pld->m3uap;

	// get sctp info
	if(extra != NULL) memcpy(&pld->sctp_info, extra, sizeof(sctp_sndrcvinfo));

	// decode
	if(m3ua::decode(buff, size, m3uap, &m3ua_rx_pool) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

	    // free pld
	    pld->action = PAT_FREE;
	    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

	    return 4;
	}
        // process NOTIFY
        if(m3uap->message->type == m3ua::NTFY){
            m3ua::Ntfy* ntfy = (m3ua::Ntfy*)m3uap->message;
            m3ua::Status* status = (m3ua::Status*)ntfy->get_parameter(m3ua::M3UA_PT_STATUS);
            // check for state change
            if(status != NULL && status->status_type == m3ua::AS_STATE_CHANGE) as_status_info = status->status_info;

            // free pld
            pld->action = PAT_FREE;
            Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
            // ok
            return 0;

        }

	// check connection phase (CLIENT phases start from 0, SERVER from 10)
	switch(conn_phase){
	    // **************** CLIENT MODE *******************

	    // ASPUP sent, waiting for ASPUP ACK
	    case 0:
	    {
		// ASPUP ACK
		if(m3uap->message->type == m3ua::ASPUP_ACK){
		    ++conn_phase;
		    aspup_ts = time(NULL);

                    // m3ua ASPAC
                    m3uap->message_class = m3ua::ASPTM;
                    m3uap->message_type = m3ua::ASPAC;
                    m3ua::Aspac* aspac = (m3ua::Aspac*)m3ua_rx_pool.request_message(m3ua::ASPAC);
                    aspac->clear_params();
                    m3uap->message = aspac;
                    // TMT
                    m3ua::TrafficModeType* tmt = (m3ua::TrafficModeType*)m3ua_rx_pool.request_param(m3ua::M3UA_PT_TRAFFIC_MODE_TYPE);
                    tmt->traffic_mode_type = (m3ua::TMTType)(int)*Connection<PLD_TYPE>::params.get_param(CPT_M3UA_TRAFFIC_MODE);
                    aspac->add_parameter(tmt);
                    // RT CTX
                    int rtctx_num = (int)*Connection<PLD_TYPE>::params.get_param(CPT_M3UA_RT_CTX);
                    if(rtctx_num != 0xffffffff){
                        m3ua::RoutingContext* rtctx = (m3ua::RoutingContext*)m3ua_rx_pool.request_param(m3ua::M3UA_PT_ROUTING_CONTEXT);
                        rtctx->routing_context = rtctx_num;
                        aspac->add_parameter(rtctx);

                    }

                    // sctp part
                    pld->ppid = sctp::M3UA;
                    pld->sid = pld->sctp_info.sinfo_stream;

                    // encode
                    m3uap->encode(pld->raw,
                            Connection<PLD_TYPE>::max_mtu,
                            0,
                            &pld->size);

                    // push to out
                    pld->action = PAT_SEND;
                    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);


		}else{
                    // free pld
                    pld->action = PAT_FREE;
                    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
                }
                break;
	    }

	    // ASPAC sent, waiting for ASPAC ACK
	    case 1:
		if(m3uap->message->type == m3ua::ASPAC_ACK){
		    ++conn_phase;
		    aspup_ts = time(NULL);

                    m3ua_active.comp_swap(false, true);
                    // run event handler
                    pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;
                    e_args.add_arg(CEAT_CONNECTION, this);
                    ConnectionBase::e_handler.run_handler(CET_CONNECTION_UP, e_args);
                    // stats
                    ConnectionBase::stats.set(CST_M3UA_ACTIVE, 1);


		}
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		break;


	    // **************** SERVER MODE *******************

	    // waiting for ASP UP
	    case 10:
		// ASP UP ACK
		if(m3uap->message->type == m3ua::ASPUP){
		    ++conn_phase;
		    // m3ua ASP UP ACK
		    m3uap->message_class = m3ua::ASPSM;
		    m3uap->message_type = m3ua::ASPUP_ACK;
		    m3ua::Aspup_ack* aspup_ack = (m3ua::Aspup_ack*)m3ua_rx_pool.request_message(m3ua::ASPUP_ACK);
		    aspup_ack->clear_params();
		    m3uap->message = aspup_ack;
		    // sctp part
		    pld->ppid = sctp::M3UA;
		    pld->sid = pld->sctp_info.sinfo_stream;
		    // encode
		    m3uap->encode(pld->raw,
				  Connection<PLD_TYPE>::max_mtu,
				  0,
				  &pld->size);

		    // push to out
		    pld->action = PAT_SEND;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		    // get new pld
		    if(Connection<PLD_TYPE>::pld_pool.pop(&pld) != 0){
			// stats
			ConnectionBase::stats.inc(CST_PLD_POOL_EMPTY, 1);
			return 2;
		    }
		    // set m3uap pointer
		    m3uap = &pld->m3uap;

		    // get sctp info
		    if(extra != NULL) memcpy(&pld->sctp_info, extra, sizeof(sctp_sndrcvinfo));

		    // m3ua NTFY
		    m3uap->message_class = m3ua::MGMT;
		    m3uap->message_type = m3ua::NTFY;
		    m3ua::Ntfy* ntfy = (m3ua::Ntfy*)m3ua_rx_pool.request_message(m3ua::NTFY);
		    ntfy->clear_params();
		    m3uap->message = ntfy;
		    // TMT
		    m3ua::TrafficModeType* tmt = (m3ua::TrafficModeType*)m3ua_rx_pool.request_param(m3ua::M3UA_PT_TRAFFIC_MODE_TYPE);
		    tmt->traffic_mode_type = (m3ua::TMTType)(int)*Connection<PLD_TYPE>::params.get_param(CPT_M3UA_TRAFFIC_MODE);
		    ntfy->add_parameter(tmt);
		    // status
		    m3ua::Status* status = (m3ua::Status*)m3ua_rx_pool.request_param(m3ua::M3UA_PT_STATUS);
		    status->status_type = m3ua::AS_STATE_CHANGE;
		    status->status_info = m3ua::AS_INACTIVE;
		    ntfy->add_parameter(status);

		    // sctp part
		    pld->ppid = sctp::M3UA;
		    pld->sid = pld->sctp_info.sinfo_stream;
		    // encode
		    m3uap->encode(pld->raw,
				  Connection<PLD_TYPE>::max_mtu,
				  0,
				  &pld->size);

		    // push to out
		    pld->action = PAT_SEND;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		}else{
		    // free pld
		    pld->action = PAT_FREE;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		}
		break;

	    // ASPAC received, send ASPAC ACK
	    case 11:
		// ASPAC
		if(m3uap->message->type == m3ua::ASPAC){
		    ++conn_phase;
		    // m3ua ASPAC ACK
		    m3uap->message_class = m3ua::ASPTM;
		    m3uap->message_type = m3ua::ASPAC_ACK;
		    m3ua::Aspac_ack* aspac_ack = (m3ua::Aspac_ack*)m3ua_rx_pool.request_message(m3ua::ASPAC_ACK);
		    aspac_ack->clear_params();
		    // use routing context and traffic type from original message
		    m3ua::RoutingContext* rtctx = (m3ua::RoutingContext*)m3uap->message->get_parameter(m3ua::M3UA_PT_ROUTING_CONTEXT);
		    m3ua::TrafficModeType* tmt = (m3ua::TrafficModeType*)m3uap->message->get_parameter(m3ua::M3UA_PT_TRAFFIC_MODE_TYPE);
		    if(rtctx != NULL) aspac_ack->add_parameter(rtctx);
		    if(tmt != NULL) aspac_ack->add_parameter(tmt);
		    m3uap->message = aspac_ack;
		    // sctp part
		    pld->ppid = sctp::M3UA;
		    pld->sid = pld->sctp_info.sinfo_stream;
		    // encode
		    m3uap->encode(pld->raw,
				  Connection<PLD_TYPE>::max_mtu,
				  0,
				  &pld->size);

		    // push to out
		    pld->action = PAT_SEND;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);


		    // get new pld
		    if(Connection<PLD_TYPE>::pld_pool.pop(&pld) != 0){
			// restart connection procedure
			conn_phase = 10;
			// stats
			ConnectionBase::stats.inc(CST_PLD_POOL_EMPTY, 1);
			return 2;
		    }
		    // set m3uap pointer
		    m3uap = &pld->m3uap;

		    // get sctp info
		    if(extra != NULL) memcpy(&pld->sctp_info, extra, sizeof(sctp_sndrcvinfo));

		    // m3ua NTFY
		    m3uap->message_class = m3ua::MGMT;
		    m3uap->message_type = m3ua::NTFY;
		    m3ua::Ntfy* ntfy = (m3ua::Ntfy*)m3ua_rx_pool.request_message(m3ua::NTFY);
		    ntfy->clear_params();
		    m3uap->message = ntfy;
		    // TMT
		    tmt = (m3ua::TrafficModeType*)m3ua_rx_pool.request_param(m3ua::M3UA_PT_TRAFFIC_MODE_TYPE);
		    tmt->traffic_mode_type = (m3ua::TMTType)(int)*Connection<PLD_TYPE>::params.get_param(CPT_M3UA_TRAFFIC_MODE);
		    ntfy->add_parameter(tmt);
		    // status
		    m3ua::Status* status = (m3ua::Status*)m3ua_rx_pool.request_param(m3ua::M3UA_PT_STATUS);
		    status->status_type = m3ua::AS_STATE_CHANGE;
		    status->status_info = m3ua::AS_ACTIVE;
		    ntfy->add_parameter(status);

		    // sctp part
		    pld->ppid = sctp::M3UA;
		    pld->sid = pld->sctp_info.sinfo_stream;
		    // encode
		    m3uap->encode(pld->raw,
				  Connection<PLD_TYPE>::max_mtu,
				  0,
				  &pld->size);

		    // push to out
		    pld->action = PAT_SEND;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		    // active
		    m3ua_active.comp_swap(false, true);
		    // run event handler
		    pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;
		    e_args.add_arg(CEAT_CONNECTION, this);
		    ConnectionBase::e_handler.run_handler(CET_CONNECTION_UP, e_args);
		    // stats
		    ConnectionBase::stats.set(CST_M3UA_ACTIVE, 1);

		}else{
		    // restart connection procedure
		    conn_phase = 10;
		    // free pld
		    pld->action = PAT_FREE;
		    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		}
		break;


	    // unknown state, return to pool
	    default:
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

		break;
	}



    // data
    }else{
	// get m3ua packet
	m3ua::M3UAPacket* m3uap = NULL;
	PLD_TYPE* pld = NULL;
	if(Connection<PLD_TYPE>::pld_pool.pop(&pld) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_PLD_POOL_EMPTY, 1);
	    return 1;
	}

	// set m3uap pointer
	m3uap = &pld->m3uap;

	// get sctp info
	if(extra != NULL) memcpy(&pld->sctp_info, extra, sizeof(sctp_sndrcvinfo));

	// copy raw data to payload
	memcpy(pld->raw, buff, size);
	pld->size = size;

	// decode
	if(m3ua::decode(pld->raw, pld->size, m3uap, &m3ua_rx_pool) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);

	    // free pld
	    pld->action = PAT_FREE;
	    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

	    return 4;
	}

	switch(m3uap->message_type){
            case m3ua::NTFY:
            {
                m3ua::Ntfy* ntfy = (m3ua::Ntfy*)m3uap->message;
                m3ua::Status* status = (m3ua::Status*)ntfy->get_parameter(m3ua::M3UA_PT_STATUS);
                // check for state change
                if(status != NULL && status->status_type == m3ua::AS_STATE_CHANGE) as_status_info = status->status_info;

                // free pld
                pld->action = PAT_FREE;
                Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

                break;
            }

	    // HEARTBEAT ACK
	    case m3ua::BEAT_ACK:
	    {
		m3ua::Beat_ack* beat_ack = (m3ua::Beat_ack*)m3uap->message;
		m3ua::HeartbeatData* bdata = (m3ua::HeartbeatData*)beat_ack->get_parameter(m3ua::M3UA_PT_HEARTBEAT);
		// check for valid data
		if(bdata->value_length == sizeof(uint32_t) && memcmp(bdata->value, &beat_data, sizeof(uint32_t)) == 0){
		    // hbeat ack is ok
		    // diff (-1)
		    beat_diff.fetch_sub(1);
		}
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		// stats
		ConnectionBase::stats.inc(CST_RX_HBEAT_COUNT, 1);

		break;
	    }

	    // HEARTBEAT
	    case m3ua::BEAT:
	    {
		// received heartbeat data
		m3ua::Beat* beat = (m3ua::Beat*)m3uap->message;
		m3ua::HeartbeatData* src_bdata = (m3ua::HeartbeatData*)beat->get_parameter(m3ua::M3UA_PT_HEARTBEAT);

		// m3ua BEAT_ACK
		m3uap->message_class = m3ua::ASPSM;
		m3uap->message_type = m3ua::BEAT_ACK;
		m3ua::Beat_ack* beat_ack = (m3ua::Beat_ack*)m3ua_rx_pool.request_message(m3ua::BEAT_ACK);
		beat_ack->clear_params();
		m3uap->message = beat_ack;

		// BEAT_ACK params
		beat_ack->clear_params();
		m3ua::HeartbeatData* bdata = (m3ua::HeartbeatData*)m3ua_rx_pool.request_param(m3ua::M3UA_PT_HEARTBEAT);
		bdata->value = src_bdata->value;
		bdata->value_length = src_bdata->value_length;
		beat_ack->add_parameter(bdata);


		// sctp part
		pld->ppid = sctp::M3UA;
		pld->sid = pld->sctp_info.sinfo_stream;

		// encode
		m3uap->encode(pld->raw,
			      Connection<PLD_TYPE>::max_mtu,
			      0,
			      &pld->size);

		// push to out
		pld->action = PAT_SEND;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		// stats
		ConnectionBase::stats.inc(CST_RX_HBEAT_COUNT, 1);
		ConnectionBase::stats.inc(CST_TX_HBEAT_COUNT, 1);

		break;
	    }
	    // DATA
	    case m3ua::DATA:
		// last sid
		_M3UAConnection<PLD_TYPE>::last_sid.set(pld->sctp_info.sinfo_stream);
		// set action
		pld->action = PAT_PROCESS;
		pld->sid = pld->sctp_info.sinfo_stream;
		// push to proc
		Connection<PLD_TYPE>::rx_proc_q.push(1, pld);
		break;


	    default:
		// free pld
		pld->action = PAT_FREE;
		Connection<PLD_TYPE>::rx_tx_q.push(1, pld);
		break;

	}

    }

    return 0;
}

template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::m3ua_connect(){
    // client mode
    if(Connection<PLD_TYPE>::direction == CD_CLIENT){
	// if socket connected
	if(Connection<PLD_TYPE>::client_socket != -1){
	    // get payload packet
	    PLD_TYPE* pld = NULL;
	    if(Connection<PLD_TYPE>::pld_pool.pop(&pld) != 0){
		// stats
		ConnectionBase::stats.inc(CST_PLD_POOL_EMPTY, 1);
		return 1;
	    }

	    // get m3ua packet
	    m3ua::M3UAPacket* m3uap = &pld->m3uap;

	    // sctp part
	    pld->ppid = sctp::M3UA;
	    pld->sid = 0;

	    // m3ua ASP UP
	    m3uap->message_class = m3ua::ASPSM;
	    m3uap->message_type = m3ua::ASPUP;
	    m3ua::Aspup* aspup = (m3ua::Aspup*)m3ua_rx_pool.request_message(m3ua::ASPUP);
	    aspup->clear_params();
	    m3uap->message = aspup;

	    // encode
	    m3uap->encode(pld->raw,
			  Connection<PLD_TYPE>::max_mtu,
			  0,
			  &pld->size);

	    // connect phase
	    conn_phase = 0;

	    // push
	    pld->action = PAT_SEND;
	    Connection<PLD_TYPE>::rx_tx_q.push(1, pld);

	    // set asp up timestamp
	    aspup_ts = time(NULL);

	    // ok
	    return 0;
	}

    // server mode
    }else{
	conn_phase = 10;
	// ok
	return 0;

    }


    return 1;
}


template<typename PLD_TYPE>
void sgn::_M3UAConnection<PLD_TYPE>::terminate(){
    if(terminated) return;

    pmink_utils::VariantParam* vp = ConnectionBase::params.get_param(CPT_R14P_STATS);
    if(vp == NULL) return;
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)(void*)*vp;

    // remove stats handlers
    std::string stats_prefix("M3UA::");
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_RX_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TX_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_MTU_LIMIT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_DISCARDED_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_PLD_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_MALFORMED_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_RX_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TX_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_PMPLD_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMSG_SEND_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMSG_MSG_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMSG_PARAM_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_M3UA_ACTIVE")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SCTP_ACTIVE")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMSG_SENT_COUNT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_NO_VALID_ROUTES")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_RX_HBEAT_COUNT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TX_HBEAT_COUNT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_CMD_PROCESS_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_ENCODE_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_RECONNECT")));

    terminated = true;


}



template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::stop(){
    // m3ua inactive
    m3ua_active.comp_swap(true, false);
	// stats
	ConnectionBase::stats.set(CST_M3UA_ACTIVE, 0);
    // stop sctp
    _SCTPConnection<PLD_TYPE>::stop();
    // ok
    return 0;
}


template<typename PLD_TYPE>
int sgn::_M3UAConnection<PLD_TYPE>::start(){
    // get params from AS
    ApplicationServer* tmp_as = as.get();//(ApplicationServer*)(void*)*ConnectionBase::params.get_param(CPT_APPLICATION_SERVER);
    if(tmp_as != NULL){
	// routing-key
	ConnectionBase::params.set_int(CPT_M3UA_TRAFFIC_MODE, (int)*tmp_as->params.get_param(CPT_M3UA_TRAFFIC_MODE));
	ConnectionBase::params.set_int(CPT_M3UA_RT_CTX, (int)*tmp_as->params.get_param(CPT_M3UA_RT_CTX));
	// decoders
	ConnectionBase::params.set_bool(CPT_DECODE_M3UA, true);
	ConnectionBase::params.set_bool(CPT_DECODE_SCCP, (bool)*tmp_as->params.get_param(CPT_DECODE_SCCP));
	ConnectionBase::params.set_bool(CPT_DECODE_ISUP, (bool)*tmp_as->params.get_param(CPT_DECODE_ISUP));
	ConnectionBase::params.set_bool(CPT_DECODE_TCAP, (bool)*tmp_as->params.get_param(CPT_DECODE_TCAP));
	ConnectionBase::params.set_bool(CPT_DECODE_GSM_MAP, (bool)*tmp_as->params.get_param(CPT_DECODE_GSM_MAP));
	ConnectionBase::params.set_bool(CPT_DECODE_SMS_TPDU, (bool)*tmp_as->params.get_param(CPT_DECODE_SMS_TPDU));


    }

    // get decoder flags for fast access
    decoders[0] = (bool)*ConnectionBase::params.get_param(CPT_DECODE_M3UA);
    decoders[1] = (bool)*ConnectionBase::params.get_param(CPT_DECODE_SCCP);
    decoders[2] = (bool)*ConnectionBase::params.get_param(CPT_DECODE_ISUP);
    decoders[3] = (bool)*ConnectionBase::params.get_param(CPT_DECODE_TCAP);
    decoders[4] = (bool)*ConnectionBase::params.get_param(CPT_DECODE_GSM_MAP);
    decoders[5] = (bool)*ConnectionBase::params.get_param(CPT_DECODE_SMS_TPDU);

    // enquire link timer
    hbeat_ts = (int)*ConnectionBase::params.get_param(CPT_M3UA_HBEAT_TIMER);

    // enquire link enabled/disabled
    hbeat_enabled = (bool)*ConnectionBase::params.get_param(CPT_DEV_M3UA_SM);

    // get dr flag
    retain_data = (bool)*ConnectionBase::params.get_param(CPT_DATA_RETENTION);

    // get asp up timeout
    aspup_timeout = (int)*ConnectionBase::params.get_param(CPT_M3UA_ASPUP_TIMER);

    // get reconnect timer
    Connection<PLD_TYPE>::reconnect_timer = aspup_timeout;

    // get dpc, opc
    opc = (int)*ConnectionBase::params.get_param(CPT_M3UA_OPC);
    dpc = (int)*ConnectionBase::params.get_param(CPT_M3UA_DPC);


    // sctp start
    int sctp_res = _SCTPConnection<PLD_TYPE>::start();
    if(sctp_res < 0) pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG,
						"Error while initializing SCTP socket for ASP [%s]",
						 Connection<PLD_TYPE>::label.c_str());

    // ok
    return 0;

}

// _TCPConnection
template<typename PLD_TYPE>
sgn::_TCPConnection<PLD_TYPE>::_TCPConnection(ConnectionDirection _direction,
						ServerConnectionType _server_type,
					        const char* _label,
						int _max_mtu,
						int _pool_size): Connection<PLD_TYPE>(CT_TCP,
										     _direction,
										     _server_type,
										     _label,
										     _max_mtu,
										     _pool_size){
    // no params for now

}

template<typename PLD_TYPE>
sgn::_TCPConnection<PLD_TYPE>::~_TCPConnection(){
    stop();
}


template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::get_client(int _server, sockaddr_in* sci){
    int socket = -1;
    if(sci != NULL){
	// zero mem
	bzero((void*)sci, sizeof(sockaddr_in));
	// sizes
	int size_sci = sizeof(sockaddr_in);
	// accept socket
	socket = accept(_server, (sockaddr*)sci, (socklen_t*)&size_sci);


    }else{
	socket = accept(_server, NULL, NULL);

    }

    if(socket > 0){
	// no delay
	int int_bool = 1;
	setsockopt( socket, IPPROTO_TCP, TCP_NODELAY, &int_bool, sizeof(int_bool));
	setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &int_bool, sizeof(int_bool));

    }


    return socket;

}


template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::shutdown_tcp_client(int _socket){
    int res = shutdown(_socket, SHUT_RDWR);
    res += close(_socket);
    return res;

}


template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::init_tcp_server(uint32_t local_addr, uint16_t local_port){
    int ret, connSock;
    struct sockaddr_in servaddr;

    /* Create  TCP Socket */
    connSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if(connSock < 0){
	return -1;
    }
    int yes = 1;
    // set socket option
    setsockopt(connSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    // automatic bind
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(local_port);
    if(local_addr == 0) servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else {
      // specific bind
      servaddr.sin_addr.s_addr = local_addr;

    }
    // bind
    ret = bind(connSock, (sockaddr *)&servaddr, sizeof(sockaddr_in));
    if(ret < 0){
	shutdown(connSock, SHUT_RDWR);
	close(connSock);
	return -1;
    }

    // no delay
    int int_bool = 1;
    setsockopt( connSock, IPPROTO_TCP, TCP_NODELAY, &int_bool, sizeof(int_bool));

    // listen
    listen(connSock, 5);
    // return socket
    return connSock;

}

template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::init_tcp_client_bind(uint32_t remote_addr,
							uint32_t local_addr,
							uint16_t local_port,
							uint16_t remote_port){
    int ret, connSock;
    struct sockaddr_in servaddr;
    struct sockaddr_in local_bind;

    /* Create  TCP Socket */
    connSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if(connSock < 0){
	return -1;
    }
    int yes = 1;
    // set socket option
    setsockopt(connSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	#ifdef SO_REUSEPORT
	setsockopt(connSock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int));
	#endif
    // bind
    bzero(&local_bind, sizeof(local_bind) );
    local_bind.sin_family = AF_INET;
    local_bind.sin_addr.s_addr = local_addr;
    local_bind.sin_port = htons(local_port);
    ret = bind(connSock, (sockaddr *)&local_bind, sizeof(sockaddr_in));
    if(ret < 0){
	shutdown(connSock, SHUT_RDWR);
	close(connSock);
	return -1;
    }
    // no delay
    int int_bool = 1;
    setsockopt( connSock, IPPROTO_TCP, TCP_NODELAY, &int_bool, sizeof(int_bool));

    /* Specify peer endpoint to which we'll connect */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(remote_port);
    servaddr.sin_addr.s_addr = remote_addr;

    // set as non blocking
    fcntl(connSock, F_SETFL, O_NONBLOCK);
    // set params for select
    fd_set fdset;
    timeval tv;
    FD_ZERO(&fdset);
    FD_SET(connSock, &fdset);
    tv.tv_sec = Connection<PLD_TYPE>::reconnect_timer;
    tv.tv_usec = 0;

    /* Connect to the server */
    ret = connect(connSock, (sockaddr*)&servaddr, sizeof(servaddr));
    // check for immediate connect
    if(ret == 0) return connSock;
    // error
    if(ret < 0){
	// check if connect in progress
	if(errno == EINPROGRESS){
	    // check if conn ready for writing and no timeout occured
	    if(select(connSock + 1, NULL, &fdset, NULL, &tv) == 1){
		// check if SO_ERROR is zero(0)
	        int so_error = -1;
	        socklen_t len = sizeof(so_error);
	        getsockopt(connSock, SOL_SOCKET, SO_ERROR, &so_error, &len);
	        if(so_error == 0) return connSock;
	    }
	}

	// close socket
	shutdown(connSock, SHUT_RDWR);
	close(connSock);
	return -1;

    }


    return -1;
}


template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::start(){
    // ip addr struct
    in_addr addr;
    uint32_t remote_ip = 0;
    uint32_t local_ip = 0;
    uint32_t local_port = 0;

    // stats
    Connection<PLD_TYPE>::stats.set(CST_TCP_ACTIVE, 0);

    // check for primary remote ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_TCP_REMOTE_IP) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_TCP_REMOTE_IP), &addr) != 0) remote_ip = addr.s_addr;

    }

    // check for primary local ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_TCP_LOCAL_IP) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_TCP_LOCAL_IP), &addr) != 0) local_ip = addr.s_addr;
    }


    // check for local port
    if(Connection<PLD_TYPE>::params.get_param(CPT_TCP_LOCAL_PORT) != NULL){
	local_port = (int)*Connection<PLD_TYPE>::params.get_param(CPT_TCP_LOCAL_PORT);

    // use global port range
    }else{
	pmink_utils::Range<uint32_t>* local_ports = (pmink_utils::Range<uint32_t>*)(void*)*Connection<PLD_TYPE>::params.get_param(CPT_LOCAL_PORT_RANGE);
	local_port = local_ports->next();
    }

    // start client (normal client or child conn in case of one to many)
    if(Connection<PLD_TYPE>::direction == CD_CLIENT || ConnectionBase::master_conn != NULL){
	// start client if not one to many server connection (socket already established by master)
	if(ConnectionBase::master_conn == NULL){
	    Connection<PLD_TYPE>::client_socket = init_tcp_client_bind(remote_ip,
								       local_ip,
								       local_port,
								       (int)*Connection<PLD_TYPE>::params.get_param(CPT_TCP_REMOTE_PORT));

	}


	// stats
	if(Connection<PLD_TYPE>::client_socket >= 0) Connection<PLD_TYPE>::stats.inc(CST_TCP_ACTIVE, 1);
	// base start
	Connection<PLD_TYPE>::start();
	// return client socket
	return Connection<PLD_TYPE>::client_socket;

    // start server
    }else{
	// start server
	Connection<PLD_TYPE>::server_socket = init_tcp_server(local_ip,
							      (int)*Connection<PLD_TYPE>::params.get_param(CPT_TCP_LOCAL_PORT));

	//char tmp[16];
	//sprintf(tmp, "server_%s", Connection<PLD_TYPE>::label.substr(0, 8).c_str());
	ConnectionBase::start();
	Connection<PLD_TYPE>::thm.start();
	//Connection<PLD_TYPE>::thm.new_thread(&Connection<PLD_TYPE>::server_loop, tmp, this);
	// return server socket
	return Connection<PLD_TYPE>::server_socket;
    }

    return Connection<PLD_TYPE>::client_socket;




    return 0;
}

template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::stop(){
    // stats
    Connection<PLD_TYPE>::stats.set(CST_TCP_ACTIVE, 0);
    // base stop
    Connection<PLD_TYPE>::stop();
    // close client socket
    if(Connection<PLD_TYPE>::client_socket != -1){
	shutdown_tcp_client(Connection<PLD_TYPE>::client_socket);
	Connection<PLD_TYPE>::client_socket = -1;
    }
    // close server socket
    if(Connection<PLD_TYPE>::server_socket != -1){
	shutdown_tcp_client(Connection<PLD_TYPE>::server_socket);
	Connection<PLD_TYPE>::server_socket = -1;
    }

    return 0;
}

template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::server_get_client(int _server_socket){
    return get_client(_server_socket, NULL);
}


template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::data_rx(unsigned char* buff, int size, int* bytes_rvcd, int* pld_type, void* extra){
    int res = recv(Connection<PLD_TYPE>::client_socket, buff, size, 0);
    // ok
    if(res > 0){
	// received payload size
	*bytes_rvcd = res;
	// received payload type
	*pld_type = 0;
	// no extra information for now
	// ok
	return 0;
    }

    // error
    return 1;
}

template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::data_rx_process(unsigned char* buff, int size, int data_type, void* extra){
    // nothing for now
    return 0;
}


template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::proc_process(PLD_TYPE* pld){
    // nothing for now
    return 0;
}


template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::data_tx(Payload* data){
    int res = send(Connection<PLD_TYPE>::client_socket, data->raw, data->size, MSG_NOSIGNAL);
    return (res > 0 ? 0 : 1);
}


template<typename PLD_TYPE>
int sgn::_TCPConnection<PLD_TYPE>::reconnect(){
    // ip addr struct
    in_addr addr;
    uint32_t remote_ip = 0;
    uint32_t local_ip = 0;
    uint32_t local_port = 0;
    // stats
    Connection<PLD_TYPE>::stats.set(CST_TCP_ACTIVE, 0);
    if(ConnectionBase::master_conn != NULL){
	ConnectionBase::master_conn->stats.dec(CST_TCP_ACTIVE, 1);
    }
    // check for remote ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_TCP_REMOTE_IP) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_TCP_REMOTE_IP), &addr) != 0) remote_ip = addr.s_addr;

    }

    // check for local ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_TCP_LOCAL_IP) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_TCP_LOCAL_IP), &addr) != 0) local_ip = addr.s_addr;
    }

    // check for local port
    if(Connection<PLD_TYPE>::params.get_param(CPT_TCP_LOCAL_PORT) != NULL){
	local_port = (int)*Connection<PLD_TYPE>::params.get_param(CPT_TCP_LOCAL_PORT);

    // use global port range
    }else{
	pmink_utils::Range<uint32_t>* local_ports = (pmink_utils::Range<uint32_t>*)(void*)*Connection<PLD_TYPE>::params.get_param(CPT_LOCAL_PORT_RANGE);
	local_port = local_ports->next();
    }

    if(Connection<PLD_TYPE>::direction == CD_SERVER){
	// close old socket
	shutdown_tcp_client(Connection<PLD_TYPE>::server_socket);
	shutdown_tcp_client(Connection<PLD_TYPE>::client_socket);
	// in case of child one-t-many connection, reset client socket
	Connection<PLD_TYPE>::client_socket = -1;

	// do not start in case of child on to many connection, handled by master conn
	if(ConnectionBase::master_conn == NULL){
	    // start server
	    Connection<PLD_TYPE>::server_socket = init_tcp_server(local_ip, local_port);
	    //char tmp[16];
	    //sprintf(tmp, "server_%s", Connection<PLD_TYPE>::label.substr(0, 8).c_str());
	    //Connection<PLD_TYPE>::thm.new_thread(&Connection<PLD_TYPE>::server_loop, tmp, this);

	}

	return Connection<PLD_TYPE>::server_socket;

    }else{
	// close old socket
	shutdown_tcp_client(Connection<PLD_TYPE>::client_socket);
	// start client
	Connection<PLD_TYPE>::client_socket = init_tcp_client_bind(remote_ip,
								   local_ip,
								   local_port,
								   (int)*Connection<PLD_TYPE>::params.get_param(CPT_TCP_REMOTE_PORT));
	// stats
	if(Connection<PLD_TYPE>::client_socket >= 0) Connection<PLD_TYPE>::stats.inc(CST_TCP_ACTIVE, 1);

	return Connection<PLD_TYPE>::client_socket;
    }


    return 0;
}

// TCPConnection
sgn::TCPConnection::TCPConnection(ConnectionDirection _direction,
				    ServerConnectionType _server_type,
		   		    const char* _label,
				    int _max_mtu,
				    int _pool_size): _TCPConnection<TcpPayload>(_direction,
										  _server_type,
										  _label,
										  _max_mtu,
										  _pool_size){

}



// SCTPConnection
sgn::SCTPConnection::SCTPConnection(ConnectionDirection _direction,
				    ServerConnectionType _server_type,
		   		    const char* _label,
				    int _max_mtu,
				    int _pool_size): _SCTPConnection<SctpPayload>(_direction,
										  _server_type,
										  _label,
										  _max_mtu,
										  _pool_size){

}


// _SCTPConnection
template<typename PLD_TYPE>
sgn::_SCTPConnection<PLD_TYPE>::_SCTPConnection(ConnectionDirection _direction,
						ServerConnectionType _server_type,
					        const char* _label,
						int _max_mtu,
						int _pool_size): Connection<PLD_TYPE>(CT_SCTP,
										     _direction,
										     _server_type,
										     _label,
										     _max_mtu,
										     _pool_size){

    // default params
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_MAX_STREAMS, 16);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_HB_INTERVAL, 30000);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_PATH_MAX_RETRANS, 5);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_MAX_INIT_RETRANS, 8);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_RTO_INITIAL, 3000);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_RTO_MAX, 60000);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_RTO_MIN, 1000);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_MAX_BURST, 4);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_SACK_TIMEOUT, 200);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_SACK_FREQ, 2);
    Connection<PLD_TYPE>::params.set_int(CPT_SCTP_VALID_COOKIE_LIFE, 60000);



}

template<typename PLD_TYPE>
sgn::_SCTPConnection<PLD_TYPE>::~_SCTPConnection(){
    stop();
}


template<typename PLD_TYPE>
int sgn::_SCTPConnection<PLD_TYPE>::server_get_client(int _server_socket){
    return sctp::get_client(_server_socket, NULL);
}



template<typename PLD_TYPE>
int sgn::_SCTPConnection<PLD_TYPE>::start(){
    // ip addr struct
    in_addr addr;
    uint32_t remote_ip_1 = 0;
    uint32_t remote_ip_2 = 0;
    uint32_t local_ip_1 = 0;
    uint32_t local_ip_2 = 0;
    uint32_t local_port = 0;

    // stats
    Connection<PLD_TYPE>::stats.set(CST_SCTP_ACTIVE, 0);

    // check for primary remote ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_IP_1) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_IP_1), &addr) != 0) remote_ip_1 = addr.s_addr;

    }
    // check for secondary remote ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_IP_2) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_IP_2), &addr) != 0) remote_ip_2 = addr.s_addr;

    }


    // check for primary local ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_IP_1) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_IP_1), &addr) != 0) local_ip_1 = addr.s_addr;
    }

    // check for secondary local ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_IP_2) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_IP_2), &addr) != 0) local_ip_2 = addr.s_addr;
    }

    // check for local port
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_PORT) != NULL){
	local_port = (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_PORT);

    // use global port range
    }else{
	pmink_utils::Range<uint32_t>* local_ports = (pmink_utils::Range<uint32_t>*)(void*)*Connection<PLD_TYPE>::params.get_param(CPT_LOCAL_PORT_RANGE);
	local_port = local_ports->next();
    }

    if(Connection<PLD_TYPE>::direction == CD_CLIENT){
	// start client
	Connection<PLD_TYPE>::client_socket = sctp::init_sctp_client_bind( remote_ip_1,
						     remote_ip_2,
						     local_ip_1,
						     local_ip_2,
						     local_port,
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_PORT),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_STREAMS),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_HB_INTERVAL),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_PATH_MAX_RETRANS),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_INIT_RETRANS),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_INITIAL),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_MAX),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_MIN),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_BURST),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_SACK_TIMEOUT),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_SACK_FREQ),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_VALID_COOKIE_LIFE)
						     );

	// stats
	if(Connection<PLD_TYPE>::client_socket >= 0) Connection<PLD_TYPE>::stats.inc(CST_SCTP_ACTIVE, 1);
	// base start
	Connection<PLD_TYPE>::start();
	// return client socket
	return Connection<PLD_TYPE>::client_socket;

    // start server
    }else{
	// start server
	Connection<PLD_TYPE>::server_socket = sctp::init_sctp_server(	local_ip_1,
						local_ip_2,
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_PORT),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_HB_INTERVAL),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_PATH_MAX_RETRANS),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_INIT_RETRANS),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_INITIAL),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_MAX),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_MIN),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_BURST),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_SACK_TIMEOUT),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_SACK_FREQ),
						(int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_VALID_COOKIE_LIFE)
						);

	//char tmp[16];
	//sprintf(tmp, "server_%s", Connection<PLD_TYPE>::label.substr(0, 8).c_str());
	ConnectionBase::start();
	Connection<PLD_TYPE>::thm.start();
	//Connection<PLD_TYPE>::thm.new_thread(&Connection<PLD_TYPE>::server_loop, tmp, this);
	// return server socket
	return Connection<PLD_TYPE>::server_socket;
    }

    return Connection<PLD_TYPE>::client_socket;



}
template<typename PLD_TYPE>
int sgn::_SCTPConnection<PLD_TYPE>::stop(){
    // stats
    Connection<PLD_TYPE>::stats.set(CST_SCTP_ACTIVE, 0);
    // base stop
    Connection<PLD_TYPE>::stop();
    // close client socket
    if(Connection<PLD_TYPE>::client_socket != -1){
	sctp::shutdown_sctp_client(Connection<PLD_TYPE>::client_socket);
	Connection<PLD_TYPE>::client_socket = -1;
    }
    // close server socket
    if(Connection<PLD_TYPE>::server_socket != -1){
	sctp::shutdown_sctp_client(Connection<PLD_TYPE>::server_socket);
	Connection<PLD_TYPE>::server_socket = -1;
    }

    return 0;

}

template<typename PLD_TYPE>
int sgn::_SCTPConnection<PLD_TYPE>::data_rx(unsigned char* buff, int size, int* bytes_rvcd, int* pld_type, void* extra){
    sctp_sndrcvinfo rcvinfo = {0};
    sctp_notification* sctp_ntf;
    sctp_assoc_change* sctp_assoc;
    int flags = 0;
    int res = sctp::rcv_sctp(Connection<PLD_TYPE>::client_socket, buff, size, &flags, &rcvinfo);

    // ok
    if(res > 0){
	// check if sctp notification
	if(flags & MSG_NOTIFICATION){
	    // received payload size
	    *bytes_rvcd = 0;
	    // notification pointer
	    sctp_ntf = (sctp_notification*)buff;
	    // check type
	    switch(sctp_ntf->sn_header.sn_type){
		// shutdown
		case SCTP_SHUTDOWN_EVENT:
		    // reconnect
		    return 2;

		// abort
		case SCTP_ASSOC_CHANGE:
		    sctp_assoc = (sctp_assoc_change*)buff;
		    if(sctp_assoc->sac_state == SCTP_COMM_LOST){
			// reconnect
			return 2;

		    }
		    break;

		// ok
		default: return 0;
	    }

	    // ok
	    return 0;
	}
	// received payload size
	*bytes_rvcd = res;
	// received payload type
	*pld_type = be32toh(rcvinfo.sinfo_ppid);
	// extra information
	if(extra != NULL) memcpy(extra, &rcvinfo, sizeof(rcvinfo));
	// ok
	return 0;
    }

    // error
    return 1;

}

template<typename PLD_TYPE>
int sgn::_SCTPConnection<PLD_TYPE>::data_rx_process(unsigned char* buff, int size, int data_type, void* extra){
    // nothing for now
    return 0;
}


template<typename PLD_TYPE>
int  sgn::_SCTPConnection<PLD_TYPE>::proc_process(PLD_TYPE* pld){
    // nothing for now
    return 0;
}


template<typename PLD_TYPE>
int sgn::_SCTPConnection<PLD_TYPE>::data_tx(Payload* data){
    // init sctp pld pointer
    PLD_TYPE* sctp_pld = NULL;
    // check payload type
    switch(data->type){
	case CT_PMINK:
	    sctp_pld = &(((PminkPayload*)data)->m3ua_pld);
	    break;

	default:
	    sctp_pld = (PLD_TYPE*)data;
	    break;
    }
    // cast check
    if(sctp_pld == NULL) return 1;
    // send
    return sctp::send_sctp(Connection<PLD_TYPE>::client_socket, data->raw, data->size, sctp_pld->ppid, sctp_pld->sid);
}


template<typename PLD_TYPE>
int sgn::_SCTPConnection<PLD_TYPE>::reconnect(){
    // ip addr struct
    in_addr addr;
    uint32_t remote_ip_1 = 0;
    uint32_t remote_ip_2 = 0;
    uint32_t local_ip_1 = 0;
    uint32_t local_ip_2 = 0;
    uint32_t local_port = 0;
    // stats
    Connection<PLD_TYPE>::stats.set(CST_SCTP_ACTIVE, 0);

    // check for primary remote ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_IP_1) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_IP_1), &addr) != 0) remote_ip_1 = addr.s_addr;

    }
    // check for secondary remote ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_IP_2) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_IP_2), &addr) != 0) remote_ip_2 = addr.s_addr;

    }


    // check for primary local ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_IP_1) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_IP_1), &addr) != 0) local_ip_1 = addr.s_addr;
    }

    // check for secondary local ip
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_IP_2) != NULL){
	if(inet_aton(*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_IP_2), &addr) != 0) local_ip_2 = addr.s_addr;
    }

    // check for local port
    if(Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_PORT) != NULL){
	local_port = (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_LOCAL_PORT);

    // use global port range
    }else{
	pmink_utils::Range<uint32_t>* local_ports = (pmink_utils::Range<uint32_t>*)(void*)*Connection<PLD_TYPE>::params.get_param(CPT_LOCAL_PORT_RANGE);
	local_port = local_ports->next();
    }

    if(Connection<PLD_TYPE>::direction == CD_SERVER){
	// close old socket
	sctp::shutdown_sctp_server(Connection<PLD_TYPE>::server_socket);
	sctp::shutdown_sctp_client(Connection<PLD_TYPE>::client_socket);
	// start server
	Connection<PLD_TYPE>::server_socket = sctp::init_sctp_server(	 local_ip_1,
						 local_ip_2,
						 local_port,
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_HB_INTERVAL),
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_PATH_MAX_RETRANS),
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_INIT_RETRANS),
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_INITIAL),
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_MAX),
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_MIN),
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_BURST),
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_SACK_TIMEOUT),
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_SACK_FREQ),
						 (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_VALID_COOKIE_LIFE)
						);
	//char tmp[16];
	//sprintf(tmp, "server_%s", Connection<PLD_TYPE>::label.substr(0, 8).c_str());
	//Connection<PLD_TYPE>::thm.new_thread(&Connection<PLD_TYPE>::server_loop, tmp, this);
	return Connection<PLD_TYPE>::server_socket;
    }else{
	// close old socket
	sctp::shutdown_sctp_client(Connection<PLD_TYPE>::client_socket);
	// start client
	Connection<PLD_TYPE>::client_socket = sctp::init_sctp_client_bind( remote_ip_1,
						     remote_ip_2,
						     local_ip_1,
						     local_ip_2,
						     local_port,
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_REMOTE_PORT),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_STREAMS),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_HB_INTERVAL),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_PATH_MAX_RETRANS),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_INIT_RETRANS),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_INITIAL),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_MAX),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_RTO_MIN),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_MAX_BURST),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_SACK_TIMEOUT),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_SACK_FREQ),
						     (int)*Connection<PLD_TYPE>::params.get_param(CPT_SCTP_VALID_COOKIE_LIFE)
						 );
	// stats
	if(Connection<PLD_TYPE>::client_socket >= 0) Connection<PLD_TYPE>::stats.inc(CST_SCTP_ACTIVE, 1);

	return Connection<PLD_TYPE>::client_socket;
    }
}

// DefaultConnectionLogic
template<typename PLD_TYPE>
sgn::DefaultConnectionLogic<PLD_TYPE>::DefaultConnectionLogic(Connection<PLD_TYPE>* _conn): conn(_conn){

}

template<typename PLD_TYPE>
int sgn::DefaultConnectionLogic<PLD_TYPE>::run(int phase, void* args){

    switch(phase){
	// rx logic
	case CLPT_RX:
	{
	    // rx params type
	    struct rx_params_t{
		int poll_res;
		short int poll_revents;
		int* poll_socket;
	    };

	    // set params pointer
	    rx_params_t* rx_params = (rx_params_t*)args;
	    // run logic
	    return conn->rx_logic(rx_params->poll_res, rx_params->poll_revents, rx_params->poll_socket);
	}
	// tx logic
	case CLPT_TX:
	    return conn->tx_logic();

	// server logic
	case CLPT_SERVER:
	{
	    // rx params type
	    struct srv_params_t{
		int poll_res;
		short int poll_revents;
		int* poll_socket;
		ConnectionBase* newconn;
	    };
	    // set params pointer
	    srv_params_t* srv_params = (srv_params_t*)args;
	    // run logic
	    return conn->server_logic(srv_params->poll_res, srv_params->poll_revents, srv_params->poll_socket, &srv_params->newconn);
	}
	// proc logic
	case CLPT_PROC:
	    return conn->proc_logic();

	// extra logic
	case CLPT_EXTRA:
	    return conn->extra_logic();

	// reconnect logic
	case CLPT_RECONNECT:
	    return conn->reconnect_logic();

	// pcap logic
	case CLPT_PCAP:
	    return conn->pcap_logic();


	// unknown
	default: break;
    }
    return 10;
}






// ConnectionBase
sgn::ConnectionBase::ConnectionBase(ConnectionType _type,
				    ConnectionDirection _direction,
				    ServerConnectionType _server_type,
				    const char* _label): type(_type),
							 direction(_direction),
							 server_type(_server_type),
							 master_conn(NULL),
							 logic(NULL),
							 client_socket(-1),
							 server_socket(-1){

    // set label
    label.assign(_label);

}


sgn::ConnectionBase::~ConnectionBase(){
    stop();
}


sgn::ConnectionLogic* sgn::ConnectionBase::get_logic(){
    return logic;
}

void sgn::ConnectionBase::set_logic(ConnectionLogic* _logic){
    logic = _logic;
}

const char* sgn::ConnectionBase::get_label(){
    return label.c_str();
}

pmink_utils::EventManager<sgn::ConnectionEventType, sgn::ConnectiontEventArgType, void*>* sgn::ConnectionBase::get_e_handler(){
    return &e_handler;
}


sgn::ConnectionType sgn::ConnectionBase::get_type(){
    return type;
}

sgn::ConnectionDirection sgn::ConnectionBase::get_direction(){
    return direction;
}

sgn::ServerConnectionType sgn::ConnectionBase::get_server_type(){
    return server_type;
}


int sgn::ConnectionBase::start(){
    active.comp_swap(false, true);
    return 0;
}

int sgn::ConnectionBase::stop(){
    active.comp_swap(true, false);
    return 0;
}

sgn::ConnectionBase* sgn::ConnectionBase::get_master_conn(){
    return master_conn;
}

sgn::ConnectionBase* sgn::ConnectionBase::get_conn(){
    return this;
}

int sgn::ConnectionBase::get_client_socket(){
    return client_socket;
}


int sgn::ConnectionBase::get_server_socket(){
    return server_socket;
}

bool sgn::ConnectionBase::is_garbage(){
    return false;
}



// SMPP_ApplicationServer
sgn::SMPP_ApplicationServer::SMPP_ApplicationServer(const char* _label){
    label.assign(_label);
    override_conn = NULL;
    stats.register_item(ASST_ASP_NOT_FOUND);

}

sgn::SMPP_ApplicationServer::~SMPP_ApplicationServer(){
    terminate();
}

void sgn::SMPP_ApplicationServer::terminate(){
    pmink_utils::VariantParam* vp = params.get_param(CPT_R14P_STATS);
    if(vp == NULL) return;
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)(void*)*vp;

    // remove stats handlers
    std::string stats_prefix("SMPP::");
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + label + "_ASP_NOT_FOUND")));

}

const char* sgn::SMPP_ApplicationServer::get_label(){
    return label.c_str();
}


sgn::SMPPConnection* sgn::SMPP_ApplicationServer::get_next_asp(){
    pmink_utils::WRRItem<SMPPConnection*>* wrr_item = wrr.run();
    if(wrr_item != NULL) return wrr_item->item;
    return NULL;
}

pmink_utils::WRR<sgn::SMPPConnection*>* sgn::SMPP_ApplicationServer::get_wrr(){
    return &wrr;
}



// ApplicationServer
sgn::ApplicationServer::ApplicationServer(const char* _label){
    label.assign(_label);
    traffic_mode = m3ua::LOADSHARE;
    override_conn = NULL;
    stats.register_item(ASST_ASP_NOT_FOUND);

}

sgn::ApplicationServer::~ApplicationServer(){
    terminate();
}

void sgn::ApplicationServer::terminate(){
    pmink_utils::VariantParam* vp = params.get_param(CPT_R14P_STATS);
    if(vp == NULL) return;
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)(void*)*vp;

    // remove stats handlers
    std::string stats_prefix("M3UA::");
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + label + "_ASP_NOT_FOUND")));

}

const char* sgn::ApplicationServer::get_label(){
    return label.c_str();
}

sgn::M3UAConnection* sgn::ApplicationServer::get_next_asp(bool* broadcast){
    // assumo no broadcast
    *broadcast = false;
    // cehck type
    switch(traffic_mode){
	case m3ua::LOADSHARE:
	{
	    pmink_utils::WRRItem<M3UAConnection*>* wrr_item = wrr.run();
	    if(wrr_item != NULL) return wrr_item->item;
	    return NULL;
	}

	case m3ua::OVERRIDE:
	    return override_conn;

	case m3ua::BROADCAST:
	    *broadcast = true;
	    return NULL;
    }

    return NULL;
}

pmink_utils::WRR<sgn::M3UAConnection*>* sgn::ApplicationServer::get_wrr(){
    return &wrr;
}



// Connection
template<typename PLD_TYPE>
sgn::Connection<PLD_TYPE>::Connection(ConnectionType _type,
				      ConnectionDirection _direction,
				      ServerConnectionType _server_type,
				      const char* _label,
				      int _max_mtu,
				      int _pool_size):	 ConnectionBase(_type, _direction, _server_type, _label),
							 pool_size(_pool_size),
							 max_mtu(_max_mtu),
							 max_in_connc(1),
							 reconnect_timer(5),
							 reconnect_ts(0){
    // set label
    label.assign(_label);
    // raw buffer (local and outer)
    raw_buff.init(_max_mtu, _pool_size * 2);

    // local payload pool
    pld_pool.init(_pool_size);
    PLD_TYPE* tmp_pld = NULL;
    // set raw buffer pointers
    for(unsigned int i = 0; i<_pool_size; i++){
	pld_pool.pop(&tmp_pld);
	tmp_pld->raw = raw_buff.get_chunk(i);
	pld_pool.push(1, tmp_pld);
    }

    // outer payload pool
    in_pool.init(_pool_size);
    in_pool.construct_objects();
    // set raw buffer pointers
    for(unsigned int i = 0; i<_pool_size; i++){
	in_pool.get_constructed(i)->raw = raw_buff.get_chunk(i + _pool_size);
    }


    // set outer queue capacity
    in_q.set_capacity(_pool_size);

    // set capacities of local spsc queues
    rx_tx_q.init(_pool_size);
    rx_proc_q.init(_pool_size);
    proc_tx_q.init(_pool_size * 2);

    // register stats
    ConnectionBase::stats.register_item(CST_RX_ERROR);
    ConnectionBase::stats.register_item(CST_TX_ERROR);
    ConnectionBase::stats.register_item(CST_RX_MSU_COUNT);
    ConnectionBase::stats.register_item(CST_TX_MSU_COUNT);




}

template<typename PLD_TYPE>
sgn::Connection<PLD_TYPE>::~Connection(){
    stop();

    // process queues, safe since all threads have finished
    PLD_TYPE* pld = NULL;
    Payload* uni_pld = NULL;
    PminkPayload* pmpld = NULL;

    // check rx_tx q
    while(rx_tx_q.pop(&pld) == 0) pld_pool.push(1, pld);
    // check rx_proc q
    while(rx_proc_q.pop(&pld) == 0) pld_pool.push(1, pld);

    // check proc_tx q
    while(proc_tx_q.pop(&uni_pld) == 0){
	switch(uni_pld->type){
	    case CT_PMINK:
		in_pool.deallocate_constructed((PminkPayload*)uni_pld);
		break;

	    default:
		pld_pool.push(1, (PLD_TYPE*)uni_pld);
		break;
	}
    }

    // check external in q
    while(in_q.pop(&pmpld)) in_pool.deallocate_constructed(pmpld);

}

template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::start(){
    rx_on_start();
    tx_on_start();
    // client threads active
    cthreads_active.set(true);
    // base start
    ConnectionBase::start();
    // ok
    return 0;
}

template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::stop(){
    // client threads not active
    cthreads_active.set(false);
    thm.stop();
    // base stop
    ConnectionBase::stop();
    return 0;
}

template<typename PLD_TYPE>
bool sgn::Connection<PLD_TYPE>::is_garbage(){
    return !cthreads_active.get();
}

template<typename PLD_TYPE>
memory::SpscQPool<PLD_TYPE>* sgn::Connection<PLD_TYPE>::get_pld_pool(){
    return &this->Connection<PLD_TYPE>::pld_pool;
}


template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::create_client(int _client_socket, ConnectionBase** new_conn){
    return 0;
}


template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::rx_on_start(){
    return 0;
}

template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::tx_on_start(){
    return 0;
}

template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::timer_event(){
    return 0;
}

template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::reconnect_logic(){
    // connection shutting down
    if(!ConnectionBase::active.get()) return 10;
    if(!cthreads_active.get()) return 10;
    if(!reconnecting.get()) return 10;

    // check timer, pause if needed
    int tdiff = time(NULL) - reconnect_ts;
    if(tdiff < reconnect_timer) return 10;

    // reconnect
    stats.inc(CST_RECONNECT, 1);
    reconnect();
    // check if socket valid, set flag
    if(client_socket != -1) reconnecting.comp_swap(true, false);
    // update ts
    reconnect_ts = time(NULL);

    return 10;
}

template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::proc_logic(){
    // connection shutting down
    if(!ConnectionBase::active.get()) return 10;
    if(!cthreads_active.get()) return 10;
    if(reconnecting.get()) return 10;

    PLD_TYPE* pld = NULL;
    PminkPayload* pmpld = NULL;
    bool idle = true;


    // reset idle
    idle = true;
    // check rx_proc queue
    if(rx_proc_q.pop(&pld) == 0){
	idle = false;

	// process
	if(pld->action == PAT_PROCESS) {
	    proc_process(pld);
	}

	// free pld
	pld->action = PAT_FREE;
	proc_tx_q.push(1, pld);
    }

    // check external in queue
    if(in_q.pop(&pmpld)){
	idle = false;
	// check action
	switch(pmpld->action){
	    case PAT_PROCESS:
		proc_process_ext(pmpld);
		break;

	    case PAT_FREE:
		in_pool.deallocate_constructed(pmpld);
		break;

	    default: break;
	}
    }

    // no packets, sleep
    if(idle) return 10;


    return 0;
}


template<typename PLD_TYPE>
int  sgn::Connection<PLD_TYPE>::extra_logic(){
    // sleep
    return 10;
}

template<typename PLD_TYPE>
int  sgn::Connection<PLD_TYPE>::pcap_logic(){
    // sleep
    return 10;
}

template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::server_logic(int poll_res, short int poll_revents, int* poll_socket, ConnectionBase** newconn){
    // check if server accepting connection
    if(!ConnectionBase::active.get()) return 10;
    if(cthreads_active.get()) return 10;

    // check socket
    if(server_socket == -1){
	// restart server
	reconnect();
	*poll_socket = server_socket;
	return 10;

    }
    // check for timeout
    if(poll_res > 0){
	// check for POLLIN event
	if((poll_revents & POLLIN) == POLLIN){
	    // get connected client
	    client_socket = server_get_client(server_socket);
	    // sanity check
	    if(client_socket == -1) return -1;
	    // stats
	    switch(type){
		case CT_SCTP:
		case CT_M3UA:
		    stats.set(CST_SCTP_ACTIVE, 1);
		    break;

		case CT_TCP:
		case CT_SMPP:
		    stats.inc(CST_TCP_ACTIVE, 1);
		    break;

		default: break;
	    }

	    // handle one to many connection
	    if(max_in_connc > 1){
		create_client(client_socket, newconn);
		return 30;

	    // handle one to one connection
	    }else{
		// finished, start RX/TX threads (call base method)
		Connection::start();
		*newconn = this;
		return 40;

	    }


	// reconnect (POLLERR, POLLHUP or POLLNVAL received)
	// poll_revents == 0 means NO EVENT
	}else if(poll_revents != 0){
	    // restart server
	    reconnect();
	    *poll_socket = server_socket;
	    return 10;
	}

    // socket idle or error
    }else{
	// idle
	if(poll_res == 0){
	    return 10;

	// error
	}else{
	    // error, restart server
	    reconnect();
	    *poll_socket = server_socket;
	    return 10;

	}

    }

    return 10;
}

template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::tx_logic(){
    // connection shutting down
    if(!ConnectionBase::active.get()) return 10;
    if(!cthreads_active.get()) return 10;
    if(reconnecting.get()) return 10;

    PLD_TYPE* pld = NULL;
    Payload* uni_pld = NULL;
    bool idle = true;
    pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;


    // reset idle
    idle = true;
    // check rx_tx q
    if(rx_tx_q.pop(&pld) == 0){
	// not idling
	idle = false;
	// check payload action type
	switch(pld->action){
	    // send
	    case PAT_SEND:
		// send
		if(data_tx(pld) != 0){
		    // stats
		    stats.inc(CST_TX_ERROR, 1);
		    // queue reconnection
		    reconnect_queued.comp_swap(false, true);

		}else{
		    // stats
		    stats.inc(CST_TX_MSU_COUNT, 1);

		}
		// return to pool
		pld_pool.push(1, pld);
		break;

	    // return to pool
	    case PAT_FREE:
		pld_pool.push(1, pld);
		break;

	    // skip other types
	    default:
		break;
	}

    }

    // check proc_tx_q
    if(proc_tx_q.pop(&uni_pld) == 0){
	// not idling
	idle = false;

	// check for pmink payload type
	if(uni_pld->type == CT_PMINK){
	    // check payload action type
	    switch(uni_pld->action){
		// send
		case PAT_SEND:
		    // send error
		    if(data_tx(uni_pld) != 0){
			// stats
			stats.inc(CST_TX_ERROR, 1);
			// queue reconnection
			reconnect_queued.comp_swap(false, true);
			// run event handler
			e_args.add_arg(CEAT_PMPLD, uni_pld);
			// handler found do not return to pool
			// further action should be handled in event handler
			if(e_handler.run_handler(CET_SEND_ERROR, e_args) == 0) break;

		    // sent ok
		    }else{
			// stats
			stats.inc(CST_TX_MSU_COUNT, 1);

		    }
		    // return to pool
		    in_pool.deallocate_constructed((PminkPayload*)uni_pld);
		    break;

		case PAT_FREE:
		    in_pool.deallocate_constructed((PminkPayload*)uni_pld);
		    break;

		// skip other types
		default:
		    break;

	    }

	// other internal payloads (m3ua, smpp)
	}else{
	    // check payload action type
	    switch(uni_pld->action){
		// return to pool
		case PAT_FREE:
		    pld_pool.push(1, (PLD_TYPE*)uni_pld);
		    break;

		// skip other types
		default:
		    break;

	    }

	}
    }

    // sleep if idle
    if(idle) return 10;


    return 0;
}


template<typename PLD_TYPE>
int sgn::Connection<PLD_TYPE>::rx_logic(int poll_res, short int poll_revents, int* poll_socket){
    // connection shutting down
    if(!ConnectionBase::active.get()) return 10;
    if(!cthreads_active.get()) return 10;
    if(reconnecting.get()) return 10;

    int res;
    pmink_utils::EventArgs<ConnectiontEventArgType, void*> e_args;
    unsigned char recv_buff[65535];
    int bytes_rcvd = 0;
    int data_type = 0;
    PLD_TYPE pld;
    // add conn to e_args
    e_args.add_arg(CEAT_CONNECTION, this);

    // update socket (can change in reconnect_logic thread)
    *poll_socket = client_socket;

    // check if reconnection was queued
    if(reconnect_queued.get()) {
	// run event handler
	e_handler.run_handler(CET_CONNECTION_DOWN, e_args);
	reconnecting.set(true);
	// reset reconnection flag
	reconnect_queued.comp_swap(true, false);
	return 10;
    }

    // timer event
    timer_event();

    // socket active
    if(poll_res > 0){
	// reconnect if socket down
	if(client_socket == -1){
	    // run event handler
	    e_handler.run_handler(CET_CONNECTION_DOWN, e_args);
	    reconnecting.set(true);
	    return 10;
	}

	// check for POLLIN event
	if((poll_revents & POLLIN) == POLLIN){
	    res = data_rx(recv_buff, sizeof(recv_buff), &bytes_rcvd, &data_type, pld.extra);
	    // data received
	    if(res == 0){
		data_rx_process(recv_buff, bytes_rcvd, data_type, pld.extra);
		// stats
		if(bytes_rcvd > 0) stats.inc(CST_RX_MSU_COUNT, 1);
		return 0;

	    }else{
		// stats
		stats.inc(CST_RX_ERROR, 1);
		// run event handler
		e_handler.run_handler(CET_CONNECTION_DOWN, e_args);
		reconnecting.set(true);
		return 10;

	    }

	// reconnect (POLLERR, POLLHUP or POLLNVAL received)
	// poll_revents == 0 means NO EVENT
	}else if(poll_revents != 0){
	    // run event handler
	    e_handler.run_handler(CET_CONNECTION_DOWN, e_args);
	    reconnecting.set(true);
	    return 10;

	}

    // socket inactive(timeout or error)
    }else{
	// idle
	if(poll_res == 0){
	    // reconnect if socket down
	    if(client_socket == -1){
		// run event handler
		e_handler.run_handler(CET_CONNECTION_DOWN, e_args);
		reconnecting.set(true);
		return 10;
	    }

	// error
	}else{
	    // run event handler
	    e_handler.run_handler(CET_CONNECTION_DOWN, e_args);
	    reconnecting.set(true);
	    return 10;

	}

    }


    return 10;
}

// PcapSmppFraming
template<typename PLD_TYPE>
sgn::PcapSmppFraming<PLD_TYPE>::PcapSmppFraming(_PCAPConnection<PLD_TYPE>* _conn): conn(_conn){}

template<typename PLD_TYPE>
int sgn::PcapSmppFraming<PLD_TYPE>::on_pdu_ready(unsigned char* pdu, unsigned int pdu_size, void* args){
    // tmp pointers
    SmppPayload* smpp_pld = NULL;
    SMPPConnection* smpp_conn = conn->smpp_conn;
    // get smpp conn internal payload (pld_pool)
    if(smpp_conn->get_pld_pool()->pop(&smpp_pld) != 0){
        // stats
        smpp_conn->stats.inc(CST_PLD_POOL_EMPTY, 1);
        return 1;
    }
    // copy packet data
    memcpy(smpp_pld->raw, pdu, pdu_size);
    smpp_pld->size = pdu_size;
    // process smpp (try to decode)
    int res = smpp_conn->proc_process(smpp_pld);
    // update stats on error
    if(res != 0) smpp_conn->stats.inc(CST_MALFORMED_MSU, 1);
    // free payload
    smpp_conn->get_pld_pool()->push(1, smpp_pld);
    // return res
    return res;
}

// PCAPConnection
sgn::PCAPConnection::PCAPConnection(ConnectionDirection _direction,
				    ServerConnectionType _server_type,
				    const char* _label,
				    int _max_mtu,
				    int _pool_size): _PCAPConnection<PcapPayload>(_direction,
										  _server_type,
										  _label,
										  _max_mtu,
										  _pool_size){

}
// _PCAPConnection
template<typename PLD_TYPE>
sgn::_PCAPConnection<PLD_TYPE>::_PCAPConnection(ConnectionDirection _direction,
						ServerConnectionType _server_type,
			   		        const char* _label,
						int _max_mtu,
						int _pool_size): Connection<PLD_TYPE>(CT_PCAP,
										      _direction,
										      _server_type,
										      _label,
										      _max_mtu,
										      _pool_size),
								 pcap_alloc_p(NULL),
								 pcap_dealloc_p(NULL),
								 terminated(false),
                                                                 pcap_type(0),
								 sd(NULL),
                                                                 reasm_buff_l(0),
                                                                 tcp_exp_ts(time(NULL)){

    // m3ua child conn
    m3ua_conn = new M3UAConnection(CD_CLIENT,
				   SCT_ONE_ONE,
				   std::string(ConnectionBase::label + "_m3ua").c_str(),
				   _max_mtu,
				   1);
    // override m3ua connection type
    m3ua_conn->type = CT_PCAP_M3UA;

    // smpp child conn
    smpp_conn = new SMPPConnection(CD_CLIENT,
                                   SCT_ONE_MANY,
                                   std::string(ConnectionBase::label + "_smpp").c_str(),
                                   _max_mtu,
                                   1);
    // override m3ua connection type
    smpp_conn->type = CT_PCAP_SMPP;

    // register stats
    ConnectionBase::stats.register_item(CST_MTU_LIMIT);
    ConnectionBase::stats.register_item(CST_DISCARDED_MSU);
    ConnectionBase::stats.register_item(CST_PLD_POOL_EMPTY);
    ConnectionBase::stats.register_item(CST_MALFORMED_MSU);
    ConnectionBase::stats.register_item(CST_TCP_CONNS);
    ConnectionBase::stats.register_item(CST_TCP_MAX_CONNS);
    ConnectionBase::stats.register_item(CST_TCP_INVALID_SEQ);
    ConnectionBase::stats.register_item(CST_TCP_MAX_SEGMS);

    // get pcap alloc/dealloc function pointers
    pcap_alloc_p = (pd_alloc_t)pmink::CURRENT_DAEMON->get_param(5);
    pcap_dealloc_p = (pd_dealloc_t)pmink::CURRENT_DAEMON->get_param(6);
    // get pcap start/finish function pointers
    pcap_start_p = (pd_start_t)pmink::CURRENT_DAEMON->get_param(7);
    pcap_finish_p = (pd_finish_t)pmink::CURRENT_DAEMON->get_param(8);

    // sctp pool
    sctp_chunk_pool.set_pool_size(100);
    sctp_chunk_pool.init_pool();

    // set raw buffer pointers for packet_descriptor
    PLD_TYPE* tmp_pld = NULL;
    for(unsigned int i = 0; i<_pool_size; i++){
	Connection<PLD_TYPE>::pld_pool.pop(&tmp_pld);
	tmp_pld->pd.data.packet = tmp_pld->raw;
	tmp_pld->pd.data.args = tmp_pld;
	Connection<PLD_TYPE>::pld_pool.push(1, tmp_pld);
    }

}

template<typename PLD_TYPE>
sgn::_PCAPConnection<PLD_TYPE>::~_PCAPConnection(){
    // terminate and stop
    terminate();
    stop();
    // flag m3ua child conn for destruction
    m3ua_conn->refc_dec();
    // flag smpp child conn for destruction
    smpp_conn->refc_dec();
    // free logic
    delete ConnectionBase::logic;

}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::data_rx(unsigned char* buff, int size, int* bytes_rvcd, int* pld_type, void* extra){
    return 0;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::data_tx(Payload* data){
    return 0;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::data_rx_process(unsigned char* buff, int size, int data_type, void* extra){
    return 0;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::proc_process(PLD_TYPE* pld){
    return 0;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::proc_process_ext(PminkPayload* pmpld){
    return 0;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::server_get_client(int _server_socket){
    return 0;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::reconnect(){
    return 0;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::convert(PminkPayload* pmpld){
    return 0;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::cmd_process(PminkPayload* pmpld, int cmd_id){
    return 0;
}

template<typename PLD_TYPE>
void sgn::_PCAPConnection<PLD_TYPE>::free_tcp_segm_args(pmink_net::TcpTracker::args_vec_t* lst){
    // free PcapPayload pointers buffered in segment
    for(pmink_net::TcpTracker::args_vec_t::const_iterator it = lst->begin();
        it != lst->end();
        it++){
        // free
        Connection<PLD_TYPE>::pld_pool.push(1, (PLD_TYPE*)*it);
    }
    lst->clear();
}

template<typename PLD_TYPE>
void sgn::_PCAPConnection<PLD_TYPE>::free_tcp_strm_args(pmink_net::TcpTracker::args_vec_t* lst){
    // free PcapPayload pointers buffered in segment
    for(pmink_net::TcpTracker::args_vec_t::const_iterator it = lst->begin();
        it != lst->end();
        it++){
        // free
        delete (pcap_smpp_frm_t*)*it;
    }
    lst->clear();
}


template<typename PLD_TYPE>
void sgn::_PCAPConnection<PLD_TYPE>::tcp_conn_expire(pmink_net::TcpConnection* tcp_conn){
    // expire tcp connection
    if(tcp_tracker.expire(tcp_conn, &segm_args_lst, &strm_args_lst) == 0){
        // free segment args
        free_tcp_segm_args(&segm_args_lst);
        // free stream args
        free_tcp_strm_args(&strm_args_lst);
    }
}


template<typename PLD_TYPE>
void sgn::_PCAPConnection<PLD_TYPE>::tcp_expire_finished(){
    // expire, check if removed connection count is > 0
    if(tcp_tracker.expire(&segm_args_lst, &strm_args_lst) > 0){
        // free segment args
        free_tcp_segm_args(&segm_args_lst);
        // free stream args
        free_tcp_strm_args(&strm_args_lst);
    }
}


template<typename PLD_TYPE>
pmink_net::TcpTracker*  sgn::_PCAPConnection<PLD_TYPE>::get_tcp_tracker(){
    return &tcp_tracker;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::pcap_logic(){
    packet_pitem_t* pitem = NULL;
    packet_descriptor* pd = NULL;
    unsigned char* payload = NULL;
    unsigned int payload_length = 0;
    sctp::Data* data_chunk = NULL;
    sctp::Chunk* sctp_chunk = NULL;
    M3uaPayload* m3ua_pld = NULL;
    SmppPayload* smpp_pld = NULL;

    // get paket from capture queue
    if(sd->lfq.pop(&pitem) == 0){
	// stats
	ConnectionBase::stats.inc(CST_RX_MSU_COUNT, 1);
	// pd pointer
	pd = &pitem->data;

	// decode eth header
	if(tcpip::get_eth_header(pd->packet, pd->caplen, &ethh) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);
	    // free
	    Connection<PLD_TYPE>::pld_pool.push(1, (PLD_TYPE*)pitem->data.args);
	    return 0;

	}

	// reset payload pointer
	payload = NULL;

	// ip or vlan
	switch(ethh.ether_type){
	    // ipv4
	    case tcpip::ETH_ET_IPv4:
		payload = ethh.payload;
		payload_length = ethh.payload_length;
		break;

	    // vlan
	    case tcpip::ETH_ET_VLAN:
		if(tcpip::get_vlan_header(ethh.payload, ethh.payload_length, &vlanh) == 0){
		    if(vlanh.ether_type == tcpip::ETH_ET_IPv4){
			payload = vlanh.payload;
			payload_length = vlanh.payload_length;
		    }

		}
		break;

	    // unknown
	    default:
		break;
	}

	// eth payload error check
	if(payload == NULL){
	    // stats
	    ConnectionBase::stats.inc(CST_DISCARDED_MSU, 1);
	    // free
	    Connection<PLD_TYPE>::pld_pool.push(1, (PLD_TYPE*)pitem->data.args);
	    return 0;
	}
	// iph
	if(tcpip::get_ip_header(payload, payload_length, &iph) != 0){
	    // stats
	    ConnectionBase::stats.inc(CST_MALFORMED_MSU, 1);
	    // free
	    Connection<PLD_TYPE>::pld_pool.push(1, (PLD_TYPE*)pitem->data.args);
	    return 0;

	}
	// skip fragmented
	if(!(!iph.more_fragments && iph.fragment_offset == 0)){
	    // stats
	    ConnectionBase::stats.inc(CST_DISCARDED_MSU, 1);
	    // free
	    Connection<PLD_TYPE>::pld_pool.push(1, (PLD_TYPE*)pitem->data.args);
	    return 0;

	}
	// SCTP/TCP
	if(!(iph.protocol == tcpip::IP_PT_SCTP || iph.protocol == tcpip::IP_PT_TCP)) {
	    // stats
	    ConnectionBase::stats.inc(CST_DISCARDED_MSU, 1);
	    // free
	    Connection<PLD_TYPE>::pld_pool.push(1, (PLD_TYPE*)pitem->data.args);
	    return 0;

	}
        // SCTP or TCP
        switch(iph.protocol){
            // sctp (m3ua)
            case tcpip::IP_PT_SCTP:
            {
                // check pcap type
                if(!(pcap_type == PT_M3UA || pcap_type == PT_M3UA_SMPP)) break;
                // decode sctp
                sctp::decode(iph.payload, iph.payload_length, &sctpp, &sctp_chunk_pool);
                // loop chunks
                for(unsigned int j = 0; j<sctpp.chunks.size(); j++){
                    // get chunk
                    sctp_chunk = sctpp.chunks[j];
                    // only process DATA chunks
                    if(sctp_chunk->type != sctp::DATA) continue;
                    // get data chunk
                    data_chunk = (sctp::Data*)sctp_chunk;
                    // skip fragmented SCTP DATA chunks
                    if(!(data_chunk->B_bit && data_chunk->E_bit)) continue;
                    // only process M3UA ppid
                    if(data_chunk->payload_protocol_type != sctp::M3UA) continue;
                    // get m3ua conn internal payload (pld_pool)
                    if(m3ua_conn->get_pld_pool()->pop(&m3ua_pld) != 0){
                        // stats
                        m3ua_conn->stats.inc(CST_PLD_POOL_EMPTY, 1);
                        continue;
                    }
                    // copy packet data
                    memcpy(m3ua_pld->raw, data_chunk->user_data, data_chunk->user_data_length);
                    m3ua_pld->size = data_chunk->user_data_length;
                    // process m3ua
                    if(m3ua_conn->proc_process(m3ua_pld) != 0){
                        // stats
                        m3ua_conn->stats.inc(CST_MALFORMED_MSU, 1);
                    }
                    // return internal payload to pool (pld_pool)
                    m3ua_conn->get_pld_pool()->push(1, m3ua_pld);
                }
                break;
            }
            // tcp (smpp)
            case tcpip::IP_PT_TCP:
            {
                // check pcap type
                if(!(pcap_type == PT_SMPP || pcap_type == PT_M3UA_SMPP)) break;
                // tcp decode
                if(pmink_net::tcp_decode(iph.payload, iph.payload_length, &tcph) > 0) break;

                // check if TCP timeout interval has passed
                if(time(NULL) - tcp_exp_ts > tcp_tracker.get_timeout()){
                    // expire finished and timed out
                    tcp_expire_finished();
                    // update tcp exp ts
                    tcp_exp_ts = time(NULL);
                }
                // run tcp tracker
                pmink_net::TcpTracker::process_res_t res = tcp_tracker.process(iph.vesion, 
                                                                               iph.source, 
                                                                               iph.destination, 
                                                                               &tcph, 
                                                                               pitem->data.args);
                // update stats
                ConnectionBase::stats.set(CST_TCP_CONNS, tcp_tracker.size());

                // check error code
                switch(res.second){
                    //+-----------------------------------------------------+
                    //| * EC_IP_VER_ERR will never occur, it is handled in  |
                    //|   get_ip_header() method                            |
                    //+-----------------------------------------------------+
                    case pmink_net::TcpTracker::EC_IP_VER_ERR:
                        break;

                    //+-----------------------------------------------------+
                    //| SYN_ONLY = true                                     |
                    //| ===============                                     |
                    //| * EC_SKIPPED occurs when SYN_ONLY flag is set and   |
                    //|   current segment does not start a new connection   |
                    //|   since SYN flag is not present; current connection |
                    //|   will not be tracked                               |
                    //|                                                     |
                    //| SYN_ONLY = false                                    |
                    //| ================                                    |
                    //| * when SYN_ONLY flag is not set we are trying to    |
                    //|   track ALL TCP connections, even those that have   |
                    //|   already been established (mid-stream, no SYN)     |
                    //| * EC_SKIPPED occurs when the following conditions   |
                    //|   are met (the connection will not be tracked):     |
                    //|     1. segment contains FIN or RST flag             |
                    //|     2. segment contains NO payload                  |
                    //+-----------------------------------------------------+
                    case pmink_net::TcpTracker::EC_SKIPPED:
                        break;

                    // max number of tracked connections reached
                    case pmink_net::TcpTracker::EC_MAX_CONNECTIONS:
                        // stats
                        ConnectionBase::stats.inc(CST_TCP_MAX_CONNS, 1); 
                        // remove expired and finished connections
                        tcp_expire_finished();
                        break;

                    // invalid sequence number received
                    case pmink_net::TcpTracker::EC_INVALID_SEQUENCE:
                        // stats
                        ConnectionBase::stats.inc(CST_TCP_INVALID_SEQ, 1); 
                        // expire tcp conn
                        tcp_conn_expire(res.first);
                        break;
                    
                    //+------------------------------------------------------+
                    //| * EC_NOT_CONNECTED will never occur when SYN_ONLY is |
                    //|   set to false (in this case)                        |
                    //| * when SYN_ONLY is true, EC_NOT_CONNECTED will occur |
                    //|   when current stream is still waiting for SYN       |
                    //| * EC_NEW_CONNECTION marks a newly tracked connection |
                    //|   that has just been detected (SYN only)             |
                    //|   and contains NO payload                            |
                    //+------------------------------------------------------+
                    case pmink_net::TcpTracker::EC_NEW_CONNECTION:
                    case pmink_net::TcpTracker::EC_NOT_CONNECTED:
                        break;

                    //+----------------------------------------------------+
                    //| * too many buffered segments, reassemble()         |
                    //|   cannot reassemble the stream and remove buffered |
                    //|   segments if some sequnce numbers are missing     |
                    //| * this can happen due to bad connection or pcap    |
                    //|   packet loss                                      |
                    //+----------------------------------------------------+
                    case pmink_net::TcpTracker::EC_MAX_SEGMENTS:
                        // stats
                        ConnectionBase::stats.inc(CST_TCP_MAX_SEGMS, 1); 
                        // expire tcp conn
                        tcp_conn_expire(res.first);
                        break;
                   
                    //+--------------------------------------------------------------+
                    //| * EC_NO_PAYLOAD occurs when detected TCP segment contains no |
                    //|   payload and it belongs to existing(already tracked)        |
                    //|   TCP connection                                             |
                    //+--------------------------------------------------------------+
                    case pmink_net::TcpTracker::EC_NO_PAYLOAD:
                        // expire tcp conn if both streams have finished
                        if(res.first->finished()) tcp_conn_expire(res.first);
                        break;

                    //+------------------------------------------------------------+
                    //| * EC_NEW_SEGMENT occurs when detected TCP segment contains |
                    //|   a payload; when SYN_ONLY is set to true, only existing   |
                    //|   connections that have already been established           |
                    //|   (SYN, SYN-ACK, ACK) can return this code since packets   |
                    //|   with SYN flag (3-way tcp handshake) don't carry payload  |
                    //| * when SYN_ONLY is set to false, connection tracking can   |
                    //|   start mid-stream and completely miss the entire TCP      |
                    //|   handshake procedure (SYN, SYN-ACK, ACK)                  |
                    //| * in case of mid-stream tracking, the first detected       |
                    //|   TCP segment will have the same effect as a regular SYN   |
                    //|   packet with a minor difference; unlike the regular SYN   |
                    //|   segment, this fake-SYN segment MUST carry a payload and  |
                    //|   therefore EC_NEW_SEGMENT can also occur for newly        |
                    //|   detected TCP connections                                 |
                    //+------------------------------------------------------------+
                    case pmink_net::TcpTracker::EC_NEW_SEGMENT:
                    {
                        // conn pointer
                        pmink_net::TcpConnection* tcp_conn = res.first;
                        // check if payload type detected as SMPP
                        if(tcp_conn->pld_type == CT_SMPP){
                            // reassemble available segments
                            int reasm_res = tcp_tracker.reassemble(tcp_conn->strm, 
                                                                   reasm_buff, 
                                                                   sizeof(reasm_buff), 
                                                                   &segm_args_lst);
                            // check for successful reassemble
                            if(reasm_res > 0){
                                // free PcapPayload pointer buffered in segment,
                                // current segment data reassembled in reasm_buff
                                free_tcp_segm_args(&segm_args_lst);
                                // current TCP stream pointer (IN/OUT)
                                pmink_net::TcpStream* strm = tcp_conn->strm;
                                // get SMPP TCP framing obj pointer
                                pcap_smpp_frm_t* smpp_frm = (pcap_smpp_frm_t*)strm->args;
                                // proces with SMPP TCP framing obj
                                smpp_frm->process(reasm_buff, reasm_res);
                            }

                        // SMPP payload still not detected, try payload detection if number of
                        // attempts is < 5
                        }else if(tcp_conn->pld_type > -5){
                            //+-----------------------------------------------------------+
                            //| * process each segment separately, try to detect SMPP PDU |
                            //| * when SMPP PDU is detected, further sugments will be     |
                            //|   processed correclty, as a TCP stream                    |
                            //| * this is needed if new TCP stream was detected           |
                            //|   mid-stream (no SYN); in this case, SMPP TCP framing     |
                            //|   might not start from the begining of PDU and could      |
                            //|   potentially cause ALL following PDU's to be skipped     |
                            //|   due to inability to properly detect PDU boundary        |
                            //| * when TCP segment is properly decoded as SMPP PDU,       |
                            //|   we can start normal TCP streaming/framing since PDU     |
                            //|   boundary was properly detected                          |
                            //+-----------------------------------------------------------+
                            
                            //+-------------------------------------------------------------+
                            //| * use connection's pld_type as a counter (dec by -1 on each |
                            //|   payload detection attempt)                                |
                            //| * if more then 5 unsuccessfull atempts have been made       |
                            //|   and payload type is still unknown, current TCP connection |
                            //|   will be tracked but ignored                               |
                            //+-------------------------------------------------------------+
                            --tcp_conn->pld_type;

                            // loop buffered segments
                            while(tcp_conn->strm->segments.size() > 0){
                                // reassemble only one segment (limit = 1)
                                int reasm_res = tcp_tracker.reassemble(tcp_conn->strm, 
                                                                       reasm_buff, 
                                                                       sizeof(reasm_buff), 
                                                                       &segm_args_lst, 
                                                                       1);
                                // break segment loop if error returned  to avoid
                                // infinite loop; reassemble() does not remove
                                // processed segment(s) if it returns with an 
                                // error (negative value) or with 0 (out buffer too small)
                                if(reasm_res <= 0) break;
                                // free PcapPayload pointer buffered in segment,
                                // current segment data reassembled in reasm_buff
                                free_tcp_segm_args(&segm_args_lst);
                                // check if number of bytes in segment are >= 16 (SMPP PDU min)
                                if(reasm_res < 16) continue;
                                // check for buffer overflow (max_mtu)
                                if(reasm_res > Connection<PLD_TYPE>::max_mtu){
                                    // stats
                                    smpp_conn->stats.inc(CST_MTU_LIMIT, 1);
                                    continue;
                                }
                                // pointer to smpp pdu l part (first 4 bytes)
                                uint32_t* tmp_pdu_l = (uint32_t*)reasm_buff;
                                // check if pdu length and segment length match
                                if(reasm_res == be32toh(*tmp_pdu_l)){
                                    // get smpp conn internal payload (pld_pool)
                                    if(smpp_conn->get_pld_pool()->pop(&smpp_pld) != 0){
                                        // stats
                                        smpp_conn->stats.inc(CST_PLD_POOL_EMPTY, 1);
                                        continue;
                                    }
                                    // copy packet data
                                    memcpy(smpp_pld->raw, reasm_buff, reasm_res);
                                    smpp_pld->size = reasm_res;
                                    // process smpp (try to decode)
                                    if(smpp_conn->proc_process(smpp_pld) == 0){
                                        // smpp decoded without errors, set pld type
                                        tcp_conn->pld_type = CT_SMPP;
                                        // +------------------------------------------------+
                                        // | * setup smpp tcp framing, normal tcp streaming |
                                        // |   is enabled from now on                       |
                                        // +------------------------------------------------+
                                        // set user args for tcp connection's IN/OUT streams
                                        tcp_conn->in_strm.args = new pcap_smpp_frm_t(this);
                                        tcp_conn->out_strm.args = new pcap_smpp_frm_t(this);
                                    }

                                    // return internal payload to pool (pld_pool)
                                    smpp_conn->get_pld_pool()->push(1, smpp_pld);
                                }
                            }
                        }
                        // return instead of break to avoid freeing PcapPayload (pitem->data.args) 
                        // since it is buffered as part of tcp segment (EC_NEW_SEGMENT return code)
                        return 0;
                    }

                    default:
                        break;

                }
                break;
            }

            default:
                // stats
                ConnectionBase::stats.inc(CST_DISCARDED_MSU, 1);
                break;
        }

	// free
	Connection<PLD_TYPE>::pld_pool.push(1, (PLD_TYPE*)pitem->data.args);
	// ok
	return 0;

    }
    // idle
    return 10;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::start(){
    // pcap type
    pcap_type = (int)*ConnectionBase::params.get_param(CPT_PCAP_TYPE);
    // init capture
    sd = PMPcap::initCapture((char*)*ConnectionBase::params.get_param(CPT_PCAP_INTERFACE),
			     Connection<PLD_TYPE>::max_mtu,
			     Connection<PLD_TYPE>::pool_size,
			     Connection<PLD_TYPE>::pool_size,
			     (int)*ConnectionBase::params.get_param(CPT_PCAP_PCAP_BUFFER),
			     pcap_alloc_p,
			     pcap_dealloc_p,
			     this,
			     pcap_start_p,
			     pcap_finish_p);

    if(sd == NULL) return 1;
    // refc inc
    sd->refc.add_fetch(1);
    // m3ua child r14p stats pointer
    m3ua_conn->params.set_pointer(sgn::CPT_R14P_STATS, (void*)*ConnectionBase::params.get_param(sgn::CPT_R14P_STATS));
    // start capture
    PMPcap::startCapture(sd->id);
    // base start
    Connection<PLD_TYPE>::start();
    // add extra stats
    pmink_utils::VariantParam* vp = ConnectionBase::params.get_param(CPT_R14P_STATS);
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)(void*)*vp;
    std::string stats_prefix("PCAP::");
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_PCAP_RX")), new sgn::PCAPStatsHandlerRX(sd->id));
    r14p_stats->add_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_PCAP_DROP")), new sgn::PCAPStatsHandlerDROP(sd->id));

    // ok
    return 0;
}

template<typename PLD_TYPE>
int sgn::_PCAPConnection<PLD_TYPE>::stop(){
    // base stop
    Connection<PLD_TYPE>::stop();
    // stop capture
    if(sd != NULL){
	// remove extra stats
	pmink_utils::VariantParam* vp = ConnectionBase::params.get_param(CPT_R14P_STATS);
	r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)(void*)*vp;
	std::string stats_prefix("PCAP::");
	delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_PCAP_RX")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_PCAP_DROP")));

	// dec sd ref count
	sd->refc.sub_fetch(1);
	// stop capture
	PMPcap::stopCapture(sd->id);
	sd = NULL;
	// NOTE
        //+--------------------------------------------------------------------------------+
        //|  * sd->lfq contains pld_pool items acquired by pd_alloc method          	   |
        //|  * pld_pool items (PcapPayload pointers) are pointers to POD type objects      |
        //|    contained in connection's raw_buff memory buffer (freed when connection     |
        //|    gets destroyed)                                                             |
        //|  * sd->lfq can contain pld_pool items at the time of destruction        	   |
        //|    and that is acceptable since POD type objects don't have destructors;       |
        //|    when pld_pool gets destroyed, all items currently present in the pool       |
        //|    will have their destructors called as well (not in this case since pld_pool |
        //|    items are POD type objects)                                                 |
        //|  * pld_pool can therefore get destroyed and deallocated even with sd->lfq      |
        //|    still containing some pld_pool items                                        |
        //|  * items still left in sd->lfq will be dangling pointers pointing      	   |
        //|    to connection's raw_buff memory buffer which was freed                      |
        //|  * sd->lfq destructor will not dereference those dangling pointers             |
        //|  * this simplifies termination procedure since no action is required           |
        //|    for handling sd->lfq items          	                                   |
        //+--------------------------------------------------------------------------------+
    }
    return 0;
}

template<typename PLD_TYPE>
void sgn::_PCAPConnection<PLD_TYPE>::terminate(){
    if(terminated) return;

    pmink_utils::VariantParam* vp = ConnectionBase::params.get_param(CPT_R14P_STATS);
    if(vp == NULL) return;
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)(void*)*vp;

    // remove stats handlers
    std::string stats_prefix("PCAP::");
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_RX_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_MTU_LIMIT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_DISCARDED_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_PLD_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_MALFORMED_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TCP_CONNECTIONS")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TCP_MAX_CONNECTIONS")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TCP_INVALID_SEQ")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_TCP_MAX_SEGMS")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_M3UA_MALFORMED_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_M3UA_PLD_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_M3UA_PMPLD_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_M3UA_SMSG_SENT_COUNT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_M3UA_SMSG_SEND_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_M3UA_SMSG_MSG_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_M3UA_SMSG_PARAM_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMPP_MALFORMED_MSU")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMPP_PLD_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMPP_PMPLD_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMPP_SMSG_SENT_COUNT")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMPP_SMSG_SEND_ERROR")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMPP_SMSG_MSG_POOL_EMPTY")));
    delete r14p_stats->remove_trap(r14p::TrapId(std::string(stats_prefix + ConnectionBase::label + "_SMPP_SMSG_PARAM_POOL_EMPTY")));

    terminated = true;


}

template<typename PLD_TYPE>
sgn::M3UAConnection* sgn::_PCAPConnection<PLD_TYPE>::get_m3ua_conn(){
    return m3ua_conn;
}

template<typename PLD_TYPE>
sgn::SMPPConnection* sgn::_PCAPConnection<PLD_TYPE>::get_smpp_conn(){
    return smpp_conn;
}
void sgn::ServiceMessageAsyncDone::run(r14p::R14PCallbackArgs* args){
    // get service message
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    // get conn pointer
    ConnectionBase* conn = (sgn::ConnectionBase*)smsg->params.get_param(2);
    // stats
    if(smsg->get_sdone_hndlr()->status == 0) conn->stats.inc(CST_SMSG_SENT_COUNT, 1);
    else conn->stats.inc(CST_SMSG_SEND_ERROR, 1);
    // return service message to pool
    smsg->get_smsg_manager()->free_smsg(smsg);
    // dec refc counter for conn
    conn->refc_dec();


}

// explicit instantiation of template types
template class sgn::Connection<sgn::PcapPayload>;
template class sgn::_M3UAConnection<sgn::M3uaPayload>;
template class sgn::_SMPPConnection<sgn::SmppPayload>;
template class sgn::_PCAPConnection<sgn::PcapPayload>;
template class sgn::DefaultConnectionLogic<sgn::SmppPayload>;
template class sgn::DefaultConnectionLogic<sgn::M3uaPayload>;
template class sgn::DefaultConnectionLogic<sgn::PcapPayload>;
