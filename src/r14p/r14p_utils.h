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
 * @file r14p_utils.h
 * @brief R14P utils library
 *
 */


#ifndef R14P_UTILS_H_
#define R14P_UTILS_H_

#include <r14p.h>
#include <map>
#include <pool.h>
#include <pmink_utils.h>

namespace r14p {

    // fwd declarations
    class ServiceStreamHandlerNew;
    class ServiceMsgManager;
    class ServiceMessage;
    class ServiceMsgManager;
    class ServiceParamFactory;
    class ServiceMessageNext;
    class ServiceMessageDone;

    /**
     * Service parameter data type
     */
    enum ServiceParamType {
	SPT_UNKNOWN	= 0,	/**< Unknown */
	SPT_OCTETS	= 1,	/**< General octet stream */
	SPT_UINT32	= 2,	/**< Unsigned 32bit integer */
	SPT_UINT64	= 3,	/**< Unsigned 64bit integer */
	SPT_FLOAT	= 4,	/**< Float */
	SPT_DOUBLE	= 5,	/**< Double */
	SPT_CSTRING	= 6,	/**< C style string */
	SPT_BOOL	= 7,	/**< Boolean */
	SPT_VARIANT	= 8	/**< Pmink variant */

    };

    /**
     * Service parameter class
     */
    class ServiceParam {
    public:
	/**
	 * Default constructor
	 */
	ServiceParam();

	/**
	 * Destructor
	 */
	virtual ~ServiceParam();

	/**
	 * Extract parameter data
	 * @param[out]	_out		Pointer to output buffer
	 * @return	0 for success or error code
	 */
	virtual int extract(void* _out) = 0;

	/**
	 * Print parameter data to standard output
	 */
	virtual void std_out();

	/**
	 * Set service param data from Variant param
	 */
	void set(pmink_utils::VariantParam* vparam);

	/**
	 * Set parameter data
	 * @param[in]	_data		Pointer to parameter data
	 * @param[in]	_data_size	Data size
	 * @return	0 for success or error code
	 */
	virtual int set_data(const void* _data, unsigned int _data_size);

	/**
	 * Set parameter data
	 * @param[in]	_data		Pointer to file
	 * @param[in]	_file_size	File size
	 * @return	0 for success or error code
	 */
	virtual int set_data(FILE* _data, unsigned int _file_size);

	/**
	 * Get pointer to parameter data
	 * @return	Pointer to parameter data
	 */
	unsigned char* get_data();

	/**
	 * Get data pointer
	 */
	unsigned char* get_data_p();

	/**
	 * Set data pointer
	 */
	void set_data_p(unsigned char* _data_p);

	/**
	 * Set data pointer to internal buffer
	 */
	void reset_data_p();

	/**
	 * Get size of parameter data
	 * @return	Parameter data size
	 */
	int get_data_size();

	/**
	 * Increment total data size
	 * @param[in]	_inc		Increment
	 */
	void inc_total_data_size(unsigned int _inc);

	/**
	 * Get total data size
	 * @return	Total data size including all fragments
	 */
	int get_total_data_size();

	/**
	 * Get service parameter data type
	 * @return	Service parameter data type
	 */
	ServiceParamType get_type();

	/**
	 * Set parameter id
	 * @param[in]	_id		Parameter id
	 */
	void set_id(uint32_t _id);

	/**
	 * Get parameter id
	 * @return	Parameter id
	 */
	uint32_t get_id();

	/**
	 * Get pointer to parameter id
	 * @return	Pointer to parameter id
	 */
	uint32_t* get_idp();

	/**
	 * Reset parameter values
	 */
	void reset();

	/**
	 * Set thread safety
	 * @param[in]	Thread safety flag
	 */
	void set_thread_safety(bool _thread_safe);

	/**
	 * Set service message manager
	 * @param[in]	Pointer to service massage manager
	 */
	void set_param_factory(ServiceParamFactory* _pfact);

	/**
	 * Get fragmentation flag
	 */
	bool is_fragmented();

	/**
	 * Get pointer to fragmentation flag
	 */
	bool* get_fragmentation_p();

	/**
	 * Set fragmentation flag
	 */
	void set_fragmented(bool _fragmented);

	/**
	 * Get param id index
	 */
	uint32_t get_index();

	/**
	 * Get variant param type
	 */
	int get_extra_type();

	/**
	 * Get fragment index
	 */
	int get_fragment_index();

