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

const int N_LANES = 3;
const double MAX_DELTA_X = 165, MIN_DELTA_X = 65;
const int N_VEHICLES_PER_LANE = 50;
const double TELEPORT_DISTANCE = N_VEHICLES_PER_LANE * MAX_DELTA_X / 1.5;
const double TELEPORT_INTERVAL = 7.0;

const double MAX_VIEW_DISTANCE = 100.0;

const int STABILISE_STEPS = 800;
const double STABILISE_DT = 1.0 / 60.0;

Interval deltaX(MIN_DELTA_X, MAX_DELTA_X); // m

const Target FAR_IN_FRONT = Target(0, 1e6); // 1000km, basically infinity
const Target FAR_IN_BACK = Target(0, -1e6); // 1000km, basically infinity

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
//    selectedVehicle = lanes[N_LANES / 2]->vehicles[N_VEHICLES_PER_LANE / 2 + 1];
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

    if(std::abs(t->dist) > MAX_VIEW_DISTANCE) {
        t->dist = std::abs(t->dist) / t->dist * 1e6; // 1000km, basically infinity.
        t->vRel = 0;
    }
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
        shouldSort = true;
    }

    if (shouldSort) {
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
            links[*iters[maxI]]->withLeft(next, prev);
        }
        if (maxI > 0) {
            Target *prev = target(*iters[maxI], *(iters[maxI - 1] - 1));
            Target *next = target(*iters[maxI], *iters[maxI - 1]);
            // we have a lane to the left
            links[*iters[maxI]]->withRight(next, prev);
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

    this->preferredVehicleFrontDistance = links[preferredVehicle]->front->dist;

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

        if (!data.changed) {
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
    for (int i = 0; i < STABILISE_STEPS; i++) {
        step(STABILISE_DT);
    }
}


void Highway::addVehicleAt(double X, double lane, double speed) {
    int l = (int) std::round(lane);
    if (l < 0 || l >= (int) lanes.size());
    auto it = lanes[l]->vehicles.begin();
    auto end = lanes[l]->vehicles.end();
    while (it != end && (*it)->getX() < X) {
        ++it;
    }
    if (it == end) {
        X = lanes[l]->vehicles.back()->getX() + deltaX.uniform();
    } else {
        X = ((*it)->getX() + (*(it - 1))->getX()) / 2;
    }

    Vehicle *v = new RandomVehicle(this, X, lane);
    //v->setV(((*it)->getV() + (*(it - 1))->getV()) / 2);
    v->setV(speed);
    v->setTargetSpeed(speed);
    lanes[l]->vehicles.insert(it, v);

}

void Highway::addVehicleInFrontOfPreferred(double speed) {
    addVehicleAt(preferredVehicle->getX() + 1, preferredVehicle->getLane(), speed);
}

void Highway::selectVehicleAt(double X, double lane) {
    selectedVehicle = nullptr;

    int l = (int) std::round(lane);
    if (l < 0 || l >= (int) lanes.size()) return;
    auto it = lanes[l]->vehicles.begin();
    auto end = lanes[l]->vehicles.end();
    while (it != end && (*it)->getX() < X - 10) {
        ++it;
    }
    if (it == end) {
        return;
    }
    Vehicle *v = *it;

    if (v->getLength() < std::abs(X - v->getX())) return;
    selectedVehicle = v;
}

void Highway::unselectVehicle() {
    selectedVehicle = nullptr;
}
