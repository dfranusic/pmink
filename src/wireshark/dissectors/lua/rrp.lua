-- rrgmented_data_presentp protocol
-- declare our protocol
rrp_proto = Proto("rrp","R14 Rating Routing Protocol and More")
-- declare protocol fields
rrp_size = ProtoField.uint32("rrp.size", "Size")
rrp_fragment = ProtoField.uint32("rrp.fragment", "Fragment")
rrp_more_fragments = ProtoField.uint32("rrp.more_fragments", "MoreFragments")
rrp_fragment_id = ProtoField.string("rrp.fragment_it", "FragmentID")
method = ProtoField.string("rrp.method", "Method")
sequence_state = ProtoField.string("rrp.sequence_state", "SequenceState")
from_port = ProtoField.uint32("rrp.from_port", "FromPort")
from = ProtoField.string("rrp.from", "From")
to_port = ProtoField.uint32("rrp.to_port", "ToPort")
to = ProtoField.string("rrp.to", "To")
call_id = ProtoField.string("rrp.call_id", "Call-ID")
session_id = ProtoField.string("rrp.session_id", "Session-ID")
sequence_no = ProtoField.uint32("rrp.sequence_no", "SequenceNo")
version = ProtoField.uint32("rrp.version", "Version")
destination_context = ProtoField.string("rrp.destination_context", "DestinationContext")
source_context = ProtoField.string("rrp.source_context", "SourceContext")
status = ProtoField.uint32("rrp.status", "StatusCode")
status_desc = ProtoField.string("rrp.status_desc", "StatusDescription")
route = ProtoField.string("rrp.route", "Route")
guid = ProtoField.string("rrp.guid", "GUID")
route_type = ProtoField.uint32("rrp.type", "Code")
route_type_method = ProtoField.string("rrp.type_method", "Method")
route_type_direction = ProtoField.string("rrp.type_direction", "Direction")
sms_originating_address = ProtoField.string("rrp.sms_originating_address", "SMSOriginatingAddress")
sms_originating_address_ton = ProtoField.uint32("rrp.sms_originating_address_ton", "SMSOriginatingAddressTON")
sms_originating_address_npi = ProtoField.uint32("rrp.sms_originating_address_npi", "SMSOriginatingAddressNPI")
sms_destination_address = ProtoField.string("rrp.sms_destination_address", "SMSDestinationAddress")
sms_destination_address_ton = ProtoField.uint32("rrp.sms_destination_address_ton", "SMSDestinationAddressTON")
sms_destination_address_npi = ProtoField.uint32("rrp.sms_destination_address_npi", "SMSDestinationAddressNPI")
sms_concat_message_id = ProtoField.string("rrp.sms_concat_message_id", "ConcatenatedMessageID")
sms_concat_parts = ProtoField.uint32("rrp.sms_concat_parts", "ConcatenatedParts")
sms_concat_part_number = ProtoField.uint32("rrp.sms_concat_part_number", "ConcatenatedPartNumber")
sms_text_dcs = ProtoField.uint32("rrp.sms_text_dcs", "SMSTextDCS")
sms_text = ProtoField.string("rrp.sms_text", "SMSText")
incoming_connection_type = ProtoField.uint32("rrp.incoming_connection_type", "IncomingConnectionType")
incoming_connection_type_method = ProtoField.string("rrp.incoming_connection_type_method", "IncomginConnectionTypeMethod")

