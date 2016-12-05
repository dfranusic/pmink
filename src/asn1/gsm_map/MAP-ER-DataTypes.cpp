#include<MAP-ER-DataTypes.h>
#include<iostream>

//RoamingNotAllowedParam
asn1::RoamingNotAllowedParam::RoamingNotAllowedParam(){
	strcpy(node_type_name, "RoamingNotAllowedParam");
	// roamingNotAllowedCause
	_roamingNotAllowedCause = NULL;
	_roamingNotAllowedCause = new RoamingNotAllowedCause();
	children.push_back(_roamingNotAllowedCause);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// additionalRoamingNotAllowedCause
	_additionalRoamingNotAllowedCause = NULL;
	children.push_back(_additionalRoamingNotAllowedCause);


}
asn1::RoamingNotAllowedParam::~RoamingNotAllowedParam(){

}

asn1::ASN1Node* asn1::RoamingNotAllowedParam::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_additionalRoamingNotAllowedCause = new AdditionalRoamingNotAllowedCause();
			_additionalRoamingNotAllowedCause->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalRoamingNotAllowedCause->tlv->tag_value = 0;
			children[2] = _additionalRoamingNotAllowedCause;
			return _additionalRoamingNotAllowedCause;

		default: return NULL;
	}
}

void asn1::RoamingNotAllowedParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::RoamingNotAllowedParam::set_additionalRoamingNotAllowedCause(){
	if(_additionalRoamingNotAllowedCause == NULL) _additionalRoamingNotAllowedCause = (AdditionalRoamingNotAllowedCause*)create_node(2);
}

//AdditionalRoamingNotAllowedCause
asn1::AdditionalRoamingNotAllowedCause::AdditionalRoamingNotAllowedCause(){
	strcpy(node_type_name, "AdditionalRoamingNotAllowedCause");

}
asn1::AdditionalRoamingNotAllowedCause::~AdditionalRoamingNotAllowedCause(){

}

//RoamingNotAllowedCause
asn1::RoamingNotAllowedCause::RoamingNotAllowedCause(){
	strcpy(node_type_name, "RoamingNotAllowedCause");

}
asn1::RoamingNotAllowedCause::~RoamingNotAllowedCause(){

}

//CallBarredParam
asn1::CallBarredParam::CallBarredParam(){
	strcpy(node_type_name, "CallBarredParam");
	// callBarringCause
	_callBarringCause = NULL;
	_callBarringCause = new CallBarringCause();
	children.push_back(_callBarringCause);

	// extensibleCallBarredParam
	_extensibleCallBarredParam = NULL;
	_extensibleCallBarredParam = new ExtensibleCallBarredParam();
	children.push_back(_extensibleCallBarredParam);


}
asn1::CallBarredParam::~CallBarredParam(){

}

//CallBarringCause
asn1::CallBarringCause::CallBarringCause(){
	strcpy(node_type_name, "CallBarringCause");

}
asn1::CallBarringCause::~CallBarringCause(){

}

//ExtensibleCallBarredParam
asn1::ExtensibleCallBarredParam::ExtensibleCallBarredParam(){
	strcpy(node_type_name, "ExtensibleCallBarredParam");
	// callBarringCause
	_callBarringCause = NULL;
	children.push_back(_callBarringCause);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// unauthorisedMessageOriginator
	_unauthorisedMessageOriginator = NULL;
	children.push_back(_unauthorisedMessageOriginator);

	// anonymousCallRejection
	_anonymousCallRejection = NULL;
	children.push_back(_anonymousCallRejection);


}
asn1::ExtensibleCallBarredParam::~ExtensibleCallBarredParam(){

}

asn1::ASN1Node* asn1::ExtensibleCallBarredParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_callBarringCause = new CallBarringCause();
			children[0] = _callBarringCause;
			return _callBarringCause;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_unauthorisedMessageOriginator = new Null();
			_unauthorisedMessageOriginator->tlv->tag_class = CONTEXT_SPECIFIC;
			_unauthorisedMessageOriginator->tlv->tag_value = 1;
			children[2] = _unauthorisedMessageOriginator;
			return _unauthorisedMessageOriginator;
		case 3:
			_anonymousCallRejection = new Null();
			_anonymousCallRejection->tlv->tag_class = CONTEXT_SPECIFIC;
			_anonymousCallRejection->tlv->tag_value = 2;
			children[3] = _anonymousCallRejection;
			return _anonymousCallRejection;

		default: return NULL;
	}
}

void asn1::ExtensibleCallBarredParam::set_callBarringCause(){
	if(_callBarringCause == NULL) _callBarringCause = (CallBarringCause*)create_node(0);
}

