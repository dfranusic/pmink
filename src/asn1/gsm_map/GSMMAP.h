#ifndef GSMMAP_H_
#define GSMMAP_H_

#include<asn1.h>
#include<MAP-CommonDataTypes.h>
#include<MAP-ExtensionDataTypes.h>
#include<MAP-CH-DataTypes.h>

namespace asn1 {
    namespace gsmmap {
	// forward declarations
	class Component;
	class Invoke;
	class InvokeParameter;
	class ReturnResult;
	class ReturnResultParameter;
	class ReturnError;
	class ReturnErrorParameter;
	class Reject;
	class InvokeIdType;
	class MAP_OPERATION;
	class GSMMAPOperationLocalvalue;
	class OperationLocalvalue;
	class MAP_ERROR;
	class GSMMAPLocalErrorcode;
	class LocalErrorcode;
	class GeneralProblem;
	class InvokeProblem;
	class ReturnResultProblem;
	class ReturnErrorProblem;
	class Bss_APDU;
	class ProvideSIWFSNumberArg;
	class ProvideSIWFSNumberRes;
	class CallDirection;
	class PurgeMSArgV2;
	class PrepareHO_ArgOld;
	class PrepareHO_ResOld;
	class SendAuthenticationInfoResOld;
	//class RAND;
	//class SRES;
	//class Kc;
	class SendIdentificationResV2;
	class TripletListold;
	class AuthenticationTriplet_v2;
	class SIWFSSignallingModifyArg;
	class SIWFSSignallingModifyRes;
	class NewPassword;
	class GetPasswordArg;
	class CurrentPassword;
	class SecureTransportArg;
	class SecureTransportErrorParam;
	class SecureTransportRes;
	class SecurityHeader;
	class ProtectedPayload;
	class SecurityParametersIndex;
	class InitialisationVector;
	class OriginalComponentIdentifier;
	class OperationCode;
	class ErrorCode;
	class PlmnContainer;
	//class Category;
	class ForwardSM_Arg;
	class SM_RP_DAold;
	class SM_RP_OAold;
	class accessType_id;
	class accessTypeNotAllowed_id;
	class SendRoutingInfoArgV2;
	class SendRoutingInfoResV2;
	class BeginSubscriberActivityArg;
	class ReturnResult_resultretres;
	class Reject_invokeIDRej;
	class Reject_problem;
	class SendAuthenticationInfoResOld_SEQUENCE;
	class PlmnContainer_operatorSS_Code;


	enum AddresStringNAIType{
	  ASNAI_UNKNOWN			= 0,
	  ASNAI_INTERNATIONAL		= 1 << 4,
	  ASNAI_NATIONAL		= 2 << 4,
	  ASNAI_NETWORK_SPECIFIC	= 3 << 4,
	  ASNAI_SUBSCRIBER		= 4 << 4,
	  ASNAI_ABBREVIATED		= 6 << 4

	};

	enum AddressStringNPType{
	    ASNP_UNKNOWN		= 0,
	    ASNP_ISDN_TELEPHONE		= 1,
	    ASNP_DATA_X121		= 3,
	    ASNP_TELEX			= 4,
	    ASNP_LAND_MOBILE		= 6,
	    ASNP_NATIONAL		= 8,
	    ASNP_PRIVATE		= 9
	};

	// ReturnResult_resultretres
	class ReturnResult_resultretres : public Sequence {
	public:
		ReturnResult_resultretres();
		~ReturnResult_resultretres();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_returnparameter();
		// nodes
		MAP_OPERATION* _opCode;
		ReturnResultParameter* _returnparameter;

	};

	// Reject_invokeIDRej
	class Reject_invokeIDRej : public Choice {
	public:
		Reject_invokeIDRej();
		~Reject_invokeIDRej();
		// nodes
		InvokeIdType* _derivable;
		Null* _not_derivable;

	};

	// Reject_problem
	class Reject_problem : public Choice {
	public:
		Reject_problem();
		~Reject_problem();
		// nodes
		GeneralProblem* _generalProblem;
		InvokeProblem* _invokeProblem;
		ReturnResultProblem* _returnResultProblem;
		ReturnErrorProblem* _returnErrorProblem;

	};

