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
#ifndef LEC_ACC_CPP_ACC_VEHICLE_H
#define LEC_ACC_CPP_ACC_VEHICLE_H


#include "Vehicle.h"

/**
 * A vehicle fitted with our adaptive cruise control system.
 */
class ACCVehicle : public Vehicle {

private:
    /**
     * Marks if the vehicle is currently slowed down by traffic.
     */
    bool unsatisfied;

    /**
     * Marks for how long the vehicle has been slowed down by traffic.
     */
    float unsatisfiedTime;

    /**
     * Checks if changing lane is a good tactic.
     */
    bool shouldChangeLane(Target *front, Target *back);

protected:
    /**
     * Implementation of the cruise control algorithm.
     */
    virtual void decideAcceleration(const Neighbours *n) override;

    virtual bool canChangeLane(Target *front, Target *back) override;

public:

    virtual void think(const Neighbours *n) override;

    virtual void step(float dt) override;

    ACCVehicle(const Vehicle &x);
};


#endif
