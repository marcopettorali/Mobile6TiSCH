import matplotlib.colors as mcolors
import numpy as np
import math

def rgb_to_hex(rgb):
    """Converts rgb color to hex"""
    return "#{:02x}{:02x}{:02x}".format(rgb[0], rgb[1], rgb[2])

def hex_to_rgb(hex):
    """Converts hex color to rgb"""
    return tuple(int(hex.lstrip("#")[i:i + 2], 16) for i in (0, 2, 4))

def check_if_rgb(color):
    """Checks if the color is rgb"""
    return type(color) == tuple and len(color) == 3

def check_if_hex(color):
    """Checks if the color is hex"""
    return type(color) == str and len(color) == 6


def mult(c1, c2):
    if check_if_hex(c1):
        c1 = hex_to_rgb(c1)
    if check_if_hex(c2):
        c2 = hex_to_rgb(c2)

    val = (round(c1[0] * c2[0] / 255), round(c1[1] * c2[1] / 255), round(c1[2] * c2[2] / 255))

    if check_if_hex(c1) and check_if_hex(c2):
        val = rgb_to_hex(val)

    return val


def brightness(color, amount):

    if amount < 0 or amount > 1:
        raise ValueError("Amount must be between 0 and 1")

    if check_if_hex(color):
        color = hex_to_rgb(color)

    val = [round((m + amount * 255)) for m in color]    

    for i in range(len(val)):
        if val[i] > 255:
            val[i] = 255
        
        if val[i] < 0:
            val[i] = 0

    val = tuple(val)

    if check_if_hex(color):
        val = rgb_to_hex(val)

    return val
    
def css4color(color_str, mode = "hex"):
    if color_str not in mcolors.CSS4_COLORS:
        raise ValueError(f"Color \"{color_str}\" not found")
    color = mcolors.CSS4_COLORS[color_str]
    if mode == "hex":
        return color
    elif mode == "rgb":
        return hex_to_rgb(color)
    else:
        raise ValueError("Mode must be 'hex' or 'rgb'")


def palette(colors_list, n):

    if type(colors_list) != list:
        raise ValueError("Colors list must be a list")

    if type(n) != int:
        raise ValueError("n must be an integer")

    if len(colors_list) == 0:
        raise ValueError("Colors list must not be empty")

    if n < 0:
        raise ValueError("n must be greater than 0")
    
    colors_list = [hex_to_rgb(css4color(c)) for c in colors_list]

    if n == 0:
        return []

    if n == 1:
        return [colors_list[0]]

    if len(colors_list) == 1:
        return [colors_list[0] for _ in range(n)]

    ret = [rgb_to_hex(colors_list[0])]

    splits = len(colors_list) - 1
    avg_shades_per_split = (n-1)/splits
    count_shades = 0
    shades_done = 0

    for i in range(splits):

        # number of shades for this split
        count_shades += avg_shades_per_split
        n_shades = math.floor(count_shades) - shades_done
        shades_done += n_shades

        c1 = colors_list[i]
        c2 = colors_list[i+1]

        # subdivide [0,1] in n parts
        vals = [float(x) for x in np.linspace(0, 1, n_shades + 1)][1:]

        for v in vals: 
            c1_shade = brightness(c1, v)
            c2_shade = brightness(c2, 1-v)

            col = mult(c1_shade, c2_shade)
            ret.append(rgb_to_hex(col))

    return ret
            

        