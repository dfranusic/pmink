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

#include <iostream>
#include <tcp_ip.h>
#include <string.h>
#include <daemon.h>
#include <pmink_utils.h>

// namespace
std::ostream& tcpip::operator<< (std::ostream& out, const TCP_UDPStreamId& sid){
    for(int i = 0; i<sid.source_ip_size - 1; i++) out << (int)sid.source_ip[i] << ".";
    out << (int)sid.source_ip[sid.source_ip_size - 1] << ":" << sid.source_port << " -> ";
    for(int i = 0; i<sid.dest_ip_size - 1; i++) out << (int)sid.dest_ip[i] << ".";
    out << (int)sid.dest_ip[sid.dest_ip_size - 1] << ":" << sid.dest_port;
    return out;
}


tcpip::ETHHeader* tcpip::get_eth_header(unsigned char* data, int data_length){
	if(data != NULL && data_length >= 14){
		ETHHeader* ethh = new ETHHeader();
		// layer type
		ethh->layer_type = OSI_LT_802_3;
		ethh->layer_number = 2;
		// raw header pointer
		ethh->buff_hdr_p = data;
		// raw header size
		ethh->buff_hdr_size = data_length;
		// destination
		ethh->destination_length = 6;
		ethh->destination = data;
		// move fwd
		data += 6;
		// source
		ethh->source_length = 6;
		ethh->source = data;
		// move fwd
		data += 6;
		// ether type
		ethh->ether_type = (EtherType)(((data[0] & 0xff) << 8) + (data[1] & 0xff));
		// move fwd
		data += 2;
		// payload
		ethh->payload_length = data_length - 14;
		ethh->payload = data;
		// result
		return ethh;

	}
	return NULL;
}

int tcpip::get_eth_header(unsigned char* data, int data_length, ETHHeader* ethh){
	if(data != NULL && data_length >= 14 && ethh != NULL){
		// layer type
		ethh->layer_type = OSI_LT_802_3;
		ethh->layer_number = 2;
		// raw header pointer
		ethh->buff_hdr_p = data;
		// raw header size
		ethh->buff_hdr_size = data_length;
		// destination
		ethh->destination_length = 6;
		ethh->destination = data;
		// move fwd
		data += 6;
		// source
		ethh->source_length = 6;
		ethh->source = data;
		// move fwd
		data += 6;
		// ether type
		ethh->ether_type = (EtherType)(((data[0] & 0xff) << 8) + (data[1] & 0xff));
		// move fwd
		data += 2;
		// payload
		ethh->payload_length = data_length - 14;
		ethh->payload = data;

		return 0;
	}

	return 1;
}



tcpip::IPHeader* tcpip::get_ip_header(unsigned char* data, int data_length){
	if(data != NULL && data_length >= 20){
		IPHeader* iph = new IPHeader();
		// raw header pointer
		iph->buff_hdr_p = data;
		// raw header size
		iph->buff_hdr_size = data_length;
		// first four bits are IP version
		iph->vesion = *data >> 4;

		// IPv4
		if(iph->vesion == 4){
		    // last 4 bits are header length in 32bit words
		    iph->header_length = (int)(*data & 0x0f)*32 / 8;
		    // layer type
		    iph->layer_type = OSI_LT_IPV4;
		    iph->layer_number = 3;
		    // fwd
		    data += 2;
		    //IP total length(2 bytes) = IP header length + all the other following headers
		    iph->total_length = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
		    // fwd
		    data += 2;
		    // identification
		    iph->identification = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
		    // fwd
		    data += 2;
		    // fragmentation
		    iph->do_not_fragment = (*data & 0x40) == 0x40;
		    iph->more_fragments = (*data & 0x20) == 0x20;
		    iph->fragment_offset = (((data[0] & 0x1f) << 8) + (data[1] & 0xff)) * 8;
		    // fwd
		    data += 2;
		    // ttl
		    iph->time_to_live = (*data & 0xff);
		    // fwd
		    data++;
		    // protocol
		    iph->protocol = (IPProtocolType)(*data & 0xff);
		    // fwd
		    data++;
		    // checksum
		    iph->checksum_length = 2;
		    iph->checksum = data;
		    // fwd
		    data += 2;
		    // source
		    iph->source_length = 4;
		    iph->source = data;
		    // fwd
		    data += 4;
		    // destination
		    iph->destination_length = 4;
		    iph->destination = data;
		    // fwd
		    data += 4;
		    // payload
		    iph->payload_length = iph->total_length - iph->header_length;
		    iph->total_payload_length = iph->payload_length;
		    iph->payload = data;

		// IPv6
		}else if(iph->vesion == 6){
		    // fixed header length of 40 bytes
		    iph->header_length = 40;
		    // layer type
		    iph->layer_type = OSI_LT_IPV6;
		    iph->layer_number = 3;
		    // traffic class
		    iph->traffic_class = ((data[0] & 0x0f) << 4) + ((data[1] & 0xf0) >> 4);
		    // fwd
		    ++data;
		    // flow label
		    iph->flow_label = ((data[0] * 0x0f) << 16) + (data[1] << 8) + (data[2] & 0xff);
		    // fwd
		    data += 3;
		    // payload length
		    iph->payload_length = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
		    iph->total_payload_length = iph->payload_length;
		    iph->total_length = iph->payload_length + iph->header_length;
		    // fwd
		    data += 2;
		    // next header
		    iph->protocol = (IPProtocolType)(*data & 0xff);
		    // fwd
		    ++data;
		    // hop limit
		    iph->hop_limit = *data * 0xff;
		    // fwd
		    ++data;
		    // source address
		    iph->source_length = 16;
		    iph->source = data;
		    // fwd
		    data += 16;
		    // destination address
		    iph->source_length = 16;
		    iph->source = data;
		    // fwd
		    data += 16;
		    iph->payload = data;

		}else return NULL;
		return iph;
	}
	return NULL;
}

int tcpip::get_ip_header(unsigned char* data, int data_length, IPHeader* iph){
	if(data != NULL && data_length >= 20 && iph != NULL){
		// raw header pointer
		iph->buff_hdr_p = data;
		// raw header size
		iph->buff_hdr_size = data_length;
		// first four bits are IP version
		iph->vesion = *data >> 4;

		// IPv4
		if(iph->vesion == 4){
		    // last 4 bits are header length in 32bit words
		    iph->header_length = (int)(*data & 0x0f)*32 / 8;
		    // layer type
		    iph->layer_type = OSI_LT_IPV4;
		    iph->layer_number = 3;
		    // fwd
		    data += 2;
		    //IP total length(2 bytes) = IP header length + all the other following headers
		    iph->total_length = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
		    // fwd
		    data += 2;
		    // identification
		    iph->identification = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
		    // fwd
		    data += 2;
		    // fragmentation
		    iph->do_not_fragment = (*data & 0x40) == 0x40;
		    iph->more_fragments = (*data & 0x20) == 0x20;
		    iph->fragment_offset = (((data[0] & 0x1f) << 8) + (data[1] & 0xff)) * 8;
		    // fwd
		    data += 2;
		    // ttl
		    iph->time_to_live = (*data & 0xff);
		    // fwd
		    data++;
		    // protocol
		    iph->protocol = (IPProtocolType)(*data & 0xff);
		    // fwd
		    data++;
		    // checksum
		    iph->checksum_length = 2;
		    iph->checksum = data;
		    // fwd
		    data += 2;
		    // source
		    iph->source_length = 4;
		    iph->source = data;
		    // fwd
		    data += 4;
		    // destination
		    iph->destination_length = 4;
		    iph->destination = data;
		    // fwd
		    data += 4;
		    // payload length (calculated from IP header and also from buffer length)
		    int pldl_1 = iph->total_length - iph->header_length;
		    int pldl_2 = data_length  - iph->header_length;
		    // check if payload length in IP header is valid
		    if(pldl_1 > pldl_2) iph->payload_length = pldl_2; else iph->payload_length = pldl_1;
		    iph->total_payload_length = iph->payload_length;
		    iph->payload = data;

		// IPv6
		}else if(iph->vesion == 6){
		    // fixed header length of 40 bytes
		    iph->header_length = 40;
		    // layer type
		    iph->layer_type = OSI_LT_IPV6;
		    iph->layer_number = 3;
		    // traffic class
		    iph->traffic_class = ((data[0] & 0x0f) << 4) + ((data[1] & 0xf0) >> 4);
		    // fwd
		    ++data;
		    // flow label
		    iph->flow_label = ((data[0] * 0x0f) << 16) + (data[1] << 8) + (data[2] & 0xff);
		    // fwd
		    data += 3;
		    // payload length
		    iph->payload_length = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
		    iph->total_payload_length = iph->payload_length;
		    iph->total_length = iph->payload_length + iph->header_length;
		    // fwd
		    data += 2;
		    // next header
		    iph->protocol = (IPProtocolType)(*data & 0xff);
		    // fwd
		    ++data;
		    // hop limit
		    iph->hop_limit = *data * 0xff;
		    // fwd
		    ++data;
		    // source address
		    iph->source_length = 16;
		    iph->source = data;
		    // fwd
		    data += 16;
		    // destination address
		    iph->destination_length = 16;
		    iph->destination = data;
		    // fwd
		    data += 16;
		    // payload
		    iph->payload = data;

		}else return 1;

		return 0;
	}
	return 1;
}



