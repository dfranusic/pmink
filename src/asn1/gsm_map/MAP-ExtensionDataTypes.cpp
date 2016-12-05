#include<MAP-ExtensionDataTypes.h>
#include<iostream>

//ExtensionContainer
asn1::ExtensionContainer::ExtensionContainer(){
	strcpy(node_type_name, "ExtensionContainer");
	// privateExtensionList
	_privateExtensionList = NULL;
	children.push_back(_privateExtensionList);

	// pcs_Extensions
	_pcs_Extensions = NULL;
	children.push_back(_pcs_Extensions);


}
asn1::ExtensionContainer::~ExtensionContainer(){

}

asn1::ASN1Node* asn1::ExtensionContainer::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_privateExtensionList = new PrivateExtensionList();
			_privateExtensionList->tlv->tag_class = CONTEXT_SPECIFIC;
			_privateExtensionList->tlv->tag_value = 0;
			children[0] = _privateExtensionList;
			return _privateExtensionList;
		case 1:
			_pcs_Extensions = new PCS_Extensions();
			_pcs_Extensions->tlv->tag_class = CONTEXT_SPECIFIC;
			_pcs_Extensions->tlv->tag_value = 1;
			children[1] = _pcs_Extensions;
			return _pcs_Extensions;

		default: return NULL;
	}
}

void asn1::ExtensionContainer::set_privateExtensionList(){
	if(_privateExtensionList == NULL) _privateExtensionList = (PrivateExtensionList*)create_node(0);
}

void asn1::ExtensionContainer::set_pcs_Extensions(){
	if(_pcs_Extensions == NULL) _pcs_Extensions = (PCS_Extensions*)create_node(1);
}

//SLR_ArgExtensionContainer
asn1::SLR_ArgExtensionContainer::SLR_ArgExtensionContainer(){
	strcpy(node_type_name, "SLR_ArgExtensionContainer");
	// privateExtensionList
	_privateExtensionList = NULL;
	children.push_back(_privateExtensionList);

	// slr_Arg_PCS_Extensions
	_slr_Arg_PCS_Extensions = NULL;
	children.push_back(_slr_Arg_PCS_Extensions);


}
asn1::SLR_ArgExtensionContainer::~SLR_ArgExtensionContainer(){

}

asn1::ASN1Node* asn1::SLR_ArgExtensionContainer::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_privateExtensionList = new PrivateExtensionList();
			_privateExtensionList->tlv->tag_class = CONTEXT_SPECIFIC;
			_privateExtensionList->tlv->tag_value = 0;
			children[0] = _privateExtensionList;
			return _privateExtensionList;
		case 1:
			_slr_Arg_PCS_Extensions = new SLR_Arg_PCS_Extensions();
			_slr_Arg_PCS_Extensions->tlv->tag_class = CONTEXT_SPECIFIC;
			_slr_Arg_PCS_Extensions->tlv->tag_value = 1;
			children[1] = _slr_Arg_PCS_Extensions;
			return _slr_Arg_PCS_Extensions;

		default: return NULL;
	}
}

void asn1::SLR_ArgExtensionContainer::set_privateExtensionList(){
	if(_privateExtensionList == NULL) _privateExtensionList = (PrivateExtensionList*)create_node(0);
}

void asn1::SLR_ArgExtensionContainer::set_slr_Arg_PCS_Extensions(){
	if(_slr_Arg_PCS_Extensions == NULL) _slr_Arg_PCS_Extensions = (SLR_Arg_PCS_Extensions*)create_node(1);
}

//PrivateExtensionList
asn1::PrivateExtensionList::PrivateExtensionList(){
	strcpy(node_type_name, "PrivateExtensionList");

}
asn1::PrivateExtensionList::~PrivateExtensionList(){
}

asn1::PrivateExtension* asn1::PrivateExtensionList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (PrivateExtension*)children[child_index]; else return NULL;
}

void asn1::PrivateExtensionList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::PrivateExtensionList::create_node(unsigned int _index){
	children[_index] = new PrivateExtension();
	return children[_index];
}

asn1::ASN1Node* asn1::PrivateExtensionList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new PrivateExtension());
		return children[children.size() - 1];
	}

}

//PrivateExtension
asn1::PrivateExtension::PrivateExtension(){
	strcpy(node_type_name, "PrivateExtension");
	// extId
	_extId = NULL;
	_extId = new Object_identifier();
	children.push_back(_extId);

	// extType
	_extType = NULL;
	children.push_back(_extType);


}
asn1::PrivateExtension::~PrivateExtension(){

}

asn1::ASN1Node* asn1::PrivateExtension::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_extType = new Any();
			children[1] = _extType;
			return _extType;

		default: return NULL;
	}
}

void asn1::PrivateExtension::set_extType(){
	if(_extType == NULL) _extType = (Any*)create_node(1);
}

//maxNumOfPrivateExtensions
asn1::maxNumOfPrivateExtensions::maxNumOfPrivateExtensions(){
	strcpy(node_type_name, "maxNumOfPrivateExtensions");

}
asn1::maxNumOfPrivateExtensions::~maxNumOfPrivateExtensions(){

}
//PCS_Extensions
asn1::PCS_Extensions::PCS_Extensions(){
	strcpy(node_type_name, "PCS_Extensions");


}
asn1::PCS_Extensions::~PCS_Extensions(){

}


//SLR_Arg_PCS_Extensions
asn1::SLR_Arg_PCS_Extensions::SLR_Arg_PCS_Extensions(){
	strcpy(node_type_name, "SLR_Arg_PCS_Extensions");
	// na_ESRK_Request
	_na_ESRK_Request = NULL;
	children.push_back(_na_ESRK_Request);


}
asn1::SLR_Arg_PCS_Extensions::~SLR_Arg_PCS_Extensions(){

}

asn1::ASN1Node* asn1::SLR_Arg_PCS_Extensions::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_na_ESRK_Request = new Null();
			_na_ESRK_Request->tlv->tag_class = CONTEXT_SPECIFIC;
			_na_ESRK_Request->tlv->tag_value = 0;
			children[0] = _na_ESRK_Request;
			return _na_ESRK_Request;

		default: return NULL;
	}
}

void asn1::SLR_Arg_PCS_Extensions::set_na_ESRK_Request(){
	if(_na_ESRK_Request == NULL) _na_ESRK_Request = (Null*)create_node(0);
}

