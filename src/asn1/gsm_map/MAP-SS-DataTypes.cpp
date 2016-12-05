#include<MAP-SS-DataTypes.h>
#include<iostream>

//RegisterSS_Arg
asn1::RegisterSS_Arg::RegisterSS_Arg(){
	strcpy(node_type_name, "RegisterSS_Arg");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	children.push_back(_ss_Code);

	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// forwardedToNumber
	_forwardedToNumber = NULL;
	children.push_back(_forwardedToNumber);

	// forwardedToSubaddress
	_forwardedToSubaddress = NULL;
	children.push_back(_forwardedToSubaddress);

	// noReplyConditionTime
	_noReplyConditionTime = NULL;
	children.push_back(_noReplyConditionTime);

	// defaultPriority
	_defaultPriority = NULL;
	children.push_back(_defaultPriority);

	// nbrUser
	_nbrUser = NULL;
	children.push_back(_nbrUser);

	// longFTN_Supported
	_longFTN_Supported = NULL;
	children.push_back(_longFTN_Supported);


}
asn1::RegisterSS_Arg::~RegisterSS_Arg(){

}

asn1::ASN1Node* asn1::RegisterSS_Arg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_basicService = new BasicServiceCode();
			children[1] = _basicService;
			return _basicService;
		case 2:
			_forwardedToNumber = new AddressString();
			_forwardedToNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardedToNumber->tlv->tag_value = 4;
			children[2] = _forwardedToNumber;
			return _forwardedToNumber;
		case 3:
			_forwardedToSubaddress = new ISDN_SubaddressString();
			_forwardedToSubaddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardedToSubaddress->tlv->tag_value = 6;
			children[3] = _forwardedToSubaddress;
			return _forwardedToSubaddress;
		case 4:
			_noReplyConditionTime = new NoReplyConditionTime();
			_noReplyConditionTime->tlv->tag_class = CONTEXT_SPECIFIC;
			_noReplyConditionTime->tlv->tag_value = 5;
			children[4] = _noReplyConditionTime;
			return _noReplyConditionTime;
		case 5:
			_defaultPriority = new EMLPP_Priority();
			_defaultPriority->tlv->tag_class = CONTEXT_SPECIFIC;
			_defaultPriority->tlv->tag_value = 7;
			children[5] = _defaultPriority;
			return _defaultPriority;
		case 6:
			_nbrUser = new MC_Bearers();
			_nbrUser->tlv->tag_class = CONTEXT_SPECIFIC;
			_nbrUser->tlv->tag_value = 8;
			children[6] = _nbrUser;
			return _nbrUser;
		case 7:
			_longFTN_Supported = new Null();
			_longFTN_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_longFTN_Supported->tlv->tag_value = 9;
			children[7] = _longFTN_Supported;
			return _longFTN_Supported;

		default: return NULL;
	}
}

void asn1::RegisterSS_Arg::set_basicService(){
	if(_basicService == NULL) _basicService = (BasicServiceCode*)create_node(1);
}

void asn1::RegisterSS_Arg::set_forwardedToNumber(){
	if(_forwardedToNumber == NULL) _forwardedToNumber = (AddressString*)create_node(2);
}

void asn1::RegisterSS_Arg::set_forwardedToSubaddress(){
	if(_forwardedToSubaddress == NULL) _forwardedToSubaddress = (ISDN_SubaddressString*)create_node(3);
}

void asn1::RegisterSS_Arg::set_noReplyConditionTime(){
	if(_noReplyConditionTime == NULL) _noReplyConditionTime = (NoReplyConditionTime*)create_node(4);
}

void asn1::RegisterSS_Arg::set_defaultPriority(){
	if(_defaultPriority == NULL) _defaultPriority = (EMLPP_Priority*)create_node(5);
}

void asn1::RegisterSS_Arg::set_nbrUser(){
	if(_nbrUser == NULL) _nbrUser = (MC_Bearers*)create_node(6);
}

void asn1::RegisterSS_Arg::set_longFTN_Supported(){
	if(_longFTN_Supported == NULL) _longFTN_Supported = (Null*)create_node(7);
}

//NoReplyConditionTime
asn1::NoReplyConditionTime::NoReplyConditionTime(){
	strcpy(node_type_name, "NoReplyConditionTime");

}
asn1::NoReplyConditionTime::~NoReplyConditionTime(){

}

//SS_Info
asn1::SS_Info::SS_Info(){
	strcpy(node_type_name, "SS_Info");
	// forwardingInfo
	_forwardingInfo = NULL;
	_forwardingInfo = new ForwardingInfo();
	_forwardingInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_forwardingInfo->tlv->tag_value = 0;
	children.push_back(_forwardingInfo);

	// callBarringInfo
	_callBarringInfo = NULL;
	_callBarringInfo = new CallBarringInfo();
	_callBarringInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_callBarringInfo->tlv->tag_value = 1;
	children.push_back(_callBarringInfo);

	// ss_Data
	_ss_Data = NULL;
	_ss_Data = new SS_Data();
	_ss_Data->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Data->tlv->tag_value = 3;
	children.push_back(_ss_Data);


}
asn1::SS_Info::~SS_Info(){

}

