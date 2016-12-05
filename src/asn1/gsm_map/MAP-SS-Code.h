#ifndef MAP_SS_CODE_H_
#define MAP_SS_CODE_H_

#include<asn1.h>

namespace asn1 {
	// forward declarations
	class SS_Code;
	class allSS;
	class allLineIdentificationSS;
	class clip;
	class clir;
	class colp;
	class colr;
	class mci;
	class allNameIdentificationSS;
	class cnap;
	class allForwardingSS;
	class cfu;
	class allCondForwardingSS;
	class cfb;
	class cfnry;
	class cfnrc;
	class cd;
	class allCallOfferingSS;
	class ect;
	class mah;
	class allCallCompletionSS;
	class cw;
	class hold;
	class ccbs_A;
	class ccbs_B;
	class mc;
	class allMultiPartySS;
	class multiPTY;
	class allCommunityOfInterest_SS;
	class cug;
	class allChargingSS;
	class aoci;
	class aocc;
	class allAdditionalInfoTransferSS;
	class uus1;
	class uus2;
	class uus3;
	class allBarringSS;
	class barringOfOutgoingCalls;
	class baoc;
	class boic;
	class boicExHC;
	class barringOfIncomingCalls;
	class baic;
	class bicRoam;
	class allPLMN_specificSS;
	class plmn_specificSS_1;
	class plmn_specificSS_2;
	class plmn_specificSS_3;
	class plmn_specificSS_4;
	class plmn_specificSS_5;
	class plmn_specificSS_6;
	class plmn_specificSS_7;
	class plmn_specificSS_8;
	class plmn_specificSS_9;
	class plmn_specificSS_A;
	class plmn_specificSS_B;
	class plmn_specificSS_C;
	class plmn_specificSS_D;
	class plmn_specificSS_E;
	class plmn_specificSS_F;
	class allCallPrioritySS;
	class emlpp;
	class allLCSPrivacyException;
	class universal;
	class callSessionRelated;
	class callSessionUnrelated;
	class plmnoperator;
	class serviceType;
	class allMOLR_SS;
	class basicSelfLocation;
	class autonomousSelfLocation;
	class transferToThirdParty;

	

	// SS_Code
	class SS_Code : public Octet_string {
	public:
		SS_Code();
		~SS_Code();

	};

	// allSS
	class allSS : public SS_Code {
	public:
		allSS();
		~allSS();

	};

	// allLineIdentificationSS
	class allLineIdentificationSS : public SS_Code {
	public:
		allLineIdentificationSS();
		~allLineIdentificationSS();

	};

	// clip
	class clip : public SS_Code {
	public:
		clip();
		~clip();

	};

	// clir
	class clir : public SS_Code {
	public:
		clir();
		~clir();

	};

	// colp
	class colp : public SS_Code {
	public:
		colp();
		~colp();

	};

	// colr
	class colr : public SS_Code {
	public:
		colr();
		~colr();

	};

	// mci
	class mci : public SS_Code {
	public:
		mci();
		~mci();

	};

	// allNameIdentificationSS
	class allNameIdentificationSS : public SS_Code {
	public:
		allNameIdentificationSS();
		~allNameIdentificationSS();

	};

	// cnap
	class cnap : public SS_Code {
	public:
		cnap();
		~cnap();

	};

	// allForwardingSS
	class allForwardingSS : public SS_Code {
	public:
		allForwardingSS();
		~allForwardingSS();

	};

	// cfu
	class cfu : public SS_Code {
	public:
		cfu();
		~cfu();

	};

	// allCondForwardingSS
	class allCondForwardingSS : public SS_Code {
	public:
		allCondForwardingSS();
		~allCondForwardingSS();

	};

	// cfb
	class cfb : public SS_Code {
	public:
		cfb();
		~cfb();

	};

	// cfnry
	class cfnry : public SS_Code {
	public:
		cfnry();
		~cfnry();

	};

	// cfnrc
	class cfnrc : public SS_Code {
	public:
		cfnrc();
		~cfnrc();

	};

