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

#include "IEEE802154TSCH.h"

Define_Module(IEEE802154TSCH);

IEEE802154TSCH::~IEEE802154TSCH() { cancelAndDelete(scheduleBeep); }

void IEEE802154TSCH::initialize(int stage) {
    BaseModule::initialize(stage);
    if (stage == INITSTAGE_PARAMS_FETCH) {
        // fetch parameters
        queueLength = par("queueLength");
        enableRetransmissionsInSharedCells = par("enableRetransmissionsInSharedCells");
        enableRetransmissionsInDedicatedCells = par("enableRetransmissionsInDedicatedCells");

        flagAcksDedCells = enableRetransmissionsInDedicatedCells;
        flagAcksShCells = enableRetransmissionsInSharedCells;

        // ASSUMPTION: retransmissions in dedicated cells are not implemented
        if (enableRetransmissionsInDedicatedCells) {
            throw cRuntimeError("Retransmissions in dedicated cells are not implemented");
        }

        // csma-ca
        macMinBE = par("macMinBE");
        macMaxBE = par("macMaxBE");
        macMaxFrameRetries = par("macMaxFrameRetries");
    }

    if (stage == INITSTAGE_MODULES_FETCH) {
        // retrieve mac address written by radio module in cross-layer database
        macAddress = crossLayerDB->getInt("macAddress");

        // retrieve pointers to other modules
        radio = check_and_cast<RadioBase *>(findModuleByPath(par("radio")));
        schedulingFunction = check_and_cast<SchedulingFunctionBase *>(findModuleByPath(par("schedulingFunction")));

        // init TSCH schedule
        broadcastAddress = crossLayerDB->getInt("broadcastAddress");
        radio->setBroadcastDomain(broadcastAddress);
        scheduleBeep = new cMessage("scheduleBeep");
        scheduleAt(simTime() + TIMESLOT_DURATION_S, scheduleBeep);
    }

    if (stage == INITSTAGE_STATS_SUBSCRIPTION) {
        statsCollector->subscribeStats("packetsDroppedNotListenedMN", STAT_TYPE_SUM);
        statsCollector->subscribeStats("ackReceived", STAT_TYPE_SUM);
        statsCollector->subscribeStats("ackSent", STAT_TYPE_SUM);
        statsCollector->subscribeStats("packetsDroppedCSMACA", STAT_TYPE_SUM);
    }

    if (stage == INITSTAGE_LAST_STAGE) {
        // get schedule from scheduling function
        schedule = schedulingFunction->getSchedule(macAddress, broadcastAddress);

        schedule->printSchedule(EV);
    }
}

void IEEE802154TSCH::handleSelfMessage(cMessage *msg) {
    if (msg->isName("scheduleBeep")) {
        // new timeslot has elapsed
        onScheduleBeep();
    } else if (msg->isName("checkSendACK")) {
        DBG(macAddress) << "Checking if ACK can be sent" << endl;
        IEEE802154TSCHPkt *tschPkt = check_and_cast<IEEE802154TSCHPkt *>(msg->removeControlInfo());

        // send ACK if enabled
        if (areRetransmissionsEnabled()) {
            sendAck(tschPkt);
        }

        delete tschPkt;
        delete msg;
    }
}

void IEEE802154TSCH::handleUpperPacket(cPacket *pkt) {
    IEEE802154TSCHPkt *tschPkt = check_and_cast<IEEE802154TSCHPkt *>(pkt);

    // schedule packet for transmission
    enqueuePacket(tschPkt, tschPkt->getUseControlCell());
}

