//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "NCMiddlewareBase.h"

#include <limits>

Define_Module(NCMiddlewareBase);

void NCMiddlewareBase::initialize(int stage) {
    MiddlewareBase::initialize(stage);

    if (stage == INITSTAGE_MODULES_FETCH) {
        schedulingFunction = check_and_cast<SchedulingFunctionBase *>(findModuleByPath(par("schedulingFunction")));
        statsCollector->registerCollectable(this);
    }

    if (stage == INITSTAGE_SHMG_NC_BUILD_SCHEDULE) {
        // trigger the scheduling function to build the schedules and store them in schedules map
        installSchedule();
    }

    if (stage == INITSTAGE_STATS_SUBSCRIPTION) {
        // init PDR dist stats for each MN
        for (auto mn : mns) {
            mn_mac_address_t macAddress = mn.first;
            statsCollector->subscribeStats(std::string(std::string("instantPDR_MN") + std::to_string(macAddress)), STAT_TYPE_VECTOR);
        }
    }

    if (stage == INITSTAGE_BEEP_INIT) {
        // initialize stats collection beep
        statsCollectionBeep = new cMessage("statsBeep");
        scheduleAt(simTime() + STATS_PERIOD_S, statsCollectionBeep);

        // initialize slotframe start beep
        slotframeStartBeep = new cMessage("slotframeStartBeep");
        scheduleAt(simTime() + slotframeLength * TIMESLOT_DURATION_S, slotframeStartBeep);
    }

    if (stage == INITSTAGE_LAST_STAGE) {
        // print sizes of mns and brs
        INF("NC") << "Number of MNs: " << mns.size() << endl;
        INF("NC") << "Number of BRs: " << brs.size() << endl;
    }
}

void NCMiddlewareBase::handleSelfMessage(cMessage *msg) {
    if (msg->isName("statsBeep")) {
        onStatsCollection(msg);

        // schedule next beep
        scheduleAt(simTime() + STATS_PERIOD_S, statsCollectionBeep);

    } else if (msg->isName("slotframeStartBeep")) {
        onSlotframeStart(msg);

        // schedule next slotframe start beep
        scheduleAt(simTime() + slotframeLength * TIMESLOT_DURATION_S, slotframeStartBeep);
    }
}

void NCMiddlewareBase::handleUpperPacket(cPacket *pkt) {
    SHMGNetworkPkt *shmgPkt = check_and_cast<SHMGNetworkPkt *>(pkt);

    // encapsulate in lower packet
    BackboneCommand *backPkt = new BackboneCommand(NAME_ENCAPSULATE("BackboneCommand", shmgPkt));
    backPkt->setSrcEncapsulatedPkt(shmgPkt->getSrc());
    backPkt->setDestEncapsulatedPkt(shmgPkt->getDest());
    backPkt->setSeqNum(shmgPkt->getSeq());
    backPkt->encapsulate(shmgPkt);

    // select the delegate BR
    br_mac_address_t delegateBR = mns[shmgPkt->getDest()].delegateBR;

    // throw exception if delegateBR is not found
    if (delegateBR == -1) {
        throw cRuntimeError("Delegate BR not found for mn %d", backPkt->getDestEncapsulatedPkt());
    }

    INF("NC") << "sending packet to " << backPkt->getDestEncapsulatedPkt() << " via " << delegateBR << endl;

    // send packet to delegateBR
    sendPacketToBR(backPkt, delegateBR);
}

void NCMiddlewareBase::handleLowerPacket(cPacket *pkt) {
    BackboneDataInfo *backPkt = check_and_cast<BackboneDataInfo *>(pkt);

    // unpack the packet
    mn_mac_address_t mobileNode = backPkt->getMobileNode();
    br_mac_address_t borderRouter = backPkt->getBorderRouter();
    int seq = backPkt->getSeq();
    double rssi = backPkt->getRssidBm();

    // update RSSI table
    rssiTable.insert(seq, mobileNode, borderRouter, simTime().dbl(), rssi);

    INF("NC") << "received packet from " << mobileNode << " via " << borderRouter << " with seq " << seq << " and rssi " << rssi << endl;

    // check if packet is duplicate
    if (!dedupTable.checkMessage(mobileNode, seq)) {
        INF("NC") << "Duplicate packet, discarding: " << mobileNode << " with seq " << seq << endl;
        delete backPkt;
    } else {
        // if not duplicate, send to upper layer
        // update delegateBR for downlink
        mns[mobileNode].delegateBR = rssiTable.getLatestHighestEntry(mobileNode).br;

        DBG("NC") << "Delegate BR for " << mobileNode << " is " << mns[mobileNode].delegateBR << endl;

        // enable ACKs on delegate BR
        brs[mns[mobileNode].delegateBR].module->setIEEE802154RetransmissionsEnabled(true);

        // consume packet
        SHMGNetworkPkt *shmgPkt = check_and_cast<SHMGNetworkPkt *>(backPkt->decapsulate());
        handleSHMGNetworkPkt(shmgPkt);

        send(shmgPkt->decapsulate(), "upperLayerOut");
        delete backPkt;
        delete shmgPkt;
    }
}

