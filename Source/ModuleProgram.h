#pragma once
#include "Module.h"
class ModuleProgram :
    public Module
{
public:
    ModuleProgram();
    ~ModuleProgram();
    char* LoadShaderSource(const char* InShader_file_name);
    int CompileShader(unsigned type, const char* source);
    unsigned CreateProgram();

public:
    unsigned int program;
    int vtx_shader;
    int frg_shader;
};

