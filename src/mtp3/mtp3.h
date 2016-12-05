/**
 * \file mtp3.h
 * \brief MTP3 library
 * \details MTP3 encoder/decoder library based on ITU-T Q.704, Q.707
 * \version 2.0
 * \date 2012
 * \author Damir Franusic <df@release14.org>
 */


#ifndef MTP3_H_
#define MTP3_H_

namespace mtp3{
	/**
	 * <pre>
	 *14.2.1 Service indicator
	 *The service indicator is used by signalling handling functions to perform message distribution
	 *(see 2.4) and, in some special applications, to perform message routing (see 2.3).
	 *
	 * </pre>
	 */
	enum ServiceIndicatorType {
		/** <b>0x00</b> - Signalling network management messages */
		SNMM = 0x00,
		/** <b>0x01</b> - Signalling network testing and maintenance messages */
		SNTMM = 0x01,
		/** <b>0x03</b> - SCCP */
		SCCP = 0x03,
		/** <b>0x04</b> - Telephone User Part */
		TUP = 0x04,
		/** <b>0x05</b> - ISDN User Part */
		ISUP = 0x05,
		/** <b>0x06</b> - Data User Part (call and circuit-related messages) */
		DUP_1 = 0x06,
		/** <b>0x07</b> - Data User Part (facility registration and cancellation messages) */
		DUP_2 = 0x07,
		/** <b>0x08</b> - Reserved for MTP Testing User Part */
		MTP_TESTING_USER_PART = 0x08,
		/** <b>0x09</b> - Broadband ISDN User Part */
		BROADBAND_ISUP = 0x09,
		/** <b>0x0a</b> - Satellite ISDN User Part */
		SAT_ISUP = 0x0a
	};

	/**
	 * <pre>
	 *The sub-service field contains the network indicator (bits C and D) and two spare bits (bits A and B).
	 *The network indicator is used by signalling message handling functions (e.g. in order to determine
	 *the relevant version of a User Part), see 2.3 and 2.4.
	 *If the network indicator is set to 00 or 01, the two spare bits, coded 00, are available for possible
	 *future needs that may require a common solution for all international User Parts.
	 *If the network indicator is set to 10 or 11, the two spare bits are for national use. They may be used,
	 *for example, to indicate message priority, which is used in the optional flow control procedure in
	 *national applications.
	 *The network indicator provides for discrimination between international and national messages. It
	 *can also be used, for example, for the discrimination between functionally two national signalling
	 *networks, each having different routing label structures and including up to 16 User Parts defined by
	 *the 16 possible codes of the service indicator.
	 *In the case of only one national signalling network the spare code of the network indicator reserved
	 *for national use can be used, for example, to define an additional 16 User Parts (making a total of
	 *32 User Parts) for that national signalling network.
	 *
	 * </pre>
	 */
	enum NetworkIndicatorType {
		/** <b>0x0a0</b> - International network */
		INTERNATIONAL = 0x00,
		/** <b>0x80</b> - National network */
		NATIONAL = 0x80,
		/** <b>0xc0</b> - Reserved for national use */
		RESERVED_NATIONAL = 0xc0
	};

	/**
	 * <pre>
	 *15.3 Heading code (H0)
	 *The heading code (H0) is the 4-bit field following the label and identifies the message group.
	 *The different heading codes are allocated as follows:
	 *
	 *0000 Spare
	 *0001 Changeover and changeback messages
	 *0010 Emergency changeover message
	 *0011 Transfer-controlled and signalling route set congestion messages
	 *0100 Transfer-prohibited-allowed-restricted messages
	 *0101 Signalling-route-set-test messages
	 *0110 Management inhibit messages
	 *0111 Traffic restart allowed message
	 *1000 Signalling-data-link-connection messages
	 *1001 Spare
	 *1010 User part flow control messages
	 *
	 * </pre>
	 */
	enum MessageGroupType {
		__MTP3_UNKNOWN_MESSAGE_GROUP__ = -1,
		// Signalling network management messages

