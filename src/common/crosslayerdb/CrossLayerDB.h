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

#ifndef __MOBILE6TISCH_CROSSLAYERDB_H_
#define __MOBILE6TISCH_CROSSLAYERDB_H_

#include <omnetpp.h>

#include <string>
#include <unordered_map>

#include "utils/initstage.h"
#include "utils/log.h"

using namespace omnetpp;

class CrossLayerDB : public cSimpleModule {
    virtual void initialize(int stage) override;

   protected:
    std::unordered_map<std::string, double> repoDouble;
    std::unordered_map<std::string, int> repoInt;
    std::unordered_map<std::string, std::string> repoString;

   public:
    /*
     * set() overwrites the value if the key already exists
     */
    void set(std::string key, double value) { repoDouble[key] = value; }
    void set(std::string key, std::string value) { repoString[key] = value; }
    void set(std::string key, int value) { repoInt[key] = value; }

    /*
     * getDouble() returns a double value if the key exists, otherwise throws an error
     */
    double getDouble(std::string key) {
        if (repoDouble.find(key) == repoDouble.end()) {
            throw cRuntimeError("Key %s not found in CrossLayerDB", key.c_str());
        }
        return repoDouble[key];
    }

    /*
     * getString() returns a string value if the key exists, otherwise throws an error
     */
    std::string getString(std::string key) {
        if (repoString.find(key) == repoString.end()) {
            throw cRuntimeError("Key %s not found in CrossLayerDB", key.c_str());
        }
        return repoString[key];
    }

    /*
     * getInt() returns an int value if the key exists, otherwise throws an error
     */
    int getInt(std::string key) {
        if (repoInt.find(key) == repoInt.end()) {
            throw cRuntimeError("Key %s not found in CrossLayerDB", key.c_str());
        }
        return repoInt[key];
    }

    /*
     * remove() removes a key from the database if it exists
     */
    void remove(std::string key) {
        repoDouble.erase(key);
        repoString.erase(key);
        repoInt.erase(key);
    }

    /*
     * clear() removes all keys from the database
     */
    void clear() {
        repoDouble.clear();
        repoString.clear();
        repoInt.clear();
    }

    /*
     * checkIfExists() returns true if the key exists, otherwise false
     */
    bool checkIfExists(std::string key) {
        return repoDouble.find(key) != repoDouble.end() || repoString.find(key) != repoString.end() || repoInt.find(key) != repoInt.end();
    }

    void print();
};

#endif
