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
 * \file m3ua.h
 * \brief M3UA library
 * \details M3UA encoder/decoder library based on RFC 3332
 * \version 2.0
 * \date 2012
 * \author Damir Franusic <df@release14.org>
 */

#ifndef M3UA_H_
#define M3UA_H_
#include <vector>
#include <map>
#include <stdint.h>

namespace m3ua{


	/**
	 * <pre>
	 *3.1.2  Message Classes and Types
	 *
	 *   The following list contains the valid Message Classes:
	 *
	 *   Message Class: 8 bits (unsigned integer)
	 *
	 *   The following list contains the valid Message Type Classes:
	 *
	 *        0     Management (MGMT) Messages
	 *        1     Transfer Messages
	 *        2     SS7 Signalling Network Management (SSNM) Messages
	 *        3     ASP State Maintenance (ASPSM) Messages
	 *        4     ASP Traffic Maintenance (ASPTM) Messages
	 *        5     Reserved for Other Sigtran Adaptation Layers
	 *        6     Reserved for Other Sigtran Adaptation Layers
	 *        7     Reserved for Other Sigtran Adaptation Layers
	 *        8     Reserved for Other Sigtran Adaptation Layers
	 *        9     Routing Key Management (RKM) Messages
	 *       10 to 127 Reserved by the IETF
	 *      128 to 255 Reserved for IETF-Defined Message Class extensions
	 * </pre>
	 */
	enum MessageClass {
		__M3UA_MESSAGE_CLASS_UNKNOWN__ = -1,
		/** <b>0x00</b> - Management (MGMT) Messages */
		MGMT = 0x00,
		/** <b>0x01</b> - Transfer Messages */
		TRANSFER = 0x01,
		/** <b>0x01</b> - SS7 Signalling Network Management (SSNM) Messages */
		SSNM = 0x02,
		/** <b>0x03</b> - ASP State Maintenance (ASPSM) Messages */
		ASPSM = 0x03,
		/** <b>0x04</b> - ASP Traffic Maintenance (ASPTM) Messages */
		ASPTM = 0x04,
		/** <b>0x09</b> - Routing Key Management (RKM) Messages */
		RKM = 0x09
	};

	/**
	 * <pre>
	 *3.1.2  Message Classes and Types
	 *
	 *      Message Type: 8 bits (unsigned integer)
	 *
	 *      The following list contains the message types for the defined
	 *      messages.
	 *
	 *      Management (MGMT) Messages (See Section 3.8)
	 *
	 *        0        Error (ERR)
	 *        1        Notify (NTFY)
	 *     2 to 127    Reserved by the IETF
	 *   128 to 255    Reserved for IETF-Defined MGMT extensions
	 *
	 *      Transfer Messages (See Section 3.3)
	 *
	 *        0        Reserved
	 *        1        Payload Data (DATA)
	 *     2 to 127    Reserved by the IETF
	 *   128 to 255    Reserved for IETF-Defined Transfer extensions
	 *
	 *      SS7 Signalling Network Management (SSNM) Messages (See Section
	 *      3.4)
	 *
	 *        0        Reserved
	 *        1        Destination Unavailable (DUNA)
	 *        2        Destination Available (DAVA)
	 *        3        Destination State Audit (DAUD)
	 *        4        Signalling Congestion (SCON)
	 *        5        Destination User Part Unavailable (DUPU)
	 *        6        Destination Restricted (DRST)
	 *     7 to 127    Reserved by the IETF
	 *   128 to 255    Reserved for IETF-Defined SSNM extensions
	 *
	 *      ASP State Maintenance (ASPSM) Messages (See Section 3.5)
	 *
	 *        0        Reserved
	 *        1        ASP Up (ASPUP)
	 *        2        ASP Down (ASPDN)
	 *        3        Heartbeat (BEAT)
	 *        4        ASP Up Acknowledgement (ASPUP ACK)
	 *        5        ASP Down Acknowledgement (ASPDN ACK)
	 *        6        Heartbeat Acknowledgement (BEAT ACK)
	 *     7 to 127    Reserved by the IETF
	 *   128 to 255    Reserved for IETF-Defined ASPSM extensions
	 *
	 *      ASP Traffic Maintenance (ASPTM) Messages (See Section 3.7)
	 *
	 *        0        Reserved
	 *        1        ASP Active (ASPAC)
	 *        2        ASP Inactive (ASPIA)
	 *        3        ASP Active Acknowledgement (ASPAC ACK)
	 *        4        ASP Inactive Acknowledgement (ASPIA ACK)
	 *     5 to 127    Reserved by the IETF
	 *   128 to 255    Reserved for IETF-Defined ASPTM extensions
	 *
	 *      Routing Key Management (RKM) Messages (See Section 3.6)
	 *
	 *        0        Reserved
	 *        1        Registration Request (REG REQ)
	 *        2        Registration Response (REG RSP)
	 *        3        Deregistration Request (DEREG REQ)
	 *        4        Deregistration Response (DEREG RSP)
	 *     5 to 127    Reserved by the IETF
	 *   128 to 255    Reserved for IETF-Defined RKM extensions
	 *
	 * </pre>
	 */
	enum MessageType {
		__M3UA_MESSAGE_TYPE_UNKNOWN__ = -1,

		// MGMT
		/** <b>0x00</b> - Error (ERR) */
		M3UA_ERR = (MGMT << 8) + 0x00,
		/** <b>0x01</b> - Notify (NTFY) */
		NTFY = (MGMT << 8) + 0x01,

		// TRANSFER
		/** <b>0x01</b> - Payload Data (DATA) */
		DATA = (TRANSFER << 8) + 0x01,

		// SSNM
		/** <b>0x01</b> - Destination Unavailable (DUNA) */
		DUNA = (SSNM << 8) + 0x01,
		/** <b>0x02</b> - Destination Available (DAVA) */
		DAVA = (SSNM << 8) + 0x02,
		/** <b>0x03</b> - Destination State Audit (DAUD) */
		DAUD = (SSNM << 8) + 0x03,
		/** <b>0x04</b> - Signalling Congestion (SCON) */
		SCON = (SSNM << 8) + 0x04,
		/** <b>0x05</b> - Destination User Part Unavailable (DUPU) */
		DUPU = (SSNM << 8) + 0x05,
		/** <b>0x06</b> - Destination Restricted (DRST) */
		DRST = (SSNM << 8) + 0x06,

		// ASPSM
		/** <b>0x01</b> - ASP Up (ASPUP) */
		ASPUP = (ASPSM << 8) + 0x01,
		/** <b>0x02</b> - ASP Down (ASPDN) */
		ASPDN = (ASPSM << 8) + 0x02,
		/** <b>0x03</b> - Heartbeat (BEAT) */
		BEAT = (ASPSM << 8) + 0x03,
		/** <b>0x04</b> - ASP Up Acknowledgement (ASPUP ACK) */
		ASPUP_ACK = (ASPSM << 8) + 0x04,
		/** <b>0x05</b> - ASP Down Acknowledgement (ASPDN ACK) */
		ASPDN_ACK = (ASPSM << 8) + 0x05,
		/** <b>0x06</b> - Heartbeat Acknowledgement (BEAT ACK) */
		BEAT_ACK = (ASPSM << 8) + 0x06,

		// ASPTM
		/** <b>0x01</b> - ASP Active (ASPAC) */
		ASPAC = (ASPTM << 8) + 0x01,
		/** <b>0x02</b> - ASP Inactive (ASPIA) */
		ASPIA = (ASPTM << 8) + 0x02,
		/** <b>0x03</b> - ASP Active Acknowledgement (ASPAC ACK) */
		ASPAC_ACK = (ASPTM << 8) + 0x03,
		/** <b>0x04</b> - ASP Inactive Acknowledgement (ASPIA ACK) */
		ASPIA_ACK = (ASPTM << 8) + 0x04,

		// RKM
		/** <b>0x01</b> -  Registration Request (REG REQ) */
		REG_REQ = (RKM << 8) + 0x01,
		/** <b>0x02</b> -  Registration Response (REG RSP) */
		REG_RSP = (RKM << 8) + 0x02,
		/** <b>0x03</b> -  Deregistration Request (DEREG REQ) */
		DEREG_REQ = (RKM << 8) + 0x03,
		/** <b>0x04</b> -  Deregistration Response (DEREG RSP) */
		DEREG_RSP = (RKM << 8) + 0x04
	};

	/**
	 * <pre>
	 *      Common Parameters.  These TLV parameters are common across the
	 *      different adaptation layers:
	 *
	 *        Parameter Name                     Parameter ID
	 *        ==============                     ============
	 *        Reserved                              0x0000
	 *        Not Used in M3UA                      0x0001
	 *        Not Used in M3UA                      0x0002
	 *        Not Used in M3UA                      0x0003
	 *        INFO String                           0x0004
	 *        Not Used in M3UA                      0x0005
	 *        Routing Context                       0x0006
	 *        Diagnostic Information                0x0007
	 *        Not Used in M3UA                      0x0008
	 *        Heartbeat Data                        0x0009
	 *        Not Used in M3UA                      0x000a
	 *        Traffic Mode Type                     0x000b
	 *        Error Code                            0x000c
	 *        Status                                0x000d
	 *        Not Used in M3UA                      0x000e
	 *        Not Used in M3UA                      0x000f
	 *        Not Used in M3UA                      0x0010
	 *        ASP Identifier                        0x0011
	 *        Affected Point Code                   0x0012
	 *        Correlation ID                        0x0013
	 *
	 *      M3UA-Specific parameters.  These TLV parameters are specific to
	 *      the M3UA protocol:
	 *
	 *        Network Appearance                    0x0200
	 *        Reserved                              0x0201
	 *        Reserved                              0x0202
	 *        Reserved                              0x0203
	 *        User/Cause                            0x0204
	 *        Congestion Indications                0x0205
	 *        Concerned Destination                 0x0206
	 *        Routing Key                           0x0207
	 *        Registration Result                   0x0208
	 *        Deregistration Result                 0x0209
	 *        Local_Routing Key Identifier          0x020a
	 *        Destination Point Code                0x020b
	 *        Service Indicators                    0x020c
	 *        Reserved                              0x020d
	 *        Originating Point Code List           0x020e
	 *        Circuit Range                         0x020f
	 *        Protocol Data                         0x0210
	 *        Reserved                              0x0211
	 *        Registration Status                   0x0212
	 *        Deregistration Status                 0x0213
	 *
	 *        Reserved by the IETF             0x0214 to 0xffff
	 *
	 *      The value of 65535 is reserved for IETF-defined extensions.
	 *      Values other than those defined in specific parameter description
	 *      are reserved for use by the IETF.
	 *
	 * </pre>
	 */
	enum ParameterType {
		// Common parameters
		/** <b>0x0004</b> - INFO String */
		M3UA_PT_INFO_STRING = 0x0004,
		/** <b>0x0006</b> - Routing Context */
		M3UA_PT_ROUTING_CONTEXT = 0x0006,
		/** <b>0x0007</b> - Diagnostic Information */
		M3UA_PT_DIAGNOSTIC_INFO = 0x0007,
		/** <b>0x0009</b> - Heartbeat Data */
		M3UA_PT_HEARTBEAT = 0x0009,
		/** <b>0x000b</b> - Traffic Mode Type */
		M3UA_PT_TRAFFIC_MODE_TYPE = 0x000b,
		/** <b>0x000c</b> - Error Code */
		M3UA_PT_ERROR_CODE = 0x000c,
		/** <b>0x000d</b> - Status */
		M3UA_PT_STATUS = 0x000d,
		/** <b>0x0011</b> - ASP Identifier */
		M3UA_PT_ASP_IDENTIFIER = 0x0011,
		/** <b>0x0012</b> - Affected Point Code */
		M3UA_PT_AFFECTED_POINT_CODE = 0x0012,
		/** <b>0x0013</b> - Correlation ID */
		M3UA_PT_CORRELATION_ID = 0x0013,

