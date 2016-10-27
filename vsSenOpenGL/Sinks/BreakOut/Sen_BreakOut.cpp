#include "Sen_BreakOut.h"


Sen_BreakOut::Sen_BreakOut()
	:playerBoardWidth(0.0f)
{
	strWindowName = "Sen BreakOut in 2D GameSpace";
}


Sen_BreakOut::~Sen_BreakOut()
{
}



void Sen_BreakOut::init2DGamePaceGL()	{

	initShaderPrograms();
	initVertexAttributes();
	initTextures();
}
void Sen_BreakOut::initShaderPrograms(){

	ShaderInfo shadersScreenTextureCoords[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_ScreenTextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TextureCoords.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shadersScreenTextureCoords);

	ShaderInfo shaders2DTextureCoords[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_2D_TextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TextureCoords.frag" },
		{ GL_NONE, NULL }
	};
	programB = LoadShaders(shaders2DTextureCoords);
}

void Sen_BreakOut::initTextures(){
	uploadTextureImage(std::string("./../WatchMe/Images/SunRaise.jpg").c_str(), backgroundTexture, "RGBA", GL_TRUE);
	uploadTextureImage(std::string("./../WatchMe/Images/paddle.png").c_str(), playerBoardTexture, "RGBA", GL_TRUE);
	
	uploadTextureImage(std::string("./../WatchMe/Images/block.png").c_str(), blockTexture, "RGB", GL_TRUE);
	uploadTextureImage(std::string("./../WatchMe/Images/paddle.png").c_str(), solidBlockTexture, "RGB", GL_TRUE);
}

void Sen_BreakOut::initBackgroundVertices(){

	GLfloat backgroundVertexAttributes[] = {
		// Positions            // Texture Coords 
		1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &backgroundVAO);
	glGenBuffers(1, &backgroundVBO);

	glBindVertexArray(backgroundVAO);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertexAttributes), backgroundVertexAttributes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Sen_BreakOut::initPlayerVertices(){

	playerBoardWidth = 1 / 4.0;
	GLfloat width = playerBoardWidth / 2.0, height = 1 / 30.0;
	GLfloat playerBoardVertexAttributes[] = {
		// Positions            // Texture Coords 
		width, height, 1.0f, 0.0f,
		-width, height, 0.0f, 0.0f,
		-width, -height, 0.0f, 1.0f,

		width, height, 1.0f, 0.0f,
		-width, -height, 0.0f, 1.0f,
		width, -height, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &playerBoardVAO);
	glGenBuffers(1, &playerBoardVBO);

	glBindVertexArray(playerBoardVAO);
	glBindBuffer(GL_ARRAY_BUFFER, playerBoardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(playerBoardVertexAttributes), playerBoardVertexAttributes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 

	playerBoardPosition = glm::vec2(0.0f, -29.0 / 30.0);
}

void Sen_BreakOut::initVertexAttributes(){

	initBackgroundVertices();
	initPlayerVertices();
	//GLfloat halfWidth = 3 / 8.0, halfHeight = 2 / 3.0;
	//GLfloat spriteVertices[] = {
	//	// Pos      // Tex
	//	-halfWidth, -halfHeight, 0.0f, 1.0f,
	//	halfWidth, halfHeight, 1.0f, 0.0f,
	//	-halfWidth, halfHeight, 0.0f, 0.0f,

	//	-halfWidth, -halfHeight, 0.0f, 1.0f,
	//	halfWidth, -halfHeight, 1.0f, 1.0f,
	//	halfWidth, halfHeight, 1.0f, 0.0f
	//};

	//glGenVertexArrays(1, &spriteVAO);
	//glGenBuffers(1, &spriteVBO);

	//glBindVertexArray(spriteVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertices), spriteVertices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

void Sen_BreakOut::paint2DGameSpaceGL()	{
	glDisable(GL_DEPTH_TEST);

	paintBackground();
	
	// Alpha blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	paintPlayer();

	glDisable(GL_BLEND);
}

void Sen_BreakOut::paintPlayer()	{

	glUseProgram(programB);
	glBindVertexArray(playerBoardVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, playerBoardTexture);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(playerBoardPosition, 0.0f)); 
	glUniformMatrix4fv(glGetUniformLocation(programB, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Sen_BreakOut::paintBackground()	{

	//model = glm::mat4();
	//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	//glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//glUniform3f(glGetUniformLocation(programA, "renderColor"), spriteRenderColor.x, spriteRenderColor.y, spriteRenderColor.z);

	glUseProgram(programA);
	glBindVertexArray(backgroundVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Sen_BreakOut::clean_2D_GrameFrame()	{
	if (glIsVertexArray(backgroundVAO))		glDeleteVertexArrays(1, &backgroundVAO);
	if (glIsBuffer(backgroundVBO))			glDeleteBuffers(1, &backgroundVBO);
	if (glIsTexture(backgroundTexture))		glDeleteTextures(1, &backgroundTexture);

	if (glIsVertexArray(playerBoardVAO))		glDeleteVertexArrays(1, &playerBoardVAO);
	if (glIsBuffer(playerBoardVBO))			glDeleteBuffers(1, &playerBoardVBO);
	if (glIsTexture(playerBoardTexture))		glDeleteTextures(1, &playerBoardTexture);

	if (glIsTexture(blockTexture))		glDeleteTextures(1, &blockTexture);
	if (glIsTexture(solidBlockTexture))		glDeleteTextures(1, &solidBlockTexture);

	if (glIsProgram(programA))	glDeleteProgram(programA);
	if (glIsProgram(programB))	glDeleteProgram(programB);
}


void Sen_BreakOut::daltaTimeUpdate(GLfloat deltaTime)	{
	//if (this->State == GAME_ACTIVE)
	//{

		GLfloat shift = PLAYER_VELOCITY * deltaTime;
		// Move playerBoard
		if (keys[GLFW_KEY_A])
		{
			playerBoardPosition.x -= shift;
			GLfloat xLeftBorder = -1.0f + playerBoardWidth / 2.0;
			if (playerBoardPosition.x < xLeftBorder)
				playerBoardPosition.x = xLeftBorder;
		}
		if (keys[GLFW_KEY_D])
		{
			playerBoardPosition.x += shift;
			GLfloat xRightBorder = 1.0f - playerBoardWidth / 2.0;
			if (playerBoardPosition.x > xRightBorder)
				playerBoardPosition.x = xRightBorder;
		}


	//}
}