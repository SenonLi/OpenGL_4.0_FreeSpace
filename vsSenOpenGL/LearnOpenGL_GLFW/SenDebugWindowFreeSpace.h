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
	

	// ==== Debug Window Vertex Attributes + FrameBuffer ====
	virtual void initDebugWindowProgram();
	
	void initDebugWindowVertexAttributes();
	void initDebugWindowFrameBuffer();


	GLuint debugWindowProgram, debugWindowOutlineProgram;
	GLuint debugWindowVAO,		debugWindowOutlineVAO;
	GLuint debugWindowVBO,		debugWindowOutlineVBO;

	GLuint debugWindowFrameBufferObject;
	GLuint debugWindowRGB_TextureAttach, debugWindowDepthStencil_RBO;



};



#endif // __SenFrameBufferFreeSpace__


