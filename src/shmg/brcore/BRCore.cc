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

#include "BRCore.h"

Define_Module(BRCore);

void BRCore::initialize(int stage) {
    if(stage == 1){
        this->ietf6TiSCH = check_and_cast<IETF6TiSCH*>(findModuleByPath(par("ietf6TiSCH")));
        this->ncCore = check_and_cast<NCCore*>(findModuleByPath(par("ncCore")));
        this->brId = getParentModule()->par("brId");

        ncCore->registerBR(ietf6TiSCH->myMacAddress, brId, this, ietf6TiSCH->mobility->currentX, ietf6TiSCH->mobility->currentY, ietf6TiSCH->mobility->par("targetCommunicationRadius").doubleValue());
    }else if (stage == 4){
        ietf6TiSCH->updateSchedule({});
        ietf6TiSCH->switchRadioBroadcastDomain(BR_BROADCAST_DOMAIN);
    }
}

void BRCore::handleMessage(cMessage *msg) {
    if (msg->getArrivalGate() == gate("lowerLayerBackboneIn")) {
        handleBackboneCommand(msg);
    }else if(msg->getArrivalGate() == gate("lowerLayer6TiSCHIn")){
        handleNetworkPacket(msg);
    }
}

void BRCore::handleBackboneCommand(cMessage* msg) {
    BackbonePkt* pkt = check_and_cast<BackbonePkt*>(msg);
    NetworkLayerPkt* netPkt = new NetworkLayerPkt("netPkt");
    
    netPkt->setSrc(BR_BROADCAST_DOMAIN);
    netPkt->setDest(pkt->getMobileNode());
    netPkt->setSeq(-1);
    netPkt->setByteLength(pkt->getByteLength());
    netPkt->encapsulate(pkt);

    send(netPkt, "lowerLayer6TiSCHOut");
}

void BRCore::handleNetworkPacket(cMessage* msg) {
    NetworkLayerPkt* netPkt = check_and_cast<NetworkLayerPkt*>(msg);
    SignalDescriptor* desc = check_and_cast<SignalDescriptor*>(netPkt->removeControlInfo());
    
    BackbonePkt* pkt = new BackbonePkt();
    pkt->setMobileNode(netPkt->getSrc());
    pkt->setBorderRouter(this->ietf6TiSCH->myMacAddress);
    pkt->setSeq(netPkt->getSeq());
    pkt->setRssi(desc->getRssi());    
    pkt->setOutPort(0);
    pkt->encapsulate(netPkt->decapsulate());

    send(pkt, "lowerLayerBackboneOut");

    delete desc;
    delete netPkt;
}
