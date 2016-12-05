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

/**
 * @file r14p.h
 * @brief R14P library
 *
 */

#ifndef R14P_H_
#define R14P_H_

#include <vector>
#include <map>
#include <deque>
#include <pthread.h>
#include <semaphore.h>
#include <r14p_asn1.h>
#include <pool.h>
#include <atomic.h>
#include <ring.h>
#include <pmink_utils.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <poll.h>
#include <sctp.h>

namespace r14p {
	/**
	 * current R14P version
	 *
	 */
	const int _R14P_VERSION_ = 1;

	/**
	 *
	 * R14P Stream type
	 *
	 */
	enum R14PStreamType {
		R14P_ST_UNKNOWN 		= -1,	/**< Unknown stream */
		R14P_ST_STATEFUL 		= 0x00, /**< Stateful stream */
		R14P_ST_STATELESS 		= 0x01, /**< Stateless stream (single packet, no stream) */
		R14P_ST_STATELESS_NO_REPLY 	= 0x02, /**< Stateless stream without reply confirmation (single packet, no stream) */

	};

	/**
	 * R14P Stream initiator type
	 */
	enum R14PStreamInitiatorType{
	    R14P_SIT_LOCAL			= 0,
	    R14P_SIT_REMOTE			= 1
	};

	/**
	 * R14P Event type
	 *
	 */
	enum R14PEventType {
		R14P_ET_CLIENT_NEW 			= 0, 	/**< New client connection */
		R14P_ET_CLIENT_TERMINATED 		= 1, 	/**< Client terminated */
		R14P_ET_STREAM_NEW			= 2, 	/**< New stream starting */
		R14P_ET_STREAM_NEXT 			= 3, 	/**< Next packet in stream (R14P_ST_STATEFUL and R14P_ST_STATELESS) */
		R14P_ET_STREAM_END 			= 4, 	/**< Stream ending */
		R14P_ET_STREAM_TIMEOUT			= 5, 	/**< Stream ending */
		R14P_ET_DATAGRAM			= 6, 	/**< New datagram */
		R14P_ET_PAYLOAD_SENT 			= 7, 	/**< Payload sent */
		R14P_ET_CLIENT_IDLE			= 8, 	/**< Client idle */
		R14P_ET_CLIENT_TERMINATING		= 9, 	/**< Client terminating */
		R14P_ET_CLIENT_RECONNECTING		= 10, 	/**< Client re-connecting */
		R14P_ET_CLIENT_RECONNECTED		= 11,	/**< Client re-connected */
		R14P_ET_HEARTBEAT_MISSED		= 12,	/**< Heartbeat missed */
		R14P_ET_HEARTBEAT_RECEIVED		= 13,	/**< Heartbeat received */
		R14P_ET_CLIENT_CREATED			= 100,	/**< Client object created */
		R14P_ET_CLIENT_DESTROYED		= 101,	/**< Client object destroyed */

		// ServiceMessage
		R14P_ET_SRVC_MSG_NEW			= 14,	/**< New ServiceMessage starting */
		R14P_ET_SRVC_SHORT_PARAM_NEW		= 15,	/**< New ServiceMessage short parameter */
		R14P_ET_SRVC_PARAM_STREAM_NEW		= 16,	/**< New ServiceMessage fragmented parameter stream starting */
		R14P_ET_SRVC_PARAM_STREAM_NEXT		= 17,	/**< Next ServiceMessage parameter fragment */
		R14P_ET_SRVC_PARAM_STREAM_END		= 18,	/**< Last ServiceMessage parameter fragment */
		R14P_ET_SRVC_MSG_COMPLETE		= 19,	/**< ServiceMessage ending, all data received */
		R14P_ET_SRVC_MSG_ERROR			= 20	/**< ServiceMessage error, missing or similar */


	};

	/**
	 * Connection direction
	 *
	 */
	enum R14PConnectionDirection {
		R14P_CD_UNKNOWN 	= -1,	/**< Unknown/ERR */
		R14P_CD_INBOUND 	= 0x00,	/**< INBOUND */
		R14P_CD_OUTBOUND	= 0x01	/**< OUTBOUND */
	};

	/**
	 * Sequence type
	 *
	 */
	enum R14PSequenceFlag {
		R14P_SF_UNKNOWN 	= -1,	/**< Unknown/ERR */
		R14P_SF_START 		= 0,	/**< Sequence starting */
		R14P_SF_CONTINUE 	= 1,	/**< Sequence continuing */
		R14P_SF_END 		= 2,	/**< Sequence ending */
		R14P_SF_STATELESS 	= 4,	/**< Single packet sequence */
		R14P_SF_CONTINUE_WAIT 	= 6,	/**< Sequence continuing and waiting for peer or timeout */
		R14P_SF_HEARTBEAT	= 7,	/**< Heartbeat */

	};

	/**
	 * Callback Arguments type
	 */
	enum R14PCBArgsType {
	    R14P_CB_INPUT_ARGS 	= 0,	/**< Input arguments */
	    R14P_CB_OUTPUT_ARGS = 1	/**< Output arguments */

	};

	/**
	 * Callback Argument type
	 */
	enum R14PCBArgType {
	    R14P_CB_ARG_IN_MSG			= 0,	/**< INBOUND R14P message */
	    R14P_CB_ARG_IN_MSG_ID		= 1,	/**< INBOUND R14P message session id */
	    R14P_CB_ARG_CLIENT 			= 2,	/**< Client connection */
	    R14P_CB_ARG_STREAM 			= 3,	/**< Stream connection */
	    R14P_CB_ARG_OUT_MSG 		= 4,	/**< OUTBOUND R14P message */
	    R14P_CB_ARG_PAYLOAD	 		= 5,	/**< OUTBOUND SCTP payload */
	    R14P_CB_ARG_BODY			= 6,	/**< R14P Body indicator */
	    R14P_CB_ARG_IN_RAW			= 7,	/**< INBOUND raw packet bytes */
	    R14P_CB_ARG_IN_RAW_LEN		= 8,	/**< INBOUND raw packet length */
	    R14P_CB_ARG_MEM_SWITCH		= 9,	/**< Switch R14P.OUTPUT message data pointers from R14P.INPUT message to R14P.PAYLOAD  */
	    R14P_CB_ARG_HBEAT_INFO		= 10,	/**< Heartbeat info object */

	    // ServiceMessage
	    R14P_CB_ARGS_SRVC_PARAM		= 11,	/**< ServiceMessage parameter */
	    R14P_CB_ARGS_SRVC_MSG		= 12	/**< ServiceMessage */
	};





	/**
	 * R14P Callback arguments class
	 *
	 */
	class R14PCallbackArgs {
	private:
		std::map<R14PCBArgType, void*> in_args;		/**< input argument list */
		std::map<R14PCBArgType, void*> out_args;	/**< output argument list */

	public:
		R14PCallbackArgs();	/**< Default constructor */

		/**
		 * Add argument to list
		 * @param[in]	_args_type 	List which argument will be inserted into
		 * @param[in]	_arg_type 	Type of argument
		 * @param[in]	_arg 		Pointer to argument
		 */
		void add_arg(R14PCBArgsType _args_type, R14PCBArgType _arg_type, void* _arg);

		/**
		 * Get argument from list
		 * @param[in]	_args_type	List used for search operation
		 * @param[in]	_arg_type	Type of argument
		 * @return	Pointer to argument or NULL if not found
		 */
		void* get_arg(R14PCBArgsType _args_type, R14PCBArgType _arg_type);

		/**
		 * Clear arguments from the list
		 * @param[in]	_args_type	List which will be cleared
		 *
		 */
		void clear_args(R14PCBArgsType _args_type);

		/**
		 * Clear arguments from the both input and output lists
		 */
		void clear_all_args();

		/**
		 * Get number of arguments currently present in the list
		 * @param[in]	_arg_type	List which will be queried
		 */
		int get_arg_count(R14PCBArgsType _arg_type);



	};

