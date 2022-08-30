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

package mobile6tisch.nodes;

import mobile6tisch.application.IApplication;
import mobile6tisch.ietf6tisch.schedulingfunction.ISchedulingFunction;
import mobile6tisch.shmg.backboneprotocol.BackboneProtocol;
import mobile6tisch.shmg.nccore.NCCore;


module NetworkCoordinator{
    parameters:
        int numBackboneWirings;
        @display("i=device/server,,0;bgb=,,,,0");
    gates:
        input backboneIn[numBackboneWirings];
        output backboneOut[numBackboneWirings];
    submodules:
        backboneProtocol: BackboneProtocol {
            numBackboneWirings = parent.numBackboneWirings;
            @display("p=55,181");
        }
        ncCore: NCCore {
            @display("p=55,111");
        }
        app: <> like IApplication {
            @display("p=55,45");
        }
        schedulingFunction: <> like ISchedulingFunction {
            @display("p=183,111");
        }
    connections:
        for i=0..(numBackboneWirings-1) {
            backboneIn[i] --> backboneProtocol.lowerLayerIn[i];
            backboneOut[i] <-- backboneProtocol.lowerLayerOut[i];
        }

        backboneProtocol.upperLayerOut --> ncCore.lowerLayerIn;
        ncCore.lowerLayerOut --> backboneProtocol.upperLayerIn;

        ncCore.upperLayerOut --> app.lowerLayerIn;
        app.lowerLayerOut --> ncCore.upperLayerIn;
}
