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

#ifndef TCP_IP_H_
#define TCP_IP_H_

#include <map>
#include <vector>
#include <inttypes.h>
#include <boost/unordered_map.hpp>
//#include <boost/pool/pool_alloc.hpp>
#include <pmpcap.h>
#include <atomic.h>
#include <ext/mt_allocator.h>


namespace tcpip {
	enum IPProtocolType {
		IP_PT_TCP 	= 0x06,
		IP_PT_UDP 	= 0x11,
		IP_PT_SCTP 	= 0x84,
		IP_PT_IPv6 	= 0x29,
		IP_PT_IPv4 	= 0x04,
		IP_PT_ICMP	= 0x01

	};

	enum EtherType {
		ETH_ET_IPv4 	= 0x0800,
		ETH_ET_IPv6 	= 0x86DD,
		ETH_ET_VLAN 	= 0x8100

	};
	enum VLANPriority {
		VLP_BACKGROUND 			= 0x20,
		VLP_BEST_EFFORT 		= 0x00,
		VLP_EXCELLENT_EFFORT 		= 0x40,
		VLP_CRITICAL_APPLICATIONS 	= 0x60,
		VLP_VIDEO_100MS_LATENCY 	= 0x80,
		VLP_VOICE_10MS_LATENCY 		= 0xa0,
		VLP_INTERNETWORK_CONTROL 	= 0xc0,
		VLP_NETWORK_CONTROL 		= 0xe0
	};


	enum L7ProtocolType {
	    L7_PT_ANY		= 9999,
	    L7_PT_UNKNOWN	= -1,
	    L7_PT_POP		= 0,
	    L7_PT_SMTP		= 1,
	    L7_PT_IMAP		= 2,
	    L7_PT_RADIUS	= 3,
	    L7_PT_DHCP		= 4,
	    L7_PT_IMF		= 5,
	    L7_PT_LAYER2	= 6,
	    L7_PT_LAYER3	= 7,
	    L7_PT_IP_PORT	= 8,
	    L7_PT_NETFLOW_9	= 9

	};

	enum OSILayerType {
	    // Unknown
	    OSI_LT_UNKNOWN	= 0,

	    // Layer 2
	    OSI_LT_802_3 	= 100,

	    // Layer 3
	    OSI_LT_IPV4		= 200,
	    OSI_LT_IPV6		= 201,

	    // Layer 4
	    OSI_LT_TCP		= 300,
	    OSI_LT_UDP		= 301,
	    OSI_LT_SCTP		= 302


	};

	class Layer7Classifier{
	public:
	    Layer7Classifier();

	    L7ProtocolType classify(		OSILayerType osi_l4_type,
						unsigned char* data,
						unsigned int data_length,
						unsigned int src_port,
						unsigned int dst_port);



	private:
	    static L7ProtocolType email_pop3(OSILayerType osi_l4_type, unsigned char* data, unsigned int data_length);
	    static L7ProtocolType email_smtp(OSILayerType osi_l4_type, unsigned char* data, unsigned int data_length);
	    static L7ProtocolType radius(OSILayerType osi_l4_type, unsigned char* data, unsigned int data_length);
	    static L7ProtocolType dhcp(OSILayerType osi_l4_type, unsigned char* data, unsigned int data_length);
	    typedef L7ProtocolType (*Clsf_method)(OSILayerType osi_l4_type, unsigned char*, unsigned int);
	    typedef std::map<uint16_t, Clsf_method>::iterator clsf_it;
	    std::map<uint16_t, Clsf_method> portmap;

	};




	class OSILayer {
	public:
	    unsigned int layer_number;
	    OSILayerType layer_type;
	    OSILayer* layer_up;
	    OSILayer* layer_down;
	    // raw header pointer in packet buffer
	    unsigned char* buff_hdr_p;
	    unsigned int buff_hdr_size;
	    void* packet;
	    void* args;
	    void* self;
	    time_t timestamp;

	};


	class ETHHeader : public OSILayer {
	public:
		unsigned int destination_length;
		unsigned char* destination;
		unsigned int source_length;
		unsigned char* source;
		unsigned int payload_length;
		unsigned char* payload;
		EtherType ether_type;
		// raw header pointer in packet buffer

	};

