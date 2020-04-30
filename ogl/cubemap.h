#pragma once
#include <GL\glew.h>
#include "material.h"
struct Cubemap
{
    GLuint vao;
    GLuint vbo;
    GLuint texture;

    Material material;
};

Cubemap create_cubemap(Material material, GLuint texture)
{
	Cubemap cubemap;

    float points[] = {
      -10.0f,  10.0f, -10.0f,
      -10.0f, -10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,
       10.0f,  10.0f, -10.0f,
      -10.0f,  10.0f, -10.0f,

      -10.0f, -10.0f,  10.0f,
      -10.0f, -10.0f, -10.0f,
      -10.0f,  10.0f, -10.0f,
      -10.0f,  10.0f, -10.0f,
      -10.0f,  10.0f,  10.0f,
      -10.0f, -10.0f,  10.0f,

       10.0f, -10.0f, -10.0f,
       10.0f, -10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,

      -10.0f, -10.0f,  10.0f,
      -10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f, -10.0f,  10.0f,
      -10.0f, -10.0f,  10.0f,

      -10.0f,  10.0f, -10.0f,
       10.0f,  10.0f, -10.0f,
       10.0f,  10.0f,  10.0f,
       10.0f,  10.0f,  10.0f,
      -10.0f,  10.0f,  10.0f,
      -10.0f,  10.0f, -10.0f,

      -10.0f, -10.0f, -10.0f,
      -10.0f, -10.0f,  10.0f,
       10.0f, -10.0f, -10.0f,
       10.0f, -10.0f, -10.0f,
      -10.0f, -10.0f,  10.0f,
       10.0f, -10.0f,  10.0f
    };
    
    glGenBuffers(1, &cubemap.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cubemap.vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &cubemap.vao);
    glBindVertexArray(cubemap.vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cubemap.vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    cubemap.material = material;
    cubemap.texture = texture;

	return cubemap;
}

void draw_cubemap(Cubemap cubemap, glm::mat4 view, glm::mat4 projection)
{
    glDepthMask(GL_FALSE);
    glUseProgram(cubemap.material.shader_program);

    // Scrub translation from the view
    view = glm::mat4(glm::mat3(view));

	GLuint viewLoc = glGetUniformLocation(cubemap.material.shader_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	
	GLuint projectionLoc = glGetUniformLocation(cubemap.material.shader_program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.texture);
    glBindVertexArray(cubemap.vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}