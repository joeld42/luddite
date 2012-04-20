#include <stdio.h>
#include <stdlib.h>

#include <luddite/core/debug.h>
#include <luddite/core/handle.h>
#include <luddite/core/singleton.h>

#include <EASTL/string.h>

struct Waffle;
bool loadResource( const char *filename, Waffle *item );
#include <luddite/core/resource.h>

// ---------------------------------------
// A silly dummy resource to test with

struct Waffle
{
    eastl::string m_toppings;    
};

// typedefs to use the resource stuff
    
struct tag_WAFFLE;
typedef luddite::Handle<tag_WAFFLE> HWaffle;


typedef luddite::HandleMgr<Waffle, HWaffle> HWaffleMgr;

// ---------------------------------------
// Resource stuff

typedef luddite::ResourceMgr<Waffle, HWaffle> BaseWaffleDB;
class WaffleDB : public BaseWaffleDB, public luddite::Singleton<WaffleDB>
{
public:
    HWaffle getWaffle( const char *name );
    void freeWaffle( HWaffle hWaf );
};

HWaffle WaffleDB::getWaffle( const char *name )
{
    return BaseWaffleDB::getResource( name );
}

void WaffleDB::freeWaffle( HWaffle hw )
{
    return BaseWaffleDB::freeResource( hw );    
}


bool loadResource( const char *filename, Waffle *waffle )
{
    printf("Loaded waffle \"%s\".\n", filename );
    waffle->m_toppings = filename;    
    return true;    
}

bool unloadResource( Waffle *waffle )
{
    printf("Free waffle \"%s\".\n", waffle->m_toppings.c_str() );    
}

// ---------------------------------------
// Global resources
WaffleDB g_waffleDB;

// ---------------------------------------

int main( int argc, char *argv[] )
{
    luddite::DBG::info( "hello luddite\n" );

    WaffleDB &wdb = WaffleDB::singleton();
    
    HWaffle hw  = wdb.getWaffle( "pecan" );
    HWaffle hw2 = wdb.getWaffle( "blueberry" );
    HWaffle hw3 = wdb.getWaffle( "plain" );

    wdb.dbgPrintUsage();    

    // release some, get some more
    HWaffle hw4 = wdb.getWaffle( "blueberry" );
    wdb.freeWaffle( hw3 );

    wdb.dbgPrintUsage();

    printf( "Unloading unused...\n" );
    wdb.unloadUnusedResources();    
    wdb.dbgPrintUsage();    


    // reuse the resource
    HWaffle hw5 = wdb.getWaffle( "plain" );
    wdb.dbgPrintUsage();    
}




