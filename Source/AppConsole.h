#pragma once
#include "imgui.h"
class AppConsole
{
public:
	AppConsole();
	~AppConsole();

	enum class LogType { Debug_Type, Warning_Type, Error_Type, System_Type};

	void Clear();
	void Error(const char* fmt, ...)IM_FMTARGS(2);
	void Debug(const char* fmt, ...)IM_FMTARGS(2);
	void Warning(const char* fmt, ...)IM_FMTARGS(2);
	void AddLog(LogType type, const char* fmt, ...);
	void Draw(const char* title, bool* p_open = NULL);



private:
	ImVec4 getColor(LogType type);


	ImGuiTextBuffer Buf;
	ImVector<int> LineOffsets;
	ImVector<LogType> LineType;

	ImGuiTextFilter Filter;

	bool selectionActivesTypes[3] = {true, true, true};
	bool AutoScroll;

};


