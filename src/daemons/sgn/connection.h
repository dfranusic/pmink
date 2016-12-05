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


#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <pmink_utils.h>
#include <pmink_net.h>
#include <spscq.h>
#include <m3ua.h>
#include <pool.h>
#include <ring.h>
#include <sccp.h>
#include <smstpdu.h>
#include <itu_tcap.h>
#include <DialoguePDUs.h>
#include <MAP-SM-DataTypes.h>
#include <GSMMAP.h>
#include <r14p_utils.h>
#include <sctp.h>
#include <smpp.h>
#include <unicode/utf8.h>
#include <unicode/unistr.h>
#include <unicode/stringpiece.h>
#include <unicode/ustream.h>
#include <unicode/ucnv.h>
#include <iconv.h>
#include <pmpcap.h>
#include <tcp_ip.h>


namespace sgn {

    enum ConnectionType {
	CT_UNKNOWN 	= 0,
	CT_SCTP 	= 1,
	CT_M3UA		= 2,
	CT_PMINK	= 3,
	CT_TCP		= 4,
	CT_SMPP		= 5,
	CT_PCAP		= 6,
	CT_PCAP_M3UA	= 7,
	CT_PCAP_SMPP	= 8
    };

    enum ConnectionDirection {
	CD_CLIENT	= 0,
	CD_SERVER	= 1,
	CD_PASSIVE	= 2
    };

    enum ServerConnectionType{
	SCT_ONE_ONE	= 0,
	SCT_ONE_MANY	= 1
    };


    enum PayloadActionType{
	PAT_SEND	= 0,
	PAT_FREE	= 1,
	PAT_PROCESS	= 2
    };

    class Payload{
    public:
	Payload();


	ConnectionType type;
	unsigned char* raw;
	int size;
	PayloadActionType action;

	void* extra;



    };

    class PcapPayload: public Payload {
    public:
	PcapPayload();

	lockfree::QItem<packet_descriptor, 0> pd;
    };

    class SctpPayload: public Payload {
    public:
	SctpPayload();

	uint32_t ppid;
	uint16_t sid;
	sctp_sndrcvinfo sctp_info;
    };

    class TcpPayload: public Payload {
    public:
	TcpPayload();


    };

    class SmppPayload: public TcpPayload{
    public:
	SmppPayload();

	smpp::SmppPDU smpp_pdu;
    };

    class M3uaPayload: public SctpPayload{
    public:
	M3uaPayload();

	m3ua::M3UAPacket m3uap;
    };


    class PminkPayload: public Payload{
    public:
	PminkPayload();

	M3uaPayload m3ua_pld;
	SmppPayload smpp_pld;
	pmink_utils::PooledVPMap<uint32_t> params;

    };

    enum ConnectionParamType{
	// dev
	CPT_DEV_M3UA_SM			= 9001,
	CPT_DEV_SMPP_SM			= 9002,

	// general
	CPT_APPLICATION_SERVER		= 1000,
	CPT_DATA_RETENTION		= 1001,
	CPT_R14P_STATS			= 1002,
	CPT_MAX_CONN_COUNT		= 1003,
	CPT_RECONNECT_TIMER		= 1004,
	CPT_DATA_PROCESSOR		= 1005,
	CPT_LOCAL_PORT_RANGE		= 1006,

	// sctp
	CPT_SCTP_MAX_STREAMS		= 0,
	CPT_SCTP_HB_INTERVAL		= 1,
	CPT_SCTP_PATH_MAX_RETRANS	= 2,
	CPT_SCTP_MAX_INIT_RETRANS	= 3,
	CPT_SCTP_RTO_INITIAL		= 4,
	CPT_SCTP_RTO_MAX		= 5,
	CPT_SCTP_RTO_MIN		= 6,
	CPT_SCTP_MAX_BURST		= 7,
	CPT_SCTP_SACK_TIMEOUT		= 8,
	CPT_SCTP_SACK_FREQ		= 9,

