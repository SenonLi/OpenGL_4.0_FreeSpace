#include "Sen_BreakOut.h"


Sen_BreakOut::Sen_BreakOut()
	: breakOutLevel(1), gameActive(GL_TRUE), currLevelFinished(GL_FALSE)
	, ballVelocity(glm::vec2(0.0f, 0.0f)), isBallStuckOnPlayerBoard(GL_TRUE)
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
	initBrickMapsVector();
	initBallVariables();

	//OutputDebugString(" After BrickMapsVector Initial \n\n");
}

void Sen_BreakOut::initBallVariables()	{

	isBallStuckOnPlayerBoard = GL_TRUE;
	currLevelFinished = GL_FALSE;

	ballVelocity = originalBallVELOCITY;
	ballSpinAngleDegree = 0.0f;
	ballSpinSpeed = originalBallSpinSpeed;

	playerBoardPosition = originalPlayerBoardPOSITION;
	ballPosition = originalBallPOSITION;
}

void Sen_BreakOut::initBrickMapsVector()	{

	mapUnsolidBrickNumVector.clear();

	GLfloat spareSide = getCubeLogoOccupancySquareSide();
	std::vector<std::vector<GLuint>> map2DBrickTypesinfo;
	
	init2DMapInfo(map2DBrickTypesinfo, 1);
	Sen_BreakOutMap levelOneMap(spareSide, map2DBrickTypesinfo);
	brickMapsVector.push_back(levelOneMap);
	mapUnsolidBrickNumVector.push_back(levelOneMap.getUnsolidBricksNum());

	init2DMapInfo(map2DBrickTypesinfo, 2);
	Sen_BreakOutMap levelTwoMap(spareSide, map2DBrickTypesinfo);
	brickMapsVector.push_back(levelTwoMap);
	mapUnsolidBrickNumVector.push_back(levelTwoMap.getUnsolidBricksNum());

	init2DMapInfo(map2DBrickTypesinfo, 3);
	Sen_BreakOutMap levelThirdMap(spareSide, map2DBrickTypesinfo);
	brickMapsVector.push_back(levelThirdMap);
	mapUnsolidBrickNumVector.push_back(levelThirdMap.getUnsolidBricksNum());


	currLevelUnsolidBrickNum = mapUnsolidBrickNumVector.at(breakOutLevel);

	//OutputDebugString(" After brickMapsVector.push_back Initial \n\n");
}

Sen_BreakOutMap::Sen_BreakOutMap(GLfloat spareSide, std::vector<std::vector<GLuint>> &map2DBrickTypesinfo) 
	:unsolidBricksNum(0)
{
	bricksVector.clear();

	GLuint totalLinesNum = map2DBrickTypesinfo.size();
	if (totalLinesNum)	{
		GLuint lineBricksNum = map2DBrickTypesinfo.at(0).size();

		GLfloat brickWidth = 2.0 / static_cast<GLfloat>(lineBricksNum);
		GLfloat brickHeight = 1.0 / static_cast<GLfloat>(totalLinesNum);

		GLuint incompleteLinesNum = std::ceil(spareSide / brickHeight);
		GLuint incompleteLineBricksNum = std::floor((2.0 - spareSide) / brickWidth);

		glm::vec2 brickSize(brickWidth, brickHeight);
		glm::vec2 brickPosition = glm::vec2(0.0f);
		glm::vec3 brickColor = glm::vec3(1.0f); // Initial: white, none for blending

		for (GLuint i = 0; i < totalLinesNum; i++)	{
			// Initial incomplete Lines of Bricks
			GLuint lineBricksNumMax = i < incompleteLinesNum ? incompleteLineBricksNum : lineBricksNum;

			for (GLuint j = 0; j < lineBricksNumMax; j++)	{

				if (map2DBrickTypesinfo.at(i).at(j) > 0)	{
					GLboolean isBrickSolid = map2DBrickTypesinfo.at(i).at(j) == 1 ? GL_TRUE : GL_FALSE;
					if (!isBrickSolid) unsolidBricksNum++;

					brickPosition.x = -1.0 + (static_cast<GLfloat>(j)+0.5) * brickWidth;
					brickPosition.y = 1.0 - (static_cast<GLfloat>(i)+0.5) * brickHeight;

					GLuint life = 0;
					if (map2DBrickTypesinfo.at(i).at(j) == 1)			{
						brickColor = glm::vec3(1.0f);
						life = 99;
					}
					else if (map2DBrickTypesinfo.at(i).at(j) == 2)		{
						brickColor = glm::vec3(0.8f, 0.8f, 0.4f); 
						life = 1;
					}
					else if (map2DBrickTypesinfo.at(i).at(j) == 3)		{
						brickColor = glm::vec3(1.0f, 0.5f, 0.0f); 
						life = 2;
					}
					else if (map2DBrickTypesinfo.at(i).at(j) == 4)		{
						brickColor = glm::vec3(0.0f, 0.7f, 0.0f); 
						life = 3;
					}
					else if (map2DBrickTypesinfo.at(i).at(j) == 5)		{
						brickColor = glm::vec3(0.9f, 0.6f, 0.9f);
						life = 4;
					}

					bricksVector.push_back(Sen_2D_BlockBrick(brickPosition, brickSize, brickColor, life, isBrickSolid));
				}
			}

		}
	}	else std::cout << "totalLinesNum == 0 !!";
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
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_PlayerColorRendering.frag" },
		{ GL_NONE, NULL }
	};
	programB = LoadShaders(shaders2DTextureCoords);

	ShaderInfo shaders2DTextureCoordsRender[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_2D_TextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TextureCoordsRender.frag" },
		{ GL_NONE, NULL }
	};
	blendUnitOneSquareProgram = LoadShaders(shaders2DTextureCoordsRender);

	ShaderInfo shaders2DTextureCoordsDiscardWhite[] = {
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_2D_TextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_DiscardPureWhitePixels.frag" },
		{ GL_NONE, NULL }
	};
	ballProgram = LoadShaders(shaders2DTextureCoordsDiscardWhite);
}

