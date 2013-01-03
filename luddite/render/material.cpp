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


const eastl::vector<Param>  & Material::params() {
    return m_params;
}

eastl::vector<Param>  & Material::mutable_params() {
    return m_params;
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
            printf("Setting param %s\n", p.m_name.c_str() );
            (*pi) = p;
            return;
        }
    }
    
    // Nope, add it as a new param
    printf("Param %s not found, adding\n", p.m_name.c_str() );
    m_params.push_back( p );
}


eastl::string const & Material::shaderKey()
{
    return m_shader->shaderKey();
}