	/**
	 * R14P Callback method
	 *
	 */
	class R14PCallbackMethod {
	public:
		R14PCallbackMethod();		/**< Default constructor */
		virtual ~R14PCallbackMethod();	/**< Destructor */

		/**
		 * Run callback method
		 * @param[in]	args	Pointer to callback arguments
		 */
		virtual void run(R14PCallbackArgs* args);

		/**
		 * Cleanup after callback method
		 * @param[in]	args	Pointer to callback arguments
		 */
		virtual void cleanup(R14PCallbackArgs* args);

		/**
		 * Set continue callback method
		 * @param[in[	cb	Pointer to callback method
		 */
		void set_continue_callback(R14PCallbackMethod* cb);

		/**
		 * Remove continue callback method
		 */
		void remove_continue_callback();

		/**
		 * Run continue handler
		 * @param[in]	args	Pointer to callback arguments
		 */
		void run_continue(R14PCallbackArgs* args);

	private:
		R14PCallbackMethod* cb_cont;	/**< continue callback */
	};

	/**
	 * R14P Callback Handler
	 *
	 */
	class R14PCallbackHandler {
	private:
		std::map<R14PEventType, R14PCallbackMethod*> callback_map;	/**< Callback method map */
	public:
		R14PCallbackHandler();	/**< Default constructor */
		~R14PCallbackHandler();	/**< Destructor */

		/**
		 * Set callback
		 * @param[in]	type	Event type to attach callback to
		 * @param[in]	method	Pointer to callbacl method
		 */
		void set_callback(R14PEventType type, R14PCallbackMethod* method);

		/**
		 * Get callback
		 * @param[in]	type	Event type to search for
		 * @return	Pointer to callback method or NULL if not found
		 */
		R14PCallbackMethod* get_callback(R14PEventType type);

		/**
		 * Remove callback
		 * @param[in]	type	Event type to search for and remove
		 *
		 */
		void remove_callback(R14PEventType type);

		/**
		 * Clear all callabcks
		 */
		void clear();

		/**
		 * Execute callback method
		 * @param[in]	type	Event type to search for
		 * @param[in]	args	Pointer to callback arguments passed to callback run method
		 * @return	True if callback method is found or False otherwise
		 */
		bool process_callback(R14PEventType type, R14PCallbackArgs* args);

		/**
		 * Cleanup callback method
		 * @param[in]	type	Event type to search for
		 * @param[in]	args	Pointer to callback arguments passed to callback cleanup method
		 * @return	True if callback method is found or False otherwise
		 */
		bool process_cleanup(R14PEventType type, R14PCallbackArgs* args);
	};

	// fwd declaration
	class R14PClient;
	class R14PPayload;



	/**
	 * R14P Stream
	 */
	class R14PStream {
	    friend class R14PClient;
	    friend class R14PStateMachine;
	    friend class R14PSession;
	private:
		boost::uuids::random_generator* random_generator;	/**< Random number generator */
		unsigned char uuid[16];					/**< Stream UUID */
		uint32_t sequence_num;					/**< Stream sequence number */
		bool sequence_reply_received;				/**< Reply received flag */
		R14PSequenceFlag sequence_flag;				/**< Sequence position flag */
		R14PClient* client;					/**< Client connection */
		R14PCallbackHandler callback_handler;			/**< Stream callback event handler */
		char destination_type[50];				/**< Stream destination type */
		char destination_id[50];				/**< Stream destination id */
		asn1::R14PMessage* r14p_message;			/**< Stream R14PMessage output buffer */
		R14PPayload* r14p_payload;				/**< Stream R14PPayload output buffer */
		time_t timestamp;					/**< Unix timestamp of Last stream update */
		std::map<uint32_t, void*> params;			/**< General parameter map */
		bool timeout;						/**< Stream timeout flag */
		R14PStream* linked_stream;				/**< Linked stream (same guid) */
		R14PStream* last_linked_side;				/**< Last stream side used (duplicate streams, same guids) */
		R14PStreamInitiatorType initiator;			/**< Stream initiator */

	public:
		R14PStream();	/**< Default constructor */

		/**
		 * Custom constructor which sets random number generator used in UUID generation
		 * @param[in]	_random_generator	Pointer to random number generator provided by boost
		 */
		R14PStream(boost::uuids::random_generator* _random_generator);
		~R14PStream();	/**< Destructor */

		/**
		 * Set R14PMessage output buffer
		 * @param[in,out]	_r14p_message	Pointer to R14PMessage
		 */
		void set_r14p_message(asn1::R14PMessage* _r14p_message);

		/**
		 * Get R14PMessage output buffer
		 * @return 	Pointer to R14PMessage output buffer
		 */
		asn1::R14PMessage* get_r14p_message();

		/**
		 * Set R14PPayload output buffer
		 * @param[in,out]	_r14p_payload	Pointer to R14PPayload output buffer
		 */
		void set_r14p_payload(R14PPayload* _r14p_payload);

		/**
		 * Get R14PPayload output buffer
		 * @return	Pointer to R14PPayload output buffer
		 */
		R14PPayload* get_r14p_payload();

		/**
		 * Get client connection
		 * @return Pointer to client connection
		 */
		R14PClient* get_client();

		/**
		 * Set stream destination
		 * @param[in]	_dest_type	Stream destination type
		 * @param[in]	_dest_id	Stream destination id
		 */
		void set_destination(const char* _dest_type, const char* _dest_id);

		/**
		 * Increment sequence number
		 */
		void inc_sequence_num();

		/**
		 * Set sequence flag to END
		 */
		void end_sequence();

		/**
		 * Set sequence flag to CONTINUE
		 */
		void continue_sequence();

		/**
		 * Set sequence flag to WAIT
		 */
		void wait_sequence();

		/**
		 * Set R14PMessage output buffer's sequence flag to CONTINUE
		 */
		void set_continue_flag();

		/**
		 * Set random number generator
		 * @param[in]	_random_generator	Random number generator provided by boost
		 */
		void set_random_generator(boost::uuids::random_generator* _random_generator);

		/*
		 * Get UUID
		 * @return	Pointer to UUID buffer
		 */
		unsigned char* get_uuid();

		/**
		 * Generate new random UUID
		 */
		void generate_uuid();

		/**
		 * Set UUID
		 * @param[in]	_uuid	Pointer to UUID data
		 */
		void set_uuid(unsigned char* _uuid);

		/**
		 * Get sequence number
		 * @return	Current sequence number
		 */
		unsigned int get_sequence_num();

		/**
		 * Get sequence flag
		 * @return	Current sequence flag
		 */
		R14PSequenceFlag get_sequence_flag();

		/**
		 * Reset stream parameters
		 * @param[in]	reset_uuid	Generate new UUID flag
		 */
		void reset(bool reset_uuid);

		/**
		 * Set sequence flag
		 * @param[in]	_sequence_flag	New sequence flag
		 */
		void set_sequence_flag(R14PSequenceFlag _sequence_flag);

		/**
		 * Generate sequence header and push to output queue
		 * @param[in]	include_body	Include R14P body flag
		 */
		void send(bool include_body = true);

		/**
		 * Set client connection
		 * @param[in]	_client		Pointer to client connection
		 */
		void set_client(R14PClient* _client);

		/**
		 * Toggle reply received flag
		 */
		void toggle_seq_reply_received();

		/**
		 * Get reply received flag
		 * @return	Reply received flag
		 */
		bool get_seq_reply_received();

		/**
		 * Set timestamp of last activity
		 * @param[in]	 _timestamp	Unix timestamp
		 */
		void set_timestamp(time_t _timestamp);

		/**
		 * Get unix timestamp of last activity
		 * @return	Unix timestamp of last activity
		 */
		time_t get_timestamp();

		/**
		 * Execute callback method
		 * @param[in]	type	Event type to search for
		 * @param[in]	args	Pointer to callback arguments passed to callback run method
		 * @return	True if callback method is found or False otherwise
		 */
		bool process_callback(R14PEventType type, R14PCallbackArgs* args);

