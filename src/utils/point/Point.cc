#include "Point.h"

void Point::normalize(double length) {
    double norm = sqrt(x * x + y * y);
    x = x * length / norm;
    y = y * length / norm;
}

bool Point::comparePoints(const Point& p1, const Point& p2, const Point& centroid) {
    // Compare points based on their angle with respect to the centroid
    double angle1 = atan2(p1.y - centroid.y, p1.x - centroid.x);
    double angle2 = atan2(p2.y - centroid.y, p2.x - centroid.x);
    return angle1 < angle2;
}