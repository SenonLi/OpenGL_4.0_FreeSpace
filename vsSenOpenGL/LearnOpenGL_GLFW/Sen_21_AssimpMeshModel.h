#pragma once
#ifndef __Sen_21_AssimpMeshModel__
#define __Sen_21_AssimpMeshModel__

#include "LearnOpenGL_GLFW/SenFreeSpaceAbstract.h"

#include "SenFreeSpacePieces/Sen_Cube.h"
#include "Support/SenMeshLinkModel.h"

class Sen_21_AssimpMeshModel :	public SenFreeSpaceAbstract
{
public:
	Sen_21_AssimpMeshModel();
	virtual ~Sen_21_AssimpMeshModel();

protected:
	void paintGL(void);
	void initialGlfwGlewGL();

	//void finalize();

	Sen_Cube stillCube;
	SenMeshLinkModel *nanoSuitModel;

};



#endif