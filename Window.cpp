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
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, 1);
                break;
            default:
                std::cerr << "Pressed: " << key << std::endl;
        }

    }
}


Window::Window(const Highway &high) : highway(high), zoom(1.0) {
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


void Window::start() {
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &width, &height);

        reset(width, height);

        glClear(GL_COLOR_BUFFER_BIT);

        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::draw() {


    auto t2 = std::chrono::system_clock::now();

    std::chrono::duration<double, std::milli> fp = t2 - startTime;
    double fp_ms = fp.count() / 1000;
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2d(0, 0);
    glVertex2d(cos(fp_ms), sin(fp_ms));
    glVertex2d(cos(fp_ms + M_PI_2), sin(M_PI_2 + fp_ms));
    glEnd();
}


void Window::reset(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-zoom * width / height, zoom * width / height, -zoom, zoom, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}


void Window::zoomIn() {
    zoom /= 1.2;
}

void Window::zoomOut() {
    zoom *= 1.2;
}

