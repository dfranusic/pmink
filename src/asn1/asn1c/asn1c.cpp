#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <antlr3.h>
#include <algorithm>
#include <asn1Lexer.h>
#include <asn1Parser.h>
using namespace std;


bool explicit_tagging = true;

pANTLR3_BASE_TREE tag_def_tree;

void fix_name(char* data, int data_length){
	if(data != NULL){
		for(int i = 0; i<data_length; i++) if(data[i] == '-') data[i] = '_';
	}
}

void fix_name(string* data){
	if(data != NULL){
		for(unsigned int i = 0; i<data->size(); i++) if((*data)[i] == '-') (*data)[i] = '_';
	}
}


void print_tree(pANTLR3_BASE_TREE tree, int depth){
	pANTLR3_BASE_TREE tmp_tree;
	int n;
	// padding
	for(int i = 0; i<depth; i++) cout << "  ";
	// node name/value
	cout << tree->toString(tree)->chars << endl;
	// children
	if(tree->children != NULL){
		// child count
		n = tree->children->size(tree->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)tree->children->get(tree->children, i);
			// print
			print_tree(tmp_tree, depth + 1);
		}
	}

}

bool is_implicit(pANTLR3_BASE_TREE node_info_node){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_child;
	string tmp_str;
	if(node_info_node != NULL){
		if(node_info_node->children != NULL){
			// child count
			int n = node_info_node->children->size(node_info_node->children);
			for(int i = 0; i<n; i++){
				tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
				tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
				if(strcmp(tmp_str.c_str(), "TAG_DESCRIPTOR") == 0){
					int n2 = tmp_tree->children->size(tmp_tree->children);
					for(int j = 0; j<n2; j++){
						tmp_tree_child = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, j);
						tmp_str = (char*)tmp_tree_child->toString(tmp_tree_child)->chars;

						if(strcmp(tmp_str.c_str(), "IMPLICIT") == 0){
							return true;
						}else if(strcmp(tmp_str.c_str(), "EXPLICIT") == 0){
							return false;

						}
					}
				}
			}
		}
	}
	return !explicit_tagging;

}

const char* get_bultin_type(pANTLR3_BASE_TREE node, const char* user_type);

bool is_explicit(pANTLR3_BASE_TREE node_info_node){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_child;
	string tmp_str;
	bool tag_desc_found = false;
	bool choice_found = false;
	if(node_info_node != NULL){
		if(node_info_node->children != NULL){
			// child count
			int n = node_info_node->children->size(node_info_node->children);
			// check for inline CHOICE first
			for(int i = 0; i<n; i++){
				tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
				tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
				 if(strcmp(tmp_str.c_str(), "CHOICE") == 0) choice_found = true;

			}

			// check for CHOICE type in list of known types
			for(int i = 0; i<n; i++){
				tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
				tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
				const char* tmp_chr = get_bultin_type(tag_def_tree, tmp_str.c_str());
				if(tmp_chr != NULL) {
				    if(strcmp(tmp_chr, "CHOICE") == 0) choice_found = true;
				}

			}

			// check tag descriptor
			for(int i = 0; i<n; i++){
				tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
				tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
				//cout << "!!!!!!!!!!!!!!!!!:" << tmp_str << endl;

				if(strcmp(tmp_str.c_str(), "TAG_DESCRIPTOR") == 0){
					tag_desc_found = true;
					int n2 = tmp_tree->children->count;
					if(n2 == 0) {
						return false;
					}
					for(int j = 0; j<n2; j++){
						tmp_tree_child = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, j);
						tmp_str = (char*)tmp_tree_child->toString(tmp_tree_child)->chars;
						//cout << tmp_str << endl;

						// explicit
						if(strcmp(tmp_str.c_str(), "EXPLICIT") == 0){
							return true;

						// implicit
						}else if(strcmp(tmp_str.c_str(), "IMPLICIT") == 0){
							if(choice_found) return true; else return false;

						// choice is always explicit
						}else if(choice_found) return true;
					}
				}
			}
			if(!tag_desc_found) return false;
		}
	}
	return explicit_tagging;

}


bool is_optional(pANTLR3_BASE_TREE node_info_node, char* member){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_child_2;
	pANTLR3_BASE_TREE tmp_tree_child_3;
	string tmp_str;
	string tmp_str2;
	stringstream convert;
	if(node_info_node->children != NULL){
		// child count
		int n = node_info_node->children->size(node_info_node->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
			tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
			if(strcmp(tmp_str.c_str(), "NODE_INFO") == 0){
				tmp_tree_child_2 = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, 0);
				tmp_str = (char*)tmp_tree_child_2->toString(tmp_tree_child_2)->chars;
				fix_name(&tmp_str);

				if(strcmp(tmp_str.c_str(), member) == 0){
					if(tmp_tree->children != NULL){
						int n2 = tmp_tree->children->count;

						for(int j = 0; j<n2; j++){
							tmp_tree_child_2 = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, j);
							tmp_str = (char*)tmp_tree_child_2->toString(tmp_tree_child_2)->chars;

							if(strcmp(tmp_str.c_str(), "TAG_DESCRIPTOR") == 0){
								//cout << "AAAAAAAAAAAAAA: " << tmp_str << " - " << member << endl;
								if(tmp_tree_child_2->children != NULL){
									int n3 = tmp_tree_child_2->children->size(tmp_tree_child_2->children);
									for(int k = 0; k<n3; k++){
										tmp_tree_child_3 = (pANTLR3_BASE_TREE)tmp_tree_child_2->children->get(tmp_tree_child_2->children, k);
										tmp_str2 = (char*)tmp_tree_child_3->toString(tmp_tree_child_3)->chars;
										if(strcmp(tmp_str2.c_str(), "OPTIONAL") == 0){
											return true;
										}

									}

								}
								break;

							}
						}

					}
					break;

				}

			}

		}
	}
	return false;
}

void get_type(	string* output_hdr,
				string* output_hdr_fwd,
				string* output_src,
				pANTLR3_BASE_TREE node_info_node,
				char* ret_type,
				int* type_size,
				bool check_inner,
				string* current_class,
				string* current_field,
				bool no_output,
				bool _explicit_tagging,
				bool skip_tag_descriptor);

pANTLR3_BASE_TREE find_node_info(pANTLR3_BASE_TREE node);

void check_optional(pANTLR3_BASE_TREE node_info_node, string* output, string* output_src, string* member){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_child_2;
	pANTLR3_BASE_TREE tmp_tree_child_3;
	string tmp_str;
	string tmp_str2;
	stringstream convert;
	bool hdr_done = false;
	if(node_info_node->children != NULL){
		// child count
		int n = node_info_node->children->size(node_info_node->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
			tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
			if(strcmp(tmp_str.c_str(), "NODE_INFO") == 0){
				tmp_tree_child_2 = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, 0);
				tmp_str = (char*)tmp_tree_child_2->toString(tmp_tree_child_2)->chars;


				if(tmp_tree->children->count > 1){
					tmp_tree_child_2 = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, 1);
					if(tmp_tree_child_2->children != NULL){
						int n3 = tmp_tree_child_2->children->size(tmp_tree_child_2->children);
						for(int k = 0; k<n3; k++){
							tmp_tree_child_3 = (pANTLR3_BASE_TREE)tmp_tree_child_2->children->get(tmp_tree_child_2->children, k);
							tmp_str2 = (char*)tmp_tree_child_3->toString(tmp_tree_child_3)->chars;
							if(strcmp(tmp_str2.c_str(), "OPTIONAL") == 0){
								// create_node
								if(!hdr_done){
									output->append("\t\t// optional\n\t\tASN1Node* create_node(unsigned int _index);\n");
									output_src->append("asn1::ASN1Node* asn1::" + *member + "::create_node(unsigned int _index){\n");
									output_src->append("\tswitch(_index){\n");
									output_src->append("\t\t@_OPTIONAL_@\n");
									output_src->append("\t\tdefault: return NULL;\n");
									output_src->append("\t}\n");
									output_src->append("}\n\n");



									hdr_done = true;

								}
								// set optional node
								// header
								fix_name(&tmp_str);
								output->append("\t\tvoid set_" + tmp_str);
								output->append("();\n");

								convert << i;

								// src
								output_src->append("void asn1::" + *member + "::set_" + tmp_str + "(){\n");
								output_src->append("\tif(_" + tmp_str + " == NULL) _" + tmp_str + " = (@PARAM_OPTIONAL@*)create_node(" + convert.str() +");\n");
								output_src->append("}\n\n");
								convert.str("");
							}

						}

					}

				}

			}

		}
	}
}


