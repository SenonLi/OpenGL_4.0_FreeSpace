#include "Sen_2D_GrameFrame.h"


Sen_2D_GrameFrame::Sen_2D_GrameFrame()
	:textureLoadImagePtr(NULL)
{
	strWindowName = "Sen GLFW 2D Game Space";
	widgetWidth = Sen2DGameSpace_widgetWidth;
	widgetHeight = Sen2DGameSpace_widgetHeight;
}


Sen_2D_GrameFrame::~Sen_2D_GrameFrame()
{
}


void Sen_2D_GrameFrame::initGlfwGlewGL()
{
	SenAbstractGLFW::initGlfwGlewGL();

	Sen2DGameLogoCube.initialCubeGL();

	// Add Stencil Test for Perfect Cube Logo
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0x01, 0xFF);    // Check GL_STENCIL_TEST only when needed;  All fragments should update the
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x00); // Disable writing to the stencil buffer

	OutputDebugString(" Sen_2D_GrameFrame Initial \n\n");

	init2DGamePaceGL();
}

void Sen_2D_GrameFrame::paintGL(void)
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

	// Paint Perfect Sen FreeSpace Logo
	Sen2DGameLogoCube.paintSenLogoCube(GLfloat(widgetWidth) / GLfloat(Sen2DGameSpace_widgetWidth), GLfloat(widgetHeight) / GLfloat(Sen2DGameSpace_widgetHeight));

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


void Sen_2D_GrameFrame::finalize(void)
{
	clean_2D_GrameFrame();

	// Clean SenFreeSpaceLogo
	Sen2DGameLogoCube.finalizeCube();
}

void Sen_2D_GrameFrame::keyDetection(GLFWwindow* widget, int key, int scancode, int action, int mode)
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
