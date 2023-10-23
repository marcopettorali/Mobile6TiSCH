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

#include "RadioBase.h"

#include <cmath>
#include <cstring>
#include <iomanip>
#include <vector>

Define_Module(RadioBase);

void RadioBase::initialize(int stage) {
    BaseModule::initialize(stage);
    if (stage == INITSTAGE_PARAMS_FETCH) {
        // Fetch parameters
        myMACAddress = par("macAddress");
        carrierFrequency = par("carrierFrequency");
        bandwidth = par("bandwidth");
        txPowMW = par("transmitterPowerMW");
        txGainMW = par("transmitterGainMW");
        rxGainMW = par("receiverGainMW");
        pathLossMW = par("pathLossMW");
        rssiDBM_Threshold = par("rssiDBM_Threshold");
        snrDBM_Threshold = toMW(par("snrDBM_Threshold"));
        thermalNoiseMW = toMW(par("thermalNoise"));
        bitrate = par("bitrate");
        propagationSpeed = par("propagationSpeed");
        offPowerMW = par("offPowerMW");
        sleepPowerMW = par("sleepPowerMW");
        rxPowerMW = par("rxPowerMW");
        txPowerMW = par("txPowerMW");

        // set initial state
        currentMode = RadioMode::OFF;

        crossLayerDB->set("macAddress", myMACAddress);
        crossLayerDB->set("currentMode", currentMode);

        // packet reception utility
        ingoingPacketBuffer = NULL;
        noisePowerMW = thermalNoiseMW;
        isFailedCollision = false;
        nextSeq = 0;
    }

    if (stage == INITSTAGE_BEEP_INIT) {
        receptionEndBeep = new cMessage("receptionEndBeep");
    }

    if (stage == INITSTAGE_MODULES_FETCH) {
        myBroadcastDomain = crossLayerDB->getInt("broadcastAddress");

        radioRegister = check_and_cast<GlobalRadioRegister *>(findModuleByPath(par("radioRegister")));
        radioRegister->registerRadio(this);
        linkLayer = check_and_cast<LinkLayerBase *>(findModuleByPath(par("linkLayer")));
        environmentManager = check_and_cast<EnvironmentManagerBase *>(findModuleByPath(par("environmentManager")));
    }
}

void RadioBase::finish() { cancelAndDelete(receptionEndBeep); }

void RadioBase::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && strcmp(msg->getName(), "receptionEndBeep") == 0) {
        checkPacketDelivery();

    } else if (msg->getArrivalGate() == gate("radioIn")) {  // from the wireless channel
        RadioPkt *pkt = check_and_cast<RadioPkt *>(msg);
        handleLowerPacket(pkt);

    } else if (msg->getArrivalGate() == gate("upperLayerIn")) {  // from upper layer
        RadioPkt *pkt = check_and_cast<RadioPkt *>(msg);
        handleUpperPacket(pkt);
    }
}