void get_tag_descriptor_src(pANTLR3_BASE_TREE node_info_node, string* output, char* member, bool root_node, bool skip_expl, string* main_wrapper, char* prefix){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_child;
	string tmp_str;
	string tmp_class("CONTEXT_SPECIFIC");
	string wrapper_name;
	string orig_member(member);
	bool expl = is_explicit(node_info_node);
	bool exp_done = false;

	// override explicit check
	// assume explicit

	if(expl && !skip_expl){
		if(!root_node){
			wrapper_name = "";
			wrapper_name.append(member);
			wrapper_name.append("_wrapper");
			strcpy(member, wrapper_name.c_str());
			//output->append("\t_");
			//output->append(member);
			//output->append("->tlv->is_explicit = true;\n");
			// ASN1Node* _test_expl_outer = new ASN1Node();


			output->append(prefix);
			output->append("ASN1Node* _");
			output->append(wrapper_name);
			output->append(" = new ASN1Node();\n");
			output->append(prefix);
			output->append("_");
			output->append(wrapper_name);
			output->append("->tlv = new TLVNode();\n");
			output->append(prefix);
			output->append("_");
			output->append(wrapper_name);
			output->append("->tlv->is_explicit = true;\n");
			// tlv = new TLVNode();




		}else{

			output->append(prefix);
			output->append("tlv->is_explicit = true;\n");


			wrapper_name = "";
			wrapper_name.append(member);
			wrapper_name.append("_wrapper");
			strcpy(member, wrapper_name.c_str());
			//output->append("\t==============\n");

			//output->append(prefix);
			//output->append(orig_member);
			//output->append("* _");
			output->append("\t_");
			output->append(wrapper_name);
			output->append(" = new ");
			output->append(orig_member);
			output->append("();\n");
			output->append(prefix);
			output->append("children.push_back(_");
			output->append(wrapper_name);
			output->append(");\n");
			//output->append("\t==============\n");


			if(main_wrapper != NULL){
				//cout << "NOT NULL" << endl;
				if(main_wrapper->size() == 0){
					*main_wrapper = "_";
					main_wrapper->append(wrapper_name);
					//cout << "AAAAAAAAAAAAAAAAAAA: " << *main_wrapper << endl;

				}

			}


/*
			output->append("\t");
			output->append("_explicit_element = new ");
			output->append(member);
			output->append("();\n");
			output->append("\tchildren.push_back(_explicit_element);\n");
*/

		}

		exp_done = true;
		//return;
	}else wrapper_name.append(member);


	if(node_info_node != NULL){
		if(node_info_node->children != NULL){
			// child count
			int n = node_info_node->children->size(node_info_node->children);
			for(int i = 0; i<n; i++){
				tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
				tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
				if(strcmp(tmp_str.c_str(), "TAG_DESCRIPTOR") == 0){
					int n2 = tmp_tree->children->size(tmp_tree->children);
					for(int j = 0; j<n2; j++){
						tmp_tree_child = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, j);
						tmp_str = (char*)tmp_tree_child->toString(tmp_tree_child)->chars;

						if(strcmp(tmp_str.c_str(), "IMPLICIT") == 0){
							//tmp_class = "";
						}else if(strcmp(tmp_str.c_str(), "EXPLICIT") == 0){
							if(!exp_done){
								if(!root_node){
									output->append(prefix);
									output->append("_");
									output->append(member);
									output->append("->tlv->is_explicit = true;\n");


								}else{

									output->append(prefix);
									output->append("tlv->is_explicit = true;\n");
									output->append(prefix);
									output->append("_explicit_element = new ");
									output->append(member);
									output->append("();\n");
									output->append(prefix);
									output->append("children.push_back(_explicit_element);\n");

								}

							}else tmp_str = "";

						}else if(strcmp(tmp_str.c_str(), "APPLICATION") == 0){
							tmp_class = "APPLICATION";
						}else if(strcmp(tmp_str.c_str(), "PRIVATE") == 0){
							tmp_class = "PRIVATE";
						}else if(strcmp(tmp_str.c_str(), "UNIVERSAL") == 0){
							tmp_class = "UNIVERSAL";
						}else{
							// tag class
							if(tmp_class.size() > 0){

								if(!root_node){
									output->append(prefix);
									output->append("_");
									output->append(wrapper_name);
									output->append("->tlv->tag_class = ");
									output->append(tmp_class);
									output->append(";\n");

								}else{
									output->append(prefix);
									output->append("tlv->tag_class = ");
									output->append(tmp_class);
									output->append(";\n");

								}

							}
							// tag value
							if(!root_node){
								output->append(prefix);
								output->append("_");
								output->append(wrapper_name);
								output->append("->tlv->tag_value = ");
								output->append(tmp_str);
								output->append(";\n");

							}else{
								output->append(prefix);
								output->append("tlv->tag_value = ");
								output->append(tmp_str);
								output->append(";\n");

							}

							// exp extra
							if(exp_done && !root_node){

								output->append(prefix);
								output->append("_");
								output->append(wrapper_name);
								output->append("->children.push_back(_");
								output->append(orig_member);
								output->append(");\n");


							}

							//cout << "get_tag_descriptor_src: " << tmp_str << " - " << atoi(tmp_str.c_str()) << endl;

						}

					}
				}



			}
		}
	}
}
void get_builtin_name(string* type);

void get_tag_descriptor_header_explicit(pANTLR3_BASE_TREE node_info_node, string* output, char* node_type, bool _is_explicit_override){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_child;
	string tmp_str;
	string tmp_node;
	bool expl = false;

	if(node_info_node != NULL){
		if(node_info_node->children != NULL){
			// child count
			int n = node_info_node->children->size(node_info_node->children);
			for(int i = 0; i<n; i++){
				tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
				tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
				if(strcmp(tmp_str.c_str(), "TAG_DESCRIPTOR") == 0){
					expl = explicit_tagging;
					int n2 = tmp_tree->children->size(tmp_tree->children);
					for(int j = 0; j<n2; j++){
						tmp_tree_child = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, j);
						tmp_str = (char*)tmp_tree_child->toString(tmp_tree_child)->chars;
						if(strcmp(tmp_str.c_str(), "EXPLICIT") == 0){
							expl = true;
							/*
							output->append("\t\t// explicit\n");
							output->append("\t\t");
							output->append(node_type);
							output->append("* _explicit_element;\n");
							*/
						}else if(strcmp(tmp_str.c_str(), "IMPLICIT") == 0){
							expl = false;
						}
					}
				}else{
					if(expl || _is_explicit_override){
						//output->append("\t\t// explicit\n");
						//output->append("\t\t");
						get_builtin_name(&tmp_str);
						//output->append(tmp_str);
						//output->append("* _explicit_element;\n");
						strcpy(node_type, tmp_str.c_str());

					}
				}



			}
		}
	}
}


