#include "Sen_01_Triangles.h"

Sen_01_Triangles::Sen_01_Triangles()
{
	strWindowName = "2D Two Triangles";
}


Sen_01_Triangles::~Sen_01_Triangles()
{
	glDeleteVertexArrays(1, verArrObjArray);
	OutputDebugString("\n ~Sen_01_Triangles()\n");
}


void Sen_01_Triangles::initialGlutGlewGL()
{
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	SenAbstractGLWidget::initialGlutGlewGL();

	glGenVertexArrays(1, verArrObjArray);
	glBindVertexArray(verArrObjArray[0]);
	GLfloat vertices[6][2] = {
		{ -0.90, -0.90 }, // Triangle 1
		{ 0.85, -0.90 },
		{ -0.90, 0.85 },
		{ 0.90, -0.85 }, // Triangle 2
		{ 0.90, 0.90 },
		{ -0.85, 0.90 }
	};
	glGenBuffers(1, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),	vertices, GL_STATIC_DRAW);
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./Chapters/01_Triangles/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "./Chapters/01_Triangles/triangles.frag" },
		{ GL_NONE, NULL }
	};
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	OutputDebugString(" Initial Triangles\n\n");
}

void Sen_01_Triangles::paintGL(void)
{
	SenAbstractGLWidget::paintGL();

	glBindVertexArray(verArrObjArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	/* swap front and back buffers */
	glFlush();
}