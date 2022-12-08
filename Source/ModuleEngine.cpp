#include "ModuleEngine.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"


ModuleEngine::ModuleEngine()
{
	log = new AppConsole();
	propertiesApp = new AppProperties();
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
	if (!ShowMenuBar()) return UPDATE_STOP;

	if (show_app_console) log->Draw("Console");
	if (show_app_properties) propertiesApp->Draw("Properties");


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

bool ModuleEngine::ShowMenuBar() {
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu")) {

			if (ImGui::MenuItem("Exit")) {
				return false;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Console")) show_app_console = !show_app_console;
			if (ImGui::MenuItem("Properties")) show_app_properties = !show_app_properties;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Miscelanea"))
		{

			if (ImGui::BeginMenu("GitHub"))
			{
				if (ImGui::MenuItem("Main Page")) {
					ShellExecuteA(NULL, "open", "", NULL, NULL, SW_SHOWNORMAL);
				}
				if (ImGui::MenuItem("Documentation")) {
					ShellExecuteA(NULL, "open", "", NULL, NULL, SW_SHOWNORMAL); //readme TODO
				}
				if (ImGui::MenuItem("Download Latest")) {
					ShellExecuteA(NULL, "open", "", NULL, NULL, SW_SHOWNORMAL); //releases TODO
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return true;
}