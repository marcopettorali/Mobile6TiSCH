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

#include "BackboneProtocol.h"

Define_Module (BackboneProtocol);

void BackboneProtocol::initialize() {
    this->numBackboneWirings = (int)par("numBackboneWirings");
}

void BackboneProtocol::handleMessage(cMessage *msg) {
    if(msg->getArrivalGate() == gate("upperLayerIn")){
        handleCommand(msg);
    }else{ //lowerlayer
        handlePacket(msg);
    }
}


//TODO implement your own middleware
void BackboneProtocol::handlePacket(cMessage* msg) {
    send(msg, "upperLayerOut");
}

void BackboneProtocol::handleCommand(cMessage* msg) {
    BackbonePkt* pkt = check_and_cast<BackbonePkt*>(msg);
    send(pkt, "lowerLayerOut", pkt->getOutPort());
}
