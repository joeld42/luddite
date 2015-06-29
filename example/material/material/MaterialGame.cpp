//
//  MaterialGame.cpp
//  material
//
//  Created by Joel Davis on 6/28/15.
//  Copyright (c) 2015 Joel Davis. All rights reserved.
//

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#include "MaterialGame.h"

#include <luddite/render/scene_objfile.h>
#include <luddite/render/gbuff_prim.h>
#include <luddite/render/texture_info.h>
#include <luddite/render/color_util.h>

MaterialGame::MaterialGame()
{
    
}

void MaterialGame::setupScene()
{
    // Set up luddite stuff
    luddite::RenderDeviceGL *renderDeviceGL = new luddite::RenderDeviceGL();
    m_renderDevice = renderDeviceGL;
    
    // just make sure viewport is set, game will set it properly later
    m_viewport = CGSizeMake( 100, 100 );
    
    // Initialize shader DB
    m_mtlDB = new luddite::MaterialDB( );
    m_mtlDB->initShaderDB();

    // Add material def files
    m_mtlDB->addMaterialDefs("basic.material.xml" );
    
//    luddite::GBuff *gbuffCube = luddite::gbuff_cube( 0.7, GLKVector3Make( 0.0, 0.5, 0.0) );
//    gbuff_setColorConstant( gbuffCube, GLKVector4Make( 1.0, 0.0, 1.0, 1.0) );
    m_mtlDemoSphere = scene_objfile_named( "mtl_test_sph.obj", renderDeviceGL, m_mtlDB );
    assert( m_mtlDemoSphere );
    
    
    
    // Make a material
    luddite::Material *mtl  = m_mtlDB->getNamedMaterial( renderDeviceGL, "mtl.outer" );
    
    // Build scene graph
    m_worldRoot = new luddite::SceneNode( "worldRoot" );

    // Make a 5x5 grid of mtl spheres
    for (int j=0; j < 5; j++)
    {
        for (int i=0; i < 5; i++)
        {
            GLKVector3 shapePos = GLKVector3Make( (float)i*3.0 -7.5,  0.0, (float)j*3.0-7.5 );
            luddite::SceneNode *shapeNode;
            shapeNode = new luddite::SceneNode( m_worldRoot );
            shapeNode->m_pos = shapePos;
            
            for (auto srcBatch : m_mtlDemoSphere->batches() )
            {
                luddite::GBatch *currBatch = new luddite::GBatch();
                currBatch->m_gbuff = srcBatch->m_gbuff;
                
                if (srcBatch->m_mtl->m_materialName=="mtl.inner")
                {
                    // inner mtl doesn't change, just reference it directly
                    currBatch->m_mtl = srcBatch->m_mtl;
                }
                else
                {
                    // Customize outer mtl per instance
                    currBatch->m_mtl = new luddite::Material( *mtl );
                    
                    GLKVector3 meshColor = hsv2rgb( GLKVector3Make( i * (180.0/M_PI), 1.0, 1.0 ));
                    currBatch->m_mtl->param( "flatColor") = GLKVector4Make( meshColor.r, meshColor.g, meshColor.b, 1.0 );
                    

                }
                
                    
                shapeNode->addGBatch( currBatch );
            }
        }
    }
    
//    m_worldRoot->addChild( m_mtlDemoSphere );
    
    
    // FIXME.. this is temporary, shouldn't need to do this (and we shouldn't
    // upload all the shaders, only used ones)
    m_mtlDB->useAllShaders( m_renderDevice );
    
    // Finally, create scene for our node graph
    m_scene = new luddite::Scene( m_worldRoot );

}

void MaterialGame::updateFixed( double dt )
{
    
}

void MaterialGame::updateDynamic(double dt)
{
    
}

void MaterialGame::drawScene()
{
    glClearColor( 0.2f, 0.2f, 0.25f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Eval and render the scene
    m_scene->eval( m_renderDevice );
    m_renderDevice->renderFrame();

}

//- (void)setViewport:(CGSize)viewport
void MaterialGame::setViewport( CGSize viewport )
{
    luddite::RenderDeviceGL *renderDeviceGL = dynamic_cast<luddite::RenderDeviceGL*>(m_renderDevice);
    m_viewport = viewport;
    
    // Setup camera (TODO: do this differently)
    //    glhPerspectivef2( _renderDevice->matProjection, 20.0, viewport.width / viewport.height, 1.0, 500.0 );
    renderDeviceGL->matProjection = GLKMatrix4MakePerspective( 50.0 * DEG2RAD,
                                                              viewport.width/viewport.height,
                                                              1.0, 500.0);
    
    GLKMatrix4 cameraXlate, cameraRot;
    cameraXlate = GLKMatrix4MakeTranslation( 0.0, -3.0, -13.0 );
    //cameraXlate = GLKMatrix4MakeTranslation( 0.0, 0.0, _zval );
    cameraRot = GLKMatrix4RotateX( cameraXlate, 35.0*DEG2RAD );
    
    renderDeviceGL->matBaseModelView = cameraXlate;
    renderDeviceGL->matBaseModelView = GLKMatrix4Multiply( cameraXlate, cameraRot );
    
}
