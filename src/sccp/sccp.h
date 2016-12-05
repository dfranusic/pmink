/**
 * \file sccp.h
 * \brief SCCP library
 * \details SCCP encoder/decoder library based on ITU-T Q.713
 * \version 2.0
 * \date 2012
 * \author Damir Franusic <df@release14.org>
 */

#ifndef SCCP_H_
#define SCCP_H_
#include<vector>
#include<map>

namespace sccp {
	/**
	 * <b>3.12 Return Cause</b>\n
	 * In the unitdata service or extended unitdata service or long unitdata service message, the "return
	 * cause" parameter field is a one octet field containing the reason for message return. Bits 1-8 are
	 * coded as follows:
	 *
	 */
	enum ReturnCauseType {
		/** <b>00000000</b> - no translation for an address of such nature */
		RTRN_NO_TRANSLATION_FOR_ADDRESS_OF_SUCH_NATURE = 0x00,
		/** <b>00000001</b> - no translation for this specific address */
		RTRN_NO_TRANSLATION_FOR_THIS_SPECIFIC_ADDRESS = 0x01,
		/** <b>00000010</b> - subsystem congestion */
		RTRN_SUBSYSTEM_CONGESTION = 0x02,
		/** <b>00000011</b> - subsystem failure */
		RTRN_SUBSYSTEM_FAILURE = 0x03,
		/** <b>00000100</b> - unequipped user */
		RTRN_UNEQUIPPED_USER = 0x04,
		/** <b>00000101</b> - MTP failure */
		RTRN_MTP_FAILURE = 0x05,
		/** <b>00000110</b> - network congestion */
		RTRN_NETWORK_CONGESTION = 0x06,
		/** <b>00000111</b> - unqualified */
		RTRN_UNQUALIFIED = 0x07,
		/** <b>00000111</b> - error in message transport */
		RTRN_ERROR_IN_MESSAGE_TRANSPORT = 0x08,
		/** <b>00001001</b> - error in local processing */
		RTRN_ERROR_IN_LOCAL_PROCESSING = 0x09,
		/** <b>00001010</b> - destination cannot perform reassembly */
		RTRN_DESTINATION_CANNOT_PERFORM_REASSEMBLY = 0x0A,
		/** <b>00001011</b> - SCCP failure */
		RTRN_SCCP_FAILURE = 0x0B,
		/** <b>00001100</b> - hop counter violation */
		RTRN_HOP_COUNTER_VIOLATION = 0x0C,
		/** <b>00001101</b> - segmentation not supported */
		RTRN_SEGMENTATION_NOT_SUPPORTED = 0x0D,
		/** <b>00001110</b> - segmentation failure */
		RTRN_SEGMENTATION_FAILURE = 0x0E
	};
	/**
	 * <b>3.13 Reset Cause</b>\n
	 * The "reset cause" parameter field is a one octet field containing the reason for the resetting of the
	 * connection.\n
	 * The coding of the reset cause field is as follows
	 *
	 */
	enum ResetCauseType {
		/** <b>00000000</b> - end user originated */
		RST_END_USER_ORIGINATED = 0x00,
		/** <b>00000001</b> - SCCP user originated */
		RST_SCCP_USER_ORIGINATED = 0x01,
		/** <b>00000010</b> - message out of order – incorrect P(S) */
		RST_OUT_OF_ORDER_PS = 0x02,
		/** <b>00000011</b> - message out of order – incorrect P(R) */
		RST_OUT_OF_ORDER_PR = 0x03,
		/** <b>00000100</b> - remote procedure error – message out of window */
		RST_RPE_MESSAGE_OUT_OF_WINDOW = 0x04,
		/** <b>00000101</b> - remote procedure error – incorrect P(S) after (re)initialization */
		RST_RPE_INCORRECT_PS = 0x05,
		/** <b>00000110</b> - remote procedure error – general */
		RST_RPE_GENERAL = 0x06,
		/** <b>00000111</b> - remote end user operational */
		RST_REMOTE_END_USER_OPERATIONAL = 0x07,
		/** <b>00001000</b> - network operational */
		RST_NETWORK_OPERATIONAL = 0x08,
		/** <b>00001001</b> - access operational */
		RST_ACCESS_OPERATIONAL = 0x09,
		/** <b>00001010</b> - network congestion */
		RST_NETWORK_CONGESTION = 0x0A,
		/** <b>00001011</b> - reserved */
		RST_RESERVED = 0x0B,
		/** <b>00001100</b> - unqualified */
		RST_UNQUALIFIED = 0x0C
	};

