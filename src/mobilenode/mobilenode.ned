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

package mobile6tisch_lite.mobilenode;
import mobile6tisch_lite.mobilenode.mobilenodecore.*;
import mobile6tisch_lite.tsch.*;
import mobile6tisch_lite.physicallayer.radio.*;
import mobile6tisch_lite.physicallayer.mobility.*;

module MobileNode
{
    parameters:
        @display("bgb=270,264");
    gates:
        input radioIn @directIn;

    submodules:
        mobilenodecore: MobileNodeCore;
        tsch: TSCH;
        radio: Radio;
        mobility: Mobility;
    connections:
        mobilenodecore.lowerLayerOut --> tsch.upperLayerIn;
        mobilenodecore.lowerLayerIn <-- tsch.upperLayerOut;

        tsch.lowerLayerOut --> radio.upperLayerIn;
        tsch.lowerLayerIn <-- radio.upperLayerOut;

        radio.radioIn <-- radioIn;

}
