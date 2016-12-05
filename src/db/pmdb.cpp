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

#include <pmdb.h>
#include <r14p_asn1.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <daemon.h>

// DBRecord
db::DBRecord::DBRecord(): type(DB_RT_SMS){

}

db::DBRecord::~DBRecord(){

}

void db::DBRecord::reset(){
    //params_p = &params;
}

//void db::DBRecord::set_params(pmink_utils::PooledVPMap<uint32_t>* _params){
//    params_p = _params;
//}


// RecordProcessor
db::RecordProcessor::RecordProcessor(DBRecordType _type, DBManager* _dbm): type(_type), dbm(_dbm){

}

db::RecordProcessor::~RecordProcessor(){

}

db::CombinedRecordProcessor::CombinedRecordProcessor(DBManager* _dbm): RecordProcessor(DB_RT_COMBINED, _dbm){
    // register stats
    stats.register_item(CRPST_SMS);
    stats.register_item(CRPST_SRI);

}

int db::CombinedRecordProcessor::process_sri(std::vector<DBRecord*>* data, int component_count){
    if(component_count == 0) return 0;
    if(dbm->get_conn() == NULL) return 2003;
    // if connection closed, return CR_SERVER_LOST client error
    if(dbm->get_conn()->isClosed()) return 2013;
    // prep statement pointer
    sql::PreparedStatement* prep = NULL;

    try{
	std::string sql("insert into Sri (SriTypeId, "
					 "OriginatingPointCode, "
					 "DestinationPointCode, "
					 "CalledGt, "
					 "CallingGt, "
					 "TcapSid, "
					 "TcapDid, "
					 "AppCtxOid, "
					 "Imsi, "
					 "Msisdn, "
					 "Nnn, "
					 "An, "
					 "Sca, "
					 "Timestamp, "
					 "ErrorTypeId, "
					 "ErrorCode) values");

	// prepare values part
	for(int i = 0; i<component_count - 1; i++) sql.append("(1, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, from_unixtime(?), ?, ?), ");
	// last values part
	sql.append("(1, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, from_unixtime(?), ?, ?)");
	// prepare
	prep = dbm->get_conn()->prepareStatement(sql);
	// set values
	int l;
	DBRecord* dbr = NULL;
	int tmp;
	int cc;
	int cd = 0;
	std::ostringstream sms_text;
	pmink_utils::VariantParam* vparam;

	// loop dbr records
	for(unsigned int i = 0; i<data->size(); i++){
	    // dbr pointer
	    dbr = (*data)[i];
	    // get tcap component count
	    vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_component_count, 0);
	    if(vparam == NULL) continue;
	    cc = (int)*vparam;
	    // loop components
	    for(int j = 0; j<cc; j++){
		// get component type
		vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_component_type, j);
		if(vparam == NULL) continue;
		tmp = (int)*vparam;
		// check if component type is invoke
		if(tmp != 1) continue;
		// get component opcode
		vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_opcode, j);
		if(vparam == NULL) continue;
		tmp = (int)*vparam;
		// check if component opcode is sendROutingInfoForSM (45)
		if(tmp != 45) continue;
		// statement index
		l = 15 * cd;

		// m3ua opc
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code)) != NULL){
		    prep->setInt(1 + l, (int)*vparam);

		}else prep->setNull(1 + l, 0);

		// m3ua dpc
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code)) != NULL){
		    prep->setInt(2 + l, (int)*vparam);

		}else prep->setNull(2 + l, 0);

		// called gt
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address)) != NULL){
		    prep->setString(3 + l, (char*)*vparam);

		}else prep->setNull(3 + l, 0);

		// calling gt
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address)) != NULL){
		    prep->setString(4 + l, (char*)*vparam);

		}else prep->setNull(4 + l, 0);

		// tcap sid
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id)) != NULL){
		    prep->setInt64(5 + l, (unsigned int)(int)*vparam);

		}else prep->setNull(5 + l, 0);

		// tcap did
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id)) != NULL){
		    prep->setInt64(6 + l, (unsigned int)(int)*vparam);

		}else prep->setNull(6 + l, 0);

		// app context
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid)) != NULL){
		    prep->setString(7 + l, (char*)*vparam);

		}else prep->setNull(7 + l, 0);

		// imsi
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_imsi, j)) != NULL){
		    prep->setString(8 + l, (char*)*vparam);

		}else prep->setNull(8 + l, 0);

		// msisdn
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, j)) != NULL){
		    prep->setString(9 + l, (char*)*vparam);

		}else prep->setNull(9 + l, 0);

		// nnn
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, j)) != NULL){
		    prep->setString(10 + l, (char*)*vparam);

		}else prep->setNull(10 + l, 0);

		// an
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_an_digits, j)) != NULL){
		    prep->setString(11 + l, (char*)*vparam);

		}else prep->setNull(11 + l, 0);

		// sca
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_sca_digits, j)) != NULL){
		    prep->setString(12 + l, (char*)*vparam);

		}else prep->setNull(12 + l, 0);

		// timestamp
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_pmink_timestamp)) == NULL){
		    prep->setInt64(13 + l, time(NULL));
		}else prep->setInt64(13 + l, (unsigned int)(int)*vparam);

		// error type
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_error_type, j)) != NULL){
		    prep->setInt(14 + l, (int)*vparam);

		}else prep->setNull(14 + l, 0);

		// error code
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_error_code, j)) != NULL){
		    prep->setInt(15 + l, (int)*vparam);

		}else prep->setNull(15 + l, 0);

		// component done count
		++cd;

	    }
	}

	// execute
	prep->execute();

	// close
	prep->close();
	delete prep;

    }catch(sql::SQLException& e){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Mysql process_sri error: [%s], error code = [%d]", e.what(), e.getErrorCode());
	// free mem
	if(prep != NULL) delete prep;
	// return error code
	return e.getErrorCode();
    }

    return 0;

}

