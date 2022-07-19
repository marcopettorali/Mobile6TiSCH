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

#include "mobility.h"

#define INTERSECTING_FLOWERS_ALPHA 0

Define_Module(Mobility);

vector<point_t> parseCustomBrPositions(const char* s);

void Mobility::initialize(int stage) {
    if (stage == 0) {  // init position + init BR positioning
        id = getParentModule()->getSubmodule("radio")->par("macAddress");

        updatePositionInterval = par("updatePositionInterval");
        mobilityType = par("mobilityType");

        // area dimensions
        areaWidth = par("areaWidth");
        areaHeight = (double)par("areaHeight") == -1   ? areaWidth
                     : (double)par("areaHeight") == -2 ? areaWidth / 2
                                                       : (double)par("areaHeight");

        radius = par("radius");

        maxXSpeed = (double)par("maxXSpeed");
        maxYSpeed = (double)par("maxYSpeed");
        maxSpeed = sqrt(maxXSpeed * maxXSpeed + maxYSpeed * maxYSpeed);

        if (mobilityType != 0) {
            switch (mobilityType) {
                case 1:  // LINEAR MOBILITY
                    if (maxYSpeed == -1) {
                        if (intuniform(0, 1) == 0) {
                            this->speedX = (double)maxXSpeed * updatePositionInterval;
                            this->speedY = 0;
                        } else {
                            this->speedX = 0;
                            this->speedY = (double)maxXSpeed * updatePositionInterval;
                        }
                    } else {
                        this->speedX = (double)maxXSpeed * updatePositionInterval;
                        this->speedY = (double)maxYSpeed * updatePositionInterval;
                    }
                    break;
                case 2:  // RANDOM WAYPOINTS
                    if (maxYSpeed == -1) {
                        maxYSpeed = maxXSpeed;
                    }
                    this->nextX = intuniform(0, areaWidth);
                    this->nextY = intuniform(0, areaHeight);

                    this->speedX = uniform(0, maxXSpeed) * updatePositionInterval;
                    this->speedY = uniform(0, maxYSpeed) * updatePositionInterval;
                    break;
            }
            move_beep = new cMessage("move_beep");
            scheduleAt(simTime() + updatePositionInterval, move_beep);
        }

        counter = 0;

        bool autoPosition = par("autoPosition");
        if (autoPosition == true) {
            this->brPositioningPolicy = par("positioningPolicy");
            brArrangementPolicy(brPositioningPolicy);
        } else {
            cDisplayString &str = getParentModule()->getDisplayString();
            this->startX = (double)(par("startX"));
            this->startY = (double)(par("startY"));
            str.setTagArg("p", 0, startX);
            str.setTagArg("p", 1, startY);
            this->currentX = startX;
            this->currentY = startY;
        }
    }
}

void Mobility::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        switch (mobilityType) {
            case 1:
                linearMobility();
                break;
            case 2:
                randomWayPoints();
                break;
        }

        scheduleAt(simTime() + updatePositionInterval, move_beep);
    }
}

void Mobility::linearMobility() {
    cDisplayString &str = getParentModule()->getDisplayString();

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

    str.setTagArg("p", 0, currentX);
    str.setTagArg("p", 1, currentY);
}

void Mobility::randomWayPoints() {
    cDisplayString &str = getParentModule()->getDisplayString();

    if (round(currentX) == round(nextX) && round(currentY) == round(nextY)) {
        startX = currentX;
        startY = currentY;
        nextX = intuniform(0, areaWidth);
        nextY = intuniform(0, areaHeight);
        speedX = uniform(0, (double)maxXSpeed) * updatePositionInterval;
        speedY = uniform(0, (double)maxYSpeed) * updatePositionInterval;
    }

    double stepX, stepY;
    if (currentX < nextX) {
        if (currentX + speedX <= nextX) {
            stepX = speedX;
        } else {
            stepX = nextX - currentX;
        }
    } else if (currentX > nextX) {
        if (currentX - speedX >= nextX) {
            stepX = -speedX;
        } else {
            stepX = nextX - currentX;
        }
    } else {
        stepX = 0;
    }

    if (currentY < nextY) {
        if (currentY + speedY <= nextY) {
            stepY = speedY;
        } else {
            stepY = nextY - currentY;
        }
    } else if (currentY > nextY) {
        if (currentY - speedY >= nextY) {
            stepY = -speedY;
        } else {
            stepY = nextY - currentY;
        }
    } else {
        stepY = 0;
    }

    currentX = currentX + stepX;
    currentY = currentY + stepY;

    str.setTagArg("p", 0, currentX);
    str.setTagArg("p", 1, currentY);
}

