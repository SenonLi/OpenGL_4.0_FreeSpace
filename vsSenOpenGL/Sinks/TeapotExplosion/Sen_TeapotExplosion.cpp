#include "Sen_TeapotExplosion.h"


Sen_TeapotExplosion::Sen_TeapotExplosion()
{
	strWindowName = "Sen Teapot Explosion in FreeSpace";
}


Sen_TeapotExplosion::~Sen_TeapotExplosion()
{
}



void Sen_TeapotExplosion::initGlfwGlewGL()
{
	Sen_24_BlendingTest::initGlfwGlewGL();
	// Draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	ShaderInfo teapotShader[] =
	{
		{ GL_VERTEX_SHADER, "./Sinks/Shaders/teapotExplosion.vert" },
		{ GL_FRAGMENT_SHADER, "./Sinks/Shaders/teapotExplosion.frag" },
		{ GL_NONE, NULL }
	};

	teapotProgram = LoadShaders(teapotShader);
	glUseProgram(teapotProgram);

	firstTeapot.initTeapot();


	OutputDebugString(" Sen_TeapotExplosion Initial \n\n");
}

void Sen_TeapotExplosion::paintScene(void)	{

	//Sen_24_BlendingTest::paintScene();

	firstTeapot.paintLinkedProgramTeapot(teapotProgram, projection, view);


	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


void Sen_TeapotExplosion::cleanDebugWindowFreeSpace()	{

	Sen_24_BlendingTest::cleanDebugWindowFreeSpace();

	firstTeapot.finalizeTeapot();
}
