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
import mobile6tisch.linklayer.contract.ILinkLayer;
import mobile6tisch.middleware.contract.IMiddleware;
import mobile6tisch.linklayer.ieee802154tsch.schedulingfunction.contract.ISchedulingFunction;
import mobile6tisch.mobility.contract.IMobility;
import mobile6tisch.physicallayer.radio.contract.IRadio;


module MobileNode{
    parameters:
        int mnId;
    gates:
        input radioIn @directIn;
    submodules:
        mobility: <> like IMobility {
            @display("p=72,195");
        }
        radio: <> like IRadio {
            @display("p=203,195");
        }
        linkLayer: <> like ILinkLayer{
            @display("p=236,116");
        }
        mnMid: <> like IMiddleware {
            @display("p=71.75,38");
        }
        app: <> like IApp {
            @display("p=203,38");
        }
        schedulingFunction: <> like ISchedulingFunction {
            @display("p=71.75,112.886665");
        }
        crossLayerDB: CrossLayerDB;
    connections:
        radioIn --> radio.radioIn;
        radio.upperLayerOut --> linkLayer.lowerLayerIn;
        radio.upperLayerIn <-- linkLayer.lowerLayerOut;
        linkLayer.upperLayerOut --> mnMid.lowerLayerIn;
        linkLayer.upperLayerIn <-- mnMid.lowerLayerOut;
        mnMid.upperLayerOut --> app.lowerLayerIn;
        mnMid.upperLayerIn <-- app.lowerLayerOut;
}
