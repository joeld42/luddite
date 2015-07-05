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

class RenderDeviceGL : public luddite::RenderDevice
{
public:
    
    // HACK: expose params here until mtl/shaders are set up
    GLint uparam_modelViewProjection;
    GLint uparam_normalMat;
    
    // TODO: have a camera object
    GLKMatrix4 matBaseModelView; // camera
    GLKMatrix4 matProjection;
    
    virtual int32_t loadShader( const std::string &shaderKey );
    
protected:
    
    virtual void _prepareFrame();
    virtual void _drawGBatch( luddite::GBatch *gbatch );
    virtual void _drawParticleBatch( luddite::GBatch *pbatch );
    virtual void _finishFrame();
    virtual void _enableBlendMode( bool enabled );
    
    // TODO: put this crap in an pimpl class
    int32_t _compileShader( const char *shaderText, uint32_t shaderType );
    void _printShaderLog( int32_t program );
    void _link( int32_t program );

    void _param( const Param &p, const GLKMatrix4 &mresult );

    void _setupMaterial(GBatch *gbatch, const GLKMatrix4 &mresult, const GLKMatrix4 &mresultPMV);
    void _bindGbuffVBO(GBuff *gbuff );
    void _bindParticleVBO(GBuff *gbuff );
    void _bindGBatchTextures(GBatch * gbatch);

    void _bindDrawVertAttribs();
};
    

enum
{
    VertexAttrib_POSITION,
    VertexAttrib_TEXCOORD,
    VertexAttrib_TANGENT,
    VertexAttrib_BITANGENT,
    VertexAttrib_NORMAL,
    VertexAttrib_COLOR,
};

}; // namespace luddite

#endif
