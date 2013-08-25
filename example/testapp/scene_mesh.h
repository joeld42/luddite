
#ifndef __scene_mesh_H_
#define __scene_mesh_H_

#include <iostream>
#include "test_scene.h"

#include <luddite/render/render_device.h>
#include <luddite/render/material_db.h>
#include <luddite/render/scene.h>

namespace TestApp
{

class SceneMesh : public TestApp::TestScene
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
};

} // namespace TestApp

#endif //__scene_mesh_H_
