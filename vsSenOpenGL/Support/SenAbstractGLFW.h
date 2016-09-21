#pragma once
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
	//virtual void reshape(int width, int height);
	//virtual void mouseMove(int x, int y);

	//int qtNormalizeAngle(int angle);



	// shader info variables
	GLuint programA, programB;
	GLuint defaultTexture;

	GLuint verArrObjArray[2];
	GLuint verBufferObjArray[2];
	GLuint verIndicesObjArray[2];

	GLint modelMatrixLocation;
	GLint projectionMatrixLocation;


};



// Function prototypes
void _KeyDetection(GLFWwindow* widgetGLFW, int key, int scancode, int action, int mode);

