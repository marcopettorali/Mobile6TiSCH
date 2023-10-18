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

#include "MNMiddlewareBase.h"

Define_Module(MNMiddlewareBase);

void MNMiddlewareBase::initialize(int stage) {
    MiddlewareBase::initialize(stage);

    if (stage == INITSTAGE_PARAMS_FETCH) {
        // retrieve id from parent module
        crossLayerDB->set("mnId", (int)getParentModule()->par("mnId").intValue());

        // set broadcast address
        crossLayerDB->set("broadcastAddress", MN_BROADCAST_ADDRESS);
    }

    if (stage == INITSTAGE_MODULES_FETCH) {
        // register to the NC
        ncMid = check_and_cast<NCMiddlewareBase *>(findModuleByPath(par("ncMid")));

        ncMid->registerMN(crossLayerDB->getInt("macAddress"), crossLayerDB->getInt("mnId"), 1 / crossLayerDB->getDouble("upPeriod"),
                          Point(crossLayerDB->getDouble("currentX"), crossLayerDB->getDouble("currentY")),
                          Point(crossLayerDB->getDouble("directionX"), crossLayerDB->getDouble("directionY")), crossLayerDB->getDouble("speed"));

        // retrieve TSCH module
        ieee802154tsch = check_and_cast<IEEE802154TSCH *>(findModuleByPath(par("ieee802154tsch")));
    }
}

void MNMiddlewareBase::handleSelfMessage(cMessage *msg) { MiddlewareBase::handleSelfMessage(msg); }

void MNMiddlewareBase::handleUpperPacket(cPacket *pkt) {
    // update seqNum and set useShared to false
    SHMGNetworkPkt *netPkt = check_and_cast<SHMGNetworkPkt *>(pkt);
    netPkt->setSeq(crossLayerDB->getInt("macAddress") * 10000 + packetsSent++);

    // send to NC
    sendDown(netPkt);
}

void MNMiddlewareBase::handleLowerPacket(cPacket *pkt) {
    SHMGNetworkPkt *netPkt = check_and_cast<SHMGNetworkPkt *>(pkt);

    // deduplicate
    if (deduplicationTable.find(netPkt->getSeq()) != deduplicationTable.end()) {
        delete netPkt;
        return;
    }

    deduplicationTable[netPkt->getSeq()] = simTime();

    // decapsulate and send to upper layer
    cPacket *upperLayerPkt = netPkt->decapsulate();
    send(upperLayerPkt, "upperLayerOut");
    delete netPkt;
}

void MNMiddlewareBase::sendDown(SHMGNetworkPkt *netPkt) {
    // encapsulate in a IEEE802154TSCHPkt
    IEEE802154TSCHPkt *tschPkt = new IEEE802154TSCHPkt(NAME_ENCAPSULATE("IEEE802154TSCHPkt", netPkt));
    tschPkt->setSrc(netPkt->getSrc());
    tschPkt->setDest(netPkt->getDest());
    tschPkt->setSeqNum(netPkt->getSeq());
    tschPkt->setUseControlCell(false);
    tschPkt->encapsulate(netPkt);

    // send to link layer
    send(tschPkt, "lowerLayerOut");
}