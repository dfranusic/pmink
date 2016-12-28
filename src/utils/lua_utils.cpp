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


#include <lua_utils.h>

void lua_utils::lua_stack_dump (lua_State *L) {
    int i;
    int top = lua_gettop(L);
    printf("================ LUA: Stack size = [%3d], State = [%p] ================\n", top, L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        int i2 = i - top - 1;
        switch (t) {
            case LUA_TSTRING:  /* strings */
                printf("(%3d, %3d) LUA_TSTRING: `%s'\n", i, i2, lua_tostring(L, i));
                break;

            case LUA_TBOOLEAN:  /* booleans */
                printf("(%3d, %3d) %s\n", i, i2, lua_toboolean(L, i) ? "LUA_TBOOLEAN: true" : "LUA_TBOOLEAN: false");
                break;

            case LUA_TNUMBER:  /* numbers */
                printf("(%3d, %3d) LUA_TNUMBER: %g\n", i, i2, lua_tonumber(L, i));
                break;

            default:  /* other values */
                printf("(%3d, %3d) OTHER: [%s]\n", i, i2, lua_typename(L, t));
                break;

        }
    }
    printf("===============================================================================\n");
}


