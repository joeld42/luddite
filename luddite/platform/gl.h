#ifndef PLATFORM_GL_H
#define PLATFORM_GL_H

#include <EABase/eabase.h>

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

// =================================================================
// Helpers for bookkeeping vertex types
// =================================================================

// For example:
//
//  struct TestVert
//  {
//     float pos[3];
//     float st[2];
//
//      static void bind()
//      {  
//          BIND_VERTEX( TestVert, pos );
//          BIND_TEXTURE_COORD( TestVert, st );        
//      }
//
//      MAKE_BINDABLE( TestVert );    
//  };
//
//  GBuff<TestVert> gbuff;

// This is for the BIND_ macros, to determine type, it never actually
// gets used or instantiated anywhere. Workaround for issue described at
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2253.html
#define MAKE_BINDABLE(VertType) \
    static VertType _example

#define BIND_VERTEX( VertType, member ) \
    glEnableClientState( GL_VERTEX_ARRAY );    \
    glVertexPointer( sizeof( VertType ::_example. member) /sizeof(float), \
                     GL_FLOAT, sizeof( VertType ),                      \
                     (GLvoid*)offsetof( VertType , member ) )

#define BIND_TEXTURE_COORD( VertType, member ) \
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );  \
    glTexCoordPointer( sizeof( VertType:: _example. member ) /sizeof(float),\
                       GL_FLOAT, sizeof( VertType ),                    \
                           (GLvoid*)offsetof( VertType, member ) )

// TODO: color, etc..

#endif
