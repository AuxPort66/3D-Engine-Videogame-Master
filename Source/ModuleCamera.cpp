#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Model.h"

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

	frustum = new Frustum();
	frustum->SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum->SetPos(float3::zero);
	frustum->SetFront(-float3::unitZ);
	frustum->SetUp(float3::unitY);

	frustum->SetHorizontalFovAndAspectRatio(horizontalFov, aspect);
	frustum->SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);

	return true;
}

update_status ModuleCamera::Update() {
	MovementCameraKey();
	MovementCameraMouse();
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	delete frustum;
	return false;
}

float4x4 ModuleCamera::GetProjectionMatrix()
{
	return frustum->ProjectionMatrix();
}

float4x4 ModuleCamera::GetViewMatrix()
{
	return frustum->ViewMatrix();
}

void ModuleCamera::SetPosition(float3 pos) {
	frustum->SetPos(pos);
}

void ModuleCamera::SetFOV(float newhorizontalFov)
{
	horizontalFov = newhorizontalFov;
	frustum->SetHorizontalFovAndAspectRatio(newhorizontalFov, aspect);
}

void ModuleCamera::SetAspectRatio(float newAspect)
{
	aspect = newAspect;
	frustum->SetHorizontalFovAndAspectRatio(frustum->HorizontalFov(), newAspect);
}

void ModuleCamera::AddPosition(float3 pos)
{
	frustum->SetPos(frustum->Pos() + pos);
}

void ModuleCamera::RotateCamera(float3 direction, float angleRadiants) {
	float3x3 rotationDeltaMatrix;
	rotationDeltaMatrix = rotationDeltaMatrix.RotateAxisAngle(direction, angleRadiants);

	vec oldFront = frustum->Front().Normalized();
	frustum->SetFront(rotationDeltaMatrix.MulDir(oldFront));
	vec oldUp = frustum->Up().Normalized();
	frustum->SetUp(rotationDeltaMatrix.MulDir(oldUp));
}

void ModuleCamera::Orbit(int x, int y) {

	float3 reference = App->renderer->GetModel()->GetCenterPoint();
	float3 direction = frustum->Pos() - reference;

	Quat quat_y(frustum->Up(), x * 0.003);
	Quat quat_x(frustum->WorldRight(), y * 0.003);

	direction = quat_x.Transform(direction);
	direction = quat_y.Transform(direction);

	frustum->SetPos(direction + reference);
	LookAt(reference);
}

void ModuleCamera::LookAt(float3& point)
{
	float3 look_direction = point - frustum->Pos();

	float3x3 matrix = float3x3::LookAt(frustum->Front(), look_direction.Normalized(), frustum->Up(), float3::unitY);

	frustum->SetFront(matrix.MulDir(frustum->Front()).Normalized());
	frustum->SetUp(matrix.MulDir(frustum->Up()).Normalized());
}

void ModuleCamera::MovementCameraKey()
{
	float speedMov = 0.05f;
	float speedRot = 0.5f;



	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT) {
		speedMov += speedMov; speedRot += speedRot;
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {

		float3 newPos = App->renderer->GetModel()->GetInitVisionPos();
		float3 reference = App->renderer->GetModel()->GetCenterPoint();
		if(App->input->GetKey(SDL_SCANCODE_LALT)) frustum->SetPos(newPos);
		LookAt(reference);
	}
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		//Rotation Camera Mouse Mode
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			AddPosition(float3(frustum->Front() * speedMov));
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			AddPosition(float3(-frustum->Front() * speedMov));
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			AddPosition(float3(frustum->WorldRight() * -speedMov));
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			AddPosition(float3(frustum->WorldRight() * speedMov));
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
			AddPosition(float3(0.0f, -speedMov, 0.0f));
		}
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
			AddPosition(float3(0.0f, speedMov, 0.0f));
		}
	}
	else {
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
			RotateCamera(frustum->WorldRight(), speedRot * (pi / 180));
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			RotateCamera(-frustum->WorldRight(), speedRot * (pi / 180));
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			RotateCamera(-float3::unitY, speedRot * (pi / 180));
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			RotateCamera(float3::unitY, speedRot * (pi / 180));
		}
	}


}

void ModuleCamera::MovementCameraMouse(){
	int motion_x, motion_y;
	bool ret = false;
	motion_x = -App->input->GetMouseXMotion();
	motion_y = App->input->GetMouseYMotion();

	float speedRot = 0.003f;
	float speedMov = 0.01f;
	float speedZoom = 0.25f;

	float3x3 rotationDeltaMatrix = float3x3::identity;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT) {
		speedRot += speedRot;
		speedMov += speedMov;
		speedZoom += speedZoom;
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && (motion_x != 0 || motion_y != 0))
	{
		RotateCamera(float3::unitY, motion_x * speedRot);
		RotateCamera(frustum->WorldRight(), -motion_y * speedRot);
	}
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && (motion_x != 0 || motion_y != 0)) {
		Orbit(motion_x, -motion_y);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT  && (motion_x != 0 || motion_y != 0))
	{
		AddPosition(float3(0.0f, motion_y * speedMov, 0.0f) + (frustum->WorldRight() * motion_x * speedMov));
	}

	int wheel = App->input->GetMouseZ();
	if (wheel != 0)
	{
		AddPosition(float3(frustum->Front() * wheel * speedZoom));
	}

}
