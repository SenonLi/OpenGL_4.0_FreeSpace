#version 330 core

in vec2 TexCoords;

out vec4 fragmentColor;

uniform sampler2D textTexture;
uniform vec3 lineTextStringColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textTexture, TexCoords).r);
	fragmentColor = vec4(lineTextStringColor, 1.0) * sampled;
}