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
 * @file config_r14p.h
 * @brief Configuration relay and notification management
 *
 */
#ifndef CONFIG_R14P_H_
#define CONFIG_R14P_H_

#include <config.h>
#include <r14p.h>

namespace config {

    class R14PCfgNtfUser: public config::UserId {
    public:
	R14PCfgNtfUser(r14p::R14PClient* _r14pc);

	r14p::R14PClient* r14pc;

    };

    class R14PCfgNotification : public CfgNotification {
    public:
	R14PCfgNotification(std::string* _cfg_path);
	~R14PCfgNotification();

	int notify(void* args);
	void* reg_user(void* usr);
	int unreg_user(void* usr);
	bool user_exists(R14PCfgNtfUser* usr);
	R14PCfgNtfUser* get_user(unsigned int usr_index);
	unsigned int get_user_count();

	config::ConfigItem ntf_cfg_lst;
	bool ready;

    private:
	std::vector<R14PCfgNtfUser> users;

    };

    // Distribute config stream next event definition
    class DistributeCfgStreamNext: public r14p::R14PCallbackMethod {
    public:
	DistributeCfgStreamNext();
        // event handler method
        void run(r14p::R14PCallbackArgs* args);

        uint32_t res_count;
        int res_index;
        config::UserId cfg_user_id;
        std::string repl_line;
        uint32_t ca_cfg_replicate;
        uint32_t pt_cfg_repl_line;
        uint32_t pt_cfg_auth_id;

    };


    // Distribute config stream finished event definition
    class DistributeCfgStreamDone: public r14p::R14PCallbackMethod {
    public:
        // event handler method
        void run(r14p::R14PCallbackArgs* args);

        DistributeCfgStreamNext* snext;

    };


    // Notify user stream next event definition
    class NtfyUsrStreamNext: public r14p::R14PCallbackMethod {
    public:
	NtfyUsrStreamNext();
        // event handler method
        void run(r14p::R14PCallbackArgs* args);

        uint32_t res_count;
        unsigned int res_index;
        config::ConfigItem cfg_flat;
        config::R14PCfgNotification* cfg_ntf;
        config::R14PCfgNtfUser* ntf_user;
        config::Config* config;
        uint32_t pt_cfg_item_value;
        uint32_t pt_cfg_item_path;
        uint32_t ca_cfg_set;
        uint32_t pt_cfg_item_count;
        uint32_t pt_cfg_item_ns;
        uint32_t pt_cfg_item_nt;

    };

    // Notify user stream finished
    class NtfyUsrStreamDone: public r14p::R14PCallbackMethod {
    public:
	NtfyUsrStreamDone();
        // event handler method
        void run(r14p::R14PCallbackArgs* args);
        // stream next
        NtfyUsrStreamNext* snext;

    };


    // Reg user stream next event definition
    class RegUsrStreamNext: public r14p::R14PCallbackMethod {
    public:
	RegUsrStreamNext();
        // event handler method
        void run(r14p::R14PCallbackArgs* args);
        // buffer
        uint32_t cfg_count;
        config::ConfigItem cfg_res;

    };

    // Reg user stream finished
    class RegUseStreamDone : public r14p::R14PCallbackMethod {
    public:
	RegUseStreamDone();
	~RegUseStreamDone();
        // event handler method
        void run(r14p::R14PCallbackArgs* args);
        // signal
        sem_t signal;
        int status;
        // stream next
        RegUsrStreamNext* snext;
    };

    // fwd declaration
    class CfgUpdateStreamNext;


    // config client terminated
    class CfgUpdateClientTerm : public r14p::R14PCallbackMethod {
    public:
        // event handler method
        void run(r14p::R14PCallbackArgs* args);

    };

    // Config update new stream
    class CfgUpdateStreamNew : public r14p::R14PCallbackMethod {
    public:
	CfgUpdateStreamNew();
        // event handler method
        void run(r14p::R14PCallbackArgs* args);
	config::CfgNtfCallback* update_done;
	// config
	config::Config* config;
    };

    // Config update stream done
    class CfgUpdateStreamDone : public r14p::R14PCallbackMethod {
    private:
	void process_cfg_events();

    public:
	CfgUpdateStreamDone();
        // event handler method
        void run(r14p::R14PCallbackArgs* args);
	// stream done
	CfgUpdateStreamNew* snew;
	CfgUpdateStreamNext* snext;
    };


    // Config update stream next
    class CfgUpdateStreamNext : public r14p::R14PCallbackMethod {
    public:
	CfgUpdateStreamNext();
        // event handler method
        void run(r14p::R14PCallbackArgs* args);
        // stream done
        CfgUpdateStreamDone sdone;
	// pending update count
	int update_count;
	int res_index;
	config::ConfigItem cfg_res;
    };


    // user login (called by user daemon)
    int	user_login(	config::Config* config,
			r14p::R14PClient* cfgd_r14pc,
			const char* _target_daemon_id,
			char* _connected_daemon_id,
			config::UserId* cfg_user_id);

    // user logout (called by user daemon)
    int user_logout(	config::Config* config,
			r14p::R14PClient* cfgd_r14pc,
			const char* _daemon_id,
			config::UserId* cfg_user_id);


    // register user to receive notifications (called by user daemon)
    int notification_request(	config::Config* config,
				r14p::R14PClient* cfgd_r14pc,
				const char* usr_root,
				config::CfgNtfCallback* update_rcvd,
				const char* _daemon_id,
				config::UserId* cfg_user_id,
				r14p::R14PCallbackMethod* non_cfg_hndlr);

    // create config event hanlers
    r14p::R14PCallbackMethod* create_cfg_event_handler(config::Config* config,
						      r14p::R14PCallbackMethod* non_cfg_hndlr = NULL);


    // notify user when configuration changes (called by config daemon after commit)
    int notify_user(	config::Config* config,
			config::ConfigItem* cfg_flat,
			//r14p::R14PClient* cfg_user,
			config::R14PCfgNtfUser* ntf_user,
			config::R14PCfgNotification* cfg_ntf);

    // replicate to other config daemon (called by config daemon)
    int replicate(	const char* repl_line,
			r14p::R14PClient* _client,
			const char* _daemon_id,
			config::UserId* _cfg_user_id);



}



#endif /* CONFIG_R14P_H_ */
