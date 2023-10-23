#ifndef POLYGON_H_
#define POLYGON_H_

#include <algorithm>
#include <vector>

#include "Point.h"
#include "Segment.h"

class Point;

class Polygon {
    std::vector<Point> points;

   public:
    // constructors
    Polygon() {}
    Polygon(std::vector<Point> points) : points(points) {}
    Polygon(const Polygon& p) : points(p.points) {}

    // getters and setters
    std::vector<Point> getPoints() { return points; }
    void addPoint(Point p) { points.push_back(p); }
    void clear() { points.clear(); }

    // utility

    // compute centroid of the polygon
    Point computeCentroid();

    // check if a point is inside the polygon
    bool isPointInside(Point point);

    // orders points in clockwise order based on their angle with respect to the centroid
    void orderPoints();

    // returns a vector of segments that make up the polygon
    std::vector<Segment> getSegments();
};

#endif