	// cd
	class cd : public SS_Code {
	public:
		cd();
		~cd();

	};

	// allCallOfferingSS
	class allCallOfferingSS : public SS_Code {
	public:
		allCallOfferingSS();
		~allCallOfferingSS();

	};

	// ect
	class ect : public SS_Code {
	public:
		ect();
		~ect();

	};

	// mah
	class mah : public SS_Code {
	public:
		mah();
		~mah();

	};

	// allCallCompletionSS
	class allCallCompletionSS : public SS_Code {
	public:
		allCallCompletionSS();
		~allCallCompletionSS();

	};

	// cw
	class cw : public SS_Code {
	public:
		cw();
		~cw();

	};

	// hold
	class hold : public SS_Code {
	public:
		hold();
		~hold();

	};

	// ccbs_A
	class ccbs_A : public SS_Code {
	public:
		ccbs_A();
		~ccbs_A();

	};

	// ccbs_B
	class ccbs_B : public SS_Code {
	public:
		ccbs_B();
		~ccbs_B();

	};

	// mc
	class mc : public SS_Code {
	public:
		mc();
		~mc();

	};

	// allMultiPartySS
	class allMultiPartySS : public SS_Code {
	public:
		allMultiPartySS();
		~allMultiPartySS();

	};

	// multiPTY
	class multiPTY : public SS_Code {
	public:
		multiPTY();
		~multiPTY();

	};

	// allCommunityOfInterest_SS
	class allCommunityOfInterest_SS : public SS_Code {
	public:
		allCommunityOfInterest_SS();
		~allCommunityOfInterest_SS();

	};

	// cug
	class cug : public SS_Code {
	public:
		cug();
		~cug();

	};

	// allChargingSS
	class allChargingSS : public SS_Code {
	public:
		allChargingSS();
		~allChargingSS();

	};

	// aoci
	class aoci : public SS_Code {
	public:
		aoci();
		~aoci();

	};

	// aocc
	class aocc : public SS_Code {
	public:
		aocc();
		~aocc();

	};

	// allAdditionalInfoTransferSS
	class allAdditionalInfoTransferSS : public SS_Code {
	public:
		allAdditionalInfoTransferSS();
		~allAdditionalInfoTransferSS();

	};

	// uus1
	class uus1 : public SS_Code {
	public:
		uus1();
		~uus1();

	};

	// uus2
	class uus2 : public SS_Code {
	public:
		uus2();
		~uus2();

	};

	// uus3
	class uus3 : public SS_Code {
	public:
		uus3();
		~uus3();

	};

	// allBarringSS
	class allBarringSS : public SS_Code {
	public:
		allBarringSS();
		~allBarringSS();

	};

	// barringOfOutgoingCalls
	class barringOfOutgoingCalls : public SS_Code {
	public:
		barringOfOutgoingCalls();
		~barringOfOutgoingCalls();

	};

	// baoc
	class baoc : public SS_Code {
	public:
		baoc();
		~baoc();

	};

	// boic
	class boic : public SS_Code {
	public:
		boic();
		~boic();

	};

	// boicExHC
	class boicExHC : public SS_Code {
	public:
		boicExHC();
		~boicExHC();

	};

	// barringOfIncomingCalls
	class barringOfIncomingCalls : public SS_Code {
	public:
		barringOfIncomingCalls();
		~barringOfIncomingCalls();

	};

	// baic
	class baic : public SS_Code {
	public:
		baic();
		~baic();

	};

	// bicRoam
	class bicRoam : public SS_Code {
	public:
		bicRoam();
		~bicRoam();

	};

	// allPLMN_specificSS
	class allPLMN_specificSS : public SS_Code {
	public:
		allPLMN_specificSS();
		~allPLMN_specificSS();

	};

	// plmn_specificSS_1
	class plmn_specificSS_1 : public SS_Code {
	public:
		plmn_specificSS_1();
		~plmn_specificSS_1();

	};

	// plmn_specificSS_2
	class plmn_specificSS_2 : public SS_Code {
	public:
		plmn_specificSS_2();
		~plmn_specificSS_2();

	};

