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

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

/**
 * @file Window.h
 * @brief Brittle abstract class that manages a GLFW3 window with a highway simulation on it
 */

#include <GLFW/glfw3.h>
#include "Highway.h"
#include "UIPresenter.h"
#include <chrono>

/**
 * @brief Brittle abstract class that manages a GLFW3 window with a highway simulation on it.
 */
class Window : public ScreenMapper {
public:

    Window(Highway &high);

    /**
     * Propagates the GLFW3 key callback to the UIPresenter and the app logic
     */
    void key_callback(int key, int scancode, int action, int mods);

    virtual ~Window();

    /**
     * Start the simulation
     */
    void start();

    virtual void draw(int width, int height) = 0;

    Highway &highway;

    UIPresenter *presenter;

protected:
    GLFWwindow *window;

    /**
     * Time at which the simulation started
     */
    std::chrono::system_clock::time_point startTime;

    /**
     * Lane width in meters
     */
    static constexpr float LANE_WIDTH = 7.0;

    /**
     * Screen width in pixels
     */
    int width;

    /**
     * Screen height in pixels
     */
    int height;

private:

    float timeElapsed();

};

#endif /* USERINTERFACE_H */

