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
            if (!shaderKey.empty() && parentMtl->m_shaderKey != shaderKey) {
                DBG::warn("Material '%s' specifies shader '%s' which doesn't match parent (%s/%s)",
                        mtlName.c_str(), shaderKey.c_str(),
                        parentMtl->m_materialName.c_str(),
                        parentMtl->m_shaderKey.c_str() );
            }
            material = new Material( *parentMtl );
        } else if (!shaderKey.empty()) {
            material = new Material();

//            material = _materialWithKey(hmm don't have device here, shaderKey )
        } else {
            DBG::error( "Material '%s' doesn't specify a shader or a parent material.");
        }

        // All parameters...
        rapidxml::xml_attribute<> *attr = currMtl->first_attribute();


        while (attr != currMtl->last_attribute())
        {
            DBG::info( "Attr: %s value %s\n", attr->name(), attr->value() );
            attr = attr->next_attribute();
        }

        // TODO: params


        // Add to db
        m_materials[mtlName] = material;

        // Next material in file
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
Material *MaterialDB::_materialWithKey( RenderDevice *device, const eastl::string &mtlKey )
{
    // look up material, create only if not exist
    int32_t shaderProg;
    auto pIter = m_shaderPrograms.find( mtlKey );
    if (pIter != m_shaderPrograms.end()) {
        shaderProg = (*pIter).second;
    } else {
        // Load the shader
        // HMM... FIXME: need to not do this until the shader is actually used
        shaderProg = device->loadShader( mtlKey );

        // TODO: if there's an error loading the shader, load a built-in error shader
        // if (!shaderProg) return _materialWithKey( device, "Luddite.Error" );
    }

    // Make a material for this
    Material *mtl = new Material();
    mtl->m_shaderKey = mtlKey;
    mtl->m_materialName = mtlKey; 
    
    mtl->m_program = shaderProg;

    return mtl;
}
