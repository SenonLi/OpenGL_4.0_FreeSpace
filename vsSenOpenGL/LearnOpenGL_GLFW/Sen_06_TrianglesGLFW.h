#pragma once
#ifndef __Sen_06_TrianglesGLFW__
#define __Sen_06_TrianglesGLFW__

#include "Support/SenAbstractGLFW.h"


class Sen_06_TrianglesGLFW :	public SenAbstractGLFW
{
public:
	Sen_06_TrianglesGLFW();
	virtual ~Sen_06_TrianglesGLFW();

protected:
	void paintGL(void);
	void initialGlfwGlewGL();
	void finalize();

};

#endif