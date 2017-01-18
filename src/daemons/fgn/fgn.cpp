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

#include <fgn.h>
#include <getopt.h>
#include <fgn_stats.h>
#include <sccp.h>
#include <m3ua.h>
#include <smstpdu.h>
#include <GSMMAP.h>
#include <smpp.h>
#include <connection.h>
#include <boost/algorithm/string.hpp>
#include "lua.hpp"
#include <lua_if.h>
#include <connection.h>
#include <dlfcn.h>

// FWorker
fgn::FWorker::FWorker(int _id, int q_size, FilterManager* _fm): id(_id), fm(_fm){
    queue.set_capacity(q_size);
    //L = luaL_newstate();
}

fgn::FWorker::FWorker(): id(0), fm(NULL){
    //L = luaL_newstate();
}

fgn::FWorker::~FWorker(){
    //lua_close(L);
}


// FilterManager
fgn::FilterManager::FilterManager(): rgx("^(?:\\[(\\d+|\\+|<-|->)])?(?:(?:(!=|==|<|<=|>|>=)?(\\d+))|(?::(.+))|(?:(!=|==|<|<=|>|>=)?(?:@(?:\\[(\\d+|\\+|<-|->)])?([A-Za-z].*)))|(?:(!=|==|>>|<<|--)?(?:@\\{([A-Za-z].*)\\}))|(?:(!=|==|<=|>=|<|>)?(.+)))"){

}

fgn::FilterManager::~FilterManager(){

}

int fgn::FilterManager::build_lists(config::ConfigItem* cfg_root, list_m_t* lm){
    // sanity check
    if(cfg_root == NULL || cfg_root->name != "lists") return 1;
    // clear lists
    lm->clear();
    // tmp vars
    config::ConfigItem* tmp_node = NULL;
    // loop lists
    for(unsigned int i = 0; i<cfg_root->children.size(); i++){
	// config list node pointer
	tmp_node = cfg_root->children[i];
	// skip deleted
	if(tmp_node->node_state == config::CONFIG_NS_DELETED) continue;
	// build rule
	build_list(tmp_node, lm);
    }

    // ok
    return 0;
}


int fgn::FilterManager::build_list(config::ConfigItem* list_root, list_m_t* lm){
    // sanity check
    if(list_root == NULL) return 1;
    // tmp vars
    config::ConfigItem* tmp_node = NULL;
    config::ConfigItem* values_node = NULL;
    // add list
    lm->add_list(list_root->name);
    // values node
    values_node = (*list_root)("values");
    if(values_node == NULL) return 1;
    // loop list items
    for(unsigned int i = 0; i<values_node->children.size(); i++){
	// config list item node pointer
	tmp_node = values_node->children[i];
	// skip deleted
	if(tmp_node->node_state == config::CONFIG_NS_DELETED) continue;
	// add item
	lm->add_sublist_item(list_root->name, tmp_node->name);
    }

    return 0;
}

int fgn::FilterManager::build_rules(config::ConfigItem* cfg_root, rule_m_t* rm){
    // sanity check
    if(cfg_root == NULL || cfg_root->name != "rules") return 1;

    // get daemon
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    // get stats manager
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)dd->get_param(3);
    // free rule stats handlers
    for(rule_m_t::rules_map_it_t it = rm->get_begin(); it != rm->get_end(); it++){
	// rule ref
	rule_t& rule_ref = it->second;
	// get rule param data
	rule_param_t* rule_data = rule_ref.get_data_p();
	// rule label
	std::string rule_lbl(rule_ref.get_label());
	// remove stats handler
	delete r14p_stats->remove_trap(r14p::TrapId(std::string(rule_lbl + "_RULE_COUNTER")));
	// get stats manager
	fgn::StatsManagerRefc* stats_m = (fgn::StatsManagerRefc*)(void*)*rule_data->get_param(2, 0, 0, RCT_PMINK);
	// free stats manager (delayed deallocation)
	stats_m->refc_dec();

	// loop rule param data
	for(rule_param_t::it_t r_it = rule_data->get_begin(); r_it != rule_data->get_end(); r_it++){
	    // check for index 0 - match part
	    if(r_it->first.index == PIT_RULE_MATCH_PART){
		// check for fragment 1 - match line type
		if(r_it->first.fragment != 1) continue;
		// check for lua type
		if((int)r_it->second == OTT_LUA){
		    // get lua script string pointer (fragment 2)
		    pmink_utils::VariantParam* vp_lua_script = rule_data->get_param(r_it->first.key, r_it->first.index, 2, r_it->first.context);
		    // sanity check
		    if(vp_lua_script == NULL) continue;
		    // free
		    delete (std::string*)(void*)*vp_lua_script;
		}
	    }
	}
    }


    // clear rules
    rm->clear();
    // tmp vars
    config::ConfigItem* tmp_node = NULL;
    // loop rules
    for(unsigned int i = 0; i<cfg_root->children.size(); i++){
	// config rule node pointer
	tmp_node = cfg_root->children[i];
	// skip deleted
	if(tmp_node->node_state == config::CONFIG_NS_DELETED) continue;
	// build rule
	build_rule(tmp_node, rm);
    }

    // ok
    return 0;
}

#ifndef FGN_CPP14
bool fgn::FilterManager::field_valid(const char* _const, bool const_only){
    // sanity check
    if(_const == NULL) return false;
    // min length
    if(strlen(_const) < 1) return false;

    // tokenize
    boost::cmatch what;
    try{
	if(!boost::regex_match(_const, what, rgx)) return 1;

	// SOTT_NUMBER
	if(what[3].str().size() > 0){
	    return true;

	// SOTT_REGEX
	}else if(what[4].str().size() > 0){
	    return true;

	// SOTT_VARIABLE
	}else if(what[7].str().size() > 0){
	    // find cfg mapping
	    uint32_t hash = pmink_utils::hash_fnv1a(what[7].str().c_str(), what[7].str().size());
	    std::map<uint32_t, CfgTranslationItem*>::iterator it = flogic.cfg_trans_map.find(hash);
	    // check if CONST type or CONST/VAR type found
	    if(it != flogic.cfg_trans_map.end() && (const_only ? it->second->type == CTMT_CONST : true)) return true;
	    return false;

	// SOTT_LIST
	}else if(what[9].str().size() > 0){
	    return true;


	// SOTT_STRING
	}else if(what[11].str().size() > 0){
	    return true;

	// unknown/error
	}else return false;

    // invalid regex pattern
    }catch(std::exception& e){
	return false;
    }

    // err
    return false;

}
#else
bool fgn::FilterManager::field_valid(const char* _const, bool const_only){
    // sanity check
    if(_const == NULL) return false;
    // min length
    if(strlen(_const) < 1) return false;
    // create string and iterators
    std::string test_str(_const);
    std::string::const_iterator iter = test_str.begin();
    std::string::const_iterator end = test_str.end();
    // output map 
    std::map<int, std::string> out_map;
    std::map<int, std::string>::iterator out_map_it;;
    bool r = false;
    try{
        r = x3::phrase_parse(iter, end, 
                             basic_matching::parser::statement, 
                             x3::ascii::space, 
                             out_map);

    }catch(x3::expectation_failure<std::string::const_iterator>& e){
        return false;
    }

    // check if fully parsed
    if(r && iter == end) {
        // type sanity check
        out_map_it = out_map.find(0);
        if(out_map_it == out_map.end()) return false;

        // check for VARIABLE type
        if(std::stoi(out_map_it->second) == OTT_VARIABLE){
	    // find cfg mapping
	    uint32_t hash = pmink_utils::hash_fnv1a(out_map[4].c_str(), out_map[4].size());
	    std::map<uint32_t, CfgTranslationItem*>::iterator it = flogic.cfg_trans_map.find(hash);
	    // check if CONST type or CONST/VAR type found
	    if(it != flogic.cfg_trans_map.end() && (const_only ? it->second->type == CTMT_CONST : true)) return true;
	    return false;
        }

        // ok for all other types
        return true;
    }

    // err
    return false;
}
#endif

int fgn::FilterManager::str_to_ciop(const char* _str){
    if(strcmp(_str, "+") == 0) return PIT_INPUT_TRANSLATE_PART;
    else if(strcmp(_str, "->") == 0) return PIT_INPUT_CMD_REQ_PART;
    else if(strcmp(_str, "<-") == 0) return PIT_INPUT_CMD_RPL_PART;
    else return atoi(_str);
}

#ifndef FGN_CPP14
/*

[1]!=@[2]tcap.const.ct_invoke

1 (left index) 			= 1
2 (operator for int item)	=
3 (int item)			=
4 (regex)			=
5 (operator for const/var item)	= !=
6 (right index)			= 2
7 (const or var item)		= tcap.const.ct_invoke
8 (operator for list item)	=
9 (list item)			=
10 (operator for text item)	=
11(text item)			=

 */
int fgn::FilterManager::prepare_match_line(rule_param_t* rdata, const char* line, uint32_t vp_id, uint32_t context, config::ConfigItem* args){
    // sanity check
    if(rdata == NULL || line == NULL) return 1;
    boost::cmatch what;
    // tokenize
    try{
	if(!boost::regex_match(line, what, rgx)) return 1;
	// set line
	rdata->set_cstr(vp_id, line, PIT_RULE_MATCH_PART, 0, context);
	// check operand type

	// SOTT_NUMBER
	if(what[3].str().size() > 0){
	    // set type
	    rdata->set_int(vp_id, OTT_NUMBER, PIT_RULE_MATCH_PART, 1, context);
	    // set left component index operator
	    rdata->set_int(vp_id, str_to_ciop(what[1].str().c_str()), PIT_RULE_MATCH_PART, 2, context);
	    // set operator
	    rdata->set_int(vp_id, flogic.op_detect(what[2].str().c_str()), PIT_RULE_MATCH_PART, 3, context);
	    // set operand
	    rdata->set_cstr(vp_id, what[3].str().c_str(), PIT_RULE_MATCH_PART, 4, context);


	// SOTT_REGEX
	}else if(what[4].str().size() > 0){
	    // set type
	    rdata->set_int(vp_id, OTT_REGEX, PIT_RULE_MATCH_PART, 1, context);
	    // set left component index operator
	    rdata->set_int(vp_id, str_to_ciop(what[1].str().c_str()), PIT_RULE_MATCH_PART, 2, context);
	    // set operand
	    rdata->set_cstr(vp_id, what[4].str().c_str(), PIT_RULE_MATCH_PART, 3, context);

	// SOTT_VARIABLE
	}else if(what[7].str().size() > 0){
	    // set type
	    rdata->set_int(vp_id, OTT_VARIABLE, PIT_RULE_MATCH_PART, 1, context);
	    // set left component index operator
	    rdata->set_int(vp_id, str_to_ciop(what[1].str().c_str()), PIT_RULE_MATCH_PART, 2, context);
	    // set operator
	    rdata->set_int(vp_id, flogic.op_detect(what[5].str().c_str()), PIT_RULE_MATCH_PART, 3, context);
	    // set right component index operator
	    rdata->set_int(vp_id, str_to_ciop(what[6].str().c_str()), PIT_RULE_MATCH_PART, 4, context);
	    // set operand
	    rdata->set_cstr(vp_id, what[7].str().c_str(), PIT_RULE_MATCH_PART, 5, context);

	// SOTT_LIST
	}else if(what[9].str().size() > 0){
	    // set type
	    rdata->set_int(vp_id, OTT_LIST, PIT_RULE_MATCH_PART, 1, context);
	    // set left component index operator
	    rdata->set_int(vp_id, str_to_ciop(what[1].str().c_str()), PIT_RULE_MATCH_PART, 2, context);
	    // set operator
	    rdata->set_int(vp_id, flogic.op_detect(what[8].str().c_str()), PIT_RULE_MATCH_PART, 3, context);
	    // set operand
	    rdata->set_cstr(vp_id, what[9].str().c_str(), PIT_RULE_MATCH_PART, 4, context);


	// SOTT_STRING
	}else if(what[11].str().size() > 0){
	    // if starting with `` use LUA scripting
	    if(what[11].str().size() > 2 && boost::starts_with(what[11].str(), "``")){
                // inline/external flag
                bool ext = false;
		// tmp string
		std::string tmp_str;
		// if next character is '/', load script from external file
		if(boost::starts_with(what[11].str(), "``/")){
		    // file path
		    std::string tmp_file_str(what[11].str(), 2);
		    // file size
		    int fsize = pmink_utils::get_file_size(tmp_file_str.c_str());
		    // check if file size is valid
		    if(fsize > 0){
			// tmp buffer
			char tmp_buff[fsize];
			// try to load file
			if(pmink_utils::load_file(tmp_file_str.c_str(), tmp_buff, &fsize) != 0){
			    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot load external Lua script [%s]", tmp_file_str.c_str());
			    return 1;
			}
			// set lua script  string
			tmp_str.assign(tmp_buff, fsize);
                        // set external flag
                        ext = true;

		    // file size error
		    }else{
			pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot load external Lua script [%s]", tmp_file_str.c_str());
			return 1;
		    }

		// load inline script string and prepend it with pmink module
		}else{
		    tmp_str = what[11].str().substr(2);
		    tmp_str.insert(0, PMINK_LUA_MODULE);
		}

		// create lua state
		lua_State* L = luaL_newstate();
		// init lua
		luaL_openlibs(L);
		// load string
		if(luaL_loadstring(L, tmp_str.c_str()) == 0){
		    // set type
		    rdata->set_int(vp_id, OTT_LUA, PIT_RULE_MATCH_PART, 1, context);
		    // set lua script (dynamic string pointer due to possible large script file)
		    std::string* tmp_lua_script_str = new std::string(tmp_str);
		    //rdata->set_cstr(vp_id, tmp_str.c_str(), PIT_RULE_MATCH_PART, 2, context);
		    rdata->set_pointer(vp_id, tmp_lua_script_str, PIT_RULE_MATCH_PART, 2, context);
                    // arguments for external lua script
                    if(ext && args != NULL){
                        // argument count
                        int argc = 0;
                        // loop args
                        for(unsigned int i = 0; i<args->children.size(); i++){
                            // arg name and value strings
                            const char* tmp_arg_n = args->children[i]->name.c_str();
                            const char* tmp_arg_v = args->children[i]->to_cstr("value");
                            // sanity check
                            if(tmp_arg_v == NULL || strlen(tmp_arg_v) <= 0) continue;
                            // arguments start with fragment index 100 
                            // =======================================
                            // argument 0: fragment 100 = argument name, fragment 101 = argument value
                            // argument 1: fragment 102 = argument name, fragment 103 = argument value
                            // =======================================
                            // two fragments exist for every argument(name and value)
                            // even fragments = argument names
                            // odd fragments  = argument values
		            rdata->set_cstr(vp_id, tmp_arg_n, PIT_RULE_MATCH_PART, 100 + (i * 2), context);
		            rdata->set_cstr(vp_id, tmp_arg_v, PIT_RULE_MATCH_PART, 100 + (i * 2) + 1, context);
                            // inc argc
                            ++argc;
                        }
                        // set argument count
                        rdata->set_int(vp_id, argc, PIT_RULE_MATCH_PART, 4, context);
                    }

		// error
		}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Lua error: %s", lua_tostring(L, -1));

		// remove state (created in slave rule processors)
		lua_close(L);

	    // normal string matching
	    }else{
		// set type
		rdata->set_int(vp_id, OTT_STRING, PIT_RULE_MATCH_PART, 1, context);
		// set left component index operator
		rdata->set_int(vp_id, str_to_ciop(what[1].str().c_str()), PIT_RULE_MATCH_PART, 2, context);
		// set operator
		rdata->set_int(vp_id, flogic.op_detect(what[10].str().c_str()), PIT_RULE_MATCH_PART, 3, context);
		// trim operand
		std::string tmp_str(what[11].str());
		boost::trim_if(tmp_str, boost::is_any_of("'"));
		// set operand
		rdata->set_cstr(vp_id, tmp_str.c_str(), PIT_RULE_MATCH_PART, 4, context);
	    }


	// unknown/error
	}else return 1;

    // invalid regex pattern
    }catch(std::exception& e){
	return 1;
    }

    // ok
    return 0;
}
#else
/*
0 = operand B type
1 = left index LCI
2 = operator
3 = right index RCI
4 = operand B
 */
int fgn::FilterManager::prepare_match_line(rule_param_t* rdata, const char* line, uint32_t vp_id, uint32_t context, config::ConfigItem* args){
    // sanity check
    if(rdata == NULL || line == NULL) return 1;
    // basic matching map
    std::map<int, std::string> bm_map;
    std::map<int, std::string>::iterator bm_map_it;
    // create string and iterators
    std::string test_str(line);
    std::string::const_iterator iter = test_str.begin();
    std::string::const_iterator end = test_str.end();
    bool r = false;
    try{
        r = x3::phrase_parse(iter, end, 
                             basic_matching::parser::statement, 
                             x3::ascii::space, 
                             bm_map);

    }catch(x3::expectation_failure<std::string::const_iterator>& e){
        return 1;
    }

    // check if fully parsed
    if(!(r && iter == end)) return 1;

    // type sanity check
    bm_map_it = bm_map.find(0);
    if(bm_map_it == bm_map.end()) return 1;

    // set line
    rdata->set_cstr(vp_id, line, PIT_RULE_MATCH_PART, 0, context);

    // check operand B type
    switch(std::stoi(bm_map_it->second)){
        // NUMBER
        case OTT_NUMBER:
        {
            // set type
            rdata->set_int(vp_id, OTT_NUMBER, PIT_RULE_MATCH_PART, 1, context);
            // set left component index operator
            rdata->set_int(vp_id, std::stoi(bm_map[1]), PIT_RULE_MATCH_PART, 2, context);
            // set operator
            rdata->set_int(vp_id, std::stoi(bm_map[2]), PIT_RULE_MATCH_PART, 3, context);
            // set operand
            rdata->set_cstr(vp_id, bm_map[4].c_str(), PIT_RULE_MATCH_PART, 4, context);
            break;
        }

        // REGEX
        case OTT_REGEX:
        {
            // set type
            rdata->set_int(vp_id, OTT_REGEX, PIT_RULE_MATCH_PART, 1, context);
            // set left component index operator
            rdata->set_int(vp_id, std::stoi(bm_map[1]), PIT_RULE_MATCH_PART, 2, context);
            // set operand
            rdata->set_cstr(vp_id, bm_map[4].c_str(), PIT_RULE_MATCH_PART, 3, context);
            break;
        }
    
        // VARIABLE
        case OTT_VARIABLE:
        {
            // set type
            rdata->set_int(vp_id, OTT_VARIABLE, PIT_RULE_MATCH_PART, 1, context);
            // set left component index operator
            rdata->set_int(vp_id, std::stoi(bm_map[1]), PIT_RULE_MATCH_PART, 2, context);
            // set operator
            rdata->set_int(vp_id, std::stoi(bm_map[2]), PIT_RULE_MATCH_PART, 3, context);
            // set right component index operator
            rdata->set_int(vp_id, std::stoi(bm_map[3]), PIT_RULE_MATCH_PART, 4, context);
            // set operand
            rdata->set_cstr(vp_id, bm_map[4].c_str(), PIT_RULE_MATCH_PART, 5, context);
            break;
        }

        // LIST
        case OTT_LIST:
        {
            // set type
            rdata->set_int(vp_id, OTT_LIST, PIT_RULE_MATCH_PART, 1, context);
            // set left component index operator
            rdata->set_int(vp_id, std::stoi(bm_map[1]), PIT_RULE_MATCH_PART, 2, context);
            // set operator
            rdata->set_int(vp_id, std::stoi(bm_map[2]), PIT_RULE_MATCH_PART, 3, context);
            // set operand
            rdata->set_cstr(vp_id, bm_map[4].c_str(), PIT_RULE_MATCH_PART, 4, context);
            break;
        }

        // STRING
        case OTT_STRING:
        {
            // if starting with `` use LUA scripting
            if(bm_map[4].size() > 2 && boost::starts_with(bm_map[4], "``")){
                // inline/external flag
                bool ext = false;
                // tmp string
                std::string tmp_str;
                // if next character is '/', load script from external file
                if(boost::starts_with(bm_map[4], "``/")){
                    // file path
                    std::string tmp_file_str(bm_map[4], 2);
                    // file size
                    int fsize = pmink_utils::get_file_size(tmp_file_str.c_str());
                    // check if file size is valid
                    if(fsize > 0){
                        // tmp buffer
                        char tmp_buff[fsize];
                        // try to load file
                        if(pmink_utils::load_file(tmp_file_str.c_str(), tmp_buff, &fsize) != 0){
                            pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot load external Lua script [%s]", tmp_file_str.c_str());
                            return 1;
                        }
                        // set lua script  string
                        tmp_str.assign(tmp_buff, fsize);
                        // set external flag
                        ext = true;

                        // file size error
                    }else{
                        pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot load external Lua script [%s]", tmp_file_str.c_str());
                        return 1;
                    }

                    // load inline script string and prepend it with pmink module
                }else{
                    tmp_str = bm_map[4].substr(2);
                    tmp_str.insert(0, PMINK_LUA_MODULE);
                }

                // create lua state
                lua_State* L = luaL_newstate();
                // init lua
                luaL_openlibs(L);
                // load string
                if(luaL_loadstring(L, tmp_str.c_str()) == 0){
                    // set type
                    rdata->set_int(vp_id, OTT_LUA, PIT_RULE_MATCH_PART, 1, context);
                    // set lua script (dynamic string pointer due to possible large script file)
                    std::string* tmp_lua_script_str = new std::string(tmp_str);
                    //rdata->set_cstr(vp_id, tmp_str.c_str(), PIT_RULE_MATCH_PART, 2, context);
                    rdata->set_pointer(vp_id, tmp_lua_script_str, PIT_RULE_MATCH_PART, 2, context);
                    // arguments for external lua script
                    if(ext && args != NULL){
                        // argument count
                        int argc = 0;
                        // loop args
                        for(unsigned int i = 0; i<args->children.size(); i++){
                            // arg name and value strings
                            const char* tmp_arg_n = args->children[i]->name.c_str();
                            const char* tmp_arg_v = args->children[i]->to_cstr("value");
                            // sanity check
                            if(tmp_arg_v == NULL || strlen(tmp_arg_v) <= 0) continue;
                            // arguments start with fragment index 100 
                            // =======================================
                            // argument 0: fragment 100 = argument name, fragment 101 = argument value
                            // argument 1: fragment 102 = argument name, fragment 103 = argument value
                            // =======================================
                            // two fragments exist for every argument(name and value)
                            // even fragments = argument names
                            // odd fragments  = argument values
                            rdata->set_cstr(vp_id, tmp_arg_n, PIT_RULE_MATCH_PART, 100 + (i * 2), context);
                            rdata->set_cstr(vp_id, tmp_arg_v, PIT_RULE_MATCH_PART, 100 + (i * 2) + 1, context);
                            // inc argc
                            ++argc;
                        }
                        // set argument count
                        rdata->set_int(vp_id, argc, PIT_RULE_MATCH_PART, 4, context);
                    }

                    // error
                }else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Lua error: %s", lua_tostring(L, -1));

                // remove state (created in slave rule processors)
                lua_close(L);

            // normal string matching
            }else{
                // set type
                rdata->set_int(vp_id, OTT_STRING, PIT_RULE_MATCH_PART, 1, context);
                // set left component index operator
                rdata->set_int(vp_id, std::stoi(bm_map[1]), PIT_RULE_MATCH_PART, 2, context);
                // set operator
                rdata->set_int(vp_id, std::stoi(bm_map[2]), PIT_RULE_MATCH_PART, 3, context);
                // trim operand
                std::string tmp_str(bm_map[4]);
                boost::trim_if(tmp_str, boost::is_any_of("'"));
                // set operand
                rdata->set_cstr(vp_id, tmp_str.c_str(), PIT_RULE_MATCH_PART, 4, context);
            }
            break;
        }
        
        // unknown
        default: return 1;

    
    }

    // ok
    return 0;
}


#endif

int fgn::FilterManager::run_rules(rule_param_t* input, rule_m_t* rm, rule_t* first_rule, rule_t** last_rule){
    return rm->process_rules(input, last_rule, first_rule);
    //return 0;
}


int fgn::FilterManager::build_dests(config::ConfigItem* dests_root, rule_param_t* params){
    if(dests_root == NULL || dests_root->name != "destination") return 1;

    int c = 0;
    std::map<uint32_t, config::ConfigItem*> tmp_map;
    config::ConfigItem* dest_rule = NULL;

    // clear
    params->erase_param(asn1::ParameterType::_pt_pmink_routing_destination, -1);
    params->erase_param(asn1::ParameterType::_pt_pmink_routing_destination_type, -1);
    params->erase_param(asn1::ParameterType::_pt_pmink_routing_sub_destination, -1);
    params->erase_param(asn1::ParameterType::_pt_pmink_routing_sub_destination_type, -1);

    // loop destinations
    for(unsigned int i = 0; i<dests_root->children.size(); i++){
	// dest rule
	dest_rule = dests_root->children[i];
	// skip if deleted
	if(dest_rule->node_state == config::CONFIG_NS_DELETED) continue;
	// add to tmp map
	tmp_map[dest_rule->to_int("priority")] = dest_rule;
    }

    // tmp map sorted by priority, read
    for(std::map<uint32_t, config::ConfigItem*>::iterator it = tmp_map.begin(); it != tmp_map.end(); it++){
	// dest rule
	dest_rule = it->second;
	// r14p destination
	if((*dest_rule)("r14p")->node_state != config::CONFIG_NS_DELETED && dest_rule->val_exists("r14p")){
	    params->set_cstr(asn1::ParameterType::_pt_pmink_routing_destination, dest_rule->to_cstr("r14p"), c);

	    // subtype
	    if((*dest_rule)("as")->node_state != config::CONFIG_NS_DELETED && dest_rule->val_exists("as")){
		params->set_int(asn1::ParameterType::_pt_pmink_routing_sub_destination_type, 1, c);
		params->set_cstr(asn1::ParameterType::_pt_pmink_routing_sub_destination, dest_rule->to_cstr("as"), c);

	    }

	    ++c;


	}

    }

    return 0;

}

