#include<MAP-OM-DataTypes.h>
#include<iostream>

//ActivateTraceModeArg
asn1::ActivateTraceModeArg::ActivateTraceModeArg(){
	strcpy(node_type_name, "ActivateTraceModeArg");
	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// traceReference
	_traceReference = NULL;
	_traceReference = new TraceReference();
	_traceReference->tlv->tag_class = CONTEXT_SPECIFIC;
	_traceReference->tlv->tag_value = 1;
	children.push_back(_traceReference);

	// traceType
	_traceType = NULL;
	_traceType = new TraceType();
	_traceType->tlv->tag_class = CONTEXT_SPECIFIC;
	_traceType->tlv->tag_value = 2;
	children.push_back(_traceType);

	// omc_Id
	_omc_Id = NULL;
	children.push_back(_omc_Id);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// traceReference2
	_traceReference2 = NULL;
	children.push_back(_traceReference2);

	// traceDepthList
	_traceDepthList = NULL;
	children.push_back(_traceDepthList);

	// traceNE_TypeList
	_traceNE_TypeList = NULL;
	children.push_back(_traceNE_TypeList);

	// traceInterfaceList
	_traceInterfaceList = NULL;
	children.push_back(_traceInterfaceList);

	// traceEventList
	_traceEventList = NULL;
	children.push_back(_traceEventList);

	// traceCollectionEntity
	_traceCollectionEntity = NULL;
	children.push_back(_traceCollectionEntity);

	// mdt_Configuration
	_mdt_Configuration = NULL;
	children.push_back(_mdt_Configuration);


}
asn1::ActivateTraceModeArg::~ActivateTraceModeArg(){

}

asn1::ASN1Node* asn1::ActivateTraceModeArg::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 0;
			children[0] = _imsi;
			return _imsi;
		}
		case 3:
		{
			_omc_Id = new AddressString();
			_omc_Id->tlv->tag_class = CONTEXT_SPECIFIC;
			_omc_Id->tlv->tag_value = 3;
			children[3] = _omc_Id;
			return _omc_Id;
		}
		case 4:
		{
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 4;
			children[4] = _extensionContainer;
			return _extensionContainer;
		}
		case 5:
		{
			_traceReference2 = new TraceReference2();
			_traceReference2->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceReference2->tlv->tag_value = 5;
			children[5] = _traceReference2;
			return _traceReference2;
		}
		case 6:
		{
			_traceDepthList = new TraceDepthList();
			_traceDepthList->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceDepthList->tlv->tag_value = 6;
			children[6] = _traceDepthList;
			return _traceDepthList;
		}
		case 7:
		{
			_traceNE_TypeList = new TraceNE_TypeList();
			_traceNE_TypeList->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceNE_TypeList->tlv->tag_value = 7;
			children[7] = _traceNE_TypeList;
			return _traceNE_TypeList;
		}
		case 8:
		{
			_traceInterfaceList = new TraceInterfaceList();
			_traceInterfaceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceInterfaceList->tlv->tag_value = 8;
			children[8] = _traceInterfaceList;
			return _traceInterfaceList;
		}
		case 9:
		{
			_traceEventList = new TraceEventList();
			_traceEventList->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceEventList->tlv->tag_value = 9;
			children[9] = _traceEventList;
			return _traceEventList;
		}
		case 10:
		{
			_traceCollectionEntity = new GSN_Address();
			_traceCollectionEntity->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceCollectionEntity->tlv->tag_value = 10;
			children[10] = _traceCollectionEntity;
			return _traceCollectionEntity;
		}
		case 11:
		{
			_mdt_Configuration = new MDT_Configuration();
			_mdt_Configuration->tlv->tag_class = CONTEXT_SPECIFIC;
			_mdt_Configuration->tlv->tag_value = 11;
			children[11] = _mdt_Configuration;
			return _mdt_Configuration;
		}

		default: return NULL;
	}
}

void asn1::ActivateTraceModeArg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(0);
}

void asn1::ActivateTraceModeArg::set_omc_Id(){
	if(_omc_Id == NULL) _omc_Id = (AddressString*)create_node(3);
}

void asn1::ActivateTraceModeArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(4);
}

void asn1::ActivateTraceModeArg::set_traceReference2(){
	if(_traceReference2 == NULL) _traceReference2 = (TraceReference2*)create_node(5);
}

void asn1::ActivateTraceModeArg::set_traceDepthList(){
	if(_traceDepthList == NULL) _traceDepthList = (TraceDepthList*)create_node(6);
}

void asn1::ActivateTraceModeArg::set_traceNE_TypeList(){
	if(_traceNE_TypeList == NULL) _traceNE_TypeList = (TraceNE_TypeList*)create_node(7);
}

void asn1::ActivateTraceModeArg::set_traceInterfaceList(){
	if(_traceInterfaceList == NULL) _traceInterfaceList = (TraceInterfaceList*)create_node(8);
}

void asn1::ActivateTraceModeArg::set_traceEventList(){
	if(_traceEventList == NULL) _traceEventList = (TraceEventList*)create_node(9);
}

void asn1::ActivateTraceModeArg::set_traceCollectionEntity(){
	if(_traceCollectionEntity == NULL) _traceCollectionEntity = (GSN_Address*)create_node(10);
}

void asn1::ActivateTraceModeArg::set_mdt_Configuration(){
	if(_mdt_Configuration == NULL) _mdt_Configuration = (MDT_Configuration*)create_node(11);
}

