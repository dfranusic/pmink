#include<MAP-SM-DataTypes.h>
#include<iostream>

//RoutingInfoForSM_Arg
asn1::RoutingInfoForSM_Arg::RoutingInfoForSM_Arg(){
	strcpy(node_type_name, "RoutingInfoForSM_Arg");
	// msisdn
	_msisdn = NULL;
	_msisdn = new ISDN_AddressString();
	_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
	_msisdn->tlv->tag_value = 0;
	children.push_back(_msisdn);

	// sm_RP_PRI
	_sm_RP_PRI = NULL;
	_sm_RP_PRI = new Boolean();
	_sm_RP_PRI->tlv->tag_class = CONTEXT_SPECIFIC;
	_sm_RP_PRI->tlv->tag_value = 1;
	children.push_back(_sm_RP_PRI);

	// serviceCentreAddress
	_serviceCentreAddress = NULL;
	_serviceCentreAddress = new AddressString();
	_serviceCentreAddress->tlv->tag_class = CONTEXT_SPECIFIC;
	_serviceCentreAddress->tlv->tag_value = 2;
	children.push_back(_serviceCentreAddress);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// gprsSupportIndicator
	_gprsSupportIndicator = NULL;
	children.push_back(_gprsSupportIndicator);

	// sm_RP_MTI
	_sm_RP_MTI = NULL;
	children.push_back(_sm_RP_MTI);

	// sm_RP_SMEA
	_sm_RP_SMEA = NULL;
	children.push_back(_sm_RP_SMEA);

	// sm_deliveryNotIntended
	_sm_deliveryNotIntended = NULL;
	children.push_back(_sm_deliveryNotIntended);

	// ip_sm_gwGuidanceIndicator
	_ip_sm_gwGuidanceIndicator = NULL;
	children.push_back(_ip_sm_gwGuidanceIndicator);

	// imsi
	_imsi = NULL;
	children.push_back(_imsi);


}
asn1::RoutingInfoForSM_Arg::~RoutingInfoForSM_Arg(){

}

asn1::ASN1Node* asn1::RoutingInfoForSM_Arg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 6;
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_gprsSupportIndicator = new Null();
			_gprsSupportIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprsSupportIndicator->tlv->tag_value = 7;
			children[4] = _gprsSupportIndicator;
			return _gprsSupportIndicator;
		case 5:
			_sm_RP_MTI = new SM_RP_MTI();
			_sm_RP_MTI->tlv->tag_class = CONTEXT_SPECIFIC;
			_sm_RP_MTI->tlv->tag_value = 8;
			children[5] = _sm_RP_MTI;
			return _sm_RP_MTI;
		case 6:
			_sm_RP_SMEA = new SM_RP_SMEA();
			_sm_RP_SMEA->tlv->tag_class = CONTEXT_SPECIFIC;
			_sm_RP_SMEA->tlv->tag_value = 9;
			children[6] = _sm_RP_SMEA;
			return _sm_RP_SMEA;
		case 7:
			_sm_deliveryNotIntended = new SM_DeliveryNotIntended();
			_sm_deliveryNotIntended->tlv->tag_class = CONTEXT_SPECIFIC;
			_sm_deliveryNotIntended->tlv->tag_value = 10;
			children[7] = _sm_deliveryNotIntended;
			return _sm_deliveryNotIntended;
		case 8:
			_ip_sm_gwGuidanceIndicator = new Null();
			_ip_sm_gwGuidanceIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_ip_sm_gwGuidanceIndicator->tlv->tag_value = 11;
			children[8] = _ip_sm_gwGuidanceIndicator;
			return _ip_sm_gwGuidanceIndicator;
		case 9:
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 12;
			children[9] = _imsi;
			return _imsi;

		default: return NULL;
	}
}

void asn1::RoutingInfoForSM_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::RoutingInfoForSM_Arg::set_gprsSupportIndicator(){
	if(_gprsSupportIndicator == NULL) _gprsSupportIndicator = (Null*)create_node(4);
}

void asn1::RoutingInfoForSM_Arg::set_sm_RP_MTI(){
	if(_sm_RP_MTI == NULL) _sm_RP_MTI = (SM_RP_MTI*)create_node(5);
}

void asn1::RoutingInfoForSM_Arg::set_sm_RP_SMEA(){
	if(_sm_RP_SMEA == NULL) _sm_RP_SMEA = (SM_RP_SMEA*)create_node(6);
}

void asn1::RoutingInfoForSM_Arg::set_sm_deliveryNotIntended(){
	if(_sm_deliveryNotIntended == NULL) _sm_deliveryNotIntended = (SM_DeliveryNotIntended*)create_node(7);
}

void asn1::RoutingInfoForSM_Arg::set_ip_sm_gwGuidanceIndicator(){
	if(_ip_sm_gwGuidanceIndicator == NULL) _ip_sm_gwGuidanceIndicator = (Null*)create_node(8);
}

void asn1::RoutingInfoForSM_Arg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(9);
}

//SM_DeliveryNotIntended
asn1::SM_DeliveryNotIntended::SM_DeliveryNotIntended(){
	strcpy(node_type_name, "SM_DeliveryNotIntended");

}
asn1::SM_DeliveryNotIntended::~SM_DeliveryNotIntended(){

}

//SM_RP_MTI
asn1::SM_RP_MTI::SM_RP_MTI(){
	strcpy(node_type_name, "SM_RP_MTI");

}
asn1::SM_RP_MTI::~SM_RP_MTI(){

}

