#pragma once
#include <vector>

#include "vertex.h"
#include <GL\glew.h>
#include "texture.h"

#include <math.h>

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	GLuint shader_program;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	GLuint colour;
};

Mesh create_mesh(std::vector<Vertex> v, std::vector<GLuint> i, std::vector<Texture> t)
{
	Mesh m = Mesh();
	m.vertices = v;
	m.indices = i;
	m.textures = t;

	const int vertex_attrib = 0;
	const int vertex_normal_attrib = 1;
	const int vertex_texture_coords_attrib = 2;

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

	glEnableVertexAttribArray(vertex_normal_attrib);
	glVertexAttribPointer(vertex_normal_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(vertex_texture_coords_attrib);
	glVertexAttribPointer(vertex_texture_coords_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));

	return m;
}

void draw_mesh(Mesh mesh, glm::mat4 identity, glm::mat4 view, glm::mat4 projection)
{
	glUseProgram(mesh.shader_program);

	GLuint identityLoc = glGetUniformLocation(mesh.shader_program, "identity");
	glUniformMatrix4fv(identityLoc, 1, GL_FALSE, glm::value_ptr(identity));

	GLuint viewLoc = glGetUniformLocation(mesh.shader_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	
	GLuint projectionLoc = glGetUniformLocation(mesh.shader_program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 transform = projection * view * identity;
	//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
	//transform = glm::translate(transform, glm::vec3(0.0f, (float)sin(glfwGetTime()), 0.0f));

	GLuint transformLoc = glGetUniformLocation(mesh.shader_program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	GLuint lightColourLoc = glGetUniformLocation(mesh.shader_program, "light_colour");
	glUniform3f(lightColourLoc, 1.0f, 1.0f, 1.0f);

	GLuint lightPositionLoc = glGetUniformLocation(mesh.shader_program, "light_position");
	glUniform3f(lightPositionLoc, sin(glfwGetTime()*2) * 24.0f, 5.0f, 5.0f);

	// For now just get the first texture
	if (mesh.textures.size() > 0)
	{
		glBindTexture(GL_TEXTURE_2D, mesh.textures[0].id);
	}
	else
	{
		log_line("No textures found for mesh; unbinding", LogLevel::TRACE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}