void asn1::ExtensibleCallBarredParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::ExtensibleCallBarredParam::set_unauthorisedMessageOriginator(){
	if(_unauthorisedMessageOriginator == NULL) _unauthorisedMessageOriginator = (Null*)create_node(2);
}

void asn1::ExtensibleCallBarredParam::set_anonymousCallRejection(){
	if(_anonymousCallRejection == NULL) _anonymousCallRejection = (Null*)create_node(3);
}

//CUG_RejectParam
asn1::CUG_RejectParam::CUG_RejectParam(){
	strcpy(node_type_name, "CUG_RejectParam");
	// cug_RejectCause
	_cug_RejectCause = NULL;
	children.push_back(_cug_RejectCause);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CUG_RejectParam::~CUG_RejectParam(){

}

asn1::ASN1Node* asn1::CUG_RejectParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_cug_RejectCause = new CUG_RejectCause();
			children[0] = _cug_RejectCause;
			return _cug_RejectCause;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CUG_RejectParam::set_cug_RejectCause(){
	if(_cug_RejectCause == NULL) _cug_RejectCause = (CUG_RejectCause*)create_node(0);
}

void asn1::CUG_RejectParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//CUG_RejectCause
asn1::CUG_RejectCause::CUG_RejectCause(){
	strcpy(node_type_name, "CUG_RejectCause");

}
asn1::CUG_RejectCause::~CUG_RejectCause(){

}

//SS_IncompatibilityCause
asn1::SS_IncompatibilityCause::SS_IncompatibilityCause(){
	strcpy(node_type_name, "SS_IncompatibilityCause");
	// ss_Code
	_ss_Code = NULL;
	children.push_back(_ss_Code);

	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);


}
asn1::SS_IncompatibilityCause::~SS_IncompatibilityCause(){

}

asn1::ASN1Node* asn1::SS_IncompatibilityCause::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ss_Code = new SS_Code();
			_ss_Code->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Code->tlv->tag_value = 1;
			children[0] = _ss_Code;
			return _ss_Code;
		case 1:
			_basicService = new BasicServiceCode();
			children[1] = _basicService;
			return _basicService;
		case 2:
			_ss_Status = new SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 4;
			children[2] = _ss_Status;
			return _ss_Status;

		default: return NULL;
	}
}

void asn1::SS_IncompatibilityCause::set_ss_Code(){
	if(_ss_Code == NULL) _ss_Code = (SS_Code*)create_node(0);
}

void asn1::SS_IncompatibilityCause::set_basicService(){
	if(_basicService == NULL) _basicService = (BasicServiceCode*)create_node(1);
}

void asn1::SS_IncompatibilityCause::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (SS_Status*)create_node(2);
}

//PW_RegistrationFailureCause
asn1::PW_RegistrationFailureCause::PW_RegistrationFailureCause(){
	strcpy(node_type_name, "PW_RegistrationFailureCause");

}
asn1::PW_RegistrationFailureCause::~PW_RegistrationFailureCause(){

}

//SM_EnumeratedDeliveryFailureCause
asn1::SM_EnumeratedDeliveryFailureCause::SM_EnumeratedDeliveryFailureCause(){
	strcpy(node_type_name, "SM_EnumeratedDeliveryFailureCause");

}
asn1::SM_EnumeratedDeliveryFailureCause::~SM_EnumeratedDeliveryFailureCause(){

}