void get_builtin_name(string* type){
	char tmp_char[500];
	strcpy(tmp_char, type->c_str());
	// BUILTIN
	if(	strcmp(type->c_str(), "SEQUENCE") == 0 ||
		strcmp(type->c_str(), "INTEGER") == 0 ||
		strcmp(type->c_str(), "CHOICE") == 0 ||
		strcmp(type->c_str(), "BOOLEAN") == 0 ||
		strcmp(type->c_str(), "BIT_STRING") == 0 ||
		strcmp(type->c_str(), "OCTET_STRING") == 0 ||
		strcmp(type->c_str(), "NULL") == 0 ||
		strcmp(type->c_str(), "OBJECT_IDENTIFIER") == 0 ||
		strcmp(type->c_str(), "OBJECT_DESCRIPTOR") == 0 ||
		strcmp(type->c_str(), "EXTERNAL") == 0 ||
		strcmp(type->c_str(), "REAL") == 0 ||
		strcmp(type->c_str(), "ENUMERATED") == 0 ||
		strcmp(type->c_str(), "EMBEDDED_PDV") == 0 ||
		strcmp(type->c_str(), "UTF8STRING") == 0 ||
		strcmp(type->c_str(), "RELATIVE_OID") == 0 ||
		strcmp(type->c_str(), "SEQUENCE_OF") == 0 ||
		strcmp(type->c_str(), "SET") == 0 ||
		strcmp(type->c_str(), "NUMERIC_STRING") == 0 ||
		strcmp(type->c_str(), "PRINTABLE_STRING") == 0 ||
		strcmp(type->c_str(), "T61STRING") == 0 ||
		strcmp(type->c_str(), "VIDEOTEX_STRING") == 0 ||
		strcmp(type->c_str(), "IA5_STRING") == 0 ||
		strcmp(type->c_str(), "UTC_TIME") == 0 ||
		strcmp(type->c_str(), "GENERALIZED_TIME") == 0 ||
		strcmp(type->c_str(), "GRAPHIC_STRING") == 0 ||
		strcmp(type->c_str(), "VISIBLE_STRING") == 0 ||
		strcmp(type->c_str(), "GENERAL_STRING") == 0 ||
		strcmp(type->c_str(), "UNIVERSAL_STRING") == 0 ||
		strcmp(type->c_str(), "CHARACTER_STRING") == 0 ||
		strcmp(type->c_str(), "BMP_STRING") == 0){
		//cout << "((((((((((((((((((((((((((get_builtin_name: " << tmp_char << endl;

		tmp_char[0] = toupper(tmp_char[0]);
		for(unsigned int j = 1; j<type->size(); j++) tmp_char[j] = tolower(tmp_char[j]);

	// ANY
	}else if(strcmp(type->c_str(), "ANY") == 0){
		//ret_type = new char[9];
		*type = "Any\0";
		//strcpy(type, "ASN1Node\0");
		//ret_type[8] = 0;
		//*type_size = 9;
		//return ret_type;

	}

	*type = "";
	type->append(tmp_char);

}



// fwd declaration
void process_sequence_choice(pANTLR3_BASE_TREE node_info_node, string* output, string* output_hdr_fwd, string* output_src, string* current_class, string* prefix);
void get_tag_specific_integer(pANTLR3_BASE_TREE node_tag_specific, string* output);
void get_tag_specific_enumerated(pANTLR3_BASE_TREE node_tag_specific, string* output, int sindex = 0);
pANTLR3_BASE_TREE find_tag_specific(pANTLR3_BASE_TREE node);
pANTLR3_BASE_TREE find_node_info(pANTLR3_BASE_TREE node);
void process_sequence_of(pANTLR3_BASE_TREE node_info_node, string* output, string* output_hdr_fwd, string* output_src, string* current_class);

