#version 330

in vec2 texcoord;
uniform sampler2D texture_1;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = texture2D(texture_1, texcoord);
}