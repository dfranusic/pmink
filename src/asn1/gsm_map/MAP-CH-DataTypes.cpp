#include<MAP-CH-DataTypes.h>
#include<iostream>

//CUG_CheckInfo
asn1::CUG_CheckInfo::CUG_CheckInfo(){
	strcpy(node_type_name, "CUG_CheckInfo");
	// cug_Interlock
	_cug_Interlock = NULL;
	_cug_Interlock = new CUG_Interlock();
	children.push_back(_cug_Interlock);

	// cug_OutgoingAccess
	_cug_OutgoingAccess = NULL;
	children.push_back(_cug_OutgoingAccess);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CUG_CheckInfo::~CUG_CheckInfo(){

}

asn1::ASN1Node* asn1::CUG_CheckInfo::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_cug_OutgoingAccess = new Null();
			children[1] = _cug_OutgoingAccess;
			return _cug_OutgoingAccess;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CUG_CheckInfo::set_cug_OutgoingAccess(){
	if(_cug_OutgoingAccess == NULL) _cug_OutgoingAccess = (Null*)create_node(1);
}

void asn1::CUG_CheckInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//NumberOfForwarding
asn1::NumberOfForwarding::NumberOfForwarding(){
	strcpy(node_type_name, "NumberOfForwarding");

}
asn1::NumberOfForwarding::~NumberOfForwarding(){

}

//SendRoutingInfoArg
asn1::SendRoutingInfoArg::SendRoutingInfoArg(){
	strcpy(node_type_name, "SendRoutingInfoArg");
	// msisdn
	_msisdn = NULL;
	_msisdn = new ISDN_AddressString();
	_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
	_msisdn->tlv->tag_value = 0;
	children.push_back(_msisdn);

	// cug_CheckInfo
	_cug_CheckInfo = NULL;
	children.push_back(_cug_CheckInfo);

	// numberOfForwarding
	_numberOfForwarding = NULL;
	children.push_back(_numberOfForwarding);

	// interrogationType
	_interrogationType = NULL;
	_interrogationType = new InterrogationType();
	_interrogationType->tlv->tag_class = CONTEXT_SPECIFIC;
	_interrogationType->tlv->tag_value = 3;
	children.push_back(_interrogationType);

	// or_Interrogation
	_or_Interrogation = NULL;
	children.push_back(_or_Interrogation);

	// or_Capability
	_or_Capability = NULL;
	children.push_back(_or_Capability);

	// gmsc_OrGsmSCF_Address
	_gmsc_OrGsmSCF_Address = NULL;
	_gmsc_OrGsmSCF_Address = new ISDN_AddressString();
	_gmsc_OrGsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gmsc_OrGsmSCF_Address->tlv->tag_value = 6;
	children.push_back(_gmsc_OrGsmSCF_Address);

	// callReferenceNumber
	_callReferenceNumber = NULL;
	children.push_back(_callReferenceNumber);

	// forwardingReason
	_forwardingReason = NULL;
	children.push_back(_forwardingReason);

	// basicServiceGroup
	_basicServiceGroup = NULL;
	children.push_back(_basicServiceGroup);

	// networkSignalInfo
	_networkSignalInfo = NULL;
	children.push_back(_networkSignalInfo);

	// camelInfo
	_camelInfo = NULL;
	children.push_back(_camelInfo);

	// suppressionOfAnnouncement
	_suppressionOfAnnouncement = NULL;
	children.push_back(_suppressionOfAnnouncement);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// alertingPattern
	_alertingPattern = NULL;
	children.push_back(_alertingPattern);

	// ccbs_Call
	_ccbs_Call = NULL;
	children.push_back(_ccbs_Call);

	// supportedCCBS_Phase
	_supportedCCBS_Phase = NULL;
	children.push_back(_supportedCCBS_Phase);

	// additionalSignalInfo
	_additionalSignalInfo = NULL;
	children.push_back(_additionalSignalInfo);

	// istSupportIndicator
	_istSupportIndicator = NULL;
	children.push_back(_istSupportIndicator);

	// pre_pagingSupported
	_pre_pagingSupported = NULL;
	children.push_back(_pre_pagingSupported);

	// callDiversionTreatmentIndicator
	_callDiversionTreatmentIndicator = NULL;
	children.push_back(_callDiversionTreatmentIndicator);

	// longFTN_Supported
	_longFTN_Supported = NULL;
	children.push_back(_longFTN_Supported);

	// suppress_VT_CSI
	_suppress_VT_CSI = NULL;
	children.push_back(_suppress_VT_CSI);

	// suppressIncomingCallBarring
	_suppressIncomingCallBarring = NULL;
	children.push_back(_suppressIncomingCallBarring);

	// gsmSCF_InitiatedCall
	_gsmSCF_InitiatedCall = NULL;
	children.push_back(_gsmSCF_InitiatedCall);

	// basicServiceGroup2
	_basicServiceGroup2 = NULL;
	children.push_back(_basicServiceGroup2);

	// networkSignalInfo2
	_networkSignalInfo2 = NULL;
	children.push_back(_networkSignalInfo2);

	// suppressMTSS
	_suppressMTSS = NULL;
	children.push_back(_suppressMTSS);

	// mtRoamingRetrySupported
	_mtRoamingRetrySupported = NULL;
	children.push_back(_mtRoamingRetrySupported);

	// callPriority
	_callPriority = NULL;
	children.push_back(_callPriority);


}
asn1::SendRoutingInfoArg::~SendRoutingInfoArg(){

}

asn1::ASN1Node* asn1::SendRoutingInfoArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_cug_CheckInfo = new CUG_CheckInfo();
			_cug_CheckInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_cug_CheckInfo->tlv->tag_value = 1;
			children[1] = _cug_CheckInfo;
			return _cug_CheckInfo;
		case 2:
			_numberOfForwarding = new NumberOfForwarding();
			_numberOfForwarding->tlv->tag_class = CONTEXT_SPECIFIC;
			_numberOfForwarding->tlv->tag_value = 2;
			children[2] = _numberOfForwarding;
			return _numberOfForwarding;
		case 4:
			_or_Interrogation = new Null();
			_or_Interrogation->tlv->tag_class = CONTEXT_SPECIFIC;
			_or_Interrogation->tlv->tag_value = 4;
			children[4] = _or_Interrogation;
			return _or_Interrogation;
		case 5:
			_or_Capability = new OR_Phase();
			_or_Capability->tlv->tag_class = CONTEXT_SPECIFIC;
			_or_Capability->tlv->tag_value = 5;
			children[5] = _or_Capability;
			return _or_Capability;
		case 7:
			_callReferenceNumber = new CallReferenceNumber();
			_callReferenceNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_callReferenceNumber->tlv->tag_value = 7;
			children[7] = _callReferenceNumber;
			return _callReferenceNumber;
		case 8:
			_forwardingReason = new ForwardingReason();
			_forwardingReason->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardingReason->tlv->tag_value = 8;
			children[8] = _forwardingReason;
			return _forwardingReason;
		case 9:
			_basicServiceGroup = new Ext_BasicServiceCode();
			_basicServiceGroup->tlv->tag_class = CONTEXT_SPECIFIC;
			_basicServiceGroup->tlv->tag_value = 9;
			children[9] = _basicServiceGroup;
			return _basicServiceGroup;
		case 10:
			_networkSignalInfo = new ExternalSignalInfo();
			_networkSignalInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_networkSignalInfo->tlv->tag_value = 10;
			children[10] = _networkSignalInfo;
			return _networkSignalInfo;
		case 11:
			_camelInfo = new CamelInfo();
			_camelInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_camelInfo->tlv->tag_value = 11;
			children[11] = _camelInfo;
			return _camelInfo;
		case 12:
			_suppressionOfAnnouncement = new SuppressionOfAnnouncement();
			_suppressionOfAnnouncement->tlv->tag_class = CONTEXT_SPECIFIC;
			_suppressionOfAnnouncement->tlv->tag_value = 12;
			children[12] = _suppressionOfAnnouncement;
			return _suppressionOfAnnouncement;
		case 13:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 13;
			children[13] = _extensionContainer;
			return _extensionContainer;
		case 14:
			_alertingPattern = new AlertingPattern();
			_alertingPattern->tlv->tag_class = CONTEXT_SPECIFIC;
			_alertingPattern->tlv->tag_value = 14;
			children[14] = _alertingPattern;
			return _alertingPattern;
		case 15:
			_ccbs_Call = new Null();
			_ccbs_Call->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Call->tlv->tag_value = 15;
			children[15] = _ccbs_Call;
			return _ccbs_Call;
		case 16:
			_supportedCCBS_Phase = new SupportedCCBS_Phase();
			_supportedCCBS_Phase->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedCCBS_Phase->tlv->tag_value = 16;
			children[16] = _supportedCCBS_Phase;
			return _supportedCCBS_Phase;
		case 17:
			_additionalSignalInfo = new Ext_ExternalSignalInfo();
			_additionalSignalInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalSignalInfo->tlv->tag_value = 17;
			children[17] = _additionalSignalInfo;
			return _additionalSignalInfo;
		case 18:
			_istSupportIndicator = new IST_SupportIndicator();
			_istSupportIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_istSupportIndicator->tlv->tag_value = 18;
			children[18] = _istSupportIndicator;
			return _istSupportIndicator;
		case 19:
			_pre_pagingSupported = new Null();
			_pre_pagingSupported->tlv->tag_class = CONTEXT_SPECIFIC;
			_pre_pagingSupported->tlv->tag_value = 19;
			children[19] = _pre_pagingSupported;
			return _pre_pagingSupported;
		case 20:
			_callDiversionTreatmentIndicator = new CallDiversionTreatmentIndicator();
			_callDiversionTreatmentIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_callDiversionTreatmentIndicator->tlv->tag_value = 20;
			children[20] = _callDiversionTreatmentIndicator;
			return _callDiversionTreatmentIndicator;
		case 21:
			_longFTN_Supported = new Null();
			_longFTN_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_longFTN_Supported->tlv->tag_value = 21;
			children[21] = _longFTN_Supported;
			return _longFTN_Supported;
		case 22:
			_suppress_VT_CSI = new Null();
			_suppress_VT_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_suppress_VT_CSI->tlv->tag_value = 22;
			children[22] = _suppress_VT_CSI;
			return _suppress_VT_CSI;
		case 23:
			_suppressIncomingCallBarring = new Null();
			_suppressIncomingCallBarring->tlv->tag_class = CONTEXT_SPECIFIC;
			_suppressIncomingCallBarring->tlv->tag_value = 23;
			children[23] = _suppressIncomingCallBarring;
			return _suppressIncomingCallBarring;
		case 24:
			_gsmSCF_InitiatedCall = new Null();
			_gsmSCF_InitiatedCall->tlv->tag_class = CONTEXT_SPECIFIC;
			_gsmSCF_InitiatedCall->tlv->tag_value = 24;
			children[24] = _gsmSCF_InitiatedCall;
			return _gsmSCF_InitiatedCall;
                case 25:
                {
                        _basicServiceGroup2 = new Ext_BasicServiceCode();
                        ASN1Node* _basicServiceGroup2_wrapper = new ASN1Node();
                        _basicServiceGroup2_wrapper->tlv = new TLVNode();
                        _basicServiceGroup2_wrapper->tlv->is_explicit = true;
                        _basicServiceGroup2_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _basicServiceGroup2_wrapper->tlv->tag_value = 25;
                        _basicServiceGroup2_wrapper->children.push_back(_basicServiceGroup2);
                        children[25] = _basicServiceGroup2_wrapper;
                        return _basicServiceGroup2;
                }
		case 26:
			_networkSignalInfo2 = new ExternalSignalInfo();
			_networkSignalInfo2->tlv->tag_class = CONTEXT_SPECIFIC;
			_networkSignalInfo2->tlv->tag_value = 26;
			children[26] = _networkSignalInfo2;
			return _networkSignalInfo2;
		case 27:
			_suppressMTSS = new SuppressMTSS();
			_suppressMTSS->tlv->tag_class = CONTEXT_SPECIFIC;
			_suppressMTSS->tlv->tag_value = 27;
			children[27] = _suppressMTSS;
			return _suppressMTSS;
		case 28:
			_mtRoamingRetrySupported = new Null();
			_mtRoamingRetrySupported->tlv->tag_class = CONTEXT_SPECIFIC;
			_mtRoamingRetrySupported->tlv->tag_value = 28;
			children[28] = _mtRoamingRetrySupported;
			return _mtRoamingRetrySupported;
		case 29:
			_callPriority = new EMLPP_Priority();
			_callPriority->tlv->tag_class = CONTEXT_SPECIFIC;
			_callPriority->tlv->tag_value = 29;
			children[29] = _callPriority;
			return _callPriority;

		default: return NULL;
	}
}

