#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexNormal;

out vec3 verWorldPosition;
out vec3 verNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	
	verWorldPosition = gl_Position.xyz;
	verNormal = vertexNormal;
}