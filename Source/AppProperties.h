#pragma once
#include "imgui.h"
class AppProperties
{
public:
	AppProperties();
	~AppProperties();

	void Init();
	void Clear();
	void Draw(const char* title, bool* p_open = NULL);

private:
};

