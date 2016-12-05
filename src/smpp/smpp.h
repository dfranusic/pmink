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

/**
 * Short Message Peer to Peer Protocol Specification v3.4
 */

#ifndef SMPP_H_
#define SMPP_H_

#include <vector>
#include <map>
#include <stdint.h>
#include <time.h>

namespace smpp {

	// smpp null string
	extern const char* SMPP_NULL_STR;

	/**
	 * The command_id field identifies the particular
	 * SMPP PDU, e.g., submit_sm, query_sm, etc.
	 *
	 * A unique command identifier is allocated to
	 * each SMPP request PDU in the range:
	 * 0x00000000 to 0x000001FF
	 * A unique command identifier is also allocated
	 * to each SMPP response PDU in the range:
	 * 0x80000000 to 0x800001FF
	 *
	 * (Note that an SMPP response command_id is
	 * identical to the corresponding request SMPP
	 * command_id, but with bit 31 set).
	 * Refer to chapter 5. for details of the complete
	 * SMPP Command ID set.
	 */
	enum SmppCommandId {
		GENERIC_NACK 			= 0x80000000,	/**< 4.3 GENERIC_NACK PDU */
		BIND_RECEIVER 			= 0x00000001,	/**< 4.1.3  BIND Operation - BIND_RECEIVER PDU */
		BIND_RECEIVER_RESP 		= 0x80000001,	/**< 4.1.4  BIND Operation - BIND_RECEIVER_RESP PDU */
		BIND_TRANSMITTER 		= 0x00000002,	/**< 4.1.1  BIND Operation - BIND_TRANSMITTER PDU */
		BIND_TRANSMITTER_RESP 		= 0x80000002,	/**< 4.1.2  BIND Operation - BIND_TRANSMITTER_RESP PDU */
		QUERY_SM 			= 0x00000003,	/**< 4.8.1  QUERY_SM Operation - QUERY_SM PDU */
		QUERY_SM_RESP 			= 0x80000003,	/**< 4.8.2  QUERY_SM Operation - QUERY_SM_RESP PDU */
		SUBMIT_SM 			= 0x00000004,	/**< 4.4.1  SUBMIT_SM Operation - SUBMIT_SM PDU */
		SUBMIT_SM_RESP 			= 0x80000004,	/**< 4.4.2  SUBMIT_SM Operation - SUBMIT_SM_RESP PDU */
		DELIVER_SM 			= 0x00000005,	/**< 4.6.1  DELIVER_SM Operation - DELIVER_SM PDU */
		DELIVER_SM_RESP 		= 0x80000005,	/**< 4.6.2  DELIVER_SM Operation - DELIVER_SM_RESP PDU */
		UNBIND 				= 0x00000006,	/**< 4.2.1  UNBIND Operation - UNBIND PDU */
		UNBIND_RESP 			= 0x80000006,	/**< 4.2.2  UNBIND Operation - UNBIND_RESP PDU */
		REPLACE_SM 			= 0x00000007,	/**< 4.10.1  REPLACE_SM Operation - REPLACE_SM PDU */
		REPLACE_SM_RESP 		= 0x80000007,	/**< 4.10.2  REPLACE_SM Operation - REPLACE_SM_RESP PDU */
		CANCEL_SM 			= 0x00000008,	/**< 4.9.1  CANCEL_SM Operation - CANCEL_SM PDU */
		CANCEL_SM_RESP 			= 0x80000008,	/**< 4.9.2  CANCEL_SM Operation - CANCEL_SM_RESP PDU */
		BIND_TRANSCEIVER 		= 0x00000009,	/**< 4.1.5  BIND Operation - BIND_TRANSCEIVER PDU */
		BIND_TRANSCEIVER_RESP 		= 0x80000009,	/**< 4.1.6  BIND Operation - BIND_TRANSCEIVER_RESP PDU */
		OUTBIND 			= 0x0000000B,	/**< 4.1.7.1  OUTBIND Operation - OUTBIND PDU */
		ENQUIRE_LINK 			= 0x00000015,	/**< 4.11.1  ENQUIRE_LINK Operation - ENQUIRE_LINK PDU */
		ENQUIRE_LINK_RESP 		= 0x80000015,	/**< 4.11.2  ENQUIRE_LINK Operation - ENQUIRE_LINK_RESP PDU */
		SUBMIT_MULTI 			= 0x00000021,	/**< 4.5.1  SUBMIT_MULTI Operation - SUBMIT_MULTI PDU */
		SUBMIT_MULTI_RESP 		= 0x80000021,	/**< 4.5.2  SUBMIT_MULTI Operation - SUBMIT_MULTI_RESP PDU */
		ALERT_NOTIFICATION 		= 0x00000102,	/**< 4.12.1  ALERT_NOTIFICATION Operation - ALERT_NOTIFICATION PDU */
		DATA_SM 			= 0x00000103,	/**< 4.7.1  DATA_SM Operation - DATA_SM PDU */
		DATA_SM_RESP 			= 0x80000103,	/**< 4.7.2  DATA_SM Operation - DATA_SM_RESP PDU */


	};
	/**
	 * 5.1.3 command_status
	 *
	 * The command_status field of an SMPP message response indicates the success or failure of an
	 * SMPP request. It is relevant only in the SMPP response message and should be set to NULL in
	 * SMPP request messages.
	 *
	 * The SMPP Error status codes are returned by the SMSC in the command_status field of the
	 * SMPP message header and in the error_status_code field of a submit_multi_resp message.
	 * The complete set of SMPP Error Codes and their associated values are defined in the following
	 * table.
	 */
	enum SmppCommandStatus {
		ESME_ROK 			= 0x00000000,	/**< No Error */
		ESME_RINVMSGLEN 		= 0x00000001,	/**< Message Length is invalid */
		ESME_RINVCMDLEN 		= 0x00000002,	/**< Command Length is invalid */
		ESME_RINVCMDID 			= 0x00000003,	/**< Invalid Command ID */
		ESME_RINVBNDSTS 		= 0x00000004,	/**< Incorrect BIND Status for given com-mand */
		ESME_RALYBND 			= 0x00000005,	/**< ESME Already in Bound State */
		ESME_RINVPRTFLG 		= 0x00000006,	/**< Invalid Priority Flag */
		ESME_RINVREGDLVFLG 		= 0x00000007,	/**< Invalid Registered Delivery Flag */
		ESME_RSYSERR 			= 0x00000008,	/**< System Error */
		ESME_RINVSRCADR 		= 0x0000000A,	/**< Invalid Source Address */
		ESME_RINVDSTADR 		= 0x0000000B,	/**< Invalid Dest Addr */
		ESME_RINVMSGID 			= 0x0000000C,	/**< Message ID is invalid */
		ESME_RBINDFAIL 			= 0x0000000D,	/**< Bind Failed */
		ESME_RINVPASWD 			= 0x0000000E,	/**< Invalid Password */
		ESME_RINVSYSID 			= 0x0000000F,	/**< Invalid System ID */
		ESME_RCANCELFAIL 		= 0x00000011,	/**< Cancel SM Failed */
		ESME_RREPLACEFAIL 		= 0x00000013,	/**< Replace SM Failed */
		ESME_RMSGQFUL 			= 0x00000014,	/**< Message Queue Full */
		ESME_RINVSERTYP 		= 0x00000015,	/**< Invalid Service Type */
		ESME_RINVNUMDESTS 		= 0x00000033,	/**< Invalid number of destinations */
		ESME_RINVDLNAME 		= 0x00000034,	/**< Invalid Distribution List name */
		ESME_RINVDESTFLAG 		= 0x00000040,	/**< Destination flag is invalid (submit_multi) */
		ESME_RINVSUBREP 		= 0x00000042,	/**< Invalid ‘submit with replace’ request (i.e. submit_sm with replace_if_present_flag set) */
		ESME_RINVESMCLASS 		= 0x00000043,	/**< Invalid esm_class field data */
		ESME_RCNTSUBDL 			= 0x00000044,	/**< Cannot Submit to Distribution List */
		ESME_RSUBMITFAIL 		= 0x00000045,	/**< submit_sm or submit_multi failed */
		ESME_RINVSRCTON 		= 0x00000048,	/**< Invalid Source address TON */
		ESME_RINVSRCNPI 		= 0x00000049,	/**< Invalid Source address NPI */
		ESME_RINVDSTTON 		= 0x00000050,	/**< Invalid Destination address TON */
		ESME_RINVDSTNPI 		= 0x00000051,	/**< Invalid Destination address NPI */
		ESME_RINVSYSTYP 		= 0x00000053,	/**< Invalid system_type field */
		ESME_RINVREPFLAG 		= 0x00000054,	/**< Invalid replace_if_present flag */
		ESME_RINVNUMMSGS 		= 0x00000055,	/**< Invalid number of messages */
		ESME_RTHROTTLED 		= 0x00000058,	/**< Throttling error (ESME has exceeded allowed message limits) */
		ESME_RINVSCHED 			= 0x00000061,	/**< Invalid Scheduled Delivery Time */
		ESME_RINVEXPIRY 		= 0x00000062,	/**< Invalid message validity period (Expiry time) */
		ESME_RINVDFTMSGID 		= 0x00000063,	/**< Predefined Message Invalid or Not Found */
		ESME_RX_T_APPN 			= 0x00000064,	/**< ESME Receiver Temporary App Error Code */
		ESME_RX_P_APPN 			= 0x00000065,	/**< ESME Receiver Permanent App Error Code */
		ESME_RX_R_APPN 			= 0x00000066,	/**< ESME Receiver Reject Message Error Code */
		ESME_RQUERYFAIL 		= 0x00000067,	/**< query_sm request failed */
		ESME_RINVOPTPARSTREAM 		= 0x000000C0,	/**< Error in the optional part of the PDU Body */
		ESME_ROPTPARNOTALLWD 		= 0x000000C1,	/**< Optional Parameter not allowed */
		ESME_RINVPARLEN 		= 0x000000C2,	/**< Invalid Parameter Length */
		ESME_RMISSINGOPTPARAM 		= 0x000000C3,	/**< Expected Optional Parameter missing */
		ESME_RINVOPTPARAMVAL 		= 0x000000C4,	/**< Invalid Optional Parameter Value */
		ESME_RDELIVERYFAILURE 		= 0x000000FE,	/**< Delivery Failure (used for data_sm_resp) */
		ESME_RUNKNOWNERR 		= 0x000000FF	/**< Unknown Error */
	};