	// SendAuthenticationInfoResOld_SEQUENCE
	class SendAuthenticationInfoResOld_SEQUENCE : public Sequence {
	public:
		SendAuthenticationInfoResOld_SEQUENCE();
		~SendAuthenticationInfoResOld_SEQUENCE();
		// nodes
		RAND* _rand;
		SRES* _sres;
		Kc* _kc;

	};

	// PlmnContainer_operatorSS_Code
	class PlmnContainer_operatorSS_Code : public Sequence_of {
	public:
		PlmnContainer_operatorSS_Code();
		~PlmnContainer_operatorSS_Code();
		// nodes
		Octet_string* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	

	// Component
	class Component : public Choice {
	public:
		Component();
		~Component();
		// nodes
		Invoke* _invoke;
		ReturnResult* _returnResultLast;
		ReturnError* _returnError;
		Reject* _reject;
		ReturnResult* _returnResultNotLast;

	};

	// Invoke
	class Invoke : public Sequence {
	public:
		Invoke();
		~Invoke();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_linkedID();
		void set_invokeparameter();
		// nodes
		InvokeIdType* _invokeID;
		InvokeIdType* _linkedID;
		MAP_OPERATION* _opCode;
		InvokeParameter* _invokeparameter;

	};

	// InvokeParameter
	class InvokeParameter : public Any {
	public:
		InvokeParameter();
		~InvokeParameter();

	};

	// ReturnResult
	class ReturnResult : public Sequence {
	public:
		ReturnResult();
		~ReturnResult();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_resultretres();
		// nodes
		InvokeIdType* _invokeID;
		ReturnResult_resultretres* _resultretres;

	};

	// ReturnResultParameter
	class ReturnResultParameter : public Any {
	public:
		ReturnResultParameter();
		~ReturnResultParameter();

	};

	// ReturnError
	class ReturnError : public Sequence {
	public:
		ReturnError();
		~ReturnError();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_parameter();
		// nodes
		InvokeIdType* _invokeID;
		MAP_ERROR* _errorCode;
		ReturnErrorParameter* _parameter;

	};

	// ReturnErrorParameter
	class ReturnErrorParameter : public Any {
	public:
		ReturnErrorParameter();
		~ReturnErrorParameter();

	};

	// Reject
	class Reject : public Sequence {
	public:
		Reject();
		~Reject();
		// nodes
		Reject_invokeIDRej* _invokeIDRej;
		Reject_problem* _problem;

	};

	// InvokeIdType
	class InvokeIdType : public Integer {
	public:
		InvokeIdType();
		~InvokeIdType();

	};

	// MAP_OPERATION
	class MAP_OPERATION : public Choice {
	public:
		MAP_OPERATION();
		~MAP_OPERATION();
		// nodes
		OperationLocalvalue* _localValue;
		Object_identifier* _globalValue;

	};

