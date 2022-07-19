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

#include "borderroutercore.h"

Define_Module(BorderRouterCore);

void BorderRouterCore::initialize(int stage) {
    if (stage == 1) {  // retrieve macAddress + init graphics
        coordinator = check_and_cast<NetworkCoordinator *>(getModuleByPath(par("coordinator")));
        macAddress = getParentModule()->getSubmodule("radio")->par("macAddress");
        tsch = check_and_cast<TSCH *>(getModuleByPath(par("tsch")));
        tsch->switchRadioBroadcastDomain(BR_BROADCAST_DOMAIN);

        // get mobility reference
        mobility = (Mobility *)getParentModule()->getSubmodule("mobility");

        coordinator->registerBR(macAddress, this, mobility->currentX, mobility->currentY, mobility->radius);
        // visualization
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("b", 0, 10);
        str.setTagArg("b", 1, 10);
        str.setTagArg("b", 2, "rect");
        str.setTagArg("b", 3, "red");

    } else if (stage == 4) {  // download schedule from manager
        vector<int> targets = coordinator->getInitialSchedule(macAddress);
        updateSchedule(targets);
    }
}

void BorderRouterCore::handleMessage(cMessage *msg) {
    if (msg->getArrivalGate() == gate("fromNetworkManager")) {
        handleDownstreamPacket(msg);
    } else if (msg->getArrivalGate() == gate("fromTSCH")) {
        handleUpstreamPacket(msg);
    }
}

void BorderRouterCore::handleDownstreamPacket(cMessage *msg) {
    DownstreamPkt *down = check_and_cast<DownstreamPkt *>(msg);

    TSCHPkt *pkt = new TSCHPkt();
    pkt->setKind(TSCHPKT_DOWN);
    pkt->setSeqNum(down->getSeqNum());
    pkt->setSrc(BR_BROADCAST_DOMAIN);
    pkt->setDest(down->getMobileNode());
    pkt->setByteLength(22);
    pkt->encapsulate(down);

    send(pkt, "toTSCH");
}

void BorderRouterCore::handleUpstreamPacket(cMessage *msg) {
    coordinator->deliverUpstreamPkt(msg);
}

void BorderRouterCore::updateSchedule(vector<int> targets) {
    tsch->updateSchedule(targets);
    EV << "BR: " << macAddress << ": schedule updated" << endl;
    if (tsch->schedule->find_mn(2000)) {  // DEBUG
        EV << "MN 2000 found" << endl;
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("b", 0, 10);
        str.setTagArg("b", 1, 10);
        str.setTagArg("b", 2, "rect");
        str.setTagArg("b", 3, "white");
        str.setTagArg("r", 0, mobility->radius);
        str.setTagArg("r", 1, "lightgreen");

    } else {
        throw new cException("MN 2000 not found");
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("b", 0, 10);
        str.setTagArg("b", 1, 10);
        str.setTagArg("b", 2, "rect");
        str.setTagArg("b", 3, "black");
        str.setTagArg("r", 0, 1);
    }
}
