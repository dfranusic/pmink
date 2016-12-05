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
#include <routing.h>


WRRConfigMod::WRRConfigMod(): r14ps(NULL){}

void WRRConfigMod::run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count){
    // sanity check
    if(r14ps->get_routing_handler() == NULL) return;
    // rebuild WRR rules
    config::ConfigItem* dests_root = cfg->children[0];
    // get destinations root
    dests_root = dests_root->find_parent("destinations");
    // sanity check
    if(dests_root == NULL) return;
    // tmp vars
    config::ConfigItem* dest_node_type = NULL;
    config::ConfigItem* nodes = NULL;
    config::ConfigItem* dest_node = NULL;
    config::ConfigItem* tmp_node = NULL;
    pmink_utils::PooledVPMap<uint32_t> tmp_params;
    r14p::R14PClient* r14pc = NULL;
    pmink_utils::WRRItem<r14p::R14PClient*>* wrr_item = NULL;
    // lock
    r14ps->lock_clients();
    // clear current routing config
    //r14ps->get_routing_handler()->clear();
    // loop destinations
    for(unsigned int i = 0; i<dests_root->children.size(); i++){
	dest_node_type = dests_root->children[i];
	// dest type deleted, continue
	if(dest_node_type->node_state == config::CONFIG_NS_DELETED){
	    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Removing [%s] routing table...",
							  dest_node_type->name.c_str());

	    r14ps->get_routing_handler()->remove_type(dest_node_type->name.c_str());
	    continue;
	}
	// log
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Processing configuration for destination type [%s] for node [%s]...",
						      dest_node_type->name.c_str(), pmink::CURRENT_DAEMON->get_daemon_id());

	// check for nodes
	if((*dest_node_type)("nodes") == NULL) {
	    pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Missing destination [%s] nodes configuration node set for node [%s]!",
							    dest_node_type->name.c_str(), pmink::CURRENT_DAEMON->get_daemon_id());
	    continue;
	}
	// get nodes
	nodes = (*dest_node_type)("nodes");

	// process nodes
	for(unsigned int j = 0; j<nodes->children.size(); j++){
	    dest_node = nodes->children[j];
	    // dest node deleted, continue
	    if(dest_node->node_state == config::CONFIG_NS_DELETED) {
		// log
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Removing node [%s] from [%s] routing table...",
							      dest_node->name.c_str(),
							      dest_node_type->name.c_str());

		r14ps->get_routing_handler()->remove_node(dest_node_type->name.c_str(), dest_node->name.c_str());
		continue;

	    // new node
	    }else if(dest_node->is_new){
		// log
		pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Adding node [%s] to [%s] routing table with weight [%d]...",
							      dest_node->name.c_str(),
							      dest_node_type->name.c_str(),
							      dest_node->to_int("weight"));

		// set weight data
		tmp_params.set_int(0, dest_node->to_int("weight"));
		// check for r14p client connection
		r14pc = r14ps->get_registered_client(dest_node_type->name.c_str(), dest_node->name.c_str(), true);
		// add to routing handler
		r14ps->get_routing_handler()->add_node(r14pc, dest_node_type->name.c_str(), dest_node->name.c_str(), &tmp_params);

	    // modified node
	    }else{
		tmp_node = (*dest_node)("weight");
		// check if modified
		if(tmp_node != NULL){
		    if(tmp_node->node_state == config::CONFIG_NS_MODIFIED){
			// log
			pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Setting weight to [%d] for node [%s] in [%s] routing table...",
								      tmp_node->to_int(),
								      dest_node->name.c_str(),
								      dest_node_type->name.c_str());

			// get item
			wrr_item = (pmink_utils::WRRItem<r14p::R14PClient*>*)r14ps->get_routing_handler()->get_node(dest_node_type->name.c_str(), dest_node->name.c_str());
			// set weight
			if(wrr_item != NULL && wrr_item->item != NULL){
			    wrr_item->weight = tmp_node->to_int();
			    // recalc (done in update client)
			    r14ps->get_routing_handler()->update_client(wrr_item->item, wrr_item->item->get_end_point_daemon_type(), wrr_item->item->get_end_point_daemon_id());
			}


		    }else if(tmp_node->node_state == config::CONFIG_NS_DELETED){
			// log
			pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Disabling node [%s] in [%s] routing table, setting weight to [0]",
								      dest_node->name.c_str(),
								      dest_node_type->name.c_str());

			// get item
			wrr_item = (pmink_utils::WRRItem<r14p::R14PClient*>*)r14ps->get_routing_handler()->get_node(dest_node_type->name.c_str(), dest_node->name.c_str());
			// set weight
			if(wrr_item != NULL && wrr_item->item != NULL){
			    wrr_item->weight = 0;
			    // recalc (done in update client)
			    r14ps->get_routing_handler()->update_client(wrr_item->item, wrr_item->item->get_end_point_daemon_type(), wrr_item->item->get_end_point_daemon_id());
			}

		    }

		}
	    }
	}
    }
    // unlock
    r14ps->unlock_clients();
}
