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

#include "MobilityBase.h"

void MobilityBase::initialize(int stage) {
    BaseModule::initialize(stage);
    if (stage == INITSTAGE_PARAMS_FETCH) {  // init position + init BR positioning

        // retrieve environment manager (it has already been initialized)
        environmentManager = check_and_cast<EnvironmentManagerBase *>(getModuleByPath(par("environmentManager")));

        bool randomInitPosition = par("randomInitPosition");

        // if the initial position is random, generate it
        if (randomInitPosition) {
            initPosition = environmentManager->getRandomPosition();
        } else {
            initPosition = Point(par("initX"), par("initY"));
        }
        currentPosition = initPosition;

        updatePositionInterval = par("updatePositionInterval");
        speed = par("speed");

        currentDirection = Point(0, 0);

        crossLayerDB->set("currentX", currentPosition.x);
        crossLayerDB->set("currentY", currentPosition.y);
        crossLayerDB->set("directionX", currentDirection.x);
        crossLayerDB->set("directionY", currentDirection.y);
        crossLayerDB->set("speed", speed);
    }

    if (stage == INITSTAGE_GRAPHICS_INIT) {
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("p", 0, currentPosition.x);
        str.setTagArg("p", 1, currentPosition.y);
    }
}

void MobilityBase::doStep() {
    currentPosition += currentDirection * speed * updatePositionInterval;
    // update crossLayerDB
    crossLayerDB->set("currentX", currentPosition.x);
    crossLayerDB->set("currentY", currentPosition.y);
}

void MobilityBase::updateGraphics() {
    cDisplayString &str = getParentModule()->getDisplayString();
    str.setTagArg("p", 0, currentPosition.x);
    str.setTagArg("p", 1, currentPosition.y);

    // draw direction arrow
    if (line != nullptr) {
        getParentModule()->getParentModule()->getCanvas()->removeFigure(line);
        delete line;
    }
    line = new cLineFigure("line");
    line->setStart(cFigure::Point(currentPosition.x, currentPosition.y));
    line->setEnd(cFigure::Point(currentPosition.x + currentDirection.x, currentPosition.y + currentDirection.y));
    line->setLineColor("red");
    line->setLineWidth(1);
    line->setEndArrowhead(cFigure::ARROW_SIMPLE);
    line->setZIndex(100);
    getParentModule()->getParentModule()->getCanvas()->addFigure(line);
}