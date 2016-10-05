#include "Sen_30_GeometryShow.h"


Sen_30_GeometryShow::Sen_30_GeometryShow()
{
	strWindowName = "Sen GLFW Geometry";
}


Sen_30_GeometryShow::~Sen_30_GeometryShow()
{
}

void Sen_30_GeometryShow::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Draw container
	glUseProgram(programA);

	// Draw Background Texture
	glBindVertexArray(verArrObjArray[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_POINTS, 0, 2);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_POINTS, 2, 2);
	glBindVertexArray(0);
}

void Sen_30_GeometryShow::finalize(void)
{
	glDeleteVertexArrays(1, verArrObjArray);
	glDeleteBuffers(1, verBufferObjArray);
}

void Sen_30_GeometryShow::initialGlfwGlewGL()
{
	SenAbstractGLFW::initialGlfwGlewGL();

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER,   "./LearnOpenGL_GLFW/Shaders/Sen_30_GeometryShow.vert" },
		{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_30_GeometryShow.frag" },
		{ GL_GEOMETRY_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_30_GeometryShow.geom" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders);

	initialVertices();


	OutputDebugString(" Initial GLFW Texture\n\n");
}


void Sen_30_GeometryShow::initialVertices()
{
	// Vertex data
	GLfloat points[] = {
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // Bottom-left
	};

	glGenVertexArrays(1, verArrObjArray);
	glGenBuffers(1, verBufferObjArray);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(verArrObjArray[0]);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	//glBindVertexArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}
