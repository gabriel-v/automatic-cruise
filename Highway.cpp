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

#include "Highway.h"
#include "RandomVehicle.h"

const int N_LANES = 3;
const int N_VEHICLES_PER_LANE = 100;
Interval deltaX(15, 40); // m
Interval intV(20, 60); // m / s

Highway::Highway() {
    for (int i = 0; i < N_LANES; i++) {
        lanes.push_back(Lane());
        double x = 0;
        for(int j = 0; j < N_VEHICLES_PER_LANE; j++) {
            x += deltaX.normal();
            lanes[i].vehicles.push_back(RandomVehicle(x, intV.normal()));
        }
    }

    prefferredVehicle = &(lanes[N_LANES/2].vehicles[N_VEHICLES_PER_LANE/2]);
}

Highway::Highway(const Highway &orig): prefferredVehicle(orig.prefferredVehicle), lanes(orig.lanes) {
}

Highway::~Highway() {
}


void Highway::step(double dt) {
    for(Lane &l: lanes) {
        for (Vehicle &v: l.vehicles) {
            // TODO v.think
        }
    }
}

