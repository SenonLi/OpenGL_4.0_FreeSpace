#include "Sen_26_FrameBufferTest.h"


Sen_26_FrameBufferTest::Sen_26_FrameBufferTest()
{
	strWindowName = "Sen_26  FrameBuffer Test";

}


Sen_26_FrameBufferTest::~Sen_26_FrameBufferTest()
{
}



void Sen_26_FrameBufferTest::initialGlfwGlewGL()
{
	SenFreeSpaceAbstract::initialGlfwGlewGL();

	// Setup some OpenGL options
	glDepthFunc(GL_LESS);

	glfwSetInputMode(widgetGLFW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.vert" },
		{ GL_FRAGMENT_SHADER, "./LearnOpenGL_GLFW/Shaders/Sen_09_ModelViewProjection.frag" },
		{ GL_NONE, NULL }
	};
	programA = LoadShaders(shaders);







	OutputDebugString(" Sen_10 First Cube Initial \n\n");
}

void Sen_26_FrameBufferTest::paintFreeSpaceGL(void)
{
	glUseProgram(programA);







	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}


void Sen_26_FrameBufferTest::cleanFreeSpace(void)	{
	if (glIsTexture(defaultTextureID))			glDeleteTextures(1, &defaultTextureID);

	if (glIsVertexArray(verArrObjArray[0]))		glDeleteVertexArrays(1, verArrObjArray);
	if (glIsBuffer(verBufferObjArray[0]))		glDeleteBuffers(1, verBufferObjArray);
	//if (glIsBuffer(verIndicesObjArray[0]))		glDeleteBuffers(1, verIndicesObjArray);

	if (glIsProgram(programA))				glDeleteProgram(programA);
}