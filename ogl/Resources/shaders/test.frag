#version 400

out vec4 frag_colour;

in vec3 colour;
in vec2 tex_coord;
in vec3 normal;
in vec3 frag_position;
in vec3 light_pos;

uniform sampler2D tex;
uniform vec3 view_position;
uniform vec3 light_colour;

void main() {
	// Ambient
	float ambient_strength = 0.2f;
	vec3 ambient = ambient_strength * light_colour;

	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light_pos - frag_position);

	// Diffuse
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * light_colour;

	// Specular
	float specular_strength = 0.5f;
	int shininess = 32;

	//vec3 view_direction = normalize(view_position - frag_position);
	vec3 view_direction = normalize(-frag_position);
	vec3 reflect_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), shininess);
	vec3 specular = specular_strength * spec * light_colour;

	frag_colour = vec4((ambient + diffuse + specular) * texture(tex, tex_coord).rgb, 1.0);
}
