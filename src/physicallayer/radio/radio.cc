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

#include "radio.h"

#include <cmath>
#include <cstring>
#include <iomanip>
#include <vector>
using namespace std;

Define_Module(Radio);

// UTILITY

#define E 2.718281828

int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

double n_choose_k(int n, int k) {
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
        macAddress = par("macAddress");
        broadcastDomain = -1;

        mode = RadioMode::OFF;

        carrierFrequency = par("carrierFrequency");
        bandwidth = par("bandwidth");
        txPowMW = par("transmitterPowerMW");
        txGainMW = par("transmitterGainMW");
        rxGainMW = par("receiverGainMW");
        pathLossMW = par("pathLossMW");
        rssiThreshold = par("rssiThreshold");
        snrThreshold = toMw(par("snrThreshold"));
        thermalNoiseMW = toMw(par("thermalNoise"));

        offPowerMW = par("offPowerMW");
        sleepPowerMW = par("sleepPowerMW");
        rxPowerMW = par("rxPowerMW");
        txPowerMW = par("txPowerMW");

        averagePowerConsumptionSignal = registerSignal("averagePowerConsumption");

        medium = check_and_cast<Medium *>(getModuleByPath(par("medium")));
        medium->registerRadio(this);

        mobility = check_and_cast<Mobility *>(getModuleByPath(par("mobility")));

        buffer = NULL;
        noisePower = thermalNoiseMW;
        receptionEnd_beep = new cMessage("receptionEnd_beep");
        frequencyCollision = false;

        nextMessageId = 0;
    }
}

void Radio::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && strcmp(msg->getName(), "receptionEnd_beep") == 0) {
        checkPacketDelivery(msg);

    } else if (msg->getArrivalGate() == gate("radioIn")) {  // from the network
        handlePacketLower(msg);

    } else if (msg->getArrivalGate() == gate("upperLayerIn")) {  // from upper layer
        handlePacketUpper(msg);
    }
}

void Radio::checkPacketDelivery(cMessage *msg) {
    PacketControlInfo *info = check_and_cast<PacketControlInfo *>(buffer->removeControlInfo());
    int srcMacAddress = info->getSrcMacAddress();
    int messageId = info->getMessageId();
    double sinr = info->getRssi() / noisePower;

    double ber = bitErrorRate(sinr);
    double per = packetErrorRate(ber, info->getBitLength());
    double isSuccessful = isReceptionSuccessful(per);

    EV << "Noise " << thermalNoiseMW << " " << noisePower << endl;
    EV << "INFORSSI " << info->getRssi() << " " << sinr << " " << ber << " " << per << " "
       << (isSuccessful ? "true" : "false") << endl;

    if (sinr < snrThreshold || !isSuccessful) {
        signalToMediumManager(srcMacAddress, messageId, 1);
        delete info;
        EV << macAddress << ": Packet " << messageId << " lost due to low SNR" << endl;
    } else {
        if (buffer->getDest() == macAddress || buffer->getDest() == broadcastDomain) {
            TSCHPkt *up = (TSCHPkt *)buffer->decapsulate();
            up->setControlInfo(info);
            send(up, "upperLayerOut");

            signalToMediumManager(srcMacAddress, messageId, 0);
            EV << macAddress << ": Packet delivered to upper layer" << endl;
        } else {
            delete info;
            EV << macAddress << ": Packet " << messageId << " lost due to wrong destination" << endl;
        }
    }
    noisePower = thermalNoiseMW;
    delete buffer;

    buffer = NULL;
    frequencyCollision = false;
}

void Radio::handlePacketLower(cMessage *msg) {
    EV << "INSIDE" << endl;
    RadioPkt *pkt = check_and_cast<RadioPkt *>(msg);
    PacketControlInfo *info = check_and_cast<PacketControlInfo *>(pkt->removeControlInfo());
    int srcMacAddress = info->getSrcMacAddress();
    int messageId = info->getMessageId();

    if (mode != RadioMode::RX) {
        delete info;
        delete msg;
        return;
    }

    double dis = distance(pkt->getPositionX(), pkt->getPositionY());
    double rssi = powerReceived(pkt->getTransmitterPowerMW(), dis);
    double rssiDBM = toDbm(rssi);

    if (rssiDBM < rssiThreshold) {
        signalToMediumManager(srcMacAddress, messageId, 3);
        delete info;
        delete pkt;
        EV << macAddress << ": Received packet with RSSI " << rssiDBM << " dBm, less than threshold " << rssiThreshold
           << " dBm, discarding packet." << endl;

    } else if (bandwidth != pkt->getBandwidth() || carrierFrequency != pkt->getCarrierFrequency()) {
        delete info;
        delete pkt;
        EV << macAddress << ": Received packet with different bandwidth or carrier frequency, discarding packet."
           << endl;

    } else if (buffer != NULL) {
        noisePower += rssi;
        signalToMediumManager(srcMacAddress, messageId, 2);
        delete info;
        delete pkt;
        EV << macAddress << ": Received packet with RSSI " << rssiDBM << " dBm, more than threshold " << rssiThreshold
           << " dBm, but already receiving another packet, discarding packet." << endl;

    } else {
        double transmissionTime = pkt->getBitLength() / medium->bitrate;
        double propagationTime = dis / medium->propagationSpeed;
        double delay = transmissionTime + propagationTime;

        PacketControlInfo *newInfo = new PacketControlInfo();
        newInfo->setDelay(delay);
        newInfo->setRssi(rssi);
        newInfo->setMessageId(messageId);
        newInfo->setSrcMacAddress(srcMacAddress);
        newInfo->setBitLength(pkt->getBitLength());
        pkt->setControlInfo(newInfo);

        buffer = pkt;
        scheduleAt(simTime() + delay, receptionEnd_beep);

        delete info;
        EV << macAddress << ": Received packet with RSSI " << rssiDBM << " dBm, more than threshold " << rssiThreshold
           << " dBm, starting reception." << endl;
    }
}

