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

#include "NewSF.h"

#include <math.h>

#include <algorithm>

#include "ortools/linear_solver/linear_solver.h"

using namespace operations_research;

Define_Module(NewSF);

struct cell_util {
    int ts;
    int ch;
    int src;
};

std::pair<int, std::vector<cell_util>> computeDownstreamSlots(int numMN, int macAddresses[], double downRates[], int frameLength, int groupSize, double Ts) {
    // compute requested cells for each node
    int downCells[numMN];
    for (int i = 0; i < numMN; i++) {
        downCells[i] = (int)ceil(frameLength * Ts * downRates[i]);
    }

    // setup optimization problem
    // https://www.sciencedirect.com/science/article/pii/S0377221702007269?casa_token=UlenXXZmpyoAAAAA:__XsE-oOJwQdPXCqRLHGNk1aD_nAxadwlfbfzq7kTa362HR8UIQfSrtns0XJw69B3CRV6SB4Uio Create the mip solver
    // with the SCIP backend.
    std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));
    if (!solver) {
        throw new cRuntimeError("Could not create solver.");
    }

    MPVariable* x[numMN * groupSize];
    for (int i = 0; i < numMN; i++) {
        for (int j = 0; j < groupSize; j++) {
            x[i * groupSize + j] = solver->MakeBoolVar("x");
        }
    }

    // create y as integer value
    const double infinity = solver->infinity();
    MPVariable* const y = solver->MakeIntVar(0.0, infinity, "y");

    // add sum constraint
    // iterate jobs
    for (int i = 0; i < numMN; i++) {
        std::string name = "mn" + std::to_string(i);

        // create a constraint for each job
        MPConstraint* const c = solver->MakeRowConstraint(1, 1, name);

        c->SetCoefficient(y, 0);

        // initialize all coefficients to 0
        for (int j = 0; j < numMN * groupSize; j++) {
            c->SetCoefficient(x[j], 0);
        }

        // iterate machines
        for (int j = 0; j < groupSize; j++) {
            c->SetCoefficient(x[i * groupSize + j], 1);
        }
    }

    for (int i = 0; i < groupSize; i++) {
        std::string name = "channel" + std::to_string(i);

        // create a constraint for each machine
        MPConstraint* const c = solver->MakeRowConstraint(0, infinity, name);
        c->SetCoefficient(y, 1);

        // initialize all coefficients to 0
        for (int j = 0; j < numMN * groupSize; j++) {
            c->SetCoefficient(x[j], 0);
        }

        // iterate jobs
        for (int j = 0; j < numMN; j++) {
            c->SetCoefficient(x[j * groupSize + i], -downCells[j]);
        }
    }

    MPObjective* const objective = solver->MutableObjective();
    for (int i = 0; i < numMN * groupSize; i++) {
        objective->SetCoefficient(x[i], 0);
    }
    objective->SetCoefficient(y, 1);
    objective->SetMinimization();

    // Solve the problem and print the solution.
    const MPSolver::ResultStatus result_status = solver->Solve();

    if (result_status != MPSolver::OPTIMAL) {
        throw new cRuntimeError("No optimal solution found.");
    }

    // retrieve solution
    // G indexes
    int gIndex[groupSize];
    for (int i = 0; i < groupSize; i++) {
        gIndex[i] = 0;
    }

    std::vector<cell_util> ret;
    for (int i = 0; i < numMN; i++) {
        for (int j = 0; j < groupSize; j++) {
            if (x[i * groupSize + j]->solution_value() == 1) {
                for (int k = 0; k < downCells[i]; k++) {
                    cell_util cell;
                    cell.ts = gIndex[j];
                    cell.ch = j;
                    cell.src = macAddresses[i];
                    ret.push_back(cell);
                    gIndex[j]++;
                }
            }
        }
    }

    return {y->solution_value(), ret};
}

void NewSF::initialize() {
    SchedulingFunctionBase::initialize();
    downloadFromNC = (bool)par("downloadFromNC");
    ncCore = check_and_cast<NCCore*>(findModuleByPath(par("ncCore")));

    groupSize = par("groupSize");
}

void NewSF::handleMessage(cMessage* msg) {}

TSCHSchedule* NewSF::getSchedule() {
    if (downloadFromNC) {
        return ncCore->getSchedule();
    } else {
        // extract information from ncCore
        std::vector<mn_vector_entry_t> mns = ncCore->getMNs();
        int numMN = mns.size();

        int macAddresses[numMN];

        double upRates[numMN];
        double downRates[numMN];
        double upDelay[numMN];
        double downDelay[numMN];

        double upPeriods[numMN];
        double downPeriods[numMN];

        for (int i = 0; i < numMN; i++) {
            macAddresses[i] = mns[i].macAddress;

            upRates[i] = mns[i].upRate;
            downRates[i] = mns[i].downRate;
            upDelay[i] = mns[i].upDelay;
            downDelay[i] = mns[i].downDelay;

            upPeriods[i] = 1 / upRates[i];
            downPeriods[i] = 1 / downRates[i];
        }

        // 1. Compute frameLength

        // compute minimum delay
        double minUpDelay = *std::min_element(upDelay, upDelay + numMN);
        double minDownDelay = *std::min_element(downDelay, downDelay + numMN);
        double minDelay = std::min(minUpDelay, minDownDelay);

        int frameLength = coprime((int)floor(minDelay / TIMESLOT_DURATION_S), 16);

        // 2. Schedulability test

        // compute number of upstream timeslots
        int upCells[numMN];
        int numUpSlots = 0;
        for (int i = 0; i < numMN; i++) {
            upCells[i] = (int)ceil(frameLength * TIMESLOT_DURATION_S * upRates[i]);
            numUpSlots += upCells[i];
        }

        // compute number of downstream timeslots
        std::pair<int, std::vector<cell_util>> ret = computeDownstreamSlots(numMN, macAddresses, downRates, frameLength, groupSize, TIMESLOT_DURATION_S);
        int numDownSlots = ret.first;
        std::vector<cell_util> downCells = ret.second;

        // check if enough timeslots
        if (frameLength < numUpSlots + numDownSlots) {
            throw new cRuntimeError("Not enough timeslots to schedule all cells.");
        }

        // 3. Place cells

        // create schedule
        TSCHSchedule* sf = new TSCHSchedule(frameLength);

        // put upstream cells
        int lastIndex = 0;
        for(int i = 0; i<numMN; i++){
            for(int j = 0; j<upCells[i]; j++){
                sf->update(lastIndex, 0, macAddresses[i], BR_BROADCAST_DOMAIN, false);
                lastIndex++;
            }
        }

        //put downstream cells
        for(int i = 0; i<downCells.size(); i++){
            sf->update(lastIndex + downCells[i].ts, downCells[i].ch, BR_BROADCAST_DOMAIN, downCells[i].src, false);
        }

        return sf;
    }
}
