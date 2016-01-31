#ifndef DEBUG_H
#define DEBUG_H

#include <luddite/platform/platform.h>

namespace luddite {
namespace DBG {

//---[ Console Output ]------------------------------------

// Verbose level is like mental ray
// 0 - silent
// 1 - fatal errors only
// 2 - errors  (same as -v "off" )
// 3 - warnings
// 4 - progress 
// 5 - info  (same as -v "on" )
// 6 - debug
// 7 - verbose debug

extern const int kVerbose_Off; // 2
extern const int kVerbose_On;  // 5
extern const int kVerbose_Dbg; // 6

extern int g_verbose_level;
extern bool g_colorterm;

    
// output messages
void fatal    ( const char *fmt, ... );
void error    ( const char *fmt, ... );
void warn     ( const char *fmt, ... );
void progress ( const char *fmt, ... );
void info     ( const char *fmt, ... );
void debug    ( const char *fmt, ... );
void vdebug   ( const char *fmt, ... );

//---[ Debug Text Output ]---------------------------

// "console style" debug text on screen
void Printf( const char *fmt, ... );

// 3D debug text
void Printf( float x, float y, float z, const char *fmt, ... );

// 2D debug text, x,y are in screen space
void Printf( int x, int y, const char *fmt, ... );

// Used to implement this text, call from gameshell
void _dbgtext_NewFrame();
void _dbgtext_FinishFrame2D();
void _dbgtext_GetProjection();

  }; // namespace DBG
}; // namespace luddite



#endif

