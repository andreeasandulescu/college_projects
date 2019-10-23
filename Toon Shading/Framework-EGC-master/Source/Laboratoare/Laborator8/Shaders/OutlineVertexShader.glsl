#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

void main()
{
	float fact = (sin(time * 2) + 1.2) / 300.0f;

	vec3 world_pos = (Model * vec4(v_position,1)).xyz;
	vec3 world_normal = normalize(mat3(Model) * v_normal);
	
	vec3 aux = world_pos + fact * world_normal;
	gl_Position = Projection * View * vec4(aux, 1);
}
