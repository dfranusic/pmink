/**
 * \file m2ua.h
 * \brief M2UA library
 * \details M2UA encoder/decoder library based on RFC 3331
 * \version 2.0
 * \date 2012
 * \author Damir Franusic <df@release14.org>
 */

#ifndef M2UA_H_
#define M2UA_H_
#include<vector>
namespace m2ua {

	/**
	 * <pre>
	 * 3.1.3  Message Class
	 *
	 *   The following List contains the valid Message Classes:
	 *
	 *   Message Class: 8 bits (unsigned integer)
	 *
	 *     0      Management (MGMT) Message [IUA/M2UA/M3UA/SUA]
	 *     1      Transfer Messages [M3UA]
	 *     2      SS7 Signalling Network Management (SSNM) Messages [M3UA/SUA]
	 *     3      ASP State Maintenance (ASPSM) Messages [IUA/M2UA/M3UA/SUA]
	 *     4      ASP Traffic Maintenance (ASPTM) Messages [IUA/M2UA/M3UA/SUA]
	 *     5      Q.921/Q.931 Boundary Primitives Transport (QPTM)
	 *            Messages [IUA]
	 *     6      MTP2 User Adaptation (MAUP) Messages [M2UA]
	 *     7      Connectionless Messages [SUA]
	 *     8      Connection-Oriented Messages [SUA]
	 *     9      Routing Key Management (RKM) Messages (M3UA)
	 *    10      Interface Identifier Management (IIM) Messages (M2UA)
	 * 11 to 127  Reserved by the IETF
	 *128 to 255  Reserved for IETF-Defined Message Class extensions
	 *
	 * </pre>
	 */
	enum MessageClass {
		__M2UA_MESSAGE_CLASS_UNKNOWN__ = -1,
		/** <b>0x00</b> - Management (MGMT) Message [IUA/M2UA/M3UA/SUA] */
		MGMT = 0x00,
		/** <b>0x01</b> - Transfer Messages [M3UA] */
		TRANSFER = 0x01,
		/** <b>0x02</b> - SS7 Signalling Network Management (SSNM) Messages [M3UA/SUA] */
		SSNM = 0x02,
		/** <b>0x03</b> - ASP State Maintenance (ASPSM) Messages [IUA/M2UA/M3UA/SUA] */
		ASPSM = 0x03,
		/** <b>0x04</b> - ASP Traffic Maintenance (ASPTM) Messages [IUA/M2UA/M3UA/SUA] */
		ASPTM = 0x04,
		/** <b>0x05</b> - Q.921/Q.931 Boundary Primitives Transport (QPTM) Messages [IUA] */
		QPTM = 0x05,
		/** <b>0x06</b> - MTP2 User Adaptation (MAUP) Messages [M2UA] */
		MAUP = 0x06,
		/** <b>0x07</b> - Connectionless Messages [SUA] */
		SUA_NO_CON = 0x07,
		/** <b>0x08</b> - Connection-Oriented Messages [SUA] */
		SUA_CON = 0x08,
		/** <b>0x09</b> - Routing Key Management (RKM) Messages (M3UA) */
		RKM = 0x09,
		/** <b>0x0a</b> - Interface Identifier Management (IIM) Messages (M2UA) */
		IIM = 0x0a
	};

	/**
	 * <pre>
	 *3.1.4  Message Type
	 *
	 *   The following List contains the Message Types for the valid Message
	 *   Classes:
	 *
	 *   MTP2 User Adaptation (MAUP) Messages
	 *
	 *        0      Reserved
	 *        1      Data
	 *        2      Establish Request
	 *        3      Establish Confirm
	 *        4      Release Request
	 *        5      Release Confirm
	 *        6      Release Indication
	 *        7      State Request
	 *        8      State Confirm
	 *        9      State Indication
	 *       10      Data Retrieval Request
	 *       11      Data Retrieval Confirm
	 *       12      Data Retrieval Indication
	 *       13      Data Retrieval Complete Indication
	 *       14      Congestion Indication
	 *       15      Data Acknowledge
	 *    16 to 127  Reserved by the IETF
	 *   128 to 255  Reserved for IETF-Defined MAUP extensions
	 *
	 *   Application Server Process State Maintenance (ASPSM) messages
	 *
	 *        0      Reserved
	 *        1      ASP Up (UP)
	 *        2      ASP Down (DOWN)
	 *        3      Heartbeat (BEAT)
	 *        4      ASP Up Ack (UP ACK)
	 *        5      ASP Down Ack (DOWN ACK)
	 *        6      Heartbeat Ack (BEAT ACK)
	 *     7 to 127  Reserved by the IETF
	 *   128 to 255  Reserved for IETF-Defined ASPSM extensions
	 *
	 *   Application Server Process Traffic Maintenance (ASPTM) messages
	 *
	 *        0      Reserved
	 *        1      ASP Active (ACTIVE)
	 *        2      ASP Inactive (INACTIVE)
	 *        3      ASP Active Ack (ACTIVE ACK)
	 *        4      ASP Inactive Ack (INACTIVE ACK)
	 *     5 to 127  Reserved by the IETF
	 *   128 to 255  Reserved for IETF-Defined ASPTM extensions
	 *
	 *   Management (MGMT) Messages
	 *
	 *        0      Error (ERR)
	 *        1      Notify (NTFY)
	 *     2 to 127  Reserved by the IETF
	 *   128 to 255  Reserved for IETF-Defined MGMT extensions
	 *
	 *   Interface Identifier Management (IIM) Messages
	 *
	 *        0        Reserved
	 *        1        Registration Request (REG REQ)
	 *        2        Registration Response (REG RSP)
	 *        3        Deregistration Request (DEREG REQ)
	 *        4        Deregistration Response (DEREG RSP)
	 *     5 to 127    Reserved by the IETF
	 *   128 to 255    Reserved for IETF-Defined IIM extensions
	 *
	 * </pre>
	 */
	enum MessageType {
		__M2UA_MESSAGE_TYPE_UNKNOWN__ = -1,

		// MTP2 User Adaptation (MAUP) Messages
		/** <b>0x01</b> - Data */
		DATA = (MAUP << 8) + 0x01,
		/** <b>0x02</b> - Establish Request */
		ESTABLISH_REQUEST = (MAUP << 8) + 0x02,
		/** <b>0x03</b> - Establish Confirm */
		ESTABLISH_CONFIRM = (MAUP << 8) + 0x03,
		/** <b>0x04</b> - Release Request */
		RELEASE_REQUEST = (MAUP << 8) + 0x04,
		/** <b>0x05</b> - Release Confirm */
		RELEASE_CONFIRM = (MAUP << 8) + 0x05,
		/** <b>0x06</b> - Release Indication */
		RELEASE_INDICATION = (MAUP << 8) + 0x06,
		/** <b>0x07</b> - State Request */
		STATE_REQUEST = (MAUP << 8) + 0x07,
		/** <b>0x08</b> - State Confirm */
		STATE_CONFIRM = (MAUP << 8) + 0x08,
		/** <b>0x09</b> - State Indication */
		STATE_INDICATION = (MAUP << 8) + 0x09,
		/** <b>0x0a</b> - Data Retrieval Request */
		DATA_RETRIEVAL_REQUEST = (MAUP << 8) + 0x0a,
		/** <b>0x0b</b> - Data Retrieval Confirm */
		DATA_RETRIEVAL_CONFIRM = (MAUP << 8) + 0x0b,
		/** <b>0x0c</b> - Data Retrieval Indication */
		DATA_RETRIEVAL_INDICATION = (MAUP << 8) + 0x0c,
		/** <b>0x0d</b> - Data Retrieval Complete Indication */
		DATA_RETRIEVAL_COMPLETE_INDICATION = (MAUP << 8) + 0x0d,
		/** <b>0x0e</b> - Congestion Indication */
		CONGESTION_INDICATION = (MAUP << 8) + 0x0e,
		/** <b>0x0f</b> - Data Acknowledge */
		DATA_ACK = (MAUP << 8) + 0x0f,

		// Application Server Process State Maintenance (ASPSM) messages
		/** <b>0x01</b> - ASP Up (UP) */
		ASP_UP = (ASPSM << 8) + 0x01,
		/** <b>0x02</b> - ASP Down (DOWN) */
		ASP_DOWN = (ASPSM << 8) + 0x02,
		/** <b>0x03</b> - Heartbeat (BEAT) */
		HEARTBEAT = (ASPSM << 8) + 0x03,
		/** <b>0x04</b> - ASP Up Ack (UP ACK) */
		ASP_UP_ACK = (ASPSM << 8) + 0x04,
		/** <b>0x05</b> - ASP Down Ack (DOWN ACK) */
		ASP_DOWN_ACK = (ASPSM << 8) + 0x05,
		/** <b>0x06</b> - Heartbeat Ack (BEAT ACK) */
		HEARTBEAT_ACK = (ASPSM << 8) + 0x06,

		// Application Server Process Traffic Maintenance (ASPTM) messages
		/** <b>0x01</b> - ASP Active (ACTIVE) */
		ASP_ACTIVE = (ASPTM << 8) + 0x01,
		/** <b>0x02</b> - ASP Inactive (INACTIVE) */
		ASP_INACTIVE = (ASPTM << 8) + 0x02,
		/** <b>0x03</b> - ASP Active Ack (ACTIVE ACK) */
		ASP_ACTIVE_ACK = (ASPTM << 8) + 0x03,
		/** <b>0x04</b> - ASP Inactive Ack (INACTIVE ACK) */
		ASP_INACTIVE_ACK = (ASPTM << 8) + 0x04,

		// Management (MGMT) Messages
		/** <b>0x00</b> - Error (ERR) */
		ERR = (MGMT << 8) + 0x00,
		/** <b>0x01</b> - Notify (NTFY) */
		NTFY = (MGMT << 8) + 0x01,

		// Interface Identifier Management (IIM) Messages
		/** <b>0x01</b> - Registration Request (REG REQ) */
		REG_REQ = (IIM << 8) + 0x01,
		/** <b>0x02</b> - Registration Response (REG RSP) */
		REG_RSP = (IIM << 8) + 0x02,
		/** <b>0x03</b> - Deregistration Request (DEREG REQ) */
		DEREG_REQ = (IIM << 8) + 0x03,
		/** <b>0x04</b> - Deregistration Response (DEREG RSP) */
		DEREG_RSP = (IIM << 8) + 0x04



	};

