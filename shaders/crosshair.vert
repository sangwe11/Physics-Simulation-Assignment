#version 330

const vec2 madd = vec2(0.5, 0.5);

in vec3 vertex;

out vec2 uvIn;

uniform mat4 view;
uniform mat4 size;

void main()
{
	uvIn = (vertex.xy * madd) + madd;
	gl_Position = view * size * vec4(vertex.xy, 0.0, 1.0);
}
