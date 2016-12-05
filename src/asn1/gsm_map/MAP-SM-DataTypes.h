#ifndef MAP_SM_DATATYPES_H_
#define MAP_SM_DATATYPES_H_

#include<asn1.h>
#include<MAP-CommonDataTypes.h>
#include<MAP-CommonDataTypes.h>
#include<MAP-ExtensionDataTypes.h>
#include<MAP-ER-DataTypes.h>

namespace asn1 {
	// forward declarations
	class RoutingInfoForSM_Arg;
	class SM_DeliveryNotIntended;
	class SM_RP_MTI;
	class SM_RP_SMEA;
	class RoutingInfoForSM_Res;
	class IP_SM_GW_Guidance;
	class LocationInfoWithLMSI;
	class Additional_Number;
	class MO_ForwardSM_Arg;
	class MO_ForwardSM_Res;
	class MT_ForwardSM_Arg;
	class MT_ForwardSM_Res;
	class SM_RP_DA;
	class SM_RP_OA;
	class SM_DeliveryTimerValue;
	class ReportSM_DeliveryStatusArg;
	class SM_DeliveryOutcome;
	class ReportSM_DeliveryStatusRes;
	class AlertServiceCentreArg;
	class InformServiceCentreArg;
	class MW_Status;
	class ReadyForSM_Arg;
	class ReadyForSM_Res;
	class AlertReason;
	class MT_ForwardSM_VGCS_Arg;
	class MT_ForwardSM_VGCS_Res;
	class DispatcherList;
	class maxNumOfDispatchers;
	class AdditionalDispatcherList;
	class maxNumOfAdditionalDispatchers;

	

	// RoutingInfoForSM_Arg
	class RoutingInfoForSM_Arg : public Sequence {
	public:
		RoutingInfoForSM_Arg();
		~RoutingInfoForSM_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_gprsSupportIndicator();
		void set_sm_RP_MTI();
		void set_sm_RP_SMEA();
		void set_sm_deliveryNotIntended();
		void set_ip_sm_gwGuidanceIndicator();
		void set_imsi();
		// nodes
		ISDN_AddressString* _msisdn;
		Boolean* _sm_RP_PRI;
		AddressString* _serviceCentreAddress;
		ExtensionContainer* _extensionContainer;
		Null* _gprsSupportIndicator;
		SM_RP_MTI* _sm_RP_MTI;
		SM_RP_SMEA* _sm_RP_SMEA;
		SM_DeliveryNotIntended* _sm_deliveryNotIntended;
		Null* _ip_sm_gwGuidanceIndicator;
		IMSI* _imsi;

	};

	// SM_DeliveryNotIntended
	class SM_DeliveryNotIntended : public Enumerated {
	public:
		SM_DeliveryNotIntended();
		~SM_DeliveryNotIntended();
		static const int _onlyIMSI_requested = 0;
		static const int _onlyMCC_MNC_requested = 1;
	};

	// SM_RP_MTI
	class SM_RP_MTI : public Integer {
	public:
		SM_RP_MTI();
		~SM_RP_MTI();

	};

	// SM_RP_SMEA
	class SM_RP_SMEA : public Octet_string {
	public:
		SM_RP_SMEA();
		~SM_RP_SMEA();

	};

	// RoutingInfoForSM_Res
	class RoutingInfoForSM_Res : public Sequence {
	public:
		RoutingInfoForSM_Res();
		~RoutingInfoForSM_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_ip_sm_gwGuidance();
		// nodes
		IMSI* _imsi;
		LocationInfoWithLMSI* _locationInfoWithLMSI;
		ExtensionContainer* _extensionContainer;
		IP_SM_GW_Guidance* _ip_sm_gwGuidance;

	};

	// IP_SM_GW_Guidance
	class IP_SM_GW_Guidance : public Sequence {
	public:
		IP_SM_GW_Guidance();
		~IP_SM_GW_Guidance();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SM_DeliveryTimerValue* _minimumDeliveryTimeValue;
		SM_DeliveryTimerValue* _recommendedDeliveryTimeValue;
		ExtensionContainer* _extensionContainer;

	};

	// LocationInfoWithLMSI
	class LocationInfoWithLMSI : public Sequence {
	public:
		LocationInfoWithLMSI();
		~LocationInfoWithLMSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_lmsi();
		void set_extensionContainer();
		void set_gprsNodeIndicator();
		void set_additional_Number();
		void set_networkNodeDiameterAddress();
		void set_additionalNetworkNodeDiameterAddress();
		void set_thirdNumber();
		void set_thirdNetworkNodeDiameterAddress();
		void set_imsNodeIndicator();
		// nodes
		ISDN_AddressString* _networkNode_Number;
		LMSI* _lmsi;
		ExtensionContainer* _extensionContainer;
		Null* _gprsNodeIndicator;
		Additional_Number* _additional_Number;
		NetworkNodeDiameterAddress* _networkNodeDiameterAddress;
		NetworkNodeDiameterAddress* _additionalNetworkNodeDiameterAddress;
		Additional_Number* _thirdNumber;
		NetworkNodeDiameterAddress* _thirdNetworkNodeDiameterAddress;
		Null* _imsNodeIndicator;

	};