	class VLANHeader : public OSILayer {
	public:
		EtherType ether_type;
		VLANPriority priority;
		bool cfi_indicator;
		unsigned int vlan_identifier;
		unsigned int payload_length;
		unsigned char* payload;
	};

	class SLLHeader : public OSILayer {
	public:
		int packet_type;
		int address_type;
		int address_length;
		int source_length;
		unsigned char* source;
		int protocol;
		int payload_length;
		unsigned char* payload;
	};


	class IPHeader : public OSILayer {
	public:
		IPHeader();
		~IPHeader();
		int reassemble(unsigned char* buffer, unsigned int max_buffer_size);

		int vesion;
		int header_length;
		int total_length;
		IPProtocolType protocol;
		uint16_t identification;
		int fragment_offset;
		bool do_not_fragment;
		bool more_fragments;
		int time_to_live;
		int checksum_length;
		unsigned char* checksum;
		int source_length;
		unsigned char* source;
		int destination_length;
		unsigned char* destination;
		int payload_length;
		unsigned char* payload;
		// fragmentation related
		std::vector<IPHeader*> fragments;
		unsigned int total_payload_length;
		void* ethh;
		// ipv6 related
		uint8_t traffic_class;
		uint32_t flow_label;
		uint8_t hop_limit;





	};

	class TCPHeader : public OSILayer {
	public:
		//TCPHeader();
		uint16_t source;
		uint16_t destination;
		uint32_t sequence_number;
		uint32_t ack_number;
		unsigned int header_length;
		// flags
		bool ns;
		bool cwr;
		bool ece;
		bool urg;
		bool ack;
		bool psh;
		bool rst;
		bool syn;
		bool fin;
		uint16_t window_size;
		uint16_t checksum;
		uint16_t urg_pointer;
		// payload
		unsigned int payload_length;
		unsigned char* payload;
		// pointer to IP header
		void* iph;
		//bool used;


	};

	class UDPHeader : public OSILayer {
	public:
		uint16_t source;
		uint16_t destination;
		uint16_t length;
		uint16_t checksum;
		// payload
		unsigned int payload_length;
		unsigned char* payload;
		// pointer to IP header
		void* iph;
		// raw header pointer in packet buffer
	};

	class IPFragId {
	public:
	    IPFragId();
	    // source ip (16 bytes max for ipv6)
	    unsigned char source_ip[16];
	    // size of source ip (ipv4 or ipv6)
	    uint16_t source_ip_size;
	    // dest ip (16 bytes max for ipv6)
	    unsigned char dest_ip[16];
	    // size of dest ip (ipv4 or ipv6)
	    uint16_t dest_ip_size;
	    // frag id
	    uint16_t identifiction;

	    bool operator ==(const IPFragId& right) const;
	    bool operator !=(const IPFragId& right) const;

	};

	class IPFragIdHash {
	public:
	    std::size_t operator() (const IPFragId& e) const;

	};


	class IPDefrag {
	public:
	    IPDefrag();
	    virtual ~IPDefrag();
	    int process(IPHeader* packet, IPHeader** result);

	    void process_timeout(unsigned int timeout);
	    virtual int free_frag(IPHeader* iph, bool free_main = true);

	protected:
	    //virtual void lock();
	    //virtual void unlock();
	    typedef __gnu_cxx::__mt_alloc<std::pair<const uint16_t, IPHeader*> > frag_allocator_t;

	    /*
	    typedef std::map<	uint16_t,
				IPHeader*,
				std::less<uint16_t>,
				boost::fast_pool_allocator<	std::pair<uint16_t, IPHeader* >,
								boost::default_user_allocator_new_delete,
								boost::details::pool::null_mutex,
								100000 > > frag_map_type;
								*/

	    typedef boost::unordered_map<	IPFragId,
						IPHeader*,
						IPFragIdHash,
						std::equal_to<IPFragId>,
						frag_allocator_t > frag_map_type;

