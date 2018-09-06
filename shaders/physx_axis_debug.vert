#version 330

uniform mat4 model;
uniform mat4 view_proj;
uniform mat4 axis_rotation;
uniform mat4 axis_scale;

in vec3 vertex;

void main(void)
{
	gl_Position = view_proj * model * axis_rotation * axis_scale * vec4(vertex, 1.0);
}