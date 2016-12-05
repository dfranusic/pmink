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

#include <stp.h>
#include <getopt.h>
#include <sccp.h>
#include <m3ua.h>
#include <stp_stats.h>
#include <smstpdu.h>
#include <GSMMAP.h>
#include <smpp.h>
#include <connection.h>
#include <boost/algorithm/string.hpp>

// CfgTranslationItem
stp::CfgTranslationItem::CfgTranslationItem(CfgTransMapType _type, uint32_t _value): type(_type), value(_value){}
stp::CfgTranslationItem::CfgTranslationItem(): type(CTMT_VAR), value(0){}
stp::CfgTranslationItem::~CfgTranslationItem(){}
uint32_t stp::CfgTranslationItem::run(){ return 0; }

// CfgTITimestamp
stp::CfgTITimestamp::CfgTITimestamp(): stp::CfgTranslationItem(CTMT_DYNAMIC, 0){}
uint32_t stp::CfgTITimestamp::run(){
    return time(NULL);
}

// RuleProcessor
stp::RuleProcessor::RuleProcessor( pmink_utils::MTMasterSlave<list_m_t, true>* list_m_m,
				  pmink_utils::MTMasterSlave<rule_m_t, true>* rule_m_m){

    list_m.set_master(list_m_m);
    rule_m.set_master(rule_m_m);


}

bool stp::RuleProcessor::sync(){
    // sync lists
    list_m.sync();
    // if sync available
    if(rule_m.can_sync()){
	// dec stats manager refc in rules
	for(rule_m_t::rules_map_it_t it = (*rule_m).get_begin(); it != (*rule_m).get_end(); it++){
	    stp::StatsManagerRefc* stats_m = (stp::StatsManagerRefc*)(void*)*it->second.get_data_p()->get_param(1, 0, 0, SRCT_PMINK);
	    stats_m->refc_dec();
	}
    }

    // sync rules (remain locked)
    if(rule_m.sync_locked()){
	// loop rules
	for(rule_m_t::rules_map_it_t it = (*rule_m).get_begin(); it != (*rule_m).get_end(); it++){
	    // set list_m pointers in rule
	    it->second.get_data_p()->set_pointer(0, &(*list_m), 0, 0, SRCT_PMINK);
	    // get stats manager
	    stp::StatsManagerRefc* stats_m = (stp::StatsManagerRefc*)(void*)*it->second.get_data_p()->get_param(1, 0, 0, SRCT_PMINK);
	    // inc refc
	    stats_m->refc_inc();

	}

	// unlock
	rule_m.unlock();

    }

    return true;
}

// StpRoutingLogic
stp::StpRoutingLogic::StpRoutingLogic(){
    // init cfg translation map

    // pmink
    cfg_trans_map[pmink_utils::hash_fnv1a_str("pmink.timestamp")] = new CfgTITimestamp();

    // *** vars
    // smpp
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.command_id")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_command_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.source_addr_ton")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_originator_ton);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.dest_addr_ton")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_recipient_ton);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.source_addr_npi")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_originator_np);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.dest_addr_npi")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_recipient_np);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.source_addr")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_originator_address);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.destination_addr")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_recipient_address);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.esm_class_mm")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_esm_message_mode);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.esm_class_mt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_esm_message_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.esm_class_gsm")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_esm_gsm_features);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.protocol_id")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_protocol_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.priority_flag")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_priority_flag);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.delivery_time")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_delivery_time);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.validity_period")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_validity_period);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.rd_smsc_dlvr_rcpt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_rd_smsc_receipt);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.rd_sme_orig_ack")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_rd_sme_ack);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.rd_intrmd_ntf")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_rd_intermediate_notification);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.replace_if_present_flag")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_replace_if_present_flag);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.data_coding")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_data_coding);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sm_default_msg_id")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sm_defaut_msg_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sm_length")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sm_length);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sar_msg_ref_num")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sar_total_segments")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sar_total_segments);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sar_segment_seqnum")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sar_segment_seqnum);

    // tcap
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.tcmt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_message_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.sid")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_source_transaction_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.did")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_destination_transaction_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.cc")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_component_count);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.ct")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_component_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.iid")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_component_invoke_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.opcode")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_opcode);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.dlg_ctx")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_dialogue_context_oid);
    // map
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.imsi")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_imsi);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.msisdn.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.msisdn.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.msisdn.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_msisdn_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.sca.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_sca_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.sca.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_sca_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.sca.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_sca_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scoa.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scoa_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scoa.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scoa.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scoa_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scda.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scda_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scda.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scda_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scda.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scda_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.nnn.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_nnn_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.nnn.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.nnn.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_nnn_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.an.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_an_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.an.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_an_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.an.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_an_digits);
    // sms tpdu
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-rp")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_rp);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-udhi")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_udhi);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-srr")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_srr);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-vpf")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_vpf);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-rd")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_rd);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-mti")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_mti);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-mr")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_mr);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-sri")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_sri);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-mms")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_mms);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-da.ton")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-da.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-da.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_da_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-oa.ton")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-oa.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-oa.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_oa_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-pid")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_pid);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-dcs")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_dcs);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-vp")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_vp);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-udl")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_udl);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-scts")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_scts);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.ie.msg_id")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_msg_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.ie.msg_parts")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_msg_parts);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.ie.msg_part")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_msg_part);



    // m3ua
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.opc")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.dpc")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.si")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.ni")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.mp")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_message_priority);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.sls")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.as")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_as_label);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.asp")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_asp_label);
    // sccp
    // calling party
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.routing-indicator")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gti")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.ssn")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.point-code")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_point_code_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gt.tt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gt.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gt.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gt.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
    // called party
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.routing-indicator")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_routing_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gti")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.ssn")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.point-code")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_point_code_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gt.tt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gt.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gt.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gt.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_gt_address);

    // *** constants
    // r14p
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_sms_dr")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_sms_data_retention);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_stp_routing")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_stp_routing);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_sgn_fwd")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_sgn_forward);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_smshub_fwd")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_smshub_forward);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_fgn_filtering")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_fgn_filtering);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.cmdid_srism_req")] = new CfgTranslationItem(CTMT_CONST, asn1::SmsHubCommandId::_shci_sri_sm_req);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.cmdid_srism_ack")] = new CfgTranslationItem(CTMT_CONST, asn1::SmsHubCommandId::_shci_sri_sm_ack);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_sctp")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_SCTP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_m3ua")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_M3UA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_tcp")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_TCP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_smpp")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_SMPP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_pcap_m3ua")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_PCAP_M3UA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_pcap_smpp")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_PCAP_SMPP);


    // smpp
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_generic_nack")] = new CfgTranslationItem(CTMT_CONST, smpp::GENERIC_NACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_receiver")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_RECEIVER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_receiver_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_RECEIVER_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_transmitter")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_TRANSMITTER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_transmitter_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_TRANSMITTER_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_query_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::QUERY_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_query_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::QUERY_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_submit_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::SUBMIT_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_submit_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::SUBMIT_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_deliver_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::DELIVER_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_deliver_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::DELIVER_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_unbind")] = new CfgTranslationItem(CTMT_CONST, smpp::UNBIND);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_unbind_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::UNBIND_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_replace_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::REPLACE_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_replace_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::REPLACE_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_cancel_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::CANCEL_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_cancel_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::CANCEL_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_transceiver")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_TRANSCEIVER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_transceiver_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_TRANSCEIVER_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_outbind")] = new CfgTranslationItem(CTMT_CONST, smpp::OUTBIND);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_enquire_link")] = new CfgTranslationItem(CTMT_CONST, smpp::ENQUIRE_LINK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_enquire_link_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::ENQUIRE_LINK_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_submit_multi")] = new CfgTranslationItem(CTMT_CONST, smpp::SUBMIT_MULTI);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_submit_multi_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::SUBMIT_MULTI_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_alert")] = new CfgTranslationItem(CTMT_CONST, smpp::ALERT_NOTIFICATION);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_data_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::DATA_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_data_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::DATA_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_international")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_INTERNATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_national")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_network_specific")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_NETWORK_SPECIFIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_subscriber")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_SUBSCRIBER_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_alphanumeric")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_ALPHANUMERIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_abbreviated")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_ABBREVIATED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_isdn_telephone")] = new CfgTranslationItem(CTMT_CONST, smpp::ISDN_TELEPHONE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_data_x121")] = new CfgTranslationItem(CTMT_CONST, smpp::DATA_X121);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_telex")] = new CfgTranslationItem(CTMT_CONST, smpp::TELEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_land_mobile")] = new CfgTranslationItem(CTMT_CONST, smpp::LAND_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_national")] = new CfgTranslationItem(CTMT_CONST, smpp::NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_private")] = new CfgTranslationItem(CTMT_CONST, smpp::PRIVATE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_ermes")] = new CfgTranslationItem(CTMT_CONST, smpp::ERMES);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_internet_ip")] = new CfgTranslationItem(CTMT_CONST, smpp::INTERNET_IP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_wap_client_id")] = new CfgTranslationItem(CTMT_CONST, smpp::WAP_CLIENT_ID);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_default")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_DEFAULT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_ia5_ascii")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_IA5_ASCII);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_8bit_binary_1")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_8BIT_BINARY_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_iso_8859_1")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_ISO_8859_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_8bit_binary_2")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_8BIT_BINARY_2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_jis")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_JIS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_8859_5")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_ISO_8859_5);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_8859_8")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_ISO_8859_8);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_ucs2")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_UCS2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_pictogram")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_PICTOGRAM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_iso_2011_jp")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_ISO_2011_JP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_extended_kanji")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_EXTENDED_KANJI);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_ks_c_5601")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_KS_C_5601);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.gsm_no_features")] = new CfgTranslationItem(CTMT_CONST, smpp::GNS_NO_SPECIFIC_FEATURES);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.gsm_udhi")] = new CfgTranslationItem(CTMT_CONST, smpp::GNS_UDHI_INDICATOR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.gsm_reply_path")] = new CfgTranslationItem(CTMT_CONST, smpp::GNS_SET_REPLY_PATH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.gsm_udhi_reply_path")] = new CfgTranslationItem(CTMT_CONST, smpp::GNS_SET_UDHI_AND_REPLY_PATH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.int_no")] = new CfgTranslationItem(CTMT_CONST, smpp::INT_NO);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.int_yes")] = new CfgTranslationItem(CTMT_CONST, smpp::INT_YES);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mm_default_smsc")] = new CfgTranslationItem(CTMT_CONST, smpp::MM_DEFAULT_SMSC_MODE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mm_datagram")] = new CfgTranslationItem(CTMT_CONST, smpp::MM_DATAGRAM_MODE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mm_forward")] = new CfgTranslationItem(CTMT_CONST, smpp::MM_FORWARD_MODE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mm_store_forward")] = new CfgTranslationItem(CTMT_CONST, smpp::MM_STORE_FORWARD_MODE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_enroute")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_ENROUTE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_delivered")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_DELIVERED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_expired")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_EXPIRED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_deleted")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_DELETED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_undeliverable")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_UNDELIVERABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_accepted")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_ACCEPTED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_rejected")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_REJECTED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_default")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_DEFAULT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_smsc_delivery_rcpt")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_SMSC_DELIVERY_RECEIPT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_delivery_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_DELIVERY_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_manual_user_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_MANUAL_USER_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_cnvrs_abort")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_CNVRS_ABORT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_intrm_dlvr_ntf")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_INTRM_DLVR_NTF);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.soa_no_sme_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::SOA_NO_SME_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.soa_sme_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::SOA_SME_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.soa_sme_manual_user_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::SOA_SME_MANUAL_USER_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.soa_sme_both")] = new CfgTranslationItem(CTMT_CONST, smpp::SOA_SME_BOTH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sdr_no_smsc_delivery")] = new CfgTranslationItem(CTMT_CONST, smpp::SDR_NO_SMSC_DELIVERY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sdr_success_failure")] = new CfgTranslationItem(CTMT_CONST, smpp::SDR_SUCCESS_FAILURE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sdr_failure")] = new CfgTranslationItem(CTMT_CONST, smpp::SDR_FAILURE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.si_not_screened")] = new CfgTranslationItem(CTMT_CONST, smpp::SI_NOT_SCREENED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.si_verified_passed")] = new CfgTranslationItem(CTMT_CONST, smpp::SI_VERIFIED_PASSED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.si_verified_failed")] = new CfgTranslationItem(CTMT_CONST, smpp::SI_VERIFIED_FAILED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.si_network_provided")] = new CfgTranslationItem(CTMT_CONST, smpp::SI_NETWORK_PROVIDED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_allowed")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_ALLOWED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_restricted")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_RESTRICTED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_not_available")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_NOT_AVAILABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dfr_destination_unavailable")] = new CfgTranslationItem(CTMT_CONST, smpp::DFR_DESTINATION_UNAVAILABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dfr_destination_address_invalid")] = new CfgTranslationItem(CTMT_CONST, smpp::DFR_DESTINATION_ADDRESS_INVALID);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dfr_perm_net_err")] = new CfgTranslationItem(CTMT_CONST, smpp::DFR_PERMANENT_NETWORK_ERROR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dfr_temp_net_err")] = new CfgTranslationItem(CTMT_CONST, smpp::DFR_TEMPORARY_NETWORK_ERROR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_ms_display")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_MS_DISPLAY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_mobile_equipment")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_MOBILE_EQUIPMENT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_smart_card_1")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_SMART_CARD_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_external_unit_1")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_EXTERNAL_UNIT_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_sms")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_SMS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_csd")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_CSD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_packet_data")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_PACKET_DATA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_ussd")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_USSD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_cdpd")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_CDPD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_data_tac")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_DATA_TAC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_flex_reflex")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_FLEX_REFLEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_cell_broadcast")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_CELL_BROADCAST);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_gsm")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_GSM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_ansi_136")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_ANSI_136);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_is_95")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_IS_95);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_pdc")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_PDC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_phs")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_PHS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_iden")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_IDEN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_amps")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_AMPS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_paging_network")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_PAGING_NETWORK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ds_nsap_even")] = new CfgTranslationItem(CTMT_CONST, smpp::DS_NSAP_EVEN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ds_nsap_odd")] = new CfgTranslationItem(CTMT_CONST, smpp::DS_NSAP_ODD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ds_user_specified")] = new CfgTranslationItem(CTMT_CONST, smpp::DS_USER_SPECIFIED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dt_temporary")] = new CfgTranslationItem(CTMT_CONST, smpp::DT_TEMPORARY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dt_default")] = new CfgTranslationItem(CTMT_CONST, smpp::DT_DEFAULT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dt_invoke")] = new CfgTranslationItem(CTMT_CONST, smpp::DT_INVOKE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_digit")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_DIGIT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_number")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_telephone_no")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_TELEPHONE_NO);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_password")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_PASSWORD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_char_line")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_CHARACTER_LINE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_menu")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_MENU);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_date")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_DATE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_time")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_TIME);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_continue")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_CONTINUE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_unspecified")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_UNSPECIFIED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_english")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_ENGLISH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_french")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_FRENCH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_spanish")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_SPANISH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_german")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_GERMAN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_portuguese")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_PORTUGUESE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mas_available")] = new CfgTranslationItem(CTMT_CONST, smpp::MAS_AVAILABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mas_denied")] = new CfgTranslationItem(CTMT_CONST, smpp::MAS_DENIED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mas_unavailable")] = new CfgTranslationItem(CTMT_CONST, smpp::MAS_UNAVAILABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mmwf_voicemail")] = new CfgTranslationItem(CTMT_CONST, smpp::MMWF_VOICEMAIL_MESSAGE_WAITING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mmwf_fax")] = new CfgTranslationItem(CTMT_CONST, smpp::MMWF_FAX_MESAGE_WAITING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mmwf_email")] = new CfgTranslationItem(CTMT_CONST, smpp::MMWF_ELECTRONIC_MAIL_MESSAGE_WAITING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mmwf_other")] = new CfgTranslationItem(CTMT_CONST, smpp::MMWF_OTHER_MESSAGE_WAITING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mv_store_indef")] = new CfgTranslationItem(CTMT_CONST, smpp::MV_STORE_INDEFINITELY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mv_power_down")] = new CfgTranslationItem(CTMT_CONST, smpp::MV_POWER_DOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mv_sid_based_reg_area")] = new CfgTranslationItem(CTMT_CONST, smpp::MV_SID_BASED_REGISTRATION_AREA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mv_display_only")] = new CfgTranslationItem(CTMT_CONST, smpp::MV_DISPLAY_ONLY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.nec_ansi_136")] = new CfgTranslationItem(CTMT_CONST, smpp::NEC_ANSI_136);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.nec_is_95")] = new CfgTranslationItem(CTMT_CONST, smpp::NEC_IS_95);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.nec_gsm")] = new CfgTranslationItem(CTMT_CONST, smpp::NEC_GSM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pc_default")] = new CfgTranslationItem(CTMT_CONST, smpp::PC_DEFAULT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pc_wcmp")] = new CfgTranslationItem(CTMT_CONST, smpp::PC_WCMP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_plevel_0")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_PRIVACY_LEVEL_0);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_plevel_1")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_PRIVACY_LEVEL_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_plevel_2")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_PRIVACY_LEVEL_2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_plevel_3")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_PRIVACY_LEVEL_3);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_ms_display")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_MS_DISPLAY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_mobile_equipment")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_MOBILE_EQUIPMENT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_smart_card_1")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_SMART_CARD_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_external_unit_1")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_EXTERNAL_UNIT_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_sms")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_SMS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_csd")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_CSD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_packet_data")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_PACKET_DATA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_ussd")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_USSD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_cdpd")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_CDPD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_data_tac")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_DATA_TAC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_flex_reflex")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_FLEX_REFLEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_cell_broadcast")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_CELL_BROADCAST);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_gsm")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_GSM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_ansi_136")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_ANSI_136);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_is_95")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_IS_95);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_pdc")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_PDC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_phs")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_PHS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_iden")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_IDEN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_amps")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_AMPS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_paging_network")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_PAGING_NETWORK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ss_nsap_even")] = new CfgTranslationItem(CTMT_CONST, smpp::SS_NSAP_EVEN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ss_nsap_odd")] = new CfgTranslationItem(CTMT_CONST, smpp::SS_NSAP_ODD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ss_user_specified")] = new CfgTranslationItem(CTMT_CONST, smpp::SS_USER_SPECIFIED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_pssd_indication")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_PSSD_INDICATION);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_pssr_indication")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_PSSR_INDICATION);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_ussr_request")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_USSR_REQUEST);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_ussn_request")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_USSN_REQUEST);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_pssd_response")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_PSSD_RESPONSE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_pssr_respoonse")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_PSSR_RESPONSE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_ussr_confirm")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_USSR_CONFIRM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_ussn_confirm")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_USSN_CONFIRM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.da_sme_address")] = new CfgTranslationItem(CTMT_CONST, smpp::DA_SME_ADDRESS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.da_dl_address")] = new CfgTranslationItem(CTMT_CONST, smpp::DA_DISTRIBUTION_LIST_ADDRESS);


    // SMS TPDU
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_unknown")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_international")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_INTERNATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_national")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_network_specific")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_NETWORK_SPECIFIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_subscriber")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_SUBSCRIBER_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_alphanumeric")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_ALPHANUMERIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_abbreviated")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_ABBREVIATED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_unknown")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_isdn_telephone")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_ISDN_TELEPHONE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_data_x121")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_DATA_X121);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_telex")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_TELEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_land_mobile")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_LAND_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_national")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_private")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_PRIVATE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_ermes")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_ERMES);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.mti_deliver")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPMTIT_SMS_DELIVER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.mti_submit")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPMTIT_SMS_SUBMIT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.vpf_np")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPVPFT_NOT_PRESENT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.vpf_relative")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPVPFT_RELATIVE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.vpf_enhanced")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPVPFT_ENHANCED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.vpf_absolute")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPVPFT_ABSOLUTE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.dcs_gsm7")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPDCSAT_GSM7);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.dcs_8bit")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPDCSAT_8BIT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.dcs_ucs2")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPDCSAT_UCS2);


    // tcap
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.tcmt_begin")] = new CfgTranslationItem(CTMT_CONST, 2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.tcmt_continue")] = new CfgTranslationItem(CTMT_CONST, 5);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.tcmt_end")] = new CfgTranslationItem(CTMT_CONST, 4);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.tcmt_abort")] = new CfgTranslationItem(CTMT_CONST, 7);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_invoke")] = new CfgTranslationItem(CTMT_CONST, 1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_result_last")] = new CfgTranslationItem(CTMT_CONST, 2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_error")] = new CfgTranslationItem(CTMT_CONST, 3);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_reject")] = new CfgTranslationItem(CTMT_CONST, 4);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_result_not_last")] = new CfgTranslationItem(CTMT_CONST, 7);


    // GSM MAP
    // nature of address
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_unknown")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_international")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_INTERNATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_national")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_network_specific")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_NETWORK_SPECIFIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_subscriber")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_SUBSCRIBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_abbreviated")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_ABBREVIATED);
    // numbering plan
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_unknown")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_isdn_telephone")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_ISDN_TELEPHONE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_data_x121")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_DATA_X121);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_telex")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_TELEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_land_mobile")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_LAND_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_national")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_private")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_PRIVATE);


    // m3ua
    // si
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_snmp")] = new CfgTranslationItem(CTMT_CONST, m3ua::SNMP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_sntmm")] = new CfgTranslationItem(CTMT_CONST, m3ua::SNTMM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_sntmsm")] = new CfgTranslationItem(CTMT_CONST, m3ua::SNTMSM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_sccp")] = new CfgTranslationItem(CTMT_CONST, m3ua::SCCP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_tup")] = new CfgTranslationItem(CTMT_CONST, m3ua::TUP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_isup")] = new CfgTranslationItem(CTMT_CONST, m3ua::ISUP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_dup_call_circuit")] = new CfgTranslationItem(CTMT_CONST, m3ua::DUP_CALL_AND_CIRCUIT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_dup_reg_canc")] = new CfgTranslationItem(CTMT_CONST, m3ua::DUP_REG_AND_CANC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_mtp_test")] = new CfgTranslationItem(CTMT_CONST, m3ua::MTP_TESTING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_bisup")] = new CfgTranslationItem(CTMT_CONST, m3ua::BISUP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_sisup")] = new CfgTranslationItem(CTMT_CONST, m3ua::SISUP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_gcp")] = new CfgTranslationItem(CTMT_CONST, m3ua::GCP);


    // sccp
    // routing indicator
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.route_on_ssn")] = new CfgTranslationItem(CTMT_CONST, sccp::ROUTE_ON_SSN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.route_on_gt")] = new CfgTranslationItem(CTMT_CONST, sccp::ROUTE_ON_GT);
    // gti
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.gti_noa")] = new CfgTranslationItem(CTMT_CONST, sccp::GT_NATURE_OF_ADDRESS_INDICATOR_ONLY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.gti_tt")] = new CfgTranslationItem(CTMT_CONST, sccp::GT_TRANSLATION_TYPE_ONLY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.gti_ttnpe")] = new CfgTranslationItem(CTMT_CONST, sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.gti_ttnpenoa")] = new CfgTranslationItem(CTMT_CONST, sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
    // numbering plan
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_unknown")] = new CfgTranslationItem(CTMT_CONST, sccp::UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_isdn_telephone")] = new CfgTranslationItem(CTMT_CONST, sccp::ISDN_TELEPHONE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_generic")] = new CfgTranslationItem(CTMT_CONST, sccp::GENERIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_data_x121")] = new CfgTranslationItem(CTMT_CONST, sccp::DATA_X121);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_telex")] = new CfgTranslationItem(CTMT_CONST, sccp::TELEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_maritime")] = new CfgTranslationItem(CTMT_CONST, sccp::MARITIME);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_land_mobile")] = new CfgTranslationItem(CTMT_CONST, sccp::LAND_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_isdn_mobile")] = new CfgTranslationItem(CTMT_CONST, sccp::ISDN_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_private")] = new CfgTranslationItem(CTMT_CONST, sccp::PRIVATE);
    // nature of address
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_unknown")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_subscriber_number")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_SUBSCRIBER_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_national")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_RESERVED_FOR_NATIONAL_USE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_national_significant")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_NATIONAL_SIGNIFICANT_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_international")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_INTERNATIONAL);
    // subsystem number
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_unknown")] = new CfgTranslationItem(CTMT_CONST, sccp::SSN_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_sccp_mngmt")] = new CfgTranslationItem(CTMT_CONST, sccp::SCCP_MANAGEMENT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_itut")] = new CfgTranslationItem(CTMT_CONST, sccp::ITU_T_RESERVED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_isup")] = new CfgTranslationItem(CTMT_CONST, sccp::ISDN_USER_PART);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_omap")] = new CfgTranslationItem(CTMT_CONST, sccp::OMAP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_map")] = new CfgTranslationItem(CTMT_CONST, sccp::MAP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_hlr")] = new CfgTranslationItem(CTMT_CONST, sccp::HLR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_vlr")] = new CfgTranslationItem(CTMT_CONST, sccp::VLR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_msc")] = new CfgTranslationItem(CTMT_CONST, sccp::MSC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_eic")] = new CfgTranslationItem(CTMT_CONST, sccp::EIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_auc")] = new CfgTranslationItem(CTMT_CONST, sccp::AUC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_isdn_sup")] = new CfgTranslationItem(CTMT_CONST, sccp::ISDN_SUPPLEMENTARY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_international")] = new CfgTranslationItem(CTMT_CONST, sccp::RESERVED_FOR_INTERNATIONAL_USE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_broadband_isdn")] = new CfgTranslationItem(CTMT_CONST, sccp::BROADBAND_ISDN_EDGE_TO_EDGE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_tc_test")] = new CfgTranslationItem(CTMT_CONST, sccp::TC_TEST_RESPONDER);


}

