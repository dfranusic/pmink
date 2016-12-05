#ifndef MAP_SS_DATATYPES_H_
#define MAP_SS_DATATYPES_H_

#include<asn1.h>
#include<MAP-SS-Code.h>
#include<MAP-CommonDataTypes.h>

namespace asn1 {
	// forward declarations
	class RegisterSS_Arg;
	class NoReplyConditionTime;
	class SS_Info;
	class ForwardingInfo;
	class ForwardingFeatureList;
	class ForwardingFeature;
	class SS_Status;
	class ForwardingOptions;
	class CallBarringInfo;
	class CallBarringFeatureList;
	class CallBarringFeature;
	class SS_Data;
	class SS_SubscriptionOption;
	class CliRestrictionOption;
	class OverrideCategory;
	class SS_ForBS_Code;
	class GenericServiceInfo;
	class CCBS_FeatureList;
	class maxNumOfCCBS_Requests;
	class CCBS_Feature;
	class CCBS_Index;
	class InterrogateSS_Res;
	class USSD_Arg;
	class USSD_Res;
	class USSD_DataCodingScheme;
	class USSD_String;
	class maxUSSD_StringLength;
	class Password;
	class GuidanceInfo;
	class SS_List;
	class maxNumOfSS;
	class SS_InfoList;
	class BasicServiceGroupList;
	class maxNumOfBasicServiceGroups;
	class SS_InvocationNotificationArg;
	class CCBS_RequestState;
	class SS_InvocationNotificationRes;
	class SS_EventSpecification;
	class maxEventSpecification;
	class RegisterCC_EntryArg;
	class CCBS_Data;
	class ServiceIndicator;
	class RegisterCC_EntryRes;
	class EraseCC_EntryArg;
	class EraseCC_EntryRes;

	

	// RegisterSS_Arg
	class RegisterSS_Arg : public Sequence {
	public:
		RegisterSS_Arg();
		~RegisterSS_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_forwardedToNumber();
		void set_forwardedToSubaddress();
		void set_noReplyConditionTime();
		void set_defaultPriority();
		void set_nbrUser();
		void set_longFTN_Supported();
		// nodes
		SS_Code* _ss_Code;
		BasicServiceCode* _basicService;
		AddressString* _forwardedToNumber;
		ISDN_SubaddressString* _forwardedToSubaddress;
		NoReplyConditionTime* _noReplyConditionTime;
		EMLPP_Priority* _defaultPriority;
		MC_Bearers* _nbrUser;
		Null* _longFTN_Supported;

	};

	// NoReplyConditionTime
	class NoReplyConditionTime : public Integer {
	public:
		NoReplyConditionTime();
		~NoReplyConditionTime();

	};

	// SS_Info
	class SS_Info : public Choice {
	public:
		SS_Info();
		~SS_Info();
		// nodes
		ForwardingInfo* _forwardingInfo;
		CallBarringInfo* _callBarringInfo;
		SS_Data* _ss_Data;

	};

	// ForwardingInfo
	class ForwardingInfo : public Sequence {
	public:
		ForwardingInfo();
		~ForwardingInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_Code();
		// nodes
		SS_Code* _ss_Code;
		ForwardingFeatureList* _forwardingFeatureList;

	};

	// ForwardingFeatureList
	class ForwardingFeatureList : public Sequence_of {
	public:
		ForwardingFeatureList();
		~ForwardingFeatureList();
		// nodes
		ForwardingFeature* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// ForwardingFeature
	class ForwardingFeature : public Sequence {
	public:
		ForwardingFeature();
		~ForwardingFeature();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_ss_Status();
		void set_forwardedToNumber();
		void set_forwardedToSubaddress();
		void set_forwardingOptions();
		void set_noReplyConditionTime();
		void set_longForwardedToNumber();
		// nodes
		BasicServiceCode* _basicService;
		SS_Status* _ss_Status;
		ISDN_AddressString* _forwardedToNumber;
		ISDN_SubaddressString* _forwardedToSubaddress;
		ForwardingOptions* _forwardingOptions;
		NoReplyConditionTime* _noReplyConditionTime;
		FTN_AddressString* _longForwardedToNumber;

	};

	// SS_Status
	class SS_Status : public Octet_string {
	public:
		SS_Status();
		~SS_Status();

	};

	// ForwardingOptions
	class ForwardingOptions : public Octet_string {
	public:
		ForwardingOptions();
		~ForwardingOptions();

	};

	// CallBarringInfo
	class CallBarringInfo : public Sequence {
	public:
		CallBarringInfo();
		~CallBarringInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_Code();
		// nodes
		SS_Code* _ss_Code;
		CallBarringFeatureList* _callBarringFeatureList;

	};

