#ifndef __MOBILE6TISCH_IEEE802154TSCHSCHEDULE_H_
#define __MOBILE6TISCH_IEEE802154TSCHSCHEDULE_H_

#include <omnetpp.h>

#include <algorithm>
#include <deque>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "utils/cpputils.h"

using namespace omnetpp;

#define NONE_ADDRESS -1

typedef std::deque<std::pair<int, void*>> timeslot_msg_queue_t;

struct cell_t {
    int channel;
    int timeslot;
    int tx;
    int rx;
    bool isShared;
    bool isControlCell;

    // redefine operator == to compare cells
    bool operator==(const cell_t& other) const {
        return (channel == other.channel && timeslot == other.timeslot && tx == other.tx && rx == other.rx && isShared == other.isShared &&
                isControlCell == other.isControlCell);
    }

    bool operator!=(const cell_t& other) const { return !(*this == other); }

    // redefine operator <<
    friend std::ostream& operator<<(std::ostream& out, const cell_t& cell) {
        out << "cell_t(" << cell.channel << "," << cell.timeslot << "," << cell.tx << "," << cell.rx << "," << cell.isShared << ","
            << cell.isControlCell << ")";
        return out;
    }
};

class IEEE802154TSCHSchedule {
    // STATE
    std::unordered_map<int /*ts*/, std::unordered_map<int /*ch*/, std::vector<cell_t>>> schedule;

   public:
    // STATE
    int slotframeLength;
    int numChannels;

    // METHODS
    // constructor & destructor
    IEEE802154TSCHSchedule(int slotframeLength, int numChannels);

    // set cell at timeslot and channel with tx and rx addresses
    void addCell(int timeslot, int channel, mac_address_t tx, mac_address_t rx, bool shared, bool control);

    // remove cells in timeslot and channel
    void removeCells(int timeslot, int channel);

    // get cells given a timeslot and channel
    std::vector<cell_t> getCells(int timeslot, int channel) { return schedule[timeslot][channel]; }

    // get cells given a tx or rx address
    std::vector<cell_t> query(mac_address_t tx = NONE_ADDRESS, mac_address_t rx = NONE_ADDRESS);

    // copy utility
    IEEE802154TSCHSchedule* copy();

    // get used cells in current timeslot
    std::vector<cell_t> getCellsInTimeslot(int currentTs);

    // print schedule
    void printSchedule(std::ostream& out);

    // adapt schedule to a new one
    void adapt(IEEE802154TSCHSchedule* newSchedule);

    // only keep the cells regarding the given mac addresses
    void filter(std::vector<mac_address_t> macAddresses);

    // returns the list of cells that differ from the given schedule
    std::vector<cell_t> diff(IEEE802154TSCHSchedule* newSchedule);
};

#endif
