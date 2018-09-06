#version 330

uniform mat4 view_proj;
uniform mat4 model;

in vec3 vertex;
in vec2 uv;
out vec2 uvIn;

void main(void)
{
	gl_Position = view_proj * model * vec4(vertex, 1.0);
	uvIn = uv;
}