void get_type(	string* output_hdr,
				string* output_hdr_fwd,
				string* output_src,
				pANTLR3_BASE_TREE node_info_node,
				char* ret_type,
				int* type_size,
				bool check_inner,
				string* current_class,
				string* current_field,
				bool no_output,
				bool _explicit_tagging,
				bool skip_tag_descriptor){
	pANTLR3_BASE_TREE tmp_tree;
	string tmp_str;
	//pANTLR3_BASE_TREE tmp_tree_child;
	string tmp_str_child;
	string tmp_inner;
	string tmp_output;
	string tmp_output_src;
	string tmp_wrapper;

	char tmp_explicit[500];
	//char* ret_type;
	if(node_info_node->children != NULL){
		//cout << "!@@@@@@@@@@@@@@@@@@@@@@@@ " << (char*)node_info_node->toString(node_info_node)->chars << endl;
		// child count
		int n = node_info_node->children->count;
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
			tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;


			if(strcmp(tmp_str.c_str(), "TAG_DESCRIPTOR") != 0 && strcmp(tmp_str.c_str(), "VALUE_NODE") != 0){
				if(check_inner){
					if(tmp_tree->children != NULL){
						//tmp_tree_child = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, 0);

						if(	strcmp(tmp_str.c_str(), "CHOICE") == 0 ||
							strcmp(tmp_str.c_str(), "SEQUENCE") == 0 ||
							strcmp(tmp_str.c_str(), "SEQUENCE_OF") == 0 ||
							strcmp(tmp_str.c_str(), "ENUMERATED") == 0) {

							string tmp_name_str;
							//cout << "=================" << endl;
							//cout << tmp_str << endl;

							tmp_inner = *current_class;
							tmp_inner.append("_");
							tmp_inner.append(current_field->c_str());

							get_type(&tmp_output, output_hdr_fwd, &tmp_output_src, node_info_node, ret_type, type_size, false, &tmp_inner, current_field, false, _explicit_tagging, skip_tag_descriptor);


							//cout << "AAAAAAAAAAA: " << ret_type << endl;



							//process_sequence_choice(node_info_node, &tmp_inner, current_class);
							//cout << tmp_inner << endl;
							// fwd declarations
							output_hdr_fwd->append("\tclass ");
							output_hdr_fwd->append(tmp_inner);
							output_hdr_fwd->append(";\n");
							//cout << "=================" << endl;

							tmp_name_str = *current_class + "_" + *current_field;
							strcpy(ret_type, tmp_name_str.c_str());
							ret_type[tmp_name_str.size() + tmp_name_str.size()] = 0;
							*type_size = tmp_name_str.size() + tmp_name_str.size() + 1;

							//output_hdr->append("\n\t};\n\n");
							//output_hdr->insert(0, tmp_output);


							// header
							int indx = output_hdr->find("\t@@AUTO_CLS@@");
							if(indx > -1){
								output_hdr->insert(indx, tmp_output);


							}else{
								output_hdr->insert(0, tmp_output);
								//cout << "ERRRRRR" << endl;
							}

							// src
							string tmp_find("\t@@CNSTR_");
							tmp_find.append(tmp_inner);
							tmp_find.append("@@");
							indx = output_src->find(tmp_find);

							if(indx > -1){
								//cout << "LITA-----------: " << tmp_find << endl;
								output_src->insert(indx, tmp_output_src);
								//output_src->append(tmp_output_src.c_str());

							}else{
								output_src->append(tmp_output_src);

							}


							//asn1_header.insert(indx, asn1_header_fwd);

							return;
						}


					}

				}


				// BUILTIN
				if(	strcmp(tmp_str.c_str(), "SEQUENCE") == 0 ||
					strcmp(tmp_str.c_str(), "INTEGER") == 0 ||
					strcmp(tmp_str.c_str(), "CHOICE") == 0 ||
					strcmp(tmp_str.c_str(), "BOOLEAN") == 0 ||
					strcmp(tmp_str.c_str(), "BIT_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "OCTET_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "NULL") == 0 ||
					strcmp(tmp_str.c_str(), "OBJECT_IDENTIFIER") == 0 ||
					strcmp(tmp_str.c_str(), "OBJECT_DESCRIPTOR") == 0 ||
					strcmp(tmp_str.c_str(), "EXTERNAL") == 0 ||
					strcmp(tmp_str.c_str(), "REAL") == 0 ||
					strcmp(tmp_str.c_str(), "ENUMERATED") == 0 ||
					strcmp(tmp_str.c_str(), "EMBEDDED_PDV") == 0 ||
					strcmp(tmp_str.c_str(), "UTF8STRING") == 0 ||
					strcmp(tmp_str.c_str(), "RELATIVE_OID") == 0 ||
					strcmp(tmp_str.c_str(), "SEQUENCE_OF") == 0 ||
					strcmp(tmp_str.c_str(), "SET") == 0 ||
					strcmp(tmp_str.c_str(), "NUMERIC_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "PRINTABLE_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "T61STRING") == 0 ||
					strcmp(tmp_str.c_str(), "VIDEOTEX_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "IA5_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "UTC_TIME") == 0 ||
					strcmp(tmp_str.c_str(), "GENERALIZED_TIME") == 0 ||
					strcmp(tmp_str.c_str(), "GRAPHIC_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "VISIBLE_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "GENERAL_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "UNIVERSAL_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "CHARACTER_STRING") == 0 ||
					strcmp(tmp_str.c_str(), "BMP_STRING") == 0){

					tmp_str[0] = toupper(tmp_str[0]);
					for(unsigned int j = 1; j<tmp_str.size(); j++) tmp_str[j] = tolower(tmp_str[j]);
					//cout << "Type: " << tmp_str << endl;

				// ANY
				}else if(strcmp(tmp_str.c_str(), "ANY") == 0){
					//ret_type = new char[9];
					tmp_str = "Any";
					//strcpy(ret_type, "ASN1Node");
					//ret_type[8] = 0;
					//*type_size = 9;
					//return ret_type;

				}

				// other types
				//ret_type = new char[tmp_str.size() + 1];

				strcpy(ret_type, tmp_str.c_str());
				ret_type[tmp_str.size()] = 0;
				*type_size = tmp_str.size() + 1;
				//return ret_type;



				//tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
				replace( tmp_str.begin(), tmp_str.end(), '-', '_' );

				//if(no_output) return;

				if(!no_output){
					//cout << tmp_tree->toString(tmp_tree)->chars << endl;
					output_hdr->append("\t// ");
					output_hdr->append(*current_class);
					output_hdr->append("\n");
					output_hdr->append("\tclass ");
					output_hdr->append(*current_class);
					//dest_hdr->append(" : public ASN1Node {\n");
					output_hdr->append(" : public ");

					// check if not explicit
					if(!is_explicit(tmp_tree)){
					    output_hdr->append(tmp_str);

					// explicit and not any
					}else if(tmp_str != "Any"){
					    output_hdr->append("Octet_string");

					// any
					}else output_hdr->append(tmp_str);

					output_hdr->append(" {\n");
					output_hdr->append("\tpublic:\n");
					output_hdr->append("\t\t");
					output_hdr->append(*current_class);
					output_hdr->append("();\n");
					output_hdr->append("\t\t~");
					output_hdr->append(*current_class);
					output_hdr->append("();\n");


					// get explicit tag descriptor - header
					if(!no_output){
						strcpy(tmp_explicit, current_class->c_str());
						get_tag_descriptor_header_explicit(node_info_node, output_hdr, tmp_explicit, false);
					}

					// explicit, pointer to inner node
					if(is_explicit(tmp_tree)){
						output_hdr->append("\t\t");
						output_hdr->append(tmp_explicit);
						output_hdr->append("* _");
						output_hdr->append(tmp_explicit);
						output_hdr->append("_wrapper");
						output_hdr->append(";\n");

					}


					// constructor
					output_src->append("//");
					output_src->append(*current_class);
					output_src->append("\n");
					output_src->append("asn1::");
					output_src->append(*current_class);
					output_src->append("::");
					output_src->append(*current_class);
					output_src->append("(){\n");
					output_src->append("\t");
					output_src->append("strcpy(node_type_name, \"");
					output_src->append(*current_class);
					output_src->append("\");\n");
					output_src->append("\t@@CNSTR_");
					output_src->append(*current_class);
					output_src->append("@@\n");
					output_src->append("}\n");



					// destructor	cout << a[0] << endl;

					output_src->append("asn1::");
					output_src->append(*current_class);
					output_src->append("::~");
					output_src->append(*current_class);
					output_src->append("(){\n");
					//output_src->append("\tchildren.clear();\n");
					output_src->append("\t@@DSTR_");
					output_src->append(*current_class);
					output_src->append("@@\n");
					output_src->append("}\n\n");


				}


				if(!no_output){
					// skip_tag_descriptor = true - inside SEQUENCE or CHOICE
					if(!skip_tag_descriptor || _explicit_tagging){
						// get tag descriptors - src
						string tmp_str("");
						string tmp_output;
						get_tag_descriptor_src(node_info_node, &tmp_output, tmp_explicit, true, (skip_tag_descriptor && _explicit_tagging), &tmp_wrapper, (char*)"\t");
						string tmp_fnd("\t@@CNSTR_");
						tmp_fnd.append(*current_class);
						tmp_fnd.append("@@");
						int idx = output_src->find(tmp_fnd);
						if(idx > -1){
							output_src->insert(idx, tmp_output);
						}

					}

				}


				// SEQUENCE
				if(strcmp(tmp_str.c_str(), "Sequence") == 0){
					process_sequence_choice(node_info_node, output_hdr, output_hdr_fwd, output_src, current_class, &tmp_wrapper);
				// SEQUENCE OF
				}else if(strcmp(tmp_str.c_str(), "Sequence_of") == 0){
					process_sequence_of(node_info_node, output_hdr, output_hdr_fwd, output_src, current_class);
				// CHOICE
				}else if(strcmp(tmp_str.c_str(), "Choice") == 0){
					process_sequence_choice(node_info_node, output_hdr, output_hdr_fwd, output_src, current_class, &tmp_wrapper);

				// INTEGER
				}else if(strcmp(tmp_str.c_str(), "Integer") == 0){
					get_tag_specific_integer(find_tag_specific(node_info_node), output_hdr);

				// ENUMERATED
				}else if(strcmp(tmp_str.c_str(), "Enumerated") == 0){
				    get_tag_specific_enumerated(node_info_node, output_hdr);


				    //std::cout << "!!!!!ENUMERATED: " << node_info_node->toString(node_info_node)->chars << std::endl;

				}







				if(!no_output) output_hdr->append("\n\t};\n\n");

				if(!no_output){
					// remove position markers
					string tmp_fnd("\t@@CNSTR_");
					tmp_fnd.append(*current_class);
					tmp_fnd.append("@@");
					int idx = output_src->find(tmp_fnd);
					if(idx > -1) output_src->replace(idx, tmp_fnd.size(), "");

					tmp_fnd = "\t@@DSTR_";
					tmp_fnd.append(*current_class);
					tmp_fnd.append("@@");
					idx = output_src->find(tmp_fnd);
					if(idx > -1) output_src->replace(idx, tmp_fnd.size(), "");

				}


			}

		}
	}
	//return NULL;
}

void get_member(pANTLR3_BASE_TREE node_info_node, char* ret_type, int* ret_type_size){
	pANTLR3_BASE_TREE tmp_tree;
	string tmp_str;
	//char* ret_type;
	if(node_info_node->children != NULL){
		// child count
		int n = node_info_node->children->size(node_info_node->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
			tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
			if(strcmp(tmp_str.c_str(), "TAG_DESCRIPTOR") != 0){
				//ret_type = new char[tmp_str.size() + 1];
				strcpy(ret_type, tmp_str.c_str());
				ret_type[tmp_str.size()] = 0;
				*ret_type_size = tmp_str.size() + 1;
				//return ret_type;
			}

		}
	}
	//return NULL;
}


void get_tag_specific_integer(pANTLR3_BASE_TREE node_tag_specific, string* output){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_2;
	string tmp_str;
	string tmp_val;

	if(node_tag_specific != NULL){
		if(node_tag_specific->children != NULL){
			if(node_tag_specific->children->size > 0){
				// child count
				int n = node_tag_specific->children->size(node_tag_specific->children);
				for(int i = 0; i<n; i++){
					tmp_tree = (pANTLR3_BASE_TREE)node_tag_specific->children->get(node_tag_specific->children, i);
					tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;

					tmp_tree_2 = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, 0);
					//cout << tmp_str << " - " << tmp_tree_2 << endl;
					if(strcmp(tmp_str.c_str(), "VALUE_NODE") != 0){
						if(tmp_tree_2 != NULL){
							tmp_val = (char*)tmp_tree_2->toString(tmp_tree_2)->chars;
							fix_name(&tmp_str);
							output->append("\t\tstatic const int _");
							output->append(tmp_str);
							output->append(" = ");
							output->append(tmp_val);
							output->append(";\n");
							//cout << "get_tag_specific: " << tmp_str << endl;

						}

					}else{
						//TODO
						return;
					}
				}

			}
		}
	}

}

