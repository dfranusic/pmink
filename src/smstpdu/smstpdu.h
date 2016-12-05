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
 * @file smstpdu.h
 * @brief pMINK sms-tpdu (3GPP TS 23.040, ETSI TS 123 038) library
 *
 */


#ifndef SMSTPDU_H_
#define SMSTPDU_H_

#include <inttypes.h>
#include <vector>
#include <map>

#define GN_BYTE_MASK ((1 << bits) - 1)

namespace smstpdu{

    /**
     * Type of number
     */
    enum TypeOfNumber {
	TON_UNKNOWN 		= 0x00,	/**< Unknown */
	TON_INTERNATIONAL 	= 0x10,	/**< International number */
	TON_NATIONAL 		= 0x20,	/**< National number */
	TON_NETWORK_SPECIFIC 	= 0x30,	/**< Network specific number */
	TON_SUBSCRIBER_NUMBER 	= 0x40,	/**< Subscriber number */
	TON_ALPHANUMERIC 	= 0x50,	/**< Alphanumeric, (coded according to 3GPP TS 23.038 [9] GSM 7-bit default alphabet) */
	TON_ABBREVIATED 	= 0x60,	/**< Abbreviated number */
	TON_RESERVED 		= 0x70	/**< Reserved for extension */
    };

    /**
     * Numbering plan
     */
    enum NumberingPlan {
	NP_UNKNOWN 		= 0x00,	/**< Unknown */
	NP_ISDN_TELEPHONE 	= 0x01,	/**< ISDN/telephone numbering plan (E.164 [17]/E.163[18]) */
	NP_DATA_X121 		= 0x03,	/**< Data numbering plan (X.121) */
	NP_TELEX 		= 0x04,	/**< Telex numbering plan */
	NP_LAND_MOBILE 		= 0x06,	/**< Land mobile numbering plan */
	NP_NATIONAL 		= 0x08,	/**< National numbering plan */
	NP_PRIVATE 		= 0x09,	/**< Private numbering plan */
	NP_ERMES 		= 0x0A,	/**< ERMES numbering plan (ETSI DE/PS 3 01-3) */
	NP_RESERVED 		= 0x0F	/**< Reserved for extension */

    };

    enum TPDUParamType {
	TPDU_PT_TP_MTI		= 0,	/**< TP-Message-Type-Indicator (TP-MTI) */
	TPDU_PT_TP_MMS 		= 1,	/**< TP-More-Messages-to-Send (TP-MMS) */
	TPDU_PT_TP_VPF 		= 2,	/**< TP-Validity-Period-Format (TP-VPF) */
	TPDU_PT_TP_SRI 		= 3,	/**< TP-Status-Report-Indication (TP-SRI) */
	TPDU_PT_TP_SRR 		= 4,	/**< TP-Status-Report-Request (TP-SRR) */
	TPDU_PT_TP_MR 		= 5,	/**< TP-Message-Reference (TP-MR) */
	TPDU_PT_TP_OA 		= 6,	/**< TP-Originating-Address (TP-OA) */
	TPDU_PT_TP_DA 		= 7,	/**< TP-Destination-Address (TP-DA) */
	TPDU_PT_TP_PID 		= 8,	/**< TP-Protocol-Identifier (TP-PID) */
	TPDU_PT_TP_DCS 		= 9,	/**< TP-Data-Coding-Scheme (TP-DCS) */
	TPDU_PT_TP_SCTS 	= 10,	/**< TP-Service-Centre-Time-Stamp (TP-SCTS) */
	TPDU_PT_TP_VP 		= 11,	/**< TP-Validity-Period (TP-VP) */
	TPDU_PT_TP_DT 		= 12,	/**< TP-Discharge-Time (TP-DT) */
	TPDU_PT_TP_RA 		= 13,	/**< TP-Recipient-Address (TP-RA) */
	TPDU_PT_TP_ST 		= 14,	/**< TP-Status (TP-ST) */
	TPDU_PT_TP_UDL 		= 15,	/**< TP-User-Data-Length (TP-UDL) */
	TPDU_PT_TP_RP 		= 16,	/**< TP-Reply-Path (TP-RP) */
	TPDU_PT_TP_MN 		= 17,	/**< TP-Message-Number (TP-MN) */
	TPDU_PT_TP_CT 		= 18,	/**< TP-Command-Type (TP-CT) */
	TPDU_PT_TP_CDL 		= 19,	/**< TP-Command-Data-Length (TP-CDL) */
	TPDU_PT_TP_CD 		= 20,	/**< TP-Command-Data (TP-CD) */
	TPDU_PT_TP_FCS 		= 21,	/**< TP-Failure-Cause (TP-FCS) */
	TPDU_PT_TP_UDHI 	= 22,	/**< TP-User-Data-Header-Indicator (TP-UDHI) */
	TPDU_PT_TP_UD 		= 23,	/**< TP-User Data (TP-UD) */
	TPDU_PT_TP_RD 		= 24,	/**< TP-Reject-Duplicates (TP-RD) */
	TPDU_PT_TP_SRQ 		= 25,	/**< TP-Status-Report-Qualifier (TP-SRQ) */
	TPDU_PT_TP_PI 		= 26,	/**< TP-Parameter-Indicator (TP-PI) */
	TPDU_PT_TP_UDH 		= 100,	/**< UDH header */
	TPDU_PT_TP_UDH_IE	= 101	/**< UDH Information Element */



    };

    /**
     * TP-MTI Type
     */
    enum TPMTIType {
	TPMTIT_UNKNOWN			= -1,	/**< Unknown */
	TPMTIT_SMS_DELIVER 		= 0,	/**< SMS-DELIVER */
	TPMTIT_SMS_SUBMIT 		= 1,	/**< SMS-SUBMIT */
	TPMTIT_SMS_STATUS_REPORT 	= 2	/**< SMS-STATS-REPORT */


    };

    /**
     * TP-Validity-Period-Format (TP-VPF) Type
     *
     */
    enum TPVPFType {
	TPVPFT_NOT_PRESENT		= 0x00,	/**< TP-VP field not present */
	TPVPFT_RELATIVE			= 0x10,	/**< TP-VP field present - relative format */
	TPVPFT_ENHANCED			= 0x08,	/**< TP-VP field present - enhanced format */
	TPVPFT_ABSOLUTE			= 0x18	/**< TP-VP field present - absolute format */


    };

    /**
     * TP-Protocol-Identifier (TP-PID) subsequent bits coding Type
     *
     */
    enum TPPIDFormatType {
	TPPIDFT_DEFAULT1		= 0x00,	/**< Default 1 */
	TPPIDFT_DEFAULT2		= 0x40,	/**< Default 2 */
	TPPIDFT_RESERVED		= 0x80,	/**< Reserved */
	TPPIDFT_SPECIFIC		= 0xc0	/**< Specific */

    };

    /**
     * TP-Protocol-Identifier (TP-PID) Interworking Type
     */
    enum TPPIDInterworkingType {
	TPPIDIT_NO_INTERWORKING_BUT_SME_TO_SME	= 0x00,	/**< no interworking, but SME-to-SME protocol */
	TPPIDIT_TELEMATIC_INTERWORKING		= 0x20	/**< telematic interworking */

    };