tcpip::SLLHeader* tcpip::get_sll_header(unsigned char* data, int data_length){
	if(data != NULL && data_length > 0){
		SLLHeader* slh = new SLLHeader();
		slh->packet_type = ((data[0]  & 0xff)<< 8) + (data[1] & 0xff);
		// fwd
		data += 2;
		slh->address_type = ((data[0]  & 0xff)<< 8) + (data[1] & 0xff);
		// fwd
		data += 2;
		slh->source_length = 8;
		slh->source = data;
		// fwd
		data += slh->source_length;
		slh->protocol =  ((data[0]  & 0xff)<< 8) + (data[1] & 0xff);
		// fwd
		data += 2;
		slh->payload_length = data_length - 16;
		slh->payload = data;
		// result
		return slh;
	}

	return NULL;

}

tcpip::VLANHeader* tcpip::get_vlan_header(unsigned char* data, int data_length){
	if(data != NULL && data_length > 0){
		VLANHeader* vlanh = new VLANHeader();
		vlanh->priority = (VLANPriority)(*data & 0xe0);
		vlanh->cfi_indicator = (*data & 0x10) == 0x10;
		vlanh->vlan_identifier = ((data[0] & 0x0f) << 8) + (data[1] & 0xff);
		// fwd
		data += 2;
		vlanh->ether_type = (EtherType)(((data[0] & 0xff) << 8) + (data[1] & 0xff));
		// fwd
		data += 2;
		vlanh->payload_length = data_length - 4;
		vlanh->payload = data;
		// result
		return vlanh;
	}
	return NULL;
}

int tcpip::get_vlan_header(unsigned char* data, int data_length, VLANHeader* vlanh){
    if(data != NULL && data_length >= 4 && vlanh != NULL){
	vlanh->priority = (VLANPriority)(*data & 0xe0);
	vlanh->cfi_indicator = (*data & 0x10) == 0x10;
	vlanh->vlan_identifier = ((data[0] & 0x0f) << 8) + (data[1] & 0xff);
	// fwd
	data += 2;
	vlanh->ether_type = (EtherType)(((data[0] & 0xff) << 8) + (data[1] & 0xff));
	// fwd
	data += 2;
	vlanh->payload_length = data_length - 4;
	vlanh->payload = data;
	// result ok
	return 0;
    }
    return 1;
}


int tcpip::get_udp_header(unsigned char* data, int data_length, UDPHeader* udph){
	if(data != NULL && data_length >= 8 && udph != NULL){
		// layer type
		udph->layer_type = OSI_LT_UDP;
		udph->layer_number = 4;
		// raw header pointer
		udph->buff_hdr_p = data;
		// raw header size
		udph->buff_hdr_size = data_length;
		// source
		udph->source = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
		// destination
		udph->destination = ((data[2] & 0xff) << 8) + (data[3] & 0xff);
		// length
		udph->length = ((data[4] & 0xff) << 8) + (data[5] & 0xff);
		// checksum
		udph->checksum = ((data[6] & 0xff) << 8) + (data[7] & 0xff);
		// payload
		udph->payload_length = data_length - 8;
		if(udph->payload_length == 0) udph->payload = NULL;
		else udph->payload = &data[8];

		return 0;

	}

	return 1;
}



int tcpip::get_tcp_header(unsigned char* data, unsigned int data_length, TCPHeader* tcph){
	if(data != NULL && data_length >= 20 && tcph != NULL){
		//tcph->used = false;
		// layer type
		tcph->layer_type = OSI_LT_TCP;
		tcph->layer_number = 4;

		// raw header pointer
		tcph->buff_hdr_p = data;
		// raw header size
		tcph->buff_hdr_size = data_length;

		// source and destination ports
		tcph->source = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
		tcph->destination = ((data[2] & 0xff) << 8) + (data[3] & 0xff);

		// sequence number and ack number
		tcph->sequence_number = ((data[4] & 0xff) << 24) +
					((data[5] & 0xff) << 16) +
					((data[6] & 0xff) << 8) +
					((data[7] & 0xff));

		tcph->ack_number = 	((data[8] & 0xff) << 24) +
					((data[9] & 0xff) << 16) +
					((data[10] & 0xff) << 8) +
					((data[11] & 0xff));

		// header length
		tcph->header_length = ((data[12] & 0xff) >> 4)* 32 / 8;

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
		tcph->window_size = ((data[14] & 0xff) << 8) + (data[15] & 0xff);
		tcph->checksum = ((data[16] & 0xff) << 8) + (data[17] & 0xff);
		tcph->urg_pointer = ((data[18] & 0xff) << 8) + (data[19] & 0xff);

		// payload
		if(tcph->header_length < data_length){
			data += tcph->header_length;
			tcph->payload = data;
			tcph->payload_length = data_length - tcph->header_length;

		// no payload
		}else{
			tcph->payload = NULL;
			tcph->payload_length = 0;
		}

		return 0;
	}

	return 1;
}


tcpip::TCPHeader* tcpip::get_tcp_header(unsigned char* data, unsigned int data_length){
	if(data != NULL && data_length >= 20){
		TCPHeader* tcph = new TCPHeader();
		// layer type
		tcph->layer_type = OSI_LT_TCP;
		tcph->layer_number = 4;
		// raw header pointer
		tcph->buff_hdr_p = data;
		// raw header size
		tcph->buff_hdr_size = data_length;
		// source and destination ports
		tcph->source = ((data[0] & 0xff) << 8) + (data[1] & 0xff);
		tcph->destination = ((data[2] & 0xff) << 8) + (data[3] & 0xff);

		// sequence number and ack number
		tcph->sequence_number = ((data[4] & 0xff) << 24) +
					((data[5] & 0xff) << 16) +
					((data[6] & 0xff) << 8) +
					((data[7] & 0xff));

		tcph->ack_number = 	((data[8] & 0xff) << 24) +
					((data[9] & 0xff) << 16) +
					((data[10] & 0xff) << 8) +
					((data[11] & 0xff));

		// header length
		tcph->header_length = ((data[12] & 0xff) >> 4)* 32 / 8;

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
		tcph->window_size = ((data[14] & 0xff) << 8) + (data[15] & 0xff);
		tcph->checksum = ((data[16] & 0xff) << 8) + (data[17] & 0xff);
		tcph->urg_pointer = ((data[18] & 0xff) << 8) + (data[19] & 0xff);

		// payload
		if(tcph->header_length < data_length){
			data += tcph->header_length;
			tcph->payload = data;
			tcph->payload_length = data_length - tcph->header_length;

		// no payload
		}else{
			tcph->payload = NULL;
			tcph->payload_length = 0;
		}

		return tcph;
	}
	return NULL;
}


// Layer7Classifier
tcpip::Layer7Classifier::Layer7Classifier(){
    portmap[25] = &email_smtp;
    portmap[110] = &email_pop3;
    portmap[1812] = &radius;
    portmap[1813] = &radius;
    portmap[1645] = &radius;
    portmap[1646] = &radius;
    portmap[67] = &dhcp;
    portmap[68] = &dhcp;

}


tcpip::L7ProtocolType tcpip::Layer7Classifier::classify(	OSILayerType osi_l4_type,
								unsigned char* data,
								unsigned int data_length,
								unsigned int src_port,
								unsigned int dst_port){
    // sanity check
    if(data == NULL || data_length == 0) return L7_PT_UNKNOWN;

    // default res
    L7ProtocolType res = L7_PT_UNKNOWN;

    // === check standard ports first ===
    // check src port
    clsf_it it = portmap.find(src_port);
    // check dest port
    if(it == portmap.end()) it = portmap.find(dst_port);

    // run if found
    if(it != portmap.end()){
	// try to detect
	res = it->second(osi_l4_type, data, data_length);
	// return if found
	if(res != L7_PT_UNKNOWN) return res;
    }

    // === payload check ===
    // pop3
    res = email_pop3(osi_l4_type, data, data_length);
    if(res != L7_PT_UNKNOWN) return res;

    // smtp
    res = email_smtp(osi_l4_type, data, data_length);
    if(res != L7_PT_UNKNOWN) return res;

    // radius
    res = radius(osi_l4_type, data, data_length);
    if(res != L7_PT_UNKNOWN) return res;

    // dhcp
    res = dhcp(osi_l4_type, data, data_length);
    if(res != L7_PT_UNKNOWN) return res;



    // unknown
    return L7_PT_UNKNOWN;
}

