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

static Interval intWidth(3.3, 3.9);
static Interval intLength(5.5, 6.6);
static Interval intSpeed(26, 48); // m / s
static Interval intActionPeriod(3, 6);
static Interval intReactionTime(2.5, 3.6);
static Interval intTargetDistance(35, 50);


static Interval intActionDecider(0, 100);

const double panicDistance = 12;

RandomVehicle::RandomVehicle(LaneChangeObserver *highway, double xx, double lane) : Vehicle(highway, lane) {
    x = xx;
    v = targetSpeed = intSpeed.uniform();
    width = intWidth.normal();
    length = intLength.normal();
    timeUntilNextAction = intActionPeriod.uniform();

    reactionTime = intReactionTime.uniform();
    targetDistance = intTargetDistance.uniform();
}

void RandomVehicle::decideAcceleration(const Neighbours *n) {
    double distCoef;
    if (n->front != nullptr) {
        distCoef = std::exp(-3 * (n->front->dist - targetDistance) / targetDistance);
        a = distCoef / (distCoef + 1) * -2 * targetDistance / reactionTime / reactionTime
            + 2 * n->front->vRel / reactionTime;
        a += 1 / (distCoef + 1) * (targetSpeed - v) / reactionTime;

        a -= exp(1.5 * (panicDistance - n->front->dist));
    } else {
        a = (targetSpeed - v) / reactionTime;
    }
}

bool RandomVehicle::canChangeLane(Target *front, Target *back) {
    if (front == nullptr || back == nullptr)
        return false;
    if (std::abs(front->dist) < panicDistance / 1.5
        || std::abs(back->dist) < panicDistance / 1.5)
        return false;

    double timeBack = -back->vRel / back->dist;
    double timeFront = -front->vRel / front->dist;

    if (timeBack > 0 && timeBack < reactionTime / 1.5) return false;
    if (timeFront > 0 && timeFront < reactionTime / 1.5) return false;
    return true;
}

void RandomVehicle::decideAction(const Neighbours *n) {
    double decision = intActionDecider.uniform();

    if (decision < 35) {
        // 35 % chance to try and change lane left, then right
        if (canChangeLane(n->frontLeft, n->backLeft)) {
            highway->notifyLaneChange(this, -1);
            return;
        }

        if (canChangeLane(n->frontRight, n->backRight)) {
            highway->notifyLaneChange(this, 1);
        }

    } else if (decision < 70) {
        // 35 % chance to try and change lane right, then left

        if (canChangeLane(n->frontRight, n->backRight)) {
            highway->notifyLaneChange(this, 1);
        }

        if (canChangeLane(n->frontLeft, n->backLeft)) {
            highway->notifyLaneChange(this, -1);
            return;
        }

    } else if (decision < 90) {
        // % 20 % chance to change speed
        targetSpeed = intSpeed.uniform();
    } else {
        // 10 % chance to think again in 1.5 seconds
        timeUntilNextAction = 1.5;
    }

}

void RandomVehicle::think(const Neighbours *n) {

    decideAcceleration(n);

    if (timeUntilNextAction < 0) {
        timeUntilNextAction = intActionPeriod.uniform();
        decideAction(n);
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