void get_tag_specific_enumerated(pANTLR3_BASE_TREE node_tag_specific, string* output, int sindex){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_2;
	string tmp_str;
	stringstream tmp_val;

	// NODE_INFO, subnode is ENUMERATED or TAG_DESCRIPTOR
	if(node_tag_specific->children->count > 1) sindex = 1; else sindex = 0;

	// ENUMERATED
	node_tag_specific = (pANTLR3_BASE_TREE)node_tag_specific->children->get(node_tag_specific->children, sindex);

	// ENUMERATED
	if(node_tag_specific != NULL){
		if(node_tag_specific->children != NULL){
			if(node_tag_specific->children->count > 0){
				// child count
				int n = node_tag_specific->children->size(node_tag_specific->children);
				for(int i = 0; i<n; i++){
					tmp_tree = (pANTLR3_BASE_TREE)node_tag_specific->children->get(node_tag_specific->children, i);
					tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;

					tmp_val.str("");
					if(tmp_tree->children != NULL){
					    if(tmp_tree->children->count > 0){
						tmp_tree_2 = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, 0);
						tmp_val << (char*)tmp_tree_2->toString(tmp_tree_2)->chars;

					    }else tmp_val << i;

					}else tmp_val << i;

					//cout << tmp_str << " - " << tmp_tree_2 << endl;
					//if(strcmp(tmp_str.c_str(), "VALUE_NODE") != 0){
						//if(tmp_tree != NULL){
							//tmp_val = (char*)tmp_tree_2->toString(tmp_tree_2)->chars;
							fix_name(&tmp_str);
							output->append("\t\tstatic const int _");
							output->append(tmp_str);
							output->append(" = ");
							output->append(tmp_val.str());
							output->append(";\n");
							//cout << "get_tag_specific: " << tmp_str << endl;

						//}

					//}else{
						//TODO
					//	return;
					//}
				}

			}
		}
	}

}



pANTLR3_BASE_TREE find_node_info(pANTLR3_BASE_TREE node){
	if(node->children != NULL){
		//cout << "find_node_info: " << node->toString(node)->chars << endl;
		pANTLR3_BASE_TREE tmp_tree;
		// child count
		int n = node->children->size(node->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)node->children->get(node->children, i);
			if(strcmp((char*)tmp_tree->toString(tmp_tree)->chars, "NODE_INFO") == 0){
				//cout << "find_node_info child: " << tmp_tree->toString(tmp_tree)->chars << endl;
				//cout << endl;
				return tmp_tree;
			}
			return find_node_info(tmp_tree);
		}
	}
	return NULL;
}

pANTLR3_BASE_TREE find_tag_specific(pANTLR3_BASE_TREE node){
	if(node->children != NULL){
		//cout << "find_tag_specific: " << node->toString(node)->chars << endl;
		pANTLR3_BASE_TREE tmp_tree;
		// child count
		int n = node->children->size(node->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)node->children->get(node->children, i);
			if(strcmp((char*)tmp_tree->toString(tmp_tree)->chars, "TAG_SPECIFIC") == 0){
				//cout << "find_node_info child: " << tmp_tree->toString(tmp_tree)->chars << endl;
				//cout << endl;
				//cout << tmp_tree->toString(tmp_tree)->chars << endl;
				return tmp_tree;
			}
			return find_tag_specific(tmp_tree);
		}
	}
	return NULL;
}

void process_sequence_of(pANTLR3_BASE_TREE node_info_node, string* output, string* output_hdr_fwd, string* output_src, string* current_class){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_child;
	pANTLR3_BASE_TREE tmp_tree_child2;
	pANTLR3_BASE_TREE tmp_tree_child3;
	string tmp_str;
	char tmp_type[500];
	int tmp_size;
	int idx;
	string tmp_fnd;

	if(node_info_node->children != NULL){
		// child count
		int n = node_info_node->children->size(node_info_node->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
			tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
			// SEQUENCE or CHOICE
			if(strcmp(tmp_str.c_str(), "SEQUENCE_OF") == 0){
				int n2 = tmp_tree->children->size(tmp_tree->children);
				for(int j = 0; j<n2; j++){
					tmp_tree_child = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, j);
					tmp_str = (char*)tmp_tree_child->toString(tmp_tree_child)->chars;
					if(strcmp(tmp_str.c_str(), "TAG_SPECIFIC") == 0){
						int n3 = tmp_tree_child->children->size(tmp_tree_child->children);
						for(int k = 0; k<n3; k++){
							tmp_tree_child2 = (pANTLR3_BASE_TREE)tmp_tree_child->children->get(tmp_tree_child->children, k);
							tmp_str = (char*)tmp_tree_child2->toString(tmp_tree_child2)->chars;
							if(strcmp(tmp_str.c_str(), "NODE_INFO") == 0){
								tmp_tree_child3 = (pANTLR3_BASE_TREE)tmp_tree_child2->children->get(tmp_tree_child2->children, 0);
								tmp_str = (char*)tmp_tree_child3->toString(tmp_tree_child3)->chars;
								// check inner
								get_type(output, output_hdr_fwd, output_src, tmp_tree_child2, tmp_type, &tmp_size, true, current_class, &tmp_str, true, explicit_tagging, false);

								tmp_str = "";
								tmp_str.append(tmp_type);

								get_builtin_name(&tmp_str);
								fix_name(&tmp_str);
								output->append("\t\t// nodes\n");
								output->append("\t\t");
								output->append(tmp_str);
								output->append("* ");
								output->append("get_child(unsigned int child_index);\n");
								output->append("\t\tvoid set_child(unsigned int child_index);\n");
								output->append("\t\tASN1Node* create_node(unsigned int _index);\n");
								output->append("\t\tASN1Node* get_next_node(unsigned int _index);\n");


								//// get tag descriptor
								//get_tag_descriptor_src(tmp_node_info, &tmp_fnd, tmp_member, false);
								tmp_fnd = "\t@@CNSTR_";
								tmp_fnd.append(*current_class);
								tmp_fnd.append("@@");
								idx = output_src->find(tmp_fnd);

								// constructor
								if(idx > -1){
									tmp_fnd = "";
									//tmp_fnd.append("\n\t");
									//tmp_fnd.append("for(int i = 0; i<20; i++) children.push_back(NULL);\n");

									//tmp_fnd.append("for(int i = 0; i<20; i++) children.push_back(new ");
									//tmp_fnd.append(tmp_str);
									//tmp_fnd.append("());\n");
									output_src->insert(idx, tmp_fnd);

								}


								// destrucctor
								tmp_fnd = "\t@@DSTR_";
								tmp_fnd.append(*current_class);
								tmp_fnd.append("@@");
								idx = output_src->find(tmp_fnd);
								if(idx > -1){
									tmp_fnd = "";

									// comment out childreb.clear
									//output_src->insert(idx - 18, "// ");

									// destructor
									/*
									idx = output_src->find(tmp_fnd);
									tmp_fnd = "";
									tmp_fnd.append("\tASN1Node* tmp_node = NULL;\n");
									tmp_fnd.append("\tint i = 0;\n");
									tmp_fnd.append("\t// free buffer nodes\n");
									tmp_fnd.append("\twhile(i < 20) {\n");
									tmp_fnd.append("\t\ttmp_node = children[0];\n");
									tmp_fnd.append("\t\tif(tmp_node != NULL) {\n");
									tmp_fnd.append("\t\t\tchildren.erase(children.begin());\n");
									tmp_fnd.append("\t\t\tdelete tmp_node;\n");
									tmp_fnd.append("\t\t}\n");
									tmp_fnd.append("\t\ti++;\n");
									tmp_fnd.append("\t}\n");
									*/

									// get_child method
									tmp_fnd.append("}\n\n");
									tmp_fnd.append("asn1::");
									tmp_fnd.append(tmp_str);
									tmp_fnd.append("* asn1::");
									tmp_fnd.append(*current_class);
									tmp_fnd.append("::get_child(unsigned int child_index){\n");
									tmp_fnd.append("\tif(child_index < children.size()) return (");
									tmp_fnd.append(tmp_str);
									tmp_fnd.append("*)children[child_index]; else return NULL;\n");


									// set_child method
									tmp_fnd.append("}\n\n");
									tmp_fnd.append("void asn1::");
									tmp_fnd.append(*current_class);
									tmp_fnd.append("::set_child(unsigned int child_index){\n");
									tmp_fnd.append("\tif(child_index < children.size()){\n");
									tmp_fnd.append("\t\t");
									tmp_fnd.append("if(children[child_index] == NULL) children[child_index] = create_node(child_index);\n");
									tmp_fnd.append("\t}else get_next_node(child_index);\n");

									// create_node method
									tmp_fnd.append("}\n\n");
									tmp_fnd.append("asn1::ASN1Node* asn1::");
									tmp_fnd.append(*current_class);
									tmp_fnd.append("::create_node(unsigned int _index){\n");
									tmp_fnd.append("\tchildren[_index] = new ");
									tmp_fnd.append(tmp_str);
									tmp_fnd.append("();\n");
									tmp_fnd.append("\treturn children[_index];\n");


									// get_next_node method
									tmp_fnd.append("}\n\n");
									tmp_fnd.append("asn1::ASN1Node* asn1::");
									tmp_fnd.append(*current_class);
									tmp_fnd.append("::get_next_node(unsigned int _index){\n");
									tmp_fnd.append("\tif(_index < children.size()) return children[_index]; else{\n");
									tmp_fnd.append("\t\tchildren.push_back(new ");
									tmp_fnd.append(tmp_str);
									tmp_fnd.append("());\n");
									tmp_fnd.append("\t\treturn children[children.size() - 1];\n");
									tmp_fnd.append("\t}\n");

									// add to output
									output_src->insert(idx, tmp_fnd);
									/*
									asn1::Component* asn1::ComponentPortion::get_child(unsigned int child_index){
										if(child_index < children.size()) return (Component*)children[child_index]; else return NULL;
									}									 *
									 *
									ASN1Node* tmp_node = NULL;
									int i = 0;
									// free 20 buffer nodes
									while(i < 20){
										tmp_node = children[0];
										if(tmp_node != NULL) {
											children.erase(children.begin());
											delete tmp_node;
										}
										i++;
									}
									 */

								}
								return;


							}

						}


					}
				}
			}
		}
		// remove position markers
		tmp_fnd = "\t@@CNSTR_";
		tmp_fnd.append(*current_class);
		tmp_fnd.append("@@");
		idx = output_src->find(tmp_fnd);
		if(idx > -1) output_src->replace(idx, tmp_fnd.size(), "");

		tmp_fnd = "\t@@DSTR_";
		tmp_fnd.append(*current_class);
		tmp_fnd.append("@@");
		idx = output_src->find(tmp_fnd);
		if(idx > -1) output_src->replace(idx, tmp_fnd.size(), "");


	}


}


