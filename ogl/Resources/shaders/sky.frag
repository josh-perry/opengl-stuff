#version 400

out vec4 frag_colour;

in vec3 texture_coords;

uniform samplerCube	skybox;

void main() {
	frag_colour = texture(skybox, texture_coords);
}