//ForwardingInfo
asn1::ForwardingInfo::ForwardingInfo(){
	strcpy(node_type_name, "ForwardingInfo");
	// ss_Code
	_ss_Code = NULL;
	children.push_back(_ss_Code);

	// forwardingFeatureList
	_forwardingFeatureList = NULL;
	_forwardingFeatureList = new ForwardingFeatureList();
	children.push_back(_forwardingFeatureList);


}
asn1::ForwardingInfo::~ForwardingInfo(){

}

asn1::ASN1Node* asn1::ForwardingInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ss_Code = new SS_Code();
			children[0] = _ss_Code;
			return _ss_Code;

		default: return NULL;
	}
}

void asn1::ForwardingInfo::set_ss_Code(){
	if(_ss_Code == NULL) _ss_Code = (SS_Code*)create_node(0);
}

//ForwardingFeatureList
asn1::ForwardingFeatureList::ForwardingFeatureList(){
	strcpy(node_type_name, "ForwardingFeatureList");

}
asn1::ForwardingFeatureList::~ForwardingFeatureList(){
}

asn1::ForwardingFeature* asn1::ForwardingFeatureList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (ForwardingFeature*)children[child_index]; else return NULL;
}

void asn1::ForwardingFeatureList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::ForwardingFeatureList::create_node(unsigned int _index){
	children[_index] = new ForwardingFeature();
	return children[_index];
}

asn1::ASN1Node* asn1::ForwardingFeatureList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new ForwardingFeature());
		return children[children.size() - 1];
	}

}

//ForwardingFeature
asn1::ForwardingFeature::ForwardingFeature(){
	strcpy(node_type_name, "ForwardingFeature");
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

	// forwardingOptions
	_forwardingOptions = NULL;
	children.push_back(_forwardingOptions);

	// noReplyConditionTime
	_noReplyConditionTime = NULL;
	children.push_back(_noReplyConditionTime);

	// longForwardedToNumber
	_longForwardedToNumber = NULL;
	children.push_back(_longForwardedToNumber);


}
asn1::ForwardingFeature::~ForwardingFeature(){

}

asn1::ASN1Node* asn1::ForwardingFeature::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_basicService = new BasicServiceCode();
			children[0] = _basicService;
			return _basicService;
		case 1:
			_ss_Status = new SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 4;
			children[1] = _ss_Status;
			return _ss_Status;
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
			_forwardingOptions = new ForwardingOptions();
			_forwardingOptions->tlv->tag_class = CONTEXT_SPECIFIC;
			_forwardingOptions->tlv->tag_value = 6;
			children[4] = _forwardingOptions;
			return _forwardingOptions;
		case 5:
			_noReplyConditionTime = new NoReplyConditionTime();
			_noReplyConditionTime->tlv->tag_class = CONTEXT_SPECIFIC;
			_noReplyConditionTime->tlv->tag_value = 7;
			children[5] = _noReplyConditionTime;
			return _noReplyConditionTime;
		case 6:
			_longForwardedToNumber = new FTN_AddressString();
			_longForwardedToNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_longForwardedToNumber->tlv->tag_value = 9;
			children[6] = _longForwardedToNumber;
			return _longForwardedToNumber;

		default: return NULL;
	}
}

void asn1::ForwardingFeature::set_basicService(){
	if(_basicService == NULL) _basicService = (BasicServiceCode*)create_node(0);
}

void asn1::ForwardingFeature::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (SS_Status*)create_node(1);
}

void asn1::ForwardingFeature::set_forwardedToNumber(){
	if(_forwardedToNumber == NULL) _forwardedToNumber = (ISDN_AddressString*)create_node(2);
}

void asn1::ForwardingFeature::set_forwardedToSubaddress(){
	if(_forwardedToSubaddress == NULL) _forwardedToSubaddress = (ISDN_SubaddressString*)create_node(3);
}

void asn1::ForwardingFeature::set_forwardingOptions(){
	if(_forwardingOptions == NULL) _forwardingOptions = (ForwardingOptions*)create_node(4);
}

void asn1::ForwardingFeature::set_noReplyConditionTime(){
	if(_noReplyConditionTime == NULL) _noReplyConditionTime = (NoReplyConditionTime*)create_node(5);
}

void asn1::ForwardingFeature::set_longForwardedToNumber(){
	if(_longForwardedToNumber == NULL) _longForwardedToNumber = (FTN_AddressString*)create_node(6);
}

//SS_Status
asn1::SS_Status::SS_Status(){
	strcpy(node_type_name, "SS_Status");

}
asn1::SS_Status::~SS_Status(){

}

//ForwardingOptions
asn1::ForwardingOptions::ForwardingOptions(){
	strcpy(node_type_name, "ForwardingOptions");

}
asn1::ForwardingOptions::~ForwardingOptions(){

}

//CallBarringInfo
asn1::CallBarringInfo::CallBarringInfo(){
	strcpy(node_type_name, "CallBarringInfo");
	// ss_Code
	_ss_Code = NULL;
	children.push_back(_ss_Code);

	// callBarringFeatureList
	_callBarringFeatureList = NULL;
	_callBarringFeatureList = new CallBarringFeatureList();
	children.push_back(_callBarringFeatureList);


}
asn1::CallBarringInfo::~CallBarringInfo(){

}

asn1::ASN1Node* asn1::CallBarringInfo::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ss_Code = new SS_Code();
			children[0] = _ss_Code;
			return _ss_Code;

		default: return NULL;
	}
}