void IEEE802154TSCH::handleLowerPacket(cPacket *pkt) {
    // if TSCH packet received
    IEEE802154TSCHPkt *tschPkt = check_and_cast<IEEE802154TSCHPkt *>(pkt);

    // if the packet is not from the MN I'm listening to, drop it
    if (listenedMacAddr != -1 && tschPkt->getSrc() != listenedMacAddr) {
        WRN(macAddress) << "Packet from " << tschPkt->getSrc() << " received, but expected from " << listenedMacAddr << endl;
        statsCollector->record(macAddress, "packetsDroppedNotListenedMN", 1);

        delete tschPkt;
        return;
    }

    // if ACK received
    if (tschPkt->getKind() == KIND_ACK) {
        if (areRetransmissionsEnabled()) {
            DBG(macAddress) << "ACK received, processing" << endl;
            handleAck(check_and_cast<IEEE802154TSCHAck *>(tschPkt->decapsulate()));
        }

        delete tschPkt;
        return;
    }

    // if data packet received
    cPacket *upperLayerPkt = check_and_cast<cPacket *>(tschPkt->decapsulate());
    send(upperLayerPkt, "upperLayerOut");
    DBG(macAddress) << "Packet sent to upper layer" << endl;

    // defer ACK transmission in case of any policy at the upper layers // TODO CHECK POOR IMPLEMENTATION DESIGN
    cMessage *checkSendACK = new cMessage("checkSendACK");
    checkSendACK->setControlInfo(tschPkt);
    scheduleAt(simTime() + GUARD_TIME_S, checkSendACK);
}

// ACK HANDLING
void IEEE802154TSCH::handleAck(IEEE802154TSCHAck *ack) {
    // check if packet is in csmaCaState
    if (csmaCaState.find(ack->getSeqNum()) != csmaCaState.end()) {
        // delete the packet
        delete csmaCaState[ack->getSeqNum()].originalPkt;
        // delete the entry from the csmaCaState table
        csmaCaState.erase(ack->getSeqNum());
        DBG(macAddress) << "Packet " << ack->getSeqNum() << " ACKed" << endl;
    }

    // update stats
    statsCollector->record(macAddress, "ackReceived", 1);

    delete ack;
}

void IEEE802154TSCH::sendAck(IEEE802154TSCHPkt *tschPkt) {
    // send ack
    IEEE802154TSCHAck *ack = new IEEE802154TSCHAck(NAME_INIT("ACK", crossLayerDB->getInt("macAddress"), tschPkt->getSeqNum()));
    ack->setSeqNum(tschPkt->getSeqNum());

    IEEE802154TSCHPkt *ackEnv = new IEEE802154TSCHPkt(NAME_ENCAPSULATE("IEEE802154TSCHPkt", ack));
    ackEnv->setByteLength(22);
    ackEnv->setKind(KIND_ACK);
    ackEnv->setSrc(macAddress);
    ackEnv->setDest(tschPkt->getSrc());
    ackEnv->encapsulate(ack);

    switchRadioMode(RadioMode::TX);
    send(encapsulateInRadioPkt(ackEnv), "lowerLayerOut");

    DBG(macAddress) << "ACK sent for packet " << tschPkt->getSeqNum() << " to " << tschPkt->getSrc() << endl;

    // update stats
    statsCollector->record(macAddress, "ackSent", 1);
}

// EVENTS

void IEEE802154TSCH::onScheduleBeep() {
    // check unacked packets at the end of the previous timeslot
    if (areRetransmissionsEnabled()) {
        checkUnackedPackets();
    }

    // increase ASN
    asn++;
    currentTs = asn % schedule->slotframeLength;

    if (currentTs == 0) {
        onSlotframeStart();
    }

    // get queue size
    statsCollector->record(macAddress, "queueSize", (int)packetsQueue.size());

    // get target cells and decide action
    std::vector<cell_t> cells = schedule->getCellsInTimeslot(currentTs);

    // check any shared cell to update remainingW
    std::vector<cell_t> sharedCells;
    for (auto &cell : cells) {
        if (cell.isShared) {
            sharedCells.push_back(cell);
        }
    }

    // update remainingW for packets in shared cells
    for (auto &cell : sharedCells) {
        if (cell.isShared) {
            updateRemainingWForPacketsInCell(cell);
        }
    }

    // check action to do in this timeslot
    if (cells.size() == 0) {
        onNoCellsInTimeslot();
    } else if (cells.size() == 1) {
        onSingleCellInTimeslot(cells[0]);
    } else {
        // invoke scheduling function conflict management policy
        cells = schedulingFunction->manageConflictInTimeslot(currentTs, cells, macAddress, broadcastAddress);
        onMultipleCellsInTimeslot(cells);
    }

    // schedule next beep
    scheduleAt(simTime() + TIMESLOT_DURATION_S, scheduleBeep);
}