//MDT_Configuration
asn1::MDT_Configuration::MDT_Configuration(){
	strcpy(node_type_name, "MDT_Configuration");
	// jobType
	_jobType = NULL;
	_jobType = new JobType();
	children.push_back(_jobType);

	// areaScope
	_areaScope = NULL;
	children.push_back(_areaScope);

	// listOfMeasurements
	_listOfMeasurements = NULL;
	children.push_back(_listOfMeasurements);

	// reportingTrigger
	_reportingTrigger = NULL;
	children.push_back(_reportingTrigger);

	// reportInterval
	_reportInterval = NULL;
	children.push_back(_reportInterval);

	// reportAmount
	_reportAmount = NULL;
	children.push_back(_reportAmount);

	// eventThresholdRSRP
	_eventThresholdRSRP = NULL;
	children.push_back(_eventThresholdRSRP);

	// eventThresholdRSRQ
	_eventThresholdRSRQ = NULL;
	children.push_back(_eventThresholdRSRQ);

	// loggingInterval
	_loggingInterval = NULL;
	children.push_back(_loggingInterval);

	// loggingDuration
	_loggingDuration = NULL;
	children.push_back(_loggingDuration);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::MDT_Configuration::~MDT_Configuration(){

}

asn1::ASN1Node* asn1::MDT_Configuration::create_node(unsigned int _index){
	switch(_index){
		case 1:
		{
			_areaScope = new AreaScope();
			children[1] = _areaScope;
			return _areaScope;
		}
		case 2:
		{
			_listOfMeasurements = new ListOfMeasurements();
			children[2] = _listOfMeasurements;
			return _listOfMeasurements;
		}
		case 3:
		{
			_reportingTrigger = new ReportingTrigger();
			_reportingTrigger->tlv->tag_class = CONTEXT_SPECIFIC;
			_reportingTrigger->tlv->tag_value = 0;
			children[3] = _reportingTrigger;
			return _reportingTrigger;
		}
		case 4:
		{
			_reportInterval = new ReportInterval();
			children[4] = _reportInterval;
			return _reportInterval;
		}
		case 5:
		{
			_reportAmount = new ReportAmount();
			_reportAmount->tlv->tag_class = CONTEXT_SPECIFIC;
			_reportAmount->tlv->tag_value = 1;
			children[5] = _reportAmount;
			return _reportAmount;
		}
		case 6:
		{
			_eventThresholdRSRP = new EventThresholdRSRP();
			children[6] = _eventThresholdRSRP;
			return _eventThresholdRSRP;
		}
		case 7:
		{
			_eventThresholdRSRQ = new EventThresholdRSRQ();
			_eventThresholdRSRQ->tlv->tag_class = CONTEXT_SPECIFIC;
			_eventThresholdRSRQ->tlv->tag_value = 2;
			children[7] = _eventThresholdRSRQ;
			return _eventThresholdRSRQ;
		}
		case 8:
		{
			_loggingInterval = new LoggingInterval();
			_loggingInterval->tlv->tag_class = CONTEXT_SPECIFIC;
			_loggingInterval->tlv->tag_value = 3;
			children[8] = _loggingInterval;
			return _loggingInterval;
		}
		case 9:
		{
			_loggingDuration = new LoggingDuration();
			_loggingDuration->tlv->tag_class = CONTEXT_SPECIFIC;
			_loggingDuration->tlv->tag_value = 4;
			children[9] = _loggingDuration;
			return _loggingDuration;
		}
		case 10:
		{
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 5;
			children[10] = _extensionContainer;
			return _extensionContainer;
		}

		default: return NULL;
	}
}

void asn1::MDT_Configuration::set_areaScope(){
	if(_areaScope == NULL) _areaScope = (AreaScope*)create_node(1);
}

void asn1::MDT_Configuration::set_listOfMeasurements(){
	if(_listOfMeasurements == NULL) _listOfMeasurements = (ListOfMeasurements*)create_node(2);
}

void asn1::MDT_Configuration::set_reportingTrigger(){
	if(_reportingTrigger == NULL) _reportingTrigger = (ReportingTrigger*)create_node(3);
}

void asn1::MDT_Configuration::set_reportInterval(){
	if(_reportInterval == NULL) _reportInterval = (ReportInterval*)create_node(4);
}

void asn1::MDT_Configuration::set_reportAmount(){
	if(_reportAmount == NULL) _reportAmount = (ReportAmount*)create_node(5);
}

void asn1::MDT_Configuration::set_eventThresholdRSRP(){
	if(_eventThresholdRSRP == NULL) _eventThresholdRSRP = (EventThresholdRSRP*)create_node(6);
}

void asn1::MDT_Configuration::set_eventThresholdRSRQ(){
	if(_eventThresholdRSRQ == NULL) _eventThresholdRSRQ = (EventThresholdRSRQ*)create_node(7);
}

void asn1::MDT_Configuration::set_loggingInterval(){
	if(_loggingInterval == NULL) _loggingInterval = (LoggingInterval*)create_node(8);
}

void asn1::MDT_Configuration::set_loggingDuration(){
	if(_loggingDuration == NULL) _loggingDuration = (LoggingDuration*)create_node(9);
}

void asn1::MDT_Configuration::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(10);
}

//JobType
asn1::JobType::JobType(){
	strcpy(node_type_name, "JobType");

}
asn1::JobType::~JobType(){

}

//AreaScope
asn1::AreaScope::AreaScope(){
	strcpy(node_type_name, "AreaScope");
	// cgi_List
	_cgi_List = NULL;
	children.push_back(_cgi_List);

	// e_utran_cgi_List
	_e_utran_cgi_List = NULL;
	children.push_back(_e_utran_cgi_List);

	// routingAreaId_List
	_routingAreaId_List = NULL;
	children.push_back(_routingAreaId_List);

	// locationAreaId_List
	_locationAreaId_List = NULL;
	children.push_back(_locationAreaId_List);

	// trackingAreaId_List
	_trackingAreaId_List = NULL;
	children.push_back(_trackingAreaId_List);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::AreaScope::~AreaScope(){

}

asn1::ASN1Node* asn1::AreaScope::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_cgi_List = new CGI_List();
			_cgi_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_cgi_List->tlv->tag_value = 0;
			children[0] = _cgi_List;
			return _cgi_List;
		}
		case 1:
		{
			_e_utran_cgi_List = new E_UTRAN_CGI_List();
			_e_utran_cgi_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_e_utran_cgi_List->tlv->tag_value = 1;
			children[1] = _e_utran_cgi_List;
			return _e_utran_cgi_List;
		}
		case 2:
		{
			_routingAreaId_List = new RoutingAreaId_List();
			_routingAreaId_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_routingAreaId_List->tlv->tag_value = 2;
			children[2] = _routingAreaId_List;
			return _routingAreaId_List;
		}
		case 3:
		{
			_locationAreaId_List = new LocationAreaId_List();
			_locationAreaId_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_locationAreaId_List->tlv->tag_value = 3;
			children[3] = _locationAreaId_List;
			return _locationAreaId_List;
		}
		case 4:
		{
			_trackingAreaId_List = new TrackingAreaId_List();
			_trackingAreaId_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_trackingAreaId_List->tlv->tag_value = 4;
			children[4] = _trackingAreaId_List;
			return _trackingAreaId_List;
		}
		case 5:
		{
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 5;
			children[5] = _extensionContainer;
			return _extensionContainer;
		}

		default: return NULL;
	}
}

void asn1::AreaScope::set_cgi_List(){
	if(_cgi_List == NULL) _cgi_List = (CGI_List*)create_node(0);
}

void asn1::AreaScope::set_e_utran_cgi_List(){
	if(_e_utran_cgi_List == NULL) _e_utran_cgi_List = (E_UTRAN_CGI_List*)create_node(1);
}

void asn1::AreaScope::set_routingAreaId_List(){
	if(_routingAreaId_List == NULL) _routingAreaId_List = (RoutingAreaId_List*)create_node(2);
}

void asn1::AreaScope::set_locationAreaId_List(){
	if(_locationAreaId_List == NULL) _locationAreaId_List = (LocationAreaId_List*)create_node(3);
}

void asn1::AreaScope::set_trackingAreaId_List(){
	if(_trackingAreaId_List == NULL) _trackingAreaId_List = (TrackingAreaId_List*)create_node(4);
}

void asn1::AreaScope::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(5);
}

//CGI_List
asn1::CGI_List::CGI_List(){
	strcpy(node_type_name, "CGI_List");

}
asn1::CGI_List::~CGI_List(){
}

asn1::GlobalCellId* asn1::CGI_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (GlobalCellId*)children[child_index]; else return NULL;
}

void asn1::CGI_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::CGI_List::create_node(unsigned int _index){
	children[_index] = new GlobalCellId();
	return children[_index];
}

asn1::ASN1Node* asn1::CGI_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new GlobalCellId());
		return children[children.size() - 1];
	}

}

//E_UTRAN_CGI_List
asn1::E_UTRAN_CGI_List::E_UTRAN_CGI_List(){
	strcpy(node_type_name, "E_UTRAN_CGI_List");

}
asn1::E_UTRAN_CGI_List::~E_UTRAN_CGI_List(){
}

asn1::E_UTRAN_CGI* asn1::E_UTRAN_CGI_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (E_UTRAN_CGI*)children[child_index]; else return NULL;
}