	/**
	 * 5.3.2 SMPP Optional Parameter Tag definitions
	 *
	 * The SMPP supported Optional Parameters and their associated Tag Values are listed in Table
	 * 5-7 below. The optional parameters are described individually in the following sections.
	 *
	 * Generic optional parameters may be applicable to all wireless network technologies i.e., GSM/
	 * iDEN, TDMA and CDMA.
	 */
	enum SmppParameterType {
		SMPP_PARAM_UNKNOWN		= 0x0000,	/**< Unknown */
		DEST_ADDR_SUBUNIT 		= 0x0005,	/**< GSM */
		DEST_NETWORK_TYPE 		= 0x0006,	/**< Generic */
		DEST_BEARER_TYPE 		= 0x0007,	/**< Generic */
		DEST_TELEMATICS_ID 		= 0x0008,	/**< GSM */
		SOURCE_ADDR_SUBUNIT 		= 0x000d,	/**< GSM */
		SOURCE_NETWORK_TYPE 		= 0x000e,	/**< Generic */
		SOURCE_BEARER_TYPE		= 0x000f,	/**< Generic */
		SOURCE_TELEMATICS_ID 		= 0x0010,	/**< GSM */
		QOS_TIME_TO_LIVE 		= 0x0017,	/**< Generic */
		PAYLOAD_TYPE 			= 0x0019,	/**< Generic */
		ADDITIONAL_STATUS_INFO_TEXT 	= 0x001d,	/**< Generic */
		RECEIPTED_MESSAGE_ID 		= 0x001e,	/**< Generic */
		MS_MSG_WAIT_FACILITIES 		= 0x0030,	/**< GSM */
		PRIVACY_INDICATOR 		= 0x0201,	/**< CDMA, TDMA */
		SOURCE_SUBADDRESS 		= 0x0202,	/**< CDMA, TDMA */
		DEST_SUBADDRESS 		= 0x0203,	/**< CDMA, TDMA */
		USER_MESSAGE_REFERENCE 		= 0x0204,	/**< Generic */
		USER_RESPONSE_CODE 		= 0x0205,	/**< CDMA, TDMA */
		SOURCE_PORT 			= 0x020a,	/**< Generic */
		DESTINATION_PORT 		= 0x020b,	/**< Generic */
		SAR_MSG_REF_NUM 		= 0x020c,	/**< Generic */
		LANGUAGE_INDICATOR 		= 0x020d,	/**< CDMA, TDMA */
		SAR_TOTAL_SEGMENTS 		= 0x020e,	/**< Generic */
		SAR_SEGMENT_SEQNUM 		= 0x020f,	/**< Generic */
		SC_INTERFACE_VERSION 		= 0x0210,	/**< Generic */
		CALLBACK_NUM_PRES_IND 		= 0x0302,	/**< TDMA */
		CALLBACK_NUM_ATAG 		= 0x0303,	/**< TDMA */
		NUMBER_OF_MESSAGES 		= 0x0304,	/**< TDMA */
		CALLBACK_NUM 			= 0x0381,	/**< CDMA, TDMA, GSM, iDEN */
		DPF_RESULT 			= 0x0420,	/**< Generic */
		SET_DPF 			= 0x0421,	/**< Generic */
		MS_AVAILABILITY_STATUS 		= 0x0422,	/**< Generic */
		NETWORK_ERROR_CODE 		= 0x0423,	/**< Generic */
		MESSAGE_PAYLOAD 		= 0x0424,	/**< Generic */
		DELIVERY_FAILURE_REASON 	= 0x0425,	/**< Generic */
		MORE_MESSAGES_TO_SEND 		= 0x0426,	/**< GSM */
		MESSAGE_STATE 			= 0x0427,	/**< Generic */
		USSD_SERVICE_OP 		= 0x0501,	/**< GSM (USSD) */
		DISPLAY_TIME 			= 0x1201,	/**< CDMA, TDMA */
		SMS_SIGNAL 			= 0x1203,	/**< TDMA */
		MS_VALIDITY 			= 0x1204,	/**< CDMA, TDMA */
		ALERT_ON_MESSAGE_DELIVERY 	= 0x130c,	/**< CDMA */
		ITS_REPLY_TYPE 			= 0x1380,	/**< CDMA */
		ITS_SESSION_INFO 		= 0x1383	/**< CDMA */
	};

	/**
	 * 5.2.5 addr_ton, source_addr_ton, dest_addr_ton, esme_addr_ton
	 *
	 * These fields define the Type of Number (TON) to be used in the SME address parameters. The
	 * following TON values are defined:
	 *
	 */
	enum SmppTON {
		TON_UNKNOWN 		= 0,	/**< Unknown */
		TON_INTERNATIONAL 	= 1,	/**< International */
		TON_NATIONAL 		= 2,	/**< National */
		TON_NETWORK_SPECIFIC 	= 3,	/**< Network Specific */
		TON_SUBSCRIBER_NUMBER 	= 4,	/**< Subscriber Number */
		TON_ALPHANUMERIC 	= 5,	/**< Alphanumeric */
		TON_ABBREVIATED 	= 6	/**< Abbreviated */

	};

	/**
	 * 5.2.6 addr_npi, source_addr_npi, dest_addr_npi, esme_addr_npi
	 *
	 * These fields define the Numeric Plan Indicator (NPI) to be used in the SME address parameters.
	 * The following NPI values are defined:
	 *
	 */
	enum SmppNPI {
		UNKNOWN 	= 0x00,	/**< Unknown */
		ISDN_TELEPHONE 	= 0x01,	/**< ISDN (E163/E164) */
		DATA_X121 	= 0x03,	/**< Data (X.121) */
		TELEX 		= 0x04,	/**< Telex (F.69) */
		LAND_MOBILE 	= 0x06,	/**< Land Mobile (E.212) */
		NATIONAL 	= 0x08,	/**< National */
		PRIVATE 	= 0x09,	/**< Private */
		ERMES 		= 0x0A,	/**< ERMES */
		INTERNET_IP 	= 0x0E,	/**< Internet (IP) */
		WAP_CLIENT_ID	= 0x12	/**< WAP Client Id (to be defined by WAP Forum) */


	};

	/**
	 * 5.2.19 data_coding
	 *
	 */
	enum Data_codingType {
		DC_DEFAULT 		= 0x00,	/**< SMSC Default Alphabet */
		DC_IA5_ASCII 		= 0x01,	/**< IA5 (CCITT T.50)/ASCII (ANSI X3.4) */
		DC_8BIT_BINARY_1 	= 0x02,	/**< Octet unspecified (8-bit binary) */
		DC_ISO_8859_1 		= 0x03,	/**< Latin 1 (ISO-8859-1) */
		DC_8BIT_BINARY_2 	= 0x04,	/**< Octet unspecified (8-bit binary) */
		DC_JIS 			= 0x05,	/**< JIS (X 0208-1990) */
		DC_ISO_8859_5 		= 0x06,	/**< Cyrllic (ISO-8859-5) */
		DC_ISO_8859_8 		= 0x07,	/**< Latin/Hebrew (ISO-8859-8) */
		DC_UCS2 		= 0x08,	/**< UCS2 (ISO/IEC-10646) */
		DC_PICTOGRAM 		= 0x09,	/**< Pictogram Encoding */
		DC_ISO_2011_JP 		= 0x0a,	/**< ISO-2022-JP (Music Codes) */
		DC_EXTENDED_KANJI 	= 0x0d,	/**< Extended Kanji JIS(X 0212-1990) */
		DC_KS_C_5601 		= 0x0e	/**< KS C 5601 */
	};


