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
#ifndef LEC_ACC_CPP_UI_CONTROLLER_H
#define LEC_ACC_CPP_UI_CONTROLLER_H

/**
 * @file UIPresenter.cpp
 * @brief Coordinates between ImGUI and app logic
 */

#include <GLFW/glfw3.h>
#include <string>
#include "Highway.h"

/**
 * 2D point, uses floats.
 * I got lazy using std::pair<float, float> so I wrote this.
 */
struct Point {
    float x;
    float y;

    Point() {
        x = y = 0;
    }

    Point(float x, float y) : x(x), y(y) { }
};

/**
 * Interface for the Window.
 */
class ScreenMapper {
public:
    virtual Point pixelToRoadCoordinates(Point pixelCoords) = 0;

    virtual Point roadToScreenCoordinates(Point roadCoords) = 0;

    virtual void zoomIn() = 0;

    virtual void zoomOut() = 0;
};

/**
 * Handles the User Interface, using ImGui.
 * This is coupled with GLFW3, because of ImGui_Init.
 */
class UIPresenter {
protected:
    /**
     * Ref to the highway
     */
    Highway &highway;

    /**
     * The window the UI is rendered to
     */
    GLFWwindow *window;

    /**
     * The logical window that maps cars to screen coordinates and pixel values
     */
    ScreenMapper *screenMapper;

    /**
     * ImGui flag used to toggle the stats window.
     */
    bool showStatsView = true;
//    bool showDemoView = false;

    /**
     * ACC target distance, in meters, being set from the UI.
     */
    float accTargetDistance = 40.0f;

    /**
     * ACC target speed, in km/h, being set from the UI.
     */
    float accTargetSpeed = 120.0f;

    /**
     * Selected vehicle target distance, in meters, being set from the UI.
     */
    float randomTargetDistance;
    /**
     * Selected vehicle target speed, in km/h, being set from the UI.
     */
    float randomTargetSpeed;

    /**
     * The speed in km/h for a new vehicle (that gets spawned when the user clicks 'add vehicle'
     */
    float newVehicleSpeed = 120.0f;

    /**
     * UI status message. This should reset once in a while
     */
    std::string status;

    /**
     * Displays stats, like ACC speed and distance.
     */
    void statsView();

    /**
     * Main command view, controls the ACC and the simulation.
     */
    void commandView();

    /**
     * Sets the state string and resets the timeout.
     */
    void setState(std::string status);

    /**
     * Time, in seconds, until the status will reset to 'OK'
     */
    float timeToStateReset;

    /**
     * True if the user clicked 'place vehicle anywhere' and the UI is waiting for a click on the road.
     */
    bool waitingForVehiclePlacement = false;

    /**
     * Resets the timer and the status to 'OK'.
     * Forgets any past state.
     */
    void resetState();


public:
    UIPresenter(Highway &highway, GLFWwindow *window, ScreenMapper *screenMapper);

    UIPresenter(const UIPresenter &orig);

    virtual ~UIPresenter();

    /**
     * Builds the ImGui interface.
     * Should be called as soon as possible into the loop.
     * @param dt Time elapsed since last calling. Used for state timeout
     */
    void present(float dt);

    /**
     * Renders the UI onto the screen.
     * Calls ImGui::Render()
     */
    void render();

    /**
     * Redirects key callbacks to ImGui.
     * Application logic may be implemented here.
     */
    void key_callback(int key, int scancode, int action, int mods);

    /**
     * Mouse button logic goes in here.
     * Controls application logic: vehicle selection, vehicle placement.
     */
    void mouse_button_callback(int button, int action, int mods);

    /**
     * View with knobs and switches for the random vehicle that the user selects.
     */
    void showRandomVehicleView();

};


#endif
