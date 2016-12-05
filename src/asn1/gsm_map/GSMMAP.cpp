#include<GSMMAP.h>
#include<iostream>

//Component
asn1::gsmmap::Component::Component(){
	strcpy(node_type_name, "Component");
	// invoke
	_invoke = NULL;
	_invoke = new Invoke();
	_invoke->tlv->tag_class = CONTEXT_SPECIFIC;
	_invoke->tlv->tag_value = 1;
	children.push_back(_invoke);

	// returnResultLast
	_returnResultLast = NULL;
	_returnResultLast = new ReturnResult();
	_returnResultLast->tlv->tag_class = CONTEXT_SPECIFIC;
	_returnResultLast->tlv->tag_value = 2;
	children.push_back(_returnResultLast);

	// returnError
	_returnError = NULL;
	_returnError = new ReturnError();
	_returnError->tlv->tag_class = CONTEXT_SPECIFIC;
	_returnError->tlv->tag_value = 3;
	children.push_back(_returnError);

	// reject
	_reject = NULL;
	_reject = new Reject();
	_reject->tlv->tag_class = CONTEXT_SPECIFIC;
	_reject->tlv->tag_value = 4;
	children.push_back(_reject);

	// returnResultNotLast
	_returnResultNotLast = NULL;
	_returnResultNotLast = new ReturnResult();
	_returnResultNotLast->tlv->tag_class = CONTEXT_SPECIFIC;
	_returnResultNotLast->tlv->tag_value = 7;
	children.push_back(_returnResultNotLast);


}
asn1::gsmmap::Component::~Component(){

}

//Invoke
asn1::gsmmap::Invoke::Invoke(){
	strcpy(node_type_name, "Invoke");
	// invokeID
	_invokeID = NULL;
	_invokeID = new InvokeIdType();
	children.push_back(_invokeID);

	// linkedID
	_linkedID = NULL;
	children.push_back(_linkedID);

	// opCode
	_opCode = NULL;
	_opCode = new MAP_OPERATION();
	children.push_back(_opCode);

	// invokeparameter
	_invokeparameter = NULL;
	children.push_back(_invokeparameter);


}
asn1::gsmmap::Invoke::~Invoke(){

}

asn1::ASN1Node* asn1::gsmmap::Invoke::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_linkedID = new InvokeIdType();
			_linkedID->tlv->tag_class = CONTEXT_SPECIFIC;
			_linkedID->tlv->tag_value = 0;
			children[1] = _linkedID;
			return _linkedID;
		case 3:
			_invokeparameter = new InvokeParameter();
			children[3] = _invokeparameter;
			return _invokeparameter;

		default: return NULL;
	}
}

void asn1::gsmmap::Invoke::set_linkedID(){
	if(_linkedID == NULL) _linkedID = (InvokeIdType*)create_node(1);
}

void asn1::gsmmap::Invoke::set_invokeparameter(){
	if(_invokeparameter == NULL) _invokeparameter = (InvokeParameter*)create_node(3);
}

//InvokeParameter
asn1::gsmmap::InvokeParameter::InvokeParameter(){
	strcpy(node_type_name, "InvokeParameter");

}
asn1::gsmmap::InvokeParameter::~InvokeParameter(){

}

//ReturnResult
asn1::gsmmap::ReturnResult::ReturnResult(){
	strcpy(node_type_name, "ReturnResult");
	// invokeID
	_invokeID = NULL;
	_invokeID = new InvokeIdType();
	children.push_back(_invokeID);

	// resultretres
	_resultretres = NULL;
	children.push_back(_resultretres);


}
asn1::gsmmap::ReturnResult::~ReturnResult(){

}

asn1::ASN1Node* asn1::gsmmap::ReturnResult::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_resultretres = new ReturnResult_resultretres();
			children[1] = _resultretres;
			return _resultretres;

		default: return NULL;
	}
}

void asn1::gsmmap::ReturnResult::set_resultretres(){
	if(_resultretres == NULL) _resultretres = (ReturnResult_resultretres*)create_node(1);
}

//ReturnResult_resultretres
asn1::gsmmap::ReturnResult_resultretres::ReturnResult_resultretres(){
	strcpy(node_type_name, "ReturnResult_resultretres");
	// opCode
	_opCode = NULL;
	_opCode = new MAP_OPERATION();
	children.push_back(_opCode);

	// returnparameter
	_returnparameter = NULL;
	children.push_back(_returnparameter);


}
asn1::gsmmap::ReturnResult_resultretres::~ReturnResult_resultretres(){

}

asn1::ASN1Node* asn1::gsmmap::ReturnResult_resultretres::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_returnparameter = new ReturnResultParameter();
			children[1] = _returnparameter;
			return _returnparameter;

		default: return NULL;
	}
}

void asn1::gsmmap::ReturnResult_resultretres::set_returnparameter(){
	if(_returnparameter == NULL) _returnparameter = (ReturnResultParameter*)create_node(1);
}

//ReturnResultParameter
asn1::gsmmap::ReturnResultParameter::ReturnResultParameter(){
	strcpy(node_type_name, "ReturnResultParameter");

}
asn1::gsmmap::ReturnResultParameter::~ReturnResultParameter(){

}

//ReturnError
asn1::gsmmap::ReturnError::ReturnError(){
	strcpy(node_type_name, "ReturnError");
	// invokeID
	_invokeID = NULL;
	_invokeID = new InvokeIdType();
	children.push_back(_invokeID);

	// errorCode
	_errorCode = NULL;
	_errorCode = new MAP_ERROR();
	children.push_back(_errorCode);

	// parameter
	_parameter = NULL;
	children.push_back(_parameter);


}
asn1::gsmmap::ReturnError::~ReturnError(){

}