int fgn::FilterManager::build_rule(config::ConfigItem* rule_root, rule_m_t* rm){
    // sanity check
    if(rule_root == NULL) return 1;
    // tmp vars
    pmink_utils::PooledVPMap<uint32_t> tmp_params;
    // map contexts
    tmp_params.set_bool(RPT_RULE_CTX_SRI, false);
    tmp_params.set_bool(RPT_RULE_CTX_SM, false);
    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, false);
    tmp_params.set_bool(RPT_RULE_CTX_SMPP, false);
    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, false);
    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, false);
    // default filter result
    tmp_params.set_int(RPT_RULE_DEFAULT_FRES, pmink_utils::RRT_STOP_OK);


    // ** default filter result
    if(rule_root->val_exists("filter_result")){
	tmp_params.set_int(RPT_RULE_DEFAULT_FRES, rule_root->to_int("filter_result"));

    }

    // *** dummy ***
    if(rule_root->val_exists("definition match dummy script")){
	if(field_valid(rule_root->to_cstr("definition match dummy script"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match dummy script"), 0, RCT_DUMMY, (*rule_root)("definition match dummy args"));

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: dummy script] in rule [%s]!",
							   rule_root->to_cstr("definition match dummy script"),
							   rule_root->name.c_str());
    }

    // *** set special commands match data ***
    // sri-rpl cgpa ssn
    if(rule_root->val_exists("definition match commands sri-for-sm cgpa ssn")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cgpa ssn"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cgpa ssn"), asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cgpa ssn] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cgpa ssn"),
							   rule_root->name.c_str());
    }

    // sri-rpl cgpa gt tt
    if(rule_root->val_exists("definition match commands sri-for-sm cgpa gt tt")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cgpa gt tt"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cgpa gt tt"), asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cgpa gt tt] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cgpa gt tt"),
							   rule_root->name.c_str());
    }

    // sri-rpl cgpa gt np
    if(rule_root->val_exists("definition match commands sri-for-sm cgpa gt np")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cgpa gt np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cgpa gt np"), asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cgpa gt np] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cgpa gt np"),
							   rule_root->name.c_str());
    }

    // sri-rpl cgpa gt nai
    if(rule_root->val_exists("definition match commands sri-for-sm cgpa gt nai")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cgpa gt nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cgpa gt nai"), asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cgpa gt nai] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cgpa gt nai"),
							   rule_root->name.c_str());
    }

    // sri-rpl cgpa gt address
    if(rule_root->val_exists("definition match commands sri-for-sm cgpa gt address")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cgpa gt address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cgpa gt address"), asn1::ParameterType::_pt_sccp_calling_pa_gt_address, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cgpa gt address] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cgpa gt address"),
							   rule_root->name.c_str());
    }

    // sri-rpl cdpa ssn
    if(rule_root->val_exists("definition match commands sri-for-sm cdpa ssn")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cdpa ssn"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cdpa ssn"), asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cdpa ssn] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cdpa ssn"),
							   rule_root->name.c_str());
    }

    // sri-rpl cdpa gt tt
    if(rule_root->val_exists("definition match commands sri-for-sm cdpa gt tt")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cdpa gt tt"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cdpa gt tt"), asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cdpa gt tt] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cdpa gt tt"),
							   rule_root->name.c_str());
    }

    // sri-rpl cdpa gt np
    if(rule_root->val_exists("definition match commands sri-for-sm cdpa gt np")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cdpa gt np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cdpa gt np"), asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cdpa gt np] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cdpa gt np"),
							   rule_root->name.c_str());
    }

    // sri-rpl cdpa gt nai
    if(rule_root->val_exists("definition match commands sri-for-sm cdpa gt nai")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cdpa gt nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cdpa gt nai"), asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cdpa gt nai] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cdpa gt nai"),
							   rule_root->name.c_str());
    }

    // sri-rpl cdpa gt address
    if(rule_root->val_exists("definition match commands sri-for-sm cdpa gt address")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm cdpa gt address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm cdpa gt address"), asn1::ParameterType::_pt_sccp_called_pa_gt_address, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm cdpa gt address] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm cdpa gt address"),
							   rule_root->name.c_str());
    }

    // sri-rpl imsi
    if(rule_root->val_exists("definition match commands sri-for-sm imsi")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm imsi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm imsi"), asn1::ParameterType::_pt_gsmmap_imsi, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm imsi] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm imsi"),
							   rule_root->name.c_str());
    }

    // sri-rpl nnn nai
    if(rule_root->val_exists("definition match commands sri-for-sm nnn nai")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm nnn nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm nnn nai"), asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm nnn nai] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm nnn nai"),
							   rule_root->name.c_str());
    }

    // sri-rpl nnn np
    if(rule_root->val_exists("definition match commands sri-for-sm nnn np")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm nnn np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm nnn np"), asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm nnn np] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm nnn np"),
							   rule_root->name.c_str());
    }

    // sri-rpl nnn address
    if(rule_root->val_exists("definition match commands sri-for-sm nnn address")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm nnn address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm nnn address"), asn1::ParameterType::_pt_gsmmap_nnn_digits, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm nnn address] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm nnn address"),
							   rule_root->name.c_str());
    }

    // sri-rpl an nai
    if(rule_root->val_exists("definition match commands sri-for-sm an nai")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm an nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm an nai"), asn1::ParameterType::_pt_gsmmap_an_type_of_number, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm an nai] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm an nai"),
							   rule_root->name.c_str());
    }

    // sri-rpl an np
    if(rule_root->val_exists("definition match commands sri-for-sm an np")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm an np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm an np"), asn1::ParameterType::_pt_gsmmap_an_numbering_plan, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm an np] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm an np"),
							   rule_root->name.c_str());
    }

    // sri-rpl an address
    if(rule_root->val_exists("definition match commands sri-for-sm an address")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm an address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm an address"), asn1::ParameterType::_pt_gsmmap_an_digits, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm an address] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm an address"),
							   rule_root->name.c_str());
    }


    // sri-rpl msisdn nai
    if(rule_root->val_exists("definition match commands sri-for-sm msisdn nai")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm msisdn nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm msisdn nai"), asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm msisdn nai] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm msisdn nai"),
							   rule_root->name.c_str());
    }

    // sri-rpl msisdn np
    if(rule_root->val_exists("definition match commands sri-for-sm msisdn np")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm msisdn np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm msisdn np"), asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm msisdn np] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm msisdn np"),
							   rule_root->name.c_str());
    }

    // sri-rpl msisdn address
    if(rule_root->val_exists("definition match commands sri-for-sm msisdn address")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm msisdn address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm msisdn address"), asn1::ParameterType::_pt_gsmmap_msisdn_digits, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm msisdn address] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm msisdn address"),
							   rule_root->name.c_str());
    }

    // sri-rpl sca nai
    if(rule_root->val_exists("definition match commands sri-for-sm sca nai")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm sca nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm sca nai"), asn1::ParameterType::_pt_gsmmap_sca_type_of_number, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm sca nai] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm sca nai"),
							   rule_root->name.c_str());
    }

    // sri-rpl sca np
    if(rule_root->val_exists("definition match commands sri-for-sm sca np")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm sca np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm sca np"), asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm sca np] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm sca np"),
							   rule_root->name.c_str());
    }

    // sri-rpl sca address
    if(rule_root->val_exists("definition match commands sri-for-sm sca address")){
	if(field_valid(rule_root->to_cstr("definition match commands sri-for-sm sca address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match commands sri-for-sm sca address"), asn1::ParameterType::_pt_gsmmap_sca_digits, RCT_CMD_SRI_RPL);
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_RPL, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: commands sri-for-sm sca address] in rule [%s]!",
							   rule_root->to_cstr("definition match commands sri-for-sm sca address"),
							   rule_root->name.c_str());
    }




    // *** set r14p match data ***
    // trunk label
    if(rule_root->val_exists("definition match r14p trunk_label")){
	if(field_valid(rule_root->to_cstr("definition match r14p trunk_label")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match r14p trunk_label"), asn1::ParameterType::_pt_pmink_trunk_label);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p trunk_label] in rule [%s]!",
							  rule_root->to_cstr("definition match r14p trunk_label"),
							  rule_root->name.c_str());
    }

    // service id
    if(rule_root->val_exists("definition match r14p service_id")){
	if(field_valid(rule_root->to_cstr("definition match r14p service_id")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match r14p service_id"), asn1::ParameterType::_pt_pmink_service_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p service_id] in rule [%s]!",
							  rule_root->to_cstr("definition match r14p service_id"),
							  rule_root->name.c_str());
    }

    // src type
    if(rule_root->val_exists("definition match r14p src_type")){
	if(field_valid(rule_root->to_cstr("definition match r14p src_type")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match r14p src_type"), asn1::ParameterType::_pt_pmink_daemon_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p src_type] in rule [%s]!",
							  rule_root->to_cstr("definition match r14p src_type"),
							  rule_root->name.c_str());
    }

    // src id
    if(rule_root->val_exists("definition match r14p src_id")){
	if(field_valid(rule_root->to_cstr("definition match r14p src_id")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match r14p src_id"), asn1::ParameterType::_pt_pmink_daemon_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p src_id] in rule [%s]!",
							  rule_root->to_cstr("definition match r14p src_id"),
							  rule_root->name.c_str());
    }

    // cmd id
    if(rule_root->val_exists("definition match r14p cmd_id")){
	if(field_valid(rule_root->to_cstr("definition match r14p cmd_id")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match r14p cmd_id"), asn1::ParameterType::_pt_pmink_command_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p cmd_id] in rule [%s]!",
							  rule_root->to_cstr("definition match r14p cmd_id"),
							  rule_root->name.c_str());
    }

    // connectoion type
    if(rule_root->val_exists("definition match r14p conn_type")){
	if(field_valid(rule_root->to_cstr("definition match r14p conn_type")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match r14p conn_type"), asn1::ParameterType::_pt_pmink_connection_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p conn_type] in rule [%s]!",
							  rule_root->to_cstr("definition match r14p conn_type"),
							  rule_root->name.c_str());
    }

    // loop count
    if(rule_root->val_exists("definition match r14p loop_count")){
	if(field_valid(rule_root->to_cstr("definition match r14p loop_count")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match r14p loop_count"), asn1::ParameterType::_pt_pmink_loop_count);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: r14p loop_count] in rule [%s]!",
							  rule_root->to_cstr("definition match r14p loop_count"),
							  rule_root->name.c_str());
    }


    // *** set m3ua match data ***
    // m3ua opc
    if(rule_root->val_exists("definition match m3ua opc")){
	if(field_valid(rule_root->to_cstr("definition match m3ua opc")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match m3ua opc"), asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua opc] in rule [%s]!",
							  rule_root->to_cstr("definition match m3ua opc"),
							  rule_root->name.c_str());
    }

    // m3ua dpc
    if(rule_root->val_exists("definition match m3ua dpc")){
	if(field_valid(rule_root->to_cstr("definition match m3ua dpc")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match m3ua dpc"), asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua dpc] in rule [%s]!",
							  rule_root->to_cstr("definition match m3ua dpc"),
							  rule_root->name.c_str());
    }

    // m3ua si
    if(rule_root->val_exists("definition match m3ua si")){
	if(field_valid(rule_root->to_cstr("definition match m3ua si")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match m3ua si"), asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua si] in rule [%s]!",
							  rule_root->to_cstr("definition match m3ua si"),
							  rule_root->name.c_str());
    }


    // m3ua ni
    if(rule_root->val_exists("definition match m3ua ni")){
	if(field_valid(rule_root->to_cstr("definition match m3ua ni")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match m3ua ni"), asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua ni] in rule [%s]!",
							  rule_root->to_cstr("definition match m3ua ni"),
							  rule_root->name.c_str());
    }

    // m3ua mp
    if(rule_root->val_exists("definition match m3ua mp")){
	if(field_valid(rule_root->to_cstr("definition match m3ua mp")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match m3ua mp"), asn1::ParameterType::_pt_m3ua_protocol_data_message_priority);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua mp] in rule [%s]!",
							  rule_root->to_cstr("definition match m3ua mp"),
							  rule_root->name.c_str());
    }

    // m3ua sls
    if(rule_root->val_exists("definition match m3ua sls")){
	if(field_valid(rule_root->to_cstr("definition match m3ua sls")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match m3ua sls"), asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua sls] in rule [%s]!",
							  rule_root->to_cstr("definition match m3ua sls"),
							  rule_root->name.c_str());
    }

    // m3ua as
    if(rule_root->val_exists("definition match m3ua as")){
	if(field_valid(rule_root->to_cstr("definition match m3ua as")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match m3ua as"), asn1::ParameterType::_pt_m3ua_as_label);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua as] in rule [%s]!",
							  rule_root->to_cstr("definition match m3ua as"),
							  rule_root->name.c_str());
    }

    // m3ua asp
    if(rule_root->val_exists("definition match m3ua asp")){
	if(field_valid(rule_root->to_cstr("definition match m3ua asp")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match m3ua asp"), asn1::ParameterType::_pt_m3ua_asp_label);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: m3ua asp] in rule [%s]!",
							  rule_root->to_cstr("definition match m3ua asp"),
							  rule_root->name.c_str());
    }

    // *** set sccp match data ***
    // **** calling party ****
    // sccp calling party routing indicator
    if(rule_root->val_exists("definition match sccp cgpa routing-indicator")){
	if(field_valid(rule_root->to_cstr("definition match sccp cgpa routing-indicator")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cgpa routing-indicator"), asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa routing-indicator] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cgpa routing-indicator"),
							  rule_root->name.c_str());
    }

    // sccp calling party gti
    if(rule_root->val_exists("definition match sccp cgpa gti")){
	if(field_valid(rule_root->to_cstr("definition match sccp cgpa gti")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cgpa gti"), asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gti] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cgpa gti"),
							  rule_root->name.c_str());
    }

    // sccp calling party ssn
    if(rule_root->val_exists("definition match sccp cgpa ssn")){
	if(field_valid(rule_root->to_cstr("definition match sccp cgpa ssn")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cgpa ssn"), asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa ssn] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cgpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp calling party point code
    if(rule_root->val_exists("definition match sccp cgpa point-code")){
	if(field_valid(rule_root->to_cstr("definition match sccp cgpa point-code")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cgpa point-code"), asn1::ParameterType::_pt_sccp_calling_pa_point_code_number);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa point-code] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cgpa point-code"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title tt
    if(rule_root->val_exists("definition match sccp cgpa gt tt")){
	if(field_valid(rule_root->to_cstr("definition match sccp cgpa gt tt")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cgpa gt tt"), asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gt tt] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cgpa gt tt"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title np
    if(rule_root->val_exists("definition match sccp cgpa gt np")){
	if(field_valid(rule_root->to_cstr("definition match sccp cgpa gt np")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cgpa gt np"), asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gt np] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cgpa gt np"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title nai
    if(rule_root->val_exists("definition match sccp cgpa gt nai")){
	if(field_valid(rule_root->to_cstr("definition match sccp cgpa gt nai")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cgpa gt nai"), asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gt nai] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cgpa gt nai"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title address
    if(rule_root->val_exists("definition match sccp cgpa gt address")){
	if(field_valid(rule_root->to_cstr("definition match sccp cgpa gt address")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cgpa gt address"), asn1::ParameterType::_pt_sccp_calling_pa_gt_address);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cgpa gt address] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cgpa gt address"),
							  rule_root->name.c_str());
    }

    // **** called party ****
    // sccp called party routing indicator
    if(rule_root->val_exists("definition match sccp cdpa routing-indicator")){
	if(field_valid(rule_root->to_cstr("definition match sccp cdpa routing-indicator")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cdpa routing-indicator"), asn1::ParameterType::_pt_sccp_called_pa_routing_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa routing-indicator] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cdpa routing-indicator"),
							  rule_root->name.c_str());
    }

    // sccp called party gti
    if(rule_root->val_exists("definition match sccp cdpa gti")){
	if(field_valid(rule_root->to_cstr("definition match sccp cdpa gti")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cdpa gti"), asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gti] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cdpa gti"),
							  rule_root->name.c_str());
    }

    // sccp called party ssn
    if(rule_root->val_exists("definition match sccp cdpa ssn")){
	if(field_valid(rule_root->to_cstr("definition match sccp cdpa ssn")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cdpa ssn"), asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa ssn] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cdpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp called party point code
    if(rule_root->val_exists("definition match sccp cdpa point-code")){
	if(field_valid(rule_root->to_cstr("definition match sccp cdpa point-code")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cdpa point-code"), asn1::ParameterType::_pt_sccp_called_pa_point_code_number);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa point-code] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cdpa point-code"),
							  rule_root->name.c_str());
    }

    // sccp called party global title tt
    if(rule_root->val_exists("definition match sccp cdpa gt tt")){
	if(field_valid(rule_root->to_cstr("definition match sccp cdpa gt tt")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cdpa gt tt"), asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gt tt] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cdpa gt tt"),
							  rule_root->name.c_str());
    }

    // sccp called party global title np
    if(rule_root->val_exists("definition match sccp cdpa gt np")){
	if(field_valid(rule_root->to_cstr("definition match sccp cdpa gt np")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cdpa gt np"), asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gt np] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cdpa gt np"),
							  rule_root->name.c_str());
    }

    // sccp called party global title nai
    if(rule_root->val_exists("definition match sccp cdpa gt nai")){
	if(field_valid(rule_root->to_cstr("definition match sccp cdpa gt nai")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cdpa gt nai"), asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gt nai] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cdpa gt nai"),
							  rule_root->name.c_str());
    }

    // sccp called party global title address
    if(rule_root->val_exists("definition match sccp cdpa gt address")){
	if(field_valid(rule_root->to_cstr("definition match sccp cdpa gt address")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match sccp cdpa gt address"), asn1::ParameterType::_pt_sccp_called_pa_gt_address);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: sccp cdpa gt address] in rule [%s]!",
							  rule_root->to_cstr("definition match sccp cdpa gt address"),
							  rule_root->name.c_str());
    }

    // *** set tcap match data ***
    // tcap message type
    if(rule_root->val_exists("definition match tcap tcmt")){
	if(field_valid(rule_root->to_cstr("definition match tcap tcmt")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match tcap tcmt"), asn1::ParameterType::_pt_tcap_message_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap tcmt] in rule [%s]!",
							  rule_root->to_cstr("definition match tcap tcmt"),
							  rule_root->name.c_str());
    }

    // tcap sid
    if(rule_root->val_exists("definition match tcap sid")){
	if(field_valid(rule_root->to_cstr("definition match tcap sid")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match tcap sid"), asn1::ParameterType::_pt_tcap_source_transaction_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap sid] in rule [%s]!",
							  rule_root->to_cstr("definition match tcap sid"),
							  rule_root->name.c_str());
    }

    // tcap did
    if(rule_root->val_exists("definition match tcap did")){
	if(field_valid(rule_root->to_cstr("definition match tcap did")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match tcap did"), asn1::ParameterType::_pt_tcap_destination_transaction_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap did] in rule [%s]!",
							  rule_root->to_cstr("definition match tcap did"),
							  rule_root->name.c_str());
    }

    // tcap component count
    if(rule_root->val_exists("definition match tcap cc")){
	if(field_valid(rule_root->to_cstr("definition match tcap cc")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match tcap cc"), asn1::ParameterType::_pt_tcap_component_count);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap cc] in rule [%s]!",
							  rule_root->to_cstr("definition match tcap cc"),
							  rule_root->name.c_str());
    }

    // tcap component type
    if(rule_root->val_exists("definition match tcap ct")){
	if(field_valid(rule_root->to_cstr("definition match tcap ct")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match tcap ct"), asn1::ParameterType::_pt_tcap_component_type);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap ct] in rule [%s]!",
							  rule_root->to_cstr("definition match tcap ct"),
							  rule_root->name.c_str());
    }

    // tcap invoke id
    if(rule_root->val_exists("definition match tcap iid")){
	if(field_valid(rule_root->to_cstr("definition match tcap iid")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match tcap iid"), asn1::ParameterType::_pt_tcap_component_invoke_id);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap iid] in rule [%s]!",
							  rule_root->to_cstr("definition match tcap iid"),
							  rule_root->name.c_str());
    }

    // tcap operation code
    if(rule_root->val_exists("definition match tcap opcode")){
	if(field_valid(rule_root->to_cstr("definition match tcap opcode")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match tcap opcode"), asn1::ParameterType::_pt_tcap_opcode);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap opcode] in rule [%s]!",
							  rule_root->to_cstr("definition match tcap opcode"),
							  rule_root->name.c_str());
    }

    // tcap dialogue application context
    if(rule_root->val_exists("definition match tcap dlg_ctx")){
	if(field_valid(rule_root->to_cstr("definition match tcap dlg_ctx")))
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match tcap dlg_ctx"), asn1::ParameterType::_pt_tcap_dialogue_context_oid);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: tcap dlg_ctx] in rule [%s]!",
							  rule_root->to_cstr("definition match tcap dlg_ctx"),
							  rule_root->name.c_str());
    }

    // *** set map match data ***
    // map sri-for-sm msisdn nai
    if(rule_root->val_exists("definition match map context sri-for-sm msisdn nai")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm msisdn nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm msisdn nai"), asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm msisdn nai] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm msisdn nai"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm msisdn np
    if(rule_root->val_exists("definition match map context sri-for-sm msisdn np")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm msisdn np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm msisdn np"), asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm msisdn np] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm msisdn np"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm msisdn address
    if(rule_root->val_exists("definition match map context sri-for-sm msisdn address")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm msisdn address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm msisdn address"), asn1::ParameterType::_pt_gsmmap_msisdn_digits, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm msisdn address] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm msisdn address"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm sca nai
    if(rule_root->val_exists("definition match map context sri-for-sm sca nai")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm sca nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm sca nai"), asn1::ParameterType::_pt_gsmmap_sca_type_of_number, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm sca nai] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm sca nai"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm sca np
    if(rule_root->val_exists("definition match map context sri-for-sm sca np")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm sca np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm sca np"), asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm sca np] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm sca np"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm sca address
    if(rule_root->val_exists("definition match map context sri-for-sm sca address")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm sca address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm sca address"), asn1::ParameterType::_pt_gsmmap_sca_digits, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm sca address] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm sca address"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm imsi
    if(rule_root->val_exists("definition match map context sri-for-sm imsi")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm imsi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm imsi"), asn1::ParameterType::_pt_gsmmap_imsi, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm imsi] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm imsi"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm an nai
    if(rule_root->val_exists("definition match map context sri-for-sm an nai")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm an nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm an nai"), asn1::ParameterType::_pt_gsmmap_an_type_of_number, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm an nai] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm an nai"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm an np
    if(rule_root->val_exists("definition match map context sri-for-sm an np")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm an np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm an np"), asn1::ParameterType::_pt_gsmmap_an_numbering_plan, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm an np] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm an np"),
							  rule_root->name.c_str());
    }

    // map sri-for-sm an address
    if(rule_root->val_exists("definition match map context sri-for-sm an address")){
	if(field_valid(rule_root->to_cstr("definition match map context sri-for-sm an address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sri-for-sm an address"), asn1::ParameterType::_pt_gsmmap_an_digits, RCT_MAP_SRI_FOR_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SRI, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sri-for-sm an address] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sri-for-sm an address"),
							  rule_root->name.c_str());
    }

    // map sm imsi
    if(rule_root->val_exists("definition match map context sm imsi")){
	if(field_valid(rule_root->to_cstr("definition match map context sm imsi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm imsi"), asn1::ParameterType::_pt_gsmmap_imsi, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm imsi] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm imsi"),
							  rule_root->name.c_str());
    }

    // map sm scda nai
    if(rule_root->val_exists("definition match map context sm scda nai")){
	if(field_valid(rule_root->to_cstr("definition match map context sm scda nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm scda nai"), asn1::ParameterType::_pt_gsmmap_scda_type_of_number, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scda nai] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm scda nai"),
							  rule_root->name.c_str());
    }

    // map sm scda np
    if(rule_root->val_exists("definition match map context sm scda np")){
	if(field_valid(rule_root->to_cstr("definition match map context sm scda np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm scda np"), asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scda np] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm scda np"),
							  rule_root->name.c_str());
    }

    // map sm scda address
    if(rule_root->val_exists("definition match map context sm scda address")){
	if(field_valid(rule_root->to_cstr("definition match map context sm scda address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm scda address"), asn1::ParameterType::_pt_gsmmap_scda_digits, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scda address] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm scda address"),
							  rule_root->name.c_str());
    }


    // map sm scoa nai
    if(rule_root->val_exists("definition match map context sm scoa nai")){
	if(field_valid(rule_root->to_cstr("definition match map context sm scoa nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm scoa nai"), asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scoa nai] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm scoa nai"),
							  rule_root->name.c_str());
    }

    // map sm scoa np
    if(rule_root->val_exists("definition match map context sm scoa np")){
	if(field_valid(rule_root->to_cstr("definition match map context sm scoa np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm scoa np"), asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scoa np] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm scoa np"),
							  rule_root->name.c_str());
    }

    // map sm scoa address
    if(rule_root->val_exists("definition match map context sm scoa address")){
	if(field_valid(rule_root->to_cstr("definition match map context sm scoa address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm scoa address"), asn1::ParameterType::_pt_gsmmap_scoa_digits, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm scoa address] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm scoa address"),
							  rule_root->name.c_str());
    }

    // map sm msisdn nai
    if(rule_root->val_exists("definition match map context sm msisdn nai")){
	if(field_valid(rule_root->to_cstr("definition match map context sm msisdn nai"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm msisdn nai"), asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm msisdn nai] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm msisdn nai"),
							  rule_root->name.c_str());
    }

    // map sm msisdn np
    if(rule_root->val_exists("definition match map context sm msisdn np")){
	if(field_valid(rule_root->to_cstr("definition match map context sm msisdn np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm msisdn np"), asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm msisdn np] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm msisdn np"),
							  rule_root->name.c_str());
    }

    // map sm msisdn address
    if(rule_root->val_exists("definition match map context sm msisdn address")){
	if(field_valid(rule_root->to_cstr("definition match map context sm msisdn address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match map context sm msisdn address"), asn1::ParameterType::_pt_gsmmap_msisdn_digits, RCT_MAP_SM);
	    tmp_params.set_bool(RPT_RULE_CTX_SM, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: map context sm msisdn address] in rule [%s]!",
							  rule_root->to_cstr("definition match map context sm msisdn address"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-rp
    if(rule_root->val_exists("definition match smstpdu tp-rp")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-rp"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-rp"), asn1::ParameterType::_pt_smstpdu_tp_rp);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-rp] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-rp"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-udhi
    if(rule_root->val_exists("definition match smstpdu tp-udhi")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-udhi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-udhi"), asn1::ParameterType::_pt_smstpdu_tp_udhi);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-udhi] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-udhi"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-srr
    if(rule_root->val_exists("definition match smstpdu tp-srr")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-srr"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-srr"), asn1::ParameterType::_pt_smstpdu_tp_srr);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-srr] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-srr"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-vpf
    if(rule_root->val_exists("definition match smstpdu tp-vpf")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-vpf"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-vpf"), asn1::ParameterType::_pt_smstpdu_tp_vpf);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-vpf] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-vpf"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-rd
    if(rule_root->val_exists("definition match smstpdu tp-rd")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-rd"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-rd"), asn1::ParameterType::_pt_smstpdu_tp_rd);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-rd] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-rd"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-mti
    if(rule_root->val_exists("definition match smstpdu tp-mti")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-mti"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-mti"), asn1::ParameterType::_pt_smstpdu_tp_mti);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-mti] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-mti"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-mr
    if(rule_root->val_exists("definition match smstpdu tp-mr")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-mr"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-mr"), asn1::ParameterType::_pt_smstpdu_tp_mr);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-mr] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-mr"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-sri
    if(rule_root->val_exists("definition match smstpdu tp-sri")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-sri"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-sri"), asn1::ParameterType::_pt_smstpdu_tp_sri);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-sri] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-sri"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-mms
    if(rule_root->val_exists("definition match smstpdu tp-mms")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-mms"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-mms"), asn1::ParameterType::_pt_smstpdu_tp_mms);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-mms] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-mms"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-da ton
    if(rule_root->val_exists("definition match smstpdu tp-da ton")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-da ton"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-da ton"), asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-da ton] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-da ton"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-da np
    if(rule_root->val_exists("definition match smstpdu tp-da np")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-da np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-da np"), asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-da np] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-da np"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-da address
    if(rule_root->val_exists("definition match smstpdu tp-da address")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-da address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-da address"), asn1::ParameterType::_pt_smstpdu_tp_da_digits);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-da address] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-da address"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-oa ton
    if(rule_root->val_exists("definition match smstpdu tp-oa ton")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-oa ton"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-oa ton"), asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-oa ton] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-oa ton"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-oa np
    if(rule_root->val_exists("definition match smstpdu tp-oa np")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-oa np"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-oa np"), asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-oa np] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-oa np"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-oa address
    if(rule_root->val_exists("definition match smstpdu tp-oa address")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-oa address"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-oa address"), asn1::ParameterType::_pt_smstpdu_tp_oa_digits);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-oa address] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-oa address"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-pid
    if(rule_root->val_exists("definition match smstpdu tp-pid")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-pid"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-pid"), asn1::ParameterType::_pt_smstpdu_tp_pid);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-pid] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-pid"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-dcs
    if(rule_root->val_exists("definition match smstpdu tp-dcs")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-dcs"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-dcs"), asn1::ParameterType::_pt_smstpdu_tp_dcs);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-dcs] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-dcs"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-vp
    if(rule_root->val_exists("definition match smstpdu tp-vp")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-vp"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-vp"), asn1::ParameterType::_pt_smstpdu_tp_vp);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-vp] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-vp"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-udl
    if(rule_root->val_exists("definition match smstpdu tp-udl")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-udl"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-udl"), asn1::ParameterType::_pt_smstpdu_tp_udl);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-udl] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-udl"),
							  rule_root->name.c_str());
    }

    // smstpdu tp-scts
    if(rule_root->val_exists("definition match smstpdu tp-scts")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu tp-scts"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu tp-scts"), asn1::ParameterType::_pt_smstpdu_tp_scts);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu tp-scts] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu tp-scts"),
							  rule_root->name.c_str());
    }

    // smstpdu ie-msg-id
    if(rule_root->val_exists("definition match smstpdu ie-msg-id")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu ie-msg-id"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu ie-msg-id"), asn1::ParameterType::_pt_smstpdu_msg_id);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu ie-msg-id] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu ie-msg-id"),
							  rule_root->name.c_str());
    }

    // smstpdu ie-msg-parts
    if(rule_root->val_exists("definition match smstpdu ie-msg-parts")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu ie-msg-parts"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu ie-msg-parts"), asn1::ParameterType::_pt_smstpdu_msg_parts);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu ie-msg-parts] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu ie-msg-parts"),
							  rule_root->name.c_str());
    }

    // smstpdu ie-msg-part
    if(rule_root->val_exists("definition match smstpdu ie-msg-part")){
	if(field_valid(rule_root->to_cstr("definition match smstpdu ie-msg-part"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smstpdu ie-msg-part"), asn1::ParameterType::_pt_smstpdu_msg_part);
	    tmp_params.set_bool(RPT_RULE_CTX_SMSTPDU, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smstpdu ie-msg-part] in rule [%s]!",
							  rule_root->to_cstr("definition match smstpdu ie-msg-part"),
							  rule_root->name.c_str());
    }


    // *** smpp ***
    // smpp as
    if(rule_root->val_exists("definition match smpp as")){
	if(field_valid(rule_root->to_cstr("definition match smpp as"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp as"), asn1::ParameterType::_pt_smpp_as_label);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp as] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp as"),
							  rule_root->name.c_str());
    }

    // smpp asp
    if(rule_root->val_exists("definition match smpp asp")){
	if(field_valid(rule_root->to_cstr("definition match smpp asp"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp asp"), asn1::ParameterType::_pt_smpp_asp_label);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp asp] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp asp"),
							  rule_root->name.c_str());
    }


    // smpp command_id
    if(rule_root->val_exists("definition match smpp command_id")){
	if(field_valid(rule_root->to_cstr("definition match smpp command_id"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp command_id"), asn1::ParameterType::_pt_smpp_command_id);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp command_id] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp command_id"),
							  rule_root->name.c_str());
    }

    // smpp source_addr_ton
    if(rule_root->val_exists("definition match smpp source_addr_ton")){
	if(field_valid(rule_root->to_cstr("definition match smpp source_addr_ton"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp source_addr_ton"), asn1::ParameterType::_pt_smpp_originator_ton);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp source_addr_ton] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp source_addr_ton"),
							  rule_root->name.c_str());
    }

    // smpp dest_addr_ton
    if(rule_root->val_exists("definition match smpp dest_addr_ton")){
	if(field_valid(rule_root->to_cstr("definition match smpp dest_addr_ton"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp dest_addr_ton"), asn1::ParameterType::_pt_smpp_recipient_ton);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp dest_addr_ton] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp dest_addr_ton"),
							  rule_root->name.c_str());
    }

    // smpp source_addr_npi
    if(rule_root->val_exists("definition match smpp source_addr_npi")){
	if(field_valid(rule_root->to_cstr("definition match smpp source_addr_npi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp source_addr_npi"), asn1::ParameterType::_pt_smpp_originator_np);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp source_addr_npi] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp source_addr_npi"),
							  rule_root->name.c_str());
    }

    // smpp dest_addr_npi
    if(rule_root->val_exists("definition match smpp dest_addr_npi")){
	if(field_valid(rule_root->to_cstr("definition match smpp dest_addr_npi"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp dest_addr_npi"), asn1::ParameterType::_pt_smpp_recipient_np);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp dest_addr_npi] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp dest_addr_npi"),
							  rule_root->name.c_str());
    }

    // smpp source_addr
    if(rule_root->val_exists("definition match smpp source_addr")){
	if(field_valid(rule_root->to_cstr("definition match smpp source_addr"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp source_addr"), asn1::ParameterType::_pt_smpp_originator_address);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp source_addr] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp source_addr"),
							  rule_root->name.c_str());
    }

    // smpp destination_addr
    if(rule_root->val_exists("definition match smpp destination_addr")){
	if(field_valid(rule_root->to_cstr("definition match smpp destination_addr"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp destination_addr"), asn1::ParameterType::_pt_smpp_recipient_address);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp destination_addr] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp destination_addr"),
							  rule_root->name.c_str());
    }

    // smpp esm_class_mm
    if(rule_root->val_exists("definition match smpp esm_class_mm")){
	if(field_valid(rule_root->to_cstr("definition match smpp esm_class_mm"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp esm_class_mm"), asn1::ParameterType::_pt_smpp_esm_message_mode);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp esm_class_mm] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp esm_class_mm"),
							  rule_root->name.c_str());
    }

    // smpp esm_class_mt
    if(rule_root->val_exists("definition match smpp esm_class_mt")){
	if(field_valid(rule_root->to_cstr("definition match smpp esm_class_mt"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp esm_class_mt"), asn1::ParameterType::_pt_smpp_esm_message_type);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp esm_class_mt] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp esm_class_mt"),
							  rule_root->name.c_str());
    }

    // smpp esm_class_gsm
    if(rule_root->val_exists("definition match smpp esm_class_gsm")){
	if(field_valid(rule_root->to_cstr("definition match smpp esm_class_gsm"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp esm_class_gsm"), asn1::ParameterType::_pt_smpp_esm_gsm_features);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp esm_class_gsm] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp esm_class_gsm"),
							  rule_root->name.c_str());
    }

    // smpp protocol_id
    if(rule_root->val_exists("definition match smpp protocol_id")){
	if(field_valid(rule_root->to_cstr("definition match smpp protocol_id"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp protocol_id"), asn1::ParameterType::_pt_smpp_protocol_id);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp protocol_id] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp protocol_id"),
							  rule_root->name.c_str());
    }

    // smpp priority_flag
    if(rule_root->val_exists("definition match smpp priority_flag")){
	if(field_valid(rule_root->to_cstr("definition match smpp priority_flag"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp priority_flag"), asn1::ParameterType::_pt_smpp_priority_flag);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp priority_flag] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp priority_flag"),
							  rule_root->name.c_str());
    }

    // smpp delivery_time
    if(rule_root->val_exists("definition match smpp delivery_time")){
	if(field_valid(rule_root->to_cstr("definition match smpp delivery_time"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp delivery_time"), asn1::ParameterType::_pt_smpp_delivery_time);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp delivery_time] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp delivery_time"),
							  rule_root->name.c_str());
    }

    // smpp validity_period
    if(rule_root->val_exists("definition match smpp validity_period")){
	if(field_valid(rule_root->to_cstr("definition match smpp validity_period"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp validity_period"), asn1::ParameterType::_pt_smpp_validity_period);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp validity_period] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp validity_period"),
							  rule_root->name.c_str());
    }

    // smpp rd_smsc_dlvr_rcpt
    if(rule_root->val_exists("definition match smpp rd_smsc_dlvr_rcpt")){
	if(field_valid(rule_root->to_cstr("definition match smpp rd_smsc_dlvr_rcpt"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp rd_smsc_dlvr_rcpt"), asn1::ParameterType::_pt_smpp_rd_smsc_receipt);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp rd_smsc_dlvr_rcpt] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp rd_smsc_dlvr_rcpt"),
							  rule_root->name.c_str());
    }

    // smpp rd_sme_orig_ack
    if(rule_root->val_exists("definition match smpp rd_sme_orig_ack")){
	if(field_valid(rule_root->to_cstr("definition match smpp rd_sme_orig_ack"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp rd_sme_orig_ack"), asn1::ParameterType::_pt_smpp_rd_sme_ack);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp rd_sme_orig_ack] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp rd_sme_orig_ack"),
							  rule_root->name.c_str());
    }

    // smpp rd_intrmd_ntf
    if(rule_root->val_exists("definition match smpp rd_intrmd_ntf")){
	if(field_valid(rule_root->to_cstr("definition match smpp rd_intrmd_ntf"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp rd_intrmd_ntf"), asn1::ParameterType::_pt_smpp_rd_intermediate_notification);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp rd_intrmd_ntf] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp rd_intrmd_ntf"),
							  rule_root->name.c_str());
    }

    // smpp replace_if_present_flag
    if(rule_root->val_exists("definition match smpp replace_if_present_flag")){
	if(field_valid(rule_root->to_cstr("definition match smpp replace_if_present_flag"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp replace_if_present_flag"), asn1::ParameterType::_pt_smpp_replace_if_present_flag);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp replace_if_present_flag] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp replace_if_present_flag"),
							  rule_root->name.c_str());
    }

    // smpp data_coding
    if(rule_root->val_exists("definition match smpp data_coding")){
	if(field_valid(rule_root->to_cstr("definition match smpp data_coding"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp data_coding"), asn1::ParameterType::_pt_smpp_data_coding);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp data_coding] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp data_coding"),
							  rule_root->name.c_str());
    }

    // smpp sm_default_msg_id
    if(rule_root->val_exists("definition match smpp sm_default_msg_id")){
	if(field_valid(rule_root->to_cstr("definition match smpp sm_default_msg_id"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp sm_default_msg_id"), asn1::ParameterType::_pt_smpp_sm_defaut_msg_id);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sm_default_msg_id] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp sm_default_msg_id"),
							  rule_root->name.c_str());
    }

    // smpp sm_length
    if(rule_root->val_exists("definition match smpp sm_length")){
	if(field_valid(rule_root->to_cstr("definition match smpp sm_length"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp sm_length"), asn1::ParameterType::_pt_smpp_sm_length);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sm_length] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp sm_length"),
							  rule_root->name.c_str());
    }

    // smpp sar_msg_ref_num
    if(rule_root->val_exists("definition match smpp sar_msg_ref_num")){
	if(field_valid(rule_root->to_cstr("definition match smpp sar_msg_ref_num"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp sar_msg_ref_num"), asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sar_msg_ref_num] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp sar_msg_ref_num"),
							  rule_root->name.c_str());
    }

    // smpp sar_total_segments
    if(rule_root->val_exists("definition match smpp sar_total_segments")){
	if(field_valid(rule_root->to_cstr("definition match smpp sar_total_segments"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp sar_total_segments"), asn1::ParameterType::_pt_smpp_sar_total_segments);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sar_total_segments] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp sar_total_segments"),
							  rule_root->name.c_str());
    }

    // smpp sar_segment_seqnum
    if(rule_root->val_exists("definition match smpp sar_segment_seqnum")){
	if(field_valid(rule_root->to_cstr("definition match smpp sar_segment_seqnum"))){
	    prepare_match_line(&tmp_params, rule_root->to_cstr("definition match smpp sar_segment_seqnum"), asn1::ParameterType::_pt_smpp_sar_segment_seqnum);
	    tmp_params.set_bool(RPT_RULE_CTX_SMPP, true);

	}else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [match: smpp sar_segment_seqnum] in rule [%s]!",
							  rule_root->to_cstr("definition match smpp sar_segment_seqnum"),
							  rule_root->name.c_str());
    }


    // *** translate part ***
    // *** set filtering result translate data ***
    if(rule_root->val_exists("definition translate filter_result")){
	if(field_valid(rule_root->to_cstr("definition translate filter_result")))
	    tmp_params.set_int(asn1::ParameterType::_pt_pmink_filter_result, rule_root->to_int("definition translate filter_result"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%d] value for [translate: filter_result] in rule [%s]!",
							  rule_root->to_int("definition translate filter_result"),
							  rule_root->name.c_str());
    }

    // *** set filter jump translate data ***
    if(rule_root->val_exists("definition translate filter_jump")){
	if(field_valid(rule_root->to_cstr("definition translate filter_jump")))
	    tmp_params.set_cstr(RPT_RULE_FJUMP, rule_root->to_cstr("definition translate filter_jump"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: filter_jump] in rule [%s]!",
							  rule_root->to_cstr("definition translate filter_jump"),
							  rule_root->name.c_str());
    }


    // *** set commands translate data ***
    // sri-req map version
    if(rule_root->val_exists("definition translate commands sri-for-sm map version")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm map version"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_gsmmap_version, rule_root->to_cstr("definition translate commands sri-for-sm map version"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cgpa ssn
    if(rule_root->val_exists("definition translate commands sri-for-sm cgpa ssn")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cgpa ssn"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, rule_root->to_cstr("definition translate commands sri-for-sm cgpa ssn"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cgpa gt tt
    if(rule_root->val_exists("definition translate commands sri-for-sm cgpa gt tt")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cgpa gt tt"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, rule_root->to_cstr("definition translate commands sri-for-sm cgpa gt tt"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cgpa gt np
    if(rule_root->val_exists("definition translate commands sri-for-sm cgpa gt np")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cgpa gt np"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, rule_root->to_cstr("definition translate commands sri-for-sm cgpa gt np"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cgpa gt nai
    if(rule_root->val_exists("definition translate commands sri-for-sm cgpa gt nai")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cgpa gt nai"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, rule_root->to_cstr("definition translate commands sri-for-sm cgpa gt nai"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cgpa gt address
    if(rule_root->val_exists("definition translate commands sri-for-sm cgpa gt address")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cgpa gt address"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, rule_root->to_cstr("definition translate commands sri-for-sm cgpa gt address"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cdpa ssn
    if(rule_root->val_exists("definition translate commands sri-for-sm cdpa ssn")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cdpa ssn"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, rule_root->to_cstr("definition translate commands sri-for-sm cdpa ssn"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cdpa gt tt
    if(rule_root->val_exists("definition translate commands sri-for-sm cdpa gt tt")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cdpa gt tt"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, rule_root->to_cstr("definition translate commands sri-for-sm cdpa gt tt"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cdpa gt np
    if(rule_root->val_exists("definition translate commands sri-for-sm cdpa gt np")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cdpa gt np"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, rule_root->to_cstr("definition translate commands sri-for-sm cdpa gt np"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cdpa gt nai
    if(rule_root->val_exists("definition translate commands sri-for-sm cdpa gt nai")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cdpa gt nai"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, rule_root->to_cstr("definition translate commands sri-for-sm cdpa gt nai"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req cdpa gt address
    if(rule_root->val_exists("definition translate commands sri-for-sm cdpa gt address")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm cdpa gt address"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_address, rule_root->to_cstr("definition translate commands sri-for-sm cdpa gt address"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req msisdn nai
    if(rule_root->val_exists("definition translate commands sri-for-sm msisdn nai")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm msisdn nai"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, rule_root->to_cstr("definition translate commands sri-for-sm msisdn nai"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req msisdn np
    if(rule_root->val_exists("definition translate commands sri-for-sm msisdn np")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm msisdn np"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, rule_root->to_cstr("definition translate commands sri-for-sm msisdn np"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req msisdn address
    if(rule_root->val_exists("definition translate commands sri-for-sm msisdn address")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm msisdn address"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_gsmmap_msisdn_digits, rule_root->to_cstr("definition translate commands sri-for-sm msisdn address"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req sca nai
    if(rule_root->val_exists("definition translate commands sri-for-sm sca nai")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm sca nai"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, rule_root->to_cstr("definition translate commands sri-for-sm sca nai"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req sca np
    if(rule_root->val_exists("definition translate commands sri-for-sm sca np")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm sca np"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, rule_root->to_cstr("definition translate commands sri-for-sm sca np"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }

    // sri-req sca address
    if(rule_root->val_exists("definition translate commands sri-for-sm sca address")){
	if(field_valid(rule_root->to_cstr("definition translate commands sri-for-sm sca address"))){
	    tmp_params.set_bool(RPT_RULE_CTX_CMD_SRI_REQ, true);
	    tmp_params.set_cstr(asn1::ParameterType::_pt_gsmmap_sca_digits, rule_root->to_cstr("definition translate commands sri-for-sm sca address"), PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);

	}
    }


    // *** set r14p translate data ***
    // trunk label
    if(rule_root->val_exists("definition translate r14p trunk_label")){
	if(field_valid(rule_root->to_cstr("definition translate r14p trunk_label")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_pmink_trunk_label, rule_root->to_cstr("definition translate r14p trunk_label"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: r14p trunk_label] in rule [%s]!",
							  rule_root->to_cstr("definition translate r14p trunk_label"),
							  rule_root->name.c_str());
    }

    // service id
    if(rule_root->val_exists("definition translate r14p service_id")){
	if(field_valid(rule_root->to_cstr("definition translate r14p service_id")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_pmink_service_id, rule_root->to_cstr("definition translate r14p service_id"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: r14p service_id] in rule [%s]!",
							  rule_root->to_cstr("definition translate r14p service_id"),
							  rule_root->name.c_str());
    }


    // *** set m3ua translate data ***
    // m3ua opc
    if(rule_root->val_exists("definition translate m3ua opc")){
	if(field_valid(rule_root->to_cstr("definition translate m3ua opc")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, rule_root->to_cstr("definition translate m3ua opc"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua opc] in rule [%s]!",
							  rule_root->to_cstr("definition translate m3ua opc"),
							  rule_root->name.c_str());
    }

    // m3ua dpc
    if(rule_root->val_exists("definition translate m3ua dpc")){
	if(field_valid(rule_root->to_cstr("definition translate m3ua dpc")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, rule_root->to_cstr("definition translate m3ua dpc"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua dpc] in rule [%s]!",
							  rule_root->to_cstr("definition translate m3ua dpc"),
							  rule_root->name.c_str());
    }

    // m3ua si
    if(rule_root->val_exists("definition translate m3ua si")){
	if(field_valid(rule_root->to_cstr("definition translate m3ua si")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, rule_root->to_cstr("definition translate m3ua si"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua si] in rule [%s]!",
							  rule_root->to_cstr("definition translate m3ua si"),
							  rule_root->name.c_str());
    }

    // m3ua ni
    if(rule_root->val_exists("definition translate m3ua ni")){
	if(field_valid(rule_root->to_cstr("definition translate m3ua ni")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, rule_root->to_cstr("definition translate m3ua ni"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua ni] in rule [%s]!",
							  rule_root->to_cstr("definition translate m3ua ni"),
							  rule_root->name.c_str());
    }

    // m3ua mp
    if(rule_root->val_exists("definition translate m3ua mp")){
	if(field_valid(rule_root->to_cstr("definition translate m3ua mp")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, rule_root->to_cstr("definition translate m3ua mp"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua mp] in rule [%s]!",
							  rule_root->to_cstr("definition translate m3ua mp"),
							  rule_root->name.c_str());
    }

    // m3ua sls
    if(rule_root->val_exists("definition translate m3ua sls")){
	if(field_valid(rule_root->to_cstr("definition translate m3ua sls")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, rule_root->to_cstr("definition translate m3ua sls"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: m3ua sls] in rule [%s]!",
							  rule_root->to_cstr("definition translate m3ua sls"),
							  rule_root->name.c_str());
    }

    // *** set sccp translate data ***
    // **** calling party ****
    // sccp calling party routing indicator
    if(rule_root->val_exists("definition translate sccp cgpa routing-indicator")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cgpa routing-indicator")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, rule_root->to_cstr("definition translate sccp cgpa routing-indicator"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa routing-indicator] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cgpa routing-indicator"),
							  rule_root->name.c_str());
    }

    // sccp calling party gti
    if(rule_root->val_exists("definition translate sccp cgpa gti")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cgpa gti")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, rule_root->to_cstr("definition translate sccp cgpa gti"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gti] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cgpa gti"),
							  rule_root->name.c_str());
    }

    // sccp calling party ssn
    if(rule_root->val_exists("definition translate sccp cgpa ssn")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cgpa ssn")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, rule_root->to_cstr("definition translate sccp cgpa ssn"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa ssn] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cgpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp calling party point code
    if(rule_root->val_exists("definition translate sccp cgpa point-code")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cgpa ssn")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, rule_root->to_cstr("definition translate sccp cgpa point-code"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa ssn] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cgpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title tt
    if(rule_root->val_exists("definition translate sccp cgpa gt tt")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cgpa gt tt")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, rule_root->to_cstr("definition translate sccp cgpa gt tt"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gt tt] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cgpa gt tt"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title np
    if(rule_root->val_exists("definition translate sccp cgpa gt np")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cgpa gt np")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, rule_root->to_cstr("definition translate sccp cgpa gt np"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gt np] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cgpa gt np"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title nai
    if(rule_root->val_exists("definition translate sccp cgpa gt nai")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cgpa gt nai")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, rule_root->to_cstr("definition translate sccp cgpa gt nai"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gt nai] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cgpa gt nai"),
							  rule_root->name.c_str());
    }

    // sccp calling party global title address
    if(rule_root->val_exists("definition translate sccp cgpa gt address")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cgpa gt address")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, rule_root->to_cstr("definition translate sccp cgpa gt address"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cgpa gt address] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cgpa gt address"),
							  rule_root->name.c_str());
    }

    // **** called party ****
    // sccp called party routing indicator
    if(rule_root->val_exists("definition translate sccp cdpa routing-indicator")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cdpa routing-indicator")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, rule_root->to_cstr("definition translate sccp cdpa routing-indicator"));

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa routing-indicator] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cdpa routing-indicator"),
							  rule_root->name.c_str());
    }

    // sccp called party gti
    if(rule_root->val_exists("definition translate sccp cdpa gti")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cdpa gti")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, rule_root->to_cstr("definition translate sccp cdpa gti"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gti] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cdpa gti"),
							  rule_root->name.c_str());
    }

    // sccp called party ssn
    if(rule_root->val_exists("definition translate sccp cdpa ssn")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cdpa ssn")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, rule_root->to_cstr("definition translate sccp cdpa ssn"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa ssn] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cdpa ssn"),
							  rule_root->name.c_str());
    }

    // sccp called party point code
    if(rule_root->val_exists("definition translate sccp cdpa point-code")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cdpa point-code")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, rule_root->to_cstr("definition translate sccp cdpa point-code"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa point-code] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cdpa point-code"),
							  rule_root->name.c_str());
    }

    // sccp called party global title tt
    if(rule_root->val_exists("definition translate sccp cdpa gt tt")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cdpa gt tt")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, rule_root->to_cstr("definition translate sccp cdpa gt tt"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gt tt] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cdpa gt tt"),
							  rule_root->name.c_str());
    }

    // sccp called party global title np
    if(rule_root->val_exists("definition translate sccp cdpa gt np")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cdpa gt np")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, rule_root->to_cstr("definition translate sccp cdpa gt np"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gt np] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cdpa gt np"),
							  rule_root->name.c_str());
    }

    // sccp called party global title nai
    if(rule_root->val_exists("definition translate sccp cdpa gt nai")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cdpa gt nai")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, rule_root->to_cstr("definition translate sccp cdpa gt nai"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gt nai] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cdpa gt nai"),
							  rule_root->name.c_str());
    }

    // sccp called party global title address
    if(rule_root->val_exists("definition translate sccp cdpa gt address")){
	if(field_valid(rule_root->to_cstr("definition translate sccp cdpa gt address")))
	    tmp_params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_address, rule_root->to_cstr("definition translate sccp cdpa gt address"), PIT_RULE_TRANSLATE_PART);

	else pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Invalid [%s] value for [translate: sccp cdpa gt address] in rule [%s]!",
							  rule_root->to_cstr("definition translate sccp cdpa gt address"),
							  rule_root->name.c_str());
    }

    // *** destinations ***
    build_dests((*rule_root)("route destination"), &tmp_params);

    // check if rule with the same priority already exists
    rule_t* rule = rm->get_rule(rule_root->to_int("priority"));
    if(rule != NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot add rule [%s], rule with priority level [%d] already exists!",
						      rule_root->name.c_str(),
						      rule_root->to_int("priority"));

	return 1;
    }

    // add new rule
    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO,  "Adding rule [%s] with priority level [%d]...",
						  rule_root->name.c_str(),
						  rule_root->to_int("priority"));

    // get daemon
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    // add stats manager
    fgn::StatsManagerRefc* stats_m = new fgn::StatsManagerRefc();
    // save in param map
    tmp_params.set_pointer(2, stats_m, 0, 0, RCT_PMINK);
    // get stats manager
    r14p::R14PStatsSession* r14p_stats = (r14p::R14PStatsSession*)dd->get_param(3);
    // register stats
    stats_m->register_item(fgn::ST_RULE_COUNTER);
    // add trap
    r14p_stats->add_trap(r14p::TrapId(std::string(rule_root->name + "_RULE_COUNTER")), new fgn::StatsHandler(stats_m, fgn::ST_RULE_COUNTER));


    // add rule
    rule_t* new_rule = rm->new_rule(rule_root->name.c_str(), rule_root->to_int("priority"), tmp_params, &flogic);


    PMDLOG(
	new_rule->get_data_p()->set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
	std::cout << "===== RULE: " << new_rule->get_label() << " =====" << std::endl;
	std::cout << *new_rule->get_data_p();
	std::cout << "=====================" << std::endl;
    )

    return 0;
}


// RuleProcessor
fgn::RuleProcessor::RuleProcessor(pmink_utils::MTMasterSlave<list_m_t, true>* list_m_m,
				  pmink_utils::MTMasterSlave<rule_m_t, true>* rule_m_m,
				  pmink_utils::MTMasterSlave<flood_m_t, true>* flood_m_m): rule_resume_index(0),
											   rule_resume_sub_index(-1),
											   rule_resume_loop_index(0){

    list_m.set_master(list_m_m);
    rule_m.set_master(rule_m_m);
    flood_m.set_master(flood_m_m);

    current_pos.index = 0;
    current_pos.sub_index = -1;
    current_pos.loop_index = 0;


}
fgn::RuleProcessor::RuleProcessor(): rule_resume_index(0),
				     rule_resume_sub_index(-1),
				     rule_resume_loop_index(0){

    current_pos.index = 0;
    current_pos.sub_index = -1;
    current_pos.loop_index = 0;

}


bool fgn::RuleProcessor::sync(){
    // sync lists
    list_m.sync();
    // sync flood lists
    flood_m.sync();
    // check if sync needed
    if(rule_m.can_sync()){
	// loop rules
	for(rule_m_t::rules_map_it_t it = (*rule_m).get_begin(); it != (*rule_m).get_end(); it++){
	    // get rule param data
	    rule_param_t* rule_data = it->second.get_data_p();
	    // get stats manager
	    fgn::StatsManagerRefc* stats_m = (fgn::StatsManagerRefc*)(void*)*it->second.get_data_p()->get_param(2, 0, 0, RCT_PMINK);
	    // dec refc
	    stats_m->refc_dec();

	    // loop rule param data
	    for(rule_param_t::it_t r_it = rule_data->get_begin(); r_it != rule_data->get_end(); r_it++){
		// check for index 0 - match part
		if(r_it->first.index == PIT_RULE_MATCH_PART){
		    // check for fragment 1 - match line type
		    if(r_it->first.fragment != 1) continue;
		    // check for lua type
		    if((int)r_it->second == OTT_LUA){
			// get lua state pointer (fragment 3)
			pmink_utils::VariantParam* vp_lua_state = rule_data->get_param(r_it->first.key, r_it->first.index, 3, r_it->first.context);
			// sanity check
			if(vp_lua_state == NULL) continue;
			// free state
			lua_close((lua_State*)(void*)*vp_lua_state);
			// get lua script string pointer (fragment 2)
			pmink_utils::VariantParam* vp_lua_script = rule_data->get_param(r_it->first.key, r_it->first.index, 2, r_it->first.context);
			// sanity check
			if(vp_lua_script == NULL) continue;
			// free
			delete (std::string*)(void*)*vp_lua_script;
		    }

		}else if(r_it->first.index == PIT_RULE_TRANSLATE_PART){
		    // nothing for now
		}

	    }
	}

    }

    // sync rules
    if(rule_m.sync_locked()){
	// reset next rule
	(*rule_m).set_next_rule(NULL);
	// set list_m pointers in rules
	for(rule_m_t::rules_map_it_t it = (*rule_m).get_begin(); it != (*rule_m).get_end(); it++){
	    // set list_m pointers in rule
	    it->second.get_data_p()->set_pointer(0, &list_m, 0, 0, RCT_PMINK);
	    // get stats manager
	    fgn::StatsManagerRefc* stats_m = (fgn::StatsManagerRefc*)(void*)*it->second.get_data_p()->get_param(2, 0, 0, RCT_PMINK);
	    // inc refc
	    stats_m->refc_inc();

	    // get rule param data
	    rule_param_t* rule_data = it->second.get_data_p();
	    // loop rule param data
	    for(rule_param_t::it_t r_it = rule_data->get_begin(); r_it != rule_data->get_end(); r_it++){
		// check for index 0 - match part
		if(r_it->first.index == PIT_RULE_MATCH_PART){
		    // check for fragment 1 - match line type
		    if(r_it->first.fragment != 1) continue;
		    // check for lua type
		    if((int)r_it->second == OTT_LUA){
			// get lua script string (fragment 2)
			pmink_utils::VariantParam* vp_lua_script = rule_data->get_param(r_it->first.key, r_it->first.index, 2, r_it->first.context);
			// sanity check
			if(vp_lua_script == NULL) continue;
			// create state
			lua_State* new_L = luaL_newstate();
			// init libs
			luaL_openlibs(new_L);
			// pre compile chunk (no need to check for result, done already in master)
			std::string* tmp_lua_script_str = new std::string(*((std::string*)(void*)*vp_lua_script));
			luaL_loadstring(new_L, tmp_lua_script_str->c_str());
			// save state (fragment 3)
			rule_data->set_pointer(r_it->first.key, new_L, r_it->first.index, 3, r_it->first.context);
			// replace lua script string pointer (fragment 2), not sharing this between slaves and master
			rule_data->set_pointer(r_it->first.key, tmp_lua_script_str, r_it->first.index, 2, r_it->first.context);

		    }

		}else if(r_it->first.index == PIT_RULE_TRANSLATE_PART){
		    // nothing for now
		}

	    }
	}

	// unlock
	rule_m.unlock();

    }

    return true;
}


// FilteringLogic
int fgn::FilteringLogic::find_set_param(const char* cfg_name,
					rule_param_t* input,
					rule_param_t* output,
					pmink_utils::VariantParamType vp_type,
					uint32_t vp_id,
					uint32_t vp_index,
					uint32_t vp_context){
    // sanity check
    if(output == NULL || cfg_name == NULL || strlen(cfg_name) < 1) return 1;
    // vars
    pmink_utils::VariantParam* vp = NULL;
    // check if regex
    if(cfg_name[0] == ':'){
	// sanity checks
	if(strlen(cfg_name) < 5) return 1;
	if(cfg_name[1] != '/') return 1;
	// tmp vars
	char* tmp;
	char* token_match;
	char* token_replace;
	char tmp_copy[256];
	// sanity check
	if(strlen(&cfg_name[1]) + 1 > sizeof(tmp_copy)) return 1;
	// make c str copy
	strcpy(tmp_copy, &cfg_name[1]);
	// tokenize
	// match part
	token_match = strtok_r(tmp_copy, "/", &tmp);
	// error check
	if(token_match == NULL) return 1;
	// replace part
	token_replace = strtok_r(NULL, "/", &tmp);
	// error check
	if(token_replace == NULL) return 1;
	// get from input
	vp = input->get_param(vp_id);
	// sanity check
	if(vp == NULL) return 1;
	// tmp data
	char tmp_data[16];
	char* data_p = NULL;
	// check type (ony int and string used)
	switch(vp->get_type()){
	    case pmink_utils::DPT_INT:
		// convert int to string
		snprintf(tmp_data, sizeof(tmp_data), "%d", (int)*vp);
		// set pointer
		data_p = tmp_data;
		break;

	    case pmink_utils::DPT_STRING:
		// set pointer
		data_p = (char*)*vp;
		break;

	    default: return 1;
	}

	// run regex
	try{
	    // match regex
	    boost::regex rgx(token_match);
	    // replace
	    std::string rpl_str = boost::regex_replace(std::string(data_p), rgx, token_replace);
	    // check type (ony int and string used)
	    switch(vp_type){
		case pmink_utils::DPT_INT:
		    output->set_int(vp_id, atoi(rpl_str.c_str()), vp_index, 0, vp_context);
		    return 0;

		case pmink_utils::DPT_STRING:
		    output->set_cstr(vp_id, rpl_str.c_str(), vp_index, 0, vp_context);
		    return 0;

		default: return 1;
	    }

	// invalid regex pattern
	}catch(std::exception& e){
	    return 1;
	}

    // check if var value
    }else if(cfg_name[0] == '@'){
	// sanity check
	if(strlen(cfg_name) < 2) return 0;
	// find cfg mapping
	uint32_t hash = pmink_utils::hash_fnv1a(&cfg_name[1], strlen(cfg_name) - 1);
	std::map<uint32_t, CfgTranslationItem*>::iterator it = cfg_trans_map.find(hash);
	if(it == cfg_trans_map.end()) return 1;
	// check if const of var
	switch(it->second->type){
	    case CTMT_CONST:
		// check output type
		switch(vp_type){
		    case pmink_utils::DPT_INT:
			output->set_int(vp_id, it->second->value, vp_index, 0, vp_context);
			return 0;

		    case pmink_utils::DPT_STRING:
			// convert int to string
			char tmp_str[256];
			snprintf(tmp_str, sizeof(tmp_str), "%d", it->second->value);
			output->set_cstr(vp_id, tmp_str, vp_index, 0, vp_context);
			return 0;

		    default: return 1;
		}
		break;

	    case CTMT_VAR:
	    {
		// get from input
		vp = input->get_param(it->second->value);
		// sanity check
		if(vp == NULL) return 1;
		// check var type (ony int and string used)
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// check if conversion needed
			if(vp_type == pmink_utils::DPT_INT) output->set_int(vp_id, (int)*vp, vp_index, 0, vp_context);
			else if(vp_type == pmink_utils::DPT_STRING){
			    // convert int to string
			    char tmp_str[256];
			    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			    output->set_cstr(vp_id, tmp_str, vp_index, 0, vp_context);

			}
			return 0;

		    case pmink_utils::DPT_STRING:
			// check if conversion needed
			if(vp_type == pmink_utils::DPT_STRING) output->set_cstr(vp_id, (char*)*vp, vp_index, 0, vp_context);
			// convert to int
			else if(vp_type == pmink_utils::DPT_INT) output->set_int(vp_id, atoi((char*)*vp), vp_index, 0, vp_context);
			return 0;

		    default: return 1;
		}
		break;
	    }

	    case CTMT_DYNAMIC:
	    {
		// check if conversion needed
		if(vp_type == pmink_utils::DPT_INT) output->set_int(vp_id, it->second->run(), vp_index, 0, vp_context);
		else if(vp_type == pmink_utils::DPT_STRING){
		    // convert int to string
		    char tmp_str[256];
		    snprintf(tmp_str, sizeof(tmp_str), "%d", it->second->run());
		    output->set_cstr(vp_id, tmp_str, vp_index, 0, vp_context);

		}

		break;
	    }
	}



    // literal value
    }else{
	// check type (ony int and string used)
	switch(vp_type){
	    case pmink_utils::DPT_INT:
		output->set_int(vp_id, atoi(cfg_name), vp_index, 0, vp_context);
		return 0;

	    case pmink_utils::DPT_STRING:
		output->set_cstr(vp_id, cfg_name, vp_index, 0, vp_context);
		return 0;

	    default: return 1;
	}

    }

    // return
    return 1;
}

bool fgn::FilteringLogic::data_match(const char* data1, const char* data2, RuleOperatorType op_t){
    // sanity check
    if(data1 == NULL || data2 == NULL) return false;
    // check operator
    switch(op_t){
	// ==
	case ROT_EQ:
	    return (strcmp(data1, data2) == 0);

	// >
	case ROT_GT:
	    return (atoi(data1) > atoi(data2));

	// >=
	case ROT_GTE:
	    return (atoi(data1) >= atoi(data2));

	// <
	case ROT_LT:
	    return (atoi(data1) < atoi(data2));

	// <=
	case ROT_LTE:
	    return (atoi(data1) <= atoi(data2));

	// !=
	case ROT_NE:
	    return (strcmp(data1, data2) != 0);

	default: break;
    }

    // default
    return false;
}


fgn::RuleOperatorType fgn::FilteringLogic::op_detect(const char* pattern){
    // sanity checks
    if(pattern == NULL) return ROT_EQ;
    if(strlen(pattern) == 0) return ROT_EQ;
    // match
    if(strcmp(pattern, "==") == 0) return ROT_EQ;
    else if(strcmp(pattern, "!=") == 0) return ROT_NE;
    else if(strcmp(pattern, ">") == 0) return ROT_GT;
    else if(strcmp(pattern, ">=") == 0) return ROT_GTE;
    else if(strcmp(pattern, "<") == 0) return ROT_LT;
    else if(strcmp(pattern, "<=") == 0) return ROT_LTE;
    else if(strcmp(pattern, ">>") == 0) return ROT_LST_ADD;
    else if(strcmp(pattern, "<<") == 0) return ROT_LST_DEL;
    else if(strcmp(pattern, "--") == 0) return ROT_LST_CLR;
    // default
    return ROT_EQ;
}

bool fgn::FilteringLogic::match(const char* data, 
                                rule_t* rule, 
                                rule_param_t* input, 
                                uint32_t vp_id, 
                                bool skip_lciop, 
                                uint32_t context, 
                                uint32_t vp_index, 
                                int* default_res_override,
                                bool script_args){
    // sanity check
    if(data == NULL || rule == NULL) return false;
    // rule params
    rule_param_t* rparam = rule->get_data_p();
    // check operand type
    pmink_utils::VariantParam* vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 1, context);
    // sanity check
    if(vp == NULL) return false;
    // default operator ==
    RuleOperatorType op_t = ROT_EQ;
    unsigned int l_ciop = 0;
    unsigned int r_ciop = 0;
    char* operand = NULL;
    lua_State* L = NULL;

    // check
    switch((int)*vp){
	// number operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operator
	// 4 - operand
	case OTT_NUMBER:
	    // lciop
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operator
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 3, context);
	    op_t = (RuleOperatorType)(int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 4, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			char tmp_str[256];
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			return data_match(tmp_str, operand, op_t);

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }
	    // match
	    return data_match(data, operand, op_t);

	// string operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operator
	// 4 - operand
	case OTT_STRING:
	    // lciop
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operator
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 3, context);
	    op_t = (RuleOperatorType)(int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 4, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			char tmp_str[256];
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			return data_match(tmp_str, operand, op_t);

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }

	    // match
	    return data_match(data, operand, op_t);

	// regex operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operand
	case OTT_REGEX:
	{
	    char tmp_str[256];
	    // lciop
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 3, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			data = tmp_str;
			break;

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }

	    // run regex
	    try{
		// user regex
		boost::regex usr_rgx(operand);
		// return result
		return boost::regex_match(data, usr_rgx);

	    // invalid regex pattern
	    }catch(std::exception& e){
		return false;
	    }
	    break;
	}

	// variable operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operator
	// 4 - right component index operator
	// 5 - operand
	case OTT_VARIABLE:
	{
	    char tmp_str[256];
	    // lciop
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operator
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 3, context);
	    op_t = (RuleOperatorType)(int)*vp;
	    // rciop
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 4, context);
	    r_ciop = (int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 5, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			data = tmp_str;
			break;

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }

	    // find cfg mapping
	    uint32_t hash = pmink_utils::hash_fnv1a(operand, strlen(operand));
	    std::map<uint32_t, CfgTranslationItem*>::iterator it = cfg_trans_map.find(hash);
	    if(it == cfg_trans_map.end()) return false;
	    // check if const of var
	    switch(it->second->type){
		case CTMT_CONST:
		{
		    // match const
		    char tmp_str[16];
		    snprintf(tmp_str, sizeof(tmp_str), "%d", it->second->value);
		    // normal match
		    return data_match(data, tmp_str, op_t);
		}
		case CTMT_VAR:
		{
		    // get from input
		    pmink_utils::VariantParam* vp = input->get_param(it->second->value, r_ciop);
		    // sanity check
		    if(vp == NULL) return false;
		    // check type (ony int and string used)
		    switch(vp->get_type()){
			case pmink_utils::DPT_INT:
			    // convert int to string
			    char tmp_str[16];
			    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			    // normal match
			    return data_match(data, tmp_str, op_t);

			case pmink_utils::DPT_STRING:
			    // normal match
			    return data_match(data, (char*)*vp, op_t);

			default: return 1;
		    }

		    break;
		}

		case CTMT_DYNAMIC:
		{
		    // match const
		    char tmp_str[16];
		    snprintf(tmp_str, sizeof(tmp_str), "%d", it->second->run());
		    // normal match
		    return data_match(data, tmp_str, op_t);
		}
	    }

	    break;
	}

	// list operand
	// 1 - type
	// 2 - left component index operator
	// 3 - operator
	// 4 - operand
	case OTT_LIST:
	{
	    char tmp_str[256];
	    // lciop
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 2, context);
	    l_ciop = (int)*vp;
	    // operator
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 3, context);
	    op_t = (RuleOperatorType)(int)*vp;
	    // operand
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 4, context);
	    operand = (char*)*vp;
	    // check if lciop different then default 0
	    if(!skip_lciop && l_ciop > 0){
		vp = input->get_param(vp_id, l_ciop);
		// sanity check
		if(vp == NULL) return false;
		// check type
		switch(vp->get_type()){
		    case pmink_utils::DPT_INT:
			// convert int to string
			snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp);
			data = tmp_str;
			break;

		    case pmink_utils::DPT_STRING:
			data = (char*)*vp;
			break;

		    default: break;
		}
	    }
	    // get list slave pointer
	    list_slv_t* list_slv_p = (list_slv_t*)(void*)*rparam->get_param(0, 0, 0, RCT_PMINK);
	    // get slave list_m ef
	    list_m_t& list_m_ref = **list_slv_p;
	    //list_m_t* list_m_p = (list_m_t*)(void*)rparam->get_param(0, 0, 0, RCT_PMINK);
	    //if(list_m_p == NULL) return false;

	    // *** special list add >> operator ***
	    if(op_t == ROT_LST_ADD){
		// *** current slave list ***
		// get/add list
		list_m_t::list_it_t list_it = list_m_ref.add_list(operand);
		// add to list
		list_m_ref.add_sublist_item(list_it, data);

		// *** main master list ***
		list_mstr_t* list_mstr_p = list_slv_p->get_master();
		list_m_t* mstr_list_m_p = &(**list_mstr_p);
		// update master
		list_mstr_p->update_start();
		list_it = mstr_list_m_p->add_list(operand);
		mstr_list_m_p->add_sublist_item(list_it, data);
		list_mstr_p->update_end();
		// ok
		return true;

	    // *** special list add << operator ***
	    }else if(op_t == ROT_LST_DEL){
		// *** current slave list ***
		// remove from slave list
		list_m_ref.del_sublist_item(operand, data);
		// *** main master list ***
		list_mstr_t* list_mstr_p = list_slv_p->get_master();
		list_m_t* mstr_list_m_p = &(**list_mstr_p);
		// update master
		list_mstr_p->update_start();
		mstr_list_m_p->del_sublist_item(operand, data);
		list_mstr_p->update_end();
		// ok
		return true;

	    // *** special list add -- operator ***
	    }else if(op_t == ROT_LST_CLR){
		// *** current slave list ***
		// remove list
		list_m_ref.remove_list(operand);
		// *** main master list ***
		list_mstr_t* list_mstr_p = list_slv_p->get_master();
		list_m_t* mstr_list_m_p = &(**list_mstr_p);
		// update master
		list_mstr_p->update_start();
		mstr_list_m_p->remove_list(operand);
		list_mstr_p->update_end();
		// ok
		return true;
	    }

	    // *** other list operators (== and !=)
	    // find item
	    std::string* lst_item = list_m_ref.get_sublist_item(operand, data);
	    // not found in list
	    if(lst_item == NULL){
		// if != return true
		if(op_t == ROT_NE) return true;
		// else (==) return false
		return false;
	    }
	    // found in list
	    // if == return true
	    if(op_t == ROT_EQ) return true;
	    // else (!=) return false
	    else return false;
	}

	#ifdef FLOGIC_PAUSE_RESUME
	// list operand
	// 1     - type
	// 2     - lua script
	// 3     - lua state
        // 4     - lua script argument count
        // >=100 - lua script arguments
	case OTT_LUA:
	{
	    // get state
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 3, context);
	    L = (lua_State*)(void*)*vp;
	    // get fgn payload pointer
	    vp = input->get_param(1, 0, 0, RCT_PMINK);
	    FgnPayload* fgn_pld = (FgnPayload*)(void*)*vp;
	    // set current input vp id and index
	    fgn_pld->current_vp_id.key = vp_id;
	    fgn_pld->current_vp_id.index = vp_index;
	    // lua result
	    int lua_res = 0;
	    // check if resuming
	    if(lua_status(L) == LUA_YIELD){
                PMDLOG(
                    std::cout << "[LUA]: Resuming from LUA_YIELD..." << std::endl;
                    lua_utils::lua_stack_dump(L);
                )
		lua_res = lua_resume(L, lua_gettop(L));

	    // starting for the first time
	    }else{
		// copy precompiled lua chunk (to have another copy prepared when lua_pcall/lua_resume removes it)
		lua_pushvalue(L, -1);
		// push payload pointer
		lua_pushlightuserdata(L, (void*)*vp);
                // script argument count (defaults to zero)
                int argc = 0;
                // args
                if(script_args){
                    // get arg count
                    pmink_utils::VariantParam* vp_arg_c = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 4, context);
                    if(vp_arg_c != NULL) argc = (int)*vp_arg_c;
                    // if arguments exist, create lua table
                    if(argc > 0){
                        // create lua table
                        lua_newtable(L);
                        // loop args
                        for(int i = 0; i<argc; i++){
                            // get argument name and value
                            pmink_utils::VariantParam* vp_arg_n = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 100 + (i * 2), context);
                            pmink_utils::VariantParam* vp_arg_v = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 100 + (i * 2) + 1, context);
                            // sanity check
                            if(vp_arg_n == NULL || vp_arg_c == NULL) continue;
                            // push to lua stack
                            lua_pushstring(L, (char*)*vp_arg_n);
                            lua_pushstring(L, (char*)*vp_arg_v);
                            // add to table
                            lua_settable(L, -3);
                        } 
                    }
                }
                PMDLOG(
                    std::cout << "[LUA]: Starting..." << std::endl;
                    lua_utils::lua_stack_dump(L);
                )
		// run script
		lua_res = lua_resume(L, (argc > 0 ? 2 : 1));
	    }

	    // normal exit
	    if(lua_res == 0){
                PMDLOG(
                    std::cout << "[LUA]: Finished without errors..." << std::endl;
                    lua_utils::lua_stack_dump(L);
                )
		// boolean res
		if(lua_isboolean(L, -1)) lua_res = (bool)lua_toboolean(L, -1);
		// int res
		else if(lua_isnumber(L, -1)) lua_res = (int)lua_tointeger(L, -1);
		// unknown res
		else lua_res = 0;
		// get num of item on stack
		int sc = lua_gettop(L);
		// remove all except precompiled lua chunk
		if(sc > 1) lua_pop(L, sc - 1);
                PMDLOG(
                    std::cout << "[LUA]: Cleanup completed..." << std::endl;
                    lua_utils::lua_stack_dump(L);
                )

	    // script error
	    }else if(lua_res > LUA_YIELD){
		pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Lua error: %s", lua_tostring(L, -1));
		lua_res = 0;
		// close state
		lua_close(L);
		// create new state
		L = luaL_newstate();
		// init libs
		luaL_openlibs(L);
		// get lua script string (fragment 2)
		pmink_utils::VariantParam* vp_lua_script = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 2, context);
		// load pre compiled chunk (no need to check for result, done already in master)
		luaL_loadstring(L, ((std::string*)(void*)*vp_lua_script)->c_str());
		// save state (fragment 3)
		rparam->set_pointer(vp_id, L, PIT_RULE_MATCH_PART, 3, context);


	    // script paused
	    }else{
                PMDLOG(
                    std::cout << "[LUA]: Paused..." << std::endl;
                    lua_utils::lua_stack_dump(L);
                )
		// set resume point
		fgn_pld->rproc->rule_resume_index = fgn_pld->rproc->current_pos.index;
		fgn_pld->rproc->rule_resume_sub_index = fgn_pld->rproc->current_pos.sub_index;
		fgn_pld->rproc->rule_resume_loop_index = fgn_pld->rproc->current_pos.loop_index;
		// override default_res in rule logic
		*default_res_override = pmink_utils::RRT_CONTINUE_PAUSE;
		// return false to stop rule logic
		lua_res = 0;
		// set nex rule to currnet tule
		rule->get_rm()->set_next_rule(rule);

	    }

	    // result
	    return (lua_res > 0 ? true : false);


	}
	#else
	// list operand
	// 1 - type
	// 2 - lua script
	// 3 - lua state
	case OTT_LUA:
	{
	    // get state
	    vp = rparam->get_param(vp_id, PIT_RULE_MATCH_PART, 3, context);
	    L = (lua_State*)(void*)*vp;
	    // get fgn payload pointer
	    vp = input->get_param(1, 0, 0, RCT_PMINK);
	    FgnPayload* fgn_pld = (FgnPayload*)(void*)*vp;
	    // set current input vp id and index
	    fgn_pld->current_vp_id.key = vp_id;
	    fgn_pld->current_vp_id.index = vp_index;
	    // copy precompiled lua chunk (to have another copy perpared when lua_pcall removes it)
	    lua_pushvalue(L, -1);
	    // push payload pointer
	    lua_pushlightuserdata(L, (void*)*vp);
	    // run script
	    int lua_res = lua_pcall(L, 1, 1, 0);
	    if(lua_res == 0){
		// boolean res
		if(lua_isboolean(L, -1)) lua_res = (bool)lua_toboolean(L, -1);
		// int res
		else if(lua_isnumber(L, -1)) lua_res = (int)lua_tointeger(L, -1);
		// unknown res
		else lua_res = 0;

	    // script error
	    }else{
		pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Lua error: %s", lua_tostring(L, -1));
		lua_res = 0;
	    }
	    // pop result or error message
	    lua_pop(L, 1);
	    // result
	    return (lua_res > 0 ? true : false);
	}
	#endif
    }

    return false;
}

int fgn::FilteringLogic::generate_sri_sm_req(rule_param_t* params){
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    // *** send to stp ***
    // get pld pointer
    FgnPayload* pld = (FgnPayload*)(void*)*params->get_param(1, 0, 0, RCT_PMINK);
    // use cmd params
    rule_param_t* cmd_params = &pld->cmd_params;
    cmd_params->clear_params();

    // gsm map version
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_version, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_version, 2));

    // msisdn nai
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, 2));

    // msisdn np
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, 2));

    // msisdn address
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_digits, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_digits, 2));

    // sca nai
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, 2));

    // sca np
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, 2));

    // sca address
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_digits, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_digits, 2));


    // called ssn
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, 2));

    // called gt tt
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, 2));

    // called gt np
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, 2));

    // called gt nai
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, 2));

    // called gt address
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_address, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_address, 2));

    // calling ssn
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, 2));

    // calling gt tt
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, 2));

    // calling gt np
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, 2));

    // calling gt nai
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, 2));

    // calling gt address
    cmd_params->set(*params,
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, PIT_INPUT_CMD_REQ_PART),
		    pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, 2));


    // remove result part (from previous invokation)
    params->erase_param(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_an_type_of_number, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_an_digits, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_gsmmap_imsi, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, PIT_INPUT_CMD_RPL_PART);
    params->erase_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, PIT_INPUT_CMD_RPL_PART);


    // set pmink command id
    cmd_params->set_int(asn1::ParameterType::_pt_pmink_command_id, asn1::SmsHubCommandId::_shci_sri_sm_req);
    // request notification
    cmd_params->set_bool(asn1::ParameterType::_pt_pmink_correlation_notification, true);
    // set guid
    cmd_params->set_octets(asn1::ParameterType::_pt_pmink_guid, &pld->guid, sizeof(pld->guid));
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