	    /*
	    typedef std::map<	uint16_t,
				IPHeader*,
				std::less<uint16_t>,
				frag_allocator_t > frag_map_type;
				*/



	    typedef frag_map_type::iterator frag_map_it;

	    frag_map_type frag_lst;



	};


	class TCP_UDPStreamId {
	public:
	    TCP_UDPStreamId();
	    // source ip (16 bytes max for ipv6)
	    unsigned char source_ip[16];
	    // size of source ip (ipv4 or ipv6)
	    uint16_t source_ip_size;
	    // dest ip (16 bytes max for ipv6)
	    unsigned char dest_ip[16];
	    // size of dest ip (ipv4 or ipv6)
	    uint16_t dest_ip_size;
	    // source port (2 bytes)
	    uint16_t source_port;
	    // dest port (2 bytes)
	    uint16_t dest_port;

	    bool operator ==(const TCP_UDPStreamId& right) const;
	    bool operator !=(const TCP_UDPStreamId& right) const;

	};

	std::ostream& operator<< (std::ostream& out, const TCP_UDPStreamId& sid);


	class TCP_UDPStreamIdHash {
	public:
	    std::size_t operator() (const TCP_UDPStreamId& e) const;

	};


	/*
	class TCPStreamIdCompare {
	public:
	    bool operator ()(const TCPStreamId& x, const TCPStreamId& y);
	};
	*/

	// fwd declaration
	class TCPConnection;


	class TCPStream {
	public:
	    TCPStream();
	    ~TCPStream();

	    // next buffered data segment (ended with PSH)
	    int next_chunk(unsigned char* buffer, unsigned int max_buffer_length, bool* more, bool *retransmission);

	    void set_reversed(bool _is_reversed);
	    void add_segment(TCPHeader* tcph);
	    void set_connection(TCPConnection* conn);
	    void clear_processed();

	    // last sequence number
	    uint32_t last_seq_num;
	    // initial sequence number
	    uint32_t isn;
	    // ISN set flag
	    bool isn_set;
	    // reversed stream flag
	    bool reversed;
	    // finished
	    bool finished;
	    // chunks queue
	    unsigned int chunk_q;
	    // tcp connection
	    TCPConnection* connection;
	    // segments
	    std::vector<TCPHeader*> segments;
	    // segment byte count
	    uint32_t seg_byte_count;
	    // offsets
	    std::vector<uint32_t> offsets;
	    // next segment index
	    unsigned int next_segment;
	    // octet count
	    uint64_t byte_count;

	    void reset();

	};

	class TCPConnection {
	public:
	    TCPConnection();
	    // tcp stream id
	    TCP_UDPStreamId stream_id;
	    uint32_t stream_hash;
	    // streams
	    TCPStream out_stream;
	    TCPStream in_stream;
	    // current active streamm, set by tcp reassembly
	    TCPStream* current_stream;
	    // finished flag
	    bool finished;
	    // layer 7 protocol type
	    L7ProtocolType l7_ptype;
	    // number of classification attempts
	    unsigned int clsf_count;
	    // timestamp
	    time_t timestamp;
	    // general param
	    void* args;


	};

	class DhcpSessionId {
	public:
	    DhcpSessionId();
	    // username
	    unsigned char username[100];
	    int username_size;

	    bool operator ==(const DhcpSessionId& right) const;
	    bool operator !=(const DhcpSessionId& right) const;


	};

	class DhcpSessionIdHash {
	public:
	    std::size_t operator() (const DhcpSessionId &e) const;

	};


	class DhcpSession {
	public:
	    DhcpSession();
	    // session id
	    DhcpSessionId session_id;
	    // timestamp
	    time_t timestamp;
	    // expiration timestamo
	    pmink::Atomic<time_t> exp_timestamp;
	    // general param
	    void* args;
	    // self
	    void* self;
	    // finished
	    pmink::Atomic<uint8_t> finished;


	};

	class DhcpSessionManager {
	private:
	    pthread_mutex_t mtx;
	public:
	    DhcpSessionManager();
	    virtual ~DhcpSessionManager();

