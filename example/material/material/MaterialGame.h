//
//  MaterialGame.h
//  material
//
//  Created by Joel Davis on 6/28/15.
//  Copyright (c) 2015 Joel Davis. All rights reserved.
//

#ifndef __material__MaterialGame__
#define __material__MaterialGame__

// for CGSize
#include <CoreGraphics/CoreGraphics.h>

#include <luddite/common/useful.h>

#include <luddite/render/render_device.h>
#include <luddite/render/render_device_gl.h>
#include <luddite/render/material_db.h>
#include <luddite/render/scene_node.h>
#include <luddite/render/scene.h>


class MaterialGame
{
public:
    MaterialGame();
    
    void setupScene();
    void setViewport( CGSize viewport );
    void updateFixed( double dt );
    void updateDynamic(double dt);
    void drawScene();

private:
    
    luddite::RenderDevice *m_renderDevice;
    luddite::MaterialDB *m_mtlDB;
    
    luddite::SceneNode *m_worldRoot;
    luddite::SceneNode *m_mtlDemoSphere;
//    luddite::Material *m_planetMtl;
    
    luddite::Scene *m_scene;
    
    CGSize m_viewport;
    
};

#endif /* defined(__material__MaterialGame__) */