	/**
	 * <b>3.13 Release Cause</b>\n
	 * The release cause parameter field is a one-octet field containing the reason for the release of the
	 * connection.\n
	 * The coding of the release cause field is as follows:
	 *
	 */
	enum ReleaseCauseType {
		/** <b>00000000</b> - end user originated */
		RLC_END_USER_ORIGINATED = 0x00,
		/** <b>00000001</b> - end user congestion */
		RLC_END_USER_CONGESTION = 0x01,
		/** <b>00000010</b> - end user failure */
		RLC_END_USER_FAILURE = 0x02,
		/** <b>00000011</b> - SCCP user originated */
		RLC_SCCP_USER_ORIGINATED = 0x03,
		/** <b>00000100</b> - remote procedure error */
		RLC_REMOTE_PROCEDUER_ERROR = 0x04,
		/** <b>00000101</b> - inconsistent connection data */
		RLC_INCONSISTENT_CONNECTION_DATA = 0x05,
		/** <b>00000110</b> - access failure */
		RLC_ACCESS_FAILURE = 0x06,
		/** <b>00000111</b> - access congestion */
		RLC_ACCESS_CONGESTION = 0x07,
		/** <b>00001000</b> - subsystem failure */
		RLC_SUBSYSTEM_FAILUER = 0x08,
		/** <b>00001001</b> - subsystem congestion */
		RLC_SUBSYSTEM_CONGESTION = 0x09,
		/** <b>00001010</b> - MTP failure */
		RLC_MTP_FAILURE = 0x0A,
		/** <b>00001011</b> - network congestion */
		RLC_NETWORK_CONGESTION = 0x0b,
		/** <b>00001100</b> - expiration of reset timer */
		RLC_EXPIRATION_OF_RESET_TIMER = 0x0C,
		/** <b>00001101</b> - expiration of receive inactivity timer */
		RLC_EXPIRATION_OF_RECEIVE_INACTIVITY_TIMER = 0x0D,
		/** <b>00001110</b> - reserved */
		RLC_RESERVED = 0x0E,
		/** <b>00001111</b> - unqualified */
		RLC_UNQUALIFIED = 0x0F,
		/** <b>00010000</b> - SCCP failure */
		RLC_SCCP_FAILURE = 0x10

	};

	/**
	 * <b>3.15 Refusal Cause</b>\n
	 * The refusal cause parameter field is a one octet field containing the reason for the refusal of the
	 * connection.\n
	 * The coding of the refusal cause field is as follows:
	 *
	 */
	enum RefusalCauseType {
		/** <b>00000000</b> - end user originated */
		RFS_END_USER_ORIGINATED = 0x00,
		/** <b>00000001</b> - end user congestion */
		RFS_END_USER_CONGESTION = 0x01,
		/** <b>00000010</b> - end user failure */
		RFS_END_USER_FAILURE = 0x02,
		/** <b>00000011</b> - SCCP user originated */
		RFS_SCCP_USER_ORIGINATED = 0x03,
		/** <b>00000100</b> - destination address unknown */
		RFS_DESTINATION_ADDRESS_UNKNOWN = 0x04,
		/** <b>00000101</b> - destination inaccessible */
		RFS_DESTINATION_INACCESSIBLE = 0x05,
		/** <b>00000110</b> - network resource – QOS not available/non-transient */
		RFS_QOS_NOT_AVAILABLE_NON_TRANASIENT = 0x06,
		/** <b>00000111</b> - network resource – QOS not available/transient */
		RFS_QOS_NOT_AVAILABLE_TRANSIENT = 0x07,
		/** <b>00001000</b> - access failure */
		RFS_ACCESS_FAILURE = 0x08,
		/** <b>00001001</b> - access congestion */
		RFS_ACCESS_CONGESTION = 0x09,
		/** <b>00001010</b> - subsystem failure */
		RFS_SUBSYSTEM_FAILURE = 0x0A,
		/** <b>00001011</b> - subsystem congestion */
		RFS_SUBSYSTEM_CONGESTION = 0x0B,
		/** <b>00001100</b> - expiration of the connection establishment timer */
		RFS_EXPIRATION_OF_CONN_ESTABLISHMENT_TIMER = 0x0C,
		/** <b>00001101</b> - incompatible user data */
		RFS_INCOMPATIBLE_USER_DATA = 0x0D,
		/** <b>00001110</b> - reserved */
		RFS_RESERVED = 0x0E,
		/** <b>00001111</b> - unqualified */
		RFS_UNQUALIFIED = 0x0F,
		/** <b>00010000</b> - hop counter violation */
		RFS_HOP_COUNTER_VIOLATION = 0x10,
		/** <b>00010001</b> - SCCP failure */
		RFS_SCCP_FAILURE = 0x11,
		/** <b>00010010</b> - no translation for an address of such nature */
		RFS_NO_TRANSLATION_FOR_ADDR_OF_SUCH_NATURE = 0x12,
		/** <b>00010011</b> - unequipped user */
		RFS_UNEQUIPPED_USER = 0x13

	};

	/**
	 * <b>3.14 Error Cause</b>\n
	 * The "error cause" parameter field is a one octet field containing the indication of the exact protocol
	 * error.\n
	 * The coding of the error cause field is as follows:
	 *
	 */
	enum ErrorCauseType {
		/** <b>00000000</b> - Local Reference Number (LRN) mismatch – unassigned destination LRN */
		UNASSIGNED_DESTINATION_LRN = 0x00,
		/** <b>00000001</b> - Local Reference Number (LRN) mismatch – inconsistent source LRN  */
		INCONSISTENT_SOURCE_LRN = 0x01,
		/** <b>00000010</b> - point code mismatch */
		POINT_CODE_MISMATCH = 0x02,
		/** <b>00000011</b> - service class mismatch */
		SERVICE_CLASS_MISMATCH = 0x03,
		/** <b>00000100</b> - unqualified */
		UNQUALIFIED = 0x04
	};