int fgn::FilteringLogic::run(rule_t* rule, rule_param_t* input){
    // computed goto labels
    #ifdef FLOGIC_PAUSE_RESUME
    static void* lbls[] = { &&rlbl_0,  &&rlbl_1,  &&rlbl_2,  &&rlbl_3,  &&rlbl_4,  &&rlbl_5,
			    &&rlbl_6,  &&rlbl_7,  &&rlbl_8,  &&rlbl_9,  &&rlbl_10, &&rlbl_11,
			    &&rlbl_12, &&rlbl_13, &&rlbl_14, &&rlbl_15, &&rlbl_16, &&rlbl_17,
			    &&rlbl_18, &&rlbl_19, &&rlbl_20, &&rlbl_21, &&rlbl_22, &&rlbl_23,
			    &&rlbl_24, &&rlbl_25, &&rlbl_26, &&rlbl_27, &&rlbl_28, &&rlbl_29,
			    &&rlbl_30, &&rlbl_31, &&rlbl_32, &&rlbl_33, &&rlbl_34, &&rlbl_35,
			    &&rlbl_36, &&rlbl_37, &&rlbl_38, &&rlbl_39, &&rlbl_40, &&rlbl_41,
			    &&rlbl_42, &&rlbl_43, &&rlbl_44, &&rlbl_45, &&rlbl_46, &&rlbl_47,
			    &&rlbl_48, &&rlbl_49, &&rlbl_50, &&rlbl_51, &&rlbl_52, &&rlbl_53,
			    &&rlbl_54, &&rlbl_55, &&rlbl_56, &&rlbl_57, &&rlbl_58, &&rlbl_59,
			    &&rlbl_60, &&rlbl_61, &&rlbl_62,
			    &&rlbl_63,
				&&rlbl_63_0, &&rlbl_63_1, &&rlbl_63_2, &&rlbl_63_3, &&rlbl_63_4,  &&rlbl_63_5,
				&&rlbl_63_6, &&rlbl_63_7, &&rlbl_63_8, &&rlbl_63_9, &&rlbl_63_10, &&rlbl_63_11,
				&&rlbl_63_12,
			    &&rlbl_64,
				&&rlbl_64_0, &&rlbl_64_1, &&rlbl_64_2, &&rlbl_64_3, &&rlbl_64_4, &&rlbl_64_5,
				&&rlbl_64_6, &&rlbl_64_7, &&rlbl_64_8, &&rlbl_64_9,
			    &&rlbl_65,
				&&rlbl_65_0,  &&rlbl_65_1,  &&rlbl_65_2,  &&rlbl_65_3,  &&rlbl_65_4,  &&rlbl_65_5,
				&&rlbl_65_6,  &&rlbl_65_7,  &&rlbl_65_8,  &&rlbl_65_9,  &&rlbl_65_10, &&rlbl_65_11,
				&&rlbl_65_12, &&rlbl_65_13, &&rlbl_65_14, &&rlbl_65_15, &&rlbl_65_16, &&rlbl_65_17,
				&&rlbl_65_18, &&rlbl_65_19, &&rlbl_65_20, &&rlbl_65_21, &&rlbl_65_22,
			    &&rlbl_66,
				&&rlbl_66_0,  &&rlbl_66_1,  &&rlbl_66_2,  &&rlbl_66_3,  &&rlbl_66_4,  &&rlbl_66_5,
				&&rlbl_66_6,  &&rlbl_66_7,  &&rlbl_66_8,  &&rlbl_66_9,  &&rlbl_66_10, &&rlbl_66_11,
				&&rlbl_66_12, &&rlbl_66_13, &&rlbl_66_14, &&rlbl_66_15, &&rlbl_66_16, &&rlbl_66_17,
				&&rlbl_66_18, &&rlbl_66_19, &&rlbl_66_20, &&rlbl_66_21, &&rlbl_66_22, &&rlbl_66_23,
				&&rlbl_66_24, &&rlbl_66_25,
			    &&lbl_translate, &&lbl_exit


    };
    #endif
    // sanity check
    if(rule == NULL || input == NULL) return pmink_utils::RRT_STOP_ERR;
    // debug
    PMDLOG(
	std::cout << "===== RUNNING RULE: " << rule->get_label() << " =====" << std::endl;
    )

    // clear jump value
    rule->get_rm()->set_next_rule(NULL);
    // vars
    pmink_utils::VariantParam* vp_spec = NULL;
    pmink_utils::VariantParam* vp_data = NULL;
    pmink_utils::VariantParam* vp_tmp = NULL;
    char tmp_str[256];
    bool res = true;
    rule_param_t* rparams = rule->get_data_p();
    bool ctx_sri = false;
    bool ctx_sm = false;
    bool ctx_smstpdu = false;
    bool ctx_smpp = false;
    bool ctx_cmd_sri_req = false;
    int rule_resume_index = 0;
    int rule_resume_sub_index = -1;
    int rule_resume_loop_index = 0;
    unsigned int cc = 0;
    bool cc_res = false;

    // get default filter res
    int default_res = pmink_utils::RRT_STOP_OK;
    vp_tmp = rparams->get_param(RPT_RULE_DEFAULT_FRES);
    if(vp_tmp != NULL) default_res = (int)*vp_tmp;
    // get sri-for-sm context
    vp_tmp = rparams->get_param(RPT_RULE_CTX_SRI);
    if(vp_tmp != NULL) ctx_sri = (bool)*vp_tmp;
    // get sm context
    vp_tmp = rparams->get_param(RPT_RULE_CTX_SM);
    if(vp_tmp != NULL) ctx_sm = (bool)*vp_tmp;
    // get smstpdu context
    vp_tmp = rparams->get_param(RPT_RULE_CTX_SMSTPDU);
    if(vp_tmp != NULL) ctx_smstpdu = (bool)*vp_tmp;
    // get smpp context
    vp_tmp = rparams->get_param(RPT_RULE_CTX_SMPP);
    if(vp_tmp != NULL) ctx_smpp = (bool)*vp_tmp;
    // get cmd sri-for-sm request context
    vp_tmp = rparams->get_param(RPT_RULE_CTX_CMD_SRI_REQ);
    if(vp_tmp != NULL) ctx_cmd_sri_req = (bool)*vp_tmp;
    // get fgn payload pointer
    vp_tmp = input->get_param(1, 0, 0, RCT_PMINK);
    FgnPayload* fgn_pld = ((FgnPayload*)(void*)*vp_tmp);
    // get rule resume index
    rule_resume_index = fgn_pld->rproc->rule_resume_index;
    // get rule resume sub index
    rule_resume_sub_index = fgn_pld->rproc->rule_resume_sub_index;
    // get rule resume loop index
    rule_resume_loop_index = fgn_pld->rproc->rule_resume_loop_index;
    // erase resume data
    fgn_pld->rproc->rule_resume_index = 0;
    fgn_pld->rproc->rule_resume_sub_index = -1;
    fgn_pld->rproc->rule_resume_loop_index = 0;
    // reset current pos
    fgn_pld->rproc->current_pos.index = 0;
    fgn_pld->rproc->current_pos.sub_index = -1;
    fgn_pld->rproc->current_pos.loop_index = 0;


    // inc rule counter (stats manager is shared between master and slave rule processors, atomics only)
    pmink_utils::StatsManager* stats_m = (pmink_utils::StatsManager*)(void*)*rule->get_data_p()->get_param(2, 0, 0, RCT_PMINK);
    stats_m->inc(fgn::ST_RULE_COUNTER, 1);

    // resume/start
    #ifdef FLOGIC_PAUSE_RESUME
    goto *lbls[rule_resume_index];
    #endif

    // *** dummy ***
    rlbl_0:
    fgn_pld->rproc->current_pos.index = 0;
    vp_spec = rparams->get_param(0, PIT_RULE_MATCH_PART, 2, RCT_DUMMY);
    if(vp_spec != NULL) res &= match(PMINK_LUA_DUMMY_STR, rule, input, 0, true, RCT_DUMMY, 0, &default_res, true);
    if(!res) return default_res;


    // *** special commands match data ***
    // *** sri-for-sm ***

    // sri-for-sm rpl cgpa ssn
    rlbl_1:
    fgn_pld->rproc->current_pos.index = 1;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl cgpa gt tt
    rlbl_2:
    fgn_pld->rproc->current_pos.index = 2;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl cgpa gt np
    rlbl_3:
    fgn_pld->rproc->current_pos.index = 3;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl cgpa gt nai
    rlbl_4:
    fgn_pld->rproc->current_pos.index = 4;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl cgpa gt address
    rlbl_5:
    fgn_pld->rproc->current_pos.index = 5;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_address, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl cdpa ssn
    rlbl_6:
    fgn_pld->rproc->current_pos.index = 6;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl cdpa gt tt
    rlbl_7:
    fgn_pld->rproc->current_pos.index = 7;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl cdpa gt np
    rlbl_8:
    fgn_pld->rproc->current_pos.index = 8;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl cdpa gt nai
    rlbl_9:
    fgn_pld->rproc->current_pos.index = 9;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl cdpa gt address
    rlbl_10:
    fgn_pld->rproc->current_pos.index = 10;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_sccp_called_pa_gt_address, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl imsi
    rlbl_11:
    fgn_pld->rproc->current_pos.index = 11;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_imsi, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_imsi, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_imsi, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl nnn nai
    rlbl_12:
    fgn_pld->rproc->current_pos.index = 12;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl nnn np
    rlbl_13:
    fgn_pld->rproc->current_pos.index = 13;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl nnn address
    rlbl_14:
    fgn_pld->rproc->current_pos.index = 14;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_nnn_digits, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl an nai
    rlbl_15:
    fgn_pld->rproc->current_pos.index = 15;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_an_type_of_number, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_an_type_of_number, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_an_type_of_number, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl an np
    rlbl_16:
    fgn_pld->rproc->current_pos.index = 16;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_an_numbering_plan, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl an address
    rlbl_17:
    fgn_pld->rproc->current_pos.index = 17;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_an_digits, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_an_digits, PIT_INPUT_CMD_RPL_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_an_digits, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_RPL_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl msisdn nai
    rlbl_18:
    fgn_pld->rproc->current_pos.index = 18;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, PIT_INPUT_CMD_REQ_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_REQ_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl msisdn np
    rlbl_19:
    fgn_pld->rproc->current_pos.index = 19;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, PIT_INPUT_CMD_REQ_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_REQ_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl msisdn address
    rlbl_20:
    fgn_pld->rproc->current_pos.index = 20;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, PIT_INPUT_CMD_REQ_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_msisdn_digits, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_REQ_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl sca nai
    rlbl_21:
    fgn_pld->rproc->current_pos.index = 21;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, PIT_INPUT_CMD_REQ_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_sca_type_of_number, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_REQ_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl sca np
    rlbl_22:
    fgn_pld->rproc->current_pos.index = 22;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, PIT_INPUT_CMD_REQ_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_REQ_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // sri-for-sm rpl sca address
    rlbl_23:
    fgn_pld->rproc->current_pos.index = 23;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_digits, PIT_RULE_MATCH_PART, 2, RCT_CMD_SRI_RPL);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_sca_digits, PIT_INPUT_CMD_REQ_PART/*(int)*vp_spec*/);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_sca_digits, true, RCT_CMD_SRI_RPL, PIT_INPUT_CMD_REQ_PART, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;


    // *** r14p match data ***
    // trunk label
    rlbl_24:
    fgn_pld->rproc->current_pos.index = 24;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_trunk_label, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_trunk_label, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_pmink_trunk_label, true, 0, 0, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // service id
    rlbl_25:
    fgn_pld->rproc->current_pos.index = 25;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_service_id, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_service_id, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_pmink_service_id, true, 0, 0, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // src type
    rlbl_26:
    fgn_pld->rproc->current_pos.index = 26;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_daemon_type, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_daemon_type, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_pmink_daemon_type, true, 0, 0, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // src id
    rlbl_27:
    fgn_pld->rproc->current_pos.index = 27;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_daemon_id, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_daemon_id, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_pmink_daemon_id, true, 0, 0, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // cmd id
    rlbl_28:
    fgn_pld->rproc->current_pos.index = 28;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_command_id, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_command_id, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_pmink_command_id, true, 0, 0, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // connection type
    rlbl_29:
    fgn_pld->rproc->current_pos.index = 29;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_connection_type, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_connection_type, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_pmink_connection_type, true, 0, 0, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // loop count
    rlbl_30:
    fgn_pld->rproc->current_pos.index = 30;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_loop_count, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_pmink_loop_count, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_pmink_loop_count, true, 0, 0, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // *** m3ua match data ***
    // m3ua opc
    rlbl_31:
    fgn_pld->rproc->current_pos.index = 31;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, true, 0, 0, &default_res);
	}else res &= false;

    }
    if(!res) return default_res;

    // m3ua dpc
    rlbl_32:
    fgn_pld->rproc->current_pos.index = 32;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // m3ua si
    rlbl_33:
    fgn_pld->rproc->current_pos.index = 33;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // m3ua ni
    rlbl_34:
    fgn_pld->rproc->current_pos.index = 34;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // m3ua mp
    rlbl_35:
    fgn_pld->rproc->current_pos.index = 35;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // m3ua sls
    rlbl_36:
    fgn_pld->rproc->current_pos.index = 36;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // m3ua as
    rlbl_37:
    fgn_pld->rproc->current_pos.index = 37;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_as_label, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_as_label, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_m3ua_as_label, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // m3ua asp
    rlbl_38:
    fgn_pld->rproc->current_pos.index = 38;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_asp_label, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_m3ua_asp_label, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_m3ua_asp_label, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // *** set sccp match data ***
    // **** calling party ****
    // sccp calling party routing indicator
    rlbl_39:
    fgn_pld->rproc->current_pos.index = 39;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    sprintf(tmp_str, "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp calling party gti
    rlbl_40:
    fgn_pld->rproc->current_pos.index = 40;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp calling party ssn
    rlbl_41:
    fgn_pld->rproc->current_pos.index = 41;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp calling party point code
    rlbl_42:
    fgn_pld->rproc->current_pos.index = 42;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp calling party global title tt
    rlbl_43:
    fgn_pld->rproc->current_pos.index = 43;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp calling party global title np
    rlbl_44:
    fgn_pld->rproc->current_pos.index = 44;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp calling party global title nai
    rlbl_45:
    fgn_pld->rproc->current_pos.index = 45;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp calling party global title address
    rlbl_46:
    fgn_pld->rproc->current_pos.index = 46;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_sccp_calling_pa_gt_address, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // **** called party ****
    // sccp called party routing indicator
    rlbl_47:
    fgn_pld->rproc->current_pos.index = 47;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp called party gti
    rlbl_48:
    fgn_pld->rproc->current_pos.index = 48;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp called party ssn
    rlbl_49:
    fgn_pld->rproc->current_pos.index = 49;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp called party point code
    rlbl_50:
    fgn_pld->rproc->current_pos.index = 50;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_point_code_number, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp called party global title tt
    rlbl_51:
    fgn_pld->rproc->current_pos.index = 51;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp called party global title np
    rlbl_52:
    fgn_pld->rproc->current_pos.index = 52;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp called party global title nai
    rlbl_53:
    fgn_pld->rproc->current_pos.index = 53;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, (int)*vp_spec);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sccp called party global title address
    rlbl_54:
    fgn_pld->rproc->current_pos.index = 54;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, PIT_RULE_MATCH_PART, 2);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, (int)*vp_spec);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_sccp_called_pa_gt_address, true, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // *** tcap match data ***
    // TC message type
    rlbl_55:
    fgn_pld->rproc->current_pos.index = 55;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_message_type);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_tcap_message_type);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_tcap_message_type, false, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // sid
    rlbl_56:
    fgn_pld->rproc->current_pos.index = 56;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_tcap_source_transaction_id, false, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // did
    rlbl_57:
    fgn_pld->rproc->current_pos.index = 57;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id);
	if(vp_data != NULL){
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_tcap_destination_transaction_id, false, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;

    // cc
    rlbl_58:
    fgn_pld->rproc->current_pos.index = 58;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_component_count);
    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_component_count);
    if(vp_data != NULL) cc = (int)*vp_data;
    if(vp_spec != NULL && vp_data != NULL){
	snprintf(tmp_str, sizeof(tmp_str), "%d", cc);
	res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_tcap_component_count, false, 0, 0, &default_res);
    }
    if(!res) return default_res;

    // ct
    rlbl_59:
    fgn_pld->rproc->current_pos.index = 59;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_component_type);
    if(vp_spec != NULL){
	// check all components
	for(unsigned int i = rule_resume_loop_index; i<cc; i++){
	    fgn_pld->rproc->current_pos.loop_index = i;
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_component_type, i);
	    if(vp_data == NULL) continue;
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res = match(tmp_str, rule, input, asn1::ParameterType::_pt_tcap_component_type, true, 0, 0, &default_res);
	    // stop loop if matched
	    if(res) break; else if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
	}
	// reset resume loop index
	rule_resume_loop_index = 0;
    }
    if(!res) return default_res;

    // iid
    rlbl_60:
    fgn_pld->rproc->current_pos.index = 60;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_component_invoke_id);
    if(vp_spec != NULL){
	// check all components
	for(unsigned int i = rule_resume_loop_index; i<cc; i++){
	    fgn_pld->rproc->current_pos.loop_index = i;
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_component_invoke_id, i);
	    if(vp_data == NULL) continue;
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res = match(tmp_str, rule, input, asn1::ParameterType::_pt_tcap_component_invoke_id, true, 0, 0, &default_res);
	    // stop loop if matched
	    if(res) break; else if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
	}
	// reset resume loop index
	rule_resume_loop_index = 0;
    }
    if(!res) return default_res;

    // opcode
    rlbl_61:
    fgn_pld->rproc->current_pos.index = 61;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_opcode);
    if(vp_spec != NULL){
	// check all components
	for(unsigned int i = rule_resume_loop_index; i<cc; i++){
	    fgn_pld->rproc->current_pos.loop_index = i;
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_opcode, i);
	    if(vp_data == NULL) continue;
	    snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
	    res = match(tmp_str, rule, input, asn1::ParameterType::_pt_tcap_opcode, true, 0, 0, &default_res);
	    // stop loop if matched
	    if(res) break; else if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
	}
	// reset resume loop index
	rule_resume_loop_index = 0;
    }
    if(!res) return default_res;

    // dlg_ctx
    rlbl_62:
    fgn_pld->rproc->current_pos.index = 62;
    vp_spec = rparams->get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
    if(vp_spec != NULL){
	vp_data = input->get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
	if(vp_data != NULL){
	    res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_tcap_dialogue_context_oid, false, 0, 0, &default_res);
	}else res &= false;
    }
    if(!res) return default_res;


    // sri-for-sm context
    rlbl_63:
    fgn_pld->rproc->current_pos.index = 63;
    if(ctx_sri){
	// tmp res
	cc_res = false;
	// check all context matching components
	for(unsigned int i = rule_resume_loop_index; i<cc; i++){
	    fgn_pld->rproc->current_pos.loop_index = i;
	    // resume/start
	    #ifdef FLOGIC_PAUSE_RESUME
	    if(rule_resume_sub_index >= 0){
		int tmp_resume_sub_index = rule_resume_sub_index;
		rule_resume_sub_index = -1;
		goto *lbls[rule_resume_index + tmp_resume_sub_index + 1];
	    }
	    #endif
	    // check if context valid for current component
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_opcode, i);
	    if(vp_data == NULL) continue;
	    if((int)*vp_data != 45) continue;

	    // msisdn nai
	    rlbl_63_0:
	    fgn_pld->rproc->current_pos.sub_index = 0;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // msisdn np
	    rlbl_63_1:
	    fgn_pld->rproc->current_pos.sub_index = 1;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // msisdn address
	    rlbl_63_2:
	    fgn_pld->rproc->current_pos.sub_index = 2;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_msisdn_digits, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // sca nai
	    rlbl_63_3:
	    fgn_pld->rproc->current_pos.sub_index = 3;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_sca_type_of_number, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // sca np
	    rlbl_63_4:
	    fgn_pld->rproc->current_pos.sub_index = 4;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // sca address
	    rlbl_63_5:
	    fgn_pld->rproc->current_pos.sub_index = 5;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_digits, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_sca_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_sca_digits, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // imsi
	    rlbl_63_6:
	    fgn_pld->rproc->current_pos.sub_index = 6;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_imsi, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_imsi, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_imsi, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // nnn nai
	    rlbl_63_7:
	    fgn_pld->rproc->current_pos.sub_index = 7;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // nnn np
	    rlbl_63_8:
	    fgn_pld->rproc->current_pos.sub_index = 8;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // nnn address
	    rlbl_63_9:
	    fgn_pld->rproc->current_pos.sub_index = 9;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_nnn_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_nnn_digits, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // an nai
	    rlbl_63_10:
	    fgn_pld->rproc->current_pos.sub_index = 10;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_an_type_of_number, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_an_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_an_type_of_number, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // an np
	    rlbl_63_11:
	    fgn_pld->rproc->current_pos.sub_index = 11;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_an_numbering_plan, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // an address
	    rlbl_63_12:
	    fgn_pld->rproc->current_pos.sub_index = 12;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_an_digits, PIT_RULE_MATCH_PART, 0, RCT_MAP_SRI_FOR_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_an_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_an_digits, true, RCT_MAP_SRI_FOR_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }


	    // if all matched, stop
	    if(cc_res) break;


	}
	// reset resume loop index
	rule_resume_loop_index = 0;
	// add to main result
	res &= cc_res;
    }
    if(!res) return default_res;


    // sm context
    rlbl_64:
    fgn_pld->rproc->current_pos.index = 64;
    if(ctx_sm){
	// tmp res
	cc_res = false;
	// check all context matching components
	for(unsigned int i = rule_resume_loop_index; i<cc; i++){
	    fgn_pld->rproc->current_pos.loop_index = i;
	    // resume/start
	    #ifdef FLOGIC_PAUSE_RESUME
	    if(rule_resume_sub_index >= 0){
		int tmp_resume_sub_index = rule_resume_sub_index;
		rule_resume_sub_index = -1;
		goto *lbls[rule_resume_index + tmp_resume_sub_index + 1];
	    }
	    #endif
	    // check if context valid for current component
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_opcode, i);
	    if(vp_data == NULL) continue;
	    if(!((int)*vp_data == 44 || (int)*vp_data == 46)) continue;

	    // imsi
	    rlbl_64_0:
	    fgn_pld->rproc->current_pos.sub_index = 0;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_imsi, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_imsi, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_imsi, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // scda nai
	    rlbl_64_1:
	    fgn_pld->rproc->current_pos.sub_index = 1;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scda_type_of_number, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scda_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_scda_type_of_number, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // scda np
	    rlbl_64_2:
	    fgn_pld->rproc->current_pos.sub_index = 2;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // scda address
	    rlbl_64_3:
	    fgn_pld->rproc->current_pos.sub_index = 3;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scda_digits, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scda_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_scda_digits, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // scoa nai
	    rlbl_64_4:
	    fgn_pld->rproc->current_pos.sub_index = 4;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // scoa np
	    rlbl_64_5:
	    fgn_pld->rproc->current_pos.sub_index = 5;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // scoa address
	    rlbl_64_6:
	    fgn_pld->rproc->current_pos.sub_index = 6;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_scoa_digits, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_scoa_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_scoa_digits, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // msisdn nai
	    rlbl_64_7:
	    fgn_pld->rproc->current_pos.sub_index = 7;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // msisdn np
	    rlbl_64_8:
	    fgn_pld->rproc->current_pos.sub_index = 8;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // msisdn address
	    rlbl_64_9:
	    fgn_pld->rproc->current_pos.sub_index = 9;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, PIT_RULE_MATCH_PART, 0, RCT_MAP_SM);
	    vp_data = input->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_gsmmap_msisdn_digits, true, RCT_MAP_SM, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }


	    // if all matched, stop
	    if(cc_res) break;

	}
	// reset resume loop index
	rule_resume_loop_index = 0;
	// add to main result
	res &= cc_res;
    }
    if(!res) return default_res;

    // smstpdu context
    rlbl_65:
    fgn_pld->rproc->current_pos.index = 65;
    if(ctx_smstpdu){
	// tmp res
	cc_res = false;
	// check all context matching components
	for(unsigned int i = rule_resume_loop_index; i<cc; i++){
	    fgn_pld->rproc->current_pos.loop_index = i;
	    // resume/start
	    #ifdef FLOGIC_PAUSE_RESUME
	    if(rule_resume_sub_index >= 0){
		int tmp_resume_sub_index = rule_resume_sub_index;
		rule_resume_sub_index = -1;
		goto *lbls[rule_resume_index + tmp_resume_sub_index + 1];
	    }
	    #endif
	    // check if context valid for current component
	    vp_data = input->get_param(asn1::ParameterType::_pt_tcap_opcode, i);
	    if(vp_data == NULL) continue;
	    if(!((int)*vp_data == 44 || (int)*vp_data == 46)) continue;

	    // tp-rp
	    rlbl_65_0:
	    fgn_pld->rproc->current_pos.sub_index = 0;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_rp, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_rp, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (bool)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_rp, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-udhi
	    rlbl_65_1:
	    fgn_pld->rproc->current_pos.sub_index = 1;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_udhi, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_udhi, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (bool)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_udhi, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-srr
	    rlbl_65_2:
	    fgn_pld->rproc->current_pos.sub_index = 2;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_srr, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_srr, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (bool)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_srr, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-vpf
	    rlbl_65_3:
	    fgn_pld->rproc->current_pos.sub_index = 3;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_vpf, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_vpf, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_vpf, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-rd
	    rlbl_65_4:
	    fgn_pld->rproc->current_pos.sub_index = 4;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_rd, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_rd, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (bool)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_rd, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-mti
	    rlbl_65_5:
	    fgn_pld->rproc->current_pos.sub_index = 5;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_mti, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_mti, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_mti, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-mr
	    rlbl_65_6:
	    fgn_pld->rproc->current_pos.sub_index = 6;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_mr, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_mr, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_mr, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-sri
	    rlbl_65_7:
	    fgn_pld->rproc->current_pos.sub_index = 7;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_sri, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_sri, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (bool)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_sri, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-mms
	    rlbl_65_8:
	    fgn_pld->rproc->current_pos.sub_index = 8;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_mms, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_mms, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (bool)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_mms, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-da ton
	    rlbl_65_9:
	    fgn_pld->rproc->current_pos.sub_index = 9;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-da np
	    rlbl_65_10:
	    fgn_pld->rproc->current_pos.sub_index = 10;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-da address
	    rlbl_65_11:
	    fgn_pld->rproc->current_pos.sub_index = 11;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_digits, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_da_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_smstpdu_tp_da_digits, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-oa ton
	    rlbl_65_12:
	    fgn_pld->rproc->current_pos.sub_index = 12;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-oa np
	    rlbl_65_13:
	    fgn_pld->rproc->current_pos.sub_index = 13;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-oa address
	    rlbl_65_14:
	    fgn_pld->rproc->current_pos.sub_index = 14;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, i);
	    if(vp_spec != NULL && vp_data != NULL){
		cc_res = match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_smstpdu_tp_oa_digits, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-pid
	    rlbl_65_15:
	    fgn_pld->rproc->current_pos.sub_index = 15;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_pid, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_pid, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_pid, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-dcs
	    rlbl_65_16:
	    fgn_pld->rproc->current_pos.sub_index = 16;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_dcs, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_dcs, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_dcs, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-vp
	    rlbl_65_17:
	    fgn_pld->rproc->current_pos.sub_index = 17;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_vp, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_vp, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_vp, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-udl
	    rlbl_65_18:
	    fgn_pld->rproc->current_pos.sub_index = 18;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_udl, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_udl, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_udl, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // tp-scts
	    rlbl_65_19:
	    fgn_pld->rproc->current_pos.sub_index = 19;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_tp_scts, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_tp_scts, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_tp_scts, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // ie-msg-id
	    rlbl_65_20:
	    fgn_pld->rproc->current_pos.sub_index = 20;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_msg_id, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_msg_id, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_msg_id, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // ie-msg-parts
	    rlbl_65_21:
	    fgn_pld->rproc->current_pos.sub_index = 21;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_msg_parts, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_msg_parts, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_msg_parts, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }

	    // ie-msg-part
	    rlbl_65_22:
	    fgn_pld->rproc->current_pos.sub_index = 22;
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_smstpdu_msg_part, PIT_RULE_MATCH_PART, 0, RCT_DEFAULT);
	    vp_data = input->get_param(asn1::ParameterType::_pt_smstpdu_msg_part, i);
	    if(vp_spec != NULL && vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		cc_res = match(tmp_str, rule, input, asn1::ParameterType::_pt_smstpdu_msg_part, true, RCT_DEFAULT, 0, &default_res);
		// next component if not matched
		if(!cc_res){
		    // stop if pausing from lua
		    if(default_res == pmink_utils::RRT_CONTINUE_PAUSE) return default_res;
		    // next component, not pausing and not matched
		    continue;
		}

	    }




	    // if all matched, stop
	    if(cc_res) break;

	}
	// reset resume loop index
	rule_resume_loop_index = 0;
	// add to main result
	res &= cc_res;

    }
    if(!res) return default_res;

    // smpp context
    rlbl_66:
    fgn_pld->rproc->current_pos.index = 66;
    if(ctx_smpp){
	// smpp as
	rlbl_66_0:
	fgn_pld->rproc->current_pos.sub_index = 0;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_as_label);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_as_label);
	    if(vp_data != NULL){
		res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_smpp_as_label, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp asp
	rlbl_66_1:
	fgn_pld->rproc->current_pos.sub_index = 1;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_asp_label);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_asp_label);
	    if(vp_data != NULL){
		res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_smpp_asp_label, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp command id
	rlbl_66_2:
	fgn_pld->rproc->current_pos.sub_index = 2;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_command_id);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_command_id);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%u", (uint32_t)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_command_id, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp source_addr_ton
	rlbl_66_3:
	fgn_pld->rproc->current_pos.sub_index = 3;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_originator_ton);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_originator_ton);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_originator_ton, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp dest_addr_ton
	rlbl_66_4:
	fgn_pld->rproc->current_pos.sub_index = 4;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_recipient_ton);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_recipient_ton);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_recipient_ton, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp source_addr_npi
	rlbl_66_5:
	fgn_pld->rproc->current_pos.sub_index = 5;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_originator_np);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_originator_np);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_originator_np, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp dest_addr_npi
	rlbl_66_6:
	fgn_pld->rproc->current_pos.sub_index = 6;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_recipient_np);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_recipient_np);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_recipient_np, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp source_addr
	rlbl_66_7:
	fgn_pld->rproc->current_pos.sub_index = 7;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_originator_address);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_originator_address);
	    if(vp_data != NULL){
		res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_smpp_originator_address, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp destination_addr
	rlbl_66_8:
	fgn_pld->rproc->current_pos.sub_index = 8;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_recipient_address);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_recipient_address);
	    if(vp_data != NULL){
		res &= match((char*)*vp_data, rule, input, asn1::ParameterType::_pt_smpp_recipient_address, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp esm_class_mm
	rlbl_66_9:
	fgn_pld->rproc->current_pos.sub_index = 9;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_esm_message_mode);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_esm_message_mode);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_esm_message_mode, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp esm_class_mt
	rlbl_66_10:
	fgn_pld->rproc->current_pos.sub_index = 10;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_esm_message_type);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_esm_message_type);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_esm_message_type, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp esm_class_gsm
	rlbl_66_11:
	fgn_pld->rproc->current_pos.sub_index = 11;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_esm_gsm_features);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_esm_gsm_features);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_esm_gsm_features, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp protocol_id
	rlbl_66_12:
	fgn_pld->rproc->current_pos.sub_index = 12;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_protocol_id);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_protocol_id);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_protocol_id, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp priority_flag
	rlbl_66_13:
	fgn_pld->rproc->current_pos.sub_index = 13;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_priority_flag);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_priority_flag);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_priority_flag, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp delivery_time
	rlbl_66_14:
	fgn_pld->rproc->current_pos.sub_index = 14;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_delivery_time);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_delivery_time);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_delivery_time, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp validity_period
	rlbl_66_15:
	fgn_pld->rproc->current_pos.sub_index = 15;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_validity_period);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_validity_period);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_validity_period, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp rd_smsc_dlvr_rcpt
	rlbl_66_16:
	fgn_pld->rproc->current_pos.sub_index = 16;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_rd_smsc_receipt);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_rd_smsc_receipt);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_rd_smsc_receipt, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp rd_sme_orig_ack
	rlbl_66_17:
	fgn_pld->rproc->current_pos.sub_index = 17;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_rd_sme_ack);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_rd_sme_ack);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_rd_sme_ack, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp rd_intrmd_ntf
	rlbl_66_18:
	fgn_pld->rproc->current_pos.sub_index = 18;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_rd_intermediate_notification);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_rd_intermediate_notification);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_rd_intermediate_notification, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp replace_if_present_flag
	rlbl_66_19:
	fgn_pld->rproc->current_pos.sub_index = 19;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_replace_if_present_flag);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_replace_if_present_flag);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_replace_if_present_flag, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp data_coding
	rlbl_66_20:
	fgn_pld->rproc->current_pos.sub_index = 20;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_data_coding);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_data_coding);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_data_coding, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp sm_default_msg_id
	rlbl_66_21:
	fgn_pld->rproc->current_pos.sub_index = 21;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sm_defaut_msg_id);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sm_defaut_msg_id);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_sm_defaut_msg_id, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp sm_length
	rlbl_66_22:
	fgn_pld->rproc->current_pos.sub_index = 22;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sm_length);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sm_length);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_sm_length, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp sar_msg_ref_num
	rlbl_66_23:
	fgn_pld->rproc->current_pos.sub_index = 23;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_sar_msg_ref_num, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp sar_total_segments
	rlbl_66_24:
	fgn_pld->rproc->current_pos.sub_index = 24;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sar_total_segments);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sar_total_segments);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_sar_total_segments, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

	// smpp sar_segment_seqnum
	rlbl_66_25:
	fgn_pld->rproc->current_pos.sub_index = 25;
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_smpp_sar_segment_seqnum);
	if(vp_spec != NULL){
	    vp_data = input->get_param(asn1::ParameterType::_pt_smpp_sar_segment_seqnum);
	    if(vp_data != NULL){
		snprintf(tmp_str, sizeof(tmp_str), "%d", (int)*vp_data);
		res &= match(tmp_str, rule, input, asn1::ParameterType::_pt_smpp_sar_segment_seqnum, false, 0, 0, &default_res);
	    }else res &= false;
	}
	if(!res) return default_res;

    }


    // do translations (if everything matched)
    lbl_translate:
    if(res){
	// *** r14p translate data ***
	// trunk label
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_trunk_label, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_STRING,
			   asn1::ParameterType::_pt_pmink_trunk_label);
	}

	// service id
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_service_id, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_pmink_service_id);
	}

	// *** m3ua translate data ***
	// m3ua opc
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code);
	}

	// m3ua dpc
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code);
	}

	// m3ua si
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator);
	}

	// m3ua ni
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator);
	}

	// m3ua mp
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_message_priority, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_message_priority);
	}

	// m3ua sls
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code);
	}

	// *** sccp translate data ***
	// **** calling party ****
	// sccp calling party routing indicator
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator);
	}

	// sccp calling party gti
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator);
	}

	// sccp calling party ssn
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);
	}

	// sccp calling party point code
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_point_code_number, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_point_code_number);
	}

	// sccp calling party global title tt
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type);
	}

	// sccp calling party global title np
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan);
	}

	// sccp calling party global title nai
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address);
	}

	// sccp calling party global title address
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_STRING,
			   asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
	}


	// **** called party ****
	// sccp called party routing indicator
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_routing_indicator, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_routing_indicator);
	}

	// sccp called party gti
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator);
	}

	// sccp called party ssn
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);
	}

	// sccp called party point code
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_point_code_number, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_point_code_number);
	}

	// sccp called party global title tt
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type);
	}

	// sccp called party global title np
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan);
	}

	// sccp called party global title nai
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_INT,
			   asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address);
	}

	// sccp called party global title address
	vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // check cfg mapping and get param value
	    find_set_param((char*)*vp_spec,
			   input,
			   input,
			   pmink_utils::DPT_STRING,
			   asn1::ParameterType::_pt_sccp_called_pa_gt_address);
	}

	// check if jumping
	vp_spec = rparams->get_param(RPT_RULE_FJUMP, PIT_RULE_TRANSLATE_PART);
	if(vp_spec != NULL){
	    // find next rule
	    rule->get_rm()->set_next_rule(rule->get_rm()->get_rule((char*)*vp_spec));
	    // if found, continue
	    if(rule->get_rm()->get_next_rule() != NULL) default_res = pmink_utils::RRT_CONTINUE;
	    // else stop processing rules
	    else default_res = pmink_utils::RRT_STOP_ERR;

	// not jumping, check for filter result override in translation part
	}else{
	    // set filter translation result
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_pmink_filter_result, PIT_RULE_TRANSLATE_PART);
	    if(vp_spec != NULL) default_res = (int)*vp_spec;
	}



	// *** special commands ***
	if(ctx_cmd_sri_req){
	    // add sri-for-sm req params to original input (index PIT_INPUT_CMD_REQ_PART)

	    // sri-for-sm req map version
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_version, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_gsmmap_version,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cgpa ssn
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cgpa gt tt
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cgpa gt np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cgpa gt nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cgpa gt address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_STRING,
			       asn1::ParameterType::_pt_sccp_calling_pa_gt_address,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cdpa ssn
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_sccp_called_pa_subsystem_number,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cdpa gt tt
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cdpa gt np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cdpa gt nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req cdpa gt address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_STRING,
			       asn1::ParameterType::_pt_sccp_called_pa_gt_address,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req msisdn nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req msisdn np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req msisdn address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_msisdn_digits, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_STRING,
			       asn1::ParameterType::_pt_gsmmap_msisdn_digits,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req sca nai
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_type_of_number, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_gsmmap_sca_type_of_number,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req sca np
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_numbering_plan, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_INT,
			       asn1::ParameterType::_pt_gsmmap_sca_numbering_plan,
			       PIT_INPUT_CMD_REQ_PART);
	    }

	    // sri-for-sm req sca address
	    vp_spec = rparams->get_param(asn1::ParameterType::_pt_gsmmap_sca_digits, PIT_RULE_TRANSLATE_PART, 0, RCT_CMD_SRI_REQ);
	    if(vp_spec != NULL){
		find_set_param((char*)*vp_spec,
			       input,
			       input,
			       pmink_utils::DPT_STRING,
			       asn1::ParameterType::_pt_gsmmap_sca_digits,
			       PIT_INPUT_CMD_REQ_PART);
	    }


	    // generate sri-sm-req
	    // if error, stop
	    if(generate_sri_sm_req(input) != 0) return pmink_utils::RRT_STOP_ERR;

	    // set next rule if not set (used later as start rule, when sri-rpl is received)
	    if(rule->get_rm()->get_next_rule() == NULL){
		// get current rule position
		rule_t::rule_it_t next_rule_it = rule->get_position();
		// next rule
		++next_rule_it;
		// check if no more rules in list
		if(next_rule_it == rule->get_rm()->get_end()){
		    // stop processing rules
		    return default_res;

		}
		// set next rule pointer
		rule->get_rm()->set_next_rule(&next_rule_it->second);
	    }

	    // pause rule processing, waiting for sri-for-sm reply
	    return pmink_utils::RRT_CONTINUE_PAUSE;
	}


    }

    // remove previous destinations
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_index, -1);
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_destination, -1);
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_destination_type, -1);
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_sub_destination_type, -1);
    input->erase_param(asn1::ParameterType::_pt_pmink_routing_sub_destination, -1);

    // default res (based on hunt-stop)
    lbl_exit:
    return default_res;
}

