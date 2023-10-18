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

package mobile6tisch.linklayer.ieee802154tsch;

import mobile6tisch.linklayer.contract.ILinkLayer;
simple IEEE802154TSCH like ILinkLayer {
    parameters:
        // BASEMODULE
        string statsCollector = default("^.^.statsCollector");
        string crossLayerDB = default("^.crossLayerDB");

        // ILINKLAYER
        @display("i=block/rxtx");

        // IEEE802154TSCH
        string radio = default("^.radio");
        string mobility = default("^.mobility");
        string schedulingFunction = default("^.schedulingFunction");

        int queueLength = default(-1);

        bool enableRetransmissionsInSharedCells = default(false);
        bool enableRetransmissionsInDedicatedCells = default(false);
        
        // csma-ca
        int macMinBE = default(3);
        int macMaxBE = default(5);
        int macMaxFrameRetries = default(3);
        
        @display("i=block/layer,,0");
    gates:
        input lowerLayerIn;
        output lowerLayerOut;

        input upperLayerIn;
        output upperLayerOut;
}
