#pragma once
#ifndef __SenFrameBufferFreeSpace__
#define __SenFrameBufferFreeSpace__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

class SenDebugWindowFreeSpace :	public SenFreeSpaceAbstract
{
public:
	SenDebugWindowFreeSpace();
	virtual ~SenDebugWindowFreeSpace();

protected:
	virtual void paintScene()	{ ; }
	virtual void cleanDebugWindowFreeSpace() { ; }

	void initGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);
	
	void keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode);
	GLboolean debugWindowSwitchPressing, debugWindowSwitch;

	// ==== Debug Window Vertex Attributes + FrameBuffer ====
	virtual void initDebugWindowProgram();
	virtual void paintDebugWindowFrameBufferGL();

	void initDebugWindowVertexAttributes();
	void initDebugWindowFrameBuffer();


	GLuint debugWindowProgram, debugWindowOutlineProgram;
	GLuint debugWindowVAO,		debugWindowOutlineVAO;
	GLuint debugWindowVBO,		debugWindowOutlineVBO;

	GLuint debugWindowFrameBufferObject;
	GLuint debugWindowRGB_TextureAttach, debugWindowDepthStencil_RBO;

	GLint debugWindowFrameBufferWidth, debugWindowFrameBufferHeight;

};



#endif // __SenFrameBufferFreeSpace__