	CPT_SCTP_VALID_COOKIE_LIFE	= 10,
	CPT_SCTP_REMOTE_IP_1		= 11,
	CPT_SCTP_REMOTE_IP_2		= 12,
	CPT_SCTP_LOCAL_IP_1		= 13,
	CPT_SCTP_LOCAL_IP_2		= 14,
	CPT_SCTP_REMOTE_PORT		= 15,
	CPT_SCTP_LOCAL_PORT		= 16,

	// m3ua
	CPT_M3UA_TRAFFIC_MODE		= 17,
	CPT_M3UA_ASPUP_TIMER		= 18,
	CPT_M3UA_HBEAT_TIMER		= 19,
	CPT_M3UA_RT_CTX			= 20,
	CPT_M3UA_OPC			= 21,
	CPT_M3UA_DPC			= 22,

	// protocol decoders
	CPT_DECODE_M3UA			= 23,
	CPT_DECODE_SCCP			= 24,
	CPT_DECODE_ISUP			= 25,
	CPT_DECODE_TCAP			= 26,
	CPT_DECODE_GSM_MAP		= 27,
	CPT_DECODE_SMS_TPDU		= 28,

	// tcp
	CPT_TCP_REMOTE_IP		= 29,
	CPT_TCP_LOCAL_IP		= 30,
	CPT_TCP_REMOTE_PORT		= 31,
	CPT_TCP_LOCAL_PORT		= 32,

	// smpp
	CPT_SMPP_SESSION_INIT_TIMER	= 33,
	CPT_SMPP_ENQUIRE_LINK_TIMER	= 34,
	CPT_SMPP_INACTIVITY_TIMER	= 35,
	CPT_SMPP_RESPONSE_TIMER		= 36,
	CPT_SMPP_BIND_METHOD		= 37,
	CPT_SMPP_BIND_SYSTEM_ID		= 38,
	CPT_SMPP_BIND_PASSWORD		= 39,
	CPT_SMPP_BIND_SYSTEM_TYPE	= 40,
	CPT_SMPP_BIND_TON		= 41,
	CPT_SMPP_BIND_NPI		= 42,
	CPT_SMPP_BIND_IF_VERSION	= 43,

	// pcap
	CPT_PCAP_INTERFACE		= 44,
	CPT_PCAP_PCAP_BUFFER		= 45,
        CPT_PCAP_TYPE                   = 46


    };

    enum ConnectiontEventArgType{
	CEAT_PMPLD			= 0,
	CEAT_AS				= 1,
	CEAT_CONNECTION			= 2
    };

    enum ConnectionEventType{
	CET_SEND_ERROR			= 0,
	CET_CONNECTION_DOWN		= 1,
	CET_CONNECTION_UP		= 2

    };

    enum ConnectionLogicPhaseType{
	CLPT_RX				= 0,
	CLPT_TX				= 1,
	CLPT_SERVER			= 2,
	CLPT_PROC			= 3,
	CLPT_EXTRA			= 4,
	CLPT_RECONNECT			= 5,
	CLPT_PCAP			= 6
    };

    class ConnectionLogic{
    public:
	virtual ~ConnectionLogic(){}
	virtual int run(int phase, void* args = NULL) = 0;
    };

    class ConnectionBase: public pmink_utils::RefCounted{
    public:
	ConnectionBase(ConnectionType _type,
		       ConnectionDirection _direction,
		       ServerConnectionType _server_type,
		       const char* _label);

	virtual ~ConnectionBase();

	const char* get_label();
	ConnectionType get_type();
	ConnectionDirection get_direction();
	ServerConnectionType get_server_type();
	virtual int start();
	virtual int stop();
	ConnectionBase* get_master_conn();
	virtual ConnectionBase* get_conn();
	ConnectionLogic* get_logic();
	void set_logic(ConnectionLogic* _logic);
	int get_client_socket();
	int get_server_socket();
	virtual bool is_garbage();


