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
#include <sstream>
#include "UIPresenter.h"
#include "imgui_impl_glfw.h"

static const double RESET_TIMEOUT = 6.0;

UIPresenter::UIPresenter(Highway &highway, GLFWwindow *window, ScreenMapper *screenMapper):
        highway(highway),
        window(window),
        screenMapper(screenMapper) {
    ImGui_ImplGlfw_Init(window, true);
    resetState();
}

UIPresenter::UIPresenter(const UIPresenter &orig):
        highway(orig.highway),
        window(orig.window),
        screenMapper(orig.screenMapper) {

}

UIPresenter::~UIPresenter() {
    ImGui_ImplGlfw_Shutdown();
}

void UIPresenter::present(double dt) {
    ImGui_ImplGlfw_NewFrame();

    commandView();
    if(showStatsView) {
        statsView();
    }

    timeToStateReset -= dt;
    if(timeToStateReset < 0) {
        resetState();
    }

    highway.preferredVehicle->setTargetSpeed(accTargetSpeed / 3.6);
    highway.preferredVehicle->setTargetDistance(accTargetDistance);

    if(showDemoView)
        ImGui::ShowTestWindow(&showDemoView);
}

void UIPresenter::key_callback(int key, int scancode, int action, int mods) {
    ImGui_ImplGlFw_KeyCallback(window, key, scancode, action, mods);
}

void UIPresenter::mouse_button_callback(int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    if(ImGui::GetIO().WantCaptureMouse) return;
    if(action != GLFW_PRESS) return;

    Point cursorPos;
    glfwGetCursorPos(window, &cursorPos.x, &cursorPos.y);

    Point roadCoords = screenMapper->pixelToRoadCoordinates(cursorPos);


    if(waitingForVehiclePlacement) {
        waitingForVehiclePlacement = false;
        highway.unselectVehicle();
        highway.addVehicleAt(roadCoords.x, roadCoords.y);
        setState("Vehicle added.");
        return;
    } else {
        highway.selectVehicleAt(roadCoords.x, roadCoords.y);
    }

    if(highway.selectedVehicle != nullptr)
        setState("Vehicle selected.");
    else
        resetState();
}

void UIPresenter::render() {
    ImGui::Render();
}


void UIPresenter::commandView() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
    ImGui::Begin("Simulation command", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SliderFloat("ACC target speed", &accTargetSpeed, 0.0f, 340.0f);
    ImGui::SliderFloat("ACC target distance", &accTargetDistance, 10.0f, 200.0f);


    if (ImGui::Button("Toggle statistics window")) {
        showStatsView ^= 1;
    }
    ImGui::SameLine();
    if (ImGui::Button("Zoom In")) {
        screenMapper->zoomIn();
    }
    ImGui::SameLine();
    if (ImGui::Button("Zoom Out")) {
        screenMapper->zoomOut();
    }
    ImGui::SameLine();
    if (ImGui::Button("ImGui Demo")) {
        showDemoView ^= 1;
    }

    ImGui::Text("ACC: Change lane ");
    ImGui::SameLine();
    if (ImGui::SmallButton("left")) {
        setState("Lane change requested.");
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("right")) {
        setState("Lane change requested.");
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("randomly")) {
        setState("Lane change requested.");
    }

    ImGui::Text("Sim: Add vehicle ");
    ImGui::SameLine();
    if (ImGui::SmallButton("in front")) {
        highway.addVehicleInFrontOfPreferred();
        setState("Vehicle added.");
    }

    ImGui::SameLine();
    if (ImGui::SmallButton("anywhere")) {
        waitingForVehiclePlacement = true;
        setState("Click on the road!");
    }

    ImGui::Text("Status: %s", status.c_str());

    ImGui::End();
}

void UIPresenter::statsView() {
    ImGui::SetNextWindowPos(ImVec2(420, 10), ImGuiSetCond_Once);
    ImGui::Begin("Statistics", &showStatsView, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("FPS: %.0f (%.1f ms/frame) ",  ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Text("ACC Speed: %.0f km/h", highway.preferredVehicle->getV() * 3.6f);
    ImGui::Text("ACC Distance to next vehicle: %.0f meters", highway.preferedVehicleFrontDistance);

    ImGui::End();
}


void UIPresenter::setState(std::string statusString) {
    status = statusString;
    timeToStateReset = RESET_TIMEOUT;
}

void UIPresenter::resetState() {
    status = "OK.";
    timeToStateReset = RESET_TIMEOUT;
    waitingForVehiclePlacement = false;
}