		// m3ua specific
		/** <b>0x0200</b> - Network Appearance */
		M3UA_PT_NETWORK_APPEARANCE = 0x0200,
		/** <b>0x0204</b> - User/Cause */
		M3UA_PT_USER_CAUSE = 0x0204,
		/** <b>0x0205</b> - Congestion Indications */
		M3UA_PT_CONGESTION_INDICATIONS = 0x0205,
		/** <b>0x0206</b> - Concerned Destination */
		M3UA_PT_CONCERNED_DESTINATION = 0x0206,
		/** <b>0x0207</b> - Routing Key */
		M3UA_PT_ROUTING_KEY = 0x0207,
		/** <b>0x0208</b> - Registration Result */
		M3UA_PT_REGISTRATION_RESULT = 0x0208,
		/** <b>0x0209</b> - Deregistration Result */
		M3UA_PT_DEREGISTRATION_RESULT = 0x0209,
		/** <b>0x020a</b> - Local_Routing Key Identifier */
		M3UA_PT_LOCAL_ROUTING_KEY_IDENTIFIER = 0x020A,
		/** <b>0x020b</b> - Destination Point Code */
		M3UA_PT_DESTINATION_POINT_CODE = 0x020B,
		/** <b>0x020c</b> - Service Indicators */
		M3UA_PT_SERVICE_INDICATORS = 0x020C,
		/** <b>0x020e</b> - Originating Point Code List */
		M3UA_PT_ORIGINATING_POINT_CODE_LIST = 0x020E,
		/** <b>0x020f</b> - Circuit Range */
		M3UA_PT_CIRCUIT_RANGE = 0x020F,
		/** <b>0x0210</b> - Protocol Data */
		M3UA_PT_PROTOCOL_DATA = 0x0210,
		/** <b>0x0212</b> - Registration Status */
		M3UA_PT_REGISTRATION_STATUS = 0x0212,
		/** <b>0x0213</b> - Deregistration Status */
		M3UA_PT_DEREGISTRATION_STATUS = 0x0213

	};
	/**
	 * <pre>
	 *   Unavailability Cause field: 16-bits (unsigned integer)
	 *
	 *      The Unavailability Cause parameter provides the reason for the
	 *      unavailability of the MTP3-User.  The valid values for the
	 *      Unavailability Cause parameter are shown in the following table.
	 *      The values agree with those provided in the SS7 MTP3 User Part
	 *      Unavailable message.  Depending on the MTP3 protocol used in the
	 *      Network Appearance, additional values may be used - the
	 *      specification of the relevant MTP3 protocol variant/version
	 *      recommendation is definitive.
	 * </pre>
	 */
	enum CauseType {
		/** <b>0x09</b> - Unknown */
		UNKNOWN = 0x00,
		/** <b>0x01</b> - Unequipped Remote User */
		UNEQUIPPED_REMOTE_USER = 0x01,
		/** <b>0x02</b> - Inaccessible Remote User */
		INACCESSIBLE_REMOTE_USER = 0x02
	};

	/**
	 * <pre>
	 *   Traffic Mode Type: 32-bit (unsigned integer)
	 *
	 *      The Traffic Mode Type parameter identifies the traffic mode of
	 *      operation of the ASP within an AS.  The valid values for Traffic
	 *      Mode Type are shown in the following table:
	 *
	 * </pre>
	 */
	enum TMTType {
		/** <b>0x01</b> - Override */
		OVERRIDE = 0x01,
		/** <b>0x02</b> - Loadshare */
		LOADSHARE = 0x02,
		/** <b>0x03</b> - Broadcast */
		BROADCAST = 0x03
	};

	/**
	 * <pre>
	 *   Status Type: 16-bits (unsigned integer)
	 *
	 *      The Status Type parameter identifies the type of the Notify
	 *      message. The following are the valid Status Type values:
	 *
	 * </pre>
	 */
	enum StatusType {
		/** <b>0x01</b> - Application Server State Change (AS-State_Change) */
		AS_STATE_CHANGE = 0x01,
		/** <b>0x02</b> - Other */
		OTHER = 0x02
	};

	/**
	 * <pre>
	 *   Status Information: 16-bits (unsigned integer)
	 *
	 *      The Status Information parameter contains more detailed
	 *      information for the notification, based on the value of the Status
	 *      Type. If the Status Type is AS-State_Change the following Status
	 *      Information values are used:
	 *
	 *         1    reserved
	 *         2    Application Server Inactive (AS-INACTIVE)
	 *         3    Application Server Active (AS-ACTIVE)
	 *         4    Application Server Pending (AS-PENDING)
	 *
	 *      These notifications are sent from an SGP to an ASP upon a change
	 *      in status of a particular Application Server. The value reflects
	 *      the new state of the Application Server.
	 *
	 *      If the Status Type is Other, then the following Status Information
	 *      values are defined:
	 *
	 *         1    Insufficient ASP Resources Active in AS
	 *         2    Alternate ASP Active
	 *         3    ASP Failure
	 *
	 *   These notifications are not based on the SGP reporting the state
	 *   change of an ASP or AS.  In the Insufficient ASP Resources case, the
	 *   SGP is indicating to an ASP_INACTIVE ASP in the AS that another ASP
	 *   is required to handle the load of the AS (Loadsharing or Broadcast
	 *   mode). For the Alternate ASP Active case, an ASP is informed when an
	 *   alternate ASP transitions to the ASP-ACTIVE state in Override mode.
	 *   The ASP Identifier (if available) of the Alternate ASP MUST be placed
	 *   in the message.  For the ASP Failure case, the SGP is indicating to
	 *   ASP(s) in the AS that one of the ASPs has transitioned to ASP-DOWN.
	 *   The ASP Identifier (if available) of the failed ASP MUST be placed in
	 *   the message.
	 *
	 * </pre>
	 */
	enum StatusInfoType {
		// AS_STATE_CHANGE
		/** <b>0x02</b> - Application Server Inactive (AS-INACTIVE) */
		AS_INACTIVE = (AS_STATE_CHANGE << 16) + 0x02,
		/** <b>0x03</b> - Application Server Active (AS-ACTIVE) */
		AS_ACTIVE = (AS_STATE_CHANGE << 16) + 0x03,
		/** <b>0x04</b> -  Application Server Pending (AS-PENDING) */
		AS_PENDING = (AS_STATE_CHANGE << 16) + 0x04,

		// OTHER
		/** <b>0x01</b> -  Insufficient ASP Resources Active in AS */
		INSUFFICIENT_ASP_RESOURCES_ACTIVE_IN_AS = (OTHER << 16) + 0x01,
		/** <b>0x02</b> -  Alternate ASP Active */
		ALTERNATE_ASP_ACTIVE = (OTHER << 16) + 0x02,
		/** <b>0x03</b> -  ASP Failure */
		ASP_FAILURE = (OTHER << 16) + 0x03

	};

	/**
	 * <pre>
	 *   Service Indicator: 8 bits (unsigned integer)
	 *
	 *   The Service Indicator field contains the SI field from the original
	 *   SS7 message justified to the least significant bit.  Unused bits are
	 *   coded `0'.
	 * </pre>
	 */
	enum ServiceIndicatorType {
		SNMP = 0x00,
		SNTMM = 0x01,
		SNTMSM = 0x02,
		SCCP = 0x03,
		TUP = 0x04,
		ISUP = 0x05,
		DUP_CALL_AND_CIRCUIT = 0x06,
		DUP_REG_AND_CANC = 0x07,
		MTP_TESTING = 0x08,
		BISUP = 0x09,
		SISUP = 0x0a,
		GCP = 0x0e
	};

	/**
	 * <pre>
	 *   Error Code: 32-bits (unsigned integer)
	 *
	 *      The Error Code parameter indicates the reason for the Error
	 *      Message. The Error parameter value can be one of the following
	 *      values:
	 *
	 * </pre>
	 */
	enum ErrorCodeType {
		/** <b>0x01</b> -  Invalid Version */
		INVALID_VERSION = 0x01,
		/** <b>0x03</b> -  Unsupported Message Class */
		UNSUPPORTED_MESSAGE_CLASS = 0x03,
		/** <b>0x04</b> -  Unsupported Message Type */
		UNSUPPORTED_MESSAGE_TYPE = 0x04,
		/** <b>0x05</b> -  Unsupported Traffic Mode Type */
		UNSUPPORTED_TRAFFIC_MODE_TYPE = 0x05,
		/** <b>0x06</b> -  Unexpected Message */
		UNEXPECTED_MESSAGE = 0x06,
		/** <b>0x07</b> -  Protocol Error */
		PROTOCOL_ERROR = 0x07,
		/** <b>0x09</b> -  Invalid Stream Identifier */
		INVALID_STREAM_IDENTIFIER = 0x09,
		/** <b>0x0d</b> -  Refused - Management Blocking */
		REFUSED_MANAGEMENT_BLOCKING = 0x0d,
		/** <b>0x0e</b> -  ASP Identifier Required */
		ASP_IDENTIFIER_REQUIRED = 0x0e,
		/** <b>0x0f</b> -  Invalid ASP Identifier */
		INVALID_ASP_IDENTIFIER = 0x0f,
		/** <b>0x11</b> -  Invalid Parameter Value */
		INVALID_PARAMETER_VALUE = 0x11,
		/** <b>0x12</b> -  Parameter Field Error */
		PARAMETER_FIELD_ERROR = 0x12,
		/** <b>0x13</b> -  Unexpected Parameter */
		UEXPECTED_PARAMETER = 0x13,
		/** <b>0x14</b> -  Destination Status Unknown */
		DESTINATION_STATUS_UNKNOWN = 0x14,
		/** <b>0x15</b> -  Invalid Network Appearance */
		INVALID_NETWORK_APPEARANCE = 0x15,
		/** <b>0x16</b> -  Missing Parameter */
		MISSING_PARAMETER = 0x16,
		/** <b>0x19</b> -  Invalid Routing Context */
		INVALID_ROUTING_CONTEXT = 0x19,
		/** <b>0x1a</b> -  No Configured AS for ASP */
		NO_CONFIGURED_AS_FOR_ASP = 0x1a
	};




	/**
	 * \brief M3UAParam: base M3UA parameter class
	 * \details This class is a base class for all M3UA parameter classes.\n
	 * \n
	 *   <pre>
	 * 3.2 Variable Length Parameter Format
	 *
	 *   M3UA messages consist of a Common Header followed by zero or more
	 *   variable length parameters, as defined by the message type.  All the
	 *   parameters contained in a message are defined in a Tag Length-Value
	 *   format as shown below.
	 *
	 *
	 *	   0                   1                   2                   3
	 *	   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *	  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *	  |          Parameter Tag        |       Parameter Length        |
	 *	  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *	  \                                                               \
	 *	  /                       Parameter Value                         /
	 *	  \                                                               \
	 *	  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Where more than one parameter is included in a message, the
	 *   parameters may be in any order, except where explicitly mandated.  A
	 *   receiver SHOULD accept the parameters in any order.
	 *
	 *	 Parameter Tag: 16 bits (unsigned integer)
	 *
	 *		  The Tag field is a 16-bit identifier of the type of parameter. It
	 *		  takes a value of 0 to 65534.  Common parameters used by adaptation
	 *		  layers are in the range of 0x00 to 0x3f.   M3UA-specific
	 *		  parameters have Tags in the range 0x0200 to 0x02ff.  The parameter
	 *		  Tags defined are as follows:
	 *
	 *		  Common Parameters.  These TLV parameters are common across the
	 *		  different adaptation layers:
	 *
	 *			Parameter Name                     Parameter ID
	 *			==============                     ============
	 *			Reserved                              0x0000
	 *			Not Used in M3UA                      0x0001
	 *			Not Used in M3UA                      0x0002
	 *			Not Used in M3UA                      0x0003
	 *			INFO String                           0x0004
	 *			Not Used in M3UA                      0x0005
	 *			Routing Context                       0x0006
	 *			Diagnostic Information                0x0007
	 *			Not Used in M3UA                      0x0008
	 *			Heartbeat Data                        0x0009
	 *			Not Used in M3UA                      0x000a
	 *			Traffic Mode Type                     0x000b
	 *			Error Code                            0x000c
	 *			Status                                0x000d
	 *			Not Used in M3UA                      0x000e
	 *			Not Used in M3UA                      0x000f
	 * 			Not Used in M3UA                      0x0010
	 * 			ASP Identifier                        0x0011
	 * 			Affected Point Code                   0x0012
	 * 			Correlation ID                        0x0013
	 *
	 * 		  M3UA-Specific parameters.  These TLV parameters are specific to
	 * 		  the M3UA protocol:
	 *
	 * 			Network Appearance                    0x0200
	 * 			Reserved                              0x0201
	 * 			Reserved                              0x0202
	 * 			Reserved                              0x0203
	 * 			User/Cause                            0x0204
	 * 			Congestion Indications                0x0205
	 * 			Concerned Destination                 0x0206
	 * 			Routing Key                           0x0207
	 * 			Registration Result                   0x0208
	 * 			Deregistration Result                 0x0209
	 * 			Local_Routing Key Identifier          0x020a
	 * 			Destination Point Code                0x020b
	 * 			Service Indicators                    0x020c
	 * 			Reserved                              0x020d
	 * 			Originating Point Code List           0x020e
	 * 			Circuit Range                         0x020f
	 * 			Protocol Data                         0x0210
	 * 			Reserved                              0x0211
	 * 			Registration Status                   0x0212
	 * 			Deregistration Status                 0x0213
	 *
	 * 			Reserved by the IETF             0x0214 to 0xffff
	 *
	 *		  The value of 65535 is reserved for IETF-defined extensions.
	 *		  Values other than those defined in specific parameter description
	 *		  are reserved for use by the IETF.
	 *
	 *	 Parameter Length: 16 bits (unsigned integer)
	 *
	 *		  The Parameter Length field contains the size of the parameter in
	 *		  bytes, including the Parameter Tag, Parameter Length, and
	 *		  Parameter Value fields.  Thus, a parameter with a zero-length
	 *		  Parameter Value field would have a Length field of 4.  The
	 *		  Parameter Length does not include any padding bytes.
	 *
	 *	 Parameter Value: variable length.
	 *
	 *		  The Parameter Value field contains the actual information to be
	 *		  transferred in the parameter.
	 *
	 *		  The total length of a parameter (including Tag, Parameter Length
	 *		  and Value fields) MUST be a multiple of 4 bytes.  If the length of
	 *		  the parameter is not a multiple of 4 bytes, the sender pads the
	 *		  Parameter at the end (i.e., after the Parameter Value field) with
	 *		  all zero bytes.  The length of the padding is NOT included in the
	 *		  parameter length field.  A sender SHOULD NOT pad with more than 3
	 *		  bytes.  The receiver MUST ignore the padding bytes.
	 *
	 *   </pre>
	 */
	class M3UAParam {
	public:
		M3UAParam();
		virtual ~M3UAParam();
		ParameterType type;
		int byte_pos;
		int value_length;
		//bool new_value_mem;
		unsigned char* value;
		virtual void init(unsigned char* data, int data_length);
		virtual void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	class LocalRoutingKeyId: public M3UAParam {
	public:
		LocalRoutingKeyId();
	};

