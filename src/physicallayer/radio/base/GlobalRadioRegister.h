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

#ifndef __MOBILE6TISCH_GLOBALRADIOREGISTER_H_
#define __MOBILE6TISCH_GLOBALRADIOREGISTER_H_

#include <omnetpp.h>

#include <unordered_map>
#include <vector>

#include "common/basemodule/BaseModule.h"
#include "common/statscollector/ICollectable.h"
#include "common/statscollector/StatsCollector.h"
#include "physicallayer/radio/base/RadioBase.h"

using namespace omnetpp;

#define NUM_OUTCOMES 8
enum TransmissionOutcome { SUCCESS, FAIL_CHANNEL_MODEL, FAIL_SINR_THRESHOLD, FAIL_COLLISION, FAIL_RSSI_THRESHOLD, FAIL_RECEPTION_CONFLICT, FAIL_LOS_OBSTRUCTION, NONE };

class RadioBase;

class GlobalRadioRegister : virtual public BaseModule, public ICollectable {
    std::vector<RadioBase *> radios;
    std::unordered_map<int, TransmissionOutcome> transmissionStats;

   protected:
    virtual void initialize(int stage) override;
    virtual void finishCollectStats() override;

   public:
    void registerRadio(RadioBase *radio) { radios.push_back(radio); };
    std::vector<RadioBase *> getRadios() { return radios; }

    void registerPacketSent(int id) { transmissionStats[id] = NONE; }

    void registerPacketReceived(int id, TransmissionOutcome outcome) {
        if (outcome < transmissionStats[id]) {
            transmissionStats[id] = outcome;
        }
    }
};

#endif