int stp::StpRoutingLogic::run(stp_rule_t* rule, stp_rule_param_t* input){
    // sanity check
    if(rule == NULL || input == NULL) return pmink_utils::RRT_STOP_ERR;
    // vars
    pmink_utils::VariantParam* vp_spec = NULL;
    pmink_utils::VariantParam* vp_data = NULL;
    pmink_utils::VariantParam* vp_tmp = NULL;
    char tmp_str[256];
    bool res = true;
    stp_rule_param_t* rparams = rule->get_data_p();
    bool hunt_stop = false;
    bool ctx_sri = false;
    bool ctx_sm = false;
    bool ctx_smstpdu = false;
    bool ctx_smpp = false;
    // get hunt-stop flag
    vp_tmp = rparams->get_param(SRPT_RULE_HUNT_STOP);
    if(vp_tmp != NULL) hunt_stop = (bool)*vp_tmp;
    int default_res = (hunt_stop ? pmink_utils::RRT_STOP_OK : pmink_utils::RRT_CONTINUE);
    // get sri-for-sm context
    vp_tmp = rparams->get_param(SRPT_RULE_CTX_SRI);
    if(vp_tmp != NULL) ctx_sri = (bool)*vp_tmp;
    // get sm context
    vp_tmp = rparams->get_param(SRPT_RULE_CTX_SM);
    if(vp_tmp != NULL) ctx_sm = (bool)*vp_tmp;
    // get smstpdu context
    vp_tmp = rparams->get_param(SRPT_RULE_CTX_SMSTPDU);
    if(vp_tmp != NULL) ctx_smstpdu = (bool)*vp_tmp;
    // get smpp context
    vp_tmp = rparams->get_param(SRPT_RULE_CTX_SMPP);
    if(vp_tmp != NULL) ctx_smpp = (bool)*vp_tmp;

    // inc rule counter (stats manager is shared between master and slave rule processors, atomics only)
    pmink_utils::StatsManager* stats_m = (pmink_utils::StatsManager*)(void*)*rule->get_data_p()->get_param(1, 0, 0, SRCT_PMINK);
    stats_m->inc(stp::SST_RULE_COUNTER, 1);


    // *** r14p match data ***
    // trunk label
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_trunk_label, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_trunk_label, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_pmink_trunk_label, true);
	}else res &= false;

    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // service id
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_service_id, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_service_id, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_pmink_service_id, true);
	}else res &= false;

    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // src type
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_daemon_type, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_daemon_type, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_pmink_daemon_type, true);
	}else res &= false;

    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // src id
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_daemon_id, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_daemon_id, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_pmink_daemon_id, true);
	}else res &= false;

    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // cmd id
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_command_id, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_command_id, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_pmink_command_id, true);
	}else res &= false;

    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // connection type
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_connection_type, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_connection_type, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_pmink_connection_type, true);
	}else res &= false;

    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // loop count
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_loop_count, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_loop_count, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_pmink_loop_count, true);
	}else res &= false;

    }
    if(!res) return pmink_utils::RRT_CONTINUE;


    // *** m3ua match data ***
    // m3ua opc
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, true);
	}else res &= false;

    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // m3ua dpc
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // m3ua si
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // m3ua ni
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // m3ua mp
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // m3ua sls
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // m3ua as
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_as_label, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_as_label, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_m3ua_as_label, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // m3ua asp
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_asp_label, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_asp_label, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_m3ua_asp_label, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // *** set sccp match data ***
    // **** calling party ****
    // sccp calling party routing indicator
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    sprintf(tmp_str, "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp calling party gti
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp calling party ssn
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp calling party point code
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp calling party global title tt
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp calling party global title np
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp calling party global title nai
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp calling party global title address
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_address, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // **** called party ****
    // sccp called party routing indicator
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp called party gti
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp called party ssn
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp called party point code
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_called_pa_point_code_number, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp called party global title tt
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp called party global title np
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp called party global title nai
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sccp called party global title address
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, SPIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_sccp_called_pa_gt_address, true);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // *** tcap match data ***
    // TC message type
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_message_type);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_tcap_message_type);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_tcap_message_type);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // sid
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_tcap_source_transaction_id);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // did
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_tcap_destination_transaction_id);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // cc
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_component_count);
    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_component_count);
    unsigned int cc = 0;
    if(vp_data != NULL) cc = (int)*vp_data;
    if(vp_spec != NULL && vp_data != NULL){
	snprintf(tmp_str, sizeof(tmp_str), "%d", cc);
	res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_tcap_component_count);
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // ct
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_component_type);
    if(vp_spec != NULL){
	// check all components
	for(unsigned int i = 0; i<cc; i++){
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_component_type, i);
	    if(vp_data == NULL) continue;
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_tcap_component_type, true);
	    // stop loop if matched
	    if(res) break;
	}
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // iid
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_component_invoke_id);
    if(vp_spec != NULL){
	// check all components
	for(unsigned int i = 0; i<cc; i++){
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_component_invoke_id, i);
	    if(vp_data == NULL) continue;
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_tcap_component_invoke_id, true);
	    // stop loop if matched
	    if(res) break;
	}
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // opcode
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_opcode);
    if(vp_spec != NULL){
	// check all components
	for(unsigned int i = 0; i<cc; i++){
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_opcode, i);
	    if(vp_data == NULL) continue;
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_tcap_opcode, true);
	    // stop loop if matched
	    if(res) break;
	}
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // dlg_ctx
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_tcap_dialogue_context_oid);
	}else res &= false;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;


    // sri-for-sm context
    if(ctx_sri){
	// tmp res
	bool cc_res = false;
	// check all context matching components
	for(unsigned int i = 0; i<cc; i++){
	    // check if context valid for current component
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_opcode, i);
	    if(vp_data == NULL) continue;
	    if((int)*vp_data != 45) continue;

	    // msisdn nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // msisdn np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // msisdn address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_gsmmap_msisdn_digits, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // sca nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_sca_type_of_number, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // sca np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // sca address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_digits, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_sca_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_gsmmap_sca_digits, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // imsi
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_imsi, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_imsi, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_gsmmap_imsi, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // nnn nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // nnn np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // nnn address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_gsmmap_nnn_digits, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // an nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_an_type_of_number, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_an_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_an_type_of_number, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // an np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_an_numbering_plan, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // an address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_an_digits, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_an_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_gsmmap_an_digits, true, SRCT_MAP_SRI_FOR_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }


	    // if all matched, stop
	    if(cc_res) break;


	}
	// add to main result
	res &= cc_res;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;


    // sm context
    if(ctx_sm){
	// tmp res
	bool cc_res = false;
	// check all context matching components
	for(unsigned int i = 0; i<cc; i++){
	    // check if context valid for current component
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_opcode, i);
	    if(vp_data == NULL) continue;
	    if(!((int)*vp_data == 44 || (int)*vp_data == 46)) continue;

	    // imsi
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_imsi, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_imsi, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_gsmmap_imsi, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // scda nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scda_type_of_number, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scda_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_scda_type_of_number, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // scda np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // scda address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scda_digits, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scda_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_gsmmap_scda_digits, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // scoa nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // scoa np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // scoa address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scoa_digits, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scoa_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_gsmmap_scoa_digits, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // msisdn nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // msisdn np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // msisdn address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, SPIT_RULE_MATCH_PART, 0, SRCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_gsmmap_msisdn_digits, true, SRCT_MAP_SM);
		// next component if not matched
		if(!cc_res) continue;

	    }


	    // if all matched, stop
	    if(cc_res) break;

	}
	// add to main result
	res &= cc_res;
    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // smstpdu context
    if(ctx_smstpdu){
	// tmp res
	bool cc_res = false;
	// check all context matching components
	for(unsigned int i = 0; i<cc; i++){
	    // check if context valid for current component
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_opcode, i);
	    if(vp_data == NULL) continue;
	    if(!((int)*vp_data == 44 || (int)*vp_data == 46)) continue;

	    // tp-rp
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_rp, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_rp, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_rp, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-udhi
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_udhi, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_udhi, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_udhi, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-srr
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_srr, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_srr, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_srr, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-vpf
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_vpf, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_vpf, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_vpf, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-rd
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_rd, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_rd, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_rd, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-mti
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_mti, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_mti, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_mti, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-mr
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_mr, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_mr, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_mr, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-sri
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_sri, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_sri, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_sri, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-mms
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_mms, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_mms, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_mms, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-da ton
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-da np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-da address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_digits, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_da_digits, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-oa ton
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-oa np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-oa address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_oa_digits, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-pid
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_pid, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_pid, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_pid, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-dcs
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_dcs, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_dcs, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_dcs, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-vp
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_vp, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_vp, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_vp, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-udl
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_udl, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_udl, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_udl, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // tp-scts
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_scts, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_scts, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_tp_scts, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // ie-msg-id
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_msg_id, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_msg_id, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_msg_id, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // ie-msg-parts
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_msg_parts, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_msg_parts, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_msg_parts, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }

	    // ie-msg-part
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_msg_part, SPIT_RULE_MATCH_PART, 0, SRCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_msg_part, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rparams, input, asn1::ParameterType::_pt_smstpdu_msg_part, true, SRCT_DEFAULT);
		// next component if not matched
		if(!cc_res) continue;

	    }




	    // if all matched, stop
	    if(cc_res) break;

	}
	// add to main result
	res &= cc_res;

    }
    if(!res) return pmink_utils::RRT_CONTINUE;

    // smpp context
    if(ctx_smpp){
	// smpp as
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_as_label);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_as_label);
	    if(vp_data != NULL){
		res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_smpp_as_label);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp asp
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_asp_label);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_asp_label);
	    if(vp_data != NULL){
		res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_smpp_asp_label);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp command id
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_command_id);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_command_id);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_command_id);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp source_addr_ton
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_originator_ton);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_originator_ton);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_originator_ton);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp dest_addr_ton
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_recipient_ton);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_recipient_ton);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_recipient_ton);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp source_addr_npi
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_originator_np);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_originator_np);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_originator_np);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp dest_addr_npi
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_recipient_np);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_recipient_np);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_recipient_np);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp source_addr
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_originator_address);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_originator_address);
	    if(vp_data != NULL){
		res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_smpp_originator_address);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp destination_addr
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_recipient_address);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_recipient_address);
	    if(vp_data != NULL){
		res &= match((char*)*vp_data, rparams, input, asn1::ParameterType::_pt_smpp_recipient_address);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp esm_class_mm
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_esm_message_mode);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_esm_message_mode);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_esm_message_mode);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp esm_class_mt
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_esm_message_type);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_esm_message_type);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_esm_message_type);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp esm_class_gsm
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_esm_gsm_features);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_esm_gsm_features);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_esm_gsm_features);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp protocol_id
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_protocol_id);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_protocol_id);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_protocol_id);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp priority_flag
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_priority_flag);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_priority_flag);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_priority_flag);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp delivery_time
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_delivery_time);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_delivery_time);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_delivery_time);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp validity_period
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_validity_period);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_validity_period);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_validity_period);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp rd_smsc_dlvr_rcpt
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_rd_smsc_receipt);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_rd_smsc_receipt);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_rd_smsc_receipt);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp rd_sme_orig_ack
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_rd_sme_ack);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_rd_sme_ack);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_rd_sme_ack);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp rd_intrmd_ntf
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_rd_intermediate_notification);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_rd_intermediate_notification);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_rd_intermediate_notification);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp replace_if_present_flag
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_replace_if_present_flag);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_replace_if_present_flag);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_replace_if_present_flag);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp data_coding
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_data_coding);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_data_coding);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_data_coding);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp sm_default_msg_id
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sm_defaut_msg_id);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sm_defaut_msg_id);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_sm_defaut_msg_id);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp sm_length
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sm_length);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sm_length);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_sm_length);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp sar_msg_ref_num
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp sar_total_segments
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sar_total_segments);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sar_total_segments);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_sar_total_segments);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

	// smpp sar_segment_seqnum
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sar_segment_seqnum);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sar_segment_seqnum);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rparams, input, asn1::ParameterType::_pt_smpp_sar_segment_seqnum);
	    }else res &= false;
	}
	if(!res) return pmink_utils::RRT_CONTINUE;

    }


    // do translations (if everything matched)
    if(res){
	// *** r14p translate data ***
	// trunk label
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_trunk_label, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_STRING,
			   asn1::ParameterType::_pt_pmink_trunk_label);
	}

	// service id
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_service_id, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_pmink_service_id);
	}

	// *** m3ua translate data ***
	// m3ua opc
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code);
	}

	// m3ua dpc
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code);
	}

	// m3ua si
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator);
	}

	// m3ua ni
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator);
	}

	// m3ua mp
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_message_priority);
	}

	// m3ua sls
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code);
	}

	// *** sccp translate data ***
	// **** calling party ****
	// sccp calling party routing indicator
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator);
	}

	// sccp calling party gti
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator);
	}

	// sccp calling party ssn
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);
	}

	// sccp calling party point code
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_point_code_number);
	}

	// sccp calling party global title tt
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type);
	}

	// sccp calling party global title np
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan);
	}

	// sccp calling party global title nai
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address);
	}

	// sccp calling party global title address
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_STRING,
			   asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
	}


	// **** called party ****
	// sccp called party routing indicator
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_routing_indicator);
	}

	// sccp called party gti
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator);
	}

	// sccp called party ssn
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);
	}

	// sccp called party point code
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_point_code_number);
	}

	// sccp called party global title tt
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type);
	}

	// sccp called party global title np
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan);
	}

	// sccp called party global title nai
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address);
	}

	// sccp called party global title address
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, SPIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_STRING,
			   asn1::ParameterType::_pt_sccp_called_pa_gt_address);
	}


    }

    // destinations
    bool more = true;
    int c = 0;
    // remove previous destinations
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_index, -1);
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_destination, -1);
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_destination_type, -1);
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_sub_destination_type, -1);
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_sub_destination, -1);

    // build dests
    while(more){
	// get destination
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_routing_destination, c);
	// stop if no more destinations found
	if(vp_spec == NULL) more = false;
	else{
	    // set data in input
	    input->set_cstr(asn1::ParameterType::_pt_pmink_routing_destination, (char*)*vp_spec, c);
	    // per destination service id
	    input->set(*rparams,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_routing_service_id, c),
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_routing_service_id, c));

	    // sub destination
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_routing_sub_destination, c);
	    vp_tmp = rparams->get_param(asn1::ParameterType::_pt_pmink_routing_sub_destination_type, c);
	    if(vp_spec != NULL && vp_tmp != NULL){
		// set data in input
		input->set_cstr(asn1::ParameterType::_pt_pmink_routing_sub_destination, (char*)*vp_spec, c);
		input->set_int(asn1::ParameterType::_pt_pmink_routing_sub_destination_type, (int)*vp_tmp, c);
	    }

	    // next
	    ++c;
	}

    }
    // default res (based on hunt-stop)
    return default_res;
}

