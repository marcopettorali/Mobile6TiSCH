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

package mobile6tisch.simulations;
import mobile6tisch.nodes.BorderRouter;
import mobile6tisch.nodes.MobileNode;
import mobile6tisch.nodes.NetworkCoordinator;
import mobile6tisch.physicallayer.radio.GlobalRadioRegister;


network Mobile6TiSCHGroupedMNs
{
    parameters:
        int numBR;
        int numLowMNs;
        int numHighMNs;
        double areaWidth @unit(m);
        double areaHeight @unit(m);

        @display("bgb=$areaWidth,$areaHeight");
    submodules:
        nc: NetworkCoordinator {
            parameters:
                numBackboneWirings = parent.numBR;
                @display("p=0,0");
        }

        br[numBR]: BorderRouter {
            parameters:
                brId = index;
                radio.macAddress = index;
                mobility.areaWidth = parent.areaWidth;
                mobility.areaHeight = parent.areaHeight;
                @display("p=85,123");
        }

        low_mns[numMN]: MobileNode {
            parameters:
                mobility.areaWidth = parent.areaWidth;
                mobility.areaHeight = parent.areaHeight;
                @display("p=34,155");
        }
        high_mns[numMN]: MobileNode {
            parameters:
                mobility.areaWidth = parent.areaWidth;
                mobility.areaHeight = parent.areaHeight;
                @display("p=180,155");
        }

        globalRadioRegister: GlobalRadioRegister {
            @display("p=$areaWidth,0");
        }
    connections:
        for i=0..(numBR - 1) {
            br[i].backboneOut --> nc.backboneIn[i];
            nc.backboneOut[i] --> br[i].backboneIn;
        }

}
