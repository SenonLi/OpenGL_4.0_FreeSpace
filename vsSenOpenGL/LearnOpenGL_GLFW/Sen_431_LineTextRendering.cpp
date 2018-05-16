#include "Sen_431_LineTextRendering.h"


Sen_431_LineTextRendering::Sen_431_LineTextRendering()
{
	strWindowName = "Sen_43 Line Text Rendering";
}


Sen_431_LineTextRendering::~Sen_431_LineTextRendering()
{
}


void Sen_431_LineTextRendering::initGlfwGlewGL()
{
	SenAbstractGLFW::initGlfwGlewGL();

	ShaderInfo shadersScreenTrueTypeFonts[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_ScreenTextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TrueTypeFontsRender.frag" },
		{ GL_NONE, NULL }
	};

	lineTextProgram = LoadShaders(shadersScreenTrueTypeFonts);

	initFreeTypeLibrary();
	initTrueTypeFontsTextures();
	initTextStringVertexAttributes();

	primaryWidgetWidth = m_WidgetWidth;
	primaryWidgetHeight = m_WidgetHeight;

	OutputDebugString(" Initial LineText Rendering\n\n");
}

void Sen_431_LineTextRendering::initFreeTypeLibrary(){
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&freeTypeLibrary))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as faceTimesNewRoman
	if (FT_New_Face(freeTypeLibrary, "C:/Windows/Fonts/times.ttf", 0, &faceTimesNewRoman))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(faceTimesNewRoman, 0, 48);
}
void Sen_431_LineTextRendering::initTrueTypeFontsTextures(){
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte character = 0; character < 128; character++)
	{
		// Generate characterTextureID
		GLuint characterTextureID;
		glGenTextures(1, &characterTextureID);
		glBindTexture(GL_TEXTURE_2D, characterTextureID);

		// Load character glyph 
		if (FT_Load_Char(faceTimesNewRoman, character, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Set characterTextureID options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			faceTimesNewRoman->glyph->bitmap.width,
			faceTimesNewRoman->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			faceTimesNewRoman->glyph->bitmap.buffer
			);

		glGenerateMipmap(GL_TEXTURE_2D);

		// Now store characterStruct for later use
		FreeTypeCharacterStruct characterStruct = {
			characterTextureID,
			glm::ivec2(faceTimesNewRoman->glyph->bitmap.width, faceTimesNewRoman->glyph->bitmap.rows),
			glm::ivec2(faceTimesNewRoman->glyph->bitmap_left, faceTimesNewRoman->glyph->bitmap_top),
			faceTimesNewRoman->glyph->advance.x
		};
		characterStructs_128_Map.insert(std::pair<GLchar, FreeTypeCharacterStruct>(character, characterStruct));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(faceTimesNewRoman);
	FT_Done_FreeType(freeTypeLibrary);

	// Recover byte-alignment restriction
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
}

void Sen_431_LineTextRendering::initTextStringVertexAttributes(){

	glGenVertexArrays(1, &lineTextVAO);
	glGenBuffers(1, &lineTextVBO);

	glBindVertexArray(lineTextVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineTextVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sen_431_LineTextRendering::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Paint Line Text Strings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	paintLineTextString("This is sample text", -0.935f, -0.916f, 2.0f * m_WidgetWidth / primaryWidgetWidth, m_WidgetWidth, m_WidgetHeight, glm::vec3(0.5, 0.8f, 0.2f));
	paintLineTextString("(C) LearnOpenGL.com", 0.25f, 0.9f, 1.0f * m_WidgetWidth / primaryWidgetWidth, m_WidgetWidth, m_WidgetHeight, glm::vec3(0.3, 0.7f, 0.9f));

	glDisable(GL_BLEND);
}

void Sen_431_LineTextRendering::finalize(void)
{
	for (std::map<GLchar, FreeTypeCharacterStruct>::iterator ptrMap = characterStructs_128_Map.begin(); ptrMap != characterStructs_128_Map.end(); ptrMap++)
	{
		if (glIsTexture(ptrMap->second.TextureID))			glDeleteTextures(1, &ptrMap->second.TextureID);
	}

	if (glIsVertexArray(lineTextVAO))	glDeleteVertexArrays(1, &lineTextVAO);
	if (glIsBuffer(lineTextVBO))	glDeleteBuffers(1, &lineTextVBO);

	if (glIsProgram(lineTextProgram))				glDeleteProgram(lineTextProgram);
}

void Sen_431_LineTextRendering::paintLineTextString(std::string lineTextString, GLfloat textScreenCoord_X, GLfloat textScreenCoord_Y, GLfloat scale, GLuint screenWidth, GLuint screenHeight, glm::vec3 lineTextStringColor)
{
	// Activate corresponding render state	
	glUseProgram(lineTextProgram);
	glUniform3f(glGetUniformLocation(lineTextProgram, "lineTextStringColor"), lineTextStringColor.x, lineTextStringColor.y, lineTextStringColor.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(lineTextVAO);

	GLfloat glyphScreenX, glyphScreenY, glyphWidth, glyphHeight;

	// Iterate through all characters
	std::string::const_iterator ptrCharacter; // is actually a pointer to the string
	for (ptrCharacter = lineTextString.begin(); ptrCharacter != lineTextString.end(); ptrCharacter++)
	{
		FreeTypeCharacterStruct characterStruct = characterStructs_128_Map[*ptrCharacter];

		glyphScreenX = textScreenCoord_X + characterStruct.BearingInPixels.x * scale / screenWidth;
		glyphScreenY = textScreenCoord_Y - (characterStruct.SizeInPixels.y - characterStruct.BearingInPixels.y) * scale / screenHeight;
		glyphWidth = characterStruct.SizeInPixels.x * scale / screenWidth;
		glyphHeight = characterStruct.SizeInPixels.y * scale / screenHeight;

		// Update VBO for each ptrCharacter
		GLfloat glyphVertices[6][4] = {
			{ glyphScreenX, glyphScreenY + glyphHeight, 0.0, 0.0 },
			{ glyphScreenX, glyphScreenY, 0.0, 1.0 },
			{ glyphScreenX + glyphWidth, glyphScreenY, 1.0, 1.0 },

			{ glyphScreenX, glyphScreenY + glyphHeight, 0.0, 0.0 },
			{ glyphScreenX + glyphWidth, glyphScreenY, 1.0, 1.0 },
			{ glyphScreenX + glyphWidth, glyphScreenY + glyphHeight, 1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, characterStruct.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, lineTextVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glyphVertices), glyphVertices); // Be sure to use glBufferSubData and not glBufferData
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		textScreenCoord_X += (characterStruct.AdvanceInPixels >> 6) * scale / screenWidth; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}