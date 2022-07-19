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

#ifndef __MOBILE6TISCH_LITE_TSCH_H_
#define __MOBILE6TISCH_LITE_TSCH_H_

#define MN_BROADCAST_DOMAIN -3
#define BR_BROADCAST_DOMAIN -4

#define TSCHPKT_UP 1
#define TSCHPKT_DOWN 2
#define TSCHPKT_ACK 3

#include <omnetpp.h>

#include <vector>

#include "../networkcoordinator/networkcoordinator.h"
#include "../physicallayer/mobility/mobility.h"
#include "../physicallayer/radio/radio.h"
#include "tschpkt/tschack_m.h"
#include "tschpkt/tschpkt_m.h"
#include "tschschedule/tschschedule.h"

class NetworkCoordinator;

using namespace omnetpp;

struct ack_table_elem {
    int seqNum;

    TSCHPkt *originalPkt;
    int count;
    int retransmissions;
};

/**
 * TODO - Generated class
 */
class TSCH : public cSimpleModule {
    NetworkCoordinator *coordinator;
    Radio *radio;
    Mobility *mobility;

    vector<timeslot_msg_queue_t> globalQueue;

    int macAddress;
    int broadcastDomain;

    int queueLength;

    int asn;

    cMessage *scheduleBeep;

    bool firstInit = true;

    double baseFrequency;
    double baseBandwidth;

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
    void schedulePacket(TSCHPkt *pkt);
    void checkUnackedPackets();

    /*UTILITY*/
    void startTSCHSchedule();
    void switchRadioMode(RadioMode status);
    void switchRadioFrequency(int channel);
    void sendToLowerLayer(TSCHPkt *m);

    int getGlobalQueueSize();

    virtual int numInitStages() const { return 5; }

   protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

   public:
    TSCHSchedule *schedule = nullptr;
    void switchRadioBroadcastDomain(int broadcastDomain);
    void updateSchedule(vector<int> targets);
};

#endif