	class DestinationPointCode: public M3UAParam {
	public:
		DestinationPointCode();
	};

	class ServiceIndicators: public M3UAParam {
	public:
		ServiceIndicators();
	};

	class OriginatingPointCodeList: public M3UAParam {
	public:
		OriginatingPointCodeList();
	};

	class CircuitRange: public M3UAParam {
	public:
		CircuitRange();
	};

	class RegistrationStatus: public M3UAParam {
	public:
		RegistrationStatus();
	};

	class DeregistrationStatus: public M3UAParam {
	public:
		DeregistrationStatus();
	};

	/**
	 * \brief AffectedPointCode: Affected Point Code M3UA parameter
	 * \details
	 * <pre>
	 *   Affected Point Code: n x 32-bits
	 *
	 *      The Affected Point Code parameter contains a list of Affected
	 *      Destination Point Code fields, each a three-octet parameter to
	 *      allow for 14-, 16- and 24-bit binary formatted SS7 Point Codes.
	 *      Affected Point Codes that are less than 24-bits, are padded on the
	 *      left to the 24-bit boundary.  The encoding is shown below for ANSI
	 *      and ITU Point Code examples.
	 *
	 *   ANSI 24-bit Point Code:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |     Mask      |    Network    |    Cluster    |     Member    |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *                      |MSB-----------------------------------------LSB|
	 *
	 *   ITU 14-bit Point Code:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |     Mask      |0 0 0 0 0 0 0 0 0 0|Zone |     Region    | SP  |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *                                           |MSB--------------------LSB|
	 *
	 *      It is optional to send an Affected Point Code parameter with more
	 *      than one Affected PC but it is mandatory to receive it.  Including
	 *      multiple Affected PCs may be useful when reception of an MTP3
	 *      management message or a linkset event simultaneously affects the
	 *      availability status of a list of destinations at an SG.
	 *
	 * </pre>
	 */
	class AffectedPointCode : public M3UAParam{
	public:
		AffectedPointCode();
	};

	/**
	 * \brief ASPIdentifier: ASP Identifier M3UA parameter
	 * \details
	 * <pre>
	 *   ASP Identifier: 32-bit unsigned integer
	 *
	 *      The optional ASP Identifier parameter contains a unique value that
	 *      is locally significant among the ASPs that support an AS.  The SGP
	 *      should save the ASP Identifier to be used, if necessary, with the
	 *      Notify message (see Section 3.8.2).
	 *
	 *      The format and description of the optional INFO String parameter
	 *      is the same as for the DUNA message (See Section 3.4.1).
	 *
	 * </pre>
	 */
	class ASPIdentifier : public M3UAParam{
	public:
		ASPIdentifier();
	};

	/**
	 * \brief ConcernedDestination: Concerned Destination M3UA parameter
	 * \details
	 * <pre>
	 *   Concerned Destination: 32-bits
	 *
	 *      The optional Concerned Destination parameter is only used if the
	 *      SCON message is sent from an ASP to the SGP. It contains the point
	 *      code of the originator of the message that triggered the SCON
	 *      message. The Concerned Destination parameter contains one
	 *      Concerned Destination Point Code field, a three-octet parameter to
	 *      allow for 14-, 16- and 24-bit binary formatted SS7 Point Codes.  A
	 *      Concerned Point Code that is less than 24-bits is padded on the
	 *      left to the 24-bit boundary.  Any resulting Transfer Controlled
	 *      (TFC) message from the SG is sent to the Concerned Point Code
	 *      using the single Affected DPC contained in the SCON message to
	 *      populate the (affected) Destination field of the TFC message
	 *
	 * </pre>
	 */
	class ConcernedDestination : public M3UAParam{
	public:
		ConcernedDestination();
	};

	/**
	 * \brief CongestionIndications: Congested Indications M3UA parameter
	 * \details
	 * <pre>
	 *   Congested Indications: 32-bits
	 *
	 *      The optional Congestion Indications parameter contains a
	 *      Congestion Level field.  This optional parameter is used to
	 *      communicate congestion levels in national MTP networks with
	 *      multiple congestion thresholds, such as in ANSI MTP3.  For MTP
	 *      congestion methods without multiple congestion levels (e.g., the
	 *      ITU international method) the parameter is not included.
	 *
	 * </pre>
	 */
	class CongestionIndications : public M3UAParam{
	public:
		CongestionIndications();
		int level;
		void init(unsigned char* data, int data_length);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief CorrelationId: Correlation Id M3UA parameter
	 * \details
	 * <pre>
	 *   Correlation Id: 32-bits (unsigned integer)
	 *
	 *   The Correlation Id parameter uniquely identifies the MSU carried in
	 *   the Protocol Data within an AS.  This Correlation Id parameter is
	 *   assigned by the sending M3UA.
	 *
	 * </pre>
	 */
	class CorrelationId : public M3UAParam{
	public:
		CorrelationId();
		int correlation_id;
		void init(unsigned char* data, int data_length);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief DeregistrationResult: Deregistration Result M3UA parameter
	 * \details
	 * <pre>
	 *   Deregistration Results:
	 *
	 *      The Deregistration Result parameter contains the deregistration
	 *      status for a single Routing Context in a DEREG REQ message.  The
	 *      number of results in a single DEREG RSP message MAY be anywhere
	 *      from one to the total number of number of Routing Context values
	 *      found in the corresponding DEREG REQ message.
	 *
	 * </pre>
	 */
	class DeregistrationResult : public M3UAParam{
	public:
		DeregistrationResult();
	};

	/**
	 * \brief DiagnosticInformation: Diagnostic Information M3UA parameter
	 * \details
	 * <pre>
	 *   Diagnostic Information: variable length
	 *
	 *      When included, the optional Diagnostic information can be any
	 *      information germane to the error condition, to assist in
	 *      identification of the error condition. The Diagnostic information
	 *      SHOULD contain the offending message.
	 *
	 * </pre>
	 */
	class DiagnosticInformation : public M3UAParam {
	public:
		DiagnosticInformation();
	};

	/**
	 * \brief ErrorCode: Error Code M3UA parameter
	 * \details
	 * <pre>
	 *  Error Code: 32-bits (unsigned integer)
	 *
	 *      The Error Code parameter indicates the reason for the Error
	 *      Message.
	 *
	 * </pre>
	 */
	class ErrorCode : public M3UAParam{
	public:
		ErrorCode();
		ErrorCodeType error_code_type;
		void init(unsigned char* data, int data_length);
		void set_error_code(int val);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief HeartbeatData: Heartbeat Data M3UA parameter
	 * \details
	 * <pre>
	 *  The Heartbeat Data parameter contents are defined by the sending
	 *   node. The Heartbeat Data could include, for example, a Heartbeat
	 *   Sequence Number and/or Timestamp.  The receiver of a BEAT message
	 *   does not process this field as it is only of significance to the
	 *   sender.  The receiver MUST respond with a BEAT Ack message.
	 *
	 * </pre>
	 */
	class HeartbeatData : public M3UAParam{
	public:
		HeartbeatData();

	};

	/**
	 * \brief InfoString: INFO String M3UA parameter
	 * \details
	 * <pre>
	 *   INFO String: variable length
	 *
	 *      The optional INFO String parameter can carry any meaningful UTF-8
	 *      [10] character string along with the message.  Length of the INFO
	 *      String parameter is from 0 to 255 octets.  No procedures are
	 *      presently identified for its use but the INFO String MAY be used
	 *      for debugging purposes.
	 *
	 * </pre>
	 */
	class InfoString : public M3UAParam{
	public:
		InfoString();
	};

	/**
	 * \brief NetworkAppearance: Netowrk Appearance M3UA parameter
	 * \details
	 * <pre>
	 *  Network Appearance - The Network Appearance is a M3UA local reference
	 *   shared by SG and AS (typically an integer) that together with an
	 *   Signaling Point Code uniquely identifies an SS7 node by indicating
	 *   the specific SS7 network it belongs to. It can be used to distinguish
	 *   between signalling traffic associated with different networks being
	 *   sent between the SG and the ASP over a common SCTP association. An
	 *   example scenario is where an SG appears as an element in multiple
	 *   separate national SS7 networks and the same Signaling Point Code
	 *   value may be reused in different networks.
	 *
	 * </pre>
	 */
	class NetworkAppearance : public M3UAParam{
	public:
		NetworkAppearance();
		int network_appearance;
		void init(unsigned char* data, int data_length);
		void set_network_appearance(int val);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief ProtocolData: Protocol Data M3UA parameter
	 * \details
	 * <pre>
	 *   Protocol Data: variable length
	 *
	 *      The Protocol Data parameter contains the original SS7 MTP3
	 *      message, including the Service Information Octet and Routing
	 *      Label.
	 *
	 * </pre>
	 *
	 */
	class ProtocolData : public M3UAParam{
	public:
		ProtocolData();
		void init(unsigned char* data, int data_length);
		ServiceIndicatorType service_indicator;
		uint8_t network_indicator;
		uint8_t message_priority;
		uint32_t destination_point_code;
		uint32_t originating_point_code;
		uint8_t signalling_link_selection_code;
		int user_protocol_data_length;
		unsigned char* user_protocol_data;
		void set_opc(uint32_t value);
		void set_dpc(uint32_t value);
		void set_si(int value);
		void set_ni(uint8_t value);
		void set_mp(uint8_t value);
		void set_sls(uint8_t value);
		void set_user_protocol_data(unsigned char* data, int data_length);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};

	/**
	 * \brief RegistrationResult: Registration Result M3UA parameter
	 * \details
	 * <pre>
	 *   Registration Results:
	 *
	 *      The Registration Result parameter contains the registration result
	 *      for a single Routing Key in an REG REQ message.  The number of
	 *      results in a single REG RSP message MUST be anywhere from one to
	 *      the total number of number of Routing Key parameters found in the
	 *      corresponding REG REQ message.  Where multiple REG RSP messages
	 *      are used in reply to REG REQ message, a specific result SHOULD be
	 *      in only one REG RSP message.  The format of each result is as
	 *      follows:
	 *
	 * </pre>
	 */
	class RegistrationResult : public M3UAParam{
	public:
		RegistrationResult();
	};

	/**
	 * \brief RoutingContext: Routing Context M3UA parameter
	 * \details
	 * <pre>
	 *   Routing Context: 32-bits (unsigned integer)
	 *
	 *      The Routing Context parameter contains the Routing Context value
	 *      associated with the DATA message.   Where a Routing Key has not
	 *      been coordinated between the SGP and ASP, sending of Routing
	 *      Context is not required.  Where multiple Routing Keys and Routing
	 *      Contexts are used across a common association, the Routing Context
	 *      MUST be sent to identify the traffic flow, assisting in the
	 *      internal distribution of Data messages.
	 *
	 * </pre>
	 */
	class RoutingContext : public M3UAParam{
	public:
		RoutingContext();
		int routing_context;
		void init(unsigned char* data, int data_length);
		void set_routing_context(int val);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief RoutingKey: Routing Key M3UA parameter
	 * \details
	 * <pre>
	 *   Routing Key: A Routing Key describes a set of SS7 parameters and
	 *   parameter values that uniquely define the range of signalling traffic
	 *   to be handled by a particular Application Server. Parameters within
	 *   the Routing Key cannot extend across more than a single Signalling
	 *   Point Management Cluster.
	 *
	 * </pre>
	 */
	class RoutingKey : public M3UAParam{
	public:
		RoutingKey();
	};