		/** <b>0x01</b> - Changeover and changeback message */
		CHM = (SNMM << 8) + 0x01,
		/** <b>0x02</b> - Emergency changeover messages */
		ECM = (SNMM << 8) + 0x02,
		/** <b>0x03</b> - Transfer-controlled and signalling route set congestion messages */
		FCM = (SNMM << 8) + 0x03,
		/** <b>0x04</b> - Transfer-prohibited-allowed-restricted messages */
		TFM = (SNMM << 8) + 0x04,
		/** <b>0x05</b> - Signalling-route-set-test messages */
		RSM = (SNMM << 8) + 0x05,
		/** <b>0x06</b> - Management inhibit messages */
		MIM = (SNMM << 8) + 0x06,
		/** <b>0x07</b> - Traffic restart allowed message */
		TRM = (SNMM << 8) + 0x07,
		/** <b>0x08</b> - Signalling-data-link-connection messages */
		DLM = (SNMM << 8) + 0x08,
		/** <b>0x0a</b> - User part flow control messages */
		UFC = (SNMM << 8) + 0x0a,

		// Signalling network testing and maintenance messages

		/** <b>0x01</b> - Test messages */
		TESTM =  (SNTMM << 8) + 0x01

	};

	/**
	 * <pre>
	 * Signalling network management messages:
	 *
	 *CBA Changeback-acknowledgement signal
	 *CBD Changeback-declaration signal
	 *CNP Connection-not-possible signal
	 *CNS Connection-not-successful signal
	 *COA Changeover-acknowledgement signal
	 *COO Changeover-order signal
	 *CSS Connection-successful signal
	 *DLC Signalling-data-link-connection-order signal
	 *ECA Emergency-changeover-acknowledgement signal
	 *ECO Emergency-changeover-order signal
	 *LFU Link forced uninhibit signal
	 *LIA Link inhibit acknowledgement signal
	 *LID Link inhibit denied signal
	 *LIN Link inhibit signal
	 *LLT Link local inhibit test signal
	 *LUA Link uninhibit acknowledgement signal
	 *LUN Link uninhibit signal
	 *LRT Link remote inhibit test signal
	 *RCT Signalling-route-set-congestion-test signal
	 *RSM Signalling-route-set-test message
	 *RSR Signalling-route-set-test signal for restricted destination (national option)
	 *RST Signalling-route-set-test signal for prohibited destination
	 *TFA Transfer-allowed signal
	 *TFC Transfer-controlled signal
	 *TFM Transfer-prohibited-transfer-allowed-transfer-restricted messages
	 *TFP Transfer-prohibited signal
	 *TFR Transfer-restricted signal (national option)
	 *TRA Traffic-restart-allowed signal
	 *UPU User part unavailable signal
	 *
	 *Signalling network testing and maintenance messages:
	 *SLTM signalling link test message
	 *SLTA signalling link test acknowledgement message
	 * </pre>
	 */
	enum MessageType {
		__MTP3_UNKNOWN_MESSAGE_TYPE__ = -1,

		// Signalling network management messages

