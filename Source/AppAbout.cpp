#include "AppAbout.h"
#include "ModuleWindow.h"
#include "Application.h"
#include <stdio.h>
#include <string>


AppAbout::AppAbout()
{
}

AppAbout::~AppAbout()
{
}

void AppAbout::Init() {

}

void AppAbout::Clear() {
}

void AppAbout::Draw(const char* title, bool* p_open) {

    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }

	ImGui::Text(TITLE);
	ImGui::Text("Developed by Sara Izquierdo Elfau.");
	ImGui::Separator();

	ImGui::Text("The libraries used:");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), " GLEW");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), " SDL");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), " DirectXTex");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), " ImGui");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), " MathGeoLib");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), " DebugDraw");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), " Assimp");
	ImGui::Separator();

	ImGui::Text("2022 Copyright(c) Sara Izquierdo Elfau");
	ImGui::Separator();


    ImGui::End();

}
