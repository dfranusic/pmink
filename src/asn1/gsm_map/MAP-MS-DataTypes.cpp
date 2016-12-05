#include<MAP-MS-DataTypes.h>
#include<iostream>

//UpdateLocationArg
asn1::UpdateLocationArg::UpdateLocationArg(){
	strcpy(node_type_name, "UpdateLocationArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// msc_Number
	_msc_Number = NULL;
	_msc_Number = new ISDN_AddressString();
	_msc_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_msc_Number->tlv->tag_value = 1;
	children.push_back(_msc_Number);

	// vlr_Number
	_vlr_Number = NULL;
	_vlr_Number = new ISDN_AddressString();
	children.push_back(_vlr_Number);

	// lmsi
	_lmsi = NULL;
	children.push_back(_lmsi);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// vlr_Capability
	_vlr_Capability = NULL;
	children.push_back(_vlr_Capability);

	// informPreviousNetworkEntity
	_informPreviousNetworkEntity = NULL;
	children.push_back(_informPreviousNetworkEntity);

	// cs_LCS_NotSupportedByUE
	_cs_LCS_NotSupportedByUE = NULL;
	children.push_back(_cs_LCS_NotSupportedByUE);

	// v_gmlc_Address
	_v_gmlc_Address = NULL;
	children.push_back(_v_gmlc_Address);

	// add_info
	_add_info = NULL;
	children.push_back(_add_info);

	// pagingArea
	_pagingArea = NULL;
	children.push_back(_pagingArea);

	// skipSubscriberDataUpdate
	_skipSubscriberDataUpdate = NULL;
	children.push_back(_skipSubscriberDataUpdate);

	// restorationIndicator
	_restorationIndicator = NULL;
	children.push_back(_restorationIndicator);

	// eplmn_List
	_eplmn_List = NULL;
	children.push_back(_eplmn_List);

	// mme_DiameterAddress
	_mme_DiameterAddress = NULL;
	children.push_back(_mme_DiameterAddress);


}
asn1::UpdateLocationArg::~UpdateLocationArg(){

}

asn1::ASN1Node* asn1::UpdateLocationArg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_lmsi = new LMSI();
			_lmsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_lmsi->tlv->tag_value = 10;
			children[3] = _lmsi;
			return _lmsi;
		case 4:
			_extensionContainer = new ExtensionContainer();
			children[4] = _extensionContainer;
			return _extensionContainer;
		case 5:
			_vlr_Capability = new VLR_Capability();
			_vlr_Capability->tlv->tag_class = CONTEXT_SPECIFIC;
			_vlr_Capability->tlv->tag_value = 6;
			children[5] = _vlr_Capability;
			return _vlr_Capability;
		case 6:
			_informPreviousNetworkEntity = new Null();
			_informPreviousNetworkEntity->tlv->tag_class = CONTEXT_SPECIFIC;
			_informPreviousNetworkEntity->tlv->tag_value = 11;
			children[6] = _informPreviousNetworkEntity;
			return _informPreviousNetworkEntity;
		case 7:
			_cs_LCS_NotSupportedByUE = new Null();
			_cs_LCS_NotSupportedByUE->tlv->tag_class = CONTEXT_SPECIFIC;
			_cs_LCS_NotSupportedByUE->tlv->tag_value = 12;
			children[7] = _cs_LCS_NotSupportedByUE;
			return _cs_LCS_NotSupportedByUE;
		case 8:
			_v_gmlc_Address = new GSN_Address();
			_v_gmlc_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_v_gmlc_Address->tlv->tag_value = 2;
			children[8] = _v_gmlc_Address;
			return _v_gmlc_Address;
		case 9:
			_add_info = new ADD_Info();
			_add_info->tlv->tag_class = CONTEXT_SPECIFIC;
			_add_info->tlv->tag_value = 13;
			children[9] = _add_info;
			return _add_info;
		case 10:
			_pagingArea = new PagingArea();
			_pagingArea->tlv->tag_class = CONTEXT_SPECIFIC;
			_pagingArea->tlv->tag_value = 14;
			children[10] = _pagingArea;
			return _pagingArea;
		case 11:
			_skipSubscriberDataUpdate = new Null();
			_skipSubscriberDataUpdate->tlv->tag_class = CONTEXT_SPECIFIC;
			_skipSubscriberDataUpdate->tlv->tag_value = 15;
			children[11] = _skipSubscriberDataUpdate;
			return _skipSubscriberDataUpdate;
		case 12:
			_restorationIndicator = new Null();
			_restorationIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_restorationIndicator->tlv->tag_value = 16;
			children[12] = _restorationIndicator;
			return _restorationIndicator;
		case 13:
			_eplmn_List = new EPLMN_List();
			_eplmn_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_eplmn_List->tlv->tag_value = 3;
			children[13] = _eplmn_List;
			return _eplmn_List;
		case 14:
			_mme_DiameterAddress = new NetworkNodeDiameterAddress();
			_mme_DiameterAddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_mme_DiameterAddress->tlv->tag_value = 4;
			children[14] = _mme_DiameterAddress;
			return _mme_DiameterAddress;

		default: return NULL;
	}
}

void asn1::UpdateLocationArg::set_lmsi(){
	if(_lmsi == NULL) _lmsi = (LMSI*)create_node(3);
}

void asn1::UpdateLocationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

void asn1::UpdateLocationArg::set_vlr_Capability(){
	if(_vlr_Capability == NULL) _vlr_Capability = (VLR_Capability*)create_node(5);
}

void asn1::UpdateLocationArg::set_informPreviousNetworkEntity(){
	if(_informPreviousNetworkEntity == NULL) _informPreviousNetworkEntity = (Null*)create_node(6);
}

void asn1::UpdateLocationArg::set_cs_LCS_NotSupportedByUE(){
	if(_cs_LCS_NotSupportedByUE == NULL) _cs_LCS_NotSupportedByUE = (Null*)create_node(7);
}

void asn1::UpdateLocationArg::set_v_gmlc_Address(){
	if(_v_gmlc_Address == NULL) _v_gmlc_Address = (GSN_Address*)create_node(8);
}

void asn1::UpdateLocationArg::set_add_info(){
	if(_add_info == NULL) _add_info = (ADD_Info*)create_node(9);
}

void asn1::UpdateLocationArg::set_pagingArea(){
	if(_pagingArea == NULL) _pagingArea = (PagingArea*)create_node(10);
}

void asn1::UpdateLocationArg::set_skipSubscriberDataUpdate(){
	if(_skipSubscriberDataUpdate == NULL) _skipSubscriberDataUpdate = (Null*)create_node(11);
}

void asn1::UpdateLocationArg::set_restorationIndicator(){
	if(_restorationIndicator == NULL) _restorationIndicator = (Null*)create_node(12);
}

void asn1::UpdateLocationArg::set_eplmn_List(){
	if(_eplmn_List == NULL) _eplmn_List = (EPLMN_List*)create_node(13);
}

void asn1::UpdateLocationArg::set_mme_DiameterAddress(){
	if(_mme_DiameterAddress == NULL) _mme_DiameterAddress = (NetworkNodeDiameterAddress*)create_node(14);
}

//VLR_Capability
asn1::VLR_Capability::VLR_Capability(){
	strcpy(node_type_name, "VLR_Capability");
	// supportedCamelPhases
	_supportedCamelPhases = NULL;
	children.push_back(_supportedCamelPhases);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// solsaSupportIndicator
	_solsaSupportIndicator = NULL;
	children.push_back(_solsaSupportIndicator);

	// istSupportIndicator
	_istSupportIndicator = NULL;
	children.push_back(_istSupportIndicator);

	// superChargerSupportedInServingNetworkEntity
	_superChargerSupportedInServingNetworkEntity = NULL;
	children.push_back(_superChargerSupportedInServingNetworkEntity);

	// longFTN_Supported
	_longFTN_Supported = NULL;
	children.push_back(_longFTN_Supported);

	// supportedLCS_CapabilitySets
	_supportedLCS_CapabilitySets = NULL;
	children.push_back(_supportedLCS_CapabilitySets);

	// offeredCamel4CSIs
	_offeredCamel4CSIs = NULL;
	children.push_back(_offeredCamel4CSIs);

	// supportedRAT_TypesIndicator
	_supportedRAT_TypesIndicator = NULL;
	children.push_back(_supportedRAT_TypesIndicator);

	// longGroupID_Supported
	_longGroupID_Supported = NULL;
	children.push_back(_longGroupID_Supported);

	// mtRoamingForwardingSupported
	_mtRoamingForwardingSupported = NULL;
	children.push_back(_mtRoamingForwardingSupported);

	// msisdn_lessOperation_Supported
	_msisdn_lessOperation_Supported = NULL;
	children.push_back(_msisdn_lessOperation_Supported);


}
asn1::VLR_Capability::~VLR_Capability(){

}

asn1::ASN1Node* asn1::VLR_Capability::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_supportedCamelPhases = new SupportedCamelPhases();
			_supportedCamelPhases->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedCamelPhases->tlv->tag_value = 0;
			children[0] = _supportedCamelPhases;
			return _supportedCamelPhases;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_solsaSupportIndicator = new Null();
			_solsaSupportIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_solsaSupportIndicator->tlv->tag_value = 2;
			children[2] = _solsaSupportIndicator;
			return _solsaSupportIndicator;
		case 3:
			_istSupportIndicator = new IST_SupportIndicator();
			_istSupportIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_istSupportIndicator->tlv->tag_value = 1;
			children[3] = _istSupportIndicator;
			return _istSupportIndicator;
                case 4:
                {
                        _superChargerSupportedInServingNetworkEntity = new SuperChargerInfo();
                        ASN1Node* _superChargerSupportedInServingNetworkEntity_wrapper = new ASN1Node();
                        _superChargerSupportedInServingNetworkEntity_wrapper->tlv = new TLVNode();
                        _superChargerSupportedInServingNetworkEntity_wrapper->tlv->is_explicit = true;
                        _superChargerSupportedInServingNetworkEntity_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _superChargerSupportedInServingNetworkEntity_wrapper->tlv->tag_value = 3;
                        _superChargerSupportedInServingNetworkEntity_wrapper->children.push_back(_superChargerSupportedInServingNetworkEntity);
                        children[4] = _superChargerSupportedInServingNetworkEntity_wrapper;
                        return _superChargerSupportedInServingNetworkEntity;
                }
		case 5:
			_longFTN_Supported = new Null();
			_longFTN_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_longFTN_Supported->tlv->tag_value = 4;
			children[5] = _longFTN_Supported;
			return _longFTN_Supported;
		case 6:
			_supportedLCS_CapabilitySets = new SupportedLCS_CapabilitySets();
			_supportedLCS_CapabilitySets->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedLCS_CapabilitySets->tlv->tag_value = 5;
			children[6] = _supportedLCS_CapabilitySets;
			return _supportedLCS_CapabilitySets;
		case 7:
			_offeredCamel4CSIs = new OfferedCamel4CSIs();
			_offeredCamel4CSIs->tlv->tag_class = CONTEXT_SPECIFIC;
			_offeredCamel4CSIs->tlv->tag_value = 6;
			children[7] = _offeredCamel4CSIs;
			return _offeredCamel4CSIs;
		case 8:
			_supportedRAT_TypesIndicator = new SupportedRAT_Types();
			_supportedRAT_TypesIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedRAT_TypesIndicator->tlv->tag_value = 7;
			children[8] = _supportedRAT_TypesIndicator;
			return _supportedRAT_TypesIndicator;
		case 9:
			_longGroupID_Supported = new Null();
			_longGroupID_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_longGroupID_Supported->tlv->tag_value = 8;
			children[9] = _longGroupID_Supported;
			return _longGroupID_Supported;
		case 10:
			_mtRoamingForwardingSupported = new Null();
			_mtRoamingForwardingSupported->tlv->tag_class = CONTEXT_SPECIFIC;
			_mtRoamingForwardingSupported->tlv->tag_value = 9;
			children[10] = _mtRoamingForwardingSupported;
			return _mtRoamingForwardingSupported;
		case 11:
			_msisdn_lessOperation_Supported = new Null();
			_msisdn_lessOperation_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn_lessOperation_Supported->tlv->tag_value = 10;
			children[11] = _msisdn_lessOperation_Supported;
			return _msisdn_lessOperation_Supported;

		default: return NULL;
	}
}

void asn1::VLR_Capability::set_supportedCamelPhases(){
	if(_supportedCamelPhases == NULL) _supportedCamelPhases = (SupportedCamelPhases*)create_node(0);
}

void asn1::VLR_Capability::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::VLR_Capability::set_solsaSupportIndicator(){
	if(_solsaSupportIndicator == NULL) _solsaSupportIndicator = (Null*)create_node(2);
}

void asn1::VLR_Capability::set_istSupportIndicator(){
	if(_istSupportIndicator == NULL) _istSupportIndicator = (IST_SupportIndicator*)create_node(3);
}

void asn1::VLR_Capability::set_superChargerSupportedInServingNetworkEntity(){
	if(_superChargerSupportedInServingNetworkEntity == NULL) _superChargerSupportedInServingNetworkEntity = (SuperChargerInfo*)create_node(4);
}

void asn1::VLR_Capability::set_longFTN_Supported(){
	if(_longFTN_Supported == NULL) _longFTN_Supported = (Null*)create_node(5);
}

void asn1::VLR_Capability::set_supportedLCS_CapabilitySets(){
	if(_supportedLCS_CapabilitySets == NULL) _supportedLCS_CapabilitySets = (SupportedLCS_CapabilitySets*)create_node(6);
}

void asn1::VLR_Capability::set_offeredCamel4CSIs(){
	if(_offeredCamel4CSIs == NULL) _offeredCamel4CSIs = (OfferedCamel4CSIs*)create_node(7);
}

void asn1::VLR_Capability::set_supportedRAT_TypesIndicator(){
	if(_supportedRAT_TypesIndicator == NULL) _supportedRAT_TypesIndicator = (SupportedRAT_Types*)create_node(8);
}

void asn1::VLR_Capability::set_longGroupID_Supported(){
	if(_longGroupID_Supported == NULL) _longGroupID_Supported = (Null*)create_node(9);
}

void asn1::VLR_Capability::set_mtRoamingForwardingSupported(){
	if(_mtRoamingForwardingSupported == NULL) _mtRoamingForwardingSupported = (Null*)create_node(10);
}

void asn1::VLR_Capability::set_msisdn_lessOperation_Supported(){
	if(_msisdn_lessOperation_Supported == NULL) _msisdn_lessOperation_Supported = (Null*)create_node(11);
}

//SupportedRAT_Types
asn1::SupportedRAT_Types::SupportedRAT_Types(){
	strcpy(node_type_name, "SupportedRAT_Types");

}
asn1::SupportedRAT_Types::~SupportedRAT_Types(){

}

//SuperChargerInfo
asn1::SuperChargerInfo::SuperChargerInfo(){
	strcpy(node_type_name, "SuperChargerInfo");
	// sendSubscriberData
	_sendSubscriberData = NULL;
	_sendSubscriberData = new Null();
	_sendSubscriberData->tlv->tag_class = CONTEXT_SPECIFIC;
	_sendSubscriberData->tlv->tag_value = 0;
	children.push_back(_sendSubscriberData);

	// subscriberDataStored
	_subscriberDataStored = NULL;
	_subscriberDataStored = new AgeIndicator();
	_subscriberDataStored->tlv->tag_class = CONTEXT_SPECIFIC;
	_subscriberDataStored->tlv->tag_value = 1;
	children.push_back(_subscriberDataStored);


}
asn1::SuperChargerInfo::~SuperChargerInfo(){

}

//AgeIndicator
asn1::AgeIndicator::AgeIndicator(){
	strcpy(node_type_name, "AgeIndicator");

}
asn1::AgeIndicator::~AgeIndicator(){

}

//IST_SupportIndicator
asn1::IST_SupportIndicator::IST_SupportIndicator(){
	strcpy(node_type_name, "IST_SupportIndicator");

}
asn1::IST_SupportIndicator::~IST_SupportIndicator(){

}

//SupportedLCS_CapabilitySets
asn1::SupportedLCS_CapabilitySets::SupportedLCS_CapabilitySets(){
	strcpy(node_type_name, "SupportedLCS_CapabilitySets");

}
asn1::SupportedLCS_CapabilitySets::~SupportedLCS_CapabilitySets(){

}

//UpdateLocationRes
asn1::UpdateLocationRes::UpdateLocationRes(){
	strcpy(node_type_name, "UpdateLocationRes");
	// hlr_Number
	_hlr_Number = NULL;
	_hlr_Number = new ISDN_AddressString();
	children.push_back(_hlr_Number);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// add_Capability
	_add_Capability = NULL;
	children.push_back(_add_Capability);

	// pagingArea_Capability
	_pagingArea_Capability = NULL;
	children.push_back(_pagingArea_Capability);


}
asn1::UpdateLocationRes::~UpdateLocationRes(){

}

asn1::ASN1Node* asn1::UpdateLocationRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_add_Capability = new Null();
			children[2] = _add_Capability;
			return _add_Capability;
		case 3:
			_pagingArea_Capability = new Null();
			_pagingArea_Capability->tlv->tag_class = CONTEXT_SPECIFIC;
			_pagingArea_Capability->tlv->tag_value = 0;
			children[3] = _pagingArea_Capability;
			return _pagingArea_Capability;

		default: return NULL;
	}
}

void asn1::UpdateLocationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::UpdateLocationRes::set_add_Capability(){
	if(_add_Capability == NULL) _add_Capability = (Null*)create_node(2);
}

void asn1::UpdateLocationRes::set_pagingArea_Capability(){
	if(_pagingArea_Capability == NULL) _pagingArea_Capability = (Null*)create_node(3);
}

//ADD_Info
asn1::ADD_Info::ADD_Info(){
	strcpy(node_type_name, "ADD_Info");
	// imeisv
	_imeisv = NULL;
	_imeisv = new IMEI();
	_imeisv->tlv->tag_class = CONTEXT_SPECIFIC;
	_imeisv->tlv->tag_value = 0;
	children.push_back(_imeisv);

	// skipSubscriberDataUpdate
	_skipSubscriberDataUpdate = NULL;
	children.push_back(_skipSubscriberDataUpdate);


}
asn1::ADD_Info::~ADD_Info(){

}

asn1::ASN1Node* asn1::ADD_Info::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_skipSubscriberDataUpdate = new Null();
			_skipSubscriberDataUpdate->tlv->tag_class = CONTEXT_SPECIFIC;
			_skipSubscriberDataUpdate->tlv->tag_value = 1;
			children[1] = _skipSubscriberDataUpdate;
			return _skipSubscriberDataUpdate;

		default: return NULL;
	}
}

void asn1::ADD_Info::set_skipSubscriberDataUpdate(){
	if(_skipSubscriberDataUpdate == NULL) _skipSubscriberDataUpdate = (Null*)create_node(1);
}

//PagingArea
asn1::PagingArea::PagingArea(){
	strcpy(node_type_name, "PagingArea");

}
asn1::PagingArea::~PagingArea(){
}

asn1::LocationArea* asn1::PagingArea::get_child(unsigned int child_index){
	if(child_index < children.size()) return (LocationArea*)children[child_index]; else return NULL;
}

void asn1::PagingArea::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::PagingArea::create_node(unsigned int _index){
	children[_index] = new LocationArea();
	return children[_index];
}

asn1::ASN1Node* asn1::PagingArea::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new LocationArea());
		return children[children.size() - 1];
	}

}

//LocationArea
asn1::LocationArea::LocationArea(){
	strcpy(node_type_name, "LocationArea");
	// laiFixedLength
	_laiFixedLength = NULL;
	_laiFixedLength = new LAIFixedLength();
	_laiFixedLength->tlv->tag_class = CONTEXT_SPECIFIC;
	_laiFixedLength->tlv->tag_value = 0;
	children.push_back(_laiFixedLength);

	// lac
	_lac = NULL;
	_lac = new LAC();
	_lac->tlv->tag_class = CONTEXT_SPECIFIC;
	_lac->tlv->tag_value = 1;
	children.push_back(_lac);


}
asn1::LocationArea::~LocationArea(){

}

//LAC
asn1::LAC::LAC(){
	strcpy(node_type_name, "LAC");

}
asn1::LAC::~LAC(){

}

//CancelLocationArg
asn1::CancelLocationArg::CancelLocationArg(){
	strcpy(node_type_name, "CancelLocationArg");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// identity
	_identity = NULL;
	_identity = new Identity();
	children.push_back(_identity);

	// cancellationType
	_cancellationType = NULL;
	children.push_back(_cancellationType);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// typeOfUpdate
	_typeOfUpdate = NULL;
	children.push_back(_typeOfUpdate);

	// mtrf_SupportedAndAuthorized
	_mtrf_SupportedAndAuthorized = NULL;
	children.push_back(_mtrf_SupportedAndAuthorized);

	// mtrf_SupportedAndNotAuthorized
	_mtrf_SupportedAndNotAuthorized = NULL;
	children.push_back(_mtrf_SupportedAndNotAuthorized);

	// newMSC_Number
	_newMSC_Number = NULL;
	children.push_back(_newMSC_Number);

	// newVLR_Number
	_newVLR_Number = NULL;
	children.push_back(_newVLR_Number);

	// new_lmsi
	_new_lmsi = NULL;
	children.push_back(_new_lmsi);


}
asn1::CancelLocationArg::~CancelLocationArg(){

}

asn1::ASN1Node* asn1::CancelLocationArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_cancellationType = new CancellationType();
			children[1] = _cancellationType;
			return _cancellationType;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_typeOfUpdate = new TypeOfUpdate();
			_typeOfUpdate->tlv->tag_class = CONTEXT_SPECIFIC;
			_typeOfUpdate->tlv->tag_value = 0;
			children[3] = _typeOfUpdate;
			return _typeOfUpdate;
		case 4:
			_mtrf_SupportedAndAuthorized = new Null();
			_mtrf_SupportedAndAuthorized->tlv->tag_class = CONTEXT_SPECIFIC;
			_mtrf_SupportedAndAuthorized->tlv->tag_value = 1;
			children[4] = _mtrf_SupportedAndAuthorized;
			return _mtrf_SupportedAndAuthorized;
		case 5:
			_mtrf_SupportedAndNotAuthorized = new Null();
			_mtrf_SupportedAndNotAuthorized->tlv->tag_class = CONTEXT_SPECIFIC;
			_mtrf_SupportedAndNotAuthorized->tlv->tag_value = 2;
			children[5] = _mtrf_SupportedAndNotAuthorized;
			return _mtrf_SupportedAndNotAuthorized;
		case 6:
			_newMSC_Number = new ISDN_AddressString();
			_newMSC_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_newMSC_Number->tlv->tag_value = 3;
			children[6] = _newMSC_Number;
			return _newMSC_Number;
		case 7:
			_newVLR_Number = new ISDN_AddressString();
			_newVLR_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_newVLR_Number->tlv->tag_value = 4;
			children[7] = _newVLR_Number;
			return _newVLR_Number;
		case 8:
			_new_lmsi = new LMSI();
			_new_lmsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_new_lmsi->tlv->tag_value = 5;
			children[8] = _new_lmsi;
			return _new_lmsi;

		default: return NULL;
	}
}

void asn1::CancelLocationArg::set_cancellationType(){
	if(_cancellationType == NULL) _cancellationType = (CancellationType*)create_node(1);
}

void asn1::CancelLocationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::CancelLocationArg::set_typeOfUpdate(){
	if(_typeOfUpdate == NULL) _typeOfUpdate = (TypeOfUpdate*)create_node(3);
}

void asn1::CancelLocationArg::set_mtrf_SupportedAndAuthorized(){
	if(_mtrf_SupportedAndAuthorized == NULL) _mtrf_SupportedAndAuthorized = (Null*)create_node(4);
}

void asn1::CancelLocationArg::set_mtrf_SupportedAndNotAuthorized(){
	if(_mtrf_SupportedAndNotAuthorized == NULL) _mtrf_SupportedAndNotAuthorized = (Null*)create_node(5);
}

void asn1::CancelLocationArg::set_newMSC_Number(){
	if(_newMSC_Number == NULL) _newMSC_Number = (ISDN_AddressString*)create_node(6);
}

void asn1::CancelLocationArg::set_newVLR_Number(){
	if(_newVLR_Number == NULL) _newVLR_Number = (ISDN_AddressString*)create_node(7);
}

void asn1::CancelLocationArg::set_new_lmsi(){
	if(_new_lmsi == NULL) _new_lmsi = (LMSI*)create_node(8);
}

//TypeOfUpdate
asn1::TypeOfUpdate::TypeOfUpdate(){
	strcpy(node_type_name, "TypeOfUpdate");

}
asn1::TypeOfUpdate::~TypeOfUpdate(){

}

//CancellationType
asn1::CancellationType::CancellationType(){
	strcpy(node_type_name, "CancellationType");

}
asn1::CancellationType::~CancellationType(){

}

//CancelLocationRes
asn1::CancelLocationRes::CancelLocationRes(){
	strcpy(node_type_name, "CancelLocationRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CancelLocationRes::~CancelLocationRes(){

}

asn1::ASN1Node* asn1::CancelLocationRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CancelLocationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//PurgeMS_Arg
asn1::PurgeMS_Arg::PurgeMS_Arg(){
	strcpy(node_type_name, "PurgeMS_Arg");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// vlr_Number
	_vlr_Number = NULL;
	children.push_back(_vlr_Number);

	// sgsn_Number
	_sgsn_Number = NULL;
	children.push_back(_sgsn_Number);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::PurgeMS_Arg::~PurgeMS_Arg(){

}

asn1::ASN1Node* asn1::PurgeMS_Arg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_vlr_Number = new ISDN_AddressString();
			_vlr_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_vlr_Number->tlv->tag_value = 0;
			children[1] = _vlr_Number;
			return _vlr_Number;
		case 2:
			_sgsn_Number = new ISDN_AddressString();
			_sgsn_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_Number->tlv->tag_value = 1;
			children[2] = _sgsn_Number;
			return _sgsn_Number;
		case 3:
			_extensionContainer = new ExtensionContainer();
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::PurgeMS_Arg::set_vlr_Number(){
	if(_vlr_Number == NULL) _vlr_Number = (ISDN_AddressString*)create_node(1);
}

void asn1::PurgeMS_Arg::set_sgsn_Number(){
	if(_sgsn_Number == NULL) _sgsn_Number = (ISDN_AddressString*)create_node(2);
}

void asn1::PurgeMS_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//PurgeMS_Res
asn1::PurgeMS_Res::PurgeMS_Res(){
	strcpy(node_type_name, "PurgeMS_Res");
	// freezeTMSI
	_freezeTMSI = NULL;
	children.push_back(_freezeTMSI);

	// freezeP_TMSI
	_freezeP_TMSI = NULL;
	children.push_back(_freezeP_TMSI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// freezeM_TMSI
	_freezeM_TMSI = NULL;
	children.push_back(_freezeM_TMSI);


}
asn1::PurgeMS_Res::~PurgeMS_Res(){

}

asn1::ASN1Node* asn1::PurgeMS_Res::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_freezeTMSI = new Null();
			_freezeTMSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_freezeTMSI->tlv->tag_value = 0;
			children[0] = _freezeTMSI;
			return _freezeTMSI;
		case 1:
			_freezeP_TMSI = new Null();
			_freezeP_TMSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_freezeP_TMSI->tlv->tag_value = 1;
			children[1] = _freezeP_TMSI;
			return _freezeP_TMSI;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_freezeM_TMSI = new Null();
			_freezeM_TMSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_freezeM_TMSI->tlv->tag_value = 2;
			children[3] = _freezeM_TMSI;
			return _freezeM_TMSI;

		default: return NULL;
	}
}

void asn1::PurgeMS_Res::set_freezeTMSI(){
	if(_freezeTMSI == NULL) _freezeTMSI = (Null*)create_node(0);
}

void asn1::PurgeMS_Res::set_freezeP_TMSI(){
	if(_freezeP_TMSI == NULL) _freezeP_TMSI = (Null*)create_node(1);
}

void asn1::PurgeMS_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::PurgeMS_Res::set_freezeM_TMSI(){
	if(_freezeM_TMSI == NULL) _freezeM_TMSI = (Null*)create_node(3);
}

//SendIdentificationArg
asn1::SendIdentificationArg::SendIdentificationArg(){
	strcpy(node_type_name, "SendIdentificationArg");
	// tmsi
	_tmsi = NULL;
	_tmsi = new TMSI();
	children.push_back(_tmsi);

	// numberOfRequestedVectors
	_numberOfRequestedVectors = NULL;
	children.push_back(_numberOfRequestedVectors);

	// segmentationProhibited
	_segmentationProhibited = NULL;
	children.push_back(_segmentationProhibited);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// msc_Number
	_msc_Number = NULL;
	children.push_back(_msc_Number);

	// previous_LAI
	_previous_LAI = NULL;
	children.push_back(_previous_LAI);

	// hopCounter
	_hopCounter = NULL;
	children.push_back(_hopCounter);

	// mtRoamingForwardingSupported
	_mtRoamingForwardingSupported = NULL;
	children.push_back(_mtRoamingForwardingSupported);

	// newVLR_Number
	_newVLR_Number = NULL;
	children.push_back(_newVLR_Number);

	// new_lmsi
	_new_lmsi = NULL;
	children.push_back(_new_lmsi);


}
asn1::SendIdentificationArg::~SendIdentificationArg(){

}

asn1::ASN1Node* asn1::SendIdentificationArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_numberOfRequestedVectors = new NumberOfRequestedVectors();
			children[1] = _numberOfRequestedVectors;
			return _numberOfRequestedVectors;
		case 2:
			_segmentationProhibited = new Null();
			children[2] = _segmentationProhibited;
			return _segmentationProhibited;
		case 3:
			_extensionContainer = new ExtensionContainer();
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_msc_Number = new ISDN_AddressString();
			children[4] = _msc_Number;
			return _msc_Number;
		case 5:
			_previous_LAI = new LAIFixedLength();
			_previous_LAI->tlv->tag_class = CONTEXT_SPECIFIC;
			_previous_LAI->tlv->tag_value = 0;
			children[5] = _previous_LAI;
			return _previous_LAI;
		case 6:
			_hopCounter = new HopCounter();
			_hopCounter->tlv->tag_class = CONTEXT_SPECIFIC;
			_hopCounter->tlv->tag_value = 1;
			children[6] = _hopCounter;
			return _hopCounter;
		case 7:
			_mtRoamingForwardingSupported = new Null();
			_mtRoamingForwardingSupported->tlv->tag_class = CONTEXT_SPECIFIC;
			_mtRoamingForwardingSupported->tlv->tag_value = 2;
			children[7] = _mtRoamingForwardingSupported;
			return _mtRoamingForwardingSupported;
		case 8:
			_newVLR_Number = new ISDN_AddressString();
			_newVLR_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_newVLR_Number->tlv->tag_value = 3;
			children[8] = _newVLR_Number;
			return _newVLR_Number;
		case 9:
			_new_lmsi = new LMSI();
			_new_lmsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_new_lmsi->tlv->tag_value = 4;
			children[9] = _new_lmsi;
			return _new_lmsi;

		default: return NULL;
	}
}

void asn1::SendIdentificationArg::set_numberOfRequestedVectors(){
	if(_numberOfRequestedVectors == NULL) _numberOfRequestedVectors = (NumberOfRequestedVectors*)create_node(1);
}

void asn1::SendIdentificationArg::set_segmentationProhibited(){
	if(_segmentationProhibited == NULL) _segmentationProhibited = (Null*)create_node(2);
}

void asn1::SendIdentificationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::SendIdentificationArg::set_msc_Number(){
	if(_msc_Number == NULL) _msc_Number = (ISDN_AddressString*)create_node(4);
}

void asn1::SendIdentificationArg::set_previous_LAI(){
	if(_previous_LAI == NULL) _previous_LAI = (LAIFixedLength*)create_node(5);
}

void asn1::SendIdentificationArg::set_hopCounter(){
	if(_hopCounter == NULL) _hopCounter = (HopCounter*)create_node(6);
}

void asn1::SendIdentificationArg::set_mtRoamingForwardingSupported(){
	if(_mtRoamingForwardingSupported == NULL) _mtRoamingForwardingSupported = (Null*)create_node(7);
}

void asn1::SendIdentificationArg::set_newVLR_Number(){
	if(_newVLR_Number == NULL) _newVLR_Number = (ISDN_AddressString*)create_node(8);
}

void asn1::SendIdentificationArg::set_new_lmsi(){
	if(_new_lmsi == NULL) _new_lmsi = (LMSI*)create_node(9);
}

//HopCounter
asn1::HopCounter::HopCounter(){
	strcpy(node_type_name, "HopCounter");

}
asn1::HopCounter::~HopCounter(){

}

//SendIdentificationRes
asn1::SendIdentificationRes::SendIdentificationRes(){
	strcpy(node_type_name, "SendIdentificationRes");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// authenticationSetList
	_authenticationSetList = NULL;
	children.push_back(_authenticationSetList);

	// currentSecurityContext
	_currentSecurityContext = NULL;
	children.push_back(_currentSecurityContext);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SendIdentificationRes::~SendIdentificationRes(){

}

asn1::ASN1Node* asn1::SendIdentificationRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_imsi = new IMSI();
			children[0] = _imsi;
			return _imsi;
		case 1:
			_authenticationSetList = new AuthenticationSetList();
			children[1] = _authenticationSetList;
			return _authenticationSetList;
                case 2:
                {
                        _currentSecurityContext = new CurrentSecurityContext();
                        ASN1Node* _currentSecurityContext_wrapper = new ASN1Node();
                        _currentSecurityContext_wrapper->tlv = new TLVNode();
                        _currentSecurityContext_wrapper->tlv->is_explicit = true;
                        _currentSecurityContext_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _currentSecurityContext_wrapper->tlv->tag_value = 2;
                        _currentSecurityContext_wrapper->children.push_back(_currentSecurityContext);
                        children[2] = _currentSecurityContext_wrapper;
                        return _currentSecurityContext;
                }
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SendIdentificationRes::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(0);
}

void asn1::SendIdentificationRes::set_authenticationSetList(){
	if(_authenticationSetList == NULL) _authenticationSetList = (AuthenticationSetList*)create_node(1);
}

void asn1::SendIdentificationRes::set_currentSecurityContext(){
	if(_currentSecurityContext == NULL) _currentSecurityContext = (CurrentSecurityContext*)create_node(2);
}

void asn1::SendIdentificationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//AuthenticationSetList
asn1::AuthenticationSetList::AuthenticationSetList(){
	strcpy(node_type_name, "AuthenticationSetList");
	// tripletList
	_tripletList = NULL;
	_tripletList = new TripletList();
	_tripletList->tlv->tag_class = CONTEXT_SPECIFIC;
	_tripletList->tlv->tag_value = 0;
	children.push_back(_tripletList);

	// quintupletList
	_quintupletList = NULL;
	_quintupletList = new QuintupletList();
	_quintupletList->tlv->tag_class = CONTEXT_SPECIFIC;
	_quintupletList->tlv->tag_value = 1;
	children.push_back(_quintupletList);


}
asn1::AuthenticationSetList::~AuthenticationSetList(){

}

//TripletList
asn1::TripletList::TripletList(){
	strcpy(node_type_name, "TripletList");

}
asn1::TripletList::~TripletList(){
}

asn1::AuthenticationTriplet* asn1::TripletList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (AuthenticationTriplet*)children[child_index]; else return NULL;
}

void asn1::TripletList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::TripletList::create_node(unsigned int _index){
	children[_index] = new AuthenticationTriplet();
	return children[_index];
}

asn1::ASN1Node* asn1::TripletList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new AuthenticationTriplet());
		return children[children.size() - 1];
	}

}

//QuintupletList
asn1::QuintupletList::QuintupletList(){
	strcpy(node_type_name, "QuintupletList");

}
asn1::QuintupletList::~QuintupletList(){
}

asn1::AuthenticationQuintuplet* asn1::QuintupletList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (AuthenticationQuintuplet*)children[child_index]; else return NULL;
}

void asn1::QuintupletList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::QuintupletList::create_node(unsigned int _index){
	children[_index] = new AuthenticationQuintuplet();
	return children[_index];
}

asn1::ASN1Node* asn1::QuintupletList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new AuthenticationQuintuplet());
		return children[children.size() - 1];
	}

}

//AuthenticationTriplet
asn1::AuthenticationTriplet::AuthenticationTriplet(){
	strcpy(node_type_name, "AuthenticationTriplet");
	// rand
	_rand = NULL;
	_rand = new RAND();
	children.push_back(_rand);

	// sres
	_sres = NULL;
	_sres = new SRES();
	children.push_back(_sres);

	// kc
	_kc = NULL;
	_kc = new Kc();
	children.push_back(_kc);


}
asn1::AuthenticationTriplet::~AuthenticationTriplet(){

}

//AuthenticationQuintuplet
asn1::AuthenticationQuintuplet::AuthenticationQuintuplet(){
	strcpy(node_type_name, "AuthenticationQuintuplet");
	// rand
	_rand = NULL;
	_rand = new RAND();
	children.push_back(_rand);

	// xres
	_xres = NULL;
	_xres = new XRES();
	children.push_back(_xres);

	// ck
	_ck = NULL;
	_ck = new CK();
	children.push_back(_ck);

	// ik
	_ik = NULL;
	_ik = new IK();
	children.push_back(_ik);

	// autn
	_autn = NULL;
	_autn = new AUTN();
	children.push_back(_autn);


}
asn1::AuthenticationQuintuplet::~AuthenticationQuintuplet(){

}

//CurrentSecurityContext
asn1::CurrentSecurityContext::CurrentSecurityContext(){
	strcpy(node_type_name, "CurrentSecurityContext");
	// gsm_SecurityContextData
	_gsm_SecurityContextData = NULL;
	_gsm_SecurityContextData = new GSM_SecurityContextData();
	_gsm_SecurityContextData->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsm_SecurityContextData->tlv->tag_value = 0;
	children.push_back(_gsm_SecurityContextData);

	// umts_SecurityContextData
	_umts_SecurityContextData = NULL;
	_umts_SecurityContextData = new UMTS_SecurityContextData();
	_umts_SecurityContextData->tlv->tag_class = CONTEXT_SPECIFIC;
	_umts_SecurityContextData->tlv->tag_value = 1;
	children.push_back(_umts_SecurityContextData);


}
asn1::CurrentSecurityContext::~CurrentSecurityContext(){

}

//GSM_SecurityContextData
asn1::GSM_SecurityContextData::GSM_SecurityContextData(){
	strcpy(node_type_name, "GSM_SecurityContextData");
	// kc
	_kc = NULL;
	_kc = new Kc();
	children.push_back(_kc);

	// cksn
	_cksn = NULL;
	_cksn = new Cksn();
	children.push_back(_cksn);


}
asn1::GSM_SecurityContextData::~GSM_SecurityContextData(){

}

//UMTS_SecurityContextData
asn1::UMTS_SecurityContextData::UMTS_SecurityContextData(){
	strcpy(node_type_name, "UMTS_SecurityContextData");
	// ck
	_ck = NULL;
	_ck = new CK();
	children.push_back(_ck);

	// ik
	_ik = NULL;
	_ik = new IK();
	children.push_back(_ik);

	// ksi
	_ksi = NULL;
	_ksi = new KSI();
	children.push_back(_ksi);


}
asn1::UMTS_SecurityContextData::~UMTS_SecurityContextData(){

}

//RAND
asn1::RAND::RAND(){
	strcpy(node_type_name, "RAND");

}
asn1::RAND::~RAND(){

}

//SRES
asn1::SRES::SRES(){
	strcpy(node_type_name, "SRES");

}
asn1::SRES::~SRES(){

}

//Kc
asn1::Kc::Kc(){
	strcpy(node_type_name, "Kc");

}
asn1::Kc::~Kc(){

}

//XRES
asn1::XRES::XRES(){
	strcpy(node_type_name, "XRES");

}
asn1::XRES::~XRES(){

}

//CK
asn1::CK::CK(){
	strcpy(node_type_name, "CK");

}
asn1::CK::~CK(){

}

//IK
asn1::IK::IK(){
	strcpy(node_type_name, "IK");

}
asn1::IK::~IK(){

}

//AUTN
asn1::AUTN::AUTN(){
	strcpy(node_type_name, "AUTN");

}
asn1::AUTN::~AUTN(){

}

//AUTS
asn1::AUTS::AUTS(){
	strcpy(node_type_name, "AUTS");

}
asn1::AUTS::~AUTS(){

}

//Cksn
asn1::Cksn::Cksn(){
	strcpy(node_type_name, "Cksn");

}
asn1::Cksn::~Cksn(){

}

//KSI
asn1::KSI::KSI(){
	strcpy(node_type_name, "KSI");

}
asn1::KSI::~KSI(){

}

//AuthenticationFailureReportArg
asn1::AuthenticationFailureReportArg::AuthenticationFailureReportArg(){
	strcpy(node_type_name, "AuthenticationFailureReportArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// failureCause
	_failureCause = NULL;
	_failureCause = new FailureCause();
	children.push_back(_failureCause);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// re_attempt
	_re_attempt = NULL;
	children.push_back(_re_attempt);

	// accessType
	_accessType = NULL;
	children.push_back(_accessType);

	// rand
	_rand = NULL;
	children.push_back(_rand);

	// vlr_Number
	_vlr_Number = NULL;
	children.push_back(_vlr_Number);

	// sgsn_Number
	_sgsn_Number = NULL;
	children.push_back(_sgsn_Number);


}
asn1::AuthenticationFailureReportArg::~AuthenticationFailureReportArg(){

}

asn1::ASN1Node* asn1::AuthenticationFailureReportArg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_re_attempt = new Boolean();
			children[3] = _re_attempt;
			return _re_attempt;
		case 4:
			_accessType = new AccessType();
			children[4] = _accessType;
			return _accessType;
		case 5:
			_rand = new RAND();
			children[5] = _rand;
			return _rand;
		case 6:
			_vlr_Number = new ISDN_AddressString();
			_vlr_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_vlr_Number->tlv->tag_value = 0;
			children[6] = _vlr_Number;
			return _vlr_Number;
		case 7:
			_sgsn_Number = new ISDN_AddressString();
			_sgsn_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_Number->tlv->tag_value = 1;
			children[7] = _sgsn_Number;
			return _sgsn_Number;

		default: return NULL;
	}
}

void asn1::AuthenticationFailureReportArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::AuthenticationFailureReportArg::set_re_attempt(){
	if(_re_attempt == NULL) _re_attempt = (Boolean*)create_node(3);
}

void asn1::AuthenticationFailureReportArg::set_accessType(){
	if(_accessType == NULL) _accessType = (AccessType*)create_node(4);
}

void asn1::AuthenticationFailureReportArg::set_rand(){
	if(_rand == NULL) _rand = (RAND*)create_node(5);
}

void asn1::AuthenticationFailureReportArg::set_vlr_Number(){
	if(_vlr_Number == NULL) _vlr_Number = (ISDN_AddressString*)create_node(6);
}

void asn1::AuthenticationFailureReportArg::set_sgsn_Number(){
	if(_sgsn_Number == NULL) _sgsn_Number = (ISDN_AddressString*)create_node(7);
}

//AccessType
asn1::AccessType::AccessType(){
	strcpy(node_type_name, "AccessType");

}
asn1::AccessType::~AccessType(){

}

//AuthenticationFailureReportRes
asn1::AuthenticationFailureReportRes::AuthenticationFailureReportRes(){
	strcpy(node_type_name, "AuthenticationFailureReportRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::AuthenticationFailureReportRes::~AuthenticationFailureReportRes(){

}

asn1::ASN1Node* asn1::AuthenticationFailureReportRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::AuthenticationFailureReportRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//FailureCause
asn1::FailureCause::FailureCause(){
	strcpy(node_type_name, "FailureCause");

}
asn1::FailureCause::~FailureCause(){

}

//UpdateGprsLocationArg
asn1::UpdateGprsLocationArg::UpdateGprsLocationArg(){
	strcpy(node_type_name, "UpdateGprsLocationArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// sgsn_Number
	_sgsn_Number = NULL;
	_sgsn_Number = new ISDN_AddressString();
	children.push_back(_sgsn_Number);

	// sgsn_Address
	_sgsn_Address = NULL;
	_sgsn_Address = new GSN_Address();
	children.push_back(_sgsn_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// sgsn_Capability
	_sgsn_Capability = NULL;
	children.push_back(_sgsn_Capability);

	// informPreviousNetworkEntity
	_informPreviousNetworkEntity = NULL;
	children.push_back(_informPreviousNetworkEntity);

	// ps_LCS_NotSupportedByUE
	_ps_LCS_NotSupportedByUE = NULL;
	children.push_back(_ps_LCS_NotSupportedByUE);

	// v_gmlc_Address
	_v_gmlc_Address = NULL;
	children.push_back(_v_gmlc_Address);

	// add_info
	_add_info = NULL;
	children.push_back(_add_info);

	// eps_info
	_eps_info = NULL;
	children.push_back(_eps_info);

	// servingNodeTypeIndicator
	_servingNodeTypeIndicator = NULL;
	children.push_back(_servingNodeTypeIndicator);

	// skipSubscriberDataUpdate
	_skipSubscriberDataUpdate = NULL;
	children.push_back(_skipSubscriberDataUpdate);

	// usedRAT_Type
	_usedRAT_Type = NULL;
	children.push_back(_usedRAT_Type);

	// gprsSubscriptionDataNotNeeded
	_gprsSubscriptionDataNotNeeded = NULL;
	children.push_back(_gprsSubscriptionDataNotNeeded);

	// nodeTypeIndicator
	_nodeTypeIndicator = NULL;
	children.push_back(_nodeTypeIndicator);

	// areaRestricted
	_areaRestricted = NULL;
	children.push_back(_areaRestricted);

	// ue_reachableIndicator
	_ue_reachableIndicator = NULL;
	children.push_back(_ue_reachableIndicator);

	// epsSubscriptionDataNotNeeded
	_epsSubscriptionDataNotNeeded = NULL;
	children.push_back(_epsSubscriptionDataNotNeeded);

	// ue_srvcc_Capability
	_ue_srvcc_Capability = NULL;
	children.push_back(_ue_srvcc_Capability);

	// eplmn_List
	_eplmn_List = NULL;
	children.push_back(_eplmn_List);

	// mmeNumberforMTSMS
	_mmeNumberforMTSMS = NULL;
	children.push_back(_mmeNumberforMTSMS);

	// smsRegisterRequest
	_smsRegisterRequest = NULL;
	children.push_back(_smsRegisterRequest);

	// sms_Only
	_sms_Only = NULL;
	children.push_back(_sms_Only);


}
asn1::UpdateGprsLocationArg::~UpdateGprsLocationArg(){

}

asn1::ASN1Node* asn1::UpdateGprsLocationArg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_extensionContainer = new ExtensionContainer();
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_sgsn_Capability = new SGSN_Capability();
			_sgsn_Capability->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_Capability->tlv->tag_value = 0;
			children[4] = _sgsn_Capability;
			return _sgsn_Capability;
		case 5:
			_informPreviousNetworkEntity = new Null();
			_informPreviousNetworkEntity->tlv->tag_class = CONTEXT_SPECIFIC;
			_informPreviousNetworkEntity->tlv->tag_value = 1;
			children[5] = _informPreviousNetworkEntity;
			return _informPreviousNetworkEntity;
		case 6:
			_ps_LCS_NotSupportedByUE = new Null();
			_ps_LCS_NotSupportedByUE->tlv->tag_class = CONTEXT_SPECIFIC;
			_ps_LCS_NotSupportedByUE->tlv->tag_value = 2;
			children[6] = _ps_LCS_NotSupportedByUE;
			return _ps_LCS_NotSupportedByUE;
		case 7:
			_v_gmlc_Address = new GSN_Address();
			_v_gmlc_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_v_gmlc_Address->tlv->tag_value = 3;
			children[7] = _v_gmlc_Address;
			return _v_gmlc_Address;
		case 8:
			_add_info = new ADD_Info();
			_add_info->tlv->tag_class = CONTEXT_SPECIFIC;
			_add_info->tlv->tag_value = 4;
			children[8] = _add_info;
			return _add_info;
                case 9:
                {
                        _eps_info = new EPS_Info();
                        ASN1Node* _eps_info_wrapper = new ASN1Node();
                        _eps_info_wrapper->tlv = new TLVNode();
                        _eps_info_wrapper->tlv->is_explicit = true;
                        _eps_info_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _eps_info_wrapper->tlv->tag_value = 5;
                        _eps_info_wrapper->children.push_back(_eps_info);
                        children[9] = _eps_info_wrapper;
                        return _eps_info;
                }
		case 10:
			_servingNodeTypeIndicator = new Null();
			_servingNodeTypeIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_servingNodeTypeIndicator->tlv->tag_value = 6;
			children[10] = _servingNodeTypeIndicator;
			return _servingNodeTypeIndicator;
		case 11:
			_skipSubscriberDataUpdate = new Null();
			_skipSubscriberDataUpdate->tlv->tag_class = CONTEXT_SPECIFIC;
			_skipSubscriberDataUpdate->tlv->tag_value = 7;
			children[11] = _skipSubscriberDataUpdate;
			return _skipSubscriberDataUpdate;
		case 12:
			_usedRAT_Type = new Used_RAT_Type();
			_usedRAT_Type->tlv->tag_class = CONTEXT_SPECIFIC;
			_usedRAT_Type->tlv->tag_value = 8;
			children[12] = _usedRAT_Type;
			return _usedRAT_Type;
		case 13:
			_gprsSubscriptionDataNotNeeded = new Null();
			_gprsSubscriptionDataNotNeeded->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprsSubscriptionDataNotNeeded->tlv->tag_value = 9;
			children[13] = _gprsSubscriptionDataNotNeeded;
			return _gprsSubscriptionDataNotNeeded;
		case 14:
			_nodeTypeIndicator = new Null();
			_nodeTypeIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_nodeTypeIndicator->tlv->tag_value = 10;
			children[14] = _nodeTypeIndicator;
			return _nodeTypeIndicator;
		case 15:
			_areaRestricted = new Null();
			_areaRestricted->tlv->tag_class = CONTEXT_SPECIFIC;
			_areaRestricted->tlv->tag_value = 11;
			children[15] = _areaRestricted;
			return _areaRestricted;
		case 16:
			_ue_reachableIndicator = new Null();
			_ue_reachableIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_ue_reachableIndicator->tlv->tag_value = 12;
			children[16] = _ue_reachableIndicator;
			return _ue_reachableIndicator;
		case 17:
			_epsSubscriptionDataNotNeeded = new Null();
			_epsSubscriptionDataNotNeeded->tlv->tag_class = CONTEXT_SPECIFIC;
			_epsSubscriptionDataNotNeeded->tlv->tag_value = 13;
			children[17] = _epsSubscriptionDataNotNeeded;
			return _epsSubscriptionDataNotNeeded;
		case 18:
			_ue_srvcc_Capability = new UE_SRVCC_Capability();
			_ue_srvcc_Capability->tlv->tag_class = CONTEXT_SPECIFIC;
			_ue_srvcc_Capability->tlv->tag_value = 14;
			children[18] = _ue_srvcc_Capability;
			return _ue_srvcc_Capability;
		case 19:
			_eplmn_List = new EPLMN_List();
			_eplmn_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_eplmn_List->tlv->tag_value = 15;
			children[19] = _eplmn_List;
			return _eplmn_List;
		case 20:
			_mmeNumberforMTSMS = new ISDN_AddressString();
			_mmeNumberforMTSMS->tlv->tag_class = CONTEXT_SPECIFIC;
			_mmeNumberforMTSMS->tlv->tag_value = 16;
			children[20] = _mmeNumberforMTSMS;
			return _mmeNumberforMTSMS;
		case 21:
			_smsRegisterRequest = new SMSRegisterRequest();
			_smsRegisterRequest->tlv->tag_class = CONTEXT_SPECIFIC;
			_smsRegisterRequest->tlv->tag_value = 17;
			children[21] = _smsRegisterRequest;
			return _smsRegisterRequest;
		case 22:
			_sms_Only = new Null();
			_sms_Only->tlv->tag_class = CONTEXT_SPECIFIC;
			_sms_Only->tlv->tag_value = 18;
			children[22] = _sms_Only;
			return _sms_Only;

		default: return NULL;
	}
}

void asn1::UpdateGprsLocationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::UpdateGprsLocationArg::set_sgsn_Capability(){
	if(_sgsn_Capability == NULL) _sgsn_Capability = (SGSN_Capability*)create_node(4);
}

void asn1::UpdateGprsLocationArg::set_informPreviousNetworkEntity(){
	if(_informPreviousNetworkEntity == NULL) _informPreviousNetworkEntity = (Null*)create_node(5);
}

void asn1::UpdateGprsLocationArg::set_ps_LCS_NotSupportedByUE(){
	if(_ps_LCS_NotSupportedByUE == NULL) _ps_LCS_NotSupportedByUE = (Null*)create_node(6);
}

void asn1::UpdateGprsLocationArg::set_v_gmlc_Address(){
	if(_v_gmlc_Address == NULL) _v_gmlc_Address = (GSN_Address*)create_node(7);
}

void asn1::UpdateGprsLocationArg::set_add_info(){
	if(_add_info == NULL) _add_info = (ADD_Info*)create_node(8);
}

void asn1::UpdateGprsLocationArg::set_eps_info(){
	if(_eps_info == NULL) _eps_info = (EPS_Info*)create_node(9);
}

void asn1::UpdateGprsLocationArg::set_servingNodeTypeIndicator(){
	if(_servingNodeTypeIndicator == NULL) _servingNodeTypeIndicator = (Null*)create_node(10);
}

void asn1::UpdateGprsLocationArg::set_skipSubscriberDataUpdate(){
	if(_skipSubscriberDataUpdate == NULL) _skipSubscriberDataUpdate = (Null*)create_node(11);
}

void asn1::UpdateGprsLocationArg::set_usedRAT_Type(){
	if(_usedRAT_Type == NULL) _usedRAT_Type = (Used_RAT_Type*)create_node(12);
}

void asn1::UpdateGprsLocationArg::set_gprsSubscriptionDataNotNeeded(){
	if(_gprsSubscriptionDataNotNeeded == NULL) _gprsSubscriptionDataNotNeeded = (Null*)create_node(13);
}

void asn1::UpdateGprsLocationArg::set_nodeTypeIndicator(){
	if(_nodeTypeIndicator == NULL) _nodeTypeIndicator = (Null*)create_node(14);
}

void asn1::UpdateGprsLocationArg::set_areaRestricted(){
	if(_areaRestricted == NULL) _areaRestricted = (Null*)create_node(15);
}

void asn1::UpdateGprsLocationArg::set_ue_reachableIndicator(){
	if(_ue_reachableIndicator == NULL) _ue_reachableIndicator = (Null*)create_node(16);
}

void asn1::UpdateGprsLocationArg::set_epsSubscriptionDataNotNeeded(){
	if(_epsSubscriptionDataNotNeeded == NULL) _epsSubscriptionDataNotNeeded = (Null*)create_node(17);
}

void asn1::UpdateGprsLocationArg::set_ue_srvcc_Capability(){
	if(_ue_srvcc_Capability == NULL) _ue_srvcc_Capability = (UE_SRVCC_Capability*)create_node(18);
}

void asn1::UpdateGprsLocationArg::set_eplmn_List(){
	if(_eplmn_List == NULL) _eplmn_List = (EPLMN_List*)create_node(19);
}

void asn1::UpdateGprsLocationArg::set_mmeNumberforMTSMS(){
	if(_mmeNumberforMTSMS == NULL) _mmeNumberforMTSMS = (ISDN_AddressString*)create_node(20);
}

void asn1::UpdateGprsLocationArg::set_smsRegisterRequest(){
	if(_smsRegisterRequest == NULL) _smsRegisterRequest = (SMSRegisterRequest*)create_node(21);
}

void asn1::UpdateGprsLocationArg::set_sms_Only(){
	if(_sms_Only == NULL) _sms_Only = (Null*)create_node(22);
}

//SMSRegisterRequest
asn1::SMSRegisterRequest::SMSRegisterRequest(){
	strcpy(node_type_name, "SMSRegisterRequest");

}
asn1::SMSRegisterRequest::~SMSRegisterRequest(){

}

//Used_RAT_Type
asn1::Used_RAT_Type::Used_RAT_Type(){
	strcpy(node_type_name, "Used_RAT_Type");

}
asn1::Used_RAT_Type::~Used_RAT_Type(){

}

//EPS_Info
asn1::EPS_Info::EPS_Info(){
	strcpy(node_type_name, "EPS_Info");
	// pdn_gw_update
	_pdn_gw_update = NULL;
	_pdn_gw_update = new PDN_GW_Update();
	_pdn_gw_update->tlv->tag_class = CONTEXT_SPECIFIC;
	_pdn_gw_update->tlv->tag_value = 0;
	children.push_back(_pdn_gw_update);

	// isr_Information
	_isr_Information = NULL;
	_isr_Information = new ISR_Information();
	_isr_Information->tlv->tag_class = CONTEXT_SPECIFIC;
	_isr_Information->tlv->tag_value = 1;
	children.push_back(_isr_Information);


}
asn1::EPS_Info::~EPS_Info(){

}

//PDN_GW_Update
asn1::PDN_GW_Update::PDN_GW_Update(){
	strcpy(node_type_name, "PDN_GW_Update");
	// apn
	_apn = NULL;
	children.push_back(_apn);

	// pdn_gw_Identity
	_pdn_gw_Identity = NULL;
	children.push_back(_pdn_gw_Identity);

	// contextId
	_contextId = NULL;
	children.push_back(_contextId);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::PDN_GW_Update::~PDN_GW_Update(){

}

asn1::ASN1Node* asn1::PDN_GW_Update::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_apn = new APN();
			_apn->tlv->tag_class = CONTEXT_SPECIFIC;
			_apn->tlv->tag_value = 0;
			children[0] = _apn;
			return _apn;
		case 1:
			_pdn_gw_Identity = new PDN_GW_Identity();
			_pdn_gw_Identity->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdn_gw_Identity->tlv->tag_value = 1;
			children[1] = _pdn_gw_Identity;
			return _pdn_gw_Identity;
		case 2:
			_contextId = new ContextId();
			_contextId->tlv->tag_class = CONTEXT_SPECIFIC;
			_contextId->tlv->tag_value = 2;
			children[2] = _contextId;
			return _contextId;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::PDN_GW_Update::set_apn(){
	if(_apn == NULL) _apn = (APN*)create_node(0);
}

void asn1::PDN_GW_Update::set_pdn_gw_Identity(){
	if(_pdn_gw_Identity == NULL) _pdn_gw_Identity = (PDN_GW_Identity*)create_node(1);
}

void asn1::PDN_GW_Update::set_contextId(){
	if(_contextId == NULL) _contextId = (ContextId*)create_node(2);
}

void asn1::PDN_GW_Update::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//ISR_Information
asn1::ISR_Information::ISR_Information(){
	strcpy(node_type_name, "ISR_Information");

}
asn1::ISR_Information::~ISR_Information(){

}

//SGSN_Capability
asn1::SGSN_Capability::SGSN_Capability(){
	strcpy(node_type_name, "SGSN_Capability");
	// solsaSupportIndicator
	_solsaSupportIndicator = NULL;
	children.push_back(_solsaSupportIndicator);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// superChargerSupportedInServingNetworkEntity
	_superChargerSupportedInServingNetworkEntity = NULL;
	children.push_back(_superChargerSupportedInServingNetworkEntity);

	// gprsEnhancementsSupportIndicator
	_gprsEnhancementsSupportIndicator = NULL;
	children.push_back(_gprsEnhancementsSupportIndicator);

	// supportedCamelPhases
	_supportedCamelPhases = NULL;
	children.push_back(_supportedCamelPhases);

	// supportedLCS_CapabilitySets
	_supportedLCS_CapabilitySets = NULL;
	children.push_back(_supportedLCS_CapabilitySets);

	// offeredCamel4CSIs
	_offeredCamel4CSIs = NULL;
	children.push_back(_offeredCamel4CSIs);

	// smsCallBarringSupportIndicator
	_smsCallBarringSupportIndicator = NULL;
	children.push_back(_smsCallBarringSupportIndicator);

	// supportedRAT_TypesIndicator
	_supportedRAT_TypesIndicator = NULL;
	children.push_back(_supportedRAT_TypesIndicator);

	// supportedFeatures
	_supportedFeatures = NULL;
	children.push_back(_supportedFeatures);

	// t_adsDataRetrieval
	_t_adsDataRetrieval = NULL;
	children.push_back(_t_adsDataRetrieval);

	// homogeneousSupportOfIMSVoiceOverPSSessions
	_homogeneousSupportOfIMSVoiceOverPSSessions = NULL;
	children.push_back(_homogeneousSupportOfIMSVoiceOverPSSessions);

	// cancellationTypeInitialAttach
	_cancellationTypeInitialAttach = NULL;
	children.push_back(_cancellationTypeInitialAttach);

	// additionalMsisdnSupport
	_additionalMsisdnSupport = NULL;
	children.push_back(_additionalMsisdnSupport);

	// msisdn_lessOperation_Supported
	_msisdn_lessOperation_Supported = NULL;
	children.push_back(_msisdn_lessOperation_Supported);


}
asn1::SGSN_Capability::~SGSN_Capability(){

}

asn1::ASN1Node* asn1::SGSN_Capability::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_solsaSupportIndicator = new Null();
			children[0] = _solsaSupportIndicator;
			return _solsaSupportIndicator;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;
                case 2:
                {
                        _superChargerSupportedInServingNetworkEntity = new SuperChargerInfo();
                        ASN1Node* _superChargerSupportedInServingNetworkEntity_wrapper = new ASN1Node();
                        _superChargerSupportedInServingNetworkEntity_wrapper->tlv = new TLVNode();
                        _superChargerSupportedInServingNetworkEntity_wrapper->tlv->is_explicit = true;
                        _superChargerSupportedInServingNetworkEntity_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _superChargerSupportedInServingNetworkEntity_wrapper->tlv->tag_value = 2;
                        _superChargerSupportedInServingNetworkEntity_wrapper->children.push_back(_superChargerSupportedInServingNetworkEntity);
                        children[2] = _superChargerSupportedInServingNetworkEntity_wrapper;
                        return _superChargerSupportedInServingNetworkEntity;
                }
		case 3:
			_gprsEnhancementsSupportIndicator = new Null();
			_gprsEnhancementsSupportIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprsEnhancementsSupportIndicator->tlv->tag_value = 3;
			children[3] = _gprsEnhancementsSupportIndicator;
			return _gprsEnhancementsSupportIndicator;
		case 4:
			_supportedCamelPhases = new SupportedCamelPhases();
			_supportedCamelPhases->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedCamelPhases->tlv->tag_value = 4;
			children[4] = _supportedCamelPhases;
			return _supportedCamelPhases;
		case 5:
			_supportedLCS_CapabilitySets = new SupportedLCS_CapabilitySets();
			_supportedLCS_CapabilitySets->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedLCS_CapabilitySets->tlv->tag_value = 5;
			children[5] = _supportedLCS_CapabilitySets;
			return _supportedLCS_CapabilitySets;
		case 6:
			_offeredCamel4CSIs = new OfferedCamel4CSIs();
			_offeredCamel4CSIs->tlv->tag_class = CONTEXT_SPECIFIC;
			_offeredCamel4CSIs->tlv->tag_value = 6;
			children[6] = _offeredCamel4CSIs;
			return _offeredCamel4CSIs;
		case 7:
			_smsCallBarringSupportIndicator = new Null();
			_smsCallBarringSupportIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_smsCallBarringSupportIndicator->tlv->tag_value = 7;
			children[7] = _smsCallBarringSupportIndicator;
			return _smsCallBarringSupportIndicator;
		case 8:
			_supportedRAT_TypesIndicator = new SupportedRAT_Types();
			_supportedRAT_TypesIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedRAT_TypesIndicator->tlv->tag_value = 8;
			children[8] = _supportedRAT_TypesIndicator;
			return _supportedRAT_TypesIndicator;
		case 9:
			_supportedFeatures = new SupportedFeatures();
			_supportedFeatures->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedFeatures->tlv->tag_value = 9;
			children[9] = _supportedFeatures;
			return _supportedFeatures;
		case 10:
			_t_adsDataRetrieval = new Null();
			_t_adsDataRetrieval->tlv->tag_class = CONTEXT_SPECIFIC;
			_t_adsDataRetrieval->tlv->tag_value = 10;
			children[10] = _t_adsDataRetrieval;
			return _t_adsDataRetrieval;
		case 11:
			_homogeneousSupportOfIMSVoiceOverPSSessions = new Boolean();
			_homogeneousSupportOfIMSVoiceOverPSSessions->tlv->tag_class = CONTEXT_SPECIFIC;
			_homogeneousSupportOfIMSVoiceOverPSSessions->tlv->tag_value = 11;
			children[11] = _homogeneousSupportOfIMSVoiceOverPSSessions;
			return _homogeneousSupportOfIMSVoiceOverPSSessions;
		case 12:
			_cancellationTypeInitialAttach = new Null();
			_cancellationTypeInitialAttach->tlv->tag_class = CONTEXT_SPECIFIC;
			_cancellationTypeInitialAttach->tlv->tag_value = 12;
			children[12] = _cancellationTypeInitialAttach;
			return _cancellationTypeInitialAttach;
		case 13:
			_additionalMsisdnSupport = new Null();
			_additionalMsisdnSupport->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalMsisdnSupport->tlv->tag_value = 13;
			children[13] = _additionalMsisdnSupport;
			return _additionalMsisdnSupport;
		case 14:
			_msisdn_lessOperation_Supported = new Null();
			_msisdn_lessOperation_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn_lessOperation_Supported->tlv->tag_value = 14;
			children[14] = _msisdn_lessOperation_Supported;
			return _msisdn_lessOperation_Supported;

		default: return NULL;
	}
}

void asn1::SGSN_Capability::set_solsaSupportIndicator(){
	if(_solsaSupportIndicator == NULL) _solsaSupportIndicator = (Null*)create_node(0);
}

void asn1::SGSN_Capability::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::SGSN_Capability::set_superChargerSupportedInServingNetworkEntity(){
	if(_superChargerSupportedInServingNetworkEntity == NULL) _superChargerSupportedInServingNetworkEntity = (SuperChargerInfo*)create_node(2);
}

void asn1::SGSN_Capability::set_gprsEnhancementsSupportIndicator(){
	if(_gprsEnhancementsSupportIndicator == NULL) _gprsEnhancementsSupportIndicator = (Null*)create_node(3);
}

void asn1::SGSN_Capability::set_supportedCamelPhases(){
	if(_supportedCamelPhases == NULL) _supportedCamelPhases = (SupportedCamelPhases*)create_node(4);
}

void asn1::SGSN_Capability::set_supportedLCS_CapabilitySets(){
	if(_supportedLCS_CapabilitySets == NULL) _supportedLCS_CapabilitySets = (SupportedLCS_CapabilitySets*)create_node(5);
}

void asn1::SGSN_Capability::set_offeredCamel4CSIs(){
	if(_offeredCamel4CSIs == NULL) _offeredCamel4CSIs = (OfferedCamel4CSIs*)create_node(6);
}

void asn1::SGSN_Capability::set_smsCallBarringSupportIndicator(){
	if(_smsCallBarringSupportIndicator == NULL) _smsCallBarringSupportIndicator = (Null*)create_node(7);
}

void asn1::SGSN_Capability::set_supportedRAT_TypesIndicator(){
	if(_supportedRAT_TypesIndicator == NULL) _supportedRAT_TypesIndicator = (SupportedRAT_Types*)create_node(8);
}

void asn1::SGSN_Capability::set_supportedFeatures(){
	if(_supportedFeatures == NULL) _supportedFeatures = (SupportedFeatures*)create_node(9);
}

void asn1::SGSN_Capability::set_t_adsDataRetrieval(){
	if(_t_adsDataRetrieval == NULL) _t_adsDataRetrieval = (Null*)create_node(10);
}

void asn1::SGSN_Capability::set_homogeneousSupportOfIMSVoiceOverPSSessions(){
	if(_homogeneousSupportOfIMSVoiceOverPSSessions == NULL) _homogeneousSupportOfIMSVoiceOverPSSessions = (Boolean*)create_node(11);
}

void asn1::SGSN_Capability::set_cancellationTypeInitialAttach(){
	if(_cancellationTypeInitialAttach == NULL) _cancellationTypeInitialAttach = (Null*)create_node(12);
}

void asn1::SGSN_Capability::set_additionalMsisdnSupport(){
	if(_additionalMsisdnSupport == NULL) _additionalMsisdnSupport = (Null*)create_node(13);
}

void asn1::SGSN_Capability::set_msisdn_lessOperation_Supported(){
	if(_msisdn_lessOperation_Supported == NULL) _msisdn_lessOperation_Supported = (Null*)create_node(14);
}

//SupportedFeatures
asn1::SupportedFeatures::SupportedFeatures(){
	strcpy(node_type_name, "SupportedFeatures");

}
asn1::SupportedFeatures::~SupportedFeatures(){

}

//UE_SRVCC_Capability
asn1::UE_SRVCC_Capability::UE_SRVCC_Capability(){
	strcpy(node_type_name, "UE_SRVCC_Capability");

}
asn1::UE_SRVCC_Capability::~UE_SRVCC_Capability(){

}

//UpdateGprsLocationRes
asn1::UpdateGprsLocationRes::UpdateGprsLocationRes(){
	strcpy(node_type_name, "UpdateGprsLocationRes");
	// hlr_Number
	_hlr_Number = NULL;
	_hlr_Number = new ISDN_AddressString();
	children.push_back(_hlr_Number);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// add_Capability
	_add_Capability = NULL;
	children.push_back(_add_Capability);

	// sgsn_mmeSeparationSupported
	_sgsn_mmeSeparationSupported = NULL;
	children.push_back(_sgsn_mmeSeparationSupported);

	// mmeRegisteredforSMS
	_mmeRegisteredforSMS = NULL;
	children.push_back(_mmeRegisteredforSMS);


}
asn1::UpdateGprsLocationRes::~UpdateGprsLocationRes(){

}

asn1::ASN1Node* asn1::UpdateGprsLocationRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_add_Capability = new Null();
			children[2] = _add_Capability;
			return _add_Capability;
		case 3:
			_sgsn_mmeSeparationSupported = new Null();
			_sgsn_mmeSeparationSupported->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_mmeSeparationSupported->tlv->tag_value = 0;
			children[3] = _sgsn_mmeSeparationSupported;
			return _sgsn_mmeSeparationSupported;
		case 4:
			_mmeRegisteredforSMS = new Null();
			_mmeRegisteredforSMS->tlv->tag_class = CONTEXT_SPECIFIC;
			_mmeRegisteredforSMS->tlv->tag_value = 1;
			children[4] = _mmeRegisteredforSMS;
			return _mmeRegisteredforSMS;

		default: return NULL;
	}
}

void asn1::UpdateGprsLocationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::UpdateGprsLocationRes::set_add_Capability(){
	if(_add_Capability == NULL) _add_Capability = (Null*)create_node(2);
}

void asn1::UpdateGprsLocationRes::set_sgsn_mmeSeparationSupported(){
	if(_sgsn_mmeSeparationSupported == NULL) _sgsn_mmeSeparationSupported = (Null*)create_node(3);
}

void asn1::UpdateGprsLocationRes::set_mmeRegisteredforSMS(){
	if(_mmeRegisteredforSMS == NULL) _mmeRegisteredforSMS = (Null*)create_node(4);
}

//EPLMN_List
asn1::EPLMN_List::EPLMN_List(){
	strcpy(node_type_name, "EPLMN_List");

}
asn1::EPLMN_List::~EPLMN_List(){
}

asn1::PLMN_Id* asn1::EPLMN_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (PLMN_Id*)children[child_index]; else return NULL;
}

void asn1::EPLMN_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::EPLMN_List::create_node(unsigned int _index){
	children[_index] = new PLMN_Id();
	return children[_index];
}

asn1::ASN1Node* asn1::EPLMN_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new PLMN_Id());
		return children[children.size() - 1];
	}

}

//ForwardAccessSignalling_Arg
asn1::ForwardAccessSignalling_Arg::ForwardAccessSignalling_Arg(){
	strcpy(node_type_name, "ForwardAccessSignalling_Arg");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// an_APDU
	_an_APDU = NULL;
	_an_APDU = new AccessNetworkSignalInfo();
	children.push_back(_an_APDU);

	// integrityProtectionInfo
	_integrityProtectionInfo = NULL;
	children.push_back(_integrityProtectionInfo);

	// encryptionInfo
	_encryptionInfo = NULL;
	children.push_back(_encryptionInfo);

	// keyStatus
	_keyStatus = NULL;
	children.push_back(_keyStatus);

	// allowedGSM_Algorithms
	_allowedGSM_Algorithms = NULL;
	children.push_back(_allowedGSM_Algorithms);

	// allowedUMTS_Algorithms
	_allowedUMTS_Algorithms = NULL;
	children.push_back(_allowedUMTS_Algorithms);

	// radioResourceInformation
	_radioResourceInformation = NULL;
	children.push_back(_radioResourceInformation);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// radioResourceList
	_radioResourceList = NULL;
	children.push_back(_radioResourceList);

	// bssmap_ServiceHandover
	_bssmap_ServiceHandover = NULL;
	children.push_back(_bssmap_ServiceHandover);

	// ranap_ServiceHandover
	_ranap_ServiceHandover = NULL;
	children.push_back(_ranap_ServiceHandover);

	// bssmap_ServiceHandoverList
	_bssmap_ServiceHandoverList = NULL;
	children.push_back(_bssmap_ServiceHandoverList);

	// currentlyUsedCodec
	_currentlyUsedCodec = NULL;
	children.push_back(_currentlyUsedCodec);

	// iuSupportedCodecsList
	_iuSupportedCodecsList = NULL;
	children.push_back(_iuSupportedCodecsList);

	// rab_ConfigurationIndicator
	_rab_ConfigurationIndicator = NULL;
	children.push_back(_rab_ConfigurationIndicator);

	// iuSelectedCodec
	_iuSelectedCodec = NULL;
	children.push_back(_iuSelectedCodec);

	// alternativeChannelType
	_alternativeChannelType = NULL;
	children.push_back(_alternativeChannelType);

	// tracePropagationList
	_tracePropagationList = NULL;
	children.push_back(_tracePropagationList);

	// aoipSupportedCodecsListAnchor
	_aoipSupportedCodecsListAnchor = NULL;
	children.push_back(_aoipSupportedCodecsListAnchor);

	// aoipSelectedCodecTarget
	_aoipSelectedCodecTarget = NULL;
	children.push_back(_aoipSelectedCodecTarget);


}
asn1::ForwardAccessSignalling_Arg::~ForwardAccessSignalling_Arg(){

}

asn1::ASN1Node* asn1::ForwardAccessSignalling_Arg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_integrityProtectionInfo = new IntegrityProtectionInformation();
			_integrityProtectionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_integrityProtectionInfo->tlv->tag_value = 0;
			children[1] = _integrityProtectionInfo;
			return _integrityProtectionInfo;
		case 2:
			_encryptionInfo = new EncryptionInformation();
			_encryptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_encryptionInfo->tlv->tag_value = 1;
			children[2] = _encryptionInfo;
			return _encryptionInfo;
		case 3:
			_keyStatus = new KeyStatus();
			_keyStatus->tlv->tag_class = CONTEXT_SPECIFIC;
			_keyStatus->tlv->tag_value = 2;
			children[3] = _keyStatus;
			return _keyStatus;
		case 4:
			_allowedGSM_Algorithms = new AllowedGSM_Algorithms();
			_allowedGSM_Algorithms->tlv->tag_class = CONTEXT_SPECIFIC;
			_allowedGSM_Algorithms->tlv->tag_value = 4;
			children[4] = _allowedGSM_Algorithms;
			return _allowedGSM_Algorithms;
		case 5:
			_allowedUMTS_Algorithms = new AllowedUMTS_Algorithms();
			_allowedUMTS_Algorithms->tlv->tag_class = CONTEXT_SPECIFIC;
			_allowedUMTS_Algorithms->tlv->tag_value = 5;
			children[5] = _allowedUMTS_Algorithms;
			return _allowedUMTS_Algorithms;
		case 6:
			_radioResourceInformation = new RadioResourceInformation();
			_radioResourceInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_radioResourceInformation->tlv->tag_value = 6;
			children[6] = _radioResourceInformation;
			return _radioResourceInformation;
		case 7:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[7] = _extensionContainer;
			return _extensionContainer;
		case 8:
			_radioResourceList = new RadioResourceList();
			_radioResourceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_radioResourceList->tlv->tag_value = 7;
			children[8] = _radioResourceList;
			return _radioResourceList;
		case 9:
			_bssmap_ServiceHandover = new BSSMAP_ServiceHandover();
			_bssmap_ServiceHandover->tlv->tag_class = CONTEXT_SPECIFIC;
			_bssmap_ServiceHandover->tlv->tag_value = 9;
			children[9] = _bssmap_ServiceHandover;
			return _bssmap_ServiceHandover;
		case 10:
			_ranap_ServiceHandover = new RANAP_ServiceHandover();
			_ranap_ServiceHandover->tlv->tag_class = CONTEXT_SPECIFIC;
			_ranap_ServiceHandover->tlv->tag_value = 8;
			children[10] = _ranap_ServiceHandover;
			return _ranap_ServiceHandover;
		case 11:
			_bssmap_ServiceHandoverList = new BSSMAP_ServiceHandoverList();
			_bssmap_ServiceHandoverList->tlv->tag_class = CONTEXT_SPECIFIC;
			_bssmap_ServiceHandoverList->tlv->tag_value = 10;
			children[11] = _bssmap_ServiceHandoverList;
			return _bssmap_ServiceHandoverList;
		case 12:
			_currentlyUsedCodec = new Codec();
			_currentlyUsedCodec->tlv->tag_class = CONTEXT_SPECIFIC;
			_currentlyUsedCodec->tlv->tag_value = 11;
			children[12] = _currentlyUsedCodec;
			return _currentlyUsedCodec;
		case 13:
			_iuSupportedCodecsList = new SupportedCodecsList();
			_iuSupportedCodecsList->tlv->tag_class = CONTEXT_SPECIFIC;
			_iuSupportedCodecsList->tlv->tag_value = 12;
			children[13] = _iuSupportedCodecsList;
			return _iuSupportedCodecsList;
		case 14:
			_rab_ConfigurationIndicator = new Null();
			_rab_ConfigurationIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_rab_ConfigurationIndicator->tlv->tag_value = 13;
			children[14] = _rab_ConfigurationIndicator;
			return _rab_ConfigurationIndicator;
		case 15:
			_iuSelectedCodec = new Codec();
			_iuSelectedCodec->tlv->tag_class = CONTEXT_SPECIFIC;
			_iuSelectedCodec->tlv->tag_value = 14;
			children[15] = _iuSelectedCodec;
			return _iuSelectedCodec;
		case 16:
			_alternativeChannelType = new RadioResourceInformation();
			_alternativeChannelType->tlv->tag_class = CONTEXT_SPECIFIC;
			_alternativeChannelType->tlv->tag_value = 15;
			children[16] = _alternativeChannelType;
			return _alternativeChannelType;
		case 17:
			_tracePropagationList = new TracePropagationList();
			_tracePropagationList->tlv->tag_class = CONTEXT_SPECIFIC;
			_tracePropagationList->tlv->tag_value = 17;
			children[17] = _tracePropagationList;
			return _tracePropagationList;
		case 18:
			_aoipSupportedCodecsListAnchor = new AoIPCodecsList();
			_aoipSupportedCodecsListAnchor->tlv->tag_class = CONTEXT_SPECIFIC;
			_aoipSupportedCodecsListAnchor->tlv->tag_value = 18;
			children[18] = _aoipSupportedCodecsListAnchor;
			return _aoipSupportedCodecsListAnchor;
		case 19:
			_aoipSelectedCodecTarget = new AoIPCodec();
			_aoipSelectedCodecTarget->tlv->tag_class = CONTEXT_SPECIFIC;
			_aoipSelectedCodecTarget->tlv->tag_value = 19;
			children[19] = _aoipSelectedCodecTarget;
			return _aoipSelectedCodecTarget;

		default: return NULL;
	}
}

void asn1::ForwardAccessSignalling_Arg::set_integrityProtectionInfo(){
	if(_integrityProtectionInfo == NULL) _integrityProtectionInfo = (IntegrityProtectionInformation*)create_node(1);
}

void asn1::ForwardAccessSignalling_Arg::set_encryptionInfo(){
	if(_encryptionInfo == NULL) _encryptionInfo = (EncryptionInformation*)create_node(2);
}

void asn1::ForwardAccessSignalling_Arg::set_keyStatus(){
	if(_keyStatus == NULL) _keyStatus = (KeyStatus*)create_node(3);
}

void asn1::ForwardAccessSignalling_Arg::set_allowedGSM_Algorithms(){
	if(_allowedGSM_Algorithms == NULL) _allowedGSM_Algorithms = (AllowedGSM_Algorithms*)create_node(4);
}

void asn1::ForwardAccessSignalling_Arg::set_allowedUMTS_Algorithms(){
	if(_allowedUMTS_Algorithms == NULL) _allowedUMTS_Algorithms = (AllowedUMTS_Algorithms*)create_node(5);
}

void asn1::ForwardAccessSignalling_Arg::set_radioResourceInformation(){
	if(_radioResourceInformation == NULL) _radioResourceInformation = (RadioResourceInformation*)create_node(6);
}

void asn1::ForwardAccessSignalling_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(7);
}

void asn1::ForwardAccessSignalling_Arg::set_radioResourceList(){
	if(_radioResourceList == NULL) _radioResourceList = (RadioResourceList*)create_node(8);
}

void asn1::ForwardAccessSignalling_Arg::set_bssmap_ServiceHandover(){
	if(_bssmap_ServiceHandover == NULL) _bssmap_ServiceHandover = (BSSMAP_ServiceHandover*)create_node(9);
}

void asn1::ForwardAccessSignalling_Arg::set_ranap_ServiceHandover(){
	if(_ranap_ServiceHandover == NULL) _ranap_ServiceHandover = (RANAP_ServiceHandover*)create_node(10);
}

void asn1::ForwardAccessSignalling_Arg::set_bssmap_ServiceHandoverList(){
	if(_bssmap_ServiceHandoverList == NULL) _bssmap_ServiceHandoverList = (BSSMAP_ServiceHandoverList*)create_node(11);
}

void asn1::ForwardAccessSignalling_Arg::set_currentlyUsedCodec(){
	if(_currentlyUsedCodec == NULL) _currentlyUsedCodec = (Codec*)create_node(12);
}

void asn1::ForwardAccessSignalling_Arg::set_iuSupportedCodecsList(){
	if(_iuSupportedCodecsList == NULL) _iuSupportedCodecsList = (SupportedCodecsList*)create_node(13);
}

void asn1::ForwardAccessSignalling_Arg::set_rab_ConfigurationIndicator(){
	if(_rab_ConfigurationIndicator == NULL) _rab_ConfigurationIndicator = (Null*)create_node(14);
}

void asn1::ForwardAccessSignalling_Arg::set_iuSelectedCodec(){
	if(_iuSelectedCodec == NULL) _iuSelectedCodec = (Codec*)create_node(15);
}

void asn1::ForwardAccessSignalling_Arg::set_alternativeChannelType(){
	if(_alternativeChannelType == NULL) _alternativeChannelType = (RadioResourceInformation*)create_node(16);
}

void asn1::ForwardAccessSignalling_Arg::set_tracePropagationList(){
	if(_tracePropagationList == NULL) _tracePropagationList = (TracePropagationList*)create_node(17);
}

void asn1::ForwardAccessSignalling_Arg::set_aoipSupportedCodecsListAnchor(){
	if(_aoipSupportedCodecsListAnchor == NULL) _aoipSupportedCodecsListAnchor = (AoIPCodecsList*)create_node(18);
}

void asn1::ForwardAccessSignalling_Arg::set_aoipSelectedCodecTarget(){
	if(_aoipSelectedCodecTarget == NULL) _aoipSelectedCodecTarget = (AoIPCodec*)create_node(19);
}

//AllowedGSM_Algorithms
asn1::AllowedGSM_Algorithms::AllowedGSM_Algorithms(){
	strcpy(node_type_name, "AllowedGSM_Algorithms");

}
asn1::AllowedGSM_Algorithms::~AllowedGSM_Algorithms(){

}

//AllowedUMTS_Algorithms
asn1::AllowedUMTS_Algorithms::AllowedUMTS_Algorithms(){
	strcpy(node_type_name, "AllowedUMTS_Algorithms");
	// integrityProtectionAlgorithms
	_integrityProtectionAlgorithms = NULL;
	children.push_back(_integrityProtectionAlgorithms);

	// encryptionAlgorithms
	_encryptionAlgorithms = NULL;
	children.push_back(_encryptionAlgorithms);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::AllowedUMTS_Algorithms::~AllowedUMTS_Algorithms(){

}

asn1::ASN1Node* asn1::AllowedUMTS_Algorithms::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_integrityProtectionAlgorithms = new PermittedIntegrityProtectionAlgorithms();
			_integrityProtectionAlgorithms->tlv->tag_class = CONTEXT_SPECIFIC;
			_integrityProtectionAlgorithms->tlv->tag_value = 0;
			children[0] = _integrityProtectionAlgorithms;
			return _integrityProtectionAlgorithms;
		case 1:
			_encryptionAlgorithms = new PermittedEncryptionAlgorithms();
			_encryptionAlgorithms->tlv->tag_class = CONTEXT_SPECIFIC;
			_encryptionAlgorithms->tlv->tag_value = 1;
			children[1] = _encryptionAlgorithms;
			return _encryptionAlgorithms;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::AllowedUMTS_Algorithms::set_integrityProtectionAlgorithms(){
	if(_integrityProtectionAlgorithms == NULL) _integrityProtectionAlgorithms = (PermittedIntegrityProtectionAlgorithms*)create_node(0);
}

void asn1::AllowedUMTS_Algorithms::set_encryptionAlgorithms(){
	if(_encryptionAlgorithms == NULL) _encryptionAlgorithms = (PermittedEncryptionAlgorithms*)create_node(1);
}

void asn1::AllowedUMTS_Algorithms::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//PermittedIntegrityProtectionAlgorithms
asn1::PermittedIntegrityProtectionAlgorithms::PermittedIntegrityProtectionAlgorithms(){
	strcpy(node_type_name, "PermittedIntegrityProtectionAlgorithms");

}
asn1::PermittedIntegrityProtectionAlgorithms::~PermittedIntegrityProtectionAlgorithms(){

}

//maxPermittedIntegrityProtectionAlgorithmsLength
asn1::maxPermittedIntegrityProtectionAlgorithmsLength::maxPermittedIntegrityProtectionAlgorithmsLength(){
	strcpy(node_type_name, "maxPermittedIntegrityProtectionAlgorithmsLength");

}
asn1::maxPermittedIntegrityProtectionAlgorithmsLength::~maxPermittedIntegrityProtectionAlgorithmsLength(){

}

//PermittedEncryptionAlgorithms
asn1::PermittedEncryptionAlgorithms::PermittedEncryptionAlgorithms(){
	strcpy(node_type_name, "PermittedEncryptionAlgorithms");

}
asn1::PermittedEncryptionAlgorithms::~PermittedEncryptionAlgorithms(){

}

//maxPermittedEncryptionAlgorithmsLength
asn1::maxPermittedEncryptionAlgorithmsLength::maxPermittedEncryptionAlgorithmsLength(){
	strcpy(node_type_name, "maxPermittedEncryptionAlgorithmsLength");

}
asn1::maxPermittedEncryptionAlgorithmsLength::~maxPermittedEncryptionAlgorithmsLength(){

}

//KeyStatus
asn1::KeyStatus::KeyStatus(){
	strcpy(node_type_name, "KeyStatus");

}
asn1::KeyStatus::~KeyStatus(){

}

//PrepareHO_Arg
asn1::PrepareHO_Arg::PrepareHO_Arg(){
	strcpy(node_type_name, "PrepareHO_Arg");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// targetCellId
	_targetCellId = NULL;
	children.push_back(_targetCellId);

	// ho_NumberNotRequired
	_ho_NumberNotRequired = NULL;
	children.push_back(_ho_NumberNotRequired);

	// targetRNCId
	_targetRNCId = NULL;
	children.push_back(_targetRNCId);

	// an_APDU
	_an_APDU = NULL;
	children.push_back(_an_APDU);

	// multipleBearerRequested
	_multipleBearerRequested = NULL;
	children.push_back(_multipleBearerRequested);

	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// integrityProtectionInfo
	_integrityProtectionInfo = NULL;
	children.push_back(_integrityProtectionInfo);

	// encryptionInfo
	_encryptionInfo = NULL;
	children.push_back(_encryptionInfo);

	// radioResourceInformation
	_radioResourceInformation = NULL;
	children.push_back(_radioResourceInformation);

	// allowedGSM_Algorithms
	_allowedGSM_Algorithms = NULL;
	children.push_back(_allowedGSM_Algorithms);

	// allowedUMTS_Algorithms
	_allowedUMTS_Algorithms = NULL;
	children.push_back(_allowedUMTS_Algorithms);

	// radioResourceList
	_radioResourceList = NULL;
	children.push_back(_radioResourceList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// rab_Id
	_rab_Id = NULL;
	children.push_back(_rab_Id);

	// bssmap_ServiceHandover
	_bssmap_ServiceHandover = NULL;
	children.push_back(_bssmap_ServiceHandover);

	// ranap_ServiceHandover
	_ranap_ServiceHandover = NULL;
	children.push_back(_ranap_ServiceHandover);

	// bssmap_ServiceHandoverList
	_bssmap_ServiceHandoverList = NULL;
	children.push_back(_bssmap_ServiceHandoverList);

	// asciCallReference
	_asciCallReference = NULL;
	children.push_back(_asciCallReference);

	// geran_classmark
	_geran_classmark = NULL;
	children.push_back(_geran_classmark);

	// iuCurrentlyUsedCodec
	_iuCurrentlyUsedCodec = NULL;
	children.push_back(_iuCurrentlyUsedCodec);

	// iuSupportedCodecsList
	_iuSupportedCodecsList = NULL;
	children.push_back(_iuSupportedCodecsList);

	// rab_ConfigurationIndicator
	_rab_ConfigurationIndicator = NULL;
	children.push_back(_rab_ConfigurationIndicator);

	// uesbi_Iu
	_uesbi_Iu = NULL;
	children.push_back(_uesbi_Iu);

	// imeisv
	_imeisv = NULL;
	children.push_back(_imeisv);

	// alternativeChannelType
	_alternativeChannelType = NULL;
	children.push_back(_alternativeChannelType);

	// tracePropagationList
	_tracePropagationList = NULL;
	children.push_back(_tracePropagationList);

	// aoipSupportedCodecsListAnchor
	_aoipSupportedCodecsListAnchor = NULL;
	children.push_back(_aoipSupportedCodecsListAnchor);

	// regionalSubscriptionData
	_regionalSubscriptionData = NULL;
	children.push_back(_regionalSubscriptionData);

	// lclsGlobalCallReference
	_lclsGlobalCallReference = NULL;
	children.push_back(_lclsGlobalCallReference);

	// lcls_Negotiation
	_lcls_Negotiation = NULL;
	children.push_back(_lcls_Negotiation);

	// lcls_Configuration_Preference
	_lcls_Configuration_Preference = NULL;
	children.push_back(_lcls_Configuration_Preference);

	// csg_SubscriptionDataList
	_csg_SubscriptionDataList = NULL;
	children.push_back(_csg_SubscriptionDataList);


}
asn1::PrepareHO_Arg::~PrepareHO_Arg(){

}

asn1::ASN1Node* asn1::PrepareHO_Arg::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_targetCellId = new GlobalCellId();
			_targetCellId->tlv->tag_class = CONTEXT_SPECIFIC;
			_targetCellId->tlv->tag_value = 0;
			children[0] = _targetCellId;
			return _targetCellId;
		case 1:
			_ho_NumberNotRequired = new Null();
			children[1] = _ho_NumberNotRequired;
			return _ho_NumberNotRequired;
		case 2:
			_targetRNCId = new RNCId();
			_targetRNCId->tlv->tag_class = CONTEXT_SPECIFIC;
			_targetRNCId->tlv->tag_value = 1;
			children[2] = _targetRNCId;
			return _targetRNCId;
		case 3:
			_an_APDU = new AccessNetworkSignalInfo();
			_an_APDU->tlv->tag_class = CONTEXT_SPECIFIC;
			_an_APDU->tlv->tag_value = 2;
			children[3] = _an_APDU;
			return _an_APDU;
		case 4:
			_multipleBearerRequested = new Null();
			_multipleBearerRequested->tlv->tag_class = CONTEXT_SPECIFIC;
			_multipleBearerRequested->tlv->tag_value = 3;
			children[4] = _multipleBearerRequested;
			return _multipleBearerRequested;
		case 5:
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 4;
			children[5] = _imsi;
			return _imsi;
		case 6:
			_integrityProtectionInfo = new IntegrityProtectionInformation();
			_integrityProtectionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_integrityProtectionInfo->tlv->tag_value = 5;
			children[6] = _integrityProtectionInfo;
			return _integrityProtectionInfo;
		case 7:
			_encryptionInfo = new EncryptionInformation();
			_encryptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_encryptionInfo->tlv->tag_value = 6;
			children[7] = _encryptionInfo;
			return _encryptionInfo;
		case 8:
			_radioResourceInformation = new RadioResourceInformation();
			_radioResourceInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_radioResourceInformation->tlv->tag_value = 7;
			children[8] = _radioResourceInformation;
			return _radioResourceInformation;
		case 9:
			_allowedGSM_Algorithms = new AllowedGSM_Algorithms();
			_allowedGSM_Algorithms->tlv->tag_class = CONTEXT_SPECIFIC;
			_allowedGSM_Algorithms->tlv->tag_value = 9;
			children[9] = _allowedGSM_Algorithms;
			return _allowedGSM_Algorithms;
		case 10:
			_allowedUMTS_Algorithms = new AllowedUMTS_Algorithms();
			_allowedUMTS_Algorithms->tlv->tag_class = CONTEXT_SPECIFIC;
			_allowedUMTS_Algorithms->tlv->tag_value = 10;
			children[10] = _allowedUMTS_Algorithms;
			return _allowedUMTS_Algorithms;
		case 11:
			_radioResourceList = new RadioResourceList();
			_radioResourceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_radioResourceList->tlv->tag_value = 11;
			children[11] = _radioResourceList;
			return _radioResourceList;
		case 12:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 8;
			children[12] = _extensionContainer;
			return _extensionContainer;
		case 13:
			_rab_Id = new RAB_Id();
			_rab_Id->tlv->tag_class = CONTEXT_SPECIFIC;
			_rab_Id->tlv->tag_value = 12;
			children[13] = _rab_Id;
			return _rab_Id;
		case 14:
			_bssmap_ServiceHandover = new BSSMAP_ServiceHandover();
			_bssmap_ServiceHandover->tlv->tag_class = CONTEXT_SPECIFIC;
			_bssmap_ServiceHandover->tlv->tag_value = 13;
			children[14] = _bssmap_ServiceHandover;
			return _bssmap_ServiceHandover;
		case 15:
			_ranap_ServiceHandover = new RANAP_ServiceHandover();
			_ranap_ServiceHandover->tlv->tag_class = CONTEXT_SPECIFIC;
			_ranap_ServiceHandover->tlv->tag_value = 14;
			children[15] = _ranap_ServiceHandover;
			return _ranap_ServiceHandover;
		case 16:
			_bssmap_ServiceHandoverList = new BSSMAP_ServiceHandoverList();
			_bssmap_ServiceHandoverList->tlv->tag_class = CONTEXT_SPECIFIC;
			_bssmap_ServiceHandoverList->tlv->tag_value = 15;
			children[16] = _bssmap_ServiceHandoverList;
			return _bssmap_ServiceHandoverList;
		case 17:
			_asciCallReference = new ASCI_CallReference();
			_asciCallReference->tlv->tag_class = CONTEXT_SPECIFIC;
			_asciCallReference->tlv->tag_value = 20;
			children[17] = _asciCallReference;
			return _asciCallReference;
		case 18:
			_geran_classmark = new GERAN_Classmark();
			_geran_classmark->tlv->tag_class = CONTEXT_SPECIFIC;
			_geran_classmark->tlv->tag_value = 16;
			children[18] = _geran_classmark;
			return _geran_classmark;
		case 19:
			_iuCurrentlyUsedCodec = new Codec();
			_iuCurrentlyUsedCodec->tlv->tag_class = CONTEXT_SPECIFIC;
			_iuCurrentlyUsedCodec->tlv->tag_value = 17;
			children[19] = _iuCurrentlyUsedCodec;
			return _iuCurrentlyUsedCodec;
		case 20:
			_iuSupportedCodecsList = new SupportedCodecsList();
			_iuSupportedCodecsList->tlv->tag_class = CONTEXT_SPECIFIC;
			_iuSupportedCodecsList->tlv->tag_value = 18;
			children[20] = _iuSupportedCodecsList;
			return _iuSupportedCodecsList;
		case 21:
			_rab_ConfigurationIndicator = new Null();
			_rab_ConfigurationIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_rab_ConfigurationIndicator->tlv->tag_value = 19;
			children[21] = _rab_ConfigurationIndicator;
			return _rab_ConfigurationIndicator;
		case 22:
			_uesbi_Iu = new UESBI_Iu();
			_uesbi_Iu->tlv->tag_class = CONTEXT_SPECIFIC;
			_uesbi_Iu->tlv->tag_value = 21;
			children[22] = _uesbi_Iu;
			return _uesbi_Iu;
		case 23:
			_imeisv = new IMEI();
			_imeisv->tlv->tag_class = CONTEXT_SPECIFIC;
			_imeisv->tlv->tag_value = 22;
			children[23] = _imeisv;
			return _imeisv;
		case 24:
			_alternativeChannelType = new RadioResourceInformation();
			_alternativeChannelType->tlv->tag_class = CONTEXT_SPECIFIC;
			_alternativeChannelType->tlv->tag_value = 23;
			children[24] = _alternativeChannelType;
			return _alternativeChannelType;
		case 25:
			_tracePropagationList = new TracePropagationList();
			_tracePropagationList->tlv->tag_class = CONTEXT_SPECIFIC;
			_tracePropagationList->tlv->tag_value = 25;
			children[25] = _tracePropagationList;
			return _tracePropagationList;
		case 26:
			_aoipSupportedCodecsListAnchor = new AoIPCodecsList();
			_aoipSupportedCodecsListAnchor->tlv->tag_class = CONTEXT_SPECIFIC;
			_aoipSupportedCodecsListAnchor->tlv->tag_value = 26;
			children[26] = _aoipSupportedCodecsListAnchor;
			return _aoipSupportedCodecsListAnchor;
		case 27:
			_regionalSubscriptionData = new ZoneCodeList();
			_regionalSubscriptionData->tlv->tag_class = CONTEXT_SPECIFIC;
			_regionalSubscriptionData->tlv->tag_value = 27;
			children[27] = _regionalSubscriptionData;
			return _regionalSubscriptionData;
		case 28:
			_lclsGlobalCallReference = new LCLS_GlobalCallReference();
			_lclsGlobalCallReference->tlv->tag_class = CONTEXT_SPECIFIC;
			_lclsGlobalCallReference->tlv->tag_value = 28;
			children[28] = _lclsGlobalCallReference;
			return _lclsGlobalCallReference;
		case 29:
			_lcls_Negotiation = new LCLS_Negotiation();
			_lcls_Negotiation->tlv->tag_class = CONTEXT_SPECIFIC;
			_lcls_Negotiation->tlv->tag_value = 29;
			children[29] = _lcls_Negotiation;
			return _lcls_Negotiation;
		case 30:
			_lcls_Configuration_Preference = new LCLS_ConfigurationPreference();
			_lcls_Configuration_Preference->tlv->tag_class = CONTEXT_SPECIFIC;
			_lcls_Configuration_Preference->tlv->tag_value = 30;
			children[30] = _lcls_Configuration_Preference;
			return _lcls_Configuration_Preference;
		case 31:
			_csg_SubscriptionDataList = new CSG_SubscriptionDataList();
			_csg_SubscriptionDataList->tlv->tag_class = CONTEXT_SPECIFIC;
			_csg_SubscriptionDataList->tlv->tag_value = 31;
			children[31] = _csg_SubscriptionDataList;
			return _csg_SubscriptionDataList;

		default: return NULL;
	}
}

void asn1::PrepareHO_Arg::set_targetCellId(){
	if(_targetCellId == NULL) _targetCellId = (GlobalCellId*)create_node(0);
}

void asn1::PrepareHO_Arg::set_ho_NumberNotRequired(){
	if(_ho_NumberNotRequired == NULL) _ho_NumberNotRequired = (Null*)create_node(1);
}

void asn1::PrepareHO_Arg::set_targetRNCId(){
	if(_targetRNCId == NULL) _targetRNCId = (RNCId*)create_node(2);
}

void asn1::PrepareHO_Arg::set_an_APDU(){
	if(_an_APDU == NULL) _an_APDU = (AccessNetworkSignalInfo*)create_node(3);
}

void asn1::PrepareHO_Arg::set_multipleBearerRequested(){
	if(_multipleBearerRequested == NULL) _multipleBearerRequested = (Null*)create_node(4);
}

void asn1::PrepareHO_Arg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(5);
}

void asn1::PrepareHO_Arg::set_integrityProtectionInfo(){
	if(_integrityProtectionInfo == NULL) _integrityProtectionInfo = (IntegrityProtectionInformation*)create_node(6);
}

void asn1::PrepareHO_Arg::set_encryptionInfo(){
	if(_encryptionInfo == NULL) _encryptionInfo = (EncryptionInformation*)create_node(7);
}

void asn1::PrepareHO_Arg::set_radioResourceInformation(){
	if(_radioResourceInformation == NULL) _radioResourceInformation = (RadioResourceInformation*)create_node(8);
}

void asn1::PrepareHO_Arg::set_allowedGSM_Algorithms(){
	if(_allowedGSM_Algorithms == NULL) _allowedGSM_Algorithms = (AllowedGSM_Algorithms*)create_node(9);
}

void asn1::PrepareHO_Arg::set_allowedUMTS_Algorithms(){
	if(_allowedUMTS_Algorithms == NULL) _allowedUMTS_Algorithms = (AllowedUMTS_Algorithms*)create_node(10);
}

void asn1::PrepareHO_Arg::set_radioResourceList(){
	if(_radioResourceList == NULL) _radioResourceList = (RadioResourceList*)create_node(11);
}

void asn1::PrepareHO_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(12);
}

void asn1::PrepareHO_Arg::set_rab_Id(){
	if(_rab_Id == NULL) _rab_Id = (RAB_Id*)create_node(13);
}

void asn1::PrepareHO_Arg::set_bssmap_ServiceHandover(){
	if(_bssmap_ServiceHandover == NULL) _bssmap_ServiceHandover = (BSSMAP_ServiceHandover*)create_node(14);
}

void asn1::PrepareHO_Arg::set_ranap_ServiceHandover(){
	if(_ranap_ServiceHandover == NULL) _ranap_ServiceHandover = (RANAP_ServiceHandover*)create_node(15);
}

void asn1::PrepareHO_Arg::set_bssmap_ServiceHandoverList(){
	if(_bssmap_ServiceHandoverList == NULL) _bssmap_ServiceHandoverList = (BSSMAP_ServiceHandoverList*)create_node(16);
}

void asn1::PrepareHO_Arg::set_asciCallReference(){
	if(_asciCallReference == NULL) _asciCallReference = (ASCI_CallReference*)create_node(17);
}

void asn1::PrepareHO_Arg::set_geran_classmark(){
	if(_geran_classmark == NULL) _geran_classmark = (GERAN_Classmark*)create_node(18);
}

void asn1::PrepareHO_Arg::set_iuCurrentlyUsedCodec(){
	if(_iuCurrentlyUsedCodec == NULL) _iuCurrentlyUsedCodec = (Codec*)create_node(19);
}

void asn1::PrepareHO_Arg::set_iuSupportedCodecsList(){
	if(_iuSupportedCodecsList == NULL) _iuSupportedCodecsList = (SupportedCodecsList*)create_node(20);
}

void asn1::PrepareHO_Arg::set_rab_ConfigurationIndicator(){
	if(_rab_ConfigurationIndicator == NULL) _rab_ConfigurationIndicator = (Null*)create_node(21);
}

void asn1::PrepareHO_Arg::set_uesbi_Iu(){
	if(_uesbi_Iu == NULL) _uesbi_Iu = (UESBI_Iu*)create_node(22);
}

void asn1::PrepareHO_Arg::set_imeisv(){
	if(_imeisv == NULL) _imeisv = (IMEI*)create_node(23);
}

void asn1::PrepareHO_Arg::set_alternativeChannelType(){
	if(_alternativeChannelType == NULL) _alternativeChannelType = (RadioResourceInformation*)create_node(24);
}

void asn1::PrepareHO_Arg::set_tracePropagationList(){
	if(_tracePropagationList == NULL) _tracePropagationList = (TracePropagationList*)create_node(25);
}

void asn1::PrepareHO_Arg::set_aoipSupportedCodecsListAnchor(){
	if(_aoipSupportedCodecsListAnchor == NULL) _aoipSupportedCodecsListAnchor = (AoIPCodecsList*)create_node(26);
}

void asn1::PrepareHO_Arg::set_regionalSubscriptionData(){
	if(_regionalSubscriptionData == NULL) _regionalSubscriptionData = (ZoneCodeList*)create_node(27);
}

void asn1::PrepareHO_Arg::set_lclsGlobalCallReference(){
	if(_lclsGlobalCallReference == NULL) _lclsGlobalCallReference = (LCLS_GlobalCallReference*)create_node(28);
}

void asn1::PrepareHO_Arg::set_lcls_Negotiation(){
	if(_lcls_Negotiation == NULL) _lcls_Negotiation = (LCLS_Negotiation*)create_node(29);
}

void asn1::PrepareHO_Arg::set_lcls_Configuration_Preference(){
	if(_lcls_Configuration_Preference == NULL) _lcls_Configuration_Preference = (LCLS_ConfigurationPreference*)create_node(30);
}

void asn1::PrepareHO_Arg::set_csg_SubscriptionDataList(){
	if(_csg_SubscriptionDataList == NULL) _csg_SubscriptionDataList = (CSG_SubscriptionDataList*)create_node(31);
}

//LCLS_GlobalCallReference
asn1::LCLS_GlobalCallReference::LCLS_GlobalCallReference(){
	strcpy(node_type_name, "LCLS_GlobalCallReference");

}
asn1::LCLS_GlobalCallReference::~LCLS_GlobalCallReference(){

}

//LCLS_Negotiation
asn1::LCLS_Negotiation::LCLS_Negotiation(){
	strcpy(node_type_name, "LCLS_Negotiation");

}
asn1::LCLS_Negotiation::~LCLS_Negotiation(){

}

//LCLS_ConfigurationPreference
asn1::LCLS_ConfigurationPreference::LCLS_ConfigurationPreference(){
	strcpy(node_type_name, "LCLS_ConfigurationPreference");

}
asn1::LCLS_ConfigurationPreference::~LCLS_ConfigurationPreference(){

}

//BSSMAP_ServiceHandoverList
asn1::BSSMAP_ServiceHandoverList::BSSMAP_ServiceHandoverList(){
	strcpy(node_type_name, "BSSMAP_ServiceHandoverList");

}
asn1::BSSMAP_ServiceHandoverList::~BSSMAP_ServiceHandoverList(){
}

asn1::BSSMAP_ServiceHandoverInfo* asn1::BSSMAP_ServiceHandoverList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (BSSMAP_ServiceHandoverInfo*)children[child_index]; else return NULL;
}

void asn1::BSSMAP_ServiceHandoverList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::BSSMAP_ServiceHandoverList::create_node(unsigned int _index){
	children[_index] = new BSSMAP_ServiceHandoverInfo();
	return children[_index];
}

asn1::ASN1Node* asn1::BSSMAP_ServiceHandoverList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new BSSMAP_ServiceHandoverInfo());
		return children[children.size() - 1];
	}

}

//BSSMAP_ServiceHandoverInfo
asn1::BSSMAP_ServiceHandoverInfo::BSSMAP_ServiceHandoverInfo(){
	strcpy(node_type_name, "BSSMAP_ServiceHandoverInfo");
	// bssmap_ServiceHandover
	_bssmap_ServiceHandover = NULL;
	_bssmap_ServiceHandover = new BSSMAP_ServiceHandover();
	children.push_back(_bssmap_ServiceHandover);

	// rab_Id
	_rab_Id = NULL;
	_rab_Id = new RAB_Id();
	children.push_back(_rab_Id);


}
asn1::BSSMAP_ServiceHandoverInfo::~BSSMAP_ServiceHandoverInfo(){

}

//maxNumOfServiceHandovers
asn1::maxNumOfServiceHandovers::maxNumOfServiceHandovers(){
	strcpy(node_type_name, "maxNumOfServiceHandovers");

}
asn1::maxNumOfServiceHandovers::~maxNumOfServiceHandovers(){

}

//BSSMAP_ServiceHandover
asn1::BSSMAP_ServiceHandover::BSSMAP_ServiceHandover(){
	strcpy(node_type_name, "BSSMAP_ServiceHandover");

}
asn1::BSSMAP_ServiceHandover::~BSSMAP_ServiceHandover(){

}

//RANAP_ServiceHandover
asn1::RANAP_ServiceHandover::RANAP_ServiceHandover(){
	strcpy(node_type_name, "RANAP_ServiceHandover");

}
asn1::RANAP_ServiceHandover::~RANAP_ServiceHandover(){

}

//RadioResourceList
asn1::RadioResourceList::RadioResourceList(){
	strcpy(node_type_name, "RadioResourceList");

}
asn1::RadioResourceList::~RadioResourceList(){
}

asn1::RadioResource* asn1::RadioResourceList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (RadioResource*)children[child_index]; else return NULL;
}

void asn1::RadioResourceList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::RadioResourceList::create_node(unsigned int _index){
	children[_index] = new RadioResource();
	return children[_index];
}

asn1::ASN1Node* asn1::RadioResourceList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new RadioResource());
		return children[children.size() - 1];
	}

}

//RadioResource
asn1::RadioResource::RadioResource(){
	strcpy(node_type_name, "RadioResource");
	// radioResourceInformation
	_radioResourceInformation = NULL;
	_radioResourceInformation = new RadioResourceInformation();
	children.push_back(_radioResourceInformation);

	// rab_Id
	_rab_Id = NULL;
	_rab_Id = new RAB_Id();
	children.push_back(_rab_Id);


}
asn1::RadioResource::~RadioResource(){

}

//maxNumOfRadioResources
asn1::maxNumOfRadioResources::maxNumOfRadioResources(){
	strcpy(node_type_name, "maxNumOfRadioResources");

}
asn1::maxNumOfRadioResources::~maxNumOfRadioResources(){

}

//PrepareHO_Res
asn1::PrepareHO_Res::PrepareHO_Res(){
	strcpy(node_type_name, "PrepareHO_Res");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// handoverNumber
	_handoverNumber = NULL;
	children.push_back(_handoverNumber);

	// relocationNumberList
	_relocationNumberList = NULL;
	children.push_back(_relocationNumberList);

	// an_APDU
	_an_APDU = NULL;
	children.push_back(_an_APDU);

	// multicallBearerInfo
	_multicallBearerInfo = NULL;
	children.push_back(_multicallBearerInfo);

	// multipleBearerNotSupported
	_multipleBearerNotSupported = NULL;
	children.push_back(_multipleBearerNotSupported);

	// selectedUMTS_Algorithms
	_selectedUMTS_Algorithms = NULL;
	children.push_back(_selectedUMTS_Algorithms);

	// chosenRadioResourceInformation
	_chosenRadioResourceInformation = NULL;
	children.push_back(_chosenRadioResourceInformation);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// iuSelectedCodec
	_iuSelectedCodec = NULL;
	children.push_back(_iuSelectedCodec);

	// iuAvailableCodecsList
	_iuAvailableCodecsList = NULL;
	children.push_back(_iuAvailableCodecsList);

	// aoipSelectedCodecTarget
	_aoipSelectedCodecTarget = NULL;
	children.push_back(_aoipSelectedCodecTarget);

	// aoipAvailableCodecsListMap
	_aoipAvailableCodecsListMap = NULL;
	children.push_back(_aoipAvailableCodecsListMap);


}
asn1::PrepareHO_Res::~PrepareHO_Res(){

}

asn1::ASN1Node* asn1::PrepareHO_Res::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_handoverNumber = new ISDN_AddressString();
			_handoverNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_handoverNumber->tlv->tag_value = 0;
			children[0] = _handoverNumber;
			return _handoverNumber;
		case 1:
			_relocationNumberList = new RelocationNumberList();
			_relocationNumberList->tlv->tag_class = CONTEXT_SPECIFIC;
			_relocationNumberList->tlv->tag_value = 1;
			children[1] = _relocationNumberList;
			return _relocationNumberList;
		case 2:
			_an_APDU = new AccessNetworkSignalInfo();
			_an_APDU->tlv->tag_class = CONTEXT_SPECIFIC;
			_an_APDU->tlv->tag_value = 2;
			children[2] = _an_APDU;
			return _an_APDU;
		case 3:
			_multicallBearerInfo = new MulticallBearerInfo();
			_multicallBearerInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_multicallBearerInfo->tlv->tag_value = 3;
			children[3] = _multicallBearerInfo;
			return _multicallBearerInfo;
		case 4:
			_multipleBearerNotSupported = new Null();
			children[4] = _multipleBearerNotSupported;
			return _multipleBearerNotSupported;
		case 5:
			_selectedUMTS_Algorithms = new SelectedUMTS_Algorithms();
			_selectedUMTS_Algorithms->tlv->tag_class = CONTEXT_SPECIFIC;
			_selectedUMTS_Algorithms->tlv->tag_value = 5;
			children[5] = _selectedUMTS_Algorithms;
			return _selectedUMTS_Algorithms;
		case 6:
			_chosenRadioResourceInformation = new ChosenRadioResourceInformation();
			_chosenRadioResourceInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_chosenRadioResourceInformation->tlv->tag_value = 6;
			children[6] = _chosenRadioResourceInformation;
			return _chosenRadioResourceInformation;
		case 7:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 4;
			children[7] = _extensionContainer;
			return _extensionContainer;
		case 8:
			_iuSelectedCodec = new Codec();
			_iuSelectedCodec->tlv->tag_class = CONTEXT_SPECIFIC;
			_iuSelectedCodec->tlv->tag_value = 7;
			children[8] = _iuSelectedCodec;
			return _iuSelectedCodec;
		case 9:
			_iuAvailableCodecsList = new CodecList();
			_iuAvailableCodecsList->tlv->tag_class = CONTEXT_SPECIFIC;
			_iuAvailableCodecsList->tlv->tag_value = 8;
			children[9] = _iuAvailableCodecsList;
			return _iuAvailableCodecsList;
		case 10:
			_aoipSelectedCodecTarget = new AoIPCodec();
			_aoipSelectedCodecTarget->tlv->tag_class = CONTEXT_SPECIFIC;
			_aoipSelectedCodecTarget->tlv->tag_value = 9;
			children[10] = _aoipSelectedCodecTarget;
			return _aoipSelectedCodecTarget;
		case 11:
			_aoipAvailableCodecsListMap = new AoIPCodecsList();
			_aoipAvailableCodecsListMap->tlv->tag_class = CONTEXT_SPECIFIC;
			_aoipAvailableCodecsListMap->tlv->tag_value = 10;
			children[11] = _aoipAvailableCodecsListMap;
			return _aoipAvailableCodecsListMap;

		default: return NULL;
	}
}

void asn1::PrepareHO_Res::set_handoverNumber(){
	if(_handoverNumber == NULL) _handoverNumber = (ISDN_AddressString*)create_node(0);
}

void asn1::PrepareHO_Res::set_relocationNumberList(){
	if(_relocationNumberList == NULL) _relocationNumberList = (RelocationNumberList*)create_node(1);
}

void asn1::PrepareHO_Res::set_an_APDU(){
	if(_an_APDU == NULL) _an_APDU = (AccessNetworkSignalInfo*)create_node(2);
}

void asn1::PrepareHO_Res::set_multicallBearerInfo(){
	if(_multicallBearerInfo == NULL) _multicallBearerInfo = (MulticallBearerInfo*)create_node(3);
}

void asn1::PrepareHO_Res::set_multipleBearerNotSupported(){
	if(_multipleBearerNotSupported == NULL) _multipleBearerNotSupported = (Null*)create_node(4);
}

void asn1::PrepareHO_Res::set_selectedUMTS_Algorithms(){
	if(_selectedUMTS_Algorithms == NULL) _selectedUMTS_Algorithms = (SelectedUMTS_Algorithms*)create_node(5);
}

void asn1::PrepareHO_Res::set_chosenRadioResourceInformation(){
	if(_chosenRadioResourceInformation == NULL) _chosenRadioResourceInformation = (ChosenRadioResourceInformation*)create_node(6);
}

void asn1::PrepareHO_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(7);
}

void asn1::PrepareHO_Res::set_iuSelectedCodec(){
	if(_iuSelectedCodec == NULL) _iuSelectedCodec = (Codec*)create_node(8);
}

void asn1::PrepareHO_Res::set_iuAvailableCodecsList(){
	if(_iuAvailableCodecsList == NULL) _iuAvailableCodecsList = (CodecList*)create_node(9);
}

void asn1::PrepareHO_Res::set_aoipSelectedCodecTarget(){
	if(_aoipSelectedCodecTarget == NULL) _aoipSelectedCodecTarget = (AoIPCodec*)create_node(10);
}

void asn1::PrepareHO_Res::set_aoipAvailableCodecsListMap(){
	if(_aoipAvailableCodecsListMap == NULL) _aoipAvailableCodecsListMap = (AoIPCodecsList*)create_node(11);
}

//SelectedUMTS_Algorithms
asn1::SelectedUMTS_Algorithms::SelectedUMTS_Algorithms(){
	strcpy(node_type_name, "SelectedUMTS_Algorithms");
	// integrityProtectionAlgorithm
	_integrityProtectionAlgorithm = NULL;
	children.push_back(_integrityProtectionAlgorithm);

	// encryptionAlgorithm
	_encryptionAlgorithm = NULL;
	children.push_back(_encryptionAlgorithm);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SelectedUMTS_Algorithms::~SelectedUMTS_Algorithms(){

}

asn1::ASN1Node* asn1::SelectedUMTS_Algorithms::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_integrityProtectionAlgorithm = new ChosenIntegrityProtectionAlgorithm();
			_integrityProtectionAlgorithm->tlv->tag_class = CONTEXT_SPECIFIC;
			_integrityProtectionAlgorithm->tlv->tag_value = 0;
			children[0] = _integrityProtectionAlgorithm;
			return _integrityProtectionAlgorithm;
		case 1:
			_encryptionAlgorithm = new ChosenEncryptionAlgorithm();
			_encryptionAlgorithm->tlv->tag_class = CONTEXT_SPECIFIC;
			_encryptionAlgorithm->tlv->tag_value = 1;
			children[1] = _encryptionAlgorithm;
			return _encryptionAlgorithm;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SelectedUMTS_Algorithms::set_integrityProtectionAlgorithm(){
	if(_integrityProtectionAlgorithm == NULL) _integrityProtectionAlgorithm = (ChosenIntegrityProtectionAlgorithm*)create_node(0);
}

void asn1::SelectedUMTS_Algorithms::set_encryptionAlgorithm(){
	if(_encryptionAlgorithm == NULL) _encryptionAlgorithm = (ChosenEncryptionAlgorithm*)create_node(1);
}

void asn1::SelectedUMTS_Algorithms::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//ChosenIntegrityProtectionAlgorithm
asn1::ChosenIntegrityProtectionAlgorithm::ChosenIntegrityProtectionAlgorithm(){
	strcpy(node_type_name, "ChosenIntegrityProtectionAlgorithm");

}
asn1::ChosenIntegrityProtectionAlgorithm::~ChosenIntegrityProtectionAlgorithm(){

}

//ChosenEncryptionAlgorithm
asn1::ChosenEncryptionAlgorithm::ChosenEncryptionAlgorithm(){
	strcpy(node_type_name, "ChosenEncryptionAlgorithm");

}
asn1::ChosenEncryptionAlgorithm::~ChosenEncryptionAlgorithm(){

}

//ChosenRadioResourceInformation
asn1::ChosenRadioResourceInformation::ChosenRadioResourceInformation(){
	strcpy(node_type_name, "ChosenRadioResourceInformation");
	// chosenChannelInfo
	_chosenChannelInfo = NULL;
	children.push_back(_chosenChannelInfo);

	// chosenSpeechVersion
	_chosenSpeechVersion = NULL;
	children.push_back(_chosenSpeechVersion);


}
asn1::ChosenRadioResourceInformation::~ChosenRadioResourceInformation(){

}

asn1::ASN1Node* asn1::ChosenRadioResourceInformation::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_chosenChannelInfo = new ChosenChannelInfo();
			_chosenChannelInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_chosenChannelInfo->tlv->tag_value = 0;
			children[0] = _chosenChannelInfo;
			return _chosenChannelInfo;
		case 1:
			_chosenSpeechVersion = new ChosenSpeechVersion();
			_chosenSpeechVersion->tlv->tag_class = CONTEXT_SPECIFIC;
			_chosenSpeechVersion->tlv->tag_value = 1;
			children[1] = _chosenSpeechVersion;
			return _chosenSpeechVersion;

		default: return NULL;
	}
}

void asn1::ChosenRadioResourceInformation::set_chosenChannelInfo(){
	if(_chosenChannelInfo == NULL) _chosenChannelInfo = (ChosenChannelInfo*)create_node(0);
}

void asn1::ChosenRadioResourceInformation::set_chosenSpeechVersion(){
	if(_chosenSpeechVersion == NULL) _chosenSpeechVersion = (ChosenSpeechVersion*)create_node(1);
}

//ChosenChannelInfo
asn1::ChosenChannelInfo::ChosenChannelInfo(){
	strcpy(node_type_name, "ChosenChannelInfo");

}
asn1::ChosenChannelInfo::~ChosenChannelInfo(){

}

//ChosenSpeechVersion
asn1::ChosenSpeechVersion::ChosenSpeechVersion(){
	strcpy(node_type_name, "ChosenSpeechVersion");

}
asn1::ChosenSpeechVersion::~ChosenSpeechVersion(){

}

//PrepareSubsequentHO_Arg
asn1::PrepareSubsequentHO_Arg::PrepareSubsequentHO_Arg(){
	strcpy(node_type_name, "PrepareSubsequentHO_Arg");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// targetCellId
	_targetCellId = NULL;
	children.push_back(_targetCellId);

	// targetMSC_Number
	_targetMSC_Number = NULL;
	_targetMSC_Number = new ISDN_AddressString();
	_targetMSC_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_targetMSC_Number->tlv->tag_value = 1;
	children.push_back(_targetMSC_Number);

	// targetRNCId
	_targetRNCId = NULL;
	children.push_back(_targetRNCId);

	// an_APDU
	_an_APDU = NULL;
	children.push_back(_an_APDU);

	// selectedRab_Id
	_selectedRab_Id = NULL;
	children.push_back(_selectedRab_Id);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// geran_classmark
	_geran_classmark = NULL;
	children.push_back(_geran_classmark);

	// rab_ConfigurationIndicator
	_rab_ConfigurationIndicator = NULL;
	children.push_back(_rab_ConfigurationIndicator);


}
asn1::PrepareSubsequentHO_Arg::~PrepareSubsequentHO_Arg(){

}

asn1::ASN1Node* asn1::PrepareSubsequentHO_Arg::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_targetCellId = new GlobalCellId();
			_targetCellId->tlv->tag_class = CONTEXT_SPECIFIC;
			_targetCellId->tlv->tag_value = 0;
			children[0] = _targetCellId;
			return _targetCellId;
		case 2:
			_targetRNCId = new RNCId();
			_targetRNCId->tlv->tag_class = CONTEXT_SPECIFIC;
			_targetRNCId->tlv->tag_value = 2;
			children[2] = _targetRNCId;
			return _targetRNCId;
		case 3:
			_an_APDU = new AccessNetworkSignalInfo();
			_an_APDU->tlv->tag_class = CONTEXT_SPECIFIC;
			_an_APDU->tlv->tag_value = 3;
			children[3] = _an_APDU;
			return _an_APDU;
		case 4:
			_selectedRab_Id = new RAB_Id();
			_selectedRab_Id->tlv->tag_class = CONTEXT_SPECIFIC;
			_selectedRab_Id->tlv->tag_value = 4;
			children[4] = _selectedRab_Id;
			return _selectedRab_Id;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 5;
			children[5] = _extensionContainer;
			return _extensionContainer;
		case 6:
			_geran_classmark = new GERAN_Classmark();
			_geran_classmark->tlv->tag_class = CONTEXT_SPECIFIC;
			_geran_classmark->tlv->tag_value = 6;
			children[6] = _geran_classmark;
			return _geran_classmark;
		case 7:
			_rab_ConfigurationIndicator = new Null();
			_rab_ConfigurationIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_rab_ConfigurationIndicator->tlv->tag_value = 7;
			children[7] = _rab_ConfigurationIndicator;
			return _rab_ConfigurationIndicator;

		default: return NULL;
	}
}

void asn1::PrepareSubsequentHO_Arg::set_targetCellId(){
	if(_targetCellId == NULL) _targetCellId = (GlobalCellId*)create_node(0);
}

void asn1::PrepareSubsequentHO_Arg::set_targetRNCId(){
	if(_targetRNCId == NULL) _targetRNCId = (RNCId*)create_node(2);
}

void asn1::PrepareSubsequentHO_Arg::set_an_APDU(){
	if(_an_APDU == NULL) _an_APDU = (AccessNetworkSignalInfo*)create_node(3);
}

void asn1::PrepareSubsequentHO_Arg::set_selectedRab_Id(){
	if(_selectedRab_Id == NULL) _selectedRab_Id = (RAB_Id*)create_node(4);
}

void asn1::PrepareSubsequentHO_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

void asn1::PrepareSubsequentHO_Arg::set_geran_classmark(){
	if(_geran_classmark == NULL) _geran_classmark = (GERAN_Classmark*)create_node(6);
}

void asn1::PrepareSubsequentHO_Arg::set_rab_ConfigurationIndicator(){
	if(_rab_ConfigurationIndicator == NULL) _rab_ConfigurationIndicator = (Null*)create_node(7);
}

//PrepareSubsequentHO_Res
asn1::PrepareSubsequentHO_Res::PrepareSubsequentHO_Res(){
	strcpy(node_type_name, "PrepareSubsequentHO_Res");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// an_APDU
	_an_APDU = NULL;
	_an_APDU = new AccessNetworkSignalInfo();
	children.push_back(_an_APDU);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::PrepareSubsequentHO_Res::~PrepareSubsequentHO_Res(){

}

asn1::ASN1Node* asn1::PrepareSubsequentHO_Res::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::PrepareSubsequentHO_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//ProcessAccessSignalling_Arg
asn1::ProcessAccessSignalling_Arg::ProcessAccessSignalling_Arg(){
	strcpy(node_type_name, "ProcessAccessSignalling_Arg");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// an_APDU
	_an_APDU = NULL;
	_an_APDU = new AccessNetworkSignalInfo();
	children.push_back(_an_APDU);

	// selectedUMTS_Algorithms
	_selectedUMTS_Algorithms = NULL;
	children.push_back(_selectedUMTS_Algorithms);

	// selectedGSM_Algorithm
	_selectedGSM_Algorithm = NULL;
	children.push_back(_selectedGSM_Algorithm);

	// chosenRadioResourceInformation
	_chosenRadioResourceInformation = NULL;
	children.push_back(_chosenRadioResourceInformation);

	// selectedRab_Id
	_selectedRab_Id = NULL;
	children.push_back(_selectedRab_Id);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// iUSelectedCodec
	_iUSelectedCodec = NULL;
	children.push_back(_iUSelectedCodec);

	// iuAvailableCodecsList
	_iuAvailableCodecsList = NULL;
	children.push_back(_iuAvailableCodecsList);

	// aoipSelectedCodecTarget
	_aoipSelectedCodecTarget = NULL;
	children.push_back(_aoipSelectedCodecTarget);

	// aoipAvailableCodecsListMap
	_aoipAvailableCodecsListMap = NULL;
	children.push_back(_aoipAvailableCodecsListMap);


}
asn1::ProcessAccessSignalling_Arg::~ProcessAccessSignalling_Arg(){

}

asn1::ASN1Node* asn1::ProcessAccessSignalling_Arg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_selectedUMTS_Algorithms = new SelectedUMTS_Algorithms();
			_selectedUMTS_Algorithms->tlv->tag_class = CONTEXT_SPECIFIC;
			_selectedUMTS_Algorithms->tlv->tag_value = 1;
			children[1] = _selectedUMTS_Algorithms;
			return _selectedUMTS_Algorithms;
		case 2:
			_selectedGSM_Algorithm = new SelectedGSM_Algorithm();
			_selectedGSM_Algorithm->tlv->tag_class = CONTEXT_SPECIFIC;
			_selectedGSM_Algorithm->tlv->tag_value = 2;
			children[2] = _selectedGSM_Algorithm;
			return _selectedGSM_Algorithm;
		case 3:
			_chosenRadioResourceInformation = new ChosenRadioResourceInformation();
			_chosenRadioResourceInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_chosenRadioResourceInformation->tlv->tag_value = 3;
			children[3] = _chosenRadioResourceInformation;
			return _chosenRadioResourceInformation;
		case 4:
			_selectedRab_Id = new RAB_Id();
			_selectedRab_Id->tlv->tag_class = CONTEXT_SPECIFIC;
			_selectedRab_Id->tlv->tag_value = 4;
			children[4] = _selectedRab_Id;
			return _selectedRab_Id;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[5] = _extensionContainer;
			return _extensionContainer;
		case 6:
			_iUSelectedCodec = new Codec();
			_iUSelectedCodec->tlv->tag_class = CONTEXT_SPECIFIC;
			_iUSelectedCodec->tlv->tag_value = 5;
			children[6] = _iUSelectedCodec;
			return _iUSelectedCodec;
		case 7:
			_iuAvailableCodecsList = new CodecList();
			_iuAvailableCodecsList->tlv->tag_class = CONTEXT_SPECIFIC;
			_iuAvailableCodecsList->tlv->tag_value = 6;
			children[7] = _iuAvailableCodecsList;
			return _iuAvailableCodecsList;
		case 8:
			_aoipSelectedCodecTarget = new AoIPCodec();
			_aoipSelectedCodecTarget->tlv->tag_class = CONTEXT_SPECIFIC;
			_aoipSelectedCodecTarget->tlv->tag_value = 7;
			children[8] = _aoipSelectedCodecTarget;
			return _aoipSelectedCodecTarget;
		case 9:
			_aoipAvailableCodecsListMap = new AoIPCodecsList();
			_aoipAvailableCodecsListMap->tlv->tag_class = CONTEXT_SPECIFIC;
			_aoipAvailableCodecsListMap->tlv->tag_value = 8;
			children[9] = _aoipAvailableCodecsListMap;
			return _aoipAvailableCodecsListMap;

		default: return NULL;
	}
}

void asn1::ProcessAccessSignalling_Arg::set_selectedUMTS_Algorithms(){
	if(_selectedUMTS_Algorithms == NULL) _selectedUMTS_Algorithms = (SelectedUMTS_Algorithms*)create_node(1);
}

void asn1::ProcessAccessSignalling_Arg::set_selectedGSM_Algorithm(){
	if(_selectedGSM_Algorithm == NULL) _selectedGSM_Algorithm = (SelectedGSM_Algorithm*)create_node(2);
}

void asn1::ProcessAccessSignalling_Arg::set_chosenRadioResourceInformation(){
	if(_chosenRadioResourceInformation == NULL) _chosenRadioResourceInformation = (ChosenRadioResourceInformation*)create_node(3);
}

void asn1::ProcessAccessSignalling_Arg::set_selectedRab_Id(){
	if(_selectedRab_Id == NULL) _selectedRab_Id = (RAB_Id*)create_node(4);
}

void asn1::ProcessAccessSignalling_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

void asn1::ProcessAccessSignalling_Arg::set_iUSelectedCodec(){
	if(_iUSelectedCodec == NULL) _iUSelectedCodec = (Codec*)create_node(6);
}

void asn1::ProcessAccessSignalling_Arg::set_iuAvailableCodecsList(){
	if(_iuAvailableCodecsList == NULL) _iuAvailableCodecsList = (CodecList*)create_node(7);
}

void asn1::ProcessAccessSignalling_Arg::set_aoipSelectedCodecTarget(){
	if(_aoipSelectedCodecTarget == NULL) _aoipSelectedCodecTarget = (AoIPCodec*)create_node(8);
}

void asn1::ProcessAccessSignalling_Arg::set_aoipAvailableCodecsListMap(){
	if(_aoipAvailableCodecsListMap == NULL) _aoipAvailableCodecsListMap = (AoIPCodecsList*)create_node(9);
}

//AoIPCodecsList
asn1::AoIPCodecsList::AoIPCodecsList(){
	strcpy(node_type_name, "AoIPCodecsList");
	// codec1
	_codec1 = NULL;
	_codec1 = new AoIPCodec();
	_codec1->tlv->tag_class = CONTEXT_SPECIFIC;
	_codec1->tlv->tag_value = 1;
	children.push_back(_codec1);

	// codec2
	_codec2 = NULL;
	children.push_back(_codec2);

	// codec3
	_codec3 = NULL;
	children.push_back(_codec3);

	// codec4
	_codec4 = NULL;
	children.push_back(_codec4);

	// codec5
	_codec5 = NULL;
	children.push_back(_codec5);

	// codec6
	_codec6 = NULL;
	children.push_back(_codec6);

	// codec7
	_codec7 = NULL;
	children.push_back(_codec7);

	// codec8
	_codec8 = NULL;
	children.push_back(_codec8);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::AoIPCodecsList::~AoIPCodecsList(){

}

asn1::ASN1Node* asn1::AoIPCodecsList::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_codec2 = new AoIPCodec();
			_codec2->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec2->tlv->tag_value = 2;
			children[1] = _codec2;
			return _codec2;
		case 2:
			_codec3 = new AoIPCodec();
			_codec3->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec3->tlv->tag_value = 3;
			children[2] = _codec3;
			return _codec3;
		case 3:
			_codec4 = new AoIPCodec();
			_codec4->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec4->tlv->tag_value = 4;
			children[3] = _codec4;
			return _codec4;
		case 4:
			_codec5 = new AoIPCodec();
			_codec5->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec5->tlv->tag_value = 5;
			children[4] = _codec5;
			return _codec5;
		case 5:
			_codec6 = new AoIPCodec();
			_codec6->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec6->tlv->tag_value = 6;
			children[5] = _codec6;
			return _codec6;
		case 6:
			_codec7 = new AoIPCodec();
			_codec7->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec7->tlv->tag_value = 7;
			children[6] = _codec7;
			return _codec7;
		case 7:
			_codec8 = new AoIPCodec();
			_codec8->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec8->tlv->tag_value = 8;
			children[7] = _codec8;
			return _codec8;
		case 8:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 9;
			children[8] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::AoIPCodecsList::set_codec2(){
	if(_codec2 == NULL) _codec2 = (AoIPCodec*)create_node(1);
}

void asn1::AoIPCodecsList::set_codec3(){
	if(_codec3 == NULL) _codec3 = (AoIPCodec*)create_node(2);
}

void asn1::AoIPCodecsList::set_codec4(){
	if(_codec4 == NULL) _codec4 = (AoIPCodec*)create_node(3);
}

void asn1::AoIPCodecsList::set_codec5(){
	if(_codec5 == NULL) _codec5 = (AoIPCodec*)create_node(4);
}

void asn1::AoIPCodecsList::set_codec6(){
	if(_codec6 == NULL) _codec6 = (AoIPCodec*)create_node(5);
}

void asn1::AoIPCodecsList::set_codec7(){
	if(_codec7 == NULL) _codec7 = (AoIPCodec*)create_node(6);
}

void asn1::AoIPCodecsList::set_codec8(){
	if(_codec8 == NULL) _codec8 = (AoIPCodec*)create_node(7);
}

void asn1::AoIPCodecsList::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(8);
}

//AoIPCodec
asn1::AoIPCodec::AoIPCodec(){
	strcpy(node_type_name, "AoIPCodec");

}
asn1::AoIPCodec::~AoIPCodec(){

}

//SupportedCodecsList
asn1::SupportedCodecsList::SupportedCodecsList(){
	strcpy(node_type_name, "SupportedCodecsList");
	// utranCodecList
	_utranCodecList = NULL;
	children.push_back(_utranCodecList);

	// geranCodecList
	_geranCodecList = NULL;
	children.push_back(_geranCodecList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SupportedCodecsList::~SupportedCodecsList(){

}

asn1::ASN1Node* asn1::SupportedCodecsList::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_utranCodecList = new CodecList();
			_utranCodecList->tlv->tag_class = CONTEXT_SPECIFIC;
			_utranCodecList->tlv->tag_value = 0;
			children[0] = _utranCodecList;
			return _utranCodecList;
		case 1:
			_geranCodecList = new CodecList();
			_geranCodecList->tlv->tag_class = CONTEXT_SPECIFIC;
			_geranCodecList->tlv->tag_value = 1;
			children[1] = _geranCodecList;
			return _geranCodecList;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SupportedCodecsList::set_utranCodecList(){
	if(_utranCodecList == NULL) _utranCodecList = (CodecList*)create_node(0);
}

void asn1::SupportedCodecsList::set_geranCodecList(){
	if(_geranCodecList == NULL) _geranCodecList = (CodecList*)create_node(1);
}

void asn1::SupportedCodecsList::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//CodecList
asn1::CodecList::CodecList(){
	strcpy(node_type_name, "CodecList");
	// codec1
	_codec1 = NULL;
	_codec1 = new Codec();
	_codec1->tlv->tag_class = CONTEXT_SPECIFIC;
	_codec1->tlv->tag_value = 1;
	children.push_back(_codec1);

	// codec2
	_codec2 = NULL;
	children.push_back(_codec2);

	// codec3
	_codec3 = NULL;
	children.push_back(_codec3);

	// codec4
	_codec4 = NULL;
	children.push_back(_codec4);

	// codec5
	_codec5 = NULL;
	children.push_back(_codec5);

	// codec6
	_codec6 = NULL;
	children.push_back(_codec6);

	// codec7
	_codec7 = NULL;
	children.push_back(_codec7);

	// codec8
	_codec8 = NULL;
	children.push_back(_codec8);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CodecList::~CodecList(){

}

asn1::ASN1Node* asn1::CodecList::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_codec2 = new Codec();
			_codec2->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec2->tlv->tag_value = 2;
			children[1] = _codec2;
			return _codec2;
		case 2:
			_codec3 = new Codec();
			_codec3->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec3->tlv->tag_value = 3;
			children[2] = _codec3;
			return _codec3;
		case 3:
			_codec4 = new Codec();
			_codec4->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec4->tlv->tag_value = 4;
			children[3] = _codec4;
			return _codec4;
		case 4:
			_codec5 = new Codec();
			_codec5->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec5->tlv->tag_value = 5;
			children[4] = _codec5;
			return _codec5;
		case 5:
			_codec6 = new Codec();
			_codec6->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec6->tlv->tag_value = 6;
			children[5] = _codec6;
			return _codec6;
		case 6:
			_codec7 = new Codec();
			_codec7->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec7->tlv->tag_value = 7;
			children[6] = _codec7;
			return _codec7;
		case 7:
			_codec8 = new Codec();
			_codec8->tlv->tag_class = CONTEXT_SPECIFIC;
			_codec8->tlv->tag_value = 8;
			children[7] = _codec8;
			return _codec8;
		case 8:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 9;
			children[8] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CodecList::set_codec2(){
	if(_codec2 == NULL) _codec2 = (Codec*)create_node(1);
}

void asn1::CodecList::set_codec3(){
	if(_codec3 == NULL) _codec3 = (Codec*)create_node(2);
}

void asn1::CodecList::set_codec4(){
	if(_codec4 == NULL) _codec4 = (Codec*)create_node(3);
}

void asn1::CodecList::set_codec5(){
	if(_codec5 == NULL) _codec5 = (Codec*)create_node(4);
}

void asn1::CodecList::set_codec6(){
	if(_codec6 == NULL) _codec6 = (Codec*)create_node(5);
}

void asn1::CodecList::set_codec7(){
	if(_codec7 == NULL) _codec7 = (Codec*)create_node(6);
}

void asn1::CodecList::set_codec8(){
	if(_codec8 == NULL) _codec8 = (Codec*)create_node(7);
}

void asn1::CodecList::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(8);
}

//Codec
asn1::Codec::Codec(){
	strcpy(node_type_name, "Codec");

}
asn1::Codec::~Codec(){

}

//GERAN_Classmark
asn1::GERAN_Classmark::GERAN_Classmark(){
	strcpy(node_type_name, "GERAN_Classmark");

}
asn1::GERAN_Classmark::~GERAN_Classmark(){

}

//SelectedGSM_Algorithm
asn1::SelectedGSM_Algorithm::SelectedGSM_Algorithm(){
	strcpy(node_type_name, "SelectedGSM_Algorithm");

}
asn1::SelectedGSM_Algorithm::~SelectedGSM_Algorithm(){

}

//SendEndSignal_Arg
asn1::SendEndSignal_Arg::SendEndSignal_Arg(){
	strcpy(node_type_name, "SendEndSignal_Arg");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// an_APDU
	_an_APDU = NULL;
	_an_APDU = new AccessNetworkSignalInfo();
	children.push_back(_an_APDU);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SendEndSignal_Arg::~SendEndSignal_Arg(){

}

asn1::ASN1Node* asn1::SendEndSignal_Arg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SendEndSignal_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//SendEndSignal_Res
asn1::SendEndSignal_Res::SendEndSignal_Res(){
	strcpy(node_type_name, "SendEndSignal_Res");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SendEndSignal_Res::~SendEndSignal_Res(){

}

asn1::ASN1Node* asn1::SendEndSignal_Res::create_node(unsigned int _index){
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

void asn1::SendEndSignal_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//RNCId
asn1::RNCId::RNCId(){
	strcpy(node_type_name, "RNCId");

}
asn1::RNCId::~RNCId(){

}

//RelocationNumberList
asn1::RelocationNumberList::RelocationNumberList(){
	strcpy(node_type_name, "RelocationNumberList");

}
asn1::RelocationNumberList::~RelocationNumberList(){
}

asn1::RelocationNumber* asn1::RelocationNumberList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (RelocationNumber*)children[child_index]; else return NULL;
}

void asn1::RelocationNumberList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::RelocationNumberList::create_node(unsigned int _index){
	children[_index] = new RelocationNumber();
	return children[_index];
}

asn1::ASN1Node* asn1::RelocationNumberList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new RelocationNumber());
		return children[children.size() - 1];
	}

}

//MulticallBearerInfo
asn1::MulticallBearerInfo::MulticallBearerInfo(){
	strcpy(node_type_name, "MulticallBearerInfo");

}
asn1::MulticallBearerInfo::~MulticallBearerInfo(){

}

//RelocationNumber
asn1::RelocationNumber::RelocationNumber(){
	strcpy(node_type_name, "RelocationNumber");
	// handoverNumber
	_handoverNumber = NULL;
	_handoverNumber = new ISDN_AddressString();
	children.push_back(_handoverNumber);

	// rab_Id
	_rab_Id = NULL;
	_rab_Id = new RAB_Id();
	children.push_back(_rab_Id);


}
asn1::RelocationNumber::~RelocationNumber(){

}

//RAB_Id
asn1::RAB_Id::RAB_Id(){
	strcpy(node_type_name, "RAB_Id");

}
asn1::RAB_Id::~RAB_Id(){

}

//maxNrOfRABs
asn1::maxNrOfRABs::maxNrOfRABs(){
	strcpy(node_type_name, "maxNrOfRABs");

}
asn1::maxNrOfRABs::~maxNrOfRABs(){

}

//maxNumOfRelocationNumber
asn1::maxNumOfRelocationNumber::maxNumOfRelocationNumber(){
	strcpy(node_type_name, "maxNumOfRelocationNumber");

}
asn1::maxNumOfRelocationNumber::~maxNumOfRelocationNumber(){

}

//RadioResourceInformation
asn1::RadioResourceInformation::RadioResourceInformation(){
	strcpy(node_type_name, "RadioResourceInformation");

}
asn1::RadioResourceInformation::~RadioResourceInformation(){

}

//IntegrityProtectionInformation
asn1::IntegrityProtectionInformation::IntegrityProtectionInformation(){
	strcpy(node_type_name, "IntegrityProtectionInformation");

}
asn1::IntegrityProtectionInformation::~IntegrityProtectionInformation(){

}

//maxNumOfIntegrityInfo
asn1::maxNumOfIntegrityInfo::maxNumOfIntegrityInfo(){
	strcpy(node_type_name, "maxNumOfIntegrityInfo");

}
asn1::maxNumOfIntegrityInfo::~maxNumOfIntegrityInfo(){

}

//EncryptionInformation
asn1::EncryptionInformation::EncryptionInformation(){
	strcpy(node_type_name, "EncryptionInformation");

}
asn1::EncryptionInformation::~EncryptionInformation(){

}

//maxNumOfEncryptionInfo
asn1::maxNumOfEncryptionInfo::maxNumOfEncryptionInfo(){
	strcpy(node_type_name, "maxNumOfEncryptionInfo");

}
asn1::maxNumOfEncryptionInfo::~maxNumOfEncryptionInfo(){

}

//SendAuthenticationInfoArg
asn1::SendAuthenticationInfoArg::SendAuthenticationInfoArg(){
	strcpy(node_type_name, "SendAuthenticationInfoArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// numberOfRequestedVectors
	_numberOfRequestedVectors = NULL;
	_numberOfRequestedVectors = new NumberOfRequestedVectors();
	children.push_back(_numberOfRequestedVectors);

	// segmentationProhibited
	_segmentationProhibited = NULL;
	children.push_back(_segmentationProhibited);

	// immediateResponsePreferred
	_immediateResponsePreferred = NULL;
	children.push_back(_immediateResponsePreferred);

	// re_synchronisationInfo
	_re_synchronisationInfo = NULL;
	children.push_back(_re_synchronisationInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// requestingNodeType
	_requestingNodeType = NULL;
	children.push_back(_requestingNodeType);

	// requestingPLMN_Id
	_requestingPLMN_Id = NULL;
	children.push_back(_requestingPLMN_Id);

	// numberOfRequestedAdditional_Vectors
	_numberOfRequestedAdditional_Vectors = NULL;
	children.push_back(_numberOfRequestedAdditional_Vectors);

	// additionalVectorsAreForEPS
	_additionalVectorsAreForEPS = NULL;
	children.push_back(_additionalVectorsAreForEPS);


}
asn1::SendAuthenticationInfoArg::~SendAuthenticationInfoArg(){

}

asn1::ASN1Node* asn1::SendAuthenticationInfoArg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_segmentationProhibited = new Null();
			children[2] = _segmentationProhibited;
			return _segmentationProhibited;
		case 3:
			_immediateResponsePreferred = new Null();
			_immediateResponsePreferred->tlv->tag_class = CONTEXT_SPECIFIC;
			_immediateResponsePreferred->tlv->tag_value = 1;
			children[3] = _immediateResponsePreferred;
			return _immediateResponsePreferred;
		case 4:
			_re_synchronisationInfo = new Re_synchronisationInfo();
			children[4] = _re_synchronisationInfo;
			return _re_synchronisationInfo;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[5] = _extensionContainer;
			return _extensionContainer;
		case 6:
			_requestingNodeType = new RequestingNodeType();
			_requestingNodeType->tlv->tag_class = CONTEXT_SPECIFIC;
			_requestingNodeType->tlv->tag_value = 3;
			children[6] = _requestingNodeType;
			return _requestingNodeType;
		case 7:
			_requestingPLMN_Id = new PLMN_Id();
			_requestingPLMN_Id->tlv->tag_class = CONTEXT_SPECIFIC;
			_requestingPLMN_Id->tlv->tag_value = 4;
			children[7] = _requestingPLMN_Id;
			return _requestingPLMN_Id;
		case 8:
			_numberOfRequestedAdditional_Vectors = new NumberOfRequestedVectors();
			_numberOfRequestedAdditional_Vectors->tlv->tag_class = CONTEXT_SPECIFIC;
			_numberOfRequestedAdditional_Vectors->tlv->tag_value = 5;
			children[8] = _numberOfRequestedAdditional_Vectors;
			return _numberOfRequestedAdditional_Vectors;
		case 9:
			_additionalVectorsAreForEPS = new Null();
			_additionalVectorsAreForEPS->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalVectorsAreForEPS->tlv->tag_value = 6;
			children[9] = _additionalVectorsAreForEPS;
			return _additionalVectorsAreForEPS;

		default: return NULL;
	}
}

void asn1::SendAuthenticationInfoArg::set_segmentationProhibited(){
	if(_segmentationProhibited == NULL) _segmentationProhibited = (Null*)create_node(2);
}

void asn1::SendAuthenticationInfoArg::set_immediateResponsePreferred(){
	if(_immediateResponsePreferred == NULL) _immediateResponsePreferred = (Null*)create_node(3);
}

void asn1::SendAuthenticationInfoArg::set_re_synchronisationInfo(){
	if(_re_synchronisationInfo == NULL) _re_synchronisationInfo = (Re_synchronisationInfo*)create_node(4);
}

void asn1::SendAuthenticationInfoArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

void asn1::SendAuthenticationInfoArg::set_requestingNodeType(){
	if(_requestingNodeType == NULL) _requestingNodeType = (RequestingNodeType*)create_node(6);
}

void asn1::SendAuthenticationInfoArg::set_requestingPLMN_Id(){
	if(_requestingPLMN_Id == NULL) _requestingPLMN_Id = (PLMN_Id*)create_node(7);
}

void asn1::SendAuthenticationInfoArg::set_numberOfRequestedAdditional_Vectors(){
	if(_numberOfRequestedAdditional_Vectors == NULL) _numberOfRequestedAdditional_Vectors = (NumberOfRequestedVectors*)create_node(8);
}

void asn1::SendAuthenticationInfoArg::set_additionalVectorsAreForEPS(){
	if(_additionalVectorsAreForEPS == NULL) _additionalVectorsAreForEPS = (Null*)create_node(9);
}

//NumberOfRequestedVectors
asn1::NumberOfRequestedVectors::NumberOfRequestedVectors(){
	strcpy(node_type_name, "NumberOfRequestedVectors");

}
asn1::NumberOfRequestedVectors::~NumberOfRequestedVectors(){

}

//Re_synchronisationInfo
asn1::Re_synchronisationInfo::Re_synchronisationInfo(){
	strcpy(node_type_name, "Re_synchronisationInfo");
	// rand
	_rand = NULL;
	_rand = new RAND();
	children.push_back(_rand);

	// auts
	_auts = NULL;
	_auts = new AUTS();
	children.push_back(_auts);


}
asn1::Re_synchronisationInfo::~Re_synchronisationInfo(){

}

//SendAuthenticationInfoRes
asn1::SendAuthenticationInfoRes::SendAuthenticationInfoRes(){
	strcpy(node_type_name, "SendAuthenticationInfoRes");
	tlv->tag_class = CONTEXT_SPECIFIC;
	tlv->tag_value = 3;
	// authenticationSetList
	_authenticationSetList = NULL;
	children.push_back(_authenticationSetList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// eps_AuthenticationSetList
	_eps_AuthenticationSetList = NULL;
	children.push_back(_eps_AuthenticationSetList);


}
asn1::SendAuthenticationInfoRes::~SendAuthenticationInfoRes(){

}

asn1::ASN1Node* asn1::SendAuthenticationInfoRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_authenticationSetList = new AuthenticationSetList();
			children[0] = _authenticationSetList;
			return _authenticationSetList;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_eps_AuthenticationSetList = new EPS_AuthenticationSetList();
			_eps_AuthenticationSetList->tlv->tag_class = CONTEXT_SPECIFIC;
			_eps_AuthenticationSetList->tlv->tag_value = 2;
			children[2] = _eps_AuthenticationSetList;
			return _eps_AuthenticationSetList;

		default: return NULL;
	}
}

void asn1::SendAuthenticationInfoRes::set_authenticationSetList(){
	if(_authenticationSetList == NULL) _authenticationSetList = (AuthenticationSetList*)create_node(0);
}

void asn1::SendAuthenticationInfoRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::SendAuthenticationInfoRes::set_eps_AuthenticationSetList(){
	if(_eps_AuthenticationSetList == NULL) _eps_AuthenticationSetList = (EPS_AuthenticationSetList*)create_node(2);
}

//EPS_AuthenticationSetList
asn1::EPS_AuthenticationSetList::EPS_AuthenticationSetList(){
	strcpy(node_type_name, "EPS_AuthenticationSetList");

}
asn1::EPS_AuthenticationSetList::~EPS_AuthenticationSetList(){
}

asn1::EPC_AV* asn1::EPS_AuthenticationSetList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (EPC_AV*)children[child_index]; else return NULL;
}

void asn1::EPS_AuthenticationSetList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::EPS_AuthenticationSetList::create_node(unsigned int _index){
	children[_index] = new EPC_AV();
	return children[_index];
}

asn1::ASN1Node* asn1::EPS_AuthenticationSetList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new EPC_AV());
		return children[children.size() - 1];
	}

}

//EPC_AV
asn1::EPC_AV::EPC_AV(){
	strcpy(node_type_name, "EPC_AV");
	// rand
	_rand = NULL;
	_rand = new RAND();
	children.push_back(_rand);

	// xres
	_xres = NULL;
	_xres = new XRES();
	children.push_back(_xres);

	// autn
	_autn = NULL;
	_autn = new AUTN();
	children.push_back(_autn);

	// kasme
	_kasme = NULL;
	_kasme = new KASME();
	children.push_back(_kasme);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::EPC_AV::~EPC_AV(){

}

asn1::ASN1Node* asn1::EPC_AV::create_node(unsigned int _index){
	switch(_index){
		case 4:
			_extensionContainer = new ExtensionContainer();
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::EPC_AV::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//KASME
asn1::KASME::KASME(){
	strcpy(node_type_name, "KASME");

}
asn1::KASME::~KASME(){

}

//RequestingNodeType
asn1::RequestingNodeType::RequestingNodeType(){
	strcpy(node_type_name, "RequestingNodeType");

}
asn1::RequestingNodeType::~RequestingNodeType(){

}

//CheckIMEI_Arg
asn1::CheckIMEI_Arg::CheckIMEI_Arg(){
	strcpy(node_type_name, "CheckIMEI_Arg");
	// imei
	_imei = NULL;
	_imei = new IMEI();
	children.push_back(_imei);

	// requestedEquipmentInfo
	_requestedEquipmentInfo = NULL;
	_requestedEquipmentInfo = new RequestedEquipmentInfo();
	children.push_back(_requestedEquipmentInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CheckIMEI_Arg::~CheckIMEI_Arg(){

}

asn1::ASN1Node* asn1::CheckIMEI_Arg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CheckIMEI_Arg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//CheckIMEI_Res
asn1::CheckIMEI_Res::CheckIMEI_Res(){
	strcpy(node_type_name, "CheckIMEI_Res");
	// equipmentStatus
	_equipmentStatus = NULL;
	children.push_back(_equipmentStatus);

	// bmuef
	_bmuef = NULL;
	children.push_back(_bmuef);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CheckIMEI_Res::~CheckIMEI_Res(){

}

asn1::ASN1Node* asn1::CheckIMEI_Res::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_equipmentStatus = new EquipmentStatus();
			children[0] = _equipmentStatus;
			return _equipmentStatus;
		case 1:
			_bmuef = new UESBI_Iu();
			children[1] = _bmuef;
			return _bmuef;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CheckIMEI_Res::set_equipmentStatus(){
	if(_equipmentStatus == NULL) _equipmentStatus = (EquipmentStatus*)create_node(0);
}

void asn1::CheckIMEI_Res::set_bmuef(){
	if(_bmuef == NULL) _bmuef = (UESBI_Iu*)create_node(1);
}

void asn1::CheckIMEI_Res::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//RequestedEquipmentInfo
asn1::RequestedEquipmentInfo::RequestedEquipmentInfo(){
	strcpy(node_type_name, "RequestedEquipmentInfo");

}
asn1::RequestedEquipmentInfo::~RequestedEquipmentInfo(){

}

//UESBI_Iu
asn1::UESBI_Iu::UESBI_Iu(){
	strcpy(node_type_name, "UESBI_Iu");
	// uesbi_IuA
	_uesbi_IuA = NULL;
	children.push_back(_uesbi_IuA);

	// uesbi_IuB
	_uesbi_IuB = NULL;
	children.push_back(_uesbi_IuB);


}
asn1::UESBI_Iu::~UESBI_Iu(){

}

asn1::ASN1Node* asn1::UESBI_Iu::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_uesbi_IuA = new UESBI_IuA();
			_uesbi_IuA->tlv->tag_class = CONTEXT_SPECIFIC;
			_uesbi_IuA->tlv->tag_value = 0;
			children[0] = _uesbi_IuA;
			return _uesbi_IuA;
		case 1:
			_uesbi_IuB = new UESBI_IuB();
			_uesbi_IuB->tlv->tag_class = CONTEXT_SPECIFIC;
			_uesbi_IuB->tlv->tag_value = 1;
			children[1] = _uesbi_IuB;
			return _uesbi_IuB;

		default: return NULL;
	}
}

void asn1::UESBI_Iu::set_uesbi_IuA(){
	if(_uesbi_IuA == NULL) _uesbi_IuA = (UESBI_IuA*)create_node(0);
}

void asn1::UESBI_Iu::set_uesbi_IuB(){
	if(_uesbi_IuB == NULL) _uesbi_IuB = (UESBI_IuB*)create_node(1);
}

//UESBI_IuA
asn1::UESBI_IuA::UESBI_IuA(){
	strcpy(node_type_name, "UESBI_IuA");

}
asn1::UESBI_IuA::~UESBI_IuA(){

}

//UESBI_IuB
asn1::UESBI_IuB::UESBI_IuB(){
	strcpy(node_type_name, "UESBI_IuB");

}
asn1::UESBI_IuB::~UESBI_IuB(){

}

//EquipmentStatus
asn1::EquipmentStatus::EquipmentStatus(){
	strcpy(node_type_name, "EquipmentStatus");

}
asn1::EquipmentStatus::~EquipmentStatus(){

}

//InsertSubscriberDataArg
asn1::InsertSubscriberDataArg::InsertSubscriberDataArg(){
	strcpy(node_type_name, "InsertSubscriberDataArg");
	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// naea_PreferredCI
	_naea_PreferredCI = NULL;
	children.push_back(_naea_PreferredCI);

	// gprsSubscriptionData
	_gprsSubscriptionData = NULL;
	children.push_back(_gprsSubscriptionData);

	// roamingRestrictedInSgsnDueToUnsupportedFeature
	_roamingRestrictedInSgsnDueToUnsupportedFeature = NULL;
	children.push_back(_roamingRestrictedInSgsnDueToUnsupportedFeature);

	// networkAccessMode
	_networkAccessMode = NULL;
	children.push_back(_networkAccessMode);

	// lsaInformation
	_lsaInformation = NULL;
	children.push_back(_lsaInformation);

	// lmu_Indicator
	_lmu_Indicator = NULL;
	children.push_back(_lmu_Indicator);

	// lcsInformation
	_lcsInformation = NULL;
	children.push_back(_lcsInformation);

	// istAlertTimer
	_istAlertTimer = NULL;
	children.push_back(_istAlertTimer);

	// superChargerSupportedInHLR
	_superChargerSupportedInHLR = NULL;
	children.push_back(_superChargerSupportedInHLR);

	// mc_SS_Info
	_mc_SS_Info = NULL;
	children.push_back(_mc_SS_Info);

	// cs_AllocationRetentionPriority
	_cs_AllocationRetentionPriority = NULL;
	children.push_back(_cs_AllocationRetentionPriority);

	// sgsn_CAMEL_SubscriptionInfo
	_sgsn_CAMEL_SubscriptionInfo = NULL;
	children.push_back(_sgsn_CAMEL_SubscriptionInfo);

	// chargingCharacteristics
	_chargingCharacteristics = NULL;
	children.push_back(_chargingCharacteristics);

	// accessRestrictionData
	_accessRestrictionData = NULL;
	children.push_back(_accessRestrictionData);

	// ics_Indicator
	_ics_Indicator = NULL;
	children.push_back(_ics_Indicator);

	// eps_SubscriptionData
	_eps_SubscriptionData = NULL;
	children.push_back(_eps_SubscriptionData);

	// csg_SubscriptionDataList
	_csg_SubscriptionDataList = NULL;
	children.push_back(_csg_SubscriptionDataList);

	// ue_ReachabilityRequestIndicator
	_ue_ReachabilityRequestIndicator = NULL;
	children.push_back(_ue_ReachabilityRequestIndicator);

	// sgsn_Number
	_sgsn_Number = NULL;
	children.push_back(_sgsn_Number);

	// mme_Name
	_mme_Name = NULL;
	children.push_back(_mme_Name);

	// subscribedPeriodicRAUTAUtimer
	_subscribedPeriodicRAUTAUtimer = NULL;
	children.push_back(_subscribedPeriodicRAUTAUtimer);

	// vplmnLIPAAllowed
	_vplmnLIPAAllowed = NULL;
	children.push_back(_vplmnLIPAAllowed);

	// mdtUserConsent
	_mdtUserConsent = NULL;
	children.push_back(_mdtUserConsent);

	// subscribedPeriodicLAUtimer
	_subscribedPeriodicLAUtimer = NULL;
	children.push_back(_subscribedPeriodicLAUtimer);

	// vplmn_Csg_SubscriptionDataList
	_vplmn_Csg_SubscriptionDataList = NULL;
	children.push_back(_vplmn_Csg_SubscriptionDataList);

	// additionalMSISDN
	_additionalMSISDN = NULL;
	children.push_back(_additionalMSISDN);

	// psAndSMS_OnlyServiceProvision
	_psAndSMS_OnlyServiceProvision = NULL;
	children.push_back(_psAndSMS_OnlyServiceProvision);

	// smsInSGSNAllowed
	_smsInSGSNAllowed = NULL;
	children.push_back(_smsInSGSNAllowed);


}
asn1::InsertSubscriberDataArg::~InsertSubscriberDataArg(){

}

asn1::ASN1Node* asn1::InsertSubscriberDataArg::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 0;
			children[0] = _imsi;
			return _imsi;
		case 4:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 14;
			children[4] = _extensionContainer;
			return _extensionContainer;
		case 5:
			_naea_PreferredCI = new NAEA_PreferredCI();
			_naea_PreferredCI->tlv->tag_class = CONTEXT_SPECIFIC;
			_naea_PreferredCI->tlv->tag_value = 15;
			children[5] = _naea_PreferredCI;
			return _naea_PreferredCI;
		case 6:
			_gprsSubscriptionData = new GPRSSubscriptionData();
			_gprsSubscriptionData->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprsSubscriptionData->tlv->tag_value = 16;
			children[6] = _gprsSubscriptionData;
			return _gprsSubscriptionData;
		case 7:
			_roamingRestrictedInSgsnDueToUnsupportedFeature = new Null();
			_roamingRestrictedInSgsnDueToUnsupportedFeature->tlv->tag_class = CONTEXT_SPECIFIC;
			_roamingRestrictedInSgsnDueToUnsupportedFeature->tlv->tag_value = 23;
			children[7] = _roamingRestrictedInSgsnDueToUnsupportedFeature;
			return _roamingRestrictedInSgsnDueToUnsupportedFeature;
		case 8:
			_networkAccessMode = new NetworkAccessMode();
			_networkAccessMode->tlv->tag_class = CONTEXT_SPECIFIC;
			_networkAccessMode->tlv->tag_value = 24;
			children[8] = _networkAccessMode;
			return _networkAccessMode;
		case 9:
			_lsaInformation = new LSAInformation();
			_lsaInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_lsaInformation->tlv->tag_value = 25;
			children[9] = _lsaInformation;
			return _lsaInformation;
		case 10:
			_lmu_Indicator = new Null();
			_lmu_Indicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_lmu_Indicator->tlv->tag_value = 21;
			children[10] = _lmu_Indicator;
			return _lmu_Indicator;
		case 11:
			_lcsInformation = new LCSInformation();
			_lcsInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_lcsInformation->tlv->tag_value = 22;
			children[11] = _lcsInformation;
			return _lcsInformation;
		case 12:
			_istAlertTimer = new IST_AlertTimerValue();
			_istAlertTimer->tlv->tag_class = CONTEXT_SPECIFIC;
			_istAlertTimer->tlv->tag_value = 26;
			children[12] = _istAlertTimer;
			return _istAlertTimer;
		case 13:
			_superChargerSupportedInHLR = new AgeIndicator();
			_superChargerSupportedInHLR->tlv->tag_class = CONTEXT_SPECIFIC;
			_superChargerSupportedInHLR->tlv->tag_value = 27;
			children[13] = _superChargerSupportedInHLR;
			return _superChargerSupportedInHLR;
		case 14:
			_mc_SS_Info = new MC_SS_Info();
			_mc_SS_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_mc_SS_Info->tlv->tag_value = 28;
			children[14] = _mc_SS_Info;
			return _mc_SS_Info;
		case 15:
			_cs_AllocationRetentionPriority = new CS_AllocationRetentionPriority();
			_cs_AllocationRetentionPriority->tlv->tag_class = CONTEXT_SPECIFIC;
			_cs_AllocationRetentionPriority->tlv->tag_value = 29;
			children[15] = _cs_AllocationRetentionPriority;
			return _cs_AllocationRetentionPriority;
		case 16:
			_sgsn_CAMEL_SubscriptionInfo = new SGSN_CAMEL_SubscriptionInfo();
			_sgsn_CAMEL_SubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_CAMEL_SubscriptionInfo->tlv->tag_value = 17;
			children[16] = _sgsn_CAMEL_SubscriptionInfo;
			return _sgsn_CAMEL_SubscriptionInfo;
		case 17:
			_chargingCharacteristics = new ChargingCharacteristics();
			_chargingCharacteristics->tlv->tag_class = CONTEXT_SPECIFIC;
			_chargingCharacteristics->tlv->tag_value = 18;
			children[17] = _chargingCharacteristics;
			return _chargingCharacteristics;
		case 18:
			_accessRestrictionData = new AccessRestrictionData();
			_accessRestrictionData->tlv->tag_class = CONTEXT_SPECIFIC;
			_accessRestrictionData->tlv->tag_value = 19;
			children[18] = _accessRestrictionData;
			return _accessRestrictionData;
		case 19:
			_ics_Indicator = new Boolean();
			_ics_Indicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_ics_Indicator->tlv->tag_value = 20;
			children[19] = _ics_Indicator;
			return _ics_Indicator;
		case 20:
			_eps_SubscriptionData = new EPS_SubscriptionData();
			_eps_SubscriptionData->tlv->tag_class = CONTEXT_SPECIFIC;
			_eps_SubscriptionData->tlv->tag_value = 31;
			children[20] = _eps_SubscriptionData;
			return _eps_SubscriptionData;
		case 21:
			_csg_SubscriptionDataList = new CSG_SubscriptionDataList();
			_csg_SubscriptionDataList->tlv->tag_class = CONTEXT_SPECIFIC;
			_csg_SubscriptionDataList->tlv->tag_value = 32;
			children[21] = _csg_SubscriptionDataList;
			return _csg_SubscriptionDataList;
		case 22:
			_ue_ReachabilityRequestIndicator = new Null();
			_ue_ReachabilityRequestIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_ue_ReachabilityRequestIndicator->tlv->tag_value = 33;
			children[22] = _ue_ReachabilityRequestIndicator;
			return _ue_ReachabilityRequestIndicator;
		case 23:
			_sgsn_Number = new ISDN_AddressString();
			_sgsn_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_Number->tlv->tag_value = 34;
			children[23] = _sgsn_Number;
			return _sgsn_Number;
		case 24:
			_mme_Name = new DiameterIdentity();
			_mme_Name->tlv->tag_class = CONTEXT_SPECIFIC;
			_mme_Name->tlv->tag_value = 35;
			children[24] = _mme_Name;
			return _mme_Name;
		case 25:
			_subscribedPeriodicRAUTAUtimer = new SubscribedPeriodicRAUTAUtimer();
			_subscribedPeriodicRAUTAUtimer->tlv->tag_class = CONTEXT_SPECIFIC;
			_subscribedPeriodicRAUTAUtimer->tlv->tag_value = 36;
			children[25] = _subscribedPeriodicRAUTAUtimer;
			return _subscribedPeriodicRAUTAUtimer;
		case 26:
			_vplmnLIPAAllowed = new Null();
			_vplmnLIPAAllowed->tlv->tag_class = CONTEXT_SPECIFIC;
			_vplmnLIPAAllowed->tlv->tag_value = 37;
			children[26] = _vplmnLIPAAllowed;
			return _vplmnLIPAAllowed;
		case 27:
			_mdtUserConsent = new Boolean();
			_mdtUserConsent->tlv->tag_class = CONTEXT_SPECIFIC;
			_mdtUserConsent->tlv->tag_value = 38;
			children[27] = _mdtUserConsent;
			return _mdtUserConsent;
		case 28:
			_subscribedPeriodicLAUtimer = new SubscribedPeriodicLAUtimer();
			_subscribedPeriodicLAUtimer->tlv->tag_class = CONTEXT_SPECIFIC;
			_subscribedPeriodicLAUtimer->tlv->tag_value = 39;
			children[28] = _subscribedPeriodicLAUtimer;
			return _subscribedPeriodicLAUtimer;
		case 29:
			_vplmn_Csg_SubscriptionDataList = new VPLMN_CSG_SubscriptionDataList();
			_vplmn_Csg_SubscriptionDataList->tlv->tag_class = CONTEXT_SPECIFIC;
			_vplmn_Csg_SubscriptionDataList->tlv->tag_value = 40;
			children[29] = _vplmn_Csg_SubscriptionDataList;
			return _vplmn_Csg_SubscriptionDataList;
		case 30:
			_additionalMSISDN = new ISDN_AddressString();
			_additionalMSISDN->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalMSISDN->tlv->tag_value = 41;
			children[30] = _additionalMSISDN;
			return _additionalMSISDN;
		case 31:
			_psAndSMS_OnlyServiceProvision = new Null();
			_psAndSMS_OnlyServiceProvision->tlv->tag_class = CONTEXT_SPECIFIC;
			_psAndSMS_OnlyServiceProvision->tlv->tag_value = 42;
			children[31] = _psAndSMS_OnlyServiceProvision;
			return _psAndSMS_OnlyServiceProvision;
		case 32:
			_smsInSGSNAllowed = new Null();
			_smsInSGSNAllowed->tlv->tag_class = CONTEXT_SPECIFIC;
			_smsInSGSNAllowed->tlv->tag_value = 43;
			children[32] = _smsInSGSNAllowed;
			return _smsInSGSNAllowed;

		default: return NULL;
	}
}

void asn1::InsertSubscriberDataArg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(0);
}

void asn1::InsertSubscriberDataArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

void asn1::InsertSubscriberDataArg::set_naea_PreferredCI(){
	if(_naea_PreferredCI == NULL) _naea_PreferredCI = (NAEA_PreferredCI*)create_node(5);
}

void asn1::InsertSubscriberDataArg::set_gprsSubscriptionData(){
	if(_gprsSubscriptionData == NULL) _gprsSubscriptionData = (GPRSSubscriptionData*)create_node(6);
}

void asn1::InsertSubscriberDataArg::set_roamingRestrictedInSgsnDueToUnsupportedFeature(){
	if(_roamingRestrictedInSgsnDueToUnsupportedFeature == NULL) _roamingRestrictedInSgsnDueToUnsupportedFeature = (Null*)create_node(7);
}

void asn1::InsertSubscriberDataArg::set_networkAccessMode(){
	if(_networkAccessMode == NULL) _networkAccessMode = (NetworkAccessMode*)create_node(8);
}

void asn1::InsertSubscriberDataArg::set_lsaInformation(){
	if(_lsaInformation == NULL) _lsaInformation = (LSAInformation*)create_node(9);
}

void asn1::InsertSubscriberDataArg::set_lmu_Indicator(){
	if(_lmu_Indicator == NULL) _lmu_Indicator = (Null*)create_node(10);
}

void asn1::InsertSubscriberDataArg::set_lcsInformation(){
	if(_lcsInformation == NULL) _lcsInformation = (LCSInformation*)create_node(11);
}

void asn1::InsertSubscriberDataArg::set_istAlertTimer(){
	if(_istAlertTimer == NULL) _istAlertTimer = (IST_AlertTimerValue*)create_node(12);
}

void asn1::InsertSubscriberDataArg::set_superChargerSupportedInHLR(){
	if(_superChargerSupportedInHLR == NULL) _superChargerSupportedInHLR = (AgeIndicator*)create_node(13);
}

void asn1::InsertSubscriberDataArg::set_mc_SS_Info(){
	if(_mc_SS_Info == NULL) _mc_SS_Info = (MC_SS_Info*)create_node(14);
}

void asn1::InsertSubscriberDataArg::set_cs_AllocationRetentionPriority(){
	if(_cs_AllocationRetentionPriority == NULL) _cs_AllocationRetentionPriority = (CS_AllocationRetentionPriority*)create_node(15);
}

void asn1::InsertSubscriberDataArg::set_sgsn_CAMEL_SubscriptionInfo(){
	if(_sgsn_CAMEL_SubscriptionInfo == NULL) _sgsn_CAMEL_SubscriptionInfo = (SGSN_CAMEL_SubscriptionInfo*)create_node(16);
}

void asn1::InsertSubscriberDataArg::set_chargingCharacteristics(){
	if(_chargingCharacteristics == NULL) _chargingCharacteristics = (ChargingCharacteristics*)create_node(17);
}

void asn1::InsertSubscriberDataArg::set_accessRestrictionData(){
	if(_accessRestrictionData == NULL) _accessRestrictionData = (AccessRestrictionData*)create_node(18);
}

void asn1::InsertSubscriberDataArg::set_ics_Indicator(){
	if(_ics_Indicator == NULL) _ics_Indicator = (Boolean*)create_node(19);
}

void asn1::InsertSubscriberDataArg::set_eps_SubscriptionData(){
	if(_eps_SubscriptionData == NULL) _eps_SubscriptionData = (EPS_SubscriptionData*)create_node(20);
}

void asn1::InsertSubscriberDataArg::set_csg_SubscriptionDataList(){
	if(_csg_SubscriptionDataList == NULL) _csg_SubscriptionDataList = (CSG_SubscriptionDataList*)create_node(21);
}

void asn1::InsertSubscriberDataArg::set_ue_ReachabilityRequestIndicator(){
	if(_ue_ReachabilityRequestIndicator == NULL) _ue_ReachabilityRequestIndicator = (Null*)create_node(22);
}

void asn1::InsertSubscriberDataArg::set_sgsn_Number(){
	if(_sgsn_Number == NULL) _sgsn_Number = (ISDN_AddressString*)create_node(23);
}

void asn1::InsertSubscriberDataArg::set_mme_Name(){
	if(_mme_Name == NULL) _mme_Name = (DiameterIdentity*)create_node(24);
}

void asn1::InsertSubscriberDataArg::set_subscribedPeriodicRAUTAUtimer(){
	if(_subscribedPeriodicRAUTAUtimer == NULL) _subscribedPeriodicRAUTAUtimer = (SubscribedPeriodicRAUTAUtimer*)create_node(25);
}

void asn1::InsertSubscriberDataArg::set_vplmnLIPAAllowed(){
	if(_vplmnLIPAAllowed == NULL) _vplmnLIPAAllowed = (Null*)create_node(26);
}

void asn1::InsertSubscriberDataArg::set_mdtUserConsent(){
	if(_mdtUserConsent == NULL) _mdtUserConsent = (Boolean*)create_node(27);
}

void asn1::InsertSubscriberDataArg::set_subscribedPeriodicLAUtimer(){
	if(_subscribedPeriodicLAUtimer == NULL) _subscribedPeriodicLAUtimer = (SubscribedPeriodicLAUtimer*)create_node(28);
}

void asn1::InsertSubscriberDataArg::set_vplmn_Csg_SubscriptionDataList(){
	if(_vplmn_Csg_SubscriptionDataList == NULL) _vplmn_Csg_SubscriptionDataList = (VPLMN_CSG_SubscriptionDataList*)create_node(29);
}

void asn1::InsertSubscriberDataArg::set_additionalMSISDN(){
	if(_additionalMSISDN == NULL) _additionalMSISDN = (ISDN_AddressString*)create_node(30);
}

void asn1::InsertSubscriberDataArg::set_psAndSMS_OnlyServiceProvision(){
	if(_psAndSMS_OnlyServiceProvision == NULL) _psAndSMS_OnlyServiceProvision = (Null*)create_node(31);
}

void asn1::InsertSubscriberDataArg::set_smsInSGSNAllowed(){
	if(_smsInSGSNAllowed == NULL) _smsInSGSNAllowed = (Null*)create_node(32);
}

//SubscribedPeriodicRAUTAUtimer
asn1::SubscribedPeriodicRAUTAUtimer::SubscribedPeriodicRAUTAUtimer(){
	strcpy(node_type_name, "SubscribedPeriodicRAUTAUtimer");

}
asn1::SubscribedPeriodicRAUTAUtimer::~SubscribedPeriodicRAUTAUtimer(){

}

//SubscribedPeriodicLAUtimer
asn1::SubscribedPeriodicLAUtimer::SubscribedPeriodicLAUtimer(){
	strcpy(node_type_name, "SubscribedPeriodicLAUtimer");

}
asn1::SubscribedPeriodicLAUtimer::~SubscribedPeriodicLAUtimer(){

}

//CSG_SubscriptionDataList
asn1::CSG_SubscriptionDataList::CSG_SubscriptionDataList(){
	strcpy(node_type_name, "CSG_SubscriptionDataList");

}
asn1::CSG_SubscriptionDataList::~CSG_SubscriptionDataList(){
}

asn1::CSG_SubscriptionData* asn1::CSG_SubscriptionDataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (CSG_SubscriptionData*)children[child_index]; else return NULL;
}

void asn1::CSG_SubscriptionDataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::CSG_SubscriptionDataList::create_node(unsigned int _index){
	children[_index] = new CSG_SubscriptionData();
	return children[_index];
}

asn1::ASN1Node* asn1::CSG_SubscriptionDataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new CSG_SubscriptionData());
		return children[children.size() - 1];
	}

}

//CSG_SubscriptionData
asn1::CSG_SubscriptionData::CSG_SubscriptionData(){
	strcpy(node_type_name, "CSG_SubscriptionData");
	// csg_Id
	_csg_Id = NULL;
	_csg_Id = new CSG_Id();
	children.push_back(_csg_Id);

	// expirationDate
	_expirationDate = NULL;
	children.push_back(_expirationDate);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// lipa_AllowedAPNList
	_lipa_AllowedAPNList = NULL;
	children.push_back(_lipa_AllowedAPNList);

	// plmn_Id
	_plmn_Id = NULL;
	children.push_back(_plmn_Id);


}
asn1::CSG_SubscriptionData::~CSG_SubscriptionData(){

}

asn1::ASN1Node* asn1::CSG_SubscriptionData::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_expirationDate = new Time();
			children[1] = _expirationDate;
			return _expirationDate;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_lipa_AllowedAPNList = new LIPA_AllowedAPNList();
			_lipa_AllowedAPNList->tlv->tag_class = CONTEXT_SPECIFIC;
			_lipa_AllowedAPNList->tlv->tag_value = 0;
			children[3] = _lipa_AllowedAPNList;
			return _lipa_AllowedAPNList;
		case 4:
			_plmn_Id = new PLMN_Id();
			_plmn_Id->tlv->tag_class = CONTEXT_SPECIFIC;
			_plmn_Id->tlv->tag_value = 1;
			children[4] = _plmn_Id;
			return _plmn_Id;

		default: return NULL;
	}
}

void asn1::CSG_SubscriptionData::set_expirationDate(){
	if(_expirationDate == NULL) _expirationDate = (Time*)create_node(1);
}

void asn1::CSG_SubscriptionData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::CSG_SubscriptionData::set_lipa_AllowedAPNList(){
	if(_lipa_AllowedAPNList == NULL) _lipa_AllowedAPNList = (LIPA_AllowedAPNList*)create_node(3);
}

void asn1::CSG_SubscriptionData::set_plmn_Id(){
	if(_plmn_Id == NULL) _plmn_Id = (PLMN_Id*)create_node(4);
}

//VPLMN_CSG_SubscriptionDataList
asn1::VPLMN_CSG_SubscriptionDataList::VPLMN_CSG_SubscriptionDataList(){
	strcpy(node_type_name, "VPLMN_CSG_SubscriptionDataList");

}
asn1::VPLMN_CSG_SubscriptionDataList::~VPLMN_CSG_SubscriptionDataList(){
}

asn1::CSG_SubscriptionData* asn1::VPLMN_CSG_SubscriptionDataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (CSG_SubscriptionData*)children[child_index]; else return NULL;
}

void asn1::VPLMN_CSG_SubscriptionDataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::VPLMN_CSG_SubscriptionDataList::create_node(unsigned int _index){
	children[_index] = new CSG_SubscriptionData();
	return children[_index];
}

asn1::ASN1Node* asn1::VPLMN_CSG_SubscriptionDataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new CSG_SubscriptionData());
		return children[children.size() - 1];
	}

}

//CSG_Id
asn1::CSG_Id::CSG_Id(){
	strcpy(node_type_name, "CSG_Id");

}
asn1::CSG_Id::~CSG_Id(){

}

//LIPA_AllowedAPNList
asn1::LIPA_AllowedAPNList::LIPA_AllowedAPNList(){
	strcpy(node_type_name, "LIPA_AllowedAPNList");

}
asn1::LIPA_AllowedAPNList::~LIPA_AllowedAPNList(){
}

asn1::APN* asn1::LIPA_AllowedAPNList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (APN*)children[child_index]; else return NULL;
}

void asn1::LIPA_AllowedAPNList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::LIPA_AllowedAPNList::create_node(unsigned int _index){
	children[_index] = new APN();
	return children[_index];
}

asn1::ASN1Node* asn1::LIPA_AllowedAPNList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new APN());
		return children[children.size() - 1];
	}

}

//maxNumOfLIPAAllowedAPN
asn1::maxNumOfLIPAAllowedAPN::maxNumOfLIPAAllowedAPN(){
	strcpy(node_type_name, "maxNumOfLIPAAllowedAPN");

}
asn1::maxNumOfLIPAAllowedAPN::~maxNumOfLIPAAllowedAPN(){

}

//EPS_SubscriptionData
asn1::EPS_SubscriptionData::EPS_SubscriptionData(){
	strcpy(node_type_name, "EPS_SubscriptionData");
	// apn_oi_Replacement
	_apn_oi_Replacement = NULL;
	children.push_back(_apn_oi_Replacement);

	// rfsp_id
	_rfsp_id = NULL;
	children.push_back(_rfsp_id);

	// ambr
	_ambr = NULL;
	children.push_back(_ambr);

	// apn_ConfigurationProfile
	_apn_ConfigurationProfile = NULL;
	children.push_back(_apn_ConfigurationProfile);

	// stn_sr
	_stn_sr = NULL;
	children.push_back(_stn_sr);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// mps_CSPriority
	_mps_CSPriority = NULL;
	children.push_back(_mps_CSPriority);

	// mps_EPSPriority
	_mps_EPSPriority = NULL;
	children.push_back(_mps_EPSPriority);

	// subscribed_vsrvcc
	_subscribed_vsrvcc = NULL;
	children.push_back(_subscribed_vsrvcc);


}
asn1::EPS_SubscriptionData::~EPS_SubscriptionData(){

}

asn1::ASN1Node* asn1::EPS_SubscriptionData::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_apn_oi_Replacement = new APN_OI_Replacement();
			_apn_oi_Replacement->tlv->tag_class = CONTEXT_SPECIFIC;
			_apn_oi_Replacement->tlv->tag_value = 0;
			children[0] = _apn_oi_Replacement;
			return _apn_oi_Replacement;
		case 1:
			_rfsp_id = new RFSP_ID();
			_rfsp_id->tlv->tag_class = CONTEXT_SPECIFIC;
			_rfsp_id->tlv->tag_value = 2;
			children[1] = _rfsp_id;
			return _rfsp_id;
		case 2:
			_ambr = new AMBR();
			_ambr->tlv->tag_class = CONTEXT_SPECIFIC;
			_ambr->tlv->tag_value = 3;
			children[2] = _ambr;
			return _ambr;
		case 3:
			_apn_ConfigurationProfile = new APN_ConfigurationProfile();
			_apn_ConfigurationProfile->tlv->tag_class = CONTEXT_SPECIFIC;
			_apn_ConfigurationProfile->tlv->tag_value = 4;
			children[3] = _apn_ConfigurationProfile;
			return _apn_ConfigurationProfile;
		case 4:
			_stn_sr = new ISDN_AddressString();
			_stn_sr->tlv->tag_class = CONTEXT_SPECIFIC;
			_stn_sr->tlv->tag_value = 6;
			children[4] = _stn_sr;
			return _stn_sr;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 5;
			children[5] = _extensionContainer;
			return _extensionContainer;
		case 6:
			_mps_CSPriority = new Null();
			_mps_CSPriority->tlv->tag_class = CONTEXT_SPECIFIC;
			_mps_CSPriority->tlv->tag_value = 7;
			children[6] = _mps_CSPriority;
			return _mps_CSPriority;
		case 7:
			_mps_EPSPriority = new Null();
			_mps_EPSPriority->tlv->tag_class = CONTEXT_SPECIFIC;
			_mps_EPSPriority->tlv->tag_value = 8;
			children[7] = _mps_EPSPriority;
			return _mps_EPSPriority;
		case 8:
			_subscribed_vsrvcc = new Null();
			_subscribed_vsrvcc->tlv->tag_class = CONTEXT_SPECIFIC;
			_subscribed_vsrvcc->tlv->tag_value = 9;
			children[8] = _subscribed_vsrvcc;
			return _subscribed_vsrvcc;

		default: return NULL;
	}
}

void asn1::EPS_SubscriptionData::set_apn_oi_Replacement(){
	if(_apn_oi_Replacement == NULL) _apn_oi_Replacement = (APN_OI_Replacement*)create_node(0);
}

void asn1::EPS_SubscriptionData::set_rfsp_id(){
	if(_rfsp_id == NULL) _rfsp_id = (RFSP_ID*)create_node(1);
}

void asn1::EPS_SubscriptionData::set_ambr(){
	if(_ambr == NULL) _ambr = (AMBR*)create_node(2);
}

void asn1::EPS_SubscriptionData::set_apn_ConfigurationProfile(){
	if(_apn_ConfigurationProfile == NULL) _apn_ConfigurationProfile = (APN_ConfigurationProfile*)create_node(3);
}

void asn1::EPS_SubscriptionData::set_stn_sr(){
	if(_stn_sr == NULL) _stn_sr = (ISDN_AddressString*)create_node(4);
}

void asn1::EPS_SubscriptionData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

void asn1::EPS_SubscriptionData::set_mps_CSPriority(){
	if(_mps_CSPriority == NULL) _mps_CSPriority = (Null*)create_node(6);
}

void asn1::EPS_SubscriptionData::set_mps_EPSPriority(){
	if(_mps_EPSPriority == NULL) _mps_EPSPriority = (Null*)create_node(7);
}

void asn1::EPS_SubscriptionData::set_subscribed_vsrvcc(){
	if(_subscribed_vsrvcc == NULL) _subscribed_vsrvcc = (Null*)create_node(8);
}

//APN_OI_Replacement
asn1::APN_OI_Replacement::APN_OI_Replacement(){
	strcpy(node_type_name, "APN_OI_Replacement");

}
asn1::APN_OI_Replacement::~APN_OI_Replacement(){

}

//RFSP_ID
asn1::RFSP_ID::RFSP_ID(){
	strcpy(node_type_name, "RFSP_ID");

}
asn1::RFSP_ID::~RFSP_ID(){

}

//APN_ConfigurationProfile
asn1::APN_ConfigurationProfile::APN_ConfigurationProfile(){
	strcpy(node_type_name, "APN_ConfigurationProfile");
	// defaultContext
	_defaultContext = NULL;
	_defaultContext = new ContextId();
	children.push_back(_defaultContext);

	// completeDataListIncluded
	_completeDataListIncluded = NULL;
	children.push_back(_completeDataListIncluded);

	// epsDataList
	_epsDataList = NULL;
	_epsDataList = new EPS_DataList();
	_epsDataList->tlv->tag_class = CONTEXT_SPECIFIC;
	_epsDataList->tlv->tag_value = 1;
	children.push_back(_epsDataList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::APN_ConfigurationProfile::~APN_ConfigurationProfile(){

}

asn1::ASN1Node* asn1::APN_ConfigurationProfile::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_completeDataListIncluded = new Null();
			children[1] = _completeDataListIncluded;
			return _completeDataListIncluded;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::APN_ConfigurationProfile::set_completeDataListIncluded(){
	if(_completeDataListIncluded == NULL) _completeDataListIncluded = (Null*)create_node(1);
}

void asn1::APN_ConfigurationProfile::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//EPS_DataList
asn1::EPS_DataList::EPS_DataList(){
	strcpy(node_type_name, "EPS_DataList");

}
asn1::EPS_DataList::~EPS_DataList(){
}

asn1::APN_Configuration* asn1::EPS_DataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (APN_Configuration*)children[child_index]; else return NULL;
}

void asn1::EPS_DataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::EPS_DataList::create_node(unsigned int _index){
	children[_index] = new APN_Configuration();
	return children[_index];
}

asn1::ASN1Node* asn1::EPS_DataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new APN_Configuration());
		return children[children.size() - 1];
	}

}

//maxNumOfAPN_Configurations
asn1::maxNumOfAPN_Configurations::maxNumOfAPN_Configurations(){
	strcpy(node_type_name, "maxNumOfAPN_Configurations");

}
asn1::maxNumOfAPN_Configurations::~maxNumOfAPN_Configurations(){

}

//APN_Configuration
asn1::APN_Configuration::APN_Configuration(){
	strcpy(node_type_name, "APN_Configuration");
	// contextId
	_contextId = NULL;
	_contextId = new ContextId();
	_contextId->tlv->tag_class = CONTEXT_SPECIFIC;
	_contextId->tlv->tag_value = 0;
	children.push_back(_contextId);

	// pdn_Type
	_pdn_Type = NULL;
	_pdn_Type = new PDN_Type();
	_pdn_Type->tlv->tag_class = CONTEXT_SPECIFIC;
	_pdn_Type->tlv->tag_value = 1;
	children.push_back(_pdn_Type);

	// servedPartyIP_IPv4_Address
	_servedPartyIP_IPv4_Address = NULL;
	children.push_back(_servedPartyIP_IPv4_Address);

	// apn
	_apn = NULL;
	_apn = new APN();
	_apn->tlv->tag_class = CONTEXT_SPECIFIC;
	_apn->tlv->tag_value = 3;
	children.push_back(_apn);

	// eps_qos_Subscribed
	_eps_qos_Subscribed = NULL;
	_eps_qos_Subscribed = new EPS_QoS_Subscribed();
	_eps_qos_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
	_eps_qos_Subscribed->tlv->tag_value = 4;
	children.push_back(_eps_qos_Subscribed);

	// pdn_gw_Identity
	_pdn_gw_Identity = NULL;
	children.push_back(_pdn_gw_Identity);

	// pdn_gw_AllocationType
	_pdn_gw_AllocationType = NULL;
	children.push_back(_pdn_gw_AllocationType);

	// vplmnAddressAllowed
	_vplmnAddressAllowed = NULL;
	children.push_back(_vplmnAddressAllowed);

	// chargingCharacteristics
	_chargingCharacteristics = NULL;
	children.push_back(_chargingCharacteristics);

	// ambr
	_ambr = NULL;
	children.push_back(_ambr);

	// specificAPNInfoList
	_specificAPNInfoList = NULL;
	children.push_back(_specificAPNInfoList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// servedPartyIP_IPv6_Address
	_servedPartyIP_IPv6_Address = NULL;
	children.push_back(_servedPartyIP_IPv6_Address);

	// apn_oi_Replacement
	_apn_oi_Replacement = NULL;
	children.push_back(_apn_oi_Replacement);

	// sipto_Permission
	_sipto_Permission = NULL;
	children.push_back(_sipto_Permission);

	// lipa_Permission
	_lipa_Permission = NULL;
	children.push_back(_lipa_Permission);


}
asn1::APN_Configuration::~APN_Configuration(){

}

asn1::ASN1Node* asn1::APN_Configuration::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_servedPartyIP_IPv4_Address = new PDP_Address();
			_servedPartyIP_IPv4_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_servedPartyIP_IPv4_Address->tlv->tag_value = 2;
			children[2] = _servedPartyIP_IPv4_Address;
			return _servedPartyIP_IPv4_Address;
		case 5:
			_pdn_gw_Identity = new PDN_GW_Identity();
			_pdn_gw_Identity->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdn_gw_Identity->tlv->tag_value = 5;
			children[5] = _pdn_gw_Identity;
			return _pdn_gw_Identity;
		case 6:
			_pdn_gw_AllocationType = new PDN_GW_AllocationType();
			_pdn_gw_AllocationType->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdn_gw_AllocationType->tlv->tag_value = 6;
			children[6] = _pdn_gw_AllocationType;
			return _pdn_gw_AllocationType;
		case 7:
			_vplmnAddressAllowed = new Null();
			_vplmnAddressAllowed->tlv->tag_class = CONTEXT_SPECIFIC;
			_vplmnAddressAllowed->tlv->tag_value = 7;
			children[7] = _vplmnAddressAllowed;
			return _vplmnAddressAllowed;
		case 8:
			_chargingCharacteristics = new ChargingCharacteristics();
			_chargingCharacteristics->tlv->tag_class = CONTEXT_SPECIFIC;
			_chargingCharacteristics->tlv->tag_value = 8;
			children[8] = _chargingCharacteristics;
			return _chargingCharacteristics;
		case 9:
			_ambr = new AMBR();
			_ambr->tlv->tag_class = CONTEXT_SPECIFIC;
			_ambr->tlv->tag_value = 9;
			children[9] = _ambr;
			return _ambr;
		case 10:
			_specificAPNInfoList = new SpecificAPNInfoList();
			_specificAPNInfoList->tlv->tag_class = CONTEXT_SPECIFIC;
			_specificAPNInfoList->tlv->tag_value = 10;
			children[10] = _specificAPNInfoList;
			return _specificAPNInfoList;
		case 11:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 11;
			children[11] = _extensionContainer;
			return _extensionContainer;
		case 12:
			_servedPartyIP_IPv6_Address = new PDP_Address();
			_servedPartyIP_IPv6_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_servedPartyIP_IPv6_Address->tlv->tag_value = 12;
			children[12] = _servedPartyIP_IPv6_Address;
			return _servedPartyIP_IPv6_Address;
		case 13:
			_apn_oi_Replacement = new APN_OI_Replacement();
			_apn_oi_Replacement->tlv->tag_class = CONTEXT_SPECIFIC;
			_apn_oi_Replacement->tlv->tag_value = 13;
			children[13] = _apn_oi_Replacement;
			return _apn_oi_Replacement;
		case 14:
			_sipto_Permission = new SIPTO_Permission();
			_sipto_Permission->tlv->tag_class = CONTEXT_SPECIFIC;
			_sipto_Permission->tlv->tag_value = 14;
			children[14] = _sipto_Permission;
			return _sipto_Permission;
		case 15:
			_lipa_Permission = new LIPA_Permission();
			_lipa_Permission->tlv->tag_class = CONTEXT_SPECIFIC;
			_lipa_Permission->tlv->tag_value = 15;
			children[15] = _lipa_Permission;
			return _lipa_Permission;

		default: return NULL;
	}
}

void asn1::APN_Configuration::set_servedPartyIP_IPv4_Address(){
	if(_servedPartyIP_IPv4_Address == NULL) _servedPartyIP_IPv4_Address = (PDP_Address*)create_node(2);
}

void asn1::APN_Configuration::set_pdn_gw_Identity(){
	if(_pdn_gw_Identity == NULL) _pdn_gw_Identity = (PDN_GW_Identity*)create_node(5);
}

void asn1::APN_Configuration::set_pdn_gw_AllocationType(){
	if(_pdn_gw_AllocationType == NULL) _pdn_gw_AllocationType = (PDN_GW_AllocationType*)create_node(6);
}

void asn1::APN_Configuration::set_vplmnAddressAllowed(){
	if(_vplmnAddressAllowed == NULL) _vplmnAddressAllowed = (Null*)create_node(7);
}

void asn1::APN_Configuration::set_chargingCharacteristics(){
	if(_chargingCharacteristics == NULL) _chargingCharacteristics = (ChargingCharacteristics*)create_node(8);
}

void asn1::APN_Configuration::set_ambr(){
	if(_ambr == NULL) _ambr = (AMBR*)create_node(9);
}

void asn1::APN_Configuration::set_specificAPNInfoList(){
	if(_specificAPNInfoList == NULL) _specificAPNInfoList = (SpecificAPNInfoList*)create_node(10);
}

void asn1::APN_Configuration::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(11);
}

void asn1::APN_Configuration::set_servedPartyIP_IPv6_Address(){
	if(_servedPartyIP_IPv6_Address == NULL) _servedPartyIP_IPv6_Address = (PDP_Address*)create_node(12);
}

void asn1::APN_Configuration::set_apn_oi_Replacement(){
	if(_apn_oi_Replacement == NULL) _apn_oi_Replacement = (APN_OI_Replacement*)create_node(13);
}

void asn1::APN_Configuration::set_sipto_Permission(){
	if(_sipto_Permission == NULL) _sipto_Permission = (SIPTO_Permission*)create_node(14);
}

void asn1::APN_Configuration::set_lipa_Permission(){
	if(_lipa_Permission == NULL) _lipa_Permission = (LIPA_Permission*)create_node(15);
}

//PDN_Type
asn1::PDN_Type::PDN_Type(){
	strcpy(node_type_name, "PDN_Type");

}
asn1::PDN_Type::~PDN_Type(){

}

//EPS_QoS_Subscribed
asn1::EPS_QoS_Subscribed::EPS_QoS_Subscribed(){
	strcpy(node_type_name, "EPS_QoS_Subscribed");
	// qos_Class_Identifier
	_qos_Class_Identifier = NULL;
	_qos_Class_Identifier = new QoS_Class_Identifier();
	_qos_Class_Identifier->tlv->tag_class = CONTEXT_SPECIFIC;
	_qos_Class_Identifier->tlv->tag_value = 0;
	children.push_back(_qos_Class_Identifier);

	// allocation_Retention_Priority
	_allocation_Retention_Priority = NULL;
	_allocation_Retention_Priority = new Allocation_Retention_Priority();
	_allocation_Retention_Priority->tlv->tag_class = CONTEXT_SPECIFIC;
	_allocation_Retention_Priority->tlv->tag_value = 1;
	children.push_back(_allocation_Retention_Priority);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::EPS_QoS_Subscribed::~EPS_QoS_Subscribed(){

}

asn1::ASN1Node* asn1::EPS_QoS_Subscribed::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::EPS_QoS_Subscribed::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//AMBR
asn1::AMBR::AMBR(){
	strcpy(node_type_name, "AMBR");
	// max_RequestedBandwidth_UL
	_max_RequestedBandwidth_UL = NULL;
	_max_RequestedBandwidth_UL = new Bandwidth();
	_max_RequestedBandwidth_UL->tlv->tag_class = CONTEXT_SPECIFIC;
	_max_RequestedBandwidth_UL->tlv->tag_value = 0;
	children.push_back(_max_RequestedBandwidth_UL);

	// max_RequestedBandwidth_DL
	_max_RequestedBandwidth_DL = NULL;
	_max_RequestedBandwidth_DL = new Bandwidth();
	_max_RequestedBandwidth_DL->tlv->tag_class = CONTEXT_SPECIFIC;
	_max_RequestedBandwidth_DL->tlv->tag_value = 1;
	children.push_back(_max_RequestedBandwidth_DL);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::AMBR::~AMBR(){

}

asn1::ASN1Node* asn1::AMBR::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::AMBR::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//SpecificAPNInfoList
asn1::SpecificAPNInfoList::SpecificAPNInfoList(){
	strcpy(node_type_name, "SpecificAPNInfoList");

}
asn1::SpecificAPNInfoList::~SpecificAPNInfoList(){
}

asn1::SpecificAPNInfo* asn1::SpecificAPNInfoList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (SpecificAPNInfo*)children[child_index]; else return NULL;
}

void asn1::SpecificAPNInfoList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::SpecificAPNInfoList::create_node(unsigned int _index){
	children[_index] = new SpecificAPNInfo();
	return children[_index];
}

asn1::ASN1Node* asn1::SpecificAPNInfoList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new SpecificAPNInfo());
		return children[children.size() - 1];
	}

}

//maxNumOfSpecificAPNInfos
asn1::maxNumOfSpecificAPNInfos::maxNumOfSpecificAPNInfos(){
	strcpy(node_type_name, "maxNumOfSpecificAPNInfos");

}
asn1::maxNumOfSpecificAPNInfos::~maxNumOfSpecificAPNInfos(){

}

//SpecificAPNInfo
asn1::SpecificAPNInfo::SpecificAPNInfo(){
	strcpy(node_type_name, "SpecificAPNInfo");
	// apn
	_apn = NULL;
	_apn = new APN();
	_apn->tlv->tag_class = CONTEXT_SPECIFIC;
	_apn->tlv->tag_value = 0;
	children.push_back(_apn);

	// pdn_gw_Identity
	_pdn_gw_Identity = NULL;
	_pdn_gw_Identity = new PDN_GW_Identity();
	_pdn_gw_Identity->tlv->tag_class = CONTEXT_SPECIFIC;
	_pdn_gw_Identity->tlv->tag_value = 1;
	children.push_back(_pdn_gw_Identity);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SpecificAPNInfo::~SpecificAPNInfo(){

}

asn1::ASN1Node* asn1::SpecificAPNInfo::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SpecificAPNInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//Bandwidth
asn1::Bandwidth::Bandwidth(){
	strcpy(node_type_name, "Bandwidth");

}
asn1::Bandwidth::~Bandwidth(){

}

//QoS_Class_Identifier
asn1::QoS_Class_Identifier::QoS_Class_Identifier(){
	strcpy(node_type_name, "QoS_Class_Identifier");

}
asn1::QoS_Class_Identifier::~QoS_Class_Identifier(){

}

//Allocation_Retention_Priority
asn1::Allocation_Retention_Priority::Allocation_Retention_Priority(){
	strcpy(node_type_name, "Allocation_Retention_Priority");
	// priority_level
	_priority_level = NULL;
	_priority_level = new Integer();
	_priority_level->tlv->tag_class = CONTEXT_SPECIFIC;
	_priority_level->tlv->tag_value = 0;
	children.push_back(_priority_level);

	// pre_emption_capability
	_pre_emption_capability = NULL;
	children.push_back(_pre_emption_capability);

	// pre_emption_vulnerability
	_pre_emption_vulnerability = NULL;
	children.push_back(_pre_emption_vulnerability);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::Allocation_Retention_Priority::~Allocation_Retention_Priority(){

}

asn1::ASN1Node* asn1::Allocation_Retention_Priority::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_pre_emption_capability = new Boolean();
			_pre_emption_capability->tlv->tag_class = CONTEXT_SPECIFIC;
			_pre_emption_capability->tlv->tag_value = 1;
			children[1] = _pre_emption_capability;
			return _pre_emption_capability;
		case 2:
			_pre_emption_vulnerability = new Boolean();
			_pre_emption_vulnerability->tlv->tag_class = CONTEXT_SPECIFIC;
			_pre_emption_vulnerability->tlv->tag_value = 2;
			children[2] = _pre_emption_vulnerability;
			return _pre_emption_vulnerability;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::Allocation_Retention_Priority::set_pre_emption_capability(){
	if(_pre_emption_capability == NULL) _pre_emption_capability = (Boolean*)create_node(1);
}

void asn1::Allocation_Retention_Priority::set_pre_emption_vulnerability(){
	if(_pre_emption_vulnerability == NULL) _pre_emption_vulnerability = (Boolean*)create_node(2);
}

void asn1::Allocation_Retention_Priority::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//PDN_GW_Identity
asn1::PDN_GW_Identity::PDN_GW_Identity(){
	strcpy(node_type_name, "PDN_GW_Identity");
	// pdn_gw_ipv4_Address
	_pdn_gw_ipv4_Address = NULL;
	children.push_back(_pdn_gw_ipv4_Address);

	// pdn_gw_ipv6_Address
	_pdn_gw_ipv6_Address = NULL;
	children.push_back(_pdn_gw_ipv6_Address);

	// pdn_gw_name
	_pdn_gw_name = NULL;
	children.push_back(_pdn_gw_name);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::PDN_GW_Identity::~PDN_GW_Identity(){

}

asn1::ASN1Node* asn1::PDN_GW_Identity::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_pdn_gw_ipv4_Address = new PDP_Address();
			_pdn_gw_ipv4_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdn_gw_ipv4_Address->tlv->tag_value = 0;
			children[0] = _pdn_gw_ipv4_Address;
			return _pdn_gw_ipv4_Address;
		case 1:
			_pdn_gw_ipv6_Address = new PDP_Address();
			_pdn_gw_ipv6_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdn_gw_ipv6_Address->tlv->tag_value = 1;
			children[1] = _pdn_gw_ipv6_Address;
			return _pdn_gw_ipv6_Address;
		case 2:
			_pdn_gw_name = new FQDN();
			_pdn_gw_name->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdn_gw_name->tlv->tag_value = 2;
			children[2] = _pdn_gw_name;
			return _pdn_gw_name;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::PDN_GW_Identity::set_pdn_gw_ipv4_Address(){
	if(_pdn_gw_ipv4_Address == NULL) _pdn_gw_ipv4_Address = (PDP_Address*)create_node(0);
}

void asn1::PDN_GW_Identity::set_pdn_gw_ipv6_Address(){
	if(_pdn_gw_ipv6_Address == NULL) _pdn_gw_ipv6_Address = (PDP_Address*)create_node(1);
}

void asn1::PDN_GW_Identity::set_pdn_gw_name(){
	if(_pdn_gw_name == NULL) _pdn_gw_name = (FQDN*)create_node(2);
}

void asn1::PDN_GW_Identity::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//FQDN
asn1::FQDN::FQDN(){
	strcpy(node_type_name, "FQDN");

}
asn1::FQDN::~FQDN(){

}

//PDN_GW_AllocationType
asn1::PDN_GW_AllocationType::PDN_GW_AllocationType(){
	strcpy(node_type_name, "PDN_GW_AllocationType");

}
asn1::PDN_GW_AllocationType::~PDN_GW_AllocationType(){

}

//AccessRestrictionData
asn1::AccessRestrictionData::AccessRestrictionData(){
	strcpy(node_type_name, "AccessRestrictionData");

}
asn1::AccessRestrictionData::~AccessRestrictionData(){

}

//CS_AllocationRetentionPriority
asn1::CS_AllocationRetentionPriority::CS_AllocationRetentionPriority(){
	strcpy(node_type_name, "CS_AllocationRetentionPriority");

}
asn1::CS_AllocationRetentionPriority::~CS_AllocationRetentionPriority(){

}

//IST_AlertTimerValue
asn1::IST_AlertTimerValue::IST_AlertTimerValue(){
	strcpy(node_type_name, "IST_AlertTimerValue");

}
asn1::IST_AlertTimerValue::~IST_AlertTimerValue(){

}

//LCSInformation
asn1::LCSInformation::LCSInformation(){
	strcpy(node_type_name, "LCSInformation");
	// gmlc_List
	_gmlc_List = NULL;
	children.push_back(_gmlc_List);

	// lcs_PrivacyExceptionList
	_lcs_PrivacyExceptionList = NULL;
	children.push_back(_lcs_PrivacyExceptionList);

	// molr_List
	_molr_List = NULL;
	children.push_back(_molr_List);

	// add_lcs_PrivacyExceptionList
	_add_lcs_PrivacyExceptionList = NULL;
	children.push_back(_add_lcs_PrivacyExceptionList);


}
asn1::LCSInformation::~LCSInformation(){

}

asn1::ASN1Node* asn1::LCSInformation::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_gmlc_List = new GMLC_List();
			_gmlc_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_gmlc_List->tlv->tag_value = 0;
			children[0] = _gmlc_List;
			return _gmlc_List;
		case 1:
			_lcs_PrivacyExceptionList = new LCS_PrivacyExceptionList();
			_lcs_PrivacyExceptionList->tlv->tag_class = CONTEXT_SPECIFIC;
			_lcs_PrivacyExceptionList->tlv->tag_value = 1;
			children[1] = _lcs_PrivacyExceptionList;
			return _lcs_PrivacyExceptionList;
		case 2:
			_molr_List = new MOLR_List();
			_molr_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_molr_List->tlv->tag_value = 2;
			children[2] = _molr_List;
			return _molr_List;
		case 3:
			_add_lcs_PrivacyExceptionList = new LCS_PrivacyExceptionList();
			_add_lcs_PrivacyExceptionList->tlv->tag_class = CONTEXT_SPECIFIC;
			_add_lcs_PrivacyExceptionList->tlv->tag_value = 3;
			children[3] = _add_lcs_PrivacyExceptionList;
			return _add_lcs_PrivacyExceptionList;

		default: return NULL;
	}
}

void asn1::LCSInformation::set_gmlc_List(){
	if(_gmlc_List == NULL) _gmlc_List = (GMLC_List*)create_node(0);
}

void asn1::LCSInformation::set_lcs_PrivacyExceptionList(){
	if(_lcs_PrivacyExceptionList == NULL) _lcs_PrivacyExceptionList = (LCS_PrivacyExceptionList*)create_node(1);
}

void asn1::LCSInformation::set_molr_List(){
	if(_molr_List == NULL) _molr_List = (MOLR_List*)create_node(2);
}

void asn1::LCSInformation::set_add_lcs_PrivacyExceptionList(){
	if(_add_lcs_PrivacyExceptionList == NULL) _add_lcs_PrivacyExceptionList = (LCS_PrivacyExceptionList*)create_node(3);
}

//GMLC_List
asn1::GMLC_List::GMLC_List(){
	strcpy(node_type_name, "GMLC_List");

}
asn1::GMLC_List::~GMLC_List(){
}

asn1::ISDN_AddressString* asn1::GMLC_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ISDN_AddressString*)children[child_index]; else return NULL;
}

void asn1::GMLC_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::GMLC_List::create_node(unsigned int _index){
	children[_index] = new ISDN_AddressString();
	return children[_index];
}

asn1::ASN1Node* asn1::GMLC_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ISDN_AddressString());
		return children[children.size() - 1];
	}

}

//maxNumOfGMLC
asn1::maxNumOfGMLC::maxNumOfGMLC(){
	strcpy(node_type_name, "maxNumOfGMLC");

}
asn1::maxNumOfGMLC::~maxNumOfGMLC(){

}

//NetworkAccessMode
asn1::NetworkAccessMode::NetworkAccessMode(){
	strcpy(node_type_name, "NetworkAccessMode");

}
asn1::NetworkAccessMode::~NetworkAccessMode(){

}

//GPRSDataList
asn1::GPRSDataList::GPRSDataList(){
	strcpy(node_type_name, "GPRSDataList");

}
asn1::GPRSDataList::~GPRSDataList(){
}

asn1::PDP_Context* asn1::GPRSDataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (PDP_Context*)children[child_index]; else return NULL;
}

void asn1::GPRSDataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::GPRSDataList::create_node(unsigned int _index){
	children[_index] = new PDP_Context();
	return children[_index];
}

asn1::ASN1Node* asn1::GPRSDataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new PDP_Context());
		return children[children.size() - 1];
	}

}

//maxNumOfPDP_Contexts
asn1::maxNumOfPDP_Contexts::maxNumOfPDP_Contexts(){
	strcpy(node_type_name, "maxNumOfPDP_Contexts");

}
asn1::maxNumOfPDP_Contexts::~maxNumOfPDP_Contexts(){

}

//PDP_Context
asn1::PDP_Context::PDP_Context(){
	strcpy(node_type_name, "PDP_Context");
	// pdp_ContextId
	_pdp_ContextId = NULL;
	_pdp_ContextId = new ContextId();
	children.push_back(_pdp_ContextId);

	// pdp_Type
	_pdp_Type = NULL;
	_pdp_Type = new PDP_Type();
	_pdp_Type->tlv->tag_class = CONTEXT_SPECIFIC;
	_pdp_Type->tlv->tag_value = 16;
	children.push_back(_pdp_Type);

	// pdp_Address
	_pdp_Address = NULL;
	children.push_back(_pdp_Address);

	// qos_Subscribed
	_qos_Subscribed = NULL;
	_qos_Subscribed = new QoS_Subscribed();
	_qos_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
	_qos_Subscribed->tlv->tag_value = 18;
	children.push_back(_qos_Subscribed);

	// vplmnAddressAllowed
	_vplmnAddressAllowed = NULL;
	children.push_back(_vplmnAddressAllowed);

	// apn
	_apn = NULL;
	_apn = new APN();
	_apn->tlv->tag_class = CONTEXT_SPECIFIC;
	_apn->tlv->tag_value = 20;
	children.push_back(_apn);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// ext_QoS_Subscribed
	_ext_QoS_Subscribed = NULL;
	children.push_back(_ext_QoS_Subscribed);

	// pdp_ChargingCharacteristics
	_pdp_ChargingCharacteristics = NULL;
	children.push_back(_pdp_ChargingCharacteristics);

	// ext2_QoS_Subscribed
	_ext2_QoS_Subscribed = NULL;
	children.push_back(_ext2_QoS_Subscribed);

	// ext3_QoS_Subscribed
	_ext3_QoS_Subscribed = NULL;
	children.push_back(_ext3_QoS_Subscribed);

	// ext4_QoS_Subscribed
	_ext4_QoS_Subscribed = NULL;
	children.push_back(_ext4_QoS_Subscribed);

	// apn_oi_Replacement
	_apn_oi_Replacement = NULL;
	children.push_back(_apn_oi_Replacement);

	// ext_pdp_Type
	_ext_pdp_Type = NULL;
	children.push_back(_ext_pdp_Type);

	// ext_pdp_Address
	_ext_pdp_Address = NULL;
	children.push_back(_ext_pdp_Address);

	// ambr
	_ambr = NULL;
	children.push_back(_ambr);

	// sipto_Permission
	_sipto_Permission = NULL;
	children.push_back(_sipto_Permission);

	// lipa_Permission
	_lipa_Permission = NULL;
	children.push_back(_lipa_Permission);


}
asn1::PDP_Context::~PDP_Context(){

}

asn1::ASN1Node* asn1::PDP_Context::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_pdp_Address = new PDP_Address();
			_pdp_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdp_Address->tlv->tag_value = 17;
			children[2] = _pdp_Address;
			return _pdp_Address;
		case 4:
			_vplmnAddressAllowed = new Null();
			_vplmnAddressAllowed->tlv->tag_class = CONTEXT_SPECIFIC;
			_vplmnAddressAllowed->tlv->tag_value = 19;
			children[4] = _vplmnAddressAllowed;
			return _vplmnAddressAllowed;
		case 6:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 21;
			children[6] = _extensionContainer;
			return _extensionContainer;
		case 7:
			_ext_QoS_Subscribed = new Ext_QoS_Subscribed();
			_ext_QoS_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
			_ext_QoS_Subscribed->tlv->tag_value = 0;
			children[7] = _ext_QoS_Subscribed;
			return _ext_QoS_Subscribed;
		case 8:
			_pdp_ChargingCharacteristics = new ChargingCharacteristics();
			_pdp_ChargingCharacteristics->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdp_ChargingCharacteristics->tlv->tag_value = 1;
			children[8] = _pdp_ChargingCharacteristics;
			return _pdp_ChargingCharacteristics;
		case 9:
			_ext2_QoS_Subscribed = new Ext2_QoS_Subscribed();
			_ext2_QoS_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
			_ext2_QoS_Subscribed->tlv->tag_value = 2;
			children[9] = _ext2_QoS_Subscribed;
			return _ext2_QoS_Subscribed;
		case 10:
			_ext3_QoS_Subscribed = new Ext3_QoS_Subscribed();
			_ext3_QoS_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
			_ext3_QoS_Subscribed->tlv->tag_value = 3;
			children[10] = _ext3_QoS_Subscribed;
			return _ext3_QoS_Subscribed;
		case 11:
			_ext4_QoS_Subscribed = new Ext4_QoS_Subscribed();
			_ext4_QoS_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
			_ext4_QoS_Subscribed->tlv->tag_value = 4;
			children[11] = _ext4_QoS_Subscribed;
			return _ext4_QoS_Subscribed;
		case 12:
			_apn_oi_Replacement = new APN_OI_Replacement();
			_apn_oi_Replacement->tlv->tag_class = CONTEXT_SPECIFIC;
			_apn_oi_Replacement->tlv->tag_value = 5;
			children[12] = _apn_oi_Replacement;
			return _apn_oi_Replacement;
		case 13:
			_ext_pdp_Type = new Ext_PDP_Type();
			_ext_pdp_Type->tlv->tag_class = CONTEXT_SPECIFIC;
			_ext_pdp_Type->tlv->tag_value = 6;
			children[13] = _ext_pdp_Type;
			return _ext_pdp_Type;
		case 14:
			_ext_pdp_Address = new PDP_Address();
			_ext_pdp_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_ext_pdp_Address->tlv->tag_value = 7;
			children[14] = _ext_pdp_Address;
			return _ext_pdp_Address;
		case 15:
			_ambr = new AMBR();
			_ambr->tlv->tag_class = CONTEXT_SPECIFIC;
			_ambr->tlv->tag_value = 10;
			children[15] = _ambr;
			return _ambr;
		case 16:
			_sipto_Permission = new SIPTO_Permission();
			_sipto_Permission->tlv->tag_class = CONTEXT_SPECIFIC;
			_sipto_Permission->tlv->tag_value = 8;
			children[16] = _sipto_Permission;
			return _sipto_Permission;
		case 17:
			_lipa_Permission = new LIPA_Permission();
			_lipa_Permission->tlv->tag_class = CONTEXT_SPECIFIC;
			_lipa_Permission->tlv->tag_value = 9;
			children[17] = _lipa_Permission;
			return _lipa_Permission;

		default: return NULL;
	}
}

void asn1::PDP_Context::set_pdp_Address(){
	if(_pdp_Address == NULL) _pdp_Address = (PDP_Address*)create_node(2);
}

void asn1::PDP_Context::set_vplmnAddressAllowed(){
	if(_vplmnAddressAllowed == NULL) _vplmnAddressAllowed = (Null*)create_node(4);
}

void asn1::PDP_Context::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(6);
}

void asn1::PDP_Context::set_ext_QoS_Subscribed(){
	if(_ext_QoS_Subscribed == NULL) _ext_QoS_Subscribed = (Ext_QoS_Subscribed*)create_node(7);
}

void asn1::PDP_Context::set_pdp_ChargingCharacteristics(){
	if(_pdp_ChargingCharacteristics == NULL) _pdp_ChargingCharacteristics = (ChargingCharacteristics*)create_node(8);
}

void asn1::PDP_Context::set_ext2_QoS_Subscribed(){
	if(_ext2_QoS_Subscribed == NULL) _ext2_QoS_Subscribed = (Ext2_QoS_Subscribed*)create_node(9);
}

void asn1::PDP_Context::set_ext3_QoS_Subscribed(){
	if(_ext3_QoS_Subscribed == NULL) _ext3_QoS_Subscribed = (Ext3_QoS_Subscribed*)create_node(10);
}

void asn1::PDP_Context::set_ext4_QoS_Subscribed(){
	if(_ext4_QoS_Subscribed == NULL) _ext4_QoS_Subscribed = (Ext4_QoS_Subscribed*)create_node(11);
}

void asn1::PDP_Context::set_apn_oi_Replacement(){
	if(_apn_oi_Replacement == NULL) _apn_oi_Replacement = (APN_OI_Replacement*)create_node(12);
}

void asn1::PDP_Context::set_ext_pdp_Type(){
	if(_ext_pdp_Type == NULL) _ext_pdp_Type = (Ext_PDP_Type*)create_node(13);
}

void asn1::PDP_Context::set_ext_pdp_Address(){
	if(_ext_pdp_Address == NULL) _ext_pdp_Address = (PDP_Address*)create_node(14);
}

void asn1::PDP_Context::set_ambr(){
	if(_ambr == NULL) _ambr = (AMBR*)create_node(15);
}

void asn1::PDP_Context::set_sipto_Permission(){
	if(_sipto_Permission == NULL) _sipto_Permission = (SIPTO_Permission*)create_node(16);
}

void asn1::PDP_Context::set_lipa_Permission(){
	if(_lipa_Permission == NULL) _lipa_Permission = (LIPA_Permission*)create_node(17);
}

//SIPTO_Permission
asn1::SIPTO_Permission::SIPTO_Permission(){
	strcpy(node_type_name, "SIPTO_Permission");

}
asn1::SIPTO_Permission::~SIPTO_Permission(){

}

//LIPA_Permission
asn1::LIPA_Permission::LIPA_Permission(){
	strcpy(node_type_name, "LIPA_Permission");

}
asn1::LIPA_Permission::~LIPA_Permission(){

}

//ContextId
asn1::ContextId::ContextId(){
	strcpy(node_type_name, "ContextId");

}
asn1::ContextId::~ContextId(){

}

//GPRSSubscriptionData
asn1::GPRSSubscriptionData::GPRSSubscriptionData(){
	strcpy(node_type_name, "GPRSSubscriptionData");
	// completeDataListIncluded
	_completeDataListIncluded = NULL;
	children.push_back(_completeDataListIncluded);

	// gprsDataList
	_gprsDataList = NULL;
	_gprsDataList = new GPRSDataList();
	_gprsDataList->tlv->tag_class = CONTEXT_SPECIFIC;
	_gprsDataList->tlv->tag_value = 1;
	children.push_back(_gprsDataList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// apn_oi_Replacement
	_apn_oi_Replacement = NULL;
	children.push_back(_apn_oi_Replacement);


}
asn1::GPRSSubscriptionData::~GPRSSubscriptionData(){

}

asn1::ASN1Node* asn1::GPRSSubscriptionData::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_completeDataListIncluded = new Null();
			children[0] = _completeDataListIncluded;
			return _completeDataListIncluded;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_apn_oi_Replacement = new APN_OI_Replacement();
			_apn_oi_Replacement->tlv->tag_class = CONTEXT_SPECIFIC;
			_apn_oi_Replacement->tlv->tag_value = 3;
			children[3] = _apn_oi_Replacement;
			return _apn_oi_Replacement;

		default: return NULL;
	}
}

void asn1::GPRSSubscriptionData::set_completeDataListIncluded(){
	if(_completeDataListIncluded == NULL) _completeDataListIncluded = (Null*)create_node(0);
}

void asn1::GPRSSubscriptionData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::GPRSSubscriptionData::set_apn_oi_Replacement(){
	if(_apn_oi_Replacement == NULL) _apn_oi_Replacement = (APN_OI_Replacement*)create_node(3);
}

//SGSN_CAMEL_SubscriptionInfo
asn1::SGSN_CAMEL_SubscriptionInfo::SGSN_CAMEL_SubscriptionInfo(){
	strcpy(node_type_name, "SGSN_CAMEL_SubscriptionInfo");
	// gprs_CSI
	_gprs_CSI = NULL;
	children.push_back(_gprs_CSI);

	// mo_sms_CSI
	_mo_sms_CSI = NULL;
	children.push_back(_mo_sms_CSI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// mt_sms_CSI
	_mt_sms_CSI = NULL;
	children.push_back(_mt_sms_CSI);

	// mt_smsCAMELTDP_CriteriaList
	_mt_smsCAMELTDP_CriteriaList = NULL;
	children.push_back(_mt_smsCAMELTDP_CriteriaList);

	// mg_csi
	_mg_csi = NULL;
	children.push_back(_mg_csi);


}
asn1::SGSN_CAMEL_SubscriptionInfo::~SGSN_CAMEL_SubscriptionInfo(){

}

asn1::ASN1Node* asn1::SGSN_CAMEL_SubscriptionInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_gprs_CSI = new GPRS_CSI();
			_gprs_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprs_CSI->tlv->tag_value = 0;
			children[0] = _gprs_CSI;
			return _gprs_CSI;
		case 1:
			_mo_sms_CSI = new SMS_CSI();
			_mo_sms_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_mo_sms_CSI->tlv->tag_value = 1;
			children[1] = _mo_sms_CSI;
			return _mo_sms_CSI;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_mt_sms_CSI = new SMS_CSI();
			_mt_sms_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_mt_sms_CSI->tlv->tag_value = 3;
			children[3] = _mt_sms_CSI;
			return _mt_sms_CSI;
		case 4:
			_mt_smsCAMELTDP_CriteriaList = new MT_smsCAMELTDP_CriteriaList();
			_mt_smsCAMELTDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_mt_smsCAMELTDP_CriteriaList->tlv->tag_value = 4;
			children[4] = _mt_smsCAMELTDP_CriteriaList;
			return _mt_smsCAMELTDP_CriteriaList;
		case 5:
			_mg_csi = new MG_CSI();
			_mg_csi->tlv->tag_class = CONTEXT_SPECIFIC;
			_mg_csi->tlv->tag_value = 5;
			children[5] = _mg_csi;
			return _mg_csi;

		default: return NULL;
	}
}

void asn1::SGSN_CAMEL_SubscriptionInfo::set_gprs_CSI(){
	if(_gprs_CSI == NULL) _gprs_CSI = (GPRS_CSI*)create_node(0);
}

void asn1::SGSN_CAMEL_SubscriptionInfo::set_mo_sms_CSI(){
	if(_mo_sms_CSI == NULL) _mo_sms_CSI = (SMS_CSI*)create_node(1);
}

void asn1::SGSN_CAMEL_SubscriptionInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::SGSN_CAMEL_SubscriptionInfo::set_mt_sms_CSI(){
	if(_mt_sms_CSI == NULL) _mt_sms_CSI = (SMS_CSI*)create_node(3);
}

void asn1::SGSN_CAMEL_SubscriptionInfo::set_mt_smsCAMELTDP_CriteriaList(){
	if(_mt_smsCAMELTDP_CriteriaList == NULL) _mt_smsCAMELTDP_CriteriaList = (MT_smsCAMELTDP_CriteriaList*)create_node(4);
}

void asn1::SGSN_CAMEL_SubscriptionInfo::set_mg_csi(){
	if(_mg_csi == NULL) _mg_csi = (MG_CSI*)create_node(5);
}

//GPRS_CSI
asn1::GPRS_CSI::GPRS_CSI(){
	strcpy(node_type_name, "GPRS_CSI");
	// gprs_CamelTDPDataList
	_gprs_CamelTDPDataList = NULL;
	children.push_back(_gprs_CamelTDPDataList);

	// camelCapabilityHandling
	_camelCapabilityHandling = NULL;
	children.push_back(_camelCapabilityHandling);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// csi_Active
	_csi_Active = NULL;
	children.push_back(_csi_Active);


}
asn1::GPRS_CSI::~GPRS_CSI(){

}

asn1::ASN1Node* asn1::GPRS_CSI::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_gprs_CamelTDPDataList = new GPRS_CamelTDPDataList();
			_gprs_CamelTDPDataList->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprs_CamelTDPDataList->tlv->tag_value = 0;
			children[0] = _gprs_CamelTDPDataList;
			return _gprs_CamelTDPDataList;
		case 1:
			_camelCapabilityHandling = new CamelCapabilityHandling();
			_camelCapabilityHandling->tlv->tag_class = CONTEXT_SPECIFIC;
			_camelCapabilityHandling->tlv->tag_value = 1;
			children[1] = _camelCapabilityHandling;
			return _camelCapabilityHandling;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 3;
			children[3] = _notificationToCSE;
			return _notificationToCSE;
		case 4:
			_csi_Active = new Null();
			_csi_Active->tlv->tag_class = CONTEXT_SPECIFIC;
			_csi_Active->tlv->tag_value = 4;
			children[4] = _csi_Active;
			return _csi_Active;

		default: return NULL;
	}
}

void asn1::GPRS_CSI::set_gprs_CamelTDPDataList(){
	if(_gprs_CamelTDPDataList == NULL) _gprs_CamelTDPDataList = (GPRS_CamelTDPDataList*)create_node(0);
}

void asn1::GPRS_CSI::set_camelCapabilityHandling(){
	if(_camelCapabilityHandling == NULL) _camelCapabilityHandling = (CamelCapabilityHandling*)create_node(1);
}

void asn1::GPRS_CSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::GPRS_CSI::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(3);
}

void asn1::GPRS_CSI::set_csi_Active(){
	if(_csi_Active == NULL) _csi_Active = (Null*)create_node(4);
}

//GPRS_CamelTDPDataList
asn1::GPRS_CamelTDPDataList::GPRS_CamelTDPDataList(){
	strcpy(node_type_name, "GPRS_CamelTDPDataList");

}
asn1::GPRS_CamelTDPDataList::~GPRS_CamelTDPDataList(){
}

asn1::GPRS_CamelTDPData* asn1::GPRS_CamelTDPDataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (GPRS_CamelTDPData*)children[child_index]; else return NULL;
}

void asn1::GPRS_CamelTDPDataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::GPRS_CamelTDPDataList::create_node(unsigned int _index){
	children[_index] = new GPRS_CamelTDPData();
	return children[_index];
}

asn1::ASN1Node* asn1::GPRS_CamelTDPDataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new GPRS_CamelTDPData());
		return children[children.size() - 1];
	}

}

//GPRS_CamelTDPData
asn1::GPRS_CamelTDPData::GPRS_CamelTDPData(){
	strcpy(node_type_name, "GPRS_CamelTDPData");
	// gprs_TriggerDetectionPoint
	_gprs_TriggerDetectionPoint = NULL;
	_gprs_TriggerDetectionPoint = new GPRS_TriggerDetectionPoint();
	_gprs_TriggerDetectionPoint->tlv->tag_class = CONTEXT_SPECIFIC;
	_gprs_TriggerDetectionPoint->tlv->tag_value = 0;
	children.push_back(_gprs_TriggerDetectionPoint);

	// serviceKey
	_serviceKey = NULL;
	_serviceKey = new ServiceKey();
	_serviceKey->tlv->tag_class = CONTEXT_SPECIFIC;
	_serviceKey->tlv->tag_value = 1;
	children.push_back(_serviceKey);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	_gsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsmSCF_Address->tlv->tag_value = 2;
	children.push_back(_gsmSCF_Address);

	// defaultSessionHandling
	_defaultSessionHandling = NULL;
	_defaultSessionHandling = new DefaultGPRS_Handling();
	_defaultSessionHandling->tlv->tag_class = CONTEXT_SPECIFIC;
	_defaultSessionHandling->tlv->tag_value = 3;
	children.push_back(_defaultSessionHandling);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::GPRS_CamelTDPData::~GPRS_CamelTDPData(){

}

asn1::ASN1Node* asn1::GPRS_CamelTDPData::create_node(unsigned int _index){
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

void asn1::GPRS_CamelTDPData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//DefaultGPRS_Handling
asn1::DefaultGPRS_Handling::DefaultGPRS_Handling(){
	strcpy(node_type_name, "DefaultGPRS_Handling");

}
asn1::DefaultGPRS_Handling::~DefaultGPRS_Handling(){

}

//GPRS_TriggerDetectionPoint
asn1::GPRS_TriggerDetectionPoint::GPRS_TriggerDetectionPoint(){
	strcpy(node_type_name, "GPRS_TriggerDetectionPoint");

}
asn1::GPRS_TriggerDetectionPoint::~GPRS_TriggerDetectionPoint(){

}

//APN
asn1::APN::APN(){
	strcpy(node_type_name, "APN");

}
asn1::APN::~APN(){

}

//PDP_Type
asn1::PDP_Type::PDP_Type(){
	strcpy(node_type_name, "PDP_Type");

}
asn1::PDP_Type::~PDP_Type(){

}

//Ext_PDP_Type
asn1::Ext_PDP_Type::Ext_PDP_Type(){
	strcpy(node_type_name, "Ext_PDP_Type");

}
asn1::Ext_PDP_Type::~Ext_PDP_Type(){

}

//PDP_Address
asn1::PDP_Address::PDP_Address(){
	strcpy(node_type_name, "PDP_Address");

}
asn1::PDP_Address::~PDP_Address(){

}

//QoS_Subscribed
asn1::QoS_Subscribed::QoS_Subscribed(){
	strcpy(node_type_name, "QoS_Subscribed");

}
asn1::QoS_Subscribed::~QoS_Subscribed(){

}

//Ext_QoS_Subscribed
asn1::Ext_QoS_Subscribed::Ext_QoS_Subscribed(){
	strcpy(node_type_name, "Ext_QoS_Subscribed");

}
asn1::Ext_QoS_Subscribed::~Ext_QoS_Subscribed(){

}

//Ext2_QoS_Subscribed
asn1::Ext2_QoS_Subscribed::Ext2_QoS_Subscribed(){
	strcpy(node_type_name, "Ext2_QoS_Subscribed");

}
asn1::Ext2_QoS_Subscribed::~Ext2_QoS_Subscribed(){

}

//Ext3_QoS_Subscribed
asn1::Ext3_QoS_Subscribed::Ext3_QoS_Subscribed(){
	strcpy(node_type_name, "Ext3_QoS_Subscribed");

}
asn1::Ext3_QoS_Subscribed::~Ext3_QoS_Subscribed(){

}

//Ext4_QoS_Subscribed
asn1::Ext4_QoS_Subscribed::Ext4_QoS_Subscribed(){
	strcpy(node_type_name, "Ext4_QoS_Subscribed");

}
asn1::Ext4_QoS_Subscribed::~Ext4_QoS_Subscribed(){

}

//ChargingCharacteristics
asn1::ChargingCharacteristics::ChargingCharacteristics(){
	strcpy(node_type_name, "ChargingCharacteristics");

}
asn1::ChargingCharacteristics::~ChargingCharacteristics(){

}

//LSAOnlyAccessIndicator
asn1::LSAOnlyAccessIndicator::LSAOnlyAccessIndicator(){
	strcpy(node_type_name, "LSAOnlyAccessIndicator");

}
asn1::LSAOnlyAccessIndicator::~LSAOnlyAccessIndicator(){

}

//LSADataList
asn1::LSADataList::LSADataList(){
	strcpy(node_type_name, "LSADataList");

}
asn1::LSADataList::~LSADataList(){
}

asn1::LSAData* asn1::LSADataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (LSAData*)children[child_index]; else return NULL;
}

void asn1::LSADataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::LSADataList::create_node(unsigned int _index){
	children[_index] = new LSAData();
	return children[_index];
}

asn1::ASN1Node* asn1::LSADataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new LSAData());
		return children[children.size() - 1];
	}

}

//maxNumOfLSAs
asn1::maxNumOfLSAs::maxNumOfLSAs(){
	strcpy(node_type_name, "maxNumOfLSAs");

}
asn1::maxNumOfLSAs::~maxNumOfLSAs(){

}

//LSAData
asn1::LSAData::LSAData(){
	strcpy(node_type_name, "LSAData");
	// lsaIdentity
	_lsaIdentity = NULL;
	_lsaIdentity = new LSAIdentity();
	_lsaIdentity->tlv->tag_class = CONTEXT_SPECIFIC;
	_lsaIdentity->tlv->tag_value = 0;
	children.push_back(_lsaIdentity);

	// lsaAttributes
	_lsaAttributes = NULL;
	_lsaAttributes = new LSAAttributes();
	_lsaAttributes->tlv->tag_class = CONTEXT_SPECIFIC;
	_lsaAttributes->tlv->tag_value = 1;
	children.push_back(_lsaAttributes);

	// lsaActiveModeIndicator
	_lsaActiveModeIndicator = NULL;
	children.push_back(_lsaActiveModeIndicator);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::LSAData::~LSAData(){

}

asn1::ASN1Node* asn1::LSAData::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_lsaActiveModeIndicator = new Null();
			_lsaActiveModeIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_lsaActiveModeIndicator->tlv->tag_value = 2;
			children[2] = _lsaActiveModeIndicator;
			return _lsaActiveModeIndicator;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::LSAData::set_lsaActiveModeIndicator(){
	if(_lsaActiveModeIndicator == NULL) _lsaActiveModeIndicator = (Null*)create_node(2);
}

void asn1::LSAData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//LSAInformation
asn1::LSAInformation::LSAInformation(){
	strcpy(node_type_name, "LSAInformation");
	// completeDataListIncluded
	_completeDataListIncluded = NULL;
	children.push_back(_completeDataListIncluded);

	// lsaOnlyAccessIndicator
	_lsaOnlyAccessIndicator = NULL;
	children.push_back(_lsaOnlyAccessIndicator);

	// lsaDataList
	_lsaDataList = NULL;
	children.push_back(_lsaDataList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::LSAInformation::~LSAInformation(){

}

asn1::ASN1Node* asn1::LSAInformation::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_completeDataListIncluded = new Null();
			children[0] = _completeDataListIncluded;
			return _completeDataListIncluded;
		case 1:
			_lsaOnlyAccessIndicator = new LSAOnlyAccessIndicator();
			_lsaOnlyAccessIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_lsaOnlyAccessIndicator->tlv->tag_value = 1;
			children[1] = _lsaOnlyAccessIndicator;
			return _lsaOnlyAccessIndicator;
		case 2:
			_lsaDataList = new LSADataList();
			_lsaDataList->tlv->tag_class = CONTEXT_SPECIFIC;
			_lsaDataList->tlv->tag_value = 2;
			children[2] = _lsaDataList;
			return _lsaDataList;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::LSAInformation::set_completeDataListIncluded(){
	if(_completeDataListIncluded == NULL) _completeDataListIncluded = (Null*)create_node(0);
}

void asn1::LSAInformation::set_lsaOnlyAccessIndicator(){
	if(_lsaOnlyAccessIndicator == NULL) _lsaOnlyAccessIndicator = (LSAOnlyAccessIndicator*)create_node(1);
}

void asn1::LSAInformation::set_lsaDataList(){
	if(_lsaDataList == NULL) _lsaDataList = (LSADataList*)create_node(2);
}

void asn1::LSAInformation::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//LSAIdentity
asn1::LSAIdentity::LSAIdentity(){
	strcpy(node_type_name, "LSAIdentity");

}
asn1::LSAIdentity::~LSAIdentity(){

}

//LSAAttributes
asn1::LSAAttributes::LSAAttributes(){
	strcpy(node_type_name, "LSAAttributes");

}
asn1::LSAAttributes::~LSAAttributes(){

}

//SubscriberData
asn1::SubscriberData::SubscriberData(){
	strcpy(node_type_name, "SubscriberData");
	// msisdn
	_msisdn = NULL;
	children.push_back(_msisdn);

	// category
	_category = NULL;
	children.push_back(_category);

	// subscriberStatus
	_subscriberStatus = NULL;
	children.push_back(_subscriberStatus);

	// bearerServiceList
	_bearerServiceList = NULL;
	children.push_back(_bearerServiceList);

	// teleserviceList
	_teleserviceList = NULL;
	children.push_back(_teleserviceList);

	// provisionedSS
	_provisionedSS = NULL;
	children.push_back(_provisionedSS);

	// odb_Data
	_odb_Data = NULL;
	children.push_back(_odb_Data);

	// roamingRestrictionDueToUnsupportedFeature
	_roamingRestrictionDueToUnsupportedFeature = NULL;
	children.push_back(_roamingRestrictionDueToUnsupportedFeature);

	// regionalSubscriptionData
	_regionalSubscriptionData = NULL;
	children.push_back(_regionalSubscriptionData);

	// vbsSubscriptionData
	_vbsSubscriptionData = NULL;
	children.push_back(_vbsSubscriptionData);

	// vgcsSubscriptionData
	_vgcsSubscriptionData = NULL;
	children.push_back(_vgcsSubscriptionData);

	// vlrCamelSubscriptionInfo
	_vlrCamelSubscriptionInfo = NULL;
	children.push_back(_vlrCamelSubscriptionInfo);


}
asn1::SubscriberData::~SubscriberData(){

}

asn1::ASN1Node* asn1::SubscriberData::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_msisdn = new ISDN_AddressString();
			_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn->tlv->tag_value = 1;
			children[0] = _msisdn;
			return _msisdn;
		case 1:
			_category = new Category();
			_category->tlv->tag_class = CONTEXT_SPECIFIC;
			_category->tlv->tag_value = 2;
			children[1] = _category;
			return _category;
		case 2:
			_subscriberStatus = new SubscriberStatus();
			_subscriberStatus->tlv->tag_class = CONTEXT_SPECIFIC;
			_subscriberStatus->tlv->tag_value = 3;
			children[2] = _subscriberStatus;
			return _subscriberStatus;
		case 3:
			_bearerServiceList = new BearerServiceList();
			_bearerServiceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_bearerServiceList->tlv->tag_value = 4;
			children[3] = _bearerServiceList;
			return _bearerServiceList;
		case 4:
			_teleserviceList = new TeleserviceList();
			_teleserviceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_teleserviceList->tlv->tag_value = 6;
			children[4] = _teleserviceList;
			return _teleserviceList;
		case 5:
			_provisionedSS = new Ext_SS_InfoList();
			_provisionedSS->tlv->tag_class = CONTEXT_SPECIFIC;
			_provisionedSS->tlv->tag_value = 7;
			children[5] = _provisionedSS;
			return _provisionedSS;
		case 6:
			_odb_Data = new ODB_Data();
			_odb_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_odb_Data->tlv->tag_value = 8;
			children[6] = _odb_Data;
			return _odb_Data;
		case 7:
			_roamingRestrictionDueToUnsupportedFeature = new Null();
			_roamingRestrictionDueToUnsupportedFeature->tlv->tag_class = CONTEXT_SPECIFIC;
			_roamingRestrictionDueToUnsupportedFeature->tlv->tag_value = 9;
			children[7] = _roamingRestrictionDueToUnsupportedFeature;
			return _roamingRestrictionDueToUnsupportedFeature;
		case 8:
			_regionalSubscriptionData = new ZoneCodeList();
			_regionalSubscriptionData->tlv->tag_class = CONTEXT_SPECIFIC;
			_regionalSubscriptionData->tlv->tag_value = 10;
			children[8] = _regionalSubscriptionData;
			return _regionalSubscriptionData;
		case 9:
			_vbsSubscriptionData = new VBSDataList();
			_vbsSubscriptionData->tlv->tag_class = CONTEXT_SPECIFIC;
			_vbsSubscriptionData->tlv->tag_value = 11;
			children[9] = _vbsSubscriptionData;
			return _vbsSubscriptionData;
		case 10:
			_vgcsSubscriptionData = new VGCSDataList();
			_vgcsSubscriptionData->tlv->tag_class = CONTEXT_SPECIFIC;
			_vgcsSubscriptionData->tlv->tag_value = 12;
			children[10] = _vgcsSubscriptionData;
			return _vgcsSubscriptionData;
		case 11:
			_vlrCamelSubscriptionInfo = new VlrCamelSubscriptionInfo();
			_vlrCamelSubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_vlrCamelSubscriptionInfo->tlv->tag_value = 13;
			children[11] = _vlrCamelSubscriptionInfo;
			return _vlrCamelSubscriptionInfo;

		default: return NULL;
	}
}

void asn1::SubscriberData::set_msisdn(){
	if(_msisdn == NULL) _msisdn = (ISDN_AddressString*)create_node(0);
}

void asn1::SubscriberData::set_category(){
	if(_category == NULL) _category = (Category*)create_node(1);
}

void asn1::SubscriberData::set_subscriberStatus(){
	if(_subscriberStatus == NULL) _subscriberStatus = (SubscriberStatus*)create_node(2);
}

void asn1::SubscriberData::set_bearerServiceList(){
	if(_bearerServiceList == NULL) _bearerServiceList = (BearerServiceList*)create_node(3);
}

void asn1::SubscriberData::set_teleserviceList(){
	if(_teleserviceList == NULL) _teleserviceList = (TeleserviceList*)create_node(4);
}

void asn1::SubscriberData::set_provisionedSS(){
	if(_provisionedSS == NULL) _provisionedSS = (Ext_SS_InfoList*)create_node(5);
}

void asn1::SubscriberData::set_odb_Data(){
	if(_odb_Data == NULL) _odb_Data = (ODB_Data*)create_node(6);
}

void asn1::SubscriberData::set_roamingRestrictionDueToUnsupportedFeature(){
	if(_roamingRestrictionDueToUnsupportedFeature == NULL) _roamingRestrictionDueToUnsupportedFeature = (Null*)create_node(7);
}

void asn1::SubscriberData::set_regionalSubscriptionData(){
	if(_regionalSubscriptionData == NULL) _regionalSubscriptionData = (ZoneCodeList*)create_node(8);
}

void asn1::SubscriberData::set_vbsSubscriptionData(){
	if(_vbsSubscriptionData == NULL) _vbsSubscriptionData = (VBSDataList*)create_node(9);
}

void asn1::SubscriberData::set_vgcsSubscriptionData(){
	if(_vgcsSubscriptionData == NULL) _vgcsSubscriptionData = (VGCSDataList*)create_node(10);
}

void asn1::SubscriberData::set_vlrCamelSubscriptionInfo(){
	if(_vlrCamelSubscriptionInfo == NULL) _vlrCamelSubscriptionInfo = (VlrCamelSubscriptionInfo*)create_node(11);
}

//Category
asn1::Category::Category(){
	strcpy(node_type_name, "Category");

}
asn1::Category::~Category(){

}

//SubscriberStatus
asn1::SubscriberStatus::SubscriberStatus(){
	strcpy(node_type_name, "SubscriberStatus");

}
asn1::SubscriberStatus::~SubscriberStatus(){

}

//BearerServiceList
asn1::BearerServiceList::BearerServiceList(){
	strcpy(node_type_name, "BearerServiceList");

}
asn1::BearerServiceList::~BearerServiceList(){
}

asn1::Ext_BearerServiceCode* asn1::BearerServiceList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Ext_BearerServiceCode*)children[child_index]; else return NULL;
}

void asn1::BearerServiceList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::BearerServiceList::create_node(unsigned int _index){
	children[_index] = new Ext_BearerServiceCode();
	return children[_index];
}

asn1::ASN1Node* asn1::BearerServiceList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Ext_BearerServiceCode());
		return children[children.size() - 1];
	}

}

//maxNumOfBearerServices
asn1::maxNumOfBearerServices::maxNumOfBearerServices(){
	strcpy(node_type_name, "maxNumOfBearerServices");

}
asn1::maxNumOfBearerServices::~maxNumOfBearerServices(){

}

//TeleserviceList
asn1::TeleserviceList::TeleserviceList(){
	strcpy(node_type_name, "TeleserviceList");

}
asn1::TeleserviceList::~TeleserviceList(){
}

asn1::Ext_TeleserviceCode* asn1::TeleserviceList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Ext_TeleserviceCode*)children[child_index]; else return NULL;
}

void asn1::TeleserviceList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::TeleserviceList::create_node(unsigned int _index){
	children[_index] = new Ext_TeleserviceCode();
	return children[_index];
}

asn1::ASN1Node* asn1::TeleserviceList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Ext_TeleserviceCode());
		return children[children.size() - 1];
	}

}

//maxNumOfTeleservices
asn1::maxNumOfTeleservices::maxNumOfTeleservices(){
	strcpy(node_type_name, "maxNumOfTeleservices");

}
asn1::maxNumOfTeleservices::~maxNumOfTeleservices(){

}

//ODB_Data
asn1::ODB_Data::ODB_Data(){
	strcpy(node_type_name, "ODB_Data");
	// odb_GeneralData
	_odb_GeneralData = NULL;
	_odb_GeneralData = new ODB_GeneralData();
	children.push_back(_odb_GeneralData);

	// odb_HPLMN_Data
	_odb_HPLMN_Data = NULL;
	children.push_back(_odb_HPLMN_Data);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ODB_Data::~ODB_Data(){

}

asn1::ASN1Node* asn1::ODB_Data::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_odb_HPLMN_Data = new ODB_HPLMN_Data();
			children[1] = _odb_HPLMN_Data;
			return _odb_HPLMN_Data;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ODB_Data::set_odb_HPLMN_Data(){
	if(_odb_HPLMN_Data == NULL) _odb_HPLMN_Data = (ODB_HPLMN_Data*)create_node(1);
}

void asn1::ODB_Data::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//ODB_GeneralData
asn1::ODB_GeneralData::ODB_GeneralData(){
	strcpy(node_type_name, "ODB_GeneralData");

}
asn1::ODB_GeneralData::~ODB_GeneralData(){

}

//ODB_HPLMN_Data
asn1::ODB_HPLMN_Data::ODB_HPLMN_Data(){
	strcpy(node_type_name, "ODB_HPLMN_Data");

}
asn1::ODB_HPLMN_Data::~ODB_HPLMN_Data(){

}

//Ext_SS_InfoList
asn1::Ext_SS_InfoList::Ext_SS_InfoList(){
	strcpy(node_type_name, "Ext_SS_InfoList");

}
asn1::Ext_SS_InfoList::~Ext_SS_InfoList(){
}

asn1::Ext_SS_Info* asn1::Ext_SS_InfoList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Ext_SS_Info*)children[child_index]; else return NULL;
}

void asn1::Ext_SS_InfoList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::Ext_SS_InfoList::create_node(unsigned int _index){
	children[_index] = new Ext_SS_Info();
	return children[_index];
}

asn1::ASN1Node* asn1::Ext_SS_InfoList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Ext_SS_Info());
		return children[children.size() - 1];
	}

}

//Ext_SS_Info
asn1::Ext_SS_Info::Ext_SS_Info(){
	strcpy(node_type_name, "Ext_SS_Info");
	// forwardingInfo
	_forwardingInfo = NULL;
	_forwardingInfo = new Ext_ForwInfo();
	_forwardingInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_forwardingInfo->tlv->tag_value = 0;
	children.push_back(_forwardingInfo);

	// callBarringInfo
	_callBarringInfo = NULL;
	_callBarringInfo = new Ext_CallBarInfo();
	_callBarringInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_callBarringInfo->tlv->tag_value = 1;
	children.push_back(_callBarringInfo);

	// cug_Info
	_cug_Info = NULL;
	_cug_Info = new CUG_Info();
	_cug_Info->tlv->tag_class = CONTEXT_SPECIFIC;
	_cug_Info->tlv->tag_value = 2;
	children.push_back(_cug_Info);

	// ss_Data
	_ss_Data = NULL;
	_ss_Data = new Ext_SS_Data();
	_ss_Data->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Data->tlv->tag_value = 3;
	children.push_back(_ss_Data);

	// emlpp_Info
	_emlpp_Info = NULL;
	_emlpp_Info = new EMLPP_Info();
	_emlpp_Info->tlv->tag_class = CONTEXT_SPECIFIC;
	_emlpp_Info->tlv->tag_value = 4;
	children.push_back(_emlpp_Info);


}
asn1::Ext_SS_Info::~Ext_SS_Info(){

}

//Ext_ForwInfo
asn1::Ext_ForwInfo::Ext_ForwInfo(){
	strcpy(node_type_name, "Ext_ForwInfo");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	children.push_back(_ss_Code);

	// forwardingFeatureList
	_forwardingFeatureList = NULL;
	_forwardingFeatureList = new Ext_ForwFeatureList();
	children.push_back(_forwardingFeatureList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::Ext_ForwInfo::~Ext_ForwInfo(){

}

asn1::ASN1Node* asn1::Ext_ForwInfo::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::Ext_ForwInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//Ext_ForwFeatureList
asn1::Ext_ForwFeatureList::Ext_ForwFeatureList(){
	strcpy(node_type_name, "Ext_ForwFeatureList");

}
asn1::Ext_ForwFeatureList::~Ext_ForwFeatureList(){
}

asn1::Ext_ForwFeature* asn1::Ext_ForwFeatureList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Ext_ForwFeature*)children[child_index]; else return NULL;
}

void asn1::Ext_ForwFeatureList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::Ext_ForwFeatureList::create_node(unsigned int _index){
	children[_index] = new Ext_ForwFeature();
	return children[_index];
}

asn1::ASN1Node* asn1::Ext_ForwFeatureList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Ext_ForwFeature());
		return children[children.size() - 1];
	}

}

//Ext_ForwFeature
asn1::Ext_ForwFeature::Ext_ForwFeature(){
	strcpy(node_type_name, "Ext_ForwFeature");
	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 4;
	children.push_back(_ss_Status);

	// forwardedToNumber
	_forwardedToNumber = NULL;
	children.push_back(_forwardedToNumber);

	// forwardedToSubaddress
	_forwardedToSubaddress = NULL;
	children.push_back(_forwardedToSubaddress);

	// forwardingOptions
	_forwardingOptions = NULL;
	children.push_back(_forwardingOptions);

	// noReplyConditionTime
	_noReplyConditionTime = NULL;
	children.push_back(_noReplyConditionTime);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// longForwardedToNumber
	_longForwardedToNumber = NULL;
	children.push_back(_longForwardedToNumber);


}
asn1::Ext_ForwFeature::~Ext_ForwFeature(){

}

asn1::ASN1Node* asn1::Ext_ForwFeature::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_basicService = new Ext_BasicServiceCode();
			children[0] = _basicService;
			return _basicService;
		case 2:
			_forwardedToNumber = new ISDN_AddressString();
			_forwardedToNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardedToNumber->tlv->tag_value = 5;
			children[2] = _forwardedToNumber;
			return _forwardedToNumber;
		case 3:
			_forwardedToSubaddress = new ISDN_SubaddressString();
			_forwardedToSubaddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardedToSubaddress->tlv->tag_value = 8;
			children[3] = _forwardedToSubaddress;
			return _forwardedToSubaddress;
		case 4:
			_forwardingOptions = new Ext_ForwOptions();
			_forwardingOptions->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardingOptions->tlv->tag_value = 6;
			children[4] = _forwardingOptions;
			return _forwardingOptions;
		case 5:
			_noReplyConditionTime = new Ext_NoRepCondTime();
			_noReplyConditionTime->tlv->tag_class = CONTEXT_SPECIFIC;
			_noReplyConditionTime->tlv->tag_value = 7;
			children[5] = _noReplyConditionTime;
			return _noReplyConditionTime;
		case 6:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 9;
			children[6] = _extensionContainer;
			return _extensionContainer;
		case 7:
			_longForwardedToNumber = new FTN_AddressString();
			_longForwardedToNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_longForwardedToNumber->tlv->tag_value = 10;
			children[7] = _longForwardedToNumber;
			return _longForwardedToNumber;

		default: return NULL;
	}
}

void asn1::Ext_ForwFeature::set_basicService(){
	if(_basicService == NULL) _basicService = (Ext_BasicServiceCode*)create_node(0);
}

void asn1::Ext_ForwFeature::set_forwardedToNumber(){
	if(_forwardedToNumber == NULL) _forwardedToNumber = (ISDN_AddressString*)create_node(2);
}

void asn1::Ext_ForwFeature::set_forwardedToSubaddress(){
	if(_forwardedToSubaddress == NULL) _forwardedToSubaddress = (ISDN_SubaddressString*)create_node(3);
}

void asn1::Ext_ForwFeature::set_forwardingOptions(){
	if(_forwardingOptions == NULL) _forwardingOptions = (Ext_ForwOptions*)create_node(4);
}

void asn1::Ext_ForwFeature::set_noReplyConditionTime(){
	if(_noReplyConditionTime == NULL) _noReplyConditionTime = (Ext_NoRepCondTime*)create_node(5);
}

void asn1::Ext_ForwFeature::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(6);
}

void asn1::Ext_ForwFeature::set_longForwardedToNumber(){
	if(_longForwardedToNumber == NULL) _longForwardedToNumber = (FTN_AddressString*)create_node(7);
}

//Ext_ForwOptions
asn1::Ext_ForwOptions::Ext_ForwOptions(){
	strcpy(node_type_name, "Ext_ForwOptions");

}
asn1::Ext_ForwOptions::~Ext_ForwOptions(){

}

//Ext_NoRepCondTime
asn1::Ext_NoRepCondTime::Ext_NoRepCondTime(){
	strcpy(node_type_name, "Ext_NoRepCondTime");

}
asn1::Ext_NoRepCondTime::~Ext_NoRepCondTime(){

}

//Ext_CallBarInfo
asn1::Ext_CallBarInfo::Ext_CallBarInfo(){
	strcpy(node_type_name, "Ext_CallBarInfo");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	children.push_back(_ss_Code);

	// callBarringFeatureList
	_callBarringFeatureList = NULL;
	_callBarringFeatureList = new Ext_CallBarFeatureList();
	children.push_back(_callBarringFeatureList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::Ext_CallBarInfo::~Ext_CallBarInfo(){

}

asn1::ASN1Node* asn1::Ext_CallBarInfo::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::Ext_CallBarInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//Ext_CallBarFeatureList
asn1::Ext_CallBarFeatureList::Ext_CallBarFeatureList(){
	strcpy(node_type_name, "Ext_CallBarFeatureList");

}
asn1::Ext_CallBarFeatureList::~Ext_CallBarFeatureList(){
}

asn1::Ext_CallBarringFeature* asn1::Ext_CallBarFeatureList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Ext_CallBarringFeature*)children[child_index]; else return NULL;
}

void asn1::Ext_CallBarFeatureList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::Ext_CallBarFeatureList::create_node(unsigned int _index){
	children[_index] = new Ext_CallBarringFeature();
	return children[_index];
}

asn1::ASN1Node* asn1::Ext_CallBarFeatureList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Ext_CallBarringFeature());
		return children[children.size() - 1];
	}

}

//Ext_CallBarringFeature
asn1::Ext_CallBarringFeature::Ext_CallBarringFeature(){
	strcpy(node_type_name, "Ext_CallBarringFeature");
	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 4;
	children.push_back(_ss_Status);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::Ext_CallBarringFeature::~Ext_CallBarringFeature(){

}

asn1::ASN1Node* asn1::Ext_CallBarringFeature::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_basicService = new Ext_BasicServiceCode();
			children[0] = _basicService;
			return _basicService;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::Ext_CallBarringFeature::set_basicService(){
	if(_basicService == NULL) _basicService = (Ext_BasicServiceCode*)create_node(0);
}

void asn1::Ext_CallBarringFeature::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//CUG_Info
asn1::CUG_Info::CUG_Info(){
	strcpy(node_type_name, "CUG_Info");
	// cug_SubscriptionList
	_cug_SubscriptionList = NULL;
	_cug_SubscriptionList = new CUG_SubscriptionList();
	children.push_back(_cug_SubscriptionList);

	// cug_FeatureList
	_cug_FeatureList = NULL;
	children.push_back(_cug_FeatureList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CUG_Info::~CUG_Info(){

}

asn1::ASN1Node* asn1::CUG_Info::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_cug_FeatureList = new CUG_FeatureList();
			children[1] = _cug_FeatureList;
			return _cug_FeatureList;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CUG_Info::set_cug_FeatureList(){
	if(_cug_FeatureList == NULL) _cug_FeatureList = (CUG_FeatureList*)create_node(1);
}

void asn1::CUG_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//CUG_SubscriptionList
asn1::CUG_SubscriptionList::CUG_SubscriptionList(){
	strcpy(node_type_name, "CUG_SubscriptionList");

}
asn1::CUG_SubscriptionList::~CUG_SubscriptionList(){
}

asn1::CUG_Subscription* asn1::CUG_SubscriptionList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (CUG_Subscription*)children[child_index]; else return NULL;
}

void asn1::CUG_SubscriptionList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::CUG_SubscriptionList::create_node(unsigned int _index){
	children[_index] = new CUG_Subscription();
	return children[_index];
}

asn1::ASN1Node* asn1::CUG_SubscriptionList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new CUG_Subscription());
		return children[children.size() - 1];
	}

}

//CUG_Subscription
asn1::CUG_Subscription::CUG_Subscription(){
	strcpy(node_type_name, "CUG_Subscription");
	// cug_Index
	_cug_Index = NULL;
	_cug_Index = new CUG_Index();
	children.push_back(_cug_Index);

	// cug_Interlock
	_cug_Interlock = NULL;
	_cug_Interlock = new CUG_Interlock();
	children.push_back(_cug_Interlock);

	// intraCUG_Options
	_intraCUG_Options = NULL;
	_intraCUG_Options = new IntraCUG_Options();
	children.push_back(_intraCUG_Options);

	// basicServiceGroupList
	_basicServiceGroupList = NULL;
	children.push_back(_basicServiceGroupList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CUG_Subscription::~CUG_Subscription(){

}

asn1::ASN1Node* asn1::CUG_Subscription::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_basicServiceGroupList = new Ext_BasicServiceGroupList();
			children[3] = _basicServiceGroupList;
			return _basicServiceGroupList;
		case 4:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CUG_Subscription::set_basicServiceGroupList(){
	if(_basicServiceGroupList == NULL) _basicServiceGroupList = (Ext_BasicServiceGroupList*)create_node(3);
}

void asn1::CUG_Subscription::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//CUG_Index
asn1::CUG_Index::CUG_Index(){
	strcpy(node_type_name, "CUG_Index");

}
asn1::CUG_Index::~CUG_Index(){

}

//CUG_Interlock
asn1::CUG_Interlock::CUG_Interlock(){
	strcpy(node_type_name, "CUG_Interlock");

}
asn1::CUG_Interlock::~CUG_Interlock(){

}

//IntraCUG_Options
asn1::IntraCUG_Options::IntraCUG_Options(){
	strcpy(node_type_name, "IntraCUG_Options");

}
asn1::IntraCUG_Options::~IntraCUG_Options(){

}

//maxNumOfCUG
asn1::maxNumOfCUG::maxNumOfCUG(){
	strcpy(node_type_name, "maxNumOfCUG");

}
asn1::maxNumOfCUG::~maxNumOfCUG(){

}

//CUG_FeatureList
asn1::CUG_FeatureList::CUG_FeatureList(){
	strcpy(node_type_name, "CUG_FeatureList");

}
asn1::CUG_FeatureList::~CUG_FeatureList(){
}

asn1::CUG_Feature* asn1::CUG_FeatureList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (CUG_Feature*)children[child_index]; else return NULL;
}

void asn1::CUG_FeatureList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::CUG_FeatureList::create_node(unsigned int _index){
	children[_index] = new CUG_Feature();
	return children[_index];
}

asn1::ASN1Node* asn1::CUG_FeatureList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new CUG_Feature());
		return children[children.size() - 1];
	}

}

//Ext_BasicServiceGroupList
asn1::Ext_BasicServiceGroupList::Ext_BasicServiceGroupList(){
	strcpy(node_type_name, "Ext_BasicServiceGroupList");

}
asn1::Ext_BasicServiceGroupList::~Ext_BasicServiceGroupList(){
}

asn1::Ext_BasicServiceCode* asn1::Ext_BasicServiceGroupList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Ext_BasicServiceCode*)children[child_index]; else return NULL;
}

void asn1::Ext_BasicServiceGroupList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::Ext_BasicServiceGroupList::create_node(unsigned int _index){
	children[_index] = new Ext_BasicServiceCode();
	return children[_index];
}

asn1::ASN1Node* asn1::Ext_BasicServiceGroupList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Ext_BasicServiceCode());
		return children[children.size() - 1];
	}

}

//maxNumOfExt_BasicServiceGroups
asn1::maxNumOfExt_BasicServiceGroups::maxNumOfExt_BasicServiceGroups(){
	strcpy(node_type_name, "maxNumOfExt_BasicServiceGroups");

}
asn1::maxNumOfExt_BasicServiceGroups::~maxNumOfExt_BasicServiceGroups(){

}

//CUG_Feature
asn1::CUG_Feature::CUG_Feature(){
	strcpy(node_type_name, "CUG_Feature");
	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// preferentialCUG_Indicator
	_preferentialCUG_Indicator = NULL;
	children.push_back(_preferentialCUG_Indicator);

	// interCUG_Restrictions
	_interCUG_Restrictions = NULL;
	_interCUG_Restrictions = new InterCUG_Restrictions();
	children.push_back(_interCUG_Restrictions);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CUG_Feature::~CUG_Feature(){

}

asn1::ASN1Node* asn1::CUG_Feature::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_basicService = new Ext_BasicServiceCode();
			children[0] = _basicService;
			return _basicService;
		case 1:
			_preferentialCUG_Indicator = new CUG_Index();
			children[1] = _preferentialCUG_Indicator;
			return _preferentialCUG_Indicator;
		case 3:
			_extensionContainer = new ExtensionContainer();
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CUG_Feature::set_basicService(){
	if(_basicService == NULL) _basicService = (Ext_BasicServiceCode*)create_node(0);
}

void asn1::CUG_Feature::set_preferentialCUG_Indicator(){
	if(_preferentialCUG_Indicator == NULL) _preferentialCUG_Indicator = (CUG_Index*)create_node(1);
}

void asn1::CUG_Feature::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//InterCUG_Restrictions
asn1::InterCUG_Restrictions::InterCUG_Restrictions(){
	strcpy(node_type_name, "InterCUG_Restrictions");

}
asn1::InterCUG_Restrictions::~InterCUG_Restrictions(){

}

//Ext_SS_Data
asn1::Ext_SS_Data::Ext_SS_Data(){
	strcpy(node_type_name, "Ext_SS_Data");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	children.push_back(_ss_Code);

	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 4;
	children.push_back(_ss_Status);

	// ss_SubscriptionOption
	_ss_SubscriptionOption = NULL;
	children.push_back(_ss_SubscriptionOption);

	// basicServiceGroupList
	_basicServiceGroupList = NULL;
	children.push_back(_basicServiceGroupList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::Ext_SS_Data::~Ext_SS_Data(){

}

asn1::ASN1Node* asn1::Ext_SS_Data::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_ss_SubscriptionOption = new SS_SubscriptionOption();
			children[2] = _ss_SubscriptionOption;
			return _ss_SubscriptionOption;
		case 3:
			_basicServiceGroupList = new Ext_BasicServiceGroupList();
			children[3] = _basicServiceGroupList;
			return _basicServiceGroupList;
		case 4:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 5;
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::Ext_SS_Data::set_ss_SubscriptionOption(){
	if(_ss_SubscriptionOption == NULL) _ss_SubscriptionOption = (SS_SubscriptionOption*)create_node(2);
}

void asn1::Ext_SS_Data::set_basicServiceGroupList(){
	if(_basicServiceGroupList == NULL) _basicServiceGroupList = (Ext_BasicServiceGroupList*)create_node(3);
}

void asn1::Ext_SS_Data::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//LCS_PrivacyExceptionList
asn1::LCS_PrivacyExceptionList::LCS_PrivacyExceptionList(){
	strcpy(node_type_name, "LCS_PrivacyExceptionList");

}
asn1::LCS_PrivacyExceptionList::~LCS_PrivacyExceptionList(){
}

asn1::LCS_PrivacyClass* asn1::LCS_PrivacyExceptionList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (LCS_PrivacyClass*)children[child_index]; else return NULL;
}

void asn1::LCS_PrivacyExceptionList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::LCS_PrivacyExceptionList::create_node(unsigned int _index){
	children[_index] = new LCS_PrivacyClass();
	return children[_index];
}

asn1::ASN1Node* asn1::LCS_PrivacyExceptionList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new LCS_PrivacyClass());
		return children[children.size() - 1];
	}

}

//maxNumOfPrivacyClass
asn1::maxNumOfPrivacyClass::maxNumOfPrivacyClass(){
	strcpy(node_type_name, "maxNumOfPrivacyClass");

}
asn1::maxNumOfPrivacyClass::~maxNumOfPrivacyClass(){

}

//LCS_PrivacyClass
asn1::LCS_PrivacyClass::LCS_PrivacyClass(){
	strcpy(node_type_name, "LCS_PrivacyClass");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	children.push_back(_ss_Code);

	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	children.push_back(_ss_Status);

	// notificationToMSUser
	_notificationToMSUser = NULL;
	children.push_back(_notificationToMSUser);

	// externalClientList
	_externalClientList = NULL;
	children.push_back(_externalClientList);

	// plmnClientList
	_plmnClientList = NULL;
	children.push_back(_plmnClientList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// ext_externalClientList
	_ext_externalClientList = NULL;
	children.push_back(_ext_externalClientList);

	// serviceTypeList
	_serviceTypeList = NULL;
	children.push_back(_serviceTypeList);


}
asn1::LCS_PrivacyClass::~LCS_PrivacyClass(){

}

asn1::ASN1Node* asn1::LCS_PrivacyClass::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_notificationToMSUser = new NotificationToMSUser();
			_notificationToMSUser->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToMSUser->tlv->tag_value = 0;
			children[2] = _notificationToMSUser;
			return _notificationToMSUser;
		case 3:
			_externalClientList = new ExternalClientList();
			_externalClientList->tlv->tag_class = CONTEXT_SPECIFIC;
			_externalClientList->tlv->tag_value = 1;
			children[3] = _externalClientList;
			return _externalClientList;
		case 4:
			_plmnClientList = new PLMNClientList();
			_plmnClientList->tlv->tag_class = CONTEXT_SPECIFIC;
			_plmnClientList->tlv->tag_value = 2;
			children[4] = _plmnClientList;
			return _plmnClientList;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[5] = _extensionContainer;
			return _extensionContainer;
		case 6:
			_ext_externalClientList = new Ext_ExternalClientList();
			_ext_externalClientList->tlv->tag_class = CONTEXT_SPECIFIC;
			_ext_externalClientList->tlv->tag_value = 4;
			children[6] = _ext_externalClientList;
			return _ext_externalClientList;
		case 7:
			_serviceTypeList = new ServiceTypeList();
			_serviceTypeList->tlv->tag_class = CONTEXT_SPECIFIC;
			_serviceTypeList->tlv->tag_value = 5;
			children[7] = _serviceTypeList;
			return _serviceTypeList;

		default: return NULL;
	}
}

void asn1::LCS_PrivacyClass::set_notificationToMSUser(){
	if(_notificationToMSUser == NULL) _notificationToMSUser = (NotificationToMSUser*)create_node(2);
}

void asn1::LCS_PrivacyClass::set_externalClientList(){
	if(_externalClientList == NULL) _externalClientList = (ExternalClientList*)create_node(3);
}

void asn1::LCS_PrivacyClass::set_plmnClientList(){
	if(_plmnClientList == NULL) _plmnClientList = (PLMNClientList*)create_node(4);
}

void asn1::LCS_PrivacyClass::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

void asn1::LCS_PrivacyClass::set_ext_externalClientList(){
	if(_ext_externalClientList == NULL) _ext_externalClientList = (Ext_ExternalClientList*)create_node(6);
}

void asn1::LCS_PrivacyClass::set_serviceTypeList(){
	if(_serviceTypeList == NULL) _serviceTypeList = (ServiceTypeList*)create_node(7);
}

//ExternalClientList
asn1::ExternalClientList::ExternalClientList(){
	strcpy(node_type_name, "ExternalClientList");

}
asn1::ExternalClientList::~ExternalClientList(){
}

asn1::ExternalClient* asn1::ExternalClientList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ExternalClient*)children[child_index]; else return NULL;
}

void asn1::ExternalClientList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::ExternalClientList::create_node(unsigned int _index){
	children[_index] = new ExternalClient();
	return children[_index];
}

asn1::ASN1Node* asn1::ExternalClientList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ExternalClient());
		return children[children.size() - 1];
	}

}

//maxNumOfExternalClient
asn1::maxNumOfExternalClient::maxNumOfExternalClient(){
	strcpy(node_type_name, "maxNumOfExternalClient");

}
asn1::maxNumOfExternalClient::~maxNumOfExternalClient(){

}

//PLMNClientList
asn1::PLMNClientList::PLMNClientList(){
	strcpy(node_type_name, "PLMNClientList");

}
asn1::PLMNClientList::~PLMNClientList(){
}

asn1::LCSClientInternalID* asn1::PLMNClientList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (LCSClientInternalID*)children[child_index]; else return NULL;
}

void asn1::PLMNClientList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::PLMNClientList::create_node(unsigned int _index){
	children[_index] = new LCSClientInternalID();
	return children[_index];
}

asn1::ASN1Node* asn1::PLMNClientList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new LCSClientInternalID());
		return children[children.size() - 1];
	}

}

//maxNumOfPLMNClient
asn1::maxNumOfPLMNClient::maxNumOfPLMNClient(){
	strcpy(node_type_name, "maxNumOfPLMNClient");

}
asn1::maxNumOfPLMNClient::~maxNumOfPLMNClient(){

}

//Ext_ExternalClientList
asn1::Ext_ExternalClientList::Ext_ExternalClientList(){
	strcpy(node_type_name, "Ext_ExternalClientList");

}
asn1::Ext_ExternalClientList::~Ext_ExternalClientList(){
}

asn1::ExternalClient* asn1::Ext_ExternalClientList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ExternalClient*)children[child_index]; else return NULL;
}

void asn1::Ext_ExternalClientList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::Ext_ExternalClientList::create_node(unsigned int _index){
	children[_index] = new ExternalClient();
	return children[_index];
}

asn1::ASN1Node* asn1::Ext_ExternalClientList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ExternalClient());
		return children[children.size() - 1];
	}

}

//maxNumOfExt_ExternalClient
asn1::maxNumOfExt_ExternalClient::maxNumOfExt_ExternalClient(){
	strcpy(node_type_name, "maxNumOfExt_ExternalClient");

}
asn1::maxNumOfExt_ExternalClient::~maxNumOfExt_ExternalClient(){

}

//ExternalClient
asn1::ExternalClient::ExternalClient(){
	strcpy(node_type_name, "ExternalClient");
	// clientIdentity
	_clientIdentity = NULL;
	_clientIdentity = new LCSClientExternalID();
	children.push_back(_clientIdentity);

	// gmlc_Restriction
	_gmlc_Restriction = NULL;
	children.push_back(_gmlc_Restriction);

	// notificationToMSUser
	_notificationToMSUser = NULL;
	children.push_back(_notificationToMSUser);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ExternalClient::~ExternalClient(){

}

asn1::ASN1Node* asn1::ExternalClient::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_gmlc_Restriction = new GMLC_Restriction();
			_gmlc_Restriction->tlv->tag_class = CONTEXT_SPECIFIC;
			_gmlc_Restriction->tlv->tag_value = 0;
			children[1] = _gmlc_Restriction;
			return _gmlc_Restriction;
		case 2:
			_notificationToMSUser = new NotificationToMSUser();
			_notificationToMSUser->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToMSUser->tlv->tag_value = 1;
			children[2] = _notificationToMSUser;
			return _notificationToMSUser;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ExternalClient::set_gmlc_Restriction(){
	if(_gmlc_Restriction == NULL) _gmlc_Restriction = (GMLC_Restriction*)create_node(1);
}

void asn1::ExternalClient::set_notificationToMSUser(){
	if(_notificationToMSUser == NULL) _notificationToMSUser = (NotificationToMSUser*)create_node(2);
}

void asn1::ExternalClient::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//GMLC_Restriction
asn1::GMLC_Restriction::GMLC_Restriction(){
	strcpy(node_type_name, "GMLC_Restriction");

}
asn1::GMLC_Restriction::~GMLC_Restriction(){

}

//NotificationToMSUser
asn1::NotificationToMSUser::NotificationToMSUser(){
	strcpy(node_type_name, "NotificationToMSUser");

}
asn1::NotificationToMSUser::~NotificationToMSUser(){

}

//ServiceTypeList
asn1::ServiceTypeList::ServiceTypeList(){
	strcpy(node_type_name, "ServiceTypeList");

}
asn1::ServiceTypeList::~ServiceTypeList(){
}

asn1::ServiceType* asn1::ServiceTypeList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ServiceType*)children[child_index]; else return NULL;
}

void asn1::ServiceTypeList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::ServiceTypeList::create_node(unsigned int _index){
	children[_index] = new ServiceType();
	return children[_index];
}

asn1::ASN1Node* asn1::ServiceTypeList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ServiceType());
		return children[children.size() - 1];
	}

}

//maxNumOfServiceType
asn1::maxNumOfServiceType::maxNumOfServiceType(){
	strcpy(node_type_name, "maxNumOfServiceType");

}
asn1::maxNumOfServiceType::~maxNumOfServiceType(){

}

//ServiceType
asn1::ServiceType::ServiceType(){
	strcpy(node_type_name, "ServiceType");
	// serviceTypeIdentity
	_serviceTypeIdentity = NULL;
	_serviceTypeIdentity = new LCSServiceTypeID();
	children.push_back(_serviceTypeIdentity);

	// gmlc_Restriction
	_gmlc_Restriction = NULL;
	children.push_back(_gmlc_Restriction);

	// notificationToMSUser
	_notificationToMSUser = NULL;
	children.push_back(_notificationToMSUser);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ServiceType::~ServiceType(){

}

asn1::ASN1Node* asn1::ServiceType::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_gmlc_Restriction = new GMLC_Restriction();
			_gmlc_Restriction->tlv->tag_class = CONTEXT_SPECIFIC;
			_gmlc_Restriction->tlv->tag_value = 0;
			children[1] = _gmlc_Restriction;
			return _gmlc_Restriction;
		case 2:
			_notificationToMSUser = new NotificationToMSUser();
			_notificationToMSUser->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToMSUser->tlv->tag_value = 1;
			children[2] = _notificationToMSUser;
			return _notificationToMSUser;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ServiceType::set_gmlc_Restriction(){
	if(_gmlc_Restriction == NULL) _gmlc_Restriction = (GMLC_Restriction*)create_node(1);
}

void asn1::ServiceType::set_notificationToMSUser(){
	if(_notificationToMSUser == NULL) _notificationToMSUser = (NotificationToMSUser*)create_node(2);
}

void asn1::ServiceType::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//MOLR_List
asn1::MOLR_List::MOLR_List(){
	strcpy(node_type_name, "MOLR_List");

}
asn1::MOLR_List::~MOLR_List(){
}

asn1::MOLR_Class* asn1::MOLR_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (MOLR_Class*)children[child_index]; else return NULL;
}

void asn1::MOLR_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::MOLR_List::create_node(unsigned int _index){
	children[_index] = new MOLR_Class();
	return children[_index];
}

asn1::ASN1Node* asn1::MOLR_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new MOLR_Class());
		return children[children.size() - 1];
	}

}

//maxNumOfMOLR_Class
asn1::maxNumOfMOLR_Class::maxNumOfMOLR_Class(){
	strcpy(node_type_name, "maxNumOfMOLR_Class");

}
asn1::maxNumOfMOLR_Class::~maxNumOfMOLR_Class(){

}

//MOLR_Class
asn1::MOLR_Class::MOLR_Class(){
	strcpy(node_type_name, "MOLR_Class");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	children.push_back(_ss_Code);

	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	children.push_back(_ss_Status);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MOLR_Class::~MOLR_Class(){

}

asn1::ASN1Node* asn1::MOLR_Class::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::MOLR_Class::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//ZoneCodeList
asn1::ZoneCodeList::ZoneCodeList(){
	strcpy(node_type_name, "ZoneCodeList");

}
asn1::ZoneCodeList::~ZoneCodeList(){
}

asn1::ZoneCode* asn1::ZoneCodeList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ZoneCode*)children[child_index]; else return NULL;
}

void asn1::ZoneCodeList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::ZoneCodeList::create_node(unsigned int _index){
	children[_index] = new ZoneCode();
	return children[_index];
}

asn1::ASN1Node* asn1::ZoneCodeList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ZoneCode());
		return children[children.size() - 1];
	}

}

//ZoneCode
asn1::ZoneCode::ZoneCode(){
	strcpy(node_type_name, "ZoneCode");

}
asn1::ZoneCode::~ZoneCode(){

}

//maxNumOfZoneCodes
asn1::maxNumOfZoneCodes::maxNumOfZoneCodes(){
	strcpy(node_type_name, "maxNumOfZoneCodes");

}
asn1::maxNumOfZoneCodes::~maxNumOfZoneCodes(){

}

//InsertSubscriberDataRes
asn1::InsertSubscriberDataRes::InsertSubscriberDataRes(){
	strcpy(node_type_name, "InsertSubscriberDataRes");
	// teleserviceList
	_teleserviceList = NULL;
	children.push_back(_teleserviceList);

	// bearerServiceList
	_bearerServiceList = NULL;
	children.push_back(_bearerServiceList);

	// ss_List
	_ss_List = NULL;
	children.push_back(_ss_List);

	// odb_GeneralData
	_odb_GeneralData = NULL;
	children.push_back(_odb_GeneralData);

	// regionalSubscriptionResponse
	_regionalSubscriptionResponse = NULL;
	children.push_back(_regionalSubscriptionResponse);

	// supportedCamelPhases
	_supportedCamelPhases = NULL;
	children.push_back(_supportedCamelPhases);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// offeredCamel4CSIs
	_offeredCamel4CSIs = NULL;
	children.push_back(_offeredCamel4CSIs);

	// supportedFeatures
	_supportedFeatures = NULL;
	children.push_back(_supportedFeatures);


}
asn1::InsertSubscriberDataRes::~InsertSubscriberDataRes(){

}

asn1::ASN1Node* asn1::InsertSubscriberDataRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_teleserviceList = new TeleserviceList();
			_teleserviceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_teleserviceList->tlv->tag_value = 1;
			children[0] = _teleserviceList;
			return _teleserviceList;
		case 1:
			_bearerServiceList = new BearerServiceList();
			_bearerServiceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_bearerServiceList->tlv->tag_value = 2;
			children[1] = _bearerServiceList;
			return _bearerServiceList;
		case 2:
			_ss_List = new SS_List();
			_ss_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_List->tlv->tag_value = 3;
			children[2] = _ss_List;
			return _ss_List;
		case 3:
			_odb_GeneralData = new ODB_GeneralData();
			_odb_GeneralData->tlv->tag_class = CONTEXT_SPECIFIC;
			_odb_GeneralData->tlv->tag_value = 4;
			children[3] = _odb_GeneralData;
			return _odb_GeneralData;
		case 4:
			_regionalSubscriptionResponse = new RegionalSubscriptionResponse();
			_regionalSubscriptionResponse->tlv->tag_class = CONTEXT_SPECIFIC;
			_regionalSubscriptionResponse->tlv->tag_value = 5;
			children[4] = _regionalSubscriptionResponse;
			return _regionalSubscriptionResponse;
		case 5:
			_supportedCamelPhases = new SupportedCamelPhases();
			_supportedCamelPhases->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedCamelPhases->tlv->tag_value = 6;
			children[5] = _supportedCamelPhases;
			return _supportedCamelPhases;
		case 6:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 7;
			children[6] = _extensionContainer;
			return _extensionContainer;
		case 7:
			_offeredCamel4CSIs = new OfferedCamel4CSIs();
			_offeredCamel4CSIs->tlv->tag_class = CONTEXT_SPECIFIC;
			_offeredCamel4CSIs->tlv->tag_value = 8;
			children[7] = _offeredCamel4CSIs;
			return _offeredCamel4CSIs;
		case 8:
			_supportedFeatures = new SupportedFeatures();
			_supportedFeatures->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedFeatures->tlv->tag_value = 9;
			children[8] = _supportedFeatures;
			return _supportedFeatures;

		default: return NULL;
	}
}

void asn1::InsertSubscriberDataRes::set_teleserviceList(){
	if(_teleserviceList == NULL) _teleserviceList = (TeleserviceList*)create_node(0);
}

void asn1::InsertSubscriberDataRes::set_bearerServiceList(){
	if(_bearerServiceList == NULL) _bearerServiceList = (BearerServiceList*)create_node(1);
}

void asn1::InsertSubscriberDataRes::set_ss_List(){
	if(_ss_List == NULL) _ss_List = (SS_List*)create_node(2);
}

void asn1::InsertSubscriberDataRes::set_odb_GeneralData(){
	if(_odb_GeneralData == NULL) _odb_GeneralData = (ODB_GeneralData*)create_node(3);
}

void asn1::InsertSubscriberDataRes::set_regionalSubscriptionResponse(){
	if(_regionalSubscriptionResponse == NULL) _regionalSubscriptionResponse = (RegionalSubscriptionResponse*)create_node(4);
}

void asn1::InsertSubscriberDataRes::set_supportedCamelPhases(){
	if(_supportedCamelPhases == NULL) _supportedCamelPhases = (SupportedCamelPhases*)create_node(5);
}

void asn1::InsertSubscriberDataRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(6);
}

void asn1::InsertSubscriberDataRes::set_offeredCamel4CSIs(){
	if(_offeredCamel4CSIs == NULL) _offeredCamel4CSIs = (OfferedCamel4CSIs*)create_node(7);
}

void asn1::InsertSubscriberDataRes::set_supportedFeatures(){
	if(_supportedFeatures == NULL) _supportedFeatures = (SupportedFeatures*)create_node(8);
}

//RegionalSubscriptionResponse
asn1::RegionalSubscriptionResponse::RegionalSubscriptionResponse(){
	strcpy(node_type_name, "RegionalSubscriptionResponse");

}
asn1::RegionalSubscriptionResponse::~RegionalSubscriptionResponse(){

}

//DeleteSubscriberDataArg
asn1::DeleteSubscriberDataArg::DeleteSubscriberDataArg(){
	strcpy(node_type_name, "DeleteSubscriberDataArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// basicServiceList
	_basicServiceList = NULL;
	children.push_back(_basicServiceList);

	// ss_List
	_ss_List = NULL;
	children.push_back(_ss_List);

	// roamingRestrictionDueToUnsupportedFeature
	_roamingRestrictionDueToUnsupportedFeature = NULL;
	children.push_back(_roamingRestrictionDueToUnsupportedFeature);

	// regionalSubscriptionIdentifier
	_regionalSubscriptionIdentifier = NULL;
	children.push_back(_regionalSubscriptionIdentifier);

	// vbsGroupIndication
	_vbsGroupIndication = NULL;
	children.push_back(_vbsGroupIndication);

	// vgcsGroupIndication
	_vgcsGroupIndication = NULL;
	children.push_back(_vgcsGroupIndication);

	// camelSubscriptionInfoWithdraw
	_camelSubscriptionInfoWithdraw = NULL;
	children.push_back(_camelSubscriptionInfoWithdraw);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// gprsSubscriptionDataWithdraw
	_gprsSubscriptionDataWithdraw = NULL;
	children.push_back(_gprsSubscriptionDataWithdraw);

	// roamingRestrictedInSgsnDueToUnsuppportedFeature
	_roamingRestrictedInSgsnDueToUnsuppportedFeature = NULL;
	children.push_back(_roamingRestrictedInSgsnDueToUnsuppportedFeature);

	// lsaInformationWithdraw
	_lsaInformationWithdraw = NULL;
	children.push_back(_lsaInformationWithdraw);

	// gmlc_ListWithdraw
	_gmlc_ListWithdraw = NULL;
	children.push_back(_gmlc_ListWithdraw);

	// istInformationWithdraw
	_istInformationWithdraw = NULL;
	children.push_back(_istInformationWithdraw);

	// specificCSI_Withdraw
	_specificCSI_Withdraw = NULL;
	children.push_back(_specificCSI_Withdraw);

	// chargingCharacteristicsWithdraw
	_chargingCharacteristicsWithdraw = NULL;
	children.push_back(_chargingCharacteristicsWithdraw);

	// stn_srWithdraw
	_stn_srWithdraw = NULL;
	children.push_back(_stn_srWithdraw);

	// epsSubscriptionDataWithdraw
	_epsSubscriptionDataWithdraw = NULL;
	children.push_back(_epsSubscriptionDataWithdraw);

	// apn_oi_replacementWithdraw
	_apn_oi_replacementWithdraw = NULL;
	children.push_back(_apn_oi_replacementWithdraw);

	// csg_SubscriptionDeleted
	_csg_SubscriptionDeleted = NULL;
	children.push_back(_csg_SubscriptionDeleted);

	// subscribedPeriodicTAU_RAU_TimerWithdraw
	_subscribedPeriodicTAU_RAU_TimerWithdraw = NULL;
	children.push_back(_subscribedPeriodicTAU_RAU_TimerWithdraw);

	// subscribedPeriodicLAU_TimerWithdraw
	_subscribedPeriodicLAU_TimerWithdraw = NULL;
	children.push_back(_subscribedPeriodicLAU_TimerWithdraw);

	// subscribed_vsrvccWithdraw
	_subscribed_vsrvccWithdraw = NULL;
	children.push_back(_subscribed_vsrvccWithdraw);

	// vplmn_Csg_SubscriptionDeleted
	_vplmn_Csg_SubscriptionDeleted = NULL;
	children.push_back(_vplmn_Csg_SubscriptionDeleted);

	// additionalMSISDN_Withdraw
	_additionalMSISDN_Withdraw = NULL;
	children.push_back(_additionalMSISDN_Withdraw);


}
asn1::DeleteSubscriberDataArg::~DeleteSubscriberDataArg(){

}

asn1::ASN1Node* asn1::DeleteSubscriberDataArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_basicServiceList = new BasicServiceList();
			_basicServiceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_basicServiceList->tlv->tag_value = 1;
			children[1] = _basicServiceList;
			return _basicServiceList;
		case 2:
			_ss_List = new SS_List();
			_ss_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_List->tlv->tag_value = 2;
			children[2] = _ss_List;
			return _ss_List;
		case 3:
			_roamingRestrictionDueToUnsupportedFeature = new Null();
			_roamingRestrictionDueToUnsupportedFeature->tlv->tag_class = CONTEXT_SPECIFIC;
			_roamingRestrictionDueToUnsupportedFeature->tlv->tag_value = 4;
			children[3] = _roamingRestrictionDueToUnsupportedFeature;
			return _roamingRestrictionDueToUnsupportedFeature;
		case 4:
			_regionalSubscriptionIdentifier = new ZoneCode();
			_regionalSubscriptionIdentifier->tlv->tag_class = CONTEXT_SPECIFIC;
			_regionalSubscriptionIdentifier->tlv->tag_value = 5;
			children[4] = _regionalSubscriptionIdentifier;
			return _regionalSubscriptionIdentifier;
		case 5:
			_vbsGroupIndication = new Null();
			_vbsGroupIndication->tlv->tag_class = CONTEXT_SPECIFIC;
			_vbsGroupIndication->tlv->tag_value = 7;
			children[5] = _vbsGroupIndication;
			return _vbsGroupIndication;
		case 6:
			_vgcsGroupIndication = new Null();
			_vgcsGroupIndication->tlv->tag_class = CONTEXT_SPECIFIC;
			_vgcsGroupIndication->tlv->tag_value = 8;
			children[6] = _vgcsGroupIndication;
			return _vgcsGroupIndication;
		case 7:
			_camelSubscriptionInfoWithdraw = new Null();
			_camelSubscriptionInfoWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_camelSubscriptionInfoWithdraw->tlv->tag_value = 9;
			children[7] = _camelSubscriptionInfoWithdraw;
			return _camelSubscriptionInfoWithdraw;
		case 8:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 6;
			children[8] = _extensionContainer;
			return _extensionContainer;
                case 9:
                {
                        _gprsSubscriptionDataWithdraw = new GPRSSubscriptionDataWithdraw();
                        ASN1Node* _gprsSubscriptionDataWithdraw_wrapper = new ASN1Node();
                        _gprsSubscriptionDataWithdraw_wrapper->tlv = new TLVNode();
                        _gprsSubscriptionDataWithdraw_wrapper->tlv->is_explicit = true;
                        _gprsSubscriptionDataWithdraw_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _gprsSubscriptionDataWithdraw_wrapper->tlv->tag_value = 10;
                        _gprsSubscriptionDataWithdraw_wrapper->children.push_back(_gprsSubscriptionDataWithdraw);
                        children[9] = _gprsSubscriptionDataWithdraw_wrapper;
                        return _gprsSubscriptionDataWithdraw;
                }
		case 10:
			_roamingRestrictedInSgsnDueToUnsuppportedFeature = new Null();
			_roamingRestrictedInSgsnDueToUnsuppportedFeature->tlv->tag_class = CONTEXT_SPECIFIC;
			_roamingRestrictedInSgsnDueToUnsuppportedFeature->tlv->tag_value = 11;
			children[10] = _roamingRestrictedInSgsnDueToUnsuppportedFeature;
			return _roamingRestrictedInSgsnDueToUnsuppportedFeature;
                case 11:
                {
                        _lsaInformationWithdraw = new LSAInformationWithdraw();
                        ASN1Node* _lsaInformationWithdraw_wrapper = new ASN1Node();
                        _lsaInformationWithdraw_wrapper->tlv = new TLVNode();
                        _lsaInformationWithdraw_wrapper->tlv->is_explicit = true;
                        _lsaInformationWithdraw_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _lsaInformationWithdraw_wrapper->tlv->tag_value = 12;
                        _lsaInformationWithdraw_wrapper->children.push_back(_lsaInformationWithdraw);
                        children[11] = _lsaInformationWithdraw_wrapper;
                        return _lsaInformationWithdraw;
                }
		case 12:
			_gmlc_ListWithdraw = new Null();
			_gmlc_ListWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_gmlc_ListWithdraw->tlv->tag_value = 13;
			children[12] = _gmlc_ListWithdraw;
			return _gmlc_ListWithdraw;
		case 13:
			_istInformationWithdraw = new Null();
			_istInformationWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_istInformationWithdraw->tlv->tag_value = 14;
			children[13] = _istInformationWithdraw;
			return _istInformationWithdraw;
		case 14:
			_specificCSI_Withdraw = new SpecificCSI_Withdraw();
			_specificCSI_Withdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_specificCSI_Withdraw->tlv->tag_value = 15;
			children[14] = _specificCSI_Withdraw;
			return _specificCSI_Withdraw;
		case 15:
			_chargingCharacteristicsWithdraw = new Null();
			_chargingCharacteristicsWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_chargingCharacteristicsWithdraw->tlv->tag_value = 16;
			children[15] = _chargingCharacteristicsWithdraw;
			return _chargingCharacteristicsWithdraw;
		case 16:
			_stn_srWithdraw = new Null();
			_stn_srWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_stn_srWithdraw->tlv->tag_value = 17;
			children[16] = _stn_srWithdraw;
			return _stn_srWithdraw;
                case 17:
                {
                        _epsSubscriptionDataWithdraw = new EPS_SubscriptionDataWithdraw();
                        ASN1Node* _epsSubscriptionDataWithdraw_wrapper = new ASN1Node();
                        _epsSubscriptionDataWithdraw_wrapper->tlv = new TLVNode();
                        _epsSubscriptionDataWithdraw_wrapper->tlv->is_explicit = true;
                        _epsSubscriptionDataWithdraw_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _epsSubscriptionDataWithdraw_wrapper->tlv->tag_value = 18;
                        _epsSubscriptionDataWithdraw_wrapper->children.push_back(_epsSubscriptionDataWithdraw);
                        children[17] = _epsSubscriptionDataWithdraw_wrapper;
                        return _epsSubscriptionDataWithdraw;
                }
		case 18:
			_apn_oi_replacementWithdraw = new Null();
			_apn_oi_replacementWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_apn_oi_replacementWithdraw->tlv->tag_value = 19;
			children[18] = _apn_oi_replacementWithdraw;
			return _apn_oi_replacementWithdraw;
		case 19:
			_csg_SubscriptionDeleted = new Null();
			_csg_SubscriptionDeleted->tlv->tag_class = CONTEXT_SPECIFIC;
			_csg_SubscriptionDeleted->tlv->tag_value = 20;
			children[19] = _csg_SubscriptionDeleted;
			return _csg_SubscriptionDeleted;
		case 20:
			_subscribedPeriodicTAU_RAU_TimerWithdraw = new Null();
			_subscribedPeriodicTAU_RAU_TimerWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_subscribedPeriodicTAU_RAU_TimerWithdraw->tlv->tag_value = 22;
			children[20] = _subscribedPeriodicTAU_RAU_TimerWithdraw;
			return _subscribedPeriodicTAU_RAU_TimerWithdraw;
		case 21:
			_subscribedPeriodicLAU_TimerWithdraw = new Null();
			_subscribedPeriodicLAU_TimerWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_subscribedPeriodicLAU_TimerWithdraw->tlv->tag_value = 23;
			children[21] = _subscribedPeriodicLAU_TimerWithdraw;
			return _subscribedPeriodicLAU_TimerWithdraw;
		case 22:
			_subscribed_vsrvccWithdraw = new Null();
			_subscribed_vsrvccWithdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_subscribed_vsrvccWithdraw->tlv->tag_value = 21;
			children[22] = _subscribed_vsrvccWithdraw;
			return _subscribed_vsrvccWithdraw;
		case 23:
			_vplmn_Csg_SubscriptionDeleted = new Null();
			_vplmn_Csg_SubscriptionDeleted->tlv->tag_class = CONTEXT_SPECIFIC;
			_vplmn_Csg_SubscriptionDeleted->tlv->tag_value = 24;
			children[23] = _vplmn_Csg_SubscriptionDeleted;
			return _vplmn_Csg_SubscriptionDeleted;
		case 24:
			_additionalMSISDN_Withdraw = new Null();
			_additionalMSISDN_Withdraw->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalMSISDN_Withdraw->tlv->tag_value = 25;
			children[24] = _additionalMSISDN_Withdraw;
			return _additionalMSISDN_Withdraw;

		default: return NULL;
	}
}

void asn1::DeleteSubscriberDataArg::set_basicServiceList(){
	if(_basicServiceList == NULL) _basicServiceList = (BasicServiceList*)create_node(1);
}

void asn1::DeleteSubscriberDataArg::set_ss_List(){
	if(_ss_List == NULL) _ss_List = (SS_List*)create_node(2);
}

void asn1::DeleteSubscriberDataArg::set_roamingRestrictionDueToUnsupportedFeature(){
	if(_roamingRestrictionDueToUnsupportedFeature == NULL) _roamingRestrictionDueToUnsupportedFeature = (Null*)create_node(3);
}

void asn1::DeleteSubscriberDataArg::set_regionalSubscriptionIdentifier(){
	if(_regionalSubscriptionIdentifier == NULL) _regionalSubscriptionIdentifier = (ZoneCode*)create_node(4);
}

void asn1::DeleteSubscriberDataArg::set_vbsGroupIndication(){
	if(_vbsGroupIndication == NULL) _vbsGroupIndication = (Null*)create_node(5);
}

void asn1::DeleteSubscriberDataArg::set_vgcsGroupIndication(){
	if(_vgcsGroupIndication == NULL) _vgcsGroupIndication = (Null*)create_node(6);
}

void asn1::DeleteSubscriberDataArg::set_camelSubscriptionInfoWithdraw(){
	if(_camelSubscriptionInfoWithdraw == NULL) _camelSubscriptionInfoWithdraw = (Null*)create_node(7);
}

void asn1::DeleteSubscriberDataArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(8);
}

void asn1::DeleteSubscriberDataArg::set_gprsSubscriptionDataWithdraw(){
	if(_gprsSubscriptionDataWithdraw == NULL) _gprsSubscriptionDataWithdraw = (GPRSSubscriptionDataWithdraw*)create_node(9);
}

void asn1::DeleteSubscriberDataArg::set_roamingRestrictedInSgsnDueToUnsuppportedFeature(){
	if(_roamingRestrictedInSgsnDueToUnsuppportedFeature == NULL) _roamingRestrictedInSgsnDueToUnsuppportedFeature = (Null*)create_node(10);
}

void asn1::DeleteSubscriberDataArg::set_lsaInformationWithdraw(){
	if(_lsaInformationWithdraw == NULL) _lsaInformationWithdraw = (LSAInformationWithdraw*)create_node(11);
}

void asn1::DeleteSubscriberDataArg::set_gmlc_ListWithdraw(){
	if(_gmlc_ListWithdraw == NULL) _gmlc_ListWithdraw = (Null*)create_node(12);
}

void asn1::DeleteSubscriberDataArg::set_istInformationWithdraw(){
	if(_istInformationWithdraw == NULL) _istInformationWithdraw = (Null*)create_node(13);
}

void asn1::DeleteSubscriberDataArg::set_specificCSI_Withdraw(){
	if(_specificCSI_Withdraw == NULL) _specificCSI_Withdraw = (SpecificCSI_Withdraw*)create_node(14);
}

void asn1::DeleteSubscriberDataArg::set_chargingCharacteristicsWithdraw(){
	if(_chargingCharacteristicsWithdraw == NULL) _chargingCharacteristicsWithdraw = (Null*)create_node(15);
}

void asn1::DeleteSubscriberDataArg::set_stn_srWithdraw(){
	if(_stn_srWithdraw == NULL) _stn_srWithdraw = (Null*)create_node(16);
}

void asn1::DeleteSubscriberDataArg::set_epsSubscriptionDataWithdraw(){
	if(_epsSubscriptionDataWithdraw == NULL) _epsSubscriptionDataWithdraw = (EPS_SubscriptionDataWithdraw*)create_node(17);
}

void asn1::DeleteSubscriberDataArg::set_apn_oi_replacementWithdraw(){
	if(_apn_oi_replacementWithdraw == NULL) _apn_oi_replacementWithdraw = (Null*)create_node(18);
}

void asn1::DeleteSubscriberDataArg::set_csg_SubscriptionDeleted(){
	if(_csg_SubscriptionDeleted == NULL) _csg_SubscriptionDeleted = (Null*)create_node(19);
}

void asn1::DeleteSubscriberDataArg::set_subscribedPeriodicTAU_RAU_TimerWithdraw(){
	if(_subscribedPeriodicTAU_RAU_TimerWithdraw == NULL) _subscribedPeriodicTAU_RAU_TimerWithdraw = (Null*)create_node(20);
}

void asn1::DeleteSubscriberDataArg::set_subscribedPeriodicLAU_TimerWithdraw(){
	if(_subscribedPeriodicLAU_TimerWithdraw == NULL) _subscribedPeriodicLAU_TimerWithdraw = (Null*)create_node(21);
}

void asn1::DeleteSubscriberDataArg::set_subscribed_vsrvccWithdraw(){
	if(_subscribed_vsrvccWithdraw == NULL) _subscribed_vsrvccWithdraw = (Null*)create_node(22);
}

void asn1::DeleteSubscriberDataArg::set_vplmn_Csg_SubscriptionDeleted(){
	if(_vplmn_Csg_SubscriptionDeleted == NULL) _vplmn_Csg_SubscriptionDeleted = (Null*)create_node(23);
}

void asn1::DeleteSubscriberDataArg::set_additionalMSISDN_Withdraw(){
	if(_additionalMSISDN_Withdraw == NULL) _additionalMSISDN_Withdraw = (Null*)create_node(24);
}

//SpecificCSI_Withdraw
asn1::SpecificCSI_Withdraw::SpecificCSI_Withdraw(){
	strcpy(node_type_name, "SpecificCSI_Withdraw");

}
asn1::SpecificCSI_Withdraw::~SpecificCSI_Withdraw(){

}

//GPRSSubscriptionDataWithdraw
asn1::GPRSSubscriptionDataWithdraw::GPRSSubscriptionDataWithdraw(){
	strcpy(node_type_name, "GPRSSubscriptionDataWithdraw");
	// allGPRSData
	_allGPRSData = NULL;
	_allGPRSData = new Null();
	children.push_back(_allGPRSData);

	// contextIdList
	_contextIdList = NULL;
	_contextIdList = new ContextIdList();
	children.push_back(_contextIdList);


}
asn1::GPRSSubscriptionDataWithdraw::~GPRSSubscriptionDataWithdraw(){

}

//EPS_SubscriptionDataWithdraw
asn1::EPS_SubscriptionDataWithdraw::EPS_SubscriptionDataWithdraw(){
	strcpy(node_type_name, "EPS_SubscriptionDataWithdraw");
	// allEPS_Data
	_allEPS_Data = NULL;
	_allEPS_Data = new Null();
	children.push_back(_allEPS_Data);

	// contextIdList
	_contextIdList = NULL;
	_contextIdList = new ContextIdList();
	children.push_back(_contextIdList);


}
asn1::EPS_SubscriptionDataWithdraw::~EPS_SubscriptionDataWithdraw(){

}

//ContextIdList
asn1::ContextIdList::ContextIdList(){
	strcpy(node_type_name, "ContextIdList");

}
asn1::ContextIdList::~ContextIdList(){
}

asn1::ContextId* asn1::ContextIdList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ContextId*)children[child_index]; else return NULL;
}

void asn1::ContextIdList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::ContextIdList::create_node(unsigned int _index){
	children[_index] = new ContextId();
	return children[_index];
}

asn1::ASN1Node* asn1::ContextIdList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ContextId());
		return children[children.size() - 1];
	}

}

//LSAInformationWithdraw
asn1::LSAInformationWithdraw::LSAInformationWithdraw(){
	strcpy(node_type_name, "LSAInformationWithdraw");
	// allLSAData
	_allLSAData = NULL;
	_allLSAData = new Null();
	children.push_back(_allLSAData);

	// lsaIdentityList
	_lsaIdentityList = NULL;
	_lsaIdentityList = new LSAIdentityList();
	children.push_back(_lsaIdentityList);


}
asn1::LSAInformationWithdraw::~LSAInformationWithdraw(){

}

//LSAIdentityList
asn1::LSAIdentityList::LSAIdentityList(){
	strcpy(node_type_name, "LSAIdentityList");

}
asn1::LSAIdentityList::~LSAIdentityList(){
}

asn1::LSAIdentity* asn1::LSAIdentityList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (LSAIdentity*)children[child_index]; else return NULL;
}

void asn1::LSAIdentityList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::LSAIdentityList::create_node(unsigned int _index){
	children[_index] = new LSAIdentity();
	return children[_index];
}

asn1::ASN1Node* asn1::LSAIdentityList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new LSAIdentity());
		return children[children.size() - 1];
	}

}

//BasicServiceList
asn1::BasicServiceList::BasicServiceList(){
	strcpy(node_type_name, "BasicServiceList");

}
asn1::BasicServiceList::~BasicServiceList(){
}

asn1::Ext_BasicServiceCode* asn1::BasicServiceList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Ext_BasicServiceCode*)children[child_index]; else return NULL;
}

void asn1::BasicServiceList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::BasicServiceList::create_node(unsigned int _index){
	children[_index] = new Ext_BasicServiceCode();
	return children[_index];
}

asn1::ASN1Node* asn1::BasicServiceList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Ext_BasicServiceCode());
		return children[children.size() - 1];
	}

}

//maxNumOfBasicServices
asn1::maxNumOfBasicServices::maxNumOfBasicServices(){
	strcpy(node_type_name, "maxNumOfBasicServices");

}
asn1::maxNumOfBasicServices::~maxNumOfBasicServices(){

}

//DeleteSubscriberDataRes
asn1::DeleteSubscriberDataRes::DeleteSubscriberDataRes(){
	strcpy(node_type_name, "DeleteSubscriberDataRes");
	// regionalSubscriptionResponse
	_regionalSubscriptionResponse = NULL;
	children.push_back(_regionalSubscriptionResponse);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::DeleteSubscriberDataRes::~DeleteSubscriberDataRes(){

}

asn1::ASN1Node* asn1::DeleteSubscriberDataRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_regionalSubscriptionResponse = new RegionalSubscriptionResponse();
			_regionalSubscriptionResponse->tlv->tag_class = CONTEXT_SPECIFIC;
			_regionalSubscriptionResponse->tlv->tag_value = 0;
			children[0] = _regionalSubscriptionResponse;
			return _regionalSubscriptionResponse;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::DeleteSubscriberDataRes::set_regionalSubscriptionResponse(){
	if(_regionalSubscriptionResponse == NULL) _regionalSubscriptionResponse = (RegionalSubscriptionResponse*)create_node(0);
}

void asn1::DeleteSubscriberDataRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//VlrCamelSubscriptionInfo
asn1::VlrCamelSubscriptionInfo::VlrCamelSubscriptionInfo(){
	strcpy(node_type_name, "VlrCamelSubscriptionInfo");
	// o_CSI
	_o_CSI = NULL;
	children.push_back(_o_CSI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// ss_CSI
	_ss_CSI = NULL;
	children.push_back(_ss_CSI);

	// o_BcsmCamelTDP_CriteriaList
	_o_BcsmCamelTDP_CriteriaList = NULL;
	children.push_back(_o_BcsmCamelTDP_CriteriaList);

	// tif_CSI
	_tif_CSI = NULL;
	children.push_back(_tif_CSI);

	// m_CSI
	_m_CSI = NULL;
	children.push_back(_m_CSI);

	// mo_sms_CSI
	_mo_sms_CSI = NULL;
	children.push_back(_mo_sms_CSI);

	// vt_CSI
	_vt_CSI = NULL;
	children.push_back(_vt_CSI);

	// t_BCSM_CAMEL_TDP_CriteriaList
	_t_BCSM_CAMEL_TDP_CriteriaList = NULL;
	children.push_back(_t_BCSM_CAMEL_TDP_CriteriaList);

	// d_CSI
	_d_CSI = NULL;
	children.push_back(_d_CSI);

	// mt_sms_CSI
	_mt_sms_CSI = NULL;
	children.push_back(_mt_sms_CSI);

	// mt_smsCAMELTDP_CriteriaList
	_mt_smsCAMELTDP_CriteriaList = NULL;
	children.push_back(_mt_smsCAMELTDP_CriteriaList);


}
asn1::VlrCamelSubscriptionInfo::~VlrCamelSubscriptionInfo(){

}

asn1::ASN1Node* asn1::VlrCamelSubscriptionInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_o_CSI = new O_CSI();
			_o_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_CSI->tlv->tag_value = 0;
			children[0] = _o_CSI;
			return _o_CSI;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_ss_CSI = new SS_CSI();
			_ss_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_CSI->tlv->tag_value = 2;
			children[2] = _ss_CSI;
			return _ss_CSI;
		case 3:
			_o_BcsmCamelTDP_CriteriaList = new O_BcsmCamelTDPCriteriaList();
			_o_BcsmCamelTDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_BcsmCamelTDP_CriteriaList->tlv->tag_value = 4;
			children[3] = _o_BcsmCamelTDP_CriteriaList;
			return _o_BcsmCamelTDP_CriteriaList;
		case 4:
			_tif_CSI = new Null();
			_tif_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_tif_CSI->tlv->tag_value = 3;
			children[4] = _tif_CSI;
			return _tif_CSI;
		case 5:
			_m_CSI = new M_CSI();
			_m_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_m_CSI->tlv->tag_value = 5;
			children[5] = _m_CSI;
			return _m_CSI;
		case 6:
			_mo_sms_CSI = new SMS_CSI();
			_mo_sms_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_mo_sms_CSI->tlv->tag_value = 6;
			children[6] = _mo_sms_CSI;
			return _mo_sms_CSI;
		case 7:
			_vt_CSI = new T_CSI();
			_vt_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_vt_CSI->tlv->tag_value = 7;
			children[7] = _vt_CSI;
			return _vt_CSI;
		case 8:
			_t_BCSM_CAMEL_TDP_CriteriaList = new T_BCSM_CAMEL_TDP_CriteriaList();
			_t_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_t_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_value = 8;
			children[8] = _t_BCSM_CAMEL_TDP_CriteriaList;
			return _t_BCSM_CAMEL_TDP_CriteriaList;
		case 9:
			_d_CSI = new D_CSI();
			_d_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_d_CSI->tlv->tag_value = 9;
			children[9] = _d_CSI;
			return _d_CSI;
		case 10:
			_mt_sms_CSI = new SMS_CSI();
			_mt_sms_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_mt_sms_CSI->tlv->tag_value = 10;
			children[10] = _mt_sms_CSI;
			return _mt_sms_CSI;
		case 11:
			_mt_smsCAMELTDP_CriteriaList = new MT_smsCAMELTDP_CriteriaList();
			_mt_smsCAMELTDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_mt_smsCAMELTDP_CriteriaList->tlv->tag_value = 11;
			children[11] = _mt_smsCAMELTDP_CriteriaList;
			return _mt_smsCAMELTDP_CriteriaList;

		default: return NULL;
	}
}

void asn1::VlrCamelSubscriptionInfo::set_o_CSI(){
	if(_o_CSI == NULL) _o_CSI = (O_CSI*)create_node(0);
}

void asn1::VlrCamelSubscriptionInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::VlrCamelSubscriptionInfo::set_ss_CSI(){
	if(_ss_CSI == NULL) _ss_CSI = (SS_CSI*)create_node(2);
}

void asn1::VlrCamelSubscriptionInfo::set_o_BcsmCamelTDP_CriteriaList(){
	if(_o_BcsmCamelTDP_CriteriaList == NULL) _o_BcsmCamelTDP_CriteriaList = (O_BcsmCamelTDPCriteriaList*)create_node(3);
}

void asn1::VlrCamelSubscriptionInfo::set_tif_CSI(){
	if(_tif_CSI == NULL) _tif_CSI = (Null*)create_node(4);
}

void asn1::VlrCamelSubscriptionInfo::set_m_CSI(){
	if(_m_CSI == NULL) _m_CSI = (M_CSI*)create_node(5);
}

void asn1::VlrCamelSubscriptionInfo::set_mo_sms_CSI(){
	if(_mo_sms_CSI == NULL) _mo_sms_CSI = (SMS_CSI*)create_node(6);
}

void asn1::VlrCamelSubscriptionInfo::set_vt_CSI(){
	if(_vt_CSI == NULL) _vt_CSI = (T_CSI*)create_node(7);
}

void asn1::VlrCamelSubscriptionInfo::set_t_BCSM_CAMEL_TDP_CriteriaList(){
	if(_t_BCSM_CAMEL_TDP_CriteriaList == NULL) _t_BCSM_CAMEL_TDP_CriteriaList = (T_BCSM_CAMEL_TDP_CriteriaList*)create_node(8);
}

void asn1::VlrCamelSubscriptionInfo::set_d_CSI(){
	if(_d_CSI == NULL) _d_CSI = (D_CSI*)create_node(9);
}

void asn1::VlrCamelSubscriptionInfo::set_mt_sms_CSI(){
	if(_mt_sms_CSI == NULL) _mt_sms_CSI = (SMS_CSI*)create_node(10);
}

void asn1::VlrCamelSubscriptionInfo::set_mt_smsCAMELTDP_CriteriaList(){
	if(_mt_smsCAMELTDP_CriteriaList == NULL) _mt_smsCAMELTDP_CriteriaList = (MT_smsCAMELTDP_CriteriaList*)create_node(11);
}

//MT_smsCAMELTDP_CriteriaList
asn1::MT_smsCAMELTDP_CriteriaList::MT_smsCAMELTDP_CriteriaList(){
	strcpy(node_type_name, "MT_smsCAMELTDP_CriteriaList");

}
asn1::MT_smsCAMELTDP_CriteriaList::~MT_smsCAMELTDP_CriteriaList(){
}

asn1::MT_smsCAMELTDP_Criteria* asn1::MT_smsCAMELTDP_CriteriaList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (MT_smsCAMELTDP_Criteria*)children[child_index]; else return NULL;
}

void asn1::MT_smsCAMELTDP_CriteriaList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::MT_smsCAMELTDP_CriteriaList::create_node(unsigned int _index){
	children[_index] = new MT_smsCAMELTDP_Criteria();
	return children[_index];
}

asn1::ASN1Node* asn1::MT_smsCAMELTDP_CriteriaList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new MT_smsCAMELTDP_Criteria());
		return children[children.size() - 1];
	}

}

//MT_smsCAMELTDP_Criteria
asn1::MT_smsCAMELTDP_Criteria::MT_smsCAMELTDP_Criteria(){
	strcpy(node_type_name, "MT_smsCAMELTDP_Criteria");
	// sms_TriggerDetectionPoint
	_sms_TriggerDetectionPoint = NULL;
	_sms_TriggerDetectionPoint = new SMS_TriggerDetectionPoint();
	children.push_back(_sms_TriggerDetectionPoint);

	// tpdu_TypeCriterion
	_tpdu_TypeCriterion = NULL;
	children.push_back(_tpdu_TypeCriterion);


}
asn1::MT_smsCAMELTDP_Criteria::~MT_smsCAMELTDP_Criteria(){

}

asn1::ASN1Node* asn1::MT_smsCAMELTDP_Criteria::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_tpdu_TypeCriterion = new TPDU_TypeCriterion();
			_tpdu_TypeCriterion->tlv->tag_class = CONTEXT_SPECIFIC;
			_tpdu_TypeCriterion->tlv->tag_value = 0;
			children[1] = _tpdu_TypeCriterion;
			return _tpdu_TypeCriterion;

		default: return NULL;
	}
}

void asn1::MT_smsCAMELTDP_Criteria::set_tpdu_TypeCriterion(){
	if(_tpdu_TypeCriterion == NULL) _tpdu_TypeCriterion = (TPDU_TypeCriterion*)create_node(1);
}

//TPDU_TypeCriterion
asn1::TPDU_TypeCriterion::TPDU_TypeCriterion(){
	strcpy(node_type_name, "TPDU_TypeCriterion");

}
asn1::TPDU_TypeCriterion::~TPDU_TypeCriterion(){
}

asn1::MT_SMS_TPDU_Type* asn1::TPDU_TypeCriterion::get_child(unsigned int child_index){
	if(child_index < children.size()) return (MT_SMS_TPDU_Type*)children[child_index]; else return NULL;
}

void asn1::TPDU_TypeCriterion::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::TPDU_TypeCriterion::create_node(unsigned int _index){
	children[_index] = new MT_SMS_TPDU_Type();
	return children[_index];
}

asn1::ASN1Node* asn1::TPDU_TypeCriterion::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new MT_SMS_TPDU_Type());
		return children[children.size() - 1];
	}

}

//maxNumOfTPDUTypes
asn1::maxNumOfTPDUTypes::maxNumOfTPDUTypes(){
	strcpy(node_type_name, "maxNumOfTPDUTypes");

}
asn1::maxNumOfTPDUTypes::~maxNumOfTPDUTypes(){

}

//MT_SMS_TPDU_Type
asn1::MT_SMS_TPDU_Type::MT_SMS_TPDU_Type(){
	strcpy(node_type_name, "MT_SMS_TPDU_Type");

}
asn1::MT_SMS_TPDU_Type::~MT_SMS_TPDU_Type(){

}

//D_CSI
asn1::D_CSI::D_CSI(){
	strcpy(node_type_name, "D_CSI");
	// dp_AnalysedInfoCriteriaList
	_dp_AnalysedInfoCriteriaList = NULL;
	children.push_back(_dp_AnalysedInfoCriteriaList);

	// camelCapabilityHandling
	_camelCapabilityHandling = NULL;
	children.push_back(_camelCapabilityHandling);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// csi_Active
	_csi_Active = NULL;
	children.push_back(_csi_Active);


}
asn1::D_CSI::~D_CSI(){

}

asn1::ASN1Node* asn1::D_CSI::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_dp_AnalysedInfoCriteriaList = new DP_AnalysedInfoCriteriaList();
			_dp_AnalysedInfoCriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_dp_AnalysedInfoCriteriaList->tlv->tag_value = 0;
			children[0] = _dp_AnalysedInfoCriteriaList;
			return _dp_AnalysedInfoCriteriaList;
		case 1:
			_camelCapabilityHandling = new CamelCapabilityHandling();
			_camelCapabilityHandling->tlv->tag_class = CONTEXT_SPECIFIC;
			_camelCapabilityHandling->tlv->tag_value = 1;
			children[1] = _camelCapabilityHandling;
			return _camelCapabilityHandling;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 3;
			children[3] = _notificationToCSE;
			return _notificationToCSE;
		case 4:
			_csi_Active = new Null();
			_csi_Active->tlv->tag_class = CONTEXT_SPECIFIC;
			_csi_Active->tlv->tag_value = 4;
			children[4] = _csi_Active;
			return _csi_Active;

		default: return NULL;
	}
}

void asn1::D_CSI::set_dp_AnalysedInfoCriteriaList(){
	if(_dp_AnalysedInfoCriteriaList == NULL) _dp_AnalysedInfoCriteriaList = (DP_AnalysedInfoCriteriaList*)create_node(0);
}

void asn1::D_CSI::set_camelCapabilityHandling(){
	if(_camelCapabilityHandling == NULL) _camelCapabilityHandling = (CamelCapabilityHandling*)create_node(1);
}

void asn1::D_CSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::D_CSI::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(3);
}

void asn1::D_CSI::set_csi_Active(){
	if(_csi_Active == NULL) _csi_Active = (Null*)create_node(4);
}

//DP_AnalysedInfoCriteriaList
asn1::DP_AnalysedInfoCriteriaList::DP_AnalysedInfoCriteriaList(){
	strcpy(node_type_name, "DP_AnalysedInfoCriteriaList");

}
asn1::DP_AnalysedInfoCriteriaList::~DP_AnalysedInfoCriteriaList(){
}

asn1::DP_AnalysedInfoCriterium* asn1::DP_AnalysedInfoCriteriaList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (DP_AnalysedInfoCriterium*)children[child_index]; else return NULL;
}

void asn1::DP_AnalysedInfoCriteriaList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::DP_AnalysedInfoCriteriaList::create_node(unsigned int _index){
	children[_index] = new DP_AnalysedInfoCriterium();
	return children[_index];
}

asn1::ASN1Node* asn1::DP_AnalysedInfoCriteriaList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new DP_AnalysedInfoCriterium());
		return children[children.size() - 1];
	}

}

//maxNumOfDP_AnalysedInfoCriteria
asn1::maxNumOfDP_AnalysedInfoCriteria::maxNumOfDP_AnalysedInfoCriteria(){
	strcpy(node_type_name, "maxNumOfDP_AnalysedInfoCriteria");

}
asn1::maxNumOfDP_AnalysedInfoCriteria::~maxNumOfDP_AnalysedInfoCriteria(){

}

//DP_AnalysedInfoCriterium
asn1::DP_AnalysedInfoCriterium::DP_AnalysedInfoCriterium(){
	strcpy(node_type_name, "DP_AnalysedInfoCriterium");
	// dialledNumber
	_dialledNumber = NULL;
	_dialledNumber = new ISDN_AddressString();
	children.push_back(_dialledNumber);

	// serviceKey
	_serviceKey = NULL;
	_serviceKey = new ServiceKey();
	children.push_back(_serviceKey);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	children.push_back(_gsmSCF_Address);

	// defaultCallHandling
	_defaultCallHandling = NULL;
	_defaultCallHandling = new DefaultCallHandling();
	children.push_back(_defaultCallHandling);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::DP_AnalysedInfoCriterium::~DP_AnalysedInfoCriterium(){

}

asn1::ASN1Node* asn1::DP_AnalysedInfoCriterium::create_node(unsigned int _index){
	switch(_index){
		case 4:
			_extensionContainer = new ExtensionContainer();
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::DP_AnalysedInfoCriterium::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//SS_CSI
asn1::SS_CSI::SS_CSI(){
	strcpy(node_type_name, "SS_CSI");
	// ss_CamelData
	_ss_CamelData = NULL;
	_ss_CamelData = new SS_CamelData();
	children.push_back(_ss_CamelData);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// csi_Active
	_csi_Active = NULL;
	children.push_back(_csi_Active);


}
asn1::SS_CSI::~SS_CSI(){

}

asn1::ASN1Node* asn1::SS_CSI::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 0;
			children[2] = _notificationToCSE;
			return _notificationToCSE;
		case 3:
			_csi_Active = new Null();
			_csi_Active->tlv->tag_class = CONTEXT_SPECIFIC;
			_csi_Active->tlv->tag_value = 1;
			children[3] = _csi_Active;
			return _csi_Active;

		default: return NULL;
	}
}

void asn1::SS_CSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::SS_CSI::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(2);
}

void asn1::SS_CSI::set_csi_Active(){
	if(_csi_Active == NULL) _csi_Active = (Null*)create_node(3);
}

//SS_CamelData
asn1::SS_CamelData::SS_CamelData(){
	strcpy(node_type_name, "SS_CamelData");
	// ss_EventList
	_ss_EventList = NULL;
	_ss_EventList = new SS_EventList();
	children.push_back(_ss_EventList);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	children.push_back(_gsmSCF_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SS_CamelData::~SS_CamelData(){

}

asn1::ASN1Node* asn1::SS_CamelData::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SS_CamelData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//SS_EventList
asn1::SS_EventList::SS_EventList(){
	strcpy(node_type_name, "SS_EventList");

}
asn1::SS_EventList::~SS_EventList(){
}

asn1::SS_Code* asn1::SS_EventList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (SS_Code*)children[child_index]; else return NULL;
}

void asn1::SS_EventList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::SS_EventList::create_node(unsigned int _index){
	children[_index] = new SS_Code();
	return children[_index];
}

asn1::ASN1Node* asn1::SS_EventList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new SS_Code());
		return children[children.size() - 1];
	}

}

//maxNumOfCamelSSEvents
asn1::maxNumOfCamelSSEvents::maxNumOfCamelSSEvents(){
	strcpy(node_type_name, "maxNumOfCamelSSEvents");

}
asn1::maxNumOfCamelSSEvents::~maxNumOfCamelSSEvents(){

}

//O_CSI
asn1::O_CSI::O_CSI(){
	strcpy(node_type_name, "O_CSI");
	// o_BcsmCamelTDPDataList
	_o_BcsmCamelTDPDataList = NULL;
	_o_BcsmCamelTDPDataList = new O_BcsmCamelTDPDataList();
	children.push_back(_o_BcsmCamelTDPDataList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// camelCapabilityHandling
	_camelCapabilityHandling = NULL;
	children.push_back(_camelCapabilityHandling);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// csiActive
	_csiActive = NULL;
	children.push_back(_csiActive);


}
asn1::O_CSI::~O_CSI(){

}

asn1::ASN1Node* asn1::O_CSI::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_camelCapabilityHandling = new CamelCapabilityHandling();
			_camelCapabilityHandling->tlv->tag_class = CONTEXT_SPECIFIC;
			_camelCapabilityHandling->tlv->tag_value = 0;
			children[2] = _camelCapabilityHandling;
			return _camelCapabilityHandling;
		case 3:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 1;
			children[3] = _notificationToCSE;
			return _notificationToCSE;
		case 4:
			_csiActive = new Null();
			_csiActive->tlv->tag_class = CONTEXT_SPECIFIC;
			_csiActive->tlv->tag_value = 2;
			children[4] = _csiActive;
			return _csiActive;

		default: return NULL;
	}
}

void asn1::O_CSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::O_CSI::set_camelCapabilityHandling(){
	if(_camelCapabilityHandling == NULL) _camelCapabilityHandling = (CamelCapabilityHandling*)create_node(2);
}

void asn1::O_CSI::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(3);
}

void asn1::O_CSI::set_csiActive(){
	if(_csiActive == NULL) _csiActive = (Null*)create_node(4);
}

//O_BcsmCamelTDPDataList
asn1::O_BcsmCamelTDPDataList::O_BcsmCamelTDPDataList(){
	strcpy(node_type_name, "O_BcsmCamelTDPDataList");

}
asn1::O_BcsmCamelTDPDataList::~O_BcsmCamelTDPDataList(){
}

asn1::O_BcsmCamelTDPData* asn1::O_BcsmCamelTDPDataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (O_BcsmCamelTDPData*)children[child_index]; else return NULL;
}

void asn1::O_BcsmCamelTDPDataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::O_BcsmCamelTDPDataList::create_node(unsigned int _index){
	children[_index] = new O_BcsmCamelTDPData();
	return children[_index];
}

asn1::ASN1Node* asn1::O_BcsmCamelTDPDataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new O_BcsmCamelTDPData());
		return children[children.size() - 1];
	}

}

//maxNumOfCamelTDPData
asn1::maxNumOfCamelTDPData::maxNumOfCamelTDPData(){
	strcpy(node_type_name, "maxNumOfCamelTDPData");

}
asn1::maxNumOfCamelTDPData::~maxNumOfCamelTDPData(){

}

//O_BcsmCamelTDPData
asn1::O_BcsmCamelTDPData::O_BcsmCamelTDPData(){
	strcpy(node_type_name, "O_BcsmCamelTDPData");
	// o_BcsmTriggerDetectionPoint
	_o_BcsmTriggerDetectionPoint = NULL;
	_o_BcsmTriggerDetectionPoint = new O_BcsmTriggerDetectionPoint();
	children.push_back(_o_BcsmTriggerDetectionPoint);

	// serviceKey
	_serviceKey = NULL;
	_serviceKey = new ServiceKey();
	children.push_back(_serviceKey);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	_gsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsmSCF_Address->tlv->tag_value = 0;
	children.push_back(_gsmSCF_Address);

	// defaultCallHandling
	_defaultCallHandling = NULL;
	_defaultCallHandling = new DefaultCallHandling();
	_defaultCallHandling->tlv->tag_class = CONTEXT_SPECIFIC;
	_defaultCallHandling->tlv->tag_value = 1;
	children.push_back(_defaultCallHandling);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::O_BcsmCamelTDPData::~O_BcsmCamelTDPData(){

}

asn1::ASN1Node* asn1::O_BcsmCamelTDPData::create_node(unsigned int _index){
	switch(_index){
		case 4:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::O_BcsmCamelTDPData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//ServiceKey
asn1::ServiceKey::ServiceKey(){
	strcpy(node_type_name, "ServiceKey");

}
asn1::ServiceKey::~ServiceKey(){

}

//O_BcsmTriggerDetectionPoint
asn1::O_BcsmTriggerDetectionPoint::O_BcsmTriggerDetectionPoint(){
	strcpy(node_type_name, "O_BcsmTriggerDetectionPoint");

}
asn1::O_BcsmTriggerDetectionPoint::~O_BcsmTriggerDetectionPoint(){

}

//O_BcsmCamelTDPCriteriaList
asn1::O_BcsmCamelTDPCriteriaList::O_BcsmCamelTDPCriteriaList(){
	strcpy(node_type_name, "O_BcsmCamelTDPCriteriaList");

}
asn1::O_BcsmCamelTDPCriteriaList::~O_BcsmCamelTDPCriteriaList(){
}

asn1::O_BcsmCamelTDP_Criteria* asn1::O_BcsmCamelTDPCriteriaList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (O_BcsmCamelTDP_Criteria*)children[child_index]; else return NULL;
}

void asn1::O_BcsmCamelTDPCriteriaList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::O_BcsmCamelTDPCriteriaList::create_node(unsigned int _index){
	children[_index] = new O_BcsmCamelTDP_Criteria();
	return children[_index];
}

asn1::ASN1Node* asn1::O_BcsmCamelTDPCriteriaList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new O_BcsmCamelTDP_Criteria());
		return children[children.size() - 1];
	}

}

//T_BCSM_CAMEL_TDP_CriteriaList
asn1::T_BCSM_CAMEL_TDP_CriteriaList::T_BCSM_CAMEL_TDP_CriteriaList(){
	strcpy(node_type_name, "T_BCSM_CAMEL_TDP_CriteriaList");

}
asn1::T_BCSM_CAMEL_TDP_CriteriaList::~T_BCSM_CAMEL_TDP_CriteriaList(){
}

asn1::T_BCSM_CAMEL_TDP_Criteria* asn1::T_BCSM_CAMEL_TDP_CriteriaList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (T_BCSM_CAMEL_TDP_Criteria*)children[child_index]; else return NULL;
}

void asn1::T_BCSM_CAMEL_TDP_CriteriaList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::T_BCSM_CAMEL_TDP_CriteriaList::create_node(unsigned int _index){
	children[_index] = new T_BCSM_CAMEL_TDP_Criteria();
	return children[_index];
}

asn1::ASN1Node* asn1::T_BCSM_CAMEL_TDP_CriteriaList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new T_BCSM_CAMEL_TDP_Criteria());
		return children[children.size() - 1];
	}

}

//O_BcsmCamelTDP_Criteria
asn1::O_BcsmCamelTDP_Criteria::O_BcsmCamelTDP_Criteria(){
	strcpy(node_type_name, "O_BcsmCamelTDP_Criteria");
	// o_BcsmTriggerDetectionPoint
	_o_BcsmTriggerDetectionPoint = NULL;
	_o_BcsmTriggerDetectionPoint = new O_BcsmTriggerDetectionPoint();
	children.push_back(_o_BcsmTriggerDetectionPoint);

	// destinationNumberCriteria
	_destinationNumberCriteria = NULL;
	children.push_back(_destinationNumberCriteria);

	// basicServiceCriteria
	_basicServiceCriteria = NULL;
	children.push_back(_basicServiceCriteria);

	// callTypeCriteria
	_callTypeCriteria = NULL;
	children.push_back(_callTypeCriteria);

	// o_CauseValueCriteria
	_o_CauseValueCriteria = NULL;
	children.push_back(_o_CauseValueCriteria);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::O_BcsmCamelTDP_Criteria::~O_BcsmCamelTDP_Criteria(){

}

asn1::ASN1Node* asn1::O_BcsmCamelTDP_Criteria::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_destinationNumberCriteria = new DestinationNumberCriteria();
			_destinationNumberCriteria->tlv->tag_class = CONTEXT_SPECIFIC;
			_destinationNumberCriteria->tlv->tag_value = 0;
			children[1] = _destinationNumberCriteria;
			return _destinationNumberCriteria;
		case 2:
			_basicServiceCriteria = new BasicServiceCriteria();
			_basicServiceCriteria->tlv->tag_class = CONTEXT_SPECIFIC;
			_basicServiceCriteria->tlv->tag_value = 1;
			children[2] = _basicServiceCriteria;
			return _basicServiceCriteria;
		case 3:
			_callTypeCriteria = new CallTypeCriteria();
			_callTypeCriteria->tlv->tag_class = CONTEXT_SPECIFIC;
			_callTypeCriteria->tlv->tag_value = 2;
			children[3] = _callTypeCriteria;
			return _callTypeCriteria;
		case 4:
			_o_CauseValueCriteria = new O_CauseValueCriteria();
			_o_CauseValueCriteria->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_CauseValueCriteria->tlv->tag_value = 3;
			children[4] = _o_CauseValueCriteria;
			return _o_CauseValueCriteria;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 4;
			children[5] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::O_BcsmCamelTDP_Criteria::set_destinationNumberCriteria(){
	if(_destinationNumberCriteria == NULL) _destinationNumberCriteria = (DestinationNumberCriteria*)create_node(1);
}

void asn1::O_BcsmCamelTDP_Criteria::set_basicServiceCriteria(){
	if(_basicServiceCriteria == NULL) _basicServiceCriteria = (BasicServiceCriteria*)create_node(2);
}

void asn1::O_BcsmCamelTDP_Criteria::set_callTypeCriteria(){
	if(_callTypeCriteria == NULL) _callTypeCriteria = (CallTypeCriteria*)create_node(3);
}

void asn1::O_BcsmCamelTDP_Criteria::set_o_CauseValueCriteria(){
	if(_o_CauseValueCriteria == NULL) _o_CauseValueCriteria = (O_CauseValueCriteria*)create_node(4);
}

void asn1::O_BcsmCamelTDP_Criteria::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

//T_BCSM_CAMEL_TDP_Criteria
asn1::T_BCSM_CAMEL_TDP_Criteria::T_BCSM_CAMEL_TDP_Criteria(){
	strcpy(node_type_name, "T_BCSM_CAMEL_TDP_Criteria");
	// t_BCSM_TriggerDetectionPoint
	_t_BCSM_TriggerDetectionPoint = NULL;
	_t_BCSM_TriggerDetectionPoint = new T_BcsmTriggerDetectionPoint();
	children.push_back(_t_BCSM_TriggerDetectionPoint);

	// basicServiceCriteria
	_basicServiceCriteria = NULL;
	children.push_back(_basicServiceCriteria);

	// t_CauseValueCriteria
	_t_CauseValueCriteria = NULL;
	children.push_back(_t_CauseValueCriteria);


}
asn1::T_BCSM_CAMEL_TDP_Criteria::~T_BCSM_CAMEL_TDP_Criteria(){

}

asn1::ASN1Node* asn1::T_BCSM_CAMEL_TDP_Criteria::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_basicServiceCriteria = new BasicServiceCriteria();
			_basicServiceCriteria->tlv->tag_class = CONTEXT_SPECIFIC;
			_basicServiceCriteria->tlv->tag_value = 0;
			children[1] = _basicServiceCriteria;
			return _basicServiceCriteria;
		case 2:
			_t_CauseValueCriteria = new T_CauseValueCriteria();
			_t_CauseValueCriteria->tlv->tag_class = CONTEXT_SPECIFIC;
			_t_CauseValueCriteria->tlv->tag_value = 1;
			children[2] = _t_CauseValueCriteria;
			return _t_CauseValueCriteria;

		default: return NULL;
	}
}

void asn1::T_BCSM_CAMEL_TDP_Criteria::set_basicServiceCriteria(){
	if(_basicServiceCriteria == NULL) _basicServiceCriteria = (BasicServiceCriteria*)create_node(1);
}

void asn1::T_BCSM_CAMEL_TDP_Criteria::set_t_CauseValueCriteria(){
	if(_t_CauseValueCriteria == NULL) _t_CauseValueCriteria = (T_CauseValueCriteria*)create_node(2);
}

//DestinationNumberCriteria
asn1::DestinationNumberCriteria::DestinationNumberCriteria(){
	strcpy(node_type_name, "DestinationNumberCriteria");
	// matchType
	_matchType = NULL;
	_matchType = new MatchType();
	_matchType->tlv->tag_class = CONTEXT_SPECIFIC;
	_matchType->tlv->tag_value = 0;
	children.push_back(_matchType);

	// destinationNumberList
	_destinationNumberList = NULL;
	children.push_back(_destinationNumberList);

	// destinationNumberLengthList
	_destinationNumberLengthList = NULL;
	children.push_back(_destinationNumberLengthList);


}
asn1::DestinationNumberCriteria::~DestinationNumberCriteria(){

}

asn1::ASN1Node* asn1::DestinationNumberCriteria::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_destinationNumberList = new DestinationNumberList();
			_destinationNumberList->tlv->tag_class = CONTEXT_SPECIFIC;
			_destinationNumberList->tlv->tag_value = 1;
			children[1] = _destinationNumberList;
			return _destinationNumberList;
		case 2:
			_destinationNumberLengthList = new DestinationNumberLengthList();
			_destinationNumberLengthList->tlv->tag_class = CONTEXT_SPECIFIC;
			_destinationNumberLengthList->tlv->tag_value = 2;
			children[2] = _destinationNumberLengthList;
			return _destinationNumberLengthList;

		default: return NULL;
	}
}

void asn1::DestinationNumberCriteria::set_destinationNumberList(){
	if(_destinationNumberList == NULL) _destinationNumberList = (DestinationNumberList*)create_node(1);
}

void asn1::DestinationNumberCriteria::set_destinationNumberLengthList(){
	if(_destinationNumberLengthList == NULL) _destinationNumberLengthList = (DestinationNumberLengthList*)create_node(2);
}

//DestinationNumberList
asn1::DestinationNumberList::DestinationNumberList(){
	strcpy(node_type_name, "DestinationNumberList");

}
asn1::DestinationNumberList::~DestinationNumberList(){
}

asn1::ISDN_AddressString* asn1::DestinationNumberList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ISDN_AddressString*)children[child_index]; else return NULL;
}

void asn1::DestinationNumberList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::DestinationNumberList::create_node(unsigned int _index){
	children[_index] = new ISDN_AddressString();
	return children[_index];
}

asn1::ASN1Node* asn1::DestinationNumberList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ISDN_AddressString());
		return children[children.size() - 1];
	}

}

//DestinationNumberLengthList
asn1::DestinationNumberLengthList::DestinationNumberLengthList(){
	strcpy(node_type_name, "DestinationNumberLengthList");

}
asn1::DestinationNumberLengthList::~DestinationNumberLengthList(){
}

asn1::Integer* asn1::DestinationNumberLengthList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Integer*)children[child_index]; else return NULL;
}

void asn1::DestinationNumberLengthList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::DestinationNumberLengthList::create_node(unsigned int _index){
	children[_index] = new Integer();
	return children[_index];
}

asn1::ASN1Node* asn1::DestinationNumberLengthList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Integer());
		return children[children.size() - 1];
	}

}

//BasicServiceCriteria
asn1::BasicServiceCriteria::BasicServiceCriteria(){
	strcpy(node_type_name, "BasicServiceCriteria");

}
asn1::BasicServiceCriteria::~BasicServiceCriteria(){
}

asn1::Ext_BasicServiceCode* asn1::BasicServiceCriteria::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Ext_BasicServiceCode*)children[child_index]; else return NULL;
}

void asn1::BasicServiceCriteria::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::BasicServiceCriteria::create_node(unsigned int _index){
	children[_index] = new Ext_BasicServiceCode();
	return children[_index];
}

asn1::ASN1Node* asn1::BasicServiceCriteria::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Ext_BasicServiceCode());
		return children[children.size() - 1];
	}

}

//maxNumOfISDN_AddressDigits
asn1::maxNumOfISDN_AddressDigits::maxNumOfISDN_AddressDigits(){
	strcpy(node_type_name, "maxNumOfISDN_AddressDigits");

}
asn1::maxNumOfISDN_AddressDigits::~maxNumOfISDN_AddressDigits(){

}

//maxNumOfCamelDestinationNumbers
asn1::maxNumOfCamelDestinationNumbers::maxNumOfCamelDestinationNumbers(){
	strcpy(node_type_name, "maxNumOfCamelDestinationNumbers");

}
asn1::maxNumOfCamelDestinationNumbers::~maxNumOfCamelDestinationNumbers(){

}

//maxNumOfCamelDestinationNumberLengths
asn1::maxNumOfCamelDestinationNumberLengths::maxNumOfCamelDestinationNumberLengths(){
	strcpy(node_type_name, "maxNumOfCamelDestinationNumberLengths");

}
asn1::maxNumOfCamelDestinationNumberLengths::~maxNumOfCamelDestinationNumberLengths(){

}

//maxNumOfCamelBasicServiceCriteria
asn1::maxNumOfCamelBasicServiceCriteria::maxNumOfCamelBasicServiceCriteria(){
	strcpy(node_type_name, "maxNumOfCamelBasicServiceCriteria");

}
asn1::maxNumOfCamelBasicServiceCriteria::~maxNumOfCamelBasicServiceCriteria(){

}

//CallTypeCriteria
asn1::CallTypeCriteria::CallTypeCriteria(){
	strcpy(node_type_name, "CallTypeCriteria");

}
asn1::CallTypeCriteria::~CallTypeCriteria(){

}

//MatchType
asn1::MatchType::MatchType(){
	strcpy(node_type_name, "MatchType");

}
asn1::MatchType::~MatchType(){

}

//O_CauseValueCriteria
asn1::O_CauseValueCriteria::O_CauseValueCriteria(){
	strcpy(node_type_name, "O_CauseValueCriteria");

}
asn1::O_CauseValueCriteria::~O_CauseValueCriteria(){
}

asn1::CauseValue* asn1::O_CauseValueCriteria::get_child(unsigned int child_index){
	if(child_index < children.size()) return (CauseValue*)children[child_index]; else return NULL;
}

void asn1::O_CauseValueCriteria::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::O_CauseValueCriteria::create_node(unsigned int _index){
	children[_index] = new CauseValue();
	return children[_index];
}

asn1::ASN1Node* asn1::O_CauseValueCriteria::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new CauseValue());
		return children[children.size() - 1];
	}

}

//T_CauseValueCriteria
asn1::T_CauseValueCriteria::T_CauseValueCriteria(){
	strcpy(node_type_name, "T_CauseValueCriteria");

}
asn1::T_CauseValueCriteria::~T_CauseValueCriteria(){
}

asn1::CauseValue* asn1::T_CauseValueCriteria::get_child(unsigned int child_index){
	if(child_index < children.size()) return (CauseValue*)children[child_index]; else return NULL;
}

void asn1::T_CauseValueCriteria::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::T_CauseValueCriteria::create_node(unsigned int _index){
	children[_index] = new CauseValue();
	return children[_index];
}

asn1::ASN1Node* asn1::T_CauseValueCriteria::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new CauseValue());
		return children[children.size() - 1];
	}

}

//maxNumOfCAMEL_O_CauseValueCriteria
asn1::maxNumOfCAMEL_O_CauseValueCriteria::maxNumOfCAMEL_O_CauseValueCriteria(){
	strcpy(node_type_name, "maxNumOfCAMEL_O_CauseValueCriteria");

}
asn1::maxNumOfCAMEL_O_CauseValueCriteria::~maxNumOfCAMEL_O_CauseValueCriteria(){

}

//maxNumOfCAMEL_T_CauseValueCriteria
asn1::maxNumOfCAMEL_T_CauseValueCriteria::maxNumOfCAMEL_T_CauseValueCriteria(){
	strcpy(node_type_name, "maxNumOfCAMEL_T_CauseValueCriteria");

}
asn1::maxNumOfCAMEL_T_CauseValueCriteria::~maxNumOfCAMEL_T_CauseValueCriteria(){

}

//CauseValue
asn1::CauseValue::CauseValue(){
	strcpy(node_type_name, "CauseValue");

}
asn1::CauseValue::~CauseValue(){

}

//DefaultCallHandling
asn1::DefaultCallHandling::DefaultCallHandling(){
	strcpy(node_type_name, "DefaultCallHandling");

}
asn1::DefaultCallHandling::~DefaultCallHandling(){

}

//CamelCapabilityHandling
asn1::CamelCapabilityHandling::CamelCapabilityHandling(){
	strcpy(node_type_name, "CamelCapabilityHandling");

}
asn1::CamelCapabilityHandling::~CamelCapabilityHandling(){

}

//SupportedCamelPhases
asn1::SupportedCamelPhases::SupportedCamelPhases(){
	strcpy(node_type_name, "SupportedCamelPhases");

}
asn1::SupportedCamelPhases::~SupportedCamelPhases(){

}

//OfferedCamel4CSIs
asn1::OfferedCamel4CSIs::OfferedCamel4CSIs(){
	strcpy(node_type_name, "OfferedCamel4CSIs");

}
asn1::OfferedCamel4CSIs::~OfferedCamel4CSIs(){

}

//OfferedCamel4Functionalities
asn1::OfferedCamel4Functionalities::OfferedCamel4Functionalities(){
	strcpy(node_type_name, "OfferedCamel4Functionalities");

}
asn1::OfferedCamel4Functionalities::~OfferedCamel4Functionalities(){

}

//SMS_CSI
asn1::SMS_CSI::SMS_CSI(){
	strcpy(node_type_name, "SMS_CSI");
	// sms_CAMEL_TDP_DataList
	_sms_CAMEL_TDP_DataList = NULL;
	children.push_back(_sms_CAMEL_TDP_DataList);

	// camelCapabilityHandling
	_camelCapabilityHandling = NULL;
	children.push_back(_camelCapabilityHandling);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// csi_Active
	_csi_Active = NULL;
	children.push_back(_csi_Active);


}
asn1::SMS_CSI::~SMS_CSI(){

}

asn1::ASN1Node* asn1::SMS_CSI::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_sms_CAMEL_TDP_DataList = new SMS_CAMEL_TDP_DataList();
			_sms_CAMEL_TDP_DataList->tlv->tag_class = CONTEXT_SPECIFIC;
			_sms_CAMEL_TDP_DataList->tlv->tag_value = 0;
			children[0] = _sms_CAMEL_TDP_DataList;
			return _sms_CAMEL_TDP_DataList;
		case 1:
			_camelCapabilityHandling = new CamelCapabilityHandling();
			_camelCapabilityHandling->tlv->tag_class = CONTEXT_SPECIFIC;
			_camelCapabilityHandling->tlv->tag_value = 1;
			children[1] = _camelCapabilityHandling;
			return _camelCapabilityHandling;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 3;
			children[3] = _notificationToCSE;
			return _notificationToCSE;
		case 4:
			_csi_Active = new Null();
			_csi_Active->tlv->tag_class = CONTEXT_SPECIFIC;
			_csi_Active->tlv->tag_value = 4;
			children[4] = _csi_Active;
			return _csi_Active;

		default: return NULL;
	}
}

void asn1::SMS_CSI::set_sms_CAMEL_TDP_DataList(){
	if(_sms_CAMEL_TDP_DataList == NULL) _sms_CAMEL_TDP_DataList = (SMS_CAMEL_TDP_DataList*)create_node(0);
}

void asn1::SMS_CSI::set_camelCapabilityHandling(){
	if(_camelCapabilityHandling == NULL) _camelCapabilityHandling = (CamelCapabilityHandling*)create_node(1);
}

void asn1::SMS_CSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::SMS_CSI::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(3);
}

void asn1::SMS_CSI::set_csi_Active(){
	if(_csi_Active == NULL) _csi_Active = (Null*)create_node(4);
}

//SMS_CAMEL_TDP_DataList
asn1::SMS_CAMEL_TDP_DataList::SMS_CAMEL_TDP_DataList(){
	strcpy(node_type_name, "SMS_CAMEL_TDP_DataList");

}
asn1::SMS_CAMEL_TDP_DataList::~SMS_CAMEL_TDP_DataList(){
}

asn1::SMS_CAMEL_TDP_Data* asn1::SMS_CAMEL_TDP_DataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (SMS_CAMEL_TDP_Data*)children[child_index]; else return NULL;
}

void asn1::SMS_CAMEL_TDP_DataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::SMS_CAMEL_TDP_DataList::create_node(unsigned int _index){
	children[_index] = new SMS_CAMEL_TDP_Data();
	return children[_index];
}

asn1::ASN1Node* asn1::SMS_CAMEL_TDP_DataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new SMS_CAMEL_TDP_Data());
		return children[children.size() - 1];
	}

}

//SMS_CAMEL_TDP_Data
asn1::SMS_CAMEL_TDP_Data::SMS_CAMEL_TDP_Data(){
	strcpy(node_type_name, "SMS_CAMEL_TDP_Data");
	// sms_TriggerDetectionPoint
	_sms_TriggerDetectionPoint = NULL;
	_sms_TriggerDetectionPoint = new SMS_TriggerDetectionPoint();
	_sms_TriggerDetectionPoint->tlv->tag_class = CONTEXT_SPECIFIC;
	_sms_TriggerDetectionPoint->tlv->tag_value = 0;
	children.push_back(_sms_TriggerDetectionPoint);

	// serviceKey
	_serviceKey = NULL;
	_serviceKey = new ServiceKey();
	_serviceKey->tlv->tag_class = CONTEXT_SPECIFIC;
	_serviceKey->tlv->tag_value = 1;
	children.push_back(_serviceKey);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	_gsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsmSCF_Address->tlv->tag_value = 2;
	children.push_back(_gsmSCF_Address);

	// defaultSMS_Handling
	_defaultSMS_Handling = NULL;
	_defaultSMS_Handling = new DefaultSMS_Handling();
	_defaultSMS_Handling->tlv->tag_class = CONTEXT_SPECIFIC;
	_defaultSMS_Handling->tlv->tag_value = 3;
	children.push_back(_defaultSMS_Handling);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SMS_CAMEL_TDP_Data::~SMS_CAMEL_TDP_Data(){

}

asn1::ASN1Node* asn1::SMS_CAMEL_TDP_Data::create_node(unsigned int _index){
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

void asn1::SMS_CAMEL_TDP_Data::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//SMS_TriggerDetectionPoint
asn1::SMS_TriggerDetectionPoint::SMS_TriggerDetectionPoint(){
	strcpy(node_type_name, "SMS_TriggerDetectionPoint");

}
asn1::SMS_TriggerDetectionPoint::~SMS_TriggerDetectionPoint(){

}

//DefaultSMS_Handling
asn1::DefaultSMS_Handling::DefaultSMS_Handling(){
	strcpy(node_type_name, "DefaultSMS_Handling");

}
asn1::DefaultSMS_Handling::~DefaultSMS_Handling(){

}

//M_CSI
asn1::M_CSI::M_CSI(){
	strcpy(node_type_name, "M_CSI");
	// mobilityTriggers
	_mobilityTriggers = NULL;
	_mobilityTriggers = new MobilityTriggers();
	children.push_back(_mobilityTriggers);

	// serviceKey
	_serviceKey = NULL;
	_serviceKey = new ServiceKey();
	children.push_back(_serviceKey);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	_gsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsmSCF_Address->tlv->tag_value = 0;
	children.push_back(_gsmSCF_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// csi_Active
	_csi_Active = NULL;
	children.push_back(_csi_Active);


}
asn1::M_CSI::~M_CSI(){

}

asn1::ASN1Node* asn1::M_CSI::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 2;
			children[4] = _notificationToCSE;
			return _notificationToCSE;
		case 5:
			_csi_Active = new Null();
			_csi_Active->tlv->tag_class = CONTEXT_SPECIFIC;
			_csi_Active->tlv->tag_value = 3;
			children[5] = _csi_Active;
			return _csi_Active;

		default: return NULL;
	}
}

void asn1::M_CSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::M_CSI::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(4);
}

void asn1::M_CSI::set_csi_Active(){
	if(_csi_Active == NULL) _csi_Active = (Null*)create_node(5);
}

//MG_CSI
asn1::MG_CSI::MG_CSI(){
	strcpy(node_type_name, "MG_CSI");
	// mobilityTriggers
	_mobilityTriggers = NULL;
	_mobilityTriggers = new MobilityTriggers();
	children.push_back(_mobilityTriggers);

	// serviceKey
	_serviceKey = NULL;
	_serviceKey = new ServiceKey();
	children.push_back(_serviceKey);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	_gsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsmSCF_Address->tlv->tag_value = 0;
	children.push_back(_gsmSCF_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// csi_Active
	_csi_Active = NULL;
	children.push_back(_csi_Active);


}
asn1::MG_CSI::~MG_CSI(){

}

asn1::ASN1Node* asn1::MG_CSI::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 2;
			children[4] = _notificationToCSE;
			return _notificationToCSE;
		case 5:
			_csi_Active = new Null();
			_csi_Active->tlv->tag_class = CONTEXT_SPECIFIC;
			_csi_Active->tlv->tag_value = 3;
			children[5] = _csi_Active;
			return _csi_Active;

		default: return NULL;
	}
}

void asn1::MG_CSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::MG_CSI::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(4);
}

void asn1::MG_CSI::set_csi_Active(){
	if(_csi_Active == NULL) _csi_Active = (Null*)create_node(5);
}

//MobilityTriggers
asn1::MobilityTriggers::MobilityTriggers(){
	strcpy(node_type_name, "MobilityTriggers");

}
asn1::MobilityTriggers::~MobilityTriggers(){
}

asn1::MM_Code* asn1::MobilityTriggers::get_child(unsigned int child_index){
	if(child_index < children.size()) return (MM_Code*)children[child_index]; else return NULL;
}

void asn1::MobilityTriggers::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::MobilityTriggers::create_node(unsigned int _index){
	children[_index] = new MM_Code();
	return children[_index];
}

asn1::ASN1Node* asn1::MobilityTriggers::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new MM_Code());
		return children[children.size() - 1];
	}

}

//maxNumOfMobilityTriggers
asn1::maxNumOfMobilityTriggers::maxNumOfMobilityTriggers(){
	strcpy(node_type_name, "maxNumOfMobilityTriggers");

}
asn1::maxNumOfMobilityTriggers::~maxNumOfMobilityTriggers(){

}

//MM_Code
asn1::MM_Code::MM_Code(){
	strcpy(node_type_name, "MM_Code");

}
asn1::MM_Code::~MM_Code(){

}

//T_CSI
asn1::T_CSI::T_CSI(){
	strcpy(node_type_name, "T_CSI");
	// t_BcsmCamelTDPDataList
	_t_BcsmCamelTDPDataList = NULL;
	_t_BcsmCamelTDPDataList = new T_BcsmCamelTDPDataList();
	children.push_back(_t_BcsmCamelTDPDataList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// camelCapabilityHandling
	_camelCapabilityHandling = NULL;
	children.push_back(_camelCapabilityHandling);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// csi_Active
	_csi_Active = NULL;
	children.push_back(_csi_Active);


}
asn1::T_CSI::~T_CSI(){

}

asn1::ASN1Node* asn1::T_CSI::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_camelCapabilityHandling = new CamelCapabilityHandling();
			_camelCapabilityHandling->tlv->tag_class = CONTEXT_SPECIFIC;
			_camelCapabilityHandling->tlv->tag_value = 0;
			children[2] = _camelCapabilityHandling;
			return _camelCapabilityHandling;
		case 3:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 1;
			children[3] = _notificationToCSE;
			return _notificationToCSE;
		case 4:
			_csi_Active = new Null();
			_csi_Active->tlv->tag_class = CONTEXT_SPECIFIC;
			_csi_Active->tlv->tag_value = 2;
			children[4] = _csi_Active;
			return _csi_Active;

		default: return NULL;
	}
}

void asn1::T_CSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::T_CSI::set_camelCapabilityHandling(){
	if(_camelCapabilityHandling == NULL) _camelCapabilityHandling = (CamelCapabilityHandling*)create_node(2);
}

void asn1::T_CSI::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(3);
}

void asn1::T_CSI::set_csi_Active(){
	if(_csi_Active == NULL) _csi_Active = (Null*)create_node(4);
}

//T_BcsmCamelTDPDataList
asn1::T_BcsmCamelTDPDataList::T_BcsmCamelTDPDataList(){
	strcpy(node_type_name, "T_BcsmCamelTDPDataList");

}
asn1::T_BcsmCamelTDPDataList::~T_BcsmCamelTDPDataList(){
}

asn1::T_BcsmCamelTDPData* asn1::T_BcsmCamelTDPDataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (T_BcsmCamelTDPData*)children[child_index]; else return NULL;
}

void asn1::T_BcsmCamelTDPDataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::T_BcsmCamelTDPDataList::create_node(unsigned int _index){
	children[_index] = new T_BcsmCamelTDPData();
	return children[_index];
}

asn1::ASN1Node* asn1::T_BcsmCamelTDPDataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new T_BcsmCamelTDPData());
		return children[children.size() - 1];
	}

}

//T_BcsmCamelTDPData
asn1::T_BcsmCamelTDPData::T_BcsmCamelTDPData(){
	strcpy(node_type_name, "T_BcsmCamelTDPData");
	// t_BcsmTriggerDetectionPoint
	_t_BcsmTriggerDetectionPoint = NULL;
	_t_BcsmTriggerDetectionPoint = new T_BcsmTriggerDetectionPoint();
	children.push_back(_t_BcsmTriggerDetectionPoint);

	// serviceKey
	_serviceKey = NULL;
	_serviceKey = new ServiceKey();
	children.push_back(_serviceKey);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	_gsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsmSCF_Address->tlv->tag_value = 0;
	children.push_back(_gsmSCF_Address);

	// defaultCallHandling
	_defaultCallHandling = NULL;
	_defaultCallHandling = new DefaultCallHandling();
	_defaultCallHandling->tlv->tag_class = CONTEXT_SPECIFIC;
	_defaultCallHandling->tlv->tag_value = 1;
	children.push_back(_defaultCallHandling);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::T_BcsmCamelTDPData::~T_BcsmCamelTDPData(){

}

asn1::ASN1Node* asn1::T_BcsmCamelTDPData::create_node(unsigned int _index){
	switch(_index){
		case 4:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::T_BcsmCamelTDPData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//T_BcsmTriggerDetectionPoint
asn1::T_BcsmTriggerDetectionPoint::T_BcsmTriggerDetectionPoint(){
	strcpy(node_type_name, "T_BcsmTriggerDetectionPoint");

}
asn1::T_BcsmTriggerDetectionPoint::~T_BcsmTriggerDetectionPoint(){

}

//SendRoutingInfoForGprsArg
asn1::SendRoutingInfoForGprsArg::SendRoutingInfoForGprsArg(){
	strcpy(node_type_name, "SendRoutingInfoForGprsArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// ggsn_Address
	_ggsn_Address = NULL;
	children.push_back(_ggsn_Address);

	// ggsn_Number
	_ggsn_Number = NULL;
	_ggsn_Number = new ISDN_AddressString();
	_ggsn_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_ggsn_Number->tlv->tag_value = 2;
	children.push_back(_ggsn_Number);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SendRoutingInfoForGprsArg::~SendRoutingInfoForGprsArg(){

}

asn1::ASN1Node* asn1::SendRoutingInfoForGprsArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_ggsn_Address = new GSN_Address();
			_ggsn_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_Address->tlv->tag_value = 1;
			children[1] = _ggsn_Address;
			return _ggsn_Address;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SendRoutingInfoForGprsArg::set_ggsn_Address(){
	if(_ggsn_Address == NULL) _ggsn_Address = (GSN_Address*)create_node(1);
}

void asn1::SendRoutingInfoForGprsArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//SendRoutingInfoForGprsRes
asn1::SendRoutingInfoForGprsRes::SendRoutingInfoForGprsRes(){
	strcpy(node_type_name, "SendRoutingInfoForGprsRes");
	// sgsn_Address
	_sgsn_Address = NULL;
	_sgsn_Address = new GSN_Address();
	_sgsn_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_sgsn_Address->tlv->tag_value = 0;
	children.push_back(_sgsn_Address);

	// ggsn_Address
	_ggsn_Address = NULL;
	children.push_back(_ggsn_Address);

	// mobileNotReachableReason
	_mobileNotReachableReason = NULL;
	children.push_back(_mobileNotReachableReason);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SendRoutingInfoForGprsRes::~SendRoutingInfoForGprsRes(){

}

asn1::ASN1Node* asn1::SendRoutingInfoForGprsRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_ggsn_Address = new GSN_Address();
			_ggsn_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_Address->tlv->tag_value = 1;
			children[1] = _ggsn_Address;
			return _ggsn_Address;
		case 2:
			_mobileNotReachableReason = new AbsentSubscriberDiagnosticSM();
			_mobileNotReachableReason->tlv->tag_class = CONTEXT_SPECIFIC;
			_mobileNotReachableReason->tlv->tag_value = 2;
			children[2] = _mobileNotReachableReason;
			return _mobileNotReachableReason;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SendRoutingInfoForGprsRes::set_ggsn_Address(){
	if(_ggsn_Address == NULL) _ggsn_Address = (GSN_Address*)create_node(1);
}

void asn1::SendRoutingInfoForGprsRes::set_mobileNotReachableReason(){
	if(_mobileNotReachableReason == NULL) _mobileNotReachableReason = (AbsentSubscriberDiagnosticSM*)create_node(2);
}

void asn1::SendRoutingInfoForGprsRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//FailureReportArg
asn1::FailureReportArg::FailureReportArg(){
	strcpy(node_type_name, "FailureReportArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// ggsn_Number
	_ggsn_Number = NULL;
	_ggsn_Number = new ISDN_AddressString();
	_ggsn_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_ggsn_Number->tlv->tag_value = 1;
	children.push_back(_ggsn_Number);

	// ggsn_Address
	_ggsn_Address = NULL;
	children.push_back(_ggsn_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::FailureReportArg::~FailureReportArg(){

}

asn1::ASN1Node* asn1::FailureReportArg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_ggsn_Address = new GSN_Address();
			_ggsn_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_Address->tlv->tag_value = 2;
			children[2] = _ggsn_Address;
			return _ggsn_Address;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::FailureReportArg::set_ggsn_Address(){
	if(_ggsn_Address == NULL) _ggsn_Address = (GSN_Address*)create_node(2);
}

void asn1::FailureReportArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//FailureReportRes
asn1::FailureReportRes::FailureReportRes(){
	strcpy(node_type_name, "FailureReportRes");
	// ggsn_Address
	_ggsn_Address = NULL;
	children.push_back(_ggsn_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::FailureReportRes::~FailureReportRes(){

}

asn1::ASN1Node* asn1::FailureReportRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ggsn_Address = new GSN_Address();
			_ggsn_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_Address->tlv->tag_value = 0;
			children[0] = _ggsn_Address;
			return _ggsn_Address;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::FailureReportRes::set_ggsn_Address(){
	if(_ggsn_Address == NULL) _ggsn_Address = (GSN_Address*)create_node(0);
}

void asn1::FailureReportRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//NoteMsPresentForGprsArg
asn1::NoteMsPresentForGprsArg::NoteMsPresentForGprsArg(){
	strcpy(node_type_name, "NoteMsPresentForGprsArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// sgsn_Address
	_sgsn_Address = NULL;
	_sgsn_Address = new GSN_Address();
	_sgsn_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_sgsn_Address->tlv->tag_value = 1;
	children.push_back(_sgsn_Address);

	// ggsn_Address
	_ggsn_Address = NULL;
	children.push_back(_ggsn_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::NoteMsPresentForGprsArg::~NoteMsPresentForGprsArg(){

}

asn1::ASN1Node* asn1::NoteMsPresentForGprsArg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_ggsn_Address = new GSN_Address();
			_ggsn_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_Address->tlv->tag_value = 2;
			children[2] = _ggsn_Address;
			return _ggsn_Address;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::NoteMsPresentForGprsArg::set_ggsn_Address(){
	if(_ggsn_Address == NULL) _ggsn_Address = (GSN_Address*)create_node(2);
}

void asn1::NoteMsPresentForGprsArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//NoteMsPresentForGprsRes
asn1::NoteMsPresentForGprsRes::NoteMsPresentForGprsRes(){
	strcpy(node_type_name, "NoteMsPresentForGprsRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::NoteMsPresentForGprsRes::~NoteMsPresentForGprsRes(){

}

asn1::ASN1Node* asn1::NoteMsPresentForGprsRes::create_node(unsigned int _index){
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

void asn1::NoteMsPresentForGprsRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//ResetArg
asn1::ResetArg::ResetArg(){
	strcpy(node_type_name, "ResetArg");
	// sendingNodenumber
	_sendingNodenumber = NULL;
	_sendingNodenumber = new SendingNode_Number();
	children.push_back(_sendingNodenumber);

	// hlr_List
	_hlr_List = NULL;
	children.push_back(_hlr_List);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ResetArg::~ResetArg(){

}

asn1::ASN1Node* asn1::ResetArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_hlr_List = new HLR_List();
			children[1] = _hlr_List;
			return _hlr_List;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ResetArg::set_hlr_List(){
	if(_hlr_List == NULL) _hlr_List = (HLR_List*)create_node(1);
}

void asn1::ResetArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//SendingNode_Number
asn1::SendingNode_Number::SendingNode_Number(){
	strcpy(node_type_name, "SendingNode_Number");
	// hlr_Number
	_hlr_Number = NULL;
	_hlr_Number = new ISDN_AddressString();
	children.push_back(_hlr_Number);

	// css_Number
	_css_Number = NULL;
	_css_Number = new ISDN_AddressString();
	_css_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_css_Number->tlv->tag_value = 1;
	children.push_back(_css_Number);


}
asn1::SendingNode_Number::~SendingNode_Number(){

}

//RestoreDataArg
asn1::RestoreDataArg::RestoreDataArg(){
	strcpy(node_type_name, "RestoreDataArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// lmsi
	_lmsi = NULL;
	children.push_back(_lmsi);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// vlr_Capability
	_vlr_Capability = NULL;
	children.push_back(_vlr_Capability);

	// restorationIndicator
	_restorationIndicator = NULL;
	children.push_back(_restorationIndicator);


}
asn1::RestoreDataArg::~RestoreDataArg(){

}

asn1::ASN1Node* asn1::RestoreDataArg::create_node(unsigned int _index){
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
			_vlr_Capability = new VLR_Capability();
			_vlr_Capability->tlv->tag_class = CONTEXT_SPECIFIC;
			_vlr_Capability->tlv->tag_value = 6;
			children[3] = _vlr_Capability;
			return _vlr_Capability;
		case 4:
			_restorationIndicator = new Null();
			_restorationIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_restorationIndicator->tlv->tag_value = 7;
			children[4] = _restorationIndicator;
			return _restorationIndicator;

		default: return NULL;
	}
}

void asn1::RestoreDataArg::set_lmsi(){
	if(_lmsi == NULL) _lmsi = (LMSI*)create_node(1);
}

void asn1::RestoreDataArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::RestoreDataArg::set_vlr_Capability(){
	if(_vlr_Capability == NULL) _vlr_Capability = (VLR_Capability*)create_node(3);
}

void asn1::RestoreDataArg::set_restorationIndicator(){
	if(_restorationIndicator == NULL) _restorationIndicator = (Null*)create_node(4);
}

//RestoreDataRes
asn1::RestoreDataRes::RestoreDataRes(){
	strcpy(node_type_name, "RestoreDataRes");
	// hlr_Number
	_hlr_Number = NULL;
	_hlr_Number = new ISDN_AddressString();
	children.push_back(_hlr_Number);

	// msNotReachable
	_msNotReachable = NULL;
	children.push_back(_msNotReachable);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::RestoreDataRes::~RestoreDataRes(){

}

asn1::ASN1Node* asn1::RestoreDataRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_msNotReachable = new Null();
			children[1] = _msNotReachable;
			return _msNotReachable;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::RestoreDataRes::set_msNotReachable(){
	if(_msNotReachable == NULL) _msNotReachable = (Null*)create_node(1);
}

void asn1::RestoreDataRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//VBSDataList
asn1::VBSDataList::VBSDataList(){
	strcpy(node_type_name, "VBSDataList");

}
asn1::VBSDataList::~VBSDataList(){
}

asn1::VoiceBroadcastData* asn1::VBSDataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (VoiceBroadcastData*)children[child_index]; else return NULL;
}

void asn1::VBSDataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::VBSDataList::create_node(unsigned int _index){
	children[_index] = new VoiceBroadcastData();
	return children[_index];
}

asn1::ASN1Node* asn1::VBSDataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new VoiceBroadcastData());
		return children[children.size() - 1];
	}

}

//VGCSDataList
asn1::VGCSDataList::VGCSDataList(){
	strcpy(node_type_name, "VGCSDataList");

}
asn1::VGCSDataList::~VGCSDataList(){
}

asn1::VoiceGroupCallData* asn1::VGCSDataList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (VoiceGroupCallData*)children[child_index]; else return NULL;
}

void asn1::VGCSDataList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::VGCSDataList::create_node(unsigned int _index){
	children[_index] = new VoiceGroupCallData();
	return children[_index];
}

asn1::ASN1Node* asn1::VGCSDataList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new VoiceGroupCallData());
		return children[children.size() - 1];
	}

}

//maxNumOfVBSGroupIds
asn1::maxNumOfVBSGroupIds::maxNumOfVBSGroupIds(){
	strcpy(node_type_name, "maxNumOfVBSGroupIds");

}
asn1::maxNumOfVBSGroupIds::~maxNumOfVBSGroupIds(){

}

//maxNumOfVGCSGroupIds
asn1::maxNumOfVGCSGroupIds::maxNumOfVGCSGroupIds(){
	strcpy(node_type_name, "maxNumOfVGCSGroupIds");

}
asn1::maxNumOfVGCSGroupIds::~maxNumOfVGCSGroupIds(){

}

//VoiceGroupCallData
asn1::VoiceGroupCallData::VoiceGroupCallData(){
	strcpy(node_type_name, "VoiceGroupCallData");
	// groupId
	_groupId = NULL;
	_groupId = new GroupId();
	children.push_back(_groupId);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// additionalSubscriptions
	_additionalSubscriptions = NULL;
	children.push_back(_additionalSubscriptions);

	// additionalInfo
	_additionalInfo = NULL;
	children.push_back(_additionalInfo);

	// longGroupId
	_longGroupId = NULL;
	children.push_back(_longGroupId);


}
asn1::VoiceGroupCallData::~VoiceGroupCallData(){

}

asn1::ASN1Node* asn1::VoiceGroupCallData::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_additionalSubscriptions = new AdditionalSubscriptions();
			children[2] = _additionalSubscriptions;
			return _additionalSubscriptions;
		case 3:
			_additionalInfo = new AdditionalInfo();
			_additionalInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalInfo->tlv->tag_value = 0;
			children[3] = _additionalInfo;
			return _additionalInfo;
		case 4:
			_longGroupId = new Long_GroupId();
			_longGroupId->tlv->tag_class = CONTEXT_SPECIFIC;
			_longGroupId->tlv->tag_value = 1;
			children[4] = _longGroupId;
			return _longGroupId;

		default: return NULL;
	}
}

void asn1::VoiceGroupCallData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::VoiceGroupCallData::set_additionalSubscriptions(){
	if(_additionalSubscriptions == NULL) _additionalSubscriptions = (AdditionalSubscriptions*)create_node(2);
}

void asn1::VoiceGroupCallData::set_additionalInfo(){
	if(_additionalInfo == NULL) _additionalInfo = (AdditionalInfo*)create_node(3);
}

void asn1::VoiceGroupCallData::set_longGroupId(){
	if(_longGroupId == NULL) _longGroupId = (Long_GroupId*)create_node(4);
}

//AdditionalInfo
asn1::AdditionalInfo::AdditionalInfo(){
	strcpy(node_type_name, "AdditionalInfo");

}
asn1::AdditionalInfo::~AdditionalInfo(){

}

//AdditionalSubscriptions
asn1::AdditionalSubscriptions::AdditionalSubscriptions(){
	strcpy(node_type_name, "AdditionalSubscriptions");

}
asn1::AdditionalSubscriptions::~AdditionalSubscriptions(){

}

//VoiceBroadcastData
asn1::VoiceBroadcastData::VoiceBroadcastData(){
	strcpy(node_type_name, "VoiceBroadcastData");
	// groupid
	_groupid = NULL;
	_groupid = new GroupId();
	children.push_back(_groupid);

	// broadcastInitEntitlement
	_broadcastInitEntitlement = NULL;
	children.push_back(_broadcastInitEntitlement);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// longGroupId
	_longGroupId = NULL;
	children.push_back(_longGroupId);


}
asn1::VoiceBroadcastData::~VoiceBroadcastData(){

}

asn1::ASN1Node* asn1::VoiceBroadcastData::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_broadcastInitEntitlement = new Null();
			children[1] = _broadcastInitEntitlement;
			return _broadcastInitEntitlement;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_longGroupId = new Long_GroupId();
			_longGroupId->tlv->tag_class = CONTEXT_SPECIFIC;
			_longGroupId->tlv->tag_value = 0;
			children[3] = _longGroupId;
			return _longGroupId;

		default: return NULL;
	}
}

void asn1::VoiceBroadcastData::set_broadcastInitEntitlement(){
	if(_broadcastInitEntitlement == NULL) _broadcastInitEntitlement = (Null*)create_node(1);
}

void asn1::VoiceBroadcastData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::VoiceBroadcastData::set_longGroupId(){
	if(_longGroupId == NULL) _longGroupId = (Long_GroupId*)create_node(3);
}

//GroupId
asn1::GroupId::GroupId(){
	strcpy(node_type_name, "GroupId");

}
asn1::GroupId::~GroupId(){

}

//Long_GroupId
asn1::Long_GroupId::Long_GroupId(){
	strcpy(node_type_name, "Long_GroupId");

}
asn1::Long_GroupId::~Long_GroupId(){

}

//ProvideSubscriberInfoArg
asn1::ProvideSubscriberInfoArg::ProvideSubscriberInfoArg(){
	strcpy(node_type_name, "ProvideSubscriberInfoArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 0;
	children.push_back(_imsi);

	// lmsi
	_lmsi = NULL;
	children.push_back(_lmsi);

	// requestedInfo
	_requestedInfo = NULL;
	_requestedInfo = new RequestedInfo();
	_requestedInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_requestedInfo->tlv->tag_value = 2;
	children.push_back(_requestedInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// callPriority
	_callPriority = NULL;
	children.push_back(_callPriority);


}
asn1::ProvideSubscriberInfoArg::~ProvideSubscriberInfoArg(){

}

asn1::ASN1Node* asn1::ProvideSubscriberInfoArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_lmsi = new LMSI();
			_lmsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_lmsi->tlv->tag_value = 1;
			children[1] = _lmsi;
			return _lmsi;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_callPriority = new EMLPP_Priority();
			_callPriority->tlv->tag_class = CONTEXT_SPECIFIC;
			_callPriority->tlv->tag_value = 4;
			children[4] = _callPriority;
			return _callPriority;

		default: return NULL;
	}
}

void asn1::ProvideSubscriberInfoArg::set_lmsi(){
	if(_lmsi == NULL) _lmsi = (LMSI*)create_node(1);
}

void asn1::ProvideSubscriberInfoArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::ProvideSubscriberInfoArg::set_callPriority(){
	if(_callPriority == NULL) _callPriority = (EMLPP_Priority*)create_node(4);
}

//ProvideSubscriberInfoRes
asn1::ProvideSubscriberInfoRes::ProvideSubscriberInfoRes(){
	strcpy(node_type_name, "ProvideSubscriberInfoRes");
	// subscriberInfo
	_subscriberInfo = NULL;
	_subscriberInfo = new SubscriberInfo();
	children.push_back(_subscriberInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ProvideSubscriberInfoRes::~ProvideSubscriberInfoRes(){

}

asn1::ASN1Node* asn1::ProvideSubscriberInfoRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ProvideSubscriberInfoRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//SubscriberInfo
asn1::SubscriberInfo::SubscriberInfo(){
	strcpy(node_type_name, "SubscriberInfo");
	// locationInformation
	_locationInformation = NULL;
	children.push_back(_locationInformation);

	// subscriberState
	_subscriberState = NULL;
	children.push_back(_subscriberState);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// locationInformationGPRS
	_locationInformationGPRS = NULL;
	children.push_back(_locationInformationGPRS);

	// ps_SubscriberState
	_ps_SubscriberState = NULL;
	children.push_back(_ps_SubscriberState);

	// imei
	_imei = NULL;
	children.push_back(_imei);

	// ms_Classmark2
	_ms_Classmark2 = NULL;
	children.push_back(_ms_Classmark2);

	// gprs_MS_Class
	_gprs_MS_Class = NULL;
	children.push_back(_gprs_MS_Class);

	// mnpInfoRes
	_mnpInfoRes = NULL;
	children.push_back(_mnpInfoRes);

	// imsVoiceOverPS_SessionsIndication
	_imsVoiceOverPS_SessionsIndication = NULL;
	children.push_back(_imsVoiceOverPS_SessionsIndication);

	// lastUE_ActivityTime
	_lastUE_ActivityTime = NULL;
	children.push_back(_lastUE_ActivityTime);

	// lastRAT_Type
	_lastRAT_Type = NULL;
	children.push_back(_lastRAT_Type);

	// eps_SubscriberState
	_eps_SubscriberState = NULL;
	children.push_back(_eps_SubscriberState);

	// locationInformationEPS
	_locationInformationEPS = NULL;
	children.push_back(_locationInformationEPS);

	// timeZone
	_timeZone = NULL;
	children.push_back(_timeZone);

	// daylightSavingTime
	_daylightSavingTime = NULL;
	children.push_back(_daylightSavingTime);


}
asn1::SubscriberInfo::~SubscriberInfo(){

}

asn1::ASN1Node* asn1::SubscriberInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_locationInformation = new LocationInformation();
			_locationInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationInformation->tlv->tag_value = 0;
			children[0] = _locationInformation;
			return _locationInformation;
                case 1:
                {
                        _subscriberState = new SubscriberState();
                        ASN1Node* _subscriberState_wrapper = new ASN1Node();
                        _subscriberState_wrapper->tlv = new TLVNode();
                        _subscriberState_wrapper->tlv->is_explicit = true;
                        _subscriberState_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _subscriberState_wrapper->tlv->tag_value = 1;
                        _subscriberState_wrapper->children.push_back(_subscriberState);
                        children[1] = _subscriberState_wrapper;
                        return _subscriberState;
                }
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_locationInformationGPRS = new LocationInformationGPRS();
			_locationInformationGPRS->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationInformationGPRS->tlv->tag_value = 3;
			children[3] = _locationInformationGPRS;
			return _locationInformationGPRS;
                case 4:
                {
                        _ps_SubscriberState = new PS_SubscriberState();
                        ASN1Node* _ps_SubscriberState_wrapper = new ASN1Node();
                        _ps_SubscriberState_wrapper->tlv = new TLVNode();
                        _ps_SubscriberState_wrapper->tlv->is_explicit = true;
                        _ps_SubscriberState_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _ps_SubscriberState_wrapper->tlv->tag_value = 4;
                        _ps_SubscriberState_wrapper->children.push_back(_ps_SubscriberState);
                        children[4] = _ps_SubscriberState_wrapper;
                        return _ps_SubscriberState;
                }
		case 5:
			_imei = new IMEI();
			_imei->tlv->tag_class = CONTEXT_SPECIFIC;
			_imei->tlv->tag_value = 5;
			children[5] = _imei;
			return _imei;
		case 6:
			_ms_Classmark2 = new MS_Classmark2();
			_ms_Classmark2->tlv->tag_class = CONTEXT_SPECIFIC;
			_ms_Classmark2->tlv->tag_value = 6;
			children[6] = _ms_Classmark2;
			return _ms_Classmark2;
		case 7:
			_gprs_MS_Class = new GPRSMSClass();
			_gprs_MS_Class->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprs_MS_Class->tlv->tag_value = 7;
			children[7] = _gprs_MS_Class;
			return _gprs_MS_Class;
		case 8:
			_mnpInfoRes = new MNPInfoRes();
			_mnpInfoRes->tlv->tag_class = CONTEXT_SPECIFIC;
			_mnpInfoRes->tlv->tag_value = 8;
			children[8] = _mnpInfoRes;
			return _mnpInfoRes;
		case 9:
			_imsVoiceOverPS_SessionsIndication = new IMS_VoiceOverPS_SessionsInd();
			_imsVoiceOverPS_SessionsIndication->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsVoiceOverPS_SessionsIndication->tlv->tag_value = 9;
			children[9] = _imsVoiceOverPS_SessionsIndication;
			return _imsVoiceOverPS_SessionsIndication;
		case 10:
			_lastUE_ActivityTime = new Time();
			_lastUE_ActivityTime->tlv->tag_class = CONTEXT_SPECIFIC;
			_lastUE_ActivityTime->tlv->tag_value = 10;
			children[10] = _lastUE_ActivityTime;
			return _lastUE_ActivityTime;
		case 11:
			_lastRAT_Type = new Used_RAT_Type();
			_lastRAT_Type->tlv->tag_class = CONTEXT_SPECIFIC;
			_lastRAT_Type->tlv->tag_value = 11;
			children[11] = _lastRAT_Type;
			return _lastRAT_Type;
                case 12:
                {
                        _eps_SubscriberState = new PS_SubscriberState();
                        ASN1Node* _eps_SubscriberState_wrapper = new ASN1Node();
                        _eps_SubscriberState_wrapper->tlv = new TLVNode();
                        _eps_SubscriberState_wrapper->tlv->is_explicit = true;
                        _eps_SubscriberState_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _eps_SubscriberState_wrapper->tlv->tag_value = 12;
                        _eps_SubscriberState_wrapper->children.push_back(_eps_SubscriberState);
                        children[12] = _eps_SubscriberState_wrapper;
                        return _eps_SubscriberState;
                }
		case 13:
			_locationInformationEPS = new LocationInformationEPS();
			_locationInformationEPS->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationInformationEPS->tlv->tag_value = 13;
			children[13] = _locationInformationEPS;
			return _locationInformationEPS;
		case 14:
			_timeZone = new TimeZone();
			_timeZone->tlv->tag_class = CONTEXT_SPECIFIC;
			_timeZone->tlv->tag_value = 14;
			children[14] = _timeZone;
			return _timeZone;
		case 15:
			_daylightSavingTime = new DaylightSavingTime();
			_daylightSavingTime->tlv->tag_class = CONTEXT_SPECIFIC;
			_daylightSavingTime->tlv->tag_value = 15;
			children[15] = _daylightSavingTime;
			return _daylightSavingTime;

		default: return NULL;
	}
}

void asn1::SubscriberInfo::set_locationInformation(){
	if(_locationInformation == NULL) _locationInformation = (LocationInformation*)create_node(0);
}

void asn1::SubscriberInfo::set_subscriberState(){
	if(_subscriberState == NULL) _subscriberState = (SubscriberState*)create_node(1);
}

void asn1::SubscriberInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::SubscriberInfo::set_locationInformationGPRS(){
	if(_locationInformationGPRS == NULL) _locationInformationGPRS = (LocationInformationGPRS*)create_node(3);
}

void asn1::SubscriberInfo::set_ps_SubscriberState(){
	if(_ps_SubscriberState == NULL) _ps_SubscriberState = (PS_SubscriberState*)create_node(4);
}

void asn1::SubscriberInfo::set_imei(){
	if(_imei == NULL) _imei = (IMEI*)create_node(5);
}

void asn1::SubscriberInfo::set_ms_Classmark2(){
	if(_ms_Classmark2 == NULL) _ms_Classmark2 = (MS_Classmark2*)create_node(6);
}

void asn1::SubscriberInfo::set_gprs_MS_Class(){
	if(_gprs_MS_Class == NULL) _gprs_MS_Class = (GPRSMSClass*)create_node(7);
}

void asn1::SubscriberInfo::set_mnpInfoRes(){
	if(_mnpInfoRes == NULL) _mnpInfoRes = (MNPInfoRes*)create_node(8);
}

void asn1::SubscriberInfo::set_imsVoiceOverPS_SessionsIndication(){
	if(_imsVoiceOverPS_SessionsIndication == NULL) _imsVoiceOverPS_SessionsIndication = (IMS_VoiceOverPS_SessionsInd*)create_node(9);
}

void asn1::SubscriberInfo::set_lastUE_ActivityTime(){
	if(_lastUE_ActivityTime == NULL) _lastUE_ActivityTime = (Time*)create_node(10);
}

void asn1::SubscriberInfo::set_lastRAT_Type(){
	if(_lastRAT_Type == NULL) _lastRAT_Type = (Used_RAT_Type*)create_node(11);
}

void asn1::SubscriberInfo::set_eps_SubscriberState(){
	if(_eps_SubscriberState == NULL) _eps_SubscriberState = (PS_SubscriberState*)create_node(12);
}

void asn1::SubscriberInfo::set_locationInformationEPS(){
	if(_locationInformationEPS == NULL) _locationInformationEPS = (LocationInformationEPS*)create_node(13);
}

void asn1::SubscriberInfo::set_timeZone(){
	if(_timeZone == NULL) _timeZone = (TimeZone*)create_node(14);
}

void asn1::SubscriberInfo::set_daylightSavingTime(){
	if(_daylightSavingTime == NULL) _daylightSavingTime = (DaylightSavingTime*)create_node(15);
}

//IMS_VoiceOverPS_SessionsInd
asn1::IMS_VoiceOverPS_SessionsInd::IMS_VoiceOverPS_SessionsInd(){
	strcpy(node_type_name, "IMS_VoiceOverPS_SessionsInd");

}
asn1::IMS_VoiceOverPS_SessionsInd::~IMS_VoiceOverPS_SessionsInd(){

}

//TimeZone
asn1::TimeZone::TimeZone(){
	strcpy(node_type_name, "TimeZone");

}
asn1::TimeZone::~TimeZone(){

}

//DaylightSavingTime
asn1::DaylightSavingTime::DaylightSavingTime(){
	strcpy(node_type_name, "DaylightSavingTime");

}
asn1::DaylightSavingTime::~DaylightSavingTime(){

}

//MNPInfoRes
asn1::MNPInfoRes::MNPInfoRes(){
	strcpy(node_type_name, "MNPInfoRes");
	// routeingNumber
	_routeingNumber = NULL;
	children.push_back(_routeingNumber);

	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// msisdn
	_msisdn = NULL;
	children.push_back(_msisdn);

	// numberPortabilityStatus
	_numberPortabilityStatus = NULL;
	children.push_back(_numberPortabilityStatus);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MNPInfoRes::~MNPInfoRes(){

}

asn1::ASN1Node* asn1::MNPInfoRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_routeingNumber = new RouteingNumber();
			_routeingNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_routeingNumber->tlv->tag_value = 0;
			children[0] = _routeingNumber;
			return _routeingNumber;
		case 1:
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 1;
			children[1] = _imsi;
			return _imsi;
		case 2:
			_msisdn = new ISDN_AddressString();
			_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn->tlv->tag_value = 2;
			children[2] = _msisdn;
			return _msisdn;
		case 3:
			_numberPortabilityStatus = new NumberPortabilityStatus();
			_numberPortabilityStatus->tlv->tag_class = CONTEXT_SPECIFIC;
			_numberPortabilityStatus->tlv->tag_value = 3;
			children[3] = _numberPortabilityStatus;
			return _numberPortabilityStatus;
		case 4:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 4;
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::MNPInfoRes::set_routeingNumber(){
	if(_routeingNumber == NULL) _routeingNumber = (RouteingNumber*)create_node(0);
}

void asn1::MNPInfoRes::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(1);
}

void asn1::MNPInfoRes::set_msisdn(){
	if(_msisdn == NULL) _msisdn = (ISDN_AddressString*)create_node(2);
}

void asn1::MNPInfoRes::set_numberPortabilityStatus(){
	if(_numberPortabilityStatus == NULL) _numberPortabilityStatus = (NumberPortabilityStatus*)create_node(3);
}

void asn1::MNPInfoRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//RouteingNumber
asn1::RouteingNumber::RouteingNumber(){
	strcpy(node_type_name, "RouteingNumber");

}
asn1::RouteingNumber::~RouteingNumber(){

}

//NumberPortabilityStatus
asn1::NumberPortabilityStatus::NumberPortabilityStatus(){
	strcpy(node_type_name, "NumberPortabilityStatus");

}
asn1::NumberPortabilityStatus::~NumberPortabilityStatus(){

}

//MS_Classmark2
asn1::MS_Classmark2::MS_Classmark2(){
	strcpy(node_type_name, "MS_Classmark2");

}
asn1::MS_Classmark2::~MS_Classmark2(){

}

//GPRSMSClass
asn1::GPRSMSClass::GPRSMSClass(){
	strcpy(node_type_name, "GPRSMSClass");
	// mSNetworkCapability
	_mSNetworkCapability = NULL;
	_mSNetworkCapability = new MSNetworkCapability();
	_mSNetworkCapability->tlv->tag_class = CONTEXT_SPECIFIC;
	_mSNetworkCapability->tlv->tag_value = 0;
	children.push_back(_mSNetworkCapability);

	// mSRadioAccessCapability
	_mSRadioAccessCapability = NULL;
	children.push_back(_mSRadioAccessCapability);


}
asn1::GPRSMSClass::~GPRSMSClass(){

}

asn1::ASN1Node* asn1::GPRSMSClass::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_mSRadioAccessCapability = new MSRadioAccessCapability();
			_mSRadioAccessCapability->tlv->tag_class = CONTEXT_SPECIFIC;
			_mSRadioAccessCapability->tlv->tag_value = 1;
			children[1] = _mSRadioAccessCapability;
			return _mSRadioAccessCapability;

		default: return NULL;
	}
}

void asn1::GPRSMSClass::set_mSRadioAccessCapability(){
	if(_mSRadioAccessCapability == NULL) _mSRadioAccessCapability = (MSRadioAccessCapability*)create_node(1);
}

//MSNetworkCapability
asn1::MSNetworkCapability::MSNetworkCapability(){
	strcpy(node_type_name, "MSNetworkCapability");

}
asn1::MSNetworkCapability::~MSNetworkCapability(){

}

//MSRadioAccessCapability
asn1::MSRadioAccessCapability::MSRadioAccessCapability(){
	strcpy(node_type_name, "MSRadioAccessCapability");

}
asn1::MSRadioAccessCapability::~MSRadioAccessCapability(){

}

//RequestedInfo
asn1::RequestedInfo::RequestedInfo(){
	strcpy(node_type_name, "RequestedInfo");
	// locationInformation
	_locationInformation = NULL;
	children.push_back(_locationInformation);

	// subscriberState
	_subscriberState = NULL;
	children.push_back(_subscriberState);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// currentLocation
	_currentLocation = NULL;
	children.push_back(_currentLocation);

	// requestedDomain
	_requestedDomain = NULL;
	children.push_back(_requestedDomain);

	// imei
	_imei = NULL;
	children.push_back(_imei);

	// ms_classmark
	_ms_classmark = NULL;
	children.push_back(_ms_classmark);

	// mnpRequestedInfo
	_mnpRequestedInfo = NULL;
	children.push_back(_mnpRequestedInfo);

	// locationInformationEPS_Supported
	_locationInformationEPS_Supported = NULL;
	children.push_back(_locationInformationEPS_Supported);

	// t_adsData
	_t_adsData = NULL;
	children.push_back(_t_adsData);

	// requestedNodes
	_requestedNodes = NULL;
	children.push_back(_requestedNodes);

	// servingNodeIndication
	_servingNodeIndication = NULL;
	children.push_back(_servingNodeIndication);

	// localTimeZoneRequest
	_localTimeZoneRequest = NULL;
	children.push_back(_localTimeZoneRequest);


}
asn1::RequestedInfo::~RequestedInfo(){

}

asn1::ASN1Node* asn1::RequestedInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_locationInformation = new Null();
			_locationInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationInformation->tlv->tag_value = 0;
			children[0] = _locationInformation;
			return _locationInformation;
		case 1:
			_subscriberState = new Null();
			_subscriberState->tlv->tag_class = CONTEXT_SPECIFIC;
			_subscriberState->tlv->tag_value = 1;
			children[1] = _subscriberState;
			return _subscriberState;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_currentLocation = new Null();
			_currentLocation->tlv->tag_class = CONTEXT_SPECIFIC;
			_currentLocation->tlv->tag_value = 3;
			children[3] = _currentLocation;
			return _currentLocation;
		case 4:
			_requestedDomain = new DomainType();
			_requestedDomain->tlv->tag_class = CONTEXT_SPECIFIC;
			_requestedDomain->tlv->tag_value = 4;
			children[4] = _requestedDomain;
			return _requestedDomain;
		case 5:
			_imei = new Null();
			_imei->tlv->tag_class = CONTEXT_SPECIFIC;
			_imei->tlv->tag_value = 6;
			children[5] = _imei;
			return _imei;
		case 6:
			_ms_classmark = new Null();
			_ms_classmark->tlv->tag_class = CONTEXT_SPECIFIC;
			_ms_classmark->tlv->tag_value = 5;
			children[6] = _ms_classmark;
			return _ms_classmark;
		case 7:
			_mnpRequestedInfo = new Null();
			_mnpRequestedInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_mnpRequestedInfo->tlv->tag_value = 7;
			children[7] = _mnpRequestedInfo;
			return _mnpRequestedInfo;
		case 8:
			_locationInformationEPS_Supported = new Null();
			_locationInformationEPS_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationInformationEPS_Supported->tlv->tag_value = 11;
			children[8] = _locationInformationEPS_Supported;
			return _locationInformationEPS_Supported;
		case 9:
			_t_adsData = new Null();
			_t_adsData->tlv->tag_class = CONTEXT_SPECIFIC;
			_t_adsData->tlv->tag_value = 8;
			children[9] = _t_adsData;
			return _t_adsData;
		case 10:
			_requestedNodes = new RequestedNodes();
			_requestedNodes->tlv->tag_class = CONTEXT_SPECIFIC;
			_requestedNodes->tlv->tag_value = 9;
			children[10] = _requestedNodes;
			return _requestedNodes;
		case 11:
			_servingNodeIndication = new Null();
			_servingNodeIndication->tlv->tag_class = CONTEXT_SPECIFIC;
			_servingNodeIndication->tlv->tag_value = 10;
			children[11] = _servingNodeIndication;
			return _servingNodeIndication;
		case 12:
			_localTimeZoneRequest = new Null();
			_localTimeZoneRequest->tlv->tag_class = CONTEXT_SPECIFIC;
			_localTimeZoneRequest->tlv->tag_value = 12;
			children[12] = _localTimeZoneRequest;
			return _localTimeZoneRequest;

		default: return NULL;
	}
}

void asn1::RequestedInfo::set_locationInformation(){
	if(_locationInformation == NULL) _locationInformation = (Null*)create_node(0);
}

void asn1::RequestedInfo::set_subscriberState(){
	if(_subscriberState == NULL) _subscriberState = (Null*)create_node(1);
}

void asn1::RequestedInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::RequestedInfo::set_currentLocation(){
	if(_currentLocation == NULL) _currentLocation = (Null*)create_node(3);
}

void asn1::RequestedInfo::set_requestedDomain(){
	if(_requestedDomain == NULL) _requestedDomain = (DomainType*)create_node(4);
}

void asn1::RequestedInfo::set_imei(){
	if(_imei == NULL) _imei = (Null*)create_node(5);
}

void asn1::RequestedInfo::set_ms_classmark(){
	if(_ms_classmark == NULL) _ms_classmark = (Null*)create_node(6);
}

void asn1::RequestedInfo::set_mnpRequestedInfo(){
	if(_mnpRequestedInfo == NULL) _mnpRequestedInfo = (Null*)create_node(7);
}

void asn1::RequestedInfo::set_locationInformationEPS_Supported(){
	if(_locationInformationEPS_Supported == NULL) _locationInformationEPS_Supported = (Null*)create_node(8);
}

void asn1::RequestedInfo::set_t_adsData(){
	if(_t_adsData == NULL) _t_adsData = (Null*)create_node(9);
}

void asn1::RequestedInfo::set_requestedNodes(){
	if(_requestedNodes == NULL) _requestedNodes = (RequestedNodes*)create_node(10);
}

void asn1::RequestedInfo::set_servingNodeIndication(){
	if(_servingNodeIndication == NULL) _servingNodeIndication = (Null*)create_node(11);
}

void asn1::RequestedInfo::set_localTimeZoneRequest(){
	if(_localTimeZoneRequest == NULL) _localTimeZoneRequest = (Null*)create_node(12);
}

//DomainType
asn1::DomainType::DomainType(){
	strcpy(node_type_name, "DomainType");

}
asn1::DomainType::~DomainType(){

}

//RequestedNodes
asn1::RequestedNodes::RequestedNodes(){
	strcpy(node_type_name, "RequestedNodes");

}
asn1::RequestedNodes::~RequestedNodes(){

}

//LocationInformation
asn1::LocationInformation::LocationInformation(){
	strcpy(node_type_name, "LocationInformation");
	// ageOfLocationInformation
	_ageOfLocationInformation = NULL;
	children.push_back(_ageOfLocationInformation);

	// geographicalInformation
	_geographicalInformation = NULL;
	children.push_back(_geographicalInformation);

	// vlr_number
	_vlr_number = NULL;
	children.push_back(_vlr_number);

	// locationNumber
	_locationNumber = NULL;
	children.push_back(_locationNumber);

	// cellGlobalIdOrServiceAreaIdOrLAI
	_cellGlobalIdOrServiceAreaIdOrLAI = NULL;
	children.push_back(_cellGlobalIdOrServiceAreaIdOrLAI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// selectedLSA_Id
	_selectedLSA_Id = NULL;
	children.push_back(_selectedLSA_Id);

	// msc_Number
	_msc_Number = NULL;
	children.push_back(_msc_Number);

	// geodeticInformation
	_geodeticInformation = NULL;
	children.push_back(_geodeticInformation);

	// currentLocationRetrieved
	_currentLocationRetrieved = NULL;
	children.push_back(_currentLocationRetrieved);

	// sai_Present
	_sai_Present = NULL;
	children.push_back(_sai_Present);

	// locationInformationEPS
	_locationInformationEPS = NULL;
	children.push_back(_locationInformationEPS);

	// userCSGInformation
	_userCSGInformation = NULL;
	children.push_back(_userCSGInformation);


}
asn1::LocationInformation::~LocationInformation(){

}

asn1::ASN1Node* asn1::LocationInformation::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ageOfLocationInformation = new AgeOfLocationInformation();
			children[0] = _ageOfLocationInformation;
			return _ageOfLocationInformation;
		case 1:
			_geographicalInformation = new GeographicalInformation();
			_geographicalInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_geographicalInformation->tlv->tag_value = 0;
			children[1] = _geographicalInformation;
			return _geographicalInformation;
		case 2:
			_vlr_number = new ISDN_AddressString();
			_vlr_number->tlv->tag_class = CONTEXT_SPECIFIC;
			_vlr_number->tlv->tag_value = 1;
			children[2] = _vlr_number;
			return _vlr_number;
		case 3:
			_locationNumber = new LocationNumber();
			_locationNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationNumber->tlv->tag_value = 2;
			children[3] = _locationNumber;
			return _locationNumber;
                case 4:
                {
                        _cellGlobalIdOrServiceAreaIdOrLAI = new CellGlobalIdOrServiceAreaIdOrLAI();
                        ASN1Node* _cellGlobalIdOrServiceAreaIdOrLAI_wrapper = new ASN1Node();
                        _cellGlobalIdOrServiceAreaIdOrLAI_wrapper->tlv = new TLVNode();
                        _cellGlobalIdOrServiceAreaIdOrLAI_wrapper->tlv->is_explicit = true;
                        _cellGlobalIdOrServiceAreaIdOrLAI_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _cellGlobalIdOrServiceAreaIdOrLAI_wrapper->tlv->tag_value = 3;
                        _cellGlobalIdOrServiceAreaIdOrLAI_wrapper->children.push_back(_cellGlobalIdOrServiceAreaIdOrLAI);
                        children[4] = _cellGlobalIdOrServiceAreaIdOrLAI_wrapper;
                        return _cellGlobalIdOrServiceAreaIdOrLAI;
                }
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 4;
			children[5] = _extensionContainer;
			return _extensionContainer;
		case 6:
			_selectedLSA_Id = new LSAIdentity();
			_selectedLSA_Id->tlv->tag_class = CONTEXT_SPECIFIC;
			_selectedLSA_Id->tlv->tag_value = 5;
			children[6] = _selectedLSA_Id;
			return _selectedLSA_Id;
		case 7:
			_msc_Number = new ISDN_AddressString();
			_msc_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_msc_Number->tlv->tag_value = 6;
			children[7] = _msc_Number;
			return _msc_Number;
		case 8:
			_geodeticInformation = new GeodeticInformation();
			_geodeticInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_geodeticInformation->tlv->tag_value = 7;
			children[8] = _geodeticInformation;
			return _geodeticInformation;
		case 9:
			_currentLocationRetrieved = new Null();
			_currentLocationRetrieved->tlv->tag_class = CONTEXT_SPECIFIC;
			_currentLocationRetrieved->tlv->tag_value = 8;
			children[9] = _currentLocationRetrieved;
			return _currentLocationRetrieved;
		case 10:
			_sai_Present = new Null();
			_sai_Present->tlv->tag_class = CONTEXT_SPECIFIC;
			_sai_Present->tlv->tag_value = 9;
			children[10] = _sai_Present;
			return _sai_Present;
		case 11:
			_locationInformationEPS = new LocationInformationEPS();
			_locationInformationEPS->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationInformationEPS->tlv->tag_value = 10;
			children[11] = _locationInformationEPS;
			return _locationInformationEPS;
		case 12:
			_userCSGInformation = new UserCSGInformation();
			_userCSGInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_userCSGInformation->tlv->tag_value = 11;
			children[12] = _userCSGInformation;
			return _userCSGInformation;

		default: return NULL;
	}
}

void asn1::LocationInformation::set_ageOfLocationInformation(){
	if(_ageOfLocationInformation == NULL) _ageOfLocationInformation = (AgeOfLocationInformation*)create_node(0);
}

void asn1::LocationInformation::set_geographicalInformation(){
	if(_geographicalInformation == NULL) _geographicalInformation = (GeographicalInformation*)create_node(1);
}

void asn1::LocationInformation::set_vlr_number(){
	if(_vlr_number == NULL) _vlr_number = (ISDN_AddressString*)create_node(2);
}

void asn1::LocationInformation::set_locationNumber(){
	if(_locationNumber == NULL) _locationNumber = (LocationNumber*)create_node(3);
}

void asn1::LocationInformation::set_cellGlobalIdOrServiceAreaIdOrLAI(){
	if(_cellGlobalIdOrServiceAreaIdOrLAI == NULL) _cellGlobalIdOrServiceAreaIdOrLAI = (CellGlobalIdOrServiceAreaIdOrLAI*)create_node(4);
}

void asn1::LocationInformation::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

void asn1::LocationInformation::set_selectedLSA_Id(){
	if(_selectedLSA_Id == NULL) _selectedLSA_Id = (LSAIdentity*)create_node(6);
}

void asn1::LocationInformation::set_msc_Number(){
	if(_msc_Number == NULL) _msc_Number = (ISDN_AddressString*)create_node(7);
}

void asn1::LocationInformation::set_geodeticInformation(){
	if(_geodeticInformation == NULL) _geodeticInformation = (GeodeticInformation*)create_node(8);
}

void asn1::LocationInformation::set_currentLocationRetrieved(){
	if(_currentLocationRetrieved == NULL) _currentLocationRetrieved = (Null*)create_node(9);
}

void asn1::LocationInformation::set_sai_Present(){
	if(_sai_Present == NULL) _sai_Present = (Null*)create_node(10);
}

void asn1::LocationInformation::set_locationInformationEPS(){
	if(_locationInformationEPS == NULL) _locationInformationEPS = (LocationInformationEPS*)create_node(11);
}

void asn1::LocationInformation::set_userCSGInformation(){
	if(_userCSGInformation == NULL) _userCSGInformation = (UserCSGInformation*)create_node(12);
}

//LocationInformationEPS
asn1::LocationInformationEPS::LocationInformationEPS(){
	strcpy(node_type_name, "LocationInformationEPS");
	// e_utranCellGlobalIdentity
	_e_utranCellGlobalIdentity = NULL;
	children.push_back(_e_utranCellGlobalIdentity);

	// trackingAreaIdentity
	_trackingAreaIdentity = NULL;
	children.push_back(_trackingAreaIdentity);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// geographicalInformation
	_geographicalInformation = NULL;
	children.push_back(_geographicalInformation);

	// geodeticInformation
	_geodeticInformation = NULL;
	children.push_back(_geodeticInformation);

	// currentLocationRetrieved
	_currentLocationRetrieved = NULL;
	children.push_back(_currentLocationRetrieved);

	// ageOfLocationInformation
	_ageOfLocationInformation = NULL;
	children.push_back(_ageOfLocationInformation);

	// mme_Name
	_mme_Name = NULL;
	children.push_back(_mme_Name);


}
asn1::LocationInformationEPS::~LocationInformationEPS(){

}

asn1::ASN1Node* asn1::LocationInformationEPS::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_e_utranCellGlobalIdentity = new E_UTRAN_CGI();
			_e_utranCellGlobalIdentity->tlv->tag_class = CONTEXT_SPECIFIC;
			_e_utranCellGlobalIdentity->tlv->tag_value = 0;
			children[0] = _e_utranCellGlobalIdentity;
			return _e_utranCellGlobalIdentity;
		case 1:
			_trackingAreaIdentity = new TA_Id();
			_trackingAreaIdentity->tlv->tag_class = CONTEXT_SPECIFIC;
			_trackingAreaIdentity->tlv->tag_value = 1;
			children[1] = _trackingAreaIdentity;
			return _trackingAreaIdentity;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_geographicalInformation = new GeographicalInformation();
			_geographicalInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_geographicalInformation->tlv->tag_value = 3;
			children[3] = _geographicalInformation;
			return _geographicalInformation;
		case 4:
			_geodeticInformation = new GeodeticInformation();
			_geodeticInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_geodeticInformation->tlv->tag_value = 4;
			children[4] = _geodeticInformation;
			return _geodeticInformation;
		case 5:
			_currentLocationRetrieved = new Null();
			_currentLocationRetrieved->tlv->tag_class = CONTEXT_SPECIFIC;
			_currentLocationRetrieved->tlv->tag_value = 5;
			children[5] = _currentLocationRetrieved;
			return _currentLocationRetrieved;
		case 6:
			_ageOfLocationInformation = new AgeOfLocationInformation();
			_ageOfLocationInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_ageOfLocationInformation->tlv->tag_value = 6;
			children[6] = _ageOfLocationInformation;
			return _ageOfLocationInformation;
		case 7:
			_mme_Name = new DiameterIdentity();
			_mme_Name->tlv->tag_class = CONTEXT_SPECIFIC;
			_mme_Name->tlv->tag_value = 7;
			children[7] = _mme_Name;
			return _mme_Name;

		default: return NULL;
	}
}

void asn1::LocationInformationEPS::set_e_utranCellGlobalIdentity(){
	if(_e_utranCellGlobalIdentity == NULL) _e_utranCellGlobalIdentity = (E_UTRAN_CGI*)create_node(0);
}

void asn1::LocationInformationEPS::set_trackingAreaIdentity(){
	if(_trackingAreaIdentity == NULL) _trackingAreaIdentity = (TA_Id*)create_node(1);
}

void asn1::LocationInformationEPS::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::LocationInformationEPS::set_geographicalInformation(){
	if(_geographicalInformation == NULL) _geographicalInformation = (GeographicalInformation*)create_node(3);
}

void asn1::LocationInformationEPS::set_geodeticInformation(){
	if(_geodeticInformation == NULL) _geodeticInformation = (GeodeticInformation*)create_node(4);
}

void asn1::LocationInformationEPS::set_currentLocationRetrieved(){
	if(_currentLocationRetrieved == NULL) _currentLocationRetrieved = (Null*)create_node(5);
}

void asn1::LocationInformationEPS::set_ageOfLocationInformation(){
	if(_ageOfLocationInformation == NULL) _ageOfLocationInformation = (AgeOfLocationInformation*)create_node(6);
}

void asn1::LocationInformationEPS::set_mme_Name(){
	if(_mme_Name == NULL) _mme_Name = (DiameterIdentity*)create_node(7);
}

//LocationInformationGPRS
asn1::LocationInformationGPRS::LocationInformationGPRS(){
	strcpy(node_type_name, "LocationInformationGPRS");
	// cellGlobalIdOrServiceAreaIdOrLAI
	_cellGlobalIdOrServiceAreaIdOrLAI = NULL;
	children.push_back(_cellGlobalIdOrServiceAreaIdOrLAI);

	// routeingAreaIdentity
	_routeingAreaIdentity = NULL;
	children.push_back(_routeingAreaIdentity);

	// geographicalInformation
	_geographicalInformation = NULL;
	children.push_back(_geographicalInformation);

	// sgsn_Number
	_sgsn_Number = NULL;
	children.push_back(_sgsn_Number);

	// selectedLSAIdentity
	_selectedLSAIdentity = NULL;
	children.push_back(_selectedLSAIdentity);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// sai_Present
	_sai_Present = NULL;
	children.push_back(_sai_Present);

	// geodeticInformation
	_geodeticInformation = NULL;
	children.push_back(_geodeticInformation);

	// currentLocationRetrieved
	_currentLocationRetrieved = NULL;
	children.push_back(_currentLocationRetrieved);

	// ageOfLocationInformation
	_ageOfLocationInformation = NULL;
	children.push_back(_ageOfLocationInformation);

	// userCSGInformation
	_userCSGInformation = NULL;
	children.push_back(_userCSGInformation);


}
asn1::LocationInformationGPRS::~LocationInformationGPRS(){

}

asn1::ASN1Node* asn1::LocationInformationGPRS::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_cellGlobalIdOrServiceAreaIdOrLAI = new CellGlobalIdOrServiceAreaIdOrLAI();
			ASN1Node* _cellGlobalIdOrServiceAreaIdOrLAI_wrapper = new ASN1Node();
			_cellGlobalIdOrServiceAreaIdOrLAI_wrapper->tlv = new TLVNode();
			_cellGlobalIdOrServiceAreaIdOrLAI_wrapper->tlv->is_explicit = true;
			_cellGlobalIdOrServiceAreaIdOrLAI_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
			_cellGlobalIdOrServiceAreaIdOrLAI_wrapper->tlv->tag_value = 0;
			_cellGlobalIdOrServiceAreaIdOrLAI_wrapper->children.push_back(_cellGlobalIdOrServiceAreaIdOrLAI);
			children[0] = _cellGlobalIdOrServiceAreaIdOrLAI_wrapper;
			return _cellGlobalIdOrServiceAreaIdOrLAI;
		}
		case 1:
			_routeingAreaIdentity = new RAIdentity();
			_routeingAreaIdentity->tlv->tag_class = CONTEXT_SPECIFIC;
			_routeingAreaIdentity->tlv->tag_value = 1;
			children[1] = _routeingAreaIdentity;
			return _routeingAreaIdentity;
		case 2:
			_geographicalInformation = new GeographicalInformation();
			_geographicalInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_geographicalInformation->tlv->tag_value = 2;
			children[2] = _geographicalInformation;
			return _geographicalInformation;
		case 3:
			_sgsn_Number = new ISDN_AddressString();
			_sgsn_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_Number->tlv->tag_value = 3;
			children[3] = _sgsn_Number;
			return _sgsn_Number;
		case 4:
			_selectedLSAIdentity = new LSAIdentity();
			_selectedLSAIdentity->tlv->tag_class = CONTEXT_SPECIFIC;
			_selectedLSAIdentity->tlv->tag_value = 4;
			children[4] = _selectedLSAIdentity;
			return _selectedLSAIdentity;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 5;
			children[5] = _extensionContainer;
			return _extensionContainer;
		case 6:
			_sai_Present = new Null();
			_sai_Present->tlv->tag_class = CONTEXT_SPECIFIC;
			_sai_Present->tlv->tag_value = 6;
			children[6] = _sai_Present;
			return _sai_Present;
		case 7:
			_geodeticInformation = new GeodeticInformation();
			_geodeticInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_geodeticInformation->tlv->tag_value = 7;
			children[7] = _geodeticInformation;
			return _geodeticInformation;
		case 8:
			_currentLocationRetrieved = new Null();
			_currentLocationRetrieved->tlv->tag_class = CONTEXT_SPECIFIC;
			_currentLocationRetrieved->tlv->tag_value = 8;
			children[8] = _currentLocationRetrieved;
			return _currentLocationRetrieved;
		case 9:
			_ageOfLocationInformation = new AgeOfLocationInformation();
			_ageOfLocationInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_ageOfLocationInformation->tlv->tag_value = 9;
			children[9] = _ageOfLocationInformation;
			return _ageOfLocationInformation;
		case 10:
			_userCSGInformation = new UserCSGInformation();
			_userCSGInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_userCSGInformation->tlv->tag_value = 10;
			children[10] = _userCSGInformation;
			return _userCSGInformation;

		default: return NULL;
	}
}

void asn1::LocationInformationGPRS::set_cellGlobalIdOrServiceAreaIdOrLAI(){
	if(_cellGlobalIdOrServiceAreaIdOrLAI == NULL) _cellGlobalIdOrServiceAreaIdOrLAI = (CellGlobalIdOrServiceAreaIdOrLAI*)create_node(0);
}

void asn1::LocationInformationGPRS::set_routeingAreaIdentity(){
	if(_routeingAreaIdentity == NULL) _routeingAreaIdentity = (RAIdentity*)create_node(1);
}

void asn1::LocationInformationGPRS::set_geographicalInformation(){
	if(_geographicalInformation == NULL) _geographicalInformation = (GeographicalInformation*)create_node(2);
}

void asn1::LocationInformationGPRS::set_sgsn_Number(){
	if(_sgsn_Number == NULL) _sgsn_Number = (ISDN_AddressString*)create_node(3);
}

void asn1::LocationInformationGPRS::set_selectedLSAIdentity(){
	if(_selectedLSAIdentity == NULL) _selectedLSAIdentity = (LSAIdentity*)create_node(4);
}

void asn1::LocationInformationGPRS::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

void asn1::LocationInformationGPRS::set_sai_Present(){
	if(_sai_Present == NULL) _sai_Present = (Null*)create_node(6);
}

void asn1::LocationInformationGPRS::set_geodeticInformation(){
	if(_geodeticInformation == NULL) _geodeticInformation = (GeodeticInformation*)create_node(7);
}

void asn1::LocationInformationGPRS::set_currentLocationRetrieved(){
	if(_currentLocationRetrieved == NULL) _currentLocationRetrieved = (Null*)create_node(8);
}

void asn1::LocationInformationGPRS::set_ageOfLocationInformation(){
	if(_ageOfLocationInformation == NULL) _ageOfLocationInformation = (AgeOfLocationInformation*)create_node(9);
}

void asn1::LocationInformationGPRS::set_userCSGInformation(){
	if(_userCSGInformation == NULL) _userCSGInformation = (UserCSGInformation*)create_node(10);
}

//UserCSGInformation
asn1::UserCSGInformation::UserCSGInformation(){
	strcpy(node_type_name, "UserCSGInformation");
	// csg_Id
	_csg_Id = NULL;
	_csg_Id = new CSG_Id();
	_csg_Id->tlv->tag_class = CONTEXT_SPECIFIC;
	_csg_Id->tlv->tag_value = 0;
	children.push_back(_csg_Id);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// accessMode
	_accessMode = NULL;
	children.push_back(_accessMode);

	// cmi
	_cmi = NULL;
	children.push_back(_cmi);


}
asn1::UserCSGInformation::~UserCSGInformation(){

}

asn1::ASN1Node* asn1::UserCSGInformation::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_accessMode = new Octet_string();
			_accessMode->tlv->tag_class = CONTEXT_SPECIFIC;
			_accessMode->tlv->tag_value = 2;
			children[2] = _accessMode;
			return _accessMode;
		case 3:
			_cmi = new Octet_string();
			_cmi->tlv->tag_class = CONTEXT_SPECIFIC;
			_cmi->tlv->tag_value = 3;
			children[3] = _cmi;
			return _cmi;

		default: return NULL;
	}
}

void asn1::UserCSGInformation::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::UserCSGInformation::set_accessMode(){
	if(_accessMode == NULL) _accessMode = (Octet_string*)create_node(2);
}

void asn1::UserCSGInformation::set_cmi(){
	if(_cmi == NULL) _cmi = (Octet_string*)create_node(3);
}

//GeographicalInformation
asn1::GeographicalInformation::GeographicalInformation(){
	strcpy(node_type_name, "GeographicalInformation");

}
asn1::GeographicalInformation::~GeographicalInformation(){

}

//GeodeticInformation
asn1::GeodeticInformation::GeodeticInformation(){
	strcpy(node_type_name, "GeodeticInformation");

}
asn1::GeodeticInformation::~GeodeticInformation(){

}

//LocationNumber
asn1::LocationNumber::LocationNumber(){
	strcpy(node_type_name, "LocationNumber");

}
asn1::LocationNumber::~LocationNumber(){

}

//SubscriberState
asn1::SubscriberState::SubscriberState(){
	strcpy(node_type_name, "SubscriberState");
	// assumedIdle
	_assumedIdle = NULL;
	_assumedIdle = new Null();
	_assumedIdle->tlv->tag_class = CONTEXT_SPECIFIC;
	_assumedIdle->tlv->tag_value = 0;
	children.push_back(_assumedIdle);

	// camelBusy
	_camelBusy = NULL;
	_camelBusy = new Null();
	_camelBusy->tlv->tag_class = CONTEXT_SPECIFIC;
	_camelBusy->tlv->tag_value = 1;
	children.push_back(_camelBusy);

	// netDetNotReachable
	_netDetNotReachable = NULL;
	_netDetNotReachable = new NotReachableReason();
	children.push_back(_netDetNotReachable);

	// notProvidedFromVLR
	_notProvidedFromVLR = NULL;
	_notProvidedFromVLR = new Null();
	_notProvidedFromVLR->tlv->tag_class = CONTEXT_SPECIFIC;
	_notProvidedFromVLR->tlv->tag_value = 2;
	children.push_back(_notProvidedFromVLR);


}
asn1::SubscriberState::~SubscriberState(){

}

//PS_SubscriberState
asn1::PS_SubscriberState::PS_SubscriberState(){
	strcpy(node_type_name, "PS_SubscriberState");
	// notProvidedFromSGSNorMME
	_notProvidedFromSGSNorMME = NULL;
	_notProvidedFromSGSNorMME = new Null();
	_notProvidedFromSGSNorMME->tlv->tag_class = CONTEXT_SPECIFIC;
	_notProvidedFromSGSNorMME->tlv->tag_value = 0;
	children.push_back(_notProvidedFromSGSNorMME);

	// ps_Detached
	_ps_Detached = NULL;
	_ps_Detached = new Null();
	_ps_Detached->tlv->tag_class = CONTEXT_SPECIFIC;
	_ps_Detached->tlv->tag_value = 1;
	children.push_back(_ps_Detached);

	// ps_AttachedNotReachableForPaging
	_ps_AttachedNotReachableForPaging = NULL;
	_ps_AttachedNotReachableForPaging = new Null();
	_ps_AttachedNotReachableForPaging->tlv->tag_class = CONTEXT_SPECIFIC;
	_ps_AttachedNotReachableForPaging->tlv->tag_value = 2;
	children.push_back(_ps_AttachedNotReachableForPaging);

	// ps_AttachedReachableForPaging
	_ps_AttachedReachableForPaging = NULL;
	_ps_AttachedReachableForPaging = new Null();
	_ps_AttachedReachableForPaging->tlv->tag_class = CONTEXT_SPECIFIC;
	_ps_AttachedReachableForPaging->tlv->tag_value = 3;
	children.push_back(_ps_AttachedReachableForPaging);

	// ps_PDP_ActiveNotReachableForPaging
	_ps_PDP_ActiveNotReachableForPaging = NULL;
	_ps_PDP_ActiveNotReachableForPaging = new PDP_ContextInfoList();
	_ps_PDP_ActiveNotReachableForPaging->tlv->tag_class = CONTEXT_SPECIFIC;
	_ps_PDP_ActiveNotReachableForPaging->tlv->tag_value = 4;
	children.push_back(_ps_PDP_ActiveNotReachableForPaging);

	// ps_PDP_ActiveReachableForPaging
	_ps_PDP_ActiveReachableForPaging = NULL;
	_ps_PDP_ActiveReachableForPaging = new PDP_ContextInfoList();
	_ps_PDP_ActiveReachableForPaging->tlv->tag_class = CONTEXT_SPECIFIC;
	_ps_PDP_ActiveReachableForPaging->tlv->tag_value = 5;
	children.push_back(_ps_PDP_ActiveReachableForPaging);

	// netDetNotReachable
	_netDetNotReachable = NULL;
	_netDetNotReachable = new NotReachableReason();
	children.push_back(_netDetNotReachable);


}
asn1::PS_SubscriberState::~PS_SubscriberState(){

}

//PDP_ContextInfoList
asn1::PDP_ContextInfoList::PDP_ContextInfoList(){
	strcpy(node_type_name, "PDP_ContextInfoList");

}
asn1::PDP_ContextInfoList::~PDP_ContextInfoList(){
}

asn1::PDP_ContextInfo* asn1::PDP_ContextInfoList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (PDP_ContextInfo*)children[child_index]; else return NULL;
}

void asn1::PDP_ContextInfoList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::PDP_ContextInfoList::create_node(unsigned int _index){
	children[_index] = new PDP_ContextInfo();
	return children[_index];
}

asn1::ASN1Node* asn1::PDP_ContextInfoList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new PDP_ContextInfo());
		return children[children.size() - 1];
	}

}

//PDP_ContextInfo
asn1::PDP_ContextInfo::PDP_ContextInfo(){
	strcpy(node_type_name, "PDP_ContextInfo");
	// pdp_ContextIdentifier
	_pdp_ContextIdentifier = NULL;
	_pdp_ContextIdentifier = new ContextId();
	_pdp_ContextIdentifier->tlv->tag_class = CONTEXT_SPECIFIC;
	_pdp_ContextIdentifier->tlv->tag_value = 0;
	children.push_back(_pdp_ContextIdentifier);

	// pdp_ContextActive
	_pdp_ContextActive = NULL;
	children.push_back(_pdp_ContextActive);

	// pdp_Type
	_pdp_Type = NULL;
	_pdp_Type = new PDP_Type();
	_pdp_Type->tlv->tag_class = CONTEXT_SPECIFIC;
	_pdp_Type->tlv->tag_value = 2;
	children.push_back(_pdp_Type);

	// pdp_Address
	_pdp_Address = NULL;
	children.push_back(_pdp_Address);

	// apn_Subscribed
	_apn_Subscribed = NULL;
	children.push_back(_apn_Subscribed);

	// apn_InUse
	_apn_InUse = NULL;
	children.push_back(_apn_InUse);

	// nsapi
	_nsapi = NULL;
	children.push_back(_nsapi);

	// transactionId
	_transactionId = NULL;
	children.push_back(_transactionId);

	// teid_ForGnAndGp
	_teid_ForGnAndGp = NULL;
	children.push_back(_teid_ForGnAndGp);

	// teid_ForIu
	_teid_ForIu = NULL;
	children.push_back(_teid_ForIu);

	// ggsn_Address
	_ggsn_Address = NULL;
	children.push_back(_ggsn_Address);

	// qos_Subscribed
	_qos_Subscribed = NULL;
	children.push_back(_qos_Subscribed);

	// qos_Requested
	_qos_Requested = NULL;
	children.push_back(_qos_Requested);

	// qos_Negotiated
	_qos_Negotiated = NULL;
	children.push_back(_qos_Negotiated);

	// chargingId
	_chargingId = NULL;
	children.push_back(_chargingId);

	// chargingCharacteristics
	_chargingCharacteristics = NULL;
	children.push_back(_chargingCharacteristics);

	// rnc_Address
	_rnc_Address = NULL;
	children.push_back(_rnc_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// qos2_Subscribed
	_qos2_Subscribed = NULL;
	children.push_back(_qos2_Subscribed);

	// qos2_Requested
	_qos2_Requested = NULL;
	children.push_back(_qos2_Requested);

	// qos2_Negotiated
	_qos2_Negotiated = NULL;
	children.push_back(_qos2_Negotiated);

	// qos3_Subscribed
	_qos3_Subscribed = NULL;
	children.push_back(_qos3_Subscribed);

	// qos3_Requested
	_qos3_Requested = NULL;
	children.push_back(_qos3_Requested);

	// qos3_Negotiated
	_qos3_Negotiated = NULL;
	children.push_back(_qos3_Negotiated);

	// qos4_Subscribed
	_qos4_Subscribed = NULL;
	children.push_back(_qos4_Subscribed);

	// qos4_Requested
	_qos4_Requested = NULL;
	children.push_back(_qos4_Requested);

	// qos4_Negotiated
	_qos4_Negotiated = NULL;
	children.push_back(_qos4_Negotiated);

	// ext_pdp_Type
	_ext_pdp_Type = NULL;
	children.push_back(_ext_pdp_Type);

	// ext_pdp_Address
	_ext_pdp_Address = NULL;
	children.push_back(_ext_pdp_Address);


}
asn1::PDP_ContextInfo::~PDP_ContextInfo(){

}

asn1::ASN1Node* asn1::PDP_ContextInfo::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_pdp_ContextActive = new Null();
			_pdp_ContextActive->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdp_ContextActive->tlv->tag_value = 1;
			children[1] = _pdp_ContextActive;
			return _pdp_ContextActive;
		case 3:
			_pdp_Address = new PDP_Address();
			_pdp_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_pdp_Address->tlv->tag_value = 3;
			children[3] = _pdp_Address;
			return _pdp_Address;
		case 4:
			_apn_Subscribed = new APN();
			_apn_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
			_apn_Subscribed->tlv->tag_value = 4;
			children[4] = _apn_Subscribed;
			return _apn_Subscribed;
		case 5:
			_apn_InUse = new APN();
			_apn_InUse->tlv->tag_class = CONTEXT_SPECIFIC;
			_apn_InUse->tlv->tag_value = 5;
			children[5] = _apn_InUse;
			return _apn_InUse;
		case 6:
			_nsapi = new NSAPI();
			_nsapi->tlv->tag_class = CONTEXT_SPECIFIC;
			_nsapi->tlv->tag_value = 6;
			children[6] = _nsapi;
			return _nsapi;
		case 7:
			_transactionId = new TransactionId();
			_transactionId->tlv->tag_class = CONTEXT_SPECIFIC;
			_transactionId->tlv->tag_value = 7;
			children[7] = _transactionId;
			return _transactionId;
		case 8:
			_teid_ForGnAndGp = new TEID();
			_teid_ForGnAndGp->tlv->tag_class = CONTEXT_SPECIFIC;
			_teid_ForGnAndGp->tlv->tag_value = 8;
			children[8] = _teid_ForGnAndGp;
			return _teid_ForGnAndGp;
		case 9:
			_teid_ForIu = new TEID();
			_teid_ForIu->tlv->tag_class = CONTEXT_SPECIFIC;
			_teid_ForIu->tlv->tag_value = 9;
			children[9] = _teid_ForIu;
			return _teid_ForIu;
		case 10:
			_ggsn_Address = new GSN_Address();
			_ggsn_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_Address->tlv->tag_value = 10;
			children[10] = _ggsn_Address;
			return _ggsn_Address;
		case 11:
			_qos_Subscribed = new Ext_QoS_Subscribed();
			_qos_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos_Subscribed->tlv->tag_value = 11;
			children[11] = _qos_Subscribed;
			return _qos_Subscribed;
		case 12:
			_qos_Requested = new Ext_QoS_Subscribed();
			_qos_Requested->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos_Requested->tlv->tag_value = 12;
			children[12] = _qos_Requested;
			return _qos_Requested;
		case 13:
			_qos_Negotiated = new Ext_QoS_Subscribed();
			_qos_Negotiated->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos_Negotiated->tlv->tag_value = 13;
			children[13] = _qos_Negotiated;
			return _qos_Negotiated;
		case 14:
			_chargingId = new GPRSChargingID();
			_chargingId->tlv->tag_class = CONTEXT_SPECIFIC;
			_chargingId->tlv->tag_value = 14;
			children[14] = _chargingId;
			return _chargingId;
		case 15:
			_chargingCharacteristics = new ChargingCharacteristics();
			_chargingCharacteristics->tlv->tag_class = CONTEXT_SPECIFIC;
			_chargingCharacteristics->tlv->tag_value = 15;
			children[15] = _chargingCharacteristics;
			return _chargingCharacteristics;
		case 16:
			_rnc_Address = new GSN_Address();
			_rnc_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_rnc_Address->tlv->tag_value = 16;
			children[16] = _rnc_Address;
			return _rnc_Address;
		case 17:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 17;
			children[17] = _extensionContainer;
			return _extensionContainer;
		case 18:
			_qos2_Subscribed = new Ext2_QoS_Subscribed();
			_qos2_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos2_Subscribed->tlv->tag_value = 18;
			children[18] = _qos2_Subscribed;
			return _qos2_Subscribed;
		case 19:
			_qos2_Requested = new Ext2_QoS_Subscribed();
			_qos2_Requested->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos2_Requested->tlv->tag_value = 19;
			children[19] = _qos2_Requested;
			return _qos2_Requested;
		case 20:
			_qos2_Negotiated = new Ext2_QoS_Subscribed();
			_qos2_Negotiated->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos2_Negotiated->tlv->tag_value = 20;
			children[20] = _qos2_Negotiated;
			return _qos2_Negotiated;
		case 21:
			_qos3_Subscribed = new Ext3_QoS_Subscribed();
			_qos3_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos3_Subscribed->tlv->tag_value = 21;
			children[21] = _qos3_Subscribed;
			return _qos3_Subscribed;
		case 22:
			_qos3_Requested = new Ext3_QoS_Subscribed();
			_qos3_Requested->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos3_Requested->tlv->tag_value = 22;
			children[22] = _qos3_Requested;
			return _qos3_Requested;
		case 23:
			_qos3_Negotiated = new Ext3_QoS_Subscribed();
			_qos3_Negotiated->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos3_Negotiated->tlv->tag_value = 23;
			children[23] = _qos3_Negotiated;
			return _qos3_Negotiated;
		case 24:
			_qos4_Subscribed = new Ext4_QoS_Subscribed();
			_qos4_Subscribed->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos4_Subscribed->tlv->tag_value = 25;
			children[24] = _qos4_Subscribed;
			return _qos4_Subscribed;
		case 25:
			_qos4_Requested = new Ext4_QoS_Subscribed();
			_qos4_Requested->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos4_Requested->tlv->tag_value = 26;
			children[25] = _qos4_Requested;
			return _qos4_Requested;
		case 26:
			_qos4_Negotiated = new Ext4_QoS_Subscribed();
			_qos4_Negotiated->tlv->tag_class = CONTEXT_SPECIFIC;
			_qos4_Negotiated->tlv->tag_value = 27;
			children[26] = _qos4_Negotiated;
			return _qos4_Negotiated;
		case 27:
			_ext_pdp_Type = new Ext_PDP_Type();
			_ext_pdp_Type->tlv->tag_class = CONTEXT_SPECIFIC;
			_ext_pdp_Type->tlv->tag_value = 28;
			children[27] = _ext_pdp_Type;
			return _ext_pdp_Type;
		case 28:
			_ext_pdp_Address = new PDP_Address();
			_ext_pdp_Address->tlv->tag_class = CONTEXT_SPECIFIC;
			_ext_pdp_Address->tlv->tag_value = 29;
			children[28] = _ext_pdp_Address;
			return _ext_pdp_Address;

		default: return NULL;
	}
}

void asn1::PDP_ContextInfo::set_pdp_ContextActive(){
	if(_pdp_ContextActive == NULL) _pdp_ContextActive = (Null*)create_node(1);
}

void asn1::PDP_ContextInfo::set_pdp_Address(){
	if(_pdp_Address == NULL) _pdp_Address = (PDP_Address*)create_node(3);
}

void asn1::PDP_ContextInfo::set_apn_Subscribed(){
	if(_apn_Subscribed == NULL) _apn_Subscribed = (APN*)create_node(4);
}

void asn1::PDP_ContextInfo::set_apn_InUse(){
	if(_apn_InUse == NULL) _apn_InUse = (APN*)create_node(5);
}

void asn1::PDP_ContextInfo::set_nsapi(){
	if(_nsapi == NULL) _nsapi = (NSAPI*)create_node(6);
}

void asn1::PDP_ContextInfo::set_transactionId(){
	if(_transactionId == NULL) _transactionId = (TransactionId*)create_node(7);
}

void asn1::PDP_ContextInfo::set_teid_ForGnAndGp(){
	if(_teid_ForGnAndGp == NULL) _teid_ForGnAndGp = (TEID*)create_node(8);
}

void asn1::PDP_ContextInfo::set_teid_ForIu(){
	if(_teid_ForIu == NULL) _teid_ForIu = (TEID*)create_node(9);
}

void asn1::PDP_ContextInfo::set_ggsn_Address(){
	if(_ggsn_Address == NULL) _ggsn_Address = (GSN_Address*)create_node(10);
}

void asn1::PDP_ContextInfo::set_qos_Subscribed(){
	if(_qos_Subscribed == NULL) _qos_Subscribed = (Ext_QoS_Subscribed*)create_node(11);
}

void asn1::PDP_ContextInfo::set_qos_Requested(){
	if(_qos_Requested == NULL) _qos_Requested = (Ext_QoS_Subscribed*)create_node(12);
}

void asn1::PDP_ContextInfo::set_qos_Negotiated(){
	if(_qos_Negotiated == NULL) _qos_Negotiated = (Ext_QoS_Subscribed*)create_node(13);
}

void asn1::PDP_ContextInfo::set_chargingId(){
	if(_chargingId == NULL) _chargingId = (GPRSChargingID*)create_node(14);
}

void asn1::PDP_ContextInfo::set_chargingCharacteristics(){
	if(_chargingCharacteristics == NULL) _chargingCharacteristics = (ChargingCharacteristics*)create_node(15);
}

void asn1::PDP_ContextInfo::set_rnc_Address(){
	if(_rnc_Address == NULL) _rnc_Address = (GSN_Address*)create_node(16);
}

void asn1::PDP_ContextInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(17);
}

void asn1::PDP_ContextInfo::set_qos2_Subscribed(){
	if(_qos2_Subscribed == NULL) _qos2_Subscribed = (Ext2_QoS_Subscribed*)create_node(18);
}

void asn1::PDP_ContextInfo::set_qos2_Requested(){
	if(_qos2_Requested == NULL) _qos2_Requested = (Ext2_QoS_Subscribed*)create_node(19);
}

void asn1::PDP_ContextInfo::set_qos2_Negotiated(){
	if(_qos2_Negotiated == NULL) _qos2_Negotiated = (Ext2_QoS_Subscribed*)create_node(20);
}

void asn1::PDP_ContextInfo::set_qos3_Subscribed(){
	if(_qos3_Subscribed == NULL) _qos3_Subscribed = (Ext3_QoS_Subscribed*)create_node(21);
}

void asn1::PDP_ContextInfo::set_qos3_Requested(){
	if(_qos3_Requested == NULL) _qos3_Requested = (Ext3_QoS_Subscribed*)create_node(22);
}

void asn1::PDP_ContextInfo::set_qos3_Negotiated(){
	if(_qos3_Negotiated == NULL) _qos3_Negotiated = (Ext3_QoS_Subscribed*)create_node(23);
}

void asn1::PDP_ContextInfo::set_qos4_Subscribed(){
	if(_qos4_Subscribed == NULL) _qos4_Subscribed = (Ext4_QoS_Subscribed*)create_node(24);
}

void asn1::PDP_ContextInfo::set_qos4_Requested(){
	if(_qos4_Requested == NULL) _qos4_Requested = (Ext4_QoS_Subscribed*)create_node(25);
}

void asn1::PDP_ContextInfo::set_qos4_Negotiated(){
	if(_qos4_Negotiated == NULL) _qos4_Negotiated = (Ext4_QoS_Subscribed*)create_node(26);
}

void asn1::PDP_ContextInfo::set_ext_pdp_Type(){
	if(_ext_pdp_Type == NULL) _ext_pdp_Type = (Ext_PDP_Type*)create_node(27);
}

void asn1::PDP_ContextInfo::set_ext_pdp_Address(){
	if(_ext_pdp_Address == NULL) _ext_pdp_Address = (PDP_Address*)create_node(28);
}

//NSAPI
asn1::NSAPI::NSAPI(){
	strcpy(node_type_name, "NSAPI");

}
asn1::NSAPI::~NSAPI(){

}

//TransactionId
asn1::TransactionId::TransactionId(){
	strcpy(node_type_name, "TransactionId");

}
asn1::TransactionId::~TransactionId(){

}

//TEID
asn1::TEID::TEID(){
	strcpy(node_type_name, "TEID");

}
asn1::TEID::~TEID(){

}

//GPRSChargingID
asn1::GPRSChargingID::GPRSChargingID(){
	strcpy(node_type_name, "GPRSChargingID");

}
asn1::GPRSChargingID::~GPRSChargingID(){

}

//NotReachableReason
asn1::NotReachableReason::NotReachableReason(){
	strcpy(node_type_name, "NotReachableReason");

}
asn1::NotReachableReason::~NotReachableReason(){

}

//AnyTimeInterrogationArg
asn1::AnyTimeInterrogationArg::AnyTimeInterrogationArg(){
	strcpy(node_type_name, "AnyTimeInterrogationArg");
        // subscriberIdentity
        _subscriberIdentity = NULL;
        _subscriberIdentity = new SubscriberIdentity();
        ASN1Node* _subscriberIdentity_wrapper = new ASN1Node();
        _subscriberIdentity_wrapper->tlv = new TLVNode();
        _subscriberIdentity_wrapper->tlv->is_explicit = true;
        _subscriberIdentity_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
        _subscriberIdentity_wrapper->tlv->tag_value = 0;
        _subscriberIdentity_wrapper->children.push_back(_subscriberIdentity);
        children.push_back(_subscriberIdentity_wrapper);

	// requestedInfo
	_requestedInfo = NULL;
	_requestedInfo = new RequestedInfo();
	_requestedInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_requestedInfo->tlv->tag_value = 1;
	children.push_back(_requestedInfo);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	_gsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsmSCF_Address->tlv->tag_value = 3;
	children.push_back(_gsmSCF_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::AnyTimeInterrogationArg::~AnyTimeInterrogationArg(){

}

asn1::ASN1Node* asn1::AnyTimeInterrogationArg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::AnyTimeInterrogationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//AnyTimeInterrogationRes
asn1::AnyTimeInterrogationRes::AnyTimeInterrogationRes(){
	strcpy(node_type_name, "AnyTimeInterrogationRes");
	// subscriberInfo
	_subscriberInfo = NULL;
	_subscriberInfo = new SubscriberInfo();
	children.push_back(_subscriberInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::AnyTimeInterrogationRes::~AnyTimeInterrogationRes(){

}

asn1::ASN1Node* asn1::AnyTimeInterrogationRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::AnyTimeInterrogationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//AnyTimeSubscriptionInterrogationArg
asn1::AnyTimeSubscriptionInterrogationArg::AnyTimeSubscriptionInterrogationArg(){
	strcpy(node_type_name, "AnyTimeSubscriptionInterrogationArg");
        // subscriberIdentity
        _subscriberIdentity = NULL;
        _subscriberIdentity = new SubscriberIdentity();
        ASN1Node* _subscriberIdentity_wrapper = new ASN1Node();
        _subscriberIdentity_wrapper->tlv = new TLVNode();
        _subscriberIdentity_wrapper->tlv->is_explicit = true;
        _subscriberIdentity_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
        _subscriberIdentity_wrapper->tlv->tag_value = 0;
        _subscriberIdentity_wrapper->children.push_back(_subscriberIdentity);
        children.push_back(_subscriberIdentity_wrapper);

	// requestedSubscriptionInfo
	_requestedSubscriptionInfo = NULL;
	_requestedSubscriptionInfo = new RequestedSubscriptionInfo();
	_requestedSubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_requestedSubscriptionInfo->tlv->tag_value = 1;
	children.push_back(_requestedSubscriptionInfo);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	_gsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsmSCF_Address->tlv->tag_value = 2;
	children.push_back(_gsmSCF_Address);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// longFTN_Supported
	_longFTN_Supported = NULL;
	children.push_back(_longFTN_Supported);


}
asn1::AnyTimeSubscriptionInterrogationArg::~AnyTimeSubscriptionInterrogationArg(){

}

asn1::ASN1Node* asn1::AnyTimeSubscriptionInterrogationArg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_longFTN_Supported = new Null();
			_longFTN_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_longFTN_Supported->tlv->tag_value = 4;
			children[4] = _longFTN_Supported;
			return _longFTN_Supported;

		default: return NULL;
	}
}

void asn1::AnyTimeSubscriptionInterrogationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::AnyTimeSubscriptionInterrogationArg::set_longFTN_Supported(){
	if(_longFTN_Supported == NULL) _longFTN_Supported = (Null*)create_node(4);
}

//AnyTimeSubscriptionInterrogationRes
asn1::AnyTimeSubscriptionInterrogationRes::AnyTimeSubscriptionInterrogationRes(){
	strcpy(node_type_name, "AnyTimeSubscriptionInterrogationRes");
	// callForwardingData
	_callForwardingData = NULL;
	children.push_back(_callForwardingData);

	// callBarringData
	_callBarringData = NULL;
	children.push_back(_callBarringData);

	// odb_Info
	_odb_Info = NULL;
	children.push_back(_odb_Info);

	// camel_SubscriptionInfo
	_camel_SubscriptionInfo = NULL;
	children.push_back(_camel_SubscriptionInfo);

	// supportedVLR_CAMEL_Phases
	_supportedVLR_CAMEL_Phases = NULL;
	children.push_back(_supportedVLR_CAMEL_Phases);

	// supportedSGSN_CAMEL_Phases
	_supportedSGSN_CAMEL_Phases = NULL;
	children.push_back(_supportedSGSN_CAMEL_Phases);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// offeredCamel4CSIsInVLR
	_offeredCamel4CSIsInVLR = NULL;
	children.push_back(_offeredCamel4CSIsInVLR);

	// offeredCamel4CSIsInSGSN
	_offeredCamel4CSIsInSGSN = NULL;
	children.push_back(_offeredCamel4CSIsInSGSN);

	// msisdn_BS_List
	_msisdn_BS_List = NULL;
	children.push_back(_msisdn_BS_List);

	// csg_SubscriptionDataList
	_csg_SubscriptionDataList = NULL;
	children.push_back(_csg_SubscriptionDataList);

	// cw_Data
	_cw_Data = NULL;
	children.push_back(_cw_Data);

	// ch_Data
	_ch_Data = NULL;
	children.push_back(_ch_Data);

	// clip_Data
	_clip_Data = NULL;
	children.push_back(_clip_Data);

	// clir_Data
	_clir_Data = NULL;
	children.push_back(_clir_Data);

	// ect_data
	_ect_data = NULL;
	children.push_back(_ect_data);


}
asn1::AnyTimeSubscriptionInterrogationRes::~AnyTimeSubscriptionInterrogationRes(){

}

asn1::ASN1Node* asn1::AnyTimeSubscriptionInterrogationRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_callForwardingData = new CallForwardingData();
			_callForwardingData->tlv->tag_class = CONTEXT_SPECIFIC;
			_callForwardingData->tlv->tag_value = 1;
			children[0] = _callForwardingData;
			return _callForwardingData;
		case 1:
			_callBarringData = new CallBarringData();
			_callBarringData->tlv->tag_class = CONTEXT_SPECIFIC;
			_callBarringData->tlv->tag_value = 2;
			children[1] = _callBarringData;
			return _callBarringData;
		case 2:
			_odb_Info = new ODB_Info();
			_odb_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_odb_Info->tlv->tag_value = 3;
			children[2] = _odb_Info;
			return _odb_Info;
		case 3:
			_camel_SubscriptionInfo = new CAMEL_SubscriptionInfo();
			_camel_SubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_camel_SubscriptionInfo->tlv->tag_value = 4;
			children[3] = _camel_SubscriptionInfo;
			return _camel_SubscriptionInfo;
		case 4:
			_supportedVLR_CAMEL_Phases = new SupportedCamelPhases();
			_supportedVLR_CAMEL_Phases->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedVLR_CAMEL_Phases->tlv->tag_value = 5;
			children[4] = _supportedVLR_CAMEL_Phases;
			return _supportedVLR_CAMEL_Phases;
		case 5:
			_supportedSGSN_CAMEL_Phases = new SupportedCamelPhases();
			_supportedSGSN_CAMEL_Phases->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedSGSN_CAMEL_Phases->tlv->tag_value = 6;
			children[5] = _supportedSGSN_CAMEL_Phases;
			return _supportedSGSN_CAMEL_Phases;
		case 6:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 7;
			children[6] = _extensionContainer;
			return _extensionContainer;
		case 7:
			_offeredCamel4CSIsInVLR = new OfferedCamel4CSIs();
			_offeredCamel4CSIsInVLR->tlv->tag_class = CONTEXT_SPECIFIC;
			_offeredCamel4CSIsInVLR->tlv->tag_value = 8;
			children[7] = _offeredCamel4CSIsInVLR;
			return _offeredCamel4CSIsInVLR;
		case 8:
			_offeredCamel4CSIsInSGSN = new OfferedCamel4CSIs();
			_offeredCamel4CSIsInSGSN->tlv->tag_class = CONTEXT_SPECIFIC;
			_offeredCamel4CSIsInSGSN->tlv->tag_value = 9;
			children[8] = _offeredCamel4CSIsInSGSN;
			return _offeredCamel4CSIsInSGSN;
		case 9:
			_msisdn_BS_List = new MSISDN_BS_List();
			_msisdn_BS_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn_BS_List->tlv->tag_value = 10;
			children[9] = _msisdn_BS_List;
			return _msisdn_BS_List;
		case 10:
			_csg_SubscriptionDataList = new CSG_SubscriptionDataList();
			_csg_SubscriptionDataList->tlv->tag_class = CONTEXT_SPECIFIC;
			_csg_SubscriptionDataList->tlv->tag_value = 11;
			children[10] = _csg_SubscriptionDataList;
			return _csg_SubscriptionDataList;
		case 11:
			_cw_Data = new CallWaitingData();
			_cw_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_cw_Data->tlv->tag_value = 12;
			children[11] = _cw_Data;
			return _cw_Data;
		case 12:
			_ch_Data = new CallHoldData();
			_ch_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_ch_Data->tlv->tag_value = 13;
			children[12] = _ch_Data;
			return _ch_Data;
		case 13:
			_clip_Data = new ClipData();
			_clip_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_clip_Data->tlv->tag_value = 14;
			children[13] = _clip_Data;
			return _clip_Data;
		case 14:
			_clir_Data = new ClirData();
			_clir_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_clir_Data->tlv->tag_value = 15;
			children[14] = _clir_Data;
			return _clir_Data;
		case 15:
			_ect_data = new EctData();
			_ect_data->tlv->tag_class = CONTEXT_SPECIFIC;
			_ect_data->tlv->tag_value = 16;
			children[15] = _ect_data;
			return _ect_data;

		default: return NULL;
	}
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_callForwardingData(){
	if(_callForwardingData == NULL) _callForwardingData = (CallForwardingData*)create_node(0);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_callBarringData(){
	if(_callBarringData == NULL) _callBarringData = (CallBarringData*)create_node(1);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_odb_Info(){
	if(_odb_Info == NULL) _odb_Info = (ODB_Info*)create_node(2);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_camel_SubscriptionInfo(){
	if(_camel_SubscriptionInfo == NULL) _camel_SubscriptionInfo = (CAMEL_SubscriptionInfo*)create_node(3);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_supportedVLR_CAMEL_Phases(){
	if(_supportedVLR_CAMEL_Phases == NULL) _supportedVLR_CAMEL_Phases = (SupportedCamelPhases*)create_node(4);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_supportedSGSN_CAMEL_Phases(){
	if(_supportedSGSN_CAMEL_Phases == NULL) _supportedSGSN_CAMEL_Phases = (SupportedCamelPhases*)create_node(5);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(6);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_offeredCamel4CSIsInVLR(){
	if(_offeredCamel4CSIsInVLR == NULL) _offeredCamel4CSIsInVLR = (OfferedCamel4CSIs*)create_node(7);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_offeredCamel4CSIsInSGSN(){
	if(_offeredCamel4CSIsInSGSN == NULL) _offeredCamel4CSIsInSGSN = (OfferedCamel4CSIs*)create_node(8);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_msisdn_BS_List(){
	if(_msisdn_BS_List == NULL) _msisdn_BS_List = (MSISDN_BS_List*)create_node(9);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_csg_SubscriptionDataList(){
	if(_csg_SubscriptionDataList == NULL) _csg_SubscriptionDataList = (CSG_SubscriptionDataList*)create_node(10);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_cw_Data(){
	if(_cw_Data == NULL) _cw_Data = (CallWaitingData*)create_node(11);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_ch_Data(){
	if(_ch_Data == NULL) _ch_Data = (CallHoldData*)create_node(12);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_clip_Data(){
	if(_clip_Data == NULL) _clip_Data = (ClipData*)create_node(13);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_clir_Data(){
	if(_clir_Data == NULL) _clir_Data = (ClirData*)create_node(14);
}

void asn1::AnyTimeSubscriptionInterrogationRes::set_ect_data(){
	if(_ect_data == NULL) _ect_data = (EctData*)create_node(15);
}

//CallWaitingData
asn1::CallWaitingData::CallWaitingData(){
	strcpy(node_type_name, "CallWaitingData");
	// cwFeatureList
	_cwFeatureList = NULL;
	_cwFeatureList = new Ext_CwFeatureList();
	_cwFeatureList->tlv->tag_class = CONTEXT_SPECIFIC;
	_cwFeatureList->tlv->tag_value = 1;
	children.push_back(_cwFeatureList);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);


}
asn1::CallWaitingData::~CallWaitingData(){

}

asn1::ASN1Node* asn1::CallWaitingData::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 2;
			children[1] = _notificationToCSE;
			return _notificationToCSE;

		default: return NULL;
	}
}

void asn1::CallWaitingData::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(1);
}

//Ext_CwFeatureList
asn1::Ext_CwFeatureList::Ext_CwFeatureList(){
	strcpy(node_type_name, "Ext_CwFeatureList");

}
asn1::Ext_CwFeatureList::~Ext_CwFeatureList(){
}

asn1::Ext_CwFeature* asn1::Ext_CwFeatureList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Ext_CwFeature*)children[child_index]; else return NULL;
}

void asn1::Ext_CwFeatureList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::Ext_CwFeatureList::create_node(unsigned int _index){
	children[_index] = new Ext_CwFeature();
	return children[_index];
}

asn1::ASN1Node* asn1::Ext_CwFeatureList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Ext_CwFeature());
		return children[children.size() - 1];
	}

}

//Ext_CwFeature
asn1::Ext_CwFeature::Ext_CwFeature(){
	strcpy(node_type_name, "Ext_CwFeature");
        // basicService
        _basicService = NULL;
        _basicService = new Ext_BasicServiceCode();
        ASN1Node* _basicService_wrapper = new ASN1Node();
        _basicService_wrapper->tlv = new TLVNode();
        _basicService_wrapper->tlv->is_explicit = true;
        _basicService_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
        _basicService_wrapper->tlv->tag_value = 1;
        _basicService_wrapper->children.push_back(_basicService);
        children.push_back(_basicService_wrapper);

	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 2;
	children.push_back(_ss_Status);


}
asn1::Ext_CwFeature::~Ext_CwFeature(){

}

//ClipData
asn1::ClipData::ClipData(){
	strcpy(node_type_name, "ClipData");
	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 1;
	children.push_back(_ss_Status);

	// overrideCategory
	_overrideCategory = NULL;
	_overrideCategory = new OverrideCategory();
	_overrideCategory->tlv->tag_class = CONTEXT_SPECIFIC;
	_overrideCategory->tlv->tag_value = 2;
	children.push_back(_overrideCategory);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);


}
asn1::ClipData::~ClipData(){

}

asn1::ASN1Node* asn1::ClipData::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 3;
			children[2] = _notificationToCSE;
			return _notificationToCSE;

		default: return NULL;
	}
}

void asn1::ClipData::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(2);
}

//ClirData
asn1::ClirData::ClirData(){
	strcpy(node_type_name, "ClirData");
	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 1;
	children.push_back(_ss_Status);

	// cliRestrictionOption
	_cliRestrictionOption = NULL;
	children.push_back(_cliRestrictionOption);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);


}
asn1::ClirData::~ClirData(){

}

asn1::ASN1Node* asn1::ClirData::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_cliRestrictionOption = new CliRestrictionOption();
			_cliRestrictionOption->tlv->tag_class = CONTEXT_SPECIFIC;
			_cliRestrictionOption->tlv->tag_value = 2;
			children[1] = _cliRestrictionOption;
			return _cliRestrictionOption;
		case 2:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 3;
			children[2] = _notificationToCSE;
			return _notificationToCSE;

		default: return NULL;
	}
}

void asn1::ClirData::set_cliRestrictionOption(){
	if(_cliRestrictionOption == NULL) _cliRestrictionOption = (CliRestrictionOption*)create_node(1);
}

void asn1::ClirData::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(2);
}

//CallHoldData
asn1::CallHoldData::CallHoldData(){
	strcpy(node_type_name, "CallHoldData");
	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 1;
	children.push_back(_ss_Status);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);


}
asn1::CallHoldData::~CallHoldData(){

}

asn1::ASN1Node* asn1::CallHoldData::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 2;
			children[1] = _notificationToCSE;
			return _notificationToCSE;

		default: return NULL;
	}
}

void asn1::CallHoldData::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(1);
}

//EctData
asn1::EctData::EctData(){
	strcpy(node_type_name, "EctData");
	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new Ext_SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 1;
	children.push_back(_ss_Status);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);


}
asn1::EctData::~EctData(){

}

asn1::ASN1Node* asn1::EctData::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 2;
			children[1] = _notificationToCSE;
			return _notificationToCSE;

		default: return NULL;
	}
}

void asn1::EctData::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(1);
}

//RequestedSubscriptionInfo
asn1::RequestedSubscriptionInfo::RequestedSubscriptionInfo(){
	strcpy(node_type_name, "RequestedSubscriptionInfo");
	// requestedSS_Info
	_requestedSS_Info = NULL;
	children.push_back(_requestedSS_Info);

	// odb
	_odb = NULL;
	children.push_back(_odb);

	// requestedCAMEL_SubscriptionInfo
	_requestedCAMEL_SubscriptionInfo = NULL;
	children.push_back(_requestedCAMEL_SubscriptionInfo);

	// supportedVLR_CAMEL_Phases
	_supportedVLR_CAMEL_Phases = NULL;
	children.push_back(_supportedVLR_CAMEL_Phases);

	// supportedSGSN_CAMEL_Phases
	_supportedSGSN_CAMEL_Phases = NULL;
	children.push_back(_supportedSGSN_CAMEL_Phases);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// additionalRequestedCAMEL_SubscriptionInfo
	_additionalRequestedCAMEL_SubscriptionInfo = NULL;
	children.push_back(_additionalRequestedCAMEL_SubscriptionInfo);

	// msisdn_BS_List
	_msisdn_BS_List = NULL;
	children.push_back(_msisdn_BS_List);

	// csg_SubscriptionDataRequested
	_csg_SubscriptionDataRequested = NULL;
	children.push_back(_csg_SubscriptionDataRequested);

	// cw_Info
	_cw_Info = NULL;
	children.push_back(_cw_Info);

	// clip_Info
	_clip_Info = NULL;
	children.push_back(_clip_Info);

	// clir_Info
	_clir_Info = NULL;
	children.push_back(_clir_Info);

	// hold_Info
	_hold_Info = NULL;
	children.push_back(_hold_Info);

	// ect_Info
	_ect_Info = NULL;
	children.push_back(_ect_Info);


}
asn1::RequestedSubscriptionInfo::~RequestedSubscriptionInfo(){

}

asn1::ASN1Node* asn1::RequestedSubscriptionInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_requestedSS_Info = new SS_ForBS_Code();
			_requestedSS_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_requestedSS_Info->tlv->tag_value = 1;
			children[0] = _requestedSS_Info;
			return _requestedSS_Info;
		case 1:
			_odb = new Null();
			_odb->tlv->tag_class = CONTEXT_SPECIFIC;
			_odb->tlv->tag_value = 2;
			children[1] = _odb;
			return _odb;
		case 2:
			_requestedCAMEL_SubscriptionInfo = new RequestedCAMEL_SubscriptionInfo();
			_requestedCAMEL_SubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_requestedCAMEL_SubscriptionInfo->tlv->tag_value = 3;
			children[2] = _requestedCAMEL_SubscriptionInfo;
			return _requestedCAMEL_SubscriptionInfo;
		case 3:
			_supportedVLR_CAMEL_Phases = new Null();
			_supportedVLR_CAMEL_Phases->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedVLR_CAMEL_Phases->tlv->tag_value = 4;
			children[3] = _supportedVLR_CAMEL_Phases;
			return _supportedVLR_CAMEL_Phases;
		case 4:
			_supportedSGSN_CAMEL_Phases = new Null();
			_supportedSGSN_CAMEL_Phases->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedSGSN_CAMEL_Phases->tlv->tag_value = 5;
			children[4] = _supportedSGSN_CAMEL_Phases;
			return _supportedSGSN_CAMEL_Phases;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 6;
			children[5] = _extensionContainer;
			return _extensionContainer;
		case 6:
			_additionalRequestedCAMEL_SubscriptionInfo = new AdditionalRequestedCAMEL_SubscriptionInfo();
			_additionalRequestedCAMEL_SubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalRequestedCAMEL_SubscriptionInfo->tlv->tag_value = 7;
			children[6] = _additionalRequestedCAMEL_SubscriptionInfo;
			return _additionalRequestedCAMEL_SubscriptionInfo;
		case 7:
			_msisdn_BS_List = new Null();
			_msisdn_BS_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn_BS_List->tlv->tag_value = 8;
			children[7] = _msisdn_BS_List;
			return _msisdn_BS_List;
		case 8:
			_csg_SubscriptionDataRequested = new Null();
			_csg_SubscriptionDataRequested->tlv->tag_class = CONTEXT_SPECIFIC;
			_csg_SubscriptionDataRequested->tlv->tag_value = 9;
			children[8] = _csg_SubscriptionDataRequested;
			return _csg_SubscriptionDataRequested;
		case 9:
			_cw_Info = new Null();
			_cw_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_cw_Info->tlv->tag_value = 10;
			children[9] = _cw_Info;
			return _cw_Info;
		case 10:
			_clip_Info = new Null();
			_clip_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_clip_Info->tlv->tag_value = 11;
			children[10] = _clip_Info;
			return _clip_Info;
		case 11:
			_clir_Info = new Null();
			_clir_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_clir_Info->tlv->tag_value = 12;
			children[11] = _clir_Info;
			return _clir_Info;
		case 12:
			_hold_Info = new Null();
			_hold_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_hold_Info->tlv->tag_value = 13;
			children[12] = _hold_Info;
			return _hold_Info;
		case 13:
			_ect_Info = new Null();
			_ect_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_ect_Info->tlv->tag_value = 14;
			children[13] = _ect_Info;
			return _ect_Info;

		default: return NULL;
	}
}

void asn1::RequestedSubscriptionInfo::set_requestedSS_Info(){
	if(_requestedSS_Info == NULL) _requestedSS_Info = (SS_ForBS_Code*)create_node(0);
}

void asn1::RequestedSubscriptionInfo::set_odb(){
	if(_odb == NULL) _odb = (Null*)create_node(1);
}

void asn1::RequestedSubscriptionInfo::set_requestedCAMEL_SubscriptionInfo(){
	if(_requestedCAMEL_SubscriptionInfo == NULL) _requestedCAMEL_SubscriptionInfo = (RequestedCAMEL_SubscriptionInfo*)create_node(2);
}

void asn1::RequestedSubscriptionInfo::set_supportedVLR_CAMEL_Phases(){
	if(_supportedVLR_CAMEL_Phases == NULL) _supportedVLR_CAMEL_Phases = (Null*)create_node(3);
}

void asn1::RequestedSubscriptionInfo::set_supportedSGSN_CAMEL_Phases(){
	if(_supportedSGSN_CAMEL_Phases == NULL) _supportedSGSN_CAMEL_Phases = (Null*)create_node(4);
}

void asn1::RequestedSubscriptionInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

void asn1::RequestedSubscriptionInfo::set_additionalRequestedCAMEL_SubscriptionInfo(){
	if(_additionalRequestedCAMEL_SubscriptionInfo == NULL) _additionalRequestedCAMEL_SubscriptionInfo = (AdditionalRequestedCAMEL_SubscriptionInfo*)create_node(6);
}

void asn1::RequestedSubscriptionInfo::set_msisdn_BS_List(){
	if(_msisdn_BS_List == NULL) _msisdn_BS_List = (Null*)create_node(7);
}

void asn1::RequestedSubscriptionInfo::set_csg_SubscriptionDataRequested(){
	if(_csg_SubscriptionDataRequested == NULL) _csg_SubscriptionDataRequested = (Null*)create_node(8);
}

void asn1::RequestedSubscriptionInfo::set_cw_Info(){
	if(_cw_Info == NULL) _cw_Info = (Null*)create_node(9);
}

void asn1::RequestedSubscriptionInfo::set_clip_Info(){
	if(_clip_Info == NULL) _clip_Info = (Null*)create_node(10);
}

void asn1::RequestedSubscriptionInfo::set_clir_Info(){
	if(_clir_Info == NULL) _clir_Info = (Null*)create_node(11);
}

void asn1::RequestedSubscriptionInfo::set_hold_Info(){
	if(_hold_Info == NULL) _hold_Info = (Null*)create_node(12);
}

void asn1::RequestedSubscriptionInfo::set_ect_Info(){
	if(_ect_Info == NULL) _ect_Info = (Null*)create_node(13);
}

//MSISDN_BS_List
asn1::MSISDN_BS_List::MSISDN_BS_List(){
	strcpy(node_type_name, "MSISDN_BS_List");

}
asn1::MSISDN_BS_List::~MSISDN_BS_List(){
}

asn1::MSISDN_BS* asn1::MSISDN_BS_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (MSISDN_BS*)children[child_index]; else return NULL;
}

void asn1::MSISDN_BS_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::MSISDN_BS_List::create_node(unsigned int _index){
	children[_index] = new MSISDN_BS();
	return children[_index];
}

asn1::ASN1Node* asn1::MSISDN_BS_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new MSISDN_BS());
		return children[children.size() - 1];
	}

}

//maxNumOfMSISDN
asn1::maxNumOfMSISDN::maxNumOfMSISDN(){
	strcpy(node_type_name, "maxNumOfMSISDN");

}
asn1::maxNumOfMSISDN::~maxNumOfMSISDN(){

}

//MSISDN_BS
asn1::MSISDN_BS::MSISDN_BS(){
	strcpy(node_type_name, "MSISDN_BS");
	// msisdn
	_msisdn = NULL;
	_msisdn = new ISDN_AddressString();
	children.push_back(_msisdn);

	// basicServiceList
	_basicServiceList = NULL;
	children.push_back(_basicServiceList);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MSISDN_BS::~MSISDN_BS(){

}

asn1::ASN1Node* asn1::MSISDN_BS::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_basicServiceList = new BasicServiceList();
			_basicServiceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_basicServiceList->tlv->tag_value = 0;
			children[1] = _basicServiceList;
			return _basicServiceList;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::MSISDN_BS::set_basicServiceList(){
	if(_basicServiceList == NULL) _basicServiceList = (BasicServiceList*)create_node(1);
}

void asn1::MSISDN_BS::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//RequestedCAMEL_SubscriptionInfo
asn1::RequestedCAMEL_SubscriptionInfo::RequestedCAMEL_SubscriptionInfo(){
	strcpy(node_type_name, "RequestedCAMEL_SubscriptionInfo");

}
asn1::RequestedCAMEL_SubscriptionInfo::~RequestedCAMEL_SubscriptionInfo(){

}

//AdditionalRequestedCAMEL_SubscriptionInfo
asn1::AdditionalRequestedCAMEL_SubscriptionInfo::AdditionalRequestedCAMEL_SubscriptionInfo(){
	strcpy(node_type_name, "AdditionalRequestedCAMEL_SubscriptionInfo");

}
asn1::AdditionalRequestedCAMEL_SubscriptionInfo::~AdditionalRequestedCAMEL_SubscriptionInfo(){

}

//CallForwardingData
asn1::CallForwardingData::CallForwardingData(){
	strcpy(node_type_name, "CallForwardingData");
	// forwardingFeatureList
	_forwardingFeatureList = NULL;
	_forwardingFeatureList = new Ext_ForwFeatureList();
	children.push_back(_forwardingFeatureList);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CallForwardingData::~CallForwardingData(){

}

asn1::ASN1Node* asn1::CallForwardingData::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_notificationToCSE = new Null();
			children[1] = _notificationToCSE;
			return _notificationToCSE;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CallForwardingData::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(1);
}

void asn1::CallForwardingData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//CallBarringData
asn1::CallBarringData::CallBarringData(){
	strcpy(node_type_name, "CallBarringData");
	// callBarringFeatureList
	_callBarringFeatureList = NULL;
	_callBarringFeatureList = new Ext_CallBarFeatureList();
	children.push_back(_callBarringFeatureList);

	// password
	_password = NULL;
	children.push_back(_password);

	// wrongPasswordAttemptsCounter
	_wrongPasswordAttemptsCounter = NULL;
	children.push_back(_wrongPasswordAttemptsCounter);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CallBarringData::~CallBarringData(){

}

asn1::ASN1Node* asn1::CallBarringData::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_password = new Password();
			children[1] = _password;
			return _password;
		case 2:
			_wrongPasswordAttemptsCounter = new WrongPasswordAttemptsCounter();
			children[2] = _wrongPasswordAttemptsCounter;
			return _wrongPasswordAttemptsCounter;
		case 3:
			_notificationToCSE = new Null();
			children[3] = _notificationToCSE;
			return _notificationToCSE;
		case 4:
			_extensionContainer = new ExtensionContainer();
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CallBarringData::set_password(){
	if(_password == NULL) _password = (Password*)create_node(1);
}

void asn1::CallBarringData::set_wrongPasswordAttemptsCounter(){
	if(_wrongPasswordAttemptsCounter == NULL) _wrongPasswordAttemptsCounter = (WrongPasswordAttemptsCounter*)create_node(2);
}

void asn1::CallBarringData::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(3);
}

void asn1::CallBarringData::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//WrongPasswordAttemptsCounter
asn1::WrongPasswordAttemptsCounter::WrongPasswordAttemptsCounter(){
	strcpy(node_type_name, "WrongPasswordAttemptsCounter");

}
asn1::WrongPasswordAttemptsCounter::~WrongPasswordAttemptsCounter(){

}

//ODB_Info
asn1::ODB_Info::ODB_Info(){
	strcpy(node_type_name, "ODB_Info");
	// odb_Data
	_odb_Data = NULL;
	_odb_Data = new ODB_Data();
	children.push_back(_odb_Data);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ODB_Info::~ODB_Info(){

}

asn1::ASN1Node* asn1::ODB_Info::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_notificationToCSE = new Null();
			children[1] = _notificationToCSE;
			return _notificationToCSE;
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ODB_Info::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(1);
}

void asn1::ODB_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//CAMEL_SubscriptionInfo
asn1::CAMEL_SubscriptionInfo::CAMEL_SubscriptionInfo(){
	strcpy(node_type_name, "CAMEL_SubscriptionInfo");
	// o_CSI
	_o_CSI = NULL;
	children.push_back(_o_CSI);

	// o_BcsmCamelTDP_CriteriaList
	_o_BcsmCamelTDP_CriteriaList = NULL;
	children.push_back(_o_BcsmCamelTDP_CriteriaList);

	// d_CSI
	_d_CSI = NULL;
	children.push_back(_d_CSI);

	// t_CSI
	_t_CSI = NULL;
	children.push_back(_t_CSI);

	// t_BCSM_CAMEL_TDP_CriteriaList
	_t_BCSM_CAMEL_TDP_CriteriaList = NULL;
	children.push_back(_t_BCSM_CAMEL_TDP_CriteriaList);

	// vt_CSI
	_vt_CSI = NULL;
	children.push_back(_vt_CSI);

	// vt_BCSM_CAMEL_TDP_CriteriaList
	_vt_BCSM_CAMEL_TDP_CriteriaList = NULL;
	children.push_back(_vt_BCSM_CAMEL_TDP_CriteriaList);

	// tif_CSI
	_tif_CSI = NULL;
	children.push_back(_tif_CSI);

	// tif_CSI_NotificationToCSE
	_tif_CSI_NotificationToCSE = NULL;
	children.push_back(_tif_CSI_NotificationToCSE);

	// gprs_CSI
	_gprs_CSI = NULL;
	children.push_back(_gprs_CSI);

	// mo_sms_CSI
	_mo_sms_CSI = NULL;
	children.push_back(_mo_sms_CSI);

	// ss_CSI
	_ss_CSI = NULL;
	children.push_back(_ss_CSI);

	// m_CSI
	_m_CSI = NULL;
	children.push_back(_m_CSI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// specificCSIDeletedList
	_specificCSIDeletedList = NULL;
	children.push_back(_specificCSIDeletedList);

	// mt_sms_CSI
	_mt_sms_CSI = NULL;
	children.push_back(_mt_sms_CSI);

	// mt_smsCAMELTDP_CriteriaList
	_mt_smsCAMELTDP_CriteriaList = NULL;
	children.push_back(_mt_smsCAMELTDP_CriteriaList);

	// mg_csi
	_mg_csi = NULL;
	children.push_back(_mg_csi);

	// o_IM_CSI
	_o_IM_CSI = NULL;
	children.push_back(_o_IM_CSI);

	// o_IM_BcsmCamelTDP_CriteriaList
	_o_IM_BcsmCamelTDP_CriteriaList = NULL;
	children.push_back(_o_IM_BcsmCamelTDP_CriteriaList);

	// d_IM_CSI
	_d_IM_CSI = NULL;
	children.push_back(_d_IM_CSI);

	// vt_IM_CSI
	_vt_IM_CSI = NULL;
	children.push_back(_vt_IM_CSI);

	// vt_IM_BCSM_CAMEL_TDP_CriteriaList
	_vt_IM_BCSM_CAMEL_TDP_CriteriaList = NULL;
	children.push_back(_vt_IM_BCSM_CAMEL_TDP_CriteriaList);


}
asn1::CAMEL_SubscriptionInfo::~CAMEL_SubscriptionInfo(){

}

asn1::ASN1Node* asn1::CAMEL_SubscriptionInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_o_CSI = new O_CSI();
			_o_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_CSI->tlv->tag_value = 0;
			children[0] = _o_CSI;
			return _o_CSI;
		case 1:
			_o_BcsmCamelTDP_CriteriaList = new O_BcsmCamelTDPCriteriaList();
			_o_BcsmCamelTDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_BcsmCamelTDP_CriteriaList->tlv->tag_value = 1;
			children[1] = _o_BcsmCamelTDP_CriteriaList;
			return _o_BcsmCamelTDP_CriteriaList;
		case 2:
			_d_CSI = new D_CSI();
			_d_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_d_CSI->tlv->tag_value = 2;
			children[2] = _d_CSI;
			return _d_CSI;
		case 3:
			_t_CSI = new T_CSI();
			_t_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_t_CSI->tlv->tag_value = 3;
			children[3] = _t_CSI;
			return _t_CSI;
		case 4:
			_t_BCSM_CAMEL_TDP_CriteriaList = new T_BCSM_CAMEL_TDP_CriteriaList();
			_t_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_t_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_value = 4;
			children[4] = _t_BCSM_CAMEL_TDP_CriteriaList;
			return _t_BCSM_CAMEL_TDP_CriteriaList;
		case 5:
			_vt_CSI = new T_CSI();
			_vt_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_vt_CSI->tlv->tag_value = 5;
			children[5] = _vt_CSI;
			return _vt_CSI;
		case 6:
			_vt_BCSM_CAMEL_TDP_CriteriaList = new T_BCSM_CAMEL_TDP_CriteriaList();
			_vt_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_vt_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_value = 6;
			children[6] = _vt_BCSM_CAMEL_TDP_CriteriaList;
			return _vt_BCSM_CAMEL_TDP_CriteriaList;
		case 7:
			_tif_CSI = new Null();
			_tif_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_tif_CSI->tlv->tag_value = 7;
			children[7] = _tif_CSI;
			return _tif_CSI;
		case 8:
			_tif_CSI_NotificationToCSE = new Null();
			_tif_CSI_NotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_tif_CSI_NotificationToCSE->tlv->tag_value = 8;
			children[8] = _tif_CSI_NotificationToCSE;
			return _tif_CSI_NotificationToCSE;
		case 9:
			_gprs_CSI = new GPRS_CSI();
			_gprs_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_gprs_CSI->tlv->tag_value = 9;
			children[9] = _gprs_CSI;
			return _gprs_CSI;
		case 10:
			_mo_sms_CSI = new SMS_CSI();
			_mo_sms_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_mo_sms_CSI->tlv->tag_value = 10;
			children[10] = _mo_sms_CSI;
			return _mo_sms_CSI;
		case 11:
			_ss_CSI = new SS_CSI();
			_ss_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_CSI->tlv->tag_value = 11;
			children[11] = _ss_CSI;
			return _ss_CSI;
		case 12:
			_m_CSI = new M_CSI();
			_m_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_m_CSI->tlv->tag_value = 12;
			children[12] = _m_CSI;
			return _m_CSI;
		case 13:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 13;
			children[13] = _extensionContainer;
			return _extensionContainer;
		case 14:
			_specificCSIDeletedList = new SpecificCSI_Withdraw();
			_specificCSIDeletedList->tlv->tag_class = CONTEXT_SPECIFIC;
			_specificCSIDeletedList->tlv->tag_value = 14;
			children[14] = _specificCSIDeletedList;
			return _specificCSIDeletedList;
		case 15:
			_mt_sms_CSI = new SMS_CSI();
			_mt_sms_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_mt_sms_CSI->tlv->tag_value = 15;
			children[15] = _mt_sms_CSI;
			return _mt_sms_CSI;
		case 16:
			_mt_smsCAMELTDP_CriteriaList = new MT_smsCAMELTDP_CriteriaList();
			_mt_smsCAMELTDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_mt_smsCAMELTDP_CriteriaList->tlv->tag_value = 16;
			children[16] = _mt_smsCAMELTDP_CriteriaList;
			return _mt_smsCAMELTDP_CriteriaList;
		case 17:
			_mg_csi = new MG_CSI();
			_mg_csi->tlv->tag_class = CONTEXT_SPECIFIC;
			_mg_csi->tlv->tag_value = 17;
			children[17] = _mg_csi;
			return _mg_csi;
		case 18:
			_o_IM_CSI = new O_CSI();
			_o_IM_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_IM_CSI->tlv->tag_value = 18;
			children[18] = _o_IM_CSI;
			return _o_IM_CSI;
		case 19:
			_o_IM_BcsmCamelTDP_CriteriaList = new O_BcsmCamelTDPCriteriaList();
			_o_IM_BcsmCamelTDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_o_IM_BcsmCamelTDP_CriteriaList->tlv->tag_value = 19;
			children[19] = _o_IM_BcsmCamelTDP_CriteriaList;
			return _o_IM_BcsmCamelTDP_CriteriaList;
		case 20:
			_d_IM_CSI = new D_CSI();
			_d_IM_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_d_IM_CSI->tlv->tag_value = 20;
			children[20] = _d_IM_CSI;
			return _d_IM_CSI;
		case 21:
			_vt_IM_CSI = new T_CSI();
			_vt_IM_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_vt_IM_CSI->tlv->tag_value = 21;
			children[21] = _vt_IM_CSI;
			return _vt_IM_CSI;
		case 22:
			_vt_IM_BCSM_CAMEL_TDP_CriteriaList = new T_BCSM_CAMEL_TDP_CriteriaList();
			_vt_IM_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_class = CONTEXT_SPECIFIC;
			_vt_IM_BCSM_CAMEL_TDP_CriteriaList->tlv->tag_value = 22;
			children[22] = _vt_IM_BCSM_CAMEL_TDP_CriteriaList;
			return _vt_IM_BCSM_CAMEL_TDP_CriteriaList;

		default: return NULL;
	}
}

void asn1::CAMEL_SubscriptionInfo::set_o_CSI(){
	if(_o_CSI == NULL) _o_CSI = (O_CSI*)create_node(0);
}

void asn1::CAMEL_SubscriptionInfo::set_o_BcsmCamelTDP_CriteriaList(){
	if(_o_BcsmCamelTDP_CriteriaList == NULL) _o_BcsmCamelTDP_CriteriaList = (O_BcsmCamelTDPCriteriaList*)create_node(1);
}

void asn1::CAMEL_SubscriptionInfo::set_d_CSI(){
	if(_d_CSI == NULL) _d_CSI = (D_CSI*)create_node(2);
}

void asn1::CAMEL_SubscriptionInfo::set_t_CSI(){
	if(_t_CSI == NULL) _t_CSI = (T_CSI*)create_node(3);
}

void asn1::CAMEL_SubscriptionInfo::set_t_BCSM_CAMEL_TDP_CriteriaList(){
	if(_t_BCSM_CAMEL_TDP_CriteriaList == NULL) _t_BCSM_CAMEL_TDP_CriteriaList = (T_BCSM_CAMEL_TDP_CriteriaList*)create_node(4);
}

void asn1::CAMEL_SubscriptionInfo::set_vt_CSI(){
	if(_vt_CSI == NULL) _vt_CSI = (T_CSI*)create_node(5);
}

void asn1::CAMEL_SubscriptionInfo::set_vt_BCSM_CAMEL_TDP_CriteriaList(){
	if(_vt_BCSM_CAMEL_TDP_CriteriaList == NULL) _vt_BCSM_CAMEL_TDP_CriteriaList = (T_BCSM_CAMEL_TDP_CriteriaList*)create_node(6);
}

void asn1::CAMEL_SubscriptionInfo::set_tif_CSI(){
	if(_tif_CSI == NULL) _tif_CSI = (Null*)create_node(7);
}

void asn1::CAMEL_SubscriptionInfo::set_tif_CSI_NotificationToCSE(){
	if(_tif_CSI_NotificationToCSE == NULL) _tif_CSI_NotificationToCSE = (Null*)create_node(8);
}

void asn1::CAMEL_SubscriptionInfo::set_gprs_CSI(){
	if(_gprs_CSI == NULL) _gprs_CSI = (GPRS_CSI*)create_node(9);
}

void asn1::CAMEL_SubscriptionInfo::set_mo_sms_CSI(){
	if(_mo_sms_CSI == NULL) _mo_sms_CSI = (SMS_CSI*)create_node(10);
}

void asn1::CAMEL_SubscriptionInfo::set_ss_CSI(){
	if(_ss_CSI == NULL) _ss_CSI = (SS_CSI*)create_node(11);
}

void asn1::CAMEL_SubscriptionInfo::set_m_CSI(){
	if(_m_CSI == NULL) _m_CSI = (M_CSI*)create_node(12);
}

void asn1::CAMEL_SubscriptionInfo::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(13);
}

void asn1::CAMEL_SubscriptionInfo::set_specificCSIDeletedList(){
	if(_specificCSIDeletedList == NULL) _specificCSIDeletedList = (SpecificCSI_Withdraw*)create_node(14);
}

void asn1::CAMEL_SubscriptionInfo::set_mt_sms_CSI(){
	if(_mt_sms_CSI == NULL) _mt_sms_CSI = (SMS_CSI*)create_node(15);
}

void asn1::CAMEL_SubscriptionInfo::set_mt_smsCAMELTDP_CriteriaList(){
	if(_mt_smsCAMELTDP_CriteriaList == NULL) _mt_smsCAMELTDP_CriteriaList = (MT_smsCAMELTDP_CriteriaList*)create_node(16);
}

void asn1::CAMEL_SubscriptionInfo::set_mg_csi(){
	if(_mg_csi == NULL) _mg_csi = (MG_CSI*)create_node(17);
}

void asn1::CAMEL_SubscriptionInfo::set_o_IM_CSI(){
	if(_o_IM_CSI == NULL) _o_IM_CSI = (O_CSI*)create_node(18);
}

void asn1::CAMEL_SubscriptionInfo::set_o_IM_BcsmCamelTDP_CriteriaList(){
	if(_o_IM_BcsmCamelTDP_CriteriaList == NULL) _o_IM_BcsmCamelTDP_CriteriaList = (O_BcsmCamelTDPCriteriaList*)create_node(19);
}

void asn1::CAMEL_SubscriptionInfo::set_d_IM_CSI(){
	if(_d_IM_CSI == NULL) _d_IM_CSI = (D_CSI*)create_node(20);
}

void asn1::CAMEL_SubscriptionInfo::set_vt_IM_CSI(){
	if(_vt_IM_CSI == NULL) _vt_IM_CSI = (T_CSI*)create_node(21);
}

void asn1::CAMEL_SubscriptionInfo::set_vt_IM_BCSM_CAMEL_TDP_CriteriaList(){
	if(_vt_IM_BCSM_CAMEL_TDP_CriteriaList == NULL) _vt_IM_BCSM_CAMEL_TDP_CriteriaList = (T_BCSM_CAMEL_TDP_CriteriaList*)create_node(22);
}

//AnyTimeModificationArg
asn1::AnyTimeModificationArg::AnyTimeModificationArg(){
	strcpy(node_type_name, "AnyTimeModificationArg");
        // subscriberIdentity
        _subscriberIdentity = NULL;
        _subscriberIdentity = new SubscriberIdentity();
        ASN1Node* _subscriberIdentity_wrapper = new ASN1Node();
        _subscriberIdentity_wrapper->tlv = new TLVNode();
        _subscriberIdentity_wrapper->tlv->is_explicit = true;
        _subscriberIdentity_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
        _subscriberIdentity_wrapper->tlv->tag_value = 0;
        _subscriberIdentity_wrapper->children.push_back(_subscriberIdentity);
        children.push_back(_subscriberIdentity_wrapper);

	// gsmSCF_Address
	_gsmSCF_Address = NULL;
	_gsmSCF_Address = new ISDN_AddressString();
	_gsmSCF_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsmSCF_Address->tlv->tag_value = 1;
	children.push_back(_gsmSCF_Address);

	// modificationRequestFor_CF_Info
	_modificationRequestFor_CF_Info = NULL;
	children.push_back(_modificationRequestFor_CF_Info);

	// modificationRequestFor_CB_Info
	_modificationRequestFor_CB_Info = NULL;
	children.push_back(_modificationRequestFor_CB_Info);

	// modificationRequestFor_CSI
	_modificationRequestFor_CSI = NULL;
	children.push_back(_modificationRequestFor_CSI);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// longFTN_Supported
	_longFTN_Supported = NULL;
	children.push_back(_longFTN_Supported);

	// modificationRequestFor_ODB_data
	_modificationRequestFor_ODB_data = NULL;
	children.push_back(_modificationRequestFor_ODB_data);

	// modificationRequestFor_IP_SM_GW_Data
	_modificationRequestFor_IP_SM_GW_Data = NULL;
	children.push_back(_modificationRequestFor_IP_SM_GW_Data);

	// activationRequestForUE_reachability
	_activationRequestForUE_reachability = NULL;
	children.push_back(_activationRequestForUE_reachability);

	// modificationRequestFor_CSG
	_modificationRequestFor_CSG = NULL;
	children.push_back(_modificationRequestFor_CSG);

	// modificationRequestFor_CW_Data
	_modificationRequestFor_CW_Data = NULL;
	children.push_back(_modificationRequestFor_CW_Data);

	// modificationRequestFor_CLIP_Data
	_modificationRequestFor_CLIP_Data = NULL;
	children.push_back(_modificationRequestFor_CLIP_Data);

	// modificationRequestFor_CLIR_Data
	_modificationRequestFor_CLIR_Data = NULL;
	children.push_back(_modificationRequestFor_CLIR_Data);

	// modificationRequestFor_HOLD_Data
	_modificationRequestFor_HOLD_Data = NULL;
	children.push_back(_modificationRequestFor_HOLD_Data);

	// modificationRequestFor_ECT_Data
	_modificationRequestFor_ECT_Data = NULL;
	children.push_back(_modificationRequestFor_ECT_Data);


}
asn1::AnyTimeModificationArg::~AnyTimeModificationArg(){

}

asn1::ASN1Node* asn1::AnyTimeModificationArg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_modificationRequestFor_CF_Info = new ModificationRequestFor_CF_Info();
			_modificationRequestFor_CF_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_CF_Info->tlv->tag_value = 2;
			children[2] = _modificationRequestFor_CF_Info;
			return _modificationRequestFor_CF_Info;
		case 3:
			_modificationRequestFor_CB_Info = new ModificationRequestFor_CB_Info();
			_modificationRequestFor_CB_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_CB_Info->tlv->tag_value = 3;
			children[3] = _modificationRequestFor_CB_Info;
			return _modificationRequestFor_CB_Info;
		case 4:
			_modificationRequestFor_CSI = new ModificationRequestFor_CSI();
			_modificationRequestFor_CSI->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_CSI->tlv->tag_value = 4;
			children[4] = _modificationRequestFor_CSI;
			return _modificationRequestFor_CSI;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 5;
			children[5] = _extensionContainer;
			return _extensionContainer;
		case 6:
			_longFTN_Supported = new Null();
			_longFTN_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_longFTN_Supported->tlv->tag_value = 6;
			children[6] = _longFTN_Supported;
			return _longFTN_Supported;
		case 7:
			_modificationRequestFor_ODB_data = new ModificationRequestFor_ODB_data();
			_modificationRequestFor_ODB_data->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_ODB_data->tlv->tag_value = 7;
			children[7] = _modificationRequestFor_ODB_data;
			return _modificationRequestFor_ODB_data;
		case 8:
			_modificationRequestFor_IP_SM_GW_Data = new ModificationRequestFor_IP_SM_GW_Data();
			_modificationRequestFor_IP_SM_GW_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_IP_SM_GW_Data->tlv->tag_value = 8;
			children[8] = _modificationRequestFor_IP_SM_GW_Data;
			return _modificationRequestFor_IP_SM_GW_Data;
		case 9:
			_activationRequestForUE_reachability = new RequestedServingNode();
			_activationRequestForUE_reachability->tlv->tag_class = CONTEXT_SPECIFIC;
			_activationRequestForUE_reachability->tlv->tag_value = 9;
			children[9] = _activationRequestForUE_reachability;
			return _activationRequestForUE_reachability;
		case 10:
			_modificationRequestFor_CSG = new ModificationRequestFor_CSG();
			_modificationRequestFor_CSG->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_CSG->tlv->tag_value = 10;
			children[10] = _modificationRequestFor_CSG;
			return _modificationRequestFor_CSG;
		case 11:
			_modificationRequestFor_CW_Data = new ModificationRequestFor_CW_Info();
			_modificationRequestFor_CW_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_CW_Data->tlv->tag_value = 11;
			children[11] = _modificationRequestFor_CW_Data;
			return _modificationRequestFor_CW_Data;
		case 12:
			_modificationRequestFor_CLIP_Data = new ModificationRequestFor_CLIP_Info();
			_modificationRequestFor_CLIP_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_CLIP_Data->tlv->tag_value = 12;
			children[12] = _modificationRequestFor_CLIP_Data;
			return _modificationRequestFor_CLIP_Data;
		case 13:
			_modificationRequestFor_CLIR_Data = new ModificationRequestFor_CLIR_Info();
			_modificationRequestFor_CLIR_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_CLIR_Data->tlv->tag_value = 13;
			children[13] = _modificationRequestFor_CLIR_Data;
			return _modificationRequestFor_CLIR_Data;
		case 14:
			_modificationRequestFor_HOLD_Data = new ModificationRequestFor_CH_Info();
			_modificationRequestFor_HOLD_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_HOLD_Data->tlv->tag_value = 14;
			children[14] = _modificationRequestFor_HOLD_Data;
			return _modificationRequestFor_HOLD_Data;
		case 15:
			_modificationRequestFor_ECT_Data = new ModificationRequestFor_ECT_Info();
			_modificationRequestFor_ECT_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_modificationRequestFor_ECT_Data->tlv->tag_value = 15;
			children[15] = _modificationRequestFor_ECT_Data;
			return _modificationRequestFor_ECT_Data;

		default: return NULL;
	}
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_CF_Info(){
	if(_modificationRequestFor_CF_Info == NULL) _modificationRequestFor_CF_Info = (ModificationRequestFor_CF_Info*)create_node(2);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_CB_Info(){
	if(_modificationRequestFor_CB_Info == NULL) _modificationRequestFor_CB_Info = (ModificationRequestFor_CB_Info*)create_node(3);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_CSI(){
	if(_modificationRequestFor_CSI == NULL) _modificationRequestFor_CSI = (ModificationRequestFor_CSI*)create_node(4);
}

void asn1::AnyTimeModificationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

void asn1::AnyTimeModificationArg::set_longFTN_Supported(){
	if(_longFTN_Supported == NULL) _longFTN_Supported = (Null*)create_node(6);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_ODB_data(){
	if(_modificationRequestFor_ODB_data == NULL) _modificationRequestFor_ODB_data = (ModificationRequestFor_ODB_data*)create_node(7);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_IP_SM_GW_Data(){
	if(_modificationRequestFor_IP_SM_GW_Data == NULL) _modificationRequestFor_IP_SM_GW_Data = (ModificationRequestFor_IP_SM_GW_Data*)create_node(8);
}

void asn1::AnyTimeModificationArg::set_activationRequestForUE_reachability(){
	if(_activationRequestForUE_reachability == NULL) _activationRequestForUE_reachability = (RequestedServingNode*)create_node(9);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_CSG(){
	if(_modificationRequestFor_CSG == NULL) _modificationRequestFor_CSG = (ModificationRequestFor_CSG*)create_node(10);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_CW_Data(){
	if(_modificationRequestFor_CW_Data == NULL) _modificationRequestFor_CW_Data = (ModificationRequestFor_CW_Info*)create_node(11);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_CLIP_Data(){
	if(_modificationRequestFor_CLIP_Data == NULL) _modificationRequestFor_CLIP_Data = (ModificationRequestFor_CLIP_Info*)create_node(12);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_CLIR_Data(){
	if(_modificationRequestFor_CLIR_Data == NULL) _modificationRequestFor_CLIR_Data = (ModificationRequestFor_CLIR_Info*)create_node(13);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_HOLD_Data(){
	if(_modificationRequestFor_HOLD_Data == NULL) _modificationRequestFor_HOLD_Data = (ModificationRequestFor_CH_Info*)create_node(14);
}

void asn1::AnyTimeModificationArg::set_modificationRequestFor_ECT_Data(){
	if(_modificationRequestFor_ECT_Data == NULL) _modificationRequestFor_ECT_Data = (ModificationRequestFor_ECT_Info*)create_node(15);
}

//ModificationRequestFor_CW_Info
asn1::ModificationRequestFor_CW_Info::ModificationRequestFor_CW_Info(){
	strcpy(node_type_name, "ModificationRequestFor_CW_Info");
	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);

	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ModificationRequestFor_CW_Info::~ModificationRequestFor_CW_Info(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_CW_Info::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_basicService = new Ext_BasicServiceCode();
			ASN1Node* _basicService_wrapper = new ASN1Node();
			_basicService_wrapper->tlv = new TLVNode();
			_basicService_wrapper->tlv->is_explicit = true;
			_basicService_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
			_basicService_wrapper->tlv->tag_value = 0;
			_basicService_wrapper->children.push_back(_basicService);
			children[0] = _basicService_wrapper;
			return _basicService;
		}
		case 1:
			_ss_Status = new Ext_SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 1;
			children[1] = _ss_Status;
			return _ss_Status;
		case 2:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 2;
			children[2] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_CW_Info::set_basicService(){
	if(_basicService == NULL) _basicService = (Ext_BasicServiceCode*)create_node(0);
}

void asn1::ModificationRequestFor_CW_Info::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (Ext_SS_Status*)create_node(1);
}

void asn1::ModificationRequestFor_CW_Info::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(2);
}

void asn1::ModificationRequestFor_CW_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//ModificationRequestFor_CH_Info
asn1::ModificationRequestFor_CH_Info::ModificationRequestFor_CH_Info(){
	strcpy(node_type_name, "ModificationRequestFor_CH_Info");
	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);

	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ModificationRequestFor_CH_Info::~ModificationRequestFor_CH_Info(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_CH_Info::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ss_Status = new Ext_SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 0;
			children[0] = _ss_Status;
			return _ss_Status;
		case 1:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 1;
			children[1] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_CH_Info::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (Ext_SS_Status*)create_node(0);
}

void asn1::ModificationRequestFor_CH_Info::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(1);
}

void asn1::ModificationRequestFor_CH_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//ModificationRequestFor_ECT_Info
asn1::ModificationRequestFor_ECT_Info::ModificationRequestFor_ECT_Info(){
	strcpy(node_type_name, "ModificationRequestFor_ECT_Info");
	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);

	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ModificationRequestFor_ECT_Info::~ModificationRequestFor_ECT_Info(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_ECT_Info::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ss_Status = new Ext_SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 0;
			children[0] = _ss_Status;
			return _ss_Status;
		case 1:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 1;
			children[1] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_ECT_Info::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (Ext_SS_Status*)create_node(0);
}

void asn1::ModificationRequestFor_ECT_Info::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(1);
}

void asn1::ModificationRequestFor_ECT_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//ModificationRequestFor_CLIR_Info
asn1::ModificationRequestFor_CLIR_Info::ModificationRequestFor_CLIR_Info(){
	strcpy(node_type_name, "ModificationRequestFor_CLIR_Info");
	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);

	// cliRestrictionOption
	_cliRestrictionOption = NULL;
	children.push_back(_cliRestrictionOption);

	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ModificationRequestFor_CLIR_Info::~ModificationRequestFor_CLIR_Info(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_CLIR_Info::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ss_Status = new Ext_SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 0;
			children[0] = _ss_Status;
			return _ss_Status;
		case 1:
			_cliRestrictionOption = new CliRestrictionOption();
			_cliRestrictionOption->tlv->tag_class = CONTEXT_SPECIFIC;
			_cliRestrictionOption->tlv->tag_value = 1;
			children[1] = _cliRestrictionOption;
			return _cliRestrictionOption;
		case 2:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 2;
			children[2] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_CLIR_Info::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (Ext_SS_Status*)create_node(0);
}

void asn1::ModificationRequestFor_CLIR_Info::set_cliRestrictionOption(){
	if(_cliRestrictionOption == NULL) _cliRestrictionOption = (CliRestrictionOption*)create_node(1);
}

void asn1::ModificationRequestFor_CLIR_Info::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(2);
}

void asn1::ModificationRequestFor_CLIR_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//ModificationRequestFor_CLIP_Info
asn1::ModificationRequestFor_CLIP_Info::ModificationRequestFor_CLIP_Info(){
	strcpy(node_type_name, "ModificationRequestFor_CLIP_Info");
	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);

	// overrideCategory
	_overrideCategory = NULL;
	children.push_back(_overrideCategory);

	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ModificationRequestFor_CLIP_Info::~ModificationRequestFor_CLIP_Info(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_CLIP_Info::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ss_Status = new Ext_SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 0;
			children[0] = _ss_Status;
			return _ss_Status;
		case 1:
			_overrideCategory = new OverrideCategory();
			_overrideCategory->tlv->tag_class = CONTEXT_SPECIFIC;
			_overrideCategory->tlv->tag_value = 1;
			children[1] = _overrideCategory;
			return _overrideCategory;
		case 2:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 2;
			children[2] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_CLIP_Info::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (Ext_SS_Status*)create_node(0);
}

void asn1::ModificationRequestFor_CLIP_Info::set_overrideCategory(){
	if(_overrideCategory == NULL) _overrideCategory = (OverrideCategory*)create_node(1);
}

void asn1::ModificationRequestFor_CLIP_Info::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(2);
}

void asn1::ModificationRequestFor_CLIP_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//ModificationRequestFor_CSG
asn1::ModificationRequestFor_CSG::ModificationRequestFor_CSG(){
	strcpy(node_type_name, "ModificationRequestFor_CSG");
	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ModificationRequestFor_CSG::~ModificationRequestFor_CSG(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_CSG::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 0;
			children[0] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_CSG::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(0);
}

void asn1::ModificationRequestFor_CSG::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//RequestedServingNode
asn1::RequestedServingNode::RequestedServingNode(){
	strcpy(node_type_name, "RequestedServingNode");

}
asn1::RequestedServingNode::~RequestedServingNode(){

}

//ServingNode
asn1::ServingNode::ServingNode(){
	strcpy(node_type_name, "ServingNode");

}
asn1::ServingNode::~ServingNode(){

}

//AnyTimeModificationRes
asn1::AnyTimeModificationRes::AnyTimeModificationRes(){
	strcpy(node_type_name, "AnyTimeModificationRes");
	// ss_InfoFor_CSE
	_ss_InfoFor_CSE = NULL;
	children.push_back(_ss_InfoFor_CSE);

	// camel_SubscriptionInfo
	_camel_SubscriptionInfo = NULL;
	children.push_back(_camel_SubscriptionInfo);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// odb_Info
	_odb_Info = NULL;
	children.push_back(_odb_Info);

	// cw_Data
	_cw_Data = NULL;
	children.push_back(_cw_Data);

	// ch_Data
	_ch_Data = NULL;
	children.push_back(_ch_Data);

	// clip_Data
	_clip_Data = NULL;
	children.push_back(_clip_Data);

	// clir_Data
	_clir_Data = NULL;
	children.push_back(_clir_Data);

	// ect_data
	_ect_data = NULL;
	children.push_back(_ect_data);

	// serviceCentreAddress
	_serviceCentreAddress = NULL;
	children.push_back(_serviceCentreAddress);


}
asn1::AnyTimeModificationRes::~AnyTimeModificationRes(){

}

asn1::ASN1Node* asn1::AnyTimeModificationRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_ss_InfoFor_CSE = new Ext_SS_InfoFor_CSE();
			ASN1Node* _ss_InfoFor_CSE_wrapper = new ASN1Node();
			_ss_InfoFor_CSE_wrapper->tlv = new TLVNode();
			_ss_InfoFor_CSE_wrapper->tlv->is_explicit = true;
			_ss_InfoFor_CSE_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_InfoFor_CSE_wrapper->tlv->tag_value = 0;
			_ss_InfoFor_CSE_wrapper->children.push_back(_ss_InfoFor_CSE);
			children[0] = _ss_InfoFor_CSE_wrapper;
			return _ss_InfoFor_CSE;
		}
		case 1:
			_camel_SubscriptionInfo = new CAMEL_SubscriptionInfo();
			_camel_SubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_camel_SubscriptionInfo->tlv->tag_value = 1;
			children[1] = _camel_SubscriptionInfo;
			return _camel_SubscriptionInfo;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		case 3:
			_odb_Info = new ODB_Info();
			_odb_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_odb_Info->tlv->tag_value = 3;
			children[3] = _odb_Info;
			return _odb_Info;
		case 4:
			_cw_Data = new CallWaitingData();
			_cw_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_cw_Data->tlv->tag_value = 4;
			children[4] = _cw_Data;
			return _cw_Data;
		case 5:
			_ch_Data = new CallHoldData();
			_ch_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_ch_Data->tlv->tag_value = 5;
			children[5] = _ch_Data;
			return _ch_Data;
		case 6:
			_clip_Data = new ClipData();
			_clip_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_clip_Data->tlv->tag_value = 6;
			children[6] = _clip_Data;
			return _clip_Data;
		case 7:
			_clir_Data = new ClirData();
			_clir_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_clir_Data->tlv->tag_value = 7;
			children[7] = _clir_Data;
			return _clir_Data;
		case 8:
			_ect_data = new EctData();
			_ect_data->tlv->tag_class = CONTEXT_SPECIFIC;
			_ect_data->tlv->tag_value = 8;
			children[8] = _ect_data;
			return _ect_data;
		case 9:
			_serviceCentreAddress = new AddressString();
			_serviceCentreAddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_serviceCentreAddress->tlv->tag_value = 9;
			children[9] = _serviceCentreAddress;
			return _serviceCentreAddress;

		default: return NULL;
	}
}

void asn1::AnyTimeModificationRes::set_ss_InfoFor_CSE(){
	if(_ss_InfoFor_CSE == NULL) _ss_InfoFor_CSE = (Ext_SS_InfoFor_CSE*)create_node(0);
}

void asn1::AnyTimeModificationRes::set_camel_SubscriptionInfo(){
	if(_camel_SubscriptionInfo == NULL) _camel_SubscriptionInfo = (CAMEL_SubscriptionInfo*)create_node(1);
}

void asn1::AnyTimeModificationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::AnyTimeModificationRes::set_odb_Info(){
	if(_odb_Info == NULL) _odb_Info = (ODB_Info*)create_node(3);
}

void asn1::AnyTimeModificationRes::set_cw_Data(){
	if(_cw_Data == NULL) _cw_Data = (CallWaitingData*)create_node(4);
}

void asn1::AnyTimeModificationRes::set_ch_Data(){
	if(_ch_Data == NULL) _ch_Data = (CallHoldData*)create_node(5);
}

void asn1::AnyTimeModificationRes::set_clip_Data(){
	if(_clip_Data == NULL) _clip_Data = (ClipData*)create_node(6);
}

void asn1::AnyTimeModificationRes::set_clir_Data(){
	if(_clir_Data == NULL) _clir_Data = (ClirData*)create_node(7);
}

void asn1::AnyTimeModificationRes::set_ect_data(){
	if(_ect_data == NULL) _ect_data = (EctData*)create_node(8);
}

void asn1::AnyTimeModificationRes::set_serviceCentreAddress(){
	if(_serviceCentreAddress == NULL) _serviceCentreAddress = (AddressString*)create_node(9);
}

//ModificationRequestFor_CF_Info
asn1::ModificationRequestFor_CF_Info::ModificationRequestFor_CF_Info(){
	strcpy(node_type_name, "ModificationRequestFor_CF_Info");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	_ss_Code->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Code->tlv->tag_value = 0;
	children.push_back(_ss_Code);

	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);

	// forwardedToNumber
	_forwardedToNumber = NULL;
	children.push_back(_forwardedToNumber);

	// forwardedToSubaddress
	_forwardedToSubaddress = NULL;
	children.push_back(_forwardedToSubaddress);

	// noReplyConditionTime
	_noReplyConditionTime = NULL;
	children.push_back(_noReplyConditionTime);

	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ModificationRequestFor_CF_Info::~ModificationRequestFor_CF_Info(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_CF_Info::create_node(unsigned int _index){
	switch(_index){
		case 1:
                {
                        _basicService = new Ext_BasicServiceCode();
                        ASN1Node* _basicService_wrapper = new ASN1Node();
                        _basicService_wrapper->tlv = new TLVNode();
                        _basicService_wrapper->tlv->is_explicit = true;
                        _basicService_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _basicService_wrapper->tlv->tag_value = 1;
                        _basicService_wrapper->children.push_back(_basicService);
                        children[1] = _basicService_wrapper;
                        return _basicService;
                }
		case 2:
			_ss_Status = new Ext_SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 2;
			children[2] = _ss_Status;
			return _ss_Status;
		case 3:
			_forwardedToNumber = new AddressString();
			_forwardedToNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardedToNumber->tlv->tag_value = 3;
			children[3] = _forwardedToNumber;
			return _forwardedToNumber;
		case 4:
			_forwardedToSubaddress = new ISDN_SubaddressString();
			_forwardedToSubaddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardedToSubaddress->tlv->tag_value = 4;
			children[4] = _forwardedToSubaddress;
			return _forwardedToSubaddress;
		case 5:
			_noReplyConditionTime = new Ext_NoRepCondTime();
			_noReplyConditionTime->tlv->tag_class = CONTEXT_SPECIFIC;
			_noReplyConditionTime->tlv->tag_value = 5;
			children[5] = _noReplyConditionTime;
			return _noReplyConditionTime;
		case 6:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 6;
			children[6] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 7:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 7;
			children[7] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_CF_Info::set_basicService(){
	if(_basicService == NULL) _basicService = (Ext_BasicServiceCode*)create_node(1);
}

void asn1::ModificationRequestFor_CF_Info::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (Ext_SS_Status*)create_node(2);
}

void asn1::ModificationRequestFor_CF_Info::set_forwardedToNumber(){
	if(_forwardedToNumber == NULL) _forwardedToNumber = (AddressString*)create_node(3);
}

void asn1::ModificationRequestFor_CF_Info::set_forwardedToSubaddress(){
	if(_forwardedToSubaddress == NULL) _forwardedToSubaddress = (ISDN_SubaddressString*)create_node(4);
}

void asn1::ModificationRequestFor_CF_Info::set_noReplyConditionTime(){
	if(_noReplyConditionTime == NULL) _noReplyConditionTime = (Ext_NoRepCondTime*)create_node(5);
}

void asn1::ModificationRequestFor_CF_Info::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(6);
}

void asn1::ModificationRequestFor_CF_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(7);
}

//ModificationRequestFor_CB_Info
asn1::ModificationRequestFor_CB_Info::ModificationRequestFor_CB_Info(){
	strcpy(node_type_name, "ModificationRequestFor_CB_Info");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	_ss_Code->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Code->tlv->tag_value = 0;
	children.push_back(_ss_Code);

	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);

	// password
	_password = NULL;
	children.push_back(_password);

	// wrongPasswordAttemptsCounter
	_wrongPasswordAttemptsCounter = NULL;
	children.push_back(_wrongPasswordAttemptsCounter);

	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ModificationRequestFor_CB_Info::~ModificationRequestFor_CB_Info(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_CB_Info::create_node(unsigned int _index){
	switch(_index){
		case 1:
                {
                        _basicService = new Ext_BasicServiceCode();
                        ASN1Node* _basicService_wrapper = new ASN1Node();
                        _basicService_wrapper->tlv = new TLVNode();
                        _basicService_wrapper->tlv->is_explicit = true;
                        _basicService_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
                        _basicService_wrapper->tlv->tag_value = 1;
                        _basicService_wrapper->children.push_back(_basicService);
                        children[1] = _basicService_wrapper;
                        return _basicService;
                }
		case 2:
			_ss_Status = new Ext_SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 2;
			children[2] = _ss_Status;
			return _ss_Status;
		case 3:
			_password = new Password();
			_password->tlv->tag_class = CONTEXT_SPECIFIC;
			_password->tlv->tag_value = 3;
			children[3] = _password;
			return _password;
		case 4:
			_wrongPasswordAttemptsCounter = new WrongPasswordAttemptsCounter();
			_wrongPasswordAttemptsCounter->tlv->tag_class = CONTEXT_SPECIFIC;
			_wrongPasswordAttemptsCounter->tlv->tag_value = 4;
			children[4] = _wrongPasswordAttemptsCounter;
			return _wrongPasswordAttemptsCounter;
		case 5:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 5;
			children[5] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 6:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 6;
			children[6] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_CB_Info::set_basicService(){
	if(_basicService == NULL) _basicService = (Ext_BasicServiceCode*)create_node(1);
}

void asn1::ModificationRequestFor_CB_Info::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (Ext_SS_Status*)create_node(2);
}

void asn1::ModificationRequestFor_CB_Info::set_password(){
	if(_password == NULL) _password = (Password*)create_node(3);
}

void asn1::ModificationRequestFor_CB_Info::set_wrongPasswordAttemptsCounter(){
	if(_wrongPasswordAttemptsCounter == NULL) _wrongPasswordAttemptsCounter = (WrongPasswordAttemptsCounter*)create_node(4);
}

void asn1::ModificationRequestFor_CB_Info::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(5);
}

void asn1::ModificationRequestFor_CB_Info::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(6);
}

//ModificationRequestFor_ODB_data
asn1::ModificationRequestFor_ODB_data::ModificationRequestFor_ODB_data(){
	strcpy(node_type_name, "ModificationRequestFor_ODB_data");
	// odb_data
	_odb_data = NULL;
	children.push_back(_odb_data);

	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::ModificationRequestFor_ODB_data::~ModificationRequestFor_ODB_data(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_ODB_data::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_odb_data = new ODB_Data();
			_odb_data->tlv->tag_class = CONTEXT_SPECIFIC;
			_odb_data->tlv->tag_value = 0;
			children[0] = _odb_data;
			return _odb_data;
		case 1:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 1;
			children[1] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_ODB_data::set_odb_data(){
	if(_odb_data == NULL) _odb_data = (ODB_Data*)create_node(0);
}

void asn1::ModificationRequestFor_ODB_data::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(1);
}

void asn1::ModificationRequestFor_ODB_data::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//ModificationRequestFor_CSI
asn1::ModificationRequestFor_CSI::ModificationRequestFor_CSI(){
	strcpy(node_type_name, "ModificationRequestFor_CSI");
	// requestedCamel_SubscriptionInfo
	_requestedCamel_SubscriptionInfo = NULL;
	_requestedCamel_SubscriptionInfo = new RequestedCAMEL_SubscriptionInfo();
	_requestedCamel_SubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_requestedCamel_SubscriptionInfo->tlv->tag_value = 0;
	children.push_back(_requestedCamel_SubscriptionInfo);

	// modifyNotificationToCSE
	_modifyNotificationToCSE = NULL;
	children.push_back(_modifyNotificationToCSE);

	// modifyCSI_State
	_modifyCSI_State = NULL;
	children.push_back(_modifyCSI_State);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// additionalRequestedCAMEL_SubscriptionInfo
	_additionalRequestedCAMEL_SubscriptionInfo = NULL;
	children.push_back(_additionalRequestedCAMEL_SubscriptionInfo);


}
asn1::ModificationRequestFor_CSI::~ModificationRequestFor_CSI(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_CSI::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_modifyNotificationToCSE = new ModificationInstruction();
			_modifyNotificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyNotificationToCSE->tlv->tag_value = 1;
			children[1] = _modifyNotificationToCSE;
			return _modifyNotificationToCSE;
		case 2:
			_modifyCSI_State = new ModificationInstruction();
			_modifyCSI_State->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyCSI_State->tlv->tag_value = 2;
			children[2] = _modifyCSI_State;
			return _modifyCSI_State;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;
		case 4:
			_additionalRequestedCAMEL_SubscriptionInfo = new AdditionalRequestedCAMEL_SubscriptionInfo();
			_additionalRequestedCAMEL_SubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_additionalRequestedCAMEL_SubscriptionInfo->tlv->tag_value = 4;
			children[4] = _additionalRequestedCAMEL_SubscriptionInfo;
			return _additionalRequestedCAMEL_SubscriptionInfo;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_CSI::set_modifyNotificationToCSE(){
	if(_modifyNotificationToCSE == NULL) _modifyNotificationToCSE = (ModificationInstruction*)create_node(1);
}

void asn1::ModificationRequestFor_CSI::set_modifyCSI_State(){
	if(_modifyCSI_State == NULL) _modifyCSI_State = (ModificationInstruction*)create_node(2);
}

void asn1::ModificationRequestFor_CSI::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

void asn1::ModificationRequestFor_CSI::set_additionalRequestedCAMEL_SubscriptionInfo(){
	if(_additionalRequestedCAMEL_SubscriptionInfo == NULL) _additionalRequestedCAMEL_SubscriptionInfo = (AdditionalRequestedCAMEL_SubscriptionInfo*)create_node(4);
}

//ModificationRequestFor_IP_SM_GW_Data
asn1::ModificationRequestFor_IP_SM_GW_Data::ModificationRequestFor_IP_SM_GW_Data(){
	strcpy(node_type_name, "ModificationRequestFor_IP_SM_GW_Data");
	// modifyRegistrationStatus
	_modifyRegistrationStatus = NULL;
	children.push_back(_modifyRegistrationStatus);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// ip_sm_gw_DiameterAddress
	_ip_sm_gw_DiameterAddress = NULL;
	children.push_back(_ip_sm_gw_DiameterAddress);


}
asn1::ModificationRequestFor_IP_SM_GW_Data::~ModificationRequestFor_IP_SM_GW_Data(){

}

asn1::ASN1Node* asn1::ModificationRequestFor_IP_SM_GW_Data::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_modifyRegistrationStatus = new ModificationInstruction();
			_modifyRegistrationStatus->tlv->tag_class = CONTEXT_SPECIFIC;
			_modifyRegistrationStatus->tlv->tag_value = 0;
			children[0] = _modifyRegistrationStatus;
			return _modifyRegistrationStatus;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;
		case 2:
			_ip_sm_gw_DiameterAddress = new NetworkNodeDiameterAddress();
			_ip_sm_gw_DiameterAddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_ip_sm_gw_DiameterAddress->tlv->tag_value = 2;
			children[2] = _ip_sm_gw_DiameterAddress;
			return _ip_sm_gw_DiameterAddress;

		default: return NULL;
	}
}

void asn1::ModificationRequestFor_IP_SM_GW_Data::set_modifyRegistrationStatus(){
	if(_modifyRegistrationStatus == NULL) _modifyRegistrationStatus = (ModificationInstruction*)create_node(0);
}

void asn1::ModificationRequestFor_IP_SM_GW_Data::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

void asn1::ModificationRequestFor_IP_SM_GW_Data::set_ip_sm_gw_DiameterAddress(){
	if(_ip_sm_gw_DiameterAddress == NULL) _ip_sm_gw_DiameterAddress = (NetworkNodeDiameterAddress*)create_node(2);
}

//ModificationInstruction
asn1::ModificationInstruction::ModificationInstruction(){
	strcpy(node_type_name, "ModificationInstruction");

}
asn1::ModificationInstruction::~ModificationInstruction(){

}

//NoteSubscriberDataModifiedArg
asn1::NoteSubscriberDataModifiedArg::NoteSubscriberDataModifiedArg(){
	strcpy(node_type_name, "NoteSubscriberDataModifiedArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// msisdn
	_msisdn = NULL;
	_msisdn = new ISDN_AddressString();
	children.push_back(_msisdn);

	// forwardingInfoFor_CSE
	_forwardingInfoFor_CSE = NULL;
	children.push_back(_forwardingInfoFor_CSE);

	// callBarringInfoFor_CSE
	_callBarringInfoFor_CSE = NULL;
	children.push_back(_callBarringInfoFor_CSE);

	// odb_Info
	_odb_Info = NULL;
	children.push_back(_odb_Info);

	// camel_SubscriptionInfo
	_camel_SubscriptionInfo = NULL;
	children.push_back(_camel_SubscriptionInfo);

	// allInformationSent
	_allInformationSent = NULL;
	children.push_back(_allInformationSent);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// ue_reachable
	_ue_reachable = NULL;
	children.push_back(_ue_reachable);

	// csg_SubscriptionDataList
	_csg_SubscriptionDataList = NULL;
	children.push_back(_csg_SubscriptionDataList);

	// cw_Data
	_cw_Data = NULL;
	children.push_back(_cw_Data);

	// ch_Data
	_ch_Data = NULL;
	children.push_back(_ch_Data);

	// clip_Data
	_clip_Data = NULL;
	children.push_back(_clip_Data);

	// clir_Data
	_clir_Data = NULL;
	children.push_back(_clir_Data);

	// ect_data
	_ect_data = NULL;
	children.push_back(_ect_data);


}
asn1::NoteSubscriberDataModifiedArg::~NoteSubscriberDataModifiedArg(){

}

asn1::ASN1Node* asn1::NoteSubscriberDataModifiedArg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_forwardingInfoFor_CSE = new Ext_ForwardingInfoFor_CSE();
			_forwardingInfoFor_CSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardingInfoFor_CSE->tlv->tag_value = 0;
			children[2] = _forwardingInfoFor_CSE;
			return _forwardingInfoFor_CSE;
		case 3:
			_callBarringInfoFor_CSE = new Ext_CallBarringInfoFor_CSE();
			_callBarringInfoFor_CSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_callBarringInfoFor_CSE->tlv->tag_value = 1;
			children[3] = _callBarringInfoFor_CSE;
			return _callBarringInfoFor_CSE;
		case 4:
			_odb_Info = new ODB_Info();
			_odb_Info->tlv->tag_class = CONTEXT_SPECIFIC;
			_odb_Info->tlv->tag_value = 2;
			children[4] = _odb_Info;
			return _odb_Info;
		case 5:
			_camel_SubscriptionInfo = new CAMEL_SubscriptionInfo();
			_camel_SubscriptionInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_camel_SubscriptionInfo->tlv->tag_value = 3;
			children[5] = _camel_SubscriptionInfo;
			return _camel_SubscriptionInfo;
		case 6:
			_allInformationSent = new Null();
			_allInformationSent->tlv->tag_class = CONTEXT_SPECIFIC;
			_allInformationSent->tlv->tag_value = 4;
			children[6] = _allInformationSent;
			return _allInformationSent;
		case 7:
			_extensionContainer = new ExtensionContainer();
			children[7] = _extensionContainer;
			return _extensionContainer;
		case 8:
			_ue_reachable = new ServingNode();
			_ue_reachable->tlv->tag_class = CONTEXT_SPECIFIC;
			_ue_reachable->tlv->tag_value = 5;
			children[8] = _ue_reachable;
			return _ue_reachable;
		case 9:
			_csg_SubscriptionDataList = new CSG_SubscriptionDataList();
			_csg_SubscriptionDataList->tlv->tag_class = CONTEXT_SPECIFIC;
			_csg_SubscriptionDataList->tlv->tag_value = 6;
			children[9] = _csg_SubscriptionDataList;
			return _csg_SubscriptionDataList;
		case 10:
			_cw_Data = new CallWaitingData();
			_cw_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_cw_Data->tlv->tag_value = 7;
			children[10] = _cw_Data;
			return _cw_Data;
		case 11:
			_ch_Data = new CallHoldData();
			_ch_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_ch_Data->tlv->tag_value = 8;
			children[11] = _ch_Data;
			return _ch_Data;
		case 12:
			_clip_Data = new ClipData();
			_clip_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_clip_Data->tlv->tag_value = 9;
			children[12] = _clip_Data;
			return _clip_Data;
		case 13:
			_clir_Data = new ClirData();
			_clir_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_clir_Data->tlv->tag_value = 10;
			children[13] = _clir_Data;
			return _clir_Data;
		case 14:
			_ect_data = new EctData();
			_ect_data->tlv->tag_class = CONTEXT_SPECIFIC;
			_ect_data->tlv->tag_value = 11;
			children[14] = _ect_data;
			return _ect_data;

		default: return NULL;
	}
}

void asn1::NoteSubscriberDataModifiedArg::set_forwardingInfoFor_CSE(){
	if(_forwardingInfoFor_CSE == NULL) _forwardingInfoFor_CSE = (Ext_ForwardingInfoFor_CSE*)create_node(2);
}

void asn1::NoteSubscriberDataModifiedArg::set_callBarringInfoFor_CSE(){
	if(_callBarringInfoFor_CSE == NULL) _callBarringInfoFor_CSE = (Ext_CallBarringInfoFor_CSE*)create_node(3);
}

void asn1::NoteSubscriberDataModifiedArg::set_odb_Info(){
	if(_odb_Info == NULL) _odb_Info = (ODB_Info*)create_node(4);
}

void asn1::NoteSubscriberDataModifiedArg::set_camel_SubscriptionInfo(){
	if(_camel_SubscriptionInfo == NULL) _camel_SubscriptionInfo = (CAMEL_SubscriptionInfo*)create_node(5);
}

void asn1::NoteSubscriberDataModifiedArg::set_allInformationSent(){
	if(_allInformationSent == NULL) _allInformationSent = (Null*)create_node(6);
}

void asn1::NoteSubscriberDataModifiedArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(7);
}

void asn1::NoteSubscriberDataModifiedArg::set_ue_reachable(){
	if(_ue_reachable == NULL) _ue_reachable = (ServingNode*)create_node(8);
}

void asn1::NoteSubscriberDataModifiedArg::set_csg_SubscriptionDataList(){
	if(_csg_SubscriptionDataList == NULL) _csg_SubscriptionDataList = (CSG_SubscriptionDataList*)create_node(9);
}

void asn1::NoteSubscriberDataModifiedArg::set_cw_Data(){
	if(_cw_Data == NULL) _cw_Data = (CallWaitingData*)create_node(10);
}

void asn1::NoteSubscriberDataModifiedArg::set_ch_Data(){
	if(_ch_Data == NULL) _ch_Data = (CallHoldData*)create_node(11);
}

void asn1::NoteSubscriberDataModifiedArg::set_clip_Data(){
	if(_clip_Data == NULL) _clip_Data = (ClipData*)create_node(12);
}

void asn1::NoteSubscriberDataModifiedArg::set_clir_Data(){
	if(_clir_Data == NULL) _clir_Data = (ClirData*)create_node(13);
}

void asn1::NoteSubscriberDataModifiedArg::set_ect_data(){
	if(_ect_data == NULL) _ect_data = (EctData*)create_node(14);
}

//NoteSubscriberDataModifiedRes
asn1::NoteSubscriberDataModifiedRes::NoteSubscriberDataModifiedRes(){
	strcpy(node_type_name, "NoteSubscriberDataModifiedRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::NoteSubscriberDataModifiedRes::~NoteSubscriberDataModifiedRes(){

}

asn1::ASN1Node* asn1::NoteSubscriberDataModifiedRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::NoteSubscriberDataModifiedRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//NoteMM_EventArg
asn1::NoteMM_EventArg::NoteMM_EventArg(){
	strcpy(node_type_name, "NoteMM_EventArg");
	// serviceKey
	_serviceKey = NULL;
	_serviceKey = new ServiceKey();
	children.push_back(_serviceKey);

	// eventMet
	_eventMet = NULL;
	_eventMet = new MM_Code();
	_eventMet->tlv->tag_class = CONTEXT_SPECIFIC;
	_eventMet->tlv->tag_value = 0;
	children.push_back(_eventMet);

	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
	_imsi->tlv->tag_value = 1;
	children.push_back(_imsi);

	// msisdn
	_msisdn = NULL;
	_msisdn = new ISDN_AddressString();
	_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
	_msisdn->tlv->tag_value = 2;
	children.push_back(_msisdn);

	// locationInformation
	_locationInformation = NULL;
	children.push_back(_locationInformation);

	// supportedCAMELPhases
	_supportedCAMELPhases = NULL;
	children.push_back(_supportedCAMELPhases);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// locationInformationGPRS
	_locationInformationGPRS = NULL;
	children.push_back(_locationInformationGPRS);

	// offeredCamel4Functionalities
	_offeredCamel4Functionalities = NULL;
	children.push_back(_offeredCamel4Functionalities);


}
asn1::NoteMM_EventArg::~NoteMM_EventArg(){

}

asn1::ASN1Node* asn1::NoteMM_EventArg::create_node(unsigned int _index){
	switch(_index){
		case 4:
			_locationInformation = new LocationInformation();
			_locationInformation->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationInformation->tlv->tag_value = 3;
			children[4] = _locationInformation;
			return _locationInformation;
		case 5:
			_supportedCAMELPhases = new SupportedCamelPhases();
			_supportedCAMELPhases->tlv->tag_class = CONTEXT_SPECIFIC;
			_supportedCAMELPhases->tlv->tag_value = 5;
			children[5] = _supportedCAMELPhases;
			return _supportedCAMELPhases;
		case 6:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 6;
			children[6] = _extensionContainer;
			return _extensionContainer;
		case 7:
			_locationInformationGPRS = new LocationInformationGPRS();
			_locationInformationGPRS->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationInformationGPRS->tlv->tag_value = 7;
			children[7] = _locationInformationGPRS;
			return _locationInformationGPRS;
		case 8:
			_offeredCamel4Functionalities = new OfferedCamel4Functionalities();
			_offeredCamel4Functionalities->tlv->tag_class = CONTEXT_SPECIFIC;
			_offeredCamel4Functionalities->tlv->tag_value = 8;
			children[8] = _offeredCamel4Functionalities;
			return _offeredCamel4Functionalities;

		default: return NULL;
	}
}

void asn1::NoteMM_EventArg::set_locationInformation(){
	if(_locationInformation == NULL) _locationInformation = (LocationInformation*)create_node(4);
}

void asn1::NoteMM_EventArg::set_supportedCAMELPhases(){
	if(_supportedCAMELPhases == NULL) _supportedCAMELPhases = (SupportedCamelPhases*)create_node(5);
}

void asn1::NoteMM_EventArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(6);
}

void asn1::NoteMM_EventArg::set_locationInformationGPRS(){
	if(_locationInformationGPRS == NULL) _locationInformationGPRS = (LocationInformationGPRS*)create_node(7);
}

void asn1::NoteMM_EventArg::set_offeredCamel4Functionalities(){
	if(_offeredCamel4Functionalities == NULL) _offeredCamel4Functionalities = (OfferedCamel4Functionalities*)create_node(8);
}

//NoteMM_EventRes
asn1::NoteMM_EventRes::NoteMM_EventRes(){
	strcpy(node_type_name, "NoteMM_EventRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::NoteMM_EventRes::~NoteMM_EventRes(){

}

asn1::ASN1Node* asn1::NoteMM_EventRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::NoteMM_EventRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//Ext_SS_InfoFor_CSE
asn1::Ext_SS_InfoFor_CSE::Ext_SS_InfoFor_CSE(){
	strcpy(node_type_name, "Ext_SS_InfoFor_CSE");
	// forwardingInfoFor_CSE
	_forwardingInfoFor_CSE = NULL;
	_forwardingInfoFor_CSE = new Ext_ForwardingInfoFor_CSE();
	_forwardingInfoFor_CSE->tlv->tag_class = CONTEXT_SPECIFIC;
	_forwardingInfoFor_CSE->tlv->tag_value = 0;
	children.push_back(_forwardingInfoFor_CSE);

	// callBarringInfoFor_CSE
	_callBarringInfoFor_CSE = NULL;
	_callBarringInfoFor_CSE = new Ext_CallBarringInfoFor_CSE();
	_callBarringInfoFor_CSE->tlv->tag_class = CONTEXT_SPECIFIC;
	_callBarringInfoFor_CSE->tlv->tag_value = 1;
	children.push_back(_callBarringInfoFor_CSE);


}
asn1::Ext_SS_InfoFor_CSE::~Ext_SS_InfoFor_CSE(){

}

//Ext_ForwardingInfoFor_CSE
asn1::Ext_ForwardingInfoFor_CSE::Ext_ForwardingInfoFor_CSE(){
	strcpy(node_type_name, "Ext_ForwardingInfoFor_CSE");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	_ss_Code->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Code->tlv->tag_value = 0;
	children.push_back(_ss_Code);

	// forwardingFeatureList
	_forwardingFeatureList = NULL;
	_forwardingFeatureList = new Ext_ForwFeatureList();
	_forwardingFeatureList->tlv->tag_class = CONTEXT_SPECIFIC;
	_forwardingFeatureList->tlv->tag_value = 1;
	children.push_back(_forwardingFeatureList);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::Ext_ForwardingInfoFor_CSE::~Ext_ForwardingInfoFor_CSE(){

}

asn1::ASN1Node* asn1::Ext_ForwardingInfoFor_CSE::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 2;
			children[2] = _notificationToCSE;
			return _notificationToCSE;
		case 3:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 3;
			children[3] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::Ext_ForwardingInfoFor_CSE::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(2);
}

void asn1::Ext_ForwardingInfoFor_CSE::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(3);
}

//Ext_CallBarringInfoFor_CSE
asn1::Ext_CallBarringInfoFor_CSE::Ext_CallBarringInfoFor_CSE(){
	strcpy(node_type_name, "Ext_CallBarringInfoFor_CSE");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	_ss_Code->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Code->tlv->tag_value = 0;
	children.push_back(_ss_Code);

	// callBarringFeatureList
	_callBarringFeatureList = NULL;
	_callBarringFeatureList = new Ext_CallBarFeatureList();
	_callBarringFeatureList->tlv->tag_class = CONTEXT_SPECIFIC;
	_callBarringFeatureList->tlv->tag_value = 1;
	children.push_back(_callBarringFeatureList);

	// password
	_password = NULL;
	children.push_back(_password);

	// wrongPasswordAttemptsCounter
	_wrongPasswordAttemptsCounter = NULL;
	children.push_back(_wrongPasswordAttemptsCounter);

	// notificationToCSE
	_notificationToCSE = NULL;
	children.push_back(_notificationToCSE);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::Ext_CallBarringInfoFor_CSE::~Ext_CallBarringInfoFor_CSE(){

}

asn1::ASN1Node* asn1::Ext_CallBarringInfoFor_CSE::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_password = new Password();
			_password->tlv->tag_class = CONTEXT_SPECIFIC;
			_password->tlv->tag_value = 2;
			children[2] = _password;
			return _password;
		case 3:
			_wrongPasswordAttemptsCounter = new WrongPasswordAttemptsCounter();
			_wrongPasswordAttemptsCounter->tlv->tag_class = CONTEXT_SPECIFIC;
			_wrongPasswordAttemptsCounter->tlv->tag_value = 3;
			children[3] = _wrongPasswordAttemptsCounter;
			return _wrongPasswordAttemptsCounter;
		case 4:
			_notificationToCSE = new Null();
			_notificationToCSE->tlv->tag_class = CONTEXT_SPECIFIC;
			_notificationToCSE->tlv->tag_value = 4;
			children[4] = _notificationToCSE;
			return _notificationToCSE;
		case 5:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 5;
			children[5] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::Ext_CallBarringInfoFor_CSE::set_password(){
	if(_password == NULL) _password = (Password*)create_node(2);
}

void asn1::Ext_CallBarringInfoFor_CSE::set_wrongPasswordAttemptsCounter(){
	if(_wrongPasswordAttemptsCounter == NULL) _wrongPasswordAttemptsCounter = (WrongPasswordAttemptsCounter*)create_node(3);
}

void asn1::Ext_CallBarringInfoFor_CSE::set_notificationToCSE(){
	if(_notificationToCSE == NULL) _notificationToCSE = (Null*)create_node(4);
}

void asn1::Ext_CallBarringInfoFor_CSE::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

//UpdateVcsgLocationArg
asn1::UpdateVcsgLocationArg::UpdateVcsgLocationArg(){
	strcpy(node_type_name, "UpdateVcsgLocationArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// msisdn
	_msisdn = NULL;
	children.push_back(_msisdn);

	// vlr_Number
	_vlr_Number = NULL;
	children.push_back(_vlr_Number);

	// sgsn_Number
	_sgsn_Number = NULL;
	children.push_back(_sgsn_Number);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::UpdateVcsgLocationArg::~UpdateVcsgLocationArg(){

}

asn1::ASN1Node* asn1::UpdateVcsgLocationArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_msisdn = new ISDN_AddressString();
			_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn->tlv->tag_value = 2;
			children[1] = _msisdn;
			return _msisdn;
		case 2:
			_vlr_Number = new ISDN_AddressString();
			_vlr_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_vlr_Number->tlv->tag_value = 0;
			children[2] = _vlr_Number;
			return _vlr_Number;
		case 3:
			_sgsn_Number = new ISDN_AddressString();
			_sgsn_Number->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_Number->tlv->tag_value = 1;
			children[3] = _sgsn_Number;
			return _sgsn_Number;
		case 4:
			_extensionContainer = new ExtensionContainer();
			children[4] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::UpdateVcsgLocationArg::set_msisdn(){
	if(_msisdn == NULL) _msisdn = (ISDN_AddressString*)create_node(1);
}

void asn1::UpdateVcsgLocationArg::set_vlr_Number(){
	if(_vlr_Number == NULL) _vlr_Number = (ISDN_AddressString*)create_node(2);
}

void asn1::UpdateVcsgLocationArg::set_sgsn_Number(){
	if(_sgsn_Number == NULL) _sgsn_Number = (ISDN_AddressString*)create_node(3);
}

void asn1::UpdateVcsgLocationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

//UpdateVcsgLocationRes
asn1::UpdateVcsgLocationRes::UpdateVcsgLocationRes(){
	strcpy(node_type_name, "UpdateVcsgLocationRes");
	// temporaryEmptySubscriptiondataIndicator
	_temporaryEmptySubscriptiondataIndicator = NULL;
	children.push_back(_temporaryEmptySubscriptiondataIndicator);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::UpdateVcsgLocationRes::~UpdateVcsgLocationRes(){

}

asn1::ASN1Node* asn1::UpdateVcsgLocationRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_temporaryEmptySubscriptiondataIndicator = new Null();
			children[0] = _temporaryEmptySubscriptiondataIndicator;
			return _temporaryEmptySubscriptiondataIndicator;
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::UpdateVcsgLocationRes::set_temporaryEmptySubscriptiondataIndicator(){
	if(_temporaryEmptySubscriptiondataIndicator == NULL) _temporaryEmptySubscriptiondataIndicator = (Null*)create_node(0);
}

void asn1::UpdateVcsgLocationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//CancelVcsgLocationArg
asn1::CancelVcsgLocationArg::CancelVcsgLocationArg(){
	strcpy(node_type_name, "CancelVcsgLocationArg");
	// identity
	_identity = NULL;
	_identity = new Identity();
	children.push_back(_identity);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CancelVcsgLocationArg::~CancelVcsgLocationArg(){

}

asn1::ASN1Node* asn1::CancelVcsgLocationArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extensionContainer = new ExtensionContainer();
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CancelVcsgLocationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//CancelVcsgLocationRes
asn1::CancelVcsgLocationRes::CancelVcsgLocationRes(){
	strcpy(node_type_name, "CancelVcsgLocationRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::CancelVcsgLocationRes::~CancelVcsgLocationRes(){

}

asn1::ASN1Node* asn1::CancelVcsgLocationRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::CancelVcsgLocationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

