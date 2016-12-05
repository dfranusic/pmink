/**
 * \file mtp2.h
 * \brief MTP2 library
 * \details MTP2 encoder/decoder library based on ITU-T Q.703
 * \version 2.0
 * \date 2012
 * \author Damir Franusic <df@release14.org>
 */

#ifndef MTP2_H_
#define MTP2_H_

namespace mtp2 {

	enum SignalUnitType {
		__MTP2_UNKNOWN_UNIT_TYPE__ = -1,
		MSU = 100,
		LSSU = 1,
		FISU = 0
	};

	class MTP2Packet {
	public:
		MTP2Packet();
		~MTP2Packet();
		SignalUnitType type;
		int backward_seq_num;
		bool backward_seq_num_indicator;
		int fwd_seq_num;
		bool fwd_seq_num_indicator;
		int length_indicator;
		char* sif;
		int sif_length;
		short unsigned int status_field;
		void encode(char* buffer, int buffer_length, int offset, int* result_length);

	};


	/**
	 * Decodes mtp2 message
	 * @param data pointer to mtp2 bytes
	 * @param data_length number of bytes contained in mtp2 packet
	 * @return Pointer to mtp2 message object or NULL no data available
	 */
	MTP2Packet* decode(char* data, int data_length);

}


#endif /* MTP2_H_ */
