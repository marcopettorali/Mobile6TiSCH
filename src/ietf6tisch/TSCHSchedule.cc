#include "TSCHSchedule.h"

#include <math.h>

TSCHSchedule::TSCHSchedule(int frame_length) {
    this->frameLength = frame_length;
    sf = new cell_t[CHANNELS * frame_length];
    for (int ts = 0; ts < frameLength; ts++) {
        for (int ch = 0; ch < CHANNELS; ch++) {
            at(ts,ch)->timeslot = ts;
            at(ts,ch)->channel = ch;
            at(ts,ch)->tx = NONE_ADDRESS;
            at(ts,ch)->rx = NONE_ADDRESS;
            at(ts,ch)->shared = false;
            at(ts,ch)->scheduled_msg = nullptr;
        }
    }
}

TSCHSchedule::~TSCHSchedule() { delete[] sf; }

void TSCHSchedule::update(int timeslot, int channel, int tx, int rx, bool shared, void* scheduled_msg) {
    at(timeslot, channel)->tx = tx;
    at(timeslot, channel)->rx = rx;
    at(timeslot, channel)->shared = shared;
    at(timeslot, channel)->scheduled_msg = scheduled_msg;

}

cell_t TSCHSchedule::findNextTransmittingTimeSlot(int srcMACAddr, int destMACAddr) {
    for (int ts = 0; ts < frameLength; ts++) {
        for (int ch = 0; ch < CHANNELS; ch++) {
            cell_t* cell = at(ts, ch);
            if (cell->tx == srcMACAddr && cell->rx == destMACAddr && cell->shared == false) {
                return *at(ts, ch);
            }
        }
    }

    return NULL_CELL;
}

cell_t TSCHSchedule::getAction(ostream& EV, int asn, int subject_address, int subject_domain, int flavor, vector<timeslot_msg_queue_t>* queue) {

    for (int ts = 0; ts < frameLength; ts++) {
        for (int ch = 0; ch < CHANNELS; ch++) {
            cell_t* cell = at(ts, ch);
            if (cell->timeslot != ts || cell->channel != ch) {
                EV << "ERROR "<< ts << " --> " << cell->timeslot <<", " << ch << " --> " << cell->channel << endl;
            }
        }
    }

    int timeslot = asn % frameLength;

    cell_t ret = {-1,-1,-1,-1,false,NULL};
    cell_t *sh = NULL, *rx = NULL, *tx = NULL;

    for (int i = 0; i < CHANNELS; i++) {
        cell_t* cell = at(timeslot, i);

        if (cell->shared) {
            sh = cell;
        } else if ((cell->tx == subject_address || cell->tx == subject_domain) && queue->at(timeslot).size() > 0 && queue->at(timeslot).front().first == i) {
            tx = cell;
        } else if (cell->rx == subject_address || cell->rx == subject_domain) {
            rx = cell;
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
            EV << "Got cell: " << ret.channel << endl;
        } else if (tx != NULL) {
            ret = (*tx);
            EV << "Got cell: " << ret.channel << endl;
            ret.scheduled_msg = queue->at(timeslot).front().second;
            queue->at(timeslot).pop_front();
        }
    }

    EV << "OUTSIDE Got cell: " << ret.tx << " " << ret.rx << " " << ret.channel << endl;


    return ret;
}

bool TSCHSchedule::isAddressPresent(int mn_address) {
    for (int i = 0; i < CHANNELS; i++) {
        for (int j = 0; j < frameLength; j++) {
            cell_t* cell = at(j, i);
            if (cell->tx == mn_address || cell->rx == mn_address) {
                return true;
            }
        }
    }
    return false;
}
