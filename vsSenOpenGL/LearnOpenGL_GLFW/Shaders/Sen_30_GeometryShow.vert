#version 400 core
layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec3 vertexColor;

out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
    gl_Position = vec4(vertexPosition.x, vertexPosition.y, 0.0f, 1.0f); 
    vs_out.color = vertexColor;
}