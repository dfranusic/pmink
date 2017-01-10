--
-- This file is part of Project MINK <http://www.release14.org>.
--
-- Copyright (C) 2012 Release14 Ltd.
-- http://www.release14.org/
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.
--
--
--
-- @file pmink.lua
-- @brief pMINK Lua module
--

-- init module table
local pmink = {}
-- set ffi
local ffi = require("ffi")
local C = ffi.C
-- declare pmink C functions
ffi.cdef [[
        typedef struct {
            char** groups;
            int size;
            bool matched;
        } rgx_result_t;

	struct sccp_consts{
		static const int NP_UNKNOWN = 0x00;
		static const int NP_ISDN_TELEPHONE = 0x10;
		static const int NP_GENERIC = 0x20;
		static const int NP_DATA_X121 = 0x30;
		static const int NP_TELEX = 0x40;
		static const int NP_MARITIME = 0x50;
		static const int NP_LAND_MOBILE = 0x60;
		static const int NP_ISDN_MOBILE = 0x70;
		static const int NP_PRIVATE = 0xE0;
		static const int NOA_UNKNOWN = 0x00;
		static const int NOA_SUBSCRIBER_NUMBER = 0x01;
		static const int NOA_RESERVED_FOR_NATIONAL_USE = 0x02;
		static const int NOA_NATIONAL_SIGNIFICANT_NUMBER = 0x03;
		static const int NOA_INTERNATIONAL = 0x04;
		static const int SSN_UNKNOWN = 0x00;
		static const int SSN_SCCP_MANAGEMENT = 0x01;
		static const int SSN_ITU_T_RESERVED = 0x02;
		static const int SSN_ISDN_USER_PART = 0x03;
		static const int SSN_OMAP = 0x04;
		static const int SSN_MAP = 0x05;
		static const int SSN_HLR = 0x06;
		static const int SSN_VLR = 0x07;
		static const int SSN_MSC = 0x08;
		static const int SSN_EIC = 0x09;
		static const int SSN_AUC = 0x0A;
		static const int SSN_ISDN_SUPPLEMENTARY = 0x0B;
		static const int SSN_RESERVED_FOR_INTERNATIONAL_USE = 0x0C;
		static const int SSN_BROADBAND_ISDN_EDGE_TO_EDGE = 0x0D;
		static const int SSN_TC_TEST_RESPONDER = 0x0E;
	};

	struct pm_consts{
		static const int _VP_CURRENT = -1;		
		static const int _TRANSLATE_PART = -2;		
		static const int _CMD_REQ_PART = -200;		
		static const int _CMD_RPL_PART = -201;		

		static const int _pt_pmink_daemon_type = 6000;
		static const int _pt_pmink_daemon_id = 6001;
		static const int _pt_pmink_auth_id = 6002;
		static const int _pt_pmink_auth_password = 6003;
		static const int _pt_pmink_daemon_ip = 6004;
		static const int _pt_pmink_daemon_port = 6005;
		static const int _pt_pmink_daemon_description = 6006;
		static const int _pt_pmink_action = 6007;
		static const int _pt_pmink_dpi = 6008;
		static const int _pt_pmink_spi = 6009;
		static const int _pt_pmink_timestamp = 6010;
		static const int _pt_pmink_timestamp_nsec = 6011;
		static const int _pt_pmink_security_phase = 6012;
		static const int _pt_pmink_loop_count = 6013;
		static const int _pt_pmink_routing_destination = 6100;
		static const int _pt_pmink_routing_source = 6101;
		static const int _pt_pmink_routing_gateway = 6102;
		static const int _pt_pmink_routing_interface = 6103;
		static const int _pt_pmink_routing_priority = 6104;
		static const int _pt_pmink_router_status = 6105;
		static const int _pt_pmink_routing_destination_type = 6106;
		static const int _pt_pmink_routing_index = 6107;
		static const int _pt_pmink_trunk_label = 6108;
		static const int _pt_pmink_connection_type = 6109;
		static const int _pt_pmink_service_id = 6110;
		static const int _pt_pmink_command_id = 6111;
		static const int _pt_pmink_routing_sub_destination = 6112;
		static const int _pt_pmink_routing_sub_destination_type = 6113;
		static const int _pt_pmink_correlation_notification = 6114;
		static const int _pt_pmink_guid = 6115;
		static const int _pt_pmink_event_id = 6200;
		static const int _pt_pmink_event_description = 6201;
		static const int _pt_pmink_event_callback_id = 6202;
		static const int _pt_pmink_event_callback_priority = 6203;
		static const int _pt_pmink_enc_public_key = 6300;
		static const int _pt_pmink_enc_private_key = 6301;
		static const int _pt_pmink_enc_type = 6302;
		static const int _pt_pmink_stats_id = 6400;
		static const int _pt_pmink_stats_description = 6401;
		static const int _pt_pmink_stats_value = 6402;
		static const int _pt_pmink_stats_count = 6403;
		static const int _pt_pmink_config_param_name = 7400;
		static const int _pt_pmink_config_param_value = 7401;
		static const int _pt_pmink_config_ac_line = 7402;
		static const int _pt_pmink_config_cfg_item_name = 7403;
		static const int _pt_pmink_config_cfg_item_desc = 7404;
		static const int _pt_pmink_config_cfg_item_ns = 7405;
		static const int _pt_pmink_config_cfg_item_value = 7406;
		static const int _pt_pmink_config_cfg_item_nvalue = 7407;
		static const int _pt_pmink_config_cfg_item_nt = 7408;
		static const int _pt_pmink_config_cfg_cm_mode = 7409;
		static const int _pt_pmink_config_cfg_ac_err = 7410;
		static const int _pt_pmink_config_cli_path = 7411;
		static const int _pt_pmink_config_cfg_line = 7412;
		static const int _pt_pmink_config_ac_err_count = 7413;
		static const int _pt_pmink_config_cfg_line_count = 7414;
		static const int _pt_pmink_config_cfg_item_path = 7415;
		static const int _pt_pmink_config_cfg_item_notify = 7416;
		static const int _pt_pmink_config_cfg_item_count = 7417;
		static const int _pt_pmink_config_replication_line = 7418;
		static const int _pt_pmink_sms_status = 7500;
		static const int _pt_pmink_sms_uuid = 7501;
		static const int _pt_pmink_filter_result = 7600;
		static const int _pt_pmink_filter_exit = 7601;
		static const int _pt_pmink_filter_list_id = 7602;
		static const int _pt_pmink_filter_list_label = 7603;
		static const int _pt_pmink_filter_data = 7604;
		static const int _pt_pmink_filter_data_size = 7605;
		static const int _pt_eth_destination_mac = 600;
		static const int _pt_eth_source_mac = 601;
		static const int _pt_ip_destination_ip = 700;
		static const int _pt_ip_source_ip = 701;
		static const int _pt_tcp_destination_port = 800;
		static const int _pt_tcp_source_port = 801;
		static const int _pt_udp_destination_port = 900;
		static const int _pt_udp_source_port = 901;
		static const int _pt_sctp_destination_port = 1000;
		static const int _pt_sctp_source_port = 1001;
		static const int _pt_gsmmap_scoa_digits = 500;
		static const int _pt_gsmmap_scoa_type_of_number = 501;
		static const int _pt_gsmmap_scoa_numbering_plan = 502;
		static const int _pt_gsmmap_scda_digits = 503;
		static const int _pt_gsmmap_scda_type_of_number = 504;
		static const int _pt_gsmmap_scda_numbering_plan = 505;
		static const int _pt_gsmmap_imsi = 506;
		static const int _pt_gsmmap_msisdn_digits = 507;
		static const int _pt_gsmmap_msisdn_type_of_number = 508;
		static const int _pt_gsmmap_msisdn_numbering_plan = 509;
		static const int _pt_tcap_source_transaction_id = 510;
		static const int _pt_tcap_destination_transaction_id = 511;
		static const int _pt_tcap_opcode = 512;
		static const int _pt_tcap_component_type = 513;
		static const int _pt_tcap_component_invoke_id = 514;
		static const int _pt_tcap_error_type = 515;
		static const int _pt_tcap_error_code = 516;
		static const int _pt_tcap_dialogue_context_oid = 517;
		static const int _pt_tcap_message_type = 518;
		static const int _pt_gsmmap_nnn_digits = 519;
		static const int _pt_gsmmap_nnn_type_of_number = 520;
		static const int _pt_gsmmap_nnn_numbering_plan = 521;
		static const int _pt_gsmmap_an_digits = 522;
		static const int _pt_gsmmap_an_type_of_number = 523;
		static const int _pt_gsmmap_an_numbering_plan = 524;
		static const int _pt_gsmmap_sca_digits = 525;
		static const int _pt_gsmmap_sca_type_of_number = 526;
		static const int _pt_gsmmap_sca_numbering_plan = 527;
		static const int _pt_tcap_component_count = 528;
		static const int _pt_tcap_dialogue_context_supported = 529;
		static const int _pt_tcap_component_index= 530;
		static const int _pt_tcap_source_transaction_id_length = 531;
		static const int _pt_tcap_destination_transaction_id_length = 532;
		static const int _pt_gsmmap_version = 533;
		static const int _pt_smstpdu_tp_udhi = 400;
		static const int _pt_smstpdu_tp_sri = 401;
		static const int _pt_smstpdu_tp_mms = 402;
		static const int _pt_smstpdu_tp_mti = 403;
		static const int _pt_smstpdu_tp_oa_type_of_number = 404;
		static const int _pt_smstpdu_tp_oa_numbering_plan = 405;
		static const int _pt_smstpdu_tp_oa_digits = 406;
		static const int _pt_smstpdu_tp_pid = 407;
		static const int _pt_smstpdu_tp_dcs = 408;
		static const int _pt_smstpdu_tp_scts = 409;
		static const int _pt_smstpdu_tp_udl = 410;
		static const int _pt_smstpdu_tp_ud = 411;
		static const int _pt_smstpdu_tp_rp = 412;
		static const int _pt_smstpdu_tp_srr = 413;
		static const int _pt_smstpdu_tp_vpf = 414;
		static const int _pt_smstpdu_tp_rd = 415;
		static const int _pt_smstpdu_tp_da_type_of_number = 416;
		static const int _pt_smstpdu_tp_da_numbering_plan = 417;
		static const int _pt_smstpdu_tp_da_digits = 418;
		static const int _pt_smstpdu_tp_vp = 419;
		static const int _pt_smstpdu_msg_id = 420;
		static const int _pt_smstpdu_msg_parts = 421;
		static const int _pt_smstpdu_msg_part = 422;
		static const int _pt_smstpdu_tp_mr = 423;
		static const int _pt_smstpdu_message_class = 424;
		static const int _pt_sccp_destination_local_reference = 300;
		static const int _pt_sccp_source_local_reference = 301;
		static const int _pt_sccp_called_party = 301;
		static const int _pt_sccp_calling_party = 302;
		static const int _pt_sccp_protocol_class = 303;
		static const int _pt_sccp_segmenting_reassembling = 304;
		static const int _pt_sccp_receive_sequence_number = 305;
		static const int _pt_sccp_sequencing_segmenting = 306;
		static const int _pt_sccp_credit = 307;
		static const int _pt_sccp_release_cause = 308;
		static const int _pt_sccp_return_cause = 309;
		static const int _pt_sccp_reset_cause = 310;
		static const int _pt_sccp_error_cause = 311;
		static const int _pt_sccp_refusal_cause = 312;
		static const int _pt_sccp_data = 313;
		static const int _pt_sccp_segmentation = 314;
		static const int _pt_sccp_hop_counter = 315;
		static const int _pt_sccp_importance = 316;
		static const int _pt_sccp_long_data = 317;
		static const int _pt_sccp_called_pa_routing_indicator = 318;
		static const int _pt_sccp_called_pa_global_title_indicator = 319;
		static const int _pt_sccp_called_pa_ssn_indicator = 320;
		static const int _pt_sccp_called_pa_point_code_indicator = 321;
		static const int _pt_sccp_called_pa_point_code_number = 322;
		static const int _pt_sccp_called_pa_subsystem_number = 323;
		static const int _pt_sccp_called_pa_gt_numbering_plan = 324;
		static const int _pt_sccp_called_pa_gt_encoding_scheme = 325;
		static const int _pt_sccp_called_pa_gt_nature_of_address = 326;
		static const int _pt_sccp_called_pa_gt_address = 327;
		static const int _pt_sccp_called_pa_gt_translation_type = 328;
		static const int _pt_sccp_calling_pa_routing_indicator = 329;
		static const int _pt_sccp_calling_pa_global_title_indicator = 330;
		static const int _pt_sccp_calling_pa_ssn_indicator = 331;
		static const int _pt_sccp_calling_pa_point_code_indicator = 332;
		static const int _pt_sccp_calling_pa_point_code_number = 333;
		static const int _pt_sccp_calling_pa_subsystem_number = 334;
		static const int _pt_sccp_calling_pa_gt_numbering_plan = 335;
		static const int _pt_sccp_calling_pa_gt_encoding_scheme = 336;
		static const int _pt_sccp_calling_pa_gt_nature_of_address = 337;
		static const int _pt_sccp_calling_pa_gt_address = 338;
		static const int _pt_sccp_calling_pa_gt_translation_type = 339;
		static const int _pt_sccp_message_type = 340;
		static const int _pt_m3ua_info_string = 200;
		static const int _pt_m3ua_routing_context = 201;
		static const int _pt_m3ua_diagnostic_info = 202;
		static const int _pt_m3ua_heartbeat = 203;
		static const int _pt_m3ua_traffic_mode_type = 204;
		static const int _pt_m3ua_error_code = 205;
		static const int _pt_m3ua_status = 206;
		static const int _pt_m3ua_asp_identifier = 207;
		static const int _pt_m3ua_affected_point_code = 208;
		static const int _pt_m3ua_correlation_id = 209;
		static const int _pt_m3ua_network_appearance = 210;
		static const int _pt_m3ua_user_cause = 211;
		static const int _pt_m3ua_congestion_indications = 212;
		static const int _pt_m3ua_concerned_destination = 213;
		static const int _pt_m3ua_routing_key = 214;
		static const int _pt_m3ua_registration_result = 215;
		static const int _pt_m3ua_deregistration_result = 216;
		static const int _pt_m3ua_local_routing_key_identifier = 217;
		static const int _pt_m3ua_destination_point_code = 218;
		static const int _pt_m3ua_service_indicators = 219;
		static const int _pt_m3ua_origination_point_code_list = 220;
		static const int _pt_m3ua_circuit_range = 221;
		static const int _pt_m3ua_protocol_data = 222;
		static const int _pt_m3ua_protocol_data_service_indicator = 223;
		static const int _pt_m3ua_protocol_data_network_indicator = 224;
		static const int _pt_m3ua_protocol_data_message_priority = 225;
		static const int _pt_m3ua_protocol_data_destination_point_code = 226;
		static const int _pt_m3ua_protocol_data_originating_point_code = 227;
		static const int _pt_m3ua_protocol_data_signalling_link_selection_code = 228;
		static const int _pt_m3ua_registration_status = 229;
		static const int _pt_m3ua_deregistration_status = 230;
		static const int _pt_m3ua_header_data = 231;
		static const int _pt_m3ua_as_label = 232;
		static const int _pt_m3ua_asp_label = 233;
		static const int _pt_smpp_system_id = 100;
		static const int _pt_smpp_password = 101;
		static const int _pt_smpp_service_type = 102;
		static const int _pt_smpp_originator_ton = 103;
		static const int _pt_smpp_originator_np = 104;
		static const int _pt_smpp_originator_address = 105;
		static const int _pt_smpp_recipient_ton = 106;
		static const int _pt_smpp_recipient_np = 107;
		static const int _pt_smpp_recipient_address = 108;
		static const int _pt_smpp_esm_message_mode = 109;
		static const int _pt_smpp_esm_message_type = 110;
		static const int _pt_smpp_esm_gsm_features = 111;
		static const int _pt_smpp_protocol_id = 112;
		static const int _pt_smpp_priority_flag = 113;
		static const int _pt_smpp_delivery_time = 114;
		static const int _pt_smpp_validity_period = 115;
		static const int _pt_smpp_rd_smsc_receipt = 116;
		static const int _pt_smpp_rd_sme_ack = 117;
		static const int _pt_smpp_rd_intermediate_notification = 118;
		static const int _pt_smpp_replace_if_present_flag = 119;
		static const int _pt_smpp_data_coding = 120;
		static const int _pt_smpp_sm_defaut_msg_id = 121;
		static const int _pt_smpp_sm_length = 122;
		static const int _pt_smpp_sm = 123;
		static const int _pt_smpp_command_id = 124;
		static const int _pt_smpp_sar_msg_ref_num = 125;
		static const int _pt_smpp_sar_total_segments = 126;
		static const int _pt_smpp_sar_segment_seqnum = 127;
		static const int _pt_smpp_header_data = 128;
		static const int _pt_smpp_peer_ip = 129;
		static const int _pt_smpp_peer_port = 130;
		static const int _pt_smpp_sequence = 131;
		static const int _pt_smpp_as_label = 132;
		static const int _pt_smpp_asp_label = 133;
		static const int _pt_smpp_dlvr_rcpt_id = 134;
		static const int _pt_smpp_dlvr_rcpt_sub = 135;
		static const int _pt_smpp_dlvr_rcpt_dlvrd = 136;
		static const int _pt_smpp_dlvr_rcpt_submit_date = 137;
		static const int _pt_smpp_dlvr_rcpt_done_date = 138;
		static const int _pt_smpp_dlvr_rcpt_stat = 139;
		static const int _pt_smpp_dlvr_rcpt_err = 140;
		static const int _pt_smpp_dlvr_rcpt_text = 141;
		static const int _pt_smpp_message_id = 142;
		static const int _pt_smpp_command_status = 143;
		static const int _pt_smpp_default_customer_encoding = 144;
		static const int _pt_smpp_default_vendor_encoding = 145;
		
	};
        // *** ascii regex ***
        int pmink_lua_regex_count(void* pm, const char* data, const char* regex);
        rgx_result_t pmink_lua_regex_match(void* pm, const char* data, const char* regex, bool grps);

	// *** utf8 ***
	int pmink_lua_utf8_upper(void* pm, const char* data, char* out);
	int pmink_lua_utf8_lower(void* pm, const char* data, char* out);
	rgx_result_t pmink_lua_utf8_regex_match(void* pm, const char* data, const char* regex, bool grps);
        int pmink_lua_utf8_regex_count(void* pm, const char* data, const char* regex);


	// *** pd request ***
	int pmink_lua_pd_req(void* pm, int action);
	void* pmink_lua_pd_req_vp_new(void* pm, int var);

	// *** hlr request ***
	void* pmink_lua_hlr_req_vp_new(void* pm, int var);
	int pmink_lua_hlr_req(void* pm);
	

	// *** flood lists ***
	// delete flood list or item in flood list
	bool pmink_lua_fl_del(void* pm, const char* list, const char* item);
	// new flood list
	bool pmink_lua_fl_new(void* pm, const char* label);
	// get item count in flood list for specified interval
	int pmink_lua_fl_get(void* pm, const char* list, int interval, const char* item);
	// add item to flood list
	bool pmink_lua_fl_add(void* pm, const char* list, const char* item);

	// ** standard lists ***
	// new standard list
	bool pmink_lua_sl_new(void* pm, const char* label);
	// delete standard list or item in list
	bool pmink_lua_sl_del(void* pm, const char* list, const char* item);
	// add new item to standard list
	bool pmink_lua_sl_add(void* pm, const char* list, const char* item);
	// checkk if item exists in standard list
	bool pmink_lua_sl_get(void* pm, const char* list, const char* item);
	// get list size
	int pmink_lua_sl_get_size(void* pm, const char* list);
	// get list item
	const char* pmink_lua_sl_get_item(void* pm, const char* list, int index);
	// *** VariantParam ***
	// new VariantParam
	void* pmink_lua_vp_new(void* pm, int var, int index);
	// get VariantParam pointer
	void* pmink_lua_vp_get(void* pm, int var, int index);
	// VariantParam to c string
	char* pmink_lua_vp_to_str(void* pm, void* vp);
	// VariantParam to bool
	bool pmink_lua_vp_to_bool(void* pm, void* vp);
	// VariantParam to c int
	int pmink_lua_vp_to_int(void* pm, void* vp);
	// VariantParam to c double
	double pmink_lua_vp_to_double(void* pm, void* vp);
	// VariantParam to c char
	char pmink_lua_vp_to_char(void* pm, void* vp);
	// VariantParam to byte array
	unsigned char* pmink_lua_vp_to_octets(void* pm, void* vp);
	// VariantParam to pointer
	void* pmink_lua_vp_to_pointer(void* pm, void* vp);
	// VariantParam get type
	int pmink_lua_vp_get_type(void* pm, void* vp);
	// VariantParam get size in bytes
	int pmink_lua_vp_get_size(void* pm, void* vp);
	// VariantParam set C string
	bool pmink_lua_vp_set_str(void* pm, void* vp, const char* val);
	// VariantParam set int
	bool pmink_lua_vp_set_int(void* pm, void* vp, int val);
	// VariantParam set bool
	bool pmink_lua_vp_set_bool(void* pm, void* vp, bool val);
	// VariantParam set double
	bool pmink_lua_vp_set_double(void* pm, void* vp, double val);
	// VariantParam set char
	bool pmink_lua_vp_set_char(void* pm, void* vp, char val);
	// VariantParam set octets
	bool pmink_lua_vp_set_octets(void* pm, void* vp, unsigned char* val, int size);
	// VariantParam set pointer
	bool pmink_lua_vp_set_pointer(void* pm, void* vp, void* val);

        // C library
        void free(void*);
]]