void Sen_BreakOut::initTextures(){
	uploadTextureImage(std::string("./../WatchMe/Images/SunRaise.jpg").c_str(), backgroundTexture, "RGBA", GL_TRUE);
	uploadTextureImage(std::string("./../WatchMe/Images/paddle.png").c_str(), playerBoardTexture, "RGBA", GL_TRUE);

	uploadTextureImage(std::string("./../WatchMe/Images/block.png").c_str(), blockTexture, "RGB", GL_TRUE);
	uploadTextureImage(std::string("./../WatchMe/Images/block_solid.png").c_str(), solidBlockTexture, "RGB", GL_TRUE);

	uploadTextureImage(std::string("./../WatchMe/Images/ball.jpg").c_str(), ballTexture, "RGBA", GL_TRUE);
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

	GLfloat width = originalPlayerBOARDWIDTH / 2.0, height = originalPlayerBOARDHEIGHT / 2.0;
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

	playerBoardWidth = originalPlayerBOARDWIDTH;
	playerBoardPosition = originalPlayerBoardPOSITION;
}

void Sen_BreakOut::initVertexAttributes(){

	initBackgroundVertices();
	initPlayerVertices();
	initUnitOneSquareVertices();
}

void Sen_BreakOut::initUnitOneSquareVertices(){
	GLfloat unitOneSquareVertexAttributes[] = {
		// Positions            // Texture Coords 
		0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 1.0f,

		0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &unitOneSquareVAO);
	glGenBuffers(1, &unitOneSquareVBO);

	glBindVertexArray(unitOneSquareVAO);
	glBindBuffer(GL_ARRAY_BUFFER, unitOneSquareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unitOneSquareVertexAttributes), unitOneSquareVertexAttributes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sen_BreakOut::paint2DGameSpaceGL()	{
	glDisable(GL_DEPTH_TEST);

	paintBackground();

	// Alpha blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	paintBricksMap();
	paintPlayer();
	paintBall();

	glDisable(GL_BLEND);
}

void Sen_BreakOut::paintBricksMap()	{

	glUseProgram(blendUnitOneSquareProgram);
	glBindVertexArray(unitOneSquareVAO);
	glActiveTexture(GL_TEXTURE0);

	glm::vec3 brickColor = glm::vec3(1.0f);
	GLuint totalBricksNum = brickMapsVector.at(breakOutLevel).bricksVector.size();
	for (GLuint i = 0; i < totalBricksNum; i++)	{
		if (brickMapsVector.at(breakOutLevel).bricksVector.at(i).getBrickNotDestroyedStatus())	{
			if (brickMapsVector.at(breakOutLevel).bricksVector.at(i).getBrickIsSolidStatus())
				glBindTexture(GL_TEXTURE_2D, solidBlockTexture);
			else  glBindTexture(GL_TEXTURE_2D, blockTexture);

			model = glm::mat4();
			model = glm::translate(	model, 
				glm::vec3(brickMapsVector.at(breakOutLevel).bricksVector.at(i).getBrickPosition(), 
				0.0f)	
				);
			model = glm::scale(model, 
				glm::vec3(brickMapsVector.at(breakOutLevel).bricksVector.at(i).getBrickSize(),
				1.0f)
				); // Last scale

			glUniformMatrix4fv(glGetUniformLocation(blendUnitOneSquareProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));


			Sen_2D_BlockBrick &brick = brickMapsVector.at(breakOutLevel).bricksVector.at(i);
			GLuint brickLife = brick.getBrickLife();

			if (brickLife == 1) 			brickColor = glm::vec3(0.8f, 0.8f, 0.4f);
			else if (brickLife == 2)		brickColor = glm::vec3(1.0f, 0.5f, 0.0f);
			else if (brickLife == 3)		brickColor = glm::vec3(0.0f, 0.7f, 0.0f);
			else if (brickLife == 4)		brickColor = glm::vec3(0.9f, 0.6f, 0.9f);
			else if (brickLife == 99)		brickColor = glm::vec3(1.0f);
			else std::cout << "Wrong Brick Life !!";

			//glUniform3f(glGetUniformLocation(blendUnitOneSquareProgram, "renderColor"),
			//	brickMapsVector.at(breakOutLevel).bricksVector.at(i).getBrickColor().x,
			//	brickMapsVector.at(breakOutLevel).bricksVector.at(i).getBrickColor().y,
			//	brickMapsVector.at(breakOutLevel).bricksVector.at(i).getBrickColor().z
			//	);
			glUniform3f(glGetUniformLocation(blendUnitOneSquareProgram, "renderColor"),
				brickColor.x, brickColor.y, brickColor.z);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Sen_BreakOut::paintBall()	{

	glUseProgram(ballProgram);
	glBindVertexArray(unitOneSquareVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ballTexture);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(ballPosition, 0.0f));
	model = glm::scale(model,
		glm::vec3(ballSquareSIDE * widgetHeight / widgetWidth, ballSquareSIDE, 1.0f)
		); // Last scale
	model = glm::rotate(model, static_cast<GLfloat>(glm::radians(ballSpinAngleDegree)), glm::vec3(0.0f, 0.0f, 1.0f));


	glUniformMatrix4fv(glGetUniformLocation(ballProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(ballProgram, "renderColor"),
		1.0f, 0.0f, 0.0f);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Sen_BreakOut::paintPlayer()	{

	glUseProgram(programB);
	glBindVertexArray(playerBoardVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, playerBoardTexture);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(playerBoardPosition, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(programB, "model"), 1, GL_FALSE, glm::value_ptr(model));
	
	glUniform3f(glGetUniformLocation(programB, "renderColor"),
		0.5f, 0.5f, 0.5f);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Sen_BreakOut::paintBackground()	{

	glUseProgram(programA);
	glBindVertexArray(backgroundVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Sen_BreakOut::init2DMapInfo(std::vector<std::vector<GLuint>> &map2DBrickTypesinfo, GLuint breakOutLevel)	{

	GLuint levelOneBrickTypesMap[][15] = {
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
		{ 1, 3, 3, 2, 3, 2, 3, 2, 3, 3, 2, 3, 3, 1, 1 },
		{ 1, 2, 3, 3, 0, 3, 3, 0, 3, 3, 0, 3, 3, 0, 1 },
		{ 1, 0, 3, 3, 0, 3, 3, 0, 3, 3, 0, 3, 5, 0, 1 },
		{ 1, 3, 2, 3, 2, 3, 2, 3, 3, 2, 3, 3, 2, 3, 1 },
		{ 1, 0, 2, 2, 0, 2, 2, 0, 2, 2, 0, 2, 2, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }
	};

	GLuint levelTwoBrickTypesMap[][17] = {
		{ 2, 3, 2, 3, 2, 4, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 4 },
		{ 2, 5, 3, 2, 2, 3, 4, 3, 2, 2, 2, 3, 2, 2, 2, 2, 3 },
		{ 2, 2, 3, 2, 2, 3, 5, 3, 2, 2, 2, 3, 2, 2, 2, 2, 3 },
		{ 2, 3, 0, 3, 3, 2, 0, 0, 2, 2, 0, 3, 1, 3, 2, 3, 2 },
		{ 2, 3, 2, 3, 2, 3, 0, 0, 2, 2, 0, 3, 2, 3, 2, 3, 2 },
		{ 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 3 },
		{ 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },
		{ 3, 2, 0, 2, 2, 0, 2, 2, 0, 2, 2, 0, 2, 2, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	};

	GLuint levelThirdBrickTypesMap[][19] = {
		{ 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 5 },
		{ 4, 2, 3, 3, 1, 3, 2, 3, 4, 5, 5, 4, 3, 3, 1, 3, 3, 5, 5 },
		{ 4, 3, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 2, 5 },
		{ 4, 2, 3, 3, 1, 3, 2, 3, 4, 5, 5, 4, 3, 3, 1, 3, 3, 5, 5 },
		{ 4, 2, 4, 1, 4, 1, 4, 0, 0, 1, 0, 0, 4, 1, 4, 1, 4, 3, 5 },
		{ 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5 },
		{ 4, 2, 3, 3, 1, 3, 2, 3, 4, 5, 5, 4, 3, 3, 1, 3, 3, 5, 5 },
		{ 4, 1, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 4, 5 },
		{ 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 5 },
		{ 4, 4, 1, 0, 3, 3, 0, 3, 3, 0, 3, 3, 0, 3, 3, 0, 1, 5, 5 },
	};

	map2DBrickTypesinfo.clear();

	if (breakOutLevel == 1)	{
		GLuint rowNum = sizeof(levelOneBrickTypesMap) / sizeof(levelOneBrickTypesMap[0]);
		for (int i = 0; i < rowNum; i++)	{
			GLuint columnNum = sizeof(levelOneBrickTypesMap[i]) / sizeof(GLuint);
			std::vector<GLuint> lineBrickTypesInfo;
			for (int j = 0; j < columnNum; j++)	{
				lineBrickTypesInfo.push_back(levelOneBrickTypesMap[i][j]);
			}
			map2DBrickTypesinfo.push_back(lineBrickTypesInfo);
		}
	}
	else if (breakOutLevel == 2)	{
		GLuint rowNum = sizeof(levelTwoBrickTypesMap) / sizeof(levelTwoBrickTypesMap[0]);
		for (int i = 0; i < rowNum; i++)	{
			GLuint columnNum = sizeof(levelTwoBrickTypesMap[i]) / sizeof(GLuint);
			std::vector<GLuint> lineBrickTypesInfo;
			for (int j = 0; j < columnNum; j++)	{
				lineBrickTypesInfo.push_back(levelTwoBrickTypesMap[i][j]);
			}
			map2DBrickTypesinfo.push_back(lineBrickTypesInfo);
		}
	}
	else if (breakOutLevel == 3)	{
		GLuint rowNum = sizeof(levelThirdBrickTypesMap) / sizeof(levelThirdBrickTypesMap[0]);
		for (int i = 0; i < rowNum; i++)	{
			GLuint columnNum = sizeof(levelThirdBrickTypesMap[i]) / sizeof(GLuint);
			std::vector<GLuint> lineBrickTypesInfo;
			for (int j = 0; j < columnNum; j++)	{
				lineBrickTypesInfo.push_back(levelThirdBrickTypesMap[i][j]);
			}
			map2DBrickTypesinfo.push_back(lineBrickTypesInfo);
		}
	}
	else	{
		std::cout << "breakOutLevel Wrong !!";
	}
}

GLboolean Sen_BreakOut::checkBrickBallSquareCollision(const Sen_2D_BlockBrick &brick)
{
	// AABB - AABB collision
	GLfloat ballRadiusWidth = ballRADIUS * widgetHeight / widgetWidth;

	bool isCollidedX = brick.getBrickPosition().x + brick.getBrickSize().x / 2.0f >= ballPosition.x - ballRadiusWidth
		&& brick.getBrickPosition().x - brick.getBrickSize().x / 2.0f <= ballPosition.x + ballRadiusWidth;

	bool isCollidedY = brick.getBrickPosition().y + brick.getBrickSize().y / 2.0f >= ballPosition.y - ballRADIUS
		&& brick.getBrickPosition().y - brick.getBrickSize().y / 2.0f <= ballPosition.y + ballRADIUS;

	return isCollidedX && isCollidedY;
}

void Sen_BreakOut::clean_2D_GrameFrame()	{
	if (glIsVertexArray(backgroundVAO))		glDeleteVertexArrays(1, &backgroundVAO);
	if (glIsBuffer(backgroundVBO))			glDeleteBuffers(1, &backgroundVBO);
	if (glIsTexture(backgroundTexture))		glDeleteTextures(1, &backgroundTexture);

	if (glIsVertexArray(playerBoardVAO))		glDeleteVertexArrays(1, &playerBoardVAO);
	if (glIsBuffer(playerBoardVBO))			glDeleteBuffers(1, &playerBoardVBO);
	if (glIsTexture(playerBoardTexture))		glDeleteTextures(1, &playerBoardTexture);

	if (glIsVertexArray(unitOneSquareVAO))		glDeleteVertexArrays(1, &unitOneSquareVAO);
	if (glIsBuffer(unitOneSquareVBO))			glDeleteBuffers(1, &unitOneSquareVBO);
	if (glIsTexture(blockTexture))			glDeleteTextures(1, &blockTexture);
	if (glIsTexture(solidBlockTexture))		glDeleteTextures(1, &solidBlockTexture);
	if (glIsTexture(ballTexture))		glDeleteTextures(1, &ballTexture);

	if (glIsProgram(programA))					glDeleteProgram(programA);
	if (glIsProgram(programB))					glDeleteProgram(programB);
	if (glIsProgram(blendUnitOneSquareProgram))	glDeleteProgram(blendUnitOneSquareProgram);
	if (glIsProgram(ballProgram))				glDeleteProgram(ballProgram);
}


void Sen_BreakOut::daltaTimeUpdate(GLfloat deltaTime)	{
		
	if (gameActive && !currLevelFinished)
	{
		GLfloat palyerShift = PLAYER_VELOCITY * deltaTime;

		// ******************* Key Input Process*************************************
		if (keys[GLFW_KEY_LEFT])
		{
			playerBoardPosition.x -= palyerShift;
			GLfloat xLeftBorder = -1.0f + playerBoardWidth / 2.0;
			if (playerBoardPosition.x < xLeftBorder)
				playerBoardPosition.x = xLeftBorder;

			if (isBallStuckOnPlayerBoard)	ballPosition.x = playerBoardPosition.x;
		}
		if (keys[GLFW_KEY_RIGHT])
		{
			playerBoardPosition.x += palyerShift;
			GLfloat xRightBorder = 1.0f - playerBoardWidth / 2.0;
			if (playerBoardPosition.x > xRightBorder)
				playerBoardPosition.x = xRightBorder;

			if (isBallStuckOnPlayerBoard)	ballPosition.x = playerBoardPosition.x;
		}
		if (keys[GLFW_KEY_SPACE])		isBallStuckOnPlayerBoard = GL_FALSE;


		// ************ Ball Movement ***********************************************
		if (!isBallStuckOnPlayerBoard)
		{
			// Move the ball
			ballSpinAngleDegree += ballSpinSpeed * deltaTime;
			normalizeAngleDegree(ballSpinAngleDegree);

			ballPosition += ballVelocity * deltaTime;
			GLfloat ballRadiusWidth = ballRADIUS * widgetHeight / widgetWidth;

			if (ballPosition.x <= -1.0f + ballRadiusWidth)
			{
				ballVelocity.x = -ballVelocity.x;
				ballPosition.x = -1.0f + ballRadiusWidth;

				ballSpinSpeed = -ballSpinSpeed;
			}
			else if (ballPosition.x >= 1.0f - ballRadiusWidth)
			{
				ballVelocity.x = -ballVelocity.x;
				ballPosition.x = 1.0f - ballRadiusWidth;

				ballSpinSpeed = -ballSpinSpeed;
			}

			if (ballPosition.y >= 1.0f - ballRADIUS)
			{
				ballVelocity.y = -ballVelocity.y;
				ballPosition.y = 1.0f - ballRADIUS;

				ballSpinSpeed = -ballSpinSpeed;
			}
			else if (ballPosition.y <= -1.0f - ballRADIUS)
			{
				initBallVariables();
			}
		}

		//bricksCollisionKill();
		bricksCollisionKill_resolution();

	}
}

GLboolean Sen_BreakOut::checkBrickBallCircleCollision(const Sen_2D_BlockBrick &brick)
{
	glm::vec2 ballPixelPosition = glm::vec2(ballPosition.x * static_cast<GLfloat>(widgetWidth) / 2.0f
		, ballPosition.y * static_cast<GLfloat>(widgetHeight) / 2.0f);

	glm::vec2 brickPixelPosition = glm::vec2(brick.getBrickPosition().x * static_cast<GLfloat>(widgetWidth) / 2.0f
		, brick.getBrickPosition().y * static_cast<GLfloat>(widgetHeight) / 2.0f);

	glm::vec2 brickPixelSize = glm::vec2(brick.getBrickSize().x * static_cast<GLfloat>(widgetWidth) / 2.0f
		, brick.getBrickSize().y * static_cast<GLfloat>(widgetHeight) / 2.0f);

	glm::vec2 brickBallPixelDistance = ballPixelPosition - brickPixelPosition;

	// if Ball's Center is inside a Block, return TRUE directly
	if (std::abs(brickBallPixelDistance.x) < brickPixelSize.x / 2.0f
		&& std::abs(brickBallPixelDistance.y) < brickPixelSize.y / 2.0f)
		return GL_TRUE;
	// otherwise Find the closest point on block to the Ball's Center
	else {

		glm::vec2 clampedBrickBallPixelDistance =
			glm::clamp(brickBallPixelDistance, -brickPixelSize / 2.0f, brickPixelSize / 2.0f);

		glm::vec2 closestPixelPosition = brickPixelPosition + clampedBrickBallPixelDistance;
		glm::vec2 closetPixelToBallCenter = closestPixelPosition - ballPixelPosition;
		
		GLfloat distanceInPixel = glm::length(closetPixelToBallCenter);
		GLfloat ballRadiusInPixel = ballRADIUS * static_cast<GLfloat>(widgetHeight) / 2.0f;
		
		GLboolean isClosetPointInCircle = distanceInPixel < ballRadiusInPixel;

		return isClosetPointInCircle;
	}
}

void Sen_BreakOut::bricksCollisionKill()
{
	// Check Bricks + LogoCube Collision
	if (ballPosition.y >= 0.0f - ballRADIUS)	{

		//for (Sen_2D_BlockBrick &brick : brickMapsVector.at(breakOutLevel).bricksVector)
		for (GLuint i = 0; i < brickMapsVector.at(breakOutLevel).bricksVector.size(); i++)
		{
			Sen_2D_BlockBrick &brick = brickMapsVector.at(breakOutLevel).bricksVector.at(i);
			if (i == 41)
				std::cout << "I am here";

			if (brick.getBrickNotDestroyedStatus())
			{
				if (checkBrickBallCircleCollision(brick))
				{
					if (!brick.getBrickIsSolidStatus())
						brick.setBrickNotDestroyStatus(GL_FALSE);
				}
			}
		}

	} 
	// Check playerBoard Collision
	else if (ballPosition.y <= -1.0f + originalPlayerBOARDHEIGHT + ballRADIUS)		{

	}
}

void Sen_BreakOut::keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(widget, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;

		if (key == GLFW_KEY_P && action == GLFW_RELEASE) gameActive = !gameActive;
	}
}











Direction Sen_BreakOut::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

Collision Sen_BreakOut::returnBrickBallCircleCollision(const Sen_2D_BlockBrick &brick) // AABB - Circle collision
{
	glm::vec2 ballPixelPosition = glm::vec2(ballPosition.x * static_cast<GLfloat>(widgetWidth) / 2.0f
		, ballPosition.y * static_cast<GLfloat>(widgetHeight) / 2.0f);

	glm::vec2 brickPixelPosition = glm::vec2(brick.getBrickPosition().x * static_cast<GLfloat>(widgetWidth) / 2.0f
		, brick.getBrickPosition().y * static_cast<GLfloat>(widgetHeight) / 2.0f);

	glm::vec2 brickPixelSize = glm::vec2(brick.getBrickSize().x * static_cast<GLfloat>(widgetWidth) / 2.0f
		, brick.getBrickSize().y * static_cast<GLfloat>(widgetHeight) / 2.0f);

	glm::vec2 brickBallPixelDistance = ballPixelPosition - brickPixelPosition;


	glm::vec2 clampedBrickBallPixelDistance =
		glm::clamp(brickBallPixelDistance, -brickPixelSize / 2.0f, brickPixelSize / 2.0f);

	glm::vec2 closestPixelPosition = brickPixelPosition + clampedBrickBallPixelDistance;
	glm::vec2 closetPixelToBallCenter = closestPixelPosition - ballPixelPosition;

	GLfloat distanceInPixel = glm::length(closetPixelToBallCenter);
	GLfloat ballRadiusInPixel = ballRADIUS * static_cast<GLfloat>(widgetHeight) / 2.0f;

	//GLboolean isClosetPointInCircle = distanceInPixel < ballRadiusInPixel;

	if (distanceInPixel < ballRadiusInPixel) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
		return std::make_tuple(GL_TRUE, VectorDirection(closetPixelToBallCenter)
		, glm::vec2(closetPixelToBallCenter.x * 2.0f / static_cast<GLfloat>(widgetWidth),
		closetPixelToBallCenter.y * 2.0f / static_cast<GLfloat>(widgetHeight)));
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));

}



void Sen_BreakOut::bricksCollisionKill_resolution()
{
	// Check Bricks + LogoCube Collision
	if (ballPosition.y >= 0.0f - ballRADIUS)	{

		//for (Sen_2D_BlockBrick &brick : brickMapsVector.at(breakOutLevel).bricksVector)
		for (GLuint i = 0; i < brickMapsVector.at(breakOutLevel).bricksVector.size(); i++)
		{
			Sen_2D_BlockBrick &brick = brickMapsVector.at(breakOutLevel).bricksVector.at(i);

			if (brick.getBrickNotDestroyedStatus())
			{
				Collision collision = returnBrickBallCircleCollision(brick);
				if (std::get<0>(collision)) // If collision is true
				{
					// Destroy block if not solid
					if (!brick.getBrickIsSolidStatus())
						if (brick.destroyedAfterHitBrick())	
							if (currLevelUnsolidBrickNum)
								if (!--currLevelUnsolidBrickNum)
									currLevelFinished = GL_TRUE;

					// Collision resolution
					Direction dir = std::get<1>(collision);
					glm::vec2 diff_vector = std::get<2>(collision);
					if (dir == LEFT__ || dir == RIGHT__) // Horizontal collision
					{
						ballVelocity.x = -ballVelocity.x; // Reverse horizontal velocity
						// Relocate
						GLfloat penetration = ballRADIUS - std::abs(diff_vector.x);
						if (dir == LEFT__)
							ballPosition.x += penetration; // Move ball to right
						else
							ballPosition.x -= penetration; // Move ball to left;
					}
					else // Vertical collision
					{
						ballVelocity.y = -ballVelocity.y; // Reverse vertical velocity
						// Relocate
						GLfloat penetration = ballRADIUS - std::abs(diff_vector.y);
						if (dir == UP)
							ballPosition.y -= penetration; // Move ball bback up
						else
							ballPosition.y += penetration; // Move ball back down
					}
				}

			}
		}

	}
	// Check playerBoard Collision
	else if (ballPosition.y <= -1.0f + originalPlayerBOARDHEIGHT + ballRADIUS)		{
		// ************** Change when Better Collision Detection **************************
		Sen_2D_BlockBrick brick;
		brick.setBrickPosition(playerBoardPosition);
		brick.setBrickSize(glm::vec2(originalPlayerBOARDWIDTH, originalPlayerBOARDHEIGHT));

		Collision result = returnBrickBallCircleCollision(brick);
		if (!isBallStuckOnPlayerBoard && std::get<0>(result))
		{
			// Check where it hit the board, and change velocity based on where it hit the board
			GLfloat centerBoard = playerBoardPosition.x;
			GLfloat distance = ballPosition.x - centerBoard;
			GLfloat percentage = distance / (playerBoardWidth / 2.0f);
			// Then move accordingly
			GLfloat strength = 2.0f;
			glm::vec2 oldVelocity = ballVelocity;
			ballVelocity.x = originalBallVELOCITY.x * percentage * strength;
			//Ball->Velocity.y = -Ball->Velocity.y;
			ballVelocity = glm::normalize(ballVelocity) * glm::length(oldVelocity); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
			// Fix sticky paddle
			ballVelocity.y = abs(ballVelocity.y);
			ballVelocity.x = ballVelocity.x * oldVelocity.x / abs(oldVelocity.x);
		}

		// ************** Change when Better Collision Detection **************************

	}
}
