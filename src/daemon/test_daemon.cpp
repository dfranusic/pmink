#include <daemon.h>

// custom daemon name and description
#define DAEMON_NAME			"test_daemon"
#define DAEMON_DESCRIPTION	"test daemon description"

// main
int main(int argc, char** argv){
	// create daemon
	pmink::DaemonDescriptor dd(DAEMON_NAME, DAEMON_DESCRIPTION);
	// process arguments
	dd.process_arguements(argc, argv);
	// init/start daemon
	pmink::daemon_init(&dd);

	// ****** daemon implementation and related ******

	// loop until terminated
	pmink::daemon_loop(&dd);
	// normal exit
	return 0;

}