-- *******************
-- *** pd wrapper ***
-- *******************
local function w_pd_match(req, res)
	-- sanity check
	if(req == nil or res == nil) then return 1 else
		-- create/set vp for every pd request param
		-- list id
		local vp_list_id 		= C.pmink_lua_pd_req_vp_new(pmink.args[1], pmink.CONST._pt_pmink_filter_list_id)
		-- filter data
		local vp_filter_data 		= C.pmink_lua_pd_req_vp_new(pmink.args[1], pmink.CONST._pt_pmink_filter_data)
		-- filter data size
		local vp_filter_data_size 	= C.pmink_lua_pd_req_vp_new(pmink.args[1], pmink.CONST._pt_pmink_filter_data_size)
		-- data size is mandatory
		if(vp_filter_data_size == nil) then return 2 end

		-- set values in vp params
		-- list id
		if(req[pmink.CONST._pt_pmink_filter_list_id] 	~= nil) then pmink.vpset(vp_list_id, 	 req[pmink.CONST._pt_pmink_filter_list_id]) end
		-- filter data size
		pmink.vpset(vp_filter_data_size, req[pmink.CONST._pt_pmink_filter_data_size])
		-- filter data
		if(req[pmink.CONST._pt_pmink_filter_data] 	~= nil) then pmink.vpset(vp_filter_data, req[pmink.CONST._pt_pmink_filter_data], pmink.vpval(vp_filter_data_size)) end
	
		-- *** send pd request and pause lua (3 = match action) ***
		C.pmink_lua_pd_req(pmink.args[1], 3)
		coroutine.yield()
		
		-- *** resume lua on pd reply or timeout ***
		-- get vp for every pd result param

		-- pd result
		local vp_pd_res	= pmink.vpget(pmink.CONST._pt_pmink_filter_result, pmink.CONST._CMD_RPL_PART)
		
		-- copy vp data to result table
		-- pd result
		if(vp_pd_res ~= nil) then res[pmink.CONST._pt_pmink_filter_result] = pmink.vpval(vp_pd_res) end

		-- ok
		return 0


	end
	-- err
	return 1
