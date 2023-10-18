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

    if (stage == INITSTAGE_PARAMS_FETCH) {
        // compute next position
        computeNextPosition();

        // update graphics
        updateGraphics();
    }
    if (stage == INITSTAGE_BEEP_INIT) {
        // schedule first update of position
        moveBeep = new cMessage("moveBeep");
        scheduleAt(simTime() + updatePositionInterval, moveBeep);
    }
}

void RandomMobility::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (currentPosition == nextPosition) {
            // destination reached, choose new one
            computeNextPosition();
        }

        // update position
        doStep();

        // schedule next update
        scheduleAt(simTime() + updatePositionInterval, moveBeep);
    } else {
        MobilityBase::handleMessage(msg);
    }
}

void RandomMobility::computeNextPosition() {
    // choose a random direction
    // Randomize coordinates and keep modulus 1
    double angle = (double)uniform(0, 1, RNG_MOBILITY_SHUFFLE) * 2 * M_PI;
    currentDirection = Point(cos(angle), sin(angle));

    // compute closest intersections with obstacles and walls along this direction
    Point intersection = environmentManager->getClosestIntersection(currentPosition, currentDirection);

    // choose a random alpha between 0 and 1
    double alpha = uniform(0, 0.9, RNG_MOBILITY_SHUFFLE);

    // compute the next position as the intersection + alpha * distance(currentPosition, intersection)
    nextPosition = currentPosition + (intersection - currentPosition) * alpha;

    // solve numerical errors
    if (nextPosition.distance(intersection) <= POINT_PRECISION_M) {
        nextPosition = intersection;
    }

    // signal that direction has changed
    hasDirectionChangedFlag = true;

    // update crossLayerDB
    crossLayerDB->set("directionX", currentDirection.x);
    crossLayerDB->set("directionY", currentDirection.y);
}

void RandomMobility::doStep() {  // ASSUMPTION: no obstacles can be in the way i.e. no obstacles are added after the nextPosition has been computed

    // if the step is longer than the distance to the next position, then the next position is reached
    double step = (currentDirection * speed * updatePositionInterval).norm();

    if (currentPosition.distance(nextPosition) < step) {
        currentPosition = nextPosition;
    } else {
        MobilityBase::doStep();
    }

    // update graphics
    updateGraphics();

    // update crossLayerDB
    crossLayerDB->set("currentX", currentPosition.x);
    crossLayerDB->set("currentY", currentPosition.y);
}
