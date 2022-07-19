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

package mobile6tisch_lite.mobilenode.mobilenodecore;

//
// TODO auto-generated module
//
simple MobileNodeCore
{
    parameters:
        string coordinator = default("^.^.nc");
        string tsch = default("^.tsch");
        
        double period @unit(s) = default(1s);
        double upRate = default(0);
        double downRate = default(0);
        
        @signal[upstreamPacketSent](type=long);
        @statistic[stats_upstreamPacketSent](source="upstreamPacketSent"; record=count; );
        @signal[downstreamPacketReceived](type=long);
        @statistic[stats_downstreamPacketReceived](source="downstreamPacketReceived"; record=count; );
        @signal[downstreamPacketDelay](type=simtime_t);
        @statistic[stats_downstreamPacketDelay](source="downstreamPacketDelay"; record=vector; );
        
        @signal[rtt](type=simtime_t);
        @statistic[stats_rtt](source="rtt"; record=vector; );
    gates:
        input lowerLayerIn;
        output lowerLayerOut;
        
}
