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

#include <iostream>
#include "RandomVehicle.h"

static Interval intSpeed(90 / 3.6, 250 / 3.6); // m / s
static Interval intActionPeriod(2.7, 3.6);
static Interval longActionPeriod(16.0, 24.0);

static Interval intActionDecider(0, 100);


RandomVehicle::RandomVehicle(LaneChangeObserver *highway, double xx, double lane) : Vehicle(highway, lane) {
    x = xx;

    timeUntilNextAction = intActionPeriod.uniform();
}

void RandomVehicle::decideAcceleration(const Neighbours *n) {
    double distCoef;
    if (n->front != nullptr) {

        distCoef = std::exp(-3.0 * (n->front->dist - targetDistance) / targetDistance);

        a = distCoef / (distCoef + 1) * -2 * targetDistance / reactionTime / reactionTime
            + 2 * n->front->vRel / reactionTime;

        a += 1 / (distCoef + 1) * (targetSpeed - v) / reactionTime;

        a -= std::exp(1.5 * (panicDistance - n->front->dist));
    } else {
        a = (targetSpeed - v) / reactionTime;
    }
}

bool RandomVehicle::canChangeLane(Target *front, Target *back) {
    if (front == nullptr || back == nullptr)
        return false;

    return !(std::abs(front->dist) < panicDistance * 3
             || std::abs(back->dist) < panicDistance * 3);
}

void RandomVehicle::decideAction() {
    double decision = intActionDecider.uniform();

    if (decision < 25) {
        action = Action::change_lane_left;
    } else if (decision < 50) {
        action = Action::change_lane_right;
    } else if (decision < 80) {
        targetSpeed = intSpeed.uniform();
        action = Action::none;
    }
}

void RandomVehicle::think(const Neighbours *n) {
    Vehicle::think(n);

    if (timeUntilNextAction < 0) {
        timeUntilNextAction = intActionPeriod.uniform();
        decideAction();
    }
}

RandomVehicle::~RandomVehicle() {
}

RandomVehicle::RandomVehicle(const RandomVehicle &other) :
        Vehicle(other),
        timeUntilNextAction(other.timeUntilNextAction) {
}


void RandomVehicle::step(double dt) {
    Vehicle::step(dt);
    timeUntilNextAction -= dt;
}

void RandomVehicle::setTargetSpeed(double targetSpeed) {
    Vehicle::setTargetSpeed(targetSpeed);
    timeUntilNextAction = longActionPeriod.normal();
}

void RandomVehicle::setTargetDistance(double targetDistance) {
    Vehicle::setTargetDistance(targetDistance);
    timeUntilNextAction = longActionPeriod.normal();
}


void RandomVehicle::setAction(Action action) {
    Vehicle::setAction(action);
    timeUntilNextAction = longActionPeriod.normal();
}

