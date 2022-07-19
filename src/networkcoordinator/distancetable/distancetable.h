#ifndef NETWORKCOORDINATOR_DISTANCETABLE_DISTANCETABLE_H_
#define NETWORKCOORDINATOR_DISTANCETABLE_DISTANCETABLE_H_

#include <deque>
#include <map>

#define TABLE_ENTRY_SIZE 256

struct packet_entry_t {
    int id;
    int br;
    double timestamp;
    double rssi;
};

using namespace std;

typedef deque<packet_entry_t> distance_table_entry_t;

class DistanceTable {
    map<int, distance_table_entry_t> distance_table;

   public:
    DistanceTable();
    virtual ~DistanceTable();

    void insert(int id, int mn, int br, double timestamp, double rssi);
    void clean(int current_timestamp);

    // queries
    packet_entry_t getLatestHighestEntry(int mn);
    distance_table_entry_t getEntries(int mn);
};
#endif /* NETWORKCOORDINATOR_DISTANCETABLE_DISTANCETABLE_H_ */
