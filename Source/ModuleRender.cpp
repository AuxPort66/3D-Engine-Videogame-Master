#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "SDL.h"
#include "GL/glew.h"


ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

bool ModuleRender::Init()
{
	//LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	context = SDL_GL_CreateContext(App->window->window);

	SDL_GL_MakeCurrent(App->window->window, context);
	SDL_GL_SetSwapInterval(0);


	GLenum err = glewInit();

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	program = App->program->CreateProgram();
	CreateTriangleVBO();
	InitFrustum();

	return true;
}

update_status ModuleRender::PreUpdate()
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(App->window->window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	RenderVBO(vbo);
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	DestroyVBO(vbo);
	SDL_GL_DeleteContext(context);
	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
}

void ModuleRender::CreateTriangleVBO()
{
	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

}

void ModuleRender::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

void ModuleRender::RenderVBO(unsigned vbo)
{
	float4x4 model, view, proj;

	view = frustum.ViewMatrix();
	proj = frustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3::zero,
		float4x4::zero,
		float3::one
	);

	glUseProgram(program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ModuleRender::InitFrustum() {

	int width = 0;
	int height = 0;
	SDL_GetWindowSize(App->window->window, &width, &height);

	float aspect = float(width / height);
	float verticalFov = pi / 4.0f;
	float horizontalFov = (pi / 180) * 90.0f;
	float nearPlaneDistance = 0.1f;
	float farPlaneDistance = 100.0f;
	
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3::zero);
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetHorizontalFovAndAspectRatio(horizontalFov, aspect);
	frustum.SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);


}