fgn::FilteringLogic::FilteringLogic(){
    // init cfg translation map

    // *** dynamic
    // pmink
    cfg_trans_map[pmink_utils::hash_fnv1a_str("pmink.timestamp")] = new CfgTITimestamp();


    // *** vars
    // smpp
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.command_id")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_command_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.source_addr_ton")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_originator_ton);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.dest_addr_ton")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_recipient_ton);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.source_addr_npi")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_originator_np);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.dest_addr_npi")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_recipient_np);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.source_addr")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_originator_address);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.destination_addr")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_recipient_address);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.esm_class_mm")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_esm_message_mode);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.esm_class_mt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_esm_message_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.esm_class_gsm")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_esm_gsm_features);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.protocol_id")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_protocol_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.priority_flag")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_priority_flag);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.delivery_time")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_delivery_time);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.validity_period")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_validity_period);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.rd_smsc_dlvr_rcpt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_rd_smsc_receipt);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.rd_sme_orig_ack")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_rd_sme_ack);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.rd_intrmd_ntf")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_rd_intermediate_notification);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.replace_if_present_flag")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_replace_if_present_flag);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.data_coding")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_data_coding);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sm_default_msg_id")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sm_defaut_msg_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sm_length")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sm_length);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sar_msg_ref_num")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sar_msg_ref_num);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sar_total_segments")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sar_total_segments);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.sar_segment_seqnum")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smpp_sar_segment_seqnum);

    // tcap
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.tcmt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_message_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.sid")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_source_transaction_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.did")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_destination_transaction_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.cc")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_component_count);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.ct")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_component_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.iid")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_component_invoke_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.opcode")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_opcode);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.dlg_ctx")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_tcap_dialogue_context_oid);
    // map
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.imsi")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_imsi);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.msisdn.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_msisdn_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.msisdn.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_msisdn_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.msisdn.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_msisdn_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.sca.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_sca_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.sca.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_sca_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.sca.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_sca_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scoa.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scoa_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scoa.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scoa.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scoa_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scda.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scda_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scda.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scda_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.scda.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_scda_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.nnn.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_nnn_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.nnn.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.nnn.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_nnn_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.an.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_an_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.an.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_an_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.an.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_gsmmap_an_digits);
    // sms tpdu
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-rp")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_rp);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-udhi")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_udhi);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-srr")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_srr);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-vpf")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_vpf);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-rd")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_rd);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-mti")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_mti);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-mr")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_mr);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-sri")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_sri);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-mms")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_mms);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-da.ton")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-da.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-da.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_da_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-oa.ton")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-oa.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-oa.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_oa_digits);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-pid")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_pid);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-dcs")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_dcs);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-vp")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_vp);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-udl")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_udl);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.tp-scts")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_tp_scts);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.ie.msg_id")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_msg_id);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.ie.msg_parts")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_msg_parts);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.ie.msg_part")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_smstpdu_msg_part);



    // m3ua
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.opc")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.dpc")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.si")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_service_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.ni")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_network_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.mp")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_message_priority);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.sls")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_protocol_data_signalling_link_selection_code);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.as")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_as_label);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.asp")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_m3ua_asp_label);
    // sccp
    // calling party
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.routing-indicator")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_routing_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gti")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_global_title_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.ssn")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.point-code")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_point_code_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gt.tt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gt.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gt.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cgpa.gt.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
    // called party
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.routing-indicator")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_routing_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gti")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_global_title_indicator);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.ssn")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.point-code")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_point_code_number);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gt.tt")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gt.np")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gt.nai")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.cdpa.gt.address")] = new CfgTranslationItem(CTMT_VAR, asn1::ParameterType::_pt_sccp_called_pa_gt_address);


    // *** constants
    // r14p
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_openli")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_openli);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_sms_dr")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_sms_data_retention);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_stp_routing")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_stp_routing);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_sgn_fwd")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_sgn_forward);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_smshub_fwd")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_smshub_forward);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_fgn_filtering")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_fgn_filtering);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_security")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_security);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.srvcid_pdn_filtering")] = new CfgTranslationItem(CTMT_CONST, asn1::ServiceId::_sid_pdn_filtering);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.cmdid_srism_req")] = new CfgTranslationItem(CTMT_CONST, asn1::SmsHubCommandId::_shci_sri_sm_req);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.cmdid_srism_ack")] = new CfgTranslationItem(CTMT_CONST, asn1::SmsHubCommandId::_shci_sri_sm_ack);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.cmdid_corr_ntf")] = new CfgTranslationItem(CTMT_CONST, asn1::SmsHubCommandId::_shci_corr_ntf);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.cmdid_sms_ack")] = new CfgTranslationItem(CTMT_CONST, asn1::SmsHubCommandId::_shci_sms_ack);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.cmdid_sms_dlvr_rcpt")] = new CfgTranslationItem(CTMT_CONST, asn1::SmsHubCommandId::_shci_sms_dlvr_rcpt);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.cmdid_smpp_generate_udh")] = new CfgTranslationItem(CTMT_CONST, asn1::SmsHubCommandId::_shci_smpp_generate_udh);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.cmdid_tcap_continue")] = new CfgTranslationItem(CTMT_CONST, asn1::SmsHubCommandId::_shci_tcap_continue);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_sctp")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_SCTP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_m3ua")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_M3UA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_tcp")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_TCP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_smpp")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_SMPP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_pcap_m3ua")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_PCAP_M3UA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("r14p.const.connt_pcap_smpp")] = new CfgTranslationItem(CTMT_CONST, sgn::CT_PCAP_SMPP);


    // smpp
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_generic_nack")] = new CfgTranslationItem(CTMT_CONST, smpp::GENERIC_NACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_receiver")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_RECEIVER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_receiver_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_RECEIVER_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_transmitter")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_TRANSMITTER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_transmitter_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_TRANSMITTER_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_query_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::QUERY_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_query_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::QUERY_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_submit_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::SUBMIT_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_submit_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::SUBMIT_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_deliver_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::DELIVER_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_deliver_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::DELIVER_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_unbind")] = new CfgTranslationItem(CTMT_CONST, smpp::UNBIND);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_unbind_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::UNBIND_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_replace_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::REPLACE_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_replace_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::REPLACE_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_cancel_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::CANCEL_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_cancel_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::CANCEL_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_transceiver")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_TRANSCEIVER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_bind_transceiver_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::BIND_TRANSCEIVER_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_outbind")] = new CfgTranslationItem(CTMT_CONST, smpp::OUTBIND);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_enquire_link")] = new CfgTranslationItem(CTMT_CONST, smpp::ENQUIRE_LINK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_enquire_link_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::ENQUIRE_LINK_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_submit_multi")] = new CfgTranslationItem(CTMT_CONST, smpp::SUBMIT_MULTI);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_submit_multi_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::SUBMIT_MULTI_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_alert")] = new CfgTranslationItem(CTMT_CONST, smpp::ALERT_NOTIFICATION);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_data_sm")] = new CfgTranslationItem(CTMT_CONST, smpp::DATA_SM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.command_data_sm_resp")] = new CfgTranslationItem(CTMT_CONST, smpp::DATA_SM_RESP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_international")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_INTERNATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_national")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_network_specific")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_NETWORK_SPECIFIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_subscriber")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_SUBSCRIBER_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_alphanumeric")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_ALPHANUMERIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ton_abbreviated")] = new CfgTranslationItem(CTMT_CONST, smpp::TON_ABBREVIATED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_isdn_telephone")] = new CfgTranslationItem(CTMT_CONST, smpp::ISDN_TELEPHONE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_data_x121")] = new CfgTranslationItem(CTMT_CONST, smpp::DATA_X121);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_telex")] = new CfgTranslationItem(CTMT_CONST, smpp::TELEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_land_mobile")] = new CfgTranslationItem(CTMT_CONST, smpp::LAND_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_national")] = new CfgTranslationItem(CTMT_CONST, smpp::NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_private")] = new CfgTranslationItem(CTMT_CONST, smpp::PRIVATE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_ermes")] = new CfgTranslationItem(CTMT_CONST, smpp::ERMES);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_internet_ip")] = new CfgTranslationItem(CTMT_CONST, smpp::INTERNET_IP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.npi_wap_client_id")] = new CfgTranslationItem(CTMT_CONST, smpp::WAP_CLIENT_ID);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_default")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_DEFAULT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_ia5_ascii")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_IA5_ASCII);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_8bit_binary_1")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_8BIT_BINARY_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_iso_8859_1")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_ISO_8859_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_8bit_binary_2")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_8BIT_BINARY_2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_jis")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_JIS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_8859_5")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_ISO_8859_5);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_8859_8")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_ISO_8859_8);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_ucs2")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_UCS2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_pictogram")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_PICTOGRAM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_iso_2011_jp")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_ISO_2011_JP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_extended_kanji")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_EXTENDED_KANJI);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dc_ks_c_5601")] = new CfgTranslationItem(CTMT_CONST, smpp::DC_KS_C_5601);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.gsm_no_features")] = new CfgTranslationItem(CTMT_CONST, smpp::GNS_NO_SPECIFIC_FEATURES);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.gsm_udhi")] = new CfgTranslationItem(CTMT_CONST, smpp::GNS_UDHI_INDICATOR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.gsm_reply_path")] = new CfgTranslationItem(CTMT_CONST, smpp::GNS_SET_REPLY_PATH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.gsm_udhi_reply_path")] = new CfgTranslationItem(CTMT_CONST, smpp::GNS_SET_UDHI_AND_REPLY_PATH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.int_no")] = new CfgTranslationItem(CTMT_CONST, smpp::INT_NO);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.int_yes")] = new CfgTranslationItem(CTMT_CONST, smpp::INT_YES);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mm_default_smsc")] = new CfgTranslationItem(CTMT_CONST, smpp::MM_DEFAULT_SMSC_MODE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mm_datagram")] = new CfgTranslationItem(CTMT_CONST, smpp::MM_DATAGRAM_MODE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mm_forward")] = new CfgTranslationItem(CTMT_CONST, smpp::MM_FORWARD_MODE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mm_store_forward")] = new CfgTranslationItem(CTMT_CONST, smpp::MM_STORE_FORWARD_MODE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_enroute")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_ENROUTE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_delivered")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_DELIVERED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_expired")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_EXPIRED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_deleted")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_DELETED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_undeliverable")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_UNDELIVERABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_accepted")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_ACCEPTED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mst_rejected")] = new CfgTranslationItem(CTMT_CONST, smpp::MST_REJECTED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_default")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_DEFAULT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_smsc_delivery_rcpt")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_SMSC_DELIVERY_RECEIPT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_delivery_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_DELIVERY_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_manual_user_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_MANUAL_USER_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_cnvrs_abort")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_CNVRS_ABORT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mt_intrm_dlvr_ntf")] = new CfgTranslationItem(CTMT_CONST, smpp::MT_INTRM_DLVR_NTF);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.soa_no_sme_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::SOA_NO_SME_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.soa_sme_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::SOA_SME_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.soa_sme_manual_user_ack")] = new CfgTranslationItem(CTMT_CONST, smpp::SOA_SME_MANUAL_USER_ACK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.soa_sme_both")] = new CfgTranslationItem(CTMT_CONST, smpp::SOA_SME_BOTH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sdr_no_smsc_delivery")] = new CfgTranslationItem(CTMT_CONST, smpp::SDR_NO_SMSC_DELIVERY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sdr_success_failure")] = new CfgTranslationItem(CTMT_CONST, smpp::SDR_SUCCESS_FAILURE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sdr_failure")] = new CfgTranslationItem(CTMT_CONST, smpp::SDR_FAILURE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.si_not_screened")] = new CfgTranslationItem(CTMT_CONST, smpp::SI_NOT_SCREENED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.si_verified_passed")] = new CfgTranslationItem(CTMT_CONST, smpp::SI_VERIFIED_PASSED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.si_verified_failed")] = new CfgTranslationItem(CTMT_CONST, smpp::SI_VERIFIED_FAILED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.si_network_provided")] = new CfgTranslationItem(CTMT_CONST, smpp::SI_NETWORK_PROVIDED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_allowed")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_ALLOWED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_restricted")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_RESTRICTED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_not_available")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_NOT_AVAILABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dfr_destination_unavailable")] = new CfgTranslationItem(CTMT_CONST, smpp::DFR_DESTINATION_UNAVAILABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dfr_destination_address_invalid")] = new CfgTranslationItem(CTMT_CONST, smpp::DFR_DESTINATION_ADDRESS_INVALID);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dfr_perm_net_err")] = new CfgTranslationItem(CTMT_CONST, smpp::DFR_PERMANENT_NETWORK_ERROR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dfr_temp_net_err")] = new CfgTranslationItem(CTMT_CONST, smpp::DFR_TEMPORARY_NETWORK_ERROR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_ms_display")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_MS_DISPLAY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_mobile_equipment")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_MOBILE_EQUIPMENT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_smart_card_1")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_SMART_CARD_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.das_external_unit_1")] = new CfgTranslationItem(CTMT_CONST, smpp::DAS_EXTERNAL_UNIT_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_sms")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_SMS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_csd")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_CSD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_packet_data")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_PACKET_DATA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_ussd")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_USSD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_cdpd")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_CDPD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_data_tac")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_DATA_TAC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_flex_reflex")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_FLEX_REFLEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.db_cell_broadcast")] = new CfgTranslationItem(CTMT_CONST, smpp::DB_CELL_BROADCAST);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_gsm")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_GSM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_ansi_136")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_ANSI_136);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_is_95")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_IS_95);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_pdc")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_PDC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_phs")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_PHS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_iden")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_IDEN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_amps")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_AMPS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dn_paging_network")] = new CfgTranslationItem(CTMT_CONST, smpp::DN_PAGING_NETWORK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ds_nsap_even")] = new CfgTranslationItem(CTMT_CONST, smpp::DS_NSAP_EVEN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ds_nsap_odd")] = new CfgTranslationItem(CTMT_CONST, smpp::DS_NSAP_ODD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ds_user_specified")] = new CfgTranslationItem(CTMT_CONST, smpp::DS_USER_SPECIFIED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dt_temporary")] = new CfgTranslationItem(CTMT_CONST, smpp::DT_TEMPORARY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dt_default")] = new CfgTranslationItem(CTMT_CONST, smpp::DT_DEFAULT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.dt_invoke")] = new CfgTranslationItem(CTMT_CONST, smpp::DT_INVOKE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_digit")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_DIGIT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_number")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_telephone_no")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_TELEPHONE_NO);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_password")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_PASSWORD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_char_line")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_CHARACTER_LINE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_menu")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_MENU);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_date")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_DATE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_time")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_TIME);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.irc_continue")] = new CfgTranslationItem(CTMT_CONST, smpp::IRC_CONTINUE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_unspecified")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_UNSPECIFIED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_english")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_ENGLISH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_french")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_FRENCH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_spanish")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_SPANISH);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_german")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_GERMAN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.li_portuguese")] = new CfgTranslationItem(CTMT_CONST, smpp::LI_PORTUGUESE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mas_available")] = new CfgTranslationItem(CTMT_CONST, smpp::MAS_AVAILABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mas_denied")] = new CfgTranslationItem(CTMT_CONST, smpp::MAS_DENIED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mas_unavailable")] = new CfgTranslationItem(CTMT_CONST, smpp::MAS_UNAVAILABLE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mmwf_voicemail")] = new CfgTranslationItem(CTMT_CONST, smpp::MMWF_VOICEMAIL_MESSAGE_WAITING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mmwf_fax")] = new CfgTranslationItem(CTMT_CONST, smpp::MMWF_FAX_MESAGE_WAITING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mmwf_email")] = new CfgTranslationItem(CTMT_CONST, smpp::MMWF_ELECTRONIC_MAIL_MESSAGE_WAITING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mmwf_other")] = new CfgTranslationItem(CTMT_CONST, smpp::MMWF_OTHER_MESSAGE_WAITING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mv_store_indef")] = new CfgTranslationItem(CTMT_CONST, smpp::MV_STORE_INDEFINITELY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mv_power_down")] = new CfgTranslationItem(CTMT_CONST, smpp::MV_POWER_DOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mv_sid_based_reg_area")] = new CfgTranslationItem(CTMT_CONST, smpp::MV_SID_BASED_REGISTRATION_AREA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.mv_display_only")] = new CfgTranslationItem(CTMT_CONST, smpp::MV_DISPLAY_ONLY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.nec_ansi_136")] = new CfgTranslationItem(CTMT_CONST, smpp::NEC_ANSI_136);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.nec_is_95")] = new CfgTranslationItem(CTMT_CONST, smpp::NEC_IS_95);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.nec_gsm")] = new CfgTranslationItem(CTMT_CONST, smpp::NEC_GSM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pc_default")] = new CfgTranslationItem(CTMT_CONST, smpp::PC_DEFAULT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pc_wcmp")] = new CfgTranslationItem(CTMT_CONST, smpp::PC_WCMP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_plevel_0")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_PRIVACY_LEVEL_0);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_plevel_1")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_PRIVACY_LEVEL_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_plevel_2")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_PRIVACY_LEVEL_2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.pi_plevel_3")] = new CfgTranslationItem(CTMT_CONST, smpp::PI_PRIVACY_LEVEL_3);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_ms_display")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_MS_DISPLAY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_mobile_equipment")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_MOBILE_EQUIPMENT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_smart_card_1")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_SMART_CARD_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sas_external_unit_1")] = new CfgTranslationItem(CTMT_CONST, smpp::SAS_EXTERNAL_UNIT_1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_sms")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_SMS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_csd")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_CSD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_packet_data")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_PACKET_DATA);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_ussd")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_USSD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_cdpd")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_CDPD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_data_tac")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_DATA_TAC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_flex_reflex")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_FLEX_REFLEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sbc_cell_broadcast")] = new CfgTranslationItem(CTMT_CONST, smpp::SBC_CELL_BROADCAST);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_unknown")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_gsm")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_GSM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_ansi_136")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_ANSI_136);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_is_95")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_IS_95);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_pdc")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_PDC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_phs")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_PHS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_iden")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_IDEN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_amps")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_AMPS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.sn_paging_network")] = new CfgTranslationItem(CTMT_CONST, smpp::SN_PAGING_NETWORK);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ss_nsap_even")] = new CfgTranslationItem(CTMT_CONST, smpp::SS_NSAP_EVEN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ss_nsap_odd")] = new CfgTranslationItem(CTMT_CONST, smpp::SS_NSAP_ODD);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.ss_user_specified")] = new CfgTranslationItem(CTMT_CONST, smpp::SS_USER_SPECIFIED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_pssd_indication")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_PSSD_INDICATION);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_pssr_indication")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_PSSR_INDICATION);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_ussr_request")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_USSR_REQUEST);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_ussn_request")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_USSN_REQUEST);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_pssd_response")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_PSSD_RESPONSE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_pssr_respoonse")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_PSSR_RESPONSE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_ussr_confirm")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_USSR_CONFIRM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.uso_ussn_confirm")] = new CfgTranslationItem(CTMT_CONST, smpp::USO_USSN_CONFIRM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.da_sme_address")] = new CfgTranslationItem(CTMT_CONST, smpp::DA_SME_ADDRESS);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smpp.const.da_dl_address")] = new CfgTranslationItem(CTMT_CONST, smpp::DA_DISTRIBUTION_LIST_ADDRESS);


    // SMS TPDU
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_unknown")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_international")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_INTERNATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_national")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_network_specific")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_NETWORK_SPECIFIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_subscriber")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_SUBSCRIBER_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_alphanumeric")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_ALPHANUMERIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.noa_abbreviated")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TON_ABBREVIATED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_unknown")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_isdn_telephone")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_ISDN_TELEPHONE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_data_x121")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_DATA_X121);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_telex")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_TELEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_land_mobile")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_LAND_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_national")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_private")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_PRIVATE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.np_ermes")] = new CfgTranslationItem(CTMT_CONST, smstpdu::NP_ERMES);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.mti_deliver")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPMTIT_SMS_DELIVER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.mti_submit")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPMTIT_SMS_SUBMIT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.vpf_np")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPVPFT_NOT_PRESENT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.vpf_relative")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPVPFT_RELATIVE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.vpf_enhanced")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPVPFT_ENHANCED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.vpf_absolute")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPVPFT_ABSOLUTE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.dcs_gsm7")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPDCSAT_GSM7);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.dcs_8bit")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPDCSAT_8BIT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("smstpdu.const.dcs_ucs2")] = new CfgTranslationItem(CTMT_CONST, smstpdu::TPDCSAT_UCS2);


    // tcap
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.tcmt_begin")] = new CfgTranslationItem(CTMT_CONST, 2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.tcmt_continue")] = new CfgTranslationItem(CTMT_CONST, 5);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.tcmt_end")] = new CfgTranslationItem(CTMT_CONST, 4);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.tcmt_abort")] = new CfgTranslationItem(CTMT_CONST, 7);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_invoke")] = new CfgTranslationItem(CTMT_CONST, 1);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_result_last")] = new CfgTranslationItem(CTMT_CONST, 2);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_error")] = new CfgTranslationItem(CTMT_CONST, 3);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_reject")] = new CfgTranslationItem(CTMT_CONST, 4);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("tcap.const.ct_result_not_last")] = new CfgTranslationItem(CTMT_CONST, 7);


    // GSM MAP
    // nature of address
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_unknown")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_international")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_INTERNATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_national")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_network_specific")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_NETWORK_SPECIFIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_subscriber")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_SUBSCRIBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.noa_abbreviated")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNAI_ABBREVIATED);
    // numbering plan
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_unknown")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_isdn_telephone")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_ISDN_TELEPHONE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_data_x121")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_DATA_X121);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_telex")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_TELEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_land_mobile")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_LAND_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_national")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_NATIONAL);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("map.const.np_private")] = new CfgTranslationItem(CTMT_CONST, asn1::gsmmap::ASNP_PRIVATE);


    // m3ua
    // si
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_snmp")] = new CfgTranslationItem(CTMT_CONST, m3ua::SNMP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_sntmm")] = new CfgTranslationItem(CTMT_CONST, m3ua::SNTMM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_sntmsm")] = new CfgTranslationItem(CTMT_CONST, m3ua::SNTMSM);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_sccp")] = new CfgTranslationItem(CTMT_CONST, m3ua::SCCP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_tup")] = new CfgTranslationItem(CTMT_CONST, m3ua::TUP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_isup")] = new CfgTranslationItem(CTMT_CONST, m3ua::ISUP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_dup_call_circuit")] = new CfgTranslationItem(CTMT_CONST, m3ua::DUP_CALL_AND_CIRCUIT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_dup_reg_canc")] = new CfgTranslationItem(CTMT_CONST, m3ua::DUP_REG_AND_CANC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_mtp_test")] = new CfgTranslationItem(CTMT_CONST, m3ua::MTP_TESTING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_bisup")] = new CfgTranslationItem(CTMT_CONST, m3ua::BISUP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_sisup")] = new CfgTranslationItem(CTMT_CONST, m3ua::SISUP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("m3ua.const.si_gcp")] = new CfgTranslationItem(CTMT_CONST, m3ua::GCP);


    // sccp
    // routing indicator
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.route_on_ssn")] = new CfgTranslationItem(CTMT_CONST, sccp::ROUTE_ON_SSN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.route_on_gt")] = new CfgTranslationItem(CTMT_CONST, sccp::ROUTE_ON_GT);
    // gti
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.gti_noa")] = new CfgTranslationItem(CTMT_CONST, sccp::GT_NATURE_OF_ADDRESS_INDICATOR_ONLY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.gti_tt")] = new CfgTranslationItem(CTMT_CONST, sccp::GT_TRANSLATION_TYPE_ONLY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.gti_ttnpe")] = new CfgTranslationItem(CTMT_CONST, sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.gti_ttnpenoa")] = new CfgTranslationItem(CTMT_CONST, sccp::GT_TRANSLATION_TYPE_NUMNBERING_PLAN_ENCODING_NATURE_OF_ADDRESS);
    // numbering plan
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_unknown")] = new CfgTranslationItem(CTMT_CONST, sccp::UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_isdn_telephone")] = new CfgTranslationItem(CTMT_CONST, sccp::ISDN_TELEPHONE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_generic")] = new CfgTranslationItem(CTMT_CONST, sccp::GENERIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_data_x121")] = new CfgTranslationItem(CTMT_CONST, sccp::DATA_X121);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_telex")] = new CfgTranslationItem(CTMT_CONST, sccp::TELEX);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_maritime")] = new CfgTranslationItem(CTMT_CONST, sccp::MARITIME);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_land_mobile")] = new CfgTranslationItem(CTMT_CONST, sccp::LAND_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_isdn_mobile")] = new CfgTranslationItem(CTMT_CONST, sccp::ISDN_MOBILE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.np_private")] = new CfgTranslationItem(CTMT_CONST, sccp::PRIVATE);
    // nature of address
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_unknown")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_subscriber_number")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_SUBSCRIBER_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_national")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_RESERVED_FOR_NATIONAL_USE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_national_significant")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_NATIONAL_SIGNIFICANT_NUMBER);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.noa_international")] = new CfgTranslationItem(CTMT_CONST, sccp::NOA_INTERNATIONAL);
    // subsystem number
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_unknown")] = new CfgTranslationItem(CTMT_CONST, sccp::SSN_UNKNOWN);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_sccp_mngmt")] = new CfgTranslationItem(CTMT_CONST, sccp::SCCP_MANAGEMENT);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_itut")] = new CfgTranslationItem(CTMT_CONST, sccp::ITU_T_RESERVED);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_isup")] = new CfgTranslationItem(CTMT_CONST, sccp::ISDN_USER_PART);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_omap")] = new CfgTranslationItem(CTMT_CONST, sccp::OMAP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_map")] = new CfgTranslationItem(CTMT_CONST, sccp::MAP);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_hlr")] = new CfgTranslationItem(CTMT_CONST, sccp::HLR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_vlr")] = new CfgTranslationItem(CTMT_CONST, sccp::VLR);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_msc")] = new CfgTranslationItem(CTMT_CONST, sccp::MSC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_eic")] = new CfgTranslationItem(CTMT_CONST, sccp::EIC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_auc")] = new CfgTranslationItem(CTMT_CONST, sccp::AUC);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_isdn_sup")] = new CfgTranslationItem(CTMT_CONST, sccp::ISDN_SUPPLEMENTARY);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_international")] = new CfgTranslationItem(CTMT_CONST, sccp::RESERVED_FOR_INTERNATIONAL_USE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_broadband_isdn")] = new CfgTranslationItem(CTMT_CONST, sccp::BROADBAND_ISDN_EDGE_TO_EDGE);
    cfg_trans_map[pmink_utils::hash_fnv1a_str("sccp.const.ssn_tc_test")] = new CfgTranslationItem(CTMT_CONST, sccp::TC_TEST_RESPONDER);


}


