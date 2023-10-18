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
    if (stage == INITSTAGE_PARAMS_FETCH) {
        this->forceVHTrajectories = (bool)par("forceVHTrajectories");

        if (forceVHTrajectories) {
            // randomly choose horizontal or vertical trajectory
            if (intuniform(0, 1, RNG_MOBILITY_SHUFFLE) == 0) {
                // generate horizontal trajectory (right or left)
                currentDirection = Point(intuniform(0, 1, RNG_MOBILITY_SHUFFLE) * 2 - 1, 0);
            } else {
                // generate vertical trajectory (up or down)
                currentDirection = Point(0, intuniform(0, 1, RNG_MOBILITY_SHUFFLE) * 2 - 1);
            }
        } else {
            // generate random angle and MN follows this angle, then bounces
            double angle = uniform(0, 2 * M_PI, RNG_MOBILITY_SHUFFLE);
            currentDirection = Point(cos(angle), sin(angle));
            currentDirection.normalize();
        }

        // set initial direction
        crossLayerDB->set("directionX", currentDirection.x);
        crossLayerDB->set("directionY", currentDirection.y);
    }

    if (stage == INITSTAGE_BEEP_INIT) {
        // schedule first move
        moveBeep = new cMessage("moveBeep");
        scheduleAt(simTime() + updatePositionInterval, moveBeep);
    }
}

void LinearMobility::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        hasDirectionChangedFlag = false;

        // update position
        doStep();

        // schedule next update
        scheduleAt(simTime() + updatePositionInterval, moveBeep);
    }
}

void LinearMobility::doStep() {
    // get closest intersection with obstacles
    Point intersection = environmentManager->getClosestIntersection(currentPosition, currentDirection);

    double distance = intersection.distance(currentPosition);

    double step = (currentDirection * speed * updatePositionInterval).norm();

    bool sig= false;
    double rem = 0;
    // check if it would walk past the intersection
    if (step >= distance) {
        sig = true;
        // compute remainingDistance as distance traveled before intersection
        double remainingDistance = step - distance;
        rem = remainingDistance;

        // intersection is closer, so move to intersection
        currentPosition = intersection;

        // invert direction
        currentDirection = Point(0, 0) - currentDirection;
        hasDirectionChangedFlag = true;

        // walk for remainingDistance
        if (remainingDistance > 0) {
            // update position
            currentPosition += currentDirection * remainingDistance;
        }else{  
            // if remaining distance is 0, just do a larger step to avoid being stuck in an obstacle
            currentPosition += currentDirection * (remainingDistance + 0.1);
        }

    } else {
        // intersection is further away, so move
        MobilityBase::doStep();
    }

    if (environmentManager->isInsideObstacle(currentPosition)) {
        std::string err = "MN is inside an obstacle! (";
        err += std::to_string(currentPosition.x);
        err += ", ";
        err += std::to_string(currentPosition.y);
        err += ")";
        err += " sig = " + std::to_string(sig);
        err += " rem = " + std::to_string(rem);
        
        throw cRuntimeError(err.c_str());
    }

    // update graphics
    updateGraphics();

    // update crossLayerDB
    crossLayerDB->set("currentX", currentPosition.x);
    crossLayerDB->set("currentY", currentPosition.y);
    crossLayerDB->set("directionX", currentDirection.x);
    crossLayerDB->set("directionY", currentDirection.y);
}
