#pragma once
#include "imgui.h"
class AppConfiguration
{
public:
	AppConfiguration();
	~AppConfiguration();

	void Init();
	void Clear();
	void Draw(const char* title, bool* p_open = NULL);

	void UpdateGraphs(float newFPS, float newMS);

private:
	ImVector<float> fps_log;

	float brightness;
	int width, height;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullDesktop = false;
};

