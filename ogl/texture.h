#pragma once
#include <GL\glew.h>
#include <assimp\scene.h>


enum TextureType
{
	DIFFUSE,
	SPECULAR
};

struct Texture
{
	GLuint id;
	TextureType type;
	std::string path;
};

aiTextureType map_TextureType_to_aiTextureType(TextureType type)
{
	switch (type)
	{
		case DIFFUSE:
			return aiTextureType_DIFFUSE;
		case SPECULAR:
			return aiTextureType_SPECULAR;
		default:
			printf("ERROR: could not map type %i to aiTextureType!", type);
			return aiTextureType_NONE;
	}
}

std::vector<Texture> get_textures_for_material(aiMaterial *mat, TextureType type)
{
	std::vector<Texture> textures;

	aiTextureType aiType = map_TextureType_to_aiTextureType(type);

	for (unsigned int i = 0; i <= mat->GetTextureCount(aiType); i++)
	{
		// Get the texture path
		aiString path;
		mat->GetTexture(aiType, i, &path);

		// Make a texture
		Texture texture;
		texture.type = type;
		texture.path = path.C_Str();

		textures.push_back(texture);
	}

	return textures;
}
