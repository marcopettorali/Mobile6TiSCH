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

#include "medium.h"

Define_Module(Medium);

void Medium::initialize() {
    bitrate = par("bitrate");
    propagationSpeed = par("propagationSpeed");

    for (int i = 0; i < 5; ++i) {
        char signalName[32];
        sprintf(signalName, "outcome%d", i);
        simsignal_t signal = registerSignal(signalName);
        cProperty *statisticTemplate = getProperties()->get("statisticTemplate", "stats_outcome");
        getEnvir()->addResultRecorders(this, signal, signalName, statisticTemplate);
        messageReceptionOutcome[i] = signal;
    }
}

void Medium::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg->isName("message_beep")) {
        IntArray *control = check_and_cast<IntArray *>(msg->removeControlInfo());

        int macAddress = control->getData(0);
        int messageId = control->getData(1);

        for (int i = 0; i < messages.size(); i++) {
            if (messages[i]->macAddress == macAddress && messages[i]->messageId == messageId) {
                if (messages[i]->failures.size() > 0) {
                    int minOutcome = messages[i]->failures[0];
                    for (int j = 1; j < messages[i]->failures.size(); j++) {
                        if (messages[i]->failures[j] < minOutcome) {
                            minOutcome = messages[i]->failures[j];
                        }
                    }
                    emit(messageReceptionOutcome[minOutcome], 1);
                    if (minOutcome != 0) {
                        EV << "FAILED: " << messageId << " " << msg->getKind() << " " << minOutcome << endl;
                    }
                } else {
                    if (macAddress == -4)
                        throw cRuntimeError("This downstream packet was not received! Go back to around %f",
                                      simTime().dbl() - MESSAGE_TIMEOUT_S);
                    emit(messageReceptionOutcome[4], 1);
                    EV << "!!!4!!! FAILED: " << macAddress<<" " << messageId << " " << msg->getKind() << " " << 4 << endl;
                }

                delete control;
                cancelAndDelete(messages[i]->beep);
                delete messages[i];
                messages.erase(messages.begin() + i);
                return;
            }
        }
    }
}

void Medium::registerRadio(cModule *radio) { radios.push_back(radio); }

vector<cModule *> Medium::getRadios() { return radios; }

void Medium::notifyMessageSent(int macAddress, int messageId, int dest, int kind) {
    Enter_Method_Silent("notifyMessageSent(%d,%d)", macAddress, messageId);

    MessageInfo *m = new MessageInfo(macAddress, messageId, dest);
    messages.push_back(m);
    m->beep = new cMessage("message_beep");
    m->beep->setKind(kind);
    IntArray *control = new IntArray();
    control->setDataArraySize(2);
    control->setData(0, macAddress);
    control->setData(1, messageId);
    m->beep->setControlInfo(control);
    scheduleAt(simTime() + MESSAGE_TIMEOUT_S, m->beep);
}

void Medium::notifyMessageReceived(int macAddress, int messageId, int outcome) {
    Enter_Method_Silent("notifyMessageReceived(%d,%d,%d)", macAddress, messageId, outcome);

    for (auto i = 0; i < messages.size(); i++) {
        if (messages[i]->macAddress == macAddress && messages[i]->messageId == messageId) {
            messages[i]->failures.push_back(outcome);
            return;
        }
    }
}
