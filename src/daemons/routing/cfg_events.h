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

#ifndef CFG_EVENTS_H_
#define CFG_EVENTS_H_

#include <r14p.h>
#include <config.h>

class WRRConfigMod: public config::CfgNtfCallback {
public:
    WRRConfigMod();
    void run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count);

    r14p::R14PSession* r14ps;
};




#endif /* CFG_EVENTS_H_ */
