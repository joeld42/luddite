//
//  material_db.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/20/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <glsw/glsw.h>
#include <rapidxml.hpp>

#include <luddite/common/debug.h>

#include <luddite/platform/platform.h>
#include <luddite/render/material.h>
#include <luddite/render/material_db.h>
#include <luddite/render/render_device.h>
#include <luddite/render/texture_info.h>


using namespace luddite;

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

            printf( "parent material is %s\n", parentMtl->m_materialName.c_str());

            luddite::TextureInfo *texInfo = parentMtl->m_tex[0];
            printf("parent texInfo is %p, (%s)\n", texInfo, texInfo->m_filename.c_str() );
            texInfo = material->m_tex[0];
            printf("mtl    texInfo is %p, (%s)\n", texInfo, texInfo->m_filename.c_str() );

        } else if (!shaderKey.empty()) {
            material = _materialWithKey( shaderKey );
        } else {
            DBG::error( "Material '%s' doesn't specify a shader or a parent material. Skipping.");
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

        // Params
        // FIXME: make this case-insensive
        rapidxml::xml_node<> *currParam = currMtl->first_node( "param" );
        while (currParam)
        {
            rapidxml::xml_attribute<> *attrName = currParam->first_attribute( "name" );
            rapidxml::xml_attribute<> *attrValue = currParam->first_attribute( "value" );
            printf("  param: %s value %s\n", attrName?attrName->value():"(null)", attrValue?attrValue->value():"(null)" );

            if ((attrName)&&(attrValue))
            {
                _parseParam( material, attrName->value(), attrValue->value() );
            }

            currParam = currParam->next_sibling("param");
        }

        // Set texture slots for the material
        rapidxml::xml_node<> *currTexture = currMtl->first_node( "texture" );
        while (currTexture)
        {
            rapidxml::xml_attribute<> *attrFilename = currTexture->first_attribute( "filename" );
            rapidxml::xml_attribute<> *attrSlot = currTexture->first_attribute( "slot" );
            printf("texture: %s value %s\n", attrFilename?attrFilename->value():"(null)", attrSlot?attrSlot->value():"(null)" );

            if (attrSlot)
            {
                int slot = atoi(attrSlot->value());
                if ((attrFilename) && (slot >=0) && (slot < kMaxTextureSlot))
                {
                    TextureInfo *texInfo = _lookupTexture( attrFilename->value() );
                    material->m_tex[slot] = texInfo;
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

void MaterialDB::_parseParam(Material *mtl, std::string const & paramName, char const *value)
{
    Param p(paramName);

    // TODO: better value parser
    if ((value[0]=='#') && (strlen(value)==7))
    {
        // html style color like #ff00ee
        uint32_t color;
        uint8_t r,g,b;
        sscanf( value, "#%X", &color );
        r = (uint8_t)((color & 0xff0000) >> 16);
        g = (uint8_t)((color & 0xff00) >> 8);
        b = (uint8_t)(color & 0xff);


        printf("setting param %s:%s (%s) to %d (%d %d %d)\n", mtl->m_materialName.c_str(), value,
                paramName.c_str(), color, r, g, b );

        p = vec4f( (float)r/255.0, (float)g/255.0, (float)b/255.0, 1.0 );
    }
    // TODO: more formats
    else
    {
        DBG::warn( "Unknown format: for param %s '%s'\n", paramName.c_str(), value );
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

luddite::TextureInfo *MaterialDB::_lookupTexture( const std::string &filename )
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
        m_textures[filename] = texInfo;
    }

    return texInfo;
}


void MaterialDB::useAllShaders(RenderDevice *device)
{
    printf("in useAllShaders...\n" );

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
            texInfo->m_texId = pfLoadTexture( texInfo->m_filename.c_str() );
        }
    }

    printf("useAllShader done...\n" );

}
