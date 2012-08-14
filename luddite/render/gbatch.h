//
//  gbatch.h
//  luddite_ios
//
//  Created by Joel Davis on 8/3/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_gbatch_h
#define luddite_gbatch_h

#include <EASTL/list.h>

#include <prmath/prmath.hpp>
#include <luddite/render/gbuff.h>
#include <luddite/render/material.h>

// Number of textures assign with the batch. More 
// can be used as regular shader params
#define kGBatchNumTex (4)

namespace luddite {
    
// A gbatch is a gbuff bound to an xform and a material
// and textures. (TODO should textures live on the material??)
struct GBatch 
{
    matrix4x4f m_xform;
    matrix4x4f m_xformInv;
    
    uint32_t m_tex[kGBatchNumTex];
    
    luddite::GBuff *m_gbuff;
    luddite::Material *m_mtl;
};
  
typedef eastl::list<GBatch*> GBatchList;
    
}; // namespace luddite

#endif