	// GSMMAPOperationLocalvalue
	class GSMMAPOperationLocalvalue : public Integer {
	public:
		GSMMAPOperationLocalvalue();
		~GSMMAPOperationLocalvalue();
		static const int _updateLocation = 2;
		static const int _cancelLocation = 3;
		static const int _provideRoamingNumber = 4;
		static const int _noteSubscriberDataModified = 5;
		static const int _resumeCallHandling = 6;
		static const int _insertSubscriberData = 7;
		static const int _deleteSubscriberData = 8;
		static const int _sendParameters = 9;
		static const int _registerSS = 10;
		static const int _eraseSS = 11;
		static const int _activateSS = 12;
		static const int _deactivateSS = 13;
		static const int _interrogateSS = 14;
		static const int _authenticationFailureReport = 15;
		static const int _notifySS = 16;
		static const int _registerPassword = 17;
		static const int _getPassword = 18;
		static const int _processUnstructuredSS_Data = 19;
		static const int _releaseResources = 20;
		static const int _mt_ForwardSM_VGCS = 21;
		static const int _sendRoutingInfo = 22;
		static const int _updateGprsLocation = 23;
		static const int _sendRoutingInfoForGprs = 24;
		static const int _failureReport = 25;
		static const int _noteMsPresentForGprs = 26;
		static const int _unAllocated_1 = 27;
		static const int _performHandover = 28;
		static const int _sendEndSignal = 29;
		static const int _performSubsequentHandover = 30;
		static const int _provideSIWFSNumber = 31;
		static const int _sIWFSSignallingModify = 32;
		static const int _processAccessSignalling = 33;
		static const int _forwardAccessSignalling = 34;
		static const int _noteInternalHandover = 35;
		static const int _unAllocated_2 = 36;
		static const int _reset = 37;
		static const int _forwardCheckSS = 38;
		static const int _prepareGroupCall = 39;
		static const int _sendGroupCallEndSignal = 40;
		static const int _processGroupCallSignalling = 41;
		static const int _forwardGroupCallSignalling = 42;
		static const int _checkIMEI = 43;
		static const int _mt_forwardSM = 44;
		static const int _sendRoutingInfoForSM = 45;
		static const int _mo_forwardSM = 46;
		static const int _reportSM_DeliveryStatus = 47;
		static const int _noteSubscriberPresent = 48;
		static const int _alertServiceCentreWithoutResult = 49;
		static const int _activateTraceMode = 50;
		static const int _deactivateTraceMode = 51;
		static const int _traceSubscriberActivity = 52;
		static const int _unAllocated_3 = 53;
		static const int _beginSubscriberActivity = 54;
		static const int _sendIdentification = 55;
		static const int _sendAuthenticationInfo = 56;
		static const int _restoreData = 57;
		static const int _sendIMSI = 58;
		static const int _processUnstructuredSS_Request = 59;
		static const int _unstructuredSS_Request = 60;
		static const int _unstructuredSS_Notify = 61;
		static const int _anyTimeSubscriptionInterrogation = 62;
		static const int _informServiceCentre = 63;
		static const int _alertServiceCentre = 64;
		static const int _anyTimeModification = 65;
		static const int _readyForSM = 66;
		static const int _purgeMS = 67;
		static const int _prepareHandover = 68;
		static const int _prepareSubsequentHandover = 69;
		static const int _provideSubscriberInfo = 70;
		static const int _anyTimeInterrogation = 71;
		static const int _ss_InvocationNotification = 72;
		static const int _setReportingState = 73;
		static const int _statusReport = 74;
		static const int _remoteUserFree = 75;
		static const int _registerCC_Entry = 76;
		static const int _eraseCC_Entry = 77;
		static const int _secureTransportClass1 = 78;
		static const int _secureTransportClass2 = 79;
		static const int _secureTransportClass3 = 80;
		static const int _secureTransportClass4 = 81;
		static const int _unAllocated_4 = 82;
		static const int _provideSubscriberLocation = 83;
		static const int _sendGroupCallInfo = 84;
		static const int _sendRoutingInfoForLCS = 85;
		static const int _subscriberLocationReport = 86;
		static const int _ist_Alert = 87;
		static const int _ist_Command = 88;
		static const int _noteMM_Event = 89;
		static const int _unAllocated_5 = 90;
		static const int _unAllocated_6 = 91;
		static const int _unAllocated_7 = 92;
		static const int _unAllocated_8 = 93;
		static const int _unAllocated_9 = 94;
		static const int _unAllocated_10 = 95;
		static const int _unAllocated_11 = 96;
		static const int _unAllocated_12 = 97;
		static const int _unAllocated_13 = 98;
		static const int _unAllocated_14 = 99;
		static const int _unAllocated_15 = 100;
		static const int _unAllocated_16 = 101;
		static const int _unAllocated_17 = 102;
		static const int _unAllocated_18 = 103;
		static const int _unAllocated_19 = 104;
		static const int _unAllocated_20 = 105;
		static const int _unAllocated_21 = 106;
		static const int _unAllocated_22 = 107;
		static const int _unAllocated_23 = 108;
		static const int _lcs_PeriodicLocationCancellation = 109;
		static const int _lcs_LocationUpdate = 110;
		static const int _lcs_PeriodicLocationRequest = 111;
		static const int _lcs_AreaEventCancellation = 112;
		static const int _lcs_AreaEventReport = 113;
		static const int _lcs_AreaEventRequest = 114;
		static const int _lcs_MOLR = 115;
		static const int _lcs_LocationNotification = 116;
		static const int _callDeflection = 117;
		static const int _userUserService = 118;
		static const int _accessRegisterCCEntry = 119;
		static const int _forwardCUG_Info = 120;
		static const int _splitMPTY = 121;
		static const int _retrieveMPTY = 122;
		static const int _holdMPTY = 123;
		static const int _buildMPTY = 124;
		static const int _forwardChargeAdvice = 125;
		static const int _explicitCT = 126;

	};

