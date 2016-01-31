//
//  gbatch.h
//  luddite_ios
//
//  Created by Joel Davis on 8/3/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_gbatch_h
#define luddite_gbatch_h

#include <list>

#include <GLKit/GLKMath.h>

#include <luddite/render/gbuff.h>
#include <luddite/render/material.h>

enum {
    GBatchFlag_BLEND   = 0x01,  // Draw after the others, in the blend pass

    GBatchFlag_LINES   = 0x02,  // Draw as lines, for debugging info
    GBatchFlag_POINTS  = 0x04,  // Draw as points, for debugging info
};

enum {
    GBatchBlendMode_OVER,
    GBatchBlendMode_ADD
};

namespace luddite {

// A gbatch is a gbuff bound to an xform and a material
// and textures. (TODO should textures live on the material??)
struct GBatch 
{
    GLKMatrix4 m_xform;
    GLKMatrix4 m_xformInv;

    luddite::GBuff *m_gbuff;
    luddite::Material *m_mtl;

    uint64_t m_flags;
    uint64_t m_blendMode;
};
  
typedef std::list<GBatch*> GBatchList;
    
}; // namespace luddite

#endif
