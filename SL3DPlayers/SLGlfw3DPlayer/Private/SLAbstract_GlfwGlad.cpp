#include "../stdafx.h"
#include "SLAbstract_GlfwGlad.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "../Public/SLGUIContext_GlfwGlad.h"

#include <stdio.h>  
#include <iostream>

static const GLint DEFAULT_GLFW_WidgetWidth = 640;
static const GLint DEFAULT_GLFW_WidgetHeight = 640;

SLAbstract_GlfwGlad* ptrSLApp_GlfwGladInstance;

extern "C" void FramebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
	ptrSLApp_GlfwGladInstance->GLResize(glfwWindow, width, height);
}

SLAbstract_GlfwGlad::SLAbstract_GlfwGlad()
{
	ptrSLApp_GlfwGladInstance = this;

	m_DefaultWidget.SetWindowName("Sen GLFW Abstract_GlfwGlad");
	m_DefaultWidget.SetWidth(DEFAULT_GLFW_WidgetWidth);
	m_DefaultWidget.SetHeight(DEFAULT_GLFW_WidgetHeight);
}

SLAbstract_GlfwGlad::~SLAbstract_GlfwGlad()
{
	SLOutputDebugString(L"\n ~SLAbstract_GlfwGlad()\n");
}

void SLAbstract_GlfwGlad::InitGlfwGladWidget()
{
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* widget = glfwCreateWindow(m_DefaultWidget.Width(), m_DefaultWidget.Height(), m_DefaultWidget.WindowName().c_str(), nullptr, nullptr);
	m_DefaultWidget.SetGlfwWindow(widget);

	glfwSetWindowPos(m_DefaultWidget.Widget(), 400, 240);
	glfwMakeContextCurrent(m_DefaultWidget.Widget());

	// Register: window re-size (by os or user resize) this callback function executes
	glfwSetFramebufferSizeCallback(m_DefaultWidget.Widget(), ::FramebufferResizeCallback);
	
	// glad: load all OpenGL function pointers
	// Attention: gladLoadGLLoader must be called after glfwMakeContextCurrent
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// enable OpenGL debug context if context allows for debug context
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
		glDebugMessageCallback(SLOpenGL_DebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	// Clear the colorbuffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, m_DefaultWidget.Width(), m_DefaultWidget.Height());
	glEnable(GL_DEPTH_TEST);

	SLOutputDebugString(L"\n Initial OpenGL\n");
}

/// <summary>GL widget rendering </summary>
/// <remark>We do initialization of GLFW_GLAD here instead of in the constructor,
///     in case the main UI has not been initialed when instancing SLAbstract_GlfwGlad </remark>
void SLAbstract_GlfwGlad::ShowWidget()
{
	SLGUIContext_GlfwGlad::GetInstance().InitialGUIContext();
	InitGlfwGladWidget();

	InitPaintContext();

	// Game loop
	while (!glfwWindowShouldClose(m_DefaultWidget.Widget()))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		PaintGL();

		// Swap the screen buffers
		glfwSwapBuffers(m_DefaultWidget.Widget());
	}

	// To enable un-repeated rendering, we put all the clean work inside destructor.
	FinalizeGL();// all the clean up works

	// Terminate GLFW, clearing any resources allocated by GLFW.
	SLGUIContext_GlfwGlad::GetInstance().TerminateGUIContext();

	PostPaintProcess(); //  Pure non-UI/non-Graphics data process codes before shut-down.
}