	/**
	 * 5.2.12 esm_class
	 *
	 * GSM Network Specific Features (bits 7-6)
	 *
	 */
	enum GSMNetworkSpecific {
		GNS_NO_SPECIFIC_FEATURES 	= 0x00,	/**< No specific features selected */
		GNS_UDHI_INDICATOR 		= 0x40,	/**< UDHI Indicator (only relevant for MT short messages) */
		GNS_SET_REPLY_PATH 		= 0x80,	/**< Set Reply Path (only relevant for GSM network) */
		GNS_SET_UDHI_AND_REPLY_PATH 	= 0xc0	/**< Set UDHI and Reply Path (only relevant for GSM network) */

	};

	/**
	 * 5.2.17 registered_delivery
	 *
	 * Intermediate Notification (bit 5)
	 *
	 */
	enum IntermediateNotificationType {
		INT_NO 	= 0x00,	/**< No Intermediate notification requested (default) */
		INT_YES = 0x10	/**< Intermediate notification requested */
	};

	/**
	 * 5.2.12 esm_class
	 *
	 * Messaging Mode (bits 1-0)
	 *
	 */
	enum MessageMode {
		MM_DEFAULT_SMSC_MODE 	= 0x00,	/**< Default SMSC Mode (e.g. Store and Forward) */
		MM_DATAGRAM_MODE 	= 0x01,	/**< Datagram mode */
		MM_FORWARD_MODE 	= 0x02,	/**< Forward (i.e. Transaction) mode */
		MM_STORE_FORWARD_MODE 	= 0x03	/**< Store and Forward mode (use to select Store and Forward mode if Default SMSC Mode is non Store and Forward) */

	};

	/**
	 * 5.2.28 message_state
	 *
	 * The following is a list of allowable states for a short message. The message_state value is
	 * returned by the SMSC to the ESME as part of the query_sm_resp PDU.
	 *
	 */
	enum MessageStateType {
		MST_ENROUTE 		= 1,	/**< The message is in enroute state. */
		MST_DELIVERED 		= 2,	/**< Message is delivered to destination */
		MST_EXPIRED 		= 3,	/**< Message validity period has expired. */
		MST_DELETED 		= 4,	/**< Message has been deleted. */
		MST_UNDELIVERABLE 	= 5,	/**< Message is undeliverable */
		MST_ACCEPTED 		= 6,	/**< Message is in accepted state (i.e. has been manually read on behalf of the subscriber by customer service) */
		MST_UNKNOWN 		= 7,	/**< Message is in invalid state */
		MST_REJECTED 		= 8	/**< Message is in a rejected state */
	};


	/**
	 * 5.2.12 esm_class
	 *
	 * Message Type (bits 5-2)
	 *
	 */
	enum MessageType {
		MT_DEFAULT 			= 0x00,	/**< Default message Type (i.e. normal message) */
		MT_SMSC_DELIVERY_RECEIPT	= 0x04, /**< Short Message contains SMSC Delivery Receipt */
		MT_DELIVERY_ACK			= 0x08,	/**< Short Message contains ESME Delivery Acknowledgement */
		MT_MANUAL_USER_ACK 		= 0x10,	/**< Short Message contains ESME Manual/User Acknowledgement */
		MT_CNVRS_ABORT			= 0x18, /**< Short Message contains Conversation Abort (Korean CDMA) */
		MT_INTRM_DLVR_NTF		= 0x20 /**< Short Message contains Intermediate Delivery Notification */

	};

	/**
	 * 5.2.17 registered_delivery
	 *
	 * SME originated Acknowledgement (bits 3 and 2)
	 *
	 */
	enum SMEOrigAckType {
		SOA_NO_SME_ACK 			= 0x00,	/**< No recipient SME acknowledgment requested (default) */
		SOA_SME_ACK 			= 0x04,	/**< SME Delivery Acknowledgement requested */
		SOA_SME_MANUAL_USER_ACK 	= 0x08,	/**< SME Manual/User Acknowledgment requested */
		SOA_SME_BOTH 			= 0x0c	/**< Both Delivery and Manual/User Acknowledgment requested */
	};

	/**
	 * 5.2.17 registered_delivery
	 *
	 * SMSC Delivery Receipt (bits 1 and 0)
	 *
	 */
	enum SMSCDeliveryReceiptType {
		SDR_NO_SMSC_DELIVERY 	= 0x00,	/**< No SMSC Delivery Receipt requested (default) */
		SDR_SUCCESS_FAILURE 	= 0x01,	/**< SMSC Delivery Receipt requested where final delivery outcome is delivery success or failure */
		SDR_FAILURE 		= 0x02	/**< SMSC Delivery Receipt requested where the final delivery outcome is delivery failure */

	};

	/**
	 * 5.3.2.37 callback_num_pres_ind
	 *
	 * The Screening  Indicator is encoded in bits 0 and 1 as follows:
	 *
	 */
	enum ScreeningIndicator {
		SI_NOT_SCREENED 	= 0,	/**< User provided, not screened */
		SI_VERIFIED_PASSED 	= 1,	/**< User provided, verified and passed */
		SI_VERIFIED_FAILED 	= 2,	/**< User provided, verified and failed */
		SI_NETWORK_PROVIDED 	= 3	/**< Network Provided */
	};

	/**
	 * 5.3.2.37 callback_num_pres_ind
	 *
	 * The Presentation Indicator is encoded in bits 2 and 3 as follows:
	 *
	 */
	enum PresentationIndicator {
		PI_ALLOWED 		= 0x00,	/**< Presentation Allowed */
		PI_RESTRICTED 		= 0x04,	/**< Presentation Restricted */
		PI_NOT_AVAILABLE 	= 0x08	/**< Number Not Available */


	};

	/**
	 * 5.3.2.33 delivery_failure_reason
	 *
	 */
	enum DeliveryFailureReasonType {
		DFR_DESTINATION_UNAVAILABLE 	= 0x00,	/**< Destination unavailable */
		DFR_DESTINATION_ADDRESS_INVALID = 0x01,	/**< Destination Address Invalid (e.g. suspended, no SMS capability, etc.) */
		DFR_PERMANENT_NETWORK_ERROR 	= 0x02,	/**< Permanent network error */
		DFR_TEMPORARY_NETWORK_ERROR 	= 0x03	/**< Temporary network error */
	};

	/**
	 * 5.3.2.1 dest_addr_subunit
	 *
	 */
	enum DestAddrSubunitType {
		DAS_UNKNOWN 		= 0x00,	/**< Unknown (default) */
		DAS_MS_DISPLAY 		= 0x01,	/**< MS Display */
		DAS_MOBILE_EQUIPMENT 	= 0x02,	/**< Mobile Equipment */
		DAS_SMART_CARD_1 	= 0x03,	/**< Smart Card 1 (expected to be SIM if a SIM exists in the MS) */
		DAS_EXTERNAL_UNIT_1 	= 0x04	/**< External Unit 1 */
	};

	/**
	 * 5.3.2.5 dest_bearer_type
	 *
	 */
	enum DestBearerCodeType {
		DB_UNKNOWN 		= 0x00,	/**< Unknown */
		DB_SMS 			= 0x01,	/**< SMS */
		DB_CSD 			= 0x02,	/**< Circuit Switched Data (CSD) */
		DB_PACKET_DATA 		= 0x03,	/**< Packet Data */
		DB_USSD 		= 0x04,	/**< USSD */
		DB_CDPD 		= 0x05,	/**< CDPD */
		DB_DATA_TAC 		= 0x06,	/**< DataTAC */
		DB_FLEX_REFLEX 		= 0x07,	/**< FLEX/ReFLEX */
		DB_CELL_BROADCAST 	= 0x08	/**< Cell Broadcast (cellcast) */
	};

	/**
	 * 5.3.2.3 dest_network_type
	 *
	 */
	enum DestNetworkCodeType {
		DN_UNKNOWN 		= 0x00,	/**< Unknown */
		DN_GSM 			= 0x01,	/**< GSM */
		DN_ANSI_136 		= 0x02,	/**< ANSI-136/TDMA */
		DN_IS_95 		= 0x03,	/**< IS-95/CDMA */
		DN_PDC 			= 0x04,	/**< PDC */
		DN_PHS 			= 0x05,	/**< PHS */
		DN_IDEN 		= 0x06,	/**< iDEN */
		DN_AMPS 		= 0x07,	/**< AMPS */
		DN_PAGING_NETWORK 	= 0x08	/**< Paging Network */
	};

