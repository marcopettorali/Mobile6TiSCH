def safe_div(x, y):
    if y == 0:
        return 0
    return x / y

def safe(key,dict):
    if key in dict:
        return dict[key]
    else:
        return 0

def analyze_repetition(rep_name):

    ret = {}

    # open the file
    with open(rep_name + ".csv", "r") as f:
        for line in f:
            # split the line
            line = line.strip()
            line = line.split(",")

            # get the name and value
            name = line[0]
            value = float(line[1])

            # store the value
            ret[name] = value

    # POST PROCESSING

    # packets loss at network layer
    sum = safe("packetsLostSuccess.sum", ret) + safe("packetsLostTransmissionErrors.sum", ret) + safe("packetsLostCrossing.sum", ret) + safe("packetsLostConflict.sum", ret) + safe("packetsLostOutOfRange.sum", ret) + safe("packetsLostOther.sum", ret)
    
    ret["packetsLostSuccess"] = safe_div(safe("packetsLostSuccess.sum", ret), sum)
    ret["packetsLostTransmissionErrors"] = safe_div(safe("packetsLostTransmissionErrors.sum", ret), sum)
    ret["packetsLostCrossing"] = safe_div(safe("packetsLostCrossing.sum", ret), sum)
    ret["packetsLostConflict"] = safe_div(safe("packetsLostConflict.sum", ret) + safe("packetsLostCrossing.sum", ret), sum)
    ret["packetsLostOutOfRange"] = safe_div(safe("packetsLostOutOfRange.sum", ret), sum)
    ret["packetsLostOther"] = safe_div(safe("packetsLostOther.sum", ret), sum)


    ret["packetsLostConflictRedundant"] = safe_div(safe("packetsLostConflictRedundant.sum", ret), sum)
    ret["packetsLostConflictNotRedundant"] = safe_div(safe("packetsLostConflictNotRedundant.sum", ret), sum)
    ret["packetsLostSuccessSupport"] = safe_div(safe("packetsLostSuccessSupport.sum", ret), sum)

    # average packetOutcomes 
    sum = 0
    for i in range(0, 7):
        if(f"radioPacketOutcomes{i}.sum" in ret):
            sum += ret[f"radioPacketOutcomes{i}.sum"]
    
    
    for i in range(0, 7):
        if(f"radioPacketOutcomes{i}.sum" in ret):
            ret[f"radioPacketOutcomes{i}"] = safe_div(
                ret[f"radioPacketOutcomes{i}.sum"], sum)
        else:
            ret[f"radioPacketOutcomes{i}"] = 0

    # add PDR for ScheduleAdv

    if("ScheduleAdvSent.sum" in ret and "ScheduleAdvReceived.sum" in ret):
        ret["ScheduleAdvPDR"] = safe_div(
            int(ret["ScheduleAdvReceived.sum"]), int(ret["ScheduleAdvSent.sum"]))

    # add PDR stats
    if("upstreamPacketsReceived.sum" in ret and "upstreamPacketsSent.sum" in ret):
        ret["upstream_pdr"] = safe_div(
            int(ret["upstreamPacketsReceived.sum"]), int(ret["upstreamPacketsSent.sum"]))
    else:
        ret["upstream_pdr"] = 0

    if("downstreamPacketsReceived.sum" in ret and "downstreamPacketsSent.sum" in ret):
        ret["downstream_pdr"] = safe_div(
            int(ret["downstreamPacketsReceived.sum"]), int(ret["downstreamPacketsSent.sum"]))
    else:
        ret["downstream_pdr"] = 0

    if("upstreamPacketsSent.sum" in ret and "downstreamPacketsSent.sum" in ret):
        ret["endtoend_pdr"] = safe_div(
            int(ret["downstreamPacketsReceived.sum"]), int(ret["upstreamPacketsSent.sum"]))
    else:
        ret["endtoend_pdr"] = 0

    print(rep_name, f"up: {ret['upstream_pdr']};",
          f"down: {ret['downstream_pdr']};", f"end-to-end: {ret['endtoend_pdr']};")
    
    # add delay stats

    return ret


if __name__ == "__main__":
    print(analyze_repetition(
        "/home/ubuntu/git/Mobile6TiSCH-Private/simulations/results/testPL/testPL-prioritybased_closestfirst,RandomMobility,2mps,200-#0"))
