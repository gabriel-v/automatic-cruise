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

static Interval intSpeed(100 / 3.6, 250 / 3.6);
static Interval intActionPeriod(5.7, 13.6);
static Interval longActionPeriod(16.0, 24.0);

static Interval intActionDecider(0, 100);


RandomVehicle::RandomVehicle(LaneChangeObserver *highway, double xx, double lane) : Vehicle(highway, lane) {
    x = xx;
    timeUntilNextAction = intActionPeriod.uniform();
}

void RandomVehicle::decideAcceleration(const Neighbours *n) {
    // positive -- we have space; negative -- we're too close
    double distanceDrift = n->front->dist - targetDistance;

    // positive -- we're going too fast; negative -- we're too slow
    double speedDrift = v - targetSpeed;

    double inertialAcceleration = 0;
    double exactAcceleration = 0;
    const double exactCoef = 1 - 10e-4;

    if(n->front == nullptr){
        // No one in sight -- smoothly coast to target speed
        a = - speedDrift / reactionTime;

    } else {

        // This acceleration calculation gives our random cars
        // a more varied driving style.
        double distCoef;
        distCoef = 1 / (std::exp(- 3.0 * distanceDrift / targetDistance) + 2);
        inertialAcceleration = 2 * n->front->vRel / reactionTime;
        inertialAcceleration -= distCoef * distanceDrift / reactionTime / reactionTime;
        inertialAcceleration -= (1 - distCoef) * speedDrift / reactionTime;
        inertialAcceleration -= std::exp(1.5 * (panicDistance - n->front->dist));

        // We may have a car in front
        if(distanceDrift > 0) {
            // Time until we reach the target distance
            double reachTime = - distanceDrift / n->front->vRel;
            if(reachTime < 0.0) {
                // We will never reach the car in front
                reachTime = 1e10;
            }
            double aCorrectRelativeSpeed = n->front->vRel / reachTime;

            // Acceleration that gets us to the target speed within reactionTime
            double aCorrectSpeed = - speedDrift / reactionTime;

            exactAcceleration = aCorrectSpeed + aCorrectRelativeSpeed;

        } else {
            // Apply panic break if needed
            double aPanicBreak = 0;
            if(-distanceDrift > panicDistance) {
                aPanicBreak = - maxAcceleration;
            }

            // Decelerate to target distance. Our speed doesn't matter anymore.
            double aCorrectDistance = distanceDrift / reactionTime / reactionTime;
            double aMatchSpeed = 2 * n->front->vRel / reactionTime;
            exactAcceleration = aPanicBreak + aMatchSpeed + aCorrectDistance;
        }
    }

//    a = (1 - exactCoef) * inertialAcceleration + exactCoef * exactAcceleration;
    a = exactAcceleration;

    // Make it a little snappy
//    if(std::abs(x) > 0.5) {
//        a += sgn(a) * 1.0;
//    }
}

bool RandomVehicle::canChangeLane(Target *front, Target *back) {
    if (front == nullptr || back == nullptr)
        return false;

    double timeToFront = -front->dist / front->vRel;
    if(timeToFront > 0 && timeToFront < reactionTime / 2) return false;

    double timeToBack = back->dist / back->vRel;
    if(timeToBack > 0 && timeToBack < reactionTime / 2) return false;

    return !(std::abs(front->dist) < panicDistance * 2
             || std::abs(back->dist) < panicDistance * 2.5);
}

void RandomVehicle::decideAction() {
    double decision = intActionDecider.uniform();

    if (decision < 25) {
        action = Action::change_lane_right;
    } else if (decision < 50) {
        if(targetSpeed > 130/3.6) {
            action = Action::change_lane_left;
        } else {
            action = Action::change_lane_right;
        }
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

