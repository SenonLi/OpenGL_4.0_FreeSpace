#include "Sen_BreakOut.h"


Sen_BreakOut::Sen_BreakOut()
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
}

void Sen_BreakOut::initTextures(){
	uploadTextureImage(std::string("./../WatchMe/Images/SunRaise.jpg").c_str(), backgroundTexture, "RGBA");
}

void Sen_BreakOut::initVertexAttributes(){

	// Initial Background Vertices
	GLfloat backgroundVertexAttributes[] = {
		// Positions            // Texture Coords 
		1.0f, 1.0f,  1.0f, 0.0f,
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

	paintBackground();

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//model = glm::mat4();
	//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	//glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//glUniform3f(glGetUniformLocation(programA, "renderColor"), spriteRenderColor.x, spriteRenderColor.y, spriteRenderColor.z);

	//glDisable(GL_BLEND);
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

void Sen_BreakOut::clean_2D_GrameFrame()	{
	if (glIsVertexArray(backgroundVAO))		glDeleteVertexArrays(1, &backgroundVAO);
	if (glIsBuffer(backgroundVBO))			glDeleteBuffers(1, &backgroundVBO);
	if (glIsTexture(backgroundTexture))		glDeleteTextures(1, &backgroundTexture);

	if (glIsProgram(programA))	glDeleteProgram(programA);
}