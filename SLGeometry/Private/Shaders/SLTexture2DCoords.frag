#version 330 core

in vec2 frag_TexCoords;

out vec4 frag_Color;

uniform sampler2D textureName;

void main()
{             
    frag_Color = texture(textureName, frag_TexCoords);
}