#include "AppProperties.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleRender.h"
#include "Model.h"
#include "Mesh.h"
#include <stdio.h>
#include <string>


AppProperties::AppProperties()
{
}

// Destructor
AppProperties::~AppProperties()
{
}

void AppProperties::Init() {

}

void AppProperties::Clear() {
}

void AppProperties::Draw(const char* title, bool* p_open) {

    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("##TabBar"))
    {
        Model* model = App->renderer->GetModel();
        if (ImGui::BeginTabItem("Transformation"))
        {
            float pos[3] = { model->GetPos().x, model->GetPos().y, model->GetPos().z };
            if (ImGui::DragFloat3("Position", pos, 0.01f)) { App->renderer->GetModel()->SetPos(float3(pos[0], pos[1], pos[2])); };

            float rot[3] = { model->GetRot().x, model->GetRot().y, model->GetRot().z };
            if (ImGui::DragFloat3("Rotation", rot, 0.01f)) { App->renderer->GetModel()->SetRot(float3(rot[0], rot[1], rot[2])); };

            float scale[3] = { model->GetScale().x, model->GetScale().y, model->GetScale().z };
            if (ImGui::DragFloat3("Scale", scale, 0.01f)) { App->renderer->GetModel()->SetScale(float3(scale[0], scale[1], scale[2])); };

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Geometry"))
        {
            ImGui::Text("Number of meshes:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%i", model->GetMeshes().size());
            ImGui::Separator();

            for (int i = 0; i < model->GetMeshes().size(); i++)
            {
                std::string SubTittle = "Mesh " + std::to_string(i + 1) + ": " + model->GetMeshes()[i].GetName();

                if (ImGui::TreeNode(SubTittle.c_str()))
                {
                    ImGui::Text("Number of Vertices:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%i", model->GetMeshes()[i].GetNumVertices());
                    ImGui::SameLine();
                    ImGui::Text("Number of triangles:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%i", model->GetMeshes()[i].GetNumTriangles());
                    ImGui::TreePop();
                }
                ImGui::Separator();
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Texture"))
        {
            ImGui::Text("Number of textures:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%i", model->GetTextures().size());
            ImGui::Separator();

            unsigned int itemsEquivalentFilter[] = { GL_NEAREST , GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR };
            const char* itemsMin[] = { "GL_NEAREST", "GL_LINEAR", "GL_NEAREST_MIPMAP_NEAREST", "GL_LINEAR_MIPMAP_NEAREST", "GL_NEAREST_MIPMAP_LINEAR", "GL_LINEAR_MIPMAP_LINEAR" };
            const char* itemsMag[] = { "GL_NEAREST", "GL_LINEAR" };
            unsigned int itemsEquivalentWrap[] = { GL_REPEAT , GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE };
            const char* itemsWrap[] = { "GL_REPEAT", "GL_CLAMP_TO_EDGE", "GL_CLAMP_TO_BORDER", "GL_MIRRORED_REPEAT", "GL_MIRROR_CLAMP_TO_EDGE" };

            for (int i = 0; i < model->GetTextures().size(); i++)
            {
                std::string SubTittle = "Texture " + std::to_string(i + 1) + ": " + std::string(model->GetTextures()[i].fileName);

                if (ImGui::TreeNode(SubTittle.c_str()))
                {
                    ImGui::Text("   Id:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", model->GetTextures()[i].texture);
                    ImGui::SameLine();
                    ImGui::Text("   Format:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), model->GetTextures()[i].format);


                    ImGui::Text("   Width:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", model->GetTextures()[i].metadata.width);
                    ImGui::SameLine();
                    ImGui::Text("   Height:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", model->GetTextures()[i].metadata.height);

                    ImGui::Text("  ");
                    ImGui::SameLine();
                    ImGui::Image((void*)(intptr_t)model->GetTextures()[i].texture, ImVec2(200, 200));

                    static int item_currentMin = 5;
                    static int item_currentMag = 1;
                    static int item_currentWrapS = 0;
                    static int item_currentWrapT = 0;

                    bool changeActivate = false;

                    if (ImGui::Combo("MIN_FILTER", &item_currentMin, itemsMin, IM_ARRAYSIZE(itemsMin))) changeActivate = true;
                    if (ImGui::Combo("MAG_FILTER", &item_currentMag, itemsMag, IM_ARRAYSIZE(itemsMag))) changeActivate = true;
                    if (ImGui::Combo("WRAP_S", &item_currentWrapS, itemsWrap, IM_ARRAYSIZE(itemsWrap))) changeActivate = true;
                    if (ImGui::Combo("WRAP_T", &item_currentWrapT, itemsWrap, IM_ARRAYSIZE(itemsWrap))) changeActivate = true;

                    bool mipMapActivate = model->GetTextures()[i].options.MipMap;
                    if (ImGui::Checkbox("MipMap", &mipMapActivate)) changeActivate = true;
                    ImGui::SameLine();

                    if (changeActivate) {
                        OptionsTexture options{
                            itemsEquivalentFilter[item_currentMin], itemsEquivalentFilter[item_currentMag],
                            itemsEquivalentWrap[item_currentWrapS], itemsEquivalentWrap[item_currentWrapT],
                            mipMapActivate
                        };
                        model->ChangeTexture(i, model->GetTextures()[i].texture, options);
                    };

                    ImGui::TreePop();
                }
                ImGui::Separator();
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();

}