	void set_callback(R14PEventType type, R14PCallbackMethod* cback);
	bool process_callback(R14PEventType type, R14PCallbackArgs* args);
	void clear_callbacks();


	// friend with ServiceMsgManager
	friend class ServiceMsgManager;
	friend class ServiceMessageNext;
	friend class ServiceMessage;
	friend class ServiceStreamHandlerNext;
	friend class ServiceStreamHandlerDone;
	friend class ServiceStreamHandlerNew;


    protected:
	void lock();				/**< Lock mutex */
	void unlock();				/**< Unlock mutex */

	void fragment(const void* _data, unsigned int _data_size);


	unsigned char data[256];		/**< Parameter data buffer */
	unsigned char* data_p;			/**< Parameter data pointer */
	const void* in_data_p;			/**< Input data pointer */
	unsigned int data_size;			/**< Parameter data size */
	unsigned int total_data_size;		/**< Total data size including all fragments */
	ServiceParamType type;			/**< Parameter type */
	uint32_t id;				/**< Parameter id */
	uint32_t index;				/**< Parameter index */
	int extra_type;				/**< Extra parameter type */
	pthread_mutex_t mtx;			/**< Mutex */
	bool thread_safe;			/**< Thread safe flag */
	bool fragmented;			/**< Fragmentation flag */
	std::vector<ServiceParam*> linked;	/**< Linked params/fragments */
	unsigned int linked_index;
	ServiceParamFactory* param_fctry;	/**< Pointer to service param factory */
	R14PCallbackHandler cb_handler;
	int fragments;
	int fragment_index;

	static bool FRAGMENTATION_DONE;		/**< Fragmentation finished, last fragment */
	static bool FRAGMENTATION_NEXT;		/**< Fragmentation in progress, more fragments coming */


	// data read callback
	typedef int (*param_data_cb_type)(ServiceParam* sc_param, const void* in, int in_size);

	param_data_cb_type param_data_cb;


	static int param_data_file(ServiceParam* sc_param, const void* in, int in_size);
	static int param_data_default(ServiceParam* sc_param, const void* in, int in_size);

    };

    /**
     * Unknown service parameter class
     */
    class ServiceParamVARIANT: public ServiceParam {
    public:
	ServiceParamVARIANT();		/**< Default constructor */
	~ServiceParamVARIANT();		/**< Destructor */

	/**
	 * Extract parameter data
	 * @param[out]	_out		Pointer to output buffer
	 * @return	0 for success or error code
	 */
	int extract(void* _out);

	/**
	 * Set parameter data
	 * @param[in]	_data		Pointer to parameter data
	 * @param[in]	_data_size	Data ssize
	 * @return	0 for success or error code
	 */

	int set_data(void* _data, unsigned int _data_size);

	/**
	 * Print parameter data to standard output
	 */
	void std_out();
    };


    /**
     * Unknown service parameter class
     */
    class ServiceParamUNKNOWN: public ServiceParam {
    public:
	ServiceParamUNKNOWN();		/**< Default constructor */
	~ServiceParamUNKNOWN();		/**< Destructor */

	/**
	 * Extract parameter data
	 * @param[out]	_out		Pointer to output buffer
	 * @return	0 for success or error code
	 */
	int extract(void* _out);

	/**
	 * Set parameter data
	 * @param[in]	_data		Pointer to parameter data
	 * @param[in]	_data_size	Data ssize
	 * @return	0 for success or error code
	 */

	int set_data(void* _data, unsigned int _data_size);

	/**
	 * Print parameter data to standard output
	 */
	void std_out();
    };

    /**
     * Boolean service parameter class
     */
    class ServiceParamBOOL: public ServiceParam {
    public:
	ServiceParamBOOL();		/**< Default constructor */
	~ServiceParamBOOL();		/**< Destructor */

	/**
	 * Extract parameter data
	 * @param[out]	_out		Pointer to output buffer
	 * @return	0 for success or error code
	 */
	int extract(void* _out);

	/**
	 * Set bool value
	 * @param[in]	_data		Parameter data
	 * @return	0 for success or error code
	 */
	int set_bool(bool _data);

	/**
	 * Print parameter data to standard output
	 */
	void std_out();

    };



    /**
     * Unsigned 32bit integer service parameter class
     */
    class ServiceParamUINT32: public ServiceParam {
    public:
	ServiceParamUINT32();		/**< Default constructor */
	~ServiceParamUINT32();		/**< Destructor */

