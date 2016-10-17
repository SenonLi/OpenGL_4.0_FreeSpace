#pragma once

#ifndef __Sen_43_LineTextRendering__
#define __Sen_43_LineTextRendering__

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


class Sen_43_LineTextRendering : public SenAbstractGLFW
{
public:
	Sen_43_LineTextRendering();
	virtual ~Sen_43_LineTextRendering();

protected:
	void paintGL(void);
	void initGlfwGlewGL();
	void finalize();

	std::map<GLchar, FreeTypeCharacterStruct> characterStructs_128_Map;

};


#endif  // __Sen_43_LineTextRendering__