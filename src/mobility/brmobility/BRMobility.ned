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

package mobile6tisch.mobility.brmobility;
import mobile6tisch.mobility.contract.IMobility;

simple BRMobility like IMobility{
    // BASEMODULE
    string statsCollector = default("^.^.statsCollector");
    string crossLayerDB = default("^.crossLayerDB");

    // IMOBILITY
    string environmentManager = default("^.^.environmentManager");
        
    bool randomInitPosition = default(false);
    double initX @unit(m) = default(0m);
    double initY @unit(m) = default(0m);
    double speed @unit(mps) = 0mps;
    double areaWidth @unit(m);
    double areaHeight @unit(m);
    double updatePositionInterval @unit(s) = default(0.5s);
    @display("i=block/cogwheel,,0");

    // BRMOBILITY
    string brDeploymentPolicy;
    double targetCommunicationRadius @unit(m);
    string listBasedPos = default("");
    bool throwErrorOnPositionInObstacle = default(false);
}
