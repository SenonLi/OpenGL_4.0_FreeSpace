#pragma once

#ifndef __Sen_431_LineTextRendering__
#define __Sen_431_LineTextRendering__

#include "Support/SenAbstractGLFW.h"
#include <map>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

/// Holds all state information relevant to a character as loaded using FreeType
struct FreeTypeCharacterStruct {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 SizeInPixels;    // SizeInPixels of glyph
	glm::ivec2 BearingInPixels;  // Offset from baseline to left/top of glyph
	GLuint AdvanceInPixels;    // Horizontal offset to advance to next glyph
};


class Sen_431_LineTextRendering : public SenAbstractGLFW
{
public:
	Sen_431_LineTextRendering();
	virtual ~Sen_431_LineTextRendering();

protected:
	void paintGL(void);
	void initGlfwGlewGL();
	void finalize();


private:

	void initFreeTypeLibrary();
	void initTrueTypeFontsTextures();
	void initTextStringVertexAttributes();

	void paintLineTextString(std::string lineTextString, GLfloat textScreenCoord_X, GLfloat textScreenCoord_Y, GLfloat scale, GLuint screenWidth, GLuint screenHeight, glm::vec3 lineTextStringColor);

	GLuint lineTextProgram, lineTextVAO, lineTextVBO;
	FT_Library freeTypeLibrary;
	FT_Face faceTimesNewRoman;
	std::map<GLchar, FreeTypeCharacterStruct> characterStructs_128_Map;

	GLint primaryWidgetWidth, primaryWidgetHeight;

};


#endif  // __Sen_431_LineTextRendering_