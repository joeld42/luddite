//
//  render_device.cpp
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <luddite/common/debug.h>
#include <luddite/render/render_device.h>

using namespace luddite;

void RenderDevice::addGBatch( luddite::GBatch *gbatch )
{
    if (gbatch->m_flags & GBatchFlag_BLEND)
    {
        // Blended batch
        m_gbatchesBlend.push_back( gbatch );
    }
    else
    {
        // Normal, non-blend batch
        m_gbatches.push_back( gbatch );
    }
}

void RenderDevice::addParticleBatch( luddite::GBatch *pbatch )
{
    m_particleBatches.push_back( pbatch );
}


// empties and draws the gbuff list
void RenderDevice::renderFrame()
{
    _prepareFrame();
    
//    std::sort( m_gbuffs.begin(), m_gbuffs.end(), GBuff::cmp_ptr );
    
    
    // draw gbatches
    if (m_gbatches.size())
    {
        _enableBlendMode( false );
        
        for( std::vector<GBatch*>::iterator gbi = m_gbatches.begin();
            gbi != m_gbatches.end(); ++gbi)
        {
            // Let the device-specific subclass draw the buffer
            _drawGBatch( *gbi );
        }
    }

    // draw blend gbatches
    bool didEnableBlend = false;
    if (m_gbatchesBlend.size())
    {
        
        std::sort( m_gbatchesBlend.begin(), m_gbatchesBlend.end(),
                  [] (const luddite::GBatch *a, const luddite::GBatch *b) -> bool
                  {
                      // ensure screenspace flagged stuff goes last
                      return (a->m_flags & GBatchFlag_SCREENSPACE) <
                             (b->m_flags & GBatchFlag_SCREENSPACE);
                  });
        
        _enableBlendMode( true );
        didEnableBlend = true;
        
        // FIXME: honor blend flags
        for( std::vector<GBatch*>::iterator gbi = m_gbatchesBlend.begin();
            gbi != m_gbatchesBlend.end(); ++gbi)
        {
            // Let the device-specific subclass draw the buffer
            _drawGBatch( *gbi );
        }
    }

    
    m_gbatches.clear();
    m_gbatchesBlend.clear();

    // next, draw particle batches
    if (m_particleBatches.size())
    {
        if (!didEnableBlend)
        {
            _enableBlendMode( true );
        }
        
        for( std::vector<GBatch*>::iterator gbi = m_particleBatches.begin();
            gbi != m_particleBatches.end(); ++gbi)
        {
            // Let the device-specific subclass draw the buffer
            _drawParticleBatch( *gbi );
        }
        m_particleBatches.clear();
    }
    
    _finishFrame();
}

int32_t RenderDevice::loadShader( const std::string &shaderKey )
{
    return SHADER_FAIL;
}
