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
#include "ACCVehicle.h"



ACCVehicle::ACCVehicle(const Vehicle &x): Vehicle(x) {
    r = 0.1;
    b = 0.1;
    g = 1.0;
}

void ACCVehicle::decideAction(const Neighbours *n) {
    //RandomVehicle::decideAction(n);
}


void ACCVehicle::decideAcceleration(const Neighbours *n) {
    double distCoef;
    if (n->front != nullptr) {
        distCoef = (double) std::exp(- 3 * (n->front->dist - targetDistance) / targetDistance);
        a = distCoef / (distCoef + 1) * -2 * targetDistance / reactionTime / reactionTime
            + 2 * n->front->vRel / reactionTime;
        a += 1 / (distCoef + 1) * (targetSpeed - v) / reactionTime;

        a -= std::exp(7.5 * (panicDistance - n->front->dist));
    } else {
        a = (targetSpeed - v) / reactionTime;
    }
}

bool ACCVehicle::canChangeLane(Target *front, Target *back) {
    return false;
}


void ACCVehicle::think(const Neighbours *n) {
    //Vehicle::think(n);
    decideAcceleration(n);

}

void ACCVehicle::step(double dt) {
    Vehicle::step(dt);
}
