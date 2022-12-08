 #include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleEngine.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include <imgui_impl_sdl.h>

#define MAX_KEYS 300
#define SCREEN_SIZE 1

ModuleInput::ModuleInput()
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
    App->engine->log->Debug("Init SDL input event system\n");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
        App->engine->log->Error("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}


update_status ModuleInput::PreUpdate() {

    SDL_PumpEvents();
    const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	int width = 0;
	int height = 0;
	SDL_GetWindowSize(App->window->window, &width, &height);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for (int i = 0; i < 5; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		switch (sdlEvent.type)
		{
			case SDL_QUIT:
				return UPDATE_STOP;
			case SDL_WINDOWEVENT:
				if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					App->window->ResizeScreen(sdlEvent.window.data1, sdlEvent.window.data2);
					App->camera->SetAspectRatio(float(sdlEvent.window.data1 / sdlEvent.window.data2));
				}
				break;
			case SDL_MOUSEWHEEL:
				mouse_z = sdlEvent.wheel.y;
				break;
			case SDL_MOUSEMOTION:
				mouse_x = sdlEvent.motion.x / SCREEN_SIZE;
				mouse_y = sdlEvent.motion.y / SCREEN_SIZE;

				mouse_x_motion = sdlEvent.motion.xrel / SCREEN_SIZE - last_mouse_swap;
				mouse_y_motion = sdlEvent.motion.yrel / SCREEN_SIZE;

				if (infiniteHorizontal)
				{
					if (mouse_x > App->window->GetScreenWidth() - 10)
					{
						int last_x = mouse_x;
						App->input->SetMouseX(10);
						last_mouse_swap = mouse_x - last_x;
					}
					else if (mouse_x < 10)
					{
						int last_x = mouse_x;
						App->input->SetMouseX(App->window->GetScreenWidth() - 10);
						last_mouse_swap = mouse_x - last_x;
					}
					else
						last_mouse_swap = 0;
				}
				else
				{
					last_mouse_swap = 0;
				}
				break;
		}
	}

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update()
{

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
    App->engine->log->Debug("Quitting SDL input event subsystem\n");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

KEY_STATE ModuleInput::GetKey(int id) const
{
    return keyboard[id];
}

KEY_STATE ModuleInput::GetMouseButton(int id) const
{
    return mouse_buttons[id];
}

void ModuleInput::SetMouseX(int x)
{
	SDL_WarpMouseInWindow(App->window->window, x, mouse_y);
	mouse_x = x;
}

void ModuleInput::SetMouseY(int y)
{
	SDL_WarpMouseInWindow(App->window->window, mouse_x, y);
	mouse_y = y;
}

int ModuleInput::GetMouseX() const
{
	return mouse_x;
}

int ModuleInput::GetMouseY() const
{
	return mouse_y;
}

int ModuleInput::GetMouseXMotion() const
{
	return mouse_x_motion;
}

int ModuleInput::GetMouseYMotion() const
{
	return mouse_y_motion;
}

int ModuleInput::GetMouseZ() const
{
	return mouse_z;
}