    /**
     * TP-Protocol-Identifier (TP-PID) Tlematic device
     */
    enum TPPIDTelematicDeviceType {
	TPPIDTDT_IMPLICIT_SPECIFIC_TO_THIS_SC		= 0x00,	/**< implicit - device type is specific to this SC, or can be concluded on the basis of the address */
	TPPIDTDT_TELEX					= 0x01,	/**< telex (or teletex reduced to telex format) */
	TPPIDTDT_GROUP_3_TELEFAX			= 0x02,	/**< group 3 telefax */
	TPPIDTDT_GROUP_4_TELEFAX			= 0x03,	/**< group 4 telefax */
	TPPIDTDT_VOICE_TELEPHONE			= 0x04,	/**< voice telephone (i.e. conversion to speech) */
	TPPIDTDT_ERMES					= 0x05,	/**< ERMES (European Radio Messaging System) */
	TPPIDTDT_NATIONAL_PAGING_SYSTEM			= 0x06,	/**< National Paging system (known to the SC) */
	TPPIDTDT_VIDEOTEX				= 0x07,	/**< Videotex (T.100 [20] /T.101 [21]) */
	TPPIDTDT_TELETEX_UNSPECIFIED			= 0x08,	/**< teletex, carrier unspecified */
	TPPIDTDT_TELETEX_PSPDN				= 0x09,	/**< teletex, in PSPDN */
	TPPIDTDT_TELETEX_CSPDN				= 0x0A,	/**< teletex, in CSPDN */
	TPPIDTDT_TELETEX_PSTN				= 0x0B,	/**< teletex, in analog PSTN */
	TPPIDTDT_TELETEX_ISDN				= 0x0C,	/**< teletex, in digital ISDN */
	TPPIDTDT_UCI					= 0x0D,	/**< UCI (Universal Computer Interface, ETSI DE/PS 3 01-3) */
	TPPIDTDT_RESERVED1				= 0x0E,	/**< Reserved */
	TPPIDTDT_RESERVED2				= 0x0F,	/**< Reserved */
	TPPIDTDT_MSG_HANDLING_FACILITY			= 0x10,	/**< a message handling facility (known to the SC) */
	TPPIDTDT_X_400_BASED_MSG_HANDLING_SYSTEM	= 0x11,	/**< any public X.400-based message handling system */
	TPPIDTDT_INTERNET_ELECTRONIC_MAIL		= 0x12,	/**< Internet Electronic Mail */
	TPPIDTDT_RESERVED3				= 0x13,	/**< Reserved */
	TPPIDTDT_RESERVED4				= 0x14,	/**< Reserved */
	TPPIDTDT_RESERVED5				= 0x15,	/**< Reserved */
	TPPIDTDT_RESERVED6				= 0x16,	/**< Reserved */
	TPPIDTDT_RESERVED7				= 0x17,	/**< Reserved */
	TPPIDTDT_SC_SPECIFIC_1				= 0x18,	/**< SC Specific */
	TPPIDTDT_SC_SPECIFIC_2				= 0x19,	/**< SC Specific */
	TPPIDTDT_SC_SPECIFIC_3				= 0x1A,	/**< SC Specific */
	TPPIDTDT_SC_SPECIFIC_4				= 0x1B,	/**< SC Specific */
	TPPIDTDT_SC_SPECIFIC_5				= 0x1C,	/**< SC Specific */
	TPPIDTDT_SC_SPECIFIC_6				= 0x1D,	/**< SC Specific */
	TPPIDTDT_SC_SPECIFIC_7				= 0x1E,	/**< SC Specific */
	TPPIDTDT_GSM_UMTS_MOBILE_STATION		= 0x1F	/**< A GSM/UMTS mobile station. The SC converts the SM from the received TP-Data-Coding-Scheme
								     to any data coding scheme supported by that MS (e.g. the default). */


    };

    /**
     * TP-Protocol-Identifier (TP-PID) Message related type
     */
    enum TPPIDMessageRelatedType {
	TPPIDMRT_SHORT_MESSAGE_TYPE0			= 0x00,	/**< Short Message Type 0 */
	TPPIDMRT_REPLACE_SHORT_MESSAGE_TYPE1		= 0x01,	/**< Replace Short Message Type 1 */
	TPPIDMRT_REPLACE_SHORT_MESSAGE_TYPE2		= 0x02,	/**< Replace Short Message Type 2 */
	TPPIDMRT_REPLACE_SHORT_MESSAGE_TYPE3		= 0x03,	/**< Replace Short Message Type 3 */
	TPPIDMRT_REPLACE_SHORT_MESSAGE_TYPE4		= 0x04,	/**< Replace Short Message Type 4 */
	TPPIDMRT_REPLACE_SHORT_MESSAGE_TYPE5		= 0x05,	/**< Replace Short Message Type 5 */
	TPPIDMRT_REPLACE_SHORT_MESSAGE_TYPE6		= 0x06,	/**< Replace Short Message Type 6 */
	TPPIDMRT_REPLACE_SHORT_MESSAGE_TYPE7		= 0x07,	/**< Replace Short Message Type 7 */
	TPPIDMRT_ENHANCED_MESSAGE_SERVICE		= 0x1E,	/**< Enhanced Message Service (Obsolete) */
	TPPIDMRT_RETURN_CALL_MESSAGE			= 0x1F,	/**< Return Call Message */
	TPPIDMRT_ANSI_136_R_DATA			= 0x3C,	/**< ANSI-136 R-DATA */
	TPPIDMRT_ME_DATA_DOWNLOAD			= 0x3D,	/**< ME Data download */
	TPPIDMRT_ME_DEPERSONALIZATION_SHORT_MESSAGE	= 0x3E,	/**< ME De-personalization Short Message */
	TPPIDMRT_U_SIM_DATA_DOWNLOAD			= 0x3F	/**< (U)SIM Data download */


    };

    /**
     * SMS Data Coding Scheme group type
     *
     */
    enum TPDCSGroupType {
	TPDCSGT_GENERAL					= 0x00,	/**< General Data Coding indication */
	TPDCSGT_AUTOMATIC_DELETION_GROUP		= 0x40,	/**< Message Marked for Automatic Deletion Group */
	TPDCSGT_MESSAGE_WAITING_DISCARD			= 0xC0,	/**< Message Waiting Indication Group: Discard Message */
	TPDCSGT_MESSAGE_WAITING_STORE			= 0xD0,	/**< Message Waiting Indication Group: Store Message GSM7 */
	TPDCSGT_MESSAGE_WAITING_STORE_UCS2		= 0xE0,	/**< Message Waiting Indication Group: Store Message UCS2 */
	TPDCSGT_DATA_CODING_MESSAGE			= 0xF0	/**< Data coding/message class */

    };

    /**
     * DCS Alphabet type
     */
    enum TPDCSAlphabetType {
	TPDCSAT_GSM7					= 0x00,	/**< GSM 7 bit default alphabet */
	TPDCSAT_8BIT					= 0x04,	/**< 8 bit data */
	TPDCSAT_UCS2					= 0x08,	/**< UCS2 (16bit) [10] */
	TPDCSAT_RESERVED				= 0x0c	/**< Reserved */

    };

