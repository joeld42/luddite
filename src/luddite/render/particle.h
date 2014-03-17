//
//  particle.h
//  luddite
//
//  Created by Joel Davis on 2/27/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#ifndef luddite_particle_h
#define luddite_particle_h

#include <GLKit/GLKMath.h>

namespace luddite
{
    
    // basic particle for simulation
    struct Particle
    {
        GLKVector3 m_pos;
        GLKVector3 m_vel;
        uint8_t	m_color[4];
        float m_age;
        float m_lifetime;
    };
    
}; // namespace luddite


#endif
