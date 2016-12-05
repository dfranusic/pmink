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

#include <connection.h>
#include <r14p_stats.h>

namespace sgn{
    enum StatsType{
	CST_RX_MSU_COUNT		= 1,
	CST_MTU_LIMIT			= 2,
	CST_DISCARDED_MSU		= 3,
	CST_PLD_POOL_EMPTY		= 4,
	CST_MALFORMED_MSU		= 5,
	CST_RX_ERROR			= 6,
	CST_TX_ERROR			= 7,
	CST_PMPLD_POOL_EMPTY		= 8,
	CST_SMSG_SEND_ERROR		= 9,
	CST_SMSG_MSG_POOL_EMPTY		= 10,
	CST_SMSG_PARAM_POOL_EMPTY	= 11,
	CST_M3UA_ACTIVE			= 12,
	CST_SCTP_ACTIVE			= 13,
	CST_TX_MSU_COUNT		= 14,
	CST_SMSG_SENT_COUNT		= 15,
	CST_NO_VALID_ROUTES		= 16,
	CST_RX_HBEAT_COUNT		= 17,
	CST_TX_HBEAT_COUNT		= 18,
	CST_TCP_ACTIVE			= 19,
	CST_SMPP_ACTIVE			= 20,
	CST_CMD_PROCESS_ERROR		= 21,
	CST_ENCODE_ERROR		= 22,
	CST_RECONNECT			= 23,
        CST_TCP_CONNS                   = 24,
        CST_TCP_MAX_CONNS               = 25,
        CST_TCP_INVALID_SEQ             = 26,
        CST_TCP_MAX_SEGMS               = 27
    };

    enum ApplicationServerStatsType{
	ASST_ASP_NOT_FOUND		= 1,

    };

    enum SgnStatsType{
	SST_AS_NOT_FOUND		= 1,
	SST_NO_VALID_ROUTES		= 2,
	SST_GET_NEXT_ROUTE_ERROR	= 3,
	SST_UNSUPPORTED_R14P_SERVICE	= 4,
	SST_SMSG_SEND_ERROR		= 5,
	SST_SMSG_MSG_POOL_EMPTY		= 6,
	SST_SMSG_PARAM_POOL_EMPTY	= 7,
	SST_SMSG_INCOMPLETE		= 8

    };

    // SgnStatsHandler
    class SgnStatsHandler: public r14p::R14PTrapHandler {
    public:
	SgnStatsHandler(pmink_utils::StatsManager* _stats, uint32_t _id);
	void run();

    private:
	pmink_utils::StatsManager* stats;
	uint32_t id;
    };

    // PCAPStatsHandlerRX
    class PCAPStatsHandlerRX: public r14p::R14PTrapHandler {
    public:
	PCAPStatsHandlerRX(int _sd_id);
	void run();

    private:
	int sd_id;
	#ifdef __PFRING__
	pfring_stat stats;
	#else
	pcap_stat stats;
	#endif
    };

    // PCAPStatsHandlerDROP
    class PCAPStatsHandlerDROP: public r14p::R14PTrapHandler {
    public:
	PCAPStatsHandlerDROP(int _sd_id);
	void run();

    private:
	int sd_id;
	#ifdef __PFRING__
	pfring_stat stats;
	#else
	pcap_stat stats;
	#endif
    };

}



