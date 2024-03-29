#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;
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
	frag_normal = v_normal;
	frag_color = v_color + time * 0.1f;
	//frag_color = frag_color 
	gl_Position = Projection * View * Model * vec4(v_position + time , 1.0);
}
