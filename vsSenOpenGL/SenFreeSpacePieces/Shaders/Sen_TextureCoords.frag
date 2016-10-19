#version 330 core

in vec2 TexCoords;
out vec4 fragmentColor;

uniform sampler2D Texture;

void main()
{             
    fragmentColor = texture(Texture, TexCoords);
}