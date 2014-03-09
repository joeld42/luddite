//
//  particle_group.h
//  luddite
//
//  Created by Joel Davis on 2/27/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#ifndef __luddite__particle_group__
#define __luddite__particle_group__

#include <luddite/render/material.h>
#include <luddite/render/emitter.h>
#include <luddite/render/particle.h>

// Emitters add particles to a particle group. Many emitters may emit into
// a single group. Emitters have a local space and may exist in the scene graph
//
// Particle group is a group of similar particles. Particles in the same
// group share the same behavior and appearance. Particle groups are all in
// world space.

namespace luddite {

    
class ParticleGroup
{
public:
    ParticleGroup( luddite::Material *mtl, size_t maxParticles=0 );

    // Main point for subclasses to customize particle behavior.
    virtual void update( float dt );

    // Emitters use these
    void addParticle( Particle p );
    bool isFull();

    // Info
    size_t particleCount();
    
    luddite::GBatch *_buildParticles();
    
protected:

    luddite::Material *m_mtl;
    std::vector<luddite::Particle> m_particles;
    size_t m_maxParticles; // cap size of group
    
    luddite::GBatch *m_gbatch;
};
    
}; // namespace luddite

#endif /* defined(__luddite__particle_group__) */