end

local function w_pd_add(req, res)
	-- sanity check
	if(req == nil or res == nil) then return 1 else
		-- create/set vp for every pd request param
		-- list id
		local vp_list_id 		= C.pmink_lua_pd_req_vp_new(pmink.args[1], pmink.CONST._pt_pmink_filter_list_id)
		-- filter data
		local vp_filter_data 		= C.pmink_lua_pd_req_vp_new(pmink.args[1], pmink.CONST._pt_pmink_filter_data)
		-- filter data size
		local vp_filter_data_size 	= C.pmink_lua_pd_req_vp_new(pmink.args[1], pmink.CONST._pt_pmink_filter_data_size)
		-- data size is mandatory
		if(vp_filter_data_size == nil) then return 2 end

		-- set values in vp params
		-- list id
		if(req[pmink.CONST._pt_pmink_filter_list_id] 	~= nil) then pmink.vpset(vp_list_id, 	 req[pmink.CONST._pt_pmink_filter_list_id]) end
		-- filter data size
		pmink.vpset(vp_filter_data_size, req[pmink.CONST._pt_pmink_filter_data_size])
		-- filter data
		if(req[pmink.CONST._pt_pmink_filter_data] 	~= nil) then pmink.vpset(vp_filter_data, req[pmink.CONST._pt_pmink_filter_data], pmink.vpval(vp_filter_data_size)) end
	
		-- *** send pd request and pause lua (1 = add action) ***
		C.pmink_lua_pd_req(pmink.args[1], 1)
		coroutine.yield()
		
		-- *** resume lua on pd reply or timeout ***
		-- get vp for every pd result param

		-- pd result
		local vp_pd_res	= pmink.vpget(pmink.CONST._pt_pmink_filter_result, pmink.CONST._CMD_RPL_PART)
		
		-- copy vp data to result table
		-- pd result
		if(vp_pd_res ~= nil) then res[pmink.CONST._pt_pmink_filter_result] = pmink.vpval(vp_pd_res) end

		-- ok
		return 0
	end
	-- err
	return 1
