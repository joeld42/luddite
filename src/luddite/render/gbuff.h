//
//  gbuff.h
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

// GBuff is a graphics buffer, a basic unit-of-work containing a buffer of
// data to send to a renderdevice.

#ifndef luddite_ios_gbuff_h
#define luddite_ios_gbuff_h

#include <vector>

#include <luddite/render/drawvert.h>

namespace luddite {
    
struct GBuff 
{    
    GBuff();
    
    luddite::DrawVert *addVerts( size_t numVerts );
    
    luddite::DrawVert *addTri(); // adds 3 verts
    luddite::DrawVert *addQuad(); // adds 6 verts
    
    // calculates tangents for lighting
    void calcTangents();
    
    std::vector<luddite::DrawVert> m_vertData;
    uint32_t m_vbo;
    uint32_t m_capacity; // for particle gbuffs, max capacity to allocate

    // NOTE(jbd): Dynamic stuff is pretty crappy, just added for the debug
    // draw stuff. Don't trust it.
    bool m_dynamic = false; // allocate with DYNAMIC_DRAW
    uint32_t m_dynamicSize; // For dynamic, how many verts did we use this frame?
};

}; // namespace luddite

#endif
