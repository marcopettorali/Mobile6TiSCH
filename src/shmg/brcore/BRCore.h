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

#ifndef __MOBILE6TISCH_BRCORE_H_
#define __MOBILE6TISCH_BRCORE_H_

#include <omnetpp.h>

#include "../../ietf6tisch/IETF6TiSCH.h"
#include "../../shmg/nccore/NCCore.h"
#include "../../shmg/backboneprotocol/BackbonePkt_m.h"

using namespace omnetpp;

class NCCore;

/**
 * TODO - Generated class
 */
class BRCore : public cSimpleModule {
    IETF6TiSCH* ietf6TiSCH;
    NCCore* ncCore;
    int brId;

    void handleBackboneCommand(cMessage* msg);
    void handleNetworkPacket(cMessage* msg);
    virtual int numInitStages() const { return 5; }

   protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage* msg) override;
};

#endif
