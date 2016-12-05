#ifndef MAP_ER_DATATYPES_H_
#define MAP_ER_DATATYPES_H_

#include<asn1.h>
#include<MAP-SS-DataTypes.h>
#include<MAP-CommonDataTypes.h>
#include<MAP-SS-Code.h>
#include<MAP-ExtensionDataTypes.h>

namespace asn1 {
	// forward declarations
	class RoamingNotAllowedParam;
	class AdditionalRoamingNotAllowedCause;
	class RoamingNotAllowedCause;
	class CallBarredParam;
	class CallBarringCause;
	class ExtensibleCallBarredParam;
	class CUG_RejectParam;
	class CUG_RejectCause;
	class SS_IncompatibilityCause;
	class PW_RegistrationFailureCause;
	class SM_EnumeratedDeliveryFailureCause;
	class SM_DeliveryFailureCause;
	class AbsentSubscriberSM_Param;
	class AbsentSubscriberDiagnosticSM;
	class SystemFailureParam;
	class ExtensibleSystemFailureParam;
	class FailureCauseParam;
	class DataMissingParam;
	class UnexpectedDataParam;
	class FacilityNotSupParam;
	class OR_NotAllowedParam;
	class UnknownSubscriberParam;
	class UnknownSubscriberDiagnostic;
	class NumberChangedParam;
	class UnidentifiedSubParam;
	class IllegalSubscriberParam;
	class IllegalEquipmentParam;
	class BearerServNotProvParam;
	class TeleservNotProvParam;
	class TracingBufferFullParam;
	class NoRoamingNbParam;
	class AbsentSubscriberParam;
	class AbsentSubscriberReason;
	class BusySubscriberParam;
	class NoSubscriberReplyParam;
	class ForwardingViolationParam;
	class ForwardingFailedParam;
	class ATI_NotAllowedParam;
	class ATSI_NotAllowedParam;
	class ATM_NotAllowedParam;
	class IllegalSS_OperationParam;
	class SS_NotAvailableParam;
	class SS_SubscriptionViolationParam;
	class InformationNotAvailableParam;
	class SubBusyForMT_SMS_Param;
	class MessageWaitListFullParam;
	class ResourceLimitationParam;
	class NoGroupCallNbParam;
	class IncompatibleTerminalParam;
	class ShortTermDenialParam;
	class LongTermDenialParam;
	class UnauthorizedRequestingNetwork_Param;
	class UnauthorizedLCSClient_Param;
	class UnauthorizedLCSClient_Diagnostic;
	class PositionMethodFailure_Param;
	class PositionMethodFailure_Diagnostic;
	class UnknownOrUnreachableLCSClient_Param;
	class MM_EventNotSupported_Param;
	class TargetCellOutsideGCA_Param;
	class OngoingGroupCallParam;

	

	// RoamingNotAllowedParam
	class RoamingNotAllowedParam : public Sequence {
	public:
		RoamingNotAllowedParam();
		~RoamingNotAllowedParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_additionalRoamingNotAllowedCause();
		// nodes
		RoamingNotAllowedCause* _roamingNotAllowedCause;
		ExtensionContainer* _extensionContainer;
		AdditionalRoamingNotAllowedCause* _additionalRoamingNotAllowedCause;

	};

	// AdditionalRoamingNotAllowedCause
	class AdditionalRoamingNotAllowedCause : public Enumerated {
	public:
		AdditionalRoamingNotAllowedCause();
		~AdditionalRoamingNotAllowedCause();
		static const int _supportedRAT_TypesNotAllowed = 0;

	};

	// RoamingNotAllowedCause
	class RoamingNotAllowedCause : public Enumerated {
	public:
		RoamingNotAllowedCause();
		~RoamingNotAllowedCause();
		static const int _plmnRoamingNotAllowed = 0;
		static const int _operatorDeterminedBarring = 3;
	};

	// CallBarredParam
	class CallBarredParam : public Choice {
	public:
		CallBarredParam();
		~CallBarredParam();
		// nodes
		CallBarringCause* _callBarringCause;
		ExtensibleCallBarredParam* _extensibleCallBarredParam;

	};

