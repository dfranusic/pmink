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
 * @brief R14P statistics and trap management
 *
 */

#ifndef R14P_STATS_H_
#define R14P_STATS_H_

#include <r14p.h>

namespace r14p {

    /**
     * Trap id
     */
    class TrapId {
    public:
	TrapId(const char* _label = NULL);
	TrapId(const std::string& _label);

	std::string label;

    };

    /**
     * User id comparison for user map
     */
    class TrapIdCompare {
    public:
	    /**
	     * Custom operator '()'
	     */
	    bool operator ()(const TrapId& x, const TrapId& y);

    };

    // fwd declarations
    class TrapStreamNew;
    class R14PStatsSession;
    class TrapId;

    class TrapClientDone: public R14PCallbackMethod {
    public:
	// event handler method
	void run(r14p::R14PCallbackArgs* args);
    };


    class TrapStreamDone: public R14PCallbackMethod {
    public:
	// event handler method
	void run(r14p::R14PCallbackArgs* args);
	TrapStreamNew* snew;

    };


    class TrapStreamNext: public R14PCallbackMethod {
    public:
	// event handler method
	void run(r14p::R14PCallbackArgs* args);
	TrapStreamDone sdone;
    };

    class TrapStreamNew: public R14PCallbackMethod {
    public:
	TrapStreamNew();
	// event handler method
	void run(r14p::R14PCallbackArgs* args);
	TrapStreamNext snext;
	std::map<TrapId, uint64_t, TrapIdCompare> traps;
	std::map<TrapId, uint64_t, TrapIdCompare>::iterator trap_iter;
	//int trap_index;
	uint32_t trap_count;
	uint32_t pt_stats_id;
	uint32_t pt_stats_count;
	uint32_t pt_stats_value;
	uint32_t pt_stats_desc;
	uint32_t stats_action;
	R14PStatsSession* ss;
	TrapStreamNew* snew;

    };


    class TrapClientNew: public R14PCallbackMethod {
    public:
	TrapClientNew();
	// event handler method
	void run(r14p::R14PCallbackArgs* args);
	TrapStreamNew snew;
	R14PStatsSession* ss;

    };




    /**
     * Custom trap handler
     */
    class R14PTrapHandler {
    public:
	R14PTrapHandler();		/**< Default constructor */
	virtual ~R14PTrapHandler();	/**< Destructor */
	virtual void run();		/**< Handler execute method */

	uint64_t value;			/**< Last retrieved value */

    };


    // R14PStatsHandler
    class R14PStatsHandler: public R14PTrapHandler {
    public:
	R14PStatsHandler(pmink::Atomic<uint64_t>* _sval_p);
	void run();

    private:
	pmink::Atomic<uint64_t>* sval_p;
    };

    // R14PStatsClientCreated
    class R14PStatsClientCreated: public r14p::R14PCallbackMethod {
    public:
	void run(r14p::R14PCallbackArgs* args);

	R14PStatsSession* r14p_stats;

    };

    class R14PStatsClientDestroyed: public r14p::R14PCallbackMethod {
    public:
	void run(r14p::R14PCallbackArgs* args);

	R14PStatsSession* r14p_stats;

    };

    /**
     * Stats session managment
     */
    class R14PStatsSession {
    public:
	/**
	 * Constructor
	 * @param[in]	_poll_interval	Trap data acquisition interval
	 * @param[in]	_host_r14ps	Pointer to host R14P session
	 * @param[in]	_stats_port	Stats R14P inbound port
	 */
	R14PStatsSession(int _poll_interval, r14p::R14PSession* _host_r14ps, int _stats_port = 0);

	/**
	 * Destructor
	 */
	~R14PStatsSession();

	/**
	 * Add new trap
	 * @param[in]	trap_id		Trap id
	 * @param[in]	handler		Pointer to trap handler
	 * @return	0 for success or error code
	 */
	int add_trap(const TrapId* trap_id, R14PTrapHandler* handler);

	/**
	 * Add new trap
	 * @param[in]	trap_id		Trap id
	 * @param[in]	handler		Pointer to trap handler
	 * @return	0 for success or error code
	 */
	int add_trap(const TrapId& trap_id, R14PTrapHandler* handler);


	/**
	 * Remove trap
	 * @param[in]	trap_id		Trap id
	 * @return	Pointer to trap handler or NULL of not found
	 */
	R14PTrapHandler* remove_trap(const TrapId& trap_id);

	/**
	 * Get trap handler
	 * @param[in,out]	trap_id		Trap id to match and update
	 * @param[in]		unsafe		If True, do not lock mutex
	 * @return	Pointer to trap handler or NULL of not found
	 */
	R14PTrapHandler* get_trap(TrapId* trap_id, bool unsafe);

	/**
	 * Get trap value
	 * @param[in,out]	trap_id		Trap id to match and update
	 * @return	Last trap value
	 */
	uint64_t get_trap_value(TrapId* trap_id);

	/**
	 * Data acquisition thread method
	 * @param[in]	args	Pointer to R14PStatsSession
	 * @return	NULL
	 */
	static void* trap_loop(void* args);

	/**
	 * Start stats session
	 */
	void start();

	/**
	 * Stop stats session
	 */
	void stop();

	/**
	 * Set required events
	 * @param[in]	_client	Pointer to client
	 */
	void setup_client(r14p::R14PClient* _client);

	/**
	 * Get R14P stats session
	 */
	r14p::R14PSession* get_r14p_session();

	/**
	 * Lock mutex
	 */
	void lock();

	/**
	 * Unlock mutex
	 */
	void unlock();

	void init_r14p_session_stats(R14PSession* _r14ps);

	std::map<TrapId, R14PTrapHandler*, TrapIdCompare>* get_trap_map();	/**< Trap map */
	int poll_interval;							/**< Data acquisition interval in seconds */

    private:
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
	 * Get number of active threads (server thread only for now)
	 * @return	Number of active threads
	 */
	unsigned int get_thread_count();

	/**
	 * Set connection activity status
	 * @param[in]	_is_active	Connection activity flag
	 */

	void set_activity(bool _is_active);

	/**
	 * Get connection activity status
	 * @return	True if connection is active or False otherwise
	 */
	bool is_active();

	pthread_mutex_t mtx_stats;						/**< Stats mutex */
	bool active;								/**< Activity flag */
	int thread_count;							/**< Active thread count */
	int stats_port;								/**< Stats inbound port */
	r14p::R14PSession* r14ps;						/**< Pointer to R14P stats session */
	r14p::R14PSession* host_r14ps;						/**< Pointer to R14P host session */
	std::map<TrapId, R14PTrapHandler*, TrapIdCompare> trap_map;		/**< Trap map */
	TrapClientNew new_client;						/**< New client event */
	TrapClientDone client_done;						/**< Client terminated event */
	R14PStatsClientCreated client_created;					/**< Client creted event */
	R14PStatsClientDestroyed client_destroyed;				/**< Client destroyed event */

    };


}


#endif /* R14P_STATS_H_ */
