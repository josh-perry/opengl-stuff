#pragma once
#include <GL\glew.h>
#include <assimp\scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

enum class TextureType
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
		case TextureType::DIFFUSE:
			return aiTextureType_DIFFUSE;
		case TextureType::SPECULAR:
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

GLuint load_texture(Texture texture)
{
	// First: try to load image
	int width;
	int height;
	int channels;
	int desiredChannels = 0; // ?

	unsigned char* data = stbi_load(("Resources/" + texture.path).c_str(), &width, &height, &channels, desiredChannels);

	if (!data)
	{
		printf("Image is fucked dude\n");
		return -1;
	}

	GLuint texture_id;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return texture_id;
}