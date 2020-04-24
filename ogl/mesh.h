#pragma once
#include <vector>

#include "vertex.h"
#include <GL\glew.h>

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

Mesh create_mesh(std::vector<Vertex> v, std::vector<GLuint> i)
{
	Mesh m = Mesh();
	m.vertices = v;
	m.indices = i;

	glGenVertexArrays(1, &m.vao);
	glGenBuffers(1, &m.vbo);
	glGenBuffers(1, &m.ebo);

	glBindVertexArray(m.vao);
	glBindBuffer(GL_ARRAY_BUFFER, m.vbo);

	glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vertex), &m.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(GLuint), &m.indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);

	glBindVertexArray(0);

	return m;
}

void draw_mesh(Mesh mesh)
{
	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}