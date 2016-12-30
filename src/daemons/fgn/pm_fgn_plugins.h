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

#include <fgn.h>

#ifndef PM_FGN_PLUGINS_H
#define PM_FGN_PLUGINS_H

// typedefs
typedef pmink_utils::VariantParam VP;
typedef asn1::ParameterType PT;
typedef pmink_utils::EventArgs<rrp::RREventArgIdType, void*> rrp_event_args_t;
typedef pmink_utils::EventHandler<rrp::RREventArgIdType, void*> rrp_event_t;
typedef lua_State LS;
typedef pmink_utils::PooledVPMap<uint32_t> vp_params_t;

// utils
#define PM_FGN_CAT(x, y) x ## y
#define PM_FGN_STR(x) #x

// get rrp routing connection and sequence
rrp::RRSequence* get_rrp_routing_seq(){
    // get RRP connection
    FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;
    dd->routing_map.wrr_mtx.lock();
    pmink_utils::WRRItem<rrp::RRConnection*>* wrr_rrc = dd->routing_map.get_next_node();
    dd->routing_map.wrr_mtx.unlock();
    // error check
    if(!(wrr_rrc && wrr_rrc->item->registered.get())) return NULL;
    // rr conn pointer
    rrp::RRConnection* rrc = wrr_rrc->item;
    // new sequence
    rrp::RRSequence* rrs = rrc->new_sequence(wrr_rrc->id, rrp::RRRT_FILTER_RATE);
    if(rrs == NULL) return NULL;
    return rrs;
}


// lua module init methods declarations
#define PM_FGN_MODULE_INIT_DECLARE(name)\
extern "C" int pm_fgn_module_init(lua_State* L);\
extern "C" PM_FGN_CAT(int luaopen_, name)(lua_State* L);

// start of lua module init methods definition
#define PM_FGN_MODULE_INIT_DEFINE(name, methods)\
int PM_FGN_CAT(luaopen_, name)(lua_State* L){\
    std::cout << "!!!LUAOPEN!!!!!: " << L << std::endl;\
    lua_pushcfunction(L, &pm_fgn_module_init);\
    return 1;\
}\
int pm_fgn_module_init(lua_State* L){\
    int argc = lua_gettop(L);\
    if(!(argc >= 1 && lua_islightuserdata(L, 1))) return 0;\
    void* p = (void*)lua_topointer(L, 1);\
    std::cout << "pm_fgn_module_init....." << argc << ", " << lua_type(L, 1) << ", " << L << std::endl;\
    lua_newtable(L);\
    methods;\
    return 1;\
}

// add user method to lua module
#define PM_FGN_MODULE_ADD_METHOD(method)\
    lua_pushlightuserdata(L, p);\
    lua_pushcclosure(L, &method, 1);\
    lua_setfield(L, -2, PM_FGN_STR(method));

// lua user method declaration and definition
#define PM_FGN_MODULE_METHOD_DECLARE(name) extern "C" int name(lua_State* L);
#define PM_FGN_MODULE_METHOD_DEFINE(name) int name(lua_State* L)


// return bool result
#define PM_FGN_MODULE_RET_BOOL(val)\
{lua_pushboolean(L, val);\
return 1;}

// return int result
#define PM_FGN_MODULE_RET_INT(val)\
{lua_pushinteger(L, val);\
return 1;}

// return string result
#define PM_FGN_MODULE_RET_STR(val)\
{lua_pushstring(L, val);\
return 1;}

// return table result
#define PM_FGN_MODULE_RET_TBL(val)\
{lua_newtable(L);\
val;\
return 1;}

// add str to table
#define PM_FGN_TBL_ADD_STR(key, val)\
{lua_pushstring(L, val);\
lua_setfield(L, -2, key);}

// add int to table
#define PM_FGN_TBL_ADD_INT(key, val)\
{lua_pushinteger(L, val);\
lua_setfield(L, -2, key);}

// add bool to table
#define PM_FGN_TBL_ADD_BOOL(key, val)\
{lua_pushboolean(L, val);\
lua_setfield(L, -2, key);}

// return table and pause(yield)
#define PM_FGN_MODULE_RET_YIELD(pld, usr_code)\
{lua_newtable(L);\
FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;\
dd->cmap.lock();\
pld->refc.fetch_add(1);\
pld->phase = 10000;\
dd->cmap.set(pld->guid, pld);\
dd->cmap.unlock();\
usr_code;\
return lua_yield(L, 1);}

// get FgnPayload as an upvalue
#define PM_FGN_PLD_GET(name) \
fgn::FgnPayload* name = (fgn::FgnPayload*)lua_topointer(L, lua_upvalueindex(1));

// get VariantParam from pld params
#define PM_FGN_VP_GET(pld, param, index) pld->params.get_param(param, index);

// module resume
#define PM_FGN_MODULE_RESUME(guid, code)\
{FgndDescriptor* dd = (FgndDescriptor*)pmink::CURRENT_DAEMON;\
fgn::FilterManager* fm = &dd->filter_m;\
fgn::FWorker* fw = fm->get_fw();\
if(fw){\
    dd->cmap.lock();\
    fgn::FgnPayload** pld = dd->cmap.get(guid);\
    if(pld != NULL && (*pld)->refc.get() == 0){\
        (*pld)->refc.fetch_add(1);\
        (*pld)->phase = fgn::CPT_CONTINUE_FILTERING;\
        dd->cmap.update_ts(guid);\
        dd->cmap.unlock();\
        code;\
        fw->queue.push(*pld);\
    }else dd->cmap.unlock();\
}}

#endif //PM_FGN_PLUGINS_H
