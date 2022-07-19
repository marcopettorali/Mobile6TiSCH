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

#include "networkcoordinator.h"

#include <math.h>

#include <algorithm>

Define_Module(NetworkCoordinator);

/******************************************************************************
 * Math utility functions
 ******************************************************************************/

int coprime(int n, int C, int s = +1) {
    int i = n;
    while (true) {
        if (__gcd(i, C) == 1) {
            return i;
        }
        i += s;
    }
}

/******************************************************************************
 * Initialization
 ******************************************************************************/
void NetworkCoordinator::initialize(int stage) {
    if (stage == 0) {  // init structures
        numMN = getParentModule()->par("numMN");

        application = getParentModule()->par("application").stringValue();
        scheduling = getParentModule()->par("scheduling").stringValue();

        channelsUsed = getParentModule()->par("channelsUsed");

        downstreamPacketSent = registerSignal("downstreamPacketSent");
        upstreamPacketReceived = registerSignal("upstreamPacketReceived");
        upstreamPacketDelay = registerSignal("upstreamPacketDelay");

        // initialize clean beep
        this->clean_beep = new cMessage("clean_beep");
        scheduleAt(simTime() + CLEAN_PERIOD_S, clean_beep);

        // setup downstream timer
        if (application == "reqres") {
            // nope
        } else if (application == "convergecast") {
        } else if (application == "downstream_rr") {
            // downstreamPeriod = par("downstreamPeriod");
            downstreamPeriod =
                frameLength * 0.015 /
                (ceil((double)numMN / channelsUsed));  // TODO fix this !!!

            downstreamBeep = new cMessage("downstreamBeep");
            scheduleAt(simTime() + downstreamPeriod, downstreamBeep);
            nextMN = 0;
        } else {
            throw new cException("Unknown application type");
        }
    } else if (stage == 3) {  // build schedule
        if (scheduling == "sddu") {
            sdduScheduling();
        } else if (scheduling == "dddu") {
            ddduScheduling();
        } else if (scheduling == "musf") {
            musfScheduling();
        } else {
            throw new cException("Unknown scheduling type");
        }
    }
}

/**********************************************************************************************
 * HANDLE PACKETS
 **********************************************************************************************/

void NetworkCoordinator::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (msg->isName("clean_beep")) {
            dedupTable.clean();
            if (scheduling == "musf") {
                // delete all brs entries in every mn which cleanTimestamp is
                // older than currentTime
                for (int i = 0; i < mns.size(); i++) {
                    mn_vector_entry_t *mn = &mns[i];
                    for (auto br = mn->brs.begin(); br != mn->brs.end();) {
                        SimTime currentTime = simTime();
                        SimTime cleanTimestamp = SimTime(br->cleanTimestamp);
                        double currentConv = currentTime.dbl();
                        double cleanConv = cleanTimestamp.dbl();

                        if (currentConv > cleanConv) {
                            // delete mn in mns list for br vector entry
                            removeMNFromBRAndUpdate(br->macAddress,
                                                  mn->macAddress);

                            br = mn->brs.erase(br);
                        } else {
                            br++;
                        }
                    }
                }
            }

            scheduleAt(simTime() + CLEAN_PERIOD_S, clean_beep);
        } else if (msg->isName("downstreamBeep")) {
            int mobileNode = mns[nextMN].macAddress;
            nextMN = (nextMN + 1) % mns.size();
            sendDownstreamPkt(mobileNode, simTime());
        }
    }
}

void NetworkCoordinator::deliverUpstreamPkt(cMessage *msg) {
    Enter_Method("deliverUpstreamPkt(%p)", msg);

    UpstreamPkt *up = check_and_cast<UpstreamPkt *>(msg);

    int mobileNode = up->getMobileNode();
    mn_vector_entry_t *mn = findMNByAddress(mobileNode);

    PacketControlInfo *info =
        check_and_cast<PacketControlInfo *>(up->removeControlInfo());

    distanceTable.insert(up->getSeqNum(), mobileNode, up->getBR(),
                         simTime().dbl(), info->getRssi());

    // perform operations on pkt received for each scheduling algorithm
    if (scheduling == "sddu" || scheduling == "dddu") {
        updateDelegateBR(mn->macAddress);
    } else if (scheduling == "musf") {
        // update cleanTimestamp for the BR of the mobile node
        upsertBRForMN(mn->macAddress, up->getBR());
        musfSchedulingOnUpstreamPkt(up, info->getRssi());
    }

    if (dedupTable.checkMessage(mobileNode, up->getSeqNum())) {
        emit(upstreamPacketReceived, 1);
        emit(upstreamPacketDelay, simTime() - up->getSendTime());

        if (application == "reqres") {
            sendDownstreamPkt(mobileNode, up->getUpstreamSendTime());
        }
    }

    delete info;
    delete up;
}

