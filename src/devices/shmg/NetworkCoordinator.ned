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

package mobile6tisch.devices.shmg;

import mobile6tisch.common.crosslayerdb.CrossLayerDB;
import mobile6tisch.applications.contract.IApp;
import mobile6tisch.middleware.contract.IMiddleware;
import mobile6tisch.linklayer.ieee802154tsch.schedulingfunction.contract.ISchedulingFunction;
import mobile6tisch.physicallayer.wired.contract.IWiredNIC;

module NetworkCoordinator{
    parameters:
        int numBackboneWirings;
        @display("i=device/server,,0;bgb=,,,,0");
    gates:
        input backboneIn[numBackboneWirings];
        output backboneOut[numBackboneWirings];
    submodules:
        wiredNIC: <> like IWiredNIC {
            numWirings = parent.numBackboneWirings;
            @display("p=55,181");
        }
        ncMid:  <> like IMiddleware {
            @display("p=55,111");
        }
        app: <> like IApp {
            @display("p=55,45");
        }
        schedulingFunction: <> like ISchedulingFunction {
            @display("p=183,111");
        }   
        crossLayerDB: CrossLayerDB;
    connections:
        for i=0..(numBackboneWirings-1) {
            backboneIn[i] --> wiredNIC.lowerLayerIn[i];
            backboneOut[i] <-- wiredNIC.lowerLayerOut[i];
        }

        wiredNIC.upperLayerOut --> ncMid.lowerLayerIn;
        ncMid.lowerLayerOut --> wiredNIC.upperLayerIn;

        ncMid.upperLayerOut --> app.lowerLayerIn;
        app.lowerLayerOut --> ncMid.upperLayerIn;
}
