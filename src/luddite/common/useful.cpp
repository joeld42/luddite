#include <stdlib.h>
#include <math.h>

#include <luddite/common/useful.h>

#include <GLKit/GLKMath.h>

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GL/gl.h>
#endif

using namespace luddite;

float luddite::randUniform()
{
#ifndef WIN32
    return drand48();
#else
	return ((float)rand() / (float)RAND_MAX);
#endif
}

float luddite::randUniform( float minVal, float maxVal )
{
    return minVal + (randUniform() * (maxVal-minVal));    
}

float luddite::randNormal()
{
    static float y1, y2;
    
    // This generates 2 numbers at a time, so 
    // alternate between generating a new one and
    // just returning the last one
    static bool doGen = true;
    if (!doGen)
    {
        // return the other number we generated
        // last time
        return y2;        
    }
    else
    {        
        float x1, x2, w;

        // reject points outside the unit circle
        do {
            x1 = (2.0f * randUniform()) - 1.0;
            x2 = (2.0f * randUniform()) - 1.0;
            w = x1*x1 + x2*x2;        
        } while (w >= 1.0 );
        
        w = sqrt( (-2.0 * logf(w)) / w );
        y1 = x1 * w;
        y2 = x2 * w;

        return y1;
    }
}

float luddite::randNormal( float mean, float stddev )
{
    return (randNormal() * stddev) + mean;
}

#if 0

// From http://www.opengl.org/wiki/GluPerspective_code

void glhFrustumf2(matrix4x4f &matrix, 
                  float left, float right, float bottom, float top,
                  float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0][0] = temp / temp2;
    matrix[0][1] = 0.0;
    matrix[0][2] = 0.0;
    matrix[0][3] = 0.0;
    matrix[1][0] = 0.0;
    matrix[1][1] = temp / temp3;
    matrix[1][2] = 0.0;
    matrix[1][3] = 0.0;
    matrix[2][0] = (right + left) / temp2;
    matrix[2][1] = (top + bottom) / temp3;
    matrix[2][2] = (-zfar - znear) / temp4;
    matrix[2][3] = -1.0;
    matrix[3][0] = 0.0;
    matrix[3][1] = 0.0;
    matrix[3][2] = (-temp * zfar) / temp4;
    matrix[3][3] = 0.0;
}

//matrix will receive the calculated perspective matrix.
//You would have to upload to your shader
// or use glLoadMatrixf if you aren't using shaders.
void glhPerspectivef2(matrix4x4f &matrix, float fovyInDegrees, float aspectRatio,
                      float znear, float zfar)
{
    float ymax, xmax;
//    float temp, temp2, temp3, temp4;
    ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
    //ymin = -ymax;
    //xmin = -ymax * aspectRatio;
    
    xmax = ymax * aspectRatio;
    glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

#endif

float luddite::sgn( float n )
{
    return (n<0)?-1.0:1.0;
}

float luddite::clamp( float x, float a, float b)
{
    if (x < a) return a;
    else if (x > b) return b;
    else return x;
}

float luddite::saturate( float x )
{
    if (x < 0.0) return 0.0;
    else if (x > 1.0) return 1.0;
    else return x;
}

float luddite::lerp( float a, float b, float t)
{
    return (a*(1.0-t)) + (b*t);
}

float luddite::smoothstep(float edge0, float edge1, float x)
{
    // Scale, bias and saturate x to 0..1 range
    x = saturate((x - edge0)/(edge1 - edge0));
    // Evaluate polynomial
    return x*x*(3 - 2*x);
}

float luddite::smootherstep(float edge0, float edge1, float x)
{
    // Scale, and clamp x to 0..1 range
    x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*x*(x*(x*6 - 15) + 10);
}

void luddite::printMatrix( const char *label, const GLKMatrix4 mat )
{
    const float *m = mat.m;
    printf("%10s: %3.2f %3.2f %3.2f %3.2f\n"
           "            %3.2f %3.2f %3.2f %3.2f\n"
           "            %3.2f %3.2f %3.2f %3.2f\n"
           "            %3.2f %3.2f %3.2f %3.2f\n",
           label,
           m[0], m[1], m[2], m[3],
           m[4], m[5], m[6], m[7],
           m[8], m[9], m[10], m[11],
           m[12], m[13], m[14], m[15] );
}

// ===========================================================================
// Error Checkings
int luddite::checkForGLErrors( const char *s, const char * file, int line )
{
    int errors = 0 ;
    int counter = 0 ;
    
    while ( counter < 1000 )
    {
        GLenum x = glGetError() ;
        
        if ( x == GL_NO_ERROR )
            return errors ;
        
//        printf( "%s:%d [%s] OpenGL error: %s [%08x]\n",  file, line, s ? s : "", gluErrorString ( x ), x ) ;
        printf( "%s:%d [%s] OpenGL error: %08x\n",  file, line, s ? s : "",  x ) ;
        errors++ ;
        counter++ ;
    }
	return 0;
}

void luddite::checkFBO()
{
	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if (status != GL_FRAMEBUFFER_COMPLETE )
	{
		const char *errStr;
		switch (status)
		{
//            case GL_FRAMEBUFFER_UNDEFINED: errStr = "GL_FRAMEBUFFER_UNDEFINED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: errStr = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: errStr = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
//    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: errStr = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
//            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: errStr = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: errStr = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_APPLE: errStr = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
            default: errStr = "UNKNOWN_ERROR"; break;
		}
		printf( "Bad framebuffer status: [0x%08X] %s\n", status, errStr );
	}
	else
	{
		printf( "glCheckFramebufferStatus good: GL_FRAMEBUFFER_COMPLETE\n" );
	}
}




