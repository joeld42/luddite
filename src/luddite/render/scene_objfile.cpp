//
//  scene_objfile.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/10/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//
#include <vector>

#include <GLKit/GLKmath.h>

#include <luddite/common/debug.h>
#include <luddite/render/scene_objfile.h>
#include <luddite/render/gbuff_prim.h>
#include <luddite/render/material.h>
#include <luddite/render/material_db.h>

using namespace luddite;

// A single face from the obj
struct _ObjVert {
    size_t pos, st, nrm;
};

// A group of faces sharing a material
struct _MtlGroup
{
    std::string mtlName;
    luddite::GBuff *gbuff;
};

static _MtlGroup *findOrCreateMtl( std::vector<_MtlGroup*> &mtlGroups,
                                  const char *mtlName )
{
    // try to find the material
    _MtlGroup *mtl = NULL;
    for (std::vector<_MtlGroup*>::iterator mi = mtlGroups.begin(); mi != mtlGroups.end(); ++mi )
    {
        if ( (*mi)->mtlName == mtlName )
        {
            // Found it
            mtl = (*mi);
            break;
        }
    }
    
    // If we didn't find the current material, create one
    if (!mtl)
    {
        mtl = new _MtlGroup();
        mtl->mtlName = mtlName;
        mtl->gbuff = new GBuff;
        mtlGroups.push_back(mtl);
//        printf("creating new mtl %s\n", mtlName);
    }
    
    return mtl;    
}

SceneNode *luddite::scene_objfile_named(char const *filename, RenderDevice *renderDevice, MaterialDB *mtlDB, float scale)
{
    std::string filePath = pfPathToResource( filename );
    
//    Assert( filePath.length(), "Couldn't find resource file"  );
    
    return scene_objfile( filePath.c_str(), renderDevice, mtlDB, scale );
}

