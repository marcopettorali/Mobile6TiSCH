#include "tschschedule.h"

#include <math.h>

TSCHSchedule::TSCHSchedule(int frame_length) {
    this->frame_length = frame_length;
    // init a matrix CHANNELS x frame_length
    sf = new cell_t[CHANNELS * frame_length];
}

TSCHSchedule::~TSCHSchedule() { delete[] sf; }

int TSCHSchedule::get_queue_size(vector<timeslot_msg_queue_t>* queue, int time_slot) {
    return queue->at(time_slot).size();
}

void TSCHSchedule::insert(int timeslot, int channel, int tx, int rx, bool shared, int sup_start, int sup_dur,
                          vector<int> sup_list, void* scheduled_msg) {
    cell_elem_t cell_elem;
    cell_elem.channel = channel;
    cell_elem.timeslot = timeslot;
    cell_elem.tx = tx;
    cell_elem.rx = rx;
    cell_elem.shared = shared;
    cell_elem.sup_start = sup_start;
    cell_elem.sup_dur = sup_dur;
    cell_elem.sup_list = sup_list;
    cell_elem.scheduled_msg = scheduled_msg;

    sf_at(timeslot, channel)->push_back(cell_elem);
}

cell_elem_t TSCHSchedule::find_next_transmitting_slot(int src, int dest) {
    for (int ts = 0; ts < frame_length; ts++) {
        for (int ch = 0; ch < CHANNELS; ch++) {
            cell_t* cell = sf_at(ts, ch);
            for (int k = 0; k < cell->size(); k++) {
                cell_elem_t elem = cell->at(k);
                if (elem.tx == src && elem.rx == dest && elem.shared == false) {
                    return sf_at(ts, ch)->at(k);
                }
            }
        }
    }

    return NULL_CELL;
}

cell_elem_t TSCHSchedule::get_action(int asn, int subject_address, int subject_domain, int flavor,
                                     vector<timeslot_msg_queue_t>* queue) {
    int timeslot = asn % frame_length;

    vector<cell_elem_t> cell_elem_list;

    for (int i = 0; i < CHANNELS; i++) {
        cell_t* cell = sf_at(timeslot, i);
        if (cell->size() > 0) {
            // for each element in the cell, check if the superschedule is active at the current asn for each cell elem
            for (int j = 0; j < cell->size(); j++) {
                cell_elem_t cell_elem = cell->at(j);

                int sf_start_asn = asn - (asn % frame_length);
                int previous_sup_start_asn =
                    cell_elem.sup_start +
                    cell_elem.sup_dur * floor((sf_start_asn - cell_elem.sup_start) / cell_elem.sup_dur);
                int sup_index = (sf_start_asn - previous_sup_start_asn) / frame_length;

                // for each element in the superschedule list, check if the superschedule is active at the current asn
                for (int k = 0; k < cell_elem.sup_list.size(); k++) {
                    if (cell_elem.sup_list.at(k) == sup_index) {
                        cell_elem_list.push_back(cell_elem);
                    }
                }
            }
        }
    }

    // if there are more cells in cell_elem_list, decide which one to use!
    return action_policy(timeslot, cell_elem_list, subject_address, subject_domain, queue, flavor);
}

cell_elem_t TSCHSchedule::action_policy(int timeslot, vector<cell_elem_t> cell_elem_list, int subject_address,
                                        int subject_domain, vector<timeslot_msg_queue_t>* queue, int flavor) {

    cell_elem_t ret;
    // if there are no cells in cell_elem_list, return an empty cell_elem
    if (cell_elem_list.size() == 0) {
        return NULL_CELL;
    }

    if (flavor == 0) {  // SD-DU / DD-DU scheduling
        cell_elem_t *sh = NULL, *rx = NULL, *tx = NULL;

        for (int i = 0; i < cell_elem_list.size(); i++) {
            if (cell_elem_list[i].shared) {
                sh = &cell_elem_list[i];
            } else if ((cell_elem_list[i].tx == subject_address || cell_elem_list[i].tx == subject_domain) &&
                       queue->at(timeslot).size() > 0 && queue->at(timeslot).front().first == i) {
                tx = &cell_elem_list[i];
            } else if (cell_elem_list[i].rx == subject_address || cell_elem_list[i].rx == subject_domain) {
                rx = &cell_elem_list[i];
            }
        }

        if (sh != NULL && tx == NULL) {
            ret = (*sh);
            if (queue->at(timeslot).size() > 0 && queue->at(timeslot).front().first == (*sh).channel) {
                ret.scheduled_msg = queue->at(timeslot).front().second;
                queue->at(timeslot).pop_front();
                ret.tx = subject_address;
            } else {
                ret.rx = subject_address;
            }
        } else {
            if (rx == NULL && tx == NULL) {
                ret.tx = -1;
                ret.rx = -1;
            } else if (rx != NULL && tx == NULL) {
                ret = (*rx);
            } else if (tx != NULL) {
                ret = (*tx);
                ret.scheduled_msg = queue->at(timeslot).front().second;
                queue->at(timeslot).pop_front();
            }
        }
    } else if (flavor == 1) {  // MUSF scheduling
        // choose a random cell from cell_elem_list
        int random_index = rand() % cell_elem_list.size();
        cell_elem_t ret = cell_elem_list[random_index];
    }

    return ret;
}

#include <ctime>
#include <fstream>
#define max(a, b) ((a) > (b) ? (a) : (b))
void TSCHSchedule::dump_to_file(const char* label) {
    std::time_t result = std::time(nullptr);

    ofstream dump;
    dump.open("dump.txt", ios::out | ios::app);
    dump << "*********************************************************" << endl;
    dump << "LABEL: " << label << ", TIMESTAMP : " << std::asctime(std::localtime(&result)) << endl;
    dump << "*********************************************************" << endl;
    for (int i = 0; i < CHANNELS; i++) {
        dump << "| ";
        for (int j = 0; j < frame_length; j++) {
            for (int k = 0; k < sf_at(j, i)->size(); k++) {
                cell_elem_t elem = sf_at(j, i)->at(k);
                dump << "(" << max(elem.tx, elem.rx) << ") ";
            }
            dump << "| ";
        }
        dump << endl;
    }

    dump.close();
}

bool TSCHSchedule::find_mn(int mn_address){
    for (int i = 0; i < CHANNELS; i++) {
        for (int j = 0; j < frame_length; j++) {
            for (int k = 0; k < sf_at(j, i)->size(); k++) {
                cell_elem_t elem = sf_at(j, i)->at(k);
                if (elem.tx == mn_address || elem.rx == mn_address) {
                    return true;
                }
            }
        }
    }
    return false;
}