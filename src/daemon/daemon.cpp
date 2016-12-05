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

#include <daemon.h>
#include <stdarg.h>
#include <sys/resource.h>
#include <sys/capability.h>

// static and extern
bool pmink::DaemonDescriptor::DAEMON_TERMINATED = false;
pmink::DaemonDescriptor* pmink::CURRENT_DAEMON = NULL;

// DaemonDescriptor
pmink::DaemonDescriptor::DaemonDescriptor(){
	DAEMON_TERMINATED = false;
	CURRENT_DAEMON = this;
	log_level.set(LLT_INFO);
}

pmink::DaemonDescriptor::DaemonDescriptor(const char* _type, const char* _id, const char* _desc){
	DAEMON_TERMINATED = false;
	CURRENT_DAEMON = this;
	log_level.set(LLT_INFO);

	bzero(full_daemon_id, sizeof(full_daemon_id));
	bzero(daemon_id, sizeof(daemon_id));
	if(_id != NULL){
	   if( set_daemon_id(_id) > 0){
		std::cout << "ERROR: Maximum size of daemon id string is 15 characters!" << std::endl;
		exit(EXIT_FAILURE);

	   }
	}

	bzero(daemon_type, sizeof(daemon_type));
	if(_type != NULL){
	    if(set_daemon_type(_type) > 0){
		std::cout << "ERROR: Maximum size of daemon type string is 15 characters!" << std::endl;
		exit(EXIT_FAILURE);

	    }
	}

	bzero(daemon_description, sizeof(daemon_description));
	if(_desc != NULL){
	    if(set_daemon_description(_desc) > 0){
		std::cout << "ERROR: Maximum size of daemon description string is 500 characters!" << std::endl;
		exit(EXIT_FAILURE);

	    }
	}


}


pmink::DaemonDescriptor::~DaemonDescriptor(){

}

void pmink::DaemonDescriptor::print_help(){
	std::cout << daemon_type << " - " << daemon_description << std::endl;
	std::cout << "Copyright (c) 2012 Release14.org" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << " -?\thelp" << std::endl;

}

void pmink::DaemonDescriptor::signal_handler(int signum){
    switch(signum) {
		case SIGTERM:
			// set termination flag
			DaemonDescriptor::DAEMON_TERMINATED = true;
			break;
    }

}

void pmink::DaemonDescriptor::set_log_level(LogLevelType _log_level){
    log_level.set(_log_level);
}

pmink::LogLevelType pmink::DaemonDescriptor::get_log_level(){
    return log_level.get();
}

void pmink::DaemonDescriptor::terminate(){

}

void* pmink::DaemonDescriptor::get_param(int param_id){
    if(params.find(param_id) != params.end()) return params[param_id];
    return NULL;
}

void pmink::DaemonDescriptor::set_param(int param_id, void* param){
    params[param_id] = param;
}


std::ostringstream& pmink::DaemonDescriptor::get_log_stream(){
    return log_stream;
}


void pmink::DaemonDescriptor::flush_log_stream(LogLevelType _log_level){
    log(_log_level, log_stream.str().c_str());
    log_stream.str("");
}


void pmink::DaemonDescriptor::log(LogLevelType _log_level, const char* msg, ...){
    // all DEBUG levels translate to syslog's DEBUG level
    //if(_log_level >= 7) _log_level = LLT_DEBUG;
    // create va list
    va_list argp;
    va_start(argp, msg);
    // log level check
    if(_log_level <= log_level.get()){
	    // open log
	    openlog(CURRENT_DAEMON->get_full_daemon_id(), LOG_PID | LOG_CONS, LOG_USER);
	    // log
	    vsyslog(LOG_USER | _log_level, msg, argp);
    }
    va_end(argp);
}


void pmink::DaemonDescriptor::process_arguements(int argc, char** argv){
	int opt;

	if(argc == 1){
		print_help();
		exit(EXIT_FAILURE);
		return;
	}else{
		while ((opt = getopt(argc, argv, "?")) != -1) {
			switch(opt){
				// help
				case '?':
					print_help();
					exit(EXIT_FAILURE);
					return;
			}
		}
	}

}