	// plmn_specificSS_3
	class plmn_specificSS_3 : public SS_Code {
	public:
		plmn_specificSS_3();
		~plmn_specificSS_3();

	};

	// plmn_specificSS_4
	class plmn_specificSS_4 : public SS_Code {
	public:
		plmn_specificSS_4();
		~plmn_specificSS_4();

	};

	// plmn_specificSS_5
	class plmn_specificSS_5 : public SS_Code {
	public:
		plmn_specificSS_5();
		~plmn_specificSS_5();

	};

	// plmn_specificSS_6
	class plmn_specificSS_6 : public SS_Code {
	public:
		plmn_specificSS_6();
		~plmn_specificSS_6();

	};

	// plmn_specificSS_7
	class plmn_specificSS_7 : public SS_Code {
	public:
		plmn_specificSS_7();
		~plmn_specificSS_7();

	};

	// plmn_specificSS_8
	class plmn_specificSS_8 : public SS_Code {
	public:
		plmn_specificSS_8();
		~plmn_specificSS_8();

	};

	// plmn_specificSS_9
	class plmn_specificSS_9 : public SS_Code {
	public:
		plmn_specificSS_9();
		~plmn_specificSS_9();

	};

	// plmn_specificSS_A
	class plmn_specificSS_A : public SS_Code {
	public:
		plmn_specificSS_A();
		~plmn_specificSS_A();

	};

	// plmn_specificSS_B
	class plmn_specificSS_B : public SS_Code {
	public:
		plmn_specificSS_B();
		~plmn_specificSS_B();

	};

	// plmn_specificSS_C
	class plmn_specificSS_C : public SS_Code {
	public:
		plmn_specificSS_C();
		~plmn_specificSS_C();

	};

	// plmn_specificSS_D
	class plmn_specificSS_D : public SS_Code {
	public:
		plmn_specificSS_D();
		~plmn_specificSS_D();

	};

	// plmn_specificSS_E
	class plmn_specificSS_E : public SS_Code {
	public:
		plmn_specificSS_E();
		~plmn_specificSS_E();

	};

	// plmn_specificSS_F
	class plmn_specificSS_F : public SS_Code {
	public:
		plmn_specificSS_F();
		~plmn_specificSS_F();

	};

	// allCallPrioritySS
	class allCallPrioritySS : public SS_Code {
	public:
		allCallPrioritySS();
		~allCallPrioritySS();

	};

	// emlpp
	class emlpp : public SS_Code {
	public:
		emlpp();
		~emlpp();

	};

	// allLCSPrivacyException
	class allLCSPrivacyException : public SS_Code {
	public:
		allLCSPrivacyException();
		~allLCSPrivacyException();

	};

	// universal
	class universal : public SS_Code {
	public:
		universal();
		~universal();

	};

	// callSessionRelated
	class callSessionRelated : public SS_Code {
	public:
		callSessionRelated();
		~callSessionRelated();

	};

	// callSessionUnrelated
	class callSessionUnrelated : public SS_Code {
	public:
		callSessionUnrelated();
		~callSessionUnrelated();

	};

	// plmnoperator
	class plmnoperator : public SS_Code {
	public:
		plmnoperator();
		~plmnoperator();

	};

	// serviceType
	class serviceType : public SS_Code {
	public:
		serviceType();
		~serviceType();

	};

	// allMOLR_SS
	class allMOLR_SS : public SS_Code {
	public:
		allMOLR_SS();
		~allMOLR_SS();

	};

	// basicSelfLocation
	class basicSelfLocation : public SS_Code {
	public:
		basicSelfLocation();
		~basicSelfLocation();

	};

	// autonomousSelfLocation
	class autonomousSelfLocation : public SS_Code {
	public:
		autonomousSelfLocation();
		~autonomousSelfLocation();

	};

	// transferToThirdParty
	class transferToThirdParty : public SS_Code {
	public:
		transferToThirdParty();
		~transferToThirdParty();

	};

}
#endif