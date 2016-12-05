#include <iostream>
#include <curses.h>
#include <cli.h>
#include <config.h>
#include <string.h>
#include <pmink_utils.h>
#include <antlr_utils.h>
#include <fstream>
#include <sstream>
#include <dirent.h>

class PluginInfo {
public:
    PluginInfo(){
	config = NULL;
	cli = NULL;
    }
    config::Config* config;
    cli::CLIService* cli;
};

// regular method handler
extern "C" void* method_handler(const char** arg_names, const char** arg_values, int arg_count){

    printw("hello from pmink test module method handler...\n");
    printw("arg count: %d\n", arg_count);
    for(int i = 0; i<arg_count; i++){
	printw("%s = %s\n", arg_names[i], arg_values[i]);
    }

    return NULL;

}

// block handler init
extern "C" void* block_handler_init(void** args, int argc){
    PluginInfo* plg = new PluginInfo();
    plg->cli = (cli::CLIService*)args[0];
    plg->config = new config::Config();
    cli::CLIService* cli = plg->cli;
    //printw("block_handler_init: hello from pmink test module block handler, state = [%d] ...\n", cli->state);
    //config::Config* cfg = new config::Config();

    //char tmp_buff[8192];
    int tmp_size = 0;
    antlr::PMinkParser* pmp = antlr::create_parser();

    pANTLR3_INPUT_STREAM input = pmp->input;
    ppminkLexer lxr = pmp->lexer;
    pANTLR3_COMMON_TOKEN_STREAM tstream = pmp->tstream;
    ppminkParser psr = pmp->parser;
    pminkParser_input_return ast;
    pminkParser_inputConfig_return_struct ast_cfg;
    config::ConfigItem* cfg_def = new config::ConfigItem();
    config::ConfigItem* cfg_cnt = new config::ConfigItem();

    //bzero(tmp_buff, 8192);
    // input
    tmp_size = pmink_utils::get_file_size("/secure/pmink/src/config/config.pmcfg");

    if(tmp_size > 0){
	char* tmp_buff = new char[tmp_size + 1];
	bzero(tmp_buff, tmp_size + 1);
	pmink_utils::load_file("/secure/pmink/src/config/config.pmcfg", tmp_buff, &tmp_size);

	// reset error state
	lxr->pLexer->rec->state->errorCount = 0;
	psr->pParser->rec->state->errorCount = 0;

	input->reuse(input, (unsigned char*)tmp_buff, tmp_size, (unsigned char*)"file_stream");
	// token stream
	tstream->reset(tstream);
	// ast
	ast = psr->input(psr);

	// err check
	int err_c = lxr->pLexer->rec->getNumberOfSyntaxErrors(lxr->pLexer->rec);
	err_c += psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec);
	if(err_c > 0){
		ANTLR3_FPRINTF(stderr, "ERROR: Invalid config contents!\n");
		exit(ANTLR3_ERR_NOMEM);

	}


	antlr::process_config_def(ast.tree, cfg_def);


	if(plg->config->validate_definition(cfg_def)){
		plg->config->load_definition(cfg_def);
		// config patterns
		antlr::config_process_patterns(ast.tree, plg->config);
		delete[] tmp_buff;
		antlr::free_mem(pmp);

		//plg->config->print_config_tree(&cfg_def, 0);
		// input
		tmp_size = 0;
		tmp_size = pmink_utils::get_file_size("/secure/pmink/src/config/test.pmcfg");
		if(tmp_size > 0){
			pmp = antlr::create_parser();
			input = pmp->input;
			lxr = pmp->lexer;
			tstream = pmp->tstream;
			psr = pmp->parser;

			tmp_buff = new char[tmp_size + 1];
			bzero(tmp_buff, tmp_size + 1);
			pmink_utils::load_file("/secure/pmink/src/config/test.pmcfg", tmp_buff, &tmp_size);
			// reset error state
			lxr->pLexer->rec->state->errorCount = 0;
			psr->pParser->rec->state->errorCount = 0;
			input->reuse(input, (unsigned char*)tmp_buff, tmp_size, (unsigned char*)"file_stream");
			// token stream
			tstream->reset(tstream);
			// ast
			ast_cfg = psr->inputConfig(psr);
			// err check
			int err_c = lxr->pLexer->rec->getNumberOfSyntaxErrors(lxr->pLexer->rec);
			err_c += psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec);
			if(err_c > 0){
				ANTLR3_FPRINTF(stderr, "ERROR: Invalid config contents!\n");
				exit(ANTLR3_ERR_NOMEM);

			}

			antlr::process_config(ast_cfg.tree, cfg_cnt);
			delete[] tmp_buff;
			antlr::free_mem(pmp);
			int res = plg->config->merge(plg->config->get_definition_root(), cfg_cnt, false);
			if(res != 0){
			    attron(COLOR_PAIR(1));
			    printw("ERROR: ");
			    attroff(COLOR_PAIR(1));
			    printw("Invalid configuration file!");
			}

			//plg->config->print_config_tree(plg->config->definition, 0);



		}


	}else{
	    attron(COLOR_PAIR(1));
	    printw("ERROR: ");
	    attroff(COLOR_PAIR(1));
	    printw("Invalid configuration definition!");
	    delete cfg_def;
	    delete[] tmp_buff;
	    antlr::free_mem(pmp);


	}


    }
    delete cfg_cnt;


    return plg;

}

