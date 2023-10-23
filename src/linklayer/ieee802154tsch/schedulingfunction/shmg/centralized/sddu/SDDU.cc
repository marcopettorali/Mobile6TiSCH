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

#include "SDDU.h"

Define_Module(SDDU);

void SDDU::initialize(int stage) {
    CentralizedSF::initialize(stage);

    if (stage == INITSTAGE_PARAMS_FETCH) {
        g = par("g");  // ASSUMPTION: G <= 16

        if (g < 1) {
            throw cRuntimeError("SDDU::initialize: g must be >= 1");
        }
    }
}

void SDDU::buildCentralizedSchedule() {
    bool dddu = (g == 1);

    int numMN = ncMid->getNumMNs();

    // determine slotframeLength
    int slotframeLength = coprime(1 + ceil((double)numMN / g) + numMN, g);

    // initialize schedule of size slotframeLength
    centralizedSchedule = new IEEE802154TSCHSchedule(slotframeLength, std::max(16, g));

    // put shared cells in the first timeslot
    centralizedSchedule->addCell(0, 0, MN_BROADCAST_ADDRESS, BR_BROADCAST_ADDRESS, false, true);

    // determine first upstream and downstream cells

    int nextUpCh, nextUpSlot, nextDownCh, nextDownSlot;
    if (dddu) {  // dddu
        nextUpCh = 0;
        nextUpSlot = 1;
        nextDownCh = 0;
        nextDownSlot = 2;
    } else {  // sddu
        nextUpCh = 0;
        nextUpSlot = 1 + ceil((double)numMN / g);
        nextDownCh = 0;
        nextDownSlot = 1;
    }

    // for each entry in mns
    for (int i = 0; i < numMN; i++) {
        int macAddress = ncMid->getMNsMacAddresses()[i];

        // allocate up cell
        centralizedSchedule->addCell(nextUpSlot, nextUpCh, macAddress, BR_BROADCAST_ADDRESS, false, false);
        if (dddu) {
            nextUpSlot += 2;
        } else {
            nextUpSlot += 1;
        }

        // allocate down cell
        centralizedSchedule->addCell(nextDownSlot, nextDownCh, BR_BROADCAST_ADDRESS, macAddress, false, false);

        if (dddu) {
            nextDownCh = 0;
            nextDownSlot += 2;

        } else {  // SDDU
            nextDownCh = (nextDownCh + 1) % g;
            if (nextDownCh == 0) {
                nextDownSlot++;
            }
        }
    }
}

std::vector<cell_t> SDDU::manageConflictInTimeslot( int currentTs, std::vector<cell_t> cellsInConflict, mac_address_t myMacAddress, mac_address_t myBroadcastAddress) {
    return basicRandomConflictManagement(cellsInConflict, myMacAddress, myBroadcastAddress);
}