#ifndef SIXTISCH_TSCHSCHEDULE_H_
#define SIXTISCH_TSCHSCHEDULE_H_

#include <deque>
#include <ostream>
#include <sstream>
#include <vector>
using namespace std;

#define CHANNELS 16
#define TIMESLOT_DURATION_S 0.015
#define GUARD_TIME_S 0.001

#define NONE_ADDRESS -1
#define NULL_CELL \
    { NONE_ADDRESS, NONE_ADDRESS, NONE_ADDRESS, NONE_ADDRESS, false, NULL }

typedef deque<pair<int, void*>> timeslot_msg_queue_t;

struct cell_t {
    // properties of the cell
    int channel;
    int timeslot;
    int tx;
    int rx;
    bool shared;

    // returned message
    void* scheduled_msg;
};

class TSCHSchedule {
    cell_t* sf;

    cell_t action_policy(int timeslot, vector<cell_t> cell_elem_list, int subject_address, int subject_domain, vector<timeslot_msg_queue_t>* queue, int flavor);

   public:
    TSCHSchedule(int frameLength);
    ~TSCHSchedule();

    int frameLength;

    cell_t* at(int timeslot, int channel) { return &sf[channel * frameLength + timeslot]; }

    void update(int timeslot, int channel, int tx, int rx, bool shared, void* scheduled_msg = NULL);

    cell_t findNextTransmittingTimeSlot(int src, int dest);

    cell_t getAction(ostream& ev, int asn, int subject_address, int subject_domain, int flavor, vector<timeslot_msg_queue_t>* queue);

    bool isAddressPresent(int mn_address);

    void printSchedule(ostream& out, bool compact=false) {
        for (int i = 0; i < CHANNELS; i++) {
            out << "| ";
            for (int j = 0; j < frameLength; j++) {
                cell_t* elem = at(j, i);
                if(compact){out << (elem->tx > elem->rx ? elem->tx : -elem->rx);}
                else{out << "(" << elem->tx << "," << elem->rx << ")";}
                out << "| ";
            }
            out << endl;
        }
    }
};

#endif /* SIXTISCH_TSCHSCHEDULE_H_ */
