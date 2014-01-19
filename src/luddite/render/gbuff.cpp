//
//  gbuff.cpp
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <GLKit/GLKMath.h>

#include <luddite/common/debug.h>
#include <luddite/render/drawvert.h>
#include <luddite/render/gbuff.h>


using namespace luddite;

GBuff::GBuff() :
    m_vbo( 0 )
{
}

luddite::DrawVert *GBuff::addVerts( size_t numVerts )
{
    DrawVert defaultVert;
    defaultVert.m_pos = GLKVector3Make( 0.0, 0.0, 0.0 );
    defaultVert.m_st  = GLKVector3Make( 0.0, 0.0, 0.0 );
    defaultVert.m_nrm = GLKVector3Make( 0.0, 1.0 , 0.0 );
    defaultVert.m_color[0] = 0xff;
    defaultVert.m_color[1] = 0xff;
    defaultVert.m_color[2] = 0xff;
    defaultVert.m_color[3] = 0xff;
    
    // Grow the array
    size_t oldSize = m_vertData.size();
    m_vertData.resize( oldSize + numVerts, defaultVert );
    
    // Return the first of the new verts
    return &(m_vertData[oldSize]);
}

luddite::DrawVert *GBuff::addTri() // adds 3 verts
{
    return addVerts( 3 );
}

luddite::DrawVert *GBuff::addQuad() // adds 6 verts
{
    return addVerts( 6 );
}
