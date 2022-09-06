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

#include "RandomMobility.h"

Define_Module(RandomMobility);

void RandomMobility::initialize(int stage) {
    MobilityBase::initialize(stage);

    if (stage == 0) {
        this->nextX = intuniform(0, areaWidth, 1);
        this->nextY = intuniform(0, areaHeight, 1);

        this->speed = par("speed");
        this->speedInterval = par("speedInterval");
        if(speedInterval == -1) speedInterval = 2 * speed;

        // choose a random speed
        speed = uniform(speed - speedInterval/2, speed + speedInterval / 2, 2);
        speedX = speed * (nextX - startX) / sqrt(pow(nextX - startX, 2) + pow(nextY - startY, 2)) * updatePositionInterval;
        speedY = speed * (nextY - startY) / sqrt(pow(nextX - startX, 2) + pow(nextY - startY, 2)) * updatePositionInterval;

        move_beep = new cMessage("move_beep");
        scheduleAt(simTime() + updatePositionInterval, move_beep);
    }
}

void RandomMobility::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (round(currentX) == nextX && round(currentY) == nextY) {
            startX = currentX;
            startY = currentY;
            nextX = intuniform(0, areaWidth, 1);
            nextY = intuniform(0, areaHeight, 1);

            speed = uniform(speed - speedInterval/2, speed + speedInterval / 2, 2);
            speedX = speed * (nextX - currentX) / DIST(nextX, nextY, currentX, currentY) * updatePositionInterval;
            speedY = speed * (nextY - currentY) / DIST(nextX, nextY, currentX, currentY) * updatePositionInterval;
        }

        if (DIST(currentX, currentY, nextX, nextY) < speed) {
            currentX = nextX;
            currentY = nextY;
        } else {
            currentX = currentX + speedX;
            currentY = currentY + speedY;
        }

        EV << speed << "m/s, (" << currentX << "," << currentY << ")" << endl;

        // update graphics
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("p", 0, currentX);
        str.setTagArg("p", 1, currentY);

        scheduleAt(simTime() + updatePositionInterval, move_beep);
    }
}
