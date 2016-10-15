#pragma once
#ifndef __Sen_30_GeometryShow__
#define __Sen_30_GeometryShow__

#include "Support/SenAbstractGLFW.h"

class Sen_30_GeometryShow :	public SenAbstractGLFW
{
public:
	Sen_30_GeometryShow();
	virtual ~Sen_30_GeometryShow();

protected:
	void paintGL(void);
	void initGlfwGlewGL();
	void finalize();

	void initialVertices();

};

#endif