asn1::ASN1Node* asn1::gsmmap::ReturnError::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_parameter = new ReturnErrorParameter();
			children[2] = _parameter;
			return _parameter;

		default: return NULL;
	}
}

void asn1::gsmmap::ReturnError::set_parameter(){
	if(_parameter == NULL) _parameter = (ReturnErrorParameter*)create_node(2);
}

//ReturnErrorParameter
asn1::gsmmap::ReturnErrorParameter::ReturnErrorParameter(){
	strcpy(node_type_name, "ReturnErrorParameter");

}
asn1::gsmmap::ReturnErrorParameter::~ReturnErrorParameter(){

}

//Reject
asn1::gsmmap::Reject::Reject(){
	strcpy(node_type_name, "Reject");
	// invokeIDRej
	_invokeIDRej = NULL;
	_invokeIDRej = new Reject_invokeIDRej();
	children.push_back(_invokeIDRej);

	// problem
	_problem = NULL;
	_problem = new Reject_problem();
	children.push_back(_problem);


}
asn1::gsmmap::Reject::~Reject(){

}

//Reject_invokeIDRej
asn1::gsmmap::Reject_invokeIDRej::Reject_invokeIDRej(){
	strcpy(node_type_name, "Reject_invokeIDRej");
	// derivable
	_derivable = NULL;
	_derivable = new InvokeIdType();
	children.push_back(_derivable);

	// not_derivable
	_not_derivable = NULL;
	_not_derivable = new Null();
	children.push_back(_not_derivable);


}
asn1::gsmmap::Reject_invokeIDRej::~Reject_invokeIDRej(){

}

//Reject_problem
asn1::gsmmap::Reject_problem::Reject_problem(){
	strcpy(node_type_name, "Reject_problem");
	// generalProblem
	_generalProblem = NULL;
	_generalProblem = new GeneralProblem();
	_generalProblem->tlv->tag_class = CONTEXT_SPECIFIC;
	_generalProblem->tlv->tag_value = 0;
	children.push_back(_generalProblem);

	// invokeProblem
	_invokeProblem = NULL;
	_invokeProblem = new InvokeProblem();
	_invokeProblem->tlv->tag_class = CONTEXT_SPECIFIC;
	_invokeProblem->tlv->tag_value = 1;
	children.push_back(_invokeProblem);

	// returnResultProblem
	_returnResultProblem = NULL;
	_returnResultProblem = new ReturnResultProblem();
	_returnResultProblem->tlv->tag_class = CONTEXT_SPECIFIC;
	_returnResultProblem->tlv->tag_value = 2;
	children.push_back(_returnResultProblem);

	// returnErrorProblem
	_returnErrorProblem = NULL;
	_returnErrorProblem = new ReturnErrorProblem();
	_returnErrorProblem->tlv->tag_class = CONTEXT_SPECIFIC;
	_returnErrorProblem->tlv->tag_value = 3;
	children.push_back(_returnErrorProblem);


}
asn1::gsmmap::Reject_problem::~Reject_problem(){

}

//InvokeIdType
asn1::gsmmap::InvokeIdType::InvokeIdType(){
	strcpy(node_type_name, "InvokeIdType");

}
asn1::gsmmap::InvokeIdType::~InvokeIdType(){

}

//MAP_OPERATION
asn1::gsmmap::MAP_OPERATION::MAP_OPERATION(){
	strcpy(node_type_name, "MAP_OPERATION");
	// localValue
	_localValue = NULL;
	_localValue = new OperationLocalvalue();
	children.push_back(_localValue);

	// globalValue
	_globalValue = NULL;
	_globalValue = new Object_identifier();
	children.push_back(_globalValue);


}
asn1::gsmmap::MAP_OPERATION::~MAP_OPERATION(){

}

//GSMMAPOperationLocalvalue
asn1::gsmmap::GSMMAPOperationLocalvalue::GSMMAPOperationLocalvalue(){
	strcpy(node_type_name, "GSMMAPOperationLocalvalue");

}
asn1::gsmmap::GSMMAPOperationLocalvalue::~GSMMAPOperationLocalvalue(){

}

//OperationLocalvalue
asn1::gsmmap::OperationLocalvalue::OperationLocalvalue(){
	strcpy(node_type_name, "OperationLocalvalue");

}
asn1::gsmmap::OperationLocalvalue::~OperationLocalvalue(){

}

//MAP_ERROR
asn1::gsmmap::MAP_ERROR::MAP_ERROR(){
	strcpy(node_type_name, "MAP_ERROR");
	// localValue
	_localValue = NULL;
	_localValue = new LocalErrorcode();
	children.push_back(_localValue);

	// globalValue
	_globalValue = NULL;
	_globalValue = new Object_identifier();
	children.push_back(_globalValue);


}
asn1::gsmmap::MAP_ERROR::~MAP_ERROR(){

}

//GSMMAPLocalErrorcode
asn1::gsmmap::GSMMAPLocalErrorcode::GSMMAPLocalErrorcode(){
	strcpy(node_type_name, "GSMMAPLocalErrorcode");

}
asn1::gsmmap::GSMMAPLocalErrorcode::~GSMMAPLocalErrorcode(){

}

//LocalErrorcode
asn1::gsmmap::LocalErrorcode::LocalErrorcode(){
	strcpy(node_type_name, "LocalErrorcode");

}
asn1::gsmmap::LocalErrorcode::~LocalErrorcode(){

}

