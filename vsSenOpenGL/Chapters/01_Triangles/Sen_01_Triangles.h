#pragma once

#include "SenAbstractGLWidget.h"

class Sen_01_Triangles : public SenAbstractGLWidget
{
public:
	Sen_01_Triangles();
	virtual ~Sen_01_Triangles();

protected:
	void paintGL(void);
	void initialGlutGlewGL();

	enum Attrib_IDs { vPosition = 0 };
	GLuint verArrObjArray[1];
	GLuint Buffers[1];
};
