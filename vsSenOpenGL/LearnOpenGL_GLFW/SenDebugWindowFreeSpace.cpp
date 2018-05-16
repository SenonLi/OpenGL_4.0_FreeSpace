#include "SenDebugWindowFreeSpace.h"


SenDebugWindowFreeSpace::SenDebugWindowFreeSpace()
	: debugWindowSwitchPressing(false), debugWindowSwitch(true), nextStringScreenCoord_X(0.0f)
{
	strWindowName = "Sen DebugWindow FreeSpace";
	pressButtonString = "Press 'Q' to ";
	triggerDebugWindowString = "Close ";
	debugWindowFuntionString = "debug window";

	normalLineTextColor = glm::vec3(1.0f, 1.0f, 1.0f);
	highlightLineTextColor = glm::vec3(1.0f, 0.0f, 0.0f);

	//lineTextOutlineColor = glm::vec3(1.0f, 231.0 / 255.0, 109.0 / 255.0);
	lineTextOutlineColor = glm::vec3((float)0x87 / (float)0xFF, (float)0xCE / float(0xFF), (float)0xEB / float(0xFF));

	switchHintScreenCoord_X = -0.97f;
	switchHintScreenCoord_Y = 0.92f;
}

SenDebugWindowFreeSpace::~SenDebugWindowFreeSpace() { ; }


void SenDebugWindowFreeSpace::initGlfwGlewGL()	{

	SenFreeSpaceAbstract::initGlfwGlewGL();

	initFreeTypeLibrary();
	initTrueTypeFontsTextures();
	initTextStringVertexAttributes();

	initDebugWindowLineTextProgram();
	initDebugWindowVertexAttributes();
	initDebugWindowFrameBuffer();

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0x5A, 0xFF);    // Check GL_STENCIL_TEST only when needed;  All fragments should update the
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);  
	glStencilMask(0x00); // Disable writing to the stencil buffer

	OutputDebugString(" Sen FrameBuffer FreeSpace Initial \n\n");
}

void SenDebugWindowFreeSpace::paintDebugWindowFrameBufferGL()	{

	glBindFramebuffer(GL_FRAMEBUFFER, debugWindowFrameBufferObject);
	glViewport(0, 0, debugWindowFrameBufferWidth, debugWindowFrameBufferHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer so why bother with clearing?

	// Get Rear CameraView
	camera.Front = -camera.Front;

	paintScene();

	camera.Front = -camera.Front; // Recover front CameraView
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_WidgetWidth, m_WidgetHeight);
}