	// CallBarringCause
	class CallBarringCause : public Enumerated {
	public:
		CallBarringCause();
		~CallBarringCause();
		static const int _barringServiceActive = 0;
		static const int _operatorBarring = 1;
	};

	// ExtensibleCallBarredParam
	class ExtensibleCallBarredParam : public Sequence {
	public:
		ExtensibleCallBarredParam();
		~ExtensibleCallBarredParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_callBarringCause();
		void set_extensionContainer();
		void set_unauthorisedMessageOriginator();
		void set_anonymousCallRejection();
		// nodes
		CallBarringCause* _callBarringCause;
		ExtensionContainer* _extensionContainer;
		Null* _unauthorisedMessageOriginator;
		Null* _anonymousCallRejection;

	};

	// CUG_RejectParam
	class CUG_RejectParam : public Sequence {
	public:
		CUG_RejectParam();
		~CUG_RejectParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_cug_RejectCause();
		void set_extensionContainer();
		// nodes
		CUG_RejectCause* _cug_RejectCause;
		ExtensionContainer* _extensionContainer;

	};

	// CUG_RejectCause
	class CUG_RejectCause : public Enumerated {
	public:
		CUG_RejectCause();
		~CUG_RejectCause();
		static const int _incomingCallsBarredWithinCUG = 0;
		static const int _subscriberNotMemberOfCUG = 1;
		static const int _requestedBasicServiceViolatesCUG_Constraints = 5;
		static const int _calledPartySS_InteractionViolation = 7;

	};

	// SS_IncompatibilityCause
	class SS_IncompatibilityCause : public Sequence {
	public:
		SS_IncompatibilityCause();
		~SS_IncompatibilityCause();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_ss_Code();
		void set_basicService();
		void set_ss_Status();
		// nodes
		SS_Code* _ss_Code;
		BasicServiceCode* _basicService;
		SS_Status* _ss_Status;

	};

	// PW_RegistrationFailureCause
	class PW_RegistrationFailureCause : public Enumerated {
	public:
		PW_RegistrationFailureCause();
		~PW_RegistrationFailureCause();
		static const int _undetermined = 0;
		static const int _invalidFormat = 1;
		static const int _newPasswordsMismatch = 2;
	};

	// SM_EnumeratedDeliveryFailureCause
	class SM_EnumeratedDeliveryFailureCause : public Enumerated {
	public:
		SM_EnumeratedDeliveryFailureCause();
		~SM_EnumeratedDeliveryFailureCause();
 		static const int _memoryCapacityExceeded = 0;
 		static const int _equipmentProtocolError = 1;
 		static const int _equipmentNotSM_Equipped = 2;
 		static const int _unknownServiceCentre = 3;
 		static const int _sc_Congestion = 4;
 		static const int _invalidSME_Address = 5;
 		static const int _subscriberNotSC_Subscriber = 6;

	};

	// SM_DeliveryFailureCause
	class SM_DeliveryFailureCause : public Sequence {
	public:
		SM_DeliveryFailureCause();
		~SM_DeliveryFailureCause();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_diagnosticInfo();
		void set_extensionContainer();
		// nodes
		SM_EnumeratedDeliveryFailureCause* _sm_EnumeratedDeliveryFailureCause;
		SignalInfo* _diagnosticInfo;
		ExtensionContainer* _extensionContainer;

	};

	// AbsentSubscriberSM_Param
	class AbsentSubscriberSM_Param : public Sequence {
	public:
		AbsentSubscriberSM_Param();
		~AbsentSubscriberSM_Param();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_absentSubscriberDiagnosticSM();
		void set_extensionContainer();
		void set_additionalAbsentSubscriberDiagnosticSM();
		void set_imsi();
		// nodes
		AbsentSubscriberDiagnosticSM* _absentSubscriberDiagnosticSM;
		ExtensionContainer* _extensionContainer;
		AbsentSubscriberDiagnosticSM* _additionalAbsentSubscriberDiagnosticSM;
		IMSI* _imsi;

	};

