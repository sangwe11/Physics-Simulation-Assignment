#version 330

uniform mat4 model;
uniform mat4 shape;
uniform mat4 view_proj;

in vec3 vertex;

void main(void)
{
	gl_Position = view_proj * model * shape * vec4(vertex, 1.0);
}
