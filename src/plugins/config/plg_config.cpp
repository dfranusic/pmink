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
#include <curses.h>
#include <cli.h>
#include <config.h>
#include <string.h>
#include <pmink_utils.h>
#include <antlr_utils.h>
#include <daemon.h>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>
#include <semaphore.h>
#include <r14p.h>
#include <plgcfg_events.h>
#include <errno.h>
#include <config_r14p.h>
#include <atomic.h>

// regular method handler
extern "C" void* method_handler(const char** arg_names, const char** arg_values, int arg_count){ return NULL; }


class HbeatMissed: public r14p::R14PCallbackMethod {
public:
    HbeatMissed(PluginInfo* _plg){
	plg = _plg;
    }
    void run(r14p::R14PCallbackArgs* args){
	r14p::HeartbeatInfo* hi = (r14p::HeartbeatInfo*)args->get_arg(r14p::R14P_CB_INPUT_ARGS, r14p::R14P_CB_ARG_HBEAT_INFO);
	// set activity flag to false
	plg->cfgd_active.comp_swap(true, false);
	// stop heartbeat
	r14p::stop_heartbeat(hi);
	// display warning
	attron(COLOR_PAIR(5));
	printw("\nWARNING: ");
	attroff(COLOR_PAIR(5));
	printw("Heartbeat not received, closing connection to [");
	attron(COLOR_PAIR(4));
	printw("%s", plg->last_cfgd_id);
	attroff(COLOR_PAIR(4));
	printw("]...\n");
	cli::CLIService::CURRENT_CLI_SERVICE->clear_curent_line();
	printw(cli::CLIService::CURRENT_CLI_SERVICE->get_prompt()->c_str());
	refresh();

    }


    PluginInfo* plg;

};

class HbeatRecv: public r14p::R14PCallbackMethod {
public:
    void run(r14p::R14PCallbackArgs* args){

	//printw("RECEIVED: %s:%s recv=%d, missed=%d\n", hi->target_daemon_id, hi->target_daemon_type, hi->received_count, hi->missed_count);
	//refresh();
    }

};


class HbeatCleanup: public r14p::R14PCallbackMethod {
public:
    HbeatCleanup(HbeatRecv* _recv, HbeatMissed* _missed){
	recv = _recv;
	missed = _missed;
    }
    void run(r14p::R14PCallbackArgs* args){
	delete recv;
	delete missed;
	delete this;
    }

    HbeatMissed* missed;
    HbeatRecv* recv;
};


