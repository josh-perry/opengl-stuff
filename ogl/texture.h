#pragma once
#include <GL\glew.h>
#include <assimp\scene.h>
#include <iostream>
#include <string>

#include <fmt/format.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "log.h"

enum class TextureType
{
	DIFFUSE,
	SPECULAR,
	CUBEMAP
};

struct Texture
{
	GLuint id;
	TextureType type;
	std::string path;
};

struct ImageData
{
	int width;
	int height;
	int channels;
	unsigned char* data;
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

ImageData load_image_data(std::string path)
{
	ImageData image_data{};

	if (path == "")
	{
		log_line("Tried to load a texture path of empty string; ignoring", LogLevel::TRACE);
		return image_data;
	}

	std::string full_path = "Resources/" + path;

	unsigned char* data = stbi_load(full_path.c_str(), &image_data.width, &image_data.height, &image_data.channels, 0);

	if (!data)
	{
		log_line(fmt::format("Failed to load image at path: '{}'", full_path), LogLevel::WARN);
		return image_data;
	}

	image_data.data = data;
	log_line(fmt::format("Loaded image: '{}'", path), LogLevel::INFO);
	return image_data;
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

void load_gltexture(Texture texture)
{
	ImageData image_data = load_image_data(texture.path);

	glBindTexture(GL_TEXTURE_2D, texture.id);

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_data.width, image_data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image_data.data);
}

void load_gltexture_cubemap(Texture texture)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

	// TODO: make this better
	std::vector<std::string> cubemap_textures{};

	for (unsigned int i = 0; i <= 5; i++)
	{
		cubemap_textures.push_back(fmt::format("{}_{}", texture.path, i));
	}

	for (unsigned int i = 0; i < cubemap_textures.size(); i++)
	{
		ImageData image_data = load_image_data(cubemap_textures[i]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image_data.width, image_data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data);
		stbi_image_free(image_data.data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

GLuint load_texture(Texture texture)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);

	texture.id = texture_id;

	if (texture.type == TextureType::CUBEMAP)
	{
		load_gltexture_cubemap(texture);
	}
	else
	{
		load_gltexture(texture);
	}

	return texture_id;
}

Texture create_texture()
{
	Texture texture{};
	return texture;
}