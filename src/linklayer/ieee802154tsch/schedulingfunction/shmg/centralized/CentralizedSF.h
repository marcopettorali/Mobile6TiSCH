//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __MOBILE6TISCH_CENTRALIZEDSF_H_
#define __MOBILE6TISCH_CENTRALIZEDSF_H_

#include <omnetpp.h>

#include <unordered_map>

#include "linklayer/ieee802154tsch/IEEE802154TSCHSchedule.h"
#include "linklayer/ieee802154tsch/schedulingfunction/base/SchedulingFunctionBase.h"
#include "middleware/shmg/ncmiddleware/base/NCMiddlewareBase.h"

using namespace omnetpp;

class CentralizedSF : public SchedulingFunctionBase {
   protected:
    // STATE
    // check if the schedule should be downloaded from the NC
    bool downloadFromNC;

    // initialSchedule flag
    bool initialSchedule = true;

    // centralized schedule for each node in the network
    IEEE802154TSCHSchedule* centralizedSchedule;

    // modules
    NCMiddlewareBase* ncMid;

    // METHODS
    virtual void initialize(int stage) override;

    // interface with a module that want an instance of the schedule
    virtual IEEE802154TSCHSchedule* getSchedule(mac_address_t macAddress, mac_address_t broadcastAddress) override final;

    // build the centralized schedule
    virtual void buildCentralizedSchedule() { throw cRuntimeError("CentralizedSF::buildCentralizedSchedules() not implemented"); }

    // rebuild the centralized schedule if rescheduled flag is true
    virtual void rebuildCentralizedSchedule() { buildCentralizedSchedule(); }

    // filter centralized schedule according to the mac addresses
    IEEE802154TSCHSchedule* filterCentralizedSchedule(mac_address_t macAddress, mac_address_t broadcastAddress);
};

#endif
