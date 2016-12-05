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

#include <pmpcap.h>
#include <string.h>
#include <unistd.h>
using namespace std;

// static list of active streams
map<int, stream_descriptor*> PMPcap::stream_lst;
char PMPcap::errbuf[PCAP_ERRBUF_SIZE];
int PMPcap::NEXT_STREAM_ID;
pthread_mutex_t PMPcap::stram_lst_mt;

PMPcap::PMPcap() {
}


void PMPcap::init(){
	//Pcap::MAX_Q_LENGTH = l;
	PMPcap::NEXT_STREAM_ID = 1;
	// init mutex
	pthread_mutex_init(&PMPcap::stram_lst_mt, NULL);
	bzero(errbuf, sizeof(errbuf));

}
PMPcap::~PMPcap() {

}
void PMPcap::addToStreamList(stream_descriptor *sd){
	//TODO
	// should use mutex
	pthread_mutex_lock(&PMPcap::stram_lst_mt);

	stream_lst[sd->id] = sd;
	pthread_mutex_unlock(&PMPcap::stram_lst_mt);
	//stream_lst.push_back(sd);


}
#ifdef __PFRING__
stream_descriptor *PMPcap::initCapture(string device,
				       int snaplen,
				       int max_q_size,
				       unsigned int buff_packet_count,
				       unsigned int pcap_buffer,
				       pd_alloc_t _pd_alloc,
				       pd_dealloc_t _pd_dealloc,
				       void* pool_owner,
				       pd_start_t pd_start,
				       pd_finish_t pd_finish){

	stream_descriptor *sd = NULL;
	int flags = PF_RING_PROMISC;
	pfring *handle = pfring_open(const_cast<char*>(device.c_str()), snaplen, flags);
	if(handle == NULL){
		cout << "pfring_open: error while loading!" << endl;

	}else{
		int err = pfring_enable_ring(handle);
		if(err == 0){
				sd = new stream_descriptor;
				// set values
				sd->handle = handle;
				sd->snaplen = snaplen;
				sd->id = PMPcap::NEXT_STREAM_ID++;
				sd->MAX_Q_SIZE = max_q_size;
				sd->MAX_Q_LIMIT_REACHED = 0;
				sd->ERR_POOL_EMPTY = 0;
				sd->ERR_SNAPLEN_LIMIT = 0;
				sd->pd_alloc = _pd_alloc;
				sd->pd_dealloc = _pd_dealloc;
				sd->pd_start = pd_start;
				sd->pd_finish = pd_finish;
				sd->pool_owner = pool_owner;
				sd->stopping = false;
				sd->refc.set(1);
				//sd->q.set_capacity(max_q_size);
				//sd->tmp_q.set_capacity(max_q_size);
				sd->lfq.init(buff_packet_count);

				//sd->q = new deque<packet_descriptor*>;
				//sd->tmp_q = new deque<packet_descriptor*>;
				//sd->buff_pool.init(buff_packet_count);
				//sd->buff_pool.construct_objects();

				// create MTU based buffer
				/*
				sd->packet_pld_buff = new unsigned char[snaplen * buff_packet_count];
				packet_descriptor* tmp_arr[buff_packet_count];
				for(unsigned int i = 0; i<buff_packet_count; i++){
				    tmp_arr[i] = sd->buff_pool.allocate_constructed();
				    tmp_arr[i]->packet = &sd->packet_pld_buff[i * snaplen];
				}
				for(unsigned int i = 0; i<buff_packet_count; i++) sd->buff_pool.deallocate_constructed(tmp_arr[i]);
				*/

				sd->core_id = 65535;
				// init mutexes and semaphores
				//pthread_mutex_init(&sd->q_lock, NULL);
				pthread_mutex_init(&sd->pool_lock, NULL);
				//sem_init(&sd->sem_signal, 0, 0);
				//add to list
				addToStreamList(sd);
				// run start handler
				if(sd->pd_start != NULL) sd->pd_start(pool_owner);

		}else cout << "pfring_enable_ring: error while enabling ring, erro number = " << err << endl;

	}
	return sd;
}
#else

