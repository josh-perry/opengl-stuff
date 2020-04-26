#pragma once
#include <glm\ext\vector_float3.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coords;
};