//SM_RP_SMEA
asn1::SM_RP_SMEA::SM_RP_SMEA(){
	strcpy(node_type_name, "SM_RP_SMEA");

}
asn1::SM_RP_SMEA::~SM_RP_SMEA(){

}

//RoutingInfoForSM_Res
asn1::RoutingInfoForSM_Res::RoutingInfoForSM_Res(){
	strcpy(node_type_name, "RoutingInfoForSM_Res");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// locationInfoWithLMSI
	_locationInfoWithLMSI = NULL;
	_locationInfoWithLMSI = new LocationInfoWithLMSI();
	_locationInfoWithLMSI->tlv->tag_class = CONTEXT_SPECIFIC;
	_locationInfoWithLMSI->tlv->tag_value = 0;
	children.push_back(_locationInfoWithLMSI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// ip_sm_gwGuidance
	_ip_sm_gwGuidance = NULL;
	children.push_back(_ip_sm_gwGuidance);


}
asn1::RoutingInfoForSM_Res::~RoutingInfoForSM_Res(){

}

asn1::ASN1Node* asn1::RoutingInfoForSM_Res::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 4;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_ip_sm_gwGuidance = new IP_SM_GW_Guidance();
			_ip_sm_gwGuidance->tlv->tag_class = CONTEXT_SPECIFIC;
			_ip_sm_gwGuidance->tlv->tag_value = 5;
			children[3] = _ip_sm_gwGuidance;
			return _ip_sm_gwGuidance;

		default: return NULL;
	}
}

void asn1::RoutingInfoForSM_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::RoutingInfoForSM_Res::set_ip_sm_gwGuidance(){
	if(_ip_sm_gwGuidance == NULL) _ip_sm_gwGuidance = (IP_SM_GW_Guidance*)create_node(3);
}

//IP_SM_GW_Guidance
asn1::IP_SM_GW_Guidance::IP_SM_GW_Guidance(){
	strcpy(node_type_name, "IP_SM_GW_Guidance");
	// minimumDeliveryTimeValue
	_minimumDeliveryTimeValue = NULL;
	_minimumDeliveryTimeValue = new SM_DeliveryTimerValue();
	children.push_back(_minimumDeliveryTimeValue);

	// recommendedDeliveryTimeValue
	_recommendedDeliveryTimeValue = NULL;
	_recommendedDeliveryTimeValue = new SM_DeliveryTimerValue();
	children.push_back(_recommendedDeliveryTimeValue);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::IP_SM_GW_Guidance::~IP_SM_GW_Guidance(){

}

asn1::ASN1Node* asn1::IP_SM_GW_Guidance::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::IP_SM_GW_Guidance::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//LocationInfoWithLMSI
asn1::LocationInfoWithLMSI::LocationInfoWithLMSI(){
	strcpy(node_type_name, "LocationInfoWithLMSI");
	// networkNode_Number
	_networkNode_Number = NULL;
	_networkNode_Number = new ISDN_AddressString();
	_networkNode_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_networkNode_Number->tlv->tag_value = 1;
	children.push_back(_networkNode_Number);

	// lmsi
	_lmsi = NULL;
	children.push_back(_lmsi);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// gprsNodeIndicator
	_gprsNodeIndicator = NULL;
	children.push_back(_gprsNodeIndicator);

	// additional_Number
	_additional_Number = NULL;
	children.push_back(_additional_Number);

	// networkNodeDiameterAddress
	_networkNodeDiameterAddress = NULL;
	children.push_back(_networkNodeDiameterAddress);

	// additionalNetworkNodeDiameterAddress
	_additionalNetworkNodeDiameterAddress = NULL;
	children.push_back(_additionalNetworkNodeDiameterAddress);

	// thirdNumber
	_thirdNumber = NULL;
	children.push_back(_thirdNumber);

	// thirdNetworkNodeDiameterAddress
	_thirdNetworkNodeDiameterAddress = NULL;
	children.push_back(_thirdNetworkNodeDiameterAddress);

	// imsNodeIndicator
	_imsNodeIndicator = NULL;
	children.push_back(_imsNodeIndicator);


}
asn1::LocationInfoWithLMSI::~LocationInfoWithLMSI(){

}

asn1::ASN1Node* asn1::LocationInfoWithLMSI::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_lmsi = new LMSI();
			children[1] = _lmsi;
			return _lmsi;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_gprsNodeIndicator = new Null();
			_gprsNodeIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprsNodeIndicator->tlv->tag_value = 5;
			children[3] = _gprsNodeIndicator;
			return _gprsNodeIndicator;
		case 4:
                {
                        _additional_Number = new Additional_Number();
                        ASN1Node* _additional_Number_wrapper = new ASN1Node();
                        _additional_Number_wrapper->tlv = new TLVNode();
                        _additional_Number_wrapper->tlv->is_explicit = true;
                        _additional_Number_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _additional_Number_wrapper->tlv->tag_value = 6;
                        _additional_Number_wrapper->children.push_back(_additional_Number);
                        children[4] = _additional_Number_wrapper;
                        return _additional_Number;
                }
		case 5:
			_networkNodeDiameterAddress = new NetworkNodeDiameterAddress();
			_networkNodeDiameterAddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_networkNodeDiameterAddress->tlv->tag_value = 7;
			children[5] = _networkNodeDiameterAddress;
			return _networkNodeDiameterAddress;
		case 6:
			_additionalNetworkNodeDiameterAddress = new NetworkNodeDiameterAddress();
			_additionalNetworkNodeDiameterAddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalNetworkNodeDiameterAddress->tlv->tag_value = 8;
			children[6] = _additionalNetworkNodeDiameterAddress;
			return _additionalNetworkNodeDiameterAddress;
		case 7:
                {
                        _thirdNumber = new Additional_Number();
                        ASN1Node* _thirdNumber_wrapper = new ASN1Node();
                        _thirdNumber_wrapper->tlv = new TLVNode();
                        _thirdNumber_wrapper->tlv->is_explicit = true;
                        _thirdNumber_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _thirdNumber_wrapper->tlv->tag_value = 9;
                        _thirdNumber_wrapper->children.push_back(_thirdNumber);
                        children[7] = _thirdNumber_wrapper;
                        return _thirdNumber;
                }
		case 8:
			_thirdNetworkNodeDiameterAddress = new NetworkNodeDiameterAddress();
			_thirdNetworkNodeDiameterAddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_thirdNetworkNodeDiameterAddress->tlv->tag_value = 10;
			children[8] = _thirdNetworkNodeDiameterAddress;
			return _thirdNetworkNodeDiameterAddress;
		case 9:
			_imsNodeIndicator = new Null();
			_imsNodeIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsNodeIndicator->tlv->tag_value = 11;
			children[9] = _imsNodeIndicator;
			return _imsNodeIndicator;

		default: return NULL;
	}
}

