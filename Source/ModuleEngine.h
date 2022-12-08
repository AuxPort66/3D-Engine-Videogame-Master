#pragma once
#include "Module.h"
#include "imgui.h"
#include "AppConsole.h"
#include <vector>

class ModuleEngine :
    public Module
{
public:
	ModuleEngine();
	~ModuleEngine();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:

	AppConsole* log = nullptr;

private:
	ImGuiContext* context;
	ImGuiIO io;

};