void NetworkCoordinator::sendDownstreamPkt(int mn, simtime_t sendTime) {
    int delegateBR = findMNByAddress(mn)->delegateBR;

    // throw exception if delegateBR is not found
    if (delegateBR == -1) {
        throw cRuntimeError("Delegate BR not found for mn %d", mn);
    }

    DownstreamPkt *down = new DownstreamPkt("DownstreamPkt");
    down->setKind(DOWNSTREAM_PACKET_K);
    down->setSeqNum(downstreamSeqNum++);
    down->setMobileNode(mn);
    down->setDelegateBR(delegateBR);
    down->setSendTime(simTime());
    down->setUpstreamSendTime(sendTime);

    for (int i = 0; i < brs.size(); i++) {
        if (brs[i].macAddress == delegateBR) {
            sendDirect(down, brs[i].module->getParentModule(),                       "fromNetworkManager");
            emit(downstreamPacketSent, 1);
            break;
        }
    }
}

mn_vector_entry_t *NetworkCoordinator::findMNByAddress(int mnAddress) {
    mn_vector_entry_t *mnEntry;
    for (int i = 0; i < mns.size(); i++) {
        if (mns[i].macAddress == mnAddress) {
            mnEntry = &mns[i];
            break;
        }
    }
    return mnEntry;
}

br_vector_entry_t *NetworkCoordinator::findBRByAddress(int brAddress) {
    br_vector_entry_t *brEntry;
    for (int i = 0; i < brs.size(); i++) {
        if (brs[i].macAddress == brAddress) {
            brEntry = &brs[i];
            break;
        }
    }
    return brEntry;
}

/**********************************************************************************************
 * NODES REGISTRATION
 **********************************************************************************************/

void NetworkCoordinator::registerMN(int macAddress, double upRate,
                                    double downRate, double x, double y,
                                    double maxSpeed) {
    // find the closest BR as delegateBR
    int delegateBR = findClosestBR(x, y);
    upsertMNForBR(delegateBR, macAddress, false);

    mn_vector_entry_t entry = {macAddress, upRate,     downRate, x,
                               y,          delegateBR, maxSpeed, {}};
    mns.push_back(entry);
}

void NetworkCoordinator::registerBR(int macAddress, BorderRouterCore *br,
                                    double x, double y, double r) {
    brs.push_back({macAddress, br, x, y, r, {}});
}

/**********************************************************************************************
 * SCHEDULING FUNCTIONS
 **********************************************************************************************/

void NetworkCoordinator::sdduScheduling() {
    // determine frameLength
    frameLength =
        coprime(1 + ceil((double)numMN / channelsUsed) + numMN, channelsUsed);

    // initialize schedule of size frameLength
    sf = new TSCHSchedule(frameLength);

    // put shared cells in the first timeslot
    sf->insert(0, 0, MN_BROADCAST_DOMAIN, BR_BROADCAST_DOMAIN, true);

    // determine first upstream and downstream cells
    int nextUpCh = 0;
    int nextUpSlot = 1 + ceil((double)numMN / channelsUsed);

    int nextDownCh = 0;
    int nextDownSlot = 1;

    // for each entry in mns
    for (int i = 0; i < mns.size(); i++) {
        int macAddress = mns[i].macAddress;

        // allocate up cell
        sf->insert(nextUpSlot, nextUpCh, macAddress, BR_BROADCAST_DOMAIN,
                   false);
        nextUpSlot++;
        nextUpCh = 0;

        // allocate down cell
        sf->insert(nextDownSlot, nextDownCh, BR_BROADCAST_DOMAIN, macAddress,
                   false);
        nextDownCh = (nextDownCh + 1) % channelsUsed;
        if (nextDownCh == 0) {
            nextDownSlot++;
        }
    }
}

