#ifndef MAP_OM_DATATYPES_H_
#define MAP_OM_DATATYPES_H_

#include<asn1.h>
#include<MAP-CommonDataTypes.h>

namespace asn1 {
	// forward declarations
	class ActivateTraceModeArg;
	class MDT_Configuration;
	class JobType;
	class AreaScope;
	class CGI_List;
	class E_UTRAN_CGI_List;
	class RoutingAreaId_List;
	class LocationAreaId_List;
	class TrackingAreaId_List;
	class ListOfMeasurements;
	class ReportingTrigger;
	class ReportInterval;
	class ReportAmount;
	class EventThresholdRSRP;
	class EventThresholdRSRQ;
	class LoggingInterval;
	class LoggingDuration;
	class TraceReference;
	class TraceReference2;
	class TraceRecordingSessionReference;
	class TraceType;
	class TraceDepthList;
	class TraceDepth;
	class TraceDepthExtension;
	class TraceNE_TypeList;
	class TraceInterfaceList;
	class MSC_S_InterfaceList;
	class MGW_InterfaceList;
	class SGSN_InterfaceList;
	class GGSN_InterfaceList;
	class RNC_InterfaceList;
	class BMSC_InterfaceList;
	class MME_InterfaceList;
	class SGW_InterfaceList;
	class PGW_InterfaceList;
	class ENB_InterfaceList;
	class TraceEventList;
	class MSC_S_EventList;
	class MGW_EventList;
	class SGSN_EventList;
	class GGSN_EventList;
	class BMSC_EventList;
	class MME_EventList;
	class SGW_EventList;
	class PGW_EventList;
	class TracePropagationList;
	class ActivateTraceModeRes;
	class DeactivateTraceModeArg;
	class DeactivateTraceModeRes;

	

	// ActivateTraceModeArg
	class ActivateTraceModeArg : public Sequence {
	public:
		ActivateTraceModeArg();
		~ActivateTraceModeArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_imsi();
		void set_omc_Id();
		void set_extensionContainer();
		void set_traceReference2();
		void set_traceDepthList();
		void set_traceNE_TypeList();
		void set_traceInterfaceList();
		void set_traceEventList();
		void set_traceCollectionEntity();
		void set_mdt_Configuration();
		// nodes
		IMSI* _imsi;
		TraceReference* _traceReference;
		TraceType* _traceType;
		AddressString* _omc_Id;
		ExtensionContainer* _extensionContainer;
		TraceReference2* _traceReference2;
		TraceDepthList* _traceDepthList;
		TraceNE_TypeList* _traceNE_TypeList;
		TraceInterfaceList* _traceInterfaceList;
		TraceEventList* _traceEventList;
		GSN_Address* _traceCollectionEntity;
		MDT_Configuration* _mdt_Configuration;

	};

	// MDT_Configuration
	class MDT_Configuration : public Sequence {
	public:
		MDT_Configuration();
		~MDT_Configuration();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_areaScope();
		void set_listOfMeasurements();
		void set_reportingTrigger();
		void set_reportInterval();
		void set_reportAmount();
		void set_eventThresholdRSRP();
		void set_eventThresholdRSRQ();
		void set_loggingInterval();
		void set_loggingDuration();
		void set_extensionContainer();
		// nodes
		JobType* _jobType;
		AreaScope* _areaScope;
		ListOfMeasurements* _listOfMeasurements;
		ReportingTrigger* _reportingTrigger;
		ReportInterval* _reportInterval;
		ReportAmount* _reportAmount;
		EventThresholdRSRP* _eventThresholdRSRP;
		EventThresholdRSRQ* _eventThresholdRSRQ;
		LoggingInterval* _loggingInterval;
		LoggingDuration* _loggingDuration;
		ExtensionContainer* _extensionContainer;

	};

