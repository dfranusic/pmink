#ifndef MAP_EXTENSIONDATATYPES_H_
#define MAP_EXTENSIONDATATYPES_H_

#include<asn1.h>

namespace asn1 {
	// forward declarations
	class ExtensionContainer;
	class SLR_ArgExtensionContainer;
	class PrivateExtensionList;
	class PrivateExtension;
	class maxNumOfPrivateExtensions;
	class SLR_Arg_PCS_Extensions;
	class PCS_Extensions;

	

	// ExtensionContainer
	class ExtensionContainer : public Sequence {
	public:
		ExtensionContainer();
		~ExtensionContainer();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_privateExtensionList();
		void set_pcs_Extensions();
		// nodes
		PrivateExtensionList* _privateExtensionList;
		PCS_Extensions* _pcs_Extensions;

	};

	// SLR_ArgExtensionContainer
	class SLR_ArgExtensionContainer : public Sequence {
	public:
		SLR_ArgExtensionContainer();
		~SLR_ArgExtensionContainer();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_privateExtensionList();
		void set_slr_Arg_PCS_Extensions();
		// nodes
		PrivateExtensionList* _privateExtensionList;
		SLR_Arg_PCS_Extensions* _slr_Arg_PCS_Extensions;

	};

	// PrivateExtensionList
	class PrivateExtensionList : public Sequence_of {
	public:
		PrivateExtensionList();
		~PrivateExtensionList();
		// nodes
		PrivateExtension* get_child(unsigned int child_index);
		void set_child(unsigned int child_index);
		ASN1Node* create_node(unsigned int _index);
		ASN1Node* get_next_node(unsigned int _index);

	};

	// PrivateExtension
	class PrivateExtension : public Sequence {
	public:
		PrivateExtension();
		~PrivateExtension();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_extType();
		// nodes
		Object_identifier* _extId;
		Any* _extType;

	};

	// maxNumOfPrivateExtensions
	class maxNumOfPrivateExtensions : public Integer {
	public:
		maxNumOfPrivateExtensions();
		~maxNumOfPrivateExtensions();

	};

	// SLR_Arg_PCS_Extensions
	class SLR_Arg_PCS_Extensions : public Sequence {
	public:
		SLR_Arg_PCS_Extensions();
		~SLR_Arg_PCS_Extensions();
		// optional
		ASN1Node* create_node(unsigned int _index);
		void set_na_ESRK_Request();
		// nodes
		Null* _na_ESRK_Request;

	};

	// PCS_Extensions
	class PCS_Extensions : public Sequence {
	public:
		PCS_Extensions();
		~PCS_Extensions();
		// optional

		// nodes

	};


}
#endif
