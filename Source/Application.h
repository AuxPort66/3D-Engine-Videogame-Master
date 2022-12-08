#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleEngine;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleTexture;
class ModuleProgram;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleEngine* engine = nullptr;
	ModuleDebugDraw* debugDraw = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleProgram* program = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;