	/**
	 * 5.3.2.16 dest_subaddress
	 *
	 */
	enum DestSubaddressType {
		DS_NSAP_EVEN 		= 0x80,	/**< NSAP (Even) [ITUT X.213] */
		DS_NSAP_ODD 		= 0x88,	/**< NSAP (Odd) [ITUT X.213] */
		DS_USER_SPECIFIED	= 0xa0	/**< User Specified */
	};


	/**
	 * 5.3.2.26 display_time
	 *
	 */
	enum DisplayTimeType {
		DT_TEMPORARY 	= 0x00,	/**< Temporary */
		DT_DEFAULT 	= 0x01,	/**< Default (default) */
		DT_INVOKE 	= 0x02	/**< Invoke */
	};

	/**
	 * 5.3.2.42 its_reply_type
	 *
	 */
	enum ItsReplyCodeType {
		IRC_DIGIT 		= 0,	/**< Digit */
		IRC_NUMBER 		= 1,	/**< Number */
		IRC_TELEPHONE_NO 	= 2,	/**< Telephone No. */
		IRC_PASSWORD 		= 3,	/**< Password */
		IRC_CHARACTER_LINE 	= 4,	/**< Character Line */
		IRC_MENU 		= 5,	/**< Menu */
		IRC_DATE 		= 6,	/**< Date */
		IRC_TIME 		= 7,	/**< Time */
		IRC_CONTINUE 		= 8	/**< Continue */
	};

	/**
	 * 5.3.2.19 language_indicator
	 *
	 */
	enum LanguageIndicatorType {
		LI_UNSPECIFIED 	= 0x00,	/**< Unspecified (default) */
		LI_ENGLISH 	= 0x01,	/**< English */
		LI_FRENCH 	= 0x02,	/**< French */
		LI_SPANISH 	= 0x03,	/**< Spanish */
		LI_GERMAN 	= 0x04,	/**< German */
		LI_PORTUGUESE 	= 0x05	/**< Portuguese */
	};

	/**
	 * 5.3.2.30 ms_availability_status
	 *
	 */
	enum MsAvailabilityStatusType {
		MAS_AVAILABLE 	= 0x00,	/**< Available (Default) */
		MAS_DENIED 	= 0x01,	/**< Denied (e.g. suspended, no SMS capability, etc.) */
		MAS_UNAVAILABLE = 0x02	/**< Unavailable */
	};


	/**
	 * 5.3.2.13 ms_msg_wait_facilities
	 *
	 */
	enum MsMsgWaitFacilitiesType {
		MMWF_VOICEMAIL_MESSAGE_WAITING 		= 0x00,	/**< Voicemail Message Waiting */
		MMWF_FAX_MESAGE_WAITING 		= 0x01,	/**< Fax Message Waiting */
		MMWF_ELECTRONIC_MAIL_MESSAGE_WAITING 	= 0x02,	/**< Electronic Mail Message Waiting */
		MMWF_OTHER_MESSAGE_WAITING 		= 0x03	/**< Other Message Waiting */
	};

	/**
	 * 5.3.2.27 ms_validity
	 *
	 */
	enum MsValidityType {
		MV_STORE_INDEFINITELY 		= 0x00,	/**< Store Indefinitely (default) */
		MV_POWER_DOWN 			= 0x01,	/**< Power Down */
		MV_SID_BASED_REGISTRATION_AREA 	= 0x02,	/**< SID based registration area */
		MV_DISPLAY_ONLY 		= 0x03	/**< Display Only */
	};

	/**
	 * 5.3.2.31 network_error_code
	 *
	 */
	enum NetworkErrorCodeType {
		NEC_ANSI_136 	= 0x01,	/**< ANSI-136 */
		NEC_IS_95 	= 0x02,	/**< IS-95 */
		NEC_GSM 	= 0x03	/**< GSM */

	};

	/**
	 * 5.3.2.10 payload_type
	 *
	 */
	enum PayloadCodeType {
		PC_DEFAULT 	= 0x00,	/**< Default. In the case of a WAP application, the default higher layer message type is a WDP message. See [WDP] for details. */
		PC_WCMP 	= 0x01	/**< WCMP message. Wireless Control Message Protocol formatted data. See [WCMP] for details. */
	};

	/**
	 * 5.3.2.14 privacy_indicator
	 *
	 */
	enum PrivacyIndicatorType {
		PI_PRIVACY_LEVEL_0 = 0x00,	/**< Privacy Level 0 (Not Restricted) (default) */
		PI_PRIVACY_LEVEL_1 = 0x01,	/**< Privacy Level 1 (Restricted) */
		PI_PRIVACY_LEVEL_2 = 0x02,	/**< Privacy Level 2 (Confidential) */
		PI_PRIVACY_LEVEL_3 = 0x03	/**< Privacy Level 3 (Secret) */
	};




	/**
	 * 5.3.2.2 source_addr_subunit
	 *
	 */
	enum SourceAddrSubunitType {
		SAS_UNKNOWN 		= 0x00,	/**< Unknown (default) */
		SAS_MS_DISPLAY 		= 0x01,	/**< MS Display */
		SAS_MOBILE_EQUIPMENT 	= 0x02,	/**< Mobile Equipment */
		SAS_SMART_CARD_1 	= 0x03,	/**< Smart Card 1 (expected to be SIM if a SIM exists in the MS) */
		SAS_EXTERNAL_UNIT_1 	= 0x04	/**< External Unit 1 */
	};

	/**
	 * 5.3.2.6 source_bearer_type
	 *
	 */
	enum SourceBearerCodeType {
		SBC_UNKNOWN 		= 0x00,	/**< Unknown */
		SBC_SMS 		= 0x01,	/**< SMS */
		SBC_CSD 		= 0x02,	/**< Circuit Switched Data (CSD) */
		SBC_PACKET_DATA 	= 0x03,	/**< Packet Data */
		SBC_USSD 		= 0x04,	/**< USSD */
		SBC_CDPD 		= 0x05,	/**< CDPD */
		SBC_DATA_TAC 		= 0x06,	/**< DataTAC */
		SBC_FLEX_REFLEX		= 0x07,	/**< FLEX/ReFLEX */
		SBC_CELL_BROADCAST 	= 0x08	/**< Cell Broadcast (cellcast) */
	};

	/**
	 * 5.3.2.4 source_network_type
	 *
	 */
	enum SourceNetworkCodeType {
		SN_UNKNOWN 		= 0x00,	/**< Unknown */
		SN_GSM 			= 0x01,	/**< GSM */
		SN_ANSI_136 		= 0x02,	/**< ANSI-136/TDMA */
		SN_IS_95 		= 0x03,	/**< IS-95/CDMA */
		SN_PDC 			= 0x04,	/**< PDC */
		SN_PHS 			= 0x05,	/**< PHS */
		SN_IDEN 		= 0x06,	/**< iDEN */
		SN_AMPS 		= 0x07,	/**< AMPS */
		SN_PAGING_NETWORK 	= 0x08	/**< Paging Network */
	};

	/**
	 * 5.3.2.15 source_subaddress
	 *
	 */
	enum SourceSubaddressType {
		SS_NSAP_EVEN 		= 0x80,	/**< NSAP (Even) [ITUT X.213] */
		SS_NSAP_ODD 		= 0x88,	/**< NSAP (Odd) [ITUT X.213] */
		SS_USER_SPECIFIED	= 0xa0	/**< User Specified */
	};


	/**
	 * 5.3.2.44 ussd_service_op
	 *
	 */
	enum UssdServiceOpType {
		USO_PSSD_INDICATION 	= 0,	/**< PSSD indication */
		USO_PSSR_INDICATION 	= 1,	/**< PSSR indication */
		USO_USSR_REQUEST 	= 2,	/**< USSR request */
		USO_USSN_REQUEST 	= 3,	/**< USSN request */
		USO_PSSD_RESPONSE 	= 16,	/**< PSSD response */
		USO_PSSR_RESPONSE 	= 17,	/**< PSSR response */
		USO_USSR_CONFIRM 	= 18,	/**< USSR confirm */
		USO_USSN_CONFIRM 	= 19	/**< USSN confirm */

	};

	/**
	 * 5.2.25 dest_flag
	 *
	 */
	enum DestAddressType {
		DA_SME_ADDRESS 			= 1,	/**< SME Address */
		DA_DISTRIBUTION_LIST_ADDRESS 	= 2	/**< Distribution List Name */
	};

	// fwd
	class SMPPPool;

	/**
	 * 4.5.1.1 Destination Address definition Table 4-14: SME_dest_address
	 *
	 */
	class DestAddress {
	public:
		SmppTON dest_addr_ton;
		SmppNPI dest_addr_npi;
		char* destination_addr;
	};