void asn1::LocationInfoWithLMSI::set_lmsi(){
	if(_lmsi == NULL) _lmsi = (LMSI*)create_node(1);
}

void asn1::LocationInfoWithLMSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::LocationInfoWithLMSI::set_gprsNodeIndicator(){
	if(_gprsNodeIndicator == NULL) _gprsNodeIndicator = (Null*)create_node(3);
}

void asn1::LocationInfoWithLMSI::set_additional_Number(){
	if(_additional_Number == NULL) _additional_Number = (Additional_Number*)create_node(4);
}

void asn1::LocationInfoWithLMSI::set_networkNodeDiameterAddress(){
	if(_networkNodeDiameterAddress == NULL) _networkNodeDiameterAddress = (NetworkNodeDiameterAddress*)create_node(5);
}

void asn1::LocationInfoWithLMSI::set_additionalNetworkNodeDiameterAddress(){
	if(_additionalNetworkNodeDiameterAddress == NULL) _additionalNetworkNodeDiameterAddress = (NetworkNodeDiameterAddress*)create_node(6);
}

void asn1::LocationInfoWithLMSI::set_thirdNumber(){
	if(_thirdNumber == NULL) _thirdNumber = (Additional_Number*)create_node(7);
}

void asn1::LocationInfoWithLMSI::set_thirdNetworkNodeDiameterAddress(){
	if(_thirdNetworkNodeDiameterAddress == NULL) _thirdNetworkNodeDiameterAddress = (NetworkNodeDiameterAddress*)create_node(8);
}

void asn1::LocationInfoWithLMSI::set_imsNodeIndicator(){
	if(_imsNodeIndicator == NULL) _imsNodeIndicator = (Null*)create_node(9);
}

//Additional_Number
asn1::Additional_Number::Additional_Number(){
	strcpy(node_type_name, "Additional_Number");
	// msc_Number
	_msc_Number = NULL;
	_msc_Number = new ISDN_AddressString();
	_msc_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_msc_Number->tlv->tag_value = 0;
	children.push_back(_msc_Number);

	// sgsn_Number
	_sgsn_Number = NULL;
	_sgsn_Number = new ISDN_AddressString();
	_sgsn_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_sgsn_Number->tlv->tag_value = 1;
	children.push_back(_sgsn_Number);


}
asn1::Additional_Number::~Additional_Number(){

}

//MO_ForwardSM_Arg
asn1::MO_ForwardSM_Arg::MO_ForwardSM_Arg(){
	strcpy(node_type_name, "MO_ForwardSM_Arg");
	// sm_RP_DA
	_sm_RP_DA = NULL;
	_sm_RP_DA = new SM_RP_DA();
	children.push_back(_sm_RP_DA);

	// sm_RP_OA
	_sm_RP_OA = NULL;
	_sm_RP_OA = new SM_RP_OA();
	children.push_back(_sm_RP_OA);

	// sm_RP_UI
	_sm_RP_UI = NULL;
	_sm_RP_UI = new SignalInfo();
	children.push_back(_sm_RP_UI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// imsi
	_imsi = NULL;
	children.push_back(_imsi);


}
asn1::MO_ForwardSM_Arg::~MO_ForwardSM_Arg(){

}

asn1::ASN1Node* asn1::MO_ForwardSM_Arg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_extensionContainer = new ExtensionContainer();
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_imsi = new IMSI();
			children[4] = _imsi;
			return _imsi;

		default: return NULL;
	}
}

void asn1::MO_ForwardSM_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::MO_ForwardSM_Arg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(4);
}