void asn1::SendRoutingInfoArg::set_cug_CheckInfo(){
	if(_cug_CheckInfo == NULL) _cug_CheckInfo = (CUG_CheckInfo*)create_node(1);
}

void asn1::SendRoutingInfoArg::set_numberOfForwarding(){
	if(_numberOfForwarding == NULL) _numberOfForwarding = (NumberOfForwarding*)create_node(2);
}

void asn1::SendRoutingInfoArg::set_or_Interrogation(){
	if(_or_Interrogation == NULL) _or_Interrogation = (Null*)create_node(4);
}

void asn1::SendRoutingInfoArg::set_or_Capability(){
	if(_or_Capability == NULL) _or_Capability = (OR_Phase*)create_node(5);
}

void asn1::SendRoutingInfoArg::set_callReferenceNumber(){
	if(_callReferenceNumber == NULL) _callReferenceNumber = (CallReferenceNumber*)create_node(7);
}

void asn1::SendRoutingInfoArg::set_forwardingReason(){
	if(_forwardingReason == NULL) _forwardingReason = (ForwardingReason*)create_node(8);
}

void asn1::SendRoutingInfoArg::set_basicServiceGroup(){
	if(_basicServiceGroup == NULL) _basicServiceGroup = (Ext_BasicServiceCode*)create_node(9);
}

void asn1::SendRoutingInfoArg::set_networkSignalInfo(){
	if(_networkSignalInfo == NULL) _networkSignalInfo = (ExternalSignalInfo*)create_node(10);
}

void asn1::SendRoutingInfoArg::set_camelInfo(){
	if(_camelInfo == NULL) _camelInfo = (CamelInfo*)create_node(11);
}

void asn1::SendRoutingInfoArg::set_suppressionOfAnnouncement(){
	if(_suppressionOfAnnouncement == NULL) _suppressionOfAnnouncement = (SuppressionOfAnnouncement*)create_node(12);
}

void asn1::SendRoutingInfoArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(13);
}

void asn1::SendRoutingInfoArg::set_alertingPattern(){
	if(_alertingPattern == NULL) _alertingPattern = (AlertingPattern*)create_node(14);
}

void asn1::SendRoutingInfoArg::set_ccbs_Call(){
	if(_ccbs_Call == NULL) _ccbs_Call = (Null*)create_node(15);
}

void asn1::SendRoutingInfoArg::set_supportedCCBS_Phase(){
	if(_supportedCCBS_Phase == NULL) _supportedCCBS_Phase = (SupportedCCBS_Phase*)create_node(16);
}

void asn1::SendRoutingInfoArg::set_additionalSignalInfo(){
	if(_additionalSignalInfo == NULL) _additionalSignalInfo = (Ext_ExternalSignalInfo*)create_node(17);
}

void asn1::SendRoutingInfoArg::set_istSupportIndicator(){
	if(_istSupportIndicator == NULL) _istSupportIndicator = (IST_SupportIndicator*)create_node(18);
}

void asn1::SendRoutingInfoArg::set_pre_pagingSupported(){
	if(_pre_pagingSupported == NULL) _pre_pagingSupported = (Null*)create_node(19);
}

void asn1::SendRoutingInfoArg::set_callDiversionTreatmentIndicator(){
	if(_callDiversionTreatmentIndicator == NULL) _callDiversionTreatmentIndicator = (CallDiversionTreatmentIndicator*)create_node(20);
}

void asn1::SendRoutingInfoArg::set_longFTN_Supported(){
	if(_longFTN_Supported == NULL) _longFTN_Supported = (Null*)create_node(21);
}

void asn1::SendRoutingInfoArg::set_suppress_VT_CSI(){
	if(_suppress_VT_CSI == NULL) _suppress_VT_CSI = (Null*)create_node(22);
}

void asn1::SendRoutingInfoArg::set_suppressIncomingCallBarring(){
	if(_suppressIncomingCallBarring == NULL) _suppressIncomingCallBarring = (Null*)create_node(23);
}

void asn1::SendRoutingInfoArg::set_gsmSCF_InitiatedCall(){
	if(_gsmSCF_InitiatedCall == NULL) _gsmSCF_InitiatedCall = (Null*)create_node(24);
}

void asn1::SendRoutingInfoArg::set_basicServiceGroup2(){
	if(_basicServiceGroup2 == NULL) _basicServiceGroup2 = (Ext_BasicServiceCode*)create_node(25);
}

void asn1::SendRoutingInfoArg::set_networkSignalInfo2(){
	if(_networkSignalInfo2 == NULL) _networkSignalInfo2 = (ExternalSignalInfo*)create_node(26);
}

void asn1::SendRoutingInfoArg::set_suppressMTSS(){
	if(_suppressMTSS == NULL) _suppressMTSS = (SuppressMTSS*)create_node(27);
}

void asn1::SendRoutingInfoArg::set_mtRoamingRetrySupported(){
	if(_mtRoamingRetrySupported == NULL) _mtRoamingRetrySupported = (Null*)create_node(28);
}

void asn1::SendRoutingInfoArg::set_callPriority(){
	if(_callPriority == NULL) _callPriority = (EMLPP_Priority*)create_node(29);
}

//SuppressionOfAnnouncement
asn1::SuppressionOfAnnouncement::SuppressionOfAnnouncement(){
	strcpy(node_type_name, "SuppressionOfAnnouncement");

}
asn1::SuppressionOfAnnouncement::~SuppressionOfAnnouncement(){

}

//SuppressMTSS
asn1::SuppressMTSS::SuppressMTSS(){
	strcpy(node_type_name, "SuppressMTSS");

}
asn1::SuppressMTSS::~SuppressMTSS(){

}

//InterrogationType
asn1::InterrogationType::InterrogationType(){
	strcpy(node_type_name, "InterrogationType");

}
asn1::InterrogationType::~InterrogationType(){

}

//OR_Phase
asn1::OR_Phase::OR_Phase(){
	strcpy(node_type_name, "OR_Phase");

}
asn1::OR_Phase::~OR_Phase(){

}

//CallReferenceNumber
asn1::CallReferenceNumber::CallReferenceNumber(){
	strcpy(node_type_name, "CallReferenceNumber");

}
asn1::CallReferenceNumber::~CallReferenceNumber(){

}

//ForwardingReason
asn1::ForwardingReason::ForwardingReason(){
	strcpy(node_type_name, "ForwardingReason");

}
asn1::ForwardingReason::~ForwardingReason(){

}

//SupportedCCBS_Phase
asn1::SupportedCCBS_Phase::SupportedCCBS_Phase(){
	strcpy(node_type_name, "SupportedCCBS_Phase");

}
asn1::SupportedCCBS_Phase::~SupportedCCBS_Phase(){

}

//CallDiversionTreatmentIndicator
asn1::CallDiversionTreatmentIndicator::CallDiversionTreatmentIndicator(){
	strcpy(node_type_name, "CallDiversionTreatmentIndicator");

}
asn1::CallDiversionTreatmentIndicator::~CallDiversionTreatmentIndicator(){

}