stream_descriptor *PMPcap::initCapture(string device,
				       int snaplen,
				       int max_q_size,
				       unsigned int buff_packet_count,
				       unsigned int pcap_buffer,
				       pd_alloc_t _pd_alloc,
				       pd_dealloc_t _pd_dealloc,
				       void* pool_owner,
				       pd_start_t pd_start,
				       pd_finish_t pd_finish){
	stream_descriptor *sd = NULL;

	pcap_t* handle = pcap_create(device.c_str(), errbuf);
	if(handle == NULL){
		cout << errbuf << endl;
	}else{
		// set buffer
		pcap_set_buffer_size(handle, pcap_buffer);
		//pcap_t *handle = pcap_open_live(device.c_str(), snaplen, 1, 10000, errbuf);
		pcap_set_promisc(handle, 1);
		pcap_activate(handle);
		sd = new stream_descriptor;
		// set values
		sd->handle = handle;
		sd->snaplen = snaplen;
		sd->id = PMPcap::NEXT_STREAM_ID++;
		sd->MAX_Q_SIZE = max_q_size;
		sd->MAX_Q_LIMIT_REACHED = 0;
		sd->ERR_POOL_EMPTY = 0;
		sd->ERR_SNAPLEN_LIMIT = 0;
		sd->pd_alloc = _pd_alloc;
		sd->pd_dealloc = _pd_dealloc;
		sd->pd_start = pd_start;
		sd->pd_finish = pd_finish;
		sd->pool_owner = pool_owner;
		sd->stopping = false;
		sd->refc.set(1);
		//sd->q.set_capacity(max_q_size);
		//sd->tmp_q.set_capacity(max_q_size);
		sd->lfq.init(buff_packet_count);

		//sd->q = new deque<packet_descriptor*>;
		//sd->tmp_q = new deque<packet_descriptor*>;
		//sd->buff_pool.init(buff_packet_count);
		//sd->buff_pool.construct_objects();

		// create MTU based buffer
		/*
		sd->packet_pld_buff = new unsigned char[snaplen * buff_packet_count];
		packet_descriptor* tmp_arr[buff_packet_count];
		for(unsigned int i = 0; i<buff_packet_count; i++){
		    tmp_arr[i] = sd->buff_pool.allocate_constructed();
		    tmp_arr[i]->packet = &sd->packet_pld_buff[i * snaplen];
		}
		for(unsigned int i = 0; i<buff_packet_count; i++) sd->buff_pool.deallocate_constructed(tmp_arr[i]);
*/

		sd->core_id = 65535;
		// init mutexes and semaphores
		//pthread_mutex_init(&sd->q_lock, NULL);
		pthread_mutex_init(&sd->pool_lock, NULL);
		//sem_init(&sd->sem_signal, 0, 0);
		//add to list
		addToStreamList(sd);
		// run start handler
		if(sd->pd_start != NULL) sd->pd_start(pool_owner);


	}
	return sd;

}
#endif

stream_descriptor* PMPcap::getStream(int id){
	stream_descriptor *sd = NULL;
	pthread_mutex_lock(&PMPcap::stram_lst_mt);
	map<int, stream_descriptor*>::iterator it = stream_lst.find(id);
	if(it != stream_lst.end()) sd = it->second;
	pthread_mutex_unlock(&PMPcap::stram_lst_mt);
	return sd;
}

#ifdef __PFRING__
void PMPcap::stopCapture(int id){
	stream_descriptor *sd = getStream(id);
	if(sd){
		pfring_breakloop(sd->handle);
	}

}
#else
void PMPcap::stopCapture(int id){
	stream_descriptor *sd = getStream(id);
	if(sd){
		pcap_breakloop(sd->handle);
	}

}
#endif


#ifdef __PFRING__
void PMPcap::stopAll(){
	pthread_mutex_lock(&PMPcap::stram_lst_mt);
	typedef map<int, stream_descriptor*>::iterator it_type;
	for(it_type it = stream_lst.begin(); it != stream_lst.end(); it++){
	    //sem_post(&it->second->sem_signal);
	    pfring_breakloop(it->second->handle);
	}
	pthread_mutex_unlock(&PMPcap::stram_lst_mt);

}

#else
void PMPcap::stopAll(){
	pthread_mutex_lock(&PMPcap::stram_lst_mt);
	typedef map<int, stream_descriptor*>::iterator it_type;
	for(it_type it = stream_lst.begin(); it != stream_lst.end(); it++){
	    //sem_post(&it->second->sem_signal);
	    pcap_breakloop(it->second->handle);
	}
	pthread_mutex_unlock(&PMPcap::stram_lst_mt);

}
#endif


