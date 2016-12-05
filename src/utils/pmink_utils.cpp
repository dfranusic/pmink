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

#include <pmink_utils.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <curses.h>
#include <dlfcn.h>
#include <fstream>
#include <math.h>
#include <boost/tokenizer.hpp>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <r14p_asn1.h>


std::map<uint32_t, std::string>* pmink_utils::create_debug_label_map(){
    std::map<uint32_t, std::string>* debug_labels = new std::map<uint32_t, std::string>();
    (*debug_labels)[asn1::ParameterType::_pt_pmink_daemon_type] = "pmink_daemon_type";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_daemon_id] = "pmink_daemon_id";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_auth_id] = "pmink_auth_id";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_auth_password] = "pmink_auth_password";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_daemon_ip] = "pmink_daemon_ip";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_daemon_port] = "pmink_daemon_port";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_daemon_description] = "pmink_daemon_description";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_action] = "pmink_action";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_dpi] = "pmink_dpi";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_spi] = "pmink_spi";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_timestamp] = "pmink_timestamp";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_timestamp_nsec] = "pmink_timestamp_nsec";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_security_phase] = "pmink_security_phase";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_loop_count] = "pmink_loop_count";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_checksum] = "pmink_checksum";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_destination] = "pmink_routing_destination";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_source] = "pmink_routing_source";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_gateway] = "pmink_routing_gateway";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_interface] = "pmink_routing_interface";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_priority] = "pmink_routing_priority";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_router_status] = "pmink_router_status";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_destination_type] = "pmink_routing_destination_type";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_index] = "pmink_routing_index";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_trunk_label] = "pmink_trunk_label";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_connection_type] = "pmink_connection_type";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_service_id] = "pmink_service_id";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_command_id] = "pmink_command_id";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_sub_destination] = "pmink_routing_sub_destination";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_sub_destination_type] = "pmink_routing_sub_destination_type";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_correlation_notification] = "pmink_correlation_notification";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_guid] = "pmink_guid";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_routing_service_id] = "pmink_routing_service_id";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_event_id] = "pmink_event_id";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_event_description] = "pmink_event_description";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_event_callback_id] = "pmink_event_callback_id";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_event_callback_priority] = "pmink_event_callback_priority";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_enc_public_key] = "pmink_enc_public_key";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_enc_private_key] = "pmink_enc_private_key";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_enc_type] = "pmink_enc_type";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_stats_id] = "pmink_stats_id";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_stats_description] = "pmink_stats_description";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_stats_value] = "pmink_stats_value";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_stats_count] = "pmink_stats_count";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_param_name] = "pmink_config_param_name";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_param_value] = "pmink_config_param_value";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_ac_line] = "pmink_config_ac_line";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_item_name] = "pmink_config_cfg_item_name";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_item_desc] = "pmink_config_cfg_item_desc";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_item_ns] = "pmink_config_cfg_item_ns";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_item_value] = "pmink_config_cfg_item_value";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_item_nvalue] = "pmink_config_cfg_item_nvalue";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_item_nt] = "pmink_config_cfg_item_nt";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_cm_mode] = "pmink_config_cfg_cm_mode";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_ac_err] = "pmink_config_cfg_ac_err";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cli_path] = "pmink_config_cli_path";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_line] = "pmink_config_cfg_line";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_ac_err_count] = "pmink_config_ac_err_count";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_line_count] = "pmink_config_cfg_line_count";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_item_path] = "pmink_config_cfg_item_path";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_item_notify] = "pmink_config_cfg_item_notify";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_cfg_item_count] = "pmink_config_cfg_item_count";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_config_replication_line] = "pmink_config_replication_line";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_sms_status] = "pmink_sms_status";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_sms_uuid] = "pmink_sms_uuid";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_filter_result] = "pmink_filter_result";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_filter_exit] = "pmink_filter_exit";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_filter_list_id] = "pmink_filter_list_id";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_filter_list_label] = "pmink_filter_list_label";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_filter_data] = "pmink_filter_data";
    (*debug_labels)[asn1::ParameterType::_pt_pmink_filter_data_size] = "pmink_filter_data_size";
    (*debug_labels)[asn1::ParameterType::_pt_eth_destination_mac] = "eth_destination_mac";
    (*debug_labels)[asn1::ParameterType::_pt_eth_source_mac] = "eth_source_mac";
    (*debug_labels)[asn1::ParameterType::_pt_ip_destination_ip] = "ip_destination_ip";
    (*debug_labels)[asn1::ParameterType::_pt_ip_source_ip] = "ip_source_ip";
    (*debug_labels)[asn1::ParameterType::_pt_tcp_destination_port] = "tcp_destination_port";
    (*debug_labels)[asn1::ParameterType::_pt_tcp_source_port] = "tcp_source_port";
    (*debug_labels)[asn1::ParameterType::_pt_udp_destination_port] = "udp_destination_port";
    (*debug_labels)[asn1::ParameterType::_pt_udp_source_port] = "udp_source_port";
    (*debug_labels)[asn1::ParameterType::_pt_sctp_destination_port] = "sctp_destination_port";
    (*debug_labels)[asn1::ParameterType::_pt_sctp_source_port] = "sctp_source_port";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_scoa_digits] = "gsmmap_scoa_digits";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_scoa_type_of_number] = "gsmmap_scoa_type_of_number";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan] = "gsmmap_scoa_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_scda_digits] = "gsmmap_scda_digits";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_scda_type_of_number] = "gsmmap_scda_type_of_number";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_scda_numbering_plan] = "gsmmap_scda_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_imsi] = "gsmmap_imsi";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_msisdn_digits] = "gsmmap_msisdn_digits";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number] = "gsmmap_msisdn_type_of_number";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan] = "gsmmap_msisdn_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_source_transaction_id] = "tcap_source_transaction_id";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_destination_transaction_id] = "tcap_destination_transaction_id";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_opcode] = "tcap_opcode";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_component_type] = "tcap_component_type";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_component_invoke_id] = "tcap_component_invoke_id";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_error_type] = "tcap_error_type";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_error_code] = "tcap_error_code";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_dialogue_context_oid] = "tcap_dialogue_context_oid";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_message_type] = "tcap_message_type";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_nnn_digits] = "gsmmap_nnn_digits";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_nnn_type_of_number] = "gsmmap_nnn_type_of_number";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan] = "gsmmap_nnn_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_an_digits] = "gsmmap_an_digits";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_an_type_of_number] = "gsmmap_an_type_of_number";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_an_numbering_plan] = "gsmmap_an_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_sca_digits] = "gsmmap_sca_digits";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_sca_type_of_number] = "gsmmap_sca_type_of_number";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_sca_numbering_plan] = "gsmmap_sca_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_component_count] = "tcap_component_count";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_dialogue_context_supported] = "tcap_dialogue_context_supported";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_component_index] = "tcap_component_index";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_source_transaction_id_length] = "tcap_source_transaction_id_length";
    (*debug_labels)[asn1::ParameterType::_pt_tcap_destination_transaction_id_length] = "tcap_destination_transaction_id_length";
    (*debug_labels)[asn1::ParameterType::_pt_gsmmap_version] = "gsmmap_version";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_udhi] = "smstpdu_tp_udhi";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_sri] = "smstpdu_tp_sri";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_mms] = "smstpdu_tp_mms";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_mti] = "smstpdu_tp_mti";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number] = "smstpdu_tp_oa_type_of_number";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan] = "smstpdu_tp_oa_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_oa_digits] = "smstpdu_tp_oa_digits";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_pid] = "smstpdu_tp_pid";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_dcs] = "smstpdu_tp_dcs";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_scts] = "smstpdu_tp_scts";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_udl] = "smstpdu_tp_udl";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_ud] = "smstpdu_tp_ud";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_rp] = "smstpdu_tp_rp";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_srr] = "smstpdu_tp_srr";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_vpf] = "smstpdu_tp_vpf";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_rd] = "smstpdu_tp_rd";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number] = "smstpdu_tp_da_type_of_number";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan] = "smstpdu_tp_da_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_da_digits] = "smstpdu_tp_da_digits";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_vp] = "smstpdu_tp_vp";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_msg_id] = "smstpdu_msg_id";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_msg_parts] = "smstpdu_msg_parts";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_msg_part] = "smstpdu_msg_part";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_tp_mr] = "smstpdu_tp_mr";
    (*debug_labels)[asn1::ParameterType::_pt_smstpdu_message_class] = "smstpdu_message_class";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_destination_local_reference] = "sccp_destination_local_reference";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_source_local_reference] = "sccp_source_local_reference";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_party] = "sccp_called_party";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_party] = "sccp_calling_party";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_protocol_class] = "sccp_protocol_class";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_segmenting_reassembling] = "sccp_segmenting_reassembling";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_receive_sequence_number] = "sccp_receive_sequence_number";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_sequencing_segmenting] = "sccp_sequencing_segmenting";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_credit] = "sccp_credit";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_release_cause] = "sccp_release_cause";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_return_cause] = "sccp_return_cause";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_reset_cause] = "sccp_reset_cause";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_error_cause] = "sccp_error_cause";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_refusal_cause] = "sccp_refusal_cause";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_data] = "sccp_data";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_segmentation] = "sccp_segmentation";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_hop_counter] = "sccp_hop_counter";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_importance] = "sccp_importance";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_long_data] = "sccp_long_data";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_routing_indicator] = "sccp_called_pa_routing_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator] = "sccp_called_pa_global_title_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_ssn_indicator] = "sccp_called_pa_ssn_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_point_code_indicator] = "sccp_called_pa_point_code_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_point_code_number] = "sccp_called_pa_point_code_number";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_subsystem_number] = "sccp_called_pa_subsystem_number";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan] = "sccp_called_pa_gt_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_gt_encoding_scheme] = "sccp_called_pa_gt_encoding_scheme";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address] = "sccp_called_pa_gt_nature_of_address";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_gt_address] = "sccp_called_pa_gt_address";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type] = "sccp_called_pa_gt_translation_type";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator] = "sccp_calling_pa_routing_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator] = "sccp_calling_pa_global_title_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_ssn_indicator] = "sccp_calling_pa_ssn_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_point_code_indicator] = "sccp_calling_pa_point_code_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_point_code_number] = "sccp_calling_pa_point_code_number";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number] = "sccp_calling_pa_subsystem_number";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan] = "sccp_calling_pa_gt_numbering_plan";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_gt_encoding_scheme] = "sccp_calling_pa_gt_encoding_scheme";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address] = "sccp_calling_pa_gt_nature_of_address";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_gt_address] = "sccp_calling_pa_gt_address";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type] = "sccp_calling_pa_gt_translation_type";
    (*debug_labels)[asn1::ParameterType::_pt_sccp_message_type] = "sccp_message_type";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_info_string] = "m3ua_info_string";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_routing_context] = "m3ua_routing_context";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_diagnostic_info] = "m3ua_diagnostic_info";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_heartbeat] = "m3ua_heartbeat";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_traffic_mode_type] = "m3ua_traffic_mode_type";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_error_code] = "m3ua_error_code";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_status] = "m3ua_status";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_asp_identifier] = "m3ua_asp_identifier";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_affected_point_code] = "m3ua_affected_point_code";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_correlation_id] = "m3ua_correlation_id";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_network_appearance] = "m3ua_network_appearance";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_user_cause] = "m3ua_user_cause";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_congestion_indications] = "m3ua_congestion_indications";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_concerned_destination] = "m3ua_concerned_destination";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_routing_key] = "m3ua_routing_key";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_registration_result] = "m3ua_registration_result";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_deregistration_result] = "m3ua_deregistration_result";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_local_routing_key_identifier] = "m3ua_local_routing_key_identifier";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_destination_point_code] = "m3ua_destination_point_code";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_service_indicators] = "m3ua_service_indicators";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_origination_point_code_list] = "m3ua_origination_point_code_list";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_circuit_range] = "m3ua_circuit_range";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_protocol_data] = "m3ua_protocol_data";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator] = "m3ua_protocol_data_service_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator] = "m3ua_protocol_data_network_indicator";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_protocol_data_message_priority] = "m3ua_protocol_data_message_priority";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code] = "m3ua_protocol_data_destination_point_code";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code] = "m3ua_protocol_data_originating_point_code";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code] = "m3ua_protocol_data_signalling_link_selection_code";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_registration_status] = "m3ua_registration_status";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_deregistration_status] = "m3ua_deregistration_status";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_header_data] = "m3ua_header_data";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_as_label] = "m3ua_as_label";
    (*debug_labels)[asn1::ParameterType::_pt_m3ua_asp_label] = "m3ua_asp_label";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_system_id] = "smpp_system_id";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_password] = "smpp_password";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_service_type] = "smpp_service_type";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_originator_ton] = "smpp_originator_ton";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_originator_np] = "smpp_originator_np";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_originator_address] = "smpp_originator_address";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_recipient_ton] = "smpp_recipient_ton";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_recipient_np] = "smpp_recipient_np";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_recipient_address] = "smpp_recipient_address";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_esm_message_mode] = "smpp_esm_message_mode";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_esm_message_type] = "smpp_esm_message_type";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_esm_gsm_features] = "smpp_esm_gsm_features";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_protocol_id] = "smpp_protocol_id";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_priority_flag] = "smpp_priority_flag";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_delivery_time] = "smpp_delivery_time";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_validity_period] = "smpp_validity_period";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_rd_smsc_receipt] = "smpp_rd_smsc_receipt";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_rd_sme_ack] = "smpp_rd_sme_ack";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_rd_intermediate_notification] = "smpp_rd_intermediate_notification";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_replace_if_present_flag] = "smpp_replace_if_present_flag";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_data_coding] = "smpp_data_coding";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_sm_defaut_msg_id] = "smpp_sm_defaut_msg_id";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_sm_length] = "smpp_sm_length";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_sm] = "smpp_sm";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_command_id] = "smpp_command_id";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_sar_msg_ref_num] = "smpp_sar_msg_ref_num";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_sar_total_segments] = "smpp_sar_total_segments";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_sar_segment_seqnum] = "smpp_sar_segment_seqnum";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_header_data] = "smpp_header_data";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_peer_ip] = "smpp_peer_ip";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_peer_port] = "smpp_peer_port";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_sequence] = "smpp_sequence";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_as_label] = "smpp_as_label";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_asp_label] = "smpp_asp_label";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_dlvr_rcpt_id] = "smpp_dlvr_rcpt_id";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_dlvr_rcpt_sub] = "smpp_dlvr_rcpt_sub";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_dlvr_rcpt_dlvrd] = "smpp_dlvr_rcpt_dlvrd";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_dlvr_rcpt_submit_date] = "smpp_dlvr_rcpt_submit_date";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_dlvr_rcpt_done_date] = "smpp_dlvr_rcpt_done_date";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_dlvr_rcpt_stat] = "smpp_dlvr_rcpt_stat";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_dlvr_rcpt_err] = "smpp_dlvr_rcpt_err";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_dlvr_rcpt_text] = "smpp_dlvr_rcpt_text";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_message_id] = "smpp_message_id";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_command_status] = "smpp_command_status";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_default_customer_encoding] = "smpp_default_customer_encoding";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_default_vendor_encoding] = "smpp_default_vendor_encoding";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_connection_id] = "smpp_connection_id";
    (*debug_labels)[asn1::ParameterType::_pt_smpp_dlvr_rcpt_format] = "smpp_dlvr_rcpt_format";
    return debug_labels;
}


