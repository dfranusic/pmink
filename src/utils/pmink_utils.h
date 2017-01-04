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

#ifndef PMINK_UTILS_H_
#define PMINK_UTILS_H_

#include <time.h>
#include <string>
#include <dirent.h>
#include <curses.h>
#include <inttypes.h>
#include <map>
#include <atomic.h>
#include <vector>
#include <sstream>
#include <spscq.h>
#include <pool.h>
#include <ext/mt_allocator.h>
#include <string.h>
#include <new>
#include <iomanip>
#include <boost/unordered_map.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/crc.hpp>
#include <limits>

// pmink debug macro
#ifdef PMDEBUG
#define PMDLOG(x) x
#else
#define PMDLOG(x)
#endif

namespace pmink_utils {
    // debug map
    std::map<uint32_t, std::string>* create_debug_label_map();

    template<typename ID_TYPE, typename DATA_TYPE>
    class FloodManager{
    public:
	typedef std::multimap<time_t, DATA_TYPE> sub_list_type_t;
	typedef typename sub_list_type_t::iterator sub_list_it_t;
	typedef typename sub_list_type_t::value_type sub_list_value_t;


	typedef std::map<ID_TYPE, sub_list_type_t> list_type_t;
	typedef typename list_type_t::iterator list_it_t;
	typedef typename list_type_t::value_type list_value_t;
	typedef std::pair<list_it_t, bool> list_insert_res_t;

	FloodManager(uint32_t _max_interval = 3600, uint_fast32_t _max_size = 1000000000): max_interval(_max_interval), max_size(_max_size){
	    if(max_interval == 0) ++max_interval;

	}

	list_it_t add_list(ID_TYPE id){
	    // add to lists map
	    list_insert_res_t in_it = lists.insert(list_value_t(id, sub_list_type_t()));
	    // return iterator
	    return in_it.first;

	}

	void clear(){
	    lists.clear();
	}

	void set_max_interval(uint32_t _max_interval){
	    if(_max_interval == 0) ++_max_interval;
	    max_interval = _max_interval;
	}

	void set_max_size(uint32_t _max_size){
	    max_size = _max_size;
	}


	bool remove_list(ID_TYPE id){
	    list_it_t it = lists.find(id);
	    if(it != lists.end()){
		lists.erase(it);
		return true;
	    }
	    return false;
	}

	list_it_t get_list(ID_TYPE id){
	    return lists.find(id);
	}


	void expire(sub_list_type_t& flood_lst){
	    // get min timestamp
	    time_t ts = time(NULL) - max_interval;
	    // get lower bound
	    sub_list_it_t low_it = flood_lst.lower_bound(ts);
	    // remove
	    flood_lst.erase(flood_lst.begin(), low_it);

	}

	void expire(ID_TYPE list_id){
	    list_it_t lst_it = get_list(list_id);
	    if(lst_it == lists.end()) return;
	    expire(lst_it->second);

	}


	DATA_TYPE* add_item(ID_TYPE list_id, DATA_TYPE item){
	    // find list
	    list_it_t list_it = lists.find(list_id);
	    if(list_it == lists.end()) return NULL;
	    // sub list ref
	    sub_list_type_t& sub_ref = list_it->second;
	    // check size and remove oldest if full
	    if(sub_ref.size() >= max_size) sub_ref.erase(sub_ref.begin());
	    // add item to sub list
	    sub_list_it_t sub_it = sub_ref.insert(sub_list_value_t(time(NULL), item));
	    // return pointer to element in sub list
	    return &sub_it->second;
	}

	DATA_TYPE* add_item(list_it_t list_it, DATA_TYPE item){
	    if(list_it == lists.end()) return NULL;
	    // sub list ref
	    sub_list_type_t& sub_ref = list_it->second;
	    // check size and remove oldest if full
	    if(sub_ref.size() >= max_size) sub_ref.erase(sub_ref.begin());
	    // add item to sub list
	    sub_list_it_t sub_it = sub_ref.insert(sub_list_value_t(time(NULL), item));
	    // return pointer to element in sub list
	    return &sub_it->second;
	}


	bool del_item(ID_TYPE list_id, DATA_TYPE item){
	    // find list
	    list_it_t list_it = lists.find(list_id);
	    if(list_it == lists.end()) return false;
	    // sublist ref
	    sub_list_type_t& sub_ref = list_it->second;
	    // remove sub items that match
	    for(sub_list_it_t sub_it = sub_ref.begin(), sub_next = sub_it; sub_it != sub_ref.end(); sub_it = sub_next){
		// next
		++sub_next;
		if(sub_it->second == item) sub_ref.erase(sub_it);
	    }

	    // ok
	    return true;
	}


	uint32_t get_item_count(ID_TYPE list_id, DATA_TYPE item, uint32_t interval){
	    // find list
	    list_it_t list_it = lists.find(list_id);
	    if(list_it == lists.end()) return 0;
	    // sub list ref
	    sub_list_type_t& sub_ref = list_it->second;
	    // get ts
	    time_t ts = time(NULL) - interval;
	    // get lower bound
	    sub_list_it_t low_it = sub_ref.lower_bound(ts);
	    // sanity check
	    if(low_it == sub_ref.end()) return 0;
	    // count
	    int resc = 0;
	    for(; low_it != sub_ref.end(); low_it++) if(low_it->second == item) ++resc;
	    // return count
	    return resc;


	}
    private:
	list_type_t lists;
	uint32_t max_interval;
	uint32_t max_size;
    };


    template<typename ID_TYPE, typename DATA_TYPE>
    class ListManager{
    public:
	typedef std::vector<DATA_TYPE> sub_list_type_t;
	typedef typename sub_list_type_t::iterator sub_list_it_t;
	typedef std::map<ID_TYPE, sub_list_type_t> list_type_t;
	typedef typename list_type_t::iterator list_it_t;
	typedef typename list_type_t::value_type list_value_t;
	typedef std::pair<list_it_t, bool> list_insert_res_t;

	ListManager(uint32_t _max_size = 1000000000): max_size(_max_size){}

	list_it_t add_list(ID_TYPE id){
	    // add to lists map
	    list_insert_res_t in_it = lists.insert(list_value_t(id, sub_list_type_t()));
	    // return iterator
	    return in_it.first;

	}

	void clear(){
	    lists.clear();
	}

	void set_max_size(uint32_t _max_size){
	    max_size = _max_size;
	}

	bool remove_list(ID_TYPE id){
	    list_it_t it = lists.find(id);
	    if(it != lists.end()){
		lists.erase(it);
		return true;
	    }
	    return false;
	}

	list_it_t get_list(ID_TYPE id){
	    return lists.find(id);
	}

	int get_sublist_size(ID_TYPE id){
	    list_it_t it = lists.find(id);
	    if(it == lists.end()) return 0;
	    return it->second.size();
	}


	DATA_TYPE* add_sublist_item(ID_TYPE list_id, DATA_TYPE item){
	    // find list
	    list_it_t list_it = lists.find(list_id);
	    if(list_it == lists.end()) return NULL;
	    // cehck max
	    if(list_it->second.size() >= max_size) return NULL;
	    // add item to sub list
	    sub_list_it_t sub_it = list_it->second.insert(list_it->second.end(), item);
	    // return pointer to element in sub list
	    return &(*sub_it);
	}

	DATA_TYPE* add_sublist_item(list_it_t list_it, DATA_TYPE item){
	    if(list_it == lists.end()) return NULL;
	    // cehck max
	    if(list_it->second.size() >= max_size) return NULL;
	    // add item to sub list
	    sub_list_it_t sub_it = list_it->second.insert(list_it->second.end(), item);
	    // return pointer to element in sub list
	    return &(*sub_it);
	}


	bool del_sublist_item(ID_TYPE list_id, DATA_TYPE item){
	    // find list
	    list_it_t list_it = lists.find(list_id);
	    if(list_it == lists.end()) return false;
	    // sublist ref
	    sub_list_type_t& sub_vec = list_it->second;
	    // remove sub items that match
	    sub_vec.erase(std::remove(sub_vec.begin(), sub_vec.end(), item), sub_vec.end());
	    // ok
	    return true;
	}


	DATA_TYPE* get_sublist_item(ID_TYPE list_id, DATA_TYPE item){
	    // find list
	    list_it_t list_it = lists.find(list_id);
	    if(list_it == lists.end()) return NULL;
	    // sub list ref
	    sub_list_type_t& sub_ref = list_it->second;
	    // find item in sub list
	    for(sub_list_it_t sub_it = sub_ref.begin(); sub_it != sub_ref.end(); sub_it++){
		if(*sub_it == item) return &(*sub_it);
	    }
	    // not found
	    return NULL;


	}

	DATA_TYPE* get_sublist_item_by_index(ID_TYPE list_id, int index){
	    // find list
	    list_it_t list_it = lists.find(list_id);
	    if(list_it == lists.end()) return NULL;
	    // sub list ref
	    sub_list_type_t& sub_ref = list_it->second;
	    // boundry check
	    if(index >= sub_ref.size()) return NULL;
	    return &sub_ref[index];

	}


    private:
	list_type_t lists;
	uint32_t max_size;
    };



/*
    template<typename DATA>
    class RefCounter{
    public:
	RefCounter(DATA* _data): data(_data){
	    ref.set(1);
	}

	int inc(){
	    return ref.add_fetch(1);
	}
	int dec(){
	    uint32_t r = ref.sub_fetch(1);
	    if(r == 0) {
		DATA* _tmp = data;
		data = NULL;
		delete _tmp;
	    }
	    return r;
	}
	int get(){
	    return ref.get();
	}

    private:
	pmink::Atomic<int> ref;
	DATA* data;
    };
*/

    template<typename ARG_ID, typename ARG_TYPE>
    class EventArgs{
    public:
	void add_arg(ARG_ID id, ARG_TYPE arg){
	    args[id] = arg;
	}

	ARG_TYPE* get_arg(ARG_ID id){
	    typename std::map<ARG_ID, ARG_TYPE>::iterator it = args.find(id);
	    if(it != args.end()) return &it->second;
	    return NULL;
	}

	void clear(){
	    args.clear();
	}

    private:
	std::map<ARG_ID, ARG_TYPE> args;
    };

    template<typename ARG_ID, typename ARG_TYPE>
    class EventHandler {
    public:
	EventHandler(){}
	virtual ~EventHandler(){}
	virtual void run(EventArgs<ARG_ID, ARG_TYPE>& args){}


    };

    class Mutex{
    public:
	Mutex();
	Mutex(const Mutex& o);

	~Mutex();


	void lock();
	void unlock();
	Mutex& operator=(const Mutex& other);

    private:
	pthread_mutex_t mtx;
    };

    class Spinlock{
    public:
	Spinlock();
	Spinlock(const Spinlock& o);
	~Spinlock();

	void lock();
	void unlock();
	Spinlock& operator=(const Spinlock& other);

    private:
	pthread_spinlock_t slock;

    };


    // fwd declaration
    class ThreadManager;


    template<typename EVENT_ID, typename ARG_ID, typename ARG_TYPE, bool TH_SAFE = true>
    class EventManager{
    public:
	typedef std::map<EVENT_ID, EventHandler<ARG_ID, ARG_TYPE>* > event_map_t;
	typedef typename event_map_t::iterator event_map_it_t;
	typedef EventHandler<ARG_ID, ARG_TYPE> event_handler_t;
	typedef EventArgs<ARG_ID, ARG_TYPE> event_args_t;

	void set_handler(EVENT_ID id, event_handler_t* handler){
	    if(TH_SAFE) mtx.lock();
	    event_map[id] = handler;
	    if(TH_SAFE) mtx.unlock();
	}

	void remove_handler(EVENT_ID id){
	    if(TH_SAFE) mtx.lock();
	    event_map_it_t it = event_map.find(id);
	    if(it != event_map.end()) event_map.erase(it);
	    if(TH_SAFE) mtx.unlock();

	}

