from analyze_scenario import analyze_scenario
import os
import sys

def main(folder):
    # list all .sca in folder
    for file in os.listdir(folder):
        if file.endswith("-#0.csv"):
            # remove -#0.csv
            scenario = file[:-7]
            analyze_scenario(folder + "/" + scenario)

if __name__=="__main__":
    if len(sys.argv) > 1:
        main(sys.argv[1])
    else:
        main("../simulations/results")
