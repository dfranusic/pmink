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

#ifndef PLGCFG_EVENTS_H_
#define PLGCFG_EVENTS_H_

#include <r14p.h>
#include <config.h>
#include <cli.h>
#include <sstream>
#include <atomic.h>

// plugin information
class PluginInfo {
public:
    PluginInfo();
    ~PluginInfo();

    config::Config* config;
    cli::CLIService* cli;
    r14p::R14PSession* r14ps;
    std::vector<std::string*> cfgd_lst;
    unsigned char last_cfgd_id[16];
    r14p::R14PClient* last_r14pc;
    sem_t sem_cfgd;
    config::UserId cfg_user_id;
    //bool cfgd_active;
    pmink::Atomic<bool> cfgd_active;
    r14p::HeartbeatInfo* hbeat;

};

// stream finished
class StreamEnd: public r14p::R14PCallbackMethod {
public:
    StreamEnd(PluginInfo* _pi);
    void run(r14p::R14PCallbackArgs* args);

private:
    PluginInfo* plugin_info;

};


// stream next
class StreamNext: public r14p::R14PCallbackMethod {
public:
    StreamNext(PluginInfo* _pi, config::ConfigItem* _cfg_res);
    void run(r14p::R14PCallbackArgs* args);
    // TAB mode
    void process_tab(r14p::R14PCallbackArgs* args);
    // ENTER mode
    void process_enter(r14p::R14PCallbackArgs* args);
    // members
    PluginInfo* plugin_info;
    config::ConfigItem* cfg_res;
    config::ConfigModeType cm_mode;
    config::ConfigACMode ac_mode;
    std::stringstream line_stream;
    std::string line_buffer;
    int line_stream_lc;
    int error_count;
    int err_index;
    std::string err_lst[50];
};




#endif /* PLGCFG_EVENTS_H_ */
