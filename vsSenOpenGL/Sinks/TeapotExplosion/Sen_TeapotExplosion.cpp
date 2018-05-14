#include "Sen_TeapotExplosion.h"


Sen_TeapotExplosion::Sen_TeapotExplosion()
{
	strWindowName = "Sen Teapot Explosion in FreeSpace";

	camera.Position = glm::vec3(1.0f, 1.0f, 2.0f);
}


Sen_TeapotExplosion::~Sen_TeapotExplosion()
{
}



void Sen_TeapotExplosion::initGlfwGlewGL()
{
	Sen_24_BlendingTest::initGlfwGlewGL();

	//ShaderInfo teapotShader[] =
	//{
	//	{ GL_VERTEX_SHADER, "./Sinks/TeapotExplosion/Shaders/teapotExplosion.vert" },
	//	{ GL_GEOMETRY_SHADER, "./Sinks/TeapotExplosion/Shaders/teapotExplosion.geom" },
	//	{ GL_FRAGMENT_SHADER, "./Sinks/TeapotExplosion/Shaders/teapotExplosion.frag" },
	//	{ GL_NONE, NULL }
	//};
	
	ShaderInfo teapotShader[] =
	{
		{ GL_VERTEX_SHADER, "./../WatchMe/Shaders/teapotExplosion.vert" },
		{ GL_GEOMETRY_SHADER, "./../WatchMe/Shaders/teapotExplosion.geom" },
		{ GL_FRAGMENT_SHADER, "./../WatchMe/Shaders/teapotExplosion.frag" },
		{ GL_NONE, NULL }
	};


	teapotProgram = LoadShaders(teapotShader);
	glUseProgram(teapotProgram);

	firstTeapot.initTeapot();


	//OutputDebugString(" Sen_TeapotExplosion Initial \n\n");
}

void Sen_TeapotExplosion::paintScene(void)	{

	Sen_24_BlendingTest::paintScene();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	firstTeapot.paintLinkedProgramTeapot(teapotProgram, projection, view);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


void Sen_TeapotExplosion::cleanDebugWindowFreeSpace()	{

	Sen_24_BlendingTest::cleanDebugWindowFreeSpace();

	firstTeapot.finalizeTeapot();
}
