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

#ifndef FGN_H_
#define FGN_H_

#include <daemon.h>
#include <r14p.h>
#include <r14p_stats.h>
#include <config.h>
#include <pmink_utils.h>
#include <sstream>
#include <boost/regex.hpp>
#include <atomic.h>
#include <r14p_utils.h>
#include <fgnd_events.h>
#include <config_r14p.h>
#include "cfg_events.h"
#include <rrp.h>
#include "lua.hpp"
#ifdef FGN_CPP14
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/version.hpp>
#include <boost/fusion/include/std_pair.hpp>
#endif
// daemon name and description
#define DAEMON_TYPE		"fgnd"
#define DAEMON_DESCRIPTION	"pMINK Filtering Gateway Node daemon"
#define DAEMON_CFG_NODE		"mno fgn"

namespace fgn{
    enum CorrelationPhaseType{
	// *** filtering states ***
	CPT_UNKNOWN				= 0,
	CPT_EXPIRED				= 1,
	CPT_NOTIFICATION_RCVD			= 2,
	CPT_SMPP_SMS_FWD			= 3,
	CPT_SS7_SMS_FWD				= 4,
	CPT_SRI_SM_REQ				= 5,
	CPT_SRI_SM_RPL				= 6,
	CPT_CONTINUE_FILTERING			= 7,
	CPT_SS7_SMS_RPL				= 8,
	CPT_UNKNOWN_RESULT			= 9,
	CPT_PD_REQ				= 10,
	CPT_PD_RPL				= 11

    };

    enum CorrelationContextType{
        CTT_IN_LEG				= 1000,
        CTT_OUT_LEG				= 2000,
        CTT_RATING				= 3000,
        CTT_ROUTING				= 4000
    };

    enum RuleParamType{
       RPT_RULE_HUNT_STOP			= 10000,
       RPT_RULE_CTX_SRI				= 10001,
       RPT_RULE_CTX_SM				= 10002,
       RPT_RULE_CTX_SMSTPDU			= 10003,
       RPT_RULE_CTX_SMPP			= 10004,
       RPT_RULE_DEFAULT_FRES			= 10005,
       RPT_RULE_FJUMP				= 10006,
       RPT_RULE_CTX_CMD_SRI_REQ			= 20000,
       RPT_RULE_CTX_CMD_SRI_RPL			= 20001,
       RPT_RULE_RESUME_INDEX			= 30000,
       RPT_RULE_RESUME_SUB_INDEX		= 30001,
       RPT_RULE_RESUME_LOOP_INDEX		= 30002

    };

    // rule/input param index
    enum ParamIndexType{
	PIT_RULE_MATCH_PART			= 0,
	PIT_RULE_TRANSLATE_PART			= 1,
	PIT_INPUT_TRANSLATE_PART		= 2,
	PIT_INPUT_CMD_REQ_PART			= 200,
	PIT_INPUT_CMD_RPL_PART			= 201
    };

    // rule contexts
    enum RuleContextType{
	RCT_DEFAULT				= 0,
	RCT_MAP_SRI_FOR_SM			= 45,
	RCT_MAP_SM				= 4446,
	RCT_CMD_SRI_REQ				= 10000,
	RCT_CMD_SRI_RPL				= 20000,
	RCT_DUMMY				= 30000,
	RCT_PMINK				= 9999
    };

    // rule operator
    enum RuleOperatorType{
	ROT_EQ					= 0,
	ROT_NE					= 1,
	ROT_GT					= 2,
	ROT_GTE					= 3,
	ROT_LT					= 4,
	ROT_LTE					= 5,
	ROT_LST_ADD				= 6,
	ROT_LST_DEL				= 7,
	ROT_LST_CLR				= 8
    };

    // operand type
    enum OperandType {
	OTT_NUMBER				= 0,
	OTT_STRING				= 1,
	OTT_REGEX				= 2,
	OTT_VARIABLE				= 3,
	OTT_LIST				= 4,
	OTT_LUA					= 5
    };