tcpip::L7ProtocolType tcpip::Layer7Classifier::dhcp(OSILayerType osi_l4_type, unsigned char* data, unsigned int data_length){
    // dhco uses UDP
    if(osi_l4_type != OSI_LT_UDP) return L7_PT_UNKNOWN;
    // min length is 20 bytes
    if(data_length >= 244) {
	// magic cookie and dhcp message type option
	if(	data[236] == 0x63 &&
		data[237] == 0x82 &&
	        data[238] == 0x53 &&
	        data[239] == 0x63 &&
	        data[240] == 0x35 &&
	        data[241] == 0x01) return L7_PT_DHCP;
    }

    return L7_PT_UNKNOWN;

}

tcpip::L7ProtocolType tcpip::Layer7Classifier::radius(OSILayerType osi_l4_type, unsigned char* data, unsigned int data_length){
    // radius uses UDP
    if(osi_l4_type != OSI_LT_UDP) return L7_PT_UNKNOWN;
    // min length is 20 bytes
    if(data_length >= 20) {
	// check code ( 1 - 5 and 11 - 13)
	if((*data > 0 && *data < 6) || (*data > 10 && *data < 14)){
	    // probably radius (nothing else to match)
	    return L7_PT_RADIUS;
	}
    }
    return L7_PT_UNKNOWN;
}

tcpip::L7ProtocolType tcpip::Layer7Classifier::email_smtp(OSILayerType osi_l4_type, unsigned char* data, unsigned int data_length){
    // smtp uses TCP
    if(osi_l4_type != OSI_LT_TCP) return L7_PT_UNKNOWN;
    if(data_length > 2 && data[data_length - 2] == 0x0d && data[data_length - 1] == 0x0a) {
        // expected server responses
        if(data_length >= 3) {
          if (memcmp(data, "220", 3) == 0) {
              // 220
              return L7_PT_SMTP;
          }else if (memcmp(data, "250", 3) == 0) {
              // 250
              return L7_PT_SMTP;
          }else if (memcmp(data, "235", 3) == 0) {
              // 235
              return L7_PT_SMTP;
          }else if (memcmp(data, "334", 3) == 0) {
              // 334
              return L7_PT_SMTP;
          }else if (memcmp(data, "354", 3) == 0) {
              // 354
              return L7_PT_SMTP;
          }
        }
        /*
        // expected client requests
        if (packet->line[a].len >= 5) {
          if ((((packet->line[a].ptr[0] == 'H' || packet->line[a].ptr[0] == 'h')
                && (packet->line[a].ptr[1] == 'E' || packet->line[a].ptr[1] == 'e'))
               || ((packet->line[a].ptr[0] == 'E' || packet->line[a].ptr[0] == 'e')
                   && (packet->line[a].ptr[1] == 'H' || packet->line[a].ptr[1] == 'h')))
              && (packet->line[a].ptr[2] == 'L' || packet->line[a].ptr[2] == 'l')
              && (packet->line[a].ptr[3] == 'O' || packet->line[a].ptr[3] == 'o')
              && packet->line[a].ptr[4] == ' ') {
            flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_HELO_EHLO;
          } else if ((packet->line[a].ptr[0] == 'M' || packet->line[a].ptr[0] == 'm')
                     && (packet->line[a].ptr[1] == 'A' || packet->line[a].ptr[1] == 'a')
                     && (packet->line[a].ptr[2] == 'I' || packet->line[a].ptr[2] == 'i')
                     && (packet->line[a].ptr[3] == 'L' || packet->line[a].ptr[3] == 'l')
                     && packet->line[a].ptr[4] == ' ') {
            flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_MAIL;
          } else if ((packet->line[a].ptr[0] == 'R' || packet->line[a].ptr[0] == 'r')
                     && (packet->line[a].ptr[1] == 'C' || packet->line[a].ptr[1] == 'c')
                     && (packet->line[a].ptr[2] == 'P' || packet->line[a].ptr[2] == 'p')
                     && (packet->line[a].ptr[3] == 'T' || packet->line[a].ptr[3] == 't')
                     && packet->line[a].ptr[4] == ' ') {
            flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_RCPT;
          } else if ((packet->line[a].ptr[0] == 'A' || packet->line[a].ptr[0] == 'a')
                     && (packet->line[a].ptr[1] == 'U' || packet->line[a].ptr[1] == 'u')
                     && (packet->line[a].ptr[2] == 'T' || packet->line[a].ptr[2] == 't')
                     && (packet->line[a].ptr[3] == 'H' || packet->line[a].ptr[3] == 'h')
                     && packet->line[a].ptr[4] == ' ') {
            flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_AUTH;
          }
        }

      }
      */
    }
    return L7_PT_UNKNOWN;
}

tcpip::L7ProtocolType tcpip::Layer7Classifier::email_pop3(OSILayerType osi_l4_type, unsigned char* data, unsigned int data_length){
    // pop3 uses TCP
    if(osi_l4_type != OSI_LT_TCP) return L7_PT_UNKNOWN;
    if((data_length > 3
		&& (data[0] == '+' && (data[1] == 'O' || data[1] == 'o')
		    && (data[2] == 'K' || data[2] == 'k')))
		|| (data_length > 4
		    && (data[0] == '-' && (data[1] == 'E' || data[1] == 'e')
			    && (data[2] == 'R' || data[2] == 'r')
                            && (data[3] == 'R' || data[3] == 'r')))) {
            // +OK or -ERR seen
	    return L7_PT_POP;

    }else{
	// pop3 commands
        if(data_length > 4) {
                if ((data[0] == 'A' || data[0] == 'a')
                        && (data[1] == 'U' || data[1] == 'u')
                        && (data[2] == 'T' || data[2] == 't')
                        && (data[3] == 'H' || data[3] == 'h')) {
			// AUTH
                        return L7_PT_POP;
                } else if ((data[0] == 'A' || data[0] == 'a')
                                   && (data[1] == 'P' || data[1] == 'p')
                                   && (data[2] == 'O' || data[2] == 'o')
                                   && (data[3] == 'P' || data[3] == 'p')) {
			// APOP
                        return L7_PT_POP;
                } else if ((data[0] == 'U' || data[0] == 'u')
                                   && (data[1] == 'S' || data[1] == 's')
                                   && (data[2] == 'E' || data[2] == 'e')
                                   && (data[3] == 'R' || data[3] == 'r')) {
			// USER
                        return L7_PT_POP;
                } else if ((data[0] == 'P' || data[0] == 'p')
                                   && (data[1] == 'A' || data[1] == 'a')
                                   && (data[2] == 'S' || data[2] == 's')
                                   && (data[3] == 'S' || data[3] == 's')) {
			// PASS
                        return L7_PT_POP;
                } else if ((data[0] == 'C' || data[0] == 'c')
                                   && (data[1] == 'A' || data[1] == 'a')
                                   && (data[2] == 'P' || data[2] == 'p')
                                   && (data[3] == 'A' || data[3] == 'a')) {
			// CAPA
                        return L7_PT_POP;
                } else if ((data[0] == 'L' || data[0] == 'l')
                                   && (data[1] == 'I' || data[1] == 'i')
                                   && (data[2] == 'S' || data[2] == 's')
                                   && (data[3] == 'T' || data[3] == 't')) {
			// LIST
                        return L7_PT_POP;
                } else if ((data[0] == 'S' || data[0] == 's')
                                   && (data[1] == 'T' || data[1] == 't')
                                   && (data[2] == 'A' || data[2] == 'a')
                                   && (data[3] == 'T' || data[3] == 't')) {
			// STAT
                        return L7_PT_POP;
                } else if ((data[0] == 'U' || data[0] == 'u')
                                   && (data[1] == 'I' || data[1] == 'i')
                                   && (data[2] == 'D' || data[2] == 'd')
                                   && (data[3] == 'L' || data[3] == 'l')) {
			// UIDL
                        return L7_PT_POP;
                } else if ((data[0] == 'R' || data[0] == 'r')
                                   && (data[1] == 'E' || data[1] == 'e')
                                   && (data[2] == 'T' || data[2] == 't')
                                   && (data[3] == 'R' || data[3] == 'r')) {
			// RETR
                        return L7_PT_POP;
                } else if ((data[0] == 'D' || data[0] == 'd')
                                   && (data[1] == 'E' || data[1] == 'e')
                                   && (data[2] == 'L' || data[2] == 'l')
                                   && (data[3] == 'E' || data[3] == 'e')) {
			// DELE
                        return L7_PT_POP;
                } else if ((data[0] == 'S' || data[0] == 's')
                        && (data[1] == 'T' || data[1] == 't')
                        && (data[2] == 'L' || data[2] == 'l')
                        && (data[3] == 'S' || data[3] == 's')) {
			// STLS
			return L7_PT_POP;
                }
        }


    }
    // email
    /*
    if (data_length > 2 && data[data_length - 2] == 0x0d && data[data_length - 1] == 0x0a){
	// probably an email
	// detection should have already taken place before
	return true;
    }
    */

    return L7_PT_UNKNOWN;
}

