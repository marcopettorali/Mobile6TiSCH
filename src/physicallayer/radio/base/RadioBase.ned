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

package mobile6tisch.physicallayer.radio.base;

import mobile6tisch.physicallayer.radio.contract.IRadio;

simple RadioBase like IRadio{
    parameters:
        // BASEMODULE
        string statsCollector = default("^.^.statsCollector");
        string crossLayerDB = default("^.crossLayerDB");

        // IRADIO
        int macAddress;
        double carrierFrequency @unit(Hz) = default(2.4GHz);
        double bandwidth @unit(Hz) = default(5.2MHz);
        double transmitterPowerMW @unit(mW) = default(1mW);
        double transmitterGainMW @unit(mW) = default(1mW);
        double receiverGainMW @unit(mW) = default(1mW);
        double pathLossMW @unit(mW) = default(1.7mW);
        double thermalNoise @unit(dBm) = default(-96dBm);
        double rssiDBM_Threshold @unit(dBm) = default(-100dBm);
        double snrDBM_Threshold @unit(dBm) = default(-8dBm);
        double bitrate @unit(bps) = default(250kbps);
        double propagationSpeed @unit(mps) = default(300000kmps);

        //energy consumption https://www.ti.com/lit/ds/symlink/cc2420.pdf
        double offPowerMW @unit(mW) = default(0mW);
        double sleepPowerMW @unit(mW) = default(1.28mW);
        double rxPowerMW @unit(mW) = default(56.4mW);
        double txPowerMW @unit(mW) = default(52.2mW);

        // modules
        string mobility = default("^.mobility");
        string radioRegister = default("^.^.globalRadioRegister");
        string linkLayer = default("^.linkLayer");
        string environmentManager = default("^.^.environmentManager");

        @display("i=block/wrxtx,,0");
    gates:
        input radioIn @directIn;
        input upperLayerIn;
        output upperLayerOut;
}