	event_handler_t* get_handler(EVENT_ID id){
	    if(TH_SAFE) mtx.lock();
	    event_map_it_t it = event_map.find(id);
	    if(it != event_map.end()) {
		if(TH_SAFE) mtx.unlock();
		return it->second;
	    }
	    if(TH_SAFE) mtx.unlock();
	    return NULL;
	}

	void clear(){
	    if(TH_SAFE) mtx.lock();
	    event_map.clear();
	    if(TH_SAFE) mtx.unlock();
	}

	int run_handler(EVENT_ID id, event_args_t& args){
	    if(TH_SAFE) mtx.lock();
	    event_map_it_t it = event_map.find(id);
	    if(it == event_map.end()){
		if(TH_SAFE) mtx.unlock();
		return 1;
	    }
	    it->second->run(args);
	    if(TH_SAFE) mtx.unlock();
	    return 0;
	}

    private:
	event_map_t event_map;
	Mutex mtx;
    };



    template<typename DTYPE, bool MASTER = true>
    class MTMasterSlave{
	friend class MTMasterSlave<DTYPE, true>;
	friend class MTMasterSlave<DTYPE, false>;
    public:
	MTMasterSlave(MTMasterSlave<DTYPE, true>* _master = NULL): master(_master), is_master(MASTER){}

	bool sync(){
	    // sanity check
	    if(is_master || master == NULL) return false;
	    // check update counter
	    uint32_t master_c = master->updtc.get();
	    if(master_c == updtc.get()) return false;
	    // lock master
	    master->mtx.lock();
	    // copy data
	    data = master->data;
	    // unlock master
	    master->mtx.unlock();
	    // update local counter
	    updtc.set(master_c);
	    // ok
	    return true;

	}

	bool sync_locked(){
	    // sanity check
	    if(is_master || master == NULL) return false;
	    // check update counter
	    uint32_t master_c = master->updtc.get();
	    if(master_c == updtc.get()) return false;
	    // lock master
	    master->mtx.lock();
	    // copy data
	    data = master->data;
	    // update local counter
	    updtc.set(master_c);
	    // ok
	    return true;

	}

	void unlock(){
	    // unlock master
	    master->mtx.unlock();
	}

	void lock(){
	    // lock master
	    master->mtx.lock();
	}

	void set_master(MTMasterSlave<DTYPE, true>* _master){
	    master = _master;
	}

	MTMasterSlave<DTYPE, true>* get_master(){
	    return master;
	}

	void update_start(){
	    mtx.lock();
	}

	void update_end(){
	    mtx.unlock();
	    updtc.fetch_add(1);
	}

	bool can_sync(){
	    uint32_t master_c = master->updtc.get();
	    if(master_c != updtc.get()) return true;
	    return false;
	}

	DTYPE& operator*(){
	    return data;
	}

    private:
	pmink::Atomic<uint32_t> updtc;
	MTMasterSlave<DTYPE, true>* master;
	Mutex mtx;
	DTYPE data;
	bool is_master;
    };

    template<typename DTYPE>
    class WRRItem{
    public:
	WRRItem(DTYPE _item, const char* _id, uint32_t _w): item(_item), weight(_w), old_weight(_w){
	    strncpy(id, _id, 65);
	    id[64] = 0;
	}

	char id[65];
	DTYPE item;
	uint32_t weight;
	uint32_t old_weight;
    };

    template<typename DTYPE>
    class WRR{
    public:
	typedef std::vector<WRRItem<DTYPE> > items_map_t;
	typedef typename items_map_t::iterator items_map_it_t;
	typedef typename items_map_t::reverse_iterator items_map_rit_t;
	typedef typename items_map_t::value_type items_map_val_t;

	/*
	 * http://kb.linuxvirtualserver.org/wiki/Weighted_Round-Robin_Scheduling
	 *
	 */
	WRR(): w_gcd(0), w_max(0), index(-1), cw(0){}

	items_map_val_t* add_item(DTYPE item, const char* id, uint32_t weight){
	    items_map_it_t it = items.insert(items.end(), WRRItem<DTYPE>(item, id, weight));
	    recalc();
	    // return
	    return &(*it);
	}

	void recalc(){
	    // empty check
	    if(items.size() == 0) {
		w_gcd = 0;
		w_max = 0;
		return;
	    }

	    // calculate gcd
	    w_gcd = items[0].weight;
	    for(unsigned int i = 1; i<items.size(); i++) w_gcd = gcd(w_gcd, items[i].weight);

	    // find max weight
	    w_max = 0;
	    for(unsigned int i = 0; i<items.size(); i++) if(items[i].weight > w_max) w_max = items[i].weight;

	}

	items_map_val_t* run(){
	    /*
		Supposing that there is a server set S = {S0, S1, …, Sn-1};
		W(Si) indicates the weight of Si;
		i indicates the server selected last time, and i is initialized with -1;
		cw is the current weight in scheduling, and cw is initialized with zero;
		max(S) is the maximum weight of all the servers in S;
		gcd(S) is the greatest common divisor of all server weights in S;

		while (true) {
		    i = (i + 1) mod n;
		    if (i == 0) {
			cw = cw - gcd(S);
			if (cw <= 0) {
			    cw = max(S);
			    if (cw == 0)
			    return NULL;
			}
		    }
		    if (W(Si) >= cw)
			return Si;
		}
	     */
	    // empty check
	    if(items.size() <= 0) return NULL;
	    // size
	    int size = items.size();
	    // counter
	    int c = 0;
	    // run
	    while(true){
		index = (index + 1) % items.size();
		if(index == 0){
		    cw = cw - w_gcd;
		    if(cw <= 0){
			cw = w_max;
			// err
			if(cw == 0) return NULL;
		    }
		}
		// get item
		WRRItem<DTYPE>& item = items[index];
		// return item (we don't want zero weight items returned)
		if(item.weight > 0 && item.weight >= cw) return &item;
		// inc counter
		++c;
		// stop if no items match
		if(c >= size) return NULL;
	    }
	    // err
	    return NULL;
	}

	int enable(items_map_val_t* item){
	    if(item == NULL) return 1;
	    // skip id already enabled
	    if(item->weight > 0) return 2;
	    item->weight = item->old_weight;
	    recalc();
	    return 0;
	}

	int enable(const char* id){
	    if(id == NULL) return 1;
	    items_map_it_t it = items.begin();
	    int dc = 0;
	    while(it != items.end()){
		if(strcmp((*it).id, id) == 0){
		    WRRItem<DTYPE>& item = *it;
		    // skip if already enabled
		    if(item.weight == 0){
			item.weight = item.old_weight;

		    }
		    ++dc;
		}
		++it;
	    }
	    recalc();
	    if(dc > 0) return 0;
	    return 2;

	}

	int disable(items_map_val_t* item){
	    if(item == NULL) return 1;
	    // skip id already disabled
	    if(item->weight == 0) return 2;
	    item->old_weight = item->weight;
	    item->weight = 0;
	    recalc();
	    return 0;
	}

	int disable(const char* id){
	    if(id == NULL) return 1;
	    items_map_it_t it = items.begin();
	    int dc = 0;
	    while(it != items.end()){
		if(strcmp((*it).id, id) == 0){
		    WRRItem<DTYPE>& item = *it;
		    // skip if already disabled
		    if(item.weight > 0){
			item.old_weight = item.weight;
			item.weight = 0;

		    }
		    ++dc;
		}
		++it;
	    }
	    recalc();
	    if(dc > 0) return 0;
	    return 2;

	}

	void clear(){
	    items.clear();
	}

	int remove(uint32_t index){
	    if(index >= items.size()) return 1;
	    items.erase(items.begin() + index);
	    recalc();
	    return 0;
	}

	int remove(const char* id){
	    if(id == NULL) return 1;
	    items_map_it_t it = items.begin();
	    int dc = 0;
	    while(it != items.end()){
		if(strcmp((*it).id, id) == 0){
		    it = items.erase(it);
		    ++dc;
		}else ++it;
	    }
	    recalc();
	    if(dc > 0) return 0;
	    return 2;
	    /*
	    for(unsigned int i = 0; i<items.size(); i++){
		if(strcmp(items[i].id, id) == 0){
		    items.erase(items.begin() + i);
		    recalc();
		    return 0;

		}
	    }
	    return 2;
	    */
	}

	items_map_val_t* get(const char* id){
	    if(id == NULL) return NULL;
	    // loop
	    for(items_map_it_t it = items.begin(); it != items.end(); it++){
		// compare id
		if(strcmp((*it).id, id) == 0) return &(*it);
	    }
	    // err
	    return NULL;
	}

	items_map_it_t remove(items_map_it_t it){
	    items_map_it_t it_res = items.remove(it);
	    recalc();
	    return it_res;
	}

	items_map_it_t begin(){
	    return items.begin();
	}

	items_map_it_t end(){
	    return items.end();
	}
	size_t size(){
	    return items.size();
	}

    private:
	uint32_t gcd(uint32_t m, uint32_t n){
	    if(n == 0) return m;
	    return gcd(n, m % n);
	}

	items_map_t items;
	uint32_t w_gcd;
	uint32_t w_max;
	int index;
	int cw;

    };


	// fwd
	template<typename DTYPE, typename IN_DTYPE> class Rule;
	template<typename DTYPE, typename IN_DTYPE> class RuleManager;

	template<typename DTYPE, typename IN_DTYPE>
	class RuleLogic{
	public:
	    RuleLogic(){}//: next_rule(NULL){}
	    virtual ~RuleLogic(){}
	    virtual int run(Rule<DTYPE, IN_DTYPE>* rule, IN_DTYPE* input) = 0;

	    //Rule<DTYPE, IN_DTYPE>* next_rule;

	};

	template<typename DTYPE, typename IN_DTYPE>
	class Rule{
	public:
	    typedef typename std::map<uint32_t, Rule<DTYPE, IN_DTYPE> >::iterator rule_it_t;

	    Rule(): priority(0),
		    active(true),
		    logic(NULL),
		    rm(NULL){

	    }

	    Rule(const char* _label,
		 uint32_t _priority,
		 DTYPE _data,
		 RuleLogic<DTYPE, IN_DTYPE>* _logic,
		 RuleManager<DTYPE, IN_DTYPE>* _rm): priority(_priority),
						     label(_label),
						     data(_data),
						     active(true),
						     logic(_logic),
						     rm(_rm){

	    }

	    ~Rule(){

	    }

	    DTYPE get_data() const{
		return data;
	    }

	    DTYPE* get_data_p() {
		return &data;
	    }


	    void set_data(DTYPE _data){
		data = _data;
	    }

	    uint32_t get_priority() const{
		return priority;
	    }

	    void set_priority(uint32_t _priority){
		priority = _priority;
	    }

	    const char* get_label() const{
		return label.c_str();
	    }

	    void set_label(const char* _label){
		label.assign(_label);

	    }

	    bool is_active(){
		return active;
	    }

	    void set_activity(bool status){
		active = status;
	    }

	    RuleLogic<DTYPE, IN_DTYPE>* get_logic() const{
		return logic;
	    }

	    RuleManager<DTYPE, IN_DTYPE>* get_rm(){
		return rm;
	    }

	    void set_position(rule_it_t pos){
		position = pos;
	    }

	    rule_it_t get_position(){
		return position;
	    }


	private:
	    uint32_t priority;
	    std::string label;
	    DTYPE data;
	    bool active;
	    RuleLogic<DTYPE, IN_DTYPE>* logic;
	    RuleManager<DTYPE, IN_DTYPE>* rm;
	    rule_it_t position;
	};


	enum RuleResultType{
	    RRT_STOP_OK			= 0,
	    RRT_STOP_ERR		= 1,
	    RRT_CONTINUE		= 2,
	    RRT_CONTINUE_PAUSE		= 3,
	    RRT_STOP_INF_LOOP		= 9999,
	    RRT_STOP_UNKNOWN_RES	= 10000
	};

