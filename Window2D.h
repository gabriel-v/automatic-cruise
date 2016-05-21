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

/**
 * @file Window2D.h
 * @brief Implementation for a 2D view of the highway
 */

#include <unordered_map>
#include "Window.h"
#include "Foliage2D.h"

/**
 * Implementation for a 2D view of the highway.
 */
class Window2D : public Window {
private:

    void drawVehicle(Vehicle *const v);
    /**
     * Draws a dash separating two lanes.
     * @param xMeters the left hand side of the screen
     * @param yScreen center line
     * @param thickness In screen coords
     */
    void drawDash(float xMeters, float yScreen, float thickness);
    /**
     * Draws an opengl rect (in screen coords) using glVertex2f.
     */
    void drawRect(float left, float right, float bottom, float top);

    /**
     * Draws a rectangle around the given vehicle
     * @param v The on-screen vehicle
     * @param red
     * @param green
     * @param blue
     */
    void markVehicle(const Vehicle *v, float red, float green, float blue);

    /**
     * Returns left and right margins shown on the screen, in meters (road coords).
     */
    std::pair<float, float> roadLimits();

    /**
     * Draws all the vehicles on this given lane
     * @param vs Sorted vehicle deque. Drawing will be done only for the vehicles on screen.
     */
    void drawVehicles(const std::deque<Vehicle *> vs);

    /**
     * Number of meters for a given
     */
    float ratio;

    /**
     * Current center of the screen, in meters.
     */
    float centerX;

    /**
     * Random triangle generator.
     */
    Foliage2D *foliage;

    /**
     * Maps each vehicle to its texture ID.
     */
    std::unordered_map<Vehicle *, GLuint> textureMap;

    /**
     * Texture list.
     */
    std::vector<GLuint> textures;

    /**
     * Reads the textures and loads them into OpenGL.
     * @see textures
     */
    void initTextures();

protected:

    virtual void zoomIn() override;

    virtual void zoomOut() override;

    virtual Point pixelToRoadCoordinates(Point pixelCoords) override;

    virtual Point roadToScreenCoordinates(Point roadCoords) override;

    float maxLeft;
    float maxRight;
    float zoom;

public:
    virtual void draw(int width, int height) override;

    Window2D(Highway &highway);

    virtual ~Window2D() override;
};


#endif
