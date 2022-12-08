#pragma once
#include "Module.h"
#include "imgui.h"
#include "AppConsole.h"
#include "AppProperties.h"
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

	bool ShowMenuBar();

public:

	AppConsole* log = nullptr;
	AppProperties* propertiesApp = nullptr;

private:

	bool show_app_console = false;
	bool show_app_properties = false;

	ImGuiContext* context;
	ImGuiIO io;

};

