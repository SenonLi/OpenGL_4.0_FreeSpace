#ifndef __SL_SHADER_PROGRAM__
#define __SL_SHADER_PROGRAM__
#pragma once

extern "C" {
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;
}

namespace slgeom
{
	//----------------------------------------------------------------------------
	//
	//  LoadShaders() takes an array of ShaderFile structures, each of which
	//    contains the type of the shader, and a pointer a C-style character
	//    string (i.e., a NULL-terminated array of characters) containing the
	//    entire shader source.
	//
	//  The array of structures is terminated by a final Shader with the
	//    "type" field set to GL_NONE.
	//
	//  LoadShaders() returns the shader program value (as returned by
	//    glCreateProgram()) on success, or zero on failure. 
	//

	typedef struct {
		GLenum       type;
		int          resourceID;
		GLuint       shader;
	}PipelineShaders;


	GLuint CreatePipelineShaderProgram(PipelineShaders*);

}//End of namespace slgeom

#endif // __SL_SHADER_PROGRAM__