// block handler exit
extern "C" void* block_handler_free(void** args, int argc){
    //cli::CLIService* cli = (cli::CLIService*)args;
    PluginInfo* plg = (PluginInfo*)args[0];
    config::Config* cfg = plg->config;
    if(cfg->get_definition_root() != NULL) delete cfg->get_definition_root();
    delete cfg;
    delete plg;
    printw("\n");
    //printw("block_handler_free: hello from pmink test module block handler, state = [%d] ...\n", cli->state);
    //printw("\nblock_handler_free: hello from pmink test module block handler...\n");
    return NULL;

}

// block handler line handler (ENTER/TAB)
extern "C" void* block_handler(void** args, int argc){
    PluginInfo* plg = (PluginInfo*)args[0];
    cli::CLIService* cli = plg->cli;
    config::Config* cfg = plg->config;
    config::ConfigItem tmp_cli_res;
    config::ConfigItem tmp_node_lst;
    config::ConfigItem* last_found = NULL;
    config::ConfigModeType mode = config::CONFIG_MT_UNKNOWN;
    std::string line(cli->get_current_line()->c_str());
    string tmp_lst[50];
    string tmp_err[50];
    int tmp_size = 0;
    int res_size = 0;
    int error_count = 0;

    // check for defintion error
    if(cfg->get_definition_root() == NULL) {
	printw(cli->get_prompt()->c_str());
	cli->clear_curent_line();
	return NULL;

    }
    // TAB
    if(cli->state == cli::CLI_ST_AUTO_COMPLETE){
	// parse line
	//antlr::parse_line(&line, tmp_lst, 50, &tmp_size, cfg->pmink_parser);
	pmink_utils::tokenize(&line, tmp_lst, 50, &tmp_size, true);
	//int error_count = 0;
	//last_found = NULL;
	//mode = config::CONFIG_MT_UNKNOWN;
	cfg->auto_complete(&mode, config::CONFIG_ACM_TAB, cfg->get_cmd_tree(), cfg->get_definition_wn(), tmp_lst, tmp_size, &tmp_cli_res, &res_size, &last_found, &error_count, tmp_err, false, &tmp_node_lst);
	// more
	int w, h;
	getmaxyx(stdscr, h, w);
	// get date size
	int line_c = tmp_cli_res.children.size();
	// create buffer win
	WINDOW* new_win = newwin(line_c, w, 0, 0);
	// get data
	cfg->print_cfg_def(mode == config::CONFIG_MT_SET ? true : false, true, &tmp_cli_res, 0, 1, new_win);
	// more
	pmink_utils::cli_more(line_c, new_win);
	// free
	delwin(new_win);



	// replace current line with auto completed values
	cli->clear_curent_line();
	for(int i = 0; i<tmp_size; i++){
		cli->get_current_line()->append(tmp_lst[i]);
		if(i < res_size) cli->get_current_line()->append(" "); else break;
	}

	//cfg->print_cfg_def(mode == config::CONFIG_MT_SET ? true : false, true, &tmp_cli_res, 0, 1);


	tmp_cli_res.children.clear();

	for(int i = 0; i<tmp_node_lst.children.size(); i++) delete tmp_node_lst.children[i];
	tmp_node_lst.children.clear();

	printw(cli->get_prompt()->c_str());
	printw(cli->get_current_line()->c_str());


    // ENTER
    }else if(cli->state == cli::CLI_ST_EXECUTE){

	// parse line
	//antlr::parse_line(&line, tmp_lst, 50, &tmp_size, cfg->pmink_parser);
	pmink_utils::tokenize(&line, tmp_lst, 50, &tmp_size, true);
	//int error_count = 0;
	//last_found = NULL;
	//mode = config::CONFIG_MT_UNKNOWN;
	cfg->auto_complete(&mode, config::CONFIG_ACM_ENTER, cfg->get_cmd_tree(), cfg->get_definition_wn(), tmp_lst, tmp_size, &tmp_cli_res, &res_size, &last_found, &error_count, tmp_err, false, &tmp_node_lst);

	// replace current line with auto completed values
	cli->clear_curent_line();
	for(int i = 0; i<tmp_size; i++){
		cli->get_current_line()->append(tmp_lst[i]);
		if(i < res_size) cli->get_current_line()->append(" "); else break;
	}
	// err
	if(tmp_cli_res.children.size() == 0) {
		for(int i = 0; i<error_count; i++){
			attron(COLOR_PAIR(1));
			printw("ERROR: ");
			attroff(COLOR_PAIR(1));
			printw(tmp_err[i].c_str());

		}
		//attron(COLOR_PAIR(1));
		//printw("ERROR: ");
		//attroff(COLOR_PAIR(1));
		//printw("Unknown command or parameter \"%s\"!\n", cli->get_current_line()->c_str());
	// ok
	}else{
		// delete mode
		if(mode == config::CONFIG_MT_DEL){
		    // single result
		    if(tmp_cli_res.children.size() == 1){
			// item
			if(tmp_cli_res.children[0]->node_type == config::CONFIG_NT_ITEM){
			    tmp_cli_res.children[0]->node_state = config::CONFIG_NS_DELETED;
			    //tmp_cli_res.children[0]->value = "";
			}

		    // multiple results
		    }else if(tmp_cli_res.children.size() > 0){
			    // check if parent is block item
			    if(tmp_cli_res.children[0]->parent->node_type == config::CONFIG_NT_BLOCK){
				    config:: ConfigItem* tmp_item = tmp_cli_res.children[0]->parent;
				    // only allow deletion of template based node
				    if(tmp_item->parent->children.size() > 1){
					    // check if template
					    if(tmp_item->parent->children[0]->is_template){
						    // loop all template based nodes, try to match
						    for(int i = 1; i< tmp_item->parent->children.size(); i++) if(!tmp_item->parent->children[i]->is_template){
							if(tmp_item->parent->children[i] == tmp_item){
							    //tmp_item->parent->children.erase(tmp_item->parent->children.begin() + i);
							    //delete tmp_item;
							    tmp_item->parent->children[i]->node_state = config::CONFIG_NS_DELETED;
							    break;
							}
						    }

					    }else{
						attron(COLOR_PAIR(1));
						printw("ERROR: ");
						attroff(COLOR_PAIR(1));
						printw("Cannot delete non template block mode \"%s\"!\n", tmp_cli_res.children[0]->parent->name.c_str());

					    }

				    }else{
					attron(COLOR_PAIR(1));
					printw("ERROR: ");
					attroff(COLOR_PAIR(1));
					printw("Cannot delete non template block mode \"%s\"!\n", tmp_cli_res.children[0]->parent->name.c_str());

				    }

			    }
		    }
		// special commands
		}else if(mode == config::CONFIG_MT_CMD){
		    if(tmp_cli_res.children.size() >= 1){
			// cmd without params
			if(tmp_cli_res.children[0]->node_type == config::CONFIG_NT_CMD){
				if(tmp_cli_res.children[0]->name == "configuration"){
				    int tmp_size = 0;
				    int w, h;;
				    getmaxyx(stdscr, h, w);
				    // get date size
				    int line_c = cfg->get_config_lc(cfg->get_definition_wn());
				    // create buffer win
				    WINDOW* new_win = newwin(line_c, w, 0, 0);
				    // get data
				    cfg->show_config(cfg->get_definition_wn(), 0, &tmp_size, false, new_win);
				    // more
				    pmink_utils::cli_more(line_c, new_win);
				    // free
				    delwin(new_win);

				}else if(tmp_cli_res.children[0]->name == "commands"){
				    int w, h;
				    getmaxyx(stdscr, h, w);
				    // get date size
				    int line_c = cfg->get_commands_lc(cfg->get_definition_wn());
				    // create buffer win
				    WINDOW* new_win = newwin(line_c, w, 0, 0);
				    // get data
				    cfg->show_commands(cfg->get_definition_wn(), 0, new_win);
				    // more
				    pmink_utils::cli_more(line_c, new_win);
				    // free
				    delwin(new_win);

				}else if(tmp_cli_res.children[0]->name == "top"){
				    // set current cfg path
				    cfg->set_definition_wn(cfg->get_definition_root());

				    // regenerate cli path
				    std::string* cpath = cli->get_current_path_line();
				    *cpath = "";
				    cli->generate_path(cli->get_current_path(), cpath);

				    // generate prompt
				    cli->generate_prompt();


				}else if(tmp_cli_res.children[0]->name == "up"){
				    if(cfg->get_definition_wn()->parent != NULL){
					// set current cfg path
					cfg->set_definition_wn(cfg->get_definition_wn()->parent);

					// regenerate cli path
					std::string* cpath = cli->get_current_path_line();
					*cpath = "";
					cli->generate_path(cli->get_current_path(), cpath);

					// generate cfg path
					std::string tmp;
					cfg->generate_path(cfg->get_definition_wn(), &tmp);
					cpath->append(tmp);

					// generate prompt
					cli->generate_prompt();


				    }
				}else if(tmp_cli_res.children[0]->name == "discard"){
				    // set current definition path to top level
				    cfg->set_definition_wn(cfg->get_definition_root());

				    // regenerate cli path
				    std::string* cpath = cli->get_current_path_line();
				    *cpath = "";
				    cli->generate_path(cli->get_current_path(), cpath);

				    // generate prompt
				    cli->generate_prompt();

				    cfg->discard(cfg->get_definition_root());

				}
			// cmd with params
			}else if(tmp_cli_res.children[0]->node_type == config::CONFIG_NT_PARAM){

			    if(tmp_cli_res.children[0]->parent != NULL){
				    if(tmp_cli_res.children[0]->parent->node_type == config::CONFIG_NT_CMD){
					if(tmp_cli_res.children[0]->parent->name == "commit"){
					    if(cfg->commit(cfg->get_definition_root(), true) > 0){
						    // set current definition path to top level
						    cfg->set_definition_wn(cfg->get_definition_root());

						    // regenerate cli path
						    std::string* cpath = cli->get_current_path_line();
						    *cpath = "";
						    cli->generate_path(cli->get_current_path(), cpath);

						    // generate prompt
						    cli->generate_prompt();

						    // get rollback count
						    DIR* dir;
						    dirent* ent;
						    int c = 0;
						    int rev_num = -1;
						    stringstream tmp_str;

						    dir = opendir("./commit-log");
						    // if dir
						    if(dir != NULL) {
							// get dir contents
							while ((ent = readdir(dir)) != NULL) {
							    if(strncmp(ent->d_name, ".rollback", 9) == 0) ++c;

							}
							// close dir
							closedir(dir);
						    }


						    tmp_str << "./commit-log/.rollback." << c << ".pmcfg";
						    // save rollback
						    std::ofstream ofs(tmp_str.str().c_str(), std::ios::out | std::ios::binary);
						    if(ofs.is_open()){
							    // save current config excluding uncommitted changes
							    cfg->show_config(cfg->get_definition_root(), 0, &tmp_size, false, &ofs, true, &tmp_cli_res.children[0]->new_value);
							    ofs.close();
							    // commit new config
							    cfg->commit(cfg->get_definition_root(), false);

						    }else{
							    attron(COLOR_PAIR(1));
							    printw("ERROR: ");
							    attroff(COLOR_PAIR(1));
							    printw("Cannot create rollback configuration!\n");

						    }

					    }
					    // clear value
					    tmp_cli_res.children[0]->new_value = "";

				    // rollback
				    }else if(tmp_cli_res.children[0]->parent->name == "rollback"){
					    if(tmp_cli_res.children[0]->new_value != ""){
						    DIR* dir;
						    dirent* ent;
						    int c = 0;
						    std::string tmp_path;
						    std::stringstream istr(tmp_cli_res.children[0]->new_value);
						    int rev_num = -1;
						    istr >> rev_num;
						    bool rev_found = false;
						    dirent** fnames;


						    int n = scandir("./commit-log/", &fnames, pmink_utils::_ac_rollback_revision_filter, pmink_utils::_ac_rollback_revision_sort);
						    if(n > 0){
							for(int i = 0; i<n; i++){
								if(rev_num == i){
								    rev_found = true;
								    tmp_path = "./commit-log/";
								    tmp_path.append(fnames[i]->d_name);
								}
								free(fnames[i]);

							}
							free(fnames);

							// if revision found
							if(rev_found){
							    tmp_size = 0;
							    // err check
							    tmp_size = pmink_utils::get_file_size(tmp_path.c_str());
							    if(tmp_size == 0){
								attron(COLOR_PAIR(1));
								printw("ERROR: ");
								attroff(COLOR_PAIR(1));
								printw("Cannot find rollback revision '%s'!\n", tmp_path.c_str());

							    }else{
								char* tmp_file_buff = new char[tmp_size + 1];
								bzero(tmp_file_buff, tmp_size + 1);
								pmink_utils::load_file(tmp_path.c_str(), tmp_file_buff, &tmp_size);
								printw("Loading rollback configuration...\n");

								antlr::PMinkParser* pmp = antlr::create_parser();
								pANTLR3_INPUT_STREAM input = pmp->input;
								ppminkLexer lxr = pmp->lexer;
								pANTLR3_COMMON_TOKEN_STREAM tstream = pmp->tstream;
								ppminkParser psr = pmp->parser;
								pminkParser_input_return ast;
								pminkParser_inputConfig_return_struct ast_cfg;
								config::ConfigItem* cfg_cnt = new config::ConfigItem();

								// reset error state
								lxr->pLexer->rec->state->errorCount = 0;
								psr->pParser->rec->state->errorCount = 0;
								input->reuse(input, (unsigned char*)tmp_file_buff, tmp_size, (unsigned char*)"file_stream");

								// token stream
								tstream->reset(tstream);
								// ast
								ast_cfg = psr->inputConfig(psr);
								// err check
								int err_c = lxr->pLexer->rec->getNumberOfSyntaxErrors(lxr->pLexer->rec);
								err_c += psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec);
								if(err_c > 0){
									attron(COLOR_PAIR(1));
									printw("ERROR: ");
									attroff(COLOR_PAIR(1));
									printw("Invalid rollback configuration file syntax!\n");

								}else{
									printw("Done\n");

									// set current definition path to top level
									cfg->set_definition_wn(cfg->get_definition_root());

									// regenerate cli path
									std::string* cpath = cli->get_current_path_line();
									*cpath = "";
									cli->generate_path(cli->get_current_path(), cpath);

									// generate prompt
									cli->generate_prompt();


									// get structure
									antlr::process_config(ast_cfg.tree, cfg_cnt);
									// validate
									if(plg->config->validate(plg->config->get_definition_root(), cfg_cnt)){

										// prepare for config data replacement
										plg->config->replace_prepare(plg->config->get_definition_root());
										// merge new data
										printw("Merging rollback configuration file...\n");
										int res = plg->config->merge(plg->config->get_definition_root(), cfg_cnt, true);
										// err check
										if(res != 0){
											attron(COLOR_PAIR(1));
											printw("ERROR: ");
											attroff(COLOR_PAIR(1));
											printw("Cannot merge configuration file contents!\n");
										}else{
											printw("Done\n");
											printw("Committing rollback configuration...\n");
											// commit new config
											cfg->commit(cfg->get_definition_root(), false);
											printw("Done\n");

										}


									// err
									}else{
									    attron(COLOR_PAIR(1));
									    printw("ERROR: ");
									    attroff(COLOR_PAIR(1));
									    printw("Invalid/undefined rollback configuration file contents!\n");

									}

								}
								// free mem
								delete cfg_cnt;
								delete[] tmp_file_buff;
								antlr::free_mem(pmp);

							    }


						    }else{
							    attron(COLOR_PAIR(1));
							    printw("ERROR: ");
							    attroff(COLOR_PAIR(1));
							    printw("Cannot find rollback revision '%s'!\n", tmp_cli_res.children[0]->new_value.c_str());

						    }


						    }else{
							attron(COLOR_PAIR(1));
							printw("ERROR: ");
							attroff(COLOR_PAIR(1));
							printw("Cannot find rollback information!\n");

						    }





					    }else{
						attron(COLOR_PAIR(1));
						printw("ERROR: ");
						attroff(COLOR_PAIR(1));
						printw("Rollback revision not defined!\n");

					    }

					    // clear value
					    tmp_cli_res.children[0]->new_value = "";

					// save conf
					}else if(tmp_cli_res.children[0]->parent->name == "save"){
					    if(tmp_cli_res.children[0]->new_value != ""){
						    std::ofstream ofs(tmp_cli_res.children[0]->new_value.c_str(), std::ios::out | std::ios::binary);
						    if(ofs.is_open()){
							    printw("Saving configuration to \"%s\"...\n", tmp_cli_res.children[0]->new_value.c_str());
							    cfg->show_config(cfg->get_definition_wn(), 0, &tmp_size, false, &ofs, false, NULL);
							    ofs.close();
							    printw("Done\n");

						    }else{
							attron(COLOR_PAIR(1));
							printw("ERROR: ");
							attroff(COLOR_PAIR(1));
							printw("Cannot create file \"%s\"\n", tmp_cli_res.children[0]->new_value.c_str());

						    }

						    // clear value
						    tmp_cli_res.children[0]->new_value = "";

					    }else{
						attron(COLOR_PAIR(1));
						printw("ERROR: ");
						attroff(COLOR_PAIR(1));
						printw("Filename not defined!\n");

					    }

					// load conf
					}else if(tmp_cli_res.children[0]->parent->name == "load"){
					    if(tmp_cli_res.children[0]->new_value != ""){
						    tmp_size = 0;
						    // err
						    tmp_size = pmink_utils::get_file_size(tmp_cli_res.children[0]->new_value.c_str());
						    if(tmp_size == 0){
							attron(COLOR_PAIR(1));
							printw("ERROR: ");
							attroff(COLOR_PAIR(1));
							printw("Cannot find file \"%s\"\n", tmp_cli_res.children[0]->new_value.c_str());

						    }else{
							char* tmp_file_buff = new char[tmp_size + 1];
							bzero(tmp_file_buff, tmp_size + 1);
							pmink_utils::load_file(tmp_cli_res.children[0]->new_value.c_str(), tmp_file_buff, &tmp_size);
							printw("Loading new configuration file \"%s\"...\n", tmp_cli_res.children[0]->new_value.c_str());

							antlr::PMinkParser* pmp = antlr::create_parser();
							pANTLR3_INPUT_STREAM input = pmp->input;
							ppminkLexer lxr = pmp->lexer;
							pANTLR3_COMMON_TOKEN_STREAM tstream = pmp->tstream;
							ppminkParser psr = pmp->parser;
							pminkParser_input_return ast;
							pminkParser_inputConfig_return_struct ast_cfg;
							config::ConfigItem* cfg_cnt = new config::ConfigItem();

							// reset error state
							lxr->pLexer->rec->state->errorCount = 0;
							psr->pParser->rec->state->errorCount = 0;
							input->reuse(input, (unsigned char*)tmp_file_buff, tmp_size, (unsigned char*)"file_stream");

							// token stream
							tstream->reset(tstream);
							// ast
							ast_cfg = psr->inputConfig(psr);
							// err check
							int err_c = lxr->pLexer->rec->getNumberOfSyntaxErrors(lxr->pLexer->rec);
							err_c += psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec);
							if(err_c > 0){
								attron(COLOR_PAIR(1));
								printw("ERROR: ");
								attroff(COLOR_PAIR(1));
								printw("Invalid configuration file syntax!\n");

							}else{
								printw("Done\n");

								// get structure
								antlr::process_config(ast_cfg.tree, cfg_cnt);
								// validate
								if(plg->config->validate(plg->config->get_definition_root(), cfg_cnt)){

									/*
									timespec ts;
									clock_gettime(0, &ts);
									std::cout.precision(15);
									std::stringstream ss;
									mkdir("./commit-log", 0777);
									ss.precision(15);
									ss << "./commit-log/cfg_";
									ss << ((double)ts.tv_sec + ((double)ts.tv_nsec / 1000000000));
									ss << ".pmcfg";
									*/

									//std::ofstream ofs(ss.str().c_str(), std::ios::out | std::ios::binary);
									//if(ofs.is_open()){
										//printw("Saving current configuration to \"%s\"...\n", ss.str().c_str());
										//cfg->show_config(cfg->get_definition_root(), 0, &tmp_size, false, &ofs, false, NULL);
										//ofs.close();
										//printw("Done\n");

										// prepare for config data replacement
										plg->config->replace_prepare(plg->config->get_definition_root());
										// merge new data
										printw("Merging new configuration file...\n");
										int res = plg->config->merge(plg->config->get_definition_root(), cfg_cnt, true);
										// err check
										if(res != 0){
											attron(COLOR_PAIR(1));
											printw("ERROR: ");
											attroff(COLOR_PAIR(1));
											printw("Cannot merge configuration file contents!\n");
										}else{
											printw("Done\n");

										}


/*
									}else{
									    attron(COLOR_PAIR(1));
									    printw("ERROR: ");
									    attroff(COLOR_PAIR(1));
									    printw("Cannot create file \"%s\"\n", ss.str().c_str());

									}
*/



								// err
								}else{
								    attron(COLOR_PAIR(1));
								    printw("ERROR: ");
								    attroff(COLOR_PAIR(1));
								    printw("Invalid/undefined configuration file contents!\n");

								}



							}
							// free mem
							delete cfg_cnt;
							delete[] tmp_file_buff;
							antlr::free_mem(pmp);
						    }
						    // clear value
						    tmp_cli_res.children[0]->new_value = "";


					    }else{
						attron(COLOR_PAIR(1));
						printw("ERROR: ");
						attroff(COLOR_PAIR(1));
						printw("Filename not defined!\n");

					    }
					}
				    }

			    }
			}

		    }
		// edit mode
		}else if(mode == config::CONFIG_MT_EDIT){
		    if(last_found != NULL){
			if(last_found->node_type == config::CONFIG_NT_BLOCK){
				// set current cfg path
				cfg->set_definition_wn(last_found);

				// regenerate cli path
				std::string* cpath = cli->get_current_path_line();
				*cpath = "";
				cli->generate_path(cli->get_current_path(), cpath);

				// generate cfg path
				std::string tmp;
				cfg->generate_path(cfg->get_definition_wn(), &tmp);

				// add cfg path to cli path
				cpath->append(tmp);

				// generate prompt
				cli->generate_prompt();

			}else{
				attron(COLOR_PAIR(1));
				printw("ERROR: ");
				attroff(COLOR_PAIR(1));
				printw("Cannot navigate to non block mode \"%s\"!\n", last_found->name.c_str());

			}
		    }

		// show mode
		}else if(mode == config::CONFIG_MT_SHOW){
			cfg->print_cfg_def(true, false, &tmp_cli_res, 0, 1);

		// set mode
		}else if(mode == config::CONFIG_MT_SET){
			for(int i = 0; i<error_count; i++){
				attron(COLOR_PAIR(1));
				printw("ERROR: ");
				attroff(COLOR_PAIR(1));
				printw(tmp_err[i].c_str());

			}
		}
	}






	tmp_cli_res.children.clear();

	// add to history
	cli->add_to_history(cli->get_current_line());
	cli->history_index = cli->get_historu_size();

	// print prompt
	printw(cli->get_prompt()->c_str());
	cli->clear_curent_line();

    }

    return NULL;

}





