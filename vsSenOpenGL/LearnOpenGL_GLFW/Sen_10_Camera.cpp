#include "Sen_10_Camera.h"


Sen_10_Camera::Sen_10_Camera()
{
	strWindowName = "Sen First Cube in FreeSpace";
}


Sen_10_Camera::~Sen_10_Camera()
{
}

void Sen_10_Camera::initGlfwGlewGL()
{
	SenFreeSpaceAbstract::initGlfwGlewGL();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


	//glfwSetInputMode(widgetGLFW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.frag" },
	//	{ GL_NONE, NULL }
	//};
	//programA = LoadShaders(shaders);


	backgroundCube.setCubeWorldAddress(glm::vec3(1.0f, 0.0f, viewCenter));
	backgroundCube.initialCubeGL();
	backgroundCube.setCubeRotation(glm::vec3(1.0f, 0.0f, 1.0f), 75.0f);

	OutputDebugString(" Sen_10 First Cube Initial \n\n");
}

void Sen_10_Camera::paintFreeSpaceGL(void)
{
	backgroundCube.paintCube(projection, view);
}


void Sen_10_Camera::cleanFreeSpace(void)	{
	backgroundCube.finalizeCube();
}