	/**
	 * <pre>
	 *   Parameter Tag: 16 bits (unsigned integer)
	 *
	 *   The Type field is a 16 bit identifier of the type of parameter.  It
	 *   takes a value of 0 to 65534.  The common parameters used by the
	 *   adaptation layers are in the range of 0x00 to 0xff.  The M2UA
	 *   specific parameters have Tags in the range 0x300 to 0x3ff.
	 *
	 *   The common parameter tags (used by all User Adaptation layers) that
	 *   M2UA uses are defined below:
	 *
	 *      Parameter Value     Parameter Name
	 *      ---------------     --------------
	 *            0 (0x00)       Reserved
	 *            1 (0x01)       Interface Identifier (Integer)
	 *            2 (0x02)       Unused
	 *            3 (0x03)       Interface Identifier (Text)
	 *            4 (0x04)       Info String
	 *            5 (0x05)       Unused
	 *            6 (0x06)       Unused
	 *            7 (0x07)       Diagnostic Information
	 *            8 (0x08)       Interface Identifier (Integer Range)
	 *            9 (0x09)       Heartbeat Data
	 *           10 (0x0a)       Unused
	 *           11 (0x0b)       Traffic Mode Type
	 *           12 (0x0c)       Error Code
	 *           13 (0x0d)       Status Type/Information
	 *           14 (0x0e)       Unused
	 *           15 (0x0f)       Unused
	 *           16 (0x10)       Unused
	 *           17 (0x11)       ASP Identifier
	 *           18 (0x12)       Unused
	 *           19 (0x13)       Correlation Id
	 *          18-255           Reserved
	 *
	 *   The M2UA specific parameter Tags defined are as follows:
	 *
	 *      Parameter Value     Parameter Name
	 *      ---------------     --------------
	 *        768 (0x0300)      Protocol Data 1
	 *        769 (0x0301)      Protocol Data 2 (TTC)
	 *        770 (0x0302)      State Request
	 *        771 (0x0303)      State Event
	 *        772 (0x0304)      Congestion Status
	 *        773 (0x0305)      Discard Status
	 *        774 (0x0306)      Action
	 *        775 (0x0307)      Sequence Number
	 *        776 (0x0308)      Retrieval Result
	 *        777 (0x0309)      Link Key
	 *        778 (0x030a)      Local-LK-Identifier
	 *        779 (0x030b)      Signalling Data Terminal (SDT) Identifier
	 *        780 (0x030c)      Signalling Data Link (SDL) Identifier
	 *        781 (0x030d)      Registration Result
	 *        782 (0x030e)      Registration Status
	 *        783 (0x030f)      De-Registration Result
	 *        784 (0x0310)      De-Registration Status
	 *
	 * </pre>
	 */
	enum ParameterType {
		// The common parameter tags (used by all User Adaptation layers) that M2UA uses are defined below
		/** <b>0x01</b> - Interface Identifier (Integer) */
		INTERFACE_INDICATOR_INTEGER = 0x01,
		/** <b>0x03</b> - Interface Identifier (Text) */
		INTERFACE_INDICATOR_TEXT= 0x03,
		/** <b>0x04</b> - Info String */
		INFO_STRING = 0x04,
		/** <b>0x07</b> - Diagnostic Information */
		DIAGNOSTIC_INFORMATION = 0x07,
		/** <b>0x08</b> - Interface Identifier (Integer Range) */
		INTERFACE_INDICATOR_INTEGER_RANGE = 0x08,
		/** <b>0x09</b> - Heartbeat Data */
		HEARTBEAT_DATA = 0x09,
		/** <b>0x0b</b> - Traffic Mode Type */
		TRAFFIC_MODE_TYPE = 0x0b,
		/** <b>0x0c</b> - Error Code */
		ERROR_CODE = 0x0c,
		/** <b>0x0d</b> - Status Type/Information */
		STATUS_TYPE_INFO = 0x0d,
		/** <b>0x11</b> - ASP Identifier */
		ASP_IDENTIFIER = 0x11,
		/** <b>0x13</b> - Correlation Id */
		CORRELATION_ID = 0x13,

		// The M2UA specific parameter Tags defined are as follows
		/** <b>0x0300</b> - Protocol Data 1 */
		PROTOCOL_DATA_1 = 0x0300,
		/** <b>0x0301</b> - rotocol Data 2 (TTC) */
		PROTOCOL_DATA_2_TTC = 0x0301,
		/** <b>0x0302</b> - State Request */
		STATE_REQUEST_PARAM = 0x0302,
		/** <b>0x0303</b> - State Event */
		STATE_EVENT = 0x0303,
		/** <b>0x0304</b> - Congestion Status */
		CONGESTION_STATUS = 0x0304,
		/** <b>0x0305</b> - Discard Status */
		DISCARD_STATUS = 0x0305,
		/** <b>0x0306</b> - Action */
		ACTION = 0x0306,
		/** <b>0x0307</b> - Sequence Number */
		SEQUENCE_NUMBER = 0x0307,
		/** <b>0x0308</b> - Retrieval Result */
		RETRIEVAL_RESULT = 0x0308,
		/** <b>0x0309</b> - Link Key */
		LINK_KEY = 0x0309,
		/** <b>0x030a</b> - Local-LK-Identifier */
		LOCAL_LK_IDENTIFIER = 0x030a,
		/** <b>0x030b</b> - Signalling Data Terminal (SDT) Identifier */
		SDT_IDENTIFIER = 0x030b,
		/** <b>0x030c</b> - Signalling Data Link (SDL) Identifier */
		SDL_IDENTIFIER = 0x030c,
		/** <b>0x030d</b> - Registration Result */
		REGISTRATION_RESULT = 0x030d,
		/** <b>0x030e</b> - Registration Status */
		REGISTRATION_STATUS = 0x030e,
		/** <b>0x030f</b> - De-Registration Result */
		DEREGISTRATION_RESULT = 0x030f,
		/** <b>0x0310</b> - De-Registration Status */
		DEREGISTRATION_STATUS = 0x0310
	};

	/**
	 * <pre>
	 *   The valid values for State are shown in the following table.
	 *
	 *            Define           Value        Description
	 *      STATUS_LPO_SET          0x0      Request local processor outage
	 *      STATUS_LPO_CLEAR        0x1      Request local processor outage
	 *                                       recovered
	 *      STATUS_EMER_SET         0x2      Request emergency alignment
	 *      STATUS_EMER_CLEAR       0x3      Request normal alignment (cancel
	 *                                       emergency)
	 *      STATUS_FLUSH_BUFFERS    0x4      Flush or clear receive, transmit
	 *                                       and retransmit queues
	 *      STATUS_CONTINUE         0x5      Continue or Resume
	 *      STATUS_CLEAR_RTB        0x6      Clear the retransmit queue
	 *      STATUS_AUDIT            0x7      Audit state of link
	 *      STATUS_CONG_CLEAR       0x8      Congestion cleared
	 *      STATUS_CONG_ACCEPT      0x9      Congestion accept
	 *      STATUS_CONG_DISCARD     0xa      Congestion discard
	 *
	 * </pre>
	 */
	enum StateType {
		/** <b>0x00</b> - Request local processor outage */
		STATUS_LPO_SET = 0x00,
		/** <b>0x01</b> - Request local processor outage recovered*/
		STATUS_LPO_CLEAR = 0x01,
		/** <b>0x02</b> - Request emergency alignment */
		STATUS_EMER_SET = 0x02,
		/** <b>0x03</b> - Request normal alignment (cancel emergency) */
		STATUS_EMER_CLEAR = 0x03,
		/** <b>0x04</b> - Flush or clear receive, transmit and retransmit queues */
		STATUS_FLUSH_BUFFERS = 0x04,
		/** <b>0x05</b> - Continue or Resume */
		STATUS_CONTINUE = 0x05,
		/** <b>0x06</b> - Clear the retransmit queue */
		STATUS_CLEAR_RTB = 0x06,
		/** <b>0x07</b> - Audit state of link */
		STATUS_AUDIT = 0x07,
		/** <b>0x08</b> - Congestion cleared */
		STATUS_CONG_CLEAR = 0x08,
		/** <b>0x09</b> - Congestion accept */
		STATUS_CONG_ACCEPT = 0x09,
		/** <b>0x0a</b> - Congestion discard */
		STATUS_CONG_DISCARD = 0x0a
	};


	/**
	 * <pre>
   The Status Type parameter identifies the type of the Notify message.
   The following are the valid Status Type values:

      Value          Description
       0x1   Application Server state change (AS_State_Change)
       0x2   Other

	 * </pre>
	 */
	enum StatusType {
		/** <b>0x01</b> - Application Server state change (AS_State_Change) */
		ST_AS_STATE_CHANGE = 0x01,
		/** <b>0x01</b> - Other */
		ST_OTHER = 0x02
	};

	/**
	 * <pre>
	 *   The Status Information parameter contains more detailed information
	 *   for the notification, based on the value of the Status Type.  If the
	 *   Status Type is AS_State_Change the following Status Information
	 *   values are used:
	 *
	 *      Value          Description
	 *       1      reserved
	 *       2      Application Server Inactive (AS_Inactive)
	 *       3      Application Server Active (AS_Active)
	 *       4      Application Server Pending (AS_Pending)
	 *
	 *   These notifications are sent from an SGP to an ASP upon a change in
	 *   status of a particular Application Server.  The value reflects the
	 *   new state of the Application Server.  The Interface Identifiers of
	 *   the AS MAY be placed in the message if desired.
	 *
	 *   If the Status Type is Other, then the following Status Information
	 *   values are defined:
	 *
	 *      Value          Description
	 *         1       Insufficient ASP resources active in AS
	 *         2       Alternate ASP Active
	 *         3       ASP Failure
	 *
	 * </pre>
	 */
	enum StatusInformationType {
		// Application Server state change (AS_State_Change)
		SI_AS_INACTIVE = (ST_AS_STATE_CHANGE << 16) + 0x02,
		SI_AS_ACTIVE = (ST_AS_STATE_CHANGE << 16) + 0x03,
		SI_AS_PENDING = (ST_AS_STATE_CHANGE << 16) + 0x04,

		// Other
		SI_INSF_ASP_RESOURCE = (ST_OTHER << 16) + 0x01,
		SI_ALT_ASP_ACTIVE = (ST_OTHER << 16) + 0x02,
		SI_ASP_FAILURE = (ST_OTHER << 16) + 0x03
	};



	/**
	 * <pre>
	 *   The valid values for Event are shown in the following table.
	 *
	 *          Define            Value          Description
	 *      EVENT_RPO_ENTER        0x1      Remote entered processor outage
	 *      EVENT_RPO_EXIT         0x2      Remote exited processor outage
	 *      EVENT_LPO_ENTER        0x3      Link entered processor outage
	 *      EVENT_LPO_EXIT         0x4      Link exited processor outage
	 *
	 * </pre>
	 */
	enum EventType {
		/** <b>0x01</b> - Remote entered processor outage */
		EVENT_RPO_ENTER = 0x01,
		/** <b>0x02</b> - Remote exited processor outage */
		EVENT_RPO_EXIT = 0x02,
		/** <b>0x03</b> - Link entered processor outage */
		EVENT_LPO_ENTER = 0x03,
		/** <b>0x04</b> - Link exited processor outage */
		EVENT_LPO_EXIT = 0x04
	};


