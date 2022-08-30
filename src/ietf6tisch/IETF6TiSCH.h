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

#ifndef __MOBILE6TISCH_IETF6TISCH_H_
#define __MOBILE6TISCH_IETF6TISCH_H_

#include <omnetpp.h>

#include <vector>

#include "../physicallayer/mobility/MobilityBase.h"
#include "../physicallayer/radio/Radio.h"
#include "NetworkLayerPkt_m.h"
#include "IETF6TiSCHAck_m.h"
#include "IETF6TiSCHPkt_m.h"
#include "TSCHSchedule.h"
#include "schedulingfunction/SchedulingFunctionBase.h"

#define KIND_ACK 1

using namespace omnetpp;

struct ack_table_elem {
    int seqNum;

    IETF6TiSCHPkt *originalPkt;
    int count;
    int retransmissions;
};

class IETF6TiSCH : public cSimpleModule {
    Radio *radio;
    SchedulingFunctionBase *schedulingFunction;

    double baseFrequency;
    double baseBandwidth;

    std::vector<timeslot_msg_queue_t> globalQueue;
    int queueLength;

    int asn;

    cMessage *scheduleBeep;

    bool firstInit = true;

    simsignal_t queueSizeSignal;
    simsignal_t packetsDroppedSignal;
    simsignal_t retransmissionsSignal;
    simsignal_t retransmissionsIntertimeSignal;
    simsignal_t retransmissionsCountSignal;
    simsignal_t atLeastOneRetransmissionSignal;

    double lastRetransmission = -1;
    bool areRetransmissionsEnabled;
    int maxRetransmissions;
    vector<ack_table_elem> ackTable;

    void onScheduleBeep();
    void schedulePacket(IETF6TiSCHPkt *pkt);
    void checkUnackedPackets();

    /*UTILITY*/
    void startTSCHSchedule();
    void switchRadioMode(RadioMode status);
    void switchRadioFrequency(int channel);

    IETF6TiSCHPkt* encapsulateIn6TiSCHPacket(NetworkLayerPkt *netPkt);
    RadioPkt *encapsulateInRadioPkt(IETF6TiSCHPkt *tschPkt);
    void sendToLowerLayer(IETF6TiSCHPkt *tschPkt);

    int getGlobalQueueSize();

    virtual int numInitStages() const { return 5; }

   protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

   public:
    int myMacAddress;
    int broadcastDomain;
    MobilityBase *mobility;
    TSCHSchedule *schedule = nullptr;
    
    void switchRadioBroadcastDomain(int broadcastDomain);
    void updateSchedule(vector<int> targets);
};

#endif