	/**
	 * 4.5.1.2 Distribution List (DL) definition Table 4-15: DL Name
	 *
	 */
	class DLAddress {
	public:
		char* dl_name;

	};

	/**
	 * 4.5.1.1 Destination Address definition
	 *
	 */
	class AddressDefinition {
	public:
		DestAddressType dest_flag;
		DestAddress dest_address;
		DLAddress dl_address;

	};

	/**
	 * 4.5.2.1 Unsuccessful deliveries
	 *
	 */
	class UnsDeliveryType {
	public:
		SmppTON dest_addr_ton;
		SmppNPI dest_addr_npi;
		char* destination_addr;
		SmppCommandStatus error_status_code;
	};


	/**
	 * Base SMPP Parameter class
	 */
	class SmppParam {
	public:
		SmppParam();
		SmppParam(SmppParameterType _type);
		virtual ~SmppParam();
		virtual int init(unsigned char* data, int data_length);
		virtual int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		SmppParameterType type;
		unsigned char* param_data;
		int length;
		int byte_pos;

	};

	/**
	 * 5.3.2.11 additional_status_info_text
	 *
	 * The additional_status_info_text parameter gives an ASCII textual description of the meaning
	 * of a response PDU. It is to be used by an implementation to allow easy diagnosis of problems.
	 *
	 */
	class Additional_status_info_text: public SmppParam {
	public:
		Additional_status_info_text();
		~Additional_status_info_text();
	};

	/**
	 * 5.3.2.41 alert_on_message_delivery
	 *
	 * The alert_on_message_delivery parameter is set to instruct a MS to alert the user (in a MS
	 * implementation specific manner) when the short message arrives at the MS.
	 *
	 */
	class Alert_on_message_delivery: public SmppParam {
	public:
		Alert_on_message_delivery();
		~Alert_on_message_delivery();
	};

	/**
	 * 5.3.2.38 callback_num_atag
	 *
	 * The callback_num_atag parameter associates an alphanumeric display with the call back
	 * number.
	 *
	 */
	class Callback_num_atag: public SmppParam {
	public:
		Callback_num_atag();
		~Callback_num_atag();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		Data_codingType data_coding;
		unsigned char* character_octets;
		unsigned int character_octet_count;
	};

	/**
	 * 5.3.2.37 callback_num_pres_ind
	 *
	 */
	class Callback_num_pres_ind: public SmppParam {
	public:
		Callback_num_pres_ind();
		~Callback_num_pres_ind();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		PresentationIndicator presentation_indicator;
		ScreeningIndicator screening_indicator;

	};

	/**
	 * 5.3.2.36 callback_num
	 *
	 * The callback_num parameter associates a call back number with the message. In TDMA
	 * networks, it is possible to send and receive multiple callback numbers to/from TDMA mobile
	 * stations.
	 *
	 */
	class Callback_num: public SmppParam {
	public:
		Callback_num();
		~Callback_num();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		bool dtmf;
		SmppTON ton;
		SmppNPI npi;
		unsigned char* digits;
		unsigned int digit_count;
	};

	/**
	 * 5.3.2.33 delivery_failure_reason
	 *
	 * The delivery_failure_reason parameter is used in the data_sm_resp operation to indicate the
	 * outcome of the message delivery attempt (only applicable for transaction message mode). If a
	 * delivery failure due to a network error is indicated, the ESME may check the
	 * network_error_code parameter (if present) for the actual network error code.
	 * The delivery_failure_reason parameter is not included if the delivery attempt was successful.
	 *
	 */
	class Delivery_failure_reason: public SmppParam {
	public:
		Delivery_failure_reason();
		~Delivery_failure_reason();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		DeliveryFailureReasonType dfr_type;
	};

	/**
	 * 5.3.2.1 dest_addr_subunit
	 *
	 * The dest_addr_subunit parameter is used to route messages when received by a mobile station,
	 * for example to a smart card in the mobile station or to an external device connected to the
	 * mobile station.
	 *
	 */
	class Dest_addr_subunit: public SmppParam {
	public:
		Dest_addr_subunit();
		~Dest_addr_subunit();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		DestAddrSubunitType das_type;
	};

	/**
	 * 5.3.2.5 dest_bearer_type
	 *
	 * The dest_bearer_type parameter is used to request the desired bearer for delivery of the
	 * message to the destination address. In the case that the receiving system (e.g. SMSC) does not
	 * support the indicated bearer type, it may treat this a failure and return a response PDU reporting
	 * a failure.
	 *
	 */
	class Dest_bearer_type: public SmppParam {
	public:
		Dest_bearer_type();
		~Dest_bearer_type();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		DestBearerCodeType db_type;
	};

	/**
	 * 5.3.2.3 dest_network_type
	 *
	 * The dest_network_type parameter is used to indicate a network type associated with the
	 * destination address of a message. In the case that the receiving system (e.g. SMSC) does not
	 * support the indicated network type, it may treat this a failure and return a response PDU
	 * reporting a failure.
	 *
	 */
	class Dest_network_type: public SmppParam {
	public:
		Dest_network_type();
		~Dest_network_type();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		DestNetworkCodeType dn_type;
	};

	/**
	 * 5.3.2.16 dest_subaddress
	 *
	 * The dest_subaddress parameter specifies a subaddress associated with the destination of the
	 * message.
	 *
	 */
	class Dest_subaddress: public SmppParam {
	public:
		Dest_subaddress();
		~Dest_subaddress();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		DestSubaddressType ds_type;
		unsigned char* subaddress;
		unsigned int subaddress_length;
	};

	/**
	 * 5.3.2.7 dest_telematics_id
	 *
	 * This parameter defines the telematic interworking to be used by the delivering system for the
	 * destination address. This is only useful when a specific dest_bearer_type parameter has also
	 * been specified as the value is bearer dependent. In the case that the receiving system (e.g.
	 * SMSC) does not support the indicated telematic interworking, it may treat this a failure and
	 * return a response PDU reporting a failure.
	 *
	 */
	class Dest_telematics_id: public SmppParam {
	public:
		Dest_telematics_id();
		~Dest_telematics_id();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint16_t value;
	};

	/**
	 * 5.3.2.21 destination_port
	 *
	 * The destination_port parameter is used to indicate the application port number associated with
	 * the destination address of the message.
	 *
	 */
	class Destination_port: public SmppParam {
	public:
		Destination_port();
		~Destination_port();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint16_t value;

	};

	/**
	 * 5.3.2.26 display_time
	 *
	 * The display_time parameter is used to associate a display time of the short message on the MS.
	 *
	 */
	class Display_time: public SmppParam {
	public:
		Display_time();
		~Display_time();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		DisplayTimeType dt_type;
	};

	/**
	 * 5.3.2.28 dpf_result
	 *
	 * The dpf_result parameter is used in the data_sm_resp PDU to indicate if delivery pending flag
	 * (DPF) was set for a delivery failure of the short message..
	 * If the dpf_result parameter is not included in the data_sm_resp PDU, the ESME should assume
	 * that DPF is not set.
	 *
	 * Currently this parameter is only applicable for the Transaction message mode.
	 *
	 */
	class Dpf_result: public SmppParam {
	public:
		Dpf_result();
		~Dpf_result();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		bool dpf_set;
	};

	/**
	 * 5.3.2.42 its_reply_type
	 *
	 * The its_reply_type parameter is a required parameter for the CDMA Interactive Teleservice as
	 * defined by the Korean PCS carriers [KORITS]. It indicates and controls the MS user’s reply
	 * method to an SMS delivery message received from the ESME.
	 *
	 */
	class Its_reply_type: public SmppParam {
	public:
		Its_reply_type();
		~Its_reply_type();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		ItsReplyCodeType its_rc_type;
	};

	/**
	 * 5.3.2.43 its_session_info
	 *
	 * The its_session_info parameter is a required parameter for the CDMA Interactive Teleservice
	 * as defined by the Korean PCS carriers [KORITS]. It contains control information for the
	 * interactive session between an MS and an ESME.
	 *
	 */
	class Its_session_info: public SmppParam {
	public:
		Its_session_info();
		~Its_session_info();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		int session_number;
		int sequence_number;
		bool end_of_session;
	};

	/**
	 * 5.3.2.19 language_indicator
	 *
	 * The language_indicator parameter is used to indicate the language of the short message.
	 *
	 */
	class Language_indicator: public SmppParam {
	public:
		Language_indicator();
		~Language_indicator();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		LanguageIndicatorType li_type;
	};

	/**
	 * 5.3.2.32 message_payload
	 *
	 * The message_payload parameter contains the user data.
	 *
	 */
	class Message_payload: public SmppParam {
	public:
		Message_payload();
		~Message_payload();
	};

