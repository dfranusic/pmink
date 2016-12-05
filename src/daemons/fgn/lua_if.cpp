/*
 * This file is part of Project MINK <http://www.release14.org>.
 *
 * Copyright (C) 2012 Release14 Ltd.
 * http://www.release14.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <lua_if.h>
#include <unicode/utf8.h>
#include <unicode/unistr.h>
#include <unicode/stringpiece.h>
#include <unicode/ustream.h>
#include <unicode/ucnv.h>
#include <boost/regex/icu.hpp>

// *** utf-8 toupper ***
int pmink_lua_utf8_upper(void* pm, const char* data, char* out){
    if(pm == NULL || data == NULL || out == NULL) return -1;
    // data l
    int data_l = strlen(data);
    // utf8 string
    icu::UnicodeString utf8_str(data, data_l, "utf-8");
    // icu error
    UErrorCode icu_err = U_ZERO_ERROR;
    // utf8 converter
    UConverter* conv_utf8 = ucnv_open("utf-8", &icu_err);
    if(conv_utf8 == NULL) return -2;
    // to upper
    utf8_str.toUpper();
    // extract to out buff
    int tmp_size = utf8_str.extract(out, data_l * 2, conv_utf8, icu_err);
    // close converter
    ucnv_close(conv_utf8);
    // err check
    if(icu_err != U_ZERO_ERROR) return -3;
    // ok, return number of bytes (no NULL termination)
    return tmp_size;

}

// *** utf-8 tolower ***
int pmink_lua_utf8_lower(void* pm, const char* data, char* out){
    if(pm == NULL || data == NULL || out == NULL) return -1;
    // data l
    int data_l = strlen(data);
    // utf8 string
    icu::UnicodeString utf8_str(data, data_l, "utf-8");
    // icu error
    UErrorCode icu_err = U_ZERO_ERROR;
    // utf8 converter
    UConverter* conv_utf8 = ucnv_open("utf-8", &icu_err);
    if(conv_utf8 == NULL) return -2;
    // to upper
    utf8_str.toLower();
    // extract to out buff
    int tmp_size = utf8_str.extract(out, data_l * 2, conv_utf8, icu_err);
    // close converter
    ucnv_close(conv_utf8);
    // err check
    if(icu_err != U_ZERO_ERROR) return -3;
    // ok, return number of bytes (no NULL termination)
    return tmp_size;

}
// *** utf-8 regex_match ***
bool pmink_lua_utf8_regex_match(void* pm, const char* data, const char* regex){
    if(pm == NULL || data == NULL || regex == NULL) return false;
    // create regex
    boost::u32regex r = boost::make_u32regex(regex);
    boost::cmatch what;
    return boost::u32regex_match(data, what, r);
}


// *** pd request ***
void* pmink_lua_pd_req_vp_new(void* pm, int var){
    if(pm == NULL) return NULL;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    switch(var){
	// DPT_OCTETS types
	case asn1::ParameterType::_pt_pmink_filter_data:
	{
	    unsigned char tmp_oct = 0;
	    return pld->params.set_octets(var, &tmp_oct, 1, fgn::PIT_INPUT_CMD_REQ_PART);
	}
	// DPT_INT types
	case asn1::ParameterType::_pt_pmink_filter_list_id:
	case asn1::ParameterType::_pt_pmink_filter_data_size:
	    return pld->params.set_int(var, 0, fgn::PIT_INPUT_CMD_REQ_PART);

	// unknown/unsupported
	default: return NULL;
    }
    // err
    return NULL;
}

int pmink_lua_pd_req(void* pm, int action){
    if(pm == NULL) return 1;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    fgn::rule_param_t* params = &pld->params;
    // use cmd params
    fgn::rule_param_t* cmd_params = &pld->cmd_params;
    cmd_params->clear_params();

    // filter data
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_filter_data, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_filter_data, 2));

    // filter list id
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_filter_list_id, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_filter_list_id, 2));


    // set pmink command id
    cmd_params->set_int(asn1::ParameterType::_pt_pmink_command_id, action);
    // request notification
    cmd_params->set_bool(asn1::ParameterType::_pt_pmink_correlation_notification, true);
    // set guid for fgn payload (_pt_pmink_guid index 0)
    cmd_params->set_octets(asn1::ParameterType::_pt_pmink_guid, (unsigned char*)&pld->guid, sizeof(pld->guid));
    // set correlation guid for current pd transaction (_pt_pmink_guid index 1)
    __uint128_t tmp_guid = 0;
    dd->generate_uuid((unsigned char*)&tmp_guid);
    cmd_params->set_octets(asn1::ParameterType::_pt_pmink_guid, (unsigned char*)&tmp_guid, sizeof(tmp_guid), 1);
    pld->params.set_octets(asn1::ParameterType::_pt_pmink_guid, (unsigned char*)&tmp_guid, sizeof(tmp_guid), fgn::PIT_INPUT_CMD_REQ_PART);
    // set service id for notification
    cmd_params->set_int(asn1::ParameterType::_pt_pmink_service_id, asn1::ServiceId::_sid_fgn_filtering);
    // send to pd
    dd->send_pd(cmd_params);

    // *** add to correlation map ***
    // lock cmap
    dd->cmap.lock();
    // inc refc
    pld->refc.fetch_add(1);
    // update phase
    pld->phase = fgn::CPT_PD_REQ;
    // add to cmap if not already there
    dd->cmap.set(pld->guid, pld);
    // unlock cmap
    dd->cmap.unlock();

    // *** add to r14p correlation map ***
    // lock cmap
    dd->cmap_r14p.lock();
    // add to cmap
    dd->cmap_r14p.set(tmp_guid, pld->guid);
    // unlock cmap
    dd->cmap_r14p.unlock();
    // ok
    return 0;
}

// *** hlr request ***
void* pmink_lua_hlr_req_vp_new(void* pm, int var){
    if(pm == NULL) return NULL;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    switch(var){
	// DPT_INT types
	case asn1::ParameterType::_pt_gsmmap_version:
	case asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number:
	case asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan:
	case asn1::ParameterType::_pt_gsmmap_sca_type_of_number:
	case asn1::ParameterType::_pt_sccp_called_pa_subsystem_number:
	case asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type:
	case asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan:
	case asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address:
	case asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number:
	case asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type:
	case asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan:
	case asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address:
	    return pld->params.set_int(var, 0, fgn::PIT_INPUT_CMD_REQ_PART);

	// DPT_STRING types
	case asn1::ParameterType::_pt_gsmmap_msisdn_digits:
	case asn1::ParameterType::_pt_gsmmap_sca_digits:
	case asn1::ParameterType::_pt_sccp_called_pa_gt_address:
	case asn1::ParameterType::_pt_sccp_calling_pa_gt_address:
	    return pld->params.set_cstr(var, "0", fgn::PIT_INPUT_CMD_REQ_PART);

	// unknown/unsupported
	default: return NULL;
    }
    // err
    return NULL;

}


int pmink_lua_hlr_req(void* pm){
    if(pm == NULL) return 1;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    fgn::rule_param_t* params = &pld->params;
    // use cmd params
    fgn::rule_param_t* cmd_params = &pld->cmd_params;
    cmd_params->clear_params();
    // gsm map version
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_version, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_version, 2));

    // msisdn nai
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, 2));

    // msisdn np
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, 2));

    // msisdn address
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_digits, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_digits, 2));

    // sca nai
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, 2));

    // sca np
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, 2));

    // sca address
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_digits, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_digits, 2));


    // called ssn
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, 2));

    // called gt tt
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, 2));

    // called gt np
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, 2));

    // called gt nai
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, 2));

    // called gt address
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_address, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_address, 2));

    // calling ssn
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, 2));

    // calling gt tt
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, 2));

    // calling gt np
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, 2));

    // calling gt nai
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, 2));

    // calling gt address
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, fgn::PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, 2));

    // remove result part (from previous invokation)
    params->erase_param(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_an_type_of_number, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_an_digits, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_imsi, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, fgn::PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, fgn::PIT_INPUT_CMD_RPL_PART);


    // set pmink command id
    cmd_params->set_int(asn1::ParameterType::_pt_pmink_command_id, asn1::SmsHubCommandId::_shci_sri_sm_req);
    // request notification
    cmd_params->set_bool(asn1::ParameterType::_pt_pmink_correlation_notification, true);
    // set guid
    cmd_params->set_octets(asn1::ParameterType::_pt_pmink_guid, (unsigned char*)&pld->guid, sizeof(pld->guid));
    // set service id for notification
    cmd_params->set_int(asn1::ParameterType::_pt_pmink_service_id, asn1::ServiceId::_sid_fgn_filtering);
    // send to stp
    dd->send_stp(cmd_params);
    // *** add to correlation map ***
    // lock cmap
    dd->cmap.lock();
    // inc refc
    pld->refc.fetch_add(1);
    // update phase
    pld->phase = fgn::CPT_SRI_SM_REQ;
    // add to cmap if not already there
    dd->cmap.set(pld->guid, pld);
    // unlock cmap
    dd->cmap.unlock();
    // ok
    return 0;
}


// *** flood lists ***
// delete flood list or item in flood list
bool pmink_lua_fl_del(void* pm, const char* list, const char* item){
    if(pm == NULL || list == NULL) return false;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::flood_m_t& lm = *pld->rproc->flood_m;
    // only delete item
    if(item != NULL){
	lm.del_item(list, item);
	// *** main master list ***
	fgn::flood_mstr_t* list_mstr_p = pld->rproc->flood_m.get_master();
	fgn::flood_m_t* mstr_list_m_p = &(**list_mstr_p);
	// update master
	list_mstr_p->update_start();
	mstr_list_m_p->del_item(list, item);
	list_mstr_p->update_end();
    }
    // delete list
    else{
	lm.remove_list(list);
	// *** main master list ***
	fgn::flood_mstr_t* list_mstr_p = pld->rproc->flood_m.get_master();
	fgn::flood_m_t* mstr_list_m_p = &(**list_mstr_p);
	// update master
	list_mstr_p->update_start();
	mstr_list_m_p->remove_list(list);
	list_mstr_p->update_end();

    }
    // ok
    return true;
}

// new flood list
bool pmink_lua_fl_new(void* pm, const char* label){
    if(pm == NULL || label == NULL) return false;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::flood_m_t& lm = *pld->rproc->flood_m;
    // new list
    lm.add_list(label);
    // *** main master list ***
    fgn::flood_mstr_t* list_mstr_p = pld->rproc->flood_m.get_master();
    fgn::flood_m_t* mstr_list_m_p = &(**list_mstr_p);
    // update master
    list_mstr_p->update_start();
    mstr_list_m_p->add_list(label);
    list_mstr_p->update_end();
    // ok
    return true;
}

// get item count in flood list for specified interval
int pmink_lua_fl_get(void* pm, const char* list, int interval, const char* item){
    if(pm == NULL || list == NULL || item == NULL) return false;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::flood_m_t& lm = *pld->rproc->flood_m;
    // get count
    return lm.get_item_count(list, item, interval);
}
// add item to flood list
bool pmink_lua_fl_add(void* pm, const char* list, const char* item){
    if(pm == NULL || list == NULL || item == NULL) return false;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::flood_m_t& lm = *pld->rproc->flood_m;
    fgn::flood_m_t::list_it_t list_it = lm.add_list(list);
    bool res = (lm.add_item(list_it, item) != NULL);
    // expire
    lm.expire(list_it->second);
    // *** main master list ***
    fgn::flood_mstr_t* list_mstr_p = pld->rproc->flood_m.get_master();
    fgn::flood_m_t* mstr_list_m_p = &(**list_mstr_p);
    // update master
    list_mstr_p->update_start();
    list_it = mstr_list_m_p->add_list(list);
    mstr_list_m_p->add_item(list_it, item);
    mstr_list_m_p->expire(list_it->second);
    list_mstr_p->update_end();
    // ok
    return res;
}

// ** standard lists ***
// new standard list
bool pmink_lua_sl_new(void* pm, const char* label){
    if(pm == NULL || label == NULL) return false;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::list_m_t& lm = *pld->rproc->list_m;
    lm.add_list(label);
    // *** main master list ***
    fgn::list_mstr_t* list_mstr_p = pld->rproc->list_m.get_master();
    fgn::list_m_t* mstr_list_m_p = &(**list_mstr_p);
    // update master
    list_mstr_p->update_start();
    mstr_list_m_p->add_list(label);
    list_mstr_p->update_end();
    // ok
    return true;
}

// delete standard list or item in list
bool pmink_lua_sl_del(void* pm, const char* list, const char* item){
    if(pm == NULL || list == NULL) return false;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::list_m_t& lm = *pld->rproc->list_m;
    // only delete item
    if(item != NULL){
	lm.del_sublist_item(list, item);
	// *** main master list ***
	fgn::list_mstr_t* list_mstr_p = pld->rproc->list_m.get_master();
	fgn::list_m_t* mstr_list_m_p = &(**list_mstr_p);
	// update master
	list_mstr_p->update_start();
	mstr_list_m_p->del_sublist_item(list, item);
	list_mstr_p->update_end();

    }
    // delete list
    else{
	lm.remove_list(list);
	// *** main master list ***
	fgn::list_mstr_t* list_mstr_p = pld->rproc->list_m.get_master();
	fgn::list_m_t* mstr_list_m_p = &(**list_mstr_p);
	// update master
	list_mstr_p->update_start();
	mstr_list_m_p->remove_list(list);
	list_mstr_p->update_end();
    }

    // ok
    return true;
}

// add new item to standard list
bool pmink_lua_sl_add(void* pm, const char* list, const char* item){
    if(pm == NULL || list == NULL || item == NULL) return false;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::list_m_t& lm = *pld->rproc->list_m;
    fgn::list_m_t::list_it_t list_it = lm.add_list(list);
    bool res = (lm.add_sublist_item(list_it, item) != NULL);
    // *** main master list ***
    fgn::list_mstr_t* list_mstr_p = pld->rproc->list_m.get_master();
    fgn::list_m_t* mstr_list_m_p = &(**list_mstr_p);
    // update master
    list_mstr_p->update_start();
    list_it = mstr_list_m_p->add_list(list);
    mstr_list_m_p->add_sublist_item(list_it, item);
    list_mstr_p->update_end();
    // ok
    return res;
}

// checkk if item exists in standard list
bool pmink_lua_sl_get(void* pm, const char* list, const char* item){
    if(pm == NULL || list == NULL || item == NULL) return false;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::list_m_t& lm = *pld->rproc->list_m;
    return (lm.get_sublist_item(list, item) != NULL);
}

// get list size
int pmink_lua_sl_get_size(void* pm, const char* list){
    if(pm == NULL || list == NULL) return 0;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::list_m_t& lm = *pld->rproc->list_m;
    return lm.get_sublist_size(list);
}

// get list item
const char* pmink_lua_sl_get_item(void* pm, const char* list, int index){
    if(pm == NULL || list == NULL) return NULL;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    fgn::list_m_t& lm = *pld->rproc->list_m;
    std::string* res = lm.get_sublist_item_by_index(list, index);
    if(res == NULL) return NULL;
    return res->c_str();
}


// *** VariantParam ***
// new VariantParam
void* pmink_lua_vp_new(void* pm, int var, int index){
    if(pm == NULL) return NULL;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    switch(var){
	// DPT_INT types
	case asn1::ParameterType::_pt_tcap_error_code:
	case asn1::ParameterType::_pt_smpp_command_status:
	    return pld->params.set_int(var, 0, index);

	// unknown/unsupported
	default: return NULL;
    }
    // err
    return NULL;
}


// get VariantParam pointer
void* pmink_lua_vp_get(void* pm, int var, int index){
    if(pm == NULL) return NULL;
    fgn::FgnPayload* pld = (fgn::FgnPayload*)pm;
    // current var
    if(var == -1) var = pld->current_vp_id.key;
    // current index
    // -1 = current index
    if(index == -1) index = pld->current_vp_id.index;
    // other special negative values, convert to positive
    else if(index < -1) index *= -1;
    // get param
    return pld->params.get_param(var, index);
}

// VariantParam to c string
char* pmink_lua_vp_to_str(void* pm, void* vp){
    if(vp == NULL) return NULL;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    return (char*)*vpp;
}

// VariantParam to bool
bool pmink_lua_vp_to_bool(void* pm, void* vp){
    if(vp == NULL) return false;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    return (bool)*vpp;
}

// VariantParam to c int
int pmink_lua_vp_to_int(void* pm, void* vp){
    if(vp == NULL) return 0;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    return (int)*vpp;
}

// VariantParam to c double
double pmink_lua_vp_to_double(void* pm, void* vp){
    if(vp == NULL) return 0;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    return (double)*vpp;
}

// VariantParam to c char
char pmink_lua_vp_to_char(void* pm, void* vp){
    if(vp == NULL) return 0;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    return (char)*vpp;
}

// VariantParam to byte array
unsigned char* pmink_lua_vp_to_octets(void* pm, void* vp){
    if(vp == NULL) return NULL;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    return (unsigned char*)*vpp;
}

// VariantParam to pointer
void* pmink_lua_vp_to_pointer(void* pm, void* vp){
    if(vp == NULL) return NULL;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    return (void*)*vpp;
}

// VariantParam get type
int pmink_lua_vp_get_type(void* pm, void* vp){
    if(vp == NULL) return 0;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    return vpp->get_type();
}

// VariantParam get size in bytes
int pmink_lua_vp_get_size(void* pm, void* vp){
    if(vp == NULL) return 0;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    return vpp->get_size();
}

// VariantParam set C string
bool pmink_lua_vp_set_str(void* pm, void* vp, const char* val){
    if(vp == NULL) return false;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    vpp->set_str(val);
    return true;
}

// VariantParam set int
bool pmink_lua_vp_set_int(void* pm, void* vp, int val){
    if(vp == NULL) return false;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    vpp->set_int(val);
    return true;
}

// VariantParam set bool
bool pmink_lua_vp_set_bool(void* pm, void* vp, bool val){
    if(vp == NULL) return false;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    vpp->set_bool(val);
    return true;
}

// VariantParam set double
bool pmink_lua_vp_set_double(void* pm, void* vp, double val){
    if(vp == NULL) return false;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    vpp->set_double(val);
    return true;
}

// VariantParam set char
bool pmink_lua_vp_set_char(void* pm, void* vp, char val){
    if(vp == NULL) return false;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    vpp->set_char(val);
    return true;
}

// VariantParam set octets
bool pmink_lua_vp_set_octets(void* pm, void* vp, unsigned char* val, int size){
    if(vp == NULL) return false;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    vpp->set_octets(val, size);
    return true;
}

// VariantParam set pointer
bool pmink_lua_vp_set_pointer(void* pm, void* vp, void* val){
    if(vp == NULL) return false;
    pmink_utils::VariantParam* vpp = (pmink_utils::VariantParam*)vp;
    vpp->set_pointer(val);
    return true;
}



