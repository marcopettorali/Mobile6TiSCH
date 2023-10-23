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

#include "GlobalRadioRegister.h"

Define_Module(GlobalRadioRegister);

void GlobalRadioRegister::initialize(int stage) {
    if (stage == INITSTAGE_MODULES_FETCH) {
        statsCollector =
            check_and_cast<StatsCollector *>(getModuleByPath("statsCollector"));
        statsCollector->registerCollectable(this);
    }
}

void GlobalRadioRegister::finishCollectStats() {
    for (auto entry : transmissionStats) {
        std::string statName = std::string("radioPacketOutcomes");
        statName += std::to_string(entry.second);
        statsCollector->record(0, statName, 1);
    }
}