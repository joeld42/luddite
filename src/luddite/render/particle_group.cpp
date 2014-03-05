//
//  particle_group.cpp
//  luddite
//
//  Created by Joel Davis on 2/27/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#include "particle_group.h"

using namespace luddite;

ParticleGroup::ParticleGroup( luddite::ParticleBatch *pbatch ) :
    m_pbatch( pbatch)
{
}

// Main point for subclasses to customize particle behavior.
void ParticleGroup::update( float dt )
{
}

// Emitters use this
void ParticleGroup::addParticle( Particle p )
{
    m_particles.push_back(p);
}
