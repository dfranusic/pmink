#ifndef MAP_COMMONDATATYPES_H_
#define MAP_COMMONDATATYPES_H_

#include<asn1.h>
#include<MAP-SS-Code.h>
#include<MAP-TS-Code.h>
#include<MAP-BS-Code.h>
#include<MAP-ExtensionDataTypes.h>

namespace asn1 {
	// forward declarations
	class TBCD_STRING;
	class DiameterIdentity;
	class AddressString;
	class maxAddressLength;
	class ISDN_AddressString;
	class maxISDN_AddressLength;
	class FTN_AddressString;
	class maxFTN_AddressLength;
	class ISDN_SubaddressString;
	class maxISDN_SubaddressLength;
	class ExternalSignalInfo;
	class SignalInfo;
	class maxSignalInfoLength;
	class ProtocolId;
	class Ext_ExternalSignalInfo;
	class Ext_ProtocolId;
	class AccessNetworkSignalInfo;
	class LongSignalInfo;
	class maxLongSignalInfoLength;
	class AccessNetworkProtocolId;
	class AlertingPattern;
	class alertingLevel_0;
	class alertingLevel_1;
	class alertingLevel_2;
	class alertingCategory_1;
	class alertingCategory_2;
	class alertingCategory_3;
	class alertingCategory_4;
	class alertingCategory_5;
	class GSN_Address;
	class Time;
	class IMSI;
	class Identity;
	class IMSI_WithLMSI;
	class ASCI_CallReference;
	class TMSI;
	class SubscriberId;
	class IMEI;
	class HLR_Id;
	class HLR_List;
	class maxNumOfHLR_Id;
	class LMSI;
	class GlobalCellId;
	class NetworkResource;
	class AdditionalNetworkResource;
	class NAEA_PreferredCI;
	class NAEA_CIC;
	class SubscriberIdentity;
	class LCSClientExternalID;
	class LCSClientInternalID;
	class LCSServiceTypeID;
	class emergencyServices;
	class emergencyAlertServices;
	class personTracking;
	class fleetManagement;
	class assetManagement;
	class trafficCongestionReporting;
	class roadsideAssistance;
	class routingToNearestCommercialEnterprise;
	class navigation;
	class citySightseeing;
	class localizedAdvertising;
	class mobileYellowPages;
	class trafficAndPublicTransportationInfo;
	class weather;
	class assetAndServiceFinding;
	class gaming;
	class findYourFriend;
	class dating;
	class chatting;
	class routeFinding;
	class whereAmI;
	class serv64;
	class serv65;
	class serv66;
	class serv67;
	class serv68;
	class serv69;
	class serv70;
	class serv71;
	class serv72;
	class serv73;
	class serv74;
	class serv75;
	class serv76;
	class serv77;
	class serv78;
	class serv79;
	class serv80;
	class serv81;
	class serv82;
	class serv83;
	class serv84;
	class serv85;
	class serv86;
	class serv87;
	class serv88;
	class serv89;
	class serv90;
	class serv91;
	class serv92;
	class serv93;
	class serv94;
	class serv95;
	class serv96;
	class serv97;
	class serv98;
	class serv99;
	class serv100;
	class serv101;
	class serv102;
	class serv103;
	class serv104;
	class serv105;
	class serv106;
	class serv107;
	class serv108;
	class serv109;
	class serv110;
	class serv111;
	class serv112;
	class serv113;
	class serv114;
	class serv115;
	class serv116;
	class serv117;
	class serv118;
	class serv119;
	class serv120;
	class serv121;
	class serv122;
	class serv123;
	class serv124;
	class serv125;
	class serv126;
	class serv127;
	class PLMN_Id;
	class E_UTRAN_CGI;
	class TA_Id;
	class RAIdentity;
	class NetworkNodeDiameterAddress;
	class CellGlobalIdOrServiceAreaIdOrLAI;
	class CellGlobalIdOrServiceAreaIdFixedLength;
	class LAIFixedLength;
	class BasicServiceCode;
	class Ext_BasicServiceCode;
	class EMLPP_Info;
	class EMLPP_Priority;
	class priorityLevelA;
	class priorityLevelB;
	class priorityLevel0;
	class priorityLevel1;
	class priorityLevel2;
	class priorityLevel3;
	class priorityLevel4;
	class MC_SS_Info;
	class MaxMC_Bearers;
	class MC_Bearers;
	class maxNumOfMC_Bearers;
	class Ext_SS_Status;
	class AgeOfLocationInformation;

	

