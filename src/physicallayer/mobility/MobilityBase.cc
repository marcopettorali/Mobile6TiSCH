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

#include "MobilityBase.h"

MobilityBase::MobilityBase() {}

MobilityBase::~MobilityBase() {
    // TODO Auto-generated destructor stub
}

void MobilityBase::initialize(int stage) {
    if (stage == 0) {  // init position + init BR positioning
        this->startX = (double)(par("startX"));
        this->startY = (double)(par("startY"));
        this->areaWidth = (double)(par("areaWidth"));
        this->areaHeight = (double)(par("areaHeight"));
        this->updatePositionInterval = par("updatePositionInterval");

        this->currentX = startX;
        this->currentY = startY;

        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("p", 0, startX);
        str.setTagArg("p", 1, startY);
    }
}
