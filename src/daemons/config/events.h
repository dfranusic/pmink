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

#ifndef CONFIGD_EVENTS_H_
#define CONFIGD_EVENTS_H_

#include <iostream>
#include <string>
#include <sstream>
#include <r14p.h>
#include <daemon.h>
#include <config.h>
#include <config_r14p.h>
#include <pmink_utils.h>
using namespace std;

// fwd declaration
class NewStream;


// stream next event definition
class StreamNext: public r14p::R14PCallbackMethod {
public:
    // constructor
    StreamNext();
    // TAB mode
    void process_tab(r14p::R14PCallbackArgs* args);
    // ENTER mode
    void process_enter(r14p::R14PCallbackArgs* args);
    // GET mode
    void process_get(r14p::R14PCallbackArgs* args);
    // handler method
    void run(r14p::R14PCallbackArgs* args);
    // members
    config::ConfigItem* cfg_res;
    NewStream* new_stream;
    uint32_t pt_cfg_cfg_line;
    uint32_t pt_cfg_cfg_error;
    uint32_t pt_cfg_item_name;
    uint32_t pt_cfg_item_path;
    uint32_t pt_cfg_item_desc;
    uint32_t pt_cfg_item_ns;
    uint32_t pt_cfg_item_value;
    uint32_t pt_cfg_item_nvalue;
    uint32_t pt_cfg_item_count;
    uint32_t pt_cfg_item_nt;


};


// stream finished
class StreamDone : public r14p::R14PCallbackMethod {
public:
    // event handler method
    void run(r14p::R14PCallbackArgs* args);

    // config change notification
    std::vector<config::CfgNotification*> ntfy_lst;

};

class NewStream;

// new stream event definition
class NewStream: public r14p::R14PCallbackMethod {
public:
    // constructor
    NewStream();
    // TAB mode
    void process_tab(r14p::R14PCallbackArgs* args);
    // ENTER mode
    void process_enter(r14p::R14PCallbackArgs* args);
    // GET mode
    void process_get(r14p::R14PCallbackArgs* args);
    // REPLICATE mode
    void process_replicate(r14p::R14PCallbackArgs* args);
    // User LOGIN
    void process_user_login(r14p::R14PCallbackArgs* args);
    // User LOGOUT
    void process_user_logout(r14p::R14PCallbackArgs* args);
    // prepare notification
    void prepare_notifications();

    // event handler method
    void run(r14p::R14PCallbackArgs* args);

    // get config user id from R14P message
    int get_cfg_uid(config::UserId* usr_id, asn1::R14PMessage* in_msg, int sess_id);

    // members
    config::UserId cfg_user_id;
    std::string line;
    std::string cli_path;
    config::Config* config;
    int config_action;
    std::string tmp_lst[50];
    std::string tmp_err[50];
    int tmp_size;
    int res_size;
    int error_count;
    unsigned int res_index;
    int err_index;
    config::ConfigItem ac_res;
    uint32_t ac_res_count;
    config::ConfigItem tmp_node_lst;
    config::ConfigItem* last_found;
    config::ConfigModeType cm_mode;
    config::ConfigACMode ac_mode;
    std::stringstream line_stream;
    std::string line_buffer;
    int line_stream_lc;
    StreamNext stream_next;
    StreamDone stream_done;
    uint32_t ca_cfg_result;
    uint32_t pt_pmink_config_ac_line;
    uint32_t pt_pmink_config_cfg_line_count;
    uint32_t pt_pmink_config_cli_path;
    uint32_t pt_pmink_config_ac_err_count;
    uint32_t pt_cfg_item_cm_mode;

};

// client idle
class ClientIdle: public r14p::R14PCallbackMethod {
public:
    ClientIdle();
    void run(r14p::R14PCallbackArgs* args);
    config::Config* config;

};

// client terminated
class ClientDone: public r14p::R14PCallbackMethod {
public:
    ClientDone(config::Config* _config);
    void run(r14p::R14PCallbackArgs* args);
    config::Config* config;

};

// client down (client terminating, client re-connecting)
class ClientDown: public r14p::R14PCallbackMethod {
public:
    ClientDown(config::Config* _config);
    void run(r14p::R14PCallbackArgs* args);
    config::Config* config;

};



// new client R14P event
class NewClient: public r14p::R14PCallbackMethod {
public:
    NewClient(config::Config* _config);
    void run(r14p::R14PCallbackArgs* args);
    // members
    NewStream new_stream;
    ClientIdle client_idle;
    config::Config* config;

};





#endif /* CONFIGD_EVENTS_H_ */
