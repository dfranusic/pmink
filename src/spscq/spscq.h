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
 * @file 	spscq.h
 * @brief 	pMINK lock-free x86 spsc queue
 * 		(special thanks to Alfredo Cardigliano from ntop.org)
 *
 */
#ifndef SPSCQ_H_
#define SPSCQ_H_

// gcc memory barrier
#define gcc_mb() __asm__ __volatile__("": : :"memory")

#include <iostream>
#include <stdlib.h>
#include <cmath>


namespace lockfree {
    class QitemBase {
    public:
	int type;

    };

    template<typename T, int TYPE>
    class QItem: public QitemBase {
    public:
	QItem(){
	    type = TYPE;
	}
	T data;
    };


    template<typename T>
    class SpscQ {
    public:
	typedef struct {
	    u_int64_t shadow_head;
	    u_char __cacheline_padding_1[56];
	    volatile u_int64_t head;
	    u_char __cacheline_padding_2[56];
	    volatile u_int64_t tail;
	    u_char __cacheline_padding_3[56];
	    u_int64_t shadow_tail;
	    u_char __cacheline_padding_4[56];
	    T** items;

	} spsc_queue_t;


	SpscQ(){
	    spsc = NULL;
	}

	~SpscQ(){
	    if(spsc != NULL){
		delete[] spsc->items;
		free(spsc);

	    }

	}

	/* qsize will be pow of 2 */
	void init(int qsize){
	    // min qsize = 256
	    if(qsize < 256) qsize = 256;
	    QSIZE = (int)pow((double)2, (int)ceil(log10(qsize + 1) / log10(2)));
	    QUEUE_ITEMS_MASK = QSIZE - 1;
	    CACHE_LINE_LEN = 64;
	    QUEUE_WATERMARK = 256; /* pow of 2 */
	    QUEUE_WATERMARK_MASK  = QUEUE_WATERMARK - 1;

	    spsc = new_spsc();
	    spsc->items = new T*[QSIZE];




	}

	inline int size(){
	    if(spsc->head >= spsc->tail) return spsc->head - spsc->tail;
	    else return QSIZE - (spsc->tail - spsc->head);
	}

	inline int pop(T** item) {
	    u_int32_t next_tail;

	    next_tail = (spsc->shadow_tail + 1) & QUEUE_ITEMS_MASK;
	    if(next_tail != spsc->head) {
		*item = spsc->items[next_tail];
		spsc->shadow_tail = next_tail;
		if((spsc->shadow_tail & QUEUE_WATERMARK_MASK) == 0) {
		    gcc_mb();
		    spsc->tail = spsc->shadow_tail;
		}
		return 0;
	    }
	    return 1;
	}

	inline int push(u_int8_t flush_item, T* item) {
	    u_int32_t next_head;
	    next_head = (spsc->shadow_head + 1) & QUEUE_ITEMS_MASK;
	    if (spsc->tail != next_head) {
		spsc->items[spsc->shadow_head] = item;

		spsc->shadow_head = next_head;
		if(flush_item || (spsc->shadow_head & QUEUE_WATERMARK_MASK) == 0) {
		    gcc_mb();
		    spsc->head = spsc->shadow_head;
		}
		return 0;
	    }
	    return 1;
	}

    private:
	spsc_queue_t* new_spsc() {
	    SpscQ<T>::spsc_queue_t* tmp_spsc = (SpscQ<T>::spsc_queue_t*)calloc(1, sizeof(SpscQ<T>::spsc_queue_t));
	    if (tmp_spsc == NULL) return NULL;
	    tmp_spsc->tail = tmp_spsc->shadow_tail = QSIZE - 1;
	    tmp_spsc->head = tmp_spsc->shadow_head = 0;
	    return tmp_spsc;
	}


	spsc_queue_t* spsc;
	int QUEUE_ITEMS_MASK;
	int CACHE_LINE_LEN;
	int QUEUE_WATERMARK;
	int QUEUE_WATERMARK_MASK;
	int QSIZE;
    };


}



#endif /* SPSCQ_H_ */
