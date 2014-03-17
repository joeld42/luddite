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
    }
    
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
        m_gbatch->m_gbuff->m_capacity = m_maxParticles;
        m_gbatch->m_xform = GLKMatrix4Identity;
        m_gbatch->m_xformInv = GLKMatrix4Identity;
    }
    
    // build geometry for particles
    GBuff *gb = m_gbatch->m_gbuff;
    gb->m_vertData.clear();
    DrawVert *verts = gb->addVerts( m_particles.size() );
    
    for (size_t i=0; i < m_particles.size(); i++)
    {
        verts[i].m_pos = m_particles[i].m_pos;
        verts[i].m_color[0] = m_particles[i].m_color[0];
        verts[i].m_color[1] = m_particles[i].m_color[1];
        verts[i].m_color[2] = m_particles[i].m_color[2];
        verts[i].m_color[3] = m_particles[i].m_color[3];
    }

    return m_gbatch;
}