	// CallBarringFeatureList
	class CallBarringFeatureList : public Sequence_of {
	public:
		CallBarringFeatureList();
		~CallBarringFeatureList();
		// nodes
		CallBarringFeature* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// CallBarringFeature
	class CallBarringFeature : public Sequence {
	public:
		CallBarringFeature();
		~CallBarringFeature();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_ss_Status();
		// nodes
		BasicServiceCode* _basicService;
		SS_Status* _ss_Status;

	};

	// SS_Data
	class SS_Data : public Sequence {
	public:
		SS_Data();
		~SS_Data();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_Code();
		void set_ss_Status();
		void set_ss_SubscriptionOption();
		void set_basicServiceGroupList();
		void set_defaultPriority();
		void set_nbrUser();
		// nodes
		SS_Code* _ss_Code;
		SS_Status* _ss_Status;
		SS_SubscriptionOption* _ss_SubscriptionOption;
		BasicServiceGroupList* _basicServiceGroupList;
		EMLPP_Priority* _defaultPriority;
		MC_Bearers* _nbrUser;

	};

	// SS_SubscriptionOption
	class SS_SubscriptionOption : public Choice {
	public:
		SS_SubscriptionOption();
		~SS_SubscriptionOption();
		// nodes
		CliRestrictionOption* _cliRestrictionOption;
		OverrideCategory* _overrideCategory;

	};

	// CliRestrictionOption
	class CliRestrictionOption : public Enumerated {
	public:
		CliRestrictionOption();
		~CliRestrictionOption();
		static const int _permanent = 0;
		static const int _temporaryDefaultRestricted = 1;
		static const int _temporaryDefaultAllowed = 2;

	};

	// OverrideCategory
	class OverrideCategory : public Enumerated {
	public:
		OverrideCategory();
		~OverrideCategory();
		static const int _overrideEnabled = 0;
		static const int _overrideDisabled = 1;
	};

	// SS_ForBS_Code
	class SS_ForBS_Code : public Sequence {
	public:
		SS_ForBS_Code();
		~SS_ForBS_Code();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_longFTN_Supported();
		// nodes
		SS_Code* _ss_Code;
		BasicServiceCode* _basicService;
		Null* _longFTN_Supported;

	};

	// GenericServiceInfo
	class GenericServiceInfo : public Sequence {
	public:
		GenericServiceInfo();
		~GenericServiceInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cliRestrictionOption();
		void set_maximumEntitledPriority();
		void set_defaultPriority();
		void set_ccbs_FeatureList();
		void set_nbrSB();
		void set_nbrUser();
		void set_nbrSN();
		// nodes
		SS_Status* _ss_Status;
		CliRestrictionOption* _cliRestrictionOption;
		EMLPP_Priority* _maximumEntitledPriority;
		EMLPP_Priority* _defaultPriority;
		CCBS_FeatureList* _ccbs_FeatureList;
		MaxMC_Bearers* _nbrSB;
		MC_Bearers* _nbrUser;
		MC_Bearers* _nbrSN;

	};

	// CCBS_FeatureList
	class CCBS_FeatureList : public Sequence_of {
	public:
		CCBS_FeatureList();
		~CCBS_FeatureList();
		// nodes
		CCBS_Feature* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfCCBS_Requests
	class maxNumOfCCBS_Requests : public Integer {
	public:
		maxNumOfCCBS_Requests();
		~maxNumOfCCBS_Requests();

	};

	// CCBS_Feature
	class CCBS_Feature : public Sequence {
	public:
		CCBS_Feature();
		~CCBS_Feature();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ccbs_Index();
		void set_b_subscriberNumber();
		void set_b_subscriberSubaddress();
		void set_basicServiceGroup();
		// nodes
		CCBS_Index* _ccbs_Index;
		ISDN_AddressString* _b_subscriberNumber;
		ISDN_SubaddressString* _b_subscriberSubaddress;
		BasicServiceCode* _basicServiceGroup;

	};

	// CCBS_Index
	class CCBS_Index : public Integer {
	public:
		CCBS_Index();
		~CCBS_Index();

	};

	// InterrogateSS_Res
	class InterrogateSS_Res : public Choice {
	public:
		InterrogateSS_Res();
		~InterrogateSS_Res();
		// nodes
		SS_Status* _ss_Status;
		BasicServiceGroupList* _basicServiceGroupList;
		ForwardingFeatureList* _forwardingFeatureList;
		GenericServiceInfo* _genericServiceInfo;

	};

	// USSD_Arg
	class USSD_Arg : public Sequence {
	public:
		USSD_Arg();
		~USSD_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_alertingPattern();
		void set_msisdn();
		// nodes
		USSD_DataCodingScheme* _ussd_DataCodingScheme;
		USSD_String* _ussd_String;
		AlertingPattern* _alertingPattern;
		ISDN_AddressString* _msisdn;

	};

	// USSD_Res
	class USSD_Res : public Sequence {
	public:
		USSD_Res();
		~USSD_Res();
		// nodes
		USSD_DataCodingScheme* _ussd_DataCodingScheme;
		USSD_String* _ussd_String;

	};