route_route_no = ProtoField.uint32("rrp.route.route_no", "RouteNo")
route_src_trunk = ProtoField.string("rrp.route.src_trunk", "SrcTrunk")
route_dst_trunk = ProtoField.string("rrp.route.dst_trunk", "DstTrunk")
route_route_type = ProtoField.string("rrp.route.type", "DstSignallingType")
route_store_and_forward = ProtoField.string("rrp.route.store_and_forward", "StoreAndForward")
route_cgpa = ProtoField.string("rrp.route.cgpa", "CgPA")
route_cdpa = ProtoField.string("rrp.route.cdpa", "CdPA")
route_ssn = ProtoField.string("rrp.route.ssn", "SSN")
route_scoa = ProtoField.string("rrp.route.scoa", "SCOA")
route_scda = ProtoField.string("rrp.route.scda", "SCDA")
route_nnn = ProtoField.string("rrp.route.nnn", "NNN")
route_imsi = ProtoField.string("rrp.route.imsi", "IMSI")
route_msisdn = ProtoField.string("rrp.route.msisdn", "MSISDN")
route_map_version = ProtoField.string("rrp.route.map_version", "MAPVersion")
route_map_auto_fallback = ProtoField.string("rrp.route.map_auto_fallback", "MapAutoFallback")
route_error_remap_list = ProtoField.string("rrp.route.error_remap_list", "ErrorRemapList")
route_filter_first = ProtoField.string("rrp.route.filter_first", "FilterFirst")
route_temp_error_list = ProtoField.string("rrp.route.temp_error_list", "TempErrorList")
route_permanent_error_list = ProtoField.string("rrp.route.permanent_error_list", "PermanentErrorList")
route_do_hlr = ProtoField.string("rrp.route.do_hlr", "DoHLR")
route_timeout = ProtoField.string("rrp.route.timeout", "Timeout")
route_route_id = ProtoField.string("rrp.route.route_id", "RouteId")
route_route_hlrgt = ProtoField.string("rrp.route.hlrgt", "HLRGtAddress")
route_route_smpp_optional_supported = ProtoField.string("rrp.route.smpp_optional_supported", "SMPPOptionalParamsSupported")
route_customer_smpp_default_charset = ProtoField.uint32("rrp.route.customer_smpp_default_charset", "CustomerSMPPDefaultCharset")
route_vendor_smpp_default_charset = ProtoField.uint32("rrp.route.vendor_smpp_default_charset", "VendorSMPPDefaultCharset")


hub_internal_status = ProtoField.uint32("rrp.hub.internal_status", "HUBInternalStatus")
hub_latency = ProtoField.uint32("rrp.hub.latency", "HUBLatency") 
hub_latency_nsec = ProtoField.uint32("rrp.hub.latency_nsec", "HUBLatencyNanoseconds") 
hub_state_id = ProtoField.uint32("rrp.hub.state_id", "StateMachinePositionId")
hub_state = ProtoField.string("rrp.hub.state", "StateMachinePosition")
packet_data = ProtoField.string("rrp.packet_data", "PacketData") 

rate_rate_direction = ProtoField.uint32("rrp.rate.rate_direction", "RateDirection")
rate_timestamp = ProtoField.uint32("rrp.rate.timestamp", "EventTimestamp")
rate_timestamp_nsec = ProtoField.uint32("rrp.rate.nanoseconds", "EventNanoseconds")
rate_direction = ProtoField.uint32("rrp.rate.direction", "EventDirection") 
rate_rrp_method = ProtoField.string("rrp.rate.rrp_method", "RRPMethod")

src_trunk = ProtoField.string("rrp.src_trunk", "SrcTrunk")
dst_trunk = ProtoField.string("rrp.dst_trunk", "DstTrunk")
m3ua_opc = ProtoField.string("rrp.m3ua_opc", "M3UA-OPC")
m3ua_dpc = ProtoField.string("rrp.m3ua_dpc", "M3UA-DPC")
sccp_cgpa = ProtoField.string("rrp.sccp_cgpa", "CgPA")
sccp_cgpa_ssn = ProtoField.string("rrp.sccp_cgpa_ssn", "CgPA-SSN")
sccp_cgpa_ton = ProtoField.string("rrp.sccp_cgpa_ton", "CgPA-TON")
sccp_cgpa_npi = ProtoField.string("rrp.sccp_cgpa_npi", "CgPA-NPI")
sccp_cdpa = ProtoField.string("rrp.sccp_cdpa", "CdPA")
sccp_cdpa_ssn = ProtoField.string("rrp.sccp_cdpa_ssn", "CdPA-SSN")
sccp_cdpa_ton = ProtoField.string("rrp.sccp_cdpa_ton", "CdPA-TON")
sccp_cdpa_npi = ProtoField.string("rrp.sccp_cdpa_npi", "CdPA-NPI")
tcap_sid = ProtoField.string("rrp.tcap_sid", "TCAP-SourceID")
tcap_did = ProtoField.string("rrp.tcap_did", "TCAP-DestinationID")
tcap_oid = ProtoField.string("rrp.tcap_oid", "TCAP-OID")
gsm_map_nnn = ProtoField.string("rrp.gsm_map_nnn", "NNN")
gsm_map_imsi = ProtoField.string("rrp.gsm_map_imsi", "IMSI")
gsm_map_msisdn = ProtoField.string("rrp.gsm_map_msisdn", "MSISDN")
gsm_map_sca = ProtoField.string("rrp.gsm_map_sca", "SCA")
gsm_map_scoa = ProtoField.string("rrp.gsm_map_scoa", "SCOA")
gsm_map_scoa_ton = ProtoField.uint32("rrp.gsm_map_scoa_ton", "SCOA-TON")
gsm_map_scoa_npi = ProtoField.uint32("rrp.gsm_map_scoa_npi", "SCOA-NPI")
gsm_map_scda = ProtoField.string("rrp.gsm_map_scda", "SCDA")
gsm_map_scda_ton = ProtoField.uint32("rrp.gsm_map_scda_ton", "SCDA-TON")
gsm_map_scda_npi = ProtoField.uint32("rrp.gsm_map_scda_npi", "SCDA-NPI")
gsm_map_version = ProtoField.string("rrp.gsm_map_version", "MAPVersion")
smpp_username = ProtoField.string("rrp.smpp_username", "SMPP-Username") 
smpp_ip = ProtoField.string("rrp.smpp_ip_address", "SMPP-IPAddress")
smpp_port = ProtoField.uint32("rrp.smpp_port", "SMPP-Port")
smpp_message_id = ProtoField.string("rrp.smpp_message_id", "SMPP-MessageId") 
smpp_drm_code = ProtoField.uint32("rrp.smpp_drm_code", "SMPP-DRM-ErrorCode")
error_type = ProtoField.uint32("rrp.event_error_type", "EventErrorType") 
error_code = ProtoField.uint32("rrp.event_error_code", "EventErrorCode")

