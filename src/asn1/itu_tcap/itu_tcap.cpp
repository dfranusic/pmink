#include<itu_tcap.h>
#include<iostream>

//ExternalPDU
asn1::itu_tcap::ExternalPDU::ExternalPDU(){
	strcpy(node_type_name, "ExternalPDU");
	tlv->tag_class = UNIVERSAL;
	tlv->tag_value = 8;
	// oid
	_oid = NULL;
	_oid = new Object_identifier();
	children.push_back(_oid);

	// dialog
	_dialog = NULL;
	_dialog = new Dialog1();
	_dialog->tlv->tag_class = CONTEXT_SPECIFIC;
	_dialog->tlv->tag_value = 0;
	children.push_back(_dialog);


}
asn1::itu_tcap::ExternalPDU::~ExternalPDU(){

}

//Dialog1
asn1::itu_tcap::Dialog1::Dialog1(){
	strcpy(node_type_name, "Dialog1");

}
asn1::itu_tcap::Dialog1::~Dialog1(){

}

//TCMessage
asn1::itu_tcap::TCMessage::TCMessage(){
	strcpy(node_type_name, "TCMessage");
	// unidirectional
	_unidirectional = NULL;
	_unidirectional = new Unidirectional();
	_unidirectional->tlv->tag_class = APPLICATION;
	_unidirectional->tlv->tag_value = 1;
	children.push_back(_unidirectional);

	// begin
	_begin = NULL;
	_begin = new Begin();
	_begin->tlv->tag_class = APPLICATION;
	_begin->tlv->tag_value = 2;
	children.push_back(_begin);

	// end
	_end = NULL;
	_end = new End();
	_end->tlv->tag_class = APPLICATION;
	_end->tlv->tag_value = 4;
	children.push_back(_end);

	// continue
	_continue = NULL;
	_continue = new Continue();
	_continue->tlv->tag_class = APPLICATION;
	_continue->tlv->tag_value = 5;
	children.push_back(_continue);

	// abort
	_abort = NULL;
	_abort = new Abort();
	_abort->tlv->tag_class = APPLICATION;
	_abort->tlv->tag_value = 7;
	children.push_back(_abort);


}
asn1::itu_tcap::TCMessage::~TCMessage(){

}

//Unidirectional
asn1::itu_tcap::Unidirectional::Unidirectional(){
	strcpy(node_type_name, "Unidirectional");
	// dialoguePortion
	_dialoguePortion = NULL;
	children.push_back(_dialoguePortion);

	// components
	_components = NULL;
	_components = new ComponentPortion();
	children.push_back(_components);


}
asn1::itu_tcap::Unidirectional::~Unidirectional(){

}

asn1::ASN1Node* asn1::itu_tcap::Unidirectional::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_dialoguePortion = new DialoguePortion();
			children[0] = _dialoguePortion;
			return _dialoguePortion;
		}

		default: return NULL;
	}
}

void asn1::itu_tcap::Unidirectional::set_dialoguePortion(){
	if(_dialoguePortion == NULL) _dialoguePortion = (DialoguePortion*)create_node(0);
}

//Begin
asn1::itu_tcap::Begin::Begin(){
	strcpy(node_type_name, "Begin");
	// otid
	_otid = NULL;
	_otid = new OrigTransactionID();
	children.push_back(_otid);

	// dialoguePortion
	_dialoguePortion = NULL;
	children.push_back(_dialoguePortion);

	// components
	_components = NULL;
	children.push_back(_components);


}
asn1::itu_tcap::Begin::~Begin(){

}

asn1::ASN1Node* asn1::itu_tcap::Begin::create_node(unsigned int _index){
	switch(_index){
		case 1:
		{
			_dialoguePortion = new DialoguePortion();
			children[1] = _dialoguePortion;
			return _dialoguePortion;
		}
		case 2:
		{
			_components = new ComponentPortion();
			children[2] = _components;
			return _components;
		}

		default: return NULL;
	}
}

void asn1::itu_tcap::Begin::set_dialoguePortion(){
	if(_dialoguePortion == NULL) _dialoguePortion = (DialoguePortion*)create_node(1);
}

void asn1::itu_tcap::Begin::set_components(){
	if(_components == NULL) _components = (ComponentPortion*)create_node(2);
}

