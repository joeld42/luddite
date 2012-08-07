//
//  scene.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/7/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <luddite/render/scene.h>

using namespace luddite;

Scene::Scene( luddite::SceneNode *root ) :
    m_sceneRoot( root )
{
    
}

// Scene Root
void Scene::setRoot( SceneNode *root )
{
    m_sceneRoot = root;
}

SceneNode *Scene::root()
{
    return m_sceneRoot;
}

// Evaluate scene and add visible nodes
// to the RenderDevice for this frame
void Scene::eval( RenderDevice *device )
{
    // TODO: use visibilty for traversal..
    matrix4x4f rootXform;
    rootXform.Identity();
    _evalNode( device, m_sceneRoot, rootXform );
}

void Scene::_evalNode( RenderDevice *device, SceneNode *node, matrix4x4f currXform )
{
    // update the transform for this batch
    matrix4x4f nodeXform = node->localXForm();
    nodeXform = nodeXform * currXform;
    
    // Add the gbatch for this node
    // For now just add everything, need to be smarter about traversal
    for ( GBatchList::const_iterator gbi = node->batches().begin();
         gbi != node->batches().end(); ++gbi )
    {
        GBatch *gbatch = *gbi;
        
        gbatch->m_xform = nodeXform;
        // TODO: xformInv
        
        // Add this batch
        device->addGBatch( gbatch );
    }
    
    // Now add all the child nodes
    for ( SceneNodeList::const_iterator ni = node->childs().begin();
         ni != node->childs().end(); ++ni )
    {
        _evalNode( device, (*ni), nodeXform );
    }
         
}

