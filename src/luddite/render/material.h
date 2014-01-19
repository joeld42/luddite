//
//  shader.h
//  luddite_ios
//
//  Created by Joel Davis on 8/13/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_shader_h
#define luddite_shader_h

#include <string>

#include <luddite/render/param.h>
#include <luddite/render/shader.h>


#define kMaxTextureSlot (4)

// Here's where the "renderdevice" abstraction starts to leak a little bit..
// the shader classes assume a GL-like shader structure, down to the handle
// types and everything.


namespace luddite {

class RenderDevice;
class Shader;
class TextureInfo;

class Material
{
public:
    Material();
    
    // TODO params
    void setParam( const Param &p );

    const std::vector<Param> &params();
    std::vector<Param> &mutable_params();

    // Modify an existing param.
    // not particularly fast, need a better way to modify
    // params per-frame
    luddite::Param &param( const std::string &name );
    
//protected:    
    std::vector<Param> m_params;
    std::string m_materialName; // name of the specific material instance

    Shader *m_shader;

    luddite::TextureInfo* m_tex[kMaxTextureSlot];

public:
    const std::string &shaderKey();

private:
    bool _loadShader( const char *shaderKey );
};
        
    
} // namespace luddite

#endif
