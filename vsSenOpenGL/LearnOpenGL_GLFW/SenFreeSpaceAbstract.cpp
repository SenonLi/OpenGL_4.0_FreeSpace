#include "SenFreeSpaceAbstract.h"

SenFreeSpaceAbstract::SenFreeSpaceAbstract()
{
	strWindowName = "Sen GLFW Free Space";

	//widgetWidth = int(widgetWidth*1.5);
}

SenFreeSpaceAbstract::~SenFreeSpaceAbstract()
{

}

void SenFreeSpaceAbstract::initialGlfwGlewGL()
{
	SenAbstractGLFW::initialGlfwGlewGL();

	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.frag" },
	//	{ GL_NONE, NULL }
	//};
	//programA = LoadShaders(shaders);

	//projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f);
	projection = glm::perspective(float(glm::radians(60.0)), (GLfloat)widgetWidth / (GLfloat)widgetHeight, 0.1f, 100.0f);

	backgroundCube.initialCube();

	OutputDebugString(" Initial GLFW Texture\n\n");
}

void SenFreeSpaceAbstract::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Draw container
	glUseProgram(programA);
	glBindVertexArray(verArrObjArray[0]);

	//GLfloat radius = abs(viewCenter);
	//GLfloat camX = float(sin(glfwGetTime()) * radius);
	//GLfloat camZ = float(viewCenter + cos(glfwGetTime()) * radius);
	//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, viewCenter), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, viewCenter), glm::vec3(0.0f, 1.0f, 0.0f));



	glBindVertexArray(0);


	backgroundCube.setCubeWorldAddress(glm::vec3(0.0f, 0.0f, viewCenter));
	backgroundCube.paintCube(projection, view);
}


void SenFreeSpaceAbstract::finalize(void)
{
	glDeleteVertexArrays(1, verArrObjArray);
	glDeleteBuffers(1, verBufferObjArray);
	glDeleteBuffers(1, verIndicesObjArray);

}