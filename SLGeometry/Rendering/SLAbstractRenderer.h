#ifndef __SLABSTRACTRENDERER__
#define __SLABSTRACTRENDERER__
#pragma once

//-------------------------- OpenGL Graphics --------------------------------------
#define GLEW_STATIC
#include <GL/glew.h>       // Support basic OpenGL

namespace slgeom
{
	class SLAbstractRenderer
	{
	public:
		SLAbstractRenderer();
		virtual ~SLAbstractRenderer();





	};

}// End of namespace slgeom

#endif // __SLABSTRACTRENDERER__