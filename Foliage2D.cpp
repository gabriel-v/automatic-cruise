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
#include "Foliage2D.h"

#include "Interval.h"

#include <GLFW/glfw3.h>
#include <iostream>

static const double POSITION_MAX = 170;
static const int N_FOLIAGES = 20;
static Interval intGreen(0.3, 0.5);
static Interval intRedBlue(0.12, 0.18);
static Interval intPosition(-POSITION_MAX, POSITION_MAX);
static Interval intHeight(-55, 55);


FoliageTriangle::FoliageTriangle(double centerX, double ratio) {
    g = intGreen.uniform();
    r = intRedBlue.uniform();
    b = intRedBlue.uniform();

    for (int i = 0; i < 3; i++) {
        pos[2 * i] = intPosition.uniform() / 2;
        pos[2 * i + 1] = intHeight.uniform();
    }

    dx = centerX  + intPosition.uniform() * (ratio+1)/ratio;
}

void Foliage2D::draw(double centerX) {
    glBegin(GL_TRIANGLES);
    for (FoliageTriangle *tr: triangles) {
        if (tr->dx < centerX +  - POSITION_MAX / ratio - POSITION_MAX) {
            tr->dx = centerX +  POSITION_MAX / ratio + intPosition.normal() + POSITION_MAX;
        }
        if(tr->dx > centerX + POSITION_MAX / ratio + POSITION_MAX) {
            tr->dx = centerX -  POSITION_MAX / ratio + intPosition.normal() - POSITION_MAX;
        }

        glColor3d(tr->r, tr->g, tr->b);
        for (int i = 0; i < 3; i++) {
            glVertex2d((-centerX + tr->pos[2 * i]/ratio +  tr->dx) * ratio, tr->pos[2 * i + 1]);
        }
    }
    glEnd();
}

Foliage2D::Foliage2D(double ratio, double centerX) : ratio(ratio) {
    for (int i = 0; i < N_FOLIAGES; i++) {
        triangles.push_back(new FoliageTriangle(centerX, ratio));
    }

}

Foliage2D::~Foliage2D() {
    for (FoliageTriangle *tr: triangles) {
        delete tr;
    }
}
