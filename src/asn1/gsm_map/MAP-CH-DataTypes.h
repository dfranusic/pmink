#ifndef MAP_CH_DATATYPES_H_
#define MAP_CH_DATATYPES_H_

#include<asn1.h>
#include<MAP-MS-DataTypes.h>
#include<MAP-SS-DataTypes.h>
#include<MAP-CommonDataTypes.h>
#include<MAP-ExtensionDataTypes.h>

namespace asn1 {
	// forward declarations
	class CUG_CheckInfo;
	class NumberOfForwarding;
	class SendRoutingInfoArg;
	class SuppressionOfAnnouncement;
	class SuppressMTSS;
	class InterrogationType;
	class OR_Phase;
	class CallReferenceNumber;
	class ForwardingReason;
	class SupportedCCBS_Phase;
	class CallDiversionTreatmentIndicator;
	class SendRoutingInfoRes;
	class AllowedServices;
	class UnavailabilityCause;
	class CCBS_Indicators;
	class RoutingInfo;
	class ForwardingData;
	class ProvideRoamingNumberArg;
	class ProvideRoamingNumberRes;
	class ResumeCallHandlingArg;
	class UU_Data;
	class UUIndicator;
	class UUI;
	class ResumeCallHandlingRes;
	class CamelInfo;
	class ExtendedRoutingInfo;
	class CamelRoutingInfo;
	class GmscCamelSubscriptionInfo;
	class SetReportingStateArg;
	class ReportingState;
	class SetReportingStateRes;
	class CCBS_SubscriberStatus;
	class StatusReportArg;
	class EventReportData;
	class CallReportData;
	class MonitoringMode;
	class CallOutcome;
	class StatusReportRes;
	class RemoteUserFreeArg;
	class RemoteUserFreeRes;
	class RUF_Outcome;
	class IST_AlertArg;
	class IST_AlertRes;
	class IST_CommandArg;
	class IST_CommandRes;
	class CallTerminationIndicator;
	class ReleaseResourcesArg;
	class ReleaseResourcesRes;

	

	// CUG_CheckInfo
	class CUG_CheckInfo : public Sequence {
	public:
		CUG_CheckInfo();
		~CUG_CheckInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cug_OutgoingAccess();
		void set_extensionContainer();
		// nodes
		CUG_Interlock* _cug_Interlock;
		Null* _cug_OutgoingAccess;
		ExtensionContainer* _extensionContainer;

	};

	// NumberOfForwarding
	class NumberOfForwarding : public Integer {
	public:
		NumberOfForwarding();
		~NumberOfForwarding();

	};

