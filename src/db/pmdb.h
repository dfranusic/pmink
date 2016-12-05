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


#ifndef PMDB_H_
#define PMDB_H_

#include <mysql_driver.h>
#include <pool.h>
#include <ring.h>
#include <semaphore.h>
#include <pmink_utils.h>

namespace db{

    enum DBRecordType {
	DB_RT_SMS	= 0,
	DB_RT_SRI_SM	= 1,
	DB_RT_COMBINED	= 2
    };

    class DBRecord {
    public:
	DBRecord();
	virtual ~DBRecord();
	// type
	DBRecordType type;
	// params
	pmink_utils::PooledVPMap<uint32_t> params;
	// pointer to params (local or remote)
	//pmink_utils::PooledVPMap<uint32_t>* params_p;


	void reset();
	//void set_params(pmink_utils::PooledVPMap<uint32_t>* _params);


    };


    class DBManager;

    class RecordProcessor{
    public:
	RecordProcessor(DBRecordType _type, DBManager* _dbm);
	virtual ~RecordProcessor();
	virtual int run(std::vector<DBRecord*>* data) = 0;

	DBRecordType type;
	DBManager* dbm;

    };


    class SMSRecordProcessor: public RecordProcessor{
    public:
	SMSRecordProcessor(DBManager* dbm);
	int run(std::vector<DBRecord*>* data);


    };


    class CombinedRecordProcessor: public RecordProcessor{
    public:
	enum StatsType{
	    CRPST_SMS	= 0,
	    CRPST_SRI	= 1
	};

	CombinedRecordProcessor(DBManager* dbm);
	int run(std::vector<DBRecord*>* data);

	int process_sms(std::vector<DBRecord*>* data, int component_count);
	int process_sri(std::vector<DBRecord*>* data, int component_count);

	// stats
	pmink_utils::StatsManager stats;

    };




    class DBManager {
    public:
	DBManager();
	~DBManager();


	int connect(const char* _username = NULL,
		    const char* _password = NULL,
		    const char* _ip_addr = NULL,
		    int _port = 0,
		    const char* _db_name = NULL);

	void init(unsigned int _batch_size, int _pool_size = -1);



	int push(DBRecord* chunk);

	memory::Pool<DBRecord, true> pool_dbr;
	pmink::RingBuffer<DBRecord*> queue;

	void register_rcp(RecordProcessor* rcp);

	sql::Connection* get_conn();

    private:
	DBRecord* pop();
	static void* db_loop(void* args);

	std::map<DBRecordType, RecordProcessor*> rcp_map;
	std::string username;
	std::string password;
	std::string address;
	int port;
	std::string db_name;
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* conn;
	unsigned int batch_size;
	sem_t signal;
	pmink_utils::ThreadManager thm;

    };



}



#endif /* PMDB_H_ */