    /**
     * DCS Message class type
     */
    enum TPDCSMessageClassType {
	TPDCSMCT_CLASS0					= 0x00,	/**< Class 0 */
	TPDCSMCT_CLASS1					= 0x01,	/**< Class 1 Default meaning: ME-specific */
	TPDCSMCT_CLASS2					= 0x02,	/**< Class 2 (U)SIM specific message */
	TPDCSMCT_CLASS3					= 0x03	/**< Class 3 Default meaning: TE specific (see 3GPP TS 27.005 [8]) */
    };

    /**
     * DCS Indication type
     */
    enum TPDCSIndicationTypeType {
	TPDCSITT_VOICE_MAIL_MESSAGE_WAITING		= 0x00,	/**< Voicemail Message Waiting */
	TPDCSITT_FAX_MESSAGE_WAITING			= 0x01,	/**< Fax Message Waiting */
	TPDCSITT_EMAIL_MESSAGE_WAITING			= 0x02,	/**< Electronic Mail Message Waiting */
	TPDCSITT_OTHER_MESSAGE_WAITING			= 0x03	/**< Other Message Waiting */

    };

    /**
     * TP-Status (TP-ST) type
     *
     */
    enum TPSTStatusType {
	TPSTST_SM_RECEIVED_BY_SME			= 0x00,	/**< Short message received by the SME */
	TPSTST_SM_SC_UNABLE_TO_CONFIRM_DELIVERY		= 0x01,	/**< Short message forwarded by the SC to the SME but the SC is unable to confirm delivery */
	TPSTST_SM_REPLACED_BY_SC			= 0x02,	/**< Short message replaced by the SC */
	TPSTST_CONGESTION				= 0x20,	/**< Congestion */
	TPSTST_SME_BUSY					= 0x21,	/**< SME busy */
	TPSTST_NO_RESPONSE_FROM_SME			= 0x22,	/**< No response from SME */
	TPSTST_SERVICE_REJECTED				= 0x23,	/**< Service rejected */
	TPSTST_TEMP_QOS_NA				= 0x24,	/**< Quality of service not available */
	TPSTST_ERR_IN_SME				= 0x25,	/**< Error in SME */
	TPSTST_REMOTE_PROC_ERR				= 0x40,	/**< Remote procedure error */
	TPSTST_INCOMPATIBLE_DEST			= 0x41,	/**< Incompatible destination */
	TPSTST_CONN_REJECTED_BY_SME			= 0x42,	/**< Connection rejected by SME */
	TPSTST_NOT_OBTAINABLE				= 0x43,	/**< Not obtainable */
	TPSTST_PERM_QOS_NA				= 0x44,	/**< Quality of service not available */
	TPSTST_NO_INTERWORKING				= 0x45,	/**< No interworking available */
	TPSTST_SM_VALIDITY_EXPIRED			= 0x46,	/**< SM Validity Period Expired */
	TPSTST_SM_DEL_BY_ORIG_SME			= 0x47,	/**< SM Deleted by originating SME */
	TPSTST_SM_DEL_BY_SC_ADMIN			= 0x48,	/**< SM Deleted by SC Administration */
	TPSTST_SM_DOES_NOT_EXIST			= 0x49,	/**< SM does not exist */
	TPSTST_CONGESTION_2				= 0x60,	/**< Congestion */
	TPSTST_SME_BUSY_2				= 0x61,	/**< SME busy */
	TPSTST_NO_RESPONSE_FROM_SME_2			= 0x62,	/**< No response from SME */
	TPSTST_SERVICE_REJECTED_2			= 0x63,	/**< Service rejected */
	TPSTST_TEMP_QOS_NA_2				= 0x64,	/**< Quality of service not available */
	TPSTST_ERR_IN_SME_2				= 0x65,	/**< Error in SME */

    };

    /**
     * TP-Command-Type (TP-CT) type
     *
     */
    enum TPCTOpTypeType {
	TPCTOT_PREV_SUBM_MSG_RELATED			= 0x00,	/**< Enquiry relating to previously submitted short message */
	TPCTOT_CANCEL_STATUS_REPORT_REQ			= 0x01,	/**< Cancel Status Report Request relating to previously submitted short message */
	TPCTOT_DELETE_PREV_SUBM				= 0x02,	/**< Delete previously submitted Short Message */
	TPCTOT_ENABLE_STATUS_REPORT_REQ			= 0x03	/**< Enable Status Report Request relating to previously submitted short message */

    };

    /**
     * TP-Failure-Cause (TP-FCS) type
     *
     */
    enum TPFCSErrorType {
	TPFCSET_TELEMATIC_INTERWORKING_NOT_SUPPORTED	= 0x80,	/**< Telematic interworking not supported */
	TPFCSET_SM_TYPE_0_NOT_SUPPORTED			= 0x81,	/**< Short message Type 0 not supported */
	TPFCSET_CANNOT_REPLACE_SM			= 0x82,	/**< Cannot replace short message */
	TPFCSET_UNSPECIFIED_TPPPID_ERR			= 0x8f,	/**< Unspecified TP-PID error */
	TPFCSET_DCS_NOT_SUPPORTED			= 0x90,	/**< Data coding scheme (alphabet) not supported */
	TPFCSET_MSG_CLASS_NOT_SUPPORTED			= 0x91,	/**< Message class not supported */
	TPFCSET_UNSPECIFIED_TPDCS_ERR			= 0x9f,	/**< Unspecified TP-DCS error */
	TPFCSET_CMD_CANNOT_BE_ACTIONED			= 0xa0,	/**< Command cannot be actioned */
	TPFCSET_CMD_UNSUPPORTED				= 0xa1,	/**< Command unsupported */
	TPFCSET_UNSPECIFIED_TP_CMD_ERR			= 0xaf,	/**< Unspecified TP-Command error */
	TPFCSET_TPDU_NOT_SUPPORTED			= 0xb0,	/**< TPDU not supported */
	TPFCSET_SC_BUSY					= 0xc0,	/**< SC busy */
	TPFCSET_NO_SC_SUBSCRIPTION			= 0xc1,	/**< No SC subscription */
	TPFCSET_SC_SYSTEM_FAILURE			= 0xc2,	/**< SC system failure */
	TPFCSET_INVALID_SME_ADDRESS			= 0xc3,	/**< Invalid SME address */
	TPFCSET_DEST_SME_BARRED				= 0xc4,	/**< Destination SME barred */
	TPFCSET_SM_REJECTED_DUPLICATE_SM		= 0xc5,	/**< SM Rejected-Duplicate SM */
	TPFCSET_TPVPF_NOT_SUPPORTED			= 0xc6,	/**< TP-VPF not supported */
	TPFCSET_TPVP_NOT_SUPPORTED			= 0xc7,	/**< TP-VP not supported */
	TPFCSET_SIM_SMS_STORAGE_FULL			= 0xd0,	/**< (U)SIM SMS storage full */
	TPFCSET_NO_SIM_STORAGE_CAPABILITY		= 0xd1,	/**< No SMS storage capability in (U)SIM */
	TPFCSET_ERROR_IN_MS				= 0xd2,	/**< Error in MS */
	TPFCSET_MEM_CAPACITY_EXCEEDED			= 0xd3,	/**< Memory Capacity Exceeded */
	TPFCSET_SIM_APP_TOOLKIT_BUSY			= 0xd4,	/**< (U)SIM Application Toolkit Busy */
	TPFCSET_SIM_DATA_DL_ERR				= 0xd5,	/**< (U)SIM data download error */
	TPFCSET_UNSPECIFIED_ERR				= 0xff	/**< Unspecified error cause */
    };

