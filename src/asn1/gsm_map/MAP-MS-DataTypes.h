#ifndef MAP_MS_DATATYPES_H_
#define MAP_MS_DATATYPES_H_

#include<asn1.h>
#include<MAP-OM-DataTypes.h>
#include<MAP-SS-DataTypes.h>
#include<MAP-ER-DataTypes.h>

namespace asn1 {
	// forward declarations
	class UpdateLocationArg;
	class VLR_Capability;
	class SupportedRAT_Types;
	class SuperChargerInfo;
	class AgeIndicator;
	class IST_SupportIndicator;
	class SupportedLCS_CapabilitySets;
	class UpdateLocationRes;
	class ADD_Info;
	class PagingArea;
	class LocationArea;
	class LAC;
	class CancelLocationArg;
	class TypeOfUpdate;
	class CancellationType;
	class CancelLocationRes;
	class PurgeMS_Arg;
	class PurgeMS_Res;
	class SendIdentificationArg;
	class HopCounter;
	class SendIdentificationRes;
	class AuthenticationSetList;
	class TripletList;
	class QuintupletList;
	class AuthenticationTriplet;
	class AuthenticationQuintuplet;
	class CurrentSecurityContext;
	class GSM_SecurityContextData;
	class UMTS_SecurityContextData;
	class RAND;
	class SRES;
	class Kc;
	class XRES;
	class CK;
	class IK;
	class AUTN;
	class AUTS;
	class Cksn;
	class KSI;
	class AuthenticationFailureReportArg;
	class AccessType;
	class AuthenticationFailureReportRes;
	class FailureCause;
	class UpdateGprsLocationArg;
	class SMSRegisterRequest;
	class Used_RAT_Type;
	class EPS_Info;
	class PDN_GW_Update;
	class ISR_Information;
	class SGSN_Capability;
	class SupportedFeatures;
	class UE_SRVCC_Capability;
	class UpdateGprsLocationRes;
	class EPLMN_List;
	class ForwardAccessSignalling_Arg;
	class AllowedGSM_Algorithms;
	class AllowedUMTS_Algorithms;
	class PermittedIntegrityProtectionAlgorithms;
	class maxPermittedIntegrityProtectionAlgorithmsLength;
	class PermittedEncryptionAlgorithms;
	class maxPermittedEncryptionAlgorithmsLength;
	class KeyStatus;
	class PrepareHO_Arg;
	class LCLS_GlobalCallReference;
	class LCLS_Negotiation;
	class LCLS_ConfigurationPreference;
	class BSSMAP_ServiceHandoverList;
	class BSSMAP_ServiceHandoverInfo;
	class maxNumOfServiceHandovers;
	class BSSMAP_ServiceHandover;
	class RANAP_ServiceHandover;
	class RadioResourceList;
	class RadioResource;
	class maxNumOfRadioResources;
	class PrepareHO_Res;
	class SelectedUMTS_Algorithms;
	class ChosenIntegrityProtectionAlgorithm;
	class ChosenEncryptionAlgorithm;
	class ChosenRadioResourceInformation;
	class ChosenChannelInfo;
	class ChosenSpeechVersion;
	class PrepareSubsequentHO_Arg;
	class PrepareSubsequentHO_Res;
	class ProcessAccessSignalling_Arg;
	class AoIPCodecsList;
	class AoIPCodec;
	class SupportedCodecsList;
	class CodecList;
	class Codec;
	class GERAN_Classmark;
	class SelectedGSM_Algorithm;
	class SendEndSignal_Arg;
	class SendEndSignal_Res;
	class RNCId;
	class RelocationNumberList;
	class MulticallBearerInfo;
	class RelocationNumber;
	class RAB_Id;
	class maxNrOfRABs;
	class maxNumOfRelocationNumber;
	class RadioResourceInformation;
	class IntegrityProtectionInformation;
	class maxNumOfIntegrityInfo;
	class EncryptionInformation;
	class maxNumOfEncryptionInfo;
	class SendAuthenticationInfoArg;
	class NumberOfRequestedVectors;
	class Re_synchronisationInfo;
	class SendAuthenticationInfoRes;
	class EPS_AuthenticationSetList;
	class EPC_AV;
	class KASME;
	class RequestingNodeType;
	class CheckIMEI_Arg;
	class CheckIMEI_Res;
	class RequestedEquipmentInfo;
	class UESBI_Iu;
	class UESBI_IuA;
	class UESBI_IuB;
	class EquipmentStatus;
	class InsertSubscriberDataArg;
	class SubscribedPeriodicRAUTAUtimer;
	class SubscribedPeriodicLAUtimer;
	class CSG_SubscriptionDataList;
	class CSG_SubscriptionData;
	class VPLMN_CSG_SubscriptionDataList;
	class CSG_Id;
	class LIPA_AllowedAPNList;
	class maxNumOfLIPAAllowedAPN;
	class EPS_SubscriptionData;
	class APN_OI_Replacement;
	class RFSP_ID;
	class APN_ConfigurationProfile;
	class EPS_DataList;
	class maxNumOfAPN_Configurations;
	class APN_Configuration;
	class PDN_Type;
	class EPS_QoS_Subscribed;
	class AMBR;
	class SpecificAPNInfoList;
	class maxNumOfSpecificAPNInfos;
	class SpecificAPNInfo;
	class Bandwidth;
	class QoS_Class_Identifier;
	class Allocation_Retention_Priority;
	class PDN_GW_Identity;
	class FQDN;
	class PDN_GW_AllocationType;
	class AccessRestrictionData;
	class CS_AllocationRetentionPriority;
	class IST_AlertTimerValue;
	class LCSInformation;
	class GMLC_List;
	class maxNumOfGMLC;
	class NetworkAccessMode;
	class GPRSDataList;
	class maxNumOfPDP_Contexts;
	class PDP_Context;
	class SIPTO_Permission;
	class LIPA_Permission;
	class ContextId;
	class GPRSSubscriptionData;
	class SGSN_CAMEL_SubscriptionInfo;
	class GPRS_CSI;
	class GPRS_CamelTDPDataList;
	class GPRS_CamelTDPData;
	class DefaultGPRS_Handling;
	class GPRS_TriggerDetectionPoint;
	class APN;
	class PDP_Type;
	class Ext_PDP_Type;
	class PDP_Address;
	class QoS_Subscribed;
	class Ext_QoS_Subscribed;
	class Ext2_QoS_Subscribed;
	class Ext3_QoS_Subscribed;
	class Ext4_QoS_Subscribed;
	class ChargingCharacteristics;
	class LSAOnlyAccessIndicator;
	class LSADataList;
	class maxNumOfLSAs;
	class LSAData;
	class LSAInformation;
	class LSAIdentity;
	class LSAAttributes;
	class SubscriberData;
	class Category;
	class SubscriberStatus;
	class BearerServiceList;
	class maxNumOfBearerServices;
	class TeleserviceList;
	class maxNumOfTeleservices;
	class ODB_Data;
	class ODB_GeneralData;
	class ODB_HPLMN_Data;
	class Ext_SS_InfoList;
	class Ext_SS_Info;
	class Ext_ForwInfo;
	class Ext_ForwFeatureList;
	class Ext_ForwFeature;
	class Ext_ForwOptions;
	class Ext_NoRepCondTime;
	class Ext_CallBarInfo;
	class Ext_CallBarFeatureList;
	class Ext_CallBarringFeature;
	class CUG_Info;
	class CUG_SubscriptionList;
	class CUG_Subscription;
	class CUG_Index;
	class CUG_Interlock;
	class IntraCUG_Options;
	class maxNumOfCUG;
	class CUG_FeatureList;
	class Ext_BasicServiceGroupList;
	class maxNumOfExt_BasicServiceGroups;
	class CUG_Feature;
	class InterCUG_Restrictions;
	class Ext_SS_Data;
	class LCS_PrivacyExceptionList;
	class maxNumOfPrivacyClass;
	class LCS_PrivacyClass;
	class ExternalClientList;
	class maxNumOfExternalClient;
	class PLMNClientList;
	class maxNumOfPLMNClient;
	class Ext_ExternalClientList;
	class maxNumOfExt_ExternalClient;
	class ExternalClient;
	class GMLC_Restriction;
	class NotificationToMSUser;
	class ServiceTypeList;
	class maxNumOfServiceType;
	class ServiceType;
	class MOLR_List;
	class maxNumOfMOLR_Class;
	class MOLR_Class;
	class ZoneCodeList;
	class ZoneCode;
	class maxNumOfZoneCodes;
	class InsertSubscriberDataRes;
	class RegionalSubscriptionResponse;
	class DeleteSubscriberDataArg;
	class SpecificCSI_Withdraw;
	class GPRSSubscriptionDataWithdraw;
	class EPS_SubscriptionDataWithdraw;
	class ContextIdList;
	class LSAInformationWithdraw;
	class LSAIdentityList;
	class BasicServiceList;
	class maxNumOfBasicServices;
	class DeleteSubscriberDataRes;
	class VlrCamelSubscriptionInfo;
	class MT_smsCAMELTDP_CriteriaList;
	class MT_smsCAMELTDP_Criteria;
	class TPDU_TypeCriterion;
	class maxNumOfTPDUTypes;
	class MT_SMS_TPDU_Type;
	class D_CSI;
	class DP_AnalysedInfoCriteriaList;
	class maxNumOfDP_AnalysedInfoCriteria;
	class DP_AnalysedInfoCriterium;
	class SS_CSI;
	class SS_CamelData;
	class SS_EventList;
	class maxNumOfCamelSSEvents;
	class O_CSI;
	class O_BcsmCamelTDPDataList;
	class maxNumOfCamelTDPData;
	class O_BcsmCamelTDPData;
	class ServiceKey;
	class O_BcsmTriggerDetectionPoint;
	class O_BcsmCamelTDPCriteriaList;
	class T_BCSM_CAMEL_TDP_CriteriaList;
	class O_BcsmCamelTDP_Criteria;
	class T_BCSM_CAMEL_TDP_Criteria;
	class DestinationNumberCriteria;
	class DestinationNumberList;
	class DestinationNumberLengthList;
	class BasicServiceCriteria;
	class maxNumOfISDN_AddressDigits;
	class maxNumOfCamelDestinationNumbers;
	class maxNumOfCamelDestinationNumberLengths;
	class maxNumOfCamelBasicServiceCriteria;
	class CallTypeCriteria;
	class MatchType;
	class O_CauseValueCriteria;
	class T_CauseValueCriteria;
	class maxNumOfCAMEL_O_CauseValueCriteria;
	class maxNumOfCAMEL_T_CauseValueCriteria;
	class CauseValue;
	class DefaultCallHandling;
	class CamelCapabilityHandling;
	class SupportedCamelPhases;
	class OfferedCamel4CSIs;
	class OfferedCamel4Functionalities;
	class SMS_CSI;
	class SMS_CAMEL_TDP_DataList;
	class SMS_CAMEL_TDP_Data;
	class SMS_TriggerDetectionPoint;
	class DefaultSMS_Handling;
	class M_CSI;
	class MG_CSI;
	class MobilityTriggers;
	class maxNumOfMobilityTriggers;
	class MM_Code;
	class T_CSI;
	class T_BcsmCamelTDPDataList;
	class T_BcsmCamelTDPData;
	class T_BcsmTriggerDetectionPoint;
	class SendRoutingInfoForGprsArg;
	class SendRoutingInfoForGprsRes;
	class FailureReportArg;
	class FailureReportRes;
	class NoteMsPresentForGprsArg;
	class NoteMsPresentForGprsRes;
	class ResetArg;
	class SendingNode_Number;
	class RestoreDataArg;
	class RestoreDataRes;
	class VBSDataList;
	class VGCSDataList;
	class maxNumOfVBSGroupIds;
	class maxNumOfVGCSGroupIds;
	class VoiceGroupCallData;
	class AdditionalInfo;
	class AdditionalSubscriptions;
	class VoiceBroadcastData;
	class GroupId;
	class Long_GroupId;
	class ProvideSubscriberInfoArg;
	class ProvideSubscriberInfoRes;
	class SubscriberInfo;
	class IMS_VoiceOverPS_SessionsInd;
	class TimeZone;
	class DaylightSavingTime;
	class MNPInfoRes;
	class RouteingNumber;
	class NumberPortabilityStatus;
	class MS_Classmark2;
	class GPRSMSClass;
	class MSNetworkCapability;
	class MSRadioAccessCapability;
	class RequestedInfo;
	class DomainType;
	class RequestedNodes;
	class LocationInformation;
	class LocationInformationEPS;
	class LocationInformationGPRS;
	class UserCSGInformation;
	class GeographicalInformation;
	class GeodeticInformation;
	class LocationNumber;
	class SubscriberState;
	class PS_SubscriberState;
	class PDP_ContextInfoList;
	class PDP_ContextInfo;
	class NSAPI;
	class TransactionId;
	class TEID;
	class GPRSChargingID;
	class NotReachableReason;
	class AnyTimeInterrogationArg;
	class AnyTimeInterrogationRes;
	class AnyTimeSubscriptionInterrogationArg;
	class AnyTimeSubscriptionInterrogationRes;
	class CallWaitingData;
	class Ext_CwFeatureList;
	class Ext_CwFeature;
	class ClipData;
	class ClirData;
	class CallHoldData;
	class EctData;
	class RequestedSubscriptionInfo;
	class MSISDN_BS_List;
	class maxNumOfMSISDN;
	class MSISDN_BS;
	class RequestedCAMEL_SubscriptionInfo;
	class AdditionalRequestedCAMEL_SubscriptionInfo;
	class CallForwardingData;
	class CallBarringData;
	class WrongPasswordAttemptsCounter;
	class ODB_Info;
	class CAMEL_SubscriptionInfo;
	class AnyTimeModificationArg;
	class ModificationRequestFor_CW_Info;
	class ModificationRequestFor_CH_Info;
	class ModificationRequestFor_ECT_Info;
	class ModificationRequestFor_CLIR_Info;
	class ModificationRequestFor_CLIP_Info;
	class ModificationRequestFor_CSG;
	class RequestedServingNode;
	class ServingNode;
	class AnyTimeModificationRes;
	class ModificationRequestFor_CF_Info;
	class ModificationRequestFor_CB_Info;
	class ModificationRequestFor_ODB_data;
	class ModificationRequestFor_CSI;
	class ModificationRequestFor_IP_SM_GW_Data;
	class ModificationInstruction;
	class NoteSubscriberDataModifiedArg;
	class NoteSubscriberDataModifiedRes;
	class NoteMM_EventArg;
	class NoteMM_EventRes;
	class Ext_SS_InfoFor_CSE;
	class Ext_ForwardingInfoFor_CSE;
	class Ext_CallBarringInfoFor_CSE;
	class UpdateVcsgLocationArg;
	class UpdateVcsgLocationRes;
	class CancelVcsgLocationArg;
	class CancelVcsgLocationRes;

	

	// UpdateLocationArg
	class UpdateLocationArg : public Sequence {
	public:
		UpdateLocationArg();
		~UpdateLocationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_lmsi();
		void set_extensionContainer();
		void set_vlr_Capability();
		void set_informPreviousNetworkEntity();
		void set_cs_LCS_NotSupportedByUE();
		void set_v_gmlc_Address();
		void set_add_info();
		void set_pagingArea();
		void set_skipSubscriberDataUpdate();
		void set_restorationIndicator();
		void set_eplmn_List();
		void set_mme_DiameterAddress();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _msc_Number;
		ISDN_AddressString* _vlr_Number;
		LMSI* _lmsi;
		ExtensionContainer* _extensionContainer;
		VLR_Capability* _vlr_Capability;
		Null* _informPreviousNetworkEntity;
		Null* _cs_LCS_NotSupportedByUE;
		GSN_Address* _v_gmlc_Address;
		ADD_Info* _add_info;
		PagingArea* _pagingArea;
		Null* _skipSubscriberDataUpdate;
		Null* _restorationIndicator;
		EPLMN_List* _eplmn_List;
		NetworkNodeDiameterAddress* _mme_DiameterAddress;

	};

	// VLR_Capability
	class VLR_Capability : public Sequence {
	public:
		VLR_Capability();
		~VLR_Capability();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_supportedCamelPhases();
		void set_extensionContainer();
		void set_solsaSupportIndicator();
		void set_istSupportIndicator();
		void set_superChargerSupportedInServingNetworkEntity();
		void set_longFTN_Supported();
		void set_supportedLCS_CapabilitySets();
		void set_offeredCamel4CSIs();
		void set_supportedRAT_TypesIndicator();
		void set_longGroupID_Supported();
		void set_mtRoamingForwardingSupported();
		void set_msisdn_lessOperation_Supported();
		// nodes
		SupportedCamelPhases* _supportedCamelPhases;
		ExtensionContainer* _extensionContainer;
		Null* _solsaSupportIndicator;
		IST_SupportIndicator* _istSupportIndicator;
		SuperChargerInfo* _superChargerSupportedInServingNetworkEntity;
		Null* _longFTN_Supported;
		SupportedLCS_CapabilitySets* _supportedLCS_CapabilitySets;
		OfferedCamel4CSIs* _offeredCamel4CSIs;
		SupportedRAT_Types* _supportedRAT_TypesIndicator;
		Null* _longGroupID_Supported;
		Null* _mtRoamingForwardingSupported;
		Null* _msisdn_lessOperation_Supported;

	};

	// SupportedRAT_Types
	class SupportedRAT_Types : public Bit_string {
	public:
		SupportedRAT_Types();
		~SupportedRAT_Types();

	};

	// SuperChargerInfo
	class SuperChargerInfo : public Choice {
	public:
		SuperChargerInfo();
		~SuperChargerInfo();
		// nodes
		Null* _sendSubscriberData;
		AgeIndicator* _subscriberDataStored;

	};

	// AgeIndicator
	class AgeIndicator : public Octet_string {
	public:
		AgeIndicator();
		~AgeIndicator();

	};

	// IST_SupportIndicator
	class IST_SupportIndicator : public Enumerated {
	public:
		IST_SupportIndicator();
		~IST_SupportIndicator();
		static const int _basicISTSupported = 0;
		static const int _istCommandSupported = 1;

	};

	// SupportedLCS_CapabilitySets
	class SupportedLCS_CapabilitySets : public Bit_string {
	public:
		SupportedLCS_CapabilitySets();
		~SupportedLCS_CapabilitySets();

	};

	// UpdateLocationRes
	class UpdateLocationRes : public Sequence {
	public:
		UpdateLocationRes();
		~UpdateLocationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_add_Capability();
		void set_pagingArea_Capability();
		// nodes
		ISDN_AddressString* _hlr_Number;
		ExtensionContainer* _extensionContainer;
		Null* _add_Capability;
		Null* _pagingArea_Capability;

	};

	// ADD_Info
	class ADD_Info : public Sequence {
	public:
		ADD_Info();
		~ADD_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_skipSubscriberDataUpdate();
		// nodes
		IMEI* _imeisv;
		Null* _skipSubscriberDataUpdate;

	};

	// PagingArea
	class PagingArea : public Sequence_of {
	public:
		PagingArea();
		~PagingArea();
		// nodes
		LocationArea* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// LocationArea
	class LocationArea : public Choice {
	public:
		LocationArea();
		~LocationArea();
		// nodes
		LAIFixedLength* _laiFixedLength;
		LAC* _lac;

	};

	// LAC
	class LAC : public Octet_string {
	public:
		LAC();
		~LAC();

	};

	// CancelLocationArg
	class CancelLocationArg : public Sequence {
	public:
		CancelLocationArg();
		~CancelLocationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cancellationType();
		void set_extensionContainer();
		void set_typeOfUpdate();
		void set_mtrf_SupportedAndAuthorized();
		void set_mtrf_SupportedAndNotAuthorized();
		void set_newMSC_Number();
		void set_newVLR_Number();
		void set_new_lmsi();
		// nodes
		Identity* _identity;
		CancellationType* _cancellationType;
		ExtensionContainer* _extensionContainer;
		TypeOfUpdate* _typeOfUpdate;
		Null* _mtrf_SupportedAndAuthorized;
		Null* _mtrf_SupportedAndNotAuthorized;
		ISDN_AddressString* _newMSC_Number;
		ISDN_AddressString* _newVLR_Number;
		LMSI* _new_lmsi;

	};

	// TypeOfUpdate
	class TypeOfUpdate : public Enumerated {
	public:
		TypeOfUpdate();
		~TypeOfUpdate();
		static const int _sgsn_change = 0;
		static const int _mme_change = 1;

	};

	// CancellationType
	class CancellationType : public Enumerated {
	public:
		CancellationType();
		~CancellationType();
		static const int _updateProcedure = 0;
		static const int _subscriptionWithdraw = 1;
		static const int _initialAttachProcedure = 2;

	};

