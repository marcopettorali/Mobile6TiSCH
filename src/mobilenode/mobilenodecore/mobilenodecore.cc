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

#include "mobilenodecore.h"

Define_Module(MobileNodeCore);

void MobileNodeCore::initialize(int stage) {
    if (stage == 2) {  // retrieve basic info for the mobile node

        // get up and downstream rates
        upRate = par("upRate");
        downRate = par("downRate");

        // get macAddress from radio module
        macAddress = getParentModule()->getSubmodule("radio")->par("macAddress");

        // set up tsch
        tsch = check_and_cast<TSCH *>(getModuleByPath(par("tsch")));
        tsch->switchRadioBroadcastDomain(MN_BROADCAST_DOMAIN);

        // get mobility reference
        mobility = (Mobility *)getParentModule()->getSubmodule("mobility");

        // register to the network coordinator
        coordinator = check_and_cast<NetworkCoordinator *>(getModuleByPath(par("coordinator")));

        coordinator->registerMN(macAddress, upRate, downRate, mobility->currentX, mobility->currentY, sqrt(mobility->maxSpeed));

        // setup statistics

        upstreamPacketSent = registerSignal("upstreamPacketSent");
        downstreamPacketReceived = registerSignal("downstreamPacketReceived");
        downstreamPacketDelay = registerSignal("downstreamPacketDelay");
        rtt = registerSignal("rtt");

        // set up graphics
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("b", 0, 10);
        str.setTagArg("b", 1, 10);
        str.setTagArg("b", 2, "oval");

        if (macAddress == 2000) {  // DEBUG
            str.setTagArg("b", 3, "blue");
        } else {
            str.setTagArg("b", 3, "lightblue");
        }

    } else if (stage == 4) {  // download schedule from manager
        tsch->updateSchedule({macAddress});

        // setup upstream periodic traffic
        application = getParentModule()->getParentModule()->par("application").stringValue();
        scheduling = getParentModule()->getParentModule()->par("scheduling").stringValue();

        period = par("period");

        if (period == -1){
            // use the maximum rate allowed by the scenario
            if(application == "convergecast"){
                period = tsch->schedule->get_frame_length() * TIMESLOT_DURATION_S;
            }else if(application == "reqres" && scheduling == "sddu"){
                int m = coordinator->getNumMNs();
                int g = coordinator->getNumChannelsUsed();
                period = 1/( floor(m/g) / (m * tsch->schedule->get_frame_length() * TIMESLOT_DURATION_S));
            }else if(application == "reqres" && scheduling == "dddu"){
                period = tsch->schedule->get_frame_length() * TIMESLOT_DURATION_S;
            }
        }
        assert(period > 0);

        beep = new cMessage("send_upstream_beep");
        scheduleAt(simTime() + period + uniform(0, 2), beep);

        seqNum = 0;
        received = 0;
    }
}

void MobileNodeCore::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        UpstreamPkt *p = new UpstreamPkt("UpstreamPkt");
        p->setByteLength(22);
        p->setSeqNum(seqNum);
        p->setSendTime(simTime());
        p->setUpstreamSendTime(simTime());

        TSCHPkt *pkt = new TSCHPkt();
        pkt->setKind(TSCHPKT_UP);

        pkt->setSeqNum(seqNum);
        pkt->setSrc(macAddress);
        pkt->setDest(BR_BROADCAST_DOMAIN);

        pkt->encapsulate(p);

        seqNum++;
        send(pkt, "lowerLayerOut");
        emit(upstreamPacketSent, 1);

        scheduleAt(simTime() + period, beep);
    } else {
        DownstreamPkt *m = check_and_cast<DownstreamPkt *>(msg);
        EV << macAddress << ": RECEIVED #" << received++ << ": " << m->getName() << endl;

        emit(downstreamPacketReceived, 1);
        emit(downstreamPacketDelay, simTime() - m->getSendTime());
        emit(rtt, simTime() - m->getUpstreamSendTime());

        delete m->removeControlInfo();
        delete m;
    }
}
