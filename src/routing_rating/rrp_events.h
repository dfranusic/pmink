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

#ifndef RRP_EVENTS_H_
#define RRP_EVENTS_H_

#include <pmink_utils.h>
#include <rrp.h>



namespace rrp{
    class RegisterEvent: public pmink_utils::EventHandler<RREventArgIdType, void*>{
    public:
	RegisterEvent(RRConnection* _rr_conn);
	void run(pmink_utils::EventArgs<RREventArgIdType, void*>& args);

	RRConnection* rr_conn;
	pmink::Atomic<bool> finished;
	int status;
    };


}



#endif /* RRP_EVENTS_H_ */
