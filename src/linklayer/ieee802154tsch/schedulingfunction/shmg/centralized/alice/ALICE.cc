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

#include "ALICE.h"

Define_Module(ALICE);

void ALICE::initialize(int stage) {
    CentralizedSF::initialize(stage);

    if (stage == INITSTAGE_PARAMS_FETCH) {
        slotframeLength = par("slotframeLength");
        numChannels = par("numChannels");

        // check parameters
        if (slotframeLength < 1) {
            throw cRuntimeError("ALICE::initialize: slotframeLength must be >= 1");
        }

        if (numChannels < 1) {
            throw cRuntimeError("ALICE::initialize: numChannels must be >= 1");
        }

        if (numChannels > 16) {
            throw cRuntimeError("ALICE::initialize: numChannels must be <= %d", 16);
        }

        // get hashSeed
        hashSeed = getRNG(RNG_HASHSEED)->intRand();
    }
}

void ALICE::buildCentralizedSchedule() {
    // ASSUMPTION: The schedule is computed ONLY ONCE by the NC, and distributed by the NC to all BRs and MNs. In fact, in this implementation, ALICE
    // does not use a hash function to select the timeslot and channel for the upstream and downstream cells, but allocates an uniformely distributed
    // random cell. This is because I could not find a simple hash() function with a good uniform distribution to be implemented in the simulator.
    // ASSUMPTION: ASNF is not used

    centralizedSchedule = new IEEE802154TSCHSchedule(slotframeLength, numChannels);

    // build upstream cell for MNs
    for (auto mn : ncMid->getMNsMacAddresses()) {
        // upstream cell
        int upTimeslot = intuniform(0, slotframeLength - 1, RNG_RESCHEDULING);
                                                                              
        int upChannel = intuniform(0, numChannels - 1, RNG_RESCHEDULING); 
        // add up cell to schedule
        centralizedSchedule->addCell(upTimeslot, upChannel, mn, BR_BROADCAST_ADDRESS, true, false);

        // downstream cell
        int downTimeslot =
            intuniform(0, slotframeLength - 1, RNG_RESCHEDULING);           
                                                                            
        int downChannel = intuniform(0, numChannels - 1, RNG_RESCHEDULING); 
        // add down cell to schedule
        centralizedSchedule->addCell(downTimeslot, downChannel, BR_BROADCAST_ADDRESS, mn, true, false);
    }
}

std::vector<cell_t> ALICE::manageConflictInTimeslot( int currentTs, std::vector<cell_t> cellsInConflict, mac_address_t myMacAddress, mac_address_t myBroadcastAddress) {
    return basicRandomConflictManagement(cellsInConflict, myMacAddress, myBroadcastAddress);
}