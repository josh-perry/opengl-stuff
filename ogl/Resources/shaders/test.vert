#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coords;
layout(location = 3) in vec3 vertex_colour;

uniform mat4 transform;
uniform mat4 identity;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light_position;

out vec3 colour;
out vec2 tex_coord;
out vec3 normal;
out vec3 frag_position;
out vec3 light_pos;

void main() {
	colour = vertex_colour;
	tex_coord = vertex_texture_coords;
	normal = mat3(transpose(inverse(identity))) * vertex_normal; // This should be done on the CPU!
	frag_position = vec3(view * identity * vec4(vertex_position, 1.0));
	light_pos = vec3(view * vec4(light_position, 1.0));

	gl_Position = transform * vec4(vertex_position, 1.0);
}
