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

package mobile6tisch_lite.physicallayer.mobility;

//
// TODO auto-generated module
//
simple Mobility
{
    parameters:
        int mobilityType;
        bool autoPosition = default(false);
        string positioningPolicy = default("intersecting_flower");
        double radius @unit(m);

        double maxXSpeed @unit(mps);
        double maxYSpeed @unit(mps) = default(-1mps);
        double startX @unit(m);
        double startY @unit(m);
        double areaWidth @unit(m);
        double areaHeight @unit(m) = default(-1m);

        double updatePositionInterval @unit(s) = default(0.5s);

        string customBrPositions = default("");
}