	/**
	 * \brief Status: Notify Status M3UA Parameter
	 * \details
	 * <pre>
	 *   Status Type: 16-bits (unsigned integer)
	 *
	 *      The Status Type parameter identifies the type of the Notify
	 *      message.
	 *
	 *   Status Information: 16-bits (unsigned integer)
	 *
	 *      The Status Information parameter contains more detailed
	 *      information for the notification, based on the value of the Status
	 *      Type.
	 *
	 * </pre>
	 */
	class Status : public M3UAParam{
	public:
		Status();
		StatusType status_type;
		StatusInfoType status_info;
		void init(unsigned char* data, int data_length);
		void set_status(StatusInfoType info);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief TrafficModeType: Traffic Mode Type M3UA parameter
	 * \details
	 * <pre>
	 *   Traffic Mode Type: 32-bit (unsigned integer)
	 *
	 *   The optional Traffic Mode Type parameter identifies the traffic mode
	 *   of operation of the ASP(s) within an Application Server
	 *
	 * </pre>
	 */
	class TrafficModeType : public M3UAParam{
	public:
		TrafficModeType();
		TMTType traffic_mode_type;
		void init(unsigned char* data, int data_length);
		void set_tmt(TMTType tmt);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	/**
	 * \brief UserCause: User/Cause M3UA parameter
	 * \details
	 * <pre>
	 *   User/Cause: 32-bits
	 *
	 *      The Unavailability Cause and MTP3-User Identity fields, associated
	 *      with the Affected PC in the Affected Point Code parameter, are
	 *      encoded as follows:
	 *
	 *   Unavailability Cause field: 16-bits (unsigned integer)
	 *
	 *      The Unavailability Cause parameter provides the reason for the
	 *      unavailability of the MTP3-User.  The valid values for the
	 *      Unavailability Cause parameter are shown in the following table.
	 *      The values agree with those provided in the SS7 MTP3 User Part
	 *      Unavailable message.  Depending on the MTP3 protocol used in the
	 *      Network Appearance, additional values may be used - the
	 *      specification of the relevant MTP3 protocol variant/version
	 *      recommendation is definitive.
	 *
	 *         0         Unknown
	 *         1         Unequipped Remote User
	 *         2         Inaccessible Remote User
	 *
	 *   MTP3-User Identity field: 16-bits (unsigned integer)
	 *
	 *      The MTP3-User Identity describes the specific MTP3-User that is
	 *      unavailable (e.g., ISUP, SCCP, ...).  Some of the valid values for
	 *      the MTP3-User Identity are shown below.  The values align with
	 *      those provided in the SS7 MTP3 User Part Unavailable message and
	 *      Service Indicator.  Depending on the MTP3 protocol variant/version
	 *      used in the network appearance, additional values may be used.
	 *      The relevant MTP3 protocol variant/version recommendation is
	 *      definitive.
	 *
	 *          0 to 2   Reserved
	 *             3     SCCP
	 *             4     TUP
	 *             5     ISUP
	 *          6 to 8  Reserved
	 *             9     Broadband ISUP
	 *            10     Satellite ISUP
	 *            11     Reserved
	 *            12     AAL type 2 Signalling
	 *            13     Bearer Independent Call Control (BICC)
	 *            14     Gateway Control Protocol
	 *            15     Reserved
	 *
	 *      The format and description of the Affected Point Code parameter is
	 *      the same as for the DUNA message (See Section 3.4.1.) except that
	 *      the Mask field is not used and only a single Affected DPC is
	 *      included.  Ranges and lists of Affected DPCs cannot be signaled in
	 *      a DUPU message, but this is consistent with UPU operation in the
	 *      SS7 network.  The Affected Destinations parameter in an MTP3 User
	 *      Part Unavailable message (UPU) received by an SGP from the SS7
	 *      network contains only one destination.
	 *
	 *      The format and description of the Network Appearance, Routing
	 *      Context, and INFO String parameters is the same as for the DUNA
	 *      message (See Section 3.4.1).
	 *
	 * </pre>
	 */
	class UserCause : public M3UAParam{
	public:
		UserCause();
		int user;
		CauseType cause;
		void init(unsigned char* data, int data_length);
		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
	};

	// fwd declaration of M3UA pool
	class M3UAPool;

	/**
	 * \brief M3UAMessage: base M3UA message class
	 * \details This class is a base class for all M3UA message types
	 */
	class M3UAMessage {
	public:
		M3UAMessage();
		virtual ~M3UAMessage();
		MessageType type;
		unsigned int byte_pos;
		virtual void init(unsigned char* data, unsigned int data_length, M3UAPool* _m3ua_pool);
		virtual void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);
		int get_params_count();
		void clear_params();
		M3UAParam* get_parameter(ParameterType _param_type);
		M3UAParam* add_parameter(M3UAParam* param);
		virtual bool check_mandatory() = 0;
		std::vector<M3UAParam*> params;
	protected:
		void process_params(unsigned char* data, unsigned int data_length, M3UAPool* _m3ua_pool);
		int encode_parameter(M3UAParam* param, unsigned char* buffer, int buffer_length, int offset);


	};

	/**
	 * \brief Scon: Signalling Congestion (SCON) M3UA message
	 * \details
	 * <pre>
	 * 3.4.4 Signalling Congestion (SCON)
	 *
	 *   The SCON message can be sent from an SGP to all concerned ASPs to
	 *   indicate that an SG has determined that there is congestion in the
	 *   SS7 network to one or more destinations, or to an ASP in response to
	 *   a DATA or DAUD message as appropriate.  For some MTP protocol
	 *   variants (e.g., ANSI MTP) the SCON message may be sent when the SS7
	 *   congestion level changes.  The SCON message MAY also be sent from the
	 *   M3UA layer of an ASP to an M3UA peer indicating that the M3UA layer
	 *   or the ASP is congested.
	 *
	 *   The SCON message contains the following parameters:
	 *
	 *      Network Appearance       Optional
	 *      Routing Context          Optional
	 *      Affected Point Code      Mandatory
	 *      Concerned Destination    Optional
	 *      Congestion Indications   Optional
	 *      INFO String              Optional
	 *
	 *   The format for SCON Message parameters is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0200          |           Length = 8          |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                       Network Appearance                      |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0006           |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                       Routing Context                         /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0012          |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |      Mask     |                 Affected PC 1                 |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                              ...                              /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |      Mask     |                 Affected PC n                 |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0206          |             Length = 8        |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |    reserved   |                 Concerned DPC                 |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0205          |             Length = 8        |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                   Reserved                    |  Cong. Level  |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |            Tag = 0x0004       |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                         INFO String                           /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *
	 *   The format and description of the Network Appearance, Routing
	 *   Context, Affected Point Code, and INFO String parameters is the same
	 *   as for the DUNA message (See Section 3.4.1).
	 *
	 *   The Affected Point Code parameter can be used to indicate congestion
	 *   of multiple destinations or ranges of destinations.
	 *
	 *   Concerned Destination: 32-bits
	 *
	 *      The optional Concerned Destination parameter is only used if the
	 *      SCON message is sent from an ASP to the SGP. It contains the point
	 *      code of the originator of the message that triggered the SCON
	 *      message. The Concerned Destination parameter contains one
	 *      Concerned Destination Point Code field, a three-octet parameter to
	 *      allow for 14-, 16- and 24-bit binary formatted SS7 Point Codes.  A
	 *      Concerned Point Code that is less than 24-bits is padded on the
	 *      left to the 24-bit boundary.  Any resulting Transfer Controlled
	 *      (TFC) message from the SG is sent to the Concerned Point Code
	 *      using the single Affected DPC contained in the SCON message to
	 *      populate the (affected) Destination field of the TFC message
	 *
	 *   Congested Indications: 32-bits
	 *
	 *      The optional Congestion Indications parameter contains a
	 *      Congestion Level field.  This optional parameter is used to
	 *      communicate congestion levels in national MTP networks with
	 *      multiple congestion thresholds, such as in ANSI MTP3.  For MTP
	 *      congestion methods without multiple congestion levels (e.g., the
	 *      ITU international method) the parameter is not included.
	 *
	 *   Congestion Level field: 8-bits (unsigned integer)
	 *
	 *      The Congestion Level field, associated with all of the Affected
	 *      DPC(s) in the Affected Destinations parameter, contains one of the
	 *      following values:
	 *
	 *         0     No Congestion or Undefined
	 *         1     Congestion Level 1
	 *         2     Congestion Level 2
	 *         3     Congestion Level 3
	 *
	 *
	 *
	 *
	 *      The congestion levels are defined in the congestion method in the
	 *      appropriate national MTP recommendations [7,8].
	 *
	 *
	 * </pre>
	 */
	class Scon : public M3UAMessage {
	public:
		Scon();
		bool check_mandatory();
	};

	/**
	 * \brief Reg_rsp: Registration Response (REG RSP) M3UA message
	 * \details
	 * <pre>
	 *		3.6.2 Registration Response (REG RSP)
	 *
	 *		   The REG RSP message is used as a response to the REG REQ message from
	 *		   a remote M3UA peer.  It contains indications of success/failure for
	 *		   registration requests and returns a unique Routing Context value for
	 *		   successful registration requests, to be used in subsequent M3UA
	 *		   Traffic Management protocol.
	 *
	 *		   The REG RSP message contains the following parameters:
	 *
	 *		   Registration Result   Mandatory
	 *
	 *		   One or more Registration Result parameters MUST be included.  The
	 *		   format for the REG RSP message is as follows:
	 *
	 *			   0                   1                   2                   3
	 *			   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |          Tag = 0x0208         |              Length           |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |                    Registration Result 1                      |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  \                                                               \
	 *			  /                              ...                              /
	 *			  \                                                               \
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |           Tag = 0x0208        |            Length             |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |                    Registration Result n                      |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *		   Registration Results:
	 *
	 *			  The Registration Result parameter contains the registration result
	 *			  for a single Routing Key in an REG REQ message.  The number of
	 *			  results in a single REG RSP message MUST be anywhere from one to
	 *			  the total number of number of Routing Key parameters found in the
	 *			  corresponding REG REQ message.  Where multiple REG RSP messages
	 *			  are used in reply to REG REQ message, a specific result SHOULD be
	 *			  in only one REG RSP message.  The format of each result is as
	 *			  follows:
	 *
	 *			   0                   1                   2                   3
	 *			   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |         Tag = 0x020a        |          Length = 8             |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |                  Local-RK-Identifier value                    |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |           Tag = 0x0212      |          Length = 8             |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |                      Registration Status                      |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |           Tag = 0x0006      |          Length = 8             |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *			  |                        Routing Context                        |
	 *			  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *		   Local-RK-Identifier: 32-bit integer
	 *
	 *			  The Local-RK-Identifier contains the same value as found in the
	 *			  matching Routing Key parameter found in the REG REQ message (See
	 *			  Section 3.6.1).
	 *
	 *		   Registration Status: 32-bit integer
	 *
	 *			  The Registration Result Status field indicates the success or the
	 *			  reason for failure of a registration request.
	 *
	 *			  Its values may be:
	 *
	 *				 0           Successfully Registered
	 *				 1           Error - Unknown
	 *				 2           Error - Invalid DPC
	 *				 3           Error - Invalid Network Appearance
	 *				 4           Error - Invalid Routing Key
	 *				 5           Error - Permission Denied
	 *				 6           Error - Cannot Support Unique Routing
	 *				 7           Error - Routing Key not Currently Provisioned
	 *				 8           Error - Insufficient Resources
	 *				 9           Error - Unsupported RK parameter Field
	 *				 10          Error - Unsupported/Invalid Traffic Handling Mode
	 *
	 *		   Routing Context: 32-bit integer
	 *
	 *			  The Routing Context field contains the Routing Context value for
	 *			  the associated Routing Key if the registration was successful.  It
	 *			  is set to "0" if the registration was not successful.
	 *
	 * </pre>
	 */
	class Reg_rsp : public M3UAMessage {
	public:
		Reg_rsp();
		bool check_mandatory();
	};

