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

void _calcTangentsForEdge( luddite::DrawVert &a, luddite::DrawVert & b );

using namespace luddite;

GBuff::GBuff() :
    m_vbo( 0 ),
    m_capacity( 0 )
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

// calculates tangents for lighting
void GBuff::calcTangents()
{
    for (int i=0; i< m_vertData.size(); i += 3)
    {
        luddite::DrawVert &a = m_vertData[i];
        luddite::DrawVert &b = m_vertData[i+1];
        luddite::DrawVert &c = m_vertData[i+2];
        
        GLKVector3 edge1 = GLKVector3Subtract( b.m_pos, a.m_pos );
        GLKVector3 edge2 = GLKVector3Subtract( c.m_pos, a.m_pos );
        GLKVector3 stEdge1 = GLKVector3Subtract( b.m_st, a.m_st );
        GLKVector3 stEdge2 = GLKVector3Subtract( c.m_st, a.m_st );
        
        float det = stEdge1.x*stEdge2.y - stEdge2.x*stEdge1.y;
        if (det < 0.0) {
            a.m_tangent = GLKVector3Make( 1.0, 0.0, 1.0 );
        }
        
        float f = 1.0f / det;
        GLKVector3 tangent;
        tangent.x = f * (stEdge2.y * edge1.x - stEdge1.y*edge2.x);
        tangent.y = f * (stEdge2.y * edge1.y - stEdge1.y*edge2.y);
        tangent.z = f * (stEdge2.y * edge1.z - stEdge1.y*edge2.z);
        tangent = GLKVector3Normalize( tangent );
        
        a.m_tangent = tangent;
        b.m_tangent = tangent;
        c.m_tangent = tangent;
    }
    
    // Now we have "faceted" tangents, accumulate and average
    // tangets which share UVs
    // TODO: support indexed meshes so this is faster
    for (int i=0; i < m_vertData.size(); i++)
    {
        int count= 1;
        GLKVector3 tot = m_vertData[i].m_tangent;
        for (int j=0; j < m_vertData.size(); j++)
        {
            if (j==i) continue;
            if (GLKVector3AllEqualToVector3( m_vertData[i].m_st, m_vertData[j].m_st))
            {
                tot = GLKVector3Add(tot, m_vertData[j].m_tangent );
                count++;
            }
        }
        if (count > 1)
        {
            tot = GLKVector3MultiplyScalar( tot, 1.0 / (float)count );
        }
//        printf ( "avg count %d\n", count );

        // store in bitangent temporarily while averaging...
        m_vertData[i].m_bitangent = tot;
    }
    
    // Compute the actual bitangent
    for (int i=0; i < m_vertData.size(); i++)
    {
        // restore averaged tangents
        m_vertData[i].m_tangent = GLKVector3Normalize( m_vertData[i].m_bitangent );

        // compute bitangent.
        m_vertData[i].m_bitangent = GLKVector3Normalize( GLKVector3CrossProduct( m_vertData[i].m_nrm, m_vertData[i].m_tangent));
    }
    
}


