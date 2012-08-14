//
//  material.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/13/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <glsw/glsw.h>

#include <luddite/render/render_device.h>
#include <luddite/render/material.h>

using namespace luddite;


void Material::initMaterialDB( const char *resourcePath )
{
    // Initialize shader path
    glswInit();
    glswSetPath( resourcePath, ".glsl" );    
}

// Looks up a specific material instance in the material list, from a 
// set of xml files with parameters.
Material *Material::getNamedMaterial( RenderDevice *device, const eastl::string &mtlName )
{    
    // TODO
    return NULL;
}

// This gets the "base" material without a specific instance of parameter 
// assignments.
Material *Material::_materialWithKey( RenderDevice *device, const eastl::string &mtlKey )
{
    // TODO: look up material, create only if not exist
    
    Material *mtl = new Material();
    mtl->m_shaderKey = mtlKey;
    mtl->m_materialName = mtlKey; 
    
    mtl->m_program = device->loadShader( mtlKey );
    
    return mtl;
}


void Material::setParam( const Param &p )
{
    // Is this parameter already in this material's 
    // parameter list?
    for (eastl::vector<Param>::iterator pi = m_params.begin();
         pi != m_params.end(); ++pi)
    {
        if ((*pi).m_name == p.m_name)
        {
            // Yep. Just replace it.
            (*pi) = p;
            return;
        }
    }
    
    // Nope, add it as a new param
    m_params.push_back( p );
}

