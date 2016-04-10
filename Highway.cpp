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

const int N_LANES = 3;
const double MAX_DELTA_X = 165, MIN_DELTA_X = 50;
const int N_VEHICLES_PER_LANE = 80;
const double TELEPORT_DISTANCE = N_VEHICLES_PER_LANE * MAX_DELTA_X / 1.5;
const double TELEPORT_INTERVAL = 20.0;

Interval deltaX(MIN_DELTA_X, MAX_DELTA_X); // m


Highway::Highway() : prefferredVehicle(NULL), lastTeleportTime(0) {
    for (int i = 0; i < N_LANES; i++) {
        Lane *lane = new Lane;

        double x = deltaX.uniform();
        for (int j = 0; j < N_VEHICLES_PER_LANE; j++) {
            x += deltaX.uniform();
            RandomVehicle *vehicle = new RandomVehicle(x);
            lane->vehicles.push_back(vehicle);
        }
        lanes.push_back(lane);
    }

    prefferredVehicle = (lanes[N_LANES / 2]->vehicles.at(N_VEHICLES_PER_LANE / 2));
    prefferredVehicle->setTargetSpeed(130);
}

Highway::Highway(const Highway &orig) :
        lanes(orig.lanes),
        prefferredVehicle(orig.prefferredVehicle),
        lastTeleportTime(0) {
}

Highway::~Highway() {
    for (Lane *l: lanes) {
        delete l;
    }
    lanes.clear();
}

void Highway::teleportVehicles() {
    double centerX = prefferredVehicle->getX();
    double X;

    Vehicle *v;
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
            v = new RandomVehicle(X);
            l->vehicles.push_back(v);
            X += deltaX.uniform();
        }

        X = l->vehicles.front()->getX() - deltaX.uniform() * 2;
        for (int i = 0; i < addFront; i++) {
            v = new RandomVehicle(X);
            l->vehicles.push_front(v);
            X -= deltaX.uniform();
        }

    }
}


Target *Highway::target(const Vehicle *current, const Vehicle *targ) {
    Target *t = new Target();
    t->dist = targ->getX() - targ->getLength() / 2 - current->getX() - current->getLength() / 2;
    t->vRel = targ->getV() - current->getV();
    return t;
}


void Highway::step(double dt) {

    lastTeleportTime += dt;
    if (lastTeleportTime > TELEPORT_INTERVAL) {
        teleportVehicles();
        lastTeleportTime -= TELEPORT_INTERVAL;
    }

    for (Lane *l: lanes) {
        std::sort(l->vehicles.begin(), l->vehicles.end(),
                  [](const Vehicle *a, const Vehicle *b) {
                      return a->getX() < b->getX();
                  });
    }


    std::map<Vehicle *, Neighbours *> links;

//    std::vector<std::deque<Vehicle *>::iterator> iters;
    for (Lane *l: lanes) {
        auto it = l->vehicles.begin();
        //iters.push_back(sit+1);
        links[*(it)] = new Neighbours(target(*it, *(it + 1)), nullptr);

        ++it;

        auto e1 = l->vehicles.end() - 1;

        while (it != e1) {
            links[*it] = new Neighbours(target(*it, *(it + 1)), target(*it, *(it - 1)));
            ++it;
        }

        links[*(it)] = new Neighbours(nullptr, target(*it, *(it - 1)));
    }

    /*while(!iters.empty()) {
        int maxI = 0;
        for(int i = 1; i < iters.size(); i++) {
            if((*iters[i])->getX() < (*iters[maxI])->getX()) {
                maxI = i;
            }
        }

        auto &it = iters[maxI];
        links[*it] = Neighbours(target(*it, *(it + 1)), target(*it, *(it - 1)));

        // TODO: check links for left and right
        if(maxI < lanes.size()) {
            // we have a lane to the right
            links[*it].withRight(target(*it, *iters[maxI + 1]), target(*it, *(iters[maxI + 1] - 1)));
        }
        if(maxI > 0) {
            // we have a lane to the left
            links[*it].withLeft(target(*it, *iters[maxI - 1]), target(*it, *(iters[maxI - 1] - 1)));
        }

        ++it;
        if((it+1) == lanes[maxI]->vehicles.end()) {
            iters.erase(iters.begin() + maxI);
        }
    } */


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


    std::cerr << "\r" << prefferredVehicle->getV();

}

