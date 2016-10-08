#include "Sen_10_Camera.h"


Sen_10_Camera::Sen_10_Camera()
{
	strWindowName = "Sen GLFW CameraModel";
}


Sen_10_Camera::~Sen_10_Camera()
{
}

void Sen_10_Camera::initialGlfwGlewGL()
{
	SenFreeSpaceAbstract::initialGlfwGlewGL();

	glfwSetInputMode(widgetGLFW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.frag" },
	//	{ GL_NONE, NULL }
	//};
	//programA = LoadShaders(shaders);


	backgroundCube.initialCube();

	OutputDebugString(" Sen_10_Camera Initial \n\n");
}

void Sen_10_Camera::paintGL(void)
{
	SenFreeSpaceAbstract::paintGL();

	backgroundCube.setCubeWorldAddress(glm::vec3(1.0f, 0.0f, viewCenter));
	backgroundCube.paintCube(projection, view);
}


void Sen_10_Camera::cleanFreeSpace(void)	{
	backgroundCube.finalize();
}