	// TBCD_STRING
	class TBCD_STRING : public Octet_string {
	public:
		TBCD_STRING();
		~TBCD_STRING();

	};

	// DiameterIdentity
	class DiameterIdentity : public Octet_string {
	public:
		DiameterIdentity();
		~DiameterIdentity();

	};

	// AddressString
	class AddressString : public Octet_string {
	public:
		AddressString();
		~AddressString();

	};

	// maxAddressLength
	class maxAddressLength : public Integer {
	public:
		maxAddressLength();
		~maxAddressLength();

	};

	// ISDN_AddressString
	class ISDN_AddressString : public AddressString {
	public:
		ISDN_AddressString();
		~ISDN_AddressString();

	};

	// maxISDN_AddressLength
	class maxISDN_AddressLength : public Integer {
	public:
		maxISDN_AddressLength();
		~maxISDN_AddressLength();

	};

	// FTN_AddressString
	class FTN_AddressString : public AddressString {
	public:
		FTN_AddressString();
		~FTN_AddressString();

	};

	// maxFTN_AddressLength
	class maxFTN_AddressLength : public Integer {
	public:
		maxFTN_AddressLength();
		~maxFTN_AddressLength();

	};

	// ISDN_SubaddressString
	class ISDN_SubaddressString : public Octet_string {
	public:
		ISDN_SubaddressString();
		~ISDN_SubaddressString();

	};

	// maxISDN_SubaddressLength
	class maxISDN_SubaddressLength : public Integer {
	public:
		maxISDN_SubaddressLength();
		~maxISDN_SubaddressLength();

	};

	// ExternalSignalInfo
	class ExternalSignalInfo : public Sequence {
	public:
		ExternalSignalInfo();
		~ExternalSignalInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ProtocolId* _protocolId;
		SignalInfo* _signalInfo;
		ExtensionContainer* _extensionContainer;

	};

	// SignalInfo
	class SignalInfo : public Octet_string {
	public:
		SignalInfo();
		~SignalInfo();

	};

	// maxSignalInfoLength
	class maxSignalInfoLength : public Integer {
	public:
		maxSignalInfoLength();
		~maxSignalInfoLength();

	};

	// ProtocolId
	class ProtocolId : public Enumerated {
	public:
		ProtocolId();
		~ProtocolId();
		static const int _gsm_0408 = 1;
		static const int _gsm_0806 = 2;
		static const int _gsm_BSSMAP = 3;
		static const int _ets_300102_1 = 4;

	};

	// Ext_ExternalSignalInfo
	class Ext_ExternalSignalInfo : public Sequence {
	public:
		Ext_ExternalSignalInfo();
		~Ext_ExternalSignalInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		Ext_ProtocolId* _ext_ProtocolId;
		SignalInfo* _signalInfo;
		ExtensionContainer* _extensionContainer;

	};

	// Ext_ProtocolId
	class Ext_ProtocolId : public Enumerated {
	public:
		Ext_ProtocolId();
		~Ext_ProtocolId();
		static const int _ets_300356 = 1;
	};

	// AccessNetworkSignalInfo
	class AccessNetworkSignalInfo : public Sequence {
	public:
		AccessNetworkSignalInfo();
		~AccessNetworkSignalInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		AccessNetworkProtocolId* _accessNetworkProtocolId;
		LongSignalInfo* _signalInfo;
		ExtensionContainer* _extensionContainer;

	};

	// LongSignalInfo
	class LongSignalInfo : public Octet_string {
	public:
		LongSignalInfo();
		~LongSignalInfo();

	};

	// maxLongSignalInfoLength
	class maxLongSignalInfoLength : public Integer {
	public:
		maxLongSignalInfoLength();
		~maxLongSignalInfoLength();

	};

	// AccessNetworkProtocolId
	class AccessNetworkProtocolId : public Enumerated {
	public:
		AccessNetworkProtocolId();
		~AccessNetworkProtocolId();
		static const int _ts3G_48006 = 1;
		static const int _ts3G_25413 = 2;
	};

	// AlertingPattern
	class AlertingPattern : public Octet_string {
	public:
		AlertingPattern();
		~AlertingPattern();

	};

