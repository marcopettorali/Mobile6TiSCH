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

#include "LinearMobility.h"

Define_Module(LinearMobility);

void LinearMobility::initialize(int stage) {
    MobilityBase::initialize(stage);
    if (stage == 0) {
        this->speed = par("speed");
        this->forceVHTrajectories = (bool) par("forceVHTrajectories");

        if (forceVHTrajectories) {
            if (intuniform(0, 1) == 0) {
                this->speedX = (double) speed * updatePositionInterval;
                this->speedY = 0;
            } else {
                this->speedX = 0;
                this->speedY = (double) speed * updatePositionInterval;
            }
        } else {
            // generate random angle
            double angle = uniform(0, 2 * M_PI);
            this->speedX = (double) speed * updatePositionInterval * cos(angle);
            this->speedY = (double) speed * updatePositionInterval * sin(angle);
        }

        move_beep = new cMessage("move_beep");
        scheduleAt(simTime() + updatePositionInterval, move_beep);
    }
}

void LinearMobility::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (currentX + speedX > areaWidth) {
            currentX = areaWidth;
            speedX = -speedX;
        } else if (currentX + speedX < 0) {
            currentX = 0;
            speedX = -speedX;
        } else {
            currentX += speedX;
        }

        if (currentY + speedY > areaHeight) {
            currentY = areaHeight;
            speedY = -speedY;
        } else if (currentY + speedY < 0) {
            currentY = 0;
            speedY = -speedY;
        } else {
            currentY += speedY;
        }

        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("p", 0, currentX);
        str.setTagArg("p", 1, currentY);

        scheduleAt(simTime() + updatePositionInterval, move_beep);
    }
}
