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

#include "Radio.h"

#include <cmath>
#include <cstring>
#include <iomanip>
#include <vector>

Define_Module(Radio);

// UTILITY

#define MATH_E 2.718281828

double nChooseK(int n, int k) {
    if (n < k) return 0.0;

    const int iK = (k << 1) > n ? n - k : k;
    const double dNSubK = (n - iK);
    int i = 1;
    double dRes = i > iK ? 1.0 : (dNSubK + i);

    for (++i; i <= iK; ++i) {
        dRes *= dNSubK + i;
        dRes /= i;
    }
    return dRes;
}

double toMw(double power) { return pow(10, power / 10); }

double toDbm(double power) { return 10 * log10(power); }

// END UTILITY

void Radio::initialize(int stage) {
    if (stage == 0) {  // Initialize variables
        myMACAddress = par("macAddress");
        myBroadcastDomain = -1;

        currentMode = RadioMode::OFF;

        carrierFrequency = par("carrierFrequency");
        bandwidth = par("bandwidth");
        txPowMW = par("transmitterPowerMW");
        txGainMW = par("transmitterGainMW");
        rxGainMW = par("receiverGainMW");
        pathLossMW = par("pathLossMW");
        rssiThreshold = par("rssiThreshold");
        snrThreshold = toMw(par("snrThreshold"));
        thermalNoiseMW = toMw(par("thermalNoise"));
        bitrate = par("bitrate");
        propagationSpeed = par("propagationSpeed");

        offPowerMW = par("offPowerMW");
        sleepPowerMW = par("sleepPowerMW");
        rxPowerMW = par("rxPowerMW");
        txPowerMW = par("txPowerMW");

        mobility = check_and_cast<MobilityBase *>(findModuleByPath(par("mobility")));
        radioRegister = check_and_cast<GlobalRadioRegister *>(findModuleByPath(par("radioRegister")));
        radioRegister->registerRadio(this);

        averagePowerConsumptionSignal = registerSignal("averagePowerConsumption");

        // packet reception utility
        ingoingPacketBuffer = NULL;
        noisePower = thermalNoiseMW;
        receptionEnd_beep = new cMessage("receptionEnd_beep");
        isFailedCollision = false;

        nextSeq = 0;
    }
}

void Radio::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && strcmp(msg->getName(), "receptionEnd_beep") == 0) {
        checkPacketDelivery();

    } else if (msg->getArrivalGate() == gate("radioIn")) {  // from the wireless radioRegister
        RadioPkt *pkt = check_and_cast<RadioPkt *>(msg);
        handleLowerPacket(pkt);

    } else if (msg->getArrivalGate() == gate("upperLayerIn")) {  // from upper layer
        RadioPkt *pkt = check_and_cast<RadioPkt *>(msg);
        handleUpperPacket(pkt);
    }
}

void Radio::handleLowerPacket(RadioPkt *pkt) {
    SignalDescriptor *desc = check_and_cast<SignalDescriptor *>(pkt->removeControlInfo());
    int idSrc = desc->getIdSrc();
    int idSeq = desc->getIdSeq();

    if (currentMode != RadioMode::RX) {
        EV << myMACAddress << ": Not in receiving mode" << endl;
        delete desc;
        delete pkt;
        return;
    }

    double dis = DIST(mobility->currentX, mobility->currentY, desc->getPositionX(), desc->getPositionY());
    double rssi = powerReceived(desc->getTransmitterPowerMW(), dis);
    double rssiDBM = toDbm(rssi);

    if (rssiDBM < rssiThreshold) {
        // signalToRadioRegister(srcMacAddress, messageId, 3);
        delete desc;
        delete pkt;
        EV << myMACAddress << ": Received packet with RSSI " << rssiDBM << " dBm, less than threshold " << rssiThreshold << " dBm, discarding packet." << endl;

    } else if (bandwidth != desc->getBandwidth() || carrierFrequency != desc->getCarrierFrequency()) {
        delete desc;
        delete pkt;
        EV << myMACAddress << ": Received packet with different bandwidth or carrier frequency, discarding packet." << endl;

    } else if (ingoingPacketBuffer != NULL) {
        noisePower += rssi;
        // signalToradioRegisterManager(srcMacAddress, messageId, 2);
        delete desc;
        delete pkt;
        EV << myMACAddress << ": Received packet with RSSI " << rssiDBM << " dBm, more than threshold " << rssiThreshold << " dBm, but already receiving another packet, discarding packet." << endl;

    } else {
        double transmissionTime = pkt->getBitLength() / bitrate;
        double propagationTime = dis / propagationSpeed;
        double delay = transmissionTime + propagationTime;

        desc->setRssi(rssi);

        ingoingPacketBuffer = pkt;
        pkt->setControlInfo(desc);
        scheduleAt(simTime() + delay, receptionEnd_beep);
        EV << myMACAddress << ": Received packet with RSSI " << rssiDBM << " dBm, more than threshold " << rssiThreshold << " dBm, starting reception." << endl;
    }
}

