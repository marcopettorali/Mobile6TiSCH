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

package mobile6tisch.mobility.contract;

moduleinterface IMobility{
    parameters:

        // BASEMODULE
        string statsCollector;
        string crossLayerDB;  

        // IMOBILITY
        string environmentManager;
        
        bool randomInitPosition;
        double initX @unit(m);
        double initY @unit(m);
        double speed @unit(mps);
        double areaWidth @unit(m);
        double areaHeight @unit(m);
        double updatePositionInterval @unit(s);
        
        @display("i=block/cogwheel");
        
}
