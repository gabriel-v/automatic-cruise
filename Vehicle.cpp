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

/**
 * @file Vehicle.cpp
 * Implements this really brittle abstract class.
 */

#include "Vehicle.h"

static Interval intSpeed(90 / 3.6f, 240 / 3.6f);
static Interval intWidth(3.0, 3.2);
static Interval intLength(5.5, 6.9);
static Interval intTargetDistance(50, 90);
static Interval intReactionTime(2.6, 4.6);
static Interval intTerminalSpeed(160 / 3.6f, 300 / 3.6f);
static Interval intMaximumAcceleration(7.0, 12.0);


const float PANIC_DISTANCE = 10.0;

const float MIN_A = -16;

Vehicle::Vehicle(LaneChangeObserver *highway, float lane) : highway(highway), lane(lane) {
    x = a = v = 0;

    v = targetSpeed = intSpeed.uniform();
    width = intWidth.normal();
    length = intLength.normal();
    targetDistance = intTargetDistance.uniform();

    reactionTime = intReactionTime.uniform();
    panicDistance = PANIC_DISTANCE;

    terminalSpeed = intTerminalSpeed.normal();
    maxAcceleration = intMaximumAcceleration.normal();

    action = Action::none;
}


Vehicle::Vehicle(const Vehicle &orig) :
        x(orig.x), v(orig.v), a(orig.a), targetSpeed(orig.targetSpeed), targetDistance(orig.targetDistance),
        width(orig.width), length(orig.length),
        highway(orig.highway), lane(orig.lane), panicDistance(orig.panicDistance),
        reactionTime(orig.reactionTime), terminalSpeed(orig.terminalSpeed), maxAcceleration(orig.maxAcceleration) {
}

Vehicle::~Vehicle() {
}


bool Vehicle::operator<(const Vehicle &other) {
    return x < other.x;
}

void Vehicle::step(float dt) {
    float MAX_A = maxAcceleration * (1.0f - v / terminalSpeed);
    if (std::abs(lane - std::round(lane)) > 0.02) {
        // We're during overtaking. We should limit
        // the acceleration to a moderate value
        MAX_A /= 6.0;
    }

    // Clamp desired acceleration to max values
    if (a < MIN_A) a = MIN_A;
    if (a > MAX_A) a = MAX_A;

    v += dt * a;
    if (v < 0) v = 0;
    x += dt * v;
}

void Vehicle::think(const Neighbours *n) {
    this->decideAcceleration(n);

    switch (action) {
        case Action::change_lane_left:
            if (this->canChangeLane(n->frontLeft, n->backLeft)) {
                highway->notifyLaneChange(this, +1);
                action = Action::none;
            }
            break;

        case Action::change_lane_right:
            if (this->canChangeLane(n->frontRight, n->backRight)) {
                highway->notifyLaneChange(this, -1);
                action = Action::none;
            }
            break;

        default:
            break;
    }
}











