#ifndef NCMIDDLEWARE_RSSITABLE_H_
#define NCMIDDLEWARE_RSSITABLE_H_

#include <deque>
#include <unordered_map>

#define TABLE_ENTRY_SIZE 256

struct packet_entry_t {
    int id;
    int br;
    double timestamp;
    double rssi;
};

typedef std::deque<packet_entry_t> rssi_table_entry_t;

class RSSITable {
    std::unordered_map<int, rssi_table_entry_t> distance_table;

   public:
    void insert(int id, int mn, int br, double timestamp, double rssi);
    void clean(int current_timestamp);

    // queries
    packet_entry_t getLatestHighestEntry(int mn);
    rssi_table_entry_t getEntries(int mn);
};
#endif