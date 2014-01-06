#ifndef LUDDITE_GAME_ENTITY_H
#define LUDDITE_GAME_ENTITY_H
//
//  entity.h
//  luddite
//
//  Created by Joel Davis on 12/23/13.
//
// Entity is a container for a bunch of behaviors, with an optional
// Scene Node

#include <list>
#include <string>

#include <luddite/common/useful.h>

namespace luddite {
    
class SceneNode;
class Behavior;
    
class Entity
{
public:
    Entity( SceneNode *node=NULL );
    
    // Behaviors
    void addBehavior( luddite::Behavior *beh );

    Behavior *getBehaviorByTag( const std::string &tag );

    void updateSim( TimeInterval dt );
    
protected:
    
    // Node that this entity controls (may be null, and the node may control
    // other nodes, but this is the most common case)
    SceneNode *m_node;
};
    
} // namespace luddite

#endif /* LUDDITE_GAME_ENTITY_H */
