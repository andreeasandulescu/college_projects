#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 3) in vec3 v_color;

// Uniform properties

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;

void main()
{
	
	frag_color.x = 0.45 * sin(time) - 0.13 * cos(time) + 0.45;
	frag_color.y = 0.1 * sin(time) + 0.32 * cos(time) + 0.44;
	frag_color.z = 0.04 * sin(time) + 0.37 * cos(time) + 0.43;
	gl_Position = Projection * View * Model * vec4(v_position , 1.0);
}
