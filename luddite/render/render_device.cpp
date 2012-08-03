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

void RenderDevice::addGBuff( luddite::GBuff *gbuff )
{
    m_gbuffs.push_back( gbuff );
//    eastl::push_heap(m_gbuffs.begin(), m_gbuffs.end() );
//    m_gbuff.push_heap(m_gbuff.begin(), m_gbuff.end(), m_gbuff.cmp_ptr );
}

// empties and draws the gbuff list
void RenderDevice::renderFrame()
{
    // TODO
//    DBG::info( "In renderFrame, will draw %d gbuffs\n", m_gbuffs.size()  );
    
//    eastl::sort( m_gbuffs.begin(), m_gbuffs.end(), GBuff::cmp_ptr );
    
    for( eastl::vector<GBuff*>::iterator gbi = m_gbuffs.begin();
        gbi != m_gbuffs.end(); ++gbi)
    {
        // Let the device-specific subclass draw the buffer
        _drawGBuff( *gbi );
    }
    
    m_gbuffs.clear();    
}

