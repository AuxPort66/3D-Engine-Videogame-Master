#include "Model.h"
#include "Application.h"
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

	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadTextures(scene->mMaterials, scene->mNumMaterials, file_name);
		LoadMeshes(scene->mMeshes, scene->mNumMeshes);
	}
	else
	{
		printf("Error Loading Model %s: %s\n", file_name, aiGetErrorString());
	}
}


void Model::LoadTextures(aiMaterial** materials, unsigned int numMaterials, const char* file_model)
{

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
}

void Model::LoadMeshes(aiMesh** meshes, unsigned int numMeshes)
{
	mMeshes.reserve(numMeshes);
	for (unsigned i = 0; i < numMeshes; ++i)
	{
		Mesh mesh(meshes[i]);
		mMeshes.push_back(mesh);
	}
	
}

void Model::DrawMeshes() {

	for (unsigned i = 0; i < mMeshes.size(); ++i) {

		mMeshes[i].Draw(mTextures);
	}
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