	// AbsentSubscriberDiagnosticSM
	class AbsentSubscriberDiagnosticSM : public Integer {
	public:
		AbsentSubscriberDiagnosticSM();
		~AbsentSubscriberDiagnosticSM();

	};

	// SystemFailureParam
	class SystemFailureParam : public Choice {
	public:
		SystemFailureParam();
		~SystemFailureParam();
		// nodes
		NetworkResource* _networkResource;
		ExtensibleSystemFailureParam* _extensibleSystemFailureParam;

	};

	// ExtensibleSystemFailureParam
	class ExtensibleSystemFailureParam : public Sequence {
	public:
		ExtensibleSystemFailureParam();
		~ExtensibleSystemFailureParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_networkResource();
		void set_extensionContainer();
		void set_additionalNetworkResource();
		void set_failureCauseParam();
		// nodes
		NetworkResource* _networkResource;
		ExtensionContainer* _extensionContainer;
		AdditionalNetworkResource* _additionalNetworkResource;
		FailureCauseParam* _failureCauseParam;

	};

	// FailureCauseParam
	class FailureCauseParam : public Enumerated {
	public:
		FailureCauseParam();
		~FailureCauseParam();
		static const int _limitReachedOnNumberOfConcurrentLocationRequests = 0;

	};

	// DataMissingParam
	class DataMissingParam : public Sequence {
	public:
		DataMissingParam();
		~DataMissingParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// UnexpectedDataParam
	class UnexpectedDataParam : public Sequence {
	public:
		UnexpectedDataParam();
		~UnexpectedDataParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_unexpectedSubscriber();
		// nodes
		ExtensionContainer* _extensionContainer;
		Null* _unexpectedSubscriber;

	};

	// FacilityNotSupParam
	class FacilityNotSupParam : public Sequence {
	public:
		FacilityNotSupParam();
		~FacilityNotSupParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_shapeOfLocationEstimateNotSupported();
		void set_neededLcsCapabilityNotSupportedInServingNode();
		// nodes
		ExtensionContainer* _extensionContainer;
		Null* _shapeOfLocationEstimateNotSupported;
		Null* _neededLcsCapabilityNotSupportedInServingNode;

	};

	// OR_NotAllowedParam
	class OR_NotAllowedParam : public Sequence {
	public:
		OR_NotAllowedParam();
		~OR_NotAllowedParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// UnknownSubscriberParam
	class UnknownSubscriberParam : public Sequence {
	public:
		UnknownSubscriberParam();
		~UnknownSubscriberParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_unknownSubscriberDiagnostic();
		// nodes
		ExtensionContainer* _extensionContainer;
		UnknownSubscriberDiagnostic* _unknownSubscriberDiagnostic;

	};

	// UnknownSubscriberDiagnostic
	class UnknownSubscriberDiagnostic : public Enumerated {
	public:
		UnknownSubscriberDiagnostic();
		~UnknownSubscriberDiagnostic();
		static const int _imsiUnknown = 0;
		static const int _gprs_eps_SubscriptionUnknown = 1;
		static const int _npdbMismatch = 2;

	};

	// NumberChangedParam
	class NumberChangedParam : public Sequence {
	public:
		NumberChangedParam();
		~NumberChangedParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// UnidentifiedSubParam
	class UnidentifiedSubParam : public Sequence {
	public:
		UnidentifiedSubParam();
		~UnidentifiedSubParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// IllegalSubscriberParam
	class IllegalSubscriberParam : public Sequence {
	public:
		IllegalSubscriberParam();
		~IllegalSubscriberParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// IllegalEquipmentParam
	class IllegalEquipmentParam : public Sequence {
	public:
		IllegalEquipmentParam();
		~IllegalEquipmentParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// BearerServNotProvParam
	class BearerServNotProvParam : public Sequence {
	public:
		BearerServNotProvParam();
		~BearerServNotProvParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// TeleservNotProvParam
	class TeleservNotProvParam : public Sequence {
	public:
		TeleservNotProvParam();
		~TeleservNotProvParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// TracingBufferFullParam
	class TracingBufferFullParam : public Sequence {
	public:
		TracingBufferFullParam();
		~TracingBufferFullParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// NoRoamingNbParam
	class NoRoamingNbParam : public Sequence {
	public:
		NoRoamingNbParam();
		~NoRoamingNbParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// AbsentSubscriberParam
	class AbsentSubscriberParam : public Sequence {
	public:
		AbsentSubscriberParam();
		~AbsentSubscriberParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_absentSubscriberReason();
		// nodes
		ExtensionContainer* _extensionContainer;
		AbsentSubscriberReason* _absentSubscriberReason;

	};