void asn1::E_UTRAN_CGI_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::E_UTRAN_CGI_List::create_node(unsigned int _index){
	children[_index] = new E_UTRAN_CGI();
	return children[_index];
}

asn1::ASN1Node* asn1::E_UTRAN_CGI_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new E_UTRAN_CGI());
		return children[children.size() - 1];
	}

}

//RoutingAreaId_List
asn1::RoutingAreaId_List::RoutingAreaId_List(){
	strcpy(node_type_name, "RoutingAreaId_List");

}
asn1::RoutingAreaId_List::~RoutingAreaId_List(){
}

asn1::RAIdentity* asn1::RoutingAreaId_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (RAIdentity*)children[child_index]; else return NULL;
}

void asn1::RoutingAreaId_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::RoutingAreaId_List::create_node(unsigned int _index){
	children[_index] = new RAIdentity();
	return children[_index];
}

asn1::ASN1Node* asn1::RoutingAreaId_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new RAIdentity());
		return children[children.size() - 1];
	}

}

//LocationAreaId_List
asn1::LocationAreaId_List::LocationAreaId_List(){
	strcpy(node_type_name, "LocationAreaId_List");

}
asn1::LocationAreaId_List::~LocationAreaId_List(){
}

asn1::LAIFixedLength* asn1::LocationAreaId_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (LAIFixedLength*)children[child_index]; else return NULL;
}

void asn1::LocationAreaId_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::LocationAreaId_List::create_node(unsigned int _index){
	children[_index] = new LAIFixedLength();
	return children[_index];
}

asn1::ASN1Node* asn1::LocationAreaId_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new LAIFixedLength());
		return children[children.size() - 1];
	}

}

//TrackingAreaId_List
asn1::TrackingAreaId_List::TrackingAreaId_List(){
	strcpy(node_type_name, "TrackingAreaId_List");

}
asn1::TrackingAreaId_List::~TrackingAreaId_List(){
}

asn1::TA_Id* asn1::TrackingAreaId_List::get_child(unsigned int child_index){
	if(child_index < children.size()) return (TA_Id*)children[child_index]; else return NULL;
}

void asn1::TrackingAreaId_List::set_child(unsigned int child_index){
	if(child_index < children.size()){
		if(children[child_index] == NULL) children[child_index] = create_node(child_index);
	}else get_next_node(child_index);
}

asn1::ASN1Node* asn1::TrackingAreaId_List::create_node(unsigned int _index){
	children[_index] = new TA_Id();
	return children[_index];
}

asn1::ASN1Node* asn1::TrackingAreaId_List::get_next_node(unsigned int _index){
	if(_index < children.size()) return children[_index]; else{
		children.push_back(new TA_Id());
		return children[children.size() - 1];
	}

}

//ListOfMeasurements
asn1::ListOfMeasurements::ListOfMeasurements(){
	strcpy(node_type_name, "ListOfMeasurements");

}
asn1::ListOfMeasurements::~ListOfMeasurements(){

}

//ReportingTrigger
asn1::ReportingTrigger::ReportingTrigger(){
	strcpy(node_type_name, "ReportingTrigger");

}
asn1::ReportingTrigger::~ReportingTrigger(){

}

//ReportInterval
asn1::ReportInterval::ReportInterval(){
	strcpy(node_type_name, "ReportInterval");

}
asn1::ReportInterval::~ReportInterval(){

}

//ReportAmount
asn1::ReportAmount::ReportAmount(){
	strcpy(node_type_name, "ReportAmount");

}
asn1::ReportAmount::~ReportAmount(){

}

//EventThresholdRSRP
asn1::EventThresholdRSRP::EventThresholdRSRP(){
	strcpy(node_type_name, "EventThresholdRSRP");

}
asn1::EventThresholdRSRP::~EventThresholdRSRP(){

}

//EventThresholdRSRQ
asn1::EventThresholdRSRQ::EventThresholdRSRQ(){
	strcpy(node_type_name, "EventThresholdRSRQ");

}
asn1::EventThresholdRSRQ::~EventThresholdRSRQ(){

}

//LoggingInterval
asn1::LoggingInterval::LoggingInterval(){
	strcpy(node_type_name, "LoggingInterval");

}
asn1::LoggingInterval::~LoggingInterval(){

}

//LoggingDuration
asn1::LoggingDuration::LoggingDuration(){
	strcpy(node_type_name, "LoggingDuration");

}
asn1::LoggingDuration::~LoggingDuration(){

}

//TraceReference
asn1::TraceReference::TraceReference(){
	strcpy(node_type_name, "TraceReference");

}
asn1::TraceReference::~TraceReference(){

}

//TraceReference2
asn1::TraceReference2::TraceReference2(){
	strcpy(node_type_name, "TraceReference2");

}
asn1::TraceReference2::~TraceReference2(){

}

//TraceRecordingSessionReference
asn1::TraceRecordingSessionReference::TraceRecordingSessionReference(){
	strcpy(node_type_name, "TraceRecordingSessionReference");

}
asn1::TraceRecordingSessionReference::~TraceRecordingSessionReference(){

}

//TraceType
asn1::TraceType::TraceType(){
	strcpy(node_type_name, "TraceType");

}
asn1::TraceType::~TraceType(){

}

//TraceDepthList
asn1::TraceDepthList::TraceDepthList(){
	strcpy(node_type_name, "TraceDepthList");
	// msc_s_TraceDepth
	_msc_s_TraceDepth = NULL;
	children.push_back(_msc_s_TraceDepth);

	// mgw_TraceDepth
	_mgw_TraceDepth = NULL;
	children.push_back(_mgw_TraceDepth);

	// sgsn_TraceDepth
	_sgsn_TraceDepth = NULL;
	children.push_back(_sgsn_TraceDepth);

	// ggsn_TraceDepth
	_ggsn_TraceDepth = NULL;
	children.push_back(_ggsn_TraceDepth);

	// rnc_TraceDepth
	_rnc_TraceDepth = NULL;
	children.push_back(_rnc_TraceDepth);

	// bmsc_TraceDepth
	_bmsc_TraceDepth = NULL;
	children.push_back(_bmsc_TraceDepth);

	// mme_TraceDepth
	_mme_TraceDepth = NULL;
	children.push_back(_mme_TraceDepth);

	// sgw_TraceDepth
	_sgw_TraceDepth = NULL;
	children.push_back(_sgw_TraceDepth);

	// pgw_TraceDepth
	_pgw_TraceDepth = NULL;
	children.push_back(_pgw_TraceDepth);

	// eNB_TraceDepth
	_eNB_TraceDepth = NULL;
	children.push_back(_eNB_TraceDepth);

	// msc_s_TraceDepthExtension
	_msc_s_TraceDepthExtension = NULL;
	children.push_back(_msc_s_TraceDepthExtension);

	// mgw_TraceDepthExtension
	_mgw_TraceDepthExtension = NULL;
	children.push_back(_mgw_TraceDepthExtension);

	// sgsn_TraceDepthExtension
	_sgsn_TraceDepthExtension = NULL;
	children.push_back(_sgsn_TraceDepthExtension);

	// ggsn_TraceDepthExtension
	_ggsn_TraceDepthExtension = NULL;
	children.push_back(_ggsn_TraceDepthExtension);

	// rnc_TraceDepthExtension
	_rnc_TraceDepthExtension = NULL;
	children.push_back(_rnc_TraceDepthExtension);

	// bmsc_TraceDepthExtension
	_bmsc_TraceDepthExtension = NULL;
	children.push_back(_bmsc_TraceDepthExtension);

	// mme_TraceDepthExtension
	_mme_TraceDepthExtension = NULL;
	children.push_back(_mme_TraceDepthExtension);

	// sgw_TraceDepthExtension
	_sgw_TraceDepthExtension = NULL;
	children.push_back(_sgw_TraceDepthExtension);

	// pgw_TraceDepthExtension
	_pgw_TraceDepthExtension = NULL;
	children.push_back(_pgw_TraceDepthExtension);

	// eNB_TraceDepthExtension
	_eNB_TraceDepthExtension = NULL;
	children.push_back(_eNB_TraceDepthExtension);


}
asn1::TraceDepthList::~TraceDepthList(){

}