	/**
	 * Extract parameter data
	 * @param[out]	_out		Pointer to output buffer
	 * @return	0 for success or error code
	 */
	int extract(void* _out);

	/**
	 * Set unsigned 32bit integer value
	 * @param[in]	_data		Parameter data
	 * @return	0 for success or error code
	 */
	int set_uint32(uint32_t _data);

	/**
	 * Print parameter data to standard output
	 */
	void std_out();

    };

    /**
     * Unsigned 64bit integer service parameter class
     */
    class ServiceParamUINT64: public ServiceParam {
    public:
	ServiceParamUINT64();		/**< Default constructor */
	~ServiceParamUINT64();		/**< Destructor */

	/**
	 * Extract parameter data
	 * @param[out]	_out		Pointer to output buffer
	 * @return	0 for success or error code
	 */
	int extract(void* _out);

	/**
	 * Set unsigned 64bit integer value
	 * @param[in]	_data		Parameter data
	 * @return	0 for success or error code
	 */
	int set_uint64(uint64_t _data);

	/**
	 * Print parameter data to standard output
	 */
	void std_out();

    };

    /**
     * C style string service parameter class
     */
    class ServiceParamCString: public ServiceParam {
    public:
	ServiceParamCString();		/**< Default constructor */
	~ServiceParamCString();		/**< Destructor */

	/**
	 * Extract parameter data
	 * @param[out]	_out		Pointer to output buffer
	 * @return	0 for success or error code
	 */
	int extract(void* _out);

	/**
	 * Set C style string value
	 * @param[in]	cstring		Pointer to C style string
	 */
	void set_cstring(char* cstring);

    };

    /**
     * General octet stream service parameter class
     */
    class ServiceParamOctets: public ServiceParam {
    public:
	ServiceParamOctets();		/**< Default constructor */
	~ServiceParamOctets();		/**< Destructor */

	/**
	 * Extract parameter data
	 * @param[out]	_out		Pointer to output buffer
	 * @return	0 for success or error code
	 */
	int extract(void* _out);

	/**
	 * Print parameter data to standard output
	 */
	void std_out();


    };

    /**
     * Service parameter factory class
     */
    class ServiceParamFactory {
    public:
	/**
	 * Constructor
	 * @param[in]	_pooled		Parameter pooling flag
	 * @param[in]	pool_size	Pool size
	 */
	ServiceParamFactory(bool _pooled = true, bool _th_safe = false, unsigned int pool_size = 100);

	/**
	 * Destructor
	 */
	~ServiceParamFactory();

	/**
	 * Create new service parameter
	 * @param[in]	param_type	Service parameter type
	 * @return	Pointer to service parameter
	 */
	ServiceParam* new_param(ServiceParamType param_type = r14p::SPT_UNKNOWN);

	/**
	 * Free service parameter
	 * @param[in]	param		Pointer to service parameter
	 * @return	0 for success or error code
	 */
	int free_param(ServiceParam* param);

    private:
	//pthread_mutex_t mtx_param;			/**< Param mutex */
	bool pooled;					/**< Pooling flag */
	memory::Pool<ServiceParamCString, true> cstr_pool;	/**< SPT_CSTRING memory pool */
	memory::Pool<ServiceParamOctets, true> oct_pool;	/**< SPT_OCTETS memory pool */
	memory::Pool<ServiceParamUINT32, true> uint32_pool;	/**< SPT_UINT32 memory pool */
	memory::Pool<ServiceParamUINT64, true> uint64_pool;	/**< SPT_UINT64 memory pool */
	memory::Pool<ServiceParamUNKNOWN, true> unknown_pool; /**< SPT_UNKNOWN memory pool */
	memory::Pool<ServiceParamBOOL, true> bool_pool; 	/**< SPT_BOOL memory pool */
	memory::Pool<ServiceParamVARIANT, true> var_pool; 	/**< SPT_VARIANT memory pool */

    };

    /**
     * Service parameter ID <--> TYPE mapping class
     */
    class ParamIdTypeMap {
    public:
	ParamIdTypeMap();	/**< Default constructor */
	~ParamIdTypeMap();	/**< Destructor */

	/**
	 * Add new mapping
	 * @param[in]	_id		Service parameter id
	 * @param[in]	_type		Service parameter type
	 * @return	0 for success or error code
	 */
	int add(uint32_t _id, ServiceParamType _type);