	// alertingLevel_0
	class alertingLevel_0 : public AlertingPattern {
	public:
		alertingLevel_0();
		~alertingLevel_0();

	};

	// alertingLevel_1
	class alertingLevel_1 : public AlertingPattern {
	public:
		alertingLevel_1();
		~alertingLevel_1();

	};

	// alertingLevel_2
	class alertingLevel_2 : public AlertingPattern {
	public:
		alertingLevel_2();
		~alertingLevel_2();

	};

	// alertingCategory_1
	class alertingCategory_1 : public AlertingPattern {
	public:
		alertingCategory_1();
		~alertingCategory_1();

	};

	// alertingCategory_2
	class alertingCategory_2 : public AlertingPattern {
	public:
		alertingCategory_2();
		~alertingCategory_2();

	};

	// alertingCategory_3
	class alertingCategory_3 : public AlertingPattern {
	public:
		alertingCategory_3();
		~alertingCategory_3();

	};

	// alertingCategory_4
	class alertingCategory_4 : public AlertingPattern {
	public:
		alertingCategory_4();
		~alertingCategory_4();

	};

	// alertingCategory_5
	class alertingCategory_5 : public AlertingPattern {
	public:
		alertingCategory_5();
		~alertingCategory_5();

	};

	// GSN_Address
	class GSN_Address : public Octet_string {
	public:
		GSN_Address();
		~GSN_Address();

	};

	// Time
	class Time : public Octet_string {
	public:
		Time();
		~Time();

	};

	// IMSI
	class IMSI : public TBCD_STRING {
	public:
		IMSI();
		~IMSI();

	};

	// Identity
	class Identity : public Choice {
	public:
		Identity();
		~Identity();
		// nodes
		IMSI* _imsi;
		IMSI_WithLMSI* _imsi_WithLMSI;

	};

	// IMSI_WithLMSI
	class IMSI_WithLMSI : public Sequence {
	public:
		IMSI_WithLMSI();
		~IMSI_WithLMSI();
		// nodes
		IMSI* _imsi;
		LMSI* _lmsi;

	};

	// ASCI_CallReference
	class ASCI_CallReference : public TBCD_STRING {
	public:
		ASCI_CallReference();
		~ASCI_CallReference();

	};

	// TMSI
	class TMSI : public Octet_string {
	public:
		TMSI();
		~TMSI();

	};

	// SubscriberId
	class SubscriberId : public Choice {
	public:
		SubscriberId();
		~SubscriberId();
		// nodes
		IMSI* _imsi;
		TMSI* _tmsi;

	};

	// IMEI
	class IMEI : public TBCD_STRING {
	public:
		IMEI();
		~IMEI();

	};

	// HLR_Id
	class HLR_Id : public IMSI {
	public:
		HLR_Id();
		~HLR_Id();

	};

	// HLR_List
	class HLR_List : public Sequence_of {
	public:
		HLR_List();
		~HLR_List();
		// nodes
		HLR_Id* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfHLR_Id
	class maxNumOfHLR_Id : public Integer {
	public:
		maxNumOfHLR_Id();
		~maxNumOfHLR_Id();

	};

	// LMSI
	class LMSI : public Octet_string {
	public:
		LMSI();
		~LMSI();

	};

	// GlobalCellId
	class GlobalCellId : public Octet_string {
	public:
		GlobalCellId();
		~GlobalCellId();

	};

	// NetworkResource
	class NetworkResource : public Enumerated {
	public:
		NetworkResource();
		~NetworkResource();
		static const int _plmn = 0;
		static const int _hlr = 1;
		static const int _vlr = 2;
		static const int _pvlr = 3;
		static const int _controllingMSC = 4;
		static const int _vmsc = 5;
		static const int _eir = 6;
		static const int _rss = 7;

	};

	// AdditionalNetworkResource
	class AdditionalNetworkResource : public Enumerated {
	public:
		AdditionalNetworkResource();
		~AdditionalNetworkResource();
		static const int _sgsn = 0;
		static const int _ggsn = 1;
		static const int _gmlc = 2;
		static const int _gsmSCF = 3;
		static const int _nplr = 4;
		static const int _auc = 5;
		static const int _ue = 6;
		static const int _mme = 7;

	};