void SenDebugWindowFreeSpace::paintFreeSpaceGL(void)	{

	// ======== If DebugWindow Open, render DebugWindow ColorTexture into Customer FrameBuffer =================================================================

	if (debugWindowSwitch)	{
		paintDebugWindowFrameBufferGL();
	}

	// ======== Render Normal Screen =============================================================
	paintScene();
	// ======== End of Normal Rendering =============================================================


	// ======== If DebugWindow Open, render DebugWindow ColorTexture into Screen =================================================================
	if (debugWindowSwitch)	{

		// Bind to default framebuffer again and draw the quad plane with attched screen texture.
		glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad


		//*********** Start of Paint stencil of "one"s ****************************************/ 

		// Clear Stencil Buffer as a beginning 
		glStencilMask(0xFF); // Enable Stencil Writing (for clearing)
		
		// Comment Seleted because already cleared to 0x00 in FreeSpace
		//glClearStencil(0x00);
		//glClear(GL_STENCIL_BUFFER_BIT); // Clear Stencil Buffer

		glStencilFunc(GL_NOTEQUAL, 0x5A, 0xFF); // All fragments should update the
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

		// Paint DebugWindow
		glUseProgram(debugWindowProgram);
		glBindVertexArray(debugWindowVAO);
		glBindTexture(GL_TEXTURE_2D, debugWindowRGB_TextureAttach);	// Use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//*********** End of Paint stencil of "one"s ****************************************/ 


		/********** Start of Paint DebugWindow Outline based on Stencil Test ****************/
		glStencilMask(0xFF); // Enable Stencil Writing (for clearing)
		glStencilFunc(GL_EQUAL, 0x00, 0xFF); // Paint where Stencil Not Equal To "One"s
		glStencilMask(0x00); // Disable writing to the stencil buffer
		
		glUseProgram(debugWindowOutlineProgram);
		glBindVertexArray(debugWindowOutlineVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// =====  "Disable stencil test" for other objects   ====
		glStencilFunc(GL_ALWAYS, 0x01, 0xFF); // All fragments should update the
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0x00); // Disable writing to the stencil buffer

		/********** End of Paint DebugWindow Outline ****************/
		
		//glEnable(GL_DEPTH_TEST);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(lineTextProgram);
	nextStringScreenCoord_X = paintLineTextString(pressButtonString, switchHintScreenCoord_X, switchHintScreenCoord_Y, m_WidgetWidth / debugWindowFrameBufferWidth, m_WidgetWidth, m_WidgetHeight, normalLineTextColor);
	triggerDebugWindowString = debugWindowSwitch ? "Close ": "Open ";
	nextStringScreenCoord_X = paintLineTextString(triggerDebugWindowString, nextStringScreenCoord_X, switchHintScreenCoord_Y, m_WidgetWidth / debugWindowFrameBufferWidth, m_WidgetWidth, m_WidgetHeight, highlightLineTextColor);
	paintLineTextString(debugWindowFuntionString, nextStringScreenCoord_X, switchHintScreenCoord_Y, m_WidgetWidth / debugWindowFrameBufferWidth, m_WidgetWidth, m_WidgetHeight, normalLineTextColor);

	paintLineTextString("Press WASD: Move", -0.95f, -0.75f, m_WidgetWidth / debugWindowFrameBufferWidth, m_WidgetWidth, m_WidgetHeight, normalLineTextColor);
	paintLineTextString("Mouse Move: Turn Around", -0.95f, -0.85f, m_WidgetWidth / debugWindowFrameBufferWidth, m_WidgetWidth, m_WidgetHeight, normalLineTextColor);
	paintLineTextString("Mouse Scroll: Zoom In/Out", -0.95f, -0.95f, m_WidgetWidth / debugWindowFrameBufferWidth, m_WidgetWidth, m_WidgetHeight, normalLineTextColor);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// =====  "Disable stencil test" for other objects   ====
	glStencilFunc(GL_ALWAYS, 0x01, 0xFF); // All fragments should update the
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x00); // Disable writing to the stencil buffer
}



