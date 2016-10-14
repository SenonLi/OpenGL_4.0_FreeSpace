#version 400 core
in vec2 TexCoords;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300;  

void main()
{
       // ==================  Inverse All Pixels' Color ========================================
	vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1, 1, 1,
        1,  -8, 1,
        1, 1, 1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    vec3 color;
    for(int i = 0; i < 9; i++)
        color += sampleTex[i] * kernel[i];
    
    fragmentColor = vec4(color, 1.0);//*/


	/*   // ==================  Inverse All Pixels' Color ========================================
	fragmentColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0); //*/
	


	/*   // ==================  Gray-Scale All Pixels' Color ========================================
	fragmentColor = texture(screenTexture, TexCoords);
	float average = 0.2126 * fragmentColor.r + 0.7152 * fragmentColor.g + 0.0722 * fragmentColor.b;
	fragmentColor = vec4(average, average, average, 1.0);		// */
	


	//fragmentColor = texture(screenTexture, TexCoords);
} 