end

local function w_pd_del(req, res)
	-- sanity check
	if(req == nil or res == nil) then return 1 else
		-- create/set vp for every pd request param
		-- list id
		local vp_list_id 		= C.pmink_lua_pd_req_vp_new(pmink.args[1], pmink.CONST._pt_pmink_filter_list_id)
		-- filter data
		local vp_filter_data 		= C.pmink_lua_pd_req_vp_new(pmink.args[1], pmink.CONST._pt_pmink_filter_data)
		-- filter data size
		local vp_filter_data_size 	= C.pmink_lua_pd_req_vp_new(pmink.args[1], pmink.CONST._pt_pmink_filter_data_size)
		-- data size is mandatory
		if(vp_filter_data_size == nil) then return 2 end

		-- set values in vp params
		-- list id
		if(req[pmink.CONST._pt_pmink_filter_list_id] 	~= nil) then pmink.vpset(vp_list_id, 	 req[pmink.CONST._pt_pmink_filter_list_id]) end
		-- filter data size
		pmink.vpset(vp_filter_data_size, req[pmink.CONST._pt_pmink_filter_data_size])
		-- filter data
		if(req[pmink.CONST._pt_pmink_filter_data] 	~= nil) then pmink.vpset(vp_filter_data, req[pmink.CONST._pt_pmink_filter_data], pmink.vpval(vp_filter_data_size)) end
	
		-- *** send pd request and pause lua (2 = del action) ***
		C.pmink_lua_pd_req(pmink.args[1], 2)
		coroutine.yield()
		
		-- *** resume lua on pd reply or timeout ***
		-- get vp for every pd result param

		-- pd result
		local vp_pd_res	= pmink.vpget(pmink.CONST._pt_pmink_filter_result, pmink.CONST._CMD_RPL_PART)
		
		-- copy vp data to result table
		-- pd result
		if(vp_pd_res ~= nil) then res[pmink.CONST._pt_pmink_filter_result] = pmink.vpval(vp_pd_res) end

		-- ok
		return 0
	end
	-- err
	return 1
