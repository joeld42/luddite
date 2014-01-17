//
//  TKCollectGame.m
//  CollectGame
//
//  Created by Joel Davis on 1/14/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import "TKCollectGame.h"

#include <luddite/common/useful.h>
#include <luddite/render/render_device_gl.h>
#include <luddite/render/material_db.h>
#include <luddite/render/scene_node.h>
#include <luddite/render/scene.h>
#include <luddite/render/scene_objfile.h>
#include <luddite/render/gbuff_prim.h>
#include <luddite/render/texture_info.h>
#include <luddite/render/color_util.h>


@interface TKCollectGame ()
{
    luddite::RenderDeviceGL *_renderDevice;
    luddite::MaterialDB *_mtlDB;
    
    std::vector<luddite::SceneNode*> _trees;
    luddite::SceneNode *_worldRoot;
    luddite::SceneNode *_groundPlane, *_player;
    
    luddite::Scene *_scene;
}
@end

@implementation TKCollectGame

@synthesize viewport=_viewport;

- (id)init
{
    self = [super init];
    if (self)
    {
        // Set up luddite stuff
        luddite::RenderDeviceGL *renderDevice = new luddite::RenderDeviceGL();
        _renderDevice = renderDevice;
        
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
    _mtlDB->addMaterialDefs("CollectGame.material.xml" );
    
    luddite::Material *mtl  = _mtlDB->getNamedMaterial( _renderDevice, "mtl.environment" );
    
    // Build scene graph
    _worldRoot = new luddite::SceneNode( "worldRoot" );
    
    // Load the ground plane obj file in the center.
    _groundPlane = scene_objfile_named( "grid10x10.obj", _renderDevice, _mtlDB );
    _worldRoot->addChild( _groundPlane );
    
    // Load the player mesh
    _player = scene_objfile_named( "player.obj", _renderDevice, _mtlDB );
    _worldRoot->addChild( _player );
    
    // Load the trees
    luddite::SceneNode *tree = scene_objfile_named("tree_fir.obj", _renderDevice, _mtlDB );
    tree->m_pos = vec3f( 2.0, 0.0, 1.5 );
    _worldRoot->addChild( tree );
    
    _trees.push_back(tree);
    
    // Add tree instances
    for (int i=0; i <20; i++)
    {
        luddite::SceneNode *treeInst = tree->makeInstance();
        
        // Generate a random position for the tree, outside the center of the
        // map (TODO: also don't overlap other trees)
        do {
            vec3f treePos = vec3f( randUniform(-10.0, 10.0), 0.0, randUniform(-10.0, 10.0) );
            treeInst->m_pos = treePos;
        } while (prmath::LengthSquared(treeInst->m_pos) < 1.0 );
        
        _worldRoot->addChild( treeInst );
        _trees.push_back( treeInst );
    }
    
    // FIXME.. this is temporary, shouldn't need to do this (and we shouldn't
    // upload all the shaders, only used ones)
    _mtlDB->useAllShaders( _renderDevice );
    
    // Finally, create scene for our node graph
    _scene = new luddite::Scene( _worldRoot );
    
}

- (void) updateFixed: (double)dt
{
}

- (void) updateDynamic: (double)dt
{
    // no dynamic stuff...
    //    NSLog( @"MeshGameDelegate: updateDynamic: %lf", dt );
}

- (void) drawScene
{
    glClearColor( 0.64f, 0.79f, 0.67f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Eval and render the scene
    _scene->eval( _renderDevice );
    _renderDevice->renderFrame();
}

- (void)setViewport:(CGSize)viewport
{
    _viewport = viewport;
    
    // Setup camera (TODO: do this differently)
    glhPerspectivef2( _renderDevice->matProjection, 20.0, viewport.width / viewport.height, 1.0, 500.0 );
    matrix4x4f cameraXlate, cameraRot;
    cameraXlate.Translate(0.0, -4, -15.0);
    cameraRot.RotateX( 15.0 * (M_PI/180.0) );
    _renderDevice->matBaseModelView = cameraXlate * cameraRot;
}

@end