void SenDebugWindowFreeSpace::initDebugWindowFrameBuffer()	{

	glGenFramebuffers(1, &debugWindowFrameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, debugWindowFrameBufferObject);
	
	// Color Texture
	debugWindowFrameBufferWidth = m_WidgetWidth;
	debugWindowFrameBufferHeight = m_WidgetHeight;

	debugWindowRGB_TextureAttach = generateAttachmentTexture(false, false, debugWindowFrameBufferWidth, debugWindowFrameBufferHeight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, debugWindowRGB_TextureAttach, 0);
	
	// Depth + Stencil RenderBuffer
	glGenRenderbuffers(1, &debugWindowDepthStencil_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, debugWindowDepthStencil_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, debugWindowFrameBufferWidth, debugWindowFrameBufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, debugWindowDepthStencil_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	// Check Complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Error::Framebuffer:: Framebuffer is not complete!" << std::endl;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);   // Recover Default Framebuffer (Main Screen)
}

void SenDebugWindowFreeSpace::initDebugWindowVertexAttributes()	{

	/******** DebugWindow Outline Quad *********/
	GLfloat debugWindowOutlineVertices[] = {
		-1.0f, 1.0f, 
		-1.0f, -1.0f + 2 * SenGOLDEN_SectionScale, 
		1.0f - 2 * SenGOLDEN_SectionScale, -1.0f + 2 * SenGOLDEN_SectionScale,

		-1.0f, 1.0f,
		1.0f - 2 * SenGOLDEN_SectionScale, -1.0f + 2 * SenGOLDEN_SectionScale, 
		1.0f - 2 * SenGOLDEN_SectionScale, 1.0f
	};

	/******** DebugWindow  Quad *********/
	GLfloat OutlineToWindowScale = SenGOLDEN_SectionScale * 1.003;// *GLfloat(m_WidgetWidth) / GLfloat(debugWindowFrameBufferWidth);
	GLfloat debugWindowVertices[] = {
		// Positions												// TexCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f + 2 * OutlineToWindowScale, 0.0f, 0.0f,
		1.0f - 2 * OutlineToWindowScale, -1.0f + 2 * OutlineToWindowScale, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f - 2 * OutlineToWindowScale, -1.0f + 2 * OutlineToWindowScale, 1.0f, 0.0f,
		1.0f - 2 * OutlineToWindowScale, 1.0f, 1.0f, 1.0f
	};

	// ****** Setup DebugWindow Outline Quad,  debugWindowOutlineVAO ****************************/
	glGenVertexArrays(1, &debugWindowOutlineVAO);
	glGenBuffers(1, &debugWindowOutlineVBO);

	glBindVertexArray(debugWindowOutlineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, debugWindowOutlineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(debugWindowOutlineVertices), debugWindowOutlineVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ****** Setup DebugWindow Quad,  debugWindowVAO ****************************/
	glGenVertexArrays(1, &debugWindowVAO);
	glGenBuffers(1, &debugWindowVBO);

	glBindVertexArray(debugWindowVAO);
	glBindBuffer(GL_ARRAY_BUFFER, debugWindowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(debugWindowVertices), debugWindowVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
}

void SenDebugWindowFreeSpace::cleanFreeSpace(void)	{

	cleanDebugWindowFreeSpace();

	// finalize LineText Rendering
	for (std::map<GLchar, FreeTypeCharacterStruct>::iterator ptrMap = characterStructs_128_Map.begin(); ptrMap != characterStructs_128_Map.end(); ptrMap++)
	{
		if (glIsTexture(ptrMap->second.TextureID))			glDeleteTextures(1, &ptrMap->second.TextureID);
	}

	if (glIsVertexArray(lineTextVAO))	glDeleteVertexArrays(1, &lineTextVAO);
	if (glIsBuffer(lineTextVBO))	glDeleteBuffers(1, &lineTextVBO);
	if (glIsProgram(lineTextProgram))				glDeleteProgram(lineTextProgram);

	// finalize Debug Window Vertex Attributes
	if (glIsProgram(debugWindowProgram))				glDeleteProgram(debugWindowProgram);
	if (glIsProgram(debugWindowOutlineProgram))			glDeleteProgram(debugWindowOutlineProgram);
	if (glIsVertexArray(debugWindowVAO))				glDeleteVertexArrays(1, &debugWindowVAO);
	if (glIsVertexArray(debugWindowVBO))				glDeleteVertexArrays(1, &debugWindowVBO);
	if (glIsBuffer(debugWindowVBO))						glDeleteBuffers(1, &debugWindowVBO);	
	if (glIsBuffer(debugWindowOutlineVBO))				glDeleteBuffers(1, &debugWindowOutlineVBO);
	
	// finalize FrameBuffer 
	if (glIsTexture(debugWindowRGB_TextureAttach))		glDeleteTextures(1, &debugWindowRGB_TextureAttach);
	if (glIsFramebuffer(debugWindowFrameBufferObject))	glDeleteFramebuffers(1, &debugWindowFrameBufferObject);
	if (glIsRenderbuffer(debugWindowDepthStencil_RBO))	glDeleteRenderbuffers(1, &debugWindowDepthStencil_RBO);
}


void SenDebugWindowFreeSpace::initDebugWindowLineTextProgram()	{

	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_22_DepthTest.vert" },
	//	{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_22_DepthTest.frag" },
	//	{ GL_NONE, NULL }
	//};

	ShaderInfo shadersScreenTexture[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_ScreenTextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TextureCoords.frag" },
		{ GL_NONE, NULL }
	};

	ShaderInfo shadersScreenSingular[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_ScreenCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_ColorRed.frag" },
		{ GL_NONE, NULL }
	};

	debugWindowProgram = LoadShaders(shadersScreenTexture);
	debugWindowOutlineProgram = LoadShaders(shadersScreenSingular);

	//********** Line Text Rendering Program *********************************************/

	ShaderInfo shadersScreenTrueTypeFonts[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_ScreenTextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TrueTypeFontsRender.frag" },
		{ GL_NONE, NULL }
	};

	lineTextProgram = LoadShaders(shadersScreenTrueTypeFonts);
}



void SenDebugWindowFreeSpace::keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode)	{

	SenFreeSpaceAbstract::keyDetection(widget, key, scancode, action, mode);

	//cout << key << endl;
	if (key == GLFW_KEY_Q)	{
		if (action == GLFW_PRESS)	debugWindowSwitchPressing = true;
		if (action == GLFW_RELEASE && debugWindowSwitchPressing)	{
			debugWindowSwitchPressing = false;
			debugWindowSwitch = !debugWindowSwitch;
		}
	}
}



void SenDebugWindowFreeSpace::initFreeTypeLibrary(){
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&freeTypeLibrary))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as faceTimesNewRoman
	if (FT_New_Face(freeTypeLibrary, "C:/Windows/Fonts/timesbd.ttf", 0, &faceTimesNewRoman))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(faceTimesNewRoman, 0, 48);
}
void SenDebugWindowFreeSpace::initTrueTypeFontsTextures(){
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

void SenDebugWindowFreeSpace::initTextStringVertexAttributes(){

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


GLfloat SenDebugWindowFreeSpace::paintLineTextString(std::string lineTextString, GLfloat textScreenCoord_X, GLfloat textScreenCoord_Y, GLfloat scale, GLuint screenWidth, GLuint screenHeight, glm::vec3 lineTextStringColor)
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







		////*************************************************************************************************************
		//glStencilMask(0x00); // Disable writing to the stencil buffer
		//glStencilFunc(GL_NOTEQUAL, 0x3A, 0xFF); // All fragments should update the
		////*************************************************************************************************************

		//GLfloat xxx = textScreenCoord_X - 0.005;
		//GLfloat yyy = textScreenCoord_Y - 0.005;
		//GLfloat sss = scale * 1.2f;

		//glyphScreenX = xxx + characterStruct.BearingInPixels.x * sss / screenWidth;
		//glyphScreenY = yyy - (characterStruct.SizeInPixels.y - characterStruct.BearingInPixels.y) * sss / screenHeight;
		//glyphWidth = characterStruct.SizeInPixels.x * sss / screenWidth;
		//glyphHeight = characterStruct.SizeInPixels.y * sss / screenHeight;

		//// Update VBO for each ptrCharacter
		//GLfloat glyphOutlineVertices[6][4] = {
		//	{ glyphScreenX, glyphScreenY + glyphHeight, 0.0, 0.0 },
		//	{ glyphScreenX, glyphScreenY, 0.0, 1.0 },
		//	{ glyphScreenX + glyphWidth, glyphScreenY, 1.0, 1.0 },

		//	{ glyphScreenX, glyphScreenY + glyphHeight, 0.0, 0.0 },
		//	{ glyphScreenX + glyphWidth, glyphScreenY, 1.0, 1.0 },
		//	{ glyphScreenX + glyphWidth, glyphScreenY + glyphHeight, 1.0, 0.0 }
		//};

		//// Update content of VBO memory
		//glBindBuffer(GL_ARRAY_BUFFER, lineTextVBO);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glyphOutlineVertices), glyphOutlineVertices); // Be sure to use glBufferSubData and not glBufferData
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		//glUniform3f(glGetUniformLocation(lineTextProgram, "lineTextStringColor"), lineTextOutlineColor.x, lineTextOutlineColor.y, lineTextOutlineColor.z);

		//// Render quad
		//glDrawArrays(GL_TRIANGLES, 0, 6);




































		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		textScreenCoord_X += (characterStruct.AdvanceInPixels >> 6) * scale / screenWidth; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textScreenCoord_X;
}