#pragma once
#include <glm\ext\vector_float3.hpp>

struct Velocity
{
	Velocity(float vx, float vy, float vz) : velocity(vx, vy, vz) {};

	glm::vec3 velocity;
};
