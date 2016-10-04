#pragma once
#ifndef __Sen_03_DrawCommands__
#define __Sen_03_DrawCommands__

#include "SenAbstractGLWidget.h"
//#include "vapp.h"
#include "Guide8th/vmath.h"

class Sen_03_DrawCommands :	public SenAbstractGLWidget
{
public:
	Sen_03_DrawCommands();
	virtual ~Sen_03_DrawCommands();

protected:
	void paintGL(void);
	void initialGlutGlewGL();
	void reshape(int width, int height);
	//void mouseMove(int x, int y);

	// Member variables
	GLuint render_prog;
	//GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[1];

	GLint render_model_matrix_loc;
	GLint render_projection_matrix_loc;

	const vmath::vec3 x_axis = vmath::vec3(1.0f, 0.0f, 0.0f);
	const vmath::vec3 y_axis = vmath::vec3(0.0f, 1.0f, 0.0f);

private:
	void initCube();
	void initFourTriangles();
	void initTeapot();
};


#endif // __Sen_03_DrawCommands__