    enum CfgTransMapType{
	CTMT_CONST				= 0,
	CTMT_VAR				= 1,
	CTMT_DYNAMIC				= 2
    };

    // types
    typedef pmink_utils::PooledVPMap<uint32_t> rule_param_t;
    typedef pmink_utils::Rule<rule_param_t, rule_param_t> rule_t;
    typedef pmink_utils::RuleLogic<rule_param_t, rule_param_t> rule_logic_t;
    typedef pmink_utils::ListManager<std::string, std::string> list_m_t;
    typedef pmink_utils::FloodManager<std::string, std::string> flood_m_t;
    typedef pmink_utils::RuleManager<rule_param_t, rule_param_t> rule_m_t;

    #ifdef FGN_CPP14
    // x3 namespace
    namespace x3 = boost::spirit::x3;
    // basic matching related
    namespace basic_matching {
        // operator
        struct opsym_: x3::symbols<unsigned int> {
            opsym_(){
                add("==",   ROT_EQ)
                   ("!=",   ROT_NE)
                   (">",    ROT_GT)
                   (">=",   ROT_GTE)
                   ("<",    ROT_LT)
                   ("<=",   ROT_LTE)
                   (">>",   ROT_LST_ADD)
                   ("<<",   ROT_LST_DEL)
                   ("--",   ROT_LST_CLR);
            }
        };
        const opsym_ opsym;

        // component index
        struct cisym_: x3::symbols<unsigned int> {
            cisym_(){
                add("+",    PIT_INPUT_TRANSLATE_PART)
                   ("->",   PIT_INPUT_CMD_REQ_PART)
                   ("<-",   PIT_INPUT_CMD_RPL_PART);
            }
        };
        const cisym_ cisym;

        // basic matching syntax parser
        /*
        
        [1] != @[2]tcap.const.ct_invoke
        =============================
        0 = operand B type
        1 = left index LCI
        2 = operator
        3 = right index RCI
        4 = operand B

        */
        namespace parser {
            using x3::uint_;
            using x3::int_;
            using x3::lit;
            using x3::_val;
            using x3::_attr;
            using x3::ascii::char_;
            using x3::ascii::print;
            using x3::space;
            using x3::lexeme;
            using x3::ascii::alnum;
            using x3::eoi;

            // process LCI
            auto proc_lci = [](auto& ctx){ _val(ctx)[1] = std::to_string(_attr(ctx)); };

            // process OPERATOR
            auto proc_op = [](auto& ctx){ _val(ctx)[2] = std::to_string(_attr(ctx)); };

            // process INT item
            auto proc_int = [](auto& ctx){
                // type
                _val(ctx)[0] = std::to_string(OTT_NUMBER);
                // int item
                _val(ctx)[4] = std::to_string(_attr(ctx));
            };

            // process LIST item
            auto proc_lst = [](auto& ctx){
                // type
                _val(ctx)[0] = std::to_string(OTT_LIST);
                // list item
                _val(ctx)[4] = _attr(ctx);
            };

            // process VAR item
            auto proc_var = [](auto& ctx){
                // type
                _val(ctx)[0] = std::to_string(OTT_VARIABLE);
                // rci
                _val(ctx)[3] = std::to_string(_attr(ctx).first);
                // const or var item
                _val(ctx)[4] = _attr(ctx).second;
            };

            // proces REGEX item
            auto proc_rgx = [](auto& ctx){
                // type
                _val(ctx)[0] = std::to_string(OTT_REGEX);
                // regex
                _val(ctx)[4] = _attr(ctx);
            };

            // process STRING item
            auto proc_str = [](auto& ctx){
                // type
                _val(ctx)[0] = std::to_string(OTT_STRING);
                // text item
                _val(ctx)[4] = _attr(ctx);
            };


