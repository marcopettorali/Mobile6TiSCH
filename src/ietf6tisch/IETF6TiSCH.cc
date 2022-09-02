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

#include "IETF6TiSCH.h"

Define_Module(IETF6TiSCH);

void IETF6TiSCH::initialize(int stage) {
    if (stage == 1) {
        // retrieve pointers to other modules
        radio = check_and_cast<Radio*>(findModuleByPath(par("radio")));
        mobility = check_and_cast<MobilityBase*>(
                findModuleByPath(par("mobility")));
        schedulingFunction = check_and_cast<SchedulingFunctionBase*>(
                findModuleByPath(par("schedulingFunction")));

        // init parameters
        baseBandwidth = par("baseBandwidth");
        baseFrequency = par("baseFrequency");
        queueLength = par("queueLength");
        maxRetransmissions = par("maxRetransmissions");
        myMacAddress = getParentModule()->getSubmodule("radio")->par(
                "macAddress");
        asn = 0;
        areRetransmissionsEnabled = par("areRetransmissionsEnabled");

        // init TSCH schedule
        radio->setBroadcastDomain(broadcastDomain);
        scheduleBeep = new cMessage("scheduleBeep");
        scheduleAt(simTime() + TIMESLOT_DURATION_S, scheduleBeep);

        // register statistics
        queueSizeSignal = registerSignal("queueSize");
        packetsDroppedSignal = registerSignal("packetsDropped");
        retransmissionsSignal = registerSignal("retransmissions");
        retransmissionsIntertimeSignal = registerSignal(
                "retransmissionsIntertime");
        retransmissionsCountSignal = registerSignal("retransmissionsCount");
        atLeastOneRetransmissionSignal = registerSignal(
                "atLeastOneRetransmission");
    }
}

void IETF6TiSCH::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg->isName("scheduleBeep")) {
        onScheduleBeep();
    } else if (msg->getArrivalGate() == gate("lowerLayerIn")
            and msg->getKind() < 0) {  // notify from radio
        if (msg->getKind() == KIND_NOTIFY_PACKET_SENT) {
            if (areRetransmissionsEnabled) {
                radio->setRadioMode(RadioMode::RX);
            }
        }
    } else if (msg->getArrivalGate() == gate("lowerLayerIn")) {  // from network
        IETF6TiSCHPkt *tschPkt = check_and_cast<IETF6TiSCHPkt*>(msg);

        if (areRetransmissionsEnabled && tschPkt->getKind() == KIND_ACK) {
            IETF6TiSCHAck *ack = check_and_cast<IETF6TiSCHAck*>(
                    tschPkt->decapsulate());

            for (int i = 0; i < ackTable.size(); i++) {
                ack_table_elem *elem = &ackTable[i];
                if (elem->seqNum == ack->getSeqNum()) {
                    elem->count += 1;
                }
            }

            delete ack;

        } else {  // non-ACK packet
            if (areRetransmissionsEnabled) {
                // send ack
                IETF6TiSCHAck *ack = new IETF6TiSCHAck();
                ack->setSeqNum(tschPkt->getSeqNum());

                IETF6TiSCHPkt *ackEnv = new IETF6TiSCHPkt();
                ackEnv->setByteLength(22);
                ackEnv->setKind(KIND_ACK);
                ackEnv->setSrc(myMacAddress);
                ackEnv->setDest(tschPkt->getSrc());
                ackEnv->encapsulate(ack);

                switchRadioMode(RadioMode::TX);
                send(encapsulateInRadioPkt(ackEnv), "lowerLayerOut");
            }

            NetworkLayerPkt *netPkt = check_and_cast<NetworkLayerPkt*>(
                    tschPkt->decapsulate());
            //cPacket *appPkt = netPkt->decapsulate();
            netPkt->setControlInfo(tschPkt->removeControlInfo());
            send(netPkt, "upperLayerOut");

        }
        delete tschPkt;
    } else if (msg->getArrivalGate() == gate("upperLayerIn")) { // from upper layer
        NetworkLayerPkt *netPkt = check_and_cast<NetworkLayerPkt*>(msg);
        IETF6TiSCHPkt *tschPkt = encapsulateIn6TiSCHPacket(netPkt);
        schedulePacket(tschPkt);
    }
}

void IETF6TiSCH::onScheduleBeep() {
    // check unacked packets at the end of the previous timeslot
    if (areRetransmissionsEnabled) {
        checkUnackedPackets();
    }

    // increase ASN
    asn++;
    if (myMacAddress == 0)
        EV << "ASN = " << asn << " TIMESLOT: " << asn % schedule->frameLength
                  << endl;  // DEBUG
    // get queue size
    emit(queueSizeSignal, getGlobalQueueSize());

    cell_t cell = schedule->getAction(EV, this->asn, myMacAddress,
            broadcastDomain, 0, &globalQueue);

    if ((cell.tx == myMacAddress || cell.tx == broadcastDomain)
            && cell.scheduled_msg != NULL) {
        IETF6TiSCHPkt *pkt = (IETF6TiSCHPkt*) cell.scheduled_msg;
        switchRadioMode(RadioMode::TX);
        switchRadioFrequency(cell.channel);
        sendToLowerLayer(pkt);
    } else if (cell.rx == myMacAddress || cell.rx == broadcastDomain) {
        switchRadioMode(RadioMode::RX);
        switchRadioFrequency(cell.channel);
    } else {
        switchRadioMode(RadioMode::SLEEP);
    }
    scheduleAt(simTime() + TIMESLOT_DURATION_S, scheduleBeep);
}

