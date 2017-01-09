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

#ifndef PMINK_LUA_IF_H_
#define PMINK_LUA_IF_H_

// lua module
#define PMINK_LUA_MODULE	"local F = require('pmink')(...);"
#define PMINK_LUA_DUMMY_STR	"lua"

// return type for regex match
typedef struct {
    char** groups;
    int size;
    bool matched;
} rgx_result_t;

extern "C" {
    // *** ascii regex ***
    int pmink_lua_regex_count(void* pm, const char* data, const char* regex);
    rgx_result_t pmink_lua_regex_match(void* pm, const char* data, const char* regex);

    // *** utf8 ***
    int pmink_lua_utf8_upper(void* pm, const char* data, char* out);
    int pmink_lua_utf8_lower(void* pm, const char* data, char* out);
    bool pmink_lua_utf8_regex_match(void* pm, const char* data, const char* regex);
    int pmink_lua_utf8_regex_count(void* pm, const char* data, const char* regex);

    // *** hlr request ***
    int pmink_lua_hlr_req(void* pm);
    void* pmink_lua_hlr_req_vp_new(void* pm, int var);

    // *** pd request ***
    int pmink_lua_pd_req(void* pm, int action);
    void* pmink_lua_pd_req_vp_new(void* pm, int var);


    // *** flood lists ***
    // delete flood list or item in flood list
    bool pmink_lua_fl_del(void* pm, const char* list, const char* item);
    // new flood list
    bool pmink_lua_fl_new(void* pm, const char* label);
    // get item count in flood list for specified interval
    int pmink_lua_fl_get(void* pm, const char* list, int interval, const char* item);
    // add item to flood list
    bool pmink_lua_fl_add(void* pm, const char* list, const char* item);

    // ** standard lists ***
    // new standard list
    bool pmink_lua_sl_new(void* pm, const char* label);
    // delete standard list or item in list
    bool pmink_lua_sl_del(void* pm, const char* list, const char* item);
    // add new item to standard list
    bool pmink_lua_sl_add(void* pm, const char* list, const char* item);
    // checkk if item exists in standard list
    bool pmink_lua_sl_get(void* pm, const char* list, const char* item);
    // get list size
    int pmink_lua_sl_get_size(void* pm, const char* list);
    // get list item
    const char* pmink_lua_sl_get_item(void* pm, const char* list, int index);

    // *** VariantParam ***
    // new VariantParam
    void* pmink_lua_vp_new(void* pm, int var, int index);
    // get VariantParam pointer
    void* pmink_lua_vp_get(void* pm, int var, int index);
    // VariantParam to c string
    char* pmink_lua_vp_to_str(void* pm, void* vp);
    // VariantParam to bool
    bool pmink_lua_vp_to_bool(void* pm, void* vp);
    // VariantParam to c int
    int pmink_lua_vp_to_int(void* pm, void* vp);
    // VariantParam to c double
    double pmink_lua_vp_to_double(void* pm, void* vp);
    // VariantParam to c char
    char pmink_lua_vp_to_char(void* pm, void* vp);
    // VariantParam to byte array
    unsigned char* pmink_lua_vp_to_octets(void* pm, void* vp);
    // VariantParam to pointer
    void* pmink_lua_vp_to_pointer(void* pm, void* vp);
    // VariantParam get type
    int pmink_lua_vp_get_type(void* pm, void* vp);
    // VariantParam get size in bytes
    int pmink_lua_vp_get_size(void* pm, void* vp);
    // VariantParam set C string
    bool pmink_lua_vp_set_str(void* pm, void* vp, const char* val);
    // VariantParam set int
    bool pmink_lua_vp_set_int(void* pm, void* vp, int val);
    // VariantParam set bool
    bool pmink_lua_vp_set_bool(void* pm, void* vp, bool val);
    // VariantParam set double
    bool pmink_lua_vp_set_double(void* pm, void* vp, double val);
    // VariantParam set char
    bool pmink_lua_vp_set_char(void* pm, void* vp, char val);
    // VariantParam set octets
    bool pmink_lua_vp_set_octets(void* pm, void* vp, unsigned char* val, int size);
    // VariantParam set pointer
    bool pmink_lua_vp_set_pointer(void* pm, void* vp, void* val);

}




#endif /* PMINK_LUA_IF_H_ */
