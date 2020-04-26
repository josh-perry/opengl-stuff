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

	// Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		// Assimp allows up to 8 textures per vertex: only get the first one for now
		const int uvChannel = 0;

		if (mesh->mTextureCoords[uvChannel])
		{
			vertex.texture_coords = glm::vec2(mesh->mTextureCoords[uvChannel][i].x, mesh->mTextureCoords[uvChannel][i].y);
		}
		else
		{
			// No texture coords: just zero them
			vertex.texture_coords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// Faces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Materials
	if (mesh->mMaterialIndex >= 0)
	{
		// Get material from scene
		//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//get_textures_for_material(material, DIFFUSE);
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
	for (unsigned int i = 0; i < model.meshes.size(); i++)
	{
		draw_mesh(model.meshes[i]);
	}
}
