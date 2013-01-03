//
//  shader.h
//  luddite_ios
//
//  Created by Joel Davis on 8/13/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_shader_h
#define luddite_shader_h

#include <prmath/prmath.hpp>

#include <EASTL/string.h>

#include <luddite/render/param.h>
#include "shader.h"

// Here's where the "renderdevice" abstraction starts to leak a little bit..
// the shader classes assume a GL-like shader structure, down to the handle
// types and everything. 

namespace luddite {

class RenderDevice;
class Shader;
    
class Material
{
public:

    // TODO params
    void setParam( const Param &p );

    const eastl::vector<Param> &params();
    eastl::vector<Param> &mutable_params();

//protected:    
    eastl::vector<Param> m_params;
    eastl::string m_materialName; // name of the specific material instance

    Shader *m_shader;

public:
    const eastl::string &shaderKey();

private:
    bool _loadShader( const char *shaderKey );
};
        
    
} // namespace luddite

#endif
