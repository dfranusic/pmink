#ifndef MAP_BS_CODE_H_
#define MAP_BS_CODE_H_

#include<asn1.h>

namespace asn1 {
	// forward declarations
	class BearerServiceCode;
	class Ext_BearerServiceCode;
	class allBearerServices;
	class allDataCDA_Services;
	class dataCDA_300bps;
	class dataCDA_1200bps;
	class dataCDA_1200_75bps;
	class dataCDA_2400bps;
	class dataCDA_4800bps;
	class dataCDA_9600bps;
	class general_dataCDA;
	class allDataCDS_Services;
	class dataCDS_1200bps;
	class dataCDS_2400bps;
	class dataCDS_4800bps;
	class dataCDS_9600bps;
	class general_dataCDS;
	class allPadAccessCA_Services;
	class padAccessCA_300bps;
	class padAccessCA_1200bps;
	class padAccessCA_1200_75bps;
	class padAccessCA_2400bps;
	class padAccessCA_4800bps;
	class padAccessCA_9600bps;
	class general_padAccessCA;
	class allDataPDS_Services;
	class dataPDS_2400bps;
	class dataPDS_4800bps;
	class dataPDS_9600bps;
	class general_dataPDS;
	class allAlternateSpeech_DataCDA;
	class allAlternateSpeech_DataCDS;
	class allSpeechFollowedByDataCDA;
	class allSpeechFollowedByDataCDS;
	class allDataCircuitAsynchronous;
	class allAsynchronousServices;
	class allDataCircuitSynchronous;
	class allSynchronousServices;
	class allPLMN_specificBS;
	class plmn_specificBS_1;
	class plmn_specificBS_2;
	class plmn_specificBS_3;
	class plmn_specificBS_4;
	class plmn_specificBS_5;
	class plmn_specificBS_6;
	class plmn_specificBS_7;
	class plmn_specificBS_8;
	class plmn_specificBS_9;
	class plmn_specificBS_A;
	class plmn_specificBS_B;
	class plmn_specificBS_C;
	class plmn_specificBS_D;
	class plmn_specificBS_E;
	class plmn_specificBS_F;

	

	// BearerServiceCode
	class BearerServiceCode : public Octet_string {
	public:
		BearerServiceCode();
		~BearerServiceCode();

	};

	// Ext_BearerServiceCode
	class Ext_BearerServiceCode : public Octet_string {
	public:
		Ext_BearerServiceCode();
		~Ext_BearerServiceCode();

	};

	// allBearerServices
	class allBearerServices : public BearerServiceCode {
	public:
		allBearerServices();
		~allBearerServices();

	};

	// allDataCDA_Services
	class allDataCDA_Services : public BearerServiceCode {
	public:
		allDataCDA_Services();
		~allDataCDA_Services();

	};

	// dataCDA_300bps
	class dataCDA_300bps : public BearerServiceCode {
	public:
		dataCDA_300bps();
		~dataCDA_300bps();

	};

	// dataCDA_1200bps
	class dataCDA_1200bps : public BearerServiceCode {
	public:
		dataCDA_1200bps();
		~dataCDA_1200bps();

	};

	// dataCDA_1200_75bps
	class dataCDA_1200_75bps : public BearerServiceCode {
	public:
		dataCDA_1200_75bps();
		~dataCDA_1200_75bps();

	};

	// dataCDA_2400bps
	class dataCDA_2400bps : public BearerServiceCode {
	public:
		dataCDA_2400bps();
		~dataCDA_2400bps();

	};

	// dataCDA_4800bps
	class dataCDA_4800bps : public BearerServiceCode {
	public:
		dataCDA_4800bps();
		~dataCDA_4800bps();

	};

	// dataCDA_9600bps
	class dataCDA_9600bps : public BearerServiceCode {
	public:
		dataCDA_9600bps();
		~dataCDA_9600bps();

	};

	// general_dataCDA
	class general_dataCDA : public BearerServiceCode {
	public:
		general_dataCDA();
		~general_dataCDA();

	};

