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
	vec3 lightGreen = vec3(0.019f, 1.0f, 0.019f);

	float aux = (sin(time) + 1.0f) /2.0f;
	frag_color = (0.7f * aux  + 0.3f) * lightGreen;

	gl_Position = Projection * View * Model * vec4(v_position , 1.0);
}
