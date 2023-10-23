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

#ifndef __MOBILE6TISCH_RANDOMMOBILITY_H_
#define __MOBILE6TISCH_RANDOMMOBILITY_H_

#include <omnetpp.h>

#include "mobility/base/MobilityBase.h"

using namespace omnetpp;

class RandomMobility : virtual public MobilityBase {
   protected:
    // STATE
    Point nextPosition;

    // BEEP
    cMessage *moveBeep;

    // METHODS
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;

    // utility
    void computeNextPosition();

    virtual void doStep() override;
};

#endif
