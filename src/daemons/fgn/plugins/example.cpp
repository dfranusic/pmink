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

// example plugin header
#include "example.h"

// define module init
PM_FGN_MODULE_INIT_DEFINE(
    // module name
    PM_FGN_MODULE,
    // * add methods "example_test" and "example_test_2"
    //   to module "example"
    PM_FGN_MODULE_ADD_METHOD(example_test)
    PM_FGN_MODULE_ADD_METHOD(example_test_2)
    PM_FGN_MODULE_ADD_METHOD(example_test_3)
)

// RRP test event
class RRPTestEvent: public rrp::rrp_event_t{
    public:
        void run(rrp::rrp_event_args_t& args){
            // RRP sequenuce
            rrp::RRSequence* rrs = (rrp::RRSequence*)*args.get_arg(rrp::RREAIT_SEQUENCE);

            // get refs for PDU and SEQUENCE params
            vp_params_t& pdu = rrs->pdu.params;
            vp_params_t& seq = rrs->params;

            // *** get SEQUENCE params ***
            // guid
            __uint128_t* guid = seq.get_octets<__uint128_t*>(0);
            // lua state
            LS* L = seq.get_pointer<LS*>(1);

            // *** get PDU params ***
            //  get GT called address
            char* GT = pdu.get_cstr(rrp::RRPT_GT_CALLED);

            //sleep(20);
            // * resume when done and add "[GT] -> "123456"
            //   to a lua result table if current payload hasn't
            //   already timed out
            PM_FGN_MODULE_RESUME(
                // payload guid
                *guid,
                // if payload is still buffered, do the following
                if(GT) PM_FGN_TBL_ADD_STR("GT", GT)
            );

            // free event
            delete this;
        }
};

// send dummy data via RRP
int test_rrp_method(__uint128_t guid, LS* L){
    // get RRP sequence for RRP routing connection
    rrp::RRSequence* rrs = get_rrp_routing_seq();
    if(!rrs) return 1;
    // get refs for PDU and SEQUENCE params
    vp_params_t& pdu = rrs->pdu.params;
    vp_params_t& seq = rrs->params;
    // *** SEQUENCE params ***
    seq.set_octets(0, &guid, sizeof(guid));
    seq.set_pointer(1, L);
    // *** PDU params ***
    // test GT Called number
    pdu.set_cstr(rrp::RRPT_GT_CALLED, "123456");
    // create event
    RRPTestEvent* rrp_event = new RRPTestEvent();
    // set event handlers
    rrs->e_handler.set_handler(rrp::RREIT_SEQUENCE_END, rrp_event);
    rrs->e_handler.set_handler(rrp::RREIT_SEQUENCE_TIMEOUT, rrp_event);
     // send
    rrs->send();
    // ok
    return 0;
}

// define "example.example_test" method
PM_FGN_MODULE_METHOD_DEFINE(example_test){
    // get fgn payload (pointer to fgn::FgnPayload)
    PM_FGN_PLD_GET(pld);
    // return and yield
    PM_FGN_MODULE_RET_YIELD(
        // payload
        pld,
        // action just BEFORE yield
        test_rrp_method(pld->guid, L)
    );
}


// define "example.example_test_2" method
PM_FGN_MODULE_METHOD_DEFINE(example_test_2){
    // get fgn payload (pointer to fgn::FgnPayload)
    PM_FGN_PLD_GET(pld);
    // if GT address == 123456, return true
    char* GT = pld->params.get_cstr(PT::_pt_sccp_called_pa_gt_address);
    if(GT && strcmp(GT, "123456") == 0) PM_FGN_MODULE_RET_BOOL(true);
    // default return (false)
    PM_FGN_MODULE_RET_BOOL(false);
}

// define "example.example_test_3" method
PM_FGN_MODULE_METHOD_DEFINE(example_test_3){
    // get fgn payload (pointer to fgn::FgnPayload)
    PM_FGN_PLD_GET(pld);

    // * return table with two entries
    //   1. key_1 => value
    //   2. key_2 => 9999
    PM_FGN_MODULE_RET_TBL(
        PM_FGN_TBL_ADD_STR("key_1", "value")
        PM_FGN_TBL_ADD_INT("key_2", 9999)
    )
}
