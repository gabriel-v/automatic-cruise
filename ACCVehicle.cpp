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

ACCVehicle::ACCVehicle(const Vehicle &x) : Vehicle(x) {
    // We're a supercar
//    terminalSpeed = 340 / 3.6;
//    maxAcceleration = 12.0;
    unsatisfiedTime = 0.0;
}

void ACCVehicle::decideAcceleration(const Neighbours *n) {
    // positive -- we have space; negative -- we're too close
    float distanceDrift = n->front->dist - targetDistance;

    // positive -- we're going too fast; negative -- we're too slow
    float speedDrift = v - targetSpeed;

    if (n->front == nullptr) {
        // No one in sight -- smoothly coast to target speed
        a = -speedDrift / reactionTime;
        unsatisfied = false;
    } else {
        // We may have a car in front
        if (distanceDrift > 0) {
            // Time until we reach the target distance
            float reachTime = -distanceDrift / n->front->vRel;
            if (reachTime <= 0.0) {
                // We will never reach the car in front
                reachTime = 1e10;
            }

            reachTime += 0.02;

            // Match the front car's speed within reachTime
            float aCorrectRelativeSpeed = n->front->vRel / reachTime;

            // Gets us to the target speed within reactionTime
            float aCorrectSpeed = -speedDrift / reactionTime;

            a = aCorrectSpeed + aCorrectRelativeSpeed;

            // We think about overtaking if we'll reach target distance within
            // 2 * reactionTime or if we're right at that distance
            unsatisfied = reachTime < reactionTime * 2 or std::abs(distanceDrift) < 1.0;


        } else {
            // Apply panic break if needed
            float aPanicBreak = 0;
            if (-distanceDrift > panicDistance) {
                aPanicBreak = -maxAcceleration;
            }

            // Decelerate to target distance. Our target speed doesn't matter anymore.
            float aCorrectDistance = distanceDrift / reactionTime / reactionTime;
            float aMatchSpeed = 2 * n->front->vRel / reactionTime;
            a = aPanicBreak + aMatchSpeed + aCorrectDistance;

            unsatisfied = true;
        }
    }

    // Make it a little snappy
    if (std::abs(x) > 0.5) {
        a += sgn(a) * 1.0;
    }

}

bool ACCVehicle::shouldChangeLane(Target *front, Target *back) {
    if (!canChangeLane(front, back)) {
        return false;
    }

    // Front vehicle is too close
    if (front->dist < targetDistance * 0.5 + panicDistance) {
        return false;
    }

//    float avgSpeed = (v + targetSpeed) / 2;
//    float newRelativeSpeed = front->vRel + v - avgSpeed;
    float reachTime = front->dist / front->vRel;

    if (reachTime < 0.0) reachTime = 1e10;

    return reachTime > reactionTime && front->vRel > -targetSpeed / 20;
}

bool ACCVehicle::canChangeLane(Target *front, Target *back) {
    if (front == nullptr || back == nullptr) {
        return false;
    }

    return !(std::abs(front->dist) < panicDistance * 2
             || std::abs(back->dist) < panicDistance * 2);
}


void ACCVehicle::think(const Neighbours *n) {
    if (unsatisfiedTime > reactionTime) {
        if (shouldChangeLane(n->frontLeft, n->backLeft)) {
            action = Action::change_lane_left;
            unsatisfiedTime = 0.0;
        } else if (shouldChangeLane(n->frontRight, n->backRight)) {
            action = Action::change_lane_right;
            unsatisfiedTime = 0.0;
        }
    }

    Vehicle::think(n);
}

void ACCVehicle::step(float dt) {
    Vehicle::step(dt);

    if (unsatisfied) {
        unsatisfiedTime += dt;
    } else {
        unsatisfiedTime = std::max(unsatisfiedTime / 2.0f, 0.0f);
    }
}
