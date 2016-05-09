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

#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <map>
#include <vector>
#include "Lane.h"
#include "Vehicle.h"


struct LaneChangeData {
    int from;
    int to;
    float progress;
    int direction;
    bool changed;
};

class Highway : public LaneChangeObserver {
public:
    Highway();

    Highway(const Highway &orig);

    virtual ~Highway();

    void step(float dt);

    void notifyLaneChange(Vehicle *v, int direction);

    void stabilise();

    void addVehicleAt(float X, float lane, float speed);

    void addVehicleInFrontOfPreferred(float speed);

    void selectVehicleAt(float X, float lane);

    void unselectVehicle();

    std::vector<Lane *> lanes;

    Vehicle *preferredVehicle = nullptr;

    Vehicle *selectedVehicle = nullptr;

    float preferredVehicleFrontDistance = 0;

private:
    float lastTeleportTime = 0;

    std::map<Vehicle *, LaneChangeData> laneChangers;

    void teleportVehicles();

    Target *target(const Vehicle *current, const Vehicle *targ);

    void sort();

};

#endif /* HIGHWAY_H */

