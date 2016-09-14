#include "SenTeapotObservationWidget.h"
#include "Sen_01_Triangles.h"
#include "SenAbstractGLWidget.h"

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	//SenTeapotObservationWidget teapotWidget;
	//teapotWidget.showWidget();

	Sen_01_Triangles triangleWidget;
	triangleWidget.showWidget();

	return 1;
}
























//#include <iostream>
//
//int main(int argc, char **argv)
//{
//	glutInit(&argc, argv);
//	glutInitWindowSize(640, 480);
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
//	glutInitContextProfile(GLUT_CORE_PROFILE);
//	glutCreateWindow("GLUT");
//	glewExperimental = GL_TRUE;
//	glewInit();
//	std::cout << glGetString(GL_VERSION) << std::endl;
//	getchar();
//	return 0;
//}