	// CancelLocationRes
	class CancelLocationRes : public Sequence {
	public:
		CancelLocationRes();
		~CancelLocationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// PurgeMS_Arg
	class PurgeMS_Arg : public Sequence {
	public:
		PurgeMS_Arg();
		~PurgeMS_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_vlr_Number();
		void set_sgsn_Number();
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _vlr_Number;
		ISDN_AddressString* _sgsn_Number;
		ExtensionContainer* _extensionContainer;

	};

	// PurgeMS_Res
	class PurgeMS_Res : public Sequence {
	public:
		PurgeMS_Res();
		~PurgeMS_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_freezeTMSI();
		void set_freezeP_TMSI();
		void set_extensionContainer();
		void set_freezeM_TMSI();
		// nodes
		Null* _freezeTMSI;
		Null* _freezeP_TMSI;
		ExtensionContainer* _extensionContainer;
		Null* _freezeM_TMSI;

	};

	// SendIdentificationArg
	class SendIdentificationArg : public Sequence {
	public:
		SendIdentificationArg();
		~SendIdentificationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_numberOfRequestedVectors();
		void set_segmentationProhibited();
		void set_extensionContainer();
		void set_msc_Number();
		void set_previous_LAI();
		void set_hopCounter();
		void set_mtRoamingForwardingSupported();
		void set_newVLR_Number();
		void set_new_lmsi();
		// nodes
		TMSI* _tmsi;
		NumberOfRequestedVectors* _numberOfRequestedVectors;
		Null* _segmentationProhibited;
		ExtensionContainer* _extensionContainer;
		ISDN_AddressString* _msc_Number;
		LAIFixedLength* _previous_LAI;
		HopCounter* _hopCounter;
		Null* _mtRoamingForwardingSupported;
		ISDN_AddressString* _newVLR_Number;
		LMSI* _new_lmsi;

	};

	// HopCounter
	class HopCounter : public Integer {
	public:
		HopCounter();
		~HopCounter();

	};

	// SendIdentificationRes
	class SendIdentificationRes : public Sequence {
	public:
		SendIdentificationRes();
		~SendIdentificationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_imsi();
		void set_authenticationSetList();
		void set_currentSecurityContext();
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		AuthenticationSetList* _authenticationSetList;
		CurrentSecurityContext* _currentSecurityContext;
		ExtensionContainer* _extensionContainer;

	};

	// AuthenticationSetList
	class AuthenticationSetList : public Choice {
	public:
		AuthenticationSetList();
		~AuthenticationSetList();
		// nodes
		TripletList* _tripletList;
		QuintupletList* _quintupletList;

	};

	// TripletList
	class TripletList : public Sequence_of {
	public:
		TripletList();
		~TripletList();
		// nodes
		AuthenticationTriplet* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// QuintupletList
	class QuintupletList : public Sequence_of {
	public:
		QuintupletList();
		~QuintupletList();
		// nodes
		AuthenticationQuintuplet* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// AuthenticationTriplet
	class AuthenticationTriplet : public Sequence {
	public:
		AuthenticationTriplet();
		~AuthenticationTriplet();
		// nodes
		RAND* _rand;
		SRES* _sres;
		Kc* _kc;

	};

	// AuthenticationQuintuplet
	class AuthenticationQuintuplet : public Sequence {
	public:
		AuthenticationQuintuplet();
		~AuthenticationQuintuplet();
		// nodes
		RAND* _rand;
		XRES* _xres;
		CK* _ck;
		IK* _ik;
		AUTN* _autn;

	};

	// CurrentSecurityContext
	class CurrentSecurityContext : public Choice {
	public:
		CurrentSecurityContext();
		~CurrentSecurityContext();
		// nodes
		GSM_SecurityContextData* _gsm_SecurityContextData;
		UMTS_SecurityContextData* _umts_SecurityContextData;

	};

	// GSM_SecurityContextData
	class GSM_SecurityContextData : public Sequence {
	public:
		GSM_SecurityContextData();
		~GSM_SecurityContextData();
		// nodes
		Kc* _kc;
		Cksn* _cksn;

	};

	// UMTS_SecurityContextData
	class UMTS_SecurityContextData : public Sequence {
	public:
		UMTS_SecurityContextData();
		~UMTS_SecurityContextData();
		// nodes
		CK* _ck;
		IK* _ik;
		KSI* _ksi;

	};

	// RAND
	class RAND : public Octet_string {
	public:
		RAND();
		~RAND();

	};

	// SRES
	class SRES : public Octet_string {
	public:
		SRES();
		~SRES();

	};

	// Kc
	class Kc : public Octet_string {
	public:
		Kc();
		~Kc();

	};

	// XRES
	class XRES : public Octet_string {
	public:
		XRES();
		~XRES();

	};

	// CK
	class CK : public Octet_string {
	public:
		CK();
		~CK();

	};

	// IK
	class IK : public Octet_string {
	public:
		IK();
		~IK();

	};

	// AUTN
	class AUTN : public Octet_string {
	public:
		AUTN();
		~AUTN();

	};

	// AUTS
	class AUTS : public Octet_string {
	public:
		AUTS();
		~AUTS();

	};

	// Cksn
	class Cksn : public Octet_string {
	public:
		Cksn();
		~Cksn();

	};

	// KSI
	class KSI : public Octet_string {
	public:
		KSI();
		~KSI();

	};

	// AuthenticationFailureReportArg
	class AuthenticationFailureReportArg : public Sequence {
	public:
		AuthenticationFailureReportArg();
		~AuthenticationFailureReportArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_re_attempt();
		void set_accessType();
		void set_rand();
		void set_vlr_Number();
		void set_sgsn_Number();
		// nodes
		IMSI* _imsi;
		FailureCause* _failureCause;
		ExtensionContainer* _extensionContainer;
		Boolean* _re_attempt;
		AccessType* _accessType;
		RAND* _rand;
		ISDN_AddressString* _vlr_Number;
		ISDN_AddressString* _sgsn_Number;

	};

	// AccessType
	class AccessType : public Enumerated {
	public:
		AccessType();
		~AccessType();
		static const int _call = 0;
		static const int _emergencyCall = 1;
		static const int _locationUpdating = 2;
		static const int _supplementaryService = 3;
		static const int _shortMessage = 4;
		static const int _gprsAttach = 5;
		static const int _routingAreaUpdating = 6;
		static const int _serviceRequest = 7;
		static const int _pdpContextActivation = 8;
		static const int _pdpContextDeactivation = 9;
		static const int _gprsDetach = 10;
	};

	// AuthenticationFailureReportRes
	class AuthenticationFailureReportRes : public Sequence {
	public:
		AuthenticationFailureReportRes();
		~AuthenticationFailureReportRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// FailureCause
	class FailureCause : public Enumerated {
	public:
		FailureCause();
		~FailureCause();
		static const int _wrongUserResponse = 0;
		static const int _wrongNetworkSignature = 1;

	};

	// UpdateGprsLocationArg
	class UpdateGprsLocationArg : public Sequence {
	public:
		UpdateGprsLocationArg();
		~UpdateGprsLocationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_sgsn_Capability();
		void set_informPreviousNetworkEntity();
		void set_ps_LCS_NotSupportedByUE();
		void set_v_gmlc_Address();
		void set_add_info();
		void set_eps_info();
		void set_servingNodeTypeIndicator();
		void set_skipSubscriberDataUpdate();
		void set_usedRAT_Type();
		void set_gprsSubscriptionDataNotNeeded();
		void set_nodeTypeIndicator();
		void set_areaRestricted();
		void set_ue_reachableIndicator();
		void set_epsSubscriptionDataNotNeeded();
		void set_ue_srvcc_Capability();
		void set_eplmn_List();
		void set_mmeNumberforMTSMS();
		void set_smsRegisterRequest();
		void set_sms_Only();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _sgsn_Number;
		GSN_Address* _sgsn_Address;
		ExtensionContainer* _extensionContainer;
		SGSN_Capability* _sgsn_Capability;
		Null* _informPreviousNetworkEntity;
		Null* _ps_LCS_NotSupportedByUE;
		GSN_Address* _v_gmlc_Address;
		ADD_Info* _add_info;
		EPS_Info* _eps_info;
		Null* _servingNodeTypeIndicator;
		Null* _skipSubscriberDataUpdate;
		Used_RAT_Type* _usedRAT_Type;
		Null* _gprsSubscriptionDataNotNeeded;
		Null* _nodeTypeIndicator;
		Null* _areaRestricted;
		Null* _ue_reachableIndicator;
		Null* _epsSubscriptionDataNotNeeded;
		UE_SRVCC_Capability* _ue_srvcc_Capability;
		EPLMN_List* _eplmn_List;
		ISDN_AddressString* _mmeNumberforMTSMS;
		SMSRegisterRequest* _smsRegisterRequest;
		Null* _sms_Only;

	};

	// SMSRegisterRequest
	class SMSRegisterRequest : public Enumerated {
	public:
		SMSRegisterRequest();
		~SMSRegisterRequest();
		static const int _sms_registration_required = 0;
		static const int _sms_registration_not_preferred = 1;
		static const int _no_preference = 2;

	};

	// Used_RAT_Type
	class Used_RAT_Type : public Enumerated {
	public:
		Used_RAT_Type();
		~Used_RAT_Type();
 		static const int _utran = 0;
 		static const int _geran = 1;
 		static const int _gan = 2;
 		static const int _i_hspa_evolution = 3;
 		static const int _e_utran = 4;

	};

	// EPS_Info
	class EPS_Info : public Choice {
	public:
		EPS_Info();
		~EPS_Info();
		// nodes
		PDN_GW_Update* _pdn_gw_update;
		ISR_Information* _isr_Information;

	};

	// PDN_GW_Update
	class PDN_GW_Update : public Sequence {
	public:
		PDN_GW_Update();
		~PDN_GW_Update();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_apn();
		void set_pdn_gw_Identity();
		void set_contextId();
		void set_extensionContainer();
		// nodes
		APN* _apn;
		PDN_GW_Identity* _pdn_gw_Identity;
		ContextId* _contextId;
		ExtensionContainer* _extensionContainer;

	};

	// ISR_Information
	class ISR_Information : public Bit_string {
	public:
		ISR_Information();
		~ISR_Information();

	};

	// SGSN_Capability
	class SGSN_Capability : public Sequence {
	public:
		SGSN_Capability();
		~SGSN_Capability();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_solsaSupportIndicator();
		void set_extensionContainer();
		void set_superChargerSupportedInServingNetworkEntity();
		void set_gprsEnhancementsSupportIndicator();
		void set_supportedCamelPhases();
		void set_supportedLCS_CapabilitySets();
		void set_offeredCamel4CSIs();
		void set_smsCallBarringSupportIndicator();
		void set_supportedRAT_TypesIndicator();
		void set_supportedFeatures();
		void set_t_adsDataRetrieval();
		void set_homogeneousSupportOfIMSVoiceOverPSSessions();
		void set_cancellationTypeInitialAttach();
		void set_additionalMsisdnSupport();
		void set_msisdn_lessOperation_Supported();
		// nodes
		Null* _solsaSupportIndicator;
		ExtensionContainer* _extensionContainer;
		SuperChargerInfo* _superChargerSupportedInServingNetworkEntity;
		Null* _gprsEnhancementsSupportIndicator;
		SupportedCamelPhases* _supportedCamelPhases;
		SupportedLCS_CapabilitySets* _supportedLCS_CapabilitySets;
		OfferedCamel4CSIs* _offeredCamel4CSIs;
		Null* _smsCallBarringSupportIndicator;
		SupportedRAT_Types* _supportedRAT_TypesIndicator;
		SupportedFeatures* _supportedFeatures;
		Null* _t_adsDataRetrieval;
		Boolean* _homogeneousSupportOfIMSVoiceOverPSSessions;
		Null* _cancellationTypeInitialAttach;
		Null* _additionalMsisdnSupport;
		Null* _msisdn_lessOperation_Supported;

	};

	// SupportedFeatures
	class SupportedFeatures : public Bit_string {
	public:
		SupportedFeatures();
		~SupportedFeatures();

	};

	// UE_SRVCC_Capability
	class UE_SRVCC_Capability : public Enumerated {
	public:
		UE_SRVCC_Capability();
		~UE_SRVCC_Capability();
		static const int _ue_srvcc_not_supported = 0;
		static const int _ue_srvcc_supported = 1;

	};

	// UpdateGprsLocationRes
	class UpdateGprsLocationRes : public Sequence {
	public:
		UpdateGprsLocationRes();
		~UpdateGprsLocationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_add_Capability();
		void set_sgsn_mmeSeparationSupported();
		void set_mmeRegisteredforSMS();
		// nodes
		ISDN_AddressString* _hlr_Number;
		ExtensionContainer* _extensionContainer;
		Null* _add_Capability;
		Null* _sgsn_mmeSeparationSupported;
		Null* _mmeRegisteredforSMS;

	};

	// EPLMN_List
	class EPLMN_List : public Sequence_of {
	public:
		EPLMN_List();
		~EPLMN_List();
		// nodes
		PLMN_Id* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// ForwardAccessSignalling_Arg
	class ForwardAccessSignalling_Arg : public Sequence {
	public:
		ForwardAccessSignalling_Arg();
		~ForwardAccessSignalling_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_integrityProtectionInfo();
		void set_encryptionInfo();
		void set_keyStatus();
		void set_allowedGSM_Algorithms();
		void set_allowedUMTS_Algorithms();
		void set_radioResourceInformation();
		void set_extensionContainer();
		void set_radioResourceList();
		void set_bssmap_ServiceHandover();
		void set_ranap_ServiceHandover();
		void set_bssmap_ServiceHandoverList();
		void set_currentlyUsedCodec();
		void set_iuSupportedCodecsList();
		void set_rab_ConfigurationIndicator();
		void set_iuSelectedCodec();
		void set_alternativeChannelType();
		void set_tracePropagationList();
		void set_aoipSupportedCodecsListAnchor();
		void set_aoipSelectedCodecTarget();
		// nodes
		AccessNetworkSignalInfo* _an_APDU;
		IntegrityProtectionInformation* _integrityProtectionInfo;
		EncryptionInformation* _encryptionInfo;
		KeyStatus* _keyStatus;
		AllowedGSM_Algorithms* _allowedGSM_Algorithms;
		AllowedUMTS_Algorithms* _allowedUMTS_Algorithms;
		RadioResourceInformation* _radioResourceInformation;
		ExtensionContainer* _extensionContainer;
		RadioResourceList* _radioResourceList;
		BSSMAP_ServiceHandover* _bssmap_ServiceHandover;
		RANAP_ServiceHandover* _ranap_ServiceHandover;
		BSSMAP_ServiceHandoverList* _bssmap_ServiceHandoverList;
		Codec* _currentlyUsedCodec;
		SupportedCodecsList* _iuSupportedCodecsList;
		Null* _rab_ConfigurationIndicator;
		Codec* _iuSelectedCodec;
		RadioResourceInformation* _alternativeChannelType;
		TracePropagationList* _tracePropagationList;
		AoIPCodecsList* _aoipSupportedCodecsListAnchor;
		AoIPCodec* _aoipSelectedCodecTarget;

	};

	// AllowedGSM_Algorithms
	class AllowedGSM_Algorithms : public Octet_string {
	public:
		AllowedGSM_Algorithms();
		~AllowedGSM_Algorithms();

	};

	// AllowedUMTS_Algorithms
	class AllowedUMTS_Algorithms : public Sequence {
	public:
		AllowedUMTS_Algorithms();
		~AllowedUMTS_Algorithms();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_integrityProtectionAlgorithms();
		void set_encryptionAlgorithms();
		void set_extensionContainer();
		// nodes
		PermittedIntegrityProtectionAlgorithms* _integrityProtectionAlgorithms;
		PermittedEncryptionAlgorithms* _encryptionAlgorithms;
		ExtensionContainer* _extensionContainer;

	};

	// PermittedIntegrityProtectionAlgorithms
	class PermittedIntegrityProtectionAlgorithms : public Octet_string {
	public:
		PermittedIntegrityProtectionAlgorithms();
		~PermittedIntegrityProtectionAlgorithms();

	};

	// maxPermittedIntegrityProtectionAlgorithmsLength
	class maxPermittedIntegrityProtectionAlgorithmsLength : public Integer {
	public:
		maxPermittedIntegrityProtectionAlgorithmsLength();
		~maxPermittedIntegrityProtectionAlgorithmsLength();

	};

	// PermittedEncryptionAlgorithms
	class PermittedEncryptionAlgorithms : public Octet_string {
	public:
		PermittedEncryptionAlgorithms();
		~PermittedEncryptionAlgorithms();

	};

	// maxPermittedEncryptionAlgorithmsLength
	class maxPermittedEncryptionAlgorithmsLength : public Integer {
	public:
		maxPermittedEncryptionAlgorithmsLength();
		~maxPermittedEncryptionAlgorithmsLength();

	};

	// KeyStatus
	class KeyStatus : public Enumerated {
	public:
		KeyStatus();
		~KeyStatus();
		static const int _old = 0;
		static const int _new = 1;

	};

	// PrepareHO_Arg
	class PrepareHO_Arg : public Sequence {
	public:
		PrepareHO_Arg();
		~PrepareHO_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_targetCellId();
		void set_ho_NumberNotRequired();
		void set_targetRNCId();
		void set_an_APDU();
		void set_multipleBearerRequested();
		void set_imsi();
		void set_integrityProtectionInfo();
		void set_encryptionInfo();
		void set_radioResourceInformation();
		void set_allowedGSM_Algorithms();
		void set_allowedUMTS_Algorithms();
		void set_radioResourceList();
		void set_extensionContainer();
		void set_rab_Id();
		void set_bssmap_ServiceHandover();
		void set_ranap_ServiceHandover();
		void set_bssmap_ServiceHandoverList();
		void set_asciCallReference();
		void set_geran_classmark();
		void set_iuCurrentlyUsedCodec();
		void set_iuSupportedCodecsList();
		void set_rab_ConfigurationIndicator();
		void set_uesbi_Iu();
		void set_imeisv();
		void set_alternativeChannelType();
		void set_tracePropagationList();
		void set_aoipSupportedCodecsListAnchor();
		void set_regionalSubscriptionData();
		void set_lclsGlobalCallReference();
		void set_lcls_Negotiation();
		void set_lcls_Configuration_Preference();
		void set_csg_SubscriptionDataList();
		// nodes
		GlobalCellId* _targetCellId;
		Null* _ho_NumberNotRequired;
		RNCId* _targetRNCId;
		AccessNetworkSignalInfo* _an_APDU;
		Null* _multipleBearerRequested;
		IMSI* _imsi;
		IntegrityProtectionInformation* _integrityProtectionInfo;
		EncryptionInformation* _encryptionInfo;
		RadioResourceInformation* _radioResourceInformation;
		AllowedGSM_Algorithms* _allowedGSM_Algorithms;
		AllowedUMTS_Algorithms* _allowedUMTS_Algorithms;
		RadioResourceList* _radioResourceList;
		ExtensionContainer* _extensionContainer;
		RAB_Id* _rab_Id;
		BSSMAP_ServiceHandover* _bssmap_ServiceHandover;
		RANAP_ServiceHandover* _ranap_ServiceHandover;
		BSSMAP_ServiceHandoverList* _bssmap_ServiceHandoverList;
		ASCI_CallReference* _asciCallReference;
		GERAN_Classmark* _geran_classmark;
		Codec* _iuCurrentlyUsedCodec;
		SupportedCodecsList* _iuSupportedCodecsList;
		Null* _rab_ConfigurationIndicator;
		UESBI_Iu* _uesbi_Iu;
		IMEI* _imeisv;
		RadioResourceInformation* _alternativeChannelType;
		TracePropagationList* _tracePropagationList;
		AoIPCodecsList* _aoipSupportedCodecsListAnchor;
		ZoneCodeList* _regionalSubscriptionData;
		LCLS_GlobalCallReference* _lclsGlobalCallReference;
		LCLS_Negotiation* _lcls_Negotiation;
		LCLS_ConfigurationPreference* _lcls_Configuration_Preference;
		CSG_SubscriptionDataList* _csg_SubscriptionDataList;

	};

	// LCLS_GlobalCallReference
	class LCLS_GlobalCallReference : public Octet_string {
	public:
		LCLS_GlobalCallReference();
		~LCLS_GlobalCallReference();

	};

	// LCLS_Negotiation
	class LCLS_Negotiation : public Bit_string {
	public:
		LCLS_Negotiation();
		~LCLS_Negotiation();

	};

	// LCLS_ConfigurationPreference
	class LCLS_ConfigurationPreference : public Bit_string {
	public:
		LCLS_ConfigurationPreference();
		~LCLS_ConfigurationPreference();

	};