	/**
	 * <b>3.4.2.2 Subsystem number</b>\n
	 * The Subsystem Number (SSN) identifies an SCCP user function and, when provided, consists of one
	 * octet coded as follows:\n
	 *
	 */
	enum SubsystemNumber {
		/** <b>00000000</b> - SSN not known/not used */
		SSN_UNKNOWN = 0x00,
		/** <b>00000001</b> - SCCP management */
		SCCP_MANAGEMENT = 0x01,
		/** <b>00000010</b> - reserved for ITU-T allocation */
		ITU_T_RESERVED = 0x02,
		/** <b>00000011</b> - ISDN user part */
		ISDN_USER_PART = 0x03,
		/** <b>00000100</b> - OMAP (Operation, Maintenance and Administration Part) */
		OMAP = 0x04,
		/** <b>00000101</b> - MAP (Mobile Application Part) */
		MAP = 0x05,
		/** <b>00000110</b> - HLR (Home Location Register) */
		HLR = 0x06,
		/** <b>00000111</b> - VLR (Visitor Location Register) */
		VLR = 0x07,
		/** <b>00001000</b> - MSC (Mobile Switching Centre) */
		MSC = 0x08,
		/** <b>00001001</b> - EIC (Equipment Identifier Centre) */
		EIC = 0x09,
		/** <b>00001010</b> - AUC (Authentication Centre) */
		AUC = 0x0A,
		/** <b>00001011</b> - ISDN supplementary services */
		ISDN_SUPPLEMENTARY = 0x0B,
		/** <b>00001100</b> - reserved for international use */
		RESERVED_FOR_INTERNATIONAL_USE = 0x0C,
		/** <b>00001101</b> - broadband ISDN edge-to-edge applications */
		BROADBAND_ISDN_EDGE_TO_EDGE = 0x0D,
		/** <b>00001110</b> - TC test responder */
		TC_TEST_RESPONDER = 0x0E
	};

	/**
	 * <b>3.4.1 Address indicator</b>\n
	 * Bit 7 of the address indicator octet contains routing information identifying which address element
	 * shall be used for routing, and is encoded as follows:\n
	 *
	 */
	enum RoutingIndicator {
		/** <b>01000000</b> - Route on SSN */
		ROUTE_ON_SSN = 0x40,
		/** <b>00000000</b> - Route on GT */
		ROUTE_ON_GT = 0x00
	};

	/**
	 * <b>3.6 Protocol class</b>\n
	 * When bits 1-4 are coded to indicate a connection-oriented-protocol class (class 2, class 3), bits 5-8
	 * are spare.\n
	 * When bits 1-4 are coded to indicate a connectionless protocol class (class 0, class 1), bits 5-8 are
	 * used to specify message handling as follows:\n
	 *
	 */
	enum MessageHandling {
		/** <b>00000000</b> - no special options */
		NO_SPECIAL_OPTIONS = 0x00,
		/** <b>10000000</b> - return message on error */
		RETURN_MESSAGE_ON_ERROR = 0x80
	};

	/**
	 * <b>3.6 Protocol class</b>\n
	 * The "protocol class" parameter field is a one-octet parameter and is structured as follows:
	 * Bits 1-4 indicating protocol class are coded as follows:\n
	 *
	 */
	enum ProtocolClassType {
		/** <b>00000000</b> - class 0 */
		CLASS0 = 0x00,
		/** <b>00000001</b> - class 1 */
		CLASS1 = 0x01,
		/** <b>00000010</b> - class 2 */
		CLASS2 = 0x02,
		/** <b>00000011</b> - class 3 */
		CLASS3 = 0x03
	};

	/**
	 * <b>3.4.2.3.3 Global title indicator = 0011</b>\n
	 * The Numbering Plan (NP) is encoded as follows:\n
	 *
	 */
	enum NumberingPlan {
		/** <b>00000000</b> - unknown */
		UNKNOWN = 0x00,
		/** <b>00010000</b> - ISDN/telephony numbering plan (Recommendations E.163 and E.164) */
		ISDN_TELEPHONE = 0x10,
		/** <b>00100000</b> - generic numbering plan */
		GENERIC = 0x20,
		/** <b>00110000</b> - data numbering plan (Recommendation X.121) */
		DATA_X121 = 0x30,
		/** <b>01000000</b> - telex numbering plan (Recommendation F.69) */
		TELEX = 0x40,
		/** <b>01010000</b> - maritime mobile numbering plan (Recommendations E.210, E.211) */
		MARITIME = 0x50,
		/** <b>01100000</b> - and mobile numbering plan (Recommendation E.212) */
		LAND_MOBILE = 0x60,
		/** <b>01110000</b> - ISDN/mobile numbering plan (Recommendation E.214) */
		ISDN_MOBILE = 0x70,
		/** <b>11100000</b> - private network or network-specific numbering plan */
		PRIVATE = 0xE0

	};

	/**
	 * <b>3.4.2.3.1 Global title indicator = 0001</b>\n
	 * Bits 1 to 7 of octet 1 contain the Nature of Address Indicator (NAI) and are coded as follows:\n
	 *
	 */
	enum NatureOfAddress {
		/** <b>00000000</b> - unknown */
		NOA_UNKNOWN = 0x00,
		/** <b>00000001</b> - subscriber number */
		NOA_SUBSCRIBER_NUMBER = 0x01,
		/** <b>00000010</b> - reserved for national use */
		NOA_RESERVED_FOR_NATIONAL_USE = 0x02,
		/** <b>00000011</b> - national significant number */
		NOA_NATIONAL_SIGNIFICANT_NUMBER = 0x03,
		/** <b>00000100</b> - international number */
		NOA_INTERNATIONAL = 0x04
	};

