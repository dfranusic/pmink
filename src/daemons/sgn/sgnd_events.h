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

#ifndef SGND_EVENTS_H_
#define SGND_EVENTS_H_

#include <atomic.h>
#include <r14p.h>
#include <config.h>
#include <r14p_utils.h>
#include <connection.h>

// fwd declarations
class NewSrvcMsg;
class SrvcMsgDone;

class HbeatMissed: public r14p::R14PCallbackMethod {
public:
    HbeatMissed(pmink::Atomic<bool>* _activity_flag);
    void run(r14p::R14PCallbackArgs* args);

    pmink::Atomic<bool>* activity_flag;
};


class HbeatRecv: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args);
};


class HbeatCleanup: public r14p::R14PCallbackMethod {
public:
    HbeatCleanup(HbeatRecv* _recv, HbeatMissed* _missed);
    void run(r14p::R14PCallbackArgs* args);

    HbeatMissed* missed;
    HbeatRecv* recv;
};


class ServiceMessageAsyncDoneExt: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args);
    SrvcMsgDone* msg_done;
};


// Service message finished
class SrvcMsgDone: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args);
    int get_next_route(r14p::ServiceMessage* smsg,
		       char* next_route,
		       uint32_t buff_size,
		       char* next_sub_route,
		       uint32_t sub_buff_size,
		       int* next_sub_type,
		       bool inc_as_index);

    int process_next(r14p::ServiceMessage* smsg, r14p::R14PStream* r14p_stream, char* next_route, uint32_t buff_size);

    NewSrvcMsg* new_msg;
    ServiceMessageAsyncDoneExt async_done;
};

// Service message error
class SrvcMsgErr: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args);
};

// New service message
class NewSrvcMsg: public r14p::R14PCallbackMethod {
public:
    NewSrvcMsg();
    ~NewSrvcMsg();

    void run(r14p::R14PCallbackArgs* args);

    // service message err handler
    SrvcMsgErr msg_err;
    SrvcMsgDone msg_done;


};

// M3UA WRR remove event
class AS_WRR_Remove: public pmink_utils::EventHandler<sgn::ConnectiontEventArgType, void*>{
public:
    void run(pmink_utils::EventArgs<sgn::ConnectiontEventArgType, void*>& args);

};

// M3UA WRR add event
class AS_WRR_Add: public pmink_utils::EventHandler<sgn::ConnectiontEventArgType, void*>{
public:
    void run(pmink_utils::EventArgs<sgn::ConnectiontEventArgType, void*>& args);

};

// SMPP WRR add event
class SMPP_WRR_Add: public pmink_utils::EventHandler<sgn::ConnectiontEventArgType, void*>{
public:
    void run(pmink_utils::EventArgs<sgn::ConnectiontEventArgType, void*>& args);

};

// SMPP WRR remove event
class SMPP_WRR_Remove: public pmink_utils::EventHandler<sgn::ConnectiontEventArgType, void*>{
public:
    void run(pmink_utils::EventArgs<sgn::ConnectiontEventArgType, void*>& args);

};





#endif /* SGND_EVENTS_H_ */
