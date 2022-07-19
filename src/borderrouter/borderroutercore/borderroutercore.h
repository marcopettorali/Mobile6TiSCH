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

#ifndef __MOBILE6TISCH_LITE_BORDERROUTERCORE_H_
#define __MOBILE6TISCH_LITE_BORDERROUTERCORE_H_

#include <omnetpp.h>

#include "../../applicationpkts/downstreampkt_m.h"
#include "../../applicationpkts/upstreampkt_m.h"
#include "../../networkcoordinator/networkcoordinator.h"
#include "../../tsch/tsch.h"

class NetworkCoordinator;
class TSCH;
class Mobility;

using namespace omnetpp;
using namespace std;

/**
 * TODO - Generated class
 */
class BorderRouterCore : public cSimpleModule {
  NetworkCoordinator *coordinator;
  TSCH *tsch;
  Mobility *mobility;

  void handleDownstreamPacket(cMessage *msg);
  void handleUpstreamPacket(cMessage *msg);
  virtual int numInitStages() const { return 5; }

 protected:
  virtual void initialize(int stage);
  virtual void handleMessage(cMessage *msg);

 public:
  int macAddress;
  void updateSchedule(vector<int> targets);
};

#endif
