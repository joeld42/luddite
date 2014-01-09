//
// Created by joeld on 4/7/13.
//
// To change the template use AppCode | Preferences | File Templates.
//



#ifndef __scene_collect_H_
#define __scene_collect_H_

#include <iostream>
#include "test_scene.h"

#include <luddite/render/render_device.h>
#include <luddite/render/material_db.h>
#include <luddite/render/scene.h>


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

protected:
    luddite::RenderDevice *m_renderDevice;
    luddite::MaterialDB *m_mtlDB;
    luddite::Scene *m_scene;

    luddite::SceneNode *m_worldRoot;
    luddite::SceneNode *m_groundPlane;
    luddite::SceneNode *m_player;
    
    eastl::vector<luddite::SceneNode*> m_trees;
};

}

#endif //__scene_collect_H_