		/**
		 * Set callback
		 * @param[in]	callback_type	Event type to attach callback to
		 * @param[in]	callback_method	Pointer to callback method
		 */
		void set_callback(R14PEventType callback_type, R14PCallbackMethod* callback_method);

		/**
		 * Get callback
		 * @param[in]	callback_type	Event type to search for
		 * @return	Pointer to callback method of NULL if not found
		 */
		R14PCallbackMethod* get_callback(R14PEventType callback_type);

		/**
		 * Remove callback
		 * @param[in]	callback_type	Event type to search for and remove
		 *
		 */
		void remove_callback(R14PEventType callback_type);

		/**
		 * Clear all callabcks
		 */
		void clear_callbacks();

		/**
		 * Get stream parameter
		 * @param[in]	param_id	Parameter id
		 * @return	Pointer to parameter
		 */
		void* get_param(uint32_t param_id);

		/**
		 * Set stream parameter
		 * @param[in]	param_id	Parameter id
		 * @param[in]	param		Pointer to parameter
		 */
		void set_param(uint32_t param_id, void* param);

		/**
		 * Remove stream parameter
		 * @param[in]	param_id	Parameter id
		 * @return	Number of parameters removed
		 */
		int remove_param(uint32_t param_id);

		/**
		 * Clear stream parameters
		 */
		void clear_params();

		/**
		 * Get timeout status
		 * @return	timeout status
		 */
		bool get_timeout_status();

		/**
		 * Set timeout status
		 * @param[in]	_status		Timeout status
		 */
		void set_timeout_status(bool _status);

	};

	// fwd declarations
	class R14PSession;
	class R14PPayload;

	/**
	 * R14P Statistics type
	 */
	enum R14PStatsType {
	    R14P_INBOUND_STATS 	= 0,	/**< INBOUND statistics */
	    R14P_OUTBOUND_STATS = 1	/**< OUTBOUND statistics */
	};

	/**
	 * R14P Statistics
	 */
	class R14PStats {
	public:
	    pmink::Atomic<uint64_t> packets;		/**< Total packets */
	    pmink::Atomic<uint64_t> bytes;		/**< Total bytes */
	    pmink::Atomic<uint64_t> datagrams;		/**< Datagram count */
	    pmink::Atomic<uint64_t> datagram_bytes;	/**< Datagram bytes */
	    pmink::Atomic<uint64_t> datagram_errors;	/**< Datagram errors */
	    pmink::Atomic<uint64_t> streams;		/**< Stream count */
	    pmink::Atomic<uint64_t> stream_bytes;	/**< Stream bytes */
	    pmink::Atomic<uint64_t> stream_errors;	/**< Stream errors */
	    pmink::Atomic<uint64_t> discarded;		/**< Discarded packets */
	    pmink::Atomic<uint64_t> malformed;		/**< Malformed packets */
	    pmink::Atomic<uint64_t> socket_errors;	/**< Socket errors */
	    pmink::Atomic<uint64_t> strm_alloc_errors;	/**< Stream allocation errors */
	    pmink::Atomic<uint64_t> strm_timeout;	/**< Stream timeouts */
	    pmink::Atomic<uint64_t> strm_loopback;	/**< Stream loopback */

	    R14PStats();			/**< Default constructor */

	    R14PStats& operator=(R14PStats& rhs);

	};


	class R14PStateMachine{
	public:
	    R14PStateMachine();
	    ~R14PStateMachine();

	    void init(R14PClient* _r14pc);
	    void run();

	    void process_sf_continue(R14PStream* tmp_stream, bool remove_stream = true);
	    void process_sf_end(R14PStream* tmp_stream, bool remove_stream = true);
	    void process_sf_stream_complete(R14PStream* tmp_stream);

	    // session pointer
	    R14PClient* r14pc;
	    int res;
	    int sctp_len;
	    pollfd fds_lst[1];
	    // set poll timeout to 5 sec
	    int poll_timeout;
	    unsigned char tmp_buff[8192];
	    sctp_sndrcvinfo rcvinfo;
	    sctp_notification* sctp_ntf;
	    sctp_assoc_change* sctp_assoc;
	    int sctp_flags;
	    asn1::R14PMessage r14p_in_message;
	    asn1::R14PMessage r14p_out_message;
	    asn1::ASN1Node root_asn1_node;
	    uint64_t tmp_in_session_id;
	    asn1::ASN1Pool asn1_pool;
	    R14PCallbackArgs cb_args;
	    R14PCallbackArgs cb_stream_args;
	    asn1::SessionId _in_session_id;
	    bool include_body;
	    bool mem_switch;
	    R14PClient* route_c;
	    std::vector<R14PClient*> routes;
	    bool route_this;
	    int custom_seq_flag;
	    //time_t tm_now;
	    asn1::TLVNode* seq_flag_tlv;
	    asn1::TLVNode* seq_num_tlv;
	    asn1::Header* header;
	    asn1::TLVNode* uuid_tlv;
	    char d_id[17];
	    char d_type[17];
	};


	/**
	 * R14P Client connection
	 *
	 */
	class R14PClient {
	    friend class R14PSession;
	    friend class R14PStateMachine;

	private:

		/**
		 * Initialize reconnection procedure
		 */
		void init_reconnect();

		/**
		 * Check for stream timeout
		 * @param[in]	override	If True, timeout all active streams
		 */
		void process_timeout(bool override = false);

		/**
		 * Initialize internal variables
		 */
		void init();

		/**
		 * Execute callbackrun  method
		 * @param[in]	type	Event type to search for
		 * @param[in]	args	Pointer to callback arguments passed to callback run method
		 * @return	True if callback method is found or False otherwise
		 */
		bool process_callback(R14PEventType type, R14PCallbackArgs* args);

		/**
		 * Execute callback cleanup method
		 * @param[in]	type	Event type to search for
		 * @param[in]	args	Pointer to callback arguments passed to callback run method
		 * @return	True if callback method is found or False otherwise
		 */
		bool process_cleanup(R14PEventType type, R14PCallbackArgs* args);

		/**
		 * Start reconnection loop, try to re-establish broken connection
		 */
		int reconnect_socket();

		/**
		 * Inbound thread method
		 * @param[in]	args	Pointer to R14PClient
		 * @return	NULL
		 */
		static void* in_loop(void* args);

		/**
		 * Inbound connection cleanup method
		 * @param[in]	args	Pointer to R14PClient
		 * @return	NULL
		 */
		static void* exit_loop(void* args);

		/**
		 * Outbound thread method
		 * @param[in]	args	Pointer to R14PClient
		 * @return	NULL
		 */
		static void* out_loop(void* args);

		/**
		 * Stream timeout thread method
		 * @param[in]	args	Pointer to R14PClient
		 * @return	NULL
		 */
		static void* timeout_loop(void* args);

		/**
		 * Client registration timeout thread method
		 * @param[in]	args	Pointer to R14PClient
		 * @return	NULL
		 */
		static void* reg_timeout_loop(void* args);

		/**
		 * Set connection activity status
		 * @param[in]	_is_active	Connection activity flag
		 */
		void set_activity(bool _is_active);

		/**
		 * Send packet via SCTP
		 * @param[in]	sctp_stream_id	SCTP stream id
		 * @param[in]	data		Pointer to data
		 * @param[in]	data_length	Length of data
		 * @return	0 if data successfully send or 1 if error occurred
		 */
		int send(unsigned int sctp_stream_id, const unsigned char* data, unsigned int data_length);

		/**
		 * Validate sequence number
		 * @param[in]	data			Raw 4 byte big endian data containing sequence number
		 * @param[in]	data_len		Length of data, should be 4
		 * @param[in]	expected_seq_num	Expected sequence number
		 * @return	True if sequence number equals to expected_seq_num of False otherwise
		 */
		bool validate_seq_num(unsigned char* data, unsigned int data_len, unsigned int expected_seq_num);