// RefCounted
// init static
pmink_utils::RefCounted::_init pmink_utils::RefCounted::_initializer;

pmink_utils::RefCounted::RefCounted(){
    ref.set(1);
    _initializer.mtx.lock();
    _initializer.ref_list.insert(_initializer.ref_list.begin(), this);
    _initializer.mtx.unlock();

}

pmink_utils::RefCounted::~RefCounted(){}

void pmink_utils::RefCounted::init(){
    _initializer.thm.new_thread(&_initializer.dealloc_loop, "refc_thread", &_initializer);

}

size_t pmink_utils::RefCounted::get_item_count(){
    _initializer.mtx.lock();
    size_t tmp = _initializer.ref_list.size();
    _initializer.mtx.unlock();
    return tmp;
}



int pmink_utils::RefCounted::refc_inc(uint32_t incby){
    return ref.add_fetch(incby);

}

int pmink_utils::RefCounted::refc_dec(){
    return ref.sub_fetch(1);

}

int pmink_utils::RefCounted::refc_get(){
    return ref.get();

}

bool pmink_utils::RefCounted::refc_condition(){
    return true;
}


void pmink_utils::RefCounted::set_interval(uint32_t _interval){
    _initializer.interval.set(_interval);
}

pmink_utils::RefCounted::_init::_init(){}

