#include<DialoguePDUs.h>
#include<iostream>

//dialogue_as_id
asn1::dialogue_as_id::dialogue_as_id(){
	strcpy(node_type_name, "dialogue_as_id");

}
asn1::dialogue_as_id::~dialogue_as_id(){

}

//DialoguePDU
asn1::DialoguePDU::DialoguePDU(){
	strcpy(node_type_name, "DialoguePDU");
	// dialogueRequest
	_dialogueRequest = NULL;
	_dialogueRequest = new AARQ_apdu();
	children.push_back(_dialogueRequest);

	// dialogueResponse
	_dialogueResponse = NULL;
	_dialogueResponse = new AARE_apdu();
	children.push_back(_dialogueResponse);

	// dialogueAbort
	_dialogueAbort = NULL;
	_dialogueAbort = new ABRT_apdu();
	children.push_back(_dialogueAbort);


}
asn1::DialoguePDU::~DialoguePDU(){

}

//AARQ_apdu
asn1::AARQ_apdu::AARQ_apdu(){
	strcpy(node_type_name, "AARQ_apdu");
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 0;
	// protocol_version
	_protocol_version = NULL;
	_protocol_version = new Bit_string();
	_protocol_version->tlv->tag_class = CONTEXT_SPECIFIC;
	_protocol_version->tlv->tag_value = 0;
	children.push_back(_protocol_version);

	// application_context_name
	_application_context_name = NULL;
	_application_context_name = new Object_identifier();
	ASN1Node* _application_context_name_wrapper = new ASN1Node();
	_application_context_name_wrapper->tlv = new TLVNode();
	_application_context_name_wrapper->tlv->is_explicit = true;
	_application_context_name_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
	_application_context_name_wrapper->tlv->tag_value = 1;
	_application_context_name_wrapper->children.push_back(_application_context_name);
	children.push_back(_application_context_name_wrapper);

	// user_information
	_user_information = NULL;
	children.push_back(_user_information);


}
asn1::AARQ_apdu::~AARQ_apdu(){

}

asn1::ASN1Node* asn1::AARQ_apdu::create_node(unsigned int _index){
	switch(_index){
		case 2:
		{
			_user_information = new AARQ_apdu_user_information();
			_user_information->tlv->tag_class = CONTEXT_SPECIFIC;
			_user_information->tlv->tag_value = 30;
			children[2] = _user_information;
			return _user_information;
		}

		default: return NULL;
	}
}

void asn1::AARQ_apdu::set_user_information(){
	if(_user_information == NULL) _user_information = (AARQ_apdu_user_information*)create_node(2);
}

//AARQ_apdu_user_information
asn1::AARQ_apdu_user_information::AARQ_apdu_user_information(){
	strcpy(node_type_name, "AARQ_apdu_user_information");

}
asn1::AARQ_apdu_user_information::~AARQ_apdu_user_information(){
}

asn1::External* asn1::AARQ_apdu_user_information::get_child(unsigned int child_index){
	if(child_index < children.size()) return (External*)children[child_index]; else return NULL;
}

void asn1::AARQ_apdu_user_information::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::AARQ_apdu_user_information::create_node(unsigned int _index){
	children[_index] = new External();
	return children[_index];
}

asn1::ASN1Node* asn1::AARQ_apdu_user_information::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new External());
		return children[children.size() - 1];
	}

}

//AARE_apdu
asn1::AARE_apdu::AARE_apdu(){
	strcpy(node_type_name, "AARE_apdu");
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 1;
	// protocol_version
	_protocol_version = NULL;
	_protocol_version = new Bit_string();
	_protocol_version->tlv->tag_class = CONTEXT_SPECIFIC;
	_protocol_version->tlv->tag_value = 0;
	children.push_back(_protocol_version);

	// application_context_name
	_application_context_name = NULL;
	_application_context_name = new Object_identifier();
	ASN1Node* _application_context_name_wrapper = new ASN1Node();
	_application_context_name_wrapper->tlv = new TLVNode();
	_application_context_name_wrapper->tlv->is_explicit = true;
	_application_context_name_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
	_application_context_name_wrapper->tlv->tag_value = 1;
	_application_context_name_wrapper->children.push_back(_application_context_name);
	children.push_back(_application_context_name_wrapper);

	// result
	_result = NULL;
	_result = new Associate_result();
	ASN1Node* _result_wrapper = new ASN1Node();
	_result_wrapper->tlv = new TLVNode();
	_result_wrapper->tlv->is_explicit = true;
	_result_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
	_result_wrapper->tlv->tag_value = 2;
	_result_wrapper->children.push_back(_result);
	children.push_back(_result_wrapper);

	// result_source_diagnostic
	_result_source_diagnostic = NULL;
	_result_source_diagnostic = new Associate_source_diagnostic();
	ASN1Node* _result_source_diagnostic_wrapper = new ASN1Node();
	_result_source_diagnostic_wrapper->tlv = new TLVNode();
	_result_source_diagnostic_wrapper->tlv->is_explicit = true;
	_result_source_diagnostic_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
	_result_source_diagnostic_wrapper->tlv->tag_value = 3;
	_result_source_diagnostic_wrapper->children.push_back(_result_source_diagnostic);
	children.push_back(_result_source_diagnostic_wrapper);

	// user_information
	_user_information = NULL;
	children.push_back(_user_information);


}
asn1::AARE_apdu::~AARE_apdu(){

}

