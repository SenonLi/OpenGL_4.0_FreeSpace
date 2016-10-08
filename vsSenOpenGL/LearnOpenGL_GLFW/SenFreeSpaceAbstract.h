#pragma once

#ifndef __SenFreeSpaceAbstract__
#define __SenFreeSpaceAbstract__

#include <soil/SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Support/SenAbstractGLFW.h"
#include "Support/SenCameraViewModel.h"
#include "SenFreeSpacePieces/Sen_Cube.h"


class SenFreeSpaceAbstract : public SenAbstractGLFW
{
public:
	SenFreeSpaceAbstract();
	virtual ~SenFreeSpaceAbstract();

	void _protectedCursorPositionDetect(GLFWwindow* widget, double xpos, double ypos)	{
		cursorPositionDetect(widget, xpos, ypos);
	}
	void _protectedMouseScrollHandler(GLFWwindow* widget, double xoffset, double yoffset)	{
		mouseScrollHandler(widget, xoffset, yoffset);
	}

protected:
	virtual void cursorPositionDetect(GLFWwindow* widget, double xpos, double ypos);
	virtual void mouseScrollHandler(GLFWwindow* widget, double xoffset, double yoffset);
	virtual void paintFreeSpaceGL(void)	{ ; }
	virtual void cleanFreeSpace(void)	{ ; }

	void paintGL(void);
	void initialGlfwGlewGL();
	void finalize();

	void keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode);


	glm::mat4 model, view, projection;



	// CameraViewModel
	SenCameraViewModel camera;

	bool keys[1024];
	GLfloat lastX = 400, lastY = 300;
	bool firstMouse = true;

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Moves/alters the camera positions based on user input
	void Do_Movement()
	{
		// CameraViewModel controls
		if (keys[GLFW_KEY_W])
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (keys[GLFW_KEY_S])
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (keys[GLFW_KEY_A])
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (keys[GLFW_KEY_D])
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}


	const GLint SenFREESPACE_widgetWidth = 960;
	const GLint SenFREESPACE_widgetHeight = GLint(SenFREESPACE_widgetWidth * SenGOLDEN_SectionScale);

private:
	void cursorPositionHandlerRegister();
	void mouseScrollHandlerRegister();

	Sen_Cube SenFreeSpaceLogoCube;

};


#endif // __SenFreeSpaceAbstract__