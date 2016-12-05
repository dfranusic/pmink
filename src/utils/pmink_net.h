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

#ifndef PMINK_NET_H_
#define PMINK_NET_H_

#include <iostream>
#include <string.h>
#include <stdint.h>
#include <pmink_utils.h>

namespace pmink_net {
    /*
     * Ip protocol type
     */
    enum IPProtocolType {
        IP_PT_TCP 	= 0x06,
        IP_PT_UDP 	= 0x11,
        IP_PT_SCTP 	= 0x84,
        IP_PT_IP6 	= 0x29,
        IP_PT_IP4 	= 0x04,
        IP_PT_ICMP	= 0x01
    };

    /*
     * Ether type
     */
    enum EtherType {
        ETH_ET_IP4 	= 0x0800,
        ETH_ET_IP6 	= 0x86DD,
        ETH_ET_VLAN 	= 0x8100

    };

    /*
     * Tcp header
     */
    class TcpHeader {
    public:
        uint16_t src;
        uint16_t dst;
        uint32_t seq_num;
        uint32_t ack_num;
        uint8_t hdr_len;
        bool ns;
        bool cwr;
        bool ece;
        bool urg;
        bool ack;
        bool psh;
        bool rst;
        bool syn;
        bool fin;
        uint16_t win_size;
        uint16_t csum;
        uint16_t urg_pntr;
        uint16_t pld_len;
        unsigned char* pld;
    };

    /*
     * Tcp segment
     */
    class TcpSegment {
    public:
        uint32_t seq_num;
        uint32_t ack_num;
        unsigned char* pld;
        uint16_t pld_len;
        const void* args;
    };

    /*
     * Tcp 4-Tuple
     */
    class TcpTuple {
    public:
        uint8_t ip_len;
        __uint128_t src_ip;
        __uint128_t dst_ip;
        uint16_t src_port;
        uint16_t dst_port;

        bool operator ==(const TcpTuple& r) const;
        bool operator !=(const TcpTuple& r) const;
    };

    /*
     * Tcp stream
     */
    class TcpStream {
    public:
        /**
         * StreamType
         */
        enum StreamType {
            ST_COMPLETE         = 0,  /**< Full stream, started with SYN flag */
            ST_INCOMPLETE       = 1   /**< Incomplete stream, missed starting SYN flag */
        };

        // types
        typedef std::pair<const uint32_t, TcpSegment> segment_value_t;
        typedef boost::fast_pool_allocator<segment_value_t> segment_alloc_t;
        typedef std::map<uint32_t, 
                         TcpSegment,
                         std::less<uint32_t>,
                         segment_alloc_t > segment_map_t;
        typedef segment_map_t::const_iterator segment_c_it_t;
        typedef segment_map_t::iterator segment_it_t;

        // members
        TcpTuple id;
        StreamType type;
        uint32_t isn;
        uint32_t next_seq_num;
        bool connected;
        bool finished;
        void* args;
        segment_map_t segments;
    };

    /*
     * Tcp connection
     */
    class TcpConnection {
    public:
        TcpConnection();
        TcpConnection(const TcpConnection& o);

        TcpConnection& operator=(TcpConnection& o);
        bool finished();

        TcpStream in_strm;
        TcpStream out_strm;
        TcpStream* strm;
        int pld_type;
        const void* args;
    };

    /* Tcp tracker
     */
    class TcpTracker {
    public:
        // types
        typedef pmink_utils::CorrelationMap<TcpTuple, TcpConnection>::cmap_it_type it_type;
        typedef std::pair<TcpConnection*, int> process_res_t;
        typedef std::vector<const void*> args_vec_t;

        /**
         * ErrorCode
         */
        enum ErrorCode {
            EC_IP_VER_ERR       = -1, /**< Unknown IP version */
            EC_SKIPPED          = -2, /**< Skipped, neither existing nor new stream */
            EC_MAX_CONNECTIONS  = -3, /**< Connection skipped, too many active connections */
            EC_INVALID_SEQUENCE = -4, /**< Invalid sequence number */
            EC_NEW_CONNECTION   = 0,  /**< New connection detected */
            EC_NEW_SEGMENT      = 1,  /**< New segment buffered */ 
            EC_NOT_CONNECTED    = 2,  /**< Stream not connected (SYN not received) */
            EC_SYN_RECEIVED     = 3,  /**< New connection detected, SYN received */
            EC_MAX_SEGMENTS     = 4,  /**< Segment skipped, too many buffered segment */
            EC_NO_PAYLOAD       = 5,  /**< Existing connection, no payload */
            EC_FIN_RECEIVED     = 6,  /**< Just disconnected, FIN received */
            EC_RST_RECEIVED     = 7   /**< Just disconnected, RST received */
        };

        /**
         * Constructor
         * @param[in]   _syn_only       If true, start tracking ONLY when SYN 
         *                              flag is receive
         * @param[in]   _max_conns      Max number of tracked connections
         * @param[in]   _timeout        Connection inactivity timeout in seconds
         * @param[in]   _max_segments   Max number of buffered segments per connection
         */
        TcpTracker(bool _syn_only = true,
                   uint32_t _max_conns = 1000, 
                   uint32_t _timeout = 300,
                   uint32_t _max_segments = 100);

        /**
         * Destructor
         */
        ~TcpTracker();
   