void process_sequence_choice(pANTLR3_BASE_TREE node_info_node, string* output, string* output_hdr_fwd, string* output_src, string* current_class, string* prefix){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_tree_child;
	pANTLR3_BASE_TREE tmp_node_info;
	string tmp_str;
	char tmp_member[500];
	char tmp_type[500];
	int tmp_size;
	int idx, idx_tmp;
	string tmp_fnd;
	string tmp_fnd2;
	string tmp_optional;
	stringstream convert;

	if(node_info_node->children != NULL){
		// child count
		int n = node_info_node->children->size(node_info_node->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)node_info_node->children->get(node_info_node->children, i);
			tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
			// SEQUENCE, CHOICE
			if(strcmp(tmp_str.c_str(), "SEQUENCE") == 0 || strcmp(tmp_str.c_str(), "CHOICE") == 0){

				// optional
				if(strcmp(tmp_str.c_str(), "SEQUENCE") == 0) check_optional(tmp_tree, output, output_src, current_class);



				output->append("\t\t// nodes\n");
				int n2 = tmp_tree->children->size(tmp_tree->children);
				for(int j = 0; j<n2; j++){
					tmp_tree_child = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, j);
					tmp_str = (char*)tmp_tree_child->toString(tmp_tree_child)->chars;



					get_member(tmp_tree_child, tmp_member, &tmp_size);
					fix_name(tmp_member, tmp_size);

					output->append("\t\t");
					tmp_node_info = find_node_info(tmp_tree_child);
					if(tmp_node_info != NULL){
						string tmp_member_str(tmp_member);



						get_type(output, output_hdr_fwd, output_src, tmp_node_info, tmp_type, &tmp_size, true, current_class, &tmp_member_str, true, is_explicit(tmp_node_info), true);
						fix_name(tmp_type, tmp_size);

						// header
						output->append(tmp_type);
						output->append("* _");
						output->append(tmp_member);
						output->append(";\n");

						// src
						tmp_fnd = "\t@@CNSTR_";
						tmp_fnd.append(*current_class);
						tmp_fnd.append("@@");
						idx = output_src->find(tmp_fnd);

						// constructor
						if(idx > -1){
							//cout << "OK: " << tmp_fnd << " - " << *current_class << " - " << tmp_type << " - " << tmp_member << endl;



							//cout << "IS_OPTIONAL: " << is_optional(tmp_tree, tmp_member) << endl;
							tmp_fnd = "\t// ";
							tmp_fnd.append(tmp_member);
							tmp_fnd.append("\n");
							tmp_fnd.append("\t_");
							tmp_fnd.append(tmp_member);
							tmp_fnd.append(" = NULL;\n");

							// mandatory
							//fix_name()
							if(!is_optional(tmp_tree, tmp_member)){
								tmp_fnd.append("\t_");
								tmp_fnd.append(tmp_member);
								tmp_fnd.append(" = new ");
								tmp_fnd.append(tmp_type);
								tmp_fnd.append("();\n");
								// get tag descriptor
								get_tag_descriptor_src(tmp_node_info, &tmp_fnd, tmp_member, false, false, NULL, (char*)"\t");

							// optional
							}else{
								//cout << "OPTIONAL: " << tmp_member << endl;
								// replace output from check_optional, insert types
								tmp_fnd2 = "@PARAM_OPTIONAL@";
								idx_tmp = output_src->find(tmp_fnd2);
								if(idx_tmp > -1) output_src->replace(idx_tmp, tmp_fnd2.size(), tmp_type);


								char tmp_member2[500];
								strcpy(tmp_member2, tmp_member);
								// create
								convert.str("");
								convert << j;
								tmp_optional.append("\t\t");
								tmp_optional.append("case " + convert.str() + ":\n");
								tmp_optional.append("\t\t{\n");
								tmp_optional.append("\t\t\t_");
								tmp_optional.append(tmp_member);
								tmp_optional.append(" = new ");
								tmp_optional.append(tmp_type);
								tmp_optional.append("();\n");

								get_tag_descriptor_src(tmp_node_info, &tmp_optional, tmp_member2, false, false, NULL, (char*)"\t\t\t");

								tmp_optional.append("\t\t\tchildren[" + convert.str() + "] = _" + tmp_member2 + ";\n");
								tmp_optional.append("\t\t\treturn _");
								tmp_optional.append(tmp_member);
								tmp_optional.append(";\n");
								tmp_optional.append("\t\t}\n");


								//cout << "======== " << tmp_member << " =======" << endl;
								//cout << tmp_optional << endl;
								//cout << "=============" << endl;

								/*
								tmp_fnd2 = "\t\t@_OPTIONAL_@";
								idx = output_src->find(tmp_fnd2);
								output_src->insert(idx, tmp_optional);


								*/
							}


							if(prefix->size() > 0){
								//tmp_fnd.append("\tchildren.push_back(");
								//tmp_fnd.append(*prefix);
								//tmp_fnd.append(");\n");

								tmp_fnd.append("\t");
								tmp_fnd.append(*prefix);
								tmp_fnd.append("->children.push_back(_");
								tmp_fnd.append(tmp_member);
								tmp_fnd.append(");\n\n");


							}else{
								tmp_fnd.append("\tchildren.push_back(_");
								tmp_fnd.append(tmp_member);
								tmp_fnd.append(");\n\n");

							}


							output_src->insert(idx, tmp_fnd);



						}else{
							cout << "CONSTRUCTOR ERR: " << tmp_fnd << " - " << *current_class << " - " << tmp_type << " - " << tmp_member << endl;
						}

						/*
						// destrucctor
						tmp_fnd = "\t@@DSTR_";
						tmp_fnd.append(*current_class);
						tmp_fnd.append("@@");
						idx = output_src->find(tmp_fnd);
						if(idx > -1){
							//cout << "OK: " << tmp_fnd << " - " << *current_class << " - " << tmp_type << " - " << tmp_member << endl;

							tmp_fnd = "\t";
							tmp_fnd.append("delete _");
							tmp_fnd.append(tmp_member);
							tmp_fnd.append(";\n");


							output_src->insert(idx, tmp_fnd);

						}else{
							cout << "DESTRUCTOR ERR: " << tmp_fnd << " - " << *current_class << " - " << tmp_type << " - " << tmp_member << endl;
						}
						*/


					}


				}
				tmp_fnd2 = "\t\t@_OPTIONAL_@";
				idx = output_src->find(tmp_fnd2);
				if(idx > -1) output_src->insert(idx, tmp_optional);

				idx = output_src->find(tmp_fnd2);
				if(idx > -1) output_src->replace(idx, tmp_fnd2.size(), "");

				/*
				// destrucctor prefix
				tmp_fnd = "\t@@DSTR_";
				tmp_fnd.append(*current_class);
				tmp_fnd.append("@@");
				idx = output_src->find(tmp_fnd);

				if(prefix->size() > 0){
					//tmp_fnd = "";
					tmp_fnd.append("\tdelete ");
					tmp_fnd.append(*prefix);
					tmp_fnd.append(";\n");
					if(idx > -1){
						output_src->insert(idx, tmp_fnd);

					}
				}
				*/

			}

		}
		// remove position markers
		tmp_fnd = "\t@@CNSTR_";
		tmp_fnd.append(*current_class);
		tmp_fnd.append("@@");
		idx = output_src->find(tmp_fnd);
		if(idx > -1) output_src->replace(idx, tmp_fnd.size(), "");

		tmp_fnd = "\t@@DSTR_";
		tmp_fnd.append(*current_class);
		tmp_fnd.append("@@");
		idx = output_src->find(tmp_fnd);
		if(idx > -1) output_src->replace(idx, tmp_fnd.size(), "");



	}
}
void process_choice(pANTLR3_BASE_TREE node_info_node){
	// TODO
}

