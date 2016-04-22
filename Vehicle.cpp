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

#include "Vehicle.h"

static Interval intSpeed(26, 48); // m / s
static Interval intWidth(3.3, 3.9);
static Interval intLength(5.5, 6.6);
static Interval intTargetDistance(35, 50);
static Interval intColor(0.0, 0.5);
static Interval intReactionTime(2.5, 3.6);
static Interval intTerminalSpeed(180 / 3.6, 350 / 3.6);
static Interval intMaximumAcceleration(8.0, 16.0);


const double PANIC_DISTANCE = 12.0;

const double MIN_A = -19;

Vehicle::Vehicle(LaneChangeObserver *highway, double lane) : highway(highway), lane(lane) {
    x = a = v = 0;

    v = targetSpeed = intSpeed.uniform();
    width = intWidth.normal();
    length = intLength.normal();
    targetDistance = intTargetDistance.uniform();

    reactionTime = intReactionTime.uniform();
    panicDistance = PANIC_DISTANCE;

    r = intColor.normal() + 0.4;
    g = intColor.normal() + 0.4;
    b = intColor.normal() + 0.3;

    terminalSpeed = intTerminalSpeed.normal();
    maxAcceleration = intMaximumAcceleration.normal();

    action = Action::none;
}


Vehicle::Vehicle(const Vehicle &orig) :
        x(orig.x), v(orig.v), a(orig.a), targetSpeed(orig.targetSpeed), targetDistance(orig.targetDistance),
        width(orig.width), length(orig.length), r(orig.r), g(orig.g), b(orig.b),
        highway(orig.highway), lane(orig.lane), panicDistance(orig.panicDistance),
        reactionTime(orig.reactionTime), terminalSpeed(orig.terminalSpeed), maxAcceleration(orig.maxAcceleration) {
}

Vehicle::~Vehicle() {
}


bool Vehicle::operator<(const Vehicle &other) {
    return x < other.x;
}

void Vehicle::step(double dt) {
    double MAX_A = maxAcceleration * (1.0 - v / terminalSpeed);
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