	/**
	 * <b>3.4.2.3.3 Global title indicator = 0011</b>\n
	 * The Encoding Scheme (ES) is encoded as follows:\n
	 *
	 */
	enum EncodingScheme {
		/** <b>00000000</b> - unknown */
		UNKNOWN_ENCODING_SCHEME = 0x00,
		/** <b>00000001</b> - BCD, odd number of digits */
		BCD_ODD = 0x01,
		/** <b>00000010</b> - BCD, even number of digits */
		BCD_EVEN = 0x02,
		/** <b>00000011</b> - national specific */
		NATIONAL_SPECIFIC = 0x03

	};

	/**
	 * <b>3.4.1 Address indicator</b>\n
	 * Bits 3-6 of the address indicator octet contain the Global Title Indicator (GTI), which is encoded as
	 * follows:\n
	 *
	 */
	enum GlobalTitleIndicator {
		/** <b>00000000</b> - no global title included */
		GT_NO_TITLE = 0x00,
		/** <b>00000100</b> - global title includes nature of address indicator only */
		GT_NATURE_OF_ADDRESS_INDICATOR_ONLY = 0x04,
		/** <b>00001000</b> - global title includes translation type only */
		GT_TRANSLATION_TYPE_ONLY = 0x08,
		/** <b>00001100</b> - global title includes translation type, numbering plan and encoding scheme */
		GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING = 0x0C,
		/** <b>00010000</b> - global title includes translation type, numbering plan, encoding scheme and nature of address indicator */
		GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS = 0x10
	};

	/**
	 * <b>3 SCPP parameters</b>\n
	 * The parameter name codes are given in Table 2 with reference to the subclauses in which they are
	 * described.\n
	 *
	 */
	enum SCCPParameterType {
		/** <b>00000000</b> - End of optional parameters  */
		END_OF_OPTIONAL_PARAMETERS = 0x00,
		/** <b>00000001</b> - Destination local reference  */
		DESTINATION_LOCAL_REFERENCE = 0x01,
		/** <b>00000010</b> - Source local reference */
		SOURCE_LOCAL_REFERENCE = 0x02,
		/** <b>00000011</b> - Called party address */
		CALLED_PARTY_ADDRESS = 0x03,
		/** <b>00000100</b> - Calling party address */
		CALLING_PARTY_ADDRESS = 0x04,
		/** <b>00000101</b> - Protocol class */
		PROTOCOL_CLASS = 0x05,
		/** <b>00000110</b> - Segmenting/reassembling */
		SEGMENTING_REASSEMBLING = 0x06,
		/** <b>00000111</b> - Receive sequence number */
		RECEIVE_SEQUENCE_NUMBER = 0x07,
		/** <b>00001000</b> - Sequencing/segmenting */
		SEQUENCING_SEGMENTING = 0x08,
		/** <b>00001001</b> - Credit */
		CREDIT = 0x09,
		/** <b>00001010</b> - Release cause  */
		RELEASE_CAUSE = 0x0A,
		/** <b>00001011</b> - Return cause  */
		RETURN_CAUSE = 0x0B,
		/** <b>00001100</b> - Reset cause  */
		RESET_CAUSE = 0x0C,
		/** <b>00001101</b> - Error cause  */
		ERROR_CAUSE = 0x0D,
		/** <b>00001110</b> - Refusal cause  */
		REFUSAL_CAUSE = 0x0E,
		/** <b>00001111</b> - Data */
		DATA = 0x0F,
		/** <b>00010000</b> - Segmentation */
		SEGMENTATION = 0x10,
		/** <b>00010001</b> - Hop counter */
		HOP_COUNTER = 0x11,
		/** <b>00010010</b> - Importance */
		IMPORTANCE = 0x12,
		/** <b>00010011</b> - Long data */
		LONG_DATA = 0x13
	};

	/**
	 * <b>2.1 Coding of the message type</b>\n
	 *
	 */
	enum MessageType {
		/** <b>00000001</b> - CR Connection request */
		CR_CONNECTION_REQUEST = 0x01,
		/** <b>00000010</b> - CC Connection confirm */
		CC_CONNECTION_CONFIRM = 0x02,
		/** <b>00000011</b> - CREF Connection refused */
		CREF_CONNECTION_REFUSED = 0x03,
		/** <b>00000100</b> - RLSD Released */
		RLSD_RELEASED = 0x04,
		/** <b>00000101</b> - RLC Release complete */
		RLC_RELEASE_COMPLETE = 0x05,
		/** <b>00000110</b> - DT1 Data form 1 */
		DT1_DATA_FORM_1 = 0x06,
		/** <b>00000111</b> - DT2 Data form 2 */
		DT2_DATA_FORM_2 = 0x07,
		/** <b>00001000</b> - AK Data acknowledgement */
		AK_DATA_ACKNOWLEDGEMENT = 0x08,
		/** <b>00001001</b> - UDT Unitdata */
		UDT_UNITDATA = 0x09,
		/** <b>00001010</b> - UDTS Unitdata service */
		UDTS_UNITDATA_SERVICE = 0x0A,
		/** <b>00001011</b> - ED Expedited data */
		ED_EXPEDITED_DATA = 0x0B,
		/** <b>00001100</b> - EA Expedited data acknowledgement */
		EA_EXPEDITED_DATA_ACKNOWLEDGMENT = 0x0C,
		/** <b>00001101</b> - RSR Reset request */
		RSR_RESET_REQUEST = 0x0D,
		/** <b>00001110</b> - RSC Reset confirm */
		RSC_RESET_CONFIRM = 0x0E,
		/** <b>00001111</b> - ERR Protocol data unit error */
		ERR_PROTOCOL_DATA_UNIT_ERROR = 0x0F,
		/** <b>00010000</b> - IT Inactivity test */
		IT_INACTIVITY_TEST = 0x10,
		/** <b>00010001</b> - XUDT Extended unitdata */
		XUDT_EXTENDED_UNITDATA = 0x11,
		/** <b>00010010</b> - XUDTS Extended unitdata service */
		XUDTS_EXTENDED_UNIDATA_SERVICE = 0x12,
		/** <b>00010011</b> - LUDT Long unitdata */
		LUDT_LONG_UNIDATA = 0x13,
		/** <b>00010100</b> - LUDTS Long unitdata service */
		LUDTS_LONG_UNIDATA_SERVICE = 0x14
	};

