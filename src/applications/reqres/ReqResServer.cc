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

#include "applications/reqres/ReqResServer.h"

Define_Module(ReqResServer);

void ReqResServer::initialize(int stage) { ApplicationBase::initialize(stage); }

void ReqResServer::handleMessage(cMessage *msg) {
    ReqResPkt *pkt = check_and_cast<ReqResPkt *>(msg);
    INF("NC") << "Received upstream packet " << pkt->getSeqNum() << " from " << pkt->getSenderMACAddress() << endl;

    // stats
    statsCollector->record(0, "upstreamPacketsReceived", 1);
    statsCollector->record(0, "upstreamDelay", simTime().dbl() - pkt->getUpSendTime().dbl());

    // send it back
    pkt->setName(NAME_INIT("ResPkt", BR_BROADCAST_ADDRESS, pkt->getSeqNum())); 
    pkt->setDownSendTime(simTime());

    SHMGNetworkPkt *netPkt = new SHMGNetworkPkt(NAME_ENCAPSULATE("SHMGNetworkPkt", pkt));
    netPkt->setSrc(BR_BROADCAST_ADDRESS); // ASSUMPTION: ReqRes Server is on the NC // ASSUMPTION: NC has macAddress = BR_BROADCAST_ADDRESS
    netPkt->setDest(pkt->getSenderMACAddress());
    netPkt->setSeq(pkt->getSeqNum());
    netPkt->encapsulate(pkt);

    // send to lower layer
    send(netPkt, "lowerLayerOut");

    statsCollector->record(0, "downstreamPacketsSent", 1);
    INF("NC") << "Sent downstream packet " << pkt->getSeqNum() << " to " << pkt->getSenderMACAddress() << endl;
}
