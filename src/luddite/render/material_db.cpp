//
//  material_db.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/20/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <glsw.h>
#include <rapidxml.hpp>

#include <luddite/common/debug.h>

#include <luddite/platform/platform.h>
#include <luddite/render/material.h>
#include <luddite/render/material_db.h>
#include <luddite/render/render_device.h>
#include <luddite/render/texture_info.h>

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GL/gl.h>
#endif


using namespace luddite;

uint32_t _parseWrapMode( const char *wrapModeStr );
uint32_t _parseFilterMode( const char *filterModeStr );

int countchars( const char *s, char target )
{
    int count=0;
    for (const char *ch=s; *ch; ch++)
    {
        if (*ch==target) count++;
    }
    return count;
}

void MaterialDB::initShaderDB( )
{
    initShaderDB( pfGetResourcePath().c_str() );
}

void MaterialDB::initShaderDB( const char *resourcePath )
{
    // Remember resource path to load material defs
    m_resourcePath = resourcePath;
    
    // Make sure resource Path has a trailing slash
    if (resourcePath[strlen(resourcePath)-1] != '/')
    {
        m_resourcePath = m_resourcePath + "/";
    }

    // Initialize shader path
    glswInit();
    glswSetPath( m_resourcePath.c_str(), ".glsl" );

    // For desktop GL
//    glswAddDirectiveToken("", "#define lowp");
//    glswAddDirectiveToken("", "#define mediump");
//    glswAddDirectiveToken("", "#define highp");

}

