#include <iostream>
#include <string>
#include <antlr3.h>
#include "asn1Lexer.h"
#include "asn1Parser.h"
using namespace std;



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

int main(int argc, char *argv[]){
	pANTLR3_UINT8 fName;
	pANTLR3_INPUT_STREAM input;
	pasn1Lexer lxr;
	pANTLR3_COMMON_TOKEN_STREAM tstream;
	pasn1Parser psr;
	asn1Parser_moduleDefinition_return langAST;
	pANTLR3_COMMON_TREE_NODE_STREAM nodes;
	

	if(argc < 2 || argv[1] == NULL){
        	fName   = (pANTLR3_UINT8)"./input";
    	}else{
        	fName   = (pANTLR3_UINT8)argv[1];
    	}
	

	input  = antlr3FileStreamNew(fName, 0);
	if( input == NULL ){
	        ANTLR3_FPRINTF(stderr, "Unable to open file %s due to malloc() failure1\n", (char *)fName);
            	exit(ANTLR3_ERR_NOMEM);
	}
	
	lxr = asn1LexerNew(input);

  	if( lxr == NULL ){
		ANTLR3_FPRINTF(stderr, "Unable to create the lexer due to malloc() failure1\n");
            	exit(ANTLR3_ERR_NOMEM);
     	}

	tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));

	if (tstream == NULL){
        	ANTLR3_FPRINTF(stderr, "Out of memory trying to allocate token stream\n");
		exit(ANTLR3_ERR_NOMEM);
	}

	psr = asn1ParserNew(tstream);
   	if (psr == NULL){
        	ANTLR3_FPRINTF(stderr, "Out of memory trying to allocate parser\n");
        	exit(ANTLR3_ERR_NOMEM);
     	}
	langAST = psr->moduleDefinition(psr);

	nodes   = antlr3CommonTreeNodeStreamNewTree(langAST.tree, ANTLR3_SIZE_HINT);
	//cout << langAST.tree->toStringTree(langAST.tree)->chars << endl;
	//cout << "============================" << endl;
	print_tree(langAST.tree, 0);
	cout << endl;
	
	//pANTLR3_BASE_TREE tmp_tree;
	//for(int i = 0; i<langAST.tree->getChildCount(langAST.tree); i++){
	//	tmp_tree = (pANTLR3_BASE_TREE)langAST.tree->getChild(langAST.tree, i);
	//	cout << tmp_tree->toString(tmp_tree)->chars << endl;
	//}


}
