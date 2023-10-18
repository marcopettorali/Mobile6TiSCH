from utils import is_point_inside_obstacles

input_filename = "obstacles.svg"


# read file
with open(input_filename, "r") as f:
    lines = f.readlines()

# split lines by tag
lines = "".join(lines).split("<")

# parse <rect> and <path> tags
goodlines = []
for line in lines:
    if "rect" in line or "path" in line:
        goodlines.append(line)

# parse <rect> and <path> tags
obstacles = []
for line in goodlines:
    if line.startswith("rect"):
        # parse <rect> tag
        x = float(line.split("x=\"")[1].split("\"")[0])
        y = float(line.split("y=\"")[1].split("\"")[0])
        w = float(line.split("width=\"")[1].split("\"")[0])
        h = float(line.split("height=\"")[1].split("\"")[0])

        obstacles.append([(x, y), (x + w, y), (x + w, y + h), (x, y + h)])

    elif line.startswith("path"):

        # parse <path> tag
        d = line.split("d=")[1].split("\"")[1].split("\"")[0]

        x1 = float(d.split("M")[1].split(" ")[1])
        y1 = float(d.split("M")[1].split(" ")[2])

        if "Q" in d:
            # parse d as M x1 y1 Q _ _ x2 y2
            x2 = float(d.split("Q ")[1].split(" ")[2])
            y2 = float(d.split("Q ")[1].split(" ")[3])

        elif "L" in d:
            # parse d as M x1 y1 L x2 y2
            x2 = float(d.split("L")[1].split(" ")[1])
            y2 = float(d.split("L")[1].split(" ")[2])

        obstacles.append([(x1, y1), (x2, y2)])

# normalize (subtract min x and min y)
min_x = min([min([p[0] for p in obstacle]) for obstacle in obstacles])
min_y = min([min([p[1] for p in obstacle]) for obstacle in obstacles])

obstacles = [[(p[0] - min_x, p[1] - min_y) for p in obstacle] for obstacle in obstacles]

# remove duplicates
new_obstacles = []
for obstacle in obstacles:
    if obstacle not in new_obstacles:
        new_obstacles.append(obstacle)

obstacles = new_obstacles

# remove obstacles that contains all obstacles
to_remove = []
for obstacle1 in obstacles:
    contains_all = True
    for obstacle2 in obstacles:
        if obstacle1 != obstacle2:
            if all([is_point_inside_obstacles([obstacle2], point) for point in obstacle1]):
                contains_all = False
                break
    if contains_all:
        to_remove.append(obstacle1)

obstacles = [obstacle for obstacle in obstacles if obstacle not in to_remove]

# remove obstacles that are contained in other obstacles
to_remove = []
for obstacle1 in obstacles:
    for obstacle2 in obstacles:
        if obstacle1 != obstacle2:
            if all([is_point_inside_obstacles([obstacle1], point) for point in obstacle2]):
                to_remove.append(obstacle2)

obstacles = [obstacle for obstacle in obstacles if obstacle not in to_remove]

# print obstacles
print("Obstacles:")
for obstacle in obstacles:
    print(obstacle, end=", ")
print()