	/**
	 * 5.3.2.35 message_state
	 *
	 * The message_state optional parameter is used by the SMSC in the deliver_sm and data_sm
	 * PDUs to indicate to the ESME the final message state for an SMSC Delivery Receipt.
	 *
	 */
	class Message_state: public SmppParam {
	public:
		Message_state();
		~Message_state();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		MessageStateType message_state;
	};

	/**
	 * 5.3.2.34 more_messages_to_send
	 *
	 * The more_messages_to_send parameter is used by the ESME in the submit_sm and data_sm
	 * operations to indicate to the SMSC that there are further messages for the same destination
	 * SME. The SMSC may use this setting for network resource optimization.
	 *
	 */
	class More_messages_to_send: public SmppParam {
	public:
		More_messages_to_send();
		~More_messages_to_send();
	};

	/**
	 * 5.3.2.30 ms_availability_status
	 *
	 * The ms_availability_status parameter is used in the alert_notification operation to indicate the
	 * availability state of the MS to the ESME.
	 *
	 * If the SMSC does not include the parameter in the alert_notification operation, the ESME
	 * should assume that the MS is in an “available” state.
	 *
	 */
	class Ms_availability_status: public SmppParam {
	public:
		Ms_availability_status();
		~Ms_availability_status();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		MsAvailabilityStatusType mas_type;
	};

	/**
	 * 5.3.2.13 ms_msg_wait_facilities
	 *
	 * The ms_msg_wait_facilities parameter allows an indication to be provided to an MS that there
	 * are messages waiting for the subscriber on systems on the PLMN. The indication can be an icon
	 * on the MS screen or other MMI indication.
	 *
	 * The ms_msg_wait_facilities can also specify the type of message associated with the message
	 * waiting indication.
	 *
	 */
	class Ms_msg_wait_facilities: public SmppParam {
	public:
		Ms_msg_wait_facilities();
		~Ms_msg_wait_facilities();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		MsMsgWaitFacilitiesType mmwf_type;
		bool indication;
	};

	/**
	 * 5.3.2.27 ms_validity
	 *
	 * The ms_validity parameter is used to provide an MS with validity information associated with
	 * the received short message.
	 *
	 */
	class Ms_validity: public SmppParam {
	public:
		Ms_validity();
		~Ms_validity();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		MsValidityType mv_type;
	};

	/**
	 * 5.3.2.31 network_error_code
	 *
	 * The network_error_code parameter is used to indicate the actual network error code for a
	 * delivery failure. The network error code is technology specific.
	 */
	class Network_error_code: public SmppParam {
	public:
		Network_error_code();
		~Network_error_code();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		NetworkErrorCodeType nec_type;
		SmppCommandStatus error_code;
	};

	/**
	 * 5.3.2.39 number_of_messages
	 *
	 * The number_of_messages parameter is used to indicate the number of messages stored in a
	 * mailbox.
	 *
	 */
	class Number_of_messages: public SmppParam {
	public:
		Number_of_messages();
		~Number_of_messages();
	};

	/**
	 * 5.3.2.10 payload_type
	 *
	 * The payload_type parameter defines the higher layer PDU type contained in the message
	 * payload.
	 *
	 */
	class Payload_type: public SmppParam {
	public:
		Payload_type();
		~Payload_type();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		PayloadCodeType pc_type;
	};

	/**
	 * 5.3.2.14 privacy_indicator
	 *
	 * The privacy_indicator indicates the privacy level of the message.
	 *
	 */
	class Privacy_indicator: public SmppParam {
	public:
		Privacy_indicator();
		~Privacy_indicator();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		PrivacyIndicatorType pi_type;
	};

	/**
	 * 5.3.2.9 qos_time_to_live
	 *
	 * This parameter defines the number of seconds which the sender requests the SMSC to keep the
	 * message if undelivered before it is deemed expired and not worth delivering. If the parameter
	 * is not present, the SMSC may apply a default value.
	 *
	 */
	class Qos_time_to_live: public SmppParam {
	public:
		Qos_time_to_live();
		~Qos_time_to_live();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint32_t value;
	};

	/**
	 * 5.3.2.12 receipted_message_id
	 *
	 * The receipted_message_id parameter indicates the ID of the message being receipted in an
	 * SMSC Delivery Receipt. This is the opaque SMSC message identifier that was returned in the
	 * message_id parameter of the SMPP response PDU that acknowledged the submission of the
	 * original message.
	 *
	 */
	class Receipted_message_id: public SmppParam {
	public:
		Receipted_message_id();
		~Receipted_message_id();
	};

	/**
	 * 5.3.2.22 sar_msg_ref_num
	 *
	 * The sar_msg_ref_num parameter is used to indicate the reference number for a particular
	 * concatenated short message.
	 */
	class Sar_msg_ref_num: public SmppParam {
	public:
		Sar_msg_ref_num();
		~Sar_msg_ref_num();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint16_t value;
	};

	/**
	 * 5.3.2.24 sar_segment_seqnum
	 *
	 * The sar_segment_seqnum parameter is used to indicate the sequence number of a particular
	 * short message within the concatenated short message.
	 *
	 */
	class Sar_segment_seqnum: public SmppParam {
	public:
		Sar_segment_seqnum();
		~Sar_segment_seqnum();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint8_t value;
	};

	/**
	 * 5.3.2.23 sar_total_segments
	 *
	 * The sar_total_segments parameter is used to indicate the total number of short messages
	 * within the concatenated short message.
	 */
	class Sar_total_segments: public SmppParam {
	public:
		Sar_total_segments();
		~Sar_total_segments();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint8_t value;
	};

	/**
	 * 5.3.2.25 sc_interface_version
	 *
	 * The sc_interface_version parameter is used to indicate the SMPP version supported by the
	 * SMSC. It is returned in the bind response PDUs.
	 *
	 */
	class Sc_interface_version: public SmppParam {
	public:
		Sc_interface_version();
		~Sc_interface_version();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint8_t value;
	};

	/**
	 * 5.3.2.29 set_dpf
	 * An ESME may use the set_dpf parameter to request the setting of a delivery pending flag (DPF)
	 * for certain delivery failure scenarios, such as
	 *
	 * - MS is unavailable for message delivery (as indicated by the HLR)
	 *
	 * The SMSC should respond to such a request with an alert_notification PDU when it detects
	 * that the destination MS has become available.
	 *
	 * The delivery failure scenarios under which DPF is set is SMSC implementation and network
	 * implementation specific. If a delivery pending flag is set by the SMSC or network (e.g. HLR),
	 * then the SMSC should indicate this to the ESME in the data_sm_resp message via the
	 * dpf_result parameter.
	 *
	 */
	class Set_dpf: public SmppParam {
	public:
		Set_dpf();
		~Set_dpf();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint8_t value;
	};

	/**
	 * 5.3.2.40 sms_signal
	 *
	 * The sms_signal parameter is used to provide a TDMA MS with alert tone information
	 * associated with the received short message.
	 *
	 */
	class Sms_signal: public SmppParam {
	public:
		Sms_signal();
		~Sms_signal();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint16_t value;
	};

	/**
	 * 5.3.2.2 source_addr_subunit
	 *
	 * The source_addr_subunit parameter is used to indicate where a message originated in the
	 * mobile station, for example a smart card in the mobile station or an external device connected
	 * to the mobile station.
	 *
	 */
	class Source_addr_subunit: public SmppParam {
	public:
		Source_addr_subunit();
		~Source_addr_subunit();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		SourceAddrSubunitType sas_type;
	};

	/**
	 * 5.3.2.6 source_bearer_type
	 *
	 * The source_bearer_type parameter indicates the wireless bearer over which the message
	 * originated.
	 *
	 */
	class Source_bearer_type: public SmppParam {
	public:
		Source_bearer_type();
		~Source_bearer_type();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		SourceBearerCodeType sbc_type;
	};

	/**
	 * 5.3.2.4 source_network_type
	 *
	 * The source_network_type parameter is used to indicate the network type associated with the
	 * device that originated the message.
	 *
	 */
	class Source_network_type: public SmppParam {
	public:
		Source_network_type();
		~Source_network_type();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		SourceNetworkCodeType snc_type;
	};

	/**
	 * 5.3.2.20 source_port
	 *
	 * The source_port parameter is used to indicate the application port number associated with the
	 * source address of the message.
	 *
	 */
	class Source_port: public SmppParam {
	public:
		Source_port();
		~Source_port();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint16_t value;
	};

	/**
	 * 5.3.2.15 source_subaddress
	 *
	 * The source_subaddress parameter specifies a subaddress associated with the originator of the
	 * message.
	 *
	 */
	class Source_subaddress: public SmppParam {
	public:
		Source_subaddress();
		~Source_subaddress();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		SourceSubaddressType ss_type;
		unsigned char* subaddress;
		unsigned int subaddress_length;
	};