	/**
	 * Remove mapping
	 * @param[in]	id		Service parameter id
	 * @return	0 for success or error code
	 */
	int remove(uint32_t id);

	/**
	 * Get mapping
	 * @param[in]	id		Service parameter id
	 * @return	Service parameter type
	 */
	ServiceParamType get(uint32_t id);

	/**
	 * Clear all mappings
	 */
	int clear();

    private:
	std::map<uint32_t, ServiceParamType> idtmap;	/**< ID <--> TYPE map */

    };


    class ServiceStreamHandlerNext: public r14p::R14PCallbackMethod {
    public:
	// handler method for ServiceMessage streams
	void run(r14p::R14PCallbackArgs* args);
	ServiceStreamHandlerNew* ssh_new;

    };

    class ServiceStreamHandlerDone: public r14p::R14PCallbackMethod {
    public:
	// handler method for ServiceMessage streams
	void run(r14p::R14PCallbackArgs* args);
	ServiceStreamHandlerNew* ssh_new;

    };

    class ServiceStreamHandlerNew: public r14p::R14PCallbackMethod {
    public:
	ServiceStreamHandlerNew();
	// handler method for ServiceMessage streams
	void run(r14p::R14PCallbackArgs* args);
	ServiceStreamHandlerNext ssh_next;
	ServiceStreamHandlerDone ssh_done;
	ServiceMsgManager* smsg_m;
	R14PCallbackMethod* usr_stream_hndlr;

    };

    class ServiceStreamNewClient: public r14p::R14PCallbackMethod {
    public:
	ServiceStreamNewClient();
        // handler method
        void run(r14p::R14PCallbackArgs* args);
        // service message manager
        r14p::ServiceMsgManager* smsg_m;
        // user stream nc handler
	R14PCallbackMethod* usr_stream_nc_hndlr;
        // user stream handler
	R14PCallbackMethod* usr_stream_hndlr;

    };

    class ServiceMessageDone: public r14p::R14PCallbackMethod {
    public:
	ServiceMessageDone();
	void run(r14p::R14PCallbackArgs* args);
	ServiceMessage* smsg;
	R14PCallbackMethod* usr_method;
	int status;
    };

    class ServiceMessageNext: public r14p::R14PCallbackMethod {
    public:
	void run(r14p::R14PCallbackArgs* args);
	ServiceMessage* smsg;
	unsigned int pc;
	unsigned int pos;
	unsigned int pindex;
    };


    class ServiceMessageAsyncDone: public r14p::R14PCallbackMethod {
    public:
	void run(r14p::R14PCallbackArgs* args);

    };


    /**
     * Service message class
     */
    class ServiceMessage {
    public:
	ServiceMessage();	/**< Default constructor */
	~ServiceMessage();	/**< Destructor */

	/**
	 * Add service parameter
	 * @param[in]	id		Service parameter id
	 * @param[in]	param		Pointer to service parameter
	 * @param[in]	index		Parameter id index
	 * @return	0 for success or error code
	 */
	int add_param(uint32_t id, ServiceParam* param, uint32_t index = 0);

	/**
	 * Remove service parameter
	 * @param[in]	id		Service parameter id
	 * @return	0 for success or error code
	 */
	int remove_param(uint32_t id);

	/**
	 * Get service parameter(s) by id
	 * @param[in]	id		Service parameter id
	 * @param[out]	out		Pointer to output vector
	 * @return	0 for success or error code
	 *
	 */
	int get_param(uint32_t id, std::vector<ServiceParam*>* out);

	/**
	 * Reset service message values
	 * @return	0 for success or error code
	 */
	int reset();

	/**
	 * Set ID <--> TYPE mapping
	 * @param[in]	idtm		Pointer to IDT mapping
	 *
	 */
	void set_idt_map(ParamIdTypeMap* idtm);

	/**
	 * Get service id
	 * @return	Service id
	 */
	uint32_t get_service_id();

	/**
	 * Get pointer to service id
	 * @return	Pointer to service id
	 */
	uint32_t* get_service_idp();

	/**
	 * Get service action
	 * @return	Service action
	 */
	uint32_t get_service_action();

	/**
	 * Get pointer to service action
	 * @return	Pointer to service action
	 */
	uint32_t* get_service_actionp();

	/**
	 * Set service id
	 * @param[in]	_service_id		Service id
	 *
	 */
	void set_service_id(uint32_t _service_id);