void IEEE802154TSCH::checkUnackedPackets() {
    // reinsert packets for CSMA-CA retransmission not already in queue
    for (auto &elem : csmaCaState) {
        bool found = false;
        for (auto it = packetsQueue.begin(); it != packetsQueue.end(); it++) {
            if (it->packet->getSeqNum() == elem.second.originalPkt->getSeqNum()) {
                found = true;
                break;
            }
        }

        if (!found) {
            // insert packet back in queue
            enqueuePacket(elem.second.originalPkt->dup(), elem.second.originalPkt->getUseControlCell());
        }
    }
}

void IEEE802154TSCH::updateRemainingWForPacketsInCell(cell_t cell) {
    for (auto &elem : csmaCaState) {
        // if packet could be sent in the cell
        if (elem.second.originalPkt->getDest() == cell.rx && elem.second.originalPkt->getUseControlCell() == cell.isControlCell) {
            // consider this cell as elapsed
            if (elem.second.remainingW > 0) {
                elem.second.remainingW--;

                DBG(macAddress) << "Packet " << elem.second.originalPkt->getSeqNum() << " has remainingW = " << elem.second.remainingW << endl;
            }
        }
    }
}

// RADIO MANAGEMENT
void IEEE802154TSCH::switchRadioMode(RadioMode status) { radio->setRadioMode(status); }

void IEEE802154TSCH::switchRadioFrequency(long long channel) {
    long long frequency = channel;  // baseFrequency + baseBandwidth * ((asn + channel) % CHANNELS);
    radio->setCarrierFrequency(frequency);
}

// PACKETS QUEUE

void IEEE802154TSCH::enqueuePacket(IEEE802154TSCHPkt *pkt, bool useControlCell) {
    if (queueLength == -1 || packetsQueue.size() < queueLength) {
        packetsQueue.push_back({pkt->getDest(), useControlCell, pkt});

        // print queue
        DBG(macAddress) << "Packet " << pkt->getSeqNum() << " enqueued for " << pkt->getDest() << endl;
        for (auto &elem : packetsQueue) {
            EV << "Packet " << elem.packet << endl;
        }

    } else {
        statsCollector->record(macAddress, "packetsDropped", 1);
        delete pkt;
    }
}

bool IEEE802154TSCH::hasEnquedPacketForMN(int rxMacAddress, bool useControlCell) {
    for (auto it = packetsQueue.begin(); it != packetsQueue.end(); it++) {
        if (it->rxMacAddress == rxMacAddress && it->useControlCell == useControlCell) {
            return true;
        }
    }
    return false;
}

IEEE802154TSCHPkt *IEEE802154TSCH::popFirstEnqueuedPacket(int macAddress, bool useControlCell) {
    IEEE802154TSCHPkt *pkt = nullptr;
    for (auto it = packetsQueue.begin(); it != packetsQueue.end(); it++) {
        if (it->rxMacAddress == macAddress && it->useControlCell == useControlCell) {
            pkt = it->packet;
            packetsQueue.erase(it);
            break;
        }
    }
    return pkt;
}

void IEEE802154TSCH::sendToLowerLayer(IEEE802154TSCHPkt *tschPkt) { sendDelayed(encapsulateInRadioPkt(tschPkt), GUARD_TIME_S, "lowerLayerOut"); }

// ENCAPSULATION UTILITY

