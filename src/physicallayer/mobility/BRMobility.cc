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

#include "BRMobility.h"

#include <string.h>

Define_Module(BRMobility);

std::vector<point_t> parseCustomBrPositions(const char *s);

void BRMobility::initialize(int stage) {
    MobilityBase::initialize(stage);
    if (stage == 0) {
        brDeploymentPolicy = (const char*) par("brDeploymentPolicy");
        brId = getParentModule()->par("brId");
        targetCommunicationRadius = (double) par("targetCommunicationRadius");

        if (strcmp(brDeploymentPolicy, "IntFlowersBRMobility") == 0) {
            intFlowersAlpha = (double) par("intFlowersAlpha");
            intersectingFlowers();
        } else if (strcmp(brDeploymentPolicy, "ListBasedBRMobility") == 0) {
            listBased();
        } else {
            throw cRuntimeError("Unknown deployment algorithm");
        }

        // update graphics
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("p", 0, currentX);
        str.setTagArg("p", 1, currentY);
        str.setTagArg("r", 0, targetCommunicationRadius);

    }
}

void BRMobility::intersectingFlowers() {
    double baseTr = sqrt(3) * targetCommunicationRadius;
    double heightTr = ((double) 3 / 2) * targetCommunicationRadius;

    int numSpacesY = (int) std::floor((double) areaHeight / heightTr);
    int strips = numSpacesY + 1;
    double marginTop = (double) (areaHeight - heightTr * numSpacesY) / 2;
    double spacingY = heightTr;

    int numSpacesX = (int) std::floor((double) areaWidth / baseTr);
    int nX = numSpacesX + 1;
    double spacingX = (double) areaWidth / numSpacesX;

    int holes = (2 * nX - 3) * (strips - 1) + 2 * nX + strips - 1;
    int eps = (int) std::floor(
            (double) sqrt(
                    sqrt(3) * intFlowersAlpha * areaWidth * (double) areaHeight
                            / holes));

    if (spacingX > 2 * targetCommunicationRadius
            || spacingY
                    > targetCommunicationRadius
                            + (double) sqrt(
                                    4 * targetCommunicationRadius
                                            * targetCommunicationRadius
                                            - spacingX * spacingX) / 2 + eps) {
        numSpacesX++;
        nX++;
        spacingX = (double) areaWidth / numSpacesX;
    }

    int currId = 0;
    bool found = false;
    for (int i = 0; i < strips; i++) {
        if (found) {
            break;
        }
        if (i % 2 == 0) {
            for (int j = 0; j < nX; j++) {
                if (brId == currId) {
                    currentX = (double) j * spacingX;
                    currentY = (double) marginTop + i * spacingY;
                    found = true;
                    break;
                }
                currId++;
            }
        } else {
            for (int j = 0; j < nX - 1; j++) {
                if (brId == currId) {
                    currentX = (double) spacingX / 2 + j * spacingX;
                    currentY = (double) marginTop + i * spacingY;
                    found = true;
                    break;
                }
                currId++;
            }
        }
    }
}

void BRMobility::listBased() {
    const char* s = par("listBasedPos");
    EV << s << endl;
    std::vector<point_t> positions = parseCustomBrPositions(s);
    currentX = positions[brId].x;
    currentY = positions[brId].y;
}

std::vector<point_t> parseCustomBrPositions(const char *s) {
    std::vector<point_t> positions;
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

    return positions;
}

void BRMobility::handleMessage(cMessage *msg) {
    // TODO - Generated method body
}
