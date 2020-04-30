#include <imgui.h>

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
#include "cubemap.h"
#include "gameobject.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

// I don't know why I need this; but I get linker errors otherwise.
// https://community.khronos.org/t/unresolved-external-symbol/19795/2
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

void print_opengl_stats();
void camera_movement(float dt, GLFWwindow* window, Camera* camera);
void camera_rotation(float dt, GLFWwindow* window, Camera* camera, MouseState mouse_state);
void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

float window_width = 800;
float window_height = 600;

void imgui_debugger(std::vector<GameObject> gameobjects)
{
	if (ImGui::Begin("Inspector"))
	{
		for(unsigned int i = 0; i < gameobjects.size(); i++)
		{
			ImGui::PushID(gameobjects[i].id);
			ImGui::Text(gameobjects[i].name.c_str());

			float p[3] = {
				gameobjects[i].world_position.x,
				gameobjects[i].world_position.y,
				gameobjects[i].world_position.z
			};

			if (ImGui::DragFloat3("Position", p))
			{
				gameobjects[i].world_position = glm::vec3(p[0], p[1], p[2]);
			}

			ImGui::PopID();
		}
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
	time_t t;
	srand((unsigned)time(&t));

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

	auto vertex_shader_src = load_string_from_file("Resources/shaders/test.vert");
	auto frag_shader_src = load_string_from_file("Resources/shaders/test.frag");
	auto mat = create_material(vertex_shader_src.c_str(), frag_shader_src.c_str());

	auto sky_vertex_shader_src = load_string_from_file("Resources/shaders/sky.vert");
	auto sky_frag_shader_src = load_string_from_file("Resources/shaders/sky.frag");
	auto sky_mat = create_material(sky_vertex_shader_src.c_str(), sky_frag_shader_src.c_str());

	auto camera = create_camera();

	auto skybox = create_texture();
	skybox.path = "sky/sky.jpg";
	skybox.type = TextureType::CUBEMAP;

	GLuint skyboxTexture = load_texture(skybox);

	Cubemap cubemap = create_cubemap();
	cubemap.material = sky_mat;
	cubemap.texture = skyboxTexture;

	std::vector<Model> models;
	std::vector<GameObject> gameobjects;

	for (unsigned int i = 0; i <= 5; i++)
	{
		GameObject gameobject = create_gameobject();

		if (i % 2 == 0)
		{
			gameobject.model = create_model("Resources/monkey.obj");
			gameobject.name = "Monkey";
		}
		else
		{
			gameobject.model = create_model("Resources/dice.obj");
			gameobject.name = "Dice";
		}

		gameobject.world_position = glm::vec3(i * 5.0f, 0.0f, 0.0f);
		gameobjects.push_back(gameobject);
	}

	for (unsigned int j = 0; j < gameobjects.size(); j++)
	{
		GameObject* gameobject = &gameobjects[j];

		for (int i = 0; i < gameobject->model.meshes.size(); i++)
		{
			gameobject->model.meshes[i].shader_program = mat.shader_program;
		}
	}

	float dt = 0.0f;
	float last_frametime = 0.0f;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	bool imgui_showing = false;

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

		if (glfwGetKey(window, GLFW_KEY_P))
		{
			imgui_showing = !imgui_showing;

			if (imgui_showing)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}

		if (imgui_showing)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		else
		{
			camera_movement(dt, window, &camera);
			camera_rotation(dt, window, &camera, mouse_state);
		}

		// Draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = glm::lookAt(
			camera.position,
			camera.position + camera.front,
			camera.up
		);
		
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), window_width / window_height, 0.1f, 100.0f);
		draw_cubemap(cubemap, view, projection);

		glm::mat4 identity = glm::mat4(1.0f);

		for (GameObject gameobject : gameobjects)
		{
			draw_gameobject(gameobject, identity, view, projection);
		}
		
		reset_mouse_deltas(&mouse_state);

		if (imgui_showing)
		{
			imgui_debugger(gameobjects);
		}

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

void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	window_width = width;
	window_height = height;
}