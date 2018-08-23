#version 330 core

layout (location = 0) in vec3 vert_Position;
layout (location = 1) in vec2 vert_TexCoords;

out vec2 frag_TexCoords;

void main()
{
    gl_Position = vec4(vert_Position, 1.0f); 
    frag_TexCoords = vert_TexCoords;
}  
