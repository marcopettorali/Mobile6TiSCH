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

#ifndef __MOBILE6TISCH_RADIO_H_
#define __MOBILE6TISCH_RADIO_H_

#include <omnetpp.h>

#include "GlobalRadioRegister.h"
#include "RadioPkt_m.h"
#include "SignalDescriptor_m.h"
#include "common/basemodule/BaseModule.h"
#include "environment/base/EnvironmentManagerBase.h"
#include "linklayer/base/LinkLayerBase.h"
#include "mobility/base/MobilityBase.h"

using namespace omnetpp;

class GlobalRadioRegister;

enum RadioMode { OFF, SLEEP, RX, TX };

class RadioBase : virtual public BaseModule {
   protected:
    // STATE

    int myMACAddress;
    int myBroadcastDomain;

    double txPowMW;
    double txGainMW;
    double rxGainMW;
    double pathLossMW;
    double thermalNoiseMW;
    double rssiDBM_Threshold;
    double snrDBM_Threshold;
    double carrierFrequency;
    double bandwidth;
    double bitrate;
    double propagationSpeed;

    // energy consumption
    double offPowerMW;
    double sleepPowerMW;
    double rxPowerMW;
    double txPowerMW;

    // current mode of the radio
    RadioMode currentMode;

    // packet currently received
    RadioPkt *ingoingPacketBuffer;
    double usefulSignalPower;
    double noisePowerMW;
    bool isFailedCollision;

    // radio seq number
    int nextSeq;

    // retransmission utility
    bool rxAfterTransmission = false;

    // modules
    LinkLayerBase *linkLayer;
    EnvironmentManagerBase *environmentManager;
    GlobalRadioRegister *radioRegister;

    // BEEP
    cMessage *receptionEndBeep;

    // METHODS
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void checkPacketDelivery();
    virtual void handleLowerPacket(RadioPkt *pkt);
    void handleUpperPacket(RadioPkt *pkt);

    double sinr(double powerReceived, double sumInterference, double noisePower);
    double powerReceivedMW(double txPower, double distance);
    double bitErrorRate(double sinr);
    double packetErrorRate(double bitErrorRate, int packetSizeBits);
    bool isReceptionSuccessful(double packetErrorRate);

   public:
    // METHODS
    void setRadioMode(RadioMode status);
    void setCarrierFrequency(long long frequency);
    void setBandwidth(double band);
    void setBroadcastDomain(int domain);

    // retransmission utility
    void enableRxAfterTransmission() { this->rxAfterTransmission = true; }
};

#endif