asn1::ASN1Node* asn1::TraceDepthList::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_msc_s_TraceDepth = new TraceDepth();
			_msc_s_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_msc_s_TraceDepth->tlv->tag_value = 0;
			children[0] = _msc_s_TraceDepth;
			return _msc_s_TraceDepth;
		}
		case 1:
		{
			_mgw_TraceDepth = new TraceDepth();
			_mgw_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_mgw_TraceDepth->tlv->tag_value = 1;
			children[1] = _mgw_TraceDepth;
			return _mgw_TraceDepth;
		}
		case 2:
		{
			_sgsn_TraceDepth = new TraceDepth();
			_sgsn_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_TraceDepth->tlv->tag_value = 2;
			children[2] = _sgsn_TraceDepth;
			return _sgsn_TraceDepth;
		}
		case 3:
		{
			_ggsn_TraceDepth = new TraceDepth();
			_ggsn_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_TraceDepth->tlv->tag_value = 3;
			children[3] = _ggsn_TraceDepth;
			return _ggsn_TraceDepth;
		}
		case 4:
		{
			_rnc_TraceDepth = new TraceDepth();
			_rnc_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_rnc_TraceDepth->tlv->tag_value = 4;
			children[4] = _rnc_TraceDepth;
			return _rnc_TraceDepth;
		}
		case 5:
		{
			_bmsc_TraceDepth = new TraceDepth();
			_bmsc_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_bmsc_TraceDepth->tlv->tag_value = 5;
			children[5] = _bmsc_TraceDepth;
			return _bmsc_TraceDepth;
		}
		case 6:
		{
			_mme_TraceDepth = new TraceDepth();
			_mme_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_mme_TraceDepth->tlv->tag_value = 6;
			children[6] = _mme_TraceDepth;
			return _mme_TraceDepth;
		}
		case 7:
		{
			_sgw_TraceDepth = new TraceDepth();
			_sgw_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgw_TraceDepth->tlv->tag_value = 7;
			children[7] = _sgw_TraceDepth;
			return _sgw_TraceDepth;
		}
		case 8:
		{
			_pgw_TraceDepth = new TraceDepth();
			_pgw_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_pgw_TraceDepth->tlv->tag_value = 8;
			children[8] = _pgw_TraceDepth;
			return _pgw_TraceDepth;
		}
		case 9:
		{
			_eNB_TraceDepth = new TraceDepth();
			_eNB_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_eNB_TraceDepth->tlv->tag_value = 9;
			children[9] = _eNB_TraceDepth;
			return _eNB_TraceDepth;
		}
		case 10:
		{
			_msc_s_TraceDepthExtension = new TraceDepthExtension();
			_msc_s_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_msc_s_TraceDepthExtension->tlv->tag_value = 10;
			children[10] = _msc_s_TraceDepthExtension;
			return _msc_s_TraceDepthExtension;
		}
		case 11:
		{
			_mgw_TraceDepthExtension = new TraceDepthExtension();
			_mgw_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_mgw_TraceDepthExtension->tlv->tag_value = 11;
			children[11] = _mgw_TraceDepthExtension;
			return _mgw_TraceDepthExtension;
		}
		case 12:
		{
			_sgsn_TraceDepthExtension = new TraceDepthExtension();
			_sgsn_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_TraceDepthExtension->tlv->tag_value = 12;
			children[12] = _sgsn_TraceDepthExtension;
			return _sgsn_TraceDepthExtension;
		}
		case 13:
		{
			_ggsn_TraceDepthExtension = new TraceDepthExtension();
			_ggsn_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_TraceDepthExtension->tlv->tag_value = 13;
			children[13] = _ggsn_TraceDepthExtension;
			return _ggsn_TraceDepthExtension;
		}
		case 14:
		{
			_rnc_TraceDepthExtension = new TraceDepthExtension();
			_rnc_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_rnc_TraceDepthExtension->tlv->tag_value = 14;
			children[14] = _rnc_TraceDepthExtension;
			return _rnc_TraceDepthExtension;
		}
		case 15:
		{
			_bmsc_TraceDepthExtension = new TraceDepthExtension();
			_bmsc_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_bmsc_TraceDepthExtension->tlv->tag_value = 15;
			children[15] = _bmsc_TraceDepthExtension;
			return _bmsc_TraceDepthExtension;
		}
		case 16:
		{
			_mme_TraceDepthExtension = new TraceDepthExtension();
			_mme_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_mme_TraceDepthExtension->tlv->tag_value = 16;
			children[16] = _mme_TraceDepthExtension;
			return _mme_TraceDepthExtension;
		}
		case 17:
		{
			_sgw_TraceDepthExtension = new TraceDepthExtension();
			_sgw_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgw_TraceDepthExtension->tlv->tag_value = 17;
			children[17] = _sgw_TraceDepthExtension;
			return _sgw_TraceDepthExtension;
		}
		case 18:
		{
			_pgw_TraceDepthExtension = new TraceDepthExtension();
			_pgw_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_pgw_TraceDepthExtension->tlv->tag_value = 18;
			children[18] = _pgw_TraceDepthExtension;
			return _pgw_TraceDepthExtension;
		}
		case 19:
		{
			_eNB_TraceDepthExtension = new TraceDepthExtension();
			_eNB_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_eNB_TraceDepthExtension->tlv->tag_value = 19;
			children[19] = _eNB_TraceDepthExtension;
			return _eNB_TraceDepthExtension;
		}

		default: return NULL;
	}
}

void asn1::TraceDepthList::set_msc_s_TraceDepth(){
	if(_msc_s_TraceDepth == NULL) _msc_s_TraceDepth = (TraceDepth*)create_node(0);
}

void asn1::TraceDepthList::set_mgw_TraceDepth(){
	if(_mgw_TraceDepth == NULL) _mgw_TraceDepth = (TraceDepth*)create_node(1);
}

void asn1::TraceDepthList::set_sgsn_TraceDepth(){
	if(_sgsn_TraceDepth == NULL) _sgsn_TraceDepth = (TraceDepth*)create_node(2);
}

void asn1::TraceDepthList::set_ggsn_TraceDepth(){
	if(_ggsn_TraceDepth == NULL) _ggsn_TraceDepth = (TraceDepth*)create_node(3);
}

void asn1::TraceDepthList::set_rnc_TraceDepth(){
	if(_rnc_TraceDepth == NULL) _rnc_TraceDepth = (TraceDepth*)create_node(4);
}

void asn1::TraceDepthList::set_bmsc_TraceDepth(){
	if(_bmsc_TraceDepth == NULL) _bmsc_TraceDepth = (TraceDepth*)create_node(5);
}

