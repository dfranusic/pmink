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

#ifndef PDND_EVENTS_H_
#define PDND_EVENTS_H_

#include <atomic.h>
#include <r14p_utils.h>


// fwd declarations
class NewSrvcMsg;

class ServiceMessageAsyncDone: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args);
};

class HbeatMissed: public r14p::R14PCallbackMethod {
public:
    HbeatMissed(pmink::Atomic<uint8_t>* _activity_flag);
    void run(r14p::R14PCallbackArgs* args);

    pmink::Atomic<uint8_t>* activity_flag;
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

// Service message finished
class SrvcMsgDone: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args);

    NewSrvcMsg* new_msg;
    ServiceMessageAsyncDone async_done;
};

// Service message error
class SrvcMsgErr: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args);
};

// New service message
class NewSrvcMsg: public r14p::R14PCallbackMethod {
private:
    SrvcMsgDone msg_done;

public:

    void run(r14p::R14PCallbackArgs* args);

    // service message err handler
    SrvcMsgErr msg_err;


};




#endif /* PDND_EVENTS_H_ */