void NetworkCoordinator::ddduScheduling() {
    // determine frameLength
    frameLength = coprime(1 + 2 * numMN, channelsUsed);

    // initialize schedule of size frameLength
    sf = new TSCHSchedule(frameLength);

    // put shared cells in the first timeslot
    sf->insert(0, 0, MN_BROADCAST_DOMAIN, BR_BROADCAST_DOMAIN, true);
    // determine first upstream and downstream cells
    int nextUpCh = 0;
    int nextUpSlot = 1 + numMN;

    int nextDownCh = 0;
    int nextDownSlot = 1;

    // for each entry in mns
    for (int i = 0; i < mns.size(); i++) {
        int macAddress = mns[i].macAddress;

        // allocate up cell
        sf->insert(nextUpSlot, nextUpCh, macAddress, BR_BROADCAST_DOMAIN,
                   false);
        nextUpSlot++;
        nextUpCh = 0;

        // allocate down cell
        sf->insert(nextDownSlot, nextDownCh, BR_BROADCAST_DOMAIN, macAddress,
                   false);
        nextDownCh = 0;
        nextDownSlot++;
    }
}

#define for_range(iter, n) for (int iter = 0; iter < n; iter++)
#define rate_to_ts(r) (int)std::floor(1.0 / (r * TIMESLOT_DURATION_S))
#define lcm(a, b) ((a * b) / __gcd(a, b))
#define utilization(len, ts) (ts < len) ? -1 : (double)1 / std::floor(ts / len)