/*
int PMPcap::free_packet_descriptor(packet_descriptor* pd, int id){
	stream_descriptor *sd = getStream(id);
	if(sd){
	    int res = sd->pd_dealloc(pd);
	    return res;
	}
	return -1;
}
*/


void PMPcap::startCapture(int id){
	//cout << "startCapture " << (int)id << endl;
	stream_descriptor *sd = getStream(id);
	//cout << "startCapture " << (int)sd->id << endl;
	if(sd){
		//cout << "startCapture" << endl;
		// init thread
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
		pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
		sched_param sp;
		sp.__sched_priority = 99;
		pthread_attr_setschedparam(&attr, &sp);

		pthread_t thr;
		pthread_create(&thr, &attr, capture_thread, (void *)sd);
		// pthread name
		char pthread_name[15];
		sprintf(pthread_name, "pmpcap_%i", id);
		pthread_setname_np(thr, pthread_name);



		//pthread_join(sd->thread, NULL);
		//pcap_loop(sd->handle, -1, packet_received, &id);
	}else{
		cout << "Stream ID [ " << id << " ] not found!" << endl;
	}
}
pcap_qitem_t PMPcap::popPacket(int id){
	stream_descriptor *sd = getStream(id);
	pcap_qitem_t qitem = NULL;

	if(sd != NULL){
	    if(sd->lfq.pop(&qitem) == 0) return qitem;

	    /*
		pthread_mutex_lock(&sd->q_lock);
		//pthread_spin_lock(&sd->s_lock);
		if(!sd->q.empty()){
			pd = sd->q.front();
			sd->q.pop_front();
			// !!! free memory when done width popped packet!!
		}
		pthread_mutex_unlock(&sd->q_lock);
		//pthread_spin_unlock(&sd->s_lock);
		 *
		 */
	}
	return qitem;

}
#ifdef __PFRING__
pfring_stat* PMPcap::getStats(int id){
	pfring_stat *ps = NULL;
	stream_descriptor *sd = PMPcap::getStream(id);
	if(sd != NULL){
		ps = new pfring_stat;
		pfring_stats(sd->handle, ps);
		//cout << "Stats: recv=" << ps->ps_recv << ", drop=" << ps->ps_drop << ", ifdrop=" << ps->ps_ifdrop <<  endl;

	}
	return ps;


}

void PMPcap::getStats(int id, pfring_stat* ps){
    stream_descriptor *sd = PMPcap::getStream(id);
    if(sd != NULL) pfring_stats(sd->handle, ps);


}

#else
pcap_stat* PMPcap::getStats(int id){
	pcap_stat *ps = NULL;
	stream_descriptor *sd = PMPcap::getStream(id);
	if(sd != NULL){
		ps = new pcap_stat;
		pcap_stats(sd->handle, ps);

	}
	return ps;

}
void PMPcap::getStats(int id, pcap_stat* ps){
    stream_descriptor *sd = PMPcap::getStream(id);
    if(sd != NULL) pcap_stats(sd->handle, ps);

}


#endif