int db::CombinedRecordProcessor::process_sms(std::vector<DBRecord*>* data, int component_count){
    if(component_count == 0) return 0;
    if(dbm->get_conn() == NULL) return 2003;
    // if connection closed, return CR_SERVER_LOST client error
    if(dbm->get_conn()->isClosed()) return 2013;
    // prep statement pointer
    sql::PreparedStatement* prep = NULL;

    try{

	std::string sql("insert into Sms (DirectionId, "
					 "SmsSizeTypeId, "
					 "PduId, "
					 "FilterActionId, "
					 "SmsStatusId, "
					 "FilterExitPoint, "
					 "SourceIp, "
					 "DestinationIp, "
					 "SourcePort, "
					 "DestinationPort, "
					 "SystemId, "
					 "CalledGt, "
					 "CallingGt, "
					 "Scda, "
					 "Scoa, "
					 "Imsi, "
					 "Msisdn, "
					 "SmsDestination, "
					 "SmsOriginating, "
					 "SmsText, "
					 "Timestamp, "
					 "SmsTextEncId, "
					 "SmsDestinationEncId, "
					 "SmsOriginatingEncId, "
					 "DestinationPointCode, "
					 "OriginatingPointCode, "
					 "TcapSid, "
					 "TcapDid, "
					 "AppCtxOid, "
					 "SmsPartnum, "
					 "SmsParts, "
					 "SmsMessageId, "
					 "ErrorTypeId, "
					 "ErrorCode) values");

	// prepare values part
	for(int i = 0; i<component_count - 1; i++)
	    sql.append("(?, ?, ?, ?, ?, ?, unhex(?), unhex(?), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, unhex(?), from_unixtime(?), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?), ");


	// last values part
	sql.append("(?, ?, ?, ?, ?, ?, unhex(?), unhex(?), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, unhex(?), from_unixtime(?), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	// prepare
	prep = dbm->get_conn()->prepareStatement(sql);
	// set values
	int l;
	DBRecord* dbr = NULL;
	int tmp;
	int size;
	int cc;
	int cd = 0;
	unsigned char* tmp_p;
	std::ostringstream sms_text;
	pmink_utils::VariantParam* vparam;


	// loop dbr records
	for(unsigned int i = 0; i<data->size(); i++){
	    // dbr pointer
	    dbr = (*data)[i];
	    // get tcap component count
	    vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_component_count, 0);
	    if(vparam == NULL) continue;
	    cc = (int)*vparam;
	    // loop components
	    for(int j = 0; j<cc; j++){
		// get component type
		vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_component_type, j);
		if(vparam == NULL) continue;
		tmp = (int)*vparam;
		// check if component type is invoke
		if(tmp != 1) continue;
		// get component opcode
		vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_opcode, j);
		if(vparam == NULL) continue;
		tmp = (int)*vparam;
		// check if component opcode is mo-forward (46) or mt-forward (44)
		if(!(tmp == 44 || tmp == 46)) continue;
		// statement index
		l = 34 * cd;
		// direction (check TP-MTI in case of sms-tpdu)
		if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_mti, j)) != NULL){
		    // direction id
		    tmp = (int)*vparam;
		    prep->setInt(1 + l, (tmp == 0 ? 2 : 1));
		    // sms size type
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_part, j)) == NULL) prep->setInt(2 + l, 1);
		    else prep->setInt(2 + l, 2);

		    // pdu id (if TP-MTI param exists, pdu type is sms-tpdu)
		    prep->setInt(3 + l, 1);

		    // filter action id
		    // TODO
		    prep->setNull(4 + l, 0);

		    // sms status id
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_error_type, j)) == NULL) prep->setInt(5 + l, 1);
		    else prep->setInt(5 + l, 2);


		    // filter exit point
		    // TODO
		    prep->setNull(6 + l, 0);

		    // source and destination ip (not used in ss7)
		    // source and destination port (not used in ss7)
		    // system id not used in ss7
		    prep->setNull(7 + l, 0);
		    prep->setNull(8 + l, 0);
		    prep->setNull(9 + l, 0);
		    prep->setNull(10 + l, 0);
		    prep->setNull(11 + l, 0);

		    // called gt
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address)) != NULL){
			prep->setString(12 + l, (char*)*vparam);

		    }else prep->setNull(12 + l, 0);

		    // calling gt
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address)) != NULL){
			prep->setString(13 + l, (char*)*vparam);

		    }else prep->setNull(13 + l, 0);

		    // scda
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_scda_digits, j)) != NULL){
			prep->setString(14 + l, (char*)*vparam);

		    }else prep->setNull(14 + l, 0);

		    // scoa
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_scoa_digits, j)) != NULL){
			prep->setString(15 + l, (char*)*vparam);

		    }else prep->setNull(15 + l, 0);

		    // imsi
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_imsi, j)) != NULL){
			prep->setString(16 + l, (char*)*vparam);

		    }else prep->setNull(16 + l, 0);

		    // msisdn
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, j)) != NULL){
			prep->setString(17 + l, (char*)*vparam);

		    }else prep->setNull(17 + l, 0);

		    // sms destination
		    if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_da_digits, j) != NULL){
			prep->setString(18 + l, (char*)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_da_digits, j));

		    }else prep->setNull(18 + l, 0);

		    // sms originating
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, j)) != NULL){
			prep->setString(19 + l, (char*)*vparam);

		    }else prep->setNull(19 + l, 0);

		    // sms text
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_ud, j)) != NULL){
			tmp_p = (unsigned char*)*vparam;
			size = vparam->get_size();
			if(size == 0) prep->setNull(20 + l, 0);
			else{
			    sms_text.str("");
			    pmink_utils::hex(tmp_p, size, 0, &sms_text);
			    prep->setString(20 + l, sms_text.str());

			}

		    }else prep->setNull(20 + l, 0);

		    // timestamp
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_pmink_timestamp)) == NULL){
			prep->setInt64(21 + l, time(NULL));
		    }else prep->setInt64(21 + l, (unsigned int)(int)*vparam);

		    // sms data coding
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_dcs, j)) != NULL){
			prep->setInt(22 + l, (int)*vparam);

		    }else prep->setNull(22 + l, 0);

		    // sms destination coding
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number, j)) != NULL){
			prep->setInt(23 + l, (int)*vparam);

		    }else prep->setNull(23 + l, 0);

		    // sms origination coding
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, j)) != NULL){
			prep->setInt(24 + l, (int)*vparam);

		    }else prep->setNull(24 + l, 0);

		    // m3ua dpc
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code)) != NULL){
			prep->setInt(25 + l, (int)*vparam);

		    }else prep->setNull(25 + l, 0);

		    // m3ua opc
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code)) != NULL){
			prep->setInt(26 + l, (int)*vparam);

		    }else prep->setNull(26 + l, 0);

		    // tcap sid
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id)) != NULL){
			prep->setInt64(27 + l, (unsigned int)(int)*vparam);

		    }else prep->setNull(27 + l, 0);

		    // tcap did
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id)) != NULL){
			prep->setInt64(28 + l, (unsigned int)(int)*vparam);

		    }else prep->setNull(28 + l, 0);

		    // app context
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid)) != NULL){
			prep->setString(29 + l, (char*)*vparam);

		    }else prep->setNull(29 + l, 0);

		    // sms part num
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_part, j)) != NULL){
			prep->setInt(30 + l, (int)*vparam);

		    }else prep->setNull(30 + l, 0);

		    // sms parts
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_parts, j)) != NULL){
			prep->setInt(31 + l, (int)*vparam);

		    }else prep->setNull(31 + l, 0);

		    // sms msg id
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_id, j)) != NULL){
			prep->setInt(32 + l, (int)*vparam);

		    }else prep->setNull(32 + l, 0);

		    // error type
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_error_type, j)) != NULL){
			prep->setInt(33 + l, (int)*vparam);

		    }else prep->setNull(33 + l, 0);

		    // error code
		    if((vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_error_code, j)) != NULL){
			prep->setInt(34 + l, (int)*vparam);

		    }else prep->setNull(34 + l, 0);

		    // component done count
		    ++cd;

		}else{
		    // TODO
		    // smpp
		}



	    }
	}

	// execute
	prep->execute();

	// close
	prep->close();
	delete prep;

    }catch(sql::SQLException& e){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Mysql process_sms error: [%s], error code = [%d]", e.what(), e.getErrorCode());
	// free mem
	if(prep != NULL) delete prep;
	// return error code
	return e.getErrorCode();
    }

    return 0;

}