void NetworkCoordinator::musfScheduling() {
    double musfCtrlMsgRateUp = par("musfCtrlMsgRateUp");
    double musfCtrlMsgRateDown = par("musfCtrlMsgRateDown");
    bool musfRemoveUnusedTimeslots = par("musfRemoveUnusedTimeslots");

    struct musf_vector_entry_t {
        int macAddress;
        char type;
        int ts;
    };

    vector<musf_vector_entry_t> musfVector;

    // convert rate requirements in timeslot requirements

    // control messages
    int tsUp = rate_to_ts(musfCtrlMsgRateUp);
    int tsDown = rate_to_ts(musfCtrlMsgRateDown);

    // mns
    for_range(i, mns.size()) {
        musfVector.push_back(
            {mns[i].macAddress, 'u', rate_to_ts(mns[i].upRate)});
        musfVector.push_back(
            {mns[i].macAddress, 'd', rate_to_ts(mns[i].downRate)});
    }

    // sort musf_vector by ts (ascending)
    sort(musfVector.begin(), musfVector.end(),
         [](const musf_vector_entry_t &a, const musf_vector_entry_t &b) {
             return a.ts < b.ts;
         });

    // define frameLength
    int minimumReq =
        (int)std::floor(std::min(musfVector.at(0).ts, min(tsUp, tsDown)));
    frameLength = coprime(minimumReq, channelsUsed, -1);

    // init matrix of required timeslots
    vector<vector<musf_vector_entry_t>> sf_matrix;
    for_range(i, frameLength * channelsUsed) { sf_matrix.push_back({}); }

    // place control cells
    double upU = utilization(frameLength, tsUp);
    double downU = utilization(frameLength, tsDown);
    bool ctrlMsgSplit;
    if (upU + downU <= 1) {
        for_range(i, channelsUsed) {
            sf_matrix[i * frameLength + 0].push_back({-1, 'u', tsUp});
            sf_matrix[i * frameLength + 0].push_back({-1, 'd', tsDown});
        }
        ctrlMsgSplit = false;
    } else {  // otherwise upstream in timeslot 0 and downstream in timeslot 1
        for_range(i, channelsUsed) {
            sf_matrix[i * frameLength + 0].push_back({-1, 'u', tsUp});
        }
        for_range(i, channelsUsed) {
            sf_matrix[i * frameLength + 1].push_back({-1, 'd', tsDown});
        }
        ctrlMsgSplit = true;
    }

    // allocate every requirement in a cell:
    // 1) reuse a cell of the same node if possible (avoid conflicts on
    // upstream/downstream in the same node) 2) find the cell with the least
    // utilization
    for (auto t : musfVector) {
        int macAddress = t.macAddress;

        bool nodeFound = false;

        map<pair<int, int>, double> candidates;
        bool exit = false;
        for (int ts = 0; !exit && ts < frameLength; ts++) {
            for (int ch = 0; ch < channelsUsed; ch++) {
                // nodes occupying the (i,j) cell
                vector<int> addressesInCell;

                // utilization of the cell + the new node
                double u = utilization(frameLength, t.ts);

                for (int k = 0; k < sf_matrix[ch * frameLength + ts].size();
                     k++) {
                    musf_vector_entry_t elem =
                        sf_matrix[ch * frameLength + ts][k];

                    addressesInCell.push_back(
                        elem.macAddress);  // remember node
                    u += utilization(frameLength,
                                     elem.ts);  // update utilization
                }

                // if the cell contains a cell of this node, and utilization is
                // < 1 => reuse
                if (find(addressesInCell.begin(), addressesInCell.end(),
                         macAddress) != addressesInCell.end() &&
                    u <= 1) {
                    nodeFound = true;
                    sf_matrix[ch * frameLength + ts].push_back(t);
                    exit = true;
                    break;
                } else if (u <= 1) {  // otherwise insert another candidate if
                                      // this timeslot does not contain any
                                      // other cell of the node
                    bool nameFound = false;
                    for (int n = 0; n < channelsUsed; n++) {
                        for (int o = 0;
                             o < sf_matrix[n * frameLength + ts].size(); o++) {
                            if (sf_matrix[n * frameLength + ts][o].macAddress ==
                                macAddress) {
                                nameFound = true;
                                break;
                            }
                        }
                    }
                    if (!nameFound) {
                        candidates[{ch, ts}] = u;
                    }
                }
            }
        }

        // if no cell was found, find the best candidate
        if (!nodeFound && candidates.size() > 0) {
            // find the cell with the minimum utilization
            int minCh = -1;
            int minTs = -1;
            double minU = -1;
            for (auto c : candidates) {
                if (minU == -1 || c.second < minU) {
                    minCh = c.first.first;
                    minTs = c.first.second;
                    minU = c.second;
                }
            }

            sf_matrix[minCh * frameLength + minTs].push_back(t);
        } else if (!nodeFound) {  // if no candidate was found, return error
            throw cRuntimeError("No cell found for node %d", macAddress);
            return;
        }
    }

    // initialize the final schedule
    sf = new TSCHSchedule(frameLength);

    // define the superschedule for each cell and insert cell in the schedule

    for (int ch = 0; ch < channelsUsed; ch++) {
        for (int ts = 0; ts < frameLength; ts++) {
            vector<musf_vector_entry_t> cell = sf_matrix[ch * frameLength + ts];
            int l = 1;

            // handle the presence of control messages slots in this timeslot
            if ((ts == 0) || (ts == 1 && ctrlMsgSplit)) {
                for_range(i, channelsUsed) {
                    for_range(j, sf_matrix[i * frameLength + ts].size()) {
                        l = lcm(l, (int)std::floor(
                                       sf_matrix[i * frameLength + ts][j].ts /
                                       frameLength));
                    }
                }
            }

            else if (cell.size() > 1) {
                for_range(i, cell.size()) {
                    l = lcm(l, (int)std::floor(cell[i].ts / frameLength));
                }
            }

            vector<pair<int, char>> superschedule;
            for_range(i, l) { superschedule.push_back({-1, 'e'}); }

            for_range(i, cell.size()) {
                vector<int> indexesInSuperschedule;

                int u = (int)std::floor(cell[i].ts / frameLength);
                int toPlace = 0;
                for_range(i, l) {
                    if (i % u == 0) {
                        toPlace++;
                    }
                    if (superschedule[i].second == 'e' && toPlace > 0) {
                        superschedule[i] = {cell[i].macAddress, cell[i].type};
                        indexesInSuperschedule.push_back(i);
                        toPlace--;
                    }
                }

                // insert cell into schedule
                int tx = (cell[i].type == 'u'
                              ? (cell[i].macAddress == -1 ? MN_BROADCAST_DOMAIN
                                                          : cell[i].macAddress)
                              : BR_BROADCAST_DOMAIN);
                int rx = (cell[i].type == 'u' ? BR_BROADCAST_DOMAIN
                                              : (cell[i].macAddress == -1
                                                     ? MN_BROADCAST_DOMAIN
                                                     : cell[i].macAddress));
                bool shared = (cell[i].macAddress == -1 ? true : false);

                if ((cell[i].macAddress == -1 && ch == 0) ||
                    (cell[i].macAddress != -1)) {  // only insert shared cells
                                                   // in the first channel
                    sf->insert(ts, ch, tx, rx, shared, 0, l,
                               indexesInSuperschedule);
                }
            }
        }
    }
}

