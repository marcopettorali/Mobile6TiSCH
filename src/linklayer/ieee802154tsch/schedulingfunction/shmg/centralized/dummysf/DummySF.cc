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

#include "DummySF.h"

Define_Module(DummySF);

void DummySF::buildCentralizedSchedule() {
    int slotframeLength = 11;
    int numChannels = 16;

    centralizedSchedule = new IEEE802154TSCHSchedule(slotframeLength, numChannels);

    centralizedSchedule->addCell(0, 0, 20000, BR_BROADCAST_ADDRESS, true, false);
    centralizedSchedule->addCell(0, 0, 20001, BR_BROADCAST_ADDRESS, true, false);
}

std::vector<cell_t> DummySF::manageConflictInTimeslot( int currentTs, std::vector<cell_t> cellsInConflict, mac_address_t myMacAddress, mac_address_t myBroadcastAddress) {
    return basicRandomConflictManagement(cellsInConflict, myMacAddress, myBroadcastAddress);
}