end

-- *******************
-- *** hlr wrapper ***
-- *******************
local function w_hlr_req(req, res)
	-- sanity check
	if(req == nil or res == nil) then return 1 else
		-- create/set vp for every hlr request param
		-- gsm map version
		local vp_gsmmap_ver 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_gsmmap_version)
		-- gsm map msisdn
		local vp_gsmmap_msisdn_ton 	= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_gsmmap_msisdn_type_of_number)
		local vp_gsmmap_msisdn_np 	= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_gsmmap_msisdn_numbering_plan)
		local vp_gsmmap_msisdn_addr 	= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_gsmmap_msisdn_digits)
		-- gsm map sca	
		local vp_gsmmap_sca_ton 	= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_gsmmap_sca_type_of_number)
		local vp_gsmmap_sca_np 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_gsmmap_sca_numbering_plan)
		local vp_gsmmap_sca_addr 	= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_gsmmap_sca_digits)
		-- sccp called party
		local vp_cdpa_ssn 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_called_pa_subsystem_number)
		local vp_cdpa_gt_tt 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_called_pa_gt_translation_type)
		local vp_cdpa_gt_np 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_called_pa_gt_numbering_plan)
		local vp_cdpa_gt_nai 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_called_pa_gt_nature_of_address)
		local vp_cdpa_gt_addr 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_called_pa_gt_address)
		-- sccp calling party
		local vp_cgpa_ssn 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_calling_pa_subsystem_number)
		local vp_cgpa_gt_tt 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_calling_pa_gt_translation_type)
		local vp_cgpa_gt_np 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_calling_pa_gt_numbering_plan)
		local vp_cgpa_gt_nai 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_calling_pa_gt_nature_of_address)
		local vp_cgpa_gt_addr 		= C.pmink_lua_hlr_req_vp_new(pmink.args[1], pmink.CONST._pt_sccp_calling_pa_gt_address)
		
		-- set values in vp params
		-- gsm map version
		if(req[pmink.CONST._pt_gsmmap_version] 			     ~= nil) 	then pmink.vpset(vp_gsmmap_ver, 	  req[pmink.CONST._pt_gsmmap_version]) end

		-- gsm map msisdn
		if(req[pmink.CONST._pt_gsmmap_msisdn_type_of_number] 	     ~= nil) 	then pmink.vpset(vp_gsmmap_msisdn_ton, 	  req[pmink.CONST._pt_gsmmap_msisdn_type_of_number]) end
		if(req[pmink.CONST._pt_gsmmap_msisdn_numbering_plan]	     ~= nil) 	then pmink.vpset(vp_gsmmap_msisdn_np, 	  req[pmink.CONST._pt_gsmmap_msisdn_numbering_plan]) end
		if(req[pmink.CONST._pt_gsmmap_msisdn_digits] 		     ~= nil) 	then pmink.vpset(vp_gsmmap_msisdn_addr,   req[pmink.CONST._pt_gsmmap_msisdn_digits]) end
		-- gsm map sca
		if(req[pmink.CONST._pt_gsmmap_sca_type_of_number] 	     ~= nil)	then pmink.vpset(vp_gsmmap_sca_ton,	  req[pmink.CONST._pt_gsmmap_sca_type_of_number]) end
		if(req[pmink.CONST._pt_gsmmap_sca_numbering_plan] 	     ~= nil) 	then pmink.vpset(vp_gsmmap_sca_np,	  req[pmink.CONST._pt_gsmmap_sca_numbering_plan]) end
		if(req[pmink.CONST._pt_gsmmap_sca_digits] 		     ~= nil) 	then pmink.vpset(vp_gsmmap_sca_addr, 	  req[pmink.CONST._pt_gsmmap_sca_digits]) end
		-- sccp called party
		if(req[pmink.CONST._pt_sccp_called_pa_subsystem_number]      ~= nil) 	then pmink.vpset(vp_cdpa_ssn, 	  	  req[pmink.CONST._pt_sccp_called_pa_subsystem_number]) end
		if(req[pmink.CONST._pt_sccp_called_pa_gt_translation_type]   ~= nil) 	then pmink.vpset(vp_cdpa_gt_tt,   	  req[pmink.CONST._pt_sccp_called_pa_gt_translation_type]) end
		if(req[pmink.CONST._pt_sccp_called_pa_gt_numbering_plan]     ~= nil) 	then pmink.vpset(vp_cdpa_gt_np,   	  req[pmink.CONST._pt_sccp_called_pa_gt_numbering_plan]) end
		if(req[pmink.CONST._pt_sccp_called_pa_gt_nature_of_address]  ~= nil) 	then pmink.vpset(vp_cdpa_gt_nai,  	  req[pmink.CONST._pt_sccp_called_pa_gt_nature_of_address]) end
		if(req[pmink.CONST._pt_sccp_called_pa_gt_address] 	     ~= nil) 	then pmink.vpset(vp_cdpa_gt_addr, 	  req[pmink.CONST._pt_sccp_called_pa_gt_address]) end
		-- sccp calling party
		if(req[pmink.CONST._pt_sccp_calling_pa_subsystem_number]     ~= nil) 	then pmink.vpset(vp_cgpa_ssn, 		  req[pmink.CONST._pt_sccp_calling_pa_subsystem_number]) end
		if(req[pmink.CONST._pt_sccp_calling_pa_gt_translation_type]  ~= nil) 	then pmink.vpset(vp_cgpa_gt_tt, 	  req[pmink.CONST._pt_sccp_calling_pa_gt_translation_type]) end
		if(req[pmink.CONST._pt_sccp_calling_pa_gt_numbering_plan]    ~= nil) 	then pmink.vpset(vp_cgpa_gt_np, 	  req[pmink.CONST._pt_sccp_calling_pa_gt_numbering_plan]) end
		if(req[pmink.CONST._pt_sccp_calling_pa_gt_nature_of_address] ~= nil) 	then pmink.vpset(vp_cgpa_gt_nai, 	  req[pmink.CONST._pt_sccp_calling_pa_gt_nature_of_address]) end
		if(req[pmink.CONST._pt_sccp_calling_pa_gt_address] 	     ~= nil)	then pmink.vpset(vp_cgpa_gt_addr, 	  req[pmink.CONST._pt_sccp_calling_pa_gt_address]) end

		-- *** send hlr request and pause lua ***
		C.pmink_lua_hlr_req(pmink.args[1])
		coroutine.yield()
		
		-- *** resume lua on hlr reply or timeout ***
		-- get vp for every hlr result param
		-- sccp called party
		vp_cdpa_ssn 	 = pmink.vpget(pmink.CONST._pt_sccp_called_pa_subsystem_number,      pmink.CONST._CMD_RPL_PART)
		vp_cdpa_gt_tt 	 = pmink.vpget(pmink.CONST._pt_sccp_called_pa_gt_translation_type,   pmink.CONST._CMD_RPL_PART)
		vp_cdpa_gt_np 	 = pmink.vpget(pmink.CONST._pt_sccp_called_pa_gt_numbering_plan,     pmink.CONST._CMD_RPL_PART)
		vp_cdpa_gt_nai 	 = pmink.vpget(pmink.CONST._pt_sccp_called_pa_gt_nature_of_address,  pmink.CONST._CMD_RPL_PART)
		vp_cdpa_gt_addr  = pmink.vpget(pmink.CONST._pt_sccp_called_pa_gt_address,     	     pmink.CONST._CMD_RPL_PART)
		-- sccp calling party
		vp_cgpa_ssn 	 = pmink.vpget(pmink.CONST._pt_sccp_calling_pa_subsystem_number,     pmink.CONST._CMD_RPL_PART)
		vp_cgpa_gt_tt 	 = pmink.vpget(pmink.CONST._pt_sccp_calling_pa_gt_translation_type,  pmink.CONST._CMD_RPL_PART)
		vp_cgpa_gt_np 	 = pmink.vpget(pmink.CONST._pt_sccp_calling_pa_gt_numbering_plan,    pmink.CONST._CMD_RPL_PART)
		vp_cgpa_gt_nai 	 = pmink.vpget(pmink.CONST._pt_sccp_calling_pa_gt_nature_of_address, pmink.CONST._CMD_RPL_PART)
		vp_cgpa_gt_addr  = pmink.vpget(pmink.CONST._pt_sccp_calling_pa_gt_address, 	     pmink.CONST._CMD_RPL_PART)
		-- gsm map
		-- nnn
		local vp_gsmmap_nnn_ton  = pmink.vpget(pmink.CONST._pt_gsmmap_nnn_type_of_number,    pmink.CONST._CMD_RPL_PART)
		local vp_gsmmap_nnn_np 	 = pmink.vpget(pmink.CONST._pt_gsmmap_nnn_numbering_plan,    pmink.CONST._CMD_RPL_PART)
		local vp_gsmmap_nnn_addr = pmink.vpget(pmink.CONST._pt_gsmmap_nnn_digits,            pmink.CONST._CMD_RPL_PART)
		-- an
		local vp_gsmmap_an_ton 	 = pmink.vpget(pmink.CONST._pt_gsmmap_an_type_of_number,     pmink.CONST._CMD_RPL_PART)
		local vp_gsmmap_an_np 	 = pmink.vpget(pmink.CONST._pt_gsmmap_an_numbering_plan,     pmink.CONST._CMD_RPL_PART)
		local vp_gsmmap_an_addr  = pmink.vpget(pmink.CONST._pt_gsmmap_an_digits,             pmink.CONST._CMD_RPL_PART)
		-- imsi
		local vp_gsmmap_imsi  	 = pmink.vpget(pmink.CONST._pt_gsmmap_imsi,                  pmink.CONST._CMD_RPL_PART)
		--
		-- copy vp data to result table
		-- sccp called party
		if(vp_cdpa_ssn 		~= nil) then res[pmink.CONST._pt_sccp_called_pa_subsystem_number] 	= pmink.vpval(vp_cdpa_ssn) end
		if(vp_cdpa_gt_tt 	~= nil) then res[pmink.CONST._pt_sccp_called_pa_gt_translation_type]   	= pmink.vpval(vp_cdpa_gt_tt) end
		if(vp_cdpa_gt_np 	~= nil) then res[pmink.CONST._pt_sccp_called_pa_gt_numbering_plan] 	= pmink.vpval(vp_cdpa_gt_np) end
		if(vp_cdpa_gt_nai 	~= nil) then res[pmink.CONST._pt_sccp_called_pa_gt_nature_of_address]  	= pmink.vpval(vp_cdpa_gt_nai) end
		if(vp_cdpa_gt_addr 	~= nil) then res[pmink.CONST._pt_sccp_called_pa_gt_address] 		= pmink.vpval(vp_cdpa_gt_addr) end
		-- sccp calling party
		if(vp_cgpa_ssn 		~= nil) then res[pmink.CONST._pt_sccp_calling_pa_subsystem_number] 	= pmink.vpval(vp_cgpa_ssn) end
		if(vp_cgpa_gt_tt 	~= nil) then res[pmink.CONST._pt_sccp_calling_pa_gt_translation_type]  	= pmink.vpval(vp_cgpa_gt_tt) end
		if(vp_cgpa_gt_np 	~= nil) then res[pmink.CONST._pt_sccp_calling_pa_gt_numbering_plan] 	= pmink.vpval(vp_cgpa_gt_np) end
		if(vp_cgpa_gt_nai 	~= nil) then res[pmink.CONST._pt_sccp_calling_pa_gt_nature_of_address] 	= pmink.vpval(vp_cgpa_gt_nai) end
		if(vp_cgpa_gt_addr 	~= nil) then res[pmink.CONST._pt_sccp_calling_pa_gt_address] 	  	= pmink.vpval(vp_cgpa_gt_addr) end
		-- gsm map
		-- nnn
		if(vp_gsmmap_nnn_ton 	~= nil) then res[pmink.CONST._pt_gsmmap_nnn_type_of_number] 	  	= pmink.vpval(vp_gsmmap_nnn_ton) end
		if(vp_gsmmap_nnn_np 	~= nil) then res[pmink.CONST._pt_gsmmap_nnn_numbering_plan]  	  	= pmink.vpval(vp_gsmmap_nnn_np) end
		if(vp_gsmmap_nnn_addr 	~= nil) then res[pmink.CONST._pt_gsmmap_nnn_digits] 	  		= pmink.vpval(vp_gsmmap_nnn_addr) end
		-- an
		if(vp_gsmmap_an_ton 	~= nil) then res[pmink.CONST._pt_gsmmap_an_type_of_number] 	  	= pmink.vpval(vp_gsmmap_an_ton) end
		if(vp_gsmmap_an_np 	~= nil) then res[pmink.CONST._pt_gsmmap_an_numbering_plan]  	  	= pmink.vpval(vp_gsmmap_an_np) end
		if(vp_gsmmap_an_addr 	~= nil) then res[pmink.CONST._pt_gsmmap_an_digits] 	  		= pmink.vpval(vp_gsmmap_an_addr) end
		-- imsi
		if(vp_gsmmap_imsi 	~= nil) then res[pmink.CONST._pt_gsmmap_imsi] 	  		  	= pmink.vpval(vp_gsmmap_imsi) end
		
		-- ok
		return 0
	end	
	-- err
	return 1