int stp::StpRoutingLogic::find_set_param(const char* cfg_name,
				    stp_rule_param_t* input,
				    stp_rule_param_t* output,
				    pmink_utils::VariantParamType vp_type,
				    uint32_t vp_id,
				    uint32_t vp_index){
    // sanity check
    if(output == NULL || cfg_name == NULL || strlen(cfg_name) < 1) return 1;
    // vars
    pmink_utils::VariantParam* vp = NULL;
    // check if regex
    if(cfg_name[0] == ':'){
	// sanity checks
	if(strlen(cfg_name) < 5) return 1;
	if(cfg_name[1] != '/') return 1;
	// tmp vars
	char* tmp;
	char* token_match;
	char* token_replace;
	char tmp_copy[256];
	// sanity check
	if(strlen(&cfg_name[1]) + 1 > sizeof(tmp_copy)) return 1;
	// make c str copy
	strcpy(tmp_copy, &cfg_name[1]);
	// tokenize
	// match part
	token_match = strtok_r(tmp_copy, "/", &tmp);
	// error check
	if(token_match == NULL) return 1;
	// replace part
	token_replace = strtok_r(NULL, "/", &tmp);
	// error check
	if(token_replace == NULL) return 1;
	// get from input
	vp = input->get_param(vp_id);
	// sanity check
	if(vp == NULL) return 1;
	// tmp data
	char tmp_data[16];
	char* data_p = NULL;
	// check type (ony int and string used)
	switch(vp->get_type()){
	    case pmink_utils::DPT_INT:
		// convert int to string
		snprintf(tmp_data, sizeof(tmp_data), "%d", (int)*vp);
		// set pointer
		data_p = tmp_data;
		break;

	    case pmink_utils::DPT_STRING:
		// set pointer
		data_p = (char*)*vp;
		break;

	    default: return 1;
	}

	// run regex
	try{
	    // match regex
	    boost::regex rgx(token_match);
	    // replace
	    std::string rpl_str = boost::regex_replace(std::string(data_p), rgx, token_replace);
	    // check type (ony int and string used)
	    switch(vp_type){
		case pmink_utils::DPT_INT:
		    output->set_int(vp_id, atoi(rpl_str.c_str()), SPIT_INPUT_TRANSLATE_PART);
		    return 0;

		case pmink_utils::DPT_STRING:
		    output->set_cstr(vp_id, rpl_str.c_str(), SPIT_INPUT_TRANSLATE_PART);
		    return 0;

		default: return 1;
	    }

	// invalid regex pattern
	}catch(std::exception& e){
	    return 1;
	}

    // check if var value
    }else if(cfg_name[0] == '@'){
	// sanity check
	if(strlen(cfg_name) < 2) return 0;
	// find cfg mapping
	uint32_t hash = pmink_utils::hash_fnv1a(&cfg_name[1], strlen(cfg_name) - 1);
	std::map<uint32_t, CfgTranslationItem*>::iterator it = cfg_trans_map.find(hash);
	if(it == cfg_trans_map.end()) return 1;
	// check if const of var
	switch(it->second->type){
	    case CTMT_CONST:
		// check output type
		switch(vp_type){
		    case pmink_utils::DPT_INT:
			output->set_int(vp_id, it->second->value, vp_index);
			return 0;

		    case pmink_utils::DPT_STRING:
			// convert int to string
			char tmp_str[256];
			snprintf(tmp_str, sizeof(tmp_str), "%d", it->second->value);
			output->set_cstr(vp_id, tmp_str, vp_index);
			return 0;

		    default: return 1;
		}
		break;

	    case CTMT_VAR:
	    {
		// get from input
		vp = input->get_param(it->second->value);
		// sanity check
		if(vp == NULL) return 1;
		// check var type (ony int and string used)
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// check if conversion needed
			if(vp_type == pmink_utils::DPT_INT) output->set_int(vp_id, (int)*vp, SPIT_INPUT_TRANSLATE_PART);
			else if(vp_type == pmink_utils::DPT_STRING){
			    // convert int to string
			    char tmp_str[256];
			    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			    output->set_cstr(vp_id, tmp_str, SPIT_INPUT_TRANSLATE_PART);

			}
			return 0;

		    case pmink_utils::DPT_STRING:
			// check if conversion needed
			if(vp_type == pmink_utils::DPT_STRING) output->set_cstr(vp_id, (char*)*vp, SPIT_INPUT_TRANSLATE_PART);
			// convert to int
			else if(vp_type == pmink_utils::DPT_INT) output->set_int(vp_id, atoi((char*)*vp), SPIT_INPUT_TRANSLATE_PART);
			return 0;

		    default: return 1;
		}
		break;
	    }

	    case CTMT_DYNAMIC:
	    {
		// check if conversion needed
		if(vp_type == pmink_utils::DPT_INT) output->set_int(vp_id, it->second->run(), SPIT_INPUT_TRANSLATE_PART);
		else if(vp_type == pmink_utils::DPT_STRING){
		    // convert int to string
		    char tmp_str[256];
		    snprintf(tmp_str, sizeof(tmp_str), "%d", it->second->run());
		    output->set_cstr(vp_id, tmp_str, SPIT_INPUT_TRANSLATE_PART);

		}

		break;
	    }
	}



    // literal value
    }else{
	// check type (ony int and string used)
	switch(vp_type){
	    case pmink_utils::DPT_INT:
		output->set_int(vp_id, atoi(cfg_name), vp_index);
		return 0;

	    case pmink_utils::DPT_STRING:
		output->set_cstr(vp_id, cfg_name, vp_index);
		return 0;

	    default: return 1;
	}

    }

    // return
    return 1;
}

