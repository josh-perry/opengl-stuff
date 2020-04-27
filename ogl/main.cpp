#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <fmt/format.h>

#include "material.h"
#include "mesh.h"
#include "model.h"
#include "util.h"
#include "camera.h"
#include "mouse_state.h"

// I don't know why I need this; but I get linker errors otherwise.
// https://community.khronos.org/t/unresolved-external-symbol/19795/2
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

void print_opengl_stats();
void camera_movement(float dt, GLFWwindow* window, Camera* camera);
void camera_rotation(float dt, GLFWwindow* window, Camera* camera, MouseState mouse_state);

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
	
	// Mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	MouseState mouse_state = create_mouse_state();

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

	auto camera = create_camera();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

	auto model = create_model("Resources/monkey.obj");

	// HACK: for now just set all the meshes to our premade material's shader
	for (unsigned int i = 0; i < model.meshes.size(); i++)
	{
		model.meshes[i].shader_program = mat.shader_program;
	}

	float dt = 0.0f;
	float last_frametime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float frametime = glfwGetTime();
		dt = frametime - last_frametime;
		last_frametime = frametime;

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		update_mouse_deltas(&mouse_state, xpos, ypos);

		glfwPollEvents();

		camera_movement(dt, window, &camera);
		camera_rotation(dt, window, &camera, mouse_state);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = glm::lookAt(
			camera.position,
			camera.position + camera.front,
			camera.up
		);

		glm::mat4 identity = glm::mat4(1.0f);
		draw_model(model, identity, view, projection);
		
		reset_mouse_deltas(&mouse_state);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void camera_rotation(float dt, GLFWwindow* window, Camera* camera, MouseState mouse_state)
{
	camera->yaw += mouse_state.dx;
	camera->pitch += mouse_state.dy;

	if (camera->pitch > 89.0f)
	{
		camera->pitch = 89.0f;
	}
	if (camera->pitch < -89.0f)
	{
		camera->pitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
	direction.y = sin(glm::radians(camera->pitch));
	direction.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));

	camera->front = glm::normalize(direction);
}

void camera_movement(float dt, GLFWwindow* window, Camera* camera)
{
	const float speed = 2.0f * dt;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->position += speed * camera->front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->position -= speed * camera->front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->position -= glm::normalize(glm::cross(camera->front, camera->up)) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->position += glm::normalize(glm::cross(camera->front, camera->up)) * speed;
	}
}

void print_opengl_stats()
{
	log_line(fmt::format("Renderer: {}",  glGetString(GL_RENDERER)), LogLevel::INFO);
	log_line(fmt::format("OpenGL version: {}", glGetString(GL_VERSION)), LogLevel::INFO);
}