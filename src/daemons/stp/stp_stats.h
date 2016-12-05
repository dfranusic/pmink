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

#include <r14p_stats.h>

#ifndef STP_STATS_H_
#define STP_STATS_H_

namespace stp{
    enum StpStatsType{
	SST_UNSUPPORTED_R14P_SERVICE	= 1,
	SST_SMSG_SEND_ERROR		= 2,
	SST_SMSG_MSG_POOL_EMPTY		= 3,
	SST_SMSG_PARAM_POOL_EMPTY	= 4,
	SST_SMSG_INCOMPLETE		= 5,
	SST_SMSG_RX_COUNT		= 6,
	SST_SMSG_TX_COUNT		= 7,
	SST_NO_VALID_ROUTES		= 8,
	SST_RULE_COUNTER		= 9



    };

    // StpStatsHandler
    class StpStatsHandler: public r14p::R14PTrapHandler {
    public:
	StpStatsHandler(pmink_utils::StatsManager* _stats, uint32_t _id);
	void run();

    private:
	pmink_utils::StatsManager* stats;
	uint32_t id;
    };


}



#endif /* STP_STATS_H_ */