//GeneralProblem
asn1::gsmmap::GeneralProblem::GeneralProblem(){
	strcpy(node_type_name, "GeneralProblem");

}
asn1::gsmmap::GeneralProblem::~GeneralProblem(){

}

//InvokeProblem
asn1::gsmmap::InvokeProblem::InvokeProblem(){
	strcpy(node_type_name, "InvokeProblem");

}
asn1::gsmmap::InvokeProblem::~InvokeProblem(){

}

//ReturnResultProblem
asn1::gsmmap::ReturnResultProblem::ReturnResultProblem(){
	strcpy(node_type_name, "ReturnResultProblem");

}
asn1::gsmmap::ReturnResultProblem::~ReturnResultProblem(){

}

//ReturnErrorProblem
asn1::gsmmap::ReturnErrorProblem::ReturnErrorProblem(){
	strcpy(node_type_name, "ReturnErrorProblem");

}
asn1::gsmmap::ReturnErrorProblem::~ReturnErrorProblem(){

}

//Bss_APDU
asn1::gsmmap::Bss_APDU::Bss_APDU(){
	strcpy(node_type_name, "Bss_APDU");
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
asn1::gsmmap::Bss_APDU::~Bss_APDU(){

}

asn1::ASN1Node* asn1::gsmmap::Bss_APDU::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_extensionContainer = new ExtensionContainer();
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::gsmmap::Bss_APDU::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//ProvideSIWFSNumberArg
asn1::gsmmap::ProvideSIWFSNumberArg::ProvideSIWFSNumberArg(){
	strcpy(node_type_name, "ProvideSIWFSNumberArg");
	// gsm_BearerCapability
	_gsm_BearerCapability = NULL;
	_gsm_BearerCapability = new ExternalSignalInfo();
	_gsm_BearerCapability->tlv->tag_class = CONTEXT_SPECIFIC;
	_gsm_BearerCapability->tlv->tag_value = 0;
	children.push_back(_gsm_BearerCapability);

	// isdn_BearerCapability
	_isdn_BearerCapability = NULL;
	_isdn_BearerCapability = new ExternalSignalInfo();
	_isdn_BearerCapability->tlv->tag_class = CONTEXT_SPECIFIC;
	_isdn_BearerCapability->tlv->tag_value = 1;
	children.push_back(_isdn_BearerCapability);

	// call_Direction
	_call_Direction = NULL;
	_call_Direction = new CallDirection();
	_call_Direction->tlv->tag_class = CONTEXT_SPECIFIC;
	_call_Direction->tlv->tag_value = 2;
	children.push_back(_call_Direction);

	// b_Subscriber_Address
	_b_Subscriber_Address = NULL;
	_b_Subscriber_Address = new ISDN_AddressString();
	_b_Subscriber_Address->tlv->tag_class = CONTEXT_SPECIFIC;
	_b_Subscriber_Address->tlv->tag_value = 3;
	children.push_back(_b_Subscriber_Address);

	// chosenChannel
	_chosenChannel = NULL;
	_chosenChannel = new ExternalSignalInfo();
	_chosenChannel->tlv->tag_class = CONTEXT_SPECIFIC;
	_chosenChannel->tlv->tag_value = 4;
	children.push_back(_chosenChannel);

	// lowerLayerCompatibility
	_lowerLayerCompatibility = NULL;
	children.push_back(_lowerLayerCompatibility);

	// highLayerCompatibility
	_highLayerCompatibility = NULL;
	children.push_back(_highLayerCompatibility);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::gsmmap::ProvideSIWFSNumberArg::~ProvideSIWFSNumberArg(){

}

asn1::ASN1Node* asn1::gsmmap::ProvideSIWFSNumberArg::create_node(unsigned int _index){
	switch(_index){
		case 5:
			_lowerLayerCompatibility = new ExternalSignalInfo();
			_lowerLayerCompatibility->tlv->tag_class = CONTEXT_SPECIFIC;
			_lowerLayerCompatibility->tlv->tag_value = 5;
			children[5] = _lowerLayerCompatibility;
			return _lowerLayerCompatibility;
		case 6:
			_highLayerCompatibility = new ExternalSignalInfo();
			_highLayerCompatibility->tlv->tag_class = CONTEXT_SPECIFIC;
			_highLayerCompatibility->tlv->tag_value = 6;
			children[6] = _highLayerCompatibility;
			return _highLayerCompatibility;
		case 7:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 7;
			children[7] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::gsmmap::ProvideSIWFSNumberArg::set_lowerLayerCompatibility(){
	if(_lowerLayerCompatibility == NULL) _lowerLayerCompatibility = (ExternalSignalInfo*)create_node(5);
}

void asn1::gsmmap::ProvideSIWFSNumberArg::set_highLayerCompatibility(){
	if(_highLayerCompatibility == NULL) _highLayerCompatibility = (ExternalSignalInfo*)create_node(6);
}

void asn1::gsmmap::ProvideSIWFSNumberArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(7);
}

//ProvideSIWFSNumberRes
asn1::gsmmap::ProvideSIWFSNumberRes::ProvideSIWFSNumberRes(){
	strcpy(node_type_name, "ProvideSIWFSNumberRes");
	// sIWFSNumber
	_sIWFSNumber = NULL;
	_sIWFSNumber = new ISDN_AddressString();
	_sIWFSNumber->tlv->tag_class = CONTEXT_SPECIFIC;
	_sIWFSNumber->tlv->tag_value = 0;
	children.push_back(_sIWFSNumber);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::gsmmap::ProvideSIWFSNumberRes::~ProvideSIWFSNumberRes(){

}

asn1::ASN1Node* asn1::gsmmap::ProvideSIWFSNumberRes::create_node(unsigned int _index){
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

void asn1::gsmmap::ProvideSIWFSNumberRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//CallDirection
asn1::gsmmap::CallDirection::CallDirection(){
	strcpy(node_type_name, "CallDirection");

}
asn1::gsmmap::CallDirection::~CallDirection(){

}

//PurgeMSArgV2
asn1::gsmmap::PurgeMSArgV2::PurgeMSArgV2(){
	strcpy(node_type_name, "PurgeMSArgV2");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// vlr_Number
	_vlr_Number = NULL;
	children.push_back(_vlr_Number);


}
asn1::gsmmap::PurgeMSArgV2::~PurgeMSArgV2(){

}

asn1::ASN1Node* asn1::gsmmap::PurgeMSArgV2::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_vlr_Number = new ISDN_AddressString();
			children[1] = _vlr_Number;
			return _vlr_Number;

		default: return NULL;
	}
}

void asn1::gsmmap::PurgeMSArgV2::set_vlr_Number(){
	if(_vlr_Number == NULL) _vlr_Number = (ISDN_AddressString*)create_node(1);
}

//PrepareHO_ArgOld
asn1::gsmmap::PrepareHO_ArgOld::PrepareHO_ArgOld(){
	strcpy(node_type_name, "PrepareHO_ArgOld");
	// targetCellId
	_targetCellId = NULL;
	children.push_back(_targetCellId);

	// ho_NumberNotRequired
	_ho_NumberNotRequired = NULL;
	children.push_back(_ho_NumberNotRequired);

	// bss_APDU
	_bss_APDU = NULL;
	children.push_back(_bss_APDU);


}
asn1::gsmmap::PrepareHO_ArgOld::~PrepareHO_ArgOld(){

}

asn1::ASN1Node* asn1::gsmmap::PrepareHO_ArgOld::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_targetCellId = new GlobalCellId();
			children[0] = _targetCellId;
			return _targetCellId;
		case 1:
			_ho_NumberNotRequired = new Null();
			children[1] = _ho_NumberNotRequired;
			return _ho_NumberNotRequired;
		case 2:
			_bss_APDU = new Bss_APDU();
			children[2] = _bss_APDU;
			return _bss_APDU;

		default: return NULL;
	}
}

void asn1::gsmmap::PrepareHO_ArgOld::set_targetCellId(){
	if(_targetCellId == NULL) _targetCellId = (GlobalCellId*)create_node(0);
}

void asn1::gsmmap::PrepareHO_ArgOld::set_ho_NumberNotRequired(){
	if(_ho_NumberNotRequired == NULL) _ho_NumberNotRequired = (Null*)create_node(1);
}

void asn1::gsmmap::PrepareHO_ArgOld::set_bss_APDU(){
	if(_bss_APDU == NULL) _bss_APDU = (Bss_APDU*)create_node(2);
}

//PrepareHO_ResOld
asn1::gsmmap::PrepareHO_ResOld::PrepareHO_ResOld(){
	strcpy(node_type_name, "PrepareHO_ResOld");
	// handoverNumber
	_handoverNumber = NULL;
	children.push_back(_handoverNumber);

	// bss_APDU
	_bss_APDU = NULL;
	children.push_back(_bss_APDU);


}
asn1::gsmmap::PrepareHO_ResOld::~PrepareHO_ResOld(){

}

asn1::ASN1Node* asn1::gsmmap::PrepareHO_ResOld::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_handoverNumber = new ISDN_AddressString();
			children[0] = _handoverNumber;
			return _handoverNumber;
		case 1:
			_bss_APDU = new Bss_APDU();
			children[1] = _bss_APDU;
			return _bss_APDU;

		default: return NULL;
	}
}

