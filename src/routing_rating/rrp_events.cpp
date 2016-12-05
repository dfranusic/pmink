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

#include <rrp_events.h>

// RegisterEvent
rrp::RegisterEvent::RegisterEvent(RRConnection* _rr_conn): rr_conn(_rr_conn), status(-1){}

void rrp::RegisterEvent::run(pmink_utils::EventArgs<RREventArgIdType, void*>& args){
    RRSequence* rrs = (RRSequence*)*args.get_arg(RREAIT_SEQUENCE);

    // check status
    pmink_utils::VariantParam* vp = rrs->pdu.params.get_param(RRPT_STATUS);
    if(vp != NULL && RRStatus::get_status((char*)*vp) == RREC_200) rr_conn->registered.set(true);

    // get source ctx
    vp = rrs->pdu.params.get_param(RRPT_SRC_CTX);
    if(vp != NULL) strcpy(rrs->destination_id, (char*)*vp);

    // check if socket needs to be changed
    vp = rrs->pdu.params.get_param(RRPT_FROM_PORT);
    if(vp == NULL) return;
    if(rrs->connection->end_point_port != (int)*rrs->pdu.params.get_param(RRPT_FROM_PORT)){
	rrs->connection->end_point_port = (int)*rrs->pdu.params.get_param(RRPT_FROM_PORT);
	rrs->connection->end_port_changed_flag.set(true);

    }
    // event handler finished
    finished.set(true);
}