	/**
	 * Set service action
	 * @param[in]	_service_action		Service action
	 *
	 */
	void set_service_action(uint32_t _service_action);

	/**
	 * Get all service message parameters
	 * @return	Pointer to parameter vector
	 *
	 */
	std::vector<ServiceParam*>* get_param_map();

	/**
	 * Get service message stream done handler
	 * @return	Pointer to stream handler
	 */
	ServiceMessageDone* get_sdone_hndlr();

	/**
	 * Get service message stream next handler
	 * @eturn	Pointer to stream handler
	 */
	ServiceMessageNext* get_snext_hndlr();

	/**
	 * Wait for stream finished semaphore
	 * @return	0 for success or error code
	 */
	int signal_wait();

	/**
	 * Signal stream finished semaphore
	 * @return	0 for success or error code
	 *
	 */
	int signal_post();

	/**
	 * Set service message manager
	 * @param[in]	_smsg_m		Pointer to service message manager
	 */
	void set_smsg_manager(ServiceMsgManager* _smsg_m);

	/**
	 * Get service message manager
	 * @return	Pointer to service mesage manager
	 */
	ServiceMsgManager* get_smsg_manager();

	/**
	 * Get current fragmented param
	 * @return	Pointer to current fragmented param
	 */
	ServiceParam* get_frag_param();

	/**
	 * Set current fragmented param
	 * @param[in]	_frag_param	Pointer to new fragmented param
	 */
	void set_frag_param(ServiceParam* _frag_param);

	/**
	 * Get complete flag
	 */
	bool is_complete();

	/**
	 * Set complete flag
	 */
	bool set_complete(bool _is_complete);
	bool set_auto_free(bool _auto_free);
	bool get_auto_free();
	void set_callback(R14PEventType type, R14PCallbackMethod* cback);
	bool process_callback(R14PEventType type, R14PCallbackArgs* args);
	void clear_callbacks();

	pmink_utils::ParameterMap<uint32_t, void*> params;	/**< General param list */
	pmink_utils::PooledVPMap<uint32_t> vpmap;
	bool missing_params;


    private:
	ParamIdTypeMap* idt_map;				/**< IDT mapping */
	std::vector<ServiceParam*> tlvs;			/**< Service parameter list */
	uint32_t service_id;					/**< Service id */
	uint32_t service_action;				/**< Service action */
	ServiceMessageDone msg_done;				/**< Stream done handler */
	ServiceMessageNext msg_next;				/**< Stream next handler */
	ServiceMsgManager* smsg_m;				/**< Service message manager */
	sem_t smsg_sem;						/**< Stream done semaphore */
	sem_t new_param_sem;					/**< New param received semaphore */
	ServiceParam* frag_param;				/**< Current fragmented param */
	pmink::Atomic<bool> complete;				/**< Complete flag */
	pmink::Atomic<uint32_t> recv_param_count;		/**< Received param count */
	R14PCallbackHandler cb_handler;
	bool auto_free;





    };

    /**
     * Stats
     */
    enum SrvcSatsType{
        SST_RX_SMSG_POOL_EMPTY		= 1,
        SST_RX_SPARAM_POOL_EMPTY	= 2
    };

    /**
     * Smsg related R14P stream params
     */
    enum SmsgParamType{
	SMSG_PT_SMSG			= 0,
	SMSG_PT_PASS			= 1
    };


    /**
     * Service message manager
     */
    class ServiceMsgManager {
    public:
	/**
	 * Constructor
	 * @param[in]	_idt_map			Pointer to IDT mapping
	 * @param[in]	_new_msg_hndlr			Handler for SERVICE MESSAGE STARTING event
	 * @param[in]	_nonsrvc_stream_hndlr		Handler for NON SERVICE MESSAGE
	 * @param[in]	pool_size			Service message faatory pool size
	 * @param[in]	param_pool_size			Param factory pool size (per type)
	 */
 	ServiceMsgManager(	ParamIdTypeMap* _idt_map,
 				R14PCallbackMethod* _new_msg_hndlr,
 				R14PCallbackMethod* _nonsrvc_stream_hndlr = NULL,
 				unsigned int pool_size = 100,
 				unsigned int param_pool_size = 1000);

 	/**
 	 * Destructor
 	 */
 	~ServiceMsgManager();

