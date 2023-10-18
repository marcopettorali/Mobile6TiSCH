#include "Polygon.h"

Point Polygon::computeCentroid() {
    Point centroid = {0.0, 0.0};
    for (const auto& vertex : points) {
        centroid.x += vertex.x;
        centroid.y += vertex.y;
    }
    centroid.x /= points.size();
    centroid.y /= points.size();
    return centroid;
}

bool Polygon::isPointInside(Point point) {
    int i, j;
    bool isInside = false;
    int numVertices = points.size();

    for (i = 0, j = numVertices - 1; i < numVertices; j = i++) {
        if (((points[i].y > point.y) != (points[j].y > point.y)) &&
            (point.x < (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)) {
            isInside = !isInside;
        }
    }

    return isInside;
}

void Polygon::orderPoints() {
    Point centroid = computeCentroid();
    std::sort(points.begin(), points.end(), [&](const Point& p1, const Point& p2) { return Point::comparePoints(p1, p2, centroid); });
}

std::vector<Segment> Polygon::getSegments() {
    std::vector<Segment> ret;
    if (points.size() == 2) {
        ret.push_back(Segment(points[0], points[1]));
        return ret;
    }

    for (int i = 0; i < points.size(); i++) {
        ret.push_back(Segment(points[i], points[(i + 1) % points.size()]));
    }

    return ret;
}

