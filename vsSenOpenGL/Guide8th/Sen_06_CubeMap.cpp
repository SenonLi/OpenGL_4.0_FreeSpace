#include "Sen_06_CubeMap.h"
//#include "vutils.h"


Sen_06_CubeMap::Sen_06_CubeMap()
{
	pChapterWindow = "Cube Map";
}


Sen_06_CubeMap::~Sen_06_CubeMap()
{


	OutputDebugString("\n ~Sen_01_Triangles()\n");

}

void Sen_06_CubeMap::initialGlutGlewGL()
{
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextVersion(4, 0);


	SenAbstractGLWidget::initialGlutGlewGL();

	//glGenVertexArrays(1, verArrObjArray);
	//glBindVertexArray(verArrObjArray[0]);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo shaders[] = {
		//{ GL_VERTEX_SHADER, "./Chapters/01_Triangles/triangles.vert" },
		//{ GL_FRAGMENT_SHADER, "./Chapters/01_Triangles/triangles.frag" },
		{ GL_VERTEX_SHADER, "./Guide8th/Shaders/Sen_06_CubeMap.vert" },
		{ GL_FRAGMENT_SHADER, "./Guide8th/Shaders/Sen_06_CubeMap.frag" },
		{ GL_NONE, NULL }
	};
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);


	// "model_matrix" is actually an array of 4 matrices
	render_model_matrix_loc = glGetUniformLocation(program, "model_matrix");
	render_projection_matrix_loc = glGetUniformLocation(program, "projection_matrix");



	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	OutputDebugString(" Initial Triangles\n\n");
}

void Sen_06_CubeMap::paintGL(void)
{
	SenAbstractGLWidget::paintGL();

	// Set up the model and projection matrix
	vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -1.0, 1.0, 1.0f, 500.0f));
	glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);
	vmath::mat4 model_matrix = vmath::translate(-1.0f, 0.0f, 5.0f);
	glUniformMatrix4fv(render_model_matrix_loc, 4, GL_FALSE, model_matrix);

	glBindVertexArray(verArrObjArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	/* swap front and back buffers */
	glFlush();
}