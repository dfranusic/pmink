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
 * @file pool.h
 * @brief Memory pool
 *
 */

#ifndef POOL_H_
#define POOL_H_

#include <new>
#include <strings.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <spscq.h>
//#include <typeinfo>

using namespace std;
namespace memory {

	// raw buffer class
	// wrapper for generic char buffer
	template <int COUNT>
	class MemChunk {
	public:
		unsigned char buffer[COUNT];
		unsigned int buffer_length;

		MemChunk(){
			buffer_length = 0;
			bzero(buffer, COUNT);
		}

		/**< Custom '==' operator */
		bool operator== (const MemChunk& right){
		    if(buffer_length != right.buffer_length) return false;
		    if(memcmp(buffer, right.buffer, buffer_length) != 0) return false;
		    return true;
		}

		int set_data(unsigned char* _data, unsigned int _data_length){
		    if(COUNT < _data_length) return 1;
		    buffer_length = _data_length;
		    memcpy(buffer, _data, _data_length);
		    return 0;
		}

		int get_max_size(){
		    return COUNT;
		}

	};

	class HeapChunkBuffer {
	public:
	    HeapChunkBuffer(): chunk_size(0),
			       chunk_count(0),
			       buff(NULL){}

	    HeapChunkBuffer(unsigned int _chunk_size, unsigned int _chunk_count): chunk_size(_chunk_size),
										  chunk_count(_chunk_count),
										  buff(NULL){
		// init
		init(chunk_size, chunk_count);

	    }

	    ~HeapChunkBuffer(){
		delete[] buff;
	    }


	    unsigned char* get_chunk(unsigned int index){
		if(index >= chunk_count) return NULL;
		return &buff[index * chunk_size];
	    }

	    void init(unsigned int _chunk_size, unsigned int _chunk_count){
		chunk_size = _chunk_size;
		chunk_count = _chunk_count;
		// init raw buffer
		buff = new unsigned char[(uint64_t)chunk_size * chunk_count];

	    }

	    unsigned int get_chunk_size(){
		return chunk_size;
	    }

	    unsigned int get_chunk_count(){
		return chunk_count;
	    }

	private:
	    unsigned int chunk_size;
	    unsigned int chunk_count;
	    unsigned char* buff;
	};




	// memory pool
	template <typename T, bool THSAFE = false>
	class Pool {
	private:
		char* buffer;
		int chunk_count;
		int chunk_size;
		uint64_t mem_size;
		int next_free_mem;
		char** raw_free_list;
		char** constructed_free_list;
		int raw_free_size;
		int constructed_free_size;
		bool construted_mode;
		//pthread_mutex_t mtx;
		pthread_spinlock_t slock;

		void* find_available(char** _source, int* _source_size){
			char* tmp = _source[--(*_source_size)];

			//std::cout << "find_available: Found free chunk: " << (int*)tmp << ", index: " << (*_source_size) <<  std::endl;

			return tmp;
		}
	public:
		void lock(){
		    //if(THSAFE) pthread_mutex_lock(&mtx);
		    if(THSAFE) pthread_spin_lock(&slock);
		}

		void unlock(){
		    //if(THSAFE) pthread_mutex_unlock(&mtx);
		    if(THSAFE) pthread_spin_unlock(&slock);
		}

		int get_free_count(){
		    int tmp = 0;
		    lock();
		    if(construted_mode) tmp = constructed_free_size; else tmp = raw_free_size;
		    unlock();
		    return tmp;
		}
		int get_chunk_count(){
			return chunk_count;
		}
		// constructors and destructors
		Pool(){
			constructed_free_size = 0;
			constructed_free_list = NULL;
			raw_free_list = NULL;
			buffer = NULL;
			raw_free_size = 0;
			chunk_count = 0;
			chunk_size = 0;
			construted_mode = false;
			//if(THSAFE) pthread_mutex_init(&mtx, NULL);
			if(THSAFE) pthread_spin_init(&slock, 0);
		}
		Pool(int _chunk_count){
			init(_chunk_count);
			//if(THSAFE) pthread_mutex_init(&mtx, NULL);
			if(THSAFE) pthread_spin_init(&slock, 0);
		}
		~Pool(){
			if(chunk_count <= 0){
			    //if(THSAFE) pthread_mutex_destroy(&mtx);
			    if(THSAFE) pthread_spin_destroy(&slock);
			    return;
			}
			if(construted_mode){
				T* tmp = NULL;
				char* tmp_c = NULL;
				for(int i = 0; i<constructed_free_size; i++){
					tmp_c = constructed_free_list[i];
					// set pointer to T object
					tmp = (T*)tmp_c;
					// call destructor
					tmp->~T();
				}
			}
			delete[] constructed_free_list;
			delete[] raw_free_list;
			delete[] buffer;
			//if(THSAFE) pthread_mutex_destroy(&mtx);
			if(THSAFE) pthread_spin_destroy(&slock);

		}

		// calculate required memory for chunk count
		static uint32_t calc_req_mem(int _chunk_count){
		    return _chunk_count * sizeof(T);
		}