	// fwd declaration of SCCP pool
	class SCCPPool;


	/**
	* \brief GlobalTitle: base GT class.\n
	* \details This class is a base class for all 4 GT classes.\n
	* \n
	* Bits 3-6 of the address indicator octet contain the Global Title Indicator (GTI), which is encoded as
	* follows:\n
	* <b>0000</b> - no global title included\n
	* <b>0001</b> - GT_NOA (global title includes nature of address indicator only)\n
	* <b>0010</b> - GT_TT (global title includes translation type only)\n
	* <b>0011</b> - GT_TTNPE (global title includes translation type, numbering plan, encoding scheme)\n
	* <b>0100</b> - GT_TTNPENOA (global title includes translation type, numbering plan, encoding scheme and nature of address indicator)\n
	* <b>0101 to 0111</b> - spare international\n
	* <b>1000 to 1110</b> - spare national\n
	* <b>1111</b> - reserved for extension\n
	*/
	class GlobalTitle {
	public:
		GlobalTitle();
		virtual ~GlobalTitle();
		int value_length;
		unsigned char* value;
		//bool new_value_mem;
		GlobalTitleIndicator type;
		static GlobalTitle* prepareNew(GlobalTitleIndicator type);
		static GlobalTitle* decode(unsigned char* data, int data_length, GlobalTitleIndicator gi, GlobalTitle* _gt, SCCPPool* _sccp_pool);
		virtual void init(unsigned char* data, int data_length);
		virtual void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief GT_TTNPENOA: Global title class representing [3.4.2.3.4 Global title indicator = 0100]
	 * \details
	<pre>
	3.4.2.3.4 Global title indicator = 0100

          8    7    6    5    4    3    2    1
                     Translation type                 octet 1
            Numbering plan      Encoding scheme       octet 2
         Spa      Nature of address indicator         octet 3
          re
                    Address information             octet 4 and
                                                      further
                                               FIGURE 11/Q.713
                                   Global title format for indicator 0100
               The field "translation type" is as described in  S  3.4.2.3.2.  The  fields
         "numbering plan" and "encoding scheme" are as described in S 3.4.2.3.3. The field
         Snature of address indicator" is as described in S 3.4.2.3.1.
               If the encoding scheme is binary coded decimal,  the  global  title  value,
         starting from octet 4, is encoded as shown in Figure 8/Q.713.
	</pre>
	 */
	class GT_TTNPENOA : public GlobalTitle {
	public:
		GT_TTNPENOA();
		~GT_TTNPENOA();
		int translation_type;
		NumberingPlan numbering_plan;
		EncodingScheme encoding_scheme;
		NatureOfAddress nature_of_address;
		unsigned char* address_information;
		int address_information_length;
		bool new_address_information_mem;;
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
		void init(unsigned char* data, int data_length);
	};

	/**
	 * \brief GT_TTNPE: Global title class representing [3.4.2.3.3 Global title indicator = 0011]
	 * \details
	 */
	class GT_TTNPE : public GlobalTitle {
	public:
		GT_TTNPE();
		~GT_TTNPE();
		int translation_type;
		NumberingPlan numbering_plan;
		EncodingScheme encoding_scheme;
		unsigned char* address_information;
		int address_information_length;
		bool new_address_information_mem;;
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
		void init(unsigned char* data, int data_length);
	};

	/**
	 * \brief GT_NOA: Global title class representing [3.4.2.3.1 Global title indicator = 0001]
	 * \details
	 */
	class GT_NOA : public GlobalTitle {
	public:
		GT_NOA();
		~GT_NOA();
		NatureOfAddress nature_of_address;
		bool odd_num_of_addr_signals;
		unsigned char* address_information;
		int address_information_length;
		bool new_address_information_mem;;
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
		void init(unsigned char* data, int data_length);
	};

	/**
	 * \brief GT_TT: Global title class representing [3.4.2.3.2 Global title indicator = 0010]
	 * \details
	 */
	class GT_TT : public GlobalTitle {
	public:
		GT_TT();
		~GT_TT();
		int translation_type;
		unsigned char* address_information;
		int address_information_length;
		bool new_address_information_mem;;
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
		void init(unsigned char* data, int data_length);
	};

	/**
	 * \brief SCCPParam: base SCPP parameter class
	 * \details
	 */
	class SCCPParam {
	public:
		SCCPParam();
		virtual ~SCCPParam();
		SCCPParameterType type;
		int byte_pos;
		int value_length;
		unsigned char* value;
		//bool new_value_mem;
		virtual void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		virtual void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);

	};

