//
//  gbuff_prim.h
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_ios_gbuff_prim_h
#define luddite_ios_gbuff_prim_h

#include <GLKit/GLKMath.h>

#include <luddite/render/gbuff.h>

// Make GBuffs from primitives. Probably more useful for placeholders
// and debugging...

namespace luddite
{
    // -------------------------------
    //   Primitives
    // -------------------------------

    luddite::GBuff *gbuff_dynamic( uint32_t numVerts );

    luddite::GBuff *gbuff_grid( uint32_t divisions=4, float size=1.0, GLKVector3  center=GLKVector3Make(0.0,0.0,0.0) );
    
    luddite::GBuff *gbuff_cube( float size=1.0, GLKVector3  center=GLKVector3Make(0.0,0.0,0.0) );
    
    luddite::GBuff *gbuff_cylinder( int nsegments=12, 
                                   float radius=0.5, float height=1.0,
                                   GLKVector3 center=GLKVector3Make(0.0, 0.0, 0.0 ) );

    void gbuff_setColorConstant( GBuff *gbuff, const GLKVector4 &color );
    
}; // namespace luddite

#endif
