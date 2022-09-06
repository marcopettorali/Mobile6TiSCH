import sys
sys.path.append("/home/marco/omnetpp-6.0/python")

import pandas as pd

import omnetpp.scave.results as results

def safe_div(x,y):
    if y == 0:
        return 0
    return x / y

def analyze_repetition(rep_name):

    ret = {}

    scalars = results.read_result_files(rep_name + ".sca")
    scalars = scalars[scalars["type"] == "scalar"]

    vectors = results.read_result_files(rep_name + ".vec")
    vectors = vectors[vectors["type"] == "vector"]

    # PDR
    num_packets_sent_by_mns = [ float(x) for x in scalars[(scalars["name"] == "stats_packetsSent:count") & (scalars["module"].str.contains("mn"))]["value"]]
    num_packets_received_by_mns = [ float(x) for x in scalars[(scalars["name"] == "stats_packetsReceived:count") & (scalars["module"].str.contains("mn"))]["value"]]
    num_packets_sent_by_nc = [ float(x) for x in scalars[(scalars["name"] == "stats_packetsSent:count") & (scalars["module"].str.contains("nc"))]["value"]][0]
    num_packets_received_by_nc = [ float(x) for x in scalars[(scalars["name"] == "stats_packetsReceived:count") & (scalars["module"].str.contains("nc"))]["value"]][0]

    upstream_pdr = safe_div(num_packets_received_by_nc, sum(num_packets_sent_by_mns))
    downstream_pdr = safe_div(sum(num_packets_received_by_mns), num_packets_sent_by_nc)
    reqres_pdr = safe_div(sum(num_packets_received_by_mns), sum(num_packets_sent_by_mns))

    ret["upstream_pdr"] = upstream_pdr
    ret["downstream_pdr"] = downstream_pdr
    ret["reqres_pdr"] = reqres_pdr

    # Delay
    upstream_delay_records = vectors[(vectors["name"] == "stats_packetsDelay:vector") & (vectors["module"].str.contains("nc"))]

    for index, row in upstream_delay_records.iterrows():
        upstream_delay = list(row["vecvalue"])
    

    downstream_delay_records = vectors[(vectors["name"] == "stats_packetsDownstreamDelay:vector") & (vectors["module"].str.contains("mn"))]
    
    for index, row in downstream_delay_records.iterrows():
        downstream_delay = list(row["vecvalue"])

    rtt_delay_records = vectors[(vectors["name"] == "stats_packetsDelay:vector") & (vectors["module"].str.contains("mn"))]

    for index, row in rtt_delay_records.iterrows():
        rtt_delay = list(row["vecvalue"])




    


    return ret

if __name__ == "__main__":
    print(analyze_repetition("simulations/results/test-10,LinearMobility-#24"))
