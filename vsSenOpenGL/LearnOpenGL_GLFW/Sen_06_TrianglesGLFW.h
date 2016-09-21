#pragma once
#include "SenAbstractGLFW.h"
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