void RadioBase::handleLowerPacket(RadioPkt *pkt) {
    DBG(myMACAddress) << "Received packet from lower layer" << endl;
    SignalDescriptor *desc = check_and_cast<SignalDescriptor *>(pkt->removeControlInfo());

    if (currentMode != RadioMode::RX) {
        WRN(myMACAddress) << "Radio is not in RX mode, discarding packet" << endl;
        delete desc;
        delete pkt;
        return;
    }

    Point currentPosition = Point(crossLayerDB->getDouble("currentX"), crossLayerDB->getDouble("currentY"));
    Point senderPosition = Point(desc->getPositionX(), desc->getPositionY());

    double dis = currentPosition.distance(senderPosition);
    double rssiMW = powerReceivedMW(desc->getTransmitterPowerMW(), dis);
    double rssiDBM = toDBM(rssiMW);

    // check if LoS between the radio and the sender intersects an obstacle
    if (environmentManager->isIntersecting(Segment(currentPosition, senderPosition))) {
        WRN(myMACAddress) << "Received packet from " << pkt->getSrc() << " but LoS is obstructed, discarding packet" << endl;
        radioRegister->registerPacketReceived(pkt->getId(), FAIL_LOS_OBSTRUCTION);
        delete desc;
        delete pkt;
        return;
    } else if (bandwidth != desc->getBandwidth() || carrierFrequency != desc->getCarrierFrequency()) {
        WRN(myMACAddress) << "Received packet with different bandwidth or carrier frequency (" << carrierFrequency << " "
                          << desc->getCarrierFrequency() << "), discarding packet." << endl;
        // TODO linkLayer->onPacketLost(pkt->getId(), pkt->getSrc(), pktX, pktY, 2);
        radioRegister->registerPacketReceived(pkt->getId(), FAIL_RECEPTION_CONFLICT);
        delete desc;
        delete pkt;

    } else if (rssiDBM < rssiDBM_Threshold) {
        // TODO linkLayer->onPacketLost(pkt->getId(), pkt->getSrc(), pktX, pktY, 1);
        radioRegister->registerPacketReceived(pkt->getId(), FAIL_RSSI_THRESHOLD);
        delete desc;
        delete pkt;
        WRN(myMACAddress) << "Received packet with RSSI " << rssiDBM << " dBm, less than threshold " << rssiDBM_Threshold
                          << " dBm, discarding packet." << endl;

    } else if (ingoingPacketBuffer != NULL) {
        noisePowerMW += rssiMW;
        // TODO linkLayer->onPacketLost(pkt->getId(), pkt->getSrc(), pktX, pktY);
        radioRegister->registerPacketReceived(pkt->getId(), FAIL_COLLISION);
        delete desc;
        delete pkt;
        WRN(myMACAddress) << "Received packet with RSSI " << rssiDBM << " dBm, more than threshold " << rssiDBM_Threshold
                          << " dBm, but already receiving another packet, discarding packet." << endl;

    } else {
        double transmissionTime = pkt->getBitLength() / bitrate;
        double propagationTime = dis / propagationSpeed;
        double delay = transmissionTime + propagationTime;

        desc->setRssiMW(rssiMW);

        ingoingPacketBuffer = pkt;
        pkt->setControlInfo(desc);
        scheduleAt(simTime() + delay, receptionEndBeep);
        INF(myMACAddress) << "Received packet with RSSI " << rssiDBM << " dBm, more than threshold " << rssiDBM_Threshold
                          << " dBm, starting reception." << endl;
    }
}

void RadioBase::handleUpperPacket(RadioPkt *pkt) {
    DBG(myMACAddress) << "Received packet " << pkt->getName() << " from upper layer" << endl;
    if (currentMode != RadioMode::TX) {
        WRN(myMACAddress) << "Radio is not in TX mode, discarding packet" << endl;
        delete pkt;
        return;
    }

    try {
        SignalDescriptor *desc = new SignalDescriptor("desc");
        desc->setIdSrc(myMACAddress);
        desc->setIdSeq(nextSeq++);
        desc->setPositionX(crossLayerDB->getDouble("currentX"));
        desc->setPositionY(crossLayerDB->getDouble("currentY"));
        desc->setCarrierFrequency(carrierFrequency);
        desc->setBandwidth(bandwidth);
        desc->setTransmitterPowerMW(txPowMW);
        desc->setTransmitterGainMW(txGainMW);

        radioRegister->registerPacketSent(pkt->getId());

        std::vector<RadioBase *> radios = radioRegister->getRadios();
        for (auto i = 0; i < radios.size(); i++) {
            if (radios.at(i) == this || radios.at(i)->currentMode != RadioMode::RX) {
                continue;
            }
            cPacket *pa = pkt->dup();

            pa->setControlInfo(desc->dup());
            sendDirect(pa, radios.at(i)->getParentModule(), "radioIn");
        }

        delete desc;
        delete pkt;

        DBG(myMACAddress) << "Packet sent" << endl;

        // if rxAfterTransmission is true, the radio will be turned on after the transmission to receive the linklayer ACK
        if (rxAfterTransmission) {
            setRadioMode(RadioMode::RX);
            DBG(myMACAddress) << "Radio set to RX mode" << endl;
        } else {
            setRadioMode(RadioMode::SLEEP);
            DBG(myMACAddress) << "Radio set to SLEEP mode" << endl;
        }
    } catch (const cException &e) {
        delete pkt;
    }
}

