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

#ifndef FGN_STATS_H_
#define FGN_STATS_H_


namespace fgn{
    enum StatsType{
	ST_UNSUPPORTED_R14P_SERVICE	= 1,
	ST_SMSG_SEND_ERROR		= 2,
	ST_SMSG_MSG_POOL_EMPTY		= 3,
	ST_SMSG_PARAM_POOL_EMPTY	= 4,
	ST_SMSG_INCOMPLETE		= 5,
	ST_SMSG_RX_COUNT		= 6,
	ST_SMSG_TX_COUNT		= 7,
	ST_RULE_COUNTER			= 9,
	ST_FWORKER_NA			= 10,
	ST_FGN_PAYLOAD_POOL_EMPTY	= 11,
	ST_RPROC_POOL_EMPTY		= 12,
	ST_TCAP_MULTIPLE_COMPONENTS	= 13

    };

    // StatsHandler
    class StatsHandler: public r14p::R14PTrapHandler {
    public:
	StatsHandler(pmink_utils::StatsManager* _stats, uint32_t _id);
	void run();

    private:
	pmink_utils::StatsManager* stats;
	uint32_t id;
    };



}



#endif /* FGN_STATS_H_ */
