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
        rapidxml::xml_attribute<> *attr = currMtl->first_attribute();
        DBG::info( "Material: %s\n", currMtl->name() );
        while (attr != currMtl->last_attribute())
        {
            DBG::info( "Attr: %s value %s\n", attr->name(), attr->value() );
            attr = attr->next_attribute();
        }
        
        currMtl = currMtl->next_sibling( "Material" );
    }
    
    // release the xml 
    free(xmlText );
}

// Looks up a specific material instance in the material list, from a 
// set of xml files with parameters.
Material *MaterialDB::getNamedMaterial( RenderDevice *device, const eastl::string &mtlName )
{    
    // TODO
    return NULL;
}

// This gets the "base" material without a specific instance of parameter 
// assignments.
Material *MaterialDB::_materialWithKey( RenderDevice *device, const eastl::string &mtlKey )
{
    // TODO: look up material, create only if not exist
    
    Material *mtl = new Material();
    mtl->m_shaderKey = mtlKey;
    mtl->m_materialName = mtlKey; 
    
    mtl->m_program = device->loadShader( mtlKey );
    
    return mtl;
}
