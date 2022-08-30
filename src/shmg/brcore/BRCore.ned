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

package mobile6tisch.shmg.brcore;

//
// TODO auto-generated module
//
simple BRCore{
    parameters:
        string ietf6TiSCH = default("^.ietf6TiSCH");
        string ncCore = default("^.^.nc.ncCore");
        @display("i=block/routing,,0");
    gates:
        input lowerLayerBackboneIn;
        output lowerLayerBackboneOut;

        input lowerLayer6TiSCHIn;
        output lowerLayer6TiSCHOut;
}
