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

#ifndef __MOBILE6TISCH_NCMIDDLEWAREBASE_H_
#define __MOBILE6TISCH_NCMIDDLEWAREBASE_H_

#include <omnetpp.h>

#include <unordered_map>
#include <vector>

#include "linklayer/ieee802154tsch/IEEE802154TSCHConstants.h"
#include "linklayer/ieee802154tsch/IEEE802154TSCHSchedule.h"
#include "linklayer/ieee802154tsch/schedulingfunction/base/SchedulingFunctionBase.h"
#include "middleware/base/MiddlewareBase.h"
#include "middleware/shmg/brmiddleware/base/BRMiddlewareBase.h"
#include "middleware/shmg/messages/BackboneCommand_m.h"
#include "middleware/shmg/messages/BackboneDataInfo_m.h"
#include "middleware/shmg/ncmiddleware/base/DedupTable.h"
#include "middleware/shmg/ncmiddleware/base/RSSITable.h"
#include "physicallayer/wired/base/WiredNICPkt_m.h"
#include "utils/shmgutils.h"

class BRMiddlewareBase;

// CONSTANTS
#define STATS_PERIOD_S 10
using namespace omnetpp;

// TYPES
// description of a MN
struct mn_desc_t {
    mn_device_id_t mnId;
    double upRate;
    Point position;
    Point direction;
    double maxSpeed;
    br_mac_address_t delegateBR = -1;
};

// description of a BR
struct br_desc_t {
    br_device_id_t brId;
    BRMiddlewareBase *module;
    Point position;
    double r;
};

// packet loss reasons (for statistics collection)
// T = target, L = listening, C = conflict
enum NetLayerPacketLossReason { PL_SUCCESS, PL_SUCCESS_SUPPORT, PL_T_L_C, PL_T_NL_C_R, PL_T_NL_C_NR, PL_T_L_NC, PL_T_NL_NC_R, PL_T_NL_NC_NR, PL_NT };

class NCMiddlewareBase : virtual public MiddlewareBase, public ICollectable {
   protected:
    // STATE
    // descriptors of all the MNs of the network
    std::unordered_map<mn_mac_address_t, mn_desc_t> mns;

    // descriptors of all the BRs of the network
    std::unordered_map<br_mac_address_t, br_desc_t> brs;

    // schedules of all the nodes of the network
    std::unordered_map<mac_address_t, IEEE802154TSCHSchedule *> schedules;  // ASSUMPTION: scheduling is centralized

    // size of the centralized schedule
    int slotframeLength;  // ASSUMPTION: all the schedules have the same slotframe length

    // scheduling function to build the schedules
    SchedulingFunctionBase *schedulingFunction;

    // RSSI table to compute the closest BR
    RSSITable rssiTable;

    // deduplication table to avoid duplicate packets
    DedupTable dedupTable;

    // BEEPS
    // statistics collection
    cMessage *statsCollectionBeep;

    // start slotframe
    cMessage *slotframeStartBeep;

    // METHODS
    virtual void initialize(int stage) override;

    // packet handling
    virtual void handleSelfMessage(cMessage *msg) override;
    virtual void handleUpperPacket(cPacket *pkt) override;
    virtual void handleLowerPacket(cPacket *pkt) override;

    virtual void handleSHMGNetworkPkt(SHMGNetworkPkt *shmgPkt){};

    // events
    virtual void onStatsCollection(cMessage *msg);
    virtual void onSlotframeStart(cMessage *msg);

    // utility

    // this method copies the internal schedule of the scheduling function into the schedules map, in order to be accessed by the
    // nodes' (MNs and BRs) scheduling functions through the getSchedule() method.
    void installSchedule();
    br_mac_address_t findClosestBR(Point p);
    void sendPacketToBR(BackboneCommand *backPkt, br_mac_address_t brMacAddress);

    // statistics
    std::unordered_map<mac_address_t, std::pair<int /*#packets received*/, int /*#packets sent*/>>
        packetsStats;  // ASSUMPTION: periodic traffic, same rate for each MN
    std::unordered_map<mac_address_t, std::vector<double>> instantPDR;

    // implement ICollectable
    virtual void finishCollectStats() override{};

   public:
    // METHODS
    // registration
    void registerMN(mn_mac_address_t macAddress, mn_device_id_t mnId, double upRate, Point position, Point direction, double maxSpeed);
    void registerBR(br_mac_address_t macAddress, br_device_id_t brId, BRMiddlewareBase *br, Point position, double r);

    // getters
    int getNumMNs() { return mns.size(); };
    int getNumBRs() { return brs.size(); };
    std::vector<mn_mac_address_t> getMNsMacAddresses() {
        std::vector<mn_mac_address_t> keys;
        for (auto const &element : mns) {
            keys.push_back(element.first);
        }
        return keys;
    };
    std::vector<br_mac_address_t> getBRsMacAddresses() {
        std::vector<br_mac_address_t> keys;
        for (auto const &element : brs) {
            keys.push_back(element.first);
        }
        return keys;
    };

    Point getMNPosition(mn_mac_address_t macAddress) { return mns[macAddress].position; };
    Point getBRPosition(br_mac_address_t macAddress) { return brs[macAddress].position; };
    Point getDirection(mn_mac_address_t macAddress) { return mns[macAddress].direction; };
    double getBRRange(br_mac_address_t macAddress) { return brs[macAddress].r; };

    // schedule management

    // this method is accessed by the scheduling function of each node (MNs and BRs) to get the schedule
    IEEE802154TSCHSchedule *getSchedule(mac_address_t macAddress, mac_address_t broadcastAddress);
};

#endif