// CfgTranslationItem
fgn::CfgTranslationItem::CfgTranslationItem(CfgTransMapType _type, uint32_t _value): type(_type), value(_value){}
fgn::CfgTranslationItem::CfgTranslationItem(): type(CTMT_VAR), value(0){}
fgn::CfgTranslationItem::~CfgTranslationItem(){}
uint32_t fgn::CfgTranslationItem::run(){ return 0; }

// CfgTITimestamp
fgn::CfgTITimestamp::CfgTITimestamp(): fgn::CfgTranslationItem(CTMT_DYNAMIC, 0){}
uint32_t fgn::CfgTITimestamp::run(){
    return time(NULL);
}

// FgnPayload
fgn::FgnPayload::FgnPayload(): current_vp_id(0){}


void fgn::FgnPayload::reset(){
    params.clear_params();
    cmd_params.clear_params();
    params_p = &params;
    tcap_op_index = 0;
    //sms_tcap_op_index = 0;
    //hlr_tcap_op_index = 0;
    route_index = 0;
    phase = CPT_UNKNOWN;
    guid = 0;
    rrp_retry = 0;
    rrp_maxr = 3;
    route_hlr.reset();
    refc.set(0);
    current_vp_id.key = 0;
    current_vp_id.index = 0;
    current_vp_id.fragment = 0;
    current_vp_id.context = 0;
    rproc = NULL;
}