//End
asn1::itu_tcap::End::End(){
	strcpy(node_type_name, "End");
	// dtid
	_dtid = NULL;
	_dtid = new DestTransactionID();
	children.push_back(_dtid);

	// dialoguePortion
	_dialoguePortion = NULL;
	children.push_back(_dialoguePortion);

	// components
	_components = NULL;
	children.push_back(_components);


}
asn1::itu_tcap::End::~End(){

}

asn1::ASN1Node* asn1::itu_tcap::End::create_node(unsigned int _index){
	switch(_index){
		case 1:
		{
			_dialoguePortion = new DialoguePortion();
			children[1] = _dialoguePortion;
			return _dialoguePortion;
		}
		case 2:
		{
			_components = new ComponentPortion();
			children[2] = _components;
			return _components;
		}

		default: return NULL;
	}
}

void asn1::itu_tcap::End::set_dialoguePortion(){
	if(_dialoguePortion == NULL) _dialoguePortion = (DialoguePortion*)create_node(1);
}

void asn1::itu_tcap::End::set_components(){
	if(_components == NULL) _components = (ComponentPortion*)create_node(2);
}

//Continue
asn1::itu_tcap::Continue::Continue(){
	strcpy(node_type_name, "Continue");
	// otid
	_otid = NULL;
	_otid = new OrigTransactionID();
	children.push_back(_otid);

	// dtid
	_dtid = NULL;
	_dtid = new DestTransactionID();
	children.push_back(_dtid);

	// dialoguePortion
	_dialoguePortion = NULL;
	children.push_back(_dialoguePortion);

	// components
	_components = NULL;
	children.push_back(_components);


}
asn1::itu_tcap::Continue::~Continue(){

}

asn1::ASN1Node* asn1::itu_tcap::Continue::create_node(unsigned int _index){
	switch(_index){
		case 2:
		{
			_dialoguePortion = new DialoguePortion();
			children[2] = _dialoguePortion;
			return _dialoguePortion;
		}
		case 3:
		{
			_components = new ComponentPortion();
			children[3] = _components;
			return _components;
		}

		default: return NULL;
	}
}

void asn1::itu_tcap::Continue::set_dialoguePortion(){
	if(_dialoguePortion == NULL) _dialoguePortion = (DialoguePortion*)create_node(2);
}

void asn1::itu_tcap::Continue::set_components(){
	if(_components == NULL) _components = (ComponentPortion*)create_node(3);
}

//Abort
asn1::itu_tcap::Abort::Abort(){
	strcpy(node_type_name, "Abort");
	// dtid
	_dtid = NULL;
	_dtid = new DestTransactionID();
	children.push_back(_dtid);

	// reason
	_reason = NULL;
	children.push_back(_reason);


}
asn1::itu_tcap::Abort::~Abort(){

}

asn1::ASN1Node* asn1::itu_tcap::Abort::create_node(unsigned int _index){
	switch(_index){
		case 1:
		{
			_reason = new Reason();
			children[1] = _reason;
			return _reason;
		}

		default: return NULL;
	}
}

void asn1::itu_tcap::Abort::set_reason(){
	if(_reason == NULL) _reason = (Reason*)create_node(1);
}

//Reason
asn1::itu_tcap::Reason::Reason(){
	strcpy(node_type_name, "Reason");
	// p_abortCause
	_p_abortCause = NULL;
	_p_abortCause = new P_AbortCause();
	children.push_back(_p_abortCause);

	// u_abortCause
	_u_abortCause = NULL;
	_u_abortCause = new DialoguePortion();
	children.push_back(_u_abortCause);


}
asn1::itu_tcap::Reason::~Reason(){

}

//DialoguePortion
asn1::itu_tcap::DialoguePortion::DialoguePortion(){
	strcpy(node_type_name, "DialoguePortion");
	tlv->is_explicit = true;
	 _External_wrapper = new External();
	children.push_back(_External_wrapper);
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 11;

}
asn1::itu_tcap::DialoguePortion::~DialoguePortion(){

}

//OrigTransactionID
asn1::itu_tcap::OrigTransactionID::OrigTransactionID(){
	strcpy(node_type_name, "OrigTransactionID");
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 8;

}
asn1::itu_tcap::OrigTransactionID::~OrigTransactionID(){

}

//DestTransactionID
asn1::itu_tcap::DestTransactionID::DestTransactionID(){
	strcpy(node_type_name, "DestTransactionID");
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 9;

}
asn1::itu_tcap::DestTransactionID::~DestTransactionID(){

}