	/**
	 * <pre>
	 *   The valid values for Congestion Status and Discard Status are shown
	 *   in the following table.
	 *
	 *            Define        Value        Description
	 *          LEVEL_NONE       0x0       No congestion
	 *          LEVEL_1          0x1       Congestion Level 1
	 *          LEVEL_2          0x2       Congestion Level 2
	 *          LEVEL_3          0x3       Congestion Level 3
	 *
	 * </pre>
	 */
	enum CongestionDiscardStatusType {
		/** <b>0x00</b> - No congestion */
		LEVEL_NONE = 0x00,
		/** <b>0x01</b> - Congestion Level 1 */
		LEVEL_1 = 0x01,
		/** <b>0x02</b> - Congestion Level 2 */
		LEVEL_2 = 0x02,
		/** <b>0x03</b> - Congestion Level 3 */
		LEVEL_3 = 0x03
	};

	/**
	 * <pre>
	 *   The valid values for Action are shown in the following table.
	 *
	 *           Define         Value       Description
	 *      ACTION_RTRV_BSN      0x1     Retrieve the backward sequence number
	 *      ACTION_RTRV_MSGS     0x2     Retrieve the PDUs from the transmit
	 *                                   and retransmit queues
	 *
	 * </pre>
	 */
	enum ActionType {
		/** <b>0x01</b> - Retrieve the backward sequence number */
		ACTION_RTRV_BSN = 0x01,
		/** <b>0x02</b> - Retrieve the PDUs from the transmit and retransmit queues */
		ACTION_RTRV_MSGS = 0x02
	};

	/**
	 * <pre>
	 *   The values for Result are shown below:
	 *
	 *           Define         Value       Description
	 *      RESULT_SUCCESS       0x0     Action successful
	 *      RESULT_FAILURE       0x1     Action failed
	 *
	 * </pre>
	 */
	enum ResultType {
		/** <b>0x00</b> - Action successful */
		RESULT_SUCCESS = 0x00,
		/** <b>0x01</b> - Action failed */
		RESULT_FAILURE = 0x01
	};

	/**
	 * <pre>
	 *   The Traffic Mode Type parameter identifies the traffic mode of
	 *   operation of the ASP within an AS.  The valid values for Type are
	 *   shown in the following table:
	 *
	 *      Value          Description
	 *       0x1            Override
	 *       0x2            Load-share
	 *       0x3            Broadcast
	 *
	 * </pre>
	 */
	enum TMTType {
		/** <b>0x01</b> - Override */
		OVERRIDE = 0x01,
		/** <b>0x02</b> - Load-share */
		LOADSHARE = 0x02,
		/** <b>0x03</b> - Broadcast */
		BROADCAST = 0x03
	};

	/**
	 * <pre>
	 *   The Error Code parameter indicates the reason for the Error Message.
	 *   The Error parameter value can be one of the following values:
	 *
	 *      Invalid Version                        0x1
	 *      Invalid Interface Identifier           0x2
	 *      Unsupported Message Class              0x3
	 *      Unsupported Message Type               0x4
	 *      Unsupported Traffic Handling Mode      0x5
	 *      Unexpected Message                     0x6
	 *      Protocol Error                         0x7
	 *      Unsupported Interface Identifier Type  0x8
	 *      Invalid Stream Identifier              0x9
	 *      Not Used in M2UA                       0xa
	 *      Not Used in M2UA                       0xb
	 *      Not Used in M2UA                       0xc
	 *      Refused - Management Blocking          0xd
	 *      ASP Identifier Required                0xe
	 *      Invalid ASP Identifier                 0xf
	 *      ASP Active for Interface Identifier(s) 0x10
	 *      Invalid Parameter Value                0x11
	 *      Parameter Field Error                  0x12
	 *      Unexpected Parameter                   0x13
	 *      Not Used in M2UA                       0x14
	 *      Not Used in M2UA                       0x15
	 *      Missing Parameter                      0x16
	 *
	 * </pre>
	 */
	enum ErrorCodeType {
		/** <b>0x01</b> - Invalid Version */
		ERR_INVALID_VER = 0x01,
		/** <b>0x02</b> - Invalid Interface Identifier */
		ERR_INVALID_INTERFACE_ID = 0x02,
		/** <b>0x03</b> - Unsupported Message Class */
		ERR_UNSUPPORTED_MESSAGE_CLASS = 0x03,
		/** <b>0x04</b> - Unsupported Message Type */
		ERR_UNSUPPORTED_MESSAGE_TYPE = 0x04,
		/** <b>0x05</b> - Unsupported Traffic Handling Mode */
		ERR_UNSUPPORTED_TRAFFIC_HANDLING_MODE = 0x05,
		/** <b>0x06</b> - Unexpected Message */
		ERR_UNEXPECTED_MESSAGE = 0x06,
		/** <b>0x07</b> - Protocol Error */
		ERR_PROTOCOL_ERROR = 0x07,
		/** <b>0x08</b> - Unsupported Interface Identifier Type */
		ERR_UNSUPPORTED_INTERFACE_ID_TYPE = 0x08,
		/** <b>0x09</b> - Invalid Stream Identifier */
		ERR_INVALID_STREAM_ID = 0x09,
		/** <b>0x0d</b> - Refused - Management Blocking */
		ERR_REFUSED = 0x0d,
		/** <b>0x0e</b> - ASP Identifier Required */
		ERR_ASP_ID_REQUIRED = 0x0e,
		/** <b>0x0f</b> - Invalid ASP Identifier */
		ERR_INVALID_ASP_IP = 0x0f,
		/** <b>0x10</b> - ASP Active for Interface Identifier(s) */
		ERR_ASP_ACTIVE_FOR_INTERFACE_ID = 0x10,
		/** <b>0x11</b> - Invalid Parameter Value */
		ERR_INVALID_PARAM_VALUE = 0x11,
		/** <b>0x12</b> - Parameter Field Error */
		ERR_PARAM_FIELD_ERROR = 0x12,
		/** <b>0x13</b> - Unexpected Parameter */
		ERR_UNEXPECTED_PARAM = 0x13,
		/** <b>0x13</b> - Missing Parameter */
		ERR_MISSING_PARAM = 0x16

	};

	/**
	 * <pre>
	 *      The Registration Result Status field indicates the success or the
	 *      reason for failure of a registration request.
	 *
	 *      Its values may be one of the following:
	 *
	 *            0         Successfully Registered
	 *            1         Error - Unknown
	 *            2         Error - Invalid SDLI
	 *            3         Error - Invalid SDTI
	 *            4         Error - Invalid Link Key
	 *            5         Error - Permission Denied
	 *            6         Error - Overlapping (Non-unique) Link Key
	 *            7         Error - Link Key not Provisioned
	 *            8         Error - Insufficient Resources
	 *
	 * </pre>
	 *
	 */
	enum RegistrationStatusType {
		/** <b>0x00</b> - Successfully Registered */
		RS_SUCCESSFULLY_REGISTERED = 0x00,
		/** <b>0x01</b> - Error - Unknown */
		RS_ERR_UNKNOWN = 0x01,
		/** <b>0x02</b> - Error - Invalid SDLI */
		RS_ERR_INVALID_SDLI = 0x02,
		/** <b>0x03</b> - Error - Error - Invalid SDTI */
		RS_ERR_INVALID_SDTI = 0x03,
		/** <b>0x04</b> - Error - Invalid Link Key */
		RS_ERR_INVALID_LK = 0x04,
		/** <b>0x05</b> - Error - Permission Denied */
		RS_ERR_PERMISSION_DENIED = 0x05,
		/** <b>0x06</b> - Error - Overlapping (Non-unique) Link Key */
		RS_ERR_OVRLP_LK = 0x06,
		/** <b>0x07</b> - Error - Link Key not Provisioned */
		RS_ERR_LK_NOT_PROVISIONED = 0x07,
		/** <b>0x08</b> - Error - Error - Insufficient Resources */
		RS_ERR_INSF_RESOURCES = 0x08
	};

	/**
	 * <pre>
	 *      The De-Registration Result Status field indicates the success or
	 *      the reason for failure of the de-registration.
	 *
	 *      Its values may be one of the following:
	 *
	 *            0         Successfully De-registered
	 *            1         Error - Unknown
	 *            2         Error - Invalid Interface Identifier
	 *            3         Error - Permission Denied
	 *            4         Error - Not Registered
	 *
	 * </pre>
	 */
	enum DeregistrationStatusType {
		/** <b>0x06</b> - Successfully De-registered */
		DRS_SUCCESSFULLY_DEREGISTERED = 0x00,
		/** <b>0x01</b> - Error - Unknown */
		DRS_ERR_UNKNOWN = 0x01,
		/** <b>0x02</b> - Error - Invalid Interface Identifier */
		DRS_ERR_INVALID_IID = 0x02,
		/** <b>0x03</b> - Error - Permission Denied */
		DRS_ERR_PERMISSION_DENIED = 0x03,
		/** <b>0x04</b> - Error - Not Registered */
		DRS_ERR_NOT_REGISTERED = 0x04
	};


