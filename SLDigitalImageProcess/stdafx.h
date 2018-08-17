// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
// TODO: reference additional headers your program requires here

//-------------------------- SLDIP Graphics --------------------------------------
#define GLEW_STATIC
#include <GL/glew.h>       // Support basic OpenGL

#include <gli/gli.hpp> // contains <cassert>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <soil/SOIL.h>

//-------------------------- Debug Helper  --------------------------------------
#include <cassert> // assert
#include "../SLInternalUtility/DebugHelper/SLDebugHelper.h"