	// NAEA_PreferredCI
	class NAEA_PreferredCI : public Sequence {
	public:
		NAEA_PreferredCI();
		~NAEA_PreferredCI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		NAEA_CIC* _naea_PreferredCIC;
		ExtensionContainer* _extensionContainer;

	};

	// NAEA_CIC
	class NAEA_CIC : public Octet_string {
	public:
		NAEA_CIC();
		~NAEA_CIC();

	};

	// SubscriberIdentity
	class SubscriberIdentity : public Choice {
	public:
		SubscriberIdentity();
		~SubscriberIdentity();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _msisdn;

	};

	// LCSClientExternalID
	class LCSClientExternalID : public Sequence {
	public:
		LCSClientExternalID();
		~LCSClientExternalID();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_externalAddress();
		void set_extensionContainer();
		// nodes
		ISDN_AddressString* _externalAddress;
		ExtensionContainer* _extensionContainer;

	};

	// LCSClientInternalID
	class LCSClientInternalID : public Enumerated {
	public:
		LCSClientInternalID();
		~LCSClientInternalID();
		static const int _broadcastService = 0;
		static const int _o_andM_HPLMN = 1;
		static const int _o_andM_VPLMN = 2;
		static const int _anonymousLocation = 3;
		static const int _targetMSsubscribedService = 4;

	};

	// LCSServiceTypeID
	class LCSServiceTypeID : public Integer {
	public:
		LCSServiceTypeID();
		~LCSServiceTypeID();

	};

	// emergencyServices
	class emergencyServices : public LCSServiceTypeID {
	public:
		emergencyServices();
		~emergencyServices();

	};

	// emergencyAlertServices
	class emergencyAlertServices : public LCSServiceTypeID {
	public:
		emergencyAlertServices();
		~emergencyAlertServices();

	};

	// personTracking
	class personTracking : public LCSServiceTypeID {
	public:
		personTracking();
		~personTracking();

	};

	// fleetManagement
	class fleetManagement : public LCSServiceTypeID {
	public:
		fleetManagement();
		~fleetManagement();

	};

	// assetManagement
	class assetManagement : public LCSServiceTypeID {
	public:
		assetManagement();
		~assetManagement();

	};

	// trafficCongestionReporting
	class trafficCongestionReporting : public LCSServiceTypeID {
	public:
		trafficCongestionReporting();
		~trafficCongestionReporting();

	};

	// roadsideAssistance
	class roadsideAssistance : public LCSServiceTypeID {
	public:
		roadsideAssistance();
		~roadsideAssistance();

	};

	// routingToNearestCommercialEnterprise
	class routingToNearestCommercialEnterprise : public LCSServiceTypeID {
	public:
		routingToNearestCommercialEnterprise();
		~routingToNearestCommercialEnterprise();

	};

	// navigation
	class navigation : public LCSServiceTypeID {
	public:
		navigation();
		~navigation();

	};

	// citySightseeing
	class citySightseeing : public LCSServiceTypeID {
	public:
		citySightseeing();
		~citySightseeing();

	};

	// localizedAdvertising
	class localizedAdvertising : public LCSServiceTypeID {
	public:
		localizedAdvertising();
		~localizedAdvertising();

	};

	// mobileYellowPages
	class mobileYellowPages : public LCSServiceTypeID {
	public:
		mobileYellowPages();
		~mobileYellowPages();

	};

	// trafficAndPublicTransportationInfo
	class trafficAndPublicTransportationInfo : public LCSServiceTypeID {
	public:
		trafficAndPublicTransportationInfo();
		~trafficAndPublicTransportationInfo();

	};

	// weather
	class weather : public LCSServiceTypeID {
	public:
		weather();
		~weather();

	};

	// assetAndServiceFinding
	class assetAndServiceFinding : public LCSServiceTypeID {
	public:
		assetAndServiceFinding();
		~assetAndServiceFinding();

	};

	// gaming
	class gaming : public LCSServiceTypeID {
	public:
		gaming();
		~gaming();

	};

	// findYourFriend
	class findYourFriend : public LCSServiceTypeID {
	public:
		findYourFriend();
		~findYourFriend();

	};

	// dating
	class dating : public LCSServiceTypeID {
	public:
		dating();
		~dating();

	};

