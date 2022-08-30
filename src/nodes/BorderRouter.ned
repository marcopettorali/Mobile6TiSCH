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

import mobile6tisch.ietf6tisch.IETF6TiSCH;
import mobile6tisch.ietf6tisch.schedulingfunction.ISchedulingFunction;
import mobile6tisch.physicallayer.mobility.BRMobility;
import mobile6tisch.physicallayer.mobility.IMobility;
import mobile6tisch.physicallayer.radio.Radio;
import mobile6tisch.shmg.backboneprotocol.BackboneProtocol;
import mobile6tisch.shmg.brcore.BRCore;
import mobile6tisch.shmg.nccore.NCCore;


module BorderRouter{
    parameters:
        int brId;
        @display("bgb=427,253,,,0;i=abstract/router,,0");
    gates:
        input radioIn @directIn;
        input backboneIn;
        output backboneOut;
    submodules:
        radio: Radio {
            @display("p=236,190");
        }
        ietf6TiSCH: IETF6TiSCH {
            @display("p=236,116");
        }

        backboneProtocol: BackboneProtocol {
            numBackboneWirings = 1;
            @display("p=66,116");
        }

        brCore: BRCore {
            @display("p=144,42");
        }
        mobility: BRMobility {
            @display("p=66,190");
        }
        schedulingFunction: <> like ISchedulingFunction {
            @display("p=349,116");
        }
    connections:
        radioIn --> radio.radioIn;
        backboneIn --> backboneProtocol.lowerLayerIn[0];
        backboneOut <-- backboneProtocol.lowerLayerOut[0];


        radio.upperLayerOut --> ietf6TiSCH.lowerLayerIn;
        ietf6TiSCH.lowerLayerOut --> radio.upperLayerIn;
        ietf6TiSCH.upperLayerOut --> brCore.lowerLayer6TiSCHIn;
        brCore.lowerLayer6TiSCHOut --> ietf6TiSCH.upperLayerIn;
        backboneProtocol.upperLayerOut --> brCore.lowerLayerBackboneIn;
        brCore.lowerLayerBackboneOut --> backboneProtocol.upperLayerIn;
}
