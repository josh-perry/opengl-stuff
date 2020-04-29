#pragma once
#include "model.h"

struct GameObject
{
	Model model;

	glm::vec3 world_position;
};

GameObject create_gameobject()
{
	GameObject gameobject{};
	return gameobject;
}

void draw_gameobject()
{

}