RadioPkt *IEEE802154TSCH::encapsulateInRadioPkt(IEEE802154TSCHPkt *tschPkt) {
    RadioPkt *radioPkt = new RadioPkt(NAME_ENCAPSULATE("RadioPkt", tschPkt));
    radioPkt->setId(intuniform(0, std::numeric_limits<int>::max(), RNG_RADIOPKT_RANDOM_ID));
    radioPkt->setSrc(tschPkt->getSrc());
    radioPkt->setDest(tschPkt->getDest());
    radioPkt->encapsulate(tschPkt);
    return radioPkt;
}

// EVENT HANDLERS

void IEEE802154TSCH::onNoCellsInTimeslot() {
    // nothing to do now, go to sleep :)
    switchRadioMode(RadioMode::SLEEP);
}

void IEEE802154TSCH::onMultipleCellsInTimeslot(std::vector<cell_t> cells) {
    // check that there is at most one RX cell
    int countRxCells =
        std::count_if(cells.begin(), cells.end(), [this](cell_t const &cell) { return cell.rx == macAddress || cell.rx == broadcastAddress; });

    if (countRxCells > 1) {
        throw cRuntimeError("Multiple RX cells in timeslot are not supported");
    }

    // prioritize transmissions
    // if there are more than one TX cell, prioritize transmission in shared cells // TODO CHECK IF NECESSARY
    std::vector<cell_t> txCells;
    std::copy_if(cells.begin(), cells.end(), std::back_inserter(txCells),
                 [this](cell_t const &cell) { return cell.tx == macAddress || cell.tx == broadcastAddress; });

    // shuffle selectedCells to avoid starvation
    std::vector<cell_t> txCellsShuffled;
    while (txCells.size() > 0) {
        int index = intuniform(0, txCells.size() - 1, RNG_SHUFFLE);
        txCellsShuffled.push_back(txCells[index]);
        txCells.erase(txCells.begin() + index);
    }

    // try to transmit in at least one cell, starting from the shared ones
    for (auto &cell : txCellsShuffled) {
        bool ret = onSingleCellInTimeslot(cell);
        if (ret) return;
    }

    // if no transmission succeded, but there is a RX cell, set the radio to RX
    if (countRxCells == 1) {
        cell_t rxCell =
            *std::find_if(cells.begin(), cells.end(), [this](cell_t const &cell) { return cell.rx == macAddress || cell.rx == broadcastAddress; });
        onSingleCellInTimeslot(rxCell);
        return;
    }

    // if no transmission succeded and there are no RX cells, go to sleep
    onNoCellsInTimeslot();
}