// block handler line handler (ENTER/TAB)
extern "C" void* block_handler(void** args, int argc){
    PluginInfo* plg = (PluginInfo*)args[0];
    cli::CLIService* cli = plg->cli;
    r14p::R14PSession* r14ps = plg->r14ps;
    config::ConfigItem configd_res;
    std::string line(cli->get_current_line()->c_str());
    r14p::R14PStream* r14p_stream = NULL;
    StreamNext stream_next(plg, &configd_res);
    StreamEnd stream_end(plg);
    int x, y;

    // get activity flag, atomic gcc intrinsic
    bool active = plg->cfgd_active.get();

    if(!active){
	attron(COLOR_PAIR(4));
	printw("Lost connection to config daemon, trying to re-connect...\n");
	attroff(COLOR_PAIR(4));
	refresh();


	// reconnect
	plg->last_r14pc = r14ps->get_registered_client((unsigned int)0);
	if(plg->last_r14pc != NULL){
		// config auth
		if(config::user_login(plg->config, plg->last_r14pc, NULL, (char*)plg->last_cfgd_id, &plg->cfg_user_id) == 0){
		    // check for valid config daemon id
		    if(strlen((char*)plg->last_cfgd_id) > 0){
			// reset config and request notifications
			if(config::notification_request(plg->config, plg->last_r14pc, "router connections", NULL, (char*)plg->last_cfgd_id, &plg->cfg_user_id, NULL) == 0){

				// create hbeat events
				HbeatRecv* hb_recv = new HbeatRecv();
				HbeatMissed* hb_missed = new HbeatMissed(plg);
				HbeatCleanup* hb_cleanup = new HbeatCleanup(hb_recv, hb_missed);

				// init hbeat
				plg->hbeat = r14p::init_heartbeat("config_daemon", (char*)plg->last_cfgd_id, plg->last_r14pc, 60, hb_recv, hb_missed, hb_cleanup);
				if(plg->hbeat != NULL){
					plg->cfgd_active.comp_swap(false, true);
					attron(COLOR_PAIR(4));
					printw("Connection to config daemon successfully re-established!\n");
					attroff(COLOR_PAIR(4));
					refresh();

				// free event memory on error
				}else{
				    delete hb_recv;
				    delete hb_missed;
				    delete hb_cleanup;
				}

			// notification error
			}else{
			    attron(COLOR_PAIR(1));
			    printw("ERROR: ");
			    attroff(COLOR_PAIR(1));
			    printw("Cannot request configuration notification!\n");
			    refresh();

			}

		    // config daemon if error
		    }else{
			    attron(COLOR_PAIR(1));
			    printw("ERROR: ");
			    attroff(COLOR_PAIR(1));
			    printw("Cannot find config daemon id!\n");
			    refresh();

		    }
		}


	}
	// prompt
	cli->clear_curent_line();
	printw(cli->get_prompt()->c_str());
	printw(cli->get_current_line()->c_str());
	refresh();


	return NULL;
    }

    r14p::R14PClient* r14p_client = plg->last_r14pc; //r14ps->get_registered_client((unsigned int)0);

    if(r14p_client == NULL){
	printw(cli->get_prompt()->c_str());
	cli->clear_curent_line();
	return NULL;

    }

    // TAB
    if(cli->state == cli::CLI_ST_AUTO_COMPLETE){
	// set ac_mode
	stream_next.ac_mode = config::CONFIG_ACM_TAB;
	// start new R14P stream
	r14p_stream = r14p_client->new_stream("config_daemon", NULL, NULL, &stream_next);
	// check if stream can be created
	if(r14p_stream == NULL){
	    attron(COLOR_PAIR(1));
	    printw("ERROR: ");
	    attroff(COLOR_PAIR(1));
	    printw("Cannot initialize R14P stream!\n");
	    return NULL;
	}
	// set end event handler
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, &stream_end);
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, &stream_end);
	// create body
	asn1::R14PMessage* r14pm = r14p_stream->get_r14p_message();
	// prepare body
	if(r14pm->_body != NULL) {
		r14pm->_body->unlink(1);
		r14pm->_body->_conf->set_linked_data(1);

	}else{
		r14pm->set_body();
		r14pm->prepare();
	}

	// set bodu
	uint32_t ac_id = htobe32(asn1::ParameterType::_pt_pmink_config_ac_line);
	uint32_t auth_id = htobe32(asn1::ParameterType::_pt_pmink_auth_id);
	uint32_t cfg_action = asn1::ConfigAction::_ca_cfg_ac;
	// remove payload
	if(r14pm->_body->_conf->_payload != NULL) r14pm->_body->_conf->_payload->unlink(1);
	// set params
	if(r14pm->_body->_conf->_params == NULL){
		r14pm->_body->_conf->set_params();
		// set children, allocate more
		for(int i = 0; i<2; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	// unlink params before setting new ones
	}else{
	    int cc = r14pm->_body->_conf->_params->children.size();
	    if(cc < 2){
		// set children, allocate more
		for(int i = cc; i<2; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	    }else if(cc > 2){
		// remove extra children if used in some other session, only 2 needed
		for(int i = 2; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
	    }
	}
	// set values
	r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&cfg_action, 1);
	// ac line
	r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&ac_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)line.c_str(), line.size());

	// auth id
	r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&auth_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)plg->cfg_user_id.user_id, strlen((char*)plg->cfg_user_id.user_id));


	// start stream
	r14p_stream->send(true);
	attron(COLOR_PAIR(4));
	printw("Waiting for config daemon...");
	attroff(COLOR_PAIR(4));
	refresh();
	// wait for signal
	timespec ts;
	clock_gettime(0, &ts);
	ts.tv_sec += 5;
	//int sres = sem_timedwait(&plg->sem_cfgd, &ts);
	int sres = sem_wait(&plg->sem_cfgd);
	//sem_wait(&plg->sem_cfgd);
	// err
	if(sres == -1){
	    if(errno == ETIMEDOUT){
		getyx(stdscr, y, x);
		move(y, 0);
		clrtoeol();
		attron(COLOR_PAIR(1));
		printw("ERROR: ");
		attroff(COLOR_PAIR(1));
		printw("Timeout while waiting for config daemon!\n");

	    }
	// ok
	}else{
		getyx(stdscr, y, x);
		move(y, 0);
		clrtoeol();
		// more
		int w, h;
		getmaxyx(stdscr, h, w);
		// get date size
		int line_c = stream_next.cfg_res->children.size();
		// create buffer win
		WINDOW* new_win = newwin(line_c, w, 0, 0);
		// get data
		plg->config->print_cfg_def(stream_next.cm_mode == config::CONFIG_MT_SET ? true : false, true, stream_next.cfg_res, 0, 1, new_win);
		// more
		cli->buff_ch = pmink_utils::cli_more(line_c, new_win, cli->get_interrupt_p());
		// free
		delwin(new_win);

	}
	// prompt
	printw(cli->get_prompt()->c_str());
	printw(cli->get_current_line()->c_str());





    // ENTER
    }else if(cli->state == cli::CLI_ST_EXECUTE){
	// set ac_mode
	stream_next.ac_mode = config::CONFIG_ACM_ENTER;
	// start new R14P stream
	r14p_stream = r14p_client->new_stream("config_daemon", NULL, NULL, &stream_next);
	// check if stream can be created
	if(r14p_stream == NULL){
	    attron(COLOR_PAIR(1));
	    printw("ERROR: ");
	    attroff(COLOR_PAIR(1));
	    printw("Cannot initialize R14P stream!\n");
	    return NULL;
	}
	// set end event handler
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_END, &stream_end);
	r14p_stream->set_callback(r14p::R14P_ET_STREAM_TIMEOUT, &stream_end);
	// create body
	asn1::R14PMessage* r14pm = r14p_stream->get_r14p_message();
	// prepare body
	if(r14pm->_body != NULL) {
		r14pm->_body->unlink(1);
		r14pm->_body->_conf->set_linked_data(1);

	}else{
		r14pm->set_body();
		r14pm->prepare();
	}

	// set bodu
	uint32_t ac_id = htobe32(asn1::ParameterType::_pt_pmink_config_ac_line);
	uint32_t auth_id = htobe32(asn1::ParameterType::_pt_pmink_auth_id);
	uint32_t cfg_action = asn1::ConfigAction::_ca_cfg_set;
	// remove payload
	if(r14pm->_body->_conf->_payload != NULL) r14pm->_body->_conf->_payload->unlink(1);
	// set params
	if(r14pm->_body->_conf->_params == NULL){
		r14pm->_body->_conf->set_params();
		// set children, allocate more
		for(int i = 0; i<2; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	// unlink params before setting new ones
	}else{
	    int cc = r14pm->_body->_conf->_params->children.size();
	    if(cc < 2){
		// set children, allocate more
		for(int i = cc; i<2; i++){
			r14pm->_body->_conf->_params->set_child(i);
			r14pm->_body->_conf->_params->get_child(i)->set_value();
			r14pm->_body->_conf->_params->get_child(i)->_value->set_child(0);

		}
		// prepare
		r14pm->prepare();

	    }else if(cc > 2){
		// remove extra children if used in some other session, only 2 needed
		for(int i = 2; i<cc; i++) r14pm->_body->_conf->_params->get_child(i)->unlink(1);
	    }
	}
	// set values
	r14pm->_body->_conf->_action->set_linked_data(1, (unsigned char*)&cfg_action, 1);
	// ac id
	r14pm->_body->_conf->_params->get_child(0)->_id->set_linked_data(1, (unsigned char*)&ac_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(0)->_value->get_child(0)->set_linked_data(1, (unsigned char*)line.c_str(), line.size());
	// auth id
	r14pm->_body->_conf->_params->get_child(1)->_id->set_linked_data(1, (unsigned char*)&auth_id, sizeof(uint32_t));
	r14pm->_body->_conf->_params->get_child(1)->_value->get_child(0)->set_linked_data(1, (unsigned char*)plg->cfg_user_id.user_id, strlen((char*)plg->cfg_user_id.user_id));

	// start stream
	r14p_stream->send(true);
	attron(COLOR_PAIR(4));
	printw("Waiting for config daemon...");
	attroff(COLOR_PAIR(4));
	refresh();
	// wait for signal
	timespec ts;
	clock_gettime(0, &ts);
	ts.tv_sec += 10;
	//int sres = sem_timedwait(&plg->sem_cfgd, &ts);
	int sres = sem_wait(&plg->sem_cfgd);
	if(sres == -1){
	    if(errno == ETIMEDOUT){
		getyx(stdscr, y, x);
		move(y, 0);
		clrtoeol();
		attron(COLOR_PAIR(1));
		printw("ERROR: ");
		attroff(COLOR_PAIR(1));
		printw("Timeout while waiting for config daemon!\n");

	    }
	// ok
	}else{
		getyx(stdscr, y, x);
		move(y, 0);
		clrtoeol();
		// more
		int w, h;
		getmaxyx(stdscr, h, w);

		// get date size
		int line_c = 0;
		// print error
		if(stream_next.error_count > 0){
			line_c = stream_next.error_count;
			// create buffer win
			WINDOW* new_win = newwin(line_c, w, 0, 0);
			// get data
			for(int i = 0; i<stream_next.error_count; i++){
			    wattron(new_win, COLOR_PAIR(1));
			    wprintw(new_win, "ERROR: ");
			    wattroff(new_win, COLOR_PAIR(1));
			    wprintw(new_win, "%s\n", stream_next.err_lst[i].c_str());
			}
			// more
			cli->buff_ch = pmink_utils::cli_more(line_c, new_win, cli->get_interrupt_p());
			// free
			delwin(new_win);

		// print cfg result
		}else{
			// get line count
			line_c = stream_next.line_stream_lc;

			// formatted only for "configuration" and "commands"
			if(*plg->cli->get_current_line() == "configuration " || *plg->cli->get_current_line() == "commands "){
				boost::regex addr_regex("(set|delete)?([\t ]*)(?:([+-])?(?:([}])|([\!a-zA-Z0-9_-]+[\t ]*))([{]?)([\t ]+)*(\".*\")?)");
				boost::smatch regex_groups;
				const int groups[] = {1, 2, 3, 4, 5, 6, 7, 8};
				bool new_line = false;
				// regular command output
				if(line_c > 0){
					// create buffer win
					WINDOW* new_win = newwin(line_c, w, 0, 0);
					// get data
					std::string tmp_str;
					while(getline(stream_next.line_stream, tmp_str)){
					    boost::regex_search(tmp_str, regex_groups, addr_regex);
					    boost::sregex_token_iterator iter(tmp_str.begin(), tmp_str.end(), addr_regex, groups);
					    boost::sregex_token_iterator end;
					    int c = 1;
					    int nlc = 0;
					    std::string str_line;
					    std::vector<std::string> tokens;

					    // get tokens in vector
					    while(iter != end){
						str_line.assign(*iter);
						tokens.push_back(str_line);
						iter++;
					    }

					    // loop tokens
					    for(unsigned int k = 0; k<tokens.size(); k++){
							str_line = tokens[k];
							new_line = false;
							switch(c){
							    // config command
							    case 1:
								wattrset(new_win, COLOR_PAIR(4));
								break;

							    // prefix (+/-)
							    case 3:
								if(str_line[0] == '-') wattrset(new_win, COLOR_PAIR(1));
								else if(str_line[0] == '+') wattrset(new_win, COLOR_PAIR(4));
								break;

							    // block open/close
							    case 4:
							    case 6:
								wattrset(new_win, COLOR_PAIR(7));
								if(str_line.size() > 0) new_line = true;
								break;

							    // item value
							    case 8:
								wattrset(new_win, COLOR_PAIR(6));
								if(str_line.size() > 0) new_line = true;
								break;

							    // default
							    default:
								// check for special '!' prefix
								if(tokens[k][0] == '!') {
								    wattrset(new_win, COLOR_PAIR(2));
								    break;
								}
								// check if token index is valid
								if(tokens.size() > k + 3){
								    // check if last token is empty
								    if(tokens[k + 3] == ""){
									    // check if last and not block open
									    if(tokens.size() == k + 4 && tokens[k + 1] != "{") wattrset(new_win, COLOR_PAIR(5));
									    // block
									    else wattrset(new_win, COLOR_PAIR(2));

								    }else wattrset(new_win, COLOR_PAIR(5));

								}else wattrset(new_win, COLOR_PAIR(2));
								break;
							}
							// print data
							wprintw(new_win, "%s", str_line.c_str());
							// print new line
							if(new_line){
							    wprintw(new_win, "\n");
							    ++nlc;
							}

							++c;
							if(c == 9){
							    c = 1;
							}

					    }
					    // if new line not printed before, print it now
					    if(nlc == 0) wprintw(new_win, "\n");


					}
					// more
					cli->buff_ch = pmink_utils::cli_more(line_c, new_win, cli->get_interrupt_p());
					// free
					delwin(new_win);

				// config items
				}else{
				    line_c = stream_next.cfg_res->children.size();
				    // create buffer win
				    WINDOW* new_win = newwin(line_c, w, 0, 0);
				    // get data
				    plg->config->print_cfg_def(true, true, stream_next.cfg_res, 0, 1, new_win);
				    // more
				    cli->buff_ch = pmink_utils::cli_more(line_c, new_win, cli->get_interrupt_p());
				    // free
				    delwin(new_win);
				}

			// unformatted
			}else{
				// lines
				if(line_c > 0){
					// create buffer win
					WINDOW* new_win = newwin(line_c, w, 0, 0);
					// get data
					std::string tmp_str;
					while(getline(stream_next.line_stream, tmp_str)) wprintw(new_win, "%s\n", tmp_str.c_str());
					// more
					cli->buff_ch = pmink_utils::cli_more(line_c, new_win, cli->get_interrupt_p());
					// free
					delwin(new_win);

				// config items
				}else{
				    line_c = stream_next.cfg_res->children.size();
				    // create buffer win
				    WINDOW* new_win = newwin(line_c, w, 0, 0);
				    // get data
				    plg->config->print_cfg_def(true, true, stream_next.cfg_res, 0, 1, new_win);
				    // more
				    cli->buff_ch = pmink_utils::cli_more(line_c, new_win, cli->get_interrupt_p());
				    // free
				    delwin(new_win);


				}
			}






		}


	}
	// print prompt
	printw(cli->get_prompt()->c_str());
	cli->clear_curent_line();

    }




    return NULL;
}


