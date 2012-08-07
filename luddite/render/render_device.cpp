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
    m_gbatches.push_back( gbatch );
}

// empties and draws the gbuff list
void RenderDevice::renderFrame()
{
    // TODO
    DBG::info( "In renderFrame, will draw %d gbatches\n", m_gbatches.size()  );
    
//    eastl::sort( m_gbuffs.begin(), m_gbuffs.end(), GBuff::cmp_ptr );
    
    for( eastl::vector<GBatch*>::iterator gbi = m_gbatches.begin();
        gbi != m_gbatches.end(); ++gbi)
    {
        // Let the device-specific subclass draw the buffer
        _drawGBatch( *gbi );
    }
    
    m_gbatches.clear();    
}