		/** <b>0b0110</b> - Changeback-acknowledgement signal */
		CBA  = (CHM) + (0b0110 << 4),
		/** <b>0b0101</b> - Changeback-declaration signal */
		CBD  = (CHM) + (0b0101 << 4),
		/** <b>0b0100</b> - Connection-not-possible signal */
		CNP  = (DLM) + (0b0100 << 4),
		/** <b>0b0011</b> - Connection-not-successful signal */
		CNS  = (DLM) + (0b0011 << 4),
		/** <b>0b0010</b> - Changeover-acknowledgement signal */
		COA  = (CHM) + (0b0010 << 4),
		/** <b>0b0001</b> - Changeover-order signal */
		COO  = (CHM) + (0b0001 << 4),
		/** <b>0b0010</b> - Connection-successful signal */
		CSS  = (DLM) + (0b0010 << 4),
		/** <b>0b0001</b> - Signalling-data-link-connection-order signal */
		DLC  = (DLM) + (0b0001 << 4),
		/** <b>0b0010</b> - Emergency-changeover-acknowledgement signal */
		ECA  = (ECM) + (0b0010 << 4),
		/** <b>0b0001</b> - Emergency-changeover-order signal */
		ECO  = (ECM) + (0b0001 << 4),
		/** <b>0b0110</b> - Link forced uninhibit signal */
		LFU  = (MIM) + (0b0110 << 4),
		/** <b>0b0011</b> - Link inhibit acknowledgement signal */
		LIA  = (MIM) + (0b0011 << 4),
		/** <b>0b0101</b> - Link inhibit denied signal */
		LID  = (MIM) + (0b0101 << 4),
		/** <b>0b0001</b> - Link inhibit signal */
		LIN  = (MIM) + (0b0001 << 4),
		/** <b>0b0111</b> - Link local inhibit test signal */
		LLT  = (MIM) + (0b0111 << 4),
		/** <b>0b0100</b> - Link uninhibit acknowledgement signal */
		LUA  = (MIM) + (0b0100 << 4),
		/** <b>0b0010</b> - Link uninhibit signal */
		LUN  = (MIM) + (0b0010 << 4),
		/** <b>0b1000</b> - Link remote inhibit test signal */
		LRT  = (MIM) + (0b1000 << 4),
		/** <b>0b0001</b> - Signalling-route-set-congestion-test signal */
		RCT  = (FCM) + (0b0001 << 4),
		/** <b>0b0010</b> - Signalling-route-set-test signal for restricted destination (national option) */
		RSR  = (RSM) + (0b0010 << 4),
		/** <b>0b0001</b> - Signalling-route-set-test signal for prohibited destination */
		RST  = (RSM) + (0b0001 << 4),
		/** <b>0b0101</b> - Transfer-allowed signal */
		TFA  = (TFM) + (0b0101 << 4),
		/** <b>0b0010</b> - Transfer-controlled signal */
		TFC  = (FCM) + (0b0010 << 4),
		/** <b>0b0001</b> - Transfer-prohibited signal */
		TFP  = (TFM) + (0b0001 << 4),
		/** <b>0b0011</b> - Transfer-restricted signal (national option) */
		TFR  = (TFM) + (0b0011 << 4),
		/** <b>0b0001</b> - Traffic-restart-allowed signal */
		TRA  = (TRM) + (0b0001 << 4),
		/** <b>0b0001</b> - User part unavailable signal */
		UPU  = (UFC) + (0b0001 << 4),

		// Signalling network testing and maintenance messages
		SLTM = (TESTM) + (0b0001 << 4),
		SLTA = (TESTM) + (0b0010 << 4)

	};

	enum UserPartIdType {
		/** <b>0x03</b> - SCCP */
		UPID_SCCP = 0x03,
		/** <b>0x04</b> - TUP */
		UPID_TUP = 0x04,
		/** <b>0x05</b> - ISUP */
		UPID_ISUP = 0x05,
		/** <b>0x06</b> - DUP */
		UPID_DUP = 0x06,
		/** <b>0x08</b> - MTP Testing User Part */
		UPID_MTP_TESTING_USER_PART = 0x08,
		/** <b>0x09</b> - Broadband ISDN User Part */
		UPID_BROADBAND_ISUP = 0x09,
		/** <b>0x0a</b> - Satellite ISDN User Part */
		UPID_SAT_ISUP = 0x0a
	};

	enum UnavailCauseType {
		/** <b>0x00</b> - unknown */
		UC_UNKNOWN = 0x00,
		/** <b>0x01</b> - unequipped remote user */
		UC_UNEQ_REMOTE_USER = 0x01,
		/** <b>0x02</b> - inaccessible remote user */
		UC_INAC_REMOTE_USER = 0x02
	};


	class MTP3Label {
	public:
		int dpc;
		int opc;
		int slc;
	};