void* pmink_utils::RefCounted::_init::dealloc_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    _init* init = (_init*)ti->user_args;

    while(ti->is_active()) {
	init->mtx.lock();
	std::vector<RefCounted*>::iterator it = init->ref_list.begin();
	while(it != init->ref_list.end()){
	    RefCounted* rfc = *it;
	    if(rfc->refc_get() <= 0 && rfc->refc_condition()){
		it = init->ref_list.erase(it);
		delete rfc;

	    }else ++it;

	}
	/*
	for(unsigned int i = 0; i<init->ref_list.size(); ){
	    RefCounted* rfc = init->ref_list[i];
	    if(rfc->refc_get() <= 0 && rfc->refc_condition()){
		init->ref_list.erase(init->ref_list.begin() + i);
		delete rfc;

	    }else ++i;
	}
	*/
	init->mtx.unlock();
	sleep(init->interval.get());
    }
    return NULL;
}

// Spinlock
pmink_utils::Spinlock::Spinlock(){
    pthread_spin_init(&slock, 0);
}

pmink_utils::Spinlock::Spinlock(const Spinlock& o){
    pthread_spin_init(&slock, 0);

}

pmink_utils::Spinlock::~Spinlock(){
    pthread_spin_destroy(&slock);

}

pmink_utils::Spinlock& pmink_utils::Spinlock::operator=(const Spinlock& other){ return *this; }

void pmink_utils::Spinlock::lock(){
    pthread_spin_lock(&slock);

}

void pmink_utils::Spinlock::unlock(){
    pthread_spin_unlock(&slock);

}

// Mutex
pmink_utils::Mutex::Mutex(){
    pthread_mutex_init(&mtx, NULL);

}

pmink_utils::Mutex::Mutex(const Mutex& o){
    pthread_mutex_init(&mtx, NULL);

}


pmink_utils::Mutex::~Mutex(){
    pthread_mutex_destroy(&mtx);

}

pmink_utils::Mutex& pmink_utils::Mutex::operator=(const Mutex& other){ return *this; }

void pmink_utils::Mutex::lock(){
    pthread_mutex_lock(&mtx);

}

void pmink_utils::Mutex::unlock(){
    pthread_mutex_unlock(&mtx);

}

// PcapDump
pmink_utils::PcapDump::PcapDump():	buffer(NULL),
					buffer_size(0),
					magic_num(0xa1b2c3d4),
					major_ver(2),
					minor_ver(4),
					tzone_offset(0),
					tstamp_accr(0),
					snapshot_l(0xffff),
					ll_hdr_type(1),
					packet_count(0){
}

pmink_utils::PcapDump::~PcapDump(){

}

pmink_utils::PcapDump::PcapDump(char* _buffer): buffer(_buffer),
						buffer_size(0),
						magic_num(0xa1b2c3d4),
						major_ver(2),
						minor_ver(4),
						tzone_offset(0),
						tstamp_accr(0),
						snapshot_l(0xffff),
						ll_hdr_type(1),
						packet_count(0){


    genereate_header();



}



int pmink_utils::PcapDump::add_packet(	const char* _data,
					uint32_t _dsize,
					uint32_t _dize_untrunc,
					uint32_t _tstamp,
					uint32_t _microsec){
    if(buffer == NULL) return -1;
    // timestamp, seconds
    memcpy(buffer, &_tstamp, 4);
    buffer += 4;
    buffer_size += 4;

    // timestamp, microseconds
    memcpy(buffer, &_microsec, 4);
    buffer += 4;
    buffer_size += 4;

    // captured length
    memcpy(buffer, &_dsize, 4);
    buffer += 4;
    buffer_size += 4;

    // un-truncated length
    memcpy(buffer, &_dize_untrunc, 4);
    buffer += 4;
    buffer_size += 4;

    // packet data
    memcpy(buffer, _data, _dsize);
    buffer += _dsize;
    buffer_size += _dsize;

    // packet count
    ++packet_count;

    // return ok
    return buffer_size;
}


char* pmink_utils::PcapDump::finish(uint32_t* _size){
    if(buffer == NULL){
	*_size = 0;
	return NULL;
    }
    *_size = buffer_size;
    buffer -= buffer_size;
    buffer_size = 0;
    packet_count = 0;
    return buffer;

}

int pmink_utils::PcapDump::genereate_header(){
    // err check
    if(buffer == NULL) return 1;

    // magic num
    memcpy(buffer, &magic_num, 4);
    buffer += 4;
    buffer_size += 4;

    // major version
    memcpy(buffer, &major_ver, 2);
    buffer += 2;
    buffer_size += 2;

    // minor version
    memcpy(buffer, &minor_ver, 2);
    buffer += 2;
    buffer_size += 2;

    // time zone offset
    memcpy(buffer, &tzone_offset, 4);
    buffer += 4;
    buffer_size += 4;

    // time stamp accuracy
    memcpy(buffer, &tstamp_accr, 4);
    buffer += 4;
    buffer_size += 4;

    // snapshot length
    memcpy(buffer, &snapshot_l, 4);
    buffer += 4;
    buffer_size += 4;

    // link layer header type
    memcpy(buffer, &ll_hdr_type, 4);
    buffer += 4;
    buffer_size += 4;

    // ok
    return 0;

}

void pmink_utils::PcapDump::set_buffer(char* _buffer){
    buffer = _buffer;
    buffer_size = 0;
    packet_count = 0;
}

char* pmink_utils::PcapDump::get_buffer(){
    return buffer;
}

uint32_t pmink_utils::PcapDump::get_buffer_size(){
    return buffer_size;
}

uint64_t pmink_utils::PcapDump::get_packet_count(){
    return packet_count;
}


void pmink_utils::PcapDump::set_ll_hdr_type(uint32_t type){
    ll_hdr_type = type;
}


// ThreadInfo
pmink_utils::ThreadInfo::ThreadInfo(): 	handle(0),
					user_args(NULL),
					userstart_routine(NULL),
					thm(NULL),
					cpu_core(65535){

    bzero(name, sizeof(name));
}

bool pmink_utils::ThreadInfo::is_active(){
    return active.get() && thm->is_active();
}

// ThreadManager
pmink_utils::ThreadManager::ThreadManager(): core_count(0){
    active.comp_swap(false, true);
    // get number of cores
    core_count = sysconf(_SC_NPROCESSORS_CONF);

}

pmink_utils::ThreadManager::~ThreadManager(){
    stop();
}



pmink_utils::ThreadInfo* pmink_utils::ThreadManager::new_thread(void *(*__start_routine) (void *),
								const char* __name,
								void* args,
								pthread_attr_t* __attr,
								uint16_t cpu_core){
    // create new thread info
    ThreadInfo* ti = new ThreadInfo();
    ti->thm = this;
    ti->user_args = args;
    ti->userstart_routine = __start_routine;
    if(__name != NULL){
	if(strlen(__name) <= 15) memcpy(ti->name, __name, strlen(__name) + 1);
    }
    // create thread
    if(pthread_create(&ti->handle, __attr, &start_wrapper, ti) == 0){
	// set name
	if(strlen(ti->name) > 0) pthread_setname_np(ti->handle, ti->name);
	// set cpu affinity
	if(cpu_core != 65535){
	    // check if core id is valid
	    if(cpu_core < core_count){
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(cpu_core, &cpuset);
		if(pthread_setaffinity_np(ti->handle, sizeof(cpu_set_t), &cpuset) == 0) ti->cpu_core = cpu_core;

	    }
	}
	// add to list
	threads.push_back(ti);
	// return thread info
	return ti;

    }else{
	delete ti;
	return NULL;
    }
}

