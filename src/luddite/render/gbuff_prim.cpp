//
//  gbuff_prim.cpp
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <luddite/common/debug.h>
#include <luddite/render/gbuff_prim.h>

using namespace luddite;

#define NUM_CUBE_VERTS (36)
static float _cubeVertData[NUM_CUBE_VERTS * 6] = 
{
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          1.0f, 0.0f, 0.0f,
    
    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,
    
    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,
    
    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,
    
    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f
};

GBuff *luddite::gbuff_cube( float size, GLKVector3 center )
{
    GBuff *gbuff = new GBuff();
    
    DrawVert *cubeVert = gbuff->addVerts( NUM_CUBE_VERTS );
    
    for (int i=0; i < NUM_CUBE_VERTS; ++i)
    {
        cubeVert->m_pos = GLKVector3Make( (_cubeVertData[i*6 + 0] * size) - center.x,
                                 (_cubeVertData[i*6 + 1] * size) - center.y,
                                 (_cubeVertData[i*6 + 2] * size) - center.z );
 
        cubeVert->m_nrm = GLKVector3Make( (_cubeVertData[i*6 + 3] * size) - center.x,
                                 (_cubeVertData[i*6 + 4] * size) - center.y,
                                 (_cubeVertData[i*6 + 5] * size) - center.z );
        
        // derive st's from position
        size_t sndx, tndx;
        switch ( i / 6) {
            case 0: sndx = 1; tndx = 2; break; // face 0, project yz
            case 1: sndx = 0; tndx = 2; break; // face 1, project xz
            case 2: sndx = 1; tndx = 2; break; // face 2, project yz
            case 3: sndx = 0; tndx = 2; break; // face 3, project xz
            case 4: sndx = 0; tndx = 1; break; // face 4, project xy
            case 5: sndx = 0; tndx = 1; break; // face 5, project xy
        }
        
        cubeVert->m_st = GLKVector3Make(  _cubeVertData[i*6 + sndx] + 0.5,
                                          _cubeVertData[i*6 + tndx] + 0.5, 0.0 );

        
        cubeVert++;
    }
    
    return gbuff;
}


GBuff *luddite::gbuff_grid( uint32_t divisions, float size, GLKVector3 center )
{
    float halfSize = size / 2.0;
    float stepSize = size / divisions;
    float stStep = 1.0 / divisions;
    
    GBuff *gbuff = new GBuff();
    
    DrawVert *gridVert = gbuff->addVerts( divisions * divisions * 6 );
    for (int j=0; j < divisions; j++)
    {
        for (int i=0; i < divisions; i++)
        {
            GLKVector3 v00 = GLKVector3Make( -halfSize + stepSize*i, 0.0, -halfSize + stepSize*j);
            GLKVector3 st00 = GLKVector3Make( stStep * i, stStep * j, 0.0 );
            
            GLKVector3 v01 = GLKVector3Make( -halfSize + stepSize*i, 0.0, -halfSize + stepSize*(j+1));
            GLKVector3 st01 = GLKVector3Make( stStep * i, stStep * (j+1), 0.0 );
            
            GLKVector3 v10 = GLKVector3Make( -halfSize + stepSize*(i+1), 0.0, -halfSize + stepSize*j);
            GLKVector3 st10 = GLKVector3Make( stStep * (i+1), stStep * j, 0.0 );
            
            GLKVector3 v11 = GLKVector3Make( -halfSize + stepSize*(i+1), 0.0, -halfSize + stepSize*(j+1));
            GLKVector3 st11 = GLKVector3Make( stStep * (i+1), stStep * (j+1), 0.0 );
            
            gridVert[0].m_pos = v00; gridVert[0].m_st = st00;
            gridVert[1].m_pos = v10; gridVert[1].m_st = st10;
            gridVert[2].m_pos = v01; gridVert[2].m_st = st01;
            
            gridVert[3].m_pos = v10; gridVert[3].m_st = st10;
            gridVert[4].m_pos = v01; gridVert[4].m_st = st01;
            gridVert[5].m_pos = v11; gridVert[5].m_st = st11;
            
            gridVert += 6;
        }
    }

    return gbuff;
}

