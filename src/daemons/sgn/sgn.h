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

#ifndef SGN_H_
#define SGN_H_

#include <daemon.h>
#include <r14p.h>
#include <r14p_stats.h>
#include <config.h>
#include <pmink_utils.h>
#include <antlr_utils.h>
#include <sstream>
#include <boost/regex.hpp>
#include <sgnd_events.h>
#include <atomic.h>
#include <r14p_utils.h>
#include <connection.h>
#include <config_r14p.h>
#include <r14p_stats.h>
#include "cfg_events.h"

// daemon name and description
#define DAEMON_TYPE		"sgnd"
#define DAEMON_DESCRIPTION	"pMINK Signalling daemon"
#define DAEMON_CFG_NODE		"mno sgn"

// data process
namespace sgn{
    class DataProcessor{
	typedef std::vector<sgn::ConnectionBase*> dproc_conns_lst_t;
	typedef pmink_utils::MTMasterSlave<dproc_conns_lst_t, false> dproc_conns_t;
    public:
	DataProcessor(uint32_t _id);
	~DataProcessor();

	void start();
	void stop();
	int add_conn(sgn::ConnectionBase* conn);
	int del_conn(const char* id);
	int sync_client_fds(dproc_conns_lst_t& lst);
	int sync_server_fds(dproc_conns_lst_t& lst);
	int sync(dproc_conns_t& conns);

	// data processor threads
	static void* dproc_rx_loop(void* args);
	static void* dproc_tx_loop(void* args);
	static void* dproc_proc_loop(void* args);
	static void* dproc_server_loop(void* args);
	static void* dproc_reconnect_loop(void* args);

	pmink_utils::ThreadManager thm;

	pmink_utils::MTMasterSlave<dproc_conns_lst_t, true> client_conns;
	pmink_utils::MTMasterSlave<dproc_conns_lst_t, true> server_conns;
	pmink_utils::MTMasterSlave<dproc_conns_lst_t, true> pcap_conns;

	uint32_t id;
	uint32_t fds_max;

	pollfd* fds_lst_client;
	uint32_t fds_size_client;

	pollfd* fds_lst_server;
	uint32_t fds_size_server;

    };

}

// routing daemon descriptor definition
class SgndDescriptor : public pmink::DaemonDescriptor {
public:
    // constructor
    SgndDescriptor(const char* _type, const char* _desc);
    // destructor
    ~SgndDescriptor();

    void process_arguements(int argc, char** argv);
    void print_help();
    void init_r14p();
    int init_config(bool _process_config = true);
    void init();
    void process_config();
    void terminate();
    static void* loop_p_expire(void* args);
    sgn::ApplicationServer* get_as(const char* label);
    sgn::SMPP_ApplicationServer* get_smpp_as(const char* label);

    // data processors
    pmink_utils::Mutex dprocessors_lock;
    pmink_utils::WRR<sgn::DataProcessor*> dprocessors;

    // m3ua connections (ASP)
    std::map<std::string, sgn::M3UAConnection*> asp_map;
    // m3ua application servers (AS)
    std::map<std::string, sgn::ApplicationServer*> as_map;

    // smpp connections (ASP)
    std::map<std::string, sgn::SMPPConnection*> smpp_asp_map;
    // smpp application servers (AS)
    std::map<std::string, sgn::SMPP_ApplicationServer*> smpp_as_map;

    // pcap connections
    std::map<std::string, sgn::PCAPConnection*> pcap_map;

    // loop protection
    pmink_utils::CorrelationMap<uint32_t, uint32_t> loop_cmap;
    pmink_utils::ThreadManager thm;

    // local port range
    pmink_utils::Range<uint32_t> local_ports;
    // pcap lock
    pmink_utils::Mutex pcap_lock;
    // as lock
    pmink_utils::Mutex as_lock;
    // smpp lock
    pmink_utils::Mutex smpp_as_lock;
    // routing daemons
    std::vector<std::string*> routing_daemons;
    // r14p session
    r14p::R14PSession* r14ps;
    // R14P stats
    r14p::R14PStatsSession* r14p_stats;
    // sgn stats
    pmink_utils::StatsManager stats;
    // idt map
    r14p::ParamIdTypeMap idt_map;
    // r14p service message manager
    r14p::ServiceMsgManager* r14psmm;
    // cfgd activity flag
    pmink::Atomic<uint8_t> cfgd_active;
    // config
    config::Config* config;
    // current cfg id
    unsigned char cfgd_id[16];
    // config auth user id
    config::UserId cfgd_uid;
    // rt r14p client
    r14p::R14PClient* rt_r14pc;
    // hbeat
    r14p::HeartbeatInfo* hbeat;
    // non srvc handler
    r14p::R14PCallbackMethod* non_srvc_hdnlr;
    // new service msg
    NewSrvcMsg new_srvc_msg;
    // extra options
    pmink_utils::VariantParamMap<uint32_t> extra_params;
    // cfg events (pcap)
    PCAPInstanceConfigMod pcap_inst_mod_handler;
    // cfg events (m3ua)
    ASPConfigMod asp_mod_handler;
    ASConfigMod as_mod_handler;
    AS_ASPConfigMod as_asp_mod_handler;
    // cfg events (smpp)
    SMPP_ASPConfigMod smpp_asp_mod_handler;
    SMPP_ASConfigMod smpp_as_mod_handler;
    SMPP_AS_ASPConfigMod smpp_as_asp_mod_handler;
    // asp/as connection up/down events (m3ua)
    AS_WRR_Add as_wrr_add_handler;
    AS_WRR_Remove as_wrr_remove_handler;
    // asp/as connection up/down events (smpp)
    SMPP_WRR_Add smpp_wrr_add_handler;
    SMPP_WRR_Remove smpp_wrr_remove_handler;

    // pcap alloc/dealloc methods
    static lockfree::QItem<packet_descriptor, 0>* pd_alloc(void* pool_owner);
    static int pd_dealloc(lockfree::QItem<packet_descriptor, 0>* pd, void* pool_owner);
    // pcap start/finish methods
    static int pd_start(void* pool_owner);
    static int pd_finish(void* pool_owner);



};



#endif /* SGN_H_ */
