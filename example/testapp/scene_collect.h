//
// Created by joeld on 4/7/13.
//
// To change the template use AppCode | Preferences | File Templates.
//



#ifndef __scene_collect_H_
#define __scene_collect_H_

#include <iostream>
#include "test_scene.h"

namespace TestApp
{

class SceneCollect : public TestApp::TestScene
{
public:
    virtual eastl::string sceneName();

    virtual void init();
//    virtual void shutdown();
//    virtual void updateFixed( float dt );
//    virtual void updateDynamic( float dt );
    virtual void render();


};

}

#endif //__scene_collect_H_
