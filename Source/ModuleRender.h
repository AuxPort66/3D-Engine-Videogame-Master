#pragma once
#include "Module.h"
#include "ModuleProgram.h"
#include "Globals.h"
#include "debugdraw.h"
#include "MathGeoLib.h"

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
	void WindowResized(unsigned width, unsigned height);

	void CreateTriangleVBO();

	void DestroyVBO(unsigned vbo);
	void RenderVBO(unsigned vbo);

	inline void* getContext() { return context; }

	inline float3 GetBackgroundColor() { return backgroundColor; }
	inline void SetBackgroundColor(float3 newColor) { backgroundColor = newColor; }

	inline float3 GetGridColor() { return gridColor; }
	inline void SetGridColor(float3 newColor) { gridColor = newColor; }

private:
	void* context;
	unsigned program;
	unsigned vbo;

	float3 backgroundColor = {0.1f,0.1f,0.1f};
	float3 gridColor = dd::colors::Gray;
};
