#include "AppConfiguration.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Application.h"
#include <string>
#include <stdio.h>


AppConfiguration::AppConfiguration()
{
}

// Destructor
AppConfiguration::~AppConfiguration()
{
}

void AppConfiguration::Init() {
    brightness = SDL_GetWindowBrightness(App->window->window);
    resizable = RESIZABLE;
    fullscreen = FULLSCREEN;
    fullDesktop = DESKTOP_FULLSCREEN;
    borderless = BORDERLESS;
}

void AppConfiguration::Clear() {
    fps_log.clear();
}

void AppConfiguration::UpdateGraphs(float newFPS, float newMS) {

    if (fps_log.size() >= 100) {
        fps_log.erase(fps_log.begin());
    }
    fps_log.push_back(newFPS);
}

void AppConfiguration::Draw(const char* title, bool* p_open) {
    

    ImGui::SetNextWindowSize(ImVec2(500, 700), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }
    
    if (ImGui::CollapsingHeader("Application")) {

        char title[25];
        sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
        ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

        SDL_version version;
        SDL_GetVersion(&version);

        ImGui::Text("SDL Version: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%i.%i.%i", version.major, version.minor, version.patch);
        ImGui::Separator();

        ImGui::Text("CPUs: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%i (Cache: %ikb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

        ImGui::Text("System Ram: ");
        ImGui::SameLine();
        double gb = SDL_GetSystemRAM() * 0.001048576;
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%lfGb", gb);

        ImGui::Text("Caps: ");
        ImGui::SameLine();
        std::string text = "";
        if (SDL_HasRDTSC) text += "RDTSC,";
        if (SDL_HasMMX) text += "MMX,";
        if (SDL_HasSSE) text += "SSE,";
        if (SDL_HasSSE2) text += "SSE2,";
        if (SDL_HasSSE3) text += "SSE3,";
        if (SDL_HasSSE41) text += "SSE41,";
        if (SDL_HasAVX) text += "AVX,";
        if (SDL_HasAVX2) text += "AVX2,";
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), &text[0]);


        ImGui::Separator();
        ImGui::Text("GPU: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%i (Cache: %ikb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
    }

    if (ImGui::CollapsingHeader("Window")) {

        int minWidth, minHeight;
        int maxWidth, maxHeight;
        SDL_GetWindowMinimumSize(App->window->window, &minWidth, &minHeight);
        SDL_GetWindowMaximumSize(App->window->window, &maxWidth, &maxHeight);

        SDL_GetWindowSize(App->window->window, &width, &height);
        if (ImGui::SliderFloat("Brightness", &brightness, 0.000, 1.000, NULL)) App->window->UpdateBrigthness(&brightness);
        if (ImGui::SliderInt("Widht", &width, minWidth, maxWidth, NULL)) App->window->UpdateSize(&width, &height);
        if (ImGui::SliderInt("Height", &height, minHeight, maxHeight, NULL)) App->window->UpdateSize(&width, &height);

        if (ImGui::Checkbox("FullScreen", &fullscreen)) App->window->SetFullScreen((SDL_bool)fullscreen);
        ImGui::SameLine();
        if (ImGui::Checkbox("Resizable", &resizable)) App->window->SetResizable((SDL_bool)resizable);

        if (ImGui::Checkbox("Borderless", &borderless)) App->window->SetBorderless((SDL_bool)!borderless);
        ImGui::SameLine();
        if (ImGui::Checkbox("Full Desktop", &fullDesktop)) App->window->SetFullDesktop((SDL_bool)fullDesktop);
    }

    if (ImGui::CollapsingHeader("Render")) {

        static float backgroundColor[3] = { App->renderer->GetBackgroundColor().x, App->renderer->GetBackgroundColor().y, App->renderer->GetBackgroundColor().z };
        if (ImGui::ColorEdit3("Background Color", (float*)&backgroundColor, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoAlpha)) {
            App->renderer->SetBackgroundColor(float3(backgroundColor[0], backgroundColor[1], backgroundColor[2]));
        }

        static float gridColor[3] = { App->renderer->GetGridColor().x, App->renderer->GetGridColor().y, App->renderer->GetGridColor().z };
        if (ImGui::ColorEdit3("Grid Color", (float*)&gridColor, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoAlpha)) {
            App->renderer->SetGridColor(float3(gridColor[0], gridColor[1], gridColor[2]));
        }

    }
    


    ImGui::End();

}