	pmink_utils::VariantParamMap<ConnectionParamType> params;
	pmink_utils::StatsManager stats;
	pmink::Atomic<bool> active;
	pmink_utils::EventManager<ConnectionEventType, ConnectiontEventArgType, void*>* get_e_handler();


    protected:
	ConnectionType type;
	ConnectionDirection direction;
	ServerConnectionType server_type;
	std::string label;
	pmink_utils::EventManager<ConnectionEventType, ConnectiontEventArgType, void*> e_handler;
	// master connection in  case of one to many server connection
	ConnectionBase* master_conn;
	pmink::Atomic<int> master_ref;
	ConnectionLogic* logic;
	int client_socket;
	int server_socket;

    };

    // fwd
    class M3UAConnection;
    class SMPPConnection;
    template<typename PLD_TYPE> class DefaultConnectionLogic;
    template<typename PLD_TYPE> class _M3UAConnection;
    template<typename PLD_TYPE> class _PCAPConnection;
    template<typename PLD_TYPE> class PcapSmppFraming;


    class ApplicationServer{
    public:
	ApplicationServer(const char* _label);
	~ApplicationServer();

	const char* get_label();
	M3UAConnection* get_next_asp(bool* broadcast);
	pmink_utils::WRR<M3UAConnection*>* get_wrr();

	pmink_utils::VariantParamMap<ConnectionParamType> params;
	std::map<std::string, M3UAConnection*> asp_map;
	m3ua::TMTType traffic_mode;
	M3UAConnection* override_conn;
	pmink_utils::StatsManager stats;
	pmink_utils::Mutex wrr_mtx;



    private:
	void terminate();

	std::string label;
	pmink_utils::WRR<M3UAConnection*> wrr;
    };

    class SMPP_ApplicationServer{
    public:
	SMPP_ApplicationServer(const char* _label);
	~SMPP_ApplicationServer();

	const char* get_label();
	SMPPConnection* get_next_asp();
	pmink_utils::WRR<SMPPConnection*>* get_wrr();

	pmink_utils::VariantParamMap<ConnectionParamType> params;
	std::map<std::string, SMPPConnection*> asp_map;
	SMPPConnection* override_conn;
	pmink_utils::StatsManager stats;
	pmink_utils::Mutex wrr_mtx;



    private:
	void terminate();

	std::string label;
	pmink_utils::WRR<SMPPConnection*> wrr;
    };



    class SMPPUser{
    public:
	SMPPUser(): max_conns(0),
		    conns(0),
		    ton(smpp::TON_UNKNOWN),
		    npi(smpp::UNKNOWN),
		    interface_version(0x34){
	    bzero(system_id, sizeof(system_id));
	    bzero(password, sizeof(password));
	    // default system_type
	    strcpy(system_type, "SMPP");
	}
	SMPPUser(const char* _system_id,
		 const char* _password,
		 const char* _system_type,
		 int _max_conns,
		 smpp::SmppTON _ton = smpp::TON_UNKNOWN,
		 smpp::SmppNPI _npi = smpp::UNKNOWN,
		 int _interface_version = 0x34):
	    max_conns(_max_conns),
	    conns(0),
	    ton(_ton),
	    npi(_npi),
	    interface_version(_interface_version){
	    // default system_type
	    strcpy(system_type, "SMPP");

	    if(_system_id != NULL) strncpy(system_id, _system_id, sizeof(system_id));
	    if(_password != NULL) strncpy(password, _password, sizeof(password));
	    if(_system_type != NULL) strncpy(system_type, _system_type, sizeof(system_type));

	}
	int max_conns;
	int conns;
	char system_id[16];
	char password[9];
	char system_type[13];
	smpp::SmppTON ton;
	smpp::SmppNPI npi;
	int interface_version;
    };


    template<typename PLD_TYPE>
    class Connection: public ConnectionBase{
	friend class DefaultConnectionLogic<PLD_TYPE>;
    public:
	Connection(ConnectionType _type,
		   ConnectionDirection _direction,
		   ServerConnectionType _server_type,
		   const char* _label,
		   int _max_mtu = 8192,
		   int _pool_size = 4096);



