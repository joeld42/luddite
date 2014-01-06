//
//  drawvert.h
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_ios_drawvert_h
#define luddite_ios_drawvert_h

#include <prmath/prmath.hpp>

namespace luddite
{

// Common per-vertex attributes
struct DrawVert
{
    vec3f m_pos;
    vec2f m_st;
    vec3f m_nrm;
    // prmath::vec3			tangents[2];
    uint8_t	m_color[4];
};
    
}; // namespace luddite


#endif