void MaterialDB::addMaterialDefs( const char *materialFile )
{
    std::string mtlFilePath = m_resourcePath + materialFile;
    
    FILE *fp = fopen( mtlFilePath.c_str(), "rb");
    if (!fp)
    {
        DBG::warn( "can't open %s\n", mtlFilePath.c_str() );
        return;
    }

    // Get size of file
    fseek(fp, 0, SEEK_END);
    size_t sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    // read the whole file
    char *xmlText = (char*)malloc(sz+1);
    fread(xmlText, sz, 1, fp);
    fclose(fp);
    
    // make sure nul-terminated
    xmlText[sz] = '\0';

    DBG::info( "About to parse xml text: \n%s\n\n", xmlText );
    
    // Parse the xml in place
    rapidxml::xml_document<> doc;    // character type defaults to char
    doc.parse<0>(xmlText);          // 0 means default parse flags

    rapidxml::xml_node<> *mtlList = doc.first_node( "MaterialList" );
    rapidxml::xml_node<> *currMtl = mtlList->first_node( "Material" );
    while (currMtl)
    {
        // Material name
        std::string mtlName;
        rapidxml::xml_attribute<> *mtlNameAttr = currMtl->first_attribute( "name" );
        if (!mtlNameAttr)
        {
            // Skip the material since we can't look it up to use it for 
            // anything without a name.
            DBG::warn( "Material missing required attribute 'name'.\n" );
            currMtl = currMtl->next_sibling( "Material" );
            continue;
        }
        mtlName = mtlNameAttr->value();
        DBG::info( "Material: %s\n", mtlName.c_str() );

        // Shader Key
        std::string shaderKey;
        rapidxml::xml_attribute<> *shaderKeyAttr = currMtl->first_attribute( "shader" );
        if (shaderKeyAttr) shaderKey = shaderKeyAttr->value();
        
        // Get the parent material if there is one
        Material *parentMtl = NULL;
        rapidxml::xml_attribute<> *parentMtlAttr = currMtl->first_attribute("parent" );
        if (parentMtlAttr)
        {
            parentMtl = _lookupMaterial( parentMtlAttr->value() );
            if (!parentMtl)
            {
                DBG::warn( "Didn't find parent material '%s' for material '%s'.\n",
                          parentMtlAttr->value(), mtlName.c_str() );
            }
        }

        // Materials need to specify either a shader or a parent. If they specify both, the shader
        // must match the parent's shader
        Material *material = NULL;
        if (parentMtl) {
            if (!shaderKey.empty() && parentMtl->shaderKey() != shaderKey) {
                DBG::warn("Material '%s' specifies shader '%s' which doesn't match parent (%s/%s)",
                        mtlName.c_str(), shaderKey.c_str(),
                        parentMtl->m_materialName.c_str(),
                        parentMtl->shaderKey().c_str() );
            }
            material = new Material( *parentMtl );
            material->m_materialName = mtlName;

//            printf( "parent material is %s\n", parentMtl->m_materialName.c_str());

            luddite::TextureInfo *texInfo = parentMtl->m_tex[0];
//            printf("parent texInfo is %p, (%s)\n", texInfo, texInfo->m_filename.c_str() );
            texInfo = material->m_tex[0];
//            printf("mtl    texInfo is %p, (%s)\n", texInfo, texInfo->m_filename.c_str() );

        } else if (!shaderKey.empty()) {
            material = _materialWithKey( shaderKey );
            material->m_materialName = mtlName;
        } else {
            DBG::error( "Material '%s' doesn't specify a shader or a parent material. Skipping.",  mtlName.c_str());
            currMtl = currMtl->next_sibling( "Material" );
            continue;
        }

        // All attrs...
//        rapidxml::xml_attribute<> *attr = currMtl->first_attribute();
//        while (attr != currMtl->last_attribute())
//        {
//            DBG::info( "Attr: %s value %s\n", attr->name(), attr->value() );
//            attr = attr->next_attribute();
//        }

        // Set blend flag (for particle materials)
        rapidxml::xml_attribute<> *attrBlend = currMtl->first_attribute( "blend" );
        if (attrBlend) {
            if (!strcmp(attrBlend->value(), "add"))
            {
                material->m_blendAdd = true;
            }
            else
            {
                DBG::warn( "Material %s: Unknown blend mode '%s'", mtlName.c_str(), attrBlend->value() );
            }
        }
        
        
        // Params
        // FIXME: make this case-insensive
        rapidxml::xml_node<> *currParam = currMtl->first_node( "param" );
        while (currParam)
        {
            rapidxml::xml_attribute<> *attrName = currParam->first_attribute( "name" );
            rapidxml::xml_attribute<> *attrValue = currParam->first_attribute( "value" );
            
            bool objectSpace = false;
            rapidxml::xml_attribute<> *attrSpace = currParam->first_attribute( "xform" );
            
            if (attrSpace)
            {
                printf("DBG: HAS AttrSpace: %s\n", attrSpace->value() );
            }
            
            if ((attrSpace) && !strcmp(attrSpace->value(), "object")) {
                objectSpace = true;
            }
            
//            printf("  param: %s value %s\n", attrName?attrName->value():"(null)", attrValue?attrValue->value():"(null)" );

            if ((attrName)&&(attrValue))
            {
                _parseParam( material, attrName->value(), attrValue->value(), objectSpace );
            }

            currParam = currParam->next_sibling("param");
        }

        // Set texture slots for the material
        rapidxml::xml_node<> *currTexture = currMtl->first_node( "texture" );
        while (currTexture)
        {
            rapidxml::xml_attribute<> *attrFilename = currTexture->first_attribute( "filename" );
            rapidxml::xml_attribute<> *attrSlot = currTexture->first_attribute( "slot" );
            rapidxml::xml_attribute<> *attrPname = currTexture->first_attribute( "pname" );
            rapidxml::xml_attribute<> *attrWrap = currTexture->first_attribute( "wrap" );
            rapidxml::xml_attribute<> *attrFilter = currTexture->first_attribute( "filter" );
            rapidxml::xml_attribute<> *attrDynamic = currTexture->first_attribute( "dynamic" );
            
            
            printf("texture: %s value %s\n", attrFilename?attrFilename->value():"(null)", attrSlot?attrSlot->value():"(null)" );

            if (attrSlot)
            {
                int slot = atoi(attrSlot->value());
                if ((attrFilename) && (slot >=0) && (slot < kMaxTextureSlot))
                {
                    TextureInfo *texInfo = lookupTexture( attrFilename->value() );
                    if ((attrDynamic) && (!strcmp(attrDynamic->value(), "true"))) {
                        texInfo->m_dynamic = true;
                    }

                    material->m_tex[slot] = texInfo;
                    
                    // Do we also have a pname?
                    if (attrPname) {
                        texInfo->m_paramName = attrPname->value();
                    }
                    
                    // Do we have a wrap mode?
                    if (attrWrap) {
                        texInfo->m_wrapMode = _parseWrapMode( attrWrap->value());
                    } else {
                        texInfo->m_wrapMode = GL_REPEAT;
                    }
                    
                    // Filter
                    if (attrFilter) {
                        texInfo->m_wrapMode = _parseFilterMode( attrFilter->value() );
                    } else {
                        texInfo->m_filterMode = TextureInfoFilter_MIPMAPPED;
                    }

                    
                }
            }

            currTexture = currTexture->next_sibling("texture");
        }


        // Add to db
        m_materials[mtlName] = material;

        // Next material in file
        currMtl = currMtl->next_sibling( "Material" );
    }
        
    
    // release the xml 
    free(xmlText );
}