#ifdef __PFRING__
void* PMPcap::capture_thread(void *args){
	stream_descriptor *sd = (stream_descriptor *)args;
	packet_descriptor *pd;
	pcap_qitem_t qitem = NULL;
	sd->thread = pthread_self();

	if(sd->core_id != 65535){
	    // check if core id is valid
	    int core_count = sysconf(_SC_NPROCESSORS_CONF);

	    if(sd->core_id < core_count){
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(sd->core_id, &cpuset);
		pthread_setaffinity_np(sd->thread, sizeof(cpu_set_t), &cpuset);

	    }
	}

	// start loop
	pfring_loop(sd->handle, packet_received, (u_char *)sd, 1);

	sd->stopping = true;


	//while(sd->lfq.pop(&qitem) == 0){
	//    sd->pd_dealloc(qitem, sd->pool_owner);
	//}


/*
	// free queue element pointers
	pthread_mutex_lock(&sd->q_lock);
	while(!sd->q.empty()){
		pd = sd->q.front();
		sd->q.pop_front();
		//delete[] pd->packet;
		//pd->packet = NULL;
		//delete pd;
		//pd = NULL;
		pthread_mutex_lock(&sd->pool_lock);
		sd->buff_pool.deallocate_constructed(pd);
		pthread_mutex_unlock(&sd->pool_lock);
	}
	// free temp q
	while(!sd->tmp_q.empty()){
		pd = sd->tmp_q.front();
		sd->tmp_q.pop_front();
		//delete[] pd->packet;
		//pd->packet = NULL;
		//delete pd;
		//pd = NULL;
		pthread_mutex_lock(&sd->pool_lock);
		sd->buff_pool.deallocate_constructed(pd);
		pthread_mutex_unlock(&sd->pool_lock);
	}

	// free queue memory
	//delete sd->q;
	//delete sd->tmp_q;
	// free stream descriptor memory
	pthread_mutex_unlock(&sd->q_lock);
*/
	pfring_close(sd->handle);
	// remove from list
	pthread_mutex_lock(&PMPcap::stram_lst_mt);
	stream_lst.erase(sd->id);
	// detach thread
	pthread_detach(sd->thread);
	// destroy mutexes and semaphotes
	//pthread_mutex_destroy(&sd->q_lock);
	pthread_mutex_destroy(&sd->pool_lock);
	//sem_destroy(&sd->sem_signal);
	//delete[] sd->packet_pld_buff;
	//delete sd;
	pthread_mutex_unlock(&PMPcap::stram_lst_mt);
	// refc dec
	sd->refc.fetch_sub(1);
	// free when refs == 0
	while(sd->refc.get() > 0) sleep(1);
	// run finish handler
	if(sd->pd_finish != NULL) sd->pd_finish(sd->pool_owner);
	delete sd;

	return NULL;
}
#else
void* PMPcap::capture_thread(void *args){



	stream_descriptor *sd = (stream_descriptor *)args;
	sd->thread = pthread_self();

	if(sd->core_id != 65535){
	    // check if core id is valid
	    int core_count = sysconf(_SC_NPROCESSORS_CONF);

	    if(sd->core_id < core_count){
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(sd->core_id, &cpuset);
		pthread_setaffinity_np(sd->thread, sizeof(cpu_set_t), &cpuset);

	    }
	}

	// start loop
	pcap_loop(sd->handle, -1, packet_received, (u_char *)sd);
	//cout << "capture stopped" << endl;

	sd->stopping = true;
	// free queue element pointers
	//pthread_mutex_lock(&sd->q_lock);
	//while(sd->lfq.pop(&qitem) == 0){
	//    sd->pd_dealloc(qitem, sd->pool_owner);

	//}

	/*
	while(!sd->q.empty()){
		pd = sd->q.front();
		sd->q.pop_front();
		//delete[] pd->packet;
		//pd->packet = NULL;
		//delete pd;
		//pd = NULL;
		pthread_mutex_lock(&sd->pool_lock);
		sd->buff_pool.deallocate_constructed(pd);
		pthread_mutex_unlock(&sd->pool_lock);
	}
	// free temp q
	while(!sd->tmp_q.empty()){
		pd = sd->tmp_q.front();
		sd->tmp_q.pop_front();
		//delete[] pd->packet;
		//pd->packet = NULL;
		//delete pd;
		//pd = NULL;
		pthread_mutex_lock(&sd->pool_lock);
		sd->buff_pool.deallocate_constructed(pd);
		pthread_mutex_unlock(&sd->pool_lock);

	}
	*/

	// free queue memory
	//delete sd->q;
	//delete sd->tmp_q;
	// free stream descriptor memory
	//pthread_mutex_unlock(&sd->q_lock);

	pcap_close(sd->handle);
	// remove from list
	pthread_mutex_lock(&PMPcap::stram_lst_mt);
	stream_lst.erase(sd->id);
	// detach thread
	pthread_detach(sd->thread);
	// destroy mutexes and semaphotes
	//pthread_mutex_destroy(&sd->q_lock);
	pthread_mutex_destroy(&sd->pool_lock);
	//sem_destroy(&sd->sem_signal);
	//delete[] sd->packet_pld_buff;
	//delete sd;
	pthread_mutex_unlock(&PMPcap::stram_lst_mt);
	// refc dec
	sd->refc.fetch_sub(1);
	// free when refs == 0
	while(sd->refc.get() > 0) sleep(1);
	// run finish handler
	if(sd->pd_finish != NULL) sd->pd_finish(sd->pool_owner);
	delete sd;

	return NULL;
}
#endif

