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
 * @file ring.h
 * @brief Circular buffer thread safe wrapper for boost::circular_buffer
 *
 */

#ifndef RING_H_
#define RING_H_

#include <boost/circular_buffer.hpp>
#include <pthread.h>

namespace pmink{
    /**
     * Ring buffer based on boost::circular_buffer
     * @param[in]	TYPE		data type
     * @param[in]	THSAFE		thread safety flag
     */
    template<typename TYPE, bool THSAFE = true>
    class RingBuffer{
    public:
	/**
	 * Default constructor
	 */
	RingBuffer(){
	    if(THSAFE) pthread_spin_init(&slock, 0);

	}

	/**
	 * Destructor
	 */
	~RingBuffer(){
	    if(THSAFE) pthread_spin_destroy(&slock);

	}

	/**
	 * pop from queue
	 * @param[out]	out		output pointer for popped data
	 * @return	true if pooped or false if empty
	 */
	bool pop(TYPE* out){
	    lock();
	    if(!buffer.empty()){
		TYPE tmp = buffer.front();
		buffer.pop_front();
		unlock();
		*out = tmp;
		return true;

	    }
	    unlock();
	    return false;
	}

	/**
	 * push to queue
	 * @param[in]	data		data tu push
	 * @return	true if pushed or false if full
	 */
	bool push(TYPE data){
	    lock();
	    if(!buffer.full()){
		buffer.push_back(data);
		unlock();
		return true;
	    }
	    unlock();
	    return false;

	}

	/**
	 * Set max queue capacity
	 * @param[in]	capacity	queue capacity
	 */
	void set_capacity(unsigned int capacity){
	    buffer.set_capacity(capacity);
	}

    private:
	/**
	 * Lock operation
	 */
	void lock(){
	    if(THSAFE) pthread_spin_lock(&slock);
	}

	/**
	 * Unlock operation
	 */
	void unlock(){
	    if(THSAFE) pthread_spin_unlock(&slock);
	}


	boost::circular_buffer<TYPE> buffer;
	pthread_spinlock_t slock;



    };

}




#endif /* RING_H_ */
