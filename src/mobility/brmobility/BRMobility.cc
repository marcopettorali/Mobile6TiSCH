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

std::vector<Point> parseCustomBrPositions(const char *s);

void BRMobility::initialize(int stage) {
    MobilityBase::initialize(stage);
    if (stage == INITSTAGE_PARAMS_FETCH) {
        brDeploymentPolicy = (const char *)par("brDeploymentPolicy");
        targetCommunicationRadius = (double)par("targetCommunicationRadius");

        crossLayerDB->set("targetCommunicationRadius", targetCommunicationRadius);

        if (strcmp(brDeploymentPolicy, "ListBasedBRMobility") == 0) {
            listBased();
        } else {
            throw cRuntimeError("Unknown deployment algorithm %s", brDeploymentPolicy);
        }

        if (par("throwErrorOnPositionInObstacle").boolValue()) {
            if (environmentManager->isInsideObstacle(currentPosition)) {
                throw cRuntimeError("A BR is inside an obstacle. Set 'throwErrorOnPositionInObstacle' to false to disable this error.");
            }
        }

        // update crossLayerDB
        crossLayerDB->set("currentX", currentPosition.x);
        crossLayerDB->set("currentY", currentPosition.y);
    }

    if (stage == INITSTAGE_GRAPHICS_INIT) {
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("p", 0, currentPosition.x);
        str.setTagArg("p", 1, currentPosition.y);
        str.setTagArg("r", 0, targetCommunicationRadius);
    }
}

void BRMobility::listBased() {
    std::string s = par("listBasedPos");

    if (s.empty()) {
        throw cRuntimeError("ListBasedBRMobility: listBasedPos is empty");
    }

    br_device_id_t brId = (br_device_id_t)getParentModule()->par("brId").intValue();
    std::vector<Point> positions = parseCustomBrPositions(s.c_str());
    currentPosition.x = positions[brId].x;
    currentPosition.y = positions[brId].y;

    crossLayerDB->set("currentPosition.x", currentPosition.x);
    crossLayerDB->set("currentPosition.y", currentPosition.y);
}

std::vector<Point> BRMobility::parseCustomBrPositions(const char *s) {
    std::vector<Point> positions;
    int i = 0;
    int brId = 0;
    while (s[i] != '\0') {
        if (s[i] == '(') {
            i++;
            Point point;
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
            brId++;
            positions.push_back(point);
        } else {
            i++;
        }
    }

    return positions;
}