void RadioBase::checkPacketDelivery() {
    SignalDescriptor *desc = check_and_cast<SignalDescriptor *>(ingoingPacketBuffer->removeControlInfo());

    // double pktX = desc->getPositionX();
    // double pktY = desc->getPositionY();

    double sinrDBM = toDBM(desc->getRssiMW() / noisePowerMW);

    double ber = bitErrorRate(sinrDBM);
    double per = packetErrorRate(ber, ingoingPacketBuffer->getBitLength());
    double isSuccessful = isReceptionSuccessful(per);

    if (sinrDBM < snrDBM_Threshold) {
        // TODO linkLayer->onPacketLost(ingoingPacketBuffer->getId(), ingoingPacketBuffer->getSrc(), pktX, pktY, 1);
        radioRegister->registerPacketReceived(ingoingPacketBuffer->getId(), FAIL_SINR_THRESHOLD);
        WRN(myMACAddress) << "Packet " << ingoingPacketBuffer->getName() << " lost due to low SNR: " << sinrDBM << " dBm vs " << snrDBM_Threshold
                          << " dBm" << endl;
    } else if (!isSuccessful) {
        // TODO linkLayer->onPacketLost(ingoingPacketBuffer->getId(), ingoingPacketBuffer->getSrc(), pktX, pktY, 1);
        radioRegister->registerPacketReceived(ingoingPacketBuffer->getId(), FAIL_CHANNEL_MODEL);
        WRN(myMACAddress) << "Packet " << ingoingPacketBuffer->getName() << " lost due to channel model" << endl;
    } else {
        if (ingoingPacketBuffer->getDest() == myMACAddress || ingoingPacketBuffer->getDest() == myBroadcastDomain) {
            cPacket *up = ingoingPacketBuffer->decapsulate();
            up->removeControlInfo();
            crossLayerDB->set("lastPacketReceivedRSSIdBm", toDBM(desc->getRssiMW()));
            DBG(myMACAddress) << "lastPacketReceivedRSSIdBm " << crossLayerDB->getDouble("lastPacketReceivedRSSIdBm") << endl;
            send(up, "upperLayerOut");
            // TODO linkLayer->onPacketReceived(ingoingPacketBuffer->getId(), ingoingPacketBuffer->getSrc(), pktX, pktY);
            radioRegister->registerPacketReceived(ingoingPacketBuffer->getId(), SUCCESS);
            INF(myMACAddress) << "Packet delivered to upper layer" << endl;
        } else {
            WRN(myMACAddress) << "Packet " << ingoingPacketBuffer->getName() << " lost due to wrong destination: " << ingoingPacketBuffer->getDest()
                              << ", myMACAddress: " << myMACAddress << ", myBroadcastDomain: " << myBroadcastDomain << endl;
        }
    }

    // delete packet
    delete desc;
    delete ingoingPacketBuffer;

    // reset noise power
    noisePowerMW = thermalNoiseMW;

    // reset buffer
    ingoingPacketBuffer = NULL;
    isFailedCollision = false;
}

void RadioBase::setRadioMode(RadioMode status) {
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

    // TODO remove this and move to upper layers
    if (myBroadcastDomain == -3) {
        statsCollector->record(myMACAddress, "averagePowerConsumptionMN", deltaPower);
    } else if (myBroadcastDomain == -4) {
        statsCollector->record(myMACAddress, "averagePowerConsumptionBR", deltaPower);
    }

    currentMode = status;
}

void RadioBase::setCarrierFrequency(long long frequency) { carrierFrequency = frequency; }

void RadioBase::setBandwidth(double band) { bandwidth = band; }

void RadioBase::setBroadcastDomain(int domain) { myBroadcastDomain = domain; }

double RadioBase::powerReceivedMW(double txPowerMW, double distance) {
    // https://www.researchgate.net/publication/3159058_Wideband_Indoor_Channel_Measurements_and_BER_Analysis_of_Frequency_Selective_Multipath_Channels_at_24_475_and_115_GHz
    double x = normal(0, 3.6, 4);
    double gain = 1 / (toMW(37.7) * pow(distance, 3.3) * toMW(x));
    return txPowerMW * (gain > 1 ? 1 : gain);
}

double RadioBase::bitErrorRate(double sinr) {
    double sum = 0;
    for (int k = 2; k <= 16; k++) {
        sum += pow(-1, k) * nChooseK(16, k) * pow(M_E, 20 * sinr * ((1.0 / k) - 1.0));
    }
    return sum / 30.0;
}

double RadioBase::packetErrorRate(double bitErrorRate, int packetSizeBits) { return 1 - pow(1 - bitErrorRate, packetSizeBits); }

bool RadioBase::isReceptionSuccessful(double packetErrorRate) {
    double random = uniform(0, 1, RNG_RADIO_SUCCESS);
    return random > packetErrorRate;
}
