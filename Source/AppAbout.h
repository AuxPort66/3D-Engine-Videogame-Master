#pragma once
#include "imgui.h"
class AppAbout
{
public:
	AppAbout();
	~AppAbout();

	void Init();
	void Clear();
	void Draw(const char* title, bool* p_open = NULL);

private:
};

