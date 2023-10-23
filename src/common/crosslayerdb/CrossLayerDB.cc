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

#include "CrossLayerDB.h"

Define_Module(CrossLayerDB);

void CrossLayerDB::initialize(int stage){
    if(stage == INITSTAGE_BASEMODULE){
        repoInt = std::unordered_map<std::string, int>();
        repoDouble = std::unordered_map<std::string, double>();
        repoString = std::unordered_map<std::string, std::string>();

        set("testInt", 0);
        set("testDouble", 0);
        set("testString", "");
    }

}


void CrossLayerDB::print() {
    for (auto const& x : repoDouble) {
        EV << x.first << ": " << x.second << endl;
    }
    for (auto const& x : repoInt) {
        EV << x.first << ": " << x.second << endl;
    }
    for (auto const& x : repoString) {
        EV << x.first << ": " << x.second << endl;
    }
}