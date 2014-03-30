//
//  emitter.cpp
//  luddite
//
//  Created by Joel Davis on 2/25/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#include "luddite/common/useful.h"
#include "luddite/render/emitter.h"
#include "luddite/render/particle.h"
#include "luddite/render/particle_group.h"

using namespace luddite;

EmitterNode::EmitterNode( SceneNode *parent ) :
        SceneNode( parent )
{
    
}

EmitterNode::EmitterNode( const std::string &name, SceneNode *parent ) :
            SceneNode( name, parent )
{
    
}


void EmitterNode::emitIntoGroup( luddite::ParticleGroup *group, float dt )
{
    size_t numParts = _numPartsForRate( m_rate, dt);

    // Emit the particles
    Particle part;
    GLKMatrix4 emitterXform = nodeXForm();
    for (size_t i=0; i < numParts; i++)
    {
        // TODO: set up particle
        GLKVector3 pos, vel;
        pos = GLKVector3Make( 0.0, 0.0, 0.0 );
        vel = GLKVector3Make( ((randUniform()*2.0) - 1.0) * 0.3,
                            ((randUniform()*2.0) - 1.0)  * 0.3,
                              randUniform() * 2.0 ); // TODO: calculate this right
        
        vel = GLKVector3MultiplyScalar( GLKVector3Normalize( vel ), randUniform( 3.0, 10.0 ) );
        part.m_lifetime = randNormal( 1.0, 0.25 );

        // transform by this emitter's xform
        part.m_pos = GLKMatrix4MultiplyVector3WithTranslation( emitterXform, pos );
        part.m_vel = GLKMatrix4MultiplyVector3( emitterXform,  vel );
        part.m_age = 0.0;

        group->addParticle( part );

        if (group->isFull()) break;
    }
    
//    printf("EmitterNode: Now has %zu particles...\n", group->particleCount() );
}

size_t EmitterNode::_numPartsForRate(float rate, float dt) {// number of parts to emit
    size_t numParts = 0;
    float numPartsReal = rate * dt;

    // Treat fractional particles as a probability -- e.g. .3 particles
    // means 30% chance to emit
    float numPartsF = floor(numPartsReal);
    float partProb = numPartsReal - numPartsF;
    numParts = (size_t)numPartsF;

    if (randUniform() < partProb)
    {
        numParts++;
    }
    return numParts;
}
