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

#include "WiredNICBase.h"

Define_Module(WiredNICBase);

void WiredNICBase::initialize(int stage) {
    BaseModule::initialize(stage);

    if (stage == INITSTAGE_PARAMS_FETCH) {
        this->numWirings = (int)par("numWirings");
    }
}

void WiredNICBase::handleMessage(cMessage* msg) {
    if (msg->getArrivalGate() == gate("upperLayerIn")) {
        cPacket* pkt = check_and_cast<cPacket*>(msg);
        handleUpperPacket(pkt);
    } else {
        cPacket* pkt = check_and_cast<cPacket*>(msg);
        handleLowerPacket(pkt);
    }
}

void WiredNICBase::handleLowerPacket(cPacket* pkt) {
    WiredNICPkt* wiredPkt = check_and_cast<WiredNICPkt*>(pkt);

    // decapsulate and send to upper layer
    cPacket* upperLayerPkt = wiredPkt->decapsulate();
    send(upperLayerPkt, "upperLayerOut");
    delete wiredPkt;
}

void WiredNICBase::handleUpperPacket(cPacket* pkt) {
    WiredNICPkt* wiredPkt = check_and_cast<WiredNICPkt*>(pkt);

    // send through lower layer
    send(pkt, "lowerLayerOut", wiredPkt->getOutPort());
    INF("WiredNICBase") << "send packet to port " << wiredPkt->getOutPort() << endl;
}
