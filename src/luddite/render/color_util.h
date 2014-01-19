//
//  color_util.h
//  luddite_ios
//
//  Created by Joel Davis on 8/6/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_color_util_h
#define luddite_color_util_h

#include <GLKit/GLKMath.h>

GLKVector3 hsv2rgb( const GLKVector3 &hsv );
GLKVector3 rgb2hsv( const GLKVector3 &rgb );

#endif