	~Connection();

	int start();
	int stop();
	bool is_garbage();

	// outer pool
	memory::Pool<PminkPayload, true> in_pool;
	// outer queue
	pmink::RingBuffer<PminkPayload*> in_q;
	memory::SpscQPool<PLD_TYPE>* get_pld_pool();


    protected:
	memory::HeapChunkBuffer raw_buff;
	pmink_utils::ThreadManager thm;
	int pool_size;
	int max_mtu;
	int max_in_connc;
	int reconnect_timer;
	time_t reconnect_ts;
	pmink::Atomic<bool> reconnect_queued;
	pmink::Atomic<bool> reconnecting;
	memory::SpscQPool<PLD_TYPE> pld_pool;
	pmink::Atomic<bool> cthreads_active;

	// lock free inner spsc queues
	lockfree::SpscQ<PLD_TYPE> rx_tx_q;
	lockfree::SpscQ<PLD_TYPE> rx_proc_q;
	lockfree::SpscQ<Payload> proc_tx_q;

	virtual int data_rx(unsigned char* buff, int size, int* bytes_rvcd, int* pld_type, void* extra = NULL) = 0;
	virtual int data_tx(Payload* data) = 0;
	virtual int data_rx_process(unsigned char* buff, int size, int data_type, void* extra = NULL) = 0;
	virtual int proc_process(PLD_TYPE* pld) = 0;
	virtual int proc_process_ext(PminkPayload* pmpld) = 0;
	virtual int server_get_client(int _server_socket) = 0;
	virtual int create_client(int _client_socke, ConnectionBase** new_conn = NULL);
	virtual int rx_on_start();
	virtual int tx_on_start();
	virtual int timer_event();
	virtual int reconnect() = 0;
	virtual int convert(PminkPayload* pmpld) = 0;
	virtual int cmd_process(PminkPayload* pmpld, int cmd_id = -1) = 0;

	int rx_logic(int poll_res, short int poll_revents, int* poll_socket);
	int tx_logic();
	int proc_logic();
	int server_logic(int poll_res, short int poll_revents, int* poll_socket, ConnectionBase** newconn);
	int reconnect_logic();
	virtual int extra_logic();
	virtual int pcap_logic();

    };

    template<typename PLD_TYPE>
    class DefaultConnectionLogic: public ConnectionLogic{
    public:
	DefaultConnectionLogic(Connection<PLD_TYPE>* _conn);
	int run(int phase, void* args);

    protected:
	 Connection<PLD_TYPE>* conn;
    };


    template<typename PLD_TYPE>
    class _TCPConnection: public Connection<PLD_TYPE> {
    public:
	_TCPConnection(ConnectionDirection _direction,
		       ServerConnectionType _server_type,
   		       const char* _label,
   		       int _max_mtu = 8192,
   		       int _pool_size = 4096);

	~_TCPConnection();

	int start();
	int stop();
	int server_get_client(int _server_socket);
	int init_tcp_client_bind(uint32_t remote_addr,
				 uint32_t local_addr,
				 uint16_t local_port,
				 uint16_t remote_port);

	int init_tcp_server(uint32_t local_addr, uint16_t local_port);
	int shutdown_tcp_client(int connSock);
	int get_client(int _server, sockaddr_in* sci);


    protected:

	int data_rx(unsigned char* buff, int size, int* bytes_rvcd, int* pld_type, void* extra);
	int data_tx(Payload* data);
	int data_rx_process(unsigned char* buff, int size, int data_type, void* extra);
	int proc_process(PLD_TYPE* pld);

	int reconnect();



    };

    class TCPConnection: public _TCPConnection<TcpPayload>{
    public:
	TCPConnection(	ConnectionDirection _direction,
			ServerConnectionType _server_type,
   		        const char* _label,
			int _max_mtu = 8192,
			int _pool_size = 4096);
    };



