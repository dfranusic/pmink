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

#include <config.h>
namespace sgn{
    class ApplicationServer;
    class SMPP_ApplicationServer;
    class M3UAConnection;
    class SMPPConnection;
}

// pcap
class PCAPInstanceConfigMod: public config::CfgNtfCallback {
public:
    void run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count);
    int del_conn(config::ConfigItem* inst);
    void add_conn(config::ConfigItem* inst);
};

// m3ua
class ASPConfigMod: public config::CfgNtfCallback {
public:
    void run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count);
    void add_conn(config::ConfigItem* asp_node, sgn::ApplicationServer* as = NULL, bool conn_active = false);
    sgn::ApplicationServer* del_conn(config::ConfigItem* asp_node, bool* active);
    void update_link(sgn::M3UAConnection* m3ua_conn, sgn::ApplicationServer* as, bool conn_active);
};


class ASConfigMod: public config::CfgNtfCallback {
public:
    void run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count);
    void del_as(config::ConfigItem* as_node);
    void add_as(config::ConfigItem* as_node);
};

class AS_ASPConfigMod: public config::CfgNtfCallback {
public:
    void run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count);
    void del_as_asp(config::ConfigItem* asp_node);
    void add_as_asp(config::ConfigItem* asp_node);
};


// smpp
class SMPP_ASPConfigMod: public config::CfgNtfCallback {
public:
    void run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count);
    void add_conn(config::ConfigItem* asp_node, sgn::SMPP_ApplicationServer* as = NULL, bool conn_active = false);
    sgn::SMPP_ApplicationServer* del_conn(config::ConfigItem* asp_node, bool* active);
    void update_link(sgn::SMPPConnection* smpp_conn, sgn::SMPP_ApplicationServer* as, bool conn_active);
};


class SMPP_ASConfigMod: public config::CfgNtfCallback {
public:
    void run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count);
    void del_as(config::ConfigItem* as_node);
    void add_as(config::ConfigItem* as_node);
};

class SMPP_AS_ASPConfigMod: public config::CfgNtfCallback {
public:
    void run(config::ConfigItem* cfg, unsigned int mod_index, unsigned int mod_count);
    void del_as_asp(config::ConfigItem* asp_node);
    void add_as_asp(config::ConfigItem* asp_node);
};





#endif /* CFG_EVENTS_H_ */