void asn1::CallBarringInfo::set_ss_Code(){
	if(_ss_Code == NULL) _ss_Code = (SS_Code*)create_node(0);
}

//CallBarringFeatureList
asn1::CallBarringFeatureList::CallBarringFeatureList(){
	strcpy(node_type_name, "CallBarringFeatureList");

}
asn1::CallBarringFeatureList::~CallBarringFeatureList(){
}

asn1::CallBarringFeature* asn1::CallBarringFeatureList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (CallBarringFeature*)children[child_index]; else return NULL;
}

void asn1::CallBarringFeatureList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::CallBarringFeatureList::create_node(unsigned int _index){
	children[_index] = new CallBarringFeature();
	return children[_index];
}

asn1::ASN1Node* asn1::CallBarringFeatureList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new CallBarringFeature());
		return children[children.size() - 1];
	}

}

//CallBarringFeature
asn1::CallBarringFeature::CallBarringFeature(){
	strcpy(node_type_name, "CallBarringFeature");
	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);


}
asn1::CallBarringFeature::~CallBarringFeature(){

}

asn1::ASN1Node* asn1::CallBarringFeature::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_basicService = new BasicServiceCode();
			children[0] = _basicService;
			return _basicService;
		case 1:
			_ss_Status = new SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 4;
			children[1] = _ss_Status;
			return _ss_Status;

		default: return NULL;
	}
}

void asn1::CallBarringFeature::set_basicService(){
	if(_basicService == NULL) _basicService = (BasicServiceCode*)create_node(0);
}

void asn1::CallBarringFeature::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (SS_Status*)create_node(1);
}

//SS_Data
asn1::SS_Data::SS_Data(){
	strcpy(node_type_name, "SS_Data");
	// ss_Code
	_ss_Code = NULL;
	children.push_back(_ss_Code);

	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);

	// ss_SubscriptionOption
	_ss_SubscriptionOption = NULL;
	children.push_back(_ss_SubscriptionOption);

	// basicServiceGroupList
	_basicServiceGroupList = NULL;
	children.push_back(_basicServiceGroupList);

	// defaultPriority
	_defaultPriority = NULL;
	children.push_back(_defaultPriority);

	// nbrUser
	_nbrUser = NULL;
	children.push_back(_nbrUser);


}
asn1::SS_Data::~SS_Data(){

}

asn1::ASN1Node* asn1::SS_Data::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ss_Code = new SS_Code();
			children[0] = _ss_Code;
			return _ss_Code;
		case 1:
			_ss_Status = new SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 4;
			children[1] = _ss_Status;
			return _ss_Status;
		case 2:
			_ss_SubscriptionOption = new SS_SubscriptionOption();
			children[2] = _ss_SubscriptionOption;
			return _ss_SubscriptionOption;
		case 3:
			_basicServiceGroupList = new BasicServiceGroupList();
			children[3] = _basicServiceGroupList;
			return _basicServiceGroupList;
		case 4:
			_defaultPriority = new EMLPP_Priority();
			children[4] = _defaultPriority;
			return _defaultPriority;
		case 5:
			_nbrUser = new MC_Bearers();
			_nbrUser->tlv->tag_class = CONTEXT_SPECIFIC;
			_nbrUser->tlv->tag_value = 5;
			children[5] = _nbrUser;
			return _nbrUser;

		default: return NULL;
	}
}

void asn1::SS_Data::set_ss_Code(){
	if(_ss_Code == NULL) _ss_Code = (SS_Code*)create_node(0);
}

void asn1::SS_Data::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (SS_Status*)create_node(1);
}

void asn1::SS_Data::set_ss_SubscriptionOption(){
	if(_ss_SubscriptionOption == NULL) _ss_SubscriptionOption = (SS_SubscriptionOption*)create_node(2);
}

void asn1::SS_Data::set_basicServiceGroupList(){
	if(_basicServiceGroupList == NULL) _basicServiceGroupList = (BasicServiceGroupList*)create_node(3);
}

void asn1::SS_Data::set_defaultPriority(){
	if(_defaultPriority == NULL) _defaultPriority = (EMLPP_Priority*)create_node(4);
}

void asn1::SS_Data::set_nbrUser(){
	if(_nbrUser == NULL) _nbrUser = (MC_Bearers*)create_node(5);
}

//SS_SubscriptionOption
asn1::SS_SubscriptionOption::SS_SubscriptionOption(){
	strcpy(node_type_name, "SS_SubscriptionOption");
	// cliRestrictionOption
	_cliRestrictionOption = NULL;
	_cliRestrictionOption = new CliRestrictionOption();
	_cliRestrictionOption->tlv->tag_class = CONTEXT_SPECIFIC;
	_cliRestrictionOption->tlv->tag_value = 2;
	children.push_back(_cliRestrictionOption);

	// overrideCategory
	_overrideCategory = NULL;
	_overrideCategory = new OverrideCategory();
	_overrideCategory->tlv->tag_class = CONTEXT_SPECIFIC;
	_overrideCategory->tlv->tag_value = 1;
	children.push_back(_overrideCategory);


}
asn1::SS_SubscriptionOption::~SS_SubscriptionOption(){

}

//CliRestrictionOption
asn1::CliRestrictionOption::CliRestrictionOption(){
	strcpy(node_type_name, "CliRestrictionOption");

}
asn1::CliRestrictionOption::~CliRestrictionOption(){

}