	// SendRoutingInfoArg
	class SendRoutingInfoArg : public Sequence {
	public:
		SendRoutingInfoArg();
		~SendRoutingInfoArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cug_CheckInfo();
		void set_numberOfForwarding();
		void set_or_Interrogation();
		void set_or_Capability();
		void set_callReferenceNumber();
		void set_forwardingReason();
		void set_basicServiceGroup();
		void set_networkSignalInfo();
		void set_camelInfo();
		void set_suppressionOfAnnouncement();
		void set_extensionContainer();
		void set_alertingPattern();
		void set_ccbs_Call();
		void set_supportedCCBS_Phase();
		void set_additionalSignalInfo();
		void set_istSupportIndicator();
		void set_pre_pagingSupported();
		void set_callDiversionTreatmentIndicator();
		void set_longFTN_Supported();
		void set_suppress_VT_CSI();
		void set_suppressIncomingCallBarring();
		void set_gsmSCF_InitiatedCall();
		void set_basicServiceGroup2();
		void set_networkSignalInfo2();
		void set_suppressMTSS();
		void set_mtRoamingRetrySupported();
		void set_callPriority();
		// nodes
		ISDN_AddressString* _msisdn;
		CUG_CheckInfo* _cug_CheckInfo;
		NumberOfForwarding* _numberOfForwarding;
		InterrogationType* _interrogationType;
		Null* _or_Interrogation;
		OR_Phase* _or_Capability;
		ISDN_AddressString* _gmsc_OrGsmSCF_Address;
		CallReferenceNumber* _callReferenceNumber;
		ForwardingReason* _forwardingReason;
		Ext_BasicServiceCode* _basicServiceGroup;
		ExternalSignalInfo* _networkSignalInfo;
		CamelInfo* _camelInfo;
		SuppressionOfAnnouncement* _suppressionOfAnnouncement;
		ExtensionContainer* _extensionContainer;
		AlertingPattern* _alertingPattern;
		Null* _ccbs_Call;
		SupportedCCBS_Phase* _supportedCCBS_Phase;
		Ext_ExternalSignalInfo* _additionalSignalInfo;
		IST_SupportIndicator* _istSupportIndicator;
		Null* _pre_pagingSupported;
		CallDiversionTreatmentIndicator* _callDiversionTreatmentIndicator;
		Null* _longFTN_Supported;
		Null* _suppress_VT_CSI;
		Null* _suppressIncomingCallBarring;
		Null* _gsmSCF_InitiatedCall;
		Ext_BasicServiceCode* _basicServiceGroup2;
		ExternalSignalInfo* _networkSignalInfo2;
		SuppressMTSS* _suppressMTSS;
		Null* _mtRoamingRetrySupported;
		EMLPP_Priority* _callPriority;

	};

	// SuppressionOfAnnouncement
	class SuppressionOfAnnouncement : public Null {
	public:
		SuppressionOfAnnouncement();
		~SuppressionOfAnnouncement();

	};

	// SuppressMTSS
	class SuppressMTSS : public Bit_string {
	public:
		SuppressMTSS();
		~SuppressMTSS();

	};

	// InterrogationType
	class InterrogationType : public Enumerated {
	public:
		InterrogationType();
		~InterrogationType();
		static const int _basicCall = 0;
		static const int _forwarding = 1;

	};

	// OR_Phase
	class OR_Phase : public Integer {
	public:
		OR_Phase();
		~OR_Phase();

	};

	// CallReferenceNumber
	class CallReferenceNumber : public Octet_string {
	public:
		CallReferenceNumber();
		~CallReferenceNumber();

	};

	// ForwardingReason
	class ForwardingReason : public Enumerated {
	public:
		ForwardingReason();
		~ForwardingReason();
		static const int _notReachable = 0;
		static const int _busy = 1;
		static const int _noReply = 2;

	};

	// SupportedCCBS_Phase
	class SupportedCCBS_Phase : public Integer {
	public:
		SupportedCCBS_Phase();
		~SupportedCCBS_Phase();

	};

	// CallDiversionTreatmentIndicator
	class CallDiversionTreatmentIndicator : public Octet_string {
	public:
		CallDiversionTreatmentIndicator();
		~CallDiversionTreatmentIndicator();

	};