//SM_DeliveryFailureCause
asn1::SM_DeliveryFailureCause::SM_DeliveryFailureCause(){
	strcpy(node_type_name, "SM_DeliveryFailureCause");
	// sm_EnumeratedDeliveryFailureCause
	_sm_EnumeratedDeliveryFailureCause = NULL;
	_sm_EnumeratedDeliveryFailureCause = new SM_EnumeratedDeliveryFailureCause();
	children.push_back(_sm_EnumeratedDeliveryFailureCause);

	// diagnosticInfo
	_diagnosticInfo = NULL;
	children.push_back(_diagnosticInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SM_DeliveryFailureCause::~SM_DeliveryFailureCause(){

}

asn1::ASN1Node* asn1::SM_DeliveryFailureCause::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_diagnosticInfo = new SignalInfo();
			children[1] = _diagnosticInfo;
			return _diagnosticInfo;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SM_DeliveryFailureCause::set_diagnosticInfo(){
	if(_diagnosticInfo == NULL) _diagnosticInfo = (SignalInfo*)create_node(1);
}

void asn1::SM_DeliveryFailureCause::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//AbsentSubscriberSM_Param
asn1::AbsentSubscriberSM_Param::AbsentSubscriberSM_Param(){
	strcpy(node_type_name, "AbsentSubscriberSM_Param");
	// absentSubscriberDiagnosticSM
	_absentSubscriberDiagnosticSM = NULL;
	children.push_back(_absentSubscriberDiagnosticSM);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// additionalAbsentSubscriberDiagnosticSM
	_additionalAbsentSubscriberDiagnosticSM = NULL;
	children.push_back(_additionalAbsentSubscriberDiagnosticSM);

	// imsi
	_imsi = NULL;
	children.push_back(_imsi);


}
asn1::AbsentSubscriberSM_Param::~AbsentSubscriberSM_Param(){

}

asn1::ASN1Node* asn1::AbsentSubscriberSM_Param::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_absentSubscriberDiagnosticSM = new AbsentSubscriberDiagnosticSM();
			children[0] = _absentSubscriberDiagnosticSM;
			return _absentSubscriberDiagnosticSM;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_additionalAbsentSubscriberDiagnosticSM = new AbsentSubscriberDiagnosticSM();
			_additionalAbsentSubscriberDiagnosticSM->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalAbsentSubscriberDiagnosticSM->tlv->tag_value = 0;
			children[2] = _additionalAbsentSubscriberDiagnosticSM;
			return _additionalAbsentSubscriberDiagnosticSM;
		case 3:
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 1;
			children[3] = _imsi;
			return _imsi;

		default: return NULL;
	}
}

void asn1::AbsentSubscriberSM_Param::set_absentSubscriberDiagnosticSM(){
	if(_absentSubscriberDiagnosticSM == NULL) _absentSubscriberDiagnosticSM = (AbsentSubscriberDiagnosticSM*)create_node(0);
}

void asn1::AbsentSubscriberSM_Param::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::AbsentSubscriberSM_Param::set_additionalAbsentSubscriberDiagnosticSM(){
	if(_additionalAbsentSubscriberDiagnosticSM == NULL) _additionalAbsentSubscriberDiagnosticSM = (AbsentSubscriberDiagnosticSM*)create_node(2);
}

void asn1::AbsentSubscriberSM_Param::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(3);
}

//AbsentSubscriberDiagnosticSM
asn1::AbsentSubscriberDiagnosticSM::AbsentSubscriberDiagnosticSM(){
	strcpy(node_type_name, "AbsentSubscriberDiagnosticSM");

}
asn1::AbsentSubscriberDiagnosticSM::~AbsentSubscriberDiagnosticSM(){

}

//SystemFailureParam
asn1::SystemFailureParam::SystemFailureParam(){
	strcpy(node_type_name, "SystemFailureParam");
	// networkResource
	_networkResource = NULL;
	_networkResource = new NetworkResource();
	children.push_back(_networkResource);

	// extensibleSystemFailureParam
	_extensibleSystemFailureParam = NULL;
	_extensibleSystemFailureParam = new ExtensibleSystemFailureParam();
	children.push_back(_extensibleSystemFailureParam);


}
asn1::SystemFailureParam::~SystemFailureParam(){

}

//ExtensibleSystemFailureParam
asn1::ExtensibleSystemFailureParam::ExtensibleSystemFailureParam(){
	strcpy(node_type_name, "ExtensibleSystemFailureParam");
	// networkResource
	_networkResource = NULL;
	children.push_back(_networkResource);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// additionalNetworkResource
	_additionalNetworkResource = NULL;
	children.push_back(_additionalNetworkResource);

	// failureCauseParam
	_failureCauseParam = NULL;
	children.push_back(_failureCauseParam);


}
asn1::ExtensibleSystemFailureParam::~ExtensibleSystemFailureParam(){

}

asn1::ASN1Node* asn1::ExtensibleSystemFailureParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_networkResource = new NetworkResource();
			children[0] = _networkResource;
			return _networkResource;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_additionalNetworkResource = new AdditionalNetworkResource();
			_additionalNetworkResource->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalNetworkResource->tlv->tag_value = 0;
			children[2] = _additionalNetworkResource;
			return _additionalNetworkResource;
		case 3:
			_failureCauseParam = new FailureCauseParam();
			_failureCauseParam->tlv->tag_class = CONTEXT_SPECIFIC;
			_failureCauseParam->tlv->tag_value = 1;
			children[3] = _failureCauseParam;
			return _failureCauseParam;

		default: return NULL;
	}
}

void asn1::ExtensibleSystemFailureParam::set_networkResource(){
	if(_networkResource == NULL) _networkResource = (NetworkResource*)create_node(0);
}

void asn1::ExtensibleSystemFailureParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::ExtensibleSystemFailureParam::set_additionalNetworkResource(){
	if(_additionalNetworkResource == NULL) _additionalNetworkResource = (AdditionalNetworkResource*)create_node(2);
}

void asn1::ExtensibleSystemFailureParam::set_failureCauseParam(){
	if(_failureCauseParam == NULL) _failureCauseParam = (FailureCauseParam*)create_node(3);
}

//FailureCauseParam
asn1::FailureCauseParam::FailureCauseParam(){
	strcpy(node_type_name, "FailureCauseParam");

}
asn1::FailureCauseParam::~FailureCauseParam(){

}

//DataMissingParam
asn1::DataMissingParam::DataMissingParam(){
	strcpy(node_type_name, "DataMissingParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::DataMissingParam::~DataMissingParam(){

}

asn1::ASN1Node* asn1::DataMissingParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::DataMissingParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//UnexpectedDataParam
asn1::UnexpectedDataParam::UnexpectedDataParam(){
	strcpy(node_type_name, "UnexpectedDataParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// unexpectedSubscriber
	_unexpectedSubscriber = NULL;
	children.push_back(_unexpectedSubscriber);


}
asn1::UnexpectedDataParam::~UnexpectedDataParam(){

}

asn1::ASN1Node* asn1::UnexpectedDataParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;
		case 1:
			_unexpectedSubscriber = new Null();
			_unexpectedSubscriber->tlv->tag_class = CONTEXT_SPECIFIC;
			_unexpectedSubscriber->tlv->tag_value = 0;
			children[1] = _unexpectedSubscriber;
			return _unexpectedSubscriber;

		default: return NULL;
	}
}

void asn1::UnexpectedDataParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

void asn1::UnexpectedDataParam::set_unexpectedSubscriber(){
	if(_unexpectedSubscriber == NULL) _unexpectedSubscriber = (Null*)create_node(1);
}

//FacilityNotSupParam
asn1::FacilityNotSupParam::FacilityNotSupParam(){
	strcpy(node_type_name, "FacilityNotSupParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// shapeOfLocationEstimateNotSupported
	_shapeOfLocationEstimateNotSupported = NULL;
	children.push_back(_shapeOfLocationEstimateNotSupported);

	// neededLcsCapabilityNotSupportedInServingNode
	_neededLcsCapabilityNotSupportedInServingNode = NULL;
	children.push_back(_neededLcsCapabilityNotSupportedInServingNode);


}
asn1::FacilityNotSupParam::~FacilityNotSupParam(){

}

asn1::ASN1Node* asn1::FacilityNotSupParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;
		case 1:
			_shapeOfLocationEstimateNotSupported = new Null();
			_shapeOfLocationEstimateNotSupported->tlv->tag_class = CONTEXT_SPECIFIC;
			_shapeOfLocationEstimateNotSupported->tlv->tag_value = 0;
			children[1] = _shapeOfLocationEstimateNotSupported;
			return _shapeOfLocationEstimateNotSupported;
		case 2:
			_neededLcsCapabilityNotSupportedInServingNode = new Null();
			_neededLcsCapabilityNotSupportedInServingNode->tlv->tag_class = CONTEXT_SPECIFIC;
			_neededLcsCapabilityNotSupportedInServingNode->tlv->tag_value = 1;
			children[2] = _neededLcsCapabilityNotSupportedInServingNode;
			return _neededLcsCapabilityNotSupportedInServingNode;

		default: return NULL;
	}
}

void asn1::FacilityNotSupParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

void asn1::FacilityNotSupParam::set_shapeOfLocationEstimateNotSupported(){
	if(_shapeOfLocationEstimateNotSupported == NULL) _shapeOfLocationEstimateNotSupported = (Null*)create_node(1);
}

void asn1::FacilityNotSupParam::set_neededLcsCapabilityNotSupportedInServingNode(){
	if(_neededLcsCapabilityNotSupportedInServingNode == NULL) _neededLcsCapabilityNotSupportedInServingNode = (Null*)create_node(2);
}

