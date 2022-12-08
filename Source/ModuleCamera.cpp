#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

ModuleCamera::ModuleCamera()
{
}

// Destructor
ModuleCamera::~ModuleCamera()
{
}

// Called before render is available
bool ModuleCamera::Init()
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(App->window->window, &width, &height);

	aspect = float(width / height);
	verticalFov = pi / 4.0f;
	horizontalFov = (pi / 180) * 90.0f;
	nearPlaneDistance = 0.1f;
	farPlaneDistance = 100.0f;

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3::zero);
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetHorizontalFovAndAspectRatio(horizontalFov, aspect);
	frustum.SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);

	return true;
}

update_status ModuleCamera::Update() {
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return false;
}

float4x4 ModuleCamera::GetProjectionMatrix()
{
	return frustum.ProjectionMatrix();
}

float4x4 ModuleCamera::GetViewMatrix()
{
	return frustum.ViewMatrix();
}