	// JobType
	class JobType : public Enumerated {
	public:
		JobType();
		~JobType();
		static const int _immediate_MDT_only = 0;
		static const int _logged_MDT_only = 1;
		static const int _trace_only = 2;
		static const int _immediate_MDT_and_trace = 3;

	};

	// AreaScope
	class AreaScope : public Sequence {
	public:
		AreaScope();
		~AreaScope();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cgi_List();
		void set_e_utran_cgi_List();
		void set_routingAreaId_List();
		void set_locationAreaId_List();
		void set_trackingAreaId_List();
		void set_extensionContainer();
		// nodes
		CGI_List* _cgi_List;
		E_UTRAN_CGI_List* _e_utran_cgi_List;
		RoutingAreaId_List* _routingAreaId_List;
		LocationAreaId_List* _locationAreaId_List;
		TrackingAreaId_List* _trackingAreaId_List;
		ExtensionContainer* _extensionContainer;

	};

	// CGI_List
	class CGI_List : public Sequence_of {
	public:
		CGI_List();
		~CGI_List();
		// nodes
		GlobalCellId* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// E_UTRAN_CGI_List
	class E_UTRAN_CGI_List : public Sequence_of {
	public:
		E_UTRAN_CGI_List();
		~E_UTRAN_CGI_List();
		// nodes
		E_UTRAN_CGI* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// RoutingAreaId_List
	class RoutingAreaId_List : public Sequence_of {
	public:
		RoutingAreaId_List();
		~RoutingAreaId_List();
		// nodes
		RAIdentity* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// LocationAreaId_List
	class LocationAreaId_List : public Sequence_of {
	public:
		LocationAreaId_List();
		~LocationAreaId_List();
		// nodes
		LAIFixedLength* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// TrackingAreaId_List
	class TrackingAreaId_List : public Sequence_of {
	public:
		TrackingAreaId_List();
		~TrackingAreaId_List();
		// nodes
		TA_Id* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// ListOfMeasurements
	class ListOfMeasurements : public Octet_string {
	public:
		ListOfMeasurements();
		~ListOfMeasurements();

	};

	// ReportingTrigger
	class ReportingTrigger : public Octet_string {
	public:
		ReportingTrigger();
		~ReportingTrigger();

	};

	// ReportInterval
	class ReportInterval : public Enumerated {
	public:
		ReportInterval();
		~ReportInterval();
		static const int _umts250ms = 0;
		static const int _umts500ms = 1;
		static const int _umts1000ms = 2;
		static const int _umts2000ms = 3;
		static const int _umts3000ms = 4;
		static const int _umts4000ms = 5;
		static const int _umts6000ms = 6;
		static const int _umts8000ms = 7;
		static const int _umts12000ms = 8;
		static const int _umts16000ms = 9;
		static const int _umts20000ms = 10;
		static const int _umts24000ms = 11;
		static const int _umts28000ms = 12;
		static const int _umts32000ms = 13;
		static const int _umts64000ms = 14;
		static const int _lte120ms = 15;
		static const int _lte240ms = 16;
		static const int _lte480ms = 17;
		static const int _lte640ms = 18;
		static const int _lte1024ms = 19;
		static const int _lte2048ms = 20;
		static const int _lte5120ms = 21;
		static const int _lte10240ms = 22;
		static const int _lte1min = 23;
		static const int _lte6min = 24;
		static const int _lte12min = 25;
		static const int _lte30min = 26;
		static const int _lte60min = 27;

	};

	// ReportAmount
	class ReportAmount : public Enumerated {
	public:
		ReportAmount();
		~ReportAmount();
		static const int _d1 = 0;
		static const int _d2 = 1;
		static const int _d4 = 2;
		static const int _d8 = 3;
		static const int _d16 = 4;
		static const int _d32 = 5;
		static const int _d64 = 6;
		static const int _infinity = 7;

	};

	// EventThresholdRSRP
	class EventThresholdRSRP : public Integer {
	public:
		EventThresholdRSRP();
		~EventThresholdRSRP();

	};

