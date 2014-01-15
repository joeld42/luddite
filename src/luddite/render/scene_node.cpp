//
//  scene_node.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/3/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <string>

#include <luddite/common/debug.h>
#include <luddite/render/scene_node.h>

using namespace luddite;

SceneNode::SceneNode( SceneNode *parent ) :
    m_pos( 0.0, 0.0, 0.0 ),
    m_rot( 0.0, 0.0, 0.0, 1.0 ),
    m_parent( parent )
{
    if (parent)
    {
        parent->addChild( this );
    }
}

SceneNode::SceneNode( const std::string &name, 
                     SceneNode *parent ) :
        m_pos( 0.0, 0.0, 0.0 ),
        m_rot( 0.0, 0.0, 0.0, 1.0 ),
        m_name(name),
        m_parent(parent)
{
    if (parent)
    {
        parent->addChild( this );
    }
}

SceneNode *SceneNode::makeInstance() const
{
    luddite::SceneNode *nodeInst = new luddite::SceneNode();
    for (auto bi = m_shapes.begin(); bi != m_shapes.end(); bi++)
    {
        luddite::GBatch *nodeBatch = new luddite::GBatch();
        nodeBatch->m_gbuff = (*bi)->m_gbuff;
        nodeBatch->m_mtl = (*bi)->m_mtl;
        nodeInst->addGBatch( nodeBatch );
    }

    return nodeInst;
}

const std::string &SceneNode::name()
{
    return m_name;
}

void SceneNode::setName( const std::string &name )
{
    m_name = name;
}

void SceneNode::addGBatch( GBatch *batch )
{
    m_shapes.push_back( batch );
}

const std::list<GBatch*> &SceneNode::batches()
{
    return m_shapes;
}

void SceneNode::addChild( SceneNode *child )
{
    child->m_parent = this;
    m_childs.push_back( child );
}

void SceneNode::removeChild( SceneNode *child )
{
    // TODO: make sure child exists in childs
    child->m_parent = NULL;
    m_childs.remove( child );
}

const SceneNodeList &SceneNode::childs()
{
    return m_childs;
}

SceneNode *SceneNode::findNamedChild( const std::string &targetName )
{
    for (std::list<SceneNode*>::iterator chi = m_childs.begin();
         chi != m_childs.end(); ++chi )
    {
        if ( (*chi)->name() == targetName ) return (*chi);
    }
    return NULL;
}

vec3f SceneNode::worldPos()
{
    return localToWorld(vec3f(0.0,0.0,0.0) );
}

matrix4x4f SceneNode::localXForm()
{
    matrix4x4f xform;
    matrix4x4f rot;
    
//    DBG::info( "[%s] m_pos is %f %f %f\n", m_name.c_str(), m_pos.x, m_pos.y, m_pos.z );
    
    xform.Identity();
    xform.Translate(m_pos);

//    DBG::info( "[%s] m_rot is %f %f %f %f\n", m_name.c_str(), m_rot.x, m_rot.y, m_rot.z, m_rot.w );
    
    rot = m_rot; // quat to matrix
    xform = rot * xform;
    
    return xform;
}

vec3f SceneNode::localToWorld( const vec3f &localPos )
{
    // TODO
    return localPos;
}

vec3f SceneNode::worldToLocal( const vec3f &worldPos )
{
    // TODO
    return worldPos;
}