//OR_NotAllowedParam
asn1::OR_NotAllowedParam::OR_NotAllowedParam(){
	strcpy(node_type_name, "OR_NotAllowedParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::OR_NotAllowedParam::~OR_NotAllowedParam(){

}

asn1::ASN1Node* asn1::OR_NotAllowedParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::OR_NotAllowedParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//UnknownSubscriberParam
asn1::UnknownSubscriberParam::UnknownSubscriberParam(){
	strcpy(node_type_name, "UnknownSubscriberParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// unknownSubscriberDiagnostic
	_unknownSubscriberDiagnostic = NULL;
	children.push_back(_unknownSubscriberDiagnostic);


}
asn1::UnknownSubscriberParam::~UnknownSubscriberParam(){

}

asn1::ASN1Node* asn1::UnknownSubscriberParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;
		case 1:
			_unknownSubscriberDiagnostic = new UnknownSubscriberDiagnostic();
			children[1] = _unknownSubscriberDiagnostic;
			return _unknownSubscriberDiagnostic;

		default: return NULL;
	}
}

void asn1::UnknownSubscriberParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

void asn1::UnknownSubscriberParam::set_unknownSubscriberDiagnostic(){
	if(_unknownSubscriberDiagnostic == NULL) _unknownSubscriberDiagnostic = (UnknownSubscriberDiagnostic*)create_node(1);
}

//UnknownSubscriberDiagnostic
asn1::UnknownSubscriberDiagnostic::UnknownSubscriberDiagnostic(){
	strcpy(node_type_name, "UnknownSubscriberDiagnostic");

}
asn1::UnknownSubscriberDiagnostic::~UnknownSubscriberDiagnostic(){

}

