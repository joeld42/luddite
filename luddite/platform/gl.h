#ifndef PLATFORM_GL_H
#define PLATFORM_GL_H

// platform specific ways of including OpenGL

// For now use GLee
#include <luddite/platform/GLee.h>

// opengl itself
#ifdef EA_PLATFORM_APPLE
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

// useful GL error checking 
namespace luddite
{

int checkForGLErrors( const char *s, const char * file, int line );
void checkVBO();

};  // namespace luddite

#define CHECKGL( msg ) luddite::checkForGLErrors( msg, __FILE__, __LINE__ )
    

#endif
