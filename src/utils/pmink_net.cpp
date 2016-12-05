/*
 * This file is part of Project MINK <http://www.release14.org>.
 *
 * Copyr (C) 2012 Release14 Ltd.
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

#include <pmink_net.h>

// TcpTuple
bool pmink_net::TcpTuple::operator ==(const TcpTuple& r) const {
    if(ip_len != r.ip_len) return false;
    if(src_ip != r.src_ip) return false;
    if(dst_ip != r.dst_ip) return false;
    if(src_port != r.src_port) return false;
    if(dst_port != r.dst_port) return false;
    return true;
}

bool pmink_net::TcpTuple::operator !=(const TcpTuple& r) const {
    if(ip_len != r.ip_len) return true;
    if(src_ip != r.src_ip) return true;
    if(dst_ip != r.dst_ip) return true;
    if(src_port != r.src_port) return true;
    if(dst_port != r.dst_port) return true;
    return false;
}

// TcpConnection
pmink_net::TcpConnection::TcpConnection(const TcpConnection& o): in_strm(o.in_strm),
                                                                 out_strm(o.out_strm),
                                                                 strm(NULL),
                                                                 pld_type(0),
                                                                 args(NULL){
    if(o.strm == &o.out_strm) strm = &out_strm;
    else if(o.strm == &o.in_strm) strm = &in_strm;
}


pmink_net::TcpConnection::TcpConnection(): in_strm(),
                                           out_strm(),
                                           strm(NULL),
                                           pld_type(0),
                                           args(NULL){}


pmink_net::TcpConnection& pmink_net::TcpConnection::operator=(TcpConnection& o){
    in_strm = o.in_strm;
    out_strm = o.out_strm;
    strm = NULL;
    pld_type = o.pld_type;
    if(o.strm == &o.out_strm) strm = &out_strm;
    else if(o.strm == &o.in_strm) strm = &in_strm;
    return *this;
}

bool pmink_net::TcpConnection::finished(){
    return (in_strm.finished && out_strm.finished);
}

// TcpTracker
pmink_net::TcpTracker::TcpTracker(bool _syn_only,
                                  uint32_t _max_conns, 
                                  uint32_t _timeout,
                                  uint32_t _max_segments): conns(_max_conns, _timeout),
                                                           max_conns(_max_conns),
                                                           max_segments(_max_segments),
                                                           tcp_timeout(_timeout),
                                                           syn_only(_syn_only){
                                   

}

pmink_net::TcpTracker::~TcpTracker(){

}

uint32_t pmink_net::TcpTracker::get_timeout(){
    return tcp_timeout;
}

void pmink_net::TcpTracker::set_timeout(uint32_t _timeout){
    tcp_timeout = _timeout;
    conns.set_timeout(_timeout);
}

void pmink_net::TcpTracker::set_syn_only(bool _syn_only){
    syn_only = _syn_only;
}

bool pmink_net::TcpTracker::get_syn_only(){
    return syn_only;
}

uint32_t pmink_net::TcpTracker::get_max_conns(){
    return max_conns;
}

void pmink_net::TcpTracker::set_max_conns(uint32_t _max_conns){
    max_conns = _max_conns;
}

uint32_t pmink_net::TcpTracker::get_max_segments(){
    return max_segments;
}

void pmink_net::TcpTracker::set_max_segments(uint32_t _max_segms){
    max_segments = _max_segms;
}


int pmink_net::TcpTracker::process_flags(TcpConnection* conn, const TcpHeader* tcph){
    // check for SYN
    if(tcph->syn){
        // check if not connected
        if(!conn->strm->connected){
            // set isn
            conn->strm->isn = tcph->seq_num;
            // next relative seq num
            conn->strm->next_seq_num = 1;
            // set connected flag
            conn->strm->connected = true;
            // stream type
            conn->strm->type = TcpStream::ST_COMPLETE;
            // SYN received
            return EC_SYN_RECEIVED;
        }
    // check for FIN
    }else if(tcph->fin){
        // check if connected
        if(conn->strm->connected){
            // set finished flag
            conn->strm->finished = true;
            // FIN received
            return EC_FIN_RECEIVED;
        }
    // check for RST
    }else if(tcph->rst){
        // set finished flag for both streams
        conn->in_strm.finished = true;
        conn->out_strm.finished = true;
        // RST received
        return EC_RST_RECEIVED;
    }
    // default return, no flags
    return EC_NEW_SEGMENT;
}

int pmink_net::TcpTracker::expire(TcpConnection* conn, args_vec_t* segm_args, args_vec_t* strm_args){
    // find conn by 4-tuple id (try reversed also)
    it_type it = find(conn->out_strm.id);
    if(it == conns.end()) return -1;
    // clear args_lst
    if(segm_args != NULL) segm_args->clear();
    if(strm_args != NULL) strm_args->clear();
    // *** gather user args for each buffered segment ***
    if(segm_args != NULL){
        // ref to in_stream segmenet map
        TcpStream::segment_map_t& in_map = conn->in_strm.segments;
        // ref to out_stream segment map
        TcpStream::segment_map_t& out_map = conn->out_strm.segments;
        // IN stream
        for(TcpStream::segment_it_t it_s = in_map.begin(); it_s != in_map.end(); it_s++){
            // check for valid user args
            if(it_s->second.args != NULL) segm_args->push_back(it_s->second.args);
        }
        // OUT stream
        for(TcpStream::segment_it_t it_s = out_map.begin(); it_s != out_map.end(); it_s++){
            // check for valid user args
            if(it_s->second.args != NULL) segm_args->push_back(it_s->second.args);
        }
    }
    // *** gather user args for IN/OUT streams ***
    if(strm_args != NULL){
        if(conn->in_strm.args != NULL) strm_args->push_back(conn->in_strm.args);
        if(conn->out_strm.args != NULL) strm_args->push_back(conn->out_strm.args);
    }
    // remove connection
    conns.remove(it);
    // ok
    return 0;
 
}

int pmink_net::TcpTracker::expire(args_vec_t* segm_args, args_vec_t* strm_args){
    // current ts
    time_t now = time(NULL);
    // res couter
    int res = 0;
    // clear args_lst
    if(segm_args != NULL) segm_args->clear();
    if(strm_args != NULL) strm_args->clear();
    // loop connections
    for(it_type it = conns.begin(), it_next = it; it != conns.end(); it = it_next){
        // conn reference
        TcpConnection& conn = it->second.data;
        // next conn
        ++it_next;
        // 1st condition = both streams finished
        bool test_1 = conn.in_strm.finished && conn.out_strm.finished;
        // 2nd condition = no activity timeout
        bool test_2 = now - it->second.ts > it->second.data_timeout;
        // if conditions not met, continue to next conn
        if(!(test_1 || test_2)) continue;
        // *** gather user args for each buffered segment ***
        if(segm_args != NULL){
            // ref to in_stream segmenet map
            TcpStream::segment_map_t& in_map = conn.in_strm.segments;
            // ref to out_stream segment map
            TcpStream::segment_map_t& out_map = conn.out_strm.segments;
            // IN stream
            for(TcpStream::segment_it_t it_s = in_map.begin(); it_s != in_map.end(); it_s++){
                // check for valid user args
                if(it_s->second.args != NULL) segm_args->push_back(it_s->second.args);
            }
            // OUT stream
            for(TcpStream::segment_it_t it_s = out_map.begin(); it_s != out_map.end(); it_s++){
                // check for valid user args
                if(it_s->second.args != NULL) segm_args->push_back(it_s->second.args);
            }
        }
        // *** gather user args for IN/OUT streams ***
        if(strm_args != NULL){
            if(conn.in_strm.args != NULL) strm_args->push_back(conn.in_strm.args);
            if(conn.out_strm.args != NULL) strm_args->push_back(conn.out_strm.args);
        }
        // remove connection
        conns.remove(it);
        // inc res counter
        ++res;
    }
    // return number of removed connections
    return res;
}

pmink_net::TcpTracker::it_type pmink_net::TcpTracker::begin(){
    return conns.begin();
}

pmink_net::TcpTracker::it_type pmink_net::TcpTracker::end(){
    return conns.end();
}

void pmink_net::TcpTracker::remove(it_type it){
    conns.remove(it);
}

pmink_net::TcpTracker::it_type pmink_net::TcpTracker::find(const TcpTuple& tpl){
    // find
    it_type it = conns.get_it(tpl);
    // try reveresed
    if(it == conns.end()){
        // copy tpl
        TcpTuple tpl_cpy = tpl;
        // reverse
        tpl_rev(&tpl_cpy);
        // find again
        it = conns.get_it(tpl_cpy);
    }
    // return iterator
    return it;
}


bool pmink_net::TcpTracker::remove(const TcpTuple& tpl){
    // find
    it_type it = conns.get_it(tpl);
    // try reveresed
    if(it == conns.end()){
        // copy tpl
        TcpTuple tpl_cpy = tpl;
        // reverse
        tpl_rev(&tpl_cpy);
        // find again
        it = conns.get_it(tpl_cpy);
    }
    // remove if found
    if(it != conns.end()){
        conns.remove(it);
        // found and removed
        return true;
    }
    // default = not found
    return false;
}

size_t pmink_net::TcpTracker::size(){
    return conns.size();
}

int pmink_net::TcpTracker::reassemble(TcpStream* strm, 
                                      unsigned char* buff, 
                                      uint32_t buff_size, 
                                      args_vec_t* segm_args,
                                      uint32_t max_seg_c){
    // sanity check
    if(strm == NULL || buff == NULL || segm_args == NULL) return -1;
    // check if connected
    if(!strm->connected) return -2;
    // clear args_lst
    segm_args->clear();
    // get next expected segment
    TcpStream::segment_it_t it = strm->segments.find(strm->next_seq_num);
    // check if segments found
    if(it == strm->segments.end()) return -3;
    // set buff counter
    uint32_t buff_c = 0;
    // set segm counter
    uint32_t segm_c = 0;
    // max_seg_c zero value check
    if(max_seg_c == 0) max_seg_c = std::numeric_limits<uint32_t>::max();
    // loop segments
    while(it != strm->segments.end()){
        // segment ref
        TcpSegment& seg = it->second;
        // buffer overflow check
        if(seg.pld_len > buff_size) break;
        // copy to out buffer
        memcpy(&buff[buff_c], seg.pld, seg.pld_len);
        // inc buff bytes written
        buff_c += seg.pld_len;
        // inc segment counter
        ++segm_c;
        // dec buff size
        buff_size -= seg.pld_len;
        // update next expected seq num
        strm->next_seq_num += seg.pld_len;
        // add user args to args_lst
        if(seg.args != NULL) segm_args->push_back(seg.args);
        // remove segment
        strm->segments.erase(it);
        // check max segment count
        if(segm_c >= max_seg_c) break;
        // next segment
        it = strm->segments.find(strm->next_seq_num);
    }
    // return bytes written
    return buff_c;
}


pmink_net::TcpTracker::process_res_t pmink_net::TcpTracker::process(uint8_t ip_ver,
                                                                    unsigned char* src_ip,
                                                                    unsigned char* dst_ip,
                                                                    const TcpHeader* tcph,
                                                                    const void* args){
    // create 4-tuple
    TcpTuple tpl = {};
    tpl.src_port = tcph->src;
    tpl.dst_port = tcph->dst;
    // check ip ver
    switch(ip_ver){
        // ipv4
        case 4:
            tpl.ip_len = 4;
            memcpy(&tpl.src_ip, src_ip, 4);
            memcpy(&tpl.dst_ip, dst_ip, 4);
            break;

        // ipv6
        case 6:
            tpl.ip_len = 16;
            memcpy(&tpl.src_ip, src_ip, 16);
            memcpy(&tpl.dst_ip, dst_ip, 16);
            break;

        default: return process_res_t(NULL, EC_IP_VER_ERR);
    }

    // find
    it_type it = conns.get_it(tpl);
    // if not found try reversed
    if(it == conns.end()){
        // reverse
        tpl_rev(&tpl);
        // find reversed
        it = conns.get_it(tpl);
        // reverse back to original
        tpl_rev(&tpl);
    }
    // connection found
    if(it != conns.end()){
        // update ts
        conns.update_ts(it);
        // get conn reference
        TcpConnection& conn = it->second.data;
        // detect stream (IN/OUT)
        if(tpl == conn.out_strm.id) conn.strm = &conn.out_strm;
        else conn.strm = &conn.in_strm;
        // process flags (SYN, FIN)
        process_flags(&conn, tcph);
        // if not connected and tracking without SYN is enabled
        if(!syn_only && !conn.strm->connected){
            // isn seq sanity check
            if(tcph->seq_num == 0) return process_res_t(&conn, EC_INVALID_SEQUENCE);
            // set type
            conn.strm->type = TcpStream::ST_INCOMPLETE;
            // dec isn by 1 (to be consistent with ST_COMPLETE, when SYN consumes 1 byte)
            conn.strm->isn = tcph->seq_num - 1;
            // next relative seq num
            conn.strm->next_seq_num = 1;
            // set connected flag
            conn.strm->connected = true;
        }
        // check if connected
        if(!conn.strm->connected) return process_res_t(&conn, EC_NOT_CONNECTED);
        // check max buffered segments
        if(conn.strm->segments.size() >= max_segments) return process_res_t(&conn, EC_MAX_SEGMENTS);
        // payload data check, only intersted in data packets
        if(tcph->pld_len == 0) return process_res_t(&conn, EC_NO_PAYLOAD);
        // create segment
        TcpSegment segm = {tcph->seq_num,
                           tcph->ack_num,
                           tcph->pld,
                           tcph->pld_len,
                           args};

        //+-----------------------------------------------------------------------------+
        //| * if SYN_ONLY if set to false, connection tracking can start with the       |
        //|   last ACK (after both FINs) and EC_INVALID_SEQUENCE can occur if newly     |
        //|   established connection reuses that ip/port combination                    |
        //| * if this happens, TCP connection should be removed from the tracking       |
        //|   list (expire/remove); that will cause the next packet belonging to tha    |
        //|   TCP connection to re-initiate tracking and sequnce numbers will be fixed  |
        //+-----------------------------------------------------------------------------+
        // seq sanity check
        if(tcph->seq_num <= conn.strm->isn) return process_res_t(&conn, EC_INVALID_SEQUENCE);
        // calculate relative sequence number
        uint32_t rel_seq = tcph->seq_num - conn.strm->isn;
        // add segment (skip duplicates/retransmissions)
        conn.strm->segments.insert(TcpStream::segment_value_t(rel_seq, segm));
        // return conn
        return process_res_t(&conn, EC_NEW_SEGMENT);

    // new connecton
    }else{
        //+-------------------------------------------------+
        //| SYN_ONLY = true                                 |
        //| * new connection starting only with SYN segment |
        //+-------------------------------------------------+
        bool test_1 = tcph->syn && !tcph->ack && tcph->ack_num == 0;

        //+--------------------------------------------------------------+
        //| SYN_ONLY = false                                             |
        //| * new connection starting anytime (mid-stream) with segment  |
        //|   meeting the following conditions:                          |
        //|                                                              |
        //|      1. segment doesn't contain FIN or RST flags             |
        //|      2. segment carries a payload                            |
        //+--------------------------------------------------------------+
        bool test_2 = !syn_only && !tcph->fin && !tcph->rst && tcph->pld_len > 0;

        // check if conditions for new conn have been met
        if(!(test_1 || test_2)) return process_res_t(NULL, EC_SKIPPED);
        // isn seq sanity check
        if(tcph->seq_num == 0) return process_res_t(NULL, EC_INVALID_SEQUENCE);
        // new connection
        TcpConnection conn;
        // current stream = outbound stream
        conn.strm = &conn.out_strm;
        // set 4-tuple id for both directions
        conn.in_strm.id = tpl;
        conn.out_strm.id = tpl;
        // reverse IN stream 4-tuple
        tpl_rev(&conn.in_strm.id);
        // set isn
        conn.strm->isn = tcph->seq_num;
        // in case of ST_INCOMPLETE stream (no SYN), dec isn isn by 1
        // (to be consistent with ST_COMPLETE, when SYN consumes 1 byte)
        if(!tcph->syn) --conn.strm->isn;
        // next relative seq num
        conn.strm->next_seq_num = 1;
        // set connected flag for OUT stream
        conn.strm->connected = true;
        // stream type
        conn.strm->type = (tcph->syn ? TcpStream::ST_COMPLETE : TcpStream::ST_INCOMPLETE);
        // add to list
        it = conns.set(tpl, conn);
        // if max num of connections reached, return error
        if(it == conns.end()) return process_res_t(NULL, EC_MAX_CONNECTIONS);
        // in case of INCOMPLETE stream type, add segment if payload exists
        if(conn.strm->type == TcpStream::ST_INCOMPLETE && tcph->pld_len > 0){
            // create segment
            TcpSegment segm = {tcph->seq_num,
                               tcph->ack_num,
                               tcph->pld,
                               tcph->pld_len,
                               args};

            // calculate relative sequence number
            uint32_t rel_seq = tcph->seq_num - conn.strm->isn;
            // add segment
            it->second.data.strm->segments.insert(TcpStream::segment_value_t(rel_seq, segm));
            // return if new connection started with regular data segment
            return process_res_t(&it->second.data, EC_NEW_SEGMENT);
        }
        // return if new connection started(SYN or no SYN)
        return process_res_t(&it->second.data, EC_NEW_CONNECTION);
    }

    // * default return, neither existing stream nor start of new stream
    // * in case of syn_only set to false, EC_SKIPPED is never returned
    return process_res_t(NULL, EC_SKIPPED);
}


void pmink_net::TcpTracker::tpl_rev(TcpTuple* tpl){
    // save tmp copy of src ip/port
    __uint128_t tmp_src_ip = tpl->src_ip;
    uint16_t tmp_src_port= tpl->src_port;
    // reverse ips
    tpl->src_ip = tpl->dst_ip;
    tpl->dst_ip = tmp_src_ip;
    // reverse ports
    tpl->src_port = tpl->dst_port;
    tpl->dst_port = tmp_src_port;
}


int pmink_net::tcp_decode(unsigned char* data, unsigned int data_length, TcpHeader* tcph){
    if(data != NULL && data_length >= 20 && tcph != NULL){
        // source and destination ports
        tcph->src = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
        tcph->dst = ((data[2] & 0xff) << 8) + (data[3] & 0xff);

        // sequence number and ack number
        tcph->seq_num = ((data[4] & 0xff) << 24) +
                        ((data[5] & 0xff) << 16) +
                        ((data[6] & 0xff) << 8) +
                        ((data[7] & 0xff));

        tcph->ack_num = ((data[8] & 0xff) << 24) +
                        ((data[9] & 0xff) << 16) +
                        ((data[10] & 0xff) << 8) +
                        ((data[11] & 0xff));

        // header length
        tcph->hdr_len= ((data[12] & 0xff) >> 4)* 32 / 8;

        // flags
        tcph->ns = (data[12] & 0x01) == 0x01;
        tcph->cwr = (data[13] & 0x80) == 0x80;
        tcph->ece = (data[13] & 0x40) == 0x40;
        tcph->urg = (data[13] & 0x20) == 0x20;
        tcph->ack = (data[13] & 0x10) == 0x10;
        tcph->psh = (data[13] & 0x08) == 0x08;
        tcph->rst = (data[13] & 0x04) == 0x04;
        tcph->syn = (data[13] & 0x02) == 0x02;
        tcph->fin = (data[13] & 0x01) == 0x01;

        // window size, checksum, urg pointer
        tcph->win_size = ((data[14] & 0xff) << 8) + (data[15] & 0xff);
        tcph->csum = ((data[16] & 0xff) << 8) + (data[17] & 0xff);
        tcph->urg_pntr = ((data[18] & 0xff) << 8) + (data[19] & 0xff);

        // payload
        if(tcph->hdr_len< data_length){
            data += tcph->hdr_len;
            tcph->pld = data;
            tcph->pld_len = data_length - tcph->hdr_len;

            // no payload
        }else{
            tcph->pld = NULL;
            tcph->pld_len = 0;
        }

        return 0;
    }

    return 1;

}
