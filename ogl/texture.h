#pragma once
#include <GL\glew.h>

enum TextureType
{
	DIFFUSE,
	SPECULAR
};

struct Texture
{
	GLuint id;
	TextureType Type;
};
