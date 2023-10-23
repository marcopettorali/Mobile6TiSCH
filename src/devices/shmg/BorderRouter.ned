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
import mobile6tisch.middleware.contract.IMiddleware;
import mobile6tisch.linklayer.contract.ILinkLayer;
import mobile6tisch.linklayer.ieee802154tsch.schedulingfunction.contract.ISchedulingFunction;
import mobile6tisch.mobility.brmobility.BRMobility;
import mobile6tisch.physicallayer.radio.contract.IRadio;
import mobile6tisch.physicallayer.wired.contract.IWiredNIC;



module BorderRouter{
    parameters:
        int brId;
    gates:
        input radioIn @directIn;
        input backboneIn;
        output backboneOut;
    submodules:
        // TSCH network
        radio: <> like IRadio {
            @display("p=236,190");
        }
        linkLayer: <> like ILinkLayer{
            @display("p=236,116");
        }
        mobility: BRMobility {
            @display("p=66,190");
        }

        schedulingFunction: <> like ISchedulingFunction {
            @display("p=349,116");
        }

        // Backbone network
        wiredNIC: <> like IWiredNIC {
            numWirings = 1;
            @display("p=55,181");
        }

        // Middleware
        brMid:  <> like IMiddleware {
            @display("p=144,42");
        }
        
        crossLayerDB: CrossLayerDB;
    connections:
        radioIn --> radio.radioIn;
        backboneIn --> wiredNIC.lowerLayerIn[0];
        backboneOut <-- wiredNIC.lowerLayerOut[0];
        radio.upperLayerOut --> linkLayer.lowerLayerIn;
        linkLayer.lowerLayerOut --> radio.upperLayerIn;
        linkLayer.upperLayerOut --> brMid.lowerLayerIn;
        brMid.lowerLayerOut --> linkLayer.upperLayerIn;
        wiredNIC.upperLayerOut --> brMid.upperLayerIn;
        brMid.upperLayerOut --> wiredNIC.upperLayerIn;
}
