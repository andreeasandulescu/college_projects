#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main()
{
	float ambient = 0.25;

	// TODO: compute world space vectors
	vec3 world_pos = (Model * vec4(v_position,1)).xyz;
	vec3 world_normal = normalize( mat3(Model) * v_normal );

	// TODO: send color light output to fragment shader
	float auxColor =  ambient_light + fat * (diffuse_light + specular_light);
	color = vec3(object_color * auxColor);
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