		/**
		 * Get routing capable client
		 * @param[in]	in_msg		Pointer to R14P message
		 * @param[in]	sess_id		Current R14P session id
		 * @param[out]	routes		Pointer to output vector for matched routes
		 * @param[out]	d_id		Pointer to C string to receive daemon id
		 * @param[out]	d_type		Pointer to C string to receive daemon type
		 * @return	0 for success or error code otherwise
		 */
		int route(asn1::R14PMessage* in_msg, uint64_t sess_id, std::vector<R14PClient*>* routes, char* d_id, char* d_type);

		/**
		 * Process outbound package
		 * @param[in]		r14ppld		Pointer to R14P payload
		 * @param[in,out]	cb_args		Pointer to callback args
		 * @return	0 for success or error code otherwise
		 */
		int out_process(R14PPayload* r14ppld, R14PCallbackArgs* cb_args);

		R14PSession* session;					/**< Session connection */
		int client_socket;					/**< Client socket id */
		int client_id;						/**< Client id */
		int poll_interval;					/**< Socket poll interval */
		char end_point_address[16];				/**< End point address */
		unsigned int end_point_port;				/**< End point port */
		char local_point_address[16];				/**< Local point ddress */
		unsigned int local_point_port;				/**< Local point port */
		char end_point_daemon_type[17];				/**< End point daemon type */
		char end_point_daemon_id[17];				/**< End point daemon id */
		R14PCallbackHandler callback_handler;			/**< Callback handler */
		boost::mt19937 ran_mt19937;				/**< random number generation algorithm provided by boost */
		// due to boost related valgrind warnings, declared as a pointer to heap allocated memory
		// when this gets fixed, random_generator should be stack declared as an offset of R14PClient class
		boost::uuids::random_generator* random_generator;	/**< random number generator provided by boost */
		pmink::Atomic<bool> active;
		bool router;						/**< Router capabilities flag */
		//bool registered;					/**< R14P daemon registered flag */
		pmink::Atomic<bool> registered;
		pmink::Atomic<bool> stream_timeout_check;		/**< Stream timeout check flag */
		pmink::Atomic<bool> reconnect_queued;			/**< Reconnect flag */

		std::vector<R14PStream*> streams;			/**< List of active streams */
		pmink::RingBuffer<R14PPayload*> out_queue;
		lockfree::SpscQ<R14PPayload> internal_out_queue;
		pmink::Atomic<unsigned int> thread_count;
		pthread_t in_thread;					/**< Inbound thread id */
		pthread_t out_thread;					/**< Outbound thread id */
		pthread_t exit_thread;					/**< Shutdown cleanup thread id */
		pthread_t timeout_thread;				/**< Timeout thread id */
		pthread_t reg_timeout_thread;				/**< R14P client registrationTimeout thread id */
		pthread_attr_t in_thread_attr;				/**< Inbound thread attributes */
		pthread_attr_t out_thread_attr;				/**< Outbound thread attributes */
		pthread_attr_t timeout_thread_attr;			/**< Timeout thread attributes */
		R14PStats in_stats;					/**< Inbound statistics */
		R14PStats out_stats;					/**< Outbound statistics */
		// locks
		pthread_mutex_t mtx_streams;				/**< Active streams mutex */
		pthread_spinlock_t slock_callback;
		pthread_spinlock_t slock_uuid;
		// semaphores
		//sem_t queue_sem;					/**< Outbound queue semaphore */
		// memory pools
		memory::Pool<memory::MemChunk<1024>, true > mc_pool;		/**< Raw chunk memory pool */
		memory::Pool<R14PPayload, true> pld_pool;			/**< R14PPayload memory pool */
		memory::Pool<asn1::R14PMessage, true> r14pm_pool;		/**< R14PMessage memory pool */
		memory::Pool<R14PStream, true> stream_pool;			/**< R14PStream memory pool */
		// ref counter
		pmink::Atomic<uint32_t> ref_counter;
		R14PStateMachine r14p_sm;



	public:


		R14PClient();					/**< Default constructor */

		/**
		 * Custom constructor
		 * @param[in]	_client_socket			SCTP socket id
		 * @param[in]	end_point_address		End point (PEER) address (IP)
		 * @param[in]	end_point_port			End point (PEER) port number
		 * @param[in]	_local_point_address		Local address (IP)
		 * @param[in]	_local_point_port		Local port number
		 * @param[in]	_direction			Connection direction (SERVER/CLIENT)
		 * @param[in]	_max_concurrent_streams		Maximum number of concurrent R14P streams
		 * @param[in]	_stream_timeout			R14P stream timeout in seconds
		 * @param[in]	_poll_interval			Socket poll interval in seconds
		 */
		R14PClient(	int _client_socket,
				const char* end_point_address,
				unsigned int end_point_port,
				const char* _local_point_address,
				unsigned int _local_point_port,
				R14PConnectionDirection _direction,
				int _max_concurrent_streams,
				int _stream_timeout,
				int _poll_interval);

		~R14PClient();					/**< Destructor */

		/**
		 * Increment ref counter
		 * @return	Value after increment
		 */
		uint32_t inc_refc();

		/**
		 * Decrement ref counter
		 * @return	Value after decrement
		 */
		uint32_t dec_refc();

		/**
		 * Get ref counter
		 * @return	Current counter value
		 */
		uint32_t get_refc();

		/**
		 * Generate uuid
		 * @param[out]	out			Output buffer
		 * @return	0 for success or error code
		 */
		int generate_uuid(unsigned char* out);

		/**
		 * Get connection activity status
		 * @return	True if connection is active or False otherwise
		 */
		bool is_active();

		/**
		 * Set registration flag
		 * @param[in]	_reg_flag		Registration flag
		 */
		void set_reg_flag(bool _reg_flag);

		/**
		 * Get registration flag
		 * @return	Registration flag
		 */
		bool is_registered();

		/**
		 * Set end point daemon id
		 * @param[in]	_did			Pointer to daemon id C string
		 */
		void set_end_point_daemon_id(const char* _did);

		/**
		 * Set end point daemon type
		 * @param[in]	_dtype			Pointer to daemon type C string
		 */
		void set_end_point_daemon_type(const char* _dtype);

		/**
		 * Get end point daemon id
		 * @return	Pointer to daemon id C string
		 */
		char* get_end_point_daemon_id();

		/**
		 * Get end point daemon type
		 * @return	Pointer to daemon type C string
		 */
		char* get_end_point_daemon_type();

		/**
		 * Register client (INBOUND, used internally)
		 * @return	0 on success or -1 if error occurred
		 */
		int register_client();

		/**
		 * Get SCTP socket id
		 * @return	SCTP socket id
		 */
		int get_client_socket();

		/**
		 * Get client id
		 * @return	Client id
		 */
		int get_client_id();

		/**
		 * Disconnect
		 * @return 	0 on success or -1 if error occurred
		 */
		int disconnect();

		/**
		 * Get end point address (IP)
		 * @return	Pointer to data containing IP remote address
		 */
		char* get_end_point_address();

		/**
		 * Get end point port
		 * @return	End point port number
		 */
		unsigned int get_end_point_port();

		/**
		 * Get local address (IP)
		 * @return	Pointer to data containing local IP address
		 */
		char* get_local_point_address();

		/**
		 * Get local port number
		 * @return	Local port number
		 */
		unsigned int get_local_point_port();

		/**
		 * Set router capabilities flag
		 * @param[in]	_is_router	Router status flag
		 */
		void set_router_flag(bool _is_router);

		/**
		 * Get router status
		 * @return	True if router enabled or False otherwise
		 */
		bool is_router();

		/**
		 * Initialize threads
		 */
		void init_threads();

		/**
		 * Get connection statistics
		 * @param[in]	stats_type	Type of data requested (IN/OUT)
		 * @param[out]	result		Pointer to result/output data structure
		 */
		void get_stats(R14PStatsType stats_type, R14PStats* result);

		/**
		 * Get connection statistics pointer
		 * @param[in]	stats_type	Type of data requested (IN/OUT)
		 * @return			Pointer to stats data structure
		 */
		R14PStats* get_stats(R14PStatsType stats_type);

