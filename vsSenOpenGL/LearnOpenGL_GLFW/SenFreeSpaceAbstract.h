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
	void initGlfwGlewGL();
	void finalize();

	void keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode);


	glm::mat4 model, view, projection;
	unsigned char* textureImagePtr;



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

	const GLfloat DEFAULT_zoomRatio = 6.0;
	const GLfloat SenFREESPACE_ZfightingOffset = 0.04f;
	const GLfloat SenGOLDEN_SectionScale = 0.618f;
	const GLint SenFREESPACE_widgetWidth = 960;
	const GLint SenFREESPACE_widgetHeight = GLint(SenFREESPACE_widgetWidth * SenGOLDEN_SectionScale);

	void uploadFreeSpaceTexture(const char* textureAddressPointer, GLuint &textureID, std::string channelType)	{
		int width, height;
		bool alpha;

		if (channelType == std::string("RGBA"))			alpha = true;
		else if (channelType == std::string("RGB"))		alpha = false;

		textureImagePtr = SOIL_load_image(textureAddressPointer, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

		// Load and create a defaultTextureID 
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTextureID object
		//// Set the defaultTextureID wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Set defaultTextureID wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureImagePtr);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(textureImagePtr);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
	}

	// Generates a texture that is suited for attachments to a framebuffer
	// Assume the widgetWidth, widgetHeight won't change
	// Considering their change, there should be a resize register function to re-initial this customer FrameBuffer
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, GLint frameBufferWidth, GLint frameBufferHeight)
	{
		// What enum to use?
		GLenum attachment_type;
		if (!depth && !stencil)
			attachment_type = GL_RGB;
		else if (depth && !stencil)
			attachment_type = GL_DEPTH_COMPONENT;
		else if (!depth && stencil)
			attachment_type = GL_STENCIL_INDEX;

		//Generate texture ID and load texture data 
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		if (!depth && !stencil)
			glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, frameBufferWidth, frameBufferHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
		else // Using both a stencil and depth test, needs special format arguments
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, frameBufferWidth, frameBufferHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}

private:
	void cursorPositionHandlerRegister();
	void mouseScrollHandlerRegister();

	Sen_Cube SenFreeSpaceLogoCube;

};


#endif // __SenFreeSpaceAbstract__