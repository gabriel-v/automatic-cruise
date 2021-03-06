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

/**
 * @file Window.cpp
 * @brief Brittle abstract class that manages a GLFW3 window with a highway simulation on it
 */

#include <chrono>
#include <iostream>
#include <GLFW/glfw3.h>
#include <map>
#include <complex>
#include "Error.h"
#include "Window.h"

/**
 * Used to ensure we have only one Window running
 */
static int window_reference_count = 0;

static void global_error_callback(int x, const char *message) {
    std::cerr << "Error " << x << ": " << message << std::endl;
}

static std::map<GLFWwindow *, Window *> activeWindows;

static void global_key_callback(GLFWwindow *window, int key, int scancode, int action,
                                int mods) {
    activeWindows[window]->presenter->key_callback(key, scancode, action, mods);
    activeWindows[window]->key_callback(key, scancode, action, mods);
}

static void global_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    activeWindows[window]->presenter->mouse_button_callback(button, action, mods);
}

void Window::key_callback(int key, int, int action, int) {
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
                break;
        }

    }
}


Window::Window(Highway &high) : highway(high) {
    if (window_reference_count > 0) {
        throw Error("Only one Window is permitted!");
    } else {
        window_reference_count++;
    }

    glfwSetErrorCallback(global_error_callback);
    if (!glfwInit()) {
        throw Error("Glfw library init failed");
    }

    startTime = std::chrono::system_clock::now();
    const GLFWvidmode *vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    width = vidmode->width;
    height = vidmode->height;

    window = glfwCreateWindow(width, height,
                              "Automagic Cruise Control - Kernel Panic Blues",
                              glfwGetPrimaryMonitor(),
                              NULL);
    if (!window) {
        glfwTerminate();
        throw Error("Glfw window creation failed");
    }
    glfwMakeContextCurrent(window);

    presenter = new UIPresenter(highway, window, this);

    activeWindows[window] = this;
    glfwSetKeyCallback(window, global_key_callback);
    glfwSetMouseButtonCallback(window, global_mouse_button_callback);
    glfwSwapInterval(1);
}

Window::~Window() {
    delete presenter;
    glfwDestroyWindow(window);
    glfwTerminate();
}

float Window::timeElapsed() {
    return std::chrono::duration<float>(
            std::chrono::system_clock::now() - startTime
    ).count();
}

void Window::start() {
    float last = timeElapsed() - 1.0f / 60.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        float now = timeElapsed();

        presenter->present(now - last);

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.0, 0.4, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        highway.step(now - last);
        draw(width, height);

        last = now;

        presenter->render();

        glfwSwapBuffers(window);
    }
}