		/**
		 * Genereate ACK message
		 * @param[in]	r14p_orig_message	Pointer to original R14P message
		 * @param[out]	r14p_out_message	Pointer to output R14P message
		 * @param[in]	_orig_session_id	Current session id of original R14P message
		 * @param[in]	_out_session_id		New session id of output message (should be 1)
		 * @param[out]	r14pld			Pointer to R14P output payload
		 * @param[in]	include_body		Include body flag (if True, body will be included in otput message)
		 * @param[in]	mem_switch		Memory switch flag (if True, data pointer in output R14P message  will be
		 * 					changed to point to output R14P payload instead of original R14P message)
		 */
		void generate_ack(	asn1::R14PMessage* r14p_orig_message,
					asn1::R14PMessage* r14p_out_message,
					uint64_t _orig_session_id,
					uint64_t _out_session_id,
					R14PPayload* r14pld,
					bool include_body,
					bool mem_switch);

		/**
		 * Genereate ERR message
		 * @param[in]	r14p_orig_message	Pointer to original R14P message
		 * @param[out	r14p_out_message	Pointer to output R14P message
		 * @param[in]	_orig_session_id	Current session id of original R14P message
		 * @param[in]	_out_session_id		New session id of output message (should be 1)
		 * @param[out]	r14pld			Pointer to R14P output payload
		 * @param[in]	mem_switch		Memory switch flag (if True, data pointer in output R14P message  will be
		 * 					changed to point to output R14P payload instead of original R14P message)
		 * @param[in]	_custom_seq_flag	Custom sequence flag
		 * @param[in]	_custom_dtype		Custom daemon type
		 * @param[in]	_custom_did		Custom daemon id
		 * @param[in]	_error_code		Error code to include in R14P output mesage
		 */
		void generate_err(	asn1::R14PMessage* r14p_orig_message,
					asn1::R14PMessage* r14p_out_message,
					uint64_t _orig_session_id,
					uint64_t _out_session_id,
					R14PPayload* r14pld,
					bool mem_switch,
					int _custom_seq_flag,
					char* _custom_dtype,
					char* _custom_did,
					int _error_code);

		/**
		 * Insert destination id in R14P header
		 * @param[in]	r14p_orig_message	Pointer to original R14P message
		 * @param[out]	r14p_out_message	Pointer to output R14P message
		 * @param[in]	_orig_session_id	Current session id of original R14P message
		 * @param[in]	_out_session_id		New session id of output message (should be 1)
		 * @param[in]	_destination_id		Pointer to destination id
		 * @param[in]	_destination_length	Length of destination id
		 * @param[out]	r14pld			Pointer to R14P output payload
		 */
		void set_destination_id(	asn1::R14PMessage* r14p_orig_message,
						asn1::R14PMessage* r14p_out_message,
						uint64_t _orig_session_id,
						uint64_t _out_session_id,
						unsigned char* _destination_id,
						int _destination_length,
						R14PPayload* r14pld);


		int update_hop_info	(	asn1::R14PMessage* r14p_orig_message,
						asn1::R14PMessage* r14p_out_message,
						uint64_t _orig_session_id,
						uint64_t _out_session_id,
						unsigned char* _destination_id,
						int _destination_length,
						R14PPayload* r14pld);


		/**
		 * Genereate stream complete message
		 * @param[in]	r14p_orig_message	Pointer to original R14P message
		 * @param[out]	r14p_out_message	Pointer to output R14P message
		 * @param[in]	_orig_session_id	Current session id of original R14P message
		 * @param[in]	_out_session_id		New session id of output message (should be 1)
		 * @param[out]	r14pld			Pointer to R14P output payload
		 */
		void generate_stream_complete(	asn1::R14PMessage* r14p_orig_message,
						asn1::R14PMessage* r14p_out_message,
						uint64_t _orig_session_id,
						uint64_t _out_session_id,
						R14PPayload* r14pld);

		/**
		 * Genereate initial stream header
		 * @param[out]	r14p_out_message	Pointer to output R14P message
		 * @param[in]	stream			Pointer to R14P stream
		 * @param[in]	_session_id		New session id of output message (should be 1)
		 * @param[out]	r14pld			Pointer to R14P output payload
		 * @param[in]	include_body		Include R14P body flag
		 * @param[in]	_dest_type		Stream destination type
		 * @param[in]	_dest_id		Stream destination id
		 */
		void generate_stream_header(	asn1::R14PMessage* r14p_out_message,
						R14PStream* stream,
						uint64_t _session_id,
						R14PPayload* r14pld,
						bool _include_body,
						const char* _dest_type,
						const char* _dest_id);


		/**
		 * Push to output queue
		 * @param[in]	payload			Pointer to R14P payload
		 * @return	0 for success or -1 if error occurred
		 */
		int push_out_queue(R14PPayload* payload);

		/**
		 * Pop from output queue
		 */
		R14PPayload* pop_out_queue();

		/**
		 * Set session connection
		 * @param[in]	_session	Pointer to session connection
		 */
		void set_session(R14PSession* _session);

		/**
		 * Get session connection
		 * @return	Pointer to session connection
		 */
		R14PSession* get_session();

		/**
		 * Get active thread count
		 * @return	Number of active threads
		 */
		unsigned int get_thread_count();

		/**
		 * Increment active thread count
		 * @return	New number of active threads
		 */
		unsigned int inc_thread_count();

		/**
		 * Decrement active thread count
		 * @return	New number of active threads
		 */
		unsigned int dec_thread_count();

		/**
		 * Create new R14P stream
		 * @return	Pointer to new R14P stream
		 */
		R14PStream* create_stream();

		/**
		 * Initialize new R14P stream
		 *
		 * @msc
		 *
		 * 	a [label="Client"], b [label="Server"];
		 * 	a box b	[label="Example of NORMAL STREAMING", textbgcolour="#ff7f7f"];
		 * 	|||;
		 * 	a=>b 	[label="sf-start seq #1"];
		 * 	b=>a 	[label="sf-continue seq #1"];
		 * 	a=>b 	[label="sf-continue seq #2"];
		 * 	b=>a 	[label="sf-end seq #2"];
		 * 	a=>b 	[label="sf-stream-complete seq #2"];
		 * 	a box b	[label="Example of STREAMING WITH DELAYED REPLY", textbgcolour="#ff7f7f"];
		 * 	|||;
		 * 	a=>b 	[label="sf-start seq #1"];
		 * 	b=>a 	[label="sf-continue seq #1"];
		 * 	a=>b 	[label="sf-continue seq #2"];
		 * 	b=>a 	[label="sf-continue-wait seq #2"];
		 * 	b=>a 	[label="sf-continue seq #2"];
		 * 	a=>b 	[label="sf-end seq #3"];
		 * 	b=>a 	[label="sf-stream-complete seq #3"];
		 *
		 * @endmsc
		 * 		 * @param[in]	_dest_type		Stream destination type
		 * @param[in]	_dest_id		Stream destination id
		 * @param[in]	_on_sent_callback	Payload send callback
		 * @param[in]	_on_reply_callback	Reply received callback (Stream Continue received from peer)
		 * @return	Pointer to new and initialized stream
		 */
		R14PStream* new_stream(	const char* _dest_type,
					const char* _dest_id,
					R14PCallbackMethod* _on_sent_callback,
					R14PCallbackMethod* _on_reply_callback);

		/**
		 * Add stream to list of active streams
		 * @param[in]	_stream			Pointer to new active stream
		 */
		void add_stream(R14PStream* _stream);

		/**
		 * Check if stream is active
		 * @param[in]	_stream			Pointer to stream
		 * @return	True if stream is active
		 */
		bool stream_exists(R14PStream* _stream);

		/**
		 * Remove stream from list of active streams (thread safe)
		 * @param[in]	_stream			Pointer to stream which will be removed
		 */
		void remove_stream(R14PStream* _stream);

		/**
		 * Remove stream from list of active streams (thread unsafe)
		 * @param[in]	_stream			Pointer to stream which will be removed
		 */
		void remove_stream_unsafe(R14PStream* _stream);

