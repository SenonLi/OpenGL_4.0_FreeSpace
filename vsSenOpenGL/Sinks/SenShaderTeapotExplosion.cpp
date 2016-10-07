#include "SenShaderTeapotExplosion.h"

#include "shaderTeapot.h"
using namespace SenShaderTeapot;

SenShaderTeapotExplosion::SenShaderTeapotExplosion()
{
}


SenShaderTeapotExplosion::~SenShaderTeapotExplosion()
{
}

void SenShaderTeapotExplosion::initialGlutGlewGL()
{
	SenAbstractGLWidget::initialGlutGlewGL();

	glGenVertexArrays(1, verArrObjArray);
	glBindVertexArray(verArrObjArray[0]);

	ShaderInfo shader_info[] =
	{
		{ GL_VERTEX_SHADER, "./Sinks/Shaders/teapotExplosion.vert" },
		{ GL_FRAGMENT_SHADER, "./Sinks/Shaders/teapotExplosion.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shader_info);
	glUseProgram(program);

	// "model_matrix" is actually an array of 4 matrices
	modelMatrixLocation = glGetUniformLocation(program, "model_matrix");
	projectionMatrixLocation = glGetUniformLocation(program, "projection_matrix");

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
	
	vmath::mat4 projection_matrix(vmath::perspective(60, aspect, 1.0f, 500.0f));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projection_matrix);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	OutputDebugString("\n Initial teapotExplosion\n");
}

void SenShaderTeapotExplosion::paintGL(void)
{
	SenAbstractGLWidget::paintGL();

	static const vmath::vec4 black = vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	glClearBufferfv(GL_COLOR, 0, black);

	vmath::mat4 projection_matrix(vmath::perspective(60, aspect, 1.0f, 500.0f));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projection_matrix);

	vmath::mat4 model_matrix = vmath::translate(0.0f, 0.0f, -10.0f) * vmath::rotate(xRot, y_axis) * vmath::rotate(yRot, x_axis);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, model_matrix);
	
	drawShaderTeapot();
	
	/* swap front and back buffers */
	glFlush();
}

void SenShaderTeapotExplosion::reshape(int width, int height)
{
	SenAbstractGLWidget::reshape(width, height);

	//glViewport(0, 0, width, height);
	//aspect = float(height) / float(width);

	glFlush();
	glutPostRedisplay();
}
