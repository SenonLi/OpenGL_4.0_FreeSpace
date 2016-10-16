#include "SenFreeSpaceAbstract.h"

SenFreeSpaceAbstract::SenFreeSpaceAbstract()
	:textureImagePtr(NULL)
{
	strWindowName = "Sen GLFW Free Space";
	widgetWidth = SenFREESPACE_widgetWidth;
	widgetHeight = SenFREESPACE_widgetHeight;
}

SenFreeSpaceAbstract::~SenFreeSpaceAbstract()
{

}

void SenFreeSpaceAbstract::initGlfwGlewGL()
{
	SenAbstractGLFW::initGlfwGlewGL();

	cursorPositionHandlerRegister();
	mouseScrollHandlerRegister();

	// Options
	glfwSetInputMode(widgetGLFW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	projection = glm::perspective(float(glm::radians(60.0)), (GLfloat)widgetWidth / (GLfloat)widgetHeight, 0.1f, 100.0f);
	SenFreeSpaceLogoCube.initialCubeGL();
	glClearColor((float)0x87 / (float)0xFF, (float)0xCE / float(0xFF), (float)0xEB / float(0xFF), 1.0f);
	

	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_ALWAYS, 0x01, 0xFF);    // Check GL_STENCIL_TEST only when needed;  All fragments should update the
	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//glStencilMask(0x00); // Disable writing to the stencil buffer

	OutputDebugString(" SenFreeSpaceAbstract Initial \n\n");
}

void SenFreeSpaceAbstract::paintGL(void)
{
	SenAbstractGLFW::paintGL();
	
	// Set CameraView and Projection
	GLfloat currentFrame = GLfloat(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	Do_Movement();

	view = camera.GetViewMatrix();
	projection = glm::perspective(camera.Zoom, (float)widgetWidth / (float)widgetHeight, 0.1f, 100.0f);


	// Paint from SubClass
	paintFreeSpaceGL();


	// Paint Sen FreeSpace Logo
	SenFreeSpaceLogoCube.paintSenLogoCube(GLfloat(widgetWidth) / GLfloat(SenFREESPACE_widgetWidth), GLfloat(widgetHeight) / GLfloat(SenFREESPACE_widgetHeight));
}

void SenFreeSpaceAbstract::finalize(void)
{
	cleanFreeSpace();

	// Clean SenFreeSpaceLogo
	SenFreeSpaceLogoCube.finalizeCube();
}


void SenFreeSpaceAbstract::keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(widget, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}


#pragma region "C style Registration of Class Method for Inheritance"

SenFreeSpaceAbstract* currentInstance;


extern "C" void _CursorPositionDetect(GLFWwindow* widget, double xpos, double ypos)
{
	currentInstance->_protectedCursorPositionDetect(widget, xpos, ypos);
}

void SenFreeSpaceAbstract::cursorPositionHandlerRegister()
{
	::currentInstance = this;
	::glfwSetCursorPosCallback(widgetGLFW, _CursorPositionDetect);
	//glfwSetScrollCallback(widgetGLFW, scroll_callback);
}

void SenFreeSpaceAbstract::cursorPositionDetect(GLFWwindow* widget, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = GLfloat(xpos);
		lastY = GLfloat(ypos);
		firstMouse = false;
	}

	GLfloat xoffset = GLfloat(xpos) - lastX;
	GLfloat yoffset = lastY - GLfloat(ypos);  // Reversed since y-coordinates go from bottom to left

	lastX = GLfloat(xpos);
	lastY = GLfloat(ypos);

	camera.ProcessMouseMovement(xoffset, yoffset);
}


extern "C" void _mouseScrollHandler(GLFWwindow* widget, double xoffset, double yoffset)
{
	currentInstance->_protectedMouseScrollHandler(widget, xoffset, yoffset);
}

void SenFreeSpaceAbstract::mouseScrollHandlerRegister()
{
	::currentInstance = this;
	::glfwSetScrollCallback(widgetGLFW, _mouseScrollHandler);
}

void SenFreeSpaceAbstract::mouseScrollHandler(GLFWwindow* widget, double xoffset, double yoffset)
{
	yoffset /= DEFAULT_zoomRatio;
	camera.ProcessMouseScroll(GLfloat(yoffset));
}

#pragma endregion