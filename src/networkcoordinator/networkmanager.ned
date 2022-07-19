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

package mobile6tisch_lite.networkcoordinator;

//
// TODO auto-generated module
//
simple NetworkCoordinator
{
    parameters:
        double downstreamPeriod @unit(s) = default(0s);
        double musfCtrlMsgRateUp = default(0);
        double musfCtrlMsgRateDown = default(0);
        bool musfRemoveUnusedTimeslots= default(true);

        @signal[upstreamPacketDelay](type=simtime_t);
        @statistic[stats_upstreamPacketDelay](source="upstreamPacketDelay"; record=vector; );
        @signal[downstreamPacketSent](type=long);
        @statistic[stats_downstreamPacketSent](source="downstreamPacketSent"; record=count; );
        @signal[upstreamPacketReceived](type=long);
        @statistic[stats_upstreamPacketReceived](source="upstreamPacketReceived"; record=count; );
}
