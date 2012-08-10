//
//  scene_objfile.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/10/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//
#include <prmath/prmath.hpp>

#include <EASTL/vector.h>
#include <luddite/render/scene_objfile.h>

using namespace luddite;

luddite::SceneNode *scene_objfile( const char *filename )
{
    // Vert, st and norms are shared for all gbuffs in the
    // obj file
    eastl::vector<vec3f> verts;
    eastl::vector<vec3f> nrms;
    eastl::vector<vec2f> sts;
    
    // TODO: default name from filename
    SceneNode *objNode = new SceneNode();    
    
    // Read the obj file
    FILE *fp = fopen( filename, "r" );
    char line[1024];
    char token[1024];
    
    while (!feof(fp))
    {
        fgets( line, 1024, fp );
        
        // Skip blank lines and comments
        if ((line[0]=='#') || (line[0]=='\n') || (strlen(line)<=1))
        {
            continue;
        }
        
        // Get token at start of line
        sscanf( line, "%s", token );
        
        // "o <objName>" 
        if (!strcmp( token, "o" ))
        {
            sscanf( line, "%*s %s", token );
            objNode->setName( token );
        }
        
        // "v <x> <y> <z>"
        else if (!strcmp(token, "v"))
        {
            vec3f v;
            sscanf( line, "%*s %f %f %f", &(v.x), &(v.y), &(v.z) );
            verts.push_back( v );
        }
        
        // "vt <s> <t>"
        else if (!strcmp( token, "vt"))
        {
            vec2f st;
            sscanf( line, "%*s %f %f", &(st.s), &(st.t) );
            sts.push_back(st);
        }
        
        // "n <x> <y> <z>"
        else if (!strcmp(token, "n"))
        {
            vec3f n;
            sscanf( line, "%*s %f %f %f", &(n.x), &(n.y), &(n.z) );
            nrms.push_back( n );
        }
        
        // TODO moar load
                
    }
    
    return objNode;
}