//MO_ForwardSM_Res
asn1::MO_ForwardSM_Res::MO_ForwardSM_Res(){
	strcpy(node_type_name, "MO_ForwardSM_Res");
	// sm_RP_UI
	_sm_RP_UI = NULL;
	children.push_back(_sm_RP_UI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MO_ForwardSM_Res::~MO_ForwardSM_Res(){

}

asn1::ASN1Node* asn1::MO_ForwardSM_Res::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_sm_RP_UI = new SignalInfo();
			children[0] = _sm_RP_UI;
			return _sm_RP_UI;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::MO_ForwardSM_Res::set_sm_RP_UI(){
	if(_sm_RP_UI == NULL) _sm_RP_UI = (SignalInfo*)create_node(0);
}

void asn1::MO_ForwardSM_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//MT_ForwardSM_Arg
asn1::MT_ForwardSM_Arg::MT_ForwardSM_Arg(){
	strcpy(node_type_name, "MT_ForwardSM_Arg");
	// sm_RP_DA
	_sm_RP_DA = NULL;
	_sm_RP_DA = new SM_RP_DA();
	children.push_back(_sm_RP_DA);

	// sm_RP_OA
	_sm_RP_OA = NULL;
	_sm_RP_OA = new SM_RP_OA();
	children.push_back(_sm_RP_OA);

	// sm_RP_UI
	_sm_RP_UI = NULL;
	_sm_RP_UI = new SignalInfo();
	children.push_back(_sm_RP_UI);

	// moreMessagesToSend
	_moreMessagesToSend = NULL;
	children.push_back(_moreMessagesToSend);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// smDeliveryTimer
	_smDeliveryTimer = NULL;
	children.push_back(_smDeliveryTimer);

	// smDeliveryStartTime
	_smDeliveryStartTime = NULL;
	children.push_back(_smDeliveryStartTime);

	// smsOverIP_OnlyIndicator
	_smsOverIP_OnlyIndicator = NULL;
	children.push_back(_smsOverIP_OnlyIndicator);


}
asn1::MT_ForwardSM_Arg::~MT_ForwardSM_Arg(){

}

asn1::ASN1Node* asn1::MT_ForwardSM_Arg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_moreMessagesToSend = new Null();
			children[3] = _moreMessagesToSend;
			return _moreMessagesToSend;
		case 4:
			_extensionContainer = new ExtensionContainer();
			children[4] = _extensionContainer;
			return _extensionContainer;
		case 5:
			_smDeliveryTimer = new SM_DeliveryTimerValue();
			children[5] = _smDeliveryTimer;
			return _smDeliveryTimer;
		case 6:
			_smDeliveryStartTime = new Time();
			children[6] = _smDeliveryStartTime;
			return _smDeliveryStartTime;
		case 7:
			_smsOverIP_OnlyIndicator = new Null();
			_smsOverIP_OnlyIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_smsOverIP_OnlyIndicator->tlv->tag_value = 0;
			children[7] = _smsOverIP_OnlyIndicator;
			return _smsOverIP_OnlyIndicator;

		default: return NULL;
	}
}

void asn1::MT_ForwardSM_Arg::set_moreMessagesToSend(){
	if(_moreMessagesToSend == NULL) _moreMessagesToSend = (Null*)create_node(3);
}

void asn1::MT_ForwardSM_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

void asn1::MT_ForwardSM_Arg::set_smDeliveryTimer(){
	if(_smDeliveryTimer == NULL) _smDeliveryTimer = (SM_DeliveryTimerValue*)create_node(5);
}

void asn1::MT_ForwardSM_Arg::set_smDeliveryStartTime(){
	if(_smDeliveryStartTime == NULL) _smDeliveryStartTime = (Time*)create_node(6);
}

void asn1::MT_ForwardSM_Arg::set_smsOverIP_OnlyIndicator(){
	if(_smsOverIP_OnlyIndicator == NULL) _smsOverIP_OnlyIndicator = (Null*)create_node(7);
}