 	/**
 	 * Setup service message event handlers for R14P client
 	 * @param[in]	r14pc			Pointer to R14P client
 	 *
 	 */
 	void setup_client(R14PClient* r14pc);

 	/**
 	 * Setup service message event handlers for R14P server
 	 * @param[in]	r14ps			Pointer to R14P session
 	 * @param[in]	_usr_stream_nc_hndlr	Pointer to user handler for NEW CLIENT event
 	 * @param[in]	_usr_stream_hndlr	Pointer to user handler for NEW STREAM event
 	 *
 	 */
 	void setup_server(	R14PSession* r14ps,
 				r14p::R14PCallbackMethod* _usr_stream_nc_hndlr,
 				r14p::R14PCallbackMethod* _usr_stream_hndlr);

 	/**
 	 * Receive new service message or block until one is available
 	 * @return	Pointer to service message
 	 */
 	//ServiceMessage* recv();

 	/**
 	 * Send service message
 	 * @param[in]	msg			Pointer to service message
 	 * @param[in]	r14pc			Pointer to R14P client
 	 * @param[in]	dtype			Pointer to destination daemon type C string
 	 * @param[in]	did			Pointer to destination daemon id C string
 	 * @param[in]	async			Async flag
 	 * @param[in]	on_sent			Pointer to message sent event handler used only in async mode only
 	 * @return 	0 for success or error code
 	 */
 	int send(	ServiceMessage* msg,
 			R14PClient* r14pc,
 			const char* dtype,
 			const char* did,
 			bool async = false,
 			r14p::R14PCallbackMethod* on_sent = &cb_async_done);

 	/**
 	 * Sync sparam map to reflect vpmap
 	 * @param[in]	msg			Pointer to service message
 	 * @return	0 for success of error code
 	 */
 	int vpmap_sparam_sync(ServiceMessage* msg);

 	/**
 	 * Create new service message
 	 * @return	Pointer to service message
 	 */
 	ServiceMessage* new_smsg();

 	/**
 	 * Free service message
 	 * @param[in]	msg		Service message pointer
 	 * @param[in]	params_only	Free only params flag
 	 * @param[in]	clear_vpmap	Clear vpmap flag
 	 * @return 	0 for success or error code
 	 */
 	int free_smsg(ServiceMessage* msg, bool params_only = false, bool clear_vpmap = true);

 	/**
 	 * Get service parameter factory
 	 * @return	Pointer to service parameter factory
 	 */
 	ServiceParamFactory* get_param_factory();

 	/**
 	 * Get IDT mapping
 	 * @return	Pointer to IDT mapping
 	 */
 	ParamIdTypeMap* get_idt_map();

 	/**
 	 * Make service message available
 	 * @param[in]	msg	Pointer to service message
 	 * @return	0 for success or error code
 	 */
 	//int publish_active_msg(ServiceMessage* msg);

	/**
	 * Generate new random UUID
	 */
	void generate_uuid(unsigned char* out);

	/**
	 * Get ServiceMessage NEW STREAM handler
	 */
	r14p::R14PCallbackMethod* get_srvcs_hndlr();

	/**
	 * Get ServiceMessage NEW CLIENT handler
	 */
	r14p::R14PCallbackMethod* get_srvcs_nc_hndlr();

	void set_new_msg_handler(R14PCallbackMethod* hndlr);
	void set_msg_err_handler(R14PCallbackMethod* hndlr);
	bool process_callback(R14PEventType type, R14PCallbackArgs* args);

	static const int MAX_PARAMS_SIZE = 768;		/**< MAX parameter size constant */
	pmink_utils::StatsManager stats;

     private:
	boost::uuids::random_generator* random_gen;	/**< Random number generator */
	boost::mt19937 ran_mt19937;			/**< random number generation algorithm provided by boost */
 	sem_t q_sem;					/**< Service message semaphore */
	ParamIdTypeMap* idt_map;			/**< IDT mapping */
 	memory::Pool<ServiceMessage, true> msg_pool;		/**< Service message memory pool */
	ServiceParamFactory* param_factory;		/**< Service parameter factory */
	ServiceStreamHandlerNew srvcs_hndlr;		/**< Service message new stream handler */
	ServiceStreamNewClient srvcs_nc;		/**< New client stream handler (R14P server) */
	R14PCallbackHandler cb_handler;
	static ServiceMessageAsyncDone cb_async_done;

     };



}


#endif /* R14P_UTILS_H_ */