//SendRoutingInfoRes
asn1::SendRoutingInfoRes::SendRoutingInfoRes(){
	strcpy(node_type_name, "SendRoutingInfoRes");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// extendedRoutingInfo
	_extendedRoutingInfo = NULL;
	children.push_back(_extendedRoutingInfo);

	// cug_CheckInfo
	_cug_CheckInfo = NULL;
	children.push_back(_cug_CheckInfo);

	// cugSubscriptionFlag
	_cugSubscriptionFlag = NULL;
	children.push_back(_cugSubscriptionFlag);

	// subscriberInfo
	_subscriberInfo = NULL;
	children.push_back(_subscriberInfo);

	// ss_List
	_ss_List = NULL;
	children.push_back(_ss_List);

	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// forwardingInterrogationRequired
	_forwardingInterrogationRequired = NULL;
	children.push_back(_forwardingInterrogationRequired);

	// vmsc_Address
	_vmsc_Address = NULL;
	children.push_back(_vmsc_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// naea_PreferredCI
	_naea_PreferredCI = NULL;
	children.push_back(_naea_PreferredCI);

	// ccbs_Indicators
	_ccbs_Indicators = NULL;
	children.push_back(_ccbs_Indicators);

	// msisdn
	_msisdn = NULL;
	children.push_back(_msisdn);

	// numberPortabilityStatus
	_numberPortabilityStatus = NULL;
	children.push_back(_numberPortabilityStatus);

	// istAlertTimer
	_istAlertTimer = NULL;
	children.push_back(_istAlertTimer);

	// supportedCamelPhasesInVMSC
	_supportedCamelPhasesInVMSC = NULL;
	children.push_back(_supportedCamelPhasesInVMSC);

	// offeredCamel4CSIsInVMSC
	_offeredCamel4CSIsInVMSC = NULL;
	children.push_back(_offeredCamel4CSIsInVMSC);

	// routingInfo2
	_routingInfo2 = NULL;
	children.push_back(_routingInfo2);

	// ss_List2
	_ss_List2 = NULL;
	children.push_back(_ss_List2);

	// basicService2
	_basicService2 = NULL;
	children.push_back(_basicService2);

	// allowedServices
	_allowedServices = NULL;
	children.push_back(_allowedServices);

	// unavailabilityCause
	_unavailabilityCause = NULL;
	children.push_back(_unavailabilityCause);

	// releaseResourcesSupported
	_releaseResourcesSupported = NULL;
	children.push_back(_releaseResourcesSupported);

	// gsm_BearerCapability
	_gsm_BearerCapability = NULL;
	children.push_back(_gsm_BearerCapability);


}
asn1::SendRoutingInfoRes::~SendRoutingInfoRes(){

}

asn1::ASN1Node* asn1::SendRoutingInfoRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 9;
			children[0] = _imsi;
			return _imsi;
		case 1:
			_extendedRoutingInfo = new ExtendedRoutingInfo();
			children[1] = _extendedRoutingInfo;
			return _extendedRoutingInfo;
		case 2:
			_cug_CheckInfo = new CUG_CheckInfo();
			_cug_CheckInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_cug_CheckInfo->tlv->tag_value = 3;
			children[2] = _cug_CheckInfo;
			return _cug_CheckInfo;
		case 3:
			_cugSubscriptionFlag = new Null();
			_cugSubscriptionFlag->tlv->tag_class = CONTEXT_SPECIFIC;
			_cugSubscriptionFlag->tlv->tag_value = 6;
			children[3] = _cugSubscriptionFlag;
			return _cugSubscriptionFlag;
		case 4:
			_subscriberInfo = new SubscriberInfo();
			_subscriberInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_subscriberInfo->tlv->tag_value = 7;
			children[4] = _subscriberInfo;
			return _subscriberInfo;
		case 5:
			_ss_List = new SS_List();
			_ss_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_List->tlv->tag_value = 1;
			children[5] = _ss_List;
			return _ss_List;
                case 6:
                {
                        _basicService = new Ext_BasicServiceCode();
                        ASN1Node* _basicService_wrapper = new ASN1Node();
                        _basicService_wrapper->tlv = new TLVNode();
                        _basicService_wrapper->tlv->is_explicit = true;
                        _basicService_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _basicService_wrapper->tlv->tag_value = 5;
                        _basicService_wrapper->children.push_back(_basicService);
                        children[6] = _basicService_wrapper;
                        return _basicService;
                }
		case 7:
			_forwardingInterrogationRequired = new Null();
			_forwardingInterrogationRequired->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardingInterrogationRequired->tlv->tag_value = 4;
			children[7] = _forwardingInterrogationRequired;
			return _forwardingInterrogationRequired;
		case 8:
			_vmsc_Address = new ISDN_AddressString();
			_vmsc_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_vmsc_Address->tlv->tag_value = 2;
			children[8] = _vmsc_Address;
			return _vmsc_Address;
		case 9:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[9] = _extensionContainer;
			return _extensionContainer;
		case 10:
			_naea_PreferredCI = new NAEA_PreferredCI();
			_naea_PreferredCI->tlv->tag_class = CONTEXT_SPECIFIC;
			_naea_PreferredCI->tlv->tag_value = 10;
			children[10] = _naea_PreferredCI;
			return _naea_PreferredCI;
		case 11:
			_ccbs_Indicators = new CCBS_Indicators();
			_ccbs_Indicators->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Indicators->tlv->tag_value = 11;
			children[11] = _ccbs_Indicators;
			return _ccbs_Indicators;
		case 12:
			_msisdn = new ISDN_AddressString();
			_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn->tlv->tag_value = 12;
			children[12] = _msisdn;
			return _msisdn;
		case 13:
			_numberPortabilityStatus = new NumberPortabilityStatus();
			_numberPortabilityStatus->tlv->tag_class = CONTEXT_SPECIFIC;
			_numberPortabilityStatus->tlv->tag_value = 13;
			children[13] = _numberPortabilityStatus;
			return _numberPortabilityStatus;
		case 14:
			_istAlertTimer = new IST_AlertTimerValue();
			_istAlertTimer->tlv->tag_class = CONTEXT_SPECIFIC;
			_istAlertTimer->tlv->tag_value = 14;
			children[14] = _istAlertTimer;
			return _istAlertTimer;
		case 15:
			_supportedCamelPhasesInVMSC = new SupportedCamelPhases();
			_supportedCamelPhasesInVMSC->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedCamelPhasesInVMSC->tlv->tag_value = 15;
			children[15] = _supportedCamelPhasesInVMSC;
			return _supportedCamelPhasesInVMSC;
		case 16:
			_offeredCamel4CSIsInVMSC = new OfferedCamel4CSIs();
			_offeredCamel4CSIsInVMSC->tlv->tag_class = CONTEXT_SPECIFIC;
			_offeredCamel4CSIsInVMSC->tlv->tag_value = 16;
			children[16] = _offeredCamel4CSIsInVMSC;
			return _offeredCamel4CSIsInVMSC;
                case 17:
                {
                        _routingInfo2 = new RoutingInfo();
                        ASN1Node* _routingInfo2_wrapper = new ASN1Node();
                        _routingInfo2_wrapper->tlv = new TLVNode();
                        _routingInfo2_wrapper->tlv->is_explicit = true;
                        _routingInfo2_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _routingInfo2_wrapper->tlv->tag_value = 17;
                        _routingInfo2_wrapper->children.push_back(_routingInfo2);
                        children[17] = _routingInfo2_wrapper;
                        return _routingInfo2;
                }
		case 18:
			_ss_List2 = new SS_List();
			_ss_List2->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_List2->tlv->tag_value = 18;
			children[18] = _ss_List2;
			return _ss_List2;
                case 19:
                {
                        _basicService2 = new Ext_BasicServiceCode();
                        ASN1Node* _basicService2_wrapper = new ASN1Node();
                        _basicService2_wrapper->tlv = new TLVNode();
                        _basicService2_wrapper->tlv->is_explicit = true;
                        _basicService2_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _basicService2_wrapper->tlv->tag_value = 19;
                        _basicService2_wrapper->children.push_back(_basicService2);
                        children[19] = _basicService2_wrapper;
                        return _basicService2;
                }
		case 20:
			_allowedServices = new AllowedServices();
			_allowedServices->tlv->tag_class = CONTEXT_SPECIFIC;
			_allowedServices->tlv->tag_value = 20;
			children[20] = _allowedServices;
			return _allowedServices;
		case 21:
			_unavailabilityCause = new UnavailabilityCause();
			_unavailabilityCause->tlv->tag_class = CONTEXT_SPECIFIC;
			_unavailabilityCause->tlv->tag_value = 21;
			children[21] = _unavailabilityCause;
			return _unavailabilityCause;
		case 22:
			_releaseResourcesSupported = new Null();
			_releaseResourcesSupported->tlv->tag_class = CONTEXT_SPECIFIC;
			_releaseResourcesSupported->tlv->tag_value = 22;
			children[22] = _releaseResourcesSupported;
			return _releaseResourcesSupported;
		case 23:
			_gsm_BearerCapability = new ExternalSignalInfo();
			_gsm_BearerCapability->tlv->tag_class = CONTEXT_SPECIFIC;
			_gsm_BearerCapability->tlv->tag_value = 23;
			children[23] = _gsm_BearerCapability;
			return _gsm_BearerCapability;

		default: return NULL;
	}
}

void asn1::SendRoutingInfoRes::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(0);
}

void asn1::SendRoutingInfoRes::set_extendedRoutingInfo(){
	if(_extendedRoutingInfo == NULL) _extendedRoutingInfo = (ExtendedRoutingInfo*)create_node(1);
}

void asn1::SendRoutingInfoRes::set_cug_CheckInfo(){
	if(_cug_CheckInfo == NULL) _cug_CheckInfo = (CUG_CheckInfo*)create_node(2);
}

void asn1::SendRoutingInfoRes::set_cugSubscriptionFlag(){
	if(_cugSubscriptionFlag == NULL) _cugSubscriptionFlag = (Null*)create_node(3);
}

void asn1::SendRoutingInfoRes::set_subscriberInfo(){
	if(_subscriberInfo == NULL) _subscriberInfo = (SubscriberInfo*)create_node(4);
}

void asn1::SendRoutingInfoRes::set_ss_List(){
	if(_ss_List == NULL) _ss_List = (SS_List*)create_node(5);
}

void asn1::SendRoutingInfoRes::set_basicService(){
	if(_basicService == NULL) _basicService = (Ext_BasicServiceCode*)create_node(6);
}

void asn1::SendRoutingInfoRes::set_forwardingInterrogationRequired(){
	if(_forwardingInterrogationRequired == NULL) _forwardingInterrogationRequired = (Null*)create_node(7);
}

void asn1::SendRoutingInfoRes::set_vmsc_Address(){
	if(_vmsc_Address == NULL) _vmsc_Address = (ISDN_AddressString*)create_node(8);
}

void asn1::SendRoutingInfoRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(9);
}

void asn1::SendRoutingInfoRes::set_naea_PreferredCI(){
	if(_naea_PreferredCI == NULL) _naea_PreferredCI = (NAEA_PreferredCI*)create_node(10);
}

void asn1::SendRoutingInfoRes::set_ccbs_Indicators(){
	if(_ccbs_Indicators == NULL) _ccbs_Indicators = (CCBS_Indicators*)create_node(11);
}

void asn1::SendRoutingInfoRes::set_msisdn(){
	if(_msisdn == NULL) _msisdn = (ISDN_AddressString*)create_node(12);
}