asn1::ASN1Node* asn1::AARE_apdu::create_node(unsigned int _index){
	switch(_index){
		case 4:
		{
			_user_information = new AARE_apdu_user_information();
			_user_information->tlv->tag_class = CONTEXT_SPECIFIC;
			_user_information->tlv->tag_value = 30;
			children[4] = _user_information;
			return _user_information;
		}

		default: return NULL;
	}
}

void asn1::AARE_apdu::set_user_information(){
	if(_user_information == NULL) _user_information = (AARE_apdu_user_information*)create_node(4);
}

//AARE_apdu_user_information
asn1::AARE_apdu_user_information::AARE_apdu_user_information(){
	strcpy(node_type_name, "AARE_apdu_user_information");

}
asn1::AARE_apdu_user_information::~AARE_apdu_user_information(){
}

asn1::External* asn1::AARE_apdu_user_information::get_child(unsigned int child_index){
	if(child_index < children.size()) return (External*)children[child_index]; else return NULL;
}

void asn1::AARE_apdu_user_information::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::AARE_apdu_user_information::create_node(unsigned int _index){
	children[_index] = new External();
	return children[_index];
}

asn1::ASN1Node* asn1::AARE_apdu_user_information::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new External());
		return children[children.size() - 1];
	}

}

//RLRQ_apdu
asn1::RLRQ_apdu::RLRQ_apdu(){
	strcpy(node_type_name, "RLRQ_apdu");
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 2;
	// reason
	_reason = NULL;
	children.push_back(_reason);

	// user_information
	_user_information = NULL;
	children.push_back(_user_information);


}
asn1::RLRQ_apdu::~RLRQ_apdu(){

}

asn1::ASN1Node* asn1::RLRQ_apdu::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_reason = new Release_request_reason();
			_reason->tlv->tag_class = CONTEXT_SPECIFIC;
			_reason->tlv->tag_value = 0;
			children[0] = _reason;
			return _reason;
		}
		case 1:
		{
			_user_information = new RLRQ_apdu_user_information();
			_user_information->tlv->tag_class = CONTEXT_SPECIFIC;
			_user_information->tlv->tag_value = 30;
			children[1] = _user_information;
			return _user_information;
		}

		default: return NULL;
	}
}

void asn1::RLRQ_apdu::set_reason(){
	if(_reason == NULL) _reason = (Release_request_reason*)create_node(0);
}

void asn1::RLRQ_apdu::set_user_information(){
	if(_user_information == NULL) _user_information = (RLRQ_apdu_user_information*)create_node(1);
}

//RLRQ_apdu_user_information
asn1::RLRQ_apdu_user_information::RLRQ_apdu_user_information(){
	strcpy(node_type_name, "RLRQ_apdu_user_information");

}
asn1::RLRQ_apdu_user_information::~RLRQ_apdu_user_information(){
}

asn1::External* asn1::RLRQ_apdu_user_information::get_child(unsigned int child_index){
	if(child_index < children.size()) return (External*)children[child_index]; else return NULL;
}

void asn1::RLRQ_apdu_user_information::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::RLRQ_apdu_user_information::create_node(unsigned int _index){
	children[_index] = new External();
	return children[_index];
}

asn1::ASN1Node* asn1::RLRQ_apdu_user_information::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new External());
		return children[children.size() - 1];
	}

}

//RLRE_apdu
asn1::RLRE_apdu::RLRE_apdu(){
	strcpy(node_type_name, "RLRE_apdu");
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 3;
	// reason
	_reason = NULL;
	children.push_back(_reason);

	// user_information
	_user_information = NULL;
	children.push_back(_user_information);


}
asn1::RLRE_apdu::~RLRE_apdu(){

}

asn1::ASN1Node* asn1::RLRE_apdu::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_reason = new Release_response_reason();
			_reason->tlv->tag_class = CONTEXT_SPECIFIC;
			_reason->tlv->tag_value = 0;
			children[0] = _reason;
			return _reason;
		}
		case 1:
		{
			_user_information = new RLRE_apdu_user_information();
			_user_information->tlv->tag_class = CONTEXT_SPECIFIC;
			_user_information->tlv->tag_value = 30;
			children[1] = _user_information;
			return _user_information;
		}

		default: return NULL;
	}
}

void asn1::RLRE_apdu::set_reason(){
	if(_reason == NULL) _reason = (Release_response_reason*)create_node(0);
}

void asn1::RLRE_apdu::set_user_information(){
	if(_user_information == NULL) _user_information = (RLRE_apdu_user_information*)create_node(1);
}