bool stp::StpRoutingLogic::data_match(const char* data1, const char* data2, StpRuleOperatorType op_t){
    // sanity check
    if(data1 == NULL || data2 == NULL) return false;
    // check operator
    switch(op_t){
	// ==
	case SROT_EQ:
	    return (strcmp(data1, data2) == 0);

	// >
	case SROT_GT:
	    return (atoi(data1) > atoi(data2));

	// >=
	case SROT_GTE:
	    return (atoi(data1) >= atoi(data2));

	// <
	case SROT_LT:
	    return (atoi(data1) < atoi(data2));

	// <=
	case SROT_LTE:
	    return (atoi(data1) <= atoi(data2));

	// !=
	case SROT_NE:
	    return (strcmp(data1, data2) != 0);

	default: break;
    }

    // default
    return false;
}


stp::StpRuleOperatorType stp::StpRoutingLogic::op_detect(const char* pattern){
    // sanity checks
    if(pattern == NULL) return SROT_EQ;
    if(strlen(pattern) == 0) return SROT_EQ;
    // match
    if(strcmp(pattern, "==") == 0) return SROT_EQ;
    else if(strcmp(pattern, "!=") == 0) return SROT_NE;
    else if(strcmp(pattern, ">") == 0) return SROT_GT;
    else if(strcmp(pattern, ">=") == 0) return SROT_GTE;
    else if(strcmp(pattern, "<") == 0) return SROT_LT;
    else if(strcmp(pattern, "<=") == 0) return SROT_LTE;
    else if(strcmp(pattern, ">>") == 0) return SROT_LST_ADD;
    else if(strcmp(pattern, "<<") == 0) return SROT_LST_DEL;
    else if(strcmp(pattern, "--") == 0) return SROT_LST_CLR;
    // default
    return SROT_EQ;
}


bool stp::StpRoutingLogic::match(const char* data, stp_rule_param_t* rparam, stp_rule_param_t* input, uint32_t vp_id, bool skip_lciop, uint32_t context){
    // sanity check
    if(data == NULL || rparam == NULL) return false;
    // check operand type
    pmink_utils::VariantParam* vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 1, context);
    // sanity check
    if(vp == NULL) return false;
    // default operator ==
    StpRuleOperatorType op_t = SROT_EQ;
    unsigned int l_ciop = 0;
    unsigned int r_ciop = 0;
    char* operand = NULL;

    // check
    switch((int)*vp){
	// number operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operator
	// 4 - operand
	case SOTT_NUMBER:
	    // lciop
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operator
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 3, context);
	    op_t = (StpRuleOperatorType)(int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 4, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			char tmp_str[256];
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			return data_match(tmp_str, operand, op_t);

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }
	    // match
	    return data_match(data, operand, op_t);

	// string operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operator
	// 4 - operand
	case SOTT_STRING:
	    // lciop
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operator
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 3, context);
	    op_t = (StpRuleOperatorType)(int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 4, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			char tmp_str[256];
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			return data_match(tmp_str, operand, op_t);

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }

	    // match
	    return data_match(data, operand, op_t);

	// regex operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operand
	case SOTT_REGEX:
	{
	    char tmp_str[256];
	    // lciop
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 3, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			data = tmp_str;
			break;

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }

	    // run regex
	    try{
		// user regex
		boost::regex usr_rgx(operand);
		// return result
		return boost::regex_match(data, usr_rgx);

	    // invalid regex pattern
	    }catch(std::exception& e){
		return false;
	    }
	    break;
	}

	// variable operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operator
	// 4 - right component index operator
	// 5 - operand
	case SOTT_VARIABLE:
	{
	    char tmp_str[256];
	    // lciop
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operator
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 3, context);
	    op_t = (StpRuleOperatorType)(int)*vp;
	    // rciop
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 4, context);
	    r_ciop = (int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 5, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			data = tmp_str;
			break;

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }

	    // find cfg mapping
	    uint32_t hash = pmink_utils::hash_fnv1a(operand, strlen(operand));
	    std::map<uint32_t, CfgTranslationItem*>::iterator it = cfg_trans_map.find(hash);
	    if(it == cfg_trans_map.end()) return false;
	    // check if const of var
	    switch(it->second->type){
		case CTMT_CONST:
		{
		    // match const
		    char tmp_str[16];
		    snprintf(tmp_str, sizeof(tmp_str), "%d", it->second->value);
		    // normal match
		    return data_match(data, tmp_str, op_t);
		}
		case CTMT_VAR:
		{
		    // get from input
		    pmink_utils::VariantParam* vp = input->get_param(it->second->value, r_ciop);
		    // sanity check
		    if(vp == NULL) return false;
		    // check type (ony int and string used)
		    switch(vp->get_type()){
			case pmink_utils::DPT_INT:
			    // convert int to string
			    char tmp_str[16];
			    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			    // normal match
			    return data_match(data, tmp_str, op_t);

			case pmink_utils::DPT_STRING:
			    // normal match
			    return data_match(data, (char*)*vp, op_t);

			default: return 1;
		    }

		    break;
		}

		case CTMT_DYNAMIC:
		{
		    // match const
		    char tmp_str[16];
		    snprintf(tmp_str, sizeof(tmp_str), "%d", it->second->run());
		    // normal match
		    return data_match(data, tmp_str, op_t);
		}
	    }

	    break;
	}

	// list operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operator
	// 4 - operand
	case SOTT_LIST:
	{
	    char tmp_str[256];
	    // lciop
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operator
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 3, context);
	    op_t = (StpRuleOperatorType)(int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, SPIT_RULE_MATCH_PART, 4, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			data = tmp_str;
			break;

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }
	    // get list m poimter
	    list_m_t* list_m_p = (list_m_t*)(void*)*rparam->get_param(0, 0, 0, SRCT_PMINK);
	    if(list_m_p == NULL) return false;

	    // *** special list add >> operator not supported on stp ***

	    // find item
	    std::string* lst_item = list_m_p->get_sublist_item(operand, data);
	    // not found in list
	    if(lst_item == NULL){
		// if != return true
		if(op_t == SROT_NE) return true;
		// else (==) return false
		return false;
	    }
	    // found in list
	    // if == return true
	    if(op_t == SROT_EQ) return true;
	    // else (!=) return false
	    else return false;
	}
    }

    return false;
}



// StpRouting
stp::StpRouting::StpRouting(): rgx("^(?:\\[(\\d+|\\+|<-|->)])?(?:(?:(!=|==|<|<=|>|>=)?(\\d+))|(?::(.+))|(?:(!=|==|<|<=|>|>=)?(?:@(?:\\[(\\d+|\\+|<-|->)])?([A-Za-z].*)))|(?:(!=|==|>>|<<|--)?(?:@\\{([A-Za-z].*)\\}))|(?:(!=|==|<|<=|>|>=)?(.+)))"){

}

stp::StpRouting::~StpRouting(){

}

bool stp::StpRouting::field_valid(const char* _const, bool const_only){
    // sanity check
    if(_const == NULL) return false;
    // min length
    if(strlen(_const) < 1) return false;

    // tokenize
    boost::cmatch what;
    try{
	if(!boost::regex_match(_const, what, rgx)) return 1;

	// SOTT_NUMBER
	if(what[3].str().size() > 0){
	    return true;

	// SOTT_REGEX
	}else if(what[4].str().size() > 0){
	    return true;

	// SOTT_VARIABLE
	}else if(what[7].str().size() > 0){
	    // find cfg mapping
	    uint32_t hash = pmink_utils::hash_fnv1a(what[7].str().c_str(), what[7].str().size());
	    std::map<uint32_t, CfgTranslationItem*>::iterator it = rlogic.cfg_trans_map.find(hash);
	    // check if CONST type or CONST/VAR type found
	    if(it != rlogic.cfg_trans_map.end() && (const_only ? it->second->type == CTMT_CONST : true)) return true;
	    return false;

	// SOTT_LIST
	}else if(what[9].str().size() > 0){
	    return true;


	// SOTT_STRING
	}else if(what[11].str().size() > 0){
	    return true;

	// unknown/error
	}else return false;

    // invalid regex pattern
    }catch(std::exception& e){
	return false;
    }

    // err
    return false;
}


int stp::StpRouting::run_rules(stp_rule_param_t* input, rule_m_t* rm){
    rm->process_rules(input);
    return 0;
}



/*

[1]!=@[2]tcap.const.ct_invoke

1 (left index) 			= 1
2 (operator for int item)	=
3 (int item)			=
4 (regex)			=
5 (operator for const/var item)	= !=
6 (right index)			= 2
7 (const or var item)		= tcap.const.ct_invoke
8 (operator for list item)	=
9 (list item)			=
10 (operator for text item)	=
11(text item)			=

 */
int stp::StpRouting::prepare_match_line(stp_rule_param_t* rdata, const char* line, uint32_t vp_id, uint32_t context){
    // sanity check
    if(rdata == NULL || line == NULL) return 1;
    boost::cmatch what;
    // tokenize
    try{
	if(!boost::regex_match(line, what, rgx)) return 1;
	// set line
	rdata->set_cstr(vp_id, line, SPIT_RULE_MATCH_PART, 0, context);
	// check operand type

	// SOTT_NUMBER
	if(what[3].str().size() > 0){
	    // set type
	    rdata->set_int(vp_id, SOTT_NUMBER, SPIT_RULE_MATCH_PART, 1, context);
	    // set left component index operator
	    rdata->set_int(vp_id, (what[1].str() == "+" ? SPIT_INPUT_TRANSLATE_PART : atoi(what[1].str().c_str())), SPIT_RULE_MATCH_PART, 2, context);
	    // set operator
	    rdata->set_int(vp_id, rlogic.op_detect(what[2].str().c_str()), SPIT_RULE_MATCH_PART, 3, context);
	    // set operand
	    rdata->set_cstr(vp_id, what[3].str().c_str(), SPIT_RULE_MATCH_PART, 4, context);


	// SOTT_REGEX
	}else if(what[4].str().size() > 0){
	    // set type
	    rdata->set_int(vp_id, SOTT_REGEX, SPIT_RULE_MATCH_PART, 1, context);
	    // set left component index operator
	    rdata->set_int(vp_id, (what[1].str() == "+" ? SPIT_INPUT_TRANSLATE_PART : atoi(what[1].str().c_str())), SPIT_RULE_MATCH_PART, 2, context);
	    // set operand
	    rdata->set_cstr(vp_id, what[4].str().c_str(), SPIT_RULE_MATCH_PART, 3, context);

	// SOTT_VARIABLE
	}else if(what[7].str().size() > 0){
	    // set type
	    rdata->set_int(vp_id, SOTT_VARIABLE, SPIT_RULE_MATCH_PART, 1, context);
	    // set left component index operator
	    rdata->set_int(vp_id, (what[1].str() == "+" ? SPIT_INPUT_TRANSLATE_PART : atoi(what[1].str().c_str())), SPIT_RULE_MATCH_PART, 2, context);
	    // set operator
	    rdata->set_int(vp_id, rlogic.op_detect(what[5].str().c_str()), SPIT_RULE_MATCH_PART, 3, context);
	    // set right component index operator
	    rdata->set_int(vp_id, (what[6].str() == "+" ? SPIT_INPUT_TRANSLATE_PART : atoi(what[6].str().c_str())), SPIT_RULE_MATCH_PART, 4, context);
	    // set operand
	    rdata->set_cstr(vp_id, what[7].str().c_str(), SPIT_RULE_MATCH_PART, 5, context);

	// SOTT_LIST
	}else if(what[9].str().size() > 0){
	    // set type
	    rdata->set_int(vp_id, SOTT_LIST, SPIT_RULE_MATCH_PART, 1, context);
	    // set left component index operator
	    rdata->set_int(vp_id, (what[1].str() == "+" ? SPIT_INPUT_TRANSLATE_PART : atoi(what[1].str().c_str())), SPIT_RULE_MATCH_PART, 2, context);
	    // set operator
	    rdata->set_int(vp_id, rlogic.op_detect(what[8].str().c_str()), SPIT_RULE_MATCH_PART, 3, context);
	    // set operand
	    rdata->set_cstr(vp_id, what[9].str().c_str(), SPIT_RULE_MATCH_PART, 4, context);


	// SOTT_STRING
	}else if(what[11].str().size() > 0){
	    // set type
	    rdata->set_int(vp_id, SOTT_STRING, SPIT_RULE_MATCH_PART, 1, context);
	    // set left component index operator
	    rdata->set_int(vp_id, (what[1].str() == "+" ? SPIT_INPUT_TRANSLATE_PART : atoi(what[1].str().c_str())), SPIT_RULE_MATCH_PART, 2, context);
	    // set operator
	    rdata->set_int(vp_id, rlogic.op_detect(what[10].str().c_str()), SPIT_RULE_MATCH_PART, 3, context);
	    // trim operand
	    std::string tmp_str(what[11].str());
	    boost::trim_if(tmp_str, boost::is_any_of("'"));
	    // set operand
	    rdata->set_cstr(vp_id, what[11].str().c_str(), SPIT_RULE_MATCH_PART, 4, context);

	// unknown/error
	}else return 1;

    // invalid regex pattern
    }catch(std::exception& e){
	return 1;
    }

    // ok
    return 0;
}


