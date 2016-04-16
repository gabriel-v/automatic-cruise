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
#include <imgui.h>
#include "UIPresenter.h"
#include "imgui_impl_glfw.h"

UIPresenter::UIPresenter(Highway &highway, GLFWwindow *window):
        highway(highway),
        window(window) {
    ImGui_ImplGlfw_Init(window, true);
    status = "OK.";
}

UIPresenter::UIPresenter(const UIPresenter &orig):
        highway(orig.highway),
        window(orig.window) {

}

UIPresenter::~UIPresenter() {
    ImGui_ImplGlfw_Shutdown();
}

void UIPresenter::present() {

    ImGui_ImplGlfw_NewFrame();

    commandView();
    if(showStatsView) {
        statsView();
    }

}

void UIPresenter::key_callback(int key, int scancode, int action, int mods) {
    ImGui_ImplGlFw_KeyCallback(window, key, scancode, action, mods);
}

void UIPresenter::render() {

    ImGui::Render();
}


void UIPresenter::commandView() {
    static bool opened = true;
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Command", &opened);

    ImGui::SliderFloat("ACC Target Speed", &accTargetSpeed, 0.0f, 250.0f);
    ImGui::SliderFloat("ACC Target Distance", &accTargetDistance, 10.0f, 120.0f);

    if (ImGui::Button("Toggle Stats")) {
        showStatsView ^= 1;
    }

    if (ImGui::Button("ACC change lane left")) {
        status = "Not Implemented.";
    }

    if (ImGui::Button("ACC change lane right")) {
        status = "Not Implemented.";
    }

    if (ImGui::Button("Add vehicle forward")) {
        status = "Not Implemented.";
    }

    if (ImGui::Button("Reset highway")) {
        status = "Not Implemented.";
    }
    if (ImGui::Button("Add vehicle forward")) {
        status = "Not Implemented.";
    }

    ImGui::Text((std::string("Status: ").append(status)).c_str());

    ImGui::End();
}

void UIPresenter::statsView() {
    ImGui::SetNextWindowPos(ImVec2(5, 320), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Statistics", &showStatsView);

    ImGui::Text("FPS: %.0f (%.1f ms/frame) ",  ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Text("ACC Speed: %.0f km/h", highway.prefferredVehicle->getV() * 3.6f);
    ImGui::Text("ACC Distance to next vehicle: %.0f meters", 0.0f);

    ImGui::End();
}

