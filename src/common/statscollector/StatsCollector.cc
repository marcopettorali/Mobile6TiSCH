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

#include "StatsCollector.h"

Define_Module(StatsCollector);

void StatsCollector::initialize(int stage) {
    if (stage == INITSTAGE_PARAMS_FETCH) {
        emitStats = par("emitStats");
        dumpStats = par("dumpStats");
        verbose = par("verbose");

        enableVectorStats = par("enableVectorStats");

        // iterate through NED parameters
        for (int i = 0; i < getNumParams(); i++) {
            const char *name = par(i).getName();

            // check if the name starts with "stat_"
            if (strncmp(name, "stats_", 6) == 0) {
                std::string statsType = par(i).stringValue();
                // remove the "stat_" prefix
                std::string statsName = std::string(name).substr(6);

                // check if it contains ___ (wildcard). Create a number of stats
                // with the same name and an integer suffix. The number of
                // statistics to create is the number after ___
                size_t pos = statsName.find("___");
                if (pos != std::string::npos) {
                    int numStats = atoi(statsName.substr(pos + 3).c_str());
                    statsName = statsName.substr(0, pos);

                    for (int j = 0; j < numStats; j++) {
                        std::string statName = statsName + std::to_string(j);
                        statsTypes[statName] = statsType;
                        recordsTable[statName] = std::unordered_map<int, std::vector<double>>();
                    }
                } else {
                    // create a statistic
                    statsTypes[statsName] = statsType;
                    recordsTable[statsName] = std::unordered_map<int, std::vector<double>>();
                }
            }
        }
    }
    if (stage == INITSTAGE_LAST_STAGE) {
        if (verbose) {
            // print all recordsTable keys
            INF("StatsCollector") << recordsTable.size() << " statistics registered:" << endl;
        }
    }
}

void StatsCollector::subscribeStats(std::string statName, std::string type) {
    // if the statistic already exists do nothing, otherwise create it
    if (statsTypes.find(statName) == statsTypes.end()) {
        statsTypes[statName] = type;
        recordsTable[statName] = std::unordered_map<int, std::vector<double>>();
    }
}

void StatsCollector::handleMessage(cMessage *msg) {}

void StatsCollector::finish() {
    // call finishCollectStats() on all registered collectables
    int count = 0;
    for (auto it = collectables.begin(); it != collectables.end(); ++it) {
        (*it)->finishCollectStats();
        count++;
    }

    INF("StatsCollector") << count << " collectables registered" << endl;

    computeResults();

    if (emitStats == true) {
        throw cRuntimeError("Emitting stats is not implemented yet");
    }

    if (dumpStats == true) {
        dumpResults();
    }
}

void StatsCollector::record(int id, std::string statName, double value) {
    // check if the statistic exists
    if (recordsTable.find(statName) == recordsTable.end()) {
        throw cRuntimeError("Statistic %s does not exist", statName.c_str());
    }

    // check if the statistic is a timeavg
    if (statsTypes[statName] == "timeavg") {
        // check if the id exists
        if (recordsTable[statName].find(id) == recordsTable[statName].end()) {
            recordsTable[statName][id] = std::vector<double>{0, 0, 0};  // {sum, count, last}
        }
        // get the last time
        double lastTime = recordsTable[statName][id][2];
        // get the current time
        double currentTime = simTime().dbl();
        // get the weighted sum
        double weightedSum = recordsTable[statName][id][0];
        // get the total time
        double totalTime = recordsTable[statName][id][1];
        // compute the new weighted sum
        weightedSum += value * (currentTime - lastTime);
        // compute the new total time
        totalTime += currentTime - lastTime;
        // update the records table
        recordsTable[statName][id] = {weightedSum, totalTime, currentTime};
    } else if (statsTypes[statName] == "sum") {
        // check if the id exists
        if (recordsTable[statName].find(id) == recordsTable[statName].end()) {
            recordsTable[statName][id] = std::vector<double>{0};  // {sum}
        }
        recordsTable[statName][id][0] += value;
    } else if (statsTypes[statName] == "vector") {
        recordsTable[statName][id].push_back(value);
    } else if (statsTypes[statName] == "mean") {
        // check if the id exists
        if (recordsTable[statName].find(id) == recordsTable[statName].end()) {
            recordsTable[statName][id] = {0, 0};  // {sum, count}
        }
        recordsTable[statName][id][0] += value;
        recordsTable[statName][id][1] += 1;

    } else {
        throw cRuntimeError("Statistic %s is not supported", statName.c_str());
    }
}