	// USSD_DataCodingScheme
	class USSD_DataCodingScheme : public Octet_string {
	public:
		USSD_DataCodingScheme();
		~USSD_DataCodingScheme();

	};

	// USSD_String
	class USSD_String : public Octet_string {
	public:
		USSD_String();
		~USSD_String();

	};

	// maxUSSD_StringLength
	class maxUSSD_StringLength : public Integer {
	public:
		maxUSSD_StringLength();
		~maxUSSD_StringLength();

	};

	// Password
	class Password : public Numeric_string {
	public:
		Password();
		~Password();

	};

	// GuidanceInfo
	class GuidanceInfo : public Enumerated {
	public:
		GuidanceInfo();
		~GuidanceInfo();
		static const int _enterPW = 0;
		static const int _enterNewPW = 1;
		static const int _enterNewPW_Again = 2;

	};

	// SS_List
	class SS_List : public Sequence_of {
	public:
		SS_List();
		~SS_List();
		// nodes
		SS_Code* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfSS
	class maxNumOfSS : public Integer {
	public:
		maxNumOfSS();
		~maxNumOfSS();

	};

	// SS_InfoList
	class SS_InfoList : public Sequence_of {
	public:
		SS_InfoList();
		~SS_InfoList();
		// nodes
		SS_Info* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// BasicServiceGroupList
	class BasicServiceGroupList : public Sequence_of {
	public:
		BasicServiceGroupList();
		~BasicServiceGroupList();
		// nodes
		BasicServiceCode* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfBasicServiceGroups
	class maxNumOfBasicServiceGroups : public Integer {
	public:
		maxNumOfBasicServiceGroups();
		~maxNumOfBasicServiceGroups();

	};

	// SS_InvocationNotificationArg
	class SS_InvocationNotificationArg : public Sequence {
	public:
		SS_InvocationNotificationArg();
		~SS_InvocationNotificationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_EventSpecification();
		void set_extensionContainer();
		void set_b_subscriberNumber();
		void set_ccbs_RequestState();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _msisdn;
		SS_Code* _ss_Event;
		SS_EventSpecification* _ss_EventSpecification;
		ExtensionContainer* _extensionContainer;
		ISDN_AddressString* _b_subscriberNumber;
		CCBS_RequestState* _ccbs_RequestState;

	};

	// CCBS_RequestState
	class CCBS_RequestState : public Enumerated {
	public:
		CCBS_RequestState();
		~CCBS_RequestState();
		static const int _request = 0;
		static const int _recall = 1;
		static const int _active = 2;
		static const int _completed = 3;
		static const int _suspended = 4;
		static const int _frozen = 5;
		static const int _deleted = 6;

	};

	// SS_InvocationNotificationRes
	class SS_InvocationNotificationRes : public Sequence {
	public:
		SS_InvocationNotificationRes();
		~SS_InvocationNotificationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// SS_EventSpecification
	class SS_EventSpecification : public Sequence_of {
	public:
		SS_EventSpecification();
		~SS_EventSpecification();
		// nodes
		AddressString* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxEventSpecification
	class maxEventSpecification : public Integer {
	public:
		maxEventSpecification();
		~maxEventSpecification();

	};

	// RegisterCC_EntryArg
	class RegisterCC_EntryArg : public Sequence {
	public:
		RegisterCC_EntryArg();
		~RegisterCC_EntryArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ccbs_Data();
		// nodes
		SS_Code* _ss_Code;
		CCBS_Data* _ccbs_Data;

	};

	// CCBS_Data
	class CCBS_Data : public Sequence {
	public:
		CCBS_Data();
		~CCBS_Data();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_serviceIndicator();
		// nodes
		CCBS_Feature* _ccbs_Feature;
		ISDN_AddressString* _translatedB_Number;
		ServiceIndicator* _serviceIndicator;
		ExternalSignalInfo* _callInfo;
		ExternalSignalInfo* _networkSignalInfo;

	};

	// ServiceIndicator
	class ServiceIndicator : public Bit_string {
	public:
		ServiceIndicator();
		~ServiceIndicator();

	};

	// RegisterCC_EntryRes
	class RegisterCC_EntryRes : public Sequence {
	public:
		RegisterCC_EntryRes();
		~RegisterCC_EntryRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ccbs_Feature();
		// nodes
		CCBS_Feature* _ccbs_Feature;

	};

	// EraseCC_EntryArg
	class EraseCC_EntryArg : public Sequence {
	public:
		EraseCC_EntryArg();
		~EraseCC_EntryArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ccbs_Index();
		// nodes
		SS_Code* _ss_Code;
		CCBS_Index* _ccbs_Index;

	};

	// EraseCC_EntryRes
	class EraseCC_EntryRes : public Sequence {
	public:
		EraseCC_EntryRes();
		~EraseCC_EntryRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_Status();
		// nodes
		SS_Code* _ss_Code;
		SS_Status* _ss_Status;

	};

}
#endif