//OverrideCategory
asn1::OverrideCategory::OverrideCategory(){
	strcpy(node_type_name, "OverrideCategory");

}
asn1::OverrideCategory::~OverrideCategory(){

}

//SS_ForBS_Code
asn1::SS_ForBS_Code::SS_ForBS_Code(){
	strcpy(node_type_name, "SS_ForBS_Code");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	children.push_back(_ss_Code);

	// basicService
	_basicService = NULL;
	children.push_back(_basicService);

	// longFTN_Supported
	_longFTN_Supported = NULL;
	children.push_back(_longFTN_Supported);


}
asn1::SS_ForBS_Code::~SS_ForBS_Code(){

}

asn1::ASN1Node* asn1::SS_ForBS_Code::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_basicService = new BasicServiceCode();
			children[1] = _basicService;
			return _basicService;
		case 2:
			_longFTN_Supported = new Null();
			_longFTN_Supported->tlv->tag_class = CONTEXT_SPECIFIC;
			_longFTN_Supported->tlv->tag_value = 4;
			children[2] = _longFTN_Supported;
			return _longFTN_Supported;

		default: return NULL;
	}
}

void asn1::SS_ForBS_Code::set_basicService(){
	if(_basicService == NULL) _basicService = (BasicServiceCode*)create_node(1);
}

void asn1::SS_ForBS_Code::set_longFTN_Supported(){
	if(_longFTN_Supported == NULL) _longFTN_Supported = (Null*)create_node(2);
}

//GenericServiceInfo
asn1::GenericServiceInfo::GenericServiceInfo(){
	strcpy(node_type_name, "GenericServiceInfo");
	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new SS_Status();
	children.push_back(_ss_Status);

	// cliRestrictionOption
	_cliRestrictionOption = NULL;
	children.push_back(_cliRestrictionOption);

	// maximumEntitledPriority
	_maximumEntitledPriority = NULL;
	children.push_back(_maximumEntitledPriority);

	// defaultPriority
	_defaultPriority = NULL;
	children.push_back(_defaultPriority);

	// ccbs_FeatureList
	_ccbs_FeatureList = NULL;
	children.push_back(_ccbs_FeatureList);

	// nbrSB
	_nbrSB = NULL;
	children.push_back(_nbrSB);

	// nbrUser
	_nbrUser = NULL;
	children.push_back(_nbrUser);

	// nbrSN
	_nbrSN = NULL;
	children.push_back(_nbrSN);


}
asn1::GenericServiceInfo::~GenericServiceInfo(){

}

asn1::ASN1Node* asn1::GenericServiceInfo::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_cliRestrictionOption = new CliRestrictionOption();
			children[1] = _cliRestrictionOption;
			return _cliRestrictionOption;
		case 2:
			_maximumEntitledPriority = new EMLPP_Priority();
			_maximumEntitledPriority->tlv->tag_class = CONTEXT_SPECIFIC;
			_maximumEntitledPriority->tlv->tag_value = 0;
			children[2] = _maximumEntitledPriority;
			return _maximumEntitledPriority;
		case 3:
			_defaultPriority = new EMLPP_Priority();
			_defaultPriority->tlv->tag_class = CONTEXT_SPECIFIC;
			_defaultPriority->tlv->tag_value = 1;
			children[3] = _defaultPriority;
			return _defaultPriority;
		case 4:
			_ccbs_FeatureList = new CCBS_FeatureList();
			_ccbs_FeatureList->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_FeatureList->tlv->tag_value = 2;
			children[4] = _ccbs_FeatureList;
			return _ccbs_FeatureList;
		case 5:
			_nbrSB = new MaxMC_Bearers();
			_nbrSB->tlv->tag_class = CONTEXT_SPECIFIC;
			_nbrSB->tlv->tag_value = 3;
			children[5] = _nbrSB;
			return _nbrSB;
		case 6:
			_nbrUser = new MC_Bearers();
			_nbrUser->tlv->tag_class = CONTEXT_SPECIFIC;
			_nbrUser->tlv->tag_value = 4;
			children[6] = _nbrUser;
			return _nbrUser;
		case 7:
			_nbrSN = new MC_Bearers();
			_nbrSN->tlv->tag_class = CONTEXT_SPECIFIC;
			_nbrSN->tlv->tag_value = 5;
			children[7] = _nbrSN;
			return _nbrSN;

		default: return NULL;
	}
}

void asn1::GenericServiceInfo::set_cliRestrictionOption(){
	if(_cliRestrictionOption == NULL) _cliRestrictionOption = (CliRestrictionOption*)create_node(1);
}

void asn1::GenericServiceInfo::set_maximumEntitledPriority(){
	if(_maximumEntitledPriority == NULL) _maximumEntitledPriority = (EMLPP_Priority*)create_node(2);
}

void asn1::GenericServiceInfo::set_defaultPriority(){
	if(_defaultPriority == NULL) _defaultPriority = (EMLPP_Priority*)create_node(3);
}

void asn1::GenericServiceInfo::set_ccbs_FeatureList(){
	if(_ccbs_FeatureList == NULL) _ccbs_FeatureList = (CCBS_FeatureList*)create_node(4);
}

void asn1::GenericServiceInfo::set_nbrSB(){
	if(_nbrSB == NULL) _nbrSB = (MaxMC_Bearers*)create_node(5);
}