//MT_ForwardSM_Res
asn1::MT_ForwardSM_Res::MT_ForwardSM_Res(){
	strcpy(node_type_name, "MT_ForwardSM_Res");
	// sm_RP_UI
	_sm_RP_UI = NULL;
	children.push_back(_sm_RP_UI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MT_ForwardSM_Res::~MT_ForwardSM_Res(){

}

asn1::ASN1Node* asn1::MT_ForwardSM_Res::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_sm_RP_UI = new SignalInfo();
			children[0] = _sm_RP_UI;
			return _sm_RP_UI;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::MT_ForwardSM_Res::set_sm_RP_UI(){
	if(_sm_RP_UI == NULL) _sm_RP_UI = (SignalInfo*)create_node(0);
}

void asn1::MT_ForwardSM_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//SM_RP_DA
asn1::SM_RP_DA::SM_RP_DA(){
	strcpy(node_type_name, "SM_RP_DA");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// lmsi
	_lmsi = NULL;
	_lmsi = new LMSI();
	_lmsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_lmsi->tlv->tag_value = 1;
	children.push_back(_lmsi);

	// serviceCentreAddressDA
	_serviceCentreAddressDA = NULL;
	_serviceCentreAddressDA = new AddressString();
	_serviceCentreAddressDA->tlv->tag_class = CONTEXT_SPECIFIC;
	_serviceCentreAddressDA->tlv->tag_value = 4;
	children.push_back(_serviceCentreAddressDA);

	// noSM_RP_DA
	_noSM_RP_DA = NULL;
	_noSM_RP_DA = new Null();
	_noSM_RP_DA->tlv->tag_class = CONTEXT_SPECIFIC;
	_noSM_RP_DA->tlv->tag_value = 5;
	children.push_back(_noSM_RP_DA);


}
asn1::SM_RP_DA::~SM_RP_DA(){

}

//SM_RP_OA
asn1::SM_RP_OA::SM_RP_OA(){
	strcpy(node_type_name, "SM_RP_OA");
	// msisdn
	_msisdn = NULL;
	_msisdn = new ISDN_AddressString();
	_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
	_msisdn->tlv->tag_value = 2;
	children.push_back(_msisdn);

	// serviceCentreAddressOA
	_serviceCentreAddressOA = NULL;
	_serviceCentreAddressOA = new AddressString();
	_serviceCentreAddressOA->tlv->tag_class = CONTEXT_SPECIFIC;
	_serviceCentreAddressOA->tlv->tag_value = 4;
	children.push_back(_serviceCentreAddressOA);

	// noSM_RP_OA
	_noSM_RP_OA = NULL;
	_noSM_RP_OA = new Null();
	_noSM_RP_OA->tlv->tag_class = CONTEXT_SPECIFIC;
	_noSM_RP_OA->tlv->tag_value = 5;
	children.push_back(_noSM_RP_OA);


}
asn1::SM_RP_OA::~SM_RP_OA(){

}

//SM_DeliveryTimerValue
asn1::SM_DeliveryTimerValue::SM_DeliveryTimerValue(){
	strcpy(node_type_name, "SM_DeliveryTimerValue");

}
asn1::SM_DeliveryTimerValue::~SM_DeliveryTimerValue(){

}

//ReportSM_DeliveryStatusArg
asn1::ReportSM_DeliveryStatusArg::ReportSM_DeliveryStatusArg(){
	strcpy(node_type_name, "ReportSM_DeliveryStatusArg");
	// msisdn
	_msisdn = NULL;
	_msisdn = new ISDN_AddressString();
	children.push_back(_msisdn);

	// serviceCentreAddress
	_serviceCentreAddress = NULL;
	_serviceCentreAddress = new AddressString();
	children.push_back(_serviceCentreAddress);

	// sm_DeliveryOutcome
	_sm_DeliveryOutcome = NULL;
	_sm_DeliveryOutcome = new SM_DeliveryOutcome();
	children.push_back(_sm_DeliveryOutcome);

	// absentSubscriberDiagnosticSM
	_absentSubscriberDiagnosticSM = NULL;
	children.push_back(_absentSubscriberDiagnosticSM);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// gprsSupportIndicator
	_gprsSupportIndicator = NULL;
	children.push_back(_gprsSupportIndicator);

	// deliveryOutcomeIndicator
	_deliveryOutcomeIndicator = NULL;
	children.push_back(_deliveryOutcomeIndicator);

	// additionalSM_DeliveryOutcome
	_additionalSM_DeliveryOutcome = NULL;
	children.push_back(_additionalSM_DeliveryOutcome);

	// additionalAbsentSubscriberDiagnosticSM
	_additionalAbsentSubscriberDiagnosticSM = NULL;
	children.push_back(_additionalAbsentSubscriberDiagnosticSM);

	// ip_sm_gw_Indicator
	_ip_sm_gw_Indicator = NULL;
	children.push_back(_ip_sm_gw_Indicator);

	// ip_sm_gw_sm_deliveryOutcome
	_ip_sm_gw_sm_deliveryOutcome = NULL;
	children.push_back(_ip_sm_gw_sm_deliveryOutcome);

	// ip_sm_gw_absentSubscriberDiagnosticSM
	_ip_sm_gw_absentSubscriberDiagnosticSM = NULL;
	children.push_back(_ip_sm_gw_absentSubscriberDiagnosticSM);

	// imsi
	_imsi = NULL;
	children.push_back(_imsi);


}
asn1::ReportSM_DeliveryStatusArg::~ReportSM_DeliveryStatusArg(){

}

asn1::ASN1Node* asn1::ReportSM_DeliveryStatusArg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_absentSubscriberDiagnosticSM = new AbsentSubscriberDiagnosticSM();
			_absentSubscriberDiagnosticSM->tlv->tag_class = CONTEXT_SPECIFIC;
			_absentSubscriberDiagnosticSM->tlv->tag_value = 0;
			children[3] = _absentSubscriberDiagnosticSM;
			return _absentSubscriberDiagnosticSM;
		case 4:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[4] = _extensionContainer;
			return _extensionContainer;
		case 5:
			_gprsSupportIndicator = new Null();
			_gprsSupportIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprsSupportIndicator->tlv->tag_value = 2;
			children[5] = _gprsSupportIndicator;
			return _gprsSupportIndicator;
		case 6:
			_deliveryOutcomeIndicator = new Null();
			_deliveryOutcomeIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_deliveryOutcomeIndicator->tlv->tag_value = 3;
			children[6] = _deliveryOutcomeIndicator;
			return _deliveryOutcomeIndicator;
		case 7:
			_additionalSM_DeliveryOutcome = new SM_DeliveryOutcome();
			_additionalSM_DeliveryOutcome->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalSM_DeliveryOutcome->tlv->tag_value = 4;
			children[7] = _additionalSM_DeliveryOutcome;
			return _additionalSM_DeliveryOutcome;
		case 8:
			_additionalAbsentSubscriberDiagnosticSM = new AbsentSubscriberDiagnosticSM();
			_additionalAbsentSubscriberDiagnosticSM->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalAbsentSubscriberDiagnosticSM->tlv->tag_value = 5;
			children[8] = _additionalAbsentSubscriberDiagnosticSM;
			return _additionalAbsentSubscriberDiagnosticSM;
		case 9:
			_ip_sm_gw_Indicator = new Null();
			_ip_sm_gw_Indicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_ip_sm_gw_Indicator->tlv->tag_value = 6;
			children[9] = _ip_sm_gw_Indicator;
			return _ip_sm_gw_Indicator;
		case 10:
			_ip_sm_gw_sm_deliveryOutcome = new SM_DeliveryOutcome();
			_ip_sm_gw_sm_deliveryOutcome->tlv->tag_class = CONTEXT_SPECIFIC;
			_ip_sm_gw_sm_deliveryOutcome->tlv->tag_value = 7;
			children[10] = _ip_sm_gw_sm_deliveryOutcome;
			return _ip_sm_gw_sm_deliveryOutcome;
		case 11:
			_ip_sm_gw_absentSubscriberDiagnosticSM = new AbsentSubscriberDiagnosticSM();
			_ip_sm_gw_absentSubscriberDiagnosticSM->tlv->tag_class = CONTEXT_SPECIFIC;
			_ip_sm_gw_absentSubscriberDiagnosticSM->tlv->tag_value = 8;
			children[11] = _ip_sm_gw_absentSubscriberDiagnosticSM;
			return _ip_sm_gw_absentSubscriberDiagnosticSM;
		case 12:
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 9;
			children[12] = _imsi;
			return _imsi;

		default: return NULL;
	}
}