	template<typename DTYPE, typename IN_DTYPE>
	class RuleManager{
	public:

	    // label comparison functor
	    class label_comp{
	    public:
		bool operator()(const char* a, const char* b){
		    return strcmp(a, b) < 0;
		}

	    };


	    // types
	    typedef std::map<uint32_t, Rule<DTYPE, IN_DTYPE> > rules_map_t;
	    typedef typename rules_map_t::iterator rules_map_it_t;
	    typedef std::map<const char*, Rule<DTYPE, IN_DTYPE>*, label_comp> label_map_t;
	    typedef typename label_map_t::iterator label_map_it_t;
	    typedef std::pair<rules_map_it_t, bool> rules_insert_res_t;
	    typedef typename rules_map_t::value_type rules_map_value_t;

	    RuleManager(): loop_max(1000000), next_rule(NULL){
		//pthread_mutex_init(&_lock, NULL);
	    }
	    ~RuleManager(){
		//pthread_mutex_destroy(&_lock);
	    }

	    void clear(){
		labels.clear();
		rules.clear();
	    }

	    rules_map_it_t get_begin(){
		return rules.begin();
	    }

	    rules_map_it_t get_end(){
		return rules.end();
	    }


	    RuleManager& operator=(const RuleManager& other) {
		labels.clear();
		rules.clear();
		// loop other rules
		for(typename rules_map_t::const_iterator it = other.rules.begin(); it != other.rules.end(); it++){
		    // other rule ref
		    const Rule<DTYPE, IN_DTYPE>& rule = it->second;
		    // create rule with data from other rule
		    new_rule(rule.get_label(), rule.get_priority(), rule.get_data(), rule.get_logic());

		}
		return *this;
	    }


	    int process_rules(IN_DTYPE* input, Rule<DTYPE, IN_DTYPE>** last_rule = NULL, Rule<DTYPE, IN_DTYPE>* first_rule = NULL){
		RuleLogic<DTYPE, IN_DTYPE>* logic;
		uint32_t lc = 0;
		int res = RRT_STOP_UNKNOWN_RES;
		// first rule is first on in list
		rules_map_it_t it = rules.begin();
		// override first rule if argument was set
		if(first_rule != NULL) it = first_rule->get_position();
		// loop rules
		for(; it != rules.end(); ){
		    // rule ref
		    Rule<DTYPE, IN_DTYPE>& rule = it->second;
		    // get logic
		    logic = rule.get_logic();
		    // run logic
		    res = logic->run(&rule, input);
		    // set last rule
		    if(last_rule != NULL) *last_rule = &rule;
		    // check if jumping or continuing
		    //if(logic->next_rule != NULL) it = logic->next_rule->get_position(); else it++;
		    if(next_rule != NULL) it = next_rule->get_position(); else it++;
		    // loop counter
		    ++lc;
		    // check for potential inifinite loop due to jumping
		    if(lc > loop_max) return RRT_STOP_INF_LOOP;
		    // check logic result
		    switch(res){
			// ok/err/pause, stop processing
			case RRT_STOP_OK:
			case RRT_STOP_ERR:
			case RRT_CONTINUE_PAUSE: return res;
			// Continue
			case RRT_CONTINUE: break;
			// unknown rule logic result type
			default: return RRT_STOP_UNKNOWN_RES;
		    }

		}
		// last result
		return res;
	    }

	    Rule<DTYPE, IN_DTYPE>* get_rule(const char* label){
		// check if label already present
		label_map_it_t it = labels.find(label);
		if(it == labels.end()) return NULL;
		// result
		return it->second;

	    }

	    Rule<DTYPE, IN_DTYPE>* get_rule(uint32_t priority){
		rules_map_it_t it = rules.find(priority);
		if(it == rules.end()) return NULL;
		// result
		return &it->second;

	    }

	    bool remove_rule(uint32_t priority){
		// check in priority list
		rules_map_it_t it = rules.find(priority);
		if(it == rules.end()) return false;

		// check in label list
		label_map_it_t it2 = labels.find(it->second.get_label());
		if(it2 == labels.end()) return false;

		// remove
		labels.erase(it2);
		rules.erase(it);
		// ok
		return true;

	    }

	    bool remove_rule(const char* label){
		// check in label list
		label_map_it_t it = labels.find(label);
		if(it == labels.end()) return false;

		// check in priority list
		rules_map_it_t it2 = rules.find(it->second->get_priority());
		if(it2 == rules.end()) return false;

		// remove
		labels.erase(it);
		rules.erase(it2);
		// ok
		return true;


	    }


	    Rule<DTYPE, IN_DTYPE>* new_rule(const char* label, uint32_t priority, DTYPE data, RuleLogic<DTYPE, IN_DTYPE>* logic){
		// check for logic
		if(logic == NULL) return NULL;
		// check if label already present
		label_map_it_t it = labels.find(label);
		if(it != labels.end()) return NULL;
		// check if priority already present
		rules_map_it_t it2 = rules.find(priority);
		if(it2 != rules.end()) return NULL;

		// create new
		Rule<DTYPE, IN_DTYPE> rule(label, priority, data, logic, this);

		// add to rule map
		rules_insert_res_t in_it = rules.insert(rules_map_value_t(priority, rule));
		// set rule position in rules map
		in_it.first->second.set_position(in_it.first);

		// add to label map
		labels[in_it.first->second.get_label()] = &in_it.first->second;

		// result
		return &in_it.first->second;


	    }


	    void lock(){
		//pthread_mutex_lock(&_lock);
		_lock.lock();
	    }

	    void unlock(){
		//pthread_mutex_unlock(&_lock);
		_lock.unlock();
	    }

	    void set_loop_max(uint32_t _loop_max){
		loop_max = _loop_max;
	    }

	    void set_next_rule(Rule<DTYPE, IN_DTYPE>* next){
		next_rule = next;
	    }

	    Rule<DTYPE, IN_DTYPE>* get_next_rule(){
		return next_rule;
	    }

	private:
	    rules_map_t rules;
	    label_map_t labels;
	    Mutex _lock;
	    //pthread_mutex_t _lock;
	    uint32_t loop_max;
	    Rule<DTYPE, IN_DTYPE>* next_rule;


	};



	/**
	 * Thread info class
	 */
	class ThreadInfo {
	public:
	    /**
	     * Default constructor
	     */
	    ThreadInfo();

	    /**
	     * Get activity status
	     * @return	true if active or false otherwise
	     */
	    bool is_active();

	    char name[16];			/**< thread name */
	    pthread_t handle;			/**< thread handle */
	    pmink::Atomic<bool> active;		/**< thread activity flag */
	    void* user_args;			/**< user start routine args */
	    void *(*userstart_routine) (void *);/**< user start routine */
	    ThreadManager* thm;			/**< thread manager pointer */
	    uint16_t cpu_core;			/**< cpu core index */


	};

	/**
	 * Thread manager class
	 */
	class ThreadManager {
	public:
	    ThreadManager();	/**< Default constructor */
	    ~ThreadManager();	/**< Destructor */

	    /**
	     * Create new thread
	     * @param[in]	__start_routine	Pointer to thread start function
	     * @param[in]	__name		Thread name C string
	     * @param[in]	__attr		Thread attributes
	     * @param[in]	args		User arguments for thread start function
	     * @param[in]	cpu_core	CPU core id
	     * @return		Pointer to thread info or NULL if error occurred
	     */
	    ThreadInfo* new_thread(	void *(*__start_routine) (void *),
					const char* __name,
					void* args,
					pthread_attr_t* __attr = NULL,
					uint16_t cpu_core = 65535);

	    /**
	     * Set activity flag to false and return when all threads have finished
	     * if _set_only flag was not set
	     * @param[in]	_set_only	If true, do not wait for threads to finish
	     */
	    void stop(bool _set_only = false);

	    /**
	     * Set activity flag to true
	     */
	    void start();

	    /**
	     * Get thread activity flag
	     */
	    bool is_active();

	    /**
	     * Get number of active threads
	     */
	    uint16_t get_thread_count();

	    /**
	     * Get thread by index
	     * @param[in]	index
	     * @return		Pointer to thread info or NULL
	     */
	    ThreadInfo* get_thread(unsigned int index);

	    /**
	     * Set activity of specific thread to false and return immediately
	     * @param[in]	index
	     * @return		0 for success or error code
	     */
	    int stop_thread(unsigned int index);

	    /**
	     * Get number of CPU cores
	     */
	    uint16_t get_core_count();

	private:
	    /**
	     * Thread start routine (user start routine wrapper)
	     */
	    static void* start_wrapper(void* args);

	    /**
	     * Increment number of active threads
	     */
	    uint16_t inc();

	    /**
	     * Decrement number of active threads
	     */
	    uint16_t dec();

	    pmink::Atomic<uint16_t> th_count;	/**< thread count */
	    std::vector<ThreadInfo*> threads;	/**< thread list */
	    pmink::Atomic<bool> active;		/**< thread activity flag */
	    uint16_t core_count;		/**< number of cores */

	};

	// fwd
	uint64_t hash_fnv1a_64bit(const void* key, int len);

	template<typename ID_TYPE, typename DATA_TYPE>
	class CorrelationMap{
	public:
	    class DataWrapper{
	    public:
		DataWrapper():
		    ts(time(NULL)),
		    data_timeout(0){
		}

		DataWrapper(const DATA_TYPE& _data, uint32_t _timeout):
		    ts(time(NULL)),
		    data_timeout(_timeout),
		    data(_data){

		}
		time_t ts;
		uint32_t data_timeout;
		DATA_TYPE data;
	    };


	    // hashing functor
	    struct fnv1a_hash {
	        size_t operator()(const ID_TYPE& x) const { return hash_fnv1a_64bit(&x, sizeof(x)); }
	    };

	    // types
	    typedef boost::fast_pool_allocator<std::pair<const ID_TYPE, DataWrapper> > cmap_alloc_t;
	    typedef boost::unordered_map<ID_TYPE,
                                         DataWrapper,
                                         fnv1a_hash,
                                         std::equal_to<ID_TYPE>,
                                         cmap_alloc_t> cmap_type;

	    typedef typename cmap_type::iterator cmap_it_type;
	    typedef typename cmap_type::value_type cmap_value_type;
	    typedef std::pair<cmap_it_type, bool> cmap_insert_type;

	    CorrelationMap():
		data_timeout(10),
		max(std::numeric_limits<uint32_t>::max()){


	    }
	    CorrelationMap(uint32_t _max, uint32_t _data_timeout = 10):
		data_timeout(_data_timeout),
		max(_max){


	    }

	    void set_max_size(uint32_t _max){
		max = _max;
	    }

	    DATA_TYPE* get(const ID_TYPE& id){
		// find
		cmap_it_type it = data_map.find(id);
		// check result, return
		if(it == data_map.end()) return NULL;
		return &it->second.data;
	    }

	    cmap_it_type get_it(const ID_TYPE& id){
		return data_map.find(id);
	    }

	    cmap_it_type set(const ID_TYPE& id, const DATA_TYPE& data, uint32_t _timeout = 0){
		// check max
		if(data_map.size() >= max) return data_map.end();
		// insert
		cmap_insert_type it = data_map.insert(cmap_value_type(id, DataWrapper(data, (_timeout == 0 ? data_timeout : _timeout))));
		// return iterator
		return it.first;

	    }

	    void set_timeout(uint32_t _timeout){
		data_timeout = _timeout;
	    }

	    uint32_t get_timeout(){
		return data_timeout;
	    }

	    bool remove(const ID_TYPE& id){
		if(data_map.erase(id) > 0) return true;
		return false;
	    }

	    void remove(cmap_it_type it){
		data_map.erase(it);
	    }

	    bool update_ts(cmap_it_type it){
		// update ts
		it->second.ts = time(NULL);
		// ok
		return true;
	    }

            time_t get_ts(const ID_TYPE& id){
                // find
		cmap_it_type it = data_map.find(id);
		// check result, return
		if(it == data_map.end()) return 0;
		return it->second.ts;
            }