/*
tcpip::TCPHeader::TCPHeader(){
    used = false;
}
*/

// IPHeader
tcpip::IPHeader::IPHeader(){
    //fragments.reserve(10);
    //is_fragmented = false;
}

tcpip::IPHeader::~IPHeader(){
    fragments.clear();
}

int tcpip::IPHeader::reassemble(unsigned char* buffer, unsigned int max_buffer_size){
    if(buffer != NULL && fragments.size() > 0 && max_buffer_size >= total_payload_length){
	// first fragment
	memcpy(&buffer[fragment_offset], payload, payload_length);
	// other fragments
	for(unsigned int i = 0; i<fragments.size(); i++){
		memcpy(&buffer[fragments[i]->fragment_offset], fragments[i]->payload, fragments[i]->payload_length);

	}
	// return total reassembled payload size
	return total_payload_length;


    }
    return 0;
}


// IPDefrag
tcpip::IPDefrag::IPDefrag(){

}

tcpip::IPDefrag::~IPDefrag(){
    frag_lst.clear();
}

//void tcpip::IPDefrag::lock(){
    // do nothing, implemented in derived classes
    // if necessary
//}
//void tcpip::IPDefrag::unlock(){
    // do nothing, implemented in derived classes
    // if necessary
//}


int tcpip::IPDefrag::free_frag(IPHeader* iph, bool free_main){
    return 0;

}

void tcpip::IPDefrag::process_timeout(unsigned int timeout){
    IPHeader* tmp = NULL;
    time_t now = time(NULL);
    for(frag_map_it it = frag_lst.begin(), it_next = it; it != frag_lst.end(); it = it_next){
	tmp = it->second;
	// next
	++it_next;
	// check if idle
	if(now - tmp->timestamp > timeout){
	    // remove
	    frag_lst.erase(it->first);
	    // free
	    free_frag(tmp, true);

	}
    }
}


int tcpip::IPDefrag::process(IPHeader* packet, IPHeader** result){
    if(packet != NULL){
	// no fragmentation for ipv6
	if(packet->vesion == 6) {
	    *result = packet;
	    return 0;
	    //return packet;
	}

	IPHeader* tmp_ipv4 = NULL;

	// detect fragmentation
	if(packet->more_fragments || packet->fragment_offset > 0){
	    // id
	    IPFragId tmp_id;
	    // source ip
	    tmp_id.source_ip_size = packet->source_length;
	    memcpy(tmp_id.source_ip, packet->source, packet->source_length);
	    // dest ip
	    tmp_id.dest_ip_size = packet->destination_length;
	    memcpy(tmp_id.dest_ip, packet->destination, packet->destination_length);
	    // identification
	    tmp_id.identifiction = packet->identification;

	    // update timestamp
	    packet->timestamp = time(NULL);

	    // reassemble
	    if(!packet->more_fragments){
		    // lock
		    //lock();
		    // find existing
		    frag_map_it it = frag_lst.find(tmp_id);
		    if(it != frag_lst.end()){
			// first fragment
			tmp_ipv4 = it->second;
			// add fragment to existing packet
			tmp_ipv4->fragments.push_back(packet);
			// calculate total payload length including all fragments
			tmp_ipv4->total_payload_length = packet->fragment_offset + packet->payload_length;
			// remove from list
			frag_lst.erase(tmp_id);
			// return prepared for reassembly
			*result = tmp_ipv4;
			return 0;
			//return tmp_ipv4;

		    }
		    // unlock
		    //unlock();

	    // save
	    }else{
		    // new fragment stream
		    if(packet->fragment_offset == 0){
			    // lock
			    //lock();
			    // check if id already exists
			    frag_map_it it = frag_lst.find(tmp_id);
			    // if stream with particular id does not exist
			    if(it == frag_lst.end()) {
				frag_lst[tmp_id] = packet;
			    }else{
				//std::cout << "!!!!FUCKUP 1!!!!" << std::endl;
				return 1;
			    }
			    // unlock
			    //unlock();

		    // existing stream
		    }else{
			    // lock
			    //lock();
			    // find existing
			    frag_map_it it = frag_lst.find(tmp_id);
			    if(it != frag_lst.end()){
				    tmp_ipv4 = it->second;
				    // add fragment to existing packet
				    tmp_ipv4->fragments.push_back(packet);
			    }else{
				//std::cout << "!!!!FUCKUP 2!!!!" << std::endl;
				return 1;
			    }
			    // unlock
			    //unlock();
		    }
		    // return NULL if waiting for more fragments
		    *result = NULL;
		    return 0;
		    //return NULL;

	    }
	// return packet, fragmentation NOT detected
	}else{
	    *result = packet;
	    return 0;
	    //return packet;
	}
    }

    return 2;
    //return NULL;
}

// DhcpReqRespCorrelation
tcpip::DhcpReqRespCorrelation::DhcpReqRespCorrelation(){
    finished = false;
    correlated = true;
    user_session = NULL;
}


// RadiusReqRespCorrelation
tcpip::RadiusReqRespCorrelation::RadiusReqRespCorrelation(){
    finished = false;
    user_session = NULL;
    acct = false;
    acct_st = 0;
}


// DhcpReqRespId
tcpip::DhcpReqRespId::DhcpReqRespId(){
    transaction_id = 0;
}

bool tcpip::DhcpReqRespId::operator ==(const DhcpReqRespId& right) const{
    if(right.stream_id != this->stream_id) return false;
    if(right.transaction_id != this->transaction_id) return false;
    return true;
}

bool tcpip::DhcpReqRespId::operator !=(const DhcpReqRespId& right) const{
    if(right.stream_id != this->stream_id) return true;
    if(right.transaction_id != this->transaction_id) return true;
    return false;
}



// RadiusReqRespId
tcpip::RadiusReqRespId::RadiusReqRespId(){
    packet_id = 0;

}

bool tcpip::RadiusReqRespId::operator ==(const RadiusReqRespId& right) const{
    if(right.stream_id != this->stream_id) return false;
    if(right.packet_id != this->packet_id) return false;
    return true;
}

bool tcpip::RadiusReqRespId::operator !=(const RadiusReqRespId& right) const{
    if(right.stream_id != this->stream_id) return true;
    if(right.packet_id != this->packet_id) return true;
    return false;
}

// DhcpSessionId
tcpip::DhcpSessionId::DhcpSessionId(){
    bzero(username, sizeof(username));
    username_size = 0;
}

bool tcpip::DhcpSessionId::operator ==(const DhcpSessionId& right) const{
    if(username_size != right.username_size) return false;
    if(memcmp(right.username, this->username, this->username_size) != 0) return false;
    return true;
}

bool tcpip::DhcpSessionId::operator !=(const DhcpSessionId& right) const{
    if(username_size != right.username_size) return true;
    if(memcmp(right.username, this->username, this->username_size) != 0) return true;
    return false;
}



// RadiusSessionId
tcpip::RadiusSessionId::RadiusSessionId(){
    bzero(username, sizeof(username));
}

bool tcpip::RadiusSessionId::operator ==(const RadiusSessionId& right) const{
    if(strcmp(right.username, this->username) != 0) return false;

    return true;
}

bool tcpip::RadiusSessionId::operator !=(const RadiusSessionId& right) const{
    if(strcmp(right.username, this->username) != 0) return true;

    return false;
}

// IPFragId
tcpip::IPFragId::IPFragId(){
    bzero(source_ip, sizeof(source_ip));
    bzero(dest_ip, sizeof(dest_ip));
    source_ip_size = 0;
    dest_ip_size = 0;
    identifiction = 0;

}

bool tcpip::IPFragId::operator ==(const IPFragId& right) const{
    if(right.identifiction != this->identifiction) return false;
    if(right.source_ip_size != this->source_ip_size) return false;
    if(right.dest_ip_size != this->dest_ip_size) return false;
    if(memcmp(right.source_ip, this->source_ip, right.source_ip_size) != 0) return false;
    if(memcmp(right.dest_ip, this->dest_ip, right.dest_ip_size) != 0) return false;
    return true;

}

bool tcpip::IPFragId::operator !=(const IPFragId& right) const{
    if(right.identifiction != this->identifiction) return true;
    if(right.source_ip_size != this->source_ip_size) return true;
    if(right.dest_ip_size != this->dest_ip_size) return true;
    if(memcmp(right.source_ip, this->source_ip, right.source_ip_size) != 0) return true;
    if(memcmp(right.dest_ip, this->dest_ip, right.dest_ip_size) != 0) return true;
    return false;

}



