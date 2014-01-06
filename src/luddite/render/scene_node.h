//
//  scene_node.h
//  luddite_ios
//
//  Created by Joel Davis on 8/3/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_ios_scene_node_h
#define luddite_ios_scene_node_h

#include <list>
#include <string>

#import <prmath/prmath.hpp>
#import <luddite/render/gbatch.h>

namespace luddite
{

class SceneNode;
typedef std::list<SceneNode*> SceneNodeList;
    
// A SceneNode is a node in the scene graph. All Scene Nodes are
// transform nodes. A scene node may have one or more GBatch
// attached which is the gbuff+material bindings for it that it should draw
class SceneNode
{
public:
    SceneNode( SceneNode *parent=NULL );
    SceneNode( const std::string &name, SceneNode *parent=NULL );
    
    // Returns an instance of this node
    SceneNode *makeInstance() const;
    
    // ----------------------------------------
    //  Identification
    // ----------------------------------------
    const std::string &name();
    void setName( const std::string &name );

    // ----------------------------------------
    //  Binding Gbuff (for shape nodes)
    // ----------------------------------------

    // Low level bindings
    void addGBatch( GBatch *batch );                                      

    const GBatchList &batches();
    
    // ----------------------------------------
    // Child Nodes and Scene Graph Traversal
    // ----------------------------------------
    void addChild( SceneNode *child );
    void removeChild( SceneNode *child );
    
    SceneNode *findNamedChild( const std::string &targetName );
     
    const SceneNodeList &childs();
        
    // ----------------------------------------
    //  Locations and Transform
    // ----------------------------------------    
    vec3f worldPos();
    vec3f localToWorld( const vec3f &localPos );
    vec3f worldToLocal( const vec3f &worldPos );
    
    // TODO wrap with accessors for dirty flag?
    vec3f  m_pos;
    quat4f m_rot;
    
    // Build the current xform
    matrix4x4f localXForm();
        
    
protected:
    std::string m_name;
    SceneNode *m_parent;
    
    GBatchList m_shapes;
    SceneNodeList m_childs;        
};


};

#endif
