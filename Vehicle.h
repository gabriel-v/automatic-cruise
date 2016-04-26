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

#ifndef VEHICLE_H
#define VEHICLE_H

#include "Interval.h"
#include "Neighbours.h"


enum class Action {
    none,
    change_lane_left,
    change_lane_right
};

class Vehicle;

class LaneChangeObserver {
public:
    virtual void notifyLaneChange(Vehicle *v, int direction) = 0;
};

class Vehicle {
public:
    Vehicle(LaneChangeObserver *highway, double lane);

    Vehicle(const Vehicle &orig);

    virtual ~Vehicle();

    virtual void think(const Neighbours *n);

    virtual void step(double dt);

    virtual bool operator<(const Vehicle &other);

protected:

    double x, v, a;
    double targetSpeed;
    double targetDistance;
    double width, length;
    LaneChangeObserver *highway;
    double lane;
    double panicDistance;
    double reactionTime;
    double terminalSpeed;
    double maxAcceleration;

    Action action;

    virtual void decideAcceleration(const Neighbours *n) = 0;

    virtual bool canChangeLane(Target *front, Target *back) = 0;

public:
    double getTargetSpeed() const {
        return targetSpeed;
    }

    double getTargetDistance() const {
        return targetDistance;
    }

    double getWidth() const {
        return width;
    }

    double getLength() const {
        return length;
    }

    double getX() const {
        return x;
    }

    double getV() const {
        return v;
    }

    virtual void setAction(Action action) {
        this->action = action;
    }

    void setV(double v) {
        this->v = v;
    }

    virtual void setTargetSpeed(double targetSpeed) {
        this->targetSpeed = targetSpeed;
    }

    void setLane(double lane) {
        this->lane = lane;
    }

    double getLane() const {
        return lane;
    }

    virtual void setTargetDistance(double targetDistance) {
        this->targetDistance = targetDistance;
    }
};

#endif /* VEHICLE_H */

