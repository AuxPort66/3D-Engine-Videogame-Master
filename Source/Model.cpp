#include "Model.h"
#include "Application.h"
#include "ModuleEngine.h"
#include "ModuleTexture.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include <assimp/postprocess.h>
#include "assimp/material.h"
#include <GL/glew.h>

void Model::Load(const char* file_name)
{
	for (Mesh& mesh : mMeshes) {
		mesh.CleanUp();
	}
	mMeshes.clear();
	for (TextureData texture : mTextures) {
		glDeleteTextures(1, &texture.texture);
	}
	mTextures.clear();

	App->engine->log->Debug(("Load Model from: " + std::string(file_name) + "\n").c_str());

	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadTextures(scene->mMaterials, scene->mNumMaterials, file_name);
		LoadMeshes(scene->mMeshes, scene->mNumMeshes);
	}
	else
	{
		App->engine->log->Error(("Error Loading Model " +  std::string(file_name) + " " + aiGetErrorString()).c_str());
	}
}


void Model::LoadTextures(aiMaterial** materials, unsigned int numMaterials, const char* file_model)
{

	App->engine->log->Debug("Start Loading Textures\n");

	aiString file;
	mTextures.reserve(numMaterials);
	for (unsigned i = 0; i < numMaterials; ++i)
	{
		if (materials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			TextureData material;
			material.fileName = file.data;

			if (App->texture->Load(material)) {
				mTextures.push_back(material);
				continue;
			}

			material.fileName = "../Source/assets/textures/" + std::string(file.data);
			if (App->texture->Load(material))
			{
				mTextures.push_back(material);
			}
		}
	}
	App->engine->log->Debug((std::to_string(numMaterials) + "textures loaded\n").c_str());

}

void Model::ChangeTexture(int id, GLuint textureid, OptionsTexture options)
{
	TextureData material = mTextures[id];
	material.options = options;
	App->texture->Load(material);
	mTextures[id] = material;
}

void Model::LoadMeshes(aiMesh** meshes, unsigned int numMeshes)
{
	App->engine->log->Debug("Start Loading Meshes\n");

	mMeshes.reserve(numMeshes);
	for (unsigned i = 0; i < numMeshes; ++i)
	{
		Mesh mesh;
		mesh.Load(meshes[i]);
		mMeshes.push_back(mesh);
	}
	
	App->engine->log->Debug((std::to_string(numMeshes) + "meshes loaded\n").c_str());

}

void Model::DrawMeshes() {

	for (unsigned i = 0; i < mMeshes.size(); ++i) {

		mMeshes[i].Draw(mTextures);
	}
}

float3 Model::GetCenterPoint() {

	float3 center = { 0,0,0 };
	for (unsigned i = 0; i < mMeshes.size(); ++i)
	{
		center += mMeshes[i].center;
	}
	return center / (float)mMeshes.size();

}

float4x4 Model::GetmatrixModel() {

	float4x4 rotMatrix = float4x4::identity;
	rotMatrix = float4x4::RotateX(rotation.x * (pi / 180));
	rotMatrix = rotMatrix * float4x4::RotateY(rotation.y * (pi / 180));
	rotMatrix = rotMatrix * float4x4::RotateZ(rotation.z * (pi / 180));

	return float4x4::FromTRS(position,
		rotMatrix,
		scale
	);
}


const float3& Model::GetInitVisionPos()
{
	float3 actualMaxVtx, actualMinVtx;

	if (mMeshes.size() > 0)
	{
		actualMaxVtx = mMeshes[0].max;
		actualMinVtx = mMeshes[0].min;
	}
	for (int i = 1; i < mMeshes.size(); ++i)
	{
		float3 maxVtx = mMeshes[i].max;
		float3 minVtx = mMeshes[i].min;

		if (maxVtx.x > actualMaxVtx.x) actualMaxVtx.x = maxVtx.x;
		if (maxVtx.y > actualMaxVtx.y) actualMaxVtx.y = maxVtx.y;
		if (maxVtx.z > actualMaxVtx.z) actualMaxVtx.z = maxVtx.z;

		if (minVtx.x < actualMinVtx.x) actualMinVtx.x = minVtx.x;
		if (minVtx.y < actualMinVtx.y) actualMinVtx.y = minVtx.y;
		if (minVtx.z < actualMinVtx.z) actualMinVtx.z = minVtx.z;
	}

	initVisionPos.Set(actualMaxVtx.x + (actualMaxVtx.x - actualMinVtx.x) / 4,
		actualMaxVtx.y + (actualMaxVtx.y - actualMinVtx.y) / 4,
		actualMaxVtx.z + (actualMaxVtx.z - actualMinVtx.z) / 4);

	return initVisionPos;
}