void get_types(pANTLR3_BASE_TREE tree, string* dest_hdr, string* dest_hdr_fwd, string* dest_src){
	pANTLR3_BASE_TREE tmp_tree;
	pANTLR3_BASE_TREE tmp_node_info;
	string tmp_str;
	char tmp_type[500];
	int tmp_size;
	if(tree->children != NULL){
		// child count
		int n = tree->children->size(tree->children);
		// fwd declarations
		dest_hdr->append("\t@@FWD_DECL@@\n");
		dest_hdr_fwd->append("// forward declarations\n");
		// for out of order classes
		dest_hdr->append("\t@@AUTO_CLS@@\n");

		// fwd declarations
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)tree->children->get(tree->children, i);
			tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
			replace( tmp_str.begin(), tmp_str.end(), '-', '_' );
			dest_hdr_fwd->append("\tclass ");
			dest_hdr_fwd->append(tmp_str);
			dest_hdr_fwd->append(";\n");

		}
		dest_hdr->append("\n");

		// classes
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)tree->children->get(tree->children, i);
			tmp_node_info = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, 0);

			// get type
			tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
			replace(tmp_str.begin(), tmp_str.end(), '-', '_' );
			get_type(dest_hdr, dest_hdr_fwd, dest_src, tmp_node_info, tmp_type, &tmp_size, false, &tmp_str, NULL, false, explicit_tagging, false);


			/*

			tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;
			replace( tmp_str.begin(), tmp_str.end(), '-', '_' );

			//cout << tmp_tree->toString(tmp_tree)->chars << endl;
			dest_hdr->append("\t// ");
			dest_hdr->append(tmp_str);
			dest_hdr->append("\n");
			dest_hdr->append("\tclass ");
			dest_hdr->append(tmp_str);
			//dest_hdr->append(" : public ASN1Node {\n");
			dest_hdr->append(" : public ");
			dest_hdr->append(tmp_type);
			dest_hdr->append(" {\n");
			dest_hdr->append("\tpublic:\n");
			dest_hdr->append("\t\t");
			dest_hdr->append(tmp_str);
			dest_hdr->append("();\n");
			dest_hdr->append("\t\t~");
			dest_hdr->append(tmp_str);
			dest_hdr->append("();\n");

			// SEQUENCE
			if(strcmp(tmp_type, "Sequence") == 0){
				process_sequence_choice(tmp_node_info, dest_hdr, &tmp_str);
			// CHOICE
			}else if(strcmp(tmp_type, "Choice") == 0){
				process_sequence_choice(tmp_node_info, dest_hdr, &tmp_str);

			// INTEGER
			}else if(strcmp(tmp_type, "Integer") == 0){
				get_tag_specific_integer(find_tag_specific(tmp_node_info), dest_hdr);
			}

			// get tag descriptor
			get_tag_descriptor_header(tmp_node_info, dest_hdr, tmp_type);


			dest_hdr->append("\n\t};\n\n");
			*/

		}
	}

}

bool use_explicit_tagging(pANTLR3_BASE_TREE definition_node_tree){
	pANTLR3_BASE_TREE tmp_tree;
	if(definition_node_tree->children != NULL){
		// child count
		int n = definition_node_tree->children->size(definition_node_tree->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)definition_node_tree->children->get(definition_node_tree->children, i);
			//cout << tmp_tree->toString(tmp_tree)->chars << endl;
			if(strcmp((char*)tmp_tree->toString(tmp_tree)->chars, "IMPLICIT") == 0) return false;
			else if(strcmp((char*)tmp_tree->toString(tmp_tree)->chars, "EXPLICIT") == 0) return true;
		}
	}


	return true;
}

