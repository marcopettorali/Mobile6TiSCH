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

#include "NCCore.h"

#include <math.h>

#include <algorithm>

Define_Module(NCCore);

/******************************************************************************
 * Initialization
 ******************************************************************************/
void NCCore::initialize(int stage) {
    if (stage == 0) {  // init structures
        schedulingFunction = check_and_cast<SchedulingFunctionBase *>(findModuleByPath(par("schedulingFunction")));

        downstreamPacketSent = registerSignal("downstreamPacketSent");
        upstreamPacketReceived = registerSignal("upstreamPacketReceived");
        upstreamPacketDelay = registerSignal("upstreamPacketDelay");

        // initialize clean beep
        this->clean_beep = new cMessage("clean_beep");
        scheduleAt(simTime() + CLEAN_PERIOD_S, clean_beep);

    } else if (stage == 3) {                                 // build schedule
        this->sf = this->schedulingFunction->getSchedule();  // TODO
        sf->printSchedule(EV);
        frameLength = sf->frameLength;
    }
}

/**********************************************************************************************
 * HANDLE PACKETS
 **********************************************************************************************/

void NCCore::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (msg->isName("clean_beep")) {
            dedupTable.clean();
            // distanceTable.clean((double)simTime().dbl());
            scheduleAt(simTime() + CLEAN_PERIOD_S, clean_beep);
        }
    } else if (msg->getArrivalGate() == gate("lowerLayerIn")) {
        BackbonePkt *backPkt = check_and_cast<BackbonePkt *>(msg);
        handleLowerLayerPacket(backPkt);
    }else if (msg->getArrivalGate() == gate("upperLayerIn")) {
        BackbonePkt *backPkt = check_and_cast<BackbonePkt *>(msg);
        handleUpperLayerPacket(backPkt);
    } 
}

void NCCore::handleLowerLayerPacket(BackbonePkt *backPkt) {
    int mobileNode = backPkt->getMobileNode();
    int borderRouter = backPkt->getBorderRouter();
    int seq = backPkt->getSeq();
    double rssi = backPkt->getRssi();

    mn_vector_entry_t *mnEntry = findMNByAddress(mobileNode);
    distanceTable.insert(seq, mobileNode, borderRouter, simTime().dbl(), rssi);

    updateDelegateBR(mnEntry->macAddress);

    if (dedupTable.checkMessage(mobileNode, seq)) {
        send(backPkt->decapsulate(), "upperLayerOut");
    }

    delete backPkt;
}

void NCCore::handleUpperLayerPacket(BackbonePkt *backPkt){
    int delegateBR = findMNByAddress(backPkt->getMobileNode())->delegateBR;

    // throw exception if delegateBR is not found
    if (delegateBR == -1) {
        throw cRuntimeError("Delegate BR not found for mn %d", backPkt->getMobileNode());
    }

    for (int i = 0; i < brs.size(); i++) {
        if (brs[i].macAddress == delegateBR) {
            backPkt->setOutPort(brs[i].brId);
            break;
        }
    }
    send(backPkt, "lowerLayerOut");
}



/**********************************************************************************************
 * NODES REGISTRATION
 **********************************************************************************************/

void NCCore::registerMN(int macAddress, double upRate, double downRate,  double upDelay, double downDelay,  double x, double y, double maxSpeed) {
    // find the closest BR as delegateBR
    int delegateBR = findClosestBR(x, y);
    upsertMNForBR(delegateBR, macAddress, false);

    mn_vector_entry_t entry = {macAddress, upRate, downRate, upDelay, downDelay, x, y, delegateBR, maxSpeed, {}};
    mns.push_back(entry);
}

void NCCore::registerBR(int macAddress, int brId, BRCore *br, double x, double y, double r) { brs.push_back({macAddress, brId, br, x, y, r, {}}); }

/****************************************************************************************
 *  UTILITY FUNCTIONS
 ****************************************************************************************/

#define distance(x1, y1, x2, y2) sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))

std::vector<int> NCCore::getBRNeighborsList(int brAddress, double radius) {
    std::vector<int> neighbors;

    // find br by address
    br_vector_entry_t br;
    for (int i = 0; i < brs.size(); i++) {
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

mn_vector_entry_t *NCCore::findMNByAddress(int mnAddress) {
    mn_vector_entry_t *mnEntry;
    for (int i = 0; i < mns.size(); i++) {
        if (mns[i].macAddress == mnAddress) {
            mnEntry = &mns[i];
            break;
        }
    }
    return mnEntry;
}

br_vector_entry_t *NCCore::findBRByAddress(int brAddress) {
    br_vector_entry_t *brEntry;
    for (int i = 0; i < brs.size(); i++) {
        if (brs[i].macAddress == brAddress) {
            brEntry = &brs[i];
            break;
        }
    }
    return brEntry;
}

void NCCore::updateDelegateBR(int mnAddress) {
    mn_vector_entry_t *mn = findMNByAddress(mnAddress);
    mn->delegateBR = distanceTable.getLatestHighestEntry(mnAddress).br;

    // update lists
    upsertBRForMN(mn->macAddress, mn->delegateBR);
    upsertMNForBR(mn->delegateBR, mn->macAddress);

    // update schedule if necessary
    br_vector_entry_t *delegateBR = findBRByAddress(mn->delegateBR);
    if (find(delegateBR->mns.begin(), delegateBR->mns.end(), mn->macAddress) ==
        delegateBR->mns.end()) {
        //delegateBR->module->updateSchedule(delegateBR->mns);
    }
}

void NCCore::upsertMNForBR(int brAddress, int mnAddressToInsert, bool scheduleToUpdate) {
    br_vector_entry_t *br = findBRByAddress(brAddress);
    auto mnEntry = find(br->mns.begin(), br->mns.end(), mnAddressToInsert);
    if (mnEntry == br->mns.end()) {
        br->mns.push_back(mnAddressToInsert);
        if (scheduleToUpdate) {
            //throw new cRuntimeError("Not implemented");
            //br->module->updateSchedule(br->mns);
        }
    } else {  // update
        *mnEntry = mnAddressToInsert;
    }
}

void NCCore::upsertBRForMN(int mnAddress, int brAddressToInsert) {
    mn_vector_entry_t *mn = findMNByAddress(mnAddress);
    br_in_mn_t *br = NULL;
    // find the br to be updated
    for (int i = 0; i < mn->brs.size(); i++) {
        if (mn->brs[i].macAddress == brAddressToInsert) {
            br = &mn->brs[i];
        }
    }
    if (br == NULL) {
        mn->brs.push_back({brAddressToInsert, (double)simTime().dbl() + BR_ENTRY_LIFETIME_S});
    } else {  // update timestamp
        *br = {brAddressToInsert, (double)simTime().dbl() + BR_ENTRY_LIFETIME_S};
    }
}

int NCCore::findClosestBR(double x, double y) {
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
