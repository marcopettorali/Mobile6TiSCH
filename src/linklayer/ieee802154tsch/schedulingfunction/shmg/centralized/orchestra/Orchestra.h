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

#ifndef __MOBILE6TISCH_ORCHESTRA_H_
#define __MOBILE6TISCH_ORCHESTRA_H_

#include <omnetpp.h>

#include <functional>

#include "linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/CentralizedSF.h"

using namespace omnetpp;

class Orchestra : public CentralizedSF {
   protected:
    // STATE
    int slotframeLength;
    int numChannels;
    bool senderBasedApproach;
    bool receiverBasedApproach;

    // hash
    int hashSeed;

    // METHODS
    virtual void initialize(int stage) override;
    virtual void buildCentralizedSchedule() override;

    virtual void rebuildCentralizedSchedule() override{};

    virtual std::vector<cell_t> manageConflictInTimeslot(int currentTs, std::vector<cell_t> cellsInConflict, mac_address_t myMacAddress,
                                                         mac_address_t myBroadcastAddress) override;

    // build schedules
    void senderBasedApproachBuildSchedule();
    void receiverBasedApproachBuildSchedule();
};

#endif