bool pmink_utils::ThreadManager::is_active(){
    return active.get();
}


pmink_utils::ThreadInfo* pmink_utils::ThreadManager::get_thread(unsigned int index){
    if(index < threads.size()) return threads[index];
    return NULL;
}

int pmink_utils::ThreadManager::stop_thread(unsigned int index){
    if(index < threads.size()){
	// set as stopped
	threads[index]->active.comp_swap(true, false);
	return 0;
    }
    return 1;
}


uint16_t pmink_utils::ThreadManager::get_core_count(){
    return core_count;
}



uint16_t pmink_utils::ThreadManager::get_thread_count(){
    return th_count.get();
}

void pmink_utils::ThreadManager::start(){
    // set flag
    active.comp_swap(false, true);

}

void pmink_utils::ThreadManager::stop(bool _set_only){
    // set flag
    active.comp_swap(true, false);
    if(!_set_only){
	// wait for threads to finish
	timespec st = {0, 100000000};
	while(th_count.get() > 0){
	    nanosleep(&st, NULL);
	}
	// clear thread list
	for(unsigned int i = 0; i<threads.size(); i++)  delete threads[i];
	threads.clear();

    }

}

uint16_t pmink_utils::ThreadManager::inc(){
    return th_count.fetch_add(1);
}

uint16_t pmink_utils::ThreadManager::dec(){
    return th_count.fetch_sub(1);
}




void* pmink_utils::ThreadManager::start_wrapper(void* args){
    ThreadInfo* ti = (ThreadInfo*)args;
    if(ti != NULL){
	// set as active
	ti->active.comp_swap(false, true);
	// increment active thread count
	ti->thm->inc();
	// run user start routine
	ti->userstart_routine(ti);
	// set as inactive
	ti->active.comp_swap(true, false);

	// detach if not PTHREAD_CREATE_DETACHED
	pthread_attr_t attr;
	int state = 0;
	pthread_getattr_np(pthread_self(), &attr);
	pthread_attr_getdetachstate(&attr, &state);
	if(state != PTHREAD_CREATE_DETACHED) pthread_detach(pthread_self());
	pthread_attr_destroy(&attr);

	// decrement active thread count
	ti->thm->dec();
    }
    return NULL;
}



// rollback filter
int  pmink_utils::_ac_rollback_revision_filter(const struct dirent* a){
    if(strncmp(a->d_name, ".rollback", 9) == 0) return 1; else return 0;
}

// timestamp sort
int pmink_utils::_ac_rollback_revision_sort(const struct dirent ** a, const struct dirent ** b){
    // file stats
    struct stat st1;
    struct stat st2;
    char tmp_ch[200];
    bzero(&st1, sizeof(struct stat));
    bzero(&st2, sizeof(struct stat));

    bzero(tmp_ch, 200);
    memcpy(tmp_ch, "./commit-log/", 13);
    memcpy(&tmp_ch[13], (*a)->d_name, strlen((*a)->d_name));
    stat(tmp_ch, &st1);

    bzero(tmp_ch, 200);
    memcpy(tmp_ch, "./commit-log/", 13);
    memcpy(&tmp_ch[13], (*b)->d_name, strlen((*b)->d_name));
    stat(tmp_ch, &st2);

    if(st1.st_mtim.tv_sec > st2.st_mtim.tv_sec) return -1;
    else if(st1.st_mtim.tv_sec < st2.st_mtim.tv_sec) return 1;
    else return 0;
}


void pmink_utils::hex(void* data, int data_length, int pre_padd, std::ostringstream* out, char delim){
    unsigned char* cdata = (unsigned char*)data;
    // pre padd
    for(int k = 0; k<pre_padd; k++){
	*out << std::setfill('0') << std::setw(2) << std::hex << 0;
	if(delim != 0) *out << delim;
    }

    // data
    for(int k = 0; k<data_length - 1; k++){
	*out << std::setfill('0') << std::setw(2) << std::hex << (int)(cdata[k] & 0xff);
	if(delim != 0) *out << delim;
    }
    // last
    *out << std::setfill('0') << std::setw(2) << std::hex << (int)(cdata[data_length - 1] & 0xff);

}

void pmink_utils::print_ip(unsigned char* data, int data_length){
    if(data_length != 4) return;
    for(int k = 0; k<3; k++) std::cout << (int)(data[k] & 0xff) << ".";
    std::cout << (int)(data[3] & 0xff);

}

void pmink_utils::print_hex(void* data, int data_length){
	unsigned char* cdata = (unsigned char*)data;
	for(int k = 0; k<data_length; k++){
		std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)(cdata[k] & 0xff)<< " ";
	}
	std::cout << std::dec << std::endl;

}

int pmink_utils::bits_required(unsigned int input){
	return (int)ceil(log10(input + 1) / log10(2));
}

int pmink_utils::bytes_required(unsigned int input){
	return ceil((double)bits_required(input) / 8);
}

int pmink_utils::num_of_digits(unsigned int input){
	return input > 0 ? (int) log10 ((double) input) + 1 : 1;
}


int pmink_utils::oid_encode_node(uint32_t val, unsigned char** output){
    // single byte
    if(val < 128){
	*(*output)++ = val;
	// one byte needed
	return 1;

    }else{
	// required bits
	int rb = bits_required(val);
	// required bytes (7 bits since last bit is not used by the value)
	int rB = ceil((double)rb / 7);
	// shift bits
	int sb = rB - 1;
	// parts
	for(unsigned int i = 0; i< rB - 1; i++) *(*output)++ = 0x80 | (val >> (((rB - 1 - i) * 8) - sb--));
	// last part
	*(*output)++ = val & 0x7f;
	// return number of bytes needed
	return rB;
    }
}

int pmink_utils::oid_decode_node(const unsigned char** oid_data, char* output, bool is_first, bool absolute){
    bool more = (**oid_data & 0x80) == 0x80;
    int c = 0;
    uint32_t n = 0;
    int p = 0;
    uint bc = 1;

    // single octet
    if(!more){
	n = **oid_data;
	(*oid_data)++;

    // multiple octets
    }else{
	// max bits
	p = (sizeof(uint32_t) - 1) * 8;

	// first
	n = ((*(*oid_data)++ & 0x7f) << p);
	p -= 8;

	// sequential
	do{
	    n |= ((**oid_data & 0x7f) << (p + bc));
	    more = (*(*oid_data)++ & 0x80) == 0x80;
	    ++bc;
	    p -= 8;

	}while(more);
	// shift to right if needed
	n >>= (sizeof(uint32_t) * 8 - bc * 8 + (bc - 1));
    }

    // check if first
    if(is_first && absolute){
	c = sprintf(output, "%d.", n / 40);
	output += c;
	c += sprintf(output, "%d.", n % 40);

    }else{
	// output
	c = sprintf(output, "%d.", n);

    }
    // return number of bytes written to output
    return c;
}

void pmink_utils::oid_decode(const unsigned char* oid_data, unsigned int oid_data_length, char* output, bool absolute ){
    const unsigned char* oid_data_end = oid_data + oid_data_length;
    const unsigned char* oid_data_start = oid_data;

    // unpack nodes
    while(oid_data < oid_data_end) output += oid_decode_node(&oid_data, output, oid_data_start == oid_data, absolute);
    // remove last "."
    *(--output) = 0;
}


int pmink_utils::oid_encode(char* oid_data, unsigned char* output, bool absolute){
    char* tmp;
    char* token;
    uint32_t n = 0;
    int res = 0;

    // first or two if in absolute mode
    token = strtok_r(oid_data, ".", &tmp);
    if(absolute){
	n = atoi(token) * 40;
	token = strtok_r(NULL, ".", &tmp);
	n += atoi(token);
	res += oid_encode_node(n, &output);

    }else{
	n = atoi(token);
	res += oid_encode_node(n, &output);

    }

    // others
    token = strtok_r(NULL, ".", &tmp);
    while(token != NULL){
	n = atoi(token);
	res += oid_encode_node(n, &output);
	token = strtok_r(NULL, ".", &tmp);

    }

    return res;

}