end



-- **********************
-- *** flood wrappers ***
-- **********************
-- fl_get wrapper
local function w_fl_get(list, interval, var)
	return C.pmink_lua_fl_get(pmink.args[1], list, interval, var)
end

-- fl_del wrapper
local function w_fl_del(list, var)
	return C.pmink_lua_fl_del(pmink.args[1], list, var)
end

-- fl_new wrapper
local function w_fl_new(label)
	return C.pmink_lua_fl_new(pmink.args[1], label)
end

-- fl_add wrapper
local function w_fl_add(list, var)
	return C.pmink_lua_fl_add(pmink.args[1], list, var)
end

-- *******************
-- *** sl wrappers ***
-- *******************
-- sl_add wrapper
local function w_sl_add(list, item)
	return C.pmink_lua_sl_add(pmink.args[1], list, item)
end

-- sl_get wrapper
local function w_sl_get(list, item)
	return C.pmink_lua_sl_get(pmink.args[1], list, item)
end

-- sl_new wrapper
local function w_sl_new(label)
	return C.pmink_lua_sl_new(pmink.args[1], label)
end


-- sl_del wrapper
local function w_sl_del(list, item)
	return C.pmink_lua_sl_del(pmink.args[1], list, item)
end

-- sl_size wrapper
local function w_sl_size(list)
	return C.pmink_lua_sl_get_size(pmink.args[1], list)