void asn1::GenericServiceInfo::set_nbrUser(){
	if(_nbrUser == NULL) _nbrUser = (MC_Bearers*)create_node(6);
}

void asn1::GenericServiceInfo::set_nbrSN(){
	if(_nbrSN == NULL) _nbrSN = (MC_Bearers*)create_node(7);
}

//CCBS_FeatureList
asn1::CCBS_FeatureList::CCBS_FeatureList(){
	strcpy(node_type_name, "CCBS_FeatureList");

}
asn1::CCBS_FeatureList::~CCBS_FeatureList(){
}

asn1::CCBS_Feature* asn1::CCBS_FeatureList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (CCBS_Feature*)children[child_index]; else return NULL;
}

void asn1::CCBS_FeatureList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::CCBS_FeatureList::create_node(unsigned int _index){
	children[_index] = new CCBS_Feature();
	return children[_index];
}

asn1::ASN1Node* asn1::CCBS_FeatureList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new CCBS_Feature());
		return children[children.size() - 1];
	}

}

//maxNumOfCCBS_Requests
asn1::maxNumOfCCBS_Requests::maxNumOfCCBS_Requests(){
	strcpy(node_type_name, "maxNumOfCCBS_Requests");

}
asn1::maxNumOfCCBS_Requests::~maxNumOfCCBS_Requests(){

}

//CCBS_Feature
asn1::CCBS_Feature::CCBS_Feature(){
	strcpy(node_type_name, "CCBS_Feature");
	// ccbs_Index
	_ccbs_Index = NULL;
	children.push_back(_ccbs_Index);

	// b_subscriberNumber
	_b_subscriberNumber = NULL;
	children.push_back(_b_subscriberNumber);

	// b_subscriberSubaddress
	_b_subscriberSubaddress = NULL;
	children.push_back(_b_subscriberSubaddress);

	// basicServiceGroup
	_basicServiceGroup = NULL;
	children.push_back(_basicServiceGroup);


}
asn1::CCBS_Feature::~CCBS_Feature(){

}

asn1::ASN1Node* asn1::CCBS_Feature::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ccbs_Index = new CCBS_Index();
			_ccbs_Index->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Index->tlv->tag_value = 0;
			children[0] = _ccbs_Index;
			return _ccbs_Index;
		case 1:
			_b_subscriberNumber = new ISDN_AddressString();
			_b_subscriberNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_b_subscriberNumber->tlv->tag_value = 1;
			children[1] = _b_subscriberNumber;
			return _b_subscriberNumber;
		case 2:
			_b_subscriberSubaddress = new ISDN_SubaddressString();
			_b_subscriberSubaddress->tlv->tag_class = CONTEXT_SPECIFIC;
			_b_subscriberSubaddress->tlv->tag_value = 2;
			children[2] = _b_subscriberSubaddress;
			return _b_subscriberSubaddress;
		case 3:
			_basicServiceGroup = new BasicServiceCode();
			_basicServiceGroup->tlv->tag_class = CONTEXT_SPECIFIC;
			_basicServiceGroup->tlv->tag_value = 3;
			children[3] = _basicServiceGroup;
			return _basicServiceGroup;

		default: return NULL;
	}
}

void asn1::CCBS_Feature::set_ccbs_Index(){
	if(_ccbs_Index == NULL) _ccbs_Index = (CCBS_Index*)create_node(0);
}

void asn1::CCBS_Feature::set_b_subscriberNumber(){
	if(_b_subscriberNumber == NULL) _b_subscriberNumber = (ISDN_AddressString*)create_node(1);
}

void asn1::CCBS_Feature::set_b_subscriberSubaddress(){
	if(_b_subscriberSubaddress == NULL) _b_subscriberSubaddress = (ISDN_SubaddressString*)create_node(2);
}

void asn1::CCBS_Feature::set_basicServiceGroup(){
	if(_basicServiceGroup == NULL) _basicServiceGroup = (BasicServiceCode*)create_node(3);
}

//CCBS_Index
asn1::CCBS_Index::CCBS_Index(){
	strcpy(node_type_name, "CCBS_Index");

}
asn1::CCBS_Index::~CCBS_Index(){

}

//InterrogateSS_Res
asn1::InterrogateSS_Res::InterrogateSS_Res(){
	strcpy(node_type_name, "InterrogateSS_Res");
	// ss_Status
	_ss_Status = NULL;
	_ss_Status = new SS_Status();
	_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Status->tlv->tag_value = 0;
	children.push_back(_ss_Status);

	// basicServiceGroupList
	_basicServiceGroupList = NULL;
	_basicServiceGroupList = new BasicServiceGroupList();
	_basicServiceGroupList->tlv->tag_class = CONTEXT_SPECIFIC;
	_basicServiceGroupList->tlv->tag_value = 2;
	children.push_back(_basicServiceGroupList);

	// forwardingFeatureList
	_forwardingFeatureList = NULL;
	_forwardingFeatureList = new ForwardingFeatureList();
	_forwardingFeatureList->tlv->tag_class = CONTEXT_SPECIFIC;
	_forwardingFeatureList->tlv->tag_value = 3;
	children.push_back(_forwardingFeatureList);

	// genericServiceInfo
	_genericServiceInfo = NULL;
	_genericServiceInfo = new GenericServiceInfo();
	_genericServiceInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_genericServiceInfo->tlv->tag_value = 4;
	children.push_back(_genericServiceInfo);


}
asn1::InterrogateSS_Res::~InterrogateSS_Res(){

}

