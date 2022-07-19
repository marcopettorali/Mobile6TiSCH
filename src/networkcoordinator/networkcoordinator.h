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

#ifndef __MOBILE6TISCH_LITE_NETWORKMANAGER_H_
#define __MOBILE6TISCH_LITE_NETWORKMANAGER_H_

#include <omnetpp.h>

#include <map>
#include <vector>
#include <string.h>

#include "../applicationpkts/applicationconstants.h"
#include "../applicationpkts/downstreampkt_m.h"
#include "../applicationpkts/upstreampkt_m.h"
#include "../borderrouter/borderroutercore/borderroutercore.h"
#include "../physicallayer/radio/packetcontrolinfo/packetcontrolinfo_m.h"
#include "../tsch/tschschedule/tschschedule.h"
#include "deduptable/deduptable.h"
#include "distancetable/distancetable.h"

using namespace omnetpp;
using namespace std;

#define BR_ENTRY_LIFETIME_S 100
#define CLEAN_PERIOD_S 100
#define MN_BROADCAST_DOMAIN -3
#define BR_BROADCAST_DOMAIN -4

class BorderRouterCore;

struct br_in_mn_t
{
    int macAddress;
    double cleanTimestamp;
};

struct mn_vector_entry_t
{
    int macAddress;
    double upRate;
    double downRate;
    double x;
    double y;
    int delegateBR;
    double maxSpeed;
    vector<br_in_mn_t> brs;
};

struct br_vector_entry_t
{
    int macAddress;
    BorderRouterCore *module;
    double x;
    double y;
    double r;
    vector<int> mns;
};

/**
 * TODO - Generated class
 */
class NetworkCoordinator : public cSimpleModule
{
    int numMN;
    int frameLength;

    string application;
    string scheduling;
    int channelsUsed;

    // downstream app
    int downstreamSeqNum = 0;
    double downstreamPeriod;
    cMessage *downstreamBeep;
    int nextMN;

    DedupTable dedupTable;
    DistanceTable distanceTable;

    vector<br_vector_entry_t> brs;
    vector<mn_vector_entry_t> mns;

    cMessage *clean_beep;

    simsignal_t downstreamPacketSent;
    simsignal_t upstreamPacketReceived;
    simsignal_t upstreamPacketDelay;

    TSCHSchedule *sf;

    // mns and brs utilities
    mn_vector_entry_t *findMNByAddress(int mnAddress);
    br_vector_entry_t *findBRByAddress(int brAddress);
    void updateDelegateBR(int mnAddress);

    void upsertMNForBR(int brAddress, int mnAddressToInsert, bool scheduleToUpdate = true);
    void upsertBRForMN(int mnAddress, int brAddressToInsert);
    void removeMNFromBRAndUpdate(int brAddress, int mnAddress);

    // handle packets
    void sendDownstreamPkt(int mn,simtime_t sendTime);

    // scheduling functions
    void sdduScheduling();
    void ddduScheduling();
    void musfScheduling();

    // musf utility functions
    int findClosestBR(double x, double y);
    vector<int> getNeighborsList(int brAddress, double radius);
    void musfSchedulingOnUpstreamPkt(UpstreamPkt *up, double rssiMW);
    void cleanMNFromFarBRs(int mnAddress, int pivotBRAddress, double radius);

protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const { return 5; }

public:
    // registration
    void registerMN(int macAddress, double upRate, double downRate, double x, double y, double maxSpeed);
    void registerBR(int macAddress, BorderRouterCore *br, double x, double y, double r);

    // initialization of nodes and brs
    TSCHSchedule *getSchedule(vector<int> targets);
    vector<int> getInitialSchedule(int brAddress);

    int getNumMNs() { return mns.size(); }
    int getNumChannelsUsed() { return channelsUsed; }

    // delivery of upstream packets
    void deliverUpstreamPkt(cMessage *msg);
};

#endif
