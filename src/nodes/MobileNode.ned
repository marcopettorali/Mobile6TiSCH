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
import mobile6tisch.ietf6tisch.IETF6TiSCH;
import mobile6tisch.ietf6tisch.schedulingfunction.ISchedulingFunction;
import mobile6tisch.physicallayer.mobility.IMobility;
import mobile6tisch.physicallayer.radio.Radio;


module MobileNode
{
    parameters:
        @display("bgb=287,263,,,0;i=device/wifilaptop,,0");
    gates:
        input radioIn @directIn;
    submodules:
        mobility: <> like IMobility {
            @display("p=72,195");
        }
        radio: Radio {
            @display("p=203,195");
        }
        ietf6TiSCH: IETF6TiSCH {
            @display("p=203,113");
        }
        app: <> like IApplication {
            @display("p=203,38");
        }
        schedulingFunction: <> like ISchedulingFunction {
            @display("p=71.75,112.886665");
        }
    connections:
        radioIn --> radio.radioIn;
        radio.upperLayerOut --> ietf6TiSCH.lowerLayerIn;
        radio.upperLayerIn <-- ietf6TiSCH.lowerLayerOut;
        ietf6TiSCH.upperLayerOut --> app.lowerLayerIn;
        app.lowerLayerOut --> ietf6TiSCH.upperLayerIn;
}
