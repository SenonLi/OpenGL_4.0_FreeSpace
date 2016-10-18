#pragma once
#ifndef __SenFrameBufferFreeSpace__
#define __SenFrameBufferFreeSpace__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"
#include "Support/SenFreeTypeFontsRendering.h"

class SenDebugWindowFreeSpace :	public SenFreeSpaceAbstract
{
public:
	SenDebugWindowFreeSpace();
	virtual ~SenDebugWindowFreeSpace();

protected:
	virtual void paintScene()	{ ; }
	virtual void cleanDebugWindowFreeSpace() { ; }

	void initGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);
	
	void keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode);
	GLboolean debugWindowSwitchPressing, debugWindowSwitch;

	// ******************* Debug Window Vertex Attributes + FrameBuffer ***************************
	virtual void initDebugWindowLineTextProgram();
	virtual void paintDebugWindowFrameBufferGL();

	void initDebugWindowVertexAttributes();
	void initDebugWindowFrameBuffer();


	GLuint debugWindowProgram, debugWindowOutlineProgram;
	GLuint debugWindowVAO,		debugWindowOutlineVAO;
	GLuint debugWindowVBO,		debugWindowOutlineVBO;

	GLuint debugWindowFrameBufferObject;
	GLuint debugWindowRGB_TextureAttach, debugWindowDepthStencil_RBO;

	GLint debugWindowFrameBufferWidth, debugWindowFrameBufferHeight;

	std::string pressButtonString, triggerDebugWindowString, debugWindowFuntionString;
	glm::vec3 normalLineTextColor, highlightLineTextColor, lineTextOutlineColor;

	// ******************* TrueType Line Text Fonts Rendering (FreeType) ***************************
	void initFreeTypeLibrary();
	void initTrueTypeFontsTextures();
	void initTextStringVertexAttributes();

	GLfloat paintLineTextString(std::string lineTextString, GLfloat textScreenCoord_X, GLfloat textScreenCoord_Y, GLfloat scale, GLuint screenWidth, GLuint screenHeight, glm::vec3 lineTextStringColor);
	
	GLfloat nextStringScreenCoord_X, switchHintScreenCoord_X, switchHintScreenCoord_Y;
	GLfloat delta = 0.005f;
	FT_Library freeTypeLibrary;
	FT_Face faceTimesNewRoman;
	std::map<GLchar, FreeTypeCharacterStruct> characterStructs_128_Map;

	GLuint lineTextProgram, lineTextVAO, lineTextVBO;
};



#endif // __SenFrameBufferFreeSpace__