        /**
         * Process TCP header
         * @param[in]   ip_ver  IP header version (4 or 6)
         * @param[in]   src_ip  Pointer to source ip address
         * @param[in]   dst_ip  Pointer to destination ip address
         * @param[in]   tcph    Pointer to TCP header
         * @param[in]   args    Pointer to user data saved with
         *                      current segment (EC_NEW_SEGMENT only)
         * @return      A pair with its member pair::first set to 
         *              current TCP connection pointer. The pair::second
         *              element in the pair is an ErrorCode
         */
        process_res_t process(uint8_t ip_ver, 
                              unsigned char* src_ip, 
                              unsigned char* dst_ip, 
                              const TcpHeader* tcph,
                              const void* args);
        
        /**
         * Reverse TCP 4-tuple
         * @param[in,out]  tpl  Pointer to TCP tuple
         */
        void tpl_rev(TcpTuple* tpl);

        /**
         * Process SYN, FIN flags
         * @param[in,out]   conn    Pointer to TCP connection
         * @param[in]       tcph    Pointer to TCP header
         */
        int process_flags(TcpConnection* conn, const TcpHeader* tcph);

        /**
         * Remove inactive and/or finished TCP connections 
         * @param[out]  segm_args   Pointer to output vector to be filled with
         *                          each removed segment's user data pointer (args)
         * @param[out]  strm_args   Pointer to output vector to be filled with
         *                          each removed stream's (IN/OUT) user data 
         *                          pointer (args)
         * @return      Number of removed TCP connections
         */
        int expire(args_vec_t* segm_args = NULL, args_vec_t* strm_args = NULL);

        /**
         * Remove TCP connection
         * @param[in]   conn        Pointer to TCP connection to be removed
         * @param[out]  segm_args   Pointer to output vector to be filled with
         *                          each removed segment's user data pointer (args)
         * @param[out]  strm_args   Pointer to output vector to be filled with
         *                          each removed stream's (IN/OUT) user data 
         *                          pointer (args)
         * @return      Zero (0) is removed or error code
         */
        int expire(TcpConnection* conn, args_vec_t* segm_args = NULL, args_vec_t* strm_args = NULL);

        /**
         * Reassemble buffered TCP segments
         * @param[in]   strm        Pointer to TCP stream
         * @param[out]  buff        Output buffer for reassembled data
         * @param[in]   buff_size   Size of output buffer
         * @param[out]  segm_args   Pointer to output vector to be filled with
         *                          each removed/reassembled segment's user data 
         *                          pointer (args)
         * @param[in]   max_seg_c   Max number of segements to reassemble; defaults
         *                          to NO LIMIT (same as 0)
         * @return      Number of bytes in output buffer (buff) or negative value
         *              for error code
         */
        int reassemble(TcpStream* strm, 
                       unsigned char* buff, 
                       uint32_t buff_size, 
                       args_vec_t* segm_args,
                       uint32_t max_seg_c = std::numeric_limits<uint32_t>::max());
        
        /**
         * Return iterator to begining
         * @return  An iterator to the first element in the container.
         */
        it_type begin();

        /**
         * Return iterator to end
         * @return  An iterator to the past-the-end element in the container
         */
        it_type end();
        
        /**
         * Remove connection by iterator
         * @param[in]   it  Connection iterator
         */
        void remove(it_type it);

        /**
         * Remove connection by 4-tuple (try reversed if not found)
         * @param[in]   tpl Connection 4-Tuple
         * @return      True if removed, False if not found
         */
        bool remove(const TcpTuple& tpl);

        /**
         * Find connection by 4-tuple (try reversed if not found)
         * @param[in]   tpl Connection 4-Tuple
         * @return      Connection iterator
         */
        it_type find(const TcpTuple& tpl);

        /**
         * Get connection map size
         * @return      Number of connections currently tracked
         */
        size_t size();

        /**
         * Get TCP connections timeout in seconds
         * @return  Connection timeout
         */
        uint32_t get_timeout();
        
        /**
         * Set TCP connectin timeout in seconds
         * @param[in]   _tomeout    Timeout value
         */
        void set_timeout(uint32_t _timeout);
        /**
         * Set SYN_ONLY flag
         * @param[in]   _syn_only   SYN_ONLY flag status
         */
        void set_syn_only(bool _syn_only);

        /**
         * Get SYN_ONLY flag
         * @return  SYN_ONLY value
         */
        bool get_syn_only();

        /**
         * Get max number of tracked connections
         * @return  Max number of tracked connections
         */
        uint32_t get_max_conns();
        
        /**
         * Set max number of tracked connections
         * @param[in]   _max_conns  Max number of tracked connections
         */
        void set_max_conns(uint32_t _max_conns);

        /**
         * Get max number of buffered segments per connection
         * @return  Max number of buffered segments per connection
         */
        uint32_t get_max_segments();
        
        /**
         * Set max number of buffered segments per connection
         * @param[in]   _max_conns  Max number of buffered segments
         */
        void set_max_segments(uint32_t _max_segms);

    private:
        pmink_utils::CorrelationMap<TcpTuple, TcpConnection> conns;
        uint32_t max_conns;
        uint32_t max_segments;
        uint32_t tcp_timeout;
        bool syn_only;
    };

    // tcp decode
    int tcp_decode(unsigned char* data, unsigned int data_length, TcpHeader* tcph);

}

#endif /* end of include guard: PMINK_NET_H_ */