void NCMiddlewareBase::onStatsCollection(cMessage *msg) {
    //  compute the packet loss for each mn in the period
    for (auto mn : packetsStats) {
        // retrieve info
        mac_address_t mnAddress = mn.first;
        std::pair<int, int> &mnStats = mn.second;

        // compute packet loss and pdr
        int successPackets = mnStats.first;
        int totalPackets = mnStats.second;

        assert(totalPackets == 0);                                                              // no registration of failed packets
        int lostPackets = (int)round(mns[mnAddress].upRate * STATS_PERIOD_S) - successPackets;  // assuming constant rate

        double packetLoss = (double)lostPackets / (double)(successPackets + lostPackets);
        packetLoss = packetLoss < 0 ? 0 : packetLoss;
        double pdr = 1 - packetLoss;

        // check if entry exists in instantPDR
        if (instantPDR.find(mnAddress) == instantPDR.end()) {
            // create new entry
            instantPDR[mnAddress] = std::vector<double>();
        }

        // add packet loss to the vector
        instantPDR[mnAddress].push_back(pdr);
        std::string statName = "instantPDR_MN" + std::to_string(mnAddress);
        statsCollector->record(0, statName, pdr);

        // reset the counters in packetsStatsPerMN
        mnStats = {0, 0};
    }
}

void NCMiddlewareBase::onSlotframeStart(cMessage *msg) {
    if (schedulingFunction->isRescheduled()) {
        for (auto br : brs) {
            br_mac_address_t macAddress = br.first;
            auto schedule = schedulingFunction->getSchedule(macAddress, BR_BROADCAST_ADDRESS);
            INF("NC") << "Schedule for BR " << macAddress << endl;
            schedule->printSchedule(EV);
            schedules[macAddress]->adapt(schedule);
            br.second.module->updateSchedule(schedule);
        }
    }
}

void NCMiddlewareBase::installSchedule() {
    // install schedule for mn
    for (auto mn : mns) {
        mn_mac_address_t macAddress = mn.first;
        auto schedule = schedulingFunction->getSchedule(macAddress, MN_BROADCAST_ADDRESS);
        INF("NC") << "Schedule for MN " << macAddress << endl;
        schedule->printSchedule(EV);
        schedules[macAddress] = schedule;
    }

    // install schedule for br
    for (auto br : brs) {
        br_mac_address_t macAddress = br.first;
        auto schedule = schedulingFunction->getSchedule(macAddress, BR_BROADCAST_ADDRESS);
        INF("NC") << "Schedule for BR " << macAddress << endl;
        schedule->printSchedule(EV);
        schedules[macAddress] = schedule;
    }

    // determine slotframe length
    slotframeLength = schedules.begin()->second->slotframeLength;  // ASSUMPTION: all the schedules have the same slotframe length
}

IEEE802154TSCHSchedule *NCMiddlewareBase::getSchedule(mac_address_t macAddress, mac_address_t broadcastAddress) {
    Enter_Method_Silent("%i,%i", macAddress, broadcastAddress);

    // return a copy of the original central schedule
    IEEE802154TSCHSchedule *schedule = schedules[macAddress]->copy();
    schedule->filter({macAddress, broadcastAddress});
    return schedule;
}

void NCMiddlewareBase::registerMN(mn_mac_address_t macAddress, mn_device_id_t mnId, double upRate, Point position, Point direction, double maxSpeed) {
    // find the closest BR as delegateBR
    br_mac_address_t delegateBR = findClosestBR(position);

    mns[macAddress] = {mnId, upRate, position, direction, maxSpeed, delegateBR};
}

void NCMiddlewareBase::registerBR(br_mac_address_t macAddress, br_device_id_t brId, BRMiddlewareBase *br, Point position, double r) {
    brs[macAddress] = {brId, br, position, r};
}

void NCMiddlewareBase::sendPacketToBR(BackboneCommand *backPkt, br_mac_address_t brMacAddress) {
    // encapsulate in WiredNIC packet
    WiredNICPkt *wiredPkt = new WiredNICPkt(NAME_ENCAPSULATE("WiredNICPkt", backPkt));
    wiredPkt->setSrc(backPkt->getSrcEncapsulatedPkt());
    wiredPkt->setDest(backPkt->getDestEncapsulatedPkt());
    wiredPkt->setSeqNum(backPkt->getSeqNum());
    wiredPkt->setOutPort(brs[brMacAddress].brId);
    wiredPkt->encapsulate(backPkt);
    send(wiredPkt, "lowerLayerOut");
    return;
}

br_mac_address_t NCMiddlewareBase::findClosestBR(Point p) {
    // iterate all brs and find the closest one to (x,y)
    auto it = std::min_element(brs.begin(), brs.end(),
                               [&p](const std::pair<br_mac_address_t, br_desc_t> &br1, const std::pair<br_mac_address_t, br_desc_t> &br2) {
                                   return p.distSq(br1.second.position) < p.distSq(br2.second.position);
                               });
    if (it == brs.end()) {
        throw cRuntimeError("BR not found");
    }
    return it->first;
}