#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "util.h"

// I don't know why I need this; but I get linker errors otherwise.
// https://community.khronos.org/t/unresolved-external-symbol/19795/2
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

void print_opengl_stats();

int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const float width = 800;
	const float height = 600;
	auto window = glfwCreateWindow((int)width, (int)height, "Hello triangle!", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	print_opengl_stats();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	auto vertex_shader_src = load_string_from_file("Resources/shaders/test.vert");
	auto frag_shader_src = load_string_from_file("Resources/shaders/test.frag");
	auto mat = create_material(vertex_shader_src.c_str(), frag_shader_src.c_str());

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 5.0f),  // eye position
		glm::vec3(0.0f, 0.0f, 0.0f),  // point at origin
		glm::vec3(0.0f, 1.0f, 0.0f)   // +Y = up
	);

	auto model = create_model("D:/monkey.obj");

	// HACK: for now just set all the meshes to our premade material's shader
	for (unsigned int i = 0; i < model.meshes.size(); i++)
	{
		model.meshes[i].shader_program = mat.shader_program;
	}

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 identity = glm::mat4(1.0f);
		draw_model(model, identity, view, projection);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void print_opengl_stats()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
}