	    DhcpSession* process(IPHeader* iph, UDPHeader* udph, bool* udph_used, void* mac, int mac_size, void* source);
	    int remove(DhcpSessionId* id);
	    virtual void remove_all();
	    void lock();
	    void unlock();

	    typedef __gnu_cxx::__mt_alloc<std::pair<const DhcpSessionId, DhcpSession*> > conn_allocator_t;

	    /*
	    typedef boost::unordered_map<	DhcpSessionId,
						DhcpSession*,
						DhcpSessionIdHash,
						std::equal_to<DhcpSessionId>,
						boost::fast_pool_allocator<	std::pair<DhcpSessionId, DhcpSession* >,
										boost::default_user_allocator_new_delete,
										boost::details::pool::null_mutex,
										100000 > > conn_lst_type;
										*/
	    typedef boost::unordered_map<	DhcpSessionId,
						DhcpSession*,
						DhcpSessionIdHash,
						std::equal_to<DhcpSessionId>,
						conn_allocator_t > conn_lst_type;

	    typedef conn_lst_type::iterator conn_lst_it;

	    virtual DhcpSession* alloc_conn(void* args = NULL);
	    virtual void free_conn(DhcpSession* conn);

	protected:

	    conn_lst_type conn_lst;


	};




	class RadiusSessionId {
	public:
	    RadiusSessionId();
	    // username
	    char username[100];

	    bool operator ==(const RadiusSessionId& right) const;
	    bool operator !=(const RadiusSessionId& right) const;


	};

	class RadiusSessionIdHash {
	public:
	    std::size_t operator() (const RadiusSessionId &e) const;

	};



	class RadiusSession {
	public:
	    RadiusSession();
	    // session id
	    RadiusSessionId session_id;
	    // timestamp
	    time_t timestamp;
	    // general param
	    void* args;
	    // self
	    void* self;
	    // finished
	    pmink::Atomic<uint8_t> finished;
	    // cleanup
	    pmink::Atomic<uint8_t> cleanup;


	};


	class RadiusSessionManager {
	private:
	    pthread_mutex_t mtx;

	public:
	    RadiusSessionManager();
	    virtual ~RadiusSessionManager();

	    RadiusSession* process(IPHeader* iph, UDPHeader* udph, bool* udph_used, void* username, int username_size, void* source);
	    int remove(RadiusSessionId* id);
	    virtual void remove_all();
	    void lock();
	    void unlock();
	    //void reg_usr_ip(IPHeader* iph, RadiusSessionId* session);
	    //int unreg_usr_ip(RadiusSessionId* session);

	    typedef __gnu_cxx::__mt_alloc<std::pair<const RadiusSessionId, RadiusSession*> > conn_allocator_t;
	    //typedef __gnu_cxx::__mt_alloc<std::pair<const __uint128_t, RadiusSessionId> > usr_ip_allocator_t;

	    /*
	    typedef boost::unordered_map<	RadiusSessionId,
						RadiusSession*,
						RadiusSessionIdHash,
						std::equal_to<RadiusSessionId>,
						boost::fast_pool_allocator<	std::pair<RadiusSessionId, RadiusSession* >,
										boost::default_user_allocator_new_delete,
										boost::details::pool::null_mutex,
										100000 > > conn_lst_type;
										*/
	    typedef boost::unordered_map<	RadiusSessionId,
						RadiusSession*,
						RadiusSessionIdHash,
						std::equal_to<RadiusSessionId>,
						conn_allocator_t > conn_lst_type;

	    typedef conn_lst_type::iterator conn_lst_it;


	    //typedef std::map<__uint128_t, RadiusSessionId, std::less<__uint128_t>, usr_ip_allocator_t> ip_usr_map_type;
	    //typedef ip_usr_map_type::iterator ip_usr_map_it;

	    virtual RadiusSession* alloc_conn(void* args = NULL);
	    virtual void free_conn(RadiusSession* conn);

	protected:

	    conn_lst_type conn_lst;
	    //ip_usr_map_type usr_ip_lst;


	};



	class DhcpReqRespId {
	public:
	    DhcpReqRespId();
	    // udp stream id
	    TCP_UDPStreamId stream_id;
	    // xid (transaction id)
	    uint32_t transaction_id;