void asn1::TraceDepthList::set_mme_TraceDepth(){
	if(_mme_TraceDepth == NULL) _mme_TraceDepth = (TraceDepth*)create_node(6);
}

void asn1::TraceDepthList::set_sgw_TraceDepth(){
	if(_sgw_TraceDepth == NULL) _sgw_TraceDepth = (TraceDepth*)create_node(7);
}

void asn1::TraceDepthList::set_pgw_TraceDepth(){
	if(_pgw_TraceDepth == NULL) _pgw_TraceDepth = (TraceDepth*)create_node(8);
}

void asn1::TraceDepthList::set_eNB_TraceDepth(){
	if(_eNB_TraceDepth == NULL) _eNB_TraceDepth = (TraceDepth*)create_node(9);
}

void asn1::TraceDepthList::set_msc_s_TraceDepthExtension(){
	if(_msc_s_TraceDepthExtension == NULL) _msc_s_TraceDepthExtension = (TraceDepthExtension*)create_node(10);
}

void asn1::TraceDepthList::set_mgw_TraceDepthExtension(){
	if(_mgw_TraceDepthExtension == NULL) _mgw_TraceDepthExtension = (TraceDepthExtension*)create_node(11);
}

void asn1::TraceDepthList::set_sgsn_TraceDepthExtension(){
	if(_sgsn_TraceDepthExtension == NULL) _sgsn_TraceDepthExtension = (TraceDepthExtension*)create_node(12);
}

void asn1::TraceDepthList::set_ggsn_TraceDepthExtension(){
	if(_ggsn_TraceDepthExtension == NULL) _ggsn_TraceDepthExtension = (TraceDepthExtension*)create_node(13);
}

void asn1::TraceDepthList::set_rnc_TraceDepthExtension(){
	if(_rnc_TraceDepthExtension == NULL) _rnc_TraceDepthExtension = (TraceDepthExtension*)create_node(14);
}

void asn1::TraceDepthList::set_bmsc_TraceDepthExtension(){
	if(_bmsc_TraceDepthExtension == NULL) _bmsc_TraceDepthExtension = (TraceDepthExtension*)create_node(15);
}

void asn1::TraceDepthList::set_mme_TraceDepthExtension(){
	if(_mme_TraceDepthExtension == NULL) _mme_TraceDepthExtension = (TraceDepthExtension*)create_node(16);
}

void asn1::TraceDepthList::set_sgw_TraceDepthExtension(){
	if(_sgw_TraceDepthExtension == NULL) _sgw_TraceDepthExtension = (TraceDepthExtension*)create_node(17);
}

void asn1::TraceDepthList::set_pgw_TraceDepthExtension(){
	if(_pgw_TraceDepthExtension == NULL) _pgw_TraceDepthExtension = (TraceDepthExtension*)create_node(18);
}

void asn1::TraceDepthList::set_eNB_TraceDepthExtension(){
	if(_eNB_TraceDepthExtension == NULL) _eNB_TraceDepthExtension = (TraceDepthExtension*)create_node(19);
}

//TraceDepth
asn1::TraceDepth::TraceDepth(){
	strcpy(node_type_name, "TraceDepth");

}
asn1::TraceDepth::~TraceDepth(){

}

//TraceDepthExtension
asn1::TraceDepthExtension::TraceDepthExtension(){
	strcpy(node_type_name, "TraceDepthExtension");

}
asn1::TraceDepthExtension::~TraceDepthExtension(){

}

//TraceNE_TypeList
asn1::TraceNE_TypeList::TraceNE_TypeList(){
	strcpy(node_type_name, "TraceNE_TypeList");

}
asn1::TraceNE_TypeList::~TraceNE_TypeList(){

}

//TraceInterfaceList
asn1::TraceInterfaceList::TraceInterfaceList(){
	strcpy(node_type_name, "TraceInterfaceList");
	// msc_s_List
	_msc_s_List = NULL;
	children.push_back(_msc_s_List);

	// mgw_List
	_mgw_List = NULL;
	children.push_back(_mgw_List);

	// sgsn_List
	_sgsn_List = NULL;
	children.push_back(_sgsn_List);

	// ggsn_List
	_ggsn_List = NULL;
	children.push_back(_ggsn_List);

	// rnc_List
	_rnc_List = NULL;
	children.push_back(_rnc_List);

	// bmsc_List
	_bmsc_List = NULL;
	children.push_back(_bmsc_List);

	// mme_List
	_mme_List = NULL;
	children.push_back(_mme_List);

	// sgw_List
	_sgw_List = NULL;
	children.push_back(_sgw_List);

	// pgw_List
	_pgw_List = NULL;
	children.push_back(_pgw_List);

	// eNB_List
	_eNB_List = NULL;
	children.push_back(_eNB_List);


}
asn1::TraceInterfaceList::~TraceInterfaceList(){

}

asn1::ASN1Node* asn1::TraceInterfaceList::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_msc_s_List = new MSC_S_InterfaceList();
			_msc_s_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_msc_s_List->tlv->tag_value = 0;
			children[0] = _msc_s_List;
			return _msc_s_List;
		}
		case 1:
		{
			_mgw_List = new MGW_InterfaceList();
			_mgw_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_mgw_List->tlv->tag_value = 1;
			children[1] = _mgw_List;
			return _mgw_List;
		}
		case 2:
		{
			_sgsn_List = new SGSN_InterfaceList();
			_sgsn_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_List->tlv->tag_value = 2;
			children[2] = _sgsn_List;
			return _sgsn_List;
		}
		case 3:
		{
			_ggsn_List = new GGSN_InterfaceList();
			_ggsn_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_List->tlv->tag_value = 3;
			children[3] = _ggsn_List;
			return _ggsn_List;
		}
		case 4:
		{
			_rnc_List = new RNC_InterfaceList();
			_rnc_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_rnc_List->tlv->tag_value = 4;
			children[4] = _rnc_List;
			return _rnc_List;
		}
		case 5:
		{
			_bmsc_List = new BMSC_InterfaceList();
			_bmsc_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_bmsc_List->tlv->tag_value = 5;
			children[5] = _bmsc_List;
			return _bmsc_List;
		}
		case 6:
		{
			_mme_List = new MME_InterfaceList();
			_mme_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_mme_List->tlv->tag_value = 6;
			children[6] = _mme_List;
			return _mme_List;
		}
		case 7:
		{
			_sgw_List = new SGW_InterfaceList();
			_sgw_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgw_List->tlv->tag_value = 7;
			children[7] = _sgw_List;
			return _sgw_List;
		}
		case 8:
		{
			_pgw_List = new PGW_InterfaceList();
			_pgw_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_pgw_List->tlv->tag_value = 8;
			children[8] = _pgw_List;
			return _pgw_List;
		}
		case 9:
		{
			_eNB_List = new ENB_InterfaceList();
			_eNB_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_eNB_List->tlv->tag_value = 9;
			children[9] = _eNB_List;
			return _eNB_List;
		}

		default: return NULL;
	}
}

void asn1::TraceInterfaceList::set_msc_s_List(){
	if(_msc_s_List == NULL) _msc_s_List = (MSC_S_InterfaceList*)create_node(0);
}

void asn1::TraceInterfaceList::set_mgw_List(){
	if(_mgw_List == NULL) _mgw_List = (MGW_InterfaceList*)create_node(1);
}

void asn1::TraceInterfaceList::set_sgsn_List(){
	if(_sgsn_List == NULL) _sgsn_List = (SGSN_InterfaceList*)create_node(2);
}

