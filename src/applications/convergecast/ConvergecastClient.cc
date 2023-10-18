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

#include "applications/convergecast/ConvergecastClient.h"

#include <fstream>
Define_Module(ConvergecastClient);

void ConvergecastClient::initialize(int stage) {
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

void ConvergecastClient::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg->isName("sendBeep")) {
        // create packets in batch
        for (int i = 0; i < batchSize; i++) {
            ConvergecastPkt *convPkt = new ConvergecastPkt(NAME_INIT("ConvergecastPkt", crossLayerDB->getInt("macAddress"), seqNum));
            convPkt->setSenderMACAddress(crossLayerDB->getInt("macAddress"));
            convPkt->setByteLength(packetSizeBytes);
            convPkt->setSeqNum(seqNum++);
            convPkt->setSendTime(simTime());

            // encapsulate in lower layer packet // ASSUMPTION: NETWORK LAYER IS SHMG
            SHMGNetworkPkt *netPkt = new SHMGNetworkPkt(NAME_ENCAPSULATE("SHMGNetworkPkt", convPkt));
            netPkt->setSrc(crossLayerDB->getInt("macAddress"));
            netPkt->setDest(BR_BROADCAST_ADDRESS);
            netPkt->encapsulate(convPkt);

            // send packet
            send(netPkt, "lowerLayerOut");

            INF(crossLayerDB->getInt("macAddress")) << "Sending packet " << convPkt->getSeqNum() << endl;
            
            // update stats
            statsCollector->record(crossLayerDB->getInt("macAddress"), "upstreamPacketsSent", 1);
        }

        // schedule next beep
        scheduleAt(simTime() + upPeriod, sendBeep);

    } else {
        ApplicationBase::handleMessage(msg);
    }
}