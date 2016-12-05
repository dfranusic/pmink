#ifndef DIALOGUEPDUS_H_
#define DIALOGUEPDUS_H_

#include<asn1.h>

namespace asn1 {
	// forward declarations
	class dialogue_as_id;
	class DialoguePDU;
	class AARQ_apdu;
	class AARE_apdu;
	class RLRQ_apdu;
	class RLRE_apdu;
	class ABRT_apdu;
	class ABRT_source;
	class Associate_result;
	class Associate_source_diagnostic;
	class Release_request_reason;
	class Release_response_reason;
	class AARQ_apdu_user_information;
	class AARE_apdu_user_information;
	class RLRQ_apdu_user_information;
	class RLRE_apdu_user_information;
	class ABRT_apdu_user_information;

	// AARQ_apdu_user_information
	class AARQ_apdu_user_information : public Sequence_of {
	public:
		AARQ_apdu_user_information();
		~AARQ_apdu_user_information();
		// nodes
		External* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// AARE_apdu_user_information
	class AARE_apdu_user_information : public Sequence_of {
	public:
		AARE_apdu_user_information();
		~AARE_apdu_user_information();
		// nodes
		External* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// RLRQ_apdu_user_information
	class RLRQ_apdu_user_information : public Sequence_of {
	public:
		RLRQ_apdu_user_information();
		~RLRQ_apdu_user_information();
		// nodes
		External* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// RLRE_apdu_user_information
	class RLRE_apdu_user_information : public Sequence_of {
	public:
		RLRE_apdu_user_information();
		~RLRE_apdu_user_information();
		// nodes
		External* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// ABRT_apdu_user_information
	class ABRT_apdu_user_information : public Sequence_of {
	public:
		ABRT_apdu_user_information();
		~ABRT_apdu_user_information();
		// nodes
		External* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	

	// dialogue_as_id
	class dialogue_as_id : public Object_identifier {
	public:
		dialogue_as_id();
		~dialogue_as_id();

	};

	// DialoguePDU
	class DialoguePDU : public Choice {
	public:
		DialoguePDU();
		~DialoguePDU();
		// nodes
		AARQ_apdu* _dialogueRequest;
		AARE_apdu* _dialogueResponse;
		ABRT_apdu* _dialogueAbort;

	};

	// AARQ_apdu
	class AARQ_apdu : public Sequence {
	public:
		AARQ_apdu();
		~AARQ_apdu();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_user_information();
		// nodes
		Bit_string* _protocol_version;
		Object_identifier* _application_context_name;
		AARQ_apdu_user_information* _user_information;

	};

	// AARE_apdu
	class AARE_apdu : public Sequence {
	public:
		AARE_apdu();
		~AARE_apdu();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_user_information();
		// nodes
		Bit_string* _protocol_version;
		Object_identifier* _application_context_name;
		Associate_result* _result;
		Associate_source_diagnostic* _result_source_diagnostic;
		AARE_apdu_user_information* _user_information;

	};

	// RLRQ_apdu
	class RLRQ_apdu : public Sequence {
	public:
		RLRQ_apdu();
		~RLRQ_apdu();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_reason();
		void set_user_information();
		// nodes
		Release_request_reason* _reason;
		RLRQ_apdu_user_information* _user_information;

	};

	// RLRE_apdu
	class RLRE_apdu : public Sequence {
	public:
		RLRE_apdu();
		~RLRE_apdu();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_reason();
		void set_user_information();
		// nodes
		Release_response_reason* _reason;
		RLRE_apdu_user_information* _user_information;

	};

	// ABRT_apdu
	class ABRT_apdu : public Sequence {
	public:
		ABRT_apdu();
		~ABRT_apdu();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_user_information();
		// nodes
		ABRT_source* _abort_source;
		ABRT_apdu_user_information* _user_information;

	};

	// ABRT_source
	class ABRT_source : public Integer {
	public:
		ABRT_source();
		~ABRT_source();
		static const int _dialogue_service_user = 0;
		static const int _dialogue_service_provider = 1;

	};

	// Associate_result
	class Associate_result : public Integer {
	public:
		Associate_result();
		~Associate_result();
		static const int _accepted = 0;
		static const int _reject_permanent = 1;

	};

	// Associate_source_diagnostic
	class Associate_source_diagnostic : public Choice {
	public:
		Associate_source_diagnostic();
		~Associate_source_diagnostic();
		// nodes
		Integer* _dialogue_service_user;
		Integer* _dialogue_service_provider;

	};

	// Release_request_reason
	class Release_request_reason : public Integer {
	public:
		Release_request_reason();
		~Release_request_reason();
		static const int _normal = 0;
		static const int _urgent = 1;
		static const int _user_defined = 30;

	};

	// Release_response_reason
	class Release_response_reason : public Integer {
	public:
		Release_response_reason();
		~Release_response_reason();
		static const int _normal = 0;
		static const int _not_finished = 1;
		static const int _user_defined = 30;

	};

}
#endif