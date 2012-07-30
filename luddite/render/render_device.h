//
//  render_device.h
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_ios_render_device_h
#define luddite_ios_render_device_h

#include <EASTL/vector.h> 
#include <luddite/render/gbuff.h>

namespace luddite
{
    
class RenderDevice
{
public:    
    void addGBuff( luddite::GBuff *gbuff );

    // empties and draws the gbuff list
    void renderFrame();
    
protected:
    
    virtual void _drawGBuff( luddite::GBuff *gbuff )=0; 
    eastl::vector<luddite::GBuff*> m_gbuffs; 
};
    
}; // namespace luddite

#endif
