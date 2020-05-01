#pragma once
#include <string>
#include <GL\glew.h>
#include <gl\GL.h>
#include <vector>
#include <fstream>

#include "log.h"
#include "util.h"

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

bool default_mat_initialized = false;
Material default_mat;

bool debug_mat_initialized = false;
Material debug_mat;

Material get_default_material()
{
	if (default_mat_initialized)
		return default_mat;

	auto vertex_shader_src = load_string_from_file("Resources/shaders/test.vert");
	auto frag_shader_src = load_string_from_file("Resources/shaders/test.frag");
	default_mat = create_material(vertex_shader_src.c_str(), frag_shader_src.c_str());

	default_mat_initialized = true;
	return default_mat;
}

Material get_debug_material()
{
	if (debug_mat_initialized)
		return debug_mat;

	auto vertex_shader_src = load_string_from_file("Resources/shaders/debug.vert");
	auto frag_shader_src = load_string_from_file("Resources/shaders/debug.frag");
	debug_mat = create_material(vertex_shader_src.c_str(), frag_shader_src.c_str());

	debug_mat_initialized = true;
	return debug_mat;
}