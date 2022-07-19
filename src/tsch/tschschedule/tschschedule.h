#ifndef SIXTISCH_TSCHSCHEDULE_H_
#define SIXTISCH_TSCHSCHEDULE_H_

#include <deque>
#include <sstream>
#include <vector>
using namespace std;

#define CHANNELS 16
#define TIMESLOT_DURATION_S 0.015
#define GUARD_TIME_S 0.001

#define NONE -1
#define NULL_CELL \
    { NONE, NONE, NONE, NONE, false, 0, 0, {0}, NULL }

typedef deque<pair<int, void*>> timeslot_msg_queue_t;

struct cell_elem_t {
    // properties of the cell
    int channel;
    int timeslot;
    int tx;
    int rx;
    bool shared;

    // superscheduling info
    int sup_start;
    int sup_dur;
    vector<int> sup_list;

    // returned message
    void* scheduled_msg;
};

typedef vector<cell_elem_t> cell_t;

class TSCHSchedule {
    cell_t* sf;

    int frame_length;

    cell_elem_t action_policy(int timeslot, vector<cell_elem_t> cell_elem_list, int subject_address, int subject_domain,
                              vector<timeslot_msg_queue_t>* queue, int flavor);

   public:
    TSCHSchedule(int frame_length);
    ~TSCHSchedule();

    int get_frame_length() { return frame_length; }

    int get_channel_count() { return CHANNELS; }

    int get_queue_size(vector<timeslot_msg_queue_t>* queue, int time_slot);

    int get_global_queue_size(vector<timeslot_msg_queue_t>* queue);

    cell_t* sf_at(int timeslot, int channel) { return &sf[channel * frame_length + timeslot]; }

    void insert(int timeslot, int channel, int tx, int rx, bool shared, int sup_start = 0, int sup_dur = 1,
                vector<int> sup_list = {0}, void* scheduled_msg = NULL);

    cell_elem_t find_next_transmitting_slot(int src, int dest);

    cell_elem_t get_action(int asn, int subject_address, int subject_domain, int flavor,
                           vector<timeslot_msg_queue_t>* queue);
    
    bool find_mn(int mn_address);

    void dump_to_file(const char* label);
};

#endif /* SIXTISCH_TSCHSCHEDULE_H_ */