	// allDataCDS_Services
	class allDataCDS_Services : public BearerServiceCode {
	public:
		allDataCDS_Services();
		~allDataCDS_Services();

	};

	// dataCDS_1200bps
	class dataCDS_1200bps : public BearerServiceCode {
	public:
		dataCDS_1200bps();
		~dataCDS_1200bps();

	};

	// dataCDS_2400bps
	class dataCDS_2400bps : public BearerServiceCode {
	public:
		dataCDS_2400bps();
		~dataCDS_2400bps();

	};

	// dataCDS_4800bps
	class dataCDS_4800bps : public BearerServiceCode {
	public:
		dataCDS_4800bps();
		~dataCDS_4800bps();

	};

	// dataCDS_9600bps
	class dataCDS_9600bps : public BearerServiceCode {
	public:
		dataCDS_9600bps();
		~dataCDS_9600bps();

	};

	// general_dataCDS
	class general_dataCDS : public BearerServiceCode {
	public:
		general_dataCDS();
		~general_dataCDS();

	};

	// allPadAccessCA_Services
	class allPadAccessCA_Services : public BearerServiceCode {
	public:
		allPadAccessCA_Services();
		~allPadAccessCA_Services();

	};

	// padAccessCA_300bps
	class padAccessCA_300bps : public BearerServiceCode {
	public:
		padAccessCA_300bps();
		~padAccessCA_300bps();

	};

	// padAccessCA_1200bps
	class padAccessCA_1200bps : public BearerServiceCode {
	public:
		padAccessCA_1200bps();
		~padAccessCA_1200bps();

	};

	// padAccessCA_1200_75bps
	class padAccessCA_1200_75bps : public BearerServiceCode {
	public:
		padAccessCA_1200_75bps();
		~padAccessCA_1200_75bps();

	};

	// padAccessCA_2400bps
	class padAccessCA_2400bps : public BearerServiceCode {
	public:
		padAccessCA_2400bps();
		~padAccessCA_2400bps();

	};

	// padAccessCA_4800bps
	class padAccessCA_4800bps : public BearerServiceCode {
	public:
		padAccessCA_4800bps();
		~padAccessCA_4800bps();

	};

	// padAccessCA_9600bps
	class padAccessCA_9600bps : public BearerServiceCode {
	public:
		padAccessCA_9600bps();
		~padAccessCA_9600bps();

	};

	// general_padAccessCA
	class general_padAccessCA : public BearerServiceCode {
	public:
		general_padAccessCA();
		~general_padAccessCA();

	};

	// allDataPDS_Services
	class allDataPDS_Services : public BearerServiceCode {
	public:
		allDataPDS_Services();
		~allDataPDS_Services();

	};

	// dataPDS_2400bps
	class dataPDS_2400bps : public BearerServiceCode {
	public:
		dataPDS_2400bps();
		~dataPDS_2400bps();

	};

	// dataPDS_4800bps
	class dataPDS_4800bps : public BearerServiceCode {
	public:
		dataPDS_4800bps();
		~dataPDS_4800bps();

	};

	// dataPDS_9600bps
	class dataPDS_9600bps : public BearerServiceCode {
	public:
		dataPDS_9600bps();
		~dataPDS_9600bps();

	};

	// general_dataPDS
	class general_dataPDS : public BearerServiceCode {
	public:
		general_dataPDS();
		~general_dataPDS();

	};

	// allAlternateSpeech_DataCDA
	class allAlternateSpeech_DataCDA : public BearerServiceCode {
	public:
		allAlternateSpeech_DataCDA();
		~allAlternateSpeech_DataCDA();

	};

	// allAlternateSpeech_DataCDS
	class allAlternateSpeech_DataCDS : public BearerServiceCode {
	public:
		allAlternateSpeech_DataCDS();
		~allAlternateSpeech_DataCDS();

	};

