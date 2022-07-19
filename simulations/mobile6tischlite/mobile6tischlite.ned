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

package mobile6tisch_lite.simulations.mobile6tischlite;
import mobile6tisch_lite.mobilenode.*;
import mobile6tisch_lite.borderrouter.*;
import mobile6tisch_lite.networkcoordinator.*;
import mobile6tisch_lite.physicallayer.medium.*;
//
// TODO Auto-generated network
//
network Mobile6TiSCHLite
{
    parameters:
        int numMN;
        int numBR;
        
		string application;
        string scheduling;
        int channelsUsed = default(16);
        
        @display("bgb=400,400");

    submodules:
        br[numBR]: BorderRouter{
        	parameters:
        	    radio.macAddress = index;
        }
        mn[numMN]: MobileNode;
        nc: NetworkCoordinator{
            @display("p=-20,-20");
        }
        
        medium: Medium{
            @display("p=-20,-20");
        }
}
