#include <GL/glew.h>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "material.h"

// I don't know why I need this; but I get linker errors otherwise.
// https://community.khronos.org/t/unresolved-external-symbol/19795/2
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 600, "Hello triangle!", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	float points[] = {
		 0.0f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	float colours[] = {
		1.0f, 0.2f, 0.2f,
		0.2f, 1.0f, 0.2f,
		0.2f, 0.2f, 1.0f
	};

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(points)/sizeof(float)) * sizeof(float), points, GL_STATIC_DRAW);

	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(colours)/sizeof(float)) * sizeof(float), colours, GL_STATIC_DRAW);

	const int vao_point_index = 0;
	const int vao_colour_index = 1;

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(vao_point_index);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(vao_point_index, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(vao_colour_index);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(vao_colour_index, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	const char* vertex_shader_src =
		"#version 400\n"
		"layout(location = 0) in vec3 vertex_position;\n"
		"layout(location = 1) in vec3 vertex_colour;\n"
		"out vec3 colour;"
		"void main() {\n"
		"	colour = vertex_colour;"
		"   gl_Position = vec4(vertex_position, 1.0);"
		"}";

	const char* frag_shader_src =
		"#version 400\n"
		"in vec3 colour;"
		"out vec4 frag_colour;\n"
		"void main() {\n"
		"	frag_colour = vec4(colour, 1.0);\n"
		"}";

	auto mat = create_material(vertex_shader_src, frag_shader_src);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(mat.shader_program);
		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