	    bool operator ==(const DhcpReqRespId& right) const;
	    bool operator !=(const DhcpReqRespId& right) const;


	};

	class DhcpReqRespIdHash {
	public:
	    std::size_t operator() (const DhcpReqRespId &e) const;

	};

	class DhcpReqRespCorrelation {
	public:
	    DhcpReqRespCorrelation();
	    // req-resp id
	    DhcpReqRespId packet_session_id;
	    // timestamp
	    time_t timestamp;
	    // general param
	    void* args;
	    // finished
	    bool finished;
	    // correlated with response
	    bool correlated;
	    // session pointer
	    DhcpSession* user_session;

	};

	class DhcpReqRespCorrelationManager {
	public:
	    DhcpReqRespCorrelationManager();
	    virtual ~DhcpReqRespCorrelationManager();

	    DhcpReqRespCorrelation* process(IPHeader* iph, UDPHeader* udph, bool* udph_used, uint32_t transaction_id, bool is_req, bool is_correlated);
	    int remove(DhcpReqRespId* id);
	    virtual void remove_all();

	    typedef __gnu_cxx::__mt_alloc<std::pair<const DhcpReqRespId, DhcpReqRespCorrelation*> > conn_allocator_t;

	    /*
	    typedef boost::unordered_map<	DhcpReqRespId,
						DhcpReqRespCorrelation*,
						DhcpReqRespIdHash,
						std::equal_to<DhcpReqRespId>,
						boost::fast_pool_allocator<	std::pair<DhcpReqRespId, DhcpReqRespCorrelation* >,
										boost::default_user_allocator_new_delete,
										boost::details::pool::null_mutex,
										100000 > > conn_lst_type;
										*/
	    typedef boost::unordered_map<	DhcpReqRespId,
						DhcpReqRespCorrelation*,
						DhcpReqRespIdHash,
						std::equal_to<DhcpReqRespId>,
						conn_allocator_t > conn_lst_type;

	    typedef conn_lst_type::iterator conn_lst_it;

	    void process_timeout(unsigned int timeout);

	protected:
	    virtual DhcpReqRespCorrelation* alloc_conn(void* args = NULL);
	    virtual void free_conn(DhcpReqRespCorrelation* conn);

	    conn_lst_type conn_lst;
	};






	class RadiusReqRespId {
	public:
	    RadiusReqRespId();
	    // udp stream id
	    TCP_UDPStreamId stream_id;
	    // radius packet identifier
	    uint8_t packet_id;

	    bool operator ==(const RadiusReqRespId& right) const;
	    bool operator !=(const RadiusReqRespId& right) const;



	};

	class RadiusReqRespIdHash {
	public:
	    std::size_t operator() (const RadiusReqRespId &e) const;

	};



	class RadiusReqRespCorrelation {
	public:
	    RadiusReqRespCorrelation();
	    // req-resp id
	    RadiusReqRespId packet_session_id;
	    // timestamp
	    time_t timestamp;
	    // general param
	    void* args;
	    // finished
	    bool finished;
	    // session pointer
	    RadiusSession* user_session;
	    // accounting
	    bool acct;
	    // acct status type
	    uint32_t acct_st;

	};



	class RadiusReqRespCorrelationManager {
	public:
	    RadiusReqRespCorrelationManager();
	    virtual ~RadiusReqRespCorrelationManager();

	    RadiusReqRespCorrelation* process(	IPHeader* iph,
						UDPHeader* udph,
						bool* udph_used,
						uint8_t packet_id,
						bool is_req,
						bool is_acct,
						uint32_t _acct_st);
	    int remove(RadiusReqRespId* id);
	    virtual void remove_all();

	    typedef __gnu_cxx::__mt_alloc<std::pair<const RadiusReqRespId, RadiusReqRespCorrelation*> > conn_allocator_t;

