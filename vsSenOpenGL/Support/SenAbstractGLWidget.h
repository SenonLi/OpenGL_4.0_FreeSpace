#pragma once
#include <stdlib.h>
#include <stdio.h>  
#include <iostream>
#include <Windows.h>
#include <string>

#include "vgl.h"
#include "LoadShaders.h"

using namespace std;

class SenAbstractGLWidget
{
public:
	SenAbstractGLWidget();
	virtual ~SenAbstractGLWidget();

	void showWidget();
	void _protectedPaintGL() { paintGL(); }
	void _protectedReshape(int width, int height) { reshape(width, height); }
	void _protectedMouseMove(int x, int y) { mouseMove(x,  y); }

protected:
	char* strWindowName;
	float xRot, yRot;
	float aspect;

	virtual void initialGlutGlewGL();
	virtual void paintGL();
	virtual void reshape(int width, int height);
	virtual void mouseMove(int x, int y);

	int qtNormalizeAngle(int angle);

	
	// shader info variables
	GLuint verArrObjArray[1];
	GLuint program;
	GLuint verBufferObjArray[1];
	GLuint verIndicesObjArray[1];

	GLint modelMatrixLocation;
	GLint projectionMatrixLocation;

	void vglAttachShaderSource(GLuint prog, GLenum type, const char * source)
	{
		GLuint sh;

		sh = glCreateShader(type);
		glShaderSource(sh, 1, &source, NULL);
		glCompileShader(sh);
		char buffer[4096];
		glGetShaderInfoLog(sh, sizeof(buffer), NULL, buffer);
		glAttachShader(prog, sh);
		glDeleteShader(sh);
	}

private:
	void drawMethodRegister();
	void mouseMoveRegister();
	void reshapeRegister();

};



void mousePress(int button, int state, int x, int y);

static void _Key(unsigned char key, int x, int y);