//
//  particle_batch.h
//  luddite
//
//  Created by Joel Davis on 2/25/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#ifndef __luddite__particle_batch__
#define __luddite__particle_batch__

#import <luddite/render/material.h>

// Might want to unify this with the gbuff so a partibatch is just
// another gbuff, but not sure about that. For now, keep it a separate
// render step

// ParticleBatches are always built in world space, since multiple local
// space emitters might contribute particles to a shared batch.
namespace luddite
{

class ParticleBatch
{
    luddite::Material *m_mtl;
};
    
} // namescape luddite

#endif /* defined(__luddite__particle_batch__) */
