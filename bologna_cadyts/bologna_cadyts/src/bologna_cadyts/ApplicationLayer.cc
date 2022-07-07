//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "ApplicationLayer.h"

#include "bologna_cadyts/ApplicationLayerMessage_m.h"

using namespace veins;
using namespace bologna_cadyts;

Define_Module(bologna_cadyts::ApplicationLayer);

void ApplicationLayer::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        lastSentMessage = simTime();
        lastUpdatedInformation = simTime();
        alpha = 0.40;
        receivedMessage = false;
        receivedSignal = registerSignal("received");
    }
}

void ApplicationLayer::onWSM(BaseFrame1609_4* frame)
{
    EV << "Frame received" << endl;
    receivedMessage = true;
}

void ApplicationLayer::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);

    if(simTime() - lastSentMessage >= 10){
        if(uniform(0.0, 1.0) < alpha){
            ApplicationLayerTestMessage* wsm = new ApplicationLayerTestMessage();
            populateWSM(wsm);
            wsm->setData(std::to_string(getParentModule()->getIndex()).c_str());
            sendDown(wsm);
        }

        lastSentMessage = simTime();
    }

    if(simTime() - lastUpdatedInformation >= 5){
        emit(receivedSignal, receivedMessage);
        lastUpdatedInformation = simTime();
        receivedMessage = false;
    }
}
