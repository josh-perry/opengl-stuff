#pragma once
#include <GL\glew.h>
#include <gl\GL.h>
#include <vector>

struct Material
{
	GLuint frag_shader;

	GLuint vertex_shader;

	GLuint shader_program;
};

bool try_compile_shader(GLuint shader)
{
	glCompileShader(shader);

	GLint compiled = 0;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(shader);
		return true;
	}

	return false;
}

Material create_material(const char* vertex_src, const char* frag_src)
{
	auto mat = Material();

	mat.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(mat.vertex_shader, 1, &vertex_src, NULL);
	try_compile_shader(mat.vertex_shader);

	mat.frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(mat.frag_shader, 1, &frag_src, NULL);
	try_compile_shader(mat.frag_shader);

	mat.shader_program = glCreateProgram();
	glAttachShader(mat.shader_program, mat.vertex_shader);
	glAttachShader(mat.shader_program, mat.frag_shader);

	glLinkProgram(mat.shader_program);

	return mat;
}