bool IEEE802154TSCH::onSingleCellInTimeslot(cell_t cell) {
    // check if there are packets to send or receive
    bool hasEnqueuedPacket = hasEnquedPacketForMN(cell.rx, cell.isControlCell);

    // If I have a packet to transmit
    if (hasEnqueuedPacket) {
        // extract packet from queue
        IEEE802154TSCHPkt *pkt = popFirstEnqueuedPacket(cell.rx, cell.isControlCell);

        // if cell is shared
        if (cell.isShared) {
            DBG(macAddress) << "Shared cell " << cell.channel << " with " << cell.tx << endl;
            // check if the packet has already been transmitted
            if (csmaCaState.find(pkt->getSeqNum()) != csmaCaState.end()) {
                if (csmaCaState[pkt->getSeqNum()].remainingW > 0) {
                    DBG(macAddress) << "Packet " << pkt->getSeqNum() << " has remainingW = " << csmaCaState[pkt->getSeqNum()].remainingW
                                    << ", cannot be transmitted yet" << endl;
                    // the packet cannot be transmitted yet
                    delete pkt;
                    return false;
                }

                // if the packet can be transmitted
                // increase retransmission counter
                csmaCaState[pkt->getSeqNum()].nb++;

                DBG(macAddress) << "Packet " << pkt->getSeqNum() << " has NB = " << csmaCaState[pkt->getSeqNum()].nb << endl;

                // check if the packet has reached the maximum number of retransmissions
                if (csmaCaState[pkt->getSeqNum()].nb > macMaxFrameRetries) {
                    // delete the entry from the csmaCaState table
                    delete csmaCaState[pkt->getSeqNum()].originalPkt;
                    csmaCaState.erase(pkt->getSeqNum());

                    // record stats
                    statsCollector->record(macAddress, "packetsDroppedCSMACA", 1);

                    DBG(macAddress) << "Packet " << pkt->getSeqNum() << " dropped after " << macMaxFrameRetries << " retransmissions" << endl;

                    // delete the packet
                    delete pkt;

                    return false;
                }

                // update CSMA-CA parameters
                csmaCaState[pkt->getSeqNum()].be = std::min(csmaCaState[pkt->getSeqNum()].be + 1, macMaxBE);
                csmaCaState[pkt->getSeqNum()].w = intuniform(0, (int)pow(2, csmaCaState[pkt->getSeqNum()].be) - 1, RNG_CSMA_CA_W);
                csmaCaState[pkt->getSeqNum()].remainingW = csmaCaState[pkt->getSeqNum()].w;

                DBG(macAddress) << "Packet " << pkt->getSeqNum() << " has BE = " << csmaCaState[pkt->getSeqNum()].be << endl;
                DBG(macAddress) << "Packet " << pkt->getSeqNum() << " has W = " << csmaCaState[pkt->getSeqNum()].w << endl;

            } else {
                // insert new entry in the csmaCaState table
                csma_ca_elem_t elem;
                elem.originalPkt = pkt->dup();
                elem.nb = 0;
                elem.be = macMinBE;
                elem.w = intuniform(0, (int)pow(2, elem.be) - 1, RNG_CSMA_CA_W);
                elem.remainingW = elem.w;

                csmaCaState[pkt->getSeqNum()] = elem;

                DBG(macAddress) << "First transmission of packet " << pkt->getSeqNum() << endl;
                DBG(macAddress) << "BE = " << elem.be << ", W = " << elem.w << endl;
            }

            // set dedicated cell in order to trigger acks correctly
            dedicatedCell = !cell.isShared;
            DBG(macAddress) << "Dedicated cell = " << dedicatedCell << endl;

            // if transmission with shared cell
            // enable reception of ACKs
            radio->enableRxAfterTransmission();
            // enable reception of ACK from any src
            listenedMacAddr = -1;
            DBG(macAddress) << "Enabled RX after transmission" << endl;
        }

        // prepare radio for transmission
        switchRadioMode(RadioMode::TX);
        switchRadioFrequency(cell.channel);
        sendToLowerLayer(pkt);
        INF(macAddress) << " transmitting on channel " << cell.channel << endl;
        return true;
    }

    // If I have to receive a packet
    if (cell.isControlCell || cell.rx == macAddress || cell.rx == broadcastAddress) {
        // update listenedMacAddr
        if (cell.isShared) {
            listenedMacAddr = -1;  // listen to any node in shared cell
        } else {
            listenedMacAddr = cell.tx;
        }

        // set dedicated cell in order to trigger acks correctly
        dedicatedCell = !cell.isShared;
        DBG(macAddress) << "Dedicated cell = " << dedicatedCell << endl;

        // prepare radio for reception
        switchRadioMode(RadioMode::RX);
        switchRadioFrequency(cell.channel);
        INF(macAddress) << " receiving on channel " << cell.channel << endl;
        return true;
    }

    // pheww, nothing to do. I sleep.
    switchRadioMode(RadioMode::SLEEP);
    return false;
}

void IEEE802154TSCH::onSlotframeStart() {
    if (macAddress == 10000) INF(macAddress) << "SLOTFRAME START" << endl;

    // print csmacastate
    DBG(macAddress) << "CSMA-CA state:" << endl;
    for (auto &elem : csmaCaState) {
        EV << "Packet " << elem.first << " has " << elem.second.nb << " retransmissions and remainingW is " << elem.second.remainingW << endl;
    }
}