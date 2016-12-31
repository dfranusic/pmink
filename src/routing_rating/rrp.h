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

#ifndef RRP_H_
#define RRP_H_

#include <pmink_utils.h>
#include <ring.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace rrp{
    const int _RRP_VERSION_ 		= 1;

    enum RRConnectionType{
	RRCT_CLIENT			= 0,
	RRCT_SERVER			= 1
    };

    const int _RRP_PARAM_COUNT		= 84;

    enum RRParamType{
	RRPT_UNKNOWN			= 0,
	RRPT_SIZE			= 1,
	RRPT_SEQUENCE_STATE		= 2,
	RRPT_FROM			= 3,
	RRPT_TO				= 4,
	RRPT_CALL_ID			= 5,
	RRPT_STATUS			= 6,
	RRPT_VERSION			= 7,
	RRPT_SEQUENCE			= 8,
	RRPT_FOUND			= 9,
	RRPT_CONFIG			= 10,
	RRPT_METHOD			= 11,
	RRPT_ROUTE			= 12,
	RRPT_SRC_TRUNK			= 13,
	RRPT_TYPE			= 14,
	RRPT_MSISDN			= 15,
	RRPT_SESSION_ID			= 16,
	RRPT_FROM_PORT			= 17,
	RRPT_TO_PORT			= 18,
	RRPT_DST_CTX			= 19,
	RRPT_SRC_CTX			= 20,
	RRPT_MAP_VERSION		= 21,
	RRPT_GT_CALLED			= 22,
	RRPT_GT_CALLING			= 23,
	RRPT_SCA			= 24,
	RRPT_IMSI			= 25,
	RRPT_CALLED_SSN			= 26,
	RRPT_CALLING_SSN		= 27,
	RRPT_NNN			= 28,
	RRPT_GUID			= 29,
	RRPT_SMS_OA_ADDR		= 30,
	RRPT_SMS_OA_TON			= 31,
	RRPT_MSG_ID			= 32,
	RRPT_DST_TRUNK			= 33,
	RRPT_CONN_TYPE			= 34,
	RRPT_ERR_INFO			= 35,
	RRPT_RATE_DIRECTION		= 36,
	RRPT_DIRECTION			= 37,
	RRPT_INTERNAL_STATUS		= 38,
	RRPT_LATENCY			= 39,
	RRPT_LATENCY_NSEC		= 40,
	RRPT_STATE			= 41,
	RRPT_PACKET_DATA		= 42,
	RRPT_TIMESTAMP			= 43,
	RRPT_TIMESTAMP_NSEC		= 44,
	RRPT_OPC			= 45,
	RRPT_DPC			= 46,
	RRPT_RRP_METHOD			= 47,
	RRPT_ERROR_CODE			= 48,
	RRPT_ERROR_TYPE			= 49,
	RRPT_USERNAME			= 50,
	RRPT_IP_ADDRESS			= 51,
	RRPT_CALLING_TON		= 52,
	RRPT_CALLING_NPI		= 53,
	RRPT_CALLED_TON			= 54,
	RRPT_CALLED_NPI			= 55,
	RRPT_TCAP_SID			= 56,
	RRPT_TCAP_DID			= 57,
	RRPT_SCOA			= 58,
	RRPT_SCOA_TON			= 59,
	RRPT_SCOA_NPI			= 60,
	RRPT_SCDA			= 61,
	RRPT_SCDA_TON			= 62,
	RRPT_SCDA_NPI			= 63,
	RRPT_TCAP_OID			= 64,
	RRPT_SMS_OA_NPI			= 65,
	RRPT_SMS_DA_ADDR		= 66,
	RRPT_SMS_DA_TON			= 67,
	RRPT_SMS_DA_NPI			= 68,
	RRPT_SMS_DCS			= 69,
	RRPT_AN				= 70,
	RRPT_SMS_CONCAT_MSG_ID		= 71,
	RRPT_SMS_CONCAT_PARTNUM		= 72,
	RRPT_SMS_CONCAT_PARTS		= 73,
	RRPT_SMS_TEXT			= 74,
	RRPT_FILTER_RESULT		= 75,
	RRPT_FILTER_EXIT_RULE		= 76,
	RRPT_DRM_ERR			= 77,
	RRPT_PORT			= 78,
	RRPT_MNP_MCC			= 79,
	RRPT_MNP_MNC			= 80,
	RRPT_MNP_CACHED			= 81,
	RRPT_SMSC_ID			= 82,
	RRPT_MSG_STATE			= 83





    };

    enum RRMethodType{
	RRRT_UNKNOWN			= 1000,
	RRRT_REGISTER			= 1001,
	RRRT_CFG_UPDATE_ERR_MAPS	= 1002,
	RRRT_TMP_ERR_LIST		= 1003,
	RRRT_ROUTE			= 1004,
	RRRT_HBEAT			= 1005,
	RRRT_STORE			= 1006,
	RRRT_DELETE			= 1007,
	RRRT_WINDOW			= 1008,
	RRRT_RATE			= 1009,
	RRRT_FILTER_RATE		= 1010,
	RRRT_MNP			= 1011


    };

    enum RRSeqStateType{
	RRSST_UNKNOWN			= 10000,
	RRSST_BEGIN			= 10001,
	RRSST_CONTINUE			= 10002,
	RRSST_END			= 10003,
	RRSST_SEQ_END			= 10004
    };


    enum RREventIdType{
	RREIT_SEQUENCE_NEW		= 0,
	RREIT_SEQUENCE_NEXT		= 1,
	RREIT_SEQUENCE_END		= 2,
	RREIT_SEQUENCE_TIMEOUT		= 3,

	RREIT_CLIENT_NEW		= 4,
	RREIT_CLIENT_TERMINATED		= 5
    };

    enum RRConnectionStateType{
	RRCST_UNKNOWN			= 0,
	RRCST_REGISTERED		= 1
    };


    enum RREventArgIdType{
	RREAIT_SEQUENCE			= 0,
        RREAIT_CONNECTION               = 1
    };


    enum RRErrorCode{
	RREC_UNKNOWN			= 0,
	RREC_200			= 200,
	RREC_401			= 401,
	RREC_404			= 404,
	RREC_481			= 481,
	RREC_482			= 482,
	RREC_483			= 483,
	RREC_485			= 485,
	RREC_505			= 505

    };

    enum RRTypeType {
	RRTT_MO_FWD_SM			= 10,
	RRTT_MT_FWD_SM			= 11,
	RRTT_SUBMIT_SM			= 20,
	RRTT_DELIVER_SM			= 21,
	RRTT_DELIVER_SM_DRM		= 22,
	RRTT_SRI_SM			= 30,
	RRTT_MNP_SRI_SM			= 40,
	RRTT_MNP_ENUM			= 41,
	RRTT_WINDOW_ADD			= 80,
	RRTT_WINDOW_DELETE		= 81,
	RRTT_RETURN_RESULT		= 100,
	RRTT_MNP_ROUTE			= 200


    };

    enum RRRateDirectionType {
	RRRDT_CUSTOMER			= 0,
	RRRDT_VENDOR			= 1
    };

    enum RRInternalStatusType {
	RRIST_UNKNOWN			= 0,
	RRIST_SUCCESS			= 1,
	RRIST_FAILURE			= 2
    };

    enum RRDirectionType {
	RRDT_RECEIVED			= 1,
	RRDT_SENT			= 2
    };

    enum RRDefaultEncodingTpye {
	RRDET_NOT_IN_USE		= 0,
	RRDET_7BIT			= 1,
	RRDET_ASCII			= 2,
	RRDET_UCS2			= 3
    };

    // fwd
    class RRSession;
    class RRSequence;
    class RRConnection;

    // typedefs
    typedef pmink_utils::EventArgs<rrp::RREventArgIdType, void*> rrp_event_args_t;
    typedef pmink_utils::EventHandler<rrp::RREventArgIdType, void*> rrp_event_t;

    class RRParam{
    public:
	RRParam(): id(0), type(pmink_utils::DPT_INT){}
	RRParam(uint32_t _id, pmink_utils::VariantParamType _type): id(_id), type(_type){}

	uint32_t id;
	pmink_utils::VariantParamType type;
    };

    class RRRouteParam{
    public:
	enum RRRouteParamType{
	    RRRPT_ROUTE_NO		= 0,
	    RRRPT_SRC_TRUNK		= 1,
	    RRRPT_DST_TRUNK		= 2,
	    RRRPT_TYPE			= 3,
	    RRRPT_STORE_AND_FWD		= 4,
	    RRRPT_CGPA			= 5,
	    RRRPT_CDPA			= 6,
	    RRRPT_SSN			= 7,
	    RRRPT_SCOA			= 8,
	    RRRPT_SCDA			= 9,
	    RRRPT_NNN			= 10,
	    RRRPT_IMSI			= 11,
	    RRRPT_MSISDN		= 12,
	    RRRPT_MAP_VER		= 13,
	    RRRPT_MAP_AUTO_FB		= 14,
	    RRRPT_ERR_REMAP_LST		= 15,
	    RRRPT_FILTER_FIRST		= 16,
	    RRRPT_TEMP_ERR_LST		= 17,
	    RRRPT_PERM_ERR_LST		= 18,
	    RRRPT_DO_HLR		= 19,
	    RRRPT_TIMEOUT		= 20,
	    RRRPT_ROUTE_ID		= 21,
	    RRRPT_HLR_GT		= 22,
	    RRRPT_SMPP_OPT_SUPPORTED	= 23,
	    RRRPT_SMPP_CUSTOMER_ENC_DEF	= 24,
	    RRRPT_SMPP_VENDOR_ENC_DEF	= 25,
	    RRRPT_SMS_OA_ADDR		= 26,
	    RRRPT_SMS_OA_TON		= 27,
	    RRRPT_SMS_OA_NPI		= 28,
	    RRRPT_SMS_DA_ADDR		= 29,
	    RRRPT_SMS_DA_TON		= 30,
	    RRRPT_SMS_DA_NPI		= 31



	};

	RRRouteParam(): count(0){}

	pmink_utils::PooledVPMap<uint32_t> params;
	uint32_t count;

	void reset();
	int decode(const char* str);
	int encode(unsigned char* out);


    };

    class RRStatus{
    public:
	RRStatus(): id(0){}
	RRStatus(uint32_t _id, const char* _msg): id(_id){
	    msg.assign(_msg);
	}
	uint32_t id;
	std::string msg;

	static uint32_t get_status(const char* str);
    };

    class RRSize{
    public:
	RRSize(): size(0), fragment(0), more_fragments(false){
	    memset(uuid, 0, sizeof(uuid));
	}
	uint32_t size;
	uint32_t fragment;
	bool more_fragments;
	unsigned char uuid[16];

	static int get_size(const char* size_str, RRSize* size_p);
	static int encode_size(RRSize* size_p, unsigned char* buffer, int buffer_length);


    };

    class RRPDU {
	friend class RRSequence;
	friend int decode(const unsigned char* data, int data_length, RRPDU* pdu, RRSession* rrs);
    public:
	RRPDU();
	~RRPDU();

	pmink_utils::PooledVPMap<uint32_t> params;
	pmink_utils::PooledVPMap<uint32_t> new_params;

	int encode(unsigned char* buffer, int buffer_length, int offset, int* result_length, RRSession* sess = NULL);

	void reset();

    private:
	pmink_utils::EventManager<RREventIdType, RREventArgIdType, void*> e_handler;
	RRSequence* sequence;
	uint32_t pcount[_RRP_PARAM_COUNT];

    };

    class RRSequence{
	friend class RRPDU;
    public:
	RRSequence();
	~RRSequence();

   // private:
	RRPDU pdu;
	pmink_utils::EventManager<RREventIdType, RREventArgIdType, void*> e_handler;
	RRConnection* connection;
	char destination_id[65];
	unsigned char uuid[16];
	bool free_on_send;
	pmink::Atomic<bool> out;
	time_t timestamp;
	bool ack_rcvd;
	bool timed_out;
	RRSeqStateType seq_state;
	uint32_t seq_num;
	RRMethodType method;
	pmink_utils::PooledVPMap<uint32_t> params;

	void toggle_ack_rcvd();
	void send();
	void reset(bool reset_uuid, bool mod_params);
	void gen_ack(bool clear_params = true);
    };


    class RRStateMachine{
    public:
	RRStateMachine();
	~RRStateMachine();

	void init(RRConnection* _rrc);
	int run(RRPDU* pdu, RRConnection* rrc);

	void process_begin(RRSequence* tmp_seq);
	void process_continue(RRSequence* tmp_seq);
	void process_end(RRSequence* tmp_seq, bool remove_seq = true);
	void process_seq_end(RRSequence* tmp_seq);

    };



    class RRConnection {
	friend class RRPDU;
	friend class RRSequence;
	friend class RRSession;
	friend class RRStateMachine;
	friend class RegisterEvent;
    public:
	RRConnection(int _socket,
		     const char* _end_point_address,
		     unsigned int _end_point_port,
		     const char* _local_point_address,
		     unsigned int _local_point_port,
		     RRConnectionType _direction,
		     int stream_pool_size,
		     RRSession* _session);

	~RRConnection();

	pmink::Atomic<RRConnectionStateType> state;
	memory::Pool<RRSequence, true> seq_pool;
	boost::mt19937 ran_mt19937;
	boost::uuids::random_generator* random_generator;
	pmink::RingBuffer<RRSequence*> out_queue;
	pmink::Atomic<bool> registered;
	pmink::Atomic<bool> seq_timeout_check;
	pmink::Atomic<bool> seqs_active;
	pmink::Atomic<bool> timestamp;
	RRSession* session;
	pmink_utils::EventManager<RREventIdType, RREventArgIdType, void*> e_handler;

	int generate_uuid(unsigned char* out);
	RRSequence* new_sequence(const char* _dest_id, RRMethodType _method, bool reset_uuid = true, RRSequence* rrs = NULL);
	bool validate_seq_num(RRPDU* pdu, unsigned int expected_seq_num);

	RRSequence* get_sequence(unsigned char* _uuid);
	void add_sequence(RRSequence* _seq);
	void remove_sequence(RRSequence* _seq);
	int register_client();
	void process_timeout(bool override = false);
	int init_reconnect();
	int reconnect_socket();


    private:
	RRConnectionType direction;
	int socket;
	std::vector<RRSequence*> seqs;
	pmink_utils::ThreadManager thm;
	unsigned char session_id[16];
	char end_point_address[16];
	unsigned int end_point_port;
	char local_point_address[16];
	unsigned int local_point_port;
	RRStateMachine rrp_sm;
	pmink::Atomic<bool> reconnect_flag;
	pmink::Atomic<bool> end_port_changed_flag;


	// locks
	pmink_utils::Mutex mtx_seqs;
	pmink_utils::Spinlock slock_uuid;

	// threads
	static void* in_loop(void* args);
	static void* out_loop(void* args);
	static void* timeout_loop(void* args);


    };


    class RRSession {
	friend class RRPDU;
	friend class RRSequence;
	friend class RRConnection;
    public:
	RRSession(const char* _daemon_id,
		  int _seq_timeout,
		  int _seq_pool,
		  uint32_t _mtu = 1450,
		  uint32_t _max_pdu_size = 5000);
	~RRSession();

	// param map
	std::map<uint32_t, RRParam> param_def_map;
	// rev param map
	std::map<uint32_t, std::string> rev_param_def_map;
	// status map
	std::map<uint32_t, RRStatus> status_def_map;
	pmink_utils::EventManager<RREventIdType, RREventArgIdType, void*> e_handler;

	const char* find_rev_str(uint32_t id);
	RRParam* find_param_id(const char* pstr);
	RRStatus* find_status_id(uint32_t id);
	void add_conn(RRConnection* conn);
	int remove_conn(RRConnection* conn);
	void set_daemon_id(const char* _daemon_id);
	void set_seq_timeout(int _seq_timeout);
	void set_seq_pool(int _seq_pool);

	RRConnection* connect(	const char* end_point_address,
				unsigned int end_point_port,
				const char* local_address = NULL,
				unsigned int local_port = 0,
				RRConnectionType direction = RRCT_CLIENT);

	int listen(unsigned int local_port, const char* local_address = NULL);


    private:
	std::vector<RRConnection*> conns;
	int seq_timeout;
	int seq_pool;
	char daemon_id[65];
	pmink_utils::Mutex mtx;
	int server_socket;
	char server_local_address[16];
	int server_port;
	pmink_utils::ThreadManager thm;
	uint32_t mtu;
	uint32_t max_pdu_size;

	void init_pdef_map();
	static void* server_loop(void* args);


    };


    int decode(const unsigned char* data, int data_length, RRPDU* pdu, RRSession* rrs);


}




#endif /* RRP_H_ */