	// EventThresholdRSRQ
	class EventThresholdRSRQ : public Integer {
	public:
		EventThresholdRSRQ();
		~EventThresholdRSRQ();

	};

	// LoggingInterval
	class LoggingInterval : public Enumerated {
	public:
		LoggingInterval();
		~LoggingInterval();
		static const int _d1dot28 = 0;
		static const int _d2dot56 = 1;
		static const int _d5dot12 = 2;
		static const int _d10dot24 = 3;
		static const int _d20dot48 = 4;
		static const int _d30dot72 = 5;
		static const int _d40dot96 = 6;
		static const int _d61dot44 = 7;

	};

	// LoggingDuration
	class LoggingDuration : public Enumerated {
	public:
		LoggingDuration();
		~LoggingDuration();
		static const int _d600sec = 0;
		static const int _d1200sec = 1;
		static const int _d2400sec = 2;
		static const int _d3600sec = 3;
		static const int _d5400sec = 4;
		static const int _d7200sec = 5;

	};

	// TraceReference
	class TraceReference : public Octet_string {
	public:
		TraceReference();
		~TraceReference();

	};

	// TraceReference2
	class TraceReference2 : public Octet_string {
	public:
		TraceReference2();
		~TraceReference2();

	};

	// TraceRecordingSessionReference
	class TraceRecordingSessionReference : public Octet_string {
	public:
		TraceRecordingSessionReference();
		~TraceRecordingSessionReference();

	};

	// TraceType
	class TraceType : public Integer {
	public:
		TraceType();
		~TraceType();

	};

	// TraceDepthList
	class TraceDepthList : public Sequence {
	public:
		TraceDepthList();
		~TraceDepthList();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_msc_s_TraceDepth();
		void set_mgw_TraceDepth();
		void set_sgsn_TraceDepth();
		void set_ggsn_TraceDepth();
		void set_rnc_TraceDepth();
		void set_bmsc_TraceDepth();
		void set_mme_TraceDepth();
		void set_sgw_TraceDepth();
		void set_pgw_TraceDepth();
		void set_eNB_TraceDepth();
		void set_msc_s_TraceDepthExtension();
		void set_mgw_TraceDepthExtension();
		void set_sgsn_TraceDepthExtension();
		void set_ggsn_TraceDepthExtension();
		void set_rnc_TraceDepthExtension();
		void set_bmsc_TraceDepthExtension();
		void set_mme_TraceDepthExtension();
		void set_sgw_TraceDepthExtension();
		void set_pgw_TraceDepthExtension();
		void set_eNB_TraceDepthExtension();
		// nodes
		TraceDepth* _msc_s_TraceDepth;
		TraceDepth* _mgw_TraceDepth;
		TraceDepth* _sgsn_TraceDepth;
		TraceDepth* _ggsn_TraceDepth;
		TraceDepth* _rnc_TraceDepth;
		TraceDepth* _bmsc_TraceDepth;
		TraceDepth* _mme_TraceDepth;
		TraceDepth* _sgw_TraceDepth;
		TraceDepth* _pgw_TraceDepth;
		TraceDepth* _eNB_TraceDepth;
		TraceDepthExtension* _msc_s_TraceDepthExtension;
		TraceDepthExtension* _mgw_TraceDepthExtension;
		TraceDepthExtension* _sgsn_TraceDepthExtension;
		TraceDepthExtension* _ggsn_TraceDepthExtension;
		TraceDepthExtension* _rnc_TraceDepthExtension;
		TraceDepthExtension* _bmsc_TraceDepthExtension;
		TraceDepthExtension* _mme_TraceDepthExtension;
		TraceDepthExtension* _sgw_TraceDepthExtension;
		TraceDepthExtension* _pgw_TraceDepthExtension;
		TraceDepthExtension* _eNB_TraceDepthExtension;

	};

