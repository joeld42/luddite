//
//  scene.h
//  luddite_ios
//
//  Created by Joel Davis on 8/7/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_scene_h
#define luddite_scene_h

#include <GLKit/GLKMath.h>

#include <luddite/render/scene_node.h>
#include <luddite/render/render_device.h>

namespace luddite
{

// TODO: visibility
    
// Top level scene class, handles scene evaluation
// and traversal and acts as a container for a scene graph.
class Scene
{
public:
    Scene( SceneNode *root=NULL );
    
    // Scene Root
    void setRoot( SceneNode *root );
    SceneNode *root();
    
    // Evaluate scene and add visible nodes
    // to the RenderDevice for this frame
    void eval( RenderDevice *device );
    
protected:
    void _evalNode( RenderDevice *device, SceneNode *node, GLKMatrix4 currXform );
    SceneNode *m_sceneRoot;
};
    
    
}; // namespace luddite

#endif