end

-- sl_getbi wrapper
local function w_sl_getbi(list, index)
	return C.pmink_lua_sl_get_item(pmink.args[1], list, index)
end


-- ******************************
-- *** variant param wrappers ***
-- ******************************
-- vp_get wrapper
local function w_vp_get(var, index)
	return C.pmink_lua_vp_get(pmink.args[1], (var == nil and -1 or var), (index == nil and -1 or index))
end

-- getval wrapper, shortcut for vpval(vpget(id))
local function w_getval(var, index)
	-- get vp
	local vp = C.pmink_lua_vp_get(pmink.args[1], (var == nil and -1 or var), (index == nil and -1 or index))
	-- cp found, return val
	if(vp ~= nil) then return pmink.vpval(vp) end
	-- vp not found
	return nil

end

-- vp_new wrapper
local function w_vp_new(var, index)
	return C.pmink_lua_vp_new(pmink.args[1], var, (index == nil and 0 or index))
end

-- vp_get_size wrapper
local function w_vp_get_size(vp)
	return C.pmink_lua_vp_get_size(pmink.args[1], vp)
end

-- pmink_vp_set wrapper
local function w_vp_set(vp, val, len)
	local vpt = C.pmink_lua_vp_get_type(pmink.args[1], vp)
	-- DPT_INT
	if vpt == 1 then return C.pmink_lua_vp_set_int(pmink.args[1], vp, val)
	-- DPT_STRING
	elseif vpt == 2 then return C.pmink_lua_vp_set_str(pmink.args[1], vp, val)
	-- DPT_DOUBLE
	elseif vpt == 3 then return C.pmink_lua_vp_set_double(pmink.args[1], vp, val)
	-- DPT_CHAR
	elseif vpt == 4 then return C.pmink_lua_vp_set_char(pmink.args[1], vp, val)
	-- DPT_BOOL
	elseif vpt == 5 then return C.pmink_lua_vp_set_bool(pmink.args[1], vp, val)
	-- DPT_OCTETS
	elseif vpt == 6 then return C.pmink_lua_vp_set_octets(pmink.args[1], vp, val, len)
	-- DPT_POINTER
	elseif vpt == 7 then return C.pmink_lua_vp_set_pointer(pmink.args[1], vp, val)
	else return nil end