void StatsCollector::computeResults() {
    for (auto itStatName = recordsTable.begin(); itStatName != recordsTable.end(); ++itStatName) {
        std::string statName = itStatName->first;

        std::unordered_map<std::string, std::vector<double>> statsVector;

        for (auto itID = itStatName->second.begin(); itID != itStatName->second.end(); ++itID) {
            std::vector<double> values = itID->second;

            if (values.size() == 0) {
                continue;
            }

            if (statsTypes[statName] == "timeavg") {
                if (values[1] == 0) {
                    statsVector["timeavg"].push_back(0);
                } else {
                    statsVector["timeavg"].push_back(values[0] / values[1]);
                }
            } else if (statsTypes[statName] == "sum") {
                statsVector["sum"].push_back(values[0]);
            } else if (statsTypes[statName] == "mean") {
                if (values[1] == 0) {
                    statsVector["mean"].push_back(0);
                } else {
                    statsVector["mean"].push_back(values[0] / values[1]);
                }

            } else if (statsTypes[statName] == "vector") {
                // count
                statsVector["count"].push_back(values.size());

                // sum
                double sum = 0;
                for (auto it = values.begin(); it != values.end(); ++it) {
                    sum += *it;
                }
                statsVector["sum"].push_back(sum);

                // compute mean and stdDev
                double mean = sum / values.size();

                double sumOfSquares = 0;
                for (auto it2 = values.begin(); it2 != values.end(); ++it2) {
                    sumOfSquares += (*it2 - mean) * (*it2 - mean);
                }

                double variance = sumOfSquares / values.size();
                double stdDev = sqrt(variance);

                statsVector["mean"].push_back(mean);
                statsVector["variance"].push_back(variance);
                statsVector["stdDev"].push_back(stdDev);

                // compute min and max
                double min = values[0];
                double max = values[0];
                for (auto it2 = values.begin(); it2 != values.end(); ++it2) {
                    if (*it2 < min) {
                        min = *it2;
                    }
                    if (*it2 > max) {
                        max = *it2;
                    }
                }

                statsVector["min"].push_back(min);
                statsVector["max"].push_back(max);

                // compute quartiles and 95th percentile
                std::sort(values.begin(), values.end());
                int size = values.size();
                double q1 = values[size / 4];
                double q2 = values[size / 2];
                double q3 = values[size * 3 / 4];
                double p95 = values[size * 95 / 100];

                statsVector["q1"].push_back(q1);
                statsVector["q2"].push_back(q2);
                statsVector["q3"].push_back(q3);
                statsVector["p95"].push_back(p95);
            }
        }

        // for each name, compute the mean value and put it in the results table
        for (auto it2 = statsVector.begin(); it2 != statsVector.end(); ++it2) {
            std::string name = it2->first;
            std::vector<double> values = it2->second;

            double sum = 0;

            for (auto it3 = values.begin(); it3 != values.end(); ++it3) {
                sum += *it3;
            }

            if (name == "count" || name == "sum") {
                resultsTable[statName][name] = sum;
                resultsTable[statName]["mean"] = sum / values.size();
            } else if (name == "mean") {
                resultsTable[statName][name] = sum / values.size();
            } else {
                resultsTable[statName][name] = sum / values.size();
            }
        }
    }
}

FILE *StatsCollector::createDumpFile() {
    // open the file in write mode in the dumpFolder directory
    cConfigurationEx *config = getEnvir()->getConfigEx();

    // retrieve result-dir folder
    auto v = config->getKeyValuePairs();
    std::string dumpFolder = "simulation/results";
    for (auto it = v.begin(); it != v.end(); it++) {
        std::string key = *it;
        if (key == "result-dir") {
            dumpFolder = *(it + 1);
            break;
        }
    }

    mkdir(dumpFolder.c_str(), 0777);

    // get the name of the configuration  and the repetition number with
    // cConfigurationEx
    std::string configName = config->getActiveConfigName();

    int repetition = atoi(config->getVariable("repetition"));

    // get named iteration variables
    std::string iterationVariables = config->getVariable(CFGVAR_ITERATIONVARS);

    // remove the "
    iterationVariables.erase(std::remove(iterationVariables.begin(), iterationVariables.end(), '"'), iterationVariables.end());

    // parse the iteration variables in an array
    std::vector<std::string> iterationVariablesArray;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = iterationVariables.find(delimiter)) != std::string::npos) {
        token = iterationVariables.substr(0, pos);
        // keep only the value after the "="
        iterationVariablesArray.push_back(token.substr(token.find("=") + 1));
        // iterationVariablesArray.push_back(token);
        iterationVariables.erase(0, pos + delimiter.length());
    }
    // add the last iteration variable
    iterationVariablesArray.push_back(iterationVariables.substr(iterationVariables.find("=") + 1));

    // create the filename
    std::string filename = dumpFolder + "/" + configName + "-";
    for (auto &iterationVariable : iterationVariablesArray) {
        filename += iterationVariable + ",";
    }
    // remove last comma
    filename.pop_back();

    // if filename ends with - delete it
    if (filename.back() == '-') {
        filename.pop_back();
    }

    filename += "-#" + std::to_string(repetition) + ".csv";

    // open a file in append mode
    FILE *file = fopen(filename.c_str(), "w");
    if (file == NULL) {
        throw cRuntimeError("Cannot open file %s", filename.c_str());
    }

    // delete all the .sca files in the directory
    std::string command = "rm " + dumpFolder + "/*-#" + std::to_string(repetition) + ".sca";
    system(command.c_str());

    return file;
}

void StatsCollector::dumpResults() {
    FILE *file = createDumpFile();

    // print each result
    for (auto it = resultsTable.begin(); it != resultsTable.end(); ++it) {
        std::string statName = it->first;
        std::unordered_map<std::string, double> values = it->second;

        for (auto it2 = values.begin(); it2 != values.end(); ++it2) {
            std::string statName2 = it2->first;
            double value = it2->second;

            fprintf(file, "%s.%s, %f\n", statName.c_str(), statName2.c_str(), value);
        }
    }

    // print all the elements in the vectorValuesTable
    // iterate in statsTypes to get the name of the vector
    if (enableVectorStats) {
        for (auto it = statsTypes.begin(); it != statsTypes.end(); ++it) {
            std::string statName = it->first;
            std::string type = it->second;
            if (statsTypes[statName] != "vector") {
                continue;
            }

            std::unordered_map<int, std::vector<double>> valuesForID = recordsTable[statName];
            for (auto it = valuesForID.begin(); it != valuesForID.end(); ++it) {
                int id = it->first;
                std::vector<double> values = it->second;
                fprintf(file, "%s.%d.values, ", statName.c_str(), id);

                for (auto it2 = values.begin(); it2 != values.end(); ++it2) {
                    fprintf(file, "%f, ", *it2);
                }
                fprintf(file, "\n");
            }
        }
    }
}