            // process operand B
            auto proc_opB = [](auto& ctx){
                // set default LCI
                 if(_val(ctx).find(1) == _val(ctx).end()) 
                    _val(ctx)[1] = "0";
                
                // set default OP if not set
                if(_val(ctx).find(2) == _val(ctx).end())
                    _val(ctx)[2] = std::to_string(ROT_EQ);

                // add TYPE (0) and OPERAND B (4)
                _val(ctx)[0] = _attr(ctx)[0];
                _val(ctx)[4] = _attr(ctx)[4];

                // RCI (3)
                if(_attr(ctx).find(3) != _attr(ctx).end()) 
                    _val(ctx)[3] = _attr(ctx)[3];
                else 
                    _val(ctx)[3] = "0";
            };

            // *************
            // *** rules ***
            // *************
            // var name
            x3::rule<class var_name, std::string> const var_name = "var_name";
            // statement
            x3::rule<class statement, std::map<int, std::string> > const statement = "statement";
            // left component index
            x3::rule<class lci, unsigned int> const lci = "lci";
            // right component index
            x3::rule<class rci, unsigned int> const rci = "rci";
            // operator
            x3::rule<class op, unsigned int> const op = "operator";
            // operand B
            x3::rule<class operandB, std::map<int, std::string> > const operandB = "operandB";
            // int operand
            x3::rule<class int_operand, int> const int_operand = "int_operand";
            // string operand
            x3::rule<class str_operand, std::string> const str_operand = "str_operand";
            // var operand
            x3::rule<class var_operand, std::pair<unsigned int, std::string> > const var_operand = "var_operand";
            // regex operand
            x3::rule<class rgx_operand, std::string> const rgx_operand = "rgx_operand";
            // list operand
            x3::rule<class lst_operand, std::string> const lst_operand = "lst_operand";
            // ************************
            // *** rule defintions  ***
            // ************************
            // var name def
            auto const var_name_def = +(alnum | char_(".") | char_("_"));
            // lci def
            auto const lci_def = lit("[") > ( lexeme[uint_] | cisym ) > lit("]");
            // rci def
            auto const rci_def = lit("[") > ( lexeme[uint_] | cisym ) > lit("]");
            // operator def
            auto const op_def = opsym;
            // int operand B def
            auto const int_operand_def = lexeme[int_] > eoi; 
            // var operand def
            auto const var_operand_def = lit("@") >> -rci >> lexeme[var_name] > eoi;
            // string operand def
            auto const str_operand_def = lexeme[+print];
            // regex operand def
            auto const rgx_operand_def = lit(":") >> lexeme[+print];
            // lst operand def
            auto const lst_operand_def = lit("@") > lit("{") >> lexeme[var_name] > lit("}") > eoi;
            // operand B def
            auto const operandB_def =   int_operand[proc_int] | 
                                        var_operand[proc_var] |
                                        lst_operand[proc_lst] |
                                        rgx_operand[proc_rgx] |
                                        str_operand[proc_str];
            // statement def
            auto const statement_def = -lci[proc_lci] >> -op[proc_op] >> operandB[proc_opB];

            // define rules
            BOOST_SPIRIT_DEFINE(lci, rci, var_name, op, int_operand, lst_operand,
                                var_operand, str_operand, rgx_operand, operandB, 
                                statement);


        } 
    }
    #endif

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


    class RRPNodeMap{
    public:
	pmink_utils::WRRItem<rrp::RRConnection*>* get_next_node();
	pmink_utils::WRR<rrp::RRConnection*>* get_wrr();

	std::map<std::string, rrp::RRConnection*> rr_map;
	pmink_utils::Mutex wrr_mtx;

    private:
	pmink_utils::WRR<rrp::RRConnection*> wrr;
    };



    // rule logic
    class FilteringLogic: public rule_logic_t{
    public:
	FilteringLogic();

