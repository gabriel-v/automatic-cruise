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



#include <iostream>
#include <SOIL.h>
#include <iomanip>
#include "Window.h"
#include "Window2D.h"
#include "Interval.h"

const int GUIDE_LENGTH = 10;
const int N_TEXTURES = 7;
static Interval one(0, 1);

void Window2D::drawRect(double left, double right, double bottom, double top) {

    glVertex2d(left, top);
    glVertex2d(left, bottom);
    glVertex2d(right, bottom);
    glVertex2d(right, top);
};

std::pair<double, double> Window2D::roadLimits() {
    return std::make_pair(centerX + maxLeft / ratio - 10, centerX + maxRight / ratio + 10);
};


void Window2D::drawVehicle(Vehicle * const v) {
    Point center = roadToScreenCoordinates(Point(v->getX(), v->getLane()));
//    glColor3d(v->getR(), v->getG(), v->getB());
    auto find = textureMap.find(v);
    if(find == textureMap.end()) {
        textureMap[v] = textures[(int)(one.uniform() * N_TEXTURES)];
        std::cout << "Adding car" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, textureMap.at(v));
    double left, right, bottom, top;

    left = center.x - ratio * v->getLength() / 2;
    right = center.x + ratio * v->getLength() / 2;
    bottom = center.y - ratio * v->getWidth() / 2;
    top = center.y + ratio * v->getWidth() / 2;


    glBegin(GL_QUADS);
    {
        glTexCoord2d(0, 0);
        glVertex2d(left, top);
        glTexCoord2d(0, 1);
        glVertex2d(left, bottom);
        glTexCoord2d(1, 1);
        glVertex2d(right, bottom);
        glTexCoord2d(1, 0);
        glVertex2d(right, top);
    }
    glEnd();

}

void Window2D::markVehicle(const Vehicle *v, float red, float green, float blue) {
    glColor3f(red, green, blue);

    const float THICKNESS = 15.0f;
    glLineWidth(THICKNESS / (float) zoom);
    glBegin(GL_LINE_LOOP);
    {
        Point center = roadToScreenCoordinates(Point(v->getX(), v->getLane()));
        drawRect(center.x - ratio * v->getLength() / 1.6, center.x + ratio * v->getLength() / 1.6,
                 center.y - ratio * v->getWidth() / 1.6, center.y + ratio * v->getWidth() / 1.6);
    }
    glEnd();
    glLineWidth(1.0f);
}

void Window2D::drawVehicles(const std::deque<Vehicle *> vs) {
    std::pair<double, double> cameraLimits = roadLimits();
    auto it = vs.begin();
    while (it != vs.end() && (*it)->getX() < cameraLimits.first) ++it;

    while (it != vs.end() && (*it)->getX() < cameraLimits.second) {
        drawVehicle(*it);
        ++it;
    }
}

void Window2D::drawDash(double xMeters, double lane) {
    xMeters = int((xMeters) / GUIDE_LENGTH) * GUIDE_LENGTH;
    Point center = roadToScreenCoordinates(Point(xMeters - lane * lane * 3, lane));

    double step = ratio * GUIDE_LENGTH;

    for (double x = center.x + maxLeft; x < maxRight; x += step) {
        drawRect(x, x + step / 2, center.y - 0.03, center.y + 0.03);
    }
}

void Window2D::draw(int width, int height) {

    maxLeft = -zoom * width / height;
    maxRight = zoom * width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-zoom * width / height, zoom * width / height, -zoom, zoom, -1, 1);
    glMatrixMode(GL_MODELVIEW);


    double front = maxRight / ratio / 2.5;
    centerX = (highway.preferredVehicle->getX()) + front;
    foliage->draw(centerX);

    glBegin(GL_QUADS);
    {
        glColor3ub(78, 76, 71);
        drawRect(maxLeft, maxRight, -1, 1);

        glColor3f(0.9, 0.9, 0.9);
        drawRect(maxLeft, maxRight, 0.93, 0.99);
        drawRect(maxLeft, maxRight, -0.99, -0.93);

        for (uint i = 0; i < highway.lanes.size() - 1; i++) {
            drawDash(centerX, i + 0.5);
        }

        glColor3f(0.7, 0.3, 0.1);

    }
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//

    glColor3d(1.0, 1.0, 1.0);

        for (uint i = 0; i < highway.lanes.size(); i++) {
            drawVehicles(highway.lanes[i]->vehicles);
        }
    glDisable(GL_TEXTURE_2D);


    if (highway.selectedVehicle != nullptr) {
        markVehicle(highway.selectedVehicle, 1.0, 0.3, 0.3);
    }
    markVehicle(highway.preferredVehicle, 0.3, 1.0, 0.4);
}

void Window2D::zoomIn() {
    zoom /= 1.15;
    if (zoom < 1.5) zoom = 1.5;
}

void Window2D::zoomOut() {
    zoom *= 1.15;
    if (zoom > 45) zoom = 45;
}

Window2D::Window2D(Highway &highway) : Window(highway), zoom(4.5) {
    ratio = 2 / (highway.lanes.size() * LANE_WIDTH);
    centerX = highway.preferredVehicle->getX();
    foliage = new Foliage2D(ratio, highway.preferredVehicle->getX());

    initTextures();
}

void Window2D::initTextures() {
    const std::string path = "../res/car";
    const std::string extn = ".png";
    GLuint tex;

    for (int i = 1; i <= N_TEXTURES; i++) {
        tex = SOIL_load_OGL_texture(
                (path + std::to_string(i) + extn).c_str(),
                SOIL_LOAD_RGBA,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_MIPMAPS
        );
        textures.push_back(tex);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

Window2D::~Window2D() {
}


Point Window2D::pixelToRoadCoordinates(Point pixelCoords) {
    pixelCoords.x = pixelCoords.x / height * 2 - (double) width / height;
    pixelCoords.y = pixelCoords.y / height * 2 - 1;
    pixelCoords.x *= zoom;
    pixelCoords.y *= zoom;

    Point screen(pixelCoords);
    double lane = (highway.lanes.size() - 1.0) / 2 + screen.y / (LANE_WIDTH * ratio);
    return Point(centerX + screen.x / ratio, highway.lanes.size() - 1 - std::round(lane));
}

Point Window2D::roadToScreenCoordinates(Point roadCoords) {
    return Point((roadCoords.x - centerX) * ratio,
                 LANE_WIDTH * ratio * (roadCoords.y - (highway.lanes.size() - 1.0) / 2));
}
