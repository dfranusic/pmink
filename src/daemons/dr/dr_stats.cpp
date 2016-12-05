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

#include <dr_stats.h>


// RXSmsgPoolEmpty
dr::RXSmsgPoolEmpty::RXSmsgPoolEmpty(DrdDescriptor* _drd): drd(_drd){

}


void dr::RXSmsgPoolEmpty::run(){
    value = drd->r14psmm->stats.get(r14p::SST_RX_SMSG_POOL_EMPTY);
}


// RXSparamPoolEmpty
dr::RXSparamPoolEmpty::RXSparamPoolEmpty(DrdDescriptor* _drd): drd(_drd){

}


void dr::RXSparamPoolEmpty::run(){
    value = drd->r14psmm->stats.get(r14p::SST_RX_SPARAM_POOL_EMPTY);
}

// RXCount
dr::RXCount::RXCount(DrdDescriptor* _drd): drd(_drd){

}


void dr::RXCount::run(){
    value = drd->stats.get(DST_RX_COUNT);
}

// UnsupportedSrvc
dr::UnsupportedSrvc::UnsupportedSrvc(DrdDescriptor* _drd): drd(_drd){

}


void dr::UnsupportedSrvc::run(){
    value = drd->stats.get(DST_UNSUPPORTED_SERVICE);
}

// SmsgIncomplete
dr::SmsgIncomplete::SmsgIncomplete(DrdDescriptor* _drd): drd(_drd){

}


void dr::SmsgIncomplete::run(){
    value = drd->stats.get(DST_SMSG_INCOMPLETE);
}

// DbrPoolEmpty
dr::DbrPoolEmpty::DbrPoolEmpty(DrdDescriptor* _drd): drd(_drd){

}


void dr::DbrPoolEmpty::run(){
    value = drd->stats.get(DST_DBR_POOL_EMPTY);
}

// TcapTimeout
dr::TcapTimeout::TcapTimeout(DrdDescriptor* _drd): drd(_drd){

}


void dr::TcapTimeout::run(){
    value = drd->stats.get(DST_TCAP_TIMEOUT);
}

// RxSMS
dr::RxSMS::RxSMS(DrdDescriptor* _drd): drd(_drd){

}


void dr::RxSMS::run(){
    value = drd->smsrcp->stats.get(db::CombinedRecordProcessor::CRPST_SMS);
}

// RxSRI
dr::RxSRI::RxSRI(DrdDescriptor* _drd): drd(_drd){

}


void dr::RxSRI::run(){
    value = drd->smsrcp->stats.get(db::CombinedRecordProcessor::CRPST_SRI);
}