uint32_t pmink_utils::hash_fnv(const void* key, int len){
   if(key == NULL) return 0;
   unsigned char *p = (unsigned char*)key;
   uint32_t h = 2166136261;
   int i;
   for(i = 0; i < len; i++) h = (h * 16777619) ^ p[i];
   return h;
}

uint32_t pmink_utils::hash_fnv1a(const void* key, int len){
   if(key == NULL) return 0;
   unsigned char *p = (unsigned char*)key;
   uint32_t h = 2166136261;
   int i;
   for(i = 0; i < len; i++) h = (h ^ p[i]) * 16777619;
   return h;
}

uint64_t pmink_utils::hash_fnv1a_64bit(const void* key, int len){
   if(key == NULL) return 0;
   unsigned char *p = (unsigned char*)key;
   uint64_t h = 14695981039346656037UL;
   int i;
   for(i = 0; i < len; i++) h = (h ^ p[i]) * 1099511628211UL;
   return h;
}


uint32_t pmink_utils::hash_fnv1a_str(const char* key){
    if(key == NULL) return 0;
    unsigned char *p = (unsigned char*)key;
    uint32_t h = 2166136261;
    uint32_t len = strlen(key);
    for(unsigned int i = 0; i < len; i++) h = (h ^ p[i]) * 16777619;
    return h;
}

uint64_t pmink_utils::hash_fnv1a_str_64bit(const char* key){
    if(key == NULL) return 0;
    unsigned char *p = (unsigned char*)key;
    uint64_t h = 14695981039346656037UL;
    uint32_t len = strlen(key);
    for(unsigned int i = 0; i < len; i++) h = (h ^ p[i]) * 1099511628211UL;
    return h;
}


uint64_t pmink_utils::mem_get_free(){
	struct sysinfo sys_info;
	sysinfo(&sys_info);
	return sys_info.freeram;
}


int pmink_utils::run_external(const char* script, char* result, int result_size){
    FILE* pipe = popen(script, "r");
    if (!pipe) return -1;
    char tmp_buff[128];
    std::string tmp_res;
    // read from pipe
    while(!feof(pipe)){
    	if(fgets(tmp_buff, 128, pipe) != NULL) tmp_res += tmp_buff;
    }
    pclose(pipe);

    // check buffer size
    if(result_size >= (tmp_res.size() + 1)){
        memcpy(result, tmp_res.c_str(), tmp_res.size());
        result[tmp_res.size()] = 0;
        return 0;

    }

    return 1;
}

void* pmink_utils::load_plugin(const char* module){
    return dlopen(module, RTLD_LAZY);

}

void pmink_utils::unload_plugin(void* handle){
    dlclose(handle);

}


void* pmink_utils::run_external_method(void* handle, const char* method, void** args, int argc, bool ncurses){
    if(handle != NULL){
	typedef void* (*exec_block)(void**, int);
	dlerror();
	exec_block exec_b = (exec_block)dlsym(handle, method);
	const char *dlsym_error = dlerror();
	if(dlsym_error){
	    if(ncurses) printw("Cannot load symbol '%s': %s\n", method, dlsym_error);
	    else std::cout << "Cannot load symbol '" << method << "': " << dlsym_error << std::endl;
	    dlclose(handle);
	}else{
	    return exec_b(args, argc);
	    //if(ncurses) printw("\n"); else std::cout << std::endl;
	}

    }else{
	if(ncurses) printw("Cannot open library: %s\n", dlerror()); else
	std::cout << "Cannot open library: " << dlerror() << std::endl;
    }
    return NULL;

}


void* pmink_utils::run_external_method(const char* module, const char* method, void** args, int argc, bool ncurses){
    void* handle = dlopen(module, RTLD_LAZY);
    if(handle != NULL){
	typedef void* (*exec_block)(void**, int);
	dlerror();
	exec_block exec_b = (exec_block)dlsym(handle, method);
	const char *dlsym_error = dlerror();
	if(dlsym_error){
	    if(ncurses) printw("Cannot load symbol '%s': %s\n", method, dlsym_error);
	    else std::cout << "Cannot load symbol '" << method << "': " << dlsym_error << std::endl;
	    dlclose(handle);
	}else{
	    void* res = exec_b(args, argc);
	    dlclose(handle);
	    return res;
	}

    }else{
	if(ncurses) printw("Cannot open library: %s\n", dlerror()); else
	std::cout << "Cannot open library: " << dlerror() << std::endl;
    }
    return NULL;

}


void* pmink_utils::run_external_method_handler(const char* module, const char** arg_names, const char** arg_values, int arg_count, bool ncurses){
    void* handle = dlopen(module, RTLD_LAZY);
    if(handle != NULL){
	typedef void* (*exec_method)(const char**, const char**, int);
	dlerror();
	exec_method exec_m = (exec_method)dlsym(handle, "method_handler");
	const char *dlsym_error = dlerror();
	if(dlsym_error){
	    if(ncurses) printw("Cannot load symbol 'method_handler': %s\n", dlsym_error);
	    else std::cout << "Cannot load symbol 'method_handler': " << dlsym_error << std::endl;
	    dlclose(handle);
	}else{
	    void* res = exec_m(arg_names, arg_values, arg_count);
	    if(ncurses) printw("\n"); else std::cout << std::endl;
	    dlclose(handle);
	    return res;
	}

    }else{
	if(ncurses) printw("Cannot open library: %s\n", dlerror()); else
	std::cout << "Cannot open library: " << dlerror() << std::endl;
    }
    return NULL;

}


void pmink_utils::run_external_print(const char* script, bool ncurses){
    FILE* pipe = popen(script, "r");
    if (!pipe) {
	return;
    }
    char tmp_buff[128];
    // read from pipe
    while(!feof(pipe)){
    	if(fgets(tmp_buff, 128, pipe) != NULL){
    	    if(ncurses) printw("%s", tmp_buff); else std::cout << tmp_buff;
    	}
    }
    if(ncurses) printw("\n"); else std::cout << std::endl;
    pclose(pipe);

}


int pmink_utils::cli_more(int line_c, WINDOW* data_win, bool* interrupt){
    int w, h, y, x, usbl_lc;
    bool more = false;
    getmaxyx(stdscr, h, w);
    getyx(stdscr, y, x);

    // usable line count
    if(line_c > h - 1){
	more = true;
	usbl_lc = h - 1;
    }else usbl_lc = line_c;

    // scrolling needed
    if(y + usbl_lc + 1 > h) {
	scrl(usbl_lc + y - h + 1);
	y = h - usbl_lc - 1;
	wmove(stdscr, h - 1, x);
    // no scrolling needed
    }else{
	wmove(stdscr, y + usbl_lc, x);

    }
    // copy win data
    copywin(data_win, stdscr, 0, 0, y, 0, y + usbl_lc - 1, w - 1, false);
    // more mode
    if(more){
	    int key_p;
	    int more_c = 0;
	    int dmaxrow = usbl_lc;
	    int line_diff = 0;
	    // loop
	    while(more && !(*interrupt)){
		// position indicator
		attron(COLOR_PAIR(8));
		printw("lines %d-%d/%d", usbl_lc * more_c + 1 + line_diff, usbl_lc * more_c + usbl_lc + line_diff, line_c);
		attroff(COLOR_PAIR(8));
		refresh();
		// wait for key press
		key_p = getch();
 		// check for interrupt
		if(*interrupt){
		    int y, x;
		    getyx(stdscr, y, x);
		    move(y, 0);
		    clrtoeol();
		    refresh();
		    return key_p;
		}

		if(key_p == KEY_PPAGE){
		    if(more_c > 0) {
			--more_c;
		    }else line_diff = 0;

		}else if(key_p == KEY_NPAGE || key_p == ' '){
		    ++more_c;

		}else if(key_p == KEY_UP){
		    if(line_diff > 0) {
			--line_diff;
		    }

		}else if(key_p == KEY_DOWN){
		    if(line_diff < line_c - 1) ++line_diff;
		}


		// next chunk
		//++more_c;
		// last chunk
		if((usbl_lc * more_c + usbl_lc) + line_diff >= line_c ){
		    dmaxrow = (line_c - (usbl_lc * more_c + line_diff)) - 1;
		    more = false;
		// full size chunk
		}else{
		    dmaxrow = usbl_lc - 1;
		}
		// clear screen
		clear();
		// copy win
		int cres = copywin(	data_win,
					stdscr,
					// source
					usbl_lc * more_c + line_diff,
					0,
					// dest
					0,
					0,
					dmaxrow,
					w - 1,
					false);

		// update cursor position
		wmove(stdscr, dmaxrow + 1, 0);
		// refresh
		refresh();
	    }

    }
    return -1;
}

