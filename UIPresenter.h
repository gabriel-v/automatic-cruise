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
#ifndef LEC_ACC_CPP_UICONTROLLER_H
#define LEC_ACC_CPP_UICONTROLLER_H


#include <GLFW/glfw3.h>
#include <string>
#include "Highway.h"


struct Point {
    double x;
    double y;

    Point() {
        x = y = 0;
    }

    Point(double x, double y): x(x), y(y){}
};

class ScreenMapper {
public:
    virtual Point pixelToRoadCoordinates(Point pixelCoords) = 0;
    virtual Point roadToScreenCoordinates(Point roadCoords) = 0;
    virtual void zoomIn() = 0;
    virtual void zoomOut() = 0;
};

class UIPresenter {
protected:
    Highway &highway;
    GLFWwindow *window;
    ScreenMapper *screenMapper;

    bool showStatsView = true;
    bool showDemoView = false;
    float accTargetDistance = 50.0f;
    float accTargetSpeed = 110.0f;

    std::string status;

    void statsView();

    void commandView();

    void setState(std::string status);

    double timeToStateReset;

    bool waitingForVehiclePlacement = false;

    void resetState();


public:
    UIPresenter(Highway &highway, GLFWwindow *window, ScreenMapper *screenMapper);

    UIPresenter(const UIPresenter &orig);

    virtual ~UIPresenter();

    void present(double dt);

    void render();

    void key_callback(int key, int scancode, int action, int mods);

    void mouse_button_callback(int button, int action, int mods);

};


#endif