	// OperationLocalvalue
	class OperationLocalvalue : public GSMMAPOperationLocalvalue {
	public:
		OperationLocalvalue();
		~OperationLocalvalue();

	};

	// MAP_ERROR
	class MAP_ERROR : public Choice {
	public:
		MAP_ERROR();
		~MAP_ERROR();
		// nodes
		LocalErrorcode* _localValue;
		Object_identifier* _globalValue;

	};

	// GSMMAPLocalErrorcode
	class GSMMAPLocalErrorcode : public Integer {
	public:
		GSMMAPLocalErrorcode();
		~GSMMAPLocalErrorcode();
		static const int _unknownSubscriber = 1;
		static const int _unknownBaseStation = 2;
		static const int _unknownMSC = 3;
		static const int _secureTransportError = 4;
		static const int _unidentifiedSubscriber = 5;
		static const int _absentSubscriberSM = 6;
		static const int _unknownEquipment = 7;
		static const int _roamingNotAllowed = 8;
		static const int _illegalSubscriber = 9;
		static const int _bearerServiceNotProvisioned = 10;
		static const int _teleserviceNotProvisioned = 11;
		static const int _illegalEquipment = 12;
		static const int _callBarred = 13;
		static const int _forwardingViolation = 14;
		static const int _cug_Reject = 15;
		static const int _illegalSS_Operation = 16;
		static const int _ss_ErrorStatus = 17;
		static const int _ss_NotAvailable = 18;
		static const int _ss_SubscriptionViolatio = 19;
		static const int _ss_Incompatibility = 20;
		static const int _facilityNotSupported = 21;
		static const int _ongoingGroupCall = 22;
		static const int _invalidTargetBaseStation = 23;
		static const int _noRadioResourceAvailable = 24;
		static const int _noHandoverNumberAvailable = 25;
		static const int _subsequentHandoverFailure = 26;
		static const int _absentSubscriber = 27;
		static const int _incompatibleTerminal = 28;
		static const int _shortTermDenial = 29;
		static const int _longTermDenial = 30;
		static const int _subscriberBusyForMT_SMS = 31;
		static const int _sm_DeliveryFailure = 32;
		static const int _messageWaitingListFull = 33;
		static const int _systemFailure = 34;
		static const int _dataMissing = 35;
		static const int _unexpectedDataValue = 36;
		static const int _pw_RegistrationFailur = 37;
		static const int _negativePW_Check = 38;
		static const int _noRoamingNumberAvailable = 39;
		static const int _tracingBufferFull = 40;
		static const int _targetCellOutsideGroupCallArea = 42;
		static const int _numberOfPW_AttemptsViolation = 43;
		static const int _numberChanged = 44;
		static const int _busySubscriber = 45;
		static const int _noSubscriberReply = 46;
		static const int _forwardingFailed = 47;
		static const int _or_NotAllowed = 48;
		static const int _ati_NotAllowed = 49;
		static const int _noGroupCallNumberAvailable = 50;
		static const int _resourceLimitation = 51;
		static const int _unauthorizedRequestingNetwork = 52;
		static const int _unauthorizedLCSClient = 53;
		static const int _positionMethodFailure = 54;
		static const int _unknownOrUnreachableLCSClient = 58;
		static const int _mm_EventNotSupported = 59;
		static const int _atsi_NotAllowed = 60;
		static const int _atm_NotAllowed = 61;
		static const int _informationNotAvailabl = 62;
		static const int _unknownAlphabe = 71;
		static const int _ussd_Busy = 72;

	};

