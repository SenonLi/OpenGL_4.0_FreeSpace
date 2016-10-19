#version 330 core

in vec2 fragInterpTextureCoord;
out vec4 fragColor;

uniform sampler2D backgroundTexture;

void main()
{
	fragColor = texture(backgroundTexture, fragInterpTextureCoord);
}