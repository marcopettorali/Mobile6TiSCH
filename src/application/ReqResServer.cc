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

#include "ReqResServer.h"

Define_Module(ReqResServer);

void ReqResServer::initialize() {
    packetsSent = registerSignal("packetsSent");
    packetsReceived = registerSignal("packetsReceived");
    packetsDelay = registerSignal("packetsDelay");
    packetsUpstreamDelay = registerSignal("packetsUpstreamDelay");
}

void ReqResServer::handleMessage(cMessage *msg) {
    ReqResPkt *pkt = check_and_cast<ReqResPkt *>(msg);

    emit(packetsReceived, 1);
    emit(packetsUpstreamDelay, simTime() - pkt->getUpSendTime());

    // send it back
    pkt->setDownSendTime(simTime());

    BackbonePkt *backPkt = new BackbonePkt();
    backPkt->setMobileNode(pkt->getSenderMACAddress());
    
    send(backPkt, "lowerLayerOut");

    EV << "Received upstream packet" << endl;


    delete pkt;
}