void asn1::ReportSM_DeliveryStatusArg::set_absentSubscriberDiagnosticSM(){
	if(_absentSubscriberDiagnosticSM == NULL) _absentSubscriberDiagnosticSM = (AbsentSubscriberDiagnosticSM*)create_node(3);
}

void asn1::ReportSM_DeliveryStatusArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

void asn1::ReportSM_DeliveryStatusArg::set_gprsSupportIndicator(){
	if(_gprsSupportIndicator == NULL) _gprsSupportIndicator = (Null*)create_node(5);
}

void asn1::ReportSM_DeliveryStatusArg::set_deliveryOutcomeIndicator(){
	if(_deliveryOutcomeIndicator == NULL) _deliveryOutcomeIndicator = (Null*)create_node(6);
}

void asn1::ReportSM_DeliveryStatusArg::set_additionalSM_DeliveryOutcome(){
	if(_additionalSM_DeliveryOutcome == NULL) _additionalSM_DeliveryOutcome = (SM_DeliveryOutcome*)create_node(7);
}

void asn1::ReportSM_DeliveryStatusArg::set_additionalAbsentSubscriberDiagnosticSM(){
	if(_additionalAbsentSubscriberDiagnosticSM == NULL) _additionalAbsentSubscriberDiagnosticSM = (AbsentSubscriberDiagnosticSM*)create_node(8);
}

void asn1::ReportSM_DeliveryStatusArg::set_ip_sm_gw_Indicator(){
	if(_ip_sm_gw_Indicator == NULL) _ip_sm_gw_Indicator = (Null*)create_node(9);
}

void asn1::ReportSM_DeliveryStatusArg::set_ip_sm_gw_sm_deliveryOutcome(){
	if(_ip_sm_gw_sm_deliveryOutcome == NULL) _ip_sm_gw_sm_deliveryOutcome = (SM_DeliveryOutcome*)create_node(10);
}

void asn1::ReportSM_DeliveryStatusArg::set_ip_sm_gw_absentSubscriberDiagnosticSM(){
	if(_ip_sm_gw_absentSubscriberDiagnosticSM == NULL) _ip_sm_gw_absentSubscriberDiagnosticSM = (AbsentSubscriberDiagnosticSM*)create_node(11);
}

void asn1::ReportSM_DeliveryStatusArg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(12);
}

//SM_DeliveryOutcome
asn1::SM_DeliveryOutcome::SM_DeliveryOutcome(){
	strcpy(node_type_name, "SM_DeliveryOutcome");

}
asn1::SM_DeliveryOutcome::~SM_DeliveryOutcome(){

}

//ReportSM_DeliveryStatusRes
asn1::ReportSM_DeliveryStatusRes::ReportSM_DeliveryStatusRes(){
	strcpy(node_type_name, "ReportSM_DeliveryStatusRes");
	// storedMSISDN
	_storedMSISDN = NULL;
	children.push_back(_storedMSISDN);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ReportSM_DeliveryStatusRes::~ReportSM_DeliveryStatusRes(){

}

asn1::ASN1Node* asn1::ReportSM_DeliveryStatusRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_storedMSISDN = new ISDN_AddressString();
			children[0] = _storedMSISDN;
			return _storedMSISDN;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ReportSM_DeliveryStatusRes::set_storedMSISDN(){
	if(_storedMSISDN == NULL) _storedMSISDN = (ISDN_AddressString*)create_node(0);
}

void asn1::ReportSM_DeliveryStatusRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//AlertServiceCentreArg
asn1::AlertServiceCentreArg::AlertServiceCentreArg(){
	strcpy(node_type_name, "AlertServiceCentreArg");
	// msisdn
	_msisdn = NULL;
	_msisdn = new ISDN_AddressString();
	children.push_back(_msisdn);

	// serviceCentreAddress
	_serviceCentreAddress = NULL;
	_serviceCentreAddress = new AddressString();
	children.push_back(_serviceCentreAddress);

	// imsi
	_imsi = NULL;
	children.push_back(_imsi);


}
asn1::AlertServiceCentreArg::~AlertServiceCentreArg(){

}

asn1::ASN1Node* asn1::AlertServiceCentreArg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_imsi = new IMSI();
			children[2] = _imsi;
			return _imsi;

		default: return NULL;
	}
}

