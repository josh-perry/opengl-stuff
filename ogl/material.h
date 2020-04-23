#pragma once
#include <GL\glew.h>
#include <gl\GL.h>

struct Material
{
	GLuint frag_shader;

	GLuint vertex_shader;

	GLuint shader_program;
};

Material create_material(const char* vertex_src, const char* frag_src)
{
	auto mat = Material();

	mat.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(mat.vertex_shader, 1, &vertex_src, NULL);
	glCompileShader(mat.vertex_shader);

	mat.frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(mat.frag_shader, 1, &frag_src, NULL);
	glCompileShader(mat.frag_shader);

	mat.shader_program = glCreateProgram();
	glAttachShader(mat.shader_program, mat.vertex_shader);
	glAttachShader(mat.shader_program, mat.frag_shader);

	glLinkProgram(mat.shader_program);

	return mat;
}