	// BSSMAP_ServiceHandoverList
	class BSSMAP_ServiceHandoverList : public Sequence_of {
	public:
		BSSMAP_ServiceHandoverList();
		~BSSMAP_ServiceHandoverList();
		// nodes
		BSSMAP_ServiceHandoverInfo* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// BSSMAP_ServiceHandoverInfo
	class BSSMAP_ServiceHandoverInfo : public Sequence {
	public:
		BSSMAP_ServiceHandoverInfo();
		~BSSMAP_ServiceHandoverInfo();
		// nodes
		BSSMAP_ServiceHandover* _bssmap_ServiceHandover;
		RAB_Id* _rab_Id;

	};

	// maxNumOfServiceHandovers
	class maxNumOfServiceHandovers : public Integer {
	public:
		maxNumOfServiceHandovers();
		~maxNumOfServiceHandovers();

	};

	// BSSMAP_ServiceHandover
	class BSSMAP_ServiceHandover : public Octet_string {
	public:
		BSSMAP_ServiceHandover();
		~BSSMAP_ServiceHandover();

	};

	// RANAP_ServiceHandover
	class RANAP_ServiceHandover : public Octet_string {
	public:
		RANAP_ServiceHandover();
		~RANAP_ServiceHandover();

	};

	// RadioResourceList
	class RadioResourceList : public Sequence_of {
	public:
		RadioResourceList();
		~RadioResourceList();
		// nodes
		RadioResource* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// RadioResource
	class RadioResource : public Sequence {
	public:
		RadioResource();
		~RadioResource();
		// nodes
		RadioResourceInformation* _radioResourceInformation;
		RAB_Id* _rab_Id;

	};

	// maxNumOfRadioResources
	class maxNumOfRadioResources : public Integer {
	public:
		maxNumOfRadioResources();
		~maxNumOfRadioResources();

	};

	// PrepareHO_Res
	class PrepareHO_Res : public Sequence {
	public:
		PrepareHO_Res();
		~PrepareHO_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_handoverNumber();
		void set_relocationNumberList();
		void set_an_APDU();
		void set_multicallBearerInfo();
		void set_multipleBearerNotSupported();
		void set_selectedUMTS_Algorithms();
		void set_chosenRadioResourceInformation();
		void set_extensionContainer();
		void set_iuSelectedCodec();
		void set_iuAvailableCodecsList();
		void set_aoipSelectedCodecTarget();
		void set_aoipAvailableCodecsListMap();
		// nodes
		ISDN_AddressString* _handoverNumber;
		RelocationNumberList* _relocationNumberList;
		AccessNetworkSignalInfo* _an_APDU;
		MulticallBearerInfo* _multicallBearerInfo;
		Null* _multipleBearerNotSupported;
		SelectedUMTS_Algorithms* _selectedUMTS_Algorithms;
		ChosenRadioResourceInformation* _chosenRadioResourceInformation;
		ExtensionContainer* _extensionContainer;
		Codec* _iuSelectedCodec;
		CodecList* _iuAvailableCodecsList;
		AoIPCodec* _aoipSelectedCodecTarget;
		AoIPCodecsList* _aoipAvailableCodecsListMap;

	};

	// SelectedUMTS_Algorithms
	class SelectedUMTS_Algorithms : public Sequence {
	public:
		SelectedUMTS_Algorithms();
		~SelectedUMTS_Algorithms();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_integrityProtectionAlgorithm();
		void set_encryptionAlgorithm();
		void set_extensionContainer();
		// nodes
		ChosenIntegrityProtectionAlgorithm* _integrityProtectionAlgorithm;
		ChosenEncryptionAlgorithm* _encryptionAlgorithm;
		ExtensionContainer* _extensionContainer;

	};

	// ChosenIntegrityProtectionAlgorithm
	class ChosenIntegrityProtectionAlgorithm : public Octet_string {
	public:
		ChosenIntegrityProtectionAlgorithm();
		~ChosenIntegrityProtectionAlgorithm();

	};

	// ChosenEncryptionAlgorithm
	class ChosenEncryptionAlgorithm : public Octet_string {
	public:
		ChosenEncryptionAlgorithm();
		~ChosenEncryptionAlgorithm();

	};

	// ChosenRadioResourceInformation
	class ChosenRadioResourceInformation : public Sequence {
	public:
		ChosenRadioResourceInformation();
		~ChosenRadioResourceInformation();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_chosenChannelInfo();
		void set_chosenSpeechVersion();
		// nodes
		ChosenChannelInfo* _chosenChannelInfo;
		ChosenSpeechVersion* _chosenSpeechVersion;

	};

	// ChosenChannelInfo
	class ChosenChannelInfo : public Octet_string {
	public:
		ChosenChannelInfo();
		~ChosenChannelInfo();

	};

	// ChosenSpeechVersion
	class ChosenSpeechVersion : public Octet_string {
	public:
		ChosenSpeechVersion();
		~ChosenSpeechVersion();

	};

	// PrepareSubsequentHO_Arg
	class PrepareSubsequentHO_Arg : public Sequence {
	public:
		PrepareSubsequentHO_Arg();
		~PrepareSubsequentHO_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_targetCellId();
		void set_targetRNCId();
		void set_an_APDU();
		void set_selectedRab_Id();
		void set_extensionContainer();
		void set_geran_classmark();
		void set_rab_ConfigurationIndicator();
		// nodes
		GlobalCellId* _targetCellId;
		ISDN_AddressString* _targetMSC_Number;
		RNCId* _targetRNCId;
		AccessNetworkSignalInfo* _an_APDU;
		RAB_Id* _selectedRab_Id;
		ExtensionContainer* _extensionContainer;
		GERAN_Classmark* _geran_classmark;
		Null* _rab_ConfigurationIndicator;

	};

	// PrepareSubsequentHO_Res
	class PrepareSubsequentHO_Res : public Sequence {
	public:
		PrepareSubsequentHO_Res();
		~PrepareSubsequentHO_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		AccessNetworkSignalInfo* _an_APDU;
		ExtensionContainer* _extensionContainer;

	};

	// ProcessAccessSignalling_Arg
	class ProcessAccessSignalling_Arg : public Sequence {
	public:
		ProcessAccessSignalling_Arg();
		~ProcessAccessSignalling_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_selectedUMTS_Algorithms();
		void set_selectedGSM_Algorithm();
		void set_chosenRadioResourceInformation();
		void set_selectedRab_Id();
		void set_extensionContainer();
		void set_iUSelectedCodec();
		void set_iuAvailableCodecsList();
		void set_aoipSelectedCodecTarget();
		void set_aoipAvailableCodecsListMap();
		// nodes
		AccessNetworkSignalInfo* _an_APDU;
		SelectedUMTS_Algorithms* _selectedUMTS_Algorithms;
		SelectedGSM_Algorithm* _selectedGSM_Algorithm;
		ChosenRadioResourceInformation* _chosenRadioResourceInformation;
		RAB_Id* _selectedRab_Id;
		ExtensionContainer* _extensionContainer;
		Codec* _iUSelectedCodec;
		CodecList* _iuAvailableCodecsList;
		AoIPCodec* _aoipSelectedCodecTarget;
		AoIPCodecsList* _aoipAvailableCodecsListMap;

	};

	// AoIPCodecsList
	class AoIPCodecsList : public Sequence {
	public:
		AoIPCodecsList();
		~AoIPCodecsList();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_codec2();
		void set_codec3();
		void set_codec4();
		void set_codec5();
		void set_codec6();
		void set_codec7();
		void set_codec8();
		void set_extensionContainer();
		// nodes
		AoIPCodec* _codec1;
		AoIPCodec* _codec2;
		AoIPCodec* _codec3;
		AoIPCodec* _codec4;
		AoIPCodec* _codec5;
		AoIPCodec* _codec6;
		AoIPCodec* _codec7;
		AoIPCodec* _codec8;
		ExtensionContainer* _extensionContainer;

	};

	// AoIPCodec
	class AoIPCodec : public Octet_string {
	public:
		AoIPCodec();
		~AoIPCodec();

	};

	// SupportedCodecsList
	class SupportedCodecsList : public Sequence {
	public:
		SupportedCodecsList();
		~SupportedCodecsList();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_utranCodecList();
		void set_geranCodecList();
		void set_extensionContainer();
		// nodes
		CodecList* _utranCodecList;
		CodecList* _geranCodecList;
		ExtensionContainer* _extensionContainer;

	};

	// CodecList
	class CodecList : public Sequence {
	public:
		CodecList();
		~CodecList();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_codec2();
		void set_codec3();
		void set_codec4();
		void set_codec5();
		void set_codec6();
		void set_codec7();
		void set_codec8();
		void set_extensionContainer();
		// nodes
		Codec* _codec1;
		Codec* _codec2;
		Codec* _codec3;
		Codec* _codec4;
		Codec* _codec5;
		Codec* _codec6;
		Codec* _codec7;
		Codec* _codec8;
		ExtensionContainer* _extensionContainer;

	};

	// Codec
	class Codec : public Octet_string {
	public:
		Codec();
		~Codec();

	};

	// GERAN_Classmark
	class GERAN_Classmark : public Octet_string {
	public:
		GERAN_Classmark();
		~GERAN_Classmark();

	};

	// SelectedGSM_Algorithm
	class SelectedGSM_Algorithm : public Octet_string {
	public:
		SelectedGSM_Algorithm();
		~SelectedGSM_Algorithm();

	};

	// SendEndSignal_Arg
	class SendEndSignal_Arg : public Sequence {
	public:
		SendEndSignal_Arg();
		~SendEndSignal_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		AccessNetworkSignalInfo* _an_APDU;
		ExtensionContainer* _extensionContainer;

	};

	// SendEndSignal_Res
	class SendEndSignal_Res : public Sequence {
	public:
		SendEndSignal_Res();
		~SendEndSignal_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// RNCId
	class RNCId : public Octet_string {
	public:
		RNCId();
		~RNCId();

	};

	// RelocationNumberList
	class RelocationNumberList : public Sequence_of {
	public:
		RelocationNumberList();
		~RelocationNumberList();
		// nodes
		RelocationNumber* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// MulticallBearerInfo
	class MulticallBearerInfo : public Integer {
	public:
		MulticallBearerInfo();
		~MulticallBearerInfo();

	};

	// RelocationNumber
	class RelocationNumber : public Sequence {
	public:
		RelocationNumber();
		~RelocationNumber();
		// nodes
		ISDN_AddressString* _handoverNumber;
		RAB_Id* _rab_Id;

	};

	// RAB_Id
	class RAB_Id : public Integer {
	public:
		RAB_Id();
		~RAB_Id();

	};

	// maxNrOfRABs
	class maxNrOfRABs : public Integer {
	public:
		maxNrOfRABs();
		~maxNrOfRABs();

	};

	// maxNumOfRelocationNumber
	class maxNumOfRelocationNumber : public Integer {
	public:
		maxNumOfRelocationNumber();
		~maxNumOfRelocationNumber();

	};

	// RadioResourceInformation
	class RadioResourceInformation : public Octet_string {
	public:
		RadioResourceInformation();
		~RadioResourceInformation();

	};

	// IntegrityProtectionInformation
	class IntegrityProtectionInformation : public Octet_string {
	public:
		IntegrityProtectionInformation();
		~IntegrityProtectionInformation();

	};

	// maxNumOfIntegrityInfo
	class maxNumOfIntegrityInfo : public Integer {
	public:
		maxNumOfIntegrityInfo();
		~maxNumOfIntegrityInfo();

	};

	// EncryptionInformation
	class EncryptionInformation : public Octet_string {
	public:
		EncryptionInformation();
		~EncryptionInformation();

	};

	// maxNumOfEncryptionInfo
	class maxNumOfEncryptionInfo : public Integer {
	public:
		maxNumOfEncryptionInfo();
		~maxNumOfEncryptionInfo();

	};

	// SendAuthenticationInfoArg
	class SendAuthenticationInfoArg : public Sequence {
	public:
		SendAuthenticationInfoArg();
		~SendAuthenticationInfoArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_segmentationProhibited();
		void set_immediateResponsePreferred();
		void set_re_synchronisationInfo();
		void set_extensionContainer();
		void set_requestingNodeType();
		void set_requestingPLMN_Id();
		void set_numberOfRequestedAdditional_Vectors();
		void set_additionalVectorsAreForEPS();
		// nodes
		IMSI* _imsi;
		NumberOfRequestedVectors* _numberOfRequestedVectors;
		Null* _segmentationProhibited;
		Null* _immediateResponsePreferred;
		Re_synchronisationInfo* _re_synchronisationInfo;
		ExtensionContainer* _extensionContainer;
		RequestingNodeType* _requestingNodeType;
		PLMN_Id* _requestingPLMN_Id;
		NumberOfRequestedVectors* _numberOfRequestedAdditional_Vectors;
		Null* _additionalVectorsAreForEPS;

	};

	// NumberOfRequestedVectors
	class NumberOfRequestedVectors : public Integer {
	public:
		NumberOfRequestedVectors();
		~NumberOfRequestedVectors();

	};

	// Re_synchronisationInfo
	class Re_synchronisationInfo : public Sequence {
	public:
		Re_synchronisationInfo();
		~Re_synchronisationInfo();
		// nodes
		RAND* _rand;
		AUTS* _auts;

	};

	// SendAuthenticationInfoRes
	class SendAuthenticationInfoRes : public Sequence {
	public:
		SendAuthenticationInfoRes();
		~SendAuthenticationInfoRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_authenticationSetList();
		void set_extensionContainer();
		void set_eps_AuthenticationSetList();
		// nodes
		AuthenticationSetList* _authenticationSetList;
		ExtensionContainer* _extensionContainer;
		EPS_AuthenticationSetList* _eps_AuthenticationSetList;

	};

	// EPS_AuthenticationSetList
	class EPS_AuthenticationSetList : public Sequence_of {
	public:
		EPS_AuthenticationSetList();
		~EPS_AuthenticationSetList();
		// nodes
		EPC_AV* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// EPC_AV
	class EPC_AV : public Sequence {
	public:
		EPC_AV();
		~EPC_AV();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		RAND* _rand;
		XRES* _xres;
		AUTN* _autn;
		KASME* _kasme;
		ExtensionContainer* _extensionContainer;

	};

	// KASME
	class KASME : public Octet_string {
	public:
		KASME();
		~KASME();

	};

	// RequestingNodeType
	class RequestingNodeType : public Enumerated {
	public:
		RequestingNodeType();
		~RequestingNodeType();
		static const int _vlr = 0;
		static const int _sgsn = 1;
		static const int _s_cscf = 2;
		static const int _bsf = 3;
		static const int _gan_aaa_server = 4;
		static const int _wlan_aaa_server = 5;
		static const int _mme = 16;
		static const int _mme_sgsn = 17;

	};

	// CheckIMEI_Arg
	class CheckIMEI_Arg : public Sequence {
	public:
		CheckIMEI_Arg();
		~CheckIMEI_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		IMEI* _imei;
		RequestedEquipmentInfo* _requestedEquipmentInfo;
		ExtensionContainer* _extensionContainer;

	};

	// CheckIMEI_Res
	class CheckIMEI_Res : public Sequence {
	public:
		CheckIMEI_Res();
		~CheckIMEI_Res();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_equipmentStatus();
		void set_bmuef();
		void set_extensionContainer();
		// nodes
		EquipmentStatus* _equipmentStatus;
		UESBI_Iu* _bmuef;
		ExtensionContainer* _extensionContainer;

	};

	// RequestedEquipmentInfo
	class RequestedEquipmentInfo : public Bit_string {
	public:
		RequestedEquipmentInfo();
		~RequestedEquipmentInfo();

	};

	// UESBI_Iu
	class UESBI_Iu : public Sequence {
	public:
		UESBI_Iu();
		~UESBI_Iu();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_uesbi_IuA();
		void set_uesbi_IuB();
		// nodes
		UESBI_IuA* _uesbi_IuA;
		UESBI_IuB* _uesbi_IuB;

	};

	// UESBI_IuA
	class UESBI_IuA : public Bit_string {
	public:
		UESBI_IuA();
		~UESBI_IuA();

	};

	// UESBI_IuB
	class UESBI_IuB : public Bit_string {
	public:
		UESBI_IuB();
		~UESBI_IuB();

	};

	// EquipmentStatus
	class EquipmentStatus : public Enumerated {
	public:
		EquipmentStatus();
		~EquipmentStatus();
		static const int _whiteListed = 0;
		static const int _blackListed = 1;
		static const int _greyListed = 2;

	};

	// InsertSubscriberDataArg
	class InsertSubscriberDataArg : public Sequence {
	public:
		InsertSubscriberDataArg();
		~InsertSubscriberDataArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_imsi();
		void set_extensionContainer();
		void set_naea_PreferredCI();
		void set_gprsSubscriptionData();
		void set_roamingRestrictedInSgsnDueToUnsupportedFeature();
		void set_networkAccessMode();
		void set_lsaInformation();
		void set_lmu_Indicator();
		void set_lcsInformation();
		void set_istAlertTimer();
		void set_superChargerSupportedInHLR();
		void set_mc_SS_Info();
		void set_cs_AllocationRetentionPriority();
		void set_sgsn_CAMEL_SubscriptionInfo();
		void set_chargingCharacteristics();
		void set_accessRestrictionData();
		void set_ics_Indicator();
		void set_eps_SubscriptionData();
		void set_csg_SubscriptionDataList();
		void set_ue_ReachabilityRequestIndicator();
		void set_sgsn_Number();
		void set_mme_Name();
		void set_subscribedPeriodicRAUTAUtimer();
		void set_vplmnLIPAAllowed();
		void set_mdtUserConsent();
		void set_subscribedPeriodicLAUtimer();
		void set_vplmn_Csg_SubscriptionDataList();
		void set_additionalMSISDN();
		void set_psAndSMS_OnlyServiceProvision();
		void set_smsInSGSNAllowed();
		// nodes
		IMSI* _imsi;
								ExtensionContainer* _extensionContainer;
		NAEA_PreferredCI* _naea_PreferredCI;
		GPRSSubscriptionData* _gprsSubscriptionData;
		Null* _roamingRestrictedInSgsnDueToUnsupportedFeature;
		NetworkAccessMode* _networkAccessMode;
		LSAInformation* _lsaInformation;
		Null* _lmu_Indicator;
		LCSInformation* _lcsInformation;
		IST_AlertTimerValue* _istAlertTimer;
		AgeIndicator* _superChargerSupportedInHLR;
		MC_SS_Info* _mc_SS_Info;
		CS_AllocationRetentionPriority* _cs_AllocationRetentionPriority;
		SGSN_CAMEL_SubscriptionInfo* _sgsn_CAMEL_SubscriptionInfo;
		ChargingCharacteristics* _chargingCharacteristics;
		AccessRestrictionData* _accessRestrictionData;
		Boolean* _ics_Indicator;
		EPS_SubscriptionData* _eps_SubscriptionData;
		CSG_SubscriptionDataList* _csg_SubscriptionDataList;
		Null* _ue_ReachabilityRequestIndicator;
		ISDN_AddressString* _sgsn_Number;
		DiameterIdentity* _mme_Name;
		SubscribedPeriodicRAUTAUtimer* _subscribedPeriodicRAUTAUtimer;
		Null* _vplmnLIPAAllowed;
		Boolean* _mdtUserConsent;
		SubscribedPeriodicLAUtimer* _subscribedPeriodicLAUtimer;
		VPLMN_CSG_SubscriptionDataList* _vplmn_Csg_SubscriptionDataList;
		ISDN_AddressString* _additionalMSISDN;
		Null* _psAndSMS_OnlyServiceProvision;
		Null* _smsInSGSNAllowed;

	};

	// SubscribedPeriodicRAUTAUtimer
	class SubscribedPeriodicRAUTAUtimer : public Integer {
	public:
		SubscribedPeriodicRAUTAUtimer();
		~SubscribedPeriodicRAUTAUtimer();

	};

	// SubscribedPeriodicLAUtimer
	class SubscribedPeriodicLAUtimer : public Integer {
	public:
		SubscribedPeriodicLAUtimer();
		~SubscribedPeriodicLAUtimer();

	};

	// CSG_SubscriptionDataList
	class CSG_SubscriptionDataList : public Sequence_of {
	public:
		CSG_SubscriptionDataList();
		~CSG_SubscriptionDataList();
		// nodes
		CSG_SubscriptionData* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// CSG_SubscriptionData
	class CSG_SubscriptionData : public Sequence {
	public:
		CSG_SubscriptionData();
		~CSG_SubscriptionData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_expirationDate();
		void set_extensionContainer();
		void set_lipa_AllowedAPNList();
		void set_plmn_Id();
		// nodes
		CSG_Id* _csg_Id;
		Time* _expirationDate;
		ExtensionContainer* _extensionContainer;
		LIPA_AllowedAPNList* _lipa_AllowedAPNList;
		PLMN_Id* _plmn_Id;

	};

