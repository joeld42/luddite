//
//  scene_node.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/3/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <EASTL/string.h>

#include <luddite/render/scene_node.h>

// FIXME: move this somewhere else
namespace eastl
{
EASTL_API EmptyString gEmptyString;
}


using namespace luddite;

SceneNode::SceneNode( SceneNode *parent ) :
    m_parent( parent ),
    m_pos( 0.0, 0.0, 0.0 ),
    m_rot( 0.0, 0.0, 0.0, 1.0 )
{
    if (parent)
    {
        parent->addChild( this );
    }
}

SceneNode::SceneNode( const eastl::string &name, 
                     SceneNode *parent ) :
    m_parent(parent),
    m_name(name),
    m_pos( 0.0, 0.0, 0.0 ),
    m_rot( 0.0, 0.0, 0.0, 1.0 )
{
    if (parent)
    {
        parent->addChild( this );
    }
}

const eastl::string &SceneNode::name()
{
    return m_name;
}

void SceneNode::setName( const eastl::string &name )
{
    m_name = name;
}

void SceneNode::addGBatch( GBatch *batch )
{
    m_shapes.push_back( batch );
}

const eastl::list<GBatch*> &SceneNode::batches()
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

SceneNode *SceneNode::findNamedChild( const eastl::string &targetName )
{
    for (eastl::list<SceneNode*>::iterator chi = m_childs.begin();
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
    
    xform.Identity();
    xform.Translate(m_pos);
    
    rot = m_rot; // quat to matrix
    xform = xform * rot;
    
    return xform;
}

vec3f SceneNode::localToWorld( const vec3f &localPos )
{

}

vec3f SceneNode::worldToLocal( const vec4f &worldPos )
{
    
}

