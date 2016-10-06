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
//#include "cppShaderLoad.h"
#include "LoadShaders.h"

class SenAbstractGLFW
{
public:
	SenAbstractGLFW();
	virtual ~SenAbstractGLFW();

	void showWidget();


protected:
	GLFWwindow* widgetGLFW;
	int widgetWidth, widgetHeight;
	char* strWindowName;
	float xRot, yRot;
	float aspect;

	virtual void initialGlfwGlewGL();
	virtual void paintGL();
	virtual void finalize();


	// shader info variables
	GLuint programA, programB;
	GLuint defaultTexture;
	GLint textureLocation;

	GLuint verArrObjArray[2];
	GLuint verBufferObjArray[2];
	GLuint verIndicesObjArray[2];

	GLint modelMatrixLocation;
	GLint projectionMatrixLocation;


};



// Function prototypes
void _KeyDetection(GLFWwindow* widgetGLFW, int key, int scancode, int action, int mode);








#endif //__SenAbstractGLFW__