//RLRE_apdu_user_information
asn1::RLRE_apdu_user_information::RLRE_apdu_user_information(){
	strcpy(node_type_name, "RLRE_apdu_user_information");

}
asn1::RLRE_apdu_user_information::~RLRE_apdu_user_information(){
}

asn1::External* asn1::RLRE_apdu_user_information::get_child(unsigned int child_index){
	if(child_index < children.size()) return (External*)children[child_index]; else return NULL;
}

void asn1::RLRE_apdu_user_information::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::RLRE_apdu_user_information::create_node(unsigned int _index){
	children[_index] = new External();
	return children[_index];
}

asn1::ASN1Node* asn1::RLRE_apdu_user_information::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new External());
		return children[children.size() - 1];
	}

}

//ABRT_apdu
asn1::ABRT_apdu::ABRT_apdu(){
	strcpy(node_type_name, "ABRT_apdu");
	tlv->tag_class = APPLICATION;
	tlv->tag_value = 4;
	// abort_source
	_abort_source = NULL;
	_abort_source = new ABRT_source();
	_abort_source->tlv->tag_class = CONTEXT_SPECIFIC;
	_abort_source->tlv->tag_value = 0;
	children.push_back(_abort_source);

	// user_information
	_user_information = NULL;
	children.push_back(_user_information);


}
asn1::ABRT_apdu::~ABRT_apdu(){

}

asn1::ASN1Node* asn1::ABRT_apdu::create_node(unsigned int _index){
	switch(_index){
		case 1:
		{
			_user_information = new ABRT_apdu_user_information();
			_user_information->tlv->tag_class = CONTEXT_SPECIFIC;
			_user_information->tlv->tag_value = 30;
			children[1] = _user_information;
			return _user_information;
		}

		default: return NULL;
	}
}

void asn1::ABRT_apdu::set_user_information(){
	if(_user_information == NULL) _user_information = (ABRT_apdu_user_information*)create_node(1);
}

//ABRT_apdu_user_information
asn1::ABRT_apdu_user_information::ABRT_apdu_user_information(){
	strcpy(node_type_name, "ABRT_apdu_user_information");

}
asn1::ABRT_apdu_user_information::~ABRT_apdu_user_information(){
}

asn1::External* asn1::ABRT_apdu_user_information::get_child(unsigned int child_index){
	if(child_index < children.size()) return (External*)children[child_index]; else return NULL;
}

void asn1::ABRT_apdu_user_information::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::ABRT_apdu_user_information::create_node(unsigned int _index){
	children[_index] = new External();
	return children[_index];
}

asn1::ASN1Node* asn1::ABRT_apdu_user_information::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new External());
		return children[children.size() - 1];
	}

}

//ABRT_source
asn1::ABRT_source::ABRT_source(){
	strcpy(node_type_name, "ABRT_source");

}
asn1::ABRT_source::~ABRT_source(){

}

//Associate_result
asn1::Associate_result::Associate_result(){
	strcpy(node_type_name, "Associate_result");

}
asn1::Associate_result::~Associate_result(){

}

//Associate_source_diagnostic
asn1::Associate_source_diagnostic::Associate_source_diagnostic(){
	strcpy(node_type_name, "Associate_source_diagnostic");
	// dialogue_service_user
	_dialogue_service_user = NULL;
	_dialogue_service_user = new Integer();
	ASN1Node* _dialogue_service_user_wrapper = new ASN1Node();
	_dialogue_service_user_wrapper->tlv = new TLVNode();
	_dialogue_service_user_wrapper->tlv->is_explicit = true;
	_dialogue_service_user_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
	_dialogue_service_user_wrapper->tlv->tag_value = 1;
	_dialogue_service_user_wrapper->children.push_back(_dialogue_service_user);
	children.push_back(_dialogue_service_user_wrapper);

	// dialogue_service_provider
	_dialogue_service_provider = NULL;
	_dialogue_service_provider = new Integer();
	ASN1Node* _dialogue_service_provider_wrapper = new ASN1Node();
	_dialogue_service_provider_wrapper->tlv = new TLVNode();
	_dialogue_service_provider_wrapper->tlv->is_explicit = true;
	_dialogue_service_provider_wrapper->tlv->tag_class = CONTEXT_SPECIFIC;
	_dialogue_service_provider_wrapper->tlv->tag_value = 2;
	_dialogue_service_provider_wrapper->children.push_back(_dialogue_service_provider);
	children.push_back(_dialogue_service_provider_wrapper);


}
asn1::Associate_source_diagnostic::~Associate_source_diagnostic(){

}

//Release_request_reason
asn1::Release_request_reason::Release_request_reason(){
	strcpy(node_type_name, "Release_request_reason");

}
asn1::Release_request_reason::~Release_request_reason(){

}

//Release_response_reason
asn1::Release_response_reason::Release_response_reason(){
	strcpy(node_type_name, "Release_response_reason");

}
asn1::Release_response_reason::~Release_response_reason(){

}