void IETF6TiSCH::checkUnackedPackets() {
    for (auto elem = ackTable.begin(); elem != ackTable.end();) {
        if (elem->count == 0 && elem->retransmissions < maxRetransmissions) {
            if (elem->retransmissions == 0) {
                emit(atLeastOneRetransmissionSignal, 1);
            }
            // reschedule transmission of the packet
            schedulePacket(elem->originalPkt->dup());
            elem->retransmissions += 1;

            emit(retransmissionsSignal, 1);
            if (lastRetransmission != -1)
                emit(retransmissionsIntertimeSignal,
                        simTime().dbl() - lastRetransmission);
            lastRetransmission = simTime().dbl();
            elem++;

        } else {  // delete the copy
            emit(retransmissionsCountSignal, elem->retransmissions);
            delete elem->originalPkt;
            // erase the entry
            elem = ackTable.erase(elem);
        }
    }
}

void IETF6TiSCH::schedulePacket(IETF6TiSCHPkt *pkt) {
    if (queueLength == -1 || getGlobalQueueSize() < queueLength) {
        cell_t cell = schedule->findNextTransmittingTimeSlot(pkt->getSrc(),
                pkt->getDest());
        if (cell.channel != -1) {
            globalQueue.at(cell.timeslot).push_back( { cell.channel, pkt });
        }
    } else {
        emit(packetsDroppedSignal, 1);
        delete pkt;
    }
}

void IETF6TiSCH::switchRadioMode(RadioMode status) {
    radio->setRadioMode(status);
}

void IETF6TiSCH::switchRadioFrequency(int channel) {
    long long frequency = baseFrequency
            + baseBandwidth * ((asn + channel) % CHANNELS);
    radio->setCarrierFrequency(frequency);
}

void IETF6TiSCH::switchRadioBroadcastDomain(int broadcastDomain) {
    this->broadcastDomain = broadcastDomain;
    radio->setBroadcastDomain(broadcastDomain);
}

void IETF6TiSCH::sendToLowerLayer(IETF6TiSCHPkt *tschPkt) {
    // check if pkt is not already in ackTable, otherwise insert it
    if (areRetransmissionsEnabled) {
        bool found = false;
        for (int i = 0; i < ackTable.size(); i++) {
            if (ackTable[i].seqNum == tschPkt->getSeqNum()) {
                found = true;
                break;
            }
        }
        if (!found) {
            ackTable.push_back( { tschPkt->getSeqNum(), tschPkt, 0, 0 });
            sendDelayed(encapsulateInRadioPkt(tschPkt->dup()), GUARD_TIME_S,
                    "lowerLayerOut");
        } else {
            sendDelayed(encapsulateInRadioPkt(tschPkt), GUARD_TIME_S,
                    "lowerLayerOut");
        }
    } else {
        sendDelayed(encapsulateInRadioPkt(tschPkt), GUARD_TIME_S,
                "lowerLayerOut");
    }
}

// ENCAPSULATION UTILITY

IETF6TiSCHPkt* IETF6TiSCH::encapsulateIn6TiSCHPacket(NetworkLayerPkt *netPkt) {
    IETF6TiSCHPkt *tschPkt = new IETF6TiSCHPkt();
    tschPkt->setByteLength(netPkt->getByteLength());
    tschPkt->setSrc(myMacAddress);
    tschPkt->setDest(netPkt->getDest());  // implement IP resolution
    tschPkt->setSeqNum(netPkt->getSeq());
    tschPkt->encapsulate(netPkt);
    return tschPkt;
}

RadioPkt* IETF6TiSCH::encapsulateInRadioPkt(IETF6TiSCHPkt *tschPkt) {
    RadioPkt *radioPkt = new RadioPkt();
    radioPkt->setByteLength(tschPkt->getByteLength());
    radioPkt->encapsulate(tschPkt);
    radioPkt->setSrc(tschPkt->getSrc());
    radioPkt->setDest(tschPkt->getDest());
    return radioPkt;
}

void IETF6TiSCH::updateSchedule(vector<int> targets) {
    if (schedule != nullptr)
        delete schedule;
    schedule = schedulingFunction->getSchedule();
    // schedule = coordinator->getSchedule(targets);
    if (firstInit) {
        this->globalQueue.resize(schedule->frameLength);
        firstInit = false;
    }
    schedule->printSchedule(EV);
}

int IETF6TiSCH::getGlobalQueueSize() {
    int ret = 0;
    for (int i = 0; i < schedule->frameLength; i++) {
        ret += globalQueue.at(i).size();
    }
    return ret;
}
