//
//  render_device_es2.h
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_ios_render_device_es2_h
#define luddite_ios_render_device_es2_h

#include <luddite/render/render_device.h>

namespace luddite
{

class RenderDeviceES2 : public luddite::RenderDevice
{
public:
    // HACK: expose params here until mtl/shaders are set up
    GLint uparam_modelViewProjection;
    GLint uparam_normalMat;
    
    // TODO: have a camera object
    matrix4x4f matBaseModelView; // camera 
    matrix4x4f matProjection;
    
    virtual int32_t loadShader( const eastl::string &shaderKey );
    
protected:
    
    virtual void _drawGBatch( luddite::GBatch *gbatch );
    
    // TODO: put this crap in an pimpl class
    int32_t _compileShader( const char *shaderText, uint32_t shaderType );
    void _printShaderLog( int32_t program );
    void _link( int32_t program );

    void _param( const Param &p );
};
    
//    vec3f m_pos;
//    vec2f m_st;
//    vec3f m_nrm;
//    // prmath::vec3			tangents[2];
//    uint8_t	m_color[4];

enum
{
    VertexAttrib_POSITION,
    VertexAttrib_TEXCOORD,
    VertexAttrib_NORMAL,
//    VertexAttrib_TANGENT,
//    VertexAttrib_BITANGENT,
    VertexAttrib_COLOR,
};

}; // namespace luddite

#endif
