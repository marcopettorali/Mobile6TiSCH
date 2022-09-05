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

#include "ConvergecastClient.h"

Define_Module(ConvergecastClient);

void ConvergecastClient::initialize(int stage) {
    ApplicationBase::initialize(stage);

    if (stage == 2) {  // retrieve basic info for the mobile node

        double upPeriod = par("upPeriod");
        double downPeriod = par("downPeriod");
        double upDelay = par("upDelay");
        double downDelay = par("downDelay");

        // set up tsch
        ietf6TiSCH = check_and_cast<IETF6TiSCH *>(findModuleByPath(par("ietf6TiSCH")));
        ietf6TiSCH->switchRadioBroadcastDomain(MN_BROADCAST_DOMAIN);

        // register to the network coordinator
        ncCore = check_and_cast<NCCore *>(findModuleByPath(par("ncCore")));

        ncCore->registerMN(ietf6TiSCH->myMacAddress, 1 / upPeriod, 1 / downPeriod, upDelay, downDelay, ietf6TiSCH->mobility->currentX, ietf6TiSCH->mobility->currentY, -1);

        // set up graphics
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("b", 0, 10);
        str.setTagArg("b", 1, 10);
        str.setTagArg("b", 2, "oval");

        if (ietf6TiSCH->myMacAddress == 2000) {  // DEBUG
            str.setTagArg("b", 3, "blue");
        } else {
            str.setTagArg("b", 3, "lightblue");
        }

        packetsSent = registerSignal("packetsSent");

    } else if (stage == 4) {  // download schedule from manager
        ietf6TiSCH->updateSchedule({ietf6TiSCH->myMacAddress});

        if (upPeriod == -1) {
            // use the maximum rate allowed by the scenario
            upPeriod = ietf6TiSCH->schedule->frameLength * TIMESLOT_DURATION_S;
        }
        assert(upPeriod > 0);

        sendBeep = new cMessage("sendBeep");
        scheduleAt(simTime() + upPeriod + uniform(0, upPeriod, 0), sendBeep);

        seqNum = 0;
    }
}

void ConvergecastClient::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        ConvergecastPkt *pkt = new ConvergecastPkt("ConvergecastPkt");
        pkt->setByteLength(22);
        pkt->setSeqNum(seqNum);
        pkt->setSendTime(simTime());

        NetworkLayerPkt *netPkt = new NetworkLayerPkt();
        netPkt->setSrc(ietf6TiSCH->myMacAddress);
        netPkt->setDest(BR_BROADCAST_DOMAIN);
        netPkt->setSeq(seqNum);
        netPkt->setByteLength(22);
        netPkt->encapsulate(pkt);

        EV << "Packet sent " << pkt->getSeqNum() << endl;

        seqNum++;
        send(netPkt, "lowerLayerOut");
        emit(packetsSent, 1);

        scheduleAt(simTime() + upPeriod, sendBeep);
    }
}
