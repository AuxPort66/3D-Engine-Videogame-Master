#pragma once
#include "Module.h"
#include "ModuleProgram.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "debugdraw.h"
#include "Model.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void DestroyVBO(unsigned vbo);
	void LoadModel(const char* fileName);
	void RenderDebugDraw();

	inline void* getContext() { return context; }

	inline Model* GetModel() { return model; }

	inline float3 GetBackgroundColor() { return backgroundColor; }
	inline void SetBackgroundColor(float3 newColor) { backgroundColor = newColor; }

	inline float3 GetGridColor() { return gridColor; }
	inline void SetGridColor(float3 newColor) { gridColor = newColor; }


public:
	SDL_Renderer* renderer;

private:
	void* context;
	unsigned program;

	float3 backgroundColor = {0.1f,0.1f,0.1f};
	float3 gridColor = dd::colors::Gray;
	Model* model;

	std::string modelName = "BakerHouse.fbx";
};
