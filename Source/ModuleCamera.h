#pragma once
#include "Module.h"
#include "MathGeoLib.h"
#include "SDL/include/SDL.h"
class ModuleCamera :
    public Module
{
public:
    ModuleCamera();
    ~ModuleCamera();

    bool Init();
    update_status Update();
    bool CleanUp();

    float4x4 GetProjectionMatrix();
    float4x4 GetViewMatrix();

    void SetFOV(float newhorizontalFov);
    void SetAspectRatio(float newAspect);
    void SetPosition(float3 pos);
    void AddPosition(float3 pos);
    void LookAt(float3& point);
    void MovementCameraKey();
    void RotateCamera(float3 direction, float angleRadiants);


private:
    Frustum frustum;
    float nearPlaneDistance;
    float farPlaneDistance;
    float verticalFov;
    float horizontalFov;
    float aspect;
};
