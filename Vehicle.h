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

/**
 * Observer callback for the highway.
 */
class LaneChangeObserver {
public:
    virtual void notifyLaneChange(Vehicle *v, int direction) = 0;
};

class Vehicle {
public:
    Vehicle(LaneChangeObserver *highway, float lane);

    Vehicle(const Vehicle &orig);

    virtual ~Vehicle();

    /**
     * Decide actions based on the neighbours and on internal state.
     */
    virtual void think(const Neighbours *n);

    /**
     * Advance the car's state using euler integration.
     */
    virtual void step(float dt);

    /**
     * Compares X coordinates. Used for sorting.
     */
    virtual bool operator<(const Vehicle &other);

protected:

    /**
     * Position on the road.
     */
    float x;
    /**
     * Speed, in m/s.
     */
    float v;
    /**
     * Acceleration, in m/s^2.
     */
    float a;
    /**
     * The speed this car is trying to keep.
     */
    float targetSpeed;
    /**
     * The desired minimum distance to the next vehicle.
     */
    float targetDistance;
    /**
     * Vehicle width, in meters.
     */
    float width;
    /**
     * Vehicle height, in meters.
     */
    float length;
    /**
     * The object that is notified when the vehicle wants to change lane.
     */
    LaneChangeObserver *highway;
    /**
     * The current lane. Has non-int values when it's currently changing lanes.
     */
    float lane;
    /**
     * Distance to the front when maximum breaking is applied.
     * lane = 0 is the rightmost position.
     */
    float panicDistance;
    /**
     * A parameter used to estimate time values.
     */
    float reactionTime;
    /**
     * The maximum speed this vehicle can reach.
     */
    float terminalSpeed;
    /**
     * The maximum acceleration this vehicle can reach.
     * Maximum acceleration decreases linearly with speed.
     *
     */
    float maxAcceleration;

    /**
     * Next desired action.
     * This field is changed to request a lane change.
     */
    Action action;

    /**
     * Decides what acceleration this vehicle will try to apply.
     */
    virtual void decideAcceleration(const Neighbours *n) = 0;

    /**
     * Decides if the vehicle can change lane.
     * @param front The vehicle in front of this one, on the target lane.
     * @param back The vehicle in the back of this one, on the target lane.
     */
    virtual bool canChangeLane(Target *front, Target *back) = 0;

public:
    float getTargetSpeed() const {
        return targetSpeed;
    }

    float getTargetDistance() const {
        return targetDistance;
    }

    float getWidth() const {
        return width;
    }

    float getLength() const {
        return length;
    }

    float getX() const {
        return x;
    }

    float getV() const {
        return v;
    }

    virtual void setAction(Action action) {
        this->action = action;
    }

    void setV(float v) {
        this->v = v;
    }

    virtual void setTargetSpeed(float targetSpeed) {
        this->targetSpeed = targetSpeed;
    }

    void setLane(float lane) {
        this->lane = lane;
    }

    float getLane() const {
        return lane;
    }

    virtual void setTargetDistance(float targetDistance) {
        this->targetDistance = targetDistance;
    }
};

/**
 * Shortcut for some algebra.
 * Should't really be in this module.
 */
template<typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif /* VEHICLE_H */

