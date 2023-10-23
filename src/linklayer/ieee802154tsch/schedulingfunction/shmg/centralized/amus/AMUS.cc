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

#include "AMUS.h"

Define_Module(AMUS);

void AMUS::initialize(int stage) {
    CentralizedSF::initialize(stage);

    if (stage == INITSTAGE_PARAMS_FETCH) {
        numChannels = par("numChannels");

        if (numChannels < 1) {
            throw cRuntimeError("AMUS::initialize: numChannels must be >= 1");
        }

        if (numChannels > 16) {
            throw cRuntimeError("AMUS::initialize: numChannels must be <= %d", 16);
        }
    }
}

void AMUS::buildCentralizedSchedule() {  // ASSUMPTION: AMUS for single hop architecture, allocates one up cell and one down cell on a single channel offset

    // determine the number of timeslots in the slotframe
    slotframeLength = coprime(2 * ncMid->getMNsMacAddresses().size(), numChannels);

    centralizedSchedule = new IEEE802154TSCHSchedule(slotframeLength, numChannels);

    // start allocating upstream cells
    int currTs = 0;

    // build upstream cell for MNs
    for (auto mn : ncMid->getMNsMacAddresses()) {
        centralizedSchedule->addCell(currTs++, 0, mn, BR_BROADCAST_ADDRESS, false, false);
    }

    // build downstream cell for MNs
    for (auto mn : ncMid->getMNsMacAddresses()) {
        centralizedSchedule->addCell(currTs++, 0, BR_BROADCAST_ADDRESS, mn, false, false);
    }
}

std::vector<cell_t> AMUS::manageConflictInTimeslot( int currentTs, std::vector<cell_t> cellsInConflict, mac_address_t myMacAddress, mac_address_t myBroadcastAddress) {
    return basicRandomConflictManagement(cellsInConflict, myMacAddress, myBroadcastAddress);
}