int db::CombinedRecordProcessor::run(std::vector<DBRecord*>* data){
    int tc_sms = 0;
    int tc_sri = 0;
    DBRecord* dbr = NULL;
    pmink_utils::VariantParam* vparam;
    // loop records
    for(unsigned int i = 0; i<data->size(); i++){
	dbr = (*data)[i];
	// get tcap component count
	vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_component_count, 0);
	if(vparam == NULL) continue;
	int cc = (int)*vparam;
	int tmp;
	// loop components
	for(int j = 0; j<cc; j++){
	    // get component type
	    vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_component_type, j);
	    // sanity check
	    if(vparam == NULL) continue;
	    tmp = (int)*vparam;
	    // check if component type is invoke
	    if(tmp != 1) continue;
	    // get component opcode
	    vparam = dbr->params.get_param(asn1::ParameterType::_pt_tcap_opcode, j);
	    // sanity check
	    if(vparam == NULL) continue;
	    tmp = (int)*vparam;
	    // check if component opcode is mo-forward (46) or mt-forward (44) or sendRoutingInfoForSM (45)
	    if(tmp < 44 || tmp > 46) continue;
	    // inc total count for sri-for-sm
	    if(tmp == 45) {
		++tc_sri;
		stats.inc(CRPST_SRI, 1);
	    }
	    else{
		// check if TP-MTI exists for MO/MT
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_mti, j) == NULL) continue;
		// inc sms component count
		++tc_sms;
		stats.inc(CRPST_SMS, 1);
	    }

	}

    }
    // process sms-tpdu
    int res_sms = process_sms(data, tc_sms);
    // process sri-for-sm
    int res_sri = process_sri(data, tc_sri);

    // return result
    return (res_sms + res_sri);
}