	// Additional_Number
	class Additional_Number : public Choice {
	public:
		Additional_Number();
		~Additional_Number();
		// nodes
		ISDN_AddressString* _msc_Number;
		ISDN_AddressString* _sgsn_Number;

	};

	// MO_ForwardSM_Arg
	class MO_ForwardSM_Arg : public Sequence {
	public:
		MO_ForwardSM_Arg();
		~MO_ForwardSM_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_imsi();
		// nodes
		SM_RP_DA* _sm_RP_DA;
		SM_RP_OA* _sm_RP_OA;
		SignalInfo* _sm_RP_UI;
		ExtensionContainer* _extensionContainer;
		IMSI* _imsi;

	};

	// MO_ForwardSM_Res
	class MO_ForwardSM_Res : public Sequence {
	public:
		MO_ForwardSM_Res();
		~MO_ForwardSM_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_sm_RP_UI();
		void set_extensionContainer();
		// nodes
		SignalInfo* _sm_RP_UI;
		ExtensionContainer* _extensionContainer;

	};

	// MT_ForwardSM_Arg
	class MT_ForwardSM_Arg : public Sequence {
	public:
		MT_ForwardSM_Arg();
		~MT_ForwardSM_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_moreMessagesToSend();
		void set_extensionContainer();
		void set_smDeliveryTimer();
		void set_smDeliveryStartTime();
		void set_smsOverIP_OnlyIndicator();
		// nodes
		SM_RP_DA* _sm_RP_DA;
		SM_RP_OA* _sm_RP_OA;
		SignalInfo* _sm_RP_UI;
		Null* _moreMessagesToSend;
		ExtensionContainer* _extensionContainer;
		SM_DeliveryTimerValue* _smDeliveryTimer;
		Time* _smDeliveryStartTime;
		Null* _smsOverIP_OnlyIndicator;

	};

	// MT_ForwardSM_Res
	class MT_ForwardSM_Res : public Sequence {
	public:
		MT_ForwardSM_Res();
		~MT_ForwardSM_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_sm_RP_UI();
		void set_extensionContainer();
		// nodes
		SignalInfo* _sm_RP_UI;
		ExtensionContainer* _extensionContainer;

	};

	// SM_RP_DA
	class SM_RP_DA : public Choice {
	public:
		SM_RP_DA();
		~SM_RP_DA();
		// nodes
		IMSI* _imsi;
		LMSI* _lmsi;
		AddressString* _serviceCentreAddressDA;
		Null* _noSM_RP_DA;

	};

	// SM_RP_OA
	class SM_RP_OA : public Choice {
	public:
		SM_RP_OA();
		~SM_RP_OA();
		// nodes
		ISDN_AddressString* _msisdn;
		AddressString* _serviceCentreAddressOA;
		Null* _noSM_RP_OA;

	};

	// SM_DeliveryTimerValue
	class SM_DeliveryTimerValue : public Integer {
	public:
		SM_DeliveryTimerValue();
		~SM_DeliveryTimerValue();

	};

	// ReportSM_DeliveryStatusArg
	class ReportSM_DeliveryStatusArg : public Sequence {
	public:
		ReportSM_DeliveryStatusArg();
		~ReportSM_DeliveryStatusArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_absentSubscriberDiagnosticSM();
		void set_extensionContainer();
		void set_gprsSupportIndicator();
		void set_deliveryOutcomeIndicator();
		void set_additionalSM_DeliveryOutcome();
		void set_additionalAbsentSubscriberDiagnosticSM();
		void set_ip_sm_gw_Indicator();
		void set_ip_sm_gw_sm_deliveryOutcome();
		void set_ip_sm_gw_absentSubscriberDiagnosticSM();
		void set_imsi();
		// nodes
		ISDN_AddressString* _msisdn;
		AddressString* _serviceCentreAddress;
		SM_DeliveryOutcome* _sm_DeliveryOutcome;
		AbsentSubscriberDiagnosticSM* _absentSubscriberDiagnosticSM;
		ExtensionContainer* _extensionContainer;
		Null* _gprsSupportIndicator;
		Null* _deliveryOutcomeIndicator;
		SM_DeliveryOutcome* _additionalSM_DeliveryOutcome;
		AbsentSubscriberDiagnosticSM* _additionalAbsentSubscriberDiagnosticSM;
		Null* _ip_sm_gw_Indicator;
		SM_DeliveryOutcome* _ip_sm_gw_sm_deliveryOutcome;
		AbsentSubscriberDiagnosticSM* _ip_sm_gw_absentSubscriberDiagnosticSM;
		IMSI* _imsi;

	};

