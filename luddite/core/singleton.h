#ifndef SINGLETON_H
#define SINGLETON_H

// =======================================================
// Simple singleton class
//
// Based on Scott Bilas's Singleton from Game Programming Gems:
// http://scottbilas.com/publications/gem-singleton/
//
// Usage: 
//  class FooManager : public Singleton<FooManager>
//  {
//      ...
//  }
//
// =======================================================

#include <luddite/core/debug.h>

namespace luddite
{
 
template <typename T> class Singleton
{
    static T* ms_singleton;
 
public:
    Singleton()
    {
        Assert(!ms_singleton, "singleton already constructed." );
        ms_singleton = static_cast<T*>(this);        
    }

    ~Singleton()
    { 
        Assert(ms_singleton, "singleton already destroyed."); 
        ms_singleton = 0; 
    }

    static T& singleton()
    { 
        Assert(ms_singleton, "singleton not yet constructed." ); 
        return *ms_singleton; 
    }

    static T* singletonPtr()
    { 
        return ms_singleton; 
    }
};
 
template <typename T> T* Singleton <T>::ms_singleton = 0;

}; // namespace luddite



#endif 
