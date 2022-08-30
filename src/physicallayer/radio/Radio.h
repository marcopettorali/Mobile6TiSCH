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
#include "SignalDescriptor_m.h"
#include "RadioPkt_m.h"
#include "../mobility/MobilityBase.h"

enum RadioMode { OFF, SLEEP, RX, TX };

using namespace omnetpp;

#define KIND_NOTIFY_PACKET_SENT -1

class Radio : public cSimpleModule {
    int myMACAddress;
    int myBroadcastDomain;

    double txPowMW;
    double txGainMW;
    double rxGainMW;
    double pathLossMW;
    double thermalNoiseMW;
    double rssiThreshold;
    double snrThreshold;
    double carrierFrequency;
    double bandwidth;
    double bitrate;
    double propagationSpeed;

    // energy consumption
    double offPowerMW;
    double sleepPowerMW;
    double rxPowerMW;
    double txPowerMW;

    RadioMode currentMode;

    MobilityBase *mobility;
    GlobalRadioRegister *radioRegister;

    RadioPkt *ingoingPacketBuffer;
    cMessage *receptionEnd_beep;
    double usefulSignalPower;
    double noisePower;
    bool isFailedCollision;

    int nextSeq;

    simsignal_t averagePowerConsumptionSignal;

    void checkPacketDelivery();
    void handleLowerPacket(RadioPkt *pkt);
    void handleUpperPacket(RadioPkt *pkt);

    //void signalToRadioRegister(int srcMacAddress, int messageId, int code);

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
