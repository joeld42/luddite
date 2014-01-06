#include <stdio.h>
#include <stdlib.h>

#include <dispatch/dispatch.h>

bool pfAssertFunc( bool expr, const char *desc, int line, const char *file, bool *skip )
{
    // FIXME
    return true;
}

void pfVMessage( int message_level, const char *fmt, va_list args)
{

    static dispatch_once_t pred;
    static bool colorterm = false;
    
    dispatch_once(&pred, ^{
        char *term = getenv( "TERM" );
        if (term) colorterm = true;
    });
    
    static const char *catname[8] = {
		"", "fatal: ", "error: ", "warn: ", 
		"progress: ", "info: ", "debug: ", "vdebug: "
	};
	
	static const char *catcolor[8] = {
		"\e[0m", // reset
		"\e[1m\e[31m", // fatal - bright red
		"\e[0m\e[31m", // error - red
		"\e[1m\e[33m", // warn - bright yellow
		"\e[1m\e[37m", // info - bright white
		"\e[1m\e[36m", // progress - bright cyan
		"\e[1m\e[44m", // debug - bright blue
		"\e[1m\e[44m"  // vdebug - bright blue
	};
    
    if (colorterm) puts( catcolor[message_level] );

    //output message
    printf( "%s", catname[message_level]);
    vprintf(fmt, args);  
    
    // reset color
    if (colorterm) printf("\e[0m" );
}
