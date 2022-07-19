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

#ifndef __MOBILE6TISCH_LITE_MEDIUM_H_
#define __MOBILE6TISCH_LITE_MEDIUM_H_

#include <omnetpp.h>
#include <vector>
#include "intarray/IntArray_m.h"

#define MESSAGE_TIMEOUT_S	60

using namespace omnetpp;
using namespace std;

struct MessageInfo {
	int macAddress;
	int messageId;
	int destMacAddress;
	cMessage *beep;

	vector<int> failures;
	MessageInfo(int addr, int msg, int dest) {
		macAddress = addr;
		messageId = msg;
		destMacAddress = dest;
	}
};

class Medium: public cSimpleModule {
	vector<cModule*> radios;
	vector<MessageInfo*> messages;

	simsignal_t messageReceptionOutcome[5];

protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
public:
	double bitrate;
	double propagationSpeed;

	void registerRadio(cModule *radio);
	vector<cModule*> getRadios();

	void notifyMessageSent(int macAddress, int messageId, int dest, int kind);
	void notifyMessageReceived(int macAddress, int messageId, int outcome);
};

#endif