void asn1::AlertServiceCentreArg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(2);
}

//InformServiceCentreArg
asn1::InformServiceCentreArg::InformServiceCentreArg(){
	strcpy(node_type_name, "InformServiceCentreArg");
	// storedMSISDN
	_storedMSISDN = NULL;
	children.push_back(_storedMSISDN);

	// mw_Status
	_mw_Status = NULL;
	children.push_back(_mw_Status);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// absentSubscriberDiagnosticSM
	_absentSubscriberDiagnosticSM = NULL;
	children.push_back(_absentSubscriberDiagnosticSM);

	// additionalAbsentSubscriberDiagnosticSM
	_additionalAbsentSubscriberDiagnosticSM = NULL;
	children.push_back(_additionalAbsentSubscriberDiagnosticSM);


}
asn1::InformServiceCentreArg::~InformServiceCentreArg(){

}

asn1::ASN1Node* asn1::InformServiceCentreArg::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_storedMSISDN = new ISDN_AddressString();
			children[0] = _storedMSISDN;
			return _storedMSISDN;
		case 1:
			_mw_Status = new MW_Status();
			children[1] = _mw_Status;
			return _mw_Status;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_absentSubscriberDiagnosticSM = new AbsentSubscriberDiagnosticSM();
			children[3] = _absentSubscriberDiagnosticSM;
			return _absentSubscriberDiagnosticSM;
		case 4:
			_additionalAbsentSubscriberDiagnosticSM = new AbsentSubscriberDiagnosticSM();
			_additionalAbsentSubscriberDiagnosticSM->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalAbsentSubscriberDiagnosticSM->tlv->tag_value = 0;
			children[4] = _additionalAbsentSubscriberDiagnosticSM;
			return _additionalAbsentSubscriberDiagnosticSM;

		default: return NULL;
	}
}

void asn1::InformServiceCentreArg::set_storedMSISDN(){
	if(_storedMSISDN == NULL) _storedMSISDN = (ISDN_AddressString*)create_node(0);
}

void asn1::InformServiceCentreArg::set_mw_Status(){
	if(_mw_Status == NULL) _mw_Status = (MW_Status*)create_node(1);
}

void asn1::InformServiceCentreArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::InformServiceCentreArg::set_absentSubscriberDiagnosticSM(){
	if(_absentSubscriberDiagnosticSM == NULL) _absentSubscriberDiagnosticSM = (AbsentSubscriberDiagnosticSM*)create_node(3);
}

void asn1::InformServiceCentreArg::set_additionalAbsentSubscriberDiagnosticSM(){
	if(_additionalAbsentSubscriberDiagnosticSM == NULL) _additionalAbsentSubscriberDiagnosticSM = (AbsentSubscriberDiagnosticSM*)create_node(4);
}

//MW_Status
asn1::MW_Status::MW_Status(){
	strcpy(node_type_name, "MW_Status");

}
asn1::MW_Status::~MW_Status(){

}

//ReadyForSM_Arg
asn1::ReadyForSM_Arg::ReadyForSM_Arg(){
	strcpy(node_type_name, "ReadyForSM_Arg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// alertReason
	_alertReason = NULL;
	_alertReason = new AlertReason();
	children.push_back(_alertReason);

	// alertReasonIndicator
	_alertReasonIndicator = NULL;
	children.push_back(_alertReasonIndicator);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// additionalAlertReasonIndicator
	_additionalAlertReasonIndicator = NULL;
	children.push_back(_additionalAlertReasonIndicator);


}
asn1::ReadyForSM_Arg::~ReadyForSM_Arg(){

}

asn1::ASN1Node* asn1::ReadyForSM_Arg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_alertReasonIndicator = new Null();
			children[2] = _alertReasonIndicator;
			return _alertReasonIndicator;
		case 3:
			_extensionContainer = new ExtensionContainer();
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_additionalAlertReasonIndicator = new Null();
			_additionalAlertReasonIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalAlertReasonIndicator->tlv->tag_value = 1;
			children[4] = _additionalAlertReasonIndicator;
			return _additionalAlertReasonIndicator;

		default: return NULL;
	}
}

void asn1::ReadyForSM_Arg::set_alertReasonIndicator(){
	if(_alertReasonIndicator == NULL) _alertReasonIndicator = (Null*)create_node(2);
}

void asn1::ReadyForSM_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::ReadyForSM_Arg::set_additionalAlertReasonIndicator(){
	if(_additionalAlertReasonIndicator == NULL) _additionalAlertReasonIndicator = (Null*)create_node(4);
}