// TCP_UDPStreamId
tcpip::TCP_UDPStreamId::TCP_UDPStreamId(){
    memset(this, 0, sizeof(*this));
    /*
    bzero(source_ip, sizeof(source_ip));
    bzero(dest_ip, sizeof(dest_ip));
    source_ip_size = 0;
    dest_ip_size = 0;
    source_port = 0;
    dest_port = 0;
    */

}

bool tcpip::TCP_UDPStreamId::operator ==(const TCP_UDPStreamId& right) const{
    if(right.source_port != this->source_port) return false;
    if(right.dest_port != this->dest_port) return false;
    if(right.source_ip_size != this->source_ip_size) return false;
    if(right.dest_ip_size != this->dest_ip_size) return false;
    if(memcmp(right.source_ip, this->source_ip, right.source_ip_size) != 0) return false;
    if(memcmp(right.dest_ip, this->dest_ip, right.dest_ip_size) != 0) return false;

    return true;

    /*
    if(	right.source_port == this->source_port &&
	right.dest_port == this->dest_port &&
	memcmp(right.source_ip, this->source_ip, right.source_ip_size) == 0 &&
	memcmp(right.dest_ip, this->dest_ip, right.dest_ip_size) == 0
    )return true;

    return false;
    */

}


bool tcpip::TCP_UDPStreamId::operator !=(const TCP_UDPStreamId& right) const{
    if(right.source_port != this->source_port) return true;
    if(right.dest_port != this->dest_port) return true;
    if(right.source_ip_size != this->source_ip_size) return true;
    if(right.dest_ip_size != this->dest_ip_size) return true;
    if(memcmp(right.source_ip, this->source_ip, right.source_ip_size) != 0) return true;
    if(memcmp(right.dest_ip, this->dest_ip, right.dest_ip_size) != 0) return true;



    return false;

    /*
    if(	right.source_port != this->source_port ||
	right.dest_port != this->dest_port ||
	memcmp(right.source_ip, this->source_ip, right.source_ip_size) != 0 ||
	memcmp(right.dest_ip, this->dest_ip, right.dest_ip_size) != 0
    )return true;

    return false;
    */
}

// TCPConnection
tcpip::TCPConnection::TCPConnection(){
    current_stream = NULL;
    finished = false;
    l7_ptype = L7_PT_UNKNOWN;
    timestamp = 0;
    clsf_count = 0;
    stream_hash = 0;

}

// TCPStream
void tcpip::TCPStream::set_reversed(bool _is_reversed){
    reversed = _is_reversed;
}

void tcpip::TCPStream::add_segment(TCPHeader* tcph){
    segments.push_back(tcph);
}

void tcpip::TCPStream::set_connection(TCPConnection* conn){
    connection = conn;
}

void tcpip::TCPStream::clear_processed(){
    segments.erase(segments.begin(), segments.begin() + next_segment);
    next_segment = 0;

}

tcpip::TCPStream::TCPStream(){
    reversed = false;
    connection = NULL;
    //segments.reserve(1000);
    //offsets.reserve(1000);
    chunk_q = 0;
    isn = 0;
    last_seq_num = 0;
    next_segment = 0;
    byte_count = 0;
    seg_byte_count = 0;
    isn_set = false;
    finished = false;

}

void tcpip::TCPStream::reset(){
    reversed = false;
    connection = NULL;
    segments.clear();
    offsets.clear();
    chunk_q = 0;
    isn = 0;
    last_seq_num = 0;
    next_segment = 0;
    byte_count = 0;
    isn_set = false;
    finished = false;

}
tcpip::TCPStream::~TCPStream(){
    segments.clear();
    offsets.clear();
}

int tcpip::TCPStream::next_chunk(unsigned char* buffer, unsigned int max_buffer_length, bool *more, bool *retransmission){
    if(buffer != NULL){
	*more = false;
	*retransmission = false;
	// check if buffered data exists and ISN is set
	if(chunk_q <= 0 || !isn_set) return 0;

	bool done = false;
	//int i = next_segment;
	TCPHeader* tcph = NULL;
	uint32_t data_pos = 0;
	int res = 0;
	int max_used_buff = 0;
	bool incomplete = false;
	uint32_t rel_seq_num = 0;
	uint32_t tmp_rel_seq_num = 0;

	//std::cout << "next_segment: " << next_segment << std::endl;
	// offset
	uint32_t _offset = offsets[0];

	// loop segments
	while(!done && next_segment < segments.size()){
	    // segment pointer
	    tcph = segments[next_segment];
	    //std::cout << "TCPH: " << tcph << ", TCPH Payload L: " << tcph->checksum << " == " <<  tcph->payload_length << " - " << next_segment << " - " << segments.size() << std::endl;
	    // check for payload
	    if(tcph->payload_length > 0){
		    // get relative sequence number
		    rel_seq_num = tcph->sequence_number - isn;
		    // detect retransmission of PSH segment
		    if(rel_seq_num <= last_seq_num && tcph->psh) *retransmission = true;
		    // buffer position
		    data_pos = rel_seq_num - _offset;

		    // check for incomplete buffer
		    if(((data_pos + tcph->payload_length) <= max_buffer_length) && (data_pos < max_buffer_length)){
			// set last sequence number
			last_seq_num = rel_seq_num;

/*
			std::cout << "data_pos: " << data_pos << ", isn_set: " << isn_set << ", rel_sn: " << rel_seq_num << ", seq_num: " << tcph->sequence_number << ", isn: " << isn << ", offset: " << _offset << ", seg.size: " << tcph->payload_length << ", last seq. n: " << last_seq_num
				<< ", src port: " << tcph->destination << ", dest port: " << tcph->source
				<< ", TCP CONN: " << connection->stream_id.dest_port << ":" << connection->stream_id.source_port
				<< ", segm: " << next_segment << "/" << segments.size() << ", pld l: " << tcph->payload_length
				<< std::endl;
*/

			// copy payload data to proper position
			memcpy(&buffer[data_pos], tcph->payload, tcph->payload_length);
			// calculate buffer size (overlapping segments considered)
			max_used_buff = data_pos + tcph->payload_length;
			if(max_used_buff > res) res = max_used_buff;

		    // buffer full, more segments available (more flag)
		    }else{
			// check fur buffer overflow
			if(tcph->payload_length <= max_buffer_length){
/*
				std::cout << "***data_pos: " << data_pos << ", isn_set: " << isn_set << ", rel_sn: " << rel_seq_num << ", seq_num: " << tcph->sequence_number << ", isn: " << isn << ", offset: " << _offset << ", seg.size: " << tcph->payload_length << ", last seq. n: " << last_seq_num
					<< ", src port: " << tcph->destination << ", dest port: " << tcph->source
					<< ", TCP CONN: " << connection->stream_id.dest_port << ":" << connection->stream_id.source_port
					<< ", segm: " << next_segment << "/" << segments.size() << ", pld l: " << tcph->payload_length
					<< std::endl;
*/
				incomplete = true;
				*more = true;
				break;

			}else{
			    // skip segment, buffer too small
			    //std::cout << "!!ERR BUFFER OVERFLOW!!!" << std::endl;

			}

		    }
	    }
	    // if PSH, stop processing
	    if(tcph->psh) done = true;

	    // next segment index
	    ++next_segment;


	}

	// dec chunk queue if completed
	if(!incomplete){
	    --chunk_q;
	    // remove processed offset
	    offsets.erase(offsets.begin());

	// more buffered data in current chunk
	}else{
	    /*
	     * 100000
	     * 99900
	     * 100
	     *
	     */
	    offsets[0] = rel_seq_num;

	    // recalculate offset
	    for(unsigned int j = next_segment + 1; j<segments.size(); j++){
		// segment pointer
		tcph = segments[j];
		// stop if PSH
		if(tcph->psh) break;
		// relative sequence
		tmp_rel_seq_num = tcph->sequence_number - isn;
		// get offset
		if(tmp_rel_seq_num < offsets[0]) offsets[0] = tmp_rel_seq_num;

		//std::cout << "***data_pos: " << data_pos << ", offset: " << offsets[0] << ", seg.size: " << tcph->payload_length << std::endl;
	    }



	    // check for buffer overflow in case of packet loss and and out of sequence retransmission (unlikely to happen but can cause infinite loop)
	    if((rel_seq_num - offsets[0]) + segments[next_segment]->payload_length > max_buffer_length){
		//pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG, "Out-of-order TCP segment detected, out of order size factor = [%d]", rel_seq_num - offsets[0]);
		// copy out-of-order payload to output buffer starting from beginning of buffer
		memcpy(buffer, segments[next_segment]->payload, segments[next_segment]->payload_length);
		res = segments[next_segment]->payload_length;

		// skip segment
		++next_segment;


	    }



	}

	// return buffer size
	return res;
    }
    // return error
    return -1;
}

