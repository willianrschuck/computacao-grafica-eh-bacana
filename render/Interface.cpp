#include "Interface.h"

void Interface::init(GLFWwindow* window) {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

}

void Interface::render() {

    if (!ImGui::GetCurrentContext()) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    ImGui::Begin("Opções", nullptr, window_flags);
    {
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "x: %.1f y: %.1f z: %.1f",
                           cameraPostion.x(), cameraPostion.y(), cameraPostion.z());

        if (isPaused) {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", " -- PAUSADO --");
            ImGui::InputFloat("Walk Speed", walkSpeed, 0.5, 1);
            ImGui::InputFloat("Rotation Speed", rotationSpeed, 0.5, 1);
        }

    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Interface::drop() {

    if (!ImGui::GetCurrentContext()) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}

