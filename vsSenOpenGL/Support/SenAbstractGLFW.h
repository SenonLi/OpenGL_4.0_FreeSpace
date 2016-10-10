#pragma once

#ifndef __SenAbstractGLFW__
#define __SenAbstractGLFW__

#include <stdlib.h>
#include <stdio.h>  
#include <iostream>
#include <Windows.h>
#include <string>

#include "vgl.h"
#include <GLFW/glfw3.h>
#include "LoadShaders.h"

class SenAbstractGLFW
{
public:
	SenAbstractGLFW();
	virtual ~SenAbstractGLFW();

	void showWidget();

	void _protectedKeyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode) { 
		keyDetection(widget, key, scancode, action, mode);
	}

protected:
	GLFWwindow* widgetGLFW;
	GLint widgetWidth, widgetHeight;
	char* strWindowName;
	float xRot, yRot;
	float aspect;

	virtual void initialGlfwGlewGL();
	virtual void paintGL();
	virtual void finalize() { ; }

	virtual void keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode);


	// shader info variables
	GLuint programA, programB;
	GLuint defaultTextureID;
	GLint textureLocation;

	GLuint verArrObjArray[2];
	GLuint verBufferObjArray[2];
	GLuint verIndicesObjArray[2];

	GLint modelMatrixLocation;
	GLint projectionMatrixLocation;


	const GLint DEFAULT_widgetWidth = 640;
	const GLint DEFAULT_widgetHeight = 640;

private:
	void keyboardRegister();

};



#endif //__SenAbstractGLFW__