timespec fgn::FgnPayload::get_pmink_ts(pmink_utils::PooledVPMap<uint32_t>* params){
    timespec ts = {time(NULL), 0};
    if(params != NULL){
	pmink_utils::VariantParam* vp1 = params->get_param(asn1::ParameterType::_pt_pmink_timestamp);
	pmink_utils::VariantParam* vp2 = params->get_param(asn1::ParameterType::_pt_pmink_timestamp_nsec);
	if(vp1 != NULL) ts.tv_sec = (int)*vp1;
	if(vp2 != NULL) ts.tv_nsec = (int)*vp2;
    }
    return ts;
}


int fgn::FgnPayload::get_mapv(pmink_utils::PooledVPMap<uint32_t>* params){
    // check context
    pmink_utils::VariantParam* vp = params->get_param(asn1::ParameterType::_pt_tcap_dialogue_context_oid);
    if(vp != NULL){
	// supported oids:
	// ===============
	// 0.4.0.0.1.0.25.3 - shortMsgMT-RelayContext-v3
	// 0.4.0.0.1.0.25.2 - shortMsgMT-RelayContext-v2

	// 0.4.0.0.1.0.20.3 - shortMsgGatewayContext-v3
	// 0.4.0.0.1.0.20.2 - shortMsgGatewayContext-v2
	// 0.4.0.0.1.0.20.1 - shortMsgGatewayContext-v1

	// 0.4.0.0.1.0.21.3 - shortMsgMO-RelayContext-v3
	// 0.4.0.0.1.0.21.2 - shortMsgMO-RelayContext-v2
	// 0.4.0.0.1.0.21.1 - shortMsgRelayContext-v1

	// str pointer
	char* ctx_oid = (char*)*vp;

	// v3
	if(strcmp(ctx_oid, "0.4.0.0.1.0.25.3") == 0 ||
	   strcmp(ctx_oid, "0.4.0.0.1.0.20.3") == 0 ||
	   strcmp(ctx_oid, "0.4.0.0.1.0.21.3") == 0) return 3;


	// v2
	else if(strcmp(ctx_oid, "0.4.0.0.1.0.25.2") == 0 ||
		strcmp(ctx_oid, "0.4.0.0.1.0.20.2") == 0 ||
		strcmp(ctx_oid, "0.4.0.0.1.0.21.2") == 0) return 2;

	// v1
	else if(strcmp(ctx_oid, "0.4.0.0.1.0.20.1") == 0 ||
		strcmp(ctx_oid, "0.4.0.0.1.0.21.1") == 0) return 1;

    }

    return 1;
}


int fgn::FgnPayload::process_expired(){
    // todo
    return 100;
}
int fgn::FgnPayload::send_rate_sri_req(){
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;

    // get RRP connection
    dd->rating_map.wrr_mtx.lock();
    pmink_utils::WRRItem<rrp::RRConnection*>* wrr_rrc = dd->rating_map.get_next_node();
    dd->rating_map.wrr_mtx.unlock();
    // error check
    if(!(wrr_rrc != NULL && wrr_rrc->item->registered.get())){
	// todo stats, error handling
	return 300;
    }
    // rr conn pointer
    rrp::RRConnection* rrc = wrr_rrc->item;
    // new sequence
    rrp::RRSequence* rrs = rrc->new_sequence(wrr_rrc->id, rrp::RRRT_FILTER_RATE);
    if(rrs == NULL) {
	// todo stats, error handling
	return 301;
    }

    // *** sequence params ***
    // guid
    rrs->pdu.params.set_octets(rrp::RRPT_GUID, &guid, sizeof(guid));
    // type
    rrs->pdu.params.set_int(rrp::RRPT_TYPE, rrp::RRTT_SRI_SM);

    // ts
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_timestamp),
			pmink_utils::ParamTuple<>(rrp::RRPT_TIMESTAMP));


    // ts nsec
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_timestamp_nsec),
			pmink_utils::ParamTuple<>(rrp::RRPT_TIMESTAMP_NSEC));

    // src trunk
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_trunk_label, 2),
			pmink_utils::ParamTuple<>(rrp::RRPT_SRC_TRUNK));

    // opc
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code),
			pmink_utils::ParamTuple<>(rrp::RRPT_OPC));
    // dpc
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code),
			pmink_utils::ParamTuple<>(rrp::RRPT_DPC));
    // cgpa
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_GT_CALLING));
    // cgpa ssn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLING_SSN));

    // cgpa ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLING_TON));

    // cgpa npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLING_NPI));


    // cdpa
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_GT_CALLED));
    // cdpa ssn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLED_SSN));

    // cdpa ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLED_TON));

    // cdpa npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLED_NPI));

    // imsi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_imsi, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_IMSI));

    // mapv
    rrs->pdu.params.set_int(rrp::RRPT_MAP_VERSION, get_mapv(&params));

    // tcap sid
    pmink_utils::VariantParam* vp = params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
    if(vp != NULL){
	uint32_t sid = htobe32((int)*vp);
	int br = pmink_utils::bytes_required(sid);
	rrs->pdu.params.set_octets(rrp::RRPT_TCAP_SID, &sid, br);

    }

    // tcap did
    vp = params.get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id);
    if(vp != NULL){
	uint32_t did = htobe32((int)*vp);
	int br = pmink_utils::bytes_required(did);
	rrs->pdu.params.set_octets(rrp::RRPT_TCAP_DID, &did, br);

    }

    // tcap dialogue oid
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_tcap_dialogue_context_oid),
			pmink_utils::ParamTuple<>(rrp::RRPT_TCAP_OID));

    // sca
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_sca_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SCA));

    // msisdn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_MSISDN));




    // latency (diff)
    // sri in
    timespec ts1 = get_pmink_ts(&params);
    /*
    timespec ts1 = {(int)*params.get_param(asn1::ParameterType::_pt_pmink_timestamp),
		    (int)*params.get_param(asn1::ParameterType::_pt_pmink_timestamp_nsec)};
		    */

    // now
    timespec ts2;
    clock_gettime(CLOCK_REALTIME, &ts2);

    // time diff
    timespec ltc = pmink_utils::time_diff(ts1, ts2);
    rrs->pdu.params.set_int(rrp::RRPT_LATENCY, ltc.tv_sec);
    rrs->pdu.params.set_int(rrp::RRPT_LATENCY_NSEC, ltc.tv_nsec);


    // packet data
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_m3ua_header_data),
			pmink_utils::ParamTuple<>(rrp::RRPT_PACKET_DATA));

    // send
    rrs->send();
    // ok
    return 0;

}

int fgn::FgnPayload::send_rate_sms(){
    pmink_utils::VariantParam* vp = params.get_param(asn1::ParameterType::_pt_pmink_connection_type);
    if(vp == NULL) return 1;
    switch((int)*vp){
	case sgn::CT_M3UA:
	    return send_rate_ss7_sms();

	case sgn::CT_SMPP:
	    // todo
	    break;

	default: return 1;
    }
    return 0;
}

int fgn::FgnPayload::send_rate_ss7_sms(){
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;

    // get RRP connection
    dd->rating_map.wrr_mtx.lock();
    pmink_utils::WRRItem<rrp::RRConnection*>* wrr_rrc = dd->rating_map.get_next_node();
    dd->rating_map.wrr_mtx.unlock();
    // error check
    if(!(wrr_rrc != NULL && wrr_rrc->item->registered.get())){
	// todo stats, error handling
	return 300;
    }
    // rr conn pointer
    rrp::RRConnection* rrc = wrr_rrc->item;
    // new sequence
    rrp::RRSequence* rrs = rrc->new_sequence(wrr_rrc->id, rrp::RRRT_FILTER_RATE);
    if(rrs == NULL) {
	// todo stats, error handling
	return 301;
    }

    // *** sequence params ***
    // guid
    rrs->pdu.params.set_octets(rrp::RRPT_GUID, &guid, sizeof(guid));
    // type
    pmink_utils::VariantParam* vp = params.get_param(asn1::ParameterType::_pt_tcap_opcode, tcap_op_index);
    if(vp != NULL){
	switch((int)*vp){
	    // mt
	    case asn1::gsmmap::GSMMAPOperationLocalvalue::_mt_forwardSM:
		rrs->pdu.params.set_int(rrp::RRPT_TYPE, rrp::RRTT_MT_FWD_SM);
		break;

	    // mo
	    case asn1::gsmmap::GSMMAPOperationLocalvalue::_mo_forwardSM:
		rrs->pdu.params.set_int(rrp::RRPT_TYPE, rrp::RRTT_MO_FWD_SM);
		break;

	}
    }
    // ts
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_timestamp),
			pmink_utils::ParamTuple<>(rrp::RRPT_TIMESTAMP));


    // ts nsec
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_timestamp_nsec),
			pmink_utils::ParamTuple<>(rrp::RRPT_TIMESTAMP_NSEC));

    // src trunk
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_trunk_label, 2),
			pmink_utils::ParamTuple<>(rrp::RRPT_SRC_TRUNK));

    // opc
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code),
			pmink_utils::ParamTuple<>(rrp::RRPT_OPC));
    // dpc
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code),
			pmink_utils::ParamTuple<>(rrp::RRPT_DPC));
    // cgpa
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_GT_CALLING));
    // cgpa ssn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLING_SSN));

    // cgpa ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLING_TON));

    // cgpa npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLING_NPI));


    // cdpa
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_GT_CALLED));
    // cdpa ssn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLED_SSN));

    // cdpa ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLED_TON));

    // cdpa npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLED_NPI));

    // imsi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_imsi, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_IMSI));

    // mapv
    rrs->pdu.params.set_int(rrp::RRPT_MAP_VERSION, get_mapv(&params));

    // tcap sid
    vp = params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
    if(vp != NULL){
	uint32_t sid = htobe32((int)*vp);
	int br = pmink_utils::bytes_required(sid);
	rrs->pdu.params.set_octets(rrp::RRPT_TCAP_SID, &sid, br);

    }

    // tcap did
    vp = params.get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id);
    if(vp != NULL){
	uint32_t did = htobe32((int)*vp);
	int br = pmink_utils::bytes_required(did);
	rrs->pdu.params.set_octets(rrp::RRPT_TCAP_DID, &did, br);

    }

    // tcap dialogue oid
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_tcap_dialogue_context_oid),
			pmink_utils::ParamTuple<>(rrp::RRPT_TCAP_OID));

    // scoa
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_scoa_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SCOA));

    // scoa ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_scoa_type_of_number, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SCOA_TON));

    // scoa npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_scoa_numbering_plan, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SCOA_NPI));

    // scda
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_scda_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SCDA));

    // scda ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_scda_type_of_number, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SCDA_TON));

    // scda npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_scda_numbering_plan, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SCDA_NPI));

    // msisdn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_msisdn_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_MSISDN));

    // nnn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_nnn_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_NNN));

    // an
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_an_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_AN));

    // tp-oa
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_tp_oa_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_OA_ADDR));

    // tp-oa ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_tp_oa_type_of_number, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_OA_TON));

    // tp-oa npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_tp_oa_numbering_plan, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_OA_NPI));

    // tp-da
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_tp_da_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_DA_ADDR));

    // tp-da ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_tp_da_type_of_number, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_DA_TON));

    // tp-da npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_tp_da_numbering_plan, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_DA_NPI));

    // ie msg ref
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_msg_id, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_CONCAT_MSG_ID));

    // ie msg partnum
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_msg_part, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_CONCAT_PARTNUM));

    // ie msg parts
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_msg_parts, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_CONCAT_PARTS));

    // dcs
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_tp_dcs, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_DCS));

    // sms text
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_smstpdu_tp_ud, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_SMS_TEXT));

    // filter result
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_filter_result),
			pmink_utils::ParamTuple<>(rrp::RRPT_FILTER_RESULT));

    // filter exit rule
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_filter_exit),
			pmink_utils::ParamTuple<>(rrp::RRPT_FILTER_EXIT_RULE));


    // latency (diff)
    // sms in
    timespec ts1 = get_pmink_ts(&params);
    /*
    timespec ts1 = {(int)*params.get_param(asn1::ParameterType::_pt_pmink_timestamp),
		    (int)*params.get_param(asn1::ParameterType::_pt_pmink_timestamp_nsec)};
		    */

    // now
    timespec ts2;
    clock_gettime(CLOCK_REALTIME, &ts2);

    // time diff
    timespec ltc = pmink_utils::time_diff(ts1, ts2);
    rrs->pdu.params.set_int(rrp::RRPT_LATENCY, ltc.tv_sec);
    rrs->pdu.params.set_int(rrp::RRPT_LATENCY_NSEC, ltc.tv_nsec);


    // packet data
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_m3ua_header_data),
			pmink_utils::ParamTuple<>(rrp::RRPT_PACKET_DATA));

    // send
    rrs->send();
    // ok
    return 0;
}

int fgn::FgnPayload::send_rate_rpl(){
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;

    // get RRP connection
    dd->rating_map.wrr_mtx.lock();
    pmink_utils::WRRItem<rrp::RRConnection*>* wrr_rrc = dd->rating_map.get_next_node();
    dd->rating_map.wrr_mtx.unlock();
    // error check
    if(!(wrr_rrc != NULL && wrr_rrc->item->registered.get())){
	// todo stats, error handling
	return 300;
    }
    // rr conn pointer
    rrp::RRConnection* rrc = wrr_rrc->item;
    // new sequence
    rrp::RRSequence* rrs = rrc->new_sequence(wrr_rrc->id, rrp::RRRT_FILTER_RATE);
    if(rrs == NULL) {
	// todo stats, error handling
	return 301;
    }

    // *** sequence params ***
    // guid
    rrs->pdu.params.set_octets(rrp::RRPT_GUID, &guid, sizeof(guid));
    // type
    rrs->pdu.params.set_int(rrp::RRPT_TYPE, rrp::RRTT_RETURN_RESULT);

    // ts
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_timestamp),
			pmink_utils::ParamTuple<>(rrp::RRPT_TIMESTAMP));


    // ts nsec
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_timestamp_nsec),
			pmink_utils::ParamTuple<>(rrp::RRPT_TIMESTAMP_NSEC));

    // src trunk
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_trunk_label, 2),
			pmink_utils::ParamTuple<>(rrp::RRPT_SRC_TRUNK));

    // opc
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_m3ua_protocol_data_originating_point_code),
			pmink_utils::ParamTuple<>(rrp::RRPT_OPC));
    // dpc
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_m3ua_protocol_data_destination_point_code),
			pmink_utils::ParamTuple<>(rrp::RRPT_DPC));
    // cgpa
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_GT_CALLING));
    // cgpa ssn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLING_SSN));

    // cgpa ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLING_TON));

    // cgpa npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLING_NPI));


    // cdpa
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_GT_CALLED));
    // cdpa ssn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLED_SSN));

    // cdpa ton
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLED_TON));

    // cdpa npi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan),
			pmink_utils::ParamTuple<>(rrp::RRPT_CALLED_NPI));

    // mapv
    rrs->pdu.params.set_int(rrp::RRPT_MAP_VERSION, get_mapv(&params));

    // tcap sid
    pmink_utils::VariantParam* vp = params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
    if(vp != NULL){
	uint32_t sid = htobe32((int)*vp);
	int br = pmink_utils::bytes_required(sid);
	rrs->pdu.params.set_octets(rrp::RRPT_TCAP_SID, &sid, br);

    }

    // tcap did
    vp = params.get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id);
    if(vp != NULL){
	uint32_t did = htobe32((int)*vp);
	int br = pmink_utils::bytes_required(did);
	rrs->pdu.params.set_octets(rrp::RRPT_TCAP_DID, &did, br);

    }

    // tcap dialogue oid
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_tcap_dialogue_context_oid),
			pmink_utils::ParamTuple<>(rrp::RRPT_TCAP_OID));


    // nnn
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_nnn_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_NNN));

    // an
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_an_digits, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_AN));

    // imsi
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_imsi, tcap_op_index),
			pmink_utils::ParamTuple<>(rrp::RRPT_IMSI));


    // default no error
    rrs->pdu.params.set_int(rrp::RRPT_ERROR_TYPE, 0);
    rrs->pdu.params.set_int(rrp::RRPT_ERROR_CODE, 0);

    // check error type
    vp = params.get_param(asn1::ParameterType::_pt_tcap_error_type, tcap_op_index);
    if(vp != NULL) rrs->pdu.params.set_int(rrp::RRPT_ERROR_TYPE, (int)*vp);

    // check error code
    vp = params.get_param(asn1::ParameterType::_pt_tcap_error_code, tcap_op_index);
    if(vp != NULL) rrs->pdu.params.set_int(rrp::RRPT_ERROR_CODE, (int)*vp);

    // latency (diff)
    // sms in
    timespec ts1 = get_pmink_ts(&params);
    /*
    timespec ts1 = {(int)*params.get_param(asn1::ParameterType::_pt_pmink_timestamp),
		    (int)*params.get_param(asn1::ParameterType::_pt_pmink_timestamp_nsec)};
		    */

    // now
    timespec ts2;
    clock_gettime(CLOCK_REALTIME, &ts2);

    // time diff
    timespec ltc = pmink_utils::time_diff(ts1, ts2);
    rrs->pdu.params.set_int(rrp::RRPT_LATENCY, ltc.tv_sec);
    rrs->pdu.params.set_int(rrp::RRPT_LATENCY_NSEC, ltc.tv_nsec);


    // packet data
    rrs->pdu.params.set(params,
			pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_m3ua_header_data),
			pmink_utils::ParamTuple<>(rrp::RRPT_PACKET_DATA));

    // send
    rrs->send();
    // ok
    return 0;
}




