#version 330

uniform vec3 axis_color;

out vec4 finalColor;

void main(void)
{
	finalColor = vec4(axis_color, 1.0f);
}