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

package mobile6tisch.physicallayer.mobility;
import mobile6tisch.physicallayer.mobility.IMobility;

simple BRMobility like IMobility{
    double startX @unit(m) = default(0m);
    double startY @unit(m) = default(0m);
    double speed @unit(mps) = 0mps;
    double areaWidth @unit(m);
    double areaHeight @unit(m);
    double updatePositionInterval @unit(s) = default(0.5s);

    string brDeploymentPolicy;
    double targetCommunicationRadius @unit(m);

    double intFlowersAlpha = default(1);

    string listBasedPos = default("");
    @display("i=block/cogwheel,,0");
}