void asn1::TraceInterfaceList::set_ggsn_List(){
	if(_ggsn_List == NULL) _ggsn_List = (GGSN_InterfaceList*)create_node(3);
}

void asn1::TraceInterfaceList::set_rnc_List(){
	if(_rnc_List == NULL) _rnc_List = (RNC_InterfaceList*)create_node(4);
}

void asn1::TraceInterfaceList::set_bmsc_List(){
	if(_bmsc_List == NULL) _bmsc_List = (BMSC_InterfaceList*)create_node(5);
}

void asn1::TraceInterfaceList::set_mme_List(){
	if(_mme_List == NULL) _mme_List = (MME_InterfaceList*)create_node(6);
}

void asn1::TraceInterfaceList::set_sgw_List(){
	if(_sgw_List == NULL) _sgw_List = (SGW_InterfaceList*)create_node(7);
}

void asn1::TraceInterfaceList::set_pgw_List(){
	if(_pgw_List == NULL) _pgw_List = (PGW_InterfaceList*)create_node(8);
}

void asn1::TraceInterfaceList::set_eNB_List(){
	if(_eNB_List == NULL) _eNB_List = (ENB_InterfaceList*)create_node(9);
}

//MSC_S_InterfaceList
asn1::MSC_S_InterfaceList::MSC_S_InterfaceList(){
	strcpy(node_type_name, "MSC_S_InterfaceList");

}
asn1::MSC_S_InterfaceList::~MSC_S_InterfaceList(){

}

//MGW_InterfaceList
asn1::MGW_InterfaceList::MGW_InterfaceList(){
	strcpy(node_type_name, "MGW_InterfaceList");

}
asn1::MGW_InterfaceList::~MGW_InterfaceList(){

}

//SGSN_InterfaceList
asn1::SGSN_InterfaceList::SGSN_InterfaceList(){
	strcpy(node_type_name, "SGSN_InterfaceList");

}
asn1::SGSN_InterfaceList::~SGSN_InterfaceList(){

}

//GGSN_InterfaceList
asn1::GGSN_InterfaceList::GGSN_InterfaceList(){
	strcpy(node_type_name, "GGSN_InterfaceList");

}
asn1::GGSN_InterfaceList::~GGSN_InterfaceList(){

}

//RNC_InterfaceList
asn1::RNC_InterfaceList::RNC_InterfaceList(){
	strcpy(node_type_name, "RNC_InterfaceList");

}
asn1::RNC_InterfaceList::~RNC_InterfaceList(){

}

//BMSC_InterfaceList
asn1::BMSC_InterfaceList::BMSC_InterfaceList(){
	strcpy(node_type_name, "BMSC_InterfaceList");

}
asn1::BMSC_InterfaceList::~BMSC_InterfaceList(){

}

//MME_InterfaceList
asn1::MME_InterfaceList::MME_InterfaceList(){
	strcpy(node_type_name, "MME_InterfaceList");

}
asn1::MME_InterfaceList::~MME_InterfaceList(){

}

//SGW_InterfaceList
asn1::SGW_InterfaceList::SGW_InterfaceList(){
	strcpy(node_type_name, "SGW_InterfaceList");

}
asn1::SGW_InterfaceList::~SGW_InterfaceList(){

}

//PGW_InterfaceList
asn1::PGW_InterfaceList::PGW_InterfaceList(){
	strcpy(node_type_name, "PGW_InterfaceList");

}
asn1::PGW_InterfaceList::~PGW_InterfaceList(){

}

//ENB_InterfaceList
asn1::ENB_InterfaceList::ENB_InterfaceList(){
	strcpy(node_type_name, "ENB_InterfaceList");

}
asn1::ENB_InterfaceList::~ENB_InterfaceList(){

}

//TraceEventList
asn1::TraceEventList::TraceEventList(){
	strcpy(node_type_name, "TraceEventList");
	// msc_s_List
	_msc_s_List = NULL;
	children.push_back(_msc_s_List);

	// mgw_List
	_mgw_List = NULL;
	children.push_back(_mgw_List);

	// sgsn_List
	_sgsn_List = NULL;
	children.push_back(_sgsn_List);

	// ggsn_List
	_ggsn_List = NULL;
	children.push_back(_ggsn_List);

	// bmsc_List
	_bmsc_List = NULL;
	children.push_back(_bmsc_List);

	// mme_List
	_mme_List = NULL;
	children.push_back(_mme_List);

	// sgw_List
	_sgw_List = NULL;
	children.push_back(_sgw_List);

	// pgw_List
	_pgw_List = NULL;
	children.push_back(_pgw_List);


}
asn1::TraceEventList::~TraceEventList(){

}

asn1::ASN1Node* asn1::TraceEventList::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_msc_s_List = new MSC_S_EventList();
			_msc_s_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_msc_s_List->tlv->tag_value = 0;
			children[0] = _msc_s_List;
			return _msc_s_List;
		}
		case 1:
		{
			_mgw_List = new MGW_EventList();
			_mgw_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_mgw_List->tlv->tag_value = 1;
			children[1] = _mgw_List;
			return _mgw_List;
		}
		case 2:
		{
			_sgsn_List = new SGSN_EventList();
			_sgsn_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgsn_List->tlv->tag_value = 2;
			children[2] = _sgsn_List;
			return _sgsn_List;
		}
		case 3:
		{
			_ggsn_List = new GGSN_EventList();
			_ggsn_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_ggsn_List->tlv->tag_value = 3;
			children[3] = _ggsn_List;
			return _ggsn_List;
		}
		case 4:
		{
			_bmsc_List = new BMSC_EventList();
			_bmsc_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_bmsc_List->tlv->tag_value = 4;
			children[4] = _bmsc_List;
			return _bmsc_List;
		}
		case 5:
		{
			_mme_List = new MME_EventList();
			_mme_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_mme_List->tlv->tag_value = 5;
			children[5] = _mme_List;
			return _mme_List;
		}
		case 6:
		{
			_sgw_List = new SGW_EventList();
			_sgw_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_sgw_List->tlv->tag_value = 6;
			children[6] = _sgw_List;
			return _sgw_List;
		}
		case 7:
		{
			_pgw_List = new PGW_EventList();
			_pgw_List->tlv->tag_class = CONTEXT_SPECIFIC;
			_pgw_List->tlv->tag_value = 7;
			children[7] = _pgw_List;
			return _pgw_List;
		}

		default: return NULL;
	}
}

void asn1::TraceEventList::set_msc_s_List(){
	if(_msc_s_List == NULL) _msc_s_List = (MSC_S_EventList*)create_node(0);
}

void asn1::TraceEventList::set_mgw_List(){
	if(_mgw_List == NULL) _mgw_List = (MGW_EventList*)create_node(1);
}

void asn1::TraceEventList::set_sgsn_List(){
	if(_sgsn_List == NULL) _sgsn_List = (SGSN_EventList*)create_node(2);
}

void asn1::TraceEventList::set_ggsn_List(){
	if(_ggsn_List == NULL) _ggsn_List = (GGSN_EventList*)create_node(3);
}

void asn1::TraceEventList::set_bmsc_List(){
	if(_bmsc_List == NULL) _bmsc_List = (BMSC_EventList*)create_node(4);
}

void asn1::TraceEventList::set_mme_List(){
	if(_mme_List == NULL) _mme_List = (MME_EventList*)create_node(5);
}

