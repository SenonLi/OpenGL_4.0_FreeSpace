#include "SenFreeSpaceAbstract.h"

SenFreeSpaceAbstract::SenFreeSpaceAbstract()
{
	strWindowName = "Sen GLFW Free Space";
}

SenFreeSpaceAbstract::~SenFreeSpaceAbstract()
{

}

void SenFreeSpaceAbstract::initialGlfwGlewGL()
{
	SenAbstractGLFW::initialGlfwGlewGL();

	cursorPositionHandlerRegister();
	mouseScrollHandlerRegister();

	// Options
	glfwSetInputMode(widgetGLFW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f);
	projection = glm::perspective(float(glm::radians(60.0)), (GLfloat)widgetWidth / (GLfloat)widgetHeight, 0.1f, 100.0f);

	OutputDebugString(" SenFreeSpaceAbstract Initial \n\n");
}

void SenFreeSpaceAbstract::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Set frame time
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	Do_Movement();

	view = camera.GetViewMatrix();
	projection = glm::perspective(camera.Zoom, (float)widgetWidth / (float)widgetHeight, 0.1f, 100.0f);



	//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

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
	yoffset /= 6.0;
	camera.ProcessMouseScroll(yoffset);
}

#pragma endregion