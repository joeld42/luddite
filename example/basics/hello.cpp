#include <stdio.h>

// EASTL
#include <EASTL/string.h>

// Luddite includes
#include <luddite/core/debug.h>
#include <luddite/core/handle.h>

// -----------------------------
struct tag_INT;
typedef luddite::Handle<tag_INT> HInt;
typedef luddite::HandleMgr<int, HInt> HIntMgr;


using namespace luddite;


int main( int argc, char *argv[] )
{
    HInt hTestInt;
    eastl::string s("hello");

    DBG::info("s is %s\n", s.c_str() );    

    //printf( "Hello luddite.\n" );    
    DBG::info( "Hello luddite, hint isNull %s\n",
               hTestInt.isNull()?"true":"false" );    

    return 1;
}