    /**
     * UDH Information Element (IE) type
     */
    enum TPUDHIEType {
	TPUDHIET_CONCATENATED_8BIT_REF			= 0x00,	/**< Concatenated short messages, 8-bit reference number */
	TPUDHIET_SPECIAL_SMS_INDICATION			= 0x01,	/**< Special SMS Message Indication */
	TPUDHIET_RESERVED				= 0x02,	/**< Reserved */
	TPUDHIET_VAL_NOT_USED				= 0x03,	/**< Value not used to avoid misinterpretation as <LF> character */
	TPUDHIET_APP_PORT_ADDR_SCHEME_8BIT		= 0x04,	/**< Application port addressing scheme, 8 bit address */
	TPUDHIET_APP_PORT_ADDR_SCHEME_16BIT		= 0x05,	/**< Application port addressing scheme, 16 bit address */
	TPUDHIET_SMSC_CTRL_PARAMS			= 0x06,	/**< SMSC Control Parameters */
	TPUDHIET_UDH_SRC_INDICATOR			= 0x07,	/**< UDH Source Indicator */
	TPUDHIET_CONCATENATED_16BIT_REF			= 0x08,	/**< Concatenated short message, 16-bit reference number */
	TPUDHIET_WIRELESS_CTRL_MSG_PROTOCOL		= 0x09,	/**< Wireless Control Message Protocol */
	TPUDHIET_TEXT_FORMATTING			= 0x0a,	/**< Text Formatting */
	TPUDHIET_PREDEFINED_SOUND			= 0x0b,	/**< Predefined Sound */
	TPUDHIET_USER_DEFINED_SOUND			= 0x0c,	/**< User Defined Sound (iMelody max 128 bytes) */
	TPUDHIET_PREDEFINED_ANIMATION			= 0x0d,	/**< Predefined Animation */
	TPUDHIET_LARGE_ANIMATION			= 0x0e,	/**< Large Animation (16*16 times 4 = 32*4 =128 bytes) */
	TPUDHIET_SMALL_ANIMATION			= 0x0f,	/**< Small Animation (8*8 times 4 = 8*4 =32 bytes) */
	TPUDHIET_LARGE_PICTURE				= 0x10,	/**< Large Picture (32*32 = 128 bytes) */
	TPUDHIET_SMALL_PICTURE				= 0x11,	/**< Small Picture (16*16 = 32 bytes) */
	TPUDHIET_VARIABLE_PICTURE			= 0x12,	/**< Variable Picture */
	TPUDHIET_RFS822_MAIL_HEADER			= 0x20	/**< RFC 822 E-Mail Header */
    };

    /**
     * TP-Status-Report-Qualifier (TP-SRQ) type
     *
     */
    enum TPSRQType {
	TPSRQT_RES_OF_SUBMIT				= 0x00,	/**< The SMS-STATUS-REPORT is the result of a SMS-SUBMIT */
	TPSRQT_RES_OF_COMMAND				= 0x20	/**< The SMS-STATUS-REPORT is the result of an SMS-COMMAND e.g. an Enquiry */


    };

    /**
     * TPDU Type
     */
    enum TPDUType {
	TPDUT_UNKNOWN					= -1,	/**< UNKNOWN */
	TPDUT_SMS_DELIVER				= 0x00,	/**< SMS-DELIVER */
	TPDUT_SMS_DELIVER_REPORT			= 0x01,	/**< SMS-DELIVER-REPORT */
	TPDUT_SMS_SUBMIT				= 0x02,	/**< SMS-SUBMIT */
	TPDUT_SMS_SUBMIT_REPORT				= 0x03,	/**< SMS-SUBMIT-REPORT */
	TPDUT_SMS_STATUS_REPORT				= 0x04,	/**< SMS-STATUS-REPORT */
	TPDUT_SMS_COMMAND				= 0x05	/**< SMS-COMMAND */

    };

    /**
     * TPDU Direction Type
     */
    enum TPDUDirectionType {
	TPDUD_UNKNOWN					= -1,	/**< UNKNOWN */
	TPDUD_SC_MS					= 0,	/**< Service Centre 	--> Mobile Subscriber */
	TPDUD_MS_SC					= 1	/**< Mobile Subscriber 	--> Service Centre */
    };

    /**
     * Base TPDU Param class
     */
    class TPDUParam {
    public:

	/**
	 * Constructor
	 * @param[in]		type		TPDU Param type
	 */
	TPDUParam(TPDUParamType _type);

	/**
	 * Destructor
	 */
	virtual ~TPDUParam();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	virtual int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU Param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	virtual void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	uint32_t length;			/**< Size of TPDU Param data */
	TPDUParamType type;			/**< Type of TPDU Param */
	unsigned char* value;			/**< TPDU Param data */

    };


    /**
     * Address field base class
     *
     * Address fields used by SM-RL are specified in 3GPP TS 24.011 [13] and 3GPP TS 29.002 [15].
     * Each address field of the SM-TL consists of the following sub-fields: An Address-Length field of one octet, a
     * Type-of-Address field of one octet, and one Address-Value field of variable length;
     *
     * The Address-Length (length_digits) field is an integer representation of the number of useful semi-octets within
     * the Address-Value * field, i.e. excludes any semi octet containing only fill bits.
     *
     *
     */
    class AddressField: public TPDUParam {
    public:
	/**
	 * Constructor
	 * @param[in]		_type		TPDU Param type
	 */
	AddressField(TPDUParamType _type);

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	uint8_t length_digits;			/**< Number of semi-octets in address_value */
	TypeOfNumber type_of_number;		/**< Type of number */
	NumberingPlan numbering_plan;		/**< Numbering plan */
	unsigned char* address_value;		/**< Address digits/text */


    };

    /**
     * TP-Message-Type-Indicator (TP-MTI) TPDU Param
     *
     * The TP-Message-Type-Indicator is a 2-bit field, located within bits no 0 and 1 of the first octet of all PDU
     *
     */
    class TP_MessageTypeIndicator: public TPDUParam {
    public:

	/**
	 * Default constructor
	 */
	TP_MessageTypeIndicator();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	TPMTIType mti_type;			/**< MTI type */

    };

