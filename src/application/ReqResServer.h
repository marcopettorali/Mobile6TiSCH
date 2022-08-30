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

#ifndef __MOBILE6TISCH_REQRESSERVER_H_
#define __MOBILE6TISCH_REQRESSERVER_H_

#include <omnetpp.h>

#include "ApplicationBase.h"
#include "ReqResPkt_m.h"
#include "../shmg/backboneprotocol/BackbonePkt_m.h"

using namespace omnetpp;

class ReqResServer : public ApplicationBase {
    simsignal_t packetsSent;
    simsignal_t packetsReceived;
    simsignal_t packetsDelay;
    simsignal_t packetsUpstreamDelay;
   protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif
