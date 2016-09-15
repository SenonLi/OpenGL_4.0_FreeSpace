#pragma once
#ifndef __Sen_03_DrawCommands__
#define __Sen_03_DrawCommands__

#include "SenAbstractGLWidget.h"
//#include "vapp.h"
#include "vmath.h"

class Sen_03_DrawCommands :	public SenAbstractGLWidget
{
public:
	Sen_03_DrawCommands();
	virtual ~Sen_03_DrawCommands();

protected:
	void paintGL(void);
	void initialGlutGlewGL();

	// Member variables
	GLuint render_prog;
	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[1];

	GLint render_model_matrix_loc;
	GLint render_projection_matrix_loc;
};


#endif // __Sen_03_DrawCommands__
