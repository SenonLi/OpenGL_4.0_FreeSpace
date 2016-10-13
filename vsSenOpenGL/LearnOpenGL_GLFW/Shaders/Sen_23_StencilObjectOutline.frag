#version 400 core

in vec2 TexCoords;
out vec4 fragmentColor;

uniform sampler2D texture1;

void main()
{             
    fragmentColor = texture(texture1, TexCoords);
}