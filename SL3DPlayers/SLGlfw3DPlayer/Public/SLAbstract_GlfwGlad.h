#ifndef __SLABSTRACT_GLFWGLAD__
#define __SLABSTRACT_GLFWGLAD__
#pragma once

extern "C" {
	typedef struct GLFWwindow GLFWwindow;
	typedef int GLint;
}

class SLGlfwWindow 
{
public:
	void SetWidth(int width) { m_WidgetWidth = width; }
	void SetHeight(int height) { m_WidgetHeight = height; }
	void SetWindowName(const std::string& windowName) { m_WindowName = windowName; }
	void SetGlfwWindow(GLFWwindow* window) {
		assert(window);
		m_WidgetGLFW = window; 
	}

	GLint Width() const { return m_WidgetWidth; }
	GLint Height() const { return m_WidgetHeight; }
	const std::string& WindowName() const { return m_WindowName; }
	GLFWwindow* Widget() { return m_WidgetGLFW; }

private:
	GLFWwindow* m_WidgetGLFW = nullptr;
	GLint m_WidgetWidth = 0;
	GLint m_WidgetHeight = 0;
	std::string m_WindowName;
};

/// <summary>Be sure to include GLAD before GLFW. The include file for GLAD contains the correct OpenGL header includes (like GL/gl.h) so including GLAD before other header files that require OpenGL does the trick. <summary>
/// <remark>Pure abstract class. To instancing SLAbstract_GlfwGlad, one must define its InitPaintContext(), PaintGL(), GLResize() and Finalize() 
///     For now, SLAbstract_GlfwGlad is only for simply OpenGL rendering test, the application written in a dead loop for performance test.
///     If codes is called in MainApp instead of ConsoleApp, OOP should be fully implemented.
/// </remark>
class SLAbstract_GlfwGlad
{
public:
	SLAbstract_GlfwGlad();
	virtual ~SLAbstract_GlfwGlad();

	void ShowWidget();

	// GLFW callback is in C, so we have to make this public; To be overrided as protected!!
	virtual void GLResize(GLFWwindow* glfwWindow, int width, int height) = 0;

protected:
	SLGlfwWindow m_DefaultWidget;

	/// <summary>Initial all required Paintables in corresponding Renderers, e.g., Vertices, ShaderProgram, Textures. </summary>
	virtual void InitPaintContext() = 0;
	
	/// <summary> Actual GL Paint/Draw commands </summary>
	virtual void PaintGL() = 0;

	/// <summary>Deallocate/Free Memories on both GPU and CPU </summary>
	virtual void FinalizeGL() = 0;

	/// <summary>Pure Data (Non-Graphic) Process, no OpenGL context at all. </summary>
	/// <remark>Put non-graphic after painting codes, e.g., data/results file writings, all in this function. </remark>
	virtual void PostPaintProcess() {};

	// Create GLFWwindow, after which we could initial GLAD, should be override when two or more widgets are needed in one app
	void InitGlfwGladWidget();

private:
	/// <remark>GLFW seems not support the check of initialed or not, we do it on our own for multiple GLFW window without redundant call of GLFW initial</remark>
	static bool m_IsGlfwGladInitialed;
};


#endif //__SLABSTRACT_GLFWGLAD__