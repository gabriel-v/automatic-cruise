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
#include <chrono>
#include <iostream>
#include <GLFW/glfw3.h>
#include <map>
#include <complex>
#include "Error.h"
#include "Window.h"

const double LANE_WIDTH = 8.0;

static void global_error_callback(int x, const char *message) {
    std::cerr << "Error " << x << ": " << message << std::endl;
}

static std::map<GLFWwindow *, Window *> activeWindows;

static void global_key_callback(GLFWwindow *window, int key, int scancode, int action,
                                int mods) {
    activeWindows[window]->key_callback(key, scancode, action, mods);
}

static void global_window_size_callback(GLFWwindow *window, int w, int h) {
    activeWindows[window]->reset(w, h);
}


void Window::key_callback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, 1);
                break;

            case GLFW_KEY_MINUS:
            case GLFW_KEY_KP_SUBTRACT:
                zoomOut();
                break;
            case GLFW_KEY_KP_ADD:
            case GLFW_KEY_EQUAL:
                zoomIn();
                break;
            default:
                std::cerr << "Pressed: " << key << std::endl;
        }

    }
}


Window::Window(Highway &high) : highway(high), zoom(3.5) {
/* Create a windowed mode window and its OpenGL context */
    startTime = std::chrono::high_resolution_clock::now();
    const GLFWvidmode *vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int width = vidmode->width;
    int height = vidmode->height;

    window = glfwCreateWindow(width, height, "Automagic Cruise Control - Kernel Panic Blues", glfwGetPrimaryMonitor(),
                              NULL);
    if (!window) {
        glfwTerminate();
        throw Error("Glfw window creation failed");
    }

    /* Make the window's context current */

    glfwSetKeyCallback(window, global_key_callback);
    glfwSetWindowSizeCallback(window, global_window_size_callback);
    glfwSwapInterval(1);

    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    activeWindows[window] = this;
}

Window::~Window() {
    glfwDestroyWindow(window);
}

void Window::init() {
    glfwSetErrorCallback(global_error_callback);
    if (!glfwInit()) {
        throw Error("Glfw initGL failed");
    }
}


void Window::term() {
    glfwTerminate();
}

double Window::timeElapsed() {
    return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startTime).count();
}


void Window::start() {
    double last = timeElapsed() - 1.0 / 60.0;
    while (!glfwWindowShouldClose(window)) {
        double now = timeElapsed();

        int width, height;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &width, &height);

        reset(width, height);

        glClearColor(0.0, 0.4, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        highway.step(now -last);
        draw();

        last = now;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void drawRect(double left, double right, double bottom, double top) {
    glBegin(GL_POLYGON);
    glVertex2d(left, top);
    glVertex2d(left, bottom);
    glVertex2d(right, bottom);
    glVertex2d(right, top);
    glEnd();
}

std::pair<double, double> Window::roadToScreen(double x, double lane) {
    double centerX = highway.prefferredVehicle->getX();
    double ratio = 2 / (highway.lanes.size() * LANE_WIDTH);

    return std::make_pair((x - centerX) * ratio, LANE_WIDTH * ratio * (lane - (highway.lanes.size() - 1.0) / 2));
};

void Window::drawVehicle(const Vehicle &v, double lane) {
    std::pair<double, double> center = roadToScreen(v.getX(), lane);
    double ratio = 2 / (highway.lanes.size() * LANE_WIDTH);
    glColor3d(v.getR(), v.getG(), v.getB());
    drawRect(center.first - ratio * v.getLength(), center.first + ratio * v.getLength() / 2,
             center.second - ratio * v.getWidth() / 2, center.second + ratio * v.getWidth() / 2);
}

void Window::drawDash(double xMeters, double lane) {
    xMeters = int((xMeters) / 16) * 16;
    std::pair<double, double> center = roadToScreen(xMeters - lane * lane * 3, lane);
    double ratio = 2 / (highway.lanes.size() * LANE_WIDTH);

    double step = ratio * 8;

    for(double x = center.first + maxLeft; x < maxRight; x += step) {
        drawRect(x, x + step / 2, center.second - 0.03, center.second + 0.03);
    }
}

void Window::draw() {

    glColor3f(0.1, 0.2, 0.3);
    drawRect(maxLeft, maxRight, -1, 1);

    glColor3f(0.9, 0.9, 0.9);
    drawRect(maxLeft, maxRight, 0.99, 0.93);
    drawRect(maxLeft, maxRight, -0.99, -0.93);

    for(int i = 0; i < highway.lanes.size() - 1; i++) {
        drawDash(highway.prefferredVehicle->getX(), i + 0.5);
    }


    glColor3f(0.7, 0.3, 0.1);
    for(int i = 0; i < highway.lanes.size(); i++) {
        for(const Vehicle *v: highway.lanes[i]->vehicles) {
            drawVehicle(*v, i);
        }
    }


}


void Window::reset(int width, int height) {
    glViewport(0, 0, width, height);
    maxLeft = -zoom * width / height;
    maxRight = zoom * width / height;


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-zoom * width / height, zoom * width / height, -zoom, zoom, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}


void Window::zoomIn() {
    zoom /= 1.15;
    if (zoom < 1.5) zoom = 1.5;
}

void Window::zoomOut() {
    zoom *= 1.15;
    if(zoom > 45) zoom = 45;
}

