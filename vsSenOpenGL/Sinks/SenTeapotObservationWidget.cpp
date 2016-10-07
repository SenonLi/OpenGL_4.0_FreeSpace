#include "SenTeapotObservationWidget.h"
#include "teapot.h"


SenTeapotObservationWidget::SenTeapotObservationWidget()
{

}

SenTeapotObservationWidget::~SenTeapotObservationWidget()
{
	OutputDebugString("\n ~SenTeapotObservationWidget()\n");
}

void SenTeapotObservationWidget::initialGlutGlewGL()
{
	SenAbstractGLWidget::initialGlutGlewGL();


	OutputDebugString("\n Initial teapotExplosion\n");
}

void SenTeapotObservationWidget::paintGL(void)
{
	SenAbstractGLWidget::paintGL();

	drawTeapot();


	/* swap front and back buffers */
	glFlush();
}