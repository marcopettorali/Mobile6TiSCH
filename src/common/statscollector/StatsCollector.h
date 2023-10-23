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

#ifndef __MOBILE6TISCH_STATSCOLLECTOR_H_
#define __MOBILE6TISCH_STATSCOLLECTOR_H_

#include <omnetpp.h>

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include "ICollectable.h"
#include "utils/initstage.h"
#include "utils/log.h"

#define STAT_TYPE_MEAN "mean"
#define STAT_TYPE_TIMEAVG "timeavg"
#define STAT_TYPE_SUM "sum"
#define STAT_TYPE_VECTOR "vector"

using namespace omnetpp;

class ICollectable;

class StatsCollector : virtual public cSimpleModule {
    bool emitStats;
    bool dumpStats;
    bool verbose;

    bool enableVectorStats;

    std::vector<ICollectable*> collectables;

    std::unordered_map<std::string, std::string> statsTypes;
    std::unordered_map<std::string, std::unordered_map<int, std::vector<double>>> recordsTable;
    std::unordered_map<std::string, std::unordered_map<std::string, double>> resultsTable;

    FILE* createDumpFile();
    void computeResults();
    void dumpResults();

   protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage* msg) override;
    virtual void finish() override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }

   public:
    void subscribeStats(std::string statsName, std::string statsType);
    void registerCollectable(ICollectable* collectable) {
        if (std::find(collectables.begin(), collectables.end(), collectable) == collectables.end()) {
            collectables.push_back(collectable);
        }

        // print each collectable
        INF("StatsCollector") << "registered collectables " << collectables.size() << endl;
    }
    void record(int id, std::string statsName, double value);
};

#endif