    /**
     * TP-More-Messages-to-Send (TP-MMS) TPDU Param
     *
     * The TP-More-Messages-to-Send is a 1-bit field, located within bit no 2 of the first octet of
     * SMS-DELIVER and SMS-STATUS-REPORT
     */
    class TP_MoreMessagestoSend: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_MoreMessagestoSend();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool msg_waiting;			/**< More mwssages waiting flag */

    };

    /**
     * TP-Validity-Period-Format (TP-VPF) TPDU Param
     *
     * The TP-Validity-Period-Format is a 2-bit field, located within bit no 3 and 4 of the first octet of SMS-SUBMIT
     *
     */
    class TP_ValidityPeriodFormat: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_ValidityPeriodFormat();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	TPVPFType vpf_type;			/**< VPF type */

    };

    /**
     * TP-Status-Report-Indication (TP-SRI) TPDU Param
     *
     * The TP-Status-Report-Indication is a 1-bit field, located within bit no. 5 of the first octet of SMS-DELIVER
     *
     */
    class TP_StatusReportIndication: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_StatusReportIndication();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool status_report;			/**< Status report flag */



    };

    /**
     * TP-Status-Report-Request (TP-SRR) TPDU Param
     *
     * The TP-Status-Report-Request is a 1-bit field, located within bit no. 5 of the first octet of
     * SMS-SUBMIT and SMS-COMMAND
     *
     */
    class TP_StatusReportRequest: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_StatusReportRequest();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool status_requested;			/**< Status report flag */


    };

    /**
     * TP-Message-Reference (TP-MR) TPDU Param
     *
     * The TP-Message-Reference field gives an integer representation of a reference number of the SMS-SUBMIT or
     * SMS-COMMAND submitted to the SC by the MS. The MS increments TP-Message-Reference by 1 for each
     * SMS-SUBMIT or SMS-COMMAND being submitted. The value to be used for each SMS-SUBMIT is obtained by
     * reading the Last-Used-TP-MR value from the SMS Status data field in the (U)SIM (see GSM TS 11.11 [16] and 3GPP
     * TS 31.102 [30]) and incrementing this value by 1. After each SMS-SUBMIT has been submitted to the network, the
     * Last-Used-TP-MR value in the (U)SIM is updated with the TP-MR that was used in the SMS-SUBMIT operation. The
     * reference number may possess values in the range 0 to 255. The value in the TP-MR assigned by the MS is the same
     * value which is received at the SC.
     *
     * In the case where no response or an RP-ERROR with an appropriate cause value (see 3GPP TS 24.011 [13]) is received
     * in response to an SMS-SUBMIT or SMS-COMMAND, then the MS shall automatically repeat the SMS-SUBMIT or
     * SMS-COMMAND but must use the same TP-MR value and set the TP-RD bit to 1 (see 9.2.3.25). The number of times
     * the MS automatically repeats the SMS-SUBMIT or SMS-COMMAND shall be in the range 1 to 3 but the precise
     * number is an implementation matter. The automatic repeat mechanism should be capable of being disabled through
     * MMI.
     *
     * If all automatic attempts fail (or in the case of no automatic attempts the first attempt fails), the user shall be informed.
     * The failed message shall be stored in the mobile in such a way that the user can request a retransmission using the same
     * TP-MR value, without the need to re-enter any information. Such storage need only be provided for a single failed
     * message, i.e. the one most recently attempted.
     * The SC should discard an SMS-SUBMIT or SMS-COMMAND which has the TP-RD bit set to a 1 and which has the
     * same TP-MR value as the previous SMS-SUBMIT or SMS-COMMAND received from the same originating address.
     * In the case of a discarded SMS-SUBMIT or SMS-COMMAND, the SC should respond with an RP-ERROR, in which
     * case the RP-ERROR shall include a SMS-SUBMIT-REPORT with TP-FCS indicating “SM Rejected – Duplicate SM”.
     * In some cases, for backward compatibility with earlier phases and versions of this specification, the SC may be
     * configured to respond with an RP-ACK..
     * The SMS-STATUS-REPORT also contains a TP-Message-Reference field. The value sent to the MS shall be the same
     * as the TP-Message-Reference value generated by the MS in the earlier SMS-SUBMIT or SMS-COMMAND to which
     * the status report relates.
     *
     *
     */
    class TP_MessageReference: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_MessageReference();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	uint8_t reference_number;		/**< Ref number */

    };

    /**
     * TP-Originating-Address (TP-OA) TPDU Param
     *
     * The TP-Originating-Address field is formatted according to the formatting rules of address fields.
     *
     */
    class TP_OriginatingAddress: public AddressField {
    public:
	/**
	 * Default constructor
	 */
	TP_OriginatingAddress();
    };

    /**
     * TP-Destination-Address (TP-DA) TPDU Param
     *
     * The The TP-Destination-Address field is formatted according to the formatting rules of address fields.
     *
     */
    class TP_DestinationAddress: public AddressField {
    public:
	TP_DestinationAddress();
    };

    /**
     * TP-Protocol-Identifier (TP-PID) TPDU Param
     *
     * The TP-Protocol-Identifier parameter serves the purposes indicated in clause 3.2.3. It consists of one octet.
     * 3.2.3 Protocol-Identifier
     *
     * The Protocol-Identifier is the information element by which the SM-TL either refers to the higher layer protocol being
     * used, or indicates interworking with a certain type of telematic device.
     * The Protocol-Identifier information element makes use of a particular field in the message types SMS-SUBMIT, SMS-
     * SUBMIT-REPORT for RP-ACK, SMS-DELIVER DELIVER, SMS-DELIVER-REPORT for RP-ACK,
     * SMS_STATUS_REPORT and SMS-COMMAND TP-Protocol-Identifier (TP-PID).
     *
     */
    class TP_ProtocolIdentifier: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_ProtocolIdentifier();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	TPPIDFormatType format;				/**< Format type */
	TPPIDInterworkingType interworking;		/**< Interworking flag */
	TPPIDTelematicDeviceType telematic_device;	/**< Telematic device type */
	TPPIDMessageRelatedType message_related;	/**< Message related type */

    };


    /**
     * TP-Data-Coding-Scheme (TP-DCS) Base class
     *
     * The TP-Data-Coding-Scheme is defined in 3GPP TS 23.038 [9].
     *
     *
     */
    class TP_DataCodingScheme: public TPDUParam {
    public:
	/**
	 * Constructor
	 * @param[in]		_dcs_grou[	DCS Group type
	 */
	TP_DataCodingScheme(TPDCSGroupType _dcs_grup);

	/**
	 * Destructor
	 */
	virtual ~TP_DataCodingScheme();

	TPDCSGroupType dcs_group;		/**< DCS group */
	TPDCSAlphabetType alphabet;		/*< Alphabet */


    };

    /**
     * General Data Coding DCS Group
     *
     */
    class TP_DCSGeneral: public TP_DataCodingScheme {
    public:
	/**
	 * Default constructor
	 */
	TP_DCSGeneral();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool compressed;			/**< Compression flag */
	bool has_message_class;			/**< Messge class included flag */
	TPDCSMessageClassType message_class;	/**< Message class */

    };

    /**
     * Data coding/message class DCS Group
     *
     */
    class TP_DCSDataCoding: public TP_DataCodingScheme {
    public:
	/**
	 * Default constructor
	 */
	TP_DCSDataCoding();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	TPDCSMessageClassType message_class;	/**< Message class */


    };

    /**
     * Message Waiting Indication Group: Discard Message DCS Group
     *
     */
    class TP_DCSDiscard: public TP_DataCodingScheme {
    public:
	/**
	 * Default constructor
	 */
	TP_DCSDiscard();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool indication;			/**< Indication flag */
	TPDCSIndicationTypeType indication_type;/**< Indication type */

    };

    /**
     * Message Waiting Indication Group: Store Message DCS Group (GSM7)
     *
     */
    class TP_DCSStore: public TP_DataCodingScheme {
    public:
	/**
	 * Default constructor
	 */
	TP_DCSStore();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool indication;			/**< Indication flag */
	TPDCSIndicationTypeType indication_type;/**< Indication type */

    };

    /**
     * Message Waiting Indication Group: Store Message DCS Group (UCS2)
     *
     */
    class TP_DCSStoreUCS2: public TP_DataCodingScheme {
    public:
	/**
	 * Default constructor
	 */
	TP_DCSStoreUCS2();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool indication;			/**< Indication flag */
	TPDCSIndicationTypeType indication_type;/**< Indication type */


    };

    /**
     * DCS Group wrapper class
     */
    class TP_DCSGroups {
    public:
	/**
	 * Get DCS group
	 * @param[in]		group_byte	DCS Group octet
	 * @return		Pointer to DCS group
	 */
	TP_DataCodingScheme* get(unsigned char group_byte);

	TP_DCSStoreUCS2 store_ucs2;		/**< Message Waiting Indication Group: Store Message DCS Group (UCS2) */
	TP_DCSStore store;			/**< Message Waiting Indication Group: Store Message DCS Group (GSM7) */
	TP_DCSDiscard discard;			/**< Message Waiting Indication Group: Discard Message DCS Group */
	TP_DCSDataCoding data_coding;		/**< Data coding/message class DCS Group */
	TP_DCSGeneral general;			/**< General Data Coding DCS Group */


    };

    /**
     * TP-Service-Centre-Time-Stamp (TP-SCTS) TPDU Param
     *
     */
    class TP_ServiceCentreTimeStamp: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_ServiceCentreTimeStamp();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	time_t timestamp;			/**< UNIX Timestamp */
	int gmt_offset;				/**< GMT offset in hours */

    };

    /**
     * TP-Validity-Period (TP-VP) TPDU Param
     *
     */
    class TP_ValidityPeriod: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_ValidityPeriod();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	time_t timestamp;			/**< UNIX Timestamp */
	int gmt_offset;				/**< GMT offset in hours */
	TPVPFType vpf_type;			/**< Vpf type */

    };

    /**
     * TP-Discharge-Time (TP-DT) TPDU Param
     *
     * The TP-Discharge-Time field indicates the time at which a previously submitted SMS-SUBMIT was successfully
     * delivered to or attempted to deliver to the recipient SME or disposed of by the SC.
     *
     * In the case of "transaction completed" the time shall be the time of the completion of the transaction. In the case of "SC
     * still trying to transfer SM" the time shall be the time of the last transfer attempt. In the case of "permanent or temporary
     * error - SC not making any more transfer attempts" the time shall be the time of either the last transfer attempt or the
     * time at which the SC disposed of the SM according to the Status outcome in TP-ST.
     *
     * The TP-Discharge-Time is given in semi-octet representation in a format identical to the TP-SCTS.
     *
     *
     */
    class TP_DischargeTime: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_DischargeTime();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	time_t timestamp;			/**< UNIX Timestamp */
	int gmt_offset;				/**< GMT offset in hours */

    };

    /**
     * TP-Recipient-Address (TP-RA) TPDU Param
     *
     * The TP-Recipient-Address field indicates the address of the SME that was the destination of the previously submitted
     * mobile originated short message being subject to the status report. The field is formatted according to the formatting
     * rules of address fields.
     *
     */
    class TP_RecipientAddress: public AddressField {
    public:
	/**
	 * Default constructor
	 */
	TP_RecipientAddress();
    };

    /**
     * TP-Status (TP-ST) TPDU Param
     *
     * The TP-Status field indicates the status of a previously submitted SMS-SUBMIT and certain SMS COMMANDS for
     * which a Status -Report has been requested. It consists of one octet.
     *
     *
     */
    class TP_Status: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_Status();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool status_used;			/**< Status used flag */
	TPSTStatusType status;			/**< Status type */

    };

    /**
     * TP-User-Data-Length (TP-UDL) TPDU Param
     *
     * If the TP-User-Data is coded using the GSM 7 bit default alphabet, the TP-User-Data-Length field gives an integer
     * representation of the number of septets within the TP-User-Data field to follow. If the 7bit default-alphabet extension
     * mechanism is used within the TP-User-Data (see 3GPP TS 23.038 [9]), the actual number of characters in the message
     * shall be less than the number of septets. If a TP-User-Data-Header field is present, then the TP-User-Data-Length value
     * is the sum of the number of septets in the TP-User-Data-Header field (including any padding) and the number of septets
     * in the TP-User-Data field which follows. See figure 9.2.3.24 (a). If the TP-User-Data is coded using 8-bit data, the
     * TP-User-Data-Length field gives an integer representation of the number of octets within the TP-User-Data field to
     * follow. If a TP-User-Data-Header field is present, then the TP-User-Data-Length value is the sum of the number of
     * octets in the TP-User-Data-Header field and the number of octets in the TP-User-Data field which follows. See figure
     * 9.2.3.24 (b).
     *
     * If the TP-User-Data is coded using UCS2 [24] data, the TP-User-Data-Length field gives an integer representation of
     * the number of octets within the TP-User-Data field to follow. If a TP-User-Data-Header field is present, then the
     * TP-User-Data-Length value is the sum of the number of octets in the TP-User-Data-Header field and the number of
     * octets in the TP-User-Data field which follows. See figure 9.2.3.24 (b).
     *
     * If the TP-User-Data is coded using compressed GSM 7 bit default alphabet or compressed 8 bit data or compressed
     * UCS2 [24] data, the TP-User-Data-Length field gives an integer representation of the number of octets after
     * compression within the TP-User-Data field to follow. If a TP-User-Data-Header field is present, then the
     * TP-User-Data-Length value is the sum of the number of uncompressed octets in the TP-User-Data-Header field and the
     * number of octets in the compressed TP-User-Data field which follows. See figure 9.2.3.24 (c)
     *
     * For other Data Coding Schemes, see 3GPP TS 23.038 [9]. If this field is zero, the TP-User-Data field shall not be
     * present.
     *
     *
     */
    class TP_UserDataLength: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_UserDataLength();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	int udl;				/**< UDL value */

    };

    /**
     * TP-Reply-Path (TP-RP) TPDU Param
     *
     * The TP-Reply-Path is a 1-bit field, located within bit no 7 of the first octet of both SMS-DELIVER and SMS-SUBMIT
     *
     *
     */
    class TP_ReplyPath: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_ReplyPath();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool rp_set;				/**< RP set flag */


    };

    /**
     * TP-Message-Number (TP-MN) TPDU Param
     *
     * The TP-Message-Number is an 8-bit field allowing an MS to refer uniquely to an SM in the SC which that MS has
     * previously submitted. The TP-MN value is the TP-MR value of a previously submitted SM
     *
     */
    class TP_MessageNumber: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_MessageNumber();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	int msg_number;				/**< Msg number */


    };

    /**
     * TP-Command-Type (TP-CT) TPDU Param
     *
     */
    class TP_CommandType: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_CommandType();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	TPCTOpTypeType operation_type;		/**< Operation type */

    };

    /**
     * TP-Command-Data-Length (TP-CDL) TPDU Param
     *
     * The TP-Command-Data-Length field is used to indicate the number of octets contained within the
     * TP-Command-Data-field. If this field is set to zero, the TP-Command-Data field shall not be present.
     *
     *
     */
    class TP_CommandDataLength: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_CommandDataLength();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	int cmd_length;				/**< Command length */

    };


    /**
     * TP-Command-Data (TP-CD) TPDU Param
     *
     * The TP-Command-Data field contains data relating to the operation requested by the MS which is to be performed at
     * the SC. The maximum length of this field is 157 octets. The usage and provision of the optional TP-Command-Data
     * field shall be determined by the function selected by the TP-Command-Type field.
     *
     *
     */
    class TP_CommandData: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_CommandData();
    };

    /**
     * TP-Failure-Cause (TP-FCS) TPDU Param
     *
     * The TP-Failure-Cause field is used to report the reason for failure to transfer or process a short message. It consists of a
     * single octet.
     *
     *
     */
    class TP_FailureCause: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_FailureCause();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	TPFCSErrorType err_type;		/**< Error type */


    };

    /**
     * TP-User-Data-Header-Indicator (TP-UDHI) TPDU Param
     *
     * The TP-User-Data-Header-Indicator is a 1 bit field within bit 6 of the first octet.
     *
     */
    class TP_UserDataHeaderIndicator: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_UserDataHeaderIndicator();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool udh_present;			/**< UDH indicator flag */


    };

    /**
     * UDH Information element base class
     */
    class TP_UDHInformationElement: public TPDUParam {
    public:
	/**
	 * Constructor
	 * @param[in]		_typr		Information Element type
	 */
	TP_UDHInformationElement(TPUDHIEType _type);

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	TPUDHIEType ie_type;			/**< IE type */


    };

    /**
     * Concatenated message 8bit reference Information Element classs
     */
    class TP_UDHIEConc8BitRef: public TP_UDHInformationElement {
    public:
	/**
	 * Default constructor
	 */
	TP_UDHIEConc8BitRef();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	unsigned int msg_id;			/**< Message reference number */
	unsigned int msg_parts;			/**< Total number of message parts */
	unsigned int msg_part;			/**< Current message part number */


    };

    /**
     * Concatenated message 16bit reference Information Element classs
     */
    class TP_UDHIEConc16BitRef: public TP_UDHInformationElement {
    public:
	/**
	 * Default constructor
	 */
	TP_UDHIEConc16BitRef();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	unsigned int msg_id;			/**< Message reference number */
	unsigned int msg_parts;			/**< Total number of message parts */
	unsigned int msg_part;			/**< Current message part number */
    };

    /**
     * IE Pool item
     */
    class IEPoolItem {
    private:
	    TP_UDHInformationElement** pool;
	    TP_UDHInformationElement* next_free_item;
	    int total_count;
	    int free_count;
	    TP_UDHInformationElement* create_param(TPUDHIEType _type);
    public:
	    IEPoolItem();
	    ~IEPoolItem();
	    TPUDHIEType type;
	    TP_UDHInformationElement* request_item();
	    void init_pool();
	    void set_pool_size(int _total_count);

    };

    /**
     * IE Pool
     */
    class IEPool {
    private:
	    std::map<TPUDHIEType, IEPoolItem*> IE_POOL;
	    int ie_count;
	    void init_ie(TPUDHIEType _ie_type);
    public:
	    IEPool();
	    ~IEPool();
	    void set_pool_size(int _ie_count);
	    void init_pool();
	    TP_UDHInformationElement* request_ie(TPUDHIEType param_type);


    };


    /**
     * User Data Header class
     */
    class TP_UserDataHeader: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_UserDataHeader(int ie_pool_size = 5);

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	std::vector<TP_UDHInformationElement*> ie_lst;	/**< List of IE elements */
	IEPool ie_pool;					/**< IE element pool */

    };

    /**
     * TP-User Data (TP-UD) TPDU Param
     *
     */
    class TP_UserData: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_UserData();

	/**
	 * User Data Header UDH
	 */
	TP_UserDataHeader udh;
    };

    /**
     * TP-Reject-Duplicates (TP-RD) TPDU Param
     *
     * The TP-Reject-Duplicates is a 1 bit field located within bit 2 of the first octet of SMS-SUBMIT
     *
     *
     */
    class TP_RejectDuplicates: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_RejectDuplicates();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool reject;				/**< Reject flag */

    };

    /**
     * TP-Status-Report-Qualifier (TP-SRQ) TPDU Param
     *
     * The TP-Status-Report-Qualifier is a 1 bit field located within bit 5 of the first octet of SMS-STATUS-REPORT
     *
     *
     */
    class TP_StatusReportQualifier: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_StatusReportQualifier();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	TPSRQType result_of;			/**< Result of type */



    };

    /**
     * TP-Parameter-Indicator (TP-PI) TPDU Param
     *
     * he TP-Parameter-Indicator comprises a number of octets between 1 and n where each bit when set to a 1 indicates that
     * a particular optional parameter is present in the fields which follow. The TP-PI is present as part of the RP-User-Data in
     * the RP-ACK for both the SMS-DELIVER TPDU and the SMS-SUBMIT TPDU
     *
     *
     */
    class TP_ParameterIndicator: public TPDUParam {
    public:
	/**
	 * Default constructor
	 */
	TP_ParameterIndicator();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	bool tp_udl;				/**< tp-udl flag */
	bool tp_dcs;				/**< tp-dcs flag */
	bool tp_pid;				/**< tp-pid flag */


    };

    /**
     * TPDU Base class
     */
    class TPDU {
    public:
	/**
	 * Constructor
	 * @param[in]		_typr		PDU Type
	 */
	TPDU(TPDUType _type);

	/**
	 * Default destructor
	 */
	virtual ~TPDU();

	/**
	 * Decode TPDU Param
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 * @return		0 for success or error code
	 */
	virtual int init(unsigned char* data, int data_length) = 0;

	/**
	 * Encode TPDU param
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	virtual void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length) = 0;

	static TPDUType detect(unsigned char first_byte, TPDUDirectionType direction);

	TPDUType type;				/**< TPDU type */


    };


    /**
     * SMS-DELIVER PDU
     */
    class SMS_Deliver: public TPDU {
    public:
	/**
	 * Default constructor
	 */
	SMS_Deliver();

	/**
	 * Decode PDU
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode PDU
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	/**
	 * Get DCS pointer
	 * @param[in]		dcs_group	DCS Group type
	 * @return		Pointer to DCS Group object
	 */
	smstpdu::TP_DataCodingScheme* get_dcs_group(TPDCSGroupType dcs_group);

	TP_MessageTypeIndicator tp_mti;		/**< TP-MTI */
	TP_MoreMessagestoSend tp_mms;		/**< TP-MMS */
	TP_ReplyPath tp_rp;			/**< TP-RP */
	TP_UserDataHeaderIndicator tp_udhi;	/**< TP-UDHI */
	TP_StatusReportIndication tp_sri;	/**< TP-SRI */
	TP_OriginatingAddress tp_oa;		/**< TP-OA */
	TP_ProtocolIdentifier tp_pid;		/**< TP-PID */
	TP_DataCodingScheme* tp_dcs;		/**< TP-DCS */
	TP_ServiceCentreTimeStamp tp_scts;	/**< TP-SCTS */
	TP_UserDataLength tp_udl;		/**< TP-UDL */
	TP_UserData tp_ud;			/**< TP-UD */

    private:
	TP_DCSGroups dcs_groups;		/**< DCS Group wrapper */

    };

    /**
     * SMS-SUBMIT PDU
     */
    class SMS_Submit: public TPDU {
    public:
	/**
	 * Default constructor
	 */
	SMS_Submit();

	/**
	 * Decode PDU
	 * @param[in]		data		Pointer to TPDU Param data
	 * @param[in]		data_length	Size of TPDU Param data
	 * @return		0 for success of error code
	 */
	int init(unsigned char* data, int data_length);

	/**
	 * Encode PDU
	 * @param[out]		buffer		Pointer to output buffer
	 * @param[in]		buffer_length	Size of output buffer
	 * @param[in]		offset		Offset into output buffer
	 * @param[out]		result_length	Number of bytes written to output buffer
	 */
	void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	/**
	 * Get DCS pointer
	 * @param[in]		dcs_group	DCS Group type
	 * @return		Pointer to DCS Group object
	 */
	smstpdu::TP_DataCodingScheme* get_dcs_group(TPDCSGroupType dcs_group);


	TP_MessageTypeIndicator tp_mti;		/**< TP-MTI */
	TP_RejectDuplicates tp_rd;		/**< TP-RD */
	TP_ValidityPeriodFormat tp_vpf;		/**< TP-VPF */
	TP_ReplyPath tp_rp;			/**< TP-RP */
	TP_UserDataHeaderIndicator tp_udhi;	/**< TP-UDHI */
	TP_StatusReportRequest tp_srr;		/**< TP-SRR */
	TP_MessageReference tp_mr;		/**< TP-MR */
	TP_DestinationAddress tp_da;		/**< TP-DA */
	TP_ProtocolIdentifier tp_pid;		/**< TP-PID */
	TP_DataCodingScheme* tp_dcs;		/**< TP-DCS */
	TP_ValidityPeriod tp_vp;		/**< TP-VP */
	TP_UserDataLength tp_udl;		/**< TP-UDL */
	TP_UserData tp_ud;			/**< TP-UD */

    private:
	TP_DCSGroups dcs_groups;		/**< DCS Group wrapper */

    };


    /**
     * GSM 7bit Alphabet
     */
    class GsmAlphabet {
    public:
	/**
	 * Decode GSM7 character as UTF-16
	 * param[in]		val		GSM7 character code
	 * @return		UTF-16 representation of GSM7 character
	 */
	static uint16_t gsm_char_decode(unsigned char val);

	/**
	 * Decode GSM7 packed data as UTF-16 string
	 * @param[in]		data		Pointer to GSM7 packed data
	 * @param[in]		data_length	Packed data size in bytes
	 * @param[in]		septet_count	Packed data septet count (number of characters)
	 * @param[out]		output		Pointer to output buffer
	 * @param[in]		out_size	Output buffer size in bytes
	 * @param[in]		septet_offset	Trim septet_offset number of character from beginning
	 * 					(used when data is pre-padded to start on septet boundry)
	 */
	static int decode(	const unsigned char* data,
				uint32_t data_length,
				uint32_t septet_count,
				uint16_t* output,
				uint32_t out_size,
				int septet_offset = 0);



	/**
	 * Encode C string into GSM7 packed data
	 * @param[in]		data		Pointer to C string
	 * @param[in]		data_length	Size of C string (including NULL terminator)
	 * @param[in]		padding_bits	Padding bits for UDH header to enuse sms text starts on septet boundry
	 * @param[in]		out_size	Size of output buffer
	 * @param[out]		output		Pointer to output buffer
	 * @param[out]		out_used	Number of output buffer used for encoding
	 *
	 */
	static int encode(	const char* data,
				int data_length,
				int padding_bits,
				int out_size,
				unsigned char* output,
				int *out_used);


	/**
	 * Decode GSM7 extension
	 * @param[in]		GSM7 charater code
	 * @return		UTF-16 representation of GSM7 character
	 */
	static uint16_t decode_ext(unsigned char val);

	/**
	 * GSM7 Alphabet size constant
	 */
	static const int GSM_ALPHABET_SIZE 	= 128;

	/**
	 * GSM7 Escape character
	 */
	static const int GSM_CHAR_ESCAPE 	= 0x1b;

	/**
	 * GSM7 Alphabet
	 */
	static uint16_t GSM_DEFAULT_ALPHABET[GSM_ALPHABET_SIZE];

	/**
	 * Unpack GSM-7 string into octets
	 * @param[in]		offset		Offset in bits (when UDH is used)
	 * @param[in]		in_length	Size of GSM-7 data
	 * @param[in]		out_length	Size of output buffer
	 * @param[in]		input		Pointer to input buffer
	 * @param[out]		output		Pointer to output buffer
	 * @return		Number of bytes in output buffer
	 *
	 */
	static int unpack(unsigned int offset,
			  unsigned int in_length,
			  unsigned int out_length,
			  const uint8_t *input,
			  unsigned char *output);

	/**
	 * Pack GSM-7 string into septets
	 * @param[in]		data		Pointer to GSM-7 C string
	 * @param[in]		data_length	Size of GSM-7 data
	 * @param[in]		padding_bits	Padding bits for UDH header to ensure sms text starts on septet boundry
	 * @param[in]		out_size	Size of output buffer
	 * @param[out]		output		Pointer to output buffer
	 * @param[out]		out_used	Number of output buffer used for encoding
	 * @return		0 for success or error code
	 *
	 */
	static int pack(const char* data,
			int data_length,
			int padding_bits,
			int out_size,
			unsigned char* output,
			int *out_used);


    };

    /**
     * Decode semi-octet digit
     * @param[in]		data		Semi-octet encoded number
     * @return			Decoded value
     */
    uint8_t decode_semi_octet_digit(unsigned char data);

    /**
     * Decode TBCD encoded C string
     * @param[in]		data		TBCD encoded C string
     * @param[in]		data_length	Size of encoded string
     * @param[out]		output		Pointer to output buffer
     * @param[in]		out_size	Size of output buffer
     * @param[in]		skip_last	Skip last digit in case of ODD BCD encoding
     */
    void decode_tbcd_octets(unsigned char* data, unsigned int data_length, unsigned char* output, unsigned int out_size, bool skip_last = false);

    /**
     * Encode C string into TBCD form
     * @param[in]		data		C string to be encoded
     * @param[in]		data_length	Size of string
     * @param[output]		output		Pointer to output buffer
     * @return			Number of output buffer bytes used for TBCD encoding
     */
    int encode_tbcd_octets(unsigned char* data, unsigned int data_length, unsigned char* output);

    /**
     * Encode semi-octet digit
     * @param[in]		val		Number to be encoded
     * @return			Encoded value
     */
    uint8_t encode_semi_octet_digit(unsigned int val);

    /**
     * Decode TCBD octet
     * @param[in]		val		TCBD encoded octet
     * @return			Decoded character
     */
    char tbcd_decode(unsigned char val);

    /**
     * Encode TCBD octet
     * @param[in]		val		Character to encode
     * @return			TBCD encoded octet
     */
    unsigned char tbcd_encode(unsigned char val);


}




#endif /* SMSTPDU_H_ */
