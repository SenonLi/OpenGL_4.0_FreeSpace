#include "../stdafx.h"

//-------------------------- OpenGL Graphics --------------------------------------
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>       // Support basic OpenGL
#endif

#include <GLFW/glfw3.h>

#include "SLAbstractGLFW_GLEW.h"

#include <stdio.h>  
#include <iostream>
//#include <string>
//#include <stdlib.h>

static const GLint DEFAULT_GLFW_WidgetWidth = 640;
static const GLint DEFAULT_GLFW_WidgetHeight = 640;
static const int SLOpenGL_MajorVersion = 4;
static const int SLOpenGL_MinorVersion = 3;

SLAbstractGLFW_GLEW::SLAbstractGLFW_GLEW()
{
	m_WindowName = "Sen GLFW Application";

	m_WidgetWidth = DEFAULT_GLFW_WidgetWidth;
	m_WidgetHeight = DEFAULT_GLFW_WidgetHeight;
}

SLAbstractGLFW_GLEW::~SLAbstractGLFW_GLEW()
{
	OutputDebugString(L"\n ~SLAbstractGLFW_GLEW()\n");
}

void SLAbstractGLFW_GLEW::InitGlfwGlew()
{
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, SLOpenGL_MajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, SLOpenGL_MinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef _DEBUG
	// comment this line in a release build! 
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	// Create a GLFWwindow object that we can use for GLFW's functions
	m_WidgetGLFW = glfwCreateWindow(m_WidgetWidth, m_WidgetHeight, m_WindowName.c_str(), nullptr, nullptr);
	glfwSetWindowPos(m_WidgetGLFW, 400, 240);
	glfwMakeContextCurrent(m_WidgetGLFW);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Due to a bug in GLEW the glewInit call always generates an OpenGL error; 
	// clear the flag(s) by calling glGetError();
	glGetError();
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

	glViewport(0, 0, m_WidgetWidth, m_WidgetHeight);

	glEnable(GL_DEPTH_TEST);

	OutputDebugString(L"\n Initial OpenGL\n");
}

void SLAbstractGLFW_GLEW::ShowWidget()
{
	InitGlfwGlew();
	InitPaintContext();

	// Game loop
	while (!glfwWindowShouldClose(m_WidgetGLFW))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Define the viewport dimensions
		glfwGetFramebufferSize(m_WidgetGLFW, &m_WidgetWidth, &m_WidgetHeight);
		glViewport(0, 0, m_WidgetWidth, m_WidgetHeight);

		PaintGL();

		// Swap the screen buffers
		glfwSwapBuffers(m_WidgetGLFW);
	}

	FinalizeGL();// all the clean up works

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	PostPaintProcess(); //  Pure non-UI/non-Graphics data process codes before shut-down.
}




GLenum SLOpenGL_CheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}


void APIENTRY SLOpenGL_DebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei /*length*/,
	const GLchar *message,
	const void* /*userParam*/)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}