	// chatting
	class chatting : public LCSServiceTypeID {
	public:
		chatting();
		~chatting();

	};

	// routeFinding
	class routeFinding : public LCSServiceTypeID {
	public:
		routeFinding();
		~routeFinding();

	};

	// whereAmI
	class whereAmI : public LCSServiceTypeID {
	public:
		whereAmI();
		~whereAmI();

	};

	// serv64
	class serv64 : public LCSServiceTypeID {
	public:
		serv64();
		~serv64();

	};

	// serv65
	class serv65 : public LCSServiceTypeID {
	public:
		serv65();
		~serv65();

	};

	// serv66
	class serv66 : public LCSServiceTypeID {
	public:
		serv66();
		~serv66();

	};

	// serv67
	class serv67 : public LCSServiceTypeID {
	public:
		serv67();
		~serv67();

	};

	// serv68
	class serv68 : public LCSServiceTypeID {
	public:
		serv68();
		~serv68();

	};

	// serv69
	class serv69 : public LCSServiceTypeID {
	public:
		serv69();
		~serv69();

	};

	// serv70
	class serv70 : public LCSServiceTypeID {
	public:
		serv70();
		~serv70();

	};

	// serv71
	class serv71 : public LCSServiceTypeID {
	public:
		serv71();
		~serv71();

	};

	// serv72
	class serv72 : public LCSServiceTypeID {
	public:
		serv72();
		~serv72();

	};

	// serv73
	class serv73 : public LCSServiceTypeID {
	public:
		serv73();
		~serv73();

	};

	// serv74
	class serv74 : public LCSServiceTypeID {
	public:
		serv74();
		~serv74();

	};

	// serv75
	class serv75 : public LCSServiceTypeID {
	public:
		serv75();
		~serv75();

	};

	// serv76
	class serv76 : public LCSServiceTypeID {
	public:
		serv76();
		~serv76();

	};

	// serv77
	class serv77 : public LCSServiceTypeID {
	public:
		serv77();
		~serv77();

	};

	// serv78
	class serv78 : public LCSServiceTypeID {
	public:
		serv78();
		~serv78();

	};

	// serv79
	class serv79 : public LCSServiceTypeID {
	public:
		serv79();
		~serv79();

	};

	// serv80
	class serv80 : public LCSServiceTypeID {
	public:
		serv80();
		~serv80();

	};

	// serv81
	class serv81 : public LCSServiceTypeID {
	public:
		serv81();
		~serv81();

	};

	// serv82
	class serv82 : public LCSServiceTypeID {
	public:
		serv82();
		~serv82();

	};

	// serv83
	class serv83 : public LCSServiceTypeID {
	public:
		serv83();
		~serv83();

	};

	// serv84
	class serv84 : public LCSServiceTypeID {
	public:
		serv84();
		~serv84();

	};

	// serv85
	class serv85 : public LCSServiceTypeID {
	public:
		serv85();
		~serv85();

	};

	// serv86
	class serv86 : public LCSServiceTypeID {
	public:
		serv86();
		~serv86();

	};

	// serv87
	class serv87 : public LCSServiceTypeID {
	public:
		serv87();
		~serv87();

	};

	// serv88
	class serv88 : public LCSServiceTypeID {
	public:
		serv88();
		~serv88();

	};

	// serv89
	class serv89 : public LCSServiceTypeID {
	public:
		serv89();
		~serv89();

	};

	// serv90
	class serv90 : public LCSServiceTypeID {
	public:
		serv90();
		~serv90();

	};

	// serv91
	class serv91 : public LCSServiceTypeID {
	public:
		serv91();
		~serv91();

	};

	// serv92
	class serv92 : public LCSServiceTypeID {
	public:
		serv92();
		~serv92();

	};

	// serv93
	class serv93 : public LCSServiceTypeID {
	public:
		serv93();
		~serv93();

	};

	// serv94
	class serv94 : public LCSServiceTypeID {
	public:
		serv94();
		~serv94();

	};

	// serv95
	class serv95 : public LCSServiceTypeID {
	public:
		serv95();
		~serv95();

	};

	// serv96
	class serv96 : public LCSServiceTypeID {
	public:
		serv96();
		~serv96();

	};

	// serv97
	class serv97 : public LCSServiceTypeID {
	public:
		serv97();
		~serv97();

	};

