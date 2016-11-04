#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexNormal;

out VerSh_OUT {
	vec3 verWorldPosition;
	vec3 verNormal;
} verSh_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	
	verSh_out.verWorldPosition = gl_Position.xyz;
	verSh_out.verNormal = vertexNormal;
}