	/**
	 * 5.3.2.8 source_telematics_id
	 *
	 * The source_telematics_id parameter indicates the type of telematics interface over which the
	 * message originated.
	 *
	 */
	class Source_telematics_id: public SmppParam {
	public:
		Source_telematics_id();
		~Source_telematics_id();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint8_t value;
	};

	/**
	 * 5.3.2.17 user_message_reference
	 *
	 * A reference assigned by the originating SME to the short message.
	 *
	 */
	class User_message_reference: public SmppParam {
	public:
		User_message_reference();
		~User_message_reference();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint16_t value;
	};

	/**
	 * 5.3.2.18 user_response_code
	 *
	 * A response code set by the user in a User Acknowledgement/Reply message. The response
	 * codes are application specific.
	 *
	 */
	class User_response_code: public SmppParam {
	public:
		User_response_code();
		~User_response_code();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		uint8_t value;
	};

	/**
	 * 5.3.2.44 ussd_service_op
	 *
	 * The ussd_service_op parameter is required to define the USSD service operation when SMPP
	 * is being used as an interface to a (GSM) USSD system.
	 *
	 */
	class Ussd_service_op: public SmppParam {
	public:
		Ussd_service_op();
		~Ussd_service_op();
		int init(unsigned char* data, int data_length);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		UssdServiceOpType uso_type;
	};


	/**
	 *  Base SMPP Command/Body class
	 */
	class SmppCommand {
	public:
		SmppCommand();
		SmppCommand(SmppCommandId _type);
		virtual ~SmppCommand();
		virtual int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		virtual int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
		int encode_optional(unsigned char* buffer, int buffer_length, int offset, int* result_length);
		int get_param_count();
		void clear_params();
		SmppParam* get_parameter(SmppParameterType _param_type);
		SmppParam* add_parameter(SmppParam* param);
		int remove_parameter(SmppParameterType _param_type);

		std::vector<SmppParam*> params;
		SmppCommandId type;
		unsigned int byte_pos;
		int length;

	protected:
		void process_params(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode_parameter(SmppParam* param, unsigned char* buffer, int buffer_length, int offset);

	};

	/**
	 * 4.1.1 BIND_TRANSMITTER Syntax
	 *
	 */
	class Bind_transmitter: public SmppCommand {
	public:
		Bind_transmitter();
		~Bind_transmitter();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* system_id;
		char* password;
		char* system_type;
		uint8_t interface_version;
		SmppTON addr_ton;
		SmppNPI addr_npi;
		char* address_range;
	};

	/**
	 * 4.1.2 BIND_TRANSMITTER_RESP Syntax
	 *
	 */
	class Bind_transmitter_resp: public SmppCommand {
	public:
		Bind_transmitter_resp();
		~Bind_transmitter_resp();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* system_id;
	};

	/**
	 * 4.1.3 BIND_RECEIVER Syntax
	 *
	 */
	class Bind_receiver: public SmppCommand {
	public:
		Bind_receiver();
		~Bind_receiver();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* system_id;
		char* password;
		char* system_type;
		unsigned int interface_version;
		SmppTON addr_ton;
		SmppNPI addr_npi;
		char* address_range;
	};

	/**
	 * 4.1.4 BIND_RECEIVER_RESP
	 *
	 */
	class Bind_receiver_resp: public SmppCommand {
	public:
		Bind_receiver_resp();
		~Bind_receiver_resp();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* system_id;
	};

	/**
	 * 4.1.5 BIND_TRANSCEIVER Syntax
	 *
	 */
	class Bind_transceiver: public SmppCommand {
	public:
		Bind_transceiver();
		~Bind_transceiver();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* system_id;
		char* password;
		char* system_type;
		unsigned int interface_version;
		SmppTON addr_ton;
		SmppNPI addr_npi;
		char* address_range;
	};

	/**
	 * 4.1.6 BIND_TRANSCEIVER_RESP
	 *
	 */
	class Bind_transceiver_resp: public SmppCommand {
	public:
		Bind_transceiver_resp();
		~Bind_transceiver_resp();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* system_id;
	};

	/**
	 * 4.1.7.1 OUTBIND Syntax
	 *
	 */
	class Outbind: public SmppCommand {
	public:
		Outbind();
		~Outbind();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* system_id;
		char* password;
	};

	/**
	 * 4.2.1 UNBIND
	 */
	class Unbind: public SmppCommand {
	public:
		Unbind();
		~Unbind();
	};

	/**
	 * 4.2.2 UNBIND_RESP
	 *
	 */
	class Unbind_resp: public SmppCommand {
	public:
		Unbind_resp();
		~Unbind_resp();
	};


	/**
	 * 4.3.1 GENERIC_NACK Syntax
	 *
	 */
	class Generic_nack: public SmppCommand {
	public:
		Generic_nack();
		~Generic_nack();
	};

	/**
	 * 4.4.1 SUBMIT_SM Syntax
	 *
	 */
	class Submit_sm: public SmppCommand {
	public:
		Submit_sm();
		~Submit_sm();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* service_type;
		SmppTON source_addr_ton;
		SmppNPI source_addr_npi;
		char* source_addr;
		SmppTON dest_addr_ton;
		SmppNPI dest_addr_npi;
		char* destination_addr;
		MessageMode esm_class_mm;
		MessageType esm_class_mt;
		GSMNetworkSpecific esm_gsm_specific;
		uint8_t protocol_id;
		uint8_t priority_flag;
		char* schedule_delivery_time;
		char* validity_period;
		SMSCDeliveryReceiptType rd_smsc_dlvr_rcpt;
		SMEOrigAckType rd_sme_orig_ack;
		IntermediateNotificationType rd_intrmd_ntf;
		uint8_t replace_if_present_flag;
		Data_codingType data_coding;
		uint8_t sm_default_msg_id;
		uint8_t sm_length;
		unsigned char* short_message;




	};

	/**
	 * 4.4.2 SUBMIT_SM_RESP
	 *
	 */
	class Submit_sm_resp: public SmppCommand {
	public:
		Submit_sm_resp();
		~Submit_sm_resp();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* message_id;
	};

	/**
	 * 4.5.1 SUBMIT_MULTI Syntax
	 *
	 */
	class Submit_multi: public SmppCommand {
	public:
		Submit_multi();
		~Submit_multi();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* service_type;
		SmppTON source_addr_ton;
		SmppNPI source_addr_npi;
		char* source_addr;
		uint8_t number_of_dests;
		std::vector<AddressDefinition> dest_address;
		MessageMode esm_class_mm;
		MessageType esm_class_mt;
		GSMNetworkSpecific esm_gsm_specific;
		uint8_t protocol_id;
		uint8_t priority_flag;
		char* schedule_delivery_time;
		char* validity_period;
		SMSCDeliveryReceiptType rd_smsc_dlvr_rcpt;
		SMEOrigAckType rd_sme_orig_ack;
		IntermediateNotificationType rd_intrmd_ntf;
		uint8_t replace_if_present_flag;
		Data_codingType data_coding;
		uint8_t sm_default_msg_id;
		uint8_t sm_length;
		unsigned char* short_message;
	};

	/**
	 * 4.5.2 SUBMIT_MULTI_RESP Syntax
	 *
	 */
	class Submit_multi_resp: public SmppCommand {
	public:
		Submit_multi_resp();
		~Submit_multi_resp();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* message_id;
		uint8_t no_unsuccess;
		std::vector<UnsDeliveryType> unsuccess_sme;

	};

	/**
	 * 4.6.1 DELIVER_SM Syntax
	 *
	 */
	class Deliver_sm: public SmppCommand {
	public:
		Deliver_sm();
		~Deliver_sm();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* service_type;
		SmppTON source_addr_ton;
		SmppNPI source_addr_npi;
		char* source_addr;
		SmppTON dest_addr_ton;
		SmppNPI dest_addr_npi;
		char* destination_addr;
		MessageMode esm_class_mm;
		MessageType esm_class_mt;
		GSMNetworkSpecific esm_gsm_specific;
		uint8_t protocol_id;
		uint8_t priority_flag;
		char* schedule_delivery_time;
		char* validity_period;
		SMSCDeliveryReceiptType rd_smsc_dlvr_rcpt;
		SMEOrigAckType rd_sme_orig_ack;
		IntermediateNotificationType rd_intrmd_ntf;
		uint8_t replace_if_present_flag;
		Data_codingType data_coding;
		uint8_t sm_default_msg_id;
		uint8_t sm_length;
		unsigned char* short_message;
	};

	/**
	 * 4.6.2 DELIVER_SM_RESP Syntax
	 *
	 */
	class Deliver_sm_resp: public SmppCommand {
	public:
		Deliver_sm_resp();
		~Deliver_sm_resp();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* message_id;
	};

