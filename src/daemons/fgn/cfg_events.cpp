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

#include "cfg_events.h"
#include <string>
#include <vector>
#include <r14p_asn1.h>
#include <daemon.h>
#include <pmink_utils.h>
#include <fgn.h>

// ListsConfigMod
void fgn::ListsConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* lists_root = cfg->children[0];
    // get lists root
    lists_root = lists_root->find_parent("lists");
    // sanity check
    if(lists_root == NULL) return;
    // tmp vars
    config::ConfigItem* list_node = NULL;
    FgndDescriptor* fgnd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    fgn::FilterManager* fm = &fgnd->filter_m;
    bool rebuild_lists = false;
    // lock
    fgnd->list_m.update_start();
    // loop lists
    for(unsigned int i = 0; i<lists_root->children.size(); i++){
	// list
	list_node = lists_root->children[i];
	// check for changes
	if(list_node->is_modified()){
	    // rebuild
	    rebuild_lists = true;
	    break;
	}
    }

    // rebuild lists
    if(rebuild_lists){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Building filtering lists...");
	fm->build_lists(lists_root, &(*fgnd->list_m));
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Finished building filtering lists...");
    }

    // unlock
    fgnd->list_m.update_end();

}

// FilterConfigMod
void fgn::FilterConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* rules_root = cfg->children[0];
    // get rules root
    rules_root = rules_root->find_parent("rules");
    // sanity check
    if(rules_root == NULL) return;
    // tmp vars
    config::ConfigItem* rule_node = NULL;
    FgndDescriptor* fgnd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    fgn::FilterManager* fm = &fgnd->filter_m;
    bool rebuild_rules = false;
    // lock
    fgnd->rule_m.update_start();
    //stpd->stp_routing.mtx.lock();
    // loop rules
    for(unsigned int i = 0; i<rules_root->children.size(); i++){
	rule_node = rules_root->children[i];
	// rule deleted, continue
	if(rule_node->node_state == config::CONFIG_NS_DELETED){
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Removing filtering rule [%s]...",
							 rule_node->name.c_str());


	    // rebuild
	    rebuild_rules = true;

	}else if(rule_node->is_new){
	    // rebuild
	    rebuild_rules = true;

	}else{
	    // check for changes
	    if(rule_node->is_modified()) rebuild_rules = true;
	}


    }

    // rebuild rules
    if(rebuild_rules){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Building filtering rules...");
	fm->build_rules(rules_root, &(*fgnd->rule_m));
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Finished building filtering rules...");
    }

    // unlock
    fgnd->rule_m.update_end();


}
