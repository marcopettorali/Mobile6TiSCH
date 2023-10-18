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

#include "BRMiddlewareBase.h"

Define_Module(BRMiddlewareBase);

void BRMiddlewareBase::initialize(int stage) {
    MiddlewareBase::initialize(stage);

    if (stage == INITSTAGE_PARAMS_FETCH) {
        crossLayerDB->set("brId", (int)getParentModule()->par("brId").intValue());
        crossLayerDB->set("broadcastAddress", BR_BROADCAST_ADDRESS);
    }

    if (stage == INITSTAGE_MODULES_FETCH) {
        ncMid = check_and_cast<NCMiddlewareBase*>(findModuleByPath(par("ncMid")));
        ncMid->registerBR(crossLayerDB->getInt("macAddress"), crossLayerDB->getInt("brId"), this, Point(crossLayerDB->getDouble("currentX"),
                          crossLayerDB->getDouble("currentY")), crossLayerDB->getDouble("targetCommunicationRadius"));

        ieee802154tsch = check_and_cast<IEEE802154TSCH*>(findModuleByPath(par("ieee802154tsch")));
    }

    if (stage == INITSTAGE_GRAPHICS_INIT) {
        // change displaystring
        cDisplayString& displayString = getParentModule()->getDisplayString();
        displayString.setTagArg("b", 0, 10);
        displayString.setTagArg("b", 1, 10);
        displayString.setTagArg("b", 2, "square");
        displayString.setTagArg("b", 3, "lightgreen");
    }

}

void BRMiddlewareBase::handleLowerPacket(cPacket* pkt) { handleNetworkPacket(pkt); }

// ASSUMPTION: upperLayer is connected to the backbone
void BRMiddlewareBase::handleUpperPacket(cPacket* pkt) { handleBackboneCommand(pkt); }

void BRMiddlewareBase::handleBackboneCommand(cPacket* pkt) {
    INF(crossLayerDB->getInt("macAddress")) << "Received downstream packet" << endl;

    BackboneCommand* backboneCmd = check_and_cast<BackboneCommand*>(pkt);

    // extract the backbone packet
    cPacket* dataPkt = backboneCmd->decapsulate();

    // encapsulate data in a network packet
    IEEE802154TSCHPkt* tschPkt = new IEEE802154TSCHPkt(NAME_ENCAPSULATE("IEEE802154TSCHPkt", dataPkt));

    tschPkt->setSrc(backboneCmd->getSrcEncapsulatedPkt());
    tschPkt->setDest(backboneCmd->getDestEncapsulatedPkt());
    tschPkt->setSeqNum(backboneCmd->getSeqNum());
    tschPkt->setUseControlCell(backboneCmd->getUseControlCellInTSCH());

    tschPkt->encapsulate(dataPkt);

    delete backboneCmd;
    send(tschPkt, "lowerLayerOut");
}

void BRMiddlewareBase::handleNetworkPacket(cPacket* pkt) {
    SHMGNetworkPkt* netPkt = check_and_cast<SHMGNetworkPkt*>(pkt);

    // disable retransmissions, waiting for the NC to choose the delegate BR
    setIEEE802154RetransmissionsEnabled(false);

    // encapsulate in a backbone packet
    BackboneDataInfo* backboneDataInfo = new BackboneDataInfo("backboneDataInfo");
    backboneDataInfo->setMobileNode(netPkt->getSrc());
    backboneDataInfo->setBorderRouter(crossLayerDB->getInt("macAddress"));
    backboneDataInfo->setSeq(netPkt->getSeq());
    backboneDataInfo->setRssidBm(crossLayerDB->getDouble("lastPacketReceivedRSSIdBm"));
    backboneDataInfo->encapsulate(netPkt);


    DBG(crossLayerDB->getInt("macAddress")) << "SHMGNETWORKPKT content" << endl;
    EV << netPkt->getSrc() << endl;
    EV << netPkt->getDest() << endl;
    EV << netPkt->getSeq() << endl;

    DBG(crossLayerDB->getInt("macAddress")) << "BACKBONEDATAINFO content" << endl;
    EV << backboneDataInfo->getMobileNode() << endl;
    EV << backboneDataInfo->getBorderRouter() << endl;
    EV << backboneDataInfo->getSeq() << endl;
    EV << backboneDataInfo->getRssidBm() << endl;

    // encapsulate to send to NC
    WiredNICPkt* wiredPkt = new WiredNICPkt(NAME_ENCAPSULATE("WiredNICPkt", backboneDataInfo));
    wiredPkt->setOutPort(0);
    wiredPkt->encapsulate(backboneDataInfo);
    send(wiredPkt, "upperLayerOut");  // ASSUMPTION: upperLayer is connected to the backbone
}

void BRMiddlewareBase::setIEEE802154RetransmissionsEnabled(bool retransmissionsEnabled) {
    ieee802154tsch->setRetransmissionsEnabledInDedicatedCells(retransmissionsEnabled);
    ieee802154tsch->setRetransmissionsEnabledInSharedCells(retransmissionsEnabled);
}