	// VPLMN_CSG_SubscriptionDataList
	class VPLMN_CSG_SubscriptionDataList : public Sequence_of {
	public:
		VPLMN_CSG_SubscriptionDataList();
		~VPLMN_CSG_SubscriptionDataList();
		// nodes
		CSG_SubscriptionData* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// CSG_Id
	class CSG_Id : public Bit_string {
	public:
		CSG_Id();
		~CSG_Id();

	};

	// LIPA_AllowedAPNList
	class LIPA_AllowedAPNList : public Sequence_of {
	public:
		LIPA_AllowedAPNList();
		~LIPA_AllowedAPNList();
		// nodes
		APN* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfLIPAAllowedAPN
	class maxNumOfLIPAAllowedAPN : public Integer {
	public:
		maxNumOfLIPAAllowedAPN();
		~maxNumOfLIPAAllowedAPN();

	};

	// EPS_SubscriptionData
	class EPS_SubscriptionData : public Sequence {
	public:
		EPS_SubscriptionData();
		~EPS_SubscriptionData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_apn_oi_Replacement();
		void set_rfsp_id();
		void set_ambr();
		void set_apn_ConfigurationProfile();
		void set_stn_sr();
		void set_extensionContainer();
		void set_mps_CSPriority();
		void set_mps_EPSPriority();
		void set_subscribed_vsrvcc();
		// nodes
		APN_OI_Replacement* _apn_oi_Replacement;
		RFSP_ID* _rfsp_id;
		AMBR* _ambr;
		APN_ConfigurationProfile* _apn_ConfigurationProfile;
		ISDN_AddressString* _stn_sr;
		ExtensionContainer* _extensionContainer;
		Null* _mps_CSPriority;
		Null* _mps_EPSPriority;
		Null* _subscribed_vsrvcc;

	};

	// APN_OI_Replacement
	class APN_OI_Replacement : public Octet_string {
	public:
		APN_OI_Replacement();
		~APN_OI_Replacement();

	};

	// RFSP_ID
	class RFSP_ID : public Integer {
	public:
		RFSP_ID();
		~RFSP_ID();

	};

	// APN_ConfigurationProfile
	class APN_ConfigurationProfile : public Sequence {
	public:
		APN_ConfigurationProfile();
		~APN_ConfigurationProfile();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_completeDataListIncluded();
		void set_extensionContainer();
		// nodes
		ContextId* _defaultContext;
		Null* _completeDataListIncluded;
		EPS_DataList* _epsDataList;
		ExtensionContainer* _extensionContainer;

	};

	// EPS_DataList
	class EPS_DataList : public Sequence_of {
	public:
		EPS_DataList();
		~EPS_DataList();
		// nodes
		APN_Configuration* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfAPN_Configurations
	class maxNumOfAPN_Configurations : public Integer {
	public:
		maxNumOfAPN_Configurations();
		~maxNumOfAPN_Configurations();

	};

	// APN_Configuration
	class APN_Configuration : public Sequence {
	public:
		APN_Configuration();
		~APN_Configuration();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_servedPartyIP_IPv4_Address();
		void set_pdn_gw_Identity();
		void set_pdn_gw_AllocationType();
		void set_vplmnAddressAllowed();
		void set_chargingCharacteristics();
		void set_ambr();
		void set_specificAPNInfoList();
		void set_extensionContainer();
		void set_servedPartyIP_IPv6_Address();
		void set_apn_oi_Replacement();
		void set_sipto_Permission();
		void set_lipa_Permission();
		// nodes
		ContextId* _contextId;
		PDN_Type* _pdn_Type;
		PDP_Address* _servedPartyIP_IPv4_Address;
		APN* _apn;
		EPS_QoS_Subscribed* _eps_qos_Subscribed;
		PDN_GW_Identity* _pdn_gw_Identity;
		PDN_GW_AllocationType* _pdn_gw_AllocationType;
		Null* _vplmnAddressAllowed;
		ChargingCharacteristics* _chargingCharacteristics;
		AMBR* _ambr;
		SpecificAPNInfoList* _specificAPNInfoList;
		ExtensionContainer* _extensionContainer;
		PDP_Address* _servedPartyIP_IPv6_Address;
		APN_OI_Replacement* _apn_oi_Replacement;
		SIPTO_Permission* _sipto_Permission;
		LIPA_Permission* _lipa_Permission;

	};

	// PDN_Type
	class PDN_Type : public Octet_string {
	public:
		PDN_Type();
		~PDN_Type();

	};

	// EPS_QoS_Subscribed
	class EPS_QoS_Subscribed : public Sequence {
	public:
		EPS_QoS_Subscribed();
		~EPS_QoS_Subscribed();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		QoS_Class_Identifier* _qos_Class_Identifier;
		Allocation_Retention_Priority* _allocation_Retention_Priority;
		ExtensionContainer* _extensionContainer;

	};

	// AMBR
	class AMBR : public Sequence {
	public:
		AMBR();
		~AMBR();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		Bandwidth* _max_RequestedBandwidth_UL;
		Bandwidth* _max_RequestedBandwidth_DL;
		ExtensionContainer* _extensionContainer;

	};

	// SpecificAPNInfoList
	class SpecificAPNInfoList : public Sequence_of {
	public:
		SpecificAPNInfoList();
		~SpecificAPNInfoList();
		// nodes
		SpecificAPNInfo* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfSpecificAPNInfos
	class maxNumOfSpecificAPNInfos : public Integer {
	public:
		maxNumOfSpecificAPNInfos();
		~maxNumOfSpecificAPNInfos();

	};

	// SpecificAPNInfo
	class SpecificAPNInfo : public Sequence {
	public:
		SpecificAPNInfo();
		~SpecificAPNInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		APN* _apn;
		PDN_GW_Identity* _pdn_gw_Identity;
		ExtensionContainer* _extensionContainer;

	};

	// Bandwidth
	class Bandwidth : public Integer {
	public:
		Bandwidth();
		~Bandwidth();

	};

	// QoS_Class_Identifier
	class QoS_Class_Identifier : public Integer {
	public:
		QoS_Class_Identifier();
		~QoS_Class_Identifier();

	};

	// Allocation_Retention_Priority
	class Allocation_Retention_Priority : public Sequence {
	public:
		Allocation_Retention_Priority();
		~Allocation_Retention_Priority();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_pre_emption_capability();
		void set_pre_emption_vulnerability();
		void set_extensionContainer();
		// nodes
		Integer* _priority_level;
		Boolean* _pre_emption_capability;
		Boolean* _pre_emption_vulnerability;
		ExtensionContainer* _extensionContainer;

	};

	// PDN_GW_Identity
	class PDN_GW_Identity : public Sequence {
	public:
		PDN_GW_Identity();
		~PDN_GW_Identity();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_pdn_gw_ipv4_Address();
		void set_pdn_gw_ipv6_Address();
		void set_pdn_gw_name();
		void set_extensionContainer();
		// nodes
		PDP_Address* _pdn_gw_ipv4_Address;
		PDP_Address* _pdn_gw_ipv6_Address;
		FQDN* _pdn_gw_name;
		ExtensionContainer* _extensionContainer;

	};

	// FQDN
	class FQDN : public Octet_string {
	public:
		FQDN();
		~FQDN();

	};

	// PDN_GW_AllocationType
	class PDN_GW_AllocationType : public Enumerated {
	public:
		PDN_GW_AllocationType();
		~PDN_GW_AllocationType();
		static const int _static = 0;
		static const int _dynamic = 1;

	};

	// AccessRestrictionData
	class AccessRestrictionData : public Bit_string {
	public:
		AccessRestrictionData();
		~AccessRestrictionData();

	};

	// CS_AllocationRetentionPriority
	class CS_AllocationRetentionPriority : public Octet_string {
	public:
		CS_AllocationRetentionPriority();
		~CS_AllocationRetentionPriority();

	};

	// IST_AlertTimerValue
	class IST_AlertTimerValue : public Integer {
	public:
		IST_AlertTimerValue();
		~IST_AlertTimerValue();

	};

	// LCSInformation
	class LCSInformation : public Sequence {
	public:
		LCSInformation();
		~LCSInformation();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_gmlc_List();
		void set_lcs_PrivacyExceptionList();
		void set_molr_List();
		void set_add_lcs_PrivacyExceptionList();
		// nodes
		GMLC_List* _gmlc_List;
		LCS_PrivacyExceptionList* _lcs_PrivacyExceptionList;
		MOLR_List* _molr_List;
		LCS_PrivacyExceptionList* _add_lcs_PrivacyExceptionList;

	};

	// GMLC_List
	class GMLC_List : public Sequence_of {
	public:
		GMLC_List();
		~GMLC_List();
		// nodes
		ISDN_AddressString* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfGMLC
	class maxNumOfGMLC : public Integer {
	public:
		maxNumOfGMLC();
		~maxNumOfGMLC();

	};

	// NetworkAccessMode
	class NetworkAccessMode : public Enumerated {
	public:
		NetworkAccessMode();
		~NetworkAccessMode();
		static const int _packetAndCircuit = 0;
		static const int _onlyCircuit = 1;
		static const int _onlyPacket = 2;

	};

	// GPRSDataList
	class GPRSDataList : public Sequence_of {
	public:
		GPRSDataList();
		~GPRSDataList();
		// nodes
		PDP_Context* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfPDP_Contexts
	class maxNumOfPDP_Contexts : public Integer {
	public:
		maxNumOfPDP_Contexts();
		~maxNumOfPDP_Contexts();

	};

	// PDP_Context
	class PDP_Context : public Sequence {
	public:
		PDP_Context();
		~PDP_Context();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_pdp_Address();
		void set_vplmnAddressAllowed();
		void set_extensionContainer();
		void set_ext_QoS_Subscribed();
		void set_pdp_ChargingCharacteristics();
		void set_ext2_QoS_Subscribed();
		void set_ext3_QoS_Subscribed();
		void set_ext4_QoS_Subscribed();
		void set_apn_oi_Replacement();
		void set_ext_pdp_Type();
		void set_ext_pdp_Address();
		void set_ambr();
		void set_sipto_Permission();
		void set_lipa_Permission();
		// nodes
		ContextId* _pdp_ContextId;
		PDP_Type* _pdp_Type;
		PDP_Address* _pdp_Address;
		QoS_Subscribed* _qos_Subscribed;
		Null* _vplmnAddressAllowed;
		APN* _apn;
		ExtensionContainer* _extensionContainer;
		Ext_QoS_Subscribed* _ext_QoS_Subscribed;
		ChargingCharacteristics* _pdp_ChargingCharacteristics;
		Ext2_QoS_Subscribed* _ext2_QoS_Subscribed;
		Ext3_QoS_Subscribed* _ext3_QoS_Subscribed;
		Ext4_QoS_Subscribed* _ext4_QoS_Subscribed;
		APN_OI_Replacement* _apn_oi_Replacement;
		Ext_PDP_Type* _ext_pdp_Type;
		PDP_Address* _ext_pdp_Address;
		AMBR* _ambr;
		SIPTO_Permission* _sipto_Permission;
		LIPA_Permission* _lipa_Permission;

	};

	// SIPTO_Permission
	class SIPTO_Permission : public Enumerated {
	public:
		SIPTO_Permission();
		~SIPTO_Permission();
		static const int _siptoAllowed = 0;
		static const int _siptoNotAllowed = 1;
	};

	// LIPA_Permission
	class LIPA_Permission : public Enumerated {
	public:
		LIPA_Permission();
		~LIPA_Permission();
		static const int _lipaProhibited = 0;
		static const int _lipaOnly = 1;
		static const int _lipaConditional = 2;

	};

	// ContextId
	class ContextId : public Integer {
	public:
		ContextId();
		~ContextId();

	};

	// GPRSSubscriptionData
	class GPRSSubscriptionData : public Sequence {
	public:
		GPRSSubscriptionData();
		~GPRSSubscriptionData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_completeDataListIncluded();
		void set_extensionContainer();
		void set_apn_oi_Replacement();
		// nodes
		Null* _completeDataListIncluded;
		GPRSDataList* _gprsDataList;
		ExtensionContainer* _extensionContainer;
		APN_OI_Replacement* _apn_oi_Replacement;

	};

	// SGSN_CAMEL_SubscriptionInfo
	class SGSN_CAMEL_SubscriptionInfo : public Sequence {
	public:
		SGSN_CAMEL_SubscriptionInfo();
		~SGSN_CAMEL_SubscriptionInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_gprs_CSI();
		void set_mo_sms_CSI();
		void set_extensionContainer();
		void set_mt_sms_CSI();
		void set_mt_smsCAMELTDP_CriteriaList();
		void set_mg_csi();
		// nodes
		GPRS_CSI* _gprs_CSI;
		SMS_CSI* _mo_sms_CSI;
		ExtensionContainer* _extensionContainer;
		SMS_CSI* _mt_sms_CSI;
		MT_smsCAMELTDP_CriteriaList* _mt_smsCAMELTDP_CriteriaList;
		MG_CSI* _mg_csi;

	};

	// GPRS_CSI
	class GPRS_CSI : public Sequence {
	public:
		GPRS_CSI();
		~GPRS_CSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_gprs_CamelTDPDataList();
		void set_camelCapabilityHandling();
		void set_extensionContainer();
		void set_notificationToCSE();
		void set_csi_Active();
		// nodes
		GPRS_CamelTDPDataList* _gprs_CamelTDPDataList;
		CamelCapabilityHandling* _camelCapabilityHandling;
		ExtensionContainer* _extensionContainer;
		Null* _notificationToCSE;
		Null* _csi_Active;

	};

	// GPRS_CamelTDPDataList
	class GPRS_CamelTDPDataList : public Sequence_of {
	public:
		GPRS_CamelTDPDataList();
		~GPRS_CamelTDPDataList();
		// nodes
		GPRS_CamelTDPData* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// GPRS_CamelTDPData
	class GPRS_CamelTDPData : public Sequence {
	public:
		GPRS_CamelTDPData();
		~GPRS_CamelTDPData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		GPRS_TriggerDetectionPoint* _gprs_TriggerDetectionPoint;
		ServiceKey* _serviceKey;
		ISDN_AddressString* _gsmSCF_Address;
		DefaultGPRS_Handling* _defaultSessionHandling;
		ExtensionContainer* _extensionContainer;

	};

	// DefaultGPRS_Handling
	class DefaultGPRS_Handling : public Enumerated {
	public:
		DefaultGPRS_Handling();
		~DefaultGPRS_Handling();
		static const int _continueTransaction = 0;
		static const int _releaseTransaction = 1;
	};

	// GPRS_TriggerDetectionPoint
	class GPRS_TriggerDetectionPoint : public Enumerated {
	public:
		GPRS_TriggerDetectionPoint();
		~GPRS_TriggerDetectionPoint();
		static const int _attach = 1;
		static const int _attachChangeOfPosition = 2;
		static const int _pdp_ContextEstablishment = 11;
		static const int _pdp_ContextEstablishmentAcknowledgement = 12;
		static const int _pdp_ContextChangeOfPosition = 14;

	};

	// APN
	class APN : public Octet_string {
	public:
		APN();
		~APN();

	};

	// PDP_Type
	class PDP_Type : public Octet_string {
	public:
		PDP_Type();
		~PDP_Type();

	};

	// Ext_PDP_Type
	class Ext_PDP_Type : public Octet_string {
	public:
		Ext_PDP_Type();
		~Ext_PDP_Type();

	};

	// PDP_Address
	class PDP_Address : public Octet_string {
	public:
		PDP_Address();
		~PDP_Address();

	};

	// QoS_Subscribed
	class QoS_Subscribed : public Octet_string {
	public:
		QoS_Subscribed();
		~QoS_Subscribed();

	};

	// Ext_QoS_Subscribed
	class Ext_QoS_Subscribed : public Octet_string {
	public:
		Ext_QoS_Subscribed();
		~Ext_QoS_Subscribed();

	};

	// Ext2_QoS_Subscribed
	class Ext2_QoS_Subscribed : public Octet_string {
	public:
		Ext2_QoS_Subscribed();
		~Ext2_QoS_Subscribed();

	};

	// Ext3_QoS_Subscribed
	class Ext3_QoS_Subscribed : public Octet_string {
	public:
		Ext3_QoS_Subscribed();
		~Ext3_QoS_Subscribed();

	};

	// Ext4_QoS_Subscribed
	class Ext4_QoS_Subscribed : public Octet_string {
	public:
		Ext4_QoS_Subscribed();
		~Ext4_QoS_Subscribed();

	};

	// ChargingCharacteristics
	class ChargingCharacteristics : public Octet_string {
	public:
		ChargingCharacteristics();
		~ChargingCharacteristics();

	};

	// LSAOnlyAccessIndicator
	class LSAOnlyAccessIndicator : public Enumerated {
	public:
		LSAOnlyAccessIndicator();
		~LSAOnlyAccessIndicator();
		static const int _accessOutsideLSAsAllowed = 0;
		static const int _accessOutsideLSAsRestricted = 1;

	};

	// LSADataList
	class LSADataList : public Sequence_of {
	public:
		LSADataList();
		~LSADataList();
		// nodes
		LSAData* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfLSAs
	class maxNumOfLSAs : public Integer {
	public:
		maxNumOfLSAs();
		~maxNumOfLSAs();

	};

	// LSAData
	class LSAData : public Sequence {
	public:
		LSAData();
		~LSAData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_lsaActiveModeIndicator();
		void set_extensionContainer();
		// nodes
		LSAIdentity* _lsaIdentity;
		LSAAttributes* _lsaAttributes;
		Null* _lsaActiveModeIndicator;
		ExtensionContainer* _extensionContainer;

	};

	// LSAInformation
	class LSAInformation : public Sequence {
	public:
		LSAInformation();
		~LSAInformation();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_completeDataListIncluded();
		void set_lsaOnlyAccessIndicator();
		void set_lsaDataList();
		void set_extensionContainer();
		// nodes
		Null* _completeDataListIncluded;
		LSAOnlyAccessIndicator* _lsaOnlyAccessIndicator;
		LSADataList* _lsaDataList;
		ExtensionContainer* _extensionContainer;

	};

	// LSAIdentity
	class LSAIdentity : public Octet_string {
	public:
		LSAIdentity();
		~LSAIdentity();

	};

	// LSAAttributes
	class LSAAttributes : public Octet_string {
	public:
		LSAAttributes();
		~LSAAttributes();

	};

	// SubscriberData
	class SubscriberData : public Sequence {
	public:
		SubscriberData();
		~SubscriberData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_msisdn();
		void set_category();
		void set_subscriberStatus();
		void set_bearerServiceList();
		void set_teleserviceList();
		void set_provisionedSS();
		void set_odb_Data();
		void set_roamingRestrictionDueToUnsupportedFeature();
		void set_regionalSubscriptionData();
		void set_vbsSubscriptionData();
		void set_vgcsSubscriptionData();
		void set_vlrCamelSubscriptionInfo();
		// nodes
		ISDN_AddressString* _msisdn;
		Category* _category;
		SubscriberStatus* _subscriberStatus;
		BearerServiceList* _bearerServiceList;
		TeleserviceList* _teleserviceList;
		Ext_SS_InfoList* _provisionedSS;
		ODB_Data* _odb_Data;
		Null* _roamingRestrictionDueToUnsupportedFeature;
		ZoneCodeList* _regionalSubscriptionData;
		VBSDataList* _vbsSubscriptionData;
		VGCSDataList* _vgcsSubscriptionData;
		VlrCamelSubscriptionInfo* _vlrCamelSubscriptionInfo;

	};

	// Category
	class Category : public Octet_string {
	public:
		Category();
		~Category();

	};

	// SubscriberStatus
	class SubscriberStatus : public Enumerated {
	public:
		SubscriberStatus();
		~SubscriberStatus();
		static const int _serviceGranted = 0;
		static const int _operatorDeterminedBarring = 1;
	};

	// BearerServiceList
	class BearerServiceList : public Sequence_of {
	public:
		BearerServiceList();
		~BearerServiceList();
		// nodes
		Ext_BearerServiceCode* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfBearerServices
	class maxNumOfBearerServices : public Integer {
	public:
		maxNumOfBearerServices();
		~maxNumOfBearerServices();

	};

	// TeleserviceList
	class TeleserviceList : public Sequence_of {
	public:
		TeleserviceList();
		~TeleserviceList();
		// nodes
		Ext_TeleserviceCode* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfTeleservices
	class maxNumOfTeleservices : public Integer {
	public:
		maxNumOfTeleservices();
		~maxNumOfTeleservices();

	};

	// ODB_Data
	class ODB_Data : public Sequence {
	public:
		ODB_Data();
		~ODB_Data();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_odb_HPLMN_Data();
		void set_extensionContainer();
		// nodes
		ODB_GeneralData* _odb_GeneralData;
		ODB_HPLMN_Data* _odb_HPLMN_Data;
		ExtensionContainer* _extensionContainer;

	};

	// ODB_GeneralData
	class ODB_GeneralData : public Bit_string {
	public:
		ODB_GeneralData();
		~ODB_GeneralData();

	};