GBuff *luddite::gbuff_cylinder( int nsegments, float radius, 
                       float height, GLKVector3 center )
{
    GBuff *gbuff = new GBuff();
    
    // four triangles for each segment (top cap, bottom cap, middle quad)
    DrawVert *cylVert = gbuff->addVerts( nsegments * 4*3 );
    
    float halfhite = height/2.0;
    GLKVector2 plast;
    GLKVector3 stlast;
    GLKVector3 nlast;
    float t0last;
    for (size_t i=0; i < nsegments; i++)
    {
        size_t ndx = i*12;
        float tval0 = ((float)i / (float)(nsegments-1)) * M_PI;
        float tval = tval0 * 2.0;
        
        GLKVector2 pcurr = GLKVector2Make( cos( tval ) * radius, sin(tval)*radius );
        GLKVector3 stcurr = GLKVector3Make( (pcurr.x*0.5)+0.5, (pcurr.y*0.5)+0.5, 0.0 );
        GLKVector3 ncurr = GLKVector3Make( pcurr.x, 0.0, pcurr.y );
        ncurr = GLKVector3Normalize( ncurr );
        
        if (i>0)
        {            
            // Top Cap
            cylVert[ndx+0].m_pos = GLKVector3Make( 0.0, halfhite, 0.0 );
            cylVert[ndx+0].m_st = GLKVector3Make( 0.5, 0.5, 0.0 );
            cylVert[ndx+0].m_nrm = GLKVector3Make( 0.0, 1.0, 0.0 );

            cylVert[ndx+1].m_pos = GLKVector3Make( plast.x, halfhite, plast.y );
            cylVert[ndx+1].m_st = stlast;
            cylVert[ndx+1].m_nrm = GLKVector3Make( 0.0, 1.0, 0.0 );

            cylVert[ndx+2].m_pos = GLKVector3Make( pcurr.x, halfhite, pcurr.y );
            cylVert[ndx+2].m_st = stcurr;
            cylVert[ndx+2].m_nrm = GLKVector3Make( 0.0, 1.0, 0.0 );

            // Bottom cap
            cylVert[ndx+3].m_pos = GLKVector3Make( 0.0, -halfhite, 0.0 );
            cylVert[ndx+3].m_st = GLKVector3Make( 0.5, 0.5, 0.0 );
            cylVert[ndx+3].m_nrm = GLKVector3Make( 0.0, -1.0, 0.0 );
            
            cylVert[ndx+4].m_pos = GLKVector3Make( plast.x, -halfhite, plast.y );
            cylVert[ndx+4].m_st = stlast;
            cylVert[ndx+4].m_nrm = GLKVector3Make( 0.0, -1.0, 0.0 );
            
            cylVert[ndx+5].m_pos = GLKVector3Make( pcurr.x, -halfhite, pcurr.y );
            cylVert[ndx+5].m_st = stcurr;
            cylVert[ndx+5].m_nrm = GLKVector3Make( 0.0, -1.0, 0.0 );
            
            // Middle part (upper tri)
            cylVert[ndx+6].m_pos = GLKVector3Make( pcurr.x, halfhite, pcurr.y );
            cylVert[ndx+6].m_st = GLKVector3Make( tval0, 0.0, 0.0 );
            cylVert[ndx+6].m_nrm = ncurr;

            cylVert[ndx+7].m_pos = GLKVector3Make( plast.x, halfhite, plast.y );
            cylVert[ndx+7].m_st = GLKVector3Make( t0last, 0.0, 0.0 );
            cylVert[ndx+7].m_nrm = nlast;

            cylVert[ndx+8].m_pos = GLKVector3Make( pcurr.x, -halfhite, pcurr.y );
            cylVert[ndx+8].m_st = GLKVector3Make( tval0, 1.0, 0.0 );
            cylVert[ndx+8].m_nrm = ncurr;

            // Middle part (lower tri)
            cylVert[ndx+9].m_pos = GLKVector3Make( pcurr.x, -halfhite, pcurr.y );
            cylVert[ndx+9].m_st = GLKVector3Make( tval0, 1.0, 0.0 );
            cylVert[ndx+9].m_nrm = ncurr;
            
            cylVert[ndx+10].m_pos = GLKVector3Make( plast.x, -halfhite, plast.y );
            cylVert[ndx+10].m_st = GLKVector3Make( t0last, 1.0, 0.0 );
            cylVert[ndx+10].m_nrm = nlast;
            
            cylVert[ndx+11].m_pos = GLKVector3Make( plast.x, halfhite, plast.y );
            cylVert[ndx+11].m_st = GLKVector3Make( t0last, 0.0, 0.0 );
            cylVert[ndx+11].m_nrm = nlast;
        }
        plast=pcurr;
        nlast=ncurr;
        stlast=stcurr;
        t0last=tval0;
    }
    
    return gbuff;
}

void luddite::gbuff_setColorConstant( GBuff *gbuff, const GLKVector4 &color )
{
    for (std::vector<luddite::DrawVert>::iterator vi = gbuff->m_vertData.begin();
         vi != gbuff->m_vertData.end(); ++vi )
    {
        luddite::DrawVert &vert = (*vi);
        vert.m_color[0] = color.r * 0xff;
        vert.m_color[1] = color.g * 0xff;
        vert.m_color[2] = color.b * 0xff;
        vert.m_color[3] = color.a * 0xff;
    }
}
