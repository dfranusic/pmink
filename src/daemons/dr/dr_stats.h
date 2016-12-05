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
#include <r14p_utils.h>
#include <dr.h>

#ifndef DR_STATS_H_
#define DR_STATS_H_

namespace dr{
    // RXSmsgPoolEmpty
    class RXSmsgPoolEmpty: public r14p::R14PTrapHandler {
    public:
	RXSmsgPoolEmpty(DrdDescriptor* _drd);
	void run();

    private:
	DrdDescriptor* drd;

    };

    // RXSparamPoolEmpty
    class RXSparamPoolEmpty: public r14p::R14PTrapHandler {
    public:
	RXSparamPoolEmpty(DrdDescriptor* _drd);
	void run();

    private:
	DrdDescriptor* drd;

    };

    // RXCount
    class RXCount: public r14p::R14PTrapHandler {
    public:
	RXCount(DrdDescriptor* _drd);
	void run();

    private:
	DrdDescriptor* drd;

    };

    // UnsupportedSrvc
    class UnsupportedSrvc: public r14p::R14PTrapHandler {
    public:
	UnsupportedSrvc(DrdDescriptor* _drd);
	void run();

    private:
	DrdDescriptor* drd;

    };

    // SmsgIncomplete
    class SmsgIncomplete: public r14p::R14PTrapHandler {
    public:
	SmsgIncomplete(DrdDescriptor* _drd);
	void run();

    private:
	DrdDescriptor* drd;

    };

    // DbrPoolEmpty
    class DbrPoolEmpty: public r14p::R14PTrapHandler {
    public:
	DbrPoolEmpty(DrdDescriptor* _drd);
	void run();

    private:
	DrdDescriptor* drd;

    };

    // TcapTimeout
    class TcapTimeout: public r14p::R14PTrapHandler {
    public:
	TcapTimeout(DrdDescriptor* _drd);
	void run();

    private:
	DrdDescriptor* drd;

    };

    // RxSMS
    class RxSMS: public r14p::R14PTrapHandler {
    public:
	RxSMS(DrdDescriptor* _drd);
	void run();

    private:
	DrdDescriptor* drd;

    };

    // RxSRI
    class RxSRI: public r14p::R14PTrapHandler {
    public:
	RxSRI(DrdDescriptor* _drd);
	void run();

    private:
	DrdDescriptor* drd;

    };


}



#endif /* DR_STATS_H_ */
