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


eastl::string const & Material::shaderKey()
{
    return m_shader->shaderKey();
}
