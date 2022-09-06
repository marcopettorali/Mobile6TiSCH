import sys
sys.path.append("/home/marco/omnetpp-6.0/python")

import pandas as pd
import json

import omnetpp.scave.results as results

# # Load the results file (scalars)
# res = results.read_result_files("simulations/results/test-10,LinearMobility-#24.sca")
# # only show rows with type == "scalar"
# res = res[res["type"] == "scalar"]
# print(res)

# # collect all rows with name == "stats_packetsSent:count" and module contains "mn"
# packets_sent_by_mns = [ float(x) for x in res[(res["name"] == "stats_packetsSent:count") & (res["module"].str.contains("mn"))]["value"]]

# print(packets_sent_by_mns)
# packets_sent = [ float(x) for x in res[res["name"] == "stats_packetsSent:count"]["value"]]

# print(packets_sent)



# Load the results file vectors
res = results.read_result_files("simulations/results/test-10,LinearMobility-#24.vec")
# only show rows with type == "scalar"
res = res[res["type"] == "vector"]

# filter for the module that contains "nc" and the name "stats_packetsDelay:vector"
upstream_delay_records = res[(res["name"] == "stats_packetsDelay:vector") & (res["module"].str.contains("nc"))]

print(res)
print(upstream_delay_records)

# iterate through the records
for index, row in upstream_delay_records.iterrows():
    print(list(row["vecvalue"]))

