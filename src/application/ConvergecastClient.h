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

#ifndef __MOBILE6TISCH_CONVERGECASTCLIENT_H_
#define __MOBILE6TISCH_CONVERGECASTCLIENT_H_

#include <omnetpp.h>

#include "../ietf6tisch/IETF6TiSCH.h"
#include "../shmg/nccore/NCCore.h"
#include "ApplicationBase.h"
#include "ConvergecastPkt_m.h"

using namespace omnetpp;

class ConvergecastClient : public ApplicationBase {
    double upPeriod;
    double downPeriod;
    double upDelay;
    double downDelay;
    
    IETF6TiSCH *ietf6TiSCH;
    NCCore *ncCore;

    cMessage *sendBeep;

    int seqNum;

    simsignal_t packetsSent;

   protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;

   public:
    virtual int numInitStages() const { return 5; }
};

#endif