	// serv98
	class serv98 : public LCSServiceTypeID {
	public:
		serv98();
		~serv98();

	};

	// serv99
	class serv99 : public LCSServiceTypeID {
	public:
		serv99();
		~serv99();

	};

	// serv100
	class serv100 : public LCSServiceTypeID {
	public:
		serv100();
		~serv100();

	};

	// serv101
	class serv101 : public LCSServiceTypeID {
	public:
		serv101();
		~serv101();

	};

	// serv102
	class serv102 : public LCSServiceTypeID {
	public:
		serv102();
		~serv102();

	};

	// serv103
	class serv103 : public LCSServiceTypeID {
	public:
		serv103();
		~serv103();

	};

	// serv104
	class serv104 : public LCSServiceTypeID {
	public:
		serv104();
		~serv104();

	};

	// serv105
	class serv105 : public LCSServiceTypeID {
	public:
		serv105();
		~serv105();

	};

	// serv106
	class serv106 : public LCSServiceTypeID {
	public:
		serv106();
		~serv106();

	};

	// serv107
	class serv107 : public LCSServiceTypeID {
	public:
		serv107();
		~serv107();

	};

	// serv108
	class serv108 : public LCSServiceTypeID {
	public:
		serv108();
		~serv108();

	};

	// serv109
	class serv109 : public LCSServiceTypeID {
	public:
		serv109();
		~serv109();

	};

	// serv110
	class serv110 : public LCSServiceTypeID {
	public:
		serv110();
		~serv110();

	};

	// serv111
	class serv111 : public LCSServiceTypeID {
	public:
		serv111();
		~serv111();

	};

	// serv112
	class serv112 : public LCSServiceTypeID {
	public:
		serv112();
		~serv112();

	};

	// serv113
	class serv113 : public LCSServiceTypeID {
	public:
		serv113();
		~serv113();

	};

	// serv114
	class serv114 : public LCSServiceTypeID {
	public:
		serv114();
		~serv114();

	};

	// serv115
	class serv115 : public LCSServiceTypeID {
	public:
		serv115();
		~serv115();

	};

	// serv116
	class serv116 : public LCSServiceTypeID {
	public:
		serv116();
		~serv116();

	};

	// serv117
	class serv117 : public LCSServiceTypeID {
	public:
		serv117();
		~serv117();

	};

	// serv118
	class serv118 : public LCSServiceTypeID {
	public:
		serv118();
		~serv118();

	};

	// serv119
	class serv119 : public LCSServiceTypeID {
	public:
		serv119();
		~serv119();

	};

	// serv120
	class serv120 : public LCSServiceTypeID {
	public:
		serv120();
		~serv120();

	};

	// serv121
	class serv121 : public LCSServiceTypeID {
	public:
		serv121();
		~serv121();

	};

	// serv122
	class serv122 : public LCSServiceTypeID {
	public:
		serv122();
		~serv122();

	};

	// serv123
	class serv123 : public LCSServiceTypeID {
	public:
		serv123();
		~serv123();

	};

	// serv124
	class serv124 : public LCSServiceTypeID {
	public:
		serv124();
		~serv124();

	};

	// serv125
	class serv125 : public LCSServiceTypeID {
	public:
		serv125();
		~serv125();

	};

	// serv126
	class serv126 : public LCSServiceTypeID {
	public:
		serv126();
		~serv126();

	};

	// serv127
	class serv127 : public LCSServiceTypeID {
	public:
		serv127();
		~serv127();

	};

	// PLMN_Id
	class PLMN_Id : public Octet_string {
	public:
		PLMN_Id();
		~PLMN_Id();

	};

	// E_UTRAN_CGI
	class E_UTRAN_CGI : public Octet_string {
	public:
		E_UTRAN_CGI();
		~E_UTRAN_CGI();

	};

	// TA_Id
	class TA_Id : public Octet_string {
	public:
		TA_Id();
		~TA_Id();

	};

	// RAIdentity
	class RAIdentity : public Octet_string {
	public:
		RAIdentity();
		~RAIdentity();

	};

	// NetworkNodeDiameterAddress
	class NetworkNodeDiameterAddress : public Sequence {
	public:
		NetworkNodeDiameterAddress();
		~NetworkNodeDiameterAddress();
		// nodes
		DiameterIdentity* _diameter_Name;
		DiameterIdentity* _diameter_Realm;

	};

	// CellGlobalIdOrServiceAreaIdOrLAI
	class CellGlobalIdOrServiceAreaIdOrLAI : public Choice {
	public:
		CellGlobalIdOrServiceAreaIdOrLAI();
		~CellGlobalIdOrServiceAreaIdOrLAI();
		// nodes
		CellGlobalIdOrServiceAreaIdFixedLength* _cellGlobalIdOrServiceAreaIdFixedLength;
		LAIFixedLength* _laiFixedLength;

	};

	// CellGlobalIdOrServiceAreaIdFixedLength
	class CellGlobalIdOrServiceAreaIdFixedLength : public Octet_string {
	public:
		CellGlobalIdOrServiceAreaIdFixedLength();
		~CellGlobalIdOrServiceAreaIdFixedLength();

	};

	// LAIFixedLength
	class LAIFixedLength : public Octet_string {
	public:
		LAIFixedLength();
		~LAIFixedLength();

	};

	// BasicServiceCode
	class BasicServiceCode : public Choice {
	public:
		BasicServiceCode();
		~BasicServiceCode();
		// nodes
		BearerServiceCode* _bearerService;
		TeleserviceCode* _teleservice;

	};

	// Ext_BasicServiceCode
	class Ext_BasicServiceCode : public Choice {
	public:
		Ext_BasicServiceCode();
		~Ext_BasicServiceCode();
		// nodes
		Ext_BearerServiceCode* _ext_BearerService;
		Ext_TeleserviceCode* _ext_Teleservice;

	};

	// EMLPP_Info
	class EMLPP_Info : public Sequence {
	public:
		EMLPP_Info();
		~EMLPP_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		EMLPP_Priority* _maximumentitledPriority;
		EMLPP_Priority* _defaultPriority;
		ExtensionContainer* _extensionContainer;

	};

	// EMLPP_Priority
	class EMLPP_Priority : public Integer {
	public:
		EMLPP_Priority();
		~EMLPP_Priority();

	};

	// priorityLevelA
	class priorityLevelA : public EMLPP_Priority {
	public:
		priorityLevelA();
		~priorityLevelA();

	};

	// priorityLevelB
	class priorityLevelB : public EMLPP_Priority {
	public:
		priorityLevelB();
		~priorityLevelB();

	};

	// priorityLevel0
	class priorityLevel0 : public EMLPP_Priority {
	public:
		priorityLevel0();
		~priorityLevel0();

	};

	// priorityLevel1
	class priorityLevel1 : public EMLPP_Priority {
	public:
		priorityLevel1();
		~priorityLevel1();

	};

	// priorityLevel2
	class priorityLevel2 : public EMLPP_Priority {
	public:
		priorityLevel2();
		~priorityLevel2();

	};

	// priorityLevel3
	class priorityLevel3 : public EMLPP_Priority {
	public:
		priorityLevel3();
		~priorityLevel3();

	};

	// priorityLevel4
	class priorityLevel4 : public EMLPP_Priority {
	public:
		priorityLevel4();
		~priorityLevel4();

	};

	// MC_SS_Info
	class MC_SS_Info : public Sequence {
	public:
		MC_SS_Info();
		~MC_SS_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SS_Code* _ss_Code;
		Ext_SS_Status* _ss_Status;
		MaxMC_Bearers* _nbrSB;
		MC_Bearers* _nbrUser;
		ExtensionContainer* _extensionContainer;

	};

	// MaxMC_Bearers
	class MaxMC_Bearers : public Integer {
	public:
		MaxMC_Bearers();
		~MaxMC_Bearers();

	};

	// MC_Bearers
	class MC_Bearers : public Integer {
	public:
		MC_Bearers();
		~MC_Bearers();

	};

	// maxNumOfMC_Bearers
	class maxNumOfMC_Bearers : public Integer {
	public:
		maxNumOfMC_Bearers();
		~maxNumOfMC_Bearers();

	};

	// Ext_SS_Status
	class Ext_SS_Status : public Octet_string {
	public:
		Ext_SS_Status();
		~Ext_SS_Status();

	};

	// AgeOfLocationInformation
	class AgeOfLocationInformation : public Integer {
	public:
		AgeOfLocationInformation();
		~AgeOfLocationInformation();

	};

}
#endif
