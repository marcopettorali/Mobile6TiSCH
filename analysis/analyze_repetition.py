import sys
sys.path.append("/home/marco/omnetpp-6.0/python")

import pandas as pd

import stats_util

import omnetpp.scave.results as results

def safe_div(x,y):
    if y == 0:
        return 0
    return x / y

def compute_mean_avgvarperc(listoflist):
    # check if list is empty
    if not listoflist:
        return -1,-1,-1

    avgs = []
    vars = []
    perc95s = []

    for l in listoflist:
        avg, var = stats_util.s_mean_value_and_variance(l)
        perc95 = stats_util.s_percentile(l, 0.95)

        avgs.append(avg)
        vars.append(var)
        perc95s.append(perc95)

    return stats_util.s_mean_value(avgs), stats_util.s_mean_value(vars), stats_util.s_mean_value(perc95s)

def analyze_repetition(rep_name):

    ret = {}

    scalars = results.read_result_files(rep_name + ".sca")
    scalars = scalars[scalars["type"] == "scalar"]

    vectors = results.read_result_files(rep_name + ".vec")
    vectors = vectors[vectors["type"] == "vector"]

    # Packets sent and received
    num_packets_sent_by_mns = [ float(x) for x in scalars[(scalars["name"] == "stats_packetsSent:count") & (scalars["module"].str.contains("mn"))]["value"]]
    num_packets_received_by_mns = [ float(x) for x in scalars[(scalars["name"] == "stats_packetsReceived:count") & (scalars["module"].str.contains("mn"))]["value"]]
    num_packets_sent_by_nc = [ float(x) for x in scalars[(scalars["name"] == "stats_packetsSent:count") & (scalars["module"].str.contains("nc"))]["value"]][0]
    num_packets_received_by_nc = [ float(x) for x in scalars[(scalars["name"] == "stats_packetsReceived:count") & (scalars["module"].str.contains("nc"))]["value"]][0]

    ret["num_packets_sent_by_mns"] = sum(num_packets_sent_by_mns)
    ret["num_packets_received_by_mns"] = sum(num_packets_received_by_mns)
    ret["num_packets_sent_by_nc"] = num_packets_sent_by_nc
    ret["num_packets_received_by_nc"] = num_packets_received_by_nc

    # PDR
    upstream_pdr = safe_div(num_packets_received_by_nc, sum(num_packets_sent_by_mns))
    downstream_pdr = safe_div(sum(num_packets_received_by_mns), num_packets_sent_by_nc)
    reqres_pdr = safe_div(sum(num_packets_received_by_mns), sum(num_packets_sent_by_mns))

    ret["upstream_pdr"] = upstream_pdr
    ret["downstream_pdr"] = downstream_pdr
    ret["reqres_pdr"] = reqres_pdr

    # Average power consumption
    avg_power_consumption_mn = [ float(x) for x in scalars[(scalars["name"] == "stats_averagePowerConsumption:timeavg") & (scalars["module"].str.contains("mn"))]["value"]]
    avg_power_consumption_br = [ float(x) for x in scalars[(scalars["name"] == "stats_averagePowerConsumption:timeavg") & (scalars["module"].str.contains("br"))]["value"]]

    ret["avg_power_consumption_mn"] = stats_util.s_mean_value(avg_power_consumption_mn)
    ret["avg_power_consumption_br"] = stats_util.s_mean_value(avg_power_consumption_br)

    # Upstream delay
    upstream_delay_records = vectors[(vectors["name"] == "stats_packetsDelay:vector") & (vectors["module"].str.contains("nc"))]
    upstream_delay = [ list(row["vecvalue"]) for _, row in upstream_delay_records.iterrows() ]
    avg, var, perc95 = compute_mean_avgvarperc(upstream_delay)
    ret["upstream_delay_avg"] = avg
    ret["upstream_delay_var"] = var
    ret["upstream_delay_perc95"] = perc95
    
    # Downstream delay
    downstream_delay_records = vectors[(vectors["name"] == "stats_packetsDownstreamDelay:vector") & (vectors["module"].str.contains("mn"))]
    downstream_delay = [ list(row["vecvalue"]) for _, row in downstream_delay_records.iterrows() ]
    avg, var, perc95 = compute_mean_avgvarperc(downstream_delay)
    ret["downstream_delay_avg"] = avg
    ret["downstream_delay_var"] = var
    ret["downstream_delay_perc95"] = perc95


    # RTT
    rtt_records = vectors[(vectors["name"] == "stats_packetsDelay:vector") & (vectors["module"].str.contains("mn"))]
    rtt = [ list(row["vecvalue"]) for _, row in rtt_records.iterrows() ]
    avg, var, perc95 = compute_mean_avgvarperc(rtt)
    ret["rtt_avg"] = avg
    ret["rtt_var"] = var
    ret["rtt_perc95"] = perc95

    # Queue size
    queue_size_mn_records = vectors[(vectors["name"] == "stats_queueSize:vector") & (vectors["module"].str.contains("mn"))]
    queue_size_mn = [ list(row["vecvalue"]) for _, row in queue_size_mn_records.iterrows() ]
    avg, var, perc95 = compute_mean_avgvarperc(queue_size_mn)
    ret["queue_size_mn_avg"] = avg
    ret["queue_size_mn_var"] = var
    ret["queue_size_mn_perc95"] = perc95

    queue_size_br_records = vectors[(vectors["name"] == "stats_queueSize:vector") & (vectors["module"].str.contains("br"))]
    queue_size_br = [ list(row["vecvalue"]) for _, row in queue_size_br_records.iterrows() ]
    avg, var, perc95 = compute_mean_avgvarperc(queue_size_br)
    ret["queue_size_br_avg"] = avg
    ret["queue_size_br_var"] = var
    ret["queue_size_br_perc95"] = perc95

    return ret

if __name__ == "__main__":
    print(analyze_repetition("simulations/results/test-10,StaticMobility-#0"))
