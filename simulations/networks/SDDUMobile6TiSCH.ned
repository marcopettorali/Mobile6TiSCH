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

package mobile6tisch.simulations.networks;
import mobile6tisch.devices.shmg.BorderRouter;
import mobile6tisch.devices.shmg.MobileNode;
import mobile6tisch.devices.shmg.NetworkCoordinator;
import mobile6tisch.physicallayer.radio.base.GlobalRadioRegister;
import mobile6tisch.common.statscollector.StatsCollector;
import mobile6tisch.environment.base.EnvironmentManagerBase;

network SDDUMobile6TiSCH{
    parameters:
        int numBR;
        int numMN;
        double areaWidth @unit(m);
        double areaHeight @unit(m);

        @display("bgb=$areaWidth,$areaHeight");
    submodules:

        statsCollector: StatsCollector{
            @display("p=0,$areaHeight");
        }

        globalRadioRegister: GlobalRadioRegister {
            @display("p=$areaWidth,0");
        }

        environmentManager: EnvironmentManagerBase {
            parameters:
                maxWidth = parent.areaWidth;
                maxHeight = parent.areaHeight;
                @display("p=$areaWidth,$areaHeight");
        }

        nc: NetworkCoordinator {
            parameters:
                numBackboneWirings = parent.numBR;
                @display("p=0,0");
        }

        br[numBR]: BorderRouter {
            parameters:
                brId = index;
                radio.macAddress = 10000 + index;
                mobility.areaWidth = parent.areaWidth;
                mobility.areaHeight = parent.areaHeight;
                @display("p=85,123");
        }

        mn[numMN]: MobileNode {
            parameters:
                mnId = index;
                radio.macAddress = 20000 + index;
                mobility.areaWidth = parent.areaWidth;
                mobility.areaHeight = parent.areaHeight;
                @display("p=85,188");
        }


    connections:
        for i=0..(numBR - 1) {
            br[i].backboneOut -->{@display("ls=,0");}--> nc.backboneIn[i];
            nc.backboneOut[i] -->{@display("ls=,0");}--> br[i].backboneIn;
        }

}
