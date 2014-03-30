//
//  material.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/13/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <luddite/common/debug.h>

#include <luddite/render/render_device.h>
#include <luddite/render/material.h>

using namespace luddite;

Material::Material() :
    m_shader(NULL),
    m_blendAdd(false)
{
    for (int i=0; i < kMaxTextureSlot; i++)
    {
        m_tex[i] = NULL;
        m_texParam[i] = PARAM_UNINITIALIZED;
    }
}

const std::vector<Param>  & Material::params() {
    return m_params;
}

std::vector<Param>  & Material::mutable_params() {
    return m_params;
}


void Material::setParam( const Param &p )
{
    printf("mtl %s setParam: %s\n", m_materialName.c_str(), p.m_name.c_str() );
    
    // Is this parameter already in this material's 
    // parameter list?
    for (std::vector<Param>::iterator pi = m_params.begin();
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

std::string const & Material::shaderKey()
{
    return m_shader->shaderKey();
}

luddite::Param &Material::param( const std::string &targetName )
{
    for (auto &p : m_params )
    {
        if (p.m_name == targetName )
        {
            return p;
        }
    }
    
    Assert( false, "Tried to modify a non-existant param");
    return m_params[0]; // keep compiler happy
}