void asn1::gsmmap::PrepareHO_ResOld::set_handoverNumber(){
	if(_handoverNumber == NULL) _handoverNumber = (ISDN_AddressString*)create_node(0);
}

void asn1::gsmmap::PrepareHO_ResOld::set_bss_APDU(){
	if(_bss_APDU == NULL) _bss_APDU = (Bss_APDU*)create_node(1);
}

//SendAuthenticationInfoResOld
asn1::gsmmap::SendAuthenticationInfoResOld::SendAuthenticationInfoResOld(){
	strcpy(node_type_name, "SendAuthenticationInfoResOld");

}
asn1::gsmmap::SendAuthenticationInfoResOld::~SendAuthenticationInfoResOld(){
}

asn1::gsmmap::SendAuthenticationInfoResOld_SEQUENCE* asn1::gsmmap::SendAuthenticationInfoResOld::get_child(unsigned int child_index){
	if(child_index < children.size()) return (SendAuthenticationInfoResOld_SEQUENCE*)children[child_index]; else return NULL;
}

void asn1::gsmmap::SendAuthenticationInfoResOld::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::gsmmap::SendAuthenticationInfoResOld::create_node(unsigned int _index){
	children[_index] = new SendAuthenticationInfoResOld_SEQUENCE();
	return children[_index];
}

asn1::ASN1Node* asn1::gsmmap::SendAuthenticationInfoResOld::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new SendAuthenticationInfoResOld_SEQUENCE());
		return children[children.size() - 1];
	}

}