void asn1::TraceEventList::set_sgw_List(){
	if(_sgw_List == NULL) _sgw_List = (SGW_EventList*)create_node(6);
}

void asn1::TraceEventList::set_pgw_List(){
	if(_pgw_List == NULL) _pgw_List = (PGW_EventList*)create_node(7);
}

//MSC_S_EventList
asn1::MSC_S_EventList::MSC_S_EventList(){
	strcpy(node_type_name, "MSC_S_EventList");

}
asn1::MSC_S_EventList::~MSC_S_EventList(){

}

//MGW_EventList
asn1::MGW_EventList::MGW_EventList(){
	strcpy(node_type_name, "MGW_EventList");

}
asn1::MGW_EventList::~MGW_EventList(){

}

//SGSN_EventList
asn1::SGSN_EventList::SGSN_EventList(){
	strcpy(node_type_name, "SGSN_EventList");

}
asn1::SGSN_EventList::~SGSN_EventList(){

}

//GGSN_EventList
asn1::GGSN_EventList::GGSN_EventList(){
	strcpy(node_type_name, "GGSN_EventList");

}
asn1::GGSN_EventList::~GGSN_EventList(){

}

//BMSC_EventList
asn1::BMSC_EventList::BMSC_EventList(){
	strcpy(node_type_name, "BMSC_EventList");

}
asn1::BMSC_EventList::~BMSC_EventList(){

}

//MME_EventList
asn1::MME_EventList::MME_EventList(){
	strcpy(node_type_name, "MME_EventList");

}
asn1::MME_EventList::~MME_EventList(){

}

//SGW_EventList
asn1::SGW_EventList::SGW_EventList(){
	strcpy(node_type_name, "SGW_EventList");

}
asn1::SGW_EventList::~SGW_EventList(){

}

//PGW_EventList
asn1::PGW_EventList::PGW_EventList(){
	strcpy(node_type_name, "PGW_EventList");

}
asn1::PGW_EventList::~PGW_EventList(){

}

//TracePropagationList
asn1::TracePropagationList::TracePropagationList(){
	strcpy(node_type_name, "TracePropagationList");
	// traceReference
	_traceReference = NULL;
	children.push_back(_traceReference);

	// traceType
	_traceType = NULL;
	children.push_back(_traceType);

	// traceReference2
	_traceReference2 = NULL;
	children.push_back(_traceReference2);

	// traceRecordingSessionReference
	_traceRecordingSessionReference = NULL;
	children.push_back(_traceRecordingSessionReference);

	// rnc_TraceDepth
	_rnc_TraceDepth = NULL;
	children.push_back(_rnc_TraceDepth);

	// rnc_InterfaceList
	_rnc_InterfaceList = NULL;
	children.push_back(_rnc_InterfaceList);

	// msc_s_TraceDepth
	_msc_s_TraceDepth = NULL;
	children.push_back(_msc_s_TraceDepth);

	// msc_s_InterfaceList
	_msc_s_InterfaceList = NULL;
	children.push_back(_msc_s_InterfaceList);

	// msc_s_EventList
	_msc_s_EventList = NULL;
	children.push_back(_msc_s_EventList);

	// mgw_TraceDepth
	_mgw_TraceDepth = NULL;
	children.push_back(_mgw_TraceDepth);

	// mgw_InterfaceList
	_mgw_InterfaceList = NULL;
	children.push_back(_mgw_InterfaceList);

	// mgw_EventList
	_mgw_EventList = NULL;
	children.push_back(_mgw_EventList);

	// rnc_TraceDepthExtension
	_rnc_TraceDepthExtension = NULL;
	children.push_back(_rnc_TraceDepthExtension);

	// msc_s_TraceDepthExtension
	_msc_s_TraceDepthExtension = NULL;
	children.push_back(_msc_s_TraceDepthExtension);

	// mgw_TraceDepthExtension
	_mgw_TraceDepthExtension = NULL;
	children.push_back(_mgw_TraceDepthExtension);


}
asn1::TracePropagationList::~TracePropagationList(){

}

asn1::ASN1Node* asn1::TracePropagationList::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_traceReference = new TraceReference();
			_traceReference->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceReference->tlv->tag_value = 0;
			children[0] = _traceReference;
			return _traceReference;
		}
		case 1:
		{
			_traceType = new TraceType();
			_traceType->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceType->tlv->tag_value = 1;
			children[1] = _traceType;
			return _traceType;
		}
		case 2:
		{
			_traceReference2 = new TraceReference2();
			_traceReference2->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceReference2->tlv->tag_value = 2;
			children[2] = _traceReference2;
			return _traceReference2;
		}
		case 3:
		{
			_traceRecordingSessionReference = new TraceRecordingSessionReference();
			_traceRecordingSessionReference->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceRecordingSessionReference->tlv->tag_value = 3;
			children[3] = _traceRecordingSessionReference;
			return _traceRecordingSessionReference;
		}
		case 4:
		{
			_rnc_TraceDepth = new TraceDepth();
			_rnc_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_rnc_TraceDepth->tlv->tag_value = 4;
			children[4] = _rnc_TraceDepth;
			return _rnc_TraceDepth;
		}
		case 5:
		{
			_rnc_InterfaceList = new RNC_InterfaceList();
			_rnc_InterfaceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_rnc_InterfaceList->tlv->tag_value = 5;
			children[5] = _rnc_InterfaceList;
			return _rnc_InterfaceList;
		}
		case 6:
		{
			_msc_s_TraceDepth = new TraceDepth();
			_msc_s_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_msc_s_TraceDepth->tlv->tag_value = 6;
			children[6] = _msc_s_TraceDepth;
			return _msc_s_TraceDepth;
		}
		case 7:
		{
			_msc_s_InterfaceList = new MSC_S_InterfaceList();
			_msc_s_InterfaceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_msc_s_InterfaceList->tlv->tag_value = 7;
			children[7] = _msc_s_InterfaceList;
			return _msc_s_InterfaceList;
		}
		case 8:
		{
			_msc_s_EventList = new MSC_S_EventList();
			_msc_s_EventList->tlv->tag_class = CONTEXT_SPECIFIC;
			_msc_s_EventList->tlv->tag_value = 8;
			children[8] = _msc_s_EventList;
			return _msc_s_EventList;
		}
		case 9:
		{
			_mgw_TraceDepth = new TraceDepth();
			_mgw_TraceDepth->tlv->tag_class = CONTEXT_SPECIFIC;
			_mgw_TraceDepth->tlv->tag_value = 9;
			children[9] = _mgw_TraceDepth;
			return _mgw_TraceDepth;
		}
		case 10:
		{
			_mgw_InterfaceList = new MGW_InterfaceList();
			_mgw_InterfaceList->tlv->tag_class = CONTEXT_SPECIFIC;
			_mgw_InterfaceList->tlv->tag_value = 10;
			children[10] = _mgw_InterfaceList;
			return _mgw_InterfaceList;
		}
		case 11:
		{
			_mgw_EventList = new MGW_EventList();
			_mgw_EventList->tlv->tag_class = CONTEXT_SPECIFIC;
			_mgw_EventList->tlv->tag_value = 11;
			children[11] = _mgw_EventList;
			return _mgw_EventList;
		}
		case 12:
		{
			_rnc_TraceDepthExtension = new TraceDepthExtension();
			_rnc_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_rnc_TraceDepthExtension->tlv->tag_value = 12;
			children[12] = _rnc_TraceDepthExtension;
			return _rnc_TraceDepthExtension;
		}
		case 13:
		{
			_msc_s_TraceDepthExtension = new TraceDepthExtension();
			_msc_s_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_msc_s_TraceDepthExtension->tlv->tag_value = 13;
			children[13] = _msc_s_TraceDepthExtension;
			return _msc_s_TraceDepthExtension;
		}
		case 14:
		{
			_mgw_TraceDepthExtension = new TraceDepthExtension();
			_mgw_TraceDepthExtension->tlv->tag_class = CONTEXT_SPECIFIC;
			_mgw_TraceDepthExtension->tlv->tag_value = 14;
			children[14] = _mgw_TraceDepthExtension;
			return _mgw_TraceDepthExtension;
		}

		default: return NULL;
	}
}

