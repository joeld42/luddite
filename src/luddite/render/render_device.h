//
//  render_device.h
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_ios_render_device_h
#define luddite_ios_render_device_h

#include <vector>

#include <luddite/render/gbuff.h>
#include <luddite/render/gbatch.h>
#include <luddite/render/particle_group.h>

#define SHADER_FAIL (0)

namespace luddite
{
    
class RenderDevice
{
public:    
    void addGBatch( luddite::GBatch *gbatch );
    void addParticleBatch( luddite::GBatch *pbatch );
    
    // empties and draws the gbuff list
    void renderFrame();
    
    //--- Material support
    
    // returns an opaque handle to a shader, or SHADER_FAIL if there
    // was an error loading shader. 
    virtual int32_t loadShader( const std::string &shaderKey );
    
protected:
    
    virtual void _prepareFrame()=0;
    virtual void _drawGBatch( luddite::GBatch *gbatch )=0;
    virtual void _drawParticleBatch( luddite::GBatch *pbatch )=0;
    virtual void _finishFrame()=0;    
    virtual void _enableBlendMode( bool enabled )=0;
    
    std::vector<luddite::GBatch*> m_gbatches;
    std::vector<luddite::GBatch*> m_gbatchesBlend;
    std::vector<luddite::GBatch*> m_particleBatches;
};
    
}; // namespace luddite

#endif