	// SendRoutingInfoRes
	class SendRoutingInfoRes : public Sequence {
	public:
		SendRoutingInfoRes();
		~SendRoutingInfoRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_imsi();
		void set_extendedRoutingInfo();
		void set_cug_CheckInfo();
		void set_cugSubscriptionFlag();
		void set_subscriberInfo();
		void set_ss_List();
		void set_basicService();
		void set_forwardingInterrogationRequired();
		void set_vmsc_Address();
		void set_extensionContainer();
		void set_naea_PreferredCI();
		void set_ccbs_Indicators();
		void set_msisdn();
		void set_numberPortabilityStatus();
		void set_istAlertTimer();
		void set_supportedCamelPhasesInVMSC();
		void set_offeredCamel4CSIsInVMSC();
		void set_routingInfo2();
		void set_ss_List2();
		void set_basicService2();
		void set_allowedServices();
		void set_unavailabilityCause();
		void set_releaseResourcesSupported();
		void set_gsm_BearerCapability();
		// nodes
		IMSI* _imsi;
		ExtendedRoutingInfo* _extendedRoutingInfo;
		CUG_CheckInfo* _cug_CheckInfo;
		Null* _cugSubscriptionFlag;
		SubscriberInfo* _subscriberInfo;
		SS_List* _ss_List;
		Ext_BasicServiceCode* _basicService;
		Null* _forwardingInterrogationRequired;
		ISDN_AddressString* _vmsc_Address;
		ExtensionContainer* _extensionContainer;
		NAEA_PreferredCI* _naea_PreferredCI;
		CCBS_Indicators* _ccbs_Indicators;
		ISDN_AddressString* _msisdn;
		NumberPortabilityStatus* _numberPortabilityStatus;
		IST_AlertTimerValue* _istAlertTimer;
		SupportedCamelPhases* _supportedCamelPhasesInVMSC;
		OfferedCamel4CSIs* _offeredCamel4CSIsInVMSC;
		RoutingInfo* _routingInfo2;
		SS_List* _ss_List2;
		Ext_BasicServiceCode* _basicService2;
		AllowedServices* _allowedServices;
		UnavailabilityCause* _unavailabilityCause;
		Null* _releaseResourcesSupported;
		ExternalSignalInfo* _gsm_BearerCapability;

	};

	// AllowedServices
	class AllowedServices : public Bit_string {
	public:
		AllowedServices();
		~AllowedServices();

	};

	// UnavailabilityCause
	class UnavailabilityCause : public Enumerated {
	public:
		UnavailabilityCause();
		~UnavailabilityCause();
		static const int _bearerServiceNotProvisioned = 1;
		static const int _teleserviceNotProvisioned = 2;
		static const int _absentSubscriber = 3;
		static const int _busySubscriber = 4;
		static const int _callBarred = 5;
		static const int _cug_Reject = 6;

	};

	// CCBS_Indicators
	class CCBS_Indicators : public Sequence {
	public:
		CCBS_Indicators();
		~CCBS_Indicators();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ccbs_Possible();
		void set_keepCCBS_CallIndicator();
		void set_extensionContainer();
		// nodes
		Null* _ccbs_Possible;
		Null* _keepCCBS_CallIndicator;
		ExtensionContainer* _extensionContainer;

	};

	// RoutingInfo
	class RoutingInfo : public Choice {
	public:
		RoutingInfo();
		~RoutingInfo();
		// nodes
		ISDN_AddressString* _roamingNumber;
		ForwardingData* _forwardingData;

	};

	// ForwardingData
	class ForwardingData : public Sequence {
	public:
		ForwardingData();
		~ForwardingData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_forwardedToNumber();
		void set_forwardedToSubaddress();
		void set_forwardingOptions();
		void set_extensionContainer();
		void set_longForwardedToNumber();
		// nodes
		ISDN_AddressString* _forwardedToNumber;
		ISDN_SubaddressString* _forwardedToSubaddress;
		ForwardingOptions* _forwardingOptions;
		ExtensionContainer* _extensionContainer;
		FTN_AddressString* _longForwardedToNumber;

	};

