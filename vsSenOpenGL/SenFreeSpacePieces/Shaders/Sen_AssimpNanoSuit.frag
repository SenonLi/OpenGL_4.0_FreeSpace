#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D meshDiffuseTexture1;

void main()
{    
    color = texture(meshDiffuseTexture1, TexCoords);
}