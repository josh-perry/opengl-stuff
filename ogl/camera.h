#pragma once
#include <glm\ext\vector_float3.hpp>
struct Camera
{
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	float yaw;
	float pitch;
	float roll;
};

Camera create_camera()
{
	Camera camera;

	camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
	camera.front = glm::vec3(0.0f, 0.0f, -1.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.yaw = 0;
	camera.pitch = 0;

	return camera;
}
