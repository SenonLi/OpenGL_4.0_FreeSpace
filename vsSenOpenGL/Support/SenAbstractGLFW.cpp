#include "SenAbstractGLFW.h"


SenAbstractGLFW::SenAbstractGLFW()
	:widgetHeight(640), widgetWidth(640), xRot(0), yRot(0), aspect(1.0)
{
	strWindowName = "Sen GLFW Application";
}

SenAbstractGLFW::~SenAbstractGLFW()
{
	OutputDebugString("\n ~SenAbstractGLWidget()\n");
}

void SenAbstractGLFW::paintGL(void)
{
	// Define the viewport dimensions
	glfwGetFramebufferSize(widgetGLFW, &widgetWidth, &widgetHeight);
	glViewport(0, 0, widgetWidth, widgetHeight);
}

void SenAbstractGLFW::initialGlfwGlewGL()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	widgetGLFW = glfwCreateWindow(widgetWidth, widgetHeight, strWindowName, nullptr, nullptr);
	glfwSetWindowPos(widgetGLFW, 400, 240);
	glfwMakeContextCurrent(widgetGLFW);

	// Set the required callback functions
	keyboardRegister();

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	glViewport(0, 0, widgetWidth, widgetHeight);

	glEnable(GL_DEPTH_TEST);

	OutputDebugString("\n Initial OpenGL\n");
}

void SenAbstractGLFW::finalize(void)
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, verArrObjArray);
	glDeleteBuffers(1, verBufferObjArray);
}

void SenAbstractGLFW::showWidget()
{
	initialGlfwGlewGL();

	//glutKeyboardFunc(_Key);    /* register callback for keyboard input */
	//glutMouseFunc(mousePress);
	////glutDisplayFunc(SenAbstractGLWidget::_Draw);    /* register callback for window redraw */
	//reshapeRegister();
	//drawMethodRegister();
	//mouseMoveRegister();


	// Game loop
	while (!glfwWindowShouldClose(widgetGLFW))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		paintGL();

		// Swap the screen buffers
		glfwSwapBuffers(widgetGLFW);
	}

	finalize();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


SenAbstractGLFW* currentInstance;

extern "C" void _KeyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode)
{
	currentInstance->_protectedKeyDetection(widget, key, scancode, action, mode);
}

void SenAbstractGLFW::keyboardRegister()
{
	::currentInstance = this;
	::glfwSetKeyCallback(widgetGLFW, _KeyDetection);

}

void SenAbstractGLFW::keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(widget, GL_TRUE);
}

