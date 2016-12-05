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


#ifndef CONFIGDC_H_
#define CONFIGDC_H_

#include <daemon.h>
#include <r14p.h>
#include <config.h>
#include <pmink_utils.h>
#include <sstream>
#include <boost/regex.hpp>
#include <atomic.h>
#include <r14p_utils.h>
#include <config_r14p.h>

// daemon name and description
#define DAEMON_TYPE		"cfgdc"
#define DAEMON_DESCRIPTION	"pMINK Config daemon client"

// stream finished
class StreamEnd: public r14p::R14PCallbackMethod {
public:
    StreamEnd();

    void run(r14p::R14PCallbackArgs* args);
    bool timeout;

};

// stream next
class StreamNext: public r14p::R14PCallbackMethod {
public:
    StreamNext();
    void run(r14p::R14PCallbackArgs* args);
    // members
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


// config daemon client descriptor
class CfgdcDescriptor : public pmink::DaemonDescriptor {
public:
    // constructor
    CfgdcDescriptor(const char* _type, const char* _desc);
    ~CfgdcDescriptor();

    // argument processor
    void process_arguements(int argc, char** argv);
    // command line options
    void print_help();
    void init_r14p();
    void init();
    // terminate event
    void terminate();
    int process_cfgd_script();
    int configd_user_auth();
    int configd_user_logout();
    int configd_process_line(char* line, bool do_discard = true, bool show_errors = true);

    // extra options
    pmink_utils::VariantParamMap<uint32_t> extra_params;
    // routing daemons
    std::vector<std::string*> routing_daemons;
    // r14p session
    r14p::R14PSession* r14ps;
    // script file
    char* cfg_script;
    // config
    config::Config* config;
    // current cfg id
    unsigned char cfgd_id[16];
    // config auth user id
    config::UserId cfgd_uid;
    // stream handlers
    StreamNext snext;
    StreamEnd send;
    // semaphore
    sem_t sem_cfgd;


};




#endif /* CONFIGDC_H_ */
