from shapely import Polygon, Point


def segment_contains(a, b, c):
    epsilon = 0.000001
    crossproduct = (c[1] - a[1]) * (b[0] - a[0]) - (c[0] - a[0]) * (b[1] - a[1])

    # compare versus epsilon for floating point values, or != 0 if using integers
    if abs(crossproduct) > epsilon:
        return False

    dotproduct = (c[0] - a[0]) * (b[0] - a[0]) + (c[1] - a[1]) * (b[1] - a[1])
    if dotproduct < 0:
        return False

    squaredlengthba = (b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1])
    if dotproduct > squaredlengthba:
        return False

    return True


def order_polygon_points(polygon):
    # Create a Shapely Polygon object
    polygon = Polygon(polygon)

    # Get the convex hull of the polygon
    hull = polygon.convex_hull

    # Extract the ordered points from the convex hull
    hull_points = hull.exterior.coords[:]

    return hull_points


def is_point_inside_obstacles(obstacles, point):
    for obstacle in obstacles:

        # if the obstacle is a line
        if len(obstacle) == 2:
            # check if point is on the line
            if segment_contains(obstacle[0], obstacle[1], point):
                return True
            continue

        obstacle = order_polygon_points(obstacle)
        if Polygon(obstacle).contains(Point(point)) or Polygon(obstacle).touches(Point(point)):
            return True

    return False