	// ProvideRoamingNumberArg
	class ProvideRoamingNumberArg : public Sequence {
	public:
		ProvideRoamingNumberArg();
		~ProvideRoamingNumberArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_msisdn();
		void set_lmsi();
		void set_gsm_BearerCapability();
		void set_networkSignalInfo();
		void set_suppressionOfAnnouncement();
		void set_gmsc_Address();
		void set_callReferenceNumber();
		void set_or_Interrogation();
		void set_extensionContainer();
		void set_alertingPattern();
		void set_ccbs_Call();
		void set_supportedCamelPhasesInInterrogatingNode();
		void set_additionalSignalInfo();
		void set_orNotSupportedInGMSC();
		void set_pre_pagingSupported();
		void set_longFTN_Supported();
		void set_suppress_VT_CSI();
		void set_offeredCamel4CSIsInInterrogatingNode();
		void set_mtRoamingRetrySupported();
		void set_pagingArea();
		void set_callPriority();
		void set_mtrf_Indicator();
		void set_oldMSC_Number();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _msc_Number;
		ISDN_AddressString* _msisdn;
		LMSI* _lmsi;
		ExternalSignalInfo* _gsm_BearerCapability;
		ExternalSignalInfo* _networkSignalInfo;
		SuppressionOfAnnouncement* _suppressionOfAnnouncement;
		ISDN_AddressString* _gmsc_Address;
		CallReferenceNumber* _callReferenceNumber;
		Null* _or_Interrogation;
		ExtensionContainer* _extensionContainer;
		AlertingPattern* _alertingPattern;
		Null* _ccbs_Call;
		SupportedCamelPhases* _supportedCamelPhasesInInterrogatingNode;
		Ext_ExternalSignalInfo* _additionalSignalInfo;
		Null* _orNotSupportedInGMSC;
		Null* _pre_pagingSupported;
		Null* _longFTN_Supported;
		Null* _suppress_VT_CSI;
		OfferedCamel4CSIs* _offeredCamel4CSIsInInterrogatingNode;
		Null* _mtRoamingRetrySupported;
		PagingArea* _pagingArea;
		EMLPP_Priority* _callPriority;
		Null* _mtrf_Indicator;
		ISDN_AddressString* _oldMSC_Number;

	};

	// ProvideRoamingNumberRes
	class ProvideRoamingNumberRes : public Sequence {
	public:
		ProvideRoamingNumberRes();
		~ProvideRoamingNumberRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_releaseResourcesSupported();
		void set_vmsc_Address();
		// nodes
		ISDN_AddressString* _roamingNumber;
		ExtensionContainer* _extensionContainer;
		Null* _releaseResourcesSupported;
		ISDN_AddressString* _vmsc_Address;

	};

	// ResumeCallHandlingArg
	class ResumeCallHandlingArg : public Sequence {
	public:
		ResumeCallHandlingArg();
		~ResumeCallHandlingArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_callReferenceNumber();
		void set_basicServiceGroup();
		void set_forwardingData();
		void set_imsi();
		void set_cug_CheckInfo();
		void set_o_CSI();
		void set_extensionContainer();
		void set_ccbs_Possible();
		void set_msisdn();
		void set_uu_Data();
		void set_allInformationSent();
		void set_d_csi();
		void set_o_BcsmCamelTDPCriteriaList();
		void set_basicServiceGroup2();
		void set_mtRoamingRetry();
		// nodes
		CallReferenceNumber* _callReferenceNumber;
		Ext_BasicServiceCode* _basicServiceGroup;
		ForwardingData* _forwardingData;
		IMSI* _imsi;
		CUG_CheckInfo* _cug_CheckInfo;
		O_CSI* _o_CSI;
		ExtensionContainer* _extensionContainer;
		Null* _ccbs_Possible;
		ISDN_AddressString* _msisdn;
		UU_Data* _uu_Data;
		Null* _allInformationSent;
		D_CSI* _d_csi;
		O_BcsmCamelTDPCriteriaList* _o_BcsmCamelTDPCriteriaList;
		Ext_BasicServiceCode* _basicServiceGroup2;
		Null* _mtRoamingRetry;

	};

	// UU_Data
	class UU_Data : public Sequence {
	public:
		UU_Data();
		~UU_Data();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_uuIndicator();
		void set_uui();
		void set_uusCFInteraction();
		void set_extensionContainer();
		// nodes
		UUIndicator* _uuIndicator;
		UUI* _uui;
		Null* _uusCFInteraction;
		ExtensionContainer* _extensionContainer;

	};

	// UUIndicator
	class UUIndicator : public Octet_string {
	public:
		UUIndicator();
		~UUIndicator();

	};

	// UUI
	class UUI : public Octet_string {
	public:
		UUI();
		~UUI();

	};