void Radio::handlePacketUpper(cMessage *msg) {
    if (mode == RadioMode::TX) {
        try {
            TSCHPkt *m = check_and_cast<TSCHPkt *>(msg);

            RadioPkt *pkt = new RadioPkt(m->getName());
            pkt->encapsulate(m);
            pkt->setSrc(m->getSrc());
            pkt->setDest(m->getDest());

            pkt->setPositionX(mobility->currentX);
            pkt->setPositionY(mobility->currentY);
            pkt->setCarrierFrequency(carrierFrequency);
            pkt->setBandwidth(bandwidth);
            pkt->setTransmitterPowerMW(txPowMW);
            pkt->setTransmitterGainMW(txGainMW);

            PacketControlInfo *info = new PacketControlInfo("packetInfo");
            info->setSrcMacAddress(macAddress);
            info->setMessageId(nextMessageId++);

            medium->notifyMessageSent(info->getSrcMacAddress(), info->getMessageId(), pkt->getDest(), m->getKind());

            vector<cModule *> radios = medium->getRadios();
            for (auto i = 0; i < radios.size(); i++) {
                if (radios.at(i) == this) {
                    continue;
                }
                cPacket *pa = pkt->dup();
                pa->setControlInfo(info->dup());
                sendDirect(pa, radios.at(i)->getParentModule(), "radioIn");
            }

            // switch to receiver mode to wait for the ACK
            if (m->getKind() != 3) {  // TSCHPKT_ACK
                setRadioMode(RadioMode::RX);
            }

            delete pkt;
            delete info;
        } catch (const cException &e) {
            delete msg;
        }

    } else {
        delete msg;
    }
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
    }else{
        deltaPower = 0;
    }

    emit(averagePowerConsumptionSignal, deltaPower);
    mode = status; 
}

void Radio::setCarrierFrequency(long long frequency) { carrierFrequency = frequency; }

void Radio::setBandwidth(double band) { bandwidth = band; }

void Radio::setBroadcastDomain(int domain) { broadcastDomain = domain; }

void Radio::signalToMediumManager(int srcMacAddress, int messageId, int code) {
    if (buffer && (buffer->getDest() == macAddress || buffer->getDest() == broadcastDomain)) {
        medium->notifyMessageReceived(srcMacAddress, messageId, code);
    }
}

double Radio::distance(double x, double y) {
    return sqrt(pow((mobility->currentX - x), 2) + pow((mobility->currentY - y), 2));
}

double Radio::computeRSSIMW(double txPowMW, double txGainMW, double dis,
                            double frq) {  // https://www.mdpi.com/2077-1312/7/9/290/pdf
    return (txPowMW * txGainMW * rxGainMW * pow((double)299792458 / frq, 2)) / (pathLossMW * pow(4 * M_PI * dis, 2));
}

double Radio::powerReceived(double txPower, double distance) {
    // https://www.researchgate.net/publication/3159058_Wideband_Indoor_Channel_Measurements_and_BER_Analysis_of_Frequency_Selective_Multipath_Channels_at_24_475_and_115_GHz
    double x = normal(0, 3.6);
    double gain = 1 / (pow(10, 3.8) * pow(distance, 3.3) * pow(10, x/10));
    return txPower * (gain > 1 ? 1 : gain);
}

double Radio::bitErrorRate(double sinr) {
    double sum = 0;
    for (int k = 2; k <= 16; k++) {
        EV << "BIN " << pow(-1, k) << " " << n_choose_k(16, k) << " " << pow(E, 20 * sinr * ((1.0 / k) - 1)) << endl;
        sum += pow(-1, k) * n_choose_k(16, k) * pow(E, 20 * sinr * ((1.0 / k) - 1.0));
    }
    return sum / 30.0;
}

double Radio::packetErrorRate(double bitErrorRate, int packetSizeBits) {
    return 1 - pow(1 - bitErrorRate, packetSizeBits);
}

bool Radio::isReceptionSuccessful(double packetErrorRate) {
    double random = uniform(0, 1);
    return random > packetErrorRate;
}
