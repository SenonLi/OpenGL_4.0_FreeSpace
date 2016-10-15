#include "Sen_06_TrianglesGLFW.h"


Sen_06_TrianglesGLFW::Sen_06_TrianglesGLFW()
{
	strWindowName = "Sen GLFW Triangle";
}

Sen_06_TrianglesGLFW::~Sen_06_TrianglesGLFW()
{
}

void Sen_06_TrianglesGLFW::initGlfwGlewGL()
{

	SenAbstractGLFW::initGlfwGlewGL();

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_06_Triangle.vert" },
		{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_06_Triangle.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders);


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions         // Colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom Left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Top 
	};


	glGenVertexArrays(1, verArrObjArray);
	glGenBuffers(1, verBufferObjArray);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(verArrObjArray[0]);

	glBindBuffer(GL_ARRAY_BUFFER, verBufferObjArray[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertexBufferObject as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind vertexArrayObject (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	OutputDebugString(" Initial Triangles\n\n");
}

void Sen_06_TrianglesGLFW::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Draw our first triangle
	glUseProgram(programA);
	glBindVertexArray(verArrObjArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Sen_06_TrianglesGLFW::finalize(void)
{
	if( glIsVertexArray(verArrObjArray[0]))	glDeleteVertexArrays(1, verArrObjArray);	
	if (glIsBuffer(verBufferObjArray[0]))	glDeleteBuffers(1, verBufferObjArray);

	if (glIsProgram(programA))				glDeleteProgram(programA);
}