#ifdef __PFRING__
void PMPcap::packet_received(const struct pfring_pkthdr *hdr, const u_char *packet, const u_char *id){

	stream_descriptor *sd = (stream_descriptor*)id;
	packet_descriptor *packet_c;
	lockfree::QItem<packet_descriptor, 0>* qitem = NULL;
	int mutex_status;


	if(sd){
		// skip is packet too big
		if(hdr->len > sd->snaplen){
		    ++sd->ERR_SNAPLEN_LIMIT;
		    return;
		}

		// init packet descriptor
		qitem = sd->pd_alloc(sd->pool_owner);
		//packet_c = sd->pd_alloc(sd->pool_owner);
		if(qitem == NULL){
		    ++sd->ERR_POOL_EMPTY;
		    return;
		}
		// get packet_c
		packet_c = &qitem->data;

		/*
		pthread_mutex_lock(&sd->pool_lock);
		packet_c = sd->buff_pool.allocate_constructed();
		pthread_mutex_unlock(&sd->pool_lock);
		if(packet_c == NULL){
		    ++sd->ERR_POOL_EMPTY;
		    return;
		}
		*/

		packet_c->caplen = hdr->caplen;
		memcpy(packet_c->packet, packet, hdr->caplen);

		if(sd->lfq.push(1, qitem) == 1){
		    sd->MAX_Q_LIMIT_REACHED++;
		    sd->pd_dealloc(qitem, sd->pool_owner);
/*
		    pthread_mutex_lock(&sd->pool_lock);
		    sd->buff_pool.deallocate_constructed(packet_c);
		    pthread_mutex_unlock(&sd->pool_lock);
*/
		}




/*
		mutex_status = pthread_mutex_trylock(&sd->q_lock);

		// if mutex locked
		if(mutex_status == 0){

			// process temp queue
			while(!sd->tmp_q.empty()){
				// add to primary q if not full
				if(!sd->q.full()){
				    sd->q.push_back(sd->tmp_q.front());
				    sd->tmp_q.pop_front();

				    // signal
				    sem_post(&sd->sem_signal);


				// primary q full, deallocate
				}else{
				    sd->MAX_Q_LIMIT_REACHED++;

				    pthread_mutex_lock(&sd->pool_lock);
				    sd->buff_pool.deallocate_constructed(sd->tmp_q.front());
				    sd->tmp_q.pop_front();
				    pthread_mutex_unlock(&sd->pool_lock);

				}
			}
			//std::deque<packet_descriptor*>().swap(*(sd->tmp_q));



			if(!sd->q.full()){
				// add to queue
				sd->q.push_back(packet_c);
				// signal
				sem_post(&sd->sem_signal);

			}else{
				sd->MAX_Q_LIMIT_REACHED++;
				//cout << "[ " << sd->id << " ] Pcap::packet_received - Q FULL! [ " << sd->MAX_Q_SIZE << " ]" << endl;
				//delete[] packet_c->packet;
				//packet_c->packet = NULL;
				//delete packet_c;
				//packet_c = NULL;
				pthread_mutex_lock(&sd->pool_lock);
				sd->buff_pool.deallocate_constructed(packet_c);
				pthread_mutex_unlock(&sd->pool_lock);

			}
			pthread_mutex_unlock(&sd->q_lock);

		}else{
			if(!sd->tmp_q.full()){
				sd->tmp_q.push_back(packet_c);
			}else{
				//cout << "[ " << sd->id << " ] Pcap::packet_received - TMP_Q FULL! [ " << sd->MAX_Q_SIZE << " ]" << endl;
				//delete[] packet_c->packet;
				//packet_c->packet = NULL;
				//delete packet_c;
				//packet_c = NULL;
				pthread_mutex_lock(&sd->pool_lock);
				sd->buff_pool.deallocate_constructed(packet_c);
				pthread_mutex_unlock(&sd->pool_lock);
			}

		}
*/
	}



}
#else
void PMPcap::packet_received(u_char *id, const struct pcap_pkthdr *hdr, const u_char *packet){

	stream_descriptor *sd = (stream_descriptor*)id;
	packet_descriptor *packet_c;
	pcap_qitem_t qitem = NULL;
	//int mutex_status;


	if(sd){
		// skip is packet too big
		if(hdr->len > sd->snaplen){
		    ++sd->ERR_SNAPLEN_LIMIT;
		    return;
		}

		// init packet descriptor
		qitem = sd->pd_alloc(sd->pool_owner);
		//packet_c = sd->pd_alloc(sd->pool_owner);
		if(qitem == NULL){
		    ++sd->ERR_POOL_EMPTY;
		    return;
		}
		// get packet_c
		packet_c = &qitem->data;

/*
		pthread_mutex_lock(&sd->pool_lock);
		packet_c = sd->buff_pool.allocate_constructed();
		pthread_mutex_unlock(&sd->pool_lock);
		if(packet_c == NULL){
		    ++sd->ERR_POOL_EMPTY;
		    return;
		}
*/
		packet_c->caplen = hdr->caplen;
		memcpy(packet_c->packet, packet, hdr->caplen);


		if(sd->lfq.push(1, qitem) == 1){
		    sd->MAX_Q_LIMIT_REACHED++;
		    sd->pd_dealloc(qitem, sd->pool_owner);
		    /*
		    pthread_mutex_lock(&sd->pool_lock);
		    sd->buff_pool.deallocate_constructed(packet_c);
		    pthread_mutex_unlock(&sd->pool_lock);
		    */

		}


		/*

		// if mutex locked
		if(mutex_status == 0){

			// process temp queue
			while(!sd->tmp_q.empty()){
				// add to primary q if not full
				if(!sd->q.full()){
				    sd->q.push_back(sd->tmp_q.front());
				    sd->tmp_q.pop_front();

				    // signal
				    sem_post(&sd->sem_signal);


				// primary q full, deallocate
				}else{
				    sd->MAX_Q_LIMIT_REACHED++;

				    pthread_mutex_lock(&sd->pool_lock);
				    sd->buff_pool.deallocate_constructed(sd->tmp_q.front());
				    sd->tmp_q.pop_front();
				    pthread_mutex_unlock(&sd->pool_lock);

				}
			}
			//std::deque<packet_descriptor*>().swap(*(sd->tmp_q));



			if(!sd->q.full()){
				// add to queue
				sd->q.push_back(packet_c);
				// signal
				sem_post(&sd->sem_signal);

			}else{
				sd->MAX_Q_LIMIT_REACHED++;
				//cout << "[ " << sd->id << " ] Pcap::packet_received - Q FULL! [ " << sd->MAX_Q_SIZE << " ]" << endl;
				//delete[] packet_c->packet;
				//packet_c->packet = NULL;
				//delete packet_c;
				//packet_c = NULL;
				pthread_mutex_lock(&sd->pool_lock);
				sd->buff_pool.deallocate_constructed(packet_c);
				pthread_mutex_unlock(&sd->pool_lock);

			}
			pthread_mutex_unlock(&sd->q_lock);

		}else{
			if(!sd->tmp_q.full()){
				sd->tmp_q.push_back(packet_c);
			}else{
				//cout << "[ " << sd->id << " ] Pcap::packet_received - TMP_Q FULL! [ " << sd->MAX_Q_SIZE << " ]" << endl;
				//delete[] packet_c->packet;
				//packet_c->packet = NULL;
				//delete packet_c;
				//packet_c = NULL;
				pthread_mutex_lock(&sd->pool_lock);
				sd->buff_pool.deallocate_constructed(packet_c);
				pthread_mutex_unlock(&sd->pool_lock);
			}

		}
*/

	}


}
#endif


#ifdef __PFRING__
void PMPcap::setBPF(pfring *handle, string filter){
	// TODO
}
#else
void PMPcap::setBPF(pcap_t *handle, string filter){
	struct bpf_program bpf;
	bpf_u_int32 netmask = 0;
	if(pcap_compile(handle, &bpf, filter.c_str(), 0, netmask) == -1){
		cout << "BPF Filter error: [ " << filter << " ]" << endl;
	}else{
		if(pcap_setfilter(handle, &bpf) == -1){
			cout << "Cannot set filter: [ " << filter << " ]" << endl;
		}
	}
}
#endif

#ifdef __PFRING__
int PMPcap::getDevices(string *buffer){
	// TODO
}
#else
int PMPcap::getDevices(string *buffer){
	pcap_if_t *alldevs, *d;
	int i = 0;
	if(pcap_findalldevs(&alldevs, errbuf) != -1){
		for(d = alldevs; d; d = d->next){
			buffer[i] = d->name;
			i++;
		}
	}
	delete alldevs;
	delete d;

	return i;
}
#endif
