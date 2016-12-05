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

/**
 * @file cli.h
 * @brief CLI library
 *
 */

#ifndef CLI_H_
#define CLI_H_

#include <string>
#include <iostream>
#include <vector>
#include <signal.h>

using namespace std;

/**
 * CTRL + character macro
 */
#define CTRL(c) ((c) & 037)

namespace cli {
	/**
	 * CLI Common node type
	 */
	enum CLINodeType {
		CLI_CONST 		= 0x01,	/**< Constant node (group) */
		CLI_METHOD 		= 0x02,	/**< External plugin method */
		CLI_SCRIPT 		= 0x03,	/**< External script */
		CLI_PARAM 		= 0x04,	/**< Script or method parameter */
		CLI_UNKNOWN 		= -1	/**< Unknown */
	};

	/**
	 * CLI State
	 */
	enum CLIState {
		CLI_ST_UNKNOWN		= -1,	/**< Unknown */
		CLI_ST_EXECUTE 		= 0,	/**< Execute command mode (ENTER) */
		CLI_ST_AUTO_COMPLETE 	= 1	/**< Auto completion mode (TAB) */
	};

	/**
	 * CLI Pattern (regex)
	 */
	class CLIPattern {
	public:
		string name;			/**< Pattern name */
		string pattern;			/**< Pattern regex */
	};

	/**
	 * CLI Item
	 */
	class CLIItem {
	public:
		CLIItem();			/**< Default constructor */
		~CLIItem();			/**< Destructor */
		CLIItem* parent;		/**< Parent node */
		CLINodeType node_type;		/**< Common node type (user defined types NOT INCLUDED */
		string name;			/**< Node name */
		string script_path;		/**< External script or plugin path */
		string type;			/**< Original node type (user defined types INCLUDED) */
		string desc;			/**< Node description */
		vector<CLIItem*> children;	/**< Child nodes */
		bool is_set;			/**< Param set flag, used only by CLI_PARAM nodes */
		string param_value;		/**< Param value, used only by CLI_PARAM nodes */

	};

	/**
	 * CLI Service
	 */
	class CLIService {
	public:
		CLIService();			/**< Default constructor */
		~CLIService();			/**< Destructor */

		/**
		 * Set CLI definition source
		 * @param[in]	cli_tree	Pointer to CLI definition
		 */
		void set_cli_tree(CLIItem* cli_tree);

		/**
		 * Start CLI interface loop
		 */
		void start();

		/**
		 * Add new CLI pattern to list of active patterns
		 * @param[in]	ptrn		Pointer to pattern descriptor
		 */
		void add_pattern(CLIPattern* ptrn);

		/**
		 * Validate parameter value
		 * @param[in]	param_value	Pointer to parameter value
		 * @param[in]	param_type	Pointer to parameter type
		 * @return	True if parameter if valid or False otherwise
		 */
		bool param_valid(string* param_value, string* param_type);

		/**
		 * Set initial welcome message
		 * @param[in]	_info_msg	Pointer to message string
		 */
		void set_info_msg(const char* _info_msg);

		/**
		 * Get pattern for specific type of parameter
		 * @param[in]	type		Pointer to parameter type string
		 * @return	Pointer to pattern descriptor or NULL if not found
		 */
		CLIPattern* get_pattern(string* type);

		/**
		 * Print formatted CLI definition tree
		 * @param[in]	tree		Pointer to CLI definition
		 * @param[in]	depth		Initial depth value, should be 0
		 */
		static void print_cli_tree(cli::CLIItem* tree, int depth);

		/**
		 * Generate prompt string
		 */
		void generate_prompt();

		/**
		 * Add current line to history list
		 * @param[in]		_line		Pointer to string containing command line data
		 */
		void add_to_history(string* _line);

		/**
		 * Get size of history list
		 * @return		Size of history list
		 */
		int get_historu_size();

		/**
		 * Get current line
		 * @return		_line		Pointer to current line string
		 */
		std::string* get_current_line();

