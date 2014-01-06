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

GBuff *luddite::gbuff_cube( float size, vec3f center )
{
    GBuff *gbuff = new GBuff();
    
    DrawVert *cubeVert = gbuff->addVerts( NUM_CUBE_VERTS );
    
    for (int i=0; i < NUM_CUBE_VERTS; ++i)
    {
        cubeVert->m_pos = vec3f( (_cubeVertData[i*6 + 0] * size) - center.x,
                                 (_cubeVertData[i*6 + 1] * size) - center.y,
                                 (_cubeVertData[i*6 + 2] * size) - center.z );
 
        cubeVert->m_nrm = vec3f( (_cubeVertData[i*6 + 3] * size) - center.x,
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
        
        cubeVert->m_st = vec2f( _cubeVertData[i*6 + sndx] + 0.5,
                                _cubeVertData[i*6 + tndx] + 0.5 );

        
        cubeVert++;
    }
    
    return gbuff;
}

GBuff *luddite::gbuff_cylinder( int nsegments, float radius, 
                       float height, vec3f center )
{
    GBuff *gbuff = new GBuff();
    
    // four triangles for each segment (top cap, bottom cap, middle quad)
    DrawVert *cylVert = gbuff->addVerts( nsegments * 4*3 );
    
    float halfhite = height/2.0;
    vec2f plast;
    vec2f stlast;
    vec3f nlast;
    float t0last;
    for (size_t i=0; i <= nsegments; i++)
    {
        size_t ndx = i*12;
        float tval0 = ((float)i / (float)(nsegments-1)) * M_PI;
        float tval = tval0 * 2.0;
        
        vec2f pcurr( cos( tval ) * radius, sin(tval)*radius );
        vec2f stcurr( (pcurr.x*0.5)+0.5, (pcurr.y*0.5)+0.5 );
        vec3f ncurr( pcurr.x, 0.0, pcurr.y );
        ncurr.Normalize();
        if (i>0)
        {            
            // Top Cap
            cylVert[ndx+0].m_pos = vec3f( 0.0, halfhite, 0.0 );
            cylVert[ndx+0].m_st = vec2f( 0.5, 0.5 );
            cylVert[ndx+0].m_nrm = vec3f( 0.0, 1.0, 0.0 );

            cylVert[ndx+1].m_pos = vec3f( plast.x, halfhite, plast.y );
            cylVert[ndx+1].m_st = stlast;
            cylVert[ndx+1].m_nrm = vec3f( 0.0, 1.0, 0.0 );

            cylVert[ndx+2].m_pos = vec3f( pcurr.x, halfhite, pcurr.y );
            cylVert[ndx+2].m_st = stcurr;
            cylVert[ndx+2].m_nrm = vec3f( 0.0, 1.0, 0.0 );

            // Bottom cap
            cylVert[ndx+3].m_pos = vec3f( 0.0, -halfhite, 0.0 );
            cylVert[ndx+3].m_st = vec2f( 0.5, 0.5 );
            cylVert[ndx+3].m_nrm = vec3f( 0.0, -1.0, 0.0 );
            
            cylVert[ndx+4].m_pos = vec3f( plast.x, -halfhite, plast.y );
            cylVert[ndx+4].m_st = stlast;
            cylVert[ndx+4].m_nrm = vec3f( 0.0, -1.0, 0.0 );
            
            cylVert[ndx+5].m_pos = vec3f( pcurr.x, -halfhite, pcurr.y );
            cylVert[ndx+5].m_st = stcurr;
            cylVert[ndx+5].m_nrm = vec3f( 0.0, -1.0, 0.0 );
            
            // Middle part (upper tri)
            cylVert[ndx+6].m_pos = vec3f( pcurr.x, halfhite, pcurr.y );
            cylVert[ndx+6].m_st = vec2f( tval0, 0.0 );
            cylVert[ndx+6].m_nrm = ncurr;

            cylVert[ndx+7].m_pos = vec3f( plast.x, halfhite, plast.y );
            cylVert[ndx+7].m_st = vec2f( t0last, 0.0 );
            cylVert[ndx+7].m_nrm = nlast;

            cylVert[ndx+8].m_pos = vec3f( pcurr.x, -halfhite, pcurr.y );
            cylVert[ndx+8].m_st = vec2f( tval0, 1.0 );
            cylVert[ndx+8].m_nrm = ncurr;

            // Middle part (lower tri)
            cylVert[ndx+9].m_pos = vec3f( pcurr.x, -halfhite, pcurr.y );
            cylVert[ndx+9].m_st = vec2f( tval0, 1.0 );
            cylVert[ndx+9].m_nrm = ncurr;
            
            cylVert[ndx+10].m_pos = vec3f( plast.x, -halfhite, plast.y );
            cylVert[ndx+10].m_st = vec2f( t0last, 1.0 );
            cylVert[ndx+10].m_nrm = nlast;
            
            cylVert[ndx+11].m_pos = vec3f( plast.x, halfhite, plast.y );
            cylVert[ndx+11].m_st = vec2f( t0last, 0.0 );
            cylVert[ndx+11].m_nrm = nlast;
        }
        plast=pcurr;
        nlast=ncurr;
        stlast=stcurr;
        t0last=tval0;
    }
    
    return gbuff;
}

void luddite::gbuff_setColorConstant( GBuff *gbuff, const vec4f &color )
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
