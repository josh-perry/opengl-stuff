#version 400

layout(location = 0) in vec3 vertex_position;

uniform mat4 view;
uniform mat4 projection;

out vec3 texture_coords;

void main() {
	texture_coords = vertex_position;
	gl_Position = projection * view * vec4(vertex_position, 1.0f); 
}
