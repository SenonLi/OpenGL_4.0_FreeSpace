#version 400 core

in vec2 TexCoords;

out vec4 fragmentColor;

uniform sampler2D texture1;

void main()
{             
    vec4 texColor = texture(texture1, TexCoords);
    
	if(texColor.a < 0.1)          discard;

    fragmentColor = texColor;
}