	    bool update_timeout(cmap_it_type it, uint32_t timeout){
		// update ts
		it->second.data_timeout = timeout;
		// ok
		return true;
	    }


	    bool update_ts(const ID_TYPE& id){
		// find
		cmap_it_type it = data_map.find(id);
		// check result, return
		if(it == data_map.end()) return false;
		// update ts
		it->second.ts = time(NULL);
		// return ok
		return true;
	    }

	    bool update_timeout(const ID_TYPE& id, uint32_t timeout){
		// find
		cmap_it_type it = data_map.find(id);
		// check result, return
		if(it == data_map.end()) return false;
		// update timeout
		it->second.data_timeout = timeout;
		// return ok
		return true;
	    }



	    void expire(std::vector<DATA_TYPE>& out){
		// res clear
		out.clear();
		// get ts
		time_t ts = time(NULL);
		// iterate
		for(cmap_it_type it = data_map.begin(), it_next = it; it != data_map.end(); it = it_next){
		    // next
		    ++it_next;
		    // check if expired
		    if(ts - it->second.ts > it->second.data_timeout){
			// copy data part
			out.push_back(DATA_TYPE(it->second.data));
			// remove from map
			data_map.erase(it);
		    }
		}
	    }

	    void expire(){
		// get ts
		time_t ts = time(NULL);
		// iterate
		for(cmap_it_type it = data_map.begin(), it_next = it; it != data_map.end(); it = it_next){
		    // next
		    ++it_next;
		    // check if expired
		    if(ts - it->second.ts > it->second.data_timeout){
			// remove from map
			data_map.erase(it);
		    }
		}
	    }

	    cmap_it_type begin(){
		return data_map.begin();
	    }

	    cmap_it_type end(){
		return data_map.end();
	    }

            size_t size(){
                return data_map.size();
                
            }

	    void lock(){
		mtx.lock();
	    }

	    void unlock(){
		mtx.unlock();
	    }

	private:
	    uint32_t data_timeout;
	    cmap_type data_map;
	    Mutex mtx;
	    uint32_t max;

	};



	class RefCounted{
	public:
	    class _init{
		friend class RefCounted;
	    public:
		_init();

	    private:
		static void* dealloc_loop(void* args);

		std::vector<RefCounted*> ref_list;
		pmink_utils::Mutex mtx;
		pmink::Atomic<uint32_t> interval;
		pmink_utils::ThreadManager thm;
	    };

	    RefCounted();
	    virtual ~RefCounted();

	    int refc_inc(uint32_t incby = 1);
	    int refc_dec();
	    int refc_get();
	    virtual bool refc_condition();
	    static void set_interval(uint32_t _interval);
	    static void init();
	    static size_t get_item_count();


	private:
	    pmink::Atomic<int> ref;
	    __attribute__ ((visibility ("hidden"))) static _init _initializer;

	};

	/**
	 * General parameter map
	 * @tparam	TID	Parameter id typename
	 * @tparam	T	Parameter typename
	 * @tparam	THSAFE	Thread safe flag
	 */
	template<typename TID, typename T, bool THSAFE = false>
	class ParameterMap {
	public:
	    /**
	     * Constructor
	     */
	    ParameterMap(){
		//if(THSAFE) pthread_mutex_init(&mtx, NULL);
		if(THSAFE) pthread_rwlock_init(&_lock, NULL);
	    }

	    /**
	     * Destructor
	     */
	    ~ParameterMap(){
		//if(THSAFE) pthread_mutex_destroy(&mtx);
		if(THSAFE) pthread_rwlock_destroy(&_lock);
	    }

	    /**
	     * Get parameter
	     * @param[in]	param_id	Parameter id
	     * @return		Parameter or NULL if not found
	     */
	    T get_param(TID param_id){
		lock_rd();
		typename std::map<TID, T>::iterator it = params.find(param_id);
		T res = (it != params.end() ? it->second : NULL);
		unlock();
		return res;
	    }

	    /**
	     * Check if parameter exists
	     * @param[in]	param_id	Parameter id
	     * @return		true if parameter exists or false otherwise
	     */
	    bool exists(TID param_id){
		lock_rd();
		typename std::map<TID, T>::iterator it = params.find(param_id);
		bool res = (it != params.end() ? true : false);
		unlock();
		return res;

	    }

	    /**
	     * Get paramter map
	     * @param[in]	Pointer to parameter map
	     */
	    std::map<TID, T>* get_param_map(){
		return &params;
	    }

	    /**
	     * Set parameter
	     * @param[in]	param_id	Parameter id
	     * @param[in]	param		Parameter
	     */
	    void set_param(TID param_id, T param){
		lock_wr();
		params[param_id] = param;
		unlock();

	    }

	    /**
	     * Remove parameter
	     * @param[in]	param_id	Parameter id
	     * @return		Number of erased parameters
	     */
	    int remove_param(TID param_id){
		lock_wr();
		int res = params.erase(param_id);
		unlock();
		return res;

	    }

	    /**
	     * Clear parameter map
	     */
	    void clear_params(){
		lock_wr();
		params.clear();
		unlock();
	    }

	private:
	    /**
	     * Read Lock
	     */
	    void lock_rd(){
		//if(THSAFE) pthread_mutex_lock(&mtx);
		if(THSAFE) pthread_rwlock_rdlock(&_lock);
	    }

	    /**
	     * Write Lock
	     */
	    void lock_wr(){
		//if(THSAFE) pthread_mutex_lock(&mtx);
		if(THSAFE) pthread_rwlock_wrlock(&_lock);
	    }



	    /**
	     * Unlock mutex
	     */
	    void unlock(){
		//if(THSAFE) pthread_mutex_unlock(&mtx);
		if(THSAFE) pthread_rwlock_unlock(&_lock);

	    }

	    std::map<TID, T> params;	/**< Parameter map */
	    //pthread_mutex_t mtx;	/**< Mutex */
	    pthread_rwlock_t _lock;	/**< RW lock */

	};

	/**
	 * Variant data type
	 */
	enum VariantParamType{
	    DPT_INT	= 1,
	    DPT_STRING 	= 2,
	    DPT_DOUBLE	= 3,
	    DPT_CHAR	= 4,
	    DPT_BOOL	= 5,
	    DPT_OCTETS	= 6,
	    DPT_POINTER	= 7
	};

	/**
	 * Variant param union
	 */
	union UVariantParam{
	    int64_t i64;
	    double d;
	    char c;
	    bool b;
	    void* p;
	    char* str;//[MAX];

	};

	/**
	 * Variant param
	 */
	class VariantParam{
	public:
	    /**
	     * Types
	     */
	    typedef __gnu_cxx::__mt_alloc<char> param_alloc_t;

	    /**
	     * Constructor
	     * @param	_type	Variant data type
	     */
	    VariantParam(VariantParamType _type, param_alloc_t* _palloc, unsigned int _max): max(_max),
											     type(_type),
											     data_size(0),
											     palloc(_palloc),
											     palloc_p(NULL){

	    }

	    /**
	     * Destructor
	     */
	    ~VariantParam(){
		if(palloc_p != NULL) {
		    palloc->deallocate(palloc_p, data_size);
		    palloc_p = NULL;
		}

	    }

            /**
             * Equality operator
             */
            bool operator==(const VariantParam& other) const {
                if(this->type != other.type) return false;    
                if(this->data_size != other.data_size) return false;
                switch(this->type){
                    case DPT_INT:
                        return this->data.i64 == other.data.i64;

                    case DPT_STRING:
                        return (strcmp(this->data.str, other.data.str) == 0);

                    case DPT_DOUBLE:
                        return this->data.d == other.data.d;

                    case DPT_CHAR:
                        return this->data.c == other.data.c;

                    case DPT_BOOL:
                        return this->data.b == other.data.b;

                    case DPT_OCTETS:
                        return (memcmp(this->data.str, other.data.str, this->data_size) == 0);

                    case DPT_POINTER:
                        return this->data.p == other.data.p;

                    default: break;
                }
                return false;
            }

            /**
             * Inequality operator
             */
            bool operator!=(const VariantParam& other) const {
                return !(*this == other);
            }

	    /**
	     * Int variant cast
	     */
	    operator int() const {
		if(type == DPT_INT) return data.i64;
		return 0;
	    }

	    /**
	     * Signed 64bit int variant cast
	     */
	    operator int64_t() const {
		if(type == DPT_INT) return data.i64;
		return 0;
	    }


	    /**
	     * Unsigned 64bit int variant cast
	     */
	    operator uint64_t() const {
		if(type == DPT_INT) return data.i64;
		return 0;
	    }

	    /**
	     * Unsigned 32bit int variant cast
	     */
	    operator uint32_t() const {
		if(type == DPT_INT) return data.i64;
		return 0;
	    }

	    /**
	     * Bool variant cast
	     */
	    operator bool() const {
		if(type == DPT_BOOL) return data.b;
		return false;
	    }

	    /**
	     * C style string variant cast
	     */
	    operator char*() const {
		if(type == DPT_STRING) return data.str;
		return NULL;
	    }

	    /**
	     * Octets variant cast
	     */
	    operator unsigned char*() const {
		if(type == DPT_OCTETS) return (unsigned char*)data.str;
		return NULL;
	    }

	    /**
	     * Double variant cast
	     */
	    operator double() const {
		if(type == DPT_DOUBLE) return data.d;
		return 0;
	    }

	    /**
	     * Char variant cast
	     */
	    operator char() const {
		if(type == DPT_CHAR) return data.c;
		return 0;
	    }

	    /**
	     * Pointer variant cast
	     */
	    operator void*() const {
		if(type == DPT_POINTER) return data.p;
		return 0;
	    }

	    /**
	     * Set Int variant data
	     * @param[in]	_data	Int data
	     */
	    void set_int(int64_t _data){
		if(type == DPT_INT){
		    data.i64 = _data;
		    data_size = sizeof(data.i64);
		}
	    }

	    /**
	     * Set bool variant data
	     * @param[in]	_data	bool data
	     */
	    void set_bool(bool _data){
		if(type == DPT_BOOL){
		    data.b = _data;
		    data_size = sizeof(data.b);
		}
	    }

	    /**
	     * Set C style string variant data
	     * @param[in]	_data	C string data
	     */
	    void set_str(const char* _data){
		if(type == DPT_STRING){
		    // get string length
		    size_t slen = strlen(_data);
		    // check for buffer overflow
		    unsigned int csize = (slen >= max ? (max - 1) : slen);
		    // allocate
		    if(palloc_p != NULL) palloc->deallocate(palloc_p, data_size);
		    palloc_p = palloc->allocate(csize + 1);
		    data.str = new(palloc_p) char[csize + 1];
		    // copy data
		    memcpy(data.str, _data, csize);
		    // set null termination
		    data.str[csize] = 0;
		    // set size
		    data_size = csize + 1;
		}
	    }

	    /**
	     * Set char variant data
	     * @param[in]	_data	char data
	     */
	    void set_char(char _data){
		if(type == DPT_CHAR){
		    data.c = _data;
		    data_size = sizeof(data.c);

		}
	    }

	    /**
	     * Set double variant data
	     * @param[in]	_data	double data
	     */
	    void set_double(double _data){
		if(type == DPT_DOUBLE){
		    data.d = _data;
		    data_size = sizeof(data.d);
		}
	    }

	    /**
	     * Set octets variant data
	     * @param[in]	_data		octets data
	     * @param[in]	_data_size	data size
	     */
	    void set_octets(const void* _data, unsigned int _data_size){
		if(type == DPT_OCTETS){
		    // check for buffer overflow
		    unsigned int csize = (_data_size > max ? max : _data_size);
		    // allocate
		    if(palloc_p != NULL) palloc->deallocate(palloc_p, data_size);
		    palloc_p = palloc->allocate(csize);
		    data.str = new(palloc_p) char[csize];
		    // copy data
		    memcpy(data.str, _data, csize);
		    // set size
		    data_size = csize;
		}
	    }

