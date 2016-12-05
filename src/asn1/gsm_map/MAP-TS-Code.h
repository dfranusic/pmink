#ifndef MAP_TS_CODE_H_
#define MAP_TS_CODE_H_

#include<asn1.h>

namespace asn1 {
	// forward declarations
	class TeleserviceCode;
	class Ext_TeleserviceCode;
	class allTeleservices;
	class allSpeechTransmissionServices;
	class telephony;
	class emergencyCalls;
	class allShortMessageServices;
	class shortMessageMT_PP;
	class shortMessageMO_PP;
	class allFacsimileTransmissionServices;
	class facsimileGroup3AndAlterSpeech;
	class automaticFacsimileGroup3;
	class facsimileGroup4;
	class allDataTeleservices;
	class allTeleservices_ExeptSMS;
	class allVoiceGroupCallServices;
	class voiceGroupCall;
	class voiceBroadcastCall;
	class allPLMN_specificTS;
	class plmn_specificTS_1;
	class plmn_specificTS_2;
	class plmn_specificTS_3;
	class plmn_specificTS_4;
	class plmn_specificTS_5;
	class plmn_specificTS_6;
	class plmn_specificTS_7;
	class plmn_specificTS_8;
	class plmn_specificTS_9;
	class plmn_specificTS_A;
	class plmn_specificTS_B;
	class plmn_specificTS_C;
	class plmn_specificTS_D;
	class plmn_specificTS_E;
	class plmn_specificTS_F;

	

	// TeleserviceCode
	class TeleserviceCode : public Octet_string {
	public:
		TeleserviceCode();
		~TeleserviceCode();

	};

	// Ext_TeleserviceCode
	class Ext_TeleserviceCode : public Octet_string {
	public:
		Ext_TeleserviceCode();
		~Ext_TeleserviceCode();

	};

	// allTeleservices
	class allTeleservices : public TeleserviceCode {
	public:
		allTeleservices();
		~allTeleservices();

	};

	// allSpeechTransmissionServices
	class allSpeechTransmissionServices : public TeleserviceCode {
	public:
		allSpeechTransmissionServices();
		~allSpeechTransmissionServices();

	};

	// telephony
	class telephony : public TeleserviceCode {
	public:
		telephony();
		~telephony();

	};

	// emergencyCalls
	class emergencyCalls : public TeleserviceCode {
	public:
		emergencyCalls();
		~emergencyCalls();

	};

	// allShortMessageServices
	class allShortMessageServices : public TeleserviceCode {
	public:
		allShortMessageServices();
		~allShortMessageServices();

	};

	// shortMessageMT_PP
	class shortMessageMT_PP : public TeleserviceCode {
	public:
		shortMessageMT_PP();
		~shortMessageMT_PP();

	};

	// shortMessageMO_PP
	class shortMessageMO_PP : public TeleserviceCode {
	public:
		shortMessageMO_PP();
		~shortMessageMO_PP();

	};

	// allFacsimileTransmissionServices
	class allFacsimileTransmissionServices : public TeleserviceCode {
	public:
		allFacsimileTransmissionServices();
		~allFacsimileTransmissionServices();

	};

	// facsimileGroup3AndAlterSpeech
	class facsimileGroup3AndAlterSpeech : public TeleserviceCode {
	public:
		facsimileGroup3AndAlterSpeech();
		~facsimileGroup3AndAlterSpeech();

	};

	// automaticFacsimileGroup3
	class automaticFacsimileGroup3 : public TeleserviceCode {
	public:
		automaticFacsimileGroup3();
		~automaticFacsimileGroup3();

	};

	// facsimileGroup4
	class facsimileGroup4 : public TeleserviceCode {
	public:
		facsimileGroup4();
		~facsimileGroup4();

	};

	// allDataTeleservices
	class allDataTeleservices : public TeleserviceCode {
	public:
		allDataTeleservices();
		~allDataTeleservices();

	};

	// allTeleservices_ExeptSMS
	class allTeleservices_ExeptSMS : public TeleserviceCode {
	public:
		allTeleservices_ExeptSMS();
		~allTeleservices_ExeptSMS();

	};

	// allVoiceGroupCallServices
	class allVoiceGroupCallServices : public TeleserviceCode {
	public:
		allVoiceGroupCallServices();
		~allVoiceGroupCallServices();

	};

	// voiceGroupCall
	class voiceGroupCall : public TeleserviceCode {
	public:
		voiceGroupCall();
		~voiceGroupCall();

	};

	// voiceBroadcastCall
	class voiceBroadcastCall : public TeleserviceCode {
	public:
		voiceBroadcastCall();
		~voiceBroadcastCall();

	};

	// allPLMN_specificTS
	class allPLMN_specificTS : public TeleserviceCode {
	public:
		allPLMN_specificTS();
		~allPLMN_specificTS();

	};

	// plmn_specificTS_1
	class plmn_specificTS_1 : public TeleserviceCode {
	public:
		plmn_specificTS_1();
		~plmn_specificTS_1();

	};

	// plmn_specificTS_2
	class plmn_specificTS_2 : public TeleserviceCode {
	public:
		plmn_specificTS_2();
		~plmn_specificTS_2();

	};

	// plmn_specificTS_3
	class plmn_specificTS_3 : public TeleserviceCode {
	public:
		plmn_specificTS_3();
		~plmn_specificTS_3();

	};

	// plmn_specificTS_4
	class plmn_specificTS_4 : public TeleserviceCode {
	public:
		plmn_specificTS_4();
		~plmn_specificTS_4();

	};

	// plmn_specificTS_5
	class plmn_specificTS_5 : public TeleserviceCode {
	public:
		plmn_specificTS_5();
		~plmn_specificTS_5();

	};

	// plmn_specificTS_6
	class plmn_specificTS_6 : public TeleserviceCode {
	public:
		plmn_specificTS_6();
		~plmn_specificTS_6();

	};

	// plmn_specificTS_7
	class plmn_specificTS_7 : public TeleserviceCode {
	public:
		plmn_specificTS_7();
		~plmn_specificTS_7();

	};

	// plmn_specificTS_8
	class plmn_specificTS_8 : public TeleserviceCode {
	public:
		plmn_specificTS_8();
		~plmn_specificTS_8();

	};

	// plmn_specificTS_9
	class plmn_specificTS_9 : public TeleserviceCode {
	public:
		plmn_specificTS_9();
		~plmn_specificTS_9();

	};

	// plmn_specificTS_A
	class plmn_specificTS_A : public TeleserviceCode {
	public:
		plmn_specificTS_A();
		~plmn_specificTS_A();

	};

	// plmn_specificTS_B
	class plmn_specificTS_B : public TeleserviceCode {
	public:
		plmn_specificTS_B();
		~plmn_specificTS_B();

	};

	// plmn_specificTS_C
	class plmn_specificTS_C : public TeleserviceCode {
	public:
		plmn_specificTS_C();
		~plmn_specificTS_C();

	};

	// plmn_specificTS_D
	class plmn_specificTS_D : public TeleserviceCode {
	public:
		plmn_specificTS_D();
		~plmn_specificTS_D();

	};

	// plmn_specificTS_E
	class plmn_specificTS_E : public TeleserviceCode {
	public:
		plmn_specificTS_E();
		~plmn_specificTS_E();

	};

	// plmn_specificTS_F
	class plmn_specificTS_F : public TeleserviceCode {
	public:
		plmn_specificTS_F();
		~plmn_specificTS_F();

	};

}
#endif