//
//  scene_node.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/3/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <luddite/render/scene_node.h>

using namespace luddite;

SceneNode::SceneNode( SceneNode *parent ) :
    m_parent( parent ),
    m_pos( 0.0, 0.0, 0.0 ),
    m_rot( 0.0, 0.0, 0.0, 1.0 )
{
}

SceneNode::SceneNode( const eastl::string &name, 
                     SceneNode *parent ) :
    m_parent(parent),
    m_name(name),
    m_pos( 0.0, 0.0, 0.0 ),
    m_rot( 0.0, 0.0, 0.0, 1.0 )
{
    
}

const eastl::string &SceneNode::name()
{
    return m_name;
}

void SceneNode::setName( const eastl::string &name )
{
    m_name = name;
}


void SceneNode::addChild( SceneNode *child )
{
    m_childs.push_back( child );
}

void SceneNode::removeChild( SceneNode *child )
{
    m_childs.remove( child );
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

vec3f SceneNode::localToWorld( const vec3f &localPos )
{
    // todo.. figure out where combined xform lives
}

vec3f SceneNode::worldToLocal( const vec4f &worldPos )
{
    
}