    template<typename PLD_TYPE>
    class _SCTPConnection: public Connection<PLD_TYPE> {
    public:
	_SCTPConnection(ConnectionDirection _direction,
			ServerConnectionType _server_type,
   		        const char* _label,
			int _max_mtu = 8192,
			int _pool_size = 4096);

	~_SCTPConnection();

	int start();
	int stop();
	int server_get_client(int _server_socket);


    protected:

	int data_rx(unsigned char* buff, int size, int* bytes_rvcd, int* pld_type, void* extra);
	int data_tx(Payload* data);
	int data_rx_process(unsigned char* buff, int size, int data_type, void* extra);
	int proc_process(PLD_TYPE* pld);

	int reconnect();



    };




    class SCTPConnection: public _SCTPConnection<SctpPayload>{
    public:
	SCTPConnection(	ConnectionDirection _direction,
			ServerConnectionType _server_type,
   		        const char* _label,
			int _max_mtu = 8192,
			int _pool_size = 4096);
    };


    class ServiceMessageAsyncDone: public r14p::R14PCallbackMethod {
    public:
	void run(r14p::R14PCallbackArgs* args);
    };

    template<typename PLD_TYPE>
    class _SMPPConnection: public _TCPConnection<PLD_TYPE> {
	friend class _PCAPConnection<PcapPayload>;
        friend class PcapSmppFraming<PcapPayload>;
    public:
	_SMPPConnection(ConnectionDirection _direction,
			ServerConnectionType _server_type,
   		        const char* _label,
			int _max_mtu = 8192,
			int _pool_size = 4096);

	~_SMPPConnection();
	bool refc_condition();
	int start();
	int stop();
	int smpp_connect();
	void terminate();
	void stop_children();

	pmink::Atomic<SMPP_ApplicationServer*> as;

	pmink::Atomic<bool> smpp_active;
	// bind user map
	pmink_utils::Mutex usr_map_mtx;
	std::map<std::string, SMPPUser> usr_map;
	// client connections when in one to many server mode
	pmink_utils::WRR<SMPPConnection*> clients;
	// current user
	SMPPUser smpp_user;
	int bind_dec(const char* _sustem_id);
	ConnectionBase* get_conn(const char* id = NULL, bool inc_refc = false);

    protected:
	int data_rx(unsigned char* buff, int size, int* bytes_rvcd, int* pld_type, void* extra);
	int data_rx_process(unsigned char* buff, int size, int data_type, void* extra);
	int proc_process(PLD_TYPE* pld);
	int proc_process_ext(PminkPayload* pmpld);
	int process_dr(PLD_TYPE* pld);
	int process_stp(PLD_TYPE* pld);
	int reconnect();
	int rx_on_start();
	void pmpld_stp(PminkPayload* pmpld);
	void pmpld_dr(PminkPayload* pmpld);
	void pmpld_notify(PminkPayload* _pmpld);
	int timer_event();
	int create_client(int _client_socket, ConnectionBase** new_conn);
	int convert(PminkPayload* pmpld);
	int cmd_process(PminkPayload* pmpld, int cmd_id = -1);
	int extra_logic();

    private:
	void process_smpp(smpp::SmppPDU* pdu, PminkPayload* pmpld);
	bool bind_auth(const char* system_id, const char* password);

	ServiceMessageAsyncDone smsg_async_done;

	// enquire link
	time_t enql_last_ts;
	smpp::SmppPDU enql_smppp;
	int enql_ts;
	bool enql_enabled;

	// smpp pools
	smpp::SMPPPool smpp_rx_pool;
	smpp::SMPPPool smpp_proc_pool;
	smpp::DlvrRcpt smpp_dlvr_rcpt_proc;

	// utf-16 be converter
	UConverter* conv_utf16be;

	// iconv converters
	iconv_t iconv_utf8_to_gsm7;
	iconv_t iconv_gsm7_to_utf8;


	// tp-ud
	smstpdu::TP_UserData tp_ud;
	// data coding
	smstpdu::TP_DCSDataCoding tp_dcs_dcoding;

	// connection phase
	uint32_t conn_phase;

	// dr flag
	bool retain_data;

