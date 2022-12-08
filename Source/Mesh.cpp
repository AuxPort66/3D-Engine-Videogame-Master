#include "Mesh.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleTexture.h"


Mesh::Mesh(const aiMesh* mesh) {
	LoadVBO(mesh);
	LoadEBO(mesh);
	material_index = mesh->mMaterialIndex;
	CreateVAO();
}


void Mesh::LoadVBO(const aiMesh* mesh)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices;
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);
	unsigned uv_offset = position_size;
	unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices;
	float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));

	float3 maxPoint = { 0,0,0 };
	float3 minPoint = { 0,0,0 };

	if (mesh->mNumVertices > 0) {
		maxPoint = float3(mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z);
		minPoint = maxPoint;
	}

	for (unsigned i = 1; i < mesh->mNumVertices; ++i) {

		if (maxPoint.x < mesh->mVertices[i].x) maxPoint.x = mesh->mVertices[i].x;
		if (maxPoint.y < mesh->mVertices[i].y) maxPoint.y = mesh->mVertices[i].y;
		if (maxPoint.z < mesh->mVertices[i].z) maxPoint.z = mesh->mVertices[i].z;

		if (minPoint.x > mesh->mVertices[i].x) minPoint.x = mesh->mVertices[i].x;
		if (minPoint.y > mesh->mVertices[i].y) minPoint.y = mesh->mVertices[i].y;
		if (minPoint.z > mesh->mVertices[i].z) minPoint.z = mesh->mVertices[i].z;
	}

	max = maxPoint;
	min = minPoint;
	center = (maxPoint + minPoint) / 2;

	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	num_vertices = mesh->mNumVertices;
	num_triangles = mesh->mNumFaces;
	name = mesh->mName.C_Str();
}

void Mesh::LoadEBO(const aiMesh* mesh)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);
	unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	num_indices = mesh->mNumFaces * 3;
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * num_vertices));
}

void Mesh::Draw(const std::vector<TextureData>& model_textures)
{
	unsigned program = App->program->program;
	const float4x4& view = App->camera->GetViewMatrix();
	const float4x4& proj = App->camera->GetProjectionMatrix();
	float4x4 model = App->renderer->GetModel()->GetmatrixModel();
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	glActiveTexture(GL_TEXTURE0);
	if (model_textures.size() > 0)
	{
		glBindTexture(GL_TEXTURE_2D, model_textures[material_index].texture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, nullptr);
}

void Mesh::CleanUp()
{
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
}