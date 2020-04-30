#pragma once
#include "model.h"

struct GameObject
{
	unsigned int id;
	std::string name;

	Model model;

	glm::vec3 world_position;
};

GameObject create_gameobject(std::string name = "Unnamed object")
{
	GameObject gameobject{};
	gameobject.id = rand();
	gameobject.name = name;

	return gameobject;
}

void draw_gameobject(GameObject gameobject, glm::mat4 identity, glm::mat4 view, glm::mat4 projection)
{
	draw_model(gameobject.model, identity, view, projection, gameobject.world_position);
}