	    /*
	    typedef boost::unordered_map<	RadiusReqRespId,
						RadiusReqRespCorrelation*,
						RadiusReqRespIdHash,
						std::equal_to<RadiusReqRespId>,
						boost::fast_pool_allocator<	std::pair<RadiusReqRespId, RadiusReqRespCorrelation* >,
										boost::default_user_allocator_new_delete,
										boost::details::pool::null_mutex,
										100000 > > conn_lst_type;
										*/
	    typedef boost::unordered_map<	RadiusReqRespId,
						RadiusReqRespCorrelation*,
						RadiusReqRespIdHash,
						std::equal_to<RadiusReqRespId>,
						conn_allocator_t > conn_lst_type;

	    typedef conn_lst_type::iterator conn_lst_it;

	    void process_timeout(unsigned int timeout);

	protected:
	    virtual RadiusReqRespCorrelation* alloc_conn(void* args = NULL);
	    virtual void free_conn(RadiusReqRespCorrelation* conn);

	    conn_lst_type conn_lst;
	};




	class TCPReassembly {
	public:
	    TCPReassembly();
	    virtual ~TCPReassembly();

	    TCPConnection* process(IPHeader* iph, TCPHeader* tcph, bool* tcph_used);
	    //int remove(TCP_UDPStreamId* id);
	    int remove(uint32_t _hash);
	    virtual void remove_all();

	    //virtual void lock();
	    //virtual void unlock();

	    //typedef __gnu_cxx::__mt_alloc<std::pair<const TCP_UDPStreamId, TCPConnection*> > conn_allocator_t;
	    typedef __gnu_cxx::__mt_alloc<std::pair<const uint32_t, TCPConnection*> > conn_allocator_t;

	    /*
	    typedef boost::unordered_map<	TCP_UDPStreamId,
						TCPConnection*,
						TCP_UDPStreamIdHash,
						std::equal_to<TCP_UDPStreamId>,
						boost::fast_pool_allocator<	std::pair<TCP_UDPStreamId, TCPConnection* >,
										boost::default_user_allocator_new_delete,
										boost::details::pool::null_mutex,
										100000 > > conn_lst_type;
										*/
	    /*
	    typedef boost::unordered_map<	TCP_UDPStreamId,
						TCPConnection*,
						TCP_UDPStreamIdHash,
						std::equal_to<TCP_UDPStreamId>,
						conn_allocator_t > conn_lst_type;
						*/


	    /*
	    typedef boost::unordered_map<	uint32_t,
						TCPConnection*,
						boost::hash<uint32_t>,
						std::equal_to<uint32_t>,
						conn_allocator_t > conn_lst_type;
	    */

	    typedef std::map<uint32_t, TCPConnection*, std::less<uint32_t>, conn_allocator_t> conn_lst_type;


	    typedef conn_lst_type::iterator conn_lst_it;

	    conn_lst_type* get_conn_lst();

	protected:
	    virtual TCPConnection* alloc_conn(void* args = NULL);
	    virtual void free_conn(TCPConnection* conn);

	    //std::map<TCPStreamId, TCPConnection*, TCPStreamIdCompare> conn_lst;
	    //boost::unordered_map<TCPStreamId, TCPConnection*, TCPStreamIdHash> conn_lst;



	    conn_lst_type conn_lst;



	};


	// eth header
	ETHHeader* get_eth_header(unsigned char* data, int data_length);
	int get_eth_header(unsigned char* data, int data_length, ETHHeader* ethh);
	// sll header
	SLLHeader* get_sll_header(unsigned char* data, int data_length);
	// vlan header
	VLANHeader* get_vlan_header(unsigned char* data, int data_length);
	int get_vlan_header(unsigned char* data, int data_length, VLANHeader* vlanh);
	// tcp header
	TCPHeader* get_tcp_header(unsigned char* data, unsigned int data_length);
	int get_tcp_header(unsigned char* data, unsigned int data_length, TCPHeader* tcph);
	// udp header
	int get_udp_header(unsigned char* data, int data_length, UDPHeader* udph);

	// ip header
	IPHeader* get_ip_header(unsigned char* data, int data_length);
	int get_ip_header(unsigned char* data, int data_length, IPHeader* iph);

}


#endif /* TCP_IP_H_ */