// IPFragIdHash
std::size_t tcpip::IPFragIdHash::operator() (const IPFragId& e) const{
    std::size_t seed = 0;
    boost::hash_combine(seed, e.dest_ip);
    boost::hash_combine(seed, e.dest_ip_size);
    boost::hash_combine(seed, e.source_ip);
    boost::hash_combine(seed, e.source_ip_size);
    boost::hash_combine(seed, e.identifiction);
    return seed;

}



// TCP_UDPStreamIdHash
std::size_t tcpip::TCP_UDPStreamIdHash::operator() (const TCP_UDPStreamId& e) const{
    std::size_t seed = 0;
    boost::hash_combine(seed, e.dest_ip);
    boost::hash_combine(seed, e.dest_ip_size);
    boost::hash_combine(seed, e.source_ip);
    boost::hash_combine(seed, e.source_ip_size);
    boost::hash_combine(seed, e.dest_port);
    boost::hash_combine(seed, e.source_port);
    return seed;
}

// DhcpReqRespIdHash
std::size_t tcpip::DhcpReqRespIdHash::operator ()(const DhcpReqRespId& e) const{
    std::size_t seed = 0;
    // regular src/dest ip/port combination
    boost::hash_combine(seed, e.stream_id.dest_ip);
    boost::hash_combine(seed, e.stream_id.dest_ip_size);
    boost::hash_combine(seed, e.stream_id.source_ip);
    boost::hash_combine(seed, e.stream_id.source_ip_size);
    boost::hash_combine(seed, e.stream_id.dest_port);
    boost::hash_combine(seed, e.stream_id.source_port);
    // dhcp specific
    boost::hash_combine(seed, e.transaction_id);
    return seed;

}

// DhcpSessionIdHash
std::size_t tcpip::DhcpSessionIdHash::operator ()(const DhcpSessionId& e) const{
    std::size_t seed = 0;
    boost::hash_combine(seed, e.username);
    boost::hash_combine(seed, e.username_size);
    return seed;

}



// RadiusReqRespIdHash
std::size_t tcpip::RadiusReqRespIdHash::operator ()(const RadiusReqRespId& e) const{
    std::size_t seed = 0;
    // regular src/dest ip/port combination
    boost::hash_combine(seed, e.stream_id.dest_ip);
    boost::hash_combine(seed, e.stream_id.dest_ip_size);
    boost::hash_combine(seed, e.stream_id.source_ip);
    boost::hash_combine(seed, e.stream_id.source_ip_size);
    boost::hash_combine(seed, e.stream_id.dest_port);
    boost::hash_combine(seed, e.stream_id.source_port);
    // radius specific
    boost::hash_combine(seed, e.packet_id);
    return seed;
}

// RadiusSessionIdHash
std::size_t tcpip::RadiusSessionIdHash::operator ()(const RadiusSessionId& e) const{
    std::size_t seed = 0;
    boost::hash_combine(seed, e.username);
    return seed;

}

/*
bool tcpip::TCPStreamIdCompare::operator ()(const TCPStreamId& x, const TCPStreamId& y){

    if(x.source_port < y.source_port) return true;
    else if(x.source_port > y.source_port) return false;
    else if(x.dest_port < y.dest_port) return true;
    else if(x.dest_port > y.dest_port) return false;
    else if(memcmp(x.source_ip, y.source_ip, x.source_ip_size) < 0) return true;
    else if(memcmp(x.source_ip, y.source_ip, x.source_ip_size) > 0) return false;
    else if(memcmp(x.dest_ip, y.dest_ip, x.dest_ip_size) < 0) return true;
    else return false;

//    return false;
    //return memcmp(&x, &y, sizeof(TCPStreamId)) < 0;
}
*/

// DhcpSession
tcpip::DhcpSession::DhcpSession(){
    timestamp = 0;
    exp_timestamp.set(9223372036854775807L);
    args = NULL;
    self = NULL;

}

// RadiusSession
tcpip::RadiusSession::RadiusSession(){
    timestamp = 0;
    args = NULL;
    self = NULL;
}


// DhcpSessionManager
tcpip::DhcpSessionManager::DhcpSessionManager(){
    pthread_mutex_init(&mtx, NULL);

}

tcpip::DhcpSessionManager::~DhcpSessionManager(){
    pthread_mutex_destroy(&mtx);
}

void tcpip::DhcpSessionManager::lock(){
    pthread_mutex_lock(&mtx);
}

void tcpip::DhcpSessionManager::unlock(){
    pthread_mutex_unlock(&mtx);
}

tcpip::DhcpSession* tcpip::DhcpSessionManager::process(IPHeader* iph, UDPHeader* udph, bool* udph_used, void* mac, int mac_size, void* source){
    DhcpSessionId tmp_id;
    DhcpSession* conn = NULL;
    *udph_used = false;

    // username
    memcpy(tmp_id.username, mac, mac_size);
    tmp_id.username_size = mac_size;
    // lock
    lock();
    // find
    conn_lst_it it = conn_lst.find(tmp_id);
    // existing session
    if(it != conn_lst.end()){
	conn = it->second;

	// update timestamp
	conn->timestamp = time(NULL);

	// unlock
	unlock();
	// return connection
	return conn;

    // new session
    }else{
	// new connection
	conn = alloc_conn(source);
	// sanity check
	if(conn != NULL){
	    // new
	    conn->finished.comp_swap(true, false);
	    // update timestamp
	    conn->timestamp = time(NULL);
	    conn->exp_timestamp.set(9223372036854775807L);
	    // set session id
	    conn->session_id = tmp_id;
	    // add connection to list
	    conn_lst[tmp_id] = conn;

	}else{
	    pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Cannot allocate DHCP Session chunk in DPI worker, memory pool empty!");

	}
	// unlock
	unlock();

	// return connection
	return conn;

    }

}


int tcpip::DhcpSessionManager::remove(DhcpSessionId* id){
    lock();
    int res = conn_lst.erase(*id);
    unlock();
    return res;
}

void tcpip::DhcpSessionManager::remove_all(){
    lock();
    conn_lst.clear();
    unlock();
}

tcpip::DhcpSession* tcpip::DhcpSessionManager::alloc_conn(void* args){
    return new DhcpSession();
}

void tcpip::DhcpSessionManager::free_conn(DhcpSession* conn){
    // nothing
}





// RadiusSessionManager
tcpip::RadiusSessionManager::RadiusSessionManager(){
    pthread_mutex_init(&mtx, NULL);
}

tcpip::RadiusSessionManager::~RadiusSessionManager(){
    pthread_mutex_destroy(&mtx);
}

void tcpip::RadiusSessionManager::lock(){
    pthread_mutex_lock(&mtx);
}

void tcpip::RadiusSessionManager::unlock(){
    pthread_mutex_unlock(&mtx);
}



tcpip::RadiusSession* tcpip::RadiusSessionManager::process(IPHeader* iph, UDPHeader* udph, bool* udph_used, void* username, int username_size, void* source){
    RadiusSessionId tmp_id;
    RadiusSession* conn = NULL;
    *udph_used = false;

    // username
    memcpy(tmp_id.username, username, username_size);
    tmp_id.username[username_size] = 0;
    // lock
    lock();
    // find
    conn_lst_it it = conn_lst.find(tmp_id);
    // existing session
    if(it != conn_lst.end()){
	conn = it->second;

	// update timestamp
	conn->timestamp = time(NULL);

	// unlock
	unlock();
	// return connection
	return conn;

    // new session
    }else{
	// new connection
	conn = alloc_conn(source);
	// sanity check
	if(conn != NULL){
	    // new
	    conn->finished.comp_swap(true, false);
	    conn->cleanup.comp_swap(true, false);
	    // update timestamp
	    conn->timestamp = time(NULL);
	    // set session id
	    conn->session_id = tmp_id;
	    // add connection to list
	    conn_lst[tmp_id] = conn;

	}else{
	    pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Cannot allocate RADIUS Session chunk in DPI worker, memory pool empty!");

	}
	// unlock
	unlock();

	// return connection
	return conn;

    }

}

int tcpip::RadiusSessionManager::remove(RadiusSessionId* id){
    lock();
    int res = conn_lst.erase(*id);
    unlock();
    return res;
}

void tcpip::RadiusSessionManager::remove_all(){
    lock();
    conn_lst.clear();
    unlock();
}

tcpip::RadiusSession* tcpip::RadiusSessionManager::alloc_conn(void* args){
    return new RadiusSession();
}

void tcpip::RadiusSessionManager::free_conn(RadiusSession* conn){
    // nothing
}



// DhcpReqRespCorrelationManager
tcpip::DhcpReqRespCorrelationManager::DhcpReqRespCorrelationManager(){

}

tcpip::DhcpReqRespCorrelationManager::~DhcpReqRespCorrelationManager(){

}

