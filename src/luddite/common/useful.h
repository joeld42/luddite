#ifndef USEFUL_H
#define USEFUL_H

#include <GLKit/GLKMath.h>

// ==============================
//  Common Types
// ==============================

typedef double TimeInterval;

// ==============================
//  Random numbers
// ==============================

// Generates a uniform random number between 0 and 1
float randUniform();

// Generates a uniform random number between minVal and maxVal
float randUniform( float minVal, float maxVal );

// Generates a random number from a normal (gaussian) distribution
// with a mean of 0 and a std deviation of 1
// (Box-Muller transform)
// NOTE: not threadsafe
float randNormal();

// Generates a random number from a normal distribution
// with the given mean and stddev
//
// NOTE: not threadsafe
float randNormal( float mean, float stddev );

// ==============================
//  general mathy stuff
// ==============================
float sgn( float n );
float clamp( float x, float a, float b); // clamp x to range [a..b]
float saturate( float x ); // clamp x to [0..1]
float lerp( float a, float b, float t);
float smoothstep(float edge0, float edge1, float x); 
float smootherstep(float edge0, float edge1, float x);

void printMatrix( const char *label, const GLKMatrix4 mat  );


#ifdef WIN32
#define M_PI (3.1415926535897932384626433832795)
#endif

#define DEG2RAD (M_PI/180.0)
#define RAD2DEG (180.0/M_PI)

// ==============================
//  GL error checking
// ==============================

int checkForGLErrors( const char *s, const char * file, int line );
void checkFBO();

#ifdef NDEBUG
#define CHECKGL( msg )
#else
#define CHECKGL( msg ) cZZheckForGLErrors( msg, __FILE__, __LINE__ );
#endif

// ==============================
//  GL camera math
// ==============================
#if 0
void glhFrustumf2(matrix4x4f &matrix, 
                  float left, float right, float bottom, float top,
                  float znear, float zfar);

void glhPerspectivef2(matrix4x4f &matrix, float fovyInDegrees, float aspectRatio,
                      float znear, float zfar);
#endif


#endif

