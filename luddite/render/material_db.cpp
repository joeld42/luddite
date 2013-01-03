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

#include <luddite/render/material.h>
#include <luddite/render/material_db.h>
#include <luddite/render/render_device.h>

using namespace luddite;

void MaterialDB::initShaderDB( const char *resourcePath )
{
    // Initialize shader path
    glswInit();
    glswSetPath( resourcePath, ".glsl" );    
    
    // Remember resource path to load material defs
    m_resourcePath = resourcePath;
}

void MaterialDB::addMaterialDefs( const char *materialFile )
{
    eastl::string mtlFilePath = m_resourcePath + materialFile;
    
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
        eastl::string mtlName;
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
        eastl::string shaderKey;
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

        // Add to db
        m_materials[mtlName] = material;

        // Next material in file
        currMtl = currMtl->next_sibling( "Material" );
    }
    
    // release the xml 
    free(xmlText );
}

void MaterialDB::_parseParam(Material *mtl, eastl::string const & paramName, char const *value)
{
    Param p(paramName);

    // TODO: better value parser
    if ((value[0]=='#') && (strlen(value)==7))
    {
        // html style color like #ff00ee
        uint32_t color;
        uint8_t r,g,b;
        sscanf( value, "#%X", &color );
        r = (color & 0xff0000) >> 16;
        g = (color & 0xff00) >> 8;
        b = (color & 0xff);


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
Material *MaterialDB::getNamedMaterial( RenderDevice *device, const eastl::string &mtlName )
{
    // Lookup the material
    Material *mtl = _lookupMaterial( mtlName );

    if (mtl) {
        mtl->m_shader->load( device );
    }

    return mtl;
}

// Looks up a material or returns NULL if not exists
Material *MaterialDB::_lookupMaterial( const eastl::string &mtlName )
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
Material *MaterialDB::_materialWithKey( const eastl::string &mtlKey )
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

void MaterialDB::useAllShaders(RenderDevice *device)
{
    printf("in useAllShaders...\n" );
    for ( auto shader : m_shaders )
    {
        shader.second->load(device);
    }
}