//USSD_Arg
asn1::USSD_Arg::USSD_Arg(){
	strcpy(node_type_name, "USSD_Arg");
	// ussd_DataCodingScheme
	_ussd_DataCodingScheme = NULL;
	_ussd_DataCodingScheme = new USSD_DataCodingScheme();
	children.push_back(_ussd_DataCodingScheme);

	// ussd_String
	_ussd_String = NULL;
	_ussd_String = new USSD_String();
	children.push_back(_ussd_String);

	// alertingPattern
	_alertingPattern = NULL;
	children.push_back(_alertingPattern);

	// msisdn
	_msisdn = NULL;
	children.push_back(_msisdn);


}
asn1::USSD_Arg::~USSD_Arg(){

}

asn1::ASN1Node* asn1::USSD_Arg::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_alertingPattern = new AlertingPattern();
			children[2] = _alertingPattern;
			return _alertingPattern;
		case 3:
			_msisdn = new ISDN_AddressString();
			_msisdn->tlv->tag_class = CONTEXT_SPECIFIC;
			_msisdn->tlv->tag_value = 0;
			children[3] = _msisdn;
			return _msisdn;

		default: return NULL;
	}
}

void asn1::USSD_Arg::set_alertingPattern(){
	if(_alertingPattern == NULL) _alertingPattern = (AlertingPattern*)create_node(2);
}

void asn1::USSD_Arg::set_msisdn(){
	if(_msisdn == NULL) _msisdn = (ISDN_AddressString*)create_node(3);
}

//USSD_Res
asn1::USSD_Res::USSD_Res(){
	strcpy(node_type_name, "USSD_Res");
	// ussd_DataCodingScheme
	_ussd_DataCodingScheme = NULL;
	_ussd_DataCodingScheme = new USSD_DataCodingScheme();
	children.push_back(_ussd_DataCodingScheme);

	// ussd_String
	_ussd_String = NULL;
	_ussd_String = new USSD_String();
	children.push_back(_ussd_String);


}
asn1::USSD_Res::~USSD_Res(){

}

//USSD_DataCodingScheme
asn1::USSD_DataCodingScheme::USSD_DataCodingScheme(){
	strcpy(node_type_name, "USSD_DataCodingScheme");

}
asn1::USSD_DataCodingScheme::~USSD_DataCodingScheme(){

}

//USSD_String
asn1::USSD_String::USSD_String(){
	strcpy(node_type_name, "USSD_String");

}
asn1::USSD_String::~USSD_String(){

}

//maxUSSD_StringLength
asn1::maxUSSD_StringLength::maxUSSD_StringLength(){
	strcpy(node_type_name, "maxUSSD_StringLength");

}
asn1::maxUSSD_StringLength::~maxUSSD_StringLength(){

}

//Password
asn1::Password::Password(){
	strcpy(node_type_name, "Password");

}
asn1::Password::~Password(){

}

//GuidanceInfo
asn1::GuidanceInfo::GuidanceInfo(){
	strcpy(node_type_name, "GuidanceInfo");

}
asn1::GuidanceInfo::~GuidanceInfo(){

}

//SS_List
asn1::SS_List::SS_List(){
	strcpy(node_type_name, "SS_List");

}
asn1::SS_List::~SS_List(){
}

asn1::SS_Code* asn1::SS_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (SS_Code*)children[child_index]; else return NULL;
}

void asn1::SS_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::SS_List::create_node(unsigned int _index){
	children[_index] = new SS_Code();
	return children[_index];
}

asn1::ASN1Node* asn1::SS_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new SS_Code());
		return children[children.size() - 1];
	}

}

//maxNumOfSS
asn1::maxNumOfSS::maxNumOfSS(){
	strcpy(node_type_name, "maxNumOfSS");

}
asn1::maxNumOfSS::~maxNumOfSS(){

}

//SS_InfoList
asn1::SS_InfoList::SS_InfoList(){
	strcpy(node_type_name, "SS_InfoList");

}
asn1::SS_InfoList::~SS_InfoList(){
}

asn1::SS_Info* asn1::SS_InfoList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (SS_Info*)children[child_index]; else return NULL;
}

void asn1::SS_InfoList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::SS_InfoList::create_node(unsigned int _index){
	children[_index] = new SS_Info();
	return children[_index];
}

asn1::ASN1Node* asn1::SS_InfoList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new SS_Info());
		return children[children.size() - 1];
	}

}

//BasicServiceGroupList
asn1::BasicServiceGroupList::BasicServiceGroupList(){
	strcpy(node_type_name, "BasicServiceGroupList");

}
asn1::BasicServiceGroupList::~BasicServiceGroupList(){
}

asn1::BasicServiceCode* asn1::BasicServiceGroupList::get_child(unsigned int child_index){
	if(child_index < children.size()) return (BasicServiceCode*)children[child_index]; else return NULL;
}

void asn1::BasicServiceGroupList::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::BasicServiceGroupList::create_node(unsigned int _index){
	children[_index] = new BasicServiceCode();
	return children[_index];
}

asn1::ASN1Node* asn1::BasicServiceGroupList::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new BasicServiceCode());
		return children[children.size() - 1];
	}

}