void asn1::SendRoutingInfoRes::set_numberPortabilityStatus(){
	if(_numberPortabilityStatus == NULL) _numberPortabilityStatus = (NumberPortabilityStatus*)create_node(13);
}

void asn1::SendRoutingInfoRes::set_istAlertTimer(){
	if(_istAlertTimer == NULL) _istAlertTimer = (IST_AlertTimerValue*)create_node(14);
}

void asn1::SendRoutingInfoRes::set_supportedCamelPhasesInVMSC(){
	if(_supportedCamelPhasesInVMSC == NULL) _supportedCamelPhasesInVMSC = (SupportedCamelPhases*)create_node(15);
}

void asn1::SendRoutingInfoRes::set_offeredCamel4CSIsInVMSC(){
	if(_offeredCamel4CSIsInVMSC == NULL) _offeredCamel4CSIsInVMSC = (OfferedCamel4CSIs*)create_node(16);
}

void asn1::SendRoutingInfoRes::set_routingInfo2(){
	if(_routingInfo2 == NULL) _routingInfo2 = (RoutingInfo*)create_node(17);
}

void asn1::SendRoutingInfoRes::set_ss_List2(){
	if(_ss_List2 == NULL) _ss_List2 = (SS_List*)create_node(18);
}

void asn1::SendRoutingInfoRes::set_basicService2(){
	if(_basicService2 == NULL) _basicService2 = (Ext_BasicServiceCode*)create_node(19);
}

void asn1::SendRoutingInfoRes::set_allowedServices(){
	if(_allowedServices == NULL) _allowedServices = (AllowedServices*)create_node(20);
}

void asn1::SendRoutingInfoRes::set_unavailabilityCause(){
	if(_unavailabilityCause == NULL) _unavailabilityCause = (UnavailabilityCause*)create_node(21);
}

void asn1::SendRoutingInfoRes::set_releaseResourcesSupported(){
	if(_releaseResourcesSupported == NULL) _releaseResourcesSupported = (Null*)create_node(22);
}

void asn1::SendRoutingInfoRes::set_gsm_BearerCapability(){
	if(_gsm_BearerCapability == NULL) _gsm_BearerCapability = (ExternalSignalInfo*)create_node(23);
}

//AllowedServices
asn1::AllowedServices::AllowedServices(){
	strcpy(node_type_name, "AllowedServices");

}
asn1::AllowedServices::~AllowedServices(){

}

//UnavailabilityCause
asn1::UnavailabilityCause::UnavailabilityCause(){
	strcpy(node_type_name, "UnavailabilityCause");

}
asn1::UnavailabilityCause::~UnavailabilityCause(){

}

//CCBS_Indicators
asn1::CCBS_Indicators::CCBS_Indicators(){
	strcpy(node_type_name, "CCBS_Indicators");
	// ccbs_Possible
	_ccbs_Possible = NULL;
	children.push_back(_ccbs_Possible);

	// keepCCBS_CallIndicator
	_keepCCBS_CallIndicator = NULL;
	children.push_back(_keepCCBS_CallIndicator);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CCBS_Indicators::~CCBS_Indicators(){

}

asn1::ASN1Node* asn1::CCBS_Indicators::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ccbs_Possible = new Null();
			_ccbs_Possible->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Possible->tlv->tag_value = 0;
			children[0] = _ccbs_Possible;
			return _ccbs_Possible;
		case 1:
			_keepCCBS_CallIndicator = new Null();
			_keepCCBS_CallIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_keepCCBS_CallIndicator->tlv->tag_value = 1;
			children[1] = _keepCCBS_CallIndicator;
			return _keepCCBS_CallIndicator;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CCBS_Indicators::set_ccbs_Possible(){
	if(_ccbs_Possible == NULL) _ccbs_Possible = (Null*)create_node(0);
}

void asn1::CCBS_Indicators::set_keepCCBS_CallIndicator(){
	if(_keepCCBS_CallIndicator == NULL) _keepCCBS_CallIndicator = (Null*)create_node(1);
}

void asn1::CCBS_Indicators::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//RoutingInfo
asn1::RoutingInfo::RoutingInfo(){
	strcpy(node_type_name, "RoutingInfo");
	// roamingNumber
	_roamingNumber = NULL;
	_roamingNumber = new ISDN_AddressString();
	children.push_back(_roamingNumber);

	// forwardingData
	_forwardingData = NULL;
	_forwardingData = new ForwardingData();
	children.push_back(_forwardingData);


}
asn1::RoutingInfo::~RoutingInfo(){

}

//ForwardingData
asn1::ForwardingData::ForwardingData(){
	strcpy(node_type_name, "ForwardingData");
	// forwardedToNumber
	_forwardedToNumber = NULL;
	children.push_back(_forwardedToNumber);

	// forwardedToSubaddress
	_forwardedToSubaddress = NULL;
	children.push_back(_forwardedToSubaddress);

	// forwardingOptions
	_forwardingOptions = NULL;
	children.push_back(_forwardingOptions);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// longForwardedToNumber
	_longForwardedToNumber = NULL;
	children.push_back(_longForwardedToNumber);


}
asn1::ForwardingData::~ForwardingData(){

}

asn1::ASN1Node* asn1::ForwardingData::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_forwardedToNumber = new ISDN_AddressString();
			_forwardedToNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardedToNumber->tlv->tag_value = 5;
			children[0] = _forwardedToNumber;
			return _forwardedToNumber;
		case 1:
			_forwardedToSubaddress = new ISDN_SubaddressString();
			_forwardedToSubaddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardedToSubaddress->tlv->tag_value = 4;
			children[1] = _forwardedToSubaddress;
			return _forwardedToSubaddress;
		case 2:
			_forwardingOptions = new ForwardingOptions();
			_forwardingOptions->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardingOptions->tlv->tag_value = 6;
			children[2] = _forwardingOptions;
			return _forwardingOptions;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 7;
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_longForwardedToNumber = new FTN_AddressString();
			_longForwardedToNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_longForwardedToNumber->tlv->tag_value = 8;
			children[4] = _longForwardedToNumber;
			return _longForwardedToNumber;

		default: return NULL;
	}
}

void asn1::ForwardingData::set_forwardedToNumber(){
	if(_forwardedToNumber == NULL) _forwardedToNumber = (ISDN_AddressString*)create_node(0);
}

void asn1::ForwardingData::set_forwardedToSubaddress(){
	if(_forwardedToSubaddress == NULL) _forwardedToSubaddress = (ISDN_SubaddressString*)create_node(1);
}

void asn1::ForwardingData::set_forwardingOptions(){
	if(_forwardingOptions == NULL) _forwardingOptions = (ForwardingOptions*)create_node(2);
}

void asn1::ForwardingData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::ForwardingData::set_longForwardedToNumber(){
	if(_longForwardedToNumber == NULL) _longForwardedToNumber = (FTN_AddressString*)create_node(4);
}