frag_table = {}
frag_processed = {}




rrp_proto.fields = { rrp_size, rrp_fragment, rrp_more_fragments, rrp_fragment_id, method, sequence_state, from_port, from, to_port, to, call_id, session_id, guid, sequence_no, version, destination_context, source_context, status, status_desc, src_trunk, dst_trunk, m3ua_opc, m3ua_dpc, sccp_cgpa, sccp_cgpa_ssn, sccp_cgpa_npi, sccp_cgpa_ton, sccp_cdpa, sccp_cdpa_ssn, sccp_cdpa_npi, sccp_cdpa_ton, tcap_sid, tcap_did, tcap_oid, gsm_map_nnn, gsm_map_imsi, gsm_map_msisdn, gsm_map_sca, gsm_map_scoa, gsm_map_scoa_ton, gsm_map_scoa_npi, gsm_map_scda, gsm_map_scda_ton, gsm_map_scda_npi, gsm_map_version, smpp_username, smpp_ip, smpp_port, smpp_message_id, smpp_drm_code, error_type, error_code, packet_data, route, route_type, route_type_method, route_type_direction, incoming_connection_type, incoming_connection_type_method, sms_originating_address, sms_originating_address_ton, sms_originating_address_npi, sms_destination_address, sms_destination_address_ton, sms_destination_address_npi, sms_concat_message_id, sms_concat_parts, sms_concat_part_number, sms_text_dcs, sms_text, route_route_no, route_src_trunk, route_dst_trunk, route_route_type, route_store_and_forward, route_cgpa, route_cdpa, route_ssn, route_scoa, route_scda, route_nnn, route_imsi, route_msisdn, route_map_version, route_map_auto_fallback, route_error_remap_list, route_filter_first, route_temp_error_list, route_permanent_error_list, route_do_hlr, route_timeout, route_route_id, route_route_hlrgt, route_route_smpp_optional_supported, route_customer_smpp_default_charset, route_vendor_smpp_default_charset, rate_rate_direction, rate_timestamp, rate_timestamp_nsec, rate_direction, rate_rrp_method, hub_internal_status, hub_latency, hub_latency_nsec, hub_state, hub_state_id }

function str2hex(str)
	hex = ""
	for c in str:gmatch"." do
		hex = hex .. tostring(string.format("%02x", string.byte(c)))
	end
	return hex
end

