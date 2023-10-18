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

package mobile6tisch.common.statscollector;

simple StatsCollector{

    bool emitStats = default(false);
    bool dumpStats = default(true);

    bool enableVectorStats = default(false);

    bool verbose = default(false);

    // stats and types
    string stats_upstreamPacketsSent = "sum";
    string stats_upstreamPacketsReceived = "sum";
    string stats_downstreamPacketsSent = "sum";
    string stats_downstreamPacketsReceived = "sum";
    string stats_upstreamDelay = "vector";
    string stats_downstreamDelay = "vector";
    string stats_endToEndDelay = "vector";
    string stats_PAFSent = "sum";
    string stats_PAFReceived = "sum";
    string stats_PAPSent = "sum";
    string stats_PAPReceived = "sum";
    string stats_queueSize = "timeavg";
    string stats_atLeastOneRetransmission = "sum";
    string stats_retransmissions = "sum";
    string stats_retransmissionsCount = "sum";
    string stats_retransmissionsIntertime = "timeavg";
    string stats_packetsDropped = "sum";

    // stats packet delivery
    string stats_pdr95Perc = "vector";

    // radio packet loss
    string stats_radioPacketOutcomes___7 = "sum";

    // packet loss network layer
    string stats_packetsLostSuccess = "sum";
    string stats_packetsLostTransmissionErrors = "sum";
    string stats_packetsLostConflict = "sum";
    string stats_packetsLostConflictRedundant = "sum";
    string stats_packetsLostConflictNotRedundant = "sum";
    string stats_packetsLostOutOfRange = "sum";
    string stats_packetsLostOther = "sum";
    string stats_packetsLostSuccessSupport = "sum";
    string stats_packetsLostCrossing = "sum";

    //hints
    string stats_hintNum = "sum";
    string stats_hint_T_L_C = "sum";
    string stats_hint_T_L_NC = "sum";

    string stats_receptionsPerPacket = "mean";

    string stats_averagePowerConsumptionMN = "timeavg";
    string stats_averagePowerConsumptionBR = "timeavg";

    string stats_receptionConflictsInSlotframe = "mean";
    string stats_targetsInTsRatios___16 = "mean";
    string stats_receptionConflictsInSlotframeNodesInvolved = "mean";
}
