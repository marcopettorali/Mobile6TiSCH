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

package mobile6tisch.physicallayer.radio.contract;

moduleinterface IRadio{
    parameters:
        // BASEMODULE
        string statsCollector;
        string crossLayerDB;

        // IRADIO
        int macAddress;
        string mobility;
        string radioRegister;
        string linkLayer;
        string environmentManager;
        
        double carrierFrequency @unit(Hz);
        double bandwidth @unit(Hz);
        double transmitterPowerMW @unit(mW);
        double transmitterGainMW @unit(mW);
        double receiverGainMW @unit(mW);
        double pathLossMW @unit(mW);
        double thermalNoise @unit(dBm);
        double rssiDBM_Threshold @unit(dBm);
        double snrDBM_Threshold @unit(dBm);
        double bitrate @unit(bps);
        double propagationSpeed @unit(mps);

        double offPowerMW @unit(mW);
        double sleepPowerMW @unit(mW);
        double rxPowerMW @unit(mW);
        double txPowerMW @unit(mW);

        @display("i=block/wrxtx,,0");
    gates:
        input radioIn @directIn;
        input upperLayerIn;
        output upperLayerOut;
}
