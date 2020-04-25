#pragma once
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "mesh.h"
#include <assimp\mesh.h>

struct Model
{
	std::vector<Mesh> meshes;
};

Mesh process_mesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return create_mesh(vertices, indices, textures);
}

void process_ai_node(Model* model, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		model->meshes.push_back(process_mesh(scene->mMeshes[node->mMeshes[i]], scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_ai_node(model, node->mChildren[i], scene);
	}
}

Model create_model(char* filepath)
{
	auto model = Model();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene)
	{
		printf("Something has gone wrongl!\n");
		printf(importer.GetErrorString());
	}
	else
	{
		process_ai_node(&model, scene->mRootNode, scene);
	}

	return model;
}

void draw_model(Model model)
{
	for (int i = 0; i < model.meshes.size(); i++)
	{
		draw_mesh(model.meshes[i]);
	}
}
