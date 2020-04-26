#version 400

out vec4 frag_colour;

in vec3 colour;
in vec2 tex_coord;

uniform sampler2D tex;

void main() {
	frag_colour = texture(tex, tex_coord);
}
