#include "AppConsole.h"
#include <algorithm>

AppConsole::AppConsole() {
	AutoScroll = true;
	Clear();
}

AppConsole::~AppConsole()
{
}

void AppConsole::Clear() {
	Buf.clear();
	LineOffsets.clear();
	LineType.clear();
	selectionActivesTypes[0] = true;
	selectionActivesTypes[1] = true;
	selectionActivesTypes[2] = true;
}

void AppConsole::Error(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	AddLog(LogType::Error_Type,fmt, args);
	va_end(args);
}

void AppConsole::Debug(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	AddLog(LogType::Debug_Type, fmt, args);
	va_end(args);
}

void AppConsole::Warning(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	AddLog(LogType::Warning_Type, fmt, args);
	va_end(args);
}

void AppConsole::AddLog(LogType type,const char* fmt,...){
	int old_size = Buf.size();
	va_list args;
	va_start(args, fmt);
	Buf.appendfv(fmt,args);
	va_end(args);
	for (int new_size = Buf.size(); old_size < new_size; old_size++) {
		bool isLastCharacterJumpLine = Buf[old_size] == '\n';
		if (isLastCharacterJumpLine) {
			LineType.push_back(type);
			LineOffsets.push_back(old_size + 1);
		}
	}
}

void AppConsole::Draw(const char* title, bool* p_open) {
	
	ImGui::SetNextWindowSize(ImVec2(500, 700), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(title, p_open)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginPopup("Options")) {
		ImGui::Checkbox("Auto-scroll", &AutoScroll);
		ImGui::EndPopup();
	}

	if (ImGui::Button("Options")) {
		ImGui::OpenPopup("Options");
	}
	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");

	Filter.Draw("Filter", -100.0f);

	if (ImGui::CollapsingHeader("Filters")) {
		ImGui::Spacing();
		ImGui::SameLine(0.0f, 5.0f);

		ImVec4 color_type = getColor(LogType::Debug_Type);

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(color_type.x - 0.4f, color_type.y - 0.4f, color_type.z - 0.4f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(color_type.x - 0.2f, color_type.y - 0.2f, color_type.z - 0.2f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(color_type.x, color_type.y, color_type.z, 0.8f));
		if (ImGui::Selectable("Debug", &selectionActivesTypes[0], 0, ImVec2(35, 15))) { Filter.Clear(); }
		ImGui::PopStyleColor(3);

		ImGui::SameLine(0.0f, 15.0f);

		color_type = getColor(LogType::Warning_Type);

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(color_type.x - 0.4f, color_type.y - 0.4f, color_type.z - 0.4f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(color_type.x - 0.2f, color_type.y - 0.2f, color_type.z - 0.2f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(color_type.x, color_type.y, color_type.z, 0.8f));
		if (ImGui::Selectable("Warning", &selectionActivesTypes[1], 0, ImVec2(50, 15))) { Filter.Clear(); }
		ImGui::PopStyleColor(3);

		ImGui::SameLine(0.0f, 15.0f);

		color_type = getColor(LogType::Error_Type);

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(color_type.x - 0.4f, color_type.y - 0.4f, color_type.z - 0.4f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(color_type.x - 0.2f, color_type.y - 0.2f, color_type.z - 0.2f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(color_type.x, color_type.y, color_type.z, 0.8f));
		if (ImGui::Selectable("Error", &selectionActivesTypes[2], 0, ImVec2(35, 15))) { Filter.Clear(); }
		ImGui::PopStyleColor(3);

		ImGui::Separator();

	}
	

	if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
		
		if (clear) 
			Clear();

		if (copy)
			ImGui::LogToClipboard();

		if (Filter.IsActive())
		{
			const char* buf_begin = Buf.begin();
			const char* line = buf_begin;
			for (int actual_line = 0; line != NULL; actual_line++)
			{
				const char* line_end = (actual_line + 1 < LineOffsets.Size) ? buf_begin + (LineOffsets[actual_line + 1] - 1) : NULL;
				if (Filter.PassFilter(line, line_end))
					ImGui::TextUnformatted(line, line_end);
				line = line_end && line_end[1] ? line_end + 1 : NULL;
			}
		}
		else if (LineOffsets.Size > 1) {
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size);
			while (clipper.Step()) {
				for (int actual_line = clipper.DisplayStart; actual_line < clipper.DisplayEnd; actual_line++) {
					const char* line_start = Buf.begin() + LineOffsets[actual_line];
					const char* line_end = (actual_line + 1 < LineOffsets.Size) ? (Buf.begin() + LineOffsets[actual_line + 1] - 1) : Buf.end();
					bool isLine = line_start != line_end;
					if (isLine  && selectionActivesTypes[(int)LineType[actual_line]]) {
						ImGui::PushStyleColor(ImGuiCol_Text, getColor(LineType[actual_line]));
						ImGui::TextUnformatted(line_start, line_end);
						ImGui::PopStyleColor();
					}
				}
			}
			clipper.End();
			ImGui::PopStyleVar();
		}

		if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
		ImGui::EndChild();
		ImGui::End();
	}
}

ImVec4 AppConsole::getColor(LogType type)
{
	switch (type)
	{
	case LogType::Error_Type:
		return ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
	case LogType::Warning_Type:
		return ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	case LogType::Debug_Type:
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	default:
		return ImVec4(0.1f, 1.0f, 0.4f, 1.0f);
	}
}