	r14p::ServiceMsgManager* r14psmm;
	r14p::R14PSession* r14ps;
	r14p::R14PClient* dr_r14pc;
	r14p::R14PClient* stp_r14pc;
	r14p::R14PClient* rt_r14pc;

	// timestamps
	time_t bind_ts;
	int bind_timeout;

	// termianted flag
	bool terminated;

	// tcp separation/buffering
	unsigned char sep_buff[65535];
	unsigned char* sep_buff_p;
	unsigned int sep_buff_l;
	// next pdu length
	unsigned int next_pdu_l;
	// next pdu received bytes
	unsigned int next_pdu_rb;

	// sequence numbers
	pmink::Atomic<uint32_t> out_seq_num;

	// message counter
	pmink::Atomic<uint64_t> sm_num;

	// enq link difff
	pmink::Atomic<int> enql_diff;






    };

    class SMPPConnection: public _SMPPConnection<SmppPayload>{
    public:
	SMPPConnection(	ConnectionDirection _direction,
			ServerConnectionType _server_type,
   		        const char* _label,
			int _max_mtu = 8192,
			int _pool_size = 4096);
    };


    class M3UASendErrorHandler: public pmink_utils::EventHandler<ConnectiontEventArgType, void*>{
    public:
	M3UASendErrorHandler(_M3UAConnection<M3uaPayload>* m3ua_conn);
	void run(pmink_utils::EventArgs<ConnectiontEventArgType, void*>& args);

	_M3UAConnection<M3uaPayload>* m3ua_conn;

    };

    template<typename PLD_TYPE>
    class _M3UAConnection: public _SCTPConnection<PLD_TYPE> {
	friend class M3UASendErrorHandler;
	friend class _PCAPConnection<PcapPayload>;
    public:
	typedef typename pmink_utils::CorrelationMap<uint32_t, uint32_t>::cmap_it_type loop_cmap_it_t;

	_M3UAConnection(ConnectionDirection _direction,
			ServerConnectionType _server_type,
   		        const char* _label,
			int _max_mtu = 8192,
			int _pool_size = 4096);

	~_M3UAConnection();


	int start();
	int stop();
	int m3ua_connect();
	void terminate();

	pmink::Atomic<bool> m3ua_active;
	pmink::Atomic<ApplicationServer*> as;



    protected:

	int data_rx_process(unsigned char* buff, int size, int data_type, void* extra);

	int proc_process(PLD_TYPE* pld);
	int proc_process_ext(PminkPayload* pmpld);
	int process_dr(PLD_TYPE* pld);
	int process_stp(PLD_TYPE* pld);
	int rx_on_start();
	void pmpld_dr(PminkPayload* pmpld);
	void pmpld_stp(PminkPayload* pmpld);
	void pmpld_notify(PminkPayload* _pmpld);
	int reconnect();

	int timer_event();
	int convert(PminkPayload* pmpld);
	int cmd_process(PminkPayload* pmpld, int cmd_id = -1);
	int extra_logic();



    private:
	static void* hbeat_loop(void* args);
	void process_m3ua(m3ua::ProtocolData* pdata, PminkPayload* pmpld);
	void process_gt(sccp::GlobalTitle* _gt, PminkPayload* pmpld, uint32_t pcode);
	void process_sccp(unsigned char* buff, int size, PminkPayload* pmpld);
	void translate_called_pa(sccp::CalledPA* cpa, PminkPayload* pmpld, sccp::SCCPPool* _sccp_pool);
	void translate_calling_pa(sccp::CallingPA* cpa, PminkPayload* pmpld, sccp::SCCPPool* _sccp_pool);
	void process_called_pa(sccp::CalledPA* cpa, PminkPayload* pmpld);
	void process_calling_pa(sccp::CallingPA* cpa, PminkPayload* pmpld);
	void process_tcap(unsigned char* buff, int size, PminkPayload* pmpld);
	void process_gsmmap(unsigned char* buff, int size, PminkPayload* pmpld, int component_type, int opcode, int component_index);
	void process_tcap_components(asn1::itu_tcap::ComponentPortion* components, PminkPayload* pmpld, uint64_t sess_id);
	void process_dialogue(asn1::DialoguePDU* dialogue, PminkPayload* pmpld, uint64_t sess_id);
	void process_smstpdu(unsigned char* buff, int size, PminkPayload* pmpld, int component_index, smstpdu::TPDUDirectionType tpdu_direction);
	bool context_supported(const char* ctx_oid);

