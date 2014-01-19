//
//  param.h
//  luddite_ios
//
//  Created by Joel Davis on 8/13/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_param_h
#define luddite_param_h

#include <string>
#include <vector>

#include <GLKit/GLKMath.h>

namespace luddite
{

enum
{
    ParamType_SCALAR,
    ParamType_VEC2,
    ParamType_VEC3,
    ParamType_VEC4,
    ParamType_MATRIX3,
    ParamType_MATRIX4
};

// Special values for gl param
enum {
    PARAM_UNINITIALIZED = -100,
    PARAM_ERROR         = -101
};

struct Param
{
    Param();
    ~Param();
    
    Param( const std::string &name );
    
    Param( const Param &other );
    const Param &operator= (const Param &other );
    
    std::string m_name;
    int m_paramType;
    union {
        float scalar;
        float *data;
    } m_val;   
    
    // Use assignment to set type and value
    const Param &operator= (float value );
    const Param &operator= (const GLKVector2 &value );
    const Param &operator= (const GLKVector3 &value );
    const Param &operator= (const GLKVector4 &value );
    //    const Param &operator= (const matrix3x3f &value );
    const Param &operator= (const GLKMatrix4 &value );

    // param in GL shader.
    int32_t m_glParam;
    
};

} // namespace luddite

#endif