//SendAuthenticationInfoResOld_SEQUENCE
asn1::gsmmap::SendAuthenticationInfoResOld_SEQUENCE::SendAuthenticationInfoResOld_SEQUENCE(){
	strcpy(node_type_name, "SendAuthenticationInfoResOld_SEQUENCE");
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
asn1::gsmmap::SendAuthenticationInfoResOld_SEQUENCE::~SendAuthenticationInfoResOld_SEQUENCE(){

}

//RAND
/*
asn1::gsmmap::RAND::RAND(){
	strcpy(node_type_name, "RAND");

}
asn1::gsmmap::RAND::~RAND(){

}
*/

//SRES
/*
asn1::gsmmap::SRES::SRES(){
	strcpy(node_type_name, "SRES");

}
asn1::gsmmap::SRES::~SRES(){

}
*/

//Kc
/*
asn1::gsmmap::Kc::Kc(){
	strcpy(node_type_name, "Kc");

}
asn1::gsmmap::Kc::~Kc(){

}
*/

//SendIdentificationResV2
asn1::gsmmap::SendIdentificationResV2::SendIdentificationResV2(){
	strcpy(node_type_name, "SendIdentificationResV2");
	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// tripletList
	_tripletList = NULL;
	children.push_back(_tripletList);


}
asn1::gsmmap::SendIdentificationResV2::~SendIdentificationResV2(){

}

asn1::ASN1Node* asn1::gsmmap::SendIdentificationResV2::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_imsi = new IMSI();
			children[0] = _imsi;
			return _imsi;
		case 1:
			_tripletList = new TripletListold();
			children[1] = _tripletList;
			return _tripletList;

		default: return NULL;
	}
}

void asn1::gsmmap::SendIdentificationResV2::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(0);
}

void asn1::gsmmap::SendIdentificationResV2::set_tripletList(){
	if(_tripletList == NULL) _tripletList = (TripletListold*)create_node(1);
}

//TripletListold
asn1::gsmmap::TripletListold::TripletListold(){
	strcpy(node_type_name, "TripletListold");

}
asn1::gsmmap::TripletListold::~TripletListold(){
}

asn1::gsmmap::AuthenticationTriplet_v2* asn1::gsmmap::TripletListold::get_child(unsigned int child_index){
	if(child_index < children.size()) return (AuthenticationTriplet_v2*)children[child_index]; else return NULL;
}

void asn1::gsmmap::TripletListold::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::gsmmap::TripletListold::create_node(unsigned int _index){
	children[_index] = new AuthenticationTriplet_v2();
	return children[_index];
}

asn1::ASN1Node* asn1::gsmmap::TripletListold::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new AuthenticationTriplet_v2());
		return children[children.size() - 1];
	}

}

//AuthenticationTriplet_v2
asn1::gsmmap::AuthenticationTriplet_v2::AuthenticationTriplet_v2(){
	strcpy(node_type_name, "AuthenticationTriplet_v2");
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
asn1::gsmmap::AuthenticationTriplet_v2::~AuthenticationTriplet_v2(){

}

//SIWFSSignallingModifyArg
asn1::gsmmap::SIWFSSignallingModifyArg::SIWFSSignallingModifyArg(){
	strcpy(node_type_name, "SIWFSSignallingModifyArg");
	// channelType
	_channelType = NULL;
	children.push_back(_channelType);

	// chosenChannel
	_chosenChannel = NULL;
	children.push_back(_chosenChannel);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::gsmmap::SIWFSSignallingModifyArg::~SIWFSSignallingModifyArg(){

}

asn1::ASN1Node* asn1::gsmmap::SIWFSSignallingModifyArg::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_channelType = new ExternalSignalInfo();
			_channelType->tlv->tag_class = CONTEXT_SPECIFIC;
			_channelType->tlv->tag_value = 0;
			children[0] = _channelType;
			return _channelType;
		case 1:
			_chosenChannel = new ExternalSignalInfo();
			_chosenChannel->tlv->tag_class = CONTEXT_SPECIFIC;
			_chosenChannel->tlv->tag_value = 1;
			children[1] = _chosenChannel;
			return _chosenChannel;
		case 2:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::gsmmap::SIWFSSignallingModifyArg::set_channelType(){
	if(_channelType == NULL) _channelType = (ExternalSignalInfo*)create_node(0);
}

void asn1::gsmmap::SIWFSSignallingModifyArg::set_chosenChannel(){
	if(_chosenChannel == NULL) _chosenChannel = (ExternalSignalInfo*)create_node(1);
}

void asn1::gsmmap::SIWFSSignallingModifyArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

//SIWFSSignallingModifyRes
asn1::gsmmap::SIWFSSignallingModifyRes::SIWFSSignallingModifyRes(){
	strcpy(node_type_name, "SIWFSSignallingModifyRes");
	// channelType
	_channelType = NULL;
	children.push_back(_channelType);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::gsmmap::SIWFSSignallingModifyRes::~SIWFSSignallingModifyRes(){

}

asn1::ASN1Node* asn1::gsmmap::SIWFSSignallingModifyRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_channelType = new ExternalSignalInfo();
			_channelType->tlv->tag_class = CONTEXT_SPECIFIC;
			_channelType->tlv->tag_value = 0;
			children[0] = _channelType;
			return _channelType;
		case 1:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 1;
			children[1] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::gsmmap::SIWFSSignallingModifyRes::set_channelType(){
	if(_channelType == NULL) _channelType = (ExternalSignalInfo*)create_node(0);
}

void asn1::gsmmap::SIWFSSignallingModifyRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(1);
}

//NewPassword
asn1::gsmmap::NewPassword::NewPassword(){
	strcpy(node_type_name, "NewPassword");

}
asn1::gsmmap::NewPassword::~NewPassword(){

}

//GetPasswordArg
asn1::gsmmap::GetPasswordArg::GetPasswordArg(){
	strcpy(node_type_name, "GetPasswordArg");

}
asn1::gsmmap::GetPasswordArg::~GetPasswordArg(){

}

//CurrentPassword
asn1::gsmmap::CurrentPassword::CurrentPassword(){
	strcpy(node_type_name, "CurrentPassword");

}
asn1::gsmmap::CurrentPassword::~CurrentPassword(){

}

//SecureTransportArg
asn1::gsmmap::SecureTransportArg::SecureTransportArg(){
	strcpy(node_type_name, "SecureTransportArg");
	// securityHeader
	_securityHeader = NULL;
	_securityHeader = new SecurityHeader();
	children.push_back(_securityHeader);

	// protectedPayload
	_protectedPayload = NULL;
	children.push_back(_protectedPayload);


}
asn1::gsmmap::SecureTransportArg::~SecureTransportArg(){

}

asn1::ASN1Node* asn1::gsmmap::SecureTransportArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_protectedPayload = new ProtectedPayload();
			children[1] = _protectedPayload;
			return _protectedPayload;

		default: return NULL;
	}
}