	// ODB_HPLMN_Data
	class ODB_HPLMN_Data : public Bit_string {
	public:
		ODB_HPLMN_Data();
		~ODB_HPLMN_Data();

	};

	// Ext_SS_InfoList
	class Ext_SS_InfoList : public Sequence_of {
	public:
		Ext_SS_InfoList();
		~Ext_SS_InfoList();
		// nodes
		Ext_SS_Info* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// Ext_SS_Info
	class Ext_SS_Info : public Choice {
	public:
		Ext_SS_Info();
		~Ext_SS_Info();
		// nodes
		Ext_ForwInfo* _forwardingInfo;
		Ext_CallBarInfo* _callBarringInfo;
		CUG_Info* _cug_Info;
		Ext_SS_Data* _ss_Data;
		EMLPP_Info* _emlpp_Info;

	};

	// Ext_ForwInfo
	class Ext_ForwInfo : public Sequence {
	public:
		Ext_ForwInfo();
		~Ext_ForwInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SS_Code* _ss_Code;
		Ext_ForwFeatureList* _forwardingFeatureList;
		ExtensionContainer* _extensionContainer;

	};

	// Ext_ForwFeatureList
	class Ext_ForwFeatureList : public Sequence_of {
	public:
		Ext_ForwFeatureList();
		~Ext_ForwFeatureList();
		// nodes
		Ext_ForwFeature* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// Ext_ForwFeature
	class Ext_ForwFeature : public Sequence {
	public:
		Ext_ForwFeature();
		~Ext_ForwFeature();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_forwardedToNumber();
		void set_forwardedToSubaddress();
		void set_forwardingOptions();
		void set_noReplyConditionTime();
		void set_extensionContainer();
		void set_longForwardedToNumber();
		// nodes
		Ext_BasicServiceCode* _basicService;
		Ext_SS_Status* _ss_Status;
		ISDN_AddressString* _forwardedToNumber;
		ISDN_SubaddressString* _forwardedToSubaddress;
		Ext_ForwOptions* _forwardingOptions;
		Ext_NoRepCondTime* _noReplyConditionTime;
		ExtensionContainer* _extensionContainer;
		FTN_AddressString* _longForwardedToNumber;

	};

	// Ext_ForwOptions
	class Ext_ForwOptions : public Octet_string {
	public:
		Ext_ForwOptions();
		~Ext_ForwOptions();

	};

	// Ext_NoRepCondTime
	class Ext_NoRepCondTime : public Integer {
	public:
		Ext_NoRepCondTime();
		~Ext_NoRepCondTime();

	};

	// Ext_CallBarInfo
	class Ext_CallBarInfo : public Sequence {
	public:
		Ext_CallBarInfo();
		~Ext_CallBarInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SS_Code* _ss_Code;
		Ext_CallBarFeatureList* _callBarringFeatureList;
		ExtensionContainer* _extensionContainer;

	};

	// Ext_CallBarFeatureList
	class Ext_CallBarFeatureList : public Sequence_of {
	public:
		Ext_CallBarFeatureList();
		~Ext_CallBarFeatureList();
		// nodes
		Ext_CallBarringFeature* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// Ext_CallBarringFeature
	class Ext_CallBarringFeature : public Sequence {
	public:
		Ext_CallBarringFeature();
		~Ext_CallBarringFeature();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_extensionContainer();
		// nodes
		Ext_BasicServiceCode* _basicService;
		Ext_SS_Status* _ss_Status;
		ExtensionContainer* _extensionContainer;

	};

	// CUG_Info
	class CUG_Info : public Sequence {
	public:
		CUG_Info();
		~CUG_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cug_FeatureList();
		void set_extensionContainer();
		// nodes
		CUG_SubscriptionList* _cug_SubscriptionList;
		CUG_FeatureList* _cug_FeatureList;
		ExtensionContainer* _extensionContainer;

	};

	// CUG_SubscriptionList
	class CUG_SubscriptionList : public Sequence_of {
	public:
		CUG_SubscriptionList();
		~CUG_SubscriptionList();
		// nodes
		CUG_Subscription* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// CUG_Subscription
	class CUG_Subscription : public Sequence {
	public:
		CUG_Subscription();
		~CUG_Subscription();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicServiceGroupList();
		void set_extensionContainer();
		// nodes
		CUG_Index* _cug_Index;
		CUG_Interlock* _cug_Interlock;
		IntraCUG_Options* _intraCUG_Options;
		Ext_BasicServiceGroupList* _basicServiceGroupList;
		ExtensionContainer* _extensionContainer;

	};

	// CUG_Index
	class CUG_Index : public Integer {
	public:
		CUG_Index();
		~CUG_Index();

	};

	// CUG_Interlock
	class CUG_Interlock : public Octet_string {
	public:
		CUG_Interlock();
		~CUG_Interlock();

	};

	// IntraCUG_Options
	class IntraCUG_Options : public Enumerated {
	public:
		IntraCUG_Options();
		~IntraCUG_Options();
		static const int _noCUG_Restrictions = 0;
		static const int _cugIC_CallBarred = 1;
		static const int _cugOG_CallBarred = 2;

	};

	// maxNumOfCUG
	class maxNumOfCUG : public Integer {
	public:
		maxNumOfCUG();
		~maxNumOfCUG();

	};

	// CUG_FeatureList
	class CUG_FeatureList : public Sequence_of {
	public:
		CUG_FeatureList();
		~CUG_FeatureList();
		// nodes
		CUG_Feature* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// Ext_BasicServiceGroupList
	class Ext_BasicServiceGroupList : public Sequence_of {
	public:
		Ext_BasicServiceGroupList();
		~Ext_BasicServiceGroupList();
		// nodes
		Ext_BasicServiceCode* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfExt_BasicServiceGroups
	class maxNumOfExt_BasicServiceGroups : public Integer {
	public:
		maxNumOfExt_BasicServiceGroups();
		~maxNumOfExt_BasicServiceGroups();

	};

	// CUG_Feature
	class CUG_Feature : public Sequence {
	public:
		CUG_Feature();
		~CUG_Feature();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_preferentialCUG_Indicator();
		void set_extensionContainer();
		// nodes
		Ext_BasicServiceCode* _basicService;
		CUG_Index* _preferentialCUG_Indicator;
		InterCUG_Restrictions* _interCUG_Restrictions;
		ExtensionContainer* _extensionContainer;

	};

	// InterCUG_Restrictions
	class InterCUG_Restrictions : public Octet_string {
	public:
		InterCUG_Restrictions();
		~InterCUG_Restrictions();

	};

	// Ext_SS_Data
	class Ext_SS_Data : public Sequence {
	public:
		Ext_SS_Data();
		~Ext_SS_Data();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_SubscriptionOption();
		void set_basicServiceGroupList();
		void set_extensionContainer();
		// nodes
		SS_Code* _ss_Code;
		Ext_SS_Status* _ss_Status;
		SS_SubscriptionOption* _ss_SubscriptionOption;
		Ext_BasicServiceGroupList* _basicServiceGroupList;
		ExtensionContainer* _extensionContainer;

	};

	// LCS_PrivacyExceptionList
	class LCS_PrivacyExceptionList : public Sequence_of {
	public:
		LCS_PrivacyExceptionList();
		~LCS_PrivacyExceptionList();
		// nodes
		LCS_PrivacyClass* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfPrivacyClass
	class maxNumOfPrivacyClass : public Integer {
	public:
		maxNumOfPrivacyClass();
		~maxNumOfPrivacyClass();

	};

	// LCS_PrivacyClass
	class LCS_PrivacyClass : public Sequence {
	public:
		LCS_PrivacyClass();
		~LCS_PrivacyClass();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_notificationToMSUser();
		void set_externalClientList();
		void set_plmnClientList();
		void set_extensionContainer();
		void set_ext_externalClientList();
		void set_serviceTypeList();
		// nodes
		SS_Code* _ss_Code;
		Ext_SS_Status* _ss_Status;
		NotificationToMSUser* _notificationToMSUser;
		ExternalClientList* _externalClientList;
		PLMNClientList* _plmnClientList;
		ExtensionContainer* _extensionContainer;
		Ext_ExternalClientList* _ext_externalClientList;
		ServiceTypeList* _serviceTypeList;

	};

	// ExternalClientList
	class ExternalClientList : public Sequence_of {
	public:
		ExternalClientList();
		~ExternalClientList();
		// nodes
		ExternalClient* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfExternalClient
	class maxNumOfExternalClient : public Integer {
	public:
		maxNumOfExternalClient();
		~maxNumOfExternalClient();

	};

	// PLMNClientList
	class PLMNClientList : public Sequence_of {
	public:
		PLMNClientList();
		~PLMNClientList();
		// nodes
		LCSClientInternalID* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfPLMNClient
	class maxNumOfPLMNClient : public Integer {
	public:
		maxNumOfPLMNClient();
		~maxNumOfPLMNClient();

	};

	// Ext_ExternalClientList
	class Ext_ExternalClientList : public Sequence_of {
	public:
		Ext_ExternalClientList();
		~Ext_ExternalClientList();
		// nodes
		ExternalClient* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfExt_ExternalClient
	class maxNumOfExt_ExternalClient : public Integer {
	public:
		maxNumOfExt_ExternalClient();
		~maxNumOfExt_ExternalClient();

	};

	// ExternalClient
	class ExternalClient : public Sequence {
	public:
		ExternalClient();
		~ExternalClient();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_gmlc_Restriction();
		void set_notificationToMSUser();
		void set_extensionContainer();
		// nodes
		LCSClientExternalID* _clientIdentity;
		GMLC_Restriction* _gmlc_Restriction;
		NotificationToMSUser* _notificationToMSUser;
		ExtensionContainer* _extensionContainer;

	};

	// GMLC_Restriction
	class GMLC_Restriction : public Enumerated {
	public:
		GMLC_Restriction();
		~GMLC_Restriction();
		static const int _gmlc_List = 0;
		static const int _home_Country = 1;

	};

	// NotificationToMSUser
	class NotificationToMSUser : public Enumerated {
	public:
		NotificationToMSUser();
		~NotificationToMSUser();
		static const int _notifyLocationAllowed = 0;
		static const int _notifyAndVerify_LocationAllowedIfNoResponse = 1;
		static const int _notifyAndVerify_LocationNotAllowedIfNoResponse = 2;
		static const int _locationNotAllowed = 3;

	};

	// ServiceTypeList
	class ServiceTypeList : public Sequence_of {
	public:
		ServiceTypeList();
		~ServiceTypeList();
		// nodes
		ServiceType* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfServiceType
	class maxNumOfServiceType : public Integer {
	public:
		maxNumOfServiceType();
		~maxNumOfServiceType();

	};

	// ServiceType
	class ServiceType : public Sequence {
	public:
		ServiceType();
		~ServiceType();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_gmlc_Restriction();
		void set_notificationToMSUser();
		void set_extensionContainer();
		// nodes
		LCSServiceTypeID* _serviceTypeIdentity;
		GMLC_Restriction* _gmlc_Restriction;
		NotificationToMSUser* _notificationToMSUser;
		ExtensionContainer* _extensionContainer;

	};

	// MOLR_List
	class MOLR_List : public Sequence_of {
	public:
		MOLR_List();
		~MOLR_List();
		// nodes
		MOLR_Class* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfMOLR_Class
	class maxNumOfMOLR_Class : public Integer {
	public:
		maxNumOfMOLR_Class();
		~maxNumOfMOLR_Class();

	};

	// MOLR_Class
	class MOLR_Class : public Sequence {
	public:
		MOLR_Class();
		~MOLR_Class();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SS_Code* _ss_Code;
		Ext_SS_Status* _ss_Status;
		ExtensionContainer* _extensionContainer;

	};

	// ZoneCodeList
	class ZoneCodeList : public Sequence_of {
	public:
		ZoneCodeList();
		~ZoneCodeList();
		// nodes
		ZoneCode* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// ZoneCode
	class ZoneCode : public Octet_string {
	public:
		ZoneCode();
		~ZoneCode();

	};

	// maxNumOfZoneCodes
	class maxNumOfZoneCodes : public Integer {
	public:
		maxNumOfZoneCodes();
		~maxNumOfZoneCodes();

	};

	// InsertSubscriberDataRes
	class InsertSubscriberDataRes : public Sequence {
	public:
		InsertSubscriberDataRes();
		~InsertSubscriberDataRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_teleserviceList();
		void set_bearerServiceList();
		void set_ss_List();
		void set_odb_GeneralData();
		void set_regionalSubscriptionResponse();
		void set_supportedCamelPhases();
		void set_extensionContainer();
		void set_offeredCamel4CSIs();
		void set_supportedFeatures();
		// nodes
		TeleserviceList* _teleserviceList;
		BearerServiceList* _bearerServiceList;
		SS_List* _ss_List;
		ODB_GeneralData* _odb_GeneralData;
		RegionalSubscriptionResponse* _regionalSubscriptionResponse;
		SupportedCamelPhases* _supportedCamelPhases;
		ExtensionContainer* _extensionContainer;
		OfferedCamel4CSIs* _offeredCamel4CSIs;
		SupportedFeatures* _supportedFeatures;

	};

	// RegionalSubscriptionResponse
	class RegionalSubscriptionResponse : public Enumerated {
	public:
		RegionalSubscriptionResponse();
		~RegionalSubscriptionResponse();
		static const int _networkNode_AreaRestricted = 0;
		static const int _tooManyZoneCodes = 1;
		static const int _zoneCodesConflict = 2;
		static const int _regionalSubscNotSupported = 3;

	};

	// DeleteSubscriberDataArg
	class DeleteSubscriberDataArg : public Sequence {
	public:
		DeleteSubscriberDataArg();
		~DeleteSubscriberDataArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicServiceList();
		void set_ss_List();
		void set_roamingRestrictionDueToUnsupportedFeature();
		void set_regionalSubscriptionIdentifier();
		void set_vbsGroupIndication();
		void set_vgcsGroupIndication();
		void set_camelSubscriptionInfoWithdraw();
		void set_extensionContainer();
		void set_gprsSubscriptionDataWithdraw();
		void set_roamingRestrictedInSgsnDueToUnsuppportedFeature();
		void set_lsaInformationWithdraw();
		void set_gmlc_ListWithdraw();
		void set_istInformationWithdraw();
		void set_specificCSI_Withdraw();
		void set_chargingCharacteristicsWithdraw();
		void set_stn_srWithdraw();
		void set_epsSubscriptionDataWithdraw();
		void set_apn_oi_replacementWithdraw();
		void set_csg_SubscriptionDeleted();
		void set_subscribedPeriodicTAU_RAU_TimerWithdraw();
		void set_subscribedPeriodicLAU_TimerWithdraw();
		void set_subscribed_vsrvccWithdraw();
		void set_vplmn_Csg_SubscriptionDeleted();
		void set_additionalMSISDN_Withdraw();
		// nodes
		IMSI* _imsi;
		BasicServiceList* _basicServiceList;
		SS_List* _ss_List;
		Null* _roamingRestrictionDueToUnsupportedFeature;
		ZoneCode* _regionalSubscriptionIdentifier;
		Null* _vbsGroupIndication;
		Null* _vgcsGroupIndication;
		Null* _camelSubscriptionInfoWithdraw;
		ExtensionContainer* _extensionContainer;
		GPRSSubscriptionDataWithdraw* _gprsSubscriptionDataWithdraw;
		Null* _roamingRestrictedInSgsnDueToUnsuppportedFeature;
		LSAInformationWithdraw* _lsaInformationWithdraw;
		Null* _gmlc_ListWithdraw;
		Null* _istInformationWithdraw;
		SpecificCSI_Withdraw* _specificCSI_Withdraw;
		Null* _chargingCharacteristicsWithdraw;
		Null* _stn_srWithdraw;
		EPS_SubscriptionDataWithdraw* _epsSubscriptionDataWithdraw;
		Null* _apn_oi_replacementWithdraw;
		Null* _csg_SubscriptionDeleted;
		Null* _subscribedPeriodicTAU_RAU_TimerWithdraw;
		Null* _subscribedPeriodicLAU_TimerWithdraw;
		Null* _subscribed_vsrvccWithdraw;
		Null* _vplmn_Csg_SubscriptionDeleted;
		Null* _additionalMSISDN_Withdraw;

	};

	// SpecificCSI_Withdraw
	class SpecificCSI_Withdraw : public Bit_string {
	public:
		SpecificCSI_Withdraw();
		~SpecificCSI_Withdraw();

	};

	// GPRSSubscriptionDataWithdraw
	class GPRSSubscriptionDataWithdraw : public Choice {
	public:
		GPRSSubscriptionDataWithdraw();
		~GPRSSubscriptionDataWithdraw();
		// nodes
		Null* _allGPRSData;
		ContextIdList* _contextIdList;

	};

	// EPS_SubscriptionDataWithdraw
	class EPS_SubscriptionDataWithdraw : public Choice {
	public:
		EPS_SubscriptionDataWithdraw();
		~EPS_SubscriptionDataWithdraw();
		// nodes
		Null* _allEPS_Data;
		ContextIdList* _contextIdList;

	};

	// ContextIdList
	class ContextIdList : public Sequence_of {
	public:
		ContextIdList();
		~ContextIdList();
		// nodes
		ContextId* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// LSAInformationWithdraw
	class LSAInformationWithdraw : public Choice {
	public:
		LSAInformationWithdraw();
		~LSAInformationWithdraw();
		// nodes
		Null* _allLSAData;
		LSAIdentityList* _lsaIdentityList;

	};

	// LSAIdentityList
	class LSAIdentityList : public Sequence_of {
	public:
		LSAIdentityList();
		~LSAIdentityList();
		// nodes
		LSAIdentity* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// BasicServiceList
	class BasicServiceList : public Sequence_of {
	public:
		BasicServiceList();
		~BasicServiceList();
		// nodes
		Ext_BasicServiceCode* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfBasicServices
	class maxNumOfBasicServices : public Integer {
	public:
		maxNumOfBasicServices();
		~maxNumOfBasicServices();

	};

	// DeleteSubscriberDataRes
	class DeleteSubscriberDataRes : public Sequence {
	public:
		DeleteSubscriberDataRes();
		~DeleteSubscriberDataRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_regionalSubscriptionResponse();
		void set_extensionContainer();
		// nodes
		RegionalSubscriptionResponse* _regionalSubscriptionResponse;
		ExtensionContainer* _extensionContainer;

	};

	// VlrCamelSubscriptionInfo
	class VlrCamelSubscriptionInfo : public Sequence {
	public:
		VlrCamelSubscriptionInfo();
		~VlrCamelSubscriptionInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_o_CSI();
		void set_extensionContainer();
		void set_ss_CSI();
		void set_o_BcsmCamelTDP_CriteriaList();
		void set_tif_CSI();
		void set_m_CSI();
		void set_mo_sms_CSI();
		void set_vt_CSI();
		void set_t_BCSM_CAMEL_TDP_CriteriaList();
		void set_d_CSI();
		void set_mt_sms_CSI();
		void set_mt_smsCAMELTDP_CriteriaList();
		// nodes
		O_CSI* _o_CSI;
		ExtensionContainer* _extensionContainer;
		SS_CSI* _ss_CSI;
		O_BcsmCamelTDPCriteriaList* _o_BcsmCamelTDP_CriteriaList;
		Null* _tif_CSI;
		M_CSI* _m_CSI;
		SMS_CSI* _mo_sms_CSI;
		T_CSI* _vt_CSI;
		T_BCSM_CAMEL_TDP_CriteriaList* _t_BCSM_CAMEL_TDP_CriteriaList;
		D_CSI* _d_CSI;
		SMS_CSI* _mt_sms_CSI;
		MT_smsCAMELTDP_CriteriaList* _mt_smsCAMELTDP_CriteriaList;

	};

	// MT_smsCAMELTDP_CriteriaList
	class MT_smsCAMELTDP_CriteriaList : public Sequence_of {
	public:
		MT_smsCAMELTDP_CriteriaList();
		~MT_smsCAMELTDP_CriteriaList();
		// nodes
		MT_smsCAMELTDP_Criteria* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// MT_smsCAMELTDP_Criteria
	class MT_smsCAMELTDP_Criteria : public Sequence {
	public:
		MT_smsCAMELTDP_Criteria();
		~MT_smsCAMELTDP_Criteria();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_tpdu_TypeCriterion();
		// nodes
		SMS_TriggerDetectionPoint* _sms_TriggerDetectionPoint;
		TPDU_TypeCriterion* _tpdu_TypeCriterion;

	};

	// TPDU_TypeCriterion
	class TPDU_TypeCriterion : public Sequence_of {
	public:
		TPDU_TypeCriterion();
		~TPDU_TypeCriterion();
		// nodes
		MT_SMS_TPDU_Type* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfTPDUTypes
	class maxNumOfTPDUTypes : public Integer {
	public:
		maxNumOfTPDUTypes();
		~maxNumOfTPDUTypes();

	};

