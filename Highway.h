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

/**
 * @file Highway.h
 * @brief Highway simulation
 */


#include <map>
#include <vector>
#include "Lane.h"
#include "Vehicle.h"

/**
 * Data kept for each vehicle currently chaing lane.
 */
struct LaneChangeData {
    int from;
    int to;
    float progress;
    int direction;
    bool changed;
};

/**
 * The one-way highway, with all it's algorithms.
 * This is basically our simulation driver.
 */
class Highway : public LaneChangeObserver {
public:
    Highway();

    Highway(const Highway &orig);

    virtual ~Highway();

    /**
     * Steps all the vehicles and calls Vehicle::think
     */
    void step(float dt);

    /**
     * Register the request for a vehicle to change lane.
     */
    void notifyLaneChange(Vehicle *v, int direction);

    /**
     * Run a number of steps to stabilise the system.
     */
    void stabilise();

    /**
     * Adds random vehicle at that approximate road coordinate.
     */
    bool addVehicleAt(float X, float lane, float speed);

    /**
     * Adds random vehicle in front of the ACC.
     */
    bool addVehicleInFrontOfPreferred(float speed);

    /**
     * Tries to select a vehicle at given road coordinate.
     * If succeeded, will set the Highway::selectedVehicle pointer.
     */
    void selectVehicleAt(float X, float lane);

    /**
     * Tries to select a vehicle at given road coordinate.
     * Unsets the Highway::selectedVehicle pointer.
     */
    void unselectVehicle();

    /**
     * The lanes of our highway. All point to the right.
     */
    std::vector<Lane *> lanes;

    /**
     * Pointer to the vehicle that will be tracked by the camera, the ACC.
     */
    Vehicle *preferredVehicle = nullptr;

    /**
     * The vehicle selected by the user, if any.
     */
    Vehicle *selectedVehicle = nullptr;

    /**
     * The distance from the ACC to the next vehicle is stored in this field.
     */
    float preferredVehicleFrontDistance = 0;

private:
    /**
     * Time elapsed since last teleport.
     */
    float lastTeleportTime = 0;

    /**
     * Stores the vehicles that are currently chaning lane.
     */
    std::map<Vehicle *, LaneChangeData> laneChangers;

    /**
     * Moves the vehicles too far to the back at the front of our column, and the other way around.
     * This is a trick to reuse resources.
     */
    void teleportVehicles();

    /**
     * Helper function, returns a Target object.
     */
    Target *target(const Vehicle *current, const Vehicle *targ);

    /**
     * Sorts the vehicles on all the lanes, after their X coordinate.
     */
    void sort();
};

#endif /* HIGHWAY_H */

