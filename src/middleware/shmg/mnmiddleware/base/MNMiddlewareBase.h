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

#ifndef __MOBILE6TISCH_MNMIDDLEWARE_H_
#define __MOBILE6TISCH_MNMIDDLEWARE_H_

#include <omnetpp.h>

#include "linklayer/ieee802154tsch/IEEE802154TSCH.h"
#include "linklayer/ieee802154tsch/IEEE802154TSCHPkt_m.h"
#include "linklayer/ieee802154tsch/shmg/SHMGLinkLayerConstants.h"
#include "middleware/base/MiddlewareBase.h"
#include "middleware/shmg/messages/SHMGNetworkPkt_m.h"
#include "middleware/shmg/ncmiddleware/base/NCMiddlewareBase.h"

#define CLEANUP_PERIOD_S 20  // used to clean up deduplication table, used to avoid high memory usage

using namespace omnetpp;

class MNMiddlewareBase : public MiddlewareBase {
   protected:
    // STATE
    int packetsSent = 0;
    std::unordered_map<int, simtime_t> deduplicationTable;

    // MODULES
    NCMiddlewareBase *ncMid;
    IEEE802154TSCH *ieee802154tsch;

    // METHODS
    virtual void initialize(int stage) override;

    // events
    virtual void handleSelfMessage(cMessage *msg) override;
    virtual void handleUpperPacket(cPacket *pkt) override;
    virtual void handleLowerPacket(cPacket *pkt) override;

    // utilities
    virtual void sendDown(SHMGNetworkPkt *netPkt);
};

#endif