void asn1::gsmmap::SecureTransportArg::set_protectedPayload(){
	if(_protectedPayload == NULL) _protectedPayload = (ProtectedPayload*)create_node(1);
}

//SecureTransportErrorParam
asn1::gsmmap::SecureTransportErrorParam::SecureTransportErrorParam(){
	strcpy(node_type_name, "SecureTransportErrorParam");
	// securityHeader
	_securityHeader = NULL;
	_securityHeader = new SecurityHeader();
	children.push_back(_securityHeader);

	// protectedPayload
	_protectedPayload = NULL;
	children.push_back(_protectedPayload);


}
asn1::gsmmap::SecureTransportErrorParam::~SecureTransportErrorParam(){

}

asn1::ASN1Node* asn1::gsmmap::SecureTransportErrorParam::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_protectedPayload = new ProtectedPayload();
			children[1] = _protectedPayload;
			return _protectedPayload;

		default: return NULL;
	}
}

void asn1::gsmmap::SecureTransportErrorParam::set_protectedPayload(){
	if(_protectedPayload == NULL) _protectedPayload = (ProtectedPayload*)create_node(1);
}

//SecureTransportRes
asn1::gsmmap::SecureTransportRes::SecureTransportRes(){
	strcpy(node_type_name, "SecureTransportRes");
	// securityHeader
	_securityHeader = NULL;
	_securityHeader = new SecurityHeader();
	children.push_back(_securityHeader);

	// protectedPayload
	_protectedPayload = NULL;
	children.push_back(_protectedPayload);


}
asn1::gsmmap::SecureTransportRes::~SecureTransportRes(){

}

asn1::ASN1Node* asn1::gsmmap::SecureTransportRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_protectedPayload = new ProtectedPayload();
			children[1] = _protectedPayload;
			return _protectedPayload;

		default: return NULL;
	}
}

void asn1::gsmmap::SecureTransportRes::set_protectedPayload(){
	if(_protectedPayload == NULL) _protectedPayload = (ProtectedPayload*)create_node(1);
}

//SecurityHeader
asn1::gsmmap::SecurityHeader::SecurityHeader(){
	strcpy(node_type_name, "SecurityHeader");
	// securityParametersIndex
	_securityParametersIndex = NULL;
	_securityParametersIndex = new SecurityParametersIndex();
	children.push_back(_securityParametersIndex);

	// originalComponentIdentifier
	_originalComponentIdentifier = NULL;
	_originalComponentIdentifier = new OriginalComponentIdentifier();
	children.push_back(_originalComponentIdentifier);

	// initialisationVector
	_initialisationVector = NULL;
	children.push_back(_initialisationVector);


}
asn1::gsmmap::SecurityHeader::~SecurityHeader(){

}

asn1::ASN1Node* asn1::gsmmap::SecurityHeader::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_initialisationVector = new InitialisationVector();
			children[2] = _initialisationVector;
			return _initialisationVector;

		default: return NULL;
	}
}

void asn1::gsmmap::SecurityHeader::set_initialisationVector(){
	if(_initialisationVector == NULL) _initialisationVector = (InitialisationVector*)create_node(2);
}

//ProtectedPayload
asn1::gsmmap::ProtectedPayload::ProtectedPayload(){
	strcpy(node_type_name, "ProtectedPayload");

}
asn1::gsmmap::ProtectedPayload::~ProtectedPayload(){

}

//SecurityParametersIndex
asn1::gsmmap::SecurityParametersIndex::SecurityParametersIndex(){
	strcpy(node_type_name, "SecurityParametersIndex");

}
asn1::gsmmap::SecurityParametersIndex::~SecurityParametersIndex(){

}

//InitialisationVector
asn1::gsmmap::InitialisationVector::InitialisationVector(){
	strcpy(node_type_name, "InitialisationVector");

}
asn1::gsmmap::InitialisationVector::~InitialisationVector(){

}

