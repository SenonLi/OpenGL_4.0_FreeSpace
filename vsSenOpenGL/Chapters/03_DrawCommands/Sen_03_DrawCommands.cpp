#include "Sen_03_DrawCommands.h"



Sen_03_DrawCommands::Sen_03_DrawCommands()
{
	strWindowName = "3D Four Triangles";
}


Sen_03_DrawCommands::~Sen_03_DrawCommands()
{
}

void Sen_03_DrawCommands::initialGlutGlewGL()
{
	int one = 1;
	char * name = "name";

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(140, 140);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);// | GLUT_DOUBLE );

	glutInit(&one, &name);

	//glutInitContextProfile(GLUT_CORE_PROFILE);
	//glutInitContextVersion(4, 0);
	glutCreateWindow("OpenGL Application");

	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}
	//SenAbstractGLWidget::initialGlutGlewGL();


	ShaderInfo shader_info[] =
	{
		{ GL_VERTEX_SHADER, "./Chapters/03_DrawCommands/primitive_restart.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "./Chapters/03_DrawCommands/primitive_restart.fs.glsl" },
		{ GL_NONE, NULL }
	};

	render_prog = LoadShaders(shader_info);

	glUseProgram(render_prog);

	// "model_matrix" is actually an array of 4 matrices
	render_model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
	render_projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");

	// A single triangle
	static const GLfloat vertex_positions[] =
	{
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
	};

	// Color for each vertex
	static const GLfloat vertex_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f
	};

	// Indices for the triangle strips
	static const GLushort vertex_indices[] =
	{
		0, 1, 2
	};

	// Set up the element array buffer
	glGenBuffers(1, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

	// Set up the vertex attributes
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertex_positions));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	OutputDebugString(" Initial Triangles\n\n");
}

void Sen_03_DrawCommands::paintGL(void)
{
	SenAbstractGLWidget::paintGL();

	float t = float(GetTickCount() & 0x1FFF) / float(0x1FFF);
	static float q = 0.0f;
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	static const vmath::vec4 black = vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	vmath::mat4 model_matrix;

	// Setup
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glClearBufferfv(GL_COLOR, 0, black);

	// Activate simple shading program
	glUseProgram(render_prog);

	// Set up the model and projection matrix
	//vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 500.0f));
	vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 500.0f));
	glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

	// Set up for a glDrawElements call
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

	// Draw Arrays...
	model_matrix = vmath::translate(-3.0f, 0.0f, -5.0f);
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// DrawElements
	model_matrix = vmath::translate(-1.0f, 0.0f, -5.0f);
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);

	// DrawElementsBaseVertex
	model_matrix = vmath::translate(1.0f, 0.0f, -5.0f);
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL, 1);

	// DrawArraysInstanced
	model_matrix = vmath::translate(3.0f, 0.0f, -5.0f);
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);   




	/* swap front and back buffers */
	glFlush();

	glutPostRedisplay();
}