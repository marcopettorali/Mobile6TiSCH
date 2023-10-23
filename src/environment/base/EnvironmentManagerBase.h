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

#ifndef __MOBILE6TISCH_ENVIRONMENTMANAGERBASE_H_
#define __MOBILE6TISCH_ENVIRONMENTMANAGERBASE_H_

#include <omnetpp.h>
#include <vector>

#include "common/basemodule/BaseModule.h"
#include "utils/point/Polygon.h"

using namespace omnetpp;

class EnvironmentManagerBase : virtual public BaseModule {
    double inftyDistance;
   protected:
    // STATE
    double maxWidth;
    double maxHeight;

    // background image (if any)
    std::string backgroundImagePath;

    // input string of obstacles
    std::string obstaclesStr;

    // collection of obstacles
    std::vector<Polygon> obstacles;

    // METHODS
    virtual void initialize(int stage) override;

    // utility
    void buildWalls();

    void parseObstacles(std::string obstacles);
    void printObstacles();
    void drawObstacles();


   public:
    bool isInsideObstacle(Point p);
    // METHODS
    // compute all intersections of a line with the obstacles
    std::vector<Point> getIntersections(Point start, Point currentDirection);
    Point getClosestIntersection(Point start, Point currentDirection);

    // return true if the segment intersects an obstacle
    bool isIntersecting(Segment segment);

    // compute random position inside the area (outside the obstacles)
    virtual Point getRandomPosition();

    // getters
    double getMaxWidth(){return maxWidth;}
    double getMaxHeight(){return maxHeight;}

};

#endif
