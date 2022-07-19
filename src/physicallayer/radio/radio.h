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

#ifndef __MOBILE6TISCH_LITE_RADIO_H_
#define __MOBILE6TISCH_LITE_RADIO_H_

#include <omnetpp.h>


#include "../../tsch/tschpkt/tschpkt_m.h"
#include "../medium/medium.h"
#include "../mobility/mobility.h"
#include "packetcontrolinfo/packetcontrolinfo_m.h"
#include "radiopkt/radiopkt_m.h"

#define RSSI_FSPL 0

using namespace omnetpp;

enum RadioMode { OFF, SLEEP, RX, TX };

class Radio : public cSimpleModule {
    int macAddress;
    int broadcastDomain;

    Medium *medium;
    Mobility *mobility;

    RadioPkt *buffer;
    cMessage *receptionEnd_beep;
    double signalPower;
    double noisePower;
    bool frequencyCollision;

    int nextMessageId;

    RadioMode mode;

    double carrierFrequency;
    double bandwidth;

    double txPowMW;
    double txGainMW;
    double rxGainMW;
    double pathLossMW;
    double thermalNoiseMW;

    double rssiThreshold;
    double snrThreshold;

    //energy consumption
    
    double offPowerMW;
    double sleepPowerMW;
    double rxPowerMW;
    double txPowerMW;

    simsignal_t averagePowerConsumptionSignal;

    void checkPacketDelivery(cMessage *msg);
    void handlePacketLower(cMessage *msg);
    void handlePacketUpper(cMessage *msg);

    void signalToMediumManager(int srcMacAddress, int messageId, int code);

    double distance(double x, double y);
    double computeRSSIMW(double txPowMW, double txGainMW, double dis, double frq);

    double sinr(double powerReceived, double sumInterference, double noisePower);
    double powerReceived(double txPower, double distance);
    double bitErrorRate(double sinr);
    double packetErrorRate(double bitErrorRate, int packetSizeBits);
    bool isReceptionSuccessful(double packetErrorRate);

    virtual int numInitStages() const { return 5; }

   protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

   public:
    void setRadioMode(RadioMode status);
    void setCarrierFrequency(long long frequency);
    void setBandwidth(double band);
    void setBroadcastDomain(int domain);
};

#endif
