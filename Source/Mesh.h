#pragma once
#include <vector>
#include <MathGeoLib.h>
#include "ModuleTexture.h"
#include <assimp/mesh.h>
class Mesh
{
public:

	Mesh(const aiMesh* mesh);

	void LoadVBO(const aiMesh* mesh);

	void LoadEBO(const aiMesh* mesh);

	void CreateVAO();

	void Draw();

	void CleanUp();

	inline void setMaterialIndex(int mi) { material_index = mi; }
	inline int GetNumVertices() { return num_vertices; }
	inline int GetNumTriangles() { return num_triangles; }
	inline std::string GetName() { return name; }

	float3 center;
	float3 max;
	float3 min;

private:
	unsigned vbo;
	unsigned vao;
	unsigned ebo;
	unsigned int num_vertices;
	unsigned int num_triangles;
	unsigned int num_indices;
	std::string name;
	unsigned int material_index;

};

