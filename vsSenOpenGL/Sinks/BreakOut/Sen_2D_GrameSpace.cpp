#include "Sen_2D_GrameSpace.h"


Sen_2D_GrameSpace::Sen_2D_GrameSpace()
	:textureLoadImagePtr(NULL)
{
	strWindowName = "Sen GLFW 2D Game Space";
	widgetWidth = Sen2DGameSpace_widgetWidth;
	widgetHeight = Sen2DGameSpace_widgetHeight;

	normalLineTextColor = glm::vec3(1.0f, 0.1f, 0.1f);
	highlightLineTextColor = glm::vec3(0.5f, 0.0f, 0.8f); 
	lineTextOutlineColor = glm::vec3(0.5f, 0.5f, 1.0f);
}


Sen_2D_GrameSpace::~Sen_2D_GrameSpace()
{
}

void Sen_2D_GrameSpace::initLineTextFontsFunctions()
{
	ShaderInfo shadersScreenTrueTypeFonts[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_ScreenTextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TrueTypeFontsRender.frag" },
		{ GL_NONE, NULL }
	};

	lineTextProgram = LoadShaders(shadersScreenTrueTypeFonts);
	initFreeTypeLibrary();
	initTrueTypeFontsTextures();
	initTextStringVertexAttributes();
}


void Sen_2D_GrameSpace::initGlfwGlewGL()
{
	SenAbstractGLFW::initGlfwGlewGL();

	initLineTextFontsFunctions();
	//********** Line Text Rendering Program *********************************************/

	Sen2DGameLogoCube.initialCubeGL(GL_TRUE);
	cubeLogoOccupancySquareSide = 2.0 * Sen2DGameLogoCube.getCubeLogoCenterToBorder();

	// Add Stencil Test for Perfect Cube Logo
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0x01, 0xFF);    // Check GL_STENCIL_TEST only when needed;  All fragments should update the
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x00); // Disable writing to the stencil buffer

	OutputDebugString(" Sen_2D_GrameSpace Initial \n\n");

	init2DGamePaceGL();
}

void Sen_2D_GrameSpace::paintGL(void)
{
	SenAbstractGLFW::paintGL();

	// Set CameraView and Projection
	GLfloat currentFrame = GLfloat(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	daltaTimeUpdate(deltaTime);

	//*********** Start of Paint stencil of "one"s ****************************************/ 

	// Clear Stencil Buffer as a beginning 
	glStencilMask(0xFF); // Enable Stencil Writing (for clearing)
	glClearStencil(0x00);
	glClear(GL_STENCIL_BUFFER_BIT); // Clear Stencil Buffer

	glStencilFunc(GL_ALWAYS, 0x5A, 0xFF); // All fragments should update the
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_DEPTH_TEST);
	// Paint Perfect Sen FreeSpace Logo
	Sen2DGameLogoCube.paintSenLogoCube();

	//*********** End of Paint stencil of "one"s ****************************************/ 


	/********** Start of Paint FreeSpace based on Stencil Test ****************/
	glStencilFunc(GL_NOTEQUAL, 0x5A, 0xFF); // Paint where Stencil Not Equal To "One"s
	glStencilMask(0x00); // Disable writing to the stencil buffer

	// Paint from SubClass
	paint2DGameSpaceGL();

	// =====  "Disable stencil test" for other objects   ====
	glStencilFunc(GL_ALWAYS, 0x01, 0xFF); // All fragments should update the
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x00); // Disable writing to the stencil buffer

	///********** End of Paint DebugWindow Outline ****************/
}


void Sen_2D_GrameSpace::finalize(void)
{
	clean_2D_GrameFrame();

	// Clean SenFreeSpaceLogo
	Sen2DGameLogoCube.finalizeCube();
}

void Sen_2D_GrameSpace::keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(widget, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}






// *********************************************************************************************
// ******************* TrueType Line Text Fonts Rendering (FreeType) ***************************

void Sen_2D_GrameSpace::initFreeTypeLibrary(){
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&freeTypeLibrary))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as faceTimesNewRoman
	if (FT_New_Face(freeTypeLibrary, "C:/Windows/Fonts/timesbd.ttf", 0, &faceTimesNewRoman))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(faceTimesNewRoman, 0, 48);
}
void Sen_2D_GrameSpace::initTrueTypeFontsTextures(){
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

void Sen_2D_GrameSpace::initTextStringVertexAttributes(){

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


GLfloat Sen_2D_GrameSpace::paintLineTextString(std::string lineTextString, GLfloat textScreenCoord_X, GLfloat textScreenCoord_Y, GLfloat scale, GLuint screenWidth, GLuint screenHeight, glm::vec3 lineTextStringColor)
{
	// Activate corresponding render state	
	glBindVertexArray(lineTextVAO);

	GLfloat glyphScreenX, glyphScreenY, glyphWidth, glyphHeight;

	// Iterate through all characters
	std::string::const_iterator ptrCharacter; // is actually a pointer to the string
	for (ptrCharacter = lineTextString.begin(); ptrCharacter != lineTextString.end(); ptrCharacter++)
	{
		// Render glyph texture over quad
		glUniform3f(glGetUniformLocation(lineTextProgram, "lineTextStringColor"), lineTextStringColor.x, lineTextStringColor.y, lineTextStringColor.z);
		FreeTypeCharacterStruct characterStruct = characterStructs_128_Map[*ptrCharacter];
		glBindTexture(GL_TEXTURE_2D, characterStruct.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, lineTextVBO);


		//*************************************************************************************************************
		glStencilMask(0xFF); // Enable Stencil Writing (for clearing)
		glStencilFunc(GL_ALWAYS, 0x3A, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//*************************************************************************************************************

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

		// Update content of VBO memory
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glyphVertices), glyphVertices); // Be sure to use glBufferSubData and not glBufferData
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		textScreenCoord_X += (characterStruct.AdvanceInPixels >> 6) * scale / screenWidth; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textScreenCoord_X;
}


