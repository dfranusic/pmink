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

#ifndef STP_H_
#define STP_H_

#include <daemon.h>
#include <r14p.h>
#include <r14p_stats.h>
#include <config.h>
#include <pmink_utils.h>
#include <sstream>
#include <boost/regex.hpp>
#include <atomic.h>
#include <r14p_utils.h>
#include <stpd_events.h>
#include <config_r14p.h>
#include "cfg_events.h"

// daemon name and description
#define DAEMON_TYPE		"stpd"
#define DAEMON_DESCRIPTION	"pMINK Signalling Transfer Point daemon"
#define DAEMON_CFG_NODE		"mno stp"


namespace stp{
    // aditional STP params
    enum StpRoutingParamType{
       SRPT_RULE_HUNT_STOP		= 10000,
       SRPT_RULE_CTX_SRI		= 10001,
       SRPT_RULE_CTX_SM			= 10002,
       SRPT_RULE_CTX_SMSTPDU		= 10003,
       SRPT_RULE_CTX_SMPP		= 10004
    };

    // rule/input param index
    enum STPParamIndexType{
	SPIT_RULE_MATCH_PART		= 0,
	SPIT_RULE_TRANSLATE_PART	= 1,
	SPIT_INPUT_TRANSLATE_PART	= 2
    };

    // rule contexts
    enum STPRuleContextType{
	SRCT_DEFAULT			= 0,
	SRCT_MAP_SRI_FOR_SM		= 45,
	SRCT_MAP_SM			= 4446,
	SRCT_PMINK			= 9999
    };

    // rule operator
    enum StpRuleOperatorType{
	SROT_EQ		= 0,
	SROT_NE		= 1,
	SROT_GT		= 2,
	SROT_GTE	= 3,
	SROT_LT		= 4,
	SROT_LTE	= 5,
	SROT_LST_ADD	= 6,
	SROT_LST_DEL	= 7,
	SROT_LST_CLR	= 8

    };

    // operand type
    enum StpOperandType {
	SOTT_NUMBER	= 0,
	SOTT_STRING	= 1,
	SOTT_REGEX	= 2,
	SOTT_VARIABLE	= 3,
	SOTT_LIST	= 4
    };




    // types
    typedef pmink_utils::PooledVPMap<uint32_t> stp_rule_param_t;
    typedef pmink_utils::Rule<stp_rule_param_t, stp_rule_param_t> stp_rule_t;
    typedef pmink_utils::RuleLogic<stp_rule_param_t, stp_rule_param_t> stp_rule_logic_t;
    typedef pmink_utils::ListManager<std::string, std::string> list_m_t;
    typedef pmink_utils::RuleManager<stp_rule_param_t, stp_rule_param_t> rule_m_t;

    enum CfgTransMapType{
	CTMT_CONST	= 0,
	CTMT_VAR	= 1,
	CTMT_DYNAMIC	= 2
    };
    // cfg mapping
    class CfgTranslationItem{
    public:
	CfgTranslationItem();
	CfgTranslationItem(CfgTransMapType _type, uint32_t _value);
	virtual ~CfgTranslationItem();

	CfgTransMapType type;
	uint32_t value;

	virtual uint32_t run();

    };

    class CfgTITimestamp: public CfgTranslationItem{
    public:
	CfgTITimestamp();
	uint32_t run();

    };



    // STP routing rules logic
    class StpRoutingLogic: public stp_rule_logic_t{
    public:
	StpRoutingLogic();

	int run(stp_rule_t* rule, stp_rule_param_t* input);
	bool match(const char* data, stp_rule_param_t* rparam, stp_rule_param_t* input, uint32_t vp_id, bool skip_lciop = false, uint32_t context = 0);
	StpRuleOperatorType op_detect(const char* pattern);
	bool data_match(const char* data1, const char* data2, StpRuleOperatorType op_t);
	int find_set_param(const char* cfg_name,
			   stp_rule_param_t* input,
			   stp_rule_param_t* output,
			   pmink_utils::VariantParamType vp_type,
			   uint32_t vp_id,
			   uint32_t vp_index = SPIT_INPUT_TRANSLATE_PART);

	std::map<uint32_t, CfgTranslationItem*> cfg_trans_map;


    };

    // ref counted stats manager
    class StatsManagerRefc: public pmink_utils::StatsManager, public pmink_utils::RefCounted{};

    // STP Rule processor
    class RuleProcessor{
    public:
	RuleProcessor(pmink_utils::MTMasterSlave<list_m_t, true>* list_m_m,
		      pmink_utils::MTMasterSlave<rule_m_t, true>* rule_m_m);

	bool sync();

	pmink_utils::MTMasterSlave<list_m_t, false> list_m;
	pmink_utils::MTMasterSlave<rule_m_t, false> rule_m;
    };

    // types
    typedef std::map<r14p::R14PClient*, stp::RuleProcessor> rulepc_map_t;
    typedef rulepc_map_t::iterator rulepc_it_t;


    // STP Routing
    class StpRouting{
	friend class ::STPRoutingConfigMod;
    public:

	StpRouting();
	~StpRouting();

	int build_lists(config::ConfigItem* cfg_root, list_m_t* lm);
	int build_list(config::ConfigItem* list_root, list_m_t* lm);
	int build_rules(config::ConfigItem* cfg_root, rule_m_t* rm);
	int build_rule(config::ConfigItem* rule_root, rule_m_t* rm);
	int build_dests(config::ConfigItem* dests_root, stp_rule_param_t* params);
	int run_rules(stp_rule_param_t* input, rule_m_t* rm);
	int prepare_match_line(stp_rule_param_t* rdata, const char* line, uint32_t vp_id, uint32_t context = 0);
	bool field_valid(const char* _const, bool const_only = false);

	boost::regex rgx;
	StpRoutingLogic rlogic;

    };




}


// stp daemon descriptor definition
class StpdDescriptor : public pmink::DaemonDescriptor {
public:
    // constructor
    StpdDescriptor(const char* _type, const char* _desc);
    // destructor
    ~StpdDescriptor();

    // argument processor
    void process_arguements(int argc, char** argv);
    // command line options
    void print_help();
    void init_r14p();
    int init_config(bool _process_config = true);
    void init();
    void process_config();
    // terminate event
    void terminate();

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
    // config r14p client
    r14p::R14PClient* cfgd_r14pc;
    // hbeat
    r14p::HeartbeatInfo* hbeat;
    // non srvc handler
    r14p::R14PCallbackMethod* non_srvc_hdnlr;
    // new service msg
    NewSrvcMsg new_srvc_msg;
    // extra options
    pmink_utils::VariantParamMap<uint32_t> extra_params;
    // cfg events
    STPRoutingConfigMod routing_mod_handler;
    STPListsConfigMod lists_mod_handler;
    // sgnd r14p client
    r14p::R14PClient* dest_r14pc;
    // stp routing
    stp::StpRouting stp_routing;
    // master list manager
    pmink_utils::MTMasterSlave<stp::list_m_t, true> list_m;
    // master rule manager
    pmink_utils::MTMasterSlave<stp::rule_m_t, true> rule_m;
    // rule processor map
    stp::rulepc_map_t rule_pc_map;



};




#endif /* STP_H_ */
