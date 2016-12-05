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
#ifndef DRD_EVENTS_H_
#define DRD_EVENTS_H_

#include <atomic.h>
#include <r14p_utils.h>
#include <pmdb.h>

class DrdDescriptor;

namespace dr{
    // fwd declarations
    class SrvcMsgParamStreamNext;
    class SrvcMsgParamStreamDone;
    class SrvcMsgNewParamStream;
    class SrvcMsgDone;
    class NewSrvcMsg;
    class SrvcMsgErr;

    // Next fragmented param fragment
    class SrvcMsgParamStreamNext: public r14p::R14PCallbackMethod {
    public:
	void run(r14p::R14PCallbackArgs* args);
	// new param stream handler
	SrvcMsgNewParamStream* new_pstream;

    };


    // Fragmented param stream finished
    class SrvcMsgParamStreamDone: public r14p::R14PCallbackMethod {
    public:
	void run(r14p::R14PCallbackArgs* args);
	// new param stream hander
	SrvcMsgNewParamStream* new_pstream;

    };



    // New Fragmented param stream
    class SrvcMsgNewParamStream: public r14p::R14PCallbackMethod {
    public:
	SrvcMsgNewParamStream();
	void run(r14p::R14PCallbackArgs* args);
	// param stream next handler
	SrvcMsgParamStreamNext param_next;
	// parma stream done handler
	SrvcMsgParamStreamDone param_done;
	// new service message handler
	NewSrvcMsg* new_msg;

    };

    // New param
    class SrvcMsgNewParam: public r14p::R14PCallbackMethod {
    public:
	void run(r14p::R14PCallbackArgs* args);
	// new service message handler
	NewSrvcMsg* new_msg;

    };


    // Service message finished
    class SrvcMsgDone: public r14p::R14PCallbackMethod {
    public:
	/*
	SrvcMsgDone(){
	    count = 0;
	}

	int count;
	*/
	void run(r14p::R14PCallbackArgs* args);
	// new service message handler
	NewSrvcMsg* new_msg;
	// fill dbr with data from TCAP correlation
	//void fill_dbr_sms(db::DBRecord* dbr, r14p::ServiceMessage* smsg_req, r14p::ServiceMessage* smsg_rpl, uint32_t req_comp_index, uint32_t rpl_comp_index);
	void fill_dbr_sms(db::DBRecord* dbr, pmink_utils::TCAPOperation* tcap_op);



    };

    // Service message error
    class SrvcMsgErr: public r14p::R14PCallbackMethod {
    public:
	void run(r14p::R14PCallbackArgs* args);
	// new service message handler
	NewSrvcMsg* new_msg;
    };



    // New service message
    class NewSrvcMsg: public r14p::R14PCallbackMethod {
    private:
	// new param handler
	SrvcMsgNewParam new_param;
	// new param stream handler
	SrvcMsgNewParamStream new_pstream;
	// service message done handler
	SrvcMsgDone msg_done;

    public:
	NewSrvcMsg();
	~NewSrvcMsg();
	void run(r14p::R14PCallbackArgs* args);

	// db manager
	db::DBManager* dbm;
	// tcap correlator
	pmink_utils::TCAPCorrelator* tcap_corr;
	// tcap correlator timeout timestamp
	pmink::Atomic<time_t> timeout_ts;
	// tcap correlation timeout
	int tcap_timeout;
	// deamon pointer
	DrdDescriptor* drd;
	// service message err handler
	SrvcMsgErr msg_err;


	// friend classes
	friend class SrvcMsgNewParam;
	friend class SrvcMsgParamStreamDone;
	friend class SrvcMsgNewParamStream;
	friend class SrvcMsgParamStreamNext;
	friend class SrvcMsgErr;

    };

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



}






#endif /* DRD_EVENTS_H_ */
