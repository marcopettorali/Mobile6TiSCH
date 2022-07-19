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

#include "tsch.h"

#include <cmath>

Define_Module(TSCH);

void TSCH::initialize(int stage) {
    if (stage == 1) {
        // retrieve pointers to other modules
        coordinator = check_and_cast<NetworkCoordinator *>(getModuleByPath(par("coordinator")));
        radio = check_and_cast<Radio *>(getModuleByPath(par("radio")));
        mobility = check_and_cast<Mobility *>(getModuleByPath(par("mobility")));

        // init parameters
        baseBandwidth = par("baseBandwidth");
        baseFrequency = par("baseFrequency");
        queueLength = par("queueLength");
        maxRetransmissions = par("maxRetransmissions");
        macAddress = getParentModule()->getSubmodule("radio")->par("macAddress");
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
        retransmissionsIntertimeSignal = registerSignal("retransmissionsIntertime");
        retransmissionsCountSignal = registerSignal("retransmissionsCount");
        atLeastOneRetransmissionSignal = registerSignal("atLeastOneRetransmission");
    }

    EV << "TSCH::initialize - stage " << stage << endl;
}

void TSCH::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg->isName("scheduleBeep")) {
        onScheduleBeep();
    } else if (msg->getArrivalGate() == gate("lowerLayerIn")) {  // from network
        EV << "TSCH: received packet from lower layer" << endl;
        TSCHPkt *tschPkt = check_and_cast<TSCHPkt *>(msg);

        if (areRetransmissionsEnabled && tschPkt->getKind() == TSCHPKT_ACK) {
            TSCHAck *ack = check_and_cast<TSCHAck *>(tschPkt->decapsulate());
            // print ackTable
            EV << "TSCH: received ACK" << endl;
            EV << "Sequence number: " << ack->getSeqNum() << endl;
            for (int i = 0; i < ackTable.size(); i++) {
                ack_table_elem *elem = &ackTable[i];
                if (elem->seqNum == ack->getSeqNum()) {
                    elem->count += 1;
                    EV << "macAddress: " << macAddress << ". Received ACK for " << ack->getSeqNum() << " from "
                       << tschPkt->getSrc() << endl;
                }
            }
            EV << "TSCH: ACK table:" << endl;
            for (auto it = ackTable.begin(); it != ackTable.end(); ++it) {
                EV << "TSCH: " << it->seqNum << ": " << it->count << endl;
            }

            delete ack;

        } else { 
            if (areRetransmissionsEnabled) {
                // send ack
                TSCHAck *ack = new TSCHAck();
                ack->setSeqNum(tschPkt->getSeqNum());

                TSCHPkt *ackEnv = new TSCHPkt();
                ackEnv->setByteLength(22);
                ackEnv->setKind(TSCHPKT_ACK);
                ackEnv->setSrc(macAddress);
                ackEnv->setDest(tschPkt->getSrc());
                ackEnv->encapsulate(ack);

                switchRadioMode(RadioMode::TX);
                send(ackEnv, "lowerLayerOut");

                EV << "macAddress: " << macAddress << ". Sent ACK for " << ack->getSeqNum() << " to "
                   << tschPkt->getSrc() << endl;
            }

            if (tschPkt->getKind() == TSCHPKT_UP) {  // send to border router
                UpstreamPkt *up = check_and_cast<UpstreamPkt *>(tschPkt->decapsulate());
                up->setMobileNode(tschPkt->getSrc());
                up->setBR(macAddress);
                up->setControlInfo(tschPkt->removeControlInfo());
                send(up, "upperLayerOut");
            } else if (tschPkt->getKind() == TSCHPKT_DOWN) {  // send to mobile node
                DownstreamPkt *down = check_and_cast<DownstreamPkt *>(tschPkt->decapsulate());
                send(down, "upperLayerOut");
            }
        }
        delete tschPkt;
    } else if (msg->getArrivalGate() == gate("upperLayerIn")) {  // from upper layer
        TSCHPkt *m = check_and_cast<TSCHPkt *>(msg);
        schedulePacket(m);
    }
}