	// LocalErrorcode
	class LocalErrorcode : public GSMMAPLocalErrorcode {
	public:
		LocalErrorcode();
		~LocalErrorcode();

	};

	// GeneralProblem
	class GeneralProblem : public Integer {
	public:
		GeneralProblem();
		~GeneralProblem();
		static const int _unrecognizedComponent = 0;
		static const int _mistypedComponent = 1;
		static const int _badlyStructuredComponent = 2;

	};

	// InvokeProblem
	class InvokeProblem : public Integer {
	public:
		InvokeProblem();
		~InvokeProblem();
		static const int _duplicateInvokeID = 0;
		static const int _unrecognizedOperation = 1;
		static const int _mistypedParameter = 2;
		static const int _resourceLimitation = 3;
		static const int _initiatingRelease = 4;
		static const int _unrecognizedLinkedID = 5;
		static const int _linkedResponseUnexpected = 6;
		static const int _unexpectedLinkedOperation = 7;

	};

	// ReturnResultProblem
	class ReturnResultProblem : public Integer {
	public:
		ReturnResultProblem();
		~ReturnResultProblem();
		static const int _unrecognizedInvokeID = 0;
		static const int _returnResultUnexpected = 1;
		static const int _mistypedParameter = 2;

	};

	// ReturnErrorProblem
	class ReturnErrorProblem : public Integer {
	public:
		ReturnErrorProblem();
		~ReturnErrorProblem();
		static const int _unrecognizedInvokeID = 0;
		static const int _returnErrorUnexpected = 1;
		static const int _unrecognizedError = 2;
		static const int _unexpectedError = 3;
		static const int _mistypedParameter = 4;

	};

	// Bss_APDU
	class Bss_APDU : public Sequence {
	public:
		Bss_APDU();
		~Bss_APDU();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ProtocolId* _protocolId;
		SignalInfo* _signalInfo;
		ExtensionContainer* _extensionContainer;

	};

	// ProvideSIWFSNumberArg
	class ProvideSIWFSNumberArg : public Sequence {
	public:
		ProvideSIWFSNumberArg();
		~ProvideSIWFSNumberArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_lowerLayerCompatibility();
		void set_highLayerCompatibility();
		void set_extensionContainer();
		// nodes
		ExternalSignalInfo* _gsm_BearerCapability;
		ExternalSignalInfo* _isdn_BearerCapability;
		CallDirection* _call_Direction;
		ISDN_AddressString* _b_Subscriber_Address;
		ExternalSignalInfo* _chosenChannel;
		ExternalSignalInfo* _lowerLayerCompatibility;
		ExternalSignalInfo* _highLayerCompatibility;
		ExtensionContainer* _extensionContainer;

	};

	// ProvideSIWFSNumberRes
	class ProvideSIWFSNumberRes : public Sequence {
	public:
		ProvideSIWFSNumberRes();
		~ProvideSIWFSNumberRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ISDN_AddressString* _sIWFSNumber;
		ExtensionContainer* _extensionContainer;

	};

	// CallDirection
	class CallDirection : public Octet_string {
	public:
		CallDirection();
		~CallDirection();

	};

	// PurgeMSArgV2
	class PurgeMSArgV2 : public Sequence {
	public:
		PurgeMSArgV2();
		~PurgeMSArgV2();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_vlr_Number();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _vlr_Number;

	};

	// PrepareHO_ArgOld
	class PrepareHO_ArgOld : public Sequence {
	public:
		PrepareHO_ArgOld();
		~PrepareHO_ArgOld();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_targetCellId();
		void set_ho_NumberNotRequired();
		void set_bss_APDU();
		// nodes
		GlobalCellId* _targetCellId;
		Null* _ho_NumberNotRequired;
		Bss_APDU* _bss_APDU;

	};

	// PrepareHO_ResOld
	class PrepareHO_ResOld : public Sequence {
	public:
		PrepareHO_ResOld();
		~PrepareHO_ResOld();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_handoverNumber();
		void set_bss_APDU();
		// nodes
		ISDN_AddressString* _handoverNumber;
		Bss_APDU* _bss_APDU;

	};