//ProvideRoamingNumberArg
asn1::ProvideRoamingNumberArg::ProvideRoamingNumberArg(){
	strcpy(node_type_name, "ProvideRoamingNumberArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// msc_Number
	_msc_Number = NULL;
	_msc_Number = new ISDN_AddressString();
	_msc_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_msc_Number->tlv->tag_value = 1;
	children.push_back(_msc_Number);

	// msisdn
	_msisdn = NULL;
	children.push_back(_msisdn);

	// lmsi
	_lmsi = NULL;
	children.push_back(_lmsi);

	// gsm_BearerCapability
	_gsm_BearerCapability = NULL;
	children.push_back(_gsm_BearerCapability);

	// networkSignalInfo
	_networkSignalInfo = NULL;
	children.push_back(_networkSignalInfo);

	// suppressionOfAnnouncement
	_suppressionOfAnnouncement = NULL;
	children.push_back(_suppressionOfAnnouncement);

	// gmsc_Address
	_gmsc_Address = NULL;
	children.push_back(_gmsc_Address);

	// callReferenceNumber
	_callReferenceNumber = NULL;
	children.push_back(_callReferenceNumber);

	// or_Interrogation
	_or_Interrogation = NULL;
	children.push_back(_or_Interrogation);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// alertingPattern
	_alertingPattern = NULL;
	children.push_back(_alertingPattern);

	// ccbs_Call
	_ccbs_Call = NULL;
	children.push_back(_ccbs_Call);

	// supportedCamelPhasesInInterrogatingNode
	_supportedCamelPhasesInInterrogatingNode = NULL;
	children.push_back(_supportedCamelPhasesInInterrogatingNode);

	// additionalSignalInfo
	_additionalSignalInfo = NULL;
	children.push_back(_additionalSignalInfo);

	// orNotSupportedInGMSC
	_orNotSupportedInGMSC = NULL;
	children.push_back(_orNotSupportedInGMSC);

	// pre_pagingSupported
	_pre_pagingSupported = NULL;
	children.push_back(_pre_pagingSupported);

	// longFTN_Supported
	_longFTN_Supported = NULL;
	children.push_back(_longFTN_Supported);

	// suppress_VT_CSI
	_suppress_VT_CSI = NULL;
	children.push_back(_suppress_VT_CSI);

	// offeredCamel4CSIsInInterrogatingNode
	_offeredCamel4CSIsInInterrogatingNode = NULL;
	children.push_back(_offeredCamel4CSIsInInterrogatingNode);

	// mtRoamingRetrySupported
	_mtRoamingRetrySupported = NULL;
	children.push_back(_mtRoamingRetrySupported);

	// pagingArea
	_pagingArea = NULL;
	children.push_back(_pagingArea);

	// callPriority
	_callPriority = NULL;
	children.push_back(_callPriority);

	// mtrf_Indicator
	_mtrf_Indicator = NULL;
	children.push_back(_mtrf_Indicator);

	// oldMSC_Number
	_oldMSC_Number = NULL;
	children.push_back(_oldMSC_Number);


}
asn1::ProvideRoamingNumberArg::~ProvideRoamingNumberArg(){

}

asn1::ASN1Node* asn1::ProvideRoamingNumberArg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_msisdn = new ISDN_AddressString();
			_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn->tlv->tag_value = 2;
			children[2] = _msisdn;
			return _msisdn;
		case 3:
			_lmsi = new LMSI();
			_lmsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_lmsi->tlv->tag_value = 4;
			children[3] = _lmsi;
			return _lmsi;
		case 4:
			_gsm_BearerCapability = new ExternalSignalInfo();
			_gsm_BearerCapability->tlv->tag_class = CONTEXT_SPECIFIC;
			_gsm_BearerCapability->tlv->tag_value = 5;
			children[4] = _gsm_BearerCapability;
			return _gsm_BearerCapability;
		case 5:
			_networkSignalInfo = new ExternalSignalInfo();
			_networkSignalInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_networkSignalInfo->tlv->tag_value = 6;
			children[5] = _networkSignalInfo;
			return _networkSignalInfo;
		case 6:
			_suppressionOfAnnouncement = new SuppressionOfAnnouncement();
			_suppressionOfAnnouncement->tlv->tag_class = CONTEXT_SPECIFIC;
			_suppressionOfAnnouncement->tlv->tag_value = 7;
			children[6] = _suppressionOfAnnouncement;
			return _suppressionOfAnnouncement;
		case 7:
			_gmsc_Address = new ISDN_AddressString();
			_gmsc_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_gmsc_Address->tlv->tag_value = 8;
			children[7] = _gmsc_Address;
			return _gmsc_Address;
		case 8:
			_callReferenceNumber = new CallReferenceNumber();
			_callReferenceNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_callReferenceNumber->tlv->tag_value = 9;
			children[8] = _callReferenceNumber;
			return _callReferenceNumber;
		case 9:
			_or_Interrogation = new Null();
			_or_Interrogation->tlv->tag_class = CONTEXT_SPECIFIC;
			_or_Interrogation->tlv->tag_value = 10;
			children[9] = _or_Interrogation;
			return _or_Interrogation;
		case 10:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 11;
			children[10] = _extensionContainer;
			return _extensionContainer;
		case 11:
			_alertingPattern = new AlertingPattern();
			_alertingPattern->tlv->tag_class = CONTEXT_SPECIFIC;
			_alertingPattern->tlv->tag_value = 12;
			children[11] = _alertingPattern;
			return _alertingPattern;
		case 12:
			_ccbs_Call = new Null();
			_ccbs_Call->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Call->tlv->tag_value = 13;
			children[12] = _ccbs_Call;
			return _ccbs_Call;
		case 13:
			_supportedCamelPhasesInInterrogatingNode = new SupportedCamelPhases();
			_supportedCamelPhasesInInterrogatingNode->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedCamelPhasesInInterrogatingNode->tlv->tag_value = 15;
			children[13] = _supportedCamelPhasesInInterrogatingNode;
			return _supportedCamelPhasesInInterrogatingNode;
		case 14:
			_additionalSignalInfo = new Ext_ExternalSignalInfo();
			_additionalSignalInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalSignalInfo->tlv->tag_value = 14;
			children[14] = _additionalSignalInfo;
			return _additionalSignalInfo;
		case 15:
			_orNotSupportedInGMSC = new Null();
			_orNotSupportedInGMSC->tlv->tag_class = CONTEXT_SPECIFIC;
			_orNotSupportedInGMSC->tlv->tag_value = 16;
			children[15] = _orNotSupportedInGMSC;
			return _orNotSupportedInGMSC;
		case 16:
			_pre_pagingSupported = new Null();
			_pre_pagingSupported->tlv->tag_class = CONTEXT_SPECIFIC;
			_pre_pagingSupported->tlv->tag_value = 17;
			children[16] = _pre_pagingSupported;
			return _pre_pagingSupported;
		case 17:
			_longFTN_Supported = new Null();
			_longFTN_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_longFTN_Supported->tlv->tag_value = 18;
			children[17] = _longFTN_Supported;
			return _longFTN_Supported;
		case 18:
			_suppress_VT_CSI = new Null();
			_suppress_VT_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_suppress_VT_CSI->tlv->tag_value = 19;
			children[18] = _suppress_VT_CSI;
			return _suppress_VT_CSI;
		case 19:
			_offeredCamel4CSIsInInterrogatingNode = new OfferedCamel4CSIs();
			_offeredCamel4CSIsInInterrogatingNode->tlv->tag_class = CONTEXT_SPECIFIC;
			_offeredCamel4CSIsInInterrogatingNode->tlv->tag_value = 20;
			children[19] = _offeredCamel4CSIsInInterrogatingNode;
			return _offeredCamel4CSIsInInterrogatingNode;
		case 20:
			_mtRoamingRetrySupported = new Null();
			_mtRoamingRetrySupported->tlv->tag_class = CONTEXT_SPECIFIC;
			_mtRoamingRetrySupported->tlv->tag_value = 21;
			children[20] = _mtRoamingRetrySupported;
			return _mtRoamingRetrySupported;
		case 21:
			_pagingArea = new PagingArea();
			_pagingArea->tlv->tag_class = CONTEXT_SPECIFIC;
			_pagingArea->tlv->tag_value = 22;
			children[21] = _pagingArea;
			return _pagingArea;
		case 22:
			_callPriority = new EMLPP_Priority();
			_callPriority->tlv->tag_class = CONTEXT_SPECIFIC;
			_callPriority->tlv->tag_value = 23;
			children[22] = _callPriority;
			return _callPriority;
		case 23:
			_mtrf_Indicator = new Null();
			_mtrf_Indicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_mtrf_Indicator->tlv->tag_value = 24;
			children[23] = _mtrf_Indicator;
			return _mtrf_Indicator;
		case 24:
			_oldMSC_Number = new ISDN_AddressString();
			_oldMSC_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_oldMSC_Number->tlv->tag_value = 25;
			children[24] = _oldMSC_Number;
			return _oldMSC_Number;

		default: return NULL;
	}
}

void asn1::ProvideRoamingNumberArg::set_msisdn(){
	if(_msisdn == NULL) _msisdn = (ISDN_AddressString*)create_node(2);
}

void asn1::ProvideRoamingNumberArg::set_lmsi(){
	if(_lmsi == NULL) _lmsi = (LMSI*)create_node(3);
}

void asn1::ProvideRoamingNumberArg::set_gsm_BearerCapability(){
	if(_gsm_BearerCapability == NULL) _gsm_BearerCapability = (ExternalSignalInfo*)create_node(4);
}

void asn1::ProvideRoamingNumberArg::set_networkSignalInfo(){
	if(_networkSignalInfo == NULL) _networkSignalInfo = (ExternalSignalInfo*)create_node(5);
}

void asn1::ProvideRoamingNumberArg::set_suppressionOfAnnouncement(){
	if(_suppressionOfAnnouncement == NULL) _suppressionOfAnnouncement = (SuppressionOfAnnouncement*)create_node(6);
}

void asn1::ProvideRoamingNumberArg::set_gmsc_Address(){
	if(_gmsc_Address == NULL) _gmsc_Address = (ISDN_AddressString*)create_node(7);
}

void asn1::ProvideRoamingNumberArg::set_callReferenceNumber(){
	if(_callReferenceNumber == NULL) _callReferenceNumber = (CallReferenceNumber*)create_node(8);
}

void asn1::ProvideRoamingNumberArg::set_or_Interrogation(){
	if(_or_Interrogation == NULL) _or_Interrogation = (Null*)create_node(9);
}

void asn1::ProvideRoamingNumberArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(10);
}

void asn1::ProvideRoamingNumberArg::set_alertingPattern(){
	if(_alertingPattern == NULL) _alertingPattern = (AlertingPattern*)create_node(11);
}

void asn1::ProvideRoamingNumberArg::set_ccbs_Call(){
	if(_ccbs_Call == NULL) _ccbs_Call = (Null*)create_node(12);
}

void asn1::ProvideRoamingNumberArg::set_supportedCamelPhasesInInterrogatingNode(){
	if(_supportedCamelPhasesInInterrogatingNode == NULL) _supportedCamelPhasesInInterrogatingNode = (SupportedCamelPhases*)create_node(13);
}

void asn1::ProvideRoamingNumberArg::set_additionalSignalInfo(){
	if(_additionalSignalInfo == NULL) _additionalSignalInfo = (Ext_ExternalSignalInfo*)create_node(14);
}

void asn1::ProvideRoamingNumberArg::set_orNotSupportedInGMSC(){
	if(_orNotSupportedInGMSC == NULL) _orNotSupportedInGMSC = (Null*)create_node(15);
}

void asn1::ProvideRoamingNumberArg::set_pre_pagingSupported(){
	if(_pre_pagingSupported == NULL) _pre_pagingSupported = (Null*)create_node(16);
}

void asn1::ProvideRoamingNumberArg::set_longFTN_Supported(){
	if(_longFTN_Supported == NULL) _longFTN_Supported = (Null*)create_node(17);
}

void asn1::ProvideRoamingNumberArg::set_suppress_VT_CSI(){
	if(_suppress_VT_CSI == NULL) _suppress_VT_CSI = (Null*)create_node(18);
}

void asn1::ProvideRoamingNumberArg::set_offeredCamel4CSIsInInterrogatingNode(){
	if(_offeredCamel4CSIsInInterrogatingNode == NULL) _offeredCamel4CSIsInInterrogatingNode = (OfferedCamel4CSIs*)create_node(19);
}

void asn1::ProvideRoamingNumberArg::set_mtRoamingRetrySupported(){
	if(_mtRoamingRetrySupported == NULL) _mtRoamingRetrySupported = (Null*)create_node(20);
}

void asn1::ProvideRoamingNumberArg::set_pagingArea(){
	if(_pagingArea == NULL) _pagingArea = (PagingArea*)create_node(21);
}

void asn1::ProvideRoamingNumberArg::set_callPriority(){
	if(_callPriority == NULL) _callPriority = (EMLPP_Priority*)create_node(22);
}

void asn1::ProvideRoamingNumberArg::set_mtrf_Indicator(){
	if(_mtrf_Indicator == NULL) _mtrf_Indicator = (Null*)create_node(23);
}

void asn1::ProvideRoamingNumberArg::set_oldMSC_Number(){
	if(_oldMSC_Number == NULL) _oldMSC_Number = (ISDN_AddressString*)create_node(24);
}

//ProvideRoamingNumberRes
asn1::ProvideRoamingNumberRes::ProvideRoamingNumberRes(){
	strcpy(node_type_name, "ProvideRoamingNumberRes");
	// roamingNumber
	_roamingNumber = NULL;
	_roamingNumber = new ISDN_AddressString();
	children.push_back(_roamingNumber);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// releaseResourcesSupported
	_releaseResourcesSupported = NULL;
	children.push_back(_releaseResourcesSupported);

	// vmsc_Address
	_vmsc_Address = NULL;
	children.push_back(_vmsc_Address);


}
asn1::ProvideRoamingNumberRes::~ProvideRoamingNumberRes(){

}