// RRPNodeMap
pmink_utils::WRRItem<rrp::RRConnection*>* fgn::RRPNodeMap::get_next_node(){
    pmink_utils::WRRItem<rrp::RRConnection*>* wrr_item = wrr.run();
    return wrr_item;

}

pmink_utils::WRR<rrp::RRConnection*>* fgn::RRPNodeMap::get_wrr(){
    return &wrr;
}


// FgndDescriptor
FgndDescriptor::FgndDescriptor(const char* _type, const char* _desc): pmink::DaemonDescriptor(_type, NULL, _desc),
								      cfgd_ovrd_c(std::numeric_limits<uint32_t>::max()),
								      r14ps(NULL),
								      r14p_stats(NULL),
								      r14psmm(NULL),
								      cfgd_r14pc(NULL),
								      hbeat(NULL),
								      non_srvc_hdnlr(NULL),
								      stp_r14pc(NULL),
								      pd_r14pc(NULL),
								      cmap_timer_res(10),
								      rrp_s("", 5, 1000){

	config = new config::Config();
	bzero(cfgd_id, sizeof(cfgd_id));

	// default extra param values
	// --r14p-streams
	extra_params.set_int(0, 1000);
	// --r14p-stimeout
	extra_params.set_int(1, 5);
	// --r14p-smsg-pool
	extra_params.set_int(2, 1000);
	// --r14p-sparam-pool
	extra_params.set_int(3, 5000);
	// --rrp-stimeout
	extra_params.set_int(4, 5);
	// --rrp-sequences
	extra_params.set_int(5, 1000);


	// stats
	stats.register_item(fgn::ST_UNSUPPORTED_R14P_SERVICE);
	stats.register_item(fgn::ST_SMSG_SEND_ERROR);
	stats.register_item(fgn::ST_SMSG_MSG_POOL_EMPTY);
	stats.register_item(fgn::ST_SMSG_PARAM_POOL_EMPTY);
	stats.register_item(fgn::ST_SMSG_INCOMPLETE);
	stats.register_item(fgn::ST_SMSG_RX_COUNT);
	stats.register_item(fgn::ST_FWORKER_NA);
	stats.register_item(fgn::ST_FGN_PAYLOAD_POOL_EMPTY);
	stats.register_item(fgn::ST_RPROC_POOL_EMPTY);
	stats.register_item(fgn::ST_TCAP_MULTIPLE_COMPONENTS);

	// random generator
	timespec tmp_time;
	clock_gettime(0, &tmp_time);
	ran_mt19937.seed(tmp_time.tv_nsec + tmp_time.tv_sec);
	random_generator = new boost::uuids::random_generator(ran_mt19937);


}

FgndDescriptor::~FgndDescriptor(){
	// service message manager
	delete r14psmm;
	// r14p stats
	delete r14p_stats;
	// random generator
	delete random_generator;

}

void FgndDescriptor::process_arguements(int argc, char** argv){
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");
	int opt;
	int option_index = 0;
	struct option long_options[] = {
	    {"r14p-streams", required_argument, 0, 0},
	    {"r14p-stimeout", required_argument, 0, 0},
	    {"r14p-smsg-pool", required_argument, 0, 0},
	    {"r14p-sparam-pool", required_argument, 0, 0},
	    {"rrp-stimeout", required_argument, 0, 0},
	    {"rrp-sequences", required_argument, 0, 0},
	    {0, 0, 0, 0}
	};


	if(argc < 5){
		print_help();
		exit(EXIT_FAILURE);
		return;
	}else{
		while ((opt = getopt_long(argc, argv, "?r:i:t:c:D", long_options, &option_index)) != -1) {
			switch(opt){
				// long options
				case 0:
				   if(long_options[option_index].flag != 0) break;
				   switch(option_index){
				       // r14p-streams
				       case 0:
					   extra_params.set_int(0, atoi(optarg));
					   break;

				       // r14p-stimeout
				       case 1:
					   extra_params.set_int(1, atoi(optarg));
					   break;

				       // r14p-smsg-pool
				       case 2:
					   extra_params.set_int(2, atoi(optarg));
					   break;

				       // r14p-sparam-pool
				       case 3:
					   extra_params.set_int(3, atoi(optarg));
					   break;

				       // rrp-stimeout
				       case 4:
					   extra_params.set_int(4, atoi(optarg));
					   break;

				       // rrp-sequences
				       case 5:
					   extra_params.set_int(5, atoi(optarg));
					   break;


				   }
				   break;

				// help

				case '?':
					print_help();
					exit(EXIT_FAILURE);


				// daemon id
				case 'i':
				    if(set_daemon_id(optarg) > 0){
					std::cout << "ERROR: Maximum size of daemon id string is 15 characters!" << std::endl;
					exit(EXIT_FAILURE);

				    }
				    break;

				// daemon type
				case 't':
				    if(set_daemon_type(optarg) > 0){
					std::cout << "ERROR: Maximum size of daemon type string is 15 characters!" << std::endl;
					exit(EXIT_FAILURE);

				    }
				    break;


				// routing daemon address
				case 'r':
				    // check pattern (ipv4:port)
				    // check if valid
				    if(!boost::regex_match(optarg, addr_regex)){
					    std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
					    exit(EXIT_FAILURE);

				    }else{
					    routing_daemons.push_back(std::string(optarg));

				    }
				    break;

				// config daemon address
				case 'c':
				    // check pattern (ipv4:port)
				    // check if valid
				    if(!boost::regex_match(optarg, addr_regex)){
					    std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
					    exit(EXIT_FAILURE);

				    }else{
					    config_daemons.push_back(std::string(optarg));
					    cfgd_ovrd_c = 0;

				    }
				    break;

				// debug mode
				case 'D':
				    set_log_level(pmink::LLT_DEBUG);
				    break;


			}
		}

		// check mandatory id
		if(strlen(get_daemon_id()) == 0){
			std::cout << "ERROR: Daemon id not defined!" << std::endl;
			exit(EXIT_FAILURE);

		}

		// check for config daemons
		if(routing_daemons.size() == 0){
			std::cout << "ERROR: Routing daemon(s) not defined!" << std::endl;
			exit(EXIT_FAILURE);

		}


	}

}

void FgndDescriptor::print_help(){
	std::cout << daemon_type << " - " << daemon_description << std::endl;
	std::cout << "Copyright (c) 2012 Release14.org" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << " -?\thelp" << std::endl;
	std::cout << " -i\tunique daemon id" << std::endl;
	std::cout << " -t\tdaemon type override" << std::endl;
	std::cout << " -r\trouting daemon address (ipv4:port)" << std::endl;
	std::cout << " -c\tconfig daemon address (ipv4:port)" << std::endl;
	std::cout << " -D\tstart in debug mode" << std::endl;
	std::cout << std::endl;
	std::cout << "R14P Options:" << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << " --r14p-streams\t\tR14P Session stream pool\t\t(default = 1000)" << std::endl;
	std::cout << " --r14p-stimeout\tR14P Stream timeout in seconds\t\t(default = 5)" << std::endl;
	std::cout << " --r14p-smsg-pool\tR14P Service message pool\t\t(default = 1000)" << std::endl;
	std::cout << " --r14p-sparam-pool\tR14P Service message parameter pool\t(default = 5000)" << std::endl;
	std::cout << std::endl;
	std::cout << "RRP Options:" << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << " --rrp-stimeout\t\tRRP Sequence timeout in seconds\t\t(default = 5)" << std::endl;
	std::cout << " --rrp-sequences\tRRP Session sequence pool\t\t(default = 1000)" << std::endl;


}



void FgndDescriptor::init_rrp(){
    rrp_s.set_daemon_id(get_daemon_id());
    rrp_s.set_seq_timeout((int)*extra_params.get_param(4));
    rrp_s.set_seq_pool((int)*extra_params.get_param(5));
}


void FgndDescriptor::init_r14p(){
	// handler for non service message messages
	non_srvc_hdnlr = config::create_cfg_event_handler(config);
	// service message manager
	r14psmm = new r14p::ServiceMsgManager(&idt_map,
					      NULL,
					      non_srvc_hdnlr,
					      (int)*extra_params.get_param(2),
					      (int)*extra_params.get_param(3));

	// set daemon params (searched for in dr::NewSrvcMsg constructor)
	set_param(0, config);
	set_param(1, r14psmm);

	// set service message handlers
	r14psmm->set_new_msg_handler(&new_srvc_msg);
	r14psmm->set_msg_err_handler(&new_srvc_msg.msg_err);

	// start R14P session
	r14ps = r14p::init_session(get_daemon_type(),
				   get_daemon_id(),
				   (int)*extra_params.get_param(0),
				   (int)*extra_params.get_param(1),
				   false,
				   (int)*extra_params.get_param(1));
	set_param(2, r14ps);

	// r14p stats
	r14p_stats = new r14p::R14PStatsSession(5, r14ps);
	set_param(3, r14p_stats);

	// start stats
	r14p_stats->start();

	// init r14p stats
	r14p_stats->init_r14p_session_stats(r14ps);

	// init stats
	r14p_stats->add_trap(r14p::TrapId("UNSUPPORTED_R14P_SERVICE"), new fgn::StatsHandler(&stats, fgn::ST_UNSUPPORTED_R14P_SERVICE));
	r14p_stats->add_trap(r14p::TrapId("SMSG_SEND_ERROR"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_SEND_ERROR));
	r14p_stats->add_trap(r14p::TrapId("SMSG_MSG_POOL_EMPTY"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_MSG_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("SMSG_PARAM_POOL_EMPTY"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_PARAM_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("SST_SMSG_INCOMPLETE"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_INCOMPLETE));
	r14p_stats->add_trap(r14p::TrapId("SMSG_RX_COUNT"), new fgn::StatsHandler(&stats, fgn::ST_SMSG_RX_COUNT));
	r14p_stats->add_trap(r14p::TrapId("FWORKER_NA"), new fgn::StatsHandler(&stats, fgn::ST_FWORKER_NA));
	r14p_stats->add_trap(r14p::TrapId("FGN_PAYLOAD_POOL_EMPTY"), new fgn::StatsHandler(&stats, fgn::ST_FGN_PAYLOAD_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("RPROC_POOL_EMPTY"), new fgn::StatsHandler(&stats, fgn::ST_RPROC_POOL_EMPTY));
	r14p_stats->add_trap(r14p::TrapId("TCAP_MULTIPLE_COMPONENTS"), new fgn::StatsHandler(&stats, fgn::ST_TCAP_MULTIPLE_COMPONENTS));


	// connect to routing daemons
	boost::smatch regex_groups;
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");

	// loop config daemons
	for(unsigned int i = 0; i<config_daemons.size(); i++){
	    // separate IP and PORT
	    boost::regex_search(config_daemons[i], regex_groups, addr_regex);
	    // connect to routing daemon
	    r14p::R14PClient* r14pc = r14ps->connect(regex_groups[1].str().c_str(),
						     atoi(regex_groups[2].str().c_str()),
						     16,
						     NULL,
						     0);

	    // setup client for service messages
	    if(r14pc != NULL) {
		// setup service message event handlers
		r14psmm->setup_client(r14pc);
		// inc configd override count
		++cfgd_ovrd_c;

	    }
	}


	// loop routing daemons
	for(unsigned int i = 0; i<routing_daemons.size(); i++){
	    // separate IP and PORT
	    boost::regex_search(routing_daemons[i], regex_groups, addr_regex);
	    // connect to routing daemon
	    r14p::R14PClient* r14pc = r14ps->connect(regex_groups[1].str().c_str(),
						     atoi(regex_groups[2].str().c_str()),
						     16,
						     NULL,
						     0);

	    // setup client for service messages
	    if(r14pc != NULL) {
		// setup service message event handlers
		r14psmm->setup_client(r14pc);

	    }

	    // connect stats with routing
	    r14pc = r14p_stats->get_r14p_session()->connect(regex_groups[1].str().c_str(), atoi(regex_groups[2].str().c_str()), 16, NULL, 0);
	    if(r14pc != NULL) r14p_stats->setup_client(r14pc);


	}


	// set stpd client
	stp_r14pc = r14ps->get_registered_client("routingd");
	// set pdnd client
	pd_r14pc = r14ps->get_registered_client("routingd");

}

int FgndDescriptor::init_config(bool _process_config){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting config daemon registration procedure...");

	// create root node string
	std::string root_node_str(DAEMON_CFG_NODE);
	root_node_str.append(" ");
	root_node_str.append(daemon_id);

	// loop routing daemons
	for(unsigned int i = 0; i<r14ps->get_client_count(); i++){
		// check cfgd override count
		if(!(i < cfgd_ovrd_c)) continue;
		// get client
		r14p::R14PClient* r14p_client = r14ps->get_client(i);
		// null check
		if(r14p_client != NULL && r14p_client->is_registered()){
			// log
			pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, 	"Connection to remote daemon established, L3 address = [%s:%d], R14P address = [%s:%s]",
									r14p_client->get_end_point_address(),
									r14p_client->get_end_point_port() ,
									r14p_client->get_end_point_daemon_type(),
									r14p_client->get_end_point_daemon_id());
			// check for active configd
			if(!cfgd_active.get()){
				// user login
				if(config::user_login(config, r14p_client, NULL, (char*)cfgd_id, &cfgd_uid) == 0){
				    if(strlen((char*)cfgd_id) > 0){
					// log
					pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "User [%s] successfully authenticated with config daemon [%s]", cfgd_uid.user_id, cfgd_id);
					// notification request
					if(config::notification_request(config, r14p_client, root_node_str.c_str(), NULL, (char*)cfgd_id, &cfgd_uid, NULL) == 0){
						// create hbeat events
						HbeatRecv* hb_recv = new HbeatRecv();
						HbeatMissed* hb_missed = new HbeatMissed(&cfgd_active);
						HbeatCleanup* hb_cleanup = new HbeatCleanup(hb_recv, hb_missed);

						// init hbeat
						hbeat = r14p::init_heartbeat("config_daemon", (char*)cfgd_id, r14p_client, 5, hb_recv, hb_missed, hb_cleanup);
						if(hbeat != NULL){
							cfgd_active.comp_swap(false, true);
							// log
							pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting R14P HBEAT for config daemon [%s], L3 address = [%s:%d]",
										   cfgd_id,
										   r14p_client->get_end_point_address(),
										   r14p_client->get_end_point_port());

						// free event memory on error
						}else{
						    delete hb_recv;
						    delete hb_missed;
						    delete hb_cleanup;
						}


						// log
						pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Registering notification request for node path [%s] with config daemon [%s]", root_node_str.c_str(), cfgd_id);
						// process config
						if(_process_config) process_config();

						// stop if config daemon connected
						// ok
						return 0;

					}else{
					    // log
					    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,	"Error while requesting notifications from config daemon [%s]!",
												cfgd_id);

					}

				    }else{
					    // log
					    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,	"Error while trying to find config daemon id via R14P connection, L3 address = [%s:%d], R14P address = [%s:%s]",
												r14p_client->get_end_point_address(),
												r14p_client->get_end_point_port() ,
												r14p_client->get_end_point_daemon_type(),
												r14p_client->get_end_point_daemon_id());

				    }

				}else{
					// log
					pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, 	"Error while trying to authenticate user [%s] with config daemon [%s:%d]!",
											cfgd_uid.user_id,
											r14p_client->get_end_point_address(),
											r14p_client->get_end_point_port());

				}

			}

		}else{
			// log
			pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Error while connecting to config daemon [%s:%d]!", r14p_client->get_end_point_address(), r14p_client->get_end_point_port());
		}


	}
	// err
	return 5;
}


void FgndDescriptor::init(){
    // pmdebug
    #ifdef PMDEBUG
    std::map<uint32_t, std::string>* debug_labels = pmink_utils::create_debug_label_map();
    set_param(9999, debug_labels);
    #endif

    // init r14p
    init_r14p();
    // init rrp
    init_rrp();
    // init config
    if(init_config() != 0){
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Cannot find any valid config daemon connection, terminating...");
	exit(EXIT_FAILURE);
    }
}


void FgndDescriptor::process_config(){
    // create root node string
    std::string root_node_str(DAEMON_CFG_NODE);
    root_node_str.append(" ");
    root_node_str.append(daemon_id);

    // get node
    config::ConfigItem* root = (*config->get_definition_root())(root_node_str.c_str());

    // check if configuration exists
    if(root == NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Configuration root node [%s] does not exist!", daemon_id);
	exit(EXIT_FAILURE);
    }

    // process configuration
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Configuration for node [%s] successfully received, verifying...", daemon_id);


    // configuration ok, process
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Configuration for node [%s] successfully verified, starting...", daemon_id);

    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting up routing connections...");

    // pools and timeouts
    cpool.init(root->to_int("pool", 100));
    cpool.construct_objects();
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting correlation pool size to [%d]...", cpool.get_chunk_count());

    // set standard list max for master list manager
    (*list_m).set_max_size(root->to_int("sl_list_max", 1000));
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting max size for standard lists to [%d]", root->to_int("sl_list_max", 1000));
    // set flood list max size for master list manager
    (*flood_m).set_max_size(root->to_int("fl_list_max", 1000));
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting max size for flood lists to [%d]", root->to_int("fl_list_max", 1000));
    // set flood list max TTL for master list manager
    (*flood_m).set_max_interval(root->to_int("fl_list_ttl", 3600));
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting max TTL for flood lists to [%d]", root->to_int("fl_list_ttl", 3600));

    // init rule processors
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting rule processor pool size to [%d]...", cpool.get_chunk_count());
    rproc_pool.init(root->to_int("pool", 100));
    rproc_pool.construct_objects();
    // set masters and max values
    for(int i = 0; i<rproc_pool.get_chunk_count(); i++){
	// masters
	rproc_pool.get_constructed(i)->rule_m.set_master(&rule_m);
	rproc_pool.get_constructed(i)->list_m.set_master(&list_m);
	rproc_pool.get_constructed(i)->flood_m.set_master(&flood_m);
	// standard lists max size
	(*rproc_pool.get_constructed(i)->list_m).set_max_size(root->to_int("sl_list_max", 1000));
	// flood lists max size
	(*rproc_pool.get_constructed(i)->flood_m).set_max_size(root->to_int("fl_list_max", 1000));
	// flood lists max TTL
	(*rproc_pool.get_constructed(i)->flood_m).set_max_interval(root->to_int("fl_list_ttl", 3600));
    }

    // correlation map timeout
    cmap.set_timeout(root->to_int("timeout", 100));
    cmap_tcap.set_timeout(root->to_int("timeout", 100));
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting correlation map default timeout to [%d] seconds...", cmap.get_timeout());
    // correlation map timer resolution
    cmap_timer_res = root->to_int("timer_res", 10);
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting correlation map timer resolution to [%d] seconds...", cmap_timer_res);

    // err count
    int errc = 0;

    // routing nodes
    if((*root)("routing") == NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Missing routing configuration node set!");
	exit(EXIT_FAILURE);

    }else{
	config::ConfigItem* routing_nodes = (*root)("routing");
	// check if empty
	if(routing_nodes->children.size() == 0) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Routing configuration node set is empty!");
	// check all nodes
	config::ConfigItem* routing_node = NULL;
	for(unsigned int i = 0; i<routing_nodes->children.size(); i++){
	    errc = 0;
	    routing_node = routing_nodes->children[i];
	    // check for mandatory routing node params
	    if(!routing_node->val_exists("remote address")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									     "Missing mandatory configuration item [routing %s remote address]!",
									     routing_node->name.c_str(), errc++);

	    if(!routing_node->val_exists("remote port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									     "Missing mandatory configuration item [routing %s remote port]!",
									     routing_node->name.c_str(), errc++);

	    if(!routing_node->val_exists("weight")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									     "Missing mandatory configuration item [routing %s weight]!",
									     routing_node->name.c_str(), errc++);

	    // err check
	    if(errc > 0){
		pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
					   "Cannot add routing node [%s], missing mandatory configuration items!",
					   routing_node->name.c_str());


	    // init rrp connection
	    }else{
		// connect
		rrp::RRConnection* rrp_c = rrp_s.connect(routing_node->to_cstr("remote address"),
							 routing_node->to_int("remote port"),
							 routing_node->to_cstr("local address"),
							 routing_node->to_int("local port"));

		// handle error
		if(rrp_c == NULL){
		    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
					       "Cannot establish RRP connection to routing node [%s]!",
					       routing_node->name.c_str());

		    continue;
		}

		// add to list
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Adding RRP connection [%s] with weight [%d]...",
					   routing_node->name.c_str(), routing_node->to_int("weight"));
		routing_map.wrr_mtx.lock();
		routing_map.get_wrr()->add_item(rrp_c, routing_node->name.c_str(), routing_node->to_int("weight"));
		routing_map.wrr_mtx.unlock();


	    }
	}
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Finished setting up routing connections...");
    }

    // rating nodes
    if((*root)("rating") == NULL){
	pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, "Missing rating configuration node set!");
	exit(EXIT_FAILURE);

    }else{
	config::ConfigItem* rating_nodes = (*root)("rating");
	// check if empty
	if(rating_nodes->children.size() == 0) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Rating configuration node set is empty!");
	// check all nodes
	config::ConfigItem* rating_node = NULL;
	for(unsigned int i = 0; i<rating_nodes->children.size(); i++){
	    errc = 0;
	    rating_node = rating_nodes->children[i];
	    // check for mandatory routing node params
	    if(!rating_node->val_exists("remote address")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									     "Missing mandatory configuration item [rating %s remote address]!",
									     rating_node->name.c_str(), errc++);

	    if(!rating_node->val_exists("remote port")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									     "Missing mandatory configuration item [rating %s remote port]!",
									     rating_node->name.c_str(), errc++);

	    if(!rating_node->val_exists("weight")) pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING,
									     "Missing mandatory configuration item [rating %s weight]!",
									     rating_node->name.c_str(), errc++);

	    // err check
	    if(errc > 0){
		pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
					   "Cannot add rating node node [%s], missing mandatory configuration items!",
					   rating_node->name.c_str());


	    // init rrp connection
	    }else{
		// connect
		rrp::RRConnection* rrp_c = rrp_s.connect(rating_node->to_cstr("remote address"),
							 rating_node->to_int("remote port"),
							 rating_node->to_cstr("local address"),
							 rating_node->to_int("local port"));

		// handle error
		if(rrp_c == NULL){
		    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR,
					       "Cannot establish RRP connection to rating node [%s]!",
					       rating_node->name.c_str());

		    continue;
		}

		// add to list
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Adding RRP connection [%s] with weight [%d]...",
					   rating_node->name.c_str(), rating_node->to_int("weight"));
		rating_map.wrr_mtx.lock();
		rating_map.get_wrr()->add_item(rrp_c, rating_node->name.c_str(), rating_node->to_int("weight"));
		rating_map.wrr_mtx.unlock();


	    }
	}
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Finished setting up rating connections...");
    }


    // build rules
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Building filtering rules...");
    rule_m.update_start();
    filter_m.build_rules((*root)("rules"), &(*rule_m));
    rule_m.update_end();
    // setup config on chage events
    (*root)("rules")->set_on_change_handler(&rules_mod_handler, true);
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Finished building filtering rules...");

    // build lists
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Building filtering lists...");
    list_m.update_start();
    filter_m.build_lists((*root)("lists"), &(*list_m));
    list_m.update_end();
    // setup config on chage events
    (*root)("lists")->set_on_change_handler(&lists_mod_handler, true);
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Finished building filtering lists...");

    // load plugins
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Load plugins...");
    init_plugins(root->to_cstr("plugin_dir"));
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Finished loading plugins...");

    // create filtering workers
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting rule processor threads...");
    int pcc = root->to_int("fworkers", 1);
    // pthread attr
    pthread_attr_t pi_thread_attr;
    // scheduling parameters
    pthread_attr_init(&pi_thread_attr);
    // explicit FIFO scheduling
    pthread_attr_setinheritsched(&pi_thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&pi_thread_attr, SCHED_FIFO);
    // priority
    sched_param pi_sp;
    // max priority
    pi_sp.__sched_priority = 99;
    // set priorities
    pthread_attr_setschedparam(&pi_thread_attr, &pi_sp);
    filter_m.thm.start();
    char tmp_str[16];

    // create workers
    for(int i = 0; i<pcc; i++){
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting rule processor thread [%d]...", i);
	sprintf(tmp_str, "fw_%d", i);
	fgn::FWorker* fw = new fgn::FWorker(i, root->to_int("pool", 100), &filter_m);
	filter_m.add_fw(fw, tmp_str, 1);
	filter_m.thm.new_thread(&filter_m.worker_loop, tmp_str, fw, &pi_thread_attr);
    }

    // create timeout thread
    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Starting rule processor timeout thread...");
    sprintf(tmp_str, "rpc_timeout");
    filter_m.thm.new_thread(&filter_m.timeout_loop, tmp_str, this, &pi_thread_attr);

    // destroy attributes
    pthread_attr_destroy(&pi_thread_attr);

}



int FgndDescriptor::generate_uuid(unsigned char* out){
    if(out == NULL) return 1;
    slock_uuid.lock();
    boost::uuids::uuid _uuid = (*random_generator)(); // operator ()
    memcpy(out, _uuid.data, 16);
    slock_uuid.unlock();
    return 0;

}

void FgndDescriptor::terminate(){
	// destroy session, free memory
	r14p::destroy_session(r14ps);
	// deallocate config memory
	if(config->get_definition_root() != NULL) delete config->get_definition_root();
	// free config
	delete config;
	// remove stats
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("UNSUPPORTED_R14P_SERVICE")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_SEND_ERROR")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_MSG_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_PARAM_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_INCOMPLETE")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("SMSG_RX_COUNT")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("FWORKER_NA")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("FGN_PAYLOAD_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("RPROC_POOL_EMPTY")));
	delete r14p_stats->remove_trap(r14p::TrapId(std::string("TCAP_MULTIPLE_COMPONENTS")));

	#ifdef PMDEBUG
	delete (std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999);
	#endif

}

void fgn::FilterManager::add_fw(FWorker* fw, const char* id, uint32_t weight){
    wrr_lock.lock();
    fworkers.add_item(fw, id, weight);
    wrr_lock.unlock();
}


fgn::FWorker* fgn::FilterManager::get_fw(){
    wrr_lock.lock();
    pmink_utils::WRRItem<fgn::FWorker*>* wrr_item = fworkers.run();
    fgn::FWorker* res = (wrr_item != NULL ? wrr_item->item : NULL);
    wrr_lock.unlock();
    return res;

}

void* fgn::FilterManager::timeout_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    time_t sleep_c = time(NULL);
    typedef pmink_utils::CorrelationMap<__uint128_t, FgnPayload*>::cmap_it_type cmap_it_type;
    typedef pmink_utils::CorrelationMap<uint32_t, __uint128_t>::cmap_it_type cmap_tcap_it_type;
    typedef pmink_utils::CorrelationMap<__uint128_t, __uint128_t>::cmap_it_type cmap_r14p_it_type;
    timespec ts = {dd->cmap_timer_res, 0};
    FWorker* fw = NULL;
    fgn::FilterManager* fm = &dd->filter_m;

    // loop
    while(ti->is_active()){
	// *** process timeouts ***
	if(time(NULL) - sleep_c > dd->cmap_timer_res){
	    // *** fgn correlations ***
	    // update timer
	    sleep_c = time(NULL);
	    // lock
	    dd->cmap.lock();
	    // current ts
	    time_t now = time(NULL);
	    // loop
	    for(cmap_it_type it = dd->cmap.begin(), it_next = it; it != dd->cmap.end(); it = it_next){
		// next
		++it_next;
		// skip if already referenced (rrp event handlers)
		if(it->second.data->refc.get() > 0) continue;

		// check if retrying rrp sequence (rrp err states 9000 - 9999)
		if(it->second.data->phase >= 9000 && it->second.data->phase <= 9999){
		    // process RRP error/timeout
		    if(it->second.data->process_expired() == 100){
			FgnPayload* tmpp = it->second.data;
			// remove from list
			dd->cmap.remove(it);
			// deallocate
			dd->cpool.deallocate_constructed(tmpp);
		    }
		    continue;
		}

		// check if expired (waiting for notification or reply from sgnd)
		if(now - it->second.ts <= it->second.data_timeout) continue;


		// process reply timeout
		if(it->second.data->process_expired() == 100){
		    FgnPayload* tmpp = it->second.data;

		    // get fw
		    fw = fm->get_fw();
		    if(fw == NULL){
			dd->stats.inc(fgn::ST_FWORKER_NA, 1);
			continue;
		    }

		    // inc refc
		    tmpp->refc.fetch_add(1);
		    // update phase
		    tmpp->phase = CPT_CONTINUE_FILTERING;
		    // update ts
		    dd->cmap.update_ts(it);
		    // send to worker queue
		    fw->queue.push(tmpp);
		}

	    }
	    dd->cmap.unlock();
	    // *** tcap correlations ***
	    // lock
	    dd->cmap_tcap.lock();
	    // loop
	    for(cmap_tcap_it_type it = dd->cmap_tcap.begin(), it_next = it; it != dd->cmap_tcap.end(); it = it_next){
		// next
		++it_next;
		// check if expired (waiting for notification or reply from sgnd)
		if(now - it->second.ts <= it->second.data_timeout) continue;

		// remove from list
		dd->cmap_tcap.remove(it);

	    }
	    dd->cmap_tcap.unlock();

	    // *** r14p correlations ***
	    // lock
	    dd->cmap_r14p.lock();
	    // loop
	    for(cmap_r14p_it_type it = dd->cmap_r14p.begin(), it_next = it; it != dd->cmap_r14p.end(); it = it_next){
		// next
		++it_next;
		// check if expired (waiting for notification or reply)
		if(now - it->second.ts <= it->second.data_timeout) continue;
		// remove from list
		dd->cmap_r14p.remove(it);
	    }
	    dd->cmap_r14p.unlock();


	// sleep
	}else{
	    nanosleep(&ts, NULL);

	}

    }

    return NULL;

}