	// SM_DeliveryOutcome
	class SM_DeliveryOutcome : public Enumerated {
	public:
		SM_DeliveryOutcome();
		~SM_DeliveryOutcome();
		static const int _memoryCapacityExceeded = 0;
		static const int _absentSubscriber = 1;
		static const int _successfulTransfer = 2;
	};

	// ReportSM_DeliveryStatusRes
	class ReportSM_DeliveryStatusRes : public Sequence {
	public:
		ReportSM_DeliveryStatusRes();
		~ReportSM_DeliveryStatusRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_storedMSISDN();
		void set_extensionContainer();
		// nodes
		ISDN_AddressString* _storedMSISDN;
		ExtensionContainer* _extensionContainer;

	};

	// AlertServiceCentreArg
	class AlertServiceCentreArg : public Sequence {
	public:
		AlertServiceCentreArg();
		~AlertServiceCentreArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_imsi();
		// nodes
		ISDN_AddressString* _msisdn;
		AddressString* _serviceCentreAddress;
		IMSI* _imsi;

	};

	// InformServiceCentreArg
	class InformServiceCentreArg : public Sequence {
	public:
		InformServiceCentreArg();
		~InformServiceCentreArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_storedMSISDN();
		void set_mw_Status();
		void set_extensionContainer();
		void set_absentSubscriberDiagnosticSM();
		void set_additionalAbsentSubscriberDiagnosticSM();
		// nodes
		ISDN_AddressString* _storedMSISDN;
		MW_Status* _mw_Status;
		ExtensionContainer* _extensionContainer;
		AbsentSubscriberDiagnosticSM* _absentSubscriberDiagnosticSM;
		AbsentSubscriberDiagnosticSM* _additionalAbsentSubscriberDiagnosticSM;

	};

	// MW_Status
	class MW_Status : public Bit_string {
	public:
		MW_Status();
		~MW_Status();

	};

	// ReadyForSM_Arg
	class ReadyForSM_Arg : public Sequence {
	public:
		ReadyForSM_Arg();
		~ReadyForSM_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_alertReasonIndicator();
		void set_extensionContainer();
		void set_additionalAlertReasonIndicator();
		// nodes
		IMSI* _imsi;
		AlertReason* _alertReason;
		Null* _alertReasonIndicator;
		ExtensionContainer* _extensionContainer;
		Null* _additionalAlertReasonIndicator;

	};

	// ReadyForSM_Res
	class ReadyForSM_Res : public Sequence {
	public:
		ReadyForSM_Res();
		~ReadyForSM_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// AlertReason
	class AlertReason : public Enumerated {
	public:
		AlertReason();
		~AlertReason();
		static const int _ms_Present = 0;
		static const int _memoryAvailable = 1;
	};

	// MT_ForwardSM_VGCS_Arg
	class MT_ForwardSM_VGCS_Arg : public Sequence {
	public:
		MT_ForwardSM_VGCS_Arg();
		~MT_ForwardSM_VGCS_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ASCI_CallReference* _asciCallReference;
		SM_RP_OA* _sm_RP_OA;
		SignalInfo* _sm_RP_UI;
		ExtensionContainer* _extensionContainer;

	};

	// MT_ForwardSM_VGCS_Res
	class MT_ForwardSM_VGCS_Res : public Sequence {
	public:
		MT_ForwardSM_VGCS_Res();
		~MT_ForwardSM_VGCS_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_sm_RP_UI();
		void set_dispatcherList();
		void set_ongoingCall();
		void set_extensionContainer();
		void set_additionalDispatcherList();
		// nodes
		SignalInfo* _sm_RP_UI;
		DispatcherList* _dispatcherList;
		Null* _ongoingCall;
		ExtensionContainer* _extensionContainer;
		AdditionalDispatcherList* _additionalDispatcherList;

	};

	// DispatcherList
	class DispatcherList : public Sequence_of {
	public:
		DispatcherList();
		~DispatcherList();
		// nodes
		ISDN_AddressString* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfDispatchers
	class maxNumOfDispatchers : public Integer {
	public:
		maxNumOfDispatchers();
		~maxNumOfDispatchers();

	};

	// AdditionalDispatcherList
	class AdditionalDispatcherList : public Sequence_of {
	public:
		AdditionalDispatcherList();
		~AdditionalDispatcherList();
		// nodes
		ISDN_AddressString* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfAdditionalDispatchers
	class maxNumOfAdditionalDispatchers : public Integer {
	public:
		maxNumOfAdditionalDispatchers();
		~maxNumOfAdditionalDispatchers();

	};

}
#endif
