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

// header guard
#ifndef PM_FGN_MODULE_EXAMPLE_H
#define PM_FGN_MODULE_EXAMPLE_H

// fgn plugin header
#include <pm_fgn_plugins.h>

// module name (pm_plg_fgn_example)
#define PM_FGN_MODULE pm_plg_fgn_example

// declare module init method
PM_FGN_MODULE_INIT_DECLARE(PM_FGN_MODULE)

// declare "pm_plg_fgn_example.example_test" method
PM_FGN_MODULE_METHOD_DECLARE(example_test)
PM_FGN_MODULE_METHOD_DECLARE(example_test_2)

#endif //PM_FGN_MODULE_EXAMPLE_H
