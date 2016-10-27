#include "Sen_47_BlendSprite.h"


Sen_47_BlendSprite::Sen_47_BlendSprite()
{
}


Sen_47_BlendSprite::~Sen_47_BlendSprite()
{
}


void Sen_47_BlendSprite::init2DGamePaceGL()	{

	//ShaderInfo shadersScreenTextureCoordsRender[] = {
	//	{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/Sen_3D_TextureCoords.vert" },
	//	{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/Sen_TextureCoordsRender.frag" },
	//	{ GL_NONE, NULL }
	//};
	ShaderInfo shadersScreenTextureCoordsRender[] = {
		{ GL_VERTEX_SHADER, "./SenFreeSpacePieces/Shaders/Sen_2D_TextureCoords.vert" },
		{ GL_FRAGMENT_SHADER, "./SenFreeSpacePieces/Shaders/Sen_TextureCoordsRender.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shadersScreenTextureCoordsRender);

	initVertexAttributes();
	uploadTextureImage(std::string("./LearnOpenGL_GLFW/Images/awesomeface.png").c_str(), spriteTexture, "RGBA");
	spriteRenderColor = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Sen_47_BlendSprite::initVertexAttributes(){

	GLfloat halfWidth = 3 / 8.0, halfHeight = 2 / 3.0;
	GLfloat spriteVertices[] = {
		// Pos      // Tex
		-halfWidth, -halfHeight, 0.0f, 1.0f,
		halfWidth,   halfHeight, 1.0f, 0.0f,
		-halfWidth,  halfHeight, 0.0f, 0.0f,

		-halfWidth, -halfHeight, 0.0f, 1.0f,
		halfWidth, -halfHeight, 1.0f, 1.0f,
		halfWidth, halfHeight, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &spriteVAO);
	glGenBuffers(1, &spriteVBO);

	glBindVertexArray(spriteVAO);
	glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertices), spriteVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sen_47_BlendSprite::paint2DGameSpaceGL()	{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Paint Floor
	glUseProgram(programA);
	glBindVertexArray(spriteVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, spriteTexture);

	model = glm::mat4();
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	glUniformMatrix4fv(glGetUniformLocation(programA, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glUniform3f(glGetUniformLocation(programA, "renderColor"), spriteRenderColor.x, spriteRenderColor.y, spriteRenderColor.z);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glDisable(GL_BLEND);
}

void Sen_47_BlendSprite::clean_2D_GrameFrame()	{
	if (glIsVertexArray(spriteVAO))		glDeleteVertexArrays(1, &spriteVAO);
	if (glIsBuffer(spriteVBO))			glDeleteBuffers(1, &spriteVBO);
	if (glIsTexture(spriteTexture))		glDeleteTextures(1, &spriteTexture);

	if (glIsProgram(programA))	glDeleteProgram(programA);
}