//maxNumOfBasicServiceGroups
asn1::maxNumOfBasicServiceGroups::maxNumOfBasicServiceGroups(){
	strcpy(node_type_name, "maxNumOfBasicServiceGroups");

}
asn1::maxNumOfBasicServiceGroups::~maxNumOfBasicServiceGroups(){

}

//SS_InvocationNotificationArg
asn1::SS_InvocationNotificationArg::SS_InvocationNotificationArg(){
	strcpy(node_type_name, "SS_InvocationNotificationArg");
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

	// ss_Event
	_ss_Event = NULL;
	_ss_Event = new SS_Code();
	_ss_Event->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Event->tlv->tag_value = 2;
	children.push_back(_ss_Event);

	// ss_EventSpecification
	_ss_EventSpecification = NULL;
	children.push_back(_ss_EventSpecification);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// b_subscriberNumber
	_b_subscriberNumber = NULL;
	children.push_back(_b_subscriberNumber);

	// ccbs_RequestState
	_ccbs_RequestState = NULL;
	children.push_back(_ccbs_RequestState);


}
asn1::SS_InvocationNotificationArg::~SS_InvocationNotificationArg(){

}

asn1::ASN1Node* asn1::SS_InvocationNotificationArg::create_node(unsigned int _index){
	switch(_index){
		case 3:
			_ss_EventSpecification = new SS_EventSpecification();
			_ss_EventSpecification->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_EventSpecification->tlv->tag_value = 3;
			children[3] = _ss_EventSpecification;
			return _ss_EventSpecification;
		case 4:
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 4;
			children[4] = _extensionContainer;
			return _extensionContainer;
		case 5:
			_b_subscriberNumber = new ISDN_AddressString();
			_b_subscriberNumber->tlv->tag_class = CONTEXT_SPECIFIC;
			_b_subscriberNumber->tlv->tag_value = 5;
			children[5] = _b_subscriberNumber;
			return _b_subscriberNumber;
		case 6:
			_ccbs_RequestState = new CCBS_RequestState();
			_ccbs_RequestState->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_RequestState->tlv->tag_value = 6;
			children[6] = _ccbs_RequestState;
			return _ccbs_RequestState;

		default: return NULL;
	}
}

void asn1::SS_InvocationNotificationArg::set_ss_EventSpecification(){
	if(_ss_EventSpecification == NULL) _ss_EventSpecification = (SS_EventSpecification*)create_node(3);
}

void asn1::SS_InvocationNotificationArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

void asn1::SS_InvocationNotificationArg::set_b_subscriberNumber(){
	if(_b_subscriberNumber == NULL) _b_subscriberNumber = (ISDN_AddressString*)create_node(5);
}

void asn1::SS_InvocationNotificationArg::set_ccbs_RequestState(){
	if(_ccbs_RequestState == NULL) _ccbs_RequestState = (CCBS_RequestState*)create_node(6);
}

//CCBS_RequestState
asn1::CCBS_RequestState::CCBS_RequestState(){
	strcpy(node_type_name, "CCBS_RequestState");

}
asn1::CCBS_RequestState::~CCBS_RequestState(){

}

//SS_InvocationNotificationRes
asn1::SS_InvocationNotificationRes::SS_InvocationNotificationRes(){
	strcpy(node_type_name, "SS_InvocationNotificationRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::SS_InvocationNotificationRes::~SS_InvocationNotificationRes(){

}

asn1::ASN1Node* asn1::SS_InvocationNotificationRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_extensionContainer = new ExtensionContainer();
			children[0] = _extensionContainer;
			return _extensionContainer;

		default: return NULL;
	}
}

void asn1::SS_InvocationNotificationRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

//SS_EventSpecification
asn1::SS_EventSpecification::SS_EventSpecification(){
	strcpy(node_type_name, "SS_EventSpecification");

}
asn1::SS_EventSpecification::~SS_EventSpecification(){
}

asn1::AddressString* asn1::SS_EventSpecification::get_child(unsigned int child_index){
	if(child_index < children.size()) return (AddressString*)children[child_index]; else return NULL;
}

void asn1::SS_EventSpecification::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::SS_EventSpecification::create_node(unsigned int _index){
	children[_index] = new AddressString();
	return children[_index];
}

asn1::ASN1Node* asn1::SS_EventSpecification::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new AddressString());
		return children[children.size() - 1];
	}

}

//maxEventSpecification
asn1::maxEventSpecification::maxEventSpecification(){
	strcpy(node_type_name, "maxEventSpecification");

}
asn1::maxEventSpecification::~maxEventSpecification(){

}

//RegisterCC_EntryArg
asn1::RegisterCC_EntryArg::RegisterCC_EntryArg(){
	strcpy(node_type_name, "RegisterCC_EntryArg");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	_ss_Code->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Code->tlv->tag_value = 0;
	children.push_back(_ss_Code);

	// ccbs_Data
	_ccbs_Data = NULL;
	children.push_back(_ccbs_Data);


}
asn1::RegisterCC_EntryArg::~RegisterCC_EntryArg(){

}

asn1::ASN1Node* asn1::RegisterCC_EntryArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_ccbs_Data = new CCBS_Data();
			_ccbs_Data->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Data->tlv->tag_value = 1;
			children[1] = _ccbs_Data;
			return _ccbs_Data;

		default: return NULL;
	}
}

void asn1::RegisterCC_EntryArg::set_ccbs_Data(){
	if(_ccbs_Data == NULL) _ccbs_Data = (CCBS_Data*)create_node(1);
}

