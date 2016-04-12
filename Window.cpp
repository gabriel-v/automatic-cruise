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
#include "Window2D.h"


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


Window::Window(Highway &high) : highway(high), zoom(4.5) {
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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

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
    double accum = 0;
    int frames = 0;
    double last = timeElapsed() - 1.0 / 60.0;
    while (!glfwWindowShouldClose(window)) {
        double now = timeElapsed();
        frames++;

        int width, height;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &width, &height);

        reset(width, height);

        glClearColor(0.0, 0.4, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        highway.step(now - last);
        draw();

        accum += now - last;
        if (accum > 1.0) {
            std::cerr << "\rFPS: " << frames << " \t";
            accum -= 1.0;
            frames = 0;
        }

        last = now;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