tcpip::DhcpReqRespCorrelation* tcpip::DhcpReqRespCorrelationManager::process(IPHeader* iph, UDPHeader* udph, bool* udph_used, uint32_t transaction_id, bool is_req, bool is_correlated){
    DhcpReqRespId tmp_id;
    DhcpReqRespCorrelation* conn = NULL;
    *udph_used = false;
    // dhcp xid
    tmp_id.transaction_id = transaction_id;

    // non broadcast destination
    if(	iph->destination[0] != 0xff &&
	iph->destination[1] != 0xff &&
	iph->destination[2] != 0xff &&
	iph->destination[3] != 0xff){

	// source ip
	tmp_id.stream_id.source_ip_size = iph->source_length;
	memcpy(tmp_id.stream_id.source_ip, iph->source, iph->source_length);
	// dest ip
	tmp_id.stream_id.dest_ip_size = iph->destination_length;
	memcpy(tmp_id.stream_id.dest_ip, iph->destination, iph->destination_length);


    }else{
	// source ip
	tmp_id.stream_id.source_ip_size = iph->source_length;
	// dest ip
	tmp_id.stream_id.dest_ip_size = iph->destination_length;

    }
    // source port
    tmp_id.stream_id.source_port = udph->source;
    // dest port
    tmp_id.stream_id.dest_port = udph->destination;

    conn_lst_it it = conn_lst.find(tmp_id);
    if(it == conn_lst.end()){
	// find reversed
	DhcpReqRespId tmp_id_rev;
	// radius packet id
	tmp_id_rev.transaction_id = transaction_id;
	// source ip = dest ip
	tmp_id_rev.stream_id.source_ip_size = tmp_id.stream_id.dest_ip_size;
	memcpy(tmp_id_rev.stream_id.source_ip, tmp_id.stream_id.dest_ip, tmp_id.stream_id.dest_ip_size);
	// dest ip = source ip
	tmp_id_rev.stream_id.dest_ip_size = tmp_id.stream_id.source_ip_size;
	memcpy(tmp_id_rev.stream_id.dest_ip, tmp_id.stream_id.source_ip, tmp_id.stream_id.source_ip_size);
	// source port = dest port
	tmp_id_rev.stream_id.source_port = tmp_id.stream_id.dest_port;
	// dest port = source port
	tmp_id_rev.stream_id.dest_port = tmp_id.stream_id.source_port;
	// find
	it = conn_lst.find(tmp_id_rev);

    // normal direction
    }

    // existing stream
    if(it != conn_lst.end() && !is_req){
	conn = it->second;

	// update timestamp
	conn->timestamp = time(NULL);

	// mark as finished (single REQ - RESP)
	conn->finished = true;

	// return connection
	return conn;

    // new stream
    }else if(it == conn_lst.end() && is_req){
	// new connection
	conn = alloc_conn(iph->args);
	// sanity check
	if(conn != NULL){
		// new
		conn->finished = false;
		conn->correlated = is_correlated;
		conn->user_session = NULL;
		// update timestamp
		conn->timestamp = time(NULL);
		// set session id
		conn->packet_session_id = tmp_id;
		// add connection to list
		conn_lst[tmp_id] = conn;
	}else{
	    pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Cannot allocate DHCP Req-Resp chunk in DPI worker, memory pool empty!");

	}

	// return connection
	return conn;

    // duplicate, skip
    }else{
	// nothing for now
    }

    // no connection
    return NULL;
}


int tcpip::DhcpReqRespCorrelationManager::remove(DhcpReqRespId* id){
    return conn_lst.erase(*id);
}

void tcpip::DhcpReqRespCorrelationManager::remove_all(){
    conn_lst.clear();
}

tcpip::DhcpReqRespCorrelation* tcpip::DhcpReqRespCorrelationManager::alloc_conn(void* args){
    return new DhcpReqRespCorrelation();
}

void tcpip::DhcpReqRespCorrelationManager::free_conn(DhcpReqRespCorrelation* conn){
    // nothing
}


void tcpip::DhcpReqRespCorrelationManager::process_timeout(unsigned int timeout){
    time_t now = time(NULL);
    // loop streams
    DhcpReqRespCorrelation* tmp = NULL;
    for(conn_lst_it it = conn_lst.begin(), it_next = it; it != conn_lst.end(); it = it_next){
	tmp = it->second;
	// next
	++it_next;
	// check if idle
	if(now - tmp->timestamp > timeout){
	    // remove
	    conn_lst.erase(it->first);
	    // free
	    free_conn(tmp);
	}

    }


}



// RadiusReqRespCorrelationManager
tcpip::RadiusReqRespCorrelationManager::RadiusReqRespCorrelationManager(){

}

tcpip::RadiusReqRespCorrelationManager::~RadiusReqRespCorrelationManager(){

}

tcpip::RadiusReqRespCorrelation* tcpip::RadiusReqRespCorrelationManager::process(	IPHeader* iph,
											UDPHeader* udph,
											bool* udph_used,
											uint8_t packet_id,
											bool is_req,
											bool is_acct,
											uint32_t _acct_st){
    RadiusReqRespId tmp_id;
    RadiusReqRespCorrelation* conn = NULL;
    *udph_used = false;

    // radius packet id
    tmp_id.packet_id = packet_id;
    // source ip
    tmp_id.stream_id.source_ip_size = iph->source_length;
    memcpy(tmp_id.stream_id.source_ip, iph->source, iph->source_length);
    // dest ip
    tmp_id.stream_id.dest_ip_size = iph->destination_length;
    memcpy(tmp_id.stream_id.dest_ip, iph->destination, iph->destination_length);
    // source port
    tmp_id.stream_id.source_port = udph->source;
    // dest port
    tmp_id.stream_id.dest_port = udph->destination;

    conn_lst_it it = conn_lst.find(tmp_id);
    if(it == conn_lst.end()){
	// find reversed
	RadiusReqRespId tmp_id_rev;
	// radius packet id
	tmp_id_rev.packet_id = packet_id;
	// source ip = dest ip
	tmp_id_rev.stream_id.source_ip_size = tmp_id.stream_id.dest_ip_size;
	memcpy(tmp_id_rev.stream_id.source_ip, tmp_id.stream_id.dest_ip, tmp_id.stream_id.dest_ip_size);
	// dest ip = source ip
	tmp_id_rev.stream_id.dest_ip_size = tmp_id.stream_id.source_ip_size;
	memcpy(tmp_id_rev.stream_id.dest_ip, tmp_id.stream_id.source_ip, tmp_id.stream_id.source_ip_size);
	// source port = dest port
	tmp_id_rev.stream_id.source_port = tmp_id.stream_id.dest_port;
	// dest port = source port
	tmp_id_rev.stream_id.dest_port = tmp_id.stream_id.source_port;
	// find
	it = conn_lst.find(tmp_id_rev);

    // normal direction
    }
    // existing stream
    if(it != conn_lst.end() && !is_req){
	conn = it->second;

	// update timestamp
	conn->timestamp = time(NULL);

	// mark as finished (single REQ - RESP)
	conn->finished = true;

	// return connection
	return conn;

    // new stream (only if Request packet)
    }else if(it == conn_lst.end() && is_req){
	// new connection
	conn = alloc_conn(iph->args);
	// sanity check
	if(conn != NULL){
		// new
		conn->finished = false;
		conn->user_session = NULL;
		// update timestamp
		conn->timestamp = time(NULL);
		// set session id
		conn->packet_session_id = tmp_id;
		// set accounting data
		conn->acct = is_acct;
		conn->acct_st = _acct_st;
		// add connection to list
		conn_lst[tmp_id] = conn;
	}else{
	    pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Cannot allocate RADIUS Req-Resp chunk in DPI worker, memory pool empty!");

	}

	// return connection
	return conn;

    // duplicate, skip
    }else{
	// nothing for now
    }

    // no connection
    return NULL;
}

int tcpip::RadiusReqRespCorrelationManager::remove(RadiusReqRespId* id){
    return conn_lst.erase(*id);
}

void tcpip::RadiusReqRespCorrelationManager::remove_all(){
    conn_lst.clear();
}

tcpip::RadiusReqRespCorrelation* tcpip::RadiusReqRespCorrelationManager::alloc_conn(void* args){
    return new RadiusReqRespCorrelation();
}

void tcpip::RadiusReqRespCorrelationManager::free_conn(RadiusReqRespCorrelation* conn){
    // nothing
}

void tcpip::RadiusReqRespCorrelationManager::process_timeout(unsigned int timeout){
    time_t now = time(NULL);
    // loop streams
    RadiusReqRespCorrelation* tmp = NULL;
    for(conn_lst_it it = conn_lst.begin(), it_next = it; it != conn_lst.end(); it = it_next){
	tmp = it->second;
	// next
	++it_next;
	// check if idle
	if(now - tmp->timestamp > timeout){
	    // remove
	    conn_lst.erase(it->first);
	    // free
	    free_conn(tmp);

	}

    }


}