	// SendAuthenticationInfoResOld
	class SendAuthenticationInfoResOld : public Sequence_of {
	public:
		SendAuthenticationInfoResOld();
		~SendAuthenticationInfoResOld();
		// nodes
		SendAuthenticationInfoResOld_SEQUENCE* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// RAND
	/*
	class RAND : public Octet_string {
	public:
		RAND();
		~RAND();

	};
	*/

	// SRES
	/*
	class SRES : public Octet_string {
	public:
		SRES();
		~SRES();

	};
	*/
	/*
	// Kc
	class Kc : public Octet_string {
	public:
		Kc();
		~Kc();

	};
	*/

	// SendIdentificationResV2
	class SendIdentificationResV2 : public Sequence {
	public:
		SendIdentificationResV2();
		~SendIdentificationResV2();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_imsi();
		void set_tripletList();
		// nodes
		IMSI* _imsi;
		TripletListold* _tripletList;

	};

	// TripletListold
	class TripletListold : public Sequence_of {
	public:
		TripletListold();
		~TripletListold();
		// nodes
		AuthenticationTriplet_v2* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// AuthenticationTriplet_v2
	class AuthenticationTriplet_v2 : public Sequence {
	public:
		AuthenticationTriplet_v2();
		~AuthenticationTriplet_v2();
		// nodes
		RAND* _rand;
		SRES* _sres;
		Kc* _kc;

	};

	// SIWFSSignallingModifyArg
	class SIWFSSignallingModifyArg : public Sequence {
	public:
		SIWFSSignallingModifyArg();
		~SIWFSSignallingModifyArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_channelType();
		void set_chosenChannel();
		void set_extensionContainer();
		// nodes
		ExternalSignalInfo* _channelType;
		ExternalSignalInfo* _chosenChannel;
		ExtensionContainer* _extensionContainer;

	};

	// SIWFSSignallingModifyRes
	class SIWFSSignallingModifyRes : public Sequence {
	public:
		SIWFSSignallingModifyRes();
		~SIWFSSignallingModifyRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_channelType();
		void set_extensionContainer();
		// nodes
		ExternalSignalInfo* _channelType;
		ExtensionContainer* _extensionContainer;

	};

	// NewPassword
	class NewPassword : public Numeric_string {
	public:
		NewPassword();
		~NewPassword();

	};

	// GetPasswordArg
	class GetPasswordArg : public Enumerated {
	public:
		GetPasswordArg();
		~GetPasswordArg();
            static const int _enterPW = 0;
            static const int _enterNewPW = 1;
            static const int _enterNewPW_Again = 2;

	};

	// CurrentPassword
	class CurrentPassword : public Numeric_string {
	public:
		CurrentPassword();
		~CurrentPassword();

	};

	// SecureTransportArg
	class SecureTransportArg : public Sequence {
	public:
		SecureTransportArg();
		~SecureTransportArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_protectedPayload();
		// nodes
		SecurityHeader* _securityHeader;
		ProtectedPayload* _protectedPayload;

	};

	// SecureTransportErrorParam
	class SecureTransportErrorParam : public Sequence {
	public:
		SecureTransportErrorParam();
		~SecureTransportErrorParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_protectedPayload();
		// nodes
		SecurityHeader* _securityHeader;
		ProtectedPayload* _protectedPayload;

	};

	// SecureTransportRes
	class SecureTransportRes : public Sequence {
	public:
		SecureTransportRes();
		~SecureTransportRes();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_protectedPayload();
		// nodes
		SecurityHeader* _securityHeader;
		ProtectedPayload* _protectedPayload;

	};

	// SecurityHeader
	class SecurityHeader : public Sequence {
	public:
		SecurityHeader();
		~SecurityHeader();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_initialisationVector();
		// nodes
		SecurityParametersIndex* _securityParametersIndex;
		OriginalComponentIdentifier* _originalComponentIdentifier;
		InitialisationVector* _initialisationVector;

	};

	// ProtectedPayload
	class ProtectedPayload : public Octet_string {
	public:
		ProtectedPayload();
		~ProtectedPayload();

	};

