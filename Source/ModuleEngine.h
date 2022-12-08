#pragma once
#include "Module.h"
#include "imgui.h"
#include "AppConsole.h"
#include "AppConfiguration.h"
#include "AppProperties.h"
#include <vector>
#include "AppAbout.h"

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
	bool ShowMenuBar();
	update_status PostUpdate();
	bool CleanUp();

public:

	AppConsole* log = nullptr;
	AppConfiguration* configApp = nullptr;
	AppProperties* propertiesApp = nullptr;
	AppAbout* aboutApp = nullptr;

private:
	ImGuiContext* context;
	ImGuiIO io;
	bool show_app_about = false;
	bool show_app_configuration = false;
	bool show_app_console = false;
	bool show_app_hardware = false;
	bool show_app_properties = false;

};

