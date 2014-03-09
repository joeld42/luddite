//
//  scene.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/7/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <luddite/common/debug.h>
#include <luddite/render/scene.h>

#include <luddite/render/emitter.h>
#include <luddite/render/particle_group.h>
#include <luddite/render/particle_batch.h>


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
//    GLKMatrix4 rootXform;
//    rootXform.Identity();
    _evalNode( device, m_sceneRoot, GLKMatrix4Identity );

    // eval pgroups into emitters
    for (auto pg : m_pgroups)
    {
        GBatch *pbatch = pg->_buildParticles();
        device->addGBatch( pbatch );
        printf("Added pbatch... %p\n", pbatch );
    }
}

void Scene::_evalNode( RenderDevice *device, SceneNode *node, GLKMatrix4 currXform )
{
    // update the transform for this batch
    GLKMatrix4 nodeXform = node->localXForm();
    //nodeXform = nodeXform * currXform;
    nodeXform = GLKMatrix4Multiply( nodeXform, currXform );
    
    // Add the gbatch for this node
    // For now just add everything, need to be smarter about traversal
    for ( GBatchList::const_iterator gbi = node->batches().begin();
         gbi != node->batches().end(); ++gbi )
    {
        GBatch *gbatch = *gbi;
        
        gbatch->m_xform = nodeXform;
        // TODO: xformInv
        
//        DBG::info( "node [%s] xform %3.2f %3.2f %3.2f %3.2f\n"
//                   "              %3.2f %3.2f %3.2f %3.2f\n"
//                   "              %3.2f %3.2f %3.2f %3.2f\n"
//                   "              %3.2f %3.2f %3.2f %3.2f\n",                  
//                  node->name().c_str(),
//
//                  nodeXform.m16[0],
//                  nodeXform.m16[1],
//                  nodeXform.m16[2],
//                  nodeXform.m16[3],
//                  
//                  nodeXform.m16[4],
//                  nodeXform.m16[5],
//                  nodeXform.m16[6],
//                  nodeXform.m16[7],
//                  
//                  nodeXform.m16[8],
//                  nodeXform.m16[9],
//                  nodeXform.m16[10],
//                  nodeXform.m16[11],
//                  
//                  nodeXform.m16[12],
//                  nodeXform.m16[13],
//                  nodeXform.m16[14],
//                  nodeXform.m16[15] );
//        
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

// Particles
void Scene::addParticleGroup( ParticleGroup *pgroup )
{
    m_pgroups.push_back(pgroup);
}