	/**
	 * \brief Reg_req: Registration Request (REG REQ) M3UA message
	 * \details
	 * <pre>
	 *	3.6.1 Registration Request (REG REQ)
	 *
	 *	   The REG REQ message is sent by an ASP to indicate to a remote M3UA
	 *	   peer that it wishes to register one or more given Routing Keys with
	 *	   the remote peer.  Typically, an ASP would send this message to an
	 *	   SGP, and expects to receive a REG RSP message in return with an
	 *	   associated Routing Context value.
	 *
	 *	   The REG REQ message contains the following parameters:
	 *
	 *	   Routing Key           Mandatory
	 *
	 *	   One or more Routing Key parameters MAY be included.  The format for
	 *	   the REG REQ message is as follows:
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |          Tag = 0x0207         |            Length             |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  \                                                               \
	 *		  /                         Routing Key 1                         /
	 *		  \                                                               \
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  \                                                               \
	 *		  /                              ...                              /
	 *		  \                                                               \
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |          Tag = 0x0207         |            Length             |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  \                                                               \
	 *		  /                         Routing Key n                         /
	 *		  \                                                               \
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *	   Routing Key: variable length
	 *
	 *		  The Routing Key parameter is mandatory. The sender of this message
	 *		  expects that the receiver of this message will create a Routing
	 *		  Key entry and assign a unique Routing Context value to it, if the
	 *		  Routing Key entry does not already exist.
	 *
	 *		  The Routing Key parameter may be present multiple times in the
	 *		  same message. This is used to allow the registration of multiple
	 *		  Routing Keys in a single message.
	 *
	 *	   The format of the Routing Key parameter is as follows.
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                       Local-RK-Identifier                     |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                  Traffic Mode Type (optional)                 |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                     Destination Point Code                    |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                  Network Appearance (optional)                |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                  Service Indicators (optional)                |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |              Originating Point Code List (optional)           |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                   Circuit Range List (optional)               |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  \                                                               \
	 *		  /                              ...                              /
	 *		  \                                                               \
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                     Destination Point Code                    |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                  Service Indicators (optional)                |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |              Originating Point Code List (optional)           |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                   Circuit Range List (optional)               |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *		  Note: The Destination Point Code, Service Indicators, Originating
	 *		  Point Code List and Circuit Range List parameters MAY be repeated
	 *		  as a grouping within the Routing Key parameter, in the structure
	 *		  shown above.
	 *
	 *	   Local-RK-Identifier: 32-bit unsigned integer
	 *
	 *		  The mandatory Local-RK-Identifier field is used to uniquely
	 *		  identify the registration request.  The Identifier value is
	 *		  assigned by the ASP, and is used to correlate the response in an
	 *		  REG RSP message with the original registration request.  The
	 *		  Identifier value must remain unique until the REG RSP message is
	 *		  received.
	 *
	 *	   The format of the Local-RK-Identifier field is as follows:
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x020a          |         Length = 8            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                  Local-RK-Identifier value                    |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *	   Traffic Mode Type: 32-bit (unsigned integer)
	 *
	 *	   The optional Traffic Mode Type parameter identifies the traffic mode
	 *	   of operation of the ASP(s) within an Application Server.  The format
	 *	   of the Traffic Mode Type Identifier is as follows:
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x000b          |         Length = 8            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                       Traffic Mode Type                       |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *	   The valid values for Traffic Mode Type are shown in the following
	 *	   table:
	 *
	 *			 1     Override
	 *			 2     Loadshare
	 *			 3     Broadcast
	 *
	 *	   Destination Point Code:
	 *
	 *		  The Destination Point Code parameter is mandatory, and identifies
	 *		  the Destination Point Code of incoming SS7 traffic for which the
	 *		  ASP is registering.  The format is the same as described for the
	 *		  Affected Destination parameter in the DUNA message (See Section
	 *		  3.4.1).  Its format is:
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x020b          |         Length = 8            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |    Mask = 0   |            Destination Point Code             |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *	   Network Appearance:
	 *
	 *		  The optional Network Appearance parameter field identifies the SS7
	 *		  network context for the Routing Key, and has the same format as in
	 *		  the DATA message (See Section 3.3.1).  The absence of the Network
	 *		  Appearance parameter in the Routing Key indicates the use of any
	 *		  Network Appearance value.  Its format is:
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x0200          |         Length = 8            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                     Network Appearance                        |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *	   Service Indicators (SI): n X 8-bit integers
	 *
	 *		  The optional SI [7,8] field contains one or more Service
	 *		  Indicators from the values as described in the MTP3-User Identity
	 *		  field of the DUPU message.  The absence of the SI parameter in the
	 *		  Routing Key indicates the use of any SI value, excluding of course
	 *		  MTP management.  Where an SI parameter does not contain a multiple
	 *		  of four SIs, the parameter is padded out to 32-byte alignment.
	 *
	 *		  The SI format is:
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x020c          |             Length            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |      SI #1    |     SI #2     |    SI #3      |    SI #4      |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  /                              ...                              /
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |      SI #n    |             0 Padding, if necessary           |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *	   OPC List:
	 *
	 *		  The Originating Point Code List parameter contains one or more SS7
	 *		  OPC entries, and its format is the same as the Destination Point
	 *		  Code parameter.  The absence of the OPC List parameter in the
	 *		  Routing Key indicates the use of any OPC value,
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x020e          |             Length            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |    Mask = 0   |          Origination Point Code #1            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |    Mask = 0   |          Origination Point Code #2            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  /                              ...                              /
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |    Mask = 0   |          Origination Point Code #n            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *	   Circuit Range:
	 *
	 *		  An ISUP controlled circuit is uniquely identified by the SS7 OPC,
	 *		  DPC and CIC value.  For the purposes of identifying Circuit Ranges
	 *		  in an M3UA Routing Key, the optional Circuit Range parameter
	 *		  includes one or more circuit ranges, each identified by an OPC and
	 *		  Upper/Lower CIC value.  The DPC is implicit as it is mandatory and
	 *		  already included in the DPC parameter of the Routing Key.  The
	 *		  absence of the Circuit Range parameter in the Routing Key
	 *		  indicates the use of any Circuit Range values, in the case of
	 *		  ISUP/TUP traffic.  The Origination Point Code is encoded the same
	 *		  as the Destination Point Code parameter, while the CIC values are
	 *		  16-bit integers.
	 *
	 *
	 *
	 *	   The Circuit Range format is as follows:
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |           Tag = 0x020f        |              Length           |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |    Mask = 0   |          Origination Point Code #1            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |       Lower CIC Value #1      |      Upper CIC Value #1       |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |    Mask = 0   |          Origination Point Code #2            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |       Lower CIC Value #2      |      Upper CIC Value #2       |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  /                              ...                              /
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 * 		  |    Mask = 0   |          Origination Point Code #n            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |       Lower CIC Value #n      |      Upper CIC Value #n       |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 * </pre>
	 */
	class Reg_req : public M3UAMessage {
	public:
		Reg_req();
		bool check_mandatory();
	};

	/**
	 * \brief Ntfy: Notify (NTFY) M3UA message
	 * \details
	 * <pre>
	 *3.8.2 Notify
	 *
	 *   The Notify message used to provide an autonomous indication of M3UA
	 *   events to an M3UA peer.
	 *
	 *   The Notify message contains the following parameters:
	 *
	 *      Status                     Mandatory
	 *      ASP Identifier             Optional
	 *      Routing Context            Optional
	 *      INFO String                Optional
	 *
	 *   The format for the Notify message is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x000d           |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Status Type            |       Status Information      |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0011           |             Length = 8        |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                        ASP Identifier                         |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0006           |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                       Routing Context                         /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0004          |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                          INFO String                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Status Type: 16-bits (unsigned integer)
	 *
	 *      The Status Type parameter identifies the type of the Notify
	 *      message. The following are the valid Status Type values:
	 *
	 *         1     Application Server State Change (AS-State_Change)
	 *         2     Other
	 *
	 *   Status Information: 16-bits (unsigned integer)
	 *
	 *      The Status Information parameter contains more detailed
	 *      information for the notification, based on the value of the Status
	 *      Type. If the Status Type is AS-State_Change the following Status
	 *      Information values are used:
	 *
	 *         1    reserved
	 *         2    Application Server Inactive (AS-INACTIVE)
	 *         3    Application Server Active (AS-ACTIVE)
	 *         4    Application Server Pending (AS-PENDING)
	 *
	 *      These notifications are sent from an SGP to an ASP upon a change
	 *      in status of a particular Application Server. The value reflects
	 *      the new state of the Application Server.
	 *
	 *      If the Status Type is Other, then the following Status Information
	 *      values are defined:
	 *
	 *         1    Insufficient ASP Resources Active in AS
	 *         2    Alternate ASP Active
	 *         3    ASP Failure
	 *
	 *   These notifications are not based on the SGP reporting the state
	 *   change of an ASP or AS.  In the Insufficient ASP Resources case, the
	 *   SGP is indicating to an ASP_INACTIVE ASP in the AS that another ASP
	 *   is required to handle the load of the AS (Loadsharing or Broadcast
	 *   mode). For the Alternate ASP Active case, an ASP is informed when an
	 *   alternate ASP transitions to the ASP-ACTIVE state in Override mode.
	 *   The ASP Identifier (if available) of the Alternate ASP MUST be placed
	 *   in the message.  For the ASP Failure case, the SGP is indicating to
	 *   ASP(s) in the AS that one of the ASPs has transitioned to ASP-DOWN.
	 *   The ASP Identifier (if available) of the failed ASP MUST be placed in
	 *   the message.
	 *
	 *   The format and description of the optional ASP Identifier is the same
	 *   as for the ASP Up message (See Section 3.5.1).  The format and
	 *   description of the Routing Context and Info String parameters is the
	 *   same as for the ASP Active message (See Section 3.7.1)
	 *
	 * </pre>
	 */
	class Ntfy : public M3UAMessage {
	public:
		Ntfy();
		bool check_mandatory();

	};

	/**
	 * \brief Err: Error (ERR) M3UA message
	 * \details
	 * <pre>
	 *3.8.1  Error
	 *
	 *   The Error message is used to notify a peer of an error event
	 *   associated with an incoming message.  For example, the message type
	 *   might be unexpected given the current state, or a parameter value
	 *   might be invalid.
	 *
	 *   The Error message contains the following parameters:
	 *
	 *      Error Code                 Mandatory
	 *      Routing Context            Mandatory*
	 *      Network Appearance         Mandatory*
	 *      Affected Point Code        Mandatory*
	 *      Diagnostic Information     Optional
	 *
	 *      (*) Only mandatory for specific Error Codes
	 *   The format for the Error message is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |          Tag = 0x000c         |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                          Error Code                           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |          Tag = 0x0006         |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                        Routing Context                        /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |          Tag - 0x0012         |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |     Mask      |             Affected Point Code  1            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                                ...                            /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |     Mask      |             Affected Point Code  n            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |           Tag = 0x0200        |           Length = 8          |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                      Network Appearance                       |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |          Tag = 0x0007         |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                     Diagnostic Information                    /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Error Code: 32-bits (unsigned integer)
	 *
	 *      The Error Code parameter indicates the reason for the Error
	 *      Message. The Error parameter value can be one of the following
	 *      values:
	 *
	 *      0x01      Invalid Version
	 *      0x02      Not Used in M3UA
	 *      0x03      Unsupported Message Class
	 *      0x04      Unsupported Message Type
	 *      0x05      Unsupported Traffic Mode Type
	 *      0x06      Unexpected Message
	 *      0x07      Protocol Error
	 *      0x08      Not used in M3UA
	 *      0x09      Invalid Stream Identifier
	 *      0x0a      Not used in M3UA
	 *      0x0b      Not used in M3UA
	 *      0x0c      Not used in M3UA
	 *      0x0d      Refused - Management Blocking
	 *      0x0e      ASP Identifier Required
	 *      0x0f      Invalid ASP Identifier
	 *      0x10      Not Used in M3UA
	 *      0x11      Invalid Parameter Value
	 *      0x12      Parameter Field Error
	 *      0x13      Unexpected Parameter
	 *      0x14      Destination Status Unknown
	 *      0x15      Invalid Network Appearance
	 *      0x16      Missing Parameter
	 *      0x17      Not Used in M3UA
	 *      0x18      Not Used in M3UA
	 *      0x19      Invalid Routing Context
	 *      0x1a      No Configured AS for ASP
	 *
	 *   The "Invalid Stream Identifier" error is sent if a message is
	 *   received on an unexpected SCTP stream (e.g., a MGMT message was
	 *   received on a stream other than "0").  Error messages MUST NOT be
	 *   generated in response to other Error messages.
	 *
	 *   The "Unsupported Message Class" error is sent if a message with an
	 *   unexpected or unsupported Message Class is received.
	 *
	 *   The "Unsupported Message Type" error is sent if a message with an
	 *   unexpected or unsupported Message Type is received.
	 *
	 *   The "Unsupported Traffic Mode Type" error is sent by a SGP if an ASP
	 *   sends an ASP Active message with an unsupported Traffic Mode Type or
	 *   a Traffic Mode Type that is inconsistent with the presently
	 *   configured mode for the Application Server.  An example would be a
	 *   case in which the SGP did not support loadsharing.
	 *
	 *   The "Unexpected Message" error MAY be sent if a defined and
	 *   recognized message is received that is not expected in the current
	 *   state (in some cases the ASP may optionally silently discard the
	 *   message and not send an Error message).  For example, silent discard
	 *   is used by an ASP if it received a DATA message from an SGP while it
	 *   was in the ASP-INACTIVE state. If the Unexpected message contained
	 *   Routing Context(s), the Routing Context(s) SHOULD be included in the
	 *   Error message.
	 *
	 *   The "Protocol Error" error is sent for any protocol anomaly (i.e.,
	 *   reception of a parameter that is syntactically correct but unexpected
	 *   in the current situation.
	 *
	 *   The "Invalid Stream Identifier" error is sent if a message is
	 *   received on an unexpected SCTP stream (e.g., a Management message was
	 *   received on a stream other than "0").
	 *
	 *   The "Refused - Management Blocking" error is sent when an ASP Up or
	 *   ASP Active message is received and the request is refused for
	 *   management reasons (e.g., management lockout").  If this error is in
	 *   response to an ASP Active message, the Routing Context(s) in the ASP
	 *   Active message SHOULD be included in the Error message.
	 *
	 *   The "ASP Identifier Required" is sent by a SGP in response to an ASP
	 *   Up message which does not contain an ASP Identifier parameter when
	 *   the SGP requires one.  The ASP SHOULD resend the ASP Up message with
	 *   an ASP Identifier.
	 *
	 *   The "Invalid ASP Identifier" is sent by an SGP in response to an ASP
	 *   Up message with an invalid (i.e., non-unique) ASP Identifier.
	 *
	 *   The "Invalid Parameter Value " error is sent if a message is received
	 *   with an invalid parameter value (e.g., a DUPU message was received
	 *   with a Mask value other than "0".
	 *
	 *   The "Parameter Field Error" would be sent if a message is received
	 *   with a parameter having a wrong length field.
	 *
	 *   The "Unexpected Parameter" error would be sent if a message contains
	 *   an invalid parameter.
	 *
	 *   The "Destination Status Unknown" Error MAY be sent if a DAUD is
	 *   received at an SG enquiring of the availability/congestion status of
	 *   a destination, and the SG does not wish to provide the status (e.g.,
	 *   the sender is not authorized to know the status).  For this error,
	 *   the invalid or unauthorized Point Code(s) MUST be included along with
	 *   the Network Appearance and/or Routing Context associated with the
	 *   Point Code(s).
	 *
	 *   The "Invalid Network Appearance" error is sent by a SGP if an ASP
	 *   sends a message with an invalid (unconfigured) Network Appearance
	 *   value. For this error, the invalid (unconfigured) Network Appearance
	 *   MUST be included in the Network Appearance parameter.
	 *
	 *   The "Missing Parameter" error would be sent if a mandatory parameter
	 *   were not included in a message.
	 *
	 *   The "Invalid Routing Context" error is sent if a message is received
	 *   from a peer with an invalid (unconfigured) Routing Context value.
	 *   For this error, the invalid Routing Context(s) MUST be included in
	 *   the Error message.
	 *
	 *   The "No Configured AS for ASP" error is sent if a message is received
	 *   from a peer without a Routing Context parameter and it is not known
	 *   by configuration data which Application Servers are referenced.
	 *
	 *   Diagnostic Information: variable length
	 *
	 *      When included, the optional Diagnostic information can be any
	 *      information germane to the error condition, to assist in
	 *      identification of the error condition. The Diagnostic information
	 *      SHOULD contain the offending message.
	 *
	 * </pre>
	 */
	class Err : public M3UAMessage {
	public:
		Err();
		bool check_mandatory();
	};

