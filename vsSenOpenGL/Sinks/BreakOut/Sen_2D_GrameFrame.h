#pragma once

#ifndef __Sen_2D_GrameFrame__
#define __Sen_2D_GrameFrame__

#include "Support/SenAbstractGLFW.h"
#include "SenFreeSpacePieces/Sen_Cube.h"

class Sen_2D_GrameFrame :	public SenAbstractGLFW
{
public:
	Sen_2D_GrameFrame();
	virtual ~Sen_2D_GrameFrame();

protected:
	virtual void clean_2D_GrameFrame(){ ; }
	virtual void daltaTimeUpdate(GLfloat deltaTime)	{ ; }
	virtual void init2DGamePaceGL() { ; }
	virtual void paint2DGameSpaceGL()	{ ; }

	void paintGL(void);
	void initGlfwGlewGL();
	void finalize();

	//GLfloat lastX = 400, lastY = 300;
	//bool firstMouse = true;

	glm::mat4 model;

	bool keys[1024];
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	
	void keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode);

	unsigned char* textureLoadImagePtr;
	void uploadTextureImage(const char* textureAddressPointer, GLuint &textureID, std::string channelType)	{
		int width, height;
		bool alpha;

		if (channelType == std::string("RGBA"))			alpha = true;
		else if (channelType == std::string("RGB"))		alpha = false;

		textureLoadImagePtr = SOIL_load_image(textureAddressPointer, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

		// Load and create a defaultTextureID 
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTextureID object
		// Set the defaultTextureID wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Set defaultTextureID wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureLoadImagePtr);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(textureLoadImagePtr);
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
	Sen_Cube Sen2DGameLogoCube;

	const GLfloat DEFAULT_zoomRatio = 6.0;
	const GLfloat SenGOLDEN_SectionScale = 0.618f;
	const GLint Sen2DGameSpace_widgetWidth = 960;
	const GLint Sen2DGameSpace_widgetHeight = GLint(Sen2DGameSpace_widgetWidth * SenGOLDEN_SectionScale);
};


#endif  // __Sen_2D_GrameFrame__