	// allSpeechFollowedByDataCDA
	class allSpeechFollowedByDataCDA : public BearerServiceCode {
	public:
		allSpeechFollowedByDataCDA();
		~allSpeechFollowedByDataCDA();

	};

	// allSpeechFollowedByDataCDS
	class allSpeechFollowedByDataCDS : public BearerServiceCode {
	public:
		allSpeechFollowedByDataCDS();
		~allSpeechFollowedByDataCDS();

	};

	// allDataCircuitAsynchronous
	class allDataCircuitAsynchronous : public BearerServiceCode {
	public:
		allDataCircuitAsynchronous();
		~allDataCircuitAsynchronous();

	};

	// allAsynchronousServices
	class allAsynchronousServices : public BearerServiceCode {
	public:
		allAsynchronousServices();
		~allAsynchronousServices();

	};

	// allDataCircuitSynchronous
	class allDataCircuitSynchronous : public BearerServiceCode {
	public:
		allDataCircuitSynchronous();
		~allDataCircuitSynchronous();

	};

	// allSynchronousServices
	class allSynchronousServices : public BearerServiceCode {
	public:
		allSynchronousServices();
		~allSynchronousServices();

	};

	// allPLMN_specificBS
	class allPLMN_specificBS : public BearerServiceCode {
	public:
		allPLMN_specificBS();
		~allPLMN_specificBS();

	};

	// plmn_specificBS_1
	class plmn_specificBS_1 : public BearerServiceCode {
	public:
		plmn_specificBS_1();
		~plmn_specificBS_1();

	};

	// plmn_specificBS_2
	class plmn_specificBS_2 : public BearerServiceCode {
	public:
		plmn_specificBS_2();
		~plmn_specificBS_2();

	};

	// plmn_specificBS_3
	class plmn_specificBS_3 : public BearerServiceCode {
	public:
		plmn_specificBS_3();
		~plmn_specificBS_3();

	};

	// plmn_specificBS_4
	class plmn_specificBS_4 : public BearerServiceCode {
	public:
		plmn_specificBS_4();
		~plmn_specificBS_4();

	};

	// plmn_specificBS_5
	class plmn_specificBS_5 : public BearerServiceCode {
	public:
		plmn_specificBS_5();
		~plmn_specificBS_5();

	};

	// plmn_specificBS_6
	class plmn_specificBS_6 : public BearerServiceCode {
	public:
		plmn_specificBS_6();
		~plmn_specificBS_6();

	};

	// plmn_specificBS_7
	class plmn_specificBS_7 : public BearerServiceCode {
	public:
		plmn_specificBS_7();
		~plmn_specificBS_7();

	};

	// plmn_specificBS_8
	class plmn_specificBS_8 : public BearerServiceCode {
	public:
		plmn_specificBS_8();
		~plmn_specificBS_8();

	};

	// plmn_specificBS_9
	class plmn_specificBS_9 : public BearerServiceCode {
	public:
		plmn_specificBS_9();
		~plmn_specificBS_9();

	};

	// plmn_specificBS_A
	class plmn_specificBS_A : public BearerServiceCode {
	public:
		plmn_specificBS_A();
		~plmn_specificBS_A();

	};

	// plmn_specificBS_B
	class plmn_specificBS_B : public BearerServiceCode {
	public:
		plmn_specificBS_B();
		~plmn_specificBS_B();

	};

	// plmn_specificBS_C
	class plmn_specificBS_C : public BearerServiceCode {
	public:
		plmn_specificBS_C();
		~plmn_specificBS_C();

	};

	// plmn_specificBS_D
	class plmn_specificBS_D : public BearerServiceCode {
	public:
		plmn_specificBS_D();
		~plmn_specificBS_D();

	};

	// plmn_specificBS_E
	class plmn_specificBS_E : public BearerServiceCode {
	public:
		plmn_specificBS_E();
		~plmn_specificBS_E();

	};

	// plmn_specificBS_F
	class plmn_specificBS_F : public BearerServiceCode {
	public:
		plmn_specificBS_F();
		~plmn_specificBS_F();

	};

}
#endif