//ReadyForSM_Res
asn1::ReadyForSM_Res::ReadyForSM_Res(){
	strcpy(node_type_name, "ReadyForSM_Res");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ReadyForSM_Res::~ReadyForSM_Res(){

}

asn1::ASN1Node* asn1::ReadyForSM_Res::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ReadyForSM_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//AlertReason
asn1::AlertReason::AlertReason(){
	strcpy(node_type_name, "AlertReason");

}
asn1::AlertReason::~AlertReason(){

}

//MT_ForwardSM_VGCS_Arg
asn1::MT_ForwardSM_VGCS_Arg::MT_ForwardSM_VGCS_Arg(){
	strcpy(node_type_name, "MT_ForwardSM_VGCS_Arg");
	// asciCallReference
	_asciCallReference = NULL;
	_asciCallReference = new ASCI_CallReference();
	children.push_back(_asciCallReference);

	// sm_RP_OA
	_sm_RP_OA = NULL;
	_sm_RP_OA = new SM_RP_OA();
	children.push_back(_sm_RP_OA);

	// sm_RP_UI
	_sm_RP_UI = NULL;
	_sm_RP_UI = new SignalInfo();
	children.push_back(_sm_RP_UI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MT_ForwardSM_VGCS_Arg::~MT_ForwardSM_VGCS_Arg(){

}

asn1::ASN1Node* asn1::MT_ForwardSM_VGCS_Arg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_extensionContainer = new ExtensionContainer();
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::MT_ForwardSM_VGCS_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//MT_ForwardSM_VGCS_Res
asn1::MT_ForwardSM_VGCS_Res::MT_ForwardSM_VGCS_Res(){
	strcpy(node_type_name, "MT_ForwardSM_VGCS_Res");
	// sm_RP_UI
	_sm_RP_UI = NULL;
	children.push_back(_sm_RP_UI);

	// dispatcherList
	_dispatcherList = NULL;
	children.push_back(_dispatcherList);

	// ongoingCall
	_ongoingCall = NULL;
	children.push_back(_ongoingCall);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// additionalDispatcherList
	_additionalDispatcherList = NULL;
	children.push_back(_additionalDispatcherList);


}
asn1::MT_ForwardSM_VGCS_Res::~MT_ForwardSM_VGCS_Res(){

}

asn1::ASN1Node* asn1::MT_ForwardSM_VGCS_Res::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_sm_RP_UI = new SignalInfo();
			_sm_RP_UI->tlv->tag_class = CONTEXT_SPECIFIC;
			_sm_RP_UI->tlv->tag_value = 0;
			children[0] = _sm_RP_UI;
			return _sm_RP_UI;
		case 1:
			_dispatcherList = new DispatcherList();
			_dispatcherList->tlv->tag_class = CONTEXT_SPECIFIC;
			_dispatcherList->tlv->tag_value = 1;
			children[1] = _dispatcherList;
			return _dispatcherList;
		case 2:
			_ongoingCall = new Null();
			children[2] = _ongoingCall;
			return _ongoingCall;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_additionalDispatcherList = new AdditionalDispatcherList();
			_additionalDispatcherList->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalDispatcherList->tlv->tag_value = 3;
			children[4] = _additionalDispatcherList;
			return _additionalDispatcherList;

		default: return NULL;
	}
}

void asn1::MT_ForwardSM_VGCS_Res::set_sm_RP_UI(){
	if(_sm_RP_UI == NULL) _sm_RP_UI = (SignalInfo*)create_node(0);
}

void asn1::MT_ForwardSM_VGCS_Res::set_dispatcherList(){
	if(_dispatcherList == NULL) _dispatcherList = (DispatcherList*)create_node(1);
}

void asn1::MT_ForwardSM_VGCS_Res::set_ongoingCall(){
	if(_ongoingCall == NULL) _ongoingCall = (Null*)create_node(2);
}

void asn1::MT_ForwardSM_VGCS_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::MT_ForwardSM_VGCS_Res::set_additionalDispatcherList(){
	if(_additionalDispatcherList == NULL) _additionalDispatcherList = (AdditionalDispatcherList*)create_node(4);
}

//DispatcherList
asn1::DispatcherList::DispatcherList(){
	strcpy(node_type_name, "DispatcherList");

}
asn1::DispatcherList::~DispatcherList(){
}

asn1::ISDN_AddressString* asn1::DispatcherList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ISDN_AddressString*)children[child_index]; else return NULL;
}

void asn1::DispatcherList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::DispatcherList::create_node(unsigned int _index){
	children[_index] = new ISDN_AddressString();
	return children[_index];
}

asn1::ASN1Node* asn1::DispatcherList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ISDN_AddressString());
		return children[children.size() - 1];
	}

}

//maxNumOfDispatchers
asn1::maxNumOfDispatchers::maxNumOfDispatchers(){
	strcpy(node_type_name, "maxNumOfDispatchers");

}
asn1::maxNumOfDispatchers::~maxNumOfDispatchers(){

}

//AdditionalDispatcherList
asn1::AdditionalDispatcherList::AdditionalDispatcherList(){
	strcpy(node_type_name, "AdditionalDispatcherList");

}
asn1::AdditionalDispatcherList::~AdditionalDispatcherList(){
}

asn1::ISDN_AddressString* asn1::AdditionalDispatcherList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ISDN_AddressString*)children[child_index]; else return NULL;
}

void asn1::AdditionalDispatcherList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::AdditionalDispatcherList::create_node(unsigned int _index){
	children[_index] = new ISDN_AddressString();
	return children[_index];
}

asn1::ASN1Node* asn1::AdditionalDispatcherList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ISDN_AddressString());
		return children[children.size() - 1];
	}

}

//maxNumOfAdditionalDispatchers
asn1::maxNumOfAdditionalDispatchers::maxNumOfAdditionalDispatchers(){
	strcpy(node_type_name, "maxNumOfAdditionalDispatchers");

}
asn1::maxNumOfAdditionalDispatchers::~maxNumOfAdditionalDispatchers(){

}