	// TraceDepth
	class TraceDepth : public Enumerated {
	public:
		TraceDepth();
		~TraceDepth();
		static const int _minimum = 0;
		static const int _medium = 1;
		static const int _maximum = 2;

	};

	// TraceDepthExtension
	class TraceDepthExtension : public Enumerated {
	public:
		TraceDepthExtension();
		~TraceDepthExtension();
		static const int _minimumWithoutVendorSpecificExtension = 0;
		static const int _mediumWithoutVendorSpecificExtension = 1;
		static const int _maximumWithoutVendorSpecificExtension = 2;

	};

	// TraceNE_TypeList
	class TraceNE_TypeList : public Bit_string {
	public:
		TraceNE_TypeList();
		~TraceNE_TypeList();

	};

	// TraceInterfaceList
	class TraceInterfaceList : public Sequence {
	public:
		TraceInterfaceList();
		~TraceInterfaceList();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_msc_s_List();
		void set_mgw_List();
		void set_sgsn_List();
		void set_ggsn_List();
		void set_rnc_List();
		void set_bmsc_List();
		void set_mme_List();
		void set_sgw_List();
		void set_pgw_List();
		void set_eNB_List();
		// nodes
		MSC_S_InterfaceList* _msc_s_List;
		MGW_InterfaceList* _mgw_List;
		SGSN_InterfaceList* _sgsn_List;
		GGSN_InterfaceList* _ggsn_List;
		RNC_InterfaceList* _rnc_List;
		BMSC_InterfaceList* _bmsc_List;
		MME_InterfaceList* _mme_List;
		SGW_InterfaceList* _sgw_List;
		PGW_InterfaceList* _pgw_List;
		ENB_InterfaceList* _eNB_List;

	};

	// MSC_S_InterfaceList
	class MSC_S_InterfaceList : public Bit_string {
	public:
		MSC_S_InterfaceList();
		~MSC_S_InterfaceList();

	};

	// MGW_InterfaceList
	class MGW_InterfaceList : public Bit_string {
	public:
		MGW_InterfaceList();
		~MGW_InterfaceList();

	};

	// SGSN_InterfaceList
	class SGSN_InterfaceList : public Bit_string {
	public:
		SGSN_InterfaceList();
		~SGSN_InterfaceList();

	};

	// GGSN_InterfaceList
	class GGSN_InterfaceList : public Bit_string {
	public:
		GGSN_InterfaceList();
		~GGSN_InterfaceList();

	};

	// RNC_InterfaceList
	class RNC_InterfaceList : public Bit_string {
	public:
		RNC_InterfaceList();
		~RNC_InterfaceList();

	};

	// BMSC_InterfaceList
	class BMSC_InterfaceList : public Bit_string {
	public:
		BMSC_InterfaceList();
		~BMSC_InterfaceList();

	};

	// MME_InterfaceList
	class MME_InterfaceList : public Bit_string {
	public:
		MME_InterfaceList();
		~MME_InterfaceList();

	};

	// SGW_InterfaceList
	class SGW_InterfaceList : public Bit_string {
	public:
		SGW_InterfaceList();
		~SGW_InterfaceList();

	};

	// PGW_InterfaceList
	class PGW_InterfaceList : public Bit_string {
	public:
		PGW_InterfaceList();
		~PGW_InterfaceList();

	};

	// ENB_InterfaceList
	class ENB_InterfaceList : public Bit_string {
	public:
		ENB_InterfaceList();
		~ENB_InterfaceList();

	};

	// TraceEventList
	class TraceEventList : public Sequence {
	public:
		TraceEventList();
		~TraceEventList();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_msc_s_List();
		void set_mgw_List();
		void set_sgsn_List();
		void set_ggsn_List();
		void set_bmsc_List();
		void set_mme_List();
		void set_sgw_List();
		void set_pgw_List();
		// nodes
		MSC_S_EventList* _msc_s_List;
		MGW_EventList* _mgw_List;
		SGSN_EventList* _sgsn_List;
		GGSN_EventList* _ggsn_List;
		BMSC_EventList* _bmsc_List;
		MME_EventList* _mme_List;
		SGW_EventList* _sgw_List;
		PGW_EventList* _pgw_List;

	};

