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

#ifndef PMPCAP_H_
#define PMPCAP_H_

#include <string>
#include <map>
#include <deque>
#include <iostream>
#include <pthread.h>
#include <pool.h>
#include <semaphore.h>
#include <spscq.h>
#include <atomic.h>



#ifdef __PFRING__
#include <pfring.h>
#undef max

#else
#include <pcap.h>
#endif
using namespace std;



#ifdef __PFRING__
#define PCAP_ERRBUF_SIZE 256
#endif

struct packet_descriptor{
    unsigned char* packet;
    unsigned int caplen;
    void* args;
};

typedef lockfree::QItem<packet_descriptor, 0>* pcap_qitem_t;
typedef pcap_qitem_t (*pd_alloc_t)(void*);
typedef int (*pd_dealloc_t)(pcap_qitem_t, void*);
typedef int (*pd_start_t)(void*);
typedef int (*pd_finish_t)(void*);

struct stream_descriptor{
	#ifdef __PFRING__
	pfring *handle;
	#else
	pcap_t *handle;
	#endif
	int id;
	u_int MAX_Q_SIZE;
	u_int MAX_Q_LIMIT_REACHED;
	u_int ERR_POOL_EMPTY;
	u_int ERR_SNAPLEN_LIMIT;
	int snaplen;
	lockfree::SpscQ<lockfree::QItem<packet_descriptor, 0> > lfq;
	//memory::Pool<packet_descriptor> buff_pool;
	//unsigned char* packet_pld_buff;
	//sem_t sem_signal;
	pthread_mutex_t pool_lock;
	pthread_t thread;
	bool stopping;
	uint16_t core_id;
	void* pool_owner;
	pmink::Atomic<int> refc;

	// alloc/dealloc
	pd_alloc_t pd_alloc;
	pd_dealloc_t pd_dealloc;
	pd_start_t pd_start;
	pd_finish_t pd_finish;

};

class PMPcap {
public:
	static map<int, stream_descriptor*> stream_lst;

	PMPcap();
	static int getDevices(string *buffer);
	static void init();
	static stream_descriptor *initCapture(string device,
					      int snaplen,
					      int max_q_size,
					      unsigned int buff_packet_count,
					      unsigned int pcap_buffer,
					      pd_alloc_t _pd_alloc,
					      pd_dealloc_t _pd_dealloc,
					      void* pool_owner,
					      pd_start_t pd_start = NULL,
					      pd_finish_t pd_finish = NULL);
	static void startCapture(int id);
	static void stopCapture(int id);
	static void stopAll();

	#ifdef __PFRING__
	static void setBPF(pfring *handle, string filter);
	static pfring_stat* getStats(int id);
	static void getStats(int id, pfring_stat* ps);
	#else
	static void setBPF(pcap_t *handle, string filter);
	static pcap_stat* getStats(int id);
	static void getStats(int id, pcap_stat* ps);
	#endif

	static stream_descriptor* getStream(int id);
	static pcap_qitem_t popPacket(int id);
	//static int free_packet_descriptor(packet_descriptor* pd, int id);
	virtual ~PMPcap();
private:
	static char errbuf[PCAP_ERRBUF_SIZE];
	static int NEXT_STREAM_ID;
	static void addToStreamList(stream_descriptor *sd);

	#ifdef __PFRING__
	static void packet_received(const struct pfring_pkthdr *hdr, const u_char *packet, const u_char *id);
	#else
	static void packet_received(u_char *id, const struct pcap_pkthdr *hdr, const u_char *packet);
	#endif

	static void *capture_thread(void *args);
	static pthread_mutex_t stram_lst_mt;
};

#endif /* PMPCAP_H_ */
