#pragma once
#include <vector>

#include "vertex.h"
#include <GL\glew.h>
#include "texture.h"

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	GLuint colour;
};

std::vector<GLfloat> get_colour_data(unsigned int vertices_count)
{
	std::vector<GLfloat> colour_data;
	for (unsigned int v = 0; v < vertices_count; v++) {
		colour_data.push_back(1.0f);
		colour_data.push_back(0.0f);
		colour_data.push_back(1.0f);
	}

	return colour_data;
}

Mesh create_mesh(std::vector<Vertex> v, std::vector<GLuint> i, std::vector<Texture> t)
{
	Mesh m = Mesh();
	m.vertices = v;
	m.indices = i;
	m.textures = t;

	const int vertex_attrib = 0;
	const int vertex_colour_attrib = 1;

	glGenVertexArrays(1, &m.vao);
	glBindVertexArray(m.vao);

	// VBO
	glGenBuffers(1, &m.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
	glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vertex), &m.vertices[0], GL_STATIC_DRAW);

	// EBO
	glGenBuffers(1, &m.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(GLuint), &m.indices[0], GL_STATIC_DRAW);

	// Setup vertex attrib
	glEnableVertexAttribArray(vertex_attrib);
	glVertexAttribPointer(vertex_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);

	// Vertex colours
	auto colour_data = get_colour_data(m.vertices.size());
	glGenBuffers(1, &m.colour);
	glBindBuffer(GL_ARRAY_BUFFER, m.colour);
	glBufferData(GL_ARRAY_BUFFER, colour_data.size() * sizeof(GLfloat), &colour_data[0], GL_STATIC_DRAW);

	// Enable colour vertex attribute
	glEnableVertexAttribArray(vertex_colour_attrib);
	glVertexAttribPointer(vertex_colour_attrib, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	return m;
}

void draw_mesh(Mesh mesh)
{
	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}