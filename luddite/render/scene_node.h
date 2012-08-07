//
//  scene_node.h
//  luddite_ios
//
//  Created by Joel Davis on 8/3/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_ios_scene_node_h
#define luddite_ios_scene_node_h

#include <EASTL/list.h>
#include <EASTL/string.h>

#import <prmath/prmath.hpp>
#import <luddite/render/gbatch.h>

namespace luddite
{

// A SceneNode is a node in the scene graph. All Scene Nodes are
// transform nodes. A scene node may have one or more GBatch
// attached which is the gbuff+material bindings for it that it should draw
class SceneNode
{
public:
    SceneNode( SceneNode *parent=NULL );
    SceneNode( const eastl::string &name, SceneNode *parent=NULL );
    
    // ----------------------------------------
    //  Identification
    // ----------------------------------------
    const eastl::string &name();
    void setName( const eastl::string &name );

    // ----------------------------------------
    //  Binding Gbuff (for shape nodes)
    // ----------------------------------------

    // Low level bindings
    void addGBatch( GBatch *batch );                                      
    
    // ----------------------------------------
    // Child Nodes and Scene Graph Traversal
    // ----------------------------------------
    void addChild( SceneNode *child );
    void removeChild( SceneNode *child );
    
    SceneNode *findNamedChild( const eastl::string &targetName );
        
    // ----------------------------------------
    //  Locations and Transform
    // ----------------------------------------    
    vec3f worldPos();
    vec3f localToWorld( const vec3f &localPos );
    vec3f worldToLocal( const vec4f &worldPos );
    
    // TODO wrap with accessors for dirty flag?
    vec3f  m_pos;
    quat4f m_rot;
        
    
protected:
    eastl::string m_name;
    SceneNode *m_parent;
    
    eastl::list<GBatch*> m_shapes;
    eastl::list<SceneNode*> m_childs;
    
    // Cached transforms
    // TODO: dirty flag, etc...
    matrix4x4f m_xform;
    matrix4x4f m_xformInv;
    
};
    
};

#endif
