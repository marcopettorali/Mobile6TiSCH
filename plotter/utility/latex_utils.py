from matplotlib import rcParams
from mpl_toolkits.axes_grid1.inset_locator import inset_axes

rcParams['ps.useafm'] = True
rcParams['pdf.use14corefonts'] = True
rcParams['text.usetex'] = True
rcParams['font.family'] = 'serif'
rcParams['font.serif'] = 'Helvetica'
#rcParams['text.latex.preamble'] = r'\boldmath'
rcParams['font.size'] = 17.0
rcParams['hatch.linewidth'] = 0.4
rcParams['xtick.labelsize'] = 16
rcParams['ytick.labelsize'] = 16
rcParams['legend.fontsize'] = 16
rcParams['hatch.linewidth'] = 0.7

MARKERS = ['s', 'o', 'v', '*', 'p', '<', 'h']
COLORS = ['k', 'b', 'g', 'r', 'c', 'm', 'y', 'maroon', 'limegreen']
HATCHES = ['', '/', 'x', '\\', 'o']
STYLES = ['-', '--', '-.', ':']

PALETTES = {"greenblue4": ["green", "limegreen", "cornflowerblue", "blue"]}

LEGEND_SMALL_SIZE = 12


def bold(s):
    if isinstance(s, str):
        # wrap every substring limited with $ ... $ with \mathbf{...}

        # find all substrings limited with $ ... $

        start = 0
        while True:
            start = s.find("$", start)
            if start == -1:
                break
            end = s.find("$", start + 1)
            if end == -1:
                break
            s = s[:start] + "_PLACEHOLDER_\\mathbf{" + s[start + 1:end] + "}_PLACEHOLDER_" + s[end + 1:]
            start = end + 1

        s = s.replace("_PLACEHOLDER_", "$")
        return "\\bf{" + s + "}"
    elif isinstance(s, int):
        return "\\bf{" + str(s) + "}"
    elif isinstance(s, float):
        return "\\bf{" + str(s) + "}"
    else:
        list = []
        for item in s:
            list.append(bold(item))
        return list


# convert the matrix to latex
def matrix_to_latex_table(matrix, xs, ys, xlabel, ylabel, caption, label, doc=False, tab=False):
    latex = ""

    if doc:
        latex += "\\documentclass{article}\n"
        latex += "\\usepackage[utf8]{inputenc}\n"
        latex += "\\title{test}\n"
        latex += "\\begin{document}\n"

    if tab:
        latex += "\\begin{table}[]\n"
        latex += "\\centering\n"

    latex += "\\begin{tabular}{c |"
    for i in range(len(matrix[0])):
        latex += " c"
    latex += "}\n"

    #put multicolumn as large as the row
    latex += "& \\multicolumn{" + str(len(matrix[0])) + "}{c}{" + ylabel + "} \\\\ \n"

    latex += f"{xlabel} &"
    for i in range(len(matrix[0])):
        # check if ys[i] is a number
        if isinstance(ys[i], str):
            latex += f"{ys[i]}"
        else:
            latex += f"{round(ys[i],2)}"
        if i != len(matrix[0]) - 1:
            latex += " &"
    latex += "\\\\\n"
    latex += "\\hline\n"
    for i, row in enumerate(matrix):
        # check if xs[i] is a number
        if isinstance(xs[i], str):
            latex += f"{xs[i]}&"
        else:
            latex += f"{round(xs[i],2)}&"
        for j, cell in enumerate(row):
            latex += f"{cell}"
            if j < len(row) - 1:
                latex += " & "
        latex += "\\\\\n"
    latex += "\\end{tabular}\n"

    if tab:
        latex += "\\caption{" + caption + "}\n"
        latex += "\\label{tab:" + label + "}\n"
        latex += "\\end{table}\n"

    if doc:
        latex += "\\end{document}\n"

    return latex