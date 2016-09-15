#pragma once

#include "SenAbstractGLWidget.h"

class SenTeapotObservationWidget: public SenAbstractGLWidget
{
public:
	SenTeapotObservationWidget();
	~SenTeapotObservationWidget();

protected:
	void paintGL(void);
	void initialGlutGlewGL();

private:

};