		/**
		 * Get stream by UUID
		 * @param[in]	_uuid			UUID to search for
		 * @return	Pointer to stream or NULL if not found
		 */
		R14PStream* get_stream(const unsigned char* _uuid);

		/**
		 * Get stream by index
		 * @param[in]	index			Stream list index
		 * @return	Pointer to stream or NULL if not found
		 */
		R14PStream* get_stream(unsigned int index);

		/**
		 * Get number of active streams
		 * @return	Number of active streams
		 */
		int get_stream_count();


		/**
		 * Send datagram and use raw bytes for payload
		 *
		 * @msc
		 * 	a [label="Client"], b [label="Server"];
		 * 	a box b	[label="Example of NORMAL STATELESS MODE", textbgcolour="#ff7f7f"];
		 * 	|||;
		 * 	a=>b 	[label="sf-stateless"];
		 * 	b=>a 	[label="sf-stream-complete"];
		 * 	a box b	[label="Example of SCTP DEPENDENT STATELESS MODE", textbgcolour="#ff7f7f"];
		 * 	|||;
		 * 	a=>b 	[label="sf-stateless-no-reply"];
		 *
		 * @endmsc
		 *
		 * @param[in]	payload_type			Payload type (R14P DATA)
		 * @param[in]	payload				Pointer to payload data
		 * @param[in]	payload_length			Length to payload data
		 * @param[in]	on_sent_callback_method		Payload send callback
		 * @param[in]	on_reply_callback_method	Reply received callback (if NULL, datagram will be sent as R14P_ST_STATELESS_NO_REPLY)
		 * @param[in]	dest_daemon_type		Datagram destination type
		 * @param[in]	dest_daemon_id			Datagram destination id
		 * @return	0 for success or 1 if error occurred
		 *
		 */
		int send_datagram(	int payload_type,
					unsigned char* payload,
					int payload_length,
					R14PCallbackMethod* on_sent_callback_method,
					R14PCallbackMethod* on_reply_callback_method,
					const char* dest_daemon_type,
					const char* dest_daemon_id);

		/**
		 * Send datagram and use already prepared R14P BODY for payload
		 *
		 * @msc
		 * 	a [label="Client"], b [label="Server"];
		 * 	a box b	[label="Example of NORMAL STATELESS MODE", textbgcolour="#ff7f7f"];
		 * 	|||;
		 * 	a=>b 	[label="sf-stateless"];
		 * 	b=>a 	[label="sf-stream-complete"];
		 * 	a box b	[label="Example of SCTP DEPENDENT STATELESS MODE", textbgcolour="#ff7f7f"];
		 * 	|||;
		 * 	a=>b 	[label="sf-stateless-no-reply"];
		 *
		 * @endmsc
		 *
		 * @param[in]	body				Pointer R14P BODY Payload
		 * @param[in]	on_sent_callback_method		Payload send callback
		 * @param[in]	on_reply_callback_method	Reply received callback (if NULL, datagram will be sent as R14P_ST_STATELESS_NO_REPLY)
		 * @param[in]	dest_daemon_type		Datagram destination type
		 * @param[in]	dest_daemon_id			Datagram destination id
		 * @return	0 for success or 1 if error occurred
		 *
		 */
		int send_datagram(	asn1::Body* body,
					R14PCallbackMethod* on_sent_callback_method,
					R14PCallbackMethod* on_reply_callback_method,
					const char* dest_daemon_type,
					const char* dest_daemon_id);



		/**
		 * Set event callback handler
		 * @param[in]	callback_type		Event type for callback
		 * @param[in]	callback_method		Pointer to callback handler method
		 * @param[in]	unsafe			If True, do not lock mutex
		 */
		void set_callback(R14PEventType callback_type, R14PCallbackMethod* callback_method, bool unsafe = false);

		/**
		 * Get callback
		 * @param[in]	callback_type	Event type to search for
		 * @param[in]	unsafe			If True, do not lock mutex
		 * @return	Pointer to callback method of NULL if not found
		 */
		R14PCallbackMethod* get_callback(R14PEventType callback_type, bool unsafe = false);

		/**
		 * Remove callbak handler
		 * @param[in]	callback_type		Event type for callback
		 * @param[in]	unsafe			If True, do not lock mutex
		 */
		void remove_callback(R14PEventType callback_type, bool unsafe = false);

		/**
		 * Deallocate memory chunk and return back to memory pool
		 * @param[in]	mem_chunk		Pointer to memory chunk which will be returned to pool
		 * @return	0 for success or error code if error occurred
		 */
		int deallocate_mc_pool(memory::MemChunk<1024>* mem_chunk);

		/**
		 * Allocate memory chunk from memory pool
		 * @return	0 for success or error code if error occurred
		 */
		memory::MemChunk<1024>* allocate_mc_pool();

		/**
		 * Deallocate payload and return back to memory pool
		 * @param[in]	r14ppld			Pointer to payload which will be returned to pool
		 * @return	0 for success or error code if error occurred
		 *
		 */
		int deallocate_pld_pool(R14PPayload* r14ppld);

		/**
		 * Allocate payload from memory pool
		 * @return	Pointer to allocated payload
		 */
		R14PPayload* allocate_pld_pool();

		/**
		 * Deallocate R14P message and return back to memory pool
		 * @param[in]	r14pm			Pointer to R14P message which will be returned to pool
		 * @return	0 for success or error code if error occurred
		 *
		 */
		int deallocate_r14pm_pool(asn1::R14PMessage* r14pm);

		/**
		 * Allocate R14P message from memory pool
		 * @return	0 for success or error code if error occurred
		 */
		asn1::R14PMessage* allocate_r14pm_pool();

		/**
		 * Deallocate stream and return back to memory pool
		 * @param[in]	stream			Pointer to stream which will be returned to pool
		 * @return	0 for success or error code if error occurred
		 *
		 */
		int deallocate_stream_pool(R14PStream* stream);

		/**
		 * Allocate stream from memory pool
		 * @return	Pointer to allocated stream
		 */
		R14PStream* allocate_stream_pool();

		R14PConnectionDirection direction;	/**< Connection direction (SERVER/CLIENT) */
		int max_concurrent_streams;		/**< Maximum number of concurrent R14P streams */
		int stream_timeout;			/**< Stream timout */


	};



	/**
	 * R14P payload
	 *
	 */
	class R14PPayload {
	private:
		R14PCallbackHandler callback_handler;	/**< Callback handler */

	public:

		R14PPayload();				/**< Default constructor */
		~R14PPayload();				/**< Destructor */

		/**
		 * Execute callback method
		 * @param[in]	type	Event type to search for
		 * @param[in]	args	Pointer to callback arguments passed to callback run method
		 * @return	True if callback method is found or False otherwise
		 */
		void process_callback(R14PEventType type, R14PCallbackArgs* args);

		/**
		 * Set callback
		 * @param[in]	callback_type		Event type to attach callback to
		 * @param[in]	callback_method		Pointer to callbacl method
		 */
		void set_callback(R14PEventType callback_type, R14PCallbackMethod* callback_method);

		/**
		 * Remove callback
		 * @param[in]	callback_type		Event type to search for and remove
		 *
		 */
		void remove_callback(R14PEventType callback_type);

		/**
		 * Clear all callbacks
		 */
		void clear_callbacks();

		bool free_on_send;			/**< Free on send flag, payload set to be deallocated after successful transfer */
		unsigned int sctp_sid;			/**< Sctp socket id */
		R14PStreamType r14p_stream_type;	/**< Stream type */
		unsigned char* raw_data;		/**< Pointer to raw encoded data */
		unsigned int raw_data_length;		/**< Length of raw encoded data */
		R14PClient* client;			/**< Client connection */
		R14PStream* stream;			/**< Stream connection */
		pmink::Atomic<bool> out;		/**< Processed in out queue flag */


	};

	/**
	 * Routing handler
	 */
	class RouteHandlerMethod {
	public:
	    RouteHandlerMethod(R14PSession* _r14ps);	/**< Default constructor */
	    virtual ~RouteHandlerMethod();		/**< Destructor */