// block handler exit
extern "C" void* block_handler_free(void** args, int argc){
    // set pointer to plg info
    PluginInfo* plg = (PluginInfo*)args[0];
    // config user logout
    r14p::R14PClient* client = plg->last_r14pc; // plg->r14ps->get_registered_client((unsigned int)0);

    bool active = plg->cfgd_active.get();

    if(client != NULL && active) config::user_logout(plg->config, client, NULL, &plg->cfg_user_id);

    //config::Config* cfg = plg->config;
    if(plg->config->get_definition_root() != NULL) delete plg->config->get_definition_root();
    // deallocate config memory
    delete plg->config;
    // status
    attron(COLOR_PAIR(4));
    // new line
    printw("\n");
    printw("Closing R14P connections...");
    attroff(COLOR_PAIR(4));
    refresh();
    // deallocate r14p session memory
    r14p::destroy_session(plg->r14ps);
    // deallocate plugin info
    delete plg;
    // print newline
    printw("\n");
    return NULL;

}



// block handler init
extern "C" void* block_handler_init(void** args, int argc){
	PluginInfo* plg = new PluginInfo();
	plg->cli = (cli::CLIService*)args[0];
	plg->config = new config::Config();
	boost::regex addr_regex("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)");
	int opt;
	std::ostringstream cli_id;

	// reset getopt index
	optind = 1;
	// process arguments
	while ((opt = getopt(plg->cli->cmdl_argc, plg->cli->cmdl_argv, "f:c:i:")) != -1) {
		switch(opt){
			case 'i':
				cli_id << optarg;
				break;

			// config daemon address
			case 'c':
				// check pattern (ipv4:port)
				// check if valid
				if(boost::regex_match(optarg, addr_regex)){
				    plg->cfgd_lst.push_back(new std::string(optarg));
				}else{
				    attron(COLOR_PAIR(1));
				    printw("ERROR: ");
				    attroff(COLOR_PAIR(1));
				    printw("Invalid daemon address format '%'!\n", optarg);

				}
				break;
		}
	}


	// get pid and generate daemon id
	pid_t pd = getpid();
	cli_id << pd;

	// start R14P session
	plg->r14ps = r14p::init_session("cli_service", cli_id.str().c_str(), 100, 5, false, 5);

	// separate IP and PORT
	boost::smatch regex_groups;

	// check caps
	bool caps = pmink::pmink_caps_valid();
	if(!caps){
	    attron(COLOR_PAIR(1));
	    printw("ERROR: ");
	    attroff(COLOR_PAIR(1));
	    printw("User has insufficient privileges, enable CAP_SYS_NICE capability or set pam_limits RTPRIO value to 100\n");  
	}

	// connect to first available config daemon
	if(caps) for(unsigned int i = 0; i<plg->cfgd_lst.size(); i++){
		// separate IP and PORT
		boost::regex_search(*plg->cfgd_lst[i], regex_groups, addr_regex);
		// connect to config daemon
		printw("Trying '%s'...", (*plg->cfgd_lst[i]).c_str());
		r14p::R14PClient* r14p_client = plg->r14ps->connect(regex_groups[1].str().c_str(), atoi(regex_groups[2].str().c_str()), 16, NULL, 0);
		// stop if successful
		if(r14p_client != NULL){
			printw("OK\n");
			if(!plg->cfgd_active.get()){
				if(config::user_login(plg->config, r14p_client, NULL, (char*)plg->last_cfgd_id, &plg->cfg_user_id) == 0){
				    if(strlen((char*)plg->last_cfgd_id) > 0){
					//std::cout << "USING CONFIG ID: " << plg->last_cfgd_id << std::endl;
					//CfgUpdate* cfg_upd = new CfgUpdate();
					if(config::notification_request(plg->config, r14p_client, "router connections", NULL, (char*)plg->last_cfgd_id, &plg->cfg_user_id, NULL) == 0){
						// hbeat events
						HbeatRecv* hb_recv = new HbeatRecv();
						HbeatMissed* hb_missed = new HbeatMissed(plg);
						HbeatCleanup* hb_cleanup = new HbeatCleanup(hb_recv, hb_missed);
						// init hbeat
						plg->hbeat = r14p::init_heartbeat("config_daemon", (char*)plg->last_cfgd_id, r14p_client, 5, hb_recv, hb_missed, hb_cleanup);

						if(plg->hbeat == NULL){
							delete hb_recv;
							delete hb_missed;
							delete hb_cleanup;
						}else{
							plg->last_r14pc = r14p_client;
							plg->cfgd_active.comp_swap(false, true);

						}


					}

				    }else{
					    attron(COLOR_PAIR(1));
					    printw("ERROR: ");
					    attroff(COLOR_PAIR(1));
					    printw("Cannot find config daemon id!\n");

				    }

				}

			}
		}else{
		    attron(COLOR_PAIR(1));
		    printw("ERROR: ");
		    attroff(COLOR_PAIR(1));
		    printw("Cannot establish R14P connection!\n");

		}

	}
	// if no connection was made
	if(plg->r14ps->get_client_count() == 0){
	    attron(COLOR_PAIR(1));
	    printw("ERROR: ");
	    attroff(COLOR_PAIR(1));
	    printw("Cannot establish config daemon connection!\n");

	}



	return plg;

}