-- create a function to dissect it
function rrp_proto.dissector(buffer,pinfo,tree)
	local tmp_info_col = "RRP"
	local tmp_method = ""
	local tmp_seq_state = ""
	local tmp_to = ""
	local tmp_from = ""
	local rrptree = tree:add(rrp_proto,buffer(),"Release14 RRP Stream")
	local headertree = rrptree:add(rrp_proto,buffer(),"R14 RRP Stream Header")
	local datatree = rrptree:add(rrp_proto,buffer(),"R14 RRP Stream Data")
	local pos = 0
	local tmp_type_method = ""
	packet_data_present = 0
	rrp_method_present = 0
	sig_type = 0
	fragmented_data_present=0
	local buf_len = buffer:len()
	for line in buffer():string():gmatch("[^\r\n]+") do
		if (string.find(line, "Size") ~= nil) then
			tmp = string.find(line, "%:") + 1
			size_decoded = string.sub(line, tmp, string.len(line))
			if (string.find(size_decoded, ";") ~= nil) then
				local posS = pos
				local counterS = 0
				local first_line_start_offset = string.len(line)
				tmplineS = size_decoded .. ";"
				for routewordS in tmplineS:gmatch("([^;]*);") do
					if (counterS == 0) then
						headertree:add(rrp_size, buffer(posS, string.len(routewordS)), tonumber(routewordS))
					elseif (counterS == 1) then
						fragment_pos = routewordS
						headertree:add(rrp_fragment, buffer(posS, string.len(routewordS)), tonumber(routewordS))
					elseif (counterS == 2) then
						fragment_more_fragments = routewordS
						if (tonumber(routewordS) == 1) then
							more_fragments = 1
							headertree:add(rrp_more_fragments, buffer(posS, string.len(routewordS)), tonumber(routewordS))
						else
							more_fragments = 0
							headertree:add(rrp_more_fragments, buffer(posS, string.len(routewordS)), tonumber(routewordS))
						end
					elseif (counterS == 3) then
						fragment_guid = routewordS
						headertree:add(rrp_fragment_id, buffer(posS, string.len(routewordS)), routewordS)
						if (more_fragments == 1) then
							pinfo.desegment_len = DESEGMENT_ONE_MORE_SEGMENT
							if (frag_processed[routewordS] == nil) then
								if (frag_table[routewordS] ~= nil) then
									tmp_buf_frag = frag_table[routewordS] .. string.sub(buffer(first_line_start_offset, buf_len - first_line_start_offset):string(), 2)
									frag_table[routewordS] = tmp_buf_frag
								else

									tmp_buf_frag = buffer(first_line_start_offset, buf_len - first_line_start_offset):string()
									frag_table[routewordS] = tmp_buf_frag
								end
							end
							fragmented_info_col = tmp_info_col .. " FRAGMENT: " .. fragment_guid .. " SEQ: " .. fragment_pos .. " MORE: " .. fragment_more_fragments
							pinfo.cols.info = fragmented_info_col 
							pinfo.cols.protocol = "RRP"
							return DESEGMENT_ONE_MORE_SEGMENT
						else
							tmp_buf_reass = tostring(frag_table[routewordS]) .. string.sub(buffer(first_line_start_offset, buf_len - first_line_start_offset):string(), 2)
							fragmented_data_byte = ByteArray.new(str2hex(tmp_buf_reass))
							fragmented_data_tvb = ByteArray.tvb(fragmented_data_byte, "Reassembled RRP")
							fragmented_data_present = 1
							frag_processed[routewordS] = 1
							fragmented_info_col = " REASSEMBLED: " .. fragment_guid .. " " 
							--frag_table[routewordS] = nil

						end
					end
					counterS = counterS + 1
					posS = posS + string.len(routewordS) + 1
				end
			else
				headertree:add(rrp_size, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			end
			--headertree:add(buffer(pos, string.len(line)), line)
		elseif (string.find(line, "SequenceState") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(sequence_state, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			tmp_seq_state = string.sub(line, tmp, string.len(line))
		elseif (string.find(line, "RRPMethod") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(rate_rrp_method, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			rrp_method_present = 1
		elseif (string.find(line, "Method") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(method, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			tmp_method = string.sub(line, tmp, string.len(line))
		elseif (string.find(line, "FromPort") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(from_port, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "From") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(from, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "ToPort") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(to_port, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "To") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(to, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Call%-ID") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(call_id, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Session%-ID") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(session_id, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "GUID") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(guid, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SequenceNo") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(sequence_no, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Version") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(version, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "DestinationContext") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(destination_context, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			tmp_to = string.sub(line, tmp, string.len(line))
		elseif (string.find(line, "SourceContext") ~= nil) then
			tmp = string.find(line, "%:") + 1
			headertree:add(source_context, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			tmp_from = string.sub(line, tmp, string.len(line))
		elseif (string.find(line, "InternalStatus") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(hub_internal_status, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Status") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(status, buffer(pos, string.len(line)), string.sub(line, tmp, tmp + 3))
			datatree:add(status_desc, buffer(pos, string.len(line)), string.sub(line, tmp + 4, string.len(line)))
		elseif (string.find(line, "ConnectionType") ~= nil) then
			tmp = string.find(line, "%:") + 1
			if (string.sub(line, tmp, string.len(line)) == "2") then
				tmp_incoming_connection_type_method = "SS7 M3UA"
			elseif (string.sub(line, tmp, string.len(line)) == "5") then
				tmp_incoming_connection_type_method = "SMPP"
			else
				tmp_incoming_connection_type_method = "Unknown"
			end
			local connection_type_tree = datatree:add(rrp_proto,buffer(),"Incoming ConnectionType")
			connection_type_tree:add(incoming_connection_type, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			connection_type_tree:add(incoming_connection_type_method, buffer(pos, string.len(line)), tmp_incoming_connection_type_method)
		elseif (string.find(line, "ErrorType") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(error_type, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "DRMErrorCode") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(smpp_drm_code, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "ErrorCode") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(error_code, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Type") ~= nil) then
			tmp = string.find(line, "%:") + 1
			--datatree:add(route_type, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			if (string.sub(line, tmp, string.len(line)) == "100") then
				tmp_type_method = "return-result-last"
				tmp_type_direction = "IN"
				sig_type = 1
			elseif (string.sub(line, tmp, string.len(line)) == "10") then
				tmp_type_method = "mo-forward-sm"
				tmp_type_direction = "IN"
				sig_type = 1
			elseif (string.sub(line, tmp, string.len(line)) == "11") then
				tmp_type_method = "mt-forward-sm"
				tmp_type_direction = "IN"
				sig_type = 1
			elseif (string.sub(line, tmp, string.len(line)) == "20") then
				tmp_type_method = "submit_sm"
				tmp_type_direction = "IN"
				sig_type = 2
			elseif (string.sub(line, tmp, string.len(line)) == "21") then
				tmp_type_method = "deliver_sm"
				tmp_type_direction = "IN"
				sig_type = 2
			elseif (string.sub(line, tmp, string.len(line)) == "22") then
				tmp_type_method = "drm deliver_sm"
				tmp_type_direction = "IN"
				sig_type = 2
			elseif (string.sub(line, tmp, string.len(line)) == "30") then
				tmp_type_method = "sri-for-sm"
				tmp_type_direction = "IN"
				sig_type = 1
			elseif (string.sub(line, tmp, string.len(line)) == "40") then
				tmp_type_method = "mnp"
				tmp_type_direction = "IN"
			elseif (string.sub(line, tmp, string.len(line)) == "80") then
				tmp_type_method = "ADD"
				tmp_type_direction = "IN"
			elseif (string.sub(line, tmp, string.len(line)) == "81") then
				tmp_type_method = "DEL"
				tmp_type_direction = "IN"
			else
				tmp_type_method = "Unknown"
				tmp_type_direction = "IN"
			end
			--datatree:add(route_type_decoded, buffer(pos, string.len(line)), tmp_type_decoded_text)
			local route_type_tree = datatree:add(rrp_proto,buffer(),"Type")
			route_type_tree:add(route_type, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			route_type_tree:add(route_type_method, buffer(pos, string.len(line)), tmp_type_method)
--			route_type_tree:add(route_type_direction, buffer(pos, string.len(line)), tmp_type_direction)
		elseif (string.find(line, "SMSOriginatingAddressTON") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_originating_address_ton, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			--datatree:add(sms_originating_address_ton, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SMSOriginatingAddressNPI") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_originating_address_npi, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SMSOriginatingAddress") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_originating_address, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SMSDestinationAddressTON") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_destination_address_ton, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SMSDestinationAddressNPI") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_destination_address_npi, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SMSDestinationAddress") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_destination_address, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "ConcatMessageId") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_concat_message_id, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Parts") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_concat_parts, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "PartNumber") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_concat_part_number, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SMSTextDCS") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_text_dcs, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SMSText") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sms_text, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "RateDirection") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(rate_rate_direction, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Direction") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(rate_direction, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "TimestampNsec") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(rate_timestamp_nsec, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Timestamp") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(rate_timestamp, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SrcTrunk") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(src_trunk, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "DstTrunk") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(dst_trunk, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "OPC") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(m3ua_opc, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "DPC") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(m3ua_dpc, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "CgSSN") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sccp_cgpa_ssn, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "CgNPI") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sccp_cgpa_npi, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "CgTON") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sccp_cgpa_ton, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "CgPA") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sccp_cgpa, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "CdSSN") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sccp_cdpa_ssn, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "CdNPI") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sccp_cdpa_npi, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "CdTON") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sccp_cdpa_ton, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "CdPA") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(sccp_cdpa, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "TCAPSId") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(tcap_sid, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "TCAPDId") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(tcap_did, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "TCAPOId") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(tcap_oid, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "NNN") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_nnn, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "IMSI") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_imsi, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "MSISDN") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_msisdn, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SCOATON") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_scoa_ton, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SCOANPI") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_scoa_npi, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SCOA") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_scoa, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SCDATON") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_scda_ton, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SCDANPI") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_scda_npi, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SCDA") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_scda, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "SCA") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_sca, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "MAPV") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(gsm_map_version, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Username") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(smpp_username, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "IPAddress") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(smpp_ip, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Port") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(smpp_port, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "MessageId") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(smpp_message_id, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "LatencyNsec") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(hub_latency_nsec, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "Latency") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(hub_latency, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
		elseif (string.find(line, "State") ~= nil) then
			tmp = string.find(line, "%:") + 1
			if (string.sub(line, tmp, string.len(line)) == "2") then
				tmp_hub_state = "CPT_SRI_SM_REQ"
			elseif (string.sub(line, tmp, string.len(line)) == "3") then
				tmp_hub_state = "CPT_SS7_SMS_FWD"
			elseif (string.sub(line, tmp, string.len(line)) == "4") then
				tmp_hub_state = "CPT_NOTIFICATION_RCVD"
			elseif (string.sub(line, tmp, string.len(line)) == "5") then
				tmp_hub_state = "CPT_NOTIFICATION_WAIT_SMS"
			elseif (string.sub(line, tmp, string.len(line)) == "6") then
				tmp_hub_state = "CPT_NOTIFICATION_WAIT_REPORT"
			elseif (string.sub(line, tmp, string.len(line)) == "7") then
				tmp_hub_state = "CPT_NOTIFICATION_WAIT_REPORT_FINAL"
			elseif (string.sub(line, tmp, string.len(line)) == "8") then
				tmp_hub_state = "CPT_NOTIFICATION_WAIT_SRI"
			elseif (string.sub(line, tmp, string.len(line)) == "9") then
				tmp_hub_state = "CPT_SMPP_SMS_ACK_WAIT"
			elseif (string.sub(line, tmp, string.len(line)) == "10") then
				tmp_hub_state = "CPT_SMPP_ACK_RCVD"
			elseif (string.sub(line, tmp, string.len(line)) == "11") then
				tmp_hub_state = "CPT_ACK_SENT"
			elseif (string.sub(line, tmp, string.len(line)) == "12") then
				tmp_hub_state = "CPT_SMPP_SMS_FWD"
			elseif (string.sub(line, tmp, string.len(line)) == "13") then
				tmp_hub_state = "CPT_SMPP_DLVR_RCPT_FWD"
			elseif (string.sub(line, tmp, string.len(line)) == "14") then
				tmp_hub_state = "CPT_SMPP_SRI_SKIP"
			elseif (string.sub(line, tmp, string.len(line)) == "15") then
				tmp_hub_state = "CPT_SMPP_DLVR_RCPT_ACK_WAIT"
			elseif (string.sub(line, tmp, string.len(line)) == "16") then
				tmp_hub_state = "CPT_TCAP_BEGIN_ONLY"
			elseif (string.sub(line, tmp, string.len(line)) == "17") then
				tmp_hub_state = "CPT_NOTIFICATION_WAIT_FINAL_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9000") then
				tmp_hub_state = "CPT_RRP_DRM_STORE_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9001") then
				tmp_hub_state = "CPT_RRP_DRM_DELETE_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9002") then
				tmp_hub_state = "CPT_RRP_DRM_ROUTE_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9003") then
				tmp_hub_state = "CPT_RRP_SMPP_SRI_ROUTE_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9004") then
				tmp_hub_state = "CPT_RRP_SS7_SRI_ROUTE_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9005") then
				tmp_hub_state = "CPT_RRP_SMPP_SMS_ROUTE_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9006") then
				tmp_hub_state = "CPT_RRP_SS7_SMS_ROUTE_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9007") then
				tmp_hub_state = "CPT_RRP_WINDOW_ADD_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9008") then
				tmp_hub_state = "CPT_RRP_WINDOW_DEL_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9009") then
				tmp_hub_state = "CPT_RRP_RATE_SRI_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9010") then
				tmp_hub_state = "CPT_RRP_RATE_SS7_SRI_ROUTE_START_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9011") then
				tmp_hub_state = "CPT_RRP_RATE_SS7_SRI_ROUTE_END_WAIT_ACK"
			elseif (string.sub(line, tmp, string.len(line)) == "9012") then
				tmp_hub_state = "CPT_RRP_RATE_SS7_SRI_ACK_WAIT_ACK"
			else 
				tmp_hub_state = "UNKNOWN"
			end
			--datatree:add(hub_state, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			local hub_state_tree = datatree:add(rrp_proto,buffer(),"Current SMS HUB State")
			hub_state_tree:add(hub_state_id, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			hub_state_tree:add(hub_state, buffer(pos, string.len(line)), tmp_hub_state)
		elseif (string.find(line, "PacketData") ~= nil) then
			tmp = string.find(line, "%:") + 1
			datatree:add(packet_data, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			packet_data_byte = ByteArray.new(string.sub(line, tmp, string.len(line)))
			packet_data_tvb = ByteArray.tvb(packet_data_byte, "Packet Data TVB")
			packet_data_tree = datatree:add(rrp_proto,buffer(),"R14 RRP Packet Data Decoded")
			packet_data_present = 1
			--	Dissector.get("m3ua"):call(packet_data_tvb, pinfo, m3ua_tree)
		else
			if (string.find(line, "Route") ~= nil) then
				tmp = string.find(line, "%:") + 1
				datatree:add(route, buffer(pos, string.len(line)), string.sub(line, tmp, string.len(line)))
			else
				datatree:add(buffer(pos, string.len(line)), line)
			end
			if (string.find(line, "Route") ~= nil) then
				local route = datatree:add(rrp_proto,buffer(),"R14 RRP Route Data Decoded")
				--local route = tree:add(rrp_proto,buffer(),"R14 RRP Route Data Decoded")
				local pos1 = pos
				local counter = 0
				tmpline = line .. ";"
				for routeword in tmpline:gmatch("([^;]*);") do
					if (counter == 0) then
						route:add(route_route_no, buffer(pos1 + 6, string.len(routeword) - 6), string.sub(routeword,7,string.len(routeword)))
						tmp_text = "Route --" .. string.sub(routeword,7,string.len(routeword)) .. "-- Decoded"
						route:set_text(tmp_text)
					elseif (counter == 1) then
						route:add(route_src_trunk,buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 2) then
						route:add(route_dst_trunk, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 3) then
						route:add(route_route_type, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 4) then
						route:add(route_store_and_forward, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 5) then
						route:add(route_cgpa, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 6) then
						route:add(route_cdpa, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 7) then
						route:add(route_ssn, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 8) then
						route:add(route_scoa, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 9) then
						route:add(route_scda, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 10) then
						route:add(route_nnn, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 11) then
						route:add(route_imsi, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 12) then
						route:add(route_msisdn, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 13) then
						route:add(route_map_version, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 14) then
						route:add(route_map_auto_fallback, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 15) then
						route:add(route_error_remap_list, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 16) then
						route:add(route_filter_first, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 17) then
						route:add(route_temp_error_list, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 18) then
						route:add(route_permanent_error_list, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 19) then
						route:add(route_do_hlr, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 20) then
						route:add(route_timeout, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 21) then
						route:add(route_route_id, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 22) then
						route:add(route_route_hlrgt, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 23) then
						route:add(route_route_smpp_optional_supported, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 24) then
						route:add(route_customer_smpp_default_charset, buffer(pos1, string.len(routeword)), routeword)
					elseif (counter == 25) then
						route:add(route_vendor_smpp_default_charset, buffer(pos1, string.len(routeword)), routeword)
					end
					counter = counter + 1
					pos1 = pos1 + string.len(routeword) + 1
				end
			end
		end
		pos = pos + string.len(line) + 1
	end
	prepend_info = 0
	if (packet_data_present == 1) then
		prepend_info = 1
		if (rrp_method_present == 0) then
			if (sig_type == 1) then
				Dissector.get("m3ua"):call(packet_data_tvb, pinfo, packet_data_tree)
			elseif (sig_type == 2) then
				Dissector.get("smpp"):call(packet_data_tvb, pinfo, packet_data_tree)
			end
		else
			Dissector.get("rrp"):call(packet_data_tvb, pinfo, packet_data_tree)
		end
	end
	if (fragmented_data_present == 1) then
		fragmented_tree_name = "Reassembled RRP Packet --" .. fragment_guid.. "--"
		--fragmented_data_tree = datatree:add(rrp_proto,buffer(), fragmented_tree_name)
		fragmented_data_tree = tree:add(rrp_proto,buffer(), fragmented_tree_name)
		Dissector.get("rrp"):call(fragmented_data_tvb, pinfo, fragmented_data_tree)
		pinfo.cols.info:append(fragmented_info_col)
		pinfo.cols.protocol = "RRP"
	else
		if (prepend_info == 1) then
			infokk = tmp_info_col .. " " .. tmp_method .. " " .. tmp_seq_state .. " " .. tmp_type_method .. " | PacketData: "
			pinfo.cols.info:prepend(infokk)
		else
			infokk = tmp_info_col .. " " .. tmp_method .. " " .. tmp_seq_state .. " " .. tmp_type_method .. " "
			pinfo.cols.info = infokk
		end
		pinfo.dst = Address.string(tmp_to)
		pinfo.src = Address.string(tmp_from)
		pinfo.cols.protocol = "RRP"
	end

	return buf_len
	
end
-- load the udp.port table
udp_table = DissectorTable.get("udp.port")
-- register our protocol to handle udp port 33000,33005
udp_table:add(33000,rrp_proto)
udp_table:add(33001,rrp_proto)
udp_table:add(33002,rrp_proto)
udp_table:add(33003,rrp_proto)
udp_table:add(33004,rrp_proto)
udp_table:add(33005,rrp_proto)
udp_table:add(33006,rrp_proto)

--Size:313
--SequenceState:BEGIN
--Method:ROUTE
--From:172.16.16.99
--FromPort:33005
--To:172.16.16.99
--ToPort:33000
--Call-ID:33ef162e5d08486b8dbd97b43087bf49
--Session-ID:0d61cd9ece9c4e09924d579589ccc3d0
--SequenceNo:1
--Version:1
--SrcTrunk:MBLX_C_01
--Type:11
--MSISDN:4916099999
--MAPV:2
--CdPA:882280982
--CgPA:882469994901
--SCA:882469994901
--DestinationContext:rt01
--SourceContext:smshub01


--[0] 0.0.0 (RouteNo) = 1
--[0] 1.0.0 (SrcTrunk) = TYNTEC
--[0] 2.0.0 (DstTrunk) = TYNTEC
--[0] 3.0.0 (Type) = 30
--[0] 4.0.0 (StoreAndForward) = 0
--[0] 5.0.0 (CgPA) = 882280982
--[0] 6.0.0 (CdPA) = 882469994901
--[0] 7.0.0 (SSN) = 6
--[0] 8.0.0 (SCOA) = 
--[0] 9.0.0 (SCDA) = 
--[0] 10.0.0 (NNN) = 882280982
--[0] 11.0.0 (IMSI) = 262010000000001
--[0] 12.0.0 (MSISDN) = 491757214122
--[0] 13.0.0 (MAPVersion) = 2
--[0] 14.0.0 (MapAutoFallback) = 0
--[0] 15.0.0 (ErrorRemapList) = 1
--[0] 16.0.0 (FilterFirst) = 0
--[0] 17.0.0 (TempErrorList) = 1
--[0] 18.0.0 (PermanentErrorList) = 1
--[0] 19.0.0 (DoHLR) = 0
--[0] 20.0.0 (Timeout) = 300
--[0] 21.0.0 (RouteId) = 0

--http://wiki.wireshark.org/LuaAPI