//OriginalComponentIdentifier
asn1::gsmmap::OriginalComponentIdentifier::OriginalComponentIdentifier(){
	strcpy(node_type_name, "OriginalComponentIdentifier");
        // operationCode
        _operationCode = NULL;
        _operationCode = new OperationCode();
        ASN1Node* _operationCode_wrapper = new ASN1Node();
        _operationCode_wrapper->tlv = new TLVNode();
        _operationCode_wrapper->tlv->is_explicit = true;
        _operationCode_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
        _operationCode_wrapper->tlv->tag_value = 0;
        _operationCode_wrapper->children.push_back(_operationCode);
        children.push_back(_operationCode_wrapper);

        // errorCode
        _errorCode = NULL;
        _errorCode = new ErrorCode();
        ASN1Node* _errorCode_wrapper = new ASN1Node();
        _errorCode_wrapper->tlv = new TLVNode();
        _errorCode_wrapper->tlv->is_explicit = true;
        _errorCode_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
        _errorCode_wrapper->tlv->tag_value = 1;
        _errorCode_wrapper->children.push_back(_errorCode);
        children.push_back(_errorCode_wrapper);

	// userInfo
	_userInfo = NULL;
	_userInfo = new Null();
	_userInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_userInfo->tlv->tag_value = 2;
	children.push_back(_userInfo);


}
asn1::gsmmap::OriginalComponentIdentifier::~OriginalComponentIdentifier(){

}

//OperationCode
asn1::gsmmap::OperationCode::OperationCode(){
	strcpy(node_type_name, "OperationCode");
	// localValue
	_localValue = NULL;
	_localValue = new Integer();
	children.push_back(_localValue);

	// globalValue
	_globalValue = NULL;
	_globalValue = new Object_identifier();
	children.push_back(_globalValue);


}
asn1::gsmmap::OperationCode::~OperationCode(){

}

//ErrorCode
asn1::gsmmap::ErrorCode::ErrorCode(){
	strcpy(node_type_name, "ErrorCode");
	// localValue
	_localValue = NULL;
	_localValue = new Integer();
	children.push_back(_localValue);

	// globalValue
	_globalValue = NULL;
	_globalValue = new Object_identifier();
	children.push_back(_globalValue);


}
asn1::gsmmap::ErrorCode::~ErrorCode(){

}

//PlmnContainer
asn1::gsmmap::PlmnContainer::PlmnContainer(){
	strcpy(node_type_name, "PlmnContainer");
	tlv->tag_class = PRIVATE;
	tlv->tag_value = 2;
	// msisdn
	_msisdn = NULL;
	children.push_back(_msisdn);

	// category
	_category = NULL;
	children.push_back(_category);

	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// operatorSS_Code
	_operatorSS_Code = NULL;
	children.push_back(_operatorSS_Code);


}
asn1::gsmmap::PlmnContainer::~PlmnContainer(){

}

asn1::ASN1Node* asn1::gsmmap::PlmnContainer::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_msisdn = new ISDN_AddressString();
			_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn->tlv->tag_value = 0;
			children[0] = _msisdn;
			return _msisdn;
		case 1:
			_category = new Category();
			_category->tlv->tag_class = CONTEXT_SPECIFIC;
			_category->tlv->tag_value = 1;
			children[1] = _category;
			return _category;
		case 2:
			_basicService = new BasicServiceCode();
			children[2] = _basicService;
			return _basicService;
		case 3:
			_operatorSS_Code = new PlmnContainer_operatorSS_Code();
			_operatorSS_Code->tlv->tag_class = CONTEXT_SPECIFIC;
			_operatorSS_Code->tlv->tag_value = 4;
			children[3] = _operatorSS_Code;
			return _operatorSS_Code;

		default: return NULL;
	}
}

void asn1::gsmmap::PlmnContainer::set_msisdn(){
	if(_msisdn == NULL) _msisdn = (ISDN_AddressString*)create_node(0);
}

void asn1::gsmmap::PlmnContainer::set_category(){
	if(_category == NULL) _category = (Category*)create_node(1);
}

void asn1::gsmmap::PlmnContainer::set_basicService(){
	if(_basicService == NULL) _basicService = (BasicServiceCode*)create_node(2);
}

void asn1::gsmmap::PlmnContainer::set_operatorSS_Code(){
	if(_operatorSS_Code == NULL) _operatorSS_Code = (PlmnContainer_operatorSS_Code*)create_node(3);
}

//PlmnContainer_operatorSS_Code
asn1::gsmmap::PlmnContainer_operatorSS_Code::PlmnContainer_operatorSS_Code(){
	strcpy(node_type_name, "PlmnContainer_operatorSS_Code");

}
asn1::gsmmap::PlmnContainer_operatorSS_Code::~PlmnContainer_operatorSS_Code(){
}

asn1::Octet_string* asn1::gsmmap::PlmnContainer_operatorSS_Code::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Octet_string*)children[child_index]; else return NULL;
}

void asn1::gsmmap::PlmnContainer_operatorSS_Code::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::gsmmap::PlmnContainer_operatorSS_Code::create_node(unsigned int _index){
	children[_index] = new Octet_string();
	return children[_index];
}

asn1::ASN1Node* asn1::gsmmap::PlmnContainer_operatorSS_Code::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Octet_string());
		return children[children.size() - 1];
	}

}

//Category
/*
asn1::gsmmap::Category::Category(){
	strcpy(node_type_name, "Category");

}
asn1::gsmmap::Category::~Category(){

}
*/

//ForwardSM_Arg
asn1::gsmmap::ForwardSM_Arg::ForwardSM_Arg(){
	strcpy(node_type_name, "ForwardSM_Arg");
	// sm_RP_DA
	_sm_RP_DA = NULL;
	_sm_RP_DA = new SM_RP_DAold();
	children.push_back(_sm_RP_DA);

	// sm_RP_OA
	_sm_RP_OA = NULL;
	_sm_RP_OA = new SM_RP_OAold();
	children.push_back(_sm_RP_OA);

	// sm_RP_UI
	_sm_RP_UI = NULL;
	_sm_RP_UI = new SignalInfo();
	children.push_back(_sm_RP_UI);

	// moreMessagesToSend
	_moreMessagesToSend = NULL;
	children.push_back(_moreMessagesToSend);


}
asn1::gsmmap::ForwardSM_Arg::~ForwardSM_Arg(){

}

