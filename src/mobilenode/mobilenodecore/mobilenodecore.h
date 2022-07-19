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

#ifndef __MOBILE6TISCH_LITE_MOBILENODECORE_H_
#define __MOBILE6TISCH_LITE_MOBILENODECORE_H_

#include <omnetpp.h>

#include "../../applicationpkts/downstreampkt_m.h"
#include "../../applicationpkts/upstreampkt_m.h"
#include "../../networkcoordinator/networkcoordinator.h"
#include "../../physicallayer/mobility/mobility.h"
#include "../../tsch/tsch.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class MobileNodeCore : public cSimpleModule {
    NetworkCoordinator *coordinator;
    TSCH *tsch;
    Mobility *mobility;

    int macAddress;

    cMessage *beep;
    int received;
    int seqNum;

    string application;
    string scheduling;

    double period;

    double upRate;
    double downRate;

    simsignal_t upstreamPacketSent;
    simsignal_t downstreamPacketReceived;
    simsignal_t downstreamPacketDelay;
    simsignal_t rtt;

    virtual int numInitStages() const { return 5; }

   protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
};

#endif
