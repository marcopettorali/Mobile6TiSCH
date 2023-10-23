#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "Point.h"
#include <iostream>

class Segment {
   public:
    // info
    Point p1;
    Point p2;

    // constructors
    Segment(Point p1, Point p2) : p1(p1), p2(p2) {}
    Segment() : p1(Point()), p2(Point()) {}
    Segment(const Segment& s) : p1(s.p1), p2(s.p2) {}

    // operators
    friend std::ostream& operator<<(std::ostream& os, const Segment& s) {
        os << "[" << s.p1 << ", " << s.p2 << "]";
        return os;
    }

    // utility

    // check if point is on the segment
    bool includesInRect(Point p);

    // check if point is on the segment
    bool contains(Point p);

    // check if two segments intersect, and if they do, copies the intersection point in the intersectionPoint parameter
    bool intersection(Segment s, Point& intersectionPoint);
};

#endif