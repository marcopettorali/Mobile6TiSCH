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

#ifndef __MOBILE6TISCH_LITE_MOBILITY_H_
#define __MOBILE6TISCH_LITE_MOBILITY_H_

#include <math.h>
#include <omnetpp.h>

#include "../medium/medium.h"

using namespace omnetpp;

struct point_t {
    double x;
    double y;
};

class Mobility : public cSimpleModule {
    int id;
    int counter;
    cMessage *move_beep;
    double areaWidth, areaHeight;
    double maxXSpeed, maxYSpeed;
    double startX, startY;
    int nextX, nextY;
    double speedX, speedY;
    double updatePositionInterval;

    int mobilityType;
    
    const char *brPositioningPolicy;

    virtual int numInitStages() const { return 5; }

   protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

    void brArrangementPolicy(const char *policy);

    void linearMobility();
    void randomWayPoints();

   public:
    double radius;  // useful for BR positioning policy and for computing positions of mns inside brs
    double currentX, currentY;
    double maxSpeed;
    void count();
    void randomPosition();
};

#endif