	// ResumeCallHandlingRes
	class ResumeCallHandlingRes : public Sequence {
	public:
		ResumeCallHandlingRes();
		~ResumeCallHandlingRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// CamelInfo
	class CamelInfo : public Sequence {
	public:
		CamelInfo();
		~CamelInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_suppress_T_CSI();
		void set_extensionContainer();
		void set_offeredCamel4CSIs();
		// nodes
		SupportedCamelPhases* _supportedCamelPhases;
		Null* _suppress_T_CSI;
		ExtensionContainer* _extensionContainer;
		OfferedCamel4CSIs* _offeredCamel4CSIs;

	};

	// ExtendedRoutingInfo
	class ExtendedRoutingInfo : public Choice {
	public:
		ExtendedRoutingInfo();
		~ExtendedRoutingInfo();
		// nodes
		RoutingInfo* _routingInfo;
		CamelRoutingInfo* _camelRoutingInfo;

	};

	// CamelRoutingInfo
	class CamelRoutingInfo : public Sequence {
	public:
		CamelRoutingInfo();
		~CamelRoutingInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_forwardingData();
		void set_extensionContainer();
		// nodes
		ForwardingData* _forwardingData;
		GmscCamelSubscriptionInfo* _gmscCamelSubscriptionInfo;
		ExtensionContainer* _extensionContainer;

	};

	// GmscCamelSubscriptionInfo
	class GmscCamelSubscriptionInfo : public Sequence {
	public:
		GmscCamelSubscriptionInfo();
		~GmscCamelSubscriptionInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_t_CSI();
		void set_o_CSI();
		void set_extensionContainer();
		void set_o_BcsmCamelTDP_CriteriaList();
		void set_t_BCSM_CAMEL_TDP_CriteriaList();
		void set_d_csi();
		// nodes
		T_CSI* _t_CSI;
		O_CSI* _o_CSI;
		ExtensionContainer* _extensionContainer;
		O_BcsmCamelTDPCriteriaList* _o_BcsmCamelTDP_CriteriaList;
		T_BCSM_CAMEL_TDP_CriteriaList* _t_BCSM_CAMEL_TDP_CriteriaList;
		D_CSI* _d_csi;

	};

	// SetReportingStateArg
	class SetReportingStateArg : public Sequence {
	public:
		SetReportingStateArg();
		~SetReportingStateArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_imsi();
		void set_lmsi();
		void set_ccbs_Monitoring();
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		LMSI* _lmsi;
		ReportingState* _ccbs_Monitoring;
		ExtensionContainer* _extensionContainer;

	};

	// ReportingState
	class ReportingState : public Enumerated {
	public:
		ReportingState();
		~ReportingState();
		static const int _stopMonitoring = 0;
		static const int _startMonitoring = 1;
	};

	// SetReportingStateRes
	class SetReportingStateRes : public Sequence {
	public:
		SetReportingStateRes();
		~SetReportingStateRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ccbs_SubscriberStatus();
		void set_extensionContainer();
		// nodes
		CCBS_SubscriberStatus* _ccbs_SubscriberStatus;
		ExtensionContainer* _extensionContainer;

	};

	// CCBS_SubscriberStatus
	class CCBS_SubscriberStatus : public Enumerated {
	public:
		CCBS_SubscriberStatus();
		~CCBS_SubscriberStatus();
		static const int _ccbsNotIdle = 0;
		static const int _ccbsIdle = 1;
		static const int _ccbsNotReachable = 2;
	};

	// StatusReportArg
	class StatusReportArg : public Sequence {
	public:
		StatusReportArg();
		~StatusReportArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_eventReportData();
		void set_callReportdata();
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		EventReportData* _eventReportData;
		CallReportData* _callReportdata;
		ExtensionContainer* _extensionContainer;

	};

	// EventReportData
	class EventReportData : public Sequence {
	public:
		EventReportData();
		~EventReportData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ccbs_SubscriberStatus();
		void set_extensionContainer();
		// nodes
		CCBS_SubscriberStatus* _ccbs_SubscriberStatus;
		ExtensionContainer* _extensionContainer;

	};

