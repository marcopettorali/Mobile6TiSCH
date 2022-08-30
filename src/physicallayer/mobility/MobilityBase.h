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

#ifndef PHYSICALLAYER_MOBILITY_MOBILITYBASE_H_
#define PHYSICALLAYER_MOBILITY_MOBILITYBASE_H_

#include <omnetpp.h>
#include <omnetpp/csimplemodule.h>

#define DIST(x1, y1, x2, y2) sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))

using namespace omnetpp;

class MobilityBase : public cSimpleModule {
   protected:
    double startX;
    double startY;
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg){};

   public:
    double currentX, currentY;
    double areaWidth;
    double areaHeight;
    double updatePositionInterval;

    MobilityBase();
    virtual ~MobilityBase();
};

#endif /* PHYSICALLAYER_MOBILITY_MOBILITYBASE_H_ */
