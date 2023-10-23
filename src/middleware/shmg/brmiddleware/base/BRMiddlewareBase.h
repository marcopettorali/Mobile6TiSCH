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

#ifndef __MOBILE6TISCH_BRMIDDLEWAREBASE_H_
#define __MOBILE6TISCH_BRMIDDLEWAREBASE_H_

#include <omnetpp.h>

#include "linklayer/ieee802154tsch/IEEE802154TSCH.h"
#include "linklayer/ieee802154tsch/IEEE802154TSCHPkt_m.h"
#include "linklayer/ieee802154tsch/shmg/SHMGLinkLayerConstants.h"
#include "middleware/base/MiddlewareBase.h"
#include "middleware/shmg/messages/BackboneCommand_m.h"
#include "middleware/shmg/messages/BackboneDataInfo_m.h"
#include "middleware/shmg/messages/SHMGNetworkPkt_m.h"
#include "middleware/shmg/ncmiddleware/base/NCMiddlewareBase.h"
#include "physicallayer/wired/base/WiredNICPkt_m.h"

class NCMiddlewareBase;

using namespace omnetpp;

class BRMiddlewareBase : public MiddlewareBase {
   protected:
    // STATE
    // modules
    NCMiddlewareBase* ncMid;
    IEEE802154TSCH* ieee802154tsch;

    //  METHODS
    virtual void initialize(int stage) override;
    virtual void handleUpperPacket(cPacket* pkt) override;
    virtual void handleLowerPacket(cPacket* pkt) override;

    // handler methods
    virtual void handleBackboneCommand(cPacket* pkt);
    virtual void handleNetworkPacket(cPacket* pkt);

   public:
    void setIEEE802154RetransmissionsEnabled(bool retransmissionsEnabled);
    void updateSchedule(IEEE802154TSCHSchedule* newSchedule) { ieee802154tsch->updateSchedule(newSchedule); }
};

#endif
