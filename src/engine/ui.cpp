#include "./ui.hpp"

#include "engine/core.hpp"

// #include "gl/glad.h"
// #include <GL/gl.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

namespace wmac::ui {

void init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(core::m_window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    // io.Fonts->AddFontFromFileTTF("res/fonts/Miracode.ttf", 16.0f);
}

void draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

        ImGui::ShowDemoWindow();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void deinit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

}