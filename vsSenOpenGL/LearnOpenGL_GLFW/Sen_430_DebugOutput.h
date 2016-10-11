#pragma once

#ifndef __Sen_430_DebugOutput__
#define __Sen_430_DebugOutput__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

class Sen_430_DebugOutput :	public SenFreeSpaceAbstract
{
public:
	Sen_430_DebugOutput();
	virtual ~Sen_430_DebugOutput();

protected:
	void initialGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);


	float viewCenter = -3.0f;
};


#endif