void NetworkCoordinator::musfSchedulingOnUpstreamPkt(UpstreamPkt *up,
                                                     double rssiMW) {
    // retrieve info about the mobile node
    int mobileNode = up->getMobileNode();
    mn_vector_entry_t *mn = findMNByAddress(mobileNode);

    // reset the time on br
    upsertBRForMN(mobileNode, up->getBR());

    // compute the worst case radius
    double distance = sqrt(0.00000582 / rssiMW);
    double worstCaseRadius =
        distance + mn->maxSpeed * 1 / (std::max(mn->upRate, mn->downRate));

    // entry in the distance table for mobileNode
    distance_table_entry_t entry = distanceTable.getEntries(mobileNode);

    // check if it is moving away from the BR which received the packet
    br_vector_entry_t *br = findBRByAddress(up->getBR());

    packet_entry_t *recent = NULL;
    packet_entry_t *older = NULL;
    for (int j = 0; j < entry.size(); j++) {
        if (entry[j].br == br->macAddress && recent == NULL) {  // TODO CHECK
            recent = &entry[j];
        } else if (entry[j].br == br->macAddress && older == NULL) {
            older = &entry[j];
        } else if (recent != NULL && older != NULL) {
            break;
        }
    }

    // if I have at least 2 entries for this br
    if (older != NULL && recent != NULL) {
        // moving prevision
        if ((recent->rssi < older->rssi) &&
            (distance >=
             br->r * sqrt(3) /
                 2)) {  // if node is moving away and distance >= sqrt(3)/2 *
                        // radius => mobile node is moving away from brs[i]

            // find neighbors of brs[i] far worst case radius
            vector<int> neighbors =
                getNeighborsList(br->macAddress, worstCaseRadius);

            // install cells of mn on each neighbor
            for (int j = 0; j < neighbors.size(); j++) {
                auto currentBR = findBRByAddress(neighbors[j]);

                br_vector_entry_t *neighbor = findBRByAddress(neighbors[j]);

                upsertBRForMN(mobileNode,
                                               neighbor->macAddress);
                upsertMNForBR(neighbor->macAddress, mobileNode);
            }

            // update delegateBR
            updateDelegateBR(mn->macAddress);
        }
    }

    // remove the cells for far BRs (after installation ==> correct BRs which
    // are neighbors of the abandoned BR but not of the one that received the
    // pkt)
    cleanMNFromFarBRs(mobileNode, up->getBR(), worstCaseRadius);
}

TSCHSchedule *NetworkCoordinator::getSchedule(vector<int> targets) {
    TSCHSchedule *ret = new TSCHSchedule(frameLength);
    for (int i = 0; i < sf->get_frame_length(); i++) {
        for (int j = 0; j < sf->get_channel_count(); j++) {
            cell_t *cell = sf->sf_at(i, j);
            for (int k = 0; k < cell->size(); k++) {
                cell_elem_t elem = cell->at(k);
                int tx = elem.tx;
                int rx = elem.rx;

                if (find(targets.begin(), targets.end(), tx) != targets.end() ||
                    find(targets.begin(), targets.end(), rx) != targets.end() ||
                    elem.shared) {
                    ret->insert(i, j, elem.tx, elem.rx, elem.shared,
                                elem.sup_start, elem.sup_dur, elem.sup_list,
                                elem.scheduled_msg);
                }
            }
        }
    }
    return ret;
}

#define distance(x1, y1, x2, y2) \
    sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))

vector<int> NetworkCoordinator::getInitialSchedule(int brAddress) {
    vector<int> targets;
    // find br by address
    br_vector_entry_t *br = findBRByAddress(brAddress);

    if (scheduling == "sddu" || scheduling == "dddu") {
        for (int i = 0; i < mns.size(); i++) {
            targets.push_back(mns[i].macAddress);
            upsertMNForBR(brAddress, mns[i].macAddress);
        }


    } else if (scheduling == "musf") {  // MUSF
        

        // find all the mns that are distant <= r from br
        for (int i = 0; i < mns.size(); i++) {
            mn_vector_entry_t mn = mns[i];
            if (distance(br->x, br->y, mn.x, mn.y) <= br->r) {
                targets.push_back(mns[i].macAddress);
                upsertBRForMN(mns[i].macAddress,
                                               br->macAddress);
                upsertMNForBR(br->macAddress, mns[i].macAddress);
            }
        }
    }

    return targets;
}

