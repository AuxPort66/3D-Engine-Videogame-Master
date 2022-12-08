#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "DirectXTex/DirectXTex.h"
#include <assimp/material.h>
#include <GL/glew.h>

struct OptionsTexture {

    GLuint minFunction;
    GLuint magFunction;
    GLuint wrapSFunction;
    GLuint wrapTFunction;
    bool MipMap;
};

struct TextureData {

    std::string fileName = "";
    const char* format = "";
    DirectX::TexMetadata metadata;
    OptionsTexture options = { GL_LINEAR_MIPMAP_LINEAR , GL_LINEAR, GL_REPEAT, GL_REPEAT, true };
    unsigned texture = NULL;
};


class ModuleTexture :
    public Module
{
public:

    bool Init();
    update_status Update();
    bool CleanUp();

    bool Load(TextureData& textureData);

    DirectX::TexMetadata GetMetaData();

private:
    DirectX::ScratchImage* imageTexture;
};
