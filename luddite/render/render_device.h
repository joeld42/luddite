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
#include <luddite/render/gbatch.h>

namespace luddite
{
    
class RenderDevice
{
public:    
    void addGBatch( luddite::GBatch *gbatch );

    // empties and draws the gbuff list
    void renderFrame();
    
protected:
    
    virtual void _drawGBatch( luddite::GBatch *gbatch )=0; 
    eastl::vector<luddite::GBatch*> m_gbatches; 
};
    
}; // namespace luddite

#endif
