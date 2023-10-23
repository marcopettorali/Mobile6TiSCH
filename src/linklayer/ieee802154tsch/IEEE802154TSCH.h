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

#include <algorithm>
#include <random>
#include <vector>

#include "IEEE802154TSCHSchedule.h"
#include "linklayer/base/LinkLayerBase.h"
#include "linklayer/ieee802154tsch/IEEE802154TSCHAck_m.h"
#include "linklayer/ieee802154tsch/IEEE802154TSCHConstants.h"
#include "linklayer/ieee802154tsch/IEEE802154TSCHPkt_m.h"
#include "physicallayer/radio/base/RadioBase.h"
#include "schedulingfunction/base/SchedulingFunctionBase.h"

#define KIND_ACK 1

using namespace omnetpp;

struct packet_elem_t {
    mac_address_t rxMacAddress;
    bool useControlCell;
    IEEE802154TSCHPkt *packet;
};

struct ack_table_elem {
    int seqNum;
    IEEE802154TSCHPkt *originalPkt;
    int count;
    int retransmissions;
};

struct ts_stat {
    int ts;
    int type;
    int numPackets = 0;
    int count = 0;
};

struct csma_ca_elem_t {
    IEEE802154TSCHPkt *originalPkt;
    int nb;          // number of retransmissions
    int be;          // backoff exponent
    int w;           // backoff window
    int remainingW;  // when = 0, the node can transmit
};

class IEEE802154TSCH : virtual public LinkLayerBase {
    // STATE
    // packets queue
    std::vector<packet_elem_t> packetsQueue;
    int queueLength;

    // CSMA-CA
    std::unordered_map<int /*seqNum*/, csma_ca_elem_t> csmaCaState;
    int macMinBE;
    int macMaxBE;
    int macMaxFrameRetries;

    // retransmissions
    bool enableRetransmissionsInSharedCells;
    bool enableRetransmissionsInDedicatedCells;

    // helpers for retransmissions
    bool flagAcksShCells;
    bool flagAcksDedCells;
    bool dedicatedCell;

    // modules
    RadioBase *radio;
    SchedulingFunctionBase *schedulingFunction;

    // BEEP
    cMessage *scheduleBeep;

    // METHODS
    void onScheduleBeep();
    void checkUnackedPackets();

   protected:
    // STATE
    IEEE802154TSCHSchedule *schedule;
    int asn = 0;
    int currentTs = 0;

    mac_address_t listenedMacAddr;

    // METHODS
    virtual void initialize(int stage) override;
    ~IEEE802154TSCH();

    // packet handling
    void handleSelfMessage(cMessage *msg) override;
    void handleUpperPacket(cPacket *pkt) override;
    void handleLowerPacket(cPacket *pkt) override;

    // ACKs handling
    void handleAck(IEEE802154TSCHAck *ack);
    void sendAck(IEEE802154TSCHPkt *pkt);

    void enqueuePacket(IEEE802154TSCHPkt *pkt, bool useControlCell);
    bool hasEnquedPacketForMN(int rxMacAddress, bool useControlCell);
    IEEE802154TSCHPkt *popFirstEnqueuedPacket(int macAddress, bool useControlCell);

    RadioPkt *encapsulateInRadioPkt(IEEE802154TSCHPkt *tschPkt);
    void sendToLowerLayer(IEEE802154TSCHPkt *tschPkt);

    // event handling
    virtual void onNoCellsInTimeslot();
    virtual void onMultipleCellsInTimeslot(std::vector<cell_t> cells);
    // return true if a cell has been served both in tx and rx
    virtual bool onSingleCellInTimeslot(cell_t cell);
    virtual void onSlotframeStart();

    // utility methods
    void updateRemainingWForPacketsInCell(cell_t cell);
    void switchRadioMode(RadioMode status);
    void switchRadioFrequency(long long channel);

   public:
    // STATE
    int macAddress;
    int broadcastAddress;

    // METHODS

    bool areRetransmissionsEnabled() { return dedicatedCell ? flagAcksDedCells : flagAcksShCells; }

    void setRetransmissionsEnabledInSharedCells(bool val) {
        if (enableRetransmissionsInSharedCells) flagAcksShCells = val;
    }
    
    void setRetransmissionsEnabledInDedicatedCells(bool val) {
        if (enableRetransmissionsInDedicatedCells) flagAcksDedCells = val;
    }

    IEEE802154TSCHSchedule *getSchedule() { return schedule; }

    void updateSchedule(IEEE802154TSCHSchedule *newSchedule) {
        schedule->adapt(newSchedule);
        delete newSchedule;
    }
};

#endif
