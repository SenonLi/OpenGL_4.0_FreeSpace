#version 330 core
layout(location = 0) in vec4 vsVertex;

void main()
{
    gl_Position = vsVertex;
}