// The last obj loader I will ever write. I hope. But probably not.
SceneNode *luddite::scene_objfile(char const *filename, RenderDevice *renderDevice, MaterialDB *mtlDB, float scale)
{
    // Vert, st and norms are shared for all gbuffs in the
    // obj file
    std::vector<GLKVector3> verts;
    std::vector<GLKVector3> nrms;
    std::vector<GLKVector3> sts;
    
    // TODO: default name from filename
    SceneNode *objNode = new SceneNode();    
    std::vector<_MtlGroup*> mtlGroups;
    _MtlGroup *currMtl = NULL;
    
    // Read the obj file
    FILE *fp = fopen( filename, "r" );
    char line[1024];
    char token[1024];
    
    while (fgets( line, 1024, fp ))
    {
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
            GLKVector3 v;
            sscanf( line, "%*s %f %f %f", &(v.x), &(v.y), &(v.z) );
            
            verts.push_back( GLKVector3MultiplyScalar( v, scale ) );
        }
        
        // "vt <s> <t>"
        else if (!strcmp( token, "vt"))
        {
            GLKVector3 st;
            sscanf( line, "%*s %f %f", &(st.s), &(st.t) );
            st.z = 0.0;
            sts.push_back(st);
        }
        
        // "n <x> <y> <z>"
        else if (!strcmp(token, "vn"))
        {
            GLKVector3 n;
            sscanf( line, "%*s %f %f %f", &(n.x), &(n.y), &(n.z) );
            nrms.push_back( n );
        }
        
        else if (!strcmp( token, "f" ))
        {
            // If there is no current material, create a default material
            if (!currMtl)
            {                
                currMtl = findOrCreateMtl( mtlGroups, "default" );
            }
            
            // Parse the face vert indexes
            std::vector<_ObjVert> faceVerts;
            char *ch = strtok( line+2, " " );
            while (ch)
            {
                size_t pndx = 0;
                size_t stndx = 0;
                size_t nrmndx = 0;
                
                // Count the slashes
                int slashCount=0;
                for (char *ch2=ch; *ch2; ch2++)
                    if ((*ch2)=='/') slashCount++;
                
                if (slashCount==0)
                {
                    // just pos
                    sscanf( ch, "%lu", &pndx );
                    stndx=1;
                    nrmndx=1;                    
                }
                else if (slashCount==1)
                {
                    // pos/st
                    sscanf( ch, "%lu/%lu", &pndx, &stndx );
                    nrmndx = 1;
                }
                else if (slashCount==2)
                {
                    if (strstr( ch, "//"))
                    {
                        // pos//nrm
                        sscanf( ch, "%lu//%lu", &pndx, &nrmndx );
                        stndx = 1;
                    }
                    else
                    {
                        // pos/st/nrm
                        sscanf( ch, "%lu/%lu/%lu", &pndx, &stndx, &nrmndx );
                    }
                }
                
                // Decrement the index because OBJs are 1-based
                pndx--; stndx--; nrmndx--;
                
                // Add this vert
                _ObjVert vert;
                vert.pos = pndx; vert.st = stndx; vert.nrm = nrmndx;                
                
                faceVerts.push_back( vert );
                
                // Next vert
                ch = strtok( NULL, " " );
            }
        
            // Now we have the whole face in faceVerts, convert it to a
            // fan of triangles for the batch. 
            
            // add dummy STs if none in obj file
            if (sts.size()==0)
            {
                sts.push_back( GLKVector3Make(0.0,0.0,0.0) );
            }
            
            // Add dummy norms if none in obj file
            if (nrms.size()==0)
            {
                nrms.push_back(GLKVector3Make(0.0,1.0,0.0) );
            }
            
            // Skip points or single particles
            if (faceVerts.size() >= 3 )
            {
                for (int b=1; b < (faceVerts.size()-1); b++)
                {                                    
                    DrawVert *tri = currMtl->gbuff->addTri();;
                    
                    tri[0].m_pos = verts[faceVerts[0].pos];
                    tri[0].m_st = sts[faceVerts[0].st];
                    tri[0].m_nrm = nrms[faceVerts[0].nrm];

                    tri[1].m_pos = verts[faceVerts[b].pos];
                    tri[1].m_st = sts[faceVerts[b].st];
                    tri[1].m_nrm = nrms[faceVerts[b].nrm];

                    tri[2].m_pos = verts[faceVerts[b+1].pos];
                    tri[2].m_st = sts[faceVerts[b+1].st];
                    tri[2].m_nrm = nrms[faceVerts[b+1].nrm];
                }
            }            
        }
        
        else if (!strcmp( token, "usemtl"))
        {
            // reuse token to get mtl name
            sscanf( line, "%*s %s", token );
            currMtl = findOrCreateMtl( mtlGroups, token );
        }
        
        // These are lines we don't care about, so silently ignore
        else if ( (!strcmp(token, "mtllib")) ||                 
                  (!strcmp(token, "s")) // "smoothing groups" ignore
                 )
        {
            // pass
        }
                 
        else
        {
            DBG::warn( "Unknown token in .OBJ file ['%s'], skipping line.\n", token );
        }        
    }
    
    // Now add all the submeshes to the node
    for (std::vector<_MtlGroup*>::iterator mi = mtlGroups.begin();
         mi != mtlGroups.end(); ++mi )
    {
        GBatch *mtlBatch = new GBatch();        

//        luddite::Material *mtl = _mtlDB->getNamedMaterial( _renderDevice, "mtl.one" );
        Material *mtl = mtlDB->getNamedMaterial( renderDevice, (*mi)->mtlName );        

        mtlBatch->m_gbuff = (*mi)->gbuff;
        mtlBatch->m_mtl = mtl;
        
        mtlBatch->m_gbuff->calcTangents();
        
        objNode->addGBatch( mtlBatch );
    }
    
    DBG::info( "Loaded OBJ '%s' : %d verts %d sts %d nrms -- %d submeshes\n", 
              filename, verts.size(), sts.size(), nrms.size(),
              objNode->batches().size() );
    
    return objNode;
}