	// MT_SMS_TPDU_Type
	class MT_SMS_TPDU_Type : public Enumerated {
	public:
		MT_SMS_TPDU_Type();
		~MT_SMS_TPDU_Type();
		static const int _sms_DELIVER = 0;
		static const int _sms_SUBMIT_REPORT = 1;
		static const int _sms_STATUS_REPORT = 2;

	};

	// D_CSI
	class D_CSI : public Sequence {
	public:
		D_CSI();
		~D_CSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_dp_AnalysedInfoCriteriaList();
		void set_camelCapabilityHandling();
		void set_extensionContainer();
		void set_notificationToCSE();
		void set_csi_Active();
		// nodes
		DP_AnalysedInfoCriteriaList* _dp_AnalysedInfoCriteriaList;
		CamelCapabilityHandling* _camelCapabilityHandling;
		ExtensionContainer* _extensionContainer;
		Null* _notificationToCSE;
		Null* _csi_Active;

	};

	// DP_AnalysedInfoCriteriaList
	class DP_AnalysedInfoCriteriaList : public Sequence_of {
	public:
		DP_AnalysedInfoCriteriaList();
		~DP_AnalysedInfoCriteriaList();
		// nodes
		DP_AnalysedInfoCriterium* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfDP_AnalysedInfoCriteria
	class maxNumOfDP_AnalysedInfoCriteria : public Integer {
	public:
		maxNumOfDP_AnalysedInfoCriteria();
		~maxNumOfDP_AnalysedInfoCriteria();

	};

	// DP_AnalysedInfoCriterium
	class DP_AnalysedInfoCriterium : public Sequence {
	public:
		DP_AnalysedInfoCriterium();
		~DP_AnalysedInfoCriterium();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ISDN_AddressString* _dialledNumber;
		ServiceKey* _serviceKey;
		ISDN_AddressString* _gsmSCF_Address;
		DefaultCallHandling* _defaultCallHandling;
		ExtensionContainer* _extensionContainer;

	};

	// SS_CSI
	class SS_CSI : public Sequence {
	public:
		SS_CSI();
		~SS_CSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_notificationToCSE();
		void set_csi_Active();
		// nodes
		SS_CamelData* _ss_CamelData;
		ExtensionContainer* _extensionContainer;
		Null* _notificationToCSE;
		Null* _csi_Active;

	};

	// SS_CamelData
	class SS_CamelData : public Sequence {
	public:
		SS_CamelData();
		~SS_CamelData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SS_EventList* _ss_EventList;
		ISDN_AddressString* _gsmSCF_Address;
		ExtensionContainer* _extensionContainer;

	};

	// SS_EventList
	class SS_EventList : public Sequence_of {
	public:
		SS_EventList();
		~SS_EventList();
		// nodes
		SS_Code* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfCamelSSEvents
	class maxNumOfCamelSSEvents : public Integer {
	public:
		maxNumOfCamelSSEvents();
		~maxNumOfCamelSSEvents();

	};

	// O_CSI
	class O_CSI : public Sequence {
	public:
		O_CSI();
		~O_CSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_camelCapabilityHandling();
		void set_notificationToCSE();
		void set_csiActive();
		// nodes
		O_BcsmCamelTDPDataList* _o_BcsmCamelTDPDataList;
		ExtensionContainer* _extensionContainer;
		CamelCapabilityHandling* _camelCapabilityHandling;
		Null* _notificationToCSE;
		Null* _csiActive;

	};

	// O_BcsmCamelTDPDataList
	class O_BcsmCamelTDPDataList : public Sequence_of {
	public:
		O_BcsmCamelTDPDataList();
		~O_BcsmCamelTDPDataList();
		// nodes
		O_BcsmCamelTDPData* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfCamelTDPData
	class maxNumOfCamelTDPData : public Integer {
	public:
		maxNumOfCamelTDPData();
		~maxNumOfCamelTDPData();

	};

	// O_BcsmCamelTDPData
	class O_BcsmCamelTDPData : public Sequence {
	public:
		O_BcsmCamelTDPData();
		~O_BcsmCamelTDPData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		O_BcsmTriggerDetectionPoint* _o_BcsmTriggerDetectionPoint;
		ServiceKey* _serviceKey;
		ISDN_AddressString* _gsmSCF_Address;
		DefaultCallHandling* _defaultCallHandling;
		ExtensionContainer* _extensionContainer;

	};

	// ServiceKey
	class ServiceKey : public Integer {
	public:
		ServiceKey();
		~ServiceKey();

	};

	// O_BcsmTriggerDetectionPoint
	class O_BcsmTriggerDetectionPoint : public Enumerated {
	public:
		O_BcsmTriggerDetectionPoint();
		~O_BcsmTriggerDetectionPoint();
		static const int _collectedInfo = 2;
		static const int _routeSelectFailure = 4;
	};

	// O_BcsmCamelTDPCriteriaList
	class O_BcsmCamelTDPCriteriaList : public Sequence_of {
	public:
		O_BcsmCamelTDPCriteriaList();
		~O_BcsmCamelTDPCriteriaList();
		// nodes
		O_BcsmCamelTDP_Criteria* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// T_BCSM_CAMEL_TDP_CriteriaList
	class T_BCSM_CAMEL_TDP_CriteriaList : public Sequence_of {
	public:
		T_BCSM_CAMEL_TDP_CriteriaList();
		~T_BCSM_CAMEL_TDP_CriteriaList();
		// nodes
		T_BCSM_CAMEL_TDP_Criteria* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// O_BcsmCamelTDP_Criteria
	class O_BcsmCamelTDP_Criteria : public Sequence {
	public:
		O_BcsmCamelTDP_Criteria();
		~O_BcsmCamelTDP_Criteria();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_destinationNumberCriteria();
		void set_basicServiceCriteria();
		void set_callTypeCriteria();
		void set_o_CauseValueCriteria();
		void set_extensionContainer();
		// nodes
		O_BcsmTriggerDetectionPoint* _o_BcsmTriggerDetectionPoint;
		DestinationNumberCriteria* _destinationNumberCriteria;
		BasicServiceCriteria* _basicServiceCriteria;
		CallTypeCriteria* _callTypeCriteria;
		O_CauseValueCriteria* _o_CauseValueCriteria;
		ExtensionContainer* _extensionContainer;

	};

	// T_BCSM_CAMEL_TDP_Criteria
	class T_BCSM_CAMEL_TDP_Criteria : public Sequence {
	public:
		T_BCSM_CAMEL_TDP_Criteria();
		~T_BCSM_CAMEL_TDP_Criteria();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicServiceCriteria();
		void set_t_CauseValueCriteria();
		// nodes
		T_BcsmTriggerDetectionPoint* _t_BCSM_TriggerDetectionPoint;
		BasicServiceCriteria* _basicServiceCriteria;
		T_CauseValueCriteria* _t_CauseValueCriteria;

	};

	// DestinationNumberCriteria
	class DestinationNumberCriteria : public Sequence {
	public:
		DestinationNumberCriteria();
		~DestinationNumberCriteria();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_destinationNumberList();
		void set_destinationNumberLengthList();
		// nodes
		MatchType* _matchType;
		DestinationNumberList* _destinationNumberList;
		DestinationNumberLengthList* _destinationNumberLengthList;

	};

	// DestinationNumberList
	class DestinationNumberList : public Sequence_of {
	public:
		DestinationNumberList();
		~DestinationNumberList();
		// nodes
		ISDN_AddressString* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// DestinationNumberLengthList
	class DestinationNumberLengthList : public Sequence_of {
	public:
		DestinationNumberLengthList();
		~DestinationNumberLengthList();
		// nodes
		Integer* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// BasicServiceCriteria
	class BasicServiceCriteria : public Sequence_of {
	public:
		BasicServiceCriteria();
		~BasicServiceCriteria();
		// nodes
		Ext_BasicServiceCode* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfISDN_AddressDigits
	class maxNumOfISDN_AddressDigits : public Integer {
	public:
		maxNumOfISDN_AddressDigits();
		~maxNumOfISDN_AddressDigits();

	};

	// maxNumOfCamelDestinationNumbers
	class maxNumOfCamelDestinationNumbers : public Integer {
	public:
		maxNumOfCamelDestinationNumbers();
		~maxNumOfCamelDestinationNumbers();

	};

	// maxNumOfCamelDestinationNumberLengths
	class maxNumOfCamelDestinationNumberLengths : public Integer {
	public:
		maxNumOfCamelDestinationNumberLengths();
		~maxNumOfCamelDestinationNumberLengths();

	};

	// maxNumOfCamelBasicServiceCriteria
	class maxNumOfCamelBasicServiceCriteria : public Integer {
	public:
		maxNumOfCamelBasicServiceCriteria();
		~maxNumOfCamelBasicServiceCriteria();

	};

	// CallTypeCriteria
	class CallTypeCriteria : public Enumerated {
	public:
		CallTypeCriteria();
		~CallTypeCriteria();
		static const int _forwarded = 0;
		static const int _notForwarded = 1;
	};

	// MatchType
	class MatchType : public Enumerated {
	public:
		MatchType();
		~MatchType();
		static const int _inhibiting = 0;
		static const int _enabling = 1;

	};

	// O_CauseValueCriteria
	class O_CauseValueCriteria : public Sequence_of {
	public:
		O_CauseValueCriteria();
		~O_CauseValueCriteria();
		// nodes
		CauseValue* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// T_CauseValueCriteria
	class T_CauseValueCriteria : public Sequence_of {
	public:
		T_CauseValueCriteria();
		~T_CauseValueCriteria();
		// nodes
		CauseValue* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfCAMEL_O_CauseValueCriteria
	class maxNumOfCAMEL_O_CauseValueCriteria : public Integer {
	public:
		maxNumOfCAMEL_O_CauseValueCriteria();
		~maxNumOfCAMEL_O_CauseValueCriteria();

	};

	// maxNumOfCAMEL_T_CauseValueCriteria
	class maxNumOfCAMEL_T_CauseValueCriteria : public Integer {
	public:
		maxNumOfCAMEL_T_CauseValueCriteria();
		~maxNumOfCAMEL_T_CauseValueCriteria();

	};

	// CauseValue
	class CauseValue : public Octet_string {
	public:
		CauseValue();
		~CauseValue();

	};

	// DefaultCallHandling
	class DefaultCallHandling : public Enumerated {
	public:
		DefaultCallHandling();
		~DefaultCallHandling();
		static const int _continueCall = 0;
		static const int _releaseCall = 1;
	};

	// CamelCapabilityHandling
	class CamelCapabilityHandling : public Integer {
	public:
		CamelCapabilityHandling();
		~CamelCapabilityHandling();

	};

	// SupportedCamelPhases
	class SupportedCamelPhases : public Bit_string {
	public:
		SupportedCamelPhases();
		~SupportedCamelPhases();

	};

	// OfferedCamel4CSIs
	class OfferedCamel4CSIs : public Bit_string {
	public:
		OfferedCamel4CSIs();
		~OfferedCamel4CSIs();

	};

	// OfferedCamel4Functionalities
	class OfferedCamel4Functionalities : public Bit_string {
	public:
		OfferedCamel4Functionalities();
		~OfferedCamel4Functionalities();

	};

	// SMS_CSI
	class SMS_CSI : public Sequence {
	public:
		SMS_CSI();
		~SMS_CSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_sms_CAMEL_TDP_DataList();
		void set_camelCapabilityHandling();
		void set_extensionContainer();
		void set_notificationToCSE();
		void set_csi_Active();
		// nodes
		SMS_CAMEL_TDP_DataList* _sms_CAMEL_TDP_DataList;
		CamelCapabilityHandling* _camelCapabilityHandling;
		ExtensionContainer* _extensionContainer;
		Null* _notificationToCSE;
		Null* _csi_Active;

	};

	// SMS_CAMEL_TDP_DataList
	class SMS_CAMEL_TDP_DataList : public Sequence_of {
	public:
		SMS_CAMEL_TDP_DataList();
		~SMS_CAMEL_TDP_DataList();
		// nodes
		SMS_CAMEL_TDP_Data* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// SMS_CAMEL_TDP_Data
	class SMS_CAMEL_TDP_Data : public Sequence {
	public:
		SMS_CAMEL_TDP_Data();
		~SMS_CAMEL_TDP_Data();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SMS_TriggerDetectionPoint* _sms_TriggerDetectionPoint;
		ServiceKey* _serviceKey;
		ISDN_AddressString* _gsmSCF_Address;
		DefaultSMS_Handling* _defaultSMS_Handling;
		ExtensionContainer* _extensionContainer;

	};

	// SMS_TriggerDetectionPoint
	class SMS_TriggerDetectionPoint : public Enumerated {
	public:
		SMS_TriggerDetectionPoint();
		~SMS_TriggerDetectionPoint();
		static const int _sms_CollectedInfo = 1;
		static const int _sms_DeliveryRequest = 2;
	};

	// DefaultSMS_Handling
	class DefaultSMS_Handling : public Enumerated {
	public:
		DefaultSMS_Handling();
		~DefaultSMS_Handling();
		static const int _continueTransaction = 0;
		static const int _releaseTransaction = 1;
	};

	// M_CSI
	class M_CSI : public Sequence {
	public:
		M_CSI();
		~M_CSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_notificationToCSE();
		void set_csi_Active();
		// nodes
		MobilityTriggers* _mobilityTriggers;
		ServiceKey* _serviceKey;
		ISDN_AddressString* _gsmSCF_Address;
		ExtensionContainer* _extensionContainer;
		Null* _notificationToCSE;
		Null* _csi_Active;

	};

	// MG_CSI
	class MG_CSI : public Sequence {
	public:
		MG_CSI();
		~MG_CSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_notificationToCSE();
		void set_csi_Active();
		// nodes
		MobilityTriggers* _mobilityTriggers;
		ServiceKey* _serviceKey;
		ISDN_AddressString* _gsmSCF_Address;
		ExtensionContainer* _extensionContainer;
		Null* _notificationToCSE;
		Null* _csi_Active;

	};

	// MobilityTriggers
	class MobilityTriggers : public Sequence_of {
	public:
		MobilityTriggers();
		~MobilityTriggers();
		// nodes
		MM_Code* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfMobilityTriggers
	class maxNumOfMobilityTriggers : public Integer {
	public:
		maxNumOfMobilityTriggers();
		~maxNumOfMobilityTriggers();

	};

	// MM_Code
	class MM_Code : public Octet_string {
	public:
		MM_Code();
		~MM_Code();

	};

	// T_CSI
	class T_CSI : public Sequence {
	public:
		T_CSI();
		~T_CSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_camelCapabilityHandling();
		void set_notificationToCSE();
		void set_csi_Active();
		// nodes
		T_BcsmCamelTDPDataList* _t_BcsmCamelTDPDataList;
		ExtensionContainer* _extensionContainer;
		CamelCapabilityHandling* _camelCapabilityHandling;
		Null* _notificationToCSE;
		Null* _csi_Active;

	};

	// T_BcsmCamelTDPDataList
	class T_BcsmCamelTDPDataList : public Sequence_of {
	public:
		T_BcsmCamelTDPDataList();
		~T_BcsmCamelTDPDataList();
		// nodes
		T_BcsmCamelTDPData* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// T_BcsmCamelTDPData
	class T_BcsmCamelTDPData : public Sequence {
	public:
		T_BcsmCamelTDPData();
		~T_BcsmCamelTDPData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		T_BcsmTriggerDetectionPoint* _t_BcsmTriggerDetectionPoint;
		ServiceKey* _serviceKey;
		ISDN_AddressString* _gsmSCF_Address;
		DefaultCallHandling* _defaultCallHandling;
		ExtensionContainer* _extensionContainer;

	};

	// T_BcsmTriggerDetectionPoint
	class T_BcsmTriggerDetectionPoint : public Enumerated {
	public:
		T_BcsmTriggerDetectionPoint();
		~T_BcsmTriggerDetectionPoint();
		static const int _termAttemptAuthorized = 12;
		static const int _tBusy = 13;
		static const int _tNoAnswer = 14;
	};

	// SendRoutingInfoForGprsArg
	class SendRoutingInfoForGprsArg : public Sequence {
	public:
		SendRoutingInfoForGprsArg();
		~SendRoutingInfoForGprsArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ggsn_Address();
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		GSN_Address* _ggsn_Address;
		ISDN_AddressString* _ggsn_Number;
		ExtensionContainer* _extensionContainer;

	};

	// SendRoutingInfoForGprsRes
	class SendRoutingInfoForGprsRes : public Sequence {
	public:
		SendRoutingInfoForGprsRes();
		~SendRoutingInfoForGprsRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ggsn_Address();
		void set_mobileNotReachableReason();
		void set_extensionContainer();
		// nodes
		GSN_Address* _sgsn_Address;
		GSN_Address* _ggsn_Address;
		AbsentSubscriberDiagnosticSM* _mobileNotReachableReason;
		ExtensionContainer* _extensionContainer;

	};

	// FailureReportArg
	class FailureReportArg : public Sequence {
	public:
		FailureReportArg();
		~FailureReportArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ggsn_Address();
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _ggsn_Number;
		GSN_Address* _ggsn_Address;
		ExtensionContainer* _extensionContainer;

	};

	// FailureReportRes
	class FailureReportRes : public Sequence {
	public:
		FailureReportRes();
		~FailureReportRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ggsn_Address();
		void set_extensionContainer();
		// nodes
		GSN_Address* _ggsn_Address;
		ExtensionContainer* _extensionContainer;

	};

	// NoteMsPresentForGprsArg
	class NoteMsPresentForGprsArg : public Sequence {
	public:
		NoteMsPresentForGprsArg();
		~NoteMsPresentForGprsArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ggsn_Address();
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		GSN_Address* _sgsn_Address;
		GSN_Address* _ggsn_Address;
		ExtensionContainer* _extensionContainer;

	};

	// NoteMsPresentForGprsRes
	class NoteMsPresentForGprsRes : public Sequence {
	public:
		NoteMsPresentForGprsRes();
		~NoteMsPresentForGprsRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// ResetArg
	class ResetArg : public Sequence {
	public:
		ResetArg();
		~ResetArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_hlr_List();
		void set_extensionContainer();
		// nodes
		SendingNode_Number* _sendingNodenumber;
		HLR_List* _hlr_List;
		ExtensionContainer* _extensionContainer;

	};

	// SendingNode_Number
	class SendingNode_Number : public Choice {
	public:
		SendingNode_Number();
		~SendingNode_Number();
		// nodes
		ISDN_AddressString* _hlr_Number;
		ISDN_AddressString* _css_Number;

	};

	// RestoreDataArg
	class RestoreDataArg : public Sequence {
	public:
		RestoreDataArg();
		~RestoreDataArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_lmsi();
		void set_extensionContainer();
		void set_vlr_Capability();
		void set_restorationIndicator();
		// nodes
		IMSI* _imsi;
		LMSI* _lmsi;
		ExtensionContainer* _extensionContainer;
		VLR_Capability* _vlr_Capability;
		Null* _restorationIndicator;

	};

	// RestoreDataRes
	class RestoreDataRes : public Sequence {
	public:
		RestoreDataRes();
		~RestoreDataRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_msNotReachable();
		void set_extensionContainer();
		// nodes
		ISDN_AddressString* _hlr_Number;
		Null* _msNotReachable;
		ExtensionContainer* _extensionContainer;

	};

	// VBSDataList
	class VBSDataList : public Sequence_of {
	public:
		VBSDataList();
		~VBSDataList();
		// nodes
		VoiceBroadcastData* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// VGCSDataList
	class VGCSDataList : public Sequence_of {
	public:
		VGCSDataList();
		~VGCSDataList();
		// nodes
		VoiceGroupCallData* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfVBSGroupIds
	class maxNumOfVBSGroupIds : public Integer {
	public:
		maxNumOfVBSGroupIds();
		~maxNumOfVBSGroupIds();

	};

	// maxNumOfVGCSGroupIds
	class maxNumOfVGCSGroupIds : public Integer {
	public:
		maxNumOfVGCSGroupIds();
		~maxNumOfVGCSGroupIds();

	};

	// VoiceGroupCallData
	class VoiceGroupCallData : public Sequence {
	public:
		VoiceGroupCallData();
		~VoiceGroupCallData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_additionalSubscriptions();
		void set_additionalInfo();
		void set_longGroupId();
		// nodes
		GroupId* _groupId;
		ExtensionContainer* _extensionContainer;
		AdditionalSubscriptions* _additionalSubscriptions;
		AdditionalInfo* _additionalInfo;
		Long_GroupId* _longGroupId;

	};

	// AdditionalInfo
	class AdditionalInfo : public Bit_string {
	public:
		AdditionalInfo();
		~AdditionalInfo();

	};

	// AdditionalSubscriptions
	class AdditionalSubscriptions : public Bit_string {
	public:
		AdditionalSubscriptions();
		~AdditionalSubscriptions();

	};