	/**
	 * \brief ProtocolClass: 3.6 Protocol class
	 * \details
	 */
	class ProtocolClass : public SCCPParam {
	public:
		ProtocolClass();
		~ProtocolClass();
		ProtocolClassType protocol_class;
		MessageHandling message_handling;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief CalledPA: 3.4 Called party address
	 * \details
	 */
	class CalledPA : public SCCPParam{
	public:
		CalledPA();
		~CalledPA();
		RoutingIndicator routing_indicator;
		GlobalTitleIndicator global_title_indicator;
		bool ssn_indicator;
		bool point_code_indicator;
		int point_code;
		SubsystemNumber subsystem_number;
		GlobalTitle* global_title;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief CalledPA: 3.5 Calling party address
	 * \details
	 */
	class CallingPA : public SCCPParam{
	public:
		CallingPA();
		~CallingPA();
		RoutingIndicator routing_indicator;
		GlobalTitleIndicator global_title_indicator;
		bool ssn_indicator;
		bool point_code_indicator;
		int point_code;
		SubsystemNumber subsystem_number;
		GlobalTitle* global_title;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief Credit: 3.10 Credit
	 * \details
	 */
	class Credit : public SCCPParam {
	public:
		Credit();
		int window_size;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief Dlr: 3.2 Destination local reference
	 * \details
	 */
	class Dlr : public SCCPParam {
	public:
		Dlr();
		int dlr_value;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief Eoop: 3.1 End of optional parameters
	 * \details
	 */
	class Eoop : public SCCPParam {
	public:
		Eoop();
	};

	/**
	 * \brief ErrorCause: 3.14 Error cause
	 * \details
	 */
	class ErrorCause : public SCCPParam {
	public:
		ErrorCause();
		ErrorCauseType error_cause;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief HopCounter: 3.18 Hop counter
	 * \details
	 */
	class HopCounter : public SCCPParam {
	public:
		HopCounter();
		int hop_value;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);

	};

	/**
	 * \brief Importance: 3.19 Importance
	 * \details
	 */
	class Importance : public SCCPParam {
	public:
		Importance();
		int importance_value;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);

	};

	/**
	 * \brief LongData: 3.20 Long data
	 * \details
	 */
	class LongData : public SCCPParam {
	public:
		LongData();
		// two octet length
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);

	};

	/**
	 * \brief ReceiveSeqNum: 3.8 Receive sequence number
	 * \details
	 */
	class ReceiveSeqNum : public SCCPParam {
	public:
		ReceiveSeqNum();
		int receive_seq_num;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};


	/**
	 * \brief RefusalCause: 3.15 Refusal cause
	 * \details
	 */
	class RefusalCause : public SCCPParam {
	public:
		RefusalCause();
		RefusalCauseType refusal_cause;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief ReleaseCause: 3.11 Release cause
	 * \details
	 */
	class ReleaseCause : public SCCPParam {
	public:
		ReleaseCause();
		ReleaseCauseType release_cause;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief ResetCause: 3.13 Reset cause
	 * \details
	 */
	class ResetCause : public SCCPParam {
	public:
		ResetCause();
		ResetCauseType reset_cause;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);

	};

	/**
	 * \brief ReturnCause: 3.12 Return cause
	 * \details
	 */
	class ReturnCause : public SCCPParam {
	public:
		ReturnCause();
		ReturnCauseType return_cause;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};


	/**
	 * \brief Segmentation: 3.17 Segmentation
	 * \details
	 */
	class Segmentation: public SCCPParam {
	public:
		Segmentation();
		bool is_first_segment;
		int class_type;
		int num_of_remaining_segments;
		int segmentation_local_ref_length;
		unsigned char* segmentation_local_ref;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief SegmReassm: 3.7 Segmenting/reassembling
	 * \details
	 */
	class SegmReassm : public SCCPParam {
	public:
		SegmReassm();
		bool more_data_indication;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief SeqSegm: 3.9 Sequencing/segmenting
	 * \details
	 */
	class SeqSegm : public SCCPParam {
	public:
		SeqSegm();
		int send_seq_num;
		int recv_seq_num;
		bool more_data_indication;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);

	};

	/**
	 * \brief Slr: 3.3 Source local reference
	 * \details
	 */
	class Slr : public SCCPParam {
	public:
		Slr();
		int slr_value;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, bool enc_type, bool enc_length);
	};

	/**
	 * \brief Data: 3.16 Data
	 * \details
	 */
	class Data : public SCCPParam {
	public:
		Data();
	};

	/**
	 * \brief SCCPPointer: representation of SCCP pointer
	 * \details
	 */
	class SCCPPointer {
	public:
		int value;
		int position;
		SCCPPointer(int _val, int _pos);
		SCCPPointer();
		virtual ~SCCPPointer();

	};


