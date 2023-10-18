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

package mobile6tisch.mobility.linearmobility;
import mobile6tisch.mobility.contract.IMobility;

simple LinearMobility like IMobility{
    // BASEMODULE
    string statsCollector = default("^.^.statsCollector");
    string crossLayerDB = default("^.crossLayerDB");

    // IMOBILITY
    string environmentManager = default("^.^.environmentManager");
        
    bool randomInitPosition = default(true);
    double initX @unit(m) = default(-1m);
    double initY @unit(m) = default(-1m);
    double speed @unit(mps) = default(1 mps);
    double areaWidth @unit(m);
    double areaHeight @unit(m);
    double updatePositionInterval @unit(s) = default(1s);
    @display("i=block/cogwheel");

    // LINEARMOBILITY    
    bool forceVHTrajectories = default(true);
}