// SMSRecordProcessor
db::SMSRecordProcessor::SMSRecordProcessor(DBManager* _dbm): RecordProcessor(DB_RT_SMS, _dbm){

}

int db::SMSRecordProcessor::run(std::vector<DBRecord*>* data){
    if(dbm->get_conn() == NULL) return 2003;
    // if connection closed, return CR_SERVER_LOST client error
    if(dbm->get_conn()->isClosed()) return 2013;
    // prep statement pointer
    sql::PreparedStatement* prep = NULL;

    try{
	std::string sql("insert into Sms (DirectionId, "
					 "SmsSizeTypeId, "
					 "PduId, "
					 "FilterActionId, "
					 "SmsStatusId, "
					 "FilterExitPoint, "
					 "SourceIp, "
					 "DestinationIp, "
					 "SourcePort, "
					 "DestinationPort, "
					 "SystemId, "
					 "CalledGt, "
					 "CallingGt, "
					 "Scda, "
					 "Scoa, "
					 "Imsi, "
					 "Msisdn, "
					 "SmsDestination, "
					 "SmsOriginating, "
					 "SmsText, "
					 "Timestamp, "
					 "SmsTextEncId, "
					 "SmsDestinationEncId, "
					 "SmsOriginatingEncId, "
					 "DestinationPointCode, "
					 "OriginatingPointCode, "
					 "TcapSid, "
					 "TcapDid, "
					 "SmsPartnum, "
					 "SmsParts, "
					 "SmsMessageId, "
					 "ErrorTypeId, "
					 "ErrorCode) values");

	// prepare values part
	for(unsigned int i = 0; i<data->size() - 1; i++)
	    sql.append("(?, ?, ?, ?, ?, ?, unhex(?), unhex(?), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, unhex(?), from_unixtime(?), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?), ");

	// last values part
	sql.append("(?, ?, ?, ?, ?, ?, unhex(?), unhex(?), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, unhex(?), from_unixtime(?), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	// prepare
	prep = dbm->get_conn()->prepareStatement(sql);
	// set values
	int l;
	DBRecord* dbr = NULL;
	int tmp;
	int size;
	unsigned char* tmp_p;
	std::ostringstream sms_text;

	for(unsigned int i = 0; i<data->size(); i++){
	    dbr = (*data)[i];
	    l = 33 * i;
	    // direction (check TP-MTI in case of sms-tpdu)
	    if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_mti, 0) != NULL){
		// direction id
		tmp = (int)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_mti, 0);
		prep->setInt(1 + l, (tmp == 0 ? 2 : 1));

		// sms size type
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_part) == NULL) prep->setInt(2 + l, 1);
		else prep->setInt(2 + l, 2);

		// pdu id (if TP-MTI param exists, pdu type is sms-tpdu)
		prep->setInt(3 + l, 1);

		// filter action id
		// TODO
		prep->setNull(4, 0);

		// sms status id
		if(dbr->params.get_param(asn1::ParameterType::_pt_pmink_sms_status) == NULL) prep->setInt(5 + l, 3);
		else prep->setInt(5 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_pmink_sms_status, 0));


		// filter exit point
		// TODO
		prep->setNull(6, 0);

		// source and destination ip (not used in ss7)
		// source and destination port (not used in ss7)
		// system id not used in ss7
		prep->setNull(7, 0);
		prep->setNull(8, 0);
		prep->setNull(9, 0);
		prep->setNull(10, 0);
		prep->setNull(11, 0);


		// called gt
		if(dbr->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address) != NULL){
		    prep->setString(12 + l, (char*)*dbr->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, 0));

		}else prep->setNull(12, 0);

		// calling gt
		if(dbr->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address) != NULL){
		    prep->setString(13 + l, (char*)*dbr->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, 0));

		}else prep->setNull(13, 0);

		// scda
		if(dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_scda_digits) != NULL){
		    prep->setString(14 + l, (char*)*dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_scda_digits, 0));

		}else prep->setNull(14, 0);

		// scoa
		if(dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_scoa_digits) != NULL){
		    prep->setString(15 + l, (char*)*dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_scoa_digits, 0));

		}else prep->setNull(15, 0);

		// imsi
		if(dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_imsi) != NULL){
		    prep->setString(16 + l, (char*)*dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_imsi, 0));

		}else prep->setNull(16, 0);

		// msisdn
		if(dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits) != NULL){
		    prep->setString(17 + l, (char*)*dbr->params.get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, 0));

		}else prep->setNull(17, 0);

		// sms destination
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_da_digits) != NULL){
		    prep->setString(18 + l, (char*)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_da_digits, 0));

		}else prep->setNull(18, 0);

		// sms originating
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_digits) != NULL){
		    prep->setString(19 + l, (char*)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, 0));

		}else prep->setNull(19, 0);

		// sms text
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_ud) != NULL){
		    tmp_p = (unsigned char*)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_ud, 0);
		    size = dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_ud, 0)->get_size();
		    sms_text.str("");
		    pmink_utils::hex(tmp_p, size, 0, &sms_text);
		    prep->setString(20 + l, sms_text.str());

		}else prep->setNull(20, 0);

		// timestamp
		if(dbr->params.get_param(asn1::ParameterType::_pt_pmink_timestamp) == NULL){
		    prep->setInt64(21 + l, time(NULL));
		}else prep->setInt64(21 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_pmink_timestamp, 0));

		// sms data coding
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_dcs) != NULL){
		    prep->setInt(22 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_dcs, 0));

		}else prep->setNull(22, 0);

		// sms destination coding
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number) != NULL){
		    prep->setInt(23 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number, 0));

		}else prep->setNull(23, 0);

		// sms origination coding
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number) != NULL){
		    prep->setInt(24 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, 0));

		}else prep->setNull(24, 0);

		// m3ua dpc
		if(dbr->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code) != NULL){
		    prep->setInt(25 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, 0));

		}else prep->setNull(25, 0);

		// m3ua opc
		if(dbr->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code) != NULL){
		    prep->setInt(26 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, 0));

		}else prep->setNull(26, 0);

		// tcap sid
		if(dbr->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id) != NULL){
		    prep->setInt(27 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id, 0));

		}else prep->setNull(27, 0);

		// tcap did
		if(dbr->params.get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id) != NULL){
		    prep->setInt(28 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id, 0));

		}else prep->setNull(28, 0);

		// sms part num
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_part) != NULL){
		    prep->setInt(29 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_part, 0));

		}else prep->setNull(29, 0);

		// sms parts
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_parts) != NULL){
		    prep->setInt(30 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_parts, 0));

		}else prep->setNull(30, 0);

		// sms msg id
		if(dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_id) != NULL){
		    prep->setInt(31 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_smstpdu_msg_id, 0));

		}else prep->setNull(31, 0);

		// error type
		if(dbr->params.get_param(asn1::ParameterType::_pt_tcap_error_type) != NULL){
		    prep->setInt(32 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_tcap_error_type, 0));

		}else prep->setNull(32, 0);

		// error code
		if(dbr->params.get_param(asn1::ParameterType::_pt_tcap_error_code) != NULL){
		    prep->setInt(33 + l, (int)*dbr->params.get_param(asn1::ParameterType::_pt_tcap_error_code, 0));

		}else prep->setNull(33, 0);


	    }else{
		// TODO smpp

	    }






	}
	// execute
	prep->execute();

	// close
	prep->close();
	delete prep;


    }catch(sql::SQLException& e){
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Mysql error: [%s], error code = [%d]", e.what(), e.getErrorCode());
	// free mem
	if(prep != NULL) delete prep;
	// return error code
	return e.getErrorCode();
    }

    return 0;
}


