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

#include <GLKit/GLKMath.h>

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
    void removeFromParent();    
    SceneNode *parent();

    
    SceneNode *findNamedChild( const std::string &targetName );
     
    const SceneNodeList &childs();
        
    // ----------------------------------------
    //  Locations and Transform
    // ----------------------------------------    
    GLKVector3 worldPos();
    GLKVector3 localToWorld( const GLKVector3 &localPos );
    GLKVector3 worldToLocal( const GLKVector3 &worldPos );
    
    // TODO wrap with accessors for dirty flag?
    GLKVector3  m_pos;
    GLKQuaternion m_rot;
    
    // Build the current xform
    GLKMatrix4 localXForm();
    GLKMatrix4 nodeXForm(); // node xform at last eval

    // used by scene::eval()
    void _sceneEval_setNodeXform( const GLKMatrix4 &xform );
    
protected:
    std::string m_name;
    SceneNode *m_parent;
    GLKMatrix4 m_nodeXform; 
    
    GBatchList m_shapes;
    SceneNodeList m_childs;        
};


};

#endif