	// MSC_S_EventList
	class MSC_S_EventList : public Bit_string {
	public:
		MSC_S_EventList();
		~MSC_S_EventList();

	};

	// MGW_EventList
	class MGW_EventList : public Bit_string {
	public:
		MGW_EventList();
		~MGW_EventList();

	};

	// SGSN_EventList
	class SGSN_EventList : public Bit_string {
	public:
		SGSN_EventList();
		~SGSN_EventList();

	};

	// GGSN_EventList
	class GGSN_EventList : public Bit_string {
	public:
		GGSN_EventList();
		~GGSN_EventList();

	};

	// BMSC_EventList
	class BMSC_EventList : public Bit_string {
	public:
		BMSC_EventList();
		~BMSC_EventList();

	};

	// MME_EventList
	class MME_EventList : public Bit_string {
	public:
		MME_EventList();
		~MME_EventList();

	};

	// SGW_EventList
	class SGW_EventList : public Bit_string {
	public:
		SGW_EventList();
		~SGW_EventList();

	};

	// PGW_EventList
	class PGW_EventList : public Bit_string {
	public:
		PGW_EventList();
		~PGW_EventList();

	};

	// TracePropagationList
	class TracePropagationList : public Sequence {
	public:
		TracePropagationList();
		~TracePropagationList();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_traceReference();
		void set_traceType();
		void set_traceReference2();
		void set_traceRecordingSessionReference();
		void set_rnc_TraceDepth();
		void set_rnc_InterfaceList();
		void set_msc_s_TraceDepth();
		void set_msc_s_InterfaceList();
		void set_msc_s_EventList();
		void set_mgw_TraceDepth();
		void set_mgw_InterfaceList();
		void set_mgw_EventList();
		void set_rnc_TraceDepthExtension();
		void set_msc_s_TraceDepthExtension();
		void set_mgw_TraceDepthExtension();
		// nodes
		TraceReference* _traceReference;
		TraceType* _traceType;
		TraceReference2* _traceReference2;
		TraceRecordingSessionReference* _traceRecordingSessionReference;
		TraceDepth* _rnc_TraceDepth;
		RNC_InterfaceList* _rnc_InterfaceList;
		TraceDepth* _msc_s_TraceDepth;
		MSC_S_InterfaceList* _msc_s_InterfaceList;
		MSC_S_EventList* _msc_s_EventList;
		TraceDepth* _mgw_TraceDepth;
		MGW_InterfaceList* _mgw_InterfaceList;
		MGW_EventList* _mgw_EventList;
		TraceDepthExtension* _rnc_TraceDepthExtension;
		TraceDepthExtension* _msc_s_TraceDepthExtension;
		TraceDepthExtension* _mgw_TraceDepthExtension;

	};

	// ActivateTraceModeRes
	class ActivateTraceModeRes : public Sequence {
	public:
		ActivateTraceModeRes();
		~ActivateTraceModeRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_traceSupportIndicator();
		// nodes
		ExtensionContainer* _extensionContainer;
		Null* _traceSupportIndicator;

	};

	// DeactivateTraceModeArg
	class DeactivateTraceModeArg : public Sequence {
	public:
		DeactivateTraceModeArg();
		~DeactivateTraceModeArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_imsi();
		void set_extensionContainer();
		void set_traceReference2();
		// nodes
		IMSI* _imsi;
		TraceReference* _traceReference;
		ExtensionContainer* _extensionContainer;
		TraceReference2* _traceReference2;

	};

	// DeactivateTraceModeRes
	class DeactivateTraceModeRes : public Sequence {
	public:
		DeactivateTraceModeRes();
		~DeactivateTraceModeRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

}
#endif