	/**
	 * \brief M2UAParam: base M2UA parameter class
	 * \details
	 * <pre>
	 *3.1.6  Variable-Length Parameter Format
	 *
	 *   M2UA messages consist of a Common Header followed by zero or more
	 *   variable-length parameters, as defined by the message type.  The
	 *   variable-length parameters contained in a message are defined in a
	 *   Tag-Length-Value format as shown below.
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |          Parameter Tag        |       Parameter Length        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   \                                                               \
	 *   /                       Parameter Value                         /
	 *   \                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Mandatory parameters MUST be placed before optional parameters in a
	 *   message.
	 *
	 *   Parameter Tag: 16 bits (unsigned integer)
	 *
	 *   The Type field is a 16 bit identifier of the type of parameter.  It
	 *   takes a value of 0 to 65534.  The common parameters used by the
	 *   adaptation layers are in the range of 0x00 to 0xff.  The M2UA
	 *   specific parameters have Tags in the range 0x300 to 0x3ff.
	 *
	 *   Parameter Length: 16 bits (unsigned integer)
	 *
	 *   The Parameter Length field contains the size of the parameter in
	 *   bytes, including the Parameter Tag, Parameter Length, and Parameter
	 *   Value fields.  Thus, a parameter with a zero-length Parameter Value
	 *   field would have a Length field of 4.  The Parameter Length does not
	 *   include any padding bytes.
	 *
	 *   Parameter Value: variable-length.
	 *
	 *   The Parameter Value field contains the actual information to be
	 *   transferred in the parameter.
	 *
	 *   The total length of a parameter (including Tag, Parameter Length and
	 *   Value fields) MUST be a multiple of 4 bytes.  If the length of the
	 *   parameter is not a multiple of 4 bytes, the sender pads the Parameter
	 *   at the end (i.e., after the Parameter Value field) with all zero
	 *   bytes.  The length of the padding is NOT included in the parameter
	 *   length field.  A sender MUST NOT pad with more than 3 bytes.  The
	 *   receiver MUST ignore the padding bytes.
	 *
	 * </pre>
	 */
	class M2UAParam {
	public:
		M2UAParam();
		virtual ~M2UAParam();
		ParameterType type;
		int byte_pos;
		int value_length;
		//bool new_value_mem;
		char* value;
		virtual void init(char* data, int data_length);
		virtual void encode(char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * <pre>
	 *   The Protocol Data field contains the MTP2-User application message in
	 *   network byte order starting with the Signalling Information Octet
	 *   (SIO).  The Correlation Id parameter uniquely identifies the MSU
	 *   carried in the Protocol Data within an AS.  This Correlation Id
	 *   parameter is assigned by the sending M2UA.  The purpose of the
	 *   Correlation Id is to permit the newly active ASP to synchronize its
	 *   processing of the traffic in each ordered stream with other ASPs in
	 *   the broadcast group.
	 *
	 * </pre>
	 */
	class ProtocolData: public M2UAParam {
	public:
		ProtocolData();
	};

	/**
	 * <pre>
	 *   The Protocol Data field contains the MTP2-User application message in
	 *   network byte order starting with the Length Indicator (LI) octet.
	 *   The Japanese TTC variant uses the spare bits of the LI octet for
	 *   priority.
	 *
	 * </pre>
	 */
	class ProtocolData_TTC: public M2UAParam {
	public:
		ProtocolData_TTC();
	};

	class Correlation_id: public M2UAParam {
	public:
		Correlation_id();
	};

	class SequenceNumber: public M2UAParam {
	public:
		SequenceNumber();
	};

	class StateRequest: public M2UAParam {
	public:
		StateRequest();
		StateType state;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class StateEvent: public M2UAParam {
	public:
		StateEvent();
		EventType event;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);

	};

	class CongestionStatus: public M2UAParam {
	public:
		CongestionStatus();
		CongestionDiscardStatusType congestion_status;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class DiscardStatus: public M2UAParam {
	public:
		DiscardStatus();
		CongestionDiscardStatusType discard_status;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class Action: public M2UAParam {
	public:
		Action();
		ActionType action;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class RetrievalResult: public M2UAParam {
	public:
		RetrievalResult();
		ResultType result;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);

	};

	class Asp_identifier: public M2UAParam {
	public:
		Asp_identifier();
	};

	class Info_string: public M2UAParam {
	public:
		Info_string();

	};

	class Heartbeat_data: public M2UAParam {
	public:
		Heartbeat_data();
	};

	class TrafficModeType: public M2UAParam {
	public:
		TrafficModeType();
		TMTType tmt_type;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);

	};

	class InterfaceIdentifier_int_range: public M2UAParam {
	public:
		InterfaceIdentifier_int_range();
	};

	class InterfaceIdentifier_int: public M2UAParam {
	public:
		InterfaceIdentifier_int();
	};

	class InterfaceIdentifier_txt: public M2UAParam {
	public:
		InterfaceIdentifier_txt();
	};


	class ErrorCode: public M2UAParam {
	public:
		ErrorCode();
		ErrorCodeType error_code;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class DiagnosticInformation: public M2UAParam {
	public:
		DiagnosticInformation();
	};

	class StatusTypeInfo: public M2UAParam {
	public:
		StatusTypeInfo();
		StatusType status_type;
		StatusInformationType status_info;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class LocalLKId: public M2UAParam {
	public:
		LocalLKId();
	};

	class SignallingDTId: public M2UAParam {
	public:
		SignallingDTId();
	};

	class SignallingDLId: public M2UAParam {
	public:
		SignallingDLId();
	};


	class LinkKey: public M2UAParam {
	public:
		LinkKey();
	};

	class RegistrationStatus: public M2UAParam {
	public:
		RegistrationStatus();
		RegistrationStatusType registration_status;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class RegistrationResult: public M2UAParam {
	public:
		RegistrationResult();


	};

	class DeregistrationStatus: public M2UAParam {
	public:
		DeregistrationStatus();
	};



	class DeregistrationResult: public M2UAParam {
	public:
		DeregistrationResult();
	};

	/**
	 * \brief M2UAMessage: base M2UA message class
	 */
	class M2UAMessage {
	public:
		M2UAMessage();
		virtual ~M2UAMessage();
		MessageType type;
		int byte_pos;
		virtual void init(char* data, int data_length);
		virtual void init_new();
		virtual void encode(char* buffer, int buffer_length, int offset, int* result_length);
		int get_params_count();
		void clear_params();
		M2UAParam* get_parameter(ParameterType _param_type);
		M2UAParam* add_parameter(ParameterType _param_type);
		virtual bool check_mandatory() = 0;
	protected:
		std::vector<M2UAParam*> params;
		void process_params(char* data, int data_length);
		int encode_parameter(M2UAParam* param, char* buffer, int buffer_length, int offset);

	};



	/**
	 * \brief M2UAPacket: main class for every decoded M2UA packet
	 * \details
	 * <pre>
	 *	3.1  Common Message Header
	 *
	 *	   The protocol messages for MTP2-User Adaptation require a message
	 *	   structure that contains a version, message class, message type,
	 *	   message length, and message contents.  This message header is common
	 *	   among all signalling protocol adaptation layers:
	 *
	 *	    0                   1                   2                   3
	 *	    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *	   |    Version    |     Spare     | Message Class | Message Type  |
	 *	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *	   |                        Message Length                         |
	 *	   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *	3.1.1  Version
	 *
	 *   The version field contains the version of the M2UA adaptation layer.
	 *   The supported versions are:
	 *
	 *         Value    Version
	 *         -----    -------
	 *           1      Release 1.0
	 *
	 *
	 *	3.1.2  Spare
	 *
	 *   The Spare field is 8-bits.  It SHOULD be set to all '0's by the
	 *   sender and ignored by the receiver.
	 *
	 *
	 *	3.1.5  Message Length
	 *
	 *   The Message Length defines the length of the message in octets,
	 *   including the header.  The Message Length MUST include parameter
	 *   padding bytes, if any.  The Message Length MUST NOT be longer than a
	 *   MTP3 message [2,3,4,5] plus the length of the common and M2UA message
	 *   headers.
	 *
	 * </pre>
	 */
	class M2UAPacket {
	public:
		M2UAPacket();
		~M2UAPacket();
		int version;
		int message_length;
		MessageClass message_class;
		MessageType message_type;
		M2UAMessage* message;
		void encode(char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief Data: Data M2UA message
	 * \details
	 * <pre>
	 *3.3.1.1 Data
	 *
	 *   The Data message contains an SS7 MTP2-User Protocol Data Unit (PDU).
	 *   The Data message contains the following parameter:
	 *
	 *      Protocol Data (mandatory)
	 *      Correlation Id (optional)
	 *
	 *   The format for the Data Message parameters is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x300)           |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                       Protocol Data                           /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x13)            |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                         Correlation Id                        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The Protocol Data field contains the MTP2-User application message in
	 *   network byte order starting with the Signalling Information Octet
	 *   (SIO).  The Correlation Id parameter uniquely identifies the MSU
	 *   carried in the Protocol Data within an AS.  This Correlation Id
	 *   parameter is assigned by the sending M2UA.  The purpose of the
	 *   Correlation Id is to permit the newly active ASP to synchronize its
	 *   processing of the traffic in each ordered stream with other ASPs in
	 *   the broadcast group.
	 *
	 *   The format for a Data Message with TTC PDU parameters is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x301)           |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                    TTC Protocol Data                          /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |          Tag (0x13)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                         Correlation Id                        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The Protocol Data field contains the MTP2-User application message in
	 *   network byte order starting with the Length Indicator (LI) octet.
	 *   The Japanese TTC variant uses the spare bits of the LI octet for
	 *   priority.
	 *
	 *   The length of the Protocol Data and TTC Protocol Data MUST NOT exceed
	 *   the length of a MTP2-User application message [2,3,5].
	 *
	 * </pre>
	 */
	class Data: public M2UAMessage {
	public:
		Data();
		bool check_mandatory();

	};

	/**
	 * \brief Data_ack:  Data Acknowledge M2UA message
	 * \details
	 * <pre>
	 *3.3.1.2  Data Acknowledge Message
	 *
	 *   The Data Acknowledge message contains the Correlation Id of the Data
	 *   message that the sending M2UA is acknowledging as successfully
	 *   processed to the peer M2UA.
	 *
	 *   The Data Acknowledge message contains the following parameter:
	 *
	 *      Correlation Id       Mandatory
	 *
	 *   The following format MUST be used for the Data Ack Message:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |          Tag (0x13)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                         Correlation Id                        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The Correlation Id parameter of the Data message and the Data Ack
	 *   message provide a mechanism, for those SG implementations capable of
	 *   taking advantage of them, to obtain an acknowledgment that the MSU
	 *   has been transferred to the M2UA peer before acknowledging the MSU to
	 *
	 *   the SS7 peer, removing the risk of losing messages due to association
	 *   failure or SCTP congestion.
	 *
	 *   The Data Ack message MUST be sent if a Correlation Id parameter is
	 *   received from the peer.  Otherwise, the Data Ack message MUST NOT be
	 *   sent.
	 *
	 *   If the Data Acknowledge is not sent for Correlation Id(s) or is sent
	 *   with Invalid Correlation Id(s), the SS7 link will eventually fail due
	 *   to lack of MTP Level 2 acknowledgments of the SS7 peer's MSUs.
	 *
	 * </pre>
	 */
	class Data_ack: public M2UAMessage {
	public:
		Data_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Establish_request: Establish Request M2UA message
	 * \details
	 * <pre>
	 *3.3.1.3  Establish (Request, Confirmation)
	 *
	 *   The Establish Request message is used to establish the SS7 link or to
	 *   indicate that the channel has been established.  The MGC controls the
	 *   state of the SS7 link.  When the MGC desires the SS7 link to be in-
	 *   service, it will send the Establish Request message.  Note that the
	 *   SGP MAY already have the SS7 link established at its layer.  If so,
	 *   upon receipt of an Establish Request, the SGP takes no action except
	 *   to send an Establish Confirm.
	 *
	 *   When the MGC sends an M2UA Establish Request message, the MGC MAY
	 *   start a timer.  This timer would be stopped upon receipt of an M2UA
	 *   Establish Confirm.  If the timer expires, the MGC would resend the
	 *   M2UA Establish Request message and restart the timer.  In other
	 *   words, the MGC MAY continue to request the establishment of the data
	 *   link on a periodic basis until the desired state is achieved or some
	 *   other action is taken (notify the Management Layer).
	 *
	 *   The mode (Normal or Emergency) for bringing the SS7 link in service
	 *   is defaulted to Normal.  The State Request (described in Section
	 *   3.3.1.5 below) can be used to change the mode to Emergency.
	 *
	 * </pre>
	 */
	class Establish_request: public M2UAMessage {
	public:
		Establish_request();
		bool check_mandatory();
	};

	/**
	 * \brief Establish_confirm: Establish Confirm M2UA message
	 * \details
	 * <pre>
	 *3.3.1.3  Establish (Request, Confirmation)
	 *
	 *   The Establish Request message is used to establish the SS7 link or to
	 *   indicate that the channel has been established.  The MGC controls the
	 *   state of the SS7 link.  When the MGC desires the SS7 link to be in-
	 *   service, it will send the Establish Request message.  Note that the
	 *   SGP MAY already have the SS7 link established at its layer.  If so,
	 *   upon receipt of an Establish Request, the SGP takes no action except
	 *   to send an Establish Confirm.
	 *
	 *   When the MGC sends an M2UA Establish Request message, the MGC MAY
	 *   start a timer.  This timer would be stopped upon receipt of an M2UA
	 *   Establish Confirm.  If the timer expires, the MGC would resend the
	 *   M2UA Establish Request message and restart the timer.  In other
	 *   words, the MGC MAY continue to request the establishment of the data
	 *   link on a periodic basis until the desired state is achieved or some
	 *   other action is taken (notify the Management Layer).
	 *
	 *   The mode (Normal or Emergency) for bringing the SS7 link in service
	 *   is defaulted to Normal.  The State Request (described in Section
	 *   3.3.1.5 below) can be used to change the mode to Emergency.
	 *
	 * </pre>
	 */
	class Establish_confirm: public M2UAMessage {
	public:
		Establish_confirm();
		bool check_mandatory();
	};

	/**
	 * \brief Release_request: Release Request M2UA message
	 * \details
	 * <pre>
	 *3.3.1.4  Release (Request, Indication, Confirmation)
	 *
	 *   This Release Request message is used to release the channel.  The
	 *   Release Confirm and Indication messages are used to indicate that the
	 *   channel has been released.
	 *
	 * </pre>
	 */
	class Release_request: public M2UAMessage {
	public:
		Release_request();
		bool check_mandatory();
	};

	/**
	 * \brief Release_indication: Release Indication M2UA message
	 * \details
	 * <pre>
	 *3.3.1.4  Release (Request, Indication, Confirmation)
	 *
	 *   This Release Request message is used to release the channel.  The
	 *   Release Confirm and Indication messages are used to indicate that the
	 *   channel has been released.
	 *
	 * </pre>
	 */
	class Release_indication: public M2UAMessage {
	public:
		Release_indication();
		bool check_mandatory();
	};

	/**
	 * \brief Release_confirm: Release Confirm M2UA message
	 * \details
	 * <pre>
	 *3.3.1.4  Release (Request, Indication, Confirmation)
	 *
	 *   This Release Request message is used to release the channel.  The
	 *   Release Confirm and Indication messages are used to indicate that the
	 *   channel has been released.
	 *
	 * </pre>
	 */
	class Release_confirm: public M2UAMessage {
	public:
		Release_confirm();
		bool check_mandatory();
	};

	/**
	 * \brief State_request: State Request M2UA message
	 * \details
	 * <pre>
	 *3.3.1.5  State Request
	 *
	 *   The State Request message can be sent from a MGC to cause an action
	 *   on a particular SS7 link supported by the Signalling Gateway Process.
	 *   The SGP sends a State Confirm to the MGC if the action has been
	 *   successfully completed.  The State Confirm reflects that state value
	 *   received in the State Request message.
	 *
	 *   The State Request message contains the following parameter:
	 *
	 *    State (mandatory)
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x302)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                             State                             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 * </pre>
	 */
	class State_request: public M2UAMessage {
	public:
		State_request();
		bool check_mandatory();

	};

	/**
	 * \brief State_confirm: State Confirm M2UA message
	 * \details
	 * <pre>
	 *3.3.1.6  State Confirm
	 *
	 *   The State Confirm message will be sent by the SGP in response to a
	 *   State Request from the MGC.  The State Confirm reflects that state
	 *   value received in the State Request message.
	 *
	 *   The State Confirm message contains the following parameter:
	 *
	 *    State (mandatory)
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x302)           |           Length = 8          |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                             State                             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 * </pre>
	 */
	class State_confirm: public M2UAMessage {
	public:
		State_confirm();
		bool check_mandatory();
	};

	/**
	 * \brief State_indication: State Indication M2UA message
	 * \details
	 * <pre>
	 *3.3.1.7  State Indication
	 *
	 *   The MTP2 State Indication message can be sent from a SGP to an ASP to
	 *   indicate a condition on a SS7 link.
	 *
	 *   The State Indication message contains the following parameter:
	 *
	 *    Event (mandatory)
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x303)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                             Event                             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 * </pre>
	 */
	class State_indication: public M2UAMessage {
	public:
		State_indication();
		bool check_mandatory();
	};

	/**
	 * \brief Congestion_indication: Congestion Indication M2UA message
	 * \details
	 * <pre>
	 *3.3.1.8  Congestion Indication
	 *
	 *   The Congestion Indication message can be sent from a Signalling
	 *   Gateway Process to an ASP to indicate the congestion status and
	 *   discard status of a SS7 link.  When the MSU buffer fill increases
	 *   above an Onset threshold or decreases below an Abatement threshold or
	 *   crosses a Discard threshold in either direction, the SGP SHALL send a
	 *   congestion indication message when it supports SS7 MTP2 variants that
	 *   support multiple congestion levels.
	 *
	 *   The SGP SHALL send the message only when there is actually a change
	 *   in either the discard level or the congestion level to report,
	 *   meaning it is different from the previously sent message.  In
	 *   addition, the SGP SHALL use an implementation dependent algorithm to
	 *   limit the frequency of congestion indication messages.
	 *
	 *   An implementation may optionally send Congestion Indication messages
	 *   on a "high priority" stream in order to potentially reduce delay.
	 *
	 *   The Congestion Indication message contains the following parameters:
	 *
	 *    Congestion Status (mandatory)
	 *    Discard Status (optional)
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x304)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                       Congestion Status                       |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x305)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                         Discard Status                        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   For SS7 networks that do not support multiple levels of congestion,
	 *   only the LEVEL_NONE and LEVEL_3 values will be used.  For SS7
	 *   networks that support multiple levels of congestion, it is possible
	 *   for all values to be used.  Refer to [2], [3] and [12] for more
	 *   details on the Congestion and Discard Status of SS7 signalling links.
	 *
	 * </pre>
	 */
	class Congestion_indication: public M2UAMessage {
	public:
		Congestion_indication();
		bool check_mandatory();

	};

	/**
	 * \brief Retrieval_request: Retrieval Request M2UA message
	 * \details
	 * <pre>
	 *3.3.1.9  Retrieval Request
	 *
	 *   The MTP2 Retrieval Request message is used during the MTP Level 3
	 *   changeover procedure to request the BSN, to retrieve PDUs from the
	 *   transmit and retransmit queues or to flush PDUs from the retransmit
	 *   queue.  Examples of the use of Retrieval Request for SS7 Link
	 *   Changeover are provided in Section 5.3.6.
	 *
	 *   The Retrieval Request message contains the following parameters:
	 *
	 *    Action (mandatory)
	 *    Sequence Number (optional)
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x306)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                            Action                             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x307)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                       Sequence Number                         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   In the Retrieval Request message, the Sequence Number field SHOULD
	 *   NOT be present if the Action field is ACTION_RTRV_BSN.  The Sequence
	 *   Number field contains the Forward Sequence Number (FSN) of the far
	 *   end if the Action is ACTION_RTRV_MSGS.
	 *
	 * </pre>
	 */
	class Retrieval_request: public M2UAMessage {
	public:
		Retrieval_request();
		bool check_mandatory();
	};


	/**
	 * \brief Retrieval_confirm: Retrieval Confirm M2UA message
	 * \details
	 * <pre>
	 *3.3.1.10  Retrieval Confirm
	 *
	 *   The MTP2 Retrieval Confirm message is sent by the Signalling Gateway
	 *   in response to a Retrieval Request message.  Examples of the use of
	 *   the Retrieval Confirm for SS7 Link Changeover are provided in Section
	 *   5.3.6.
	 *
	 *   The Retrieval Confirm message contains the following parameters:
	 *
	 *    Action (mandatory)
	 *    Result (mandatory)
	 *    Sequence Number (optional)
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x306)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                            Action                             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x308)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                            Result                             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x307)           |          Length = 8           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                       Sequence Number                         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   When the Signalling Gateway Process sends a Retrieval Confirm to a
	 *   Retrieval Request, it echos the Action field.  If the Action was
	 *   ACTION_RTRV_BSN and the SGP successfully retrieved the BSN, the SGP
	 *   will put the Backward Sequence Number (BSN) in the Sequence Number
	 *   field and will indicate a success in the Result field.  If the BSN
	 *   could not be retrieved, the Sequence Number field will not be
	 *   included and the Result field will indicate failure.
	 *
	 *   For a Retrieval Confirm with Action of ACTION_RTRV_MSGS, the value of
	 *   the Result field will indicate success or failure.  A failure means
	 *   that the buffers could not be retrieved.  The Sequence Number field
	 *   is not used with ACTION_RTRV_MSGS.
	 *
	 * </pre>
	 *
	 */
	class Retrieval_confirm: public M2UAMessage {
	public:
		Retrieval_confirm();
		bool check_mandatory();
	};

	/**
	 * \brief Retrieval_indication: Retrieval Indication M2UA message
	 * \details
	 * <pre>
	 *3.3.1.11  Retrieval Indication
	 *
	 *   The Retrieval Indication message is sent by the Signalling Gateway
	 *   with a PDU from the transmit or retransmit queue.  The Retrieval
	 *   Indication message does not contain the Action or Sequence Number
	 *   fields, just a MTP3 Protocol Data Unit (PDU) from the transmit or
	 *   retransmit queue.  Examples of the use of the Retrieval Indication
	 *   for SS7 Link Changeover are provided in Section 5.3.6.
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |          Tag (0x300)          |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                       Protocol Data                           /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   For TTC Data messages, the following parameter will be used to
	 *   indicate a TTC PDU which starts at LI.
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |          Tag (0x301)          |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     TTC Protocol Data                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The M2UA implementation MAY consider the use of the bundling feature
	 *   of SCTP for Retrieval Indication messages.
	 *
	 * </pre>
	 */
	class Retrieval_indication: public M2UAMessage {
	public:
		Retrieval_indication();
		bool check_mandatory();
	};

	/**
	 * \brief Retrieval_complete_indication: Retrieval Complete Indication M2UA message
	 * \details
	 * <pre>
	 *3.3.1.12  Retrieval Complete Indication
	 *
	 *   The MTP2 Retrieval Complete Indication message is exactly the same as
	 *   the MTP2 Retrieval Indication message except that it also indicates
	 *   that retrieval is complete.  In addition, it MAY contain a PDU (which
	 *   MUST be the last PDU) from the transmit or retransmit queue.
	 *
	 * </pre>
	 */
	class Retrieval_complete_indication: public M2UAMessage {
	public:
		Retrieval_complete_indication();
		bool check_mandatory();
	};

	/**
	 * \brief Asp_up: ASP Up (ASPUP) M2UA message
	 * \details
	 * <pre>
	 *3.3.2.1  ASP Up (ASPUP)
	 *
	 *   The ASP Up (ASPUP) message is used to indicate to a remote M2UA peer
	 *   that the Adaptation layer is ready to receive traffic or maintenance
	 *   messages.
	 *
	 *   The ASPUP message contains the following parameters
	 *
	 *      ASP Identifier (optional)
	 *      Info String (optional)
	 *
	 *   Note: The ASP Identifier MUST be used where the SGP cannot
	 *         identify the ASP by pre-configured address/port number
	 *         information (e.g., where an ASP is resident on a Host using
	 *         dynamic address/port number assignment).
	 *
	 *   The format for ASPUP Message parameters is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0x11)          |             Length = 8        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                        ASP Identifier*                        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0x4)           |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The optional ASP Identifier parameter would contain a unique value
	 *   that is locally significant among the ASPs that support an AS.  The
	 *   SGP should save the ASP Identifier to be used, if necessary, with the
	 *   Notify message (see Section 3.3.3.2).
	 *
	 *   The optional INFO String parameter can carry any meaningful UTF-8 [6]
	 *   character string along with the message.  Length of the INFO String
	 *   parameter is from 0 to 255 octets.  No procedures are presently
	 *   identified for its use but the INFO String MAY be used for debugging
	 *   purposes.
	 *
	 * </pre>
	 */
	class Asp_up: public M2UAMessage {
	public:
		Asp_up();
		bool check_mandatory();
	};

	/**
	 * \brief Asp_up_ack: ASP Up Ack M2UA message
	 * \details
	 * <pre>
	 *3.3.2.2 ASP Up Ack
	 *
	 *   The ASP Up Ack message is used to acknowledge an ASP Up message
	 *   received from a remote M2UA peer.
	 *
	 *   The ASPUP Ack message contains the following parameters:
	 *
	 *      INFO String (optional)
	 *
	 *   The format for ASPUP Ack Message parameters is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0x4)           |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional Info String parameter is
	 *   the same as for the ASP UP message (See Section 3.3.2.1).
	 *
	 * </pre>
	 */
	class Asp_up_ack: public M2UAMessage {
	public:
		Asp_up_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Asp_down: ASP Down (ASPDN) M2UA message
	 * \details
	 * <pre>
	 *3.3.2.3  ASP Down (ASPDN)
	 *
	 *   The ASP Down (ASPDN) message is used to indicate to a remote M2UA
	 *   peer that the adaptation layer is not ready to receive traffic or
	 *   maintenance messages.
	 *
	 *   The ASPDN message contains the following parameters
	 *
	 *       INFO String (optional)
	 *
	 *   The format for the ASPDN message parameters is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0x4)           |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional Info String parameter is
	 *   the same as for the ASP Up message (See Section 3.3.2.1).
	 *
	 * </pre>
	 */
	class Asp_down: public M2UAMessage {
	public:
		Asp_down();
		bool check_mandatory();
	};

	/**
	 * \brief Asp_down_ack: ASP Down Ack M2UA message
	 * \details
	 * <pre>
	 *3.3.2.4 ASP Down Ack
	 *
	 *   The ASP Down Ack message is used to acknowledge an ASP Down message
	 *   received from a remote M2UA peer.
	 *
	 *   The ASP Down Ack message contains the following parameters:
	 *
	 *       INFO String (optional)
	 *
	 *   The format for the ASPDN Ack message parameters is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0x4)           |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional Info String parameter is
	 *   the same as for the ASP UP message (See Section 3.3.2.1).
	 *
	 * </pre>
	 *
	 */
	class Asp_down_ack: public M2UAMessage {
	public:
		Asp_down_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Heartbeat: Heartbeat (BEAT) M2UA message
	 * \details
	 * <pre>
	 *3.3.2.5  Heartbeat (BEAT)
	 *
	 *   The Heartbeat message is optionally used to ensure that the M2UA
	 *   peers are still available to each other.
	 *
	 *   The BEAT message contains the following parameter:
	 *
	 *       Heartbeat Data           Optional
	 *
	 *   The format for the BEAT message is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |            Tag = 0x0009       |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                       Heartbeat Data                          /
	 *   \                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The sending node defines the Heartbeat Data field contents.  It may
	 *   include a Heartbeat Sequence Number and/or time stamp, or other
	 *   implementation specific details.
	 *
	 *   The receiver of a Heartbeat message does not process this field as it
	 *   is only of significance to the sender.  The receiver echoes the
	 *   content of the Heartbeat Data in a BEAT ACK message.
	 *
	 * </pre>
	 */
	class Heartbeat: public M2UAMessage {
	public:
		Heartbeat();
		bool check_mandatory();
	};

	/**
	 * \brief Heartbeat_ack: Heartbeat Ack (BEAT ACK) M2UA message
	 * \details
	 * <pre>
	 *3.3.2.6  Heartbeat Ack (BEAT ACK)
	 *
	 *   The Heartbeat ACK message is sent in response to a BEAT message.  A
	 *   peer MUST send a BEAT ACK in response to a BEAT message.  It includes
	 *   all the parameters of the received Heartbeat message, without any
	 *   change.
	 *
	 *   The BEAT ACK message contains the following parameter:
	 *
	 *       Heartbeat Data           Optional
	 *
	 *   The format for the BEAT ACK message is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |            Tag = 0x0009       |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                       Heartbeat Data                          /
	 *   \                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The sending node defines the Heartbeat Data field contents.  It may
	 *   include a Heartbeat Sequence Number and/or time stamp, or other
	 *   implementation specific details.
	 *
	 *   The receiver of a Heartbeat message does not process this field as it
	 *   is only of significance to the sender.  The receiver echoes the
	 *   content of the Heartbeat Data in a BEAT ACK message.
	 *
	 * </pre>
	 */
	class Heartbeat_ack: public M2UAMessage {
	public:
		Heartbeat_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Asp_active: ASP Active (ASPAC) M2UA message
	 * \details
	 * <pre>
	 *3.3.2.7  ASP Active (ASPAC)
	 *
	 *   The ASPAC message is sent by an ASP to indicate to an SGP that it is
	 *   Active and ready to be used.
	 *
	 *   The ASPAC message contains the following parameters:
	 *
	 *      Traffic Mode Type (optional)
	 *      Interface Identifier (optional)
	 *         - Combination of integer and integer ranges, OR
	 *         - string (text formatted)
	 *      INFO String (optional)
	 *
	 *   The format for the ASPAC message using integer formatted Interface
	 *   Identifiers is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0xb)           |            Length = 8         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                     Traffic Mode Type                         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |     Tag (0x1=integer)         |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifiers*                    /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |    Tag (0x8=integer range)    |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *     .                                                            .
	 *     .                                                            .
	 *     .                                                            .
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier StartN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier StopN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                    of Tag Type 0x1 or 0x8                     \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x4)             |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format for the ASPAC message using text formatted (string)
	 *   Interface Identifiers is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0xb)           |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                     Traffic Mode Type                         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |       Tag (0x3=string)        |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifier*                     /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                       of Tag Type 0x3                         \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x4)             |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Within a particular AS, only one Traffic Mode Type can be used.  The
	 *   Override value indicates that the ASP is operating in Override mode,
	 *   where the ASP takes over all traffic in an Application Server (i.e.,
	 *   primary/backup operation), over-riding any currently active ASPs in
	 *   the AS.  In Load-share mode, the ASP will share in the traffic
	 *   distribution with any other currently active ASPs.  In Broadcast
	 *   mode, all of the Active ASPs receive all message traffic in the
	 *   Application Server.
	 *
	 *   The optional Interface Identifiers parameter contains a list of
	 *   Interface Identifier integers (Type 0x1 or Type 0x8) or text strings
	 *   (Type 0x3)indexing the Application Server traffic that the sending
	 *   ASP is configured/registered to receive.  If integer formatted
	 *   Interface Identifiers are being used, the ASP can also send ranges of
	 *   Interface Identifiers (Type 0x8).  Interface Identifier types Integer
	 *   (0x1) and Integer Range (0x8) are allowed in the same message.  Text
	 *   formatted Interface Identifiers (0x3) cannot be used with either
	 *   Integer (0x1) or Integer Range (0x8) types.
	 *
	 *   If no Interface Identifiers are included, the message is for all
	 *   provisioned Interface Identifiers within the AS(s) in which the ASP
	 *   is provisioned.  If only a subset of Interface Identifiers for an AS
	 *   are included, the ASP is noted as Active for all the Interface
	 *   Identifiers provisioned for that AS.
	 *
	 *   Note: If the optional Interface Identifier parameter is present, the
	 *         integer formatted Interface Identifier MUST be supported, while
	 *         the text formatted Interface Identifier MAY be supported.
	 *
	 *   An SGP that receives an ASPAC with an incorrect or unsupported
	 *   Traffic Mode Type for a particular Interface Identifier will respond
	 *   with an Error Message (Cause: Unsupported Traffic Handling Mode).
	 *
	 *   The format and description of the optional Info String parameter is
	 *   the same as for the ASP UP message (See Section 3.3.2.1).
	 *
	 * </pre>
	 */
	class Asp_active: public M2UAMessage {
	public:
		Asp_active();
		bool check_mandatory();
	};

	/**
	 * \brief Asp_active_ack: ASP Active Ack M2UA message
	 * \details
	 * <pre>
	 *3.3.2.8  ASP Active Ack
	 *
	 *   The ASP Active (ASPAC) Ack message is used to acknowledge an ASP
	 *   Active message received from a remote M2UA peer.
	 *
	 *   The ASPAC Ack message contains the following parameters:
	 *
	 *      Traffic Mode Type (optional)
	 *      Interface Identifier (optional)
	 *         - Combination of integer and integer ranges, OR
	 *         - string (text formatted)
	 *      INFO String (optional)
	 *
	 *   The format for the ASPAC Ack message with Integer-formatted Interface
	 *   Identifiers is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0xb)           |            Length = 8         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                      Traffic Mode Type                        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |     Tag (0x1=integer)         |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifiers*                    /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |    Tag (0x8=integer range)    |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *    .                                                            .
	 *    .                                                            .
	 *    .                                                            .
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier StartN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier StopN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                    of Tag Type 0x1 or 0x8                     \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x4)             |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format for the ASP Active Ack message using text formatted
	 *   (string) Interface Identifiers is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0xb)           |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                     Traffic Mode Type                         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |       Tag (0x3=string)        |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifier*                     /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                       of Tag Type 0x3                         \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x4)             |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional Info String parameter is
	 *   the same as for the ASP Up message (See Section 3.3.2.1).
	 *
	 *   The format of the optional Interface Identifier parameter is the same
	 *   as for the ASP Active message (See Section 3.3.2.7).
	 *
	 *   The format and description of the optional Info String parameter is
	 *   the same as for the ASP Up message (See Section 3.3.2.1).
	 *
	 * </pre>
	 *
	 */
	class Asp_active_ack: public M2UAMessage {
	public:
		Asp_active_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Asp_inactive: ASP Inactive (ASPIA) M2UA message
	 * \details
	 * <pre>
	 *3.3.2.9  ASP Inactive (ASPIA)
	 *
	 *   The ASP Inactive (ASPIA) message is sent by an ASP to indicate to an
	 *   SGP that it is no longer an active ASP to be used from within a list
	 *   of ASPs.  The SGP will respond with an ASPIA Ack message and either
	 *   discard incoming messages or buffer for a timed period and then
	 *   discard.
	 *
	 *   The ASPIA message contains the following parameters:
	 *
	 *      Interface Identifiers (optional)
	 *         - Combination of integer and integer ranges, OR
	 *         - string (text formatted)
	 *      INFO String (optional)
	 *
	 *   The format for the ASP Inactive message parameters using Integer
	 *   formatted Interface Identifiers is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |     Tag (0x1=integer)         |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifiers*                    /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |    Tag (0x8=integer range)    |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *    .                                                            .
	 *    .                                                            .
	 *    .                                                            .
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier StartN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier StopN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                    of Tag Type 0x1 or 0x8                     \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0x4)           |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *  The format for the ASP Inactive message using text formatted (string)
	 *   Interface Identifiers is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |       Tag (0x3=string)        |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifier*                     /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                      of Tag Type 0x3                          \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x4)             |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format of the optional Interface Identifier parameter is the same
	 *   as for the ASP Active message (See Section 3.3.2.7).
	 *
	 *   The format and description of the optional Info String parameter is
	 *   the same as for the ASP Up message (See Section 3.3.2.1).
	 *
	 *   The optional Interface Identifiers parameter contains a list of
	 *   Interface Identifier integers indexing the Application Server traffic
	 *   that the sending ASP is configured/registered to receive, but does
	 *   not want to receive at this time.
	 *
	 * </pre>
	 */
	class Asp_inactive: public M2UAMessage {
	public:
		Asp_inactive();
		bool check_mandatory();
	};


	/**
	 * \brief Asp_inactive_ack: ASP Inactive Ack M2UA message
	 * \details
	 * <pre>
	 *3.3.2.10 ASP Inactive Ack
	 *
	 *   The ASP Inactive (ASPIA) Ack message is used to acknowledge an ASP
	 *   Inactive message received from a remote M2UA peer.
	 *
	 *   The ASPIA Ack message contains the following parameters:
	 *
	 *      Interface Identifiers (optional)
	 *         - Combination of integer and integer ranges, OR
	 *         - string (text formatted)
	 *      INFO String (optional)
	 *
	 *   The format for the ASPIA Ack message is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |     Tag (0x1=integer)         |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifiers*                    /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |    Tag (0x8=integer range)    |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *    .                                                             .
	 *    .                                                             .
	 *    .                                                             .
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier StartN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier StopN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                    of Tag Type 0x1 or 0x8                     \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x4)             |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format for the ASP Inactive Ack message using text formatted
	 *   (string) Interface Identifiers is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |       Tag (0x3=string)        |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifier*                     /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                      of Tag Type 0x3                          \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x4)             |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format of the optional Interface Identifier parameter is the same
	 *   as for the ASP Active message (See Section 3.3.2.7).
	 *
	 *   The format and description of the optional Info String parameter is
	 *   the same as for the ASP Up message (See Section 3.3.2.1).
	 *
	 * </pre>
	 */
	class Asp_inactive_ack: public M2UAMessage {
	public:
		Asp_inactive_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Error: Error (ERR) M2UA message
	 * \details
	 * <pre>
	 *3.3.3.1  Error (ERR)
	 *
	 *   The Error (ERR) message is used to notify a peer of an error event
	 *   associated with an incoming message.  For example, the message type
	 *   might be unexpected given the current state, or a parameter value
	 *   might be invalid.
	 *
	 *   An Error message MUST not be generated in response to other Error
	 *   messages.
	 *
	 *   The ERR message contains the following parameters:
	 *
	 *      Error Code (mandatory)
	 *      Interface Identifier (optional)
	 *      Diagnostic Information (optional)
	 *
	 *   The format for the ERR message is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0xc)           |            Length = 8         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                          Error Code                           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |     Tag (0x1, 0x3, or 0x8)    |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifier(s)*                  /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0x7)           |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Diagnostic Information*                   /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The Error Code parameter indicates the reason for the Error Message.
	 *
	 *   The "Invalid Version" error would be sent if a message was received
	 *   with an invalid or unsupported version.  The Error message would
	 *   contain the supported version in the Common header.  The Error
	 *   message could optionally provide the supported version in the
	 *   Diagnostic Information area.
	 *
	 *   The "Invalid Interface Identifier" error would be sent by a SGP if an
	 *   ASP sends a message (i.e. an ASP Active message) with an invalid (not
	 *   configured) Interface Identifier value.  One of the optional
	 *   Interface Identifier parameters (Integer-based, text-based or integer
	 *   range) MUST be used with this error code to identify the invalid
	 *   Interface Identifier(s) received.
	 *
	 *   The "Unsupported Traffic Handling Mode" error would be sent by a SGP
	 *   if an ASP sends an ASP Active with an unsupported Traffic Handling
	 *   Mode.  An example would be a case in which the SGP did not support
	 *   load-sharing.  One of the optional Interface Identifier parameters
	 *   (Integer-based, text-based or integer range) MAY be used with this
	 *   error code to identify the Interface Identifier(s).
	 *
	 *   The "Unexpected Message" error would be sent by an ASP if it received
	 *   a MAUP message from an SGP while it was in the Inactive state.
	 *
	 *   The "Protocol Error" error would be sent for any protocol anomaly
	 *   (i.e. a bogus message).
	 *
	 *   The "Invalid Stream Identifier" error would be sent if a message was
	 *   received on an unexpected SCTP stream (i.e. a MGMT message was
	 *   received on a stream other than "0").
	 *
	 *   The "Unsupported Interface Identifier Type" error would be sent by a
	 *   SGP if an ASP sends a Text formatted Interface Identifier and the SGP
	 *   only supports Integer formatted Interface Identifiers.  When the ASP
	 *   receives this error, it will need to resend its message with an
	 *   Integer formatted Interface Identifier.
	 *
	 *   The "Unsupported Message Class" error would be sent if a message with
	 *   an unexpected or unsupported Message Class is received.
	 *
	 *   The "Refused - Management Blocking" error is sent when an ASP Up or
	 *   ASP Active message is received and the request is refused for
	 *   management reasons (e.g., management lock-out").
	 *
	 *   The "ASP Identifier Required" is sent by a SGP in response to an
	 *   ASPUP message which does not contain an ASP Identifier parameter when
	 *   the SGP requires one.  The ASP SHOULD resend the ASPUP message with
	 *   an ASP Identifier.
	 *
	 *   The "Invalid ASP Identifier" is sent by a SGP in response to an ASPUP
	 *   message with an invalid (i.e. non-unique) ASP Identifier.
	 *
	 *   The "ASP Currently Active for Interface Identifier(s)" error is sent
	 *   by a SGP when a Deregistration request is received from an ASP that
	 *   is active for Interface Identifier(s) specified in the Deregistration
	 *   request.  One of the optional Interface Identifier parameters
	 *   (Integer-based, text-based or integer range) MAY be used with this
	 *   error code to identify the Interface Identifier(s).
	 *
	 *   The "Invalid Parameter Value " error is sent if a message is received
	 *   with an invalid parameter value (e.g., a State Request with an an
	 *   undefined State).
	 *
	 *   The "Parameter Field Error" would be sent if a message with a
	 *   parameter has a wrong length field.
	 *
	 *   The "Unexpected Parameter" error would be sent if a message contains
	 *   an invalid parameter.
	 *
	 *   The "Missing Parameter" error would be sent if a mandatory parameter
	 *   was not included in a message.
	 *
	 *   The optional Diagnostic information can be any information germane to
	 *   the error condition, to assist in the identification of the error
	 *   condition.  In the case of an Invalid Version Error Code the
	 *   Diagnostic information includes the supported Version parameter.  In
	 *   the other cases, the Diagnostic information SHOULD be the first 40
	 *   bytes of the offending message.
	 *
	 * </pre>
	 *
	 */
	class Error: public M2UAMessage {
	public:
		Error();
		bool check_mandatory();
	};

	/**
	 * \brief Notify: Notify (NTFY) M2UA message
	 * \details
	 * <pre>
	 *   The Notify message is used to provide an autonomous indication of
	 *   M2UA events to an M2UA peer.
	 *
	 *   The NTFY message contains the following parameters:
	 *
	 *      Status Type (mandatory)
	 *      Status Information (mandatory)
	 *      ASP Identifier (optional)
	 *      Interface Identifiers (optional)
	 *      INFO String (optional)
	 *
	 *   The format for the Notify message with Integer-formatted Interface
	 *   Identifiers is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0xd)           |            Length = 8         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |        Status Type            |      Status Information       |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0x11)          |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                        ASP Identifier*                        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |     Tag (0x1=integer)         |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifiers*                    /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |    Tag (0x8=integer range)    |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop1*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier Start2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier Stop2*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *    .                                                             .
	 *    .                                                             .
	 *    .                                                             .
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Interface Identifier StartN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                  Interface Identifier StopN*                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                    of Tag Type 0x1 or 0x8                     \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x4)             |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format for the Notify message with Text-formatted Interface
	 *   Identifiers is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0xd)           |            Length = 8         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |        Status Type            |      Status Information       |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |           Tag (0x11)          |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                        ASP Identifier*                        |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |       Tag (0x3=string)        |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                     Interface Identifier*                     /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \              Additional Interface Identifiers                 /
	 *   /                        of Tag Type 0x3                        \
	 *   \                                                               /
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag (0x4)             |             Length            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   /                                                               \
	 *   \                          INFO String*                         /
	 *   /                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   In the Insufficient ASP Resources case, the SGP is indicating to an
	 *   ASP-INACTIVE ASP(s) in the AS that another ASP is required in order
	 *   to handle the load of the AS (Load-sharing mode).  For the Alternate
	 *   ASP Active case, the formerly Active ASP is informed when an
	 *   alternate ASP transitions to the ASP Active state in Override mode.
	 *   The ASP Identifier (if available) of the Alternate ASP MUST be placed
	 *   in the message.  For the ASP Failure case, the SGP is indicating to
	 *   ASP(s) in the AS that one of the ASPs has transitioned to ASP-DOWN.
	 *   The ASP Identifier (if available) of the failed ASP MUST be placed in
	 *   the message.
	 *
	 *   For each of the Status Information values in Status Type Other, the
	 *   Interface Identifiers of the affected AS MAY be placed in the message
	 *   if desired.
	 *
	 *   The format of the optional Interface Identifier parameter is the same
	 *   as for the ASP Active message (See Section 3.3.2.7).
	 *
	 *   The format and description of the optional Info String parameter is
	 *   the same as for the ASP Up message (See Section 3.3.2.1).
	 *
	 *
	 * </pre>
	 */
	class Notify: public M2UAMessage {
	public:
		Notify();
		bool check_mandatory();
	};

	/**
	 * \brief Registration_request: Registration Request (REG REQ) M2UA message
	 * \details
	 * <pre>
	 *3.3.4.1  Registration Request (REG REQ)
	 *
	 *   The REG REQ message is sent by an ASP to indicate to a remote M2UA
	 *   peer that it wishes to register one or more given Link Keys with the
	 *   remote peer.  Typically, an ASP would send this message to an SGP,
	 *   and expect to receive a REG RSP in return with an associated
	 *   Interface Identifier value.
	 *
	 *   The REG REQ message contains the following parameter:
	 *
	 *      Link Key          (mandatory)
	 *
	 *   The format for the REG REQ message is as follows
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag = 0x0309          |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   \                                                               \
	 *   /                           Link Key 1                          /
	 *   \                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   \                                                               \
	 *   /                              ...                              /
	 *   \                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag = 0x0309          |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   \                                                               \
	 *   /                           Link Key n                          /
	 *   \                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Link Key:   fixed length
	 *
	 *      The Link Key parameter is mandatory.  The sender of this message
	 *      expects that the receiver of this message will create a Link Key
	 *      entry and assign a unique Interface Identifier value to it, if the
	 *      Link Key entry does not yet exist.
	 *
	 *      The Link Key parameter may be present multiple times in the same
	 *      message.  This is used to allow the registration of multiple Link
	 *      Keys in a single message.
	 *
	 *   The format of the Link Key parameter is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                       Local-LK-Identifier                     |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                 Signalling Data Terminal Identifier           |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                   Signalling Data Link Identifier             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Local-LK-Identifier: 32-bit integer
	 *
	 *      The mandatory Local-LK-Identifier field is used to uniquely
	 *      (between ASP and SGP) identify the registration request.  The
	 *      Identifier value is assigned by the ASP, and is used to correlate
	 *      the response in a REG RSP message with the original registration
	 *      request.  The Identifier value MUST remain unique until the REG
	 *      RSP is received.
	 *
	 *      The format of the Local-LK-Identifier field is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag = 0x030a          |         Length = 8            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                    Local-LK-Identifier value                  |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Signalling Data Terminal Identifier
	 *
	 *      The Signalling Data Terminal Identifier parameter is mandatory.
	 *      It identifies the Signalling Data Terminal associated with the SS7
	 *      link for which the ASP is registering.  The format is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag = 0x030b          |         Length = 8            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |          Reserved             |        SDT Identifier         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *      The SDT Identifier is a 32-bit unsigned value which may only be
	 *      significant to 12 or 14 bits depending on the SS7 variant which is
	 *      supported by the MTP Level 3 at the ASP.  Insignificant SDT
	 *      Identifier bits are coded 0.
	 *
	 *   Signalling Data Link Identifier
	 *
	 *      The Signalling Data Link Identifier parameter is mandatory.  It
	 *      identifies the Signalling Data Link Identifier associated with the
	 *      SS7 link for which the ASP is registering.  The format is as
	 *      follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |         Tag = 0x030c          |         Length = 8            |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |          Reserved             |        SDL Identifier         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *      The SDL Identifier is a 32-bit unsigned value which may only be
	 *      significant to 12 or 14 bits depending on the SS7 variant which
	 *      is supported by the MTP Level 3 at the ASP.  Insignificant SDLI
	 *      bits are coded 0.
	 *
	 * </pre>
	 *
	 */
	class Registration_request: public M2UAMessage {
	public:
		Registration_request();
		bool check_mandatory();
	};

	/**
	 * \brief Registration_response: Registration Response (REG RSP) M2UA message
	 * \details
	 * <pre>
3.3.4.2  Registration Response (REG RSP)

   The REG RSP message is used as a response to the REG REQ message
   from a remote M2UA peer.  It contains indications of success/failure
   for registration requests and returns a unique Interface Identifier
   value for successful registration requests, to be used in subsequent
   M2UA Traffic Management protocol.

  The REG RSP message contains the following parameter:

      Registration Results   (mandatory)

   The format for the REG RSP message is as follows:

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Tag = 0x030d          |            Length             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \                                                               \
   /                    Registration Result 1                      /
   \                                                               \
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \                                                               \
   /                              ...                              /
   \                                                               \
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Tag = 0x030d          |            Length             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \                                                               \
   /                    Registration Result n                      /
   \                                                               \
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   Registration Results:  fixed length

      The Registration Results parameter contains one or more results,
      each containing the registration status for a single Link Key in
      the REG REQ message.  The number of results in a single REG RSP
      message MAY match the number of Link Key parameters found in the
      corresponding REG REQ message.  The format of each result is as
      follows:

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Local-LK-Identifier                      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Registration Status                      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Interface Identifier                     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   Local-LK-Identifier:  32-bit integer

      The Local-LK-Identifier contains the same value as found in the
      matching Link Key parameter found in the REG REQ message.  The
      format of the Local-LK-Identifier is shown in Section 3.3.4.1.

   Registration Status:  32-bit integer

      The Registration Result Status field indicates the success or the
      reason for failure of a registration request.


      The format of the Registration Status field is as follows:

       0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |         Tag = 0x030e          |         Length = 8            |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                      Registration Status                      |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   Interface Identifier:  32-bit integer

      The Interface Identifier field contains the Interface Identifier
      for the associated Link Key if the registration is successful.  It
      is set to "0" if the registration was not successful.  The format
      of integer-based and text-based Interface Identifier parameters
      are shown in Section 3.2.

	 * </pre>
	 */
	class Registration_response: public M2UAMessage {
	public:
		Registration_response();
		bool check_mandatory();
	};

	/**
	 * \brief Deregistration_request: De-Registration Request (DEREG REQ) M2UA message
	 * \details
	 * <pre>
	 *3.3.4.3  De-Registration Request (DEREG REQ)
	 *
	 *   The DEREG REQ message is sent by an ASP to indicate to a remote M2UA
	 *   peer that it wishes to de-register a given Interface Identifier.
	 *   Typically, an ASP would send this message to an SGP, and expects to
	 *   receive a DEREG RSP in return reflecting the Interface Identifier and
	 *   containing a de-registration status.
	 *
	 *
	 *   The DEREG REQ message contains the following parameter:
	 *
	 *      Interface Identifier  (mandatory)
	 *
	 *   The format for the DEREG REQ message is as follows:
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |     Tag = 0x1 or 0x3          |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   \                                                               \
	 *   /                    Interface Identifier 1                     /
	 *   \                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   \                                                               \
	 *   /                              ...                              /
	 *   \                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |     Tag = 0x1 or 0x3          |            Length             |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   \                                                               \
	 *   /                    Interface Identifier n                     /
	 *   \                                                               \
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Interface Identifier
	 *
	 *      The Interface Identifier parameter contains a Interface Identifier
	 *      indexing the Application Server traffic that the sending ASP is
	 *      currently registered to receive from the SGP but now wishes to
	 *      de-register.  The format of integer-based and text-based Interface
	 *      Identifier parameters are shown in Section 3.2.
	 *
	 * </pre>
	 */
	class Deregistration_request: public M2UAMessage {
	public:
		Deregistration_request();
		bool check_mandatory();
	};

	/**
	 * \brief Deregistration_response: De-Registration Response (DEREG RSP) M2UA message
	 * \details
	 * <pre>
3.3.4.4  De-Registration Response (DEREG RSP)

   The DEREG RSP message is used as a response to the DEREG REQ message
   from a remote M2UA peer.

   The DEREG RSP message contains the following parameter:

      De-Registration Results   (mandatory)

   The format for the DEREG RSP message is as follows:

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Tag = 0x030f          |            Length             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \                                                               \
   /                  De-Registration Result 1                     /
   \                                                               \
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \                                                               \
   /                              ...                              /
   \                                                               \
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Tag = 0x030f          |            Length             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \                                                               \
   /                  De-Registration Result n                     /
   \                                                               \
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   De-Registration Results:  fixed length

      The De-Registration Results parameter contains one or more
      results, each containing the de-registration status for a single
      Interface Identifier in the DEREG REQ message.  The number of
      results in a single DEREG RSP message MAY match the number of
      Interface Identifier parameters found in the corresponding DEREG
      REQ message.  The format of each result is as follows:

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Interface Identifier                     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                     De-Registration Status                    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   Interface Identifier:  32-bit integer

      The Interface Identifier field contains the Interface Identifier
      value of the matching Link Key to de-register, as found in the
      DEREG REQ.  The format of integer-based and text-based Interface
      Identifier parameters are shown in Section 3.2.

   De-Registration Status:  32-bit integer

      The De-Registration Result Status field indicates the success or
      the reason for failure of the de-registration.


      The format of the De-Registration Status field is as follows:

       0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |         Tag = 0x0310          |         Length = 8            |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                    De-Registration Status                     |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	 * </pre>
	 */
	class Deregistration_response: public M2UAMessage {
	public:
		Deregistration_response();
		bool check_mandatory();
	};




	/**
	 * Creates m2ua parameter object based on parameter type
	 * @param _param_type m2ua parameter type
	 * @return Pointer to m2ua parameter object or NULL if parameter not supported
	 */
	M2UAParam* get_param(ParameterType _param_type);

	/**
	 * Decodes m2ua message
	 * @param data pointer to m2ua bytes
	 * @param data_length number of bytes contained in m2ua packet
	 * @return Pointer to m2ua message object or NULL no data available
	 */
	M2UAPacket* decode(char* data, int data_length);

}


#endif /* M2UA_H_ */
