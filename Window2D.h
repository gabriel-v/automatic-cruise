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

#ifndef LEC_ACC_CPP_WINDOW2D_H
#define LEC_ACC_CPP_WINDOW2D_H

#include "Window.h"
#include "Foliage2D.h"

class Window2D : public Window {
private:

    void drawVehicle(const Vehicle *v);

    void drawDash(double xMeters, double yScreen);

    void drawRect(double left, double right, double bottom, double top);

    std::pair<double, double> roadLimits();

    void drawVehicles(const std::deque<Vehicle *> vs);

    double ratio;
    double centerX;

    Foliage2D *foliage;



protected:

    virtual void zoomIn();

    virtual void zoomOut();

    virtual Point pixelToRoadCoordinates(Point pixelCoords);

    virtual Point roadToScreenCoordinates(Point roadCoords);


    double maxLeft, maxRight;
    double zoom;

public:
    virtual void draw(int width, int height);

    Window2D(Highway &highway);

    virtual ~Window2D();

    void markVehicle(const Vehicle *v);
};


#endif
