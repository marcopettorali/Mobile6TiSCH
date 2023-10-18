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

#include "applications/reqres/ReqResClient.h"

#include <fstream>
Define_Module(ReqResClient);

void ReqResClient::initialize(int stage) {
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_PARAMS_FETCH) {
        packetSizeBytes = par("packetSizeBytes");
        upPeriod = par("upPeriod");
        batchSize = par("batchSize");

        crossLayerDB->set("upPeriod", upPeriod);
        crossLayerDB->set("packetSizeBytes", packetSizeBytes);
    }

    if (stage == INITSTAGE_BEEP_INIT) {
        sendBeep = new cMessage("sendBeep");
        scheduleAt(simTime() + upPeriod + uniform(0, upPeriod, RNG_APP_DELAYED_INIT), sendBeep);
    }

    if (stage == INITSTAGE_GRAPHICS_INIT) {
        cDisplayString &str = getParentModule()->getDisplayString();
        str.setTagArg("b", 0, 10);
        str.setTagArg("b", 1, 10);
        str.setTagArg("b", 2, "oval");

        if (crossLayerDB->getInt("macAddress") == 20000) {  // DEBUG
            str.setTagArg("b", 3, "blue");
        } else {
            str.setTagArg("b", 3, "lightblue");
        }
    }
}

void ReqResClient::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg->isName("sendBeep")) {
        // create packets in batch
        for (int i = 0; i < batchSize; i++) {
            ReqResPkt *reqresPkt = new ReqResPkt(NAME_INIT("ReqPkt", crossLayerDB->getInt("macAddress"), seqNum));
            reqresPkt->setSenderMACAddress(crossLayerDB->getInt("macAddress"));
            reqresPkt->setByteLength(packetSizeBytes);
            reqresPkt->setSeqNum(seqNum++);
            reqresPkt->setUpSendTime(simTime());

            // encapsulate in lower layer packet // ASSUMPTION: NETWORK LAYER IS SHMG
            SHMGNetworkPkt *netPkt = new SHMGNetworkPkt(NAME_ENCAPSULATE("SHMGNetworkPkt", reqresPkt));
            netPkt->setSrc(crossLayerDB->getInt("macAddress"));
            netPkt->setDest(BR_BROADCAST_ADDRESS);
            netPkt->encapsulate(reqresPkt);

            // send packet
            send(netPkt, "lowerLayerOut");

            INF(crossLayerDB->getInt("macAddress")) << "Sending packet " << reqresPkt->getSeqNum() << endl;
            // update stats
            statsCollector->record(crossLayerDB->getInt("macAddress"), "upstreamPacketsSent", 1);
        }

        // schedule next beep
        scheduleAt(simTime() + upPeriod, sendBeep);

    } else {
        ReqResPkt *pkt = check_and_cast<ReqResPkt *>(((cPacket *)msg));
        statsCollector->record(crossLayerDB->getInt("macAddress"), "downstreamPacketsReceived", 1);
        statsCollector->record(crossLayerDB->getInt("macAddress"), "endToEndDelay", simTime().dbl() - pkt->getUpSendTime().dbl());
        statsCollector->record(crossLayerDB->getInt("macAddress"), "downstreamDelay", simTime().dbl() - pkt->getDownSendTime().dbl());
        INF(crossLayerDB->getInt("macAddress")) << "Received downstream packet " << pkt->getName() << endl;
        delete msg;
    }
}