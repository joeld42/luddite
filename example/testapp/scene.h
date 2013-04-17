//
// Created by joeld on 4/7/13.
//
// To change the template use AppCode | Preferences | File Templates.
//



#ifndef __scene_H_
#define __scene_H_

#include <iostream>

#include <EASTL/string.h>

namespace TestApp
{

// Test "scene"
class Scene {
    public:
    virtual eastl::string sceneName()=0;

    // Scene stuff
    virtual void init();
    virtual void shutdown();
    virtual void updateFixed( float dt );
    virtual void updateDynamic( float dt );
    virtual void render();

};

};

#endif //__scene_H_
