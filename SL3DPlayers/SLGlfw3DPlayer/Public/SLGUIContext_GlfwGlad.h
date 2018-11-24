#ifndef __SLGUICONTEXT_GLFWGLAD__
#define __SLGUICONTEXT_GLFWGLAD__
#pragma once

extern "C" { 
	typedef unsigned int GLuint;
	typedef int GLint;
	typedef int GLsizei;
	typedef char GLchar;
	typedef unsigned int GLenum;
}

#define SLOpenGL_CheckError() SLOpenGL_CheckError_(__FILE__, __LINE__)


/// <summary> Be sure to include GLAD before GLFW. The include file for GLAD contains the correct OpenGL header includes
///    (like GL/gl.h) so including GLAD before other header files that require OpenGL does the trick. 
///
///    SLGUIContext_GlfwGlad is desgined as a singleton; any GlfwWindow related and OpenGL extension related commands should go through SLGUIContext_GlfwGlad.
/// <summary>

class SLGUIContext_GlfwGlad
{
public:
	static SLGUIContext_GlfwGlad& GetInstance();

	void InitialGUIContext();
	void TerminateGUIContext();

private:
	static std::unique_ptr<SLGUIContext_GlfwGlad> m_Instance;

	/// <remark>GLFW seems not support the check of initialed or not, 
	// we do it on our own for multiple GLFW window without redundant call of GLFW/GLAD initial</remark>
	static bool m_IsGlfwGladInitialed;

	SLGUIContext_GlfwGlad() = default;
	SLGUIContext_GlfwGlad(const SLGUIContext_GlfwGlad&) = delete;
	SLGUIContext_GlfwGlad& operator = (const SLGUIContext_GlfwGlad&) = delete;
};


GLenum SLOpenGL_CheckError_(const char *file, int line);

void APIENTRY SLOpenGL_DebugOutput(GLenum source,	GLenum type,	GLuint id,
		GLenum severity,	GLsizei length,	const GLchar *message,	const void *userParam);


#endif //__SLGUICONTEXT_GLFWGLAD__