	// SecurityParametersIndex
	class SecurityParametersIndex : public Octet_string {
	public:
		SecurityParametersIndex();
		~SecurityParametersIndex();

	};

	// InitialisationVector
	class InitialisationVector : public Octet_string {
	public:
		InitialisationVector();
		~InitialisationVector();

	};

	// OriginalComponentIdentifier
	class OriginalComponentIdentifier : public Choice {
	public:
		OriginalComponentIdentifier();
		~OriginalComponentIdentifier();
		// nodes
		OperationCode* _operationCode;
		ErrorCode* _errorCode;
		Null* _userInfo;

	};

	// OperationCode
	class OperationCode : public Choice {
	public:
		OperationCode();
		~OperationCode();
		// nodes
		Integer* _localValue;
		Object_identifier* _globalValue;

	};

	// ErrorCode
	class ErrorCode : public Choice {
	public:
		ErrorCode();
		~ErrorCode();
		// nodes
		Integer* _localValue;
		Object_identifier* _globalValue;

	};

	// PlmnContainer
	class PlmnContainer : public Sequence {
	public:
		PlmnContainer();
		~PlmnContainer();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_msisdn();
		void set_category();
		void set_basicService();
		void set_operatorSS_Code();
		// nodes
		ISDN_AddressString* _msisdn;
		Category* _category;
		BasicServiceCode* _basicService;
		PlmnContainer_operatorSS_Code* _operatorSS_Code;

	};

	// Category
	/*
	class Category : public Octet_string {
	public:
		Category();
		~Category();

	};
	*/

	// ForwardSM_Arg
	class ForwardSM_Arg : public Sequence {
	public:
		ForwardSM_Arg();
		~ForwardSM_Arg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_moreMessagesToSend();
		// nodes
		SM_RP_DAold* _sm_RP_DA;
		SM_RP_OAold* _sm_RP_OA;
		SignalInfo* _sm_RP_UI;
		Null* _moreMessagesToSend;

	};

	// SM_RP_DAold
	class SM_RP_DAold : public Choice {
	public:
		SM_RP_DAold();
		~SM_RP_DAold();
		// nodes
		IMSI* _imsi;
		LMSI* _lmsi;
		AddressString* _serviceCentreAddressDA;
		Null* _noSM_RP_DA;

	};

	// SM_RP_OAold
	class SM_RP_OAold : public Choice {
	public:
		SM_RP_OAold();
		~SM_RP_OAold();
		// nodes
		ISDN_AddressString* _msisdn;
		AddressString* _serviceCentreAddressOA;
		Null* _noSM_RP_OA;

	};

	// accessType_id
	class accessType_id : public Object_identifier {
	public:
		accessType_id();
		~accessType_id();

	};

	// accessTypeNotAllowed_id
	class accessTypeNotAllowed_id : public Object_identifier {
	public:
		accessTypeNotAllowed_id();
		~accessTypeNotAllowed_id();

	};

	// SendRoutingInfoArgV2
	class SendRoutingInfoArgV2 : public Sequence {
	public:
		SendRoutingInfoArgV2();
		~SendRoutingInfoArgV2();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cug_CheckInfo();
		void set_numberOfForwarding();
		void set_networkSignalInfo();
		// nodes
		ISDN_AddressString* _msisdn;
		CUG_CheckInfo* _cug_CheckInfo;
		NumberOfForwarding* _numberOfForwarding;
		ExternalSignalInfo* _networkSignalInfo;

	};

	// SendRoutingInfoResV2
	class SendRoutingInfoResV2 : public Sequence {
	public:
		SendRoutingInfoResV2();
		~SendRoutingInfoResV2();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cug_CheckInfo();
		// nodes
		IMSI* _imsi;
		RoutingInfo* _routingInfo;
		CUG_CheckInfo* _cug_CheckInfo;

	};

	// BeginSubscriberActivityArg
	class BeginSubscriberActivityArg : public Sequence {
	public:
		BeginSubscriberActivityArg();
		~BeginSubscriberActivityArg();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_msisdn();
		// nodes
		IMSI* _imsi;
		ISDN_AddressString* _originatingEntityNumber;
		AddressString* _msisdn;

	};

    }

}
#endif
