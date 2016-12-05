#include<iostream>
#include<string.h>
#include<mtp2.h>


// base mtp2 class
mtp2::MTP2Packet::MTP2Packet(){
	type = __MTP2_UNKNOWN_UNIT_TYPE__;
	sif = NULL;
	sif_length = 0;
}
mtp2::MTP2Packet::~MTP2Packet(){}

void mtp2::MTP2Packet::encode(char* buffer, int buffer_length, int offset, int* result_length){
	// offset
	buffer += offset;

	buffer[0] = backward_seq_num + (backward_seq_num_indicator ? 0x80 : 0x00);
	buffer[1] = fwd_seq_num + (fwd_seq_num_indicator ? 0x80 : 0x00);

	switch(type){
		case FISU:
			buffer[2] = 0x00;
			*result_length = 3;
			break;
		case LSSU:
			buffer[2] = 1;
			*result_length = 3;
			break;
		case MSU:
			buffer[2] = (sif_length >= 62 ? 63 : sif_length);
			if(sif != NULL && sif_length > 0) memcpy(&buffer[3], sif, sif_length);
			*result_length = sif_length + 3;
			break;
	}



}

// main mtp2 decode method

mtp2::MTP2Packet* mtp2::decode(char* data, int data_length){
	if(data != NULL && data_length > 0){
		MTP2Packet* mtp2p = new MTP2Packet();

		mtp2p->backward_seq_num = *data & 0x7f;
		mtp2p->backward_seq_num_indicator = (*data & 0x80) == 0x80;
		data++;

		mtp2p->fwd_seq_num = *data & 0x7f;
		mtp2p->fwd_seq_num_indicator = (*data & 0x80) == 0x80;
		data++;

		mtp2p->length_indicator = *data & 0x3f;
		data++;

		// FISU
		if(mtp2p->length_indicator == 0){
			mtp2p->type = FISU;

		// LSSU
		}else if(mtp2p->length_indicator == 1 || mtp2p->length_indicator == 2){
			mtp2p->type = LSSU;
			mtp2p->status_field = *data;

		// MSU
		}else if(mtp2p->length_indicator > 2){
			mtp2p->type = MSU;
			mtp2p->sif = data;
			mtp2p->sif_length = data_length - 3;
		}

		// result
		return mtp2p;

	}
	return NULL;
}
