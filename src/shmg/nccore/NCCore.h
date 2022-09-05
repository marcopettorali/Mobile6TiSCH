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

#ifndef __MOBILE6TISCH_NCCORE_H_
#define __MOBILE6TISCH_NCCORE_H_

#include <omnetpp.h>

#include <vector>

#include "../../ietf6tisch/NetworkLayerPkt_m.h"
#include "../../ietf6tisch/schedulingfunction/SchedulingFunctionBase.h"
#include "../backboneprotocol/BackbonePkt_m.h"
#include "../brcore/BRCore.h"
#include "DedupTable.h"
#include "DistanceTable.h"

using namespace omnetpp;

#define BR_ENTRY_LIFETIME_S 100
#define CLEAN_PERIOD_S 100
#define MN_BROADCAST_DOMAIN -3
#define BR_BROADCAST_DOMAIN -4

class BRCore;

struct br_in_mn_t {
    int macAddress;
    double cleanTimestamp;
};

struct mn_vector_entry_t {
    int macAddress;
    double upRate;
    double downRate;
    double upDelay;
    double downDelay;
    double x;
    double y;
    int delegateBR;
    double maxSpeed;
    std::vector<br_in_mn_t> brs;
};

struct br_vector_entry_t {
    int macAddress;
    int brId;
    BRCore *module;
    double x;
    double y;
    double r;
    std::vector<int> mns;
};

class NCCore : public cSimpleModule {
    friend class SchedulingFunctionBase;

    int frameLength;

    DedupTable dedupTable;
    DistanceTable distanceTable;

    std::vector<br_vector_entry_t> brs;
    std::vector<mn_vector_entry_t> mns;

    cMessage *clean_beep;

    SchedulingFunctionBase *schedulingFunction;
    TSCHSchedule *sf;

    // mns and brs utilities
    mn_vector_entry_t *findMNByAddress(int mnAddress);
    br_vector_entry_t *findBRByAddress(int brAddress);

    simsignal_t downstreamPacketSent;
    simsignal_t upstreamPacketReceived;
    simsignal_t upstreamPacketDelay;

    void handleLowerLayerPacket(BackbonePkt *backPkt);
    void handleUpperLayerPacket(BackbonePkt *backPkt);
    void updateDelegateBR(int mnAddress);

    int findClosestBR(double x, double y);
    void upsertMNForBR(int brAddress, int mnAddressToInsert, bool scheduleToUpdate = true);
    void upsertBRForMN(int mnAddress, int brAddressToInsert);
    void removeMNFromBRAndUpdate(int brAddress, int mnAddress);

   protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const { return 5; }

   public:
    int getNumMNs() { return mns.size(); }
    TSCHSchedule* getSchedule() { return sf; }
    std::vector<mn_vector_entry_t> getMNs() { return mns; }

    // registration
    void registerMN(int macAddress, double upRate, double downRate, double upDelay, double downDelay, double x, double y, double maxSpeed);
    void registerBR(int macAddress, int brId, BRCore *br, double x, double y, double r);

    // initialization of nodes and brs
    TSCHSchedule *getSchedule(std::vector<int> targets);
    std::vector<int> getInitialSchedule(int brAddress);

    std::vector<int> getBRNeighborsList(int brAddress, double radius);
    
};

#endif