// DBManager
db::DBManager::DBManager(): port(3306),
			    driver(NULL),
			    conn(NULL),
			    batch_size(10){

    sem_init(&signal, 0, 0);
}

db::DBManager::~DBManager(){
    sem_post(&signal);
    sem_destroy(&signal);

}

sql::Connection* db::DBManager::get_conn(){
    return conn;
}


int db::DBManager::connect(const char* _username,
			   const char* _password,
			   const char* _ip_addr,
			   int _port,
			   const char* _db_name){
    try{
	// check for previous
	if(conn != NULL) {
	    conn->close();
	    delete conn;
	    conn = NULL;
	}
	// remember params
	if(_username != NULL) username.assign(_username);
	if(_password != NULL) password.assign(_password);
	if(_ip_addr != NULL) address.assign(_ip_addr);
	if(_port > 0) port = _port;
	if(_db_name != NULL) db_name.assign(_db_name);


	// driver
	driver = sql::mysql::get_driver_instance();

	// connection
	std::ostringstream db_url("tcp://");
	db_url << address << ":" << port;
	conn = driver->connect(db_url.str(), username, password);
	conn->setSchema(db_name);

	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Mysql successfully connected, address = [%s:%d]", address.c_str(), port);


    }catch(sql::SQLException& e){
	//std::cout << "ERR: " << e.what() << " - " << e.getErrorCode() << std::endl;
	pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Mysql error: [%s], error code = [%d]", e.what(), e.getErrorCode());

	// free mem
	if(conn != NULL) {
	    delete conn;
	    conn = NULL;
	}

	return 1;
    }
    return 0;
}

