#pragma once

#ifndef __Sen_2D_GrameSpace__
#define __Sen_2D_GrameSpace__

#include "Support/SenAbstractGLFW.h"
#include "SenFreeSpacePieces/Sen_Cube.h"
#include "Support/SenFreeTypeFontsRendering.h"

class Sen_2D_GrameSpace :	public SenAbstractGLFW
{
public:
	Sen_2D_GrameSpace();
	virtual ~Sen_2D_GrameSpace();

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
	glm::mat4 model{ 1.0f };

	bool keys[1024];
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	
	inline GLfloat getCubeLogoOccupancySquareSide() const { return cubeLogoOccupancySquareSide; }
	void keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode);

	unsigned char* textureLoadImagePtr;
	void uploadTextureImage(const char* textureAddressPointer, GLuint &textureID, const std::string &channelType, GLboolean highDefinition = GL_FALSE)	{
		int width, height;
		GLboolean alpha;
		if (channelType == std::string("RGBA"))			alpha = GL_TRUE;
		else if (channelType == std::string("RGB"))		alpha = GL_FALSE;

		textureLoadImagePtr = SOIL_load_image(textureAddressPointer, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
		// Load and create a defaultTextureID 
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this defaultTextureID object
		
		// Set the defaultTextureID wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Set defaultTextureID wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, highDefinition ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureLoadImagePtr);
		if (!highDefinition)	glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(textureLoadImagePtr);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind defaultTextureID when done, so we won't accidentily mess up our defaultTextureID.
	}

	// Generates a texture that is suited for attachments to a framebuffer
	// Assume the m_WidgetWidth, m_WidgetHeight won't change
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


	// *********************************************************************************************
	// ******************* TrueType Line Text Fonts Rendering (FreeType) ***************************
	void initLineTextFontsFunctions();
	void initFreeTypeLibrary();
	void initTrueTypeFontsTextures();
	void initTextStringVertexAttributes();

	GLfloat paintLineTextString(std::string lineTextString, GLfloat textScreenCoord_X, GLfloat textScreenCoord_Y, GLfloat scale, GLuint screenWidth, GLuint screenHeight, glm::vec3 lineTextStringColor);

	FT_Library freeTypeLibrary;
	FT_Face faceTimesNewRoman;
	std::map<GLchar, FreeTypeCharacterStruct> characterStructs_128_Map;

	GLuint lineTextProgram, lineTextVAO, lineTextVBO;
	glm::vec3 normalLineTextColor, highlightLineTextColor, lineTextOutlineColor;

	//GLfloat nextStringScreenCoord_X;


	const GLfloat DEFAULT_zoomRatio = 6.0;
	const GLfloat SenGOLDEN_SectionScale = 0.618f;
	const GLint Sen2DGameSpace_widgetWidth = 960;
	const GLint Sen2DGameSpace_widgetHeight = GLint(Sen2DGameSpace_widgetWidth * SenGOLDEN_SectionScale);

private:
	Sen_Cube Sen2DGameLogoCube;
	GLfloat cubeLogoOccupancySquareSide;

};


#endif  // __Sen_2D_GrameSpace__