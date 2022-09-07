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

package mobile6tisch.ietf6tisch;

//
// TODO auto-generated module
//
simple IETF6TiSCH
{
    parameters:
        string radio = default("^.radio");
        string mobility = default("^.mobility");
        string schedulingFunction = default("^.schedulingFunction");

        double baseFrequency @unit(Hz) = default(2.4GHz);
        double baseBandwidth @unit(Hz) = default(5.2MHz);

        int queueLength = default(-1);

        bool areRetransmissionsEnabled = default(true);
        int maxRetransmissions = default(4);

        @signal[stagesMN](type=long);
        @statistic[stats_stagesMN](source="stagesMN"; record=vector; );

        @signal[queueSize](type=long);
        @statistic[stats_queueSize](source="queueSize"; record=vector; );

        @signal[packetsDropped](type=long);
        @statistic[stats_packetsDropped](source="packetsDropped"; record=count; );

        @signal[retransmissions](type=long);
        @statistic[stats_retransmissions](source="retransmissions"; record=count; );

        @signal[retransmissionsIntertime](type=double);
        @statistic[stats_retransmissionsIntertime](source="retransmissionsIntertime"; record=timeavg; );

        @signal[retransmissionsCount](type=long);
        @statistic[stats_retransmissionsCount](source="retransmissionsCount"; record=avg; );

        @signal[atLeastOneRetransmission](type=long);
        @statistic[stats_atLeastOneRetransmission](source="atLeastOneRetransmission"; record=count; );

        @display("i=block/layer,,0");
    gates:
        input lowerLayerIn;
        output lowerLayerOut;

        input upperLayerIn;
        output upperLayerOut;
}
