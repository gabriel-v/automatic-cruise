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

#ifndef LEC_ACC_CPP_RANDOMVEHICLE_H
#define LEC_ACC_CPP_RANDOMVEHICLE_H

#include "Vehicle.h"

/**
 * Vehicles with random actions.
 * Will change lane, speed and distance once in a while.
 * Can be ordered to change lane, speed or distance, and will hold onto those values
 * for a longer time, so the user can see the effect.
 */
class RandomVehicle : public Vehicle {
public:

    /**
     * Override that sets a timeout on RandomVehicle::timeUntilNextAction
     */
    virtual void setAction(Action action) override;

    virtual void think(const Neighbours *n) override;

    virtual void step(float dt) override;

    RandomVehicle(LaneChangeObserver *highway, float x, float lane);

    RandomVehicle(const RandomVehicle &orig);

    virtual ~RandomVehicle();

    /**
     * Override that sets a timeout on RandomVehicle::timeUntilNextAction
     */
    virtual void setTargetSpeed(float targetSpeed) override;
    /**
     * Override that sets a timeout on RandomVehicle::timeUntilNextAction
     */
    virtual void setTargetDistance(float targetDistance) override;


protected:

    /**
     * Time until the vehicle does another random thing.
     */
    float timeUntilNextAction;

    virtual void decideAcceleration(const Neighbours *n) override;

    /**
     * Randomly selects an action to perform.
     */
    void decideAction();

    virtual bool canChangeLane(Target *front, Target *back) override;
};


#endif