asn1::ASN1Node* asn1::ProvideRoamingNumberRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_releaseResourcesSupported = new Null();
			children[2] = _releaseResourcesSupported;
			return _releaseResourcesSupported;
		case 3:
			_vmsc_Address = new ISDN_AddressString();
			children[3] = _vmsc_Address;
			return _vmsc_Address;

		default: return NULL;
	}
}

void asn1::ProvideRoamingNumberRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::ProvideRoamingNumberRes::set_releaseResourcesSupported(){
	if(_releaseResourcesSupported == NULL) _releaseResourcesSupported = (Null*)create_node(2);
}

void asn1::ProvideRoamingNumberRes::set_vmsc_Address(){
	if(_vmsc_Address == NULL) _vmsc_Address = (ISDN_AddressString*)create_node(3);
}

//ResumeCallHandlingArg
asn1::ResumeCallHandlingArg::ResumeCallHandlingArg(){
	strcpy(node_type_name, "ResumeCallHandlingArg");
	// callReferenceNumber
	_callReferenceNumber = NULL;
	children.push_back(_callReferenceNumber);

	// basicServiceGroup
	_basicServiceGroup = NULL;
	children.push_back(_basicServiceGroup);

	// forwardingData
	_forwardingData = NULL;
	children.push_back(_forwardingData);

	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// cug_CheckInfo
	_cug_CheckInfo = NULL;
	children.push_back(_cug_CheckInfo);

	// o_CSI
	_o_CSI = NULL;
	children.push_back(_o_CSI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// ccbs_Possible
	_ccbs_Possible = NULL;
	children.push_back(_ccbs_Possible);

	// msisdn
	_msisdn = NULL;
	children.push_back(_msisdn);

	// uu_Data
	_uu_Data = NULL;
	children.push_back(_uu_Data);

	// allInformationSent
	_allInformationSent = NULL;
	children.push_back(_allInformationSent);

	// d_csi
	_d_csi = NULL;
	children.push_back(_d_csi);

	// o_BcsmCamelTDPCriteriaList
	_o_BcsmCamelTDPCriteriaList = NULL;
	children.push_back(_o_BcsmCamelTDPCriteriaList);

	// basicServiceGroup2
	_basicServiceGroup2 = NULL;
	children.push_back(_basicServiceGroup2);

	// mtRoamingRetry
	_mtRoamingRetry = NULL;
	children.push_back(_mtRoamingRetry);


}
asn1::ResumeCallHandlingArg::~ResumeCallHandlingArg(){

}

asn1::ASN1Node* asn1::ResumeCallHandlingArg::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_callReferenceNumber = new CallReferenceNumber();
			_callReferenceNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_callReferenceNumber->tlv->tag_value = 0;
			children[0] = _callReferenceNumber;
			return _callReferenceNumber;
                case 1:
                {
                        _basicServiceGroup = new Ext_BasicServiceCode();
                        ASN1Node* _basicServiceGroup_wrapper = new ASN1Node();
                        _basicServiceGroup_wrapper->tlv = new TLVNode();
                        _basicServiceGroup_wrapper->tlv->is_explicit = true;
                        _basicServiceGroup_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _basicServiceGroup_wrapper->tlv->tag_value = 1;
                        _basicServiceGroup_wrapper->children.push_back(_basicServiceGroup);
                        children[1] = _basicServiceGroup_wrapper;
                        return _basicServiceGroup;
                }
		case 2:
			_forwardingData = new ForwardingData();
			_forwardingData->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardingData->tlv->tag_value = 2;
			children[2] = _forwardingData;
			return _forwardingData;
		case 3:
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 3;
			children[3] = _imsi;
			return _imsi;
		case 4:
			_cug_CheckInfo = new CUG_CheckInfo();
			_cug_CheckInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_cug_CheckInfo->tlv->tag_value = 4;
			children[4] = _cug_CheckInfo;
			return _cug_CheckInfo;
		case 5:
			_o_CSI = new O_CSI();
			_o_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_CSI->tlv->tag_value = 5;
			children[5] = _o_CSI;
			return _o_CSI;
		case 6:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 7;
			children[6] = _extensionContainer;
			return _extensionContainer;
		case 7:
			_ccbs_Possible = new Null();
			_ccbs_Possible->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Possible->tlv->tag_value = 8;
			children[7] = _ccbs_Possible;
			return _ccbs_Possible;
		case 8:
			_msisdn = new ISDN_AddressString();
			_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn->tlv->tag_value = 9;
			children[8] = _msisdn;
			return _msisdn;
		case 9:
			_uu_Data = new UU_Data();
			_uu_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_uu_Data->tlv->tag_value = 10;
			children[9] = _uu_Data;
			return _uu_Data;
		case 10:
			_allInformationSent = new Null();
			_allInformationSent->tlv->tag_class = CONTEXT_SPECIFIC;
			_allInformationSent->tlv->tag_value = 11;
			children[10] = _allInformationSent;
			return _allInformationSent;
		case 11:
			_d_csi = new D_CSI();
			_d_csi->tlv->tag_class = CONTEXT_SPECIFIC;
			_d_csi->tlv->tag_value = 12;
			children[11] = _d_csi;
			return _d_csi;
		case 12:
			_o_BcsmCamelTDPCriteriaList = new O_BcsmCamelTDPCriteriaList();
			_o_BcsmCamelTDPCriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_BcsmCamelTDPCriteriaList->tlv->tag_value = 13;
			children[12] = _o_BcsmCamelTDPCriteriaList;
			return _o_BcsmCamelTDPCriteriaList;
                case 13:
                {
                        _basicServiceGroup2 = new Ext_BasicServiceCode();
                        ASN1Node* _basicServiceGroup2_wrapper = new ASN1Node();
                        _basicServiceGroup2_wrapper->tlv = new TLVNode();
                        _basicServiceGroup2_wrapper->tlv->is_explicit = true;
                        _basicServiceGroup2_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _basicServiceGroup2_wrapper->tlv->tag_value = 14;
                        _basicServiceGroup2_wrapper->children.push_back(_basicServiceGroup2);
                        children[13] = _basicServiceGroup2_wrapper;
                        return _basicServiceGroup2;
                }
		case 14:
			_mtRoamingRetry = new Null();
			_mtRoamingRetry->tlv->tag_class = CONTEXT_SPECIFIC;
			_mtRoamingRetry->tlv->tag_value = 15;
			children[14] = _mtRoamingRetry;
			return _mtRoamingRetry;

		default: return NULL;
	}
}

void asn1::ResumeCallHandlingArg::set_callReferenceNumber(){
	if(_callReferenceNumber == NULL) _callReferenceNumber = (CallReferenceNumber*)create_node(0);
}

void asn1::ResumeCallHandlingArg::set_basicServiceGroup(){
	if(_basicServiceGroup == NULL) _basicServiceGroup = (Ext_BasicServiceCode*)create_node(1);
}

void asn1::ResumeCallHandlingArg::set_forwardingData(){
	if(_forwardingData == NULL) _forwardingData = (ForwardingData*)create_node(2);
}

void asn1::ResumeCallHandlingArg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(3);
}

void asn1::ResumeCallHandlingArg::set_cug_CheckInfo(){
	if(_cug_CheckInfo == NULL) _cug_CheckInfo = (CUG_CheckInfo*)create_node(4);
}

void asn1::ResumeCallHandlingArg::set_o_CSI(){
	if(_o_CSI == NULL) _o_CSI = (O_CSI*)create_node(5);
}

void asn1::ResumeCallHandlingArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(6);
}

void asn1::ResumeCallHandlingArg::set_ccbs_Possible(){
	if(_ccbs_Possible == NULL) _ccbs_Possible = (Null*)create_node(7);
}

void asn1::ResumeCallHandlingArg::set_msisdn(){
	if(_msisdn == NULL) _msisdn = (ISDN_AddressString*)create_node(8);
}

void asn1::ResumeCallHandlingArg::set_uu_Data(){
	if(_uu_Data == NULL) _uu_Data = (UU_Data*)create_node(9);
}

void asn1::ResumeCallHandlingArg::set_allInformationSent(){
	if(_allInformationSent == NULL) _allInformationSent = (Null*)create_node(10);
}

void asn1::ResumeCallHandlingArg::set_d_csi(){
	if(_d_csi == NULL) _d_csi = (D_CSI*)create_node(11);
}

void asn1::ResumeCallHandlingArg::set_o_BcsmCamelTDPCriteriaList(){
	if(_o_BcsmCamelTDPCriteriaList == NULL) _o_BcsmCamelTDPCriteriaList = (O_BcsmCamelTDPCriteriaList*)create_node(12);
}

void asn1::ResumeCallHandlingArg::set_basicServiceGroup2(){
	if(_basicServiceGroup2 == NULL) _basicServiceGroup2 = (Ext_BasicServiceCode*)create_node(13);
}

void asn1::ResumeCallHandlingArg::set_mtRoamingRetry(){
	if(_mtRoamingRetry == NULL) _mtRoamingRetry = (Null*)create_node(14);
}

