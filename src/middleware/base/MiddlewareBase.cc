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

#include "MiddlewareBase.h"

Define_Module(MiddlewareBase);

void MiddlewareBase::initialize(int stage) { BaseModule::initialize(stage); }

void MiddlewareBase::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        handleSelfMessage(msg);
    } else if (msg->arrivedOn("upperLayerIn")) {
        handleUpperPacket(check_and_cast<cPacket *>(msg));
    } else if (msg->arrivedOn("lowerLayerIn")) {
        handleLowerPacket(check_and_cast<cPacket *>(msg));
    }else{
        BaseModule::handleMessage(msg);
    }
}