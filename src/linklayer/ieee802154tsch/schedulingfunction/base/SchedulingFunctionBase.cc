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

#include "SchedulingFunctionBase.h"

Define_Module(SchedulingFunctionBase);

std::vector<cell_t> SchedulingFunctionBase::basicRandomConflictManagement(std::vector<cell_t> cellsInConflict, mac_address_t myMacAddress,
                                                                          mac_address_t myBroadcastAddress) {
    std::vector<cell_t> ret;

    // select all tx cellsInConflict and one random rx cell
    std::vector<cell_t> txCells;
    std::vector<cell_t> rxCells;

    for (auto cell : cellsInConflict) {
        if (cell.tx == myMacAddress || cell.tx == myBroadcastAddress) {
            txCells.push_back(cell);
        }

        if (cell.rx == myMacAddress || cell.rx == myBroadcastAddress) {
            rxCells.push_back(cell);
        }
    }

    // push back all tx cellsInConflict
    for (auto cell : txCells) {
        ret.push_back(cell);
    }

    // push back one random rx cell
    if (rxCells.size() > 0) {
        ret.push_back(rxCells.at(intuniform(0, rxCells.size() - 1, RNG_SHUFFLE)));
    }

    return ret;
}