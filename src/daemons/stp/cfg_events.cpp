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
#include <stp.h>

// STPListsConfigMod
void STPListsConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* lists_root = cfg->children[0];
    // get lists root
    lists_root = lists_root->find_parent("lists");
    // sanity check
    if(lists_root == NULL) return;
    // tmp vars
    config::ConfigItem* list_node = NULL;
    StpdDescriptor* stpd = (StpdDescriptor*)pmink::CURRENT_DAEMON;
    stp::StpRouting* stpr = &stpd->stp_routing;
    bool rebuild_lists = false;
    // lock
    stpd->list_m.update_start();
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
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Building routing lists...");
	stpr->build_lists(lists_root, &(*stpd->list_m));
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Finished building routing lists...");
    }

    // unlock
    stpd->list_m.update_end();

}

// STPRoutingConfigMod
void STPRoutingConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    config::ConfigItem* routing_root = cfg->children[0];
    // get routing root
    routing_root = routing_root->find_parent("routing");
    // sanity check
    if(routing_root == NULL) return;
    // tmp vars
    config::ConfigItem* rule_node = NULL;
    StpdDescriptor* stpd = (StpdDescriptor*)pmink::CURRENT_DAEMON;
    stp::StpRouting* stpr = &stpd->stp_routing;
    bool rebuild_rules = false;
    // lock
    stpd->rule_m.update_start();
    // loop rules
    for(unsigned int i = 0; i<routing_root->children.size(); i++){
	rule_node = routing_root->children[i];
	// rule deleted, continue
	if(rule_node->node_state == config::CONFIG_NS_DELETED){
	    pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Removing routing rule [%s]...",
							 rule_node->name.c_str());


	    // rebuild
	    rebuild_rules = true;

	}else if(rule_node->is_new){
	    // rebuild
	    rebuild_rules = true;

	}else{
	    // check for changes
	    if(rule_node->is_modified()){
		// rebuild
		rebuild_rules = true;

	    }
	}


    }

    // rebuild rules
    if(rebuild_rules){
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Building routing rules...");
	stpr->build_rules(routing_root, &(*stpd->rule_m));
	pmink::CURRENT_DAEMON->log(pmink::LLT_INFO, "Finished building routing rules...");
    }

    // unlock
    stpd->rule_m.update_end();


}
