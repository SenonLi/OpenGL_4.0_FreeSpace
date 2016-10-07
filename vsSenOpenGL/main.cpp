//#include "Sinks/SenTeapotObservationWidget.h"
//#include "Guide8th/Sen_01_Triangles.h"
//#include "Guide8th/Sen_03_DrawCommands.h"
//#include "Guide8th/Sen_06_CubeMap.h"
//
//#include "Sinks/SenShaderTeapotExplosion.h"
//#include "LearnOpenGL_GLFW/Sen_06_TrianglesGLFW.h"
//#include "LearnOpenGL_GLFW/Sen_07_TextureGLFW.h"
//#include "LearnOpenGL_GLFW/Sen_09_ModelViewProjection.h"
#include "LearnOpenGL_GLFW/Sen_10_Camera.h"
//#include "LearnOpenGL_GLFW/Sen_30_GeometryShow.h"

int main(int argc, char *argv[])
{
	//Sen_10_Camera cameraWidget;
	//cameraWidget.showWidget();

	Sen_10_Camera glfwSenCubeWidget;
	glfwSenCubeWidget.showWidget();

	//Sen_06_TrianglesGLFW glfwTextureWidget;
	//glfwTextureWidget.showWidget();

	//SenTeapotObservationWidget teapotWidget;
	//teapotWidget.showWidget();

	//SenShaderTeapotExplosion shaderTeapotWidget;
	//shaderTeapotWidget.showWidget();


	//Sen_30_GeometryShow triangleWidget;
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