end

-- vp_to_val wrapper
local function w_vp_to_val(vp)
	if vp == nil then vp = C.pmink_lua_vp_get(pmink.args[1], -1, -1) end
	local vpt = C.pmink_lua_vp_get_type(pmink.args[1], vp)
	-- DPT_INT
	if vpt == 1 then return C.pmink_lua_vp_to_int(pmink.args[1], vp)
	-- DPT_STRING
	elseif vpt == 2 then return C.pmink_lua_vp_to_str(pmink.args[1], vp)
	-- DPT_DOUBLE
	elseif vpt == 3 then return C.pmink_lua_vp_to_double(pmink.args[1], vp)
	-- DPT_CHAR
	elseif vpt == 4 then return C.pmink_lua_vp_to_char(pmink.args[1], vp)
	-- DPT_BOOL
	elseif vpt == 5 then return C.pmink_lua_vp_to_bool(pmink.args[1], vp)
	-- DPT_OCTETS
	elseif vpt == 6 then return C.pmink_lua_vp_to_octets(pmink.args[1], vp)
	-- DPT_POINTER
	elseif vpt == 7 then return C.pmink_lua_vp_to_pointer(pmink.args[1], vp)
	else return nil end
end
-- ****************************
-- *** ascii regex wrappers ***
-- ****************************
-- ascii regex count wrapper
local function w_ascii_regex_count(data, regex)
    return C.pmink_lua_regex_count(pmink.args[1], data, regex);
end

-- ascii regex match wrapper
local function w_ascii_regex_match(data, regex, grps)
    local res = C.pmink_lua_regex_match(pmink.args[1], data, regex, (grps == nil and false or true));
    -- nil if not matched (groups or no groups)
    if not res.matched then return nil end
    -- if not using groups, return true if matched
    if not grps then return true end
    -- if using groups and matched, return table
    local tbl_res = {}
    tbl_res[0] = res.size
    tbl_res['G'] = {}
    for i = 0, res.size - 1 do
        tbl_res['G'][i] = ffi.string(res.groups[i])
        C.free(res.groups[i])
    end
    C.free(res.groups)
    return tbl_res
end

-- **********************
-- *** utf-8 wrappers ***
-- **********************
-- utf8 regex count wrapper
local function w_utf8_regex_count(data, regex)
	return C.pmink_lua_utf8_regex_count(pmink.args[1], data, regex)
end

-- utf8_upper wrapper
local function w_utf8_upper(data)
	-- out buffer
	local out_buff = ffi.new("char[?]", #data * 2)
	-- convert
	local cnv_res = C.pmink_lua_utf8_upper(pmink.args[1], data, out_buff)
	-- check res
	if cnv_res > 0 then
		return ffi.string(out_buff, cvn_res)
	end
	return nil
end

-- utf8_lower wrapper
local function w_utf8_lower(data)
	-- out buffer
	local out_buff = ffi.new("char[?]", #data * 2)
	-- convert
	local cnv_res = C.pmink_lua_utf8_lower(pmink.args[1], data, out_buff)
	-- check res
	if cnv_res > 0 then
		return ffi.string(out_buff, cvn_res)
	end
	return nil
end

-- utf8_regex_match wrapper
local function w_utf8_regex_match(data, regex, grps)
    local res = C.pmink_lua_utf8_regex_match(pmink.args[1], data, regex, (grps == nil and false or true));
    -- nil if not matched (groups or no groups)
    if not res.matched then return nil end
    -- if not using groups, return true if matched
    if not grps then return true end
    -- if using groups and matched, return table
    local tbl_res = {}
    tbl_res[0] = res.size
    tbl_res['G'] = {}
    for i = 0, res.size - 1 do
        tbl_res['G'][i] = ffi.string(res.groups[i])
        C.free(res.groups[i])
    end
    C.free(res.groups)
    return tbl_res
end


-- **************************
-- *** module init method ***
-- **************************
local function init(...)
	-- general
	pmink.CONST = ffi.new("struct pm_consts")
	pmink.CONST_SCCP = ffi.new("struct sccp_consts")
	pmink.args = {...}
	pmink.user_args = pmink.args[2]
	pmink.string = ffi.string
	pmink.cast = ffi.cast
	pmink.sizeof = ffi.sizeof
	-- flood related
	pmink.fl_add = w_fl_add
	pmink.fl_get = w_fl_get
	pmink.fl_del = w_fl_del
	pmink.fl_new = w_fl_new
	-- standard lists
	pmink.sl_add = w_sl_add
	pmink.sl_get = w_sl_get
	pmink.sl_del = w_sl_del
	pmink.sl_new = w_sl_new
	pmink.sl_size = w_sl_size
	pmink.sl_getbi = w_sl_getbi
	-- vp realted
	pmink.vpget = w_vp_get
	pmink.vpset = w_vp_set
	pmink.vpval = w_vp_to_val
	pmink.vpsize = w_vp_get_size
	pmink.vpnew = w_vp_new
	pmink.getval = w_getval
	-- hlr related
	pmink.hlr_req = w_hlr_req
	-- pd related
	pmink.pd_match = w_pd_match
	pmink.pd_add = w_pd_add
	pmink.pd_del = w_pd_del
	-- utf-8 related
	pmink.utf8upper = w_utf8_upper
	pmink.utf8lower = w_utf8_lower
	pmink.utf8match = w_utf8_regex_match
        pmink.utf8cmatch = w_utf8_regex_count
        -- ascii regex related
        pmink.match = w_ascii_regex_match
        pmink.cmatch = w_ascii_regex_count
	return pmink
end

-- return module init method
return init