	/**
	 * 4.7.1 DATA_SM Syntax
	 *
	 */
	class Data_sm: public SmppCommand {
	public:
		Data_sm();
		~Data_sm();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* service_type;
		SmppTON source_addr_ton;
		SmppNPI source_addr_npi;
		char* source_addr;
		SmppTON dest_addr_ton;
		SmppNPI dest_addr_npi;
		char* destination_addr;
		MessageMode esm_class_mm;
		MessageType esm_class_mt;
		GSMNetworkSpecific esm_gsm_specific;
		SMSCDeliveryReceiptType rd_smsc_dlvr_rcpt;
		SMEOrigAckType rd_sme_orig_ack;
		IntermediateNotificationType rd_intrmd_ntf;
		Data_codingType data_coding;
	};

	/**
	 * 4.7.2 DATA_SM_RESP Syntax
	 *
	 */
	class Data_sm_resp: public SmppCommand {
	public:
		Data_sm_resp();
		~Data_sm_resp();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* message_id;
	};

	/**
	 * 4.8.1 QUERY_SM Syntax
	 *
	 */
	class Query_sm: public SmppCommand {
	public:
		Query_sm();
		~Query_sm();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* message_id;
		SmppTON source_addr_ton;
		SmppNPI source_addr_npi;
		char* source_addr;

	};

	/**
	 * 4.8.2 QUERY_SM_RESP Syntax
	 *
	 */
	class Query_sm_resp: public SmppCommand {
	public:
		Query_sm_resp();
		~Query_sm_resp();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* message_id;
		char* final_date;
		MessageStateType message_state;
		uint8_t error_code;
	};

	/**
	 * 4.9.1 CANCEL_SM Syntax
	 *
	 */
	class Cancel_sm: public SmppCommand {
	public:
		Cancel_sm();
		~Cancel_sm();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* service_type;
		char* message_id;
		SmppTON source_addr_ton;
		SmppNPI source_addr_npi;
		char* source_addr;
		SmppTON dest_addr_ton;
		SmppNPI dest_addr_npi;
		char* destination_addr;


	};

	/**
	 * 4.9.2 CANCEL_SM_RESP Syntax
	 *
	 */
	class Cancel_sm_resp: public SmppCommand {
	public:
		Cancel_sm_resp();
		~Cancel_sm_resp();




	};

	/**
	 * 4.10.1 REPLACE_SM Syntax
	 *
	 */
	class Replace_sm: public SmppCommand {
	public:
		Replace_sm();
		~Replace_sm();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		char* message_id;
		SmppTON source_addr_ton;
		SmppNPI source_addr_npi;
		char* source_addr;
		char* schedule_delivery_time;
		char* validity_period;
		SMSCDeliveryReceiptType rd_smsc_dlvr_rcpt;
		SMEOrigAckType rd_sme_orig_ack;
		IntermediateNotificationType rd_intrmd_ntf;
		uint8_t sm_default_msg_id;
		uint8_t sm_length;
		unsigned char* short_message;
	};

	/**
	 * 4.10.2 REPLACE_SM_RESP Syntax
	 *
	 */
	class Replace_sm_resp: public SmppCommand {
	public:
		Replace_sm_resp();
		~Replace_sm_resp();
	};

	/**
	 * 4.11.1 ENQUIRE_LINK Syntax
	 *
	 */
	class Enquire_link: public SmppCommand {
	public:
		Enquire_link();
		~Enquire_link();
	};

	/**
	 * 4.11.2 ENQUIRE_LINK_RESP Syntax
	 *
	 */
	class Enquire_link_resp: public SmppCommand {
	public:
		Enquire_link_resp();
		~Enquire_link_resp();
	};

	/**
	 * 4.12.1 ALERT_NOTIFICATION Syntax
	 *
	 */
	class Alert_notification: public SmppCommand {
	public:
		Alert_notification();
		~Alert_notification();
		int init(unsigned char* data, unsigned int data_length, SMPPPool* _smpp_pool);
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		SmppTON source_addr_ton;
		SmppNPI source_addr_npi;
		char* source_addr;
		SmppTON esme_addr_ton;
		SmppNPI esme_addr_npi;
		char* esme_addr;



	};


	// smpp pool classes
	class SMPPParamPoolItem {
	private:
		SmppParam** pool;
		SmppParam* next_free_item;
		int total_count;
		int free_count;
		SmppParam* create_param(SmppParameterType _param_type);
	public:
		SMPPParamPoolItem();
		~SMPPParamPoolItem();
		SmppParameterType type;
		SmppParam* request_item();
		void init_pool();
		void set_pool_size(int _total_count);

	};

	class SMPPCommandPoolItem {
	private:
		SmppCommand** pool;
		SmppCommand* next_free_item;
		int total_count;
		int free_count;
		SmppCommand* create_command(SmppCommandId _command_type);
	public:
		SMPPCommandPoolItem();
		~SMPPCommandPoolItem();
		SmppCommandId type;
		SmppCommand* request_item();
		void init_pool();
		void set_pool_size(int _total_count);

	};

	class SMPPPool {
	private:
		std::map<SmppCommandId, SMPPCommandPoolItem*> COMMAND_POOL;
		std::map<SmppParameterType, SMPPParamPoolItem*> PARAM_POOL;
		int param_count;
		int command_count;
		void init_param(SmppParameterType _param_type);
		void init_command(SmppCommandId _command_type);
	public:
		SMPPPool();
		~SMPPPool();
		void set_pool_size(int _param_count, int _command_count);
		void init_pool();
		SmppCommand* request_command(SmppCommandId command_type);
		SmppParam* request_param(SmppParameterType param_type);


	};

	/**
	 * 3.2 SMPP PDU
	 *
	 */
	class SmppPDU {
	public:
		SmppPDU();
		virtual ~SmppPDU();
		int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

		// header
		uint32_t command_length;
		SmppCommandId command_id;
		SmppCommandStatus command_status;
		uint32_t sequence_number;
		// body
		SmppCommand* command;
		// pool
		SMPPPool _pool;

	};

        /**
         * Decode C string length
         * @param[in]   data        Pointer to C string data
         * @param[in]   data_length Length of C string data in bytes
         * @return      Length of C string (including NULL terminator)
         */
	int Cstr_len(unsigned char* data, int data_length);

        /**
         * Decode SMPP time format
         * @param[in]   data        Pointer to time data
         * @return      Unix timestamp
         */
	time_t decode_time_format(char* data);

        /**
         * SMPP Delivery receipt class
         */
	class DlvrRcpt{
	public:
	    DlvrRcpt();

	    char id[64];
	    uint32_t sub;
	    uint32_t dlvrd;
	    time_t submit_date;
	    time_t done_date;
	    MessageStateType stat;
	    uint32_t err;
	    unsigned char text[20];
	    uint32_t text_size;

	    void reset();
	    int decode(unsigned char* data, int data_length);
	    int encode(unsigned char* buffer, int buffer_length, int* result_length);
	    // *** other variants ***
	    int decode_var_1(unsigned char* data, int data_length);
	    int decode_var_2(unsigned char* data, int data_length);

	};

        /**
         * TCP framing class for SMPP PDU
         */
        class TcpFraming {
        public:
            /**
             * Constructor
             */
            TcpFraming();

            /**
             * Destructor
             */
            virtual ~TcpFraming();

            /**
             * Process smpp data bytes, call on_pdu_ready method for each reassembled
             * SMPP PDU
             * @param[in]   buff        Pointer to raw smpp data
             * @param[in]   biff_size   Size of raw smpp data in
             * @param[in]   args        Generic user arguments
             * @return      Zero (0) for success of error code 
             */
            int process(unsigned char* buff, unsigned int buff_size, void* args = NULL);

            /**
             * Reset internal counters
             */
            void reset();

            /**
             * Reassembled PDU event handler
             * @param[in]   pdu         Pointer to raw PDU data
             * @param[in]   pdu_size    Size of raw PDU data
             * @param[in]   args        Generic user arguments, passed from process method
             * @return      Zero (0) for success or error code
             */
            virtual int on_pdu_ready(unsigned char* pdu, unsigned int pdu_size, void* args) = 0;
        
        private:
            // tcp separation/buffering
            unsigned char sep_buff[65535];
            unsigned char* sep_buff_p;
            unsigned int sep_buff_l;
            // next pdu length
            unsigned int next_pdu_l;
            // next pdu received bytes
            unsigned int next_pdu_rb;
        };


	/**
	 * Decodes smpp message
	 * @param[in]   data            pointer to smpp bytes
	 * @param[in]   data_length     number of bytes contained in smpp packet
         * @param[out]  pdu             Pointer to SMPP PDU object
         * @param[in]   smpp_pool       Pointer to SMPP pool
	 * @return      Zero (0) for success or error code
	 */
	int decode(unsigned char* data, unsigned int data_length, SmppPDU* pdu, SMPPPool* smpp_pool);


}




#endif /* SMPP_H_ */
