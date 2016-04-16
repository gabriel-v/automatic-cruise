/*
 *  Copyright (c)  2016, Gabriel Vijiala, Stefan Teodorescu
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its contributors may
 *  be used to endorse or promote products derived from this software without
 *  specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include "Highway.h"
#include "RandomVehicle.h"
#include "ACCVehicle.h"
#include "Error.h"

const int N_LANES = 3;
const double MAX_DELTA_X = 165, MIN_DELTA_X = 55;
const int N_VEHICLES_PER_LANE = 40;
const double TELEPORT_DISTANCE = N_VEHICLES_PER_LANE * MAX_DELTA_X / 1.5;
const double TELEPORT_INTERVAL = 10.0;


const int STABILISE_STEPS = 1000;
const double STABILISE_DT = 1.0/60.0;

Interval deltaX(MIN_DELTA_X, MAX_DELTA_X); // m

const Target FAR_IN_FRONT = Target(0, 1e50);
const Target FAR_IN_BACK = Target(0, -1e50);

Highway::Highway() : preferredVehicle(nullptr), lastTeleportTime(0) {
    for (int i = 0; i < N_LANES; i++) {
        Lane *lane = new Lane;

        double x = deltaX.uniform();
        for (int j = 0; j < N_VEHICLES_PER_LANE; j++) {
            x += deltaX.uniform();
            RandomVehicle *vehicle = new RandomVehicle(this, x, i);
            lane->vehicles.push_back(vehicle);
        }
        lanes.push_back(lane);
    }

    preferredVehicle = (lanes[N_LANES / 2]->vehicles.at(N_VEHICLES_PER_LANE / 2));
    lanes[N_LANES / 2]->vehicles[N_VEHICLES_PER_LANE / 2] = new ACCVehicle(*preferredVehicle);
    delete preferredVehicle;

    preferredVehicle = lanes[N_LANES / 2]->vehicles[N_VEHICLES_PER_LANE / 2];
    selectedVehicle = lanes[N_LANES / 2]->vehicles[N_VEHICLES_PER_LANE / 2 + 1];
}

Highway::Highway(const Highway &orig) :
        lanes(orig.lanes),
        preferredVehicle(orig.preferredVehicle),
        lastTeleportTime(0) {
}

Highway::~Highway() {
    for (Lane *l: lanes) {
        delete l;
    }
    lanes.clear();
}

void Highway::teleportVehicles() {
    double centerX = preferredVehicle->getX();
    double X;

    Vehicle *v;
    double lane = 0;
    for (Lane *l: lanes) {
        int addFront = 0, addBack = 0;
        while (std::abs(l->vehicles.front()->getX() - centerX) > TELEPORT_DISTANCE) {
            v = l->vehicles.front();
            l->vehicles.pop_front();
            addBack++;
            delete v;
        }

        while (std::abs(l->vehicles.back()->getX() - centerX) > TELEPORT_DISTANCE) {
            v = l->vehicles.back();
            l->vehicles.pop_back();
            addFront++;
            delete v;
        }

        X = l->vehicles.back()->getX() + deltaX.uniform() * 2;
        for (int i = 0; i < addBack; i++) {
            v = new RandomVehicle(this, X, lane);
            l->vehicles.push_back(v);
            X += deltaX.uniform();
        }

        X = l->vehicles.front()->getX() - deltaX.uniform() * 2;
        for (int i = 0; i < addFront; i++) {
            v = new RandomVehicle(this, X, lane);
            l->vehicles.push_front(v);
            X -= deltaX.uniform();
        }
        lane += 1;
    }
}


Target *Highway::target(const Vehicle *current, const Vehicle *targ) {
    Target *t = new Target();
    if (targ->getX() > current->getX()) {
        t->dist = (targ->getX() - targ->getLength() / 2) - (current->getX() + current->getLength() / 2);
    } else {
        t->dist = (targ->getX() + targ->getLength() / 2) - (current->getX() - current->getLength() / 2);
    }

    t->vRel = targ->getV() - current->getV();
    return t;
}

void Highway::sort() {

    for (Lane *l: lanes) {
        std::sort(l->vehicles.begin(), l->vehicles.end(),
                  [](const Vehicle *a, const Vehicle *b) {
                      return a->getX() < b->getX();
                  });
    }
}

void Highway::step(double dt) {

    lastTeleportTime += dt;
    if (lastTeleportTime > TELEPORT_INTERVAL) {
        teleportVehicles();
        lastTeleportTime -= TELEPORT_INTERVAL;
    }

    if(shouldSort) {
        shouldSort = false;
        sort();
    }

    std::map<Vehicle *, Neighbours *> links;
    std::vector<std::deque<Vehicle *>::iterator> iters;

    for (Lane *l: lanes) {
        auto it = l->vehicles.begin();
        iters.push_back(it + 1);
        links[*(it)] = new Neighbours(target(*it, *(it + 1)), new Target(FAR_IN_BACK));

        ++it;

        auto e1 = l->vehicles.end() - 1;

        while (it != e1) {
            links[*it] = new Neighbours(target(*it, *(it + 1)), target(*it, *(it - 1)));
            ++it;
        }

        links[*(it)] = new Neighbours(new Target(FAR_IN_FRONT), target(*it, *(it - 1)));
    }


    while (!iters.empty()) {
        uint maxI = 0;
        for (uint i = 1; i < iters.size(); i++) {
            if ((*iters[i])->getX() < (*iters[maxI])->getX()) {
                maxI = i;
            }
        }

        if (maxI < lanes.size() - 1) {
            Target *prev = target(*iters[maxI], *(iters[maxI + 1] - 1));
            Target *next = target(*iters[maxI], *iters[maxI + 1]);
            // we have a lane to the right
            links[*iters[maxI]]->withRight(next, prev);
        }
        if (maxI > 0) {
            Target *prev = target(*iters[maxI], *(iters[maxI - 1] - 1));
            Target *next = target(*iters[maxI], *iters[maxI - 1]);
            // we have a lane to the left
            links[*iters[maxI]]->withLeft(next, prev);
        }

        ++iters[maxI];
        if ((iters[maxI] + 1) == lanes[maxI]->vehicles.end()) {
            break;
        }
    }


    for (Lane *l: lanes) {
        for (Vehicle *v: l->vehicles) {
            v->think(links[v]);
        }
    }

    for (auto &p: links) {
        delete p.second;
    }


    for (Lane *l: lanes) {
        for (Vehicle *v: l->vehicles) {
            v->step(dt);
        }
    }

    auto i = laneChangers.begin();
    while (i != laneChangers.end()) {
        auto &p = *i;
        Vehicle *v = p.first;
        LaneChangeData &data = p.second;

        if(!data.changed) {
            data.changed = true;
            lanes[data.to]->vehicles.push_back(v);
            auto it = lanes[data.from]->vehicles.begin();
            while (it != lanes[data.from]->vehicles.end() && *it != v) ++it;
            lanes[data.from]->vehicles.erase(it);

            shouldSort = true;
        }

        data.progress += dt;
        v->setLane(data.from + data.progress * data.direction);

        if (data.progress >= 1) {
            i = laneChangers.erase(i);
        } else {
            ++i;
        }

    }
}

void Highway::notifyLaneChange(Vehicle *v, int direction) {
    LaneChangeData data;
    data.direction = direction;
    data.from = (int) std::round(v->getLane());
    data.to = data.from + direction;
    data.changed = false;

    if (data.to < 0) return;
    if (data.to >= (int) lanes.size()) return;

    data.progress = 0;
    laneChangers[v] = data;
}


void Highway::stabilise() {
    for(int i = 0; i < STABILISE_STEPS; i++){
        step(STABILISE_DT);
    }
}


void Highway::addVehicleAt(double X, double lane) {
    throw Error("addVehicleAt not implemented");
}

void Highway::addVehicleInFrontOfPreferred() {
    throw Error("addVehicleInFrontOfPreferred not implemented");
}

void Highway::selectVehicleAt(double X, double lane) {
    throw Error("selectVehicleAt not implemented");
}

void Highway::unselectVehicle() {
    selectedVehicle = nullptr;
}