	int run(rule_t* rule, rule_param_t* input);
	bool match( const char* data, 
                    rule_t* rule, 
                    rule_param_t* input, 
                    uint32_t vp_id, 
                    bool skip_lciop = false, 
                    uint32_t context = 0, 
                    uint32_t vp_index = 0, 
                    int* default_res_override = NULL,
                    bool script_args = false);
	RuleOperatorType op_detect(const char* pattern);
	bool data_match(const char* data1, const char* data2, RuleOperatorType op_t);
	int find_set_param(const char* cfg_name,
			   rule_param_t* input,
			   rule_param_t* output,
			   pmink_utils::VariantParamType vp_type,
			   uint32_t vp_id,
			   uint32_t vp_index = PIT_INPUT_TRANSLATE_PART,
			   uint32_t vp_context = 0);

	std::map<uint32_t, CfgTranslationItem*> cfg_trans_map;




	int generate_sri_sm_req(rule_param_t* params);

    };

    // types
    typedef pmink_utils::MTMasterSlave<list_m_t, false> list_slv_t;
    typedef pmink_utils::MTMasterSlave<list_m_t, true> list_mstr_t;
    typedef pmink_utils::MTMasterSlave<flood_m_t, false> flood_slv_t;
    typedef pmink_utils::MTMasterSlave<flood_m_t, true> flood_mstr_t;

    // ref counted stats manager
    class StatsManagerRefc: public pmink_utils::StatsManager, public pmink_utils::RefCounted{};


    // Rule processor
    class RuleProcessor{
    public:
	RuleProcessor();
	RuleProcessor(pmink_utils::MTMasterSlave<list_m_t, true>* list_m_m,
		      pmink_utils::MTMasterSlave<rule_m_t, true>* rule_m_m,
		      pmink_utils::MTMasterSlave<flood_m_t, true>* flood_m_m);

	bool sync();

	pmink_utils::MTMasterSlave<list_m_t, false> list_m;
	pmink_utils::MTMasterSlave<flood_m_t, false> flood_m;
	pmink_utils::MTMasterSlave<rule_m_t, false> rule_m;
	int rule_resume_index;
	int rule_resume_sub_index;
	int rule_resume_loop_index;

	struct Current_pos{
	  int index;
	  int sub_index;
	  int loop_index;
	} current_pos;
    };

    // types
    typedef std::map<r14p::R14PClient*, RuleProcessor> rulepc_map_t;
    typedef rulepc_map_t::iterator rulepc_it_t;

    class FgnPayload{
    public:
	FgnPayload();

	pmink_utils::PooledVPMap<uint32_t> params;
	pmink_utils::PooledVPMap<uint32_t> cmd_params;
	pmink_utils::PooledVPMap<uint32_t>* params_p;
	int tcap_op_index;
	//int sms_tcap_op_index;
	//int hlr_tcap_op_index;
	int route_index;
	int rrp_retry;
	int rrp_maxr;
	CorrelationPhaseType phase;
	__uint128_t guid;
	rrp::RRRouteParam route_hlr;
	pmink::Atomic<uint32_t> refc;
	RuleProcessor* rproc;
	pmink_utils::ParamTuple<> current_vp_id;

	void reset();
	int process_expired();
	int get_mapv(pmink_utils::PooledVPMap<uint32_t>* params);
	timespec get_pmink_ts(pmink_utils::PooledVPMap<uint32_t>* params);

	// rating
	int send_rate_sri_req();
	int send_rate_ss7_sms();
	int send_rate_sms();
	int send_rate_rpl();

    };

    // fwd
    class FilterManager;

    // Fworker
    class FWorker{
    public:
	FWorker();
	FWorker(int _id, int q_size, FilterManager* _fm);
	~FWorker();

	pmink::RingBuffer<FgnPayload*> queue;
	uint32_t id;
	FilterManager* fm;
	//lua_State* L;

    };



