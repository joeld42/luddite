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

// Here's where the "renderdevice" abstraction starts to leak a little bit..
// the shader classes assume a GL-like shader structure, down to the handle
// types and everything. 

namespace luddite {

class RenderDevice;
    
class Material
{
public:

    // TODO params
    
    // Material "Database"
    static void initMaterialDB( const char *resourcePath );
    static Material *getNamedMaterial( RenderDevice *device, const eastl::string &mtlName );
    
    // TODO: make private
    static Material *_materialWithKey( RenderDevice *device, const eastl::string &mtlKey );
    
    void setParam( const Param &p );
    
protected:    
    eastl::vector<Param> m_params;
    
    eastl::string m_shaderKey; // name of the shader in GLSW format
    eastl::string m_materialName; // name of the specific material instance    
    
    //DBG
public:
    int32_t m_program; // shader program    
    
private:
    bool _loadShader( const char *shaderKey );
};
        
    
} // namespace luddite

#endif
