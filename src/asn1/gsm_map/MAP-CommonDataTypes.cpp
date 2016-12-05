#include<MAP-CommonDataTypes.h>
#include<iostream>

//TBCD_STRING
asn1::TBCD_STRING::TBCD_STRING(){
	strcpy(node_type_name, "TBCD_STRING");

}
asn1::TBCD_STRING::~TBCD_STRING(){

}

//DiameterIdentity
asn1::DiameterIdentity::DiameterIdentity(){
	strcpy(node_type_name, "DiameterIdentity");

}
asn1::DiameterIdentity::~DiameterIdentity(){

}

//AddressString
asn1::AddressString::AddressString(){
	strcpy(node_type_name, "AddressString");

}
asn1::AddressString::~AddressString(){

}

//maxAddressLength
asn1::maxAddressLength::maxAddressLength(){
	strcpy(node_type_name, "maxAddressLength");

}
asn1::maxAddressLength::~maxAddressLength(){

}

//ISDN_AddressString
asn1::ISDN_AddressString::ISDN_AddressString(){
	strcpy(node_type_name, "ISDN_AddressString");

}
asn1::ISDN_AddressString::~ISDN_AddressString(){

}

//maxISDN_AddressLength
asn1::maxISDN_AddressLength::maxISDN_AddressLength(){
	strcpy(node_type_name, "maxISDN_AddressLength");

}
asn1::maxISDN_AddressLength::~maxISDN_AddressLength(){

}

//FTN_AddressString
asn1::FTN_AddressString::FTN_AddressString(){
	strcpy(node_type_name, "FTN_AddressString");

}
asn1::FTN_AddressString::~FTN_AddressString(){

}

//maxFTN_AddressLength
asn1::maxFTN_AddressLength::maxFTN_AddressLength(){
	strcpy(node_type_name, "maxFTN_AddressLength");

}
asn1::maxFTN_AddressLength::~maxFTN_AddressLength(){

}

//ISDN_SubaddressString
asn1::ISDN_SubaddressString::ISDN_SubaddressString(){
	strcpy(node_type_name, "ISDN_SubaddressString");

}
asn1::ISDN_SubaddressString::~ISDN_SubaddressString(){

}

//maxISDN_SubaddressLength
asn1::maxISDN_SubaddressLength::maxISDN_SubaddressLength(){
	strcpy(node_type_name, "maxISDN_SubaddressLength");

}
asn1::maxISDN_SubaddressLength::~maxISDN_SubaddressLength(){

}