// TCPReassembly
tcpip::TCPReassembly::TCPReassembly(){

}

tcpip::TCPReassembly::~TCPReassembly(){

}


tcpip::TCPConnection* tcpip::TCPReassembly::alloc_conn(void* args){
    return new TCPConnection();
}
void tcpip::TCPReassembly::free_conn(TCPConnection* conn){
    // nothing
}

void tcpip::TCPReassembly::remove_all(){
    //lock();
    conn_lst.clear();
    //unlock();
}

//void tcpip::TCPReassembly::lock(){
    // do nothing, implemented in derived classes
    // if necessary

//}

//void tcpip::TCPReassembly::unlock(){
    // do nothing, implemented in derived classes
    // if necessary

//}


int tcpip::TCPReassembly::remove(uint32_t _hash){
    //if(!no_lock) lock();
    int res = conn_lst.erase(_hash);
    //std::cout << "REMOVE: " << res << std::endl;
    //if(!no_lock) unlock();
    return res;
}

tcpip::TCPReassembly::conn_lst_type* tcpip::TCPReassembly::get_conn_lst(){
    return &conn_lst;
}



tcpip::TCPConnection* tcpip::TCPReassembly::process(IPHeader* iph, TCPHeader* tcph, bool* tcph_used){
    TCP_UDPStreamId tmp_id;
    bool rev_found = false;
    TCPConnection* conn = NULL;
    *tcph_used = false;
    uint32_t hash = 0;

    // source ip
    tmp_id.source_ip_size = iph->source_length;
    memcpy(tmp_id.source_ip, iph->source, iph->source_length);
    // dest ip
    tmp_id.dest_ip_size = iph->destination_length;
    memcpy(tmp_id.dest_ip, iph->destination, iph->destination_length);
    // source port
    tmp_id.source_port = tcph->source;
    // dest port
    tmp_id.dest_port = tcph->destination;

    // hash
    hash = pmink_utils::hash_fnv1a(&tmp_id, sizeof(TCP_UDPStreamId));

    // find
    conn_lst_it it = conn_lst.find(hash);
    if(it == conn_lst.end()){
	// find reversed
	TCP_UDPStreamId tmp_id_rev;
	// source ip = dest ip
	tmp_id_rev.source_ip_size = tmp_id.dest_ip_size;
	memcpy(tmp_id_rev.source_ip, tmp_id.dest_ip, tmp_id.dest_ip_size);
	// dest ip = source ip
	tmp_id_rev.dest_ip_size = tmp_id.source_ip_size;
	memcpy(tmp_id_rev.dest_ip, tmp_id.source_ip, tmp_id.source_ip_size);
	// source port = dest port
	tmp_id_rev.source_port = tmp_id.dest_port;
	// dest port = source port
	tmp_id_rev.dest_port = tmp_id.source_port;

	// find
	hash = pmink_utils::hash_fnv1a(&tmp_id_rev, sizeof(TCP_UDPStreamId));
	it = conn_lst.find(hash);

    // normal direction
    }else rev_found = true;


    // existing stream
    if(it != conn_lst.end()){
	conn = it->second;

	// set current stream direction
	conn->current_stream = (rev_found ? &conn->in_stream : &conn->out_stream);

	// RST flag, stream finished
	if(tcph->rst) conn->finished = true;

	// FIN flag
	if(tcph->fin){
	    conn->current_stream->finished = true;
	    // if FIN sent from both sides
	    if(conn->in_stream.finished && conn->out_stream.finished) conn->finished = true;
	}

	// update timestamp
	conn->timestamp = time(NULL);

	// if finished return
	if(conn->finished) return conn;

	// SYN handshake
	if(tcph->syn){
	    // SYN, ACK = stage 2 of 3 stage handshake
	    // ACK number should be client's ISN + 1
	    // set server's sequence number
	    if(tcph->ack){
		if(tcph->ack_number == conn->out_stream.isn + 1){
		    // check if isn was already set
		    if(!conn->current_stream->isn_set){
			// set isn
			conn->current_stream->isn = tcph->sequence_number;
			conn->current_stream->isn_set = true;

		    }else if(conn->current_stream->isn != tcph->sequence_number){
			pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG4, "TCP SYN-ACK retransmission with modified sequence number, new isn = [%u], old isn = [%u], skipping segment!",
									tcph->sequence_number, conn->current_stream->isn);

		    }
		}else{
		    pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG4, "Invalid TCP SYN-ACK ack number = [%u], expected ack number = [%u], skipping segment!",
								    tcph->ack_number, conn->out_stream.isn + 1);

		}
	    // no ACK = stage 1 ?
	    }else{
		// no ACK and no ACK seq
		if(tcph->ack_number == 0){
		    // SYN retransmission error check
		    if(conn->current_stream->isn_set && conn->current_stream->isn != tcph->sequence_number){
			pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG4, "TCP SYN retransmission with modified sequence number, new isn = [%u], old isn = [%u], skipping segment!",
									tcph->sequence_number, conn->current_stream->isn);

		    // SYN retransmission seems to be ok
		    }else{
			// server -> client (stage 2 handshake suspicious, should contain ACK)
			if(conn->current_stream == &conn->in_stream){
			    // update ISN
			    conn->current_stream->isn = tcph->sequence_number;
			    conn->current_stream->isn_set = true;

			// client -> server (SYN retransmission)
			}else{
			    // update ISN
			    conn->current_stream->isn = tcph->sequence_number;
			    conn->current_stream->isn_set = true;

			}

		    }

		}
	    }



	}else{
	    // only add segments with payload and check if ISN was set
	    if(tcph->payload_length > 0 && conn->current_stream->isn_set){
		// check if sequence number is ok
		if(tcph->sequence_number > conn->current_stream->isn){
		    // add new segment
		    conn->current_stream->add_segment(tcph);
		    conn->current_stream->byte_count += tcph->payload_length;
		    conn->current_stream->seg_byte_count += tcph->payload_length;
		    *tcph_used = true;
		    //tcph->used = true;

		    // set tmp pointers
		    std::vector<uint32_t>* offsets = &conn->current_stream->offsets;
		    TCPStream* cstream = conn->current_stream;
		    uint32_t rel_seq = tcph->sequence_number - cstream->isn;

		    // new chunk
		    if(offsets->size() <= cstream->chunk_q) offsets->push_back(rel_seq);
		    // chunk present, recalculate offset
		    else{
			uint32_t tmp_offset = (*offsets)[offsets->size() - 1];
			if(rel_seq < tmp_offset) (*offsets)[offsets->size() - 1] = rel_seq;
		    }

		    // if PSH, chunk is ready for reassembly
		    if(tcph->psh) ++conn->current_stream->chunk_q;

		}else{
			pmink::CURRENT_DAEMON->log(pmink::LLT_DEBUG4, "Invalid TCP sequence number = [%u], isn = [%u], skipping segment!",
									tcph->sequence_number, conn->current_stream->isn);

		}

	    }
	}


	// return connection
	return conn;

    // new stream (SYN flag, no ACK flag, ACK NUM is 0), stage 1 of 3 stage handshake
    }else if(tcph->syn && !tcph->ack && tcph->ack_number == 0){
	// new connection
	conn = alloc_conn(iph->args);
	// sanity check
	if(conn != NULL){
		// reset l7 type
		conn->l7_ptype = tcpip::L7_PT_UNKNOWN;
		// reset classification attempt counter
		conn->clsf_count = 0;
		// update timestamp
		conn->timestamp = time(NULL);
		// reset finished flag
		conn->finished = false;
		conn->out_stream.finished = false;
		conn->in_stream.finished = false;
		// set initial sequence number
		conn->out_stream.isn = tcph->sequence_number;
		conn->out_stream.isn_set = true;
		// reset isn set
		conn->in_stream.isn_set = false;
		// reset byte count
		conn->in_stream.byte_count = 0;
		conn->in_stream.seg_byte_count = 0;
		conn->out_stream.byte_count = 0;
		conn->out_stream.seg_byte_count = 0;
		// set connection pointers
		conn->in_stream.set_connection(conn);
		conn->out_stream.set_connection(conn);
		// set reversed direction
		conn->in_stream.set_reversed(true);
		// set stream id
		conn->stream_id = tmp_id;
		conn->stream_hash = hash;
		// set current active direction
		conn->current_stream = &conn->out_stream;
		// add connection to list
		conn_lst[hash] = conn;
	}else{
	    pmink::CURRENT_DAEMON->log(pmink::LLT_WARNING, "Cannot allocate TCP Connection chunk in DPI worker, memory pool empty!");

	}

	// return connection
	return conn;
    }else{
	// nothing

    }

    // no connection
    return NULL;
}