//NumberChangedParam
asn1::NumberChangedParam::NumberChangedParam(){
	strcpy(node_type_name, "NumberChangedParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::NumberChangedParam::~NumberChangedParam(){

}

asn1::ASN1Node* asn1::NumberChangedParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::NumberChangedParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//UnidentifiedSubParam
asn1::UnidentifiedSubParam::UnidentifiedSubParam(){
	strcpy(node_type_name, "UnidentifiedSubParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::UnidentifiedSubParam::~UnidentifiedSubParam(){

}

asn1::ASN1Node* asn1::UnidentifiedSubParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::UnidentifiedSubParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//IllegalSubscriberParam
asn1::IllegalSubscriberParam::IllegalSubscriberParam(){
	strcpy(node_type_name, "IllegalSubscriberParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::IllegalSubscriberParam::~IllegalSubscriberParam(){

}

asn1::ASN1Node* asn1::IllegalSubscriberParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::IllegalSubscriberParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//IllegalEquipmentParam
asn1::IllegalEquipmentParam::IllegalEquipmentParam(){
	strcpy(node_type_name, "IllegalEquipmentParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::IllegalEquipmentParam::~IllegalEquipmentParam(){

}

asn1::ASN1Node* asn1::IllegalEquipmentParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::IllegalEquipmentParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//BearerServNotProvParam
asn1::BearerServNotProvParam::BearerServNotProvParam(){
	strcpy(node_type_name, "BearerServNotProvParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::BearerServNotProvParam::~BearerServNotProvParam(){

}

asn1::ASN1Node* asn1::BearerServNotProvParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::BearerServNotProvParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//TeleservNotProvParam
asn1::TeleservNotProvParam::TeleservNotProvParam(){
	strcpy(node_type_name, "TeleservNotProvParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::TeleservNotProvParam::~TeleservNotProvParam(){

}

asn1::ASN1Node* asn1::TeleservNotProvParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::TeleservNotProvParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//TracingBufferFullParam
asn1::TracingBufferFullParam::TracingBufferFullParam(){
	strcpy(node_type_name, "TracingBufferFullParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::TracingBufferFullParam::~TracingBufferFullParam(){

}

asn1::ASN1Node* asn1::TracingBufferFullParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::TracingBufferFullParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//NoRoamingNbParam
asn1::NoRoamingNbParam::NoRoamingNbParam(){
	strcpy(node_type_name, "NoRoamingNbParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::NoRoamingNbParam::~NoRoamingNbParam(){

}

asn1::ASN1Node* asn1::NoRoamingNbParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::NoRoamingNbParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//AbsentSubscriberParam
asn1::AbsentSubscriberParam::AbsentSubscriberParam(){
	strcpy(node_type_name, "AbsentSubscriberParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// absentSubscriberReason
	_absentSubscriberReason = NULL;
	children.push_back(_absentSubscriberReason);


}
asn1::AbsentSubscriberParam::~AbsentSubscriberParam(){

}

asn1::ASN1Node* asn1::AbsentSubscriberParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;
		case 1:
			_absentSubscriberReason = new AbsentSubscriberReason();
			_absentSubscriberReason->tlv->tag_class = CONTEXT_SPECIFIC;
			_absentSubscriberReason->tlv->tag_value = 0;
			children[1] = _absentSubscriberReason;
			return _absentSubscriberReason;

		default: return NULL;
	}
}

void asn1::AbsentSubscriberParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

void asn1::AbsentSubscriberParam::set_absentSubscriberReason(){
	if(_absentSubscriberReason == NULL) _absentSubscriberReason = (AbsentSubscriberReason*)create_node(1);
}

//AbsentSubscriberReason
asn1::AbsentSubscriberReason::AbsentSubscriberReason(){
	strcpy(node_type_name, "AbsentSubscriberReason");

}
asn1::AbsentSubscriberReason::~AbsentSubscriberReason(){

}

//BusySubscriberParam
asn1::BusySubscriberParam::BusySubscriberParam(){
	strcpy(node_type_name, "BusySubscriberParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// ccbs_Possible
	_ccbs_Possible = NULL;
	children.push_back(_ccbs_Possible);

	// ccbs_Busy
	_ccbs_Busy = NULL;
	children.push_back(_ccbs_Busy);


}
asn1::BusySubscriberParam::~BusySubscriberParam(){

}

asn1::ASN1Node* asn1::BusySubscriberParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;
		case 1:
			_ccbs_Possible = new Null();
			_ccbs_Possible->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Possible->tlv->tag_value = 0;
			children[1] = _ccbs_Possible;
			return _ccbs_Possible;
		case 2:
			_ccbs_Busy = new Null();
			_ccbs_Busy->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Busy->tlv->tag_value = 1;
			children[2] = _ccbs_Busy;
			return _ccbs_Busy;

		default: return NULL;
	}
}

void asn1::BusySubscriberParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

void asn1::BusySubscriberParam::set_ccbs_Possible(){
	if(_ccbs_Possible == NULL) _ccbs_Possible = (Null*)create_node(1);
}

void asn1::BusySubscriberParam::set_ccbs_Busy(){
	if(_ccbs_Busy == NULL) _ccbs_Busy = (Null*)create_node(2);
}

//NoSubscriberReplyParam
asn1::NoSubscriberReplyParam::NoSubscriberReplyParam(){
	strcpy(node_type_name, "NoSubscriberReplyParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::NoSubscriberReplyParam::~NoSubscriberReplyParam(){

}

asn1::ASN1Node* asn1::NoSubscriberReplyParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::NoSubscriberReplyParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//ForwardingViolationParam
asn1::ForwardingViolationParam::ForwardingViolationParam(){
	strcpy(node_type_name, "ForwardingViolationParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ForwardingViolationParam::~ForwardingViolationParam(){

}

asn1::ASN1Node* asn1::ForwardingViolationParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ForwardingViolationParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//ForwardingFailedParam
asn1::ForwardingFailedParam::ForwardingFailedParam(){
	strcpy(node_type_name, "ForwardingFailedParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ForwardingFailedParam::~ForwardingFailedParam(){

}

asn1::ASN1Node* asn1::ForwardingFailedParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ForwardingFailedParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//ATI_NotAllowedParam
asn1::ATI_NotAllowedParam::ATI_NotAllowedParam(){
	strcpy(node_type_name, "ATI_NotAllowedParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ATI_NotAllowedParam::~ATI_NotAllowedParam(){

}

asn1::ASN1Node* asn1::ATI_NotAllowedParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ATI_NotAllowedParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//ATSI_NotAllowedParam
asn1::ATSI_NotAllowedParam::ATSI_NotAllowedParam(){
	strcpy(node_type_name, "ATSI_NotAllowedParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ATSI_NotAllowedParam::~ATSI_NotAllowedParam(){

}

asn1::ASN1Node* asn1::ATSI_NotAllowedParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ATSI_NotAllowedParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//ATM_NotAllowedParam
asn1::ATM_NotAllowedParam::ATM_NotAllowedParam(){
	strcpy(node_type_name, "ATM_NotAllowedParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ATM_NotAllowedParam::~ATM_NotAllowedParam(){

}

asn1::ASN1Node* asn1::ATM_NotAllowedParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ATM_NotAllowedParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//IllegalSS_OperationParam
asn1::IllegalSS_OperationParam::IllegalSS_OperationParam(){
	strcpy(node_type_name, "IllegalSS_OperationParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::IllegalSS_OperationParam::~IllegalSS_OperationParam(){

}

asn1::ASN1Node* asn1::IllegalSS_OperationParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::IllegalSS_OperationParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//SS_NotAvailableParam
asn1::SS_NotAvailableParam::SS_NotAvailableParam(){
	strcpy(node_type_name, "SS_NotAvailableParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SS_NotAvailableParam::~SS_NotAvailableParam(){

}

asn1::ASN1Node* asn1::SS_NotAvailableParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SS_NotAvailableParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//SS_SubscriptionViolationParam
asn1::SS_SubscriptionViolationParam::SS_SubscriptionViolationParam(){
	strcpy(node_type_name, "SS_SubscriptionViolationParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SS_SubscriptionViolationParam::~SS_SubscriptionViolationParam(){

}

asn1::ASN1Node* asn1::SS_SubscriptionViolationParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SS_SubscriptionViolationParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//InformationNotAvailableParam
asn1::InformationNotAvailableParam::InformationNotAvailableParam(){
	strcpy(node_type_name, "InformationNotAvailableParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::InformationNotAvailableParam::~InformationNotAvailableParam(){

}

asn1::ASN1Node* asn1::InformationNotAvailableParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::InformationNotAvailableParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//SubBusyForMT_SMS_Param
asn1::SubBusyForMT_SMS_Param::SubBusyForMT_SMS_Param(){
	strcpy(node_type_name, "SubBusyForMT_SMS_Param");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// gprsConnectionSuspended
	_gprsConnectionSuspended = NULL;
	children.push_back(_gprsConnectionSuspended);


}
asn1::SubBusyForMT_SMS_Param::~SubBusyForMT_SMS_Param(){

}

asn1::ASN1Node* asn1::SubBusyForMT_SMS_Param::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;
		case 1:
			_gprsConnectionSuspended = new Null();
			children[1] = _gprsConnectionSuspended;
			return _gprsConnectionSuspended;

		default: return NULL;
	}
}

void asn1::SubBusyForMT_SMS_Param::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

void asn1::SubBusyForMT_SMS_Param::set_gprsConnectionSuspended(){
	if(_gprsConnectionSuspended == NULL) _gprsConnectionSuspended = (Null*)create_node(1);
}

//MessageWaitListFullParam
asn1::MessageWaitListFullParam::MessageWaitListFullParam(){
	strcpy(node_type_name, "MessageWaitListFullParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MessageWaitListFullParam::~MessageWaitListFullParam(){

}

asn1::ASN1Node* asn1::MessageWaitListFullParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::MessageWaitListFullParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//ResourceLimitationParam
asn1::ResourceLimitationParam::ResourceLimitationParam(){
	strcpy(node_type_name, "ResourceLimitationParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ResourceLimitationParam::~ResourceLimitationParam(){

}

asn1::ASN1Node* asn1::ResourceLimitationParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ResourceLimitationParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//NoGroupCallNbParam
asn1::NoGroupCallNbParam::NoGroupCallNbParam(){
	strcpy(node_type_name, "NoGroupCallNbParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::NoGroupCallNbParam::~NoGroupCallNbParam(){

}

asn1::ASN1Node* asn1::NoGroupCallNbParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::NoGroupCallNbParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//IncompatibleTerminalParam
asn1::IncompatibleTerminalParam::IncompatibleTerminalParam(){
	strcpy(node_type_name, "IncompatibleTerminalParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::IncompatibleTerminalParam::~IncompatibleTerminalParam(){

}

asn1::ASN1Node* asn1::IncompatibleTerminalParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::IncompatibleTerminalParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//ShortTermDenialParam
asn1::ShortTermDenialParam::ShortTermDenialParam(){
	strcpy(node_type_name, "ShortTermDenialParam");


}
asn1::ShortTermDenialParam::~ShortTermDenialParam(){

}

//LongTermDenialParam
asn1::LongTermDenialParam::LongTermDenialParam(){
	strcpy(node_type_name, "LongTermDenialParam");


}
asn1::LongTermDenialParam::~LongTermDenialParam(){

}

//UnauthorizedRequestingNetwork_Param
asn1::UnauthorizedRequestingNetwork_Param::UnauthorizedRequestingNetwork_Param(){
	strcpy(node_type_name, "UnauthorizedRequestingNetwork_Param");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::UnauthorizedRequestingNetwork_Param::~UnauthorizedRequestingNetwork_Param(){

}

asn1::ASN1Node* asn1::UnauthorizedRequestingNetwork_Param::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::UnauthorizedRequestingNetwork_Param::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//UnauthorizedLCSClient_Param
asn1::UnauthorizedLCSClient_Param::UnauthorizedLCSClient_Param(){
	strcpy(node_type_name, "UnauthorizedLCSClient_Param");
	// unauthorizedLCSClient_Diagnostic
	_unauthorizedLCSClient_Diagnostic = NULL;
	children.push_back(_unauthorizedLCSClient_Diagnostic);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::UnauthorizedLCSClient_Param::~UnauthorizedLCSClient_Param(){

}

asn1::ASN1Node* asn1::UnauthorizedLCSClient_Param::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_unauthorizedLCSClient_Diagnostic = new UnauthorizedLCSClient_Diagnostic();
			_unauthorizedLCSClient_Diagnostic->tlv->tag_class = CONTEXT_SPECIFIC;
			_unauthorizedLCSClient_Diagnostic->tlv->tag_value = 0;
			children[0] = _unauthorizedLCSClient_Diagnostic;
			return _unauthorizedLCSClient_Diagnostic;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::UnauthorizedLCSClient_Param::set_unauthorizedLCSClient_Diagnostic(){
	if(_unauthorizedLCSClient_Diagnostic == NULL) _unauthorizedLCSClient_Diagnostic = (UnauthorizedLCSClient_Diagnostic*)create_node(0);
}

void asn1::UnauthorizedLCSClient_Param::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//UnauthorizedLCSClient_Diagnostic
asn1::UnauthorizedLCSClient_Diagnostic::UnauthorizedLCSClient_Diagnostic(){
	strcpy(node_type_name, "UnauthorizedLCSClient_Diagnostic");

}
asn1::UnauthorizedLCSClient_Diagnostic::~UnauthorizedLCSClient_Diagnostic(){

}

//PositionMethodFailure_Param
asn1::PositionMethodFailure_Param::PositionMethodFailure_Param(){
	strcpy(node_type_name, "PositionMethodFailure_Param");
	// positionMethodFailure_Diagnostic
	_positionMethodFailure_Diagnostic = NULL;
	children.push_back(_positionMethodFailure_Diagnostic);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::PositionMethodFailure_Param::~PositionMethodFailure_Param(){

}

asn1::ASN1Node* asn1::PositionMethodFailure_Param::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_positionMethodFailure_Diagnostic = new PositionMethodFailure_Diagnostic();
			_positionMethodFailure_Diagnostic->tlv->tag_class = CONTEXT_SPECIFIC;
			_positionMethodFailure_Diagnostic->tlv->tag_value = 0;
			children[0] = _positionMethodFailure_Diagnostic;
			return _positionMethodFailure_Diagnostic;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::PositionMethodFailure_Param::set_positionMethodFailure_Diagnostic(){
	if(_positionMethodFailure_Diagnostic == NULL) _positionMethodFailure_Diagnostic = (PositionMethodFailure_Diagnostic*)create_node(0);
}

void asn1::PositionMethodFailure_Param::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//PositionMethodFailure_Diagnostic
asn1::PositionMethodFailure_Diagnostic::PositionMethodFailure_Diagnostic(){
	strcpy(node_type_name, "PositionMethodFailure_Diagnostic");

}
asn1::PositionMethodFailure_Diagnostic::~PositionMethodFailure_Diagnostic(){

}

//UnknownOrUnreachableLCSClient_Param
asn1::UnknownOrUnreachableLCSClient_Param::UnknownOrUnreachableLCSClient_Param(){
	strcpy(node_type_name, "UnknownOrUnreachableLCSClient_Param");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::UnknownOrUnreachableLCSClient_Param::~UnknownOrUnreachableLCSClient_Param(){

}

asn1::ASN1Node* asn1::UnknownOrUnreachableLCSClient_Param::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::UnknownOrUnreachableLCSClient_Param::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//MM_EventNotSupported_Param
asn1::MM_EventNotSupported_Param::MM_EventNotSupported_Param(){
	strcpy(node_type_name, "MM_EventNotSupported_Param");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MM_EventNotSupported_Param::~MM_EventNotSupported_Param(){

}

asn1::ASN1Node* asn1::MM_EventNotSupported_Param::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::MM_EventNotSupported_Param::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//TargetCellOutsideGCA_Param
asn1::TargetCellOutsideGCA_Param::TargetCellOutsideGCA_Param(){
	strcpy(node_type_name, "TargetCellOutsideGCA_Param");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::TargetCellOutsideGCA_Param::~TargetCellOutsideGCA_Param(){

}

asn1::ASN1Node* asn1::TargetCellOutsideGCA_Param::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::TargetCellOutsideGCA_Param::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//OngoingGroupCallParam
asn1::OngoingGroupCallParam::OngoingGroupCallParam(){
	strcpy(node_type_name, "OngoingGroupCallParam");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::OngoingGroupCallParam::~OngoingGroupCallParam(){

}

asn1::ASN1Node* asn1::OngoingGroupCallParam::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::OngoingGroupCallParam::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

