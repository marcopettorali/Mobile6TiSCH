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

#include "common/basemodule/BaseModule.h"
#include "environment/base/EnvironmentManagerBase.h"
#include "sensors/orientation/OrientationSensor.h"
#include "sensors/position/PositionSensor.h"
#include "utils/point/Point.h"
#include "utils/point/Polygon.h"
#include "utils/point/Segment.h"

using namespace omnetpp;

class MobilityBase : virtual public BaseModule {
    friend class PositionSensor;
    friend class OrientationSensor;

   protected:
    // STATE
    bool hasDirectionChangedFlag = false;

    bool randomInitPosition;

    // points
    Point initPosition;
    Point currentPosition;
    Point currentDirection;

    double maxWidth;
    double maxHeight;
    double updatePositionInterval;
    double speed;

    // graphics
    cLineFigure* line = nullptr;

    // MODULES
    EnvironmentManagerBase* environmentManager;

    // METHODS
    virtual void initialize(int stage) override;

    // utility
    void updateGraphics();

    virtual void doStep();

   public:
    // METHODS
    bool hasDirectionChanged() {
        bool ret = hasDirectionChangedFlag;
        hasDirectionChangedFlag = false;
        return ret;
    }

    // getters
    Point getInitPosition() const { return initPosition; }
    Point getCurrentPosition() const { return currentPosition; }
    Point getCurrentDirection() const { return currentDirection; }

};

#endif
