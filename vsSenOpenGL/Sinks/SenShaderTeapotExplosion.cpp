#include "SenShaderTeapotExplosion.h"

#include "shaderTeapot.h"
using namespace SenShaderTeapot;

SenShaderTeapotExplosion::SenShaderTeapotExplosion()
{
	//widgetWidth = DEFAULT_widgetWidth;
	//widgetHeight = DEFAULT_widgetHeight;
}


SenShaderTeapotExplosion::~SenShaderTeapotExplosion()
{
}

void SenShaderTeapotExplosion::initGlfwGlewGL()
{
	SenDebugWindowFreeSpace::initGlfwGlewGL();


	ShaderInfo shader_info[] =
	{
		{ GL_VERTEX_SHADER, "./Sinks/Shaders/teapotExplosion.vert" },
		{ GL_FRAGMENT_SHADER, "./Sinks/Shaders/teapotExplosion.frag" },
		{ GL_NONE, NULL }
	};

	programA = LoadShaders(shader_info);
	glUseProgram(programA);

	glGenVertexArrays(1, verArrObjArray);
	glBindVertexArray(verArrObjArray[0]);

	// "model" is actually an array of 4 matrices
	modelMatrixLocation = glGetUniformLocation(programA, "model");
	projectionMatrixLocation = glGetUniformLocation(programA, "projection");

	// Set up the element array buffer
	glGenBuffers(1, verIndicesObjArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verIndicesObjArray[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shaderTeapotIndices), shaderTeapotIndices, GL_STATIC_DRAW);

	// Set up the vertex attributes
	glGenBuffers(1, verBufferObjArray);
	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shaderTeapotVertices), shaderTeapotVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	GLuint vertexNormalArray[1];
	glGenBuffers(1, vertexNormalArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexNormalArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shaderTeapotNormals), shaderTeapotNormals, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), NULL);
	glEnableVertexAttribArray(1);
	
	vmath::mat4 projection(vmath::perspective(60, aspect, 1.0f, 500.0f));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projection);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	OutputDebugString("\n Initial teapotExplosion\n");
}

void SenShaderTeapotExplosion::paintScene()
{
	glUseProgram(programA);
	glBindVertexArray(verArrObjArray[0]);

	vmath::mat4 projection(vmath::perspective(60, aspect, 1.0f, 500.0f));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projection);

	vmath::mat4 model = vmath::translate(0.0f, 0.0f, -10.0f) * vmath::rotate(xRot, y_axis) * vmath::rotate(yRot, x_axis);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, model);
	
	drawShaderTeapot();

	glUseProgram(0);
	glBindVertexArray(0);
}

void SenShaderTeapotExplosion::cleanDebugWindowFreeSpace()	{
	if (glIsVertexArray(verArrObjArray[0]))	glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))	glDeleteBuffers(1, verBufferObjArray);
	if (glIsBuffer(verIndicesObjArray[0]))	glDeleteBuffers(1, verIndicesObjArray);


	if (glIsProgram(programA))				glDeleteProgram(programA);
}