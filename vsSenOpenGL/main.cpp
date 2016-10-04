//#include "SenTeapotObservationWidget.h"
//#include "Sen_01_Triangles.h"
//#include "Sen_03_DrawCommands.h"
//#include "Sen_06_CubeMap.h"

//#include "SenShaderTeapotExplosion.h"
#include "Sen_07_TextureGLFW.h"
//#include "Sen_06_TrianglesGLFW.h"

int main(int argc, char *argv[])
{

	Sen_07_TextureGLFW glfwTextureWidget;
	glfwTextureWidget.showWidget();

	//SenTeapotObservationWidget teapotWidget;
	//teapotWidget.showWidget();

	//SenShaderTeapotExplosion shaderTeapotWidget;
	//shaderTeapotWidget.showWidget();


	//Sen_01_Triangles triangleWidget;
	//triangleWidget.showWidget();

	//Sen_03_DrawCommands drawCommandWidget;
	//drawCommandWidget.showWidget();

	//Sen_06_CubeMap cubeMapWidget;
	//cubeMapWidget.showWidget();

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