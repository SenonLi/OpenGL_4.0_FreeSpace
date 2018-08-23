#ifndef __SLABSTRACT_GLFW_GLEW__
#define __SLABSTRACT_GLFW_GLEW__
#pragma once

extern "C" { 
	typedef struct GLFWwindow GLFWwindow; 
	typedef unsigned int GLuint;
	typedef int GLint;
	typedef int GLsizei;
	typedef char GLchar;
	typedef unsigned int GLenum;
}

#define SLOpenGL_CheckError() SLOpenGL_CheckError_(__FILE__, __LINE__)


/// <summary>To use GLFW to initial window (UI), we have to know GLint, which means GLEW must be initialed before GLFW, they must be together. <summary>
/// <remark>Pure abstract class. To instancing SLAbstractGLFW_GLEW, one must define its InitPaintContext(), PaintGL() and Finalize() </remark>
class SLAbstractGLFW_GLEW
{
public:
	SLAbstractGLFW_GLEW();
	virtual ~SLAbstractGLFW_GLEW();

	void ShowWidget();

protected:
	GLFWwindow* m_WidgetGLFW;
	GLint m_WidgetWidth, m_WidgetHeight;
	std::string m_WindowName;

	/// <summary>Initial all required Paintables in corresponding Renderers, e.g., Vertices, ShaderProgram, Textures. </summary>
	virtual void InitPaintContext() = 0;
	
	/// <summary> Actual GL Paint/Draw commands </summary>
	virtual void PaintGL()          = 0;

	/// <summary>Deallocate/Free Memories on both GPU and CPU </summary>
	virtual void FinalizeGL()       = 0;

	/// <summary>Pure Data (Non-Graphic) Process, no OpenGL context at all. </summary>
	/// <remark>Put non-graphic after painting codes, e.g., data/results file writings, all in this function. </remark>
	virtual void PostPaintProcess() {};

private:
	void InitGlfwGlew();

};


GLenum SLOpenGL_CheckError_(const char *file, int line);

void APIENTRY SLOpenGL_DebugOutput(GLenum source,	GLenum type,	GLuint id,
		GLenum severity,	GLsizei length,	const GLchar *message,	const void *userParam);


#endif //__SLABSTRACT_GLFW_GLEW__