	    /**
	     * Run handler method
	     * @param[in]	all_routes	Pointer to a list of acceptable routes
	     * @param[out]	chosen_routes	Pointer to output vector containing chosen routes
	     */
	    virtual void run(std::vector<R14PClient*>* all_routes, std::vector<R14PClient*>* chosen_routes);

	    virtual void* add_node(R14PClient* r14pc,
				   const char* node_type,
				   const char* node_id,
				   pmink_utils::PooledVPMap<uint32_t>* params);

	    virtual void* get_node(const char* node_type, const char* node_id);



	    virtual void* update_client(R14PClient* r14pc,
					const char* node_type,
					const char* node_id);

	    virtual int remove_type(const char* node_type);
	    virtual int remove_node(const char* node_type, const char* node_id);

	    virtual void clear();

	private:
	    R14PSession* r14ps;

	};

	/**
	 * WRR Routing handler
	 */
	class WRRRouteHandler: public RouteHandlerMethod{
	public:
	    // types
	    typedef std::map<uint32_t, pmink_utils::WRR<r14p::R14PClient*> > wrr_map_t;
	    typedef wrr_map_t::iterator wrr_map_it_t;
	    typedef wrr_map_t::value_type wrr_map_value_t;
	    typedef std::pair<wrr_map_it_t, bool> wrr_map_insert_t;


	    WRRRouteHandler(R14PSession* _r14ps);
	    ~WRRRouteHandler();

	    /**
	     * Run handler method
	     * @param[in]	all_routes	Pointer to a list of acceptable routes
	     * @param[out]	chosen_routes	Pointer to output vector containing chosen routes
	     */
	    void run(std::vector<R14PClient*>* all_routes, std::vector<R14PClient*>* chosen_routes);

	    void* add_node(R14PClient* r14pc,
			   const char* node_type,
			   const char* node_id,
			   pmink_utils::PooledVPMap<uint32_t>* params);

	    void* get_node(const char* node_type, const char* node_id);


	    void* update_client(R14PClient* r14pc,
				const char* node_type,
				const char* node_id);

	    int remove_type(const char* node_type);
	    int remove_node(const char* node_type, const char* node_id);


	    void clear();

	private:
	    // wrr map
	    wrr_map_t wrr_map;

	};

	/*
	 * R14P Routing algorithm
	 */
	enum R14PRoutingAlgorithm{
	    R14P_RA_AUTO	= 0,	/**< Automatic, use first from the list (no cfgd required) */
	    R14P_RA_WRR		= 1	/**< Weighted Round Robin (cfgd required) */
	};

	/**
	 * R14P Session information
	 *
	 */
	class R14PSession {
	friend class RouteHandlerMethod;
	friend class WRRRouteHandler;
	public:
		/**
		 * Custom constructor
		 * @param[in]	_daemon_type			Session daemon type
		 * @param[in]	_daemon_id			Session daemon id
		 * @param[in]	_max_concurrent_streams		Maximum number of concurrent R14P streams
		 * @param[in]	_stream_timeout			R14P stream timeout
		 * @param[in]	_router				R14P router capability flag
		 * @param[in]	_poll_interval			Socket poll interval in seconds
		 */
		R14PSession(	const char* _daemon_type,
				const char* _daemon_id,
				int _max_concurrent_streams,
				int _stream_timeout,
				bool _router,
				int _poll_interval);

		~R14PSession();	/**< Destructor */

		/**
		 * Register client
		 * @param[in]	client			Pointer to R14PClient
		 * @param[in]	dest_daemon_type	Pointer to registration point daemon type
		 */
		int register_client(R14PClient* client, const char* dest_daemon_type);

		/**
		 * Set routing handler
		 * @param[in]	rhandler	Pointer to user defined routing handler
		 */
		void set_routing_handler(RouteHandlerMethod* rhandler);

		/**
		 * Set routing algorithm
		 * @param[in]	algo			Routing algorithm
		 */
		void set_routing_algo(R14PRoutingAlgorithm algo);

		/**
		 * Get routing handler
		 * @return	Pointer to user defined routing handler
		 */
		RouteHandlerMethod* get_routing_handler();

		/**
		 * Add client to list of active clients
		 * @param[in]	client	Pointer to client connection
		 */
		void add_client(R14PClient* client);

		/**
		 * Lock client list
		 */
		void lock_clients();

		/**
		 * Unlock client list
		 */
		void unlock_clients();

		/**
		 * Get router flag
		 * @return	True if router capabilities are active or False otherwise
		 */
		bool is_router();

		/**
		 * Find acceptable route for specific daemon type and/or daemon id
		 * @param[in]	_client		Pointer to calling client
		 * @param[in]	_daemon_type	Pointer to daemon type C string
		 * @param[in]	_daemon_id	Pointer to daemon id C string
		 * @param[out]	routes		Pointer to output vector for matched routes
		 * @return	0 for success or error code otherwise
		 */
		int find_route(	R14PClient* _client,
				const char* _daemon_type,
				const char* _daemon_id,
				std::vector<R14PClient*>* routes);

		/**
		 * Execute callback method
		 * @param[in]	type	Event type to search for
		 * @param[in]	args	Pointer to callback arguments passed to callback run method
		 * @return	True if callback method is found or False otherwise
		 */
		void process_callback(R14PEventType type, R14PCallbackArgs* args);

		/**
		 * Get number of active threads (server thread only for now)
		 * @return	Number of active threads
		 */
		unsigned int get_thread_count();

		/**
		 * Increment number of active threads
		 * @return	New number of active threads
		 */
		unsigned int inc_thread_count();

		/**
		 * Decrement number of active threds
		 * @return	New number of active threads
		 */
		unsigned int dec_thread_count();

		/**
		 * Get deamon type
		 * @return	Pointer to data containing daemon type
		 */
		char* get_daemon_type();

		/**
		 * Get deamon id
		 * @return	Pointer to data containing daemon id
		 */
		char* get_daemon_id();

		/**
		 * Get active client by index
		 * @param[in]	client_index	Client index in active client list
		 * @param[in]	unsafe		Use mutex if False
		 * @return	Pointer to client connection
		 */
		R14PClient* get_client(unsigned int client_index, bool unsafe = false);

		/**
		 * Get active and registered client by index
		 * @param[in]	client_index	Client index in active client list
		 * @param[in]	unsafe		Use mutex if False
		 * @return	Pointer to client connection
		 */
		R14PClient* get_registered_client(unsigned int client_index, bool unsafe = false);

		/**
		 * Get active and registered client by type
		 * @param[in]	daemon_type	Client type
		 * @param[in]	unsafe		Use mutex if False
		 * @return	Pointer to client connection
		 */
		R14PClient* get_registered_client(const char* daemon_type, bool unsafe = false);

		/**
		 * Get active and registered client by type and id
		 * @param[in]	daemon_type	Client type
		 * @param[in]	daemon_id	Client id
		 * @param[in]	unsafe		Use mutex if False
		 * @return	Pointer to client connection
		 */
		R14PClient* get_registered_client(const char* daemon_type, const char* daemon_id, bool unsafe = false);

		/**
		 * Get active client by index
		 * @param[in]	client	Pointer to client
		 * @return	Pointer to active client or NULL if terminated
		 */
		R14PClient* get_client(R14PClient* client);

		/**
		 * Get number of acrive clients
		 * @param[in]	unsafe		Use mutex if False
		 * @return	Number of active clients
		 */
		unsigned int get_client_count(bool unsafe = false);

		/**
		 * Remove client by index
		 * @param[in]	client_index	Client index in active client list
		 * @return 	0 for success or -1 if error occurre
		 */
		int remove_client(unsigned int client_index);

		/**
		 * Remoce client
		 * @param[in]	r14p_client	Pointer to client connection
		 * @return 	0 for success or -1 if error occurre
		 */
		int remove_client(R14PClient* r14p_client);

