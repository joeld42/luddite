//
//  emitter.h
//  luddite
//
//  Created by Joel Davis on 2/25/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#ifndef __luddite__emitter__
#define __luddite__emitter__

#include <GLKit/GLKMath.h>

#include <luddite/render/particle_batch.h>
#include <luddite/render/scene_node.h>
#include <luddite/render/particle.h>


namespace luddite
{

class ParticleGroup;
    
// An emitter is a scene node that also adds particles to
// a particle batch.
class EmitterNode : public SceneNode
{
    EmitterNode( SceneNode *parent=NULL );
    EmitterNode( const std::string &name, SceneNode *parent=NULL );

    void emitIntoGroup( luddite::ParticleGroup *group, float dt );
    
public:
    float m_rate; // particles per second avg.
    GLKVector3 m_dir; // direction
    float m_spreadAngle; // degrees, 0..180
};
    
} // namespace luddite

#endif /* defined(__luddite__emitter__) */