void db::DBManager::init(unsigned int _batch_size, int _pool_size){
    batch_size = _batch_size;
    if(_pool_size == -1) _pool_size = _batch_size * 10;
    // init queue
    queue.set_capacity(_pool_size);
    // init pools
    pool_dbr.init(_pool_size);
    pool_dbr.construct_objects();
    // init threads
    pthread_attr_t pi_thread_attr;
    // scheduling parameters
    pthread_attr_init(&pi_thread_attr);
    // create detached
    pthread_attr_setdetachstate(&pi_thread_attr, PTHREAD_CREATE_DETACHED);
    thm.new_thread(&db_loop, "pm_db", this, &pi_thread_attr);
    // destroy attributes
    pthread_attr_destroy(&pi_thread_attr);

}

int db::DBManager::push(DBRecord* chunk){
    bool res = queue.push(chunk);

    if(res) sem_post(&signal);
    else pool_dbr.deallocate_constructed(chunk);

    return !res;
}

void db::DBManager::register_rcp(RecordProcessor* rcp){
    rcp_map[rcp->type] = rcp;
}


void* db::DBManager::db_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    db::DBManager* dbm = (db::DBManager*)ti->user_args;
    RecordProcessor* rcp = NULL;

    if(dbm != NULL){
	int res;
	timespec ts;
	DBRecord* dbr = NULL;
	std::map<DBRecordType, std::vector<DBRecord*> > batch_map;
	typedef std::map<DBRecordType, std::vector<DBRecord*> >::iterator batch_it_t;
	typedef std::map<DBRecordType, RecordProcessor*>::iterator rcp_it_t;

	// init batch map
	for(rcp_it_t it = dbm->rcp_map.begin(); it != dbm->rcp_map.end(); it++){
	    batch_map[it->first] = std::vector<DBRecord*>();

	}


	// loop
	while(ti->is_active()){
	    // pop record and push to batch list
	    if(dbm->queue.pop(&dbr)) batch_map[dbr->type].push_back(dbr);
	    // loop batches
	    for(batch_it_t it = batch_map.begin(); it != batch_map.end(); it++){
		// check if batch is ready
		if(it->second.size() >= dbm->batch_size){
		    // get rcp
		    rcp = dbm->rcp_map[it->first];
		    // run rcp
		    res = rcp->run(&it->second);
		    // reconnect
		    if(res >= 2000) dbm->connect();
		    // if flushed to db, free batch buffer
		    else{
			// free batch buffer
			for(unsigned int i = 0; i<it->second.size(); i++) dbm->pool_dbr.deallocate_constructed(it->second[i]);
			it->second.clear();

		    }

		}

	    }


	    // wait for signal or timeout
	    clock_gettime(CLOCK_REALTIME, &ts);
	    ts.tv_sec += 1;
	    sem_timedwait(&dbm->signal, &ts);

	}


    }
    // free conn
    if(dbm->conn != NULL){
	dbm->conn->close();
	delete dbm->conn;
    }

    return NULL;

}