void asn1::TracePropagationList::set_traceReference(){
	if(_traceReference == NULL) _traceReference = (TraceReference*)create_node(0);
}

void asn1::TracePropagationList::set_traceType(){
	if(_traceType == NULL) _traceType = (TraceType*)create_node(1);
}

void asn1::TracePropagationList::set_traceReference2(){
	if(_traceReference2 == NULL) _traceReference2 = (TraceReference2*)create_node(2);
}

void asn1::TracePropagationList::set_traceRecordingSessionReference(){
	if(_traceRecordingSessionReference == NULL) _traceRecordingSessionReference = (TraceRecordingSessionReference*)create_node(3);
}

void asn1::TracePropagationList::set_rnc_TraceDepth(){
	if(_rnc_TraceDepth == NULL) _rnc_TraceDepth = (TraceDepth*)create_node(4);
}

void asn1::TracePropagationList::set_rnc_InterfaceList(){
	if(_rnc_InterfaceList == NULL) _rnc_InterfaceList = (RNC_InterfaceList*)create_node(5);
}

void asn1::TracePropagationList::set_msc_s_TraceDepth(){
	if(_msc_s_TraceDepth == NULL) _msc_s_TraceDepth = (TraceDepth*)create_node(6);
}

void asn1::TracePropagationList::set_msc_s_InterfaceList(){
	if(_msc_s_InterfaceList == NULL) _msc_s_InterfaceList = (MSC_S_InterfaceList*)create_node(7);
}

void asn1::TracePropagationList::set_msc_s_EventList(){
	if(_msc_s_EventList == NULL) _msc_s_EventList = (MSC_S_EventList*)create_node(8);
}

void asn1::TracePropagationList::set_mgw_TraceDepth(){
	if(_mgw_TraceDepth == NULL) _mgw_TraceDepth = (TraceDepth*)create_node(9);
}

void asn1::TracePropagationList::set_mgw_InterfaceList(){
	if(_mgw_InterfaceList == NULL) _mgw_InterfaceList = (MGW_InterfaceList*)create_node(10);
}

void asn1::TracePropagationList::set_mgw_EventList(){
	if(_mgw_EventList == NULL) _mgw_EventList = (MGW_EventList*)create_node(11);
}

void asn1::TracePropagationList::set_rnc_TraceDepthExtension(){
	if(_rnc_TraceDepthExtension == NULL) _rnc_TraceDepthExtension = (TraceDepthExtension*)create_node(12);
}

void asn1::TracePropagationList::set_msc_s_TraceDepthExtension(){
	if(_msc_s_TraceDepthExtension == NULL) _msc_s_TraceDepthExtension = (TraceDepthExtension*)create_node(13);
}

void asn1::TracePropagationList::set_mgw_TraceDepthExtension(){
	if(_mgw_TraceDepthExtension == NULL) _mgw_TraceDepthExtension = (TraceDepthExtension*)create_node(14);
}

//ActivateTraceModeRes
asn1::ActivateTraceModeRes::ActivateTraceModeRes(){
	strcpy(node_type_name, "ActivateTraceModeRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// traceSupportIndicator
	_traceSupportIndicator = NULL;
	children.push_back(_traceSupportIndicator);


}
asn1::ActivateTraceModeRes::~ActivateTraceModeRes(){

}

asn1::ASN1Node* asn1::ActivateTraceModeRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[0] = _extensionContainer;
			return _extensionContainer;
		}
		case 1:
		{
			_traceSupportIndicator = new Null();
			_traceSupportIndicator->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceSupportIndicator->tlv->tag_value = 1;
			children[1] = _traceSupportIndicator;
			return _traceSupportIndicator;
		}

		default: return NULL;
	}
}

void asn1::ActivateTraceModeRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

void asn1::ActivateTraceModeRes::set_traceSupportIndicator(){
	if(_traceSupportIndicator == NULL) _traceSupportIndicator = (Null*)create_node(1);
}

//DeactivateTraceModeArg
asn1::DeactivateTraceModeArg::DeactivateTraceModeArg(){
	strcpy(node_type_name, "DeactivateTraceModeArg");
	// imsi
	_imsi = NULL;
	children.push_back(_imsi);

	// traceReference
	_traceReference = NULL;
	_traceReference = new TraceReference();
	_traceReference->tlv->tag_class = CONTEXT_SPECIFIC;
	_traceReference->tlv->tag_value = 1;
	children.push_back(_traceReference);

	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);

	// traceReference2
	_traceReference2 = NULL;
	children.push_back(_traceReference2);


}
asn1::DeactivateTraceModeArg::~DeactivateTraceModeArg(){

}

asn1::ASN1Node* asn1::DeactivateTraceModeArg::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_imsi = new IMSI();
			_imsi->tlv->tag_class = CONTEXT_SPECIFIC;
			_imsi->tlv->tag_value = 0;
			children[0] = _imsi;
			return _imsi;
		}
		case 2:
		{
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 2;
			children[2] = _extensionContainer;
			return _extensionContainer;
		}
		case 3:
		{
			_traceReference2 = new TraceReference2();
			_traceReference2->tlv->tag_class = CONTEXT_SPECIFIC;
			_traceReference2->tlv->tag_value = 3;
			children[3] = _traceReference2;
			return _traceReference2;
		}

		default: return NULL;
	}
}

void asn1::DeactivateTraceModeArg::set_imsi(){
	if(_imsi == NULL) _imsi = (IMSI*)create_node(0);
}

void asn1::DeactivateTraceModeArg::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(2);
}

void asn1::DeactivateTraceModeArg::set_traceReference2(){
	if(_traceReference2 == NULL) _traceReference2 = (TraceReference2*)create_node(3);
}

//DeactivateTraceModeRes
asn1::DeactivateTraceModeRes::DeactivateTraceModeRes(){
	strcpy(node_type_name, "DeactivateTraceModeRes");
	// extensionContainer
	_extensionContainer = NULL;
	children.push_back(_extensionContainer);


}
asn1::DeactivateTraceModeRes::~DeactivateTraceModeRes(){

}

asn1::ASN1Node* asn1::DeactivateTraceModeRes::create_node(unsigned int _index){
	switch(_index){
		case 0:
		{
			_extensionContainer = new ExtensionContainer();
			_extensionContainer->tlv->tag_class = CONTEXT_SPECIFIC;
			_extensionContainer->tlv->tag_value = 0;
			children[0] = _extensionContainer;
			return _extensionContainer;
		}

		default: return NULL;
	}
}

void asn1::DeactivateTraceModeRes::set_extensionContainer(){
	if(_extensionContainer == NULL) _extensionContainer = (ExtensionContainer*)create_node(0);
}

