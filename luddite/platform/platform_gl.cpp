#include <stdio.h>

#include <luddite/core/debug.h>

#include <luddite/platform/gl.h>

using namespace luddite;

namespace luddite
{
    

// ===========================================================================
// Error Checkings
int checkForGLErrors( const char *s, const char * file, int line )
 {
    int errors = 0 ;
    int counter = 0 ;

    while ( counter < 1000 )
    {
      GLenum x = glGetError() ;

      if ( x == GL_NO_ERROR )
        return errors ;

      const GLubyte *err = gluErrorString( x );
      printf("err string is %p\n", err );      
      printf("error %s\n", err );      

	  DBG::error( "%s:%d OpenGL error: %s; %s [%d]\n", 
                  file, line,
                  s ? s : "", gluErrorString ( x ), x ) ;
      
      Assert( 0, "gl error has occured." );      
      

      errors++ ;
      counter++ ;
    }
	return 0;
}

// ===========================================================================
void checkFBO()
{
	GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT )
	{
		const char *errStr;
		switch (status)
		{
		case GL_FRAMEBUFFER_UNDEFINED: errStr = "GL_FRAMEBUFFER_UNDEFINED"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: errStr = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: errStr = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: errStr = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: errStr = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
		case GL_FRAMEBUFFER_UNSUPPORTED: errStr = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: errStr = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;		
		default: errStr = "UNKNOWN_ERROR"; break;
		}
		DBG::error( "Bad framebuffer status: [0x%08X] %s\n", status, errStr );
	}
	else
	{
		DBG::info( "glCheckFramebufferStatus good: GL_FRAMEBUFFER_COMPLETE" );
	}
}

}; // namespace luddite

