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

#include "SDDUSF.h"

#include <math.h>

#include <algorithm>

Define_Module(SDDUSF);

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

void SDDUSF::initialize() {
    SchedulingFunctionBase::initialize();
    downloadFromNC = (bool)par("downloadFromNC");
    ncCore = check_and_cast<NCCore*>(findModuleByPath(par("ncCore")));

    groupSize = par("groupSize");
}

void SDDUSF::handleMessage(cMessage* msg) {
    // TODO - Generated method body
}

TSCHSchedule* SDDUSF::getSchedule() {
    if (downloadFromNC) {
        return ncCore->getSchedule();
    } else {
        int numMN = ncCore->getNumMNs();

        // determine frameLength
        int frameLength = coprime(1 + ceil((double)numMN / groupSize) + numMN, groupSize);

        // initialize schedule of size frameLength
        TSCHSchedule* sf = new TSCHSchedule(frameLength);

        // put shared cells in the first timeslot
        sf->update(0, 0, MN_BROADCAST_DOMAIN, BR_BROADCAST_DOMAIN, true);

        // determine first upstream and downstream cells
        int nextUpCh = 0;
        int nextUpSlot = 1 + ceil((double)numMN / groupSize);

        int nextDownCh = 0;
        int nextDownSlot = 1;

        // for each entry in mns
        for (int i = 0; i < numMN; i++) {
            int macAddress = ncCore->getMNs()[i].macAddress;

            // allocate up cell
            sf->update(nextUpSlot, nextUpCh, macAddress, BR_BROADCAST_DOMAIN, false);
            nextUpSlot++;
            nextUpCh = 0;

            // allocate down cell
            sf->update(nextDownSlot, nextDownCh, BR_BROADCAST_DOMAIN, macAddress, false);
            nextDownCh = (nextDownCh + 1) % groupSize;
            if (nextDownCh == 0) {
                nextDownSlot++;
            }
        }
        return sf;
    }
}