//ExternalSignalInfo
asn1::ExternalSignalInfo::ExternalSignalInfo(){
	strcpy(node_type_name, "ExternalSignalInfo");
	// protocolId
	_protocolId = NULL;
	_protocolId = new ProtocolId();
	children.push_back(_protocolId);

	// signalInfo
	_signalInfo = NULL;
	_signalInfo = new SignalInfo();
	children.push_back(_signalInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ExternalSignalInfo::~ExternalSignalInfo(){

}

asn1::ASN1Node* asn1::ExternalSignalInfo::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ExternalSignalInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//SignalInfo
asn1::SignalInfo::SignalInfo(){
	strcpy(node_type_name, "SignalInfo");

}
asn1::SignalInfo::~SignalInfo(){

}

//maxSignalInfoLength
asn1::maxSignalInfoLength::maxSignalInfoLength(){
	strcpy(node_type_name, "maxSignalInfoLength");

}
asn1::maxSignalInfoLength::~maxSignalInfoLength(){

}

//ProtocolId
asn1::ProtocolId::ProtocolId(){
	strcpy(node_type_name, "ProtocolId");

}
asn1::ProtocolId::~ProtocolId(){

}

//Ext_ExternalSignalInfo
asn1::Ext_ExternalSignalInfo::Ext_ExternalSignalInfo(){
	strcpy(node_type_name, "Ext_ExternalSignalInfo");
	// ext_ProtocolId
	_ext_ProtocolId = NULL;
	_ext_ProtocolId = new Ext_ProtocolId();
	children.push_back(_ext_ProtocolId);

	// signalInfo
	_signalInfo = NULL;
	_signalInfo = new SignalInfo();
	children.push_back(_signalInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::Ext_ExternalSignalInfo::~Ext_ExternalSignalInfo(){

}

asn1::ASN1Node* asn1::Ext_ExternalSignalInfo::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::Ext_ExternalSignalInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//Ext_ProtocolId
asn1::Ext_ProtocolId::Ext_ProtocolId(){
	strcpy(node_type_name, "Ext_ProtocolId");

}
asn1::Ext_ProtocolId::~Ext_ProtocolId(){

}

//AccessNetworkSignalInfo
asn1::AccessNetworkSignalInfo::AccessNetworkSignalInfo(){
	strcpy(node_type_name, "AccessNetworkSignalInfo");
	// accessNetworkProtocolId
	_accessNetworkProtocolId = NULL;
	_accessNetworkProtocolId = new AccessNetworkProtocolId();
	children.push_back(_accessNetworkProtocolId);

	// signalInfo
	_signalInfo = NULL;
	_signalInfo = new LongSignalInfo();
	children.push_back(_signalInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::AccessNetworkSignalInfo::~AccessNetworkSignalInfo(){

}

asn1::ASN1Node* asn1::AccessNetworkSignalInfo::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::AccessNetworkSignalInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//LongSignalInfo
asn1::LongSignalInfo::LongSignalInfo(){
	strcpy(node_type_name, "LongSignalInfo");

}
asn1::LongSignalInfo::~LongSignalInfo(){

}

//maxLongSignalInfoLength
asn1::maxLongSignalInfoLength::maxLongSignalInfoLength(){
	strcpy(node_type_name, "maxLongSignalInfoLength");

}
asn1::maxLongSignalInfoLength::~maxLongSignalInfoLength(){

}

//AccessNetworkProtocolId
asn1::AccessNetworkProtocolId::AccessNetworkProtocolId(){
	strcpy(node_type_name, "AccessNetworkProtocolId");

}
asn1::AccessNetworkProtocolId::~AccessNetworkProtocolId(){

}

//AlertingPattern
asn1::AlertingPattern::AlertingPattern(){
	strcpy(node_type_name, "AlertingPattern");

}
asn1::AlertingPattern::~AlertingPattern(){

}

//alertingLevel_0
asn1::alertingLevel_0::alertingLevel_0(){
	strcpy(node_type_name, "alertingLevel_0");

}
asn1::alertingLevel_0::~alertingLevel_0(){

}

//alertingLevel_1
asn1::alertingLevel_1::alertingLevel_1(){
	strcpy(node_type_name, "alertingLevel_1");

}
asn1::alertingLevel_1::~alertingLevel_1(){

}

//alertingLevel_2
asn1::alertingLevel_2::alertingLevel_2(){
	strcpy(node_type_name, "alertingLevel_2");

}
asn1::alertingLevel_2::~alertingLevel_2(){

}

//alertingCategory_1
asn1::alertingCategory_1::alertingCategory_1(){
	strcpy(node_type_name, "alertingCategory_1");

}
asn1::alertingCategory_1::~alertingCategory_1(){

}

//alertingCategory_2
asn1::alertingCategory_2::alertingCategory_2(){
	strcpy(node_type_name, "alertingCategory_2");

}
asn1::alertingCategory_2::~alertingCategory_2(){

}

//alertingCategory_3
asn1::alertingCategory_3::alertingCategory_3(){
	strcpy(node_type_name, "alertingCategory_3");

}
asn1::alertingCategory_3::~alertingCategory_3(){

}

//alertingCategory_4
asn1::alertingCategory_4::alertingCategory_4(){
	strcpy(node_type_name, "alertingCategory_4");

}
asn1::alertingCategory_4::~alertingCategory_4(){

}

//alertingCategory_5
asn1::alertingCategory_5::alertingCategory_5(){
	strcpy(node_type_name, "alertingCategory_5");

}
asn1::alertingCategory_5::~alertingCategory_5(){

}

//GSN_Address
asn1::GSN_Address::GSN_Address(){
	strcpy(node_type_name, "GSN_Address");

}
asn1::GSN_Address::~GSN_Address(){

}

//Time
asn1::Time::Time(){
	strcpy(node_type_name, "Time");

}
asn1::Time::~Time(){

}

//IMSI
asn1::IMSI::IMSI(){
	strcpy(node_type_name, "IMSI");

}
asn1::IMSI::~IMSI(){

}

//Identity
asn1::Identity::Identity(){
	strcpy(node_type_name, "Identity");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// imsi_WithLMSI
	_imsi_WithLMSI = NULL;
	_imsi_WithLMSI = new IMSI_WithLMSI();
	children.push_back(_imsi_WithLMSI);


}
asn1::Identity::~Identity(){

}

//IMSI_WithLMSI
asn1::IMSI_WithLMSI::IMSI_WithLMSI(){
	strcpy(node_type_name, "IMSI_WithLMSI");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// lmsi
	_lmsi = NULL;
	_lmsi = new LMSI();
	children.push_back(_lmsi);


}
asn1::IMSI_WithLMSI::~IMSI_WithLMSI(){

}

//ASCI_CallReference
asn1::ASCI_CallReference::ASCI_CallReference(){
	strcpy(node_type_name, "ASCI_CallReference");

}
asn1::ASCI_CallReference::~ASCI_CallReference(){

}

//TMSI
asn1::TMSI::TMSI(){
	strcpy(node_type_name, "TMSI");

}
asn1::TMSI::~TMSI(){

}

//SubscriberId
asn1::SubscriberId::SubscriberId(){
	strcpy(node_type_name, "SubscriberId");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// tmsi
	_tmsi = NULL;
	_tmsi = new TMSI();
	_tmsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_tmsi->tlv->tag_value = 1;
	children.push_back(_tmsi);


}
asn1::SubscriberId::~SubscriberId(){

}

//IMEI
asn1::IMEI::IMEI(){
	strcpy(node_type_name, "IMEI");

}
asn1::IMEI::~IMEI(){

}

//HLR_Id
asn1::HLR_Id::HLR_Id(){
	strcpy(node_type_name, "HLR_Id");

}
asn1::HLR_Id::~HLR_Id(){

}

//HLR_List
asn1::HLR_List::HLR_List(){
	strcpy(node_type_name, "HLR_List");

}
asn1::HLR_List::~HLR_List(){
}

asn1::HLR_Id* asn1::HLR_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (HLR_Id*)children[child_index]; else return NULL;
}

void asn1::HLR_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::HLR_List::create_node(unsigned int _index){
	children[_index] = new HLR_Id();
	return children[_index];
}

asn1::ASN1Node* asn1::HLR_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new HLR_Id());
		return children[children.size() - 1];
	}

}

//maxNumOfHLR_Id
asn1::maxNumOfHLR_Id::maxNumOfHLR_Id(){
	strcpy(node_type_name, "maxNumOfHLR_Id");

}
asn1::maxNumOfHLR_Id::~maxNumOfHLR_Id(){

}

//LMSI
asn1::LMSI::LMSI(){
	strcpy(node_type_name, "LMSI");

}
asn1::LMSI::~LMSI(){

}

//GlobalCellId
asn1::GlobalCellId::GlobalCellId(){
	strcpy(node_type_name, "GlobalCellId");

}
asn1::GlobalCellId::~GlobalCellId(){

}

//NetworkResource
asn1::NetworkResource::NetworkResource(){
	strcpy(node_type_name, "NetworkResource");

}
asn1::NetworkResource::~NetworkResource(){

}

//AdditionalNetworkResource
asn1::AdditionalNetworkResource::AdditionalNetworkResource(){
	strcpy(node_type_name, "AdditionalNetworkResource");

}
asn1::AdditionalNetworkResource::~AdditionalNetworkResource(){

}

//NAEA_PreferredCI
asn1::NAEA_PreferredCI::NAEA_PreferredCI(){
	strcpy(node_type_name, "NAEA_PreferredCI");
	// naea_PreferredCIC
	_naea_PreferredCIC = NULL;
	_naea_PreferredCIC = new NAEA_CIC();
	_naea_PreferredCIC->tlv->tag_class = CONTEXT_SPECIFIC;
	_naea_PreferredCIC->tlv->tag_value = 0;
	children.push_back(_naea_PreferredCIC);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::NAEA_PreferredCI::~NAEA_PreferredCI(){

}

asn1::ASN1Node* asn1::NAEA_PreferredCI::create_node(unsigned int _index){
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

void asn1::NAEA_PreferredCI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//NAEA_CIC
asn1::NAEA_CIC::NAEA_CIC(){
	strcpy(node_type_name, "NAEA_CIC");

}
asn1::NAEA_CIC::~NAEA_CIC(){

}

//SubscriberIdentity
asn1::SubscriberIdentity::SubscriberIdentity(){
	strcpy(node_type_name, "SubscriberIdentity");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// msisdn
	_msisdn = NULL;
	_msisdn = new ISDN_AddressString();
	_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
	_msisdn->tlv->tag_value = 1;
	children.push_back(_msisdn);


}
asn1::SubscriberIdentity::~SubscriberIdentity(){

}

//LCSClientExternalID
asn1::LCSClientExternalID::LCSClientExternalID(){
	strcpy(node_type_name, "LCSClientExternalID");
	// externalAddress
	_externalAddress = NULL;
	children.push_back(_externalAddress);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::LCSClientExternalID::~LCSClientExternalID(){

}

asn1::ASN1Node* asn1::LCSClientExternalID::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_externalAddress = new ISDN_AddressString();
			_externalAddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_externalAddress->tlv->tag_value = 0;
			children[0] = _externalAddress;
			return _externalAddress;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::LCSClientExternalID::set_externalAddress(){
	if(_externalAddress == NULL) _externalAddress = (ISDN_AddressString*)create_node(0);
}

void asn1::LCSClientExternalID::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//LCSClientInternalID
asn1::LCSClientInternalID::LCSClientInternalID(){
	strcpy(node_type_name, "LCSClientInternalID");

}
asn1::LCSClientInternalID::~LCSClientInternalID(){

}

//LCSServiceTypeID
asn1::LCSServiceTypeID::LCSServiceTypeID(){
	strcpy(node_type_name, "LCSServiceTypeID");

}
asn1::LCSServiceTypeID::~LCSServiceTypeID(){

}

//emergencyServices
asn1::emergencyServices::emergencyServices(){
	strcpy(node_type_name, "emergencyServices");

}
asn1::emergencyServices::~emergencyServices(){

}

//emergencyAlertServices
asn1::emergencyAlertServices::emergencyAlertServices(){
	strcpy(node_type_name, "emergencyAlertServices");

}
asn1::emergencyAlertServices::~emergencyAlertServices(){

}

//personTracking
asn1::personTracking::personTracking(){
	strcpy(node_type_name, "personTracking");

}
asn1::personTracking::~personTracking(){

}

//fleetManagement
asn1::fleetManagement::fleetManagement(){
	strcpy(node_type_name, "fleetManagement");

}
asn1::fleetManagement::~fleetManagement(){

}

//assetManagement
asn1::assetManagement::assetManagement(){
	strcpy(node_type_name, "assetManagement");

}
asn1::assetManagement::~assetManagement(){

}

//trafficCongestionReporting
asn1::trafficCongestionReporting::trafficCongestionReporting(){
	strcpy(node_type_name, "trafficCongestionReporting");

}
asn1::trafficCongestionReporting::~trafficCongestionReporting(){

}

//roadsideAssistance
asn1::roadsideAssistance::roadsideAssistance(){
	strcpy(node_type_name, "roadsideAssistance");

}
asn1::roadsideAssistance::~roadsideAssistance(){

}

//routingToNearestCommercialEnterprise
asn1::routingToNearestCommercialEnterprise::routingToNearestCommercialEnterprise(){
	strcpy(node_type_name, "routingToNearestCommercialEnterprise");

}
asn1::routingToNearestCommercialEnterprise::~routingToNearestCommercialEnterprise(){

}

//navigation
asn1::navigation::navigation(){
	strcpy(node_type_name, "navigation");

}
asn1::navigation::~navigation(){

}

//citySightseeing
asn1::citySightseeing::citySightseeing(){
	strcpy(node_type_name, "citySightseeing");

}
asn1::citySightseeing::~citySightseeing(){

}

//localizedAdvertising
asn1::localizedAdvertising::localizedAdvertising(){
	strcpy(node_type_name, "localizedAdvertising");

}
asn1::localizedAdvertising::~localizedAdvertising(){

}

//mobileYellowPages
asn1::mobileYellowPages::mobileYellowPages(){
	strcpy(node_type_name, "mobileYellowPages");

}
asn1::mobileYellowPages::~mobileYellowPages(){

}

//trafficAndPublicTransportationInfo
asn1::trafficAndPublicTransportationInfo::trafficAndPublicTransportationInfo(){
	strcpy(node_type_name, "trafficAndPublicTransportationInfo");

}
asn1::trafficAndPublicTransportationInfo::~trafficAndPublicTransportationInfo(){

}

//weather
asn1::weather::weather(){
	strcpy(node_type_name, "weather");

}
asn1::weather::~weather(){

}

//assetAndServiceFinding
asn1::assetAndServiceFinding::assetAndServiceFinding(){
	strcpy(node_type_name, "assetAndServiceFinding");

}
asn1::assetAndServiceFinding::~assetAndServiceFinding(){

}

//gaming
asn1::gaming::gaming(){
	strcpy(node_type_name, "gaming");

}
asn1::gaming::~gaming(){

}

//findYourFriend
asn1::findYourFriend::findYourFriend(){
	strcpy(node_type_name, "findYourFriend");

}
asn1::findYourFriend::~findYourFriend(){

}

//dating
asn1::dating::dating(){
	strcpy(node_type_name, "dating");

}
asn1::dating::~dating(){

}

//chatting
asn1::chatting::chatting(){
	strcpy(node_type_name, "chatting");

}
asn1::chatting::~chatting(){

}

//routeFinding
asn1::routeFinding::routeFinding(){
	strcpy(node_type_name, "routeFinding");

}
asn1::routeFinding::~routeFinding(){

}

//whereAmI
asn1::whereAmI::whereAmI(){
	strcpy(node_type_name, "whereAmI");

}
asn1::whereAmI::~whereAmI(){

}

//serv64
asn1::serv64::serv64(){
	strcpy(node_type_name, "serv64");

}
asn1::serv64::~serv64(){

}

//serv65
asn1::serv65::serv65(){
	strcpy(node_type_name, "serv65");

}
asn1::serv65::~serv65(){

}

//serv66
asn1::serv66::serv66(){
	strcpy(node_type_name, "serv66");

}
asn1::serv66::~serv66(){

}

//serv67
asn1::serv67::serv67(){
	strcpy(node_type_name, "serv67");

}
asn1::serv67::~serv67(){

}

//serv68
asn1::serv68::serv68(){
	strcpy(node_type_name, "serv68");

}
asn1::serv68::~serv68(){

}

//serv69
asn1::serv69::serv69(){
	strcpy(node_type_name, "serv69");

}
asn1::serv69::~serv69(){

}

//serv70
asn1::serv70::serv70(){
	strcpy(node_type_name, "serv70");

}
asn1::serv70::~serv70(){

}

//serv71
asn1::serv71::serv71(){
	strcpy(node_type_name, "serv71");

}
asn1::serv71::~serv71(){

}

//serv72
asn1::serv72::serv72(){
	strcpy(node_type_name, "serv72");

}
asn1::serv72::~serv72(){

}

//serv73
asn1::serv73::serv73(){
	strcpy(node_type_name, "serv73");

}
asn1::serv73::~serv73(){

}

//serv74
asn1::serv74::serv74(){
	strcpy(node_type_name, "serv74");

}
asn1::serv74::~serv74(){

}

//serv75
asn1::serv75::serv75(){
	strcpy(node_type_name, "serv75");

}
asn1::serv75::~serv75(){

}

//serv76
asn1::serv76::serv76(){
	strcpy(node_type_name, "serv76");

}
asn1::serv76::~serv76(){

}

//serv77
asn1::serv77::serv77(){
	strcpy(node_type_name, "serv77");

}
asn1::serv77::~serv77(){

}

//serv78
asn1::serv78::serv78(){
	strcpy(node_type_name, "serv78");

}
asn1::serv78::~serv78(){

}

//serv79
asn1::serv79::serv79(){
	strcpy(node_type_name, "serv79");

}
asn1::serv79::~serv79(){

}

//serv80
asn1::serv80::serv80(){
	strcpy(node_type_name, "serv80");

}
asn1::serv80::~serv80(){

}

//serv81
asn1::serv81::serv81(){
	strcpy(node_type_name, "serv81");

}
asn1::serv81::~serv81(){

}

//serv82
asn1::serv82::serv82(){
	strcpy(node_type_name, "serv82");

}
asn1::serv82::~serv82(){

}

//serv83
asn1::serv83::serv83(){
	strcpy(node_type_name, "serv83");

}
asn1::serv83::~serv83(){

}

//serv84
asn1::serv84::serv84(){
	strcpy(node_type_name, "serv84");

}
asn1::serv84::~serv84(){

}

//serv85
asn1::serv85::serv85(){
	strcpy(node_type_name, "serv85");

}
asn1::serv85::~serv85(){

}

//serv86
asn1::serv86::serv86(){
	strcpy(node_type_name, "serv86");

}
asn1::serv86::~serv86(){

}

//serv87
asn1::serv87::serv87(){
	strcpy(node_type_name, "serv87");

}
asn1::serv87::~serv87(){

}

//serv88
asn1::serv88::serv88(){
	strcpy(node_type_name, "serv88");

}
asn1::serv88::~serv88(){

}

//serv89
asn1::serv89::serv89(){
	strcpy(node_type_name, "serv89");

}
asn1::serv89::~serv89(){

}

//serv90
asn1::serv90::serv90(){
	strcpy(node_type_name, "serv90");

}
asn1::serv90::~serv90(){

}

//serv91
asn1::serv91::serv91(){
	strcpy(node_type_name, "serv91");

}
asn1::serv91::~serv91(){

}

//serv92
asn1::serv92::serv92(){
	strcpy(node_type_name, "serv92");

}
asn1::serv92::~serv92(){

}

//serv93
asn1::serv93::serv93(){
	strcpy(node_type_name, "serv93");

}
asn1::serv93::~serv93(){

}

//serv94
asn1::serv94::serv94(){
	strcpy(node_type_name, "serv94");

}
asn1::serv94::~serv94(){

}

//serv95
asn1::serv95::serv95(){
	strcpy(node_type_name, "serv95");

}
asn1::serv95::~serv95(){

}

//serv96
asn1::serv96::serv96(){
	strcpy(node_type_name, "serv96");

}
asn1::serv96::~serv96(){

}

//serv97
asn1::serv97::serv97(){
	strcpy(node_type_name, "serv97");

}
asn1::serv97::~serv97(){

}

//serv98
asn1::serv98::serv98(){
	strcpy(node_type_name, "serv98");

}
asn1::serv98::~serv98(){

}

//serv99
asn1::serv99::serv99(){
	strcpy(node_type_name, "serv99");

}
asn1::serv99::~serv99(){

}

//serv100
asn1::serv100::serv100(){
	strcpy(node_type_name, "serv100");

}
asn1::serv100::~serv100(){

}

//serv101
asn1::serv101::serv101(){
	strcpy(node_type_name, "serv101");

}
asn1::serv101::~serv101(){

}

//serv102
asn1::serv102::serv102(){
	strcpy(node_type_name, "serv102");

}
asn1::serv102::~serv102(){

}

//serv103
asn1::serv103::serv103(){
	strcpy(node_type_name, "serv103");

}
asn1::serv103::~serv103(){

}

//serv104
asn1::serv104::serv104(){
	strcpy(node_type_name, "serv104");

}
asn1::serv104::~serv104(){

}

//serv105
asn1::serv105::serv105(){
	strcpy(node_type_name, "serv105");

}
asn1::serv105::~serv105(){

}

//serv106
asn1::serv106::serv106(){
	strcpy(node_type_name, "serv106");

}
asn1::serv106::~serv106(){

}

//serv107
asn1::serv107::serv107(){
	strcpy(node_type_name, "serv107");

}
asn1::serv107::~serv107(){

}

//serv108
asn1::serv108::serv108(){
	strcpy(node_type_name, "serv108");

}
asn1::serv108::~serv108(){

}

//serv109
asn1::serv109::serv109(){
	strcpy(node_type_name, "serv109");

}
asn1::serv109::~serv109(){

}

//serv110
asn1::serv110::serv110(){
	strcpy(node_type_name, "serv110");

}
asn1::serv110::~serv110(){

}

//serv111
asn1::serv111::serv111(){
	strcpy(node_type_name, "serv111");

}
asn1::serv111::~serv111(){

}

//serv112
asn1::serv112::serv112(){
	strcpy(node_type_name, "serv112");

}
asn1::serv112::~serv112(){

}

//serv113
asn1::serv113::serv113(){
	strcpy(node_type_name, "serv113");

}
asn1::serv113::~serv113(){

}

//serv114
asn1::serv114::serv114(){
	strcpy(node_type_name, "serv114");

}
asn1::serv114::~serv114(){

}

//serv115
asn1::serv115::serv115(){
	strcpy(node_type_name, "serv115");

}
asn1::serv115::~serv115(){

}

//serv116
asn1::serv116::serv116(){
	strcpy(node_type_name, "serv116");

}
asn1::serv116::~serv116(){

}

//serv117
asn1::serv117::serv117(){
	strcpy(node_type_name, "serv117");

}
asn1::serv117::~serv117(){

}

//serv118
asn1::serv118::serv118(){
	strcpy(node_type_name, "serv118");

}
asn1::serv118::~serv118(){

}

//serv119
asn1::serv119::serv119(){
	strcpy(node_type_name, "serv119");

}
asn1::serv119::~serv119(){

}

//serv120
asn1::serv120::serv120(){
	strcpy(node_type_name, "serv120");

}
asn1::serv120::~serv120(){

}

//serv121
asn1::serv121::serv121(){
	strcpy(node_type_name, "serv121");

}
asn1::serv121::~serv121(){

}

//serv122
asn1::serv122::serv122(){
	strcpy(node_type_name, "serv122");

}
asn1::serv122::~serv122(){

}

//serv123
asn1::serv123::serv123(){
	strcpy(node_type_name, "serv123");

}
asn1::serv123::~serv123(){

}

//serv124
asn1::serv124::serv124(){
	strcpy(node_type_name, "serv124");

}
asn1::serv124::~serv124(){

}

//serv125
asn1::serv125::serv125(){
	strcpy(node_type_name, "serv125");

}
asn1::serv125::~serv125(){

}

//serv126
asn1::serv126::serv126(){
	strcpy(node_type_name, "serv126");

}
asn1::serv126::~serv126(){

}

//serv127
asn1::serv127::serv127(){
	strcpy(node_type_name, "serv127");

}
asn1::serv127::~serv127(){

}

//PLMN_Id
asn1::PLMN_Id::PLMN_Id(){
	strcpy(node_type_name, "PLMN_Id");

}
asn1::PLMN_Id::~PLMN_Id(){

}

//E_UTRAN_CGI
asn1::E_UTRAN_CGI::E_UTRAN_CGI(){
	strcpy(node_type_name, "E_UTRAN_CGI");

}
asn1::E_UTRAN_CGI::~E_UTRAN_CGI(){

}

//TA_Id
asn1::TA_Id::TA_Id(){
	strcpy(node_type_name, "TA_Id");

}
asn1::TA_Id::~TA_Id(){

}

//RAIdentity
asn1::RAIdentity::RAIdentity(){
	strcpy(node_type_name, "RAIdentity");

}
asn1::RAIdentity::~RAIdentity(){

}

//NetworkNodeDiameterAddress
asn1::NetworkNodeDiameterAddress::NetworkNodeDiameterAddress(){
	strcpy(node_type_name, "NetworkNodeDiameterAddress");
	// diameter_Name
	_diameter_Name = NULL;
	_diameter_Name = new DiameterIdentity();
	_diameter_Name->tlv->tag_class = CONTEXT_SPECIFIC;
	_diameter_Name->tlv->tag_value = 0;
	children.push_back(_diameter_Name);

	// diameter_Realm
	_diameter_Realm = NULL;
	_diameter_Realm = new DiameterIdentity();
	_diameter_Realm->tlv->tag_class = CONTEXT_SPECIFIC;
	_diameter_Realm->tlv->tag_value = 1;
	children.push_back(_diameter_Realm);


}
asn1::NetworkNodeDiameterAddress::~NetworkNodeDiameterAddress(){

}

//CellGlobalIdOrServiceAreaIdOrLAI
asn1::CellGlobalIdOrServiceAreaIdOrLAI::CellGlobalIdOrServiceAreaIdOrLAI(){
	strcpy(node_type_name, "CellGlobalIdOrServiceAreaIdOrLAI");
	// cellGlobalIdOrServiceAreaIdFixedLength
	_cellGlobalIdOrServiceAreaIdFixedLength = NULL;
	_cellGlobalIdOrServiceAreaIdFixedLength = new CellGlobalIdOrServiceAreaIdFixedLength();
	_cellGlobalIdOrServiceAreaIdFixedLength->tlv->tag_class = CONTEXT_SPECIFIC;
	_cellGlobalIdOrServiceAreaIdFixedLength->tlv->tag_value = 0;
	children.push_back(_cellGlobalIdOrServiceAreaIdFixedLength);

	// laiFixedLength
	_laiFixedLength = NULL;
	_laiFixedLength = new LAIFixedLength();
	_laiFixedLength->tlv->tag_class = CONTEXT_SPECIFIC;
	_laiFixedLength->tlv->tag_value = 1;
	children.push_back(_laiFixedLength);


}
asn1::CellGlobalIdOrServiceAreaIdOrLAI::~CellGlobalIdOrServiceAreaIdOrLAI(){

}

//CellGlobalIdOrServiceAreaIdFixedLength
asn1::CellGlobalIdOrServiceAreaIdFixedLength::CellGlobalIdOrServiceAreaIdFixedLength(){
	strcpy(node_type_name, "CellGlobalIdOrServiceAreaIdFixedLength");

}
asn1::CellGlobalIdOrServiceAreaIdFixedLength::~CellGlobalIdOrServiceAreaIdFixedLength(){

}

//LAIFixedLength
asn1::LAIFixedLength::LAIFixedLength(){
	strcpy(node_type_name, "LAIFixedLength");

}
asn1::LAIFixedLength::~LAIFixedLength(){

}

//BasicServiceCode
asn1::BasicServiceCode::BasicServiceCode(){
	strcpy(node_type_name, "BasicServiceCode");
	// bearerService
	_bearerService = NULL;
	_bearerService = new BearerServiceCode();
	_bearerService->tlv->tag_class = CONTEXT_SPECIFIC;
	_bearerService->tlv->tag_value = 2;
	children.push_back(_bearerService);

	// teleservice
	_teleservice = NULL;
	_teleservice = new TeleserviceCode();
	_teleservice->tlv->tag_class = CONTEXT_SPECIFIC;
	_teleservice->tlv->tag_value = 3;
	children.push_back(_teleservice);


}
asn1::BasicServiceCode::~BasicServiceCode(){

}

//Ext_BasicServiceCode
asn1::Ext_BasicServiceCode::Ext_BasicServiceCode(){
	strcpy(node_type_name, "Ext_BasicServiceCode");
	// ext_BearerService
	_ext_BearerService = NULL;
	_ext_BearerService = new Ext_BearerServiceCode();
	_ext_BearerService->tlv->tag_class = CONTEXT_SPECIFIC;
	_ext_BearerService->tlv->tag_value = 2;
	children.push_back(_ext_BearerService);

	// ext_Teleservice
	_ext_Teleservice = NULL;
	_ext_Teleservice = new Ext_TeleserviceCode();
	_ext_Teleservice->tlv->tag_class = CONTEXT_SPECIFIC;
	_ext_Teleservice->tlv->tag_value = 3;
	children.push_back(_ext_Teleservice);


}
asn1::Ext_BasicServiceCode::~Ext_BasicServiceCode(){

}

//EMLPP_Info
asn1::EMLPP_Info::EMLPP_Info(){
	strcpy(node_type_name, "EMLPP_Info");
	// maximumentitledPriority
	_maximumentitledPriority = NULL;
	_maximumentitledPriority = new EMLPP_Priority();
	children.push_back(_maximumentitledPriority);

	// defaultPriority
	_defaultPriority = NULL;
	_defaultPriority = new EMLPP_Priority();
	children.push_back(_defaultPriority);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::EMLPP_Info::~EMLPP_Info(){

}

asn1::ASN1Node* asn1::EMLPP_Info::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::EMLPP_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//EMLPP_Priority
asn1::EMLPP_Priority::EMLPP_Priority(){
	strcpy(node_type_name, "EMLPP_Priority");

}
asn1::EMLPP_Priority::~EMLPP_Priority(){

}

//priorityLevelA
asn1::priorityLevelA::priorityLevelA(){
	strcpy(node_type_name, "priorityLevelA");

}
asn1::priorityLevelA::~priorityLevelA(){

}

//priorityLevelB
asn1::priorityLevelB::priorityLevelB(){
	strcpy(node_type_name, "priorityLevelB");

}
asn1::priorityLevelB::~priorityLevelB(){

}

//priorityLevel0
asn1::priorityLevel0::priorityLevel0(){
	strcpy(node_type_name, "priorityLevel0");

}
asn1::priorityLevel0::~priorityLevel0(){

}

//priorityLevel1
asn1::priorityLevel1::priorityLevel1(){
	strcpy(node_type_name, "priorityLevel1");

}
asn1::priorityLevel1::~priorityLevel1(){

}

//priorityLevel2
asn1::priorityLevel2::priorityLevel2(){
	strcpy(node_type_name, "priorityLevel2");

}
asn1::priorityLevel2::~priorityLevel2(){

}

//priorityLevel3
asn1::priorityLevel3::priorityLevel3(){
	strcpy(node_type_name, "priorityLevel3");

}
asn1::priorityLevel3::~priorityLevel3(){

}

//priorityLevel4
asn1::priorityLevel4::priorityLevel4(){
	strcpy(node_type_name, "priorityLevel4");

}
asn1::priorityLevel4::~priorityLevel4(){

}

//MC_SS_Info
asn1::MC_SS_Info::MC_SS_Info(){
	strcpy(node_type_name, "MC_SS_Info");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	_ss_Code->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Code->tlv->tag_value = 0;
	children.push_back(_ss_Code);

	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 1;
	children.push_back(_ss_Status);

	// nbrSB
	_nbrSB = NULL;
	_nbrSB = new MaxMC_Bearers();
	_nbrSB->tlv->tag_class = CONTEXT_SPECIFIC;
	_nbrSB->tlv->tag_value = 2;
	children.push_back(_nbrSB);

	// nbrUser
	_nbrUser = NULL;
	_nbrUser = new MC_Bearers();
	_nbrUser->tlv->tag_class = CONTEXT_SPECIFIC;
	_nbrUser->tlv->tag_value = 3;
	children.push_back(_nbrUser);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MC_SS_Info::~MC_SS_Info(){

}

asn1::ASN1Node* asn1::MC_SS_Info::create_node(unsigned int _index){
	switch(_index){
		case 4:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 4;
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::MC_SS_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//MaxMC_Bearers
asn1::MaxMC_Bearers::MaxMC_Bearers(){
	strcpy(node_type_name, "MaxMC_Bearers");

}
asn1::MaxMC_Bearers::~MaxMC_Bearers(){

}

//MC_Bearers
asn1::MC_Bearers::MC_Bearers(){
	strcpy(node_type_name, "MC_Bearers");

}
asn1::MC_Bearers::~MC_Bearers(){

}

//maxNumOfMC_Bearers
asn1::maxNumOfMC_Bearers::maxNumOfMC_Bearers(){
	strcpy(node_type_name, "maxNumOfMC_Bearers");

}
asn1::maxNumOfMC_Bearers::~maxNumOfMC_Bearers(){

}

//Ext_SS_Status
asn1::Ext_SS_Status::Ext_SS_Status(){
	strcpy(node_type_name, "Ext_SS_Status");

}
asn1::Ext_SS_Status::~Ext_SS_Status(){

}

//AgeOfLocationInformation
asn1::AgeOfLocationInformation::AgeOfLocationInformation(){
	strcpy(node_type_name, "AgeOfLocationInformation");

}
asn1::AgeOfLocationInformation::~AgeOfLocationInformation(){

}

