#include "Segment.h"

bool Segment::includesInRect(Point p) {
    // check if point is on the segment
    double minX = std::min(p1.x, p2.x);
    double maxX = std::max(p1.x, p2.x);
    double minY = std::min(p1.y, p2.y);
    double maxY = std::max(p1.y, p2.y);

    if (p.x < minX || p.x > maxX || p.y < minY || p.y > maxY) {
        return false;
    }

    return true;
}

bool Segment::intersection(Segment s, Point& intersectionPoint) {
    double a1 = p2.y - p1.y;
    double b1 = p1.x - p2.x;
    double c1 = a1 * (p1.x) + b1 * (p1.y);

    double a2 = s.p2.y - s.p1.y;
    double b2 = s.p1.x - s.p2.x;
    double c2 = a2 * (s.p1.x) + b2 * (s.p1.y);

    double det = a1 * b2 - a2 * b1;

    if (det == 0) {
        return false;
    }

    double x = (b2 * c1 - b1 * c2) / det;
    double y = (a1 * c2 - a2 * c1) / det;

    Point ret(x, y);

    // check if intersection is included in both segments
    if (!this->includesInRect(ret) || !s.includesInRect(ret)) {
        return false;
    }

    intersectionPoint = ret;
    return true;
}

bool Segment::contains(Point p) {
    double cross = (p.y - p1.y) * (p2.x - p1.x) - (p.x - p1.x) * (p2.y - p1.y);
    if (std::fabs(cross) > 0.0001) {
        return false;
    }

    double dot = (p.x - p1.x) * (p2.x - p1.x) + (p.y - p1.y) * (p2.y - p1.y);
    if (dot < 0) {
        return false;
    }

    double len_sq = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
    if (dot > len_sq) {
        return false;
    }

    return true;
}