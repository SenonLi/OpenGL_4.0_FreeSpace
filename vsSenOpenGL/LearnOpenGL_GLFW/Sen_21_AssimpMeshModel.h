#pragma once
#ifndef __Sen_21_AssimpMeshModel__
#define __Sen_21_AssimpMeshModel__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

#include "Support/SenMeshLinkModel.h"

class Sen_21_AssimpMeshModel :	public SenFreeSpaceAbstract
{
public:
	Sen_21_AssimpMeshModel();
	virtual ~Sen_21_AssimpMeshModel();

protected:
	void initialGlfwGlewGL();
	void paintFreeSpaceGL(void);
	void cleanFreeSpace(void);

	//void finalize();

	SenMeshLinkModel *nanoSuitModel;
};



#endif