	ServiceMessageAsyncDone smsg_async_done;

	// hbeat
	time_t hbeat_last_ts;
	m3ua::M3UAPacket hbeat_m3uap;
	int hbeat_ts;
	bool hbeat_enabled;

	// m3ua pools
	m3ua::M3UAPool m3ua_rx_pool;
	m3ua::M3UAPool m3ua_proc_pool;

	// connection phase
	uint32_t conn_phase;

        // AS status info
        m3ua::StatusInfoType as_status_info;

	// opc, dpc
	uint32_t opc;
	uint32_t dpc;

	// hbeat
	pmink::Atomic<uint32_t> beat_miss;
	pmink::Atomic<int> beat_diff;
	uint32_t beat_data;

	// sccp
	sccp::SCCPPool sccp_pool;
	// ber
	asn1::ASN1Pool asn1_pool;

	// utf-8 converter
	UConverter* conv_utf8;

	// iconv converters
	iconv_t iconv_utf8_to_gsm7;
	iconv_t iconv_gsm7_to_utf8;

	// random generator
	boost::random::mt19937 rng;
	boost::random::uniform_int_distribution<uint32_t> rng_dstr;

	// tcap
	asn1::itu_tcap::TCMessage tcm;
	asn1::itu_tcap::TCMessage tcm_enc;
	asn1::ASN1Node root_tcap_node;
	asn1::ASN1Node root_tcap_dialogue_node;
	asn1::DialoguePDU dialogue_pdu;
	asn1::DialoguePDU dialogue_pdu_enc;
	asn1::gsmmap::ErrorCode gsmmap_error_code_enc;
	// gsm map
	asn1::ASN1Node root_gsmmap_node;
	asn1::ASN1Node root_gsmmap_err_node;
	asn1::MT_ForwardSM_Arg mt_fwd_sm;
	asn1::MT_ForwardSM_Arg mt_fwd_sm_enc;
	asn1::MO_ForwardSM_Arg mo_fwd_sm;
	asn1::RoutingInfoForSM_Arg sri_sm_arg;
	asn1::RoutingInfoForSM_Arg sri_sm_arg_enc;
	asn1::RoutingInfoForSM_Res sri_sm_res;
	asn1::RoutingInfoForSM_Res sri_sm_res_enc;
	asn1::gsmmap::ErrorCode gsmmap_error_code;
	// smstpdu
	smstpdu::SMS_Submit sms_submit;
	smstpdu::SMS_Deliver sms_deliver;


	// 0 - tcap session
	// 1 - gsm map mo-forward session
	// 2 - gsm map mt-forward session
	// 3 - gsm map error session
	// 4 - tcap dialogue pdu
	// 5 - gsm map sri-sm-arg session
	// 6 - gsm map sri-sm-res session
	asn1::SessionId asn1_session_id[7];

	// decoder flags for fast access
	// to avoid frequent traversal of parameter map
	bool decoders[6];

	// dr flag
	bool retain_data;

	r14p::ServiceMsgManager* r14psmm;
	r14p::R14PSession* r14ps;
	r14p::R14PClient* dr_r14pc;
	r14p::R14PClient* stp_r14pc;
	r14p::R14PClient* rt_r14pc;

	// timestamps
	time_t aspup_ts;
	int aspup_timeout;

	// events
	//RouterTerminated rt_term;
	M3UASendErrorHandler send_e_handler;

	// termianted flag
	bool terminated;

	// last DATA sid
	pmink::Atomic<uint16_t> last_sid;