	/**
	 * \brief Dupu: Destination User Part Unavailable (DUPU) M3UA message
	 * \details
	 * <pre>
	 *	3.4.5 Destination User Part Unavailable (DUPU)
	 *
	 *	   The DUPU message is used by an SGP to inform concerned ASPs that a
	 *	   remote peer MTP3-User Part (e.g., ISUP or SCCP) at an SS7 node is
	 *	   unavailable.
	 *
	 *	   The DUPU message contains the following parameters:
	 *
	 *		  Network Appearance       Optional
	 *		  Routing Context          Optional
	 *		  Affected Point Code      Mandatory
	 *		  User/Cause               Mandatory
	 *		  INFO String              Optional
	 *
	 *	   The format for DUPU message parameters is as follows:
	 *
	 *		   0                   1                   2                   3
	 *		   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x0200          |             Length = 8        |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |                      Network Appearance                       |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |        Tag = 0x0006           |             Length            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  \                                                               \
	 *		  /                       Routing Context                         /
	 *		  \                                                               \
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x0012          |          Length = 8           |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |   Mask = 0    |                  Affected PC                  |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x0204          |          Length = 8           |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |             Cause             |            User               |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  |         Tag = 0x0004          |             Length            |
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  \                                                               \
	 *		  /                          INFO String                          /
	 *		  \                                                               \
	 *		  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *		  User/Cause: 32-bits
	 *
	 *			 The Unavailability Cause and MTP3-User Identity fields, associated
	 *			 with the Affected PC in the Affected Point Code parameter, are
	 *			 encoded as follows:
	 *
	 *		  Unavailability Cause field: 16-bits (unsigned integer)
	 *
	 *			 The Unavailability Cause parameter provides the reason for the
	 *			 unavailability of the MTP3-User.  The valid values for the
	 *			 Unavailability Cause parameter are shown in the following table.
	 *			 The values agree with those provided in the SS7 MTP3 User Part
	 *			 Unavailable message.  Depending on the MTP3 protocol used in the
	 *			 Network Appearance, additional values may be used - the
	 *			 specification of the relevant MTP3 protocol variant/version
	 *			 recommendation is definitive.
	 *
	 *				0         Unknown
	 *				1         Unequipped Remote User
	 *				2         Inaccessible Remote User
	 *
	 *		  MTP3-User Identity field: 16-bits (unsigned integer)
	 *
	 *			 The MTP3-User Identity describes the specific MTP3-User that is
	 *			 unavailable (e.g., ISUP, SCCP, ...).  Some of the valid values for
	 *			 the MTP3-User Identity are shown below.  The values align with
	 *			 those provided in the SS7 MTP3 User Part Unavailable message and
	 *			 Service Indicator.  Depending on the MTP3 protocol variant/version
	 *			 used in the network appearance, additional values may be used.
	 *			 The relevant MTP3 protocol variant/version recommendation is
	 *			 definitive.
	 *
	 *				 0 to 2   Reserved
	 *					3     SCCP
	 *					4     TUP
	 *					5     ISUP
	 *				 6 to 8  Reserved
	 *					9     Broadband ISUP
	 *				   10     Satellite ISUP
	 *				   11     Reserved
	 *				   12     AAL type 2 Signalling
	 *				   13     Bearer Independent Call Control (BICC)
	 *				   14     Gateway Control Protocol
	 *				   15     Reserved
	 *
	 *			 The format and description of the Affected Point Code parameter is
	 *			 the same as for the DUNA message (See Section 3.4.1.) except that
	 *			 the Mask field is not used and only a single Affected DPC is
	 *			 included.  Ranges and lists of Affected DPCs cannot be signaled in
	 *			 a DUPU message, but this is consistent with UPU operation in the
	 *			 SS7 network.  The Affected Destinations parameter in an MTP3 User
	 *			 Part Unavailable message (UPU) received by an SGP from the SS7
	 *			 network contains only one destination.
	 *
	 *			 The format and description of the Network Appearance, Routing
	 *			 Context, and INFO String parameters is the same as for the DUNA
	 *			 message (See Section 3.4.1).
	 *</pre>
	 */
	class Dupu : public M3UAMessage {
	public:
		Dupu();
		bool check_mandatory();
	};

	/**
	 * \brief Duna: Destination Unavailable (DUNA) M3UA message
	 * \details
	 * <pre>
	 *3.4.1 Destination Unavailable (DUNA)
	 *
	 *   The DUNA message is sent from an SGP in an SG to all concerned ASPs
	 *   to indicate that the SG has determined that one or more SS7
	 *   destinations are unreachable.  It is also sent by an SGP in response
	 *   to a message from the ASP to an unreachable SS7 destination.  As an
	 *   implementation option the SG may suppress the sending of subsequent
	 *   "response" DUNA messages regarding a certain unreachable SS7
	 *   destination for a certain period to give the remote side time to
	 *   react.  If there is no alternate route via another SG, the MTP3-User
	 *   at the ASP is expected to stop traffic to the affected destination
	 *   via the SG as per the defined MTP3-User procedures.
	 *
	 *   The DUNA message contains the following parameters:
	 *
	 *      Network Appearance      Optional
	 *      Routing Context         Optional
	 *      Affected Point Code     Mandatory
	 *      INFO String             Optional
	 *
	 *   The format for DUNA Message parameters is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0200          |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                      Network Appearance                       |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0006           |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                       Routing Context                         /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0012          |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |     Mask      |                 Affected PC 1                 |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                              ...                              /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |     Mask      |                 Affected PC n                 |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |          Tag = 0x0004         |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                          INFO String                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Network Appearance: 32-bit unsigned integer
	 *
	 *      See Section 3.3.1
	 *
	 *   Routing Context: n x 32-bits (unsigned integer)
	 *
	 *      The optional Routing Context parameter contains the Routing
	 *      Context values associated with the DUNA message.  Where a Routing
	 *      Key has not been coordinated between the SGP and ASP, sending of
	 *      Routing Context is not required.  Where multiple Routing Keys and
	 *      Routing Contexts are used across a common association, the Routing
	 *      Context(s) MUST be sent to identify the concerned traffic flows
	 *      for which the DUNA message applies, assisting in outgoing traffic
	 *      management and internal distribution of MTP-PAUSE indications to
	 *      MTP3-Users at the receiver.
	 *
	 *   Affected Point Code: n x 32-bits
	 *
	 *      The Affected Point Code parameter contains a list of Affected
	 *      Destination Point Code fields, each a three-octet parameter to
	 *      allow for 14-, 16- and 24-bit binary formatted SS7 Point Codes.
	 *      Affected Point Codes that are less than 24-bits, are padded on the
	 *      left to the 24-bit boundary.  The encoding is shown below for ANSI
	 *      and ITU Point Code examples.
	 *
	 *   ANSI 24-bit Point Code:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |     Mask      |    Network    |    Cluster    |     Member    |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *                      |MSB-----------------------------------------LSB|
	 *
	 *   ITU 14-bit Point Code:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |     Mask      |0 0 0 0 0 0 0 0 0 0|Zone |     Region    | SP  |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *                                           |MSB--------------------LSB|
	 *
	 *      It is optional to send an Affected Point Code parameter with more
	 *      than one Affected PC but it is mandatory to receive it.  Including
	 *      multiple Affected PCs may be useful when reception of an MTP3
	 *      management message or a linkset event simultaneously affects the
	 *      availability status of a list of destinations at an SG.
	 *
	 *   Mask: 8-bits (unsigned integer)
	 *
	 *      The Mask field can be used to identify a contiguous range of
	 *      Affected Destination Point Codes.  Identifying a contiguous range
	 *      of Affected DPCs may be useful when reception of an MTP3
	 *      management message or a linkset event simultaneously affects the
	 *      availability status of a series of destinations at an SG.
	 *
	 *      The Mask parameter is an integer representing a bit mask that can
	 *      be applied to the related Affected PC field.  The bit mask
	 *      identifies how many bits of the Affected PC field are significant
	 *      and which are effectively "wildcarded".  For example, a mask of
	 *      "8" indicates that the last eight bits of the PC is "wildcarded".
	 *      For an ANSI 24-bit Affected PC, this is equivalent to signalling
	 *      that all PCs in an ANSI Cluster are unavailable.  A mask of "3"
	 *      indicates that the last three bits of the PC is "wildcarded".  For
	 *      a 14-bit ITU Affected PC, this is equivalent to signaling that an
	 *      ITU
	 *
	 *      Region is unavailable.  A mask value equal (or greater than) the
	 *      number of bits in the PC indicates that the entire network
	 *      appearance is affected - this is used to indicate network
	 *      isolation to the ASP.
	 *
	 *   INFO String: variable length
	 *
	 *      The optional INFO String parameter can carry any meaningful UTF-8
	 *      [10] character string along with the message.  Length of the INFO
	 *      String parameter is from 0 to 255 octets.  No procedures are
	 *      presently identified for its use but the INFO String MAY be used
	 *      for debugging purposes.
	 *
	 * </pre>
	 */
	class Duna : public M3UAMessage {
	public:
		Duna();
		bool check_mandatory();
	};

	/**
	 * \brief Drst: Destination Restricted (DRST) M3UA message
	 * \details
	 * <pre>
	 *3.4.6 Destination Restricted (DRST)
	 *
	 *   The DRST message is optionally sent from the SGP to all concerned
	 *   ASPs to indicate that the SG has determined that one or more SS7
	 *   destinations are now restricted from the point of view of the SG, or
	 *   in response to a DAUD message if appropriate.  The M3UA layer at the
	 *   ASP is expected to send traffic to the affected destination via an
	 *   alternate SG with route(s) of equal priority, but only if such an
	 *   alternate route exists and is available.  If the affected destination
	 *   is currently considered unavailable by the ASP, The MTP3-User should
	 *   be informed that traffic to the affected destination can be resumed.
	 *   In this case, the M3UA layer should route the traffic through the SG
	 *   initiating the DRST message.
	 *
	 *   This message is optional for the SG to send and it is optional for
	 *   the ASP to act on any information received in the message.  It is for
	 *   use in the "STP" case described in Section 1.4.1.
	 *
	 *   The DRST message contains the following parameters:
	 *
	 *      Network Appearance       Optional
	 *      Routing Context          Optional
	 *      Affected Point Code      Mandatory
	 *      INFO String              Optional
	 *
	 *   The format and description of the Network Appearance, Routing
	 *   Context, Affected Point Code and INFO String parameters is the same
	 *   as for the DUNA message (See Section 3.4.1).
	 *
	 * </pre>
	 */
	class Drst : public M3UAMessage {
	public:
		Drst();
		bool check_mandatory();
	};