	// AbsentSubscriberReason
	class AbsentSubscriberReason : public Enumerated {
	public:
		AbsentSubscriberReason();
		~AbsentSubscriberReason();
		static const int _imsiDetach = 0;
		static const int _restrictedArea = 1;
		static const int _noPageResponse = 2;
		static const int _purgedMS = 3;
		static const int _mtRoamingRetry = 4;
		static const int _busySubscriber = 5;

	};

	// BusySubscriberParam
	class BusySubscriberParam : public Sequence {
	public:
		BusySubscriberParam();
		~BusySubscriberParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_ccbs_Possible();
		void set_ccbs_Busy();
		// nodes
		ExtensionContainer* _extensionContainer;
		Null* _ccbs_Possible;
		Null* _ccbs_Busy;

	};

	// NoSubscriberReplyParam
	class NoSubscriberReplyParam : public Sequence {
	public:
		NoSubscriberReplyParam();
		~NoSubscriberReplyParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// ForwardingViolationParam
	class ForwardingViolationParam : public Sequence {
	public:
		ForwardingViolationParam();
		~ForwardingViolationParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// ForwardingFailedParam
	class ForwardingFailedParam : public Sequence {
	public:
		ForwardingFailedParam();
		~ForwardingFailedParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// ATI_NotAllowedParam
	class ATI_NotAllowedParam : public Sequence {
	public:
		ATI_NotAllowedParam();
		~ATI_NotAllowedParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// ATSI_NotAllowedParam
	class ATSI_NotAllowedParam : public Sequence {
	public:
		ATSI_NotAllowedParam();
		~ATSI_NotAllowedParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// ATM_NotAllowedParam
	class ATM_NotAllowedParam : public Sequence {
	public:
		ATM_NotAllowedParam();
		~ATM_NotAllowedParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// IllegalSS_OperationParam
	class IllegalSS_OperationParam : public Sequence {
	public:
		IllegalSS_OperationParam();
		~IllegalSS_OperationParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// SS_NotAvailableParam
	class SS_NotAvailableParam : public Sequence {
	public:
		SS_NotAvailableParam();
		~SS_NotAvailableParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// SS_SubscriptionViolationParam
	class SS_SubscriptionViolationParam : public Sequence {
	public:
		SS_SubscriptionViolationParam();
		~SS_SubscriptionViolationParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// InformationNotAvailableParam
	class InformationNotAvailableParam : public Sequence {
	public:
		InformationNotAvailableParam();
		~InformationNotAvailableParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// SubBusyForMT_SMS_Param
	class SubBusyForMT_SMS_Param : public Sequence {
	public:
		SubBusyForMT_SMS_Param();
		~SubBusyForMT_SMS_Param();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		void set_gprsConnectionSuspended();
		// nodes
		ExtensionContainer* _extensionContainer;
		Null* _gprsConnectionSuspended;

	};

	// MessageWaitListFullParam
	class MessageWaitListFullParam : public Sequence {
	public:
		MessageWaitListFullParam();
		~MessageWaitListFullParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// ResourceLimitationParam
	class ResourceLimitationParam : public Sequence {
	public:
		ResourceLimitationParam();
		~ResourceLimitationParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// NoGroupCallNbParam
	class NoGroupCallNbParam : public Sequence {
	public:
		NoGroupCallNbParam();
		~NoGroupCallNbParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// IncompatibleTerminalParam
	class IncompatibleTerminalParam : public Sequence {
	public:
		IncompatibleTerminalParam();
		~IncompatibleTerminalParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// ShortTermDenialParam
	class ShortTermDenialParam : public Sequence {
	public:
		ShortTermDenialParam();
		~ShortTermDenialParam();
		// nodes

	};

