from stats_util import s_mean_confidence_interval
from analyze_repetition import analyze_repetition
import json
import os
import sys

import multiprocessing as mp

scenario_results = {}

def append(results):
    global scenario_results

    if results == None:
        return

    for metric in results:
        if not metric in scenario_results:
            scenario_results[metric] = []
        scenario_results[metric].append(results[metric])


def analyze_scenario(scenario, reps = 35):

    global scenario_results
    scenario_results = {}
    scenario_ci = {}
    outcomes = {}

    pool = mp.Pool(processes=mp.cpu_count())
    results = [pool.apply_async(analyze_repetition, args=(scenario + "-#" + str(i),), callback=append) for i in range(reps)]

    pool.close()
    pool.join()
    
    for key in scenario_results:
        low, high = s_mean_confidence_interval(scenario_results[key])
        scenario_ci[key] = [low, high]

    # create folder analysis if it does not exist
    if not os.path.exists("analysis"):
        os.makedirs("analysis")

    with open(f"analysis/{scenario.split('/')[-1]}.json", "a") as file:
        file.write(json.dumps(scenario_ci, indent=4))
        
if __name__=="__main__":
    if len(sys.argv) > 2:
        analyze_scenario(sys.argv[1], int(sys.argv[2]))
    elif len(sys.argv) > 1:
        analyze_scenario(sys.argv[1])
    else:
        analyze_scenario("simulations/results/test-10,StaticMobility")

