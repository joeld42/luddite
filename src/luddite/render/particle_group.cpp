//
//  particle_group.cpp
//  luddite
//
//  Created by Joel Davis on 2/27/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#include "particle_group.h"

using namespace luddite;

ParticleGroup::ParticleGroup( Material *mtl, size_t maxParticles ) :
    m_mtl( mtl ),
    m_maxParticles(maxParticles),
    m_gbatch(NULL)
{
}

// Main point for subclasses to customize particle behavior.
void ParticleGroup::update( float dt )
{
    // This really should happen with a subclass, or components, but
    // for now just do it here...
    GLKVector3 gravity = GLKVector3Make( 0.0, -1.0 * dt, 0.0 );
    for ( Particle &p : m_particles)
    {
        GLKVector3 dp = GLKVector3MultiplyScalar( p.m_vel, dt );
        p.m_pos = GLKVector3Add( p.m_pos, dp );
        p.m_age += dt;
        
        // Gravity
        p.m_vel = GLKVector3Add( p.m_vel, gravity );
        
        // Age particle
        p.m_age += dt;
    }
    
    // age out any old particles
    m_particles.erase( std::remove_if( m_particles.begin(), m_particles.end(),
                                      [](const Particle &p)
                                      { return p.m_age > p.m_lifetime; }),
                      m_particles.end() );
    
}

// Emitters use this
void ParticleGroup::addParticle( Particle p )
{
    if (!isFull())
    {
        m_particles.push_back(p);
    }
}

bool ParticleGroup::isFull()
{
    return !((m_maxParticles==0)||(m_particles.size() < m_maxParticles));
}


size_t ParticleGroup::particleCount()
{
    return m_particles.size();
}

size_t ParticleGroup::maxParticles()
{
    return m_maxParticles;
}

luddite::GBatch *ParticleGroup::_buildParticles()
{
    if (!m_gbatch)
    {
        m_gbatch = new GBatch();
        m_gbatch->m_mtl = m_mtl;
        m_gbatch->m_gbuff = new GBuff();
        m_gbatch->m_gbuff->m_capacity = (uint32_t)m_maxParticles;
        m_gbatch->m_xform = GLKMatrix4Identity;
        m_gbatch->m_xformInv = GLKMatrix4Identity;
    }
    
    // build geometry for particles
    GBuff *gb = m_gbatch->m_gbuff;
    gb->m_vertData.clear();
    DrawVert *verts = gb->addVerts( m_particles.size() );
    
    for (size_t i=0; i < m_particles.size(); i++)
    {
        const Particle &p = m_particles[i];
        verts[i].m_pos = p.m_pos;
        verts[i].m_color[0] = p.m_color[0];
        verts[i].m_color[1] = p.m_color[1];
        verts[i].m_color[2] = p.m_color[2];
        verts[i].m_color[3] = p.m_color[3];

        // yuk, just stuff the particle age, etc into unused drawvert fields
        // really should have a separate vert type for particles
        verts[i].m_st = GLKVector3Make( p.m_age/p.m_lifetime, 0, 0 );
    }

    return m_gbatch;
}