void MaterialDB::_parseParam(Material *mtl, std::string const & paramName, char const *value, bool objectSpace)
{
    Param p(paramName);

    printf("paramName %s value %s\n", paramName.c_str(), value );
    
    // TODO: better value parser
    int numComma = countchars(value, ',');
    if ((value[0]=='#') && (strlen(value)==7))
    {
        // html style color like #ff00ee
        uint32_t color;
        uint8_t r,g,b;
        sscanf( value, "#%X", &color );
        r = (uint8_t)((color & 0xff0000) >> 16);
        g = (uint8_t)((color & 0xff00) >> 8);
        b = (uint8_t)(color & 0xff);


//        printf("setting param %s:%s (%s) to %d (%d %d %d)\n", mtl->m_materialName.c_str(), value,
//                paramName.c_str(), color, r, g, b );

        p = GLKVector4Make( (float)r/255.0, (float)g/255.0, (float)b/255.0, 1.0 );
    }
    else if (numComma==3)
    {
        // 4 floats
        float x,y,z,w;
        sscanf( value, "%f,%f,%f,%f", &x, &y, &z, &w );
        p = GLKVector4Make( x,y,z,w);
    }
    else if (numComma==2)
    {
        // 3 floats
        float x,y,z;
        sscanf( value, "%f,%f,%f", &x, &y, &z );
        printf("got vec3 %f %f %f\n", x, y, z );
        p = GLKVector3Make( x,y,z );
    }
    else if (numComma==1)
    {
        // 2 floats
        float x,y;
        sscanf( value, "%f,%f", &x, &y  );
        p = GLKVector2Make( x,y );
    }
    else
    {
        // Is it a single float?
        p = atof(value);
        
//        // TODO: more formats, like x,y,z
//        DBG::warn( "Unknown format: for param %s '%s'\n", paramName.c_str(), value );
    }

    // Set param space
    if (objectSpace)
    {
        p.m_space = ParamSpace_OBJECT;
    }
    
    // Add the param
    mtl->setParam( p );
}


// Looks up a specific material instance in the material list, from a
// set of xml files with parameters.
Material *MaterialDB::getNamedMaterial( RenderDevice *device, const std::string &mtlName )
{
    // Lookup the material
    Material *mtl = _lookupMaterial( mtlName );
    
    DBG::info( "getNamedMaterial, lookup material '%s'\n", mtlName.c_str() );

    if (mtl) {
        mtl->m_shader->load( device );
    }

    return mtl;
}

// Looks up a material or returns NULL if not exists
Material *MaterialDB::_lookupMaterial( const std::string &mtlName )
{
    auto mtlIter = m_materials.find(mtlName);
    if (mtlIter != m_materials.end())
    {
        return (*mtlIter).second;
    }
    return NULL;

}

// This gets the "base" material without a specific instance of parameter 
// assignments.
Material *MaterialDB::_materialWithKey( const std::string &mtlKey )
{
    // look up material, create only if not exist
    Shader *shader;
    auto pIter = m_shaders.find( mtlKey );
    if (pIter != m_shaders.end()) {
        shader = (*pIter).second;
    } else {
        // Load the shader
        shader = new Shader( mtlKey );
        m_shaders[mtlKey] = shader;
    }

    // Make a material for this shader
    Material *mtl = new Material();
    mtl->m_shader = shader;
    mtl->m_materialName = mtlKey; 

    return mtl;
}

