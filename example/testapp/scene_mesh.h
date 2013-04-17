
#ifndef __scene_mesh_H_
#define __scene_mesh_H_

#include <iostream>
#import "scene.h"

#import <luddite/render/render_device.h>
#import <luddite/render/material_db.h>

namespace TestApp
{

class SceneMesh : public TestApp::Scene
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
};

} // namespace TestApp

#endif //__scene_mesh_H_