void pmink_utils::tokenize(std::string* data, std::string* result, int result_max_size, int* result_size, bool keep_quotes){
    if(data != NULL && result != NULL && result_size != NULL){
	    *result_size = 0;
	    try{
		    boost::tokenizer<boost::escaped_list_separator<char> > tok(*data, boost::escaped_list_separator<char>('\\', ' ', '\"'));
		    for(boost::tokenizer<boost::escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg){
			// skip empty tokens
			if(*beg != ""){
				result[(*result_size)++] = *beg;
				// keep quotes or not
				if(keep_quotes){
				    if(result[*result_size - 1].find(' ') < result[*result_size - 1].size()){
					result[*result_size - 1].insert(0, "\"");
					result[*result_size - 1].append("\"");
				    }
				}
				// buffer overflow check
				if(*result_size >= result_max_size) return;
			}
		    }

	    }catch(std::exception& e){
		*result_size = 0;
		// ignore
	    }

    }

}


int pmink_utils::save_file(const char* filename, char* data, int data_size, bool append){
    //std::cout << "APPEND: " << append << std::endl;
    std::ofstream ofs(filename, std::ios::out | (append ? std::ios::app : std::ios::trunc) | std::ios::binary);
    if(ofs.is_open()){
	ofs.write(data, data_size);
	ofs.close();
	return 0;
    }

    return 1;

}

int pmink_utils::get_file_size(const char* filename){
    std::ifstream ifs(filename, std::ios::binary | std::ios::in | std::ios::ate);
    if(ifs.is_open()){
	int fsize = ifs.tellg();
	ifs.close();
	return fsize;
    }
    return 0;
}


int pmink_utils::load_file(const char* filename, char* result, int* result_size){
    std::ifstream ifs(filename, std::ios::binary | std::ios::in | std::ios::ate);
    *result_size = 0;
    if(ifs.is_open()){
	    *result_size = ifs.tellg();
	    ifs.seekg(0, std::ios::beg);
	    ifs.read(result, (long)(*result_size));
	    ifs.close();
	    return 0;
    }
    return 1;
}

int pmink_utils::copy_file(const char* src, const char* dst){
    std::ifstream src_s(src, std::ios::binary);
    std::ofstream dst_s(dst, std::ios::binary);
    dst_s << src_s.rdbuf();
    dst_s.flush();
    dst_s.close();
    return 0;
}