	/**
	 * \brief Dereg_rsp: Deregistration Response (DEREG RSP) M3UA message
	 * \details
	 * <pre>
	 *3.6.4 Deregistration Response (DEREG RSP)
	 *
	 *   The DEREG RSP message is used as a response to the DEREG REQ message
	 *   from a remote M3UA peer.
	 *
	 *   The DEREG RSP message contains the following parameters:
	 *
	 *      Deregistration Result    Mandatory
	 *
	 *   One or more Deregistration Result parameters MUST be included.  The
	 *   format for the DEREG RSP message is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |          Tag = 0x0209         |               Length          |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                   Deregistration Result 1                     |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                              ...                              /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |           Tag = 0x0209        |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                   Deregistration Result n                     |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Deregistration Results:
	 *
	 *      The Deregistration Result parameter contains the deregistration
	 *      status for a single Routing Context in a DEREG REQ message.  The
	 *      number of results in a single DEREG RSP message MAY be anywhere
	 *      from one to the total number of number of Routing Context values
	 *      found in the corresponding DEREG REQ message.
	 *
	 *      Where multiple DEREG RSP messages are used in reply to DEREG REQ
	 *      message, a specific result SHOULD be in only one DEREG RSP
	 *      message.  The format of each result is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0006          |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                        Routing Context                        |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0213          |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                     Deregistration Status                     |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Routing Context: 32-bit integer
	 *
	 *      The Routing Context field contains the Routing Context value of
	 *      the matching Routing Key to deregister, as found in the DEREG REQ
	 *      message.
	 *
	 *      Deregistration Status: 32-bit integer
	 *
	 *      The Deregistration Result Status field indicates the success or
	 *      the reason for failure of the deregistration.
	 *
	 *      Its values may be:
	 *
	 *         0           Successfully Deregistered
	 *         1           Error - Unknown
	 *         2           Error - Invalid Routing Context
	 *         3           Error - Permission Denied
	 *         4           Error - Not Registered
	 *         5           Error - ASP Currently Active for Routing Context
	 *
	 * </pre>
	 */
	class Dereg_rsp : public M3UAMessage {
	public:
		Dereg_rsp();
		bool check_mandatory();
	};

	/**
	 * \brief Dereg_req: Deregistration Request (DEREG REQ) M3UA message
	 * \details
	 * <pre>
	 *3.6.3 Deregistration Request (DEREG REQ)
	 *
	 *   The DEREG REQ message is sent by an ASP to indicate to a remote M3UA
	 *   peer that it wishes to deregister a given Routing Key.  Typically, an
	 *   ASP would send this message to an SGP, and expects to receive a DEREG
	 *   RSP message in return with the associated Routing Context value.
	 *
	 *   The DEREG REQ message contains the following parameters:
	 *
	 *      Routing Context       Mandatory
	 *
	 *   The format for the DEREG REQ message is as follows:
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0006          |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                       Routing Context                         /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Routing Context: n X 32-bit integers
	 *
	 *      The Routing Context parameter contains (a list of) integers
	 *      indexing the Application Server traffic that the sending ASP is
	 *      currently registered to receive from the SGP but now wishes to
	 *      deregister.
	 *
	 * </pre>
	 */
	class Dereg_req : public M3UAMessage {
	public:
		Dereg_req();
		bool check_mandatory();
	};

	/**
	 * \brief Dava: Destination Available (DAVA) M3UA message
	 * \details
	 * <pre>
	 *   The DAVA message is sent from an SGP to all concerned ASPs to
	 *   indicate that the SG has determined that one or more SS7 destinations
	 *   are now reachable (and not restricted), or in response to a DAUD
	 *   message if appropriate. If the ASP M3UA layer previously had no
	 *   routes to the affected destinations the ASP MTP3-User protocol is
	 *   informed and may now resume traffic to the affected destination.  The
	 *   ASP M3UA layer now routes the MTP3-user traffic through the SG
	 *   initiating the DAVA message.
	 *
	 *   The DAVA message contains the following parameters:
	 *
	 *      Network Appearance       Optional
	 *      Routing Context          Optional
	 *      Affected Point Code      Mandatory
	 *      INFO String              Optional
	 *
	 *   The format and description of the Network Appearance, Routing
	 *   Context, Affected Point Code and INFO String parameters is the same
	 *   as for the DUNA message (See Section 3.4.1).
	 *
	 * </pre>
	 */
	class Dava : public M3UAMessage {
	public:
		Dava();
		bool check_mandatory();
	};

	/**
	 * \brief Daud: Destination State Audit (DAUD) M3UA message
	 * \details
	 * <pre>
	 *3.4.3 Destination State Audit (DAUD)
	 *
	 *   The DAUD message MAY be sent from the ASP to the SGP to audit the
	 *   availability/congestion state of SS7 routes from the SG to one or
	 *   more affected destinations.
	 *
	 *   The DAUD message contains the following parameters:
	 *
	 *      Network Appearance      Optional
	 *      Routing Context         Optional
	 *      Affected Point Code     Mandatory
	 *      INFO String             Optional
	 *
	 *   The format and description of DAUD Message parameters is the same as
	 *   for the DUNA message (See Section 3.4.1).
	 *
	 * </pre>
	 */
	class Daud : public M3UAMessage {
	public:
		Daud();
		bool check_mandatory();

	};

	/**
	 * \brief Beat_ack: Heartbeat Acknowledgement (BEAT Ack) M3UA message
	 * \details
	 * <pre>
	 *3.5.6 Heartbeat Acknowledgement (BEAT Ack)
	 *
	 *   The BEAT Ack message is sent in response to a received BEAT message.
	 *   It includes all the parameters of the received BEAT message, without
	 *   any change.
	 *
	 * </pre>
	 */
	class Beat_ack : public M3UAMessage {
	public:
		Beat_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Beat: Heartbeat (BEAT) M3UA message
	 * \details
	 * <pre>
	 *3.5.5 Heartbeat (BEAT)
	 *
	 *   The BEAT message is optionally used to ensure that the M3UA peers are
	 *   still available to each other.  It is recommended for use when the
	 *   M3UA runs over a transport layer other than the SCTP, which has its
	 *   own heartbeat.
	 *
	 *   The BEAT message contains the following parameters:
	 *
	 *   Heartbeat Data         Optional
	 *
	 *   The format for the BEAT message is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0009          |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                       Heartbeat Data                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The Heartbeat Data parameter contents are defined by the sending
	 *   node. The Heartbeat Data could include, for example, a Heartbeat
	 *   Sequence Number and/or Timestamp.  The receiver of a BEAT message
	 *   does not process this field as it is only of significance to the
	 *   sender.  The receiver MUST respond with a BEAT Ack message.
	 *
	 * </pre>
	 */
	class Beat : public M3UAMessage {
	public:
		Beat();
		bool check_mandatory();
	};

	/**
	 * \brief Aspup_ack: ASP Up Acknowledgement (ASP Up Ack) M3UA message
	 * \details
	 * <pre>
	 *3.5.2 ASP Up Acknowledgement (ASP Up Ack)
	 *
	 *   The ASP UP Ack message is used to acknowledge an ASP Up message
	 *   received from a  remote M3UA peer.
	 *
	 *   The ASP Up Ack message contains the following parameters:
	 *
	 *   INFO String (optional)
	 *
	 *   The format for ASP Up Ack message parameters is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag =0x0004             |             Length          |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                          INFO String                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional INFO String parameter is
	 *   the same as for the DUNA message (See Section 3.4.1).  The INFO
	 *   String in an ASP Up Ack message is independent from the INFO String
	 *   in the ASP Up message (i.e., it does not have to echo back the INFO
	 *   String received).
	 *
	 * </pre>
	 */
	class Aspup_ack : public M3UAMessage {
	public:
		Aspup_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Aspup: ASP Up M3UA message
	 * \details
	 * <pre>
	 *3.5.1 ASP Up
	 *
	 *   The ASP Up message is used to indicate to a remote M3UA peer that the
	 *   adaptation layer is ready to receive any ASPSM/ASPTM messages for all
	 *   Routing Keys that the ASP is configured to serve.
	 *
	 *   The ASP Up message contains the following parameters:
	 *
	 *      ASP Identifier                Optional
	 *      INFO String                   Optional
	 *
	 *   The format for ASP Up message parameters is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0011          |           Length = 8          |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                         ASP Identifier                        |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0004          |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                          INFO String                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   ASP Identifier: 32-bit unsigned integer
	 *
	 *      The optional ASP Identifier parameter contains a unique value that
	 *      is locally significant among the ASPs that support an AS.  The SGP
	 *      should save the ASP Identifier to be used, if necessary, with the
	 *      Notify message (see Section 3.8.2).
	 *
	 *      The format and description of the optional INFO String parameter
	 *      is the same as for the DUNA message (See Section 3.4.1).
	 *
	 * </pre>
	 */
	class Aspup : public M3UAMessage {
	public:
		Aspup();
		bool check_mandatory();
	};

	/**
	 * \brief Aspia_ack: ASP Inactive Acknowledgement (ASP Inactive Ack) M3UA message
	 * \details
	 * <pre>
	 *3.7.4 ASP Inactive Acknowledgement (ASP Inactive Ack)
	 *
	 *   The ASP Inactive Ack message is used to acknowledge an ASP Inactive
	 *   message received from a remote M3UA peer.
	 *
	 *   The ASP Inactive Ack message contains the following parameters:
	 *
	 *      Routing Context       Optional
	 *      INFO String           Optional
	 *
	 *   The format for the ASP Inactive Ack message is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0006          |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                       Routing Context                         /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0004          |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                          INFO String                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional INFO String parameter is
	 *   the same as for the DUNA message (See Section 3.4.1.)
	 *
	 *   The INFO String in an ASP Inactive Ack message is independent from
	 *   the INFO String in the ASP Inactive message (i.e., it does not have
	 *   to echo back the INFO String received).
	 *
	 *   The format of the Routing Context parameter is the same as for the
	 *   ASP Inactive message. (See Section 3.7.3).
	 *
	 * </pre>
	 */
	class Aspia_ack : public M3UAMessage {
	public:
		Aspia_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Aspia: ASP Inactive M3UA message
	 * \details
	 * <pre>
	 *3.7.3  ASP Inactive
	 *
	 *   The ASP Inactive message is sent by an ASP to indicate to a remote
	 *   M3UA peer that it is no longer an active ASP to be used from within a
	 *   list of ASPs.  The ASP Inactive message affects only the ASP state in
	 *   the Routing Keys identified by the Routing Contexts, if present.
	 *
	 *   The ASP Inactive message contains the following parameters:
	 *
	 *      Routing Context         Optional
	 *      INFO String             Optional
	 *
	 *   The format for the ASP Inactive message parameters is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0006          |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                       Routing Context                         /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0004          |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                          INFO String                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional Routing Context and INFO
	 *   String parameters is the same as for the ASP Active message (See
	 *   Section 3.5.5.)
	 *
	 * </pre>
	 */
	class Aspia : public M3UAMessage {
	public:
		Aspia();
		bool check_mandatory();

	};

	/**
	 * \brief Aspdn_ack: ASP Down Acknowledgement (ASP Down Ack) M3UA message
	 * \details
	 * <pre>
	 *3.5.4 ASP Down Acknowledgement (ASP Down Ack)
	 *
	 *   The ASP Down Ack message is used to acknowledge an ASP Down message
	 *   received from a remote M3UA peer.
	 *
	 *   The ASP Down Ack message contains the following parameters:
	 *
	 *   INFO String     Optional
	 *
	 *   The format for the ASP Down Ack message parameters is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0004          |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                         INFO String                           /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional INFO String parameter is
	 *   the same as for the DUNA message (See Section 3.4.1).
	 *
	 *   The INFO String in an ASP Down Ack message is independent from the
	 *   INFO String in the ASP Down message (i.e., it does not have to echo
	 *   back the INFO String received).
	 *
	 * </pre>
	 */
	class Aspdn_ack : public M3UAMessage {
	public:
		Aspdn_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Aspdn: ASP Down M3UA message
	 * \details
	 * <pre>
	 *3.5.3 ASP Down
	 *
	 *   The ASP Down message is used to indicate to a remote M3UA peer that
	 *   the adaptation layer is NOT ready to receive DATA, SSNM, RKM or ASPTM
	 *   messages.
	 *
	 *   The ASP Down message contains the following parameters:
	 *
	 *   INFO String    Optional
	 *
	 *  The format for the ASP Down message parameters is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag =0x0004           |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                         INFO String                           /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional INFO String parameter is
	 *   the same as for the DUNA message (See Section 3.4.1).
	 *
	 * </pre>
	 */
	class Aspdn : public M3UAMessage {
	public:
		Aspdn();
		bool check_mandatory();

	};

	/**
	 * \brief Aspac_ack: ASP Active Acknowledgement (ASP Active Ack) M3UA message
	 * \details
	 * <pre>
	 *3.7.2 ASP Active Acknowledgement (ASP Active Ack)
	 *
	 *   The ASP Active Ack message is used to acknowledge an ASP Active
	 *   message received from a remote M3UA peer.
	 *
	 *   The ASP Active Ack message contains the following parameters:
	 *
	 *      Traffic Mode Type     Optional
	 *      Routing Context       Optional
	 *      INFO String           Optional
	 *
	 *   The format for the ASP Active Ack message is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |           Tag = 0x000b        |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                      Traffic Mode Type                        |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |            Tag = 0x0006       |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                       Routing Context                         /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |           Tag = 0x0004        |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                          INFO String                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   The format and description of the optional INFO String parameter is
	 *   the same as for the DUNA message (See Section 3.4.1).
	 *
	 *   The INFO String in an ASP Active Ack message is independent from the
	 *   INFO String in the ASP Active message (i.e., it does not have to echo
	 *   back the INFO String received).
	 *
	 *   The format of the Traffic Mode Type and Routing Context parameters is
	 *   the same as for the ASP Active message. (See Section 3.7.1).
	 *
	 * </pre>
	 */
	class Aspac_ack : public M3UAMessage {
	public:
		Aspac_ack();
		bool check_mandatory();
	};