		/**
		 * Clear current line
		 */
		void clear_curent_line();

		/**
		 * Get current prompt
		 * @return		_line		Pointer to current prompt string
		 */
		std::string* get_prompt();

		/**
		 * Generate path string
		 * @param[in]		def		Pointer to CLI definition of node currently located in
		 * @param[out]		result		Pointer to result string
		 */
		void generate_path(CLIItem* def, std::string* result);

		/**
		 * Get current path string string
		 * @return		Pointer to current path string
		 */
		std::string* get_current_path_line();

		/**
		 * Get CLI service id
		 * @return		Pointer to current id string
		 */
		std::string* get_id();

		/**
		 * Set CLI service id
		 * @param[in]		_id		Pointer to new id string
		 */
		void set_id(std::string* _id);

		/**
		 * Signal handler (CTRL + C)
		 * @param[in]		signum		Signal code
		 */
		static void signal_handler(int signum);

		/**
		 * Get current path
		 * @return		Pointer to current path
		 */
		cli::CLIItem* get_current_path();


		bool toggle_interrupt();
		bool get_interrupt();
		void set_interrupt(bool _val);
		bool* get_interrupt_p();

		unsigned int max_history;	/**< Maximum number of history lines */
		unsigned int history_index;	/**< Current history index */
		CLIState state;			/**< Current CLI state */
		string current_line;		/**< Current use input */
		char** cmdl_argv;		/**< Command line arguments */
		int cmdl_argc;			/**< Command line argument count */
		bool interrupt;			/**< SIGINT flag */
		int buff_ch;			/**< Buffered/interrupted character */

		/**
		 * Current CLI Service pointer
		 */
		static CLIService* CURRENT_CLI_SERVICE;

	private:
		/**
		 * Initialize colors
		 */
		void init_colors();

		/**
		 * Print CLI definition help context
		 * @param[in]	def		Pointer to CLI definition
		 * @param[in]	level		Initial level (usually 0)
		 * @param[in]	max_levels	Maximum level (usually 1)
		 *
		 */
		void print_cli_def(CLIItem* def, int level, int max_levels);

		/**
		 * Auto complete user input, generate help context data
		 * @param[in]		def		Pointer to CLI definition
		 * @param[in,out]	line		Pointer to tokenized user input
		 * @param[in]		line_size	Number of tokens contained in user input
		 * @param[out]		result		Pointer to CLI output result for help context data
		 * @param[out]		result_size	Number of perfectly matched tokens
		 * @param[out]		last_found	Pointer to last perfectly matched CLI node
		 */
		void cli_auto_complete(CLIItem* def, string* line, int line_size, CLIItem* result, int* result_size, CLIItem** last_found);

		/**
		 * Search CLI definition for partially matched data
		 * @param[in]		def		Pointer to CLI definition
		 * @param[in]		current_level	Initial level (usually 0)
		 * @param[in]		target_level	Target level (usually 0)
		 * @param[in]		target		Pointer to string with data to search for
		 * @param[out]		result		Pointer to CLI output result for matched data
		 */
		void search_cli_def(CLIItem* def, int current_level, int target_level, string* target, CLIItem* result);

		// void* here, trying to localize antlr specific refs and includes to antlr_utils only
		void* pmink_parser;			/**< Pointer to CLI config parser */
		CLIItem* cli_def;			/**< Pointer to main CLI definition */
		CLIItem* current_path;			/**< Pointer to CLI definition of node currently located in */
		string current_path_line;		/**< Current path string (pwd) */
		string prompt;				/**< Prompt string */
		string info_msg;			/**< Initial welcome message */
		vector<string*> history;		/**< History list */
		vector<CLIPattern*> patterns;		/**< Pattern list */
		bool external_handler;			/**< External handler flag (block plugin mode) */
		void* external_plugin;			/**< Pointer returned by plugin init method */
		void* external_plugin_handle;		/**< External plugin handle */
		std::string cli_id;			/**< CLI service id */

	};



}


#endif /* CLI_H_ */
