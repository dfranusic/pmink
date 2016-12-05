#ifndef TCAP_H_
#define TCAP_H_

#include<asn1.h>

namespace asn1 {
    namespace itu_tcap {
	// forward declarations
	class ExternalPDU;
	class Dialog1;
	class TCMessage;
	class Unidirectional;
	class Begin;
	class End;
	class Continue;
	class Abort;
	class Reason;
	class DialoguePortion;
	class OrigTransactionID;
	class DestTransactionID;
	class P_AbortCause;
	class ComponentPortion;
	class Component;
	class Invoke;
	class Parameter;
	class ReturnResult;
	class ReturnError;
	class Reject;
	class InvokeIdType;
	class OPERATION;
	class ERROR;
	class GeneralProblem;
	class InvokeProblem;
	class ReturnResultProblem;
	class ReturnErrorProblem;
	class ErrorCode;
	class ReturnResult_resultretres;
	class Reject_invokeIDRej;
	class Reject_problem;

	// ReturnResult_resultretres
	class ReturnResult_resultretres : public Sequence {
	public:
		ReturnResult_resultretres();
		~ReturnResult_resultretres();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_parameter();
		// nodes
		OPERATION* _opCode;
		Parameter* _parameter;

	};

	// Reject_invokeIDRej
	class Reject_invokeIDRej : public Choice {
	public:
		Reject_invokeIDRej();
		~Reject_invokeIDRej();
		// nodes
		InvokeIdType* _derivable;
		Null* _not_derivable;

	};

	// Reject_problem
	class Reject_problem : public Choice {
	public:
		Reject_problem();
		~Reject_problem();
		// nodes
		GeneralProblem* _generalProblem;
		InvokeProblem* _invokeProblem;
		ReturnResultProblem* _returnResultProblem;
		ReturnErrorProblem* _returnErrorProblem;

	};

	

	// ExternalPDU
	class ExternalPDU : public Sequence {
	public:
		ExternalPDU();
		~ExternalPDU();
		// nodes
		Object_identifier* _oid;
		Dialog1* _dialog;

	};

	// Dialog1
	class Dialog1 : public Octet_string {
	public:
		Dialog1();
		~Dialog1();

	};

	// TCMessage
	class TCMessage : public Choice {
	public:
		TCMessage();
		~TCMessage();
		// nodes
		Unidirectional* _unidirectional;
		Begin* _begin;
		End* _end;
		Continue* _continue;
		Abort* _abort;

	};

	// Unidirectional
	class Unidirectional : public Sequence {
	public:
		Unidirectional();
		~Unidirectional();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_dialoguePortion();
		// nodes
		DialoguePortion* _dialoguePortion;
		ComponentPortion* _components;

	};

	// Begin
	class Begin : public Sequence {
	public:
		Begin();
		~Begin();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_dialoguePortion();
		void set_components();
		// nodes
		OrigTransactionID* _otid;
		DialoguePortion* _dialoguePortion;
		ComponentPortion* _components;

	};

	// End
	class End : public Sequence {
	public:
		End();
		~End();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_dialoguePortion();
		void set_components();
		// nodes
		DestTransactionID* _dtid;
		DialoguePortion* _dialoguePortion;
		ComponentPortion* _components;

	};

	// Continue
	class Continue : public Sequence {
	public:
		Continue();
		~Continue();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_dialoguePortion();
		void set_components();
		// nodes
		OrigTransactionID* _otid;
		DestTransactionID* _dtid;
		DialoguePortion* _dialoguePortion;
		ComponentPortion* _components;

	};

	// Abort
	class Abort : public Sequence {
	public:
		Abort();
		~Abort();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_reason();
		// nodes
		DestTransactionID* _dtid;
		Reason* _reason;

	};

	// Reason
	class Reason : public Choice {
	public:
		Reason();
		~Reason();
		// nodes
		P_AbortCause* _p_abortCause;
		DialoguePortion* _u_abortCause;

	};

	// DialoguePortion
	class DialoguePortion : public Octet_string {
	public:
		DialoguePortion();
		~DialoguePortion();
		External* _External_wrapper;
	};

