#version 330 core

in vec2 TexCoords;
out vec4 fragmentColor;

uniform sampler2D imageTexture;
uniform vec3 renderColor;

void main()
{
	vec4 textureColor = texture(imageTexture, TexCoords);
	textureColor = sqrt(textureColor);
	textureColor = sqrt(textureColor);
	fragmentColor = vec4(renderColor, 1.0) * textureColor;
}