timespec pmink_utils::time_diff(timespec start, timespec end) {
	timespec temp;
	if((end.tv_nsec - start.tv_nsec) < 0){
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	}else{
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}


// TCAPOperation
pmink_utils::TCAPOperation::TCAPOperation(): tcap_trans(NULL),
					     invoke_id(0),
					     finished(false),
					     processed(false),
					     //request_data(NULL),
					     req_component_index(0),
					     //reply_data(NULL),
					     rpl_component_index(0){

}



// TCAPTransaction
pmink_utils::TCAPTransaction::TCAPTransaction(): timestamp(0),
						 status(TCAP_ST_ACTIVE){
    //pthread_spin_init(&slock, 0);

}

pmink_utils::TCAPTransaction::~TCAPTransaction(){
    //pthread_spin_destroy(&slock);

}

/*
void pmink_utils::TCAPTransaction::lock(){
    pthread_spin_lock(&slock);
}

void pmink_utils::TCAPTransaction::unlock(){
    pthread_spin_unlock(&slock);
}
*/


// TCAPCorrelator
pmink_utils::TCAPCorrelator::TCAPCorrelator(unsigned int _max_trans): max_trans(_max_trans){
    pthread_spin_init(&slock, 0);
}

pmink_utils::TCAPCorrelator::~TCAPCorrelator(){
    pthread_spin_destroy(&slock);
}


void pmink_utils::TCAPCorrelator::lock(){
    pthread_spin_lock(&slock);
}

void pmink_utils::TCAPCorrelator::unlock(){
    pthread_spin_unlock(&slock);
}


void pmink_utils::TCAPCorrelator::process_timeout(unsigned int timeout, std::vector<tcap_tmap_it_t>* result, bool append){
    time_t now = time(NULL);
    if(!append) result->clear();

    // loop
    for(tcap_tmap_it_t it = transaction_tmap.begin(); it != transaction_tmap.end(); it++){
	// check if idle
	if(it->second.status != TCAP_ST_EXPIRED && now - it->second.timestamp > timeout){
	    // add to result
	    result->push_back(it);
	    // set as finished (call cleanup)
	    it->second.status = TCAP_ST_TIMEOUT;

	}
    }


}

int pmink_utils::TCAPCorrelator::remove_transaction(tcap_tmap_it_t it){
    // remove from transaction map
    transactions.erase(it->second.transaction_it);
    // remove from tmap
    transaction_tmap.erase(it);
    return 0;
}


int pmink_utils::TCAPCorrelator::remove_transaction(TCAPTransactionKey& key){
    // find
    tcap_map_it_t it = transactions.find(key);
    if(it == transactions.end()) return -3;
    // remove from tmap
    transaction_tmap.erase(it->second);
    // remove from map
    transactions.erase(it);
    return 0;
}

pmink_utils::TCAPTransaction* pmink_utils::TCAPCorrelator::update_key(tcap_map_it_t& it, TCAPTransactionKey* new_key){
    if(it->first.key_1 == 0){
	// get key
	TCAPTransactionKey tmp_key = it->first;
	// update non zero key
	if(new_key->key_1 > 0 && new_key->key_1 != tmp_key.key_2){
	    tmp_key.key_1 = new_key->key_1;
	}else if(new_key->key_2 > 0 && new_key->key_2 != tmp_key.key_2) tmp_key.key_1 = new_key->key_2;
	// get old value
	TCAPTransaction tcap_tmp = it->second->second;
	// remove from tmap
	transaction_tmap.erase(it->second);
	// remove from map
	transactions.erase(it);
	// insert in tmap
	uint64_t index = 0;
	tcap_tmap_rev_it_t tmap_rev = transaction_tmap.rbegin();
	// check if transaction tmap is empty
	if(tmap_rev != transaction_tmap.rend()) index = tmap_rev->first + 1;
	// insert
	tcap_tmap_insert_t tmap_in = transaction_tmap.insert(tcap_tmap_value_t(index, tcap_tmp));
	// insert in transaction map
	tcap_map_insert_t trans_in = transactions.insert(tcap_map_value_t(tmp_key, tmap_in.first));
	// update transaction map iterator in tmap
	tmap_in.first->second.transaction_it = trans_in.first;
	// pointer to transaction
	TCAPTransaction* tcap_p = &tmap_in.first->second;
	// set data
	for(unsigned int i = 0; i<tcap_p->operations.size(); i++) tcap_p->operations[i].tcap_trans = tcap_p;
	tcap_p->id = tmp_key;
	// return
	return tcap_p;


    }else if(it->first.key_2 == 0){
	// get key
	TCAPTransactionKey tmp_key = it->first;
	// update non zero key
	if(new_key->key_1 > 0 && new_key->key_1 != tmp_key.key_1){
	    tmp_key.key_2 = new_key->key_1;
	}else if(new_key->key_2 > 0 && new_key->key_2 != tmp_key.key_1) tmp_key.key_2 = new_key->key_2;
	// get old value
	TCAPTransaction tcap_tmp = it->second->second;
	// remove from tmap
	transaction_tmap.erase(it->second);
	// remove from map
	transactions.erase(it);
	// insert in tmap
	uint64_t index = 0;
	tcap_tmap_rev_it_t tmap_rev = transaction_tmap.rbegin();
	// check if transaction tmap is empty
	if(tmap_rev != transaction_tmap.rend()) index = tmap_rev->first + 1;
	// insert
	tcap_tmap_insert_t tmap_in = transaction_tmap.insert(tcap_tmap_value_t(index, tcap_tmp));
	// insert in transaction map
	tcap_map_insert_t trans_in = transactions.insert(tcap_map_value_t(tmp_key, tmap_in.first));
	// update transaction map iterator in tmap
	tmap_in.first->second.transaction_it = trans_in.first;
	// pointer to transaction
	TCAPTransaction* tcap_p = &tmap_in.first->second;
	// set data
	for(unsigned int i = 0; i<tcap_p->operations.size(); i++) tcap_p->operations[i].tcap_trans = tcap_p;
	tcap_p->id = tmp_key;
	// return
	return tcap_p;

    }
    // return
    return &it->second->second;

}

pmink_utils::TCAPTransaction* pmink_utils::TCAPCorrelator::validate_transaction(uint32_t sid, uint32_t did, uint32_t gt_called, uint32_t gt_calling, bool sid_present, bool did_present){
    TCAPTransactionKey tcap_key(sid, did, gt_calling);
    tcap_key.generate_keys(sid, did, gt_calling, sid_present, did_present);
    tcap_map_it_t it = transactions.find(tcap_key);

    if(it == transactions.end()){
	tcap_key.sid = did;
	tcap_key.did = sid;
	tcap_key.gt = gt_called;
	tcap_key.generate_keys(did, sid, gt_called, did_present, sid_present);
	it = transactions.find(tcap_key);
	if(it != transactions.end()){
	    // update key
	    return update_key(it, &tcap_key);
	}
    }else{
	// update key
	return update_key(it, &tcap_key);
    }

    return NULL;
}


int pmink_utils::TCAPCorrelator::expire(std::vector<tcap_tmap_it_t>* result, bool append){
    if(!append) result->clear();
    if(transaction_tmap.size() >= max_trans){
	// num of translations to remove
	int exp_count = transaction_tmap.size() -  max_trans + 1;
	int c = exp_count;
	// loop
	for(tcap_tmap_it_t it = transaction_tmap.begin(); it != transaction_tmap.end(); it++){
	    // add to result
	    result->push_back(it);
	    // set status
	    it->second.status = TCAP_ST_EXPIRED;
	    // dec count
	    --c;
	    // check if done
	    if(c <= 0) break;
	}
	// return num of removed
	return exp_count;
    }

    // ok
    return 0;
}

pmink_utils::TCAPOperation* pmink_utils::TCAPCorrelator::operation_exists(TCAPTransaction* tcap_trans, uint32_t invoke_id){
    for(unsigned int i = 0; i<tcap_trans->operations.size(); i++) {
	TCAPOperation& op = tcap_trans->operations[i];
	if(!op.finished && op.invoke_id == invoke_id) return &op;
    }
    return NULL;
}


int pmink_utils::TCAPCorrelator::abort_operations(TCAPTransaction* tcap_trans, pmink_utils::PooledVPMap<uint32_t>* params, bool set_reply){
    if(tcap_trans == NULL) return 1;

    // loop operations
    for(unsigned int i = 0; i<tcap_trans->operations.size(); i++){
	TCAPOperation& tcap_op = tcap_trans->operations[i];
	// skip finished
	if(tcap_op.finished) continue;
	// set data
	tcap_op.finished = true;
	//tcap_op.reply_data = data;
	if(set_reply){
	    tcap_op.reply_params = *params;
	    tcap_op.rpl_component_index = 0;

	}
    }

    return 0;
}


int pmink_utils::TCAPCorrelator::process_operations(TCAPTransaction* tcap_trans, pmink_utils::PooledVPMap<uint32_t>* params, void* data){
    // get tcap component count
    pmink_utils::VariantParam* vparam = params->get_param(asn1::ParameterType::_pt_tcap_component_count);
    if(vparam == NULL) return 1;
    unsigned int cc = (int)*vparam;
    unsigned int tmp;
    TCAPOperation* tcap_op_p;

    // loop components
    for(unsigned int j = 0; j<cc; j++){
	// new tcap op
	TCAPOperation tcap_op;
	// get invoke id
	vparam = params->get_param(asn1::ParameterType::_pt_tcap_component_invoke_id, j);
	// sanity check
	if(vparam == NULL) continue;
	// get invoke id value
	tmp = (int)*vparam;
	tcap_op.invoke_id = tmp;

	// correlate component
	// 1 = Invoke, correlated reply should not be Invoke component type
	// 2 = ReturnResultLast
	// 3 = ReturnError
	// 4 = Reject
	// 7 = ReturnResultNotLast

	// check for pending invokes and correlate
	if((tcap_op_p = operation_exists(tcap_trans, tmp)) != NULL){
	    // get component type
	    vparam = params->get_param(asn1::ParameterType::_pt_tcap_component_type, j);
	    if(vparam == NULL) continue;
	    tmp = (int)*vparam;
	    // check for valid component type
	    if(tmp == 1) continue;
	    // set data
	    tcap_op_p->finished = true;
	    tcap_op_p->reply_params = *params;
	    tcap_op_p->rpl_component_index = j;
	    //std::cout << "!!! TCAP Operations EXISTS, Invoke Id: " << tcap_op_p->invoke_id << std::endl;

	// add to list, correlation not found
	}else{
	    // get component type
	    vparam = params->get_param(asn1::ParameterType::_pt_tcap_component_type, j);
	    if(vparam == NULL) continue;
	    tmp = (int)*vparam;
	    // check for valid component type
	    if(tmp != 1) continue;
	    // set data
	    tcap_op.request_params = *params;
	    tcap_op.req_component_index = j;
	    tcap_op.tcap_trans = tcap_trans;
	    // add to list of operations
	    tcap_trans->operations.push_back(tcap_op);
	    //std::cout << "TCAP Operations ADD Invoke Id: " << tcap_op.invoke_id << std::endl;

	}

    }
    return 0;
}

void pmink_utils::TCAPCorrelator::set_max_trans(int _max_trans){
    max_trans = _max_trans;
}


uint32_t pmink_utils::TCAPCorrelator::get_size(){
    return transactions.size();
}


int pmink_utils::TCAPCorrelator::cleanup(TCAPTransaction* trans){
    if(trans == NULL) return -1;
    if(trans->status != TCAP_ST_ACTIVE){
	// find
	tcap_map_it_t it = transactions.find(trans->id);
	if(it == transactions.end()) return -3;

	// remove from tmap
	transaction_tmap.erase(it->second);

	// remove from map
	transactions.erase(it);

	return 0;
    }
    return -2;
}


pmink_utils::TCAPTransaction* pmink_utils::TCAPCorrelator::process(void* data, pmink_utils::PooledVPMap<uint32_t>* params){
    // sides (SID, GTCLG)
    // ==================
    // A = 1.10
    // B = 2.10

    // flow examples
    // 				SID	DID	GTCLG	GTCLD	ACTION
    //				======================================
    //	1. A->B begin		1	-	10	20	ADD side A (1.10)
    //	2. B<-A continue	2	1	20	10	CORRELATE on A, ADD side B (2.10), link A <-> B
    // 	3. A->B continue	1	2	10	20	CORRELATE on A + B
    // 	4. B->A continue	2	1	20	10	CORRELATE on A + B
    //	5. B->A end		-	1	20	10	CORRELATE on A/B, REMOVE A and B

    // 				SID	DID	GTCLG	GTCLD	ACTION
    //				======================================
    //	1. A->B begin		1	-	10	20	ADD side A (1.10)
    //	2. B->A end		-	1	20	10	CORRELATE on A/B, REMOVE A

    // 				SID	DID	GTCLG	GTCLD	ACTION
    //				======================================
    //	1. A->B begin		1	-	10	20	ADD side A (1.10)
    //	2. B<-A continue	2	1	20	10	CORRELATE on A, ADD side B (2.10), link A <-> B
    // 	3. A->B continue	1	2	10	20	CORRELATE on A + B
    // 	4. B->A continue	2	1	20	10	CORRELATE on A + B
    //	5. A->B end		-	2	10	20	CORRELATE on A/B, REMOVE A and B

    // 				SID	DID	GTCLG	GTCLD	ACTION
    //				======================================
    //	1. A->B begin		1	-	10	20	ADD side A (1.10)
    //	2. B<-A continue	2	1	20	10	CORRELATE on A, ADD side B (2.10), link A <-> B
    // 	3. A->B continue	1	2	10	20	CORRELATE on A + B
    //	5. A->B end		-	2	10	20	CORRELATE on A/B, REMOVE A and B



    // key = SID + GTCLG
    // =================



    // null check
    if(data == NULL || params == NULL) return NULL;

    pmink_utils::VariantParam* vparam;
    TCAPTransaction tcap_leg;
    TCAPTransaction* tcap_matched = NULL;
    TCAPTransactionKey tcap_key;
    bool sccp_err = false;
    uint32_t gt_called = 0;
    uint32_t gt_calling = 0;
    uint32_t sid = 0;
    bool sid_present = false;
    bool did_present = false;
    uint32_t did = 0;
    uint32_t tcap_mt = 0;



    // check for sccp udts correlation
    if((vparam = params->get_param(asn1::ParameterType::_pt_sccp_message_type)) != NULL){
	switch((int)*vparam){
	    // UDTS
	    case 0x0a:
	    // XUDTS
	    case 0x12:
		sccp_err = true;
		break;
	}
    }
    // get gt called
    if((vparam = params->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address)) != NULL)
	gt_called = pmink_utils::hash_fnv1a((char*)*vparam, vparam->get_size());

    // get gt calling
    if((vparam = params->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address)) != NULL)
	gt_calling = pmink_utils::hash_fnv1a((char*)*vparam, vparam->get_size());

    // sanity check
    if(gt_called == 0 || gt_calling == 0) return NULL;

    // get sid
    if((vparam = params->get_param(asn1::ParameterType::_pt_tcap_source_transaction_id)) != NULL) {
	sid = (int)*vparam;
	sid_present = true;
    }
    // get did
    if((vparam = params->get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id)) != NULL) {
	did = (int)*vparam;
	did_present = true;
    }
    // check for tcap message type
    if((vparam = params->get_param(asn1::ParameterType::_pt_tcap_message_type)) == NULL) return NULL;
    else tcap_mt = (int)*vparam;


    // check for sccp udts error
    if(sccp_err){
	// check sid + gt combinations
	tcap_matched = validate_transaction(sid, sid, gt_called, gt_calling, sid_present, did_present);
	// if matched
	if(tcap_matched != NULL){
	    //std::cout << "TCAP session ending due to SCCP error: " << std::hex << tcap_matched->id.sid << std::endl;
	    tcap_matched->timestamp = time(NULL);
	    // set transaction status
	    tcap_matched->status = TCAP_ST_SCCP_ERROR;
	    // abort active operations
	    abort_operations(tcap_matched, params);

	    //std::cout << "Removing transaction: " << tcap_map.erase(tcap_matched->id) << std::endl;
	    return tcap_matched;

	}else return NULL;

    }
    // map iterator
    //tcap_map_it it;
    tcap_map_it_t trans_it;
    tcap_tmap_it_t tmap_it;

    // ***** correlate *****
    // check TCAP message type
    switch(tcap_mt){
	// Begin
	case 2:
	    //std::cout << "---- TCAP BEGIN ---: " << std::hex << sid << " - " << did << " - " << gt_calling << " - " << gt_called << std::endl;
	    // set sid and gt
	    tcap_key.sid = sid;
	    tcap_key.gt = gt_calling;
	    tcap_key.generate_keys(sid, did, gt_calling, sid_present, did_present);
	    // find
	    trans_it = transactions.find(tcap_key);
	    // add if not found
	    if(trans_it == transactions.end()){
		tcap_leg.id = tcap_key;
		tcap_leg.timestamp = time(NULL);

		// insert in tmap
		uint64_t index = 0;
		tcap_tmap_rev_it_t tmap_rev = transaction_tmap.rbegin();

		// check if transaction tmap is empty
		if(tmap_rev != transaction_tmap.rend()) index = tmap_rev->first + 1;

		// insert
		tcap_tmap_insert_t tmap_in = transaction_tmap.insert(tcap_tmap_value_t(index, tcap_leg));

		// insert in transaction map
		tcap_map_insert_t trans_in = transactions.insert(tcap_map_value_t(tcap_key, tmap_in.first));

		// update transaction map iterator in tmap
		tmap_in.first->second.transaction_it = trans_in.first;

		// get pointer to transaction
		tcap_matched = &tmap_in.first->second;

		//std::cout << "Adding new TCAP session: " << std::hex << sid << std::endl;
		// process operations
		process_operations(tcap_matched, params, data);

	    }else{
		//std::cout << "!!!!DUPLICATE!!!!: " << std::hex << sid << ", " << did << std::endl;
		return NULL;
	    }

	    // process
	    /*
	    std::cout << "==== processing begin ====" << std::endl;
	    std::cout << std::dec << *params;
	    std::cout << std::endl;
	    */
	    break;

	// End
	case 4:
	    //std::cout << "---- TCAP END ---: " << std::hex << sid << " - " << did << " - " << gt_calling << " - " << gt_called << std::endl;
	    // only did is present, use did param for both sid in did in validate_transaction
	    if((tcap_matched = validate_transaction(sid, did, gt_called, gt_calling, sid_present, did_present)) != NULL){
		//std::cout << "TCAP session END matched: " << std::hex << tcap_matched->id.sid << std::endl;
		tcap_matched->timestamp = time(NULL);
		// process operations
		process_operations(tcap_matched, params, data);
		// finish any pending operations (no reply)
		abort_operations(tcap_matched, params, false);

	    }else{
		//std::cout << "END not matched: " << sid << ", " << did << std::endl;
		return NULL;
	    }

	    // process
	    /*
	    std::cout << "==== processing end ====" << std::endl;
	    std::cout << std::dec << *params;
	    std::cout << std::endl;
	    */
	    tcap_matched->status = TCAP_ST_FINISHED;
	    //std::cout << "Removing transaction: " << tcap_map.erase(tcap_matched->id) << std::endl;
	    break;

	// Continue
	case 5:
	    //std::cout << "---- TCAP CONTINUE ---: " << std::hex << sid << " - " << did << " - " << gt_calling << " - " << gt_called << std::endl;
	    if((tcap_matched = validate_transaction(sid, did, gt_called, gt_calling, sid_present, did_present)) != NULL){
		//std::cout << "TCAP session CONTINUE matched: " << std::hex << tcap_matched->id.sid << std::endl;
		tcap_matched->timestamp = time(NULL);
		// process operations
		process_operations(tcap_matched, params, data);

	    }else {
		//std::cout << "CONTINUE not matched: " << sid << ", " << did << std::endl;
		return NULL;
	    }


	    // process
	    //std::cout << "==== processing continue ====" << std::endl;
	    break;

	// Abort
	case 7:
	    //std::cout << "---- TCAP ABORT ---: " << std::hex << sid << " - " << did << " - " << gt_calling << " - " << gt_called << std::endl;
	    // only did is present, use did param for both sid in did in validate_transaction
	    if((tcap_matched = validate_transaction(sid, did, gt_called, gt_calling, sid_present, did_present)) != NULL){
		//std::cout << "TCAP session ABORT matched: " << std::hex << tcap_matched->id.sid << std::endl;
		tcap_matched->timestamp = time(NULL);
		// abort active operations
		abort_operations(tcap_matched, params);

	    }else {
		//std::cout << "ABORT not matched: " << sid << ", " << did << std::endl;
		return NULL;
	    }

	    // process
	    /*
	    std::cout << "==== processing abort ====" << std::endl;
	    std::cout << std::dec << *params;
	    std::cout << std::endl;
	    */
	    tcap_matched->status = TCAP_ST_ABORTED;
	    //std::cout << "Removing transaction: " << remove_transaction(tcap_matched->id) << std::endl;
	    break;


	// unkonwn/unsupported
	default:
	    return NULL;
    }

    //std::cout << "PENDING: " << tcap_map.size() << std::endl;
    return tcap_matched;


}