	// VoiceBroadcastData
	class VoiceBroadcastData : public Sequence {
	public:
		VoiceBroadcastData();
		~VoiceBroadcastData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_broadcastInitEntitlement();
		void set_extensionContainer();
		void set_longGroupId();
		// nodes
		GroupId* _groupid;
		Null* _broadcastInitEntitlement;
		ExtensionContainer* _extensionContainer;
		Long_GroupId* _longGroupId;

	};

	// GroupId
	class GroupId : public TBCD_STRING {
	public:
		GroupId();
		~GroupId();

	};

	// Long_GroupId
	class Long_GroupId : public TBCD_STRING {
	public:
		Long_GroupId();
		~Long_GroupId();

	};

	// ProvideSubscriberInfoArg
	class ProvideSubscriberInfoArg : public Sequence {
	public:
		ProvideSubscriberInfoArg();
		~ProvideSubscriberInfoArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_lmsi();
		void set_extensionContainer();
		void set_callPriority();
		// nodes
		IMSI* _imsi;
		LMSI* _lmsi;
		RequestedInfo* _requestedInfo;
		ExtensionContainer* _extensionContainer;
		EMLPP_Priority* _callPriority;

	};

	// ProvideSubscriberInfoRes
	class ProvideSubscriberInfoRes : public Sequence {
	public:
		ProvideSubscriberInfoRes();
		~ProvideSubscriberInfoRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SubscriberInfo* _subscriberInfo;
		ExtensionContainer* _extensionContainer;

	};

	// SubscriberInfo
	class SubscriberInfo : public Sequence {
	public:
		SubscriberInfo();
		~SubscriberInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_locationInformation();
		void set_subscriberState();
		void set_extensionContainer();
		void set_locationInformationGPRS();
		void set_ps_SubscriberState();
		void set_imei();
		void set_ms_Classmark2();
		void set_gprs_MS_Class();
		void set_mnpInfoRes();
		void set_imsVoiceOverPS_SessionsIndication();
		void set_lastUE_ActivityTime();
		void set_lastRAT_Type();
		void set_eps_SubscriberState();
		void set_locationInformationEPS();
		void set_timeZone();
		void set_daylightSavingTime();
		// nodes
		LocationInformation* _locationInformation;
		SubscriberState* _subscriberState;
		ExtensionContainer* _extensionContainer;
		LocationInformationGPRS* _locationInformationGPRS;
		PS_SubscriberState* _ps_SubscriberState;
		IMEI* _imei;
		MS_Classmark2* _ms_Classmark2;
		GPRSMSClass* _gprs_MS_Class;
		MNPInfoRes* _mnpInfoRes;
		IMS_VoiceOverPS_SessionsInd* _imsVoiceOverPS_SessionsIndication;
		Time* _lastUE_ActivityTime;
		Used_RAT_Type* _lastRAT_Type;
		PS_SubscriberState* _eps_SubscriberState;
		LocationInformationEPS* _locationInformationEPS;
		TimeZone* _timeZone;
		DaylightSavingTime* _daylightSavingTime;

	};

	// IMS_VoiceOverPS_SessionsInd
	class IMS_VoiceOverPS_SessionsInd : public Enumerated {
	public:
		IMS_VoiceOverPS_SessionsInd();
		~IMS_VoiceOverPS_SessionsInd();
		static const int _imsVoiceOverPS_SessionsNotSupported = 0;
		static const int _imsVoiceOverPS_SessionsSupported = 1;
		static const int _unknown = 2;

	};

	// TimeZone
	class TimeZone : public Octet_string {
	public:
		TimeZone();
		~TimeZone();

	};

	// DaylightSavingTime
	class DaylightSavingTime : public Enumerated {
	public:
		DaylightSavingTime();
		~DaylightSavingTime();
		static const int _noAdjustment = 0;
		static const int _plusOneHourAdjustment = 1;
		static const int _plusTwoHoursAdjustment = 2;
	};

	// MNPInfoRes
	class MNPInfoRes : public Sequence {
	public:
		MNPInfoRes();
		~MNPInfoRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_routeingNumber();
		void set_imsi();
		void set_msisdn();
		void set_numberPortabilityStatus();
		void set_extensionContainer();
		// nodes
		RouteingNumber* _routeingNumber;
		IMSI* _imsi;
		ISDN_AddressString* _msisdn;
		NumberPortabilityStatus* _numberPortabilityStatus;
		ExtensionContainer* _extensionContainer;

	};

	// RouteingNumber
	class RouteingNumber : public TBCD_STRING {
	public:
		RouteingNumber();
		~RouteingNumber();

	};

	// NumberPortabilityStatus
	class NumberPortabilityStatus : public Enumerated {
	public:
		NumberPortabilityStatus();
		~NumberPortabilityStatus();
		static const int _notKnownToBePorted = 0;
		static const int _ownNumberPortedOut = 1;
		static const int _foreignNumberPortedToForeignNetwork = 2;
		static const int _ownNumberNotPortedOut = 4;
		static const int _foreignNumberPortedIn = 5;

	};

	// MS_Classmark2
	class MS_Classmark2 : public Octet_string {
	public:
		MS_Classmark2();
		~MS_Classmark2();

	};

	// GPRSMSClass
	class GPRSMSClass : public Sequence {
	public:
		GPRSMSClass();
		~GPRSMSClass();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_mSRadioAccessCapability();
		// nodes
		MSNetworkCapability* _mSNetworkCapability;
		MSRadioAccessCapability* _mSRadioAccessCapability;

	};

	// MSNetworkCapability
	class MSNetworkCapability : public Octet_string {
	public:
		MSNetworkCapability();
		~MSNetworkCapability();

	};

	// MSRadioAccessCapability
	class MSRadioAccessCapability : public Octet_string {
	public:
		MSRadioAccessCapability();
		~MSRadioAccessCapability();

	};

	// RequestedInfo
	class RequestedInfo : public Sequence {
	public:
		RequestedInfo();
		~RequestedInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_locationInformation();
		void set_subscriberState();
		void set_extensionContainer();
		void set_currentLocation();
		void set_requestedDomain();
		void set_imei();
		void set_ms_classmark();
		void set_mnpRequestedInfo();
		void set_locationInformationEPS_Supported();
		void set_t_adsData();
		void set_requestedNodes();
		void set_servingNodeIndication();
		void set_localTimeZoneRequest();
		// nodes
		Null* _locationInformation;
		Null* _subscriberState;
		ExtensionContainer* _extensionContainer;
		Null* _currentLocation;
		DomainType* _requestedDomain;
		Null* _imei;
		Null* _ms_classmark;
		Null* _mnpRequestedInfo;
		Null* _locationInformationEPS_Supported;
		Null* _t_adsData;
		RequestedNodes* _requestedNodes;
		Null* _servingNodeIndication;
		Null* _localTimeZoneRequest;

	};

	// DomainType
	class DomainType : public Enumerated {
	public:
		DomainType();
		~DomainType();
		static const int _cs_Domain = 0;
		static const int _ps_Domain = 1;
	};

	// RequestedNodes
	class RequestedNodes : public Bit_string {
	public:
		RequestedNodes();
		~RequestedNodes();

	};

	// LocationInformation
	class LocationInformation : public Sequence {
	public:
		LocationInformation();
		~LocationInformation();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ageOfLocationInformation();
		void set_geographicalInformation();
		void set_vlr_number();
		void set_locationNumber();
		void set_cellGlobalIdOrServiceAreaIdOrLAI();
		void set_extensionContainer();
		void set_selectedLSA_Id();
		void set_msc_Number();
		void set_geodeticInformation();
		void set_currentLocationRetrieved();
		void set_sai_Present();
		void set_locationInformationEPS();
		void set_userCSGInformation();
		// nodes
		AgeOfLocationInformation* _ageOfLocationInformation;
		GeographicalInformation* _geographicalInformation;
		ISDN_AddressString* _vlr_number;
		LocationNumber* _locationNumber;
		CellGlobalIdOrServiceAreaIdOrLAI* _cellGlobalIdOrServiceAreaIdOrLAI;
		ExtensionContainer* _extensionContainer;
		LSAIdentity* _selectedLSA_Id;
		ISDN_AddressString* _msc_Number;
		GeodeticInformation* _geodeticInformation;
		Null* _currentLocationRetrieved;
		Null* _sai_Present;
		LocationInformationEPS* _locationInformationEPS;
		UserCSGInformation* _userCSGInformation;

	};

	// LocationInformationEPS
	class LocationInformationEPS : public Sequence {
	public:
		LocationInformationEPS();
		~LocationInformationEPS();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_e_utranCellGlobalIdentity();
		void set_trackingAreaIdentity();
		void set_extensionContainer();
		void set_geographicalInformation();
		void set_geodeticInformation();
		void set_currentLocationRetrieved();
		void set_ageOfLocationInformation();
		void set_mme_Name();
		// nodes
		E_UTRAN_CGI* _e_utranCellGlobalIdentity;
		TA_Id* _trackingAreaIdentity;
		ExtensionContainer* _extensionContainer;
		GeographicalInformation* _geographicalInformation;
		GeodeticInformation* _geodeticInformation;
		Null* _currentLocationRetrieved;
		AgeOfLocationInformation* _ageOfLocationInformation;
		DiameterIdentity* _mme_Name;

	};

	// LocationInformationGPRS
	class LocationInformationGPRS : public Sequence {
	public:
		LocationInformationGPRS();
		~LocationInformationGPRS();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cellGlobalIdOrServiceAreaIdOrLAI();
		void set_routeingAreaIdentity();
		void set_geographicalInformation();
		void set_sgsn_Number();
		void set_selectedLSAIdentity();
		void set_extensionContainer();
		void set_sai_Present();
		void set_geodeticInformation();
		void set_currentLocationRetrieved();
		void set_ageOfLocationInformation();
		void set_userCSGInformation();
		// nodes
		CellGlobalIdOrServiceAreaIdOrLAI* _cellGlobalIdOrServiceAreaIdOrLAI;
		RAIdentity* _routeingAreaIdentity;
		GeographicalInformation* _geographicalInformation;
		ISDN_AddressString* _sgsn_Number;
		LSAIdentity* _selectedLSAIdentity;
		ExtensionContainer* _extensionContainer;
		Null* _sai_Present;
		GeodeticInformation* _geodeticInformation;
		Null* _currentLocationRetrieved;
		AgeOfLocationInformation* _ageOfLocationInformation;
		UserCSGInformation* _userCSGInformation;

	};

	// UserCSGInformation
	class UserCSGInformation : public Sequence {
	public:
		UserCSGInformation();
		~UserCSGInformation();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_accessMode();
		void set_cmi();
		// nodes
		CSG_Id* _csg_Id;
		ExtensionContainer* _extensionContainer;
		Octet_string* _accessMode;
		Octet_string* _cmi;

	};

	// GeographicalInformation
	class GeographicalInformation : public Octet_string {
	public:
		GeographicalInformation();
		~GeographicalInformation();

	};

	// GeodeticInformation
	class GeodeticInformation : public Octet_string {
	public:
		GeodeticInformation();
		~GeodeticInformation();

	};

	// LocationNumber
	class LocationNumber : public Octet_string {
	public:
		LocationNumber();
		~LocationNumber();

	};

	// SubscriberState
	class SubscriberState : public Choice {
	public:
		SubscriberState();
		~SubscriberState();
		// nodes
		Null* _assumedIdle;
		Null* _camelBusy;
		NotReachableReason* _netDetNotReachable;
		Null* _notProvidedFromVLR;

	};

	// PS_SubscriberState
	class PS_SubscriberState : public Choice {
	public:
		PS_SubscriberState();
		~PS_SubscriberState();
		// nodes
		Null* _notProvidedFromSGSNorMME;
		Null* _ps_Detached;
		Null* _ps_AttachedNotReachableForPaging;
		Null* _ps_AttachedReachableForPaging;
		PDP_ContextInfoList* _ps_PDP_ActiveNotReachableForPaging;
		PDP_ContextInfoList* _ps_PDP_ActiveReachableForPaging;
		NotReachableReason* _netDetNotReachable;

	};

	// PDP_ContextInfoList
	class PDP_ContextInfoList : public Sequence_of {
	public:
		PDP_ContextInfoList();
		~PDP_ContextInfoList();
		// nodes
		PDP_ContextInfo* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// PDP_ContextInfo
	class PDP_ContextInfo : public Sequence {
	public:
		PDP_ContextInfo();
		~PDP_ContextInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_pdp_ContextActive();
		void set_pdp_Address();
		void set_apn_Subscribed();
		void set_apn_InUse();
		void set_nsapi();
		void set_transactionId();
		void set_teid_ForGnAndGp();
		void set_teid_ForIu();
		void set_ggsn_Address();
		void set_qos_Subscribed();
		void set_qos_Requested();
		void set_qos_Negotiated();
		void set_chargingId();
		void set_chargingCharacteristics();
		void set_rnc_Address();
		void set_extensionContainer();
		void set_qos2_Subscribed();
		void set_qos2_Requested();
		void set_qos2_Negotiated();
		void set_qos3_Subscribed();
		void set_qos3_Requested();
		void set_qos3_Negotiated();
		void set_qos4_Subscribed();
		void set_qos4_Requested();
		void set_qos4_Negotiated();
		void set_ext_pdp_Type();
		void set_ext_pdp_Address();
		// nodes
		ContextId* _pdp_ContextIdentifier;
		Null* _pdp_ContextActive;
		PDP_Type* _pdp_Type;
		PDP_Address* _pdp_Address;
		APN* _apn_Subscribed;
		APN* _apn_InUse;
		NSAPI* _nsapi;
		TransactionId* _transactionId;
		TEID* _teid_ForGnAndGp;
		TEID* _teid_ForIu;
		GSN_Address* _ggsn_Address;
		Ext_QoS_Subscribed* _qos_Subscribed;
		Ext_QoS_Subscribed* _qos_Requested;
		Ext_QoS_Subscribed* _qos_Negotiated;
		GPRSChargingID* _chargingId;
		ChargingCharacteristics* _chargingCharacteristics;
		GSN_Address* _rnc_Address;
		ExtensionContainer* _extensionContainer;
		Ext2_QoS_Subscribed* _qos2_Subscribed;
		Ext2_QoS_Subscribed* _qos2_Requested;
		Ext2_QoS_Subscribed* _qos2_Negotiated;
		Ext3_QoS_Subscribed* _qos3_Subscribed;
		Ext3_QoS_Subscribed* _qos3_Requested;
		Ext3_QoS_Subscribed* _qos3_Negotiated;
		Ext4_QoS_Subscribed* _qos4_Subscribed;
		Ext4_QoS_Subscribed* _qos4_Requested;
		Ext4_QoS_Subscribed* _qos4_Negotiated;
		Ext_PDP_Type* _ext_pdp_Type;
		PDP_Address* _ext_pdp_Address;

	};

	// NSAPI
	class NSAPI : public Integer {
	public:
		NSAPI();
		~NSAPI();

	};

	// TransactionId
	class TransactionId : public Octet_string {
	public:
		TransactionId();
		~TransactionId();

	};

	// TEID
	class TEID : public Octet_string {
	public:
		TEID();
		~TEID();

	};

	// GPRSChargingID
	class GPRSChargingID : public Octet_string {
	public:
		GPRSChargingID();
		~GPRSChargingID();

	};

	// NotReachableReason
	class NotReachableReason : public Enumerated {
	public:
		NotReachableReason();
		~NotReachableReason();
		static const int _msPurged = 0;
		static const int _imsiDetached = 1;
		static const int _restrictedArea = 2;
		static const int _notRegistered = 3;

	};

	// AnyTimeInterrogationArg
	class AnyTimeInterrogationArg : public Sequence {
	public:
		AnyTimeInterrogationArg();
		~AnyTimeInterrogationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SubscriberIdentity* _subscriberIdentity;
		RequestedInfo* _requestedInfo;
		ISDN_AddressString* _gsmSCF_Address;
		ExtensionContainer* _extensionContainer;

	};

	// AnyTimeInterrogationRes
	class AnyTimeInterrogationRes : public Sequence {
	public:
		AnyTimeInterrogationRes();
		~AnyTimeInterrogationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		SubscriberInfo* _subscriberInfo;
		ExtensionContainer* _extensionContainer;

	};

	// AnyTimeSubscriptionInterrogationArg
	class AnyTimeSubscriptionInterrogationArg : public Sequence {
	public:
		AnyTimeSubscriptionInterrogationArg();
		~AnyTimeSubscriptionInterrogationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_longFTN_Supported();
		// nodes
		SubscriberIdentity* _subscriberIdentity;
		RequestedSubscriptionInfo* _requestedSubscriptionInfo;
		ISDN_AddressString* _gsmSCF_Address;
		ExtensionContainer* _extensionContainer;
		Null* _longFTN_Supported;

	};

	// AnyTimeSubscriptionInterrogationRes
	class AnyTimeSubscriptionInterrogationRes : public Sequence {
	public:
		AnyTimeSubscriptionInterrogationRes();
		~AnyTimeSubscriptionInterrogationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_callForwardingData();
		void set_callBarringData();
		void set_odb_Info();
		void set_camel_SubscriptionInfo();
		void set_supportedVLR_CAMEL_Phases();
		void set_supportedSGSN_CAMEL_Phases();
		void set_extensionContainer();
		void set_offeredCamel4CSIsInVLR();
		void set_offeredCamel4CSIsInSGSN();
		void set_msisdn_BS_List();
		void set_csg_SubscriptionDataList();
		void set_cw_Data();
		void set_ch_Data();
		void set_clip_Data();
		void set_clir_Data();
		void set_ect_data();
		// nodes
		CallForwardingData* _callForwardingData;
		CallBarringData* _callBarringData;
		ODB_Info* _odb_Info;
		CAMEL_SubscriptionInfo* _camel_SubscriptionInfo;
		SupportedCamelPhases* _supportedVLR_CAMEL_Phases;
		SupportedCamelPhases* _supportedSGSN_CAMEL_Phases;
		ExtensionContainer* _extensionContainer;
		OfferedCamel4CSIs* _offeredCamel4CSIsInVLR;
		OfferedCamel4CSIs* _offeredCamel4CSIsInSGSN;
		MSISDN_BS_List* _msisdn_BS_List;
		CSG_SubscriptionDataList* _csg_SubscriptionDataList;
		CallWaitingData* _cw_Data;
		CallHoldData* _ch_Data;
		ClipData* _clip_Data;
		ClirData* _clir_Data;
		EctData* _ect_data;

	};

	// CallWaitingData
	class CallWaitingData : public Sequence {
	public:
		CallWaitingData();
		~CallWaitingData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_notificationToCSE();
		// nodes
		Ext_CwFeatureList* _cwFeatureList;
		Null* _notificationToCSE;

	};

	// Ext_CwFeatureList
	class Ext_CwFeatureList : public Sequence_of {
	public:
		Ext_CwFeatureList();
		~Ext_CwFeatureList();
		// nodes
		Ext_CwFeature* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// Ext_CwFeature
	class Ext_CwFeature : public Sequence {
	public:
		Ext_CwFeature();
		~Ext_CwFeature();
		// nodes
		Ext_BasicServiceCode* _basicService;
		Ext_SS_Status* _ss_Status;

	};

	// ClipData
	class ClipData : public Sequence {
	public:
		ClipData();
		~ClipData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_notificationToCSE();
		// nodes
		Ext_SS_Status* _ss_Status;
		OverrideCategory* _overrideCategory;
		Null* _notificationToCSE;

	};

	// ClirData
	class ClirData : public Sequence {
	public:
		ClirData();
		~ClirData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cliRestrictionOption();
		void set_notificationToCSE();
		// nodes
		Ext_SS_Status* _ss_Status;
		CliRestrictionOption* _cliRestrictionOption;
		Null* _notificationToCSE;

	};

	// CallHoldData
	class CallHoldData : public Sequence {
	public:
		CallHoldData();
		~CallHoldData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_notificationToCSE();
		// nodes
		Ext_SS_Status* _ss_Status;
		Null* _notificationToCSE;

	};

	// EctData
	class EctData : public Sequence {
	public:
		EctData();
		~EctData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_notificationToCSE();
		// nodes
		Ext_SS_Status* _ss_Status;
		Null* _notificationToCSE;

	};