	// OrigTransactionID
	class OrigTransactionID : public Octet_string {
	public:
		OrigTransactionID();
		~OrigTransactionID();

	};

	// DestTransactionID
	class DestTransactionID : public Octet_string {
	public:
		DestTransactionID();
		~DestTransactionID();

	};

	// P_AbortCause
	class P_AbortCause : public Integer {
	public:
		P_AbortCause();
		~P_AbortCause();

	};

	// ComponentPortion
	class ComponentPortion : public Sequence_of {
	public:
		ComponentPortion();
		~ComponentPortion();
		// nodes
		Component* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// Component
	class Component : public Choice {
	public:
		Component();
		~Component();
		// nodes
		Invoke* _invoke;
		ReturnResult* _returnResultLast;
		ReturnError* _returnError;
		Reject* _reject;
		ReturnResult* _returnResultNotLast;

	};

	// Invoke
	class Invoke : public Sequence {
	public:
		Invoke();
		~Invoke();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_linkedID();
		void set_parameter();
		// nodes
		InvokeIdType* _invokeID;
		InvokeIdType* _linkedID;
		OPERATION* _opCode;
		Parameter* _parameter;

	};

	// Parameter
	class Parameter : public Any {
	public:
		Parameter();
		~Parameter();

	};

	// ReturnResult
	class ReturnResult : public Sequence {
	public:
		ReturnResult();
		~ReturnResult();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_resultretres();
		// nodes
		InvokeIdType* _invokeID;
		ReturnResult_resultretres* _resultretres;

	};

	// ReturnError
	class ReturnError : public Sequence {
	public:
		ReturnError();
		~ReturnError();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_parameter();
		// nodes
		InvokeIdType* _invokeID;
		ErrorCode* _errorCode;
		Parameter* _parameter;

	};

	// Reject
	class Reject : public Sequence {
	public:
		Reject();
		~Reject();
		// nodes
		Reject_invokeIDRej* _invokeIDRej;
		Reject_problem* _problem;

	};

	// InvokeIdType
	class InvokeIdType : public Integer {
	public:
		InvokeIdType();
		~InvokeIdType();

	};

	// OPERATION
	class OPERATION : public Choice {
	public:
		OPERATION();
		~OPERATION();
		// nodes
		Integer* _localValue;
		Object_identifier* _globalValue;

	};

	// ERROR
	class ERROR : public Choice {
	public:
		ERROR();
		~ERROR();
		// nodes
		Integer* _localValue;
		Object_identifier* _globalValue;

	};

	// GeneralProblem
	class GeneralProblem : public Integer {
	public:
		GeneralProblem();
		~GeneralProblem();
		static const int _unrecognizedComponent = 0;
		static const int _mistypedComponent = 1;
		static const int _badlyStructuredComponent = 2;

	};

	// InvokeProblem
	class InvokeProblem : public Integer {
	public:
		InvokeProblem();
		~InvokeProblem();
		static const int _duplicateInvokeID = 0;
		static const int _unrecognizedOperation = 1;
		static const int _mistypedParameter = 2;
		static const int _resourceLimitation = 3;
		static const int _initiatingRelease = 4;
		static const int _unrecognizedLinkedID = 5;
		static const int _linkedResponseUnexpected = 6;
		static const int _unexpectedLinkedOperation = 7;

	};

	// ReturnResultProblem
	class ReturnResultProblem : public Integer {
	public:
		ReturnResultProblem();
		~ReturnResultProblem();
		static const int _unrecognizedInvokeID = 0;
		static const int _returnResultUnexpected = 1;
		static const int _mistypedParameter = 2;

	};

	// ReturnErrorProblem
	class ReturnErrorProblem : public Integer {
	public:
		ReturnErrorProblem();
		~ReturnErrorProblem();
		static const int _unrecognizedInvokeID = 0;
		static const int _returnErrorUnexpected = 1;
		static const int _unrecognizedError = 2;
		static const int _unexpectedError = 3;
		static const int _mistypedParameter = 4;

	};

	// ErrorCode
	class ErrorCode : public Choice {
	public:
		ErrorCode();
		~ErrorCode();
		// nodes
		Integer* _nationaler;
		Integer* _privateer;

	};

    }

}
#endif