void Mobility::brArrangementPolicy(const char *policy) {
    if (strcmp(policy, "intersecting_flower") == 0) {
        double baseTr = sqrt(3) * radius;
        double heightTr = ((double)3 / 2) * radius;

        int numSpacesY = (int)std::floor((double)areaHeight / heightTr);
        int strips = numSpacesY + 1;
        double marginTop = (double)(areaHeight - heightTr * numSpacesY) / 2;
        double spacingY = heightTr;

        int numSpacesX = (int)std::floor((double)areaWidth / baseTr);
        int nX = numSpacesX + 1;
        double spacingX = (double)areaWidth / numSpacesX;

        int holes = (2 * nX - 3) * (strips - 1) + 2 * nX + strips - 1;
        int eps = (int)std::floor((double)sqrt(sqrt(3) * INTERSECTING_FLOWERS_ALPHA * areaWidth * (double)areaHeight / holes));

        if (spacingX > 2 * radius ||
            spacingY > radius + (double)sqrt(4 * radius * radius - spacingX * spacingX) / 2 + eps) {
            numSpacesX++;
            nX++;
            spacingX = (double)areaWidth / numSpacesX;
        }

        int currId = 0;
        bool found = false;
        for (int i = 0; i < strips; i++) {
            if (found) {
                break;
            }
            if (i % 2 == 0) {
                for (int j = 0; j < nX; j++) {
                    if (id == currId) {
                        currentX = (double)j * spacingX;
                        currentY = (double)marginTop + i * spacingY;
                        found = true;
                        break;
                    }
                    currId++;
                }
            } else {
                for (int j = 0; j < nX - 1; j++) {
                    if (id == currId) {
                        currentX = (double)spacingX / 2 + j * spacingX;
                        currentY = (double)marginTop + i * spacingY;
                        found = true;
                        break;
                    }
                    currId++;
                }
            }
            EV << "currId = " << currId << endl;
            EV << "currentX = " << currentX << endl;
            EV << "currentY = " << currentY << endl;
        }

    } else if (strcmp(policy, "old_intersecting_flower") == 0) {
        double effectiveRadiusX;
        double effectiveRadiusY = (double)radius * 3 / 2;

        int rowSize = floor((double)areaWidth / (radius * sqrt(3))) + 1;
        int layers = floor((double)areaHeight / effectiveRadiusY) + 1;

        if (rowSize == 1 && layers == 2) {
            if (id == 0) {
                currentX = 0;
                currentY = areaHeight / 2;
            } else if (id == 1) {
                currentX = areaWidth;
                currentY = areaHeight / 2;
            }
        }

        if (rowSize > 1 && ((double)areaWidth / (rowSize - 1)) / radius >= 1.85) {
            rowSize++;
        }

        effectiveRadiusX = (double)areaWidth / (2 * (rowSize - 1));

        double marginY = (double)(areaHeight - effectiveRadiusY * (layers - 1)) / 2.0;

        int currId = 0;
        bool found = false;
        for (int i = 0; i < layers; i++) {
            if (found == true) {
                break;
            }
            if (i % 2 == 0) {
                for (int j = 0; j < rowSize; j++) {
                    if (id == currId) {
                        currentX = (double)j * 2 * effectiveRadiusX;
                        currentY = (double)marginY + i * effectiveRadiusY;
                        EV << "currId = " << currId << endl;
                        found = true;
                        break;
                    }
                    currId++;
                }
            } else {
                for (int j = 0; j < rowSize - 1; j++) {
                    if (id == currId) {
                        currentX = (double)effectiveRadiusX + j * 2 * effectiveRadiusX;
                        currentY = (double)marginY + i * effectiveRadiusY;
                        found = true;
                        break;
                    }
                    currId++;
                }
            }
        }

        if (areaWidth < 0.75 * 2 * radius) {
            currentX = (double)areaWidth / 2;
        }

        if (areaHeight < 0.75 * 2 * radius) {
            currentY = (double)areaHeight / 2;
        }

        if (!found) {
            currentX = 10000;
            currentY = 10000;
        }
    } else if (strcmp(policy, "custom") == 0) {
        vector<point_t> positions = parseCustomBrPositions(par("customBrPositions"));
        currentX = positions[id].x;
        currentY = positions[id].y;
    } else {
        throw cRuntimeError("Unsupported arrangement policy");
    }

    cDisplayString &str = getParentModule()->getDisplayString();
    str.setTagArg("p", 0, currentX);
    str.setTagArg("p", 1, currentY);
}

void Mobility::count() {
    Enter_Method_Silent("count()");
    counter++;
}

void Mobility::randomPosition() {
    Enter_Method_Silent("moveOnGrid()");

    if (counter == 10000) {
        endSimulation();
    }

    currentX = uniform(0, areaWidth);
    currentY = uniform(0, areaHeight);

    cDisplayString &str = getParentModule()->getDisplayString();
    str.setTagArg("p", 0, currentX);
    str.setTagArg("p", 1, currentY);
}

vector<point_t> parseCustomBrPositions(const char* s){
    vector<point_t> positions;
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == '(') {
            i++;
            point_t point;
            point.x = atof(s + i);
            while (s[i] != ',') {
                i++;
            }
            i++;
            point.y = atof(s + i);
            while (s[i] != ')') {
                i++;
            }
            i++;
            positions.push_back(point);
        } else {
            i++;
        }
    }

    // show positions
    EV << "Custom positions: ";
    for (int i = 0; i < positions.size(); i++) {
        EV << "(" << positions[i].x << "," << positions[i].y << ") ";
    }
    EV << endl;
    

    return positions;
}