vector<int> NetworkCoordinator::getNeighborsList(int brAddress, double radius) {
    vector<int> neighbors;

    // find br by address
    br_vector_entry_t br;
    for_range(i, brs.size()) {
        if (brs[i].macAddress == brAddress) {
            br = brs[i];
            break;
        }
    }

    for (int i = 0; i < brs.size(); i++) {
        br_vector_entry_t br2 = brs[i];
        // continue if br2 is the same as br
        if (br2.macAddress == br.macAddress) {
            continue;
        }
        // check if br2 is in range
        if (distance(br.x, br.y, br2.x, br2.y) - br2.r <= radius) {
            neighbors.push_back(br2.macAddress);
        }
    }
    return neighbors;
}

void NetworkCoordinator::updateDelegateBR(int mnAddress) {
    mn_vector_entry_t *mn = findMNByAddress(mnAddress);
    mn->delegateBR = distanceTable.getLatestHighestEntry(mnAddress).br;

    // update lists
    upsertBRForMN(mn->macAddress, mn->delegateBR);
    upsertMNForBR(mn->delegateBR, mn->macAddress);

    // update schedule if necessary
    br_vector_entry_t *delegateBR = findBRByAddress(mn->delegateBR);
    if (find(delegateBR->mns.begin(), delegateBR->mns.end(), mn->macAddress) ==
        delegateBR->mns.end()) {
        delegateBR->module->updateSchedule(delegateBR->mns);
    }
}

void NetworkCoordinator::upsertMNForBR(int brAddress,
                                            int mnAddressToInsert,
                                            bool scheduleToUpdate) {
    br_vector_entry_t *br = findBRByAddress(brAddress);
    auto mnEntry = find(br->mns.begin(), br->mns.end(), mnAddressToInsert);
    if (mnEntry == br->mns.end()) {
        br->mns.push_back(mnAddressToInsert);
        if (scheduleToUpdate) {
            br->module->updateSchedule(br->mns);
        }
    } else {  // update
        *mnEntry = mnAddressToInsert;
    }
}
void NetworkCoordinator::upsertBRForMN(int mnAddress,
                                                        int brAddressToInsert) {
    mn_vector_entry_t *mn = findMNByAddress(mnAddress);
    br_in_mn_t *br = NULL;
    // find the br to be updated
    for (int i = 0; i < mn->brs.size(); i++) {
        if (mn->brs[i].macAddress == brAddressToInsert) {
            br = &mn->brs[i];
        }
    }
    if (br == NULL) {
        mn->brs.push_back(
            {brAddressToInsert, (double)simTime().dbl() + BR_ENTRY_LIFETIME_S});
    } else {  // update timestamp
        *br = {brAddressToInsert,
               (double)simTime().dbl() + BR_ENTRY_LIFETIME_S};
    }
}

void NetworkCoordinator::removeMNFromBRAndUpdate(int brAddress, int mnAddress) {
    br_vector_entry_t *br = findBRByAddress(brAddress);
    auto mnEntry = find(br->mns.begin(), br->mns.end(), mnAddress);
    if (mnEntry != br->mns.end()) {
        br->mns.erase(mnEntry);
        br->module->updateSchedule(br->mns);
    }
}

void NetworkCoordinator::cleanMNFromFarBRs(int mnAddress, int pivotBRAddress,
                                           double radius) {
    mn_vector_entry_t *mn = findMNByAddress(mnAddress);
    br_vector_entry_t *br = findBRByAddress(pivotBRAddress);

    // iterate through brs
    for (int i = 0; i < brs.size(); i++) {
        br_vector_entry_t *br2 = &brs[i];
        // continue if br2 is the same as br
        if (br2->macAddress == br->macAddress) {
            continue;
        }

        // check if br2 is far from br
        if (distance(br->x, br->y, br2->x, br2->y) - br2->r > radius) {
            // check if mn is in br2
            auto mnEntry = find(br2->mns.begin(), br2->mns.end(), mnAddress);
            if (mnEntry != br2->mns.end()) {
                // remove mnEntry
                br2->mns.erase(mnEntry);
                br2->module->updateSchedule(br2->mns);
            }
        }
    }
}

int NetworkCoordinator::findClosestBR(double x, double y) {
    // iterate all brs and find the closest one to (x,y)
    double minDistance = numeric_limits<double>::max();
    int closestBR = -1;
    for (int i = 0; i < brs.size(); i++) {
        br_vector_entry_t *br = &brs[i];
        double distance = distance(x, y, br->x, br->y);
        if (distance < minDistance) {
            minDistance = distance;
            closestBR = br->macAddress;
        }
    }
    return closestBR;
}