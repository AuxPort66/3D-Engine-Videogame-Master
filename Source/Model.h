#pragma once
#include<vector> 
#include "ModuleTexture.h"
#include "Mesh.h"
#include "Math/float3.h"


struct aiMaterial;
struct aiMesh;

class Model
{
public:
	void Load(const char* file_name);

	void LoadTextures(aiMaterial** materials, unsigned int numMaterials, const char* file_model);

	void LoadMeshes(aiMesh** meshes, unsigned int numMeshes);

	void DrawMeshes();

	float3 GetCenterPoint();

	inline std::vector<TextureData>& GetTextures() { return mTextures; };
	inline std::vector<Mesh>& GetMeshes() { return mMeshes; };

	inline float3 GetPos() { return position; };
	inline float3 GetRot() { return rotation; };
	inline float3 GetScale() { return scale; };

	inline void SetPos(float3 pos) { position = pos; };
	inline void SetRot(float3 rot) { rotation = rot; };
	inline void SetScale(float3 sc) { scale = sc; };

	float4x4 GetmatrixModel();

	const float3& GetInitVisionPos();

private:
	std::vector<TextureData> mTextures;
	std::vector<Mesh> mMeshes;

	float3 position = float3::zero;
	float3 rotation = float3::zero;
	float3 scale = float3::one;

	float3 initVisionPos;
};

