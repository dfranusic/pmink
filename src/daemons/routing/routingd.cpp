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

#include <routing.h>

// main
int main(int argc, char** argv){
    // create daemon
    RoutingdDescriptor dd(DAEMON_TYPE, DAEMON_DESCRIPTION);
    // process arguments
    dd.process_arguements(argc, argv);
    // init/start daemon
    //pmink::daemon_init(&dd);
    pmink::daemon_start(&dd);
    signal(SIGTERM, &pmink::signal_handler);
    // init
    dd.init();
    // loop until terminated
    pmink::daemon_loop(&dd);
    // sleep couple of seconds
    sleep(5);
    // normal exit
    return 0;
}
