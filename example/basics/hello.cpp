#include <stdio.h>

// EASTL
#include <EASTL/string.h>

// Luddite 
#include <luddite/common/debug.h>

using namespace luddite;

// ---------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    eastl::string s("hello");


    DBG::info( "Hello luddite\n" );    
    DBG::warn( "Warning... %d bottle of beer left\n", 99 );
    
    return 1;
}





