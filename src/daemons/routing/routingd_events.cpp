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

#include <routingd_events.h>
#include <routing.h>



HbeatMissed::HbeatMissed(pmink::Atomic<bool>* _activity_flag){
    activity_flag = _activity_flag;

}

void HbeatMissed::run(r14p::R14PCallbackArgs* args){
	r14p::HeartbeatInfo* hi = (r14p::HeartbeatInfo*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_HBEAT_INFO);
	// set activity flag to false
	activity_flag->comp_swap(true, false);
	// stop heartbeat
	r14p::stop_heartbeat(hi);
	// display warning
	pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "R14P HBEAT not received, closing connection to [%s]...",
				   hi->target_daemon_id);

}

void HbeatRecv::run(r14p::R14PCallbackArgs* args){
    // do nothing
}

HbeatCleanup::HbeatCleanup(HbeatRecv* _recv, HbeatMissed* _missed){
	recv = _recv;
	missed = _missed;

}

void HbeatCleanup::run(r14p::R14PCallbackArgs* args){
	delete recv;
	delete missed;
	delete this;

	// get routingd pointer
	RoutingdDescriptor* routingd = (RoutingdDescriptor*)pmink::CURRENT_DAEMON;
	// init config until connected
	while(!pmink::DaemonDescriptor::DAEMON_TERMINATED && routingd->init_config(false) != 0){
	    sleep(5);
	}

}


