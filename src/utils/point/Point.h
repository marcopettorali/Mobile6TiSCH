#ifndef POINT_H_
#define POINT_H_

#include <cmath>
#include <iostream>

#define POINT_PRECISION_M 0.001

class Point {
   public:
    // info
    double x;
    double y;

    // constructors
    Point(double x, double y) : x(x), y(y) { roundCoords(); }

    Point() : Point(0, 0) {}

    Point(const Point& p) : Point(p.x, p.y) {}

    // utility
    void roundCoords() {
        x = round(x / POINT_PRECISION_M) * POINT_PRECISION_M;
        y = round(y / POINT_PRECISION_M) * POINT_PRECISION_M;
    }

    // returns the length of the vector
    double norm() { return sqrt(x * x + y * y); }

    // computes the distance between two points
    double distance(Point p) { return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2)); }

    // computes the squared distance (for comparisons: more efficient than comparing distances)
    double distSq(Point p) const { return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y); }

    // normalize the vector with the given length
    void normalize(double length = 1);

    // check if one point comes before another in the clockwise order around a reference point
    static bool comparePoints(const Point& p1, const Point& p2, const Point& reference);

    // operators
    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point& p) const { return !(*this == p); }

    Point operator+(const Point& p) const {
        Point ret(x + p.x, y + p.y);
        ret.roundCoords();
        return ret;
    }

    Point& operator+=(const Point& p) { return *this = *this + p; }

    Point operator-(const Point& p) const {
        Point ret(x - p.x, y - p.y);
        ret.roundCoords();
        return ret;
    }

    Point& operator-=(const Point& p) { return *this = *this - p; }

    Point operator*(double scalar) const {
        Point ret = Point(x * scalar, y * scalar);
        ret.roundCoords();
        return ret;
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

#endif