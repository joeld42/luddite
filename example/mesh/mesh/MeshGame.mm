//
//  MeshGameDelegate.m
//  mesh
//
//  Created by Joel Davis on 1/9/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import "MeshGame.h"

#include <luddite/common/useful.h>

#include <luddite/render/render_device_gl.h>
#include <luddite/render/material_db.h>
#include <luddite/render/scene_node.h>
#include <luddite/render/scene.h>
#include <luddite/render/scene_objfile.h>
#include <luddite/render/gbuff_prim.h>
#include <luddite/render/texture_info.h>
#include <luddite/render/color_util.h>


@interface MeshGame ()
{
    luddite::RenderDeviceGL *_renderDeviceGL;
    luddite::MaterialDB *_mtlDB;

    std::vector<luddite::SceneNode*> _meshNodes;
    luddite::SceneNode *_worldRoot;
    luddite::SceneNode *_spinnyNode, *_monkeyNode;
    
    luddite::Scene *_scene;    
}
@end

@implementation MeshGame

@synthesize viewport=_viewport;

- (id)init
{
    self = [super init];
    if (self)
    {
        // Set up luddite stuff
        luddite::RenderDeviceGL *renderDeviceGL = new luddite::RenderDeviceGL();
        _renderDeviceGL = renderDeviceGL;
        
        [self setupMeshScene];
    }
    return self;
}

- (void)setupMeshScene
{
   
    // just init with a default viewport, game can set it later
    self.viewport = CGSizeMake( 100.0, 100.0 );
    
    // Initialize shader DB
    _mtlDB = new luddite::MaterialDB( );
    _mtlDB->initShaderDB();
    
    // Add material def files
    _mtlDB->addMaterialDefs("example_mesh.material.xml" );
    
    luddite::GBuff *gbuffCube = luddite::gbuff_cube( 0.7, GLKVector3Make( 0.0, 0.5, 0.0) );
    gbuff_setColorConstant( gbuffCube, GLKVector4Make( 1.0, 0.0, 1.0, 1.0) );
    
    luddite::GBuff *gbuffCyl = luddite::gbuff_cylinder();
    gbuff_setColorConstant( gbuffCyl, GLKVector4Make( 0.0, 1.0, 0.0, 1.0) );
    
    // Build scene graph
    _worldRoot = new luddite::SceneNode( "worldRoot" );
    
    // Make a material
    luddite::Material *mtl  = _mtlDB->getNamedMaterial( _renderDeviceGL, "mtl.one" );
    luddite::Material *mtl3 = _mtlDB->getNamedMaterial( _renderDeviceGL, "mtl.three" );
    
    // empty node to spin the scene
    _spinnyNode = new luddite::SceneNode( _worldRoot );
    
    // Load the monkey obj file in the center. This obj has 3 different materials,
    // each will be a separate gbatch
    _monkeyNode = scene_objfile_named( "suzanne.obj", _renderDeviceGL, _mtlDB );
    if (_monkeyNode)
    {
        _monkeyNode->m_rot = GLKQuaternionMakeWithAngleAndAxis(20.0*DEG2RAD, 1.0, 0.0, 0.0);
        _spinnyNode->addChild( _monkeyNode );
    }
    
    // instance a ring of shapes around the monkey
    bool cube = true;
    for (float t=0.0; t <= 2.0*M_PI; t += 20.0 * (M_PI/180.0 ) )
    {
        // Make a node for this shape
        GLKVector3 shapePos = GLKVector3Make( cos(t)*3.0, 0.3, sin(t)*3.0 );
        luddite::SceneNode *shapeNode;
        shapeNode = new luddite::SceneNode( _spinnyNode );
        shapeNode->m_pos = shapePos;
        
        // bind gbuff to a new gbatch, attach that to scene node
        luddite::GBatch *currBatch = new luddite::GBatch();
        currBatch->m_mtl = NULL;
        if (cube)
        {
            currBatch->m_gbuff = gbuffCube;
            currBatch->m_mtl = new luddite::Material( *mtl );
        }
        else
        {
            // alternate shapes are a cylinder in a wavy pattern
            shapeNode->m_pos.y = sin(t*3) * 0.25;
            currBatch->m_gbuff = gbuffCyl;
            currBatch->m_mtl = new luddite::Material( *mtl3 );
        }
        cube = !cube;
        
        // Set the material color
        GLKVector3 meshColor = hsv2rgb( GLKVector3Make( t * (180.0/M_PI), 1.0, 1.0 ));
        currBatch->m_mtl->param( "dbgColor") = GLKVector4Make( meshColor.r, meshColor.g, meshColor.b, 1.0 );
        
        // Add this as a batch to the node
        shapeNode->addGBatch( currBatch );
        _meshNodes.push_back(shapeNode);
    }
    
    
    // FIXME.. this is temporary, shouldn't need to do this (and we shouldn't
    // upload all the shaders, only used ones)
    _mtlDB->useAllShaders( _renderDeviceGL );
    
    // Finally, create scene for our node graph
    _scene = new luddite::Scene( _worldRoot );
    
    // Init zval
    _zval = -20.0;

}

- (void) updateFixed: (double)dt
{
//    quat4f qrot;
    GLKQuaternion qrot = GLKQuaternionMakeWithAngleAndAxis(0.2f*dt, 0.0, 1.0, 0.0 );
    
    // update the spinny node
    _spinnyNode->m_rot = GLKQuaternionMultiply( _spinnyNode->m_rot, qrot );
    
    // spin the meshes in the opposite direction
    qrot = GLKQuaternionMakeWithAngleAndAxis(-0.3f*dt, 0.0, 1.0, 0.0 );
    for (auto mesh : _meshNodes)
    {
        mesh->m_rot = GLKQuaternionMultiply( mesh->m_rot, qrot);
//        mesh->m_rot *= qrot;
    }

}

- (void) updateDynamic: (double)dt
{
    // no dynamic stuff...
//    NSLog( @"MeshGameDelegate: updateDynamic: %lf", dt );
}

- (void) drawScene
{
    glClearColor( 0.2f, 0.2f, 0.25f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Eval and render the scene
    _scene->eval( _renderDeviceGL );
    _renderDeviceGL->renderFrame();
}

- (void)setViewport:(CGSize)viewport
{
    _viewport = viewport;
    
    // Setup camera (TODO: do this differently)
    //    glhPerspectivef2( _renderDevice->matProjection, 20.0, viewport.width / viewport.height, 1.0, 500.0 );
    _renderDeviceGL->matProjection = GLKMatrix4MakePerspective( 20.0 * DEG2RAD,
                                                             viewport.width/viewport.height,
                                                             1.0, 500.0);
    
    GLKMatrix4 cameraXlate, cameraRot;
//    cameraXlate = GLKMatrix4MakeTranslation( 0.0, -4.0, -15.0 );
    cameraXlate = GLKMatrix4MakeTranslation( 0.0, 0.0, _zval );
    cameraRot = GLKMatrix4RotateX( cameraXlate, 15.0*DEG2RAD );

    _renderDeviceGL->matBaseModelView = cameraXlate;
    _renderDeviceGL->matBaseModelView = GLKMatrix4Multiply( cameraXlate, cameraRot );

}

- (void)setZval:(CGFloat)zval
{
    _zval = zval;
    NSLog( @"zval is %f", _zval );
    [self setViewport: _viewport];
}

@end