	/**
	 * \brief SCCPMessage: base SCCP message class
	 * \details
	 */
	class SCCPMessage {
	public:
		SCCPMessage();
		virtual ~SCCPMessage();
		MessageType type;
		SCCPPointer* pointer;
		int pointer_size;
		SCCPParam* get_optional_param(SCCPParameterType _param_type);
		int get_optional_params_count();
		virtual void init_new(SCCPPool* _sccp_pool);
		virtual void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool) = 0;
		virtual void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length) = 0;
//	protected:
		int byte_pos;
		std::vector<SCCPParam*> optional_params;
		void process_optional_params(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void encode_optional_params(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief RSR_ResetRequest: 4.14 Reset request (RSR)
	 * \details
	 */
	class RSR_ResetRequest : public SCCPMessage {
	public:
		RSR_ResetRequest();
		~RSR_ResetRequest();
		Dlr* destination_local_reference;
		Slr* source_local_reference;
		ResetCause* reset_cause;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief RSC_ResetConfirmation: 4.15 Reset confirmation (RSC)
	 * \details
	 */
	class RSC_ResetConfirmation : public SCCPMessage {
	public:
		RSC_ResetConfirmation();
		~RSC_ResetConfirmation();
		Dlr* destination_local_reference;
		Slr* source_local_reference;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief RLSD_Released: 4.5 Released (RLSD)
	 * \details
	 */
	class RLSD_Released : public SCCPMessage {
	public:
		RLSD_Released();
		~RLSD_Released();
		Dlr* destination_local_reference;
		Slr* source_local_reference;
		ReleaseCause* release_cause;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief RLC_ReleaseComplete: 4.5 Release complete (RLC)
	 * \details
	 */
	class RLC_ReleaseComplete : public SCCPMessage {
	public:
		RLC_ReleaseComplete();
		~RLC_ReleaseComplete();
		Dlr* destination_local_reference;
		Slr* source_local_reference;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief LUDTS_LongUnitdataService: 4.21 Long unitdata service (LUDTS)
	 * \details
	 */
	class LUDTS_LongUnitdataService : public SCCPMessage {
	public:
		LUDTS_LongUnitdataService();
		~LUDTS_LongUnitdataService();
		ReturnCause* return_cause;
		HopCounter* hop_counter;
		CalledPA* called_party_address;
		CallingPA* calling_party_address;
		LongData* long_data;
		Segmentation* segmentation;
		Importance* importance;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief LUDT_LongUnitdata: 4.20 Long unitdata (LUDT)
	 * \details
	 */
	class LUDT_LongUnitdata : public SCCPMessage {
	public:
		LUDT_LongUnitdata();
		~LUDT_LongUnitdata();
		ProtocolClass* protocol_class;
		HopCounter* hop_counter;
		CalledPA* called_party_address;
		CallingPA* calling_party_address;
		LongData* long_data;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief IT_InactivityTest: 4.17 Inactivity test (IT)
	 * \details
	 */
	class IT_InactivityTest : public SCCPMessage {
	public:
		IT_InactivityTest();
		~IT_InactivityTest();
		Dlr* destination_local_reference;
		Slr* source_local_reference;
		ProtocolClass* protocol_class;
		SeqSegm* sequencing_segmenting;
		Credit* credit;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief ERR_PduError: 4.16 Protocol data unit error (ERR)
	 * \details
	 */
	class ERR_PduError : public SCCPMessage {
	public:
		ERR_PduError();
		~ERR_PduError();
		Dlr* destination_local_reference;
		ErrorCause* error_cause;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief ED_ExpeditedData: 4.12 Expedited data (ED)
	 * \details
	 */
	class ED_ExpeditedData : public SCCPMessage {
	public:
		ED_ExpeditedData();
		~ED_ExpeditedData();
		Dlr* destination_local_reference;
		Data* data;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief EA_ExpeditedDataAck: 4.13 Expedited data acknowledgement (EA)
	 * \details
	 */
	class EA_ExpeditedDataAck : public SCCPMessage {
	public:
		EA_ExpeditedDataAck();
		~EA_ExpeditedDataAck();
		Dlr* destination_local_reference;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief DT2_DataForm2: 4.8 Data form 2 (DT2)
	 * \details
	 */
	class DT2_DataForm2 : public SCCPMessage {
	public:
		DT2_DataForm2();
		~DT2_DataForm2();
		Dlr* destination_local_reference;
		SegmReassm* segmenting_reassembling;
		Data* data;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief DT1_DataForm1: 4.7 Data form 1 (DT1)
	 * \details
	 */
	class DT1_DataForm1 : public SCCPMessage {
	public:
		DT1_DataForm1();
		~DT1_DataForm1();
		Dlr* destination_local_reference;
		SegmReassm* segmenting_reassembling;
		Data* data;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief CREF_ConnectionRefused: 4.4 Connection refused (CREF)
	 * \details
	 */
	class CREF_ConnectionRefused : public SCCPMessage {
	public:
		CREF_ConnectionRefused();
		~CREF_ConnectionRefused();
		Dlr* destination_local_reference;
		RefusalCause* refusal_cause;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief CR_ConnectionRequest: 4.2 Connection request (CR)
	 * \details
	 */
	class CR_ConnectionRequest : public SCCPMessage {
	public:
		CR_ConnectionRequest();
		~CR_ConnectionRequest();
		Slr* source_local_reference;
		ProtocolClass* protocol_class;
		CalledPA* called_party_address;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief CC_ConnectionConfirm: 4.3 Connection confirm (CC)
	 * \details
	 */
	class CC_ConnectionConfirm : public SCCPMessage {
	public:
		CC_ConnectionConfirm();
		~CC_ConnectionConfirm();
		Dlr* destination_local_reference;
		Slr* source_local_reference;
		ProtocolClass* protocol_class;
		CalledPA* called_party_address;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief AK_DataAcknowledgement: 4.9 Data acknowledgement (AK)
	 * \details
	 */
	class AK_DataAcknowledgement : public SCCPMessage {
	public:
		AK_DataAcknowledgement();
		~AK_DataAcknowledgement();
		Dlr* destination_local_reference;
		ReceiveSeqNum* receive_sequence_number;
		Credit* credit;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief UDTS_UnitDataService: 4.11 Unitdata service (UDTS)
	 * \details
	 */
	class UDTS_UnitDataService : public SCCPMessage {
	public:
		UDTS_UnitDataService();
		~UDTS_UnitDataService();
		ReturnCause* return_cause;
		CalledPA* called_party_address;
		CallingPA* calling_party_address;
		Data* data;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief UDT_UnitData: 4.10 Unitdata (UDT)
	 * \details
	 */
	class UDT_UnitData : public SCCPMessage {
	public:
		UDT_UnitData();
		~UDT_UnitData();
		ProtocolClass* protocol_class;
		CalledPA* called_party_address;
		CallingPA* calling_party_address;
		Data* data;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief XUDT_ExtendedUnitdata: 4.18 Extended unitdata (XUDT)
	 * \details
	 */
	class XUDT_ExtendedUnitdata : public SCCPMessage {
	public:
		XUDT_ExtendedUnitdata();
		~XUDT_ExtendedUnitdata();
		ProtocolClass* protocol_class;
		HopCounter* hop_counter;
		CalledPA* called_party_address;
		CallingPA* calling_party_address;
		Data* data;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief XUDTS_ExtendedUnitdataService: 4.19 Extended unitdata service (XUDTS)
	 * \details
	 */
	class XUDTS_ExtendedUnitdataService : public SCCPMessage {
	public:
		XUDTS_ExtendedUnitdataService();
		~XUDTS_ExtendedUnitdataService();
		ReturnCause* return_cause;
		HopCounter* hop_counter;
		CalledPA* called_party_address;
		CallingPA* calling_party_address;
		Data* data;
		void init(unsigned char* data, int data_length, SCCPPool* _sccp_pool);
		void init_new(SCCPPool* _sccp_pool);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};


	// SCCP pool classes
	class SCCPGTPoolItem {
	private:
		GlobalTitle** pool;
		GlobalTitle* next_free_item;
		int total_count;
		int free_count;
		GlobalTitle* create_gt(GlobalTitleIndicator _param_type);
	public:
		SCCPGTPoolItem();
		~SCCPGTPoolItem();
		GlobalTitleIndicator type;
		GlobalTitle* request_item();
		int get_free_count();
		void return_item(GlobalTitle* item);
		void init_pool();
		void set_pool_size(int _total_count);

	};

	class SCCPParamPoolItem {
	private:
		SCCPParam** pool;
		SCCPParam* next_free_item;
		int total_count;
		int free_count;
		SCCPParam* create_param(SCCPParameterType _param_type);
	public:
		SCCPParamPoolItem();
		~SCCPParamPoolItem();
		SCCPParameterType type;
		SCCPParam* request_item();
		int get_free_count();
		void return_item(SCCPParam* item);
		void init_pool();
		void set_pool_size(int _total_count);

	};

	class SCCPMessagePoolItem {
	private:
		SCCPMessage** pool;
		SCCPMessage* next_free_item;
		int total_count;
		int free_count;
		SCCPMessage* create_message(MessageType _param_type);
	public:
		SCCPMessagePoolItem();
		~SCCPMessagePoolItem();
		MessageType type;
		SCCPMessage* request_item();
		int get_free_count();
		void return_item(SCCPMessage* item);
		void init_pool();
		void set_pool_size(int _total_count);

	};

	class SCCPPool {
	private:
		std::map<MessageType, SCCPMessagePoolItem*> MESSAGE_POOL;
		std::map<SCCPParameterType, SCCPParamPoolItem*> PARAM_POOL;
		std::map<GlobalTitleIndicator, SCCPGTPoolItem*> GT_POOL;
		int param_count;
		int message_count;
		int free_message_count;
		int free_param_count;
		void init_param(SCCPParameterType _param_type);
		void init_message(MessageType _msg_type);
		void init_gt(GlobalTitleIndicator _gt_type);
	public:
		SCCPPool();
		~SCCPPool();
		void set_pool_size(int _param_count, int _message_count);
		void init_pool();
		SCCPMessage* request_message(MessageType msg_type);
		void return_message(SCCPMessage* message);
		SCCPParam* request_param(SCCPParameterType param_type);
		void return_param(SCCPParam* param);
		GlobalTitle* request_gt(GlobalTitleIndicator gt_type);
		void return_gt(GlobalTitle* gt);
		int get_free_message_count(MessageType msg_type);
		int get_free_param_count(SCCPParameterType param_type);


	};





	/**
	 * Creates sccp parameter object based on parameter type
	 * @param _param_type sccp parameter type
	 * @return Pointer to sccp parameter object or NULL if parameter not supported
	 */
	SCCPParam* get_param(SCCPParameterType _param_type);

	/**
	 * Decodes sccp message
	 * @param data pointer to sccp bytes
	 * @param data_length number of bytes contained in sccp packet
	 * @return Pointer to sccp m3ssage object or NULL no data available
	 */
	SCCPMessage* decode(unsigned char* data, int data_length, SCCPPool* _sccp_pool);

}


#endif /* SCCP_H_ */
