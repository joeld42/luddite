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


namespace luddite {

enum
{
    BatchType_TRIANGLES,
    BatchType_PARTICLES
};
    
// A gbatch is a gbuff bound to an xform and a material
// and textures. (TODO should textures live on the material??)
struct GBatch 
{
    GLKMatrix4 m_xform;
    GLKMatrix4 m_xformInv;

    luddite::GBuff *m_gbuff;
    luddite::Material *m_mtl;
    int m_batchType;
};
  
typedef std::list<GBatch*> GBatchList;
    
}; // namespace luddite

#endif