	class MTP3Message {
	public:
		MTP3Message();
		virtual ~MTP3Message();
		MessageGroupType message_group;
		MessageType message_type;
		virtual void init(char* data, int data_length) = 0;
		virtual void encode(char* buffer, int buffer_length, int offset, int* result_length) = 0;
	};

	// Signalling network management messages
	class Changeover_order: public MTP3Message {
	public:
		Changeover_order();
		int fsn;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class Changeover_ack: public MTP3Message {
	public:
		Changeover_ack();
		int fsn;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class Changeback_declaration: public MTP3Message {
	public:
		Changeback_declaration();
		int changeback_code;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class Changeback_ack: public MTP3Message {
	public:
		Changeback_ack();
		int changeback_code;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class EmergencyChageover_order: public MTP3Message {
	public:
		EmergencyChageover_order();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class EmergencyChageover_ack: public MTP3Message {
	public:
		EmergencyChageover_ack();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class TransferProhibited: public MTP3Message {
	public:
		TransferProhibited();
		int destination;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class TransferAllowed: public MTP3Message {
	public:
		TransferAllowed();
		int destination;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class TransferRestricted: public MTP3Message {
	public:
		TransferRestricted();
		int destination;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class RouteSetTestProhibitedDest: public MTP3Message {
	public:
		RouteSetTestProhibitedDest();
		int destination;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class RouteSetTestRestrictedDest: public MTP3Message {
	public:
		RouteSetTestRestrictedDest();
		int destination;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class LinkInhibit: public MTP3Message {
	public:
		LinkInhibit();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class LinkUninhibit: public MTP3Message {
	public:
		LinkUninhibit();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class LinkInhibited_ack: public MTP3Message {
	public:
		LinkInhibited_ack();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class LinkUninhibited_ack: public MTP3Message {
	public:
		LinkUninhibited_ack();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class LinkInhibit_denied: public MTP3Message {
	public:
		LinkInhibit_denied();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class LinkForceUninhibit: public MTP3Message {
	public:
		LinkForceUninhibit();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class LinkLocalInhibitTest: public MTP3Message {
	public:
		LinkLocalInhibitTest();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class LinkRemoteInhibitTest: public MTP3Message {
	public:
		LinkRemoteInhibitTest();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class TrafficRestartAllowed: public MTP3Message {
	public:
		TrafficRestartAllowed();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class DataLinkConn_order: public MTP3Message {
	public:
		DataLinkConn_order();
		int data_link_identity;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class ConnSuccessful: public MTP3Message {
	public:
		ConnSuccessful();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class ConnNotSuccessful: public MTP3Message {
	public:
		ConnNotSuccessful();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class ConnNotPossible: public MTP3Message {
	public:
		ConnNotPossible();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class TransferControlled: public MTP3Message {
	public:
		TransferControlled();
		int destination;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class RouteSetCongestionTest: public MTP3Message {
	public:
		RouteSetCongestionTest();
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class UserPartUnavailable: public MTP3Message {
	public:
		UserPartUnavailable();
		int destination;
		UserPartIdType user_part_id;
		UnavailCauseType unavailability_cause;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	// Signalling network testing and maintenance messages

	class Sltm: public MTP3Message {
	public:
		Sltm();
		int test_pattern_length;
		char* test_pattern;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};

	class Slta: public MTP3Message {
	public:
		Slta();
		int test_pattern_length;
		char* test_pattern;
		void init(char* data, int data_length);
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};



	class MTP3Packet {
	public:
		MTP3Packet();
		~MTP3Packet();
		NetworkIndicatorType network_indicator;
		ServiceIndicatorType service_indicator;
		MTP3Label label;
		char* sif;
		int sif_length;
		MTP3Message* signalling_message;
		void encode(char* buffer, int buffer_length, int offset, int* result_length);
	};




	/**
	 * Decodes mtp3 message
	 * @param data pointer to mtp3 bytes
	 * @param data_length number of bytes contained in mtp3 packet
	 * @return Pointer to mtp3 message object or NULL no data available
	 */
	MTP3Packet* decode(char* data, int data_length);

}



#endif /* MTP3_H_ */