asn1::ASN1Node* asn1::gsmmap::ForwardSM_Arg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_moreMessagesToSend = new Null();
			children[3] = _moreMessagesToSend;
			return _moreMessagesToSend;

		default: return NULL;
	}
}

void asn1::gsmmap::ForwardSM_Arg::set_moreMessagesToSend(){
	if(_moreMessagesToSend == NULL) _moreMessagesToSend = (Null*)create_node(3);
}

//SM_RP_DAold
asn1::gsmmap::SM_RP_DAold::SM_RP_DAold(){
	strcpy(node_type_name, "SM_RP_DAold");
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
asn1::gsmmap::SM_RP_DAold::~SM_RP_DAold(){

}

//SM_RP_OAold
asn1::gsmmap::SM_RP_OAold::SM_RP_OAold(){
	strcpy(node_type_name, "SM_RP_OAold");
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
asn1::gsmmap::SM_RP_OAold::~SM_RP_OAold(){

}

//accessType_id
asn1::gsmmap::accessType_id::accessType_id(){
	strcpy(node_type_name, "accessType_id");

}
asn1::gsmmap::accessType_id::~accessType_id(){

}

//accessTypeNotAllowed_id
asn1::gsmmap::accessTypeNotAllowed_id::accessTypeNotAllowed_id(){
	strcpy(node_type_name, "accessTypeNotAllowed_id");

}
asn1::gsmmap::accessTypeNotAllowed_id::~accessTypeNotAllowed_id(){

}

//SendRoutingInfoArgV2
asn1::gsmmap::SendRoutingInfoArgV2::SendRoutingInfoArgV2(){
	strcpy(node_type_name, "SendRoutingInfoArgV2");
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

	// networkSignalInfo
	_networkSignalInfo = NULL;
	children.push_back(_networkSignalInfo);


}
asn1::gsmmap::SendRoutingInfoArgV2::~SendRoutingInfoArgV2(){

}

asn1::ASN1Node* asn1::gsmmap::SendRoutingInfoArgV2::create_node(unsigned int _index){
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
		case 3:
			_networkSignalInfo = new ExternalSignalInfo();
			_networkSignalInfo->tlv->tag_class = CONTEXT_SPECIFIC;
			_networkSignalInfo->tlv->tag_value = 10;
			children[3] = _networkSignalInfo;
			return _networkSignalInfo;

		default: return NULL;
	}
}

void asn1::gsmmap::SendRoutingInfoArgV2::set_cug_CheckInfo(){
	if(_cug_CheckInfo == NULL) _cug_CheckInfo = (CUG_CheckInfo*)create_node(1);
}

void asn1::gsmmap::SendRoutingInfoArgV2::set_numberOfForwarding(){
	if(_numberOfForwarding == NULL) _numberOfForwarding = (NumberOfForwarding*)create_node(2);
}

void asn1::gsmmap::SendRoutingInfoArgV2::set_networkSignalInfo(){
	if(_networkSignalInfo == NULL) _networkSignalInfo = (ExternalSignalInfo*)create_node(3);
}

//SendRoutingInfoResV2
asn1::gsmmap::SendRoutingInfoResV2::SendRoutingInfoResV2(){
	strcpy(node_type_name, "SendRoutingInfoResV2");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// routingInfo
	_routingInfo = NULL;
	_routingInfo = new RoutingInfo();
	children.push_back(_routingInfo);

	// cug_CheckInfo
	_cug_CheckInfo = NULL;
	children.push_back(_cug_CheckInfo);


}
asn1::gsmmap::SendRoutingInfoResV2::~SendRoutingInfoResV2(){

}

asn1::ASN1Node* asn1::gsmmap::SendRoutingInfoResV2::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_cug_CheckInfo = new CUG_CheckInfo();
			children[2] = _cug_CheckInfo;
			return _cug_CheckInfo;

		default: return NULL;
	}
}

void asn1::gsmmap::SendRoutingInfoResV2::set_cug_CheckInfo(){
	if(_cug_CheckInfo == NULL) _cug_CheckInfo = (CUG_CheckInfo*)create_node(2);
}

//BeginSubscriberActivityArg
asn1::gsmmap::BeginSubscriberActivityArg::BeginSubscriberActivityArg(){
	strcpy(node_type_name, "BeginSubscriberActivityArg");
	// imsi
	_imsi = NULL;
	_imsi = new IMSI();
	children.push_back(_imsi);

	// originatingEntityNumber
	_originatingEntityNumber = NULL;
	_originatingEntityNumber = new ISDN_AddressString();
	children.push_back(_originatingEntityNumber);

	// msisdn
	_msisdn = NULL;
	children.push_back(_msisdn);


}
asn1::gsmmap::BeginSubscriberActivityArg::~BeginSubscriberActivityArg(){

}

asn1::ASN1Node* asn1::gsmmap::BeginSubscriberActivityArg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_msisdn = new AddressString();
			_msisdn->tlv->tag_class = PRIVATE;
			_msisdn->tlv->tag_value = 28;
			children[2] = _msisdn;
			return _msisdn;

		default: return NULL;
	}
}

void asn1::gsmmap::BeginSubscriberActivityArg::set_msisdn(){
	if(_msisdn == NULL) _msisdn = (AddressString*)create_node(2);
}

