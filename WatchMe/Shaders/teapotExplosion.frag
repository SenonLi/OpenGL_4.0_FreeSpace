#version 330

out vec4 fragmentColor;

//in vec3 verWorldPosition;
//in vec3 verNormal;

in GeomSh_OUT {
	vec3 verWorldPosition;
	vec3 verNormal;
} fragSh_in;

uniform vec4 colorAmbient = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec4 colorDiffuse = vec4(0.0, 1.0, 0.0, 1.0);
uniform vec4 colorSpecular = vec4(1.00000f, 0.980392f, 0.549020f, 1.0f);

uniform float shininess = 25.0f;

uniform vec3 lightPosition = vec3(-25.0, 25.0, 100.0);


void main(void)
{
    //fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
	vec3 lightDirection = normalize(lightPosition - fragSh_in.verWorldPosition);
	vec3 normal = normalize(fragSh_in.verNormal);
	vec3 halfVector = normalize(lightDirection + normalize(fragSh_in.verWorldPosition));
	float diffuse = max(0.0, dot(normal, lightDirection));
	float specular = pow(max(0.0, dot(fragSh_in.verNormal, halfVector)), shininess);
	fragmentColor = colorAmbient + diffuse * colorDiffuse + specular * colorSpecular; // */

}
