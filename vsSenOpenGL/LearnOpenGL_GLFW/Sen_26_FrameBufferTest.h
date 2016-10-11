#pragma once

#ifndef __Sen_26_FrameBufferTest__
#define __Sen_26_FrameBufferTest__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

class Sen_26_FrameBufferTest :	public SenFreeSpaceAbstract
{
public:
	Sen_26_FrameBufferTest();
	virtual ~Sen_26_FrameBufferTest();

protected:
	void initialGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);

};


#endif