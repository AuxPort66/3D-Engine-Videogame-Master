#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_SCREEN_WIDTH 1920
#define MAX_SCREEN_HEIGHT 1080
#define MIN_SCREEN_WIDTH 640
#define MIN_SCREEN_HEIGHT 480
#define FULLSCREEN false
#define BORDERLESS false
#define RESIZABLE false
#define DESKTOP_FULLSCREEN false
#define VSYNC true
#define TITLE "Tiembla Unreal"