	/**
	 * \brief Aspac: ASP Active M3UA message
	 * \details
	 * <pre>
	 *3.7.1 ASP Active
	 *
	 *   The ASP Active message is sent by an ASP to indicate to a remote M3UA
	 *   peer that it is ready to process signalling traffic for a particular
	 *   Application Server.  The ASP Active message affects only the ASP
	 *   state for the Routing Keys identified by the Routing Contexts, if
	 *   present.
	 *
	 *   The ASP Active message contains the following parameters:
	 *
	 *      Traffic Mode Type     Optional
	 *      Routing Context       Optional
	 *      INFO String           Optional
	 *
	 *   The format for the ASP Active message is as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x000b          |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                      Traffic Mode Type                        |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0006          |            Length             |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                       Routing Context                         /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |         Tag = 0x0004          |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                          INFO String                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Traffic Mode Type: 32-bit (unsigned integer)
	 *
	 *      The Traffic Mode Type parameter identifies the traffic mode of
	 *      operation of the ASP within an AS.  The valid values for Traffic
	 *      Mode Type are shown in the following table:
	 *
	 *         1         Override
	 *         2         Loadshare
	 *         3         Broadcast
	 *
	 *     Within a particular Routing Context, Override, Loadshare and
	 *      Broadcast SHOULD NOT be mixed.  The Override value indicates that
	 *      the ASP is operating in Override mode, and the ASP takes over all
	 *      traffic in an Application Server (i.e., primary/backup operation),
	 *      overriding any currently active ASPs in the AS.  In Loadshare
	 *      mode, the ASP will share in the traffic distribution with any
	 *      other currently active ASPs.  In Broadcast mode, the ASP will
	 *      receive the same messages as any other currently active ASP.
	 *
	 *   Routing Context: n X 32-bit integers
	 *
	 *      The optional Routing Context parameter contains (a list of)
	 *      integers indexing the Application Server traffic that the sending
	 *      ASP is configured/registered to receive.
	 *
	 *      There is one-to-one relationship between an index entry and an SGP
	 *      Routing Key or AS Name.  Because an AS can only appear in one
	 *      Network Appearance, the Network Appearance parameter is not
	 *      required in the ASP Active message.
	 *
	 *      An Application Server Process may be configured to process traffic
	 *      for more than one logical Application Server.  From the
	 *      perspective of an ASP, a Routing Context defines a range of
	 *      signalling traffic that the ASP is currently configured to receive
	 *      from the SGP.  For example, an ASP could be configured to support
	 *      call processing for multiple ranges of PSTN trunks and therefore
	 *      receive related signalling traffic, identified by separate SS7
	 *      DPC/OPC/CIC ranges.
	 *
	 *   The format and description of the optional INFO String parameter is
	 *   the same as for the DUNA message (See Section 3.4.1).
	 *
	 * </pre>
	 */
	class Aspac : public M3UAMessage {
	public:
		Aspac();
		bool check_mandatory();

	};

	/**
	 * \brief Data: Payload Data Message (DATA) M3UA message
	 * \details
	 * <pre>
	 *3.3.1 Payload Data Message (DATA)
	 *
	 *   The DATA message contains the SS7 MTP3-User protocol data, which is
	 *   an MTP-TRANSFER primitive, including the complete MTP3 Routing Label.
	 *   The DATA message contains the following variable length parameters:
	 *
	 *      Network Appearance       Optional
	 *      Routing Context          Optional
	 *      Protocol Data            Mandatory
	 *      Correlation Id           Optional
	 *
	 *   The following format MUST be used for the Data Message:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0200           |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                       Network Appearance                      |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0006           |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                        Routing Context                        |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0210           |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                        Protocol Data                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0013           |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                        Correlation Id                         |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Network Appearance: 32-bits (unsigned integer)
	 *
	 *      The Network Appearance parameter identifies the SS7 network
	 *      context for the message and implicitly identifies the SS7 Point
	 *      Code format used, the SS7 Network Indicator value, and the MTP3
	 *      and possibly the MTP3-User protocol type/variant/version used
	 *      within the specific SS7 network.  Where an SG operates in the
	 *      context of a single SS7 network, or individual SCTP associations
	 *      are dedicated to each SS7 network context, the Network Appearance
	 *      parameter is not required.  In other cases the parameter may be
	 *      configured to be present for the use of the receiver.
	 *
	 *      The Network Appearance parameter value is of local significance
	 *      only, coordinated between the SGP and ASP. Therefore, in the case
	 *      where an ASP is connected to more than one SGP, the same SS7
	 *      network context may be identified by different Network Appearance
	 *      values depending over which SGP a message is being
	 *      transmitted/received.
	 *
	 *      Where the optional Network Appearance parameter is present, it
	 *      must be the first parameter in the message as it defines the
	 *      format of the Protocol Data field.
	 *
	 *
	 *   The following format MUST be used for the Data Message:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0200           |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                       Network Appearance                      |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0006           |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                        Routing Context                        |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0210           |             Length            |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                        Protocol Data                          /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |        Tag = 0x0013           |          Length = 8           |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                        Correlation Id                         |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Network Appearance: 32-bits (unsigned integer)
	 *
	 *      The Network Appearance parameter identifies the SS7 network
	 *      context for the message and implicitly identifies the SS7 Point
	 *      Code format used, the SS7 Network Indicator value, and the MTP3
	 *      and possibly the MTP3-User protocol type/variant/version used
	 *      within the specific SS7 network.  Where an SG operates in the
	 *      context of a single SS7 network, or individual SCTP associations
	 *      are dedicated to each SS7 network context, the Network Appearance
	 *      parameter is not required.  In other cases the parameter may be
	 *      configured to be present for the use of the receiver.
	 *
	 *      The Network Appearance parameter value is of local significance
	 *      only, coordinated between the SGP and ASP. Therefore, in the case
	 *      where an ASP is connected to more than one SGP, the same SS7
	 *      network context may be identified by different Network Appearance
	 *      values depending over which SGP a message is being
	 *      transmitted/received.
	 *
	 *      Where the optional Network Appearance parameter is present, it
	 *      must be the first parameter in the message as it defines the
	 *      format of the Protocol Data field.
	 *
	 *   The Protocol Data parameter is encoded as follows:
	 *
	 *       0                   1                   2                   3
	 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                     Originating Point Code                    |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                     Destination Point Code                    |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |       SI      |       NI      |      MP       |      SLS      |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                     User Protocol Data                        /
	 *      \                                                               \
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 *   Originating Point Code: 32 bits (unsigned integer)
	 *   Destination Point Code: 32 bits (unsigned integer)
	 *
	 *   The Originating and Destination Point Code fields contains the OPC
	 *   and DPC from the routing label of the original SS7 message in Network
	 *   Byte Order, justified to the least significant bit.  Unused bits are
	 *   coded `0'.
	 *
	 *   Service Indicator: 8 bits (unsigned integer)
	 *
	 *   The Service Indicator field contains the SI field from the original
	 *   SS7 message justified to the least significant bit.  Unused bits are
	 *   coded `0'.
	 *
	 *   Network Indicator: 8-bits (unsigned integer)
	 *
	 *   The Network Indicator contains the NI field from the original SS7
	 *   message justified to the least significant bit.  Unused bits are
	 *   coded `0'.
	 *
	 *   Message Priority: 8 bits (unsigned integer)
	 *
	 *   The Message Priority field contains the MP bits (if any) from the
	 *   original SS7 message, both for ANSI-style and TTC-style [29] message
	 *   priority bits. The MP bits are aligned to the least significant bit.
	 *   Unused bits are coded `0'.
	 *
	 *   Signalling Link Selection: 8 bits (unsigned integer)
	 *
	 *   The Signalling Link Selection field contains the SLS bits from the
	 *   routing label of the original SS7 message justified to the least
	 *   significant bit and in Network Byte Order.  Unused bits are coded
	 *   `0'.
	 *
	 *   User Protocol Data: (byte string)
	 *
	 *   The User Protocol Data field contains a byte string of MTP-User
	 *   information from the original SS7 message starting with the first
	 *   byte of the original SS7 message following the Routing Label.
	 *
	 *   Correlation Id: 32-bits (unsigned integer)
	 *
	 *   The Correlation Id parameter uniquely identifies the MSU carried in
	 *   the Protocol Data within an AS.  This Correlation Id parameter is
	 *   assigned by the sending M3UA.
	 *
	 * </pre>
	 *
	 */
	class Data : public M3UAMessage {
	public:
		Data();
		bool check_mandatory();
	};


	// M3UA pool classes
	class M3UAParamPoolItem {
	private:
		M3UAParam** pool;
		M3UAParam* next_free_item;
		int total_count;
		int free_count;
		M3UAParam* create_param(ParameterType _param_type);
	public:
		M3UAParamPoolItem();
		~M3UAParamPoolItem();
		ParameterType type;
		M3UAParam* request_item();
		void init_pool();
		void set_pool_size(int _total_count);

	};

	class M3UAMessagePoolItem {
	private:
		M3UAMessage** pool;
		M3UAMessage* next_free_item;
		int total_count;
		int free_count;
		M3UAMessage* create_message(MessageType _param_type);
	public:
		M3UAMessagePoolItem();
		~M3UAMessagePoolItem();
		MessageType type;
		M3UAMessage* request_item();
		void init_pool();
		void set_pool_size(int _total_count);

	};


	class M3UAPool {
	private:
		std::map<MessageType, M3UAMessagePoolItem*> MESSAGE_POOL;
		std::map<ParameterType, M3UAParamPoolItem*> PARAM_POOL;
		int param_count;
		int message_count;
		void init_param(ParameterType _param_type);
		void init_message(MessageType _msg_type);
	public:
		M3UAPool();
		~M3UAPool();
		void set_pool_size(int _param_count, int _message_count);
		void init_pool();
		M3UAMessage* request_message(MessageType msg_type);
		M3UAParam* request_param(ParameterType param_type);


	};


	/**
	 * \brief M3UAPacket: main class for every decoded M3UA packet
	 * \details This class represents M3UA decoded packet containing common parameters and abstract M3UA message parameter\m
	 * <pre>
	 * <pre>
	 * 3.1 Common Message Header
	 *
	 *   The protocol messages for MTP3-User Adaptation require a message
	 *   header which contains the adaptation layer version, the message type,
	 *   and message length.
	 *
	 *      0                   1                   2                   3
	 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |    Version    |   Reserved    | Message Class | Message Type  |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      |                        Message Length                         |
	 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *      \                                                               \
	 *      /                                                               /
	 *
	 *   All fields in an M3UA message MUST be transmitted in the network byte
	 *   order, unless otherwise stated.
	 *
	 *3.1.1 M3UA Protocol Version: 8 bits (unsigned integer)
	 *
	 *   The version field contains the version of the M3UA adaptation layer.
	 *
	 *   The supported versions are the following:
	 *
	 *         1      Release 1.0
	 *
	 *
	 *3.1.3  Reserved: 8 bits
	 *
	 *   The Reserved field SHOULD be set to all '0's and ignored by the
	 *   receiver.
	 *
	 *3.1.4  Message Length: 32-bits (unsigned integer)
	 *
	 *   The Message Length defines the length of the message in octets,
	 *   including the Common Header.  The Message Length MUST include
	 *   parameter padding bytes, if any.
	 *
	 *   Note: A receiver SHOULD accept the message whether or not the final
	 *   parameter padding is included in the message length.
	 *
	 * </pre>
	 */
	class M3UAPacket {
	public:
		M3UAPacket();
		~M3UAPacket();

		uint8_t version;
		MessageClass message_class;
		MessageType message_type;
		uint32_t message_length;
		M3UAMessage* message;
		M3UAPool _pool;

		void encode(unsigned char* buffer, int buffer_length, int offset, int* result_length);

	};


	/**
	 * Creates m3ua parameter object based on parameter type
	 * @param _param_type m3ua parameter type
	 * @return Pointer to m3ua parameter object or NULL if parameter not supported
	 */
	M3UAParam* get_param(ParameterType _param_type);

	/**
	 * Decodes m3ua message
	 * @param data pointer to m3ua bytes
	 * @param data_length number of bytes contained in m3ua packet
	 * @return Pointer to m3ua message object or NULL no data available
	 */
	M3UAPacket* decode(unsigned char* data, int data_length);

	int decode(unsigned char* data, int data_length, M3UAPacket* _m3uap, M3UAPool* _m3ua_pool);


}


#endif /* M3UA_H_ */
