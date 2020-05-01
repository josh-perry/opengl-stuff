#include <imgui.h>

#include <GL/glew.h>

#include <entt/entt.hpp>

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
#include "cubemap.h"
#include "gameobject.h"

// Components
#include "position.h"
#include "velocity.h"
#include "renderable.h"
#include "collider.h"

// I don't know why I need this; but I get linker errors otherwise.
// https://community.khronos.org/t/unresolved-external-symbol/19795/2
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

void print_opengl_stats();
void camera_movement(float dt, GLFWwindow* window, Camera* camera);
void camera_rotation(float dt, GLFWwindow* window, Camera* camera, MouseState mouse_state);
void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
void create_monkey(entt::registry& registry);

float window_width = 800;
float window_height = 600;

void update(float dt, entt::registry& registry)
{
	registry.view<Position, Velocity>().each([dt](auto& pos, auto& vel) {
		pos.position += vel.velocity * dt;
	});
}

void draw(entt::registry& registry, Camera camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 identity = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(
		camera.position,
		camera.position + camera.front,
		camera.up
	);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), window_width / window_height, 0.1f, 100.0f);


	registry.view<Cubemap, Renderable>().each([camera, view, projection](auto& cubemap, auto& renderable)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		draw_cubemap(cubemap, view, projection);
	});

	registry.view<Position, Renderable, Model>().each([camera, identity, view, projection](auto& pos, auto& renderable, auto& model)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		draw_model(model, identity, view, projection, pos.position);
	});

	registry.view<BoxCollider, Position>().each([camera, identity, view, projection](auto& box, auto& pos)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh(box.mesh, identity, view, projection, pos.position);
	});
}

int main()
{
	set_min_log_level(LogLevel::DEBUG);

	if (!glfwInit())
	{
		log_line("Could not start GLFW3", LogLevel::FATAL);
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow((int)window_width, (int)window_height, "Hello triangle!", NULL, NULL);

	if (!window)
	{
		log_line("Could not open window", LogLevel::FATAL);
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	// Setup callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

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

	auto sky_vertex_shader_src = load_string_from_file("Resources/shaders/sky.vert");
	auto sky_frag_shader_src = load_string_from_file("Resources/shaders/sky.frag");
	auto sky_mat = create_material(sky_vertex_shader_src.c_str(), sky_frag_shader_src.c_str());

	auto camera = create_camera();

	auto skybox = create_texture();
	skybox.path = "sky/sky.jpg";
	skybox.type = TextureType::CUBEMAP;
	GLuint skybox_texture = load_texture(skybox);

	entt::registry registry;
	create_monkey(registry);

	auto cubemap = registry.create();
	registry.assign<Renderable>(cubemap, Renderable{});
	registry.assign<Cubemap>(cubemap, create_cubemap(sky_mat, skybox_texture));

	float dt = 0.0f;
	float last_frametime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// Frametimes
		float frametime = glfwGetTime();
		dt = frametime - last_frametime;
		last_frametime = frametime;

		// Input
		glfwPollEvents();

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		update_mouse_deltas(&mouse_state, x, y);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}

		camera_movement(dt, window, &camera);
		camera_rotation(dt, window, &camera, mouse_state);

		update(dt, registry);
		draw(registry, camera);

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
	log_line(fmt::format("Renderer: {}", glGetString(GL_RENDERER)), LogLevel::INFO);
	log_line(fmt::format("OpenGL version: {}", glGetString(GL_VERSION)), LogLevel::INFO);
}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	window_width = width;
	window_height = height;
}

void create_monkey(entt::registry& registry)
{
	auto p = Position{};
	p.position = glm::vec3(0.0f, 0.0f, 0.0f);

	auto monkey = registry.create();
	registry.assign<Position>(monkey, p);
	registry.assign<Renderable>(monkey, Renderable{});
	registry.assign<Velocity>(monkey, Velocity{ 1.0f, 0.0f, 0.0f });
	registry.assign<Model>(monkey, create_model("Resources/monkey.obj"));
	registry.assign<BoxCollider>(monkey, create_boxcollider(glm::vec3(1.0f, 1.0f, 1.0f)));
}
