#include "ModuleEngine.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"


ModuleEngine::ModuleEngine()
{
	log = new AppConsole();
}

// Destructor
ModuleEngine::~ModuleEngine()
{
}

bool ModuleEngine::Init() {

	context = ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	return true;
}

bool ModuleEngine::Start() {
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->getContext());
	ImGui_ImplOpenGL3_Init("#version 130");

	return true;
}

update_status ModuleEngine::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEngine::Update()
{
	return UPDATE_CONTINUE;
}


update_status ModuleEngine::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(App->window->window, App->renderer->getContext());

	return UPDATE_CONTINUE;
}

bool ModuleEngine::CleanUp() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