//CCBS_Data
asn1::CCBS_Data::CCBS_Data(){
	strcpy(node_type_name, "CCBS_Data");
	// ccbs_Feature
	_ccbs_Feature = NULL;
	_ccbs_Feature = new CCBS_Feature();
	_ccbs_Feature->tlv->tag_class = CONTEXT_SPECIFIC;
	_ccbs_Feature->tlv->tag_value = 0;
	children.push_back(_ccbs_Feature);

	// translatedB_Number
	_translatedB_Number = NULL;
	_translatedB_Number = new ISDN_AddressString();
	_translatedB_Number->tlv->tag_class = CONTEXT_SPECIFIC;
	_translatedB_Number->tlv->tag_value = 1;
	children.push_back(_translatedB_Number);

	// serviceIndicator
	_serviceIndicator = NULL;
	children.push_back(_serviceIndicator);

	// callInfo
	_callInfo = NULL;
	_callInfo = new ExternalSignalInfo();
	_callInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_callInfo->tlv->tag_value = 3;
	children.push_back(_callInfo);

	// networkSignalInfo
	_networkSignalInfo = NULL;
	_networkSignalInfo = new ExternalSignalInfo();
	_networkSignalInfo->tlv->tag_class = CONTEXT_SPECIFIC;
	_networkSignalInfo->tlv->tag_value = 4;
	children.push_back(_networkSignalInfo);


}
asn1::CCBS_Data::~CCBS_Data(){

}

asn1::ASN1Node* asn1::CCBS_Data::create_node(unsigned int _index){
	switch(_index){
		case 2:
			_serviceIndicator = new ServiceIndicator();
			_serviceIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_serviceIndicator->tlv->tag_value = 2;
			children[2] = _serviceIndicator;
			return _serviceIndicator;

		default: return NULL;
	}
}

void asn1::CCBS_Data::set_serviceIndicator(){
	if(_serviceIndicator == NULL) _serviceIndicator = (ServiceIndicator*)create_node(2);
}

//ServiceIndicator
asn1::ServiceIndicator::ServiceIndicator(){
	strcpy(node_type_name, "ServiceIndicator");

}
asn1::ServiceIndicator::~ServiceIndicator(){

}

//RegisterCC_EntryRes
asn1::RegisterCC_EntryRes::RegisterCC_EntryRes(){
	strcpy(node_type_name, "RegisterCC_EntryRes");
	// ccbs_Feature
	_ccbs_Feature = NULL;
	children.push_back(_ccbs_Feature);


}
asn1::RegisterCC_EntryRes::~RegisterCC_EntryRes(){

}

asn1::ASN1Node* asn1::RegisterCC_EntryRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
			_ccbs_Feature = new CCBS_Feature();
			_ccbs_Feature->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Feature->tlv->tag_value = 0;
			children[0] = _ccbs_Feature;
			return _ccbs_Feature;

		default: return NULL;
	}
}

void asn1::RegisterCC_EntryRes::set_ccbs_Feature(){
	if(_ccbs_Feature == NULL) _ccbs_Feature = (CCBS_Feature*)create_node(0);
}

//EraseCC_EntryArg
asn1::EraseCC_EntryArg::EraseCC_EntryArg(){
	strcpy(node_type_name, "EraseCC_EntryArg");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	_ss_Code->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Code->tlv->tag_value = 0;
	children.push_back(_ss_Code);

	// ccbs_Index
	_ccbs_Index = NULL;
	children.push_back(_ccbs_Index);


}
asn1::EraseCC_EntryArg::~EraseCC_EntryArg(){

}

asn1::ASN1Node* asn1::EraseCC_EntryArg::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_ccbs_Index = new CCBS_Index();
			_ccbs_Index->tlv->tag_class = CONTEXT_SPECIFIC;
			_ccbs_Index->tlv->tag_value = 1;
			children[1] = _ccbs_Index;
			return _ccbs_Index;

		default: return NULL;
	}
}

void asn1::EraseCC_EntryArg::set_ccbs_Index(){
	if(_ccbs_Index == NULL) _ccbs_Index = (CCBS_Index*)create_node(1);
}

//EraseCC_EntryRes
asn1::EraseCC_EntryRes::EraseCC_EntryRes(){
	strcpy(node_type_name, "EraseCC_EntryRes");
	// ss_Code
	_ss_Code = NULL;
	_ss_Code = new SS_Code();
	_ss_Code->tlv->tag_class = CONTEXT_SPECIFIC;
	_ss_Code->tlv->tag_value = 0;
	children.push_back(_ss_Code);

	// ss_Status
	_ss_Status = NULL;
	children.push_back(_ss_Status);


}
asn1::EraseCC_EntryRes::~EraseCC_EntryRes(){

}

asn1::ASN1Node* asn1::EraseCC_EntryRes::create_node(unsigned int _index){
	switch(_index){
		case 1:
			_ss_Status = new SS_Status();
			_ss_Status->tlv->tag_class = CONTEXT_SPECIFIC;
			_ss_Status->tlv->tag_value = 1;
			children[1] = _ss_Status;
			return _ss_Status;

		default: return NULL;
	}
}

void asn1::EraseCC_EntryRes::set_ss_Status(){
	if(_ss_Status == NULL) _ss_Status = (SS_Status*)create_node(1);
}

