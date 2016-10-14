#version 330 core

in vec2 TexCoords;
out vec4 fragmentColor;

uniform sampler2D texture1;

float near = 1.0; 
float far = 100.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    //fragmentColor = texture(texture1, TexCoords);
    
	
	// divide by far to get depth in range [0,1] for visualization purposes.
	float depth = LinearizeDepth(gl_FragCoord.z) / far;//  + 1.0f;
	depth = sqrt(depth);
	fragmentColor = vec4(vec3(depth), 1.0f);//*/
}