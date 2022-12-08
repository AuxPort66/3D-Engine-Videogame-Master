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

	MovementCameraKey();
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

void ModuleCamera::SetPosition(float3 pos) {
	frustum.SetPos(pos);
}

void ModuleCamera::SetFOV(float newhorizontalFov)
{
	horizontalFov = newhorizontalFov;
	frustum.SetHorizontalFovAndAspectRatio(newhorizontalFov, aspect);
}

void ModuleCamera::SetAspectRatio(float newAspect)
{
	aspect = newAspect;
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), newAspect);
}

void ModuleCamera::AddPosition(float3 pos)
{
	frustum.SetPos(frustum.Pos() + pos);
}

void ModuleCamera::RotateCamera(float3 direction, float angleRadiants) {
	float3x3 rotationDeltaMatrix;
	rotationDeltaMatrix = rotationDeltaMatrix.RotateAxisAngle(direction, angleRadiants);

	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationDeltaMatrix.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationDeltaMatrix.MulDir(oldUp));
}

void ModuleCamera::LookAt(float3& point)
{
	float3 look_direction = point - frustum.Pos();

	float3x3 matrix = float3x3::LookAt(frustum.Front(), look_direction.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(matrix.MulDir(frustum.Up()).Normalized());
}

void ModuleCamera::MovementCameraKey()
{
	float speedMov = 0.001f;
	float speedRot = 0.02f;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		AddPosition(float3(frustum.Front() * speedMov));
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		AddPosition(float3(-frustum.Front() * speedMov));
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		AddPosition(float3(frustum.WorldRight() * -speedMov));
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		AddPosition(float3(frustum.WorldRight() * speedMov));
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
		AddPosition(float3(0.0f, -speedMov, 0.0f));
	}
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		AddPosition(float3(0.0f, speedMov, 0.0f));
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		RotateCamera(frustum.WorldRight(), speedRot * (pi / 180));
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		RotateCamera(-frustum.WorldRight(), speedRot * (pi / 180));
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		RotateCamera(-float3::unitY, speedRot * (pi / 180));
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		RotateCamera(float3::unitY, speedRot * (pi / 180));
	}	
	
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT) {
		speedMov += speedMov; speedRot += speedRot;
	}

}