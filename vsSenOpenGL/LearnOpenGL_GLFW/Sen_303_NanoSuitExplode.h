#pragma once

#ifndef __Sen_303_NanoSuitExplode__
#define __Sen_303_NanoSuitExplode__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"
#include "Support/SenMeshLinkModel.h"


class Sen_303_NanoSuitExplode :	public SenFreeSpaceAbstract
{
public:
	Sen_303_NanoSuitExplode();
	virtual ~Sen_303_NanoSuitExplode();

protected:
	void initialGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);

	SenMeshLinkModel *nanoSuitModel;
};


#endif