	// loop protection  map
	pmink_utils::CorrelationMap<uint32_t, uint32_t>* loop_cmap;



    };


    class M3UAConnection: public _M3UAConnection<M3uaPayload>{
    public:
	M3UAConnection(	ConnectionDirection _direction,
			ServerConnectionType _server_type,
   		        const char* _label,
			int _max_mtu = 8192,
			int _pool_size = 4096);
    };

    template<typename PLD_TYPE>
    class PcapSmppFraming: public smpp::TcpFraming {
    public:
        PcapSmppFraming(_PCAPConnection<PLD_TYPE>* _conn);
        int on_pdu_ready(unsigned char* pdu, unsigned int pdu_size, void* args);

    private:
        _PCAPConnection<PLD_TYPE>* conn;
    };

    template<typename PLD_TYPE>
    class _PCAPConnection: public Connection<PLD_TYPE> {
        friend class PcapSmppFraming<PLD_TYPE>;
    public:
	typedef lockfree::QItem<packet_descriptor, 0> packet_pitem_t;
        typedef PcapSmppFraming<PLD_TYPE> pcap_smpp_frm_t;
        enum PcapType {
           PT_M3UA       = 0,
           PT_SMPP       = 1,
           PT_M3UA_SMPP  = 2
        };

	_PCAPConnection(ConnectionDirection _direction,
		        ServerConnectionType _server_type,
   		        const char* _label,
   		        int _max_mtu = 8192,
   		        int _pool_size = 4096);

	~_PCAPConnection();

	int start();
	int stop();
	void terminate();
	M3UAConnection* get_m3ua_conn();
        SMPPConnection* get_smpp_conn();
        pmink_net::TcpTracker* get_tcp_tracker();

	pd_alloc_t pcap_alloc_p;
	pd_dealloc_t pcap_dealloc_p;
	pd_start_t pcap_start_p;
	pd_finish_t pcap_finish_p;

    private:
        void free_tcp_segm_args(pmink_net::TcpTracker::args_vec_t* lst);
        void free_tcp_strm_args(pmink_net::TcpTracker::args_vec_t* lst);
        void tcp_conn_expire(pmink_net::TcpConnection* tcp_conn);
        void tcp_expire_finished();


	// termianted flag
	bool terminated;

        // pcap type (m3ua, smpp or both)
        int pcap_type;

	// m3ua
	M3UAConnection* m3ua_conn;
	stream_descriptor* sd;

        // smpp
        SMPPConnection* smpp_conn;


	// protocol headers and pools
	tcpip::ETHHeader ethh;
	tcpip::VLANHeader vlanh;
	tcpip::IPHeader iph;
        pmink_net::TcpHeader tcph;
	sctp::SCTPPacket sctpp;
	sctp::ChunkPool sctp_chunk_pool;

        // tcp
        pmink_net::TcpTracker tcp_tracker;
	unsigned char reasm_buff[65535];
	unsigned int reasm_buff_l;
        pmink_net::TcpTracker::args_vec_t segm_args_lst;
        pmink_net::TcpTracker::args_vec_t strm_args_lst;
        time_t tcp_exp_ts;

    protected:
	int data_rx(unsigned char* buff, int size, int* bytes_rvcd, int* pld_type, void* extra);
	int data_tx(Payload* data);
	int data_rx_process(unsigned char* buff, int size, int data_type, void* extra);
	int proc_process(PLD_TYPE* pld);
	int proc_process_ext(PminkPayload* pmpld);
	int server_get_client(int _server_socket);
	int reconnect();
	int convert(PminkPayload* pmpld);
	int cmd_process(PminkPayload* pmpld, int cmd_id);
	int pcap_logic();

    };

    class PCAPConnection: public _PCAPConnection<PcapPayload>{
    public:
	PCAPConnection(	ConnectionDirection _direction,
			ServerConnectionType _server_type,
   		        const char* _label,
			int _max_mtu = 8192,
			int _pool_size = 4096);
    };

}



#endif /* CONNECTION_H_ */
