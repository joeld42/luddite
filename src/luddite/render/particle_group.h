//
//  particle_group.h
//  luddite
//
//  Created by Joel Davis on 2/27/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#ifndef __luddite__particle_group__
#define __luddite__particle_group__

#include <luddite/render/particle_batch.h>
#include <luddite/render/emitter.h>
#include <luddite/render/particle.h>

// Emitters add particles to a particle group. Many emitters may emit into
// a single group. Emitters have a local space and may exist in the scene graph
//
// Particle group is a group of similar particles. Particles in the same
// group share the same behavior and appearance. Particle groups are all in
// world space.
//
// Particle batches (Pbatch) are what is boiled down and sent to the gpu.
//
//
// +------------+
// | Emitter 1  |-+
// +------------+ |   +------------+
//                +-->| PGroup A   |-+
// +------------+ |   +------------+ |     +------------+
// | Emitter 2  |-+                  +---->| PBatch     |
// +------------+     +------------+ |     +------------+
//                 ,->| PGroup B   |-+
// +------------+  |  +------------+
// | Emitter 3  |+-/
// +------------+
//

namespace luddite {

    
class ParticleGroup
{
public:
    ParticleGroup( ParticleBatch *pbatch );

    // Main point for subclasses to customize particle behavior.
    virtual void update( float dt );

    // Emitters use this
    void addParticle( Particle p );
    
protected:
    
    luddite::ParticleBatch *m_pbatch;
    std::vector<luddite::Particle> m_particles;
    
};
    
}; // namespace luddite

#endif /* defined(__luddite__particle_group__) */
