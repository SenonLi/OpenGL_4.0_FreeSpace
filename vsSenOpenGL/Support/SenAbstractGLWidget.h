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
	void _protectedMouseMove(int x, int y) { mouseMove(x,  y); }

protected:
	virtual void initialGlutGlewGL();
	virtual void paintGL();

	char* strWindowName;
	float xRot, yRot;
	float aspect;

	int qtNormalizeAngle(int angle);
	void mouseMove(int x, int y);

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

};



void mousePress(int button, int state, int x, int y);

static void _Reshape(int width, int height);
static void _Key(unsigned char key, int x, int y);