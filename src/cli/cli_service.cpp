/*
 * This file is part of Project MINK <http://www.release14.org>.
 *
 * Copyright (C) 2012 Release14 Ltd.
 * http://www.release14.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <antlr_utils.h>
#include <pmink_utils.h>
#include <stdio.h>
#include <signal.h>
#include <boost/regex.hpp>
#include <r14p.h>
using namespace std;




void print_help(){
	cout << "cli_service - pMINK CLI service" << endl;
	cout << "Copyright (c) 2012 Release14.org" << endl;
	cout << endl;
	cout << "Options:" << endl;
	cout << " -i\tunique service id" << endl;
	cout << " -f\tcli definition file" << endl;
	cout << " -c\tconfig daemon address (ipv4:port)" << endl;

}


int main(int argc, char** argv){
	int opt;
	pANTLR3_UINT8 fName = NULL;
	pANTLR3_INPUT_STREAM input;
	ppminkLexer lxr;
	pANTLR3_COMMON_TOKEN_STREAM tstream;
	ppminkParser psr;
	pminkParser_input_return ast;
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");
	std::string cli_id;
	// argc check
	if(argc < 7){
		print_help();
		return 1;
	}else{
		while ((opt = getopt(argc, argv, "f:c:i:")) != -1) {
			switch(opt){
				// service id
				case 'i':
					cli_id.append(optarg);
					if(cli_id.size() > 10){
					    std::cout << "ERROR: Maximum size of service id string is 10 characters!" << std::endl;
					    exit(EXIT_FAILURE);

					}

					break;

				// cli definition
				case 'f':
					fName   = (pANTLR3_UINT8)optarg;
					break;
				// config daemon address
				case 'c':
					// check pattern (ipv4:port)
					// check if valid
					if(!boost::regex_match(optarg, addr_regex)){
						std::cout << "ERROR: Invalid daemon address format '" << optarg << "'!" << std::endl;
						exit(EXIT_FAILURE);

					}
					break;
			}
		}
	}
	if(cli_id == ""){
		std::cout << "ERROR: Service id not defined!" << std::endl;
		exit(EXIT_FAILURE);

	}


	// definition check
	if(fName != NULL){
		// input
		input  = antlr3FileStreamNew(fName, 0);
		if(input == NULL){
			ANTLR3_FPRINTF(stderr, "ERROR: Cannot open file \"%s\"\n", (char *)fName);
			exit(ANTLR3_ERR_NOMEM);
		}
		// lexer
		lxr = pminkLexerNew(input);

	  	if(lxr == NULL){
			ANTLR3_FPRINTF(stderr, "ERROR: Cannot create lexer!\n");
			exit(ANTLR3_ERR_NOMEM);
		}

	  	// token stream
		tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));
		if(tstream == NULL){
			ANTLR3_FPRINTF(stderr, "ERROR: Cannot allocate token stream!\n");
			exit(ANTLR3_ERR_NOMEM);
		}
		// parser
		psr = pminkParserNew(tstream);

		if(psr == NULL){
			ANTLR3_FPRINTF(stderr, "ERROR: Cannot create parser!\n");
			exit(ANTLR3_ERR_NOMEM);
		}

		// ast
		ast = psr->input(psr);

		// err check
		int err_c = lxr->pLexer->rec->getNumberOfSyntaxErrors(lxr->pLexer->rec);
		err_c += psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec);

		if(err_c > 0){
			ANTLR3_FPRINTF(stderr, "ERROR: Invalid cli definition!\n");
			exit(ANTLR3_ERR_NOMEM);

		}

		// process cli ast, generate cli definition
		cli::CLIItem cli_tree;
		antlr::cli_process_config(ast.tree, &cli_tree);

		// set cli definition
		cli::CLIService cli_service;
		cli_service.cmdl_argc = argc;
		cli_service.cmdl_argv = argv;
		cli_service.set_cli_tree(&cli_tree);
		// get cli patterns
		antlr::cli_process_patterns(ast.tree, &cli_service);
		// custom ctrl + c
		signal(SIGINT, &cli::CLIService::signal_handler);
		// start cli service
		cli_service.start();
		// free antlr memory
		// stream
		tstream->free(tstream);
		tstream = NULL;
		// input
		input->close(input);
		input = NULL;
		// lexer
		lxr->free(lxr);
		lxr = NULL;
		// parser
		psr->free(psr);
		psr = NULL;



	}

	return 0;
}


