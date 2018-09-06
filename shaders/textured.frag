#version 330

uniform sampler2D diffuseMap;
uniform bool hasDiffuseMap;
uniform vec3 diffuseColor;
uniform float opacity;

in vec2 uvIn;
out vec4 finalColor;

void main(void)
{
	if(hasDiffuseMap)
	{
		finalColor = texture(diffuseMap, vec2(uvIn.x, -uvIn.y));
	}
	else
	{
		finalColor = vec4(diffuseColor, 1.0f);
	}
}
