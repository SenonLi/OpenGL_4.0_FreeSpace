#version 330 core

in vec2 TexCoords;
out vec4 fragmentColor;

uniform sampler2D imageTexture;
uniform vec3 renderColor;

void main()
{
    vec4 textureColor = texture(imageTexture, TexCoords);
    
	if(textureColor.r == 1.0 && textureColor.g == 1.0 && textureColor.b  == 1.0)          discard;

	fragmentColor = vec4(renderColor, 1.0) * textureColor;
}