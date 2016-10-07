#pragma once
#ifndef __Sen_06_CubeMap__
#define __Sen_06_CubeMap__

#include "Support/SenAbstractGLWidget.h"

//#include "vapp.h"
#include "Guide8th/vmath.h"


class Sen_06_CubeMap :	public SenAbstractGLWidget
{
public:
	Sen_06_CubeMap();
	virtual ~Sen_06_CubeMap();

protected:
	void paintGL(void);
	void initialGlutGlewGL();

	// Member variables
	char* pChapterWindow;

	enum Attrib_IDs { vPosition = 0 };
	GLuint verArrObjArray[1];
	GLuint Buffers[1];

	GLint render_model_matrix_loc;
	GLint render_projection_matrix_loc;

};





#endif
