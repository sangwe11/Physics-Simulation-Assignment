#version 330

in vec2 uvIn;

out vec4 finalColor;

uniform sampler2D crosshairTexture;

void main()
{
	vec4 texture_color = texture(crosshairTexture, uvIn);
	
	if(texture_color.a == 0.0)
		discard;
		
	finalColor = texture_color;
}