int pmink::DaemonDescriptor::set_daemon_id(const char* _id){
	if(_id == NULL) return 1;
	if(strlen(_id) == 0) return 1;
	if(strlen(_id) <= 15){
	    strcpy(daemon_id, _id);
	    // prefix with "pmink."
	    memcpy(full_daemon_id, "pmink.", 6);
	    // add daemon id after prefix
	    memcpy(&full_daemon_id[6], daemon_id, strlen(daemon_id));
	    return 0;
	}

	return 1;
	//if(_id != NULL) strcat(daemon_id, _id);

}
const char* pmink::DaemonDescriptor::get_daemon_id(){
	return daemon_id;
}

const char* pmink::DaemonDescriptor::get_full_daemon_id(){
	return full_daemon_id;
}


int pmink::DaemonDescriptor::set_daemon_type(const char* _type){
	if(_type == NULL) return 1;
	if(strlen(_type) == 0) return 1;
	if(strlen(_type) <= 15) {
	    strcpy(daemon_type, _type);
	    return 0;
	}

	return 1;
	//strcpy(daemon_type, "pmink.");
	//if(_type != NULL) strcat(daemon_type, _type);

}
int pmink::DaemonDescriptor::set_daemon_description(const char* _desc){
	if(strlen(_desc) == 0) return 1;
	if(strlen(_desc) <= 500){
	    strcpy(daemon_description, _desc);
	    return 0;
	}

	return 1;

}

const char* pmink::DaemonDescriptor::get_daemon_type(){
	return daemon_type;
}

const char* pmink::DaemonDescriptor::get_daemon_description(){
	return daemon_description;
}


void pmink::daemon_start(DaemonDescriptor* daemon_descriptor){
	if(daemon_descriptor != NULL){
	    // open log
	    openlog(daemon_descriptor->get_full_daemon_id(), LOG_PID | LOG_CONS, LOG_USER);
	    // log
	    syslog(LOG_INFO, "starting...");
	    // caps check
	    if(!pmink_caps_valid()){
		// log
		syslog(LOG_ERR, "User has insufficient privileges, enable CAP_SYS_NICE capability or set pam_limits RTPRIO value to 100");
		// exit
		exit(EXIT_FAILURE);
	    }

	}

}

void pmink::daemon_terminate(DaemonDescriptor* daemon_descriptor){
	if(daemon_descriptor != NULL){
	    syslog(LOG_INFO, "terminating...");
	    daemon_descriptor->terminate();
	    closelog();
	}
}

void pmink::daemon_loop(DaemonDescriptor* daemon_descriptor){
	while(!pmink::DaemonDescriptor::DAEMON_TERMINATED){
		sleep(30);
		//syslog(LOG_DEBUG, "heart-beat...");

	}
	// release memory
	daemon_terminate(daemon_descriptor);


}

bool pmink::pmink_caps_valid(){
    // check caps
    cap_t caps;
    caps = cap_get_proc();
    // check if CAP_SYS_NICE privilege is set
    cap_flag_value_t t;
    cap_get_flag(caps, CAP_SYS_NICE, CAP_PERMITTED, &t);
    // free mem
    cap_free(caps);
    // if CAP_SYS_NICE was set, return (running under root probably)
    if(t == CAP_SET) return true;
    // check rlimits
    rlimit rlim;
    getrlimit(RLIMIT_RTPRIO, &rlim);
    // if RLIMIT_RTPRIO soft and hard limites are set to 100, return
    if(rlim.rlim_cur == 100 && rlim.rlim_max == 100) return true;
    // both CAP_SYS_NICE and RLIMIT_RTPRIO privileges are insufficient,
    // return false
    return false;
}


void pmink::signal_handler(int signum){
    if(CURRENT_DAEMON != NULL) CURRENT_DAEMON->signal_handler(signum);
}


void pmink::daemon_init(DaemonDescriptor* daemon_descriptor){
    pid_t pid, sid;

    /* Fork off the parent process */
    pid = fork();
    if(pid < 0) exit(EXIT_FAILURE);

    /* If we got a good PID, then we can exit the parent process. */
    if(pid > 0) exit(EXIT_SUCCESS);

    /* child process executing from here, fork return 0 in child process */

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if(sid < 0) exit(EXIT_FAILURE);

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if((chdir("/")) < 0) exit(EXIT_FAILURE);

    /* Close standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // custom signal handlers
    signal(SIGTERM, &signal_handler);

    // start daemon
    daemon_start(daemon_descriptor);


}