const char* get_bultin_type(pANTLR3_BASE_TREE node, const char* user_type){
    pANTLR3_BASE_TREE tmp_tree;
    pANTLR3_BASE_TREE tmp_tree2;
    pANTLR3_BASE_TREE tmp_tree3;
    string tmp_str;
    string tmp_str2;
    string tmp_str3;
    const char* res = NULL;

    if(node->children != NULL){
	// child count
	int n = node->children->size(node->children);
	for(int i = 0; i<n; i++){
	    tmp_tree = (pANTLR3_BASE_TREE)node->children->get(node->children, i);
	    tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;

	    // name found, check definition
	    if(strcmp(tmp_str.c_str(), user_type) == 0){
		if(tmp_tree->children != NULL){
		    if(tmp_tree->children->count > 0){
			    tmp_tree2 = (pANTLR3_BASE_TREE)tmp_tree->children->get(tmp_tree->children, 0);
			    tmp_str2 = (char*)tmp_tree2->toString(tmp_tree2)->chars;
			    // look for NODE_INFO
			    if(strcmp(tmp_str2.c_str(), "NODE_INFO") == 0){

				// look through NODE_INFO
				int n2 = tmp_tree2->children->count;
				for(int j = 0; j<n2; j++){
				    tmp_tree3 = (pANTLR3_BASE_TREE)tmp_tree2->children->get(tmp_tree2->children, j);
				    tmp_str3 = (char*)tmp_tree3->toString(tmp_tree3)->chars;

					if(	strcmp(tmp_str3.c_str(), "SEQUENCE") == 0 ||
						strcmp(tmp_str3.c_str(), "INTEGER") == 0 ||
						strcmp(tmp_str3.c_str(), "CHOICE") == 0 ||
						strcmp(tmp_str3.c_str(), "BOOLEAN") == 0 ||
						strcmp(tmp_str3.c_str(), "BIT_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "OCTET_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "NULL") == 0 ||
						strcmp(tmp_str3.c_str(), "OBJECT_IDENTIFIER") == 0 ||
						strcmp(tmp_str3.c_str(), "OBJECT_DESCRIPTOR") == 0 ||
						strcmp(tmp_str3.c_str(), "EXTERNAL") == 0 ||
						strcmp(tmp_str3.c_str(), "REAL") == 0 ||
						strcmp(tmp_str3.c_str(), "ENUMERATED") == 0 ||
						strcmp(tmp_str3.c_str(), "EMBEDDED_PDV") == 0 ||
						strcmp(tmp_str3.c_str(), "UTF8STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "RELATIVE_OID") == 0 ||
						strcmp(tmp_str3.c_str(), "SEQUENCE_OF") == 0 ||
						strcmp(tmp_str3.c_str(), "SET") == 0 ||
						strcmp(tmp_str3.c_str(), "NUMERIC_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "PRINTABLE_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "T61STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "VIDEOTEX_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "IA5_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "UTC_TIME") == 0 ||
						strcmp(tmp_str3.c_str(), "GENERALIZED_TIME") == 0 ||
						strcmp(tmp_str3.c_str(), "GRAPHIC_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "VISIBLE_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "GENERAL_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "UNIVERSAL_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "CHARACTER_STRING") == 0 ||
						strcmp(tmp_str3.c_str(), "BMP_STRING") == 0){

					    return tmp_str3.c_str();

					}



				}

			    }

		    }

		}
		    //tmp_tree2 = (pANTLR3_BASE_TREE)tmp_tree->get(node->children, i);
		    //tmp_str = (char*)tmp_tree->toString(tmp_tree)->chars;


	    }


	    // children
	    res = get_bultin_type(tmp_tree, user_type);
	    if(res != NULL) return res;


	}
	// TODO
    }
    return res;
}


pANTLR3_BASE_TREE get_body(pANTLR3_BASE_TREE definition_node_tree){
	pANTLR3_BASE_TREE tmp_tree;
	if(definition_node_tree->children != NULL){
		// child count
		int n = definition_node_tree->children->size(definition_node_tree->children);
		for(int i = 0; i<n; i++){
			tmp_tree = (pANTLR3_BASE_TREE)definition_node_tree->children->get(definition_node_tree->children, i);
			//cout << tmp_tree->toString(tmp_tree)->chars << endl;
			if(strcmp((char*)tmp_tree->toString(tmp_tree)->chars, "BODY_NODE") == 0) return tmp_tree;
		}
	}

	return NULL;
}


void print_help(){
	cout << "asn1c - pMINK ASN.1 C++ compiler" << endl;
	cout << "Copyright (c) 2012 Release14.org" << endl;
	cout << "Usage: asn1c [options]" << endl;
	cout << endl;
	cout << "Options:" << endl;
	cout << " -f\tspecify asn.1 input file" << endl;
	cout << " -p\tprint asn.1 structure" << endl;
	cout << " -g\tgenerate c++ code" << endl;
	cout << " -t\tredirect code generation to stdout" << endl;
	cout << " -c\tsyntax check asn.1 definition" << endl;
	cout << " -o\tspecify output directory" << endl;
	cout << " -?\thelp" << endl;

}

int main(int argc, char** argv){
	pANTLR3_UINT8 fName = NULL;
	pANTLR3_INPUT_STREAM input;
	pasn1Lexer lxr;
	pANTLR3_COMMON_TOKEN_STREAM tstream;
	pasn1Parser psr;
	asn1Parser_moduleDefinition_return langAST;
	string gen_file_base;
	ofstream hdr_file;
	ofstream src_file;

	string asn1_header;
	string asn1_header_fwd;
	string asn1_source;
	bool print = false;
	bool check = false;
	bool generate = false;

	char std_out_str[] = "stdout";
	char current_dir[] = "./";
	string out_dir(current_dir);


	int opt;


	if(argc == 1){
		print_help();
		return 1;
	}else{
		while ((opt = getopt(argc, argv, "?pgtco:f:")) != -1) {
			switch(opt){
				case '?':
					print_help();
					return 0;
				case 'f':
					fName   = (pANTLR3_UINT8)optarg;
					break;
				case 'p':
					//cout << "PRINT" << endl;
					print = true;
					break;
				case 'g':
					//cout << "GENERATE" << endl;
					generate = true;
					break;
				case 'c':
					//cout << "CHECK" << endl;
					check = true;
					break;
				case 't':
					out_dir = std_out_str;
					break;
				case 'o':
					//cout << "OUTDIR: " << optarg << endl;
					out_dir = optarg;
					break;

			}

		}

	}






	if(fName != NULL){
		string tmp((char*)fName);
		// extract filename from full path
		tmp = tmp.substr(tmp.find_last_of( '/' ) +1);
		// remove ext
		tmp = tmp.substr(0, tmp.find_last_of( '.' ));

		// src
		gen_file_base = tmp;
		asn1_source.append("#include<");
		asn1_source.append(tmp);
		asn1_source.append(".h");
		asn1_source.append(">\n#include<iostream>\n\n");

		// uppercase
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		tmp.append("_H_");
		fix_name(&tmp);

		// header
		asn1_header.append("#ifndef ");
		asn1_header.append(tmp);
		asn1_header.append("\n#define ");
		asn1_header.append(tmp);
		asn1_header.append("\n\n#include<asn1.h>\n\nnamespace asn1 {\n");



		input  = antlr3FileStreamNew(fName, 0);
		if( input == NULL ){
		        ANTLR3_FPRINTF(stderr, "ERROR: Cannot open file \"%s\"\n", (char *)fName);
	            	exit(ANTLR3_ERR_NOMEM);
		}

		lxr = asn1LexerNew(input);

	  	if( lxr == NULL ){
			ANTLR3_FPRINTF(stderr, "ERROR: Cannot create lexer!\n");
	            	exit(ANTLR3_ERR_NOMEM);
	     	}

		tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));

		if (tstream == NULL){
	        	ANTLR3_FPRINTF(stderr, "ERROR: Out of memory while trying to allocate token stream\n");
			exit(ANTLR3_ERR_NOMEM);
		}

		psr = asn1ParserNew(tstream);
	   	if (psr == NULL){
	        	ANTLR3_FPRINTF(stderr, "ERROR: Out of memory while trying to allocate parser\n");
	        	exit(ANTLR3_ERR_NOMEM);
	     	}
		langAST = psr->moduleDefinition(psr);

		int err_c = psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec);

		if(err_c > 0){
			cout << "Error in ASN.1 syntax, error count: " << err_c << endl;
			return 1;
		}else{
			if(check){
				cout << "ASN.1 syntax check completed without errors! " << endl;
				return 0;
			}
		}

		if(print){
			print_tree(langAST.tree, 0);
			return 0;
		}

		if(!generate) return 0;
		antlr3CommonTreeNodeStreamNewTree(langAST.tree, ANTLR3_SIZE_HINT);

		//cout << langAST.tree->toStringTree(langAST.tree)->chars << endl;
		//cout << "============================" << endl;



		// DEFINTION NODE
		pANTLR3_BASE_TREE tmp_t = (pANTLR3_BASE_TREE)langAST.tree->children->get(langAST.tree->children, 1);

		// get default tagging mode
		explicit_tagging = use_explicit_tagging(tmp_t);

		// BODY
		tmp_t = get_body(tmp_t);

		//return 1;
		// TAG DEFINITIONS
		tmp_t = (pANTLR3_BASE_TREE)tmp_t->children->get(tmp_t->children, 2);
		tag_def_tree = tmp_t;

		get_types(tmp_t, &asn1_header, &asn1_header_fwd, &asn1_source);
		asn1_header.append("}\n#endif");

		int indx = asn1_header.find("@@FWD_DECL@@");
		asn1_header.replace(indx, 13, "\n");
		asn1_header.insert(indx, asn1_header_fwd);
		//replace(asn1_header.begin(), asn1_header.end(), "@@FWD_DECL@@", bla);


		indx = asn1_header.find("@@AUTO_CLS@@");
		asn1_header.replace(indx, 12, "");

		if(strcmp(out_dir.c_str(), std_out_str) == 0){
			cout << "============ HEADER ================" << endl;
			cout << asn1_header << endl;
			cout << "============ SRC ===================" << endl;
			cout << asn1_source << endl;

		}else{
			string hdr_filename(out_dir + "/" + gen_file_base + ".h");
			string src_filename(out_dir + "/" + gen_file_base + ".cpp");

			cout << "Generating header file \"" << gen_file_base << ".h\"..." <<  endl;
			hdr_file.open(hdr_filename.c_str());
			hdr_file << asn1_header;
			hdr_file.close();
			cout << "Finished generating header file \"" << hdr_filename << "\"" <<  endl;

			cout << endl;

			cout << "Generating src file \"" << hdr_filename << "\"..." <<  endl;
			src_file.open(src_filename.c_str());
			src_file << asn1_source;
			src_file.close();
			cout << "Finished generating src file \"" << hdr_filename << "\"" <<  endl;
		}


	}else{
		cout << "ERROR: ASN.1 input file not specified!" << endl;
	}





}