int stp::StpRouting::build_rule(config::ConfigItem* rule_root, rule_m_t* rm){
    // sanity check
    if(rule_root == NULL) return 1;
    // tmp vars
    pmink_utils::PooledVPMap<uint32_t> tmp_params;
    // rule hunt stop
    tmp_params.set_bool(SRPT_RULE_HUNT_STOP, rule_root->to_bool("hunt-stop"));
    // map contexts
    tmp_params.set_bool(SRPT_RULE_CTX_SRI, false);
    tmp_params.set_bool(SRPT_RULE_CTX_SM, false);
    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, false);
    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, false);

    // *** set r14p match data ***
    // trunk label
    if(rule_root->val_exists("route match r14p trunk_label")){
	if(field_valid(rule_root->to_cstr("route match r14p trunk_label")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match r14p trunk_label"), asn1::ParameterType::_pt_pmink_trunk_label);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p trunk_label] in rule [%s]!",
							  rule_root->to_cstr("route match r14p trunk_label"),
							  rule_root->name.c_str());
    }

    // service id
    if(rule_root->val_exists("route match r14p service_id")){
	if(field_valid(rule_root->to_cstr("route match r14p service_id")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match r14p service_id"), asn1::ParameterType::_pt_pmink_service_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p service_id] in rule [%s]!",
							  rule_root->to_cstr("route match r14p service_id"),
							  rule_root->name.c_str());
    }

    // src type
    if(rule_root->val_exists("route match r14p src_type")){
	if(field_valid(rule_root->to_cstr("route match r14p src_type")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match r14p src_type"), asn1::ParameterType::_pt_pmink_daemon_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p src_type] in rule [%s]!",
							  rule_root->to_cstr("route match r14p src_type"),
							  rule_root->name.c_str());
    }

    // src id
    if(rule_root->val_exists("route match r14p src_id")){
	if(field_valid(rule_root->to_cstr("route match r14p src_id")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match r14p src_id"), asn1::ParameterType::_pt_pmink_daemon_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p src_id] in rule [%s]!",
							  rule_root->to_cstr("route match r14p src_id"),
							  rule_root->name.c_str());
    }

    // cmd id
    if(rule_root->val_exists("route match r14p cmd_id")){
	if(field_valid(rule_root->to_cstr("route match r14p cmd_id")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match r14p cmd_id"), asn1::ParameterType::_pt_pmink_command_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p cmd_id] in rule [%s]!",
							  rule_root->to_cstr("route match r14p cmd_id"),
							  rule_root->name.c_str());
    }

    // connectoion type
    if(rule_root->val_exists("route match r14p conn_type")){
	if(field_valid(rule_root->to_cstr("route match r14p conn_type")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match r14p conn_type"), asn1::ParameterType::_pt_pmink_connection_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p conn_type] in rule [%s]!",
							  rule_root->to_cstr("route match r14p conn_type"),
							  rule_root->name.c_str());
    }

    // loop count
    if(rule_root->val_exists("route match r14p loop_count")){
	if(field_valid(rule_root->to_cstr("route match r14p loop_count")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match r14p loop_count"), asn1::ParameterType::_pt_pmink_loop_count);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p loop_count] in rule [%s]!",
							  rule_root->to_cstr("route match r14p loop_count"),
							  rule_root->name.c_str());
    }


    // *** set m3ua match data ***
    // m3ua opc
    if(rule_root->val_exists("route match m3ua opc")){
	if(field_valid(rule_root->to_cstr("route match m3ua opc")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match m3ua opc"), asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua opc] in rule [%s]!",
							  rule_root->to_cstr("route match m3ua opc"),
							  rule_root->name.c_str());
    }

    // m3ua dpc
    if(rule_root->val_exists("route match m3ua dpc")){
	if(field_valid(rule_root->to_cstr("route match m3ua dpc")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match m3ua dpc"), asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua dpc] in rule [%s]!",
							  rule_root->to_cstr("route match m3ua dpc"),
							  rule_root->name.c_str());
    }

    // m3ua si
    if(rule_root->val_exists("route match m3ua si")){
	if(field_valid(rule_root->to_cstr("route match m3ua si")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match m3ua si"), asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua si] in rule [%s]!",
							  rule_root->to_cstr("route match m3ua si"),
							  rule_root->name.c_str());
    }


    // m3ua ni
    if(rule_root->val_exists("route match m3ua ni")){
	if(field_valid(rule_root->to_cstr("route match m3ua ni")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match m3ua ni"), asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua ni] in rule [%s]!",
							  rule_root->to_cstr("route match m3ua ni"),
							  rule_root->name.c_str());
    }

    // m3ua mp
    if(rule_root->val_exists("route match m3ua mp")){
	if(field_valid(rule_root->to_cstr("route match m3ua mp")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match m3ua mp"), asn1::ParameterType::_pt_m3ua_protocol_data_message_priority);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua mp] in rule [%s]!",
							  rule_root->to_cstr("route match m3ua mp"),
							  rule_root->name.c_str());
    }

    // m3ua sls
    if(rule_root->val_exists("route match m3ua sls")){
	if(field_valid(rule_root->to_cstr("route match m3ua sls")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match m3ua sls"), asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua sls] in rule [%s]!",
							  rule_root->to_cstr("route match m3ua sls"),
							  rule_root->name.c_str());
    }

    // m3ua as
    if(rule_root->val_exists("route match m3ua as")){
	if(field_valid(rule_root->to_cstr("route match m3ua as")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match m3ua as"), asn1::ParameterType::_pt_m3ua_as_label);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua as] in rule [%s]!",
							  rule_root->to_cstr("route match m3ua as"),
							  rule_root->name.c_str());
    }

    // m3ua asp
    if(rule_root->val_exists("route match m3ua asp")){
	if(field_valid(rule_root->to_cstr("route match m3ua asp")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match m3ua asp"), asn1::ParameterType::_pt_m3ua_asp_label);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua asp] in rule [%s]!",
							  rule_root->to_cstr("route match m3ua asp"),
							  rule_root->name.c_str());
    }

    // *** set sccp match data ***
    // **** calling party ****
    // sccp calling party routing indicator
    if(rule_root->val_exists("route match sccp cgpa routing-indicator")){
	if(field_valid(rule_root->to_cstr("route match sccp cgpa routing-indicator")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cgpa routing-indicator"), asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa routing-indicator] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cgpa routing-indicator"),
							  rule_root->name.c_str());
    }

    // sccp calling party gti
    if(rule_root->val_exists("route match sccp cgpa gti")){
	if(field_valid(rule_root->to_cstr("route match sccp cgpa gti")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cgpa gti"), asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gti] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cgpa gti"),
							  rule_root->name.c_str());
    }

    // sccp calling party ssn
    if(rule_root->val_exists("route match sccp cgpa ssn")){
	if(field_valid(rule_root->to_cstr("route match sccp cgpa ssn")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cgpa ssn"), asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa ssn] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cgpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp calling party point code
    if(rule_root->val_exists("route match sccp cgpa point-code")){
	if(field_valid(rule_root->to_cstr("route match sccp cgpa point-code")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cgpa point-code"), asn1::ParameterType::_pt_sccp_calling_pa_point_code_number);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa point-code] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cgpa point-code"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title tt
    if(rule_root->val_exists("route match sccp cgpa gt tt")){
	if(field_valid(rule_root->to_cstr("route match sccp cgpa gt tt")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cgpa gt tt"), asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gt tt] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cgpa gt tt"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title np
    if(rule_root->val_exists("route match sccp cgpa gt np")){
	if(field_valid(rule_root->to_cstr("route match sccp cgpa gt np")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cgpa gt np"), asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gt np] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cgpa gt np"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title nai
    if(rule_root->val_exists("route match sccp cgpa gt nai")){
	if(field_valid(rule_root->to_cstr("route match sccp cgpa gt nai")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cgpa gt nai"), asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gt nai] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cgpa gt nai"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title address
    if(rule_root->val_exists("route match sccp cgpa gt address")){
	if(field_valid(rule_root->to_cstr("route match sccp cgpa gt address")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cgpa gt address"), asn1::ParameterType::_pt_sccp_calling_pa_gt_address);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gt address] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cgpa gt address"),
							  rule_root->name.c_str());
    }

    // **** called party ****
    // sccp called party routing indicator
    if(rule_root->val_exists("route match sccp cdpa routing-indicator")){
	if(field_valid(rule_root->to_cstr("route match sccp cdpa routing-indicator")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cdpa routing-indicator"), asn1::ParameterType::_pt_sccp_called_pa_routing_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa routing-indicator] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cdpa routing-indicator"),
							  rule_root->name.c_str());
    }

    // sccp called party gti
    if(rule_root->val_exists("route match sccp cdpa gti")){
	if(field_valid(rule_root->to_cstr("route match sccp cdpa gti")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cdpa gti"), asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gti] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cdpa gti"),
							  rule_root->name.c_str());
    }

    // sccp called party ssn
    if(rule_root->val_exists("route match sccp cdpa ssn")){
	if(field_valid(rule_root->to_cstr("route match sccp cdpa ssn")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cdpa ssn"), asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa ssn] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cdpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp called party point code
    if(rule_root->val_exists("route match sccp cdpa point-code")){
	if(field_valid(rule_root->to_cstr("route match sccp cdpa point-code")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cdpa point-code"), asn1::ParameterType::_pt_sccp_called_pa_point_code_number);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa point-code] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cdpa point-code"),
							  rule_root->name.c_str());
    }

    // sccp called party global title tt
    if(rule_root->val_exists("route match sccp cdpa gt tt")){
	if(field_valid(rule_root->to_cstr("route match sccp cdpa gt tt")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cdpa gt tt"), asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gt tt] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cdpa gt tt"),
							  rule_root->name.c_str());
    }

    // sccp called party global title np
    if(rule_root->val_exists("route match sccp cdpa gt np")){
	if(field_valid(rule_root->to_cstr("route match sccp cdpa gt np")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cdpa gt np"), asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gt np] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cdpa gt np"),
							  rule_root->name.c_str());
    }

    // sccp called party global title nai
    if(rule_root->val_exists("route match sccp cdpa gt nai")){
	if(field_valid(rule_root->to_cstr("route match sccp cdpa gt nai")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cdpa gt nai"), asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gt nai] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cdpa gt nai"),
							  rule_root->name.c_str());
    }

    // sccp called party global title address
    if(rule_root->val_exists("route match sccp cdpa gt address")){
	if(field_valid(rule_root->to_cstr("route match sccp cdpa gt address")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match sccp cdpa gt address"), asn1::ParameterType::_pt_sccp_called_pa_gt_address);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gt address] in rule [%s]!",
							  rule_root->to_cstr("route match sccp cdpa gt address"),
							  rule_root->name.c_str());
    }

    // *** set tcap match data ***
    // tcap message type
    if(rule_root->val_exists("route match tcap tcmt")){
	if(field_valid(rule_root->to_cstr("route match tcap tcmt")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match tcap tcmt"), asn1::ParameterType::_pt_tcap_message_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap tcmt] in rule [%s]!",
							  rule_root->to_cstr("route match tcap tcmt"),
							  rule_root->name.c_str());
    }

    // tcap sid
    if(rule_root->val_exists("route match tcap sid")){
	if(field_valid(rule_root->to_cstr("route match tcap sid")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match tcap sid"), asn1::ParameterType::_pt_tcap_source_transaction_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap sid] in rule [%s]!",
							  rule_root->to_cstr("route match tcap sid"),
							  rule_root->name.c_str());
    }

    // tcap did
    if(rule_root->val_exists("route match tcap did")){
	if(field_valid(rule_root->to_cstr("route match tcap did")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match tcap did"), asn1::ParameterType::_pt_tcap_destination_transaction_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap did] in rule [%s]!",
							  rule_root->to_cstr("route match tcap did"),
							  rule_root->name.c_str());
    }

    // tcap component count
    if(rule_root->val_exists("route match tcap cc")){
	if(field_valid(rule_root->to_cstr("route match tcap cc")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match tcap cc"), asn1::ParameterType::_pt_tcap_component_count);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap cc] in rule [%s]!",
							  rule_root->to_cstr("route match tcap cc"),
							  rule_root->name.c_str());
    }

    // tcap component type
    if(rule_root->val_exists("route match tcap ct")){
	if(field_valid(rule_root->to_cstr("route match tcap ct")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match tcap ct"), asn1::ParameterType::_pt_tcap_component_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap ct] in rule [%s]!",
							  rule_root->to_cstr("route match tcap ct"),
							  rule_root->name.c_str());
    }

    // tcap invoke id
    if(rule_root->val_exists("route match tcap iid")){
	if(field_valid(rule_root->to_cstr("route match tcap iid")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match tcap iid"), asn1::ParameterType::_pt_tcap_component_invoke_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap iid] in rule [%s]!",
							  rule_root->to_cstr("route match tcap iid"),
							  rule_root->name.c_str());
    }

    // tcap operation code
    if(rule_root->val_exists("route match tcap opcode")){
	if(field_valid(rule_root->to_cstr("route match tcap opcode")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match tcap opcode"), asn1::ParameterType::_pt_tcap_opcode);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap opcode] in rule [%s]!",
							  rule_root->to_cstr("route match tcap opcode"),
							  rule_root->name.c_str());
    }

    // tcap dialogue application context
    if(rule_root->val_exists("route match tcap dlg_ctx")){
	if(field_valid(rule_root->to_cstr("route match tcap dlg_ctx")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match tcap dlg_ctx"), asn1::ParameterType::_pt_tcap_dialogue_context_oid);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap dlg_ctx] in rule [%s]!",
							  rule_root->to_cstr("route match tcap dlg_ctx"),
							  rule_root->name.c_str());
    }

    // *** set map match data ***
    // map sri-for-sm msisdn nai
    if(rule_root->val_exists("route match map context sri-for-sm msisdn nai")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm msisdn nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm msisdn nai"), asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm msisdn nai] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm msisdn nai"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm msisdn np
    if(rule_root->val_exists("route match map context sri-for-sm msisdn np")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm msisdn np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm msisdn np"), asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm msisdn np] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm msisdn np"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm msisdn address
    if(rule_root->val_exists("route match map context sri-for-sm msisdn address")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm msisdn address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm msisdn address"), asn1::ParameterType::_pt_gsmmap_msisdn_digits, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm msisdn address] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm msisdn address"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm sca nai
    if(rule_root->val_exists("route match map context sri-for-sm sca nai")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm sca nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm sca nai"), asn1::ParameterType::_pt_gsmmap_sca_type_of_number, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm sca nai] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm sca nai"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm sca np
    if(rule_root->val_exists("route match map context sri-for-sm sca np")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm sca np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm sca np"), asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm sca np] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm sca np"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm sca address
    if(rule_root->val_exists("route match map context sri-for-sm sca address")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm sca address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm sca address"), asn1::ParameterType::_pt_gsmmap_sca_digits, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm sca address] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm sca address"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm imsi
    if(rule_root->val_exists("route match map context sri-for-sm imsi")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm imsi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm imsi"), asn1::ParameterType::_pt_gsmmap_imsi, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm imsi] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm imsi"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm an nai
    if(rule_root->val_exists("route match map context sri-for-sm an nai")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm an nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm an nai"), asn1::ParameterType::_pt_gsmmap_an_type_of_number, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm an nai] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm an nai"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm an np
    if(rule_root->val_exists("route match map context sri-for-sm an np")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm an np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm an np"), asn1::ParameterType::_pt_gsmmap_an_numbering_plan, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm an np] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm an np"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm an address
    if(rule_root->val_exists("route match map context sri-for-sm an address")){
	if(field_valid(rule_root->to_cstr("route match map context sri-for-sm an address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sri-for-sm an address"), asn1::ParameterType::_pt_gsmmap_an_digits, SRCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm an address] in rule [%s]!",
							  rule_root->to_cstr("route match map context sri-for-sm an address"),
							  rule_root->name.c_str());
    }

    // map sm imsi
    if(rule_root->val_exists("route match map context sm imsi")){
	if(field_valid(rule_root->to_cstr("route match map context sm imsi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm imsi"), asn1::ParameterType::_pt_gsmmap_imsi, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm imsi] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm imsi"),
							  rule_root->name.c_str());
    }

    // map sm scda nai
    if(rule_root->val_exists("route match map context sm scda nai")){
	if(field_valid(rule_root->to_cstr("route match map context sm scda nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm scda nai"), asn1::ParameterType::_pt_gsmmap_scda_type_of_number, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scda nai] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm scda nai"),
							  rule_root->name.c_str());
    }

    // map sm scda np
    if(rule_root->val_exists("route match map context sm scda np")){
	if(field_valid(rule_root->to_cstr("route match map context sm scda np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm scda np"), asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scda np] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm scda np"),
							  rule_root->name.c_str());
    }

    // map sm scda address
    if(rule_root->val_exists("route match map context sm scda address")){
	if(field_valid(rule_root->to_cstr("route match map context sm scda address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm scda address"), asn1::ParameterType::_pt_gsmmap_scda_digits, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scda address] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm scda address"),
							  rule_root->name.c_str());
    }


    // map sm scoa nai
    if(rule_root->val_exists("route match map context sm scoa nai")){
	if(field_valid(rule_root->to_cstr("route match map context sm scoa nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm scoa nai"), asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scoa nai] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm scoa nai"),
							  rule_root->name.c_str());
    }

    // map sm scoa np
    if(rule_root->val_exists("route match map context sm scoa np")){
	if(field_valid(rule_root->to_cstr("route match map context sm scoa np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm scoa np"), asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scoa np] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm scoa np"),
							  rule_root->name.c_str());
    }

    // map sm scoa address
    if(rule_root->val_exists("route match map context sm scoa address")){
	if(field_valid(rule_root->to_cstr("route match map context sm scoa address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm scoa address"), asn1::ParameterType::_pt_gsmmap_scoa_digits, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scoa address] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm scoa address"),
							  rule_root->name.c_str());
    }

    // map sm msisdn nai
    if(rule_root->val_exists("route match map context sm msisdn nai")){
	if(field_valid(rule_root->to_cstr("route match map context sm msisdn nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm msisdn nai"), asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm msisdn nai] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm msisdn nai"),
							  rule_root->name.c_str());
    }

    // map sm msisdn np
    if(rule_root->val_exists("route match map context sm msisdn np")){
	if(field_valid(rule_root->to_cstr("route match map context sm msisdn np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm msisdn np"), asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm msisdn np] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm msisdn np"),
							  rule_root->name.c_str());
    }

    // map sm msisdn address
    if(rule_root->val_exists("route match map context sm msisdn address")){
	if(field_valid(rule_root->to_cstr("route match map context sm msisdn address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match map context sm msisdn address"), asn1::ParameterType::_pt_gsmmap_msisdn_digits, SRCT_MAP_SM);
	    tmp_params.set_bool(SRPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm msisdn address] in rule [%s]!",
							  rule_root->to_cstr("route match map context sm msisdn address"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-rp
    if(rule_root->val_exists("route match smstpdu tp-rp")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-rp"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-rp"), asn1::ParameterType::_pt_smstpdu_tp_rp);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-rp] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-rp"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-udhi
    if(rule_root->val_exists("route match smstpdu tp-udhi")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-udhi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-udhi"), asn1::ParameterType::_pt_smstpdu_tp_udhi);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-udhi] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-udhi"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-srr
    if(rule_root->val_exists("route match smstpdu tp-srr")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-srr"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-srr"), asn1::ParameterType::_pt_smstpdu_tp_srr);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-srr] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-srr"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-vpf
    if(rule_root->val_exists("route match smstpdu tp-vpf")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-vpf"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-vpf"), asn1::ParameterType::_pt_smstpdu_tp_vpf);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-vpf] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-vpf"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-rd
    if(rule_root->val_exists("route match smstpdu tp-rd")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-rd"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-rd"), asn1::ParameterType::_pt_smstpdu_tp_rd);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-rd] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-rd"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-mti
    if(rule_root->val_exists("route match smstpdu tp-mti")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-mti"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-mti"), asn1::ParameterType::_pt_smstpdu_tp_mti);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-mti] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-mti"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-mr
    if(rule_root->val_exists("route match smstpdu tp-mr")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-mr"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-mr"), asn1::ParameterType::_pt_smstpdu_tp_mr);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-mr] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-mr"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-sri
    if(rule_root->val_exists("route match smstpdu tp-sri")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-sri"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-sri"), asn1::ParameterType::_pt_smstpdu_tp_sri);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-sri] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-sri"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-mms
    if(rule_root->val_exists("route match smstpdu tp-mms")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-mms"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-mms"), asn1::ParameterType::_pt_smstpdu_tp_mms);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-mms] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-mms"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-da ton
    if(rule_root->val_exists("route match smstpdu tp-da ton")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-da ton"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-da ton"), asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-da ton] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-da ton"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-da np
    if(rule_root->val_exists("route match smstpdu tp-da np")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-da np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-da np"), asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-da np] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-da np"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-da address
    if(rule_root->val_exists("route match smstpdu tp-da address")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-da address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-da address"), asn1::ParameterType::_pt_smstpdu_tp_da_digits);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-da address] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-da address"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-oa ton
    if(rule_root->val_exists("route match smstpdu tp-oa ton")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-oa ton"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-oa ton"), asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-oa ton] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-oa ton"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-oa np
    if(rule_root->val_exists("route match smstpdu tp-oa np")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-oa np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-oa np"), asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-oa np] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-oa np"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-oa address
    if(rule_root->val_exists("route match smstpdu tp-oa address")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-oa address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-oa address"), asn1::ParameterType::_pt_smstpdu_tp_oa_digits);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-oa address] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-oa address"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-pid
    if(rule_root->val_exists("route match smstpdu tp-pid")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-pid"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-pid"), asn1::ParameterType::_pt_smstpdu_tp_pid);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-pid] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-pid"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-dcs
    if(rule_root->val_exists("route match smstpdu tp-dcs")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-dcs"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-dcs"), asn1::ParameterType::_pt_smstpdu_tp_dcs);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-dcs] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-dcs"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-vp
    if(rule_root->val_exists("route match smstpdu tp-vp")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-vp"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-vp"), asn1::ParameterType::_pt_smstpdu_tp_vp);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-vp] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-vp"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-udl
    if(rule_root->val_exists("route match smstpdu tp-udl")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-udl"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-udl"), asn1::ParameterType::_pt_smstpdu_tp_udl);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-udl] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-udl"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-scts
    if(rule_root->val_exists("route match smstpdu tp-scts")){
	if(field_valid(rule_root->to_cstr("route match smstpdu tp-scts"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu tp-scts"), asn1::ParameterType::_pt_smstpdu_tp_scts);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-scts] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu tp-scts"),
							  rule_root->name.c_str());
    }

    // smstpdu ie-msg-id
    if(rule_root->val_exists("route match smstpdu ie-msg-id")){
	if(field_valid(rule_root->to_cstr("route match smstpdu ie-msg-id"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu ie-msg-id"), asn1::ParameterType::_pt_smstpdu_msg_id);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu ie-msg-id] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu ie-msg-id"),
							  rule_root->name.c_str());
    }

    // smstpdu ie-msg-parts
    if(rule_root->val_exists("route match smstpdu ie-msg-parts")){
	if(field_valid(rule_root->to_cstr("route match smstpdu ie-msg-parts"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu ie-msg-parts"), asn1::ParameterType::_pt_smstpdu_msg_parts);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu ie-msg-parts] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu ie-msg-parts"),
							  rule_root->name.c_str());
    }

    // smstpdu ie-msg-part
    if(rule_root->val_exists("route match smstpdu ie-msg-part")){
	if(field_valid(rule_root->to_cstr("route match smstpdu ie-msg-part"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smstpdu ie-msg-part"), asn1::ParameterType::_pt_smstpdu_msg_part);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu ie-msg-part] in rule [%s]!",
							  rule_root->to_cstr("route match smstpdu ie-msg-part"),
							  rule_root->name.c_str());
    }


    // *** smpp ***
    // smpp as
    if(rule_root->val_exists("route match smpp as")){
	if(field_valid(rule_root->to_cstr("route match smpp as"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp as"), asn1::ParameterType::_pt_smpp_as_label);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp as] in rule [%s]!",
							  rule_root->to_cstr("route match smpp as"),
							  rule_root->name.c_str());
    }

    // smpp asp
    if(rule_root->val_exists("route match smpp asp")){
	if(field_valid(rule_root->to_cstr("route match smpp asp"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp asp"), asn1::ParameterType::_pt_smpp_asp_label);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp asp] in rule [%s]!",
							  rule_root->to_cstr("route match smpp asp"),
							  rule_root->name.c_str());
    }


    // smpp command_id
    if(rule_root->val_exists("route match smpp command_id")){
	if(field_valid(rule_root->to_cstr("route match smpp command_id"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp command_id"), asn1::ParameterType::_pt_smpp_command_id);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp command_id] in rule [%s]!",
							  rule_root->to_cstr("route match smpp command_id"),
							  rule_root->name.c_str());
    }

    // smpp source_addr_ton
    if(rule_root->val_exists("route match smpp source_addr_ton")){
	if(field_valid(rule_root->to_cstr("route match smpp source_addr_ton"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp source_addr_ton"), asn1::ParameterType::_pt_smpp_originator_ton);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp source_addr_ton] in rule [%s]!",
							  rule_root->to_cstr("route match smpp source_addr_ton"),
							  rule_root->name.c_str());
    }

    // smpp dest_addr_ton
    if(rule_root->val_exists("route match smpp dest_addr_ton")){
	if(field_valid(rule_root->to_cstr("route match smpp dest_addr_ton"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp dest_addr_ton"), asn1::ParameterType::_pt_smpp_recipient_ton);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp dest_addr_ton] in rule [%s]!",
							  rule_root->to_cstr("route match smpp dest_addr_ton"),
							  rule_root->name.c_str());
    }

    // smpp source_addr_npi
    if(rule_root->val_exists("route match smpp source_addr_npi")){
	if(field_valid(rule_root->to_cstr("route match smpp source_addr_npi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp source_addr_npi"), asn1::ParameterType::_pt_smpp_originator_np);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp source_addr_npi] in rule [%s]!",
							  rule_root->to_cstr("route match smpp source_addr_npi"),
							  rule_root->name.c_str());
    }

    // smpp dest_addr_npi
    if(rule_root->val_exists("route match smpp dest_addr_npi")){
	if(field_valid(rule_root->to_cstr("route match smpp dest_addr_npi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp dest_addr_npi"), asn1::ParameterType::_pt_smpp_recipient_np);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp dest_addr_npi] in rule [%s]!",
							  rule_root->to_cstr("route match smpp dest_addr_npi"),
							  rule_root->name.c_str());
    }

    // smpp source_addr
    if(rule_root->val_exists("route match smpp source_addr")){
	if(field_valid(rule_root->to_cstr("route match smpp source_addr"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp source_addr"), asn1::ParameterType::_pt_smpp_originator_address);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp source_addr] in rule [%s]!",
							  rule_root->to_cstr("route match smpp source_addr"),
							  rule_root->name.c_str());
    }

    // smpp destination_addr
    if(rule_root->val_exists("route match smpp destination_addr")){
	if(field_valid(rule_root->to_cstr("route match smpp destination_addr"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp destination_addr"), asn1::ParameterType::_pt_smpp_recipient_address);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp destination_addr] in rule [%s]!",
							  rule_root->to_cstr("route match smpp destination_addr"),
							  rule_root->name.c_str());
    }

    // smpp esm_class_mm
    if(rule_root->val_exists("route match smpp esm_class_mm")){
	if(field_valid(rule_root->to_cstr("route match smpp esm_class_mm"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp esm_class_mm"), asn1::ParameterType::_pt_smpp_esm_message_mode);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp esm_class_mm] in rule [%s]!",
							  rule_root->to_cstr("route match smpp esm_class_mm"),
							  rule_root->name.c_str());
    }

    // smpp esm_class_mt
    if(rule_root->val_exists("route match smpp esm_class_mt")){
	if(field_valid(rule_root->to_cstr("route match smpp esm_class_mt"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp esm_class_mt"), asn1::ParameterType::_pt_smpp_esm_message_type);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp esm_class_mt] in rule [%s]!",
							  rule_root->to_cstr("route match smpp esm_class_mt"),
							  rule_root->name.c_str());
    }

    // smpp esm_class_gsm
    if(rule_root->val_exists("route match smpp esm_class_gsm")){
	if(field_valid(rule_root->to_cstr("route match smpp esm_class_gsm"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp esm_class_gsm"), asn1::ParameterType::_pt_smpp_esm_gsm_features);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp esm_class_gsm] in rule [%s]!",
							  rule_root->to_cstr("route match smpp esm_class_gsm"),
							  rule_root->name.c_str());
    }

    // smpp protocol_id
    if(rule_root->val_exists("route match smpp protocol_id")){
	if(field_valid(rule_root->to_cstr("route match smpp protocol_id"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp protocol_id"), asn1::ParameterType::_pt_smpp_protocol_id);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp protocol_id] in rule [%s]!",
							  rule_root->to_cstr("route match smpp protocol_id"),
							  rule_root->name.c_str());
    }

    // smpp priority_flag
    if(rule_root->val_exists("route match smpp priority_flag")){
	if(field_valid(rule_root->to_cstr("route match smpp priority_flag"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp priority_flag"), asn1::ParameterType::_pt_smpp_priority_flag);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp priority_flag] in rule [%s]!",
							  rule_root->to_cstr("route match smpp priority_flag"),
							  rule_root->name.c_str());
    }

    // smpp delivery_time
    if(rule_root->val_exists("route match smpp delivery_time")){
	if(field_valid(rule_root->to_cstr("route match smpp delivery_time"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp delivery_time"), asn1::ParameterType::_pt_smpp_delivery_time);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp delivery_time] in rule [%s]!",
							  rule_root->to_cstr("route match smpp delivery_time"),
							  rule_root->name.c_str());
    }

    // smpp validity_period
    if(rule_root->val_exists("route match smpp validity_period")){
	if(field_valid(rule_root->to_cstr("route match smpp validity_period"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp validity_period"), asn1::ParameterType::_pt_smpp_validity_period);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp validity_period] in rule [%s]!",
							  rule_root->to_cstr("route match smpp validity_period"),
							  rule_root->name.c_str());
    }

    // smpp rd_smsc_dlvr_rcpt
    if(rule_root->val_exists("route match smpp rd_smsc_dlvr_rcpt")){
	if(field_valid(rule_root->to_cstr("route match smpp rd_smsc_dlvr_rcpt"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp rd_smsc_dlvr_rcpt"), asn1::ParameterType::_pt_smpp_rd_smsc_receipt);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp rd_smsc_dlvr_rcpt] in rule [%s]!",
							  rule_root->to_cstr("route match smpp rd_smsc_dlvr_rcpt"),
							  rule_root->name.c_str());
    }

    // smpp rd_sme_orig_ack
    if(rule_root->val_exists("route match smpp rd_sme_orig_ack")){
	if(field_valid(rule_root->to_cstr("route match smpp rd_sme_orig_ack"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp rd_sme_orig_ack"), asn1::ParameterType::_pt_smpp_rd_sme_ack);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp rd_sme_orig_ack] in rule [%s]!",
							  rule_root->to_cstr("route match smpp rd_sme_orig_ack"),
							  rule_root->name.c_str());
    }

    // smpp rd_intrmd_ntf
    if(rule_root->val_exists("route match smpp rd_intrmd_ntf")){
	if(field_valid(rule_root->to_cstr("route match smpp rd_intrmd_ntf"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp rd_intrmd_ntf"), asn1::ParameterType::_pt_smpp_rd_intermediate_notification);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp rd_intrmd_ntf] in rule [%s]!",
							  rule_root->to_cstr("route match smpp rd_intrmd_ntf"),
							  rule_root->name.c_str());
    }

    // smpp replace_if_present_flag
    if(rule_root->val_exists("route match smpp replace_if_present_flag")){
	if(field_valid(rule_root->to_cstr("route match smpp replace_if_present_flag"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp replace_if_present_flag"), asn1::ParameterType::_pt_smpp_replace_if_present_flag);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp replace_if_present_flag] in rule [%s]!",
							  rule_root->to_cstr("route match smpp replace_if_present_flag"),
							  rule_root->name.c_str());
    }

    // smpp data_coding
    if(rule_root->val_exists("route match smpp data_coding")){
	if(field_valid(rule_root->to_cstr("route match smpp data_coding"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp data_coding"), asn1::ParameterType::_pt_smpp_data_coding);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp data_coding] in rule [%s]!",
							  rule_root->to_cstr("route match smpp data_coding"),
							  rule_root->name.c_str());
    }

    // smpp sm_default_msg_id
    if(rule_root->val_exists("route match smpp sm_default_msg_id")){
	if(field_valid(rule_root->to_cstr("route match smpp sm_default_msg_id"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp sm_default_msg_id"), asn1::ParameterType::_pt_smpp_sm_defaut_msg_id);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sm_default_msg_id] in rule [%s]!",
							  rule_root->to_cstr("route match smpp sm_default_msg_id"),
							  rule_root->name.c_str());
    }

    // smpp sm_length
    if(rule_root->val_exists("route match smpp sm_length")){
	if(field_valid(rule_root->to_cstr("route match smpp sm_length"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp sm_length"), asn1::ParameterType::_pt_smpp_sm_length);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sm_length] in rule [%s]!",
							  rule_root->to_cstr("route match smpp sm_length"),
							  rule_root->name.c_str());
    }

    // smpp sar_msg_ref_num
    if(rule_root->val_exists("route match smpp sar_msg_ref_num")){
	if(field_valid(rule_root->to_cstr("route match smpp sar_msg_ref_num"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp sar_msg_ref_num"), asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sar_msg_ref_num] in rule [%s]!",
							  rule_root->to_cstr("route match smpp sar_msg_ref_num"),
							  rule_root->name.c_str());
    }

    // smpp sar_total_segments
    if(rule_root->val_exists("route match smpp sar_total_segments")){
	if(field_valid(rule_root->to_cstr("route match smpp sar_total_segments"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp sar_total_segments"), asn1::ParameterType::_pt_smpp_sar_total_segments);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sar_total_segments] in rule [%s]!",
							  rule_root->to_cstr("route match smpp sar_total_segments"),
							  rule_root->name.c_str());
    }

    // smpp sar_segment_seqnum
    if(rule_root->val_exists("route match smpp sar_segment_seqnum")){
	if(field_valid(rule_root->to_cstr("route match smpp sar_segment_seqnum"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("route match smpp sar_segment_seqnum"), asn1::ParameterType::_pt_smpp_sar_segment_seqnum);
	    tmp_params.set_bool(SRPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sar_segment_seqnum] in rule [%s]!",
							  rule_root->to_cstr("route match smpp sar_segment_seqnum"),
							  rule_root->name.c_str());
    }


    // *** translate part ***
    // *** set r14p translate data ***
    // trunk label
    if(rule_root->val_exists("route translate r14p trunk_label")){
	if(field_valid(rule_root->to_cstr("route translate r14p trunk_label")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_pmink_trunk_label, rule_root->to_cstr("route translate r14p trunk_label"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: r14p trunk_label] in rule [%s]!",
							  rule_root->to_cstr("route translate r14p trunk_label"),
							  rule_root->name.c_str());
    }

    // service id
    if(rule_root->val_exists("route translate r14p service_id")){
	if(field_valid(rule_root->to_cstr("route translate r14p service_id")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_pmink_service_id, rule_root->to_cstr("route translate r14p service_id"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: r14p service_id] in rule [%s]!",
							  rule_root->to_cstr("route translate r14p service_id"),
							  rule_root->name.c_str());
    }


    // *** set m3ua translate data ***
    // m3ua opc
    if(rule_root->val_exists("route translate m3ua opc")){
	if(field_valid(rule_root->to_cstr("route translate m3ua opc")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, rule_root->to_cstr("route translate m3ua opc"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua opc] in rule [%s]!",
							  rule_root->to_cstr("route translate m3ua opc"),
							  rule_root->name.c_str());
    }

    // m3ua dpc
    if(rule_root->val_exists("route translate m3ua dpc")){
	if(field_valid(rule_root->to_cstr("route translate m3ua dpc")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, rule_root->to_cstr("route translate m3ua dpc"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua dpc] in rule [%s]!",
							  rule_root->to_cstr("route translate m3ua dpc"),
							  rule_root->name.c_str());
    }

    // m3ua si
    if(rule_root->val_exists("route translate m3ua si")){
	if(field_valid(rule_root->to_cstr("route translate m3ua si")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, rule_root->to_cstr("route translate m3ua si"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua si] in rule [%s]!",
							  rule_root->to_cstr("route translate m3ua si"),
							  rule_root->name.c_str());
    }

    // m3ua ni
    if(rule_root->val_exists("route translate m3ua ni")){
	if(field_valid(rule_root->to_cstr("route translate m3ua ni")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, rule_root->to_cstr("route translate m3ua ni"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua ni] in rule [%s]!",
							  rule_root->to_cstr("route translate m3ua ni"),
							  rule_root->name.c_str());
    }

    // m3ua mp
    if(rule_root->val_exists("route translate m3ua mp")){
	if(field_valid(rule_root->to_cstr("route translate m3ua mp")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, rule_root->to_cstr("route translate m3ua mp"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua mp] in rule [%s]!",
							  rule_root->to_cstr("route translate m3ua mp"),
							  rule_root->name.c_str());
    }

    // m3ua sls
    if(rule_root->val_exists("route translate m3ua sls")){
	if(field_valid(rule_root->to_cstr("route translate m3ua sls")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, rule_root->to_cstr("route translate m3ua sls"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua sls] in rule [%s]!",
							  rule_root->to_cstr("route translate m3ua sls"),
							  rule_root->name.c_str());
    }

    // *** set sccp translate data ***
    // **** calling party ****
    // sccp calling party routing indicator
    if(rule_root->val_exists("route translate sccp cgpa routing-indicator")){
	if(field_valid(rule_root->to_cstr("route translate sccp cgpa routing-indicator")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, rule_root->to_cstr("route translate sccp cgpa routing-indicator"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa routing-indicator] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cgpa routing-indicator"),
							  rule_root->name.c_str());
    }

    // sccp calling party gti
    if(rule_root->val_exists("route translate sccp cgpa gti")){
	if(field_valid(rule_root->to_cstr("route translate sccp cgpa gti")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, rule_root->to_cstr("route translate sccp cgpa gti"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gti] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cgpa gti"),
							  rule_root->name.c_str());
    }

    // sccp calling party ssn
    if(rule_root->val_exists("route translate sccp cgpa ssn")){
	if(field_valid(rule_root->to_cstr("route translate sccp cgpa ssn")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, rule_root->to_cstr("route translate sccp cgpa ssn"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa ssn] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cgpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp calling party point code
    if(rule_root->val_exists("route translate sccp cgpa point-code")){
	if(field_valid(rule_root->to_cstr("route translate sccp cgpa ssn")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, rule_root->to_cstr("route translate sccp cgpa point-code"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa ssn] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cgpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title tt
    if(rule_root->val_exists("route translate sccp cgpa gt tt")){
	if(field_valid(rule_root->to_cstr("route translate sccp cgpa gt tt")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, rule_root->to_cstr("route translate sccp cgpa gt tt"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gt tt] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cgpa gt tt"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title np
    if(rule_root->val_exists("route translate sccp cgpa gt np")){
	if(field_valid(rule_root->to_cstr("route translate sccp cgpa gt np")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, rule_root->to_cstr("route translate sccp cgpa gt np"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gt np] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cgpa gt np"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title nai
    if(rule_root->val_exists("route translate sccp cgpa gt nai")){
	if(field_valid(rule_root->to_cstr("route translate sccp cgpa gt nai")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, rule_root->to_cstr("route translate sccp cgpa gt nai"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gt nai] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cgpa gt nai"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title address
    if(rule_root->val_exists("route translate sccp cgpa gt address")){
	if(field_valid(rule_root->to_cstr("route translate sccp cgpa gt address")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, rule_root->to_cstr("route translate sccp cgpa gt address"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gt address] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cgpa gt address"),
							  rule_root->name.c_str());
    }

    // **** called party ****
    // sccp called party routing indicator
    if(rule_root->val_exists("route translate sccp cdpa routing-indicator")){
	if(field_valid(rule_root->to_cstr("route translate sccp cdpa routing-indicator")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, rule_root->to_cstr("route translate sccp cdpa routing-indicator"));

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa routing-indicator] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cdpa routing-indicator"),
							  rule_root->name.c_str());
    }

    // sccp called party gti
    if(rule_root->val_exists("route translate sccp cdpa gti")){
	if(field_valid(rule_root->to_cstr("route translate sccp cdpa gti")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, rule_root->to_cstr("route translate sccp cdpa gti"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gti] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cdpa gti"),
							  rule_root->name.c_str());
    }

    // sccp called party ssn
    if(rule_root->val_exists("route translate sccp cdpa ssn")){
	if(field_valid(rule_root->to_cstr("route translate sccp cdpa ssn")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, rule_root->to_cstr("route translate sccp cdpa ssn"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa ssn] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cdpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp called party point code
    if(rule_root->val_exists("route translate sccp cdpa point-code")){
	if(field_valid(rule_root->to_cstr("route translate sccp cdpa point-code")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, rule_root->to_cstr("route translate sccp cdpa point-code"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa point-code] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cdpa point-code"),
							  rule_root->name.c_str());
    }

    // sccp called party global title tt
    if(rule_root->val_exists("route translate sccp cdpa gt tt")){
	if(field_valid(rule_root->to_cstr("route translate sccp cdpa gt tt")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, rule_root->to_cstr("route translate sccp cdpa gt tt"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gt tt] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cdpa gt tt"),
							  rule_root->name.c_str());
    }

    // sccp called party global title np
    if(rule_root->val_exists("route translate sccp cdpa gt np")){
	if(field_valid(rule_root->to_cstr("route translate sccp cdpa gt np")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, rule_root->to_cstr("route translate sccp cdpa gt np"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gt np] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cdpa gt np"),
							  rule_root->name.c_str());
    }

    // sccp called party global title nai
    if(rule_root->val_exists("route translate sccp cdpa gt nai")){
	if(field_valid(rule_root->to_cstr("route translate sccp cdpa gt nai")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, rule_root->to_cstr("route translate sccp cdpa gt nai"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gt nai] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cdpa gt nai"),
							  rule_root->name.c_str());
    }

    // sccp called party global title address
    if(rule_root->val_exists("route translate sccp cdpa gt address")){
	if(field_valid(rule_root->to_cstr("route translate sccp cdpa gt address")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_address, rule_root->to_cstr("route translate sccp cdpa gt address"), SPIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gt address] in rule [%s]!",
							  rule_root->to_cstr("route translate sccp cdpa gt address"),
							  rule_root->name.c_str());
    }

    // *** destinations ***
    build_dests((*rule_root)("route destination"), &tmp_params);

    // check if rule with the same priority already exists
    stp_rule_t* rule = rm->get_rule(rule_root->to_int("priority"));
    if(rule != NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot add rule [%s], rule with priority level [%d] already exists!",
						      rule_root->name.c_str(),
						      rule_root->to_int("priority"));

	return 1;
    }

    // add new rule
    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO,  "Adding rule [%s] with priority level [%d]...",
						  rule_root->name.c_str(),
						  rule_root->to_int("priority"));


    // get daemon
    StpdDescriptor* dd = (StpdDescriptor*)pmink::CURRENT_DAEMON;
    // add stats manager
    stp::StatsManagerRefc* stats_m = new stp::StatsManagerRefc();
    // save in param map
    tmp_params.set_pointer(1, stats_m, 0, 0, SRCT_PMINK);
    // get stats manager
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)dd->get_param(3);
    // register stats
    stats_m->register_item(stp::SST_RULE_COUNTER);
    // add trap
    r14p_stats->add_trap(r14p::TrapId(std::string(rule_root->name + "_RULE_COUNTER")), new stp::StpStatsHandler(stats_m, stp::SST_RULE_COUNTER));

    // add rule
    stp_rule_t* new_rule = rm->new_rule(rule_root->name.c_str(), rule_root->to_int("priority"), tmp_params, &rlogic);

    PMDLOG(
	new_rule->get_data_p()->set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
	std::cout << "===== RULE: " << new_rule->get_label() << " =====" << std::endl;
	std::cout << *new_rule->get_data_p();
	std::cout << "=====================" << std::endl;
    )
    return 0;
}

int stp::StpRouting::build_dests(config::ConfigItem* dests_root, stp_rule_param_t* params){
    if(dests_root == NULL || dests_root->name != "destination") return 1;

    int c = 0;
    std::map<uint32_t, config::ConfigItem*> tmp_map;
    config::ConfigItem* dest_rule = NULL;

    // clear
    params->erase_param(asn1::ParameterType::_pt_pmink_routing_destination, -1);
    params->erase_param(asn1::ParameterType::_pt_pmink_routing_destination_type, -1);
    params->erase_param(asn1::ParameterType::_pt_pmink_routing_sub_destination, -1);
    params->erase_param(asn1::ParameterType::_pt_pmink_routing_sub_destination_type, -1);

    // loop destinations
    for(unsigned int i = 0; i<dests_root->children.size(); i++){
	// dest rule
	dest_rule = dests_root->children[i];
	// skip if deleted
	if(dest_rule->node_state == config::CONFIG_NS_DELETED) continue;
	// add to tmp map
	tmp_map[dest_rule->to_int("priority")] = dest_rule;
    }

    // tmp map sorted by priority, read
    for(std::map<uint32_t, config::ConfigItem*>::iterator it = tmp_map.begin(); it != tmp_map.end(); it++){
	// dest rule
	dest_rule = it->second;
	// r14p destination
	if((*dest_rule)("r14p") != NULL && (*dest_rule)("r14p")->node_state != config::CONFIG_NS_DELETED && dest_rule->val_exists("r14p")){
	    params->set_cstr(asn1::ParameterType::_pt_pmink_routing_destination, dest_rule->to_cstr("r14p"), c);

	    // subtype
	    if((*dest_rule)("as") != NULL && (*dest_rule)("as")->node_state != config::CONFIG_NS_DELETED && dest_rule->val_exists("as")){
		params->set_int(asn1::ParameterType::_pt_pmink_routing_sub_destination_type, 1, c);
		params->set_cstr(asn1::ParameterType::_pt_pmink_routing_sub_destination, dest_rule->to_cstr("as"), c);

	    }

	    // service id
	    if((*dest_rule)("service_id") != NULL && (*dest_rule)("service_id")->node_state != config::CONFIG_NS_DELETED && dest_rule->val_exists("service_id")){
		rlogic.find_set_param(dest_rule->to_cstr("service_id"),
				      params,
				      params,
				      pmink_utils::DPT_INT,
				      asn1::ParameterType::_pt_pmink_routing_service_id,
				      c);
	    }
	    ++c;
	}
    }

    return 0;

}


int stp::StpRouting::build_list(config::ConfigItem* list_root, list_m_t* lm){
    // sanity check
    if(list_root == NULL) return 1;
    // tmp vars
    config::ConfigItem* tmp_node = NULL;
    config::ConfigItem* values_node = NULL;
    // add list
    lm->add_list(list_root->name);
    // values node
    values_node = (*list_root)("values");
    if(values_node == NULL) return 1;
    // loop list items
    for(unsigned int i = 0; i<values_node->children.size(); i++){
	// config list item node pointer
	tmp_node = values_node->children[i];
	// skip deleted
	if(tmp_node->node_state == config::CONFIG_NS_DELETED) continue;
	// add item
	lm->add_sublist_item(list_root->name, tmp_node->name);
    }

    return 0;
}


int stp::StpRouting::build_lists(config::ConfigItem* cfg_root, list_m_t* lm){
    // sanity check
    if(cfg_root == NULL || cfg_root->name != "lists") return 1;
    // clear lists
    lm->clear();
    // tmp vars
    config::ConfigItem* tmp_node = NULL;
    // loop lists
    for(unsigned int i = 0; i<cfg_root->children.size(); i++){
	// config list node pointer
	tmp_node = cfg_root->children[i];
	// skip deleted
	if(tmp_node->node_state == config::CONFIG_NS_DELETED) continue;
	// build rule
	build_list(tmp_node, lm);
    }

    // ok
    return 0;
}


int stp::StpRouting::build_rules(config::ConfigItem* cfg_root, rule_m_t* rm){
    // sanity check
    if(cfg_root == NULL || cfg_root->name != "routing") return 1;
    // get daemon
    StpdDescriptor* dd = (StpdDescriptor*)pmink::CURRENT_DAEMON;
    // get stats manager
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)dd->get_param(3);
    // free rule stats handlers
    for(rule_m_t::rules_map_it_t it = rm->get_begin(); it != rm->get_end(); it++){
	// rule ref
	stp_rule_t& rule_ref = it->second;
	// rule label
	std::string rule_lbl(rule_ref.get_label());
	// remove stats handler
	delete r14p_stats->remove_trap(r14p::TrapId(std::string(rule_lbl + "_RULE_COUNTER")));
	// get stats manager
	stp::StatsManagerRefc* stats_m = (stp::StatsManagerRefc*)(void*)*rule_ref.get_data_p()->get_param(1, 0, 0, SRCT_PMINK);
	// free stats manager (delayed deallocation)
	stats_m->refc_dec();
    }


    // clear rules
    rm->clear();
    // tmp vars
    config::ConfigItem* tmp_node = NULL;
    // loop rules
    for(unsigned int i = 0; i<cfg_root->children.size(); i++){
	// config rule node pointer
	tmp_node = cfg_root->children[i];
	// skip deleted
	if(tmp_node->node_state == config::CONFIG_NS_DELETED) continue;
	// build rule
	build_rule(tmp_node, rm);

    }

    // ok
    return 0;
}


// StpdDescriptor
StpdDescriptor::StpdDescriptor(const char* _type, const char* _desc): pmink::DaemonDescriptor(_type, NULL, _desc),
								      r14ps(NULL),
								      r14p_stats(NULL),
								      r14psmm(NULL),
								      cfgd_r14pc(NULL),
								      hbeat(NULL),
								      non_srvc_hdnlr(NULL),
								      dest_r14pc(NULL){

	config = new config::Config();
	bzero(cfgd_id, sizeof(cfgd_id));

	// default extra param values
	// --r14p-streams
	extra_params.set_int(0, 10000);
	// --r14p-stimeout
	extra_params.set_int(1, 5);
	// --r14p-smsg-pool
	extra_params.set_int(2, 1000);
	// --r14p-sparam-pool
	extra_params.set_int(3, 100000);

	// stats
	stats.register_item(stp::SST_UNSUPPORTED_R14P_SERVICE);
	stats.register_item(stp::SST_SMSG_SEND_ERROR);
	stats.register_item(stp::SST_SMSG_MSG_POOL_EMPTY);
	stats.register_item(stp::SST_SMSG_PARAM_POOL_EMPTY);
	stats.register_item(stp::SST_SMSG_INCOMPLETE);
	stats.register_item(stp::SST_SMSG_RX_COUNT);
	//stats.register_item(stp::SST_SMSG_TX_COUNT);
	stats.register_item(stp::SST_NO_VALID_ROUTES);


}

StpdDescriptor::~StpdDescriptor(){
	// free routing deamons address strings
	for(unsigned int i = 0; i<routing_daemons.size(); i++) delete routing_daemons[i];
	// service message manager
	delete r14psmm;
	// r14p stats
	delete r14p_stats;

}

void StpdDescriptor::process_arguements(int argc, char** argv){
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");
	int opt;
	int option_index = 0;
	struct option long_options[] = {
	    {"r14p-streams", required_argument, 0, 0},
	    {"r14p-stimeout", required_argument, 0, 0},
	    {"r14p-smsg-pool", required_argument, 0, 0},
	    {"r14p-sparam-pool", required_argument, 0, 0},
	    {0, 0, 0, 0}
	};


	if(argc < 5){
		print_help();
		exit(EXIT_FAILURE);
		return;
	}else{
		while ((opt = getopt_long(argc, argv, "?r:i:D", long_options, &option_index)) != -1) {
			switch(opt){
				// long options
				case 0:
				   if(long_options[option_index].flag != 0) break;
				   switch(option_index){
				       // r14p-streams
				       case 0:
					   extra_params.set_int(0, atoi(optarg));
					   break;

				       // r14p-stimeout
				       case 1:
					   extra_params.set_int(1, atoi(optarg));
					   break;

				       // r14p-smsg-pool
				       case 2:
					   extra_params.set_int(2, atoi(optarg));
					   break;

				       // r14p-sparam-pool
				       case 3:
					   extra_params.set_int(3, atoi(optarg));
					   break;
				   }
				   break;

				// help
				case '?':
					print_help();
					exit(EXIT_FAILURE);


				// daemon id
				case 'i':
				    if(set_daemon_id(optarg) > 0){
					std::cout << "ERROR: Maximum size of daemon id string is 15 characters!" << std::endl;
					exit(EXIT_FAILURE);

				    }
				    break;

				// routing daemon address
				case 'r':
				    // check pattern (ipv4:port)
				    // check if valid
				    if(!boost::regex_match(optarg, addr_regex)){
					    std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
					    exit(EXIT_FAILURE);

				    }else{
					    routing_daemons.push_back(new std::string(optarg));

				    }
				    break;

				// debug mode
				case 'D':
				    set_log_level(pmink::LLT_DEBUG);
				    break;


			}
		}

		// check mandatory id
		if(strlen(get_daemon_id()) == 0){
			std::cout << "ERROR: Daemon id not defined!" << std::endl;
			exit(EXIT_FAILURE);

		}

		// check for routing daemons
		if(routing_daemons.size() == 0){
			std::cout << "ERROR: Routing daemon(s) not defined!" << std::endl;
			exit(EXIT_FAILURE);

		}


	}

}

void StpdDescriptor::print_help(){
	std::cout << daemon_type << " - " << daemon_description << std::endl;
	std::cout << "Copyright (c) 2012 Release14.org" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << " -?\thelp" << std::endl;
	std::cout << " -i\tunique daemon id" << std::endl;
	std::cout << " -r\trouting daemon address (ipv4:port)" << std::endl;
	std::cout << " -D\tstart in debug mode" << std::endl;
	std::cout << std::endl;
	std::cout << "R14P Options:" << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << " --r14p-streams\t\tR14P Session stream pool\t\t(default = 10000)" << std::endl;
	std::cout << " --r14p-stimeout\tR14P Stream timeout in seconds\t\t(default = 5)" << std::endl;
	std::cout << " --r14p-smsg-pool\tR14P Service message pool\t\t(default = 1000)" << std::endl;
	std::cout << " --r14p-sparam-pool\tR14P Service message parameter pool\t(default = 100000)" << std::endl;

}

int StpdDescriptor::init_config(bool _process_config){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting config daemon registration procedure...");

	// create root node string
	std::string root_node_str(DAEMON_CFG_NODE);
	root_node_str.append(" ");
	root_node_str.append(daemon_id);

	// loop routing daemons
	for(unsigned int i = 0; i<r14ps->get_client_count(); i++){
		// get client
		r14p::R14PClient* r14p_client = r14ps->get_client(i);
		// null check
		if(r14p_client != NULL && r14p_client->is_registered()){
			// log
			pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, 	"Connection to remote daemon established, L3 address = [%s:%d], R14P address = [%s:%s]",
									r14p_client->get_end_point_address(),
									r14p_client->get_end_point_port() ,
									r14p_client->get_end_point_daemon_type(),
									r14p_client->get_end_point_daemon_id());
			// check for active configd
			if(!cfgd_active.get()){
				// user login
				if(config::user_login(config, r14p_client, NULL, (char*)cfgd_id, &cfgd_uid) == 0){
				    if(strlen((char*)cfgd_id) > 0){
					// log
					pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "User [%s] successfully authenticated with config daemon [%s]", cfgd_uid.user_id, cfgd_id);
					// notification request
					if(config::notification_request(config, r14p_client, root_node_str.c_str(), NULL, (char*)cfgd_id, &cfgd_uid, NULL) == 0){
						// create hbeat events
						HbeatRecv* hb_recv = new HbeatRecv();
						HbeatMissed* hb_missed = new HbeatMissed(&cfgd_active);
						HbeatCleanup* hb_cleanup = new HbeatCleanup(hb_recv, hb_missed);

						// init hbeat
						hbeat = r14p::init_heartbeat("config_daemon", (char*)cfgd_id, r14p_client, 5, hb_recv, hb_missed, hb_cleanup);
						if(hbeat != NULL){
							cfgd_active.comp_swap(false, true);
							// log
							pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting R14P HBEAT for config daemon [%s], L3 address = [%s:%d]",
										   cfgd_id,
										   r14p_client->get_end_point_address(),
										   r14p_client->get_end_point_port());

						// free event memory on error
						}else{
						    delete hb_recv;
						    delete hb_missed;
						    delete hb_cleanup;
						}


						// log
						pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Registering notification request for node path [%s] with config daemon [%s]", root_node_str.c_str(), cfgd_id);
						// process config
						if(_process_config) process_config();

						// stop if config daemon connected
						// ok
						return 0;

					}else{
					    // log
					    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,	"Error while requesting notifications from config daemon [%s]!",
												cfgd_id);

					}

				    }else{
					    // log
					    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,	"Error while trying to find config daemon id via R14P connection, L3 address = [%s:%d], R14P address = [%s:%s]",
												r14p_client->get_end_point_address(),
												r14p_client->get_end_point_port() ,
												r14p_client->get_end_point_daemon_type(),
												r14p_client->get_end_point_daemon_id());

				    }

				}else{
					// log
					pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, 	"Error while trying to authenticate user [%s] with config daemon [%s:%d]!",
											cfgd_uid.user_id,
											r14p_client->get_end_point_address(),
											r14p_client->get_end_point_port());

				}

			}

		}else{
			// log
			pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Error while connecting to config daemon [%s:%d]!", r14p_client->get_end_point_address(), r14p_client->get_end_point_port());
		}


	}
	// err
	return 5;
}

void StpdDescriptor::init(){
    // pmdebug
    #ifdef PMDEBUG
    std::map<uint32_t, std::string>* debug_labels = pmink_utils::create_debug_label_map();
    set_param(9999, debug_labels);
    #endif

    // init r14p
    init_r14p();
    // init config
    if(init_config() != 0){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot find any valid config daemon connection, terminating...");
	exit(EXIT_FAILURE);
    }
}

void StpdDescriptor::process_config(){
    // create root node string
    std::string root_node_str(DAEMON_CFG_NODE);
    root_node_str.append(" ");
    root_node_str.append(daemon_id);

    // get node
    config::ConfigItem* root = (*config->get_definition_root())(root_node_str.c_str());

    // check if configuration exists
    if(root == NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Configuration root node [%s] does not exist!", daemon_id);
	exit(EXIT_FAILURE);
    }

    // process configuration
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Configuration for node [%s] successfully received, verifying...", daemon_id);


    // configuration ok, process
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Configuration for node [%s] successfully verified, starting...", daemon_id);


    // build rules
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Building routing rules...");
    rule_m.update_start();
    stp_routing.build_rules((*root)("routing"), &(*rule_m));
    rule_m.update_end();
    // setup config on chage events
    (*root)("routing")->set_on_change_handler(&routing_mod_handler, true);
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Finished building routing rules...");

    // build lists
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Building routing lists...");
    list_m.update_start();
    stp_routing.build_lists((*root)("lists"), &(*list_m));
    list_m.update_end();
    // setup config on chage events
    (*root)("lists")->set_on_change_handler(&lists_mod_handler, true);
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Finished building routing lists...");



}

void StpdDescriptor::init_r14p(){
	// handler for non service message messages
	non_srvc_hdnlr = config::create_cfg_event_handler(config);
	// service message manager
	r14psmm = new r14p::ServiceMsgManager(&idt_map,
					      NULL,
					      non_srvc_hdnlr,
					      (int)*extra_params.get_param(2),
					      (int)*extra_params.get_param(3));

	// set daemon params (searched for in dr::NewSrvcMsg constructor)
	set_param(0, config);
	set_param(1, r14psmm);

	// set service message handlers
	r14psmm->set_new_msg_handler(&new_srvc_msg);
	r14psmm->set_msg_err_handler(&new_srvc_msg.msg_err);

	// start R14P session
	r14ps = r14p::init_session(get_daemon_type(),
				   get_daemon_id(),
				   (int)*extra_params.get_param(0),
				   (int)*extra_params.get_param(1),
				   false,
				   (int)*extra_params.get_param(1));
	set_param(2, r14ps);

	// r14p stats
	r14p_stats = new r14p::R14PStatsSession(5, r14ps);
	set_param(3, r14p_stats);

	// start stats
	r14p_stats->start();

	// init r14p stats
	r14p_stats->init_r14p_session_stats(r14ps);

	// init stp stats
	r14p_stats->add_trap(r14p::TrapId("UNSUPPORTED_R14P_SERVICE"), new stp::StpStatsHandler(&stats, stp::SST_UNSUPPORTED_R14P_SERVICE));
	r14p_stats->add_trap(r14p::TrapId("SMSG_SEND_ERROR"), new stp::StpStatsHandler(&stats, stp::SST_SMSG_SEND_ERROR));
	r14p_stats->add_trap(r14p::TrapId("SMSG_MSG_POOL_EMPTY"), new stp::StpStatsHandler(&stats, stp::SST_SMSG_MSG_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("SMSG_PARAM_POOL_EMPTY"), new stp::StpStatsHandler(&stats, stp::SST_SMSG_PARAM_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("SST_SMSG_INCOMPLETE"), new stp::StpStatsHandler(&stats, stp::SST_SMSG_INCOMPLETE));
	r14p_stats->add_trap(r14p::TrapId("SMSG_RX_COUNT"), new stp::StpStatsHandler(&stats, stp::SST_SMSG_RX_COUNT));
	//r14p_stats->add_trap(r14p::TrapId("SMSG_TX_COUNT"), new stp::StpStatsHandler(&stats, stp::SST_SMSG_TX_COUNT));
	r14p_stats->add_trap(r14p::TrapId("NO_VALID_ROUTES"), new stp::StpStatsHandler(&stats, stp::SST_NO_VALID_ROUTES));


	// connect to routing daemons
	boost::smatch regex_groups;
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");

	// loop routing daemons
	for(unsigned int i = 0; i<routing_daemons.size(); i++){
	    // separate IP and PORT
	    boost::regex_search(*routing_daemons[i], regex_groups, addr_regex);
	    // connect to routing daemon
	    r14p::R14PClient* r14pc = r14ps->connect(regex_groups[1].str().c_str(),
						     atoi(regex_groups[2].str().c_str()),
						     16,
						     NULL,
						     0);

	    // setup client for service messages
	    if(r14pc != NULL) {
		// setup service message event handlers
		r14psmm->setup_client(r14pc);
		// rule processor mao
		rule_pc_map.insert(stp::rulepc_map_t::value_type(r14pc, stp::RuleProcessor(&list_m, &rule_m)));
		pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Setting up Rule processor for [%s:%s]...",
					   r14pc->get_end_point_daemon_id(), r14pc->get_end_point_daemon_type());

	    }

	    // connect stats with routing
	    r14pc = r14p_stats->get_r14p_session()->connect(regex_groups[1].str().c_str(), atoi(regex_groups[2].str().c_str()), 16, NULL, 0);
	    if(r14pc != NULL) r14p_stats->setup_client(r14pc);


	}
	// set sgnd client
	dest_r14pc = r14ps->get_registered_client("routingd");

}

void StpdDescriptor::terminate(){
	// destroy session, free memory
	r14p::destroy_session(r14ps);
	// deallocate config memory
	if(config->get_definition_root() != NULL) delete config->get_definition_root();
	// free config
	delete config;
	// remove stats
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("UNSUPPORTED_R14P_SERVICE")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_SEND_ERROR")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_MSG_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_PARAM_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_INCOMPLETE")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_RX_COUNT")));
	//delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_TX_COUNT")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("NO_VALID_ROUTES")));

	#ifdef PMDEBUG
	delete (std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999);
	#endif

}