		/**
		 * Establish SCTP connection to end point
		 * @param[in]	end_point_address	End point address (IP)
		 * @param[in]	end_point_port		End point port number
		 * @param[in]	stream_count		SCTP stream count
		 * @param[in]	local_address		Local address (IP) - if NULL, automatic bind
		 * @param[in]	local_port		Local port number (if zero, automatic port)
		 * @param[in]	skip_r14p_reg		If True, skip R14P Registration (mandatory for routing)
		 * @return	Pointer to new R14P client with established SCTP socket connection
		 */
		R14PClient* connect(	const char* end_point_address,
					unsigned int end_point_port,
					int stream_count,
					const char* local_address,
					unsigned int local_port,
					bool skip_r14p_reg = false);

		/**
		 * Get server socket id
		 * @return	Server socket id
		 */
		int get_server_socket();

		/**
		 * Set server socket id
		 * @param[in]	_socket			Server socket id
		 */
		void set_server_socket(int _socket);

		/**
		 * Get server activity flag
		 * @return	Server activity flag
		 */
		bool get_server_mode();

		/**
		 * Set server activity flag
		 * @param[in]	_server_mode		Server activity flag
		 */
		void set_server_mode(bool _server_mode);

		/**
		 * Start server
		 * @param[in]	bind_address		Local address (IP) - if NULL, automatic bind
		 * @param[in]	bind_port		Local port number (if zero, automatic port)
		 * @return	Server socket id
		 */
		int start_server(const char* bind_address, unsigned int bind_port);

		/**
		 * Stop server
		 * @return	0 for success, -1 if error occurred
		 */
		int stop_server();

		/**
		 * Set callback
		 * @param[in]	callback_type		Event type to attach callback to
		 * @param[in]	callback_method		Pointer to callbacl method
		 */
		void set_callback(R14PEventType callback_type, R14PCallbackMethod* callback_method);

		/**
		 * Remove callback
		 * @param[in]	callback_type		Event type to search for and remove
		 *
		 */
		void remove_callback(R14PEventType callback_type);


	    private:
		    /**
		     * Server thread method
		     * @param[in]	args	Pointer to R14PClient
		     * @return	NULL
		     */
		    static void* server_loop(void* args);

		    R14PCallbackHandler callback_handler;	/**< Callback handler */
		    pthread_mutex_t mtx_callback;		/**< Callback mutex */
		    pthread_mutex_t mtx_clients;		/**< Active clients mutex */
		    pmink::Atomic<unsigned int> thread_count;
		    pthread_t server_thread;			/**< Server thread id */
		    pthread_attr_t server_thread_attr;		/**< Server thread attributes */
		    pmink::Atomic<int> server_socket;
		    int poll_interval;				/**< Socket poll interval */
		    pmink::Atomic<bool> server_mode;
		    char daemon_type[50];			/**< Session daemon type */
		    char daemon_id[50];				/**< Session daemon id */
		    std::vector<R14PClient*> clients;		/**< List of active clients (inbound + outbound) */
		    int max_concurrent_streams;			/**< Maximum number of concurrent R14P streams */
		    int stream_timeout;				/**< R14P stream timeout */
		    bool router;				/**< Router capability flag */
		    RouteHandlerMethod* rh_method;		/*<< Routing handler */
	};

	/**
	 * Heartbeat info
	 */
	class HeartbeatInfo {
	private:
	    pmink::Atomic<bool> active;
	    pmink::Atomic<bool> next;
	    pmink::Atomic<uint64_t> total_sent_count;			/**< Sent heartbeat counter */
	    pmink::Atomic<uint64_t> total_received_count;		/**< Sent heartbeat counter */
	    pmink::Atomic<uint64_t> missed_count;			/**< Missed heartbeat counter */
	    pmink::Atomic<uint64_t> received_count;			/**< Received heartbeat counter */

	public:
	    HeartbeatInfo();				/**< Default constructor */
	    ~HeartbeatInfo();				/**< Destructor */

	    /**
	     * Heartbeat thread method
	     * @param[in]	args	Pointer to HeartbeatInfo
	     * @return	NULL
	     */
	    static void* heartbeat_loop(void* args);

	    /**
	     * Get activity status
	     * @return	True if connection is active or False otherwise
	     */
	    bool is_active();

	    /**
	     * Get ready for next status
	     * @return True if ready for next ot False otherwise
	     */
	    bool next_ready();

	    /**
	     * Set readu for next status
	     */
	    void set_next(bool _next);

	    /**
	     * Set connection activity status
	     * @param[in]	_is_active	Connection activity flag
	     */
	    void set_activity(bool _is_active);

	    /**
	     * Increment total number of received heartbeats
	     */
	    void inc_total_received();

	    /**
	     * Increment total number of sent heartbeats
	     */
	    void inc_total_sent();

	    /**
	     * Get toal number of received heartbeats
	     * @return		Total number of received heartbeats
	     */
	    uint64_t get_total_received();

	    /**
	     * Get total number of sent heartbeats
	     * @return		Total number of sent heartbeats
	     */
	    uint64_t get_total_sent();

	    /**
	     * Increment total number of error free heartbeats
	     */
	    void inc_received();

	    /**
	     * Increment total number of missed heartbeats
	     */
	    void inc_missed();

	    /**
	     * Get total number of error free heartbeats
	     * @return		Total number of error free heartbeats
	     */
	    uint64_t get_received();

	    /**
	     * Get total number of missed heartbeats
	     * @return		Total number of missed heartbeats
	     */
	    uint64_t get_missed();

	    /**
	     * Reset missed number of heartbeats to 0
	     */
	    void reset_missed();


	    R14PClient* r14pc;				/**< R14P client connection */
	    unsigned int interval;			/**< Heartbeat interval in seconds */
	    char target_daemon_type[17];		/**< Target daemon type */
	    char target_daemon_id[17];			/**< Target daemon id */
	    R14PCallbackMethod* on_received;		/**< On heartbeat received event */
	    R14PCallbackMethod* on_missed;		/**< On heartbeat missed event */
	    R14PCallbackMethod* on_cleanup;		/**< On heartbeat cleanup event */
	};


	/**
	 * Initialize R14P session
	 * @param[in]		_daemon_type			Session daemon type
	 * @param[in]		_daemon_id			Session daemon id
	 * @param[in]		_max_concurrent_streams		Maximum number of concurrent R14P streams
	 * @param[in]		_stream_timeout			R14P stream timeout in seconds
	 * @param[in]		_router				R14P router capability flag
	 * @param[in]		_poll_interval			Socket poll interval
	 * @return		Pointer to R14P session
	 */
	R14PSession* init_session(	const char* _daemon_type,
					const char* _daemon_id,
					int _max_concurrent_streams,
					int _stream_timeout,
					bool _router,
					int _poll_interval);


	/**
	 * Destroy R14P Session
	 * @param[in,out]	r14p_session			Pointer to R14P session
	 * @return		0 for success, 1 if error occurred
	 */
	int destroy_session(R14PSession* r14p_session);

	/**
	 * Initialize heartbeat session
	 * @param[in]		_daemon_type			Target daemon type
	 * @param[in]		_daemon_id			Target daemon id
	 * @param[in]		_client				R14P client connection
	 * @param[in]		interval			Heartbeat interval in seconds
	 * @param[in]		_on_received			On heartbeat received event
	 * @param[in]		_on_missed			On heartbeat missed event
	 * @param[in]		_on_cleanup			On heartbeat cleanup event
	 * @return		Pointer to HeartbeatInfo or NULL if error occurred
	 *
	 */
	HeartbeatInfo* init_heartbeat(	const char* _daemon_type,
					const char* _daemon_id,
					R14PClient* _client,
					unsigned int interval,
					R14PCallbackMethod* _on_received,
					R14PCallbackMethod* _on_missed,
					R14PCallbackMethod* _on_cleanup);

	/**
	 * Stop heartbeat session
	 * @param[in]		Pointer to heartbeat info
	 */
	void stop_heartbeat(HeartbeatInfo* hi);
}




#endif /* R14P_H_ */
