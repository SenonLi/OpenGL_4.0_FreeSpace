#pragma once

#ifndef __SenFreeTypeFontsRendering__
#define __SenFreeTypeFontsRendering__

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


class SenFreeTypeFontsRendering {
public:


protected:


};


#endif // __SenFreeTypeFontsRendering__