#include "stdafx.h"
//#include "LearnOpenGL_GLFW/Sen_24_BlendingTest.h"
#include "Sinks/BreakOut/Sen_BreakOut.h"
#include "SLDigitalImageProcess.h"

#include "SLBinaryEllipseCorrelation.h"
using namespace slopencv;
//#include "OpenCVTutorialExamples/CVFindContours.h"
#include "SLPointToEllipse.h"

int main(int argc, char *argv[])
{
	SLPointToEllipse cameraWidget;
	////SLDigitalImageProcess cameraWidget;
	//cameraWidget.ShowWidget();
	cameraWidget.Samples_CaculateDistanceFromPointToEllipse();


	while (true);
	return 1;
}


