#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>

#ifdef WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <conio.h>

#include <dbghelp.h>
#pragma comment(linker, "/defaultlib:dbghelp.lib")

#else

// Linux 
#include <termios.h>
#include <unistd.h>

#include <execinfo.h>

#endif

//#include <SDL.h>
//#include <SDL_endian.h>

#include "debug.h"

using namespace luddite;
using namespace DBG;

// globals;
int  DBG::g_verbose_level = DBG::kVerbose_On;
bool DBG::g_colorterm = true;

const int DBG::kVerbose_Off = 2;
const int DBG::kVerbose_On  = 5;
const int DBG::kVerbose_Dbg = 6;


#define MAX_SYMBOL_NAME (1024)

/////////////////////////////////////////////////////////
void vmessage( int message_level, const char *fmt, va_list args) {

	if (message_level <= g_verbose_level ) 
    {
		pfVMessage( message_level, fmt, args);  
    }
    
        // non-fatal error
	if (message_level > 1) return;
	
#ifndef NDEBUG
	Assert( 0, "Fatal Error" ); // break into debugger
#else
	exit(1); // quit
#endif
}

#define def_output_fn( fname, level ) \
	void DBG::fname( const char *fmt, ... ) { \
		va_list args; \
		va_start(args,fmt);  \
		vmessage( level, fmt, args); \
		va_end(args); }

def_output_fn( fatal,    1 );
def_output_fn( error,    2 );
def_output_fn( warn,     3 );
def_output_fn( progress, 4 );
def_output_fn( info,     5 );
def_output_fn( debug,    6 );
def_output_fn( vdebug,   7 );

#undef def_output_fn