	// LongTermDenialParam
	class LongTermDenialParam : public Sequence {
	public:
		LongTermDenialParam();
		~LongTermDenialParam();
		// nodes

	};

	// UnauthorizedRequestingNetwork_Param
	class UnauthorizedRequestingNetwork_Param : public Sequence {
	public:
		UnauthorizedRequestingNetwork_Param();
		~UnauthorizedRequestingNetwork_Param();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// UnauthorizedLCSClient_Param
	class UnauthorizedLCSClient_Param : public Sequence {
	public:
		UnauthorizedLCSClient_Param();
		~UnauthorizedLCSClient_Param();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_unauthorizedLCSClient_Diagnostic();
		void set_extensionContainer();
		// nodes
		UnauthorizedLCSClient_Diagnostic* _unauthorizedLCSClient_Diagnostic;
		ExtensionContainer* _extensionContainer;

	};

	// UnauthorizedLCSClient_Diagnostic
	class UnauthorizedLCSClient_Diagnostic : public Enumerated {
	public:
		UnauthorizedLCSClient_Diagnostic();
		~UnauthorizedLCSClient_Diagnostic();
		static const int _noAdditionalInformation = 0;
		static const int _clientNotInMSPrivacyExceptionList = 1;
		static const int _callToClientNotSetup = 2;
		static const int _privacyOverrideNotApplicable = 3;
		static const int _disallowedByLocalRegulatoryRequirements = 4;
		static const int _unauthorizedPrivacyClass = 5;
		static const int _unauthorizedCallSessionUnrelatedExternalClient = 6;
		static const int _unauthorizedCallSessionRelatedExternalClient = 7;
	};

	// PositionMethodFailure_Param
	class PositionMethodFailure_Param : public Sequence {
	public:
		PositionMethodFailure_Param();
		~PositionMethodFailure_Param();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_positionMethodFailure_Diagnostic();
		void set_extensionContainer();
		// nodes
		PositionMethodFailure_Diagnostic* _positionMethodFailure_Diagnostic;
		ExtensionContainer* _extensionContainer;

	};

	// PositionMethodFailure_Diagnostic
	class PositionMethodFailure_Diagnostic : public Enumerated {
	public:
		PositionMethodFailure_Diagnostic();
		~PositionMethodFailure_Diagnostic();
		static const int _congestion = 0;
		static const int _insufficientResources = 1;
		static const int _insufficientMeasurementData = 2;
		static const int _inconsistentMeasurementData = 3;
		static const int _locationProcedureNotCompleted = 4;
		static const int _locationProcedureNotSupportedByTargetMS = 5;
		static const int _qoSNotAttainable = 6;
		static const int _positionMethodNotAvailableInNetwork = 7;
		static const int _positionMethodNotAvailableInLocationArea = 8;
	};

	// UnknownOrUnreachableLCSClient_Param
	class UnknownOrUnreachableLCSClient_Param : public Sequence {
	public:
		UnknownOrUnreachableLCSClient_Param();
		~UnknownOrUnreachableLCSClient_Param();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// MM_EventNotSupported_Param
	class MM_EventNotSupported_Param : public Sequence {
	public:
		MM_EventNotSupported_Param();
		~MM_EventNotSupported_Param();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// TargetCellOutsideGCA_Param
	class TargetCellOutsideGCA_Param : public Sequence {
	public:
		TargetCellOutsideGCA_Param();
		~TargetCellOutsideGCA_Param();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

	// OngoingGroupCallParam
	class OngoingGroupCallParam : public Sequence {
	public:
		OngoingGroupCallParam();
		~OngoingGroupCallParam();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extensionContainer();
		// nodes
		ExtensionContainer* _extensionContainer;

	};

}
#endif