void* fgn::FilterManager::worker_loop(void* args){
    pmink_utils::ThreadInfo* ti = (pmink_utils::ThreadInfo*)args;
    fgn::FWorker* fw = (fgn::FWorker*)ti->user_args;
    fgn::FilterManager* fm = fw->fm;
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    fgn::FgnPayload* pld = NULL;
    timespec ts = {0, 1000000};
    int res;
    rule_t* last_rule = NULL;
    __uint128_t tmp_guid = 0;
    __uint128_t* tmp_guid_p = NULL;

    // loop
    while(ti->is_active()){
	// check queue
	if(fw->queue.pop(&pld)){
	    // check pld phase
	    switch(pld->phase){
		// ss7 sms, smpp sms
		case CPT_SS7_SMS_FWD:
		case CPT_SMPP_SMS_FWD:
		{
		    // sync rules and lists with master
		    pld->rproc->sync();
		    // set pld pointer
		    pld->params_p->set_pointer(1, pld, 0, 0, RCT_PMINK);
		    // run rules
		    res = fm->run_rules(pld->params_p, &(*pld->rproc->rule_m), NULL, &last_rule);

		    // debug
		    #ifdef PMDEBUG
		    pld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
		    #endif
		    PMDLOG(
			std::cout << "===== CPT_SS7_SMS_FWD/CPT_SMPP_SMS_FWD =====" << std::endl;
			std::cout << pld->params;
			std::cout << "============================================" << std::endl;
		    )

		    // if pausing, do not deallocate (added to guid map in rule processor)
		    if(res == pmink_utils::RRT_CONTINUE_PAUSE){
			// dec refc (inc in method that paused filtering processor)
			pld->refc.sub_fetch(1);

		    // *** not pausing, filtering finished ***
		    }else{
			// *** send result ***
			// set filtering result
			pld->params.set_int(asn1::ParameterType::_pt_pmink_filter_result, res);
			// set last rule
			if(last_rule != NULL) pld->params.set_cstr(asn1::ParameterType::_pt_pmink_filter_exit, last_rule->get_label());
			// set tcap component index
			pld->params.set_int(asn1::ParameterType::_pt_tcap_component_index, pld->tcap_op_index);
			// if rejected, send error
			if(res != pmink_utils::RRT_STOP_OK){
			    pld->params.set_int(asn1::ParameterType::_pt_pmink_command_id, asn1::SmsHubCommandId::_shci_sms_ack);
			    pld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 10);

			    // set error code if not set in rules
			    if(pld->params.get_param(asn1::ParameterType::_pt_tcap_error_code) == NULL)
				pld->params.set_int(asn1::ParameterType::_pt_tcap_error_code, asn1::gsmmap::GSMMAPLocalErrorcode::_unknownSubscriber);

			    if(pld->params.get_param(asn1::ParameterType::_pt_smpp_command_status) == NULL)
				pld->params.set_int(asn1::ParameterType::_pt_smpp_command_status, smpp::ESME_RX_P_APPN);

			    // gt calling -> gt called
			    pmink_utils::VariantParam* vp = pld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address);
			    if(vp != NULL) pld->params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, (char*)*vp, 2);
			    // gt called -> gt calling
			    vp = pld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
			    if(vp != NULL) pld->params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_address, (char*)*vp, 2);
			    // calling ssn -> called ssn
			    vp = pld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);
			    if(vp != NULL) pld->params.set_int(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, (int)*vp, 2);
			    // gt called ssn -> gt calling ssn
			    vp = pld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);
			    if(vp != NULL) pld->params.set_int(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, (int)*vp, 2);
			    // pmink guid
			    pld->params.set_octets(asn1::ParameterType::_pt_pmink_guid, &pld->guid, sizeof(pld->guid));
			}
			// debug
			#ifdef PMDEBUG
			pld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
			#endif
			PMDLOG(
			    std::cout << "===== FILTERING FINISHED =====" << std::endl;
			    std::cout << pld->params;
			    std::cout << "==============================" << std::endl;
			)

			// send
			dd->send_stp(pld->params_p);

			// rate
			pld->send_rate_sms();

			// remove from guid map
			dd->cmap.lock();
			dd->cmap.remove(pld->guid);
			dd->cmap.unlock();
			// free rule processor
			dd->rproc_pool.deallocate_constructed(pld->rproc);
			// free pld
			dd->cpool.deallocate_constructed(pld);

		    }
		    // do not deallocate (already done if filtering finished)
		    continue;
		}

		// ss7 sms reply
		case CPT_SS7_SMS_RPL:
		{
		    // rate
		    pld->send_rate_rpl();
		    break;
		}

		// unknown result
		case CPT_UNKNOWN_RESULT:
		{
		    // rate
		    pld->send_rate_rpl();
		    break;
		}

		// cmd reply timeout, continue filtering
		// pld pointer already in cmap, no matching needed
		case CPT_CONTINUE_FILTERING:
		{
		    // continue with rules
		    // set pld pointer
		    pld->params.set_pointer(1, pld, 0, 0, RCT_PMINK);
		    // run rules
		    res = fm->run_rules(pld->params_p, &(*pld->rproc->rule_m), (*pld->rproc->rule_m).get_next_rule(), &last_rule);

		    // debug
		    #ifdef PMDEBUG
		    pld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
		    #endif
		    PMDLOG(
			std::cout << "===== CPT_CONTINUE_FILTERING =====" << std::endl;
			std::cout << pld->params;
			std::cout << "==================================" << std::endl;
		    )



		    // *** pausing, filtering not finished ***
		    // change phase
		    if(res == pmink_utils::RRT_CONTINUE_PAUSE){
			// dec refc (1 for timeot_loop, 1 for for method that paused filtering processor)
			pld->refc.sub_fetch(2);


		    // *** not pausing, filtering finished ***
		    }else{
			// *** send result ***
			// set filtering result
			pld->params.set_int(asn1::ParameterType::_pt_pmink_filter_result, res);
			// set last rule
			if(last_rule != NULL) pld->params.set_cstr(asn1::ParameterType::_pt_pmink_filter_exit, last_rule->get_label());
			// set tcap component index
			pld->params.set_int(asn1::ParameterType::_pt_tcap_component_index, pld->tcap_op_index);
			// if rejected, send error
			if(res != pmink_utils::RRT_STOP_OK){
			    pld->params.set_int(asn1::ParameterType::_pt_pmink_command_id, asn1::SmsHubCommandId::_shci_sms_ack);
			    pld->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 10);

			    // set error code if not set in rules
			    if(pld->params.get_param(asn1::ParameterType::_pt_tcap_error_code) == NULL)
				pld->params.set_int(asn1::ParameterType::_pt_tcap_error_code, asn1::gsmmap::GSMMAPLocalErrorcode::_unknownSubscriber);

			    if(pld->params.get_param(asn1::ParameterType::_pt_smpp_command_status) == NULL)
				pld->params.set_int(asn1::ParameterType::_pt_smpp_command_status, smpp::ESME_RX_P_APPN);

			    // gt calling -> gt called
			    pmink_utils::VariantParam* vp = pld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address);
			    if(vp != NULL) pld->params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, (char*)*vp, 2);
			    // gt called -> gt calling
			    vp = pld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
			    if(vp != NULL) pld->params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_address, (char*)*vp, 2);
			    // calling ssn -> called ssn
			    vp = pld->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);
			    if(vp != NULL) pld->params.set_int(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, (int)*vp, 2);
			    // gt called ssn -> gt calling ssn
			    vp = pld->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);
			    if(vp != NULL) pld->params.set_int(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, (int)*vp, 2);
			    // pmink guid
			    pld->params.set_octets(asn1::ParameterType::_pt_pmink_guid, &pld->guid, sizeof(pld->guid));
			}
			// debug
			#ifdef PMDEBUG
			pld->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
			#endif
			PMDLOG(
			    std::cout << "===== FILTERING FINISHED =====" << std::endl;
			    std::cout << pld->params;
			    std::cout << "==============================" << std::endl;
			)

			// send
			dd->send_stp(pld->params_p);
			// rate
			pld->send_rate_sms();

			// remove from guid map
			dd->cmap.lock();
			dd->cmap.remove(pld->guid);
			dd->cmap.unlock();
			// free rule processor
			dd->rproc_pool.deallocate_constructed(pld->rproc);
			// free pld
			dd->cpool.deallocate_constructed(pld);

		    }
		    // do not deallocate (already done if filtering finished)
		    continue;
		}

		// sri-for-sm request
		case CPT_SRI_SM_REQ:
		{
		    // rate
		    pld->send_rate_sri_req();
		    break;
		}

		// pd reply
		case CPT_PD_RPL:
		{
		    // try to correlate
		    pmink_utils::VariantParam* vp_pld_guid = pld->params.get_param(asn1::ParameterType::_pt_pmink_guid);
		    pmink_utils::VariantParam* vp_trans_guid_remote = pld->params.get_param(asn1::ParameterType::_pt_pmink_guid, 1);
		    if(vp_pld_guid != NULL && vp_trans_guid_remote != NULL){
			// check guid size
			if(vp_pld_guid->get_size() != 16 || vp_trans_guid_remote->get_size() != 16) break;
			// get guid as 128bit int
			memcpy(&tmp_guid, (unsigned char*)*vp_pld_guid, vp_pld_guid->get_size());
			// lock
			dd->cmap.lock();
			// find in guid map
			FgnPayload** matched = dd->cmap.get(tmp_guid);
			if(matched == NULL || (*matched)->phase != CPT_PD_REQ){
			    dd->cmap.unlock();
			    break;

			}
			// check transaction guid matched
			pmink_utils::VariantParam* vp_trans_guid_local = (*matched)->params.get_param(asn1::ParameterType::_pt_pmink_guid, fgn::PIT_INPUT_CMD_REQ_PART);
			if(vp_trans_guid_local == NULL) break;

			// match transaction guids
			if(memcmp((unsigned char*)*vp_trans_guid_local, (unsigned char*)*vp_trans_guid_remote, 16) != 0) break;

			// inc refc
			(*matched)->refc.fetch_add(1);
			// update ts
			dd->cmap.update_ts(tmp_guid);
			// unlock
			dd->cmap.unlock();
			// update phase
			(*matched)->phase = CPT_CONTINUE_FILTERING;
			// add pd rpl data

			// pd result
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_filter_result),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_pmink_filter_result, PIT_INPUT_CMD_RPL_PART));

			// continue with rules
			// set pld pointer
			(*matched)->params.set_pointer(1, *matched, 0, 0, RCT_PMINK);
			// run rules
			res = fm->run_rules((*matched)->params_p, &(*(*matched)->rproc->rule_m), (*(*matched)->rproc->rule_m).get_next_rule(), &last_rule);
			// debug
			#ifdef PMDEBUG
			(*matched)->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
			#endif
			PMDLOG(
			    std::cout << "===== CPT_PD_RPL =====" << std::endl;
			    std::cout << (*matched)->params;
			    std::cout << "======================" << std::endl;
			)

			// get trans guid as 128bit int
			memcpy(&tmp_guid, (unsigned char*)*vp_trans_guid_local, vp_trans_guid_local->get_size());
			// remove cmd r14p correlation
			dd->cmap_r14p.lock();
			dd->cmap_r14p.remove(tmp_guid);
			dd->cmap_r14p.unlock();

			// *** pausing, filtering not finished ***
			// change phase
			if(res == pmink_utils::RRT_CONTINUE_PAUSE){
			    // dec refc (1 extra for method that paused filtering processor)
			    (*matched)->refc.sub_fetch(2);


			// *** not pausing, filtering finished ***
			}else{
			    // *** send result ***
			    // set filtering result
			    (*matched)->params.set_int(asn1::ParameterType::_pt_pmink_filter_result, res);
			    // set last rule
			    if(last_rule != NULL) (*matched)->params.set_cstr(asn1::ParameterType::_pt_pmink_filter_exit, last_rule->get_label());
			    // set tcap component index
			    (*matched)->params.set_int(asn1::ParameterType::_pt_tcap_component_index, (*matched)->tcap_op_index);
			    // if rejected, send error
			    if(res != pmink_utils::RRT_STOP_OK){
				(*matched)->params.set_int(asn1::ParameterType::_pt_pmink_command_id, asn1::SmsHubCommandId::_shci_sms_ack);
				(*matched)->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 10);

				// set error code if not set in rules
				if((*matched)->params.get_param(asn1::ParameterType::_pt_tcap_error_code) == NULL)
				    (*matched)->params.set_int(asn1::ParameterType::_pt_tcap_error_code, asn1::gsmmap::GSMMAPLocalErrorcode::_unknownSubscriber);

				if((*matched)->params.get_param(asn1::ParameterType::_pt_smpp_command_status) == NULL)
				    (*matched)->params.set_int(asn1::ParameterType::_pt_smpp_command_status, smpp::ESME_RX_P_APPN);

				// gt calling -> gt called
				pmink_utils::VariantParam* vp = (*matched)->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address);
				if(vp != NULL) (*matched)->params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, (char*)*vp, 2);
				// gt called -> gt calling
				vp = (*matched)->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
				if(vp != NULL) (*matched)->params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_address, (char*)*vp, 2);
				// calling ssn -> called ssn
				vp = (*matched)->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);
				if(vp != NULL) (*matched)->params.set_int(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, (int)*vp, 2);
				// gt called ssn -> gt calling ssn
				vp = (*matched)->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);
				if(vp != NULL) (*matched)->params.set_int(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, (int)*vp, 2);
				// pmink guid
				(*matched)->params.set_octets(asn1::ParameterType::_pt_pmink_guid, &(*matched)->guid, sizeof((*matched)->guid));
			    }

			    // debug
			    #ifdef PMDEBUG
			    (*matched)->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
			    #endif
			    PMDLOG(
				std::cout << "===== FILTERING FINISHED =====" << std::endl;
				std::cout << (*matched)->params;
				std::cout << "==============================" << std::endl;
			    )

			    // send
			    dd->send_stp((*matched)->params_p);
			    // rate sms
			    (*matched)->send_rate_sms();

			    // remove from guid map
			    dd->cmap.lock();
			    dd->cmap.remove((*matched)->guid);
			    dd->cmap.unlock();
			    // free rule processor
			    dd->rproc_pool.deallocate_constructed((*matched)->rproc);
			    // free pld
			    dd->cpool.deallocate_constructed((*matched));
			}

		    }
		    break;
		}

		// sri-for-sm reply
		case CPT_SRI_SM_RPL:
		{
		    // rate sri
		    pld->send_rate_rpl();

		    // try to correlate
		    pmink_utils::VariantParam* vp_tcap_did = pld->params.get_param(asn1::ParameterType::_pt_tcap_destination_transaction_id);
		    if(vp_tcap_did != NULL){
			// hash
			uint32_t hash = pmink_utils::hash_fnv1a_64bit(&vp_tcap_did->get_data()->i64, vp_tcap_did->get_size());
			// lock
			dd->cmap_tcap.lock();
			// find in tcap map
			tmp_guid_p = dd->cmap_tcap.get(hash);
			if(tmp_guid_p != NULL) tmp_guid = *tmp_guid_p; else tmp_guid = 0;
			// unlock
			dd->cmap_tcap.unlock();
			// sanity check
			if(tmp_guid_p == NULL){
			    // send to stp
			    dd->send_stp(&pld->params);
			    break;
			}

			// lock
			dd->cmap.lock();
			// find in guid map
			FgnPayload** matched = dd->cmap.get(tmp_guid);
			if(matched == NULL || (*matched)->phase != CPT_SRI_SM_RPL){
			    //std::cout << "!!!GUID NOT MATCHED!!!" << std::endl;
			    dd->cmap.unlock();
			    // send to stp
			    dd->send_stp(&pld->params);
			    break;

			}

			// inc refc
			(*matched)->refc.fetch_add(1);
			// update ts
			dd->cmap.update_ts(tmp_guid);

			// unlock
			dd->cmap.unlock();
			// update phase
			(*matched)->phase = CPT_CONTINUE_FILTERING;

			// add sri-for-sm rpl data


			// cgpa ssn
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, PIT_INPUT_CMD_RPL_PART));
			// cgpa gt tt
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_translation_type, PIT_INPUT_CMD_RPL_PART));
			// cgpa gt np
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_numbering_plan, PIT_INPUT_CMD_RPL_PART));
			// cgpa gt nai
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_nature_of_address, PIT_INPUT_CMD_RPL_PART));
			// cgpa gt address
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_address),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, PIT_INPUT_CMD_RPL_PART));
			// cdpa ssn
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, PIT_INPUT_CMD_RPL_PART));
			// cdpa gt tt
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_translation_type, PIT_INPUT_CMD_RPL_PART));
			// cdpa gt np
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_numbering_plan, PIT_INPUT_CMD_RPL_PART));
			// cdpa gt nai
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_nature_of_address, PIT_INPUT_CMD_RPL_PART));
			// cdpa gt address
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_address),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_sccp_called_pa_gt_address, PIT_INPUT_CMD_RPL_PART));


			// nnn nai
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, pld->tcap_op_index),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_nnn_type_of_number, PIT_INPUT_CMD_RPL_PART));

			// nnn np
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, pld->tcap_op_index),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_nnn_numbering_plan, PIT_INPUT_CMD_RPL_PART));

			// nnn address
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_nnn_digits, pld->tcap_op_index),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_nnn_digits, PIT_INPUT_CMD_RPL_PART));

			// an nai
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_an_type_of_number, pld->tcap_op_index),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_an_type_of_number, PIT_INPUT_CMD_RPL_PART));

			// an np
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, pld->tcap_op_index),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_an_numbering_plan, PIT_INPUT_CMD_RPL_PART));

			// an address
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_an_digits, pld->tcap_op_index),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_an_digits, PIT_INPUT_CMD_RPL_PART));

			// imsi
			(*matched)->params.set(pld->params,
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_imsi, pld->tcap_op_index),
						pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_gsmmap_imsi, PIT_INPUT_CMD_RPL_PART));


			// continue with rules
			// set pld pointer
			(*matched)->params.set_pointer(1, *matched, 0, 0, RCT_PMINK);
			// run rules
			res = fm->run_rules((*matched)->params_p, &(*(*matched)->rproc->rule_m), (*(*matched)->rproc->rule_m).get_next_rule(), &last_rule);

			// debug
			#ifdef PMDEBUG
			(*matched)->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
			#endif
			PMDLOG(
			    std::cout << "===== CPT_SRI_SM_RPL =====" << std::endl;
			    std::cout << (*matched)->params;
			    std::cout << "==========================" << std::endl;
			)

			// remove cmd tcap correlation
			dd->cmap_tcap.lock();
			dd->cmap_tcap.remove(hash);
			dd->cmap_tcap.unlock();

			// *** pausing, filtering not finished ***
			// change phase
			if(res == pmink_utils::RRT_CONTINUE_PAUSE){
			    // dec refc (1 extra for method that paused filtering processor)
			    (*matched)->refc.sub_fetch(2);


			// *** not pausing, filtering finished ***
			}else{
			    // *** send result ***
			    // set filtering result
			    (*matched)->params.set_int(asn1::ParameterType::_pt_pmink_filter_result, res);
			    // set last rule
			    if(last_rule != NULL) (*matched)->params.set_cstr(asn1::ParameterType::_pt_pmink_filter_exit, last_rule->get_label());
			    // set tcap component index
			    (*matched)->params.set_int(asn1::ParameterType::_pt_tcap_component_index, (*matched)->tcap_op_index);
			    // if rejected, send error
			    if(res != pmink_utils::RRT_STOP_OK){
				(*matched)->params.set_int(asn1::ParameterType::_pt_pmink_command_id, asn1::SmsHubCommandId::_shci_sms_ack);
				(*matched)->params.set_int(asn1::ParameterType::_pt_tcap_error_type, 10);

				// set error code if not set in rules
				if((*matched)->params.get_param(asn1::ParameterType::_pt_tcap_error_code) == NULL)
				    (*matched)->params.set_int(asn1::ParameterType::_pt_tcap_error_code, asn1::gsmmap::GSMMAPLocalErrorcode::_unknownSubscriber);

				if((*matched)->params.get_param(asn1::ParameterType::_pt_smpp_command_status) == NULL)
				    (*matched)->params.set_int(asn1::ParameterType::_pt_smpp_command_status, smpp::ESME_RX_P_APPN);

				// gt calling -> gt called
				pmink_utils::VariantParam* vp = (*matched)->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_gt_address);
				if(vp != NULL) (*matched)->params.set_cstr(asn1::ParameterType::_pt_sccp_calling_pa_gt_address, (char*)*vp, 2);
				// gt called -> gt calling
				vp = (*matched)->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_gt_address);
				if(vp != NULL) (*matched)->params.set_cstr(asn1::ParameterType::_pt_sccp_called_pa_gt_address, (char*)*vp, 2);
				// calling ssn -> called ssn
				vp = (*matched)->params.get_param(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number);
				if(vp != NULL) (*matched)->params.set_int(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number, (int)*vp, 2);
				// gt called ssn -> gt calling ssn
				vp = (*matched)->params.get_param(asn1::ParameterType::_pt_sccp_calling_pa_subsystem_number);
				if(vp != NULL) (*matched)->params.set_int(asn1::ParameterType::_pt_sccp_called_pa_subsystem_number, (int)*vp, 2);
				// pmink guid
				(*matched)->params.set_octets(asn1::ParameterType::_pt_pmink_guid, &(*matched)->guid, sizeof((*matched)->guid));
			    }

			    // debug
			    #ifdef PMDEBUG
			    (*matched)->params.set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
			    #endif
			    PMDLOG(
				std::cout << "===== FILTERING FINISHED =====" << std::endl;
				std::cout << (*matched)->params;
				std::cout << "==============================" << std::endl;
			    )

			    // send
			    dd->send_stp((*matched)->params_p);
			    // rate sms
			    (*matched)->send_rate_sms();

			    // remove from guid map
			    dd->cmap.lock();
			    dd->cmap.remove((*matched)->guid);
			    dd->cmap.unlock();
			    // free rule processor
			    dd->rproc_pool.deallocate_constructed((*matched)->rproc);
			    // free pld
			    dd->cpool.deallocate_constructed((*matched));
			}



		    }else{
			// send to stp
			dd->send_stp(&pld->params);

		    }

		    break;

		}
		// notification
		case CPT_NOTIFICATION_RCVD:
		{
		    pmink_utils::VariantParam* vp_guid = pld->params.get_param(asn1::ParameterType::_pt_pmink_guid);
		    if(vp_guid != NULL){
			__uint128_t tmp_guid = 0;
			memcpy(&tmp_guid, (unsigned char*)*vp_guid, vp_guid->get_size());
			// lock
			dd->cmap.lock();
			// find
			FgnPayload** matched = dd->cmap.get(tmp_guid);
			// check if matched and in correct phase
			if(matched == NULL || (*matched)->phase != CPT_SRI_SM_REQ){
			    dd->cmap.unlock();
			    break;

			}

			// set tcap sid of outgoing sri-for-sm request
			(*matched)->params.set(pld->params,
					       pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_tcap_source_transaction_id),
					       pmink_utils::ParamTuple<>(asn1::ParameterType::_pt_tcap_source_transaction_id, PIT_INPUT_CMD_REQ_PART));

			// update phase
			(*matched)->phase = CPT_SRI_SM_RPL;
			// update ts
			dd->cmap.update_ts(tmp_guid);

			// unlock
			dd->cmap.unlock();

			// add to tcap list
			dd->cmap_tcap.lock();
			// get tcap sid
			pmink_utils::VariantParam* vp_tcap_sid = pld->params.get_param(asn1::ParameterType::_pt_tcap_source_transaction_id);
			// connect tcap sid to guid
			if(vp_tcap_sid != NULL){
			    uint32_t hash = pmink_utils::hash_fnv1a_64bit(&vp_tcap_sid->get_data()->i64, vp_tcap_sid->get_size());
			    dd->cmap_tcap.set(hash, tmp_guid);

			}

			dd->cmap_tcap.unlock();

		    }
		    break;
		}

		default: break;
	    }
	    // free rule processor
	    dd->rproc_pool.deallocate_constructed(pld->rproc);
	    // free pld
	    dd->cpool.deallocate_constructed(pld);

	// sleep
	}else{
	    nanosleep(&ts, NULL);

	}

    }
    return NULL;

}

void FgndDescriptor::send_stp(fgn::rule_param_t* params){
    // debug
    #ifdef PMDEBUG
    params->set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== SENDING TO STP =====" << std::endl;
	std::cout << *params;
	std::cout << "==========================" << std::endl;
    )
    // *********************************************
    // ************ push to stp daemon *************
    // *********************************************
    // get new router if connection broken
    if(stp_r14pc == NULL || !stp_r14pc->is_registered()) stp_r14pc = r14ps->get_registered_client("routingd");
    // get first routing daemon
    r14p::R14PClient* r14pc = stp_r14pc;
    // null check
    if(r14pc == NULL){
	stats.inc(fgn::ST_SMSG_SEND_ERROR, 1);
	return;
    }
    // allocate new service message
    r14p::ServiceMessage* msg = r14psmm->new_smsg();
    // msg sanity check
    if(msg == NULL){
	stats.inc(fgn::ST_SMSG_MSG_POOL_EMPTY, 1);
	return;
    }
    // service id
    msg->set_service_id(asn1::ServiceId::_sid_stp_routing);
    // copy params
    msg->vpmap = *params;
    // set source daemon type
    msg->vpmap.set_cstr(asn1::ParameterType::_pt_pmink_daemon_type, pmink::CURRENT_DAEMON->get_daemon_type());
    // set source daemon id
    msg->vpmap.set_cstr(asn1::ParameterType::_pt_pmink_daemon_id, pmink::CURRENT_DAEMON->get_daemon_id());

    // sync
    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(msg) != 0){
	// free
	r14psmm->free_smsg(msg);
	stats.inc(fgn::ST_SMSG_PARAM_POOL_EMPTY, 1);
	return;

    }

    // send service message
    int res = r14psmm->send(msg, r14pc, "stpd", NULL, true, &smsg_async_done);
    if(res != 0){
	// stats
	stats.inc(fgn::ST_SMSG_SEND_ERROR, 1);
	// free
	r14psmm->free_smsg(msg);

    }


    // *********************************************
    // *********************************************

}

void FgndDescriptor::send_pd(fgn::rule_param_t* params){
    // *********************************************
    // ************ push to pd daemon *************
    // *********************************************
    // get new router if connection broken
    if(pd_r14pc == NULL || !pd_r14pc->is_registered()) pd_r14pc = r14ps->get_registered_client("routingd");
    // get first routing daemon
    r14p::R14PClient* r14pc = pd_r14pc;
    // null check
    if(r14pc == NULL){
	stats.inc(fgn::ST_SMSG_SEND_ERROR, 1);
	return;
    }
    // allocate new service message
    r14p::ServiceMessage* msg = r14psmm->new_smsg();
    // msg sanity check
    if(msg == NULL){
	stats.inc(fgn::ST_SMSG_MSG_POOL_EMPTY, 1);
	return;
    }
    // service id
    msg->set_service_id(asn1::ServiceId::_sid_pdn_filtering);
    // copy params
    msg->vpmap = *params;
    // set source daemon type
    msg->vpmap.set_cstr(asn1::ParameterType::_pt_pmink_daemon_type, pmink::CURRENT_DAEMON->get_daemon_type());
    // set source daemon id
    msg->vpmap.set_cstr(asn1::ParameterType::_pt_pmink_daemon_id, pmink::CURRENT_DAEMON->get_daemon_id());

    // sync
    // sync vpmap
    if(r14psmm->vpmap_sparam_sync(msg) != 0){
	// free
	r14psmm->free_smsg(msg);
	stats.inc(fgn::ST_SMSG_PARAM_POOL_EMPTY, 1);
	return;

    }
    // debug
    #ifdef PMDEBUG
    params->set_label_p((std::map<uint32_t, std::string>*)pmink::CURRENT_DAEMON->get_param(9999));
    #endif
    PMDLOG(
	std::cout << "===== SENDING TO PDN =====" << std::endl;
	std::cout << msg->vpmap;
	std::cout << "==========================" << std::endl;
    )

    // send service message
    int res = r14psmm->send(msg, r14pc, "pdnd", NULL, true, &smsg_async_done);
    if(res != 0){
	// stats
	stats.inc(fgn::ST_SMSG_SEND_ERROR, 1);
	// free
	r14psmm->free_smsg(msg);

    }


    // *********************************************
    // *********************************************

}

const char* FgndDescriptor::get_plugin_name(void * hndl){
    if(!hndl) return NULL;
    typedef const char* (*dl_func_t)();
    dlerror();
    dl_func_t dl_func = (dl_func_t)dlsym(hndl, "pm_fgn_module_get_name");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        // log
        pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, dlsym_error);
        // close
        dlclose(hndl);
        return NULL;
    }
    // run method
    return dl_func();
}

int FgndDescriptor::init_plugin_vars(fgn::Plugin* plugin){
    if(!plugin || !plugin->handle) return 1;
    typedef void (*dl_func_t)(fgn::Plugin*);
    dlerror();
    dl_func_t dl_func = (dl_func_t)dlsym(plugin->handle, "pm_fgn_module_init_vars");
    const char *dlsym_error = dlerror();
    if(dlsym_error) return 2;
    // run method
    dl_func(plugin);
    // ok
    return 0;
}
void FgndDescriptor::init_plugins(const char* dir){
    dirent** fnames;
    std::string tmp_str;
    // scan dir
    if(!dir || strlen(dir) == 0) dir = ".";
    int n = scandir(dir, &fnames, NULL, &alphasort);
    // if no error
    if(n >= 0){
	for(int i = 0; i<n; i++){
	    // check if file is actually a file and not a dir
	    if(fnames[i]->d_type == DT_REG || fnames[i]->d_type == DT_LNK){
		const char *ext = strrchr(fnames[i]->d_name, '.');
		if((!ext) || (ext == fnames[i]->d_name)) continue;
		else if(strcmp(ext, ".so") != 0) continue;
		// init plugin
		fgn::Plugin plugin;
		plugin.fname.assign(dir);
		plugin.fname.append("/");
		plugin.fname.append(fnames[i]->d_name);
		plugin.handle = dlopen(plugin.fname.c_str(), RTLD_LAZY);
		if(!plugin.handle) {
		    // log
		    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, 
					       "Error while loading user plugin [%s]!", plugin.fname.c_str());
		    continue;
		}
                // get plugin name from library
		const char* plg_name = get_plugin_name(plugin.handle);
                // err check
                if(!plg_name) continue;
                // check if pplugin alrady exists
                fgn::plugin_m_t::iterator it = plugins.find(plg_name);
                if(it != plugins.end()){
		    // log
		    pmink::CURRENT_DAEMON->log(pmink::LLT_ERROR, 
					       "Plugin [file = '%s', name = '%s'] already"
                                               "registered, disabling both plugins due to name clash!",
                                               plugin.fname.c_str(),
                                               plg_name);

                    // erase from plugin list
                    plugins.erase(plg_name);
                    // close current library
                    dlclose(plugin.handle);
                    // next
                    continue;

                }
                // log
		pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, 
                                           "Loading plugin: name = [%s], filename = [%s]", 
                                           plg_name,
                                           plugin.fname.c_str());
                // add to list
                plugins[plg_name] = plugin;
                // init plugins vars (optional)
                init_plugin_vars(&plugins[plg_name]);
	    }

	    // free file
	    free(fnames[i]);

	}
	// free file list
	free(fnames);

    }

}


void ServiceMessageAsyncDone::run(r14p::R14PCallbackArgs* args){
    // get service message
    r14p::ServiceMessage* smsg = (r14p::ServiceMessage*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARGS_SRVC_MSG);
    // return service message to pool
    smsg->get_smsg_manager()->free_smsg(smsg);


}

