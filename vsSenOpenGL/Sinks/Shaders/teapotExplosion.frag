#version 330

out vec4 fragmentColor;

in vec3 verWorldPosition;
in vec3 verNormal;

uniform vec4 colorAmbient = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec4 colorDiffuse = vec4(0.0, 1.0, 0.0, 1.0);
uniform vec4 colorSpecular = vec4(1.00000f, 0.980392f, 0.549020f, 1.0f);

uniform float shininess = 75.0f;

uniform vec3 lightPosition = vec3(-0.0, 25.0, 25.0);


void main(void)
{
    //fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
	vec3 lightDirection = normalize(lightPosition - verWorldPosition);
	vec3 normal = normalize(verNormal);
	vec3 halfVector = normalize(lightDirection + normalize(verWorldPosition));
	float diffuse = max(0.0, dot(normal, lightDirection));
	float specular = pow(max(0.0, dot(verNormal, halfVector)), shininess);
	fragmentColor = colorAmbient + diffuse * colorDiffuse + specular * colorSpecular; // */

}