void TSCH::onScheduleBeep() {
    // check unacked packets at the end of the previous timeslot
    if (areRetransmissionsEnabled) {
        checkUnackedPackets();
    }

    // increase ASN
    asn++;
    if (macAddress == 0) EV << "ASN = " << asn << " TIMESLOT: " << asn % schedule->get_frame_length() << endl;  // DEBUG
    // get queue size
    emit(queueSizeSignal, getGlobalQueueSize());

    EV << macAddress << ": Queue size: " << getGlobalQueueSize() << endl;

    cell_elem_t cell = schedule->get_action(this->asn, macAddress, broadcastDomain, 0, &globalQueue);

    if ((cell.tx == macAddress || cell.tx == broadcastDomain) && cell.scheduled_msg != NULL) {
        TSCHPkt *pkt = (TSCHPkt *)cell.scheduled_msg;
        switchRadioMode(RadioMode::TX);
        switchRadioFrequency(cell.channel);
        sendToLowerLayer(pkt);
    } else if (cell.rx == macAddress || cell.rx == broadcastDomain) {
        switchRadioMode(RadioMode::RX);
        switchRadioFrequency(cell.channel);
    } else {
        switchRadioMode(RadioMode::SLEEP);
    }
    scheduleAt(simTime() + TIMESLOT_DURATION_S, scheduleBeep);
}

void TSCH::checkUnackedPackets() {
    for (auto elem = ackTable.begin(); elem != ackTable.end();) {
        if (elem->count == 0 && elem->retransmissions < maxRetransmissions) {
            if(elem->retransmissions == 0) {
                emit(atLeastOneRetransmissionSignal, 1);
            }
            // reschedule transmission of the packet
            schedulePacket(elem->originalPkt->dup());
            elem->retransmissions += 1;
            EV << "macAddress: " << macAddress << ". Rescheduled packet " << elem->originalPkt->getSeqNum() << endl;

            emit(retransmissionsSignal, 1);
            if (lastRetransmission != -1) emit(retransmissionsIntertimeSignal, simTime().dbl() - lastRetransmission);
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

void TSCH::schedulePacket(TSCHPkt *pkt) {
    if (queueLength == -1 || getGlobalQueueSize() < queueLength) {
        cell_elem_t cell = schedule->find_next_transmitting_slot(pkt->getSrc(), pkt->getDest());
        if (cell.channel != -1) {
            globalQueue.at(cell.timeslot).push_back({cell.channel, pkt});
        }
    } else {
        emit(packetsDroppedSignal, 1);
        delete pkt;
    }
}

void TSCH::switchRadioMode(RadioMode status) { radio->setRadioMode(status); }

void TSCH::switchRadioFrequency(int channel) {
    long long frequency = baseFrequency + baseBandwidth * channel;
    radio->setCarrierFrequency(frequency);
}

void TSCH::switchRadioBroadcastDomain(int broadcastDomain) { this->broadcastDomain = broadcastDomain; }

void TSCH::sendToLowerLayer(TSCHPkt *pkt) {
    // check if pkt is not already in ackTable, otherwise insert it

    if (areRetransmissionsEnabled) {
        bool found = false;
        for (int i = 0; i < ackTable.size(); i++) {
            if (ackTable[i].seqNum == pkt->getSeqNum()) {
                found = true;
                break;
            }
        }
        if (!found) {
            ackTable.push_back({pkt->getSeqNum(), pkt, 0, 0});
            sendDelayed(pkt->dup(), GUARD_TIME_S, "lowerLayerOut");
        } else {
            sendDelayed(pkt, GUARD_TIME_S, "lowerLayerOut");
        }
    } else {
        sendDelayed(pkt, GUARD_TIME_S, "lowerLayerOut");
    }
}

void TSCH::updateSchedule(vector<int> targets) {
    if (schedule != nullptr) delete schedule;
    schedule = coordinator->getSchedule(targets);
    if (firstInit) {
        this->globalQueue.resize(schedule->get_frame_length());
        firstInit = false;
    }
}

int TSCH::getGlobalQueueSize() {
    int ret = 0;
    for (int i = 0; i < schedule->get_frame_length(); i++) {
        ret += globalQueue.at(i).size();
    }
    return ret;
}
