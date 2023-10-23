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

#ifndef __MOBILE6TISCH_SCHEDULINGFUNCTION_H_
#define __MOBILE6TISCH_SCHEDULINGFUNCTION_H_

#include <omnetpp.h>

#include "common/basemodule/BaseModule.h"
#include "linklayer/ieee802154tsch/IEEE802154TSCHSchedule.h"

using namespace omnetpp;

class SchedulingFunctionBase : virtual public BaseModule {
   protected:
    // STATE
    bool rescheduled = false;

   public:
    bool isRescheduled() { return rescheduled; }

    // retrieve the schedule for a given node
    virtual IEEE802154TSCHSchedule* getSchedule(int macAddress, int broadcastAddress) {
        throw cRuntimeError("SchedulingFunctionBase::getSchedule() not implemented");
    }

    // defines a policy for managing the conflict between multiple rx cells in the same timeslot // ASSUMPTION: nodes have 1 radio
    virtual std::vector<cell_t> manageConflictInTimeslot( int currentTs, std::vector<cell_t> cellsInConflict, mac_address_t myMacAddress, mac_address_t myBroadcastAddress) {
        throw cRuntimeError("CentralizedSF::manageConflictInTimeslot( int currentTs, ) not implemented");
    }

    // UTILITY
    // conflict management policy: keep all tx cells and one rx cell
    std::vector<cell_t> basicRandomConflictManagement (std::vector<cell_t> cells, mac_address_t myMacAddress, mac_address_t myBroadcastAddress);
};

#endif
