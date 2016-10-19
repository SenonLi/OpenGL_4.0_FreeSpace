#version 330 core

in vec2 fragInterpTextureCoord;

out vec4 fragColor;

uniform sampler2D newLayerTexture;
uniform sampler2D backgroundTexture;
uniform sampler2D bbbbbb;

void main()
{

	//fragColor = texture(backgroundTexture, fragInterpTextureCoord);

	fragColor = mix(texture(backgroundTexture, fragInterpTextureCoord),
					 texture(newLayerTexture, fragInterpTextureCoord), 0.1);//*/
}