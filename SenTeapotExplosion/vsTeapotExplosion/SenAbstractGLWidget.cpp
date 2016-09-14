#include "SenAbstractGLWidget.h"

float mouseX = 0.0, mouseY = 0.0;
bool mouseRightDown = false, mouseLeftDown = false;
SenAbstractGLWidget* currentInstance;

SenAbstractGLWidget::SenAbstractGLWidget()
{
	mouseX = 0.0;
	mouseY = 0.0;
	mouseRightDown = false;
	mouseLeftDown = false;
	xRot = 0; 
	yRot = 0;
}

SenAbstractGLWidget::~SenAbstractGLWidget()
{
	OutputDebugString("\n ~SenAbstractGLWidget()\n");
}

void SenAbstractGLWidget::initialGlutGlewGL()
{
	// initial GLUT for windowing
	glutInitDisplayMode(GLUT_RGBA);// | GLUT_DEPTH);	//set display mode to have:	- a colour buffer (RGBA)- double buffering (for swap)- a depth buffer
	glutInitWindowSize(640, 480);// set window size 
	glutInitWindowPosition(600, 300);           // window location

	glutCreateWindow("Sen Teapot");
	glClearColor(0.2, 0.2, 0.2, 1.0f);

	glewExperimental = GL_TRUE;
	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}

	/* enable lighting */
	glEnable(GL_LIGHTING);
	/* enable light zero (note: uses default values) */
	glEnable(GL_LIGHT0);
	/* enable depth testing */
	glEnable(GL_DEPTH_TEST);
	/* enable color and material */
	glEnable(GL_COLOR_MATERIAL);



	OutputDebugString("\n Initial OpenGL\n");
}


void SenAbstractGLWidget::paintGL(void)
{
	/* clear the color buffer and depth buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(yRot, 1, 0, 0);   // pitch
	glRotatef(xRot, 0, 1, 0);   // heading


	/* draw and show buffers */
	//glFlush();
}

void _Reshape(int width, int height)
{
	int side = width > height ? height : width;
	/* set the viewport to the window width and height */
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	/* load a projection matrix that matches the window aspect ratio */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
	glOrtho(-5.0, +5.0, -5.0, +5.0, -5.0, +5.0);

	/* return to modelview matrix mode*/
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void _Key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: /* escape key */
		exit(0);
		break;
	}
}

extern "C" void _Draw()
{
	currentInstance->_protectedPaintGL();
}

void SenAbstractGLWidget::drawMethodRegister()
{
	::currentInstance = this;
	::glutDisplayFunc(::_Draw);
}

extern "C" void _MouseMove(int x, int y)
{
	currentInstance->_protectedMouseMove(x, y);
}

void SenAbstractGLWidget::mouseMoveRegister()
{
	::currentInstance = this;
	::glutMotionFunc(::_MouseMove);
}

void SenAbstractGLWidget::showWidget()
{
	initialGlutGlewGL();

	glutReshapeFunc(_Reshape); /* register callback for window resize */
	glutKeyboardFunc(_Key);    /* register callback for keyboard input */
	glutMouseFunc(mousePress);
	//glutDisplayFunc(SenAbstractGLWidget::_Draw);    /* register callback for window redraw */
	drawMethodRegister();
	mouseMoveRegister();

	glutMainLoop(); /* start the main event loop */
}

int SenAbstractGLWidget::qtNormalizeAngle(int angle)
{
	while (angle < 0)   angle += 360;
	while (angle > 360) angle -= 360;
	return angle;
}

void SenAbstractGLWidget::mouseMove(int x, int y)
{
	int dx = x - mouseX;
	int dy = y - mouseY;

	if (mouseLeftDown) {
		xRot = qtNormalizeAngle(xRot + dx);
		yRot = qtNormalizeAngle(yRot + dy);
	}

	mouseX = x;
	mouseY = y;

	glutPostRedisplay();
}



void mousePress(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)			mouseLeftDown = true;
		else if (state == GLUT_UP)		mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)			mouseRightDown = true;
		else if (state == GLUT_UP)		mouseRightDown = false;
	}
}