	// CallReportData
	class CallReportData : public Sequence {
	public:
		CallReportData();
		~CallReportData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_monitoringMode();
		void set_callOutcome();
		void set_extensionContainer();
		// nodes
		MonitoringMode* _monitoringMode;
		CallOutcome* _callOutcome;
		ExtensionContainer* _extensionContainer;

	};

	// MonitoringMode
	class MonitoringMode : public Enumerated {
	public:
		MonitoringMode();
		~MonitoringMode();
		static const int _a_side = 0;
		static const int _b_side = 1;

	};

	// CallOutcome
	class CallOutcome : public Enumerated {
	public:
		CallOutcome();
		~CallOutcome();
		static const int _success = 0;
		static const int _failure = 1;
		static const int _busy = 2;

	};

	// StatusReportRes
	class StatusReportRes : public Sequence {
	public:
		StatusReportRes();
		~StatusReportRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// RemoteUserFreeArg
	class RemoteUserFreeArg : public Sequence {
	public:
		RemoteUserFreeArg();
		~RemoteUserFreeArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_replaceB_Number();
		void set_alertingPattern();
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		ExternalSignalInfo* _callInfo;
		CCBS_Feature* _ccbs_Feature;
		ISDN_AddressString* _translatedB_Number;
		Null* _replaceB_Number;
		AlertingPattern* _alertingPattern;
		ExtensionContainer* _extensionContainer;

	};

	// RemoteUserFreeRes
	class RemoteUserFreeRes : public Sequence {
	public:
		RemoteUserFreeRes();
		~RemoteUserFreeRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		RUF_Outcome* _ruf_Outcome;
		ExtensionContainer* _extensionContainer;

	};

	// RUF_Outcome
	class RUF_Outcome : public Enumerated {
	public:
		RUF_Outcome();
		~RUF_Outcome();
		static const int _accepted = 0;
		static const int _rejected = 1;
		static const int _noResponseFromFreeMS = 2;
		static const int _noResponseFromBusyMS = 3;
		static const int _udubFromFreeMS = 4;
		static const int _udubFromBusyMS = 5;

	};

	// IST_AlertArg
	class IST_AlertArg : public Sequence {
	public:
		IST_AlertArg();
		~IST_AlertArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		ExtensionContainer* _extensionContainer;

	};

	// IST_AlertRes
	class IST_AlertRes : public Sequence {
	public:
		IST_AlertRes();
		~IST_AlertRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_istAlertTimer();
		void set_istInformationWithdraw();
		void set_callTerminationIndicator();
		void set_extensionContainer();
		// nodes
		IST_AlertTimerValue* _istAlertTimer;
		Null* _istInformationWithdraw;
		CallTerminationIndicator* _callTerminationIndicator;
		ExtensionContainer* _extensionContainer;

	};

	// IST_CommandArg
	class IST_CommandArg : public Sequence {
	public:
		IST_CommandArg();
		~IST_CommandArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		ExtensionContainer* _extensionContainer;

	};

	// IST_CommandRes
	class IST_CommandRes : public Sequence {
	public:
		IST_CommandRes();
		~IST_CommandRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// CallTerminationIndicator
	class CallTerminationIndicator : public Enumerated {
	public:
		CallTerminationIndicator();
		~CallTerminationIndicator();
		static const int _terminateCallActivityReferred = 0;
		static const int _terminateAllCallActivities = 1;
	};

	// ReleaseResourcesArg
	class ReleaseResourcesArg : public Sequence {
	public:
		ReleaseResourcesArg();
		~ReleaseResourcesArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ISDN_AddressString* _msrn;
		ExtensionContainer* _extensionContainer;

	};

	// ReleaseResourcesRes
	class ReleaseResourcesRes : public Sequence {
	public:
		ReleaseResourcesRes();
		~ReleaseResourcesRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

}
#endif
