#version 400 core
layout (location = 0) in vec3 vertexCoord;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoord;

out vec2 fragInterpTextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{

	//gl_Position = vec4(vertexCoord, 1.0f);

	gl_Position = projection * view * model * vec4(vertexCoord, 1.0f);

	fragInterpTextureCoord = textureCoord;
}