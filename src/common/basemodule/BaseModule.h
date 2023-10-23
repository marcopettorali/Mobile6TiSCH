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

#ifndef __MOBILE6TISCH_BASEMODULE_H_
#define __MOBILE6TISCH_BASEMODULE_H_

#include <omnetpp.h>

#include "common/crosslayerdb/CrossLayerDB.h"
#include "common/statscollector/StatsCollector.h"
#include "utils/cpputils.h"
#include "utils/initstage.h"
#include "utils/log.h"
#include "utils/mathutils.h"
#include "utils/rng_headers.h"

using namespace omnetpp;

class BaseModule : public cSimpleModule {
   protected:
    StatsCollector *statsCollector;
    CrossLayerDB *crossLayerDB;

    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;

   public:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
};

#endif