//P_AbortCause
asn1::itu_tcap::P_AbortCause::P_AbortCause(){
	strcpy(node_type_name, "P_AbortCause");
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 10;

}
asn1::itu_tcap::P_AbortCause::~P_AbortCause(){

}

//ComponentPortion
asn1::itu_tcap::ComponentPortion::ComponentPortion(){
	strcpy(node_type_name, "ComponentPortion");
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 12;

}
asn1::itu_tcap::ComponentPortion::~ComponentPortion(){
}

asn1::itu_tcap::Component* asn1::itu_tcap::ComponentPortion::get_child(unsigned int child_index){
	if(child_index < children.size()) return (Component*)children[child_index]; else return NULL;
}

void asn1::itu_tcap::ComponentPortion::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::itu_tcap::ComponentPortion::create_node(unsigned int _index){
	children[_index] = new Component();
	return children[_index];
}

asn1::ASN1Node* asn1::itu_tcap::ComponentPortion::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new Component());
		return children[children.size() - 1];
	}

}

//Component
asn1::itu_tcap::Component::Component(){
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
asn1::itu_tcap::Component::~Component(){

}

//Invoke
asn1::itu_tcap::Invoke::Invoke(){
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
	_opCode = new OPERATION();
	children.push_back(_opCode);

	// parameter
	_parameter = NULL;
	children.push_back(_parameter);


}
asn1::itu_tcap::Invoke::~Invoke(){

}

asn1::ASN1Node* asn1::itu_tcap::Invoke::create_node(unsigned int _index){
	switch(_index){
		case 1:
		{
			_linkedID = new InvokeIdType();
			_linkedID->tlv->tag_class = CONTEXT_SPECIFIC;
			_linkedID->tlv->tag_value = 0;
			children[1] = _linkedID;
			return _linkedID;
		}
		case 3:
		{
			_parameter = new Parameter();
			children[3] = _parameter;
			return _parameter;
		}

		default: return NULL;
	}
}

void asn1::itu_tcap::Invoke::set_linkedID(){
	if(_linkedID == NULL) _linkedID = (InvokeIdType*)create_node(1);
}

void asn1::itu_tcap::Invoke::set_parameter(){
	if(_parameter == NULL) _parameter = (Parameter*)create_node(3);
}

//Parameter
asn1::itu_tcap::Parameter::Parameter(){
	strcpy(node_type_name, "Parameter");

}
asn1::itu_tcap::Parameter::~Parameter(){

}

//ReturnResult
asn1::itu_tcap::ReturnResult::ReturnResult(){
	strcpy(node_type_name, "ReturnResult");
	// invokeID
	_invokeID = NULL;
	_invokeID = new InvokeIdType();
	children.push_back(_invokeID);

	// resultretres
	_resultretres = NULL;
	children.push_back(_resultretres);


}
asn1::itu_tcap::ReturnResult::~ReturnResult(){

}

asn1::ASN1Node* asn1::itu_tcap::ReturnResult::create_node(unsigned int _index){
	switch(_index){
		case 1:
		{
			_resultretres = new ReturnResult_resultretres();
			children[1] = _resultretres;
			return _resultretres;
		}

		default: return NULL;
	}
}

void asn1::itu_tcap::ReturnResult::set_resultretres(){
	if(_resultretres == NULL) _resultretres = (ReturnResult_resultretres*)create_node(1);
}

//ReturnResult_resultretres
asn1::itu_tcap::ReturnResult_resultretres::ReturnResult_resultretres(){
	strcpy(node_type_name, "ReturnResult_resultretres");
	// opCode
	_opCode = NULL;
	_opCode = new OPERATION();
	children.push_back(_opCode);

	// parameter
	_parameter = NULL;
	children.push_back(_parameter);


}
asn1::itu_tcap::ReturnResult_resultretres::~ReturnResult_resultretres(){

}

asn1::ASN1Node* asn1::itu_tcap::ReturnResult_resultretres::create_node(unsigned int _index){
	switch(_index){
		case 1:
		{
			_parameter = new Parameter();
			children[1] = _parameter;
			return _parameter;
		}

		default: return NULL;
	}
}

void asn1::itu_tcap::ReturnResult_resultretres::set_parameter(){
	if(_parameter == NULL) _parameter = (Parameter*)create_node(1);
}

//ReturnError
asn1::itu_tcap::ReturnError::ReturnError(){
	strcpy(node_type_name, "ReturnError");
	// invokeID
	_invokeID = NULL;
	_invokeID = new InvokeIdType();
	children.push_back(_invokeID);

	// errorCode
	_errorCode = NULL;
	_errorCode = new ErrorCode();
	children.push_back(_errorCode);

	// parameter
	_parameter = NULL;
	children.push_back(_parameter);


}
asn1::itu_tcap::ReturnError::~ReturnError(){

}

asn1::ASN1Node* asn1::itu_tcap::ReturnError::create_node(unsigned int _index){
	switch(_index){
		case 2:
		{
			_parameter = new Parameter();
			children[2] = _parameter;
			return _parameter;
		}

		default: return NULL;
	}
}

void asn1::itu_tcap::ReturnError::set_parameter(){
	if(_parameter == NULL) _parameter = (Parameter*)create_node(2);
}

//Reject
asn1::itu_tcap::Reject::Reject(){
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
asn1::itu_tcap::Reject::~Reject(){

}

//Reject_invokeIDRej
asn1::itu_tcap::Reject_invokeIDRej::Reject_invokeIDRej(){
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
asn1::itu_tcap::Reject_invokeIDRej::~Reject_invokeIDRej(){

}

//Reject_problem
asn1::itu_tcap::Reject_problem::Reject_problem(){
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
asn1::itu_tcap::Reject_problem::~Reject_problem(){

}

//InvokeIdType
asn1::itu_tcap::InvokeIdType::InvokeIdType(){
	strcpy(node_type_name, "InvokeIdType");

}
asn1::itu_tcap::InvokeIdType::~InvokeIdType(){

}

//OPERATION
asn1::itu_tcap::OPERATION::OPERATION(){
	strcpy(node_type_name, "OPERATION");
	// localValue
	_localValue = NULL;
	_localValue = new Integer();
	children.push_back(_localValue);

	// globalValue
	_globalValue = NULL;
	_globalValue = new Object_identifier();
	children.push_back(_globalValue);


}
asn1::itu_tcap::OPERATION::~OPERATION(){

}

//ERROR
asn1::itu_tcap::ERROR::ERROR(){
	strcpy(node_type_name, "ERROR");
	// localValue
	_localValue = NULL;
	_localValue = new Integer();
	children.push_back(_localValue);

	// globalValue
	_globalValue = NULL;
	_globalValue = new Object_identifier();
	children.push_back(_globalValue);


}
asn1::itu_tcap::ERROR::~ERROR(){

}

//GeneralProblem
asn1::itu_tcap::GeneralProblem::GeneralProblem(){
	strcpy(node_type_name, "GeneralProblem");

}
asn1::itu_tcap::GeneralProblem::~GeneralProblem(){

}

//InvokeProblem
asn1::itu_tcap::InvokeProblem::InvokeProblem(){
	strcpy(node_type_name, "InvokeProblem");

}
asn1::itu_tcap::InvokeProblem::~InvokeProblem(){

}

//ReturnResultProblem
asn1::itu_tcap::ReturnResultProblem::ReturnResultProblem(){
	strcpy(node_type_name, "ReturnResultProblem");

}
asn1::itu_tcap::ReturnResultProblem::~ReturnResultProblem(){

}

//ReturnErrorProblem
asn1::itu_tcap::ReturnErrorProblem::ReturnErrorProblem(){
	strcpy(node_type_name, "ReturnErrorProblem");

}
asn1::itu_tcap::ReturnErrorProblem::~ReturnErrorProblem(){

}

//ErrorCode
asn1::itu_tcap::ErrorCode::ErrorCode(){
	strcpy(node_type_name, "ErrorCode");
	// nationaler
	_nationaler = NULL;
	_nationaler = new Integer();
	_nationaler->tlv->tag_class = PRIVATE;
	_nationaler->tlv->tag_value = 19;
	children.push_back(_nationaler);

	// privateer
	_privateer = NULL;
	_privateer = new Integer();
	_privateer->tlv->tag_class = PRIVATE;
	_privateer->tlv->tag_value = 20;
	children.push_back(_privateer);


}
asn1::itu_tcap::ErrorCode::~ErrorCode(){

}