    // Filtering
    class FilterManager{
	friend class FilterConfigMod;
    public:

	FilterManager();
	~FilterManager();

	int build_lists(config::ConfigItem* cfg_root, list_m_t* lm);
	int build_list(config::ConfigItem* list_root, list_m_t* lm);
	int build_rules(config::ConfigItem* cfg_root, rule_m_t* rm);
	int build_rule(config::ConfigItem* rule_root, rule_m_t* rm);
	int build_dests(config::ConfigItem* dests_root, rule_param_t* params);
	int run_rules(rule_param_t* input, rule_m_t* rm, rule_t* first_rule = NULL, rule_t** last_rule = NULL);
	int str_to_ciop(const char* _str);
	int prepare_match_line(rule_param_t* rdata, const char* line, uint32_t vp_id, uint32_t context = 0, config::ConfigItem* args = NULL);
	bool field_valid(const char* _const, bool const_only = false);
	static void* worker_loop(void* args);
	static void* timeout_loop(void* args);
	FWorker* get_fw();
	void add_fw(FWorker* fw, const char* id, uint32_t weight);

	boost::regex rgx;
	pmink_utils::ThreadManager thm;
	FilteringLogic flogic;
	pmink_utils::Mutex wrr_lock;
	pmink_utils::WRR<FWorker*> fworkers;

    };


}

class ServiceMessageAsyncDone: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args);
};

// fgnd descriptor definition
class FgndDescriptor : public pmink::DaemonDescriptor {
public:
    // constructor
    FgndDescriptor(const char* _type, const char* _desc);
    // destructor
    ~FgndDescriptor();

    // argument processor
    void process_arguements(int argc, char** argv);
    // command line options
    void print_help();
    void init_r14p();
    void init_rrp();
    int init_config(bool _process_config = true);
    void init();
    void process_config();
    // terminate event
    void terminate();
    int generate_uuid(unsigned char* out);
    void send_stp(fgn::rule_param_t* params);
    void send_pd(fgn::rule_param_t* params);

    // routing daemons
    std::vector<std::string> routing_daemons;
    // config
    std::vector<std::string> config_daemons;
    uint32_t cfgd_ovrd_c;
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
    // stp r14p client
    r14p::R14PClient* stp_r14pc;
    // pd r14p client
    r14p::R14PClient* pd_r14pc;
    // correlation pool
    memory::Pool<fgn::FgnPayload, true> cpool;
    // rule processor pool
    memory::Pool<fgn::RuleProcessor, true> rproc_pool;
    // correlation maps
    pmink_utils::CorrelationMap<__uint128_t, fgn::FgnPayload*> cmap;
    pmink_utils::CorrelationMap<uint32_t, __uint128_t> cmap_tcap;
    pmink_utils::CorrelationMap<__uint128_t, __uint128_t> cmap_r14p;
    // correlation timer resolution
    uint32_t cmap_timer_res;
    // routing node map
    fgn::RRPNodeMap routing_map;
    // rating map
    fgn::RRPNodeMap rating_map;
    // random generator
    pmink_utils::Spinlock slock_uuid;
    boost::mt19937 ran_mt19937;
    boost::uuids::random_generator* random_generator;
    // rrp
    rrp::RRSession rrp_s;
    // r14p srvc msg senet handler
    ServiceMessageAsyncDone smsg_async_done;
    // cfg events
    fgn::FilterConfigMod rules_mod_handler;
    fgn::ListsConfigMod lists_mod_handler;
    // filter manager
    fgn::FilterManager filter_m;
    // master list manager
    pmink_utils::MTMasterSlave<fgn::list_m_t, true> list_m;
    // master rule processor
    pmink_utils::MTMasterSlave<fgn::rule_m_t, true> rule_m;
    // master flood manager
    pmink_utils::MTMasterSlave<fgn::flood_m_t, true> flood_m;
};



#endif /* FGN_H_ */