//UU_Data
asn1::UU_Data::UU_Data(){
	strcpy(node_type_name, "UU_Data");
	// uuIndicator
	_uuIndicator = NULL;
	children.push_back(_uuIndicator);

	// uui
	_uui = NULL;
	children.push_back(_uui);

	// uusCFInteraction
	_uusCFInteraction = NULL;
	children.push_back(_uusCFInteraction);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::UU_Data::~UU_Data(){

}

asn1::ASN1Node* asn1::UU_Data::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_uuIndicator = new UUIndicator();
			_uuIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_uuIndicator->tlv->tag_value = 0;
			children[0] = _uuIndicator;
			return _uuIndicator;
		case 1:
			_uui = new UUI();
			_uui->tlv->tag_class = CONTEXT_SPECIFIC;
			_uui->tlv->tag_value = 1;
			children[1] = _uui;
			return _uui;
		case 2:
			_uusCFInteraction = new Null();
			_uusCFInteraction->tlv->tag_class = CONTEXT_SPECIFIC;
			_uusCFInteraction->tlv->tag_value = 2;
			children[2] = _uusCFInteraction;
			return _uusCFInteraction;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::UU_Data::set_uuIndicator(){
	if(_uuIndicator == NULL) _uuIndicator = (UUIndicator*)create_node(0);
}

void asn1::UU_Data::set_uui(){
	if(_uui == NULL) _uui = (UUI*)create_node(1);
}

void asn1::UU_Data::set_uusCFInteraction(){
	if(_uusCFInteraction == NULL) _uusCFInteraction = (Null*)create_node(2);
}

void asn1::UU_Data::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//UUIndicator
asn1::UUIndicator::UUIndicator(){
	strcpy(node_type_name, "UUIndicator");

}
asn1::UUIndicator::~UUIndicator(){

}

//UUI
asn1::UUI::UUI(){
	strcpy(node_type_name, "UUI");

}
asn1::UUI::~UUI(){

}

//ResumeCallHandlingRes
asn1::ResumeCallHandlingRes::ResumeCallHandlingRes(){
	strcpy(node_type_name, "ResumeCallHandlingRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ResumeCallHandlingRes::~ResumeCallHandlingRes(){

}

asn1::ASN1Node* asn1::ResumeCallHandlingRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ResumeCallHandlingRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//CamelInfo
asn1::CamelInfo::CamelInfo(){
	strcpy(node_type_name, "CamelInfo");
	// supportedCamelPhases
	_supportedCamelPhases = NULL;
	_supportedCamelPhases = new SupportedCamelPhases();
	children.push_back(_supportedCamelPhases);

	// suppress_T_CSI
	_suppress_T_CSI = NULL;
	children.push_back(_suppress_T_CSI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// offeredCamel4CSIs
	_offeredCamel4CSIs = NULL;
	children.push_back(_offeredCamel4CSIs);


}
asn1::CamelInfo::~CamelInfo(){

}

asn1::ASN1Node* asn1::CamelInfo::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_suppress_T_CSI = new Null();
			children[1] = _suppress_T_CSI;
			return _suppress_T_CSI;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_offeredCamel4CSIs = new OfferedCamel4CSIs();
			_offeredCamel4CSIs->tlv->tag_class = CONTEXT_SPECIFIC;
			_offeredCamel4CSIs->tlv->tag_value = 0;
			children[3] = _offeredCamel4CSIs;
			return _offeredCamel4CSIs;

		default: return NULL;
	}
}

void asn1::CamelInfo::set_suppress_T_CSI(){
	if(_suppress_T_CSI == NULL) _suppress_T_CSI = (Null*)create_node(1);
}

void asn1::CamelInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::CamelInfo::set_offeredCamel4CSIs(){
	if(_offeredCamel4CSIs == NULL) _offeredCamel4CSIs = (OfferedCamel4CSIs*)create_node(3);
}

//ExtendedRoutingInfo
asn1::ExtendedRoutingInfo::ExtendedRoutingInfo(){
	strcpy(node_type_name, "ExtendedRoutingInfo");
	// routingInfo
	_routingInfo = NULL;
	_routingInfo = new RoutingInfo();
	children.push_back(_routingInfo);

	// camelRoutingInfo
	_camelRoutingInfo = NULL;
	_camelRoutingInfo = new CamelRoutingInfo();
	_camelRoutingInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_camelRoutingInfo->tlv->tag_value = 8;
	children.push_back(_camelRoutingInfo);


}
asn1::ExtendedRoutingInfo::~ExtendedRoutingInfo(){

}

//CamelRoutingInfo
asn1::CamelRoutingInfo::CamelRoutingInfo(){
	strcpy(node_type_name, "CamelRoutingInfo");
	// forwardingData
	_forwardingData = NULL;
	children.push_back(_forwardingData);

	// gmscCamelSubscriptionInfo
	_gmscCamelSubscriptionInfo = NULL;
	_gmscCamelSubscriptionInfo = new GmscCamelSubscriptionInfo();
	_gmscCamelSubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_gmscCamelSubscriptionInfo->tlv->tag_value = 0;
	children.push_back(_gmscCamelSubscriptionInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CamelRoutingInfo::~CamelRoutingInfo(){

}

asn1::ASN1Node* asn1::CamelRoutingInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_forwardingData = new ForwardingData();
			children[0] = _forwardingData;
			return _forwardingData;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CamelRoutingInfo::set_forwardingData(){
	if(_forwardingData == NULL) _forwardingData = (ForwardingData*)create_node(0);
}

void asn1::CamelRoutingInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//GmscCamelSubscriptionInfo
asn1::GmscCamelSubscriptionInfo::GmscCamelSubscriptionInfo(){
	strcpy(node_type_name, "GmscCamelSubscriptionInfo");
	// t_CSI
	_t_CSI = NULL;
	children.push_back(_t_CSI);

	// o_CSI
	_o_CSI = NULL;
	children.push_back(_o_CSI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// o_BcsmCamelTDP_CriteriaList
	_o_BcsmCamelTDP_CriteriaList = NULL;
	children.push_back(_o_BcsmCamelTDP_CriteriaList);

	// t_BCSM_CAMEL_TDP_CriteriaList
	_t_BCSM_CAMEL_TDP_CriteriaList = NULL;
	children.push_back(_t_BCSM_CAMEL_TDP_CriteriaList);

	// d_csi
	_d_csi = NULL;
	children.push_back(_d_csi);


}
asn1::GmscCamelSubscriptionInfo::~GmscCamelSubscriptionInfo(){

}

asn1::ASN1Node* asn1::GmscCamelSubscriptionInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_t_CSI = new T_CSI();
			_t_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_t_CSI->tlv->tag_value = 0;
			children[0] = _t_CSI;
			return _t_CSI;
		case 1:
			_o_CSI = new O_CSI();
			_o_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_CSI->tlv->tag_value = 1;
			children[1] = _o_CSI;
			return _o_CSI;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_o_BcsmCamelTDP_CriteriaList = new O_BcsmCamelTDPCriteriaList();
			_o_BcsmCamelTDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_BcsmCamelTDP_CriteriaList->tlv->tag_value = 3;
			children[3] = _o_BcsmCamelTDP_CriteriaList;
			return _o_BcsmCamelTDP_CriteriaList;
		case 4:
			_t_BCSM_CAMEL_TDP_CriteriaList = new T_BCSM_CAMEL_TDP_CriteriaList();
			_t_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_t_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_value = 4;
			children[4] = _t_BCSM_CAMEL_TDP_CriteriaList;
			return _t_BCSM_CAMEL_TDP_CriteriaList;
		case 5:
			_d_csi = new D_CSI();
			_d_csi->tlv->tag_class = CONTEXT_SPECIFIC;
			_d_csi->tlv->tag_value = 5;
			children[5] = _d_csi;
			return _d_csi;

		default: return NULL;
	}
}

void asn1::GmscCamelSubscriptionInfo::set_t_CSI(){
	if(_t_CSI == NULL) _t_CSI = (T_CSI*)create_node(0);
}

void asn1::GmscCamelSubscriptionInfo::set_o_CSI(){
	if(_o_CSI == NULL) _o_CSI = (O_CSI*)create_node(1);
}

void asn1::GmscCamelSubscriptionInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::GmscCamelSubscriptionInfo::set_o_BcsmCamelTDP_CriteriaList(){
	if(_o_BcsmCamelTDP_CriteriaList == NULL) _o_BcsmCamelTDP_CriteriaList = (O_BcsmCamelTDPCriteriaList*)create_node(3);
}

void asn1::GmscCamelSubscriptionInfo::set_t_BCSM_CAMEL_TDP_CriteriaList(){
	if(_t_BCSM_CAMEL_TDP_CriteriaList == NULL) _t_BCSM_CAMEL_TDP_CriteriaList = (T_BCSM_CAMEL_TDP_CriteriaList*)create_node(4);
}

void asn1::GmscCamelSubscriptionInfo::set_d_csi(){
	if(_d_csi == NULL) _d_csi = (D_CSI*)create_node(5);
}

//SetReportingStateArg
asn1::SetReportingStateArg::SetReportingStateArg(){
	strcpy(node_type_name, "SetReportingStateArg");
	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// lmsi
	_lmsi = NULL;
	children.push_back(_lmsi);

	// ccbs_Monitoring
	_ccbs_Monitoring = NULL;
	children.push_back(_ccbs_Monitoring);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SetReportingStateArg::~SetReportingStateArg(){

}

asn1::ASN1Node* asn1::SetReportingStateArg::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 0;
			children[0] = _imsi;
			return _imsi;
		case 1:
			_lmsi = new LMSI();
			_lmsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_lmsi->tlv->tag_value = 1;
			children[1] = _lmsi;
			return _lmsi;
		case 2:
			_ccbs_Monitoring = new ReportingState();
			_ccbs_Monitoring->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Monitoring->tlv->tag_value = 2;
			children[2] = _ccbs_Monitoring;
			return _ccbs_Monitoring;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SetReportingStateArg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(0);
}

void asn1::SetReportingStateArg::set_lmsi(){
	if(_lmsi == NULL) _lmsi = (LMSI*)create_node(1);
}

void asn1::SetReportingStateArg::set_ccbs_Monitoring(){
	if(_ccbs_Monitoring == NULL) _ccbs_Monitoring = (ReportingState*)create_node(2);
}

void asn1::SetReportingStateArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//ReportingState
asn1::ReportingState::ReportingState(){
	strcpy(node_type_name, "ReportingState");

}
asn1::ReportingState::~ReportingState(){

}

//SetReportingStateRes
asn1::SetReportingStateRes::SetReportingStateRes(){
	strcpy(node_type_name, "SetReportingStateRes");
	// ccbs_SubscriberStatus
	_ccbs_SubscriberStatus = NULL;
	children.push_back(_ccbs_SubscriberStatus);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SetReportingStateRes::~SetReportingStateRes(){

}

asn1::ASN1Node* asn1::SetReportingStateRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ccbs_SubscriberStatus = new CCBS_SubscriberStatus();
			_ccbs_SubscriberStatus->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_SubscriberStatus->tlv->tag_value = 0;
			children[0] = _ccbs_SubscriberStatus;
			return _ccbs_SubscriberStatus;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SetReportingStateRes::set_ccbs_SubscriberStatus(){
	if(_ccbs_SubscriberStatus == NULL) _ccbs_SubscriberStatus = (CCBS_SubscriberStatus*)create_node(0);
}

void asn1::SetReportingStateRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//CCBS_SubscriberStatus
asn1::CCBS_SubscriberStatus::CCBS_SubscriberStatus(){
	strcpy(node_type_name, "CCBS_SubscriberStatus");

}
asn1::CCBS_SubscriberStatus::~CCBS_SubscriberStatus(){

}

//StatusReportArg
asn1::StatusReportArg::StatusReportArg(){
	strcpy(node_type_name, "StatusReportArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// eventReportData
	_eventReportData = NULL;
	children.push_back(_eventReportData);

	// callReportdata
	_callReportdata = NULL;
	children.push_back(_callReportdata);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::StatusReportArg::~StatusReportArg(){

}

asn1::ASN1Node* asn1::StatusReportArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_eventReportData = new EventReportData();
			_eventReportData->tlv->tag_class = CONTEXT_SPECIFIC;
			_eventReportData->tlv->tag_value = 1;
			children[1] = _eventReportData;
			return _eventReportData;
		case 2:
			_callReportdata = new CallReportData();
			_callReportdata->tlv->tag_class = CONTEXT_SPECIFIC;
			_callReportdata->tlv->tag_value = 2;
			children[2] = _callReportdata;
			return _callReportdata;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::StatusReportArg::set_eventReportData(){
	if(_eventReportData == NULL) _eventReportData = (EventReportData*)create_node(1);
}

void asn1::StatusReportArg::set_callReportdata(){
	if(_callReportdata == NULL) _callReportdata = (CallReportData*)create_node(2);
}

void asn1::StatusReportArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//EventReportData
asn1::EventReportData::EventReportData(){
	strcpy(node_type_name, "EventReportData");
	// ccbs_SubscriberStatus
	_ccbs_SubscriberStatus = NULL;
	children.push_back(_ccbs_SubscriberStatus);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::EventReportData::~EventReportData(){

}

asn1::ASN1Node* asn1::EventReportData::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ccbs_SubscriberStatus = new CCBS_SubscriberStatus();
			_ccbs_SubscriberStatus->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_SubscriberStatus->tlv->tag_value = 0;
			children[0] = _ccbs_SubscriberStatus;
			return _ccbs_SubscriberStatus;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::EventReportData::set_ccbs_SubscriberStatus(){
	if(_ccbs_SubscriberStatus == NULL) _ccbs_SubscriberStatus = (CCBS_SubscriberStatus*)create_node(0);
}

void asn1::EventReportData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//CallReportData
asn1::CallReportData::CallReportData(){
	strcpy(node_type_name, "CallReportData");
	// monitoringMode
	_monitoringMode = NULL;
	children.push_back(_monitoringMode);

	// callOutcome
	_callOutcome = NULL;
	children.push_back(_callOutcome);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CallReportData::~CallReportData(){

}

asn1::ASN1Node* asn1::CallReportData::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_monitoringMode = new MonitoringMode();
			_monitoringMode->tlv->tag_class = CONTEXT_SPECIFIC;
			_monitoringMode->tlv->tag_value = 0;
			children[0] = _monitoringMode;
			return _monitoringMode;
		case 1:
			_callOutcome = new CallOutcome();
			_callOutcome->tlv->tag_class = CONTEXT_SPECIFIC;
			_callOutcome->tlv->tag_value = 1;
			children[1] = _callOutcome;
			return _callOutcome;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CallReportData::set_monitoringMode(){
	if(_monitoringMode == NULL) _monitoringMode = (MonitoringMode*)create_node(0);
}

void asn1::CallReportData::set_callOutcome(){
	if(_callOutcome == NULL) _callOutcome = (CallOutcome*)create_node(1);
}

void asn1::CallReportData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//MonitoringMode
asn1::MonitoringMode::MonitoringMode(){
	strcpy(node_type_name, "MonitoringMode");

}
asn1::MonitoringMode::~MonitoringMode(){

}

//CallOutcome
asn1::CallOutcome::CallOutcome(){
	strcpy(node_type_name, "CallOutcome");

}
asn1::CallOutcome::~CallOutcome(){

}

//StatusReportRes
asn1::StatusReportRes::StatusReportRes(){
	strcpy(node_type_name, "StatusReportRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::StatusReportRes::~StatusReportRes(){

}

asn1::ASN1Node* asn1::StatusReportRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::StatusReportRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//RemoteUserFreeArg
asn1::RemoteUserFreeArg::RemoteUserFreeArg(){
	strcpy(node_type_name, "RemoteUserFreeArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// callInfo
	_callInfo = NULL;
	_callInfo = new ExternalSignalInfo();
	_callInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_callInfo->tlv->tag_value = 1;
	children.push_back(_callInfo);

	// ccbs_Feature
	_ccbs_Feature = NULL;
	_ccbs_Feature = new CCBS_Feature();
	_ccbs_Feature->tlv->tag_class = CONTEXT_SPECIFIC;
	_ccbs_Feature->tlv->tag_value = 2;
	children.push_back(_ccbs_Feature);

	// translatedB_Number
	_translatedB_Number = NULL;
	_translatedB_Number = new ISDN_AddressString();
	_translatedB_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_translatedB_Number->tlv->tag_value = 3;
	children.push_back(_translatedB_Number);

	// replaceB_Number
	_replaceB_Number = NULL;
	children.push_back(_replaceB_Number);

	// alertingPattern
	_alertingPattern = NULL;
	children.push_back(_alertingPattern);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::RemoteUserFreeArg::~RemoteUserFreeArg(){

}

asn1::ASN1Node* asn1::RemoteUserFreeArg::create_node(unsigned int _index){
	switch(_index){
		case 4:
			_replaceB_Number = new Null();
			_replaceB_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_replaceB_Number->tlv->tag_value = 4;
			children[4] = _replaceB_Number;
			return _replaceB_Number;
		case 5:
			_alertingPattern = new AlertingPattern();
			_alertingPattern->tlv->tag_class = CONTEXT_SPECIFIC;
			_alertingPattern->tlv->tag_value = 5;
			children[5] = _alertingPattern;
			return _alertingPattern;
		case 6:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 6;
			children[6] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::RemoteUserFreeArg::set_replaceB_Number(){
	if(_replaceB_Number == NULL) _replaceB_Number = (Null*)create_node(4);
}

void asn1::RemoteUserFreeArg::set_alertingPattern(){
	if(_alertingPattern == NULL) _alertingPattern = (AlertingPattern*)create_node(5);
}

void asn1::RemoteUserFreeArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(6);
}

//RemoteUserFreeRes
asn1::RemoteUserFreeRes::RemoteUserFreeRes(){
	strcpy(node_type_name, "RemoteUserFreeRes");
	// ruf_Outcome
	_ruf_Outcome = NULL;
	_ruf_Outcome = new RUF_Outcome();
	_ruf_Outcome->tlv->tag_class = CONTEXT_SPECIFIC;
	_ruf_Outcome->tlv->tag_value = 0;
	children.push_back(_ruf_Outcome);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::RemoteUserFreeRes::~RemoteUserFreeRes(){

}

asn1::ASN1Node* asn1::RemoteUserFreeRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::RemoteUserFreeRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//RUF_Outcome
asn1::RUF_Outcome::RUF_Outcome(){
	strcpy(node_type_name, "RUF_Outcome");

}
asn1::RUF_Outcome::~RUF_Outcome(){

}

//IST_AlertArg
asn1::IST_AlertArg::IST_AlertArg(){
	strcpy(node_type_name, "IST_AlertArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::IST_AlertArg::~IST_AlertArg(){

}

asn1::ASN1Node* asn1::IST_AlertArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::IST_AlertArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//IST_AlertRes
asn1::IST_AlertRes::IST_AlertRes(){
	strcpy(node_type_name, "IST_AlertRes");
	// istAlertTimer
	_istAlertTimer = NULL;
	children.push_back(_istAlertTimer);

	// istInformationWithdraw
	_istInformationWithdraw = NULL;
	children.push_back(_istInformationWithdraw);

	// callTerminationIndicator
	_callTerminationIndicator = NULL;
	children.push_back(_callTerminationIndicator);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::IST_AlertRes::~IST_AlertRes(){

}

asn1::ASN1Node* asn1::IST_AlertRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_istAlertTimer = new IST_AlertTimerValue();
			_istAlertTimer->tlv->tag_class = CONTEXT_SPECIFIC;
			_istAlertTimer->tlv->tag_value = 0;
			children[0] = _istAlertTimer;
			return _istAlertTimer;
		case 1:
			_istInformationWithdraw = new Null();
			_istInformationWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_istInformationWithdraw->tlv->tag_value = 1;
			children[1] = _istInformationWithdraw;
			return _istInformationWithdraw;
		case 2:
			_callTerminationIndicator = new CallTerminationIndicator();
			_callTerminationIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_callTerminationIndicator->tlv->tag_value = 2;
			children[2] = _callTerminationIndicator;
			return _callTerminationIndicator;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::IST_AlertRes::set_istAlertTimer(){
	if(_istAlertTimer == NULL) _istAlertTimer = (IST_AlertTimerValue*)create_node(0);
}

void asn1::IST_AlertRes::set_istInformationWithdraw(){
	if(_istInformationWithdraw == NULL) _istInformationWithdraw = (Null*)create_node(1);
}

void asn1::IST_AlertRes::set_callTerminationIndicator(){
	if(_callTerminationIndicator == NULL) _callTerminationIndicator = (CallTerminationIndicator*)create_node(2);
}

void asn1::IST_AlertRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//IST_CommandArg
asn1::IST_CommandArg::IST_CommandArg(){
	strcpy(node_type_name, "IST_CommandArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::IST_CommandArg::~IST_CommandArg(){

}

asn1::ASN1Node* asn1::IST_CommandArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::IST_CommandArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//IST_CommandRes
asn1::IST_CommandRes::IST_CommandRes(){
	strcpy(node_type_name, "IST_CommandRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::IST_CommandRes::~IST_CommandRes(){

}

asn1::ASN1Node* asn1::IST_CommandRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::IST_CommandRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//CallTerminationIndicator
asn1::CallTerminationIndicator::CallTerminationIndicator(){
	strcpy(node_type_name, "CallTerminationIndicator");

}
asn1::CallTerminationIndicator::~CallTerminationIndicator(){

}

//ReleaseResourcesArg
asn1::ReleaseResourcesArg::ReleaseResourcesArg(){
	strcpy(node_type_name, "ReleaseResourcesArg");
	// msrn
	_msrn = NULL;
	_msrn = new ISDN_AddressString();
	children.push_back(_msrn);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ReleaseResourcesArg::~ReleaseResourcesArg(){

}

asn1::ASN1Node* asn1::ReleaseResourcesArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ReleaseResourcesArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//ReleaseResourcesRes
asn1::ReleaseResourcesRes::ReleaseResourcesRes(){
	strcpy(node_type_name, "ReleaseResourcesRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ReleaseResourcesRes::~ReleaseResourcesRes(){

}

asn1::ASN1Node* asn1::ReleaseResourcesRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ReleaseResourcesRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

