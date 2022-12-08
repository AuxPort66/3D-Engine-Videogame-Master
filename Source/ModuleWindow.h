#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	void SetFullScreen(SDL_bool fullscreen);
	void SetResizable(SDL_bool resizable);
	void SetBorderless(SDL_bool bordered);
	void SetFullDesktop(SDL_bool fullDesktop);

	void UpdateSize(int* width, int* height);

	int GetScreenWidth() const;

	int GetScreenHeight() const;

	void ResizeScreen(int width, int height);

	void UpdateBrigthness(float* brightness);

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

private:

	int width = 0;
	int height = 0;

};

#endif // __ModuleWindow_H__