	    /**
	     * Set pointer variant data
	     * @param[in]	_data	double data
	     */
	    void set_pointer(void* _data){
		if(type == DPT_POINTER){
		    data.p = _data;
		    data_size = sizeof(void*);
		}
	    }

	    /**
	     * Get current data size in bytes
	     * @return		Data size in bytes
	     */
	    unsigned int get_size() const{
		return data_size;
	    }

	    /**
	     * Get parameter type
	     * @return		Parameter type
	     */
	    VariantParamType get_type() const{
		return type;
	    }

	    /**
	     * Set parameter type
	     */
	    void set_type(VariantParamType _type){
		type = _type;
	    }

	    /**
	     * Get variant data pointer
	     */
	    const UVariantParam* get_data() const{
		return &data;
	    }

	    /**
	     * Output operator<< for VariantParam
	     *
	     */
	    friend std::ostream& operator<< (std::ostream& out, const VariantParam& param){
		switch(param.type){
		    case DPT_INT:
			out << "(I) " << (int)param;
			break;

		    case DPT_OCTETS:
		    {
                        out << "(O) ";

			unsigned char* cdata = (unsigned char*)param;
			for(unsigned int k = 0; k<param.data_size; k++){
			    out << std::setfill('0') << std::setw(2) << std::hex << (int)(cdata[k] & 0xff)<< " ";
			}
			out << std::dec;

			break;
		    }

		    case DPT_STRING:
			out << "(S) " << (char*)param;
			break;

		    case DPT_BOOL:
			out << "(B) " << ((bool)param ? "true" : "false");
			break;

		    case DPT_CHAR:
			out << "(C)" << "'" << (char)param << "'";
			break;

		    case DPT_DOUBLE:
			out << "(D) " << (double)param;
			break;

		    case DPT_POINTER:
			out << "(P) " << (void*)param;
			break;

		}
		return out;
	    }

	private:
	    //UVariantParam<MAX> data;
	    unsigned int max;
	    UVariantParam data;
	    VariantParamType type;
	    unsigned int data_size;
	    param_alloc_t* palloc;
	    param_alloc_t::pointer palloc_p;

	};


	template<typename TID = uint32_t>
	class ParamTuple {
	public:
	    ParamTuple(TID _key, uint32_t _index = 0, uint32_t _fragment = 0, uint32_t _context = 0): key(_key),
												      index(_index),
												      fragment(_fragment),
												      context(_context){}
	    TID key;
	    uint32_t index;
	    uint32_t fragment;
	    uint32_t context;

	    bool operator != (const ParamTuple& right) const {
		if(this->key == right.key) return false;
		if(this->index == right.index) return false;
		if(this->fragment == right.fragment) return false;
		if(this->context == right.context) return false;
		return true;

	    }
	    bool operator == (const ParamTuple& right) const {
		if(this->key != right.key) return false;
		if(this->index != right.index) return false;
		if(this->fragment != right.fragment) return false;
		if(this->context != right.context) return false;
		return true;
	    }

	    /**
	     * Standard output operator
	     */
	    friend ostream& operator<<(ostream& out, const ParamTuple& pt){
		out << "[" << pt.context << "] " << pt.key << "." << pt.index << "." << pt.fragment;
		return out;

	    }

	};


	template<typename TID>
	class ParamTupleCompare{
	public:
	    bool operator ()(const ParamTuple<TID>& x, const ParamTuple<TID>& y){
		if(x.key < y.key) return true;
		else if(x.key == y.key){
		    if(x.index < y.index) return true;
		    else if(x.index == y.index){
			if(x.fragment < y.fragment) return true;
			else if(x.fragment == y.fragment){
			    if(x.context < y.context) return true;
			}
		    }

		}

		return false;
		//return x.key < y.key || (x.index + x.key + x.fragment) < (y.index + y.key + y.fragment);
	    }

	};

	/**
	 * Variant param map
	 * @param[in]	TID		key type
	 * @param[in]	_Alloc		param map allocator
	 */
	template<typename TID,
		 typename _Alloc = std::allocator<std::pair<const ParamTuple<TID>, VariantParam > > >
	class VariantParamMap {
	public:
	    /**
	     * Types
	     */
	    typedef std::map<ParamTuple<TID>, VariantParam, ParamTupleCompare<TID>, _Alloc> pmap_t;
	    typedef typename pmap_t::value_type pmap_value_t;
	    typedef typename pmap_t::iterator it_t;
	    typedef typename pmap_t::const_iterator cit_t;
	    typedef std::pair<it_t, bool> insert_res_t;
	    typedef __gnu_cxx::__pool_base::_Tune tune_t;
	    typedef __gnu_cxx::__mt_alloc<char> param_alloc_t;

	    /**
	     * Constructor
	     */
	    VariantParamMap(unsigned int MAX_STR_SIZE = 256, const _Alloc& alloc = _Alloc()): max(MAX_STR_SIZE),
											      params(ParamTupleCompare<TID>(), alloc){
		// set param allocator
		//int tmp = (int)pow((double)2, (int)ceil(log10(MAX_STR_SIZE) / log10(2)));
		//tune_t t_opt(16, tmp * 2 , 32, tmp * 10, 20, 10, false);
		tune_t t_opt(16, 2048, 32, 10240, 4096, 10, false);
		param_alloc._M_set_options(t_opt);
		labels_p = &labels;

	    }