		// calculate chunk count for memory size
		static uint32_t calc_mem_fit(uint32_t mem_mb){
		    return ceil((double)(mem_mb * 1024 * 1024) / sizeof(T));
		}


		// init
		void init(int _chunk_count){
			construted_mode = false;
			// chunks
			chunk_count = _chunk_count;
			chunk_size = sizeof(T);
			mem_size = (uint64_t)chunk_count * chunk_size;

			// main buffer
			buffer = new char[mem_size];
			bzero(this->buffer, this->mem_size);
			next_free_mem = 0;


			// free lists
			raw_free_list = new char* [chunk_count];
			constructed_free_list = new char* [chunk_count];
			raw_free_size = chunk_count;
			constructed_free_size = 0;


			//cout << "Pool item chunk size: " << chunk_size << endl;
			//cout << "Pool item chunk count: " << chunk_count << endl;
			//cout << "Pool total mem size: " << mem_size << ", " << (int*)buffer << endl;
			//cout << "Pool mem: " << (int*)buffer << endl;

			// init extra bytes
			for(uint64_t i = 0; i<mem_size; i += chunk_size){
				// link with list
				raw_free_list[i / chunk_size] = &buffer[i];
			}
		}
		// construct objects if not using raw memory
		void construct_objects(){
			if(chunk_count <= 0) return;
			T* tmp = NULL;
			char* tmp_c = NULL;
			T* tmp_arr = new(buffer) T[chunk_count];

			for(int i = 0; i<chunk_count; i++){
				// allocate T
				tmp = &tmp_arr[i];
				tmp_c = (char*)tmp;

				//tmp = new(allocate_raw()) T();
				//tmp_c = (char*)tmp;
				// link with list
				constructed_free_list[i] = tmp_c;
			}
			// update linked list size
			construted_mode = true;
			constructed_free_size = chunk_count;
			// disable raw linked list
			raw_free_size = 0;
		}


		// allocate raw memory chunk
		void* allocate_raw(){
			lock();
			// check free list
			if(raw_free_size > 0){
			    void* tmp = find_available(raw_free_list, &raw_free_size);
			    unlock();
			    return tmp;

			}

			//cout << "NO FREE CHUNKS! " << endl;
			unlock();
			return NULL;

		}

		// get specific by index
		T* get_constructed(unsigned int index){
		    if(index >= chunk_count) return NULL;
		    return (T*)constructed_free_list[index];

		}

		// allocate already constructed object
		T* allocate_constructed(){
			lock();
			// check free list
			if(constructed_free_size > 0){
				T* tmp = (T*)find_available(constructed_free_list, &constructed_free_size);
				unlock();
				return tmp;

			}

			//cout << "NO FREE CHUNKS! " << endl;
			unlock();
			return NULL;

		}
		// deallocate constructed object
		int deallocate_constructed(T* obj){
			if(chunk_count <= 0) return 4;
			lock();

			if((obj != NULL) && (constructed_free_size < chunk_count)){
			    //  tmp err debug
			    /*
			    T* ttmp = NULL;
			    for(unsigned int i = 0; i<constructed_free_size; i++){
				ttmp = (T*)constructed_free_list[i];
				if(ttmp == obj){
				    std::cout << "ERRR!!!!!!!!!!!!!: " << typeid(T).name() << std::endl;
				    unlock();
				    return 5;
				}
			    }
			    */

				// set pointer
				char* tmp = (char*)obj;
				// return to linked list
				constructed_free_list[constructed_free_size++] = tmp;
				//cout << "deallocate_object: Next free item index: " << (constructed_free_size - 1) << " - " << (int*)tmp << endl;
				unlock();
				return 0;

			}
			unlock();
			return 1;
		}
		// deallocate raw memory
		int deallocate_raw(T* obj){
			if(chunk_count <= 0) return 4;

			lock();
			if((obj != NULL) && (raw_free_size < chunk_count)){
				// set pointer
				char* tmp = (char*)obj;

				// call destructor
				obj->~T();
				// return to linked list
				raw_free_list[raw_free_size++] = tmp;
				//cout << "deallocate: Next free item index: " << (raw_free_size - 1) << " - " << (int*)tmp << endl;
				unlock();
				return 0;

			}
			unlock();
			return 1;
		}


	};

	// spsc based memory pool
	template <typename T>
	class SpscQPool: public lockfree::SpscQ<T>{
	public:
	    SpscQPool(){
		ready = false;
	    }
	    ~SpscQPool(){
		if(!ready) return;
		T* tmp;
		// return to pool
		while(lockfree::SpscQ<T>::pop(&tmp) == 0) pool.deallocate_constructed(tmp);
	    }

	    void init(unsigned int pool_size){
		// init q
		lockfree::SpscQ<T>::init(pool_size);
		// init pool
		pool.init(pool_size);
		pool.construct_objects();
		// transfer to spsc q
		for(unsigned int i = 0; i<pool_size; i++) lockfree::SpscQ<T>::push(1, pool.allocate_constructed());
		// ready
		ready = true;
	    }

	private:
	    Pool<T> pool;
	    bool ready;

	};



};


#endif /* POOL_H_ */
