#ifndef __MOBILE6TISCH_ICOLLECTABLE_H_
#define __MOBILE6TISCH_ICOLLECTABLE_H_

#include <omnetpp.h>

#include "StatsCollector.h"

using namespace omnetpp;

class ICollectable {
    friend class StatsCollector;

   protected:
    virtual void finishCollectStats() = 0;
};

#endif