void Radio::handleUpperPacket(RadioPkt *pkt) {
    if (currentMode != RadioMode::TX) {
        delete pkt;
    }

    try {
        SignalDescriptor *desc = new SignalDescriptor("desc");
        desc->setIdSrc(myMACAddress);
        desc->setIdSeq(nextSeq++);
        desc->setPositionX(mobility->currentX);
        desc->setPositionY(mobility->currentY);
        desc->setCarrierFrequency(carrierFrequency);
        desc->setBandwidth(bandwidth);
        desc->setTransmitterPowerMW(txPowMW);
        desc->setTransmitterGainMW(txGainMW);

        std::vector<cModule *> radios = radioRegister->getRadios();
        for (auto i = 0; i < radios.size(); i++) {
            if (radios.at(i) == this) {
                continue;
            }
            cPacket *pa = pkt->dup();
            pa->setControlInfo(desc->dup());
            sendDirect(pa, radios.at(i)->getParentModule(), "radioIn");
        }

        // notify upper layer of correct sending
        cMessage * notify = new cMessage("notifyPacketSent");
        notify->setKind(KIND_NOTIFY_PACKET_SENT);
        send(notify, "upperLayerOut");

        delete desc;
        delete pkt;
    } catch (const cException &e) {
        delete pkt;
    }
}

void Radio::checkPacketDelivery() {
    SignalDescriptor *desc = check_and_cast<SignalDescriptor*>(ingoingPacketBuffer->removeControlInfo());

    int idSeq = desc->getIdSeq();
    double sinr = desc->getRssi() / noisePower;

    double ber = bitErrorRate(sinr);
    double per = packetErrorRate(ber, desc->getBitLength());
    double isSuccessful = isReceptionSuccessful(per);

    if (sinr < snrThreshold || !isSuccessful) {
        //signalToRadioRegister(idSrc, idSeq, 1);
        delete desc;
        EV << myMACAddress << ": Packet " << idSeq << " lost due to low SNR" << endl;
    } else {
        if (ingoingPacketBuffer->getDest() == myMACAddress || ingoingPacketBuffer->getDest() == myBroadcastDomain) {
            cPacket *up = ingoingPacketBuffer->decapsulate();
            up->setControlInfo(desc);
            send(up, "upperLayerOut");
            //signalToRadioRegister(idSrc, idSeq, 0);
            EV << myMACAddress << ": Packet delivered to upper layer" << endl;
        } else {
            delete desc;
            EV << myMACAddress << ": Packet " << idSeq << " lost due to wrong destination: " << ingoingPacketBuffer->getDest() << endl;
        }
    }
    noisePower = thermalNoiseMW;
    delete ingoingPacketBuffer;

    ingoingPacketBuffer = NULL;
    isFailedCollision = false;
}

void Radio::setRadioMode(RadioMode status) {
    // choose deltaPower according to the status of the radio
    double deltaPower;
    if (status == RadioMode::TX) {
        deltaPower = txPowerMW;
    } else if (status == RadioMode::RX) {
        deltaPower = rxPowerMW;
    } else if (status == RadioMode::SLEEP) {
        deltaPower = sleepPowerMW;
    } else {
        deltaPower = 0;
    }

    emit(averagePowerConsumptionSignal, deltaPower);
    currentMode = status;
}

void Radio::setCarrierFrequency(long long frequency) { carrierFrequency = frequency; }

void Radio::setBandwidth(double band) { bandwidth = band; }

void Radio::setBroadcastDomain(int domain) { myBroadcastDomain = domain; }


double Radio::computeRSSIMW(double txPowMW, double txGainMW, double dis,
                            double frq) {  // https://www.mdpi.com/2077-1312/7/9/290/pdf
    return (txPowMW * txGainMW * rxGainMW * pow((double)299792458 / frq, 2)) / (pathLossMW * pow(4 * M_PI * dis, 2));
}

double Radio::powerReceived(double txPower, double distance) {
    // https://www.researchgate.net/publication/3159058_Wideband_Indoor_Channel_Measurements_and_BER_Analysis_of_Frequency_Selective_Multipath_Channels_at_24_475_and_115_GHz
    double x = normal(0, 3.6, 4);
    double gain = 1 / (pow(10, 3.8) * pow(distance, 3.3) * pow(10, x / 10));
    return txPower * (gain > 1 ? 1 : gain);
}

double Radio::bitErrorRate(double sinr) {
    double sum = 0;
    for (int k = 2; k <= 16; k++) {
        sum += pow(-1, k) * nChooseK(16, k) * pow(MATH_E, 20 * sinr * ((1.0 / k) - 1.0));
    }
    return sum / 30.0;
}

double Radio::packetErrorRate(double bitErrorRate, int packetSizeBits) { return 1 - pow(1 - bitErrorRate, packetSizeBits); }

bool Radio::isReceptionSuccessful(double packetErrorRate) {
    double random = uniform(0, 1, 3);
    return random > packetErrorRate;
}
