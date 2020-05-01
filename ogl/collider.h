#pragma once
#include <glm\ext\vector_float3.hpp>

#include "mesh_generators.h"

struct Collider
{

};

struct BoxCollider
{
	glm::vec3 size;

	Mesh mesh;
};

BoxCollider create_boxcollider(glm::vec3 size)
{
	BoxCollider box;
	box.size = size;

	std::vector<float> cube = generate_cube(1.0f, 1.0f, 1.0f);
	std::vector<Vertex> v;

	for(unsigned int i = 0; i < cube.size(); i += 3)
	{
		Vertex vertex{};
		vertex.position = glm::vec3(cube[i], cube[i+1], cube[i+2]);
		v.push_back(vertex);
	}

	box.mesh = create_mesh(v, std::vector<GLuint>{}, std::vector<Texture>{});
	box.mesh.material = get_debug_material();

	return box;
}