#pragma once
#include <string>
#include <GL\glew.h>
#include <gl\GL.h>
#include <vector>
#include <fstream>

#include "log.h"

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

		std::string s(infoLog.begin(), infoLog.end());
		log_line(s, LogLevel::ERROR);

		glDeleteShader(shader);
		return true;
	}

	return false;
}

Material create_material(const char* vertex_src, const char* frag_src)
{
	auto mat = Material();

	log_line(fmt::format("Compiling vertex shader"), LogLevel::INFO);
	log_line(fmt::format("START_SRC\n{}\nEND_SRC", vertex_src), LogLevel::DEBUG);
	mat.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(mat.vertex_shader, 1, &vertex_src, NULL);
	try_compile_shader(mat.vertex_shader);

	log_line(fmt::format("Compiling fragment shader"), LogLevel::INFO);
	log_line(fmt::format("START_SRC\n{}\nEND_SRC", frag_src), LogLevel::DEBUG);
	mat.frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(mat.frag_shader, 1, &frag_src, NULL);
	try_compile_shader(mat.frag_shader);

	mat.shader_program = glCreateProgram();
	glAttachShader(mat.shader_program, mat.vertex_shader);
	glAttachShader(mat.shader_program, mat.frag_shader);

	glLinkProgram(mat.shader_program);

	return mat;
}