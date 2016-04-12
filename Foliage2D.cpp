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

static const double POSITION_MAX = 60;
static const int N_FOLIAGES = 30;
static Interval intGreen(0.4, 1.0);
static Interval intRedBlue(0.2, 0.6);
static Interval intPosition(-POSITION_MAX, POSITION_MAX);


FoliageTriangle::FoliageTriangle() {
    g = intGreen.uniform();
    r = intRedBlue.uniform();
    b = intRedBlue.uniform();

    for (int i = 0; i < 6; i++) {
        pos[i] = intPosition.uniform();
    }

    dx = 0;
}

void Foliage2D::draw(double centerX, double maxLeft, double maxRight) {
    glBegin(GL_TRIANGLES);
    for (FoliageTriangle *tr: triangles) {
        if (tr->dx < centerX + maxLeft / ratio - POSITION_MAX) {
            tr->dx = centerX + maxRight / ratio + POSITION_MAX;
        }
        std::cout << "CenterX: " << centerX << " tr->x (screen): "
        << (centerX - tr->pos[0] + tr->dx) * ratio << std::endl;
        glColor3d(tr->r, tr->b, tr->g);
        for (int i = 0; i < 3; i++) {
            glVertex2d((centerX - tr->pos[2 * i] + tr->dx) * ratio, tr->pos[2 * i + 1] * ratio);
        }
    }
    glEnd();
}

Foliage2D::Foliage2D(double ratio) : ratio(ratio) {
    for (int i = 0; i < N_FOLIAGES; i++) {
        triangles.push_back(new FoliageTriangle);
    }

}

Foliage2D::~Foliage2D() {
    for (FoliageTriangle *tr: triangles) {
        delete tr;
    }
}
