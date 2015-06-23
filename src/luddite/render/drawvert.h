//
//  drawvert.h
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_ios_drawvert_h
#define luddite_ios_drawvert_h

#include <GLKit/GLKMath.h>

namespace luddite
{

// Common per-vertex attributes
struct DrawVert
{
    GLKVector3 m_pos;
    GLKVector3 m_st;
    GLKVector3 m_nrm;
    GLKVector3 m_tangent;
    GLKVector3 m_bitangent;
    uint8_t	m_color[4];
};
    
}; // namespace luddite


#endif