luddite::TextureInfo *MaterialDB::lookupTexture( const std::string &filename )
{
    TextureInfo *texInfo;
    auto texIter = m_textures.find( filename );
    if (texIter != m_textures.end())
    {
        texInfo = (*texIter).second;
    }
    else
    {
        // didn't find, create new
        texInfo = new TextureInfo();
        texInfo->m_filename = filename;
        texInfo->m_width = 0;
        texInfo->m_height = 0;
        texInfo->m_dynamic = false;
        m_textures[filename] = texInfo;
    }

    return texInfo;
}


void MaterialDB::useAllShaders(RenderDevice *device)
{
    for ( auto shader : m_shaders )
    {
        // load the shader
        shader.second->load(device);
    }

    for (auto itex : m_textures )
    {
        TextureInfo *texInfo = itex.second;
        if (texInfo->m_texId==0)
        {
            // Texture not loaded yet..
            // TODO: load w/h and other stuff here ...
//            printf("Loading texture %s\n", texInfo->m_filename.c_str() );
            if (!texInfo->m_dynamic) {
                texInfo->m_texId = pfLoadTexture( texInfo->m_filename.c_str() );
            } else {
                printf("dynamic texture for %s\n", texInfo->m_filename.c_str() );
                
                // "dynamic texture". Basically just make a placeholder and trust the app to
                // put something there later.
                const int placeholderSz = 32;
                uint8_t *data = (uint8_t*)malloc( placeholderSz*placeholderSz*4 );
                for (int j=0; j < placeholderSz; j++) {
                    for (int i=0; i < placeholderSz; i++) {
                        size_t ndx = ((j*placeholderSz)+i) * 4;
                        
                        data[ndx+0] = 0x55;
                        data[ndx+1] = ((i&1)==(j&1))?0xFF:0x00;
                        data[ndx+2] = 0x55;
                        data[ndx+3] = 0xFF;
                    }
                }
                glGenTextures(1, &(texInfo->m_texId));
                
                glBindTexture(GL_TEXTURE_2D, texInfo->m_texId  );
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, placeholderSz, placeholderSz, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
                glGenerateMipmap( GL_TEXTURE_2D);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
                
                free(data);
            }
            
            glBindTexture(GL_TEXTURE_2D, texInfo->m_texId  );
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texInfo->m_wrapMode );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texInfo->m_wrapMode );
            
            if (texInfo->m_filterMode == TextureInfoFilter_NEAREST) {
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

            } else if (texInfo->m_filterMode == TextureInfoFilter_LINEAR) {

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

            } else if (texInfo->m_filterMode == TextureInfoFilter_MIPMAPPED) {
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
            }

        }
    }
    
}


void MaterialDB::dumpMaterialDB()
{
    for (auto &mtli : m_materials)
    {
        std::string mtlName = mtli.first;
        Material *mtl = mtli.second;
        printf("---[ %15s (%18s ) ]----------\n", mtlName.c_str(), mtl->m_materialName.c_str() );
        for (auto &pi : mtl->params())
        {
            if (pi.m_paramType == ParamType_SCALAR)
            {
                printf("  %10s : %f\n", pi.m_name.c_str(), pi.m_val.scalar );
            }
            else
            {
                printf("  %10s : ...\n", pi.m_name.c_str() ); // todo more types
            }
        }
        
    }
}

#pragma mark - Helpers


uint32_t _parseWrapMode( const char *wrapModeStr )
{
    if (!strcmp( wrapModeStr, "clamp"))
    {
        return GL_CLAMP_TO_EDGE;
    }
    else // "repeat", or undefined.. use repeat
    {
        return GL_REPEAT;
    }
}

uint32_t _parseFilterMode( const char *filterModeStr )
{
    if (!strcmp( filterModeStr, "nearest")) {
        return TextureInfoFilter_NEAREST;
    } else if (!strcmp( filterModeStr, "linear")) {
        return TextureInfoFilter_LINEAR;
    } else {
        return TextureInfoFilter_MIPMAPPED;
    }
    
}




