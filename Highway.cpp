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
#include "Highway.h"
#include "RandomVehicle.h"

const int N_LANES = 3;
const double MAX_DELTA_X = 105, MIN_DELTA_X = 40;
const int N_VEHICLES_PER_LANE = 250;
const double TELEPORT_DISTANCE = N_VEHICLES_PER_LANE * MAX_DELTA_X / 3;
const double TELEPORT_INTERVAL = 5.0;
Interval deltaX(MIN_DELTA_X, MAX_DELTA_X); // m
Interval intV(20, 80); // m / s

Highway::Highway(): prefferredVehicle(NULL), lastTeleportTime(0) {
    for (int i = 0; i < N_LANES; i++) {
        Lane *lane = new Lane;

        double x = deltaX.uniform();
        for(int j = 0; j < N_VEHICLES_PER_LANE; j++) {
            x += deltaX.uniform();
            RandomVehicle *vehicle = new RandomVehicle(x, intV.uniform());
            lane->vehicles.push_back(vehicle);
        }
        lanes.push_back(lane);
    }

    prefferredVehicle = (lanes[N_LANES/2]->vehicles.at(N_VEHICLES_PER_LANE/2));
}

Highway::Highway(const Highway &orig): lanes(orig.lanes), prefferredVehicle(orig.prefferredVehicle), lastTeleportTime(0) {
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


    for (Lane *l: lanes) {
        std::vector<Vehicle *> inFront, inBack;


        while(std::abs(l->vehicles.front()->getX() - centerX) > TELEPORT_DISTANCE) {
            inFront.push_back(l->vehicles.front());
            l->vehicles.pop_front();
        }

        while(std::abs(l->vehicles.back()->getX() - centerX) > TELEPORT_DISTANCE) {
            inBack.push_back(l->vehicles.back());
            l->vehicles.pop_back();
        }

        X = l->vehicles.back()->getX() + deltaX.uniform();
        for(Vehicle *v: inFront) {
            v->setX(X);
            l->vehicles.push_back(v);
            X += deltaX.uniform();
        }

        X = l->vehicles.front()->getX() - deltaX.uniform();
        for(Vehicle *v: inBack) {
            v->setX(X);
            l->vehicles.push_front(v);
            X -= deltaX.uniform();
        }

        inBack.clear();
        inFront.clear();

    }
}


void Highway::step(double dt) {

    lastTeleportTime += dt;
    if(lastTeleportTime > TELEPORT_INTERVAL) {
        teleportVehicles();
        lastTeleportTime -= TELEPORT_INTERVAL;
    }

    for(Lane *l: lanes) {
        std::sort(l->vehicles.begin(), l->vehicles.end());
    }

    Neighbours n(NULL, NULL);
    for(Lane *l: lanes) {
        for (Vehicle *v: l->vehicles) {
            v->think(n);
            v->step(dt);
        }
    }
}

