#pragma once
#include "Module.h"
class ModuleProgram :
    public Module
{
public:
    ModuleProgram();
    ~ModuleProgram();
    unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

public:
    unsigned int program;
};