	    /**
	     * Copy constructor
	     */
	    VariantParamMap(const VariantParamMap& o){
		VariantParamMap& other = const_cast<VariantParamMap&>(o);
		// set max
		max = other.max;
		// set param allocator
		//int tmp = (int)pow((double)2, (int)ceil(log10(other.max) / log10(2)));
		//tune_t t_opt(16, tmp, 32, tmp * 10, 20, 10, false);
		tune_t t_opt(16, 2048, 32, 10240, 4096, 10, false);
		param_alloc._M_set_options(t_opt);
		// copy labels
		labels = other.labels;
		labels_p = &labels;
		// loop other params
		for(it_t it = other.params.begin(); it != other.params.end(); it++){
		    // other param
		    VariantParam& vparam = it->second;
		    // create param with data from other param
		    switch(vparam.get_type()){
			case DPT_INT:
			    set_int(it->first.key, vparam.get_data()->i64, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_STRING:
			    set_cstr(it->first.key, vparam.get_data()->str, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_DOUBLE:
			    set_double(it->first.key, vparam.get_data()->d, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_CHAR:
			    set_char(it->first.key, vparam.get_data()->c, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_BOOL:
			    set_bool(it->first.key, vparam.get_data()->b, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_OCTETS:
			    set_octets(it->first.key, vparam.get_data()->str, vparam.get_size(), it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_POINTER:
			    set_pointer(it->first.key, vparam.get_data()->p, it->first.index, it->first.fragment, it->first.context);
			    break;

		    }

		}

	    }

            /**
             * Equality operator
             */
            bool operator==(const VariantParamMap<TID, _Alloc>& other) const {
                return (this->params == other.params);
            }

            /**
             * Inequality operator
             */
            bool operator!=(const VariantParamMap<TID, _Alloc>& other) const {
                return (this->params != other.params);
            }

	    /**
	     * += operator
	     */
	    VariantParamMap<TID, _Alloc>& operator+=(const VariantParamMap<TID, _Alloc>& other) {
		// self assignment check
		if(this == &other) return *this;
		// loop other params
		for(cit_t it = other.params.begin(); it != other.params.end(); it++){
		    // other param
		    const VariantParam& vparam = it->second;
                    // skip if it exists
                    if(get_param(it->first.key, it->first.index, it->first.fragment, it->first.context) != NULL) continue;
		    // create param with data from other param
		    switch(vparam.get_type()){
			case DPT_INT:
			    set_int(it->first.key, vparam.get_data()->i64, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_STRING:
			    set_cstr(it->first.key, vparam.get_data()->str, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_DOUBLE:
			    set_double(it->first.key, vparam.get_data()->d, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_CHAR:
			    set_char(it->first.key, vparam.get_data()->c, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_BOOL:
			    set_bool(it->first.key, vparam.get_data()->b, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_OCTETS:
			    set_octets(it->first.key, vparam.get_data()->str, vparam.get_size(), it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_POINTER:
			    set_pointer(it->first.key, vparam.get_data()->p, it->first.index, it->first.fragment, it->first.context);
			    break;

		    }

		}
		return *this;
	    }


	    /**
	     * Assignment operator
	     */
	    VariantParamMap<TID, _Alloc>& operator=(const VariantParamMap<TID, _Alloc>& other) {
		// self assignment check
		if(this == &other) return *this;
		params.clear();
		// copy labels
		labels = other.labels;
		// loop other params
		for(cit_t it = other.params.begin(); it != other.params.end(); it++){
		    // other param
		    const VariantParam& vparam = it->second;
		    // create param with data from other param
		    switch(vparam.get_type()){
			case DPT_INT:
			    set_int(it->first.key, vparam.get_data()->i64, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_STRING:
			    set_cstr(it->first.key, vparam.get_data()->str, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_DOUBLE:
			    set_double(it->first.key, vparam.get_data()->d, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_CHAR:
			    set_char(it->first.key, vparam.get_data()->c, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_BOOL:
			    set_bool(it->first.key, vparam.get_data()->b, it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_OCTETS:
			    set_octets(it->first.key, vparam.get_data()->str, vparam.get_size(), it->first.index, it->first.fragment, it->first.context);
			    break;

			case DPT_POINTER:
			    set_pointer(it->first.key, vparam.get_data()->p, it->first.index, it->first.fragment, it->first.context);
			    break;

		    }

		}
		return *this;
	    }

	    /**
	     * Validate CRC32
	     * @param[in]	checksum_key	checksum parameter id, skipped in calculation
	     * @param[in]	allow_missing	allow missing checksum
	     */
	    bool crc32_validate(TID checksum_key, bool allow_missing){
		// get checksum param
		VariantParam* vp = get_param(checksum_key);
		// check if checkusm missing
		if(vp == NULL){
		    // missing checkusm allowed
		    if(allow_missing) return true;
		    // missing checksum not allowed
		    return false;
		}
		// check if checksum key is DPT_INT
		if(vp->get_type() != DPT_INT) return false;
		// calcuate checksum
		uint32_t tmp_crc = crc32_generate(checksum_key);
		// compare
		if(tmp_crc == (int)*vp) return true;
		// not matched
		return false;
	    }
	    /**
	     * Calculate CRC32
	     * @param[in]	checksum_key	checksum parameter id, skipped in calculation
	     */
	    uint32_t crc32_generate(TID checksum_key){
		// reset
		_crc32.reset();
		// loop parms
		for(it_t it = params.begin(); it != params.end(); it++){
		    // skip actual checksum param
		    if(it->first.key == checksum_key) continue;
		    // param ref
		    VariantParam& vp = it->second;
		    // check type
		    switch(vp.get_type()){
			case pmink_utils::DPT_INT:
			    _crc32.process_bytes(&vp.get_data()->i64, vp.get_size());
			    break;

			case pmink_utils::DPT_STRING:
			    _crc32.process_bytes(vp.get_data()->str, vp.get_size());
			    break;

			case pmink_utils::DPT_DOUBLE:
			    _crc32.process_bytes(&vp.get_data()->d, vp.get_size());
			    break;

			case pmink_utils::DPT_CHAR:
			    _crc32.process_bytes(&vp.get_data()->c, vp.get_size());
			    break;

			case pmink_utils::DPT_BOOL:
			    _crc32.process_bytes(&vp.get_data()->b, vp.get_size());
			    break;

			case pmink_utils::DPT_OCTETS:
			    _crc32.process_bytes(vp.get_data()->str, vp.get_size());
			    break;

			case pmink_utils::DPT_POINTER:
			    _crc32.process_bytes(vp.get_data()->p, vp.get_size());
			    break;

			default: break;

		    }
		}

		// return checksum
		return _crc32.checksum();
	    }

	    /**
	     * Add int param
	     * @param[in]	id	key
	     * @param[in]	data	param data
	     */
	    VariantParam* set_int(TID id, int64_t data, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		insert_res_t rt = params.insert(pmap_value_t(ParamTuple<TID>(id, index, fragment, context),
						VariantParam(DPT_INT, &param_alloc, max)));
		rt.first->second.set_int(data);
		return &rt.first->second;
	    }

	    /**
	     * Increment int param
	     * @param[in]	id	key
	     * @param[in]	data	param data
	     */
	    VariantParam* inc_int(TID id, int64_t data, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		insert_res_t rt = params.insert(pmap_value_t(ParamTuple<TID>(id, index, fragment, context),
						VariantParam(DPT_INT, &param_alloc, max)));


		if(!rt.second) rt.first->second.get_data()->i64 += data;

		return &rt.first->second;
	    }


	    /**
	     * Add bool param
	     * @param[in]	id	key
	     * @param[in]	data	param data
	     */
	    VariantParam* set_bool(TID id, bool data, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		insert_res_t rt = params.insert(pmap_value_t(ParamTuple<TID>(id, index, fragment, context),
						VariantParam(DPT_BOOL, &param_alloc, max)));
		rt.first->second.set_bool(data);
		return &rt.first->second;
	    }


	    /**
	     * Add C string param
	     * @param[in]	id	key
	     * @param[in]	data	param data
	     */
	    VariantParam* set_cstr(TID id, const char* data, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		insert_res_t rt = params.insert(pmap_value_t(ParamTuple<TID>(id, index, fragment, context),
						VariantParam(DPT_STRING, &param_alloc, max)));
		rt.first->second.set_str(data);
		return &rt.first->second;
	    }

	    /**
	     * Add char param
	     * @param[in]	id	key
	     * @param[in]	data	param data
	     */
	    VariantParam* set_char(TID id, char data, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		insert_res_t rt = params.insert(pmap_value_t(ParamTuple<TID>(id, index, fragment, context),
						VariantParam(DPT_CHAR, &param_alloc, max)));
		rt.first->second.set_char(data);
		return &rt.first->second;
	    }

	    /**
	     * Add double param
	     * @param[in]	id	key
	     * @param[in]	data	param data
	     */
	    VariantParam* set_double(TID id, double data, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		insert_res_t rt = params.insert(pmap_value_t(ParamTuple<TID>(id, index, fragment, context),
						VariantParam(DPT_DOUBLE, &param_alloc, max)));
		rt.first->second.set_double(data);
		return &rt.first->second;
	    }

	    /**
	     * Add octets param
	     * @param[in]	id		key
	     * @param[in]	data		param data
	     * @param[in]	data_size	param data size
	     */
	    VariantParam* set_octets(TID id, const void* data, unsigned int data_size, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		insert_res_t rt = params.insert(pmap_value_t(ParamTuple<TID>(id, index, fragment, context),
						VariantParam(DPT_OCTETS, &param_alloc, max)));
		rt.first->second.set_octets(data, data_size);
		return &rt.first->second;
	    }

	    /**
	     * Add pointer param
	     * @param[in]	id	key
	     * @param[in]	data	param data
	     */
	    VariantParam* set_pointer(TID id, void* data, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		insert_res_t rt = params.insert(pmap_value_t(ParamTuple<TID>(id, index, fragment, context),
						VariantParam(DPT_POINTER, &param_alloc, max)));
		rt.first->second.set_pointer(data);
		return &rt.first->second;
	    }

	    VariantParam* set(VariantParam* vp, TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		switch(vp->get_type()){
		    case DPT_INT:
			return set_int(id, vp->get_data()->i64, index, fragment, context);

		    case DPT_STRING:
			return set_cstr(id, vp->get_data()->str, index, fragment, context);

		    case DPT_DOUBLE:
			return set_double(id, vp->get_data()->d, index, fragment, context);

		    case DPT_CHAR:
			return set_char(id, vp->get_data()->c, index, fragment, context);

		    case DPT_BOOL:
			return set_bool(id, vp->get_data()->b, index, fragment, context);

		    case DPT_OCTETS:
			return set_octets(id, vp->get_data()->str, vp->get_size(), index, fragment, context);

		    case DPT_POINTER:
			return set_pointer(id, vp->get_data()->p, index, fragment, context);

		}

		return NULL;

	    }

	    bool set(VariantParamMap& other_map, ParamTuple<TID> other_id, ParamTuple<TID> id){
		// get from other map
		VariantParam* other_vp = other_map.get_param(other_id.key, other_id.index, other_id.fragment, other_id.context);
		// cehck if found
		if(other_vp == NULL) return false;
		// set
		if(set(other_vp, id.key, id.index, id.fragment, id.context) == NULL) return false;
		// ok
		return true;
	    }




	    /**
	     * Defragment parameter
	     * @param[in]	id	key
	     * @param[in]	index	param index
	     * @return		Pointer to defragmented parameter
	     */
	    VariantParam* defragment(TID id, uint32_t index = 0, uint32_t context = 0){
		// find first fragment
		it_t it = params.find(ParamTuple<TID>(id, index, 0, context));
		// sanity check
		if(it == params.end()) return NULL;
		// tmp buffer
		unsigned char tmp_buff[max];
		// counter
		unsigned int c = 0;
		// loop fragments
		for(it_t it_next = it; it != params.end() && it->first.key == id && it->first.index == index && it->first.context == context; it = it_next){
		    // next
		    ++it_next;
		    // copy data
		    memcpy(&tmp_buff[c], it->second.get_data()->str, it->second.get_size());
		    // next fragment
		    c += it->second.get_size();
		    // sanity check
		    if(c >= max) {
			c = max;
			break;
		    }

		    // erase fragment
		    params.erase(it);

		}

		// create and return new param
		return set_octets(id, tmp_buff, c, index, 0, context);


	    }



	    /**
	     * Get param
	     * @param[in]	id	key
	     * @return		Pointer to variant param or NULL if not found
	     */
	    VariantParam* get_param(TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		it_t it = params.find(ParamTuple<TID>(id, index, fragment, context));
		if(it != params.end()) return &it->second;
		return NULL;

	    }

	    /**
	     * Get param as int
	     */
            int64_t get_int(TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
                VariantParam* vp = get_param(id, index, fragment, context);
                if(vp == NULL) return 0;
                return (int64_t)*vp;
            }

	    /**
	     * Get param as c string
	     */
            char* get_cstr(TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
                VariantParam* vp = get_param(id, index, fragment, context);
                if(vp == NULL) return 0;
                return (char*)*vp;
            }

	    /**
	     * Get param as double
	     */
            double get_double(TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
                VariantParam* vp = get_param(id, index, fragment, context);
                if(vp == NULL) return 0;
                return (double)*vp;
            }

	    /**
	     * Get param as char
	     */
            char get_char(TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
                VariantParam* vp = get_param(id, index, fragment, context);
                if(vp == NULL) return 0;
                return (char)*vp;
            }

	    /**
	     * Get param as bool
	     */
            char get_bool(TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
                VariantParam* vp = get_param(id, index, fragment, context);
                if(vp == NULL) return 0;
                return (bool)*vp;
            }

	    /**
	     * Get param as octets
	     */
            unsigned char* get_octets(TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
                VariantParam* vp = get_param(id, index, fragment, context);
                if(vp == NULL) return 0;
                return (unsigned char*)*vp;
            }

	    /**
	     * Get POINTER param and cast it
	     */
            template<typename PTR_TYPE>
            PTR_TYPE get_pointer(TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
                VariantParam* vp = get_param(id, index, fragment, context);
                if(vp == NULL) return NULL;
                return (PTR_TYPE)(void*)*vp;
            }

	    /**
	     * Get OCTETS param and cast it
	     */
            template<typename CAST_TYPE>
            CAST_TYPE get_octets(TID id, uint32_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
                VariantParam* vp = get_param(id, index, fragment, context);
                if(vp == NULL) return NULL;
                return (CAST_TYPE)(unsigned char*)*vp;
            }


	    /**
	     * Check if context exists
	     * @param[in]	context	context
	     * @return		true if found or false otherwise
	     */
	    bool context_exists(uint32_t context){
		for(it_t it = params.begin(); it != params.end(); it++){
		    if(it->first.context == context) return true;
		}
		return false;
	    }

	    bool swap(ParamTuple<TID> id1, ParamTuple<TID> id2){
		// find
		it_t it1 = params.find(id1);
		it_t it2 = params.find(id2);
		// sanity
		if(it1 == params.end() || it2 == params.end()) return false;
		if(it1 == it2) return false;
		// param refs
		VariantParam& vp1 = it1->second;
		VariantParam& vp2 = it2->second;
		// check if types are the same
		if(vp1.get_type() != vp2.get_type()) return false;
		// check type
		switch(vp1.get_type()){
		    case DPT_INT:
		    {
			int64_t tmp = (int)vp1;
			set_int(id1.key, vp2.get_data()->i64, id1.index, id1.fragment, id1.context);
			set_int(id2.key, tmp, id2.index, id2.fragment, id2.context);
			break;
		    }
		    case DPT_STRING:
		    {
			char tmp[max];
			memcpy(tmp, (char*)vp1, vp1.get_size());
			set_cstr(id1.key, vp2.get_data()->str, id1.index, id1.fragment, id1.context);
			set_cstr(id2.key, tmp, id2.index, id2.fragment, id2.context);
			break;
		    }

		    case DPT_DOUBLE:
		    {
			double tmp = (double)vp1;
			set_double(id1.key, vp2.get_data()->d, id1.index, id1.fragment, id1.context);
			set_double(id2.key, tmp, id2.index, id2.fragment, id2.context);
			break;
		    }

		    case DPT_CHAR:
		    {
			char tmp = (char)vp1;
			set_char(id1.key, vp2.get_data()->c, id1.index, id1.fragment, id1.context);
			set_char(id2.key, tmp, id2.index, id2.fragment, id2.context);
			break;
		    }

		    case DPT_BOOL:
		    {
			bool tmp = (bool)vp1;
			set_bool(id1.key, vp2.get_data()->b, id1.index, id1.fragment, id1.context);
			set_bool(id2.key, tmp, id2.index, id2.fragment, id2.context);
			break;
		    }
		    case DPT_OCTETS:
		    {
			unsigned char tmp[max];
			memcpy(tmp, (unsigned char*)vp1, vp1.get_size());
			int tmp_s = vp1.get_size();
			set_octets(id1.key, vp2.get_data()->str, vp2.get_size(), id1.index, id1.fragment, id1.context);
			set_octets(id2.key, tmp, tmp_s, id2.index, id2.fragment, id2.context);
			break;
		    }

		    case DPT_POINTER:
		    {
			void* tmp = (void*)vp1;
			set_pointer(id1.key, vp2.get_data()->p, id1.index, id1.fragment, id1.context);
			set_pointer(id2.key, tmp, id2.index, id2.fragment, id2.context);
			break;
		    }
		}

		// ok
		return true;
	    }


	    /**
	     * Remove param from list
	     * @param[in]	id	key
	     * @return		Number of removed params
	     */
	    size_t erase_param(TID id, int64_t index = 0, uint32_t fragment = 0, uint32_t context = 0){
		if(index == -1){
		    int c = 0;
		    for(it_t it = params.begin(), it_next = it; it != params.end(); it = it_next){
			// next
			++it_next;
			// check key
			if(it->first.key == id && it->first.context == context) {
			    params.erase(it);
			    ++c;
			}
		    }
		    // return
		    return c;

		}else return params.erase(ParamTuple<TID>(id, index, fragment, context));
	    }

	    /**
	     * Remove all params
	     */
	    void clear_params(){
		params.clear();
	    }

	    /**
	     * Get iterator to beginning of params
	     */
	    it_t get_begin(){
		return params.begin();
	    }

	    /**
	     * Get iterator to end of params
	     */
	    it_t get_end(){
		return params.end();
	    }

	    /**
	     * Standard output operator
	     */
	    friend ostream& operator<<(ostream& out, const VariantParamMap& pmap){
		for(cit_t it = pmap.params.begin(); it != pmap.params.end(); it++){
		    // key
		    out << std::dec << it->first;
		    // find label
		    typename std::map<TID, std::string>::iterator itl = pmap.labels_p->find(it->first.key);
		    if(itl != pmap.labels_p->end()) out << " (" << itl->second << ")";
		    // value
		    out << " = " << it->second;
		    // endl
		    out << std::endl;
		}
		return out;

	    }

	    /**
	     * Set param id label
	     */
	    void set_label(TID id, const char* label){
		labels[id] = label;
	    }

	    /**
	     * Set label map pointer
	     */
	    void set_label_p(std::map<TID, std::string>* labels){
		labels_p = labels;
	    }

	    /**
	     * Get param map size
	     */
	    size_t size(){
		return params.size();
	    }

	protected:
	    unsigned int max;
	    param_alloc_t param_alloc;
	    std::map<TID, std::string> labels;
	    std::map<TID, std::string>* labels_p;
	    std::map<ParamTuple<TID>, VariantParam, ParamTupleCompare<TID>, _Alloc> params;	/**< Parameter map */
	    boost::crc_32_type _crc32;


	};


	/**
	 * Pooled Variant param map (GCC mt_allocator)
	 * @param[in]	TID		key type
	 */
	template<typename TID>
	class PooledVPMap: public VariantParamMap<TID,
						  __gnu_cxx::__mt_alloc<std::pair<const ParamTuple<TID>, pmink_utils::VariantParam > > >{
	public:
	    /**
	     * types
	     */
	    typedef __gnu_cxx::__pool_base::_Tune tune_t;
	    typedef __gnu_cxx::__mt_alloc<std::pair<const ParamTuple<TID>, pmink_utils::VariantParam > > mt_alloc_t;

	    /**
	     * Constructor
	     */
	    PooledVPMap(unsigned int MAX_STR_SIZE = 1024): VariantParamMap<TID, mt_alloc_t>(MAX_STR_SIZE, mt_alloc_t()){
		// mt_allocator tuning
		//tune_t t_opt(16, tmp * 2, 32, tmp * 1000, 32, 10, false);
		//tune_t t_opt(16, 2048, 32, 10240, 32, 10, false);
		// set mt_allocator params
		//int tmp = (int)pow((double)2, (int)ceil(log10(sizeof(std::pair<const ParamTuple<TID>, pmink_utils::VariantParam >)) / log10(2)));
		//mt_alloc_t tmp_alloc;
		//tmp_alloc._M_set_options(t_opt);
		//this->params = std::map<ParamTuple<TID>, VariantParam, ParamTupleCompare<TID>, mt_alloc_t>(ParamTupleCompare<TID>(), tmp_alloc);
	    }
	};

	template<typename TYPE, bool TH_SAFE = true>
	class Range{
	public:
	    Range(): from(std::numeric_limits<TYPE>::min()),
		     to(std::numeric_limits<TYPE>::max()),
		     rnext(from){

	    }
	    Range(TYPE _from, TYPE _to): from(_from),
					 to(_to),
					 rnext(from){

	    }

	    TYPE max(){
		return to;
	    }

	    TYPE min(){
		return from;
	    }

	    TYPE next(){
		if(TH_SAFE) lock.lock();
		if(rnext > to) rnext = from;
		TYPE res = rnext++;
		if(TH_SAFE) lock.unlock();
		return res;
	    }

	    void set_range(TYPE _from, TYPE _to){
		from = _from;
		to = _to;
		rnext = from;
	    }

	private:
	    TYPE from;
	    TYPE to;
	    TYPE rnext;
	    Spinlock lock;
	};

	class StatsManager {
	public:
	    typedef std::map<uint32_t, pmink::Atomic<uint64_t> >::iterator it_t;

	    StatsManager(){}
	    virtual ~StatsManager(){}

	    void register_item(uint32_t id){
		stats_map[id] = pmink::Atomic<uint64_t>();
	    }

	    void inc(uint32_t id, uint64_t val){
		it_t it = stats_map.find(id);
		if(it != stats_map.end()) it->second.fetch_add(val);
	    }

	    void dec(uint32_t id, uint64_t val){
		it_t it = stats_map.find(id);
		if(it != stats_map.end()) it->second.fetch_sub(val);
	    }

	    uint64_t get(uint32_t id){
		it_t it = stats_map.find(id);
		if(it != stats_map.end()) return it->second.fetch_add(0);
		return 0;

	    }

	    uint64_t set(uint32_t id, uint64_t val){
		it_t it = stats_map.find(id);
		if(it != stats_map.end()) return it->second.set(val);
		return 0;

	    }

	private:
	    std::map<uint32_t, pmink::Atomic<uint64_t> > stats_map;
	};

	// TCAP transaction key
	class TCAPTransactionKey{
	public:
	    TCAPTransactionKey(): sid(0),
				  did(0),
				  gt(0),
				  key_1(),
				  key_2(0){}

	    TCAPTransactionKey(uint32_t _sid, uint32_t _did, uint32_t _gt): sid(_sid),
									    did(_did),
									    gt(_gt),
									    key_1(0),
									    key_2(0){}



	    bool operator< (const TCAPTransactionKey& other) const {
		// sid == sid
		if(key_1 == other.key_1 && key_1 > 0) return key_2 > 0 && key_2 < other.key_2;
		// sid == did
		if(key_1 == other.key_2 && key_1 > 0) return key_2 > 0 && key_2 < other.key_1;
		// did == sid
		if(key_2 == other.key_1 && key_2 > 0) return key_1 > 0 && key_1 < other.key_2;
		// did == did
		if(key_2 == other.key_2 && key_2 > 0) return key_1 > 0 && key_1 < other.key_1;
		// other
		return (key_1 < other.key_1 || (!(other.key_1 < key_1) && key_2 < other.key_2));

	    }

	    uint32_t sid;
	    uint32_t did;
	    uint32_t gt; // gt calling of initiator

	    uint64_t key_1; // A: sid(32bit) + gt(32bit hash)
	    uint64_t key_2;// B: sid(32bit) + gt(32bit hash)


	    void reset(){
		sid = 0;
		did = 0;
		gt = 0;
		key_1 = 0;
		key_2 = 0;
	    }

	    void generate_keys(uint32_t _sid, uint32_t _did, uint32_t _gt, bool sid_present, bool did_present){
		// gt
		gt = _gt;

		// sid
		if(sid_present) {
		    key_1 = _sid | ((uint64_t)_gt << 32);
		    sid = _sid;
		}

		// did
		if(did_present) {
		    key_2 = _did | ((uint64_t)_gt << 32);
		    did = _did;

		}
	    }

/*
	    bool operator != (const TCAPTransactionKey& right){
		if(this->sid == right.sid) return false;
		//if(this->did == right.did) return false;
		if(this->gt == right.gt) return false;
		return true;

	    }
	    bool operator == (const TCAPTransactionKey& right){
		if(this->sid != right.sid) return false;
		//if(this->did != right.did) return false;
		if(this->gt != right.gt) return false;
		return true;
	    }
*/

	};

/*
	// TCAP key compare
	class TCAPTransactionCompare{
	public:
	    bool operator ()(const TCAPTransactionKey& x, const TCAPTransactionKey& y){
		// sid == sid
		if(x.key_1 == y.key_1 && x.key_1 > 0) return x.key_2 > 0 && x.key_2 < y.key_2;
		// sid == did
		if(x.key_1 == y.key_2 && x.key_1 > 0) return x.key_2 > 0 && x.key_2 < y.key_1;
		// did == sid
		if(x.key_2 == y.key_1 && x.key_2 > 0) return x.key_1 > 0 && x.key_1 < y.key_2;
		// did == did
		if(x.key_2 == y.key_2 && x.key_2 > 0) return x.key_1 > 0 && x.key_1 < y.key_1;
		// other
		return (x.key_1 < y.key_1 || (!(y.key_1 < x.key_1) && x.key_2 < y.key_2));

	    }

	};

*/
	// fwd declaration
	class TCAPTransaction;

	// tmap
	typedef __gnu_cxx::__mt_alloc<std::pair<const uint64_t, TCAPTransaction> > tcap_tmap_alloc_t;
	typedef std::map<uint64_t, TCAPTransaction, std::less<uint64_t>, tcap_tmap_alloc_t> tcap_tmap_t;
	typedef tcap_tmap_t::iterator tcap_tmap_it_t;
	typedef tcap_tmap_t::reverse_iterator tcap_tmap_rev_it_t;
	typedef std::pair<tcap_tmap_it_t, bool> tcap_tmap_insert_t;
	typedef tcap_tmap_t::value_type tcap_tmap_value_t;
	// tcap
	typedef __gnu_cxx::__mt_alloc<std::pair<const TCAPTransactionKey, tcap_tmap_it_t> > tcap_map_alloc_t;
	typedef std::map<TCAPTransactionKey, tcap_tmap_it_t, std::less<TCAPTransactionKey>, tcap_map_alloc_t> tcap_map_t;
	typedef tcap_map_t::iterator tcap_map_it_t;
	typedef std::pair<tcap_map_it_t, bool> tcap_map_insert_t;
	typedef tcap_map_t::value_type tcap_map_value_t;

	// TCAP Operation
	class TCAPOperation{
	public:
	    TCAPOperation();

	    TCAPOperation& operator=(const TCAPOperation& other) {
		TCAPOperation& o = const_cast<TCAPOperation&>(other);
		tcap_trans = o.tcap_trans;
		invoke_id = o.invoke_id;
		finished = o.finished;
		processed = o.processed;
		req_component_index = o.req_component_index;
		rpl_component_index = o.rpl_component_index;
		request_params = o.request_params;
		reply_params = o.reply_params;
		return *this;

	    }

	    TCAPTransaction* tcap_trans;
	    uint32_t invoke_id;
	    bool finished;
	    bool processed;
	    uint32_t req_component_index;
	    uint32_t rpl_component_index;
	    PooledVPMap<uint32_t> request_params;
	    PooledVPMap<uint32_t> reply_params;

	};
	// TCAP Transaction status type
	enum TCAPStatusType{
	    TCAP_ST_ACTIVE	= 0,
	    TCAP_ST_FINISHED 	= 1,
	    TCAP_ST_ABORTED	= 2,
	    TCAP_ST_TIMEOUT	= 3,
	    TCAP_ST_EXPIRED	= 4,
	    TCAP_ST_SCCP_ERROR	= 5
	};

	// TCAP transaction
	class TCAPTransaction{
	public:
	    TCAPTransaction();
	    ~TCAPTransaction();

	    TCAPTransactionKey id;
	    time_t timestamp;
	    std::vector<TCAPOperation> operations;
	    TCAPStatusType status;
	    tcap_map_it_t transaction_it;

	};


	// TCAP correlator
	class TCAPCorrelator{
	public:
	    TCAPCorrelator(unsigned int _max_trans = 1000);
	    ~TCAPCorrelator();


	    TCAPTransaction* process(void* data, pmink_utils::PooledVPMap<uint32_t>* params);
	    void process_timeout(unsigned int timeout, std::vector<tcap_tmap_it_t>* result, bool append = true);
	    TCAPTransaction* validate_transaction(uint32_t sid, uint32_t did, uint32_t gt_called, uint32_t gt_calling, bool sid_present, bool did_present);
	    TCAPTransaction* update_key(tcap_map_it_t& it, TCAPTransactionKey* new_key);
	    int remove_transaction(TCAPTransactionKey& key);
	    int remove_transaction(tcap_tmap_it_t it);
	    int cleanup(TCAPTransaction* trans);

	    void lock();
	    void unlock();

	    void set_max_trans(int _max_trans);

	    uint32_t get_size();
	    int expire(std::vector<tcap_tmap_it_t>* result, bool append = true);

	private:
	    //tcap_map_type tcap_map;
	    pthread_spinlock_t slock;
	    unsigned int max_trans;

	    tcap_map_t transactions;
	    tcap_tmap_t transaction_tmap;


	    int process_operations(TCAPTransaction* tcap_trans, pmink_utils::PooledVPMap<uint32_t>* params, void* data);
	    int abort_operations(TCAPTransaction* tcap_trans, pmink_utils::PooledVPMap<uint32_t>* params, bool set_reply = true);
	    TCAPOperation* operation_exists(TCAPTransaction* tcap_trans, uint32_t invoke_id);

	};



	/**
	 * Pcap dumper
	 */
	class PcapDump {
	public:
	    /**
	     * Constructor
	     * @param[out]	_buffer		Pointer to output buffer
	     */
	    PcapDump(char* _buffer);

	    /**
	     * Default constructor
	     */
	    PcapDump();

	    /**
	     * Destructor
	     */
	    ~PcapDump();

	    /**
	     * Add new packet
	     * @param[in]	_data			Pointer to packet data
	     * @param[in]	_dsize			Packet size
	     * @param[in]	_dize_untrunc		Un-truncated packet size
	     * @param[in]	_tstamp			Packet timestamp (seconds part)
	     * @param[in]	_microsec		Packet timestamp (microseconds part)
	     * @return		0 for success or error code
	     *
	     */
	    int add_packet(	const char* _data,
				uint32_t _dsize,
				uint32_t _dize_untrunc,
				uint32_t _tstamp,
				uint32_t _microsec);

	    /**
	     * Finish pcap stream
	     * @param[out]	_size			Pointer to variable whose value will be set to
	     * 						size of pcap stream
	     * @return		Pointer to pcap stream buffer
	     */
	    char* finish(uint32_t* _size);

	    /**
	     * Generate pcap header
	     */
	    int genereate_header();

	    /**
	     * Set output buffer
	     * @param[out]	_buffer			Pointer to output buffer
	     */
	    void set_buffer(char* _buffer);

	    /**
	     * Get output buffer
	     * @return		Pointer to output buffer
	     */
	    char* get_buffer();

	    /**
	     * Get buffer size
	     * @return		Number of bytes contained in buffer
	     */
	    uint32_t get_buffer_size();

	    /**
	     * Get packet count
	     * @return		Number of packets contained in buffer
	     */
	    uint64_t get_packet_count();

	    /**
	     * Ser link layer header type
	     * @param[in]	type		Link layer header type
	     */
	    void set_ll_hdr_type(uint32_t type);


	private:
	    char* buffer;
	    uint32_t buffer_size;
	    uint32_t magic_num;
	    uint16_t major_ver;
	    uint16_t minor_ver;
	    uint32_t tzone_offset;
	    uint32_t tstamp_accr;
	    uint32_t snapshot_l;
	    uint32_t ll_hdr_type;
	    uint64_t packet_count;



	};

	/**
	 * FNV 32bit hash (http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx)
	 * @param[in]		key		Pointer to source data
	 * @param[in]		len		Length of source data
	 * @return		32bit FNV hash
	 */
	uint32_t hash_fnv(const void* key, int len);

	/**
	 * FNV 32bit hash (http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx)
	 * @param[in]		key		Pointer to source data
	 * @param[in]		len		Length of source data
	 * @return		32bit FNV hash
	 */
	uint32_t hash_fnv1a(const void* key, int len);

	/**
	 * FNV-1a 64bit hash (http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx)
	 * @param[in]		key		Pointer to source data
	 * @param[in]		len		Length of source data
	 * @return		64bit FNV hash
	 */
	uint64_t hash_fnv1a_64bit(const void* key, int len);

	/**
	 * FNV 32bit hash (http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx)
	 * @param[in]		str		Pointer to source string data
	 * @return		32bit FNV hash
	 */
	uint32_t hash_fnv1a_str(const char* key);

	/**
	 * FNV 64bit hash (http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx)
	 * @param[in]		str		Pointer to source string data
	 * @return		64bit FNV hash
	 */
	uint64_t hash_fnv1a_str_64bit(const char* key);


	/**
	 * Encode OID
	 * @param[in,out]	oid_data	OID C string
	 * @param[out]		output		Pointer to output buffer
	 * @param[in]		absolute	Absolute if True or Relative if False
	 * @return		Number of bytes contained in output buffer
	 */
	int oid_encode(char* oid_data, unsigned char* output, bool absolute = true);

	/**
	 * Decode OID
	 * @param[in]		oid_data	Pointer to encoded OID data
	 * @param[in]		oid_data_length	Size of encoded OID data
	 * @param[out]		output		Pointer to output buffer
	 * @param[in]		absolute	Absolute if True or Relative if False
	 *
	 */
	void oid_decode(const unsigned char* oid_data, unsigned int oid_data_length, char* output, bool absolute = true);

	/**
	 * Decode OID node (used by oid_decode)
	 * @param[in]		oid_data	Pointer to encoded OID node data
	 * @param[out]		output		Pointer to output buffer
	 * @param[in]		is_first	First node flag
	 * @param[in]		absolute	Absolute if True or Relative if False
	 * @return		Number of bytes written to output buffer
	 *
	 */
	int oid_decode_node(const unsigned char** oid_data, char* output, bool is_first, bool absolute);

	/**
	 * Encode OID node (used bu oid_encode)
	 * @param[in]		val		OID Node value
	 * @param[out]		output		Pointer to output buffer
	 * @return		Number of bytes written to output buffer
	 */
	int oid_encode_node(uint32_t val, unsigned char** output);

	/**
	 * Get number of bits required for int
	 * @param[in]		input		Integer value
	 * @return		Number of bits required for input
	 */
	int bits_required(unsigned int input);

	/**
	 * Get number of bytes required for int
	 * @param[in]		input		Integer value
	 * @return		Number of bytes required for input
	 */
	int bytes_required(unsigned int input);

	/**
	 * Get number of digits in an int
	 * @param[in]		input		Integer value
	 * @return		Number of digits
	 *
	 */
	int num_of_digits(unsigned int input);

	/**
	 * Get free system memory
	 */
	uint64_t mem_get_free();

	/**
	 * Run external script
	 * @param[in]	script		Pointer to script path
	 * @param[out]	result		Pointer to output buffer
	 * @param[in]	result_size	Maximum size of output buffer
	 * @return	0 for success or error code if error occurred
	 */
	int run_external(const char* script, char* result, int result_size);

	/**
	 * Run extarnal script and print output
	 * @param[in]	script		Pointer to script path
	 * @param[in]	ncurses		Ncurses flag (if false, use std::cout)
	 *
	 */
	void run_external_print(const char* script, bool ncurses);

	/**
	 * Run external plugin command handler
	 * @param[in]	module		Pointer to module path
	 * @param[in]	arg_names	Pointer to list of name arguments
	 * @param[in]	arg_values	Pointer to list of value arguments
	 * @param[in]	arg_count	Number of arguments
	 * @param[in]	ncurses		Ncurses flag (if false, use std::cout)
	 * @return	NULL, reserved for future use
	 */
	void* run_external_method_handler(const char* module, const char** arg_names, const char** arg_values, int arg_count, bool ncurses);

	/**
	 * Run external plugin method
	 * @param[in]	handle		Pointer to module handle
	 * @param[in]	method		Pointer to method name
	 * @param[in]	args		Pointer to list of arguments
	 * @param[in]	argc		Number of arguments
	 * @param[in]	ncurses		Ncurses flag (if false, use std::cout)
	 * @return	Module dependent
	 *
	 */
	void* run_external_method(void* handle, const char* method, void** args, int argc, bool ncurses);

	/**
	 * Run external plugin method
	 * @param[in]	handle		Pointer to module path
	 * @param[in]	method		Pointer to method name
	 * @param[in]	args		Pointer to list of arguments
	 * @param[in]	argc		Number of arguments
	 * @param[in]	ncurses		Ncurses flag (if false, use std::cout)
	 * @return	Module dependent
	 *
	 */
	void* run_external_method(const char* module, const char* method, void** args, int argc, bool ncurses);

	/**
	 * Load plugin
	 * @param[in]	module		Pointer to module path
	 * @return	Plugin handle
	 */
	void* load_plugin(const char* module);

	/**
	 * Unload plugin
	 * @param[in]	handle		Plugin handle
	 */
	void unload_plugin(void* handle);

	/**
	 * Calculate time difference
	 * @param[in]	start		Start time
	 * @param[in]	end		End time
	 * @return 	Time difference
	 */
	timespec time_diff(timespec start, timespec end);

	/**
	 * Print byte data in hex format to standard output
	 * @param[in]	data		Pointer to raw data
	 * @param[in]	data_length	Data length
	 */
	void print_hex(void* data, int data_length);

	/**
	 * Print ip data in user dot format
	 * @param[in]	data		Pointer to raw data
	 * @param[in]	data_length	Data length
	 */
	void print_ip(unsigned char* data, int data_length);

	/**
	 * Print byte data in hex format to output stream
	 * @param[in]	data		Pointer to raw data
	 * @param[in]	data_length	Data length
	 * @param[in]	pre_padd	Initial number of padding bytes
	 * @param[in]	out		Output stream
	 * @param[in]	delim		Delimiter
	 */
	void hex(void* data, int data_length, int pre_padd, std::ostringstream* out, char delim = 0);

	/**
	 * Load raw file contents
	 * @param[in]	filename	Filename
	 * @param[out]	result		Pointer to output buffer
	 * @param[out]	result_size	Pointer to int which will contain file size
	 * @return	0 for success or error code if error occurred
	 *
	 */
	int load_file(const char* filename, char* result, int* result_size);

	/**
	 * Copy file
	 * @param[in]	src		Source filename
	 * @param[in]	dst		Destination filename
	 * @return	0 for success or error code if error occurred
	 */
	int copy_file(const char* src, const char* dst);

	/**
	 * Get file size
	 * @param[in]	filename	Filename
	 * @return	File size
	 */
	int get_file_size(const char* filename);

	/**
	 * Save raw data to file
	 * @param[in]	filename	Output filename
	 * @param[in]	data		Pointer to raw data
	 * @param[in]	data_size	Data size
	 * @param[in]	append		File append flag
	 * @return	0 for success or error code
	 */
	int save_file(const char* filename, char* data, int data_size, bool append = false);

	/**
	 * Auto complete rollback revision sort, used by scandir
	 * @param[in]	a		Item one from scandir
	 * @param[in]	b		Item two from scandir
	 * @return	-1 if a > b, 1 if a < b, or 0
	 */
	int _ac_rollback_revision_sort(const struct dirent ** a, const struct dirent ** b);

	/**
	 * Auto complete rollback file filtr
	 * @param[in]	a		Item provided by scandir
	 * @return	1 if valid or 0 otherwise
	 */
	int _ac_rollback_revision_filter(const struct dirent* a);

	/**
	 * Tokenize string
	 * @param[in]	data		Pointer to string
	 * @param[out]	result		Pointer to output string list
	 * @param[in]	result_max_size	Maximum size of output list
	 * @param[out]	result_size	Pointer to int which will contain number of tokens in output list
	 * @param[in]	keep_quotes	If set, keep quotes if needed
	 */
	void tokenize(std::string* data, std::string* result, int result_max_size, int* result_size, bool keep_quotes);

	/**
	 * Implementations of "more"
	 * @param[in]	line_c		Number of lines present in data window buffer
	 * @param[out]	data_win	Pointer to data window buffer
	 * @param[in]	interrupt	Pointer to interrupt flag, if True stop line output
	 * @return	interrupted characted or -1
	 *
	 */
	int cli_more(int line_c, WINDOW* data_win, bool* interrupt);

}

#endif /* PMINK_UTILS_H_ */
