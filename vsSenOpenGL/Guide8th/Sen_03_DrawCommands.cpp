#include "Sen_03_DrawCommands.h"
#include "Guide8th/External/shaderTeapotTest.h"


Sen_03_DrawCommands::Sen_03_DrawCommands()
{
	strWindowName = "3D Four Triangles";
}


Sen_03_DrawCommands::~Sen_03_DrawCommands()
{
}

void Sen_03_DrawCommands::initialGlutGlewGL()
{
	SenAbstractGLWidget::initialGlutGlewGL();

	glGenVertexArrays(1, verArrObjArray);
	glBindVertexArray(verArrObjArray[0]);

	ShaderInfo shader_info[] =
	{
		{ GL_VERTEX_SHADER, "./Guide8th/Shaders/Sen_03_DrawCommands.vert" },
		{ GL_FRAGMENT_SHADER, "./Guide8th/Shaders/Sen_03_DrawCommands.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shader_info);
	glUseProgram(program);

	// "model_matrix" is actually an array of 4 matrices
	modelMatrixLocation = glGetUniformLocation(program, "model_matrix");
	projectionMatrixLocation = glGetUniformLocation(program, "projection_matrix");

	//initFourTriangles();
	initCube();
	//initTeapot();

	//vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 500.0f));
	vmath::mat4 projection_matrix(vmath::perspective(60, aspect, 1.0f, 500.0f));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projection_matrix);
	
	OutputDebugString(" Initial Triangles\n\n");
}

void Sen_03_DrawCommands::paintGL(void)
{
	SenAbstractGLWidget::paintGL();
	static const vmath::vec4 black = vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	//vmath::mat4 model_matrix;
	glClearBufferfv(GL_COLOR, 0, black);

	// Set up the model and projection matrix
	//vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 50.0f));
	vmath::mat4 projection_matrix(vmath::perspective(60, aspect, 1.0f, 500.0f));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projection_matrix);
	
	//// Draw Arrays...
	//model_matrix = vmath::translate(-3.0f, 0.0f, -5.0f) * vmath::rotate(xRot, Y) * vmath::rotate(yRot, X);
	//glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, model_matrix);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	vmath::mat4 model_matrix = vmath::translate(0.0f, 0.0f, -10.0f) * vmath::rotate(xRot, y_axis) * vmath::rotate(yRot, x_axis);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, model_matrix);

	// DrawFourTriangles
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const GLvoid *)(0 * sizeof(GLushort)));

	// DrawCube
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const GLvoid *)(9 * sizeof(GLushort)));

	//drawTestTeapot();

	// DrawElementsBaseVertex
	//model_matrix = (vmath::translate(1.0f, 0.0f, -5.0f) * vmath::rotate(xRot, Y) * vmath::rotate(yRot, X));

	//glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, model_matrix);
	//glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL, 0);

	//// DrawArraysInstanced
	//model_matrix = (vmath::translate(3.0f, 0.0f, -5.0f) * vmath::rotate(xRot, Y) * vmath::rotate(yRot, X));

	//glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, model_matrix);
	//glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
	

	/* swap front and back buffers */
	glFlush();
	glutPostRedisplay();
}

void Sen_03_DrawCommands::reshape(int width, int height)
{
	//SenAbstractGLWidget::reshape(width, height);

	glViewport(0, 0, width, height);
	aspect = float(height) / float(width);
	//glutPostRedisplay();
}


void Sen_03_DrawCommands::initCube()
{
	// 8 corners of a cube, side length 2, centered on the origin
	static const GLfloat cube_positions[] =
	{
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	// Color for each vertex
	static const GLfloat cube_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	};

	// Indices for the triangle strips
	static const GLushort cube_indices[] =
	{
		0, 1, 2, 3, 6, 7, 4, 5,         // First strip
		0xFFFF,                         // <<-- This is the restart index
		2, 6, 0, 4, 1, 5, 3, 7          // Second strip
	};

	// Set up the element array buffer
	glGenBuffers(1, verIndicesObjArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verIndicesObjArray[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	// Set up the vertex attributes
	glGenBuffers(1, verBufferObjArray);
	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions) + sizeof(cube_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_positions), cube_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_positions), sizeof(cube_colors), cube_colors);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(cube_positions));
	glEnableVertexAttribArray(1);
}

void Sen_03_DrawCommands::initFourTriangles()
{
	// A single triangle
	static const GLfloat vertex_positions[] =
	{
		-5.0f, -5.0f, 0.0f, 1.0f,
		5.0f, -5.0f, 0.0f, 1.0f,
		-5.0f, 5.0f, 0.0f, 1.0f,
		5.0f, 5.0f, 0.0f, 1.0f,
	};

	// Color for each vertex
	static const GLfloat vertex_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f
	};

	// 8 corners of a cube, side length 2, centered on the origin
	static const GLushort vertex_indices[] =
	{
		0, 1, 2, 3, 2, 1
	};

	// Set up the element array buffer
	glGenBuffers(1, verIndicesObjArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verIndicesObjArray[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

	// Set up the vertex attributes
	glGenBuffers(1, verBufferObjArray);
	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertex_positions));
	glEnableVertexAttribArray(1);
}

void Sen_03_DrawCommands::initTeapot()
{
	glGenBuffers(1, verIndicesObjArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verIndicesObjArray[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(testTeapotIndices), testTeapotIndices, GL_STATIC_DRAW);

	// Set up the vertex attributes
	glGenBuffers(1, verBufferObjArray);
	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testTeapotVertices), testTeapotVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}