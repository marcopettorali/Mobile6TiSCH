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

#include "EnvironmentManagerBase.h"

Define_Module(EnvironmentManagerBase);

void EnvironmentManagerBase::initialize(int stage) {
    if (stage == INITSTAGE_ENVIRONMENT) {
        maxWidth = par("maxWidth");
        maxHeight = par("maxHeight");

        backgroundImagePath = par("backgroundImagePath").stdstringValue();

        if (backgroundImagePath != "") {
            // draw background image
            getParentModule()->getDisplayString().setTagArg("bgi", 0, backgroundImagePath.c_str());
        }

        // update inftyDist as the largest possible distance in the area
        inftyDistance = Point(0, 0).distance(Point(maxWidth, maxHeight));

        // build walls
        buildWalls();

        // get obstacles string
        obstaclesStr = par("obstacles").stdstringValue();

        // parse obstacles
        parseObstacles(obstaclesStr);

        // draw obstacles
        drawObstacles();

        // print obstacles
        printObstacles();
    }
}

void EnvironmentManagerBase::buildWalls() {
    // build 4 polygons (lines) corresponding to the walls of the area
    // so that MNs will automatically bounce on them

    obstacles.push_back(Polygon({Point(0, 0), Point(maxWidth, 0)}));
    obstacles.push_back(Polygon({Point(maxWidth, 0), Point(maxWidth, maxHeight)}));
    obstacles.push_back(Polygon({Point(maxWidth, maxHeight), Point(0, maxHeight)}));
    obstacles.push_back(Polygon({Point(0, maxHeight), Point(0, 0)}));
}

void EnvironmentManagerBase::parseObstacles(std::string str) {
    // if string is empty, do nothing
    if (str == "") {
        INF("environmentManager") << "No obstaclesStr specified." << std::endl;
        return;
    }

    // parse str formatted as follows:
    // [(x1,y1),(x2,y2)],[(x1,y1),(x2,y2),(x3,y3)],...

    // use a state-machine approach to parse the string
    std::string pointString = "";
    Point currPoint;
    Polygon currPolygon;
    bool inObstacle = false;
    bool rectMode = false;

    for (int i = 0; i < str.length(); i++) {
        char c = str[i];

        switch (c) {
            case '[':
                inObstacle = true;
                pointString = "";
                break;
            case ']':
                inObstacle = false;

                // if rectMode is true, then the polygon is a rectangle
                if (rectMode) {
                    // parse pointString as x,y,w,h
                    double x, y, w, h;
                    sscanf(pointString.c_str(), "%lf,%lf,%lf,%lf", &x, &y, &w, &h);

                    // add points
                    currPolygon.addPoint(Point(x, y));
                    currPolygon.addPoint(Point(x + w, y));
                    currPolygon.addPoint(Point(x + w, y + h));
                    currPolygon.addPoint(Point(x, y + h));

                    // reset rectMode
                    rectMode = false;
                }

                // order points in the polygon in clockwise order
                currPolygon.orderPoints();
                obstacles.push_back(currPolygon);
                currPolygon.clear();
                break;
            case '(':
                pointString = "";
                break;
            case ')':
                sscanf(pointString.c_str(), "%lf,%lf", &currPoint.x, &currPoint.y);
                currPolygon.addPoint(currPoint);
                break;
            case ',':
                if (inObstacle) {
                    pointString += ",";
                }
                break;
            case 'r':
                rectMode = true;
                break;
            case ' ':
                // ignore
                break;
            default:
                pointString += c;
                break;
        }
    }
}

void EnvironmentManagerBase::printObstacles() {
    DBG("environmentManager") << "Obstacles:" << std::endl;
    bool firstObstacle = true;
    for (auto obstacle : obstacles) {
        if (!firstObstacle) {
            EV << ", ";
        }
        EV << "[";
        bool firstPoint = true;
        for (auto point : obstacle.getPoints()) {
            if (!firstPoint) {
                EV << ", ";
            }
            EV << point;
            firstPoint = false;
        }
        EV << "]";
        firstObstacle = false;
    }
    EV << std::endl;
}

void EnvironmentManagerBase::drawObstacles() {
    for (auto polygon : obstacles) {
        // create figure
        cPolygonFigure *polygonFig = new cPolygonFigure("polygon");
        for (auto point : polygon.getPoints()) {
            polygonFig->addPoint(cFigure::Point(point.x, point.y));
        }

        // set style
        polygonFig->setLineColor(cFigure::Color("black"));
        polygonFig->setLineWidth(2);
        polygonFig->setFilled(true);
        polygonFig->setFillColor(cFigure::Color("black"));
        polygonFig->setFillOpacity(0.9);

        // append to parent's canvas
        getParentModule()->getCanvas()->addFigure(polygonFig);
    }
}

bool EnvironmentManagerBase::isInsideObstacle(Point p) {
    for (auto obstacle : obstacles) {
        if (obstacle.isPointInside(p)) {
            return true;
        }
    }
    return false;
}

Point EnvironmentManagerBase::getRandomPosition() {
    // get random position
    Point randomPosition;
    do {
        randomPosition.x = uniform(0, maxWidth, RNG_MOBILITY_SHUFFLE);
        randomPosition.y = uniform(0, maxHeight, RNG_MOBILITY_SHUFFLE);
    } while (isInsideObstacle(randomPosition));

    return randomPosition;
}

std::vector<Point> EnvironmentManagerBase::getIntersections(Point start, Point currentDirection) {
    // compute a line segment that goes from start to infinity in the currentDirection of currentDirection
    Point end = start + currentDirection * inftyDistance;
    Segment trajectory = Segment(start, end);

    std::vector<Point> ret;

    // for each obstacle
    for (auto obstacle : obstacles) {
        // for each segment
        for (auto segment : obstacle.getSegments()) {
            // compute intersection
            Point intersection;
            bool hasIntersection = trajectory.intersection(segment, intersection);

            // if intersection exists
            if (hasIntersection) {
                // add it to the list
                ret.push_back(intersection);
            }
        }
    }

    if (ret.size() == 0) {
        throw cRuntimeError("No intersection found (at least the walls should be there)");
    }

    return ret;
}

Point EnvironmentManagerBase::getClosestIntersection(Point start, Point currentDirection) {
    std::vector<Point> intersections = getIntersections(start, currentDirection);

    // compute closest intersection
    Point closestIntersection;
    double minDistance = inftyDistance;
    for (auto intersection : intersections) {
        double distance = start.distance(intersection);
        // exclude the start point
        if (distance == 0) {
            continue;
        }
        if (distance < minDistance) {
            minDistance = distance;
            closestIntersection = intersection;
        }
    }

    return closestIntersection;
}

bool EnvironmentManagerBase::isIntersecting(Segment segment) {
    // for each obstacle
    for (auto obstacle : obstacles) {
        // for each segment
        for (auto obstacleSegment : obstacle.getSegments()) {
            // compute intersection
            Point intersection;
            bool hasIntersection = segment.intersection(obstacleSegment, intersection);

            // if intersection exists
            if (hasIntersection) {
                EV << "intersection found: " << intersection << endl;
                EV << "segment: " << segment << endl;
                EV << "obstacleSegment: " << obstacleSegment << endl;
                return true;
            }
        }
    }
    return false;
}