	// RequestedSubscriptionInfo
	class RequestedSubscriptionInfo : public Sequence {
	public:
		RequestedSubscriptionInfo();
		~RequestedSubscriptionInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_requestedSS_Info();
		void set_odb();
		void set_requestedCAMEL_SubscriptionInfo();
		void set_supportedVLR_CAMEL_Phases();
		void set_supportedSGSN_CAMEL_Phases();
		void set_extensionContainer();
		void set_additionalRequestedCAMEL_SubscriptionInfo();
		void set_msisdn_BS_List();
		void set_csg_SubscriptionDataRequested();
		void set_cw_Info();
		void set_clip_Info();
		void set_clir_Info();
		void set_hold_Info();
		void set_ect_Info();
		// nodes
		SS_ForBS_Code* _requestedSS_Info;
		Null* _odb;
		RequestedCAMEL_SubscriptionInfo* _requestedCAMEL_SubscriptionInfo;
		Null* _supportedVLR_CAMEL_Phases;
		Null* _supportedSGSN_CAMEL_Phases;
		ExtensionContainer* _extensionContainer;
		AdditionalRequestedCAMEL_SubscriptionInfo* _additionalRequestedCAMEL_SubscriptionInfo;
		Null* _msisdn_BS_List;
		Null* _csg_SubscriptionDataRequested;
		Null* _cw_Info;
		Null* _clip_Info;
		Null* _clir_Info;
		Null* _hold_Info;
		Null* _ect_Info;

	};

	// MSISDN_BS_List
	class MSISDN_BS_List : public Sequence_of {
	public:
		MSISDN_BS_List();
		~MSISDN_BS_List();
		// nodes
		MSISDN_BS* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// maxNumOfMSISDN
	class maxNumOfMSISDN : public Integer {
	public:
		maxNumOfMSISDN();
		~maxNumOfMSISDN();

	};

	// MSISDN_BS
	class MSISDN_BS : public Sequence {
	public:
		MSISDN_BS();
		~MSISDN_BS();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicServiceList();
		void set_extensionContainer();
		// nodes
		ISDN_AddressString* _msisdn;
		BasicServiceList* _basicServiceList;
		ExtensionContainer* _extensionContainer;

	};

	// RequestedCAMEL_SubscriptionInfo
	class RequestedCAMEL_SubscriptionInfo : public Enumerated {
	public:
		RequestedCAMEL_SubscriptionInfo();
		~RequestedCAMEL_SubscriptionInfo();
		static const int _o_CSI = 0;
		static const int _t_CSI = 1;
		static const int _vt_CSI = 2;
		static const int _tif_CSI = 3;
		static const int _gprs_CSI = 4;
		static const int _mo_sms_CSI = 5;
		static const int _ss_CSI = 6;
		static const int _m_CSI = 7;
		static const int _d_csi = 8;

	};

	// AdditionalRequestedCAMEL_SubscriptionInfo
	class AdditionalRequestedCAMEL_SubscriptionInfo : public Enumerated {
	public:
		AdditionalRequestedCAMEL_SubscriptionInfo();
		~AdditionalRequestedCAMEL_SubscriptionInfo();
		static const int _mt_sms_CSI = 0;
		static const int _mg_csi = 1;
		static const int _o_IM_CSI = 2;
		static const int _d_IM_CSI = 3;
		static const int _vt_IM_CSI = 4;
	};

	// CallForwardingData
	class CallForwardingData : public Sequence {
	public:
		CallForwardingData();
		~CallForwardingData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_notificationToCSE();
		void set_extensionContainer();
		// nodes
		Ext_ForwFeatureList* _forwardingFeatureList;
		Null* _notificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// CallBarringData
	class CallBarringData : public Sequence {
	public:
		CallBarringData();
		~CallBarringData();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_password();
		void set_wrongPasswordAttemptsCounter();
		void set_notificationToCSE();
		void set_extensionContainer();
		// nodes
		Ext_CallBarFeatureList* _callBarringFeatureList;
		Password* _password;
		WrongPasswordAttemptsCounter* _wrongPasswordAttemptsCounter;
		Null* _notificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// WrongPasswordAttemptsCounter
	class WrongPasswordAttemptsCounter : public Integer {
	public:
		WrongPasswordAttemptsCounter();
		~WrongPasswordAttemptsCounter();

	};

	// ODB_Info
	class ODB_Info : public Sequence {
	public:
		ODB_Info();
		~ODB_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_notificationToCSE();
		void set_extensionContainer();
		// nodes
		ODB_Data* _odb_Data;
		Null* _notificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// CAMEL_SubscriptionInfo
	class CAMEL_SubscriptionInfo : public Sequence {
	public:
		CAMEL_SubscriptionInfo();
		~CAMEL_SubscriptionInfo();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_o_CSI();
		void set_o_BcsmCamelTDP_CriteriaList();
		void set_d_CSI();
		void set_t_CSI();
		void set_t_BCSM_CAMEL_TDP_CriteriaList();
		void set_vt_CSI();
		void set_vt_BCSM_CAMEL_TDP_CriteriaList();
		void set_tif_CSI();
		void set_tif_CSI_NotificationToCSE();
		void set_gprs_CSI();
		void set_mo_sms_CSI();
		void set_ss_CSI();
		void set_m_CSI();
		void set_extensionContainer();
		void set_specificCSIDeletedList();
		void set_mt_sms_CSI();
		void set_mt_smsCAMELTDP_CriteriaList();
		void set_mg_csi();
		void set_o_IM_CSI();
		void set_o_IM_BcsmCamelTDP_CriteriaList();
		void set_d_IM_CSI();
		void set_vt_IM_CSI();
		void set_vt_IM_BCSM_CAMEL_TDP_CriteriaList();
		// nodes
		O_CSI* _o_CSI;
		O_BcsmCamelTDPCriteriaList* _o_BcsmCamelTDP_CriteriaList;
		D_CSI* _d_CSI;
		T_CSI* _t_CSI;
		T_BCSM_CAMEL_TDP_CriteriaList* _t_BCSM_CAMEL_TDP_CriteriaList;
		T_CSI* _vt_CSI;
		T_BCSM_CAMEL_TDP_CriteriaList* _vt_BCSM_CAMEL_TDP_CriteriaList;
		Null* _tif_CSI;
		Null* _tif_CSI_NotificationToCSE;
		GPRS_CSI* _gprs_CSI;
		SMS_CSI* _mo_sms_CSI;
		SS_CSI* _ss_CSI;
		M_CSI* _m_CSI;
		ExtensionContainer* _extensionContainer;
		SpecificCSI_Withdraw* _specificCSIDeletedList;
		SMS_CSI* _mt_sms_CSI;
		MT_smsCAMELTDP_CriteriaList* _mt_smsCAMELTDP_CriteriaList;
		MG_CSI* _mg_csi;
		O_CSI* _o_IM_CSI;
		O_BcsmCamelTDPCriteriaList* _o_IM_BcsmCamelTDP_CriteriaList;
		D_CSI* _d_IM_CSI;
		T_CSI* _vt_IM_CSI;
		T_BCSM_CAMEL_TDP_CriteriaList* _vt_IM_BCSM_CAMEL_TDP_CriteriaList;

	};

	// AnyTimeModificationArg
	class AnyTimeModificationArg : public Sequence {
	public:
		AnyTimeModificationArg();
		~AnyTimeModificationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_modificationRequestFor_CF_Info();
		void set_modificationRequestFor_CB_Info();
		void set_modificationRequestFor_CSI();
		void set_extensionContainer();
		void set_longFTN_Supported();
		void set_modificationRequestFor_ODB_data();
		void set_modificationRequestFor_IP_SM_GW_Data();
		void set_activationRequestForUE_reachability();
		void set_modificationRequestFor_CSG();
		void set_modificationRequestFor_CW_Data();
		void set_modificationRequestFor_CLIP_Data();
		void set_modificationRequestFor_CLIR_Data();
		void set_modificationRequestFor_HOLD_Data();
		void set_modificationRequestFor_ECT_Data();
		// nodes
		SubscriberIdentity* _subscriberIdentity;
		ISDN_AddressString* _gsmSCF_Address;
		ModificationRequestFor_CF_Info* _modificationRequestFor_CF_Info;
		ModificationRequestFor_CB_Info* _modificationRequestFor_CB_Info;
		ModificationRequestFor_CSI* _modificationRequestFor_CSI;
		ExtensionContainer* _extensionContainer;
		Null* _longFTN_Supported;
		ModificationRequestFor_ODB_data* _modificationRequestFor_ODB_data;
		ModificationRequestFor_IP_SM_GW_Data* _modificationRequestFor_IP_SM_GW_Data;
		RequestedServingNode* _activationRequestForUE_reachability;
		ModificationRequestFor_CSG* _modificationRequestFor_CSG;
		ModificationRequestFor_CW_Info* _modificationRequestFor_CW_Data;
		ModificationRequestFor_CLIP_Info* _modificationRequestFor_CLIP_Data;
		ModificationRequestFor_CLIR_Info* _modificationRequestFor_CLIR_Data;
		ModificationRequestFor_CH_Info* _modificationRequestFor_HOLD_Data;
		ModificationRequestFor_ECT_Info* _modificationRequestFor_ECT_Data;

	};

	// ModificationRequestFor_CW_Info
	class ModificationRequestFor_CW_Info : public Sequence {
	public:
		ModificationRequestFor_CW_Info();
		~ModificationRequestFor_CW_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_ss_Status();
		void set_modifyNotificationToCSE();
		void set_extensionContainer();
		// nodes
		Ext_BasicServiceCode* _basicService;
		Ext_SS_Status* _ss_Status;
		ModificationInstruction* _modifyNotificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// ModificationRequestFor_CH_Info
	class ModificationRequestFor_CH_Info : public Sequence {
	public:
		ModificationRequestFor_CH_Info();
		~ModificationRequestFor_CH_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_Status();
		void set_modifyNotificationToCSE();
		void set_extensionContainer();
		// nodes
		Ext_SS_Status* _ss_Status;
		ModificationInstruction* _modifyNotificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// ModificationRequestFor_ECT_Info
	class ModificationRequestFor_ECT_Info : public Sequence {
	public:
		ModificationRequestFor_ECT_Info();
		~ModificationRequestFor_ECT_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_Status();
		void set_modifyNotificationToCSE();
		void set_extensionContainer();
		// nodes
		Ext_SS_Status* _ss_Status;
		ModificationInstruction* _modifyNotificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// ModificationRequestFor_CLIR_Info
	class ModificationRequestFor_CLIR_Info : public Sequence {
	public:
		ModificationRequestFor_CLIR_Info();
		~ModificationRequestFor_CLIR_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_Status();
		void set_cliRestrictionOption();
		void set_modifyNotificationToCSE();
		void set_extensionContainer();
		// nodes
		Ext_SS_Status* _ss_Status;
		CliRestrictionOption* _cliRestrictionOption;
		ModificationInstruction* _modifyNotificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// ModificationRequestFor_CLIP_Info
	class ModificationRequestFor_CLIP_Info : public Sequence {
	public:
		ModificationRequestFor_CLIP_Info();
		~ModificationRequestFor_CLIP_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_Status();
		void set_overrideCategory();
		void set_modifyNotificationToCSE();
		void set_extensionContainer();
		// nodes
		Ext_SS_Status* _ss_Status;
		OverrideCategory* _overrideCategory;
		ModificationInstruction* _modifyNotificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// ModificationRequestFor_CSG
	class ModificationRequestFor_CSG : public Sequence {
	public:
		ModificationRequestFor_CSG();
		~ModificationRequestFor_CSG();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_modifyNotificationToCSE();
		void set_extensionContainer();
		// nodes
		ModificationInstruction* _modifyNotificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// RequestedServingNode
	class RequestedServingNode : public Bit_string {
	public:
		RequestedServingNode();
		~RequestedServingNode();

	};

	// ServingNode
	class ServingNode : public Bit_string {
	public:
		ServingNode();
		~ServingNode();

	};

	// AnyTimeModificationRes
	class AnyTimeModificationRes : public Sequence {
	public:
		AnyTimeModificationRes();
		~AnyTimeModificationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_InfoFor_CSE();
		void set_camel_SubscriptionInfo();
		void set_extensionContainer();
		void set_odb_Info();
		void set_cw_Data();
		void set_ch_Data();
		void set_clip_Data();
		void set_clir_Data();
		void set_ect_data();
		void set_serviceCentreAddress();
		// nodes
		Ext_SS_InfoFor_CSE* _ss_InfoFor_CSE;
		CAMEL_SubscriptionInfo* _camel_SubscriptionInfo;
		ExtensionContainer* _extensionContainer;
		ODB_Info* _odb_Info;
		CallWaitingData* _cw_Data;
		CallHoldData* _ch_Data;
		ClipData* _clip_Data;
		ClirData* _clir_Data;
		EctData* _ect_data;
		AddressString* _serviceCentreAddress;

	};

	// ModificationRequestFor_CF_Info
	class ModificationRequestFor_CF_Info : public Sequence {
	public:
		ModificationRequestFor_CF_Info();
		~ModificationRequestFor_CF_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_ss_Status();
		void set_forwardedToNumber();
		void set_forwardedToSubaddress();
		void set_noReplyConditionTime();
		void set_modifyNotificationToCSE();
		void set_extensionContainer();
		// nodes
		SS_Code* _ss_Code;
		Ext_BasicServiceCode* _basicService;
		Ext_SS_Status* _ss_Status;
		AddressString* _forwardedToNumber;
		ISDN_SubaddressString* _forwardedToSubaddress;
		Ext_NoRepCondTime* _noReplyConditionTime;
		ModificationInstruction* _modifyNotificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// ModificationRequestFor_CB_Info
	class ModificationRequestFor_CB_Info : public Sequence {
	public:
		ModificationRequestFor_CB_Info();
		~ModificationRequestFor_CB_Info();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_basicService();
		void set_ss_Status();
		void set_password();
		void set_wrongPasswordAttemptsCounter();
		void set_modifyNotificationToCSE();
		void set_extensionContainer();
		// nodes
		SS_Code* _ss_Code;
		Ext_BasicServiceCode* _basicService;
		Ext_SS_Status* _ss_Status;
		Password* _password;
		WrongPasswordAttemptsCounter* _wrongPasswordAttemptsCounter;
		ModificationInstruction* _modifyNotificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// ModificationRequestFor_ODB_data
	class ModificationRequestFor_ODB_data : public Sequence {
	public:
		ModificationRequestFor_ODB_data();
		~ModificationRequestFor_ODB_data();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_odb_data();
		void set_modifyNotificationToCSE();
		void set_extensionContainer();
		// nodes
		ODB_Data* _odb_data;
		ModificationInstruction* _modifyNotificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// ModificationRequestFor_CSI
	class ModificationRequestFor_CSI : public Sequence {
	public:
		ModificationRequestFor_CSI();
		~ModificationRequestFor_CSI();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_modifyNotificationToCSE();
		void set_modifyCSI_State();
		void set_extensionContainer();
		void set_additionalRequestedCAMEL_SubscriptionInfo();
		// nodes
		RequestedCAMEL_SubscriptionInfo* _requestedCamel_SubscriptionInfo;
		ModificationInstruction* _modifyNotificationToCSE;
		ModificationInstruction* _modifyCSI_State;
		ExtensionContainer* _extensionContainer;
		AdditionalRequestedCAMEL_SubscriptionInfo* _additionalRequestedCAMEL_SubscriptionInfo;

	};

	// ModificationRequestFor_IP_SM_GW_Data
	class ModificationRequestFor_IP_SM_GW_Data : public Sequence {
	public:
		ModificationRequestFor_IP_SM_GW_Data();
		~ModificationRequestFor_IP_SM_GW_Data();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_modifyRegistrationStatus();
		void set_extensionContainer();
		void set_ip_sm_gw_DiameterAddress();
		// nodes
		ModificationInstruction* _modifyRegistrationStatus;
		ExtensionContainer* _extensionContainer;
		NetworkNodeDiameterAddress* _ip_sm_gw_DiameterAddress;

	};

	// ModificationInstruction
	class ModificationInstruction : public Enumerated {
	public:
		ModificationInstruction();
		~ModificationInstruction();
		static const int _deactivate = 0;
		static const int _activate = 1;

	};

	// NoteSubscriberDataModifiedArg
	class NoteSubscriberDataModifiedArg : public Sequence {
	public:
		NoteSubscriberDataModifiedArg();
		~NoteSubscriberDataModifiedArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_forwardingInfoFor_CSE();
		void set_callBarringInfoFor_CSE();
		void set_odb_Info();
		void set_camel_SubscriptionInfo();
		void set_allInformationSent();
		void set_extensionContainer();
		void set_ue_reachable();
		void set_csg_SubscriptionDataList();
		void set_cw_Data();
		void set_ch_Data();
		void set_clip_Data();
		void set_clir_Data();
		void set_ect_data();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _msisdn;
		Ext_ForwardingInfoFor_CSE* _forwardingInfoFor_CSE;
		Ext_CallBarringInfoFor_CSE* _callBarringInfoFor_CSE;
		ODB_Info* _odb_Info;
		CAMEL_SubscriptionInfo* _camel_SubscriptionInfo;
		Null* _allInformationSent;
		ExtensionContainer* _extensionContainer;
		ServingNode* _ue_reachable;
		CSG_SubscriptionDataList* _csg_SubscriptionDataList;
		CallWaitingData* _cw_Data;
		CallHoldData* _ch_Data;
		ClipData* _clip_Data;
		ClirData* _clir_Data;
		EctData* _ect_data;

	};

	// NoteSubscriberDataModifiedRes
	class NoteSubscriberDataModifiedRes : public Sequence {
	public:
		NoteSubscriberDataModifiedRes();
		~NoteSubscriberDataModifiedRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// NoteMM_EventArg
	class NoteMM_EventArg : public Sequence {
	public:
		NoteMM_EventArg();
		~NoteMM_EventArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_locationInformation();
		void set_supportedCAMELPhases();
		void set_extensionContainer();
		void set_locationInformationGPRS();
		void set_offeredCamel4Functionalities();
		// nodes
		ServiceKey* _serviceKey;
		MM_Code* _eventMet;
		IMSI* _imsi;
		ISDN_AddressString* _msisdn;
		LocationInformation* _locationInformation;
		SupportedCamelPhases* _supportedCAMELPhases;
		ExtensionContainer* _extensionContainer;
		LocationInformationGPRS* _locationInformationGPRS;
		OfferedCamel4Functionalities* _offeredCamel4Functionalities;

	};

	// NoteMM_EventRes
	class NoteMM_EventRes : public Sequence {
	public:
		NoteMM_EventRes();
		~NoteMM_EventRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// Ext_SS_InfoFor_CSE
	class Ext_SS_InfoFor_CSE : public Choice {
	public:
		Ext_SS_InfoFor_CSE();
		~Ext_SS_InfoFor_CSE();
		// nodes
		Ext_ForwardingInfoFor_CSE* _forwardingInfoFor_CSE;
		Ext_CallBarringInfoFor_CSE* _callBarringInfoFor_CSE;

	};

	// Ext_ForwardingInfoFor_CSE
	class Ext_ForwardingInfoFor_CSE : public Sequence {
	public:
		Ext_ForwardingInfoFor_CSE();
		~Ext_ForwardingInfoFor_CSE();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_notificationToCSE();
		void set_extensionContainer();
		// nodes
		SS_Code* _ss_Code;
		Ext_ForwFeatureList* _forwardingFeatureList;
		Null* _notificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// Ext_CallBarringInfoFor_CSE
	class Ext_CallBarringInfoFor_CSE : public Sequence {
	public:
		Ext_CallBarringInfoFor_CSE();
		~Ext_CallBarringInfoFor_CSE();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_password();
		void set_wrongPasswordAttemptsCounter();
		void set_notificationToCSE();
		void set_extensionContainer();
		// nodes
		SS_Code* _ss_Code;
		Ext_CallBarFeatureList* _callBarringFeatureList;
		Password* _password;
		WrongPasswordAttemptsCounter* _wrongPasswordAttemptsCounter;
		Null* _notificationToCSE;
		ExtensionContainer* _extensionContainer;

	};

	// UpdateVcsgLocationArg
	class UpdateVcsgLocationArg : public Sequence {
	public:
		UpdateVcsgLocationArg();
		~UpdateVcsgLocationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_msisdn();
		void set_vlr_Number();
		void set_sgsn_Number();
		void set_extensionContainer();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _msisdn;
		ISDN_AddressString* _vlr_Number;
		ISDN_AddressString* _sgsn_Number;
		ExtensionContainer* _extensionContainer;

	};

	// UpdateVcsgLocationRes
	class UpdateVcsgLocationRes : public Sequence {
	public:
		UpdateVcsgLocationRes();
		~UpdateVcsgLocationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_temporaryEmptySubscriptiondataIndicator();
		void set_extensionContainer();
		// nodes
		Null* _temporaryEmptySubscriptiondataIndicator;
		ExtensionContainer* _extensionContainer;

	};

	// CancelVcsgLocationArg
	class CancelVcsgLocationArg : public Sequence {
	public:
		CancelVcsgLocationArg();
		~CancelVcsgLocationArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		Identity* _identity;
		ExtensionContainer* _extensionContainer;

	};

	// CancelVcsgLocationRes
	class CancelVcsgLocationRes : public Sequence {
	public:
		CancelVcsgLocationRes();
		~CancelVcsgLocationRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

}
#endif
