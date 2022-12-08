#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

		if (FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}
		if (DESKTOP_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			SDL_DisplayMode dm;
			SDL_GetCurrentDisplayMode(0, &dm);
			SDL_SetWindowMaximumSize(window, dm.w, dm.h);
			SDL_SetWindowMinimumSize(window, MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetFullScreen(SDL_bool fullscreen)
{
	Uint32 value = 0;
	if (fullscreen) value = SDL_WINDOW_FULLSCREEN;
	SDL_SetWindowFullscreen(window, value);
}

void ModuleWindow::SetResizable(SDL_bool resizable)
{
	SDL_SetWindowResizable(window, resizable);
}

void ModuleWindow::SetBorderless(SDL_bool bordered)
{
	SDL_SetWindowBordered(window, bordered);
}

void ModuleWindow::SetFullDesktop(SDL_bool fullDesktop)
{
	Uint32 value = 0;
	if (fullDesktop) value = SDL_WINDOW_FULLSCREEN_DESKTOP;
	SDL_SetWindowFullscreen(window, value);
}

void ModuleWindow::UpdateBrigthness(float* brightness) {
	SDL_SetWindowBrightness(window, *brightness);
}

void ModuleWindow::UpdateSize(int* width, int* height) {
	SDL_SetWindowSize(window, *width, *height);
}

int ModuleWindow::GetScreenWidth() const
{
	return width;
}

int ModuleWindow::GetScreenHeight() const
{
	return height;
}

void ModuleWindow::ResizeScreen(int width, int height)
{
	this->width = width;
	this->height = height;
}
